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

void IntegrationPluginSunSpec::setupThing(ThingSetupInfo *info)
{
    if (info->thing()->thingClassId() == sunspecConnectionThingClassId) {
        QHostAddress address = QHostAddress(info->thing()->paramValue(sunspecConnectionThingIpAddressParamTypeId).toString());
        SunSpec *sunSpec = new SunSpec(address, 502, this);
        m_sunSpecConnections.insert(info->thing(), sunSpec);
        if (!sunSpec->connectModbus()) {
            QTimer::singleShot(2000, info, [this, info] {
                setupThing(info);
            });
            qCWarning(dcSunSpec()) << "Error connecting to SunSpec device";
            return;
        }
        connect(sunSpec, &SunSpec::connectionStateChanged, info, [info] (bool status) {
            qCDebug(dcSunSpec()) << "Modbus Inverter init finished" << status;
            if (status) {
                info->finish(Thing::ThingErrorNoError);
            } else {
                info->finish(Thing::ThingErrorHardwareFailure);
            }
        });

        connect(info, &ThingSetupInfo::aborted, sunSpec, [info, this] {
            m_sunSpecConnections.take(info->thing())->deleteLater();
        });
        //connect(sunSpec, &SunSpecInverter::inverterDataReceived, this, &IntegrationPluginSunSpec::onInverterDataReceived);

    } else if (info->thing()->thingClassId() == sunspecInverterThingClassId) {
        QHostAddress address = QHostAddress(info->thing()->paramValue(sunspecInverterThingIpAddressParamTypeId).toString());
        SunSpecInverter *sunSpecInverter = new SunSpecInverter(address, 502, this);
        m_sunSpecInverters.insert(info->thing(), sunSpecInverter);
        if (!sunSpecInverter->connectModbus()) {
            QTimer::singleShot(2000, info, [this, info] {
                setupThing(info);
            });
            qCWarning(dcSunSpec()) << "Error connecting to SunSpec device";
            return;
        }
        connect(sunSpecInverter, &SunSpecInverter::initFinished, info, [info] {
            qCDebug(dcSunSpec()) << "Modbus Inverter init finished";
            info->finish(Thing::ThingErrorNoError);
        });

        connect(info, &ThingSetupInfo::aborted, sunSpecInverter, [info, this] {
            m_sunSpecInverters.take(info->thing())->deleteLater();
        });
        connect(sunSpecInverter, &SunSpecInverter::inverterDataReceived, this, &IntegrationPluginSunSpec::onInverterDataReceived);

    } else if (info->thing()->thingClassId() == sunspecMeterThingClassId) {
        QHostAddress address = QHostAddress(info->thing()->paramValue(sunspecMeterThingIpAddressParamTypeId).toString());
        SunSpecMeter *sunSpecMeter = new SunSpecMeter(address, 502, this);
        m_sunSpecMeters.insert(info->thing(), sunSpecMeter);
        if (!sunSpecMeter->connectModbus()) {
            QTimer::singleShot(2000, info, [this, info] {
                setupThing(info);
            });
        }

        connect(info, &ThingSetupInfo::aborted, sunSpecMeter, [info, this] {
            m_sunSpecMeters.take(info->thing())->deleteLater();
        });

    } else if (info->thing()->thingClassId() == sunspecTrackerThingClassId) {
        QHostAddress address = QHostAddress(info->thing()->paramValue(sunspecTrackerThingIpAddressParamTypeId).toString());
        SunSpecTracker *sunSpecTracker = new SunSpecTracker(address, 502, this);
        m_sunSpecTrackers.insert(info->thing(), sunSpecTracker);
        if (!sunSpecTracker->connectModbus()) {
            QTimer::singleShot(2000, info, [this, info] {
                setupThing(info);
            });
        }
        connect(info, &ThingSetupInfo::aborted, sunSpecTracker, [info, this] {
            m_sunSpecTrackers.take(info->thing())->deleteLater();
        });

    } else if (info->thing()->thingClassId() == sunspecStorageThingClassId) {
        QHostAddress address = QHostAddress(info->thing()->paramValue(sunspecStorageThingIpAddressParamTypeId).toString());
        SunSpecStorage *sunSpecStorage = new SunSpecStorage(address, 502, this);
        m_sunSpecStorages.insert(info->thing(), sunSpecStorage);
        if (!sunSpecStorage->connectModbus()) {
            QTimer::singleShot(2000, info, [this, info] {
                setupThing(info);
            });
        }
        connect(info, &ThingSetupInfo::aborted, sunSpecStorage, [info, this] {
            m_sunSpecStorages.take(info->thing())->deleteLater();
        });

    } else {
        Q_ASSERT_X(false, "setupThing", QString("Unhandled thingClassId: %1").arg(info->thing()->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginSunSpec::postSetupThing(Thing *thing)
{
    Q_UNUSED(thing)

    if (!m_refreshTimer) {
        int refreshTime = configValue(sunSpecPluginUpdateIntervalParamTypeId).toInt();
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(refreshTime);
        connect(m_refreshTimer, &PluginTimer::timeout, this, &IntegrationPluginSunSpec::onRefreshTimer);
    }

    if (thing->thingClassId() == sunspecInverterThingClassId) {
        SunSpecInverter *sunSpecInverter = m_sunSpecInverters.take(thing);
        if (!sunSpecInverter)
            return;
        sunSpecInverter->getInverterMap();
        thing->setParamValue(sunspecInverterThingManufacturerParamTypeId, sunSpecInverter->manufacturer());
        thing->setParamValue(sunspecInverterThingSerialNumberParamTypeId, sunSpecInverter->serialNumber());
        thing->setParamValue(sunspecInverterThingDeviceModelParamTypeId, sunSpecInverter->deviceModel());

    } else if (thing->thingClassId() == sunspecMeterThingClassId) {
        SunSpecMeter *sunSpecMeter = m_sunSpecMeters.take(thing);
        if (!sunSpecMeter)
            return;
        //TODO upate data
        thing->setParamValue(sunspecMeterThingManufacturerParamTypeId, sunSpecMeter->manufacturer());
        thing->setParamValue(sunspecMeterThingSerialNumberParamTypeId, sunSpecMeter->serialNumber());
        thing->setParamValue(sunspecMeterThingDeviceModelParamTypeId, sunSpecMeter->deviceModel());

    } else if (thing->thingClassId() == sunspecTrackerThingClassId) {
        SunSpecTracker *sunSpecTracker = m_sunSpecTrackers.take(thing);
        if (!sunSpecTracker)
            return;
        //TODO upate data
        thing->setParamValue(sunspecTrackerThingManufacturerParamTypeId, sunSpecTracker->manufacturer());
        thing->setParamValue(sunspecTrackerThingSerialNumberParamTypeId, sunSpecTracker->serialNumber());
        thing->setParamValue(sunspecTrackerThingDeviceModelParamTypeId, sunSpecTracker->deviceModel());

    } else if (thing->thingClassId() == sunspecStorageThingClassId) {
        SunSpecStorage *sunSpecStorage = m_sunSpecStorages.take(thing);
        if (!sunSpecStorage)
            return;
        //TODO upate data
        thing->setParamValue(sunspecStorageThingManufacturerParamTypeId, sunSpecStorage->manufacturer());
        thing->setParamValue(sunspecStorageThingSerialNumberParamTypeId, sunSpecStorage->serialNumber());
        thing->setParamValue(sunspecStorageThingDeviceModelParamTypeId, sunSpecStorage->deviceModel());

    } else {
        Q_ASSERT_X(false, "postSetupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }

}

void IntegrationPluginSunSpec::thingRemoved(Thing *thing)
{
    Q_UNUSED(thing)

    if (thing->thingClassId() == sunspecInverterThingClassId) {
        SunSpecInverter *sunSpecInverter = m_sunSpecInverters.take(thing);
        if (sunSpecInverter)
            sunSpecInverter->deleteLater();

    } else if (thing->thingClassId() == sunspecMeterThingClassId) {
        SunSpecMeter *sunSpecMeter = m_sunSpecMeters.take(thing);
        if (sunSpecMeter)
            sunSpecMeter->deleteLater();

    } else if (thing->thingClassId() == sunspecTrackerThingClassId) {
        SunSpecTracker *sunSpecTracker = m_sunSpecTrackers.take(thing);
        if (sunSpecTracker)
            sunSpecTracker->deleteLater();

    } else if (thing->thingClassId() == sunspecStorageThingClassId) {
        SunSpecStorage *sunSpecStorage = m_sunSpecStorages.take(thing);
        if (sunSpecStorage)
            sunSpecStorage->deleteLater();

    } else {
        Q_ASSERT_X(false, "thingRemoved", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }

    if (myThings().isEmpty()) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_refreshTimer);
        m_refreshTimer = nullptr;
    }
}

void IntegrationPluginSunSpec::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    if (thing->thingClassId() == sunspecInverterThingClassId) {
        SunSpecInverter *sunSpecInverter = m_sunSpecInverters.value(thing);
        if (!sunSpecInverter) {
            qWarning(dcSunSpec()) << "Could not find SunSpec inverter for thing" << thing->name();
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }
        info->finish(Thing::ThingErrorActionTypeNotFound);

    } else if (thing->thingClassId() == sunspecMeterThingClassId) {
        info->finish(Thing::ThingErrorActionTypeNotFound);

    } else if (thing->thingClassId() == sunspecTrackerThingClassId) {
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

void IntegrationPluginSunSpec::onRefreshTimer()
{
    //get data
    foreach (SunSpecInverter *inverter, m_sunSpecInverters) {
        inverter->getInverterMap();
    }
    foreach (SunSpecMeter *meter, m_sunSpecMeters) {
        Q_UNUSED(meter) //TODO
    }
    foreach (SunSpecTracker *tracker, m_sunSpecTrackers) {
        Q_UNUSED(tracker) //TODO
    }
    foreach (SunSpecStorage *storage, m_sunSpecStorages) {
        Q_UNUSED(storage) //TODO
    }
}

void IntegrationPluginSunSpec::onPluginConfigurationChanged(const ParamTypeId &paramTypeId, const QVariant &value)
{
    // Check refresh schedule
    if (paramTypeId == sunSpecPluginUpdateIntervalParamTypeId) {
        if (m_refreshTimer) {
            int refreshTime = value.toInt();
            m_refreshTimer->stop();
            m_refreshTimer->startTimer(refreshTime);
        }
    }
}

void IntegrationPluginSunSpec::onConnectionStateChanged(bool status)
{
    Q_UNUSED(status)
}

void IntegrationPluginSunSpec::onWriteRequestExecuted(QUuid requestId, bool success)
{
    Q_UNUSED(requestId)
    Q_UNUSED(success)
}

void IntegrationPluginSunSpec::onWriteRequestError(QUuid requestId, const QString &error)
{
    Q_UNUSED(requestId)
    Q_UNUSED(error)
}

void IntegrationPluginSunSpec::onInverterDataReceived(const SunSpecInverter::InverterData &inverterData)
{
    SunSpecInverter *inverter = static_cast<SunSpecInverter *>(sender());
    Thing *thing = m_sunSpecInverters.key(inverter);

    if(!thing) {
        return;
    }
    thing->setStateValue(sunspecInverterAcPowerStateTypeId, inverterData.acPower);
    thing->setStateValue(sunspecInverterAcEnergyStateTypeId, inverterData.acEnergy/1000.00);
    thing->setStateValue(sunspecInverterLineFrequencyStateTypeId, inverterData.lineFrequency);
    thing->setStateValue(sunspecInverterTotalCurrentStateTypeId, inverterData.acCurrent);
    thing->setStateValue(sunspecInverterCabinetTemperatureStateTypeId, inverterData.cabinetTemperature);

    thing->setStateValue(sunspecInverterPhaseACurrentStateTypeId, inverterData.phaseACurrent);
    thing->setStateValue(sunspecInverterPhaseBCurrentStateTypeId, inverterData.phaseBCurrent);
    thing->setStateValue(sunspecInverterPhaseCCurrentStateTypeId, inverterData.phaseCCurrent);

    thing->setStateValue(sunspecInverterPhaseANVoltageStateTypeId, inverterData.phaseVoltageAN);
    thing->setStateValue(sunspecInverterPhaseBNVoltageStateTypeId, inverterData.phaseVoltageBN);
    thing->setStateValue(sunspecInverterPhaseCNVoltageStateTypeId, inverterData.phaseVoltageCN);

    switch(inverterData.operatingState) {
    case SunSpec::SunSpecOperatingState::Off:
        thing->setStateValue(sunspecInverterOperatingStateStateTypeId, "Off");
        break;
    case SunSpec::SunSpecOperatingState::MPPT:
        thing->setStateValue(sunspecInverterOperatingStateStateTypeId, "MPPT");
        break;
    case SunSpec::SunSpecOperatingState::Fault:
        thing->setStateValue(sunspecInverterOperatingStateStateTypeId, "Fault");
        break;
    case SunSpec::SunSpecOperatingState::Standby:
        thing->setStateValue(sunspecInverterOperatingStateStateTypeId, "Standby");
        break;
    case SunSpec::SunSpecOperatingState::Sleeping:
        thing->setStateValue(sunspecInverterOperatingStateStateTypeId, "Sleeping");
        break;
    case SunSpec::SunSpecOperatingState::Starting:
        thing->setStateValue(sunspecInverterOperatingStateStateTypeId, "Starting");
        break;
    case SunSpec::SunSpecOperatingState::Throttled:
        thing->setStateValue(sunspecInverterOperatingStateStateTypeId, "Throttled");
        break;
    case SunSpec::SunSpecOperatingState::ShuttingDown:
        thing->setStateValue(sunspecInverterOperatingStateStateTypeId, "Shutting down");
        break;
    }

    switch(inverterData.event) {
    case SunSpec::SunSpecEvent1::OVER_TEMP:
        thing->setStateValue(sunspecInverterErrorStateTypeId, "Over temperature");
        break;
    case SunSpec::SunSpecEvent1::UNDER_TEMP:
        thing->setStateValue(sunspecInverterErrorStateTypeId, "Under temperature");
        break;
    case SunSpec::SunSpecEvent1::GroundFault:
        thing->setStateValue(sunspecInverterErrorStateTypeId, "Ground fault");
        break;
    case SunSpec::SunSpecEvent1::MEMORY_LOSS:
        thing->setStateValue(sunspecInverterErrorStateTypeId, "Memory loss");
        break;
    case SunSpec::SunSpecEvent1::AC_OVER_VOLT:
        thing->setStateValue(sunspecInverterErrorStateTypeId, "AC voltage above limit");
        break;
    case SunSpec::SunSpecEvent1::CABINET_OPEN:
        thing->setStateValue(sunspecInverterErrorStateTypeId, "Cabinet open");
        break;
    case SunSpec::SunSpecEvent1::AC_DISCONNECT:
        thing->setStateValue(sunspecInverterErrorStateTypeId, "AC disconnect open");
        break;
    case SunSpec::SunSpecEvent1::AC_UNDER_VOLT:
        thing->setStateValue(sunspecInverterErrorStateTypeId, "AC voltage under limit");
        break;
    case SunSpec::SunSpecEvent1::DC_DISCONNECT:
        thing->setStateValue(sunspecInverterErrorStateTypeId, "DC disconnect open");
        break;
    case SunSpec::SunSpecEvent1::DcOverVolatage:
        thing->setStateValue(sunspecInverterErrorStateTypeId, "DC over voltage");
        break;
    case SunSpec::SunSpecEvent1::OVER_FREQUENCY:
        thing->setStateValue(sunspecInverterErrorStateTypeId, "Frequency above limit");
        break;
    case SunSpec::SunSpecEvent1::GRID_DISCONNECT:
        thing->setStateValue(sunspecInverterErrorStateTypeId, "Grid disconnect");
        break;
    case SunSpec::SunSpecEvent1::HW_TEST_FAILURE:
        thing->setStateValue(sunspecInverterErrorStateTypeId, "Hardware test failure");
        break;
    case SunSpec::SunSpecEvent1::MANUAL_SHUTDOWN:
        thing->setStateValue(sunspecInverterErrorStateTypeId, "Manual shutdown");
        break;
    case SunSpec::SunSpecEvent1::UNDER_FREQUENCY:
        thing->setStateValue(sunspecInverterErrorStateTypeId, "Frequency under limit");
        break;
    case SunSpec::SunSpecEvent1::BLOWN_STRING_FUSE:
        thing->setStateValue(sunspecInverterErrorStateTypeId, "Blown string fuse on input");
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

void IntegrationPluginSunSpec::onTrackerDataReceived(const SunSpecTracker::TrackerData &trackerData)
{
    Q_UNUSED(trackerData)
    SunSpecTracker *tracker = static_cast<SunSpecTracker *>(sender());
    Thing *thing = m_sunSpecTrackers.key(tracker);

    if(!thing) {
        return;
    }
    //thing->setStateValue(sunspecStorageStorageStateStateTypeId, storageData.chargingState);
}
