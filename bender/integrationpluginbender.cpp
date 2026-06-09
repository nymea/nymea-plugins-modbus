// SPDX-License-Identifier: GPL-3.0-or-later

#include "integrationpluginbender.h"
#include "omccidiscovery.h"
#include "plugininfo.h"
#include "tqdm100discovery.h"

#include <hardwaremanager.h>
#include <network/networkdevicediscovery.h>

#include <QModbusReply>

namespace {
static const quint32 InvalidUnsignedValue = 0xffffffff;

static bool isValidMeterValue(quint32 value)
{
    return value != InvalidUnsignedValue;
}
}

IntegrationPluginBender::IntegrationPluginBender()
{
}

void IntegrationPluginBender::discoverThings(ThingDiscoveryInfo *info)
{
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcBender()) << "The network discovery is not available on this platform.";
        info->finish(Thing::ThingErrorUnsupportedFeature, QT_TR_NOOP("The network device discovery is not available."));
        return;
    }

    if (info->thingClassId() == omcciThingClassId) {
        OmcciDiscovery *discovery = new OmcciDiscovery(hardwareManager()->networkDeviceDiscovery(), info);
        connect(discovery, &OmcciDiscovery::discoveryFinished, info, [=]() {
            foreach (const OmcciDiscovery::Result &result, discovery->discoveryResults()) {
                QString title = result.serialNumber.isEmpty() ? "Bender OMCCI" : "Bender " + result.serialNumber;
                QStringList descriptionParts;
                if (!result.firmwareVersion.isEmpty())
                    descriptionParts << "Version: " + result.firmwareVersion;
                if (!result.serialNumber.isEmpty())
                    descriptionParts << "Serial: " + result.serialNumber;
                descriptionParts << result.networkDeviceInfo.address().toString();

                ThingDescriptor descriptor(omcciThingClassId, title, descriptionParts.join(" - "));
                ParamList params;
                params << Param(omcciThingMacAddressParamTypeId, result.networkDeviceInfo.thingParamValueMacAddress());
                params << Param(omcciThingHostNameParamTypeId, result.networkDeviceInfo.thingParamValueHostName());
                params << Param(omcciThingAddressParamTypeId, result.networkDeviceInfo.thingParamValueAddress());
                descriptor.setParams(params);

                Thing *existingThing = myThings().findByParams(params);
                if (existingThing)
                    descriptor.setThingId(existingThing->id());

                info->addThingDescriptor(descriptor);
            }

            info->finish(Thing::ThingErrorNoError);
        });

        discovery->startDiscovery();
        return;
    }

    if (info->thingClassId() == tqdm100ThingClassId) {
        TqDm100Discovery *discovery = new TqDm100Discovery(hardwareManager()->networkDeviceDiscovery(), info);
        connect(discovery, &TqDm100Discovery::discoveryFinished, info, [=]() {
            foreach (const TqDm100Discovery::Result &result, discovery->discoveryResults()) {
                ThingDescriptor descriptor(tqdm100ThingClassId, "Bender TQ-DM100", result.networkDeviceInfo.address().toString());
                ParamList params;
                params << Param(tqdm100ThingMacAddressParamTypeId, result.networkDeviceInfo.thingParamValueMacAddress());
                params << Param(tqdm100ThingHostNameParamTypeId, result.networkDeviceInfo.thingParamValueHostName());
                params << Param(tqdm100ThingAddressParamTypeId, result.networkDeviceInfo.thingParamValueAddress());
                descriptor.setParams(params);

                Thing *existingThing = myThings().findByParams(params);
                if (existingThing)
                    descriptor.setThingId(existingThing->id());

                info->addThingDescriptor(descriptor);
            }

            info->finish(Thing::ThingErrorNoError);
        });

        discovery->startDiscovery();
        return;
    }
}

void IntegrationPluginBender::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcBender()) << "Setting up" << thing << thing->params();

    if (thing->thingClassId() != tqdm100ThingClassId && thing->thingClassId() != omcciThingClassId)
        return;

    if (m_monitors.contains(thing))
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));

    if (thing->thingClassId() == tqdm100ThingClassId && m_tqDm100Connections.contains(thing))
        m_tqDm100Connections.take(thing)->deleteLater();

    if (thing->thingClassId() == omcciThingClassId && m_omcciConnections.contains(thing))
        m_omcciConnections.take(thing)->deleteLater();

    NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(thing);
    if (!monitor) {
        qCWarning(dcBender()) << "Unable to register monitor with the given params" << thing->params();
        info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("Unable to set up the connection with this configuration, please reconfigure the connection."));
        return;
    }

    m_monitors.insert(thing, monitor);
    connect(info, &ThingSetupInfo::aborted, monitor, [this, thing]() {
        if (m_monitors.contains(thing))
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
        if (m_tqDm100Connections.contains(thing))
            m_tqDm100Connections.take(thing)->deleteLater();
        if (m_omcciConnections.contains(thing))
            m_omcciConnections.take(thing)->deleteLater();
    });

    auto setupConnection = [this, info, thing]() {
        if (thing->thingClassId() == tqdm100ThingClassId) {
            setupTqDm100Connection(info);
        } else if (thing->thingClassId() == omcciThingClassId) {
            setupOmcciConnection(info);
        }
    };

    if (info->isInitialSetup() && !monitor->reachable()) {
        qCDebug(dcBender()) << "Waiting for network monitor before continuing setup of" << thing->name();
        connect(monitor, &NetworkDeviceMonitor::reachableChanged, info, [setupConnection](bool reachable) {
            if (reachable)
                setupConnection();
        });
    } else {
        setupConnection();
    }
}

void IntegrationPluginBender::postSetupThing(Thing *thing)
{
    Q_UNUSED(thing)

    if (m_pluginTimer)
        return;

    qCDebug(dcBender()) << "Starting plugin timer...";
    m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(2);
    connect(m_pluginTimer, &PluginTimer::timeout, this, [this]() {
        foreach (BenderTqDm100ModbusTcpConnection *connection, m_tqDm100Connections) {
            connection->update();
            Thing *thing = m_tqDm100Connections.key(connection);
            quint16 lifeBit = m_lifeBits.value(thing, 0) ? 0 : 1;
            m_lifeBits.insert(thing, lifeBit);
            connection->setLifeBit(lifeBit);
        }

        foreach (BenderOmcciModbusTcpConnection *connection, m_omcciConnections)
            connection->update();
    });
    m_pluginTimer->start();
}

void IntegrationPluginBender::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();

    if (thing->thingClassId() == tqdm100ThingClassId) {
        BenderTqDm100ModbusTcpConnection *connection = m_tqDm100Connections.value(thing);
        if (!connection || !connection->reachable()) {
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        if (info->action().actionTypeId() == tqdm100PowerActionTypeId) {
            bool power = info->action().paramValue(tqdm100PowerActionPowerParamTypeId).toBool();
            quint16 current = power ? qRound(thing->stateValue(tqdm100MaxChargingCurrentStateTypeId).toDouble()) : 0;
            QModbusReply *reply = connection->setChargeCurrent(current);
            connect(reply, &QModbusReply::finished, info, [=]() {
                if (reply->error() == QModbusDevice::NoError) {
                    thing->setStateValue(tqdm100PowerStateTypeId, power);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    qCWarning(dcBender()) << "Error setting TQ-DM100 power:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
                reply->deleteLater();
            });
            return;
        }

        if (info->action().actionTypeId() == tqdm100MaxChargingCurrentActionTypeId) {
            quint16 current = qRound(info->action().paramValue(tqdm100MaxChargingCurrentActionMaxChargingCurrentParamTypeId).toDouble());
            if (!thing->stateValue(tqdm100PowerStateTypeId).toBool()) {
                thing->setStateValue(tqdm100MaxChargingCurrentStateTypeId, current);
                info->finish(Thing::ThingErrorNoError);
                return;
            }

            QModbusReply *reply = connection->setChargeCurrent(current);
            connect(reply, &QModbusReply::finished, info, [=]() {
                if (reply->error() == QModbusDevice::NoError) {
                    thing->setStateValue(tqdm100MaxChargingCurrentStateTypeId, current);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    qCWarning(dcBender()) << "Error setting TQ-DM100 current:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
                reply->deleteLater();
            });
            return;
        }
    }

    if (thing->thingClassId() == omcciThingClassId) {
        BenderOmcciModbusTcpConnection *connection = m_omcciConnections.value(thing);
        if (!connection || !connection->reachable()) {
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        if (info->action().actionTypeId() == omcciPowerActionTypeId) {
            bool power = info->action().paramValue(omcciPowerActionPowerParamTypeId).toBool();
            QModbusReply *reply = connection->setChargePointAvailability(power ? BenderOmcciModbusTcpConnection::ChargePointAvailabilityOperative :
                                                                                 BenderOmcciModbusTcpConnection::ChargePointAvailabilityInoperative);
            connect(reply, &QModbusReply::finished, info, [=]() {
                if (reply->error() == QModbusDevice::NoError) {
                    thing->setStateValue(omcciPowerStateTypeId, power);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    qCWarning(dcBender()) << "Error setting OMCCI availability:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
                reply->deleteLater();
            });
            return;
        }

        if (info->action().actionTypeId() == omcciMaxChargingCurrentActionTypeId) {
            quint16 current = qRound(info->action().paramValue(omcciMaxChargingCurrentActionMaxChargingCurrentParamTypeId).toDouble());
            QModbusReply *reply = connection->setHemsCurrentLimit(current);
            connect(reply, &QModbusReply::finished, info, [=]() {
                if (reply->error() == QModbusDevice::NoError) {
                    thing->setStateValue(omcciMaxChargingCurrentStateTypeId, current);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    qCWarning(dcBender()) << "Error setting OMCCI HEMS current limit:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
                reply->deleteLater();
            });
            return;
        }
    }
}

void IntegrationPluginBender::thingRemoved(Thing *thing)
{
    if (m_tqDm100Connections.contains(thing)) {
        BenderTqDm100ModbusTcpConnection *connection = m_tqDm100Connections.take(thing);
        connection->disconnectDevice();
        connection->deleteLater();
    }

    if (m_omcciConnections.contains(thing)) {
        BenderOmcciModbusTcpConnection *connection = m_omcciConnections.take(thing);
        connection->disconnectDevice();
        connection->deleteLater();
    }

    m_lifeBits.remove(thing);

    if (m_monitors.contains(thing))
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));

    if (myThings().isEmpty() && m_pluginTimer) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginBender::setupTqDm100Connection(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    NetworkDeviceMonitor *monitor = m_monitors.value(thing);
    QHostAddress address = monitor->networkDeviceInfo().address();

    qCDebug(dcBender()) << "Setting up TQ-DM100 on" << address.toString();
    BenderTqDm100ModbusTcpConnection *connection = new BenderTqDm100ModbusTcpConnection(address, 502, 0xff, this);
    connect(info, &ThingSetupInfo::aborted, connection, &BenderTqDm100ModbusTcpConnection::deleteLater);

    connect(monitor, &NetworkDeviceMonitor::reachableChanged, thing, [=](bool reachable) {
        qCDebug(dcBender()) << "TQ-DM100 monitor reachable changed for" << thing->name() << reachable;
        if (!thing->setupComplete())
            return;

        if (reachable && !thing->stateValue(tqdm100ConnectedStateTypeId).toBool()) {
            connection->modbusTcpMaster()->setHostAddress(monitor->networkDeviceInfo().address());
            connection->connectDevice();
        } else if (!reachable) {
            connection->disconnectDevice();
            thing->setStateValue(tqdm100ConnectedStateTypeId, false);
            clearLivePowerStates(thing);
        }
    });

    connect(connection, &BenderTqDm100ModbusTcpConnection::reachableChanged, thing, [=](bool reachable) {
        if (reachable) {
            connection->initialize();
        } else {
            thing->setStateValue(tqdm100ConnectedStateTypeId, false);
            clearLivePowerStates(thing);
        }
    });

    connect(connection, &BenderTqDm100ModbusTcpConnection::initializationFinished, info, [=](bool success) {
        if (!success) {
            qCWarning(dcBender()) << "TQ-DM100 connection init failed" << thing->name() << address.toString();
            if (m_monitors.contains(thing))
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            connection->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("Error communicating with the wallbox."));
            return;
        }

        m_tqDm100Connections.insert(thing, connection);
        thing->setStateValue(tqdm100ConnectedStateTypeId, true);
        thing->setStateValue(tqdm100PhaseCountStateTypeId, 3);
        info->finish(Thing::ThingErrorNoError);
        connection->update();
    });

    connect(connection, &BenderTqDm100ModbusTcpConnection::initializationFinished, thing, [=](bool success) {
        if (!thing->setupComplete())
            return;

        thing->setStateValue(tqdm100ConnectedStateTypeId, success);
        if (!success)
            clearLivePowerStates(thing);
    });

    connect(connection, &BenderTqDm100ModbusTcpConnection::chargeStateChanged, thing, [thing](BenderTqDm100ModbusTcpConnection::ChargeState chargeState) {
        const bool charging = chargeState == BenderTqDm100ModbusTcpConnection::ChargeStateCharging;
        thing->setStateValue(tqdm100ChargingStateTypeId, charging);
    });

    connect(connection, &BenderTqDm100ModbusTcpConnection::cableStateChanged, thing, [thing](quint16 cableState) {
        thing->setStateValue(tqdm100PluggedInStateTypeId, cableState != 0);
    });

    connect(connection, &BenderTqDm100ModbusTcpConnection::totalPowerChanged, thing, [thing](quint32 totalPower) {
        thing->setStateValue(tqdm100CurrentPowerStateTypeId, isValidMeterValue(totalPower) ? totalPower : 0);
    });

    connect(connection, &BenderTqDm100ModbusTcpConnection::totalEnergyChanged, thing, [thing](quint32 totalEnergy) {
        if (isValidMeterValue(totalEnergy))
            thing->setStateValue(tqdm100TotalEnergyConsumedStateTypeId, totalEnergy / 1000.0);
    });

    connect(connection, &BenderTqDm100ModbusTcpConnection::sessionEnergyChanged, thing, [thing](quint16 sessionEnergy) {
        thing->setStateValue(tqdm100SessionEnergyStateTypeId, sessionEnergy / 1000.0);
    });

    connect(connection, &BenderTqDm100ModbusTcpConnection::chargeCurrentChanged, thing, [thing](quint16 chargeCurrent) {
        thing->setStateValue(tqdm100PowerStateTypeId, chargeCurrent > 0);
        if (chargeCurrent > 0)
            thing->setStateValue(tqdm100MaxChargingCurrentStateTypeId, chargeCurrent);
    });

    connect(connection, &BenderTqDm100ModbusTcpConnection::minimumCurrentLimitChanged, thing, [thing](quint16 minimumCurrentLimit) {
        thing->setStateMinValue(tqdm100MaxChargingCurrentStateTypeId, qMax<quint16>(minimumCurrentLimit, 6));
    });
    connect(connection, &BenderTqDm100ModbusTcpConnection::evseMaxCurrentChanged, thing, [this, thing](quint16) {
        updateTqDm100MaxCurrent(thing);
    });
    connect(connection, &BenderTqDm100ModbusTcpConnection::cableMaxCurrentChanged, thing, [this, thing](quint16) {
        updateTqDm100MaxCurrent(thing);
    });
    connect(connection, &BenderTqDm100ModbusTcpConnection::evMaxCurrentChanged, thing, [this, thing](quint16) {
        updateTqDm100MaxCurrent(thing);
    });

    connection->connectDevice();
}

void IntegrationPluginBender::setupOmcciConnection(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    NetworkDeviceMonitor *monitor = m_monitors.value(thing);
    QHostAddress address = monitor->networkDeviceInfo().address();

    qCDebug(dcBender()) << "Setting up OMCCI on" << address.toString();
    BenderOmcciModbusTcpConnection *connection = new BenderOmcciModbusTcpConnection(address, 502, 0xff, this);
    connect(info, &ThingSetupInfo::aborted, connection, &BenderOmcciModbusTcpConnection::deleteLater);

    connect(monitor, &NetworkDeviceMonitor::reachableChanged, thing, [=](bool reachable) {
        qCDebug(dcBender()) << "OMCCI monitor reachable changed for" << thing->name() << reachable;
        if (!thing->setupComplete())
            return;

        if (reachable && !thing->stateValue(omcciConnectedStateTypeId).toBool()) {
            connection->modbusTcpMaster()->setHostAddress(monitor->networkDeviceInfo().address());
            connection->connectDevice();
        } else if (!reachable) {
            connection->disconnectDevice();
            thing->setStateValue(omcciConnectedStateTypeId, false);
            clearLivePowerStates(thing);
        }
    });

    connect(connection, &BenderOmcciModbusTcpConnection::reachableChanged, thing, [=](bool reachable) {
        if (reachable) {
            connection->initialize();
        } else {
            thing->setStateValue(omcciConnectedStateTypeId, false);
            clearLivePowerStates(thing);
        }
    });

    connect(connection, &BenderOmcciModbusTcpConnection::initializationFinished, info, [=](bool success) {
        if (!success) {
            qCWarning(dcBender()) << "OMCCI connection init failed" << thing->name() << address.toString();
            if (m_monitors.contains(thing))
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            connection->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("Error communicating with the wallbox."));
            return;
        }

        m_omcciConnections.insert(thing, connection);
        thing->setStateValue(omcciConnectedStateTypeId, true);
        thing->setStateValue(omcciPhaseCountStateTypeId, 3);
        thing->setStateValue(omcciSerialNumberStateTypeId, connection->serialNumber().trimmed());
        thing->setStateValue(omcciVersionStateTypeId, connection->firmwareVersion().trimmed());
        thing->setStateValue(omcciPowerStateTypeId, connection->chargePointAvailability() == BenderOmcciModbusTcpConnection::ChargePointAvailabilityOperative);
        info->finish(Thing::ThingErrorNoError);
        connection->update();
    });

    connect(connection, &BenderOmcciModbusTcpConnection::initializationFinished, thing, [=](bool success) {
        if (!thing->setupComplete())
            return;

        thing->setStateValue(omcciConnectedStateTypeId, success);
        if (!success)
            clearLivePowerStates(thing);
    });

    connect(connection, &BenderOmcciModbusTcpConnection::ocppCpStatusChanged, thing, [thing](BenderOmcciModbusTcpConnection::OcppCpStatus status) {
        bool charging = status == BenderOmcciModbusTcpConnection::OcppCpStatusCharging;
        bool pluggedIn = status == BenderOmcciModbusTcpConnection::OcppCpStatusOccupied ||
                         status == BenderOmcciModbusTcpConnection::OcppCpStatusPreparing ||
                         status == BenderOmcciModbusTcpConnection::OcppCpStatusCharging ||
                         status == BenderOmcciModbusTcpConnection::OcppCpStatusSuspendedEvse ||
                         status == BenderOmcciModbusTcpConnection::OcppCpStatusSuspendedEv ||
                         status == BenderOmcciModbusTcpConnection::OcppCpStatusFinishing;

        thing->setStateValue(omcciChargingStateTypeId, charging);
        thing->setStateValue(omcciPluggedInStateTypeId, pluggedIn);
    });

    connect(connection, &BenderOmcciModbusTcpConnection::vehicleStateChanged, thing, [thing](BenderOmcciModbusTcpConnection::VehicleState vehicleState) {
        thing->setStateValue(omcciPluggedInStateTypeId, vehicleState == BenderOmcciModbusTcpConnection::VehicleStateB ||
                                                         vehicleState == BenderOmcciModbusTcpConnection::VehicleStateC ||
                                                         vehicleState == BenderOmcciModbusTcpConnection::VehicleStateD);
    });

    connect(connection, &BenderOmcciModbusTcpConnection::chargePointAvailabilityChanged, thing, [thing](BenderOmcciModbusTcpConnection::ChargePointAvailability availability) {
        thing->setStateValue(omcciPowerStateTypeId, availability == BenderOmcciModbusTcpConnection::ChargePointAvailabilityOperative);
    });

    connect(connection, &BenderOmcciModbusTcpConnection::totalPowerChanged, thing, [thing](quint32 totalPower) {
        thing->setStateValue(omcciCurrentPowerStateTypeId, isValidMeterValue(totalPower) ? totalPower : 0);
    });

    connect(connection, &BenderOmcciModbusTcpConnection::powerPhaseAChanged, thing, [thing](quint32 power) {
        thing->setStateValue(omcciCurrentPowerPhaseAStateTypeId, isValidMeterValue(power) ? power : 0);
    });
    connect(connection, &BenderOmcciModbusTcpConnection::powerPhaseBChanged, thing, [thing](quint32 power) {
        thing->setStateValue(omcciCurrentPowerPhaseBStateTypeId, isValidMeterValue(power) ? power : 0);
    });
    connect(connection, &BenderOmcciModbusTcpConnection::powerPhaseCChanged, thing, [thing](quint32 power) {
        thing->setStateValue(omcciCurrentPowerPhaseCStateTypeId, isValidMeterValue(power) ? power : 0);
    });

    connect(connection, &BenderOmcciModbusTcpConnection::totalEnergyChanged, thing, [thing](quint32 totalEnergy) {
        if (isValidMeterValue(totalEnergy))
            thing->setStateValue(omcciTotalEnergyConsumedStateTypeId, totalEnergy / 1000.0);
    });
    connect(connection, &BenderOmcciModbusTcpConnection::energyPhaseAChanged, thing, [thing](quint32 energy) {
        if (isValidMeterValue(energy))
            thing->setStateValue(omcciEnergyConsumedPhaseAStateTypeId, energy / 1000.0);
    });
    connect(connection, &BenderOmcciModbusTcpConnection::energyPhaseBChanged, thing, [thing](quint32 energy) {
        if (isValidMeterValue(energy))
            thing->setStateValue(omcciEnergyConsumedPhaseBStateTypeId, energy / 1000.0);
    });
    connect(connection, &BenderOmcciModbusTcpConnection::energyPhaseCChanged, thing, [thing](quint32 energy) {
        if (isValidMeterValue(energy))
            thing->setStateValue(omcciEnergyConsumedPhaseCStateTypeId, energy / 1000.0);
    });
    connect(connection, &BenderOmcciModbusTcpConnection::sessionEnergyChanged, thing, [thing](quint32 energy) {
        if (isValidMeterValue(energy))
            thing->setStateValue(omcciSessionEnergyStateTypeId, energy / 1000.0);
    });

    connect(connection, &BenderOmcciModbusTcpConnection::hemsCurrentLimitChanged, thing, [thing](quint16 current) {
        thing->setStateValue(omcciMaxChargingCurrentStateTypeId, current);
    });
    connect(connection, &BenderOmcciModbusTcpConnection::minimumCurrentLimitChanged, thing, [thing](quint16 minimumCurrentLimit) {
        thing->setStateMinValue(omcciMaxChargingCurrentStateTypeId, qMax<quint16>(minimumCurrentLimit, 6));
    });
    connect(connection, &BenderOmcciModbusTcpConnection::evMaxCurrentChanged, thing, [this, thing](quint16) {
        updateOmcciMaxCurrent(thing);
    });

    connection->connectDevice();
}

void IntegrationPluginBender::updateTqDm100MaxCurrent(Thing *thing)
{
    BenderTqDm100ModbusTcpConnection *connection = m_tqDm100Connections.value(thing);
    if (!connection)
        return;

    quint16 maxCurrent = 32;
    if (connection->evseMaxCurrent() > 0)
        maxCurrent = qMin(maxCurrent, connection->evseMaxCurrent());
    if (connection->cableMaxCurrent() > 0)
        maxCurrent = qMin(maxCurrent, connection->cableMaxCurrent());
    if (connection->evMaxCurrent() > 0)
        maxCurrent = qMin(maxCurrent, connection->evMaxCurrent());

    thing->setStateMaxValue(tqdm100MaxChargingCurrentStateTypeId, qMax<quint16>(maxCurrent, 6));
}

void IntegrationPluginBender::updateOmcciMaxCurrent(Thing *thing)
{
    BenderOmcciModbusTcpConnection *connection = m_omcciConnections.value(thing);
    if (!connection)
        return;

    quint16 maxCurrent = connection->evMaxCurrent() > 0 ? connection->evMaxCurrent() : 32;
    thing->setStateMaxValue(omcciMaxChargingCurrentStateTypeId, qMax<quint16>(maxCurrent, 6));
}

void IntegrationPluginBender::clearLivePowerStates(Thing *thing)
{
    if (thing->thingClassId() == tqdm100ThingClassId) {
        thing->setStateValue(tqdm100CurrentPowerStateTypeId, 0);
        return;
    }

    if (thing->thingClassId() == omcciThingClassId) {
        thing->setStateValue(omcciCurrentPowerStateTypeId, 0);
        thing->setStateValue(omcciCurrentPowerPhaseAStateTypeId, 0);
        thing->setStateValue(omcciCurrentPowerPhaseBStateTypeId, 0);
        thing->setStateValue(omcciCurrentPowerPhaseCStateTypeId, 0);
    }
}
