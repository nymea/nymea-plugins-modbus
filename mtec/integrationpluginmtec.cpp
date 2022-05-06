/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2021, nymea GmbH
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

#include <network/networkdevicediscovery.h>

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
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {

            qCDebug(dcMTec()) << "Found" << networkDeviceInfo;

            QString title;
            if (networkDeviceInfo.hostName().isEmpty()) {
                title = networkDeviceInfo.address().toString();
            } else {
                title = networkDeviceInfo.hostName() + " (" + networkDeviceInfo.address().toString() + ")";
            }

            QString description;
            if (networkDeviceInfo.macAddressManufacturer().isEmpty()) {
                description = networkDeviceInfo.macAddress();
            } else {
                description = networkDeviceInfo.macAddress() + " (" + networkDeviceInfo.macAddressManufacturer() + ")";
            }

            ThingDescriptor descriptor(mtecThingClassId, title, description);
            ParamList params;
            params << Param(mtecThingIpAddressParamTypeId, networkDeviceInfo.address().toString());
            params << Param(mtecThingMacAddressParamTypeId, networkDeviceInfo.macAddress());
            descriptor.setParams(params);

            // Check if we already have set up this device
            Things existingThings = myThings().filterByParam(mtecThingMacAddressParamTypeId, networkDeviceInfo.macAddress());
            if (existingThings.count() == 1) {
                qCDebug(dcMTec()) << "This heat pump already exists in the system!" << networkDeviceInfo;
                descriptor.setThingId(existingThings.first()->id());
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

    if (thing->thingClassId() == mtecThingClassId) {
        QHostAddress hostAddress = QHostAddress(thing->paramValue(mtecThingIpAddressParamTypeId).toString());
        if (hostAddress.isNull()) {
            info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("No IP address given"));
            return;
        }

        qCDebug(dcMTec()) << "Using ip address" << hostAddress.toString();

        MTec *mtec = new MTec(hostAddress, this);
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
}

void IntegrationPluginMTec::postSetupThing(Thing *thing)
{
    if (thing->thingClassId() == mtecThingClassId) {
        MTec *mtec = m_mtecConnections.value(thing);
        if (mtec) {
            update(thing);
        }

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



