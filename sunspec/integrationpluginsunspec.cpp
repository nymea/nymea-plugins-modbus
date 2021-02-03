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

#include <QHostAddress>

IntegrationPluginSunSpec::IntegrationPluginSunSpec()
{

}

void IntegrationPluginSunSpec::init()
{
    m_connectedStateTypeIds.insert(sunspecConnectionThingClassId, sunspecConnectionConnectedStateTypeId);
    m_connectedStateTypeIds.insert(sunspecSinglePhaseInverterThingClassId, sunspecSinglePhaseInverterConnectedStateTypeId);
    m_connectedStateTypeIds.insert(sunspecSplitPhaseInverterThingClassId, sunspecSplitPhaseInverterConnectedStateTypeId);
    m_connectedStateTypeIds.insert(sunspecThreePhaseInverterThingClassId, sunspecThreePhaseInverterConnectedStateTypeId);
    m_connectedStateTypeIds.insert(sunspecStorageThingClassId, sunspecStorageConnectedStateTypeId);
    m_connectedStateTypeIds.insert(sunspecSinglePhaseMeterThingClassId, sunspecSinglePhaseMeterConnectedStateTypeId);
    m_connectedStateTypeIds.insert(sunspecSplitPhaseMeterThingClassId, sunspecSplitPhaseMeterConnectedStateTypeId);
    m_connectedStateTypeIds.insert(sunspecThreePhaseMeterThingClassId, sunspecThreePhaseMeterConnectedStateTypeId);

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

    m_acPowerStateTypeIds.insert(sunspecSinglePhaseInverterThingClassId, sunspecSinglePhaseInverterAcPowerStateTypeId);
    m_acPowerStateTypeIds.insert(sunspecSplitPhaseInverterThingClassId, sunspecSplitPhaseInverterAcPowerStateTypeId);
    m_acPowerStateTypeIds.insert(sunspecThreePhaseInverterThingClassId, sunspecThreePhaseInverterAcPowerStateTypeId);

    m_acEnergyStateTypeIds.insert(sunspecSinglePhaseInverterThingClassId, sunspecSinglePhaseInverterAcEnergyStateTypeId);
    m_acEnergyStateTypeIds.insert(sunspecSplitPhaseInverterThingClassId, sunspecSplitPhaseInverterAcEnergyStateTypeId);
    m_acEnergyStateTypeIds.insert(sunspecThreePhaseInverterThingClassId, sunspecThreePhaseInverterAcEnergyStateTypeId);

    m_inverterOperatingStateTypeIds.insert(sunspecSinglePhaseMeterThingClassId, sunspecSinglePhaseInverterOperatingStateStateTypeId);
    m_inverterOperatingStateTypeIds.insert(sunspecSplitPhaseMeterThingClassId, sunspecSplitPhaseInverterOperatingStateStateTypeId);
    m_inverterOperatingStateTypeIds.insert(sunspecThreePhaseMeterThingClassId, sunspecThreePhaseInverterOperatingStateStateTypeId);

    m_inverterErrorStateTypeIds.insert(sunspecSinglePhaseMeterThingClassId, sunspecSinglePhaseInverterErrorStateTypeId);
    m_inverterErrorStateTypeIds.insert(sunspecSplitPhaseMeterThingClassId, sunspecSplitPhaseInverterErrorStateTypeId);
    m_inverterErrorStateTypeIds.insert(sunspecThreePhaseMeterThingClassId, sunspecThreePhaseInverterErrorStateTypeId);

    m_frequencyStateTypeIds.insert(sunspecSinglePhaseInverterThingClassId, sunspecSinglePhaseInverterFrequencyStateTypeId);
    m_frequencyStateTypeIds.insert(sunspecSplitPhaseInverterThingClassId, sunspecSplitPhaseInverterFrequencyStateTypeId);
    m_frequencyStateTypeIds.insert(sunspecThreePhaseInverterThingClassId, sunspecThreePhaseInverterFrequencyStateTypeId);

    m_frequencyStateTypeIds.insert(sunspecSinglePhaseMeterThingClassId, sunspecSinglePhaseMeterFrequencyStateTypeId);
    m_frequencyStateTypeIds.insert(sunspecSplitPhaseMeterThingClassId, sunspecSplitPhaseMeterFrequencyStateTypeId);
    m_frequencyStateTypeIds.insert(sunspecThreePhaseMeterThingClassId, sunspecThreePhaseMeterFrequencyStateTypeId);
}

void IntegrationPluginSunSpec::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcSunSpec()) << "Setup thing" << thing->name();
    if (thing->thingClassId() == sunspecConnectionThingClassId) {

        QHostAddress address = QHostAddress(info->thing()->paramValue(sunspecConnectionThingIpAddressParamTypeId).toString());
        int port = info->thing()->paramValue(sunspecConnectionThingPortParamTypeId).toInt();
        int slaveId = info->thing()->paramValue(sunspecConnectionThingSlaveIdParamTypeId).toInt();

        if (m_sunSpecConnections.contains(thing->id())) {
            qCDebug(dcSunSpec()) << "Reconfigure SunSpec connection with new address" << address;
            m_sunSpecConnections.take(thing->id())->deleteLater();
        }
        SunSpec *sunSpec;
        sunSpec = new SunSpec(address, port, slaveId, this);
        sunSpec->setTimeout(configValue(sunSpecPluginTimeoutParamTypeId).toUInt());
        sunSpec->setNumberOfRetries(configValue(sunSpecPluginNumberOfRetriesParamTypeId).toUInt());
        m_sunSpecConnections.insert(thing->id(), sunSpec);

        if (!sunSpec->connectModbus()) {
            qCWarning(dcSunSpec()) << "Error connecting to SunSpec device";
            return info->finish(Thing::ThingErrorHardwareNotAvailable);
        }
        connect(sunSpec, &SunSpec::connectionStateChanged, info, [sunSpec, info] (bool status) {
            qCDebug(dcSunSpec()) << "Modbus connection init finished" << status;
            sunSpec->findBaseRegister();
            connect(sunSpec, &SunSpec::foundBaseRegister, info, [info] (uint modbusAddress) {
                qCDebug(dcSunSpec()) << "Found base register" << modbusAddress;
                info->finish(Thing::ThingErrorNoError);
            });
        });

        connect(info, &ThingSetupInfo::aborted, sunSpec, &SunSpec::deleteLater);
        connect(sunSpec, &SunSpec::destroyed, [this, thing] {
            m_sunSpecConnections.remove(thing->id());
        });
        connect(sunSpec, &SunSpec::foundSunSpecModel, this, &IntegrationPluginSunSpec::onFoundSunSpecModel);
        connect(sunSpec, &SunSpec::sunspecModelSearchFinished, this, &IntegrationPluginSunSpec::onSunSpecModelSearchFinished);
        connect(sunSpec, &SunSpec::commonModelReceived, thing, [thing] (const QString &manufacturer, const QString &deviceModel, const QString &serielNumber) {
            thing->setStateValue(sunspecConnectionConnectedStateTypeId, true);
            thing->setStateValue(sunspecConnectionManufacturerStateTypeId, manufacturer);
            thing->setStateValue(sunspecConnectionDeviceModelStateTypeId, deviceModel);
            thing->setStateValue(sunspecConnectionSerialNumberStateTypeId, serielNumber);
        });

    } else if (thing->thingClassId() == sunspecThreePhaseInverterThingClassId ||
               thing->thingClassId() == sunspecSplitPhaseInverterThingClassId ||
               thing->thingClassId() == sunspecSinglePhaseInverterThingClassId ) {

        Thing *parent = myThings().findById(thing->parentId());
        if (parent->setupStatus() == Thing::ThingSetupStatusComplete) {
            setupInverter(info);
        } else {
            connect(parent, &Thing::setupStatusChanged, info, [this, info] {
                setupInverter(info);
            });
        }
    } else if (thing->thingClassId() == sunspecSinglePhaseMeterThingClassId ||
               thing->thingClassId() == sunspecSplitPhaseMeterThingClassId ||
               thing->thingClassId() == sunspecThreePhaseMeterThingClassId) {

        Thing *parent = myThings().findById(thing->parentId());
        if (parent->setupStatus() == Thing::ThingSetupStatusComplete) {
            setupMeter(info);
        } else {
            connect(parent, &Thing::setupStatusChanged, info, [this, info] {
                setupMeter(info);
            });
        }

    } else if (info->thing()->thingClassId() == sunspecStorageThingClassId) {

        Thing *parent = myThings().findById(thing->parentId());
        if (parent->setupStatus() == Thing::ThingSetupStatusComplete) {
            setupStorage(info);
        } else {
            connect(parent, &Thing::setupStatusChanged, info, [this, info] {
                setupStorage(info);
            });
        }
    } else {
        Q_ASSERT_X(false, "setupThing", QString("Unhandled thingClassId: %1").arg(info->thing()->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginSunSpec::postSetupThing(Thing *thing)
{
    qCDebug(dcSunSpec()) << "Post setup thing" << thing->name();

    if (!m_refreshTimer) {
        qCDebug(dcSunSpec()) << "Starting refresh timer";
        int refreshTime = configValue(sunSpecPluginUpdateIntervalParamTypeId).toInt();
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(refreshTime);
        connect(m_refreshTimer, &PluginTimer::timeout, this, &IntegrationPluginSunSpec::onRefreshTimer);
    }

    if (thing->thingClassId() == sunspecConnectionThingClassId) {
        SunSpec *connection = m_sunSpecConnections.value(thing->id());
        if (!connection) {
            qCDebug(dcSunSpec()) << "SunSpecConnection not found";
            return;
        }
        connection->readCommonModel();
        connection->findSunSpecModels(QList<SunSpec::ModelId>()); // Discover all models, without filter

    } else if (thing->thingClassId() == sunspecSinglePhaseInverterThingClassId ||
               thing->thingClassId() == sunspecSplitPhaseInverterThingClassId ||
               thing->thingClassId() == sunspecThreePhaseInverterThingClassId) {

        SunSpecInverter *sunSpecInverter = m_sunSpecInverters.value(thing);
        if (!sunSpecInverter) {
            qCDebug(dcSunSpec()) << "SunSpecInverter not found";
            return;
        }
        sunSpecInverter->getInverterModelDataBlock();

    } else if (thing->thingClassId() == sunspecSinglePhaseMeterThingClassId ||
               thing->thingClassId() == sunspecSplitPhaseMeterThingClassId ||
               thing->thingClassId() == sunspecThreePhaseMeterThingClassId) {

        SunSpecMeter *sunSpecMeter = m_sunSpecMeters.value(thing);
        if (!sunSpecMeter) {
            qCDebug(dcSunSpec()) << "SunSpecMeter not found";
            return;
        }
        sunSpecMeter->getMeterModelDataBlock();

    } else if (thing->thingClassId() == sunspecStorageThingClassId) {
        SunSpecStorage *sunSpecStorage = m_sunSpecStorages.value(thing);
        if (!sunSpecStorage) {
            qCDebug(dcSunSpec()) << "SunSpecStorage not found";
            return;
        }
        sunSpecStorage->getStorageModelDataBlock();

    } else {
        Q_ASSERT_X(false, "postSetupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }

}

void IntegrationPluginSunSpec::thingRemoved(Thing *thing)
{
    qCDebug(dcSunSpec()) << "Thing removed" << thing->name();

    if (thing->thingClassId() == sunspecConnectionThingClassId) {
        SunSpec *sunSpecConnection = m_sunSpecConnections.take(thing->id());
        if (sunSpecConnection)
            sunSpecConnection->deleteLater();

    } else if (thing->thingClassId() == sunspecSinglePhaseInverterThingClassId ||
               thing->thingClassId() == sunspecSplitPhaseInverterThingClassId ||
               thing->thingClassId() == sunspecThreePhaseInverterThingClassId) {
        SunSpecInverter *sunSpecInverter = m_sunSpecInverters.take(thing);
        if (sunSpecInverter)
            sunSpecInverter->deleteLater();

    } else if (thing->thingClassId() == sunspecSinglePhaseMeterThingClassId ||
               thing->thingClassId() == sunspecSplitPhaseMeterThingClassId ||
               thing->thingClassId() == sunspecThreePhaseMeterThingClassId) {
        SunSpecMeter *sunSpecMeter = m_sunSpecMeters.take(thing);
        if (sunSpecMeter)
            sunSpecMeter->deleteLater();

    } else if (thing->thingClassId() == sunspecStorageThingClassId) {
        SunSpecStorage *sunSpecStorage = m_sunSpecStorages.take(thing);
        if (sunSpecStorage)
            sunSpecStorage->deleteLater();

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

    if (thing->thingClassId() == sunspecSinglePhaseInverterThingClassId ||
            thing->thingClassId() == sunspecSplitPhaseInverterThingClassId ||
            thing->thingClassId() == sunspecThreePhaseInverterThingClassId) {

        SunSpecInverter *sunSpecInverter = m_sunSpecInverters.value(thing);
        if (!sunSpecInverter) {
            qWarning(dcSunSpec()) << "Could not find SunSpec inverter for thing" << thing->name();
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }
        info->finish(Thing::ThingErrorActionTypeNotFound);

    } else if (thing->thingClassId() == sunspecSinglePhaseMeterThingClassId ||
               thing->thingClassId() == sunspecSplitPhaseMeterThingClassId ||
               thing->thingClassId() == sunspecThreePhaseMeterThingClassId) {
        info->finish(Thing::ThingErrorActionTypeNotFound);

    } else if (thing->thingClassId() == sunspecStorageThingClassId) {
        SunSpecStorage *sunSpecStorage = m_sunSpecStorages.value(thing);
        if (!sunSpecStorage) {
            qWarning(dcSunSpec()) << "Could not find sunspec instance for thing";
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        if (action.actionTypeId() == sunspecStorageGridChargingActionTypeId) {
            QUuid requestId = sunSpecStorage->setGridCharging(action.param(sunspecStorageGridChargingActionGridChargingParamTypeId).value().toBool());
            if (requestId.isNull()) {
                info->finish(Thing::ThingErrorHardwareFailure);
            } else {
                m_asyncActions.insert(requestId, info);
                connect(info, &ThingActionInfo::aborted, this, [requestId, this] {m_asyncActions.remove(requestId);});
            }
        } else if (action.actionTypeId() == sunspecStorageEnableChargingActionTypeId) {
            bool charging = action.param(sunspecStorageEnableChargingActionEnableChargingParamTypeId).value().toBool();
            bool discharging = thing->stateValue(sunspecStorageEnableDischargingStateTypeId).toBool();
            QUuid requestId = sunSpecStorage->setStorageControlMode(charging, discharging);
            if (requestId.isNull()) {
                info->finish(Thing::ThingErrorHardwareFailure);
            } else {
                m_asyncActions.insert(requestId, info);
                connect(info, &ThingActionInfo::aborted, this, [requestId, this] {m_asyncActions.remove(requestId);});
            }
        } else if (action.actionTypeId() == sunspecStorageChargingRateActionTypeId) {
            QUuid requestId = sunSpecStorage->setChargingRate(action.param(sunspecStorageChargingRateActionChargingRateParamTypeId).value().toInt());
            if (requestId.isNull()) {
                info->finish(Thing::ThingErrorHardwareFailure);
            } else {
                m_asyncActions.insert(requestId, info);
                connect(info, &ThingActionInfo::aborted, this, [requestId, this] {m_asyncActions.remove(requestId);});
            }
        } else if (action.actionTypeId() == sunspecStorageEnableDischargingActionTypeId) {
            bool discharging = action.param(sunspecStorageEnableDischargingActionEnableDischargingParamTypeId).value().toBool();
            bool charging = thing->stateValue(sunspecStorageEnableChargingStateTypeId).toBool();
            QUuid requestId = sunSpecStorage->setStorageControlMode(charging, discharging);
            if (requestId.isNull()) {
                info->finish(Thing::ThingErrorHardwareFailure);
            } else {
                m_asyncActions.insert(requestId, info);
                connect(info, &ThingActionInfo::aborted, this, [requestId, this] {m_asyncActions.remove(requestId);});
            }
        } else if (action.actionTypeId() == sunspecStorageDischargingRateActionTypeId) {
            QUuid requestId = sunSpecStorage->setDischargingRate(action.param(sunspecStorageDischargingRateActionDischargingRateParamTypeId).value().toInt());
            if (requestId.isNull()) {
                info->finish(Thing::ThingErrorHardwareFailure);
            } else {
                m_asyncActions.insert(requestId, info);
                connect(info, &ThingActionInfo::aborted, this, [requestId, this] {m_asyncActions.remove(requestId);});
            }
        } else {
            Q_ASSERT_X(false, "executeAction", QString("Unhandled action: %1").arg(action.actionTypeId().toString()).toUtf8());
        }
    } else {
        Q_ASSERT_X(false, "executeAction", QString("Unhandled thingClassId: %1").arg(info->thing()->thingClassId().toString()).toUtf8());
    }
}

bool IntegrationPluginSunSpec::checkIfThingExists(uint modelId, uint modbusAddress)
{
    Q_FOREACH(Thing *thing, myThings()) {
        if (thing->paramValue(m_modelIdParamTypeIds.value(thing->thingClassId())).toUInt() == modelId &&
                thing->paramValue(m_modbusAddressParamTypeIds.value(thing->thingClassId())).toUInt() == modbusAddress) {
            return true;
        }
    }
    return false;
}

void IntegrationPluginSunSpec::setupInverter(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    uint modelId = thing->paramValue(m_modelIdParamTypeIds.value(thing->thingClassId())).toInt();
    int modbusAddress = thing->paramValue(m_modbusAddressParamTypeIds.value(thing->thingClassId())).toInt();
    SunSpec *connection = m_sunSpecConnections.value(thing->parentId());
    if (!connection) {
        qCWarning(dcSunSpec()) << "Could not find SunSpec connection";
        return info->finish(Thing::ThingErrorHardwareNotAvailable);
    }
    SunSpecInverter *sunSpecInverter = new SunSpecInverter(connection, SunSpec::ModelId(modelId), modbusAddress);
    sunSpecInverter->init();
    connect(sunSpecInverter, &SunSpecInverter::initFinished, info, [this, sunSpecInverter, info] (bool success){
        qCDebug(dcSunSpec()) << "Modbus Inverter init finished, success:" << success;
        if (success) {
            m_sunSpecInverters.insert(info->thing(), sunSpecInverter);
            info->finish(Thing::ThingErrorNoError);
        } else {
            info->finish(Thing::ThingErrorHardwareNotAvailable);
        }
    });

    connect(info, &ThingSetupInfo::aborted, sunSpecInverter, &SunSpecInverter::deleteLater);
    connect(sunSpecInverter, &SunSpecInverter::destroyed, thing, [thing, this] {m_sunSpecInverters.remove(thing);});
    connect(sunSpecInverter, &SunSpecInverter::inverterDataReceived, this, &IntegrationPluginSunSpec::onInverterDataReceived);
}

void IntegrationPluginSunSpec::setupMeter(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    uint modelId = thing->paramValue(m_modelIdParamTypeIds.value(thing->thingClassId())).toInt();
    int modbusAddress = thing->paramValue(m_modbusAddressParamTypeIds.value(thing->thingClassId())).toInt();
    SunSpec *connection = m_sunSpecConnections.value(thing->parentId());
    if (!connection) {
        qCWarning(dcSunSpec()) << "Could not find SunSpec connection";
        return info->finish(Thing::ThingErrorHardwareNotAvailable);
    }
    SunSpecMeter *sunSpecMeter = new SunSpecMeter(connection, SunSpec::ModelId(modelId), modbusAddress);
    sunSpecMeter->init();
    connect(sunSpecMeter, &SunSpecMeter::initFinished, info, [this, sunSpecMeter, info] (bool success){
        qCDebug(dcSunSpec()) << "Modbus meter init finished, success:" << success;
        if (success) {
            m_sunSpecMeters.insert(info->thing(), sunSpecMeter);
            info->finish(Thing::ThingErrorNoError);
        } else {
            info->finish(Thing::ThingErrorHardwareNotAvailable);
        }
    });

    connect(info, &ThingSetupInfo::aborted, sunSpecMeter, &SunSpecMeter::deleteLater);
    connect(sunSpecMeter, &SunSpecMeter::destroyed, thing, [thing, this] {m_sunSpecMeters.remove(thing);});
    connect(sunSpecMeter, &SunSpecMeter::meterDataReceived, this, &IntegrationPluginSunSpec::onMeterDataReceived);
}

void IntegrationPluginSunSpec::setupStorage(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    uint modelId = thing->paramValue(m_modelIdParamTypeIds.value(thing->thingClassId())).toInt();
    int modbusAddress = thing->paramValue(m_modbusAddressParamTypeIds.value(thing->thingClassId())).toInt();
    SunSpec *connection = m_sunSpecConnections.value(thing->parentId());
    if (!connection) {
        qCWarning(dcSunSpec()) << "Could not find SunSpec connection";
        return info->finish(Thing::ThingErrorHardwareNotAvailable);
    }
    SunSpecStorage *sunSpecStorage = new SunSpecStorage(connection, SunSpec::ModelId(modelId), modbusAddress);
    sunSpecStorage->init();
    connect(sunSpecStorage, &SunSpecStorage::initFinished, info, [this, sunSpecStorage, info] (bool success){
        qCDebug(dcSunSpec()) << "Modbus storage init finished, success:" << success;
        if (success) {
            m_sunSpecStorages.insert(info->thing(), sunSpecStorage);
            info->finish(Thing::ThingErrorNoError);
        } else {
            info->finish(Thing::ThingErrorHardwareNotAvailable);
        }
    });

    connect(info, &ThingSetupInfo::aborted, sunSpecStorage, &SunSpecStorage::deleteLater);
    connect(sunSpecStorage, &SunSpecStorage::destroyed, thing, [thing, this] {m_sunSpecStorages.remove(thing);});
    connect(sunSpecStorage, &SunSpecStorage::storageDataReceived, this, &IntegrationPluginSunSpec::onStorageDataReceived);
}

void IntegrationPluginSunSpec::onRefreshTimer()
{
    foreach (SunSpec *connection, m_sunSpecConnections) {
        connection->readCommonModel(); //check connection
    }
    foreach (SunSpecInverter *inverter, m_sunSpecInverters) {
        inverter->getInverterModelDataBlock();
    }
    foreach (SunSpecMeter *meter, m_sunSpecMeters) {
        meter->getMeterModelDataBlock();
    }
    foreach (SunSpecStorage *storage, m_sunSpecStorages) {
        storage->getStorageModelDataBlock();
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
        qCDebug(dcSunSpec()) << "Updating number of retires" << value.toUInt();
        Q_FOREACH(SunSpec *connection, m_sunSpecConnections) {
            connection->setNumberOfRetries(value.toUInt());
        }
    } else if (paramTypeId == sunSpecPluginTimeoutParamTypeId) {
        qCDebug(dcSunSpec()) << "Updating timeout" << value.toUInt() << "[ms]";
        Q_FOREACH(SunSpec *connection, m_sunSpecConnections) {
            connection->setTimeout(value.toUInt());
        }
    } else {
        qCWarning(dcSunSpec()) << "Unknown plugin configuration" << paramTypeId << "Value" << value;
    }
}

void IntegrationPluginSunSpec::onConnectionStateChanged(bool status)
{
    SunSpec *connection = static_cast<SunSpec *>(sender());
    Thing *thing = myThings().findById(m_sunSpecConnections.key(connection));
    if (!thing)
        return;
    qCDebug(dcSunSpec()) << "Connection state changed" << status << thing->name();
    if (thing->thingClassId() == sunspecConnectionConnectedStateTypeId) {
        thing->setStateValue(sunspecConnectionConnectedStateTypeId, status);
    }

    Q_FOREACH(Thing *child, myThings().filterByParentId(thing->id())) {
        child->setStateValue(m_connectedStateTypeIds.value(thing->thingClassId()), status);
    }
}

void IntegrationPluginSunSpec::onFoundSunSpecModel(SunSpec::ModelId modelId, int modbusStartRegister)
{
    SunSpec *connection = static_cast<SunSpec *>(sender());
    Thing *thing = myThings().findById(m_sunSpecConnections.key(connection));
    if (!thing) {
        qCWarning(dcSunSpec()) << "Thing not found for SunSpec connection" << connection->deviceModel() << connection->serialNumber();
        return;
    }

    qCDebug(dcSunSpec()) << "On model received" << modelId << "start register" << modbusStartRegister << "Thing:" << thing->name();
    if (checkIfThingExists(modelId, modbusStartRegister)) {
        return;
    }
    QString model = thing->stateValue(sunspecConnectionDeviceModelStateTypeId).toString();
    switch (modelId) {
    case SunSpec::ModelId::ModelIdInverterSinglePhase:
    case SunSpec::ModelId::ModelIdInverterSinglePhaseFloat: {
        ThingDescriptor descriptor(sunspecSinglePhaseInverterThingClassId, model+tr(" single phase inverter"), "", thing->id());
        ParamList params;
        params.append(Param(sunspecSinglePhaseInverterThingModelIdParamTypeId, modelId));
        params.append(Param(sunspecSinglePhaseInverterThingModbusAddressParamTypeId, modbusStartRegister));
        descriptor.setParams(params);
        emit autoThingsAppeared({descriptor});
    } break;
    case SunSpec::ModelId::ModelIdInverterSplitPhase:
    case SunSpec::ModelId::ModelIdInverterSplitPhaseFloat: {
        ThingDescriptor descriptor(sunspecSplitPhaseInverterThingClassId, model+tr(" split phase inverter"), "", thing->id());
        ParamList params;
        params.append(Param(sunspecSplitPhaseInverterThingModelIdParamTypeId, modelId));
        params.append(Param(sunspecSplitPhaseInverterThingModbusAddressParamTypeId, modbusStartRegister));
        descriptor.setParams(params);
        emit autoThingsAppeared({descriptor});
    } break;
    case SunSpec::ModelId::ModelIdInverterThreePhase:
    case SunSpec::ModelId::ModelIdInverterThreePhaseFloat: {
        ThingDescriptor descriptor(sunspecThreePhaseInverterThingClassId, model+tr(" three phase inverter"), "", thing->id());
        ParamList params;
        params.append(Param(sunspecThreePhaseInverterThingModelIdParamTypeId, modelId));
        params.append(Param(sunspecThreePhaseInverterThingModbusAddressParamTypeId, modbusStartRegister));
        descriptor.setParams(params);
        emit autoThingsAppeared({descriptor});
    } break;

    case SunSpec::ModelIdSinglePhaseMeter:
    case SunSpec::ModelIdSinglePhaseMeterFloat: {
        ThingDescriptor descriptor(sunspecSinglePhaseMeterThingClassId, model+tr(" meter"), "", thing->id());
        ParamList params;
        params.append(Param(sunspecSinglePhaseMeterThingModelIdParamTypeId, modelId));
        params.append(Param(sunspecSinglePhaseMeterThingModbusAddressParamTypeId, modbusStartRegister));
        descriptor.setParams(params);
        emit autoThingsAppeared({descriptor});
    } break;
    case SunSpec::ModelIdSplitSinglePhaseMeter:
    case SunSpec::ModelIdSplitSinglePhaseMeterFloat: {
        ThingDescriptor descriptor(sunspecSplitPhaseMeterThingClassId, model+tr(" meter"), "", thing->id());
        ParamList params;
        params.append(Param(sunspecSplitPhaseMeterThingModelIdParamTypeId, modelId));
        params.append(Param(sunspecSplitPhaseMeterThingModbusAddressParamTypeId, modbusStartRegister));
        descriptor.setParams(params);
        emit autoThingsAppeared({descriptor});
    } break;
    case SunSpec::ModelIdWyeConnectThreePhaseMeter:
    case SunSpec::ModelIdDeltaConnectThreePhaseMeter:
    case SunSpec::ModelIdWyeConnectThreePhaseMeterFloat:
    case SunSpec::ModelIdDeltaConnectThreePhaseMeterFloat: {
        ThingDescriptor descriptor(sunspecThreePhaseMeterThingClassId, model+" meter", "", thing->id());
        ParamList params;
        params.append(Param(sunspecThreePhaseMeterThingModelIdParamTypeId, modelId));
        params.append(Param(sunspecThreePhaseMeterThingModbusAddressParamTypeId, modbusStartRegister));
        descriptor.setParams(params);
        emit autoThingsAppeared({descriptor});
    } break;
    case SunSpec::ModelIdStorage: {
        ThingDescriptor descriptor(sunspecStorageThingClassId, model+" storage", "", thing->id());
        ParamList params;
        params.append(Param(sunspecStorageThingModelIdParamTypeId, modelId));
        params.append(Param(sunspecStorageThingModbusAddressParamTypeId, modbusStartRegister));
        descriptor.setParams(params);
        emit autoThingsAppeared({descriptor});
    } break;
    default:
        qCDebug(dcSunSpec()) << "Block Id not handled";
    }
}

void IntegrationPluginSunSpec::onSunSpecModelSearchFinished(const QHash<SunSpec::ModelId, int> &modelIds)
{
    SunSpec *connection = static_cast<SunSpec *>(sender());
    Thing *thing = myThings().findById(m_sunSpecConnections.key(connection));
    if (!thing) {
        qCWarning(dcSunSpec()) << "Thing not found for SunSpec connection" << connection->deviceModel() << connection->serialNumber();
        return;
    }
    qCDebug(dcSunSpec()) << "On sunspec model search finished, models:" << modelIds.count();
}

void IntegrationPluginSunSpec::onWriteRequestExecuted(QUuid requestId, bool success)
{
    qCDebug(dcSunSpec()) << "Write request executed" << requestId << success;
}

void IntegrationPluginSunSpec::onWriteRequestError(QUuid requestId, const QString &error)
{
    Q_UNUSED(requestId)
    Q_UNUSED(error)
    qCDebug(dcSunSpec()) << "Write request error" << error;
}

void IntegrationPluginSunSpec::onInverterDataReceived(const SunSpecInverter::InverterData &inverterData)
{
    SunSpecInverter *inverter = static_cast<SunSpecInverter *>(sender());
    Thing *thing = m_sunSpecInverters.key(inverter);

    if(!thing) {
        return;
    }

    qCDebug(dcSunSpec()) << "Inverter data received";
    qCDebug(dcSunSpec()) << "   - Total AC Current" << inverterData.acCurrent << "[A]";
    qCDebug(dcSunSpec()) << "   - Phase A Current" << inverterData.phaseACurrent << "[A]";
    qCDebug(dcSunSpec()) << "   - Phase B Current" << inverterData.phaseBCurrent << "[A]";
    qCDebug(dcSunSpec()) << "   - Phase C Current" << inverterData.phaseCCurrent << "[A]";
    qCDebug(dcSunSpec()) << "   - Phase voltage AB" << inverterData.phaseVoltageAB << "[V]";
    qCDebug(dcSunSpec()) << "   - Phase voltage BC" << inverterData.phaseVoltageBC << "[V]";
    qCDebug(dcSunSpec()) << "   - Phase voltage CA" << inverterData.phaseVoltageCA << "[V]";
    qCDebug(dcSunSpec()) << "   - Phase voltage AN" << inverterData.phaseVoltageAN << "[V]";
    qCDebug(dcSunSpec()) << "   - Phase voltage BN" << inverterData.phaseVoltageBN << "[V]";
    qCDebug(dcSunSpec()) << "   - Phase voltage CN" << inverterData.phaseVoltageCN << "[V]";
    qCDebug(dcSunSpec()) << "   - AC Power" << inverterData.acPower << "[W]";
    qCDebug(dcSunSpec()) << "   - Line frequency" << inverterData.lineFrequency << "[Hz]";
    qCDebug(dcSunSpec()) << "   - AC energy" << inverterData.acEnergy << "[Wh]";
    qCDebug(dcSunSpec()) << "   - Cabinet temperature" << inverterData.cabinetTemperature << "[°C]";
    qCDebug(dcSunSpec()) << "   - Operating state" << inverterData.operatingState;

    thing->setStateValue(m_connectedStateTypeIds.value(thing->thingClassId()), true);
    thing->setStateValue(m_acPowerStateTypeIds.value(thing->thingClassId()), inverterData.acPower/1000.00);
    thing->setStateValue(m_acEnergyStateTypeIds.value(thing->thingClassId()), inverterData.acEnergy/1000.00);
    thing->setStateValue(m_frequencyStateTypeIds.value(thing->thingClassId()), inverterData.lineFrequency);

    thing->setStateValue(sunspecThreePhaseInverterTotalCurrentStateTypeId, inverterData.acCurrent);
    thing->setStateValue(sunspecThreePhaseInverterCabinetTemperatureStateTypeId, inverterData.cabinetTemperature);

    if (thing->thingClassId() == sunspecSplitPhaseInverterThingClassId) {

        thing->setStateValue(sunspecSplitPhaseInverterPhaseANVoltageStateTypeId, inverterData.phaseVoltageAN);
        thing->setStateValue(sunspecSplitPhaseInverterPhaseBNVoltageStateTypeId, inverterData.phaseVoltageBN);

        thing->setStateValue(sunspecSplitPhaseInverterPhaseACurrentStateTypeId, inverterData.phaseACurrent);
        thing->setStateValue(sunspecSplitPhaseInverterPhaseBCurrentStateTypeId, inverterData.phaseBCurrent);

    } else if (thing->thingClassId() == sunspecThreePhaseInverterThingClassId) {

        thing->setStateValue(sunspecThreePhaseInverterPhaseANVoltageStateTypeId, inverterData.phaseVoltageAN);
        thing->setStateValue(sunspecThreePhaseInverterPhaseBNVoltageStateTypeId, inverterData.phaseVoltageBN);
        thing->setStateValue(sunspecThreePhaseInverterPhaseCNVoltageStateTypeId, inverterData.phaseVoltageCN);

        thing->setStateValue(sunspecThreePhaseInverterPhaseACurrentStateTypeId, inverterData.phaseACurrent);
        thing->setStateValue(sunspecThreePhaseInverterPhaseBCurrentStateTypeId, inverterData.phaseBCurrent);
        thing->setStateValue(sunspecThreePhaseInverterPhaseCCurrentStateTypeId, inverterData.phaseCCurrent);
    }

    switch(inverterData.operatingState) {
    case SunSpec::SunSpecOperatingState::Off:
        thing->setStateValue(sunspecThreePhaseInverterOperatingStateStateTypeId, "Off");
        break;
    case SunSpec::SunSpecOperatingState::MPPT:
        thing->setStateValue(sunspecThreePhaseInverterOperatingStateStateTypeId, "MPPT");
        break;
    case SunSpec::SunSpecOperatingState::Fault:
        thing->setStateValue(sunspecThreePhaseInverterOperatingStateStateTypeId, "Fault");
        break;
    case SunSpec::SunSpecOperatingState::Standby:
        thing->setStateValue(sunspecThreePhaseInverterOperatingStateStateTypeId, "Standby");
        break;
    case SunSpec::SunSpecOperatingState::Sleeping:
        thing->setStateValue(sunspecThreePhaseInverterOperatingStateStateTypeId, "Sleeping");
        break;
    case SunSpec::SunSpecOperatingState::Starting:
        thing->setStateValue(sunspecThreePhaseInverterOperatingStateStateTypeId, "Starting");
        break;
    case SunSpec::SunSpecOperatingState::Throttled:
        thing->setStateValue(sunspecThreePhaseInverterOperatingStateStateTypeId, "Throttled");
        break;
    case SunSpec::SunSpecOperatingState::ShuttingDown:
        thing->setStateValue(sunspecThreePhaseInverterOperatingStateStateTypeId, "Shutting down");
        break;
    }

    //FIXME: Event1 may have multiple states at once. Only one is stated in nymea
    if (inverterData.event1.overTemperature) {
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Over temperature");
    } else if (inverterData.event1.underTemperature) {
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Under temperature");
    } else if (inverterData.event1.groundFault) {
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Ground fault");
    } else if (inverterData.event1.memoryLoss) {
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Memory loss");
    } else if (inverterData.event1.acOverVolt) {
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "AC voltage above limit");
    } else if (inverterData.event1.cabinetOpen) {
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Cabinet open");
    } else if (inverterData.event1.acDisconnect) {
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "AC disconnect open");
    } else if (inverterData.event1.acUnderVolt) {
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "AC voltage under limit");
    } else if (inverterData.event1.dcDicconnect) {
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "DC disconnect open");
    } else if (inverterData.event1.dcOverVoltage) {
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "DC over voltage");
    } else if (inverterData.event1.overFrequency) {
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Frequency above limit");
    } else if (inverterData.event1.gridDisconnect) {
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Grid disconnect");
    } else if (inverterData.event1.hwTestFailure) {
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Hardware test failure");
    } else if (inverterData.event1.manualShutdown) {
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Manual shutdown");
    } else if (inverterData.event1.underFrequency) {
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Frequency under limit");
    } else if (inverterData.event1.blownStringFuse) {
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Blown string fuse on input");
    } else {
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "None");
    }
}

void IntegrationPluginSunSpec::onStorageDataReceived(const SunSpecStorage::StorageData &mandatory, const SunSpecStorage::StorageDataOptional &optional)
{
    SunSpecStorage *storage = static_cast<SunSpecStorage *>(sender());
    Thing *thing = m_sunSpecStorages.key(storage);

    if(!thing) {
        return;
    }
    thing->setStateValue(m_connectedStateTypeIds.value(thing->thingClassId()), true);

    thing->setStateValue(sunspecStorageChargingRateStateTypeId, mandatory.maxChargeRate);
    thing->setStateValue(sunspecStorageDischargingRateStateTypeId, mandatory.maxDischargeRate);

    bool charging = false;
    switch (optional.chargeSatus) {
    case SunSpecStorage::ChargingStatusOff:
        thing->setStateValue(sunspecStorageStorageStatusStateTypeId, "Off");
        break;
    case SunSpecStorage::ChargingStatusFull:
        thing->setStateValue(sunspecStorageStorageStatusStateTypeId, "Full");
        break;
    case SunSpecStorage::ChargingStatusEmpty:
        thing->setStateValue(sunspecStorageStorageStatusStateTypeId, "Empty");
        break;
    case SunSpecStorage::ChargingStatusHolding:
        thing->setStateValue(sunspecStorageStorageStatusStateTypeId, "Holding");
        break;
    case SunSpecStorage::ChargingStatusTesting:
        thing->setStateValue(sunspecStorageStorageStatusStateTypeId, "Testing");
        break;
    case SunSpecStorage::ChargingStatusCharging:
        thing->setStateValue(sunspecStorageStorageStatusStateTypeId, "Charging");
        break;
    case SunSpecStorage::ChargingStatusDischarging:
        thing->setStateValue(sunspecStorageStorageStatusStateTypeId, "Discharging");
        break;
    };
    double batteryLevel = optional.currentlyAvailableEnergy;
    thing->setStateValue(sunspecStorageBatteryLevelStateTypeId, batteryLevel);
    thing->setStateValue(sunspecStorageBatteryCriticalStateTypeId, (batteryLevel < 5 && !charging));
}

void IntegrationPluginSunSpec::onMeterDataReceived(const SunSpecMeter::MeterData &meterData)
{
    Q_UNUSED(meterData)
    SunSpecMeter *meter = static_cast<SunSpecMeter *>(sender());
    Thing *thing = m_sunSpecMeters.key(meter);

    if (!thing) {
        return;
    }

    thing->setStateValue(m_connectedStateTypeIds.value(thing->thingClassId()), true);

    qCDebug(dcSunSpec()) << "Meter data received";
    qCDebug(dcSunSpec()) << "   - Total AC Current" << meterData.totalAcCurrent << "[A]";
    qCDebug(dcSunSpec()) << "   - Phase A current" << meterData.phaseACurrent << "[A]";
    qCDebug(dcSunSpec()) << "   - Phase B current" << meterData.phaseBCurrent << "[A]";
    qCDebug(dcSunSpec()) << "   - Phase C current" << meterData.phaseCCurrent << "[A]";
    qCDebug(dcSunSpec()) << "   - Voltage LN" << meterData.voltageLN << "[V]";
    qCDebug(dcSunSpec()) << "   - Phase voltage AN" << meterData.phaseVoltageAN << "[V]";
    qCDebug(dcSunSpec()) << "   - Phase voltage BN" << meterData.phaseVoltageBN << "[V]";
    qCDebug(dcSunSpec()) << "   - Phase voltage CN" << meterData.phaseVoltageCN<< "[V]";
    qCDebug(dcSunSpec()) << "   - Voltage LL" << meterData.voltageLL << "[V]";
    qCDebug(dcSunSpec()) << "   - Phase voltage AB" << meterData.phaseVoltageAB << "[V]";
    qCDebug(dcSunSpec()) << "   - Phase voltage BC" << meterData.phaseVoltageBC << "[V]";
    qCDebug(dcSunSpec()) << "   - Phase voltage CA" << meterData.phaseVoltageCA<< "[V]";
    qCDebug(dcSunSpec()) << "   - Frequency" << meterData.frequency << "[Hz]";
    qCDebug(dcSunSpec()) << "   - Total real power" << meterData.totalRealPower << "[W]";
    qCDebug(dcSunSpec()) << "   - Total real energy exported" << meterData.totalRealEnergyExported<< "[kWH]";
    qCDebug(dcSunSpec()) << "   - Total real energy imported" << meterData.totalRealEnergyImported<< "[kWH]";

    thing->setStateValue(sunspecThreePhaseMeterTotalCurrentStateTypeId, meterData.totalAcCurrent);
    thing->setStateValue(sunspecThreePhaseMeterPhaseACurrentStateTypeId, meterData.phaseACurrent);
    thing->setStateValue(sunspecThreePhaseMeterPhaseBCurrentStateTypeId, meterData.phaseBCurrent);
    thing->setStateValue(sunspecThreePhaseMeterPhaseCCurrentStateTypeId, meterData.phaseCCurrent);
    thing->setStateValue(sunspecThreePhaseMeterLnACVoltageStateTypeId, meterData.voltageLN);
    thing->setStateValue(sunspecThreePhaseMeterTotalRealPowerEventTypeId, meterData.totalRealPower);
    thing->setStateValue(sunspecThreePhaseMeterEnergyExportedStateTypeId, meterData.totalRealEnergyExported);
    thing->setStateValue(sunspecThreePhaseMeterEnergyImportedStateTypeId, meterData.totalRealEnergyImported);
    thing->setStateValue(m_frequencyStateTypeIds.value(thing->thingClassId()), meterData.frequency);
}
