/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2022, nymea GmbH
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

#include "integrationpluginidm.h"
#include "plugininfo.h"

#include <network/networkdevicediscovery.h>
#include <hardwaremanager.h>

IntegrationPluginIdm::IntegrationPluginIdm()
{

}

void IntegrationPluginIdm::discoverThings(ThingDiscoveryInfo *info)
{
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcIdm()) << "Failed to discover network devices. The network device discovery is not available.";
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The discovery is not available."));
        return;
    }

    qCDebug(dcIdm()) << "Discovering network...";
    NetworkDeviceDiscoveryReply *discoveryReply = hardwareManager()->networkDeviceDiscovery()->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        ThingDescriptors descriptors;
        qCDebug(dcIdm()) << "Discovery finished. Found" << discoveryReply->networkDeviceInfos().count() << "devices";
        foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {
            qCDebug(dcIdm()) << networkDeviceInfo;
            QString title;
            if (networkDeviceInfo.hostName().isEmpty()) {
                title += networkDeviceInfo.address().toString();
            } else {
                title += networkDeviceInfo.address().toString() + " (" + networkDeviceInfo.hostName() + ")";
            }

            QString description;
            if (networkDeviceInfo.macAddressManufacturer().isEmpty()) {
                description = networkDeviceInfo.macAddress();
            } else {
                description = networkDeviceInfo.macAddress() + " (" + networkDeviceInfo.macAddressManufacturer() + ")";
            }

            ThingDescriptor descriptor(navigator2ThingClassId, title, description);

            // Check if we already have set up this device
            Things existingThings = myThings().filterByParam(navigator2ThingMacAddressParamTypeId, networkDeviceInfo.macAddress());
            if (existingThings.count() == 1) {
                qCDebug(dcIdm()) << "This thing already exists in the system." << existingThings.first() << networkDeviceInfo;
                descriptor.setThingId(existingThings.first()->id());
            }

            ParamList params;
            params << Param(navigator2ThingMacAddressParamTypeId, networkDeviceInfo.macAddress());
            descriptor.setParams(params);
            info->addThingDescriptor(descriptor);
        }
        info->finish(Thing::ThingErrorNoError);
    });
}

void IntegrationPluginIdm::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcIdm()) << "Setup" << thing << thing->params();

    // Inverter (connection)
    if (thing->thingClassId() == navigator2ThingClassId) {

        // Handle reconfigure
        if (m_connections.contains(thing)) {
            qCDebug(dcIdm()) << "Reconfiguring existing thing" << thing->name();
            m_connections.take(thing)->deleteLater();

            if (m_monitors.contains(thing)) {
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        }

        MacAddress macAddress = MacAddress(thing->paramValue(navigator2ThingMacAddressParamTypeId).toString());
        if (!macAddress.isValid()) {
            qCWarning(dcIdm()) << "The configured mac address is not valid" << thing->params();
            info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The MAC address is not known. Please reconfigure the thing."));
            return;
        }

        // Create the monitor
        NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(macAddress);
        m_monitors.insert(thing, monitor);

        QHostAddress address = monitor->networkDeviceInfo().address();
        if (address.isNull()) {
            qCWarning(dcIdm()) << "Cannot set up thing. The host address is not known yet. Maybe it will be available in the next run...";
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("The host address is not known yet. Trying later again."));
            return;
        }

        // Clean up in case the setup gets aborted
        connect(info, &ThingSetupInfo::aborted, monitor, [=](){
            if (m_monitors.contains(thing)) {
                qCDebug(dcIdm()) << "Unregister monitor because setup has been aborted.";
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        });

        // Wait for the monitor to be ready
        if (monitor->reachable()) {
            // Thing already reachable...let's continue with the setup
            setupConnection(info);
        } else {
            qCDebug(dcIdm()) << "Waiting for the network monitor to get reachable before continue to set up the connection" << thing->name() << address.toString() << "...";
            connect(monitor, &NetworkDeviceMonitor::reachableChanged, info, [=](bool reachable){
                if (reachable) {
                    qCDebug(dcIdm()) << "The monitor for thing setup" << thing->name() << "is now reachable. Continue setup...";
                    setupConnection(info);
                }
            });
        }

        return;
    }
}

void IntegrationPluginIdm::postSetupThing(Thing *thing)
{
    Q_UNUSED(thing)

    if (!m_refreshTimer) {
        qCDebug(dcIdm()) << "Starting refresh timer";
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(10);
        connect(m_refreshTimer, &PluginTimer::timeout, this, [this](){
            foreach (IdmModbusTcpConnection *connection, m_connections) {
                connection->update();
            }
        });

        m_refreshTimer->start();
    }
}

void IntegrationPluginIdm::thingRemoved(Thing *thing)
{
    qCDebug(dcIdm()) << "thingRemoved called" << thing->name();

    if (thing->thingClassId() == navigator2ThingClassId) {
        if (m_connections.contains(thing)) {
            m_connections.take(thing)->deleteLater();
        }
    }

    // Unregister related hardware resources
    if (m_monitors.contains(thing))
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));

    if (myThings().isEmpty()) {
        qCDebug(dcIdm()) << "Stopping refresh timer";
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_refreshTimer);
        m_refreshTimer = nullptr;
    }
}

void IntegrationPluginIdm::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    if (thing->thingClassId() == navigator2ThingClassId) {
        IdmModbusTcpConnection *connection = m_connections.value(thing);
        if (!connection) {
            qCWarning(dcIdm()) << "Failed to execute action. Could not find connection for" << thing;
            info->finish(Thing::ThingErrorHardwareFailure);
            return;
        }

        if (!connection->connected())
            info->finish(Thing::ThingErrorHardwareNotAvailable);


        if (action.actionTypeId() == navigator2TargetTemperatureActionTypeId) {
            float targetTemperature = action.paramValue(navigator2TargetTemperatureActionTargetTemperatureParamTypeId).toDouble();
            qCDebug(dcIdm()) << "Setting room target temperature to" << targetTemperature << "°C";
            QModbusReply *reply = connection->setRoomTemperatureTargetHeatingEco(targetTemperature);
            if (!reply) {
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }

            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, info, [info, reply, thing, targetTemperature]{
                if (reply->error() != QModbusDevice::NoError) {
                    info->finish(Thing::ThingErrorHardwareFailure);
                    return;
                }

                qCDebug(dcIdm()) << "Room target temperature set successfully to" << targetTemperature << "°C";
                thing->setStateValue(navigator2TargetTemperatureStateTypeId, targetTemperature);
                info->finish(Thing::ThingErrorNoError);
            });
        } else {
            Q_ASSERT_X(false, "executeAction", QString("Unhandled action: %1").arg(action.actionTypeId().toString()).toUtf8());
        }
    } else {
        Q_ASSERT_X(false, "executeAction", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginIdm::setupConnection(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    QHostAddress address = m_monitors.value(thing)->networkDeviceInfo().address();

    qCDebug(dcIdm()) << "Setting up IDM on" << address.toString() << 502 << "unit ID:" << 1;
    IdmModbusTcpConnection *connection = new IdmModbusTcpConnection(address, 502, 1, this);
    connect(info, &ThingSetupInfo::aborted, connection, &IdmModbusTcpConnection::deleteLater);

    // Reconnect on monitor reachable changed
    NetworkDeviceMonitor *monitor = m_monitors.value(thing);
    connect(monitor, &NetworkDeviceMonitor::reachableChanged, thing, [=](bool reachable){
        qCDebug(dcIdm()) << "Network device monitor reachable changed for" << thing->name() << reachable;
        if (!thing->setupComplete())
            return;

        if (reachable && !thing->stateValue("connected").toBool()) {
            connection->setHostAddress(monitor->networkDeviceInfo().address());
            connection->connectDevice();
        } else if (!reachable) {
            // Note: We disable autoreconnect explicitly and we will
            // connect the device once the monitor says it is reachable again
            connection->disconnectDevice();
        }
    });

    connect(connection, &IdmModbusTcpConnection::reachableChanged, thing, [this, thing, connection](bool reachable){
        qCDebug(dcIdm()) << "Reachable changed to" << reachable << "for" << thing;
        if (reachable) {
            // Connected true will be set after successfull init
            connection->initialize();
        } else {
            thing->setStateValue("connected", false);
            foreach (Thing *childThing, myThings().filterByParentId(thing->id())) {
                childThing->setStateValue("connected", false);
            }
        }
    });

    connect(connection, &IdmModbusTcpConnection::initializationFinished, thing, [=](bool success){
        if (!thing->setupComplete())
            return;

        thing->setStateValue("connected", success);
        foreach (Thing *childThing, myThings().filterByParentId(thing->id())) {
            childThing->setStateValue("connected", success);
        }

        if (!success) {
            // Try once to reconnect the device
            connection->reconnectDevice();
        }
    });

    connect(connection, &IdmModbusTcpConnection::initializationFinished, info, [=](bool success){
        if (!success) {
            qCWarning(dcIdm()) << "Connection init finished with errors" << thing->name() << connection->hostAddress().toString();
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(monitor);
            connection->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("Could not initialize the communication with the device."));
            return;
        }

        qCDebug(dcIdm()) << "Connection init finished successfully" << connection;
        m_connections.insert(thing, connection);
        info->finish(Thing::ThingErrorNoError);

        // Set connected true
        thing->setStateValue("connected", true);
        foreach (Thing *childThing, myThings().filterByParentId(thing->id())) {
            childThing->setStateValue("connected", true);
        }


        connect(connection, &IdmModbusTcpConnection::updateFinished, thing, [=](){
            qCDebug(dcIdm()) << "Updated" << connection;

            thing->setStateValue(navigator2ConnectedStateTypeId, connection->reachable());
            thing->setStateValue(navigator2TemperatureStateTypeId, connection->roomTemperature());
            thing->setStateValue(navigator2OutsideTemperatureStateTypeId, connection->outdoorTemperature());
            thing->setStateValue(navigator2WaterTemperatureStateTypeId, connection->heatStorageTemperature());
            thing->setStateValue(navigator2TargetTemperatureStateTypeId, connection->roomTemperatureTargetHeatingEco());
            thing->setStateValue(navigator2TargetWaterTemperatureStateTypeId, connection->targetHotWaterTemperature());
            thing->setStateValue(navigator2CurrentPowerStateTypeId, connection->currentPowerConsumption() * 1000.0);
            thing->setStateValue(navigator2EnergyProducedHeatingStateTypeId, connection->energyHeating());
            thing->setStateValue(navigator2EnergyProducedCoolingStateTypeId, connection->energyCooling());
            thing->setStateValue(navigator2EnergyProducedHotWaterStateTypeId, connection->energyHotWater());

            switch (connection->heatPumpOperatingMode()) {
            case IdmModbusTcpConnection::HeatPumpOperationModeOff:
                thing->setStateValue(navigator2ModeStateTypeId, "Off");
                break;
            case IdmModbusTcpConnection::HeatPumpOperationModeHeating:
                thing->setStateValue(navigator2ModeStateTypeId, "Heating");
                break;
            case IdmModbusTcpConnection::HeatPumpOperationModeCooling:
                thing->setStateValue(navigator2ModeStateTypeId, "Cooling");
                break;
            case IdmModbusTcpConnection::HeatPumpOperationModeHotWater:
                thing->setStateValue(navigator2ModeStateTypeId, "Hot water");
                break;
            case IdmModbusTcpConnection::HeatPumpOperationModeDefrost:
                thing->setStateValue(navigator2ModeStateTypeId, "Defrost");
                break;
            }

            thing->setStateValue(navigator2HeatingOnStateTypeId, connection->heatPumpOperatingMode() == IdmModbusTcpConnection::HeatPumpOperationModeHeating);
            thing->setStateValue(navigator2CoolingOnStateTypeId, connection->heatPumpOperatingMode() == IdmModbusTcpConnection::HeatPumpOperationModeCooling);

            switch (connection->systemOperationMode()) {
            case IdmModbusTcpConnection::SystemOperationModeStandby:
                thing->setStateValue(navigator2OperationModeStateTypeId, "Standby");
                break;
            case IdmModbusTcpConnection::SystemOperationModeAutomatic:
                thing->setStateValue(navigator2OperationModeStateTypeId, "Automatic");
                break;
            case IdmModbusTcpConnection::SystemOperationModeAbsent:
                thing->setStateValue(navigator2OperationModeStateTypeId, "Absent");
                break;
            case IdmModbusTcpConnection::SystemOperationModeWarmWaterOnly:
                thing->setStateValue(navigator2OperationModeStateTypeId, "Hot water only");
                break;
            case IdmModbusTcpConnection::SystemOperationModeHeatingCoolingOnly:
                thing->setStateValue(navigator2OperationModeStateTypeId, "Heating cooling only");
                break;
            }

            thing->setStateValue(navigator2ErrorStateTypeId, connection->currentFaultNumber());

        });

        // Update registers
        connection->update();
    });

    connection->connectDevice();
}



