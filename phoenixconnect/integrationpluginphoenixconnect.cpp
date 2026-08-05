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

#include "integrationpluginphoenixconnect.h"
#include "plugininfo.h"

#include "phoenixmodbustcpconnection.h"
#include "eboxprofessionalmodbustcpconnection.h"
#include "eboxprofessionaldiscovery.h"
#include "phoenixdiscovery.h"

#include <network/networkdevicediscovery.h>
#include <types/param.h>
#include <plugintimer.h>

#include <QDebug>
#include <QStringList>
#include <QJsonDocument>
#include <QNetworkInterface>
#include <QSet>

namespace {

QString normalizedModbusString(QString value)
{
    value.remove(QChar('\0'));
    return value.trimmed();
}

int phaseCountFromSetup(quint16 phaseL1, quint16 phaseL2, quint16 phaseL3)
{
    if (phaseL1 < 1 || phaseL1 > 3)
        return 0;

    QSet<quint16> phases = {phaseL1};
    for (quint16 phase : {phaseL2, phaseL3}) {
        if (phase == 0)
            continue;
        if (phase > 3 || phases.contains(phase))
            return 0;
        phases.insert(phase);
    }
    return phases.size();
}

}


IntegrationPluginPhoenixConnect::IntegrationPluginPhoenixConnect()
{

}

void IntegrationPluginPhoenixConnect::discoverThings(ThingDiscoveryInfo *info)
{
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcPhoenixConnect()) << "Failed to discover network devices. The network device discovery is not available.";
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The network cannot be searched."));
        return;
    }

    if (info->thingClassId() == compleoEBoxProfessionalThingClassId) {
        EBoxProfessionalDiscovery *discovery = new EBoxProfessionalDiscovery(hardwareManager()->networkDeviceDiscovery(), info);
        connect(discovery, &EBoxProfessionalDiscovery::discoveryFinished, info, [this, discovery, info]() {
            for (const EBoxProfessionalDiscovery::Result &result : discovery->results()) {
                ThingDescriptor descriptor(compleoEBoxProfessionalThingClassId,
                                           QStringLiteral("Compleo eBOX professional"),
                                           result.serialNumber);

                ParamList params;
                params << Param(compleoEBoxProfessionalThingMacAddressParamTypeId, result.networkDeviceInfo.thingParamValueMacAddress());
                params << Param(compleoEBoxProfessionalThingHostNameParamTypeId, result.networkDeviceInfo.thingParamValueHostName());
                params << Param(compleoEBoxProfessionalThingAddressParamTypeId, result.networkDeviceInfo.thingParamValueAddress());
                params << Param(compleoEBoxProfessionalThingPortParamTypeId, result.port);
                descriptor.setParams(params);

                if (Thing *existingThing = myThings().findByParams(params))
                    descriptor.setThingId(existingThing->id());

                info->addThingDescriptor(descriptor);
            }
            info->finish(Thing::ThingErrorNoError);
        });
        discovery->startDiscovery();
        return;
    }

    PhoenixDiscovery *discovery = new PhoenixDiscovery(hardwareManager()->networkDeviceDiscovery(), info);
    connect(discovery, &PhoenixDiscovery::discoveryFinished, info, [=](){
        foreach (const PhoenixDiscovery::Result &result, discovery->discoveryResults()) {

            QString name = supportedThings().findById(info->thingClassId()).displayName();
            QString description = result.serialNumber;
            ThingDescriptor descriptor(info->thingClassId(), name, description);
            qCDebug(dcPhoenixConnect()) << "Discovered:" << descriptor.title() << descriptor.description();

            ParamTypeId macAddressParamTypeId = supportedThings().findById(info->thingClassId()).paramTypes().findByName("macAddress").id();
            ParamTypeId hostNameParamTypeId = supportedThings().findById(info->thingClassId()).paramTypes().findByName("hostName").id();
            ParamTypeId addressParamTypeId = supportedThings().findById(info->thingClassId()).paramTypes().findByName("address").id();

            ParamList params;
            params << Param(macAddressParamTypeId, result.networkDeviceInfo.thingParamValueMacAddress());
            params << Param(hostNameParamTypeId, result.networkDeviceInfo.thingParamValueHostName());
            params << Param(addressParamTypeId, result.networkDeviceInfo.thingParamValueAddress());
            descriptor.setParams(params);

            // Check if we already have set up this device
            // FIXME: maybe we should save the serialnumber as parameter in order to identify already known devices
            Thing *existingThing = myThings().findByParams(params);
            if (existingThing) {
                qCDebug(dcPhoenixConnect()) << "This wallbox already exists in the system:" << result.networkDeviceInfo;
                descriptor.setThingId(existingThing->id());
            }

            info->addThingDescriptor(descriptor);
        }

        info->finish(Thing::ThingErrorNoError);
    });
    discovery->startDiscovery();
}


void IntegrationPluginPhoenixConnect::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    if (thing->thingClassId() == compleoEBoxProfessionalThingClassId) {
        setupEBoxProfessional(info);
        return;
    }

    if (m_connections.contains(thing)) {
        qCDebug(dcPhoenixConnect()) << "Reconfiguring existing thing" << thing->name();
        m_connections.take(thing)->deleteLater();
    } else {
        qCDebug(dcPhoenixConnect()) << "Setting up a new device:" << thing->params();
    }

    NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(thing);
    if (!monitor) {
        qCWarning(dcPhoenixConnect()) << "Unable to create monitor with the given parameters" << thing->params();
        info->finish(Thing::ThingErrorInvalidParameter);
        return;
    }

    PhoenixModbusTcpConnection *connection = new PhoenixModbusTcpConnection(monitor->networkDeviceInfo().address(), 502, 255, this);
    connect(info, &ThingSetupInfo::aborted, connection, &PhoenixModbusTcpConnection::deleteLater);
    connect(info, &ThingSetupInfo::aborted, monitor, [monitor, this](){ hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(monitor);});

    connect(thing, &Thing::settingChanged, this, [thing](const ParamTypeId &paramTypeId, const QVariant &value){
        if (paramTypeId == thing->thingClass().settingsTypes().findByName("phaseCount").id()) {
            thing->setStateValue("phaseCount", value);
        }
    });

    connect(monitor, &NetworkDeviceMonitor::networkDeviceInfoChanged, this, [=](const NetworkDeviceInfo &networkDeviceInfo){
        connection->modbusTcpMaster()->setHostAddress(networkDeviceInfo.address());
    });

    connect(connection, &PhoenixModbusTcpConnection::reachableChanged, thing, [connection, thing](bool reachable){
        qCDebug(dcPhoenixConnect()) << "Reachable state changed" << reachable;
        if (reachable) {
            connection->initialize();
        } else {
            thing->setStateValue("connected", false);
        }
    });

    // Only during setup
    connect(connection, &PhoenixModbusTcpConnection::initializationFinished, info, [this, thing, connection, monitor, info](bool success){
        if (!success) {
            qCDebug(dcPhoenixConnect()) << "Failed to init modbus connection to" << thing->name();
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(monitor);
            connection->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("Could not initialize the communication with the wallbox."));
            return;
        }

        m_connections.insert(thing, connection);
        m_monitors.insert(thing, monitor);
        info->finish(Thing::ThingErrorNoError);
    });

    connect(connection, &PhoenixModbusTcpConnection::updateFinished, thing, [this, connection, thing](){
        qCDebug(dcPhoenixConnect()) << "Update finished:" << thing->name() << connection;
        if (thing->thingClassId() == scapoVisionThingClassId || thing->thingClassId() == wallbeProThingClassId || thing->thingClassId() == compleoProThingClassId) {
            updatePhaseCount(thing);
        }
    });

    connect(connection, &PhoenixModbusTcpConnection::initializationFinished, thing, [thing, connection](bool success){
        if (success) {
            thing->setStateValue("connected", true);
            thing->setStateValue("firmwareVersion", connection->firmwareVersion());
        }
    });

    // Handle property changed signals
    connect(connection, &PhoenixModbusTcpConnection::cpStatusChanged, thing, [thing, this](quint16 cpStatus){
        qCDebug(dcPhoenixConnect()) << "CP Signal state changed:" << (char)cpStatus;
        thing->setStateValue("pluggedIn", cpStatus >= 66);
        evaluateChargingState(thing);
    });

    connect(connection, &PhoenixModbusTcpConnection::chargingEnabledChanged, this, [thing, this](quint16 chargingEnabled){
        qCDebug(dcPhoenixConnect()) << "Charging enabled changed:" << chargingEnabled;
        evaluateChargingState(thing);
    });

    connect(connection, &PhoenixModbusTcpConnection::chargingPausedChanged, this, [thing, this](quint16 chargingPaused){
        qCDebug(dcPhoenixConnect()) << "Charging paused changed:" << chargingPaused;
        thing->setStateValue("power", chargingPaused == 0);
        evaluateChargingState(thing);
    });

    connect(connection, &PhoenixModbusTcpConnection::chargingAllowedChanged, this, [thing, this](quint16 chargingEnabled){
        qCDebug(dcPhoenixConnect()) << "Charging enabled changed:" << chargingEnabled;
        evaluateChargingState(thing);
    });

    connect(connection, &PhoenixModbusTcpConnection::chargingCurrentChanged, thing, [/*thing*/](quint16 chargingCurrent) {
        qCDebug(dcPhoenixConnect()) << "Charging current changed" << chargingCurrent / 10;
    });

    connect(connection, &PhoenixModbusTcpConnection::maximumChargingCurrentChanged, thing, [thing](quint16 maxChargingCurrent) {
        qCDebug(dcPhoenixConnect()) << "Max charging current changed" << maxChargingCurrent;
        thing->setStateValue("maxChargingCurrent", maxChargingCurrent / 10.0); // 100mA -> 1A
    });

    connect(connection, &PhoenixModbusTcpConnection::activePowerChanged, thing, [thing](quint32 activePower) {
        qCDebug(dcPhoenixConnect()) << "Active power consumption changed" << activePower;
        if (thing->hasState("currentPower")) {
            // Note: Explicitly casting to to signed integer as apparently in some setups the meter may
            // occationally report negative energy flow by some 1 - 3 Watt causing the modbus controller
            // register to underflow and report values like 0xFFFFFFFE (-2W) etc.
            thing->setStateValue("currentPower", (qint32)activePower);
        }
    });

    connect(connection, &PhoenixModbusTcpConnection::totalEnergyChanged, thing, [thing](quint32 totalEnergy) {
        qCDebug(dcPhoenixConnect()) << "Total energy consumption changed" << totalEnergy;
        if (thing->hasState("totalEnergyConsumed")) {
            thing->setStateValue("totalEnergyConsumed", 1.0 * totalEnergy / 1000);
        }
    });

    connect(connection, &PhoenixModbusTcpConnection::errorCodeChanged, thing, [](PhoenixModbusTcpConnection::ErrorCode errorCode){
        qCDebug(dcPhoenixConnect()) << "Error code changed:" << errorCode;
    });

    connection->connectDevice();
}

void IntegrationPluginPhoenixConnect::postSetupThing(Thing *thing)
{
    qCDebug(dcPhoenixConnect()) << "Post setup thing" << thing->name();

    if (!m_pluginTimer) {
        qCDebug(dcPhoenixConnect()) << "Starting plugin timer";
        m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(10);
        connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
            foreach (Thing *thing, myThings()) {
                NetworkDeviceMonitor *monitor = m_monitors.value(thing);
                if (thing->setupStatus() == Thing::ThingSetupStatusComplete && monitor && monitor->reachable()) {
                    qCDebug(dcPhoenixConnect()) << "Updating" << thing->name() << monitor->macAddress() << monitor->networkDeviceInfo().address().toString();
                    if (EBoxProfessionalModbusTcpConnection *connection = m_eBoxConnections.value(thing)) {
                        connection->update();
                    } else if (PhoenixModbusTcpConnection *connection = m_connections.value(thing)) {
                        connection->update();
                    }
                } else {
                    qCDebug(dcPhoenixConnect()) << thing->name() << "isn't reachable. Not updating.";
                }
            }
        });
    }
}

void IntegrationPluginPhoenixConnect::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    if (thing->thingClassId() == compleoEBoxProfessionalThingClassId) {
        executeEBoxProfessionalAction(info);
        return;
    }

    PhoenixModbusTcpConnection *connection = m_connections.value(thing);
    if (!connection) {
        qCWarning(dcPhoenixConnect()) << "Modbus connection not available";
        info->finish(Thing::ThingErrorHardwareFailure);
        return;
    }

    ActionType actionType = thing->thingClass().actionTypes().findById(info->action().actionTypeId());
    if (actionType.name() == "power") {
        bool enabled = info->action().paramValue(actionType.id()).toBool();

        QModbusReply *reply = connection->setChargingPaused(!enabled);
        connect(reply, &QModbusReply::finished, info, [info, thing, reply, enabled](){
            if (reply->error() != QModbusDevice::NoError) {
                qCWarning(dcPhoenixConnect()) << "Error" << (enabled ? "starting" : "stopping") << "charging:" << reply->error() << reply->errorString();
                info->finish(Thing::ThingErrorHardwareFailure);
            } else {
                qCDebug(dcPhoenixConnect()) << "Charging" << (enabled ? "started" : "stopped") << "with success";
                thing->setStateValue("power", enabled);
                info->finish(Thing::ThingErrorNoError);
            }
        });

    } else if (actionType.name() == "maxChargingCurrent") {
        double current = qRound(action.param(actionType.id()).value().toDouble() * 10) / 10.0;

        qCDebug(dcPhoenixConnect()) << "Charging power set to" << current;
        QModbusReply *reply = connection->setMaximumChargingCurrent(current * 10);
        connect(reply, &QModbusReply::finished, info, [info, thing, reply, current](){
            if (reply->error() != QModbusDevice::NoError) {
                qCWarning(dcPhoenixConnect()) << "Error setting charging current" << reply->error() << reply->errorString();
                info->finish(Thing::ThingErrorHardwareFailure);
            } else {
                qCDebug(dcPhoenixConnect()) << "Max charging current set to" << current;
                thing->setStateValue("maxChargingCurrent", current);
                info->finish(Thing::ThingErrorNoError);
            }
        });

    } else {
        Q_ASSERT_X(false, "executeAction", QString("Unhandled action: %1").arg(actionType.name()).toUtf8());
    }
}

void IntegrationPluginPhoenixConnect::thingRemoved(Thing *thing)
{
    qCDebug(dcPhoenixConnect()) << "Removing device" << thing->name();
    if (m_connections.contains(thing)) {
        m_connections.take(thing)->deleteLater();
    }
    if (m_eBoxConnections.contains(thing))
        m_eBoxConnections.take(thing)->deleteLater();
    m_eBoxChargingCurrentStateBuffer.remove(thing);
    m_eBoxChargingCurrentWriteQueues.remove(thing);
    m_eBoxChargingCurrentWritesActive.remove(thing);
    if (m_monitors.contains(thing))
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));

    if (myThings().isEmpty()) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginPhoenixConnect::setupEBoxProfessional(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    if (m_eBoxConnections.contains(thing))
        m_eBoxConnections.take(thing)->deleteLater();
    if (m_monitors.contains(thing))
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));

    NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(thing);
    if (!monitor) {
        qCWarning(dcPhoenixConnect()) << "Unable to create network monitor for" << thing;
        info->finish(Thing::ThingErrorInvalidParameter);
        return;
    }

    const quint16 port = thing->paramValue(compleoEBoxProfessionalThingPortParamTypeId).toUInt();
    constexpr quint16 slaveId = 1;
    EBoxProfessionalModbusTcpConnection *connection = new EBoxProfessionalModbusTcpConnection(
                monitor->networkDeviceInfo().address(), port, slaveId, this);

    connect(thing, &Thing::settingChanged, connection, [this, thing](const ParamTypeId &paramTypeId, const QVariant &) {
        if (paramTypeId == compleoEBoxProfessionalSettingsPhaseCountParamTypeId)
            updateEBoxProfessionalState(thing);
    });

    connect(info, &ThingSetupInfo::aborted, connection, &EBoxProfessionalModbusTcpConnection::deleteLater);
    connect(info, &ThingSetupInfo::aborted, monitor, [this, monitor]() {
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(monitor);
    });

    connect(monitor, &NetworkDeviceMonitor::networkDeviceInfoChanged, connection,
            [connection](const NetworkDeviceInfo &networkDeviceInfo) {
        connection->modbusTcpMaster()->setHostAddress(networkDeviceInfo.address());
    });

    connect(connection, &EBoxProfessionalModbusTcpConnection::reachableChanged, thing,
            [connection, thing](bool reachable) {
        qCDebug(dcPhoenixConnect()) << "Compleo eBOX reachable changed:" << reachable;
        if (reachable) {
            if (!connection->initializing())
                connection->initialize();
        } else {
            thing->setStateValue(compleoEBoxProfessionalConnectedStateTypeId, false);
        }
    });

    connect(connection, &EBoxProfessionalModbusTcpConnection::initializationFinished, info,
            [this, info, thing, connection, monitor](bool success) {
        if (!success || connection->modbusTableVersion() == 0
                || normalizedModbusString(connection->chargeboxId()).isEmpty()
                || normalizedModbusString(connection->serialNumber()).isEmpty()) {
            qCWarning(dcPhoenixConnect()) << "Could not identify Compleo eBOX professional at"
                                         << connection->modbusTcpMaster()->hostAddress();
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(monitor);
            connection->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure,
                         QT_TR_NOOP("Could not initialize the communication with the wallbox."));
            return;
        }

        m_eBoxConnections.insert(thing, connection);
        m_monitors.insert(thing, monitor);
        info->finish(Thing::ThingErrorNoError);
    });

    connect(connection, &EBoxProfessionalModbusTcpConnection::initializationFinished, thing,
            [this, thing, connection](bool success) {
        if (!success)
            return;

        thing->setStateValue(compleoEBoxProfessionalConnectedStateTypeId, true);
        thing->setStateValue(compleoEBoxProfessionalFirmwareVersionStateTypeId,
                             normalizedModbusString(connection->firmwareVersion()));
        updateEBoxProfessionalState(thing);
        connection->update();
    });

    connect(connection, &EBoxProfessionalModbusTcpConnection::updateFinished, thing,
            [this, thing, connection]() {
        if (!connection->reachable()) {
            thing->setStateValue(compleoEBoxProfessionalConnectedStateTypeId, false);
            return;
        }
        thing->setStateValue(compleoEBoxProfessionalConnectedStateTypeId, true);
        updateEBoxProfessionalState(thing);
    });

    connection->connectDevice();
}

void IntegrationPluginPhoenixConnect::executeEBoxProfessionalAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    EBoxProfessionalModbusTcpConnection *connection = m_eBoxConnections.value(thing);
    if (!connection) {
        info->finish(Thing::ThingErrorHardwareFailure);
        return;
    }

    const ActionType actionType = thing->thingClass().actionTypes().findById(info->action().actionTypeId());
    if (actionType.name() == "power") {
        const bool enabled = info->action().paramValue(actionType.id()).toBool();
        m_eBoxChargingCurrentStateBuffer[thing].power = enabled;
        enqueueEBoxProfessionalStateWrite(info, connection);
        return;
    }

    if (actionType.name() == "maxChargingCurrent") {
        const double current = qRound(info->action().paramValue(actionType.id()).toDouble() * 10) / 10.0;
        if (current < 6 || current > 32) {
            info->finish(Thing::ThingErrorInvalidParameter);
            return;
        }

        m_eBoxChargingCurrentStateBuffer[thing].maxChargingCurrent = current;
        enqueueEBoxProfessionalStateWrite(info, connection);
        return;
    }

    Q_ASSERT_X(false, "executeEBoxProfessionalAction", QString("Unhandled action: %1").arg(actionType.name()).toUtf8());
}

void IntegrationPluginPhoenixConnect::enqueueEBoxProfessionalStateWrite(
        ThingActionInfo *info, EBoxProfessionalModbusTcpConnection *connection)
{
    Thing *thing = info->thing();
    EBoxChargingCurrentRequest request;
    request.state = m_eBoxChargingCurrentStateBuffer.value(thing);
    request.info = info;
    m_eBoxChargingCurrentWriteQueues[thing].enqueue(request);

    sendNextEBoxProfessionalStateWrite(thing, connection);
}

void IntegrationPluginPhoenixConnect::sendNextEBoxProfessionalStateWrite(
        Thing *thing, EBoxProfessionalModbusTcpConnection *connection)
{
    if (m_eBoxChargingCurrentWritesActive.contains(thing)
            || m_eBoxChargingCurrentWriteQueues.value(thing).isEmpty())
        return;

    const EBoxChargingCurrentRequest request = m_eBoxChargingCurrentWriteQueues[thing].dequeue();
    const float registerCurrent = request.state.power
            ? static_cast<float>(request.state.maxChargingCurrent) : 0.0f;

    QVector<quint16> values;
    values.append(connection->setMaxCurrentPhase1DataUnit(registerCurrent).values());
    values.append(connection->setMaxCurrentPhase2DataUnit(registerCurrent).values());
    values.append(connection->setMaxCurrentPhase3DataUnit(registerCurrent).values());

    QModbusDataUnit dataUnit(QModbusDataUnit::HoldingRegisters,
                             EBoxProfessionalModbusTcpConnection::RegisterMaxCurrentPhase1,
                             values.size());
    dataUnit.setValues(values);

    m_eBoxChargingCurrentWritesActive.insert(thing);
    QModbusReply *reply = connection->modbusTcpMaster()->sendWriteRequest(dataUnit, connection->slaveId());

    if (!reply) {
        qCWarning(dcPhoenixConnect()) << "Could not create eBOX charging current block write request";
        m_eBoxChargingCurrentWritesActive.remove(thing);
        if (request.info)
            request.info->finish(Thing::ThingErrorHardwareFailure);
        sendNextEBoxProfessionalStateWrite(thing, connection);
        return;
    }

    connect(reply, &QModbusReply::finished, this, [this, thing, connection, reply, request, registerCurrent]() {
        reply->deleteLater();
        m_eBoxChargingCurrentWritesActive.remove(thing);

        if (reply->error() != QModbusDevice::NoError) {
            qCWarning(dcPhoenixConnect()) << "Error setting eBOX charging state"
                                         << request.state.power << request.state.maxChargingCurrent << "A:"
                                         << reply->errorString();
            if (request.info)
                request.info->finish(Thing::ThingErrorHardwareFailure);
        } else {
            qCDebug(dcPhoenixConnect()) << "Set eBOX charging state"
                                       << request.state.power << request.state.maxChargingCurrent << "A"
                                       << "using register current" << registerCurrent << "A";
            thing->setStateValue(compleoEBoxProfessionalPowerStateTypeId, request.state.power);
            thing->setStateValue(compleoEBoxProfessionalMaxChargingCurrentStateTypeId,
                                 request.state.maxChargingCurrent);
            if (request.info)
                request.info->finish(Thing::ThingErrorNoError);
        }

        if (m_eBoxConnections.value(thing) == connection)
            sendNextEBoxProfessionalStateWrite(thing, connection);
    });
}

void IntegrationPluginPhoenixConnect::updateEBoxProfessionalState(Thing *thing)
{
    EBoxProfessionalModbusTcpConnection *connection = m_eBoxConnections.value(thing);
    if (!connection)
        return;

    qCDebug(dcPhoenixConnect()) << "Updated" << thing->name() << connection;

    const QString mode3State = normalizedModbusString(connection->socket1Mode3State()).toUpper();
    const bool pluggedIn = mode3State.startsWith('B') || mode3State.startsWith('C') || mode3State.startsWith('D')
            || connection->socket1CableState() == EBoxProfessionalModbusTcpConnection::CableStateLockedCableWithCar;
    const bool charging = mode3State == QStringLiteral("C2") || mode3State == QStringLiteral("D2");
    thing->setStateValue(compleoEBoxProfessionalPluggedInStateTypeId, pluggedIn);
    thing->setStateValue(compleoEBoxProfessionalChargingStateTypeId, charging);

    const double maxCurrent = qRound(qMin(connection->maxCurrentPhase1(),
                                          qMin(connection->maxCurrentPhase2(), connection->maxCurrentPhase3())) * 10.0) / 10.0;
    if (maxCurrent >= 6 && maxCurrent <= 32)
        thing->setStateValue(compleoEBoxProfessionalMaxChargingCurrentStateTypeId, maxCurrent);
    const bool power = maxCurrent > 0.0;
    thing->setStateValue(compleoEBoxProfessionalPowerStateTypeId, power);

    if (!m_eBoxChargingCurrentStateBuffer.contains(thing)) {
        EBoxChargingCurrentState state;
        state.power = power;
        if (maxCurrent >= 6.0 && maxCurrent <= 32.0)
            state.maxChargingCurrent = maxCurrent;
        else
            state.maxChargingCurrent = qBound(6.0,
                                              thing->stateValue(compleoEBoxProfessionalMaxChargingCurrentStateTypeId).toDouble(),
                                              32.0);
        m_eBoxChargingCurrentStateBuffer.insert(thing, state);
    }

    const int mappedPhaseCount = phaseCountFromSetup(connection->stationPhaseSetupL1(),
                                                     connection->stationPhaseSetupL2(),
                                                     connection->stationPhaseSetupL3());
    const uint configuredPhaseCount = qBound(1u,
                                             thing->setting(compleoEBoxProfessionalSettingsPhaseCountParamTypeId).toUInt(),
                                             3u);
    thing->setStateValue(compleoEBoxProfessionalPhaseCountStateTypeId,
                         mappedPhaseCount > 0 ? mappedPhaseCount : configuredPhaseCount);
}

void IntegrationPluginPhoenixConnect::updatePhaseCount(Thing *thing)
{
    PhoenixModbusTcpConnection *connection = m_connections.value(thing);
    int phaseCount = 0;
    if (connection->voltageI1() > 100) {
        phaseCount++;
    }
    if (connection->voltageI2() > 100) {
        phaseCount++;
    }
    if (connection->voltageI3() > 100) {
        phaseCount++;
    }
    thing->setStateValue("phaseCount", qMax(1, phaseCount));
}

void IntegrationPluginPhoenixConnect::evaluateChargingState(Thing *thing)
{
    PhoenixModbusTcpConnection *connection = m_connections.value(thing);
    bool charging = connection->cpStatus() >= 67 && connection->chargingPaused() == 0 && connection->chargingAllowed() == 1;
    thing->setStateValue("charging", charging);
}
