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

#include "sunspeccontrolsmodel.h"
#include "sunspecconnection.h"

SunSpecControlsModel::SunSpecControlsModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 123, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixed;

    initDataPoints();
}

SunSpecControlsModel::~SunSpecControlsModel()
{

}

QString SunSpecControlsModel::name() const
{
    return "controls";
}

QString SunSpecControlsModel::description() const
{
    return "Immediate Inverter Controls ";
}

QString SunSpecControlsModel::label() const
{
    return "Immediate Controls";
}

quint16 SunSpecControlsModel::connWinTms() const
{
    return m_connWinTms;
}

QModbusReply *SunSpecControlsModel::setConnWinTms(quint16 connWinTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("Conn_WinTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(connWinTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecControlsModel::connRvrtTms() const
{
    return m_connRvrtTms;
}

QModbusReply *SunSpecControlsModel::setConnRvrtTms(quint16 connRvrtTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("Conn_RvrtTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(connRvrtTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecControlsModel::Conn SunSpecControlsModel::conn() const
{
    return m_conn;
}

QModbusReply *SunSpecControlsModel::setConn(Conn conn)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("Conn");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(conn));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecControlsModel::wMaxLimPct() const
{
    return m_wMaxLimPct;
}

QModbusReply *SunSpecControlsModel::setWMaxLimPct(float wMaxLimPct)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("WMaxLimPct");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(wMaxLimPct, m_wMaxLimPctSf, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecControlsModel::wMaxLimPctWinTms() const
{
    return m_wMaxLimPctWinTms;
}

QModbusReply *SunSpecControlsModel::setWMaxLimPctWinTms(quint16 wMaxLimPctWinTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("WMaxLimPct_WinTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(wMaxLimPctWinTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecControlsModel::wMaxLimPctRvrtTms() const
{
    return m_wMaxLimPctRvrtTms;
}

QModbusReply *SunSpecControlsModel::setWMaxLimPctRvrtTms(quint16 wMaxLimPctRvrtTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("WMaxLimPct_RvrtTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(wMaxLimPctRvrtTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecControlsModel::wMaxLimPctRmpTms() const
{
    return m_wMaxLimPctRmpTms;
}

QModbusReply *SunSpecControlsModel::setWMaxLimPctRmpTms(quint16 wMaxLimPctRmpTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("WMaxLimPct_RmpTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(wMaxLimPctRmpTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecControlsModel::Wmaxlim_ena SunSpecControlsModel::wMaxLimEna() const
{
    return m_wMaxLimEna;
}

QModbusReply *SunSpecControlsModel::setWMaxLimEna(Wmaxlim_ena wMaxLimEna)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("WMaxLim_Ena");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(wMaxLimEna));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecControlsModel::outPfSet() const
{
    return m_outPfSet;
}

QModbusReply *SunSpecControlsModel::setOutPfSet(float outPfSet)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("OutPFSet");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(outPfSet, m_outPfSetSf, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecControlsModel::outPfSetWinTms() const
{
    return m_outPfSetWinTms;
}

QModbusReply *SunSpecControlsModel::setOutPfSetWinTms(quint16 outPfSetWinTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("OutPFSet_WinTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(outPfSetWinTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecControlsModel::outPfSetRvrtTms() const
{
    return m_outPfSetRvrtTms;
}

QModbusReply *SunSpecControlsModel::setOutPfSetRvrtTms(quint16 outPfSetRvrtTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("OutPFSet_RvrtTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(outPfSetRvrtTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecControlsModel::outPfSetRmpTms() const
{
    return m_outPfSetRmpTms;
}

QModbusReply *SunSpecControlsModel::setOutPfSetRmpTms(quint16 outPfSetRmpTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("OutPFSet_RmpTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(outPfSetRmpTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecControlsModel::Outpfset_ena SunSpecControlsModel::outPfSetEna() const
{
    return m_outPfSetEna;
}

QModbusReply *SunSpecControlsModel::setOutPfSetEna(Outpfset_ena outPfSetEna)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("OutPFSet_Ena");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(outPfSetEna));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecControlsModel::vArWMaxPct() const
{
    return m_vArWMaxPct;
}

QModbusReply *SunSpecControlsModel::setVArWMaxPct(float vArWMaxPct)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("VArWMaxPct");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vArWMaxPct, m_vArPctSf, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecControlsModel::vArMaxPct() const
{
    return m_vArMaxPct;
}

QModbusReply *SunSpecControlsModel::setVArMaxPct(float vArMaxPct)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("VArMaxPct");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vArMaxPct, m_vArPctSf, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecControlsModel::vArAvalPct() const
{
    return m_vArAvalPct;
}

QModbusReply *SunSpecControlsModel::setVArAvalPct(float vArAvalPct)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("VArAvalPct");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vArAvalPct, m_vArPctSf, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecControlsModel::vArPctWinTms() const
{
    return m_vArPctWinTms;
}

QModbusReply *SunSpecControlsModel::setVArPctWinTms(quint16 vArPctWinTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("VArPct_WinTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(vArPctWinTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecControlsModel::vArPctRvrtTms() const
{
    return m_vArPctRvrtTms;
}

QModbusReply *SunSpecControlsModel::setVArPctRvrtTms(quint16 vArPctRvrtTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("VArPct_RvrtTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(vArPctRvrtTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecControlsModel::vArPctRmpTms() const
{
    return m_vArPctRmpTms;
}

QModbusReply *SunSpecControlsModel::setVArPctRmpTms(quint16 vArPctRmpTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("VArPct_RmpTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(vArPctRmpTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecControlsModel::Varpct_mod SunSpecControlsModel::vArPctMod() const
{
    return m_vArPctMod;
}

QModbusReply *SunSpecControlsModel::setVArPctMod(Varpct_mod vArPctMod)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("VArPct_Mod");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(vArPctMod));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecControlsModel::Varpct_ena SunSpecControlsModel::vArPctEna() const
{
    return m_vArPctEna;
}

QModbusReply *SunSpecControlsModel::setVArPctEna(Varpct_ena vArPctEna)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("VArPct_Ena");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(vArPctEna));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
qint16 SunSpecControlsModel::wMaxLimPctSf() const
{
    return m_wMaxLimPctSf;
}
qint16 SunSpecControlsModel::outPfSetSf() const
{
    return m_outPfSetSf;
}
qint16 SunSpecControlsModel::vArPctSf() const
{
    return m_vArPctSf;
}
void SunSpecControlsModel::initDataPoints()
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

    SunSpecDataPoint connWinTmsDataPoint;
    connWinTmsDataPoint.setName("Conn_WinTms");
    connWinTmsDataPoint.setLabel("Conn_WinTms");
    connWinTmsDataPoint.setDescription("Time window for connect/disconnect.");
    connWinTmsDataPoint.setUnits("Secs");
    connWinTmsDataPoint.setSize(1);
    connWinTmsDataPoint.setAddressOffset(2);
    connWinTmsDataPoint.setBlockOffset(0);
    connWinTmsDataPoint.setSunSpecDataType("uint16");
    connWinTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    connWinTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(connWinTmsDataPoint.name(), connWinTmsDataPoint);

    SunSpecDataPoint connRvrtTmsDataPoint;
    connRvrtTmsDataPoint.setName("Conn_RvrtTms");
    connRvrtTmsDataPoint.setLabel("Conn_RvrtTms");
    connRvrtTmsDataPoint.setDescription("Timeout period for connect/disconnect.");
    connRvrtTmsDataPoint.setUnits("Secs");
    connRvrtTmsDataPoint.setSize(1);
    connRvrtTmsDataPoint.setAddressOffset(3);
    connRvrtTmsDataPoint.setBlockOffset(1);
    connRvrtTmsDataPoint.setSunSpecDataType("uint16");
    connRvrtTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    connRvrtTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(connRvrtTmsDataPoint.name(), connRvrtTmsDataPoint);

    SunSpecDataPoint connDataPoint;
    connDataPoint.setName("Conn");
    connDataPoint.setLabel("Conn");
    connDataPoint.setDescription("Enumerated valued.  Connection control.");
    connDataPoint.setMandatory(true);
    connDataPoint.setSize(1);
    connDataPoint.setAddressOffset(4);
    connDataPoint.setBlockOffset(2);
    connDataPoint.setSunSpecDataType("enum16");
    connDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    connDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(connDataPoint.name(), connDataPoint);

    SunSpecDataPoint wMaxLimPctDataPoint;
    wMaxLimPctDataPoint.setName("WMaxLimPct");
    wMaxLimPctDataPoint.setLabel("WMaxLimPct");
    wMaxLimPctDataPoint.setDescription("Set power output to specified level.");
    wMaxLimPctDataPoint.setUnits("% WMax");
    wMaxLimPctDataPoint.setMandatory(true);
    wMaxLimPctDataPoint.setSize(1);
    wMaxLimPctDataPoint.setAddressOffset(5);
    wMaxLimPctDataPoint.setBlockOffset(3);
    wMaxLimPctDataPoint.setScaleFactorName("WMaxLimPct_SF");
    wMaxLimPctDataPoint.setSunSpecDataType("uint16");
    wMaxLimPctDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    wMaxLimPctDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wMaxLimPctDataPoint.name(), wMaxLimPctDataPoint);

    SunSpecDataPoint wMaxLimPctWinTmsDataPoint;
    wMaxLimPctWinTmsDataPoint.setName("WMaxLimPct_WinTms");
    wMaxLimPctWinTmsDataPoint.setLabel("WMaxLimPct_WinTms");
    wMaxLimPctWinTmsDataPoint.setDescription("Time window for power limit change.");
    wMaxLimPctWinTmsDataPoint.setUnits("Secs");
    wMaxLimPctWinTmsDataPoint.setSize(1);
    wMaxLimPctWinTmsDataPoint.setAddressOffset(6);
    wMaxLimPctWinTmsDataPoint.setBlockOffset(4);
    wMaxLimPctWinTmsDataPoint.setSunSpecDataType("uint16");
    wMaxLimPctWinTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    wMaxLimPctWinTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wMaxLimPctWinTmsDataPoint.name(), wMaxLimPctWinTmsDataPoint);

    SunSpecDataPoint wMaxLimPctRvrtTmsDataPoint;
    wMaxLimPctRvrtTmsDataPoint.setName("WMaxLimPct_RvrtTms");
    wMaxLimPctRvrtTmsDataPoint.setLabel("WMaxLimPct_RvrtTms");
    wMaxLimPctRvrtTmsDataPoint.setDescription("Timeout period for power limit.");
    wMaxLimPctRvrtTmsDataPoint.setUnits("Secs");
    wMaxLimPctRvrtTmsDataPoint.setSize(1);
    wMaxLimPctRvrtTmsDataPoint.setAddressOffset(7);
    wMaxLimPctRvrtTmsDataPoint.setBlockOffset(5);
    wMaxLimPctRvrtTmsDataPoint.setSunSpecDataType("uint16");
    wMaxLimPctRvrtTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    wMaxLimPctRvrtTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wMaxLimPctRvrtTmsDataPoint.name(), wMaxLimPctRvrtTmsDataPoint);

    SunSpecDataPoint wMaxLimPctRmpTmsDataPoint;
    wMaxLimPctRmpTmsDataPoint.setName("WMaxLimPct_RmpTms");
    wMaxLimPctRmpTmsDataPoint.setLabel("WMaxLimPct_RmpTms");
    wMaxLimPctRmpTmsDataPoint.setDescription("Ramp time for moving from current setpoint to new setpoint.");
    wMaxLimPctRmpTmsDataPoint.setUnits("Secs");
    wMaxLimPctRmpTmsDataPoint.setSize(1);
    wMaxLimPctRmpTmsDataPoint.setAddressOffset(8);
    wMaxLimPctRmpTmsDataPoint.setBlockOffset(6);
    wMaxLimPctRmpTmsDataPoint.setSunSpecDataType("uint16");
    wMaxLimPctRmpTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    wMaxLimPctRmpTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wMaxLimPctRmpTmsDataPoint.name(), wMaxLimPctRmpTmsDataPoint);

    SunSpecDataPoint wMaxLimEnaDataPoint;
    wMaxLimEnaDataPoint.setName("WMaxLim_Ena");
    wMaxLimEnaDataPoint.setLabel("WMaxLim_Ena");
    wMaxLimEnaDataPoint.setDescription("Enumerated valued.  Throttle enable/disable control.");
    wMaxLimEnaDataPoint.setMandatory(true);
    wMaxLimEnaDataPoint.setSize(1);
    wMaxLimEnaDataPoint.setAddressOffset(9);
    wMaxLimEnaDataPoint.setBlockOffset(7);
    wMaxLimEnaDataPoint.setSunSpecDataType("enum16");
    wMaxLimEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    wMaxLimEnaDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wMaxLimEnaDataPoint.name(), wMaxLimEnaDataPoint);

    SunSpecDataPoint outPfSetDataPoint;
    outPfSetDataPoint.setName("OutPFSet");
    outPfSetDataPoint.setLabel("OutPFSet");
    outPfSetDataPoint.setDescription("Set power factor to specific value - cosine of angle.");
    outPfSetDataPoint.setUnits("cos()");
    outPfSetDataPoint.setMandatory(true);
    outPfSetDataPoint.setSize(1);
    outPfSetDataPoint.setAddressOffset(10);
    outPfSetDataPoint.setBlockOffset(8);
    outPfSetDataPoint.setScaleFactorName("OutPFSet_SF");
    outPfSetDataPoint.setSunSpecDataType("int16");
    outPfSetDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    outPfSetDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(outPfSetDataPoint.name(), outPfSetDataPoint);

    SunSpecDataPoint outPfSetWinTmsDataPoint;
    outPfSetWinTmsDataPoint.setName("OutPFSet_WinTms");
    outPfSetWinTmsDataPoint.setLabel("OutPFSet_WinTms");
    outPfSetWinTmsDataPoint.setDescription("Time window for power factor change.");
    outPfSetWinTmsDataPoint.setUnits("Secs");
    outPfSetWinTmsDataPoint.setSize(1);
    outPfSetWinTmsDataPoint.setAddressOffset(11);
    outPfSetWinTmsDataPoint.setBlockOffset(9);
    outPfSetWinTmsDataPoint.setSunSpecDataType("uint16");
    outPfSetWinTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    outPfSetWinTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(outPfSetWinTmsDataPoint.name(), outPfSetWinTmsDataPoint);

    SunSpecDataPoint outPfSetRvrtTmsDataPoint;
    outPfSetRvrtTmsDataPoint.setName("OutPFSet_RvrtTms");
    outPfSetRvrtTmsDataPoint.setLabel("OutPFSet_RvrtTms");
    outPfSetRvrtTmsDataPoint.setDescription("Timeout period for power factor.");
    outPfSetRvrtTmsDataPoint.setUnits("Secs");
    outPfSetRvrtTmsDataPoint.setSize(1);
    outPfSetRvrtTmsDataPoint.setAddressOffset(12);
    outPfSetRvrtTmsDataPoint.setBlockOffset(10);
    outPfSetRvrtTmsDataPoint.setSunSpecDataType("uint16");
    outPfSetRvrtTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    outPfSetRvrtTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(outPfSetRvrtTmsDataPoint.name(), outPfSetRvrtTmsDataPoint);

    SunSpecDataPoint outPfSetRmpTmsDataPoint;
    outPfSetRmpTmsDataPoint.setName("OutPFSet_RmpTms");
    outPfSetRmpTmsDataPoint.setLabel("OutPFSet_RmpTms");
    outPfSetRmpTmsDataPoint.setDescription("Ramp time for moving from current setpoint to new setpoint.");
    outPfSetRmpTmsDataPoint.setUnits("Secs");
    outPfSetRmpTmsDataPoint.setSize(1);
    outPfSetRmpTmsDataPoint.setAddressOffset(13);
    outPfSetRmpTmsDataPoint.setBlockOffset(11);
    outPfSetRmpTmsDataPoint.setSunSpecDataType("uint16");
    outPfSetRmpTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    outPfSetRmpTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(outPfSetRmpTmsDataPoint.name(), outPfSetRmpTmsDataPoint);

    SunSpecDataPoint outPfSetEnaDataPoint;
    outPfSetEnaDataPoint.setName("OutPFSet_Ena");
    outPfSetEnaDataPoint.setLabel("OutPFSet_Ena");
    outPfSetEnaDataPoint.setDescription("Enumerated valued.  Fixed power factor enable/disable control.");
    outPfSetEnaDataPoint.setMandatory(true);
    outPfSetEnaDataPoint.setSize(1);
    outPfSetEnaDataPoint.setAddressOffset(14);
    outPfSetEnaDataPoint.setBlockOffset(12);
    outPfSetEnaDataPoint.setSunSpecDataType("enum16");
    outPfSetEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    outPfSetEnaDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(outPfSetEnaDataPoint.name(), outPfSetEnaDataPoint);

    SunSpecDataPoint vArWMaxPctDataPoint;
    vArWMaxPctDataPoint.setName("VArWMaxPct");
    vArWMaxPctDataPoint.setLabel("VArWMaxPct");
    vArWMaxPctDataPoint.setDescription("Reactive power in percent of WMax.");
    vArWMaxPctDataPoint.setUnits("% WMax");
    vArWMaxPctDataPoint.setSize(1);
    vArWMaxPctDataPoint.setAddressOffset(15);
    vArWMaxPctDataPoint.setBlockOffset(13);
    vArWMaxPctDataPoint.setScaleFactorName("VArPct_SF");
    vArWMaxPctDataPoint.setSunSpecDataType("int16");
    vArWMaxPctDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vArWMaxPctDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vArWMaxPctDataPoint.name(), vArWMaxPctDataPoint);

    SunSpecDataPoint vArMaxPctDataPoint;
    vArMaxPctDataPoint.setName("VArMaxPct");
    vArMaxPctDataPoint.setLabel("VArMaxPct");
    vArMaxPctDataPoint.setDescription("Reactive power in percent of VArMax.");
    vArMaxPctDataPoint.setUnits("% VArMax");
    vArMaxPctDataPoint.setSize(1);
    vArMaxPctDataPoint.setAddressOffset(16);
    vArMaxPctDataPoint.setBlockOffset(14);
    vArMaxPctDataPoint.setScaleFactorName("VArPct_SF");
    vArMaxPctDataPoint.setSunSpecDataType("int16");
    vArMaxPctDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vArMaxPctDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vArMaxPctDataPoint.name(), vArMaxPctDataPoint);

    SunSpecDataPoint vArAvalPctDataPoint;
    vArAvalPctDataPoint.setName("VArAvalPct");
    vArAvalPctDataPoint.setLabel("VArAvalPct");
    vArAvalPctDataPoint.setDescription("Reactive power in percent of VArAval.");
    vArAvalPctDataPoint.setUnits("% VArAval");
    vArAvalPctDataPoint.setSize(1);
    vArAvalPctDataPoint.setAddressOffset(17);
    vArAvalPctDataPoint.setBlockOffset(15);
    vArAvalPctDataPoint.setScaleFactorName("VArPct_SF");
    vArAvalPctDataPoint.setSunSpecDataType("int16");
    vArAvalPctDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vArAvalPctDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vArAvalPctDataPoint.name(), vArAvalPctDataPoint);

    SunSpecDataPoint vArPctWinTmsDataPoint;
    vArPctWinTmsDataPoint.setName("VArPct_WinTms");
    vArPctWinTmsDataPoint.setLabel("VArPct_WinTms");
    vArPctWinTmsDataPoint.setDescription("Time window for VAR limit change.");
    vArPctWinTmsDataPoint.setUnits("Secs");
    vArPctWinTmsDataPoint.setSize(1);
    vArPctWinTmsDataPoint.setAddressOffset(18);
    vArPctWinTmsDataPoint.setBlockOffset(16);
    vArPctWinTmsDataPoint.setSunSpecDataType("uint16");
    vArPctWinTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vArPctWinTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vArPctWinTmsDataPoint.name(), vArPctWinTmsDataPoint);

    SunSpecDataPoint vArPctRvrtTmsDataPoint;
    vArPctRvrtTmsDataPoint.setName("VArPct_RvrtTms");
    vArPctRvrtTmsDataPoint.setLabel("VArPct_RvrtTms");
    vArPctRvrtTmsDataPoint.setDescription("Timeout period for VAR limit.");
    vArPctRvrtTmsDataPoint.setUnits("Secs");
    vArPctRvrtTmsDataPoint.setSize(1);
    vArPctRvrtTmsDataPoint.setAddressOffset(19);
    vArPctRvrtTmsDataPoint.setBlockOffset(17);
    vArPctRvrtTmsDataPoint.setSunSpecDataType("uint16");
    vArPctRvrtTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vArPctRvrtTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vArPctRvrtTmsDataPoint.name(), vArPctRvrtTmsDataPoint);

    SunSpecDataPoint vArPctRmpTmsDataPoint;
    vArPctRmpTmsDataPoint.setName("VArPct_RmpTms");
    vArPctRmpTmsDataPoint.setLabel("VArPct_RmpTms");
    vArPctRmpTmsDataPoint.setDescription("Ramp time for moving from current setpoint to new setpoint.");
    vArPctRmpTmsDataPoint.setUnits("Secs");
    vArPctRmpTmsDataPoint.setSize(1);
    vArPctRmpTmsDataPoint.setAddressOffset(20);
    vArPctRmpTmsDataPoint.setBlockOffset(18);
    vArPctRmpTmsDataPoint.setSunSpecDataType("uint16");
    vArPctRmpTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vArPctRmpTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vArPctRmpTmsDataPoint.name(), vArPctRmpTmsDataPoint);

    SunSpecDataPoint vArPctModDataPoint;
    vArPctModDataPoint.setName("VArPct_Mod");
    vArPctModDataPoint.setLabel("VArPct_Mod");
    vArPctModDataPoint.setDescription("Enumerated value. VAR percent limit mode.");
    vArPctModDataPoint.setSize(1);
    vArPctModDataPoint.setAddressOffset(21);
    vArPctModDataPoint.setBlockOffset(19);
    vArPctModDataPoint.setSunSpecDataType("enum16");
    vArPctModDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vArPctModDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vArPctModDataPoint.name(), vArPctModDataPoint);

    SunSpecDataPoint vArPctEnaDataPoint;
    vArPctEnaDataPoint.setName("VArPct_Ena");
    vArPctEnaDataPoint.setLabel("VArPct_Ena");
    vArPctEnaDataPoint.setDescription("Enumerated valued.  Percent limit VAr enable/disable control.");
    vArPctEnaDataPoint.setMandatory(true);
    vArPctEnaDataPoint.setSize(1);
    vArPctEnaDataPoint.setAddressOffset(22);
    vArPctEnaDataPoint.setBlockOffset(20);
    vArPctEnaDataPoint.setSunSpecDataType("enum16");
    vArPctEnaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vArPctEnaDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vArPctEnaDataPoint.name(), vArPctEnaDataPoint);

    SunSpecDataPoint wMaxLimPctSfDataPoint;
    wMaxLimPctSfDataPoint.setName("WMaxLimPct_SF");
    wMaxLimPctSfDataPoint.setLabel("WMaxLimPct_SF");
    wMaxLimPctSfDataPoint.setDescription("Scale factor for power output percent.");
    wMaxLimPctSfDataPoint.setMandatory(true);
    wMaxLimPctSfDataPoint.setSize(1);
    wMaxLimPctSfDataPoint.setAddressOffset(23);
    wMaxLimPctSfDataPoint.setBlockOffset(21);
    wMaxLimPctSfDataPoint.setSunSpecDataType("sunssf");
    wMaxLimPctSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wMaxLimPctSfDataPoint.name(), wMaxLimPctSfDataPoint);

    SunSpecDataPoint outPfSetSfDataPoint;
    outPfSetSfDataPoint.setName("OutPFSet_SF");
    outPfSetSfDataPoint.setLabel("OutPFSet_SF");
    outPfSetSfDataPoint.setDescription("Scale factor for power factor.");
    outPfSetSfDataPoint.setMandatory(true);
    outPfSetSfDataPoint.setSize(1);
    outPfSetSfDataPoint.setAddressOffset(24);
    outPfSetSfDataPoint.setBlockOffset(22);
    outPfSetSfDataPoint.setSunSpecDataType("sunssf");
    outPfSetSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(outPfSetSfDataPoint.name(), outPfSetSfDataPoint);

    SunSpecDataPoint vArPctSfDataPoint;
    vArPctSfDataPoint.setName("VArPct_SF");
    vArPctSfDataPoint.setLabel("VArPct_SF");
    vArPctSfDataPoint.setDescription("Scale factor for reactive power percent.");
    vArPctSfDataPoint.setSize(1);
    vArPctSfDataPoint.setAddressOffset(25);
    vArPctSfDataPoint.setBlockOffset(23);
    vArPctSfDataPoint.setSunSpecDataType("sunssf");
    vArPctSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vArPctSfDataPoint.name(), vArPctSfDataPoint);

}

void SunSpecControlsModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("WMaxLimPct_SF").isValid())
        m_wMaxLimPctSf = m_dataPoints.value("WMaxLimPct_SF").toInt16();

    if (m_dataPoints.value("OutPFSet_SF").isValid())
        m_outPfSetSf = m_dataPoints.value("OutPFSet_SF").toInt16();

    if (m_dataPoints.value("VArPct_SF").isValid())
        m_vArPctSf = m_dataPoints.value("VArPct_SF").toInt16();


    // Update properties according to the data point type
    if (m_dataPoints.value("Conn_WinTms").isValid())
        m_connWinTms = m_dataPoints.value("Conn_WinTms").toUInt16();

    if (m_dataPoints.value("Conn_RvrtTms").isValid())
        m_connRvrtTms = m_dataPoints.value("Conn_RvrtTms").toUInt16();

    if (m_dataPoints.value("Conn").isValid())
        m_conn = static_cast<Conn>(m_dataPoints.value("Conn").toUInt16());

    if (m_dataPoints.value("WMaxLimPct").isValid())
        m_wMaxLimPct = m_dataPoints.value("WMaxLimPct").toFloatWithSSF(m_wMaxLimPctSf);

    if (m_dataPoints.value("WMaxLimPct_WinTms").isValid())
        m_wMaxLimPctWinTms = m_dataPoints.value("WMaxLimPct_WinTms").toUInt16();

    if (m_dataPoints.value("WMaxLimPct_RvrtTms").isValid())
        m_wMaxLimPctRvrtTms = m_dataPoints.value("WMaxLimPct_RvrtTms").toUInt16();

    if (m_dataPoints.value("WMaxLimPct_RmpTms").isValid())
        m_wMaxLimPctRmpTms = m_dataPoints.value("WMaxLimPct_RmpTms").toUInt16();

    if (m_dataPoints.value("WMaxLim_Ena").isValid())
        m_wMaxLimEna = static_cast<Wmaxlim_ena>(m_dataPoints.value("WMaxLim_Ena").toUInt16());

    if (m_dataPoints.value("OutPFSet").isValid())
        m_outPfSet = m_dataPoints.value("OutPFSet").toFloatWithSSF(m_outPfSetSf);

    if (m_dataPoints.value("OutPFSet_WinTms").isValid())
        m_outPfSetWinTms = m_dataPoints.value("OutPFSet_WinTms").toUInt16();

    if (m_dataPoints.value("OutPFSet_RvrtTms").isValid())
        m_outPfSetRvrtTms = m_dataPoints.value("OutPFSet_RvrtTms").toUInt16();

    if (m_dataPoints.value("OutPFSet_RmpTms").isValid())
        m_outPfSetRmpTms = m_dataPoints.value("OutPFSet_RmpTms").toUInt16();

    if (m_dataPoints.value("OutPFSet_Ena").isValid())
        m_outPfSetEna = static_cast<Outpfset_ena>(m_dataPoints.value("OutPFSet_Ena").toUInt16());

    if (m_dataPoints.value("VArWMaxPct").isValid())
        m_vArWMaxPct = m_dataPoints.value("VArWMaxPct").toFloatWithSSF(m_vArPctSf);

    if (m_dataPoints.value("VArMaxPct").isValid())
        m_vArMaxPct = m_dataPoints.value("VArMaxPct").toFloatWithSSF(m_vArPctSf);

    if (m_dataPoints.value("VArAvalPct").isValid())
        m_vArAvalPct = m_dataPoints.value("VArAvalPct").toFloatWithSSF(m_vArPctSf);

    if (m_dataPoints.value("VArPct_WinTms").isValid())
        m_vArPctWinTms = m_dataPoints.value("VArPct_WinTms").toUInt16();

    if (m_dataPoints.value("VArPct_RvrtTms").isValid())
        m_vArPctRvrtTms = m_dataPoints.value("VArPct_RvrtTms").toUInt16();

    if (m_dataPoints.value("VArPct_RmpTms").isValid())
        m_vArPctRmpTms = m_dataPoints.value("VArPct_RmpTms").toUInt16();

    if (m_dataPoints.value("VArPct_Mod").isValid())
        m_vArPctMod = static_cast<Varpct_mod>(m_dataPoints.value("VArPct_Mod").toUInt16());

    if (m_dataPoints.value("VArPct_Ena").isValid())
        m_vArPctEna = static_cast<Varpct_ena>(m_dataPoints.value("VArPct_Ena").toUInt16());

    if (m_dataPoints.value("WMaxLimPct_SF").isValid())
        m_wMaxLimPctSf = m_dataPoints.value("WMaxLimPct_SF").toInt16();

    if (m_dataPoints.value("OutPFSet_SF").isValid())
        m_outPfSetSf = m_dataPoints.value("OutPFSet_SF").toInt16();

    if (m_dataPoints.value("VArPct_SF").isValid())
        m_vArPctSf = m_dataPoints.value("VArPct_SF").toInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecControlsModel *model)
{
    debug.nospace().noquote() << "SunSpecControlsModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("Conn_WinTms") << "-->";
    if (model->dataPoints().value("Conn_WinTms").isValid()) {
        debug.nospace().noquote() << model->connWinTms() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Conn_RvrtTms") << "-->";
    if (model->dataPoints().value("Conn_RvrtTms").isValid()) {
        debug.nospace().noquote() << model->connRvrtTms() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Conn") << "-->";
    if (model->dataPoints().value("Conn").isValid()) {
        debug.nospace().noquote() << model->conn() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WMaxLimPct") << "-->";
    if (model->dataPoints().value("WMaxLimPct").isValid()) {
        debug.nospace().noquote() << model->wMaxLimPct() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WMaxLimPct_WinTms") << "-->";
    if (model->dataPoints().value("WMaxLimPct_WinTms").isValid()) {
        debug.nospace().noquote() << model->wMaxLimPctWinTms() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WMaxLimPct_RvrtTms") << "-->";
    if (model->dataPoints().value("WMaxLimPct_RvrtTms").isValid()) {
        debug.nospace().noquote() << model->wMaxLimPctRvrtTms() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WMaxLimPct_RmpTms") << "-->";
    if (model->dataPoints().value("WMaxLimPct_RmpTms").isValid()) {
        debug.nospace().noquote() << model->wMaxLimPctRmpTms() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WMaxLim_Ena") << "-->";
    if (model->dataPoints().value("WMaxLim_Ena").isValid()) {
        debug.nospace().noquote() << model->wMaxLimEna() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("OutPFSet") << "-->";
    if (model->dataPoints().value("OutPFSet").isValid()) {
        debug.nospace().noquote() << model->outPfSet() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("OutPFSet_WinTms") << "-->";
    if (model->dataPoints().value("OutPFSet_WinTms").isValid()) {
        debug.nospace().noquote() << model->outPfSetWinTms() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("OutPFSet_RvrtTms") << "-->";
    if (model->dataPoints().value("OutPFSet_RvrtTms").isValid()) {
        debug.nospace().noquote() << model->outPfSetRvrtTms() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("OutPFSet_RmpTms") << "-->";
    if (model->dataPoints().value("OutPFSet_RmpTms").isValid()) {
        debug.nospace().noquote() << model->outPfSetRmpTms() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("OutPFSet_Ena") << "-->";
    if (model->dataPoints().value("OutPFSet_Ena").isValid()) {
        debug.nospace().noquote() << model->outPfSetEna() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VArWMaxPct") << "-->";
    if (model->dataPoints().value("VArWMaxPct").isValid()) {
        debug.nospace().noquote() << model->vArWMaxPct() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VArMaxPct") << "-->";
    if (model->dataPoints().value("VArMaxPct").isValid()) {
        debug.nospace().noquote() << model->vArMaxPct() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VArAvalPct") << "-->";
    if (model->dataPoints().value("VArAvalPct").isValid()) {
        debug.nospace().noquote() << model->vArAvalPct() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VArPct_WinTms") << "-->";
    if (model->dataPoints().value("VArPct_WinTms").isValid()) {
        debug.nospace().noquote() << model->vArPctWinTms() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VArPct_RvrtTms") << "-->";
    if (model->dataPoints().value("VArPct_RvrtTms").isValid()) {
        debug.nospace().noquote() << model->vArPctRvrtTms() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VArPct_RmpTms") << "-->";
    if (model->dataPoints().value("VArPct_RmpTms").isValid()) {
        debug.nospace().noquote() << model->vArPctRmpTms() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VArPct_Mod") << "-->";
    if (model->dataPoints().value("VArPct_Mod").isValid()) {
        debug.nospace().noquote() << model->vArPctMod() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VArPct_Ena") << "-->";
    if (model->dataPoints().value("VArPct_Ena").isValid()) {
        debug.nospace().noquote() << model->vArPctEna() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
