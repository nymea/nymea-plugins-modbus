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

#include "sunspecstorage.h"
#include "extern-plugininfo.h"

#include <models/sunspecstoragemodel.h>

SunSpecStorage::SunSpecStorage(Thing *thing, SunSpecModel *model, QObject *parent) :
    SunSpecThing(thing, model, parent)
{

}

void SunSpecStorage::readBlockData()
{
    m_model->readBlockData();
}

void SunSpecStorage::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();
    SunSpecStorageModel *storage = qobject_cast<SunSpecStorageModel *>(m_model);

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
}

void SunSpecStorage::onBlockDataUpdated()
{
    SunSpecStorageModel *storage = qobject_cast<SunSpecStorageModel *>(m_model);
    qCDebug(dcSunSpec()) << m_thing->name() << "block data updated" << storage;

    m_thing->setStateValue(sunspecStorageConnectedStateTypeId, true);
    m_thing->setStateValue(sunspecStorageBatteryCriticalStateTypeId, storage->chaState() < 5);
    m_thing->setStateValue(sunspecStorageBatteryLevelStateTypeId, qRound(storage->chaState()));
    m_thing->setStateValue(sunspecStorageGridChargingStateTypeId, storage->chaGriSet() == SunSpecStorageModel::ChagrisetGrid);
    m_thing->setStateValue(sunspecStorageEnableChargingStateTypeId, storage->storCtlMod().testFlag(SunSpecStorageModel::Storctl_modCharge));
    m_thing->setStateValue(sunspecStorageChargingRateStateTypeId, storage->wChaGra());
    m_thing->setStateValue(sunspecStorageDischargingRateStateTypeId, storage->wDisChaGra());

    switch (storage->chaSt()) {
    case SunSpecStorageModel::ChastOff:
        m_thing->setStateValue(sunspecStorageBatteryLevelStateTypeId, "Off");
        m_thing->setStateValue(sunspecStorageChargingStateTypeId, false);
        m_thing->setStateValue(sunspecStorageDischargingStateTypeId, false);
        break;
    case SunSpecStorageModel::ChastEmpty:
        m_thing->setStateValue(sunspecStorageBatteryLevelStateTypeId, "Empty");
        m_thing->setStateValue(sunspecStorageChargingStateTypeId, false);
        m_thing->setStateValue(sunspecStorageDischargingStateTypeId, false);
        break;
    case SunSpecStorageModel::ChastDischarging:
        m_thing->setStateValue(sunspecStorageBatteryLevelStateTypeId, "Discharging");
        m_thing->setStateValue(sunspecStorageChargingStateTypeId, false);
        m_thing->setStateValue(sunspecStorageDischargingStateTypeId, true);
        break;
    case SunSpecStorageModel::ChastCharging:
        m_thing->setStateValue(sunspecStorageBatteryLevelStateTypeId, "Charging");
        m_thing->setStateValue(sunspecStorageChargingStateTypeId, true);
        m_thing->setStateValue(sunspecStorageDischargingStateTypeId, false);
        break;
    case SunSpecStorageModel::ChastFull:
        m_thing->setStateValue(sunspecStorageBatteryLevelStateTypeId, "Full");
        m_thing->setStateValue(sunspecStorageChargingStateTypeId, false);
        m_thing->setStateValue(sunspecStorageDischargingStateTypeId, false);
        break;
    case SunSpecStorageModel::ChastHolding:
        m_thing->setStateValue(sunspecStorageBatteryLevelStateTypeId, "Holding");
        m_thing->setStateValue(sunspecStorageChargingStateTypeId, false);
        m_thing->setStateValue(sunspecStorageDischargingStateTypeId, false);
        break;
    case SunSpecStorageModel::ChastTesting:
        m_thing->setStateValue(sunspecStorageBatteryLevelStateTypeId, "Testing");
        m_thing->setStateValue(sunspecStorageChargingStateTypeId, false);
        m_thing->setStateValue(sunspecStorageDischargingStateTypeId, false);
        break;
    }

}
