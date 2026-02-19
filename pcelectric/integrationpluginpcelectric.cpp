// SPDX-License-Identifier: GPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2026, chargebyte austria GmbH
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

#include "integrationpluginpcelectric.h"
#include "pcelectricdiscovery.h"
#include "plugininfo.h"

#include <hardware/electricity.h>
#include <hardwaremanager.h>

IntegrationPluginPcElectric::IntegrationPluginPcElectric() {}

void IntegrationPluginPcElectric::init()
{
    //qCCritical(dcPcElectric()) << QString("%1").arg(QString::number(49155, 2));

    m_addressParamTypes[ev11ThingClassId] = ev11ThingAddressParamTypeId;
    m_addressParamTypes[ev11NoMeterThingClassId] = ev11NoMeterThingAddressParamTypeId;

    m_hostNameParamTypes[ev11ThingClassId] = ev11ThingHostNameParamTypeId;
    m_hostNameParamTypes[ev11NoMeterThingClassId] = ev11NoMeterThingHostNameParamTypeId;

    m_macParamTypes[ev11ThingClassId] = ev11ThingMacAddressParamTypeId;
    m_macParamTypes[ev11NoMeterThingClassId] = ev11NoMeterThingMacAddressParamTypeId;

    m_serialNumberParamTypes[ev11ThingClassId] = ev11ThingSerialNumberParamTypeId;
    m_serialNumberParamTypes[ev11NoMeterThingClassId] = ev11NoMeterThingSerialNumberParamTypeId;
}

void IntegrationPluginPcElectric::discoverThings(ThingDiscoveryInfo *info)
{
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcPcElectric()) << "The network discovery is not available on this platform.";
        info->finish(Thing::ThingErrorUnsupportedFeature, QT_TR_NOOP("The network device discovery is not available."));
        return;
    }

    // Create a discovery with the info as parent for auto deleting the object once the discovery info is done
    PcElectricDiscovery *discovery = new PcElectricDiscovery(hardwareManager()->networkDeviceDiscovery(), 502, 1, info);
    connect(discovery, &PcElectricDiscovery::discoveryFinished, info, [=]() {
        foreach (const PcElectricDiscovery::Result &result, discovery->results()) {
            if (info->thingClassId() != result.thingClassId)
                continue;

            ThingDescriptor descriptor(result.thingClassId,
                                       "PCE EV11.3 (" + result.serialNumber + ")",
                                       "Version: " + result.firmwareRevision + " - " + result.networkDeviceInfo.address().toString());
            qCDebug(dcPcElectric()) << "Discovered:" << descriptor.title() << descriptor.description();

            // Check if we already have set up this device
            Things existingThings = myThings().filterByParam(m_serialNumberParamTypes.value(result.thingClassId), result.serialNumber);
            if (existingThings.length() == 1) {
                qCDebug(dcPcElectric()) << "This PCE wallbox already exists in the system:" << result.serialNumber << result.networkDeviceInfo;
                descriptor.setThingId(existingThings.first()->id());
            }

            ParamList params;
            params << Param(m_macParamTypes.value(result.thingClassId), result.networkDeviceInfo.thingParamValueMacAddress());
            params << Param(m_hostNameParamTypes.value(result.thingClassId), result.networkDeviceInfo.thingParamValueHostName());
            params << Param(m_addressParamTypes.value(result.thingClassId), result.networkDeviceInfo.thingParamValueAddress());
            params << Param(m_serialNumberParamTypes.value(result.thingClassId), result.serialNumber);
            // Note: if we discover also the port and modbusaddress, we must fill them in from the discovery here, for now everywhere the defaults...
            descriptor.setParams(params);
            info->addThingDescriptor(descriptor);
        }

        info->finish(Thing::ThingErrorNoError);
    });

    // Start the discovery process
    discovery->startDiscovery();
}

void IntegrationPluginPcElectric::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcPcElectric()) << "Setup thing" << thing << thing->params();

    if (m_connections.contains(thing)) {
        qCDebug(dcPcElectric()) << "Reconfiguring existing thing" << thing->name();
        m_connections.take(thing)->deleteLater();

        if (m_monitors.contains(thing)) {
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
        }
    }

    NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(thing);
    if (!monitor) {
        qCWarning(dcPcElectric()) << "Could not create a valid network device monitor for the given parameters" << thing->params();
        info->finish(Thing::ThingErrorInvalidParameter);
        return;
    }

    m_monitors.insert(thing, monitor);

    connect(info, &ThingSetupInfo::aborted, monitor, [this, thing]() {
        if (m_monitors.contains(thing)) {
            qCDebug(dcPcElectric()) << "Unregistering monitor because setup has been aborted.";
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
        }
    });

    // Only make sure the connection is working in the initial setup, otherwise we let the monitor do the work
    if (info->isInitialSetup()) {
        // Continue with setup only if we know that the network device is reachable
        if (monitor->reachable()) {
            setupConnection(info);
        } else {
            // otherwise wait until we reach the networkdevice before setting up the device
            qCDebug(dcPcElectric()) << "Network device" << thing->name() << "is not reachable yet. Continue with the setup once reachable.";
            connect(monitor, &NetworkDeviceMonitor::reachableChanged, info, [=](bool reachable) {
                if (reachable) {
                    qCDebug(dcPcElectric()) << "Network device" << thing->name() << "is now reachable. Continue with the setup...";
                    setupConnection(info);
                }
            });
        }
    } else {
        setupConnection(info);
    }

    return;
}

void IntegrationPluginPcElectric::postSetupThing(Thing *thing)
{
    qCDebug(dcPcElectric()) << "Post setup thing" << thing->name();

    if (!m_refreshTimer) {
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(1);
        connect(m_refreshTimer, &PluginTimer::timeout, this, [this] {
            foreach (PceWallbox *connection, m_connections) {
                if (connection->reachable()) {
                    connection->update();
                }
            }
        });

        qCDebug(dcPcElectric()) << "Starting refresh timer...";
        m_refreshTimer->start();
    }

    PceWallbox::ChargingCurrentState chargingCurrentState;
    chargingCurrentState.power = thing->stateValue("power").toBool();
    chargingCurrentState.maxChargingCurrent = thing->stateValue("maxChargingCurrent").toDouble();
    chargingCurrentState.desiredPhaseCount = thing->stateValue("desiredPhaseCount").toUInt();
    qCDebug(dcPcElectric()) << "Initialize charging current state with cached values" << chargingCurrentState;
    m_chargingCurrentStateBuffer[thing] = chargingCurrentState;
}

void IntegrationPluginPcElectric::thingRemoved(Thing *thing)
{
    qCDebug(dcPcElectric()) << "Thing removed" << thing->name();

    if (m_connections.contains(thing)) {
        PceWallbox *connection = m_connections.take(thing);
        connection->disconnectDevice();
        connection->deleteLater();
    }

    if (m_initialUpdate.contains(thing))
        m_initialUpdate.remove(thing);

    if (m_chargingCurrentStateBuffer.contains(thing))
        m_chargingCurrentStateBuffer.remove(thing);

    // Unregister related hardware resources
    if (m_monitors.contains(thing))
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));

    if (myThings().isEmpty() && m_refreshTimer) {
        qCDebug(dcPcElectric()) << "Stopping reconnect timer";
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_refreshTimer);
        m_refreshTimer = nullptr;
    }
}

void IntegrationPluginPcElectric::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();

    PceWallbox *connection = m_connections.value(thing);
    if (!connection->reachable()) {
        qCWarning(dcPcElectric()) << "Could not execute action because the connection is not available.";
        info->finish(Thing::ThingErrorHardwareNotAvailable);
        return;
    }

    if (info->action().actionTypeId() == ev11PowerActionTypeId || info->action().actionTypeId() == ev11NoMeterPowerActionTypeId) {
        bool power = false;
        if (info->action().actionTypeId() == ev11PowerActionTypeId) {
            power = info->action().paramValue(ev11PowerActionPowerParamTypeId).toBool();
        } else if (info->action().actionTypeId() == ev11NoMeterPowerActionTypeId) {
            power = info->action().paramValue(ev11NoMeterPowerActionPowerParamTypeId).toBool();
        }

        qCDebug(dcPcElectric()) << "Set charging enabled to" << power;
        // Update buffer
        m_chargingCurrentStateBuffer[thing].power = power;

        quint16 registerValue = PceWallbox::deriveRegisterFromStates(m_chargingCurrentStateBuffer.value(thing));
        qCDebug(dcPcElectric()) << "Writing charging current register" << registerValue;
        QueuedModbusReply *reply = connection->setChargingCurrentAsync(registerValue);
        connect(reply, &QueuedModbusReply::finished, info, [reply, info, thing, power, registerValue]() {
            if (reply->error() != QModbusDevice::NoError) {
                qCWarning(dcPcElectric()) << "Could not set power state to" << power << "(" << registerValue << ")" << reply->errorString();
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }

            qCDebug(dcPcElectric()) << "Successfully set power state to" << power << "(" << registerValue << ")";
            thing->setStateValue("power", power);
            info->finish(Thing::ThingErrorNoError);
        });

        return;

    } else if (info->action().actionTypeId() == ev11MaxChargingCurrentActionTypeId || info->action().actionTypeId() == ev11NoMeterMaxChargingCurrentActionTypeId) {
        double desiredChargingCurrent = 6;
        if (info->action().actionTypeId() == ev11MaxChargingCurrentActionTypeId) {
            desiredChargingCurrent = info->action().paramValue(ev11MaxChargingCurrentActionMaxChargingCurrentParamTypeId).toDouble();
        } else if (info->action().actionTypeId() == ev11NoMeterMaxChargingCurrentActionTypeId) {
            desiredChargingCurrent = info->action().paramValue(ev11NoMeterMaxChargingCurrentActionMaxChargingCurrentParamTypeId).toDouble();
        }

        qCDebug(dcPcElectric()) << "Set max charging current to" << desiredChargingCurrent << "A";

        // Update buffer
        m_chargingCurrentStateBuffer[thing].maxChargingCurrent = desiredChargingCurrent;

        quint16 registerValue = PceWallbox::deriveRegisterFromStates(m_chargingCurrentStateBuffer.value(thing));
        qCDebug(dcPcElectric()) << "Writing charging current register" << registerValue;
        QueuedModbusReply *reply = connection->setChargingCurrentAsync(registerValue);
        connect(reply, &QueuedModbusReply::finished, info, [reply, info, thing, desiredChargingCurrent]() {
            if (reply->error() != QModbusDevice::NoError) {
                qCWarning(dcPcElectric()) << "Could not set charging current to" << desiredChargingCurrent << reply->errorString();
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }

            qCDebug(dcPcElectric()) << "Successfully set charging current (" << desiredChargingCurrent << ")";
            thing->setStateValue("maxChargingCurrent", desiredChargingCurrent);
            info->finish(Thing::ThingErrorNoError);
        });

        return;

    } else if (info->action().actionTypeId() == ev11DesiredPhaseCountActionTypeId || info->action().actionTypeId() == ev11NoMeterDesiredPhaseCountActionTypeId) {
        uint desiredPhaseCount = 1;
        if (info->action().actionTypeId() == ev11DesiredPhaseCountActionTypeId) {
            desiredPhaseCount = info->action().paramValue(ev11DesiredPhaseCountActionDesiredPhaseCountParamTypeId).toUInt();

        } else if (info->action().actionTypeId() == ev11NoMeterDesiredPhaseCountActionTypeId) {
            desiredPhaseCount = info->action().paramValue(ev11NoMeterDesiredPhaseCountActionDesiredPhaseCountParamTypeId).toUInt();
        }

        qCDebug(dcPcElectric()) << "Set desried phase count to" << desiredPhaseCount;

        // Update buffer
        m_chargingCurrentStateBuffer[thing].desiredPhaseCount = desiredPhaseCount;

        quint16 registerValue = PceWallbox::deriveRegisterFromStates(m_chargingCurrentStateBuffer.value(thing));
        qCDebug(dcPcElectric()) << "Writing charging current register" << registerValue;
        QueuedModbusReply *reply = connection->setChargingCurrentAsync(registerValue);
        connect(reply, &QueuedModbusReply::finished, info, [reply, info, thing, desiredPhaseCount]() {
            if (reply->error() != QModbusDevice::NoError) {
                qCWarning(dcPcElectric()) << "Could not set desired phase count to" << desiredPhaseCount << reply->errorString();
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }

            qCDebug(dcPcElectric()) << "Successfully set phase count (" << desiredPhaseCount << ")";
            thing->setStateValue("desiredPhaseCount", desiredPhaseCount);
            info->finish(Thing::ThingErrorNoError);
        });

        return;
    }

    Q_ASSERT_X(false, "IntegrationPluginPcElectric::executeAction", QString("Unhandled action: %1").arg(info->action().actionTypeId().toString()).toLocal8Bit());
}

void IntegrationPluginPcElectric::setupConnection(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    NetworkDeviceMonitor *monitor = m_monitors.value(thing);

    qCDebug(dcPcElectric()) << "Setting up PCE wallbox using" << monitor->networkDeviceInfo().address().toString();

    PceWallbox *connection = new PceWallbox(monitor->networkDeviceInfo().address(), 502, 1, this);
    connect(info, &ThingSetupInfo::aborted, connection, &PceWallbox::deleteLater);

    if (monitor->networkDeviceInfo().isComplete())
        connection->modbusTcpMaster()->setHostAddress(monitor->networkDeviceInfo().address());

    // Monitor reachability
    connect(monitor, &NetworkDeviceMonitor::reachableChanged, thing, [thing, connection, monitor](bool reachable) {
        if (!thing->setupComplete())
            return;

        qCDebug(dcPcElectric()) << "Network device monitor for" << thing->name() << (reachable ? "is now reachable" : "is not reachable any more");
        if (reachable && !thing->stateValue("connected").toBool()) {
            connection->modbusTcpMaster()->setHostAddress(monitor->networkDeviceInfo().address());
            connection->connectDevice();
        } else if (!reachable) {
            // Note: We disable autoreconnect explicitly and we will
            // connect the device once the monitor says it is reachable again
            connection->disconnectDevice();
        }
    });

    // Connection reachability
    connect(connection, &PceWallbox::reachableChanged, thing, [this, thing](bool reachable) {
        qCInfo(dcPcElectric()) << "Reachable changed to" << reachable << "for" << thing;
        m_initialUpdate[thing] = true;
        thing->setStateValue("connected", reachable);

        // Reset energy related information if not reachable
        if (!reachable && thing->thingClassId() == ev11ThingClassId) {
            thing->setStateValue("currentPower", 0);
            thing->setStateValue("currentPowerPhaseA", 0);
            thing->setStateValue("currentPowerPhaseB", 0);
            thing->setStateValue("currentPowerPhaseC", 0);
            thing->setStateValue("voltagePhaseA", 0);
            thing->setStateValue("voltagePhaseB", 0);
            thing->setStateValue("voltagePhaseC", 0);
            thing->setStateValue("currentPhaseA", 0);
            thing->setStateValue("currentPhaseB", 0);
            thing->setStateValue("currentPhaseC", 0);
        }
    });

    connect(connection, &PceWallbox::updateFinished, thing, [this, thing, connection]() {
        qCDebug(dcPcElectric()) << "Update finished for" << thing;
        qCDebug(dcPcElectric()) << connection;

        if (connection->phaseAutoSwitch()) {
            thing->setStatePossibleValues("desiredPhaseCount", {1, 3}); // Enable phase switching

            switch (connection->chargingRelayState()) {
            case EV11ModbusTcpConnection::ChargingRelayStateNoCharging:
                // Not charging, assuming phase count matches the desired phase count...
                thing->setStateValue("phaseCount", thing->stateValue("desiredPhaseCount"));
                break;
            case EV11ModbusTcpConnection::ChargingRelayStateSinglePhase:
                thing->setStateValue("phaseCount", 1);
                break;
            case EV11ModbusTcpConnection::ChargingRelayStateTheePhase:
                thing->setStateValue("phaseCount", 3);
                break;
            }
        } else {
            // Note: if auto phase switching is disabled, the wallbox forces 3 phase charging
            thing->setStatePossibleValues("desiredPhaseCount", {3}); // Disable phase switching (default 3)
            thing->setStateValue("desiredPhaseCount", 3);
            thing->setStateValue("phaseCount", 3);
        }

        thing->setStateMaxValue("maxChargingCurrent", connection->maxChargingCurrentDip() / 1000);
        thing->setStateValue("pluggedIn", connection->chargingState() >= PceWallbox::ChargingStateB1 && connection->chargingState() < PceWallbox::ChargingStateError);
        thing->setStateValue("charging", connection->chargingState() == PceWallbox::ChargingStateC2);

        switch (connection->chargingState()) {
        case PceWallbox::ChargingStateInitializing:
            thing->setStateValue("status", "Init");
            break;
        case PceWallbox::ChargingStateA1:
            thing->setStateValue("status", "A1");
            break;
        case PceWallbox::ChargingStateA2:
            thing->setStateValue("status", "A2");
            break;
        case PceWallbox::ChargingStateB1:
            thing->setStateValue("status", "B1");
            break;
        case PceWallbox::ChargingStateB2:
            thing->setStateValue("status", "B2");
            break;
        case PceWallbox::ChargingStateC1:
            thing->setStateValue("status", "C1");
            break;
        case PceWallbox::ChargingStateC2:
            thing->setStateValue("status", "C2");
            break;
        case PceWallbox::ChargingStateError:
            thing->setStateValue("status", "F");
            break;
        }

        thing->setStateValue("currentVersion", connection->firmwareRevision());
        thing->setStateValue("temperature", connection->temperature());

        switch (connection->error()) {
        case EV11ModbusTcpConnection::ErrorNoError:
            thing->setStateValue("error", "Kein Fehler aktiv");
            break;
        case EV11ModbusTcpConnection::ErrorOverheating:
            thing->setStateValue("error", "1: Übertemperatur. Ladevorgang wird automatisch fortgesetzt.");
            break;
        case EV11ModbusTcpConnection::ErrorDCFaultCurrent:
            thing->setStateValue("error", "2: DC Fehlerstromsensor ausgelöst.");
            break;
        case EV11ModbusTcpConnection::ErrorChargingWithVentilation:
            thing->setStateValue("error", "3: Ladeanforderung mit Belüftung.");
            break;
        case EV11ModbusTcpConnection::ErrorCPErrorEF:
            thing->setStateValue("error", "4: CP Signal, Fehlercode E oder F.");
            break;
        case EV11ModbusTcpConnection::ErrorCPErrorBypass:
            thing->setStateValue("error", "5: CP Signal, bypass.");
            break;
        case EV11ModbusTcpConnection::ErrorCPErrorDiodFault:
            thing->setStateValue("error", "6: CP Signal, Diode defekt.");
            break;
        case EV11ModbusTcpConnection::ErrorDCFaultCurrentCalibrating:
            thing->setStateValue("error", "7: DC Fehlerstromsensor, Kalibrirung.");
            break;
        case EV11ModbusTcpConnection::ErrorDCFaultCurrentCommunication:
            thing->setStateValue("error", "8: DC Fehlerstromsensor, Kommunikationsfehler.");
            break;
        case EV11ModbusTcpConnection::ErrorDCFaultCurrentError:
            thing->setStateValue("error", "9: DC Fehlerstromsensor, Fehler.");
            break;
        case EV11ModbusTcpConnection::ErrorGridMonitoringError:
            thing->setStateValue("error", "11: Netzüberwachung ausgelöst.");
            break;
        }

        if (connection->firmwareRevision() >= "0025") {
            thing->setStateValue("digitalInputFlag", QString("0b%1").arg(connection->digitalInputFlag(), 16, 2, QLatin1Char('0')));
            thing->setStateValue("modeR37", connection->modeR37());

            // Energy information only available with meter and 0025
            if (thing->thingClassId() == ev11ThingClassId) {
                thing->setStateValue("currentPower", connection->currentPower());
                thing->setStateValue("sessionEnergy", connection->powerMeter0());
                thing->setStateValue("totalEnergyConsumed", connection->totalEnergyConsumed());
                thing->setStateValue("currentPowerPhaseA", connection->activePowerL1());
                thing->setStateValue("currentPowerPhaseB", connection->activePowerL2());
                thing->setStateValue("currentPowerPhaseC", connection->activePowerL3());
                thing->setStateValue("voltagePhaseA", connection->voltageL1());
                thing->setStateValue("voltagePhaseB", connection->voltageL2());
                thing->setStateValue("voltagePhaseC", connection->voltageL3());
                thing->setStateValue("currentPhaseA", connection->currentL1());
                thing->setStateValue("currentPhaseB", connection->currentL2());
                thing->setStateValue("currentPhaseC", connection->currentL3());
            }

        } else {
            // In firmware 0019 there is no current power register, depending on the CP state we can assume the car is consuming the amount
            // we adjusted, if the car is full, the CP state will change back to B2
            if (thing->thingClassId() == ev11ThingClassId) {
                thing->setStateValue("sessionEnergy", connection->powerMeter0());
                if (connection->chargingState() == PceWallbox::ChargingStateC2 && connection->currentPower() == 0) {
                    // We are currently chargin, but the wallbox reports 0 W (which is expected), let's calculate the theoretical power...
                    double assumedCurrentPower = thing->stateValue("phaseCount").toInt() * 230 * thing->stateValue("maxChargingCurrent").toDouble();
                    qCDebug(dcPcElectric())
                        << "Assuming current power"
                        << assumedCurrentPower
                        << "W ("
                        << thing->stateValue("phaseCount").toInt()
                        << "phases * 230 V *"
                        << thing->stateValue("maxChargingCurrent").toDouble()
                        << "A )";
                    thing->setStateValue("currentPower", assumedCurrentPower);
                } else {
                    thing->setStateValue("currentPower", 0);
                }
            }
        }

        if (m_initialUpdate.value(thing)) {
            m_initialUpdate[thing] = false;

            qCDebug(dcPcElectric()) << "Update initial charger states from charging current register...";

            PceWallbox::ChargingCurrentState chargingCurrentState = PceWallbox::deriveStatesFromRegister(connection->chargingCurrent());
            qCDebug(dcPcElectric()) << chargingCurrentState;
            thing->setStateValue("power", chargingCurrentState.power);
            thing->setStateValue("desiredPhaseCount", chargingCurrentState.desiredPhaseCount);
            if (chargingCurrentState.power)
                thing->setStateValue("maxChargingCurrent", chargingCurrentState.maxChargingCurrent);

            m_chargingCurrentStateBuffer[thing] = chargingCurrentState;

            qCDebug(dcPcElectric()) << "Updating initial settings after connecting...";

            thing->setSettingValue("ledBrightness", connection->ledBrightness());

            switch (connection->digitalInputMode()) {
            case EV11ModbusTcpConnection::DigitalInputModeEnableCharging:
                thing->setSettingValue("digitalInputMode", "0 | Charging allowed");
                break;
            case EV11ModbusTcpConnection::DigitalInputModeEnableChargingInverted:
                thing->setSettingValue("digitalInputMode", "1 | Charging allowed inverted");
                break;
            case EV11ModbusTcpConnection::DigitalInputModePwmS0Enabled:
                thing->setSettingValue("digitalInputMode", "2 | PWM and S0 signaling");
                break;
            case EV11ModbusTcpConnection::DigitalInputModeLimitS0Enabled:
                thing->setSettingValue("digitalInputMode", "3 | Limit and S0 signaling");
                break;
            }

            thing->setStateValue("digitalInputMode", connection->digitalInputMode());

            if (connection->firmwareRevision() >= "0025") {
                thing->setSettingValue("phaseAutoSwitchPause", connection->phaseAutoSwitchPause());
                thing->setSettingValue("phaseAutoSwitchMinChargingTime", connection->phaseAutoSwitchMinChargingTime());
                thing->setSettingValue("forceChargingResume", connection->forceChargingResume() == 1 ? true : false);
            }
        }
    });

    connect(thing, &Thing::settingChanged, connection, [thing, connection](const ParamTypeId &paramTypeId, const QVariant &value) {
        if (paramTypeId == ev11SettingsLedBrightnessParamTypeId || paramTypeId == ev11NoMeterSettingsLedBrightnessParamTypeId) {
            quint16 percentage = value.toUInt();

            qCDebug(dcPcElectric()) << "Setting LED brightness to" << percentage << "%";
            QueuedModbusReply *reply = connection->setLedBrightnessAsync(percentage);
            connect(reply, &QueuedModbusReply::finished, thing, [reply, percentage]() {
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcPcElectric()) << "Could not set led brightness to" << percentage << "%" << reply->errorString();
                    return;
                }

                qCDebug(dcPcElectric()) << "Successfully set led brightness to" << percentage << "%";
            });
        } else if (paramTypeId == ev11SettingsDigitalInputModeParamTypeId || paramTypeId == ev11NoMeterSettingsDigitalInputModeParamTypeId) {
            QString mode = value.toString();
            qCDebug(dcPcElectric()) << "Setting Digital input mode to" << mode;

            EV11ModbusTcpConnection::DigitalInputMode modeValue;
            if (mode == "0 | Charging allowed") {
                modeValue = EV11ModbusTcpConnection::DigitalInputModeEnableCharging;
            } else if (mode == "1 | Charging allowed inverted") {
                modeValue = EV11ModbusTcpConnection::DigitalInputModeEnableChargingInverted;
            } else if (mode == "2 | PWM and S0 signaling") {
                modeValue = EV11ModbusTcpConnection::DigitalInputModePwmS0Enabled;
            } else if (mode == "3 | Limit and S0 signaling") {
                modeValue = EV11ModbusTcpConnection::DigitalInputModeLimitS0Enabled;
            } else {
                qCWarning(dcPcElectric()) << "Unknown digital input mode value" << mode;
                return;
            }

            QueuedModbusReply *reply = connection->setDigitalInputModeAsync(modeValue);
            connect(reply, &QueuedModbusReply::finished, thing, [thing, reply, modeValue]() {
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcPcElectric()) << "Could not set digital input mode to" << modeValue << reply->errorString();
                    return;
                }

                qCDebug(dcPcElectric()) << "Successfully set digital input mode to" << modeValue;
                thing->setStateValue("digitalInputMode", modeValue);
            });
        } else if (paramTypeId == ev11SettingsPhaseAutoSwitchPauseParamTypeId || paramTypeId == ev11NoMeterSettingsPhaseAutoSwitchPauseParamTypeId) {
            quint16 registerValue = value.toUInt();

            qCDebug(dcPcElectric()) << "Setting phase auto switch pause to" << registerValue << "s";
            QueuedModbusReply *reply = connection->setPhaseAutoSwitchPauseAsync(registerValue);
            connect(reply, &QueuedModbusReply::finished, thing, [reply, registerValue]() {
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcPcElectric()) << "Could not set phase auto switch pause to" << registerValue << "s" << reply->errorString();
                    return;
                }

                qCDebug(dcPcElectric()) << "Successfully set phase auto switch pause to" << registerValue << "s";
            });
        } else if (paramTypeId == ev11SettingsPhaseAutoSwitchMinChargingTimeParamTypeId || paramTypeId == ev11NoMeterSettingsPhaseAutoSwitchMinChargingTimeParamTypeId) {
            quint16 registerValue = value.toUInt();

            qCDebug(dcPcElectric()) << "Setting phase auto switch min charging current" << registerValue << "s";
            QueuedModbusReply *reply = connection->setPhaseAutoSwitchMinChargingTimeAsync(registerValue);
            connect(reply, &QueuedModbusReply::finished, thing, [reply, registerValue]() {
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcPcElectric()) << "Could not set phase auto switch min charging current to" << registerValue << "s" << reply->errorString();
                    return;
                }

                qCDebug(dcPcElectric()) << "Successfully set phase auto switch min charging current to" << registerValue << "s";
                //thing->setSettingValue("phaseAutoSwitchMinChargingTime", registerValue);
            });
        } else if (paramTypeId == ev11SettingsForceChargingResumeParamTypeId || paramTypeId == ev11NoMeterSettingsForceChargingResumeParamTypeId) {
            quint16 registerValue = value.toBool() ? 1 : 0;

            qCDebug(dcPcElectric()) << "Setting force charging resume to" << registerValue;
            QueuedModbusReply *reply = connection->setForceChargingResumeAsync(registerValue);
            connect(reply, &QueuedModbusReply::finished, thing, [reply, registerValue]() {
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcPcElectric()) << "Could not set force charging resume to" << registerValue << reply->errorString();
                    return;
                }

                qCDebug(dcPcElectric()) << "Successfully set force charging resume to" << registerValue;
                //thing->setSettingValue("forceChargingResume", registerValue == 1 ? true : false);
            });
        }
    });

    m_connections.insert(thing, connection);
    info->finish(Thing::ThingErrorNoError);

    // Connect right the way if the monitor indicates reachable, otherwise the connect will handle the connect later
    if (monitor->reachable())
        connection->connectDevice();
}
