/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2022, nymea GmbH
* Contact: contact@nymea.io
*
* This fileDescriptor is part of nymea.
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
#include "plugininfo.h"

#include <network/networkdevicediscovery.h>
#include <hardwaremanager.h>

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
                title = "Huawei FusionSolar";
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

        HuaweiFusionSolar *connection = new HuaweiFusionSolar(hostAddress, port, slaveId, this);

        connect(connection, &HuaweiFusionSolar::connectionStateChanged, this, [this, thing, connection](bool status){
            qCDebug(dcHuawei()) << "Connected changed to" << status << "for" << thing;
            if (status) {
                // Connected true will be set after successfull init
                connection->initialize();
                thing->setStateValue(huaweiInverterConnectedStateTypeId, true);
            } else {
                thing->setStateValue(huaweiInverterConnectedStateTypeId, false);
            }

            foreach (Thing *childThing, myThings().filterByParentId(thing->id())) {
                childThing->setStateValue("connected", status);
            }
        });

        connect(connection, &HuaweiFusionSolar::inverterActivePowerChanged, this, [thing](float inverterActivePower){
            qCDebug(dcHuawei()) << "Inverter power changed" << inverterActivePower * -1000.0 << "W";
            thing->setStateValue(huaweiInverterCurrentPowerStateTypeId, inverterActivePower * -1000.0);
        });

        connect(connection, &HuaweiFusionSolar::inverterDeviceStatusChanged, this, [thing](HuaweiFusionSolar::InverterDeviceStatus inverterDeviceStatus){
            qCDebug(dcHuawei()) << "Inverter device status changed" << inverterDeviceStatus;
            Q_UNUSED(thing)
        });

        connect(connection, &HuaweiFusionSolar::inverterEnergyProducedChanged, this, [thing](float inverterEnergyProduced){
            qCDebug(dcHuawei()) << "Inverter total energy produced changed" << inverterEnergyProduced << "kWh";
            thing->setStateValue(huaweiInverterTotalEnergyProducedStateTypeId, inverterEnergyProduced);
        });

        // Meter
        connect(connection, &HuaweiFusionSolar::powerMeterActivePowerChanged, this, [this, thing](qint32 powerMeterActivePower){
            Things meterThings = myThings().filterByParentId(thing->id()).filterByThingClassId(huaweiMeterThingClassId);
            if (!meterThings.isEmpty()) {
                qCDebug(dcHuawei()) << "Meter power changed" << powerMeterActivePower << "W";
                // Note: > 0 -> return, < 0 consume
                meterThings.first()->setStateValue(huaweiMeterCurrentPowerStateTypeId, -powerMeterActivePower);
            }
        });

        // Battery 1
        connect(connection, &HuaweiFusionSolar::lunaBattery1StatusChanged, this, [this, thing](HuaweiFusionSolar::BatteryDeviceStatus lunaBattery1Status){
            qCDebug(dcHuawei()) << "Battery 1 status changed" << lunaBattery1Status;
            if (lunaBattery1Status != HuaweiFusionSolar::BatteryDeviceStatusOffline) {
                // Check if w have to create the energy storage
                Things batteryThings = myThings().filterByParentId(thing->id()).filterByThingClassId(huaweiBatteryThingClassId);
                bool alreadySetUp = false;
                foreach (Thing *batteryThing, batteryThings) {
                    if (batteryThing->paramValue(huaweiBatteryThingUnitParamTypeId).toUInt() == 1) {
                        alreadySetUp = true;
                    }
                }

                if (!alreadySetUp) {
                    qCDebug(dcHuawei()) << "Set up huawei energy storage 1 for" << thing;
                    ThingDescriptor descriptor(huaweiBatteryThingClassId, "Luna 2000 Battery", QString(), thing->id());
                    ParamList params;
                    params.append(Param(huaweiBatteryThingUnitParamTypeId, 1));
                    descriptor.setParams(params);
                    emit autoThingsAppeared(ThingDescriptors() << descriptor);
                }
            }
        });

        connect(connection, &HuaweiFusionSolar::lunaBattery1PowerChanged, this, [this, thing](qint32 lunaBattery1Power){
            qCDebug(dcHuawei()) << "Battery 1 power changed" << lunaBattery1Power << "W";
            Things batteryThings = myThings().filterByParentId(thing->id()).filterByThingClassId(huaweiBatteryThingClassId).filterByParam(huaweiBatteryThingUnitParamTypeId, 1);
            if (!batteryThings.isEmpty()) {
                batteryThings.first()->setStateValue(huaweiBatteryCurrentPowerStateTypeId, lunaBattery1Power);
                if (lunaBattery1Power < 0) {
                    batteryThings.first()->setStateValue(huaweiBatteryChargingStateStateTypeId, "discharging");
                } else if (lunaBattery1Power > 0) {
                    batteryThings.first()->setStateValue(huaweiBatteryChargingStateStateTypeId, "charging");
                } else {
                    batteryThings.first()->setStateValue(huaweiBatteryChargingStateStateTypeId, "idle");
                }
            }
        });

        connect(connection, &HuaweiFusionSolar::lunaBattery1SocChanged, this, [this, thing](float lunaBattery1Soc){
            qCDebug(dcHuawei()) << "Battery 1 SOC changed" << lunaBattery1Soc << "%";
            Things batteryThings = myThings().filterByParentId(thing->id()).filterByThingClassId(huaweiBatteryThingClassId).filterByParam(huaweiBatteryThingUnitParamTypeId, 1);
            if (!batteryThings.isEmpty()) {
                batteryThings.first()->setStateValue(huaweiBatteryBatteryLevelStateTypeId, lunaBattery1Soc);
                batteryThings.first()->setStateValue(huaweiBatteryBatteryCriticalStateTypeId, lunaBattery1Soc < 10);
            }
        });

        // Battery 2
        connect(connection, &HuaweiFusionSolar::lunaBattery2StatusChanged, this, [this, thing](HuaweiFusionSolar::BatteryDeviceStatus lunaBattery1Status){
            qCDebug(dcHuawei()) << "Battery 2 status changed" << lunaBattery1Status;
            if (lunaBattery1Status != HuaweiFusionSolar::BatteryDeviceStatusOffline) {
                // Check if w have to create the energy storage
                Things batteryThings = myThings().filterByParentId(thing->id()).filterByThingClassId(huaweiBatteryThingClassId);
                bool alreadySetUp = false;
                foreach (Thing *batteryThing, batteryThings) {
                    if (batteryThing->paramValue(huaweiBatteryThingUnitParamTypeId).toUInt() == 2) {
                        alreadySetUp = true;
                    }
                }

                if (!alreadySetUp) {
                    qCDebug(dcHuawei()) << "Set up huawei energy storage 2 for" << thing;
                    ThingDescriptor descriptor(huaweiBatteryThingClassId, "Luna 2000 Battery", QString(), thing->id());
                    ParamList params;
                    params.append(Param(huaweiBatteryThingUnitParamTypeId, 2));
                    descriptor.setParams(params);
                    emit autoThingsAppeared(ThingDescriptors() << descriptor);
                }
            }
        });

        connect(connection, &HuaweiFusionSolar::lunaBattery2PowerChanged, this, [this, thing](qint32 lunaBattery2Power){
            qCDebug(dcHuawei()) << "Battery 2 power changed" << lunaBattery2Power << "W";
            Things batteryThings = myThings().filterByParentId(thing->id()).filterByThingClassId(huaweiBatteryThingClassId).filterByParam(huaweiBatteryThingUnitParamTypeId, 2);
            if (!batteryThings.isEmpty()) {
                batteryThings.first()->setStateValue(huaweiBatteryCurrentPowerStateTypeId, lunaBattery2Power);

                if (lunaBattery2Power < 0) {
                    batteryThings.first()->setStateValue(huaweiBatteryChargingStateStateTypeId, "discharging");
                } else if (lunaBattery2Power > 0) {
                    batteryThings.first()->setStateValue(huaweiBatteryChargingStateStateTypeId, "charging");
                } else {
                    batteryThings.first()->setStateValue(huaweiBatteryChargingStateStateTypeId, "idle");
                }
            }
        });

        connect(connection, &HuaweiFusionSolar::lunaBattery2SocChanged, this, [this, thing](float lunaBattery2Soc){
            qCDebug(dcHuawei()) << "Battery 2 SOC changed" << lunaBattery2Soc << "%";
            Things batteryThings = myThings().filterByParentId(thing->id()).filterByThingClassId(huaweiBatteryThingClassId).filterByParam(huaweiBatteryThingUnitParamTypeId, 2);
            if (!batteryThings.isEmpty()) {
                batteryThings.first()->setStateValue(huaweiBatteryBatteryLevelStateTypeId, lunaBattery2Soc);
                batteryThings.first()->setStateValue(huaweiBatteryBatteryCriticalStateTypeId, lunaBattery2Soc < 10);
            }
        });

        m_connections.insert(thing, connection);
        connection->connectDevice();

        // FIXME: make async and check if this is really a huawei
        info->finish(Thing::ThingErrorNoError);
    }

    if (thing->thingClassId() == huaweiMeterThingClassId) {
        // Nothing to do here, we get all information from the inverter connection
        info->finish(Thing::ThingErrorNoError);
        Thing *parentThing = myThings().findById(thing->parentId());
        if (parentThing) {
            thing->setStateValue("connected", parentThing->stateValue("connected").toBool());
        }

    }

    if (thing->thingClassId() == huaweiBatteryThingClassId) {
        // Nothing to do here, we get all information from the inverter connection
        info->finish(Thing::ThingErrorNoError);
        Thing *parentThing = myThings().findById(thing->parentId());
        if (parentThing) {
            thing->setStateValue("connected", parentThing->stateValue("connected").toBool());
        }
    }
}

void IntegrationPluginHuawei::postSetupThing(Thing *thing)
{
    if (thing->thingClassId() == huaweiInverterThingClassId) {
        if (!m_pluginTimer) {
            qCDebug(dcHuawei()) << "Starting plugin timer...";
            m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(5);
            connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
                foreach(HuaweiFusionSolar *connection, m_connections) {
                    if (connection->connected()) {
                        connection->update();
                    }
                }
            });

            m_pluginTimer->start();
        }

        // Check if w have to set up a child meter for this inverter connection
        if (myThings().filterByParentId(thing->id()).filterByThingClassId(huaweiMeterThingClassId).isEmpty()) {
            qCDebug(dcHuawei()) << "Set up huawei meter for" << thing;
            emit autoThingsAppeared(ThingDescriptors() << ThingDescriptor(huaweiMeterThingClassId, "Huawei Power Meter", QString(), thing->id()));
        }
    }
}

void IntegrationPluginHuawei::thingRemoved(Thing *thing)
{
    if (thing->thingClassId() == huaweiInverterThingClassId && m_connections.contains(thing)) {
        m_connections.take(thing)->deleteLater();
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


