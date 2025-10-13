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

#include "sunspecstoragemodel.h"
#include "sunspecconnection.h"

SunSpecStorageModel::SunSpecStorageModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 124, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixed;

    initDataPoints();
}

SunSpecStorageModel::~SunSpecStorageModel()
{

}

QString SunSpecStorageModel::name() const
{
    return "storage";
}

QString SunSpecStorageModel::description() const
{
    return "Basic Storage Controls ";
}

QString SunSpecStorageModel::label() const
{
    return "Storage";
}

float SunSpecStorageModel::wChaMax() const
{
    return m_wChaMax;
}

QModbusReply *SunSpecStorageModel::setWChaMax(float wChaMax)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("WChaMax");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(wChaMax, m_wChaMaxSf, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecStorageModel::wChaGra() const
{
    return m_wChaGra;
}

QModbusReply *SunSpecStorageModel::setWChaGra(float wChaGra)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("WChaGra");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(wChaGra, m_wChaDisChaGraSf, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecStorageModel::wDisChaGra() const
{
    return m_wDisChaGra;
}

QModbusReply *SunSpecStorageModel::setWDisChaGra(float wDisChaGra)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("WDisChaGra");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(wDisChaGra, m_wChaDisChaGraSf, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecStorageModel::Storctl_modFlags SunSpecStorageModel::storCtlMod() const
{
    return m_storCtlMod;
}

QModbusReply *SunSpecStorageModel::setStorCtlMod(Storctl_modFlags storCtlMod)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("StorCtl_Mod");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(storCtlMod));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecStorageModel::vaChaMax() const
{
    return m_vaChaMax;
}

QModbusReply *SunSpecStorageModel::setVaChaMax(float vaChaMax)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("VAChaMax");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(vaChaMax, m_vaChaMaxSf, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecStorageModel::minRsvPct() const
{
    return m_minRsvPct;
}

QModbusReply *SunSpecStorageModel::setMinRsvPct(float minRsvPct)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("MinRsvPct");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(minRsvPct, m_minRsvPctSf, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecStorageModel::chaState() const
{
    return m_chaState;
}
float SunSpecStorageModel::storAval() const
{
    return m_storAval;
}
float SunSpecStorageModel::inBatV() const
{
    return m_inBatV;
}
SunSpecStorageModel::Chast SunSpecStorageModel::chaSt() const
{
    return m_chaSt;
}
float SunSpecStorageModel::outWRte() const
{
    return m_outWRte;
}

QModbusReply *SunSpecStorageModel::setOutWRte(float outWRte)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("OutWRte");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(outWRte, m_inOutWRteSf, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecStorageModel::inWRte() const
{
    return m_inWRte;
}

QModbusReply *SunSpecStorageModel::setInWRte(float inWRte)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("InWRte");
    QVector<quint16> registers = SunSpecDataPoint::convertFromFloatWithSSF(inWRte, m_inOutWRteSf, dp.dataType());

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecStorageModel::inOutWRteWinTms() const
{
    return m_inOutWRteWinTms;
}

QModbusReply *SunSpecStorageModel::setInOutWRteWinTms(quint16 inOutWRteWinTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("InOutWRte_WinTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(inOutWRteWinTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecStorageModel::inOutWRteRvrtTms() const
{
    return m_inOutWRteRvrtTms;
}

QModbusReply *SunSpecStorageModel::setInOutWRteRvrtTms(quint16 inOutWRteRvrtTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("InOutWRte_RvrtTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(inOutWRteRvrtTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
quint16 SunSpecStorageModel::inOutWRteRmpTms() const
{
    return m_inOutWRteRmpTms;
}

QModbusReply *SunSpecStorageModel::setInOutWRteRmpTms(quint16 inOutWRteRmpTms)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("InOutWRte_RmpTms");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(inOutWRteRmpTms);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecStorageModel::Chagriset SunSpecStorageModel::chaGriSet() const
{
    return m_chaGriSet;
}

QModbusReply *SunSpecStorageModel::setChaGriSet(Chagriset chaGriSet)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("ChaGriSet");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(chaGriSet));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
qint16 SunSpecStorageModel::wChaMaxSf() const
{
    return m_wChaMaxSf;
}
qint16 SunSpecStorageModel::wChaDisChaGraSf() const
{
    return m_wChaDisChaGraSf;
}
qint16 SunSpecStorageModel::vaChaMaxSf() const
{
    return m_vaChaMaxSf;
}
qint16 SunSpecStorageModel::minRsvPctSf() const
{
    return m_minRsvPctSf;
}
qint16 SunSpecStorageModel::chaStateSf() const
{
    return m_chaStateSf;
}
qint16 SunSpecStorageModel::storAvalSf() const
{
    return m_storAvalSf;
}
qint16 SunSpecStorageModel::inBatVSf() const
{
    return m_inBatVSf;
}
qint16 SunSpecStorageModel::inOutWRteSf() const
{
    return m_inOutWRteSf;
}
void SunSpecStorageModel::initDataPoints()
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

    SunSpecDataPoint wChaMaxDataPoint;
    wChaMaxDataPoint.setName("WChaMax");
    wChaMaxDataPoint.setLabel("WChaMax");
    wChaMaxDataPoint.setDescription("Setpoint for maximum charge.");
    wChaMaxDataPoint.setUnits("W");
    wChaMaxDataPoint.setMandatory(true);
    wChaMaxDataPoint.setSize(1);
    wChaMaxDataPoint.setAddressOffset(2);
    wChaMaxDataPoint.setBlockOffset(0);
    wChaMaxDataPoint.setScaleFactorName("WChaMax_SF");
    wChaMaxDataPoint.setSunSpecDataType("uint16");
    wChaMaxDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    wChaMaxDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wChaMaxDataPoint.name(), wChaMaxDataPoint);

    SunSpecDataPoint wChaGraDataPoint;
    wChaGraDataPoint.setName("WChaGra");
    wChaGraDataPoint.setLabel("WChaGra");
    wChaGraDataPoint.setDescription("Setpoint for maximum charging rate. Default is MaxChaRte.");
    wChaGraDataPoint.setUnits("% WChaMax/sec");
    wChaGraDataPoint.setMandatory(true);
    wChaGraDataPoint.setSize(1);
    wChaGraDataPoint.setAddressOffset(3);
    wChaGraDataPoint.setBlockOffset(1);
    wChaGraDataPoint.setScaleFactorName("WChaDisChaGra_SF");
    wChaGraDataPoint.setSunSpecDataType("uint16");
    wChaGraDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    wChaGraDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wChaGraDataPoint.name(), wChaGraDataPoint);

    SunSpecDataPoint wDisChaGraDataPoint;
    wDisChaGraDataPoint.setName("WDisChaGra");
    wDisChaGraDataPoint.setLabel("WDisChaGra");
    wDisChaGraDataPoint.setDescription("Setpoint for maximum discharge rate. Default is MaxDisChaRte.");
    wDisChaGraDataPoint.setUnits("% WChaMax/sec");
    wDisChaGraDataPoint.setMandatory(true);
    wDisChaGraDataPoint.setSize(1);
    wDisChaGraDataPoint.setAddressOffset(4);
    wDisChaGraDataPoint.setBlockOffset(2);
    wDisChaGraDataPoint.setScaleFactorName("WChaDisChaGra_SF");
    wDisChaGraDataPoint.setSunSpecDataType("uint16");
    wDisChaGraDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    wDisChaGraDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wDisChaGraDataPoint.name(), wDisChaGraDataPoint);

    SunSpecDataPoint storCtlModDataPoint;
    storCtlModDataPoint.setName("StorCtl_Mod");
    storCtlModDataPoint.setLabel("StorCtl_Mod");
    storCtlModDataPoint.setDescription("Activate hold/discharge/charge storage control mode. Bitfield value.");
    storCtlModDataPoint.setMandatory(true);
    storCtlModDataPoint.setSize(1);
    storCtlModDataPoint.setAddressOffset(5);
    storCtlModDataPoint.setBlockOffset(3);
    storCtlModDataPoint.setSunSpecDataType("bitfield16");
    storCtlModDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    storCtlModDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(storCtlModDataPoint.name(), storCtlModDataPoint);

    SunSpecDataPoint vaChaMaxDataPoint;
    vaChaMaxDataPoint.setName("VAChaMax");
    vaChaMaxDataPoint.setLabel("VAChaMax");
    vaChaMaxDataPoint.setDescription("Setpoint for maximum charging VA.");
    vaChaMaxDataPoint.setUnits("VA");
    vaChaMaxDataPoint.setSize(1);
    vaChaMaxDataPoint.setAddressOffset(6);
    vaChaMaxDataPoint.setBlockOffset(4);
    vaChaMaxDataPoint.setScaleFactorName("VAChaMax_SF");
    vaChaMaxDataPoint.setSunSpecDataType("uint16");
    vaChaMaxDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    vaChaMaxDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vaChaMaxDataPoint.name(), vaChaMaxDataPoint);

    SunSpecDataPoint minRsvPctDataPoint;
    minRsvPctDataPoint.setName("MinRsvPct");
    minRsvPctDataPoint.setLabel("MinRsvPct");
    minRsvPctDataPoint.setDescription("Setpoint for minimum reserve for storage as a percentage of the nominal maximum storage.");
    minRsvPctDataPoint.setUnits("% WChaMax");
    minRsvPctDataPoint.setSize(1);
    minRsvPctDataPoint.setAddressOffset(7);
    minRsvPctDataPoint.setBlockOffset(5);
    minRsvPctDataPoint.setScaleFactorName("MinRsvPct_SF");
    minRsvPctDataPoint.setSunSpecDataType("uint16");
    minRsvPctDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    minRsvPctDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minRsvPctDataPoint.name(), minRsvPctDataPoint);

    SunSpecDataPoint chaStateDataPoint;
    chaStateDataPoint.setName("ChaState");
    chaStateDataPoint.setLabel("ChaState");
    chaStateDataPoint.setDescription("Currently available energy as a percent of the capacity rating.");
    chaStateDataPoint.setUnits("% AhrRtg");
    chaStateDataPoint.setSize(1);
    chaStateDataPoint.setAddressOffset(8);
    chaStateDataPoint.setBlockOffset(6);
    chaStateDataPoint.setScaleFactorName("ChaState_SF");
    chaStateDataPoint.setSunSpecDataType("uint16");
    chaStateDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(chaStateDataPoint.name(), chaStateDataPoint);

    SunSpecDataPoint storAvalDataPoint;
    storAvalDataPoint.setName("StorAval");
    storAvalDataPoint.setLabel("StorAval");
    storAvalDataPoint.setDescription("State of charge (ChaState) minus storage reserve (MinRsvPct) times capacity rating (AhrRtg).");
    storAvalDataPoint.setUnits("AH");
    storAvalDataPoint.setSize(1);
    storAvalDataPoint.setAddressOffset(9);
    storAvalDataPoint.setBlockOffset(7);
    storAvalDataPoint.setScaleFactorName("StorAval_SF");
    storAvalDataPoint.setSunSpecDataType("uint16");
    storAvalDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(storAvalDataPoint.name(), storAvalDataPoint);

    SunSpecDataPoint inBatVDataPoint;
    inBatVDataPoint.setName("InBatV");
    inBatVDataPoint.setLabel("InBatV");
    inBatVDataPoint.setDescription("Internal battery voltage.");
    inBatVDataPoint.setUnits("V");
    inBatVDataPoint.setSize(1);
    inBatVDataPoint.setAddressOffset(10);
    inBatVDataPoint.setBlockOffset(8);
    inBatVDataPoint.setScaleFactorName("InBatV_SF");
    inBatVDataPoint.setSunSpecDataType("uint16");
    inBatVDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(inBatVDataPoint.name(), inBatVDataPoint);

    SunSpecDataPoint chaStDataPoint;
    chaStDataPoint.setName("ChaSt");
    chaStDataPoint.setLabel("ChaSt");
    chaStDataPoint.setDescription("Charge status of storage device. Enumerated value.");
    chaStDataPoint.setSize(1);
    chaStDataPoint.setAddressOffset(11);
    chaStDataPoint.setBlockOffset(9);
    chaStDataPoint.setSunSpecDataType("enum16");
    chaStDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(chaStDataPoint.name(), chaStDataPoint);

    SunSpecDataPoint outWRteDataPoint;
    outWRteDataPoint.setName("OutWRte");
    outWRteDataPoint.setLabel("OutWRte");
    outWRteDataPoint.setDescription("Percent of max discharge rate.");
    outWRteDataPoint.setUnits("% WDisChaMax");
    outWRteDataPoint.setSize(1);
    outWRteDataPoint.setAddressOffset(12);
    outWRteDataPoint.setBlockOffset(10);
    outWRteDataPoint.setScaleFactorName("InOutWRte_SF");
    outWRteDataPoint.setSunSpecDataType("int16");
    outWRteDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    outWRteDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(outWRteDataPoint.name(), outWRteDataPoint);

    SunSpecDataPoint inWRteDataPoint;
    inWRteDataPoint.setName("InWRte");
    inWRteDataPoint.setLabel("InWRte");
    inWRteDataPoint.setDescription("Percent of max charging rate.");
    inWRteDataPoint.setUnits(" % WChaMax");
    inWRteDataPoint.setSize(1);
    inWRteDataPoint.setAddressOffset(13);
    inWRteDataPoint.setBlockOffset(11);
    inWRteDataPoint.setScaleFactorName("InOutWRte_SF");
    inWRteDataPoint.setSunSpecDataType("int16");
    inWRteDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    inWRteDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(inWRteDataPoint.name(), inWRteDataPoint);

    SunSpecDataPoint inOutWRteWinTmsDataPoint;
    inOutWRteWinTmsDataPoint.setName("InOutWRte_WinTms");
    inOutWRteWinTmsDataPoint.setLabel("InOutWRte_WinTms");
    inOutWRteWinTmsDataPoint.setDescription("Time window for charge/discharge rate change.");
    inOutWRteWinTmsDataPoint.setUnits("Secs");
    inOutWRteWinTmsDataPoint.setSize(1);
    inOutWRteWinTmsDataPoint.setAddressOffset(14);
    inOutWRteWinTmsDataPoint.setBlockOffset(12);
    inOutWRteWinTmsDataPoint.setSunSpecDataType("uint16");
    inOutWRteWinTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    inOutWRteWinTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(inOutWRteWinTmsDataPoint.name(), inOutWRteWinTmsDataPoint);

    SunSpecDataPoint inOutWRteRvrtTmsDataPoint;
    inOutWRteRvrtTmsDataPoint.setName("InOutWRte_RvrtTms");
    inOutWRteRvrtTmsDataPoint.setLabel("InOutWRte_RvrtTms");
    inOutWRteRvrtTmsDataPoint.setDescription("Timeout period for charge/discharge rate.");
    inOutWRteRvrtTmsDataPoint.setUnits("Secs");
    inOutWRteRvrtTmsDataPoint.setSize(1);
    inOutWRteRvrtTmsDataPoint.setAddressOffset(15);
    inOutWRteRvrtTmsDataPoint.setBlockOffset(13);
    inOutWRteRvrtTmsDataPoint.setSunSpecDataType("uint16");
    inOutWRteRvrtTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    inOutWRteRvrtTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(inOutWRteRvrtTmsDataPoint.name(), inOutWRteRvrtTmsDataPoint);

    SunSpecDataPoint inOutWRteRmpTmsDataPoint;
    inOutWRteRmpTmsDataPoint.setName("InOutWRte_RmpTms");
    inOutWRteRmpTmsDataPoint.setLabel("InOutWRte_RmpTms");
    inOutWRteRmpTmsDataPoint.setDescription("Ramp time for moving from current setpoint to new setpoint.");
    inOutWRteRmpTmsDataPoint.setUnits("Secs");
    inOutWRteRmpTmsDataPoint.setSize(1);
    inOutWRteRmpTmsDataPoint.setAddressOffset(16);
    inOutWRteRmpTmsDataPoint.setBlockOffset(14);
    inOutWRteRmpTmsDataPoint.setSunSpecDataType("uint16");
    inOutWRteRmpTmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    inOutWRteRmpTmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(inOutWRteRmpTmsDataPoint.name(), inOutWRteRmpTmsDataPoint);

    SunSpecDataPoint chaGriSetDataPoint;
    chaGriSetDataPoint.setName("ChaGriSet");
    chaGriSetDataPoint.setSize(1);
    chaGriSetDataPoint.setAddressOffset(17);
    chaGriSetDataPoint.setBlockOffset(15);
    chaGriSetDataPoint.setSunSpecDataType("enum16");
    chaGriSetDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    chaGriSetDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(chaGriSetDataPoint.name(), chaGriSetDataPoint);

    SunSpecDataPoint wChaMaxSfDataPoint;
    wChaMaxSfDataPoint.setName("WChaMax_SF");
    wChaMaxSfDataPoint.setLabel("WChaMax_SF");
    wChaMaxSfDataPoint.setDescription("Scale factor for maximum charge.");
    wChaMaxSfDataPoint.setMandatory(true);
    wChaMaxSfDataPoint.setSize(1);
    wChaMaxSfDataPoint.setAddressOffset(18);
    wChaMaxSfDataPoint.setBlockOffset(16);
    wChaMaxSfDataPoint.setSunSpecDataType("sunssf");
    wChaMaxSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wChaMaxSfDataPoint.name(), wChaMaxSfDataPoint);

    SunSpecDataPoint wChaDisChaGraSfDataPoint;
    wChaDisChaGraSfDataPoint.setName("WChaDisChaGra_SF");
    wChaDisChaGraSfDataPoint.setLabel("WChaDisChaGra_SF");
    wChaDisChaGraSfDataPoint.setDescription("Scale factor for maximum charge and discharge rate.");
    wChaDisChaGraSfDataPoint.setMandatory(true);
    wChaDisChaGraSfDataPoint.setSize(1);
    wChaDisChaGraSfDataPoint.setAddressOffset(19);
    wChaDisChaGraSfDataPoint.setBlockOffset(17);
    wChaDisChaGraSfDataPoint.setSunSpecDataType("sunssf");
    wChaDisChaGraSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wChaDisChaGraSfDataPoint.name(), wChaDisChaGraSfDataPoint);

    SunSpecDataPoint vaChaMaxSfDataPoint;
    vaChaMaxSfDataPoint.setName("VAChaMax_SF");
    vaChaMaxSfDataPoint.setLabel("VAChaMax_SF");
    vaChaMaxSfDataPoint.setDescription("Scale factor for maximum charging VA.");
    vaChaMaxSfDataPoint.setSize(1);
    vaChaMaxSfDataPoint.setAddressOffset(20);
    vaChaMaxSfDataPoint.setBlockOffset(18);
    vaChaMaxSfDataPoint.setSunSpecDataType("sunssf");
    vaChaMaxSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vaChaMaxSfDataPoint.name(), vaChaMaxSfDataPoint);

    SunSpecDataPoint minRsvPctSfDataPoint;
    minRsvPctSfDataPoint.setName("MinRsvPct_SF");
    minRsvPctSfDataPoint.setLabel("MinRsvPct_SF");
    minRsvPctSfDataPoint.setDescription("Scale factor for minimum reserve percentage.");
    minRsvPctSfDataPoint.setSize(1);
    minRsvPctSfDataPoint.setAddressOffset(21);
    minRsvPctSfDataPoint.setBlockOffset(19);
    minRsvPctSfDataPoint.setSunSpecDataType("sunssf");
    minRsvPctSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minRsvPctSfDataPoint.name(), minRsvPctSfDataPoint);

    SunSpecDataPoint chaStateSfDataPoint;
    chaStateSfDataPoint.setName("ChaState_SF");
    chaStateSfDataPoint.setLabel("ChaState_SF");
    chaStateSfDataPoint.setDescription("Scale factor for available energy percent.");
    chaStateSfDataPoint.setSize(1);
    chaStateSfDataPoint.setAddressOffset(22);
    chaStateSfDataPoint.setBlockOffset(20);
    chaStateSfDataPoint.setSunSpecDataType("sunssf");
    chaStateSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(chaStateSfDataPoint.name(), chaStateSfDataPoint);

    SunSpecDataPoint storAvalSfDataPoint;
    storAvalSfDataPoint.setName("StorAval_SF");
    storAvalSfDataPoint.setLabel("StorAval_SF");
    storAvalSfDataPoint.setDescription("Scale factor for state of charge.");
    storAvalSfDataPoint.setSize(1);
    storAvalSfDataPoint.setAddressOffset(23);
    storAvalSfDataPoint.setBlockOffset(21);
    storAvalSfDataPoint.setSunSpecDataType("sunssf");
    storAvalSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(storAvalSfDataPoint.name(), storAvalSfDataPoint);

    SunSpecDataPoint inBatVSfDataPoint;
    inBatVSfDataPoint.setName("InBatV_SF");
    inBatVSfDataPoint.setLabel("InBatV_SF");
    inBatVSfDataPoint.setDescription("Scale factor for battery voltage.");
    inBatVSfDataPoint.setSize(1);
    inBatVSfDataPoint.setAddressOffset(24);
    inBatVSfDataPoint.setBlockOffset(22);
    inBatVSfDataPoint.setSunSpecDataType("sunssf");
    inBatVSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(inBatVSfDataPoint.name(), inBatVSfDataPoint);

    SunSpecDataPoint inOutWRteSfDataPoint;
    inOutWRteSfDataPoint.setName("InOutWRte_SF");
    inOutWRteSfDataPoint.setLabel("InOutWRte_SF");
    inOutWRteSfDataPoint.setDescription("Scale factor for percent charge/discharge rate.");
    inOutWRteSfDataPoint.setSize(1);
    inOutWRteSfDataPoint.setAddressOffset(25);
    inOutWRteSfDataPoint.setBlockOffset(23);
    inOutWRteSfDataPoint.setSunSpecDataType("sunssf");
    inOutWRteSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(inOutWRteSfDataPoint.name(), inOutWRteSfDataPoint);

}

void SunSpecStorageModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("WChaMax_SF").isValid())
        m_wChaMaxSf = m_dataPoints.value("WChaMax_SF").toInt16();

    if (m_dataPoints.value("WChaDisChaGra_SF").isValid())
        m_wChaDisChaGraSf = m_dataPoints.value("WChaDisChaGra_SF").toInt16();

    if (m_dataPoints.value("VAChaMax_SF").isValid())
        m_vaChaMaxSf = m_dataPoints.value("VAChaMax_SF").toInt16();

    if (m_dataPoints.value("MinRsvPct_SF").isValid())
        m_minRsvPctSf = m_dataPoints.value("MinRsvPct_SF").toInt16();

    if (m_dataPoints.value("ChaState_SF").isValid())
        m_chaStateSf = m_dataPoints.value("ChaState_SF").toInt16();

    if (m_dataPoints.value("StorAval_SF").isValid())
        m_storAvalSf = m_dataPoints.value("StorAval_SF").toInt16();

    if (m_dataPoints.value("InBatV_SF").isValid())
        m_inBatVSf = m_dataPoints.value("InBatV_SF").toInt16();

    if (m_dataPoints.value("InOutWRte_SF").isValid())
        m_inOutWRteSf = m_dataPoints.value("InOutWRte_SF").toInt16();


    // Update properties according to the data point type
    if (m_dataPoints.value("WChaMax").isValid())
        m_wChaMax = m_dataPoints.value("WChaMax").toFloatWithSSF(m_wChaMaxSf);

    if (m_dataPoints.value("WChaGra").isValid())
        m_wChaGra = m_dataPoints.value("WChaGra").toFloatWithSSF(m_wChaDisChaGraSf);

    if (m_dataPoints.value("WDisChaGra").isValid())
        m_wDisChaGra = m_dataPoints.value("WDisChaGra").toFloatWithSSF(m_wChaDisChaGraSf);

    if (m_dataPoints.value("StorCtl_Mod").isValid())
        m_storCtlMod = static_cast<Storctl_modFlags>(m_dataPoints.value("StorCtl_Mod").toUInt16());

    if (m_dataPoints.value("VAChaMax").isValid())
        m_vaChaMax = m_dataPoints.value("VAChaMax").toFloatWithSSF(m_vaChaMaxSf);

    if (m_dataPoints.value("MinRsvPct").isValid())
        m_minRsvPct = m_dataPoints.value("MinRsvPct").toFloatWithSSF(m_minRsvPctSf);

    if (m_dataPoints.value("ChaState").isValid())
        m_chaState = m_dataPoints.value("ChaState").toFloatWithSSF(m_chaStateSf);

    if (m_dataPoints.value("StorAval").isValid())
        m_storAval = m_dataPoints.value("StorAval").toFloatWithSSF(m_storAvalSf);

    if (m_dataPoints.value("InBatV").isValid())
        m_inBatV = m_dataPoints.value("InBatV").toFloatWithSSF(m_inBatVSf);

    if (m_dataPoints.value("ChaSt").isValid())
        m_chaSt = static_cast<Chast>(m_dataPoints.value("ChaSt").toUInt16());

    if (m_dataPoints.value("OutWRte").isValid())
        m_outWRte = m_dataPoints.value("OutWRte").toFloatWithSSF(m_inOutWRteSf);

    if (m_dataPoints.value("InWRte").isValid())
        m_inWRte = m_dataPoints.value("InWRte").toFloatWithSSF(m_inOutWRteSf);

    if (m_dataPoints.value("InOutWRte_WinTms").isValid())
        m_inOutWRteWinTms = m_dataPoints.value("InOutWRte_WinTms").toUInt16();

    if (m_dataPoints.value("InOutWRte_RvrtTms").isValid())
        m_inOutWRteRvrtTms = m_dataPoints.value("InOutWRte_RvrtTms").toUInt16();

    if (m_dataPoints.value("InOutWRte_RmpTms").isValid())
        m_inOutWRteRmpTms = m_dataPoints.value("InOutWRte_RmpTms").toUInt16();

    if (m_dataPoints.value("ChaGriSet").isValid())
        m_chaGriSet = static_cast<Chagriset>(m_dataPoints.value("ChaGriSet").toUInt16());

    if (m_dataPoints.value("WChaMax_SF").isValid())
        m_wChaMaxSf = m_dataPoints.value("WChaMax_SF").toInt16();

    if (m_dataPoints.value("WChaDisChaGra_SF").isValid())
        m_wChaDisChaGraSf = m_dataPoints.value("WChaDisChaGra_SF").toInt16();

    if (m_dataPoints.value("VAChaMax_SF").isValid())
        m_vaChaMaxSf = m_dataPoints.value("VAChaMax_SF").toInt16();

    if (m_dataPoints.value("MinRsvPct_SF").isValid())
        m_minRsvPctSf = m_dataPoints.value("MinRsvPct_SF").toInt16();

    if (m_dataPoints.value("ChaState_SF").isValid())
        m_chaStateSf = m_dataPoints.value("ChaState_SF").toInt16();

    if (m_dataPoints.value("StorAval_SF").isValid())
        m_storAvalSf = m_dataPoints.value("StorAval_SF").toInt16();

    if (m_dataPoints.value("InBatV_SF").isValid())
        m_inBatVSf = m_dataPoints.value("InBatV_SF").toInt16();

    if (m_dataPoints.value("InOutWRte_SF").isValid())
        m_inOutWRteSf = m_dataPoints.value("InOutWRte_SF").toInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecStorageModel *model)
{
    debug.nospace().noquote() << "SunSpecStorageModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("WChaMax") << "-->";
    if (model->dataPoints().value("WChaMax").isValid()) {
        debug.nospace().noquote() << model->wChaMax() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WChaGra") << "-->";
    if (model->dataPoints().value("WChaGra").isValid()) {
        debug.nospace().noquote() << model->wChaGra() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WDisChaGra") << "-->";
    if (model->dataPoints().value("WDisChaGra").isValid()) {
        debug.nospace().noquote() << model->wDisChaGra() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("StorCtl_Mod") << "-->";
    if (model->dataPoints().value("StorCtl_Mod").isValid()) {
        debug.nospace().noquote() << model->storCtlMod() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VAChaMax") << "-->";
    if (model->dataPoints().value("VAChaMax").isValid()) {
        debug.nospace().noquote() << model->vaChaMax() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("MinRsvPct") << "-->";
    if (model->dataPoints().value("MinRsvPct").isValid()) {
        debug.nospace().noquote() << model->minRsvPct() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ChaState") << "-->";
    if (model->dataPoints().value("ChaState").isValid()) {
        debug.nospace().noquote() << model->chaState() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("StorAval") << "-->";
    if (model->dataPoints().value("StorAval").isValid()) {
        debug.nospace().noquote() << model->storAval() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("InBatV") << "-->";
    if (model->dataPoints().value("InBatV").isValid()) {
        debug.nospace().noquote() << model->inBatV() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ChaSt") << "-->";
    if (model->dataPoints().value("ChaSt").isValid()) {
        debug.nospace().noquote() << model->chaSt() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("OutWRte") << "-->";
    if (model->dataPoints().value("OutWRte").isValid()) {
        debug.nospace().noquote() << model->outWRte() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("InWRte") << "-->";
    if (model->dataPoints().value("InWRte").isValid()) {
        debug.nospace().noquote() << model->inWRte() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("InOutWRte_WinTms") << "-->";
    if (model->dataPoints().value("InOutWRte_WinTms").isValid()) {
        debug.nospace().noquote() << model->inOutWRteWinTms() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("InOutWRte_RvrtTms") << "-->";
    if (model->dataPoints().value("InOutWRte_RvrtTms").isValid()) {
        debug.nospace().noquote() << model->inOutWRteRvrtTms() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("InOutWRte_RmpTms") << "-->";
    if (model->dataPoints().value("InOutWRte_RmpTms").isValid()) {
        debug.nospace().noquote() << model->inOutWRteRmpTms() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ChaGriSet") << "-->";
    if (model->dataPoints().value("ChaGriSet").isValid()) {
        debug.nospace().noquote() << model->chaGriSet() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
