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

#include "integrationpluginkostal.h"

#include "network/networkdevicediscovery.h"
#include "hardwaremanager.h"
#include "plugininfo.h"

IntegrationPluginKostal::IntegrationPluginKostal()
{

}

void IntegrationPluginKostal::discoverThings(ThingDiscoveryInfo *info)
{
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcKostal()) << "The network discovery is not available on this platform.";
        info->finish(Thing::ThingErrorUnsupportedFeature, QT_TR_NOOP("The network device discovery is not available."));
        return;
    }

    NetworkDeviceDiscoveryReply *discoveryReply = hardwareManager()->networkDeviceDiscovery()->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {

            qCDebug(dcKostal()) << "Found" << networkDeviceInfo;

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

            ThingDescriptor descriptor(kostalInverterThingClassId, title, description);
            ParamList params;
            params << Param(kostalInverterThingIpAddressParamTypeId, networkDeviceInfo.address().toString());
            params << Param(kostalInverterThingMacAddressParamTypeId, networkDeviceInfo.macAddress());
            descriptor.setParams(params);

            // Check if we already have set up this device
            Things existingThings = myThings().filterByParam(kostalInverterThingMacAddressParamTypeId, networkDeviceInfo.macAddress());
            if (existingThings.count() == 1) {
                qCDebug(dcKostal()) << "This connection already exists in the system:" << networkDeviceInfo;
                descriptor.setThingId(existingThings.first()->id());
            }

            info->addThingDescriptor(descriptor);
        }

        info->finish(Thing::ThingErrorNoError);
    });
}

void IntegrationPluginKostal::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcKostal()) << "Setup" << thing << thing->params();

    if (thing->thingClassId() == kostalInverterThingClassId) {
        QHostAddress hostAddress = QHostAddress(thing->paramValue(kostalInverterThingIpAddressParamTypeId).toString());
        if (hostAddress.isNull()) {
            info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("No IP address given"));
            return;
        }

        uint port = thing->paramValue(kostalInverterThingPortParamTypeId).toUInt();
        quint16 slaveId = thing->paramValue(kostalInverterThingSlaveIdParamTypeId).toUInt();

        KostalConnection *kostalConnection = new KostalConnection(hostAddress, port, slaveId, this);
        connect(kostalConnection, &KostalConnection::initializationFinished, this, [this, thing, kostalConnection, info]{
            qCDebug(dcKostal()) << "Connection init finished" << kostalConnection->inverterArticleNumber();
            // FIXME: check if success
            m_kostalConnections.insert(thing, kostalConnection);
            info->finish(Thing::ThingErrorNoError);

            connect(kostalConnection, &KostalConnection::totalHomeConsumptionChanged, this, [thing](float totalHomeConsumption){
                qCDebug(dcKostal()) << thing << "total home consumption changed" << totalHomeConsumption << "Wh";
                thing->setStateValue(kostalInverterTotalEnergyProducedStateTypeId, totalHomeConsumption / 1000); // kWh
            });

        });



    }
}

void IntegrationPluginKostal::postSetupThing(Thing *thing)
{
    Q_UNUSED(thing)
//    if (thing->thingClassId() == mtecThingClassId) {

//        if (!m_pluginTimer) {
//            qCDebug(dcKostal()) << "Starting plugin timer...";
//            m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(10);
//            connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
//                foreach (Thing *thing, myThings().filterByThingClassId(mtecThingClassId)) {
//                    update(thing);
//                }
//            });
//        }
//    }
}

void IntegrationPluginKostal::thingRemoved(Thing *thing)
{
    Q_UNUSED(thing)

    if (myThings().isEmpty() && m_pluginTimer) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginKostal::executeAction(ThingActionInfo *info)
{

    info->finish(Thing::ThingErrorNoError);
}

void IntegrationPluginKostal::update(Thing *thing)
{
    Q_UNUSED(thing)
}


