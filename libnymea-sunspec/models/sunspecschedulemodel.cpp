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

#include "sunspecschedulemodel.h"
#include "sunspecconnection.h"

SunSpecScheduleModelRepeatingBlock::SunSpecScheduleModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecScheduleModel *parent) :
    SunSpecModelRepeatingBlock(blockIndex, blockSize, modbusStartRegister, parent)
{
    m_byteOrder = parent->byteOrder();
    initDataPoints();
}

QString SunSpecScheduleModelRepeatingBlock::name() const
{
    return "repeating";
}

SunSpecScheduleModel *SunSpecScheduleModelRepeatingBlock::parentModel() const
{
    return m_parentModel;
}

quint16 SunSpecScheduleModelRepeatingBlock::actPts() const
{
    return m_actPts;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setActPts(quint16 actPts)
{
    SunSpecDataPoint dp = m_dataPoints.value("ActPts");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(actPts);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

quint32 SunSpecScheduleModelRepeatingBlock::strTms() const
{
    return m_strTms;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setStrTms(quint32 strTms)
{
    SunSpecDataPoint dp = m_dataPoints.value("StrTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt32(strTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

quint16 SunSpecScheduleModelRepeatingBlock::repPer() const
{
    return m_repPer;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setRepPer(quint16 repPer)
{
    SunSpecDataPoint dp = m_dataPoints.value("RepPer");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(repPer);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

SunSpecScheduleModelRepeatingBlock::Intvtyp SunSpecScheduleModelRepeatingBlock::schdTyp() const
{
    return m_schdTyp;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setSchdTyp(Intvtyp schdTyp)
{
    SunSpecDataPoint dp = m_dataPoints.value("IntvTyp");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(schdTyp));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

SunSpecScheduleModelRepeatingBlock::Xtyp SunSpecScheduleModelRepeatingBlock::xTyp() const
{
    return m_xTyp;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setXTyp(Xtyp xTyp)
{
    SunSpecDataPoint dp = m_dataPoints.value("XTyp");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(xTyp));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

SunSpecScheduleModelRepeatingBlock::Ytyp SunSpecScheduleModelRepeatingBlock::yTyp() const
{
    return m_yTyp;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setYTyp(Ytyp yTyp)
{
    SunSpecDataPoint dp = m_dataPoints.value("YTyp");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(yTyp));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecScheduleModelRepeatingBlock::x1() const
{
    return m_x1;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setX1(float x1)
{
    SunSpecDataPoint dp = m_dataPoints.value("X1");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(x1);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecScheduleModelRepeatingBlock::y1() const
{
    return m_y1;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setY1(float y1)
{
    SunSpecDataPoint dp = m_dataPoints.value("Y1");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(y1);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecScheduleModelRepeatingBlock::x2() const
{
    return m_x2;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setX2(float x2)
{
    SunSpecDataPoint dp = m_dataPoints.value("X2");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(x2);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecScheduleModelRepeatingBlock::y2() const
{
    return m_y2;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setY2(float y2)
{
    SunSpecDataPoint dp = m_dataPoints.value("Y2");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(y2);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecScheduleModelRepeatingBlock::x3() const
{
    return m_x3;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setX3(float x3)
{
    SunSpecDataPoint dp = m_dataPoints.value("X3");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(x3);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecScheduleModelRepeatingBlock::y3() const
{
    return m_y3;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setY3(float y3)
{
    SunSpecDataPoint dp = m_dataPoints.value("Y3");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(y3);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecScheduleModelRepeatingBlock::x4() const
{
    return m_x4;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setX4(float x4)
{
    SunSpecDataPoint dp = m_dataPoints.value("X4");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(x4);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecScheduleModelRepeatingBlock::y4() const
{
    return m_y4;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setY4(float y4)
{
    SunSpecDataPoint dp = m_dataPoints.value("Y4");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(y4);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecScheduleModelRepeatingBlock::x5() const
{
    return m_x5;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setX5(float x5)
{
    SunSpecDataPoint dp = m_dataPoints.value("X5");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(x5);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecScheduleModelRepeatingBlock::y5() const
{
    return m_y5;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setY5(float y5)
{
    SunSpecDataPoint dp = m_dataPoints.value("Y5");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(y5);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecScheduleModelRepeatingBlock::x6() const
{
    return m_x6;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setX6(float x6)
{
    SunSpecDataPoint dp = m_dataPoints.value("X6");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(x6);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecScheduleModelRepeatingBlock::y6() const
{
    return m_y6;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setY6(float y6)
{
    SunSpecDataPoint dp = m_dataPoints.value("Y6");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(y6);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecScheduleModelRepeatingBlock::x7() const
{
    return m_x7;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setX7(float x7)
{
    SunSpecDataPoint dp = m_dataPoints.value("X7");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(x7);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecScheduleModelRepeatingBlock::y7() const
{
    return m_y7;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setY7(float y7)
{
    SunSpecDataPoint dp = m_dataPoints.value("Y7");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(y7);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecScheduleModelRepeatingBlock::x8() const
{
    return m_x8;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setX8(float x8)
{
    SunSpecDataPoint dp = m_dataPoints.value("X8");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(x8);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecScheduleModelRepeatingBlock::y8() const
{
    return m_y8;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setY8(float y8)
{
    SunSpecDataPoint dp = m_dataPoints.value("Y8");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(y8);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecScheduleModelRepeatingBlock::x9() const
{
    return m_x9;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setX9(float x9)
{
    SunSpecDataPoint dp = m_dataPoints.value("X9");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(x9);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecScheduleModelRepeatingBlock::y9() const
{
    return m_y9;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setY9(float y9)
{
    SunSpecDataPoint dp = m_dataPoints.value("Y9");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(y9);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecScheduleModelRepeatingBlock::x10() const
{
    return m_x10;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setX10(float x10)
{
    SunSpecDataPoint dp = m_dataPoints.value("X10");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(x10);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecScheduleModelRepeatingBlock::y10() const
{
    return m_y10;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setY10(float y10)
{
    SunSpecDataPoint dp = m_dataPoints.value("Y10");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(y10);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

QString SunSpecScheduleModelRepeatingBlock::nam() const
{
    return m_nam;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setNam(const QString &nam)
{
    SunSpecDataPoint dp = m_dataPoints.value("Nam");
    QVector<quint16> registers = SunSpecDataPoint::convertFromString(nam, dp.size());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

quint16 SunSpecScheduleModelRepeatingBlock::winTms() const
{
    return m_winTms;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setWinTms(quint16 winTms)
{
    SunSpecDataPoint dp = m_dataPoints.value("WinTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(winTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

quint16 SunSpecScheduleModelRepeatingBlock::rmpTms() const
{
    return m_rmpTms;
}

QModbusReply *SunSpecScheduleModelRepeatingBlock::setRmpTms(quint16 rmpTms)
{
    SunSpecDataPoint dp = m_dataPoints.value("RmpTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(rmpTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

quint16 SunSpecScheduleModelRepeatingBlock::actIndx() const
{
    return m_actIndx;
}

void SunSpecScheduleModelRepeatingBlock::initDataPoints()
{
    SunSpecDataPoint actPtsDataPoint;
    actPtsDataPoint.setName("ActPts");
    actPtsDataPoint.setLabel("ActPts");
    actPtsDataPoint.setDescription("Number of active entries in schedule.");
    actPtsDataPoint.setMandatory(true);
    actPtsDataPoint.setSize(1);
    actPtsDataPoint.setAddressOffset(0);
    actPtsDataPoint.setSunSpecDataType("uint16");
    actPtsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    actPtsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(actPtsDataPoint.name(), actPtsDataPoint);

    SunSpecDataPoint strTmsDataPoint;
    strTmsDataPoint.setName("StrTms");
    strTmsDataPoint.setLabel("StrTms");
    strTmsDataPoint.setDescription("Schedule start in seconds since 2000 JAN 01 00:00:00 UTC.");
    strTmsDataPoint.setUnits("Secs");
    strTmsDataPoint.setMandatory(true);
    strTmsDataPoint.setSize(2);
    strTmsDataPoint.setAddressOffset(1);
    strTmsDataPoint.setSunSpecDataType("uint32");
    strTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    strTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(strTmsDataPoint.name(), strTmsDataPoint);

    SunSpecDataPoint repPerDataPoint;
    repPerDataPoint.setName("RepPer");
    repPerDataPoint.setLabel("RepPer");
    repPerDataPoint.setDescription("The repetition count for time-based schedules (0=repeat forever)");
    repPerDataPoint.setMandatory(true);
    repPerDataPoint.setSize(1);
    repPerDataPoint.setAddressOffset(3);
    repPerDataPoint.setBlockOffset(1);
    repPerDataPoint.setSunSpecDataType("uint16");
    repPerDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    repPerDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(repPerDataPoint.name(), repPerDataPoint);

    SunSpecDataPoint schdTypDataPoint;
    schdTypDataPoint.setName("IntvTyp");
    schdTypDataPoint.setLabel("SchdTyp");
    schdTypDataPoint.setDescription("The repetition frequency for time-based schedules: no repeat=0");
    schdTypDataPoint.setMandatory(true);
    schdTypDataPoint.setSize(1);
    schdTypDataPoint.setAddressOffset(4);
    schdTypDataPoint.setBlockOffset(2);
    schdTypDataPoint.setSunSpecDataType("enum16");
    schdTypDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    schdTypDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(schdTypDataPoint.name(), schdTypDataPoint);

    SunSpecDataPoint xTypDataPoint;
    xTypDataPoint.setName("XTyp");
    xTypDataPoint.setLabel("XTyp");
    xTypDataPoint.setDescription("The meaning of the X-values in the array. ");
    xTypDataPoint.setMandatory(true);
    xTypDataPoint.setSize(1);
    xTypDataPoint.setAddressOffset(5);
    xTypDataPoint.setBlockOffset(3);
    xTypDataPoint.setSunSpecDataType("enum16");
    xTypDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    xTypDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(xTypDataPoint.name(), xTypDataPoint);

    SunSpecDataPoint xSfDataPoint;
    xSfDataPoint.setName("X_SF");
    xSfDataPoint.setLabel("X_SF");
    xSfDataPoint.setDescription("Scale factor for schedule range values.");
    xSfDataPoint.setMandatory(true);
    xSfDataPoint.setSize(1);
    xSfDataPoint.setAddressOffset(6);
    xSfDataPoint.setBlockOffset(4);
    xSfDataPoint.setSunSpecDataType("sunssf");
    xSfDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    xSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(xSfDataPoint.name(), xSfDataPoint);

    SunSpecDataPoint yTypDataPoint;
    yTypDataPoint.setName("YTyp");
    yTypDataPoint.setLabel("YTyp");
    yTypDataPoint.setDescription("The meaning of the Y-values in the array.");
    yTypDataPoint.setMandatory(true);
    yTypDataPoint.setSize(1);
    yTypDataPoint.setAddressOffset(7);
    yTypDataPoint.setBlockOffset(5);
    yTypDataPoint.setSunSpecDataType("enum16");
    yTypDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    yTypDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(yTypDataPoint.name(), yTypDataPoint);

    SunSpecDataPoint ySfDataPoint;
    ySfDataPoint.setName("Y_SF");
    ySfDataPoint.setLabel("Y_SF");
    ySfDataPoint.setDescription("Scale factor for schedule target values.");
    ySfDataPoint.setMandatory(true);
    ySfDataPoint.setSize(1);
    ySfDataPoint.setAddressOffset(8);
    ySfDataPoint.setBlockOffset(6);
    ySfDataPoint.setSunSpecDataType("sunssf");
    ySfDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    ySfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(ySfDataPoint.name(), ySfDataPoint);

    SunSpecDataPoint x1DataPoint;
    x1DataPoint.setName("X1");
    x1DataPoint.setLabel("X1");
    x1DataPoint.setDescription("Entry 1 range.");
    x1DataPoint.setMandatory(true);
    x1DataPoint.setSize(2);
    x1DataPoint.setAddressOffset(9);
    x1DataPoint.setBlockOffset(7);
    x1DataPoint.setScaleFactorName("X_SF");
    x1DataPoint.setSunSpecDataType("int32");
    x1DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    x1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(x1DataPoint.name(), x1DataPoint);

    SunSpecDataPoint y1DataPoint;
    y1DataPoint.setName("Y1");
    y1DataPoint.setLabel("Y1");
    y1DataPoint.setDescription("Entry 1 target.");
    y1DataPoint.setMandatory(true);
    y1DataPoint.setSize(2);
    y1DataPoint.setAddressOffset(11);
    y1DataPoint.setBlockOffset(9);
    y1DataPoint.setScaleFactorName("Y_SF");
    y1DataPoint.setSunSpecDataType("int32");
    y1DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    y1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(y1DataPoint.name(), y1DataPoint);

    SunSpecDataPoint x2DataPoint;
    x2DataPoint.setName("X2");
    x2DataPoint.setLabel("X2");
    x2DataPoint.setDescription("Entry 2 range.");
    x2DataPoint.setSize(2);
    x2DataPoint.setAddressOffset(13);
    x2DataPoint.setBlockOffset(11);
    x2DataPoint.setScaleFactorName("X_SF");
    x2DataPoint.setSunSpecDataType("int32");
    x2DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    x2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(x2DataPoint.name(), x2DataPoint);

    SunSpecDataPoint y2DataPoint;
    y2DataPoint.setName("Y2");
    y2DataPoint.setLabel("Y2");
    y2DataPoint.setDescription("Entry 2 target.");
    y2DataPoint.setSize(2);
    y2DataPoint.setAddressOffset(15);
    y2DataPoint.setBlockOffset(13);
    y2DataPoint.setScaleFactorName("Y_SF");
    y2DataPoint.setSunSpecDataType("int32");
    y2DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    y2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(y2DataPoint.name(), y2DataPoint);

    SunSpecDataPoint x3DataPoint;
    x3DataPoint.setName("X3");
    x3DataPoint.setLabel("X3");
    x3DataPoint.setDescription("Entry 3 range.");
    x3DataPoint.setSize(2);
    x3DataPoint.setAddressOffset(17);
    x3DataPoint.setBlockOffset(15);
    x3DataPoint.setScaleFactorName("X_SF");
    x3DataPoint.setSunSpecDataType("int32");
    x3DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    x3DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(x3DataPoint.name(), x3DataPoint);

    SunSpecDataPoint y3DataPoint;
    y3DataPoint.setName("Y3");
    y3DataPoint.setLabel("Y3");
    y3DataPoint.setDescription("Entry 3 target.");
    y3DataPoint.setSize(2);
    y3DataPoint.setAddressOffset(19);
    y3DataPoint.setBlockOffset(17);
    y3DataPoint.setScaleFactorName("Y_SF");
    y3DataPoint.setSunSpecDataType("int32");
    y3DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    y3DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(y3DataPoint.name(), y3DataPoint);

    SunSpecDataPoint x4DataPoint;
    x4DataPoint.setName("X4");
    x4DataPoint.setLabel("X4");
    x4DataPoint.setDescription("Entry 4 range.");
    x4DataPoint.setSize(2);
    x4DataPoint.setAddressOffset(21);
    x4DataPoint.setBlockOffset(19);
    x4DataPoint.setScaleFactorName("X_SF");
    x4DataPoint.setSunSpecDataType("int32");
    x4DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    x4DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(x4DataPoint.name(), x4DataPoint);

    SunSpecDataPoint y4DataPoint;
    y4DataPoint.setName("Y4");
    y4DataPoint.setLabel("Y4");
    y4DataPoint.setDescription("Entry 4 target.");
    y4DataPoint.setSize(2);
    y4DataPoint.setAddressOffset(23);
    y4DataPoint.setBlockOffset(21);
    y4DataPoint.setScaleFactorName("Y_SF");
    y4DataPoint.setSunSpecDataType("int32");
    y4DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    y4DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(y4DataPoint.name(), y4DataPoint);

    SunSpecDataPoint x5DataPoint;
    x5DataPoint.setName("X5");
    x5DataPoint.setLabel("X5");
    x5DataPoint.setDescription("Entry 15range.");
    x5DataPoint.setSize(2);
    x5DataPoint.setAddressOffset(25);
    x5DataPoint.setBlockOffset(23);
    x5DataPoint.setScaleFactorName("X_SF");
    x5DataPoint.setSunSpecDataType("int32");
    x5DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    x5DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(x5DataPoint.name(), x5DataPoint);

    SunSpecDataPoint y5DataPoint;
    y5DataPoint.setName("Y5");
    y5DataPoint.setLabel("Y5");
    y5DataPoint.setDescription("Entry 5 target.");
    y5DataPoint.setSize(2);
    y5DataPoint.setAddressOffset(27);
    y5DataPoint.setBlockOffset(25);
    y5DataPoint.setScaleFactorName("Y_SF");
    y5DataPoint.setSunSpecDataType("int32");
    y5DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    y5DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(y5DataPoint.name(), y5DataPoint);

    SunSpecDataPoint x6DataPoint;
    x6DataPoint.setName("X6");
    x6DataPoint.setLabel("X6");
    x6DataPoint.setDescription("Entry 6 range.");
    x6DataPoint.setSize(2);
    x6DataPoint.setAddressOffset(29);
    x6DataPoint.setBlockOffset(27);
    x6DataPoint.setScaleFactorName("X_SF");
    x6DataPoint.setSunSpecDataType("int32");
    x6DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    x6DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(x6DataPoint.name(), x6DataPoint);

    SunSpecDataPoint y6DataPoint;
    y6DataPoint.setName("Y6");
    y6DataPoint.setLabel("Y6");
    y6DataPoint.setDescription("Entry 6 target.");
    y6DataPoint.setSize(2);
    y6DataPoint.setAddressOffset(31);
    y6DataPoint.setBlockOffset(29);
    y6DataPoint.setScaleFactorName("Y_SF");
    y6DataPoint.setSunSpecDataType("int32");
    y6DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    y6DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(y6DataPoint.name(), y6DataPoint);

    SunSpecDataPoint x7DataPoint;
    x7DataPoint.setName("X7");
    x7DataPoint.setLabel("X7");
    x7DataPoint.setDescription("Entry 7 range.");
    x7DataPoint.setSize(2);
    x7DataPoint.setAddressOffset(33);
    x7DataPoint.setBlockOffset(31);
    x7DataPoint.setScaleFactorName("X_SF");
    x7DataPoint.setSunSpecDataType("int32");
    x7DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    x7DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(x7DataPoint.name(), x7DataPoint);

    SunSpecDataPoint y7DataPoint;
    y7DataPoint.setName("Y7");
    y7DataPoint.setLabel("Y7");
    y7DataPoint.setDescription("Entry 7 target.");
    y7DataPoint.setSize(2);
    y7DataPoint.setAddressOffset(35);
    y7DataPoint.setBlockOffset(33);
    y7DataPoint.setScaleFactorName("Y_SF");
    y7DataPoint.setSunSpecDataType("int32");
    y7DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    y7DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(y7DataPoint.name(), y7DataPoint);

    SunSpecDataPoint x8DataPoint;
    x8DataPoint.setName("X8");
    x8DataPoint.setLabel("X8");
    x8DataPoint.setDescription("Entry 8 range.");
    x8DataPoint.setSize(2);
    x8DataPoint.setAddressOffset(37);
    x8DataPoint.setBlockOffset(35);
    x8DataPoint.setScaleFactorName("X_SF");
    x8DataPoint.setSunSpecDataType("int32");
    x8DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    x8DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(x8DataPoint.name(), x8DataPoint);

    SunSpecDataPoint y8DataPoint;
    y8DataPoint.setName("Y8");
    y8DataPoint.setLabel("Y8");
    y8DataPoint.setDescription("Entry 8 target.");
    y8DataPoint.setSize(2);
    y8DataPoint.setAddressOffset(39);
    y8DataPoint.setBlockOffset(37);
    y8DataPoint.setScaleFactorName("Y_SF");
    y8DataPoint.setSunSpecDataType("int32");
    y8DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    y8DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(y8DataPoint.name(), y8DataPoint);

    SunSpecDataPoint x9DataPoint;
    x9DataPoint.setName("X9");
    x9DataPoint.setLabel("X9");
    x9DataPoint.setDescription("Entry 9 range.");
    x9DataPoint.setSize(2);
    x9DataPoint.setAddressOffset(41);
    x9DataPoint.setBlockOffset(39);
    x9DataPoint.setScaleFactorName("X_SF");
    x9DataPoint.setSunSpecDataType("int32");
    x9DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    x9DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(x9DataPoint.name(), x9DataPoint);

    SunSpecDataPoint y9DataPoint;
    y9DataPoint.setName("Y9");
    y9DataPoint.setLabel("Y9");
    y9DataPoint.setDescription("Entry 9 target.");
    y9DataPoint.setSize(2);
    y9DataPoint.setAddressOffset(43);
    y9DataPoint.setBlockOffset(41);
    y9DataPoint.setScaleFactorName("Y_SF");
    y9DataPoint.setSunSpecDataType("int32");
    y9DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    y9DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(y9DataPoint.name(), y9DataPoint);

    SunSpecDataPoint x10DataPoint;
    x10DataPoint.setName("X10");
    x10DataPoint.setLabel("X10");
    x10DataPoint.setDescription("Entry 10 range.");
    x10DataPoint.setSize(2);
    x10DataPoint.setAddressOffset(45);
    x10DataPoint.setBlockOffset(43);
    x10DataPoint.setScaleFactorName("X_SF");
    x10DataPoint.setSunSpecDataType("int32");
    x10DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    x10DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(x10DataPoint.name(), x10DataPoint);

    SunSpecDataPoint y10DataPoint;
    y10DataPoint.setName("Y10");
    y10DataPoint.setLabel("Y10");
    y10DataPoint.setDescription("Entry 10 target.");
    y10DataPoint.setSize(2);
    y10DataPoint.setAddressOffset(47);
    y10DataPoint.setBlockOffset(45);
    y10DataPoint.setScaleFactorName("Y_SF");
    y10DataPoint.setSunSpecDataType("int32");
    y10DataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    y10DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(y10DataPoint.name(), y10DataPoint);

    SunSpecDataPoint namDataPoint;
    namDataPoint.setName("Nam");
    namDataPoint.setLabel("Nam");
    namDataPoint.setDescription("Optional description for schedule.");
    namDataPoint.setSize(8);
    namDataPoint.setAddressOffset(49);
    namDataPoint.setBlockOffset(47);
    namDataPoint.setSunSpecDataType("string");
    namDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    namDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(namDataPoint.name(), namDataPoint);

    SunSpecDataPoint winTmsDataPoint;
    winTmsDataPoint.setName("WinTms");
    winTmsDataPoint.setLabel("WinTms");
    winTmsDataPoint.setDescription("Time window for schedule entry change.");
    winTmsDataPoint.setUnits("Secs");
    winTmsDataPoint.setSize(1);
    winTmsDataPoint.setAddressOffset(57);
    winTmsDataPoint.setBlockOffset(55);
    winTmsDataPoint.setSunSpecDataType("uint16");
    winTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    winTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(winTmsDataPoint.name(), winTmsDataPoint);

    SunSpecDataPoint rmpTmsDataPoint;
    rmpTmsDataPoint.setName("RmpTms");
    rmpTmsDataPoint.setLabel("RmpTms");
    rmpTmsDataPoint.setDescription("Ramp time for moving from current target to new target.");
    rmpTmsDataPoint.setUnits("Secs");
    rmpTmsDataPoint.setSize(1);
    rmpTmsDataPoint.setAddressOffset(58);
    rmpTmsDataPoint.setBlockOffset(56);
    rmpTmsDataPoint.setSunSpecDataType("uint16");
    rmpTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    rmpTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(rmpTmsDataPoint.name(), rmpTmsDataPoint);

    SunSpecDataPoint actIndxDataPoint;
    actIndxDataPoint.setName("ActIndx");
    actIndxDataPoint.setLabel("ActIndx");
    actIndxDataPoint.setDescription("Index of active entry in the active schedule.");
    actIndxDataPoint.setMandatory(true);
    actIndxDataPoint.setSize(1);
    actIndxDataPoint.setAddressOffset(59);
    actIndxDataPoint.setBlockOffset(57);
    actIndxDataPoint.setSunSpecDataType("uint16");
    actIndxDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(actIndxDataPoint.name(), actIndxDataPoint);

}

void SunSpecScheduleModelRepeatingBlock::processBlockData(const QVector<quint16> blockData)
{
    m_blockData = blockData;

    // Scale factors
    if (m_dataPoints.value("X_SF").isValid())
        m_xSf = m_dataPoints.value("X_SF").toInt16();

    if (m_dataPoints.value("Y_SF").isValid())
        m_ySf = m_dataPoints.value("Y_SF").toInt16();


    // Update properties according to the data point type
    if (m_dataPoints.value("ActPts").isValid())
        m_actPts = m_dataPoints.value("ActPts").toUInt16();

    if (m_dataPoints.value("StrTms").isValid())
        m_strTms = m_dataPoints.value("StrTms").toUInt32();

    if (m_dataPoints.value("RepPer").isValid())
        m_repPer = m_dataPoints.value("RepPer").toUInt16();

    if (m_dataPoints.value("IntvTyp").isValid())
        m_schdTyp = static_cast<Intvtyp>(m_dataPoints.value("IntvTyp").toUInt16());

    if (m_dataPoints.value("XTyp").isValid())
        m_xTyp = static_cast<Xtyp>(m_dataPoints.value("XTyp").toUInt16());

    if (m_dataPoints.value("X_SF").isValid())
        m_xSf = m_dataPoints.value("X_SF").toInt16();

    if (m_dataPoints.value("YTyp").isValid())
        m_yTyp = static_cast<Ytyp>(m_dataPoints.value("YTyp").toUInt16());

    if (m_dataPoints.value("Y_SF").isValid())
        m_ySf = m_dataPoints.value("Y_SF").toInt16();

    if (m_dataPoints.value("X1").isValid())
        m_x1 = m_dataPoints.value("X1").toFloatWithSSF(m_xSf);

    if (m_dataPoints.value("Y1").isValid())
        m_y1 = m_dataPoints.value("Y1").toFloatWithSSF(m_ySf);

    if (m_dataPoints.value("X2").isValid())
        m_x2 = m_dataPoints.value("X2").toFloatWithSSF(m_xSf);

    if (m_dataPoints.value("Y2").isValid())
        m_y2 = m_dataPoints.value("Y2").toFloatWithSSF(m_ySf);

    if (m_dataPoints.value("X3").isValid())
        m_x3 = m_dataPoints.value("X3").toFloatWithSSF(m_xSf);

    if (m_dataPoints.value("Y3").isValid())
        m_y3 = m_dataPoints.value("Y3").toFloatWithSSF(m_ySf);

    if (m_dataPoints.value("X4").isValid())
        m_x4 = m_dataPoints.value("X4").toFloatWithSSF(m_xSf);

    if (m_dataPoints.value("Y4").isValid())
        m_y4 = m_dataPoints.value("Y4").toFloatWithSSF(m_ySf);

    if (m_dataPoints.value("X5").isValid())
        m_x5 = m_dataPoints.value("X5").toFloatWithSSF(m_xSf);

    if (m_dataPoints.value("Y5").isValid())
        m_y5 = m_dataPoints.value("Y5").toFloatWithSSF(m_ySf);

    if (m_dataPoints.value("X6").isValid())
        m_x6 = m_dataPoints.value("X6").toFloatWithSSF(m_xSf);

    if (m_dataPoints.value("Y6").isValid())
        m_y6 = m_dataPoints.value("Y6").toFloatWithSSF(m_ySf);

    if (m_dataPoints.value("X7").isValid())
        m_x7 = m_dataPoints.value("X7").toFloatWithSSF(m_xSf);

    if (m_dataPoints.value("Y7").isValid())
        m_y7 = m_dataPoints.value("Y7").toFloatWithSSF(m_ySf);

    if (m_dataPoints.value("X8").isValid())
        m_x8 = m_dataPoints.value("X8").toFloatWithSSF(m_xSf);

    if (m_dataPoints.value("Y8").isValid())
        m_y8 = m_dataPoints.value("Y8").toFloatWithSSF(m_ySf);

    if (m_dataPoints.value("X9").isValid())
        m_x9 = m_dataPoints.value("X9").toFloatWithSSF(m_xSf);

    if (m_dataPoints.value("Y9").isValid())
        m_y9 = m_dataPoints.value("Y9").toFloatWithSSF(m_ySf);

    if (m_dataPoints.value("X10").isValid())
        m_x10 = m_dataPoints.value("X10").toFloatWithSSF(m_xSf);

    if (m_dataPoints.value("Y10").isValid())
        m_y10 = m_dataPoints.value("Y10").toFloatWithSSF(m_ySf);

    if (m_dataPoints.value("Nam").isValid())
        m_nam = m_dataPoints.value("Nam").toString();

    if (m_dataPoints.value("WinTms").isValid())
        m_winTms = m_dataPoints.value("WinTms").toUInt16();

    if (m_dataPoints.value("RmpTms").isValid())
        m_rmpTms = m_dataPoints.value("RmpTms").toUInt16();

    if (m_dataPoints.value("ActIndx").isValid())
        m_actIndx = m_dataPoints.value("ActIndx").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}


SunSpecScheduleModel::SunSpecScheduleModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 133, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixedAndRepeating;

    initDataPoints();
}

SunSpecScheduleModel::~SunSpecScheduleModel()
{

}

QString SunSpecScheduleModel::name() const
{
    return "schedule";
}

QString SunSpecScheduleModel::description() const
{
    return "Basic Scheduling ";
}

QString SunSpecScheduleModel::label() const
{
    return "Basic Scheduling";
}

SunSpecScheduleModel::ActschdFlags SunSpecScheduleModel::actSchd() const
{
    return m_actSchd;
}

QModbusReply *SunSpecScheduleModel::setActSchd(ActschdFlags actSchd)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("ActSchd");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt32(static_cast<quint32>(actSchd));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecScheduleModel::ModenaFlags SunSpecScheduleModel::modEna() const
{
    return m_modEna;
}

QModbusReply *SunSpecScheduleModel::setModEna(ModenaFlags modEna)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("ModEna");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(modEna));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecScheduleModel::nSchd() const
{
    return m_nSchd;
}
quint16 SunSpecScheduleModel::nPts() const
{
    return m_nPts;
}
quint16 SunSpecScheduleModel::pad() const
{
    return m_pad;
}
void SunSpecScheduleModel::initDataPoints()
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

    SunSpecDataPoint actSchdDataPoint;
    actSchdDataPoint.setName("ActSchd");
    actSchdDataPoint.setLabel("ActSchd");
    actSchdDataPoint.setDescription("Bitfield of active schedules");
    actSchdDataPoint.setMandatory(true);
    actSchdDataPoint.setSize(2);
    actSchdDataPoint.setAddressOffset(2);
    actSchdDataPoint.setBlockOffset(0);
    actSchdDataPoint.setSunSpecDataType("bitfield32");
    actSchdDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    actSchdDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(actSchdDataPoint.name(), actSchdDataPoint);

    SunSpecDataPoint modEnaDataPoint;
    modEnaDataPoint.setName("ModEna");
    modEnaDataPoint.setLabel("ModEna");
    modEnaDataPoint.setDescription("Is basic scheduling active.");
    modEnaDataPoint.setMandatory(true);
    modEnaDataPoint.setSize(1);
    modEnaDataPoint.setAddressOffset(4);
    modEnaDataPoint.setBlockOffset(2);
    modEnaDataPoint.setSunSpecDataType("bitfield16");
    modEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    modEnaDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(modEnaDataPoint.name(), modEnaDataPoint);

    SunSpecDataPoint nSchdDataPoint;
    nSchdDataPoint.setName("NSchd");
    nSchdDataPoint.setLabel("NSchd");
    nSchdDataPoint.setDescription("Number of schedules supported (recommend min. 4, max 32)");
    nSchdDataPoint.setMandatory(true);
    nSchdDataPoint.setSize(1);
    nSchdDataPoint.setAddressOffset(5);
    nSchdDataPoint.setBlockOffset(3);
    nSchdDataPoint.setSunSpecDataType("uint16");
    nSchdDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(nSchdDataPoint.name(), nSchdDataPoint);

    SunSpecDataPoint nPtsDataPoint;
    nPtsDataPoint.setName("NPts");
    nPtsDataPoint.setLabel("NPts");
    nPtsDataPoint.setDescription("Number of schedule entries supported (maximum of 10).");
    nPtsDataPoint.setMandatory(true);
    nPtsDataPoint.setSize(1);
    nPtsDataPoint.setAddressOffset(6);
    nPtsDataPoint.setBlockOffset(4);
    nPtsDataPoint.setSunSpecDataType("uint16");
    nPtsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(nPtsDataPoint.name(), nPtsDataPoint);

    SunSpecDataPoint padDataPoint;
    padDataPoint.setName("Pad");
    padDataPoint.setLabel("Pad");
    padDataPoint.setDescription("Pad register.");
    padDataPoint.setSize(1);
    padDataPoint.setAddressOffset(7);
    padDataPoint.setBlockOffset(5);
    padDataPoint.setSunSpecDataType("pad");
    padDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(padDataPoint.name(), padDataPoint);

}

void SunSpecScheduleModel::processBlockData()
{
    // Update properties according to the data point type
    if (m_dataPoints.value("ActSchd").isValid())
        m_actSchd = static_cast<ActschdFlags>(m_dataPoints.value("ActSchd").toUInt32());

    if (m_dataPoints.value("ModEna").isValid())
        m_modEna = static_cast<ModenaFlags>(m_dataPoints.value("ModEna").toUInt16());

    if (m_dataPoints.value("NSchd").isValid())
        m_nSchd = m_dataPoints.value("NSchd").toUInt16();

    if (m_dataPoints.value("NPts").isValid())
        m_nPts = m_dataPoints.value("NPts").toUInt16();

    if (m_dataPoints.value("Pad").isValid())
        m_pad = m_dataPoints.value("Pad").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecScheduleModel *model)
{
    debug.nospace().noquote() << "SunSpecScheduleModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("ActSchd") << "-->";
    if (model->dataPoints().value("ActSchd").isValid()) {
        debug.nospace().noquote() << model->actSchd() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModEna") << "-->";
    if (model->dataPoints().value("ModEna").isValid()) {
        debug.nospace().noquote() << model->modEna() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("NSchd") << "-->";
    if (model->dataPoints().value("NSchd").isValid()) {
        debug.nospace().noquote() << model->nSchd() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("NPts") << "-->";
    if (model->dataPoints().value("NPts").isValid()) {
        debug.nospace().noquote() << model->nPts() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Pad") << "-->";
    if (model->dataPoints().value("Pad").isValid()) {
        debug.nospace().noquote() << model->pad() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
