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
#include "extern-plugininfo.h"

SunSpecStorage::SunSpecStorage(SunSpec *sunspec, SunSpec::BlockId blockId, int modbusAddress) :
    QObject(sunspec),
    m_connection(sunspec),
    m_id(blockId),
    m_mapModbusStartRegister(modbusAddress)
{
    qCDebug(dcSunSpec()) << "SunSpecStorage: Setting up storage";
    connect(m_connection, &SunSpec::mapReceived, this, &SunSpecStorage::onModbusMapReceived);
}

SunSpec::BlockId SunSpecStorage::blockId()
{
    return m_id;

}

void SunSpecStorage::init()
{
    qCDebug(dcSunSpec()) << "SunSpecInverter: Init";
    m_connection->readMapHeader(m_mapModbusStartRegister);
    connect(m_connection, &SunSpec::mapHeaderReceived, this, [this] (uint modbusAddress, SunSpec::BlockId mapId, uint mapLength) {
        qCDebug(dcSunSpec()) << "SunSpecInverter: Map Header received, modbus address:" << modbusAddress << "map Id:" << mapId << "map length:" << mapLength;
        m_mapLength = mapLength;
        emit initFinished(true);
        m_initFinishedSuccess = true;
    });
    QTimer::singleShot(10000, this,[this] {
       if (!m_initFinishedSuccess) {
           emit initFinished(false);
       }
    });
}

void SunSpecStorage::getStorageMap()
{
    m_connection->readMap(m_mapModbusStartRegister, m_mapLength);
}

void SunSpecStorage::readStorageBlockHeader()
{
    m_connection->readMapHeader(m_mapModbusStartRegister);
}

QUuid SunSpecStorage::setGridCharging(bool enabled)
{
    // Name ChaGriSet
    /* Setpoint to enable/dis-
    able charging from grid
    PV (charging from grid 0 disabled)
    GRID (charging from 1 grid enabled*/

    uint registerAddress = m_mapModbusStartRegister + Model124::Model124ChaGriSet;
    quint16 value = enabled;
    return m_connection->writeHoldingRegister(registerAddress, value);
}

QUuid SunSpecStorage::setStorageControlMode(bool chargingEnabled, bool dischargingEnabled)
{
    // Set charge bit to enable charge limit, set discharge bit to enable discharge limit, set both bits to enable both limits
    quint16 value = ((static_cast<quint16>(chargingEnabled) << StorageControlBitFieldCharge) |
                     (static_cast<quint16>(dischargingEnabled) << StorageControlBitFieldDischarge)) ;

    uint modbusRegister = m_mapModbusStartRegister + Model124::Model124ActivateStorageControlMode;
    return m_connection->writeHoldingRegister(modbusRegister, value);
}

QUuid SunSpecStorage::setChargingRate(int rate)
{
    //Register Name InWRte
    /* Defines the maximum charge rate (charge limit). Default is 100% */

    uint modbusRegister = m_mapModbusStartRegister + Model124::Model124SetpointMaximumChargingRate;
    int16_t value = rate * 100;
    return m_connection->writeHoldingRegister(modbusRegister, value);
}

QUuid SunSpecStorage::setDischargingRate(int charging)
{
    //Register Name OutWRte
    /* Defines the maximum discharge rate (discharge limit). Default is 100% */
    uint modbusRegister = m_mapModbusStartRegister + Model124::Model124SetpointMaximumDischargeRate;
    quint16 value = charging * 100;
    return m_connection->writeHoldingRegister(modbusRegister, value);
}

void SunSpecStorage::onModbusMapReceived(SunSpec::BlockId mapId, uint mapLength, const QVector<quint16> &data)
{
    Q_UNUSED(mapLength)
    switch (mapId) {
    case SunSpec::BlockIdStorage: {
        StorageData storageData;
        storageData.chargingState = ChargingState(data[Model124::Model124ChargeStatus]);
        emit storageDataReceived(storageData);
    } break;
    default:
        break;
    }
}
