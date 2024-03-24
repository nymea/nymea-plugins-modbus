/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2024, nymea GmbH
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

#include "integrationpluginsungrow.h"
#include "plugininfo.h"
#include "sungrowdiscovery.h"

#include <network/networkdevicediscovery.h>
#include <hardwaremanager.h>

IntegrationPluginSungrow::IntegrationPluginSungrow()
{

}

void IntegrationPluginSungrow::discoverThings(ThingDiscoveryInfo *info)
{
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcSungrow()) << "The network discovery is not available on this platform.";
        info->finish(Thing::ThingErrorUnsupportedFeature, QT_TR_NOOP("The network device discovery is not available."));
        return;
    }

    // Create a discovery with the info as parent for auto deleting the object once the discovery info is done
    SungrowDiscovery *discovery = new SungrowDiscovery(hardwareManager()->networkDeviceDiscovery(), 502, 1, info);
    connect(discovery, &SungrowDiscovery::discoveryFinished, info, [=](){
        foreach (const SungrowDiscovery::SungrowDiscoveryResult &result, discovery->discoveryResults()) {
            QString title = "Sungrow Inverter";

            if (!result.serialNumber.isEmpty())
                title.append(" " + result.serialNumber);

            ThingDescriptor descriptor(sungrowInverterTcpThingClassId, title, result.networkDeviceInfo.address().toString() + " " + result.networkDeviceInfo.macAddress());
            qCInfo(dcSungrow()) << "Discovered:" << descriptor.title() << descriptor.description();

            // Check if we already have set up this device
            Things existingThings = myThings().filterByParam(sungrowInverterTcpThingMacAddressParamTypeId, result.networkDeviceInfo.macAddress());
            if (existingThings.count() == 1) {
                qCDebug(dcSungrow()) << "This Sungrow inverter already exists in the system:" << result.networkDeviceInfo;
                descriptor.setThingId(existingThings.first()->id());
            }

            ParamList params;
            params << Param(sungrowInverterTcpThingMacAddressParamTypeId, result.networkDeviceInfo.macAddress());
            descriptor.setParams(params);
            info->addThingDescriptor(descriptor);
        }

        info->finish(Thing::ThingErrorNoError);
    });

    // Start the discovery process
    discovery->startDiscovery();
}

void IntegrationPluginSungrow::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCInfo(dcSungrow()) << "Setup" << thing << thing->params();

    // Inverter (connection)
    if (thing->thingClassId() == sungrowInverterTcpThingClassId) {

        // Handle reconfiguration
        if (m_tcpConnections.contains(thing)) {
            qCDebug(dcSungrow()) << "Reconfiguring existing thing" << thing->name();
            m_tcpConnections.take(thing)->deleteLater();
            if (m_monitors.contains(thing)) {
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        }

        MacAddress macAddress = MacAddress(thing->paramValue(sungrowInverterTcpThingMacAddressParamTypeId).toString());
        if (!macAddress.isValid()) {
            qCWarning(dcSungrow()) << "The configured mac address is not valid" << thing->params();
            info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The MAC address is not known. Please reconfigure the thing."));
            return;
        }

        // Create the monitor
        NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(macAddress);
        m_monitors.insert(thing, monitor);
        connect(info, &ThingSetupInfo::aborted, monitor, [=](){
            // Clean up in case the setup gets aborted
            if (m_monitors.contains(thing)) {
                qCDebug(dcSungrow()) << "Unregister monitor because the setup has been aborted.";
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        });

        QHostAddress address = m_monitors.value(thing)->networkDeviceInfo().address();
        uint port = thing->paramValue(sungrowInverterTcpThingPortParamTypeId).toUInt();
        quint16 slaveId = thing->paramValue(sungrowInverterTcpThingSlaveIdParamTypeId).toUInt();

        qCInfo(dcSungrow()) << "Setting up Sungrow on" << address.toString() << port << "unit ID:" << slaveId;
        auto sungrowConnection = new SungrowModbusTcpConnection(address, port, slaveId, this);
        connect(info, &ThingSetupInfo::aborted, sungrowConnection, &SungrowModbusTcpConnection::deleteLater);

        // Reconnect on monitor reachable changed
        connect(monitor, &NetworkDeviceMonitor::reachableChanged, thing, [=](bool reachable){
            qCDebug(dcSungrow()) << "Network device monitor reachable changed for" << thing->name() << reachable;
            if (!thing->setupComplete())
                return;

            if (reachable && !thing->stateValue("connected").toBool()) {
                sungrowConnection->modbusTcpMaster()->setHostAddress(monitor->networkDeviceInfo().address());
                sungrowConnection->reconnectDevice();
            } else if (!reachable) {
                // Note: We disable auto reconnect explicitly and we will
                // connect the device once the monitor says it is reachable again
                sungrowConnection->disconnectDevice();
            }
        });

        connect(sungrowConnection, &SungrowModbusTcpConnection::reachableChanged, thing, [this, thing, sungrowConnection](bool reachable){
            qCDebug(dcSungrow()) << "Reachable changed to" << reachable << "for" << thing;
            if (reachable) {
                // Connected true will be set after successfull init
                sungrowConnection->initialize();
            } else {
                thing->setStateValue("connected", false);
                foreach (Thing *childThing, myThings().filterByParentId(thing->id())) {
                    childThing->setStateValue("connected", false);
                }

                // Reset any energy data due to connection loss
                Thing *child = getMeterThing(thing, 1);
                if (child) {
                    child->setStateValue(sungrowMeterCurrentPowerStateTypeId, 0);
                }

                child = getMeterThing(thing, 2);
                if (child) {
                    child->setStateValue(sungrowMeterCurrentPowerStateTypeId, 0);
                    child->setStateValue(sungrowMeterCurrentPowerPhaseAStateTypeId, 0);
                    child->setStateValue(sungrowMeterCurrentPowerPhaseBStateTypeId, 0);
                    child->setStateValue(sungrowMeterCurrentPowerPhaseCStateTypeId, 0);
                }

                child = getBatteryThing(thing);
                if (child) {
//                    child->setStateValue(sungrowBatteryVoltageStateTypeId, sungrowConnection->batteryVoltage());
                    child->setStateValue(sungrowBatteryCurrentPowerStateTypeId, 0);
                    child->setStateValue(sungrowBatteryChargingStateStateTypeId, "idle");
                }
            }
        });

        connect(sungrowConnection, &SungrowModbusTcpConnection::initializationFinished, thing, [=](bool success){
            thing->setStateValue("connected", success);

            foreach (Thing *childThing, myThings().filterByParentId(thing->id())) {
                childThing->setStateValue("connected", success);
            }

            if (!success) {
                // Try once to reconnect the device
                sungrowConnection->reconnectDevice();
            } else {
                qCInfo(dcSungrow()) << "Connection initialized successfully for" << thing;
                sungrowConnection->update();
            }
        });

        connect(sungrowConnection, &SungrowModbusTcpConnection::updateFinished, thing, [=](){
            qCDebug(dcSungrow()) << "Updated" << sungrowConnection;

            ThingClass meterThingClass = thingClass(sungrowMeterThingClassId);

            // Check if we have to create the meter for this Sungrow inverter, or remove it due to communication errors
            if (myThings().filterByParentId(thing->id()).filterByThingClassId(sungrowMeterThingClassId).isEmpty()) {
                if (true == 1 && myThings().filterByParentId(thing->id()).filterByThingClassId(sungrowMeterThingClassId).filterByParam(sungrowMeterThingIdParamTypeId, 1).isEmpty()) {
                    //TODO replace true with a meter available check
                    qCDebug(dcSungrow()) << "There is no meter set up for this inverter. Creating a meter 1 for" << thing << sungrowConnection->modbusTcpMaster();
                    ThingDescriptor descriptor(sungrowMeterThingClassId, meterThingClass.displayName(), QString(), thing->id());
                    ParamList params;
                    params.append(Param(sungrowMeterThingIdParamTypeId, 1));
                    descriptor.setParams(params);
                    emit autoThingsAppeared(ThingDescriptors() << descriptor);
                }
            }

            // Check if we have to create the battery for the Sungrow inverter
//            if (sungrowConnection->batteryType() != SungrowModbusTcpConnection::BatteryTypeNoBattery && myThings().filterByParentId(thing->id()).filterByThingClassId(sungrowBatteryThingClassId).isEmpty()) {
//                qCDebug(dcSungrow()) << "There is a battery connected but not set up yet. Creating a battery...";
//                ThingClass batteryThingClass = thingClass(sungrowBatteryThingClassId);
//                ThingDescriptor descriptor(sungrowBatteryThingClassId, batteryThingClass.displayName(), QString(), thing->id());
//                emit autoThingsAppeared(ThingDescriptors() << descriptor);
//            }

            // Update inverter states


            thing->setStateValue(sungrowInverterTcpCurrentPowerStateTypeId, sungrowConnection->totalPVPower());
            thing->setStateValue(sungrowInverterTcpTemperatureStateTypeId, sungrowConnection->inverterTemperature());
//            thing->setStateValue(sungrowInverterTcpFrequencyStateTypeId, sungrowConnection->gridFrequency());
//            thing->setStateValue(sungrowInverterTcpTotalEnergyProducedStateTypeId, sungrowConnection->totalPVExport());

            // Update the meter 1 if available
            Thing *meterThing = getMeterThing(thing, 1);
            if (meterThing) {
//                meterThing->setStateValue(sungrowMeterTotalEnergyConsumedStateTypeId, sungrowConnection->totalImportEnergy());
//                meterThing->setStateValue(sungrowMeterTotalEnergyProducedStateTypeId, sungrowConnection->totalExportEnergy());

//                // Power
//                meterThing->setStateValue(sungrowMeterCurrentPowerStateTypeId, sungrowConnection->totalActivePower());
            }

            // Update the battery if available
            Thing *batteryThing = getBatteryThing(thing);
            if (batteryThing) {
//                batteryThing->setStateValue(sungrowBatteryVoltageStateTypeId, sungrowConnection->batteryVoltage());
//                batteryThing->setStateValue(sungrowBatteryTemperatureStateTypeId, sungrowConnection->batteryTemperature());
//                batteryThing->setStateValue(sungrowBatteryBatteryLevelStateTypeId, sungrowConnection->batteryLevel());
//                batteryThing->setStateValue(sungrowBatteryBatteryCriticalStateTypeId, sungrowConnection->batteryLevel() < 5);

//                batteryThing->setStateValue(sungrowBatteryCurrentPowerStateTypeId, sungrowConnection->batteryPower());
                quint16 runningState = 0x01; //TODO sungrowConnection->runningState();
                if (runningState & (0x1 << 1)) { //Bit 1: Battery charging bit
                    batteryThing->setStateValue(sungrowBatteryChargingStateStateTypeId, "charging");
                } else if (runningState & (0x1 << 2)) { //Bit 2: Battery discharging bit
                    batteryThing->setStateValue(sungrowBatteryChargingStateStateTypeId, "discharging");
                } else {
                    batteryThing->setStateValue(sungrowBatteryChargingStateStateTypeId, "idle");
                }
            }
        });

        m_tcpConnections.insert(thing, sungrowConnection);

        if (monitor->reachable())
            sungrowConnection->connectDevice();

        info->finish(Thing::ThingErrorNoError);
        return;
    }

    // Meter
    if (thing->thingClassId() == sungrowMeterThingClassId) {

        // Get the parent thing and the associated connection
        Thing *connectionThing = myThings().findById(thing->parentId());
        if (!connectionThing) {
            qCWarning(dcSungrow()) << "Failed to set up Sungrow energy meter because the parent thing with ID" << thing->parentId().toString() << "could not be found.";
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        auto sungrowConnection = m_tcpConnections.value(connectionThing);
        if (!sungrowConnection) {
            qCWarning(dcSungrow()) << "Failed to set up Sungrow energy meter because the connection for" << connectionThing << "does not exist.";
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        // Note: The states will be handled in the parent inverter thing on updated

        info->finish(Thing::ThingErrorNoError);
        return;
    }

    // Battery
    if (thing->thingClassId() == sungrowBatteryThingClassId) {
        // Get the parent thing and the associated connection
        Thing *connectionThing = myThings().findById(thing->parentId());
        if (!connectionThing) {
            qCWarning(dcSungrow()) << "Failed to set up Sungrow battery because the parent thing with ID" << thing->parentId().toString() << "could not be found.";
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        auto sungrowConnection = m_tcpConnections.value(connectionThing);
        if (!sungrowConnection) {
            qCWarning(dcSungrow()) << "Failed to set up Sungrow battery because the connection for" << connectionThing << "does not exist.";
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        // Note: The states will be handled in the parent inverter thing on updated

        info->finish(Thing::ThingErrorNoError);
        return;
    }
}

void IntegrationPluginSungrow::postSetupThing(Thing *thing)
{

    if (thing->thingClassId() == sungrowInverterTcpThingClassId) {

        // Create the update timer if not already set up
        if (!m_refreshTimer) {
            qCDebug(dcSungrow()) << "Starting plugin timer...";
            m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(2);
            connect(m_refreshTimer, &PluginTimer::timeout, this, [this] {
                foreach(SungrowModbusTcpConnection *connection, m_tcpConnections) {
                    if (connection->initializing())
                        continue;

                    //qCDebug(dcSungrow()) << "Update connection" << connection->modbusTcpMaster()->hostAddress().toString();
                    connection->update();
                }
            });

            m_refreshTimer->start();
        }
        return;
    }

    if (thing->thingClassId() == sungrowMeterThingClassId || thing->thingClassId() == sungrowBatteryThingClassId) {
        Thing *connectionThing = myThings().findById(thing->parentId());
        if (connectionThing) {
            thing->setStateValue("connected", connectionThing->stateValue("connected"));
        }

        return;
    }
}

void IntegrationPluginSungrow::thingRemoved(Thing *thing)
{
    if (thing->thingClassId() == sungrowInverterTcpThingClassId && m_tcpConnections.contains(thing)) {
        auto connection = m_tcpConnections.take(thing);
        connection->modbusTcpMaster()->disconnectDevice();
        delete connection;
    }

    // Unregister related hardware resources
    if (m_monitors.contains(thing))
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));

    if (myThings().isEmpty() && m_refreshTimer) {
        qCDebug(dcSungrow()) << "Stopping refresh timer";
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_refreshTimer);
        m_refreshTimer = nullptr;
    }
}

Thing *IntegrationPluginSungrow::getMeterThing(Thing *parentThing, uint meterId)
{
    Things meterThings = myThings().filterByParentId(parentThing->id()).filterByThingClassId(sungrowMeterThingClassId).filterByParam(sungrowMeterThingIdParamTypeId, meterId);
    if (meterThings.isEmpty())
        return nullptr;

    return meterThings.first();
}

Thing *IntegrationPluginSungrow::getBatteryThing(Thing *parentThing)
{
    Things batteryThings = myThings().filterByParentId(parentThing->id()).filterByThingClassId(sungrowBatteryThingClassId);
    if (batteryThings.isEmpty())
        return nullptr;

    return batteryThings.first();
}
