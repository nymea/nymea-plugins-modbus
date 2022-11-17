﻿/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
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

#include "integrationpluginalphainnotec.h"

#include "network/networkdevicediscovery.h"
#include "hardwaremanager.h"
#include "plugininfo.h"

IntegrationPluginAlphaInnotec::IntegrationPluginAlphaInnotec()
{

}

void IntegrationPluginAlphaInnotec::discoverThings(ThingDiscoveryInfo *info)
{
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcAlphaInnotec()) << "The network discovery is not available on this platform.";
        info->finish(Thing::ThingErrorUnsupportedFeature, QT_TR_NOOP("The network device discovery is not available."));
        return;
    }

    NetworkDeviceDiscoveryReply *discoveryReply = hardwareManager()->networkDeviceDiscovery()->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {

            qCDebug(dcAlphaInnotec()) << "Found" << networkDeviceInfo;

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

            ThingDescriptor descriptor(alphaConnectThingClassId, title, description);
            ParamList params;
            params << Param(alphaConnectThingIpAddressParamTypeId, networkDeviceInfo.address().toString());
            params << Param(alphaConnectThingMacAddressParamTypeId, networkDeviceInfo.macAddress());
            descriptor.setParams(params);

            // Check if we already have set up this device
            Things existingThings = myThings().filterByParam(alphaConnectThingMacAddressParamTypeId, networkDeviceInfo.macAddress());
            if (existingThings.count() == 1) {
                qCDebug(dcAlphaInnotec()) << "This connection already exists in the system:" << networkDeviceInfo;
                descriptor.setThingId(existingThings.first()->id());
            }

            info->addThingDescriptor(descriptor);
        }

        info->finish(Thing::ThingErrorNoError);
    });
}

void IntegrationPluginAlphaInnotec::startMonitoringAutoThings()
{

}

void IntegrationPluginAlphaInnotec::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcAlphaInnotec()) << "Setup" << thing << thing->params();

    if (thing->thingClassId() == alphaConnectThingClassId) {
        QHostAddress hostAddress = QHostAddress(thing->paramValue(alphaConnectThingIpAddressParamTypeId).toString());
        if (hostAddress.isNull()) {
            info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("No IP address given"));
            return;
        }

        uint port = thing->paramValue(alphaConnectThingPortParamTypeId).toUInt();
        quint16 slaveId = thing->paramValue(alphaConnectThingSlaveIdParamTypeId).toUInt();

        AlphaConnectModbusTcpConnection *alphaConnectTcpConnection = new AlphaConnectModbusTcpConnection(hostAddress, port, slaveId, this);
        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::connectionStateChanged, this, [thing, alphaConnectTcpConnection](bool status){
            qCDebug(dcAlphaInnotec()) << "Connected changed to" << status << "for" << thing;
            if (status) {
                alphaConnectTcpConnection->update();
            }

            thing->setStateValue(alphaConnectConnectedStateTypeId, status);
        });


        // Input registers
//        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::meanTemperatureChanged, this, [thing](float meanTemperature){
//            qCDebug(dcAlphaInnotec()) << thing << "mean temperature changed" << meanTemperature << "°C";
//            thing->setStateValue(alphaConnectMeanTemperatureStateTypeId, meanTemperature);
//        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::flowTemperatureChanged, this, [thing](float flowTemperature){
            qCDebug(dcAlphaInnotec()) << thing << "flow temperature changed" << flowTemperature << "°C";
            thing->setStateValue(alphaConnectFlowTemperatureStateTypeId, flowTemperature);
        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::returnTemperatureChanged, this, [thing](float returnTemperature){
            qCDebug(dcAlphaInnotec()) << thing << "return temperature changed" << returnTemperature << "°C";
            thing->setStateValue(alphaConnectReturnTemperatureStateTypeId, returnTemperature);
        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::externalReturnTemperatureChanged, this, [thing](float externalReturnTemperature){
            qCDebug(dcAlphaInnotec()) << thing << "external return temperature changed" << externalReturnTemperature << "°C";
            thing->setStateValue(alphaConnectExternalReturnTemperatureStateTypeId, externalReturnTemperature);
        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::hotWaterTemperatureChanged, this, [thing](float hotWaterTemperature){
            qCDebug(dcAlphaInnotec()) << thing << "hot water temperature changed" << hotWaterTemperature << "°C";
            thing->setStateValue(alphaConnectHotWaterTemperatureStateTypeId, hotWaterTemperature);
        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::hotGasTemperatureChanged, this, [thing](float hotGasTemperature){
            qCDebug(dcAlphaInnotec()) << thing << "hot gas temperature changed" << hotGasTemperature << "°C";
            thing->setStateValue(alphaConnectHotGasTemperatureStateTypeId, hotGasTemperature);
        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::heatSourceInletTemperatureChanged, this, [thing](float heatSourceInletTemperature){
            qCDebug(dcAlphaInnotec()) << thing << "heat source inlet temperature changed" << heatSourceInletTemperature << "°C";
            thing->setStateValue(alphaConnectHeatSourceInletTemperatureStateTypeId, heatSourceInletTemperature);
        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::heatSourceOutletTemperatureChanged, this, [thing](float heatSourceOutletTemperature){
            qCDebug(dcAlphaInnotec()) << thing << "heat source outlet temperature changed" << heatSourceOutletTemperature << "°C";
            thing->setStateValue(alphaConnectHeatSourceOutletTemperatureStateTypeId, heatSourceOutletTemperature);
        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::roomTemperature1Changed, this, [thing](float roomTemperature1){
            qCDebug(dcAlphaInnotec()) << thing << "room remote adjuster 1 temperature changed" << roomTemperature1 << "°C";
            thing->setStateValue(alphaConnectRoomTemperature1StateTypeId, roomTemperature1);
        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::roomTemperature2Changed, this, [thing](float roomTemperature2){
            qCDebug(dcAlphaInnotec()) << thing << "room remote adjuster 2 temperature changed" << roomTemperature2 << "°C";
            thing->setStateValue(alphaConnectRoomTemperature2StateTypeId, roomTemperature2);
        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::roomTemperature3Changed, this, [thing](float roomTemperature3){
            qCDebug(dcAlphaInnotec()) << thing << "room remote adjuster 3 temperature changed" << roomTemperature3 << "°C";
            thing->setStateValue(alphaConnectRoomTemperature2StateTypeId, roomTemperature3);
        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::solarCollectorTemperatureChanged, this, [thing](float solarCollectorTemperature){
            qCDebug(dcAlphaInnotec()) << thing << "solar collector temperature changed" << solarCollectorTemperature << "°C";
            thing->setStateValue(alphaConnectSolarCollectorTemperatureStateTypeId, solarCollectorTemperature);
        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::solarStorageTankTemperatureChanged, this, [thing](float solarStorageTankTemperature){
            qCDebug(dcAlphaInnotec()) << thing << "solar storage tank temperature changed" << solarStorageTankTemperature << "°C";
            thing->setStateValue(alphaConnectSolarCollectorTemperatureStateTypeId, solarStorageTankTemperature);
        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::externalEnergySourceTemperatureChanged, this, [thing](float externalEnergySourceTemperature){
            qCDebug(dcAlphaInnotec()) << thing << "external energy source temperature changed" << externalEnergySourceTemperature << "°C";
            thing->setStateValue(alphaConnectExternalEnergySourceTemperatureStateTypeId, externalEnergySourceTemperature);
        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::supplyAirTemperatureChanged, this, [thing](float supplyAirTemperature){
            qCDebug(dcAlphaInnotec()) << thing << "supply air temperature changed" << supplyAirTemperature << "°C";
            thing->setStateValue(alphaConnectSupplyAirTemperatureStateTypeId, supplyAirTemperature);
        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::externalAirTemperatureChanged, this, [thing](float externalAirTemperature){
            qCDebug(dcAlphaInnotec()) << thing << "external air temperature changed" << externalAirTemperature << "°C";
            thing->setStateValue(alphaConnectExternalAirTemperatureStateTypeId, externalAirTemperature);
        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::heatingPumpOperatingHoursChanged, this, [thing](quint16 heatingPumpOperatingHours){
            qCDebug(dcAlphaInnotec()) << thing << "heating pump operating hours changed" << heatingPumpOperatingHours;
            thing->setStateValue(alphaConnectHeatingPumpOperatingHoursStateTypeId, heatingPumpOperatingHours);
        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::systemStatusChanged, this, [thing](AlphaConnectModbusTcpConnection::SystemStatus systemStatus){
            qCDebug(dcAlphaInnotec()) << thing << "system status changed" << systemStatus;
            switch (systemStatus) {
            case AlphaConnectModbusTcpConnection::SystemStatusHeatingMode:
                thing->setStateValue(alphaConnectSystemStatusStateTypeId, "Heating mode");
                break;
            case AlphaConnectModbusTcpConnection::SystemStatusDomesticHotWater:
                thing->setStateValue(alphaConnectSystemStatusStateTypeId, "Domestic hot water");
                break;
            case AlphaConnectModbusTcpConnection::SystemStatusSwimmingPool:
                thing->setStateValue(alphaConnectSystemStatusStateTypeId, "Swimming pool");
                break;
            case AlphaConnectModbusTcpConnection::SystemStatusEVUOff:
                thing->setStateValue(alphaConnectSystemStatusStateTypeId, "EUV off");
                break;
            case AlphaConnectModbusTcpConnection::SystemStatusDefrost:
                thing->setStateValue(alphaConnectSystemStatusStateTypeId, "Defrost");
                break;
            case AlphaConnectModbusTcpConnection::SystemStatusOff:
                thing->setStateValue(alphaConnectSystemStatusStateTypeId, "Off");
                break;
            case AlphaConnectModbusTcpConnection::SystemStatusExternalEnergySource:
                thing->setStateValue(alphaConnectSystemStatusStateTypeId, "External energy source");
                break;
            case AlphaConnectModbusTcpConnection::SystemStatusCoolingMode:
                thing->setStateValue(alphaConnectSystemStatusStateTypeId, "Cooling mode");
                break;
            }

            // Set heating and cooling states according to the system state
            thing->setStateValue(alphaConnectHeatingOnStateTypeId, systemStatus == AlphaConnectModbusTcpConnection::SystemStatusHeatingMode);
            thing->setStateValue(alphaConnectCoolingOnStateTypeId, systemStatus == AlphaConnectModbusTcpConnection::SystemStatusCoolingMode);
        });

        // Energy
        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::totalHeatEnergyChanged, this, [thing](float totalHeatEnergy){
            qCDebug(dcAlphaInnotec()) << thing << "total heating energy changed" << totalHeatEnergy << "kWh";
            thing->setStateValue(alphaConnectTotalEnergyStateTypeId, totalHeatEnergy);
        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::heatingEnergyChanged, this, [thing](float heatingEnergy){
            qCDebug(dcAlphaInnotec()) << thing << "heating energy changed" << heatingEnergy << "kWh";
            thing->setStateValue(alphaConnectHeatingEnergyStateTypeId, heatingEnergy);
        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::waterHeatEnergyChanged, this, [thing](float waterHeatEnergy){
            qCDebug(dcAlphaInnotec()) << thing << "water heat energy changed" << waterHeatEnergy << "kWh";
            thing->setStateValue(alphaConnectHotWaterEnergyStateTypeId, waterHeatEnergy);
        });

//        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::swimmingPoolHeatEnergyChanged, this, [thing](float swimmingPoolHeatEnergy){
//            qCDebug(dcAlphaInnotec()) << thing << "swimming pool heat energy changed" << swimmingPoolHeatEnergy << "kWh";
//            thing->setStateValue(alphaConnectSwimmingPoolEnergyStateTypeId, swimmingPoolHeatEnergy);
//        });

        // Holding registers
        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::outdoorTemperatureChanged, this, [thing](float outdoorTemperature){
            qCDebug(dcAlphaInnotec()) << thing << "outdoor temperature changed" << outdoorTemperature << "°C";
            thing->setStateValue(alphaConnectOutdoorTemperatureStateTypeId, outdoorTemperature);
        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::returnSetpointTemperatureChanged, this, [thing](float returnSetpointTemperature){
            qCDebug(dcAlphaInnotec()) << thing << "return setpoint temperature changed" << returnSetpointTemperature << "°C";
            thing->setStateValue(alphaConnectReturnSetpointTemperatureStateTypeId, returnSetpointTemperature);
        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::hotWaterSetpointTemperatureChanged, this, [thing](float hotWaterSetpointTemperature){
            qCDebug(dcAlphaInnotec()) << thing << "hot water setpoint temperature changed" << hotWaterSetpointTemperature << "°C";
            thing->setStateValue(alphaConnectHotWaterSetpointTemperatureStateTypeId, hotWaterSetpointTemperature);
        });

        connect(alphaConnectTcpConnection, &AlphaConnectModbusTcpConnection::smartGridChanged, this, [thing](AlphaConnectModbusTcpConnection::SmartGridState smartGridState){
            qCDebug(dcAlphaInnotec()) << thing << "smart grid state changed" << smartGridState;
            switch (smartGridState) {
            case AlphaConnectModbusTcpConnection::SmartGridStateOff:
                thing->setStateValue(alphaConnectSgReadyModeStateTypeId, "Off");
                break;
            case AlphaConnectModbusTcpConnection::SmartGridStateLow:
                thing->setStateValue(alphaConnectSgReadyModeStateTypeId, "Low");
                break;
            case AlphaConnectModbusTcpConnection::SmartGridStateStandard:
                thing->setStateValue(alphaConnectSgReadyModeStateTypeId, "Standard");
                break;
            case AlphaConnectModbusTcpConnection::SmartGridStateHigh:
                thing->setStateValue(alphaConnectSgReadyModeStateTypeId, "High");
                break;
            }
        });

        m_alpaConnectTcpThings.insert(thing, alphaConnectTcpConnection);
        alphaConnectTcpConnection->connectDevice();

        // FIXME: make async and check if this is really an alpha connect
        info->finish(Thing::ThingErrorNoError);
    }
}

void IntegrationPluginAlphaInnotec::postSetupThing(Thing *thing)
{
    if (thing->thingClassId() == alphaConnectThingClassId) {
        if (!m_pluginTimer) {
            qCDebug(dcAlphaInnotec()) << "Starting plugin timer...";
            m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(10);
            connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
                foreach (AlphaConnectModbusTcpConnection *connection, m_alpaConnectTcpThings) {
                    if (connection->connected()) {
                        connection->update();
                    }
                }
            });

            m_pluginTimer->start();
        }
    }
}

void IntegrationPluginAlphaInnotec::thingRemoved(Thing *thing)
{
    if (thing->thingClassId() == alphaConnectThingClassId && m_alpaConnectTcpThings.contains(thing)) {
        AlphaConnectModbusTcpConnection *connection = m_alpaConnectTcpThings.take(thing);
        delete connection;
    }

    if (myThings().isEmpty() && m_pluginTimer) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginAlphaInnotec::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    AlphaConnectModbusTcpConnection *connection = m_alpaConnectTcpThings.value(thing);

    if (!connection->connected()) {
        qCWarning(dcAlphaInnotec()) << "Could not execute action. The modbus connection is currently not available.";
        info->finish(Thing::ThingErrorHardwareNotAvailable);
        return;
    }

    if (thing->thingClassId() == alphaConnectThingClassId) {
  /*      if (info->action().actionTypeId() == alphaConnectOutdoorTemperatureActionTypeId) {
            double outdoorTemperature = info->action().paramValue(alphaConnectOutdoorTemperatureActionOutdoorTemperatureParamTypeId).toDouble();
            qCDebug(dcAlphaInnotec()) << "Execute action" << info->action().actionTypeId().toString() << info->action().params();
            QModbusReply *reply = connection->setOutdoorTemperature(outdoorTemperature);
            if (!reply) {
                qCWarning(dcAlphaInnotec()) << "Execute action failed because the reply could not be created.";
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }

            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, info, [info, reply, outdoorTemperature]{
                if (reply->error() != QModbusDevice::NoError) {
                    info->finish(Thing::ThingErrorHardwareFailure);
                    qCWarning(dcAlphaInnotec()) << "Set outdoor temperature finished with error" << reply->errorString();
                    return;
                }

                qCDebug(dcAlphaInnotec()) << "Execute action finished successfully" << info->action().actionTypeId().toString() << info->action().params();
                info->thing()->setStateValue(alphaConnectOutdoorTemperatureStateTypeId, outdoorTemperature);
                info->finish(Thing::ThingErrorNoError);
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaInnotec()) << "Modbus reply error occurred while execute action" << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else */
        if (info->action().actionTypeId() == alphaConnectHotWaterSetpointTemperatureActionTypeId) {
            double temperature = info->action().paramValue(alphaConnectHotWaterSetpointTemperatureActionHotWaterSetpointTemperatureParamTypeId).toDouble();
            qCDebug(dcAlphaInnotec()) << "Execute action" << info->action().actionTypeId().toString() << info->action().params();
            QModbusReply *reply = connection->setHotWaterSetpointTemperature(temperature);
            if (!reply) {
                qCWarning(dcAlphaInnotec()) << "Execute action failed because the reply could not be created.";
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }

            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, info, [info, reply, temperature]{
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcAlphaInnotec()) << "Set hot water setpoint temperature finished with error" << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                    return;
                }

                qCDebug(dcAlphaInnotec()) << "Execute action finished successfully" << info->action().actionTypeId().toString() << info->action().params();
                info->thing()->setStateValue(alphaConnectHotWaterSetpointTemperatureStateTypeId, temperature);
                info->finish(Thing::ThingErrorNoError);
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaInnotec()) << "Modbus reply error occurred while execute action" << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else if (info->action().actionTypeId() == alphaConnectReturnSetpointTemperatureActionTypeId) {
            double temperature = info->action().paramValue(alphaConnectReturnSetpointTemperatureActionReturnSetpointTemperatureParamTypeId).toDouble();
            qCDebug(dcAlphaInnotec()) << "Execute action" << info->action().actionTypeId().toString() << info->action().params();
            QModbusReply *reply = connection->setReturnSetpointTemperature(temperature);
            if (!reply) {
                qCWarning(dcAlphaInnotec()) << "Execute action failed because the reply could not be created.";
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }

            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, info, [info, reply, temperature]{
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcAlphaInnotec()) << "Set return setpoint temperature finished with error" << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                    return;
                }

                qCDebug(dcAlphaInnotec()) << "Execute action finished successfully" << info->action().actionTypeId().toString() << info->action().params();
                info->thing()->setStateValue(alphaConnectReturnSetpointTemperatureStateTypeId, temperature);
                info->finish(Thing::ThingErrorNoError);
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaInnotec()) << "Modbus reply error occurred while execute action" << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else if (info->action().actionTypeId() == alphaConnectSgReadyModeActionTypeId) {
            QString sgReadyModeString = info->action().paramValue(alphaConnectSgReadyModeActionSgReadyModeParamTypeId).toString();
            qCDebug(dcAlphaInnotec()) << "Execute action" << info->action().actionTypeId().toString() << info->action().params();
            AlphaConnectModbusTcpConnection::SmartGridState sgReadyState;
            if (sgReadyModeString == "Off") {
                sgReadyState = AlphaConnectModbusTcpConnection::SmartGridStateOff;
            } else if (sgReadyModeString == "Low") {
                sgReadyState = AlphaConnectModbusTcpConnection::SmartGridStateLow;
            } else if (sgReadyModeString == "High") {
                sgReadyState = AlphaConnectModbusTcpConnection::SmartGridStateHigh;
            } else {
                sgReadyState = AlphaConnectModbusTcpConnection::SmartGridStateStandard;
            }

            QModbusReply *reply = connection->setSmartGrid(sgReadyState);
            if (!reply) {
                qCWarning(dcAlphaInnotec()) << "Execute action failed because the reply could not be created.";
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }

            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, info, [info, reply, sgReadyModeString]{
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcAlphaInnotec()) << "Set SG ready mode finished with error" << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                    return;
                }

                qCDebug(dcAlphaInnotec()) << "Execute action finished successfully" << info->action().actionTypeId().toString() << info->action().params();
                info->thing()->setStateValue(alphaConnectSgReadyModeStateTypeId, sgReadyModeString);
                info->finish(Thing::ThingErrorNoError);
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaInnotec()) << "Modbus reply error occurred while execute action" << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        }
    }

    info->finish(Thing::ThingErrorNoError);
}


