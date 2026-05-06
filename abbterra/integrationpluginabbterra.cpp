#include "integrationpluginabbterra.h"
#include "abbterrartudiscovery.h"
#include "abbterratcpdiscovery.h"
#include "abbterrautils.h"

#include "integrations/thing.h"
#include "integrations/thingactioninfo.h"
#include "integrations/thingdescriptor.h"
#include "integrations/thingdiscoveryinfo.h"
#include "integrations/thingsetupinfo.h"
#include "plugininfo.h"

#include <QLoggingCategory>
#include <QtMath>
#include <utility>

#include <hardwaremanager.h>
#include <hardware/modbus/modbusrtuhardwareresource.h>
#include <network/networkdevicediscovery.h>

IntegrationPluginAbbterra::IntegrationPluginAbbterra()
{
}

void IntegrationPluginAbbterra::discoverThings(ThingDiscoveryInfo *info)
{
    if (info->thingClassId() == terraAcTcpThingClassId) {
        AbbTerraTcpDiscovery *discovery = new AbbTerraTcpDiscovery(hardwareManager()->networkDeviceDiscovery(), info);
        connect(discovery, &AbbTerraTcpDiscovery::discoveryFinished, info, [this, info, discovery]() {
            foreach (const AbbTerraTcpDiscovery::Result &result, discovery->results()) {
                ThingDescriptor descriptor(terraAcTcpThingClassId, result.productName, result.serialNumber);
                ParamList params;
                params.append(Param(terraAcTcpThingMacAddressParamTypeId, result.networkDeviceInfo.thingParamValueMacAddress()));
                params.append(Param(terraAcTcpThingHostNameParamTypeId, result.networkDeviceInfo.thingParamValueHostName()));
                params.append(Param(terraAcTcpThingAddressParamTypeId, result.networkDeviceInfo.thingParamValueAddress()));
                params.append(Param(terraAcTcpThingPortParamTypeId, 502));
                params.append(Param(terraAcTcpThingSlaveIdParamTypeId, 1));
                descriptor.setParams(params);

                if (Thing *existingThing = myThings().findByParams(params)) {
                    descriptor.setThingId(existingThing->id());
                }

                info->addThingDescriptor(descriptor);
            }

            info->finish(Thing::ThingErrorNoError);
        });
        discovery->startDiscovery();
        return;
    }

    if (info->thingClassId() == terraAcRtuThingClassId) {
        AbbTerraRtuDiscovery *discovery = new AbbTerraRtuDiscovery(hardwareManager()->modbusRtuResource(), info);
        connect(discovery, &AbbTerraRtuDiscovery::discoveryFinished, info, [this, info, discovery](bool modbusRtuMasterAvailable) {
            if (!modbusRtuMasterAvailable) {
                info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("No connected Modbus RTU master available."));
                return;
            }

            foreach (const AbbTerraRtuDiscovery::Result &result, discovery->results()) {
                ThingDescriptor descriptor(terraAcRtuThingClassId, result.productName, QStringLiteral("Slave ID: %1").arg(result.slaveId));
                ParamList params{
                    Param(terraAcRtuThingRtuMasterParamTypeId, result.modbusRtuMasterId),
                    Param(terraAcRtuThingSlaveIdParamTypeId, result.slaveId)
                };
                descriptor.setParams(params);

                if (Thing *existingThing = myThings().findByParams(params)) {
                    descriptor.setThingId(existingThing->id());
                }

                info->addThingDescriptor(descriptor);
            }

            info->finish(Thing::ThingErrorNoError);
        });
        discovery->startDiscovery();
        return;
    }

    info->finish(Thing::ThingErrorNoError);
}

void IntegrationPluginAbbterra::setupThing(ThingSetupInfo *info)
{
    if (info->thing()->thingClassId() == terraAcTcpThingClassId) {
        setupTcpThing(info);
        return;
    }

    if (info->thing()->thingClassId() == terraAcRtuThingClassId) {
        setupRtuThing(info);
        return;
    }

    info->finish(Thing::ThingErrorUnsupportedFeature);
}

void IntegrationPluginAbbterra::postSetupThing(Thing *thing)
{
    Q_UNUSED(thing)

    if (m_pluginTimer) {
        return;
    }

    m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(30);
    connect(m_pluginTimer, &PluginTimer::timeout, this, [this]() {
        foreach (AbbTerraModbusTcpConnection *connection, m_tcpConnections) {
            connection->update();
        }
        foreach (AbbTerraModbusRtuConnection *connection, m_rtuConnections) {
            connection->update();
        }
    });
    m_pluginTimer->start();
}

void IntegrationPluginAbbterra::thingRemoved(Thing *thing)
{
    delete m_tcpConnections.take(thing);
    delete m_rtuConnections.take(thing);
    if (m_monitors.contains(thing)) {
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
    }

    if (myThings().isEmpty() && m_pluginTimer) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginAbbterra::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();

    if (thing->thingClassId() == terraAcTcpThingClassId) {
        AbbTerraModbusTcpConnection *connection = m_tcpConnections.value(thing);
        if (!connection || !connection->reachable()) {
            info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The charging station is not reachable."));
            return;
        }

        if (info->action().actionTypeId() == terraAcTcpPowerActionTypeId) {
            const bool power = info->action().paramValue(terraAcTcpPowerActionPowerParamTypeId).toBool();
            const quint32 currentMilliAmps = power ? static_cast<quint32>(qRound(thing->stateValue(terraAcTcpMaxChargingCurrentStateTypeId).toDouble() * 1000.0)) : 0;
            QModbusReply *reply = connection->setChargingCurrentLimitCommand(currentMilliAmps);
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, info, [info, thing, connection, reply, power]() {
                if (reply->error() == QModbusDevice::NoError) {
                    thing->setStateValue(terraAcTcpPowerStateTypeId, power);
                    connection->update();
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
            return;
        }

        if (info->action().actionTypeId() == terraAcTcpMaxChargingCurrentActionTypeId) {
            const double current = info->action().paramValue(terraAcTcpMaxChargingCurrentActionMaxChargingCurrentParamTypeId).toDouble();
            QModbusReply *reply = connection->setChargingCurrentLimitCommand(static_cast<quint32>(qRound(current * 1000.0)));
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, info, [info, thing, connection, reply, current]() {
                if (reply->error() == QModbusDevice::NoError) {
                    thing->setStateValue(terraAcTcpMaxChargingCurrentStateTypeId, current);
                    thing->setStateValue(terraAcTcpPowerStateTypeId, current >= 6.0);
                    connection->update();
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
            return;
        }

        info->finish(Thing::ThingErrorUnsupportedFeature);
        return;
    }

    if (thing->thingClassId() == terraAcRtuThingClassId) {
        AbbTerraModbusRtuConnection *connection = m_rtuConnections.value(thing);
        if (!connection || !connection->reachable()) {
            info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The charging station is not reachable."));
            return;
        }

        if (info->action().actionTypeId() == terraAcRtuPowerActionTypeId) {
            const bool power = info->action().paramValue(terraAcRtuPowerActionPowerParamTypeId).toBool();
            const quint32 currentMilliAmps = power ? static_cast<quint32>(qRound(thing->stateValue(terraAcRtuMaxChargingCurrentStateTypeId).toDouble() * 1000.0)) : 0;
            ModbusRtuReply *reply = connection->setChargingCurrentLimitCommand(currentMilliAmps);
            connect(reply, &ModbusRtuReply::finished, info, [info, thing, connection, reply, power]() {
                if (reply->error() == ModbusRtuReply::NoError) {
                    thing->setStateValue(terraAcRtuPowerStateTypeId, power);
                    connection->update();
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
            return;
        }

        if (info->action().actionTypeId() == terraAcRtuMaxChargingCurrentActionTypeId) {
            const double current = info->action().paramValue(terraAcRtuMaxChargingCurrentActionMaxChargingCurrentParamTypeId).toDouble();
            ModbusRtuReply *reply = connection->setChargingCurrentLimitCommand(static_cast<quint32>(qRound(current * 1000.0)));
            connect(reply, &ModbusRtuReply::finished, info, [info, thing, connection, reply, current]() {
                if (reply->error() == ModbusRtuReply::NoError) {
                    thing->setStateValue(terraAcRtuMaxChargingCurrentStateTypeId, current);
                    thing->setStateValue(terraAcRtuPowerStateTypeId, current >= 6.0);
                    connection->update();
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
            return;
        }
    }

    info->finish(Thing::ThingErrorUnsupportedFeature);
}

void IntegrationPluginAbbterra::setupTcpThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    if (m_tcpConnections.contains(thing)) {
        m_tcpConnections.take(thing)->deleteLater();
    }

    NetworkDeviceMonitor *monitor = m_monitors.value(thing);
    if (!monitor) {
        monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(thing);
        if (!monitor) {
            info->finish(Thing::ThingErrorInvalidParameter);
            return;
        }
        m_monitors.insert(thing, monitor);
    }

    const quint16 port = static_cast<quint16>(thing->paramValue(terraAcTcpThingPortParamTypeId).toUInt());
    const quint16 slaveId = static_cast<quint16>(thing->paramValue(terraAcTcpThingSlaveIdParamTypeId).toUInt());
    AbbTerraModbusTcpConnection *connection = new AbbTerraModbusTcpConnection(monitor->networkDeviceInfo().address(), port, slaveId, thing);

    connect(info, &ThingSetupInfo::aborted, connection, &AbbTerraModbusTcpConnection::deleteLater);
    connect(info, &ThingSetupInfo::aborted, monitor, [this, thing]() {
        if (m_monitors.contains(thing)) {
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
        }
    });

    connect(monitor, &NetworkDeviceMonitor::networkDeviceInfoChanged, connection, [connection](const NetworkDeviceInfo &networkDeviceInfo) {
        connection->modbusTcpMaster()->setHostAddress(networkDeviceInfo.address());
    });

    connect(connection, &AbbTerraModbusTcpConnection::reachableChanged, thing, [this, thing, connection](bool reachable) {
        if (reachable) {
            connection->initialize();
        } else {
            setDisconnectedState(thing);
        }
    });

    connect(connection, &AbbTerraModbusTcpConnection::initializationFinished, thing, [this, thing, connection](bool success) {
        if (!success) {
            return;
        }

        const AbbTerraUtils::DeviceInfo deviceInfo = AbbTerraUtils::deviceInfoFromValues(connection->serialNumber(),
                                                                                         connection->firmwareVersionRaw(),
                                                                                         connection->userSettableMaxCurrent());
        if (!deviceInfo.valid) {
            return;
        }

        thing->setStateValue(terraAcTcpConnectedStateTypeId, true);
        thing->setStateValue(terraAcTcpFirmwareVersionStateTypeId, deviceInfo.firmwareVersion);
        thing->setStateValue(terraAcTcpSerialNumberStateTypeId, deviceInfo.serialNumber);
        thing->setStateMinMaxValues(terraAcTcpMaxChargingCurrentStateTypeId, 6.0, deviceInfo.maxChargingCurrent);
        applyTimeoutSetting(thing, connection);
    });

    connect(connection, &AbbTerraModbusTcpConnection::initializationFinished, info, [this, info, thing, connection](bool success) {
        if (!success) {
            connection->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("Could not initialize the communication with the charger."));
            return;
        }

        const AbbTerraUtils::DeviceInfo deviceInfo = AbbTerraUtils::deviceInfoFromValues(connection->serialNumber(),
                                                                                         connection->firmwareVersionRaw(),
                                                                                         connection->userSettableMaxCurrent());
        if (!deviceInfo.valid) {
            connection->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("The device does not match the ABB Terra AC Modbus register map."));
            return;
        }

        m_tcpConnections.insert(thing, connection);
        connection->update();
        info->finish(Thing::ThingErrorNoError);
    });

    connect(connection, &AbbTerraModbusTcpConnection::updateFinished, thing, [this, thing, connection]() {
        updateThing(thing, connection);
    });

    connect(thing, &Thing::settingChanged, connection, [this, thing, connection](const ParamTypeId &paramTypeId, const QVariant &) {
        if (paramTypeId == terraAcTcpSettingsCommunicationTimeoutParamTypeId) {
            applyTimeoutSetting(thing, connection);
        }
    });
}

void IntegrationPluginAbbterra::setupRtuThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    if (m_rtuConnections.contains(thing)) {
        m_rtuConnections.take(thing)->deleteLater();
    }

    ModbusRtuMaster *master = hardwareManager()->modbusRtuResource()->getModbusRtuMaster(thing->paramValue(terraAcRtuThingRtuMasterParamTypeId).toUuid());
    if (!master) {
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The Modbus RTU connection is not available."));
        return;
    }

    const quint16 slaveId = static_cast<quint16>(thing->paramValue(terraAcRtuThingSlaveIdParamTypeId).toUInt());
    AbbTerraModbusRtuConnection *connection = new AbbTerraModbusRtuConnection(master, slaveId, thing);

    connect(connection, &AbbTerraModbusRtuConnection::reachableChanged, thing, [this, thing, connection](bool reachable) {
        if (reachable) {
            connection->initialize();
        } else {
            setDisconnectedState(thing);
        }
    });

    connect(connection, &AbbTerraModbusRtuConnection::initializationFinished, thing, [this, thing, connection](bool success) {
        if (!success) {
            return;
        }

        const AbbTerraUtils::DeviceInfo deviceInfo = AbbTerraUtils::deviceInfoFromValues(connection->serialNumber(),
                                                                                         connection->firmwareVersionRaw(),
                                                                                         connection->userSettableMaxCurrent());
        if (!deviceInfo.valid) {
            return;
        }

        thing->setStateValue(terraAcRtuConnectedStateTypeId, true);
        thing->setStateValue(terraAcRtuFirmwareVersionStateTypeId, deviceInfo.firmwareVersion);
        thing->setStateValue(terraAcRtuSerialNumberStateTypeId, deviceInfo.serialNumber);
        thing->setStateMinMaxValues(terraAcRtuMaxChargingCurrentStateTypeId, 6.0, deviceInfo.maxChargingCurrent);
        applyTimeoutSetting(thing, connection);
    });

    connect(connection, &AbbTerraModbusRtuConnection::initializationFinished, info, [this, info, thing, connection](bool success) {
        if (!success) {
            connection->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("Could not initialize the communication with the charger."));
            return;
        }

        const AbbTerraUtils::DeviceInfo deviceInfo = AbbTerraUtils::deviceInfoFromValues(connection->serialNumber(),
                                                                                         connection->firmwareVersionRaw(),
                                                                                         connection->userSettableMaxCurrent());
        if (!deviceInfo.valid) {
            connection->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("The device does not match the ABB Terra AC Modbus register map."));
            return;
        }

        m_rtuConnections.insert(thing, connection);
        connection->update();
        info->finish(Thing::ThingErrorNoError);
    });

    connect(connection, &AbbTerraModbusRtuConnection::updateFinished, thing, [this, thing, connection]() {
        updateThing(thing, connection);
    });

    connect(thing, &Thing::settingChanged, connection, [this, thing, connection](const ParamTypeId &paramTypeId, const QVariant &) {
        if (paramTypeId == terraAcRtuSettingsCommunicationTimeoutParamTypeId) {
            applyTimeoutSetting(thing, connection);
        }
    });
}

void IntegrationPluginAbbterra::applyTimeoutSetting(Thing *thing, AbbTerraModbusTcpConnection *connection)
{
    QModbusReply *reply = connection->setCommunicationTimeoutCommand(static_cast<quint16>(thing->setting(terraAcTcpSettingsCommunicationTimeoutParamTypeId).toUInt()));
    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
    connect(reply, &QModbusReply::finished, connection, [connection, reply]() {
        if (reply->error() != QModbusDevice::NoError && connection->reachable()) {
            connection->updateCommunicationTimeoutReadback();
        }
    });
}

void IntegrationPluginAbbterra::applyTimeoutSetting(Thing *thing, AbbTerraModbusRtuConnection *connection)
{
    ModbusRtuReply *reply = connection->setCommunicationTimeoutCommand(static_cast<quint16>(thing->setting(terraAcRtuSettingsCommunicationTimeoutParamTypeId).toUInt()));
    connect(reply, &ModbusRtuReply::finished, connection, [connection, reply]() {
        if (reply->error() != ModbusRtuReply::NoError && connection->reachable()) {
            connection->updateCommunicationTimeoutReadback();
        }
    });
}

void IntegrationPluginAbbterra::updateThing(Thing *thing, AbbTerraModbusTcpConnection *connection)
{
    thing->setStateValue(terraAcTcpConnectedStateTypeId, connection->reachable());
    thing->setStateValue(terraAcTcpPluggedInStateTypeId, AbbTerraUtils::isVehiclePluggedIn(connection->chargingStateRaw(), connection->socketLockState()));
    thing->setStateValue(terraAcTcpChargingStateTypeId, AbbTerraUtils::isCharging(connection->chargingStateRaw(), connection->activePower()));
    thing->setStateValue(terraAcTcpPowerStateTypeId, connection->chargingCurrentLimit() >= 6000);
    thing->setStateValue(terraAcTcpMaxChargingCurrentStateTypeId, connection->chargingCurrentLimit() / 1000.0);
    thing->setStateValue(terraAcTcpPhaseCountStateTypeId, AbbTerraUtils::phaseCount(connection->voltageL1(), connection->voltageL2(), connection->voltageL3()));
    thing->setStateValue(terraAcTcpCurrentPowerStateTypeId, static_cast<double>(connection->activePower()));
    thing->setStateValue("currentPhase1", connection->currentL1() / 1000.0);
    thing->setStateValue("currentPhase2", connection->currentL2() / 1000.0);
    thing->setStateValue("currentPhase3", connection->currentL3() / 1000.0);
    thing->setStateValue("voltagePhase1", connection->voltageL1() / 10.0);
    thing->setStateValue("voltagePhase2", connection->voltageL2() / 10.0);
    thing->setStateValue("voltagePhase3", connection->voltageL3() / 10.0);
    thing->setStateValue(terraAcTcpSessionEnergyStateTypeId, connection->sessionEnergy() / 1000.0);
    thing->setStateValue(terraAcTcpErrorCodeStateTypeId, connection->errorCode());
    thing->setSettingValue(terraAcTcpSettingsCommunicationTimeoutParamTypeId, connection->communicationTimeoutReadback());
}

void IntegrationPluginAbbterra::updateThing(Thing *thing, AbbTerraModbusRtuConnection *connection)
{
    thing->setStateValue(terraAcRtuConnectedStateTypeId, connection->reachable());
    thing->setStateValue(terraAcRtuPluggedInStateTypeId, AbbTerraUtils::isVehiclePluggedIn(connection->chargingStateRaw(), connection->socketLockState()));
    thing->setStateValue(terraAcRtuChargingStateTypeId, AbbTerraUtils::isCharging(connection->chargingStateRaw(), connection->activePower()));
    thing->setStateValue(terraAcRtuPowerStateTypeId, connection->chargingCurrentLimit() >= 6000);
    thing->setStateValue(terraAcRtuMaxChargingCurrentStateTypeId, connection->chargingCurrentLimit() / 1000.0);
    thing->setStateValue(terraAcRtuPhaseCountStateTypeId, AbbTerraUtils::phaseCount(connection->voltageL1(), connection->voltageL2(), connection->voltageL3()));
    thing->setStateValue(terraAcRtuCurrentPowerStateTypeId, static_cast<double>(connection->activePower()));
    thing->setStateValue("currentPhase1", connection->currentL1() / 1000.0);
    thing->setStateValue("currentPhase2", connection->currentL2() / 1000.0);
    thing->setStateValue("currentPhase3", connection->currentL3() / 1000.0);
    thing->setStateValue("voltagePhase1", connection->voltageL1() / 10.0);
    thing->setStateValue("voltagePhase2", connection->voltageL2() / 10.0);
    thing->setStateValue("voltagePhase3", connection->voltageL3() / 10.0);
    thing->setStateValue(terraAcRtuSessionEnergyStateTypeId, connection->sessionEnergy() / 1000.0);
    thing->setStateValue(terraAcRtuErrorCodeStateTypeId, connection->errorCode());
    thing->setSettingValue(terraAcRtuSettingsCommunicationTimeoutParamTypeId, connection->communicationTimeoutReadback());
}

void IntegrationPluginAbbterra::setDisconnectedState(Thing *thing)
{
    if (thing->thingClassId() == terraAcTcpThingClassId) {
        thing->setStateValue(terraAcTcpConnectedStateTypeId, false);
        thing->setStateValue(terraAcTcpChargingStateTypeId, false);
        thing->setStateValue(terraAcTcpPluggedInStateTypeId, false);
        thing->setStateValue(terraAcTcpCurrentPowerStateTypeId, 0);
        thing->setStateValue("currentPhase1", 0);
        thing->setStateValue("currentPhase2", 0);
        thing->setStateValue("currentPhase3", 0);
        thing->setStateValue("voltagePhase1", 0);
        thing->setStateValue("voltagePhase2", 0);
        thing->setStateValue("voltagePhase3", 0);
        return;
    }

    if (thing->thingClassId() == terraAcRtuThingClassId) {
        thing->setStateValue(terraAcRtuConnectedStateTypeId, false);
        thing->setStateValue(terraAcRtuChargingStateTypeId, false);
        thing->setStateValue(terraAcRtuPluggedInStateTypeId, false);
        thing->setStateValue(terraAcRtuCurrentPowerStateTypeId, 0);
        thing->setStateValue("currentPhase1", 0);
        thing->setStateValue("currentPhase2", 0);
        thing->setStateValue("currentPhase3", 0);
        thing->setStateValue("voltagePhase1", 0);
        thing->setStateValue("voltagePhase2", 0);
        thing->setStateValue("voltagePhase3", 0);
    }
}
