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

#include "integrationplugingrowatt.h"
#include "plugininfo.h"

#include <QLoggingCategory>
#include <hardware/modbus/modbusrtuhardwareresource.h>
#include <hardwaremanager.h>

Q_LOGGING_CATEGORY(dcGrowattPlugin, "GrowattPlugin")

IntegrationPluginGrowatt::IntegrationPluginGrowatt() {}

void IntegrationPluginGrowatt::discoverThings(ThingDiscoveryInfo *info)
{
    if (info->thingClassId() != growattWitInverterThingClassId) {
        info->finish(Thing::ThingErrorUnsupportedFeature);
        return;
    }

    if (hardwareManager()->modbusRtuResource()->modbusRtuMasters().isEmpty()) {
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("No Modbus RTU interface available. Please set up a Modbus RTU interface first."));
        return;
    }

    uint slaveAddress = info->params().paramValue(growattWitInverterDiscoverySlaveAddressParamTypeId).toUInt();
    if (slaveAddress == 0 || slaveAddress > 254) {
        info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The Modbus slave address must be a value between 1 and 254."));
        return;
    }

    foreach (ModbusRtuMaster *modbusMaster, hardwareManager()->modbusRtuResource()->modbusRtuMasters()) {
        if (!modbusMaster->connected())
            continue;

        ThingDescriptor descriptor(info->thingClassId(), QStringLiteral("Growatt WIT Inverter"), QStringLiteral("%1 - slave %2").arg(modbusMaster->serialPort()).arg(slaveAddress));
        ParamList params;
        params << Param(growattWitInverterThingModbusMasterUuidParamTypeId, modbusMaster->modbusUuid());
        params << Param(growattWitInverterThingSlaveAddressParamTypeId, slaveAddress);
        descriptor.setParams(params);

        Thing *existingThing = myThings().findByParams(params);
        if (existingThing)
            descriptor.setThingId(existingThing->id());

        info->addThingDescriptor(descriptor);
    }

    info->finish(Thing::ThingErrorNoError);
}

void IntegrationPluginGrowatt::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcGrowattPlugin()) << "Setup" << thing << thing->params();

    if (thing->thingClassId() == growattWitInverterThingClassId) {
        setupInverter(info);
        return;
    }

    if (thing->thingClassId() == growattMeterThingClassId || thing->thingClassId() == growattBatteryThingClassId) {
        info->finish(Thing::ThingErrorNoError);
        Thing *parentThing = myThings().findById(thing->parentId());
        if (parentThing)
            thing->setStateValue("connected", parentThing->stateValue("connected").toBool());
        return;
    }

    info->finish(Thing::ThingErrorUnsupportedFeature);
}

void IntegrationPluginGrowatt::postSetupThing(Thing *thing)
{
    if (thing->thingClassId() != growattWitInverterThingClassId)
        return;

    if (!m_pluginTimer) {
        m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(5);
        connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
            foreach (GrowattModbusRtuConnection *connection, m_connections)
                connection->update();
        });

        qCDebug(dcGrowattPlugin()) << "Starting plugin timer...";
        m_pluginTimer->start();
    }

    if (!meterThing(thing)) {
        emit autoThingsAppeared({ThingDescriptor(growattMeterThingClassId, QStringLiteral("Growatt Energy Meter"), QString(), thing->id())});
    }
}

void IntegrationPluginGrowatt::thingRemoved(Thing *thing)
{
    if (m_connections.contains(thing))
        m_connections.take(thing)->deleteLater();

    if (myThings().filterByThingClassId(growattWitInverterThingClassId).isEmpty() && m_pluginTimer) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginGrowatt::executeAction(ThingActionInfo *info)
{
    info->finish(Thing::ThingErrorUnsupportedFeature);
}

void IntegrationPluginGrowatt::setupInverter(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    uint slaveAddress = thing->paramValue(growattWitInverterThingSlaveAddressParamTypeId).toUInt();
    if (slaveAddress == 0 || slaveAddress > 254) {
        info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The Modbus slave address must be a value between 1 and 254."));
        return;
    }

    QUuid masterUuid = thing->paramValue(growattWitInverterThingModbusMasterUuidParamTypeId).toUuid();
    if (!hardwareManager()->modbusRtuResource()->hasModbusRtuMaster(masterUuid)) {
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The Modbus RTU resource is not available."));
        return;
    }

    if (m_connections.contains(thing))
        m_connections.take(thing)->deleteLater();

    auto *connection = new GrowattModbusRtuConnection(hardwareManager()->modbusRtuResource()->getModbusRtuMaster(masterUuid), slaveAddress, this);
    m_connections.insert(thing, connection);

    connect(connection, &GrowattModbusRtuConnection::reachableChanged, thing, [this, thing, connection](bool reachable) {
        qCDebug(dcGrowattPlugin()) << thing->name() << "reachable changed" << reachable;
        if (reachable) {
            connection->initialize();
            return;
        }

        setConnectedStates(thing, false);
        thing->setStateValue(growattWitInverterCurrentPowerStateTypeId, 0);

        Thing *meter = meterThing(thing);
        if (meter) {
            meter->setStateValue(growattMeterCurrentPowerStateTypeId, 0);
            meter->setStateValue(growattMeterCurrentPhaseAStateTypeId, 0);
            meter->setStateValue(growattMeterCurrentPhaseBStateTypeId, 0);
            meter->setStateValue(growattMeterCurrentPhaseCStateTypeId, 0);
            meter->setStateValue(growattMeterVoltagePhaseAStateTypeId, 0);
            meter->setStateValue(growattMeterVoltagePhaseBStateTypeId, 0);
            meter->setStateValue(growattMeterVoltagePhaseCStateTypeId, 0);
        }

        Thing *battery = batteryThing(thing);
        if (battery)
            battery->setStateValue(growattBatteryCurrentPowerStateTypeId, 0);
    });

    connect(connection, &GrowattModbusRtuConnection::initializationFinished, thing, [this, thing, connection](bool success) {
        setConnectedStates(thing, success);
        if (success) {
            qCDebug(dcGrowattPlugin()) << "Initialization successful for" << thing << connection->serialNumber() << connection->deviceTypeCode();
            connection->update();
        }
    });

    connect(connection, &GrowattModbusRtuConnection::updateFinished, thing, [this, thing, connection]() {
        if (connection->reachable())
            setConnectedStates(thing, true);
        updateInverterState(thing, connection);
        updateMeterState(thing, connection);
        updateBatteryState(thing, connection);
    });

    connection->initialize();
    info->finish(Thing::ThingErrorNoError);
}

void IntegrationPluginGrowatt::setConnectedStates(Thing *inverterThing, bool connected)
{
    inverterThing->setStateValue("connected", connected);
    foreach (Thing *childThing, myThings().filterByParentId(inverterThing->id()))
        childThing->setStateValue("connected", connected);
}

Thing *IntegrationPluginGrowatt::meterThing(Thing *inverterThing) const
{
    Things things = myThings().filterByParentId(inverterThing->id()).filterByThingClassId(growattMeterThingClassId);
    return things.isEmpty() ? nullptr : things.first();
}

Thing *IntegrationPluginGrowatt::batteryThing(Thing *inverterThing) const
{
    Things things = myThings().filterByParentId(inverterThing->id()).filterByThingClassId(growattBatteryThingClassId);
    return things.isEmpty() ? nullptr : things.first();
}

void IntegrationPluginGrowatt::updateInverterState(Thing *thing, GrowattModbusRtuConnection *connection)
{
    thing->setStateValue(growattWitInverterCurrentPowerStateTypeId, -connection->inverterOutputPower());
    thing->setStateValue(growattWitInverterTotalEnergyProducedStateTypeId, connection->totalGeneratedEnergy());
    thing->setStateValue(growattWitInverterTemperatureStateTypeId, connection->inverterTemperature());
    thing->setStateValue(growattWitInverterFrequencyStateTypeId, connection->gridFrequency());
}

void IntegrationPluginGrowatt::updateMeterState(Thing *thing, GrowattModbusRtuConnection *connection)
{
    Thing *meter = meterThing(thing);
    if (!meter)
        return;

    meter->setStateValue(growattMeterCurrentPowerStateTypeId, connection->powerToUserTotal() - connection->powerToGridTotal());
    meter->setStateValue(growattMeterTotalEnergyConsumedStateTypeId, connection->energyToUserTotal());
    meter->setStateValue(growattMeterTotalEnergyProducedStateTypeId, connection->energyToGridTotal());
    meter->setStateValue(growattMeterVoltagePhaseAStateTypeId, connection->gridPhaseRVoltageStorage());
    meter->setStateValue(growattMeterVoltagePhaseBStateTypeId, connection->gridPhaseSVoltageStorage());
    meter->setStateValue(growattMeterVoltagePhaseCStateTypeId, connection->gridPhaseTVoltageStorage());
    meter->setStateValue(growattMeterCurrentPhaseAStateTypeId, connection->gridPhaseRCurrentStorage());
    meter->setStateValue(growattMeterCurrentPhaseBStateTypeId, connection->gridPhaseSCurrentStorage());
    meter->setStateValue(growattMeterCurrentPhaseCStateTypeId, connection->gridPhaseTCurrentStorage());
}

void IntegrationPluginGrowatt::updateBatteryState(Thing *thing, GrowattModbusRtuConnection *connection)
{
    Thing *battery = batteryThing(thing);
    bool available = batteryAvailable(connection);

    if (available && !battery) {
        emit autoThingsAppeared({ThingDescriptor(growattBatteryThingClassId, QStringLiteral("Growatt Energy Storage"), QString(), thing->id())});
        return;
    }

    if (!available && battery) {
        emit autoThingDisappeared(battery->id());
        return;
    }

    if (!battery)
        return;

    float currentPower = -connection->batteryPower();
    battery->setStateValue(growattBatteryCurrentPowerStateTypeId, currentPower);
    battery->setStateValue(growattBatteryBatteryLevelStateTypeId, connection->batterySoc());
    battery->setStateValue(growattBatteryBatteryCriticalStateTypeId, connection->batterySoc() < 10);

    QString chargingState = QStringLiteral("idle");
    if (currentPower > 0) {
        chargingState = QStringLiteral("charging");
    } else if (currentPower < 0) {
        chargingState = QStringLiteral("discharging");
    }
    battery->setStateValue(growattBatteryChargingStateStateTypeId, chargingState);
}

bool IntegrationPluginGrowatt::batteryAvailable(GrowattModbusRtuConnection *connection) const
{
    return connection->batterySoc() > 0 || connection->batteryVoltage() > 0 || !qFuzzyIsNull(connection->batteryPower());
}
