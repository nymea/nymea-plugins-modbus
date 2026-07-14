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

IntegrationPluginAmperfied::ChargingCurrentControl &IntegrationPluginAmperfied::chargingCurrentControl(Thing *thing)
{
    if (!m_chargingCurrentControls.contains(thing)) {
        ChargingCurrentControl control;
        control.requestedCurrent = static_cast<quint16>(qRound(thing->stateValue("maxChargingCurrent").toDouble() * 10));
        m_chargingCurrentControls.insert(thing, control);
    }
    return m_chargingCurrentControls[thing];
}

void IntegrationPluginAmperfied::setChargingCurrentError(Thing *thing, const QString &error)
{
    const QString value = error.isEmpty() ? QStringLiteral("No error") : error;
    if (thing->stateValue("error").toString() != value)
        thing->setStateValue("error", value);
}

void IntegrationPluginAmperfied::handleChargingCurrentUpdate(Thing *thing, quint16 chargingCurrent, quint16 watchdogTimeout, quint16 failSafeCurrent)
{
    ChargingCurrentControl &control = chargingCurrentControl(thing);
    thing->setStateValue("power", chargingCurrent != 0);

    if (!control.hasExpectedCurrent) {
        // On the first update, adopt an active command from the wallbox. If charging is
        // disabled, retain the cached current that will be applied when it is enabled.
        if (chargingCurrent != 0) {
            control.requestedCurrent = chargingCurrent;
            thing->setStateValue("maxChargingCurrent", chargingCurrent / 10.0);
        }
        setChargingCurrentError(thing, QString());
        return;
    }

    if (chargingCurrent == control.expectedCurrent) {
        control.lastUnexpectedCurrent = -1;
        setChargingCurrentError(thing, QString());
        return;
    }

    if (control.lastUnexpectedCurrent == chargingCurrent)
        return;

    control.lastUnexpectedCurrent = chargingCurrent;
    const QString error = QStringLiteral("Charging current command mismatch: requested %1 A, wallbox reports %2 A (watchdog %3 ms, fail-safe %4 A).")
                              .arg(control.expectedCurrent / 10.0, 0, 'f', 1)
                              .arg(chargingCurrent / 10.0, 0, 'f', 1)
                              .arg(watchdogTimeout)
                              .arg(failSafeCurrent / 10.0, 0, 'f', 1);
    qCWarning(dcAmperfied()) << thing->name() << error;
    setChargingCurrentError(thing, error);
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
    Q_UNUSED(thing)
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
            const quint16 expectedCurrent = power ? chargingCurrentControl(info->thing()).requestedCurrent : 0;
            ModbusRtuReply *reply = connection->setChargingCurrent(expectedCurrent);
            connect(reply, &ModbusRtuReply::finished, info, [this, info, connection, reply, power, expectedCurrent](){
                if (reply->error() != ModbusRtuReply::NoError) {
                    qCWarning(dcAmperfied()) << "Error setting power:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                    return;
                }

                ModbusRtuReply *verificationReply = connection->readChargingCurrent();
                connect(verificationReply, &ModbusRtuReply::finished, info, [this, info, verificationReply, power, expectedCurrent](){
                    if (verificationReply->error() != ModbusRtuReply::NoError
                            || verificationReply->result().isEmpty()
                            || verificationReply->result().constFirst() != expectedCurrent) {
                        const quint16 actualCurrent = verificationReply->result().isEmpty() ? 0 : verificationReply->result().constFirst();
                        const QString error = QStringLiteral("Could not verify charging current command: expected %1 A, read %2 A.")
                                                  .arg(expectedCurrent / 10.0, 0, 'f', 1)
                                                  .arg(actualCurrent / 10.0, 0, 'f', 1);
                        qCWarning(dcAmperfied()) << info->thing()->name() << error << verificationReply->errorString();
                        setChargingCurrentError(info->thing(), error);
                        info->finish(Thing::ThingErrorHardwareFailure);
                        return;
                    }

                    ChargingCurrentControl &control = chargingCurrentControl(info->thing());
                    control.expectedCurrent = expectedCurrent;
                    control.hasExpectedCurrent = true;
                    control.lastUnexpectedCurrent = -1;
                    info->thing()->setStateValue(energyControlPowerStateTypeId, power);
                    setChargingCurrentError(info->thing(), QString());
                    info->finish(Thing::ThingErrorNoError);
                });
            });
            return;
        }

        if (info->action().actionTypeId() == energyControlMaxChargingCurrentActionTypeId) {
            bool power = info->thing()->stateValue(energyControlPowerStateTypeId).toBool();
            double current = qRound(info->action().paramValue(energyControlMaxChargingCurrentActionMaxChargingCurrentParamTypeId).toDouble() * 10) / 10.0;
            const quint16 expectedCurrent = static_cast<quint16>(qRound(current * 10));
            if (!power) {
                chargingCurrentControl(info->thing()).requestedCurrent = expectedCurrent;
                qCInfo(dcAmperfied()) << info->thing()->name() << "Stored charging current" << current << "A; application is deferred because charging is disabled.";
                info->thing()->setStateValue(energyControlMaxChargingCurrentStateTypeId, current);
                info->finish(Thing::ThingErrorNoError);
                return;
            }

            ModbusRtuReply *reply = connection->setChargingCurrent(expectedCurrent);
            connect(reply, &ModbusRtuReply::finished, info, [this, info, connection, reply, current, expectedCurrent](){
                if (reply->error() != ModbusRtuReply::NoError) {
                    qCWarning(dcAmperfied()) << "Error setting charging current:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                    return;
                }

                ModbusRtuReply *verificationReply = connection->readChargingCurrent();
                connect(verificationReply, &ModbusRtuReply::finished, info, [this, info, verificationReply, current, expectedCurrent](){
                    if (verificationReply->error() != ModbusRtuReply::NoError
                            || verificationReply->result().isEmpty()
                            || verificationReply->result().constFirst() != expectedCurrent) {
                        const quint16 actualCurrent = verificationReply->result().isEmpty() ? 0 : verificationReply->result().constFirst();
                        const QString error = QStringLiteral("Could not verify charging current command: expected %1 A, read %2 A.")
                                                  .arg(expectedCurrent / 10.0, 0, 'f', 1)
                                                  .arg(actualCurrent / 10.0, 0, 'f', 1);
                        qCWarning(dcAmperfied()) << info->thing()->name() << error << verificationReply->errorString();
                        setChargingCurrentError(info->thing(), error);
                        info->finish(Thing::ThingErrorHardwareFailure);
                        return;
                    }

                    ChargingCurrentControl &control = chargingCurrentControl(info->thing());
                    control.requestedCurrent = expectedCurrent;
                    control.expectedCurrent = expectedCurrent;
                    control.hasExpectedCurrent = true;
                    control.lastUnexpectedCurrent = -1;
                    info->thing()->setStateValue(energyControlMaxChargingCurrentStateTypeId, current);
                    setChargingCurrentError(info->thing(), QString());
                    info->finish(Thing::ThingErrorNoError);
                });
            });
            return;
        }

    }

    if (info->thing()->thingClassId() == connectHomeThingClassId
            || info->thing()->thingClassId() == connectBusinessThingClassId
            || info->thing()->thingClassId() == connectSolarThingClassId) {

        AmperfiedModbusTcpConnection *connection = m_tcpConnections.value(info->thing());

        ActionType actionType = info->thing()->thingClass().actionTypes().findById(info->action().actionTypeId());

        if (actionType.name() == "power") {
            bool power = info->action().paramValue(actionType.paramTypes().findByName("power").id()).toBool();
            const quint16 expectedCurrent = power ? chargingCurrentControl(info->thing()).requestedCurrent : 0;
            QModbusReply *reply = connection->setChargingCurrent(expectedCurrent);
            connect(reply, &QModbusReply::finished, info, [this, info, connection, reply, power, expectedCurrent](){
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcAmperfied()) << "Error setting power:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                    return;
                }

                QModbusReply *verificationReply = connection->readChargingCurrent();
                connect(verificationReply, &QModbusReply::finished, info, [this, info, verificationReply, power, expectedCurrent](){
                    const QModbusDataUnit result = verificationReply->result();
                    const quint16 actualCurrent = result.valueCount() > 0 ? result.value(0) : 0;
                    if (verificationReply->error() != QModbusDevice::NoError || result.valueCount() == 0 || actualCurrent != expectedCurrent) {
                        const QString error = QStringLiteral("Could not verify charging current command: expected %1 A, read %2 A.")
                                                  .arg(expectedCurrent / 10.0, 0, 'f', 1)
                                                  .arg(actualCurrent / 10.0, 0, 'f', 1);
                        qCWarning(dcAmperfied()) << info->thing()->name() << error << verificationReply->errorString();
                        setChargingCurrentError(info->thing(), error);
                        info->finish(Thing::ThingErrorHardwareFailure);
                        return;
                    }

                    ChargingCurrentControl &control = chargingCurrentControl(info->thing());
                    control.expectedCurrent = expectedCurrent;
                    control.hasExpectedCurrent = true;
                    control.lastUnexpectedCurrent = -1;
                    info->thing()->setStateValue("power", power);
                    setChargingCurrentError(info->thing(), QString());
                    info->finish(Thing::ThingErrorNoError);
                });
            });
        } else if (actionType.name() == "maxChargingCurrent") {
            bool power = info->thing()->stateValue("power").toBool();
            double current = qRound(info->action().paramValue(actionType.paramTypes().findByName("maxChargingCurrent").id()).toDouble() * 10) / 10.0;
            const quint16 expectedCurrent = static_cast<quint16>(qRound(current * 10));
            if (!power) {
                chargingCurrentControl(info->thing()).requestedCurrent = expectedCurrent;
                qCInfo(dcAmperfied()) << info->thing()->name() << "Stored charging current" << current << "A; application is deferred because charging is disabled.";
                info->thing()->setStateValue("maxChargingCurrent", current);
                info->finish(Thing::ThingErrorNoError);
                return;
            }

            QModbusReply *reply = connection->setChargingCurrent(expectedCurrent);
            connect(reply, &QModbusReply::finished, info, [this, info, connection, reply, current, expectedCurrent](){
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcAmperfied()) << "Error setting charging current:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                    return;
                }

                QModbusReply *verificationReply = connection->readChargingCurrent();
                connect(verificationReply, &QModbusReply::finished, info, [this, info, verificationReply, current, expectedCurrent](){
                    const QModbusDataUnit result = verificationReply->result();
                    const quint16 actualCurrent = result.valueCount() > 0 ? result.value(0) : 0;
                    if (verificationReply->error() != QModbusDevice::NoError || result.valueCount() == 0 || actualCurrent != expectedCurrent) {
                        const QString error = QStringLiteral("Could not verify charging current command: expected %1 A, read %2 A.")
                                                  .arg(expectedCurrent / 10.0, 0, 'f', 1)
                                                  .arg(actualCurrent / 10.0, 0, 'f', 1);
                        qCWarning(dcAmperfied()) << info->thing()->name() << error << verificationReply->errorString();
                        setChargingCurrentError(info->thing(), error);
                        info->finish(Thing::ThingErrorHardwareFailure);
                        return;
                    }

                    ChargingCurrentControl &control = chargingCurrentControl(info->thing());
                    control.requestedCurrent = expectedCurrent;
                    control.expectedCurrent = expectedCurrent;
                    control.hasExpectedCurrent = true;
                    control.lastUnexpectedCurrent = -1;
                    info->thing()->setStateValue("maxChargingCurrent", current);
                    setChargingCurrentError(info->thing(), QString());
                    info->finish(Thing::ThingErrorNoError);
                });
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

    m_chargingCurrentControls.remove(thing);

    if (myThings().isEmpty() && m_pluginTimer) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginAmperfied::setupRtuConnection(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
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

        handleChargingCurrentUpdate(thing, connection->chargingCurrent(), connection->watchdogTimeout(), connection->failSafeCurrent());
        thing->setStateMinMaxValues(energyControlMaxChargingCurrentStateTypeId, connection->minChargingCurrent(), connection->maxChargingCurrent());
        thing->setStateValue(energyControlCurrentPowerStateTypeId, connection->currentPower());
        thing->setStateValue(energyControlTotalEnergyConsumedStateTypeId, connection->totalEnergy() / 1000.0);
        thing->setStateValue(energyControlSessionEnergyStateTypeId, connection->sessionEnergy() / 1000.0);
        switch (connection->chargingState()) {
        case AmperfiedModbusRtuConnection::ChargingStateUndefined:
        case AmperfiedModbusRtuConnection::ChargingStateA1:
        case AmperfiedModbusRtuConnection::ChargingStateA2:
            thing->setStateValue(energyControlPluggedInStateTypeId, false);
            break;
        case AmperfiedModbusRtuConnection::ChargingStateB1:
        case AmperfiedModbusRtuConnection::ChargingStateB2:
        case AmperfiedModbusRtuConnection::ChargingStateC1:
        case AmperfiedModbusRtuConnection::ChargingStateC2:
            thing->setStateValue(energyControlPluggedInStateTypeId, true);
            break;
        case AmperfiedModbusRtuConnection::ChargingStateDerating:
        case AmperfiedModbusRtuConnection::ChargingStateE:
        case AmperfiedModbusRtuConnection::ChargingStateError:
        case AmperfiedModbusRtuConnection::ChargingStateF:
            qCWarning(dcAmperfied()) << "Erraneous charging state:" << connection->chargingState();
            thing->setStateValue(energyControlPluggedInStateTypeId, false);
            break;
        }

        int phaseCount = 0;
        if (connection->currentL1() > 1) {
            phaseCount++;
        }
        if (connection->currentL2() > 1) {
            phaseCount++;
        }
        if (connection->currentL3() > 1) {
            phaseCount++;
        }
        if (phaseCount > 0) {
            thing->setStateValue(energyControlPhaseCountStateTypeId, phaseCount);
        }
        thing->setStateValue(energyControlChargingStateTypeId, phaseCount > 0);
    });

    connection->update();

}

void IntegrationPluginAmperfied::setupTcpConnection(ThingSetupInfo *info)
{
    qCDebug(dcAmperfied()) << "setting up TCP connection";
    Thing *thing = info->thing();
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

        handleChargingCurrentUpdate(thing, connection->chargingCurrent(), connection->watchdogTimeout(), connection->failSafeCurrent());
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
            thing->setStateValue("pluggedIn", true);
            thing->setStateValue("charging", true);
            break;
        case AmperfiedModbusTcpConnection::ChargingStateDerating:
        case AmperfiedModbusTcpConnection::ChargingStateE:
        case AmperfiedModbusTcpConnection::ChargingStateError:
        case AmperfiedModbusTcpConnection::ChargingStateF:
            qCWarning(dcAmperfied()) << "Erraneous CP signal state:" << connection->chargingState();
            thing->setStateValue("charging", false);
        }

        int phaseCount = 0;
        if (connection->currentL1() > 1) {
            phaseCount++;
        }
        if (connection->currentL2() > 1) {
            phaseCount++;
        }
        if (connection->currentL3() > 1) {
            phaseCount++;
        }
        if (phaseCount > 0) {
            thing->setStateValue("phaseCount", phaseCount);
        }
    });

    connection->connectDevice();
}
