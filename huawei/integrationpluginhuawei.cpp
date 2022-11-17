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

#include "huaweifusionsolardiscovery.h"
#include "integrationpluginhuawei.h"
#include "plugininfo.h"

#include <hardware/modbus/modbusrtuhardwareresource.h>
#include <hardwaremanager.h>

IntegrationPluginHuawei::IntegrationPluginHuawei()
{

}

void IntegrationPluginHuawei::discoverThings(ThingDiscoveryInfo *info)
{
    if (info->thingClassId() == huaweiFusionSolarInverterThingClassId) {
        if (!hardwareManager()->networkDeviceDiscovery()->available()) {
            qCWarning(dcHuawei()) << "The network discovery is not available on this platform.";
            info->finish(Thing::ThingErrorUnsupportedFeature, QT_TR_NOOP("The network device discovery is not available."));
            return;
        }

        // Create a discovery with the info as parent for auto deleting the object once the discovery info is done
        HuaweiFusionSolarDiscovery *discovery = new HuaweiFusionSolarDiscovery(hardwareManager()->networkDeviceDiscovery(), 502, 1, info);
        connect(discovery, &HuaweiFusionSolarDiscovery::discoveryFinished, info, [=](){
            foreach (const NetworkDeviceInfo &networkDeviceInfo, discovery->discoveryResults()) {

                ThingDescriptor descriptor(huaweiFusionSolarInverterThingClassId, QT_TR_NOOP("Huawei Solar Inverter"), networkDeviceInfo.macAddress() + " - " + networkDeviceInfo.address().toString());
                qCDebug(dcHuawei()) << "Discovered:" << descriptor.title() << descriptor.description();

                // Check if we already have set up this device
                Things existingThings = myThings().filterByParam(huaweiFusionSolarInverterThingMacAddressParamTypeId, networkDeviceInfo.macAddress());
                if (existingThings.count() == 1) {
                    qCDebug(dcHuawei()) << "This inverter already exists in the system:" << networkDeviceInfo;
                    descriptor.setThingId(existingThings.first()->id());
                }

                ParamList params;
                params << Param(huaweiFusionSolarInverterThingMacAddressParamTypeId, networkDeviceInfo.macAddress());
                // Note: if we discover also the port and modbusaddress, we must fill them in from the discovery here, for now everywhere the defaults...
                descriptor.setParams(params);
                info->addThingDescriptor(descriptor);
            }

            info->finish(Thing::ThingErrorNoError);
        });

        // Start the discovery process
        discovery->startDiscovery();

    } else if (info->thingClassId() == huaweiRtuInverterThingClassId) {
        qCDebug(dcHuawei()) << "Discovering modbus RTU resources...";
        if (hardwareManager()->modbusRtuResource()->modbusRtuMasters().isEmpty()) {
            info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("No Modbus RTU interface available. Please set up a Modbus RTU interface first."));
            return;
        }

        uint slaveAddress = info->params().paramValue(huaweiRtuInverterThingSlaveAddressParamTypeId).toUInt();
        if (slaveAddress > 254 || slaveAddress == 0) {
            info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The Modbus slave address must be a value between 1 and 254."));
            return;
        }

        foreach (ModbusRtuMaster *modbusMaster, hardwareManager()->modbusRtuResource()->modbusRtuMasters()) {
            qCDebug(dcHuawei()) << "Found RTU master resource" << modbusMaster << "connected" << modbusMaster->connected();
            if (!modbusMaster->connected())
                continue;

            ThingDescriptor descriptor(info->thingClassId(), "Huawei Inverter", QString::number(slaveAddress) + " " + modbusMaster->serialPort());
            ParamList params;
            params << Param(huaweiRtuInverterThingSlaveAddressParamTypeId, slaveAddress);
            params << Param(huaweiRtuInverterThingModbusMasterUuidParamTypeId, modbusMaster->modbusUuid());
            descriptor.setParams(params);
            info->addThingDescriptor(descriptor);
        }

        info->finish(Thing::ThingErrorNoError);
    }
}

void IntegrationPluginHuawei::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcHuawei()) << "Setup" << thing << thing->params();

    if (thing->thingClassId() == huaweiFusionSolarInverterThingClassId) {

        // Handle reconfigure
        if (m_connections.contains(thing)) {
            m_connections.take(thing)->deleteLater();

            if (m_monitors.contains(thing)) {
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        }

        // Make sure we have a valid mac address, otherwise no monitor and not auto searching is possible
        MacAddress macAddress = MacAddress(thing->paramValue(huaweiFusionSolarInverterThingMacAddressParamTypeId).toString());
        if (macAddress.isNull()) {
            qCWarning(dcHuawei()) << "Failed to set up Fusion Solar because the MAC address is not valid:" << thing->paramValue(huaweiFusionSolarInverterThingMacAddressParamTypeId).toString() << macAddress.toString();
            info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The MAC address is not vaild. Please reconfigure the device to fix this."));
            return;
        }

        // Create a monitor so we always get the correct IP in the network and see if the device is reachable without polling on our own
        NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(macAddress);
        m_monitors.insert(thing, monitor);

        // Continue with setup only if we know that the network device is reachable
        if (monitor->reachable()) {
            setupFusionSolar(info);
        } else {
            // otherwise wait until we reach the networkdevice before setting up the device
            qCDebug(dcHuawei()) << "Network device" << thing->name() << "is not reachable yet. Continue with the setup once reachable.";
            connect(monitor, &NetworkDeviceMonitor::reachableChanged, info, [=](bool reachable){
                if (reachable) {
                    qCDebug(dcHuawei()) << "Network device" << thing->name() << "is now reachable. Continue with the setup...";
                    setupFusionSolar(info);
                }
            });
        }

        return;
    }

    if (thing->thingClassId() == huaweiRtuInverterThingClassId) {

        uint address = thing->paramValue(huaweiRtuInverterThingSlaveAddressParamTypeId).toUInt();
        if (address > 254 || address == 0) {
            qCWarning(dcHuawei()) << "Setup failed, slave address is not valid" << address;
            info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("The Modbus address not valid. It must be a value between 1 and 254."));
            return;
        }

        QUuid uuid = thing->paramValue(huaweiRtuInverterThingModbusMasterUuidParamTypeId).toUuid();
        if (!hardwareManager()->modbusRtuResource()->hasModbusRtuMaster(uuid)) {
            qCWarning(dcHuawei()) << "Setup failed, hardware manager not available";
            info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("The Modbus RTU resource is not available."));
            return;
        }

        if (m_rtuConnections.contains(thing)) {
            qCDebug(dcHuawei()) << "Already have a Huawei connection for this thing. Cleaning up old connection and initializing new one...";
            delete m_rtuConnections.take(thing);
        }

        ModbusRtuMaster *rtuMaster = hardwareManager()->modbusRtuResource()->getModbusRtuMaster(uuid);
        HuaweiModbusRtuConnection *connection = new HuaweiModbusRtuConnection(rtuMaster, address, this);

        connect(connection, &HuaweiModbusRtuConnection::reachableChanged, this, [this, thing, connection](bool reachable){
            qCDebug(dcHuawei()) << thing->name() << "reachable changed" << reachable;
            if (reachable) {
                // Connected true will be set after successfull init
                connection->initialize();
            } else {
                thing->setStateValue("connected", false);
                foreach (Thing *childThing, myThings().filterByParentId(thing->id())) {
                    childThing->setStateValue("connected", false);
                }
            }
        });

        connect(connection, &HuaweiModbusRtuConnection::initializationFinished, this, [this, thing, connection](bool success){
            if (success) {
                thing->setStateValue("connected", true);
                foreach (Thing *childThing, myThings().filterByParentId(thing->id())) {
                    childThing->setStateValue("connected", true);
                }

                connection->update();
            }
        });

        connect(connection, &HuaweiModbusRtuConnection::inverterActivePowerChanged, this, [thing](float inverterActivePower){
            qCDebug(dcHuawei()) << "Inverter power changed" << inverterActivePower * -1000.0 << "W";
            thing->setStateValue(huaweiRtuInverterCurrentPowerStateTypeId, inverterActivePower * -1000.0);
        });

        connect(connection, &HuaweiModbusRtuConnection::inverterDeviceStatusChanged, this, [thing](HuaweiModbusRtuConnection::InverterDeviceStatus inverterDeviceStatus){
            qCDebug(dcHuawei()) << "Inverter device status changed" << inverterDeviceStatus;
            Q_UNUSED(thing)
        });

        connect(connection, &HuaweiModbusRtuConnection::inverterEnergyProducedChanged, this, [thing](float inverterEnergyProduced){
            qCDebug(dcHuawei()) << "Inverter total energy produced changed" << inverterEnergyProduced << "kWh";
            thing->setStateValue(huaweiRtuInverterTotalEnergyProducedStateTypeId, inverterEnergyProduced);
        });

        // Meter
        connect(connection, &HuaweiModbusRtuConnection::powerMeterActivePowerChanged, this, [this, thing](qint32 powerMeterActivePower){
            Things meterThings = myThings().filterByParentId(thing->id()).filterByThingClassId(huaweiMeterThingClassId);
            if (!meterThings.isEmpty()) {
                qCDebug(dcHuawei()) << "Meter power changed" << powerMeterActivePower << "W";
                // Note: > 0 -> return, < 0 consume
                meterThings.first()->setStateValue(huaweiMeterCurrentPowerStateTypeId, -powerMeterActivePower);
            }
        });

        // Battery 1
        connect(connection, &HuaweiModbusRtuConnection::lunaBattery1StatusChanged, this, [this, thing](HuaweiModbusRtuConnection::BatteryDeviceStatus lunaBattery1Status){
            qCDebug(dcHuawei()) << "Battery 1 status changed" << lunaBattery1Status;
            if (lunaBattery1Status != HuaweiModbusRtuConnection::BatteryDeviceStatusOffline) {
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

        connect(connection, &HuaweiModbusRtuConnection::lunaBattery1PowerChanged, this, [this, thing](qint32 lunaBattery1Power){
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

        connect(connection, &HuaweiModbusRtuConnection::lunaBattery1SocChanged, this, [this, thing](float lunaBattery1Soc){
            qCDebug(dcHuawei()) << "Battery 1 SOC changed" << lunaBattery1Soc << "%";
            Things batteryThings = myThings().filterByParentId(thing->id()).filterByThingClassId(huaweiBatteryThingClassId).filterByParam(huaweiBatteryThingUnitParamTypeId, 1);
            if (!batteryThings.isEmpty()) {
                batteryThings.first()->setStateValue(huaweiBatteryBatteryLevelStateTypeId, lunaBattery1Soc);
                batteryThings.first()->setStateValue(huaweiBatteryBatteryCriticalStateTypeId, lunaBattery1Soc < 10);
            }
        });

        // Battery 2
        connect(connection, &HuaweiModbusRtuConnection::lunaBattery2StatusChanged, this, [this, thing](HuaweiModbusRtuConnection::BatteryDeviceStatus lunaBattery1Status){
            qCDebug(dcHuawei()) << "Battery 2 status changed" << lunaBattery1Status;
            if (lunaBattery1Status != HuaweiModbusRtuConnection::BatteryDeviceStatusOffline) {
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

        connect(connection, &HuaweiModbusRtuConnection::lunaBattery2PowerChanged, this, [this, thing](qint32 lunaBattery2Power){
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

        connect(connection, &HuaweiModbusRtuConnection::lunaBattery2SocChanged, this, [this, thing](float lunaBattery2Soc){
            qCDebug(dcHuawei()) << "Battery 2 SOC changed" << lunaBattery2Soc << "%";
            Things batteryThings = myThings().filterByParentId(thing->id()).filterByThingClassId(huaweiBatteryThingClassId).filterByParam(huaweiBatteryThingUnitParamTypeId, 2);
            if (!batteryThings.isEmpty()) {
                batteryThings.first()->setStateValue(huaweiBatteryBatteryLevelStateTypeId, lunaBattery2Soc);
                batteryThings.first()->setStateValue(huaweiBatteryBatteryCriticalStateTypeId, lunaBattery2Soc < 10);
            }
        });

        m_rtuConnections.insert(thing, connection);
        connection->initialize();

        // FIXME: make async and check if this is really a huawei
        info->finish(Thing::ThingErrorNoError);
        return;
    }

    if (thing->thingClassId() == huaweiMeterThingClassId) {
        // Nothing to do here, we get all information from the inverter connection
        info->finish(Thing::ThingErrorNoError);
        Thing *parentThing = myThings().findById(thing->parentId());
        if (parentThing) {
            thing->setStateValue("connected", parentThing->stateValue("connected").toBool());
        }
        return;
    }

    if (thing->thingClassId() == huaweiBatteryThingClassId) {
        // Nothing to do here, we get all information from the inverter connection
        info->finish(Thing::ThingErrorNoError);
        Thing *parentThing = myThings().findById(thing->parentId());
        if (parentThing) {
            thing->setStateValue("connected", parentThing->stateValue("connected").toBool());
        }
        return;
    }
}

void IntegrationPluginHuawei::postSetupThing(Thing *thing)
{
    if (thing->thingClassId() == huaweiFusionSolarInverterThingClassId || thing->thingClassId() == huaweiRtuInverterThingClassId) {
        if (!m_pluginTimer) {
            qCDebug(dcHuawei()) << "Starting plugin timer...";
            m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(5);
            connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
                foreach(HuaweiFusionSolar *connection, m_connections) {
                    if (connection->connected()) {
                        connection->update();
                    }
                }

                foreach(HuaweiModbusRtuConnection *connection, m_rtuConnections) {
                    connection->update();
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
    if (thing->thingClassId() == huaweiFusionSolarInverterThingClassId && m_connections.contains(thing)) {
        m_connections.take(thing)->deleteLater();

        if (m_monitors.contains(thing)) {
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
        }
    }

    if (thing->thingClassId() == huaweiRtuInverterThingClassId && m_rtuConnections.contains(thing)) {
        m_rtuConnections.take(thing)->deleteLater();
    }

    if (myThings().isEmpty() && m_pluginTimer) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginHuawei::setupFusionSolar(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    NetworkDeviceMonitor *monitor = m_monitors.value(thing);
    uint port = thing->paramValue(huaweiFusionSolarInverterThingPortParamTypeId).toUInt();
    quint16 slaveId = thing->paramValue(huaweiFusionSolarInverterThingSlaveIdParamTypeId).toUInt();

    HuaweiFusionSolar *connection = new HuaweiFusionSolar(monitor->networkDeviceInfo().address(), port, slaveId, this);

    qCDebug(dcHuawei()) << "Finish setup huawei fusion solar dongle" << monitor->networkDeviceInfo().address().toString() << port << slaveId;

    connect(monitor, &NetworkDeviceMonitor::reachableChanged, thing, [=](bool reachable){
        if (!thing->setupComplete())
            return;

        qCDebug(dcHuawei()) << "Network device monitor for" << thing->name() << (reachable ? "is now reachable" : "is not reachable any more" );
        if (reachable) {
            // Update address and refresh
            thing->setStateValue("hostAddress", monitor->networkDeviceInfo().address().toString());
            connection->setHostAddress(monitor->networkDeviceInfo().address());
            connection->update();
        }
    });

    connect(connection, &HuaweiFusionSolar::reachableChanged, this, [this, thing, connection](bool reachable){
        qCDebug(dcHuawei()) << "Reachable changed to" << reachable << "for" << thing;
        if (reachable) {
            // Connected true will be set after successfull init
            connection->initialize();
            thing->setStateValue("connected", true);
        } else {
            thing->setStateValue("connected", false);
        }

        foreach (Thing *childThing, myThings().filterByParentId(thing->id())) {
            childThing->setStateValue("connected", reachable);
        }
    });

    connect(connection, &HuaweiFusionSolar::inverterActivePowerChanged, this, [](float inverterActivePower){
        qCDebug(dcHuawei()) << "Inverter power changed" << inverterActivePower * -1000.0 << "W";
    });

    connect(connection, &HuaweiFusionSolar::actualInverterPowerChanged, this, [thing](float actualInverterPower){
        qCDebug(dcHuawei()) << "Inverter actual power changed" << actualInverterPower << "W";
        thing->setStateValue(huaweiFusionSolarInverterCurrentPowerStateTypeId, actualInverterPower);
    });

    connect(connection, &HuaweiFusionSolar::inverterDeviceStatusChanged, this, [thing](HuaweiFusionSolar::InverterDeviceStatus inverterDeviceStatus){
        qCDebug(dcHuawei()) << "Inverter device status changed" << inverterDeviceStatus;
        Q_UNUSED(thing)
    });

    connect(connection, &HuaweiFusionSolar::inverterEnergyProducedChanged, this, [thing](float inverterEnergyProduced){
        qCDebug(dcHuawei()) << "Inverter total energy produced changed" << inverterEnergyProduced << "kWh";
        thing->setStateValue(huaweiFusionSolarInverterTotalEnergyProducedStateTypeId, inverterEnergyProduced);
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

