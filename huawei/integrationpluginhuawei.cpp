/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2021, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea.
* This project including source code and documentation is protected by
* copyright law, and remains the property of nymea GmbH. All rights, including
* reproduction, publication, editing and translation, are reserved. The use of
* this project is subject to the terms of a license agreement to be concluded
* with nymea GmbH in accordance with the terms of use of nymea GmbH, available
* under https://nymea.io/license
*
* GNU Lesser General Public License Usage
* Alternatively, this project may be redistributed and/or modified under the
* terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; version 3. This project is distributed in the hope that
* it will be useful, but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this project. If not, see <https://www.gnu.org/licenses/>.
*
* For any further details and any questions please contact us under
* contact@nymea.io or see our FAQ/Licensing Information on
* https://nymea.io/license/faq
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "integrationpluginhuawei.h"

#include "network/networkdevicediscovery.h"
#include "hardwaremanager.h"
#include "plugininfo.h"

IntegrationPluginHuawei::IntegrationPluginHuawei()
{

}

void IntegrationPluginHuawei::discoverThings(ThingDiscoveryInfo *info)
{
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcHuawei()) << "The network discovery is not available on this platform.";
        info->finish(Thing::ThingErrorUnsupportedFeature, QT_TR_NOOP("The network device discovery is not available."));
        return;
    }

    NetworkDeviceDiscoveryReply *discoveryReply = hardwareManager()->networkDeviceDiscovery()->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {

            qCDebug(dcHuawei()) << "Found" << networkDeviceInfo;

            // Filter for mac manufacturer
            if (!networkDeviceInfo.macAddressManufacturer().contains("Huawei"))
                continue;

            QString title;
            if (networkDeviceInfo.hostName().isEmpty()) {
                title = networkDeviceInfo.address().toString();
            } else {
                title = networkDeviceInfo.hostName() + " (" + networkDeviceInfo.address().toString() + ")";
            }

            QString description;
            if (networkDeviceInfo.macAddressManufacturer().isEmpty()) {
                description = networkDeviceInfo.macAddress();
            } else {
                description = networkDeviceInfo.macAddress() + " (" + networkDeviceInfo.macAddressManufacturer() + ")";
            }

            ThingDescriptor descriptor(huaweiInverterThingClassId, title, description);
            ParamList params;
            params << Param(huaweiInverterThingIpAddressParamTypeId, networkDeviceInfo.address().toString());
            params << Param(huaweiInverterThingMacAddressParamTypeId, networkDeviceInfo.macAddress());
            descriptor.setParams(params);

            // Check if we already have set up this device
            Things existingThings = myThings().filterByParam(huaweiInverterThingMacAddressParamTypeId, networkDeviceInfo.macAddress());
            if (existingThings.count() == 1) {
                qCDebug(dcHuawei()) << "This connection already exists in the system:" << networkDeviceInfo;
                descriptor.setThingId(existingThings.first()->id());
            }

            info->addThingDescriptor(descriptor);
        }

        info->finish(Thing::ThingErrorNoError);
    });
}

void IntegrationPluginHuawei::startMonitoringAutoThings()
{

}

void IntegrationPluginHuawei::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcHuawei()) << "Setup" << thing << thing->params();

    if (thing->thingClassId() == huaweiInverterThingClassId) {
        QHostAddress hostAddress = QHostAddress(thing->paramValue(huaweiInverterThingIpAddressParamTypeId).toString());
        if (hostAddress.isNull()) {
            info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("No IP address given"));
            return;
        }

        uint port = thing->paramValue(huaweiInverterThingPortParamTypeId).toUInt();
        quint16 slaveId = thing->paramValue(huaweiInverterThingSlaveIdParamTypeId).toUInt();

        HuaweiModbusTcpConnection *connection = new HuaweiModbusTcpConnection(hostAddress, port, slaveId, this);
        connect(connection, &HuaweiModbusTcpConnection::initializationFinished, this, [this, thing, connection, info]{
            qCDebug(dcHuawei()) << "Connection init" << connection;

            // FIXME: check if success

            m_connections.insert(thing, connection);
            info->finish(Thing::ThingErrorNoError);

            // Set connected true
            thing->setStateValue(huaweiInverterConnectedStateTypeId, true);

            // Update registers
            connection->update();
        });

        connect(connection, &HuaweiModbusTcpConnection::connectionStateChanged, this, [thing, connection](bool status){
            qCDebug(dcHuawei()) << "Connected changed to" << status << "for" << thing;
            if (status) {
                // Connected true will be set after successfull init
                connection->initialize();
            } else {
                thing->setStateValue(huaweiInverterConnectedStateTypeId, false);

            }
        });

        connection->connectDevice();

        // FIXME: make async and check if this is really a huawei
        info->finish(Thing::ThingErrorNoError);
    }
}

void IntegrationPluginHuawei::postSetupThing(Thing *thing)
{
    Q_UNUSED(thing)
    if (thing->thingClassId() == huaweiInverterThingClassId) {
        if (!m_pluginTimer) {
            qCDebug(dcHuawei()) << "Starting plugin timer...";
            m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(10);
            connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
                foreach(HuaweiModbusTcpConnection *connection, m_connections) {
                    if (connection->connected()) {
                        connection->update();
                    }
                }
            });
        }
    }
}

void IntegrationPluginHuawei::thingRemoved(Thing *thing)
{
    if (thing->thingClassId() == huaweiInverterThingClassId && m_connections.contains(thing)) {
        HuaweiModbusTcpConnection *connection = m_connections.take(thing);
        delete connection;
    }

    if (myThings().isEmpty() && m_pluginTimer) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginHuawei::executeAction(ThingActionInfo *info)
{
    info->finish(Thing::ThingErrorNoError);
}


