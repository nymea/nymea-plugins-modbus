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

#include "integrationpluginsunspec.h"
#include "plugininfo.h"

#include <network/networkdevicediscovery.h>

#include <sunspecmodel.h>
#include <models/sunspecmodelfactory.h>
#include <models/sunspeccommonmodel.h>

// Inverters
#include <models/sunspecinvertersinglephasemodel.h>
#include <models/sunspecinvertersinglephasefloatmodel.h>
#include <models/sunspecinvertersplitphasemodel.h>
#include <models/sunspecinvertersplitphasefloatmodel.h>
#include <models/sunspecinverterthreephasemodel.h>
#include <models/sunspecinverterthreephasefloatmodel.h>

// Meters
#include <models/sunspecmeterthreephasemodel.h>
#include <models/sunspecmetersinglephasemodel.h>
#include <models/sunspecmetersinglephasefloatmodel.h>
#include <models/sunspecmetersplitsinglephaseabnmodel.h>
#include <models/sunspecmetersplitsinglephasefloatmodel.h>
#include <models/sunspecmeterthreephasewyeconnectmodel.h>
#include <models/sunspecmeterthreephasedeltaconnectmodel.h>
#include <models/sunspecdeltaconnectthreephaseabcmetermodel.h>

#include <models/sunspecstoragemodel.h>

#include "solaredgebattery.h"

#include <QHostAddress>

IntegrationPluginSunSpec::IntegrationPluginSunSpec()
{

}

void IntegrationPluginSunSpec::init()
{
    // SunSpec connection params
    m_connectionIpParamTypeIds.insert(sunspecConnectionThingClassId, sunspecConnectionThingIpAddressParamTypeId);
    m_connectionIpParamTypeIds.insert(solarEdgeConnectionThingClassId, solarEdgeConnectionThingIpAddressParamTypeId);

    m_connectionPortParamTypeIds.insert(sunspecConnectionThingClassId, sunspecConnectionThingPortParamTypeId);
    m_connectionPortParamTypeIds.insert(solarEdgeConnectionThingClassId, solarEdgeConnectionThingPortParamTypeId);

    m_connectionMacAddressParamTypeIds.insert(sunspecConnectionThingClassId, sunspecConnectionThingMacAddressParamTypeId);
    m_connectionMacAddressParamTypeIds.insert(solarEdgeConnectionThingClassId, solarEdgeConnectionThingMacAddressParamTypeId);

    m_connectionSlaveIdParamTypeIds.insert(sunspecConnectionThingClassId, sunspecConnectionThingSlaveIdParamTypeId);
    m_connectionSlaveIdParamTypeIds.insert(solarEdgeConnectionThingClassId, solarEdgeConnectionThingSlaveIdParamTypeId);

    // Params for sunspec things
    m_modelIdParamTypeIds.insert(sunspecSinglePhaseInverterThingClassId, sunspecSinglePhaseInverterThingModelIdParamTypeId);
    m_modelIdParamTypeIds.insert(sunspecSplitPhaseInverterThingClassId, sunspecSplitPhaseInverterThingModelIdParamTypeId);
    m_modelIdParamTypeIds.insert(sunspecThreePhaseInverterThingClassId, sunspecThreePhaseInverterThingModelIdParamTypeId);
    m_modelIdParamTypeIds.insert(sunspecStorageThingClassId, sunspecStorageThingModelIdParamTypeId);
    m_modelIdParamTypeIds.insert(sunspecSinglePhaseMeterThingClassId, sunspecSinglePhaseMeterThingModelIdParamTypeId);
    m_modelIdParamTypeIds.insert(sunspecSplitPhaseMeterThingClassId, sunspecSplitPhaseMeterThingModelIdParamTypeId);
    m_modelIdParamTypeIds.insert(sunspecThreePhaseMeterThingClassId, sunspecThreePhaseMeterThingModelIdParamTypeId);

    m_modbusAddressParamTypeIds.insert(solarEdgeBatteryThingClassId, solarEdgeBatteryThingModbusAddressParamTypeId);
    m_modbusAddressParamTypeIds.insert(sunspecSinglePhaseInverterThingClassId, sunspecSinglePhaseInverterThingModbusAddressParamTypeId);
    m_modbusAddressParamTypeIds.insert(sunspecSplitPhaseInverterThingClassId, sunspecSplitPhaseInverterThingModbusAddressParamTypeId);
    m_modbusAddressParamTypeIds.insert(sunspecThreePhaseInverterThingClassId, sunspecThreePhaseInverterThingModbusAddressParamTypeId);
    m_modbusAddressParamTypeIds.insert(sunspecStorageThingClassId, sunspecStorageThingModbusAddressParamTypeId);
    m_modbusAddressParamTypeIds.insert(sunspecSinglePhaseMeterThingClassId, sunspecSinglePhaseMeterThingModbusAddressParamTypeId);
    m_modbusAddressParamTypeIds.insert(sunspecSplitPhaseMeterThingClassId, sunspecSplitPhaseMeterThingModbusAddressParamTypeId);
    m_modbusAddressParamTypeIds.insert(sunspecThreePhaseMeterThingClassId, sunspecThreePhaseMeterThingModbusAddressParamTypeId);

    m_manufacturerParamTypeIds.insert(solarEdgeBatteryThingClassId, solarEdgeBatteryThingManufacturerParamTypeId);
    m_manufacturerParamTypeIds.insert(sunspecSinglePhaseInverterThingClassId, sunspecSinglePhaseInverterThingManufacturerParamTypeId);
    m_manufacturerParamTypeIds.insert(sunspecSplitPhaseInverterThingClassId, sunspecSplitPhaseInverterThingManufacturerParamTypeId);
    m_manufacturerParamTypeIds.insert(sunspecThreePhaseInverterThingClassId, sunspecThreePhaseInverterThingManufacturerParamTypeId);
    m_manufacturerParamTypeIds.insert(sunspecStorageThingClassId, sunspecStorageThingManufacturerParamTypeId);
    m_manufacturerParamTypeIds.insert(sunspecSinglePhaseMeterThingClassId, sunspecSinglePhaseMeterThingManufacturerParamTypeId);
    m_manufacturerParamTypeIds.insert(sunspecSplitPhaseMeterThingClassId, sunspecSplitPhaseMeterThingManufacturerParamTypeId);
    m_manufacturerParamTypeIds.insert(sunspecThreePhaseMeterThingClassId, sunspecThreePhaseMeterThingManufacturerParamTypeId);

    m_deviceModelParamTypeIds.insert(solarEdgeBatteryThingClassId, solarEdgeBatteryThingDeviceModelParamTypeId);
    m_deviceModelParamTypeIds.insert(sunspecSinglePhaseInverterThingClassId, sunspecSinglePhaseInverterThingDeviceModelParamTypeId);
    m_deviceModelParamTypeIds.insert(sunspecSplitPhaseInverterThingClassId, sunspecSplitPhaseInverterThingDeviceModelParamTypeId);
    m_deviceModelParamTypeIds.insert(sunspecThreePhaseInverterThingClassId, sunspecThreePhaseInverterThingDeviceModelParamTypeId);
    m_deviceModelParamTypeIds.insert(sunspecStorageThingClassId, sunspecStorageThingDeviceModelParamTypeId);
    m_deviceModelParamTypeIds.insert(sunspecSinglePhaseMeterThingClassId, sunspecSinglePhaseMeterThingDeviceModelParamTypeId);
    m_deviceModelParamTypeIds.insert(sunspecSplitPhaseMeterThingClassId, sunspecSplitPhaseMeterThingDeviceModelParamTypeId);
    m_deviceModelParamTypeIds.insert(sunspecThreePhaseMeterThingClassId, sunspecThreePhaseMeterThingDeviceModelParamTypeId);

    m_serialNumberParamTypeIds.insert(solarEdgeBatteryThingClassId, solarEdgeBatteryThingSerialNumberParamTypeId);
    m_serialNumberParamTypeIds.insert(sunspecSinglePhaseInverterThingClassId, sunspecSinglePhaseInverterThingSerialNumberParamTypeId);
    m_serialNumberParamTypeIds.insert(sunspecSplitPhaseInverterThingClassId, sunspecSplitPhaseInverterThingSerialNumberParamTypeId);
    m_serialNumberParamTypeIds.insert(sunspecThreePhaseInverterThingClassId, sunspecThreePhaseInverterThingSerialNumberParamTypeId);
    m_serialNumberParamTypeIds.insert(sunspecStorageThingClassId, sunspecStorageThingSerialNumberParamTypeId);
    m_serialNumberParamTypeIds.insert(sunspecSinglePhaseMeterThingClassId, sunspecSinglePhaseMeterThingSerialNumberParamTypeId);
    m_serialNumberParamTypeIds.insert(sunspecSplitPhaseMeterThingClassId, sunspecSplitPhaseMeterThingSerialNumberParamTypeId);
    m_serialNumberParamTypeIds.insert(sunspecThreePhaseMeterThingClassId, sunspecThreePhaseMeterThingSerialNumberParamTypeId);
}

void IntegrationPluginSunSpec::discoverThings(ThingDiscoveryInfo *info)
{
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcSunSpec()) << "Failed to discover network devices. The network device discovery is not available.";
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The discovery is not available."));
        return;
    }

    NetworkDeviceDiscoveryReply *discoveryReply = hardwareManager()->networkDeviceDiscovery()->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        ThingDescriptors descriptors;
        qCDebug(dcSunSpec()) << "Discovery finished. Found" << discoveryReply->networkDeviceInfos().count() << "devices";
        foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {

            // Filter depending on the thing class
            QString title;
            if (info->thingClassId() == solarEdgeConnectionThingClassId) {
                // Filter for solar edge registered mac addresses
                if (!networkDeviceInfo.macAddressManufacturer().toLower().contains("solaredge"))
                    continue;

                if (networkDeviceInfo.hostName().isEmpty()) {
                    title += "SolarEdge (" + networkDeviceInfo.address().toString() + ")";
                } else {
                    title += networkDeviceInfo.address().toString() + " (" + networkDeviceInfo.hostName() + ")";
                }
            } else {
                // Generic or not discoverable sunspec connection, show all network results
                if (networkDeviceInfo.hostName().isEmpty()) {
                    title += networkDeviceInfo.address().toString();
                } else {
                    title += networkDeviceInfo.address().toString() + " (" + networkDeviceInfo.hostName() + ")";
                }
            }

            // Description will be common
            QString description;
            if (networkDeviceInfo.macAddressManufacturer().isEmpty()) {
                description = networkDeviceInfo.macAddress();
            } else {
                description = networkDeviceInfo.macAddress() + " (" + networkDeviceInfo.macAddressManufacturer() + ")";
            }

            qCDebug(dcSunSpec()) << networkDeviceInfo;
            ThingDescriptor descriptor(info->thingClassId(), title, description);

            // Check if we already have set up this device
            Things existingThings = myThings().filterByParam(m_connectionMacAddressParamTypeIds.value(info->thingClassId()), networkDeviceInfo.macAddress());
            if (existingThings.count() == 1) {
                //qCDebug(dcSunSpec()) << "This thing already exists in the system." << existingThings.first() << networkDeviceInfo;
                descriptor.setThingId(existingThings.first()->id());
            }

            ParamList params;
            params << Param(m_connectionIpParamTypeIds.value(info->thingClassId()), networkDeviceInfo.address().toString());
            params << Param(m_connectionMacAddressParamTypeIds.value(info->thingClassId()), networkDeviceInfo.macAddress());
            descriptor.setParams(params);
            info->addThingDescriptor(descriptor);
        }

        // Discovery done
        info->finish(Thing::ThingErrorNoError);
    });
}

void IntegrationPluginSunSpec::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcSunSpec()) << "Setup thing" << thing;
    qCDebug(dcSunSpec()) << thing->params();

    if (thing->thingClassId() == sunspecConnectionThingClassId
            || thing->thingClassId() == solarEdgeConnectionThingClassId) {
        setupConnection(info);

    } else if (thing->thingClassId() == sunspecThreePhaseInverterThingClassId
               || thing->thingClassId() == sunspecSplitPhaseInverterThingClassId
               || thing->thingClassId() == sunspecSinglePhaseInverterThingClassId ) {
        Thing *parentThing = myThings().findById(thing->parentId());
        if (parentThing->setupStatus() == Thing::ThingSetupStatusComplete) {
            setupInverter(info);
        } else {
            connect(parentThing, &Thing::setupStatusChanged, info, [this, info] {
                setupInverter(info);
            });
        }

    } else if (thing->thingClassId() == sunspecThreePhaseMeterThingClassId
               || thing->thingClassId() == sunspecSplitPhaseMeterThingClassId
               || thing->thingClassId() == sunspecSinglePhaseMeterThingClassId ) {
        Thing *parentThing = myThings().findById(thing->parentId());
        if (parentThing->setupStatus() == Thing::ThingSetupStatusComplete) {
            setupMeter(info);
        } else {
            connect(parentThing, &Thing::setupStatusChanged, info, [this, info] {
                setupMeter(info);
            });
        }

    } else if (thing->thingClassId() == sunspecStorageThingClassId) {
        Thing *parentThing = myThings().findById(thing->parentId());
        if (parentThing->setupStatus() == Thing::ThingSetupStatusComplete) {
            setupStorage(info);
        } else {
            connect(parentThing, &Thing::setupStatusChanged, info, [this, info] {
                setupStorage(info);
            });
        }

    } else if (thing->thingClassId() == solarEdgeBatteryThingClassId) {
        Thing *parentThing = myThings().findById(thing->parentId());
        if (parentThing->setupStatus() == Thing::ThingSetupStatusComplete) {
            setupSolarEdgeBattery(info);
        } else {
            connect(parentThing, &Thing::setupStatusChanged, info, [this, info] {
                setupSolarEdgeBattery(info);
            });
        }

    } else {
        Q_ASSERT_X(false, "setupThing", QString("Unhandled thingClassId: %1").arg(info->thing()->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginSunSpec::postSetupThing(Thing *thing)
{
    qCDebug(dcSunSpec()) << "Post setup thing" << thing->name();

    // Create the refresh timer if not already set up
    if (!m_refreshTimer) {
        qCDebug(dcSunSpec()) << "Starting refresh timer";
        int refreshTime = configValue(sunSpecPluginUpdateIntervalParamTypeId).toInt();
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(refreshTime);
        connect(m_refreshTimer, &PluginTimer::timeout, this, &IntegrationPluginSunSpec::onRefreshTimer);
    }

    // Run the autodiscovery on any sunspec connection type
    if (m_sunSpecConnections.contains(thing->id())) {
        SunSpecConnection *connection = m_sunSpecConnections.value(thing->id());
        if (!connection) {
            qCWarning(dcSunSpec()) << "SunSpecConnection not found for" << thing;
            return;
        }

        connection->startDiscovery();

        // Discovery modbus based batteries for solar edge connections
        if (thing->thingClassId() == solarEdgeConnectionThingClassId) {
            searchSolarEdgeBatteries(connection);
        }

    } else if (m_sunSpecThings.contains(thing)) {
        SunSpecThing *sunSpecThing = m_sunSpecThings.value(thing);
        sunSpecThing->readBlockData();
    } else if (m_sunSpecInverters.contains(thing)) {
        m_sunSpecInverters.value(thing)->readBlockData();
    } else if (m_sunSpecMeters.contains(thing)) {
        m_sunSpecMeters.value(thing)->readBlockData();
    } else if (m_sunSpecStorages.contains(thing)) {
        m_sunSpecStorages.value(thing)->readBlockData();
    }  else {
        Q_ASSERT_X(false, "postSetupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginSunSpec::thingRemoved(Thing *thing)
{
    qCDebug(dcSunSpec()) << "Thing removed" << thing->name();

    if (m_sunSpecConnections.contains(thing->id())) {
        SunSpecConnection *connection = m_sunSpecConnections.take(thing->id());
        if (connection)
            connection->deleteLater();

    } else if (m_sunSpecThings.contains(thing)) {
        SunSpecThing *sunSpecThing = m_sunSpecThings.take(thing);
        if (sunSpecThing)
            delete sunSpecThing;

    } else if (m_sunSpecInverters.contains(thing)) {
        m_sunSpecInverters.remove(thing);
    } else if (m_sunSpecMeters.contains(thing)) {
        m_sunSpecMeters.remove(thing);
    } else if (m_sunSpecStorages.contains(thing)) {
        m_sunSpecStorages.remove(thing);
    } else {
        Q_ASSERT_X(false, "thingRemoved", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }

    if (myThings().isEmpty()) {
        qCDebug(dcSunSpec()) << "Stopping refresh timer";
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_refreshTimer);
        m_refreshTimer = nullptr;
    }
}

void IntegrationPluginSunSpec::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    if (thing->thingClassId() == sunspecStorageThingClassId) {
        SunSpecStorageModel *storage = qobject_cast<SunSpecStorageModel *>(m_sunSpecStorages.value(thing));
        if (!storage) {
            qWarning(dcSunSpec()) << "Could not find sunspec model instance for thing" << thing;
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        if (!thing->stateValue(sunspecStorageConnectedStateTypeId).toBool()) {
            qWarning(dcSunSpec()) << "Could not execute action for" << thing << "because the SunSpec connection is not connected.";
            info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The SunSpec connection is not connected."));
            return;
        }

        if (action.actionTypeId() == sunspecStorageGridChargingActionTypeId) {
            bool gridCharging = action.param(sunspecStorageGridChargingActionGridChargingParamTypeId).value().toBool();
            QModbusReply *reply = storage->setChaGriSet(gridCharging ? SunSpecStorageModel::ChagrisetGrid : SunSpecStorageModel::ChagrisetPv);
            if (!reply) {
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }

            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, info, [info, reply]{
                if (reply->error() != QModbusDevice::NoError) {
                    info->finish(Thing::ThingErrorHardwareFailure);
                    return;
                }
                info->finish(Thing::ThingErrorNoError);
            });
        } else if (action.actionTypeId() == sunspecStorageEnableChargingActionTypeId || action.actionTypeId() == sunspecStorageEnableDischargingActionTypeId) {
            SunSpecStorageModel::Storctl_modFlags controlModeFlags;
            if (action.param(sunspecStorageEnableChargingActionEnableChargingParamTypeId).value().toBool())
                controlModeFlags.setFlag(SunSpecStorageModel::Storctl_modCharge);

            if (thing->stateValue(sunspecStorageEnableDischargingStateTypeId).toBool())
                controlModeFlags.setFlag(SunSpecStorageModel::Storctl_modDiScharge);

            QModbusReply *reply = storage->setStorCtlMod(controlModeFlags);
            if (!reply) {
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, info, [info, reply]{
                if (reply->error() != QModbusDevice::NoError) {
                    info->finish(Thing::ThingErrorHardwareFailure);
                    return;
                }
                info->finish(Thing::ThingErrorNoError);
            });
        } else if (action.actionTypeId() == sunspecStorageChargingRateActionTypeId) {
            QModbusReply *reply = storage->setInWRte(action.param(sunspecStorageChargingRateActionChargingRateParamTypeId).value().toInt());
            if (!reply) {
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, info, [info, reply]{
                if (reply->error() != QModbusDevice::NoError) {
                    info->finish(Thing::ThingErrorHardwareFailure);
                    return;
                }
                info->finish(Thing::ThingErrorNoError);
            });
        } else if (action.actionTypeId() == sunspecStorageDischargingRateActionTypeId) {
            QModbusReply *reply = storage->setOutWRte(action.param(sunspecStorageDischargingRateActionDischargingRateParamTypeId).value().toInt());
            if (!reply) {
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, info, [info, reply]{
                if (reply->error() != QModbusDevice::NoError) {
                    info->finish(Thing::ThingErrorHardwareFailure);
                    return;
                }
                info->finish(Thing::ThingErrorNoError);
            });
        } else {
            Q_ASSERT_X(false, "executeAction", QString("Unhandled action: %1").arg(action.actionTypeId().toString()).toUtf8());
        }

    } else {
        Q_ASSERT_X(false, "executeAction", QString("Unhandled thingClassId: %1").arg(info->thing()->thingClassId().toString()).toUtf8());
    }
}

bool IntegrationPluginSunSpec::sunspecThingAlreadyAdded(uint modelId, uint modbusAddress, const ThingId &parentId)
{
    foreach (Thing *thing, myThings()) {
        if (!m_modbusAddressParamTypeIds.contains(thing->thingClassId()))
            continue;

        uint thingModelId = thing->paramValue(m_modelIdParamTypeIds.value(thing->thingClassId())).toUInt();
        uint thingModbusAddress = thing->paramValue(m_modbusAddressParamTypeIds.value(thing->thingClassId())).toUInt();
        if (thingModelId == modelId && thingModbusAddress == modbusAddress && thing->parentId() == parentId) {
            return true;
        }
    }
    return false;
}

void IntegrationPluginSunSpec::processDiscoveryResult(Thing *thing, SunSpecConnection *connection)
{
    qCDebug(dcSunSpec()) << "Processing discovery result from" << thing->name() << connection;

    // Process all models
    checkAutoSetupModels(thing, connection->models());
}

void IntegrationPluginSunSpec::checkAutoSetupModels(Thing *connectionThing, QList<SunSpecModel *> models)
{
    // Process the models and check if we can create any auto device if not already added
    foreach (SunSpecModel *model, models) {
        // Make sure we have not added this model yet
        if (sunspecThingAlreadyAdded(model->modelId(), model->modbusStartRegister(), connectionThing->id())) {
            qCDebug(dcSunSpec()) << "Thing already set up for" << model;
            continue;
        }

        switch (model->modelId()) {
        case SunSpecModelFactory::ModelIdCommon:
            // Skip the common model, we already handled this one for each thing model
            break;
        case SunSpecModelFactory::ModelIdInverterSinglePhase:
        case SunSpecModelFactory::ModelIdInverterSinglePhaseFloat:
            autocreateSunSpecModelThing(sunspecSinglePhaseInverterThingClassId, QT_TR_NOOP("Single Phase Inverter"), connectionThing->id(), model);
            break;
        case SunSpecModelFactory::ModelIdInverterSplitPhase:
        case SunSpecModelFactory::ModelIdInverterSplitPhaseFloat:
            autocreateSunSpecModelThing(sunspecSplitPhaseInverterThingClassId, QT_TR_NOOP("Split Phase Inverter"), connectionThing->id(), model);
            break;
        case SunSpecModelFactory::ModelIdInverterThreePhase:
        case SunSpecModelFactory::ModelIdInverterThreePhaseFloat:
            autocreateSunSpecModelThing(sunspecThreePhaseInverterThingClassId, QT_TR_NOOP("Three Phase Inverter"), connectionThing->id(), model);
            break;
        case SunSpecModelFactory::ModelIdMeterSinglePhase:
        case SunSpecModelFactory::ModelIdMeterSinglePhaseFloat:
            autocreateSunSpecModelThing(sunspecSinglePhaseMeterThingClassId, QT_TR_NOOP("Single Phase Meter"), connectionThing->id(), model);
            break;
        case SunSpecModelFactory::ModelIdMeterSplitSinglePhaseAbn:
        case SunSpecModelFactory::ModelIdMeterSplitSinglePhaseFloat:
            autocreateSunSpecModelThing(sunspecSplitPhaseMeterThingClassId, QT_TR_NOOP("Split Phase Meter"), connectionThing->id(), model);
            break;
        case SunSpecModelFactory::ModelIdMeterThreePhase:
        case SunSpecModelFactory::ModelIdDeltaConnectThreePhaseAbcMeter:
        case SunSpecModelFactory::ModelIdMeterThreePhaseWyeConnect:
        case SunSpecModelFactory::ModelIdMeterThreePhaseDeltaConnect:
            autocreateSunSpecModelThing(sunspecThreePhaseMeterThingClassId, QT_TR_NOOP("Three Phase Meter"), connectionThing->id(), model);
            break;
        case SunSpecModelFactory::ModelIdStorage:
            autocreateSunSpecModelThing(sunspecStorageThingClassId, QT_TR_NOOP("Storage"), connectionThing->id(), model);
            break;
        default:
            qCWarning(dcSunSpec()) << "Plugin has no implementation for detected" << model;
            break;
        }
    }
}

void IntegrationPluginSunSpec::setupConnection(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    QHostAddress address = QHostAddress(info->thing()->paramValue(m_connectionIpParamTypeIds.value(thing->thingClassId())).toString());
    int port = info->thing()->paramValue(m_connectionPortParamTypeIds.value(thing->thingClassId())).toInt();
    int slaveId = info->thing()->paramValue(m_connectionSlaveIdParamTypeIds.value(thing->thingClassId())).toInt();

    if (m_sunSpecConnections.contains(thing->id())) {
        qCDebug(dcSunSpec()) << "Reconfigure SunSpec connection with new address" << address;
        m_sunSpecConnections.take(thing->id())->deleteLater();
    }

    SunSpecConnection *connection = nullptr;
    if (thing->thingClassId() == solarEdgeConnectionThingClassId) {
        // Note: for some reason, solar edge is using big endian register order instead
        // of little endian as specified in sunspec and even solar edge documentation
        connection = new SunSpecConnection(address, port, slaveId, SunSpecDataPoint::ByteOrderBigEndian, this);
    } else {
        SunSpecDataPoint::ByteOrder endianness = SunSpecDataPoint::ByteOrderLittleEndian;
        QString endiannessParam = thing->paramValue("endianness").toString();
        if (endiannessParam == "Big Endian")
            endianness = SunSpecDataPoint::ByteOrderBigEndian;

        connection = new SunSpecConnection(address, port, slaveId, endianness, this);
    }
    connection->setTimeout(configValue(sunSpecPluginTimeoutParamTypeId).toUInt());
    connection->setNumberOfRetries(configValue(sunSpecPluginNumberOfRetriesParamTypeId).toUInt());

    // Update all child things connected states for this connection
    connect(connection, &SunSpecConnection::connectedChanged, thing, [this, connection, thing] (bool connected) {
        if (connected) {
            qCDebug(dcSunSpec()) << connection << "connected";
        } else {
            qCWarning(dcSunSpec()) << connection << "disconnected";
        }

        thing->setStateValue("connected", connected);

        // Update connected state of child things
        foreach (Thing *child, myThings().filterByParentId(thing->id())) {
            child->setStateValue("connected", connected);

            // Refresh childs if connected successfully
            if (connected && m_sunSpecThings.contains(child)) {
                m_sunSpecThings.value(child)->readBlockData();
            }
        }
    });

    // Only during setup
    connect(connection, &SunSpecConnection::connectedChanged, info, [this, connection, info] (bool connected) {
        //qCDebug(dcSunSpec()) << "SunSpec connected changed during setup:" << (connected ? "connected" : "disconnected");
        if (connected) {
            connect(connection, &SunSpecConnection::discoveryFinished, info, [this, connection, info] (bool success) {
                if (success) {
                    qCDebug(dcSunSpec()) << "Discovery finished successfully during setup of" << connection << ". Found SunSpec data on base register" << connection->baseRegister();
                    m_sunSpecConnections.insert(info->thing()->id(), connection);
                    info->finish(Thing::ThingErrorNoError);
                    processDiscoveryResult(info->thing(), connection);
                } else {
                    qCWarning(dcSunSpec()) << "Discovery finished with errors during setup of" << connection;
                    info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("The SunSpec discovery finished with errors. Please make sure this is a SunSpec device."));
                }
            });
            // Perform initial discovery, finish if a valid base register has been found
            connection->startDiscovery();
        } else {
            info->finish(Thing::ThingErrorHardwareNotAvailable);
        }
    });

    connect(info, &ThingSetupInfo::aborted, connection, &SunSpecConnection::deleteLater);
    connect(connection, &SunSpecConnection::destroyed, thing, [this, thing] { m_sunSpecConnections.remove(thing->id()); });

    if (!connection->connectDevice()) {
        qCWarning(dcSunSpec()) << "Error connecting to SunSpec device" << thing->name();
        info->finish(Thing::ThingErrorHardwareNotAvailable);
        connection->deleteLater();
        return;
    }
}

void IntegrationPluginSunSpec::setupInverter(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    uint modelId = thing->paramValue(m_modelIdParamTypeIds.value(thing->thingClassId())).toInt();
    int modbusStartRegister = thing->paramValue(m_modbusAddressParamTypeIds.value(thing->thingClassId())).toInt();
    SunSpecConnection *connection = m_sunSpecConnections.value(thing->parentId());
    if (!connection) {
        qCWarning(dcSunSpec()) << "Could not find SunSpec parent connection for" << thing;
        return info->finish(Thing::ThingErrorHardwareNotAvailable);
    }

    // Get the model from the connection
    foreach (SunSpecModel *model, connection->models()) {
        if (model->modelId() == modelId && model->modbusStartRegister() == modbusStartRegister) {
            connect(model, &SunSpecModel::blockUpdated, this, &IntegrationPluginSunSpec::onInverterBlockUpdated);
            m_sunSpecInverters.insert(thing, model);
            info->finish(Thing::ThingErrorNoError);
            return;
        }
    }
}

void IntegrationPluginSunSpec::setupMeter(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    uint modelId = thing->paramValue(m_modelIdParamTypeIds.value(thing->thingClassId())).toInt();
    int modbusStartRegister = thing->paramValue(m_modbusAddressParamTypeIds.value(thing->thingClassId())).toInt();
    SunSpecConnection *connection = m_sunSpecConnections.value(thing->parentId());
    if (!connection) {
        qCWarning(dcSunSpec()) << "Could not find SunSpec parent connection for" << thing;
        return info->finish(Thing::ThingErrorHardwareNotAvailable);
    }

    // Get the model from the connection
    foreach (SunSpecModel *model, connection->models()) {
        if (model->modelId() == modelId && model->modbusStartRegister() == modbusStartRegister) {
            m_sunSpecMeters.insert(thing, model);
            connect(model, &SunSpecModel::blockUpdated, this, &IntegrationPluginSunSpec::onMeterBlockUpdated);
            info->finish(Thing::ThingErrorNoError);
            return;
        }
    }
}

void IntegrationPluginSunSpec::setupStorage(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    uint modelId = thing->paramValue(m_modelIdParamTypeIds.value(thing->thingClassId())).toInt();
    int modbusStartRegister = thing->paramValue(m_modbusAddressParamTypeIds.value(thing->thingClassId())).toInt();
    SunSpecConnection *connection = m_sunSpecConnections.value(thing->parentId());
    if (!connection) {
        qCWarning(dcSunSpec()) << "Could not find SunSpec parent connection for" << thing;
        return info->finish(Thing::ThingErrorHardwareNotAvailable);
    }

    // Get the model from the connection
    foreach (SunSpecModel *model, connection->models()) {
        if (model->modelId() == modelId && model->modbusStartRegister() == modbusStartRegister) {
            connect(model, &SunSpecModel::blockUpdated, this, &IntegrationPluginSunSpec::onStorageBlockUpdated);
            m_sunSpecStorages.insert(thing, model);
            info->finish(Thing::ThingErrorNoError);
            return;
        }
    }
}

void IntegrationPluginSunSpec::setupSolarEdgeBattery(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    int modbusStartRegister = thing->paramValue(solarEdgeBatteryThingModbusAddressParamTypeId).toUInt();
    SunSpecConnection *connection = m_sunSpecConnections.value(thing->parentId());
    if (!connection) {
        qCWarning(dcSunSpec()) << "Could not find SunSpec parent connection for sunspec battery" << thing;
        return info->finish(Thing::ThingErrorHardwareNotAvailable);
    }

    qCDebug(dcSunSpec()) << "Setting up SolarEdge battery...";
    SolarEdgeBattery *battery = new SolarEdgeBattery(thing, connection, modbusStartRegister, connection);
    connect(battery, &SolarEdgeBattery::initFinished, connection, [=](bool success) {
        if (!success) {
            qCWarning(dcSunSpec()) << "Failed to initialize SolarEdge battery data during setup";
            battery->deleteLater();
            return info->finish(Thing::ThingErrorHardwareFailure);
        }

        m_sunSpecThings.insert(thing, battery);
        connect(battery, &SolarEdgeBattery::blockDataUpdated, this, &IntegrationPluginSunSpec::onSolarEdgeBatteryBlockUpdated);
        info->finish(Thing::ThingErrorNoError);
        // Set up successfully, init done, we are connected for sure
        thing->setStateValue(solarEdgeBatteryConnectedStateTypeId, true);
    });

    // Start initializing battery data
    battery->init();
}

void IntegrationPluginSunSpec::searchSolarEdgeBatteries(SunSpecConnection *connection)
{
    qCDebug(dcSunSpec()) << "Searching for connected SolarEdge batteries...";
    ThingId parentThingId = m_sunSpecConnections.key(connection);
    if (parentThingId.isNull()) {
        qCWarning(dcSunSpec()) << "Could not search for SolarEdge batteries because of find parent ThingId connection for" << connection->hostAddress().toString();
        return;
    }

    // Batteries are not mapped to the sunspec layer, so we have to treat them as normal modbus registers.
    // Read the battery id to verify if the battery is connected.
    // Battery 1: start register 0xE100, device id register 0xE140
    // Battery 2: start register 0xE200, device id register 0xE240
    searchSolarEdgeBattery(connection, parentThingId, 0xE100);
    searchSolarEdgeBattery(connection, parentThingId, 0xE200);
}

void IntegrationPluginSunSpec::searchSolarEdgeBattery(SunSpecConnection *connection, const ThingId &parentThingId, quint16 startRegister)
{
    // Read the battery data, if init failed there is no battery connected,
    // otherwise there is a battery and we should check if we need to setup the battery thing

    // Create a temporary battery object without thing for init
    qCDebug(dcSunSpec()) << "Checking presence of SolarEdge battery on modbus register" << startRegister;
    SolarEdgeBattery *battery = new SolarEdgeBattery(nullptr, connection, startRegister, connection);
    connect(battery, &SolarEdgeBattery::initFinished, connection, [=](bool success) {
        // Delete this object since we used it only for set up
        battery->deleteLater();

        // If init failed, no battery connected
        if (!success) {
            qCDebug(dcSunSpec()) << "No SolarEdge battery connected on register" << startRegister << "- not creating thing.";
            return;
        }

        qCDebug(dcSunSpec()) << "Battery initialized successfully." << battery->batteryData().manufacturerName << battery->batteryData().model;
        // Check if we already created this battery
        if (!myThings().filterByParam(solarEdgeBatteryThingSerialNumberParamTypeId, battery->batteryData().serialNumber).isEmpty()) {
            qCDebug(dcSunSpec()) << "Battery already set up" << battery->batteryData().serialNumber;
        } else {
            // Create new battery device in the system
            ThingDescriptor descriptor(solarEdgeBatteryThingClassId, battery->batteryData().manufacturerName + " - " + battery->batteryData().model, QString(), parentThingId);
            ParamList params;
            params.append(Param(solarEdgeBatteryThingModbusAddressParamTypeId, startRegister));
            params.append(Param(solarEdgeBatteryThingManufacturerParamTypeId, battery->batteryData().manufacturerName));
            params.append(Param(solarEdgeBatteryThingDeviceModelParamTypeId, battery->batteryData().model));
            params.append(Param(solarEdgeBatteryThingSerialNumberParamTypeId, battery->batteryData().serialNumber));
            descriptor.setParams(params);
            emit autoThingsAppeared({descriptor});
        }
    });

    // Try to initialize battery data
    battery->init();
}

double IntegrationPluginSunSpec::calculateSolarEdgePvProduction(Thing *thing, double acPower, double dcPower)
{
    // Note: for solar edge, we need to calculate pv power from battery and AC/DC current depending
    // on loading or discharging of the battery

    double pvPower = acPower;
    Thing *parentThing = myThings().findById(thing->parentId());
    if (parentThing && parentThing->thingClassId() == solarEdgeConnectionThingClassId) {
        SolarEdgeBattery *battery = nullptr;
        // This is a solar edge, let's see if we have a battery for this connection
        foreach (Thing *sunspecThing, m_sunSpecThings.keys()) {
            if (sunspecThing->thingClassId() == solarEdgeBatteryThingClassId && thing->parentId() == parentThing->id()) {
                battery = qobject_cast<SolarEdgeBattery *>(m_sunSpecThings.value(sunspecThing));
                break;
            }
        }

        // Check if we have a meter...
        Thing *meterThing = nullptr;
        foreach (Thing *sunspecThing, m_sunSpecMeters.keys()) {
            if (thing->parentId() == parentThing->id()) {
                meterThing = sunspecThing;
                break;
            }
        }

        // This is a solar edge, let's see if we have a battery for this connection
        if (battery) {
            double meterCurrentPower = meterThing ? meterThing->stateValue("currentPower").toDouble() : 0;
            qCDebug(dcSunSpec()) << "SolarEdge: found battery for inverter: calculate actual PV power from battery DC power and inverter DC power...";
            qCDebug(dcSunSpec()) << "--> SolarEdge: -------------------------------------------------------";
            qCDebug(dcSunSpec()) << "--> SolarEdge: meter power:" << meterCurrentPower << "W";
            qCDebug(dcSunSpec()) << "--> SolarEdge: inverter AC power:" << acPower << "W";
            qCDebug(dcSunSpec()) << "--> SolarEdge: inverter DC power:" << dcPower << "W";
            qCDebug(dcSunSpec()) << "--> SolarEdge: battery DC power:" << battery->batteryData().instantaneousPower << "W";
            qCDebug(dcSunSpec()) << "--> SolarEdge: battery DC state:" << battery->batteryData().batteryStatus;

            switch (battery->batteryData().batteryStatus) {
            case SolarEdgeBattery::BatteryStatus::Charge: {
                // Actual PV = inverter AC power - battery power
                pvPower = acPower - battery->batteryData().instantaneousPower;
                qCDebug(dcSunSpec()) << "--> SolarEdge: calculate actual PV power: inverter AC power - battery power:" << acPower << "-" << battery->batteryData().instantaneousPower << "=" << pvPower;
                break;
            }
            case SolarEdgeBattery::BatteryStatus::Discharge: {
                // Actual PV = inverter DC power + battery power
                pvPower = dcPower - battery->batteryData().instantaneousPower;
                qCDebug(dcSunSpec()) << "--> SolarEdge: calculate actual PV power: inverter DC power - battery power:" << dcPower << "-" << battery->batteryData().instantaneousPower << "=" << pvPower;
                if (pvPower > 0) {
                    qCDebug(dcSunSpec()) << "--> SolarEdge: actual PV power: 0W | loss:" << pvPower << "W";
                    pvPower = 0;
                }
                break;
            }
            default:
                // Idle: No change required, AC power is actual PV energy
                pvPower = acPower;
                break;
            }

            // Calculate self consumption
            double selfConsumption = pvPower - meterCurrentPower + battery->batteryData().instantaneousPower;
            qCDebug(dcSunSpec()) << "--> SolarEdge: self consumption" << selfConsumption << "W";
        }
    }

    return pvPower;
}

void IntegrationPluginSunSpec::autocreateSunSpecModelThing(const ThingClassId &thingClassId, const QString &thingName, const ThingId &parentId, SunSpecModel *model)
{
    ThingDescriptor descriptor(thingClassId);
    descriptor.setParentId(parentId);

    QString finalThingName;
    if (model->commonModelInfo().manufacturerName.isEmpty()) {
        finalThingName = thingName;
    } else {
        finalThingName = model->commonModelInfo().manufacturerName + " " + thingName;
    }

    descriptor.setTitle(finalThingName);

    ParamList params;
    params.append(Param(m_modelIdParamTypeIds.value(descriptor.thingClassId()), model->modelId()));
    params.append(Param(m_modbusAddressParamTypeIds.value(descriptor.thingClassId()), model->modbusStartRegister()));
    params.append(Param(m_manufacturerParamTypeIds.value(descriptor.thingClassId()), model->commonModelInfo().manufacturerName));
    params.append(Param(m_deviceModelParamTypeIds.value(descriptor.thingClassId()), model->commonModelInfo().modelName));
    params.append(Param(m_serialNumberParamTypeIds.value(descriptor.thingClassId()), model->commonModelInfo().serialNumber));
    descriptor.setParams(params);
    emit autoThingsAppeared({descriptor});
}

QString IntegrationPluginSunSpec::getInverterStateString(quint16 state)
{
    // Note: this works because the models have all the same value
    QString stateString;
    switch (state) {
    case SunSpecInverterSinglePhaseModel::StOff:
        stateString = "Off";
        break;
    case SunSpecInverterSinglePhaseModel::StSleeping:
        stateString = "Sleeping";
        break;
    case SunSpecInverterSinglePhaseModel::StStarting:
        stateString = "Starting";
        break;
    case SunSpecInverterSinglePhaseModel::StMppt:
        stateString = "MPPT";
        break;
    case SunSpecInverterSinglePhaseModel::StThrottled:
        stateString = "Throttled";
        break;
    case SunSpecInverterSinglePhaseModel::StShuttingDown:
        stateString = "Shutting down";
        break;
    case SunSpecInverterSinglePhaseModel::StFault:
        stateString = "Fault";
        break;
    case SunSpecInverterSinglePhaseModel::StStandby:
        stateString = "Standby";
        break;
    }
    return stateString;
}

QString IntegrationPluginSunSpec::getInverterErrorString(quint32 flag)
{
    // Note: this works because the models have all the same value
    QStringList errorStrings;
    SunSpecInverterSinglePhaseModel::Evt1Flags event1Flag = static_cast<SunSpecInverterSinglePhaseModel::Evt1Flags>(flag);
    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1GroundFault)) {
        errorStrings.append(QT_TR_NOOP("Ground fault"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1DcOverVolt)) {
        errorStrings.append(QT_TR_NOOP("DC over voltage"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1AcDisconnect)) {
        errorStrings.append(QT_TR_NOOP("AC disconnect"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1DcDisconnect)) {
        errorStrings.append(QT_TR_NOOP("DC disconnect"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1GridDisconnect)) {
        errorStrings.append(QT_TR_NOOP("Grid disconnect"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1CabinetOpen)) {
        errorStrings.append(QT_TR_NOOP("Cabinet open"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1ManualShutdown)) {
        errorStrings.append(QT_TR_NOOP("Manual shutdown"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1OverTemp)) {
        errorStrings.append(QT_TR_NOOP("Over temperature"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1OverFrequency)) {
        errorStrings.append(QT_TR_NOOP("Over frequency"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1UnderFrequency)) {
        errorStrings.append(QT_TR_NOOP("Under frequency"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1AcOverVolt)) {
        errorStrings.append(QT_TR_NOOP("AC over voltage"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1AcUnderVolt)) {
        errorStrings.append(QT_TR_NOOP("AC under voltage"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1BlownStringFuse)) {
        errorStrings.append(QT_TR_NOOP("Blown string fuse"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1UnderTemp)) {
        errorStrings.append(QT_TR_NOOP("Under temperature"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1MemoryLoss)) {
        errorStrings.append(QT_TR_NOOP("Memory loss"));
    }

    if (event1Flag.testFlag(SunSpecInverterSinglePhaseModel::Evt1HwTestFailure)) {
        errorStrings.append(QT_TR_NOOP("Hardware test failure"));
    }

    if (errorStrings.isEmpty()) {
        return QT_TR_NOOP("No error");
    } else {
        return errorStrings.join(", ");
    }

}

double IntegrationPluginSunSpec::fixValueSign(double targetValue, double powerValue)
{
    // Some sunspec devices (i.e. SolarEdge return an absolute value on the phase current.
    // This method makes sure the phase current has the same sign as the phase power value.
    bool sameSign = ((targetValue < 0) == (powerValue < 0));
    return (sameSign ? targetValue : -targetValue);
}


void IntegrationPluginSunSpec::onRefreshTimer()
{
    // Update meters
    foreach (SunSpecModel *model, m_sunSpecMeters.values()) {
        model->readBlockData();
    }

    // Update storage
    foreach (SunSpecModel *model, m_sunSpecStorages.values()) {
        model->readBlockData();
    }

    // Update all other sunspec thing blocks
    foreach (SunSpecThing *sunSpecThing, m_sunSpecThings) {
        if (sunSpecThing->connection()->connected()) {
            sunSpecThing->readBlockData();
        }
    }

    // Update inverters
    foreach (SunSpecModel *model, m_sunSpecInverters.values()) {
        model->readBlockData();
    }
}

void IntegrationPluginSunSpec::onPluginConfigurationChanged(const ParamTypeId &paramTypeId, const QVariant &value)
{
    // Check refresh schedule
    if (paramTypeId == sunSpecPluginUpdateIntervalParamTypeId) {
        qCDebug(dcSunSpec()) << "Update interval has changed" << value.toInt();
        if (m_refreshTimer) {
            int refreshTime = value.toInt();
            m_refreshTimer->stop();
            hardwareManager()->pluginTimerManager()->unregisterTimer(m_refreshTimer);
            m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(refreshTime);
            connect(m_refreshTimer, &PluginTimer::timeout, this, &IntegrationPluginSunSpec::onRefreshTimer);
            m_refreshTimer->start();
        }
    } else if (paramTypeId == sunSpecPluginNumberOfRetriesParamTypeId) {
        qCDebug(dcSunSpec()) << "Updating number of retries" << value.toUInt();
        foreach (SunSpecConnection *connection, m_sunSpecConnections) {
            connection->setNumberOfRetries(value.toUInt());
        }
    } else if (paramTypeId == sunSpecPluginTimeoutParamTypeId) {
        qCDebug(dcSunSpec()) << "Updating timeout" << value.toUInt() << "[ms]";
        foreach (SunSpecConnection *connection, m_sunSpecConnections) {
            connection->setTimeout(value.toUInt());
        }
    } else {
        qCWarning(dcSunSpec()) << "Unknown plugin configuration" << paramTypeId << "Value" << value;
    }
}

void IntegrationPluginSunSpec::onInverterBlockUpdated()
{
    SunSpecModel *model = qobject_cast<SunSpecModel *>(sender());
    Thing *thing = m_sunSpecInverters.key(model);
    if (!thing) return;

    // Get parent thing
    Thing *parentThing = myThings().findById(thing->parentId());
    if (!parentThing)
        return;

    switch (model->modelId()) {
    case SunSpecModelFactory::ModelIdInverterSinglePhase: {
        SunSpecInverterSinglePhaseModel *inverter = qobject_cast<SunSpecInverterSinglePhaseModel *>(model);
        qCDebug(dcSunSpec()) << thing->name() << "block data updated" << inverter;
        thing->setStateValue(sunspecSinglePhaseInverterConnectedStateTypeId, true);
        thing->setStateValue(sunspecSinglePhaseInverterVersionStateTypeId, model->commonModelInfo().versionString);

        // Note: solar edge needs some calculations for the current pv power
        double currentPower = calculateSolarEdgePvProduction(thing, -inverter->watts(), -inverter->dcWatts());
        thing->setStateValue(sunspecSinglePhaseInverterCurrentPowerStateTypeId, currentPower);
        thing->setStateValue(sunspecSinglePhaseInverterTotalEnergyProducedStateTypeId, inverter->wattHours() / 1000.0);
        thing->setStateValue(sunspecSinglePhaseInverterTotalCurrentStateTypeId, inverter->amps());
        thing->setStateValue(sunspecSinglePhaseInverterFrequencyStateTypeId, inverter->hz());
        thing->setStateValue(sunspecSinglePhaseInverterCabinetTemperatureStateTypeId, inverter->cabinetTemperature());
        thing->setStateValue(sunspecSinglePhaseInverterPhaseVoltageStateTypeId, inverter->phaseVoltageAn());
        thing->setStateValue(sunspecSinglePhaseInverterOperatingStateStateTypeId, getInverterStateString(inverter->operatingState()));
        thing->setStateValue(sunspecSinglePhaseInverterErrorStateTypeId, getInverterErrorString(inverter->event1()));
        thing->setStateValue(sunspecSinglePhaseInverterVoltageDcStateTypeId, inverter->dcVoltage());
        thing->setStateValue(sunspecSinglePhaseInverterCurrentDcStateTypeId, inverter->dcAmps());
        thing->setStateValue(sunspecSinglePhaseInverterCurrentPowerDcStateTypeId, -inverter->dcWatts());
        break;
    }
    case SunSpecModelFactory::ModelIdInverterSinglePhaseFloat: {
        SunSpecInverterSinglePhaseFloatModel *inverter = qobject_cast<SunSpecInverterSinglePhaseFloatModel *>(model);
        qCDebug(dcSunSpec()) << thing->name() << "block data updated" << inverter;
        thing->setStateValue(sunspecSinglePhaseInverterConnectedStateTypeId, true);
        thing->setStateValue(sunspecSinglePhaseInverterVersionStateTypeId, model->commonModelInfo().versionString);

        // Note: solar edge needs some calculations for the current pv power
        double currentPower = calculateSolarEdgePvProduction(thing, -inverter->watts(), -inverter->dcWatts());
        thing->setStateValue(sunspecSinglePhaseInverterCurrentPowerStateTypeId, currentPower);
        thing->setStateValue(sunspecSinglePhaseInverterTotalEnergyProducedStateTypeId, inverter->wattHours() / 1000.0);
        thing->setStateValue(sunspecSinglePhaseInverterTotalCurrentStateTypeId, inverter->amps());
        thing->setStateValue(sunspecSinglePhaseInverterFrequencyStateTypeId, inverter->hz());
        thing->setStateValue(sunspecSinglePhaseInverterCabinetTemperatureStateTypeId, inverter->cabinetTemperature());
        thing->setStateValue(sunspecSinglePhaseInverterPhaseVoltageStateTypeId, inverter->phaseVoltageAn());
        thing->setStateValue(sunspecSinglePhaseInverterOperatingStateStateTypeId, getInverterStateString(inverter->operatingState()));
        thing->setStateValue(sunspecSinglePhaseInverterErrorStateTypeId, getInverterErrorString(inverter->event1()));
        thing->setStateValue(sunspecSinglePhaseInverterVoltageDcStateTypeId, inverter->dcVoltage());
        thing->setStateValue(sunspecSinglePhaseInverterCurrentDcStateTypeId, inverter->dcAmps());
        thing->setStateValue(sunspecSinglePhaseInverterCurrentPowerDcStateTypeId, -inverter->dcWatts());
        break;
    }
    case SunSpecModelFactory::ModelIdInverterSplitPhase: {
        SunSpecInverterSplitPhaseModel *inverter = qobject_cast<SunSpecInverterSplitPhaseModel *>(model);
        qCDebug(dcSunSpec()) << thing->name() << "block data updated" << inverter;
        thing->setStateValue(sunspecSplitPhaseInverterConnectedStateTypeId, true);
        thing->setStateValue(sunspecSplitPhaseInverterVersionStateTypeId, model->commonModelInfo().versionString);

        double currentPower = calculateSolarEdgePvProduction(thing, -inverter->watts(), -inverter->dcWatts());
        thing->setStateValue(sunspecSplitPhaseInverterCurrentPowerStateTypeId, currentPower);
        thing->setStateValue(sunspecSplitPhaseInverterTotalEnergyProducedStateTypeId, inverter->wattHours() / 1000.0);
        thing->setStateValue(sunspecSplitPhaseInverterTotalCurrentStateTypeId, inverter->amps());
        thing->setStateValue(sunspecSplitPhaseInverterFrequencyStateTypeId, inverter->hz());
        thing->setStateValue(sunspecSplitPhaseInverterCabinetTemperatureStateTypeId, inverter->cabinetTemperature());
        thing->setStateValue(sunspecSplitPhaseInverterPhaseANVoltageStateTypeId, inverter->phaseVoltageAn());
        thing->setStateValue(sunspecSplitPhaseInverterPhaseBNVoltageStateTypeId, inverter->phaseVoltageBn());
        thing->setStateValue(sunspecSplitPhaseInverterPhaseACurrentStateTypeId, inverter->ampsPhaseA());
        thing->setStateValue(sunspecSplitPhaseInverterPhaseBCurrentStateTypeId, inverter->ampsPhaseB());
        thing->setStateValue(sunspecSplitPhaseInverterOperatingStateStateTypeId, getInverterStateString(inverter->operatingState()));
        thing->setStateValue(sunspecSplitPhaseInverterErrorStateTypeId, getInverterErrorString(inverter->event1()));
        thing->setStateValue(sunspecSplitPhaseInverterVoltageDcStateTypeId, inverter->dcVoltage());
        thing->setStateValue(sunspecSplitPhaseInverterCurrentDcStateTypeId, inverter->dcAmps());
        thing->setStateValue(sunspecSplitPhaseInverterCurrentPowerDcStateTypeId, -inverter->dcWatts());
        break;
    }
    case SunSpecModelFactory::ModelIdInverterSplitPhaseFloat: {
        SunSpecInverterSplitPhaseFloatModel *inverter = qobject_cast<SunSpecInverterSplitPhaseFloatModel *>(model);
        qCDebug(dcSunSpec()) << thing->name() << "block data updated" << inverter;
        thing->setStateValue(sunspecSplitPhaseInverterConnectedStateTypeId, true);
        thing->setStateValue(sunspecSplitPhaseInverterVersionStateTypeId, model->commonModelInfo().versionString);

        double currentPower = calculateSolarEdgePvProduction(thing, -inverter->watts(), -inverter->dcWatts());
        thing->setStateValue(sunspecSplitPhaseInverterCurrentPowerStateTypeId, currentPower);
        thing->setStateValue(sunspecSplitPhaseInverterTotalEnergyProducedStateTypeId, inverter->wattHours() / 1000.0);
        thing->setStateValue(sunspecSplitPhaseInverterTotalCurrentStateTypeId, inverter->amps());
        thing->setStateValue(sunspecSplitPhaseInverterFrequencyStateTypeId, inverter->hz());
        thing->setStateValue(sunspecSplitPhaseInverterCabinetTemperatureStateTypeId, inverter->cabinetTemperature());
        thing->setStateValue(sunspecSplitPhaseInverterPhaseANVoltageStateTypeId, inverter->phaseVoltageAn());
        thing->setStateValue(sunspecSplitPhaseInverterPhaseBNVoltageStateTypeId, inverter->phaseVoltageBn());
        thing->setStateValue(sunspecSplitPhaseInverterPhaseACurrentStateTypeId, inverter->ampsPhaseA());
        thing->setStateValue(sunspecSplitPhaseInverterPhaseBCurrentStateTypeId, inverter->ampsPhaseB());
        thing->setStateValue(sunspecSplitPhaseInverterOperatingStateStateTypeId, getInverterStateString(inverter->operatingState()));
        thing->setStateValue(sunspecSplitPhaseInverterErrorStateTypeId, getInverterErrorString(inverter->event1()));
        thing->setStateValue(sunspecSplitPhaseInverterVoltageDcStateTypeId, inverter->dcVoltage());
        thing->setStateValue(sunspecSplitPhaseInverterCurrentDcStateTypeId, inverter->dcAmps());
        thing->setStateValue(sunspecSplitPhaseInverterCurrentPowerDcStateTypeId, -inverter->dcWatts());
        break;
    }
    case SunSpecModelFactory::ModelIdInverterThreePhase: {
        SunSpecInverterThreePhaseModel *inverter = qobject_cast<SunSpecInverterThreePhaseModel *>(model);
        qCDebug(dcSunSpec()) << thing->name() << "block data updated" << inverter;
        thing->setStateValue(sunspecThreePhaseInverterConnectedStateTypeId, true);
        thing->setStateValue(sunspecThreePhaseInverterVersionStateTypeId, model->commonModelInfo().versionString);

        double currentPower = calculateSolarEdgePvProduction(thing, -inverter->watts(), -inverter->dcWatts());
        thing->setStateValue(sunspecThreePhaseInverterCurrentPowerStateTypeId, currentPower);
        thing->setStateValue(sunspecThreePhaseInverterTotalEnergyProducedStateTypeId, inverter->wattHours() / 1000.0);
        thing->setStateValue(sunspecThreePhaseInverterTotalCurrentStateTypeId, inverter->amps());
        thing->setStateValue(sunspecThreePhaseInverterFrequencyStateTypeId, inverter->hz());
        thing->setStateValue(sunspecThreePhaseInverterCabinetTemperatureStateTypeId, inverter->cabinetTemperature());
        thing->setStateValue(sunspecThreePhaseInverterPhaseANVoltageStateTypeId, inverter->phaseVoltageAn());
        thing->setStateValue(sunspecThreePhaseInverterPhaseBNVoltageStateTypeId, inverter->phaseVoltageBn());
        thing->setStateValue(sunspecThreePhaseInverterPhaseCNVoltageStateTypeId, inverter->phaseVoltageCn());
        thing->setStateValue(sunspecThreePhaseInverterPhaseACurrentStateTypeId, inverter->ampsPhaseA());
        thing->setStateValue(sunspecThreePhaseInverterPhaseBCurrentStateTypeId, inverter->ampsPhaseB());
        thing->setStateValue(sunspecThreePhaseInverterPhaseCCurrentStateTypeId, inverter->ampsPhaseC());
        thing->setStateValue(sunspecThreePhaseInverterOperatingStateStateTypeId, getInverterStateString(inverter->operatingState()));
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, getInverterErrorString(inverter->event1()));
        thing->setStateValue(sunspecThreePhaseInverterVoltageDcStateTypeId, inverter->dcVoltage());
        thing->setStateValue(sunspecThreePhaseInverterCurrentDcStateTypeId, inverter->dcAmps());
        thing->setStateValue(sunspecThreePhaseInverterCurrentPowerDcStateTypeId, -inverter->dcWatts());
        break;
    }
    case SunSpecModelFactory::ModelIdInverterThreePhaseFloat: {
        SunSpecInverterThreePhaseFloatModel *inverter = qobject_cast<SunSpecInverterThreePhaseFloatModel *>(model);
        qCDebug(dcSunSpec()) << thing->name() << "block data updated" << inverter;
        thing->setStateValue(sunspecThreePhaseInverterConnectedStateTypeId, true);
        thing->setStateValue(sunspecThreePhaseInverterVersionStateTypeId, model->commonModelInfo().versionString);

        double currentPower = calculateSolarEdgePvProduction(thing, -inverter->watts(), -inverter->dcWatts());
        thing->setStateValue(sunspecThreePhaseInverterCurrentPowerStateTypeId, currentPower);
        thing->setStateValue(sunspecThreePhaseInverterTotalEnergyProducedStateTypeId, inverter->wattHours() / 1000.0);
        thing->setStateValue(sunspecThreePhaseInverterTotalCurrentStateTypeId, inverter->amps());
        thing->setStateValue(sunspecThreePhaseInverterFrequencyStateTypeId, inverter->hz());
        thing->setStateValue(sunspecThreePhaseInverterCabinetTemperatureStateTypeId, inverter->cabinetTemperature());
        thing->setStateValue(sunspecThreePhaseInverterPhaseANVoltageStateTypeId, inverter->phaseVoltageAn());
        thing->setStateValue(sunspecThreePhaseInverterPhaseBNVoltageStateTypeId, inverter->phaseVoltageBn());
        thing->setStateValue(sunspecThreePhaseInverterPhaseCNVoltageStateTypeId, inverter->phaseVoltageCn());
        thing->setStateValue(sunspecThreePhaseInverterPhaseACurrentStateTypeId, inverter->ampsPhaseA());
        thing->setStateValue(sunspecThreePhaseInverterPhaseBCurrentStateTypeId, inverter->ampsPhaseB());
        thing->setStateValue(sunspecThreePhaseInverterPhaseCCurrentStateTypeId, inverter->ampsPhaseC());
        thing->setStateValue(sunspecThreePhaseInverterOperatingStateStateTypeId, getInverterStateString(inverter->operatingState()));
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, getInverterErrorString(inverter->event1()));
        thing->setStateValue(sunspecThreePhaseInverterVoltageDcStateTypeId, inverter->dcVoltage());
        thing->setStateValue(sunspecThreePhaseInverterCurrentDcStateTypeId, inverter->dcAmps());
        thing->setStateValue(sunspecThreePhaseInverterCurrentPowerDcStateTypeId, -inverter->dcWatts());
        break;
    }
    default:
        qCWarning(dcSunSpec()) << "Received block data from unhandled model" << model;
        break;
    }
}

void IntegrationPluginSunSpec::onMeterBlockUpdated()
{
    SunSpecModel *model = qobject_cast<SunSpecModel *>(sender());
    Thing *thing = m_sunSpecMeters.key(model);
    if (!thing) return;

    // Get parent thing
    Thing *parentThing = myThings().findById(thing->parentId());
    if (!parentThing)
        return;

    switch (model->modelId()) {
    case SunSpecModelFactory::ModelIdMeterSinglePhase: {
        SunSpecMeterSinglePhaseModel *meter = qobject_cast<SunSpecMeterSinglePhaseModel *>(model);
        qCDebug(dcSunSpec()) << thing->name() << "block data updated" << meter;
        thing->setStateValue(sunspecSinglePhaseMeterConnectedStateTypeId, true);
        thing->setStateValue(sunspecSinglePhaseMeterCurrentPowerStateTypeId, -meter->watts());
        thing->setStateValue(sunspecSinglePhaseMeterTotalEnergyProducedStateTypeId, meter->totalWattHoursExported() / 1000.0);
        thing->setStateValue(sunspecSinglePhaseMeterTotalEnergyConsumedStateTypeId, meter->totalWattHoursImported() / 1000.0);
        thing->setStateValue(sunspecSinglePhaseMeterCurrentPhaseAStateTypeId, -meter->ampsPhaseA());
        thing->setStateValue(sunspecSinglePhaseMeterVoltagePhaseAStateTypeId, meter->phaseVoltageAn());
        thing->setStateValue(sunspecSinglePhaseMeterFrequencyStateTypeId, meter->hz());
        thing->setStateValue(sunspecSinglePhaseMeterVersionStateTypeId, model->commonModelInfo().versionString);
        break;
    }
    case SunSpecModelFactory::ModelIdMeterSinglePhaseFloat: {
        SunSpecMeterSinglePhaseFloatModel *meter = qobject_cast<SunSpecMeterSinglePhaseFloatModel *>(model);
        qCDebug(dcSunSpec()) << thing->name() << "block data updated" << meter;
        thing->setStateValue(sunspecSinglePhaseMeterConnectedStateTypeId, true);
        thing->setStateValue(sunspecSinglePhaseMeterCurrentPowerStateTypeId, -meter->watts());
        thing->setStateValue(sunspecSinglePhaseMeterTotalEnergyProducedStateTypeId, meter->totalWattHoursExported() / 1000.0);
        thing->setStateValue(sunspecSinglePhaseMeterTotalEnergyConsumedStateTypeId, meter->totalWattHoursImported() / 1000.0);
        thing->setStateValue(sunspecSinglePhaseMeterCurrentPhaseAStateTypeId, fixValueSign(meter->ampsPhaseA(), -meter->watts()));
        thing->setStateValue(sunspecSinglePhaseMeterVoltagePhaseAStateTypeId, meter->phaseVoltageAn());
        thing->setStateValue(sunspecSinglePhaseMeterFrequencyStateTypeId, meter->hz());
        thing->setStateValue(sunspecSinglePhaseMeterVersionStateTypeId, model->commonModelInfo().versionString);
        break;
    }
    case SunSpecModelFactory::ModelIdMeterSplitSinglePhaseAbn: {
        SunSpecMeterSplitSinglePhaseAbnModel *meter = qobject_cast<SunSpecMeterSplitSinglePhaseAbnModel *>(model);
        qCDebug(dcSunSpec()) << thing->name() << "block data updated" << meter;
        thing->setStateValue(sunspecSplitPhaseMeterConnectedStateTypeId, true);
        thing->setStateValue(sunspecSplitPhaseMeterTotalEnergyProducedStateTypeId, meter->totalWattHoursExported() / 1000.0);
        thing->setStateValue(sunspecSplitPhaseMeterTotalEnergyConsumedStateTypeId, meter->totalWattHoursImported() / 1000.0);
        thing->setStateValue(sunspecSplitPhaseMeterEnergyConsumedPhaseAStateTypeId, meter->totalWattHoursImportedPhaseA() / 1000.0);
        thing->setStateValue(sunspecSplitPhaseMeterEnergyConsumedPhaseBStateTypeId, meter->totalWattHoursImportedPhaseB() / 1000.0);
        thing->setStateValue(sunspecSplitPhaseMeterEnergyProducedPhaseAStateTypeId, meter->totalWattHoursExportedPhaseA() / 1000.0);
        thing->setStateValue(sunspecSplitPhaseMeterEnergyProducedPhaseBStateTypeId, meter->totalWattHoursExportedPhaseB() / 1000.0);
        thing->setStateValue(sunspecSplitPhaseMeterCurrentPowerStateTypeId, -meter->watts());
        thing->setStateValue(sunspecSplitPhaseMeterTotalCurrentStateTypeId, fixValueSign(meter->amps(), -meter->watts()));
        thing->setStateValue(sunspecSplitPhaseMeterCurrentPowerPhaseAStateTypeId, -meter->wattsPhaseA());
        thing->setStateValue(sunspecSplitPhaseMeterCurrentPowerPhaseBStateTypeId, -meter->wattsPhaseB());
        thing->setStateValue(sunspecSplitPhaseMeterCurrentPhaseAStateTypeId, fixValueSign(meter->ampsPhaseA(), -meter->wattsPhaseA()));
        thing->setStateValue(sunspecSplitPhaseMeterCurrentPhaseBStateTypeId, fixValueSign(meter->ampsPhaseB(), -meter->wattsPhaseB()));
        thing->setStateValue(sunspecSplitPhaseMeterLnACVoltageStateTypeId, meter->voltageLn());
        thing->setStateValue(sunspecSplitPhaseMeterVoltagePhaseAStateTypeId, meter->phaseVoltageAn());
        thing->setStateValue(sunspecSplitPhaseMeterVoltagePhaseBStateTypeId, meter->phaseVoltageBn());
        thing->setStateValue(sunspecSplitPhaseMeterFrequencyStateTypeId, meter->hz());
        thing->setStateValue(sunspecSplitPhaseMeterVersionStateTypeId, model->commonModelInfo().versionString);
        break;
    }
    case SunSpecModelFactory::ModelIdMeterSplitSinglePhaseFloat: {
        SunSpecMeterSplitSinglePhaseFloatModel *meter = qobject_cast<SunSpecMeterSplitSinglePhaseFloatModel *>(model);
        qCDebug(dcSunSpec()) << thing->name() << "block data updated" << meter;
        thing->setStateValue(sunspecSplitPhaseMeterConnectedStateTypeId, true);
        thing->setStateValue(sunspecSplitPhaseMeterTotalEnergyProducedStateTypeId, meter->totalWattHoursExported() / 1000.0);
        thing->setStateValue(sunspecSplitPhaseMeterTotalEnergyConsumedStateTypeId, meter->totalWattHoursImported() / 1000.0);
        thing->setStateValue(sunspecSplitPhaseMeterEnergyConsumedPhaseAStateTypeId, meter->totalWattHoursImportedPhaseA() / 1000.0);
        thing->setStateValue(sunspecSplitPhaseMeterEnergyConsumedPhaseBStateTypeId, meter->totalWattHoursImportedPhaseB() / 1000.0);
        thing->setStateValue(sunspecSplitPhaseMeterEnergyProducedPhaseAStateTypeId, meter->totalWattHoursExportedPhaseA() / 1000.0);
        thing->setStateValue(sunspecSplitPhaseMeterEnergyProducedPhaseBStateTypeId, meter->totalWattHoursExportedPhaseB() / 1000.0);
        thing->setStateValue(sunspecSplitPhaseMeterCurrentPowerStateTypeId, -meter->watts());
        thing->setStateValue(sunspecSplitPhaseMeterTotalCurrentStateTypeId, fixValueSign(meter->amps(), -meter->watts()));
        thing->setStateValue(sunspecSplitPhaseMeterCurrentPowerPhaseAStateTypeId, -meter->wattsPhaseA());
        thing->setStateValue(sunspecSplitPhaseMeterCurrentPowerPhaseBStateTypeId, -meter->wattsPhaseB());
        thing->setStateValue(sunspecSplitPhaseMeterCurrentPhaseAStateTypeId, fixValueSign(meter->ampsPhaseA(), -meter->wattsPhaseA()));
        thing->setStateValue(sunspecSplitPhaseMeterCurrentPhaseBStateTypeId, fixValueSign(meter->ampsPhaseB(), -meter->wattsPhaseB()));
        thing->setStateValue(sunspecSplitPhaseMeterLnACVoltageStateTypeId, meter->voltageLn());
        thing->setStateValue(sunspecSplitPhaseMeterVoltagePhaseAStateTypeId, meter->phaseVoltageAn());
        thing->setStateValue(sunspecSplitPhaseMeterVoltagePhaseBStateTypeId, meter->phaseVoltageBn());
        thing->setStateValue(sunspecSplitPhaseMeterFrequencyStateTypeId, meter->hz());
        thing->setStateValue(sunspecSplitPhaseMeterVersionStateTypeId, model->commonModelInfo().versionString);
        break;
    }
    case SunSpecModelFactory::ModelIdMeterThreePhase: {
        SunSpecMeterThreePhaseModel *meter = qobject_cast<SunSpecMeterThreePhaseModel *>(model);
        qCDebug(dcSunSpec()) << thing->name() << "block data updated" << meter;
        thing->setStateValue(sunspecThreePhaseMeterConnectedStateTypeId, true);
        thing->setStateValue(sunspecThreePhaseMeterTotalEnergyProducedStateTypeId, meter->totalWattHoursExported() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterTotalEnergyConsumedStateTypeId, meter->totalWattHoursImported() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyConsumedPhaseAStateTypeId, meter->totalWattHoursImportedPhaseA() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyConsumedPhaseBStateTypeId, meter->totalWattHoursImportedPhaseB() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyConsumedPhaseCStateTypeId, meter->totalWattHoursImportedPhaseC() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyProducedPhaseAStateTypeId, meter->totalWattHoursExportedPhaseA() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyProducedPhaseBStateTypeId, meter->totalWattHoursExportedPhaseB() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyProducedPhaseCStateTypeId, meter->totalWattHoursExportedPhaseC() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterCurrentPowerStateTypeId, -meter->watts());
        thing->setStateValue(sunspecThreePhaseMeterCurrentPowerPhaseAStateTypeId, -meter->wattsPhaseA());
        thing->setStateValue(sunspecThreePhaseMeterCurrentPowerPhaseBStateTypeId, -meter->wattsPhaseB());
        thing->setStateValue(sunspecThreePhaseMeterCurrentPowerPhaseCStateTypeId, -meter->wattsPhaseC());
        thing->setStateValue(sunspecThreePhaseMeterCurrentPhaseAStateTypeId, fixValueSign(meter->ampsPhaseA(), -meter->wattsPhaseA()));
        thing->setStateValue(sunspecThreePhaseMeterCurrentPhaseBStateTypeId, fixValueSign(meter->ampsPhaseB(), -meter->wattsPhaseB()));
        thing->setStateValue(sunspecThreePhaseMeterCurrentPhaseCStateTypeId, fixValueSign(meter->ampsPhaseC(), -meter->wattsPhaseC()));
        thing->setStateValue(sunspecThreePhaseMeterVoltagePhaseAStateTypeId, meter->phaseVoltageAn());
        thing->setStateValue(sunspecThreePhaseMeterVoltagePhaseBStateTypeId, meter->phaseVoltageBn());
        thing->setStateValue(sunspecThreePhaseMeterVoltagePhaseCStateTypeId, meter->phaseVoltageCn());
        thing->setStateValue(sunspecThreePhaseMeterFrequencyStateTypeId, meter->hz());
        thing->setStateValue(sunspecThreePhaseMeterVersionStateTypeId, model->commonModelInfo().versionString);
        break;
    }
    case SunSpecModelFactory::ModelIdDeltaConnectThreePhaseAbcMeter: {
        SunSpecDeltaConnectThreePhaseAbcMeterModel *meter = qobject_cast<SunSpecDeltaConnectThreePhaseAbcMeterModel *>(model);
        qCDebug(dcSunSpec()) << thing->name() << "block data updated" << meter;
        thing->setStateValue(sunspecThreePhaseMeterConnectedStateTypeId, true);
        thing->setStateValue(sunspecThreePhaseMeterTotalEnergyProducedStateTypeId, meter->totalWattHoursExported() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterTotalEnergyConsumedStateTypeId, meter->totalWattHoursImported() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyConsumedPhaseAStateTypeId, meter->totalWattHoursImportedPhaseA() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyConsumedPhaseBStateTypeId, meter->totalWattHoursImportedPhaseB() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyConsumedPhaseCStateTypeId, meter->totalWattHoursImportedPhaseC() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyProducedPhaseAStateTypeId, meter->totalWattHoursExportedPhaseA() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyProducedPhaseBStateTypeId, meter->totalWattHoursExportedPhaseB() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyProducedPhaseCStateTypeId, meter->totalWattHoursExportedPhaseC() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterCurrentPowerStateTypeId, -meter->watts());
        thing->setStateValue(sunspecThreePhaseMeterCurrentPowerPhaseAStateTypeId, -meter->wattsPhaseA());
        thing->setStateValue(sunspecThreePhaseMeterCurrentPowerPhaseBStateTypeId, -meter->wattsPhaseB());
        thing->setStateValue(sunspecThreePhaseMeterCurrentPowerPhaseCStateTypeId, -meter->wattsPhaseC());
        thing->setStateValue(sunspecThreePhaseMeterCurrentPhaseAStateTypeId, fixValueSign(meter->ampsPhaseA(), -meter->wattsPhaseA()));
        thing->setStateValue(sunspecThreePhaseMeterCurrentPhaseBStateTypeId, fixValueSign(meter->ampsPhaseB(), -meter->wattsPhaseB()));
        thing->setStateValue(sunspecThreePhaseMeterCurrentPhaseCStateTypeId, fixValueSign(meter->ampsPhaseC(), -meter->wattsPhaseC()));
        thing->setStateValue(sunspecThreePhaseMeterVoltagePhaseAStateTypeId, meter->phaseVoltageAn());
        thing->setStateValue(sunspecThreePhaseMeterVoltagePhaseBStateTypeId, meter->phaseVoltageBn());
        thing->setStateValue(sunspecThreePhaseMeterVoltagePhaseCStateTypeId, meter->phaseVoltageCn());
        thing->setStateValue(sunspecThreePhaseMeterFrequencyStateTypeId, meter->hz());
        thing->setStateValue(sunspecThreePhaseMeterVersionStateTypeId, model->commonModelInfo().versionString);
        break;
    }
    case SunSpecModelFactory::ModelIdMeterThreePhaseWyeConnect: {
        SunSpecMeterThreePhaseWyeConnectModel *meter = qobject_cast<SunSpecMeterThreePhaseWyeConnectModel *>(model);
        qCDebug(dcSunSpec()) << thing->name() << "block data updated" << meter;
        thing->setStateValue(sunspecThreePhaseMeterConnectedStateTypeId, true);
        thing->setStateValue(sunspecThreePhaseMeterTotalEnergyProducedStateTypeId, meter->totalWattHoursExported() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterTotalEnergyConsumedStateTypeId, meter->totalWattHoursImported() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyConsumedPhaseAStateTypeId, meter->totalWattHoursImportedPhaseA() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyConsumedPhaseBStateTypeId, meter->totalWattHoursImportedPhaseB() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyConsumedPhaseCStateTypeId, meter->totalWattHoursImportedPhaseC() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyProducedPhaseAStateTypeId, meter->totalWattHoursExportedPhaseA() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyProducedPhaseBStateTypeId, meter->totalWattHoursExportedPhaseB() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyProducedPhaseCStateTypeId, meter->totalWattHoursExportedPhaseC() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterCurrentPowerStateTypeId, -meter->watts());
        thing->setStateValue(sunspecThreePhaseMeterCurrentPowerPhaseAStateTypeId, -meter->wattsPhaseA());
        thing->setStateValue(sunspecThreePhaseMeterCurrentPowerPhaseBStateTypeId, -meter->wattsPhaseB());
        thing->setStateValue(sunspecThreePhaseMeterCurrentPowerPhaseCStateTypeId, -meter->wattsPhaseC());
        thing->setStateValue(sunspecThreePhaseMeterCurrentPhaseAStateTypeId, fixValueSign(meter->ampsPhaseA(), -meter->wattsPhaseA()));
        thing->setStateValue(sunspecThreePhaseMeterCurrentPhaseBStateTypeId, fixValueSign(meter->ampsPhaseB(), -meter->wattsPhaseB()));
        thing->setStateValue(sunspecThreePhaseMeterCurrentPhaseCStateTypeId, fixValueSign(meter->ampsPhaseC(), -meter->wattsPhaseC()));
        thing->setStateValue(sunspecThreePhaseMeterVoltagePhaseAStateTypeId, meter->phaseVoltageAn());
        thing->setStateValue(sunspecThreePhaseMeterVoltagePhaseBStateTypeId, meter->phaseVoltageBn());
        thing->setStateValue(sunspecThreePhaseMeterVoltagePhaseCStateTypeId, meter->phaseVoltageCn());
        thing->setStateValue(sunspecThreePhaseMeterFrequencyStateTypeId, meter->hz());
        thing->setStateValue(sunspecThreePhaseMeterVersionStateTypeId, model->commonModelInfo().versionString);
        break;
    }
    case SunSpecModelFactory::ModelIdMeterThreePhaseDeltaConnect: {
        SunSpecMeterThreePhaseDeltaConnectModel *meter = qobject_cast<SunSpecMeterThreePhaseDeltaConnectModel *>(model);
        qCDebug(dcSunSpec()) << thing->name() << "block data updated" << meter;
        thing->setStateValue(sunspecThreePhaseMeterConnectedStateTypeId, true);
        thing->setStateValue(sunspecThreePhaseMeterTotalEnergyProducedStateTypeId, meter->totalWattHoursExported() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterTotalEnergyConsumedStateTypeId, meter->totalWattHoursImported() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyConsumedPhaseAStateTypeId, meter->totalWattHoursImportedPhaseA() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyConsumedPhaseBStateTypeId, meter->totalWattHoursImportedPhaseB() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyConsumedPhaseCStateTypeId, meter->totalWattHoursImportedPhaseC() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyProducedPhaseAStateTypeId, meter->totalWattHoursExportedPhaseA() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyProducedPhaseBStateTypeId, meter->totalWattHoursExportedPhaseB() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterEnergyProducedPhaseCStateTypeId, meter->totalWattHoursExportedPhaseC() / 1000.0);
        thing->setStateValue(sunspecThreePhaseMeterCurrentPowerStateTypeId, -meter->watts());
        thing->setStateValue(sunspecThreePhaseMeterCurrentPowerPhaseAStateTypeId, -meter->wattsPhaseA());
        thing->setStateValue(sunspecThreePhaseMeterCurrentPowerPhaseBStateTypeId, -meter->wattsPhaseB());
        thing->setStateValue(sunspecThreePhaseMeterCurrentPowerPhaseCStateTypeId, -meter->wattsPhaseC());
        thing->setStateValue(sunspecThreePhaseMeterCurrentPhaseAStateTypeId, fixValueSign(meter->ampsPhaseA(), -meter->wattsPhaseA()));
        thing->setStateValue(sunspecThreePhaseMeterCurrentPhaseBStateTypeId, fixValueSign(meter->ampsPhaseB(), -meter->wattsPhaseB()));
        thing->setStateValue(sunspecThreePhaseMeterCurrentPhaseCStateTypeId, fixValueSign(meter->ampsPhaseC(), -meter->wattsPhaseC()));
        thing->setStateValue(sunspecThreePhaseMeterVoltagePhaseAStateTypeId, meter->phaseVoltageAn());
        thing->setStateValue(sunspecThreePhaseMeterVoltagePhaseBStateTypeId, meter->phaseVoltageBn());
        thing->setStateValue(sunspecThreePhaseMeterVoltagePhaseCStateTypeId, meter->phaseVoltageCn());
        thing->setStateValue(sunspecThreePhaseMeterFrequencyStateTypeId, meter->hz());
        thing->setStateValue(sunspecThreePhaseMeterVersionStateTypeId, model->commonModelInfo().versionString);
        break;
    }
    default:
        qCWarning(dcSunSpec()) << "Received block data from unhandled model" << model;
        break;
    }
}

void IntegrationPluginSunSpec::onStorageBlockUpdated()
{
    SunSpecModel *model = qobject_cast<SunSpecModel *>(sender());
    Thing *thing = m_sunSpecStorages.key(model);
    if (!thing) return;

    SunSpecStorageModel *storage = qobject_cast<SunSpecStorageModel *>(model);
    qCDebug(dcSunSpec()) << thing->name() << "block data updated" << storage;

    thing->setStateValue(sunspecStorageConnectedStateTypeId, true);
    thing->setStateValue(sunspecStorageVersionStateTypeId, model->commonModelInfo().versionString);

    thing->setStateValue(sunspecStorageBatteryCriticalStateTypeId, storage->chaState() < 5);
    thing->setStateValue(sunspecStorageBatteryLevelStateTypeId, qRound(storage->chaState()));
    thing->setStateValue(sunspecStorageGridChargingStateTypeId, storage->chaGriSet() == SunSpecStorageModel::ChagrisetGrid);
    thing->setStateValue(sunspecStorageEnableChargingStateTypeId, storage->storCtlMod().testFlag(SunSpecStorageModel::Storctl_modCharge));
    thing->setStateValue(sunspecStorageChargingRateStateTypeId, storage->wChaGra());
    thing->setStateValue(sunspecStorageDischargingRateStateTypeId, storage->wDisChaGra());

    switch (storage->chaSt()) {
    case SunSpecStorageModel::ChastOff:
        thing->setStateValue(sunspecStorageStorageStatusStateTypeId, "Off");
        thing->setStateValue(sunspecStorageChargingStateStateTypeId, "idle");
        break;
    case SunSpecStorageModel::ChastEmpty:
        thing->setStateValue(sunspecStorageStorageStatusStateTypeId, "Empty");
        thing->setStateValue(sunspecStorageChargingStateStateTypeId, "idle");
        break;
    case SunSpecStorageModel::ChastDischarging:
        thing->setStateValue(sunspecStorageStorageStatusStateTypeId, "Discharging");
        thing->setStateValue(sunspecStorageChargingStateStateTypeId, "discharging");
        break;
    case SunSpecStorageModel::ChastCharging:
        thing->setStateValue(sunspecStorageStorageStatusStateTypeId, "Charging");
        thing->setStateValue(sunspecStorageChargingStateStateTypeId, "charging");
        break;
    case SunSpecStorageModel::ChastFull:
        thing->setStateValue(sunspecStorageStorageStatusStateTypeId, "Full");
        thing->setStateValue(sunspecStorageChargingStateStateTypeId, "idle");
        break;
    case SunSpecStorageModel::ChastHolding:
        thing->setStateValue(sunspecStorageStorageStatusStateTypeId, "Holding");
        thing->setStateValue(sunspecStorageChargingStateStateTypeId, "idle");
        break;
    case SunSpecStorageModel::ChastTesting:
        thing->setStateValue(sunspecStorageStorageStatusStateTypeId, "Testing");
        thing->setStateValue(sunspecStorageChargingStateStateTypeId, "idle");
        break;
    }
}

void IntegrationPluginSunSpec::onSolarEdgeBatteryBlockUpdated()
{
    SolarEdgeBattery *battery = qobject_cast<SolarEdgeBattery *>(sender());
    Thing *thing = battery->thing();

    qCDebug(dcSunSpec()) << "SolarEdgeBattery: block updated:" << battery->batteryData();

    QString chargingState = "idle";
    switch (battery->batteryData().batteryStatus) {
    case SolarEdgeBattery::Off:
        chargingState = "idle";
        thing->setStateValue(solarEdgeBatteryBatteryStatusStateTypeId, "Off");
        break;
    case SolarEdgeBattery::Standby:
        chargingState = "idle";
        thing->setStateValue(solarEdgeBatteryBatteryStatusStateTypeId, "Standby");
        break;
    case SolarEdgeBattery::Init:
        chargingState = "idle";
        thing->setStateValue(solarEdgeBatteryBatteryStatusStateTypeId, "Init");
        break;
    case SolarEdgeBattery::Charge:
        chargingState = "charging";
        thing->setStateValue(solarEdgeBatteryBatteryStatusStateTypeId, "Charging");
        break;
    case SolarEdgeBattery::Discharge:
        chargingState = "discharging";
        thing->setStateValue(solarEdgeBatteryBatteryStatusStateTypeId, "Discharging");
        break;
    case SolarEdgeBattery::Fault:
        chargingState = "idle";
        thing->setStateValue(solarEdgeBatteryBatteryStatusStateTypeId, "Fault");
        break;
    case SolarEdgeBattery::Holding:
        chargingState = "idle";
        thing->setStateValue(solarEdgeBatteryBatteryStatusStateTypeId, "Holding");
        break;
    case SolarEdgeBattery::Idle:
        chargingState = "idle";
        thing->setStateValue(solarEdgeBatteryBatteryStatusStateTypeId, "Idle");
        break;
    }

    thing->setStateValue(solarEdgeBatteryBatteryCriticalStateTypeId, (battery->batteryData().stateOfEnergy < 5) && chargingState != "charging");
    thing->setStateValue(solarEdgeBatteryBatteryLevelStateTypeId, battery->batteryData().stateOfEnergy);
    thing->setStateValue(solarEdgeBatteryChargingStateStateTypeId, chargingState);
    thing->setStateValue(solarEdgeBatteryRatedEnergyStateTypeId, battery->batteryData().ratedEnergy / 1000.0); // kWh
    thing->setStateValue(solarEdgeBatteryAverageTemperatureStateTypeId, battery->batteryData().averageTemperature);
    thing->setStateValue(solarEdgeBatteryInstantaneousVoltageStateTypeId, battery->batteryData().instantaneousVoltage);
    thing->setStateValue(solarEdgeBatteryInstantaneousCurrentStateTypeId, battery->batteryData().instantaneousCurrent);
    thing->setStateValue(solarEdgeBatteryCurrentPowerStateTypeId, battery->batteryData().instantaneousPower);
    thing->setStateValue(solarEdgeBatteryMaxEnergyStateTypeId, battery->batteryData().maxEnergy / 1000.0); // kWh
    thing->setStateValue(solarEdgeBatteryCapacityStateTypeId, battery->batteryData().availableEnergy / 1000.0); // kWh
    thing->setStateValue(solarEdgeBatteryStateOfHealthStateTypeId, battery->batteryData().stateOfHealth);
    thing->setStateValue(solarEdgeBatteryVersionStateTypeId, battery->batteryData().firmwareVersion);
}
