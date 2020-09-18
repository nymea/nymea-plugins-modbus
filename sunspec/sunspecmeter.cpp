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

#include "sunspecmeter.h"
#include "extern-plugininfo.h"

SunSpecMeter::SunSpecMeter(const QHostAddress &hostAddress, uint port, QObject *parent) : SunSpec(hostAddress, port, parent)
{
    connect(m_modbusTcpClient, &QModbusClient::stateChanged, this, [this] (QModbusDevice::State state) {
        if (state == QModbusDevice::ConnectedState) {
            qCDebug(dcSunSpec()) << "Meter connected successfully";
            QList<BlockId> mapIds;
            mapIds.append(BlockId::SinglePhaseMeter);
            mapIds.append(BlockId::SplitSinglePhaseMeter);
            mapIds.append(BlockId::WyeConnectThreePhaseMeter);
            mapIds.append(BlockId::DeltaConnectThreePhaseMeter);
            mapIds.append(BlockId::SinglePhaseMeterFloat);
            mapIds.append(BlockId::SplitSinglePhaseMeterFloat);
            mapIds.append(BlockId::WyeConnectThreePhaseMeterFloat);
            mapIds.append(BlockId::DeltaConnectThreePhaseMeterFloat);
            findModbusMap(mapIds);
        }
    });
}

void SunSpecMeter::geMeterMap()
{
    readMap(m_mapModbusStartRegister, m_mapLength);
}

void SunSpecMeter::readMeterBlockHeader()
{

}
