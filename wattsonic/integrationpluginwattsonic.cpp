// SPDX-License-Identifier: GPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of nymea-plugins-modbus.
*
* nymea-plugins-modbus is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* nymea-plugins-modbus is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with nymea-plugins-modbus. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "integrationpluginwattsonic.h"
#include "plugininfo.h"
#include "wattsonicdiscovery.h"

#include <network/networkdevicediscovery.h>
#include <hardwaremanager.h>

IntegrationPluginWattsonic::IntegrationPluginWattsonic()
{

}

void IntegrationPluginWattsonic::discoverThings(ThingDiscoveryInfo *info)
{
    if (info->thingClassId() == inverterThingClassId) {

        WattsonicDiscovery *discovery = new WattsonicDiscovery(hardwareManager()->modbusRtuResource(), info);
        connect(discovery, &WattsonicDiscovery::discoveryFinished, info, [=](bool modbusRtuMasterAvailable){

            if (!modbusRtuMasterAvailable) {
                info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("No suitable Modbus RTU connection available. Please set up a Modbus RTU master with a baudrate of 9600, 8 data bits, 1 stop bit and no parity."));
                return;
            }

            foreach (const WattsonicDiscovery::Result &result, discovery->discoveryResults()) {

                QString name = "Wattsonic " + result.inverterInfo.type + + " " + result.inverterInfo.model;
                ThingDescriptor descriptor(inverterThingClassId, name, QString("Serial number: %1").arg(result.serialNumber));
                qCDebug(dcWattsonic()) << "Discovered:" << descriptor.title() << descriptor.description();

                ParamList params {
                    {inverterThingModbusMasterUuidParamTypeId, result.modbusRtuMasterId},
                    {inverterThingSlaveAddressParamTypeId, result.slaveId},
                    {inverterThingSerialNumberParamTypeId, result.serialNumber}
                };
                descriptor.setParams(params);

                // Check if we already have set up this device
                Thing *existingThing = myThings().findByParams(params);
                if (existingThing) {
                    qCDebug(dcWattsonic()) << "This inverter already exists in the system:" << result.serialNumber;
                    descriptor.setThingId(existingThing->id());
                }

                info->addThingDescriptor(descriptor);
            }

            info->finish(Thing::ThingErrorNoError);
        });

        discovery->startDiscovery(info->params().paramValue(inverterDiscoverySlaveAddressParamTypeId).toUInt());
    }
}

void IntegrationPluginWattsonic::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcWattsonic()) << "Setup" << thing << thing->params();

    if (info->thing()->thingClassId() == inverterThingClassId) {
        if (m_connections.contains(thing)) {
            qCDebug(dcWattsonic()) << "Reconfiguring existing thing" << thing->name();
            m_connections.take(thing)->deleteLater();
        }

        setupWattsonicConnection(info);
        return;
    }

    if (info->thing()->thingClassId() == meterThingClassId) {
        info->finish(Thing::ThingErrorNoError);
        return;
    }

    if (info->thing()->thingClassId() == batteryThingClassId) {
        info->finish(Thing::ThingErrorNoError);
        return;
    }

}

void IntegrationPluginWattsonic::postSetupThing(Thing *thing)
{
    Q_UNUSED(thing)

    if (!m_pluginTimer) {
        qCDebug(dcWattsonic()) << "Starting plugin timer...";
        m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(2);
        connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
            foreach(WattsonicModbusRtuConnection *connection, m_connections) {
                qCDebug(dcWattsonic()) << "Updating connection" << connection->modbusRtuMaster()->serialPort() << connection->slaveId();
                connection->update();
            }
        });

        m_pluginTimer->start();
    }
}

void IntegrationPluginWattsonic::thingRemoved(Thing *thing)
{
    if (thing->thingClassId() == inverterThingClassId && m_connections.contains(thing)) {
        m_connections.take(thing)->deleteLater();
    }

    if (myThings().isEmpty() && m_pluginTimer) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginWattsonic::setupWattsonicConnection(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    uint slaveId = thing->paramValue(inverterThingSlaveAddressParamTypeId).toUInt();
    if (slaveId > 247 || slaveId == 0) {
        qCWarning(dcWattsonic()) << "Setup failed, slave ID is not valid" << slaveId;
        info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("The Modbus address not valid. It must be a value between 1 and 247."));
        return;
    }

    QUuid uuid = thing->paramValue(inverterThingModbusMasterUuidParamTypeId).toUuid();
    if (!hardwareManager()->modbusRtuResource()->hasModbusRtuMaster(uuid)) {
        qCWarning(dcWattsonic()) << "Setup failed, hardware manager not available";
        info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("The Modbus RTU resource is not available."));
        return;
    }

    QString inverterGenerationModeString = thing->paramValue(inverterThingInverterGenerationParamTypeId).toString();
    WattsonicInverter::InverterGenerationMode inverterGenerationMode;
    if (inverterGenerationModeString == "Gen2") {
        inverterGenerationMode = WattsonicInverter::InverterGenerationModeGen2;
    } else if (inverterGenerationModeString == "Gen3") {
        inverterGenerationMode = WattsonicInverter::InverterGenerationModeGen3;
    } else {
        inverterGenerationMode = WattsonicInverter::InverterGenerationModeAuto;
    }

    WattsonicInverter *connection = new WattsonicInverter(hardwareManager()->modbusRtuResource()->getModbusRtuMaster(uuid), slaveId, inverterGenerationMode, this);
    m_connections.insert(thing, connection);

    // Only for setup
    connect(info, &ThingSetupInfo::aborted, connection, &WattsonicInverter::deleteLater);
    connect(info, &ThingSetupInfo::aborted, this, [=](){
        m_connections.take(info->thing())->deleteLater();
    });

    connect(connection, &WattsonicInverter::customInitializationFinished, info, [=](bool success){
        qCDebug(dcWattsonic()) << "Initialization finished" << success;
        if (info->isInitialSetup() && !success) {
            m_connections.take(info->thing())->deleteLater();
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        info->finish(Thing::ThingErrorNoError);
        qCDebug(dcWattsonic) << "Firmware version:" << connection->firmwareVersion();
    });

    // Runtime connections
    connect(connection, &WattsonicInverter::reachableChanged, thing, [connection, thing, this](bool reachable){
        qCDebug(dcWattsonic()) << "Reachable state changed" << reachable;
        if (reachable) {
            connection->initialize();
        }

        setConnectedStates(thing, reachable);
    });

    connect(connection, &WattsonicInverter::reachableChanged, thing, [this, thing](bool reachable){
        setConnectedStates(thing, reachable);
    });

    connect(connection, &WattsonicInverter::updateFinished, thing, [this, connection, thing](){
        qCDebug(dcWattsonic()) << "Update finished:" << thing->name() << connection;

        // We received an update, make sure we are connected
        setConnectedStates(thing, true);

        Thing *inverter = thing;
        inverter->setStateValue(inverterCurrentPowerStateTypeId, connection->pvInputTotalPower() * -1.0);
        inverter->setStateValue(inverterTotalEnergyProducedStateTypeId, connection->totalPVGenerationFromInstallation() * 0.1);
        qCInfo(dcWattsonic()) << "Updating inverter:" << inverter->stateValue(inverterCurrentPowerStateTypeId).toDouble() << "W" << inverter->stateValue(inverterTotalEnergyProducedStateTypeId).toDouble() << "kWh";

        Things meters = myThings().filterByParentId(thing->id()).filterByThingClassId(meterThingClassId);        

        // Check if a meter is connected or not. We detect a meter by reading the totalEnergyPurchasedFromGrid totalEnergyInjectedToGrid registers,
        // As of now, there is no other way to detect the presence of the meter. Voltage is always there, even if there is no meter connected.

        // Some meters do not have the total counters, for whatever reason...
        bool meterDetected = connection->totalEnergyInjectedToGrid() != 0 ||
                             connection->totalEnergyPurchasedFromGrid() != 0 ||
                             connection->phaseAPower() != 0 ||
                             connection->phaseBPower() != 0 ||
                             connection->phaseCPower() != 0;

        if (meterDetected) {
            if (meters.isEmpty()) {
                qCInfo(dcWattsonic()) << "No energy meter set up yet but measurements detected. Creating meter thing...";
                QString parentSerialNumber = thing->paramValue(inverterThingSerialNumberParamTypeId).toString();
                QString name = "Wattsonic energy meter";
                if (!parentSerialNumber.isEmpty())
                    name.append(" (" + parentSerialNumber + ")");

                ThingDescriptor descriptor(meterThingClassId, name, QString(), thing->id());
                emit autoThingsAppeared({descriptor});
            }
        } else {
            // No meter detected
            if (!meters.isEmpty()) {
                // Remove existing meter thing
                qCInfo(dcWattsonic()) << "Meter set up yet but energy measurments detected in the registers. Removing meter thing...";
                emit autoThingDisappeared(meters.first()->id());
            }
        }

        if (!meters.isEmpty()) {
            Thing *meter = meters.first();
            meter->setStateValue(meterCurrentPowerStateTypeId, connection->totalPowerOnMeter() * -1.0);
            meter->setStateValue(meterTotalEnergyConsumedStateTypeId, connection->totalEnergyPurchasedFromGrid());
            meter->setStateValue(meterTotalEnergyProducedStateTypeId, connection->totalEnergyInjectedToGrid());
            meter->setStateValue(meterCurrentPowerPhaseAStateTypeId, connection->phaseAPower() * -1.0);
            meter->setStateValue(meterCurrentPowerPhaseBStateTypeId, connection->phaseBPower() * -1.0);
            meter->setStateValue(meterCurrentPowerPhaseCStateTypeId, connection->phaseCPower() * -1.0);
            meter->setStateValue(meterVoltagePhaseAStateTypeId, connection->gridPhaseAVoltage());
            meter->setStateValue(meterVoltagePhaseBStateTypeId, connection->gridPhaseBVoltage());
            meter->setStateValue(meterVoltagePhaseCStateTypeId, connection->gridPhaseCVoltage());
            // The phase current registers don't seem to contain proper values. Calculating ourselves instead
            // meter->setStateValue(meterCurrentPhaseAStateTypeId, connection->gridPhaseACurrent());
            // meter->setStateValue(meterCurrentPhaseBStateTypeId, connection->gridPhaseBCurrent());
            // meter->setStateValue(meterCurrentPhaseCStateTypeId, connection->gridPhaseCCurrent());
            meter->setStateValue(meterCurrentPhaseAStateTypeId, (connection->phaseAPower() * -1.0) / connection->gridPhaseAVoltage());
            meter->setStateValue(meterCurrentPhaseBStateTypeId, (connection->phaseBPower() * -1.0) / connection->gridPhaseBVoltage());
            meter->setStateValue(meterCurrentPhaseCStateTypeId, (connection->phaseCPower() * -1.0) / connection->gridPhaseCVoltage());
            qCInfo(dcWattsonic()) << "Updating meter:" << meter->stateValue(meterCurrentPowerStateTypeId).toDouble() << "W" << meter->stateValue(meterTotalEnergyProducedStateTypeId).toDouble() << "kWh";
        }

        Things batteries = myThings().filterByParentId(thing->id()).filterByThingClassId(batteryThingClassId);

        // Check if a battery is connected or not. We detect a battery by reading the DC voltage, if there is no voltage, there is no battery
        if (connection->batteryVoltageDc() > 0) {
            if (batteries.isEmpty()) {
                qCInfo(dcWattsonic()) << "No battery set up yet but DC voltage detected in the registers. Creating battery thing...";
                QString parentSerialNumber = thing->paramValue(inverterThingSerialNumberParamTypeId).toString();
                QString name = "Wattsonic energy storage";
                if (!parentSerialNumber.isEmpty())
                    name.append(" (" + parentSerialNumber + ")");

                ThingDescriptor descriptor(batteryThingClassId, name, QString(), thing->id());
                emit autoThingsAppeared({descriptor});
            }
        } else {
            if (!batteries.isEmpty()) {
                qCInfo(dcWattsonic()) << "Battery set up yet but no DC voltage detected in the registers. Removing battery thing...";
                emit autoThingDisappeared(batteries.first()->id());
            }
        }

        if (!batteries.isEmpty() && connection->SOC() > 0) {
            Thing *battery = batteries.first();
            QHash<WattsonicInverter::BatteryMode, QString> map {
                {WattsonicInverter::BatteryModeDischarge, "discharging"},
                {WattsonicInverter::BatteryModeCharge, "charging"}
            };

            battery->setStateValue(batteryChargingStateStateTypeId, map.value(connection->batteryMode()));
            battery->setStateValue(batteryCurrentPowerStateTypeId, connection->batteryPower() * -1.0);
            battery->setStateValue(batteryBatteryLevelStateTypeId, connection->SOC());
            battery->setStateValue(batteryBatteryCriticalStateTypeId, connection->SOC() < 5);
        }
    });
}

void IntegrationPluginWattsonic::setConnectedStates(Thing *thing, bool connected)
{
    thing->setStateValue("connected", connected);
    foreach (Thing *child, myThings().filterByParentId(thing->id())) {
        child->setStateValue("connected", connected);
    }
}
