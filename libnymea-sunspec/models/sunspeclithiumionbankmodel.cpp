// SPDX-License-Identifier: LGPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of libnymea-sunspec.
*
* libnymea-sunspec is free software: you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* as published by the Free Software Foundation, either version 3
* of the License, or (at your option) any later version.
*
* libnymea-sunspec is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with libnymea-sunspec. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "sunspeclithiumionbankmodel.h"
#include "sunspecconnection.h"

SunSpecLithiumIonBankModelRepeatingBlock::SunSpecLithiumIonBankModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecLithiumIonBankModel *parent) :
    SunSpecModelRepeatingBlock(blockIndex, blockSize, modbusStartRegister, parent)
{
    m_parentModel = parent;
    m_byteOrder = parent->byteOrder();
    initDataPoints();
}

QString SunSpecLithiumIonBankModelRepeatingBlock::name() const
{
    return "string";
}

SunSpecLithiumIonBankModel *SunSpecLithiumIonBankModelRepeatingBlock::parentModel() const
{
    return m_parentModel;
}

quint16 SunSpecLithiumIonBankModelRepeatingBlock::moduleCount() const
{
    return m_moduleCount;
}
SunSpecLithiumIonBankModelRepeatingBlock::StrstFlags SunSpecLithiumIonBankModelRepeatingBlock::stringStatus() const
{
    return m_stringStatus;
}
SunSpecLithiumIonBankModelRepeatingBlock::Strconfail SunSpecLithiumIonBankModelRepeatingBlock::connectionFailureReason() const
{
    return m_connectionFailureReason;
}
float SunSpecLithiumIonBankModelRepeatingBlock::stringStateOfCharge() const
{
    return m_stringStateOfCharge;
}
float SunSpecLithiumIonBankModelRepeatingBlock::stringStateOfHealth() const
{
    return m_stringStateOfHealth;
}
float SunSpecLithiumIonBankModelRepeatingBlock::stringCurrent() const
{
    return m_stringCurrent;
}
float SunSpecLithiumIonBankModelRepeatingBlock::maxCellVoltage() const
{
    return m_maxCellVoltage;
}
quint16 SunSpecLithiumIonBankModelRepeatingBlock::maxCellVoltageModule() const
{
    return m_maxCellVoltageModule;
}
float SunSpecLithiumIonBankModelRepeatingBlock::minCellVoltage() const
{
    return m_minCellVoltage;
}
quint16 SunSpecLithiumIonBankModelRepeatingBlock::minCellVoltageModule() const
{
    return m_minCellVoltageModule;
}
float SunSpecLithiumIonBankModelRepeatingBlock::averageCellVoltage() const
{
    return m_averageCellVoltage;
}
float SunSpecLithiumIonBankModelRepeatingBlock::maxModuleTemperature() const
{
    return m_maxModuleTemperature;
}
quint16 SunSpecLithiumIonBankModelRepeatingBlock::maxModuleTemperatureModule() const
{
    return m_maxModuleTemperatureModule;
}
float SunSpecLithiumIonBankModelRepeatingBlock::minModuleTemperature() const
{
    return m_minModuleTemperature;
}
quint16 SunSpecLithiumIonBankModelRepeatingBlock::minModuleTemperatureModule() const
{
    return m_minModuleTemperatureModule;
}
float SunSpecLithiumIonBankModelRepeatingBlock::averageModuleTemperature() const
{
    return m_averageModuleTemperature;
}
SunSpecLithiumIonBankModelRepeatingBlock::Strdisrsn SunSpecLithiumIonBankModelRepeatingBlock::disabledReason() const
{
    return m_disabledReason;
}
SunSpecLithiumIonBankModelRepeatingBlock::StrconstFlags SunSpecLithiumIonBankModelRepeatingBlock::contactorStatus() const
{
    return m_contactorStatus;
}
SunSpecLithiumIonBankModelRepeatingBlock::Strevt1Flags SunSpecLithiumIonBankModelRepeatingBlock::stringEvent1() const
{
    return m_stringEvent1;
}
quint32 SunSpecLithiumIonBankModelRepeatingBlock::stringEvent2() const
{
    return m_stringEvent2;
}
quint32 SunSpecLithiumIonBankModelRepeatingBlock::vendorStringEventBitfield1() const
{
    return m_vendorStringEventBitfield1;
}
quint32 SunSpecLithiumIonBankModelRepeatingBlock::vendorStringEventBitfield2() const
{
    return m_vendorStringEventBitfield2;
}
SunSpecLithiumIonBankModelRepeatingBlock::Strsetena SunSpecLithiumIonBankModelRepeatingBlock::enableDisableString() const
{
    return m_enableDisableString;
}

QModbusReply *SunSpecLithiumIonBankModelRepeatingBlock::setEnableDisableString(Strsetena enableDisableString)
{
    SunSpecDataPoint dp = m_dataPoints.value("StrSetEna");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(enableDisableString));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

SunSpecLithiumIonBankModelRepeatingBlock::Strsetcon SunSpecLithiumIonBankModelRepeatingBlock::connectDisconnectString() const
{
    return m_connectDisconnectString;
}

QModbusReply *SunSpecLithiumIonBankModelRepeatingBlock::setConnectDisconnectString(Strsetcon connectDisconnectString)
{
    SunSpecDataPoint dp = m_dataPoints.value("StrSetCon");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(connectDisconnectString));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

quint16 SunSpecLithiumIonBankModelRepeatingBlock::pad1() const
{
    return m_pad1;
}
quint16 SunSpecLithiumIonBankModelRepeatingBlock::pad2() const
{
    return m_pad2;
}

void SunSpecLithiumIonBankModelRepeatingBlock::initDataPoints()
{
    SunSpecDataPoint moduleCountDataPoint;
    moduleCountDataPoint.setName("StrNMod");
    moduleCountDataPoint.setLabel("Module Count");
    moduleCountDataPoint.setDescription("Count of modules in the string.");
    moduleCountDataPoint.setMandatory(true);
    moduleCountDataPoint.setSize(1);
    moduleCountDataPoint.setAddressOffset(0);
    moduleCountDataPoint.setSunSpecDataType("uint16");
    moduleCountDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(moduleCountDataPoint.name(), moduleCountDataPoint);

    SunSpecDataPoint stringStatusDataPoint;
    stringStatusDataPoint.setName("StrSt");
    stringStatusDataPoint.setLabel("String Status");
    stringStatusDataPoint.setDescription("Current status of the string.");
    stringStatusDataPoint.setMandatory(true);
    stringStatusDataPoint.setSize(2);
    stringStatusDataPoint.setAddressOffset(1);
    stringStatusDataPoint.setSunSpecDataType("bitfield32");
    stringStatusDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringStatusDataPoint.name(), stringStatusDataPoint);

    SunSpecDataPoint connectionFailureReasonDataPoint;
    connectionFailureReasonDataPoint.setName("StrConFail");
    connectionFailureReasonDataPoint.setLabel("Connection Failure Reason");
    connectionFailureReasonDataPoint.setSize(1);
    connectionFailureReasonDataPoint.setAddressOffset(3);
    connectionFailureReasonDataPoint.setBlockOffset(1);
    connectionFailureReasonDataPoint.setSunSpecDataType("enum16");
    connectionFailureReasonDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(connectionFailureReasonDataPoint.name(), connectionFailureReasonDataPoint);

    SunSpecDataPoint stringStateOfChargeDataPoint;
    stringStateOfChargeDataPoint.setName("StrSoC");
    stringStateOfChargeDataPoint.setLabel("String State of Charge");
    stringStateOfChargeDataPoint.setDescription("Battery string state of charge, expressed as a percentage.");
    stringStateOfChargeDataPoint.setUnits("%");
    stringStateOfChargeDataPoint.setMandatory(true);
    stringStateOfChargeDataPoint.setSize(1);
    stringStateOfChargeDataPoint.setAddressOffset(4);
    stringStateOfChargeDataPoint.setBlockOffset(2);
    stringStateOfChargeDataPoint.setScaleFactorName("SoC_SF");
    stringStateOfChargeDataPoint.setSunSpecDataType("uint16");
    stringStateOfChargeDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringStateOfChargeDataPoint.name(), stringStateOfChargeDataPoint);

    SunSpecDataPoint stringStateOfHealthDataPoint;
    stringStateOfHealthDataPoint.setName("StrSoH");
    stringStateOfHealthDataPoint.setLabel("String State of Health");
    stringStateOfHealthDataPoint.setDescription("Battery string state of health, expressed as a percentage.");
    stringStateOfHealthDataPoint.setUnits("%");
    stringStateOfHealthDataPoint.setSize(1);
    stringStateOfHealthDataPoint.setAddressOffset(5);
    stringStateOfHealthDataPoint.setBlockOffset(3);
    stringStateOfHealthDataPoint.setScaleFactorName("SoH_SF");
    stringStateOfHealthDataPoint.setSunSpecDataType("uint16");
    stringStateOfHealthDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringStateOfHealthDataPoint.name(), stringStateOfHealthDataPoint);

    SunSpecDataPoint stringCurrentDataPoint;
    stringCurrentDataPoint.setName("StrA");
    stringCurrentDataPoint.setLabel("String Current");
    stringCurrentDataPoint.setDescription("String current measurement.");
    stringCurrentDataPoint.setUnits("A");
    stringCurrentDataPoint.setMandatory(true);
    stringCurrentDataPoint.setSize(1);
    stringCurrentDataPoint.setAddressOffset(6);
    stringCurrentDataPoint.setBlockOffset(4);
    stringCurrentDataPoint.setScaleFactorName("A_SF");
    stringCurrentDataPoint.setSunSpecDataType("int16");
    stringCurrentDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringCurrentDataPoint.name(), stringCurrentDataPoint);

    SunSpecDataPoint maxCellVoltageDataPoint;
    maxCellVoltageDataPoint.setName("StrCellVMax");
    maxCellVoltageDataPoint.setLabel("Max Cell Voltage");
    maxCellVoltageDataPoint.setDescription("Maximum voltage for all cells in the string.");
    maxCellVoltageDataPoint.setUnits("V");
    maxCellVoltageDataPoint.setMandatory(true);
    maxCellVoltageDataPoint.setSize(1);
    maxCellVoltageDataPoint.setAddressOffset(7);
    maxCellVoltageDataPoint.setBlockOffset(5);
    maxCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    maxCellVoltageDataPoint.setSunSpecDataType("uint16");
    maxCellVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxCellVoltageDataPoint.name(), maxCellVoltageDataPoint);

    SunSpecDataPoint maxCellVoltageModuleDataPoint;
    maxCellVoltageModuleDataPoint.setName("StrCellVMaxMod");
    maxCellVoltageModuleDataPoint.setLabel("Max Cell Voltage Module");
    maxCellVoltageModuleDataPoint.setDescription("Module containing the maximum cell voltage.");
    maxCellVoltageModuleDataPoint.setSize(1);
    maxCellVoltageModuleDataPoint.setAddressOffset(8);
    maxCellVoltageModuleDataPoint.setBlockOffset(6);
    maxCellVoltageModuleDataPoint.setSunSpecDataType("uint16");
    maxCellVoltageModuleDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxCellVoltageModuleDataPoint.name(), maxCellVoltageModuleDataPoint);

    SunSpecDataPoint minCellVoltageDataPoint;
    minCellVoltageDataPoint.setName("StrCellVMin");
    minCellVoltageDataPoint.setLabel("Min Cell Voltage");
    minCellVoltageDataPoint.setDescription("Minimum voltage for all cells in the string.");
    minCellVoltageDataPoint.setUnits("V");
    minCellVoltageDataPoint.setMandatory(true);
    minCellVoltageDataPoint.setSize(1);
    minCellVoltageDataPoint.setAddressOffset(9);
    minCellVoltageDataPoint.setBlockOffset(7);
    minCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    minCellVoltageDataPoint.setSunSpecDataType("uint16");
    minCellVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minCellVoltageDataPoint.name(), minCellVoltageDataPoint);

    SunSpecDataPoint minCellVoltageModuleDataPoint;
    minCellVoltageModuleDataPoint.setName("StrCellVMinMod");
    minCellVoltageModuleDataPoint.setLabel("Min Cell Voltage Module");
    minCellVoltageModuleDataPoint.setDescription("Module containing the minimum cell voltage.");
    minCellVoltageModuleDataPoint.setSize(1);
    minCellVoltageModuleDataPoint.setAddressOffset(10);
    minCellVoltageModuleDataPoint.setBlockOffset(8);
    minCellVoltageModuleDataPoint.setSunSpecDataType("uint16");
    minCellVoltageModuleDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minCellVoltageModuleDataPoint.name(), minCellVoltageModuleDataPoint);

    SunSpecDataPoint averageCellVoltageDataPoint;
    averageCellVoltageDataPoint.setName("StrCellVAvg");
    averageCellVoltageDataPoint.setLabel("Average Cell Voltage");
    averageCellVoltageDataPoint.setDescription("Average voltage for all cells in the string.");
    averageCellVoltageDataPoint.setUnits("V");
    averageCellVoltageDataPoint.setMandatory(true);
    averageCellVoltageDataPoint.setSize(1);
    averageCellVoltageDataPoint.setAddressOffset(11);
    averageCellVoltageDataPoint.setBlockOffset(9);
    averageCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    averageCellVoltageDataPoint.setSunSpecDataType("uint16");
    averageCellVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(averageCellVoltageDataPoint.name(), averageCellVoltageDataPoint);

    SunSpecDataPoint maxModuleTemperatureDataPoint;
    maxModuleTemperatureDataPoint.setName("StrModTmpMax");
    maxModuleTemperatureDataPoint.setLabel("Max Module Temperature");
    maxModuleTemperatureDataPoint.setDescription("Maximum temperature for all modules in the bank.");
    maxModuleTemperatureDataPoint.setUnits("C");
    maxModuleTemperatureDataPoint.setMandatory(true);
    maxModuleTemperatureDataPoint.setSize(1);
    maxModuleTemperatureDataPoint.setAddressOffset(12);
    maxModuleTemperatureDataPoint.setBlockOffset(10);
    maxModuleTemperatureDataPoint.setScaleFactorName("ModTmp_SF");
    maxModuleTemperatureDataPoint.setSunSpecDataType("int16");
    maxModuleTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxModuleTemperatureDataPoint.name(), maxModuleTemperatureDataPoint);

    SunSpecDataPoint maxModuleTemperatureModuleDataPoint;
    maxModuleTemperatureModuleDataPoint.setName("StrModTmpMaxMod");
    maxModuleTemperatureModuleDataPoint.setLabel("Max Module Temperature Module");
    maxModuleTemperatureModuleDataPoint.setDescription("Module with the maximum temperature.");
    maxModuleTemperatureModuleDataPoint.setSize(1);
    maxModuleTemperatureModuleDataPoint.setAddressOffset(13);
    maxModuleTemperatureModuleDataPoint.setBlockOffset(11);
    maxModuleTemperatureModuleDataPoint.setSunSpecDataType("uint16");
    maxModuleTemperatureModuleDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxModuleTemperatureModuleDataPoint.name(), maxModuleTemperatureModuleDataPoint);

    SunSpecDataPoint minModuleTemperatureDataPoint;
    minModuleTemperatureDataPoint.setName("StrModTmpMin");
    minModuleTemperatureDataPoint.setLabel("Min Module Temperature");
    minModuleTemperatureDataPoint.setDescription("Minimum temperature for all modules in the bank.");
    minModuleTemperatureDataPoint.setUnits("C");
    minModuleTemperatureDataPoint.setMandatory(true);
    minModuleTemperatureDataPoint.setSize(1);
    minModuleTemperatureDataPoint.setAddressOffset(14);
    minModuleTemperatureDataPoint.setBlockOffset(12);
    minModuleTemperatureDataPoint.setScaleFactorName("ModTmp_SF");
    minModuleTemperatureDataPoint.setSunSpecDataType("int16");
    minModuleTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minModuleTemperatureDataPoint.name(), minModuleTemperatureDataPoint);

    SunSpecDataPoint minModuleTemperatureModuleDataPoint;
    minModuleTemperatureModuleDataPoint.setName("StrModTmpMinMod");
    minModuleTemperatureModuleDataPoint.setLabel("Min Module Temperature Module");
    minModuleTemperatureModuleDataPoint.setDescription("Module with the minimum temperature.");
    minModuleTemperatureModuleDataPoint.setSize(1);
    minModuleTemperatureModuleDataPoint.setAddressOffset(15);
    minModuleTemperatureModuleDataPoint.setBlockOffset(13);
    minModuleTemperatureModuleDataPoint.setSunSpecDataType("uint16");
    minModuleTemperatureModuleDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minModuleTemperatureModuleDataPoint.name(), minModuleTemperatureModuleDataPoint);

    SunSpecDataPoint averageModuleTemperatureDataPoint;
    averageModuleTemperatureDataPoint.setName("StrModTmpAvg");
    averageModuleTemperatureDataPoint.setLabel("Average Module Temperature");
    averageModuleTemperatureDataPoint.setDescription("Average temperature for all modules in the bank.");
    averageModuleTemperatureDataPoint.setUnits("C");
    averageModuleTemperatureDataPoint.setMandatory(true);
    averageModuleTemperatureDataPoint.setSize(1);
    averageModuleTemperatureDataPoint.setAddressOffset(16);
    averageModuleTemperatureDataPoint.setBlockOffset(14);
    averageModuleTemperatureDataPoint.setScaleFactorName("ModTmp_SF");
    averageModuleTemperatureDataPoint.setSunSpecDataType("int16");
    averageModuleTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(averageModuleTemperatureDataPoint.name(), averageModuleTemperatureDataPoint);

    SunSpecDataPoint disabledReasonDataPoint;
    disabledReasonDataPoint.setName("StrDisRsn");
    disabledReasonDataPoint.setLabel("Disabled Reason");
    disabledReasonDataPoint.setDescription("Reason why the string is currently disabled.");
    disabledReasonDataPoint.setSize(1);
    disabledReasonDataPoint.setAddressOffset(17);
    disabledReasonDataPoint.setBlockOffset(15);
    disabledReasonDataPoint.setSunSpecDataType("enum16");
    disabledReasonDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(disabledReasonDataPoint.name(), disabledReasonDataPoint);

    SunSpecDataPoint contactorStatusDataPoint;
    contactorStatusDataPoint.setName("StrConSt");
    contactorStatusDataPoint.setLabel("Contactor Status");
    contactorStatusDataPoint.setDescription("Status of the contactor(s) for the string.");
    contactorStatusDataPoint.setSize(2);
    contactorStatusDataPoint.setAddressOffset(18);
    contactorStatusDataPoint.setBlockOffset(16);
    contactorStatusDataPoint.setSunSpecDataType("bitfield32");
    contactorStatusDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(contactorStatusDataPoint.name(), contactorStatusDataPoint);

    SunSpecDataPoint stringEvent1DataPoint;
    stringEvent1DataPoint.setName("StrEvt1");
    stringEvent1DataPoint.setLabel("String Event 1");
    stringEvent1DataPoint.setDescription("Alarms, warnings and status values.  Bit flags.");
    stringEvent1DataPoint.setMandatory(true);
    stringEvent1DataPoint.setSize(2);
    stringEvent1DataPoint.setAddressOffset(20);
    stringEvent1DataPoint.setBlockOffset(18);
    stringEvent1DataPoint.setSunSpecDataType("bitfield32");
    stringEvent1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringEvent1DataPoint.name(), stringEvent1DataPoint);

    SunSpecDataPoint stringEvent2DataPoint;
    stringEvent2DataPoint.setName("StrEvt2");
    stringEvent2DataPoint.setLabel("String Event 2");
    stringEvent2DataPoint.setDescription("Alarms, warnings and status values.  Bit flags.");
    stringEvent2DataPoint.setSize(2);
    stringEvent2DataPoint.setAddressOffset(22);
    stringEvent2DataPoint.setBlockOffset(20);
    stringEvent2DataPoint.setSunSpecDataType("bitfield32");
    stringEvent2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringEvent2DataPoint.name(), stringEvent2DataPoint);

    SunSpecDataPoint vendorStringEventBitfield1DataPoint;
    vendorStringEventBitfield1DataPoint.setName("StrEvtVnd1");
    vendorStringEventBitfield1DataPoint.setLabel("Vendor String Event Bitfield 1");
    vendorStringEventBitfield1DataPoint.setDescription("Vendor defined events.");
    vendorStringEventBitfield1DataPoint.setSize(2);
    vendorStringEventBitfield1DataPoint.setAddressOffset(24);
    vendorStringEventBitfield1DataPoint.setBlockOffset(22);
    vendorStringEventBitfield1DataPoint.setSunSpecDataType("bitfield32");
    vendorStringEventBitfield1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vendorStringEventBitfield1DataPoint.name(), vendorStringEventBitfield1DataPoint);

    SunSpecDataPoint vendorStringEventBitfield2DataPoint;
    vendorStringEventBitfield2DataPoint.setName("StrEvtVnd2");
    vendorStringEventBitfield2DataPoint.setLabel("Vendor String Event Bitfield 2");
    vendorStringEventBitfield2DataPoint.setDescription("Vendor defined events.");
    vendorStringEventBitfield2DataPoint.setSize(2);
    vendorStringEventBitfield2DataPoint.setAddressOffset(26);
    vendorStringEventBitfield2DataPoint.setBlockOffset(24);
    vendorStringEventBitfield2DataPoint.setSunSpecDataType("bitfield32");
    vendorStringEventBitfield2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vendorStringEventBitfield2DataPoint.name(), vendorStringEventBitfield2DataPoint);

    SunSpecDataPoint enableDisableStringDataPoint;
    enableDisableStringDataPoint.setName("StrSetEna");
    enableDisableStringDataPoint.setLabel("Enable/Disable String");
    enableDisableStringDataPoint.setDescription("Enables and disables the string.");
    enableDisableStringDataPoint.setSize(1);
    enableDisableStringDataPoint.setAddressOffset(28);
    enableDisableStringDataPoint.setBlockOffset(26);
    enableDisableStringDataPoint.setSunSpecDataType("enum16");
    enableDisableStringDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    enableDisableStringDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(enableDisableStringDataPoint.name(), enableDisableStringDataPoint);

    SunSpecDataPoint connectDisconnectStringDataPoint;
    connectDisconnectStringDataPoint.setName("StrSetCon");
    connectDisconnectStringDataPoint.setLabel("Connect/Disconnect String");
    connectDisconnectStringDataPoint.setDescription("Connects and disconnects the string.");
    connectDisconnectStringDataPoint.setSize(1);
    connectDisconnectStringDataPoint.setAddressOffset(29);
    connectDisconnectStringDataPoint.setBlockOffset(27);
    connectDisconnectStringDataPoint.setSunSpecDataType("enum16");
    connectDisconnectStringDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    connectDisconnectStringDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(connectDisconnectStringDataPoint.name(), connectDisconnectStringDataPoint);

    SunSpecDataPoint pad1DataPoint;
    pad1DataPoint.setName("Pad1");
    pad1DataPoint.setLabel("Pad");
    pad1DataPoint.setDescription("Pad register.");
    pad1DataPoint.setMandatory(true);
    pad1DataPoint.setSize(1);
    pad1DataPoint.setAddressOffset(30);
    pad1DataPoint.setBlockOffset(28);
    pad1DataPoint.setSunSpecDataType("pad");
    pad1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pad1DataPoint.name(), pad1DataPoint);

    SunSpecDataPoint pad2DataPoint;
    pad2DataPoint.setName("Pad2");
    pad2DataPoint.setLabel("Pad");
    pad2DataPoint.setDescription("Pad register.");
    pad2DataPoint.setMandatory(true);
    pad2DataPoint.setSize(1);
    pad2DataPoint.setAddressOffset(31);
    pad2DataPoint.setBlockOffset(29);
    pad2DataPoint.setSunSpecDataType("pad");
    pad2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pad2DataPoint.name(), pad2DataPoint);

}

void SunSpecLithiumIonBankModelRepeatingBlock::processBlockData()
{
    // Update properties according to the data point type
    if (m_dataPoints.value("StrNMod").isValid())
        m_moduleCount = m_dataPoints.value("StrNMod").toUInt16();

    if (m_dataPoints.value("StrSt").isValid())
        m_stringStatus = static_cast<StrstFlags>(m_dataPoints.value("StrSt").toUInt32());

    if (m_dataPoints.value("StrConFail").isValid())
        m_connectionFailureReason = static_cast<Strconfail>(m_dataPoints.value("StrConFail").toUInt16());

    if (m_dataPoints.value("StrSoC").isValid())
        m_stringStateOfCharge = m_dataPoints.value("StrSoC").toFloatWithSSF(m_parentModel->soC_SF());

    if (m_dataPoints.value("StrSoH").isValid())
        m_stringStateOfHealth = m_dataPoints.value("StrSoH").toFloatWithSSF(m_parentModel->soH_SF());

    if (m_dataPoints.value("StrA").isValid())
        m_stringCurrent = m_dataPoints.value("StrA").toFloatWithSSF(m_parentModel->a_SF());

    if (m_dataPoints.value("StrCellVMax").isValid())
        m_maxCellVoltage = m_dataPoints.value("StrCellVMax").toFloatWithSSF(m_parentModel->cellV_SF());

    if (m_dataPoints.value("StrCellVMaxMod").isValid())
        m_maxCellVoltageModule = m_dataPoints.value("StrCellVMaxMod").toUInt16();

    if (m_dataPoints.value("StrCellVMin").isValid())
        m_minCellVoltage = m_dataPoints.value("StrCellVMin").toFloatWithSSF(m_parentModel->cellV_SF());

    if (m_dataPoints.value("StrCellVMinMod").isValid())
        m_minCellVoltageModule = m_dataPoints.value("StrCellVMinMod").toUInt16();

    if (m_dataPoints.value("StrCellVAvg").isValid())
        m_averageCellVoltage = m_dataPoints.value("StrCellVAvg").toFloatWithSSF(m_parentModel->cellV_SF());

    if (m_dataPoints.value("StrModTmpMax").isValid())
        m_maxModuleTemperature = m_dataPoints.value("StrModTmpMax").toFloatWithSSF(m_parentModel->modTmp_SF());

    if (m_dataPoints.value("StrModTmpMaxMod").isValid())
        m_maxModuleTemperatureModule = m_dataPoints.value("StrModTmpMaxMod").toUInt16();

    if (m_dataPoints.value("StrModTmpMin").isValid())
        m_minModuleTemperature = m_dataPoints.value("StrModTmpMin").toFloatWithSSF(m_parentModel->modTmp_SF());

    if (m_dataPoints.value("StrModTmpMinMod").isValid())
        m_minModuleTemperatureModule = m_dataPoints.value("StrModTmpMinMod").toUInt16();

    if (m_dataPoints.value("StrModTmpAvg").isValid())
        m_averageModuleTemperature = m_dataPoints.value("StrModTmpAvg").toFloatWithSSF(m_parentModel->modTmp_SF());

    if (m_dataPoints.value("StrDisRsn").isValid())
        m_disabledReason = static_cast<Strdisrsn>(m_dataPoints.value("StrDisRsn").toUInt16());

    if (m_dataPoints.value("StrConSt").isValid())
        m_contactorStatus = static_cast<StrconstFlags>(m_dataPoints.value("StrConSt").toUInt32());

    if (m_dataPoints.value("StrEvt1").isValid())
        m_stringEvent1 = static_cast<Strevt1Flags>(m_dataPoints.value("StrEvt1").toUInt32());

    if (m_dataPoints.value("StrEvt2").isValid())
        m_stringEvent2 = m_dataPoints.value("StrEvt2").toUInt32();

    if (m_dataPoints.value("StrEvtVnd1").isValid())
        m_vendorStringEventBitfield1 = m_dataPoints.value("StrEvtVnd1").toUInt32();

    if (m_dataPoints.value("StrEvtVnd2").isValid())
        m_vendorStringEventBitfield2 = m_dataPoints.value("StrEvtVnd2").toUInt32();

    if (m_dataPoints.value("StrSetEna").isValid())
        m_enableDisableString = static_cast<Strsetena>(m_dataPoints.value("StrSetEna").toUInt16());

    if (m_dataPoints.value("StrSetCon").isValid())
        m_connectDisconnectString = static_cast<Strsetcon>(m_dataPoints.value("StrSetCon").toUInt16());

    if (m_dataPoints.value("Pad1").isValid())
        m_pad1 = m_dataPoints.value("Pad1").toUInt16();

    if (m_dataPoints.value("Pad2").isValid())
        m_pad2 = m_dataPoints.value("Pad2").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}


SunSpecLithiumIonBankModel::SunSpecLithiumIonBankModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 803, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixedAndRepeating;

    initDataPoints();

    connect(this, &SunSpecModel::initFinished, this, &SunSpecLithiumIonBankModel::setupRepeatingBlocks);
}

SunSpecLithiumIonBankModel::~SunSpecLithiumIonBankModel()
{

}

QString SunSpecLithiumIonBankModel::name() const
{
    return "lithium_ion_bank";
}

QString SunSpecLithiumIonBankModel::description() const
{
    return QString();
}

QString SunSpecLithiumIonBankModel::label() const
{
    return "Lithium-Ion Battery Bank Model";
}

quint16 SunSpecLithiumIonBankModel::stringCount() const
{
    return m_stringCount;
}
quint16 SunSpecLithiumIonBankModel::connectedStringCount() const
{
    return m_connectedStringCount;
}
float SunSpecLithiumIonBankModel::maxModuleTemperature() const
{
    return m_maxModuleTemperature;
}
quint16 SunSpecLithiumIonBankModel::maxModuleTemperatureString() const
{
    return m_maxModuleTemperatureString;
}
quint16 SunSpecLithiumIonBankModel::maxModuleTemperatureModule() const
{
    return m_maxModuleTemperatureModule;
}
float SunSpecLithiumIonBankModel::minModuleTemperature() const
{
    return m_minModuleTemperature;
}
quint16 SunSpecLithiumIonBankModel::minModuleTemperatureString() const
{
    return m_minModuleTemperatureString;
}
quint16 SunSpecLithiumIonBankModel::minModuleTemperatureModule() const
{
    return m_minModuleTemperatureModule;
}
qint16 SunSpecLithiumIonBankModel::averageModuleTemperature() const
{
    return m_averageModuleTemperature;
}
float SunSpecLithiumIonBankModel::maxStringVoltage() const
{
    return m_maxStringVoltage;
}
quint16 SunSpecLithiumIonBankModel::maxStringVoltageString() const
{
    return m_maxStringVoltageString;
}
float SunSpecLithiumIonBankModel::minStringVoltage() const
{
    return m_minStringVoltage;
}
quint16 SunSpecLithiumIonBankModel::minStringVoltageString() const
{
    return m_minStringVoltageString;
}
float SunSpecLithiumIonBankModel::averageStringVoltage() const
{
    return m_averageStringVoltage;
}
float SunSpecLithiumIonBankModel::maxStringCurrent() const
{
    return m_maxStringCurrent;
}
quint16 SunSpecLithiumIonBankModel::maxStringCurrentString() const
{
    return m_maxStringCurrentString;
}
float SunSpecLithiumIonBankModel::minStringCurrent() const
{
    return m_minStringCurrent;
}
quint16 SunSpecLithiumIonBankModel::minStringCurrentString() const
{
    return m_minStringCurrentString;
}
float SunSpecLithiumIonBankModel::averageStringCurrent() const
{
    return m_averageStringCurrent;
}
quint16 SunSpecLithiumIonBankModel::batteryCellBalancingCount() const
{
    return m_batteryCellBalancingCount;
}
qint16 SunSpecLithiumIonBankModel::cellV_SF() const
{
    return m_cellV_SF;
}
qint16 SunSpecLithiumIonBankModel::modTmp_SF() const
{
    return m_modTmp_SF;
}
qint16 SunSpecLithiumIonBankModel::a_SF() const
{
    return m_a_SF;
}
qint16 SunSpecLithiumIonBankModel::soH_SF() const
{
    return m_soH_SF;
}
qint16 SunSpecLithiumIonBankModel::soC_SF() const
{
    return m_soC_SF;
}
qint16 SunSpecLithiumIonBankModel::v_SF() const
{
    return m_v_SF;
}
void SunSpecLithiumIonBankModel::initDataPoints()
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

    SunSpecDataPoint stringCountDataPoint;
    stringCountDataPoint.setName("NStr");
    stringCountDataPoint.setLabel("String Count");
    stringCountDataPoint.setDescription("Number of strings in the bank.");
    stringCountDataPoint.setMandatory(true);
    stringCountDataPoint.setSize(1);
    stringCountDataPoint.setAddressOffset(2);
    stringCountDataPoint.setBlockOffset(0);
    stringCountDataPoint.setSunSpecDataType("uint16");
    stringCountDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringCountDataPoint.name(), stringCountDataPoint);

    SunSpecDataPoint connectedStringCountDataPoint;
    connectedStringCountDataPoint.setName("NStrCon");
    connectedStringCountDataPoint.setLabel("Connected String Count");
    connectedStringCountDataPoint.setDescription("Number of strings with contactor closed.");
    connectedStringCountDataPoint.setMandatory(true);
    connectedStringCountDataPoint.setSize(1);
    connectedStringCountDataPoint.setAddressOffset(3);
    connectedStringCountDataPoint.setBlockOffset(1);
    connectedStringCountDataPoint.setSunSpecDataType("uint16");
    connectedStringCountDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(connectedStringCountDataPoint.name(), connectedStringCountDataPoint);

    SunSpecDataPoint maxModuleTemperatureDataPoint;
    maxModuleTemperatureDataPoint.setName("ModTmpMax");
    maxModuleTemperatureDataPoint.setLabel("Max Module Temperature");
    maxModuleTemperatureDataPoint.setDescription("Maximum temperature for all modules in the bank.");
    maxModuleTemperatureDataPoint.setMandatory(true);
    maxModuleTemperatureDataPoint.setSize(1);
    maxModuleTemperatureDataPoint.setAddressOffset(4);
    maxModuleTemperatureDataPoint.setBlockOffset(2);
    maxModuleTemperatureDataPoint.setScaleFactorName("ModTmp_SF");
    maxModuleTemperatureDataPoint.setSunSpecDataType("int16");
    maxModuleTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxModuleTemperatureDataPoint.name(), maxModuleTemperatureDataPoint);

    SunSpecDataPoint maxModuleTemperatureStringDataPoint;
    maxModuleTemperatureStringDataPoint.setName("ModTmpMaxStr");
    maxModuleTemperatureStringDataPoint.setLabel("Max Module Temperature String");
    maxModuleTemperatureStringDataPoint.setDescription("String containing the module with maximum temperature.");
    maxModuleTemperatureStringDataPoint.setSize(1);
    maxModuleTemperatureStringDataPoint.setAddressOffset(5);
    maxModuleTemperatureStringDataPoint.setBlockOffset(3);
    maxModuleTemperatureStringDataPoint.setSunSpecDataType("uint16");
    maxModuleTemperatureStringDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxModuleTemperatureStringDataPoint.name(), maxModuleTemperatureStringDataPoint);

    SunSpecDataPoint maxModuleTemperatureModuleDataPoint;
    maxModuleTemperatureModuleDataPoint.setName("ModTmpMaxMod");
    maxModuleTemperatureModuleDataPoint.setLabel("Max Module Temperature Module");
    maxModuleTemperatureModuleDataPoint.setDescription("Module with maximum temperature.");
    maxModuleTemperatureModuleDataPoint.setSize(1);
    maxModuleTemperatureModuleDataPoint.setAddressOffset(6);
    maxModuleTemperatureModuleDataPoint.setBlockOffset(4);
    maxModuleTemperatureModuleDataPoint.setSunSpecDataType("uint16");
    maxModuleTemperatureModuleDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxModuleTemperatureModuleDataPoint.name(), maxModuleTemperatureModuleDataPoint);

    SunSpecDataPoint minModuleTemperatureDataPoint;
    minModuleTemperatureDataPoint.setName("ModTmpMin");
    minModuleTemperatureDataPoint.setLabel("Min Module Temperature");
    minModuleTemperatureDataPoint.setDescription("Minimum temperature for all modules in the bank.");
    minModuleTemperatureDataPoint.setUnits("C");
    minModuleTemperatureDataPoint.setMandatory(true);
    minModuleTemperatureDataPoint.setSize(1);
    minModuleTemperatureDataPoint.setAddressOffset(7);
    minModuleTemperatureDataPoint.setBlockOffset(5);
    minModuleTemperatureDataPoint.setScaleFactorName("ModTmp_SF");
    minModuleTemperatureDataPoint.setSunSpecDataType("int16");
    minModuleTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minModuleTemperatureDataPoint.name(), minModuleTemperatureDataPoint);

    SunSpecDataPoint minModuleTemperatureStringDataPoint;
    minModuleTemperatureStringDataPoint.setName("ModTmpMinStr");
    minModuleTemperatureStringDataPoint.setLabel("Min Module Temperature String");
    minModuleTemperatureStringDataPoint.setDescription("String containing the module with minimum temperature.");
    minModuleTemperatureStringDataPoint.setSize(1);
    minModuleTemperatureStringDataPoint.setAddressOffset(8);
    minModuleTemperatureStringDataPoint.setBlockOffset(6);
    minModuleTemperatureStringDataPoint.setSunSpecDataType("uint16");
    minModuleTemperatureStringDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minModuleTemperatureStringDataPoint.name(), minModuleTemperatureStringDataPoint);

    SunSpecDataPoint minModuleTemperatureModuleDataPoint;
    minModuleTemperatureModuleDataPoint.setName("ModTmpMinMod");
    minModuleTemperatureModuleDataPoint.setLabel("Min Module Temperature Module");
    minModuleTemperatureModuleDataPoint.setDescription("Module with minimum temperature.");
    minModuleTemperatureModuleDataPoint.setSize(1);
    minModuleTemperatureModuleDataPoint.setAddressOffset(9);
    minModuleTemperatureModuleDataPoint.setBlockOffset(7);
    minModuleTemperatureModuleDataPoint.setSunSpecDataType("uint16");
    minModuleTemperatureModuleDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minModuleTemperatureModuleDataPoint.name(), minModuleTemperatureModuleDataPoint);

    SunSpecDataPoint averageModuleTemperatureDataPoint;
    averageModuleTemperatureDataPoint.setName("ModTmpAvg");
    averageModuleTemperatureDataPoint.setLabel("Average Module Temperature");
    averageModuleTemperatureDataPoint.setDescription("Average temperature for all modules in the bank.");
    averageModuleTemperatureDataPoint.setSize(1);
    averageModuleTemperatureDataPoint.setAddressOffset(10);
    averageModuleTemperatureDataPoint.setBlockOffset(8);
    averageModuleTemperatureDataPoint.setSunSpecDataType("int16");
    averageModuleTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(averageModuleTemperatureDataPoint.name(), averageModuleTemperatureDataPoint);

    SunSpecDataPoint maxStringVoltageDataPoint;
    maxStringVoltageDataPoint.setName("StrVMax");
    maxStringVoltageDataPoint.setLabel("Max String Voltage");
    maxStringVoltageDataPoint.setDescription("Maximum string voltage for all strings in the bank.");
    maxStringVoltageDataPoint.setUnits("V");
    maxStringVoltageDataPoint.setSize(1);
    maxStringVoltageDataPoint.setAddressOffset(11);
    maxStringVoltageDataPoint.setBlockOffset(9);
    maxStringVoltageDataPoint.setScaleFactorName("V_SF");
    maxStringVoltageDataPoint.setSunSpecDataType("uint16");
    maxStringVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxStringVoltageDataPoint.name(), maxStringVoltageDataPoint);

    SunSpecDataPoint maxStringVoltageStringDataPoint;
    maxStringVoltageStringDataPoint.setName("StrVMaxStr");
    maxStringVoltageStringDataPoint.setLabel("Max String Voltage String");
    maxStringVoltageStringDataPoint.setDescription("String with maximum voltage.");
    maxStringVoltageStringDataPoint.setSize(1);
    maxStringVoltageStringDataPoint.setAddressOffset(12);
    maxStringVoltageStringDataPoint.setBlockOffset(10);
    maxStringVoltageStringDataPoint.setSunSpecDataType("uint16");
    maxStringVoltageStringDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxStringVoltageStringDataPoint.name(), maxStringVoltageStringDataPoint);

    SunSpecDataPoint minStringVoltageDataPoint;
    minStringVoltageDataPoint.setName("StrVMin");
    minStringVoltageDataPoint.setLabel("Min String Voltage");
    minStringVoltageDataPoint.setDescription("Minimum string voltage for all strings in the bank.");
    minStringVoltageDataPoint.setUnits("V");
    minStringVoltageDataPoint.setSize(1);
    minStringVoltageDataPoint.setAddressOffset(13);
    minStringVoltageDataPoint.setBlockOffset(11);
    minStringVoltageDataPoint.setScaleFactorName("V_SF");
    minStringVoltageDataPoint.setSunSpecDataType("uint16");
    minStringVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minStringVoltageDataPoint.name(), minStringVoltageDataPoint);

    SunSpecDataPoint minStringVoltageStringDataPoint;
    minStringVoltageStringDataPoint.setName("StrVMinStr");
    minStringVoltageStringDataPoint.setLabel("Min String Voltage String");
    minStringVoltageStringDataPoint.setDescription("String with minimum voltage.");
    minStringVoltageStringDataPoint.setSize(1);
    minStringVoltageStringDataPoint.setAddressOffset(14);
    minStringVoltageStringDataPoint.setBlockOffset(12);
    minStringVoltageStringDataPoint.setSunSpecDataType("uint16");
    minStringVoltageStringDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minStringVoltageStringDataPoint.name(), minStringVoltageStringDataPoint);

    SunSpecDataPoint averageStringVoltageDataPoint;
    averageStringVoltageDataPoint.setName("StrVAvg");
    averageStringVoltageDataPoint.setLabel("Average String Voltage");
    averageStringVoltageDataPoint.setDescription("Average string voltage for all strings in the bank.");
    averageStringVoltageDataPoint.setUnits("V");
    averageStringVoltageDataPoint.setSize(1);
    averageStringVoltageDataPoint.setAddressOffset(15);
    averageStringVoltageDataPoint.setBlockOffset(13);
    averageStringVoltageDataPoint.setScaleFactorName("V_SF");
    averageStringVoltageDataPoint.setSunSpecDataType("uint16");
    averageStringVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(averageStringVoltageDataPoint.name(), averageStringVoltageDataPoint);

    SunSpecDataPoint maxStringCurrentDataPoint;
    maxStringCurrentDataPoint.setName("StrAMax");
    maxStringCurrentDataPoint.setLabel("Max String Current");
    maxStringCurrentDataPoint.setDescription("Maximum current of any string in the bank.");
    maxStringCurrentDataPoint.setUnits("A");
    maxStringCurrentDataPoint.setSize(1);
    maxStringCurrentDataPoint.setAddressOffset(16);
    maxStringCurrentDataPoint.setBlockOffset(14);
    maxStringCurrentDataPoint.setScaleFactorName("A_SF");
    maxStringCurrentDataPoint.setSunSpecDataType("int16");
    maxStringCurrentDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxStringCurrentDataPoint.name(), maxStringCurrentDataPoint);

    SunSpecDataPoint maxStringCurrentStringDataPoint;
    maxStringCurrentStringDataPoint.setName("StrAMaxStr");
    maxStringCurrentStringDataPoint.setLabel("Max String Current String");
    maxStringCurrentStringDataPoint.setDescription("String with the maximum current.");
    maxStringCurrentStringDataPoint.setSize(1);
    maxStringCurrentStringDataPoint.setAddressOffset(17);
    maxStringCurrentStringDataPoint.setBlockOffset(15);
    maxStringCurrentStringDataPoint.setSunSpecDataType("uint16");
    maxStringCurrentStringDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxStringCurrentStringDataPoint.name(), maxStringCurrentStringDataPoint);

    SunSpecDataPoint minStringCurrentDataPoint;
    minStringCurrentDataPoint.setName("StrAMin");
    minStringCurrentDataPoint.setLabel("Min String Current");
    minStringCurrentDataPoint.setDescription("Minimum current of any string in the bank.");
    minStringCurrentDataPoint.setUnits("A");
    minStringCurrentDataPoint.setSize(1);
    minStringCurrentDataPoint.setAddressOffset(18);
    minStringCurrentDataPoint.setBlockOffset(16);
    minStringCurrentDataPoint.setScaleFactorName("A_SF");
    minStringCurrentDataPoint.setSunSpecDataType("int16");
    minStringCurrentDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minStringCurrentDataPoint.name(), minStringCurrentDataPoint);

    SunSpecDataPoint minStringCurrentStringDataPoint;
    minStringCurrentStringDataPoint.setName("StrAMinStr");
    minStringCurrentStringDataPoint.setLabel("Min String Current String");
    minStringCurrentStringDataPoint.setDescription("String with the minimum current.");
    minStringCurrentStringDataPoint.setSize(1);
    minStringCurrentStringDataPoint.setAddressOffset(19);
    minStringCurrentStringDataPoint.setBlockOffset(17);
    minStringCurrentStringDataPoint.setSunSpecDataType("uint16");
    minStringCurrentStringDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minStringCurrentStringDataPoint.name(), minStringCurrentStringDataPoint);

    SunSpecDataPoint averageStringCurrentDataPoint;
    averageStringCurrentDataPoint.setName("StrAAvg");
    averageStringCurrentDataPoint.setLabel("Average String Current");
    averageStringCurrentDataPoint.setDescription("Average string current for all strings in the bank.");
    averageStringCurrentDataPoint.setUnits("A");
    averageStringCurrentDataPoint.setSize(1);
    averageStringCurrentDataPoint.setAddressOffset(20);
    averageStringCurrentDataPoint.setBlockOffset(18);
    averageStringCurrentDataPoint.setScaleFactorName("A_SF");
    averageStringCurrentDataPoint.setSunSpecDataType("int16");
    averageStringCurrentDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(averageStringCurrentDataPoint.name(), averageStringCurrentDataPoint);

    SunSpecDataPoint batteryCellBalancingCountDataPoint;
    batteryCellBalancingCountDataPoint.setName("NCellBal");
    batteryCellBalancingCountDataPoint.setLabel("Battery Cell Balancing Count");
    batteryCellBalancingCountDataPoint.setDescription("Total number of cells that are currently being balanced.");
    batteryCellBalancingCountDataPoint.setSize(1);
    batteryCellBalancingCountDataPoint.setAddressOffset(21);
    batteryCellBalancingCountDataPoint.setBlockOffset(19);
    batteryCellBalancingCountDataPoint.setSunSpecDataType("uint16");
    batteryCellBalancingCountDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(batteryCellBalancingCountDataPoint.name(), batteryCellBalancingCountDataPoint);

    SunSpecDataPoint cellV_SFDataPoint;
    cellV_SFDataPoint.setName("CellV_SF");
    cellV_SFDataPoint.setDescription("Scale factor for cell voltage.");
    cellV_SFDataPoint.setMandatory(true);
    cellV_SFDataPoint.setSize(1);
    cellV_SFDataPoint.setAddressOffset(22);
    cellV_SFDataPoint.setBlockOffset(20);
    cellV_SFDataPoint.setSunSpecDataType("sunssf");
    cellV_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(cellV_SFDataPoint.name(), cellV_SFDataPoint);

    SunSpecDataPoint modTmp_SFDataPoint;
    modTmp_SFDataPoint.setName("ModTmp_SF");
    modTmp_SFDataPoint.setDescription("Scale factor for module temperatures.");
    modTmp_SFDataPoint.setMandatory(true);
    modTmp_SFDataPoint.setSize(1);
    modTmp_SFDataPoint.setAddressOffset(23);
    modTmp_SFDataPoint.setBlockOffset(21);
    modTmp_SFDataPoint.setSunSpecDataType("sunssf");
    modTmp_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(modTmp_SFDataPoint.name(), modTmp_SFDataPoint);

    SunSpecDataPoint a_SFDataPoint;
    a_SFDataPoint.setName("A_SF");
    a_SFDataPoint.setDescription("Scale factor for string currents.");
    a_SFDataPoint.setMandatory(true);
    a_SFDataPoint.setSize(1);
    a_SFDataPoint.setAddressOffset(24);
    a_SFDataPoint.setBlockOffset(22);
    a_SFDataPoint.setSunSpecDataType("sunssf");
    a_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(a_SFDataPoint.name(), a_SFDataPoint);

    SunSpecDataPoint soH_SFDataPoint;
    soH_SFDataPoint.setName("SoH_SF");
    soH_SFDataPoint.setDescription("Scale factor for string state of health.");
    soH_SFDataPoint.setSize(1);
    soH_SFDataPoint.setAddressOffset(25);
    soH_SFDataPoint.setBlockOffset(23);
    soH_SFDataPoint.setSunSpecDataType("sunssf");
    soH_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(soH_SFDataPoint.name(), soH_SFDataPoint);

    SunSpecDataPoint soC_SFDataPoint;
    soC_SFDataPoint.setName("SoC_SF");
    soC_SFDataPoint.setDescription("Scale factor for string state of charge.");
    soC_SFDataPoint.setMandatory(true);
    soC_SFDataPoint.setSize(1);
    soC_SFDataPoint.setAddressOffset(26);
    soC_SFDataPoint.setBlockOffset(24);
    soC_SFDataPoint.setSunSpecDataType("sunssf");
    soC_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(soC_SFDataPoint.name(), soC_SFDataPoint);

    SunSpecDataPoint v_SFDataPoint;
    v_SFDataPoint.setName("V_SF");
    v_SFDataPoint.setDescription("Scale factor for string voltage.");
    v_SFDataPoint.setSize(1);
    v_SFDataPoint.setAddressOffset(27);
    v_SFDataPoint.setBlockOffset(25);
    v_SFDataPoint.setSunSpecDataType("sunssf");
    v_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v_SFDataPoint.name(), v_SFDataPoint);

}

void SunSpecLithiumIonBankModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("CellV_SF").isValid())
        m_cellV_SF = m_dataPoints.value("CellV_SF").toInt16();

    if (m_dataPoints.value("ModTmp_SF").isValid())
        m_modTmp_SF = m_dataPoints.value("ModTmp_SF").toInt16();

    if (m_dataPoints.value("A_SF").isValid())
        m_a_SF = m_dataPoints.value("A_SF").toInt16();

    if (m_dataPoints.value("SoH_SF").isValid())
        m_soH_SF = m_dataPoints.value("SoH_SF").toInt16();

    if (m_dataPoints.value("SoC_SF").isValid())
        m_soC_SF = m_dataPoints.value("SoC_SF").toInt16();

    if (m_dataPoints.value("V_SF").isValid())
        m_v_SF = m_dataPoints.value("V_SF").toInt16();


    // Update properties according to the data point type
    if (m_dataPoints.value("NStr").isValid())
        m_stringCount = m_dataPoints.value("NStr").toUInt16();

    if (m_dataPoints.value("NStrCon").isValid())
        m_connectedStringCount = m_dataPoints.value("NStrCon").toUInt16();

    if (m_dataPoints.value("ModTmpMax").isValid())
        m_maxModuleTemperature = m_dataPoints.value("ModTmpMax").toFloatWithSSF(m_modTmp_SF);

    if (m_dataPoints.value("ModTmpMaxStr").isValid())
        m_maxModuleTemperatureString = m_dataPoints.value("ModTmpMaxStr").toUInt16();

    if (m_dataPoints.value("ModTmpMaxMod").isValid())
        m_maxModuleTemperatureModule = m_dataPoints.value("ModTmpMaxMod").toUInt16();

    if (m_dataPoints.value("ModTmpMin").isValid())
        m_minModuleTemperature = m_dataPoints.value("ModTmpMin").toFloatWithSSF(m_modTmp_SF);

    if (m_dataPoints.value("ModTmpMinStr").isValid())
        m_minModuleTemperatureString = m_dataPoints.value("ModTmpMinStr").toUInt16();

    if (m_dataPoints.value("ModTmpMinMod").isValid())
        m_minModuleTemperatureModule = m_dataPoints.value("ModTmpMinMod").toUInt16();

    if (m_dataPoints.value("ModTmpAvg").isValid())
        m_averageModuleTemperature = m_dataPoints.value("ModTmpAvg").toInt16();

    if (m_dataPoints.value("StrVMax").isValid())
        m_maxStringVoltage = m_dataPoints.value("StrVMax").toFloatWithSSF(m_v_SF);

    if (m_dataPoints.value("StrVMaxStr").isValid())
        m_maxStringVoltageString = m_dataPoints.value("StrVMaxStr").toUInt16();

    if (m_dataPoints.value("StrVMin").isValid())
        m_minStringVoltage = m_dataPoints.value("StrVMin").toFloatWithSSF(m_v_SF);

    if (m_dataPoints.value("StrVMinStr").isValid())
        m_minStringVoltageString = m_dataPoints.value("StrVMinStr").toUInt16();

    if (m_dataPoints.value("StrVAvg").isValid())
        m_averageStringVoltage = m_dataPoints.value("StrVAvg").toFloatWithSSF(m_v_SF);

    if (m_dataPoints.value("StrAMax").isValid())
        m_maxStringCurrent = m_dataPoints.value("StrAMax").toFloatWithSSF(m_a_SF);

    if (m_dataPoints.value("StrAMaxStr").isValid())
        m_maxStringCurrentString = m_dataPoints.value("StrAMaxStr").toUInt16();

    if (m_dataPoints.value("StrAMin").isValid())
        m_minStringCurrent = m_dataPoints.value("StrAMin").toFloatWithSSF(m_a_SF);

    if (m_dataPoints.value("StrAMinStr").isValid())
        m_minStringCurrentString = m_dataPoints.value("StrAMinStr").toUInt16();

    if (m_dataPoints.value("StrAAvg").isValid())
        m_averageStringCurrent = m_dataPoints.value("StrAAvg").toFloatWithSSF(m_a_SF);

    if (m_dataPoints.value("NCellBal").isValid())
        m_batteryCellBalancingCount = m_dataPoints.value("NCellBal").toUInt16();

    if (m_dataPoints.value("CellV_SF").isValid())
        m_cellV_SF = m_dataPoints.value("CellV_SF").toInt16();

    if (m_dataPoints.value("ModTmp_SF").isValid())
        m_modTmp_SF = m_dataPoints.value("ModTmp_SF").toInt16();

    if (m_dataPoints.value("A_SF").isValid())
        m_a_SF = m_dataPoints.value("A_SF").toInt16();

    if (m_dataPoints.value("SoH_SF").isValid())
        m_soH_SF = m_dataPoints.value("SoH_SF").toInt16();

    if (m_dataPoints.value("SoC_SF").isValid())
        m_soC_SF = m_dataPoints.value("SoC_SF").toInt16();

    if (m_dataPoints.value("V_SF").isValid())
        m_v_SF = m_dataPoints.value("V_SF").toInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

void SunSpecLithiumIonBankModel::setupRepeatingBlocks()
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
                                        << ", extra bytes:"
                                        << repeatingBlocksDataSize % m_repeatingBlockLength
                                        << "). Repeating blocks will not be handled!";
        return;
    }
    const auto numberOfBlocks = repeatingBlocksDataSize / m_repeatingBlockLength;
    const auto repeatingBlocksOffset = m_fixedBlockLength + headerLength;
    for (int i = 0; i < numberOfBlocks; ++i) {
        const auto blockStartRegister = static_cast<quint16>(modbusStartRegister() + repeatingBlocksOffset + m_repeatingBlockLength * i);
        const auto block = new SunSpecLithiumIonBankModelRepeatingBlock(i, m_repeatingBlockLength, blockStartRegister, this);
        m_repeatingBlocks.append(block);
    }
}

QDebug operator<<(QDebug debug, SunSpecLithiumIonBankModel *model)
{
    debug.nospace().noquote() << "SunSpecLithiumIonBankModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("NStr") << "-->";
    if (model->dataPoints().value("NStr").isValid()) {
        debug.nospace().noquote() << model->stringCount() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("NStrCon") << "-->";
    if (model->dataPoints().value("NStrCon").isValid()) {
        debug.nospace().noquote() << model->connectedStringCount() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMax") << "-->";
    if (model->dataPoints().value("ModTmpMax").isValid()) {
        debug.nospace().noquote() << model->maxModuleTemperature() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMaxStr") << "-->";
    if (model->dataPoints().value("ModTmpMaxStr").isValid()) {
        debug.nospace().noquote() << model->maxModuleTemperatureString() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMaxMod") << "-->";
    if (model->dataPoints().value("ModTmpMaxMod").isValid()) {
        debug.nospace().noquote() << model->maxModuleTemperatureModule() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMin") << "-->";
    if (model->dataPoints().value("ModTmpMin").isValid()) {
        debug.nospace().noquote() << model->minModuleTemperature() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMinStr") << "-->";
    if (model->dataPoints().value("ModTmpMinStr").isValid()) {
        debug.nospace().noquote() << model->minModuleTemperatureString() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMinMod") << "-->";
    if (model->dataPoints().value("ModTmpMinMod").isValid()) {
        debug.nospace().noquote() << model->minModuleTemperatureModule() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpAvg") << "-->";
    if (model->dataPoints().value("ModTmpAvg").isValid()) {
        debug.nospace().noquote() << model->averageModuleTemperature() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("StrVMax") << "-->";
    if (model->dataPoints().value("StrVMax").isValid()) {
        debug.nospace().noquote() << model->maxStringVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("StrVMaxStr") << "-->";
    if (model->dataPoints().value("StrVMaxStr").isValid()) {
        debug.nospace().noquote() << model->maxStringVoltageString() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("StrVMin") << "-->";
    if (model->dataPoints().value("StrVMin").isValid()) {
        debug.nospace().noquote() << model->minStringVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("StrVMinStr") << "-->";
    if (model->dataPoints().value("StrVMinStr").isValid()) {
        debug.nospace().noquote() << model->minStringVoltageString() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("StrVAvg") << "-->";
    if (model->dataPoints().value("StrVAvg").isValid()) {
        debug.nospace().noquote() << model->averageStringVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("StrAMax") << "-->";
    if (model->dataPoints().value("StrAMax").isValid()) {
        debug.nospace().noquote() << model->maxStringCurrent() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("StrAMaxStr") << "-->";
    if (model->dataPoints().value("StrAMaxStr").isValid()) {
        debug.nospace().noquote() << model->maxStringCurrentString() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("StrAMin") << "-->";
    if (model->dataPoints().value("StrAMin").isValid()) {
        debug.nospace().noquote() << model->minStringCurrent() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("StrAMinStr") << "-->";
    if (model->dataPoints().value("StrAMinStr").isValid()) {
        debug.nospace().noquote() << model->minStringCurrentString() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("StrAAvg") << "-->";
    if (model->dataPoints().value("StrAAvg").isValid()) {
        debug.nospace().noquote() << model->averageStringCurrent() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("NCellBal") << "-->";
    if (model->dataPoints().value("NCellBal").isValid()) {
        debug.nospace().noquote() << model->batteryCellBalancingCount() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
