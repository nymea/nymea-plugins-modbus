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

        // Note: The states will be handled in the parent inverter thing on updated

        info->finish(Thing::ThingErrorNoError);
        return;
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

        // Note: The states will be handled in the parent inverter thing on updated

        info->finish(Thing::ThingErrorNoError);
        return;
    }
}

void IntegrationPluginKostal::postSetupThing(Thing *thing)
{
    if (thing->thingClassId() == kostalInverterThingClassId) {

        // Create the update timer if not already set up
        if (!m_pluginTimer) {
            qCDebug(dcKostal()) << "Starting plugin timer...";
            m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(2);
            connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
                foreach(KostalModbusTcpConnection *connection, m_kostalConnections) {
                    qCDebug(dcKostal()) << "Update connection" << connection->hostAddress().toString();
                    connection->update();
                }
            });

            m_pluginTimer->start();
        }

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
                        ThingDescriptor descriptor(kostalMeterThingClassId, meterThingClass.displayName(), QString(), thing->id());
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
            QString batteryName = kostalConnection->batteryManufacturer();
            if (kostalConnection->batteryModelId() != 0) {
                batteryName += " - " + QString::number(kostalConnection->batteryModelId());
            }

            ThingClass batteryThingClass = thingClass(kostalBatteryThingClassId);
            ThingDescriptor descriptor(kostalBatteryThingClassId, batteryName, QString::number(kostalConnection->batterySerialNumber()), thing->id());
            // No params required, all we need is the connection
            emit autoThingsAppeared(ThingDescriptors() << descriptor);
        }

        return;
    }

    if (thing->thingClassId() == kostalMeterThingClassId || thing->thingClassId() == kostalBatteryThingClassId) {
        Thing *connectionThing = myThings().findById(thing->parentId());
        if (connectionThing) {
            thing->setStateValue("connected", connectionThing->stateValue("connected"));
        }

        return;
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

    connect(kostalConnection, &KostalModbusTcpConnection::initializationFinished, thing, [=](bool success){
        if (!thing->setupComplete())
            return;

        thing->setStateValue("connected", success);
        foreach (Thing *childThing, myThings().filterByParentId(thing->id())) {
            childThing->setStateValue("connected", success);
        }

        if (!success) {
            // Try once to reconnect the device
            kostalConnection->reconnectDevice();
        }
    });

    connect(kostalConnection, &KostalModbusTcpConnection::initializationFinished, info, [=](bool success){
        if (!success) {
            qCWarning(dcKostal()) << "Connection init finished with errors" << thing->name() << kostalConnection->hostAddress().toString();
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(monitor);
            kostalConnection->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("Could not initialize the communication with the inverter."));
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

        // Make sure we use the same endianness as the inverter (register size 1, so independent from endianess)
        switch (kostalConnection->modbusByteOrder()) {
        case KostalModbusTcpConnection::ByteOrderBigEndian:
            kostalConnection->setEndianness(ModbusDataUtils::ByteOrderBigEndian);
            break;
        case KostalModbusTcpConnection::ByteOrderLittleEndian:
            kostalConnection->setEndianness(ModbusDataUtils::ByteOrderLittleEndian);
            break;
        }

        connect(kostalConnection, &KostalModbusTcpConnection::updateFinished, thing, [=](){
            qCDebug(dcKostal()) << "Updated" << kostalConnection;

            // Current
            thing->setStateValue(kostalInverterPhaseACurrentStateTypeId, kostalConnection->currentPhase1());
            thing->setStateValue(kostalInverterPhaseBCurrentStateTypeId, kostalConnection->currentPhase2());
            thing->setStateValue(kostalInverterPhaseCCurrentStateTypeId, kostalConnection->currentPhase3());

            // Voltage
            thing->setStateValue(kostalInverterVoltagePhaseAStateTypeId, kostalConnection->voltagePhase1());
            thing->setStateValue(kostalInverterVoltagePhaseBStateTypeId, kostalConnection->voltagePhase2());
            thing->setStateValue(kostalInverterVoltagePhaseCStateTypeId, kostalConnection->voltagePhase3());

            // Phase power
            thing->setStateValue(kostalInverterCurrentPowerPhaseAStateTypeId, kostalConnection->activePowerPhase1());
            thing->setStateValue(kostalInverterCurrentPowerPhaseBStateTypeId, kostalConnection->activePowerPhase2());
            thing->setStateValue(kostalInverterCurrentPowerPhaseCStateTypeId, kostalConnection->activePowerPhase3());

            // Others
            thing->setStateValue(kostalInverterFrequencyStateTypeId, kostalConnection->gridFrequencyInverter());
            thing->setStateValue(kostalInverterTotalEnergyProducedStateTypeId, kostalConnection->totalYield() / 1000.0); // kWh

            // Power
            thing->setStateValue(kostalInverterCurrentPowerStateTypeId, - (kostalConnection->totalAcPower() - kostalConnection->batteryActualPower()));

            // Update the battery if available
            Things batteryThings = myThings().filterByParentId(thing->id()).filterByThingClassId(kostalBatteryThingClassId);
            if (batteryThings.count() == 1) {
                Thing *batteryThing = batteryThings.first();

                batteryThing->setStateValue(kostalBatteryVoltageStateTypeId, kostalConnection->batteryVoltage());
                batteryThing->setStateValue(kostalBatteryTemperatureStateTypeId, kostalConnection->batteryTemperature());
                batteryThing->setStateValue(kostalBatteryBatteryLevelStateTypeId, kostalConnection->batteryStateOfCharge());
                batteryThing->setStateValue(kostalBatteryBatteryCriticalStateTypeId, kostalConnection->batteryStateOfCharge() < 5);

                // Note: this is the wrong capacity, as of now not known.
                //batteryThing->setStateValue(kostalBatteryCapacityStateTypeId, kostalConnection->batteryWorkCapacity() / 1000.0); // kWh

                double batteryPower = -kostalConnection->batteryActualPower();
                batteryThing->setStateValue(kostalBatteryCurrentPowerStateTypeId, batteryPower);
                if (batteryPower == 0) {
                    batteryThing->setStateValue(kostalBatteryChargingStateStateTypeId, "idle");
                } else if (batteryPower < 0) {
                    batteryThing->setStateValue(kostalBatteryChargingStateStateTypeId, "discharging");
                } else if (batteryPower > 0) {
                    batteryThing->setStateValue(kostalBatteryChargingStateStateTypeId, "charging");
                }
            }

            // Update the meter if available
            Things meterThings = myThings().filterByParentId(thing->id()).filterByThingClassId(kostalMeterThingClassId);
            if (meterThings.count() == 1) {
                Thing *meterThing = meterThings.first();

                // Power
                meterThing->setStateValue(kostalMeterCurrentPhaseAStateTypeId, kostalConnection->powerMeterCurrentPhase1());
                meterThing->setStateValue(kostalMeterCurrentPhaseBStateTypeId, kostalConnection->powerMeterCurrentPhase2());
                meterThing->setStateValue(kostalMeterCurrentPhaseCStateTypeId, kostalConnection->powerMeterCurrentPhase3());

                // Voltage
                meterThing->setStateValue(kostalMeterVoltagePhaseAStateTypeId, kostalConnection->powerMeterVoltagePhase1());
                meterThing->setStateValue(kostalMeterVoltagePhaseBStateTypeId, kostalConnection->powerMeterVoltagePhase2());
                meterThing->setStateValue(kostalMeterVoltagePhaseCStateTypeId, kostalConnection->powerMeterVoltagePhase3());

                // Current
                meterThing->setStateValue(kostalMeterCurrentPowerPhaseAStateTypeId, kostalConnection->powerMeterActivePowerPhase1());
                meterThing->setStateValue(kostalMeterCurrentPowerPhaseBStateTypeId, kostalConnection->powerMeterActivePowerPhase2());
                meterThing->setStateValue(kostalMeterCurrentPowerPhaseCStateTypeId, kostalConnection->powerMeterActivePowerPhase3());

                meterThing->setStateValue(kostalMeterFrequencyStateTypeId, kostalConnection->gridFrequencyPowerMeter());

                meterThing->setStateValue(kostalMeterTotalEnergyConsumedStateTypeId, kostalConnection->totalHomeConsumptionFromGrid() / 1000.0); // kWh
                meterThing->setStateValue(kostalMeterTotalEnergyProducedStateTypeId, kostalConnection->totalEnergyAcToGrid() / 1000.0); // kWh

                // Set the power as last value
                meterThing->setStateValue(kostalMeterCurrentPowerStateTypeId, kostalConnection->powerMeterTotalActivePower());
            }
        });

        // Update registers
        kostalConnection->update();
    });

    kostalConnection->connectDevice();
}
