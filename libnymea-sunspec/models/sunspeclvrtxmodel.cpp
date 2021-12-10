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

#include "sunspeclvrtxmodel.h"
#include "sunspecconnection.h"

SunSpecLvrtxModelRepeatingBlock::SunSpecLvrtxModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecLvrtxModel *parent) :
    SunSpecModelRepeatingBlock(blockIndex, blockSize, modbusStartRegister, parent)
{
    m_byteOrder = parent->byteOrder();
    initDataPoints();
}

QString SunSpecLvrtxModelRepeatingBlock::name() const
{
    return "curve";
}

SunSpecLvrtxModel *SunSpecLvrtxModelRepeatingBlock::parentModel() const
{
    return m_parentModel;
}

quint16 SunSpecLvrtxModelRepeatingBlock::actPt() const
{
    return m_actPt;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setActPt(quint16 actPt)
{
    SunSpecDataPoint dp = m_dataPoints.value("ActPt");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(actPt);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::tms1() const
{
    return m_tms1;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setTms1(float tms1)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms1");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms1, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::v1() const
{
    return m_v1;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setV1(float v1)
{
    SunSpecDataPoint dp = m_dataPoints.value("V1");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v1, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::tms2() const
{
    return m_tms2;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setTms2(float tms2)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms2");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms2, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::v2() const
{
    return m_v2;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setV2(float v2)
{
    SunSpecDataPoint dp = m_dataPoints.value("V2");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v2, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::tms3() const
{
    return m_tms3;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setTms3(float tms3)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms3");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms3, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::v3() const
{
    return m_v3;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setV3(float v3)
{
    SunSpecDataPoint dp = m_dataPoints.value("V3");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v3, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::tms4() const
{
    return m_tms4;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setTms4(float tms4)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms4");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms4, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::v4() const
{
    return m_v4;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setV4(float v4)
{
    SunSpecDataPoint dp = m_dataPoints.value("V4");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v4, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::tms5() const
{
    return m_tms5;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setTms5(float tms5)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms5");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms5, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::v5() const
{
    return m_v5;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setV5(float v5)
{
    SunSpecDataPoint dp = m_dataPoints.value("V5");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v5, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::tms6() const
{
    return m_tms6;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setTms6(float tms6)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms6");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms6, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::v6() const
{
    return m_v6;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setV6(float v6)
{
    SunSpecDataPoint dp = m_dataPoints.value("V6");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v6, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::tms7() const
{
    return m_tms7;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setTms7(float tms7)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms7");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms7, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::v7() const
{
    return m_v7;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setV7(float v7)
{
    SunSpecDataPoint dp = m_dataPoints.value("V7");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v7, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::tms8() const
{
    return m_tms8;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setTms8(float tms8)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms8");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms8, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::v8() const
{
    return m_v8;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setV8(float v8)
{
    SunSpecDataPoint dp = m_dataPoints.value("V8");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v8, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::tms9() const
{
    return m_tms9;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setTms9(float tms9)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms9");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms9, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::v9() const
{
    return m_v9;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setV9(float v9)
{
    SunSpecDataPoint dp = m_dataPoints.value("V9");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v9, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::tms10() const
{
    return m_tms10;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setTms10(float tms10)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms10");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms10, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::v10() const
{
    return m_v10;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setV10(float v10)
{
    SunSpecDataPoint dp = m_dataPoints.value("V10");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v10, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::tms11() const
{
    return m_tms11;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setTms11(float tms11)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms11");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms11, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::v11() const
{
    return m_v11;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setV11(float v11)
{
    SunSpecDataPoint dp = m_dataPoints.value("V11");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v11, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::tms12() const
{
    return m_tms12;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setTms12(float tms12)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms12");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms12, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::v12() const
{
    return m_v12;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setV12(float v12)
{
    SunSpecDataPoint dp = m_dataPoints.value("V12");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v12, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::tms13() const
{
    return m_tms13;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setTms13(float tms13)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms13");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms13, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::v13() const
{
    return m_v13;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setV13(float v13)
{
    SunSpecDataPoint dp = m_dataPoints.value("V13");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v13, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::tms14() const
{
    return m_tms14;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setTms14(float tms14)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms14");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms14, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::v14() const
{
    return m_v14;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setV14(float v14)
{
    SunSpecDataPoint dp = m_dataPoints.value("V14");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v14, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::tms15() const
{
    return m_tms15;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setTms15(float tms15)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms15");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms15, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::v15() const
{
    return m_v15;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setV15(float v15)
{
    SunSpecDataPoint dp = m_dataPoints.value("V15");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v15, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::tms16() const
{
    return m_tms16;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setTms16(float tms16)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms16");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms16, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::v16() const
{
    return m_v16;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setV16(float v16)
{
    SunSpecDataPoint dp = m_dataPoints.value("V16");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v16, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::tms17() const
{
    return m_tms17;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setTms17(float tms17)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms17");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms17, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::v17() const
{
    return m_v17;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setV17(float v17)
{
    SunSpecDataPoint dp = m_dataPoints.value("V17");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v17, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::tms18() const
{
    return m_tms18;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setTms18(float tms18)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms18");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms18, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::v18() const
{
    return m_v18;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setV18(float v18)
{
    SunSpecDataPoint dp = m_dataPoints.value("V18");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v18, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::tms19() const
{
    return m_tms19;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setTms19(float tms19)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms19");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms19, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::v19() const
{
    return m_v19;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setV19(float v19)
{
    SunSpecDataPoint dp = m_dataPoints.value("V19");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v19, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::tms20() const
{
    return m_tms20;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setTms20(float tms20)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms20");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms20, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLvrtxModelRepeatingBlock::v20() const
{
    return m_v20;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setV20(float v20)
{
    SunSpecDataPoint dp = m_dataPoints.value("V20");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(v20, m_parentModel->vSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

QString SunSpecLvrtxModelRepeatingBlock::crvNam() const
{
    return m_crvNam;
}

QModbusReply *SunSpecLvrtxModelRepeatingBlock::setCrvNam(const QString &crvNam)
{
    SunSpecDataPoint dp = m_dataPoints.value("CrvNam");
    QVector<quint16> registers = SunSpecDataPoint::convertFromString(crvNam, dp.size());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

SunSpecLvrtxModelRepeatingBlock::Readonly SunSpecLvrtxModelRepeatingBlock::readOnly() const
{
    return m_readOnly;
}

void SunSpecLvrtxModelRepeatingBlock::initDataPoints()
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

    SunSpecDataPoint tms1DataPoint;
    tms1DataPoint.setName("Tms1");
    tms1DataPoint.setLabel("Tms1");
    tms1DataPoint.setDescription("Point 1 duration.");
    tms1DataPoint.setUnits("Secs");
    tms1DataPoint.setMandatory(true);
    tms1DataPoint.setSize(1);
    tms1DataPoint.setAddressOffset(1);
    tms1DataPoint.setScaleFactorName("Tms_SF");
    tms1DataPoint.setSunSpecDataType("uint16");
    tms1DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    tms1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tms1DataPoint.name(), tms1DataPoint);

    SunSpecDataPoint v1DataPoint;
    v1DataPoint.setName("V1");
    v1DataPoint.setLabel("V1");
    v1DataPoint.setDescription("Point 1 voltage.");
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

    SunSpecDataPoint tms2DataPoint;
    tms2DataPoint.setName("Tms2");
    tms2DataPoint.setLabel("Tms2");
    tms2DataPoint.setDescription("Point 2 duration.");
    tms2DataPoint.setUnits("Secs");
    tms2DataPoint.setSize(1);
    tms2DataPoint.setAddressOffset(3);
    tms2DataPoint.setBlockOffset(1);
    tms2DataPoint.setScaleFactorName("Tms_SF");
    tms2DataPoint.setSunSpecDataType("uint16");
    tms2DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    tms2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tms2DataPoint.name(), tms2DataPoint);

    SunSpecDataPoint v2DataPoint;
    v2DataPoint.setName("V2");
    v2DataPoint.setLabel("V2");
    v2DataPoint.setDescription("Point 2 voltage.");
    v2DataPoint.setUnits("% VRef");
    v2DataPoint.setSize(1);
    v2DataPoint.setAddressOffset(4);
    v2DataPoint.setBlockOffset(2);
    v2DataPoint.setScaleFactorName("V_SF");
    v2DataPoint.setSunSpecDataType("uint16");
    v2DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v2DataPoint.name(), v2DataPoint);

    SunSpecDataPoint tms3DataPoint;
    tms3DataPoint.setName("Tms3");
    tms3DataPoint.setLabel("Tms3");
    tms3DataPoint.setDescription("Point 3 duration.");
    tms3DataPoint.setUnits("Secs");
    tms3DataPoint.setSize(1);
    tms3DataPoint.setAddressOffset(5);
    tms3DataPoint.setBlockOffset(3);
    tms3DataPoint.setScaleFactorName("Tms_SF");
    tms3DataPoint.setSunSpecDataType("uint16");
    tms3DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    tms3DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tms3DataPoint.name(), tms3DataPoint);

    SunSpecDataPoint v3DataPoint;
    v3DataPoint.setName("V3");
    v3DataPoint.setLabel("V3");
    v3DataPoint.setDescription("Point 3 voltage.");
    v3DataPoint.setUnits("% VRef");
    v3DataPoint.setSize(1);
    v3DataPoint.setAddressOffset(6);
    v3DataPoint.setBlockOffset(4);
    v3DataPoint.setScaleFactorName("V_SF");
    v3DataPoint.setSunSpecDataType("uint16");
    v3DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v3DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v3DataPoint.name(), v3DataPoint);

    SunSpecDataPoint tms4DataPoint;
    tms4DataPoint.setName("Tms4");
    tms4DataPoint.setLabel("Tms4");
    tms4DataPoint.setDescription("Point 4 duration.");
    tms4DataPoint.setUnits("Secs");
    tms4DataPoint.setSize(1);
    tms4DataPoint.setAddressOffset(7);
    tms4DataPoint.setBlockOffset(5);
    tms4DataPoint.setScaleFactorName("Tms_SF");
    tms4DataPoint.setSunSpecDataType("uint16");
    tms4DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    tms4DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tms4DataPoint.name(), tms4DataPoint);

    SunSpecDataPoint v4DataPoint;
    v4DataPoint.setName("V4");
    v4DataPoint.setLabel("V4");
    v4DataPoint.setDescription("Point 4 voltage.");
    v4DataPoint.setUnits("% VRef");
    v4DataPoint.setSize(1);
    v4DataPoint.setAddressOffset(8);
    v4DataPoint.setBlockOffset(6);
    v4DataPoint.setScaleFactorName("V_SF");
    v4DataPoint.setSunSpecDataType("uint16");
    v4DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v4DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v4DataPoint.name(), v4DataPoint);

    SunSpecDataPoint tms5DataPoint;
    tms5DataPoint.setName("Tms5");
    tms5DataPoint.setLabel("Tms5");
    tms5DataPoint.setDescription("Point 5 duration.");
    tms5DataPoint.setUnits("Secs");
    tms5DataPoint.setSize(1);
    tms5DataPoint.setAddressOffset(9);
    tms5DataPoint.setBlockOffset(7);
    tms5DataPoint.setScaleFactorName("Tms_SF");
    tms5DataPoint.setSunSpecDataType("uint16");
    tms5DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    tms5DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tms5DataPoint.name(), tms5DataPoint);

    SunSpecDataPoint v5DataPoint;
    v5DataPoint.setName("V5");
    v5DataPoint.setLabel("V5");
    v5DataPoint.setDescription("Point 5 voltage.");
    v5DataPoint.setUnits("% VRef");
    v5DataPoint.setSize(1);
    v5DataPoint.setAddressOffset(10);
    v5DataPoint.setBlockOffset(8);
    v5DataPoint.setScaleFactorName("V_SF");
    v5DataPoint.setSunSpecDataType("uint16");
    v5DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v5DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v5DataPoint.name(), v5DataPoint);

    SunSpecDataPoint tms6DataPoint;
    tms6DataPoint.setName("Tms6");
    tms6DataPoint.setLabel("Tms6");
    tms6DataPoint.setDescription("Point 6 duration.");
    tms6DataPoint.setUnits("Secs");
    tms6DataPoint.setSize(1);
    tms6DataPoint.setAddressOffset(11);
    tms6DataPoint.setBlockOffset(9);
    tms6DataPoint.setScaleFactorName("Tms_SF");
    tms6DataPoint.setSunSpecDataType("uint16");
    tms6DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    tms6DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tms6DataPoint.name(), tms6DataPoint);

    SunSpecDataPoint v6DataPoint;
    v6DataPoint.setName("V6");
    v6DataPoint.setLabel("V6");
    v6DataPoint.setDescription("Point 6 voltage.");
    v6DataPoint.setUnits("% VRef");
    v6DataPoint.setSize(1);
    v6DataPoint.setAddressOffset(12);
    v6DataPoint.setBlockOffset(10);
    v6DataPoint.setScaleFactorName("V_SF");
    v6DataPoint.setSunSpecDataType("uint16");
    v6DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v6DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v6DataPoint.name(), v6DataPoint);

    SunSpecDataPoint tms7DataPoint;
    tms7DataPoint.setName("Tms7");
    tms7DataPoint.setLabel("Tms7");
    tms7DataPoint.setDescription("Point 7 duration.");
    tms7DataPoint.setUnits("Secs");
    tms7DataPoint.setSize(1);
    tms7DataPoint.setAddressOffset(13);
    tms7DataPoint.setBlockOffset(11);
    tms7DataPoint.setScaleFactorName("Tms_SF");
    tms7DataPoint.setSunSpecDataType("uint16");
    tms7DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    tms7DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tms7DataPoint.name(), tms7DataPoint);

    SunSpecDataPoint v7DataPoint;
    v7DataPoint.setName("V7");
    v7DataPoint.setLabel("V7");
    v7DataPoint.setDescription("Point 7 voltage.");
    v7DataPoint.setUnits("% VRef");
    v7DataPoint.setSize(1);
    v7DataPoint.setAddressOffset(14);
    v7DataPoint.setBlockOffset(12);
    v7DataPoint.setScaleFactorName("V_SF");
    v7DataPoint.setSunSpecDataType("uint16");
    v7DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v7DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v7DataPoint.name(), v7DataPoint);

    SunSpecDataPoint tms8DataPoint;
    tms8DataPoint.setName("Tms8");
    tms8DataPoint.setLabel("Tms8");
    tms8DataPoint.setDescription("Point 8 duration.");
    tms8DataPoint.setUnits("Secs");
    tms8DataPoint.setSize(1);
    tms8DataPoint.setAddressOffset(15);
    tms8DataPoint.setBlockOffset(13);
    tms8DataPoint.setScaleFactorName("Tms_SF");
    tms8DataPoint.setSunSpecDataType("uint16");
    tms8DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    tms8DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tms8DataPoint.name(), tms8DataPoint);

    SunSpecDataPoint v8DataPoint;
    v8DataPoint.setName("V8");
    v8DataPoint.setLabel("V8");
    v8DataPoint.setDescription("Point 8 voltage.");
    v8DataPoint.setUnits("% VRef");
    v8DataPoint.setSize(1);
    v8DataPoint.setAddressOffset(16);
    v8DataPoint.setBlockOffset(14);
    v8DataPoint.setScaleFactorName("V_SF");
    v8DataPoint.setSunSpecDataType("uint16");
    v8DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v8DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v8DataPoint.name(), v8DataPoint);

    SunSpecDataPoint tms9DataPoint;
    tms9DataPoint.setName("Tms9");
    tms9DataPoint.setLabel("Tms9");
    tms9DataPoint.setDescription("Point 9 duration.");
    tms9DataPoint.setUnits("Secs");
    tms9DataPoint.setSize(1);
    tms9DataPoint.setAddressOffset(17);
    tms9DataPoint.setBlockOffset(15);
    tms9DataPoint.setScaleFactorName("Tms_SF");
    tms9DataPoint.setSunSpecDataType("uint16");
    tms9DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    tms9DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tms9DataPoint.name(), tms9DataPoint);

    SunSpecDataPoint v9DataPoint;
    v9DataPoint.setName("V9");
    v9DataPoint.setLabel("V9");
    v9DataPoint.setDescription("Point 9 voltage.");
    v9DataPoint.setUnits("% VRef");
    v9DataPoint.setSize(1);
    v9DataPoint.setAddressOffset(18);
    v9DataPoint.setBlockOffset(16);
    v9DataPoint.setScaleFactorName("V_SF");
    v9DataPoint.setSunSpecDataType("uint16");
    v9DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v9DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v9DataPoint.name(), v9DataPoint);

    SunSpecDataPoint tms10DataPoint;
    tms10DataPoint.setName("Tms10");
    tms10DataPoint.setLabel("Tms10");
    tms10DataPoint.setDescription("Point 10 duration.");
    tms10DataPoint.setUnits("Secs");
    tms10DataPoint.setSize(1);
    tms10DataPoint.setAddressOffset(19);
    tms10DataPoint.setBlockOffset(17);
    tms10DataPoint.setScaleFactorName("Tms_SF");
    tms10DataPoint.setSunSpecDataType("uint16");
    tms10DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    tms10DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tms10DataPoint.name(), tms10DataPoint);

    SunSpecDataPoint v10DataPoint;
    v10DataPoint.setName("V10");
    v10DataPoint.setLabel("V10");
    v10DataPoint.setDescription("Point 10 voltage.");
    v10DataPoint.setUnits("% VRef");
    v10DataPoint.setSize(1);
    v10DataPoint.setAddressOffset(20);
    v10DataPoint.setBlockOffset(18);
    v10DataPoint.setScaleFactorName("V_SF");
    v10DataPoint.setSunSpecDataType("uint16");
    v10DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v10DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v10DataPoint.name(), v10DataPoint);

    SunSpecDataPoint tms11DataPoint;
    tms11DataPoint.setName("Tms11");
    tms11DataPoint.setLabel("Tms11");
    tms11DataPoint.setDescription("Point 11 duration.");
    tms11DataPoint.setUnits("Secs");
    tms11DataPoint.setSize(1);
    tms11DataPoint.setAddressOffset(21);
    tms11DataPoint.setBlockOffset(19);
    tms11DataPoint.setScaleFactorName("Tms_SF");
    tms11DataPoint.setSunSpecDataType("uint16");
    tms11DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    tms11DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tms11DataPoint.name(), tms11DataPoint);

    SunSpecDataPoint v11DataPoint;
    v11DataPoint.setName("V11");
    v11DataPoint.setLabel("V11");
    v11DataPoint.setDescription("Point 11 voltage.");
    v11DataPoint.setUnits("% VRef");
    v11DataPoint.setSize(1);
    v11DataPoint.setAddressOffset(22);
    v11DataPoint.setBlockOffset(20);
    v11DataPoint.setScaleFactorName("V_SF");
    v11DataPoint.setSunSpecDataType("uint16");
    v11DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v11DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v11DataPoint.name(), v11DataPoint);

    SunSpecDataPoint tms12DataPoint;
    tms12DataPoint.setName("Tms12");
    tms12DataPoint.setLabel("Tms12");
    tms12DataPoint.setDescription("Point 12 duration.");
    tms12DataPoint.setUnits("Secs");
    tms12DataPoint.setSize(1);
    tms12DataPoint.setAddressOffset(23);
    tms12DataPoint.setBlockOffset(21);
    tms12DataPoint.setScaleFactorName("Tms_SF");
    tms12DataPoint.setSunSpecDataType("uint16");
    tms12DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    tms12DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tms12DataPoint.name(), tms12DataPoint);

    SunSpecDataPoint v12DataPoint;
    v12DataPoint.setName("V12");
    v12DataPoint.setLabel("V12");
    v12DataPoint.setDescription("Point 12 voltage.");
    v12DataPoint.setUnits("% VRef");
    v12DataPoint.setSize(1);
    v12DataPoint.setAddressOffset(24);
    v12DataPoint.setBlockOffset(22);
    v12DataPoint.setScaleFactorName("V_SF");
    v12DataPoint.setSunSpecDataType("uint16");
    v12DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v12DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v12DataPoint.name(), v12DataPoint);

    SunSpecDataPoint tms13DataPoint;
    tms13DataPoint.setName("Tms13");
    tms13DataPoint.setLabel("Tms13");
    tms13DataPoint.setDescription("Point 13 duration.");
    tms13DataPoint.setUnits("Secs");
    tms13DataPoint.setSize(1);
    tms13DataPoint.setAddressOffset(25);
    tms13DataPoint.setBlockOffset(23);
    tms13DataPoint.setScaleFactorName("Tms_SF");
    tms13DataPoint.setSunSpecDataType("uint16");
    tms13DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    tms13DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tms13DataPoint.name(), tms13DataPoint);

    SunSpecDataPoint v13DataPoint;
    v13DataPoint.setName("V13");
    v13DataPoint.setLabel("V13");
    v13DataPoint.setDescription("Point 13 voltage.");
    v13DataPoint.setUnits("% VRef");
    v13DataPoint.setSize(1);
    v13DataPoint.setAddressOffset(26);
    v13DataPoint.setBlockOffset(24);
    v13DataPoint.setScaleFactorName("V_SF");
    v13DataPoint.setSunSpecDataType("uint16");
    v13DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v13DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v13DataPoint.name(), v13DataPoint);

    SunSpecDataPoint tms14DataPoint;
    tms14DataPoint.setName("Tms14");
    tms14DataPoint.setLabel("Tms14");
    tms14DataPoint.setDescription("Point 14 duration.");
    tms14DataPoint.setUnits("Secs");
    tms14DataPoint.setSize(1);
    tms14DataPoint.setAddressOffset(27);
    tms14DataPoint.setBlockOffset(25);
    tms14DataPoint.setScaleFactorName("Tms_SF");
    tms14DataPoint.setSunSpecDataType("uint16");
    tms14DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    tms14DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tms14DataPoint.name(), tms14DataPoint);

    SunSpecDataPoint v14DataPoint;
    v14DataPoint.setName("V14");
    v14DataPoint.setLabel("V14");
    v14DataPoint.setDescription("Point 14 voltage.");
    v14DataPoint.setUnits("% VRef");
    v14DataPoint.setSize(1);
    v14DataPoint.setAddressOffset(28);
    v14DataPoint.setBlockOffset(26);
    v14DataPoint.setScaleFactorName("V_SF");
    v14DataPoint.setSunSpecDataType("uint16");
    v14DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v14DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v14DataPoint.name(), v14DataPoint);

    SunSpecDataPoint tms15DataPoint;
    tms15DataPoint.setName("Tms15");
    tms15DataPoint.setLabel("Tms15");
    tms15DataPoint.setDescription("Point 15 duration.");
    tms15DataPoint.setUnits("Secs");
    tms15DataPoint.setSize(1);
    tms15DataPoint.setAddressOffset(29);
    tms15DataPoint.setBlockOffset(27);
    tms15DataPoint.setScaleFactorName("Tms_SF");
    tms15DataPoint.setSunSpecDataType("uint16");
    tms15DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    tms15DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tms15DataPoint.name(), tms15DataPoint);

    SunSpecDataPoint v15DataPoint;
    v15DataPoint.setName("V15");
    v15DataPoint.setLabel("V15");
    v15DataPoint.setDescription("Point 15 voltage.");
    v15DataPoint.setUnits("% VRef");
    v15DataPoint.setSize(1);
    v15DataPoint.setAddressOffset(30);
    v15DataPoint.setBlockOffset(28);
    v15DataPoint.setScaleFactorName("V_SF");
    v15DataPoint.setSunSpecDataType("uint16");
    v15DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v15DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v15DataPoint.name(), v15DataPoint);

    SunSpecDataPoint tms16DataPoint;
    tms16DataPoint.setName("Tms16");
    tms16DataPoint.setLabel("Tms16");
    tms16DataPoint.setDescription("Point 16 duration.");
    tms16DataPoint.setUnits("Secs");
    tms16DataPoint.setSize(1);
    tms16DataPoint.setAddressOffset(31);
    tms16DataPoint.setBlockOffset(29);
    tms16DataPoint.setScaleFactorName("Tms_SF");
    tms16DataPoint.setSunSpecDataType("uint16");
    tms16DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    tms16DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tms16DataPoint.name(), tms16DataPoint);

    SunSpecDataPoint v16DataPoint;
    v16DataPoint.setName("V16");
    v16DataPoint.setLabel("V16");
    v16DataPoint.setDescription("Point 16 voltage.");
    v16DataPoint.setUnits("% VRef");
    v16DataPoint.setSize(1);
    v16DataPoint.setAddressOffset(32);
    v16DataPoint.setBlockOffset(30);
    v16DataPoint.setScaleFactorName("V_SF");
    v16DataPoint.setSunSpecDataType("uint16");
    v16DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v16DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v16DataPoint.name(), v16DataPoint);

    SunSpecDataPoint tms17DataPoint;
    tms17DataPoint.setName("Tms17");
    tms17DataPoint.setLabel("Tms17");
    tms17DataPoint.setDescription("Point 17 duration.");
    tms17DataPoint.setUnits("Secs");
    tms17DataPoint.setSize(1);
    tms17DataPoint.setAddressOffset(33);
    tms17DataPoint.setBlockOffset(31);
    tms17DataPoint.setScaleFactorName("Tms_SF");
    tms17DataPoint.setSunSpecDataType("uint16");
    tms17DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    tms17DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tms17DataPoint.name(), tms17DataPoint);

    SunSpecDataPoint v17DataPoint;
    v17DataPoint.setName("V17");
    v17DataPoint.setLabel("V17");
    v17DataPoint.setDescription("Point 17 voltage.");
    v17DataPoint.setUnits("% VRef");
    v17DataPoint.setSize(1);
    v17DataPoint.setAddressOffset(34);
    v17DataPoint.setBlockOffset(32);
    v17DataPoint.setScaleFactorName("V_SF");
    v17DataPoint.setSunSpecDataType("uint16");
    v17DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v17DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v17DataPoint.name(), v17DataPoint);

    SunSpecDataPoint tms18DataPoint;
    tms18DataPoint.setName("Tms18");
    tms18DataPoint.setLabel("Tms18");
    tms18DataPoint.setDescription("Point 18 duration.");
    tms18DataPoint.setUnits("Secs");
    tms18DataPoint.setSize(1);
    tms18DataPoint.setAddressOffset(35);
    tms18DataPoint.setBlockOffset(33);
    tms18DataPoint.setScaleFactorName("Tms_SF");
    tms18DataPoint.setSunSpecDataType("uint16");
    tms18DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    tms18DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tms18DataPoint.name(), tms18DataPoint);

    SunSpecDataPoint v18DataPoint;
    v18DataPoint.setName("V18");
    v18DataPoint.setLabel("V18");
    v18DataPoint.setDescription("Point 18 voltage.");
    v18DataPoint.setUnits("% VRef");
    v18DataPoint.setSize(1);
    v18DataPoint.setAddressOffset(36);
    v18DataPoint.setBlockOffset(34);
    v18DataPoint.setScaleFactorName("V_SF");
    v18DataPoint.setSunSpecDataType("uint16");
    v18DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v18DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v18DataPoint.name(), v18DataPoint);

    SunSpecDataPoint tms19DataPoint;
    tms19DataPoint.setName("Tms19");
    tms19DataPoint.setLabel("Tms19");
    tms19DataPoint.setDescription("Point 19 duration.");
    tms19DataPoint.setUnits("Secs");
    tms19DataPoint.setSize(1);
    tms19DataPoint.setAddressOffset(37);
    tms19DataPoint.setBlockOffset(35);
    tms19DataPoint.setScaleFactorName("Tms_SF");
    tms19DataPoint.setSunSpecDataType("uint16");
    tms19DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    tms19DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tms19DataPoint.name(), tms19DataPoint);

    SunSpecDataPoint v19DataPoint;
    v19DataPoint.setName("V19");
    v19DataPoint.setLabel("V19");
    v19DataPoint.setDescription("Point 19 voltage.");
    v19DataPoint.setUnits("% VRef");
    v19DataPoint.setSize(1);
    v19DataPoint.setAddressOffset(38);
    v19DataPoint.setBlockOffset(36);
    v19DataPoint.setScaleFactorName("V_SF");
    v19DataPoint.setSunSpecDataType("uint16");
    v19DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v19DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v19DataPoint.name(), v19DataPoint);

    SunSpecDataPoint tms20DataPoint;
    tms20DataPoint.setName("Tms20");
    tms20DataPoint.setLabel("Tms20");
    tms20DataPoint.setDescription("Point 20 duration.");
    tms20DataPoint.setUnits("Secs");
    tms20DataPoint.setSize(1);
    tms20DataPoint.setAddressOffset(39);
    tms20DataPoint.setBlockOffset(37);
    tms20DataPoint.setScaleFactorName("Tms_SF");
    tms20DataPoint.setSunSpecDataType("uint16");
    tms20DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    tms20DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tms20DataPoint.name(), tms20DataPoint);

    SunSpecDataPoint v20DataPoint;
    v20DataPoint.setName("V20");
    v20DataPoint.setLabel("V20");
    v20DataPoint.setDescription("Point 20 voltage.");
    v20DataPoint.setUnits("% VRef");
    v20DataPoint.setSize(1);
    v20DataPoint.setAddressOffset(40);
    v20DataPoint.setBlockOffset(38);
    v20DataPoint.setScaleFactorName("V_SF");
    v20DataPoint.setSunSpecDataType("uint16");
    v20DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    v20DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v20DataPoint.name(), v20DataPoint);

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

    SunSpecDataPoint readOnlyDataPoint;
    readOnlyDataPoint.setName("ReadOnly");
    readOnlyDataPoint.setLabel("ReadOnly");
    readOnlyDataPoint.setDescription("Enumerated value indicates if curve is read-only or can be modified.");
    readOnlyDataPoint.setMandatory(true);
    readOnlyDataPoint.setSize(1);
    readOnlyDataPoint.setAddressOffset(49);
    readOnlyDataPoint.setBlockOffset(47);
    readOnlyDataPoint.setSunSpecDataType("enum16");
    readOnlyDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(readOnlyDataPoint.name(), readOnlyDataPoint);

}

void SunSpecLvrtxModelRepeatingBlock::processBlockData(const QVector<quint16> blockData)
{
    m_blockData = blockData;

    // Update properties according to the data point type
    if (m_dataPoints.value("ActPt").isValid())
        m_actPt = m_dataPoints.value("ActPt").toUInt16();

    if (m_dataPoints.value("Tms1").isValid())
        m_tms1 = m_dataPoints.value("Tms1").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("V1").isValid())
        m_v1 = m_dataPoints.value("V1").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("Tms2").isValid())
        m_tms2 = m_dataPoints.value("Tms2").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("V2").isValid())
        m_v2 = m_dataPoints.value("V2").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("Tms3").isValid())
        m_tms3 = m_dataPoints.value("Tms3").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("V3").isValid())
        m_v3 = m_dataPoints.value("V3").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("Tms4").isValid())
        m_tms4 = m_dataPoints.value("Tms4").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("V4").isValid())
        m_v4 = m_dataPoints.value("V4").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("Tms5").isValid())
        m_tms5 = m_dataPoints.value("Tms5").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("V5").isValid())
        m_v5 = m_dataPoints.value("V5").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("Tms6").isValid())
        m_tms6 = m_dataPoints.value("Tms6").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("V6").isValid())
        m_v6 = m_dataPoints.value("V6").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("Tms7").isValid())
        m_tms7 = m_dataPoints.value("Tms7").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("V7").isValid())
        m_v7 = m_dataPoints.value("V7").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("Tms8").isValid())
        m_tms8 = m_dataPoints.value("Tms8").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("V8").isValid())
        m_v8 = m_dataPoints.value("V8").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("Tms9").isValid())
        m_tms9 = m_dataPoints.value("Tms9").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("V9").isValid())
        m_v9 = m_dataPoints.value("V9").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("Tms10").isValid())
        m_tms10 = m_dataPoints.value("Tms10").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("V10").isValid())
        m_v10 = m_dataPoints.value("V10").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("Tms11").isValid())
        m_tms11 = m_dataPoints.value("Tms11").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("V11").isValid())
        m_v11 = m_dataPoints.value("V11").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("Tms12").isValid())
        m_tms12 = m_dataPoints.value("Tms12").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("V12").isValid())
        m_v12 = m_dataPoints.value("V12").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("Tms13").isValid())
        m_tms13 = m_dataPoints.value("Tms13").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("V13").isValid())
        m_v13 = m_dataPoints.value("V13").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("Tms14").isValid())
        m_tms14 = m_dataPoints.value("Tms14").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("V14").isValid())
        m_v14 = m_dataPoints.value("V14").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("Tms15").isValid())
        m_tms15 = m_dataPoints.value("Tms15").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("V15").isValid())
        m_v15 = m_dataPoints.value("V15").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("Tms16").isValid())
        m_tms16 = m_dataPoints.value("Tms16").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("V16").isValid())
        m_v16 = m_dataPoints.value("V16").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("Tms17").isValid())
        m_tms17 = m_dataPoints.value("Tms17").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("V17").isValid())
        m_v17 = m_dataPoints.value("V17").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("Tms18").isValid())
        m_tms18 = m_dataPoints.value("Tms18").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("V18").isValid())
        m_v18 = m_dataPoints.value("V18").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("Tms19").isValid())
        m_tms19 = m_dataPoints.value("Tms19").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("V19").isValid())
        m_v19 = m_dataPoints.value("V19").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("Tms20").isValid())
        m_tms20 = m_dataPoints.value("Tms20").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("V20").isValid())
        m_v20 = m_dataPoints.value("V20").toFloatWithSSF(m_parentModel->vSf());

    if (m_dataPoints.value("CrvNam").isValid())
        m_crvNam = m_dataPoints.value("CrvNam").toString();

    if (m_dataPoints.value("ReadOnly").isValid())
        m_readOnly = static_cast<Readonly>(m_dataPoints.value("ReadOnly").toUInt16());


    qCDebug(dcSunSpecModelData()) << this;
}


SunSpecLvrtxModel::SunSpecLvrtxModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 139, modelLength, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixedAndRepeating;

    initDataPoints();
}

SunSpecLvrtxModel::~SunSpecLvrtxModel()
{

}

QString SunSpecLvrtxModel::name() const
{
    return "lvrtx";
}

QString SunSpecLvrtxModel::description() const
{
    return "LVRT extended curve";
}

QString SunSpecLvrtxModel::label() const
{
    return "LVRTX";
}

quint16 SunSpecLvrtxModel::actCrv() const
{
    return m_actCrv;
}

QModbusReply *SunSpecLvrtxModel::setActCrv(quint16 actCrv)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("ActCrv");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(actCrv);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecLvrtxModel::ModenaFlags SunSpecLvrtxModel::modEna() const
{
    return m_modEna;
}

QModbusReply *SunSpecLvrtxModel::setModEna(ModenaFlags modEna)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("ModEna");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(modEna));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecLvrtxModel::winTms() const
{
    return m_winTms;
}

QModbusReply *SunSpecLvrtxModel::setWinTms(quint16 winTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("WinTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(winTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecLvrtxModel::rvrtTms() const
{
    return m_rvrtTms;
}

QModbusReply *SunSpecLvrtxModel::setRvrtTms(quint16 rvrtTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("RvrtTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(rvrtTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecLvrtxModel::rmpTms() const
{
    return m_rmpTms;
}

QModbusReply *SunSpecLvrtxModel::setRmpTms(quint16 rmpTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("RmpTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(rmpTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecLvrtxModel::nCrv() const
{
    return m_nCrv;
}
quint16 SunSpecLvrtxModel::nPt() const
{
    return m_nPt;
}
qint16 SunSpecLvrtxModel::tmsSf() const
{
    return m_tmsSf;
}
qint16 SunSpecLvrtxModel::vSf() const
{
    return m_vSf;
}
SunSpecLvrtxModel::Crvtype SunSpecLvrtxModel::crvType() const
{
    return m_crvType;
}
void SunSpecLvrtxModel::initDataPoints()
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
    modEnaDataPoint.setDescription("LVRT control mode. Enable active curve.  Bitfield value.");
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
    winTmsDataPoint.setDescription("Time window for LVRT change.");
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
    rvrtTmsDataPoint.setDescription("Timeout period for LVRT curve selection.");
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
    nPtDataPoint.setDescription("Number of curve points supported (maximum of 20).");
    nPtDataPoint.setMandatory(true);
    nPtDataPoint.setSize(1);
    nPtDataPoint.setAddressOffset(8);
    nPtDataPoint.setBlockOffset(6);
    nPtDataPoint.setSunSpecDataType("uint16");
    nPtDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(nPtDataPoint.name(), nPtDataPoint);

    SunSpecDataPoint tmsSfDataPoint;
    tmsSfDataPoint.setName("Tms_SF");
    tmsSfDataPoint.setLabel("Tms_SF");
    tmsSfDataPoint.setDescription("Scale factor for duration.");
    tmsSfDataPoint.setMandatory(true);
    tmsSfDataPoint.setSize(1);
    tmsSfDataPoint.setAddressOffset(9);
    tmsSfDataPoint.setBlockOffset(7);
    tmsSfDataPoint.setSunSpecDataType("sunssf");
    tmsSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tmsSfDataPoint.name(), tmsSfDataPoint);

    SunSpecDataPoint vSfDataPoint;
    vSfDataPoint.setName("V_SF");
    vSfDataPoint.setLabel("V_SF");
    vSfDataPoint.setDescription("Scale factor for percent VRef.");
    vSfDataPoint.setMandatory(true);
    vSfDataPoint.setSize(1);
    vSfDataPoint.setAddressOffset(10);
    vSfDataPoint.setBlockOffset(8);
    vSfDataPoint.setSunSpecDataType("sunssf");
    vSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vSfDataPoint.name(), vSfDataPoint);

    SunSpecDataPoint crvTypeDataPoint;
    crvTypeDataPoint.setName("CrvType");
    crvTypeDataPoint.setMandatory(true);
    crvTypeDataPoint.setSize(1);
    crvTypeDataPoint.setAddressOffset(11);
    crvTypeDataPoint.setBlockOffset(9);
    crvTypeDataPoint.setSunSpecDataType("enum16");
    crvTypeDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(crvTypeDataPoint.name(), crvTypeDataPoint);

}

void SunSpecLvrtxModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("Tms_SF").isValid())
        m_tmsSf = m_dataPoints.value("Tms_SF").toInt16();

    if (m_dataPoints.value("V_SF").isValid())
        m_vSf = m_dataPoints.value("V_SF").toInt16();


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

    if (m_dataPoints.value("Tms_SF").isValid())
        m_tmsSf = m_dataPoints.value("Tms_SF").toInt16();

    if (m_dataPoints.value("V_SF").isValid())
        m_vSf = m_dataPoints.value("V_SF").toInt16();

    if (m_dataPoints.value("CrvType").isValid())
        m_crvType = static_cast<Crvtype>(m_dataPoints.value("CrvType").toUInt16());


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecLvrtxModel *model)
{
    debug.nospace().noquote() << "SunSpecLvrtxModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
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

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CrvType") << "-->";
    if (model->dataPoints().value("CrvType").isValid()) {
        debug.nospace().noquote() << model->crvType() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
