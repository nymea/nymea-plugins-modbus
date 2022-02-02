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

#include "integrationpluginstiebeleltron.h"

#include "network/networkdevicediscovery.h"
#include "hardwaremanager.h"
#include "plugininfo.h"

IntegrationPluginStiebelEltron::IntegrationPluginStiebelEltron()
{

}

void IntegrationPluginStiebelEltron::discoverThings(ThingDiscoveryInfo *info)
{
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcStiebelEltron()) << "The network discovery is not available on this platform.";
        info->finish(Thing::ThingErrorUnsupportedFeature, QT_TR_NOOP("The network device discovery is not available."));
        return;
    }

    NetworkDeviceDiscoveryReply *discoveryReply = hardwareManager()->networkDeviceDiscovery()->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {

            qCDebug(dcStiebelEltron()) << "Found" << networkDeviceInfo;

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

            ThingDescriptor descriptor(stiebelEltronThingClassId, title, description);
            ParamList params;
            params << Param(stiebelEltronThingIpAddressParamTypeId, networkDeviceInfo.address().toString());
            params << Param(stiebelEltronThingMacAddressParamTypeId, networkDeviceInfo.macAddress());
            descriptor.setParams(params);

            // Check if we already have set up this device
            Things existingThings = myThings().filterByParam(stiebelEltronThingMacAddressParamTypeId, networkDeviceInfo.macAddress());
            if (existingThings.count() == 1) {
                qCDebug(dcStiebelEltron()) << "This connection already exists in the system:" << networkDeviceInfo;
                descriptor.setThingId(existingThings.first()->id());
            }

            info->addThingDescriptor(descriptor);
        }

        info->finish(Thing::ThingErrorNoError);
    });
}

void IntegrationPluginStiebelEltron::startMonitoringAutoThings()
{

}

void IntegrationPluginStiebelEltron::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcStiebelEltron()) << "Setup" << thing << thing->params();

    if (thing->thingClassId() == stiebelEltronThingClassId) {

        QHostAddress address(thing->paramValue(stiebelEltronThingIpAddressParamTypeId).toString());
        quint16 port = thing->paramValue(stiebelEltronThingPortParamTypeId).toUInt();
        quint16 slaveId = thing->paramValue(stiebelEltronThingSlaveIdParamTypeId).toUInt();

        StiebelEltronModbusConnection *connection = new StiebelEltronModbusConnection(address, port, slaveId, this);

        connection->connectDevice();


        m_connections.insert(thing, connection);
        info->finish(Thing::ThingErrorNoError);
    }


}

void IntegrationPluginStiebelEltron::postSetupThing(Thing *thing)
{
    if (thing->thingClassId() == stiebelEltronThingClassId) {
        if (!m_pluginTimer) {
            qCDebug(dcStiebelEltron()) << "Starting plugin timer...";
            m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(10);
            connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
                foreach (StiebelEltronModbusConnection *connection, m_connections) {
                    if (connection->connected()) {
                        connection->update();
                    }
                }
            });

            m_pluginTimer->start();
        }
    }
}

void IntegrationPluginStiebelEltron::thingRemoved(Thing *thing)
{
    if (thing->thingClassId() == stiebelEltronThingClassId && m_connections.contains(thing)) {
        m_connections.take(thing)->deleteLater();
    }

    if (myThings().isEmpty() && m_pluginTimer) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginStiebelEltron::executeAction(ThingActionInfo *info)
{
   info->finish(Thing::ThingErrorNoError);
}


