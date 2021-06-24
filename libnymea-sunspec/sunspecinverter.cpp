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

#include <QTimer>

SunSpecInverter::SunSpecInverter(SunSpec *sunspec, SunSpec::ModelId modelId, int modbusStartRegister, QObject *parent) :
    SunSpecDevice(sunspec, modelId, modbusStartRegister, parent)
{
    qCDebug(dcSunSpec()) << "Inverter: Setting up inverter";
    connect(m_connection, &SunSpec::modelDataBlockReceived, this, &SunSpecInverter::onModelDataBlockReceived);
}

void SunSpecInverter::init()
{
    qCDebug(dcSunSpec()) << "Inverter: Initialize...";
    connect(m_connection, &SunSpec::modelHeaderReceived, this, [this] (uint modbusAddress, SunSpec::ModelId modelId, uint length) {
        if (modelId == m_modelId) {
            qCDebug(dcSunSpec()) << "Inverter: Model Header received, modbus address:" << modbusAddress << "model Id:" << modelId << "length:" << length;
            m_modelLength = length;
            m_initFinishedSuccess = true;
            emit initFinished(true);
        }
    });

    readModelHeader();

    QTimer::singleShot(10000, this,[this] {
        if (!m_initFinishedSuccess) {
            emit initFinished(false);
        }
    });
}

void SunSpecInverter::readModelHeader()
{
    qCDebug(dcSunSpec()) << "Inverter: get inverter model header from modbus register" << m_modbusStartRegister;
    m_connection->readModelHeader(m_modbusStartRegister);
}

void SunSpecInverter::readBlockData()
{
    qCDebug(dcSunSpec()) << "Inverter: read inverter model data block, modbus register" << m_modbusStartRegister << "length" << m_modelLength;
    m_connection->readModelDataBlock(m_modbusStartRegister, m_modelLength);
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

void SunSpecInverter::onModelDataBlockReceived(SunSpec::ModelId modelId, uint length, QVector<quint16> data)
{
    if (modelId != m_modelId) {
        return;
    }

    if (length < m_modelLength) {
        qCDebug(dcSunSpec()) << "Inverter: on model data block received, model length is too short" << length;
        return;
    }

    qCDebug(dcSunSpec()) << "Inverter: Received data block for" << modelId;
    InverterData inverterData;
    switch (modelId) {
    case SunSpec::ModelIdInverterSinglePhase:
    case SunSpec::ModelIdInverterSplitPhase:
    case SunSpec::ModelIdInverterThreePhase: {
        inverterData.acCurrent= m_connection->convertToFloatWithSSF(data[Model10X::Model10XAcCurrent], data[Model10X::Model10XAmpereScaleFactor]);
        inverterData.acPower = m_connection->convertToFloatWithSSF(data[Model10X::Model10XACPower], data[Model10X::Model10XWattScaleFactor]);
        inverterData.lineFrequency = m_connection->convertToFloatWithSSF(data[Model10X::Model10XLineFrequency], data[Model10X::Model10XHerzScaleFactor]);

        quint16 ampereScaleFactor =  data[Model10X::Model10XAmpereScaleFactor];
        inverterData.phaseACurrent = m_connection->convertToFloatWithSSF(data[Model10X::Model10XPhaseACurrent], ampereScaleFactor);
        inverterData.phaseBCurrent = m_connection->convertToFloatWithSSF(data[Model10X::Model10XPhaseBCurrent], ampereScaleFactor);
        inverterData.phaseCCurrent = m_connection->convertToFloatWithSSF(data[Model10X::Model10XPhaseCCurrent], ampereScaleFactor);
        quint16 voltageScaleFactor = data[Model10X::Model10XVoltageScaleFactor];
        inverterData.phaseVoltageAN = m_connection->convertToFloatWithSSF(data[Model10X::Model10XPhaseVoltageAN], voltageScaleFactor);
        inverterData.phaseVoltageBN = m_connection->convertToFloatWithSSF(data[Model10X::Model10XPhaseVoltageBN], voltageScaleFactor);
        inverterData.phaseVoltageCN = m_connection->convertToFloatWithSSF(data[Model10X::Model10XPhaseVoltageCN], voltageScaleFactor);

        quint32 acEnergy = ((static_cast<quint32>(data.value(Model10X::Model10XAcEnergy))<<16)|static_cast<quint32>(data.value(Model10X::Model10XAcEnergy+1)));
        inverterData.acEnergy = m_connection->convertToFloatWithSSF(acEnergy, data[Model10X::Model10XWattHoursScaleFactor]);

        inverterData.cabinetTemperature = m_connection->convertToFloatWithSSF(data[Model10X::Model10XCabinetTemperature], data[Model10X::Model10XTemperatureScaleFactor]);
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
