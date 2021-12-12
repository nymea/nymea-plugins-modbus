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

#include "sunspecvoltwattmodel.h"
#include "sunspecconnection.h"

SunSpecVoltWattModelRepeatingBlock::SunSpecVoltWattModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecVoltWattModel *parent) :
    SunSpecModelRepeatingBlock(blockIndex, blockSize, modbusStartRegister, parent)
{
    m_byteOrder = parent->byteOrder();
    initDataPoints();
}

QString SunSpecVoltWattModelRepeatingBlock::name() const
{
    return "curve";
}

SunSpecVoltWattModel *SunSpecVoltWattModelRepeatingBlock::parentModel() const
{
    return m_parentModel;
}

quint16 SunSpecVoltWattModelRepeatingBlock::actPt() const
{
    return m_actPt;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setActPt(quint16 actPt)
{
    SunSpecDataPoint dp = m_dataPoints.value("ActPt");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(actPt);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

SunSpecVoltWattModelRepeatingBlock::Deptref SunSpecVoltWattModelRepeatingBlock::deptRef() const
{
    return m_deptRef;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setDeptRef(Deptref deptRef)
{
    SunSpecDataPoint dp = m_dataPoints.value("DeptRef");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(deptRef));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::v1() const
{
    return m_v1;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setV1(float v1)
{
    SunSpecDataPoint dp = m_dataPoints.value("V1");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v1, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::w1() const
{
    return m_w1;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setW1(float w1)
{
    SunSpecDataPoint dp = m_dataPoints.value("W1");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w1, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::v2() const
{
    return m_v2;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setV2(float v2)
{
    SunSpecDataPoint dp = m_dataPoints.value("V2");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v2, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::w2() const
{
    return m_w2;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setW2(float w2)
{
    SunSpecDataPoint dp = m_dataPoints.value("W2");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w2, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::v3() const
{
    return m_v3;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setV3(float v3)
{
    SunSpecDataPoint dp = m_dataPoints.value("V3");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v3, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::w3() const
{
    return m_w3;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setW3(float w3)
{
    SunSpecDataPoint dp = m_dataPoints.value("W3");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w3, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::v4() const
{
    return m_v4;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setV4(float v4)
{
    SunSpecDataPoint dp = m_dataPoints.value("V4");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v4, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::w4() const
{
    return m_w4;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setW4(float w4)
{
    SunSpecDataPoint dp = m_dataPoints.value("W4");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w4, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::v5() const
{
    return m_v5;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setV5(float v5)
{
    SunSpecDataPoint dp = m_dataPoints.value("V5");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v5, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::w5() const
{
    return m_w5;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setW5(float w5)
{
    SunSpecDataPoint dp = m_dataPoints.value("W5");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w5, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::v6() const
{
    return m_v6;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setV6(float v6)
{
    SunSpecDataPoint dp = m_dataPoints.value("V6");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v6, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::w6() const
{
    return m_w6;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setW6(float w6)
{
    SunSpecDataPoint dp = m_dataPoints.value("W6");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w6, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::v7() const
{
    return m_v7;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setV7(float v7)
{
    SunSpecDataPoint dp = m_dataPoints.value("V7");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v7, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::w7() const
{
    return m_w7;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setW7(float w7)
{
    SunSpecDataPoint dp = m_dataPoints.value("W7");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w7, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::v8() const
{
    return m_v8;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setV8(float v8)
{
    SunSpecDataPoint dp = m_dataPoints.value("V8");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v8, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::w8() const
{
    return m_w8;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setW8(float w8)
{
    SunSpecDataPoint dp = m_dataPoints.value("W8");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w8, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::v9() const
{
    return m_v9;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setV9(float v9)
{
    SunSpecDataPoint dp = m_dataPoints.value("V9");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v9, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::w9() const
{
    return m_w9;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setW9(float w9)
{
    SunSpecDataPoint dp = m_dataPoints.value("W9");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w9, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::v10() const
{
    return m_v10;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setV10(float v10)
{
    SunSpecDataPoint dp = m_dataPoints.value("V10");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v10, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::w10() const
{
    return m_w10;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setW10(float w10)
{
    SunSpecDataPoint dp = m_dataPoints.value("W10");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w10, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::v11() const
{
    return m_v11;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setV11(float v11)
{
    SunSpecDataPoint dp = m_dataPoints.value("V11");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v11, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::w11() const
{
    return m_w11;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setW11(float w11)
{
    SunSpecDataPoint dp = m_dataPoints.value("W11");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w11, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::v12() const
{
    return m_v12;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setV12(float v12)
{
    SunSpecDataPoint dp = m_dataPoints.value("V12");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v12, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::w12() const
{
    return m_w12;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setW12(float w12)
{
    SunSpecDataPoint dp = m_dataPoints.value("W12");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w12, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::v13() const
{
    return m_v13;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setV13(float v13)
{
    SunSpecDataPoint dp = m_dataPoints.value("V13");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v13, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::w13() const
{
    return m_w13;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setW13(float w13)
{
    SunSpecDataPoint dp = m_dataPoints.value("W13");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w13, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::v14() const
{
    return m_v14;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setV14(float v14)
{
    SunSpecDataPoint dp = m_dataPoints.value("V14");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v14, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::w14() const
{
    return m_w14;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setW14(float w14)
{
    SunSpecDataPoint dp = m_dataPoints.value("W14");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w14, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::v15() const
{
    return m_v15;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setV15(float v15)
{
    SunSpecDataPoint dp = m_dataPoints.value("V15");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v15, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::w15() const
{
    return m_w15;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setW15(float w15)
{
    SunSpecDataPoint dp = m_dataPoints.value("W15");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w15, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::v16() const
{
    return m_v16;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setV16(float v16)
{
    SunSpecDataPoint dp = m_dataPoints.value("V16");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v16, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::w16() const
{
    return m_w16;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setW16(float w16)
{
    SunSpecDataPoint dp = m_dataPoints.value("W16");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w16, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::v17() const
{
    return m_v17;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setV17(float v17)
{
    SunSpecDataPoint dp = m_dataPoints.value("V17");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v17, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::w17() const
{
    return m_w17;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setW17(float w17)
{
    SunSpecDataPoint dp = m_dataPoints.value("W17");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w17, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::v18() const
{
    return m_v18;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setV18(float v18)
{
    SunSpecDataPoint dp = m_dataPoints.value("V18");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v18, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::w18() const
{
    return m_w18;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setW18(float w18)
{
    SunSpecDataPoint dp = m_dataPoints.value("W18");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w18, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::v19() const
{
    return m_v19;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setV19(float v19)
{
    SunSpecDataPoint dp = m_dataPoints.value("V19");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v19, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::w19() const
{
    return m_w19;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setW19(float w19)
{
    SunSpecDataPoint dp = m_dataPoints.value("W19");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w19, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::v20() const
{
    return m_v20;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setV20(float v20)
{
    SunSpecDataPoint dp = m_dataPoints.value("V20");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v20, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::w20() const
{
    return m_w20;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setW20(float w20)
{
    SunSpecDataPoint dp = m_dataPoints.value("W20");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(w20, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

QString SunSpecVoltWattModelRepeatingBlock::crvNam() const
{
    return m_crvNam;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setCrvNam(const QString &crvNam)
{
    SunSpecDataPoint dp = m_dataPoints.value("CrvNam");
    QVector<quint16> registers = SunSpecDataPoint::convertFromString(crvNam, dp.size());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

quint16 SunSpecVoltWattModelRepeatingBlock::rmpPt1Tms() const
{
    return m_rmpPt1Tms;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setRmpPt1Tms(quint16 rmpPt1Tms)
{
    SunSpecDataPoint dp = m_dataPoints.value("RmpPt1Tms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(rmpPt1Tms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::rmpDecTmm() const
{
    return m_rmpDecTmm;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setRmpDecTmm(float rmpDecTmm)
{
    SunSpecDataPoint dp = m_dataPoints.value("RmpDecTmm");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(rmpDecTmm, m_parentModel->rmpIncDecSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltWattModelRepeatingBlock::rmpIncTmm() const
{
    return m_rmpIncTmm;
}

QModbusReply *SunSpecVoltWattModelRepeatingBlock::setRmpIncTmm(float rmpIncTmm)
{
    SunSpecDataPoint dp = m_dataPoints.value("RmpIncTmm");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(rmpIncTmm, m_parentModel->rmpIncDecSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

SunSpecVoltWattModelRepeatingBlock::Readonly SunSpecVoltWattModelRepeatingBlock::readOnly() const
{
    return m_readOnly;
}

void SunSpecVoltWattModelRepeatingBlock::initDataPoints()
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

    SunSpecDataPoint deptRefDataPoint;
    deptRefDataPoint.setName("DeptRef");
    deptRefDataPoint.setLabel("DeptRef");
    deptRefDataPoint.setDescription("Defines the meaning of the Watts DeptRef.  1=% WMax 2=% WAvail");
    deptRefDataPoint.setMandatory(true);
    deptRefDataPoint.setSize(1);
    deptRefDataPoint.setAddressOffset(1);
    deptRefDataPoint.setSunSpecDataType("enum16");
    deptRefDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    deptRefDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(deptRefDataPoint.name(), deptRefDataPoint);

    SunSpecDataPoint v1DataPoint;
    v1DataPoint.setName("V1");
    v1DataPoint.setLabel("V1");
    v1DataPoint.setDescription("Point 1 Volts.");
    v1DataPoint.setUnits("% VRef");
    v1DataPoint.setMandatory(true);
    v1DataPoint.setSize(1);
    v1DataPoint.setAddressOffset(2);
    v1DataPoint.setBlockOffset(0);
    v1DataPoint.setScaleFactorName("V_SF");
    v1DataPoint.setSunSpecDataType("uint16");
    v1DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v1DataPoint.name(), v1DataPoint);

    SunSpecDataPoint w1DataPoint;
    w1DataPoint.setName("W1");
    w1DataPoint.setLabel("W1");
    w1DataPoint.setDescription("Point 1 Watts.");
    w1DataPoint.setUnits("% VRef");
    w1DataPoint.setMandatory(true);
    w1DataPoint.setSize(1);
    w1DataPoint.setAddressOffset(3);
    w1DataPoint.setBlockOffset(1);
    w1DataPoint.setScaleFactorName("DeptRef_SF");
    w1DataPoint.setSunSpecDataType("int16");
    w1DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w1DataPoint.name(), w1DataPoint);

    SunSpecDataPoint v2DataPoint;
    v2DataPoint.setName("V2");
    v2DataPoint.setLabel("V2");
    v2DataPoint.setDescription("Point 2 Volts.");
    v2DataPoint.setUnits("% VRef");
    v2DataPoint.setSize(1);
    v2DataPoint.setAddressOffset(4);
    v2DataPoint.setBlockOffset(2);
    v2DataPoint.setScaleFactorName("V_SF");
    v2DataPoint.setSunSpecDataType("uint16");
    v2DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v2DataPoint.name(), v2DataPoint);

    SunSpecDataPoint w2DataPoint;
    w2DataPoint.setName("W2");
    w2DataPoint.setLabel("W2");
    w2DataPoint.setDescription("Point 2 Watts.");
    w2DataPoint.setUnits("% VRef");
    w2DataPoint.setSize(1);
    w2DataPoint.setAddressOffset(5);
    w2DataPoint.setBlockOffset(3);
    w2DataPoint.setScaleFactorName("DeptRef_SF");
    w2DataPoint.setSunSpecDataType("int16");
    w2DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w2DataPoint.name(), w2DataPoint);

    SunSpecDataPoint v3DataPoint;
    v3DataPoint.setName("V3");
    v3DataPoint.setLabel("V3");
    v3DataPoint.setDescription("Point 3 Volts.");
    v3DataPoint.setUnits("% VRef");
    v3DataPoint.setSize(1);
    v3DataPoint.setAddressOffset(6);
    v3DataPoint.setBlockOffset(4);
    v3DataPoint.setScaleFactorName("V_SF");
    v3DataPoint.setSunSpecDataType("uint16");
    v3DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v3DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v3DataPoint.name(), v3DataPoint);

    SunSpecDataPoint w3DataPoint;
    w3DataPoint.setName("W3");
    w3DataPoint.setLabel("W3");
    w3DataPoint.setDescription("Point 3 Watts.");
    w3DataPoint.setUnits("% VRef");
    w3DataPoint.setSize(1);
    w3DataPoint.setAddressOffset(7);
    w3DataPoint.setBlockOffset(5);
    w3DataPoint.setScaleFactorName("DeptRef_SF");
    w3DataPoint.setSunSpecDataType("int16");
    w3DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w3DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w3DataPoint.name(), w3DataPoint);

    SunSpecDataPoint v4DataPoint;
    v4DataPoint.setName("V4");
    v4DataPoint.setLabel("V4");
    v4DataPoint.setDescription("Point 4 Volts.");
    v4DataPoint.setUnits("% VRef");
    v4DataPoint.setSize(1);
    v4DataPoint.setAddressOffset(8);
    v4DataPoint.setBlockOffset(6);
    v4DataPoint.setScaleFactorName("V_SF");
    v4DataPoint.setSunSpecDataType("uint16");
    v4DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v4DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v4DataPoint.name(), v4DataPoint);

    SunSpecDataPoint w4DataPoint;
    w4DataPoint.setName("W4");
    w4DataPoint.setLabel("W4");
    w4DataPoint.setDescription("Point 4 Watts.");
    w4DataPoint.setUnits("% VRef");
    w4DataPoint.setSize(1);
    w4DataPoint.setAddressOffset(9);
    w4DataPoint.setBlockOffset(7);
    w4DataPoint.setScaleFactorName("DeptRef_SF");
    w4DataPoint.setSunSpecDataType("int16");
    w4DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w4DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w4DataPoint.name(), w4DataPoint);

    SunSpecDataPoint v5DataPoint;
    v5DataPoint.setName("V5");
    v5DataPoint.setLabel("V5");
    v5DataPoint.setDescription("Point 5 Volts.");
    v5DataPoint.setUnits("% VRef");
    v5DataPoint.setSize(1);
    v5DataPoint.setAddressOffset(10);
    v5DataPoint.setBlockOffset(8);
    v5DataPoint.setScaleFactorName("V_SF");
    v5DataPoint.setSunSpecDataType("uint16");
    v5DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v5DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v5DataPoint.name(), v5DataPoint);

    SunSpecDataPoint w5DataPoint;
    w5DataPoint.setName("W5");
    w5DataPoint.setLabel("W5");
    w5DataPoint.setDescription("Point 5 Watts.");
    w5DataPoint.setUnits("% VRef");
    w5DataPoint.setSize(1);
    w5DataPoint.setAddressOffset(11);
    w5DataPoint.setBlockOffset(9);
    w5DataPoint.setScaleFactorName("DeptRef_SF");
    w5DataPoint.setSunSpecDataType("int16");
    w5DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w5DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w5DataPoint.name(), w5DataPoint);

    SunSpecDataPoint v6DataPoint;
    v6DataPoint.setName("V6");
    v6DataPoint.setLabel("V6");
    v6DataPoint.setDescription("Point 6 Volts.");
    v6DataPoint.setUnits("% VRef");
    v6DataPoint.setSize(1);
    v6DataPoint.setAddressOffset(12);
    v6DataPoint.setBlockOffset(10);
    v6DataPoint.setScaleFactorName("V_SF");
    v6DataPoint.setSunSpecDataType("uint16");
    v6DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v6DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v6DataPoint.name(), v6DataPoint);

    SunSpecDataPoint w6DataPoint;
    w6DataPoint.setName("W6");
    w6DataPoint.setLabel("W6");
    w6DataPoint.setDescription("Point 6 Watts.");
    w6DataPoint.setUnits("% VRef");
    w6DataPoint.setSize(1);
    w6DataPoint.setAddressOffset(13);
    w6DataPoint.setBlockOffset(11);
    w6DataPoint.setScaleFactorName("DeptRef_SF");
    w6DataPoint.setSunSpecDataType("int16");
    w6DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w6DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w6DataPoint.name(), w6DataPoint);

    SunSpecDataPoint v7DataPoint;
    v7DataPoint.setName("V7");
    v7DataPoint.setLabel("V7");
    v7DataPoint.setDescription("Point 7 Volts.");
    v7DataPoint.setUnits("% VRef");
    v7DataPoint.setSize(1);
    v7DataPoint.setAddressOffset(14);
    v7DataPoint.setBlockOffset(12);
    v7DataPoint.setScaleFactorName("V_SF");
    v7DataPoint.setSunSpecDataType("uint16");
    v7DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v7DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v7DataPoint.name(), v7DataPoint);

    SunSpecDataPoint w7DataPoint;
    w7DataPoint.setName("W7");
    w7DataPoint.setLabel("W7");
    w7DataPoint.setDescription("Point 7 Watts.");
    w7DataPoint.setUnits("% VRef");
    w7DataPoint.setSize(1);
    w7DataPoint.setAddressOffset(15);
    w7DataPoint.setBlockOffset(13);
    w7DataPoint.setScaleFactorName("DeptRef_SF");
    w7DataPoint.setSunSpecDataType("int16");
    w7DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w7DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w7DataPoint.name(), w7DataPoint);

    SunSpecDataPoint v8DataPoint;
    v8DataPoint.setName("V8");
    v8DataPoint.setLabel("V8");
    v8DataPoint.setDescription("Point 8 Volts.");
    v8DataPoint.setUnits("% VRef");
    v8DataPoint.setSize(1);
    v8DataPoint.setAddressOffset(16);
    v8DataPoint.setBlockOffset(14);
    v8DataPoint.setScaleFactorName("V_SF");
    v8DataPoint.setSunSpecDataType("uint16");
    v8DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v8DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v8DataPoint.name(), v8DataPoint);

    SunSpecDataPoint w8DataPoint;
    w8DataPoint.setName("W8");
    w8DataPoint.setLabel("W8");
    w8DataPoint.setDescription("Point 8 Watts.");
    w8DataPoint.setUnits("% VRef");
    w8DataPoint.setSize(1);
    w8DataPoint.setAddressOffset(17);
    w8DataPoint.setBlockOffset(15);
    w8DataPoint.setScaleFactorName("DeptRef_SF");
    w8DataPoint.setSunSpecDataType("int16");
    w8DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w8DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w8DataPoint.name(), w8DataPoint);

    SunSpecDataPoint v9DataPoint;
    v9DataPoint.setName("V9");
    v9DataPoint.setLabel("V9");
    v9DataPoint.setDescription("Point 9 Volts.");
    v9DataPoint.setUnits("% VRef");
    v9DataPoint.setSize(1);
    v9DataPoint.setAddressOffset(18);
    v9DataPoint.setBlockOffset(16);
    v9DataPoint.setScaleFactorName("V_SF");
    v9DataPoint.setSunSpecDataType("uint16");
    v9DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v9DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v9DataPoint.name(), v9DataPoint);

    SunSpecDataPoint w9DataPoint;
    w9DataPoint.setName("W9");
    w9DataPoint.setLabel("W9");
    w9DataPoint.setDescription("Point 9 Watts.");
    w9DataPoint.setUnits("% VRef");
    w9DataPoint.setSize(1);
    w9DataPoint.setAddressOffset(19);
    w9DataPoint.setBlockOffset(17);
    w9DataPoint.setScaleFactorName("DeptRef_SF");
    w9DataPoint.setSunSpecDataType("int16");
    w9DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w9DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w9DataPoint.name(), w9DataPoint);

    SunSpecDataPoint v10DataPoint;
    v10DataPoint.setName("V10");
    v10DataPoint.setLabel("V10");
    v10DataPoint.setDescription("Point 10 Volts.");
    v10DataPoint.setUnits("% VRef");
    v10DataPoint.setSize(1);
    v10DataPoint.setAddressOffset(20);
    v10DataPoint.setBlockOffset(18);
    v10DataPoint.setScaleFactorName("V_SF");
    v10DataPoint.setSunSpecDataType("uint16");
    v10DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v10DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v10DataPoint.name(), v10DataPoint);

    SunSpecDataPoint w10DataPoint;
    w10DataPoint.setName("W10");
    w10DataPoint.setLabel("W10");
    w10DataPoint.setDescription("Point 10 Watts.");
    w10DataPoint.setUnits("% VRef");
    w10DataPoint.setSize(1);
    w10DataPoint.setAddressOffset(21);
    w10DataPoint.setBlockOffset(19);
    w10DataPoint.setScaleFactorName("DeptRef_SF");
    w10DataPoint.setSunSpecDataType("int16");
    w10DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w10DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w10DataPoint.name(), w10DataPoint);

    SunSpecDataPoint v11DataPoint;
    v11DataPoint.setName("V11");
    v11DataPoint.setLabel("V11");
    v11DataPoint.setDescription("Point 11 Volts.");
    v11DataPoint.setUnits("% VRef");
    v11DataPoint.setSize(1);
    v11DataPoint.setAddressOffset(22);
    v11DataPoint.setBlockOffset(20);
    v11DataPoint.setScaleFactorName("V_SF");
    v11DataPoint.setSunSpecDataType("uint16");
    v11DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v11DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v11DataPoint.name(), v11DataPoint);

    SunSpecDataPoint w11DataPoint;
    w11DataPoint.setName("W11");
    w11DataPoint.setLabel("W11");
    w11DataPoint.setDescription("Point 11 Watts.");
    w11DataPoint.setUnits("% VRef");
    w11DataPoint.setSize(1);
    w11DataPoint.setAddressOffset(23);
    w11DataPoint.setBlockOffset(21);
    w11DataPoint.setScaleFactorName("DeptRef_SF");
    w11DataPoint.setSunSpecDataType("int16");
    w11DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w11DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w11DataPoint.name(), w11DataPoint);

    SunSpecDataPoint v12DataPoint;
    v12DataPoint.setName("V12");
    v12DataPoint.setLabel("V12");
    v12DataPoint.setDescription("Point 12 Volts.");
    v12DataPoint.setUnits("% VRef");
    v12DataPoint.setSize(1);
    v12DataPoint.setAddressOffset(24);
    v12DataPoint.setBlockOffset(22);
    v12DataPoint.setScaleFactorName("V_SF");
    v12DataPoint.setSunSpecDataType("uint16");
    v12DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v12DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v12DataPoint.name(), v12DataPoint);

    SunSpecDataPoint w12DataPoint;
    w12DataPoint.setName("W12");
    w12DataPoint.setLabel("W12");
    w12DataPoint.setDescription("Point 12 Watts.");
    w12DataPoint.setUnits("% VRef");
    w12DataPoint.setSize(1);
    w12DataPoint.setAddressOffset(25);
    w12DataPoint.setBlockOffset(23);
    w12DataPoint.setScaleFactorName("DeptRef_SF");
    w12DataPoint.setSunSpecDataType("int16");
    w12DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w12DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w12DataPoint.name(), w12DataPoint);

    SunSpecDataPoint v13DataPoint;
    v13DataPoint.setName("V13");
    v13DataPoint.setLabel("V13");
    v13DataPoint.setDescription("Point 13 Volts.");
    v13DataPoint.setUnits("% VRef");
    v13DataPoint.setSize(1);
    v13DataPoint.setAddressOffset(26);
    v13DataPoint.setBlockOffset(24);
    v13DataPoint.setScaleFactorName("V_SF");
    v13DataPoint.setSunSpecDataType("uint16");
    v13DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v13DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v13DataPoint.name(), v13DataPoint);

    SunSpecDataPoint w13DataPoint;
    w13DataPoint.setName("W13");
    w13DataPoint.setLabel("W13");
    w13DataPoint.setDescription("Point 13 Watts.");
    w13DataPoint.setUnits("% VRef");
    w13DataPoint.setSize(1);
    w13DataPoint.setAddressOffset(27);
    w13DataPoint.setBlockOffset(25);
    w13DataPoint.setScaleFactorName("DeptRef_SF");
    w13DataPoint.setSunSpecDataType("int16");
    w13DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w13DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w13DataPoint.name(), w13DataPoint);

    SunSpecDataPoint v14DataPoint;
    v14DataPoint.setName("V14");
    v14DataPoint.setLabel("V14");
    v14DataPoint.setDescription("Point 14 Volts.");
    v14DataPoint.setUnits("% VRef");
    v14DataPoint.setSize(1);
    v14DataPoint.setAddressOffset(28);
    v14DataPoint.setBlockOffset(26);
    v14DataPoint.setScaleFactorName("V_SF");
    v14DataPoint.setSunSpecDataType("uint16");
    v14DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v14DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v14DataPoint.name(), v14DataPoint);

    SunSpecDataPoint w14DataPoint;
    w14DataPoint.setName("W14");
    w14DataPoint.setLabel("W14");
    w14DataPoint.setDescription("Point 14 Watts.");
    w14DataPoint.setUnits("% VRef");
    w14DataPoint.setSize(1);
    w14DataPoint.setAddressOffset(29);
    w14DataPoint.setBlockOffset(27);
    w14DataPoint.setScaleFactorName("DeptRef_SF");
    w14DataPoint.setSunSpecDataType("int16");
    w14DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w14DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w14DataPoint.name(), w14DataPoint);

    SunSpecDataPoint v15DataPoint;
    v15DataPoint.setName("V15");
    v15DataPoint.setLabel("V15");
    v15DataPoint.setDescription("Point 15 Volts.");
    v15DataPoint.setUnits("% VRef");
    v15DataPoint.setSize(1);
    v15DataPoint.setAddressOffset(30);
    v15DataPoint.setBlockOffset(28);
    v15DataPoint.setScaleFactorName("V_SF");
    v15DataPoint.setSunSpecDataType("uint16");
    v15DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v15DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v15DataPoint.name(), v15DataPoint);

    SunSpecDataPoint w15DataPoint;
    w15DataPoint.setName("W15");
    w15DataPoint.setLabel("W15");
    w15DataPoint.setDescription("Point 15 Watts.");
    w15DataPoint.setUnits("% VRef");
    w15DataPoint.setSize(1);
    w15DataPoint.setAddressOffset(31);
    w15DataPoint.setBlockOffset(29);
    w15DataPoint.setScaleFactorName("DeptRef_SF");
    w15DataPoint.setSunSpecDataType("int16");
    w15DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w15DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w15DataPoint.name(), w15DataPoint);

    SunSpecDataPoint v16DataPoint;
    v16DataPoint.setName("V16");
    v16DataPoint.setLabel("V16");
    v16DataPoint.setDescription("Point 16 Volts.");
    v16DataPoint.setUnits("% VRef");
    v16DataPoint.setSize(1);
    v16DataPoint.setAddressOffset(32);
    v16DataPoint.setBlockOffset(30);
    v16DataPoint.setScaleFactorName("V_SF");
    v16DataPoint.setSunSpecDataType("uint16");
    v16DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v16DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v16DataPoint.name(), v16DataPoint);

    SunSpecDataPoint w16DataPoint;
    w16DataPoint.setName("W16");
    w16DataPoint.setLabel("W16");
    w16DataPoint.setDescription("Point 16 Watts.");
    w16DataPoint.setUnits("% VRef");
    w16DataPoint.setSize(1);
    w16DataPoint.setAddressOffset(33);
    w16DataPoint.setBlockOffset(31);
    w16DataPoint.setScaleFactorName("DeptRef_SF");
    w16DataPoint.setSunSpecDataType("int16");
    w16DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w16DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w16DataPoint.name(), w16DataPoint);

    SunSpecDataPoint v17DataPoint;
    v17DataPoint.setName("V17");
    v17DataPoint.setLabel("V17");
    v17DataPoint.setDescription("Point 17 Volts.");
    v17DataPoint.setUnits("% VRef");
    v17DataPoint.setSize(1);
    v17DataPoint.setAddressOffset(34);
    v17DataPoint.setBlockOffset(32);
    v17DataPoint.setScaleFactorName("V_SF");
    v17DataPoint.setSunSpecDataType("uint16");
    v17DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v17DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v17DataPoint.name(), v17DataPoint);

    SunSpecDataPoint w17DataPoint;
    w17DataPoint.setName("W17");
    w17DataPoint.setLabel("W17");
    w17DataPoint.setDescription("Point 17 Watts.");
    w17DataPoint.setUnits("% VRef");
    w17DataPoint.setSize(1);
    w17DataPoint.setAddressOffset(35);
    w17DataPoint.setBlockOffset(33);
    w17DataPoint.setScaleFactorName("DeptRef_SF");
    w17DataPoint.setSunSpecDataType("int16");
    w17DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w17DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w17DataPoint.name(), w17DataPoint);

    SunSpecDataPoint v18DataPoint;
    v18DataPoint.setName("V18");
    v18DataPoint.setLabel("V18");
    v18DataPoint.setDescription("Point 18 Volts.");
    v18DataPoint.setUnits("% VRef");
    v18DataPoint.setSize(1);
    v18DataPoint.setAddressOffset(36);
    v18DataPoint.setBlockOffset(34);
    v18DataPoint.setScaleFactorName("V_SF");
    v18DataPoint.setSunSpecDataType("uint16");
    v18DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v18DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v18DataPoint.name(), v18DataPoint);

    SunSpecDataPoint w18DataPoint;
    w18DataPoint.setName("W18");
    w18DataPoint.setLabel("W18");
    w18DataPoint.setDescription("Point 18 Watts.");
    w18DataPoint.setUnits("% VRef");
    w18DataPoint.setSize(1);
    w18DataPoint.setAddressOffset(37);
    w18DataPoint.setBlockOffset(35);
    w18DataPoint.setScaleFactorName("DeptRef_SF");
    w18DataPoint.setSunSpecDataType("int16");
    w18DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w18DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w18DataPoint.name(), w18DataPoint);

    SunSpecDataPoint v19DataPoint;
    v19DataPoint.setName("V19");
    v19DataPoint.setLabel("V19");
    v19DataPoint.setDescription("Point 19 Volts.");
    v19DataPoint.setUnits("% VRef");
    v19DataPoint.setSize(1);
    v19DataPoint.setAddressOffset(38);
    v19DataPoint.setBlockOffset(36);
    v19DataPoint.setScaleFactorName("V_SF");
    v19DataPoint.setSunSpecDataType("uint16");
    v19DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v19DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v19DataPoint.name(), v19DataPoint);

    SunSpecDataPoint w19DataPoint;
    w19DataPoint.setName("W19");
    w19DataPoint.setLabel("W19");
    w19DataPoint.setDescription("Point 19 Watts.");
    w19DataPoint.setUnits("% VRef");
    w19DataPoint.setSize(1);
    w19DataPoint.setAddressOffset(39);
    w19DataPoint.setBlockOffset(37);
    w19DataPoint.setScaleFactorName("DeptRef_SF");
    w19DataPoint.setSunSpecDataType("int16");
    w19DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w19DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w19DataPoint.name(), w19DataPoint);

    SunSpecDataPoint v20DataPoint;
    v20DataPoint.setName("V20");
    v20DataPoint.setLabel("V20");
    v20DataPoint.setDescription("Point 20 Volts.");
    v20DataPoint.setUnits("% VRef");
    v20DataPoint.setSize(1);
    v20DataPoint.setAddressOffset(40);
    v20DataPoint.setBlockOffset(38);
    v20DataPoint.setScaleFactorName("V_SF");
    v20DataPoint.setSunSpecDataType("uint16");
    v20DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v20DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v20DataPoint.name(), v20DataPoint);

    SunSpecDataPoint w20DataPoint;
    w20DataPoint.setName("W20");
    w20DataPoint.setLabel("W20");
    w20DataPoint.setDescription("Point 20 Watts.");
    w20DataPoint.setUnits("% VRef");
    w20DataPoint.setSize(1);
    w20DataPoint.setAddressOffset(41);
    w20DataPoint.setBlockOffset(39);
    w20DataPoint.setScaleFactorName("DeptRef_SF");
    w20DataPoint.setSunSpecDataType("int16");
    w20DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    w20DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(w20DataPoint.name(), w20DataPoint);

    SunSpecDataPoint crvNamDataPoint;
    crvNamDataPoint.setName("CrvNam");
    crvNamDataPoint.setLabel("CrvNam");
    crvNamDataPoint.setDescription("Optional description for curve.");
    crvNamDataPoint.setSize(8);
    crvNamDataPoint.setAddressOffset(42);
    crvNamDataPoint.setBlockOffset(40);
    crvNamDataPoint.setSunSpecDataType("string");
    crvNamDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    crvNamDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(crvNamDataPoint.name(), crvNamDataPoint);

    SunSpecDataPoint rmpPt1TmsDataPoint;
    rmpPt1TmsDataPoint.setName("RmpPt1Tms");
    rmpPt1TmsDataPoint.setLabel("RmpPt1Tms");
    rmpPt1TmsDataPoint.setDescription("The time of the PT1 in seconds (time to accomplish a change of 95%).");
    rmpPt1TmsDataPoint.setUnits("Secs");
    rmpPt1TmsDataPoint.setSize(1);
    rmpPt1TmsDataPoint.setAddressOffset(50);
    rmpPt1TmsDataPoint.setBlockOffset(48);
    rmpPt1TmsDataPoint.setSunSpecDataType("uint16");
    rmpPt1TmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    rmpPt1TmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(rmpPt1TmsDataPoint.name(), rmpPt1TmsDataPoint);

    SunSpecDataPoint rmpDecTmmDataPoint;
    rmpDecTmmDataPoint.setName("RmpDecTmm");
    rmpDecTmmDataPoint.setLabel("RmpDecTmm");
    rmpDecTmmDataPoint.setDescription("The maximum rate at which the watt value may be reduced in response to changes in the voltage value.");
    rmpDecTmmDataPoint.setUnits("% WMax/min");
    rmpDecTmmDataPoint.setSize(1);
    rmpDecTmmDataPoint.setAddressOffset(51);
    rmpDecTmmDataPoint.setBlockOffset(49);
    rmpDecTmmDataPoint.setScaleFactorName("RmpIncDec_SF");
    rmpDecTmmDataPoint.setSunSpecDataType("uint16");
    rmpDecTmmDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    rmpDecTmmDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(rmpDecTmmDataPoint.name(), rmpDecTmmDataPoint);

    SunSpecDataPoint rmpIncTmmDataPoint;
    rmpIncTmmDataPoint.setName("RmpIncTmm");
    rmpIncTmmDataPoint.setLabel("RmpIncTmm");
    rmpIncTmmDataPoint.setDescription("The maximum rate at which the watt value may be increased in response to changes in the voltage value.");
    rmpIncTmmDataPoint.setUnits("% WMax/min");
    rmpIncTmmDataPoint.setSize(1);
    rmpIncTmmDataPoint.setAddressOffset(52);
    rmpIncTmmDataPoint.setBlockOffset(50);
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
    readOnlyDataPoint.setAddressOffset(53);
    readOnlyDataPoint.setBlockOffset(51);
    readOnlyDataPoint.setSunSpecDataType("enum16");
    readOnlyDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(readOnlyDataPoint.name(), readOnlyDataPoint);

}

void SunSpecVoltWattModelRepeatingBlock::processBlockData(const QVector<quint16> blockData)
{
    m_blockData = blockData;

    // Update properties according to the data point type
    if (m_dataPoints.value("ActPt").isValid())
        m_actPt = m_dataPoints.value("ActPt").toUInt16();

    if (m_dataPoints.value("DeptRef").isValid())
        m_deptRef = static_cast<Deptref>(m_dataPoints.value("DeptRef").toUInt16());

    if (m_dataPoints.value("V1").isValid())
        m_v1 = m_dataPoints.value("V1").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("W1").isValid())
        m_w1 = m_dataPoints.value("W1").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V2").isValid())
        m_v2 = m_dataPoints.value("V2").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("W2").isValid())
        m_w2 = m_dataPoints.value("W2").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V3").isValid())
        m_v3 = m_dataPoints.value("V3").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("W3").isValid())
        m_w3 = m_dataPoints.value("W3").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V4").isValid())
        m_v4 = m_dataPoints.value("V4").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("W4").isValid())
        m_w4 = m_dataPoints.value("W4").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V5").isValid())
        m_v5 = m_dataPoints.value("V5").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("W5").isValid())
        m_w5 = m_dataPoints.value("W5").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V6").isValid())
        m_v6 = m_dataPoints.value("V6").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("W6").isValid())
        m_w6 = m_dataPoints.value("W6").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V7").isValid())
        m_v7 = m_dataPoints.value("V7").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("W7").isValid())
        m_w7 = m_dataPoints.value("W7").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V8").isValid())
        m_v8 = m_dataPoints.value("V8").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("W8").isValid())
        m_w8 = m_dataPoints.value("W8").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V9").isValid())
        m_v9 = m_dataPoints.value("V9").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("W9").isValid())
        m_w9 = m_dataPoints.value("W9").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V10").isValid())
        m_v10 = m_dataPoints.value("V10").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("W10").isValid())
        m_w10 = m_dataPoints.value("W10").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V11").isValid())
        m_v11 = m_dataPoints.value("V11").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("W11").isValid())
        m_w11 = m_dataPoints.value("W11").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V12").isValid())
        m_v12 = m_dataPoints.value("V12").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("W12").isValid())
        m_w12 = m_dataPoints.value("W12").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V13").isValid())
        m_v13 = m_dataPoints.value("V13").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("W13").isValid())
        m_w13 = m_dataPoints.value("W13").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V14").isValid())
        m_v14 = m_dataPoints.value("V14").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("W14").isValid())
        m_w14 = m_dataPoints.value("W14").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V15").isValid())
        m_v15 = m_dataPoints.value("V15").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("W15").isValid())
        m_w15 = m_dataPoints.value("W15").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V16").isValid())
        m_v16 = m_dataPoints.value("V16").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("W16").isValid())
        m_w16 = m_dataPoints.value("W16").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V17").isValid())
        m_v17 = m_dataPoints.value("V17").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("W17").isValid())
        m_w17 = m_dataPoints.value("W17").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V18").isValid())
        m_v18 = m_dataPoints.value("V18").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("W18").isValid())
        m_w18 = m_dataPoints.value("W18").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V19").isValid())
        m_v19 = m_dataPoints.value("V19").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("W19").isValid())
        m_w19 = m_dataPoints.value("W19").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V20").isValid())
        m_v20 = m_dataPoints.value("V20").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("W20").isValid())
        m_w20 = m_dataPoints.value("W20").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("CrvNam").isValid())
        m_crvNam = m_dataPoints.value("CrvNam").toString();

    if (m_dataPoints.value("RmpPt1Tms").isValid())
        m_rmpPt1Tms = m_dataPoints.value("RmpPt1Tms").toUInt16();

    if (m_dataPoints.value("RmpDecTmm").isValid())
        m_rmpDecTmm = m_dataPoints.value("RmpDecTmm").toFloatWithSSF(m_parentModel->rmpIncDecSf());

    if (m_dataPoints.value("RmpIncTmm").isValid())
        m_rmpIncTmm = m_dataPoints.value("RmpIncTmm").toFloatWithSSF(m_parentModel->rmpIncDecSf());

    if (m_dataPoints.value("ReadOnly").isValid())
        m_readOnly = static_cast<Readonly>(m_dataPoints.value("ReadOnly").toUInt16());


    qCDebug(dcSunSpecModelData()) << this;
}


SunSpecVoltWattModel::SunSpecVoltWattModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 132, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixedAndRepeating;

    initDataPoints();
}

SunSpecVoltWattModel::~SunSpecVoltWattModel()
{

}

QString SunSpecVoltWattModel::name() const
{
    return "volt_watt";
}

QString SunSpecVoltWattModel::description() const
{
    return "Volt-Watt ";
}

QString SunSpecVoltWattModel::label() const
{
    return "Volt-Watt";
}

quint16 SunSpecVoltWattModel::actCrv() const
{
    return m_actCrv;
}

QModbusReply *SunSpecVoltWattModel::setActCrv(quint16 actCrv)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("ActCrv");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(actCrv);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecVoltWattModel::ModenaFlags SunSpecVoltWattModel::modEna() const
{
    return m_modEna;
}

QModbusReply *SunSpecVoltWattModel::setModEna(ModenaFlags modEna)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("ModEna");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(modEna));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecVoltWattModel::winTms() const
{
    return m_winTms;
}

QModbusReply *SunSpecVoltWattModel::setWinTms(quint16 winTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("WinTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(winTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecVoltWattModel::rvrtTms() const
{
    return m_rvrtTms;
}

QModbusReply *SunSpecVoltWattModel::setRvrtTms(quint16 rvrtTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("RvrtTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(rvrtTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecVoltWattModel::rmpTms() const
{
    return m_rmpTms;
}

QModbusReply *SunSpecVoltWattModel::setRmpTms(quint16 rmpTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("RmpTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(rmpTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecVoltWattModel::nCrv() const
{
    return m_nCrv;
}
quint16 SunSpecVoltWattModel::nPt() const
{
    return m_nPt;
}
qint16 SunSpecVoltWattModel::vSf() const
{
    return m_vSf;
}
qint16 SunSpecVoltWattModel::deptRefSf() const
{
    return m_deptRefSf;
}
qint16 SunSpecVoltWattModel::rmpIncDecSf() const
{
    return m_rmpIncDecSf;
}
void SunSpecVoltWattModel::initDataPoints()
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
    modEnaDataPoint.setDescription("Is Volt-Watt control active.");
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
    winTmsDataPoint.setDescription("Time window for volt-watt change.");
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
    rvrtTmsDataPoint.setDescription("Timeout period for volt-watt curve selection.");
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
    nPtDataPoint.setDescription("Number of points in array (maximum 20).");
    nPtDataPoint.setMandatory(true);
    nPtDataPoint.setSize(1);
    nPtDataPoint.setAddressOffset(8);
    nPtDataPoint.setBlockOffset(6);
    nPtDataPoint.setSunSpecDataType("uint16");
    nPtDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(nPtDataPoint.name(), nPtDataPoint);

    SunSpecDataPoint vSfDataPoint;
    vSfDataPoint.setName("V_SF");
    vSfDataPoint.setLabel("V_SF");
    vSfDataPoint.setDescription("Scale factor for percent VRef.");
    vSfDataPoint.setMandatory(true);
    vSfDataPoint.setSize(1);
    vSfDataPoint.setAddressOffset(9);
    vSfDataPoint.setBlockOffset(7);
    vSfDataPoint.setSunSpecDataType("sunssf");
    vSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vSfDataPoint.name(), vSfDataPoint);

    SunSpecDataPoint deptRefSfDataPoint;
    deptRefSfDataPoint.setName("DeptRef_SF");
    deptRefSfDataPoint.setLabel("DeptRef_SF");
    deptRefSfDataPoint.setDescription("Scale Factor for % DeptRef");
    deptRefSfDataPoint.setMandatory(true);
    deptRefSfDataPoint.setSize(1);
    deptRefSfDataPoint.setAddressOffset(10);
    deptRefSfDataPoint.setBlockOffset(8);
    deptRefSfDataPoint.setSunSpecDataType("sunssf");
    deptRefSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(deptRefSfDataPoint.name(), deptRefSfDataPoint);

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

void SunSpecVoltWattModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("V_SF").isValid())
        m_vSf = m_dataPoints.value("V_SF").toInt16();

    if (m_dataPoints.value("DeptRef_SF").isValid())
        m_deptRefSf = m_dataPoints.value("DeptRef_SF").toInt16();

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

    if (m_dataPoints.value("V_SF").isValid())
        m_vSf = m_dataPoints.value("V_SF").toInt16();

    if (m_dataPoints.value("DeptRef_SF").isValid())
        m_deptRefSf = m_dataPoints.value("DeptRef_SF").toInt16();

    if (m_dataPoints.value("RmpIncDec_SF").isValid())
        m_rmpIncDecSf = m_dataPoints.value("RmpIncDec_SF").toInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecVoltWattModel *model)
{
    debug.nospace().noquote() << "SunSpecVoltWattModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
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
