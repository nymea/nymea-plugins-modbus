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

#include "sunspecstringcombiner.h"
#include "extern-plugininfo.h"

SunSpecStringCombiner::SunSpecStringCombiner(const QHostAddress &hostAddress, uint port, QObject *parent) : SunSpec(hostAddress, port, parent)
{
    connect(m_modbusTcpClient, &QModbusClient::stateChanged, this, [this] (QModbusDevice::State state) {
        if (state == QModbusDevice::ConnectedState) {
            qCDebug(dcSunSpec()) << "String combiner connected successfully";
            QList<BlockId> mapIds;
            mapIds.append(BlockId::StringCombiner);
            mapIds.append(BlockId::StringCombinerCurrent);
            mapIds.append(BlockId::StringCombinerAdvanced);
            mapIds.append(BlockId::StringCombinerCurrentAdvanced);
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

    connect(this, &SunSpec::mapReceived, this, &SunSpecStringCombiner::onModbusMapReceived);
}

void SunSpecStringCombiner::getStringCombinerMap()
{

}

void SunSpecStringCombiner::readStringCombinerMapHeader()
{
    readMap(m_mapModbusStartRegister, m_mapLength);
}

void SunSpecStringCombiner::onModbusMapReceived(SunSpec::BlockId mapId, uint mapLength, QVector<quint16> data)
{

    switch (mapId) {
    case BlockId::StringCombiner: {
        int rbCount = (mapLength-14)/8;
        qCDebug(dcSunSpec()) << "Map" << mapId << "Repeating Block Count" << rbCount;
    } break;
    case BlockId::StringCombinerCurrent:
    case BlockId::StringCombinerAdvanced:
    case BlockId::StringCombinerCurrentAdvanced: {
        //StringCombinerData stringCombinerData;
        //stringCombinerData.acCurrent= convertValueWithSSF(data[Model10X::Model10XAcCurrent], data[Model10X::Model10XAmpereScaleFactor]);
    } break;
    default:
        break;
    }
}
