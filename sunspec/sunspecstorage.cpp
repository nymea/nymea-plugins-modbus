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

SunSpecStorage::SunSpecStorage(const QHostAddress &hostAddress, uint port, QObject *parent) : SunSpec(hostAddress, port, parent)
{
    connect(m_modbusTcpClient, &QModbusClient::stateChanged, this, [this] (QModbusDevice::State state) {
        if (state == QModbusDevice::ConnectedState) {
            qCDebug(dcSunSpec()) << "Inverter connected successfully";
            QList<BlockId> mapIds;
            mapIds.append(BlockId::Storage);
            findModbusMap(mapIds);
        }
    });
    connect(this, &SunSpec::foundModbusMap, this, [this] (BlockId mapId, uint modbusRegisterAddress) {
        qCDebug(dcSunSpec()) << "Read map header for mapId" << mapId << "and modbus register" << modbusRegisterAddress;
        readMapHeader(modbusRegisterAddress);
    });

    connect(this, &SunSpec::mapHeaderReceived, this, [this] (uint modbusAddress, BlockId mapId, uint mapLength) {
        m_id = mapId;
        m_mapLength = mapLength;
        m_mapModbusStartRegister = modbusAddress;
        readMap(modbusAddress, mapLength);
    });

    connect(this, &SunSpec::mapReceived, this, &SunSpecStorage::onModbusMapReceived);
}

void SunSpecStorage::getStorageMap()
{
    readMap(m_mapModbusStartRegister, m_mapLength);
}

void SunSpecStorage::readStorageBlockHeader()
{
    readMapHeader(m_mapModbusStartRegister);
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
    return writeHoldingRegister(m_slaveId, registerAddress, value);
}

QUuid SunSpecStorage::setStorageControlMode(bool chargingEnabled, bool dischargingEnabled)
{
    // Set charge bit to enable charge limit, set discharge bit to enable discharge limit, set both bits to enable both limits
    quint16 value = ((static_cast<quint16>(chargingEnabled) << StorageControlBitFieldCharge) |
                     (static_cast<quint16>(dischargingEnabled) << StorageControlBitFieldDischarge)) ;

    uint modbusRegister = m_mapModbusStartRegister + Model124::Model124ActivateStorageControlMode;
    return writeHoldingRegister(m_slaveId, modbusRegister, value);
}

QUuid SunSpecStorage::setChargingRate(int rate)
{
    //Register Name InWRte
    /* Defines the maximum charge rate (charge limit). Default is 100% */

    uint modbusRegister = m_mapModbusStartRegister + Model124::Model124SetpointMaximumChargingRate;
    int16_t value = rate * 100;
    return writeHoldingRegister(m_slaveId, modbusRegister, value);
}

QUuid SunSpecStorage::setDischargingRate(int charging)
{
    //Register Name OutWRte
    /* Defines the maximum discharge rate (discharge limit). Default is 100% */
    uint modbusRegister = m_mapModbusStartRegister + Model124::Model124SetpointMaximumDischargeRate;
    quint16 value = charging * 100;
    return writeHoldingRegister(m_slaveId, modbusRegister, value);
}

void SunSpecStorage::onModbusMapReceived(SunSpec::BlockId mapId, uint mapLength, const QVector<quint16> &data)
{
    Q_UNUSED(mapLength)
    switch (mapId) {
    case BlockId::Storage: {
        StorageData storageData;
        storageData.chargingState = ChargingState(data[Model124::Model124ChargeStatus]);
        emit storageDataReceived(storageData);
    } break;
    default:
        break;
    }
}
