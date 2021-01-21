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

SunSpecMeter::SunSpecMeter(SunSpec *sunspec, SunSpec::BlockId blockId, int modbusAddress) :
    QObject(sunspec),
    m_connection(sunspec),
    m_id(blockId),
    m_mapModbusStartRegister(modbusAddress)
{
    qCDebug(dcSunSpec()) << "SunSpecMeter: Setting up meter";
    connect(m_connection, &SunSpec::mapReceived, this, &SunSpecMeter::onModbusMapReceived);
}

SunSpec::BlockId SunSpecMeter::blockId()
{
    return m_id;
}

void SunSpecMeter::init()
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

void SunSpecMeter::getMeterMap()
{
   m_connection->readMap(m_mapModbusStartRegister, m_mapLength);
}

void SunSpecMeter::readMeterBlockHeader()
{

}

void SunSpecMeter::onModbusMapReceived(SunSpec::BlockId mapId, uint mapLength, QVector<quint16> data)
{
    Q_UNUSED(mapLength)
    Q_UNUSED(data)
    switch (mapId) {
    default:
        break;
    }
}
