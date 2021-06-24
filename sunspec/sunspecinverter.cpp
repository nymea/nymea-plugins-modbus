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

SunSpecInverter::SunSpecInverter(SunSpec *sunspec, SunSpec::ModelId modelId, int modbusAddress) :
    QObject(sunspec),
    m_connection(sunspec),
    m_id(modelId),
    m_modelModbusStartRegister(modbusAddress)
{
    qCDebug(dcSunSpec()) << "SunSpecInverter: Setting up inverter";
    connect(m_connection, &SunSpec::modelDataBlockReceived, this, &SunSpecInverter::onModelDataBlockReceived);
}

SunSpec::ModelId SunSpecInverter::modelId()
{
    return m_id;
}

void SunSpecInverter::init()
{
    qCDebug(dcSunSpec()) << "SunSpecInverter: Init";
    m_connection->readModelHeader(m_modelModbusStartRegister);
    connect(m_connection, &SunSpec::modelHeaderReceived, this, [this] (uint modbusAddress, SunSpec::ModelId modelId, uint length) {
        if (modelId == m_id) {
            qCDebug(dcSunSpec()) << "SunSpecInverter: Model Header received, modbus address:" << modbusAddress << "model Id:" << modelId << "length:" << length;
            m_modelLength = length;
            emit initFinished(true);
            m_initFinishedSuccess = true;
        }
    });
    QTimer::singleShot(10000, this,[this] {
        if (!m_initFinishedSuccess) {
            emit initFinished(false);
        }
    });
}

void SunSpecInverter::getInverterModelDataBlock()
{
    qCDebug(dcSunSpec()) << "SunSpecInverter: get inverter model data block, modbus register" << m_modelModbusStartRegister << "length" << m_modelLength;
    m_connection->readModelDataBlock(m_modelModbusStartRegister, m_modelLength);
}

SunSpecInverter::SunSpecEvent1 SunSpecInverter::bitfieldToSunSpecEvent1(quint16 register0, quint16 register1)
{
    SunSpecEvent1 event1;
    quint32 value = (static_cast<quint32>(register0)<<16 | register1);
    //qCDebug(dcSunSpec()) << "Event1" << QString::number(value, 16);
    event1.groundFault      = ((value & (0x01 << 0)) != 0);
    event1.dcOverVoltage    = ((value & (0x01 << 1)) != 0);
    event1.acDisconnect     = ((value & (0x01 << 2)) != 0);
    event1.dcDicconnect     = ((value & (0x01 << 3)) != 0);
    event1.gridDisconnect   = ((value & (0x01 << 4)) != 0);
    event1.cabinetOpen      = ((value & (0x01 << 5)) != 0);
    event1.manualShutdown   = ((value & (0x01 << 6)) != 0);
    event1.overTemperature  = ((value & (0x01 << 7)) != 0);
    event1.overFrequency    = ((value & (0x01 << 8)) != 0);
    event1.underFrequency   = ((value & (0x01 << 9)) != 0);
    event1.acOverVolt       = ((value & (0x01 << 10)) != 0);
    event1.acUnderVolt      = ((value & (0x01 << 11)) != 0);
    event1.blownStringFuse  = ((value & (0x01 << 12)) != 0);
    event1.underTemperature = ((value & (0x01 << 13)) != 0);
    event1.memoryLoss       = ((value & (0x01 << 14)) != 0);
    event1.hwTestFailure    = ((value & (0x01 << 15)) != 0);
    return event1;
}

void SunSpecInverter::getInverterModelHeader()
{
    qCDebug(dcSunSpec()) << "SunSpecInverter: get inverter model header, modbus register" << m_modelModbusStartRegister;
    m_connection->readModelHeader(m_modelModbusStartRegister);
}

void SunSpecInverter::onModelDataBlockReceived(SunSpec::ModelId modelId, uint length, QVector<quint16> data)
{
    Q_UNUSED(length)
    if (modelId != m_id) {
        return;
    }
    if (length < m_modelLength) {
        qCDebug(dcSunSpec()) << "SunSpecInverter: on model data block received, model length is too short" << length;
        return;
    }
    InverterData inverterData;

    qCDebug(dcSunSpec()) << "SunSpecInverter: Received" << modelId;
    switch (modelId) {
    case SunSpec::ModelIdInverterSinglePhase:
    case SunSpec::ModelIdInverterSplitPhase:
    case SunSpec::ModelIdInverterThreePhase: {
        inverterData.acCurrent= m_connection->convertToFloatWithSSF(data[Model10X::Model10XAcCurrent], data[Model10X::Model10XAmpereScaleFactor]);
        inverterData.acPower = m_connection->convertToFloatWithSSF(data[Model10X::Model10XACPower], data[Model10X::Model10XWattScaleFactor]);
        inverterData.lineFrequency = m_connection->convertToFloatWithSSF(data[Model10X::Model10XLineFrequency], data[Model10X::Model10XHerzScaleFactor]);

        qint16 ampereScaleFactor = static_cast<qint16>(data[Model10X::Model10XAmpereScaleFactor]);
        qCDebug(dcSunSpec()) << "Ampere scale factor" << ampereScaleFactor;
        inverterData.phaseACurrent = m_connection->convertToFloatWithSSF(data[Model10X::Model10XPhaseACurrent], ampereScaleFactor);
        inverterData.phaseBCurrent = m_connection->convertToFloatWithSSF(data[Model10X::Model10XPhaseBCurrent], ampereScaleFactor);
        inverterData.phaseCCurrent = m_connection->convertToFloatWithSSF(data[Model10X::Model10XPhaseCCurrent], ampereScaleFactor);

        qint16 voltageScaleFactor = static_cast<qint16>(data[Model10X::Model10XVoltageScaleFactor]);
        qCDebug(dcSunSpec()) << "Voltage scale factor" << voltageScaleFactor;
        inverterData.phaseVoltageAN = m_connection->convertToFloatWithSSF(data[Model10X::Model10XPhaseVoltageAN], voltageScaleFactor);
        inverterData.phaseVoltageBN = m_connection->convertToFloatWithSSF(data[Model10X::Model10XPhaseVoltageBN], voltageScaleFactor);
        inverterData.phaseVoltageCN = m_connection->convertToFloatWithSSF(data[Model10X::Model10XPhaseVoltageCN], voltageScaleFactor);

        quint32 acEnergy = ((static_cast<quint32>(data.value(Model10X::Model10XAcEnergy)) << 16) | static_cast<quint32>(data.value(Model10X::Model10XAcEnergy + 1)));
        inverterData.acEnergy = m_connection->convertToFloatWithSSF(acEnergy, data[Model10X::Model10XWattHoursScaleFactor]);

        qint16 temperatureScaleFactor = static_cast<qint16>(data[Model10X::Model10XTemperatureScaleFactor]);
        qCDebug(dcSunSpec()) << "Temperature scale factor" << temperatureScaleFactor << "temperature value" << data[Model10X::Model10XCabinetTemperature];
        inverterData.cabinetTemperature = m_connection->convertToFloatWithSSF(data[Model10X::Model10XCabinetTemperature], temperatureScaleFactor);

        inverterData.event1 = bitfieldToSunSpecEvent1(data[Model10X::Model10XEvent1], data[Model10X::Model10XEvent1+1]);
        inverterData.operatingState = SunSpec::SunSpecOperatingState(data[Model10X::Model10XOperatingState]);
        emit inverterDataReceived(inverterData);

    } break;
    case SunSpec::ModelIdInverterThreePhaseFloat:
    case SunSpec::ModelIdInverterSplitPhaseFloat:
    case SunSpec::ModelIdInverterSinglePhaseFloat: {

        inverterData.acCurrent = m_connection->convertFloatValues(data[Model11X::Model11XAcCurrent], data[Model11X::Model11XAcCurrent+1]);

        inverterData.phaseVoltageAN = m_connection->convertFloatValues(data[Model11X::Model11XPhaseVoltageAN], data[Model11X::Model11XPhaseVoltageAN+1]);
        inverterData.phaseVoltageBN = m_connection->convertFloatValues(data[Model11X::Model11XPhaseVoltageBN], data[Model11X::Model11XPhaseVoltageBN+1]);
        inverterData.phaseVoltageCN = m_connection->convertFloatValues(data[Model11X::Model11XPhaseVoltageCN], data[Model11X::Model11XPhaseVoltageCN+1]);

        inverterData.phaseACurrent  = m_connection->convertFloatValues(data[Model11X::Model11XPhaseACurrent], data[Model11X::Model11XPhaseACurrent+1]);
        inverterData.phaseBCurrent  = m_connection->convertFloatValues(data[Model11X::Model11XPhaseBCurrent], data[Model11X::Model11XPhaseBCurrent+1]);
        inverterData.phaseCCurrent  = m_connection->convertFloatValues(data[Model11X::Model11XPhaseCCurrent], data[Model11X::Model11XPhaseCCurrent+1]);

        inverterData.acPower        = m_connection->convertFloatValues(data[Model11X::Model11XACPower], data[Model11X::Model11XACPower+1]);
        inverterData.lineFrequency  = m_connection->convertFloatValues(data[Model11X::Model11XLineFrequency], data[Model11X::Model11XLineFrequency+1]);

        inverterData.acEnergy       = m_connection->convertFloatValues(data[Model11X::Model11XAcEnergy], data[Model11X::Model11XAcEnergy+1]);
        inverterData.cabinetTemperature = m_connection->convertFloatValues(data[Model11X::Model11XCabinetTemperature], data[Model11X::Model11XCabinetTemperature+1]);
        inverterData.event1 = bitfieldToSunSpecEvent1(data[Model11X::Model11XEvent1], data[Model11X::Model11XEvent1+1]);
        inverterData.operatingState = SunSpec::SunSpecOperatingState(data[Model11X::Model11XOperatingState]);
        emit inverterDataReceived(inverterData);
    } break;
    default:
        //ignore
        break;
    }
}
