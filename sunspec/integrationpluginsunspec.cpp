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

    m_mapIdParamTypeIds.insert(sunspecSinglePhaseInverterThingClassId, sunspecSinglePhaseInverterThingMapIdParamTypeId);
    m_mapIdParamTypeIds.insert(sunspecSplitPhaseInverterThingClassId, sunspecSplitPhaseInverterThingMapIdParamTypeId);
    m_mapIdParamTypeIds.insert(sunspecThreePhaseInverterThingClassId, sunspecThreePhaseInverterThingMapIdParamTypeId);
    m_mapIdParamTypeIds.insert(sunspecStorageThingClassId, sunspecStorageThingMapIdParamTypeId);
    m_mapIdParamTypeIds.insert(sunspecSinglePhaseMeterThingClassId, sunspecSinglePhaseMeterThingMapIdParamTypeId);
    m_mapIdParamTypeIds.insert(sunspecSplitPhaseMeterThingClassId, sunspecSplitPhaseMeterThingMapIdParamTypeId);
    m_mapIdParamTypeIds.insert(sunspecThreePhaseMeterThingClassId, sunspecThreePhaseMeterThingMapIdParamTypeId);

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
        //int slaveId = info->thing()->paramValue(sunspecConnectionThingSlaveIdParamTypeId).toInt();

        SunSpec *sunSpec;
        if (m_sunSpecConnections.contains(thing->id())) {
            qCDebug(dcSunSpec()) << "Reconfigure SunSpec connection with new address" << address;
            sunSpec = m_sunSpecConnections.value(thing->id());
            sunSpec->setHostAddress(address);
        } else {
            sunSpec = new SunSpec(address, port, this);
            m_sunSpecConnections.insert(thing->id(), sunSpec);
        }

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
        connect(sunSpec, &SunSpec::foundModbusMap, this, &IntegrationPluginSunSpec::onFoundModbusMap);
        connect(sunSpec, &SunSpec::modbusMapSearchFinished, this, &IntegrationPluginSunSpec::onModbusMapSearchFinished);
        connect(sunSpec, &SunSpec::commonMapReceived, thing, [thing] (const QString &manufacturer, const QString &deviceModel, const QString &serielNumber) {
            thing->setStateValue(sunspecConnectionConnectedStateTypeId, true);
            thing->setStateValue(sunspecConnectionManufacturerStateTypeId, manufacturer);
            thing->setStateValue(sunspecConnectionDeviceModelStateTypeId, deviceModel);
            thing->setStateValue(sunspecConnectionSerialNumberStateTypeId, serielNumber);
        });

    } else if (thing->thingClassId() == sunspecThreePhaseInverterThingClassId ||
               thing->thingClassId() == sunspecSplitPhaseInverterThingClassId ||
               thing->thingClassId() == sunspecSinglePhaseInverterThingClassId ) {

        uint mapId = thing->paramValue(m_mapIdParamTypeIds.value(thing->thingClassId())).toInt();
        int modbusAddress = thing->paramValue(m_modbusAddressParamTypeIds.value(thing->thingClassId())).toInt();
        SunSpec *connection = m_sunSpecConnections.value(thing->parentId());
        if (!connection) {
            qCWarning(dcSunSpec()) << "Could not find SunSpec connection";
            return info->finish(Thing::ThingErrorHardwareNotAvailable);
        }
        SunSpecInverter *sunSpecInverter = new SunSpecInverter(connection, SunSpec::BlockId(mapId), modbusAddress);
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

    } else if (thing->thingClassId() == sunspecSinglePhaseMeterThingClassId ||
               thing->thingClassId() == sunspecSplitPhaseMeterThingClassId ||
               thing->thingClassId() == sunspecThreePhaseMeterThingClassId) {

        uint mapId = thing->paramValue(m_mapIdParamTypeIds.value(thing->thingClassId())).toInt();
        int modbusAddress = thing->paramValue(m_modbusAddressParamTypeIds.value(thing->thingClassId())).toInt();
        SunSpec *connection = m_sunSpecConnections.value(thing->parentId());
        if (!connection) {
            qCWarning(dcSunSpec()) << "Could not find SunSpec connection";
            return info->finish(Thing::ThingErrorHardwareNotAvailable);
        }
        SunSpecMeter *sunSpecMeter = new SunSpecMeter(connection, SunSpec::BlockId(mapId), modbusAddress);
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

    } else if (info->thing()->thingClassId() == sunspecStorageThingClassId) {


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
        connection->readCommonMap();
        connection->findModbusMap(QList<SunSpec::BlockId>()); // Discover all maps, without filter

    } else if (thing->thingClassId() == sunspecSinglePhaseInverterThingClassId ||
               thing->thingClassId() == sunspecSplitPhaseInverterThingClassId ||
               thing->thingClassId() == sunspecThreePhaseInverterThingClassId) {

        SunSpecInverter *sunSpecInverter = m_sunSpecInverters.value(thing);
        if (!sunSpecInverter) {
            qCDebug(dcSunSpec()) << "SunSpecInverter not found";
            return;
        }
        sunSpecInverter->getInverterMap();

    } else if (thing->thingClassId() == sunspecSinglePhaseMeterThingClassId ||
               thing->thingClassId() == sunspecSplitPhaseMeterThingClassId ||
               thing->thingClassId() == sunspecThreePhaseMeterThingClassId) {

        SunSpecMeter *sunSpecMeter = m_sunSpecMeters.value(thing);
        if (!sunSpecMeter) {
            qCDebug(dcSunSpec()) << "SunSpecMeter not found";
            return;
        }
        sunSpecMeter->getMeterMap();

    } else if (thing->thingClassId() == sunspecStorageThingClassId) {
        SunSpecStorage *sunSpecStorage = m_sunSpecStorages.value(thing);
        if (!sunSpecStorage) {
            qCDebug(dcSunSpec()) << "SunSpecStorage not found";
            return;
        }
        sunSpecStorage->getStorageMap();

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
        } else if (action.actionTypeId() == sunspecStorageEnableChargingLimitActionTypeId) {
            /*int value = (action.param(sunspecStorageEnableChargingLimitActionEnableChargingLimitParamTypeId).value().toBool() << 1) | thing->stateValue(sunspecStorageEnableDischargingLimitStateTypeId).toBool();
            QUuid requestId = sunSpecStorage->setStorageControlMode(value);
            if (requestId.isNull()) {
                info->finish(Thing::ThingErrorHardwareFailure);
            } else {
                m_asyncActions.insert(requestId, info);
                connect(info, &ThingActionInfo::aborted, this, [requestId, this] {m_asyncActions.remove(requestId);});
            }*/
        } else if (action.actionTypeId() == sunspecStorageChargingRateActionTypeId) {
            QUuid requestId = sunSpecStorage->setChargingRate(action.param(sunspecStorageChargingRateActionChargingRateParamTypeId).value().toInt());
            if (requestId.isNull()) {
                info->finish(Thing::ThingErrorHardwareFailure);
            } else {
                m_asyncActions.insert(requestId, info);
                connect(info, &ThingActionInfo::aborted, this, [requestId, this] {m_asyncActions.remove(requestId);});
            }
        } else if (action.actionTypeId() == sunspecStorageEnableDischargingLimitActionTypeId) {
            /*int value = (action.param(sunspecStorageEnableDischargingLimitActionEnableDischargingLimitParamTypeId).value().toBool() << 1) | thing->stateValue(sunspecStorageEnableChargingLimitStateTypeId).toBool();
            QUuid requestId = sunSpecStorage->setStorageControlMode(value);
            if (requestId.isNull()) {
                info->finish(Thing::ThingErrorHardwareFailure);
            } else {
                m_asyncActions.insert(requestId, info);
                connect(info, &ThingActionInfo::aborted, this, [requestId, this] {m_asyncActions.remove(requestId);});
            }*/
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

bool IntegrationPluginSunSpec::checkIfThingExists(uint mapId, uint modbusAddress)
{
    Q_FOREACH(Thing *thing, myThings()) {
        if (thing->paramValue(m_mapIdParamTypeIds.value(thing->thingClassId())).toUInt() == mapId &&
             thing->paramValue(m_modbusAddressParamTypeIds.value(thing->thingClassId())).toUInt() == modbusAddress) {
            return true;
        }
    }
    return false;
}

void IntegrationPluginSunSpec::onRefreshTimer()
{
    foreach (SunSpec *connection, m_sunSpecConnections) {
        connection->readCommonMap(); //check connection
    }
    foreach (SunSpecInverter *inverter, m_sunSpecInverters) {
        inverter->getInverterMap();
    }
    foreach (SunSpecMeter *meter, m_sunSpecMeters) {
        meter->getMeterMap();
    }
    foreach (SunSpecStorage *storage, m_sunSpecStorages) {
        storage->getStorageMap();
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

void IntegrationPluginSunSpec::onMapHeaderReceived(uint modbusAddress, SunSpec::BlockId mapId, uint mapLength)
{
    qCDebug(dcSunSpec()) << "On map header received" << modbusAddress << mapId << mapLength;
}

void IntegrationPluginSunSpec::onMapReceived(SunSpec::BlockId mapId, uint mapLength, QVector<quint16> data)
{
    Q_UNUSED(data)
    SunSpec *connection = static_cast<SunSpec *>(sender());
    Thing *thing = myThings().findById(m_sunSpecConnections.key(connection));
    if (!thing)
        return;

    qCDebug(dcSunSpec()) << "On map received" << mapId << "Map length" << mapLength << "Thing:" << thing->name();
}

void IntegrationPluginSunSpec::onFoundModbusMap(SunSpec::BlockId mapId, int modbusStartRegister)
{
    SunSpec *connection = static_cast<SunSpec *>(sender());
    Thing *thing = myThings().findById(m_sunSpecConnections.key(connection));
    if (!thing) {
        qCWarning(dcSunSpec()) << "Thing not found for SunSpec connection" << connection->deviceModel() << connection->serialNumber();
        return;
    }

    qCDebug(dcSunSpec()) << "On map received" << mapId << "Map length" << modbusStartRegister << "Thing:" << thing->name();
    if (checkIfThingExists(mapId, modbusStartRegister)) {
        return;
    }
    QString model = thing->stateValue(sunspecConnectionDeviceModelStateTypeId).toString();
    switch (mapId) {
    case SunSpec::BlockId::BlockIdInverterSinglePhase:
    case SunSpec::BlockId::BlockIdInverterSinglePhaseFloat: {
        ThingDescriptor descriptor(sunspecSinglePhaseInverterThingClassId, model+tr(" single phase inverter"), "", thing->id());
        ParamList params;
        params.append(Param(sunspecSinglePhaseInverterThingMapIdParamTypeId, mapId));
        params.append(Param(sunspecSinglePhaseInverterThingModbusAddressParamTypeId, modbusStartRegister));
        descriptor.setParams(params);
        emit autoThingsAppeared({descriptor});
    } break;
    case SunSpec::BlockId::BlockIdInverterSplitPhase:
    case SunSpec::BlockId::BlockIdInverterSplitPhaseFloat: {
        ThingDescriptor descriptor(sunspecSplitPhaseInverterThingClassId, model+tr(" split phase inverter"), "", thing->id());
        ParamList params;
        params.append(Param(sunspecSplitPhaseInverterThingMapIdParamTypeId, mapId));
        params.append(Param(sunspecSplitPhaseInverterThingModbusAddressParamTypeId, modbusStartRegister));
        descriptor.setParams(params);
        emit autoThingsAppeared({descriptor});
    } break;
    case SunSpec::BlockId::BlockIdInverterThreePhase:
    case SunSpec::BlockId::BlockIdInverterThreePhaseFloat: {
        ThingDescriptor descriptor(sunspecThreePhaseInverterThingClassId, model+tr(" three phase inverter"), "", thing->id());
        ParamList params;
        params.append(Param(sunspecThreePhaseInverterThingMapIdParamTypeId, mapId));
        params.append(Param(sunspecThreePhaseInverterThingModbusAddressParamTypeId, modbusStartRegister));
        descriptor.setParams(params);
        emit autoThingsAppeared({descriptor});
    } break;

    case SunSpec::BlockIdSinglePhaseMeter:
    case SunSpec::BlockIdSinglePhaseMeterFloat: {
        ThingDescriptor descriptor(sunspecSinglePhaseMeterThingClassId, model+tr(" Meter"), "", thing->id());
        ParamList params;
        params.append(Param(sunspecSinglePhaseMeterThingMapIdParamTypeId, mapId));
        params.append(Param(sunspecSinglePhaseMeterThingModbusAddressParamTypeId, modbusStartRegister));
        descriptor.setParams(params);
        emit autoThingsAppeared({descriptor});
    } break;
    case SunSpec::BlockIdSplitSinglePhaseMeter:
    case SunSpec::BlockIdSplitSinglePhaseMeterFloat: {
        ThingDescriptor descriptor(sunspecSplitPhaseMeterThingClassId, model+tr(" Meter"), "", thing->id());
        ParamList params;
        params.append(Param(sunspecSplitPhaseMeterThingMapIdParamTypeId, mapId));
        params.append(Param(sunspecSplitPhaseMeterThingModbusAddressParamTypeId, modbusStartRegister));
        descriptor.setParams(params);
        emit autoThingsAppeared({descriptor});
    } break;
    case SunSpec::BlockIdWyeConnectThreePhaseMeterFloat:
    case SunSpec::BlockIdDeltaConnectThreePhaseMeterFloat: {
        ThingDescriptor descriptor(sunspecThreePhaseMeterThingClassId, model+" Meter", "", thing->id());
        ParamList params;
        params.append(Param(sunspecThreePhaseMeterThingMapIdParamTypeId, mapId));
        params.append(Param(sunspecThreePhaseMeterThingModbusAddressParamTypeId, modbusStartRegister));
        descriptor.setParams(params);
        emit autoThingsAppeared({descriptor});
    } break;
    case SunSpec::BlockIdStorage: {
        ThingDescriptor descriptor(sunspecStorageThingClassId, model+" Storage", "", thing->id());
        ParamList params;
        params.append(Param(sunspecStorageThingMapIdParamTypeId, mapId));
        params.append(Param(sunspecThreePhaseInverterThingModbusAddressParamTypeId, modbusStartRegister));
        descriptor.setParams(params);
        emit autoThingsAppeared({descriptor});
    } break;
    default:
        qCDebug(dcSunSpec()) << "Block Id not handled";
    }
}

void IntegrationPluginSunSpec::onModbusMapSearchFinished(const QHash<SunSpec::BlockId, int> &mapIds)
{
    SunSpec *connection = static_cast<SunSpec *>(sender());
    Thing *thing = myThings().findById(m_sunSpecConnections.key(connection));
    if (!thing) {
        qCWarning(dcSunSpec()) << "Thing not found for SunSpec connection" << connection->deviceModel() << connection->serialNumber();
        return;
    }

    qCDebug(dcSunSpec()) << "On modbus map search finished, maps:" << mapIds.count();

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
    thing->setStateValue(m_connectedStateTypeIds.value(thing->thingClassId()), true);
    thing->setStateValue(m_acPowerStateTypeIds.value(thing->thingClassId()), inverterData.acPower/1000.00);
    thing->setStateValue(m_acEnergyStateTypeIds.value(thing->thingClassId()), inverterData.acEnergy/1000.00);
    thing->setStateValue(m_frequencyStateTypeIds.value(thing->thingClassId()), inverterData.lineFrequency);

    thing->setStateValue(sunspecThreePhaseInverterTotalCurrentStateTypeId, inverterData.acCurrent);
    thing->setStateValue(sunspecThreePhaseInverterCabinetTemperatureStateTypeId, inverterData.cabinetTemperature);


    if (thing->thingClassId() == sunspecSplitPhaseMeterThingClassId) {

        thing->setStateValue(sunspecSplitPhaseInverterPhaseANVoltageStateTypeId, inverterData.phaseVoltageAN);
        thing->setStateValue(sunspecSplitPhaseInverterPhaseBNVoltageStateTypeId, inverterData.phaseVoltageBN);

        thing->setStateValue(sunspecSplitPhaseInverterPhaseACurrentStateTypeId, inverterData.phaseACurrent);
        thing->setStateValue(sunspecSplitPhaseInverterPhaseBCurrentStateTypeId, inverterData.phaseBCurrent);

    } else if (thing->thingClassId() == sunspecThreePhaseMeterThingClassId) {

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

    switch(inverterData.event) {
    case SunSpec::SunSpecEvent1::OVER_TEMP:
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Over temperature");
        break;
    case SunSpec::SunSpecEvent1::UNDER_TEMP:
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Under temperature");
        break;
    case SunSpec::SunSpecEvent1::GroundFault:
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Ground fault");
        break;
    case SunSpec::SunSpecEvent1::MEMORY_LOSS:
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Memory loss");
        break;
    case SunSpec::SunSpecEvent1::AC_OVER_VOLT:
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "AC voltage above limit");
        break;
    case SunSpec::SunSpecEvent1::CABINET_OPEN:
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Cabinet open");
        break;
    case SunSpec::SunSpecEvent1::AC_DISCONNECT:
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "AC disconnect open");
        break;
    case SunSpec::SunSpecEvent1::AC_UNDER_VOLT:
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "AC voltage under limit");
        break;
    case SunSpec::SunSpecEvent1::DC_DISCONNECT:
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "DC disconnect open");
        break;
    case SunSpec::SunSpecEvent1::DcOverVolatage:
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "DC over voltage");
        break;
    case SunSpec::SunSpecEvent1::OVER_FREQUENCY:
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Frequency above limit");
        break;
    case SunSpec::SunSpecEvent1::GRID_DISCONNECT:
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Grid disconnect");
        break;
    case SunSpec::SunSpecEvent1::HW_TEST_FAILURE:
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Hardware test failure");
        break;
    case SunSpec::SunSpecEvent1::MANUAL_SHUTDOWN:
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Manual shutdown");
        break;
    case SunSpec::SunSpecEvent1::UNDER_FREQUENCY:
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Frequency under limit");
        break;
    case SunSpec::SunSpecEvent1::BLOWN_STRING_FUSE:
        thing->setStateValue(sunspecThreePhaseInverterErrorStateTypeId, "Blown string fuse on input");
        break;
    }
}

void IntegrationPluginSunSpec::onStorageDataReceived(const SunSpecStorage::StorageData &storageData)
{
    SunSpecStorage *storage = static_cast<SunSpecStorage *>(sender());
    Thing *thing = m_sunSpecStorages.key(storage);

    if(!thing) {
        return;
    }
    thing->setStateValue(sunspecStorageStorageStateStateTypeId, storageData.chargingState);
}

void IntegrationPluginSunSpec::onMeterDataReceived(const SunSpecMeter::MeterData &meterData)
{
    Q_UNUSED(meterData)
    SunSpecMeter *meter = static_cast<SunSpecMeter *>(sender());
    Thing *thing = m_sunSpecMeters.key(meter);

    if(!thing) {
        return;
    }
    //thing->setStateValue(sunspecMeterStorageStateStateTypeId, meterData.event);
}
