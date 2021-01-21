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

#include "sunspecinverter.h"
#include "extern-plugininfo.h"

#include <QTimer>

SunSpecInverter::SunSpecInverter(SunSpec *sunspec, SunSpec::BlockId mapId, int modbusAddress) :
    QObject(sunspec),
    m_connection(sunspec),
    m_id(mapId),
    m_mapModbusStartRegister(modbusAddress)
{
    qCDebug(dcSunSpec()) << "SunSpecInverter: Setting up inverter";
    connect(m_connection, &SunSpec::mapReceived, this, &SunSpecInverter::onModbusMapReceived);
}

SunSpec::BlockId SunSpecInverter::blockId()
{
    return m_id;
}

void SunSpecInverter::init()
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

void SunSpecInverter::getInverterMap()
{
    // TODO check map length to modbus max value
    m_connection->readMap(m_mapModbusStartRegister, m_mapLength);
}

void SunSpecInverter::readInverterBlockHeader()
{
    m_connection->readMapHeader(m_mapModbusStartRegister);
}

void SunSpecInverter::onModbusMapReceived(SunSpec::BlockId mapId, uint mapLength, QVector<quint16> data)
{
    Q_UNUSED(mapLength)
    if (mapId != m_id) {
        return;
    }
    if (mapLength < m_mapLength) {
        qCDebug(dcSunSpec()) << "SunSpecInverter: on modbus map received, map length ist too short" << mapLength;
        //return;
    }
    InverterData inverterData;

    switch (mapId) {
    case SunSpec::BlockIdInverterSinglePhase:
    case SunSpec::BlockIdInverterSplitPhase:
    case SunSpec::BlockIdInverterThreePhase: {

        qCDebug(dcSunSpec()) << "Inverter with SSF values:";
        qCDebug(dcSunSpec()) << "   -   AC Current:" << data[Model10X::Model10XAcCurrent];
        qCDebug(dcSunSpec()) << "   -   Phase A Current:" << data[Model10X::Model10XPhaseACurrent];
        qCDebug(dcSunSpec()) << "   -   Phase B Current:" << data[Model10X::Model10XPhaseBCurrent];
        qCDebug(dcSunSpec()) << "   -   Phase C Current:" << data[Model10X::Model10XPhaseCCurrent];
        qCDebug(dcSunSpec()) << "   -   Ampere scale factor:" << static_cast<qint16>(data[Model10X::Model10XAmpereScaleFactor]);

        inverterData.acCurrent= m_connection->convertValueWithSSF(data[Model10X::Model10XAcCurrent], data[Model10X::Model10XAmpereScaleFactor]);
        inverterData.acPower = m_connection->convertValueWithSSF(data[Model10X::Model10XACPower], data[Model10X::Model10XWattScaleFactor]);
        inverterData.lineFrequency = m_connection->convertValueWithSSF(data[Model10X::Model10XLineFrequency], data[Model10X::Model10XHerzScaleFactor]);

        inverterData.phaseACurrent = m_connection->convertValueWithSSF(data[Model10X::Model10XPhaseACurrent], data[Model10X::Model10XAmpereScaleFactor]);
        inverterData.phaseBCurrent = m_connection->convertValueWithSSF(data[Model10X::Model10XPhaseBCurrent], data[Model10X::Model10XAmpereScaleFactor]);
        inverterData.phaseCCurrent = m_connection->convertValueWithSSF(data[Model10X::Model10XPhaseCCurrent], data[Model10X::Model10XAmpereScaleFactor]);

        inverterData.phaseVoltageAN = m_connection->convertValueWithSSF(data[Model10X::Model10XPhaseVoltageAN], data[Model10X::Model10XVoltageScaleFactor]);
        inverterData.phaseVoltageBN = m_connection->convertValueWithSSF(data[Model10X::Model10XPhaseVoltageBN], data[Model10X::Model10XVoltageScaleFactor]);
        inverterData.phaseVoltageCN = m_connection->convertValueWithSSF(data[Model10X::Model10XPhaseVoltageCN], data[Model10X::Model10XVoltageScaleFactor]);

        qCDebug(dcSunSpec()) << "AC energy converting:";
        qCDebug(dcSunSpec()) << "   - Origin" << data[Model10X::Model10XAcEnergy];
        qCDebug(dcSunSpec()) << "   - SSF" << static_cast<qint16>(data[Model10X::Model10XWattHoursScaleFactor]);
        qCDebug(dcSunSpec()) << "   - Converted "<< m_connection->convertValueWithSSF(data[Model10X::Model10XAcEnergy], data[Model10X::Model10XWattHoursScaleFactor]);
        inverterData.acEnergy = m_connection->convertValueWithSSF(data[Model10X::Model10XAcEnergy], data[Model10X::Model10XWattHoursScaleFactor]);

        inverterData.cabinetTemperature = m_connection->convertValueWithSSF(data[Model10X::Model10XCabinetTemperature], data[Model10X::Model10XTemperatureScaleFactor]);
        inverterData.event = SunSpec::SunSpecEvent1(data[Model10X::Model10XEvent1]);
        inverterData.operatingState = SunSpec::SunSpecOperatingState(data[Model10X::Model10XOperatingState]);
        emit inverterDataReceived(inverterData);

    } break;
    case SunSpec::BlockIdInverterThreePhaseFloat:
    case SunSpec::BlockIdInverterSplitPhaseFloat:
    case SunSpec::BlockIdInverterSinglePhaseFloat: {

        inverterData.acCurrent = m_connection->convertFloatValues(data[Model11X::Model11XAcCurrent], data[Model11X::Model11XAcCurrent+1]);

        inverterData.phaseCCurrent = m_connection->convertFloatValues(data[Model11X::Model11XPhaseCCurrent], data[Model11X::Model11XPhaseCCurrent+1]);
        inverterData.phaseVoltageCN = m_connection->convertFloatValues(data[Model11X::Model11XPhaseVoltageCN], data[Model11X::Model11XPhaseVoltageCN+1]);

        inverterData.phaseBCurrent = m_connection->convertFloatValues(data[Model11X::Model11XPhaseBCurrent], data[Model11X::Model11XPhaseBCurrent+1]);
        inverterData.phaseVoltageBN = m_connection->convertFloatValues(data[Model11X::Model11XPhaseVoltageBN], data[Model11X::Model11XPhaseVoltageBN+1]);

        inverterData.phaseACurrent = m_connection->convertFloatValues(data[Model11X::Model11XPhaseACurrent], data[Model11X::Model11XPhaseACurrent+1]);
        inverterData.phaseVoltageAN = m_connection->convertFloatValues(data[Model11X::Model11XPhaseVoltageAN], data[Model11X::Model11XPhaseVoltageAN+1]);

        inverterData.acPower = m_connection->convertFloatValues(data[Model11X::Model11XACPower], data[Model11X::Model11XACPower+1]);
        inverterData.lineFrequency = m_connection->convertFloatValues(data[Model11X::Model11XLineFrequency], data[Model11X::Model11XLineFrequency+1]);

        inverterData.acEnergy = m_connection->convertFloatValues(data[Model11X::Model11XAcEnergy], data[Model11X::Model11XAcEnergy+1]);
        inverterData.cabinetTemperature =m_connection->convertFloatValues(data[Model11X::Model11XCabinetTemperature], data[Model11X::Model11XCabinetTemperature+1]);
        inverterData.event = SunSpec::SunSpecEvent1(data[Model11X::Model11XEvent1]);
        inverterData.operatingState = SunSpec::SunSpecOperatingState(data[Model11X::Model11XOperatingState]);
        emit inverterDataReceived(inverterData);
    } break;
    default:
        //ignore
        break;
    }
}
