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

#include "sunspecstorage.h"

SunSpecStorage::SunSpecStorage(SunSpec *sunspec, SunSpec::ModelId modelId, int modbusAddress) :
    QObject(sunspec),
    m_connection(sunspec),
    m_id(modelId),
    m_modelModbusStartRegister(modbusAddress)
{
    qCDebug(dcSunSpec()) << "SunSpecStorage: Setting up storage";
    connect(m_connection, &SunSpec::modelDataBlockReceived, this, &SunSpecStorage::onModelDataBlockReceived);
}

SunSpec::ModelId SunSpecStorage::modelId()
{
    return m_id;
}

void SunSpecStorage::init()
{
    qCDebug(dcSunSpec()) << "SunSpecStorage: Init";
    getStorageModelHeader();
    connect(m_connection, &SunSpec::modelHeaderReceived, this, [this] (uint modbusAddress, SunSpec::ModelId modelId, uint length) {
        if (modelId == m_id) {
            qCDebug(dcSunSpec()) << "SunSpecStorager: Model header received, modbus address:" << modbusAddress << "model Id:" << modelId << "length:" << length;
            m_modelLength = length;
            emit initFinished(true);
            m_initFinishedSuccess = true;
        }
    });
    QTimer::singleShot(10000, this, [this] {
        if (!m_initFinishedSuccess) {
            emit initFinished(false);
        }
    });
}

void SunSpecStorage::getStorageModelDataBlock()
{
    qCDebug(dcSunSpec()) << "SunSpecStorage: get storage model data block, modbus register" << m_modelModbusStartRegister << "length" << m_modelLength;
    m_connection->readModelDataBlock(m_modelModbusStartRegister, m_modelLength);
}

void SunSpecStorage::getStorageModelHeader()
{
    qCDebug(dcSunSpec()) << "SunSpecStorage: get storage model header, modbus register" << m_modelModbusStartRegister << "length" << m_modelLength;
    m_connection->readModelHeader(m_modelModbusStartRegister);
}

QUuid SunSpecStorage::setGridCharging(bool enabled)
{
    // Name ChaGriSet
    /* Setpoint to enable/dis-
    able charging from grid
    PV (charging from grid 0 disabled)
    GRID (charging from 1 grid enabled*/

    uint registerAddress = m_modelModbusStartRegister + Model124Optional::Model124ChaGriSet;
    quint16 value = enabled;
    return m_connection->writeHoldingRegister(registerAddress, value);
}

QUuid SunSpecStorage::setStorageControlMode(bool chargingEnabled, bool dischargingEnabled)
{
    // Set charge bit to enable charge limit, set discharge bit to enable discharge limit, set both bits to enable both limits
    quint16 value = ((static_cast<quint16>(chargingEnabled)) |
                     (static_cast<quint16>(dischargingEnabled) << 1)) ;

    uint modbusRegister = m_modelModbusStartRegister + Model124::Model124StorCtl_Mod;
    return m_connection->writeHoldingRegister(modbusRegister, value);
}

QUuid SunSpecStorage::setChargingRate(float rate)
{
    if (!m_scaleFactorsSet) {
        qCWarning(dcSunSpec()) << "SunSpecStorage: Set charging rate, scale factors are not set";
        return "";
    }
    if (rate < 0.00 || rate > 100.00) {
        qCWarning(dcSunSpec()) << "SunSpecStorage: Set charging rate, rate out of boundaries [0, 100]";
        return "";
    }
    uint modbusRegister = m_modelModbusStartRegister + Model124::Model124WChaGra;
    quint16 value = m_connection->convertFromFloatWithSSF(rate, m_WChaDisChaGra_SF);
    return m_connection->writeHoldingRegister(modbusRegister, value);
}

QUuid SunSpecStorage::setDischargingRate(float rate)
{
    if (!m_scaleFactorsSet) {
        qCWarning(dcSunSpec()) << "SunSpecStorage: Set discharging rate, scale factors are not set";
    }
    if (rate < 0.00 || rate > 100.00) {
        qCWarning(dcSunSpec()) << "SunSpecStorage: Set doscharging rate, rate out of boundaries [0, 100]";
        return "";
    }
    uint modbusRegister = m_modelModbusStartRegister + Model124::Model124WDisChaGra;
    quint16 value = m_connection->convertFromFloatWithSSF(rate, m_WChaDisChaGra_SF);
    return m_connection->writeHoldingRegister(modbusRegister, value);
}

void SunSpecStorage::onModelDataBlockReceived(SunSpec::ModelId modelId, uint length, const QVector<quint16> &data)
{
    if (modelId != m_id) {
        return;
    }

    if (length < m_modelLength) {
        qCDebug(dcSunSpec()) << "SunSpecMeter: on model data block received, model length is too short" << length;
        return;
    }

    qCDebug(dcSunSpec()) << "SunSpecStorage: Received" << modelId;
    switch (modelId) {
    case SunSpec::ModelIdStorage: {
        StorageData mandatory;
        mandatory.WChaMax = m_connection->convertToFloatWithSSF(data[Model124WChaMax], data[Model124WChaMax_SF]);
        mandatory.WChaGra = m_connection->convertToFloatWithSSF(data[Model124WChaGra], data[Model124WChaDisChaGra_SF]);
        mandatory.WDisChaGra = m_connection->convertToFloatWithSSF(data[Model124WDisChaGra], data[Model124WChaDisChaGra_SF]);
        mandatory.StorCtl_Mod_ChargingEnabled = data[Model124StorCtl_Mod]&0x01;
        mandatory.StorCtl_Mod_DischargingEnabled = data[Model124StorCtl_Mod]&0x02;

        StorageDataOptional optional;
        optional.VAChaMax  = m_connection->convertToFloatWithSSF(data[Model124VAChaMax], data[Model124VAChaMax_SF]);
        optional.MinRsvPct = m_connection->convertToFloatWithSSF(data[Model124MinRsvPct], data[Model124MinRsvPct_SF]);
        optional.ChaState  = m_connection->convertToFloatWithSSF(data[Model124ChaState], data[Model124ChaState_SF]);
        optional.StorAval  = m_connection->convertToFloatWithSSF(data[Model124StorAval], data[Model124StorAval_SF]);
        optional.InBatV    = m_connection->convertToFloatWithSSF(data[Model124InBatV], data[Model124InBatV_SF]);
        optional.ChaSt     = ChargingStatus(data[Model124ChaSt]);
        optional.OutWRte   = m_connection->convertToFloatWithSSF(data[Model124OutWRte], data[Model124InOutWRte_SF]);
        optional.InWRte    = m_connection->convertToFloatWithSSF(data[Model124InWRte], data[Model124InOutWRte_SF]);
        optional.InOutWRte_WinTms  = data[Model124InOutWRte_WinTms];
        optional.InOutWRte_RvrtTms = data[Model124InOutWRte_RvrtTms];
        optional.InOutWRte_RmpTms  = data[Model124InOutWRte_RmpTms];
        optional.ChaGriSet = GridCharge(data[Model124ChaGriSet]);
        emit storageDataReceived(mandatory, optional);
    } break;
    case SunSpec::ModelIdBatteryBaseModel:
    case SunSpec::ModelIdLithiumIonBatteryModel: {
        qCDebug(dcSunSpec()) << "Model not yet supported";
    }
    default:
        break;
    }
}
