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

void IntegrationPluginKostal::startMonitoringAutoThings()
{

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
            qCDebug(dcKostal()) << "Connection init" << kostalConnection;

            // FIXME: check if success

            m_kostalConnections.insert(thing, kostalConnection);
            info->finish(Thing::ThingErrorNoError);

            // Set connected true
            thing->setStateValue(kostalInverterConnectedStateTypeId, true);
            foreach (Thing *childThing, myThings().filterByParentId(thing->id())) {
                if (childThing->thingClassId() == kostalBatteryThingClassId) {
                    childThing->setSettingValue(kostalBatteryConnectedStateTypeId, true);
                } else if (childThing->thingClassId() == kostalMeterThingClassId) {
                    childThing->setSettingValue(kostalMeterConnectedStateTypeId, true);
                }
            }

            connect(kostalConnection, &KostalConnection::totalHomeConsumptionChanged, this, [thing](float totalHomeConsumption){
                qCDebug(dcKostal()) << thing << "total home consumption changed" << totalHomeConsumption << "Wh";
                thing->setStateValue(kostalInverterTotalEnergyProducedStateTypeId, totalHomeConsumption / 1000.0); // kWh
            });

            // Total current
            connect(kostalConnection, &KostalConnection::currentPhase1Changed, this, [thing, kostalConnection](float currentPhase1){
                qCDebug(dcKostal()) << thing << "current phase 1 changed" << currentPhase1 << "A";
                thing->setStateValue(kostalInverterPhaseACurrentStateTypeId, currentPhase1); // A
                thing->setStateValue(kostalInverterTotalCurrentStateTypeId, kostalConnection->currentPhase1() + kostalConnection->currentPhase2() + kostalConnection->currentPhase3()); // A
            });

            connect(kostalConnection, &KostalConnection::currentPhase2Changed, this, [thing, kostalConnection](float currentPhase2){
                qCDebug(dcKostal()) << thing << "current phase 2 changed" << currentPhase2 << "A";
                thing->setStateValue(kostalInverterPhaseBCurrentStateTypeId, currentPhase2); // A
                thing->setStateValue(kostalInverterTotalCurrentStateTypeId, kostalConnection->currentPhase1() + kostalConnection->currentPhase2() + kostalConnection->currentPhase3()); // A
            });

            connect(kostalConnection, &KostalConnection::currentPhase3Changed, this, [thing, kostalConnection](float currentPhase3){
                qCDebug(dcKostal()) << thing << "current phase 3 changed" << currentPhase3 << "A";
                thing->setStateValue(kostalInverterPhaseCCurrentStateTypeId, currentPhase3); // A
                thing->setStateValue(kostalInverterTotalCurrentStateTypeId, kostalConnection->currentPhase1() + kostalConnection->currentPhase2() + kostalConnection->currentPhase3()); // A
            });

            connect(kostalConnection, &KostalConnection::gridFrequencyChanged, this, [thing](float gridFrequency){
                qCDebug(dcKostal()) << thing << "grid frequency changed" << gridFrequency << "Hz";
                thing->setStateValue(kostalInverterFrequencyStateTypeId, gridFrequency);
            });

            // Update registers
            kostalConnection->update();
        });

        connect(kostalConnection, &KostalConnection::connectionStateChanged, this, [this, thing, kostalConnection](bool status){
            qCDebug(dcKostal()) << "Connected changed to" << status << "for" << thing;
            if (status) {
                kostalConnection->initialize();
            } else {
                thing->setStateValue(kostalInverterConnectedStateTypeId, false);
                foreach (Thing *childThing, myThings().filterByParentId(thing->id())) {
                    if (childThing->thingClassId() == kostalBatteryThingClassId) {
                        childThing->setSettingValue(kostalBatteryConnectedStateTypeId, false);
                    } else if (childThing->thingClassId() == kostalMeterThingClassId) {
                        childThing->setSettingValue(kostalMeterConnectedStateTypeId, false);
                    }
                }
            }
        });
    }
}

void IntegrationPluginKostal::postSetupThing(Thing *thing)
{
    if (thing->thingClassId() == kostalInverterThingClassId) {
        if (!m_pluginTimer) {
            qCDebug(dcKostal()) << "Starting plugin timer...";
            m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(10);
            connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
                foreach(KostalConnection *connection, m_kostalConnections) {
                    if (connection->connected()) {
                        connection->update();
                    }
                }
            });
        }
    }
}

void IntegrationPluginKostal::thingRemoved(Thing *thing)
{
    if (thing->thingClassId() == kostalInverterThingClassId && m_kostalConnections.contains(thing)) {
        KostalConnection *connection = m_kostalConnections.take(thing);
        delete connection;
    }

    if (myThings().isEmpty() && m_pluginTimer) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginKostal::executeAction(ThingActionInfo *info)
{
    info->finish(Thing::ThingErrorNoError);
}


