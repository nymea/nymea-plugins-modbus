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

#include "network/networkdevicediscovery.h"
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
        foreach (const NetworkDevice &networkDevice, discoveryReply->networkDevices()) {

            qCDebug(dcMTec()) << "Found" << networkDevice;

            QString title;
            if (networkDevice.hostName().isEmpty()) {
                title = networkDevice.address().toString();
            } else {
                title = networkDevice.hostName() + " (" + networkDevice.address().toString() + ")";
            }

            QString description;
            if (networkDevice.macAddressManufacturer().isEmpty()) {
                description = networkDevice.macAddress();
            } else {
                description = networkDevice.macAddress() + " (" + networkDevice.macAddressManufacturer() + ")";
            }

            ThingDescriptor descriptor(mtecThingClassId, title, description);
            ParamList params;
            params << Param(mtecThingIpAddressParamTypeId, networkDevice.address().toString());
            params << Param(mtecThingMacAddressParamTypeId, networkDevice.macAddress());
            descriptor.setParams(params);

            // Check if we already have set up this device
            Things existingThings = myThings().filterByParam(mtecThingMacAddressParamTypeId, networkDevice.macAddress());
            if (existingThings.count() == 1) {
                qCDebug(dcMTec()) << "This heat pump already exists in the system!" << networkDevice;
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

        connect(mtec, &MTec::waterTankTemperatureChanged, thing, [=](double waterTankTemperature){
            qCDebug(dcMTec()) << thing << "Water tank temperature" << waterTankTemperature << "°C";
            thing->setStateValue(mtecWaterTankTemperatureStateTypeId, waterTankTemperature);
        });

        connect(mtec, &MTec::bufferTankTemperatureChanged, thing, [=](double bufferTankTemperature){
            qCDebug(dcMTec()) << thing << "Buffer tank temperature" << bufferTankTemperature << "°C";
            thing->setStateValue(mtecBufferTankTemperatureStateTypeId, bufferTankTemperature);
        });

        connect(mtec, &MTec::totalAccumulatedElectricalEnergyChanged, thing, [=](double totalAccumulatedElectricalEnergy){
            qCDebug(dcMTec()) << thing << "Total accumulated energy" << totalAccumulatedElectricalEnergy << "kWh";
            thing->setStateValue(mtecTotalAccumulatedElectricalEnergyStateTypeId, totalAccumulatedElectricalEnergy);
        });

        connect(mtec, &MTec::heatPumpStateChanged, thing, [=](MTec::HeatpumpState heatPumpState){
            qCDebug(dcMTec()) << thing << "Heat pump state" << heatPumpState;
            switch (heatPumpState) {
            case MTec::HeatpumpStateStandby:
                thing->setStateValue(mtecHeatPumpStateStateTypeId, "Standby");
                break;
            case MTec::HeatpumpStatePreRun:
                thing->setStateValue(mtecHeatPumpStateStateTypeId, "Pre run");
                break;
            case MTec::HeatpumpStateAutomaticHeat:
                thing->setStateValue(mtecHeatPumpStateStateTypeId, "Automatic heat");
                break;
            case MTec::HeatpumpStateDefrost:
                thing->setStateValue(mtecHeatPumpStateStateTypeId, "Defrost");
                break;
            case MTec::HeatpumpStateAutomaticCool:
                thing->setStateValue(mtecHeatPumpStateStateTypeId, "Automatic cool");
                break;
            case MTec::HeatpumpStatePostRun:
                thing->setStateValue(mtecHeatPumpStateStateTypeId, "Post run");
                break;
            case MTec::HeatpumpStateSaftyShutdown:
                thing->setStateValue(mtecHeatPumpStateStateTypeId, "Safty shutdown");
                break;
            case MTec::HeatpumpStateError:
                thing->setStateValue(mtecHeatPumpStateStateTypeId, "Error");
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
//    Thing *thing = info->thing();
//    Action action = info->action();
    info->finish(Thing::ThingErrorNoError);
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



