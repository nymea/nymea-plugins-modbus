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

#include "integrationpluginpcelectric.h"
#include "pcelectricdiscovery.h"
#include "plugininfo.h"

#include <hardwaremanager.h>
#include <hardware/electricity.h>

IntegrationPluginPcElectric::IntegrationPluginPcElectric()
{

}

void IntegrationPluginPcElectric::init()
{
    //qCCritical(dcPcElectric()) << QString("%1").arg(QString::number(49155, 2));
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
    connect(discovery, &PcElectricDiscovery::discoveryFinished, info, [=](){
        foreach (const PcElectricDiscovery::Result &result, discovery->results()) {

            ThingDescriptor descriptor(ev11ThingClassId, "PCE EV11.3 (" + result.serialNumber + ")", "Version: " + result.firmwareRevision + " - " + result.networkDeviceInfo.address().toString());
            qCDebug(dcPcElectric()) << "Discovered:" << descriptor.title() << descriptor.description();

            // Check if we already have set up this device
            Things existingThings = myThings().filterByParam(ev11ThingSerialNumberParamTypeId, result.serialNumber);
            if (existingThings.length() == 1) {
                qCDebug(dcPcElectric()) << "This PCE wallbox already exists in the system:" << result.serialNumber << result.networkDeviceInfo;
                descriptor.setThingId(existingThings.first()->id());
            }

            ParamList params;
            params << Param(ev11ThingMacAddressParamTypeId, result.networkDeviceInfo.thingParamValueMacAddress());
            params << Param(ev11ThingHostNameParamTypeId, result.networkDeviceInfo.thingParamValueHostName());
            params << Param(ev11ThingAddressParamTypeId, result.networkDeviceInfo.thingParamValueAddress());
            params << Param(ev11ThingSerialNumberParamTypeId, result.serialNumber);
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

    connect(info, &ThingSetupInfo::aborted, monitor, [=](){
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
            connect(monitor, &NetworkDeviceMonitor::reachableChanged, info, [=](bool reachable){
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
    chargingCurrentState.power = thing->stateValue(ev11PowerStateTypeId).toBool();
    chargingCurrentState.maxChargingCurrent = thing->stateValue(ev11MaxChargingCurrentStateTypeId).toDouble();
    chargingCurrentState.desiredPhaseCount = thing->stateValue(ev11DesiredPhaseCountStateTypeId).toDouble();
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

    if (info->action().actionTypeId() == ev11PowerActionTypeId) {

        bool power = info->action().paramValue(ev11PowerActionPowerParamTypeId).toBool();
        qCDebug(dcPcElectric()) << "Set charging enabled to" << power;

        // Update buffer
        m_chargingCurrentStateBuffer[thing].power = power;

        quint16 registerValue = PceWallbox::deriveRegisterFromStates(m_chargingCurrentStateBuffer.value(thing));
        qCDebug(dcPcElectric()) << "Writing charging current register" << registerValue;
        QueuedModbusReply *reply = connection->setChargingCurrentAsync(registerValue);
        connect(reply, &QueuedModbusReply::finished, info, [reply, info, thing, power, registerValue](){
            if (reply->error() != QModbusDevice::NoError) {
                qCWarning(dcPcElectric()) << "Could not set power state to" << power << "(" << registerValue << ")" << reply->errorString();
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }

            qCDebug(dcPcElectric()) << "Successfully set power state to" << power << "(" << registerValue << ")";
            thing->setStateValue(ev11PowerStateTypeId, power);
            info->finish(Thing::ThingErrorNoError);
        });

        return;

    } else if (info->action().actionTypeId() == ev11MaxChargingCurrentActionTypeId) {

        uint desiredChargingCurrent = info->action().paramValue(ev11MaxChargingCurrentActionMaxChargingCurrentParamTypeId).toUInt();
        qCDebug(dcPcElectric()) << "Set max charging current to" << desiredChargingCurrent << "A";

        // Update buffer
        m_chargingCurrentStateBuffer[thing].maxChargingCurrent = desiredChargingCurrent;

        quint16 registerValue = PceWallbox::deriveRegisterFromStates(m_chargingCurrentStateBuffer.value(thing));
        qCDebug(dcPcElectric()) << "Writing charging current register" << registerValue;
        QueuedModbusReply *reply = connection->setChargingCurrentAsync(registerValue);
        connect(reply, &QueuedModbusReply::finished, info, [reply, info, thing, desiredChargingCurrent](){
            if (reply->error() != QModbusDevice::NoError) {
                qCWarning(dcPcElectric()) << "Could not set charging current to" << desiredChargingCurrent << reply->errorString();
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }

            qCDebug(dcPcElectric()) << "Successfully set charging current (" << desiredChargingCurrent << ")";
            thing->setStateValue(ev11MaxChargingCurrentStateTypeId, desiredChargingCurrent);
            info->finish(Thing::ThingErrorNoError);
        });

        return;

    } else if (info->action().actionTypeId() == ev11DesiredPhaseCountActionTypeId) {

        uint desiredPhaseCount = info->action().paramValue(ev11DesiredPhaseCountActionDesiredPhaseCountParamTypeId).toUInt();
        qCDebug(dcPcElectric()) << "Set desried phase count to" << desiredPhaseCount;

        // Update buffer
        m_chargingCurrentStateBuffer[thing].desiredPhaseCount = desiredPhaseCount;

        quint16 registerValue = PceWallbox::deriveRegisterFromStates(m_chargingCurrentStateBuffer.value(thing));
        qCDebug(dcPcElectric()) << "Writing charging current register" << registerValue;
        QueuedModbusReply *reply = connection->setChargingCurrentAsync(registerValue);
        connect(reply, &QueuedModbusReply::finished, info, [reply, info, thing, desiredPhaseCount](){
            if (reply->error() != QModbusDevice::NoError) {
                qCWarning(dcPcElectric()) << "Could not set desired phase count to" << desiredPhaseCount << reply->errorString();
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }

            qCDebug(dcPcElectric()) << "Successfully set phase count (" << desiredPhaseCount << ")";
            thing->setStateValue(ev11DesiredPhaseCountStateTypeId, desiredPhaseCount);
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
    connect(monitor, &NetworkDeviceMonitor::reachableChanged, thing, [=](bool reachable){
        if (!thing->setupComplete())
            return;

        qCDebug(dcPcElectric()) << "Network device monitor for" << thing->name() << (reachable ? "is now reachable" : "is not reachable any more" );
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
    connect(connection, &PceWallbox::reachableChanged, thing, [this, thing](bool reachable){
        qCInfo(dcPcElectric()) << "Reachable changed to" << reachable << "for" << thing;
        m_initialUpdate[thing] = true;
        thing->setStateValue("connected", reachable);
    });

    connect(connection, &PceWallbox::updateFinished, thing, [this, thing, connection](){
        qCDebug(dcPcElectric()) << "Update finished for" << thing;
        qCDebug(dcPcElectric()) << connection;
        if (!connection->phaseAutoSwitch()) {
            // Note: if auto phase switching is disabled, the wallbox forces 3 phase charging
            thing->setStatePossibleValues(ev11DesiredPhaseCountStateTypeId, { 3 }); // Disable phase switching (default 3)
            thing->setStateValue(ev11DesiredPhaseCountStateTypeId, 3);
            thing->setStateValue(ev11PhaseCountStateTypeId, 3);
        } else {
            thing->setStatePossibleValues(ev11DesiredPhaseCountStateTypeId, { 1, 3 }); // Enable phase switching
        }

        if (connection->chargingRelayState() != EV11ModbusTcpConnection::ChargingRelayStateNoCharging) {
            if (connection->chargingRelayState() == EV11ModbusTcpConnection::ChargingRelayStateSinglePhase) {
                thing->setStateValue(ev11PhaseCountStateTypeId, 1);
            } else if (connection->chargingRelayState() == EV11ModbusTcpConnection::ChargingRelayStateTheePhase) {
                thing->setStateValue(ev11PhaseCountStateTypeId, 3);
            }
        }

        thing->setStateMaxValue(ev11MaxChargingCurrentStateTypeId, connection->maxChargingCurrentDip() / 1000);
        thing->setStateValue(ev11PluggedInStateTypeId, connection->chargingState() >= PceWallbox::ChargingStateB1 &&
                                                           connection->chargingState() < PceWallbox::ChargingStateError);

        thing->setStateValue(ev11ChargingStateTypeId, connection->chargingState() == PceWallbox::ChargingStateC2);
        if (connection->chargingRelayState() != EV11ModbusTcpConnection::ChargingRelayStateNoCharging) {
            thing->setStateValue(ev11PhaseCountStateTypeId, connection->chargingRelayState() == EV11ModbusTcpConnection::ChargingRelayStateSinglePhase ? 1 : 3);
        }

        switch (connection->chargingState()) {
        case PceWallbox::ChargingStateInitializing:
            thing->setStateValue(ev11StatusStateTypeId, "Init");
            break;
        case PceWallbox::ChargingStateA1:
            thing->setStateValue(ev11StatusStateTypeId, "A1");
            break;
        case PceWallbox::ChargingStateA2:
            thing->setStateValue(ev11StatusStateTypeId, "A2");
            break;
        case PceWallbox::ChargingStateB1:
            thing->setStateValue(ev11StatusStateTypeId, "B1");
            break;
        case PceWallbox::ChargingStateB2:
            thing->setStateValue(ev11StatusStateTypeId, "B2");
            break;
        case PceWallbox::ChargingStateC1:
            thing->setStateValue(ev11StatusStateTypeId, "C1");
            break;
        case PceWallbox::ChargingStateC2:
            thing->setStateValue(ev11StatusStateTypeId, "C2");
            break;
        case PceWallbox::ChargingStateError:
            thing->setStateValue(ev11StatusStateTypeId, "F");
            break;
        }

        thing->setStateValue(ev11CurrentVersionStateTypeId, connection->firmwareRevision());
        thing->setStateValue(ev11SessionEnergyStateTypeId, connection->powerMeter0());
        thing->setStateValue(ev11TemperatureStateTypeId, connection->temperature());

        switch (connection->error()) {
        case EV11ModbusTcpConnection::ErrorNoError:
            thing->setStateValue(ev11ErrorStateTypeId, "Kein Fehler aktiv");
            break;
        case EV11ModbusTcpConnection::ErrorOverheating:
            thing->setStateValue(ev11ErrorStateTypeId, "1: Übertemperatur. Ladevorgang wird automatisch fortgesetzt.");
            break;
        case EV11ModbusTcpConnection::ErrorDCFaultCurrent:
            thing->setStateValue(ev11ErrorStateTypeId, "2: DC Fehlerstromsensor ausgelöst.");
            break;
        case EV11ModbusTcpConnection::ErrorChargingWithVentilation:
            thing->setStateValue(ev11ErrorStateTypeId, "3: Ladeanforderung mit Belüftung.");
            break;
        case EV11ModbusTcpConnection::ErrorCPErrorEF:
            thing->setStateValue(ev11ErrorStateTypeId, "4: CP Signal, Fehlercode E oder F.");
            break;
        case EV11ModbusTcpConnection::ErrorCPErrorBypass:
            thing->setStateValue(ev11ErrorStateTypeId, "5: CP Signal, bypass.");
            break;
        case EV11ModbusTcpConnection::ErrorCPErrorDiodFault:
            thing->setStateValue(ev11ErrorStateTypeId, "6: CP Signal, Diode defekt.");
            break;
        case EV11ModbusTcpConnection::ErrorDCFaultCurrentCalibrating:
            thing->setStateValue(ev11ErrorStateTypeId, "7: DC Fehlerstromsensor, Kalibrirung.");
            break;
        case EV11ModbusTcpConnection::ErrorDCFaultCurrentCommunication:
            thing->setStateValue(ev11ErrorStateTypeId, "8: DC Fehlerstromsensor, Kommunikationsfehler.");
            break;
        case EV11ModbusTcpConnection::ErrorDCFaultCurrentError:
            thing->setStateValue(ev11ErrorStateTypeId, "9: DC Fehlerstromsensor, Fehler.");
            break;
        }

        if (m_initialUpdate.value(thing)) {

            m_initialUpdate[thing] = false;

            qCDebug(dcPcElectric()) << "Update initial charger states from charging current register...";

            PceWallbox::ChargingCurrentState chargingCurrentState = PceWallbox::deriveStatesFromRegister(connection->chargingCurrent());
            qCDebug(dcPcElectric()) << chargingCurrentState;
            thing->setStateValue(ev11PowerStateTypeId, chargingCurrentState.power);
            thing->setStateValue(ev11DesiredPhaseCountStateTypeId, chargingCurrentState.desiredPhaseCount);
            if (chargingCurrentState.power) {
                thing->setStateValue(ev11MaxChargingCurrentStateTypeId, chargingCurrentState.maxChargingCurrent);
            }

            m_chargingCurrentStateBuffer[thing] = chargingCurrentState;

            qCDebug(dcPcElectric()) << "Updating initial settings after connecting...";

            thing->setSettingValue(ev11SettingsLedBrightnessParamTypeId, connection->ledBrightness());

            switch (connection->digitalInputMode()) {
            case EV11ModbusTcpConnection::DigitalInputModeEnableCharging:
                thing->setSettingValue(ev11SettingsDigitalInputModeParamTypeId, "0 | Charging allowed");
                break;
            case EV11ModbusTcpConnection::DigitalInputModeEnableChargingInverted:
                thing->setSettingValue(ev11SettingsDigitalInputModeParamTypeId, "1 | Charging allowed inverted");
                break;
            case EV11ModbusTcpConnection::DigitalInputModePwmS0Enabled:
                thing->setSettingValue(ev11SettingsDigitalInputModeParamTypeId, "2 | PWM and S0 signaling");
                break;
            case EV11ModbusTcpConnection::DigitalInputModeLimitS0Enabled:
                thing->setSettingValue(ev11SettingsDigitalInputModeParamTypeId, "3 | Limit and S0 signaling");
                break;
            }
            thing->setStateValue(ev11DigitalInputModeStateTypeId, connection->digitalInputMode());


            if (connection->firmwareRevision() >= "0022") {
                thing->setSettingValue(ev11SettingsPhaseAutoSwitchPauseParamTypeId, connection->phaseAutoSwitchPause());
                thing->setStateValue(ev11SettingsPhaseAutoSwitchMinChargingTimeParamTypeId, connection->phaseAutoSwitchMinChargingTime());
                thing->setStateValue(ev11SettingsForceChargingResumeParamTypeId, connection->forceChargingResume() == 1 ? true : false);
            }
        }

        if (connection->firmwareRevision() >= "0022") {
            thing->setStateValue(ev11CurrentPowerStateTypeId, connection->currentPower());
            thing->setStateValue(ev11DigitalInputFlagStateTypeId, QString("0b%1").arg(connection->digitalInputFlag(), 16, 2, QLatin1Char('0')));
        }

    });

    connect(thing, &Thing::settingChanged, connection, [thing, connection](const ParamTypeId &paramTypeId, const QVariant &value){

        if (paramTypeId == ev11SettingsLedBrightnessParamTypeId) {
            quint16 percentage = value.toUInt();

            qCDebug(dcPcElectric()) << "Setting LED brightness to" << percentage << "%";
            QueuedModbusReply *reply = connection->setLedBrightnessAsync(percentage);
            connect(reply, &QueuedModbusReply::finished, thing, [reply, percentage](){
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcPcElectric()) << "Could not set led brightness to" << percentage << "%" << reply->errorString();
                    return;
                }

                qCDebug(dcPcElectric()) << "Successfully set led brightness to" << percentage << "%";
            });
        } else if (paramTypeId == ev11SettingsDigitalInputModeParamTypeId) {
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
            connect(reply, &QueuedModbusReply::finished, thing, [thing, reply, modeValue](){
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcPcElectric()) << "Could not set digital input mode to" << modeValue << reply->errorString();
                    return;
                }

                qCDebug(dcPcElectric()) << "Successfully set digital input mode to" << modeValue;
                thing->setStateValue(ev11DigitalInputModeStateTypeId, modeValue);
            });
        } else if (paramTypeId == ev11SettingsPhaseAutoSwitchPauseParamTypeId) {
            quint16 registerValue = value.toUInt();

            qCDebug(dcPcElectric()) << "Setting phase auto switch pause to" << registerValue << "s";
            QueuedModbusReply *reply = connection->setPhaseAutoSwitchPauseAsync(registerValue);
            connect(reply, &QueuedModbusReply::finished, thing, [reply, registerValue](){
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcPcElectric()) << "Could not set phase auto switch pause to" << registerValue << "s" << reply->errorString();
                    return;
                }

                qCDebug(dcPcElectric()) << "Successfully set phase auto switch pause to" << registerValue << "s";
            });
        } else if (paramTypeId == ev11SettingsPhaseAutoSwitchMinChargingTimeParamTypeId) {
            quint16 registerValue = value.toUInt();

            qCDebug(dcPcElectric()) << "Setting phase auto switch min charging current" << registerValue << "s";
            QueuedModbusReply *reply = connection->setPhaseAutoSwitchMinChargingTimeAsync(registerValue);
            connect(reply, &QueuedModbusReply::finished, thing, [reply, registerValue](){
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcPcElectric()) << "Could not set phase auto switch min charging current to" << registerValue << "s" << reply->errorString();
                    return;
                }

                qCDebug(dcPcElectric()) << "Successfully set phase auto switch min charging current to" << registerValue << "s";
            });
        } else if (paramTypeId == ev11SettingsForceChargingResumeParamTypeId) {
            quint16 registerValue = value.toBool() ? 1 : 0;

            qCDebug(dcPcElectric()) << "Setting force charging resume to" << registerValue;
            QueuedModbusReply *reply = connection->setForceChargingResumeAsync(registerValue);
            connect(reply, &QueuedModbusReply::finished, thing, [reply, registerValue](){
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcPcElectric()) << "Could not set force charging resume to" << registerValue << reply->errorString();
                    return;
                }

                qCDebug(dcPcElectric()) << "Successfully set force charging resume to" << registerValue;
            });
        }
    });

    m_connections.insert(thing, connection);
    info->finish(Thing::ThingErrorNoError);

    // Connect reight the way if the monitor indicates reachable, otherwise the connect will handle the connect later
    if (monitor->reachable())
        connection->connectDevice();
}

