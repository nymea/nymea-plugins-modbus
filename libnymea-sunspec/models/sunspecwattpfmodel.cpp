/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2021, nymea GmbH
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

#include "sunspecwattpfmodel.h"
#include "sunspecconnection.h"

SunSpecWattPfModelRepeatingBlock::SunSpecWattPfModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecWattPfModel *parent) :
    SunSpecModelRepeatingBlock(blockIndex, blockSize, modbusStartRegister, parent)
{
    m_byteOrder = parent->byteOrder();
    initDataPoints();
}

QString SunSpecWattPfModelRepeatingBlock::name() const
{
    return "curve";
}

SunSpecWattPfModel *SunSpecWattPfModelRepeatingBlock::parentModel() const
{
    return m_parentModel;
}

quint16 SunSpecWattPfModelRepeatingBlock::actPt() const
{
    return m_actPt;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setActPt(quint16 actPt)
{
    SunSpecDataPoint dp = m_dataPoints.value("ActPt");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(actPt);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::w1() const
{
    return m_w1;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setW1(float w1)
{
    SunSpecDataPoint dp = m_dataPoints.value("W1");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w1, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::pf1() const
{
    return m_pf1;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setPf1(float pf1)
{
    SunSpecDataPoint dp = m_dataPoints.value("PF1");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(pf1, m_parentModel->pfSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::w2() const
{
    return m_w2;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setW2(float w2)
{
    SunSpecDataPoint dp = m_dataPoints.value("W2");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w2, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::pf2() const
{
    return m_pf2;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setPf2(float pf2)
{
    SunSpecDataPoint dp = m_dataPoints.value("PF2");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(pf2, m_parentModel->pfSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::w3() const
{
    return m_w3;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setW3(float w3)
{
    SunSpecDataPoint dp = m_dataPoints.value("W3");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w3, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::pf3() const
{
    return m_pf3;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setPf3(float pf3)
{
    SunSpecDataPoint dp = m_dataPoints.value("PF3");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(pf3, m_parentModel->pfSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::w4() const
{
    return m_w4;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setW4(float w4)
{
    SunSpecDataPoint dp = m_dataPoints.value("W4");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w4, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::pf4() const
{
    return m_pf4;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setPf4(float pf4)
{
    SunSpecDataPoint dp = m_dataPoints.value("PF4");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(pf4, m_parentModel->pfSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::w5() const
{
    return m_w5;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setW5(float w5)
{
    SunSpecDataPoint dp = m_dataPoints.value("W5");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w5, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::pf5() const
{
    return m_pf5;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setPf5(float pf5)
{
    SunSpecDataPoint dp = m_dataPoints.value("PF5");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(pf5, m_parentModel->pfSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::w6() const
{
    return m_w6;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setW6(float w6)
{
    SunSpecDataPoint dp = m_dataPoints.value("W6");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w6, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::pf6() const
{
    return m_pf6;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setPf6(float pf6)
{
    SunSpecDataPoint dp = m_dataPoints.value("PF6");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(pf6, m_parentModel->pfSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::w7() const
{
    return m_w7;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setW7(float w7)
{
    SunSpecDataPoint dp = m_dataPoints.value("W7");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w7, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::pf7() const
{
    return m_pf7;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setPf7(float pf7)
{
    SunSpecDataPoint dp = m_dataPoints.value("PF7");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(pf7, m_parentModel->pfSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::w8() const
{
    return m_w8;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setW8(float w8)
{
    SunSpecDataPoint dp = m_dataPoints.value("W8");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w8, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::pf8() const
{
    return m_pf8;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setPf8(float pf8)
{
    SunSpecDataPoint dp = m_dataPoints.value("PF8");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(pf8, m_parentModel->pfSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::w9() const
{
    return m_w9;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setW9(float w9)
{
    SunSpecDataPoint dp = m_dataPoints.value("W9");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w9, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::pf9() const
{
    return m_pf9;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setPf9(float pf9)
{
    SunSpecDataPoint dp = m_dataPoints.value("PF9");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(pf9, m_parentModel->pfSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::w10() const
{
    return m_w10;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setW10(float w10)
{
    SunSpecDataPoint dp = m_dataPoints.value("W10");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w10, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::pf10() const
{
    return m_pf10;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setPf10(float pf10)
{
    SunSpecDataPoint dp = m_dataPoints.value("PF10");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(pf10, m_parentModel->pfSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::w11() const
{
    return m_w11;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setW11(float w11)
{
    SunSpecDataPoint dp = m_dataPoints.value("W11");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w11, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::pf11() const
{
    return m_pf11;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setPf11(float pf11)
{
    SunSpecDataPoint dp = m_dataPoints.value("PF11");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(pf11, m_parentModel->pfSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::w12() const
{
    return m_w12;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setW12(float w12)
{
    SunSpecDataPoint dp = m_dataPoints.value("W12");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w12, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::pf12() const
{
    return m_pf12;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setPf12(float pf12)
{
    SunSpecDataPoint dp = m_dataPoints.value("PF12");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(pf12, m_parentModel->pfSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::w13() const
{
    return m_w13;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setW13(float w13)
{
    SunSpecDataPoint dp = m_dataPoints.value("W13");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w13, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::pf13() const
{
    return m_pf13;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setPf13(float pf13)
{
    SunSpecDataPoint dp = m_dataPoints.value("PF13");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(pf13, m_parentModel->pfSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::w14() const
{
    return m_w14;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setW14(float w14)
{
    SunSpecDataPoint dp = m_dataPoints.value("W14");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w14, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::pf14() const
{
    return m_pf14;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setPf14(float pf14)
{
    SunSpecDataPoint dp = m_dataPoints.value("PF14");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(pf14, m_parentModel->pfSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::w15() const
{
    return m_w15;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setW15(float w15)
{
    SunSpecDataPoint dp = m_dataPoints.value("W15");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w15, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::pf15() const
{
    return m_pf15;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setPf15(float pf15)
{
    SunSpecDataPoint dp = m_dataPoints.value("PF15");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(pf15, m_parentModel->pfSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::w16() const
{
    return m_w16;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setW16(float w16)
{
    SunSpecDataPoint dp = m_dataPoints.value("W16");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w16, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::pf16() const
{
    return m_pf16;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setPf16(float pf16)
{
    SunSpecDataPoint dp = m_dataPoints.value("PF16");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(pf16, m_parentModel->pfSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::w17() const
{
    return m_w17;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setW17(float w17)
{
    SunSpecDataPoint dp = m_dataPoints.value("W17");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w17, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::pf17() const
{
    return m_pf17;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setPf17(float pf17)
{
    SunSpecDataPoint dp = m_dataPoints.value("PF17");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(pf17, m_parentModel->pfSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::w18() const
{
    return m_w18;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setW18(float w18)
{
    SunSpecDataPoint dp = m_dataPoints.value("W18");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w18, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::pf18() const
{
    return m_pf18;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setPf18(float pf18)
{
    SunSpecDataPoint dp = m_dataPoints.value("PF18");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(pf18, m_parentModel->pfSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::w19() const
{
    return m_w19;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setW19(float w19)
{
    SunSpecDataPoint dp = m_dataPoints.value("W19");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w19, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::pf19() const
{
    return m_pf19;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setPf19(float pf19)
{
    SunSpecDataPoint dp = m_dataPoints.value("PF19");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(pf19, m_parentModel->pfSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::w20() const
{
    return m_w20;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setW20(float w20)
{
    SunSpecDataPoint dp = m_dataPoints.value("W20");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w20, m_parentModel->wSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::pf20() const
{
    return m_pf20;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setPf20(float pf20)
{
    SunSpecDataPoint dp = m_dataPoints.value("PF20");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(pf20, m_parentModel->pfSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

QString SunSpecWattPfModelRepeatingBlock::crvNam() const
{
    return m_crvNam;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setCrvNam(const QString &crvNam)
{
    SunSpecDataPoint dp = m_dataPoints.value("CrvNam");
    QVector<quint16> registers = SunSpecDataPoint::convertFromString(crvNam, dp.size());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

quint16 SunSpecWattPfModelRepeatingBlock::rmpPt1Tms() const
{
    return m_rmpPt1Tms;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setRmpPt1Tms(quint16 rmpPt1Tms)
{
    SunSpecDataPoint dp = m_dataPoints.value("RmpPT1Tms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(rmpPt1Tms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::rmpDecTmm() const
{
    return m_rmpDecTmm;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setRmpDecTmm(float rmpDecTmm)
{
    SunSpecDataPoint dp = m_dataPoints.value("RmpDecTmm");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(rmpDecTmm, m_parentModel->rmpIncDecSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecWattPfModelRepeatingBlock::rmpIncTmm() const
{
    return m_rmpIncTmm;
}

QModbusReply *SunSpecWattPfModelRepeatingBlock::setRmpIncTmm(float rmpIncTmm)
{
    SunSpecDataPoint dp = m_dataPoints.value("RmpIncTmm");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(rmpIncTmm, m_parentModel->rmpIncDecSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

SunSpecWattPfModelRepeatingBlock::Readonly SunSpecWattPfModelRepeatingBlock::readOnly() const
{
    return m_readOnly;
}
quint16 SunSpecWattPfModelRepeatingBlock::pad() const
{
    return m_pad;
}

void SunSpecWattPfModelRepeatingBlock::initDataPoints()
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

    SunSpecDataPoint w1DataPoint;
    w1DataPoint.setName("W1");
    w1DataPoint.setLabel("W1");
    w1DataPoint.setDescription("Point 1 Watts.");
    w1DataPoint.setUnits("% WMax");
    w1DataPoint.setMandatory(true);
    w1DataPoint.setSize(1);
    w1DataPoint.setAddressOffset(1);
    w1DataPoint.setScaleFactorName("W_SF");
    w1DataPoint.setSunSpecDataType("int16");
    w1DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w1DataPoint.name(), w1DataPoint);

    SunSpecDataPoint pf1DataPoint;
    pf1DataPoint.setName("PF1");
    pf1DataPoint.setLabel("PF1");
    pf1DataPoint.setDescription("Point 1 PF in EEI notation.");
    pf1DataPoint.setUnits("cos()");
    pf1DataPoint.setMandatory(true);
    pf1DataPoint.setSize(1);
    pf1DataPoint.setAddressOffset(2);
    pf1DataPoint.setBlockOffset(0);
    pf1DataPoint.setScaleFactorName("PF_SF");
    pf1DataPoint.setSunSpecDataType("int16");
    pf1DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    pf1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pf1DataPoint.name(), pf1DataPoint);

    SunSpecDataPoint w2DataPoint;
    w2DataPoint.setName("W2");
    w2DataPoint.setLabel("W2");
    w2DataPoint.setDescription("Point 2 Watts.");
    w2DataPoint.setUnits("% WMax");
    w2DataPoint.setSize(1);
    w2DataPoint.setAddressOffset(3);
    w2DataPoint.setBlockOffset(1);
    w2DataPoint.setScaleFactorName("W_SF");
    w2DataPoint.setSunSpecDataType("int16");
    w2DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w2DataPoint.name(), w2DataPoint);

    SunSpecDataPoint pf2DataPoint;
    pf2DataPoint.setName("PF2");
    pf2DataPoint.setLabel("PF2");
    pf2DataPoint.setDescription("Point 2 PF in EEI notation.");
    pf2DataPoint.setUnits("cos()");
    pf2DataPoint.setSize(1);
    pf2DataPoint.setAddressOffset(4);
    pf2DataPoint.setBlockOffset(2);
    pf2DataPoint.setScaleFactorName("PF_SF");
    pf2DataPoint.setSunSpecDataType("int16");
    pf2DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    pf2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pf2DataPoint.name(), pf2DataPoint);

    SunSpecDataPoint w3DataPoint;
    w3DataPoint.setName("W3");
    w3DataPoint.setLabel("W3");
    w3DataPoint.setDescription("Point 3 Watts.");
    w3DataPoint.setUnits("% WMax");
    w3DataPoint.setSize(1);
    w3DataPoint.setAddressOffset(5);
    w3DataPoint.setBlockOffset(3);
    w3DataPoint.setScaleFactorName("W_SF");
    w3DataPoint.setSunSpecDataType("int16");
    w3DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w3DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w3DataPoint.name(), w3DataPoint);

    SunSpecDataPoint pf3DataPoint;
    pf3DataPoint.setName("PF3");
    pf3DataPoint.setLabel("PF3");
    pf3DataPoint.setDescription("Point 3 PF in EEI notation.");
    pf3DataPoint.setUnits("cos()");
    pf3DataPoint.setSize(1);
    pf3DataPoint.setAddressOffset(6);
    pf3DataPoint.setBlockOffset(4);
    pf3DataPoint.setScaleFactorName("PF_SF");
    pf3DataPoint.setSunSpecDataType("int16");
    pf3DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    pf3DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pf3DataPoint.name(), pf3DataPoint);

    SunSpecDataPoint w4DataPoint;
    w4DataPoint.setName("W4");
    w4DataPoint.setLabel("W4");
    w4DataPoint.setDescription("Point 4 Watts.");
    w4DataPoint.setUnits("% WMax");
    w4DataPoint.setSize(1);
    w4DataPoint.setAddressOffset(7);
    w4DataPoint.setBlockOffset(5);
    w4DataPoint.setScaleFactorName("W_SF");
    w4DataPoint.setSunSpecDataType("int16");
    w4DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w4DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w4DataPoint.name(), w4DataPoint);

    SunSpecDataPoint pf4DataPoint;
    pf4DataPoint.setName("PF4");
    pf4DataPoint.setLabel("PF4");
    pf4DataPoint.setDescription("Point 4 PF in EEI notation.");
    pf4DataPoint.setUnits("cos()");
    pf4DataPoint.setSize(1);
    pf4DataPoint.setAddressOffset(8);
    pf4DataPoint.setBlockOffset(6);
    pf4DataPoint.setScaleFactorName("PF_SF");
    pf4DataPoint.setSunSpecDataType("int16");
    pf4DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    pf4DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pf4DataPoint.name(), pf4DataPoint);

    SunSpecDataPoint w5DataPoint;
    w5DataPoint.setName("W5");
    w5DataPoint.setLabel("W5");
    w5DataPoint.setDescription("Point 5 Watts.");
    w5DataPoint.setUnits("% WMax");
    w5DataPoint.setSize(1);
    w5DataPoint.setAddressOffset(9);
    w5DataPoint.setBlockOffset(7);
    w5DataPoint.setScaleFactorName("W_SF");
    w5DataPoint.setSunSpecDataType("int16");
    w5DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w5DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w5DataPoint.name(), w5DataPoint);

    SunSpecDataPoint pf5DataPoint;
    pf5DataPoint.setName("PF5");
    pf5DataPoint.setLabel("PF5");
    pf5DataPoint.setDescription("Point 5 PF in EEI notation.");
    pf5DataPoint.setUnits("cos()");
    pf5DataPoint.setSize(1);
    pf5DataPoint.setAddressOffset(10);
    pf5DataPoint.setBlockOffset(8);
    pf5DataPoint.setScaleFactorName("PF_SF");
    pf5DataPoint.setSunSpecDataType("int16");
    pf5DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    pf5DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pf5DataPoint.name(), pf5DataPoint);

    SunSpecDataPoint w6DataPoint;
    w6DataPoint.setName("W6");
    w6DataPoint.setLabel("W6");
    w6DataPoint.setDescription("Point 6 Watts.");
    w6DataPoint.setUnits("% WMax");
    w6DataPoint.setSize(1);
    w6DataPoint.setAddressOffset(11);
    w6DataPoint.setBlockOffset(9);
    w6DataPoint.setScaleFactorName("W_SF");
    w6DataPoint.setSunSpecDataType("int16");
    w6DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w6DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w6DataPoint.name(), w6DataPoint);

    SunSpecDataPoint pf6DataPoint;
    pf6DataPoint.setName("PF6");
    pf6DataPoint.setLabel("PF6");
    pf6DataPoint.setDescription("Point 6 PF in EEI notation.");
    pf6DataPoint.setUnits("cos()");
    pf6DataPoint.setSize(1);
    pf6DataPoint.setAddressOffset(12);
    pf6DataPoint.setBlockOffset(10);
    pf6DataPoint.setScaleFactorName("PF_SF");
    pf6DataPoint.setSunSpecDataType("int16");
    pf6DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    pf6DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pf6DataPoint.name(), pf6DataPoint);

    SunSpecDataPoint w7DataPoint;
    w7DataPoint.setName("W7");
    w7DataPoint.setLabel("W7");
    w7DataPoint.setDescription("Point 7 Watts.");
    w7DataPoint.setUnits("% WMax");
    w7DataPoint.setSize(1);
    w7DataPoint.setAddressOffset(13);
    w7DataPoint.setBlockOffset(11);
    w7DataPoint.setScaleFactorName("W_SF");
    w7DataPoint.setSunSpecDataType("int16");
    w7DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w7DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w7DataPoint.name(), w7DataPoint);

    SunSpecDataPoint pf7DataPoint;
    pf7DataPoint.setName("PF7");
    pf7DataPoint.setLabel("PF7");
    pf7DataPoint.setDescription("Point 7 PF in EEI notation.");
    pf7DataPoint.setUnits("cos()");
    pf7DataPoint.setSize(1);
    pf7DataPoint.setAddressOffset(14);
    pf7DataPoint.setBlockOffset(12);
    pf7DataPoint.setScaleFactorName("PF_SF");
    pf7DataPoint.setSunSpecDataType("int16");
    pf7DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    pf7DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pf7DataPoint.name(), pf7DataPoint);

    SunSpecDataPoint w8DataPoint;
    w8DataPoint.setName("W8");
    w8DataPoint.setLabel("W8");
    w8DataPoint.setDescription("Point 8 Watts.");
    w8DataPoint.setUnits("% WMax");
    w8DataPoint.setSize(1);
    w8DataPoint.setAddressOffset(15);
    w8DataPoint.setBlockOffset(13);
    w8DataPoint.setScaleFactorName("W_SF");
    w8DataPoint.setSunSpecDataType("int16");
    w8DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w8DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w8DataPoint.name(), w8DataPoint);

    SunSpecDataPoint pf8DataPoint;
    pf8DataPoint.setName("PF8");
    pf8DataPoint.setLabel("PF8");
    pf8DataPoint.setDescription("Point 8 PF in EEI notation.");
    pf8DataPoint.setUnits("cos()");
    pf8DataPoint.setSize(1);
    pf8DataPoint.setAddressOffset(16);
    pf8DataPoint.setBlockOffset(14);
    pf8DataPoint.setScaleFactorName("PF_SF");
    pf8DataPoint.setSunSpecDataType("int16");
    pf8DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    pf8DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pf8DataPoint.name(), pf8DataPoint);

    SunSpecDataPoint w9DataPoint;
    w9DataPoint.setName("W9");
    w9DataPoint.setLabel("W9");
    w9DataPoint.setDescription("Point 9 Watts.");
    w9DataPoint.setUnits("% WMax");
    w9DataPoint.setSize(1);
    w9DataPoint.setAddressOffset(17);
    w9DataPoint.setBlockOffset(15);
    w9DataPoint.setScaleFactorName("W_SF");
    w9DataPoint.setSunSpecDataType("int16");
    w9DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w9DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w9DataPoint.name(), w9DataPoint);

    SunSpecDataPoint pf9DataPoint;
    pf9DataPoint.setName("PF9");
    pf9DataPoint.setLabel("PF9");
    pf9DataPoint.setDescription("Point 9 PF in EEI notation.");
    pf9DataPoint.setUnits("cos()");
    pf9DataPoint.setSize(1);
    pf9DataPoint.setAddressOffset(18);
    pf9DataPoint.setBlockOffset(16);
    pf9DataPoint.setScaleFactorName("PF_SF");
    pf9DataPoint.setSunSpecDataType("int16");
    pf9DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    pf9DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pf9DataPoint.name(), pf9DataPoint);

    SunSpecDataPoint w10DataPoint;
    w10DataPoint.setName("W10");
    w10DataPoint.setLabel("W10");
    w10DataPoint.setDescription("Point 10 Watts.");
    w10DataPoint.setUnits("% WMax");
    w10DataPoint.setSize(1);
    w10DataPoint.setAddressOffset(19);
    w10DataPoint.setBlockOffset(17);
    w10DataPoint.setScaleFactorName("W_SF");
    w10DataPoint.setSunSpecDataType("int16");
    w10DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w10DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w10DataPoint.name(), w10DataPoint);

    SunSpecDataPoint pf10DataPoint;
    pf10DataPoint.setName("PF10");
    pf10DataPoint.setLabel("PF10");
    pf10DataPoint.setDescription("Point 10 PF in EEI notation.");
    pf10DataPoint.setUnits("cos()");
    pf10DataPoint.setSize(1);
    pf10DataPoint.setAddressOffset(20);
    pf10DataPoint.setBlockOffset(18);
    pf10DataPoint.setScaleFactorName("PF_SF");
    pf10DataPoint.setSunSpecDataType("int16");
    pf10DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    pf10DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pf10DataPoint.name(), pf10DataPoint);

    SunSpecDataPoint w11DataPoint;
    w11DataPoint.setName("W11");
    w11DataPoint.setLabel("W11");
    w11DataPoint.setDescription("Point 11 Watts.");
    w11DataPoint.setUnits("% WMax");
    w11DataPoint.setSize(1);
    w11DataPoint.setAddressOffset(21);
    w11DataPoint.setBlockOffset(19);
    w11DataPoint.setScaleFactorName("W_SF");
    w11DataPoint.setSunSpecDataType("int16");
    w11DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w11DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w11DataPoint.name(), w11DataPoint);

    SunSpecDataPoint pf11DataPoint;
    pf11DataPoint.setName("PF11");
    pf11DataPoint.setLabel("PF11");
    pf11DataPoint.setDescription("Point 11 PF in EEI notation.");
    pf11DataPoint.setUnits("cos()");
    pf11DataPoint.setSize(1);
    pf11DataPoint.setAddressOffset(22);
    pf11DataPoint.setBlockOffset(20);
    pf11DataPoint.setScaleFactorName("PF_SF");
    pf11DataPoint.setSunSpecDataType("int16");
    pf11DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    pf11DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pf11DataPoint.name(), pf11DataPoint);

    SunSpecDataPoint w12DataPoint;
    w12DataPoint.setName("W12");
    w12DataPoint.setLabel("W12");
    w12DataPoint.setDescription("Point 12 Watts.");
    w12DataPoint.setUnits("% WMax");
    w12DataPoint.setSize(1);
    w12DataPoint.setAddressOffset(23);
    w12DataPoint.setBlockOffset(21);
    w12DataPoint.setScaleFactorName("W_SF");
    w12DataPoint.setSunSpecDataType("int16");
    w12DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w12DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w12DataPoint.name(), w12DataPoint);

    SunSpecDataPoint pf12DataPoint;
    pf12DataPoint.setName("PF12");
    pf12DataPoint.setLabel("PF12");
    pf12DataPoint.setDescription("Point 12 PF in EEI notation.");
    pf12DataPoint.setUnits("cos()");
    pf12DataPoint.setSize(1);
    pf12DataPoint.setAddressOffset(24);
    pf12DataPoint.setBlockOffset(22);
    pf12DataPoint.setScaleFactorName("PF_SF");
    pf12DataPoint.setSunSpecDataType("int16");
    pf12DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    pf12DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pf12DataPoint.name(), pf12DataPoint);

    SunSpecDataPoint w13DataPoint;
    w13DataPoint.setName("W13");
    w13DataPoint.setLabel("W13");
    w13DataPoint.setDescription("Point 13 Watts.");
    w13DataPoint.setUnits("% WMax");
    w13DataPoint.setSize(1);
    w13DataPoint.setAddressOffset(25);
    w13DataPoint.setBlockOffset(23);
    w13DataPoint.setScaleFactorName("W_SF");
    w13DataPoint.setSunSpecDataType("int16");
    w13DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w13DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w13DataPoint.name(), w13DataPoint);

    SunSpecDataPoint pf13DataPoint;
    pf13DataPoint.setName("PF13");
    pf13DataPoint.setLabel("PF13");
    pf13DataPoint.setDescription("Point 13 PF in EEI notation.");
    pf13DataPoint.setUnits("cos()");
    pf13DataPoint.setSize(1);
    pf13DataPoint.setAddressOffset(26);
    pf13DataPoint.setBlockOffset(24);
    pf13DataPoint.setScaleFactorName("PF_SF");
    pf13DataPoint.setSunSpecDataType("int16");
    pf13DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    pf13DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pf13DataPoint.name(), pf13DataPoint);

    SunSpecDataPoint w14DataPoint;
    w14DataPoint.setName("W14");
    w14DataPoint.setLabel("W14");
    w14DataPoint.setDescription("Point 14 Watts.");
    w14DataPoint.setUnits("% WMax");
    w14DataPoint.setSize(1);
    w14DataPoint.setAddressOffset(27);
    w14DataPoint.setBlockOffset(25);
    w14DataPoint.setScaleFactorName("W_SF");
    w14DataPoint.setSunSpecDataType("int16");
    w14DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w14DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w14DataPoint.name(), w14DataPoint);

    SunSpecDataPoint pf14DataPoint;
    pf14DataPoint.setName("PF14");
    pf14DataPoint.setLabel("PF14");
    pf14DataPoint.setDescription("Point 14 PF in EEI notation.");
    pf14DataPoint.setUnits("cos()");
    pf14DataPoint.setSize(1);
    pf14DataPoint.setAddressOffset(28);
    pf14DataPoint.setBlockOffset(26);
    pf14DataPoint.setScaleFactorName("PF_SF");
    pf14DataPoint.setSunSpecDataType("int16");
    pf14DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    pf14DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pf14DataPoint.name(), pf14DataPoint);

    SunSpecDataPoint w15DataPoint;
    w15DataPoint.setName("W15");
    w15DataPoint.setLabel("W15");
    w15DataPoint.setDescription("Point 15 Watts.");
    w15DataPoint.setUnits("% WMax");
    w15DataPoint.setSize(1);
    w15DataPoint.setAddressOffset(29);
    w15DataPoint.setBlockOffset(27);
    w15DataPoint.setScaleFactorName("W_SF");
    w15DataPoint.setSunSpecDataType("int16");
    w15DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w15DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w15DataPoint.name(), w15DataPoint);

    SunSpecDataPoint pf15DataPoint;
    pf15DataPoint.setName("PF15");
    pf15DataPoint.setLabel("PF15");
    pf15DataPoint.setDescription("Point 15 PF in EEI notation.");
    pf15DataPoint.setUnits("cos()");
    pf15DataPoint.setSize(1);
    pf15DataPoint.setAddressOffset(30);
    pf15DataPoint.setBlockOffset(28);
    pf15DataPoint.setScaleFactorName("PF_SF");
    pf15DataPoint.setSunSpecDataType("int16");
    pf15DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    pf15DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pf15DataPoint.name(), pf15DataPoint);

    SunSpecDataPoint w16DataPoint;
    w16DataPoint.setName("W16");
    w16DataPoint.setLabel("W16");
    w16DataPoint.setDescription("Point 16 Watts.");
    w16DataPoint.setUnits("% WMax");
    w16DataPoint.setSize(1);
    w16DataPoint.setAddressOffset(31);
    w16DataPoint.setBlockOffset(29);
    w16DataPoint.setScaleFactorName("W_SF");
    w16DataPoint.setSunSpecDataType("int16");
    w16DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w16DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w16DataPoint.name(), w16DataPoint);

    SunSpecDataPoint pf16DataPoint;
    pf16DataPoint.setName("PF16");
    pf16DataPoint.setLabel("PF16");
    pf16DataPoint.setDescription("Point 16 PF in EEI notation.");
    pf16DataPoint.setUnits("cos()");
    pf16DataPoint.setSize(1);
    pf16DataPoint.setAddressOffset(32);
    pf16DataPoint.setBlockOffset(30);
    pf16DataPoint.setScaleFactorName("PF_SF");
    pf16DataPoint.setSunSpecDataType("int16");
    pf16DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    pf16DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pf16DataPoint.name(), pf16DataPoint);

    SunSpecDataPoint w17DataPoint;
    w17DataPoint.setName("W17");
    w17DataPoint.setLabel("W17");
    w17DataPoint.setDescription("Point 17 Watts.");
    w17DataPoint.setUnits("% WMax");
    w17DataPoint.setSize(1);
    w17DataPoint.setAddressOffset(33);
    w17DataPoint.setBlockOffset(31);
    w17DataPoint.setScaleFactorName("W_SF");
    w17DataPoint.setSunSpecDataType("int16");
    w17DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w17DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w17DataPoint.name(), w17DataPoint);

    SunSpecDataPoint pf17DataPoint;
    pf17DataPoint.setName("PF17");
    pf17DataPoint.setLabel("PF17");
    pf17DataPoint.setDescription("Point 17 PF in EEI notation.");
    pf17DataPoint.setUnits("cos()");
    pf17DataPoint.setSize(1);
    pf17DataPoint.setAddressOffset(34);
    pf17DataPoint.setBlockOffset(32);
    pf17DataPoint.setScaleFactorName("PF_SF");
    pf17DataPoint.setSunSpecDataType("int16");
    pf17DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    pf17DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pf17DataPoint.name(), pf17DataPoint);

    SunSpecDataPoint w18DataPoint;
    w18DataPoint.setName("W18");
    w18DataPoint.setLabel("W18");
    w18DataPoint.setDescription("Point 18 Watts.");
    w18DataPoint.setUnits("% WMax");
    w18DataPoint.setSize(1);
    w18DataPoint.setAddressOffset(35);
    w18DataPoint.setBlockOffset(33);
    w18DataPoint.setScaleFactorName("W_SF");
    w18DataPoint.setSunSpecDataType("int16");
    w18DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w18DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w18DataPoint.name(), w18DataPoint);

    SunSpecDataPoint pf18DataPoint;
    pf18DataPoint.setName("PF18");
    pf18DataPoint.setLabel("PF18");
    pf18DataPoint.setDescription("Point 18 PF in EEI notation.");
    pf18DataPoint.setUnits("cos()");
    pf18DataPoint.setSize(1);
    pf18DataPoint.setAddressOffset(36);
    pf18DataPoint.setBlockOffset(34);
    pf18DataPoint.setScaleFactorName("PF_SF");
    pf18DataPoint.setSunSpecDataType("int16");
    pf18DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    pf18DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pf18DataPoint.name(), pf18DataPoint);

    SunSpecDataPoint w19DataPoint;
    w19DataPoint.setName("W19");
    w19DataPoint.setLabel("W19");
    w19DataPoint.setDescription("Point 19 Watts.");
    w19DataPoint.setUnits("% WMax");
    w19DataPoint.setSize(1);
    w19DataPoint.setAddressOffset(37);
    w19DataPoint.setBlockOffset(35);
    w19DataPoint.setScaleFactorName("W_SF");
    w19DataPoint.setSunSpecDataType("int16");
    w19DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w19DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w19DataPoint.name(), w19DataPoint);

    SunSpecDataPoint pf19DataPoint;
    pf19DataPoint.setName("PF19");
    pf19DataPoint.setLabel("PF19");
    pf19DataPoint.setDescription("Point 19 PF in EEI notation.");
    pf19DataPoint.setUnits("cos()");
    pf19DataPoint.setSize(1);
    pf19DataPoint.setAddressOffset(38);
    pf19DataPoint.setBlockOffset(36);
    pf19DataPoint.setScaleFactorName("PF_SF");
    pf19DataPoint.setSunSpecDataType("int16");
    pf19DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    pf19DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pf19DataPoint.name(), pf19DataPoint);

    SunSpecDataPoint w20DataPoint;
    w20DataPoint.setName("W20");
    w20DataPoint.setLabel("W20");
    w20DataPoint.setDescription("Point 20 Watts.");
    w20DataPoint.setUnits("% WMax");
    w20DataPoint.setSize(1);
    w20DataPoint.setAddressOffset(39);
    w20DataPoint.setBlockOffset(37);
    w20DataPoint.setScaleFactorName("W_SF");
    w20DataPoint.setSunSpecDataType("int16");
    w20DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w20DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w20DataPoint.name(), w20DataPoint);

    SunSpecDataPoint pf20DataPoint;
    pf20DataPoint.setName("PF20");
    pf20DataPoint.setLabel("PF20");
    pf20DataPoint.setDescription("Point 20 PF in EEI notation.");
    pf20DataPoint.setUnits("cos()");
    pf20DataPoint.setSize(1);
    pf20DataPoint.setAddressOffset(40);
    pf20DataPoint.setBlockOffset(38);
    pf20DataPoint.setScaleFactorName("PF_SF");
    pf20DataPoint.setSunSpecDataType("int16");
    pf20DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    pf20DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pf20DataPoint.name(), pf20DataPoint);

    SunSpecDataPoint crvNamDataPoint;
    crvNamDataPoint.setName("CrvNam");
    crvNamDataPoint.setLabel("CrvNam");
    crvNamDataPoint.setDescription("Optional description for curve.");
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
    rmpDecTmmDataPoint.setDescription("The maximum rate at which the power factor may be reduced in response to changes in the power value.");
    rmpDecTmmDataPoint.setUnits("% PF/min");
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
    rmpIncTmmDataPoint.setDescription("The maximum rate at which the power factor may be increased in response to changes in the power value.");
    rmpIncTmmDataPoint.setUnits("% PF/min");
    rmpIncTmmDataPoint.setSize(1);
    rmpIncTmmDataPoint.setAddressOffset(51);
    rmpIncTmmDataPoint.setBlockOffset(49);
    rmpIncTmmDataPoint.setScaleFactorName("RmpIncDec_SF");
    rmpIncTmmDataPoint.setSunSpecDataType("uint16");
    rmpIncTmmDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    rmpIncTmmDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(rmpIncTmmDataPoint.name(), rmpIncTmmDataPoint);

    SunSpecDataPoint readOnlyDataPoint;
    readOnlyDataPoint.setName("ReadOnly");
    readOnlyDataPoint.setLabel("ReadOnly");
    readOnlyDataPoint.setDescription("Enumerated value indicates if curve is read-only or can be modified.");
    readOnlyDataPoint.setMandatory(true);
    readOnlyDataPoint.setSize(1);
    readOnlyDataPoint.setAddressOffset(52);
    readOnlyDataPoint.setBlockOffset(50);
    readOnlyDataPoint.setSunSpecDataType("enum16");
    readOnlyDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(readOnlyDataPoint.name(), readOnlyDataPoint);

    SunSpecDataPoint padDataPoint;
    padDataPoint.setName("Pad");
    padDataPoint.setSize(1);
    padDataPoint.setAddressOffset(53);
    padDataPoint.setBlockOffset(51);
    padDataPoint.setSunSpecDataType("pad");
    padDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(padDataPoint.name(), padDataPoint);

}

void SunSpecWattPfModelRepeatingBlock::processBlockData(const QVector<quint16> blockData)
{
    m_blockData = blockData;

    // Update properties according to the data point type
    if (m_dataPoints.value("ActPt").isValid())
        m_actPt = m_dataPoints.value("ActPt").toUInt16();

    if (m_dataPoints.value("W1").isValid())
        m_w1 = m_dataPoints.value("W1").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("PF1").isValid())
        m_pf1 = m_dataPoints.value("PF1").toFloatWithSSF(m_parentModel->pfSf());

    if (m_dataPoints.value("W2").isValid())
        m_w2 = m_dataPoints.value("W2").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("PF2").isValid())
        m_pf2 = m_dataPoints.value("PF2").toFloatWithSSF(m_parentModel->pfSf());

    if (m_dataPoints.value("W3").isValid())
        m_w3 = m_dataPoints.value("W3").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("PF3").isValid())
        m_pf3 = m_dataPoints.value("PF3").toFloatWithSSF(m_parentModel->pfSf());

    if (m_dataPoints.value("W4").isValid())
        m_w4 = m_dataPoints.value("W4").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("PF4").isValid())
        m_pf4 = m_dataPoints.value("PF4").toFloatWithSSF(m_parentModel->pfSf());

    if (m_dataPoints.value("W5").isValid())
        m_w5 = m_dataPoints.value("W5").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("PF5").isValid())
        m_pf5 = m_dataPoints.value("PF5").toFloatWithSSF(m_parentModel->pfSf());

    if (m_dataPoints.value("W6").isValid())
        m_w6 = m_dataPoints.value("W6").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("PF6").isValid())
        m_pf6 = m_dataPoints.value("PF6").toFloatWithSSF(m_parentModel->pfSf());

    if (m_dataPoints.value("W7").isValid())
        m_w7 = m_dataPoints.value("W7").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("PF7").isValid())
        m_pf7 = m_dataPoints.value("PF7").toFloatWithSSF(m_parentModel->pfSf());

    if (m_dataPoints.value("W8").isValid())
        m_w8 = m_dataPoints.value("W8").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("PF8").isValid())
        m_pf8 = m_dataPoints.value("PF8").toFloatWithSSF(m_parentModel->pfSf());

    if (m_dataPoints.value("W9").isValid())
        m_w9 = m_dataPoints.value("W9").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("PF9").isValid())
        m_pf9 = m_dataPoints.value("PF9").toFloatWithSSF(m_parentModel->pfSf());

    if (m_dataPoints.value("W10").isValid())
        m_w10 = m_dataPoints.value("W10").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("PF10").isValid())
        m_pf10 = m_dataPoints.value("PF10").toFloatWithSSF(m_parentModel->pfSf());

    if (m_dataPoints.value("W11").isValid())
        m_w11 = m_dataPoints.value("W11").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("PF11").isValid())
        m_pf11 = m_dataPoints.value("PF11").toFloatWithSSF(m_parentModel->pfSf());

    if (m_dataPoints.value("W12").isValid())
        m_w12 = m_dataPoints.value("W12").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("PF12").isValid())
        m_pf12 = m_dataPoints.value("PF12").toFloatWithSSF(m_parentModel->pfSf());

    if (m_dataPoints.value("W13").isValid())
        m_w13 = m_dataPoints.value("W13").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("PF13").isValid())
        m_pf13 = m_dataPoints.value("PF13").toFloatWithSSF(m_parentModel->pfSf());

    if (m_dataPoints.value("W14").isValid())
        m_w14 = m_dataPoints.value("W14").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("PF14").isValid())
        m_pf14 = m_dataPoints.value("PF14").toFloatWithSSF(m_parentModel->pfSf());

    if (m_dataPoints.value("W15").isValid())
        m_w15 = m_dataPoints.value("W15").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("PF15").isValid())
        m_pf15 = m_dataPoints.value("PF15").toFloatWithSSF(m_parentModel->pfSf());

    if (m_dataPoints.value("W16").isValid())
        m_w16 = m_dataPoints.value("W16").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("PF16").isValid())
        m_pf16 = m_dataPoints.value("PF16").toFloatWithSSF(m_parentModel->pfSf());

    if (m_dataPoints.value("W17").isValid())
        m_w17 = m_dataPoints.value("W17").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("PF17").isValid())
        m_pf17 = m_dataPoints.value("PF17").toFloatWithSSF(m_parentModel->pfSf());

    if (m_dataPoints.value("W18").isValid())
        m_w18 = m_dataPoints.value("W18").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("PF18").isValid())
        m_pf18 = m_dataPoints.value("PF18").toFloatWithSSF(m_parentModel->pfSf());

    if (m_dataPoints.value("W19").isValid())
        m_w19 = m_dataPoints.value("W19").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("PF19").isValid())
        m_pf19 = m_dataPoints.value("PF19").toFloatWithSSF(m_parentModel->pfSf());

    if (m_dataPoints.value("W20").isValid())
        m_w20 = m_dataPoints.value("W20").toFloatWithSSF(m_parentModel->wSf());

    if (m_dataPoints.value("PF20").isValid())
        m_pf20 = m_dataPoints.value("PF20").toFloatWithSSF(m_parentModel->pfSf());

    if (m_dataPoints.value("CrvNam").isValid())
        m_crvNam = m_dataPoints.value("CrvNam").toString();

    if (m_dataPoints.value("RmpPT1Tms").isValid())
        m_rmpPt1Tms = m_dataPoints.value("RmpPT1Tms").toUInt16();

    if (m_dataPoints.value("RmpDecTmm").isValid())
        m_rmpDecTmm = m_dataPoints.value("RmpDecTmm").toFloatWithSSF(m_parentModel->rmpIncDecSf());

    if (m_dataPoints.value("RmpIncTmm").isValid())
        m_rmpIncTmm = m_dataPoints.value("RmpIncTmm").toFloatWithSSF(m_parentModel->rmpIncDecSf());

    if (m_dataPoints.value("ReadOnly").isValid())
        m_readOnly = static_cast<Readonly>(m_dataPoints.value("ReadOnly").toUInt16());

    if (m_dataPoints.value("Pad").isValid())
        m_pad = m_dataPoints.value("Pad").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}


SunSpecWattPfModel::SunSpecWattPfModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 131, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixedAndRepeating;

    initDataPoints();
}

SunSpecWattPfModel::~SunSpecWattPfModel()
{

}

QString SunSpecWattPfModel::name() const
{
    return "watt_pf";
}

QString SunSpecWattPfModel::description() const
{
    return "Watt-Power Factor ";
}

QString SunSpecWattPfModel::label() const
{
    return "Watt-PF";
}

quint16 SunSpecWattPfModel::actCrv() const
{
    return m_actCrv;
}

QModbusReply *SunSpecWattPfModel::setActCrv(quint16 actCrv)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("ActCrv");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(actCrv);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecWattPfModel::ModenaFlags SunSpecWattPfModel::modEna() const
{
    return m_modEna;
}

QModbusReply *SunSpecWattPfModel::setModEna(ModenaFlags modEna)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("ModEna");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(modEna));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecWattPfModel::winTms() const
{
    return m_winTms;
}

QModbusReply *SunSpecWattPfModel::setWinTms(quint16 winTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("WinTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(winTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecWattPfModel::rvrtTms() const
{
    return m_rvrtTms;
}

QModbusReply *SunSpecWattPfModel::setRvrtTms(quint16 rvrtTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("RvrtTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(rvrtTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecWattPfModel::rmpTms() const
{
    return m_rmpTms;
}

QModbusReply *SunSpecWattPfModel::setRmpTms(quint16 rmpTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("RmpTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(rmpTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecWattPfModel::nCrv() const
{
    return m_nCrv;
}
quint16 SunSpecWattPfModel::nPt() const
{
    return m_nPt;
}
qint16 SunSpecWattPfModel::wSf() const
{
    return m_wSf;
}
qint16 SunSpecWattPfModel::pfSf() const
{
    return m_pfSf;
}
qint16 SunSpecWattPfModel::rmpIncDecSf() const
{
    return m_rmpIncDecSf;
}
void SunSpecWattPfModel::initDataPoints()
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
    modEnaDataPoint.setDescription("Is watt-PF mode active.");
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
    winTmsDataPoint.setDescription("Time window for watt-PF change.");
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
    rvrtTmsDataPoint.setDescription("Timeout period for watt-PF curve selection.");
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
    nCrvDataPoint.setDescription("Number of curves supported (recommend 4).");
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
    nPtDataPoint.setDescription("Max number of points in array.");
    nPtDataPoint.setMandatory(true);
    nPtDataPoint.setSize(1);
    nPtDataPoint.setAddressOffset(8);
    nPtDataPoint.setBlockOffset(6);
    nPtDataPoint.setSunSpecDataType("uint16");
    nPtDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(nPtDataPoint.name(), nPtDataPoint);

    SunSpecDataPoint wSfDataPoint;
    wSfDataPoint.setName("W_SF");
    wSfDataPoint.setLabel("W_SF");
    wSfDataPoint.setDescription("Scale factor for percent WMax.");
    wSfDataPoint.setMandatory(true);
    wSfDataPoint.setSize(1);
    wSfDataPoint.setAddressOffset(9);
    wSfDataPoint.setBlockOffset(7);
    wSfDataPoint.setSunSpecDataType("sunssf");
    wSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wSfDataPoint.name(), wSfDataPoint);

    SunSpecDataPoint pfSfDataPoint;
    pfSfDataPoint.setName("PF_SF");
    pfSfDataPoint.setLabel("PF_SF");
    pfSfDataPoint.setDescription("Scale factor for PF.");
    pfSfDataPoint.setMandatory(true);
    pfSfDataPoint.setSize(1);
    pfSfDataPoint.setAddressOffset(10);
    pfSfDataPoint.setBlockOffset(8);
    pfSfDataPoint.setSunSpecDataType("sunssf");
    pfSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pfSfDataPoint.name(), pfSfDataPoint);

    SunSpecDataPoint rmpIncDecSfDataPoint;
    rmpIncDecSfDataPoint.setName("RmpIncDec_SF");
    rmpIncDecSfDataPoint.setLabel("RmpIncDec_SF");
    rmpIncDecSfDataPoint.setDescription("Scale factor for increment and decrement ramps.");
    rmpIncDecSfDataPoint.setSize(1);
    rmpIncDecSfDataPoint.setAddressOffset(11);
    rmpIncDecSfDataPoint.setBlockOffset(9);
    rmpIncDecSfDataPoint.setSunSpecDataType("sunssf");
    rmpIncDecSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(rmpIncDecSfDataPoint.name(), rmpIncDecSfDataPoint);

}

void SunSpecWattPfModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("W_SF").isValid())
        m_wSf = m_dataPoints.value("W_SF").toInt16();

    if (m_dataPoints.value("PF_SF").isValid())
        m_pfSf = m_dataPoints.value("PF_SF").toInt16();

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

    if (m_dataPoints.value("W_SF").isValid())
        m_wSf = m_dataPoints.value("W_SF").toInt16();

    if (m_dataPoints.value("PF_SF").isValid())
        m_pfSf = m_dataPoints.value("PF_SF").toInt16();

    if (m_dataPoints.value("RmpIncDec_SF").isValid())
        m_rmpIncDecSf = m_dataPoints.value("RmpIncDec_SF").toInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecWattPfModel *model)
{
    debug.nospace().noquote() << "SunSpecWattPfModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
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
