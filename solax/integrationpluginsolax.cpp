/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2023, nymea GmbH
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

#include "integrationpluginsolax.h"
#include "plugininfo.h"
#include "solaxdiscovery.h"

#include <network/networkdevicediscovery.h>
#include <hardwaremanager.h>

IntegrationPluginSolax::IntegrationPluginSolax()
{

}

void IntegrationPluginSolax::discoverThings(ThingDiscoveryInfo *info)
{
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcSolax()) << "The network discovery is not available on this platform.";
        info->finish(Thing::ThingErrorUnsupportedFeature, QT_TR_NOOP("The network device discovery is not available."));
        return;
    }

    // Create a discovery with the info as parent for auto deleting the object once the discovery info is done
    SolaxDiscovery *discovery = new SolaxDiscovery(hardwareManager()->networkDeviceDiscovery(), 502, 1, info);
    connect(discovery, &SolaxDiscovery::discoveryFinished, info, [=](){
        foreach (const SolaxDiscovery::SolaxDiscoveryResult &result, discovery->discoveryResults()) {

            QString title;
            if (result.productName.isEmpty()) {
                title = "SolaX Inverter";
            } else {
                title = "SolaX " + result.productName;
            }

            if (!result.serialNumber.isEmpty())
                title.append(" " + result.serialNumber);

            ThingDescriptor descriptor(solaxInverterTcpThingClassId, title, result.networkDeviceInfo.address().toString() + " " + result.networkDeviceInfo.macAddress());
            qCInfo(dcSolax()) << "Discovered:" << descriptor.title() << descriptor.description();

            // Check if we already have set up this device
            Things existingThings = myThings().filterByParam(solaxInverterTcpThingMacAddressParamTypeId, result.networkDeviceInfo.macAddress());
            if (existingThings.count() == 1) {
                qCDebug(dcSolax()) << "This solax inverter already exists in the system:" << result.networkDeviceInfo;
                descriptor.setThingId(existingThings.first()->id());
            }

            ParamList params;
            params << Param(solaxInverterTcpThingMacAddressParamTypeId, result.networkDeviceInfo.macAddress());
            // Note: if we discover also the port and modbusaddress, we must fill them in from the discovery here, for now everywhere the defaults...
            descriptor.setParams(params);
            info->addThingDescriptor(descriptor);
        }

        info->finish(Thing::ThingErrorNoError);
    });

    // Start the discovery process
    discovery->startDiscovery();
}

void IntegrationPluginSolax::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCInfo(dcSolax()) << "Setup" << thing << thing->params();

    // Inverter (connection)
    if (thing->thingClassId() == solaxInverterTcpThingClassId) {

        // Handle reconfigure
        if (m_tcpConnections.contains(thing)) {
            qCDebug(dcSolax()) << "Reconfiguring existing thing" << thing->name();
            m_tcpConnections.take(thing)->deleteLater();
            if (m_monitors.contains(thing)) {
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        }

        MacAddress macAddress = MacAddress(thing->paramValue(solaxInverterTcpThingMacAddressParamTypeId).toString());
        if (!macAddress.isValid()) {
            qCWarning(dcSolax()) << "The configured mac address is not valid" << thing->params();
            info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The MAC address is not known. Please reconfigure the thing."));
            return;
        }

        // Create the monitor
        NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(macAddress);
        m_monitors.insert(thing, monitor);
        connect(info, &ThingSetupInfo::aborted, monitor, [=](){
            // Clean up in case the setup gets aborted
            if (m_monitors.contains(thing)) {
                qCDebug(dcSolax()) << "Unregister monitor because the setup has been aborted.";
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        });

        QHostAddress address = m_monitors.value(thing)->networkDeviceInfo().address();
        uint port = thing->paramValue(solaxInverterTcpThingPortParamTypeId).toUInt();
        quint16 slaveId = thing->paramValue(solaxInverterTcpThingSlaveIdParamTypeId).toUInt();

        qCInfo(dcSolax()) << "Setting up solax on" << address.toString() << port << "unit ID:" << slaveId;
        SolaxModbusTcpConnection *solaxConnection = new SolaxModbusTcpConnection(address, port, slaveId, this);
        connect(info, &ThingSetupInfo::aborted, solaxConnection, &SolaxModbusTcpConnection::deleteLater);

        // Reconnect on monitor reachable changed
        connect(monitor, &NetworkDeviceMonitor::reachableChanged, thing, [=](bool reachable){
            qCDebug(dcSolax()) << "Network device monitor reachable changed for" << thing->name() << reachable;
            if (!thing->setupComplete())
                return;

            if (reachable && !thing->stateValue("connected").toBool()) {
                solaxConnection->modbusTcpMaster()->setHostAddress(monitor->networkDeviceInfo().address());
                solaxConnection->reconnectDevice();
            } else if (!reachable) {
                // Note: We disable autoreconnect explicitly and we will
                // connect the device once the monitor says it is reachable again
                solaxConnection->disconnectDevice();
            }
        });

        connect(solaxConnection, &SolaxModbusTcpConnection::reachableChanged, thing, [this, thing, solaxConnection](bool reachable){
            qCDebug(dcSolax()) << "Reachable changed to" << reachable << "for" << thing;
            if (reachable) {
                // Connected true will be set after successfull init
                solaxConnection->initialize();
            } else {
                thing->setStateValue("connected", false);
                foreach (Thing *childThing, myThings().filterByParentId(thing->id())) {
                    childThing->setStateValue("connected", false);
                }

                // Reset any energy data due to connection loss
                Thing *child = getMeterThing(thing, 1);
                if (child) {
                    child->setStateValue(solaxMeterCurrentPowerStateTypeId, 0);
                }

                child = getMeterThing(thing, 2);
                if (child) {
                    child->setStateValue(solaxMeterCurrentPowerStateTypeId, 0);
                    child->setStateValue(solaxMeterCurrentPowerPhaseAStateTypeId, 0);
                    child->setStateValue(solaxMeterCurrentPowerPhaseBStateTypeId, 0);
                    child->setStateValue(solaxMeterCurrentPowerPhaseCStateTypeId, 0);
                }

                Things inverterThings = myThings().filterByParentId(thing->id()).filterByThingClassId(solaxInverterChildThingClassId);
                if (!inverterThings.isEmpty()) {
                    child = inverterThings.first();
                    child->setSettingValue(solaxInverterChildCurrentPowerStateTypeId, 0);
                }

                child = getBatteryThing(thing);
                if (child) {
                    child->setStateValue(solaxBatteryVoltageStateTypeId, solaxConnection->batteryVoltage());
                    child->setStateValue(solaxBatteryCurrentPowerStateTypeId, 0);
                    child->setStateValue(solaxBatteryChargingStateStateTypeId, "idle");
                }
            }
        });

        connect(solaxConnection, &SolaxModbusTcpConnection::initializationFinished, thing, [=](bool success){
            thing->setStateValue("connected", success);

            foreach (Thing *childThing, myThings().filterByParentId(thing->id())) {
                childThing->setStateValue("connected", success);
            }

            if (!success) {
                // Try once to reconnect the device
                solaxConnection->reconnectDevice();
            } else {
                // Start update cycle
                solaxConnection->update();
            }
        });

        connect(solaxConnection, &SolaxModbusTcpConnection::updateFinished, thing, [=](){
            qCDebug(dcSolax()) << "Updated" << solaxConnection;

            ThingClass meterThingClass = thingClass(solaxMeterThingClassId);

            // Check if we have to create the meter for this solax inverter, or remove it due to communication errors
            if (myThings().filterByParentId(thing->id()).filterByThingClassId(solaxMeterThingClassId).isEmpty()) {
                if (solaxConnection->meter1ComState() == 1 && myThings().filterByParentId(thing->id()).filterByThingClassId(solaxMeterThingClassId).filterByParam(solaxMeterThingIdParamTypeId, 1).isEmpty()) {
                    qCDebug(dcSolax()) << "There is no meter set up for this inverter. Creating a meter 1 for" << thing << solaxConnection->modbusTcpMaster();
                    ThingDescriptor descriptor(solaxMeterThingClassId, meterThingClass.displayName(), QString(), thing->id());
                    ParamList params;
                    params.append(Param(solaxMeterThingIdParamTypeId, 1));
                    descriptor.setParams(params);
                    emit autoThingsAppeared(ThingDescriptors() << descriptor);
                }
            }

            // Note: it is possible to connect an additional inverter to the solax inverter, which will be measured by the meter 2.
            //       if so, we create a child inverter instead of the second meter thing.
            if (solaxConnection->meter2ComState() == 1) {
                if (thing->setting(solaxInverterTcpSettingsMeter2InverterParamTypeId).toBool()) {
                    if (myThings().filterByParentId(thing->id()).filterByThingClassId(solaxInverterChildThingClassId).isEmpty()) {
                        // Add the meter 2 as child inverter
                        emit autoThingsAppeared(ThingDescriptors() << ThingDescriptor(solaxInverterChildThingClassId, "SolaX inverter", QString(), thing->id()));
                    }
                } else {
                    if (myThings().filterByParentId(thing->id()).filterByThingClassId(solaxMeterThingClassId).filterByParam(solaxMeterThingIdParamTypeId, 2).isEmpty()) {
                        ThingDescriptor descriptor(solaxMeterThingClassId, meterThingClass.displayName() + " 2", QString(), thing->id());
                        ParamList params;
                        params.append(Param(solaxMeterThingIdParamTypeId, 2));
                        descriptor.setParams(params);
                        emit autoThingsAppeared(ThingDescriptors() << descriptor);
                    }
                }
            } else {
                // Communication error with meter 2, remove any child devices for meter 2 registers
                cleanupMeter2(thing);
                cleanupChildInverters(thing);
            }

            // Check if we have to create the battery for the solax inverter
            if (solaxConnection->batteryConnected() != 0 && myThings().filterByParentId(thing->id()).filterByThingClassId(solaxBatteryThingClassId).isEmpty()) {
                qCDebug(dcSolax()) << "There is a battery connected but not set up yet. Creating a battery...";
                ThingClass batteryThingClass = thingClass(solaxBatteryThingClassId);
                ThingDescriptor descriptor(solaxBatteryThingClassId, batteryThingClass.displayName(), QString(), thing->id());
                emit autoThingsAppeared(ThingDescriptors() << descriptor);
            }

            // Update inverter states
            thing->setStateValue(solaxInverterTcpCurrentPowerStateTypeId, -solaxConnection->inverterPower());
            thing->setStateValue(solaxInverterTcpCurrentStateTypeId, -solaxConnection->inverterCurrent());
            thing->setStateValue(solaxInverterTcpCurrentVoltageStateTypeId, solaxConnection->inverterVoltage());
            thing->setStateValue(solaxInverterTcpTemperatureStateTypeId, solaxConnection->temperature());
            thing->setStateValue(solaxInverterTcpFrequencyStateTypeId, solaxConnection->inverterFrequency());
            thing->setStateValue(solaxInverterTcpTotalEnergyProducedStateTypeId, solaxConnection->totalEnergyProduced());

            // Update the meter 1 if available
            Thing *meterThing = getMeterThing(thing, 1);
            if (meterThing) {
                meterThing->setStateValue(solaxMeterTotalEnergyConsumedStateTypeId, solaxConnection->meterTotalEnergyConsumend());
                meterThing->setStateValue(solaxMeterTotalEnergyProducedStateTypeId, solaxConnection->meterTotalEnergyProduced());

                // Power
                meterThing->setStateValue(solaxMeterCurrentPowerStateTypeId, -solaxConnection->meterPower());
            }

            // Update inverter 2 states if available
            if (thing->setting(solaxInverterTcpSettingsMeter2InverterParamTypeId).toBool()) {
                Things childInverters = myThings().filterByParentId(thing->id()).filterByThingClassId(solaxInverterChildThingClassId);
                if (!childInverters.isEmpty()) {
                    Thing *childInverter = childInverters.first();
                    childInverter->setStateValue(solaxInverterChildCurrentPowerStateTypeId, -solaxConnection->meter2Power());
                    childInverter->setStateValue(solaxInverterChildTotalEnergyProducedStateTypeId, solaxConnection->meter2EnergyProduced());
                }
            } else {
                // Update the meter 2 if available
                meterThing = getMeterThing(thing, 2);
                if (meterThing) {
                    meterThing->setStateValue(solaxMeterTotalEnergyConsumedStateTypeId, solaxConnection->meter2EnergyConsumed());
                    meterThing->setStateValue(solaxMeterTotalEnergyProducedStateTypeId, solaxConnection->meter2EnergyProduced());

                    // Power
                    meterThing->setStateValue(solaxMeterCurrentPowerStateTypeId, solaxConnection->meter2Power());
                    meterThing->setStateValue(solaxMeterCurrentPowerPhaseAStateTypeId, solaxConnection->meter2PowerR());
                    meterThing->setStateValue(solaxMeterCurrentPowerPhaseBStateTypeId, solaxConnection->meter2PowerS());
                    meterThing->setStateValue(solaxMeterCurrentPowerPhaseCStateTypeId, solaxConnection->meter2PowerT());
                }
            }

            // Update the battery if available
            Thing *batteryThing = getBatteryThing(thing);
            if (batteryThing) {
                batteryThing->setStateValue(solaxBatteryVoltageStateTypeId, solaxConnection->batteryVoltage());
                batteryThing->setStateValue(solaxBatteryTemperatureStateTypeId, solaxConnection->batteryTemperature());
                batteryThing->setStateValue(solaxBatteryBatteryLevelStateTypeId, solaxConnection->batteryCapacity());
                batteryThing->setStateValue(solaxBatteryBatteryCriticalStateTypeId, solaxConnection->batteryCapacity() < 5);

                double batteryPower = solaxConnection->batteryPower();
                batteryThing->setStateValue(solaxBatteryCurrentPowerStateTypeId, solaxConnection->batteryPower());
                if (batteryPower == 0) {
                    batteryThing->setStateValue(solaxBatteryChargingStateStateTypeId, "idle");
                } else if (batteryPower < 0) {
                    batteryThing->setStateValue(solaxBatteryChargingStateStateTypeId, "discharging");
                } else if (batteryPower > 0) {
                    batteryThing->setStateValue(solaxBatteryChargingStateStateTypeId, "charging");
                }
            }

            // Run the next update cycle
            solaxConnection->update();
        });

        connect(thing, &Thing::settingChanged, solaxConnection, [this, thing](const ParamTypeId &paramTypeId, const QVariant &value){
            if (paramTypeId == solaxInverterTcpSettingsMeter2InverterParamTypeId) {
                // Note: we just need to cleanup here, if there is any device connected, it will be created in the update method.
                if (value.toBool()) {
                    // The meter will be used as inverter. Clean up any meters for meter 2 registers...
                    cleanupMeter2(thing);
                } else {
                    // The meter will be used as meter.  Clean up any child inverters for meter 2 registers...
                    cleanupChildInverters(thing);
                }
            }
        });

        m_tcpConnections.insert(thing, solaxConnection);

        if (monitor->reachable())
            solaxConnection->connectDevice();

        info->finish(Thing::ThingErrorNoError);
        return;
    }

    // Meter
    if (thing->thingClassId() == solaxMeterThingClassId) {

        // Get the parent thing and the associated connection
        Thing *connectionThing = myThings().findById(thing->parentId());
        if (!connectionThing) {
            qCWarning(dcSolax()) << "Failed to set up solax energy meter because the parent thing with ID" << thing->parentId().toString() << "could not be found.";
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        SolaxModbusTcpConnection *solaxConnection = m_tcpConnections.value(connectionThing);
        if (!solaxConnection) {
            qCWarning(dcSolax()) << "Failed to set up solax energy meter because the connection for" << connectionThing << "does not exist.";
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        // Note: The states will be handled in the parent inverter thing on updated

        info->finish(Thing::ThingErrorNoError);
        return;
    }

    // Child inverter
    if (thing->thingClassId() == solaxInverterChildThingClassId) {

        // Get the parent thing and the associated connection
        Thing *connectionThing = myThings().findById(thing->parentId());
        if (!connectionThing) {
            qCWarning(dcSolax()) << "Failed to set up solax child inverter because the parent thing with ID" << thing->parentId().toString() << "could not be found.";
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        SolaxModbusTcpConnection *solaxConnection = m_tcpConnections.value(connectionThing);
        if (!solaxConnection) {
            qCWarning(dcSolax()) << "Failed to set up solax child inverter because the connection for" << connectionThing << "does not exist.";
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        // Note: The states will be handled in the parent inverter thing on updated

        info->finish(Thing::ThingErrorNoError);
        return;
    }

    // Battery
    if (thing->thingClassId() == solaxBatteryThingClassId) {
        // Get the parent thing and the associated connection
        Thing *connectionThing = myThings().findById(thing->parentId());
        if (!connectionThing) {
            qCWarning(dcSolax()) << "Failed to set up solax battery because the parent thing with ID" << thing->parentId().toString() << "could not be found.";
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        SolaxModbusTcpConnection *solaxConnection = m_tcpConnections.value(connectionThing);
        if (!solaxConnection) {
            qCWarning(dcSolax()) << "Failed to set up solax battery because the connection for" << connectionThing << "does not exist.";
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        // Note: The states will be handled in the parent inverter thing on updated

        info->finish(Thing::ThingErrorNoError);
        return;
    }
}

void IntegrationPluginSolax::postSetupThing(Thing *thing)
{
    if (thing->thingClassId() == solaxMeterThingClassId || thing->thingClassId() == solaxBatteryThingClassId || thing->thingClassId() == solaxInverterChildThingClassId) {
        Thing *connectionThing = myThings().findById(thing->parentId());
        if (connectionThing) {
            thing->setStateValue("connected", connectionThing->stateValue("connected"));
        }

        return;
    }
}

void IntegrationPluginSolax::thingRemoved(Thing *thing)
{
    if (thing->thingClassId() == solaxInverterTcpThingClassId && m_tcpConnections.contains(thing)) {
        SolaxModbusTcpConnection *connection = m_tcpConnections.take(thing);
        connection->modbusTcpMaster()->disconnectDevice();
        delete connection;
    }

    // Unregister related hardware resources
    if (m_monitors.contains(thing))
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));

}

Thing *IntegrationPluginSolax::getMeterThing(Thing *parentThing, uint meterId)
{
    Things meterThings = myThings().filterByParentId(parentThing->id()).filterByThingClassId(solaxMeterThingClassId).filterByParam(solaxMeterThingIdParamTypeId, meterId);
    if (meterThings.isEmpty())
        return nullptr;

    return meterThings.first();
}

Thing *IntegrationPluginSolax::getBatteryThing(Thing *parentThing)
{
    Things batteryThings = myThings().filterByParentId(parentThing->id()).filterByThingClassId(solaxBatteryThingClassId);
    if (batteryThings.isEmpty())
        return nullptr;

    return batteryThings.first();
}

void IntegrationPluginSolax::cleanupMeter2(Thing *parentThing)
{
    Things meters = myThings().filterByParentId(parentThing->id()).filterByThingClassId(solaxMeterThingClassId).filterByParam(solaxMeterThingIdParamTypeId, 2);
    if (!meters.isEmpty()) {
        emit autoThingDisappeared(meters.first()->id());
    }
}

void IntegrationPluginSolax::cleanupChildInverters(Thing *parentThing)
{
    Things inverterThings = myThings().filterByParentId(parentThing->id()).filterByThingClassId(solaxInverterChildThingClassId);
    if (!inverterThings.isEmpty()) {
        foreach (Thing *inverterThing, inverterThings) {
            emit autoThingDisappeared(inverterThing->id());
        }
    }
}

