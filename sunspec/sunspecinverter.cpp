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

SunSpecInverter::SunSpecInverter(const QHostAddress &hostAddress, uint port, QObject *parent) : SunSpec(hostAddress, port, parent)
{
    connect(m_modbusTcpClient, &QModbusClient::stateChanged, this, [this] (QModbusDevice::State state) {
        if (state == QModbusDevice::ConnectedState) {
            qCDebug(dcSunSpec()) << "Inverter connected successfully";
            QList<BlockId> mapIds;
            mapIds.append(BlockId::InverterSinglePhase);
            mapIds.append(BlockId::InverterSplitPhase);
            mapIds.append(BlockId::InverterThreePhase);
            mapIds.append(BlockId::InverterSinglePhaseFloat);
            mapIds.append(BlockId::InverterSplitPhaseFloat);
            mapIds.append(BlockId::InverterThreePhaseFloat);
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

    connect(this, &SunSpec::mapReceived, this, &SunSpecInverter::onModbusMapReceived);
}

void SunSpecInverter::getInverterMap()
{
    readMap(m_mapModbusStartRegister, m_mapLength);
}

void SunSpecInverter::readInverterBlockHeader()
{
    readMapHeader(m_mapModbusStartRegister);
}

void SunSpecInverter::onModbusMapReceived(SunSpec::BlockId mapId, uint mapLength, QVector<quint16> data)
{
    Q_UNUSED(mapLength)
    switch (mapId) {
    case BlockId::InverterSinglePhase:
    case BlockId::InverterSplitPhase:
    case BlockId::InverterThreePhase: {
        InverterData inverterData;
        inverterData.acCurrent= convertValueWithSSF(data[Model10X::Model10XAcCurrent], data[Model10X::Model10XAmpereScaleFactor]);
        inverterData.acPower = convertValueWithSSF(data[Model10X::Model10XACPower], data[Model10X::Model10XWattScaleFactor]);
        inverterData.lineFrequency = convertValueWithSSF(data[Model10X::Model10XLineFrequency], data[Model10X::Model10XHerzScaleFactor]);

        inverterData.phaseACurrent = convertValueWithSSF(data[Model10X::Model10XPhaseACurrent], data[Model10X::Model10XAmpereScaleFactor]);
        inverterData.phaseBCurrent = convertValueWithSSF(data[Model10X::Model10XPhaseBCurrent], data[Model10X::Model10XAmpereScaleFactor]);
        inverterData.phaseCCurrent = convertValueWithSSF(data[Model10X::Model10XPhaseCCurrent], data[Model10X::Model10XAmpereScaleFactor]);

        inverterData.phaseVoltageAN = convertValueWithSSF(data[Model10X::Model10XPhaseVoltageAN], data[Model10X::Model10XVoltageScaleFactor]);
        inverterData.phaseVoltageBN = convertValueWithSSF(data[Model10X::Model10XPhaseVoltageBN], data[Model10X::Model10XVoltageScaleFactor]);
        inverterData.phaseVoltageCN = convertValueWithSSF(data[Model10X::Model10XPhaseVoltageCN], data[Model10X::Model10XVoltageScaleFactor]);

        inverterData.acEnergy = convertValueWithSSF(data[Model10X::Model10XAcEnergy], data[Model10X::Model10XWattHoursScaleFactor]);

        inverterData.cabinetTemperature = convertValueWithSSF(data[Model10X::Model10XCabinetTemperature], data[Model10X::Model10XTemperatureScaleFactor]);
        inverterData.event = SunSpecEvent1(data[Model10X::Model10XEvent1]);
        inverterData.operatingState = SunSpecOperatingState(data[Model10X::Model10XOperatingState]);
        emit inverterDataReceived(inverterData);

    } break;
    case BlockId::InverterSinglePhaseFloat:
    case BlockId::InverterSplitPhaseFloat:
    case BlockId::InverterThreePhaseFloat: {
        InverterData inverterData;
        inverterData.acCurrent = convertFloatValues(data[Model11X::Model11XAcCurrent], data[Model11X::Model11XAcCurrent+1]);

        inverterData.phaseACurrent = convertFloatValues(data[Model11X::Model11XPhaseACurrent], data[Model11X::Model11XPhaseACurrent+1]);
        inverterData.phaseBCurrent = convertFloatValues(data[Model11X::Model11XPhaseBCurrent], data[Model11X::Model11XPhaseBCurrent+1]);
        inverterData.phaseCCurrent = convertFloatValues(data[Model11X::Model11XPhaseCCurrent], data[Model11X::Model11XPhaseCCurrent+1]);

        inverterData.phaseVoltageAN = convertFloatValues(data[Model11X::Model11XPhaseVoltageAN], data[Model11X::Model11XPhaseVoltageAN+1]);
        inverterData.phaseVoltageBN = convertFloatValues(data[Model11X::Model11XPhaseVoltageBN], data[Model11X::Model11XPhaseVoltageBN+1]);
        inverterData.phaseVoltageCN = convertFloatValues(data[Model11X::Model11XPhaseVoltageCN], data[Model11X::Model11XPhaseVoltageCN+1]);

        inverterData.acPower = convertFloatValues(data[Model11X::Model11XACPower], data[Model11X::Model11XACPower+1]);
        inverterData.lineFrequency = convertFloatValues(data[Model11X::Model11XLineFrequency], data[Model11X::Model11XLineFrequency+1]);

        inverterData.acEnergy = convertFloatValues(data[Model11X::Model11XAcEnergy], data[Model11X::Model11XAcEnergy+1]);
        inverterData.cabinetTemperature = convertFloatValues(data[Model11X::Model11XCabinetTemperature], data[Model11X::Model11XCabinetTemperature+1]);
        inverterData.event = SunSpecEvent1(data[Model11X::Model11XEvent1]);
        inverterData.operatingState = SunSpecOperatingState(data[Model11X::Model11XOperatingState]);
        emit inverterDataReceived(inverterData);
    } break;
    default:
        //ignore
        break;
    }
}
