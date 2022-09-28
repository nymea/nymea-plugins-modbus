/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright 2013 - 2021, nymea GmbH, Consolinno Energy GmbH, L. Heizinger
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

#include "integrationpluginstiebeleltron.h"
#include "plugininfo.h"

#include <hardwaremanager.h>
#include <network/networkdevicediscovery.h>

IntegrationPluginStiebelEltron::IntegrationPluginStiebelEltron() {}

void IntegrationPluginStiebelEltron::discoverThings(ThingDiscoveryInfo *info) {
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcStiebelEltron()) << "The network discovery is not available on this platform.";
        info->finish(Thing::ThingErrorUnsupportedFeature,
                     QT_TR_NOOP("The network device discovery is not available."));
        return;
    }

    NetworkDeviceDiscoveryReply *discoveryReply = hardwareManager()->networkDeviceDiscovery()->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=]() {
        foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {
            qCDebug(dcStiebelEltron()) << "Found" << networkDeviceInfo;

            QString title;
            if (networkDeviceInfo.hostName().isEmpty()) {
                title = networkDeviceInfo.address().toString();
            } else {
                if (!networkDeviceInfo.hostName().contains("StiebelEltronISG")) continue;
                title = networkDeviceInfo.hostName() + " (" + networkDeviceInfo.address().toString() + ")";
            }

            QString description;
            if (networkDeviceInfo.macAddressManufacturer().isEmpty()) {
                description = networkDeviceInfo.macAddress();
            } else {
                description =
                    networkDeviceInfo.macAddress() + " (" + networkDeviceInfo.macAddressManufacturer() + ")";
            }

            ThingDescriptor descriptor(stiebelEltronThingClassId, title, description);
            ParamList params;
            params << Param(stiebelEltronThingIpAddressParamTypeId, networkDeviceInfo.address().toString());
            params << Param(stiebelEltronThingMacAddressParamTypeId, networkDeviceInfo.macAddress());
            descriptor.setParams(params);

            // Check if we already have set up this device
            Things existingThings = myThings().filterByParam(stiebelEltronThingMacAddressParamTypeId,
                                                             networkDeviceInfo.macAddress());
            if (existingThings.count() == 1) {
                qCDebug(dcStiebelEltron())
                    << "This connection already exists in the system:" << networkDeviceInfo;
                descriptor.setThingId(existingThings.first()->id());
            }

            info->addThingDescriptor(descriptor);
        }

        info->finish(Thing::ThingErrorNoError);
    });
}

void IntegrationPluginStiebelEltron::startMonitoringAutoThings() {}

void IntegrationPluginStiebelEltron::setupThing(ThingSetupInfo *info) {
    Thing *thing = info->thing();
    qCDebug(dcStiebelEltron()) << "Setup" << thing << thing->params();

    if (thing->thingClassId() == stiebelEltronThingClassId) {
        QHostAddress address(thing->paramValue(stiebelEltronThingIpAddressParamTypeId).toString());
        quint16 port = thing->paramValue(stiebelEltronThingPortParamTypeId).toUInt();
        quint16 slaveId = thing->paramValue(stiebelEltronThingSlaveIdParamTypeId).toUInt();

        StiebelEltronModbusTcpConnection *connection =
            new StiebelEltronModbusTcpConnection(address, port, slaveId, this);

        connect(connection, &StiebelEltronModbusTcpConnection::connectionStateChanged, thing,
                [thing, connection](bool status) {
                    qCDebug(dcStiebelEltron()) << "Connected changed to" << status << "for" << thing;
                    if (status) {
                        connection->update();
                    }

                    thing->setStateValue(stiebelEltronConnectedStateTypeId, status);
                });

        connect(connection, &StiebelEltronModbusTcpConnection::outdoorTemperatureChanged, thing,
                [thing](float outdoorTemperature) {
                    qCDebug(dcStiebelEltron())
                        << thing << "outdoor temperature changed" << outdoorTemperature << "°C";
                    thing->setStateValue(stiebelEltronOutdoorTemperatureStateTypeId, outdoorTemperature);
                });

        connect(connection, &StiebelEltronModbusTcpConnection::flowTemperatureChanged, thing,
                [thing](float flowTemperature) {
                    qCDebug(dcStiebelEltron())
                        << thing << "flow temperature changed" << flowTemperature << "°C";
                    thing->setStateValue(stiebelEltronFlowTemperatureStateTypeId, flowTemperature);
                });

        connect(connection, &StiebelEltronModbusTcpConnection::hotWaterTemperatureChanged, thing,
                [thing](float hotWaterTemperature) {
                    qCDebug(dcStiebelEltron())
                        << thing << "hot water temperature changed" << hotWaterTemperature << "°C";
                    thing->setStateValue(stiebelEltronHotWaterTemperatureStateTypeId, hotWaterTemperature);
                });

        connect(connection, &StiebelEltronModbusTcpConnection::storageTankTemperatureChanged, thing,
                [thing](float storageTankTemperature) {
                    qCDebug(dcStiebelEltron())
                        << thing << "Storage tank temperature changed" << storageTankTemperature << "°C";
                    thing->setStateValue(stiebelEltronStorageTankTemperatureStateTypeId,
                                         storageTankTemperature);
                });

        connect(connection, &StiebelEltronModbusTcpConnection::returnTemperatureChanged, thing,
                [thing](float returnTemperature) {
                    qCDebug(dcStiebelEltron())
                        << thing << "return temperature changed" << returnTemperature << "°C";
                    thing->setStateValue(stiebelEltronReturnTemperatureStateTypeId, returnTemperature);
                });

        connect(connection, &StiebelEltronModbusTcpConnection::heatingEnergyChanged, thing,
                [thing](quint32 heatingEnergy) {
                    // kWh and MWh of energy are stored in two registers an read as
                    // an uint32. The following arithmetic splits the uint32 into
                    // two uint16 and sums up the MWh and kWh values.
                    quint32 correctedEnergy = (heatingEnergy >> 16) + (heatingEnergy & 0xFFFF) * 1000;
                    qCDebug(dcStiebelEltron())
                        << thing << "Heating energy changed" << correctedEnergy << "kWh";
                    thing->setStateValue(stiebelEltronHeatingEnergyStateTypeId, correctedEnergy);
                });

        connect(connection, &StiebelEltronModbusTcpConnection::hotWaterEnergyChanged, thing,
                [thing](quint32 hotWaterEnergy) {
                    // see comment in heatingEnergyChanged
                    quint32 correctedEnergy = (hotWaterEnergy >> 16) + (hotWaterEnergy & 0xFFFF) * 1000;
                    qCDebug(dcStiebelEltron())
                        << thing << "Hot Water energy changed" << correctedEnergy << "kWh";
                    thing->setStateValue(stiebelEltronHotWaterEnergyStateTypeId, correctedEnergy);
                });

        connect(connection, &StiebelEltronModbusTcpConnection::consumedEnergyHeatingChanged, thing,
                [thing](quint32 consumedEnergyHeatingEnergy) {
                    // see comment in heatingEnergyChanged
                    quint32 correctedEnergy =
                        (consumedEnergyHeatingEnergy >> 16) + (consumedEnergyHeatingEnergy & 0xFFFF) * 1000;
                    qCDebug(dcStiebelEltron())
                        << thing << "Consumed energy Heating changed" << correctedEnergy << "kWh";
                    thing->setStateValue(stiebelEltronConsumedEnergyHeatingStateTypeId, correctedEnergy);
                });

        connect(connection, &StiebelEltronModbusTcpConnection::consumedEnergyHotWaterChanged, thing,
                [thing](quint32 consumedEnergyHotWaterEnergy) {
                    // see comment in heatingEnergyChanged
                    quint32 correctedEnergy =
                        (consumedEnergyHotWaterEnergy >> 16) + (consumedEnergyHotWaterEnergy & 0xFFFF) * 1000;
                    qCDebug(dcStiebelEltron())
                        << thing << "Consumed energy hot water changed" << correctedEnergy << "kWh";
                    thing->setStateValue(stiebelEltronConsumedEnergyHotWaterStateTypeId, correctedEnergy);
                });

        connect(connection, &StiebelEltronModbusTcpConnection::operatingModeChanged, thing,
                [thing](StiebelEltronModbusTcpConnection::OperatingMode operatingMode) {
                    qCDebug(dcStiebelEltron()) << thing << "operating mode changed " << operatingMode;
                    switch (operatingMode) {
                        case StiebelEltronModbusTcpConnection::OperatingModeEmergency:
                            thing->setStateValue(stiebelEltronOperatingModeStateTypeId, "Emergency");
                            break;
                        case StiebelEltronModbusTcpConnection::OperatingModeStandby:
                            thing->setStateValue(stiebelEltronOperatingModeStateTypeId, "Standby");
                            break;
                        case StiebelEltronModbusTcpConnection::OperatingModeProgram:
                            thing->setStateValue(stiebelEltronOperatingModeStateTypeId, "Program");
                            break;
                        case StiebelEltronModbusTcpConnection::OperatingModeComfort:
                            thing->setStateValue(stiebelEltronOperatingModeStateTypeId, "Comfort");
                            break;
                        case StiebelEltronModbusTcpConnection::OperatingModeEco:
                            thing->setStateValue(stiebelEltronOperatingModeStateTypeId, "Eco");
                            break;
                        case StiebelEltronModbusTcpConnection::OperatingModeHotWater:
                            thing->setStateValue(stiebelEltronOperatingModeStateTypeId, "Hot water");
                            break;
                    }
                });

        connect(connection, &StiebelEltronModbusTcpConnection::systemStatusChanged, thing,
                [thing](uint16_t systemStatus) {
                    qCDebug(dcStiebelEltron()) << thing << "System status changed " << systemStatus;
                    thing->setStateValue(stiebelEltronPumpOneStateTypeId, systemStatus & (1 << 0));
                    thing->setStateValue(stiebelEltronPumpTwoStateTypeId, systemStatus & (1 << 1));
                    thing->setStateValue(stiebelEltronHeatingUpStateTypeId, systemStatus & (1 << 2));
                    thing->setStateValue(stiebelEltronAuxHeatingStateTypeId, systemStatus & (1 << 3));
                    thing->setStateValue(stiebelEltronHeatingStateTypeId, systemStatus & (1 << 4));
                    thing->setStateValue(stiebelEltronHotWaterStateTypeId, systemStatus & (1 << 5));
                    thing->setStateValue(stiebelEltronCompressorStateTypeId, systemStatus & (1 << 6));
                    thing->setStateValue(stiebelEltronSummerModeStateTypeId, systemStatus & (1 << 7));
                    thing->setStateValue(stiebelEltronCoolingModeStateTypeId, systemStatus & (1 << 8));
                    thing->setStateValue(stiebelEltronDefrostingStateTypeId, systemStatus & (1 << 9));
                    thing->setStateValue(stiebelEltronSilentModeStateTypeId, systemStatus & (1 << 10));
                    thing->setStateValue(stiebelEltronSilentMode2StateTypeId, systemStatus & (1 << 11));
                });

        connect(connection, &StiebelEltronModbusTcpConnection::sgReadyStateChanged, thing,
                [thing](StiebelEltronModbusTcpConnection::SmartGridState smartGridState) {
                    qCDebug(dcStiebelEltron()) << thing << "SG Ready mode changed" << smartGridState;
                    switch (smartGridState) {
                        case StiebelEltronModbusTcpConnection::SmartGridStateModeOne:
                            thing->setStateValue(stiebelEltronSgReadyModeStateTypeId, "Off");
                            break;
                        case StiebelEltronModbusTcpConnection::SmartGridStateModeTwo:
                            thing->setStateValue(stiebelEltronSgReadyModeStateTypeId, "Low");
                            break;
                        case StiebelEltronModbusTcpConnection::SmartGridStateModeThree:
                            thing->setStateValue(stiebelEltronSgReadyModeStateTypeId, "Standard");
                            break;
                        case StiebelEltronModbusTcpConnection::SmartGridStateModeFour:
                            thing->setStateValue(stiebelEltronSgReadyModeStateTypeId, "High");
                            break;
                    }
                });
        connect(connection, &StiebelEltronModbusTcpConnection::sgReadyActiveChanged, thing,
                [thing](bool smartGridActive) {
                    qCDebug(dcStiebelEltron()) << thing << "SG Ready activation changed" << smartGridActive;
                    thing->setStateValue(stiebelEltronSgReadyActiveStateTypeId, smartGridActive);
                });

        m_connections.insert(thing, connection);
        connection->connectDevice();

        info->finish(Thing::ThingErrorNoError);
    }
}

void IntegrationPluginStiebelEltron::postSetupThing(Thing *thing) {
    if (thing->thingClassId() == stiebelEltronThingClassId) {
        if (!m_pluginTimer) {
            qCDebug(dcStiebelEltron()) << "Starting plugin timer...";
            m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(10);
            connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
                foreach (StiebelEltronModbusTcpConnection *connection, m_connections) {
                    if (connection->connected()) {
                        connection->update();
                    }
                }
            });

            m_pluginTimer->start();
        }
    }
}

void IntegrationPluginStiebelEltron::thingRemoved(Thing *thing) {
    if (thing->thingClassId() == stiebelEltronThingClassId && m_connections.contains(thing)) {
        m_connections.take(thing)->deleteLater();
    }

    if (myThings().isEmpty() && m_pluginTimer) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginStiebelEltron::executeAction(ThingActionInfo *info) {
    Thing *thing = info->thing();
    StiebelEltronModbusTcpConnection *connection = m_connections.value(thing);

    if (!connection->connected()) {
        qCWarning(dcStiebelEltron()) << "Could not execute action. The modbus connection is currently "
                                        "not available.";
        info->finish(Thing::ThingErrorHardwareNotAvailable);
        return;
    }

    // Got this from StiebelEltron plugin, not sure if necessary
    if (thing->thingClassId() != stiebelEltronThingClassId) {
        info->finish(Thing::ThingErrorNoError);
    }

    if (info->action().actionTypeId() == stiebelEltronSgReadyActiveActionTypeId) {
        bool sgReadyActiveBool =
            info->action().paramValue(stiebelEltronSgReadyActiveActionSgReadyActiveParamTypeId).toBool();
        qCDebug(dcStiebelEltron()) << "Execute action" << info->action().actionTypeId().toString()
                                   << info->action().params();
        qCDebug(dcStiebelEltron()) << "Value: " << sgReadyActiveBool;

        QModbusReply *reply = connection->setSgReadyActive(sgReadyActiveBool);
        if (!reply) {
            qCWarning(dcStiebelEltron()) << "Execute action failed because the "
                                            "reply could not be created.";
            info->finish(Thing::ThingErrorHardwareFailure);
            return;
        }

        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        connect(reply, &QModbusReply::finished, info, [info, reply, sgReadyActiveBool] {
            if (reply->error() != QModbusDevice::NoError) {
                qCWarning(dcStiebelEltron())
                    << "Set SG ready activation finished with error" << reply->errorString();
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }

            qCDebug(dcStiebelEltron()) << "Execute action finished successfully"
                                       << info->action().actionTypeId().toString() << info->action().params();
            info->thing()->setStateValue(stiebelEltronSgReadyActiveStateTypeId, sgReadyActiveBool);
            info->finish(Thing::ThingErrorNoError);
        });

        connect(reply, &QModbusReply::errorOccurred, this, [reply](QModbusDevice::Error error) {
            qCWarning(dcStiebelEltron())
                << "Modbus reply error occurred while execute action" << error << reply->errorString();
            emit reply->finished();  // To make sure it will be deleted
        });
    } else if (info->action().actionTypeId() == stiebelEltronSgReadyModeActionTypeId) {
        QString sgReadyModeString =
            info->action().paramValue(stiebelEltronSgReadyModeActionSgReadyModeParamTypeId).toString();
        qCDebug(dcStiebelEltron()) << "Execute action" << info->action().actionTypeId().toString()
                                   << info->action().params();
        StiebelEltronModbusTcpConnection::SmartGridState sgReadyState;
        if (sgReadyModeString == "Off") {
            sgReadyState = StiebelEltronModbusTcpConnection::SmartGridStateModeOne;
        } else if (sgReadyModeString == "Low") {
            sgReadyState = StiebelEltronModbusTcpConnection::SmartGridStateModeTwo;
        } else if (sgReadyModeString == "Standard") {
            sgReadyState = StiebelEltronModbusTcpConnection::SmartGridStateModeThree;
        } else if (sgReadyModeString == "High") {
            sgReadyState = StiebelEltronModbusTcpConnection::SmartGridStateModeFour;
        } else {
            qCWarning(dcStiebelEltron())
                << "Failed to set SG Ready mode. An unknown SG Ready mode was passed: " << sgReadyModeString;
            info->finish(Thing::ThingErrorHardwareFailure);  // TODO better matching error type?
            return;
        }

        QModbusReply *reply = connection->setSgReadyState(sgReadyState);
        if (!reply) {
            qCWarning(dcStiebelEltron()) << "Execute action failed because the "
                                            "reply could not be created.";
            info->finish(Thing::ThingErrorHardwareFailure);
            return;
        }

        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        connect(reply, &QModbusReply::finished, info, [info, reply, sgReadyModeString] {
            if (reply->error() != QModbusDevice::NoError) {
                qCWarning(dcStiebelEltron())
                    << "Set SG ready mode finished with error" << reply->errorString();
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }

            qCDebug(dcStiebelEltron()) << "Execute action finished successfully"
                                       << info->action().actionTypeId().toString() << info->action().params();
            info->thing()->setStateValue(stiebelEltronSgReadyModeStateTypeId, sgReadyModeString);
            info->finish(Thing::ThingErrorNoError);
        });

        connect(reply, &QModbusReply::errorOccurred, this, [reply](QModbusDevice::Error error) {
            qCWarning(dcStiebelEltron())
                << "Modbus reply error occurred while execute action" << error << reply->errorString();
            emit reply->finished();  // To make sure it will be deleted
        });
    }
    info->finish(Thing::ThingErrorNoError);
}

