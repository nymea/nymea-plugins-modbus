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

#include <network/networkdevicediscovery.h>
#include <network/networkdevicemonitor.h>

#include "integrationpluginmtec.h"
#include "plugininfo.h"

IntegrationPluginMTec::IntegrationPluginMTec()
{

}

void IntegrationPluginMTec::discoverThings(ThingDiscoveryInfo *info)
{
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcMTec()) << "The network discovery is not available on this platform.";
        info->finish(Thing::ThingErrorUnsupportedFeature, QT_TR_NOOP("The network device discovery is not available."));
        return;
    }

    // Perform a network device discovery and filter for "go-eCharger" hosts
    NetworkDeviceDiscoveryReply *discoveryReply = hardwareManager()->networkDeviceDiscovery()->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {

            qCDebug(dcMTec()) << "Found" << networkDeviceInfo;

            QString title;
            QString description;
            MacAddressInfo macInfo;

            switch (networkDeviceInfo.monitorMode()) {
            case NetworkDeviceInfo::MonitorModeMac:
                macInfo = networkDeviceInfo.macAddressInfos().constFirst();
                description = networkDeviceInfo.address().toString();
                if (!macInfo.vendorName().isEmpty())
                    description += " - " + networkDeviceInfo.macAddressInfos().constFirst().vendorName();

                if (networkDeviceInfo.hostName().isEmpty()) {
                    title = macInfo.macAddress().toString();
                } else {
                    title = networkDeviceInfo.hostName() + " (" + macInfo.macAddress().toString() + ")";
                }

                break;
            case NetworkDeviceInfo::MonitorModeHostName:
                title = networkDeviceInfo.hostName();
                description = networkDeviceInfo.address().toString();
                break;
            case NetworkDeviceInfo::MonitorModeIp:
                title = "Network device " + networkDeviceInfo.address().toString();
                description = "Interface: " + networkDeviceInfo.networkInterface().name();
                break;
            }

            ThingDescriptor descriptor(mtecThingClassId, title, description);

            ParamList params;
            params << Param(mtecThingMacAddressParamTypeId, networkDeviceInfo.thingParamValueMacAddress());
            params << Param(mtecThingAddressParamTypeId, networkDeviceInfo.thingParamValueAddress());
            params << Param(mtecThingHostNameParamTypeId, networkDeviceInfo.thingParamValueHostName());
            descriptor.setParams(params);

            // Check if we already have set up this device
            Thing *existingThing = myThings().findByParams(params);
            if (existingThing) {
                qCDebug(dcMTec()) << "This heat pump already exists in the system:" << networkDeviceInfo;
                descriptor.setThingId(existingThing->id());
            }

            info->addThingDescriptor(descriptor);
        }

        info->finish(Thing::ThingErrorNoError);
    });
}

void IntegrationPluginMTec::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcMTec()) << "Setup" << thing;

    NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(thing);
    if (!monitor) {
        qCWarning(dcMTec()) << "Unable to register monitor with the given params" << thing->params();
        info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("Unable to set up the connection with this configuration, please reconfigure the connection."));
        return;
    }

    qCInfo(dcMTec()) << "Set up MTec modbus connection with" << monitor;
    m_monitors.insert(thing, monitor);
    connect(info, &ThingSetupInfo::aborted, monitor, [this, thing](){
        if (m_monitors.contains(thing)) {
            qCDebug(dcMTec()) << "Unregistering monitor because setup has been aborted.";
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
        }
    });


    QHostAddress address = monitor->networkDeviceInfo().address();
    if (address.isNull()) {
        qCWarning(dcMTec()) << "Cannot set up thing. The host address is not known yet. Maybe it will be available in the next run...";
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
        info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("The host address is not known yet. Trying later again."));
        return;
    }

    MTec *mtec = new MTec(address, this);
    connect(monitor, &NetworkDeviceMonitor::reachableChanged, this, [=](bool reachable ){
        qCDebug(dcMTec()) << "Monitor reachable changed to" << reachable << "for" << thing;
        if (reachable && !thing->stateValue("connected").toBool()) {
            mtec->modbusTcpMaster()->setHostAddress(monitor->networkDeviceInfo().address());
            mtec->connectDevice();
        } else if (!reachable) {
            // Note: We disable autoreconnect explicitly and we will
            // connect the device once the monitor says it is reachable again
            mtec->disconnectDevice();
        }
    });

    connect(mtec, &MTec::connectedChanged, thing, [=](bool connected){
        qCDebug(dcMTec()) << thing << "Connected changed to" << connected;
        thing->setStateValue(mtecConnectedStateTypeId, connected);
    });

    connect(mtec, &MTec::roomTemperatureChanged, thing, [=](double roomTemperature){
        qCDebug(dcMTec()) << thing << "Room temperature" << roomTemperature << "°C";
        thing->setStateValue(mtecTemperatureStateTypeId, roomTemperature);
    });

    connect(mtec, &MTec::targetRoomTemperatureChanged, thing, [=](double targetRoomTemperature){
        qCDebug(dcMTec()) << thing << "Target room temperature" << targetRoomTemperature << "°C";
        thing->setStateValue(mtecTargetTemperatureStateTypeId, targetRoomTemperature);
    });

    connect(mtec, &MTec::waterTankTopTemperatureChanged, thing, [=](double waterTankTopTemperature){
        qCDebug(dcMTec()) << thing << "Water tank top temperature" << waterTankTopTemperature << "°C";
        thing->setStateValue(mtecWaterTankTopTemperatureStateTypeId, waterTankTopTemperature);
    });

    connect(mtec, &MTec::bufferTankTemperatureChanged, thing, [=](double bufferTankTemperature){
        qCDebug(dcMTec()) << thing << "Buffer tank temperature" << bufferTankTemperature << "°C";
        thing->setStateValue(mtecBufferTankTemperatureStateTypeId, bufferTankTemperature);
    });

    connect(mtec, &MTec::totalAccumulatedHeatingEnergyChanged, thing, [=](double totalAccumulatedHeatingEnergy){
        qCDebug(dcMTec()) << thing << "Total accumulated heating energy" << totalAccumulatedHeatingEnergy << "kWh";
        thing->setStateValue(mtecTotalAccumulatedHeatingEnergyStateTypeId, totalAccumulatedHeatingEnergy);
    });

    connect(mtec, &MTec::totalAccumulatedElectricalEnergyChanged, thing, [=](double totalAccumulatedElectricalEnergy){
        qCDebug(dcMTec()) << thing << "Total accumulated electrical energy" << totalAccumulatedElectricalEnergy << "kWh";
        thing->setStateValue(mtecTotalAccumulatedElectricalEnergyStateTypeId, totalAccumulatedElectricalEnergy);
    });

    connect(mtec, &MTec::heatPumpStateChanged, thing, [=](MTec::HeatpumpState heatPumpState){
        qCDebug(dcMTec()) << thing << "Heat pump state" << heatPumpState;
        switch (heatPumpState) {
        case MTec::HeatpumpStateStandby:
            thing->setStateValue(mtecHeatPumpStateStateTypeId, "Standby");
            thing->setStateValue(mtecHeatingOnStateTypeId, false);
            thing->setStateValue(mtecCoolingOnStateTypeId, false);
            break;
        case MTec::HeatpumpStatePreRun:
            thing->setStateValue(mtecHeatPumpStateStateTypeId, "Pre run");
            thing->setStateValue(mtecHeatingOnStateTypeId, false);
            thing->setStateValue(mtecCoolingOnStateTypeId, false);
            break;
        case MTec::HeatpumpStateAutomaticHeat:
            thing->setStateValue(mtecHeatPumpStateStateTypeId, "Automatic heat");
            thing->setStateValue(mtecHeatingOnStateTypeId, true);
            thing->setStateValue(mtecCoolingOnStateTypeId, false);
            break;
        case MTec::HeatpumpStateDefrost:
            thing->setStateValue(mtecHeatPumpStateStateTypeId, "Defrost");
            thing->setStateValue(mtecHeatingOnStateTypeId, false);
            thing->setStateValue(mtecCoolingOnStateTypeId, false);
            break;
        case MTec::HeatpumpStateAutomaticCool:
            thing->setStateValue(mtecHeatPumpStateStateTypeId, "Automatic cool");
            thing->setStateValue(mtecHeatingOnStateTypeId, false);
            thing->setStateValue(mtecCoolingOnStateTypeId, true);
            break;
        case MTec::HeatpumpStatePostRun:
            thing->setStateValue(mtecHeatingOnStateTypeId, false);
            thing->setStateValue(mtecHeatPumpStateStateTypeId, "Post run");
            thing->setStateValue(mtecCoolingOnStateTypeId, false);
            break;
        case MTec::HeatpumpStateSaftyShutdown:
            thing->setStateValue(mtecHeatingOnStateTypeId, false);
            thing->setStateValue(mtecHeatPumpStateStateTypeId, "Safty shutdown");
            thing->setStateValue(mtecCoolingOnStateTypeId, false);
            break;
        case MTec::HeatpumpStateError:
            thing->setStateValue(mtecHeatingOnStateTypeId, false);
            thing->setStateValue(mtecHeatPumpStateStateTypeId, "Error");
            thing->setStateValue(mtecCoolingOnStateTypeId, false);
            break;
        }
    });

    connect(mtec, &MTec::heatMeterPowerConsumptionChanged, thing, [=](double heatMeterPowerConsumption){
        qCDebug(dcMTec()) << thing << "Heat meter power consumption" << heatMeterPowerConsumption << "W";
        thing->setStateValue(mtecHeatMeterPowerConsumptionStateTypeId, heatMeterPowerConsumption);
    });

    connect(mtec, &MTec::energyMeterPowerConsumptionChanged, thing, [=](double energyMeterPowerConsumption){
        qCDebug(dcMTec()) << thing << "Energy meter power consumption" << energyMeterPowerConsumption << "W";
        thing->setStateValue(mtecEnergyMeterPowerConsumptionStateTypeId, energyMeterPowerConsumption);
    });

    connect(mtec, &MTec::actualExcessEnergySmartHomeChanged, thing, [=](double actualExcessEnergySmartHome){
        qCDebug(dcMTec()) << thing << "Smart home energy" << actualExcessEnergySmartHome << "W";
        thing->setStateValue(mtecSmartHomeEnergyStateTypeId, actualExcessEnergySmartHome);
    });

    connect(mtec, &MTec::actualExcessEnergySmartHomeElectricityMeterChanged, thing, [=](double actualExcessEnergySmartHomeElectricityMeter){
        qCDebug(dcMTec()) << thing << "Smart home energy electrical meter" << actualExcessEnergySmartHomeElectricityMeter << "W";
        thing->setStateValue(mtecSmartHomeEnergyElectricityMeterStateTypeId, actualExcessEnergySmartHomeElectricityMeter);
    });

    connect(mtec, &MTec::actualOutdoorTemperatureChanged, thing, [=](double actualOutdoorTemperature){
        qCDebug(dcMTec()) << thing << "Outdoor temperature" << actualOutdoorTemperature << "°C";
        thing->setStateValue(mtecOutdoorTemperatureStateTypeId, actualOutdoorTemperature);
    });

    m_mtecConnections.insert(thing, mtec);

    // TODO: start timer and give 15 seconds until connected, since the controler is down for ~10 seconds after a disconnect

    if (!mtec->connectDevice()) {
        qCWarning(dcMTec()) << "Initial connect returned false. Lets wait 15 seconds until the connection can be established.";
    }

    info->finish(Thing::ThingErrorNoError);
}

void IntegrationPluginMTec::postSetupThing(Thing *thing)
{
    if (m_mtecConnections.contains(thing))
        update(thing);

    if (!m_pluginTimer) {
        qCDebug(dcMTec()) << "Starting plugin timer...";
        m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(10);
        connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
            foreach (Thing *thing, myThings().filterByThingClassId(mtecThingClassId)) {
                update(thing);
            }
        });
    }
}

void IntegrationPluginMTec::thingRemoved(Thing *thing)
{
    if (m_mtecConnections.contains(thing)) {
        MTec *mtec = m_mtecConnections.take(thing);
        if (mtec) {
            mtec->disconnectDevice();
            mtec->deleteLater();
        }
    }

    if (m_monitors.contains(thing))
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));

    if (myThings().isEmpty()) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginMTec::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    MTec *mtec = m_mtecConnections.value(thing);
    if (!mtec) {
        qCWarning(dcMTec()) << "Could not execute action because the MTec connection could not be found for" << thing;
        info->finish(Thing::ThingErrorHardwareNotAvailable);
        return;
    }

    // Make sure we are connected
    if (!mtec->connected()) {
        qCWarning(dcMTec()) << "Could not execute action because the MTec connection is not connected" << thing;
        info->finish(Thing::ThingErrorHardwareNotAvailable);
        return;
    }

    if (action.actionTypeId() == mtecTargetTemperatureActionTypeId) {
        double targetTemperature = action.paramValue(mtecTargetTemperatureActionTargetTemperatureParamTypeId).toDouble();
        qCDebug(dcMTec()) << "Setting target temperature" << targetTemperature << "°C";
        QModbusReply *reply = mtec->setTargetRoomTemperature(targetTemperature);
        if (!reply) {
            qCWarning(dcMTec()) << "Failed to send modbus request" << thing;
            info->finish(Thing::ThingErrorHardwareFailure);
            return;
        }

        connect(reply, &QModbusReply::finished, this, [=]() {
            reply->deleteLater();
            if (reply->error() == QModbusDevice::NoError) {
                qCDebug(dcMTec()) << "Setting target temperature" << targetTemperature << "°C" << "finished successfully";
                thing->setStateValue(mtecTargetTemperatureStateTypeId, targetTemperature);
                info->finish(Thing::ThingErrorNoError);
            } else {
                info->finish(Thing::ThingErrorHardwareFailure);
            }
        });

        connect(reply, &QModbusReply::errorOccurred, this, [=](QModbusDevice::Error error) {
            qCWarning(dcMTec()) << thing << "Action execution finished due to modbus replay error:" << error;
            reply->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure);
        });
    } else if (action.actionTypeId() == mtecSmartHomeEnergyActionTypeId) {
        quint16 energy = action.paramValue(mtecSmartHomeEnergyActionSmartHomeEnergyParamTypeId).toUInt();
        qCDebug(dcMTec()) << "Setting smart home energy to" << energy << "W";
        QModbusReply *reply = mtec->setSmartHomeEnergy(energy);
        if (!reply) {
            qCWarning(dcMTec()) << "Failed to send modbus request" << thing;
            info->finish(Thing::ThingErrorHardwareFailure);
            return;
        }

        connect(reply, &QModbusReply::finished, this, [=]() {
            reply->deleteLater();
            if (reply->error() == QModbusDevice::NoError) {
                qCDebug(dcMTec()) << "Setting smart home energy" << energy << "W" << "finished successfully";
                thing->setStateValue(mtecSmartHomeEnergyStateTypeId, energy);
                info->finish(Thing::ThingErrorNoError);
            } else {
                info->finish(Thing::ThingErrorHardwareFailure);
            }
        });

        connect(reply, &QModbusReply::errorOccurred, this, [=](QModbusDevice::Error error) {
            qCWarning(dcMTec()) << thing << "Action execution finished due to modbus replay error:" << error;
            reply->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure);
        });
    } else {
        Q_ASSERT_X(false, "executeAction", QString("Unhandled action: %1").arg(action.actionTypeId().toString()).toUtf8());
    }
}

void IntegrationPluginMTec::update(Thing *thing)
{
    if (thing->thingClassId() == mtecThingClassId) {
        qCDebug(dcMTec()) << "Updating thing" << thing;
        MTec *mtec = m_mtecConnections.value(thing);
        if (!mtec) return;
        mtec->updateValues();
    }
}



