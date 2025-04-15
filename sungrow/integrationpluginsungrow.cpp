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
    SungrowDiscovery *discovery = new SungrowDiscovery(hardwareManager()->networkDeviceDiscovery(), m_modbusTcpPort, m_modbusSlaveAddress, info);
    connect(discovery, &SungrowDiscovery::discoveryFinished, info, [=](){
        foreach (const SungrowDiscovery::SungrowDiscoveryResult &result, discovery->discoveryResults()) {
            QString title = "Sungrow " + QString::number(result.nominalOutputPower) + "kW Inverter";

            if (!result.serialNumber.isEmpty())
                title.append(" " + result.serialNumber);

            ThingDescriptor descriptor(sungrowInverterTcpThingClassId, title, result.networkDeviceInfo.address().toString());
            qCInfo(dcSungrow()) << "Discovered:" << descriptor.title() << descriptor.description();

            ParamList params;
            params << Param(sungrowInverterTcpThingMacAddressParamTypeId, result.networkDeviceInfo.thingParamValueMacAddress());
            params << Param(sungrowInverterTcpThingHostNameParamTypeId, result.networkDeviceInfo.thingParamValueHostName());
            params << Param(sungrowInverterTcpThingAddressParamTypeId, result.networkDeviceInfo.thingParamValueAddress());
            descriptor.setParams(params);

            // Check if we already have set up this device
            Thing *existingThing = myThings().findByParams(params);
            if (existingThing) {
                qCDebug(dcSungrow()) << "This thing already exists in the system:" << result.networkDeviceInfo;
                descriptor.setThingId(existingThing->id());
            }

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

    if (thing->thingClassId() == sungrowInverterTcpThingClassId) {

        // Handle reconfiguration
        if (m_tcpConnections.contains(thing)) {
            qCDebug(dcSungrow()) << "Reconfiguring existing thing" << thing->name();
            m_tcpConnections.take(thing)->deleteLater();
            if (m_monitors.contains(thing)) {
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        }

        // Create the monitor
        NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(thing);
        if (!monitor) {
            qCWarning(dcSungrow()) << "Unable to register monitor with the given params" << thing->params();
            info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("Unable to set up the connection with this configuration, please reconfigure the connection."));
            return;
        }

        m_monitors.insert(thing, monitor);

        connect(info, &ThingSetupInfo::aborted, monitor, [=](){
            // Clean up in case the setup gets aborted
            if (m_monitors.contains(thing)) {
                qCDebug(dcSungrow()) << "Unregister monitor because the setup has been aborted.";
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        });

        QHostAddress address = m_monitors.value(thing)->networkDeviceInfo().address();

        qCInfo(dcSungrow()) << "Setting up Sungrow on" << address.toString();
        auto sungrowConnection = new SungrowModbusTcpConnection(address, m_modbusTcpPort , m_modbusSlaveAddress, this);
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
                // Note: Auto reconnect is disabled explicitly and
                // the device will be connected once the monitor says it is reachable again
                sungrowConnection->disconnectDevice();
            }
        });

        connect(sungrowConnection, &SungrowModbusTcpConnection::reachableChanged, thing, [this, thing, sungrowConnection](bool reachable){
            qCInfo(dcSungrow()) << "Reachable changed to" << reachable << "for" << thing;
            if (reachable) {
                // Connected true will be set after successfull init
                sungrowConnection->initialize();
            } else {
                thing->setStateValue("connected", false);
                thing->setStateValue(sungrowInverterTcpCurrentPowerStateTypeId, 0);

                foreach (Thing *childThing, myThings().filterByParentId(thing->id())) {
                    childThing->setStateValue("connected", false);
                }

                Thing *child = getMeterThing(thing);
                if (child) {
                    child->setStateValue(sungrowMeterCurrentPowerStateTypeId, 0);
                    child->setStateValue(sungrowMeterCurrentPhaseAStateTypeId, 0);
                    child->setStateValue(sungrowMeterCurrentPhaseBStateTypeId, 0);
                    child->setStateValue(sungrowMeterCurrentPhaseCStateTypeId, 0);
                    child->setStateValue(sungrowMeterApparentPowerPhaseAStateTypeId, 0);
                    child->setStateValue(sungrowMeterApparentPowerPhaseBStateTypeId, 0);
                    child->setStateValue(sungrowMeterApparentPowerPhaseCStateTypeId, 0);
                }

                child = getBatteryThing(thing);
                if (child) {
                    child->setStateValue(sungrowBatteryCurrentPowerStateTypeId, 0);
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

            if (myThings().filterByParentId(thing->id()).filterByThingClassId(sungrowMeterThingClassId).isEmpty()) {
                qCDebug(dcSungrow()) << "There is no meter set up for this inverter. Creating a meter for" << thing << sungrowConnection->modbusTcpMaster();
                ThingClass meterThingClass = thingClass(sungrowMeterThingClassId);
                ThingDescriptor descriptor(sungrowMeterThingClassId, meterThingClass.displayName() + " " + sungrowConnection->serialNumber(), QString(), thing->id());
                emit autoThingsAppeared(ThingDescriptors() << descriptor);
            }

            // Check if a battery is connected to this Sungrow inverter
            if (sungrowConnection->batteryType() != SungrowModbusTcpConnection::BatteryTypeNoBattery &&
                    myThings().filterByParentId(thing->id()).filterByThingClassId(sungrowBatteryThingClassId).isEmpty()) {
                qCDebug(dcSungrow()) << "There is a battery connected but not set up yet. Creating a battery.";
                ThingClass batteryThingClass = thingClass(sungrowBatteryThingClassId);
                ThingDescriptor descriptor(sungrowBatteryThingClassId, batteryThingClass.displayName() + " " + sungrowConnection->serialNumber(), QString(), thing->id());
                emit autoThingsAppeared(ThingDescriptors() << descriptor);
            }

            // Update inverter states
            thing->setStateValue(sungrowInverterTcpCurrentPowerStateTypeId, static_cast<double>(sungrowConnection->totalPVPower()) * -1);
            thing->setStateValue(sungrowInverterTcpTemperatureStateTypeId, sungrowConnection->inverterTemperature());
            thing->setStateValue(sungrowInverterTcpFrequencyStateTypeId, sungrowConnection->gridFrequency());
            thing->setStateValue(sungrowInverterTcpTotalEnergyProducedStateTypeId, sungrowConnection->totalPVGeneration());

            // Update the meter if available
            Thing *meterThing = getMeterThing(thing);
            if (meterThing) {
                quint16 runningState = sungrowConnection->runningState();
                qCDebug(dcSungrow()) << "Power generated from PV:" << (runningState & (0x1 << 0) ? "true" : "false");
                qCDebug(dcSungrow()) << "Battery charging:" << (runningState & (0x1 << 1) ? "true" : "false");
                qCDebug(dcSungrow()) << "Battery discharging:" << (runningState & (0x1 << 2) ? "true" : "false");
                qCDebug(dcSungrow()) << "Positive load power:" << (runningState & (0x1 << 3) ? "true" : "false");
                qCDebug(dcSungrow()) << "Feed-in power:" << (runningState & (0x1 << 4) ? "true" : "false");
                qCDebug(dcSungrow()) << "Import power from grid:" << (runningState & (0x1 << 5) ? "true" : "false");
                qCDebug(dcSungrow()) << "Negative load power:" << (runningState & (0x1 << 7) ? "true" : "false");
                meterThing->setStateValue(sungrowMeterCurrentPowerStateTypeId, sungrowConnection->totalActivePower() * -1);
                meterThing->setStateValue(sungrowMeterTotalEnergyConsumedStateTypeId, sungrowConnection->totalImportEnergy());
                meterThing->setStateValue(sungrowMeterTotalEnergyProducedStateTypeId, sungrowConnection->totalExportEnergy());
                meterThing->setStateValue(sungrowMeterCurrentPhaseAStateTypeId, sungrowConnection->phaseACurrent() * -1);
                meterThing->setStateValue(sungrowMeterCurrentPhaseBStateTypeId, sungrowConnection->phaseBCurrent() * -1);
                meterThing->setStateValue(sungrowMeterCurrentPhaseCStateTypeId, sungrowConnection->phaseCCurrent() * -1);
                meterThing->setStateValue(sungrowMeterVoltagePhaseAStateTypeId, sungrowConnection->phaseAVoltage());
                meterThing->setStateValue(sungrowMeterVoltagePhaseBStateTypeId, sungrowConnection->phaseBVoltage());
                meterThing->setStateValue(sungrowMeterVoltagePhaseCStateTypeId, sungrowConnection->phaseCVoltage());
                meterThing->setStateValue(sungrowMeterApparentPowerPhaseAStateTypeId, sungrowConnection->phaseAVoltage() * sungrowConnection->phaseACurrent() * -1);
                meterThing->setStateValue(sungrowMeterApparentPowerPhaseBStateTypeId, sungrowConnection->phaseBVoltage() * sungrowConnection->phaseBCurrent() * -1);
                meterThing->setStateValue(sungrowMeterApparentPowerPhaseCStateTypeId, sungrowConnection->phaseCVoltage() * sungrowConnection->phaseCCurrent() * -1);
                meterThing->setStateValue(sungrowMeterFrequencyStateTypeId, sungrowConnection->gridFrequency());
            }

            // Update the battery if available
            Thing *batteryThing = getBatteryThing(thing);
            if (batteryThing) {
                batteryThing->setStateValue(sungrowBatteryVoltageStateTypeId, sungrowConnection->batteryVoltage());
                batteryThing->setStateValue(sungrowBatteryTemperatureStateTypeId, sungrowConnection->batteryTemperature());
                batteryThing->setStateValue(sungrowBatteryBatteryLevelStateTypeId, sungrowConnection->batteryLevel());
                batteryThing->setStateValue(sungrowBatteryBatteryCriticalStateTypeId, sungrowConnection->batteryLevel() < 5);

                // Note: since firmware 2024 this is a int16 value, and we can use the value directly without convertion
                if (sungrowConnection->batteryPower() < 0) {
                    batteryThing->setStateValue(sungrowBatteryCurrentPowerStateTypeId, sungrowConnection->batteryPower());
                } else {
                    qint16 batteryPower = (sungrowConnection->runningState() & (0x1 << 1) ? sungrowConnection->batteryPower() : sungrowConnection->batteryPower() * -1);
                    batteryThing->setStateValue(sungrowBatteryCurrentPowerStateTypeId, batteryPower);
                }

                quint16 runningState = sungrowConnection->runningState();
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
                foreach(auto thing, myThings().filterByThingClassId(sungrowInverterTcpThingClassId)) {
                    auto monitor = m_monitors.value(thing);
                    if (!monitor->reachable()) {
                        continue;
                    }

                    auto connection = m_tcpConnections.value(thing);
                    if (connection->initializing()) {
                        qCDebug(dcSungrow()) << "Skip updating" << connection->modbusTcpMaster() << "since the connection is still initializing.";
                        continue;
                    }
                    if (connection->reachable()) {
                        qCDebug(dcSungrow()) << "Updating connection" << connection->modbusTcpMaster()->hostAddress().toString();
                        connection->update();
                    } else {
                        qCDebug(dcSungrow()) << "Device not reachable. Probably a TCP connection error. Reconnecting TCP socket";
                        connection->reconnectDevice();
                    }
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

Thing *IntegrationPluginSungrow::getMeterThing(Thing *parentThing)
{
    Things meterThings = myThings().filterByParentId(parentThing->id()).filterByThingClassId(sungrowMeterThingClassId);
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
