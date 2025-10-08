/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2025, nymea GmbH
* Contact: contact@nymea.io
*
* This fileDescriptor is part of nymea.
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

#include "sunspecfreqwattmodel.h"
#include "sunspecconnection.h"

SunSpecFreqWattModelRepeatingBlock::SunSpecFreqWattModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecFreqWattModel *parent) :
    SunSpecModelRepeatingBlock(blockIndex, blockSize, modbusStartRegister, parent)
{
    m_parentModel = parent;
    m_byteOrder = parent->byteOrder();
    initDataPoints();
}

QString SunSpecFreqWattModelRepeatingBlock::name() const
{
    return "curve";
}

SunSpecFreqWattModel *SunSpecFreqWattModelRepeatingBlock::parentModel() const
{
    return m_parentModel;
}

quint16 SunSpecFreqWattModelRepeatingBlock::actPt() const
{
    return m_actPt;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setActPt(quint16 actPt)
{
    SunSpecDataPoint dp = m_dataPoints.value("ActPt");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(actPt);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::hz1() const
{
    return m_hz1;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setHz1(float hz1)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz1");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz1, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::w1() const
{
    return m_w1;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setW1(float w1)
{
    SunSpecDataPoint dp = m_dataPoints.value("W1");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w1, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::hz2() const
{
    return m_hz2;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setHz2(float hz2)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz2");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz2, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::w2() const
{
    return m_w2;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setW2(float w2)
{
    SunSpecDataPoint dp = m_dataPoints.value("W2");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w2, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::hz3() const
{
    return m_hz3;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setHz3(float hz3)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz3");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz3, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::w3() const
{
    return m_w3;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setW3(float w3)
{
    SunSpecDataPoint dp = m_dataPoints.value("W3");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w3, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::hz4() const
{
    return m_hz4;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setHz4(float hz4)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz4");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz4, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::w4() const
{
    return m_w4;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setW4(float w4)
{
    SunSpecDataPoint dp = m_dataPoints.value("W4");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w4, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::hz5() const
{
    return m_hz5;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setHz5(float hz5)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz5");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz5, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::w5() const
{
    return m_w5;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setW5(float w5)
{
    SunSpecDataPoint dp = m_dataPoints.value("W5");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w5, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::hz6() const
{
    return m_hz6;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setHz6(float hz6)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz6");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz6, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::w6() const
{
    return m_w6;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setW6(float w6)
{
    SunSpecDataPoint dp = m_dataPoints.value("W6");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w6, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::hz7() const
{
    return m_hz7;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setHz7(float hz7)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz7");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz7, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::w7() const
{
    return m_w7;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setW7(float w7)
{
    SunSpecDataPoint dp = m_dataPoints.value("W7");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w7, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::hz8() const
{
    return m_hz8;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setHz8(float hz8)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz8");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz8, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::w8() const
{
    return m_w8;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setW8(float w8)
{
    SunSpecDataPoint dp = m_dataPoints.value("W8");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w8, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::hz9() const
{
    return m_hz9;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setHz9(float hz9)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz9");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz9, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::w9() const
{
    return m_w9;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setW9(float w9)
{
    SunSpecDataPoint dp = m_dataPoints.value("W9");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w9, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::hz10() const
{
    return m_hz10;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setHz10(float hz10)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz10");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz10, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::w10() const
{
    return m_w10;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setW10(float w10)
{
    SunSpecDataPoint dp = m_dataPoints.value("W10");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w10, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::hz11() const
{
    return m_hz11;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setHz11(float hz11)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz11");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz11, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::w11() const
{
    return m_w11;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setW11(float w11)
{
    SunSpecDataPoint dp = m_dataPoints.value("W11");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w11, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::hz12() const
{
    return m_hz12;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setHz12(float hz12)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz12");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz12, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::w12() const
{
    return m_w12;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setW12(float w12)
{
    SunSpecDataPoint dp = m_dataPoints.value("W12");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w12, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::hz13() const
{
    return m_hz13;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setHz13(float hz13)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz13");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz13, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::w13() const
{
    return m_w13;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setW13(float w13)
{
    SunSpecDataPoint dp = m_dataPoints.value("W13");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w13, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::hz14() const
{
    return m_hz14;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setHz14(float hz14)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz14");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz14, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::w14() const
{
    return m_w14;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setW14(float w14)
{
    SunSpecDataPoint dp = m_dataPoints.value("W14");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w14, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::hz15() const
{
    return m_hz15;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setHz15(float hz15)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz15");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz15, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::w15() const
{
    return m_w15;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setW15(float w15)
{
    SunSpecDataPoint dp = m_dataPoints.value("W15");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w15, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::hz16() const
{
    return m_hz16;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setHz16(float hz16)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz16");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz16, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::w16() const
{
    return m_w16;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setW16(float w16)
{
    SunSpecDataPoint dp = m_dataPoints.value("W16");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w16, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::hz17() const
{
    return m_hz17;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setHz17(float hz17)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz17");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz17, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::w17() const
{
    return m_w17;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setW17(float w17)
{
    SunSpecDataPoint dp = m_dataPoints.value("W17");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w17, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::hz18() const
{
    return m_hz18;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setHz18(float hz18)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz18");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz18, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::w18() const
{
    return m_w18;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setW18(float w18)
{
    SunSpecDataPoint dp = m_dataPoints.value("W18");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w18, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::hz19() const
{
    return m_hz19;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setHz19(float hz19)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz19");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz19, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::w19() const
{
    return m_w19;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setW19(float w19)
{
    SunSpecDataPoint dp = m_dataPoints.value("W19");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w19, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::hz20() const
{
    return m_hz20;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setHz20(float hz20)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz20");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz20, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::w20() const
{
    return m_w20;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setW20(float w20)
{
    SunSpecDataPoint dp = m_dataPoints.value("W20");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w20, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

QString SunSpecFreqWattModelRepeatingBlock::crvNam() const
{
    return m_crvNam;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setCrvNam(const QString &crvNam)
{
    SunSpecDataPoint dp = m_dataPoints.value("CrvNam");
    QVector<quint16> registers = SunSpecDataPoint::convertFromString(crvNam, dp.size());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

quint16 SunSpecFreqWattModelRepeatingBlock::rmpPt1Tms() const
{
    return m_rmpPt1Tms;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setRmpPt1Tms(quint16 rmpPt1Tms)
{
    SunSpecDataPoint dp = m_dataPoints.value("RmpPT1Tms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(rmpPt1Tms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::rmpDecTmm() const
{
    return m_rmpDecTmm;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setRmpDecTmm(float rmpDecTmm)
{
    SunSpecDataPoint dp = m_dataPoints.value("RmpDecTmm");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(rmpDecTmm, m_parentModel->rmpIncDecSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::rmpIncTmm() const
{
    return m_rmpIncTmm;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setRmpIncTmm(float rmpIncTmm)
{
    SunSpecDataPoint dp = m_dataPoints.value("RmpIncTmm");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(rmpIncTmm, m_parentModel->rmpIncDecSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::rmpRsUp() const
{
    return m_rmpRsUp;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setRmpRsUp(float rmpRsUp)
{
    SunSpecDataPoint dp = m_dataPoints.value("RmpRsUp");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(rmpRsUp, m_parentModel->rmpIncDecSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

quint16 SunSpecFreqWattModelRepeatingBlock::snptW() const
{
    return m_snptW;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setSnptW(quint16 snptW)
{
    SunSpecDataPoint dp = m_dataPoints.value("SnptW");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(snptW));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::wRef() const
{
    return m_wRef;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setWRef(float wRef)
{
    SunSpecDataPoint dp = m_dataPoints.value("WRef");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(wRef, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::wRefStrHz() const
{
    return m_wRefStrHz;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setWRefStrHz(float wRefStrHz)
{
    SunSpecDataPoint dp = m_dataPoints.value("WRefStrHz");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(wRefStrHz, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecFreqWattModelRepeatingBlock::wRefStopHz() const
{
    return m_wRefStopHz;
}

QModbusReply *SunSpecFreqWattModelRepeatingBlock::setWRefStopHz(float wRefStopHz)
{
    SunSpecDataPoint dp = m_dataPoints.value("WRefStopHz");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(wRefStopHz, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

SunSpecFreqWattModelRepeatingBlock::Readonly SunSpecFreqWattModelRepeatingBlock::readOnly() const
{
    return m_readOnly;
}

void SunSpecFreqWattModelRepeatingBlock::initDataPoints()
{
    SunSpecDataPoint actPtDataPoint;
    actPtDataPoint.setName("ActPt");
    actPtDataPoint.setLabel("ActPt");
    actPtDataPoint.setDescription("Number of active points in array.");
    actPtDataPoint.setMandatory(true);
    actPtDataPoint.setSize(1);
    actPtDataPoint.setAddressOffset(0);
    actPtDataPoint.setSunSpecDataType("uint16");
    actPtDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    actPtDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(actPtDataPoint.name(), actPtDataPoint);

    SunSpecDataPoint hz1DataPoint;
    hz1DataPoint.setName("Hz1");
    hz1DataPoint.setLabel("Hz1");
    hz1DataPoint.setDescription("Point 1 Hertz.");
    hz1DataPoint.setUnits("Hz");
    hz1DataPoint.setMandatory(true);
    hz1DataPoint.setSize(1);
    hz1DataPoint.setAddressOffset(1);
    hz1DataPoint.setScaleFactorName("Hz_SF");
    hz1DataPoint.setSunSpecDataType("uint16");
    hz1DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hz1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hz1DataPoint.name(), hz1DataPoint);

    SunSpecDataPoint w1DataPoint;
    w1DataPoint.setName("W1");
    w1DataPoint.setLabel("W1");
    w1DataPoint.setDescription("Point 1 Watts.");
    w1DataPoint.setUnits("% WRef");
    w1DataPoint.setMandatory(true);
    w1DataPoint.setSize(1);
    w1DataPoint.setAddressOffset(2);
    w1DataPoint.setBlockOffset(0);
    w1DataPoint.setScaleFactorName("W_SF");
    w1DataPoint.setSunSpecDataType("int16");
    w1DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w1DataPoint.name(), w1DataPoint);

    SunSpecDataPoint hz2DataPoint;
    hz2DataPoint.setName("Hz2");
    hz2DataPoint.setLabel("Hz2");
    hz2DataPoint.setDescription("Point 2 Hertz.");
    hz2DataPoint.setUnits("Hz");
    hz2DataPoint.setSize(1);
    hz2DataPoint.setAddressOffset(3);
    hz2DataPoint.setBlockOffset(1);
    hz2DataPoint.setScaleFactorName("Hz_SF");
    hz2DataPoint.setSunSpecDataType("uint16");
    hz2DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hz2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hz2DataPoint.name(), hz2DataPoint);

    SunSpecDataPoint w2DataPoint;
    w2DataPoint.setName("W2");
    w2DataPoint.setLabel("W2");
    w2DataPoint.setDescription("Point 2 Watts.");
    w2DataPoint.setUnits("% WRef");
    w2DataPoint.setSize(1);
    w2DataPoint.setAddressOffset(4);
    w2DataPoint.setBlockOffset(2);
    w2DataPoint.setScaleFactorName("W_SF");
    w2DataPoint.setSunSpecDataType("int16");
    w2DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w2DataPoint.name(), w2DataPoint);

    SunSpecDataPoint hz3DataPoint;
    hz3DataPoint.setName("Hz3");
    hz3DataPoint.setLabel("Hz3");
    hz3DataPoint.setDescription("Point 3 Hertz.");
    hz3DataPoint.setUnits("Hz");
    hz3DataPoint.setSize(1);
    hz3DataPoint.setAddressOffset(5);
    hz3DataPoint.setBlockOffset(3);
    hz3DataPoint.setScaleFactorName("Hz_SF");
    hz3DataPoint.setSunSpecDataType("uint16");
    hz3DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hz3DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hz3DataPoint.name(), hz3DataPoint);

    SunSpecDataPoint w3DataPoint;
    w3DataPoint.setName("W3");
    w3DataPoint.setLabel("W3");
    w3DataPoint.setDescription("Point 3 Watts.");
    w3DataPoint.setUnits("% WRef");
    w3DataPoint.setSize(1);
    w3DataPoint.setAddressOffset(6);
    w3DataPoint.setBlockOffset(4);
    w3DataPoint.setScaleFactorName("W_SF");
    w3DataPoint.setSunSpecDataType("int16");
    w3DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w3DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w3DataPoint.name(), w3DataPoint);

    SunSpecDataPoint hz4DataPoint;
    hz4DataPoint.setName("Hz4");
    hz4DataPoint.setLabel("Hz4");
    hz4DataPoint.setDescription("Point 4 Hertz.");
    hz4DataPoint.setUnits("Hz");
    hz4DataPoint.setSize(1);
    hz4DataPoint.setAddressOffset(7);
    hz4DataPoint.setBlockOffset(5);
    hz4DataPoint.setScaleFactorName("Hz_SF");
    hz4DataPoint.setSunSpecDataType("uint16");
    hz4DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hz4DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hz4DataPoint.name(), hz4DataPoint);

    SunSpecDataPoint w4DataPoint;
    w4DataPoint.setName("W4");
    w4DataPoint.setLabel("W4");
    w4DataPoint.setDescription("Point 4 Watts.");
    w4DataPoint.setUnits("% WRef");
    w4DataPoint.setSize(1);
    w4DataPoint.setAddressOffset(8);
    w4DataPoint.setBlockOffset(6);
    w4DataPoint.setScaleFactorName("W_SF");
    w4DataPoint.setSunSpecDataType("int16");
    w4DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w4DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w4DataPoint.name(), w4DataPoint);

    SunSpecDataPoint hz5DataPoint;
    hz5DataPoint.setName("Hz5");
    hz5DataPoint.setLabel("Hz5");
    hz5DataPoint.setDescription("Point 5 Hertz.");
    hz5DataPoint.setUnits("Hz");
    hz5DataPoint.setSize(1);
    hz5DataPoint.setAddressOffset(9);
    hz5DataPoint.setBlockOffset(7);
    hz5DataPoint.setScaleFactorName("Hz_SF");
    hz5DataPoint.setSunSpecDataType("uint16");
    hz5DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hz5DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hz5DataPoint.name(), hz5DataPoint);

    SunSpecDataPoint w5DataPoint;
    w5DataPoint.setName("W5");
    w5DataPoint.setLabel("W5");
    w5DataPoint.setDescription("Point 5 Watts.");
    w5DataPoint.setUnits("% WRef");
    w5DataPoint.setSize(1);
    w5DataPoint.setAddressOffset(10);
    w5DataPoint.setBlockOffset(8);
    w5DataPoint.setScaleFactorName("W_SF");
    w5DataPoint.setSunSpecDataType("int16");
    w5DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w5DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w5DataPoint.name(), w5DataPoint);

    SunSpecDataPoint hz6DataPoint;
    hz6DataPoint.setName("Hz6");
    hz6DataPoint.setLabel("Hz6");
    hz6DataPoint.setDescription("Point 6 Hertz.");
    hz6DataPoint.setUnits("Hz");
    hz6DataPoint.setSize(1);
    hz6DataPoint.setAddressOffset(11);
    hz6DataPoint.setBlockOffset(9);
    hz6DataPoint.setScaleFactorName("Hz_SF");
    hz6DataPoint.setSunSpecDataType("uint16");
    hz6DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hz6DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hz6DataPoint.name(), hz6DataPoint);

    SunSpecDataPoint w6DataPoint;
    w6DataPoint.setName("W6");
    w6DataPoint.setLabel("W6");
    w6DataPoint.setDescription("Point 6 Watts.");
    w6DataPoint.setUnits("% WRef");
    w6DataPoint.setSize(1);
    w6DataPoint.setAddressOffset(12);
    w6DataPoint.setBlockOffset(10);
    w6DataPoint.setScaleFactorName("W_SF");
    w6DataPoint.setSunSpecDataType("int16");
    w6DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w6DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w6DataPoint.name(), w6DataPoint);

    SunSpecDataPoint hz7DataPoint;
    hz7DataPoint.setName("Hz7");
    hz7DataPoint.setLabel("Hz7");
    hz7DataPoint.setDescription("Point 7 Hertz.");
    hz7DataPoint.setUnits("Hz");
    hz7DataPoint.setSize(1);
    hz7DataPoint.setAddressOffset(13);
    hz7DataPoint.setBlockOffset(11);
    hz7DataPoint.setScaleFactorName("Hz_SF");
    hz7DataPoint.setSunSpecDataType("uint16");
    hz7DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hz7DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hz7DataPoint.name(), hz7DataPoint);

    SunSpecDataPoint w7DataPoint;
    w7DataPoint.setName("W7");
    w7DataPoint.setLabel("W7");
    w7DataPoint.setDescription("Point 7 Watts.");
    w7DataPoint.setUnits("% WRef");
    w7DataPoint.setSize(1);
    w7DataPoint.setAddressOffset(14);
    w7DataPoint.setBlockOffset(12);
    w7DataPoint.setScaleFactorName("W_SF");
    w7DataPoint.setSunSpecDataType("int16");
    w7DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w7DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w7DataPoint.name(), w7DataPoint);

    SunSpecDataPoint hz8DataPoint;
    hz8DataPoint.setName("Hz8");
    hz8DataPoint.setLabel("Hz8");
    hz8DataPoint.setDescription("Point 8 Hertz.");
    hz8DataPoint.setUnits("Hz");
    hz8DataPoint.setSize(1);
    hz8DataPoint.setAddressOffset(15);
    hz8DataPoint.setBlockOffset(13);
    hz8DataPoint.setScaleFactorName("Hz_SF");
    hz8DataPoint.setSunSpecDataType("uint16");
    hz8DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hz8DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hz8DataPoint.name(), hz8DataPoint);

    SunSpecDataPoint w8DataPoint;
    w8DataPoint.setName("W8");
    w8DataPoint.setLabel("W8");
    w8DataPoint.setDescription("Point 8 Watts.");
    w8DataPoint.setUnits("% WRef");
    w8DataPoint.setSize(1);
    w8DataPoint.setAddressOffset(16);
    w8DataPoint.setBlockOffset(14);
    w8DataPoint.setScaleFactorName("W_SF");
    w8DataPoint.setSunSpecDataType("int16");
    w8DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w8DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w8DataPoint.name(), w8DataPoint);

    SunSpecDataPoint hz9DataPoint;
    hz9DataPoint.setName("Hz9");
    hz9DataPoint.setLabel("Hz9");
    hz9DataPoint.setDescription("Point 9 Hertz.");
    hz9DataPoint.setUnits("Hz");
    hz9DataPoint.setSize(1);
    hz9DataPoint.setAddressOffset(17);
    hz9DataPoint.setBlockOffset(15);
    hz9DataPoint.setScaleFactorName("Hz_SF");
    hz9DataPoint.setSunSpecDataType("uint16");
    hz9DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hz9DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hz9DataPoint.name(), hz9DataPoint);

    SunSpecDataPoint w9DataPoint;
    w9DataPoint.setName("W9");
    w9DataPoint.setLabel("W9");
    w9DataPoint.setDescription("Point 9 Watts.");
    w9DataPoint.setUnits("% WRef");
    w9DataPoint.setSize(1);
    w9DataPoint.setAddressOffset(18);
    w9DataPoint.setBlockOffset(16);
    w9DataPoint.setScaleFactorName("W_SF");
    w9DataPoint.setSunSpecDataType("int16");
    w9DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w9DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w9DataPoint.name(), w9DataPoint);

    SunSpecDataPoint hz10DataPoint;
    hz10DataPoint.setName("Hz10");
    hz10DataPoint.setLabel("Hz10");
    hz10DataPoint.setDescription("Point 10 Hertz.");
    hz10DataPoint.setUnits("Hz");
    hz10DataPoint.setSize(1);
    hz10DataPoint.setAddressOffset(19);
    hz10DataPoint.setBlockOffset(17);
    hz10DataPoint.setScaleFactorName("Hz_SF");
    hz10DataPoint.setSunSpecDataType("uint16");
    hz10DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hz10DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hz10DataPoint.name(), hz10DataPoint);

    SunSpecDataPoint w10DataPoint;
    w10DataPoint.setName("W10");
    w10DataPoint.setLabel("W10");
    w10DataPoint.setDescription("Point 10 Watts.");
    w10DataPoint.setUnits("% WRef");
    w10DataPoint.setSize(1);
    w10DataPoint.setAddressOffset(20);
    w10DataPoint.setBlockOffset(18);
    w10DataPoint.setScaleFactorName("W_SF");
    w10DataPoint.setSunSpecDataType("int16");
    w10DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w10DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w10DataPoint.name(), w10DataPoint);

    SunSpecDataPoint hz11DataPoint;
    hz11DataPoint.setName("Hz11");
    hz11DataPoint.setLabel("Hz11");
    hz11DataPoint.setDescription("Point 11 Hertz.");
    hz11DataPoint.setUnits("Hz");
    hz11DataPoint.setSize(1);
    hz11DataPoint.setAddressOffset(21);
    hz11DataPoint.setBlockOffset(19);
    hz11DataPoint.setScaleFactorName("Hz_SF");
    hz11DataPoint.setSunSpecDataType("uint16");
    hz11DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hz11DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hz11DataPoint.name(), hz11DataPoint);

    SunSpecDataPoint w11DataPoint;
    w11DataPoint.setName("W11");
    w11DataPoint.setLabel("W11");
    w11DataPoint.setDescription("Point 11 Watts.");
    w11DataPoint.setUnits("% WRef");
    w11DataPoint.setSize(1);
    w11DataPoint.setAddressOffset(22);
    w11DataPoint.setBlockOffset(20);
    w11DataPoint.setScaleFactorName("W_SF");
    w11DataPoint.setSunSpecDataType("int16");
    w11DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w11DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w11DataPoint.name(), w11DataPoint);

    SunSpecDataPoint hz12DataPoint;
    hz12DataPoint.setName("Hz12");
    hz12DataPoint.setLabel("Hz12");
    hz12DataPoint.setDescription("Point 12 Hertz.");
    hz12DataPoint.setUnits("Hz");
    hz12DataPoint.setSize(1);
    hz12DataPoint.setAddressOffset(23);
    hz12DataPoint.setBlockOffset(21);
    hz12DataPoint.setScaleFactorName("Hz_SF");
    hz12DataPoint.setSunSpecDataType("uint16");
    hz12DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hz12DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hz12DataPoint.name(), hz12DataPoint);

    SunSpecDataPoint w12DataPoint;
    w12DataPoint.setName("W12");
    w12DataPoint.setLabel("W12");
    w12DataPoint.setDescription("Point 12 Watts.");
    w12DataPoint.setUnits("% WRef");
    w12DataPoint.setSize(1);
    w12DataPoint.setAddressOffset(24);
    w12DataPoint.setBlockOffset(22);
    w12DataPoint.setScaleFactorName("W_SF");
    w12DataPoint.setSunSpecDataType("int16");
    w12DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w12DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w12DataPoint.name(), w12DataPoint);

    SunSpecDataPoint hz13DataPoint;
    hz13DataPoint.setName("Hz13");
    hz13DataPoint.setLabel("Hz13");
    hz13DataPoint.setDescription("Point 13 Hertz.");
    hz13DataPoint.setUnits("Hz");
    hz13DataPoint.setSize(1);
    hz13DataPoint.setAddressOffset(25);
    hz13DataPoint.setBlockOffset(23);
    hz13DataPoint.setScaleFactorName("Hz_SF");
    hz13DataPoint.setSunSpecDataType("uint16");
    hz13DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hz13DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hz13DataPoint.name(), hz13DataPoint);

    SunSpecDataPoint w13DataPoint;
    w13DataPoint.setName("W13");
    w13DataPoint.setLabel("W13");
    w13DataPoint.setDescription("Point 13 Watts.");
    w13DataPoint.setUnits("% WRef");
    w13DataPoint.setSize(1);
    w13DataPoint.setAddressOffset(26);
    w13DataPoint.setBlockOffset(24);
    w13DataPoint.setScaleFactorName("W_SF");
    w13DataPoint.setSunSpecDataType("int16");
    w13DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w13DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w13DataPoint.name(), w13DataPoint);

    SunSpecDataPoint hz14DataPoint;
    hz14DataPoint.setName("Hz14");
    hz14DataPoint.setLabel("Hz14");
    hz14DataPoint.setDescription("Point 14 Hertz.");
    hz14DataPoint.setUnits("Hz");
    hz14DataPoint.setSize(1);
    hz14DataPoint.setAddressOffset(27);
    hz14DataPoint.setBlockOffset(25);
    hz14DataPoint.setScaleFactorName("Hz_SF");
    hz14DataPoint.setSunSpecDataType("uint16");
    hz14DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hz14DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hz14DataPoint.name(), hz14DataPoint);

    SunSpecDataPoint w14DataPoint;
    w14DataPoint.setName("W14");
    w14DataPoint.setLabel("W14");
    w14DataPoint.setDescription("Point 14 Watts.");
    w14DataPoint.setUnits("% WRef");
    w14DataPoint.setSize(1);
    w14DataPoint.setAddressOffset(28);
    w14DataPoint.setBlockOffset(26);
    w14DataPoint.setScaleFactorName("W_SF");
    w14DataPoint.setSunSpecDataType("int16");
    w14DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w14DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w14DataPoint.name(), w14DataPoint);

    SunSpecDataPoint hz15DataPoint;
    hz15DataPoint.setName("Hz15");
    hz15DataPoint.setLabel("Hz15");
    hz15DataPoint.setDescription("Point 15 Hertz.");
    hz15DataPoint.setUnits("Hz");
    hz15DataPoint.setSize(1);
    hz15DataPoint.setAddressOffset(29);
    hz15DataPoint.setBlockOffset(27);
    hz15DataPoint.setScaleFactorName("Hz_SF");
    hz15DataPoint.setSunSpecDataType("uint16");
    hz15DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hz15DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hz15DataPoint.name(), hz15DataPoint);

    SunSpecDataPoint w15DataPoint;
    w15DataPoint.setName("W15");
    w15DataPoint.setLabel("W15");
    w15DataPoint.setDescription("Point 15 Watts.");
    w15DataPoint.setUnits("% WRef");
    w15DataPoint.setSize(1);
    w15DataPoint.setAddressOffset(30);
    w15DataPoint.setBlockOffset(28);
    w15DataPoint.setScaleFactorName("W_SF");
    w15DataPoint.setSunSpecDataType("int16");
    w15DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w15DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w15DataPoint.name(), w15DataPoint);

    SunSpecDataPoint hz16DataPoint;
    hz16DataPoint.setName("Hz16");
    hz16DataPoint.setLabel("Hz16");
    hz16DataPoint.setDescription("Point 16 Hertz.");
    hz16DataPoint.setUnits("Hz");
    hz16DataPoint.setSize(1);
    hz16DataPoint.setAddressOffset(31);
    hz16DataPoint.setBlockOffset(29);
    hz16DataPoint.setScaleFactorName("Hz_SF");
    hz16DataPoint.setSunSpecDataType("uint16");
    hz16DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hz16DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hz16DataPoint.name(), hz16DataPoint);

    SunSpecDataPoint w16DataPoint;
    w16DataPoint.setName("W16");
    w16DataPoint.setLabel("W16");
    w16DataPoint.setDescription("Point 16 Watts.");
    w16DataPoint.setUnits("% WRef");
    w16DataPoint.setSize(1);
    w16DataPoint.setAddressOffset(32);
    w16DataPoint.setBlockOffset(30);
    w16DataPoint.setScaleFactorName("W_SF");
    w16DataPoint.setSunSpecDataType("int16");
    w16DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w16DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w16DataPoint.name(), w16DataPoint);

    SunSpecDataPoint hz17DataPoint;
    hz17DataPoint.setName("Hz17");
    hz17DataPoint.setLabel("Hz17");
    hz17DataPoint.setDescription("Point 17 Hertz.");
    hz17DataPoint.setUnits("Hz");
    hz17DataPoint.setSize(1);
    hz17DataPoint.setAddressOffset(33);
    hz17DataPoint.setBlockOffset(31);
    hz17DataPoint.setScaleFactorName("Hz_SF");
    hz17DataPoint.setSunSpecDataType("uint16");
    hz17DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hz17DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hz17DataPoint.name(), hz17DataPoint);

    SunSpecDataPoint w17DataPoint;
    w17DataPoint.setName("W17");
    w17DataPoint.setLabel("W17");
    w17DataPoint.setDescription("Point 17 Watts.");
    w17DataPoint.setUnits("% WRef");
    w17DataPoint.setSize(1);
    w17DataPoint.setAddressOffset(34);
    w17DataPoint.setBlockOffset(32);
    w17DataPoint.setScaleFactorName("W_SF");
    w17DataPoint.setSunSpecDataType("int16");
    w17DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w17DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w17DataPoint.name(), w17DataPoint);

    SunSpecDataPoint hz18DataPoint;
    hz18DataPoint.setName("Hz18");
    hz18DataPoint.setLabel("Hz18");
    hz18DataPoint.setDescription("Point 18 Hertz.");
    hz18DataPoint.setUnits("Hz");
    hz18DataPoint.setSize(1);
    hz18DataPoint.setAddressOffset(35);
    hz18DataPoint.setBlockOffset(33);
    hz18DataPoint.setScaleFactorName("Hz_SF");
    hz18DataPoint.setSunSpecDataType("uint16");
    hz18DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hz18DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hz18DataPoint.name(), hz18DataPoint);

    SunSpecDataPoint w18DataPoint;
    w18DataPoint.setName("W18");
    w18DataPoint.setLabel("W18");
    w18DataPoint.setDescription("Point 18 Watts.");
    w18DataPoint.setUnits("% WRef");
    w18DataPoint.setSize(1);
    w18DataPoint.setAddressOffset(36);
    w18DataPoint.setBlockOffset(34);
    w18DataPoint.setScaleFactorName("W_SF");
    w18DataPoint.setSunSpecDataType("int16");
    w18DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w18DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w18DataPoint.name(), w18DataPoint);

    SunSpecDataPoint hz19DataPoint;
    hz19DataPoint.setName("Hz19");
    hz19DataPoint.setLabel("Hz19");
    hz19DataPoint.setDescription("Point 19 Hertz.");
    hz19DataPoint.setUnits("Hz");
    hz19DataPoint.setSize(1);
    hz19DataPoint.setAddressOffset(37);
    hz19DataPoint.setBlockOffset(35);
    hz19DataPoint.setScaleFactorName("Hz_SF");
    hz19DataPoint.setSunSpecDataType("uint16");
    hz19DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hz19DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hz19DataPoint.name(), hz19DataPoint);

    SunSpecDataPoint w19DataPoint;
    w19DataPoint.setName("W19");
    w19DataPoint.setLabel("W19");
    w19DataPoint.setDescription("Point 19 Watts.");
    w19DataPoint.setUnits("% WRef");
    w19DataPoint.setSize(1);
    w19DataPoint.setAddressOffset(38);
    w19DataPoint.setBlockOffset(36);
    w19DataPoint.setScaleFactorName("W_SF");
    w19DataPoint.setSunSpecDataType("int16");
    w19DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w19DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w19DataPoint.name(), w19DataPoint);

    SunSpecDataPoint hz20DataPoint;
    hz20DataPoint.setName("Hz20");
    hz20DataPoint.setLabel("Hz20");
    hz20DataPoint.setDescription("Point 20 Hertz.");
    hz20DataPoint.setUnits("Hz");
    hz20DataPoint.setSize(1);
    hz20DataPoint.setAddressOffset(39);
    hz20DataPoint.setBlockOffset(37);
    hz20DataPoint.setScaleFactorName("Hz_SF");
    hz20DataPoint.setSunSpecDataType("uint16");
    hz20DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    hz20DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hz20DataPoint.name(), hz20DataPoint);

    SunSpecDataPoint w20DataPoint;
    w20DataPoint.setName("W20");
    w20DataPoint.setLabel("W20");
    w20DataPoint.setDescription("Point 20 Watts.");
    w20DataPoint.setUnits("% WRef");
    w20DataPoint.setSize(1);
    w20DataPoint.setAddressOffset(40);
    w20DataPoint.setBlockOffset(38);
    w20DataPoint.setScaleFactorName("W_SF");
    w20DataPoint.setSunSpecDataType("int16");
    w20DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w20DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w20DataPoint.name(), w20DataPoint);

    SunSpecDataPoint crvNamDataPoint;
    crvNamDataPoint.setName("CrvNam");
    crvNamDataPoint.setLabel("CrvNam");
    crvNamDataPoint.setDescription("Optional description for curve. (Max 16 chars)");
    crvNamDataPoint.setSize(8);
    crvNamDataPoint.setAddressOffset(41);
    crvNamDataPoint.setBlockOffset(39);
    crvNamDataPoint.setSunSpecDataType("string");
    crvNamDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    crvNamDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(crvNamDataPoint.name(), crvNamDataPoint);

    SunSpecDataPoint rmpPt1TmsDataPoint;
    rmpPt1TmsDataPoint.setName("RmpPT1Tms");
    rmpPt1TmsDataPoint.setLabel("RmpPT1Tms");
    rmpPt1TmsDataPoint.setDescription("The time of the PT1 in seconds (time to accomplish a change of 95%).");
    rmpPt1TmsDataPoint.setUnits("Secs");
    rmpPt1TmsDataPoint.setSize(1);
    rmpPt1TmsDataPoint.setAddressOffset(49);
    rmpPt1TmsDataPoint.setBlockOffset(47);
    rmpPt1TmsDataPoint.setSunSpecDataType("uint16");
    rmpPt1TmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    rmpPt1TmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(rmpPt1TmsDataPoint.name(), rmpPt1TmsDataPoint);

    SunSpecDataPoint rmpDecTmmDataPoint;
    rmpDecTmmDataPoint.setName("RmpDecTmm");
    rmpDecTmmDataPoint.setLabel("RmpDecTmm");
    rmpDecTmmDataPoint.setDescription("The maximum rate at which the power value may be reduced in response to changes in the frequency value.");
    rmpDecTmmDataPoint.setUnits("% WMax/min");
    rmpDecTmmDataPoint.setSize(1);
    rmpDecTmmDataPoint.setAddressOffset(50);
    rmpDecTmmDataPoint.setBlockOffset(48);
    rmpDecTmmDataPoint.setScaleFactorName("RmpIncDec_SF");
    rmpDecTmmDataPoint.setSunSpecDataType("uint16");
    rmpDecTmmDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    rmpDecTmmDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(rmpDecTmmDataPoint.name(), rmpDecTmmDataPoint);

    SunSpecDataPoint rmpIncTmmDataPoint;
    rmpIncTmmDataPoint.setName("RmpIncTmm");
    rmpIncTmmDataPoint.setLabel("RmpIncTmm");
    rmpIncTmmDataPoint.setDescription("The maximum rate at which the power value may be increased in response to changes in the frequency value.");
    rmpIncTmmDataPoint.setUnits("% WMax/min");
    rmpIncTmmDataPoint.setSize(1);
    rmpIncTmmDataPoint.setAddressOffset(51);
    rmpIncTmmDataPoint.setBlockOffset(49);
    rmpIncTmmDataPoint.setScaleFactorName("RmpIncDec_SF");
    rmpIncTmmDataPoint.setSunSpecDataType("uint16");
    rmpIncTmmDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    rmpIncTmmDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(rmpIncTmmDataPoint.name(), rmpIncTmmDataPoint);

    SunSpecDataPoint rmpRsUpDataPoint;
    rmpRsUpDataPoint.setName("RmpRsUp");
    rmpRsUpDataPoint.setLabel("RmpRsUp");
    rmpRsUpDataPoint.setDescription("The maximum rate at which the power may be increased after releasing the frozen value of snap shot function. ");
    rmpRsUpDataPoint.setUnits("% WMax/min");
    rmpRsUpDataPoint.setSize(1);
    rmpRsUpDataPoint.setAddressOffset(52);
    rmpRsUpDataPoint.setBlockOffset(50);
    rmpRsUpDataPoint.setScaleFactorName("RmpIncDec_SF");
    rmpRsUpDataPoint.setSunSpecDataType("uint16");
    rmpRsUpDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    rmpRsUpDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(rmpRsUpDataPoint.name(), rmpRsUpDataPoint);

    SunSpecDataPoint snptWDataPoint;
    snptWDataPoint.setName("SnptW");
    snptWDataPoint.setLabel("SnptW");
    snptWDataPoint.setDescription("1=enable snapshot/capture mode");
    snptWDataPoint.setMandatory(true);
    snptWDataPoint.setSize(1);
    snptWDataPoint.setAddressOffset(53);
    snptWDataPoint.setBlockOffset(51);
    snptWDataPoint.setSunSpecDataType("bitfield16");
    snptWDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    snptWDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(snptWDataPoint.name(), snptWDataPoint);

    SunSpecDataPoint wRefDataPoint;
    wRefDataPoint.setName("WRef");
    wRefDataPoint.setLabel("WRef");
    wRefDataPoint.setDescription("Reference active power (default = WMax).");
    wRefDataPoint.setUnits("W");
    wRefDataPoint.setSize(1);
    wRefDataPoint.setAddressOffset(54);
    wRefDataPoint.setBlockOffset(52);
    wRefDataPoint.setScaleFactorName("W_SF");
    wRefDataPoint.setSunSpecDataType("uint16");
    wRefDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    wRefDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wRefDataPoint.name(), wRefDataPoint);

    SunSpecDataPoint wRefStrHzDataPoint;
    wRefStrHzDataPoint.setName("WRefStrHz");
    wRefStrHzDataPoint.setLabel("WRefStrHz");
    wRefStrHzDataPoint.setDescription("Frequency deviation from nominal frequency at the time of the snapshot to start constraining power output.");
    wRefStrHzDataPoint.setUnits("Hz");
    wRefStrHzDataPoint.setSize(1);
    wRefStrHzDataPoint.setAddressOffset(55);
    wRefStrHzDataPoint.setBlockOffset(53);
    wRefStrHzDataPoint.setScaleFactorName("Hz_SF");
    wRefStrHzDataPoint.setSunSpecDataType("uint16");
    wRefStrHzDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    wRefStrHzDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wRefStrHzDataPoint.name(), wRefStrHzDataPoint);

    SunSpecDataPoint wRefStopHzDataPoint;
    wRefStopHzDataPoint.setName("WRefStopHz");
    wRefStopHzDataPoint.setLabel("WRefStopHz");
    wRefStopHzDataPoint.setDescription("Frequency deviation from nominal frequency at which to release the power output.");
    wRefStopHzDataPoint.setUnits("Hz");
    wRefStopHzDataPoint.setSize(1);
    wRefStopHzDataPoint.setAddressOffset(56);
    wRefStopHzDataPoint.setBlockOffset(54);
    wRefStopHzDataPoint.setScaleFactorName("Hz_SF");
    wRefStopHzDataPoint.setSunSpecDataType("uint16");
    wRefStopHzDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    wRefStopHzDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wRefStopHzDataPoint.name(), wRefStopHzDataPoint);

    SunSpecDataPoint readOnlyDataPoint;
    readOnlyDataPoint.setName("ReadOnly");
    readOnlyDataPoint.setLabel("ReadOnly");
    readOnlyDataPoint.setDescription("Enumerated value indicates if curve is read-only or can be modified.");
    readOnlyDataPoint.setMandatory(true);
    readOnlyDataPoint.setSize(1);
    readOnlyDataPoint.setAddressOffset(57);
    readOnlyDataPoint.setBlockOffset(55);
    readOnlyDataPoint.setSunSpecDataType("enum16");
    readOnlyDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(readOnlyDataPoint.name(), readOnlyDataPoint);

}

void SunSpecFreqWattModelRepeatingBlock::processBlockData()
{
    // Update properties according to the data point type
    if (m_dataPoints.value("ActPt").isValid())
        m_actPt = m_dataPoints.value("ActPt").toUInt16();

    if (m_dataPoints.value("Hz1").isValid())
        m_hz1 = m_dataPoints.value("Hz1").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("W1").isValid())
        m_w1 = m_dataPoints.value("W1").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("Hz2").isValid())
        m_hz2 = m_dataPoints.value("Hz2").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("W2").isValid())
        m_w2 = m_dataPoints.value("W2").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("Hz3").isValid())
        m_hz3 = m_dataPoints.value("Hz3").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("W3").isValid())
        m_w3 = m_dataPoints.value("W3").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("Hz4").isValid())
        m_hz4 = m_dataPoints.value("Hz4").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("W4").isValid())
        m_w4 = m_dataPoints.value("W4").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("Hz5").isValid())
        m_hz5 = m_dataPoints.value("Hz5").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("W5").isValid())
        m_w5 = m_dataPoints.value("W5").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("Hz6").isValid())
        m_hz6 = m_dataPoints.value("Hz6").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("W6").isValid())
        m_w6 = m_dataPoints.value("W6").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("Hz7").isValid())
        m_hz7 = m_dataPoints.value("Hz7").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("W7").isValid())
        m_w7 = m_dataPoints.value("W7").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("Hz8").isValid())
        m_hz8 = m_dataPoints.value("Hz8").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("W8").isValid())
        m_w8 = m_dataPoints.value("W8").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("Hz9").isValid())
        m_hz9 = m_dataPoints.value("Hz9").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("W9").isValid())
        m_w9 = m_dataPoints.value("W9").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("Hz10").isValid())
        m_hz10 = m_dataPoints.value("Hz10").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("W10").isValid())
        m_w10 = m_dataPoints.value("W10").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("Hz11").isValid())
        m_hz11 = m_dataPoints.value("Hz11").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("W11").isValid())
        m_w11 = m_dataPoints.value("W11").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("Hz12").isValid())
        m_hz12 = m_dataPoints.value("Hz12").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("W12").isValid())
        m_w12 = m_dataPoints.value("W12").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("Hz13").isValid())
        m_hz13 = m_dataPoints.value("Hz13").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("W13").isValid())
        m_w13 = m_dataPoints.value("W13").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("Hz14").isValid())
        m_hz14 = m_dataPoints.value("Hz14").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("W14").isValid())
        m_w14 = m_dataPoints.value("W14").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("Hz15").isValid())
        m_hz15 = m_dataPoints.value("Hz15").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("W15").isValid())
        m_w15 = m_dataPoints.value("W15").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("Hz16").isValid())
        m_hz16 = m_dataPoints.value("Hz16").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("W16").isValid())
        m_w16 = m_dataPoints.value("W16").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("Hz17").isValid())
        m_hz17 = m_dataPoints.value("Hz17").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("W17").isValid())
        m_w17 = m_dataPoints.value("W17").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("Hz18").isValid())
        m_hz18 = m_dataPoints.value("Hz18").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("W18").isValid())
        m_w18 = m_dataPoints.value("W18").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("Hz19").isValid())
        m_hz19 = m_dataPoints.value("Hz19").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("W19").isValid())
        m_w19 = m_dataPoints.value("W19").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("Hz20").isValid())
        m_hz20 = m_dataPoints.value("Hz20").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("W20").isValid())
        m_w20 = m_dataPoints.value("W20").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("CrvNam").isValid())
        m_crvNam = m_dataPoints.value("CrvNam").toString();

    if (m_dataPoints.value("RmpPT1Tms").isValid())
        m_rmpPt1Tms = m_dataPoints.value("RmpPT1Tms").toUInt16();

    if (m_dataPoints.value("RmpDecTmm").isValid())
        m_rmpDecTmm = m_dataPoints.value("RmpDecTmm").toFloatWithSSF(m_parentModel->rmpIncDecSf());

    if (m_dataPoints.value("RmpIncTmm").isValid())
        m_rmpIncTmm = m_dataPoints.value("RmpIncTmm").toFloatWithSSF(m_parentModel->rmpIncDecSf());

    if (m_dataPoints.value("RmpRsUp").isValid())
        m_rmpRsUp = m_dataPoints.value("RmpRsUp").toFloatWithSSF(m_parentModel->rmpIncDecSf());

    if (m_dataPoints.value("SnptW").isValid())
        m_snptW = m_dataPoints.value("SnptW").toUInt16();

    if (m_dataPoints.value("WRef").isValid())
        m_wRef = m_dataPoints.value("WRef").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("WRefStrHz").isValid())
        m_wRefStrHz = m_dataPoints.value("WRefStrHz").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("WRefStopHz").isValid())
        m_wRefStopHz = m_dataPoints.value("WRefStopHz").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("ReadOnly").isValid())
        m_readOnly = static_cast<Readonly>(m_dataPoints.value("ReadOnly").toUInt16());


    qCDebug(dcSunSpecModelData()) << this;
}


SunSpecFreqWattModel::SunSpecFreqWattModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 134, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixedAndRepeating;

    initDataPoints();

    connect(this, &SunSpecModel::initFinished, this, &SunSpecFreqWattModel::setupRepeatingBlocks);
}

SunSpecFreqWattModel::~SunSpecFreqWattModel()
{

}

QString SunSpecFreqWattModel::name() const
{
    return "freq_watt";
}

QString SunSpecFreqWattModel::description() const
{
    return "Curve-Based Frequency-Watt ";
}

QString SunSpecFreqWattModel::label() const
{
    return "Freq-Watt Crv";
}

quint16 SunSpecFreqWattModel::actCrv() const
{
    return m_actCrv;
}

QModbusReply *SunSpecFreqWattModel::setActCrv(quint16 actCrv)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("ActCrv");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(actCrv);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecFreqWattModel::ModenaFlags SunSpecFreqWattModel::modEna() const
{
    return m_modEna;
}

QModbusReply *SunSpecFreqWattModel::setModEna(ModenaFlags modEna)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("ModEna");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(modEna));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecFreqWattModel::winTms() const
{
    return m_winTms;
}

QModbusReply *SunSpecFreqWattModel::setWinTms(quint16 winTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("WinTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(winTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecFreqWattModel::rvrtTms() const
{
    return m_rvrtTms;
}

QModbusReply *SunSpecFreqWattModel::setRvrtTms(quint16 rvrtTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("RvrtTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(rvrtTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecFreqWattModel::rmpTms() const
{
    return m_rmpTms;
}

QModbusReply *SunSpecFreqWattModel::setRmpTms(quint16 rmpTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("RmpTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(rmpTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecFreqWattModel::nCrv() const
{
    return m_nCrv;
}
quint16 SunSpecFreqWattModel::nPt() const
{
    return m_nPt;
}
qint16 SunSpecFreqWattModel::hzSf() const
{
    return m_hzSf;
}
qint16 SunSpecFreqWattModel::wSf() const
{
    return m_wSf;
}
qint16 SunSpecFreqWattModel::rmpIncDecSf() const
{
    return m_rmpIncDecSf;
}
void SunSpecFreqWattModel::initDataPoints()
{
    SunSpecDataPoint modelIdDataPoint;
    modelIdDataPoint.setName("ID");
    modelIdDataPoint.setLabel("Model ID");
    modelIdDataPoint.setDescription("Model identifier");
    modelIdDataPoint.setMandatory(true);
    modelIdDataPoint.setSize(1);
    modelIdDataPoint.setAddressOffset(0);
    modelIdDataPoint.setSunSpecDataType("uint16");
    modelIdDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(modelIdDataPoint.name(), modelIdDataPoint);

    SunSpecDataPoint modelLengthDataPoint;
    modelLengthDataPoint.setName("L");
    modelLengthDataPoint.setLabel("Model Length");
    modelLengthDataPoint.setDescription("Model length");
    modelLengthDataPoint.setMandatory(true);
    modelLengthDataPoint.setSize(1);
    modelLengthDataPoint.setAddressOffset(1);
    modelLengthDataPoint.setSunSpecDataType("uint16");
    modelLengthDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(modelLengthDataPoint.name(), modelLengthDataPoint);

    SunSpecDataPoint actCrvDataPoint;
    actCrvDataPoint.setName("ActCrv");
    actCrvDataPoint.setLabel("ActCrv");
    actCrvDataPoint.setDescription("Index of active curve. 0=no active curve.");
    actCrvDataPoint.setMandatory(true);
    actCrvDataPoint.setSize(1);
    actCrvDataPoint.setAddressOffset(2);
    actCrvDataPoint.setBlockOffset(0);
    actCrvDataPoint.setSunSpecDataType("uint16");
    actCrvDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    actCrvDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(actCrvDataPoint.name(), actCrvDataPoint);

    SunSpecDataPoint modEnaDataPoint;
    modEnaDataPoint.setName("ModEna");
    modEnaDataPoint.setLabel("ModEna");
    modEnaDataPoint.setDescription("Is curve-based Frequency-Watt control active.");
    modEnaDataPoint.setMandatory(true);
    modEnaDataPoint.setSize(1);
    modEnaDataPoint.setAddressOffset(3);
    modEnaDataPoint.setBlockOffset(1);
    modEnaDataPoint.setSunSpecDataType("bitfield16");
    modEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    modEnaDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(modEnaDataPoint.name(), modEnaDataPoint);

    SunSpecDataPoint winTmsDataPoint;
    winTmsDataPoint.setName("WinTms");
    winTmsDataPoint.setLabel("WinTms");
    winTmsDataPoint.setDescription("Time window for freq-watt change.");
    winTmsDataPoint.setUnits("Secs");
    winTmsDataPoint.setSize(1);
    winTmsDataPoint.setAddressOffset(4);
    winTmsDataPoint.setBlockOffset(2);
    winTmsDataPoint.setSunSpecDataType("uint16");
    winTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    winTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(winTmsDataPoint.name(), winTmsDataPoint);

    SunSpecDataPoint rvrtTmsDataPoint;
    rvrtTmsDataPoint.setName("RvrtTms");
    rvrtTmsDataPoint.setLabel("RvrtTms");
    rvrtTmsDataPoint.setDescription("Timeout period for freq-watt curve selection.");
    rvrtTmsDataPoint.setUnits("Secs");
    rvrtTmsDataPoint.setSize(1);
    rvrtTmsDataPoint.setAddressOffset(5);
    rvrtTmsDataPoint.setBlockOffset(3);
    rvrtTmsDataPoint.setSunSpecDataType("uint16");
    rvrtTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    rvrtTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(rvrtTmsDataPoint.name(), rvrtTmsDataPoint);

    SunSpecDataPoint rmpTmsDataPoint;
    rmpTmsDataPoint.setName("RmpTms");
    rmpTmsDataPoint.setLabel("RmpTms");
    rmpTmsDataPoint.setDescription("Ramp time for moving from current mode to new mode.");
    rmpTmsDataPoint.setUnits("Secs");
    rmpTmsDataPoint.setSize(1);
    rmpTmsDataPoint.setAddressOffset(6);
    rmpTmsDataPoint.setBlockOffset(4);
    rmpTmsDataPoint.setSunSpecDataType("uint16");
    rmpTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    rmpTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(rmpTmsDataPoint.name(), rmpTmsDataPoint);

    SunSpecDataPoint nCrvDataPoint;
    nCrvDataPoint.setName("NCrv");
    nCrvDataPoint.setLabel("NCrv");
    nCrvDataPoint.setDescription("Number of curves supported (recommend min. 4).");
    nCrvDataPoint.setMandatory(true);
    nCrvDataPoint.setSize(1);
    nCrvDataPoint.setAddressOffset(7);
    nCrvDataPoint.setBlockOffset(5);
    nCrvDataPoint.setSunSpecDataType("uint16");
    nCrvDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(nCrvDataPoint.name(), nCrvDataPoint);

    SunSpecDataPoint nPtDataPoint;
    nPtDataPoint.setName("NPt");
    nPtDataPoint.setLabel("NPt");
    nPtDataPoint.setDescription("Number of curve points supported (maximum of 10).");
    nPtDataPoint.setMandatory(true);
    nPtDataPoint.setSize(1);
    nPtDataPoint.setAddressOffset(8);
    nPtDataPoint.setBlockOffset(6);
    nPtDataPoint.setSunSpecDataType("uint16");
    nPtDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(nPtDataPoint.name(), nPtDataPoint);

    SunSpecDataPoint hzSfDataPoint;
    hzSfDataPoint.setName("Hz_SF");
    hzSfDataPoint.setLabel("Hz_SF");
    hzSfDataPoint.setDescription("Scale factor for frequency.");
    hzSfDataPoint.setUnits("SF");
    hzSfDataPoint.setMandatory(true);
    hzSfDataPoint.setSize(1);
    hzSfDataPoint.setAddressOffset(9);
    hzSfDataPoint.setBlockOffset(7);
    hzSfDataPoint.setSunSpecDataType("sunssf");
    hzSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hzSfDataPoint.name(), hzSfDataPoint);

    SunSpecDataPoint wSfDataPoint;
    wSfDataPoint.setName("W_SF");
    wSfDataPoint.setLabel("W_SF");
    wSfDataPoint.setDescription("Scale factor for percent WRef.");
    wSfDataPoint.setUnits("SF");
    wSfDataPoint.setMandatory(true);
    wSfDataPoint.setSize(1);
    wSfDataPoint.setAddressOffset(10);
    wSfDataPoint.setBlockOffset(8);
    wSfDataPoint.setSunSpecDataType("sunssf");
    wSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wSfDataPoint.name(), wSfDataPoint);

    SunSpecDataPoint rmpIncDecSfDataPoint;
    rmpIncDecSfDataPoint.setName("RmpIncDec_SF");
    rmpIncDecSfDataPoint.setLabel("RmpIncDec_SF");
    rmpIncDecSfDataPoint.setDescription("Scale factor for increment and decrement ramps.");
    rmpIncDecSfDataPoint.setUnits("SF");
    rmpIncDecSfDataPoint.setSize(1);
    rmpIncDecSfDataPoint.setAddressOffset(11);
    rmpIncDecSfDataPoint.setBlockOffset(9);
    rmpIncDecSfDataPoint.setSunSpecDataType("sunssf");
    rmpIncDecSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(rmpIncDecSfDataPoint.name(), rmpIncDecSfDataPoint);

}

void SunSpecFreqWattModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("Hz_SF").isValid())
        m_hzSf = m_dataPoints.value("Hz_SF").toInt16();

    if (m_dataPoints.value("W_SF").isValid())
        m_wSf = m_dataPoints.value("W_SF").toInt16();

    if (m_dataPoints.value("RmpIncDec_SF").isValid())
        m_rmpIncDecSf = m_dataPoints.value("RmpIncDec_SF").toInt16();


    // Update properties according to the data point type
    if (m_dataPoints.value("ActCrv").isValid())
        m_actCrv = m_dataPoints.value("ActCrv").toUInt16();

    if (m_dataPoints.value("ModEna").isValid())
        m_modEna = static_cast<ModenaFlags>(m_dataPoints.value("ModEna").toUInt16());

    if (m_dataPoints.value("WinTms").isValid())
        m_winTms = m_dataPoints.value("WinTms").toUInt16();

    if (m_dataPoints.value("RvrtTms").isValid())
        m_rvrtTms = m_dataPoints.value("RvrtTms").toUInt16();

    if (m_dataPoints.value("RmpTms").isValid())
        m_rmpTms = m_dataPoints.value("RmpTms").toUInt16();

    if (m_dataPoints.value("NCrv").isValid())
        m_nCrv = m_dataPoints.value("NCrv").toUInt16();

    if (m_dataPoints.value("NPt").isValid())
        m_nPt = m_dataPoints.value("NPt").toUInt16();

    if (m_dataPoints.value("Hz_SF").isValid())
        m_hzSf = m_dataPoints.value("Hz_SF").toInt16();

    if (m_dataPoints.value("W_SF").isValid())
        m_wSf = m_dataPoints.value("W_SF").toInt16();

    if (m_dataPoints.value("RmpIncDec_SF").isValid())
        m_rmpIncDecSf = m_dataPoints.value("RmpIncDec_SF").toInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

void SunSpecFreqWattModel::setupRepeatingBlocks()
{
    if (!m_repeatingBlocks.isEmpty()) {
        foreach (SunSpecModelRepeatingBlock *block, m_repeatingBlocks) {
            block->deleteLater();
        }
        m_repeatingBlocks.clear();
    }

    const auto headerLength = 2;
    const auto repeatingBlocksDataSize = m_blockData.size() - headerLength - m_fixedBlockLength;
    if (repeatingBlocksDataSize % m_repeatingBlockLength != 0) {
            qCWarning(dcSunSpecModelData()) << "Unexpected repeating block data size:"
                                            << repeatingBlocksDataSize
                                            << "(repeating block size:"
                                            << m_repeatingBlockLength
                                            << "), extra bytes:"
                                            << repeatingBlocksDataSize % m_repeatingBlockLength;
    }
    const auto numberOfBlocks = repeatingBlocksDataSize / m_repeatingBlockLength;
    const auto repeatingBlocksOffset = m_fixedBlockLength + headerLength;
    for (int i = 0; i < numberOfBlocks; ++i) {
        const auto blockStartRegister = static_cast<quint16>(modbusStartRegister() + repeatingBlocksOffset + m_repeatingBlockLength * i);
        const auto block = new SunSpecFreqWattModelRepeatingBlock(i, m_repeatingBlockLength, blockStartRegister, this);
        m_repeatingBlocks.append(block);
    }
}

QDebug operator<<(QDebug debug, SunSpecFreqWattModel *model)
{
    debug.nospace().noquote() << "SunSpecFreqWattModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("ActCrv") << "-->";
    if (model->dataPoints().value("ActCrv").isValid()) {
        debug.nospace().noquote() << model->actCrv() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModEna") << "-->";
    if (model->dataPoints().value("ModEna").isValid()) {
        debug.nospace().noquote() << model->modEna() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WinTms") << "-->";
    if (model->dataPoints().value("WinTms").isValid()) {
        debug.nospace().noquote() << model->winTms() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("RvrtTms") << "-->";
    if (model->dataPoints().value("RvrtTms").isValid()) {
        debug.nospace().noquote() << model->rvrtTms() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("RmpTms") << "-->";
    if (model->dataPoints().value("RmpTms").isValid()) {
        debug.nospace().noquote() << model->rmpTms() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("NCrv") << "-->";
    if (model->dataPoints().value("NCrv").isValid()) {
        debug.nospace().noquote() << model->nCrv() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("NPt") << "-->";
    if (model->dataPoints().value("NPt").isValid()) {
        debug.nospace().noquote() << model->nPt() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
