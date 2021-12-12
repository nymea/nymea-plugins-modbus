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

#include "sunspecvoltvarmodel.h"
#include "sunspecconnection.h"

SunSpecVoltVarModelRepeatingBlock::SunSpecVoltVarModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecVoltVarModel *parent) :
    SunSpecModelRepeatingBlock(blockIndex, blockSize, modbusStartRegister, parent)
{
    m_byteOrder = parent->byteOrder();
    initDataPoints();
}

QString SunSpecVoltVarModelRepeatingBlock::name() const
{
    return "curve";
}

SunSpecVoltVarModel *SunSpecVoltVarModelRepeatingBlock::parentModel() const
{
    return m_parentModel;
}

quint16 SunSpecVoltVarModelRepeatingBlock::actPt() const
{
    return m_actPt;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setActPt(quint16 actPt)
{
    SunSpecDataPoint dp = m_dataPoints.value("ActPt");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(actPt);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

SunSpecVoltVarModelRepeatingBlock::Deptref SunSpecVoltVarModelRepeatingBlock::deptRef() const
{
    return m_deptRef;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setDeptRef(Deptref deptRef)
{
    SunSpecDataPoint dp = m_dataPoints.value("DeptRef");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(deptRef));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::v1() const
{
    return m_v1;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setV1(float v1)
{
    SunSpecDataPoint dp = m_dataPoints.value("V1");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v1, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::vAr1() const
{
    return m_vAr1;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setVAr1(float vAr1)
{
    SunSpecDataPoint dp = m_dataPoints.value("VAr1");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vAr1, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::v2() const
{
    return m_v2;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setV2(float v2)
{
    SunSpecDataPoint dp = m_dataPoints.value("V2");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v2, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::vAr2() const
{
    return m_vAr2;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setVAr2(float vAr2)
{
    SunSpecDataPoint dp = m_dataPoints.value("VAr2");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vAr2, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::v3() const
{
    return m_v3;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setV3(float v3)
{
    SunSpecDataPoint dp = m_dataPoints.value("V3");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v3, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::vAr3() const
{
    return m_vAr3;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setVAr3(float vAr3)
{
    SunSpecDataPoint dp = m_dataPoints.value("VAr3");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vAr3, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::v4() const
{
    return m_v4;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setV4(float v4)
{
    SunSpecDataPoint dp = m_dataPoints.value("V4");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v4, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::vAr4() const
{
    return m_vAr4;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setVAr4(float vAr4)
{
    SunSpecDataPoint dp = m_dataPoints.value("VAr4");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vAr4, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::v5() const
{
    return m_v5;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setV5(float v5)
{
    SunSpecDataPoint dp = m_dataPoints.value("V5");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v5, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::vAr5() const
{
    return m_vAr5;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setVAr5(float vAr5)
{
    SunSpecDataPoint dp = m_dataPoints.value("VAr5");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vAr5, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::v6() const
{
    return m_v6;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setV6(float v6)
{
    SunSpecDataPoint dp = m_dataPoints.value("V6");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v6, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::vAr6() const
{
    return m_vAr6;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setVAr6(float vAr6)
{
    SunSpecDataPoint dp = m_dataPoints.value("VAr6");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vAr6, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::v7() const
{
    return m_v7;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setV7(float v7)
{
    SunSpecDataPoint dp = m_dataPoints.value("V7");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v7, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::vAr7() const
{
    return m_vAr7;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setVAr7(float vAr7)
{
    SunSpecDataPoint dp = m_dataPoints.value("VAr7");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vAr7, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::v8() const
{
    return m_v8;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setV8(float v8)
{
    SunSpecDataPoint dp = m_dataPoints.value("V8");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v8, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::vAr8() const
{
    return m_vAr8;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setVAr8(float vAr8)
{
    SunSpecDataPoint dp = m_dataPoints.value("VAr8");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vAr8, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::v9() const
{
    return m_v9;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setV9(float v9)
{
    SunSpecDataPoint dp = m_dataPoints.value("V9");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v9, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::vAr9() const
{
    return m_vAr9;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setVAr9(float vAr9)
{
    SunSpecDataPoint dp = m_dataPoints.value("VAr9");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vAr9, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::v10() const
{
    return m_v10;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setV10(float v10)
{
    SunSpecDataPoint dp = m_dataPoints.value("V10");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v10, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::vAr10() const
{
    return m_vAr10;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setVAr10(float vAr10)
{
    SunSpecDataPoint dp = m_dataPoints.value("VAr10");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vAr10, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::v11() const
{
    return m_v11;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setV11(float v11)
{
    SunSpecDataPoint dp = m_dataPoints.value("V11");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v11, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::vAr11() const
{
    return m_vAr11;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setVAr11(float vAr11)
{
    SunSpecDataPoint dp = m_dataPoints.value("VAr11");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vAr11, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::v12() const
{
    return m_v12;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setV12(float v12)
{
    SunSpecDataPoint dp = m_dataPoints.value("V12");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v12, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::vAr12() const
{
    return m_vAr12;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setVAr12(float vAr12)
{
    SunSpecDataPoint dp = m_dataPoints.value("VAr12");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vAr12, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::v13() const
{
    return m_v13;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setV13(float v13)
{
    SunSpecDataPoint dp = m_dataPoints.value("V13");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v13, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::vAr13() const
{
    return m_vAr13;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setVAr13(float vAr13)
{
    SunSpecDataPoint dp = m_dataPoints.value("VAr13");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vAr13, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::v14() const
{
    return m_v14;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setV14(float v14)
{
    SunSpecDataPoint dp = m_dataPoints.value("V14");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v14, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::vAr14() const
{
    return m_vAr14;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setVAr14(float vAr14)
{
    SunSpecDataPoint dp = m_dataPoints.value("VAr14");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vAr14, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::v15() const
{
    return m_v15;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setV15(float v15)
{
    SunSpecDataPoint dp = m_dataPoints.value("V15");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v15, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::vAr15() const
{
    return m_vAr15;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setVAr15(float vAr15)
{
    SunSpecDataPoint dp = m_dataPoints.value("VAr15");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vAr15, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::v16() const
{
    return m_v16;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setV16(float v16)
{
    SunSpecDataPoint dp = m_dataPoints.value("V16");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v16, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::vAr16() const
{
    return m_vAr16;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setVAr16(float vAr16)
{
    SunSpecDataPoint dp = m_dataPoints.value("VAr16");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vAr16, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::v17() const
{
    return m_v17;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setV17(float v17)
{
    SunSpecDataPoint dp = m_dataPoints.value("V17");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v17, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::vAr17() const
{
    return m_vAr17;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setVAr17(float vAr17)
{
    SunSpecDataPoint dp = m_dataPoints.value("VAr17");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vAr17, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::v18() const
{
    return m_v18;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setV18(float v18)
{
    SunSpecDataPoint dp = m_dataPoints.value("V18");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v18, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::vAr18() const
{
    return m_vAr18;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setVAr18(float vAr18)
{
    SunSpecDataPoint dp = m_dataPoints.value("VAr18");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vAr18, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::v19() const
{
    return m_v19;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setV19(float v19)
{
    SunSpecDataPoint dp = m_dataPoints.value("V19");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v19, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::vAr19() const
{
    return m_vAr19;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setVAr19(float vAr19)
{
    SunSpecDataPoint dp = m_dataPoints.value("VAr19");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vAr19, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::v20() const
{
    return m_v20;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setV20(float v20)
{
    SunSpecDataPoint dp = m_dataPoints.value("V20");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v20, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::vAr20() const
{
    return m_vAr20;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setVAr20(float vAr20)
{
    SunSpecDataPoint dp = m_dataPoints.value("VAr20");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vAr20, m_parentModel->deptRefSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

QString SunSpecVoltVarModelRepeatingBlock::crvNam() const
{
    return m_crvNam;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setCrvNam(const QString &crvNam)
{
    SunSpecDataPoint dp = m_dataPoints.value("CrvNam");
    QVector<quint16> registers = SunSpecDataPoint::convertFromString(crvNam, dp.size());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

quint16 SunSpecVoltVarModelRepeatingBlock::rmpTms() const
{
    return m_rmpTms;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setRmpTms(quint16 rmpTms)
{
    SunSpecDataPoint dp = m_dataPoints.value("RmpTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(rmpTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::rmpDecTmm() const
{
    return m_rmpDecTmm;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setRmpDecTmm(float rmpDecTmm)
{
    SunSpecDataPoint dp = m_dataPoints.value("RmpDecTmm");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(rmpDecTmm, m_parentModel->rmpIncDec_SF(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecVoltVarModelRepeatingBlock::rmpIncTmm() const
{
    return m_rmpIncTmm;
}

QModbusReply *SunSpecVoltVarModelRepeatingBlock::setRmpIncTmm(float rmpIncTmm)
{
    SunSpecDataPoint dp = m_dataPoints.value("RmpIncTmm");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(rmpIncTmm, m_parentModel->rmpIncDec_SF(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

SunSpecVoltVarModelRepeatingBlock::Readonly SunSpecVoltVarModelRepeatingBlock::readOnly() const
{
    return m_readOnly;
}

void SunSpecVoltVarModelRepeatingBlock::initDataPoints()
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
    deptRefDataPoint.setDescription("Meaning of dependent variable: 1=%WMax 2=%VArMax 3=%VArAval.");
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

    SunSpecDataPoint vAr1DataPoint;
    vAr1DataPoint.setName("VAr1");
    vAr1DataPoint.setLabel("VAr1");
    vAr1DataPoint.setDescription("Point 1 VARs.");
    vAr1DataPoint.setMandatory(true);
    vAr1DataPoint.setSize(1);
    vAr1DataPoint.setAddressOffset(3);
    vAr1DataPoint.setBlockOffset(1);
    vAr1DataPoint.setScaleFactorName("DeptRef_SF");
    vAr1DataPoint.setSunSpecDataType("int16");
    vAr1DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vAr1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vAr1DataPoint.name(), vAr1DataPoint);

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

    SunSpecDataPoint vAr2DataPoint;
    vAr2DataPoint.setName("VAr2");
    vAr2DataPoint.setLabel("VAr2");
    vAr2DataPoint.setDescription("Point 2 VARs.");
    vAr2DataPoint.setSize(1);
    vAr2DataPoint.setAddressOffset(5);
    vAr2DataPoint.setBlockOffset(3);
    vAr2DataPoint.setScaleFactorName("DeptRef_SF");
    vAr2DataPoint.setSunSpecDataType("int16");
    vAr2DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vAr2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vAr2DataPoint.name(), vAr2DataPoint);

    SunSpecDataPoint v3DataPoint;
    v3DataPoint.setName("V3");
    v3DataPoint.setLabel("V3");
    v3DataPoint.setDescription("Point 2 Volts.");
    v3DataPoint.setUnits("% VRef");
    v3DataPoint.setSize(1);
    v3DataPoint.setAddressOffset(6);
    v3DataPoint.setBlockOffset(4);
    v3DataPoint.setScaleFactorName("V_SF");
    v3DataPoint.setSunSpecDataType("uint16");
    v3DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v3DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v3DataPoint.name(), v3DataPoint);

    SunSpecDataPoint vAr3DataPoint;
    vAr3DataPoint.setName("VAr3");
    vAr3DataPoint.setLabel("VAr3");
    vAr3DataPoint.setDescription("Point 3 VARs.");
    vAr3DataPoint.setSize(1);
    vAr3DataPoint.setAddressOffset(7);
    vAr3DataPoint.setBlockOffset(5);
    vAr3DataPoint.setScaleFactorName("DeptRef_SF");
    vAr3DataPoint.setSunSpecDataType("int16");
    vAr3DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vAr3DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vAr3DataPoint.name(), vAr3DataPoint);

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

    SunSpecDataPoint vAr4DataPoint;
    vAr4DataPoint.setName("VAr4");
    vAr4DataPoint.setLabel("VAr4");
    vAr4DataPoint.setDescription("Point 4 VARs.");
    vAr4DataPoint.setSize(1);
    vAr4DataPoint.setAddressOffset(9);
    vAr4DataPoint.setBlockOffset(7);
    vAr4DataPoint.setScaleFactorName("DeptRef_SF");
    vAr4DataPoint.setSunSpecDataType("int16");
    vAr4DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vAr4DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vAr4DataPoint.name(), vAr4DataPoint);

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

    SunSpecDataPoint vAr5DataPoint;
    vAr5DataPoint.setName("VAr5");
    vAr5DataPoint.setLabel("VAr5");
    vAr5DataPoint.setDescription("Point 5 VARs.");
    vAr5DataPoint.setSize(1);
    vAr5DataPoint.setAddressOffset(11);
    vAr5DataPoint.setBlockOffset(9);
    vAr5DataPoint.setScaleFactorName("DeptRef_SF");
    vAr5DataPoint.setSunSpecDataType("int16");
    vAr5DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vAr5DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vAr5DataPoint.name(), vAr5DataPoint);

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

    SunSpecDataPoint vAr6DataPoint;
    vAr6DataPoint.setName("VAr6");
    vAr6DataPoint.setLabel("VAr6");
    vAr6DataPoint.setDescription("Point 6 VARs.");
    vAr6DataPoint.setSize(1);
    vAr6DataPoint.setAddressOffset(13);
    vAr6DataPoint.setBlockOffset(11);
    vAr6DataPoint.setScaleFactorName("DeptRef_SF");
    vAr6DataPoint.setSunSpecDataType("int16");
    vAr6DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vAr6DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vAr6DataPoint.name(), vAr6DataPoint);

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

    SunSpecDataPoint vAr7DataPoint;
    vAr7DataPoint.setName("VAr7");
    vAr7DataPoint.setLabel("VAr7");
    vAr7DataPoint.setDescription("Point 7 VARs.");
    vAr7DataPoint.setSize(1);
    vAr7DataPoint.setAddressOffset(15);
    vAr7DataPoint.setBlockOffset(13);
    vAr7DataPoint.setScaleFactorName("DeptRef_SF");
    vAr7DataPoint.setSunSpecDataType("int16");
    vAr7DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vAr7DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vAr7DataPoint.name(), vAr7DataPoint);

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

    SunSpecDataPoint vAr8DataPoint;
    vAr8DataPoint.setName("VAr8");
    vAr8DataPoint.setLabel("VAr8");
    vAr8DataPoint.setDescription("Point 8 VARs.");
    vAr8DataPoint.setSize(1);
    vAr8DataPoint.setAddressOffset(17);
    vAr8DataPoint.setBlockOffset(15);
    vAr8DataPoint.setScaleFactorName("DeptRef_SF");
    vAr8DataPoint.setSunSpecDataType("int16");
    vAr8DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vAr8DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vAr8DataPoint.name(), vAr8DataPoint);

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

    SunSpecDataPoint vAr9DataPoint;
    vAr9DataPoint.setName("VAr9");
    vAr9DataPoint.setLabel("VAr9");
    vAr9DataPoint.setDescription("Point 9 VARs.");
    vAr9DataPoint.setSize(1);
    vAr9DataPoint.setAddressOffset(19);
    vAr9DataPoint.setBlockOffset(17);
    vAr9DataPoint.setScaleFactorName("DeptRef_SF");
    vAr9DataPoint.setSunSpecDataType("int16");
    vAr9DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vAr9DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vAr9DataPoint.name(), vAr9DataPoint);

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

    SunSpecDataPoint vAr10DataPoint;
    vAr10DataPoint.setName("VAr10");
    vAr10DataPoint.setLabel("VAr10");
    vAr10DataPoint.setDescription("Point 10 VARs.");
    vAr10DataPoint.setSize(1);
    vAr10DataPoint.setAddressOffset(21);
    vAr10DataPoint.setBlockOffset(19);
    vAr10DataPoint.setScaleFactorName("DeptRef_SF");
    vAr10DataPoint.setSunSpecDataType("int16");
    vAr10DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vAr10DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vAr10DataPoint.name(), vAr10DataPoint);

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

    SunSpecDataPoint vAr11DataPoint;
    vAr11DataPoint.setName("VAr11");
    vAr11DataPoint.setLabel("VAr11");
    vAr11DataPoint.setDescription("Point 11 VARs.");
    vAr11DataPoint.setSize(1);
    vAr11DataPoint.setAddressOffset(23);
    vAr11DataPoint.setBlockOffset(21);
    vAr11DataPoint.setScaleFactorName("DeptRef_SF");
    vAr11DataPoint.setSunSpecDataType("int16");
    vAr11DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vAr11DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vAr11DataPoint.name(), vAr11DataPoint);

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

    SunSpecDataPoint vAr12DataPoint;
    vAr12DataPoint.setName("VAr12");
    vAr12DataPoint.setLabel("VAr12");
    vAr12DataPoint.setDescription("Point 12 VARs.");
    vAr12DataPoint.setSize(1);
    vAr12DataPoint.setAddressOffset(25);
    vAr12DataPoint.setBlockOffset(23);
    vAr12DataPoint.setScaleFactorName("DeptRef_SF");
    vAr12DataPoint.setSunSpecDataType("int16");
    vAr12DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vAr12DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vAr12DataPoint.name(), vAr12DataPoint);

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

    SunSpecDataPoint vAr13DataPoint;
    vAr13DataPoint.setName("VAr13");
    vAr13DataPoint.setLabel("VAr13");
    vAr13DataPoint.setDescription("Point 13 VARs.");
    vAr13DataPoint.setSize(1);
    vAr13DataPoint.setAddressOffset(27);
    vAr13DataPoint.setBlockOffset(25);
    vAr13DataPoint.setScaleFactorName("DeptRef_SF");
    vAr13DataPoint.setSunSpecDataType("int16");
    vAr13DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vAr13DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vAr13DataPoint.name(), vAr13DataPoint);

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

    SunSpecDataPoint vAr14DataPoint;
    vAr14DataPoint.setName("VAr14");
    vAr14DataPoint.setLabel("VAr14");
    vAr14DataPoint.setDescription("Point 14 VARs.");
    vAr14DataPoint.setSize(1);
    vAr14DataPoint.setAddressOffset(29);
    vAr14DataPoint.setBlockOffset(27);
    vAr14DataPoint.setScaleFactorName("DeptRef_SF");
    vAr14DataPoint.setSunSpecDataType("int16");
    vAr14DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vAr14DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vAr14DataPoint.name(), vAr14DataPoint);

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

    SunSpecDataPoint vAr15DataPoint;
    vAr15DataPoint.setName("VAr15");
    vAr15DataPoint.setLabel("VAr15");
    vAr15DataPoint.setDescription("Point 15 VARs.");
    vAr15DataPoint.setSize(1);
    vAr15DataPoint.setAddressOffset(31);
    vAr15DataPoint.setBlockOffset(29);
    vAr15DataPoint.setScaleFactorName("DeptRef_SF");
    vAr15DataPoint.setSunSpecDataType("int16");
    vAr15DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vAr15DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vAr15DataPoint.name(), vAr15DataPoint);

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

    SunSpecDataPoint vAr16DataPoint;
    vAr16DataPoint.setName("VAr16");
    vAr16DataPoint.setLabel("VAr16");
    vAr16DataPoint.setDescription("Point 16 VARs.");
    vAr16DataPoint.setSize(1);
    vAr16DataPoint.setAddressOffset(33);
    vAr16DataPoint.setBlockOffset(31);
    vAr16DataPoint.setScaleFactorName("DeptRef_SF");
    vAr16DataPoint.setSunSpecDataType("int16");
    vAr16DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vAr16DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vAr16DataPoint.name(), vAr16DataPoint);

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

    SunSpecDataPoint vAr17DataPoint;
    vAr17DataPoint.setName("VAr17");
    vAr17DataPoint.setLabel("VAr17");
    vAr17DataPoint.setDescription("Point 17 VARs.");
    vAr17DataPoint.setSize(1);
    vAr17DataPoint.setAddressOffset(35);
    vAr17DataPoint.setBlockOffset(33);
    vAr17DataPoint.setScaleFactorName("DeptRef_SF");
    vAr17DataPoint.setSunSpecDataType("int16");
    vAr17DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vAr17DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vAr17DataPoint.name(), vAr17DataPoint);

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

    SunSpecDataPoint vAr18DataPoint;
    vAr18DataPoint.setName("VAr18");
    vAr18DataPoint.setLabel("VAr18");
    vAr18DataPoint.setDescription("Point 18 VARs.");
    vAr18DataPoint.setSize(1);
    vAr18DataPoint.setAddressOffset(37);
    vAr18DataPoint.setBlockOffset(35);
    vAr18DataPoint.setScaleFactorName("DeptRef_SF");
    vAr18DataPoint.setSunSpecDataType("int16");
    vAr18DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vAr18DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vAr18DataPoint.name(), vAr18DataPoint);

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

    SunSpecDataPoint vAr19DataPoint;
    vAr19DataPoint.setName("VAr19");
    vAr19DataPoint.setLabel("VAr19");
    vAr19DataPoint.setDescription("Point 19 VARs.");
    vAr19DataPoint.setSize(1);
    vAr19DataPoint.setAddressOffset(39);
    vAr19DataPoint.setBlockOffset(37);
    vAr19DataPoint.setScaleFactorName("DeptRef_SF");
    vAr19DataPoint.setSunSpecDataType("int16");
    vAr19DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vAr19DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vAr19DataPoint.name(), vAr19DataPoint);

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

    SunSpecDataPoint vAr20DataPoint;
    vAr20DataPoint.setName("VAr20");
    vAr20DataPoint.setLabel("VAr20");
    vAr20DataPoint.setDescription("Point 20 VARs.");
    vAr20DataPoint.setSize(1);
    vAr20DataPoint.setAddressOffset(41);
    vAr20DataPoint.setBlockOffset(39);
    vAr20DataPoint.setScaleFactorName("DeptRef_SF");
    vAr20DataPoint.setSunSpecDataType("int16");
    vAr20DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vAr20DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vAr20DataPoint.name(), vAr20DataPoint);

    SunSpecDataPoint crvNamDataPoint;
    crvNamDataPoint.setName("CrvNam");
    crvNamDataPoint.setLabel("CrvNam");
    crvNamDataPoint.setDescription("Optional description for curve. (Max 16 chars)");
    crvNamDataPoint.setSize(8);
    crvNamDataPoint.setAddressOffset(42);
    crvNamDataPoint.setBlockOffset(40);
    crvNamDataPoint.setSunSpecDataType("string");
    crvNamDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    crvNamDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(crvNamDataPoint.name(), crvNamDataPoint);

    SunSpecDataPoint rmpTmsDataPoint;
    rmpTmsDataPoint.setName("RmpTms");
    rmpTmsDataPoint.setLabel("RmpTms");
    rmpTmsDataPoint.setDescription("The time of the PT1 in seconds (time to accomplish a change of 95%).");
    rmpTmsDataPoint.setUnits("Secs");
    rmpTmsDataPoint.setSize(1);
    rmpTmsDataPoint.setAddressOffset(50);
    rmpTmsDataPoint.setBlockOffset(48);
    rmpTmsDataPoint.setSunSpecDataType("uint16");
    rmpTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    rmpTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(rmpTmsDataPoint.name(), rmpTmsDataPoint);

    SunSpecDataPoint rmpDecTmmDataPoint;
    rmpDecTmmDataPoint.setName("RmpDecTmm");
    rmpDecTmmDataPoint.setLabel("RmpDecTmm");
    rmpDecTmmDataPoint.setDescription("The maximum rate at which the VAR value may be reduced in response to changes in the voltage value. %refVal is %WMax %VArMax or %VArAval depending on value of DeptRef.");
    rmpDecTmmDataPoint.setUnits("% ref_value/min");
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
    rmpIncTmmDataPoint.setDescription("The maximum rate at which the VAR value may be increased in response to changes in the voltage value. %refVal is %WMax %VArMax or %VArAval depending on value of DeptRef.");
    rmpIncTmmDataPoint.setUnits("% ref_value/min");
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
    readOnlyDataPoint.setDescription("Boolean flag indicates if curve is read-only or can be modified.");
    readOnlyDataPoint.setMandatory(true);
    readOnlyDataPoint.setSize(1);
    readOnlyDataPoint.setAddressOffset(53);
    readOnlyDataPoint.setBlockOffset(51);
    readOnlyDataPoint.setSunSpecDataType("enum16");
    readOnlyDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(readOnlyDataPoint.name(), readOnlyDataPoint);

}

void SunSpecVoltVarModelRepeatingBlock::processBlockData(const QVector<quint16> blockData)
{
    m_blockData = blockData;

    // Update properties according to the data point type
    if (m_dataPoints.value("ActPt").isValid())
        m_actPt = m_dataPoints.value("ActPt").toUInt16();

    if (m_dataPoints.value("DeptRef").isValid())
        m_deptRef = static_cast<Deptref>(m_dataPoints.value("DeptRef").toUInt16());

    if (m_dataPoints.value("V1").isValid())
        m_v1 = m_dataPoints.value("V1").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("VAr1").isValid())
        m_vAr1 = m_dataPoints.value("VAr1").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V2").isValid())
        m_v2 = m_dataPoints.value("V2").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("VAr2").isValid())
        m_vAr2 = m_dataPoints.value("VAr2").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V3").isValid())
        m_v3 = m_dataPoints.value("V3").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("VAr3").isValid())
        m_vAr3 = m_dataPoints.value("VAr3").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V4").isValid())
        m_v4 = m_dataPoints.value("V4").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("VAr4").isValid())
        m_vAr4 = m_dataPoints.value("VAr4").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V5").isValid())
        m_v5 = m_dataPoints.value("V5").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("VAr5").isValid())
        m_vAr5 = m_dataPoints.value("VAr5").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V6").isValid())
        m_v6 = m_dataPoints.value("V6").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("VAr6").isValid())
        m_vAr6 = m_dataPoints.value("VAr6").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V7").isValid())
        m_v7 = m_dataPoints.value("V7").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("VAr7").isValid())
        m_vAr7 = m_dataPoints.value("VAr7").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V8").isValid())
        m_v8 = m_dataPoints.value("V8").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("VAr8").isValid())
        m_vAr8 = m_dataPoints.value("VAr8").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V9").isValid())
        m_v9 = m_dataPoints.value("V9").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("VAr9").isValid())
        m_vAr9 = m_dataPoints.value("VAr9").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V10").isValid())
        m_v10 = m_dataPoints.value("V10").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("VAr10").isValid())
        m_vAr10 = m_dataPoints.value("VAr10").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V11").isValid())
        m_v11 = m_dataPoints.value("V11").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("VAr11").isValid())
        m_vAr11 = m_dataPoints.value("VAr11").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V12").isValid())
        m_v12 = m_dataPoints.value("V12").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("VAr12").isValid())
        m_vAr12 = m_dataPoints.value("VAr12").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V13").isValid())
        m_v13 = m_dataPoints.value("V13").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("VAr13").isValid())
        m_vAr13 = m_dataPoints.value("VAr13").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V14").isValid())
        m_v14 = m_dataPoints.value("V14").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("VAr14").isValid())
        m_vAr14 = m_dataPoints.value("VAr14").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V15").isValid())
        m_v15 = m_dataPoints.value("V15").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("VAr15").isValid())
        m_vAr15 = m_dataPoints.value("VAr15").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V16").isValid())
        m_v16 = m_dataPoints.value("V16").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("VAr16").isValid())
        m_vAr16 = m_dataPoints.value("VAr16").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V17").isValid())
        m_v17 = m_dataPoints.value("V17").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("VAr17").isValid())
        m_vAr17 = m_dataPoints.value("VAr17").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V18").isValid())
        m_v18 = m_dataPoints.value("V18").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("VAr18").isValid())
        m_vAr18 = m_dataPoints.value("VAr18").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V19").isValid())
        m_v19 = m_dataPoints.value("V19").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("VAr19").isValid())
        m_vAr19 = m_dataPoints.value("VAr19").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("V20").isValid())
        m_v20 = m_dataPoints.value("V20").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("VAr20").isValid())
        m_vAr20 = m_dataPoints.value("VAr20").toFloatWithSSF(m_parentModel->deptRefSf());

    if (m_dataPoints.value("CrvNam").isValid())
        m_crvNam = m_dataPoints.value("CrvNam").toString();

    if (m_dataPoints.value("RmpTms").isValid())
        m_rmpTms = m_dataPoints.value("RmpTms").toUInt16();

    if (m_dataPoints.value("RmpDecTmm").isValid())
        m_rmpDecTmm = m_dataPoints.value("RmpDecTmm").toFloatWithSSF(m_parentModel->rmpIncDec_SF());

    if (m_dataPoints.value("RmpIncTmm").isValid())
        m_rmpIncTmm = m_dataPoints.value("RmpIncTmm").toFloatWithSSF(m_parentModel->rmpIncDec_SF());

    if (m_dataPoints.value("ReadOnly").isValid())
        m_readOnly = static_cast<Readonly>(m_dataPoints.value("ReadOnly").toUInt16());


    qCDebug(dcSunSpecModelData()) << this;
}


SunSpecVoltVarModel::SunSpecVoltVarModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 126, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixedAndRepeating;

    initDataPoints();
}

SunSpecVoltVarModel::~SunSpecVoltVarModel()
{

}

QString SunSpecVoltVarModel::name() const
{
    return "volt_var";
}

QString SunSpecVoltVarModel::description() const
{
    return "Static Volt-VAR Arrays ";
}

QString SunSpecVoltVarModel::label() const
{
    return "Static Volt-VAR";
}

quint16 SunSpecVoltVarModel::actCrv() const
{
    return m_actCrv;
}

QModbusReply *SunSpecVoltVarModel::setActCrv(quint16 actCrv)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("ActCrv");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(actCrv);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecVoltVarModel::ModenaFlags SunSpecVoltVarModel::modEna() const
{
    return m_modEna;
}

QModbusReply *SunSpecVoltVarModel::setModEna(ModenaFlags modEna)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("ModEna");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(modEna));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecVoltVarModel::winTms() const
{
    return m_winTms;
}

QModbusReply *SunSpecVoltVarModel::setWinTms(quint16 winTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("WinTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(winTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecVoltVarModel::rvrtTms() const
{
    return m_rvrtTms;
}

QModbusReply *SunSpecVoltVarModel::setRvrtTms(quint16 rvrtTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("RvrtTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(rvrtTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecVoltVarModel::rmpTms() const
{
    return m_rmpTms;
}

QModbusReply *SunSpecVoltVarModel::setRmpTms(quint16 rmpTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("RmpTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(rmpTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecVoltVarModel::nCrv() const
{
    return m_nCrv;
}
quint16 SunSpecVoltVarModel::nPt() const
{
    return m_nPt;
}
qint16 SunSpecVoltVarModel::vSf() const
{
    return m_vSf;
}
qint16 SunSpecVoltVarModel::deptRefSf() const
{
    return m_deptRefSf;
}
qint16 SunSpecVoltVarModel::rmpIncDec_SF() const
{
    return m_rmpIncDec_SF;
}
void SunSpecVoltVarModel::initDataPoints()
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
    modEnaDataPoint.setDescription("Is Volt-VAR control active.");
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
    winTmsDataPoint.setDescription("Time window for volt-VAR change.");
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
    rvrtTmsDataPoint.setDescription("Timeout period for volt-VAR curve selection.");
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
    rmpTmsDataPoint.setDescription("The time of the PT1 in seconds (time to accomplish a change of 95%).");
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
    nPtDataPoint.setDescription("Number of curve points supported (maximum of 20).");
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
    deptRefSfDataPoint.setDescription("scale factor for dependent variable.");
    deptRefSfDataPoint.setMandatory(true);
    deptRefSfDataPoint.setSize(1);
    deptRefSfDataPoint.setAddressOffset(10);
    deptRefSfDataPoint.setBlockOffset(8);
    deptRefSfDataPoint.setSunSpecDataType("sunssf");
    deptRefSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(deptRefSfDataPoint.name(), deptRefSfDataPoint);

    SunSpecDataPoint rmpIncDec_SFDataPoint;
    rmpIncDec_SFDataPoint.setName("RmpIncDec_SF");
    rmpIncDec_SFDataPoint.setSize(1);
    rmpIncDec_SFDataPoint.setAddressOffset(11);
    rmpIncDec_SFDataPoint.setBlockOffset(9);
    rmpIncDec_SFDataPoint.setSunSpecDataType("sunssf");
    rmpIncDec_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(rmpIncDec_SFDataPoint.name(), rmpIncDec_SFDataPoint);

}

void SunSpecVoltVarModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("V_SF").isValid())
        m_vSf = m_dataPoints.value("V_SF").toInt16();

    if (m_dataPoints.value("DeptRef_SF").isValid())
        m_deptRefSf = m_dataPoints.value("DeptRef_SF").toInt16();

    if (m_dataPoints.value("RmpIncDec_SF").isValid())
        m_rmpIncDec_SF = m_dataPoints.value("RmpIncDec_SF").toInt16();


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
        m_rmpIncDec_SF = m_dataPoints.value("RmpIncDec_SF").toInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecVoltVarModel *model)
{
    debug.nospace().noquote() << "SunSpecVoltVarModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
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
