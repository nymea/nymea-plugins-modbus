/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2022, nymea GmbH
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
#include "plugininfo.h"
#include "kostaldiscovery.h"

#include <network/networkdevicediscovery.h>
#include <hardwaremanager.h>

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

    // Create a discovery with the info as parent for auto deleting the object once the discovery info is done
    KostalDiscovery *discovery = new KostalDiscovery(hardwareManager()->networkDeviceDiscovery(), 1502, 71, info);
    connect(discovery, &KostalDiscovery::discoveryFinished, info, [=](){
        foreach (const KostalDiscovery::KostalDiscoveryResult &result, discovery->discoveryResults()) {

            ThingDescriptor descriptor(kostalInverterThingClassId, result.manufacturerName + " " + result.productName, "Serial: " + result.serialNumber + " - " + result.networkDeviceInfo.address().toString());
            qCDebug(dcKostal()) << "Discovered:" << descriptor.title() << descriptor.description();

            // Check if we already have set up this device
            Things existingThings = myThings().filterByParam(kostalInverterThingMacAddressParamTypeId, result.networkDeviceInfo.macAddress());
            if (existingThings.count() == 1) {
                qCDebug(dcKostal()) << "This Kostal inverter already exists in the system:" << result.networkDeviceInfo;
                descriptor.setThingId(existingThings.first()->id());
            }

            ParamList params;
            params << Param(kostalInverterThingMacAddressParamTypeId, result.networkDeviceInfo.macAddress());
            // Note: if we discover also the port and modbusaddress, we must fill them in from the discovery here, for now everywhere the defaults...
            descriptor.setParams(params);
            info->addThingDescriptor(descriptor);
        }

        info->finish(Thing::ThingErrorNoError);
    });

    // Start the discovery process
    discovery->startDiscovery();

//    NetworkDeviceDiscoveryReply *discoveryReply = hardwareManager()->networkDeviceDiscovery()->discover();
//    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
//        foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {

//            qCDebug(dcKostal()) << "Found" << networkDeviceInfo;

//            QString title;
//            if (networkDeviceInfo.hostName().isEmpty()) {
//                title = networkDeviceInfo.address().toString();
//            } else {
//                title = networkDeviceInfo.hostName() + " (" + networkDeviceInfo.address().toString() + ")";
//            }

//            QString description;
//            if (networkDeviceInfo.macAddressManufacturer().isEmpty()) {
//                description = networkDeviceInfo.macAddress();
//            } else {
//                description = networkDeviceInfo.macAddress() + " (" + networkDeviceInfo.macAddressManufacturer() + ")";
//            }

//            ThingDescriptor descriptor(kostalInverterThingClassId, title, description);
//            ParamList params;
//            params << Param(kostalInverterThingMacAddressParamTypeId, networkDeviceInfo.macAddress());
//            descriptor.setParams(params);

//            // Check if we already have set up this device
//            Things existingThings = myThings().filterByParam(kostalInverterThingMacAddressParamTypeId, networkDeviceInfo.macAddress());
//            if (existingThings.count() == 1) {
//                qCDebug(dcKostal()) << "This connection already exists in the system:" << networkDeviceInfo;
//                descriptor.setThingId(existingThings.first()->id());
//            }

//            info->addThingDescriptor(descriptor);
//        }

//        info->finish(Thing::ThingErrorNoError);
//    });
}

void IntegrationPluginKostal::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcKostal()) << "Setup" << thing << thing->params();

    // Inverter (connection)
    if (thing->thingClassId() == kostalInverterThingClassId) {

        // Handle reconfigure
        if (m_kostalConnections.contains(thing)) {
            qCDebug(dcKostal()) << "Reconfiguring existing thing" << thing->name();
            m_kostalConnections.take(thing)->deleteLater();

            if (m_monitors.contains(thing)) {
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        }

        MacAddress macAddress = MacAddress(thing->paramValue(kostalInverterThingMacAddressParamTypeId).toString());
        if (!macAddress.isValid()) {
            qCWarning(dcKostal()) << "The configured mac address is not valid" << thing->params();
            info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The MAC address is not known. Please reconfigure the thing."));
            return;
        }

        // Create the monitor
        NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(macAddress);
        m_monitors.insert(thing, monitor);

        QHostAddress address = monitor->networkDeviceInfo().address();
        if (address.isNull()) {
            qCWarning(dcKostal()) << "Cannot set up thing. The host address is not known yet. Maybe it will be available in the next run...";
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("The host address is not known yet. Trying later again."));
            return;
        }

        // Clean up in case the setup gets aborted
        connect(info, &ThingSetupInfo::aborted, monitor, [=](){
            if (m_monitors.contains(thing)) {
                qCDebug(dcKostal()) << "Unregister monitor because setup has been aborted.";
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        });

        // Wait for the monitor to be ready
        if (monitor->reachable()) {
            // Thing already reachable...let's continue with the setup
            setupKostalConnection(info);
        } else {
            qCDebug(dcKostal()) << "Waiting for the network monitor to get reachable before continue to set up the connection" << thing->name() << address.toString() << "...";
            connect(monitor, &NetworkDeviceMonitor::reachableChanged, info, [=](bool reachable){
                if (reachable) {
                    qCDebug(dcKostal()) << "The monitor for thing setup" << thing->name() << "is now reachable. Continue setup...";
                    setupKostalConnection(info);
                }
            });
        }
        return;
    }

    // Meter
    if (thing->thingClassId() == kostalMeterThingClassId) {
        // Get the parent thing and the associated connection
        Thing *connectionThing = myThings().findById(thing->parentId());
        if (!connectionThing) {
            qCWarning(dcKostal()) << "Failed to set up kostal energy meter because the parent thing with ID" << thing->parentId().toString() << "could not be found.";
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        KostalModbusTcpConnection *kostalConnection = m_kostalConnections.value(connectionThing);
        if (!kostalConnection) {
            qCWarning(dcKostal()) << "Failed to set up kostal energy meter because the connection for" << connectionThing << "does not exist.";
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        // Note: The connected state will be handled in the parent inverter thing

        // Update the meter data from the kostal connection containing all information

        connect(kostalConnection, &KostalModbusTcpConnection::powerMeterTotalActivePowerChanged, this, [thing](float powerMeterTotalActivePower){
            qCDebug(dcKostal()) << thing << "total active power changed" << powerMeterTotalActivePower << "W";
            thing->setStateValue(kostalMeterCurrentPowerStateTypeId, powerMeterTotalActivePower);
        });

        // TODO: set toal energy consumed/produced

        connect(kostalConnection, &KostalModbusTcpConnection::powerMeterTotalActivePowerChanged, this, [thing](float powerMeterTotalActivePower){
            qCDebug(dcKostal()) << thing << "total active power changed" << powerMeterTotalActivePower << "W";
            thing->setStateValue(kostalMeterCurrentPowerStateTypeId, powerMeterTotalActivePower);
        });

        // Current
        connect(kostalConnection, &KostalModbusTcpConnection::powerMeterCurrentPhase1Changed, this, [thing](float powerMeterCurrentPhase1){
            qCDebug(dcKostal()) << thing << "current phase 1 changed" << powerMeterCurrentPhase1 << "A";
            thing->setStateValue(kostalMeterCurrentPhaseAStateTypeId, powerMeterCurrentPhase1);
        });

        connect(kostalConnection, &KostalModbusTcpConnection::powerMeterCurrentPhase2Changed, this, [thing](float powerMeterCurrentPhase2){
            qCDebug(dcKostal()) << thing << "current phase 2 changed" << powerMeterCurrentPhase2 << "A";
            thing->setStateValue(kostalMeterCurrentPhaseBStateTypeId, powerMeterCurrentPhase2);
        });

        connect(kostalConnection, &KostalModbusTcpConnection::powerMeterCurrentPhase3Changed, this, [thing](float powerMeterCurrentPhase3){
            qCDebug(dcKostal()) << thing << "current phase 3 changed" << powerMeterCurrentPhase3 << "A";
            thing->setStateValue(kostalMeterCurrentPhaseCStateTypeId, powerMeterCurrentPhase3);
        });

        // Voltage
        connect(kostalConnection, &KostalModbusTcpConnection::powerMeterVoltagePhase1Changed, this, [thing](float powerMeterVoltagePhase1){
            qCDebug(dcKostal()) << thing << "voltage phase 1 changed" << powerMeterVoltagePhase1 << "V";
            thing->setStateValue(kostalMeterVoltagePhaseAStateTypeId, powerMeterVoltagePhase1);
        });

        connect(kostalConnection, &KostalModbusTcpConnection::powerMeterVoltagePhase2Changed, this, [thing](float powerMeterVoltagePhase2){
            qCDebug(dcKostal()) << thing << "voltage phase 2 changed" << powerMeterVoltagePhase2 << "V";
            thing->setStateValue(kostalMeterVoltagePhaseBStateTypeId, powerMeterVoltagePhase2);
        });

        connect(kostalConnection, &KostalModbusTcpConnection::powerMeterVoltagePhase3Changed, this, [thing](float powerMeterVoltagePhase3){
            qCDebug(dcKostal()) << thing << "voltage phase 3 changed" << powerMeterVoltagePhase3 << "V";
            thing->setStateValue(kostalMeterVoltagePhaseCStateTypeId, powerMeterVoltagePhase3);
        });

        // Power
        connect(kostalConnection, &KostalModbusTcpConnection::powerMeterActivePowerPhase1Changed, this, [thing](float powerMeterActivePowerPhase1){
            qCDebug(dcKostal()) << thing << "power phase 1 changed" << powerMeterActivePowerPhase1 << "W";
            thing->setStateValue(kostalMeterCurrentPowerPhaseAStateTypeId, powerMeterActivePowerPhase1);
        });

        connect(kostalConnection, &KostalModbusTcpConnection::powerMeterActivePowerPhase2Changed, this, [thing](float powerMeterActivePowerPhase2){
            qCDebug(dcKostal()) << thing << "power phase 2 changed" << powerMeterActivePowerPhase2 << "W";
            thing->setStateValue(kostalMeterCurrentPowerPhaseBStateTypeId, powerMeterActivePowerPhase2);
        });

        connect(kostalConnection, &KostalModbusTcpConnection::powerMeterActivePowerPhase3Changed, this, [thing](float powerMeterActivePowerPhase3){
            qCDebug(dcKostal()) << thing << "power phase 3 changed" << powerMeterActivePowerPhase3 << "W";
            thing->setStateValue(kostalMeterCurrentPowerPhaseCStateTypeId, powerMeterActivePowerPhase3);
        });

        connect(kostalConnection, &KostalModbusTcpConnection::gridFrequencyPowerMeterChanged, this, [thing](float gridFrequency){
            qCDebug(dcKostal()) << thing << "grid frequency changed" << gridFrequency << "Hz";
            thing->setStateValue(kostalMeterFrequencyStateTypeId, gridFrequency);
        });

        info->finish(Thing::ThingErrorNoError);
    }

    if (thing->thingClassId() == kostalBatteryThingClassId) {
        // Get the parent thing and the associated connection
        Thing *connectionThing = myThings().findById(thing->parentId());
        if (!connectionThing) {
            qCWarning(dcKostal()) << "Failed to set up kostal battery because the parent thing with ID" << thing->parentId().toString() << "could not be found.";
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        KostalModbusTcpConnection *kostalConnection = m_kostalConnections.value(connectionThing);
        if (!kostalConnection) {
            qCWarning(dcKostal()) << "Failed to set up kostal battery because the connection for" << connectionThing << "does not exist.";
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        // Note: The connected state will be handled in the parent inverter thing

        connect(kostalConnection, &KostalModbusTcpConnection::batteryStateOfChargeChanged, this, [thing](quint16 batteryStateOfCharge){
            qCDebug(dcKostal()) << thing << "battery SoC changed" << batteryStateOfCharge << "%";
            thing->setStateValue(kostalBatteryBatteryLevelStateTypeId, batteryStateOfCharge);
            thing->setStateValue(kostalBatteryBatteryCriticalStateTypeId, batteryStateOfCharge < 5);
        });


        connect(kostalConnection, &KostalModbusTcpConnection::batteryActualPowerChanged, this, [thing](float batteryActualPower){
            qCDebug(dcKostal()) << thing << "battery charge current changed" << batteryActualPower << "W";
            thing->setStateValue(kostalBatteryCurrentPowerStateTypeId, batteryActualPower);
            if (batteryActualPower == 0) {
                thing->setStateValue(kostalBatteryChargingStateStateTypeId, "idle");
            } else if (batteryActualPower > 0) {
                thing->setStateValue(kostalBatteryChargingStateStateTypeId, "discharging");
            } else if (batteryActualPower < 0) {
                thing->setStateValue(kostalBatteryChargingStateStateTypeId, "charging");
            }
        });

        connect(kostalConnection, &KostalModbusTcpConnection::batteryWorkCapacityChanged, this, [thing](quint32 batteryWorkCapacity){
            qCDebug(dcKostal()) << thing << "battery work capacity changed" << batteryWorkCapacity << "Wh";
            thing->setStateValue(kostalBatteryCapacityStateTypeId, batteryWorkCapacity / 1000.0); // kWh
        });

        info->finish(Thing::ThingErrorNoError);
    }
}

void IntegrationPluginKostal::postSetupThing(Thing *thing)
{
    if (thing->thingClassId() == kostalInverterThingClassId) {

        KostalModbusTcpConnection *kostalConnection = m_kostalConnections.value(thing);

        // Check if we have to create the meter for the Kostal inverter
        if (myThings().filterByParentId(thing->id()).filterByThingClassId(kostalMeterThingClassId).isEmpty()) {
            qCDebug(dcKostal()) << "--> Read block \"powerMeterValues\" registers from:" << 220 << "size:" << 38;
            QModbusReply *reply = kostalConnection->readBlockPowerMeterValues();
            if (!reply) {
                qCWarning(dcKostal()) << "Error occurred while reading block \"powerMeterValues\" registers";
                return;
            }

            if (reply->isFinished()) {
                reply->deleteLater(); // Broadcast reply returns immediatly
                return;
            }

            connect(reply, &QModbusReply::finished, this, [=](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> blockValues = unit.values();

                    bool notZero = false;
                    for (int i = 0; i < blockValues.size(); i++) {
                        if (blockValues.at(i) != 0) {
                            notZero = true;
                            break;
                        }
                    }

                    if (notZero) {
                        qCDebug(dcKostal()) << "There is a meter connected but not set up yet. Creating a meter...";
                        // No meter thing created for this inverter, lets create one with the inverter as parent
                        ThingClass meterThingClass = thingClass(kostalMeterThingClassId);
                        ThingDescriptor descriptor(kostalMeterThingClassId, meterThingClass.name(), QString(), thing->id());
                        // No params required, all we need is the connection
                        emit autoThingsAppeared(ThingDescriptors() << descriptor);
                    } else {
                        qCDebug(dcKostal()) << "There is no meter connected to the inverter" << thing;
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error){
                qCWarning(dcKostal()) << "Modbus reply error occurred while updating block \"powerMeterValues\" registers" << error << reply->errorString();
            });
        }

        // Check if we have to create the battery for the Kostal inverter
        if (myThings().filterByParentId(thing->id()).filterByThingClassId(kostalBatteryThingClassId).isEmpty()) {
            if (kostalConnection->batteryType() == KostalModbusTcpConnection::BatteryTypeNoBattery) {
                qCDebug(dcKostal()) << "There is no battery connected to the inverter" << thing;
                return;
            }

            qCDebug(dcKostal()) << "There is a battery connected but not set up yet. Creating a battery" << kostalConnection->batteryType();
            ThingClass batteryThingClass = thingClass(kostalBatteryThingClassId);
            ThingDescriptor descriptor(kostalBatteryThingClassId, kostalConnection->batteryManufacturer() + " - " + kostalConnection->batteryModelId(), QString(), thing->id());
            // No params required, all we need is the connection
            emit autoThingsAppeared(ThingDescriptors() << descriptor);
        }

        // Create the update timer if not already set up
        if (!m_pluginTimer) {
            qCDebug(dcKostal()) << "Starting plugin timer...";
            m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(4);
            connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
                foreach(KostalModbusTcpConnection *connection, m_kostalConnections) {
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
        KostalModbusTcpConnection *connection = m_kostalConnections.take(thing);
        delete connection;
    }

    // Unregister related hardware resources
    if (m_monitors.contains(thing))
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));

    if (myThings().isEmpty() && m_pluginTimer) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginKostal::setupKostalConnection(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    QHostAddress address = m_monitors.value(thing)->networkDeviceInfo().address();
    uint port = thing->paramValue(kostalInverterThingPortParamTypeId).toUInt();
    quint16 slaveId = thing->paramValue(kostalInverterThingSlaveIdParamTypeId).toUInt();

    qCDebug(dcKostal()) << "Setting up kostal on" << address.toString() << port << "unit ID:" << slaveId;
    KostalModbusTcpConnection *kostalConnection = new KostalModbusTcpConnection(address, port, slaveId, this);
    connect(info, &ThingSetupInfo::aborted, kostalConnection, &KostalModbusTcpConnection::deleteLater);

    // Reconnect on monitor reachable changed
    NetworkDeviceMonitor *monitor = m_monitors.value(thing);
    connect(monitor, &NetworkDeviceMonitor::reachableChanged, thing, [=](bool reachable){
        qCDebug(dcKostal()) << "Network device monitor reachable changed for" << thing->name() << reachable;
        if (!thing->setupComplete())
            return;

        if (reachable && !thing->stateValue("connected").toBool()) {
            kostalConnection->setHostAddress(monitor->networkDeviceInfo().address());
            kostalConnection->connectDevice();
        } else if (!reachable) {
            // Note: We disable autoreconnect explicitly and we will
            // connect the device once the monitor says it is reachable again
            kostalConnection->disconnectDevice();
        }
    });

    connect(kostalConnection, &KostalModbusTcpConnection::initializationFinished, info, [=](bool success){
        if (!success) {
            qCWarning(dcKostal()) << "Connection init finished with errors" << thing->name() << kostalConnection->hostAddress().toString();
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(monitor);
            kostalConnection->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("Could not initialize the communication with the wallbox."));
            return;
        }

        qCDebug(dcKostal()) << "Connection init finished successfully" << kostalConnection;
        m_kostalConnections.insert(thing, kostalConnection);
        info->finish(Thing::ThingErrorNoError);

        // Set connected true
        thing->setStateValue("connected", true);
        foreach (Thing *childThing, myThings().filterByParentId(thing->id())) {
            childThing->setStateValue("connected", true);
        }

        connect(kostalConnection, &KostalModbusTcpConnection::totalAcPowerChanged, thing, [thing](float totalAcPower){
            qCDebug(dcKostal()) << thing << "total AC power changed" << totalAcPower << "W";
            thing->setStateValue(kostalInverterCurrentPowerStateTypeId, - totalAcPower);
        });

        connect(kostalConnection, &KostalModbusTcpConnection::totalYieldChanged, thing, [thing](float totalYield){
            qCDebug(dcKostal()) << thing << "total yeald changed" << totalYield << "Wh";
            thing->setStateValue(kostalInverterTotalEnergyProducedStateTypeId, totalYield / 1000.0); // kWh
        });

        // Current
        connect(kostalConnection, &KostalModbusTcpConnection::currentPhase1Changed, thing, [thing](float currentPhase1){
            qCDebug(dcKostal()) << thing << "current phase 1 changed" << currentPhase1 << "A";
            thing->setStateValue(kostalInverterPhaseACurrentStateTypeId, currentPhase1); // A
        });

        connect(kostalConnection, &KostalModbusTcpConnection::currentPhase2Changed, thing, [thing](float currentPhase2){
            qCDebug(dcKostal()) << thing << "current phase 2 changed" << currentPhase2 << "A";
            thing->setStateValue(kostalInverterPhaseBCurrentStateTypeId, currentPhase2); // A
        });

        connect(kostalConnection, &KostalModbusTcpConnection::currentPhase3Changed, thing, [thing](float currentPhase3){
            qCDebug(dcKostal()) << thing << "current phase 3 changed" << currentPhase3 << "A";
            thing->setStateValue(kostalInverterPhaseCCurrentStateTypeId, currentPhase3); // A
        });

        // Voltage
        connect(kostalConnection, &KostalModbusTcpConnection::voltagePhase1Changed, thing, [thing](float voltagePhase1){
            qCDebug(dcKostal()) << thing << "voltage phase 1 changed" << voltagePhase1 << "V";
            thing->setStateValue(kostalInverterVoltagePhaseAStateTypeId, voltagePhase1);
        });

        connect(kostalConnection, &KostalModbusTcpConnection::voltagePhase2Changed, thing, [thing](float voltagePhase2){
            qCDebug(dcKostal()) << thing << "voltage phase 2 changed" << voltagePhase2 << "V";
            thing->setStateValue(kostalInverterVoltagePhaseBStateTypeId, voltagePhase2);
        });

        connect(kostalConnection, &KostalModbusTcpConnection::voltagePhase3Changed, thing, [thing](float voltagePhase3){
            qCDebug(dcKostal()) << thing << "voltage phase 3 changed" << voltagePhase3 << "V";
            thing->setStateValue(kostalInverterVoltagePhaseCStateTypeId, voltagePhase3);
        });

        // Current power
        connect(kostalConnection, &KostalModbusTcpConnection::activePowerPhase1Changed, thing, [thing](float activePowerPhase1){
            qCDebug(dcKostal()) << thing << "active power phase 1 changed" << activePowerPhase1 << "W";
            thing->setStateValue(kostalInverterCurrentPowerPhaseAStateTypeId, activePowerPhase1);
        });

        connect(kostalConnection, &KostalModbusTcpConnection::activePowerPhase2Changed, thing, [thing](float activePowerPhase2){
            qCDebug(dcKostal()) << thing << "active power phase 2 changed" << activePowerPhase2 << "W";
            thing->setStateValue(kostalInverterCurrentPowerPhaseBStateTypeId, activePowerPhase2);
        });

        connect(kostalConnection, &KostalModbusTcpConnection::activePowerPhase3Changed, thing, [thing](float activePowerPhase3){
            qCDebug(dcKostal()) << thing << "active power phase 3 changed" << activePowerPhase3 << "W";
            thing->setStateValue(kostalInverterCurrentPowerPhaseCStateTypeId, activePowerPhase3);
        });

        connect(kostalConnection, &KostalModbusTcpConnection::gridFrequencyInverterChanged, thing, [thing](float gridFrequencyInverter){
            qCDebug(dcKostal()) << thing << "grid frequency changed" << gridFrequencyInverter << "Hz";
            thing->setStateValue(kostalInverterFrequencyStateTypeId, gridFrequencyInverter);
        });

        // Update registers
        kostalConnection->update();
    });

    connect(kostalConnection, &KostalModbusTcpConnection::reachableChanged, thing, [this, thing, kostalConnection](bool reachable){
        qCDebug(dcKostal()) << "Reachable changed to" << reachable << "for" << thing;
        if (reachable) {
            // Connected true will be set after successfull init
            kostalConnection->initialize();
        } else {
            thing->setStateValue("connected", false);
            foreach (Thing *childThing, myThings().filterByParentId(thing->id())) {
                childThing->setStateValue("connected", false);
            }
        }
    });


    connect(kostalConnection, &KostalModbusTcpConnection::initializationFinished, info, [=](bool success){
        if (success) {
            thing->setStateValue("connected", true);
        } else {
            thing->setStateValue("connected", false);
            // Try once to reconnect the device
            kostalConnection->reconnectDevice();
        }
    });

    kostalConnection->connectDevice();
}
