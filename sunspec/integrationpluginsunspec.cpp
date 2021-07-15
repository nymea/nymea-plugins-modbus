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
#include "integrationpluginsunspec.h"
#include "network/networkdevicediscovery.h"
#include "solaredgebattery.h"

#include <sunspecmodel.h>
#include <models/sunspeccommonmodel.h>

#include <QHostAddress>

IntegrationPluginSunSpec::IntegrationPluginSunSpec()
{

}

void IntegrationPluginSunSpec::init()
{
    // Connection params
    m_connectionIpParamTypeIds.insert(sunspecConnectionThingClassId, sunspecConnectionThingIpAddressParamTypeId);
    m_connectionIpParamTypeIds.insert(solarEdgeConnectionThingClassId, solarEdgeConnectionThingIpAddressParamTypeId);

    m_connectionPortParamTypeIds.insert(sunspecConnectionThingClassId, sunspecConnectionThingPortParamTypeId);
    m_connectionPortParamTypeIds.insert(solarEdgeConnectionThingClassId, solarEdgeConnectionThingPortParamTypeId);

    m_connectionMacAddressParamTypeIds.insert(sunspecConnectionThingClassId, sunspecConnectionThingMacAddressParamTypeId);
    m_connectionMacAddressParamTypeIds.insert(solarEdgeConnectionThingClassId, solarEdgeConnectionThingMacAddressParamTypeId);

    m_connectionSlaveIdParamTypeIds.insert(sunspecConnectionThingClassId, sunspecConnectionThingSlaveIdParamTypeId);
    m_connectionSlaveIdParamTypeIds.insert(solarEdgeConnectionThingClassId, solarEdgeConnectionThingSlaveIdParamTypeId);

    // Connection states
    m_connectionManufacturerStateTypeIds.insert(sunspecConnectionThingClassId, sunspecConnectionManufacturerStateTypeId);
    m_connectionManufacturerStateTypeIds.insert(solarEdgeConnectionThingClassId, solarEdgeConnectionManufacturerStateTypeId);

    m_connectionDeviceModelStateTypeIds.insert(sunspecConnectionThingClassId, sunspecConnectionDeviceModelStateTypeId);
    m_connectionDeviceModelStateTypeIds.insert(solarEdgeConnectionThingClassId, solarEdgeConnectionDeviceModelStateTypeId);

    m_connectionVersionStateTypeIds.insert(sunspecConnectionThingClassId, sunspecConnectionVersionStateTypeId);
    m_connectionVersionStateTypeIds.insert(solarEdgeConnectionThingClassId, solarEdgeConnectionVersionStateTypeId);

    m_connectionSerialNumberStateTypeIds.insert(sunspecConnectionThingClassId, sunspecConnectionSerialNumberStateTypeId);
    m_connectionSerialNumberStateTypeIds.insert(solarEdgeConnectionThingClassId, solarEdgeConnectionSerialNumberStateTypeId);


    // Connected state for all things
    m_connectedStateTypeIds.insert(sunspecConnectionThingClassId, sunspecConnectionConnectedStateTypeId);
    m_connectedStateTypeIds.insert(solarEdgeConnectionThingClassId, solarEdgeConnectionConnectedStateTypeId);
    m_connectedStateTypeIds.insert(solarEdgeBatteryThingClassId, solarEdgeBatteryConnectedStateTypeId);

    m_connectedStateTypeIds.insert(sunspecStorageThingClassId, sunspecStorageConnectedStateTypeId);
    m_connectedStateTypeIds.insert(sunspecSinglePhaseInverterThingClassId, sunspecSinglePhaseInverterConnectedStateTypeId);
    m_connectedStateTypeIds.insert(sunspecSplitPhaseInverterThingClassId, sunspecSplitPhaseInverterConnectedStateTypeId);
    m_connectedStateTypeIds.insert(sunspecThreePhaseInverterThingClassId, sunspecThreePhaseInverterConnectedStateTypeId);
    m_connectedStateTypeIds.insert(sunspecSinglePhaseMeterThingClassId, sunspecSinglePhaseMeterConnectedStateTypeId);
    m_connectedStateTypeIds.insert(sunspecSplitPhaseMeterThingClassId, sunspecSplitPhaseMeterConnectedStateTypeId);
    m_connectedStateTypeIds.insert(sunspecThreePhaseMeterThingClassId, sunspecThreePhaseMeterConnectedStateTypeId);

    // Model id params for sunspec things
    m_modelIdParamTypeIds.insert(sunspecSinglePhaseInverterThingClassId, sunspecSinglePhaseInverterThingModelIdParamTypeId);
    m_modelIdParamTypeIds.insert(sunspecSplitPhaseInverterThingClassId, sunspecSplitPhaseInverterThingModelIdParamTypeId);
    m_modelIdParamTypeIds.insert(sunspecThreePhaseInverterThingClassId, sunspecThreePhaseInverterThingModelIdParamTypeId);
    m_modelIdParamTypeIds.insert(sunspecStorageThingClassId, sunspecStorageThingModelIdParamTypeId);
    m_modelIdParamTypeIds.insert(sunspecSinglePhaseMeterThingClassId, sunspecSinglePhaseMeterThingModelIdParamTypeId);
    m_modelIdParamTypeIds.insert(sunspecSplitPhaseMeterThingClassId, sunspecSplitPhaseMeterThingModelIdParamTypeId);
    m_modelIdParamTypeIds.insert(sunspecThreePhaseMeterThingClassId, sunspecThreePhaseMeterThingModelIdParamTypeId);

    m_modbusAddressParamTypeIds.insert(sunspecSinglePhaseInverterThingClassId, sunspecSinglePhaseInverterThingModbusAddressParamTypeId);
    m_modbusAddressParamTypeIds.insert(sunspecSplitPhaseInverterThingClassId, sunspecSplitPhaseInverterThingModbusAddressParamTypeId);
    m_modbusAddressParamTypeIds.insert(sunspecThreePhaseInverterThingClassId, sunspecThreePhaseInverterThingModbusAddressParamTypeId);
    m_modbusAddressParamTypeIds.insert(sunspecStorageThingClassId, sunspecStorageThingModbusAddressParamTypeId);
    m_modbusAddressParamTypeIds.insert(sunspecSinglePhaseMeterThingClassId, sunspecSinglePhaseMeterThingModbusAddressParamTypeId);
    m_modbusAddressParamTypeIds.insert(sunspecSplitPhaseMeterThingClassId, sunspecSplitPhaseMeterThingModbusAddressParamTypeId);
    m_modbusAddressParamTypeIds.insert(sunspecThreePhaseMeterThingClassId, sunspecThreePhaseMeterThingModbusAddressParamTypeId);

}

void IntegrationPluginSunSpec::discoverThings(ThingDiscoveryInfo *info)
{
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcSunSpec()) << "Failed to discover network devices. The network device discovery is not available.";
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The discovery is not available."));
        return;
    }

    NetworkDeviceDiscoveryReply *discoveryReply = hardwareManager()->networkDeviceDiscovery()->discover();
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
    qCDebug(dcSunSpec()) << "Setup thing" << thing->name();

    if (thing->thingClassId() == sunspecConnectionThingClassId || thing->thingClassId() == solarEdgeConnectionThingClassId) {
        setupConnection(info);
    } else if (thing->thingClassId() == sunspecThreePhaseInverterThingClassId || thing->thingClassId() == sunspecSplitPhaseInverterThingClassId || thing->thingClassId() == sunspecSinglePhaseInverterThingClassId ) {
        Thing *parentThing = myThings().findById(thing->parentId());
        if (parentThing->setupStatus() == Thing::ThingSetupStatusComplete) {
            setupInverter(info);
        } else {
            connect(parentThing, &Thing::setupStatusChanged, info, [this, info] {
                setupInverter(info);
            });
        }
    } else if (thing->thingClassId() == sunspecThreePhaseMeterThingClassId || thing->thingClassId() == sunspecSplitPhaseMeterThingClassId || thing->thingClassId() == sunspecSinglePhaseMeterThingClassId ) {
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
    } else if (m_sunspecThings.contains(thing)) {
        SunSpecThing *sunSpecThing = m_sunspecThings.value(thing);
        sunSpecThing->readBlockData();
    } else {
        Q_ASSERT_X(false, "postSetupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginSunSpec::thingRemoved(Thing *thing)
{
    qCDebug(dcSunSpec()) << "Thing removed" << thing->name();

    if (thing->thingClassId() == sunspecConnectionThingClassId) {
        SunSpecConnection *connection = m_sunSpecConnections.take(thing->id());
        if (connection)
            connection->deleteLater();

    } else if (m_sunspecThings.contains(thing)) {
        SunSpecThing *sunSpecThing = m_sunspecThings.take(thing);
        if (sunSpecThing)
            delete sunSpecThing;

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
        SunSpecStorage *sunSpecStorage = qobject_cast<SunSpecStorage *>(m_sunspecThings.value(thing));
        if (!sunSpecStorage) {
            qWarning(dcSunSpec()) << "Could not find sunspec instance for thing";
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        if (!sunSpecStorage->model()->connection()->connected()) {
            qWarning(dcSunSpec()) << "Could not execute action for" << thing << "because the SunSpec connection is not connected.";
            info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The SunSpec connection is not connected."));
            return;
        }

        sunSpecStorage->executeAction(info);
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

    // First process the common model
    foreach (SunSpecModel *model, connection->models()) {
        if (model->modelId() == SunSpecModelFactory::ModelIdCommon) {
            SunSpecCommonModel *common = qobject_cast<SunSpecCommonModel *>(model);
            qCDebug(dcSunSpec()) << common;
            thing->setStateValue(m_connectedStateTypeIds.value(thing->thingClassId()), true);
            thing->setStateValue(m_connectionManufacturerStateTypeIds.value(thing->thingClassId()), common->manufacturer());
            thing->setStateValue(m_connectionDeviceModelStateTypeIds.value(thing->thingClassId()), common->model());
            thing->setStateValue(m_connectionVersionStateTypeIds.value(thing->thingClassId()), common->version());
            thing->setStateValue(m_connectionSerialNumberStateTypeIds.value(thing->thingClassId()), common->serialNumber());
        }
    }

    // Now process the other models and check if we can create any auto device if not already added
    foreach (SunSpecModel *model, connection->models()) {
        // Make sure we have not added this model yet
        if (sunspecThingAlreadyAdded(model->modelId(), model->modbusStartRegister(), thing->id())) {
            qCDebug(dcSunSpec()) << "Thing already set up for" << model;
            continue;
        }

        switch (model->modelId()) {
        case SunSpecModelFactory::ModelIdCommon:
            // Skip the common model, we already handled this one for each thing model
            break;
        case SunSpecModelFactory::ModelIdInverterSinglePhase:
        case SunSpecModelFactory::ModelIdInverterSinglePhaseFloat: {
            ThingDescriptor descriptor(sunspecSinglePhaseInverterThingClassId, thing->stateValue(m_connectionDeviceModelStateTypeIds.value(thing->thingClassId())).toString() + " " + QT_TR_NOOP("Single Phase Inverter"), "", thing->id());
            ParamList params;
            params.append(Param(sunspecSinglePhaseInverterThingModelIdParamTypeId, model->modelId()));
            params.append(Param(sunspecSinglePhaseInverterThingModbusAddressParamTypeId, model->modbusStartRegister()));
            descriptor.setParams(params);
            emit autoThingsAppeared({descriptor});
            break;
        }
        case SunSpecModelFactory::ModelIdInverterSplitPhase:
        case SunSpecModelFactory::ModelIdInverterSplitPhaseFloat: {
            ThingDescriptor descriptor(sunspecSplitPhaseInverterThingClassId, thing->stateValue(m_connectionDeviceModelStateTypeIds.value(thing->thingClassId())).toString() + " " + QT_TR_NOOP("Split Phase Inverter"), "", thing->id());
            ParamList params;
            params.append(Param(sunspecSplitPhaseInverterThingModelIdParamTypeId, model->modelId()));
            params.append(Param(sunspecSplitPhaseInverterThingModbusAddressParamTypeId, model->modbusStartRegister()));
            descriptor.setParams(params);
            emit autoThingsAppeared({descriptor});
            break;
        }
        case SunSpecModelFactory::ModelIdInverterThreePhase:
        case SunSpecModelFactory::ModelIdInverterThreePhaseFloat: {
            ThingDescriptor descriptor(sunspecThreePhaseInverterThingClassId, thing->stateValue(m_connectionDeviceModelStateTypeIds.value(thing->thingClassId())).toString() + " " + QT_TR_NOOP("Three Phase Inverter"), "", thing->id());
            ParamList params;
            params.append(Param(sunspecThreePhaseInverterThingModelIdParamTypeId, model->modelId()));
            params.append(Param(sunspecThreePhaseInverterThingModbusAddressParamTypeId, model->modbusStartRegister()));
            descriptor.setParams(params);
            emit autoThingsAppeared({descriptor});
            break;
        }
        case SunSpecModelFactory::ModelIdMeterSinglePhase:
        case SunSpecModelFactory::ModelIdMeterSinglePhaseFloat: {
            ThingDescriptor descriptor(sunspecSinglePhaseMeterThingClassId, thing->stateValue(m_connectionDeviceModelStateTypeIds.value(thing->thingClassId())).toString() + " " + QT_TR_NOOP("Single Phase Meter"), "", thing->id());
            ParamList params;
            params.append(Param(sunspecSinglePhaseMeterThingModelIdParamTypeId, model->modelId()));
            params.append(Param(sunspecSinglePhaseMeterThingModbusAddressParamTypeId, model->modbusStartRegister()));
            descriptor.setParams(params);
            emit autoThingsAppeared({descriptor});
            break;
        }
        case SunSpecModelFactory::ModelIdMeterSplitSinglePhaseAbn:
        case SunSpecModelFactory::ModelIdMeterSplitSinglePhaseFloat: {
            ThingDescriptor descriptor(sunspecSplitPhaseMeterThingClassId, thing->stateValue(m_connectionDeviceModelStateTypeIds.value(thing->thingClassId())).toString() + " " + QT_TR_NOOP("Split Phase Meter"), "", thing->id());
            ParamList params;
            params.append(Param(sunspecSplitPhaseMeterThingModelIdParamTypeId, model->modelId()));
            params.append(Param(sunspecSplitPhaseMeterThingModbusAddressParamTypeId, model->modbusStartRegister()));
            descriptor.setParams(params);
            emit autoThingsAppeared({descriptor});
            break;
        }
        case SunSpecModelFactory::ModelIdMeterThreePhase:
        case SunSpecModelFactory::ModelIdDeltaConnectThreePhaseAbcMeter:
        case SunSpecModelFactory::ModelIdMeterThreePhaseWyeConnect:
        case SunSpecModelFactory::ModelIdMeterThreePhaseDeltaConnect: {
            ThingDescriptor descriptor(sunspecThreePhaseMeterThingClassId, thing->stateValue(m_connectionDeviceModelStateTypeIds.value(thing->thingClassId())).toString() + " " + QT_TR_NOOP("Three Phase Meter"), "", thing->id());
            ParamList params;
            params.append(Param(sunspecThreePhaseMeterThingModelIdParamTypeId, model->modelId()));
            params.append(Param(sunspecThreePhaseMeterThingModbusAddressParamTypeId, model->modbusStartRegister()));
            descriptor.setParams(params);
            emit autoThingsAppeared({descriptor});
            break;
        }
        case SunSpecModelFactory::ModelIdStorage: {
            ThingDescriptor descriptor(sunspecStorageThingClassId, thing->stateValue(m_connectionDeviceModelStateTypeIds.value(thing->thingClassId())).toString() + " " + QT_TR_NOOP("Storage"), "", thing->id());
            ParamList params;
            params.append(Param(sunspecStorageThingModelIdParamTypeId, model->modelId()));
            params.append(Param(sunspecStorageThingModbusAddressParamTypeId, model->modbusStartRegister()));
            descriptor.setParams(params);
            emit autoThingsAppeared({descriptor});
            break;
        }
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

    SunSpecConnection *connection = new SunSpecConnection(address, port, slaveId, this);
    connection->setTimeout(configValue(sunSpecPluginTimeoutParamTypeId).toUInt());
    connection->setNumberOfRetries(configValue(sunSpecPluginNumberOfRetriesParamTypeId).toUInt());

    // Update all child things connected states for this connection
    connect(connection, &SunSpecConnection::connectedChanged, thing, [this, connection, thing] (bool connected) {
        qCDebug(dcSunSpec()) << connection << (connected ? "connected" : "disconnected");
        thing->setStateValue(m_connectedStateTypeIds.value(thing->thingClassId()), connected);

        // Update connected state of child things
        foreach (Thing *child, myThings().filterByParentId(thing->id())) {
            child->setStateValue(m_connectedStateTypeIds.value(child->thingClassId()), connected);

            // Refresh childs if connected successfully
            if (connected && m_sunspecThings.contains(child)) {
                m_sunspecThings.value(child)->readBlockData();
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
        qCWarning(dcSunSpec()) << "Could not find SunSpec connection";
        return info->finish(Thing::ThingErrorHardwareNotAvailable);
    }

    // Get the model from the connection
    foreach (SunSpecModel *model, connection->models()) {
        if (model->modelId() == modelId && model->modbusStartRegister() == modbusStartRegister) {
            SunSpecInverter *inverter = new SunSpecInverter(thing, model, this);
            m_sunspecThings.insert(thing, inverter);
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
        qCWarning(dcSunSpec()) << "Could not find SunSpec connection";
        return info->finish(Thing::ThingErrorHardwareNotAvailable);
    }

    // Get the model from the connection
    foreach (SunSpecModel *model, connection->models()) {
        if (model->modelId() == modelId && model->modbusStartRegister() == modbusStartRegister) {
            SunSpecMeter *meter = new SunSpecMeter(thing, model, this);
            m_sunspecThings.insert(thing, meter);
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
        qCWarning(dcSunSpec()) << "Could not find SunSpec connection";
        return info->finish(Thing::ThingErrorHardwareNotAvailable);
    }

    // Get the model from the connection
    foreach (SunSpecModel *model, connection->models()) {
        if (model->modelId() == modelId && model->modbusStartRegister() == modbusStartRegister) {
            SunSpecStorage *storage = new SunSpecStorage(thing, model, this);
            m_sunspecThings.insert(thing, storage);
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
        qCWarning(dcSunSpec()) << "Could not find SunSpec connection for setting up SolarEdge battery";
        return info->finish(Thing::ThingErrorHardwareNotAvailable);
    }

    qCDebug(dcSunSpec()) << "Setting up SolarEdge battery...";
    SolarEdgeBattery *battery = new SolarEdgeBattery(thing, connection, modbusStartRegister, connection);
    connect(battery, &SolarEdgeBattery::initFinished, connection, [=](bool success) {
        if (!success) {
            qCWarning(dcSunSpec()) << "Failed to initialize SolarEdge battery data during setup";
            return info->finish(Thing::ThingErrorHardwareFailure);
        }

        m_sunspecThings.insert(thing, battery);
        info->finish(Thing::ThingErrorNoError);
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
    // Read the battery device id to verify if the battery is connected.
    // Example: start register 0xE100, device id register 0xE140

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, startRegister + 0x40, 1);
    if (QModbusReply *reply = connection->modbusTcpClient()->sendReadRequest(request, connection->slaveId())) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [=]() {

                if (reply->error() != QModbusDevice::NoError) {
                    qCDebug(dcSunSpec()) << "SolarEdge battery seems not to be connected on" << startRegister;
                    return;
                }

                const QModbusDataUnit unit = reply->result();
                if (unit.values().isEmpty()) {
                    return;
                }

                quint16 batteryDeviceId = unit.value(0);
                if (batteryDeviceId == 255) {
                    qCDebug(dcSunSpec()) << "No SolarEdge battery connected on" << startRegister;
                    return;
                }

                // Create a temporary battery object without thing
                qCDebug(dcSunSpec()) << "Found SolarEdge battery on modbus register" << startRegister;
                SolarEdgeBattery *battery = new SolarEdgeBattery(nullptr, connection, startRegister, connection);
                connect(battery, &SolarEdgeBattery::initFinished, connection, [=](bool success) {

                    // Delete this object since we used it only for set up
                    battery->deleteLater();

                    if (!success) {
                        qCWarning(dcSunSpec()) << "Failed to initialize SolarEdge battery on register" << battery->modbusStartRegister();
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
                        params.append(Param(solarEdgeBatteryThingSerialNumberParamTypeId, battery->batteryData().serialNumber));
                        descriptor.setParams(params);
                        emit autoThingsAppeared({descriptor});
                    }

                });

                // Start initializing battery data
                battery->init();
            });
        } else {
            delete reply; // broadcast replies return immediately
            return;
        }
    }
}

void IntegrationPluginSunSpec::onRefreshTimer()
{
    // Update all sunspec thing blocks
    foreach (SunSpecThing *sunSpecThing, m_sunspecThings) {
        if (sunSpecThing->connection()->connected()) {
            sunSpecThing->readBlockData();
        }
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
            m_refreshTimer->startTimer(refreshTime);
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
