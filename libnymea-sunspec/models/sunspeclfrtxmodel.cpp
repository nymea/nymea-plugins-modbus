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

#include "sunspeclfrtxmodel.h"
#include "sunspecconnection.h"

SunSpecLfrtxModelRepeatingBlock::SunSpecLfrtxModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecLfrtxModel *parent) :
    SunSpecModelRepeatingBlock(blockIndex, blockSize, modbusStartRegister, parent)
{
    initDataPoints();
}

QString SunSpecLfrtxModelRepeatingBlock::name() const
{
    return "curve";
}

SunSpecLfrtxModel *SunSpecLfrtxModelRepeatingBlock::parentModel() const
{
    return m_parentModel;
}

quint16 SunSpecLfrtxModelRepeatingBlock::actPt() const
{
    return m_actPt;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setActPt(quint16 actPt)
{
    SunSpecDataPoint dp = m_dataPoints.value("ActPt");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(actPt);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::tms1() const
{
    return m_tms1;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setTms1(float tms1)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms1");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms1, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::hz1() const
{
    return m_hz1;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setHz1(float hz1)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz1");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz1, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::tms2() const
{
    return m_tms2;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setTms2(float tms2)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms2");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms2, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::hz2() const
{
    return m_hz2;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setHz2(float hz2)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz2");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz2, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::tms3() const
{
    return m_tms3;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setTms3(float tms3)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms3");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms3, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::hz3() const
{
    return m_hz3;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setHz3(float hz3)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz3");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz3, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::tms4() const
{
    return m_tms4;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setTms4(float tms4)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms4");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms4, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::hz4() const
{
    return m_hz4;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setHz4(float hz4)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz4");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz4, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::tms5() const
{
    return m_tms5;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setTms5(float tms5)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms5");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms5, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::hz5() const
{
    return m_hz5;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setHz5(float hz5)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz5");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz5, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::tms6() const
{
    return m_tms6;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setTms6(float tms6)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms6");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms6, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::hz6() const
{
    return m_hz6;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setHz6(float hz6)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz6");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz6, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::tms7() const
{
    return m_tms7;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setTms7(float tms7)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms7");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms7, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::hz7() const
{
    return m_hz7;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setHz7(float hz7)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz7");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz7, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::tms8() const
{
    return m_tms8;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setTms8(float tms8)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms8");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms8, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::hz8() const
{
    return m_hz8;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setHz8(float hz8)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz8");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz8, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::tms9() const
{
    return m_tms9;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setTms9(float tms9)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms9");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms9, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::hz9() const
{
    return m_hz9;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setHz9(float hz9)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz9");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz9, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::tms10() const
{
    return m_tms10;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setTms10(float tms10)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms10");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms10, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::hz10() const
{
    return m_hz10;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setHz10(float hz10)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz10");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz10, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::tms11() const
{
    return m_tms11;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setTms11(float tms11)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms11");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms11, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::hz11() const
{
    return m_hz11;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setHz11(float hz11)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz11");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz11, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::tms12() const
{
    return m_tms12;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setTms12(float tms12)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms12");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms12, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::hz12() const
{
    return m_hz12;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setHz12(float hz12)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz12");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz12, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::tms13() const
{
    return m_tms13;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setTms13(float tms13)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms13");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms13, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::hz13() const
{
    return m_hz13;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setHz13(float hz13)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz13");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz13, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::tms14() const
{
    return m_tms14;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setTms14(float tms14)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms14");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms14, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::hz14() const
{
    return m_hz14;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setHz14(float hz14)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz14");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz14, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::tms15() const
{
    return m_tms15;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setTms15(float tms15)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms15");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms15, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::hz15() const
{
    return m_hz15;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setHz15(float hz15)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz15");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz15, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::tms16() const
{
    return m_tms16;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setTms16(float tms16)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms16");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms16, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::hz16() const
{
    return m_hz16;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setHz16(float hz16)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz16");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz16, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::tms17() const
{
    return m_tms17;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setTms17(float tms17)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms17");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms17, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::hz17() const
{
    return m_hz17;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setHz17(float hz17)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz17");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz17, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::tms18() const
{
    return m_tms18;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setTms18(float tms18)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms18");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms18, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::hz18() const
{
    return m_hz18;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setHz18(float hz18)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz18");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz18, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::tms19() const
{
    return m_tms19;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setTms19(float tms19)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms19");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms19, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::hz19() const
{
    return m_hz19;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setHz19(float hz19)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz19");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz19, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::tms20() const
{
    return m_tms20;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setTms20(float tms20)
{
    SunSpecDataPoint dp = m_dataPoints.value("Tms20");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(tms20, m_parentModel->tmsSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecLfrtxModelRepeatingBlock::hz20() const
{
    return m_hz20;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setHz20(float hz20)
{
    SunSpecDataPoint dp = m_dataPoints.value("Hz20");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(hz20, m_parentModel->hzSf(), dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

QString SunSpecLfrtxModelRepeatingBlock::crvNam() const
{
    return m_crvNam;
}

QModbusReply *SunSpecLfrtxModelRepeatingBlock::setCrvNam(const QString &crvNam)
{
    SunSpecDataPoint dp = m_dataPoints.value("CrvNam");
    QVector<quint16> registers = SunSpecDataPoint::convertFromString(crvNam, dp.size());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

SunSpecLfrtxModelRepeatingBlock::Readonly SunSpecLfrtxModelRepeatingBlock::readOnly() const
{
    return m_readOnly;
}

void SunSpecLfrtxModelRepeatingBlock::initDataPoints()
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
    m_dataPoints.insert(tms1DataPoint.name(), tms1DataPoint);

    SunSpecDataPoint hz1DataPoint;
    hz1DataPoint.setName("Hz1");
    hz1DataPoint.setLabel("Hz1");
    hz1DataPoint.setDescription("Point 1 frequency.");
    hz1DataPoint.setUnits("Hz");
    hz1DataPoint.setMandatory(true);
    hz1DataPoint.setSize(1);
    hz1DataPoint.setAddressOffset(2);
    hz1DataPoint.setBlockOffset(0);
    hz1DataPoint.setScaleFactorName("Hz_SF");
    hz1DataPoint.setSunSpecDataType("uint16");
    hz1DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hz1DataPoint.name(), hz1DataPoint);

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
    m_dataPoints.insert(tms2DataPoint.name(), tms2DataPoint);

    SunSpecDataPoint hz2DataPoint;
    hz2DataPoint.setName("Hz2");
    hz2DataPoint.setLabel("Hz2");
    hz2DataPoint.setDescription("Point 2 frequency.");
    hz2DataPoint.setUnits("Hz");
    hz2DataPoint.setSize(1);
    hz2DataPoint.setAddressOffset(4);
    hz2DataPoint.setBlockOffset(2);
    hz2DataPoint.setScaleFactorName("Hz_SF");
    hz2DataPoint.setSunSpecDataType("uint16");
    hz2DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hz2DataPoint.name(), hz2DataPoint);

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
    m_dataPoints.insert(tms3DataPoint.name(), tms3DataPoint);

    SunSpecDataPoint hz3DataPoint;
    hz3DataPoint.setName("Hz3");
    hz3DataPoint.setLabel("Hz3");
    hz3DataPoint.setDescription("Point 3 frequency.");
    hz3DataPoint.setUnits("Hz");
    hz3DataPoint.setSize(1);
    hz3DataPoint.setAddressOffset(6);
    hz3DataPoint.setBlockOffset(4);
    hz3DataPoint.setScaleFactorName("Hz_SF");
    hz3DataPoint.setSunSpecDataType("uint16");
    hz3DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hz3DataPoint.name(), hz3DataPoint);

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
    m_dataPoints.insert(tms4DataPoint.name(), tms4DataPoint);

    SunSpecDataPoint hz4DataPoint;
    hz4DataPoint.setName("Hz4");
    hz4DataPoint.setLabel("Hz4");
    hz4DataPoint.setDescription("Point 4 frequency.");
    hz4DataPoint.setUnits("Hz");
    hz4DataPoint.setSize(1);
    hz4DataPoint.setAddressOffset(8);
    hz4DataPoint.setBlockOffset(6);
    hz4DataPoint.setScaleFactorName("Hz_SF");
    hz4DataPoint.setSunSpecDataType("uint16");
    hz4DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hz4DataPoint.name(), hz4DataPoint);

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
    m_dataPoints.insert(tms5DataPoint.name(), tms5DataPoint);

    SunSpecDataPoint hz5DataPoint;
    hz5DataPoint.setName("Hz5");
    hz5DataPoint.setLabel("Hz5");
    hz5DataPoint.setDescription("Point 5 frequency.");
    hz5DataPoint.setUnits("Hz");
    hz5DataPoint.setSize(1);
    hz5DataPoint.setAddressOffset(10);
    hz5DataPoint.setBlockOffset(8);
    hz5DataPoint.setScaleFactorName("Hz_SF");
    hz5DataPoint.setSunSpecDataType("uint16");
    hz5DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hz5DataPoint.name(), hz5DataPoint);

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
    m_dataPoints.insert(tms6DataPoint.name(), tms6DataPoint);

    SunSpecDataPoint hz6DataPoint;
    hz6DataPoint.setName("Hz6");
    hz6DataPoint.setLabel("Hz6");
    hz6DataPoint.setDescription("Point 6 frequency.");
    hz6DataPoint.setUnits("Hz");
    hz6DataPoint.setSize(1);
    hz6DataPoint.setAddressOffset(12);
    hz6DataPoint.setBlockOffset(10);
    hz6DataPoint.setScaleFactorName("Hz_SF");
    hz6DataPoint.setSunSpecDataType("uint16");
    hz6DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hz6DataPoint.name(), hz6DataPoint);

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
    m_dataPoints.insert(tms7DataPoint.name(), tms7DataPoint);

    SunSpecDataPoint hz7DataPoint;
    hz7DataPoint.setName("Hz7");
    hz7DataPoint.setLabel("Hz7");
    hz7DataPoint.setDescription("Point 7 frequency.");
    hz7DataPoint.setUnits("Hz");
    hz7DataPoint.setSize(1);
    hz7DataPoint.setAddressOffset(14);
    hz7DataPoint.setBlockOffset(12);
    hz7DataPoint.setScaleFactorName("Hz_SF");
    hz7DataPoint.setSunSpecDataType("uint16");
    hz7DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hz7DataPoint.name(), hz7DataPoint);

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
    m_dataPoints.insert(tms8DataPoint.name(), tms8DataPoint);

    SunSpecDataPoint hz8DataPoint;
    hz8DataPoint.setName("Hz8");
    hz8DataPoint.setLabel("Hz8");
    hz8DataPoint.setDescription("Point 8 frequency.");
    hz8DataPoint.setUnits("Hz");
    hz8DataPoint.setSize(1);
    hz8DataPoint.setAddressOffset(16);
    hz8DataPoint.setBlockOffset(14);
    hz8DataPoint.setScaleFactorName("Hz_SF");
    hz8DataPoint.setSunSpecDataType("uint16");
    hz8DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hz8DataPoint.name(), hz8DataPoint);

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
    m_dataPoints.insert(tms9DataPoint.name(), tms9DataPoint);

    SunSpecDataPoint hz9DataPoint;
    hz9DataPoint.setName("Hz9");
    hz9DataPoint.setLabel("Hz9");
    hz9DataPoint.setDescription("Point 9 frequency.");
    hz9DataPoint.setUnits("Hz");
    hz9DataPoint.setSize(1);
    hz9DataPoint.setAddressOffset(18);
    hz9DataPoint.setBlockOffset(16);
    hz9DataPoint.setScaleFactorName("Hz_SF");
    hz9DataPoint.setSunSpecDataType("uint16");
    hz9DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hz9DataPoint.name(), hz9DataPoint);

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
    m_dataPoints.insert(tms10DataPoint.name(), tms10DataPoint);

    SunSpecDataPoint hz10DataPoint;
    hz10DataPoint.setName("Hz10");
    hz10DataPoint.setLabel("Hz10");
    hz10DataPoint.setDescription("Point 10 frequency.");
    hz10DataPoint.setUnits("Hz");
    hz10DataPoint.setSize(1);
    hz10DataPoint.setAddressOffset(20);
    hz10DataPoint.setBlockOffset(18);
    hz10DataPoint.setScaleFactorName("Hz_SF");
    hz10DataPoint.setSunSpecDataType("uint16");
    hz10DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hz10DataPoint.name(), hz10DataPoint);

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
    m_dataPoints.insert(tms11DataPoint.name(), tms11DataPoint);

    SunSpecDataPoint hz11DataPoint;
    hz11DataPoint.setName("Hz11");
    hz11DataPoint.setLabel("Hz11");
    hz11DataPoint.setDescription("Point 11 frequency.");
    hz11DataPoint.setUnits("Hz");
    hz11DataPoint.setSize(1);
    hz11DataPoint.setAddressOffset(22);
    hz11DataPoint.setBlockOffset(20);
    hz11DataPoint.setScaleFactorName("Hz_SF");
    hz11DataPoint.setSunSpecDataType("uint16");
    hz11DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hz11DataPoint.name(), hz11DataPoint);

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
    m_dataPoints.insert(tms12DataPoint.name(), tms12DataPoint);

    SunSpecDataPoint hz12DataPoint;
    hz12DataPoint.setName("Hz12");
    hz12DataPoint.setLabel("Hz12");
    hz12DataPoint.setDescription("Point 12 frequency.");
    hz12DataPoint.setUnits("Hz");
    hz12DataPoint.setSize(1);
    hz12DataPoint.setAddressOffset(24);
    hz12DataPoint.setBlockOffset(22);
    hz12DataPoint.setScaleFactorName("Hz_SF");
    hz12DataPoint.setSunSpecDataType("uint16");
    hz12DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hz12DataPoint.name(), hz12DataPoint);

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
    m_dataPoints.insert(tms13DataPoint.name(), tms13DataPoint);

    SunSpecDataPoint hz13DataPoint;
    hz13DataPoint.setName("Hz13");
    hz13DataPoint.setLabel("Hz13");
    hz13DataPoint.setDescription("Point 13 frequency.");
    hz13DataPoint.setUnits("Hz");
    hz13DataPoint.setSize(1);
    hz13DataPoint.setAddressOffset(26);
    hz13DataPoint.setBlockOffset(24);
    hz13DataPoint.setScaleFactorName("Hz_SF");
    hz13DataPoint.setSunSpecDataType("uint16");
    hz13DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hz13DataPoint.name(), hz13DataPoint);

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
    m_dataPoints.insert(tms14DataPoint.name(), tms14DataPoint);

    SunSpecDataPoint hz14DataPoint;
    hz14DataPoint.setName("Hz14");
    hz14DataPoint.setLabel("Hz14");
    hz14DataPoint.setDescription("Point 14 frequency.");
    hz14DataPoint.setUnits("Hz");
    hz14DataPoint.setSize(1);
    hz14DataPoint.setAddressOffset(28);
    hz14DataPoint.setBlockOffset(26);
    hz14DataPoint.setScaleFactorName("Hz_SF");
    hz14DataPoint.setSunSpecDataType("uint16");
    hz14DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hz14DataPoint.name(), hz14DataPoint);

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
    m_dataPoints.insert(tms15DataPoint.name(), tms15DataPoint);

    SunSpecDataPoint hz15DataPoint;
    hz15DataPoint.setName("Hz15");
    hz15DataPoint.setLabel("Hz15");
    hz15DataPoint.setDescription("Point 15 frequency.");
    hz15DataPoint.setUnits("Hz");
    hz15DataPoint.setSize(1);
    hz15DataPoint.setAddressOffset(30);
    hz15DataPoint.setBlockOffset(28);
    hz15DataPoint.setScaleFactorName("Hz_SF");
    hz15DataPoint.setSunSpecDataType("uint16");
    hz15DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hz15DataPoint.name(), hz15DataPoint);

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
    m_dataPoints.insert(tms16DataPoint.name(), tms16DataPoint);

    SunSpecDataPoint hz16DataPoint;
    hz16DataPoint.setName("Hz16");
    hz16DataPoint.setLabel("Hz16");
    hz16DataPoint.setDescription("Point 16 frequency.");
    hz16DataPoint.setUnits("Hz");
    hz16DataPoint.setSize(1);
    hz16DataPoint.setAddressOffset(32);
    hz16DataPoint.setBlockOffset(30);
    hz16DataPoint.setScaleFactorName("Hz_SF");
    hz16DataPoint.setSunSpecDataType("uint16");
    hz16DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hz16DataPoint.name(), hz16DataPoint);

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
    m_dataPoints.insert(tms17DataPoint.name(), tms17DataPoint);

    SunSpecDataPoint hz17DataPoint;
    hz17DataPoint.setName("Hz17");
    hz17DataPoint.setLabel("Hz17");
    hz17DataPoint.setDescription("Point 17 frequency.");
    hz17DataPoint.setUnits("Hz");
    hz17DataPoint.setSize(1);
    hz17DataPoint.setAddressOffset(34);
    hz17DataPoint.setBlockOffset(32);
    hz17DataPoint.setScaleFactorName("Hz_SF");
    hz17DataPoint.setSunSpecDataType("uint16");
    hz17DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hz17DataPoint.name(), hz17DataPoint);

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
    m_dataPoints.insert(tms18DataPoint.name(), tms18DataPoint);

    SunSpecDataPoint hz18DataPoint;
    hz18DataPoint.setName("Hz18");
    hz18DataPoint.setLabel("Hz18");
    hz18DataPoint.setDescription("Point 18 frequency.");
    hz18DataPoint.setUnits("Hz");
    hz18DataPoint.setSize(1);
    hz18DataPoint.setAddressOffset(36);
    hz18DataPoint.setBlockOffset(34);
    hz18DataPoint.setScaleFactorName("Hz_SF");
    hz18DataPoint.setSunSpecDataType("uint16");
    hz18DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hz18DataPoint.name(), hz18DataPoint);

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
    m_dataPoints.insert(tms19DataPoint.name(), tms19DataPoint);

    SunSpecDataPoint hz19DataPoint;
    hz19DataPoint.setName("Hz19");
    hz19DataPoint.setLabel("Hz19");
    hz19DataPoint.setDescription("Point 19 frequency.");
    hz19DataPoint.setUnits("Hz");
    hz19DataPoint.setSize(1);
    hz19DataPoint.setAddressOffset(38);
    hz19DataPoint.setBlockOffset(36);
    hz19DataPoint.setScaleFactorName("Hz_SF");
    hz19DataPoint.setSunSpecDataType("uint16");
    hz19DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hz19DataPoint.name(), hz19DataPoint);

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
    m_dataPoints.insert(tms20DataPoint.name(), tms20DataPoint);

    SunSpecDataPoint hz20DataPoint;
    hz20DataPoint.setName("Hz20");
    hz20DataPoint.setLabel("Hz20");
    hz20DataPoint.setDescription("Point 20 frequency.");
    hz20DataPoint.setUnits("Hz");
    hz20DataPoint.setSize(1);
    hz20DataPoint.setAddressOffset(40);
    hz20DataPoint.setBlockOffset(38);
    hz20DataPoint.setScaleFactorName("Hz_SF");
    hz20DataPoint.setSunSpecDataType("uint16");
    hz20DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(hz20DataPoint.name(), hz20DataPoint);

    SunSpecDataPoint crvNamDataPoint;
    crvNamDataPoint.setName("CrvNam");
    crvNamDataPoint.setLabel("CrvNam");
    crvNamDataPoint.setDescription("Optional description for curve.");
    crvNamDataPoint.setSize(8);
    crvNamDataPoint.setAddressOffset(41);
    crvNamDataPoint.setBlockOffset(39);
    crvNamDataPoint.setSunSpecDataType("string");
    crvNamDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
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
    m_dataPoints.insert(readOnlyDataPoint.name(), readOnlyDataPoint);

}

void SunSpecLfrtxModelRepeatingBlock::processBlockData(const QVector<quint16> blockData)
{
    m_blockData = blockData;

    // Update properties according to the data point type
    if (m_dataPoints.value("ActPt").isValid())
        m_actPt = m_dataPoints.value("ActPt").toUInt16();

    if (m_dataPoints.value("Tms1").isValid())
        m_tms1 = m_dataPoints.value("Tms1").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("Hz1").isValid())
        m_hz1 = m_dataPoints.value("Hz1").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("Tms2").isValid())
        m_tms2 = m_dataPoints.value("Tms2").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("Hz2").isValid())
        m_hz2 = m_dataPoints.value("Hz2").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("Tms3").isValid())
        m_tms3 = m_dataPoints.value("Tms3").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("Hz3").isValid())
        m_hz3 = m_dataPoints.value("Hz3").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("Tms4").isValid())
        m_tms4 = m_dataPoints.value("Tms4").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("Hz4").isValid())
        m_hz4 = m_dataPoints.value("Hz4").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("Tms5").isValid())
        m_tms5 = m_dataPoints.value("Tms5").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("Hz5").isValid())
        m_hz5 = m_dataPoints.value("Hz5").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("Tms6").isValid())
        m_tms6 = m_dataPoints.value("Tms6").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("Hz6").isValid())
        m_hz6 = m_dataPoints.value("Hz6").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("Tms7").isValid())
        m_tms7 = m_dataPoints.value("Tms7").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("Hz7").isValid())
        m_hz7 = m_dataPoints.value("Hz7").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("Tms8").isValid())
        m_tms8 = m_dataPoints.value("Tms8").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("Hz8").isValid())
        m_hz8 = m_dataPoints.value("Hz8").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("Tms9").isValid())
        m_tms9 = m_dataPoints.value("Tms9").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("Hz9").isValid())
        m_hz9 = m_dataPoints.value("Hz9").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("Tms10").isValid())
        m_tms10 = m_dataPoints.value("Tms10").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("Hz10").isValid())
        m_hz10 = m_dataPoints.value("Hz10").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("Tms11").isValid())
        m_tms11 = m_dataPoints.value("Tms11").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("Hz11").isValid())
        m_hz11 = m_dataPoints.value("Hz11").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("Tms12").isValid())
        m_tms12 = m_dataPoints.value("Tms12").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("Hz12").isValid())
        m_hz12 = m_dataPoints.value("Hz12").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("Tms13").isValid())
        m_tms13 = m_dataPoints.value("Tms13").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("Hz13").isValid())
        m_hz13 = m_dataPoints.value("Hz13").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("Tms14").isValid())
        m_tms14 = m_dataPoints.value("Tms14").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("Hz14").isValid())
        m_hz14 = m_dataPoints.value("Hz14").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("Tms15").isValid())
        m_tms15 = m_dataPoints.value("Tms15").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("Hz15").isValid())
        m_hz15 = m_dataPoints.value("Hz15").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("Tms16").isValid())
        m_tms16 = m_dataPoints.value("Tms16").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("Hz16").isValid())
        m_hz16 = m_dataPoints.value("Hz16").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("Tms17").isValid())
        m_tms17 = m_dataPoints.value("Tms17").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("Hz17").isValid())
        m_hz17 = m_dataPoints.value("Hz17").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("Tms18").isValid())
        m_tms18 = m_dataPoints.value("Tms18").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("Hz18").isValid())
        m_hz18 = m_dataPoints.value("Hz18").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("Tms19").isValid())
        m_tms19 = m_dataPoints.value("Tms19").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("Hz19").isValid())
        m_hz19 = m_dataPoints.value("Hz19").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("Tms20").isValid())
        m_tms20 = m_dataPoints.value("Tms20").toFloatWithSSF(m_parentModel->tmsSf());

    if (m_dataPoints.value("Hz20").isValid())
        m_hz20 = m_dataPoints.value("Hz20").toFloatWithSSF(m_parentModel->hzSf());

    if (m_dataPoints.value("CrvNam").isValid())
        m_crvNam = m_dataPoints.value("CrvNam").toString();

    if (m_dataPoints.value("ReadOnly").isValid())
        m_readOnly = static_cast<Readonly>(m_dataPoints.value("ReadOnly").toUInt16());


    qCDebug(dcSunSpecModelData()) << this;
}


SunSpecLfrtxModel::SunSpecLfrtxModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 143, modelLength, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixedAndRepeating;

    initDataPoints();
}

SunSpecLfrtxModel::~SunSpecLfrtxModel()
{

}

QString SunSpecLfrtxModel::name() const
{
    return "lfrtx";
}

QString SunSpecLfrtxModel::description() const
{
    return "LFRT extended curve";
}

QString SunSpecLfrtxModel::label() const
{
    return "LFRTX";
}

quint16 SunSpecLfrtxModel::actCrv() const
{
    return m_actCrv;
}

QModbusReply *SunSpecLfrtxModel::setActCrv(quint16 actCrv)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("ActCrv");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(actCrv);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecLfrtxModel::ModenaFlags SunSpecLfrtxModel::modEna() const
{
    return m_modEna;
}

QModbusReply *SunSpecLfrtxModel::setModEna(ModenaFlags modEna)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("ModEna");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(modEna));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecLfrtxModel::winTms() const
{
    return m_winTms;
}

QModbusReply *SunSpecLfrtxModel::setWinTms(quint16 winTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("WinTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(winTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecLfrtxModel::rvrtTms() const
{
    return m_rvrtTms;
}

QModbusReply *SunSpecLfrtxModel::setRvrtTms(quint16 rvrtTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("RvrtTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(rvrtTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecLfrtxModel::rmpTms() const
{
    return m_rmpTms;
}

QModbusReply *SunSpecLfrtxModel::setRmpTms(quint16 rmpTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("RmpTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(rmpTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecLfrtxModel::nCrv() const
{
    return m_nCrv;
}
quint16 SunSpecLfrtxModel::nPt() const
{
    return m_nPt;
}
qint16 SunSpecLfrtxModel::tmsSf() const
{
    return m_tmsSf;
}
qint16 SunSpecLfrtxModel::hzSf() const
{
    return m_hzSf;
}
SunSpecLfrtxModel::Crvtype SunSpecLfrtxModel::crvType() const
{
    return m_crvType;
}
void SunSpecLfrtxModel::initDataPoints()
{
    SunSpecDataPoint modelIdDataPoint;
    modelIdDataPoint.setName("ID");
    modelIdDataPoint.setLabel("Model ID");
    modelIdDataPoint.setDescription("Model identifier");
    modelIdDataPoint.setMandatory(true);
    modelIdDataPoint.setSize(1);
    modelIdDataPoint.setAddressOffset(0);
    modelIdDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(modelIdDataPoint.name(), modelIdDataPoint);

    SunSpecDataPoint modelLengthDataPoint;
    modelLengthDataPoint.setName("L");
    modelLengthDataPoint.setLabel("Model Length");
    modelLengthDataPoint.setDescription("Model length");
    modelLengthDataPoint.setMandatory(true);
    modelLengthDataPoint.setSize(1);
    modelLengthDataPoint.setAddressOffset(1);
    modelLengthDataPoint.setSunSpecDataType("uint16");
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
    m_dataPoints.insert(actCrvDataPoint.name(), actCrvDataPoint);

    SunSpecDataPoint modEnaDataPoint;
    modEnaDataPoint.setName("ModEna");
    modEnaDataPoint.setLabel("ModEna");
    modEnaDataPoint.setDescription("LHzRT control mode. Enable active curve.  Bitfield value.");
    modEnaDataPoint.setMandatory(true);
    modEnaDataPoint.setSize(1);
    modEnaDataPoint.setAddressOffset(3);
    modEnaDataPoint.setBlockOffset(1);
    modEnaDataPoint.setSunSpecDataType("bitfield16");
    modEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(modEnaDataPoint.name(), modEnaDataPoint);

    SunSpecDataPoint winTmsDataPoint;
    winTmsDataPoint.setName("WinTms");
    winTmsDataPoint.setLabel("WinTms");
    winTmsDataPoint.setDescription("Time window for LFRT change.");
    winTmsDataPoint.setUnits("Secs");
    winTmsDataPoint.setSize(1);
    winTmsDataPoint.setAddressOffset(4);
    winTmsDataPoint.setBlockOffset(2);
    winTmsDataPoint.setSunSpecDataType("uint16");
    winTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(winTmsDataPoint.name(), winTmsDataPoint);

    SunSpecDataPoint rvrtTmsDataPoint;
    rvrtTmsDataPoint.setName("RvrtTms");
    rvrtTmsDataPoint.setLabel("RvrtTms");
    rvrtTmsDataPoint.setDescription("Timeout period for LFRT curve selection.");
    rvrtTmsDataPoint.setUnits("Secs");
    rvrtTmsDataPoint.setSize(1);
    rvrtTmsDataPoint.setAddressOffset(5);
    rvrtTmsDataPoint.setBlockOffset(3);
    rvrtTmsDataPoint.setSunSpecDataType("uint16");
    rvrtTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
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
    m_dataPoints.insert(tmsSfDataPoint.name(), tmsSfDataPoint);

    SunSpecDataPoint hzSfDataPoint;
    hzSfDataPoint.setName("Hz_SF");
    hzSfDataPoint.setLabel("Hz_SF");
    hzSfDataPoint.setDescription("Scale factor for frequency.");
    hzSfDataPoint.setMandatory(true);
    hzSfDataPoint.setSize(1);
    hzSfDataPoint.setAddressOffset(10);
    hzSfDataPoint.setBlockOffset(8);
    hzSfDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(hzSfDataPoint.name(), hzSfDataPoint);

    SunSpecDataPoint crvTypeDataPoint;
    crvTypeDataPoint.setName("CrvType");
    crvTypeDataPoint.setMandatory(true);
    crvTypeDataPoint.setSize(1);
    crvTypeDataPoint.setAddressOffset(11);
    crvTypeDataPoint.setBlockOffset(9);
    crvTypeDataPoint.setSunSpecDataType("enum16");
    m_dataPoints.insert(crvTypeDataPoint.name(), crvTypeDataPoint);

}

void SunSpecLfrtxModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("Tms_SF").isValid())
        m_tmsSf = m_dataPoints.value("Tms_SF").toInt16();

    if (m_dataPoints.value("Hz_SF").isValid())
        m_hzSf = m_dataPoints.value("Hz_SF").toInt16();


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

    if (m_dataPoints.value("Hz_SF").isValid())
        m_hzSf = m_dataPoints.value("Hz_SF").toInt16();

    if (m_dataPoints.value("CrvType").isValid())
        m_crvType = static_cast<Crvtype>(m_dataPoints.value("CrvType").toUInt16());


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecLfrtxModel *model)
{
    debug.nospace().noquote() << "SunSpecLfrtxModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")" << endl;
    debug.nospace().noquote() << "    - " << model->dataPoints().value("ActCrv") << "-->";
    if (model->dataPoints().value("ActCrv").isValid()) {
        debug.nospace().noquote() << model->actCrv() << endl;
    } else {
        debug.nospace().noquote() << "NaN" << endl;
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModEna") << "-->";
    if (model->dataPoints().value("ModEna").isValid()) {
        debug.nospace().noquote() << model->modEna() << endl;
    } else {
        debug.nospace().noquote() << "NaN" << endl;
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WinTms") << "-->";
    if (model->dataPoints().value("WinTms").isValid()) {
        debug.nospace().noquote() << model->winTms() << endl;
    } else {
        debug.nospace().noquote() << "NaN" << endl;
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("RvrtTms") << "-->";
    if (model->dataPoints().value("RvrtTms").isValid()) {
        debug.nospace().noquote() << model->rvrtTms() << endl;
    } else {
        debug.nospace().noquote() << "NaN" << endl;
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("RmpTms") << "-->";
    if (model->dataPoints().value("RmpTms").isValid()) {
        debug.nospace().noquote() << model->rmpTms() << endl;
    } else {
        debug.nospace().noquote() << "NaN" << endl;
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("NCrv") << "-->";
    if (model->dataPoints().value("NCrv").isValid()) {
        debug.nospace().noquote() << model->nCrv() << endl;
    } else {
        debug.nospace().noquote() << "NaN" << endl;
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("NPt") << "-->";
    if (model->dataPoints().value("NPt").isValid()) {
        debug.nospace().noquote() << model->nPt() << endl;
    } else {
        debug.nospace().noquote() << "NaN" << endl;
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CrvType") << "-->";
    if (model->dataPoints().value("CrvType").isValid()) {
        debug.nospace().noquote() << model->crvType() << endl;
    } else {
        debug.nospace().noquote() << "NaN" << endl;
    }


    return debug.space().quote();
}
