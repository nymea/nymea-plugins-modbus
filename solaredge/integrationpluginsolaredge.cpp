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

#include "plugininfo.h"
#include "integrationpluginsolaredge.h"
#include "network/networkdevicediscovery.h"

#include <QHostAddress>

IntegrationPluginSolarEdge::IntegrationPluginSolarEdge()
{

}

void IntegrationPluginSolarEdge::init()
{
    m_connectedStateTypeIds.insert(solarEdgeConnectionThingClassId, solarEdgeConnectionConnectedStateTypeId);
    m_connectedStateTypeIds.insert(solarEdgeSinglePhaseInverterThingClassId, solarEdgeSinglePhaseInverterConnectedStateTypeId);
    m_connectedStateTypeIds.insert(solarEdgeSplitPhaseInverterThingClassId, solarEdgeSplitPhaseInverterConnectedStateTypeId);
    m_connectedStateTypeIds.insert(solarEdgeThreePhaseInverterThingClassId, solarEdgeThreePhaseInverterConnectedStateTypeId);
    m_connectedStateTypeIds.insert(solarEdgeSinglePhaseMeterThingClassId, solarEdgeSinglePhaseMeterConnectedStateTypeId);
    m_connectedStateTypeIds.insert(solarEdgeSplitPhaseMeterThingClassId, solarEdgeSplitPhaseMeterConnectedStateTypeId);
    m_connectedStateTypeIds.insert(solarEdgeThreePhaseMeterThingClassId, solarEdgeThreePhaseMeterConnectedStateTypeId);

    m_modelIdParamTypeIds.insert(solarEdgeSinglePhaseInverterThingClassId, solarEdgeSinglePhaseInverterThingModelIdParamTypeId);
    m_modelIdParamTypeIds.insert(solarEdgeSplitPhaseInverterThingClassId, solarEdgeSplitPhaseInverterThingModelIdParamTypeId);
    m_modelIdParamTypeIds.insert(solarEdgeThreePhaseInverterThingClassId, solarEdgeThreePhaseInverterThingModelIdParamTypeId);
    m_modelIdParamTypeIds.insert(solarEdgeSinglePhaseMeterThingClassId, solarEdgeSinglePhaseMeterThingModelIdParamTypeId);
    m_modelIdParamTypeIds.insert(solarEdgeSplitPhaseMeterThingClassId, solarEdgeSplitPhaseMeterThingModelIdParamTypeId);
    m_modelIdParamTypeIds.insert(solarEdgeThreePhaseMeterThingClassId, solarEdgeThreePhaseMeterThingModelIdParamTypeId);

    m_modbusAddressParamTypeIds.insert(solarEdgeSinglePhaseInverterThingClassId, solarEdgeSinglePhaseInverterThingModbusAddressParamTypeId);
    m_modbusAddressParamTypeIds.insert(solarEdgeSplitPhaseInverterThingClassId, solarEdgeSplitPhaseInverterThingModbusAddressParamTypeId);
    m_modbusAddressParamTypeIds.insert(solarEdgeThreePhaseInverterThingClassId, solarEdgeThreePhaseInverterThingModbusAddressParamTypeId);
    m_modbusAddressParamTypeIds.insert(solarEdgeSinglePhaseMeterThingClassId, solarEdgeSinglePhaseMeterThingModbusAddressParamTypeId);
    m_modbusAddressParamTypeIds.insert(solarEdgeSplitPhaseMeterThingClassId, solarEdgeSplitPhaseMeterThingModbusAddressParamTypeId);
    m_modbusAddressParamTypeIds.insert(solarEdgeThreePhaseMeterThingClassId, solarEdgeThreePhaseMeterThingModbusAddressParamTypeId);

    m_inverterCurrentPowerStateTypeIds.insert(solarEdgeSinglePhaseInverterThingClassId, solarEdgeSinglePhaseInverterCurrentPowerStateTypeId);
    m_inverterCurrentPowerStateTypeIds.insert(solarEdgeSplitPhaseInverterThingClassId, solarEdgeSplitPhaseInverterCurrentPowerStateTypeId);
    m_inverterCurrentPowerStateTypeIds.insert(solarEdgeThreePhaseInverterThingClassId, solarEdgeThreePhaseInverterCurrentPowerStateTypeId);

    m_inverterTotalEnergyProducedStateTypeIds.insert(solarEdgeSinglePhaseInverterThingClassId, solarEdgeSinglePhaseInverterTotalEnergyProducedStateTypeId);
    m_inverterTotalEnergyProducedStateTypeIds.insert(solarEdgeSplitPhaseInverterThingClassId, solarEdgeSplitPhaseInverterTotalEnergyProducedStateTypeId);
    m_inverterTotalEnergyProducedStateTypeIds.insert(solarEdgeThreePhaseInverterThingClassId, solarEdgeThreePhaseInverterTotalEnergyProducedStateTypeId);

    m_inverterOperatingStateTypeIds.insert(solarEdgeSinglePhaseInverterThingClassId, solarEdgeSinglePhaseInverterOperatingStateStateTypeId);
    m_inverterOperatingStateTypeIds.insert(solarEdgeSplitPhaseInverterThingClassId, solarEdgeSplitPhaseInverterOperatingStateStateTypeId);
    m_inverterOperatingStateTypeIds.insert(solarEdgeThreePhaseInverterThingClassId, solarEdgeThreePhaseInverterOperatingStateStateTypeId);

    m_inverterErrorStateTypeIds.insert(solarEdgeSinglePhaseInverterThingClassId, solarEdgeSinglePhaseInverterErrorStateTypeId);
    m_inverterErrorStateTypeIds.insert(solarEdgeSplitPhaseInverterThingClassId, solarEdgeSplitPhaseInverterErrorStateTypeId);
    m_inverterErrorStateTypeIds.insert(solarEdgeThreePhaseInverterThingClassId, solarEdgeThreePhaseInverterErrorStateTypeId);

    m_inverterCabinetTemperatureStateTypeIds.insert(solarEdgeSinglePhaseInverterThingClassId, solarEdgeSinglePhaseInverterCabinetTemperatureStateTypeId);
    m_inverterCabinetTemperatureStateTypeIds.insert(solarEdgeSplitPhaseInverterThingClassId, solarEdgeSplitPhaseInverterCabinetTemperatureStateTypeId);
    m_inverterCabinetTemperatureStateTypeIds.insert(solarEdgeThreePhaseInverterThingClassId, solarEdgeThreePhaseInverterCabinetTemperatureStateTypeId);

    m_inverterAcCurrentStateTypeIds.insert(solarEdgeSinglePhaseInverterThingClassId, solarEdgeSinglePhaseInverterTotalCurrentStateTypeId);
    m_inverterAcCurrentStateTypeIds.insert(solarEdgeSplitPhaseInverterThingClassId, solarEdgeSplitPhaseInverterTotalCurrentStateTypeId);
    m_inverterAcCurrentStateTypeIds.insert(solarEdgeThreePhaseInverterThingClassId, solarEdgeThreePhaseInverterTotalCurrentStateTypeId);

    m_frequencyStateTypeIds.insert(solarEdgeSinglePhaseInverterThingClassId, solarEdgeSinglePhaseInverterFrequencyStateTypeId);
    m_frequencyStateTypeIds.insert(solarEdgeSplitPhaseInverterThingClassId, solarEdgeSplitPhaseInverterFrequencyStateTypeId);
    m_frequencyStateTypeIds.insert(solarEdgeThreePhaseInverterThingClassId, solarEdgeThreePhaseInverterFrequencyStateTypeId);

    m_frequencyStateTypeIds.insert(solarEdgeSinglePhaseMeterThingClassId, solarEdgeSinglePhaseMeterFrequencyStateTypeId);
    m_frequencyStateTypeIds.insert(solarEdgeSplitPhaseMeterThingClassId, solarEdgeSplitPhaseMeterFrequencyStateTypeId);
    m_frequencyStateTypeIds.insert(solarEdgeThreePhaseMeterThingClassId, solarEdgeThreePhaseMeterFrequencyStateTypeId);
}

void IntegrationPluginSolarEdge::discoverThings(ThingDiscoveryInfo *info)
{
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcSolarEdge()) << "Failed to discover network devices. The network device discovery is not available.";
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The discovery is not available."));
        return;
    }

    NetworkDeviceDiscoveryReply *discoveryReply = hardwareManager()->networkDeviceDiscovery()->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        ThingDescriptors descriptors;
        qCDebug(dcSolarEdge()) << "Discovery finished. Found" << discoveryReply->networkDeviceInfos().count() << "devices";
        foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {
            qCDebug(dcSolarEdge()) << networkDeviceInfo;

            // Filter for solar edge registered mac addresses
            if (!networkDeviceInfo.macAddressManufacturer().toLower().contains("solaredge"))
                continue;

            QString title;
            if (networkDeviceInfo.hostName().isEmpty()) {
                title += "SolarEdge (" + networkDeviceInfo.address().toString() + ")";
            } else {
                title += networkDeviceInfo.address().toString() + " (" + networkDeviceInfo.hostName() + ")";
            }

            QString description;
            if (networkDeviceInfo.macAddressManufacturer().isEmpty()) {
                description = networkDeviceInfo.macAddress();
            } else {
                description = networkDeviceInfo.macAddress() + " (" + networkDeviceInfo.macAddressManufacturer() + ")";
            }

            ThingDescriptor descriptor(solarEdgeConnectionThingClassId, title, description);

            // Check if we already have set up this device
            Things existingThings = myThings().filterByParam(solarEdgeConnectionThingIpAddressParamTypeId, networkDeviceInfo.address().toString());
            if (existingThings.count() == 1) {
                qCDebug(dcSolarEdge()) << "This thing already exists in the system." << existingThings.first() << networkDeviceInfo;
                descriptor.setThingId(existingThings.first()->id());
            }

            ParamList params;
            params << Param(solarEdgeConnectionThingIpAddressParamTypeId, networkDeviceInfo.address().toString());
            descriptor.setParams(params);
            info->addThingDescriptor(descriptor);
        }
        info->finish(Thing::ThingErrorNoError);
    });
}

void IntegrationPluginSolarEdge::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcSolarEdge()) << "Setup thing" << thing->name();
    if (thing->thingClassId() == solarEdgeConnectionThingClassId) {
        QHostAddress address = QHostAddress(info->thing()->paramValue(solarEdgeConnectionThingIpAddressParamTypeId).toString());
        int port = info->thing()->paramValue(solarEdgeConnectionThingPortParamTypeId).toInt();
        int slaveId = info->thing()->paramValue(solarEdgeConnectionThingSlaveIdParamTypeId).toInt();

        if (m_sunSpecConnections.contains(thing->id())) {
            qCDebug(dcSolarEdge()) << "Reconfigure SunSpec connection with new address" << address;
            m_sunSpecConnections.take(thing->id())->deleteLater();
        }

        SunSpec *sunSpec = new SunSpec(address, port, slaveId, this);
        sunSpec->setTimeout(configValue(solarEdgePluginTimeoutParamTypeId).toUInt());
        sunSpec->setNumberOfRetries(configValue(solarEdgePluginNumberOfRetriesParamTypeId).toUInt());
        m_sunSpecConnections.insert(thing->id(), sunSpec);
        if (!sunSpec->connectModbus()) {
            qCWarning(dcSolarEdge()) << "Error connecting to SunSpec device";
            return info->finish(Thing::ThingErrorHardwareNotAvailable);
        }

        connect(sunSpec, &SunSpec::connectionStateChanged, info, [sunSpec, info] (bool status) {
            qCDebug(dcSolarEdge()) << "Modbus connection init finished" << status;
            sunSpec->findBaseRegister();
            connect(sunSpec, &SunSpec::foundBaseRegister, info, [info] (uint modbusAddress) {
                qCDebug(dcSolarEdge()) << "Found sunspec base register" << modbusAddress;
                info->finish(Thing::ThingErrorNoError);
            });
        });

        connect(info, &ThingSetupInfo::aborted, sunSpec, &SunSpec::deleteLater);
        connect(sunSpec, &SunSpec::destroyed, thing, [this, thing] {
            m_sunSpecConnections.remove(thing->id());
        });

        connect(sunSpec, &SunSpec::foundSunSpecModel, this, &IntegrationPluginSolarEdge::onFoundSunSpecModel);
        connect(sunSpec, &SunSpec::sunspecModelSearchFinished, this, &IntegrationPluginSolarEdge::onSunSpecModelSearchFinished);
        connect(sunSpec, &SunSpec::commonModelReceived, thing, [thing] (const QString &manufacturer, const QString &deviceModel, const QString &serielNumber) {
            thing->setStateValue(solarEdgeConnectionConnectedStateTypeId, true);
            thing->setStateValue(solarEdgeConnectionManufacturerStateTypeId, manufacturer);
            thing->setStateValue(solarEdgeConnectionDeviceModelStateTypeId, deviceModel);
            thing->setStateValue(solarEdgeConnectionSerialNumberStateTypeId, serielNumber);
        });

    } else if (thing->thingClassId() == solarEdgeThreePhaseInverterThingClassId ||
               thing->thingClassId() == solarEdgeSplitPhaseInverterThingClassId ||
               thing->thingClassId() == solarEdgeSinglePhaseInverterThingClassId ) {

        Thing *parent = myThings().findById(thing->parentId());
        if (parent->setupStatus() == Thing::ThingSetupStatusComplete) {
            setupInverter(info);
        } else {
            connect(parent, &Thing::setupStatusChanged, info, [this, info] { setupInverter(info); });
        }
    } else if (thing->thingClassId() == solarEdgeSinglePhaseMeterThingClassId ||
               thing->thingClassId() == solarEdgeSplitPhaseMeterThingClassId ||
               thing->thingClassId() == solarEdgeThreePhaseMeterThingClassId) {

        Thing *parent = myThings().findById(thing->parentId());
        if (parent->setupStatus() == Thing::ThingSetupStatusComplete) {
            setupMeter(info);
        } else {
            connect(parent, &Thing::setupStatusChanged, info, [this, info] { setupMeter(info); });
        }

    } /*else if (info->thing()->thingClassId() == solarEdgeStorageThingClassId) {

        Thing *parent = myThings().findById(thing->parentId());
        if (parent->setupStatus() == Thing::ThingSetupStatusComplete) {
            setupStorage(info);
        } else {
            connect(parent, &Thing::setupStatusChanged, info, [this, info] {
                setupStorage(info);
            });
        }
    } */else {
        Q_ASSERT_X(false, "setupThing", QString("Unhandled thingClassId: %1").arg(info->thing()->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginSolarEdge::postSetupThing(Thing *thing)
{
    qCDebug(dcSolarEdge()) << "Post setup thing" << thing->name();

    if (!m_refreshTimer) {
        qCDebug(dcSolarEdge()) << "Starting refresh timer";
        int refreshTime = configValue(solarEdgePluginUpdateIntervalParamTypeId).toInt();
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(refreshTime);
        connect(m_refreshTimer, &PluginTimer::timeout, this, &IntegrationPluginSolarEdge::onRefreshTimer);
    }

    if (thing->thingClassId() == solarEdgeConnectionThingClassId) {
        SunSpec *connection = m_sunSpecConnections.value(thing->id());
        if (!connection) {
            qCDebug(dcSolarEdge()) << "SunSpecConnection not found";
            return;
        }
        connection->readCommonModel();
        connection->findSunSpecModels(QList<SunSpec::ModelId>()); // Discover all models, without filter

    } else if (thing->thingClassId() == solarEdgeSinglePhaseInverterThingClassId ||
               thing->thingClassId() == solarEdgeSplitPhaseInverterThingClassId ||
               thing->thingClassId() == solarEdgeThreePhaseInverterThingClassId) {

        SunSpecInverter *sunSpecInverter = m_sunSpecInverters.value(thing);
        if (!sunSpecInverter) {
            qCDebug(dcSolarEdge()) << "SunSpecInverter not found";
            return;
        }
        sunSpecInverter->readBlockData();

    } else if (thing->thingClassId() == solarEdgeSinglePhaseMeterThingClassId ||
               thing->thingClassId() == solarEdgeSplitPhaseMeterThingClassId ||
               thing->thingClassId() == solarEdgeThreePhaseMeterThingClassId) {

        SunSpecMeter *sunSpecMeter = m_sunSpecMeters.value(thing);
        if (!sunSpecMeter) {
            qCDebug(dcSolarEdge()) << "SunSpecMeter not found";
            return;
        }
        sunSpecMeter->readBlockData();

    } /*else if (thing->thingClassId() == solarEdgeStorageThingClassId) {
        SunSpecStorage *sunSpecStorage = m_sunSpecStorages.value(thing);
        if (!sunSpecStorage) {
            qCDebug(dcSolarEdge()) << "SunSpecStorage not found";
            return;
        }
        sunSpecStorage->getStorageModelDataBlock();

    }*/ else {
        Q_ASSERT_X(false, "postSetupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }

}

void IntegrationPluginSolarEdge::thingRemoved(Thing *thing)
{
    qCDebug(dcSolarEdge()) << "Thing removed" << thing->name();

    if (thing->thingClassId() == solarEdgeConnectionThingClassId) {
        SunSpec *sunSpecConnection = m_sunSpecConnections.take(thing->id());
        if (sunSpecConnection)
            sunSpecConnection->deleteLater();

    } else if (thing->thingClassId() == solarEdgeSinglePhaseInverterThingClassId ||
               thing->thingClassId() == solarEdgeSplitPhaseInverterThingClassId ||
               thing->thingClassId() == solarEdgeThreePhaseInverterThingClassId) {
        SunSpecInverter *sunSpecInverter = m_sunSpecInverters.take(thing);
        if (sunSpecInverter)
            sunSpecInverter->deleteLater();

    } else if (thing->thingClassId() == solarEdgeSinglePhaseMeterThingClassId ||
               thing->thingClassId() == solarEdgeSplitPhaseMeterThingClassId ||
               thing->thingClassId() == solarEdgeThreePhaseMeterThingClassId) {
        SunSpecMeter *sunSpecMeter = m_sunSpecMeters.take(thing);
        if (sunSpecMeter)
            sunSpecMeter->deleteLater();

    } /*else if (thing->thingClassId() == solarEdgeStorageThingClassId) {
        SunSpecStorage *sunSpecStorage = m_sunSpecStorages.take(thing);
        if (sunSpecStorage)
            sunSpecStorage->deleteLater();

    }*/ else {
        Q_ASSERT_X(false, "thingRemoved", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }

    if (myThings().isEmpty()) {
        qCDebug(dcSolarEdge()) << "Stopping refresh timer";
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_refreshTimer);
        m_refreshTimer = nullptr;
    }
}

void IntegrationPluginSolarEdge::executeAction(ThingActionInfo *info)
{
    Q_UNUSED(info)

//    Thing *thing = info->thing();
//    Action action = info->action();

//    if (thing->thingClassId() == solarEdgeStorageThingClassId) {
//        SunSpecStorage *sunSpecStorage = m_sunSpecStorages.value(thing);
//        if (!sunSpecStorage) {
//            qWarning(dcSolarEdge()) << "Could not find solarEdge instance for thing";
//            info->finish(Thing::ThingErrorHardwareNotAvailable);
//            return;
//        }

//        if (action.actionTypeId() == solarEdgeStorageGridChargingActionTypeId) {
//            QUuid requestId = sunSpecStorage->setGridCharging(action.param(solarEdgeStorageGridChargingActionGridChargingParamTypeId).value().toBool());
//            if (requestId.isNull()) {
//                info->finish(Thing::ThingErrorHardwareFailure);
//            } else {
//                m_asyncActions.insert(requestId, info);
//                connect(info, &ThingActionInfo::aborted, this, [requestId, this] {m_asyncActions.remove(requestId);});
//            }
//        } else if (action.actionTypeId() == solarEdgeStorageEnableChargingActionTypeId) {
//            bool charging = action.param(solarEdgeStorageEnableChargingActionEnableChargingParamTypeId).value().toBool();
//            bool discharging = thing->stateValue(solarEdgeStorageEnableDischargingStateTypeId).toBool();
//            QUuid requestId = sunSpecStorage->setStorageControlMode(charging, discharging);
//            if (requestId.isNull()) {
//                info->finish(Thing::ThingErrorHardwareFailure);
//            } else {
//                m_asyncActions.insert(requestId, info);
//                connect(info, &ThingActionInfo::aborted, this, [requestId, this] {m_asyncActions.remove(requestId);});
//            }
//        } else if (action.actionTypeId() == solarEdgeStorageChargingRateActionTypeId) {
//            QUuid requestId = sunSpecStorage->setChargingRate(action.param(solarEdgeStorageChargingRateActionChargingRateParamTypeId).value().toInt());
//            if (requestId.isNull()) {
//                info->finish(Thing::ThingErrorHardwareFailure);
//            } else {
//                m_asyncActions.insert(requestId, info);
//                connect(info, &ThingActionInfo::aborted, this, [requestId, this] {m_asyncActions.remove(requestId);});
//            }
//        } else if (action.actionTypeId() == solarEdgeStorageEnableDischargingActionTypeId) {
//            bool discharging = action.param(solarEdgeStorageEnableDischargingActionEnableDischargingParamTypeId).value().toBool();
//            bool charging = thing->stateValue(solarEdgeStorageEnableChargingStateTypeId).toBool();
//            QUuid requestId = sunSpecStorage->setStorageControlMode(charging, discharging);
//            if (requestId.isNull()) {
//                info->finish(Thing::ThingErrorHardwareFailure);
//            } else {
//                m_asyncActions.insert(requestId, info);
//                connect(info, &ThingActionInfo::aborted, this, [requestId, this] {m_asyncActions.remove(requestId);});
//            }
//        } else if (action.actionTypeId() == solarEdgeStorageDischargingRateActionTypeId) {
//            QUuid requestId = sunSpecStorage->setDischargingRate(action.param(solarEdgeStorageDischargingRateActionDischargingRateParamTypeId).value().toInt());
//            if (requestId.isNull()) {
//                info->finish(Thing::ThingErrorHardwareFailure);
//            } else {
//                m_asyncActions.insert(requestId, info);
//                connect(info, &ThingActionInfo::aborted, this, [requestId, this] {m_asyncActions.remove(requestId);});
//            }
//        } else {
//            Q_ASSERT_X(false, "executeAction", QString("Unhandled action: %1").arg(action.actionTypeId().toString()).toUtf8());
//        }
//    } else {
//        Q_ASSERT_X(false, "executeAction", QString("Unhandled thingClassId: %1").arg(info->thing()->thingClassId().toString()).toUtf8());
//    }
}

bool IntegrationPluginSolarEdge::checkIfThingExists(uint modelId, uint modbusAddress)
{
    foreach (Thing *thing, myThings()) {
        if (thing->paramValue(m_modelIdParamTypeIds.value(thing->thingClassId())).toUInt() == modelId &&
                thing->paramValue(m_modbusAddressParamTypeIds.value(thing->thingClassId())).toUInt() == modbusAddress) {
            return true;
        }
    }

    return false;
}

void IntegrationPluginSolarEdge::setupInverter(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    uint modelId = thing->paramValue(m_modelIdParamTypeIds.value(thing->thingClassId())).toInt();
    int modbusAddress = thing->paramValue(m_modbusAddressParamTypeIds.value(thing->thingClassId())).toInt();

    SunSpec *connection = m_sunSpecConnections.value(thing->parentId());
    if (!connection) {
        qCWarning(dcSolarEdge()) << "Could not find SunSpec connection";
        return info->finish(Thing::ThingErrorHardwareNotAvailable);
    }

    SunSpecInverter *sunSpecInverter = new SunSpecInverter(connection, SunSpec::ModelId(modelId), modbusAddress);
    sunSpecInverter->init();
    connect(sunSpecInverter, &SunSpecInverter::initFinished, info, [this, sunSpecInverter, info] (bool success){
        qCDebug(dcSolarEdge()) << "Modbus Inverter init finished, success:" << success;
        if (success) {
            m_sunSpecInverters.insert(info->thing(), sunSpecInverter);
            info->finish(Thing::ThingErrorNoError);
        } else {
            info->finish(Thing::ThingErrorHardwareNotAvailable);
        }
    });

    connect(info, &ThingSetupInfo::aborted, sunSpecInverter, &SunSpecInverter::deleteLater);
    connect(sunSpecInverter, &SunSpecInverter::destroyed, thing, [thing, this] {m_sunSpecInverters.remove(thing);});
    connect(sunSpecInverter, &SunSpecInverter::inverterDataReceived, this, &IntegrationPluginSolarEdge::onInverterDataReceived);
}

void IntegrationPluginSolarEdge::setupMeter(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    uint modelId = thing->paramValue(m_modelIdParamTypeIds.value(thing->thingClassId())).toInt();
    int modbusAddress = thing->paramValue(m_modbusAddressParamTypeIds.value(thing->thingClassId())).toInt();

    SunSpec *connection = m_sunSpecConnections.value(thing->parentId());
    if (!connection) {
        qCWarning(dcSolarEdge()) << "Could not find SunSpec connection";
        return info->finish(Thing::ThingErrorHardwareNotAvailable);
    }

    SunSpecMeter *sunSpecMeter = new SunSpecMeter(connection, SunSpec::ModelId(modelId), modbusAddress);
    sunSpecMeter->init();
    connect(sunSpecMeter, &SunSpecMeter::initFinished, info, [this, sunSpecMeter, info] (bool success){
        qCDebug(dcSolarEdge()) << "Modbus meter init finished, success:" << success;
        if (success) {
            m_sunSpecMeters.insert(info->thing(), sunSpecMeter);
            info->finish(Thing::ThingErrorNoError);
        } else {
            info->finish(Thing::ThingErrorHardwareNotAvailable);
        }
    });

    connect(info, &ThingSetupInfo::aborted, sunSpecMeter, &SunSpecMeter::deleteLater);
    connect(sunSpecMeter, &SunSpecMeter::destroyed, thing, [thing, this] {m_sunSpecMeters.remove(thing);});
    connect(sunSpecMeter, &SunSpecMeter::meterDataReceived, this, &IntegrationPluginSolarEdge::onMeterDataReceived);
}

void IntegrationPluginSolarEdge::setupStorage(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    uint modelId = thing->paramValue(m_modelIdParamTypeIds.value(thing->thingClassId())).toInt();
    int modbusAddress = thing->paramValue(m_modbusAddressParamTypeIds.value(thing->thingClassId())).toInt();

    SunSpec *connection = m_sunSpecConnections.value(thing->parentId());
    if (!connection) {
        qCWarning(dcSolarEdge()) << "Could not find SunSpec connection for" << thing;
        return info->finish(Thing::ThingErrorHardwareNotAvailable);
    }

    SunSpecStorage *sunSpecStorage = new SunSpecStorage(connection, SunSpec::ModelId(modelId), modbusAddress);
    sunSpecStorage->init();
    connect(sunSpecStorage, &SunSpecStorage::initFinished, info, [this, sunSpecStorage, info] (bool success){
        qCDebug(dcSolarEdge()) << "Modbus storage init finished, success:" << success;
        if (success) {
            m_sunSpecStorages.insert(info->thing(), sunSpecStorage);
            info->finish(Thing::ThingErrorNoError);
        } else {
            info->finish(Thing::ThingErrorHardwareNotAvailable);
        }
    });

    connect(info, &ThingSetupInfo::aborted, sunSpecStorage, &SunSpecStorage::deleteLater);
    connect(sunSpecStorage, &SunSpecStorage::destroyed, thing, [thing, this] {m_sunSpecStorages.remove(thing);});
    connect(sunSpecStorage, &SunSpecStorage::storageDataReceived, this, &IntegrationPluginSolarEdge::onStorageDataReceived);
}

void IntegrationPluginSolarEdge::searchBatteries()
{
    qCDebug(dcSolarEdge()) << "Searching for connected batteries...";


}

void IntegrationPluginSolarEdge::onRefreshTimer()
{
    foreach (SunSpec *connection, m_sunSpecConnections) {
        connection->readCommonModel(); //check connection
    }

    foreach (SunSpecInverter *inverter, m_sunSpecInverters) {
        inverter->readBlockData();
    }

    foreach (SunSpecMeter *meter, m_sunSpecMeters) {
        meter->readBlockData();
    }

    foreach (SunSpecStorage *storage, m_sunSpecStorages) {
        storage->readBlockData();
    }
}

void IntegrationPluginSolarEdge::onPluginConfigurationChanged(const ParamTypeId &paramTypeId, const QVariant &value)
{
    // Check refresh schedule
    if (paramTypeId == solarEdgePluginUpdateIntervalParamTypeId) {
        qCDebug(dcSolarEdge()) << "Update interval has changed" << value.toInt();
        if (m_refreshTimer) {
            int refreshTime = value.toInt();
            m_refreshTimer->stop();
            m_refreshTimer->startTimer(refreshTime);
        }
    } else if (paramTypeId == solarEdgePluginNumberOfRetriesParamTypeId) {
        qCDebug(dcSolarEdge()) << "Updating number of retries" << value.toUInt();
        foreach (SunSpec *connection, m_sunSpecConnections) {
            connection->setNumberOfRetries(value.toUInt());
        }
    } else if (paramTypeId == solarEdgePluginTimeoutParamTypeId) {
        qCDebug(dcSolarEdge()) << "Updating timeout" << value.toUInt() << "[ms]";
        foreach (SunSpec *connection, m_sunSpecConnections) {
            connection->setTimeout(value.toUInt());
        }
    } else {
        qCWarning(dcSolarEdge()) << "Unknown plugin configuration" << paramTypeId << "Value" << value;
    }
}

void IntegrationPluginSolarEdge::onConnectionStateChanged(bool status)
{
    SunSpec *connection = static_cast<SunSpec *>(sender());
    Thing *thing = myThings().findById(m_sunSpecConnections.key(connection));
    if (!thing) return;

    qCDebug(dcSolarEdge()) << "Connection state changed" << status << thing->name();
    if (thing->thingClassId() == solarEdgeConnectionConnectedStateTypeId) {
        thing->setStateValue(solarEdgeConnectionConnectedStateTypeId, status);
    }

    foreach (Thing *child, myThings().filterByParentId(thing->id())) {
        child->setStateValue(m_connectedStateTypeIds.value(thing->thingClassId()), status);
    }
}

void IntegrationPluginSolarEdge::onFoundSunSpecModel(SunSpec::ModelId modelId, int modbusStartRegister)
{
    SunSpec *connection = static_cast<SunSpec *>(sender());
    Thing *thing = myThings().findById(m_sunSpecConnections.key(connection));
    if (!thing) {
        qCWarning(dcSolarEdge()) << "Thing not found for SunSpec connection" << connection->deviceModel() << connection->serialNumber();
        return;
    }

    qCDebug(dcSolarEdge()) << "On model received" << modelId << "start register" << modbusStartRegister << "Thing:" << thing->name();
    if (checkIfThingExists(modelId, modbusStartRegister)) {
        return;
    }
    QString model = thing->stateValue(solarEdgeConnectionDeviceModelStateTypeId).toString();
    switch (modelId) {
    case SunSpec::ModelId::ModelIdInverterSinglePhase:
    case SunSpec::ModelId::ModelIdInverterSinglePhaseFloat: {
        ThingDescriptor descriptor(solarEdgeSinglePhaseInverterThingClassId, model+tr(" single phase inverter"), "", thing->id());
        ParamList params;
        params.append(Param(solarEdgeSinglePhaseInverterThingModelIdParamTypeId, modelId));
        params.append(Param(solarEdgeSinglePhaseInverterThingModbusAddressParamTypeId, modbusStartRegister));
        descriptor.setParams(params);
        emit autoThingsAppeared({descriptor});
    } break;
    case SunSpec::ModelId::ModelIdInverterSplitPhase:
    case SunSpec::ModelId::ModelIdInverterSplitPhaseFloat: {
        ThingDescriptor descriptor(solarEdgeSplitPhaseInverterThingClassId, model+tr(" split phase inverter"), "", thing->id());
        ParamList params;
        params.append(Param(solarEdgeSplitPhaseInverterThingModelIdParamTypeId, modelId));
        params.append(Param(solarEdgeSplitPhaseInverterThingModbusAddressParamTypeId, modbusStartRegister));
        descriptor.setParams(params);
        emit autoThingsAppeared({descriptor});
    } break;
    case SunSpec::ModelId::ModelIdInverterThreePhase:
    case SunSpec::ModelId::ModelIdInverterThreePhaseFloat: {
        ThingDescriptor descriptor(solarEdgeThreePhaseInverterThingClassId, model+tr(" three phase inverter"), "", thing->id());
        ParamList params;
        params.append(Param(solarEdgeThreePhaseInverterThingModelIdParamTypeId, modelId));
        params.append(Param(solarEdgeThreePhaseInverterThingModbusAddressParamTypeId, modbusStartRegister));
        descriptor.setParams(params);
        emit autoThingsAppeared({descriptor});
    } break;

    case SunSpec::ModelIdSinglePhaseMeter:
    case SunSpec::ModelIdSinglePhaseMeterFloat: {
        ThingDescriptor descriptor(solarEdgeSinglePhaseMeterThingClassId, model+tr(" meter"), "", thing->id());
        ParamList params;
        params.append(Param(solarEdgeSinglePhaseMeterThingModelIdParamTypeId, modelId));
        params.append(Param(solarEdgeSinglePhaseMeterThingModbusAddressParamTypeId, modbusStartRegister));
        descriptor.setParams(params);
        emit autoThingsAppeared({descriptor});
    } break;
    case SunSpec::ModelIdSplitSinglePhaseMeter:
    case SunSpec::ModelIdSplitSinglePhaseMeterFloat: {
        ThingDescriptor descriptor(solarEdgeSplitPhaseMeterThingClassId, model+tr(" meter"), "", thing->id());
        ParamList params;
        params.append(Param(solarEdgeSplitPhaseMeterThingModelIdParamTypeId, modelId));
        params.append(Param(solarEdgeSplitPhaseMeterThingModbusAddressParamTypeId, modbusStartRegister));
        descriptor.setParams(params);
        emit autoThingsAppeared({descriptor});
    } break;
    case SunSpec::ModelIdWyeConnectThreePhaseMeter:
    case SunSpec::ModelIdDeltaConnectThreePhaseMeter:
    case SunSpec::ModelIdWyeConnectThreePhaseMeterFloat:
    case SunSpec::ModelIdDeltaConnectThreePhaseMeterFloat: {
        ThingDescriptor descriptor(solarEdgeThreePhaseMeterThingClassId, model+" meter", "", thing->id());
        ParamList params;
        params.append(Param(solarEdgeThreePhaseMeterThingModelIdParamTypeId, modelId));
        params.append(Param(solarEdgeThreePhaseMeterThingModbusAddressParamTypeId, modbusStartRegister));
        descriptor.setParams(params);
        emit autoThingsAppeared({descriptor});
    } break;
//    case SunSpec::ModelIdStorage: {
//        ThingDescriptor descriptor(solarEdgeStorageThingClassId, model+" storage", "", thing->id());
//        ParamList params;
//        params.append(Param(solarEdgeStorageThingModelIdParamTypeId, modelId));
//        params.append(Param(solarEdgeStorageThingModbusAddressParamTypeId, modbusStartRegister));
//        descriptor.setParams(params);
//        emit autoThingsAppeared({descriptor});
//    } break;
    default:
        qCDebug(dcSolarEdge()) << "Model Id not handled";
    }
}

void IntegrationPluginSolarEdge::onSunSpecModelSearchFinished(const QHash<SunSpec::ModelId, int> &modelIds)
{
    SunSpec *connection = static_cast<SunSpec *>(sender());
    Thing *thing = myThings().findById(m_sunSpecConnections.key(connection));
    if (!thing) {
        qCWarning(dcSolarEdge()) << "Thing not found for SunSpec connection" << connection->deviceModel() << connection->serialNumber();
        return;
    }
    qCDebug(dcSolarEdge()) << "On solarEdge model search finished, models:" << modelIds.count();
}

void IntegrationPluginSolarEdge::onWriteRequestExecuted(QUuid requestId, bool success)
{
    qCDebug(dcSolarEdge()) << "Write request executed" << requestId << success;
    if (m_asyncActions.contains(requestId)) {
        ThingActionInfo *info = m_asyncActions.take(requestId);
        if (success) {
            info->finish(Thing::ThingErrorNoError);
        } else {
            info->finish(Thing::ThingErrorHardwareFailure);
        }
    }
}

void IntegrationPluginSolarEdge::onWriteRequestError(QUuid requestId, const QString &error)
{
    qCDebug(dcSolarEdge()) << "Write request error" << requestId << error;
}

void IntegrationPluginSolarEdge::onInverterDataReceived(const SunSpecInverter::InverterData &inverterData)
{
    SunSpecInverter *inverter = static_cast<SunSpecInverter *>(sender());
    Thing *thing = m_sunSpecInverters.key(inverter);
    if (!thing) return;

    qCDebug(dcSolarEdge()) << "Inverter data received";
    qCDebug(dcSolarEdge()) << "   - Total AC Current" << inverterData.acCurrent << "[A]";
    qCDebug(dcSolarEdge()) << "   - Phase A Current" << inverterData.phaseACurrent << "[A]";
    qCDebug(dcSolarEdge()) << "   - Phase B Current" << inverterData.phaseBCurrent << "[A]";
    qCDebug(dcSolarEdge()) << "   - Phase C Current" << inverterData.phaseCCurrent << "[A]";
    qCDebug(dcSolarEdge()) << "   - Phase voltage AB" << inverterData.phaseVoltageAB << "[V]";
    qCDebug(dcSolarEdge()) << "   - Phase voltage BC" << inverterData.phaseVoltageBC << "[V]";
    qCDebug(dcSolarEdge()) << "   - Phase voltage CA" << inverterData.phaseVoltageCA << "[V]";
    qCDebug(dcSolarEdge()) << "   - Phase voltage AN" << inverterData.phaseVoltageAN << "[V]";
    qCDebug(dcSolarEdge()) << "   - Phase voltage BN" << inverterData.phaseVoltageBN << "[V]";
    qCDebug(dcSolarEdge()) << "   - Phase voltage CN" << inverterData.phaseVoltageCN << "[V]";
    qCDebug(dcSolarEdge()) << "   - AC Power" << inverterData.acPower << "[W]";
    qCDebug(dcSolarEdge()) << "   - Line frequency" << inverterData.lineFrequency << "[Hz]";
    qCDebug(dcSolarEdge()) << "   - AC energy" << inverterData.acEnergy << "[Wh]";
    qCDebug(dcSolarEdge()) << "   - Cabinet temperature" << inverterData.cabinetTemperature << "[°C]";
    qCDebug(dcSolarEdge()) << "   - Operating state" << inverterData.operatingState;

    thing->setStateValue(m_connectedStateTypeIds.value(thing->thingClassId()), true);
    thing->setStateValue(m_inverterCurrentPowerStateTypeIds.value(thing->thingClassId()), inverterData.acPower);
    thing->setStateValue(m_inverterTotalEnergyProducedStateTypeIds.value(thing->thingClassId()), inverterData.acEnergy/1000.00);
    thing->setStateValue(m_frequencyStateTypeIds.value(thing->thingClassId()), inverterData.lineFrequency);

    thing->setStateValue(m_inverterAcCurrentStateTypeIds.value(thing->thingClassId()), inverterData.acCurrent);
    thing->setStateValue(m_inverterCabinetTemperatureStateTypeIds.value(thing->thingClassId()), inverterData.cabinetTemperature);

    if (thing->thingClassId() == solarEdgeSinglePhaseInverterThingClassId)  {
        thing->setStateValue(solarEdgeSinglePhaseInverterPhaseVoltageStateTypeId, inverterData.phaseVoltageAN);
    } else if (thing->thingClassId() == solarEdgeSplitPhaseInverterThingClassId) {
        thing->setStateValue(solarEdgeSplitPhaseInverterPhaseANVoltageStateTypeId, inverterData.phaseVoltageAN);
        thing->setStateValue(solarEdgeSplitPhaseInverterPhaseBNVoltageStateTypeId, inverterData.phaseVoltageBN);

        thing->setStateValue(solarEdgeSplitPhaseInverterPhaseACurrentStateTypeId, inverterData.phaseACurrent);
        thing->setStateValue(solarEdgeSplitPhaseInverterPhaseBCurrentStateTypeId, inverterData.phaseBCurrent);
    } else if (thing->thingClassId() == solarEdgeThreePhaseInverterThingClassId) {
        thing->setStateValue(solarEdgeThreePhaseInverterPhaseANVoltageStateTypeId, inverterData.phaseVoltageAN);
        thing->setStateValue(solarEdgeThreePhaseInverterPhaseBNVoltageStateTypeId, inverterData.phaseVoltageBN);
        thing->setStateValue(solarEdgeThreePhaseInverterPhaseCNVoltageStateTypeId, inverterData.phaseVoltageCN);

        thing->setStateValue(solarEdgeThreePhaseInverterPhaseACurrentStateTypeId, inverterData.phaseACurrent);
        thing->setStateValue(solarEdgeThreePhaseInverterPhaseBCurrentStateTypeId, inverterData.phaseBCurrent);
        thing->setStateValue(solarEdgeThreePhaseInverterPhaseCCurrentStateTypeId, inverterData.phaseCCurrent);
    }

    switch(inverterData.operatingState) {
    case SunSpec::SunSpecOperatingState::Off:
        thing->setStateValue(solarEdgeThreePhaseInverterOperatingStateStateTypeId, "Off");
        break;
    case SunSpec::SunSpecOperatingState::MPPT:
        thing->setStateValue(solarEdgeThreePhaseInverterOperatingStateStateTypeId, "MPPT");
        break;
    case SunSpec::SunSpecOperatingState::Fault:
        thing->setStateValue(solarEdgeThreePhaseInverterOperatingStateStateTypeId, "Fault");
        break;
    case SunSpec::SunSpecOperatingState::Standby:
        thing->setStateValue(solarEdgeThreePhaseInverterOperatingStateStateTypeId, "Standby");
        break;
    case SunSpec::SunSpecOperatingState::Sleeping:
        thing->setStateValue(solarEdgeThreePhaseInverterOperatingStateStateTypeId, "Sleeping");
        break;
    case SunSpec::SunSpecOperatingState::Starting:
        thing->setStateValue(solarEdgeThreePhaseInverterOperatingStateStateTypeId, "Starting");
        break;
    case SunSpec::SunSpecOperatingState::Throttled:
        thing->setStateValue(solarEdgeThreePhaseInverterOperatingStateStateTypeId, "Throttled");
        break;
    case SunSpec::SunSpecOperatingState::ShuttingDown:
        thing->setStateValue(solarEdgeThreePhaseInverterOperatingStateStateTypeId, "Shutting down");
        break;
    }

    //FIXME: Event1 may have multiple states at once. Only one is stated in nymea
    if (inverterData.event1.overTemperature) {
        thing->setStateValue(m_inverterErrorStateTypeIds.value(thing->thingClassId()), "Over temperature");
    } else if (inverterData.event1.underTemperature) {
        thing->setStateValue(m_inverterErrorStateTypeIds.value(thing->thingClassId()), "Under temperature");
    } else if (inverterData.event1.groundFault) {
        thing->setStateValue(m_inverterErrorStateTypeIds.value(thing->thingClassId()), "Ground fault");
    } else if (inverterData.event1.memoryLoss) {
        thing->setStateValue(m_inverterErrorStateTypeIds.value(thing->thingClassId()), "Memory loss");
    } else if (inverterData.event1.acOverVolt) {
        thing->setStateValue(m_inverterErrorStateTypeIds.value(thing->thingClassId()), "AC voltage above limit");
    } else if (inverterData.event1.cabinetOpen) {
        thing->setStateValue(m_inverterErrorStateTypeIds.value(thing->thingClassId()), "Cabinet open");
    } else if (inverterData.event1.acDisconnect) {
        thing->setStateValue(m_inverterErrorStateTypeIds.value(thing->thingClassId()), "AC disconnect open");
    } else if (inverterData.event1.acUnderVolt) {
        thing->setStateValue(m_inverterErrorStateTypeIds.value(thing->thingClassId()), "AC voltage under limit");
    } else if (inverterData.event1.dcDicconnect) {
        thing->setStateValue(m_inverterErrorStateTypeIds.value(thing->thingClassId()), "DC disconnect open");
    } else if (inverterData.event1.dcOverVoltage) {
        thing->setStateValue(m_inverterErrorStateTypeIds.value(thing->thingClassId()), "DC over voltage");
    } else if (inverterData.event1.overFrequency) {
        thing->setStateValue(m_inverterErrorStateTypeIds.value(thing->thingClassId()), "Frequency above limit");
    } else if (inverterData.event1.gridDisconnect) {
        thing->setStateValue(m_inverterErrorStateTypeIds.value(thing->thingClassId()), "Grid disconnect");
    } else if (inverterData.event1.hwTestFailure) {
        thing->setStateValue(m_inverterErrorStateTypeIds.value(thing->thingClassId()), "Hardware test failure");
    } else if (inverterData.event1.manualShutdown) {
        thing->setStateValue(m_inverterErrorStateTypeIds.value(thing->thingClassId()), "Manual shutdown");
    } else if (inverterData.event1.underFrequency) {
        thing->setStateValue(m_inverterErrorStateTypeIds.value(thing->thingClassId()), "Frequency under limit");
    } else if (inverterData.event1.blownStringFuse) {
        thing->setStateValue(m_inverterErrorStateTypeIds.value(thing->thingClassId()), "Blown string fuse on input");
    } else {
        thing->setStateValue(m_inverterErrorStateTypeIds.value(thing->thingClassId()), "None");
    }
}

void IntegrationPluginSolarEdge::onStorageDataReceived(const SunSpecStorage::StorageData &mandatory, const SunSpecStorage::StorageDataOptional &optional)
{
    SunSpecStorage *storage = static_cast<SunSpecStorage *>(sender());
    Thing *thing = m_sunSpecStorages.key(storage);

    if(!thing) return;

    qCDebug(dcSolarEdge()) << "Storage data received";
    qCDebug(dcSolarEdge()) << "   - Setpoint for maximum charge" << mandatory.WChaMax  << "[W]";
    qCDebug(dcSolarEdge()) << "   - Setpoint for maximum charging rate." << mandatory.WChaGra  << "[%]";
    qCDebug(dcSolarEdge()) << "   - Setpoint for maximum discharging rate." << mandatory.WDisChaGra  << "[%]";
    qCDebug(dcSolarEdge()) << "   - Charging enabled" << mandatory.StorCtl_Mod_ChargingEnabled;
    qCDebug(dcSolarEdge()) << "   - Discharging enabled" << mandatory.StorCtl_Mod_DischargingEnabled;
    qCDebug(dcSolarEdge()) << "   - Storage status" << optional.ChaSt;
    qCDebug(dcSolarEdge()) << "   - Currently available energy" << optional.ChaState << "[%]";
    qCDebug(dcSolarEdge()) << "   - Grid charging enabled" << optional.ChaGriSet;

//    thing->setStateValue(m_connectedStateTypeIds.value(thing->thingClassId()), true);
//    thing->setStateValue(solarEdgeStorageChargingRateStateTypeId, mandatory.WChaGra);
//    thing->setStateValue(solarEdgeStorageDischargingRateStateTypeId, mandatory.WDisChaGra);
//    thing->setStateValue(solarEdgeStorageEnableChargingStateTypeId, mandatory.StorCtl_Mod_ChargingEnabled);
//    thing->setStateValue(solarEdgeStorageEnableDischargingStateTypeId, mandatory.StorCtl_Mod_DischargingEnabled);
//    thing->setStateValue(solarEdgeStorageGridChargingStateTypeId, optional.ChaGriSet);

//    bool charging = false;
//    switch (optional.ChaSt) {
//    case SunSpecStorage::ChargingStatusOff:
//        thing->setStateValue(solarEdgeStorageStorageStatusStateTypeId, "Off");
//        break;
//    case SunSpecStorage::ChargingStatusFull:
//        thing->setStateValue(solarEdgeStorageStorageStatusStateTypeId, "Full");
//        break;
//    case SunSpecStorage::ChargingStatusEmpty:
//        thing->setStateValue(solarEdgeStorageStorageStatusStateTypeId, "Empty");
//        break;
//    case SunSpecStorage::ChargingStatusHolding:
//        thing->setStateValue(solarEdgeStorageStorageStatusStateTypeId, "Holding");
//        break;
//    case SunSpecStorage::ChargingStatusTesting:
//        thing->setStateValue(solarEdgeStorageStorageStatusStateTypeId, "Testing");
//        break;
//    case SunSpecStorage::ChargingStatusCharging:
//        thing->setStateValue(solarEdgeStorageStorageStatusStateTypeId, "Charging");
//        break;
//    case SunSpecStorage::ChargingStatusDischarging:
//        thing->setStateValue(solarEdgeStorageStorageStatusStateTypeId, "Discharging");
//        break;
//    };
//    double batteryLevel = optional.ChaState;
//    thing->setStateValue(solarEdgeStorageBatteryLevelStateTypeId, batteryLevel);
//    thing->setStateValue(solarEdgeStorageBatteryCriticalStateTypeId, (batteryLevel < 5 && !charging));
}

void IntegrationPluginSolarEdge::onMeterDataReceived(const SunSpecMeter::MeterData &meterData)
{
    SunSpecMeter *meter = static_cast<SunSpecMeter *>(sender());
    Thing *thing = m_sunSpecMeters.key(meter);
    if (!thing) return;

    qCDebug(dcSolarEdge()) << "Meter data received";
    qCDebug(dcSolarEdge()) << "   - Total AC Current" << meterData.totalAcCurrent << "[A]";
    qCDebug(dcSolarEdge()) << "   - Phase A current" << meterData.phaseACurrent << "[A]";
    qCDebug(dcSolarEdge()) << "   - Phase B current" << meterData.phaseBCurrent << "[A]";
    qCDebug(dcSolarEdge()) << "   - Phase C current" << meterData.phaseCCurrent << "[A]";
    qCDebug(dcSolarEdge()) << "   - Voltage LN" << meterData.voltageLN << "[V]";
    qCDebug(dcSolarEdge()) << "   - Phase voltage AN" << meterData.phaseVoltageAN << "[V]";
    qCDebug(dcSolarEdge()) << "   - Phase voltage BN" << meterData.phaseVoltageBN << "[V]";
    qCDebug(dcSolarEdge()) << "   - Phase voltage CN" << meterData.phaseVoltageCN<< "[V]";
    qCDebug(dcSolarEdge()) << "   - Voltage LL" << meterData.voltageLL << "[V]";
    qCDebug(dcSolarEdge()) << "   - Phase voltage AB" << meterData.phaseVoltageAB << "[V]";
    qCDebug(dcSolarEdge()) << "   - Phase voltage BC" << meterData.phaseVoltageBC << "[V]";
    qCDebug(dcSolarEdge()) << "   - Phase voltage CA" << meterData.phaseVoltageCA << "[V]";
    qCDebug(dcSolarEdge()) << "   - Frequency" << meterData.frequency << "[Hz]";
    qCDebug(dcSolarEdge()) << "   - Total real power" << meterData.totalRealPower << "[W]";
    qCDebug(dcSolarEdge()) << "   - Total real energy exported" << meterData.totalRealEnergyExported<< "[kWH]";
    qCDebug(dcSolarEdge()) << "   - Total real energy imported" << meterData.totalRealEnergyImported<< "[kWH]";

    thing->setStateValue(m_connectedStateTypeIds.value(thing->thingClassId()), true);
    thing->setStateValue(m_frequencyStateTypeIds.value(thing->thingClassId()), meterData.frequency);

    if (thing->thingClassId() == solarEdgeSinglePhaseMeterThingClassId) {
        thing->setStateValue(solarEdgeSinglePhaseMeterPhaseACurrentStateTypeId, meterData.phaseACurrent);
        thing->setStateValue(solarEdgeSinglePhaseMeterCurrentPowerEventTypeId, meterData.totalRealPower);
        thing->setStateValue(solarEdgeSinglePhaseMeterLnACVoltageStateTypeId, meterData.voltageLN);
        thing->setStateValue(solarEdgeSinglePhaseMeterTotalEnergyProducedStateTypeId, meterData.totalRealEnergyExported);
        thing->setStateValue(solarEdgeSinglePhaseMeterTotalEnergyConsumedStateTypeId, meterData.totalRealEnergyImported);

    } else if (thing->thingClassId() == solarEdgeSplitPhaseMeterThingClassId) {
        thing->setStateValue(solarEdgeSplitPhaseMeterPhaseACurrentStateTypeId, meterData.phaseACurrent);
        thing->setStateValue(solarEdgeSplitPhaseMeterPhaseBCurrentStateTypeId, meterData.phaseBCurrent);
        thing->setStateValue(solarEdgeSplitPhaseMeterCurrentPowerEventTypeId, meterData.totalRealPower);
        thing->setStateValue(solarEdgeSplitPhaseMeterLnACVoltageStateTypeId, meterData.voltageLN);
        thing->setStateValue(solarEdgeSplitPhaseMeterPhaseANVoltageStateTypeId, meterData.phaseVoltageAN);
        thing->setStateValue(solarEdgeSplitPhaseMeterPhaseBNVoltageStateTypeId, meterData.phaseVoltageBN);
        thing->setStateValue(solarEdgeSplitPhaseMeterTotalEnergyProducedStateTypeId, meterData.totalRealEnergyExported);
        thing->setStateValue(solarEdgeSplitPhaseMeterTotalEnergyConsumedStateTypeId, meterData.totalRealEnergyImported);

    } else if (thing->thingClassId() == solarEdgeThreePhaseMeterThingClassId) {
        thing->setStateValue(solarEdgeThreePhaseMeterPhaseACurrentStateTypeId, meterData.phaseACurrent);
        thing->setStateValue(solarEdgeThreePhaseMeterPhaseBCurrentStateTypeId, meterData.phaseBCurrent);
        thing->setStateValue(solarEdgeThreePhaseMeterPhaseCCurrentStateTypeId, meterData.phaseCCurrent);
        thing->setStateValue(solarEdgeThreePhaseMeterLnACVoltageStateTypeId, meterData.voltageLN);
        thing->setStateValue(solarEdgeThreePhaseMeterPhaseANVoltageStateTypeId, meterData.phaseVoltageAN);
        thing->setStateValue(solarEdgeThreePhaseMeterPhaseBNVoltageStateTypeId, meterData.phaseVoltageBN);
        thing->setStateValue(solarEdgeThreePhaseMeterPhaseCNVoltageStateTypeId, meterData.phaseVoltageCN);
        thing->setStateValue(solarEdgeThreePhaseMeterCurrentPowerEventTypeId, meterData.totalRealPower);
        thing->setStateValue(solarEdgeThreePhaseMeterTotalEnergyProducedStateTypeId, meterData.totalRealEnergyExported);
        thing->setStateValue(solarEdgeThreePhaseMeterTotalEnergyConsumedStateTypeId, meterData.totalRealEnergyImported);
    }
}
