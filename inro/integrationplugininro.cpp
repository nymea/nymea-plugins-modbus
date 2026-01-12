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

#include "integrationplugininro.h"
#include "plugininfo.h"

#include <hardwaremanager.h>
#include <hardware/electricity.h>

#include "pantaboxdiscovery.h"

IntegrationPluginInro::IntegrationPluginInro()
{

}

void IntegrationPluginInro::discoverThings(ThingDiscoveryInfo *info)
{
    PantaboxDiscovery *discovery = new PantaboxDiscovery(info);
    connect(discovery, &PantaboxDiscovery::discoveryFinished, info, [this, info, discovery](){

        foreach (const PantaboxDiscovery::Result &result, discovery->results()) {
            QString title = QString("PANTABOX - %1").arg(result.deviceInfo.serialNumber);
            QString description = QString("%1 (%2)").arg(result.deviceInfo.macAddress.toString(), result.deviceInfo.ipAddress.toString());
            ThingDescriptor descriptor(pantaboxThingClassId, title, description);

            // Check if we already have set up this device
            Things existingThings = myThings().filterByParam(pantaboxThingSerialNumberParamTypeId, result.deviceInfo.serialNumber);
            if (existingThings.length() == 1) {
                qCDebug(dcInro()) << "This PANTABOX already exists in the system:" << result.deviceInfo.serialNumber << result.deviceInfo.ipAddress.toString();
                descriptor.setThingId(existingThings.first()->id());
            }

            ParamList params;
            params << Param(pantaboxThingMacAddressParamTypeId, result.deviceInfo.macAddress.toString());
            params << Param(pantaboxThingSerialNumberParamTypeId, result.deviceInfo.serialNumber);
            descriptor.setParams(params);
            info->addThingDescriptor(descriptor);
        }

        info->finish(Thing::ThingErrorNoError);
    });

    discovery->startDiscovery();
}

void IntegrationPluginInro::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcInro()) << "Setup thing" << thing << thing->params();

    if (m_connections.contains(thing)) {
        qCDebug(dcInro()) << "Reconfiguring existing thing" << thing->name();
        Pantabox *connection = m_connections.take(thing);
        connection->modbusTcpMaster()->disconnectDevice();
        connection->deleteLater();
        thing->setStateValue(pantaboxConnectedStateTypeId, false);
    }

    QString serialNumber = thing->paramValue(pantaboxThingSerialNumberParamTypeId).toString();

    if (serialNumber.isEmpty()) {
        qCWarning(dcInro()) << "Could not set up PANTABOX because the configured serial number is empty" << thing->params();
        info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The serial number is not known. Please reconfigure the thing."));
        return;
    }

    setupConnection(info);
}

void IntegrationPluginInro::postSetupThing(Thing *thing)
{
    qCDebug(dcInro()) << "Post setup thing" << thing->name();
    if (!m_refreshTimer) {
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(2);
        connect(m_refreshTimer, &PluginTimer::timeout, this, [this] {
            foreach (Pantabox *connection, m_connections) {
                if (connection->reachable()) {
                    connection->update();
                }
            }
        });

        qCDebug(dcInro()) << "Starting refresh timer...";
        m_refreshTimer->start();
    }
}

void IntegrationPluginInro::executeAction(ThingActionInfo *info)
{
    if (info->thing()->thingClassId() == pantaboxThingClassId) {

        Pantabox *connection = m_connections.value(info->thing());

        if (!connection->reachable()) {
            qCWarning(dcInro()) << "Cannot execute action. The PANTABOX is not reachable";
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        if (info->action().actionTypeId() == pantaboxPowerActionTypeId) {
            bool power = info->action().paramValue(pantaboxPowerActionPowerParamTypeId).toBool();

            // Play/pause charging session feature is available since Modbus Tcp version 1.2 (0x0001 0x0002) 0x10002 = 65538
            if (connection->modbusTcpVersion() < 65538) {
                // When power is set by user, charging is going to stop or start depending on setting
                qCDebug(dcInro()) << "Set power by user" << (power ? 1 : 0);
                QModbusReply *reply = connection->setChargingEnabled(power ? 1 : 0);

                if (!reply) {
                    qCWarning(dcInro()) << "Execute action failed because the reply could not be created.";
                    info->finish(Thing::ThingErrorHardwareFailure);
                    return;
                }

                connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
                connect(reply, &QModbusReply::finished, info, [info, reply, power](){
                    if (reply->error() == QModbusDevice::NoError) {
                        info->thing()->setStateValue(pantaboxPowerStateTypeId, power);
                        qCDebug(dcInro()) << "Set power by user finished successfully";
                        info->finish(Thing::ThingErrorNoError);
                    } else {
                        qCWarning(dcInro()) << "Error setting power by user:" << reply->error() << reply->errorString();
                        info->finish(Thing::ThingErrorHardwareFailure);
                    }
                });
                return;

            } else {

                // Modbus version >= 1.2

                if (info->action().triggeredBy() == Action::TriggeredByUser) {

                    // When power is set by user, charging is going to stop or start depending on setting
                    qCDebug(dcInro()) << "Set power by user" << (power ? 1 : 0);
                    QModbusReply *reply = connection->setChargingEnabled(power ? 1 : 0);

                    if (!reply) {
                        qCWarning(dcInro()) << "Execute action failed because the reply could not be created.";
                        info->finish(Thing::ThingErrorHardwareFailure);
                        return;
                    }

                    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
                    connect(reply, &QModbusReply::finished, info, [info, reply, power](){
                        if (reply->error() == QModbusDevice::NoError) {
                            info->thing()->setStateValue(pantaboxPowerStateTypeId, power);
                            qCDebug(dcInro()) << "Set power by user finished successfully";
                            info->finish(Thing::ThingErrorNoError);
                        } else {
                            qCWarning(dcInro()) << "Error setting power by user:" << reply->error() << reply->errorString();
                            info->finish(Thing::ThingErrorHardwareFailure);
                        }
                    });
                    return;

                } else {
                    // When power is set to 0 by automatisnm, max charging current is set to 0 otherwise take the configured max charging current
                    qCDebug(dcInro()) << "Going to play/pause charging session";

                    quint16 chargingCurrent = power ? info->thing()->stateValue(pantaboxMaxChargingCurrentStateTypeId).toUInt() : 0;
                    QModbusReply *reply = connection->setMaxChargingCurrent(chargingCurrent);

                    if (!reply) {
                        qCWarning(dcInro()) << "Execute action failed because the reply could not be created.";
                        info->finish(Thing::ThingErrorHardwareFailure);
                        return;
                    }

                    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
                    connect(reply, &QModbusReply::finished, info, [info, reply, power, connection](){
                        if (reply->error() == QModbusDevice::NoError) {
                            qCDebug(dcInro()) << (power ? "Play" : "Pause") << "session by energy manager";
                            info->finish(Thing::ThingErrorNoError);
                            if (power) {
                                // Make sure the charging is enabled, just in case
                                QModbusReply *reply = connection->setChargingEnabled(1);
                                connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
                                connect(reply, &QModbusReply::finished, info, [reply](){
                                    if (reply->error() != QModbusDevice::NoError) {
                                        qCWarning(dcInro()) << "Error setting charging enabled:" << reply->error() << reply->errorString();
                                    }
                                });
                            }
                        } else {
                            qCWarning(dcInro()) << "Error setting charging current:" << reply->error() << reply->errorString();
                            info->finish(Thing::ThingErrorHardwareFailure);
                        }

                    });

                    return;
                }
            }
        }

        if (info->action().actionTypeId() == pantaboxMaxChargingCurrentActionTypeId) {
            quint16 chargingCurrent = static_cast<quint16>(qRound(info->action().paramValue(pantaboxMaxChargingCurrentActionMaxChargingCurrentParamTypeId).toDouble()));
            qCDebug(dcInro()) << "PANTABOX: Set max charging current" << chargingCurrent << "A";

            QModbusReply *reply = connection->setMaxChargingCurrent(chargingCurrent);
            if (!reply) {
                qCWarning(dcInro()) << "Execute action failed because the reply could not be created.";
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }

            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, info, [info, reply, chargingCurrent](){
                if (reply->error() == QModbusDevice::NoError) {
                    info->thing()->setStateValue(pantaboxMaxChargingCurrentStateTypeId, chargingCurrent);
                    qCDebug(dcInro()) << "PANTABOX: Set max charging current finished successfully";
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    qCWarning(dcInro()) << "Error setting charging current:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
            return;
        }
    }
}

void IntegrationPluginInro::thingRemoved(Thing *thing)
{
    qCDebug(dcInro()) << "Thing removed" << thing->name();

    if (m_connections.contains(thing)) {
        Pantabox *connection = m_connections.take(thing);
        connection->disconnectDevice();
        connection->deleteLater();
    }

    m_initReadRequired.remove(thing);

    if (myThings().isEmpty() && m_refreshTimer) {
        qCDebug(dcInro()) << "Stopping reconnect timer";
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_refreshTimer);
        m_refreshTimer = nullptr;
    }

    if (myThings().isEmpty() && m_udpDiscovery) {
        qCDebug(dcInro()) << "Destroy UDP discovery since not needed any more";
        m_udpDiscovery->deleteLater();
        m_udpDiscovery = nullptr;
    }
}

void IntegrationPluginInro::setupConnection(ThingSetupInfo *info)
{
    if (!m_udpDiscovery)
        m_udpDiscovery = new PantaboxUdpDiscovery(this);

    Thing *thing = info->thing();

    Pantabox *connection = new Pantabox(QHostAddress(), 502, 1, this);
    connect(info, &ThingSetupInfo::aborted, connection, &Pantabox::deleteLater);

    connect(m_udpDiscovery, &PantaboxUdpDiscovery::pantaboxDiscovered, connection, [connection, thing](const PantaboxUdpDiscovery::DeviceInfo &deviceInfo){
        QString serialNumber = thing->paramValue(pantaboxThingSerialNumberParamTypeId).toString();
        if (deviceInfo.serialNumber != serialNumber)
            return;

        connection->modbusTcpMaster()->setHostAddress(deviceInfo.ipAddress);

        if (!connection->reachable()) {

            if (connection->modbusTcpMaster()->connected()) {
                qCDebug(dcInro()) << "Received discovery paket for" << thing->name() <<
                    "which is not reachable but the TCP socket is still connected. Reconnecting the TCP socket on" <<
                    deviceInfo.ipAddress.toString();
                connection->modbusTcpMaster()->reconnectDevice();
            } else {
                qCDebug(dcInro()) << "Received discovery paket for" << thing->name() <<
                    "which is not reachable and not connected. Start connecting to the PANTABOX on" <<
                    deviceInfo.ipAddress.toString();
                connection->connectDevice();
            }
        }
    });

    // Connection reachability
    connect(connection, &Pantabox::reachableChanged, thing, [thing, connection](bool reachable){
        qCInfo(dcInro()) << "Reachable changed to" << reachable << "for" << thing;
        thing->setStateValue("connected", reachable);

        if (!reachable) {
            // Reset energy live values on disconnected
            thing->setStateValue(pantaboxCurrentPowerStateTypeId, 0);
        } else {
            connection->initialize();
        }
    });

    connect(connection, &Pantabox::initializationFinished, thing, [this, thing, connection](bool success){
        if (success) {
            thing->setStateValue(pantaboxModbusTcpVersionStateTypeId, Pantabox::modbusVersionToString(connection->modbusTcpVersion()));
            m_initReadRequired[thing] = true;
        }
    });

    connect(connection, &Pantabox::updateFinished, thing, [this, thing, connection](){
        qCDebug(dcInro()) << "Update finished for" << thing;
        qCDebug(dcInro()) << connection;

        QString chargingStateString;
        switch(connection->chargingState()) {
        case Pantabox::ChargingStateA:
            chargingStateString = "A";
            break;
        case Pantabox::ChargingStateB:
            chargingStateString = "B";
            break;
        case Pantabox::ChargingStateC:
            chargingStateString = "C";
            break;
        case Pantabox::ChargingStateD:
            chargingStateString = "D";
            break;
        case Pantabox::ChargingStateE:
            chargingStateString = "E";
            break;
        case Pantabox::ChargingStateF:
            chargingStateString = "F";
            break;
        }
        thing->setStateValue(pantaboxChargingStateStateTypeId, chargingStateString);

        // A: not connected
        // B: connected, not charging
        // C: connected, charging
        // D: ventilation required
        // E: F: fault/error
        thing->setStateValue(pantaboxPluggedInStateTypeId, connection->chargingState() >= Pantabox::ChargingStateB);
        thing->setStateValue(pantaboxChargingStateTypeId, connection->chargingState() >= Pantabox::ChargingStateC);
        thing->setStateValue(pantaboxCurrentPowerStateTypeId, connection->currentPower()); // W
        thing->setStateValue(pantaboxSessionEnergyStateTypeId, connection->chargedEnergy() / 1000.0); // Wh
        thing->setStateMaxValue(pantaboxMaxChargingCurrentActionTypeId, connection->maxPossibleChargingCurrent());

        // Phase count is a setting, since we don't get the information from the device.
        // Maybe we could assume the from current power and set charging current how many phases get used,
        // but we could not tell which pashes are active.

        Electricity::Phases phases = Electricity::convertPhasesFromString(thing->setting(pantaboxSettingsPhasesParamTypeId).toString());
        thing->setStateValue(pantaboxPhaseCountStateTypeId, Electricity::getPhaseCount(phases));
        thing->setStateValue(pantaboxUsedPhasesStateTypeId, thing->setting(pantaboxSettingsPhasesParamTypeId).toString());

        // Following states depend on the modbus TCP version, default they will be reset.
        thing->setStateValue(pantaboxFirmwareVersionStateTypeId, connection->firmwareVersion());
        thing->setStateValue(pantaboxTotalEnergyConsumedStateTypeId, connection->absoluteEnergy() / 1000.0); // Wh

        // Sync states only right after the connection
        if (m_initReadRequired.value(thing, false)) {
            qCDebug(dcInro()) << "Set initial charging current and charging enabled values.";
            m_initReadRequired.remove(thing);
            if (connection->maxChargingCurrent() > 0) {
                thing->setStateValue(pantaboxMaxChargingCurrentStateTypeId, connection->maxChargingCurrent());
            }
            thing->setStateValue(pantaboxPowerStateTypeId, connection->chargingEnabled());
        }
    });

    m_connections.insert(thing, connection);
    info->finish(Thing::ThingErrorNoError);
}
