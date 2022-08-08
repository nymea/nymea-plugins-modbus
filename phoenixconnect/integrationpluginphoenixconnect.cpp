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
        qCWarning(dcPhoenixContact()) << "Failed to discover network devices. The network device discovery is not available.";
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The network cannot be searched."));
        return;
    }

    qCDebug(dcPhoenixContact()) << "Starting network discovery...";
    NetworkDeviceDiscoveryReply *discoveryReply = hardwareManager()->networkDeviceDiscovery()->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, info, [=](){
        ThingDescriptors descriptors;
        qCDebug(dcPhoenixContact()) << "Discovery finished. Found" << discoveryReply->networkDeviceInfos().count() << "devices";
        foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {
            qCDebug(dcPhoenixContact()) << networkDeviceInfo;

            if (networkDeviceInfo.macAddressManufacturer() != "wallbe GmbH" && !networkDeviceInfo.macAddressManufacturer().contains("Phoenix", Qt::CaseSensitivity::CaseInsensitive)) {
                continue;
            }

            ThingClass thingClass = supportedThings().findById(info->thingClassId());
            ParamTypeId macAddressParamType = thingClass.paramTypes().findByName("mac").id();

            ThingDescriptor descriptor(info->thingClassId(), thingClass.displayName(), networkDeviceInfo.address().toString());
            descriptor.setParams({Param(macAddressParamType, networkDeviceInfo.macAddress())});

            // Check if we already have set up this device
            Thing *existingThing = myThings().findByParams(descriptor.params());
            if (existingThing) {
                qCDebug(dcPhoenixContact()) << "Found already existing" << thingClass.name() << "wallbox:" << existingThing->name() << networkDeviceInfo;
                descriptor.setThingId(existingThing->id());
            } else {
                qCDebug(dcPhoenixContact()) << "Found new" << thingClass.name() << "wallbox";
            }

            info->addThingDescriptor(descriptor);
        }
        info->finish(Thing::ThingErrorNoError);
    });
}


void IntegrationPluginPhoenixConnect::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    if (m_connections.contains(thing)) {
        qCDebug(dcPhoenixContact()) << "Reconfiguring existing thing" << thing->name();
        m_connections.take(thing)->deleteLater();
    } else {
        qCDebug(dcPhoenixContact()) << "Setting up a new device:" << thing->params();
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

    connect(connection, &PhoenixModbusTcpConnection::reachableChanged, thing, [connection, thing](bool reachable){
        qCDebug(dcPhoenixContact()) << "Reachable state changed" << reachable;
        if (reachable) {
            connection->initialize();
        } else {
            thing->setStateValue("connected", false);
        }
    });

    // Only during setup
    connect(connection, &PhoenixModbusTcpConnection::initializationFinished, info, [this, thing, connection, monitor, info](bool success){
        if (success) {
            qCDebug(dcPhoenixContact()) << "Phoenix wallbox initialized. Firmware version:" << connection->firmwareVersion();
            m_connections.insert(thing, connection);
            m_monitors.insert(thing, monitor);
            info->finish(Thing::ThingErrorNoError);
        } else {
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(monitor);
            connection->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("Could not initialize the communication with the wallbox."));
        }
    });

    connect(connection, &PhoenixModbusTcpConnection::updateFinished, thing, [connection, thing](){
        qCDebug(dcPhoenixContact()) << "Update finished:" << thing->name() << connection;
    });

    connect(connection, &PhoenixModbusTcpConnection::initializationFinished, thing, [thing, connection](bool success){
        if (success) {
            thing->setStateValue("connected", true);
            thing->setStateValue("firmwareVersion", connection->firmwareVersion());
        }
    });

    // Handle property changed signals
    connect(connection, &PhoenixModbusTcpConnection::cpStatusChanged, thing, [thing, connection](quint16 cpStatus){
        qCDebug(dcPhoenixContact()) << "CP Signal state changed:" << (char)cpStatus;
        thing->setStateValue("pluggedIn", cpStatus >= 66);
        thing->setStateValue("charging", cpStatus >= 67 && connection->chargingEnabled() > 0);
    });

    connect(connection, &PhoenixModbusTcpConnection::chargingEnabledChanged, this, [thing, connection](quint16 chargingEnabled){
        qCDebug(dcPhoenixContact()) << "Charging enabled changed:" << chargingEnabled;
        thing->setStateValue("power", chargingEnabled > 0);
        thing->setStateValue("charging", chargingEnabled > 0 && connection->cpStatus() >= 67);
    });

    connect(connection, &PhoenixModbusTcpConnection::chargingCurrentChanged, thing, [/*thing*/](quint16 chargingCurrent) {
        qCDebug(dcPhoenixContact()) << "Charging current changed" << chargingCurrent / 10;
    });

    connect(connection, &PhoenixModbusTcpConnection::maximumChargingCurrentChanged, thing, [thing](quint16 maxChargingCurrent) {
        qCDebug(dcPhoenixContact()) << "Max charging current changed" << maxChargingCurrent;
        thing->setStateValue("maxChargingCurrent", 1.0 * maxChargingCurrent / 10); // 100mA -> 1A
    });

    connect(connection, &PhoenixModbusTcpConnection::activePowerChanged, thing, [thing](quint32 activePower) {
        qCDebug(dcPhoenixContact()) << "Active power consumption changed" << activePower;
        if (thing->hasState("currentPower")) {
            thing->setStateValue("currentPower", activePower);
        }
    });

    connect(connection, &PhoenixModbusTcpConnection::totalEnergyChanged, thing, [thing](quint32 totalEnergy) {
        qCDebug(dcPhoenixContact()) << "Total energy consumption changed" << totalEnergy;
        if (thing->hasState("totalEnergyConsumed")) {
            thing->setStateValue("totalEnergyConsumed", 1.0 * totalEnergy / 1000);
        }
    });

    connect(connection, &PhoenixModbusTcpConnection::errorCodeChanged, thing, [](PhoenixModbusTcpConnection::ErrorCode errorCode){
        qCDebug(dcPhoenixContact()) << "Error code changed:" << errorCode;
    });

    connect(connection, &PhoenixModbusTcpConnection::voltageI1Changed, thing, [this, thing](){ updatePhaseCount(thing); });
    connect(connection, &PhoenixModbusTcpConnection::voltageI2Changed, thing, [this, thing](){ updatePhaseCount(thing); });
    connect(connection, &PhoenixModbusTcpConnection::voltageI3Changed, thing, [this, thing](){ updatePhaseCount(thing); });

    connection->connectDevice();
}

void IntegrationPluginPhoenixConnect::postSetupThing(Thing *thing)
{
    qCDebug(dcPhoenixContact()) << "Post setup thing" << thing->name();

    if (!m_pluginTimer) {
        qCDebug(dcPhoenixContact()) << "Starting plugin timer";
        m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(10);
        connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
            foreach (Thing *thing, myThings()) {
                if (m_monitors.value(thing)->reachable()) {
                    qCDebug(dcPhoenixContact()) << "Updating" << thing->name();
                    m_connections.value(thing)->update();
                } else {
                    qCDebug(dcPhoenixContact()) << thing->name() << "isn't reachable. Not updating.";
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
        qCWarning(dcPhoenixContact()) << "Modbus connection not available";
        info->finish(Thing::ThingErrorHardwareFailure);
        return;
    }

    ActionType actionType = thing->thingClass().actionTypes().findById(info->action().actionTypeId());
    if (actionType.name() == "power") {
        bool enabled = info->action().paramValue(actionType.id()).toBool();
        QModbusReply *reply = connection->setChargingEnabled(enabled);
        connect(reply, &QModbusReply::finished, info, [info, thing, reply, enabled](){
            if (reply->error() != QModbusDevice::NoError) {
                qCWarning(dcPhoenixContact()) << "Error setting charging enabled" << reply->error() << reply->errorString();
                info->finish(Thing::ThingErrorHardwareFailure);
            } else {
                qCDebug(dcPhoenixContact()) << "Charging enabled set with success";
                thing->setStateValue("power", enabled);
                info->finish(Thing::ThingErrorNoError);
            }
        });

    } else if (actionType.name() == "maxChargingCurrent") {
        uint16_t current = action.param(actionType.id()).value().toUInt();
        qCDebug(dcPhoenixContact()) << "Charging power set to" << current;
        QModbusReply *reply = connection->setMaximumChargingCurrent(current * 10);
        connect(reply, &QModbusReply::finished, info, [info, thing, reply, current](){
            if (reply->error() != QModbusDevice::NoError) {
                qCWarning(dcPhoenixContact()) << "Error setting charging current" << reply->error() << reply->errorString();
                info->finish(Thing::ThingErrorHardwareFailure);
            } else {
                qCDebug(dcPhoenixContact()) << "Max charging current set to" << current;
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
    qCDebug(dcPhoenixContact()) << "Removing device" << thing->name();
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
    thing->setStateValue("phaseCount", phaseCount);
}
