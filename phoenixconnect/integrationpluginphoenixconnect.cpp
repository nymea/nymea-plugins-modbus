/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
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

#include "integrationpluginphoenixconnect.h"
#include "plugininfo.h"

#include "phoenixmodbustcpconnection.h"
#include "phoenixdiscovery.h"

#include <network/networkdevicediscovery.h>
#include <types/param.h>
#include <plugintimer.h>

#include <QDebug>
#include <QStringList>
#include <QJsonDocument>
#include <QNetworkInterface>


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

    PhoenixDiscovery *discovery = new PhoenixDiscovery(hardwareManager()->networkDeviceDiscovery(), info);
    connect(discovery, &PhoenixDiscovery::discoveryFinished, info, [=](){
        foreach (const PhoenixDiscovery::Result &result, discovery->discoveryResults()) {

            QString name = supportedThings().findById(info->thingClassId()).displayName();
            QString description = result.serialNumber;
            ThingDescriptor descriptor(info->thingClassId(), name, description);
            qCDebug(dcPhoenixConnect()) << "Discovered:" << descriptor.title() << descriptor.description();

            ParamTypeId macParamTypeId = supportedThings().findById(info->thingClassId()).paramTypes().findByName("mac").id();
            Things existingThings = myThings().filterByParam(macParamTypeId, result.networkDeviceInfo.macAddress());
            if (existingThings.count() == 1) {
                qCDebug(dcPhoenixConnect()) << "This wallbox already exists in the system:" << result.networkDeviceInfo;
                descriptor.setThingId(existingThings.first()->id());
            }

            ParamList params;
            params << Param(macParamTypeId, result.networkDeviceInfo.macAddress());
            descriptor.setParams(params);
            info->addThingDescriptor(descriptor);
        }

        info->finish(Thing::ThingErrorNoError);
    });
    discovery->startDiscovery();
}


void IntegrationPluginPhoenixConnect::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    if (m_connections.contains(thing)) {
        qCDebug(dcPhoenixConnect()) << "Reconfiguring existing thing" << thing->name();
        m_connections.take(thing)->deleteLater();
    } else {
        qCDebug(dcPhoenixConnect()) << "Setting up a new device:" << thing->params();
    }


    MacAddress mac = MacAddress(thing->paramValue("mac").toString());
    if (!mac.isValid()) {
        info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The given MAC address is not valid."));
        return;
    }
    NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(mac);

    PhoenixModbusTcpConnection *connection = new PhoenixModbusTcpConnection(monitor->networkDeviceInfo().address(), 502, 255, this);
    connect(info, &ThingSetupInfo::aborted, connection, &PhoenixModbusTcpConnection::deleteLater);
    connect(info, &ThingSetupInfo::aborted, monitor, [monitor, this](){ hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(monitor);});

    connect(thing, &Thing::settingChanged, this, [thing](const ParamTypeId &paramTypeId, const QVariant &value){
        if (paramTypeId == thing->thingClass().settingsTypes().findByName("phaseCount").id()) {
            thing->setStateValue("phaseCount", value);
        }
    });

    connect(monitor, &NetworkDeviceMonitor::networkDeviceInfoChanged, this, [=](const NetworkDeviceInfo &networkDeviceInfo){
        connection->setHostAddress(networkDeviceInfo.address());
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
        thing->setStateValue("maxChargingCurrent", 1.0 * maxChargingCurrent / 10); // 100mA -> 1A
    });

    connect(connection, &PhoenixModbusTcpConnection::activePowerChanged, thing, [thing](quint32 activePower) {
        qCDebug(dcPhoenixConnect()) << "Active power consumption changed" << activePower;
        if (thing->hasState("currentPower")) {
            thing->setStateValue("currentPower", activePower);
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
                if (thing->setupStatus() == Thing::ThingSetupStatusComplete && m_monitors.value(thing)->reachable()) {
                    qCDebug(dcPhoenixConnect()) << "Updating" << thing->name() << m_monitors.value(thing)->macAddress() << m_monitors.value(thing)->networkDeviceInfo().address().toString();
                    m_connections.value(thing)->update();
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
        uint16_t current = action.param(actionType.id()).value().toUInt();
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
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
    }

    if (myThings().isEmpty()) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
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
