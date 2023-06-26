/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2023, nymea GmbH
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

                QString name = "Wattsonic hybrid inverter";
                ThingDescriptor descriptor(inverterThingClassId, name, result.serialNumber);
                qCDebug(dcWattsonic()) << "Discovered:" << descriptor.title() << descriptor.description();

                ParamList params {
                    {inverterThingModbusMasterUuidParamTypeId, result.modbusRtuMasterId},
                    {inverterThingSlaveAddressParamTypeId, result.slaveId}
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
        discovery->startDiscovery();
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
    if (thing->thingClassId() == inverterThingClassId) {
        Things meters = myThings().filterByParentId(thing->id()).filterByThingClassId(meterThingClassId);
        if (meters.isEmpty()) {
            qCInfo(dcWattsonic()) << "No energy meter set up yet. Creating thing...";
            ThingDescriptor descriptor(meterThingClassId, "Wattsonic energy meter", QString(), thing->id());
            emit autoThingsAppeared({descriptor});
        }
        Things batteries = myThings().filterByParentId(thing->id()).filterByThingClassId(batteryThingClassId);
        if (batteries.isEmpty()) {
            qCInfo(dcWattsonic()) << "No battery set up yet. Creating thing...";
            ThingDescriptor descriptor(batteryThingClassId, "Wattsonic energy storage", QString(), thing->id());
            emit autoThingsAppeared({descriptor});
        }
    }


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
        delete m_connections.take(thing);
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

    WattsonicModbusRtuConnection *connection = new WattsonicModbusRtuConnection(hardwareManager()->modbusRtuResource()->getModbusRtuMaster(uuid), slaveId, this);
    connect(info, &ThingSetupInfo::aborted, connection, &ModbusRtuMaster::deleteLater);

    m_connections.insert(thing, connection);
    connect(info, &ThingSetupInfo::aborted, this, [=](){
        m_connections.take(info->thing())->deleteLater();
    });

    connect(connection, &WattsonicModbusRtuConnection::reachableChanged, thing, [connection, thing, this](bool reachable){
        qCDebug(dcWattsonic()) << "Reachable state changed" << reachable;
        if (reachable) {
            connection->initialize();
        } else {
            thing->setStateValue("connected", false);
            foreach (Thing *child, myThings().filterByParentId(thing->id())) {
                child->setStateValue("connected", true);
            }
        }
    });

    connect(connection, &WattsonicModbusRtuConnection::initializationFinished, info, [=](bool success){
        qCDebug(dcWattsonic()) << "Initialisation finished" << success;
        if (info->isInitialSetup() && !success) {
            m_connections.take(info->thing())->deleteLater();
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        info->finish(Thing::ThingErrorNoError);

        if (success) {
            qCDebug(dcWattsonic) << "Firmware version:" << connection->firmwareVersion();
//            info->thing()->setStateValue(inverterCurrentVersionStateTypeId, compact20Connection->firmwareVersion());
        }
    });

    connect(connection, &WattsonicModbusRtuConnection::reachableChanged, thing, [=](bool reachable){
        thing->setStateValue(inverterConnectedStateTypeId, reachable);
        foreach (Thing *child, myThings().filterByParentId(thing->id())) {
            child->setStateValue("connected", reachable);
        }
    });


    connect(connection, &WattsonicModbusRtuConnection::updateFinished, thing, [this, connection, thing](){
        qCDebug(dcWattsonic()) << "Update finished:" << thing->name() << connection;

        Thing *inverter = thing;

        inverter->setStateValue(inverterCurrentPowerStateTypeId, connection->pAC() * -1.0);
        inverter->setStateValue(inverterTotalEnergyProducedStateTypeId, connection->totalPVGenerationFromInstallation() * 0.1);
        qCInfo(dcWattsonic()) << "Updating inverter:" << inverter->stateValue(inverterCurrentPowerStateTypeId).toDouble() << "W" << inverter->stateValue(inverterTotalEnergyProducedStateTypeId).toDouble() << "kWh";

        Things meters = myThings().filterByParentId(thing->id()).filterByThingClassId(meterThingClassId);
        if (!meters.isEmpty()) {
            Thing *meter = meters.first();
            meter->setStateValue(meterCurrentPowerStateTypeId, connection->totalPowerOnMeter() * -1.0);
            meter->setStateValue(meterTotalEnergyConsumedStateTypeId, connection->totalEnergyPurchasedFromGrid() / 10.0);
            meter->setStateValue(meterTotalEnergyProducedStateTypeId, connection->totalEnergyInjectedToGrid() / 10.0);
            meter->setStateValue(meterCurrentPowerPhaseAStateTypeId, connection->phaseAPower() * -1.0);
            meter->setStateValue(meterCurrentPowerPhaseBStateTypeId, connection->phaseBPower() * -1.0);
            meter->setStateValue(meterCurrentPowerPhaseCStateTypeId, connection->phaseCPower() * -1.0);
            meter->setStateValue(meterVoltagePhaseAStateTypeId, connection->gridPhaseAVoltage() / 10.0);
            meter->setStateValue(meterVoltagePhaseBStateTypeId, connection->gridPhaseBVoltage() / 10.0);
            meter->setStateValue(meterVoltagePhaseCStateTypeId, connection->gridPhaseCVoltage() / 10.0);
            // The phase current registers don't seem to contain proper values. Calculating ourselves instead
//            meter->setStateValue(meterCurrentPhaseAStateTypeId, connection->gridPhaseACurrent() / 10.0);
//            meter->setStateValue(meterCurrentPhaseBStateTypeId, connection->gridPhaseBCurrent() / 10.0);
//            meter->setStateValue(meterCurrentPhaseCStateTypeId, connection->gridPhaseCCurrent() / 10.0);
            meter->setStateValue(meterCurrentPhaseAStateTypeId, (connection->phaseAPower() * -1.0) / (connection->gridPhaseAVoltage() / 10.0));
            meter->setStateValue(meterCurrentPhaseBStateTypeId, (connection->phaseBPower() * -1.0) / (connection->gridPhaseBVoltage() / 10.0));
            meter->setStateValue(meterCurrentPhaseCStateTypeId, (connection->phaseCPower() * -1.0) / (connection->gridPhaseCVoltage() / 10.0));
            qCInfo(dcWattsonic()) << "Updating meter:" << meter->stateValue(meterCurrentPowerStateTypeId).toDouble() << "W" << meter->stateValue(meterTotalEnergyProducedStateTypeId).toDouble() << "kWh";
        }
        Things batteries = myThings().filterByParentId(thing->id()).filterByThingClassId(batteryThingClassId);
        if (!batteries.isEmpty() && connection->SOC() > 0) {
            Thing *battery = batteries.first();
            QHash<WattsonicModbusRtuConnection::BatteryMode, QString> map {
                {WattsonicModbusRtuConnection::BatteryModeDischarge, "discharging"},
                {WattsonicModbusRtuConnection::BatteryModeCharge, "charging"}
            };
            battery->setStateValue(batteryChargingStateStateTypeId, map.value(connection->batteryMode()));
            battery->setStateValue(batteryCurrentPowerStateTypeId, connection->batteryPower() * -1.0);
            battery->setStateValue(batteryBatteryLevelStateTypeId, connection->SOC() / 100.0);
            battery->setStateValue(batteryBatteryCriticalStateTypeId, connection->SOC() < 500);
        }

    });


}
