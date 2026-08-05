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

#include "integrationpluginamperfied.h"
#include "plugininfo.h"
#include "energycontroldiscovery.h"
#include "amperfiedconnectdiscovery.h"

#include <network/networkdevicediscovery.h>
#include <hardwaremanager.h>
#include <hardware/modbus/modbusrtuhardwareresource.h>

IntegrationPluginAmperfied::IntegrationPluginAmperfied()
{

}

IntegrationPluginAmperfied::ChargingCurrentState &IntegrationPluginAmperfied::chargingCurrentState(Thing *thing)
{
    if (!m_chargingCurrentStates.contains(thing)) {
        ChargingCurrentState state;
        state.power = thing->stateValue("power").toBool();
        state.maxChargingCurrent = static_cast<quint16>(qRound(thing->stateValue("maxChargingCurrent").toDouble() * 10));
        quint16 phaseCount = static_cast<quint16>(thing->stateValue("phaseCount").toUInt());
        if (phaseCount == 1 || phaseCount == 3)
            state.phaseCount = phaseCount;
        m_chargingCurrentStates.insert(thing, state);
    }

    return m_chargingCurrentStates[thing];
}

quint16 IntegrationPluginAmperfied::stablePhaseCount(Thing *thing, quint16 phaseSwitchControl)
{
    ChargingCurrentState &state = chargingCurrentState(thing);

    // The phase count describes the configured topology. Do not derive it from
    // instantaneous currents: startup and shutdown residuals can briefly make a
    // three-phase charger look single-phase. Retain the last valid configuration
    // and use the conservative three-phase default until one is available.
    if (phaseSwitchControl == 1 || phaseSwitchControl == 3)
        state.phaseCount = phaseSwitchControl;

    return state.phaseCount;
}

void IntegrationPluginAmperfied::discoverThings(ThingDiscoveryInfo *info)
{
    if (info->thingClassId() == energyControlThingClassId) {
        EnergyControlDiscovery *discovery = new EnergyControlDiscovery(hardwareManager()->modbusRtuResource(), info);

        connect(discovery, &EnergyControlDiscovery::discoveryFinished, info, [this, info, discovery](bool modbusMasterAvailable){
            if (!modbusMasterAvailable) {
                info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("No modbus RTU master with appropriate settings found. Please set up a modbus RTU master with a baudrate of 19200, 8 data bis, 1 stop bit and even parity first."));
                return;
            }

            qCInfo(dcAmperfied()) << "Discovery results:" << discovery->discoveryResults().length();

            foreach (const EnergyControlDiscovery::Result &result, discovery->discoveryResults()) {
                ThingDescriptor descriptor(energyControlThingClassId, "Amperfied Energy Control", QString("Slave ID: %1").arg(result.slaveId));

                ParamList params{
                    {energyControlThingRtuMasterParamTypeId, result.modbusRtuMasterId},
                    {energyControlThingSlaveIdParamTypeId, result.slaveId}
                };
                descriptor.setParams(params);

                Thing *existingThing = myThings().findByParams(params);
                if (existingThing) {
                    descriptor.setThingId(existingThing->id());
                }
                info->addThingDescriptor(descriptor);
            }

            info->finish(Thing::ThingErrorNoError);
        });

        discovery->startDiscovery();

        return;
    }

    if (info->thingClassId() == connectHomeThingClassId || info->thingClassId() == connectBusinessThingClassId || info->thingClassId() == connectSolarThingClassId) {
        AmperfiedConnectDiscovery *discovery = new AmperfiedConnectDiscovery(hardwareManager()->networkDeviceDiscovery(), info);
        connect(discovery, &AmperfiedConnectDiscovery::discoveryFinished, info, [this, info, discovery](){
            qCInfo(dcAmperfied()) << "Discovery results:" << discovery->discoveryResults().length();

            foreach (const AmperfiedConnectDiscovery::Result &result, discovery->discoveryResults()) {
                QString description;
                switch (result.networkDeviceInfo.monitorMode()) {
                case NetworkDeviceInfo::MonitorModeMac:
                    description = "MAC " + result.networkDeviceInfo.macAddressInfos().constFirst().macAddress().toString();
                    break;
                case NetworkDeviceInfo::MonitorModeHostName:
                    description = "Host name " + result.networkDeviceInfo.hostName();
                    break;
                case NetworkDeviceInfo::MonitorModeIp:
                    description = "IP " + result.networkDeviceInfo.address().toString();
                    break;
                }

                ThingDescriptor descriptor(info->thingClassId(), "Amperfied " + result.modelName, description);

                ParamTypeId macAddressParamTypeId = thingClass(info->thingClassId()).paramTypes().findByName("macAddress").id();
                ParamTypeId hostNameParamTypeId = thingClass(info->thingClassId()).paramTypes().findByName("hostName").id();
                ParamTypeId addressParamTypeId = thingClass(info->thingClassId()).paramTypes().findByName("address").id();

                ParamList params;
                params.append(Param(macAddressParamTypeId, result.networkDeviceInfo.thingParamValueMacAddress()));
                params.append(Param(hostNameParamTypeId, result.networkDeviceInfo.thingParamValueHostName()));
                params.append(Param(addressParamTypeId, result.networkDeviceInfo.thingParamValueAddress()));
                descriptor.setParams(params);

                Thing *existingThing = myThings().findByParams(params);
                if (existingThing) {
                    descriptor.setThingId(existingThing->id());
                }
                info->addThingDescriptor(descriptor);
            }

            info->finish(Thing::ThingErrorNoError);

        });
        QHash<ThingClassId, QString> map = {
            {connectHomeThingClassId, "connect.home"},
            {connectBusinessThingClassId, "connect.business"},
            {connectSolarThingClassId, "connect.solar"}
        };
        discovery->startDiscovery(map.value(info->thingClassId()));
    }
}

void IntegrationPluginAmperfied::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcAmperfied()) << "Setup" << thing << thing->params();

    if (thing->thingClassId() == energyControlThingClassId) {

        if (m_rtuConnections.contains(thing)) {
            qCDebug(dcAmperfied()) << "Reconfiguring existing thing" << thing->name();
            m_rtuConnections.take(thing)->deleteLater();
        }

        setupRtuConnection(info);
        return;
    }


    if (info->thing()->thingClassId() == connectHomeThingClassId
            || info->thing()->thingClassId() == connectSolarThingClassId
            || info->thing()->thingClassId() == connectBusinessThingClassId) {
        if (m_tcpConnections.contains(info->thing())) {
            delete m_tcpConnections.take(info->thing());
        }

        NetworkDeviceMonitor *monitor = m_monitors.value(info->thing());
        if (!monitor) {
            monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(thing);
            m_monitors.insert(thing, monitor);
        }

        connect(info, &ThingSetupInfo::aborted, monitor, [=](){
            if (m_monitors.contains(thing)) {
                qCDebug(dcAmperfied()) << "Unregistering monitor because setup has been aborted.";
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        });

        qCDebug(dcAmperfied()) << "Monitor reachable" << monitor->reachable() << thing->paramValue("macAddress").toString();
        if (monitor->reachable()) {
            setupTcpConnection(info);
        } else {
            connect(monitor, &NetworkDeviceMonitor::reachableChanged, info, [this, info](bool reachable){
                qCDebug(dcAmperfied()) << "Monitor reachable changed!" << reachable;
                if (reachable) {
                    setupTcpConnection(info);
                }
            });
        }
    }
}

void IntegrationPluginAmperfied::postSetupThing(Thing *thing)
{
    chargingCurrentState(thing);

    if (!m_pluginTimer) {
        qCDebug(dcAmperfied()) << "Starting plugin timer...";
        m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(2);
        connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
            foreach(AmperfiedModbusRtuConnection *connection, m_rtuConnections) {
                qCDebug(dcAmperfied()) << "Updating connection" << connection->modbusRtuMaster() << connection->slaveId();
                connection->update();
            }
            foreach(AmperfiedModbusTcpConnection *connection, m_tcpConnections) {
                qCDebug(dcAmperfied()) << "Updating connection" << connection->modbusTcpMaster()->hostAddress();
                connection->update();
            }
        });
        m_pluginTimer->start();
    }
}

void IntegrationPluginAmperfied::executeAction(ThingActionInfo *info)
{
    if (info->thing()->thingClassId() == energyControlThingClassId) {
        AmperfiedModbusRtuConnection *connection = m_rtuConnections.value(info->thing());

        if (info->action().actionTypeId() == energyControlPowerActionTypeId) {
            bool power = info->action().paramValue(energyControlPowerActionPowerParamTypeId).toBool();
            ChargingCurrentState &state = chargingCurrentState(info->thing());
            state.power = power;
            ModbusRtuReply *reply = connection->setChargingCurrent(state.power ? state.maxChargingCurrent : 0);
            connect(reply, &ModbusRtuReply::finished, info, [info, reply, power](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    info->thing()->setStateValue(energyControlPowerStateTypeId, power);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    qCWarning(dcAmperfied()) << "Error setting power:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
            return;
        }

        if (info->action().actionTypeId() == energyControlMaxChargingCurrentActionTypeId) {
            double current = qRound(info->action().paramValue(energyControlMaxChargingCurrentActionMaxChargingCurrentParamTypeId).toDouble() * 10) / 10.0;
            ChargingCurrentState &state = chargingCurrentState(info->thing());
            state.maxChargingCurrent = static_cast<quint16>(qRound(current * 10));
            ModbusRtuReply *reply = connection->setChargingCurrent(state.power ? state.maxChargingCurrent : 0);
            connect(reply, &ModbusRtuReply::finished, info, [info, reply, current](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    info->thing()->setStateValue(energyControlMaxChargingCurrentStateTypeId, current);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    qCWarning(dcAmperfied()) << "Error setting power:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
        }

    }

    if (info->thing()->thingClassId() == connectHomeThingClassId
            || info->thing()->thingClassId() == connectBusinessThingClassId
            || info->thing()->thingClassId() == connectSolarThingClassId) {

        AmperfiedModbusTcpConnection *connection = m_tcpConnections.value(info->thing());

        ActionType actionType = info->thing()->thingClass().actionTypes().findById(info->action().actionTypeId());

        if (actionType.name() == "power") {
            bool power = info->action().paramValue(actionType.paramTypes().findByName("power").id()).toBool();
            ChargingCurrentState &state = chargingCurrentState(info->thing());
            state.power = power;
            QModbusReply *reply = connection->setChargingCurrent(state.power ? state.maxChargingCurrent : 0);
            connect(reply, &QModbusReply::finished, info, [info, reply, power](){
                if (reply->error() == QModbusDevice::NoError) {
                    info->thing()->setStateValue("power", power);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    qCWarning(dcAmperfied()) << "Error setting power:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
        } else if (actionType.name() == "maxChargingCurrent") {
            double current = qRound(info->action().paramValue(actionType.paramTypes().findByName("maxChargingCurrent").id()).toDouble() * 10) / 10.0;
            ChargingCurrentState &state = chargingCurrentState(info->thing());
            state.maxChargingCurrent = static_cast<quint16>(qRound(current * 10));
            QModbusReply *reply = connection->setChargingCurrent(state.power ? state.maxChargingCurrent : 0);
            connect(reply, &QModbusReply::finished, info, [info, reply, current](){
                if (reply->error() == QModbusDevice::NoError) {
                    info->thing()->setStateValue("maxChargingCurrent", current);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    qCWarning(dcAmperfied()) << "Error setting power:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
        } else if (actionType.name() == "desiredPhaseCount") {
            uint desiredPhaseCount = info->thing()->stateValue("desiredPhaseCount").toBool();
            QModbusReply *reply = connection->setPhaseSwitchControl(desiredPhaseCount);
            connect(reply, &QModbusReply::finished, info, [info, reply, desiredPhaseCount](){
                if (reply->error() == QModbusDevice::NoError) {
                    info->thing()->setStateValue("desiredPhaseCount", desiredPhaseCount);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    qCWarning(dcAmperfied()) << "Error setting desired phase count:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
        } else {
            info->finish(Thing::ThingErrorUnsupportedFeature);
        }

    }

}

void IntegrationPluginAmperfied::thingRemoved(Thing *thing)
{
    m_chargingCurrentStates.remove(thing);
    m_initialUpdates.remove(thing);

    if (thing->thingClassId() == energyControlThingClassId) {
        delete m_rtuConnections.take(thing);
    }

    if (thing->thingClassId() == connectHomeThingClassId
            || thing->thingClassId() == connectBusinessThingClassId
            || thing->thingClassId() == connectSolarThingClassId) {
        delete m_tcpConnections.take(thing);
    }

    if (m_monitors.contains(thing))
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));

    if (myThings().isEmpty() && m_pluginTimer) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginAmperfied::setupRtuConnection(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    m_initialUpdates.insert(thing);
    ModbusRtuMaster *master = hardwareManager()->modbusRtuResource()->getModbusRtuMaster(thing->paramValue(energyControlThingRtuMasterParamTypeId).toUuid());
    if (!master) {
        qCWarning(dcAmperfied()) << "The Modbus Master is not available any more.";
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The modbus RTU connection is not available."));
        return;
    }
    quint16 slaveId = thing->paramValue(energyControlThingSlaveIdParamTypeId).toUInt();
    AmperfiedModbusRtuConnection *connection = new AmperfiedModbusRtuConnection(master, slaveId, thing);

    connect(connection, &AmperfiedModbusRtuConnection::reachableChanged, thing, [connection, thing](bool reachable){
        if (reachable) {
            connection->initialize();
        } else {
            thing->setStateValue(energyControlCurrentPowerStateTypeId, 0);
            thing->setStateValue(energyControlConnectedStateTypeId, false);
        }
    });
    connect(connection, &AmperfiedModbusRtuConnection::initializationFinished, thing, [connection, thing](bool success){
        if (success) {
            thing->setStateValue(energyControlConnectedStateTypeId, true);

            // Disabling the auto-standby as it will shut down modbus
            connection->setStandby(AmperfiedModbusRtuConnection::StandbyStandbyDisabled);
        }
    });

    connect(connection, &AmperfiedModbusRtuConnection::initializationFinished, info, [this, info, connection](bool success){
        if (success) {
            if (connection->version() < 0x0107) {
                qCWarning(dcAmperfied()) << "We require at least version 1.0.8.";
                info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("The firmware of this wallbox is too old. Please update the wallbox to at least firmware 1.0.7."));
                delete connection;
                return;
            }
            m_rtuConnections.insert(info->thing(), connection);
            info->finish(Thing::ThingErrorNoError);
        } else {
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("The wallbox is not responding"));
        }
    });

    connect(connection, &AmperfiedModbusRtuConnection::updateFinished, thing, [this, connection, thing](){
        qCDebug(dcAmperfied()) << "Updated:" << connection;

        ChargingCurrentState &state = chargingCurrentState(thing);
        if (m_initialUpdates.remove(thing)) {
            state.power = connection->chargingCurrent() != 0;
            if (state.power)
                state.maxChargingCurrent = connection->chargingCurrent();
        }

        if (connection->chargingCurrent() == 0) {
            thing->setStateValue(energyControlPowerStateTypeId, false);
        } else {
            thing->setStateValue(energyControlPowerStateTypeId, true);
            thing->setStateValue(energyControlMaxChargingCurrentStateTypeId, connection->chargingCurrent() / 10.0);
        }
        thing->setStateMinMaxValues(energyControlMaxChargingCurrentStateTypeId, connection->minChargingCurrent(), connection->maxChargingCurrent());
        thing->setStateValue(energyControlCurrentPowerStateTypeId, connection->currentPower());
        thing->setStateValue(energyControlTotalEnergyConsumedStateTypeId, connection->totalEnergy() / 1000.0);
        thing->setStateValue(energyControlSessionEnergyStateTypeId, connection->sessionEnergy() / 1000.0);
        switch (connection->chargingState()) {
        case AmperfiedModbusRtuConnection::ChargingStateUndefined:
        case AmperfiedModbusRtuConnection::ChargingStateA1:
        case AmperfiedModbusRtuConnection::ChargingStateA2:
            thing->setStateValue(energyControlPluggedInStateTypeId, false);
            thing->setStateValue(energyControlChargingStateTypeId, false);
            break;
        case AmperfiedModbusRtuConnection::ChargingStateB1:
        case AmperfiedModbusRtuConnection::ChargingStateB2:
            thing->setStateValue(energyControlPluggedInStateTypeId, true);
            thing->setStateValue(energyControlChargingStateTypeId, false);
            break;
        case AmperfiedModbusRtuConnection::ChargingStateC1:
        case AmperfiedModbusRtuConnection::ChargingStateC2:
        case AmperfiedModbusRtuConnection::ChargingStateDerating:
            thing->setStateValue(energyControlPluggedInStateTypeId, true);
            thing->setStateValue(energyControlChargingStateTypeId, true);
            break;
        case AmperfiedModbusRtuConnection::ChargingStateE:
        case AmperfiedModbusRtuConnection::ChargingStateError:
        case AmperfiedModbusRtuConnection::ChargingStateF:
            qCWarning(dcAmperfied()) << "Erraneous charging state:" << connection->chargingState();
            thing->setStateValue(energyControlPluggedInStateTypeId, false);
            thing->setStateValue(energyControlChargingStateTypeId, false);
            break;
        }

        thing->setStateValue(energyControlPhaseCountStateTypeId,
                             stablePhaseCount(thing, connection->phaseSwitchControl()));
    });

    connection->update();

}

void IntegrationPluginAmperfied::setupTcpConnection(ThingSetupInfo *info)
{
    qCDebug(dcAmperfied()) << "setting up TCP connection";
    Thing *thing = info->thing();
    m_initialUpdates.insert(thing);
    NetworkDeviceMonitor *monitor = m_monitors.value(info->thing());
    AmperfiedModbusTcpConnection *connection = new AmperfiedModbusTcpConnection(monitor->networkDeviceInfo().address(), 502, 1, info->thing());

    connect(connection, &AmperfiedModbusTcpConnection::reachableChanged, thing, [connection, thing](bool reachable){
        if (reachable) {
            connection->initialize();
        } else {
            thing->setStateValue("currentPower", 0);
            thing->setStateValue("connected", false);
        }
    });


    connect(connection, &AmperfiedModbusTcpConnection::initializationFinished, info, [this, info, connection](bool success){
        if (success) {
            if (connection->version() < 0x0107) {
                qCWarning(dcAmperfied()) << "We require at least version 1.0.8.";
                info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("The firmware of this wallbox is too old. Please update the wallbox to at least firmware 1.0.7."));
                delete connection;
                return;
            }
            m_tcpConnections.insert(info->thing(), connection);
            info->finish(Thing::ThingErrorNoError);
            connection->update();
        } else {
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("The wallbox is not responding"));
        }
    });

    connect(connection, &AmperfiedModbusTcpConnection::updateFinished, thing, [this, connection, thing](){
        qCDebug(dcAmperfied()) << "Updated:" << connection;

        thing->setStateValue("connected", true);

        ChargingCurrentState &state = chargingCurrentState(thing);
        if (m_initialUpdates.remove(thing)) {
            state.power = connection->chargingCurrent() != 0;
            if (state.power)
                state.maxChargingCurrent = connection->chargingCurrent();
        }

        if (connection->chargingCurrent() == 0) {
            thing->setStateValue("power", false);
        } else {
            thing->setStateValue("power", true);
            thing->setStateValue("maxChargingCurrent", connection->chargingCurrent() / 10.0);
        }
        thing->setStateMinMaxValues("maxChargingCurrent", connection->minChargingCurrent(), connection->maxChargingCurrent());
        thing->setStateValue("currentPower", connection->currentPower());
        thing->setStateValue("totalEnergyConsumed", connection->totalEnergy() / 1000.0);
        thing->setStateValue("sessionEnergy", connection->sessionEnergy() / 1000.0);
        switch (connection->chargingState()) {
        case AmperfiedModbusTcpConnection::ChargingStateUndefined:
        case AmperfiedModbusTcpConnection::ChargingStateA1:
        case AmperfiedModbusTcpConnection::ChargingStateA2:
            thing->setStateValue("pluggedIn", false);
            thing->setStateValue("charging", false);
            break;
        case AmperfiedModbusTcpConnection::ChargingStateB1:
        case AmperfiedModbusTcpConnection::ChargingStateB2:
            thing->setStateValue("pluggedIn", true);
            thing->setStateValue("charging", false);
            break;
        case AmperfiedModbusTcpConnection::ChargingStateC1:
        case AmperfiedModbusTcpConnection::ChargingStateC2:
        case AmperfiedModbusTcpConnection::ChargingStateDerating:
            thing->setStateValue("pluggedIn", true);
            thing->setStateValue("charging", true);
            break;
        case AmperfiedModbusTcpConnection::ChargingStateE:
        case AmperfiedModbusTcpConnection::ChargingStateError:
        case AmperfiedModbusTcpConnection::ChargingStateF:
            qCWarning(dcAmperfied()) << "Erraneous CP signal state:" << connection->chargingState();
            thing->setStateValue("charging", false);
        }

        thing->setStateValue("phaseCount", stablePhaseCount(thing, connection->phaseSwitchControl()));
    });

    connection->connectDevice();
}
