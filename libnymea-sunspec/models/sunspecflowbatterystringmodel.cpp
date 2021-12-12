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

#include "sunspecflowbatterystringmodel.h"
#include "sunspecconnection.h"

SunSpecFlowBatteryStringModelRepeatingBlock::SunSpecFlowBatteryStringModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecFlowBatteryStringModel *parent) :
    SunSpecModelRepeatingBlock(blockIndex, blockSize, modbusStartRegister, parent)
{
    m_byteOrder = parent->byteOrder();
    initDataPoints();
}

QString SunSpecFlowBatteryStringModelRepeatingBlock::name() const
{
    return "module";
}

SunSpecFlowBatteryStringModel *SunSpecFlowBatteryStringModelRepeatingBlock::parentModel() const
{
    return m_parentModel;
}

quint16 SunSpecFlowBatteryStringModelRepeatingBlock::moduleIndex() const
{
    return m_moduleIndex;
}
quint16 SunSpecFlowBatteryStringModelRepeatingBlock::stackCount() const
{
    return m_stackCount;
}
SunSpecFlowBatteryStringModelRepeatingBlock::ModstFlags SunSpecFlowBatteryStringModelRepeatingBlock::moduleStatus() const
{
    return m_moduleStatus;
}
float SunSpecFlowBatteryStringModelRepeatingBlock::moduleStateOfCharge() const
{
    return m_moduleStateOfCharge;
}
float SunSpecFlowBatteryStringModelRepeatingBlock::openCircuitVoltage() const
{
    return m_openCircuitVoltage;
}
float SunSpecFlowBatteryStringModelRepeatingBlock::externalVoltage() const
{
    return m_externalVoltage;
}
float SunSpecFlowBatteryStringModelRepeatingBlock::maximumCellVoltage() const
{
    return m_maximumCellVoltage;
}
quint16 SunSpecFlowBatteryStringModelRepeatingBlock::maxCellVoltageCell() const
{
    return m_maxCellVoltageCell;
}
float SunSpecFlowBatteryStringModelRepeatingBlock::minimumCellVoltage() const
{
    return m_minimumCellVoltage;
}
quint16 SunSpecFlowBatteryStringModelRepeatingBlock::minCellVoltageCell() const
{
    return m_minCellVoltageCell;
}
float SunSpecFlowBatteryStringModelRepeatingBlock::averageCellVoltage() const
{
    return m_averageCellVoltage;
}
float SunSpecFlowBatteryStringModelRepeatingBlock::anolyteTemperature() const
{
    return m_anolyteTemperature;
}
float SunSpecFlowBatteryStringModelRepeatingBlock::catholyteTemperature() const
{
    return m_catholyteTemperature;
}
SunSpecFlowBatteryStringModelRepeatingBlock::ModconstFlags SunSpecFlowBatteryStringModelRepeatingBlock::contactorStatus() const
{
    return m_contactorStatus;
}
SunSpecFlowBatteryStringModelRepeatingBlock::Modevt1Flags SunSpecFlowBatteryStringModelRepeatingBlock::moduleEvent1() const
{
    return m_moduleEvent1;
}
SunSpecFlowBatteryStringModelRepeatingBlock::Modevt2Flags SunSpecFlowBatteryStringModelRepeatingBlock::moduleEvent2() const
{
    return m_moduleEvent2;
}
SunSpecFlowBatteryStringModelRepeatingBlock::Modconfail SunSpecFlowBatteryStringModelRepeatingBlock::connectionFailureReason() const
{
    return m_connectionFailureReason;
}
SunSpecFlowBatteryStringModelRepeatingBlock::Modsetena SunSpecFlowBatteryStringModelRepeatingBlock::enableDisableModule() const
{
    return m_enableDisableModule;
}

QModbusReply *SunSpecFlowBatteryStringModelRepeatingBlock::setEnableDisableModule(Modsetena enableDisableModule)
{
    SunSpecDataPoint dp = m_dataPoints.value("ModSetEna");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(enableDisableModule));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

SunSpecFlowBatteryStringModelRepeatingBlock::Modsetcon SunSpecFlowBatteryStringModelRepeatingBlock::connectDisconnectModule() const
{
    return m_connectDisconnectModule;
}

QModbusReply *SunSpecFlowBatteryStringModelRepeatingBlock::setConnectDisconnectModule(Modsetcon connectDisconnectModule)
{
    SunSpecDataPoint dp = m_dataPoints.value("ModSetCon");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(connectDisconnectModule));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

SunSpecFlowBatteryStringModelRepeatingBlock::Moddisrsn SunSpecFlowBatteryStringModelRepeatingBlock::disabledReason() const
{
    return m_disabledReason;
}

void SunSpecFlowBatteryStringModelRepeatingBlock::initDataPoints()
{
    SunSpecDataPoint moduleIndexDataPoint;
    moduleIndexDataPoint.setName("ModIdx");
    moduleIndexDataPoint.setLabel("Module Index");
    moduleIndexDataPoint.setDescription("Index of the module within the string.");
    moduleIndexDataPoint.setMandatory(true);
    moduleIndexDataPoint.setSize(1);
    moduleIndexDataPoint.setAddressOffset(0);
    moduleIndexDataPoint.setSunSpecDataType("uint16");
    moduleIndexDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(moduleIndexDataPoint.name(), moduleIndexDataPoint);

    SunSpecDataPoint stackCountDataPoint;
    stackCountDataPoint.setName("ModNStk");
    stackCountDataPoint.setLabel("Stack Count");
    stackCountDataPoint.setDescription("Number of stacks in this module.");
    stackCountDataPoint.setMandatory(true);
    stackCountDataPoint.setSize(1);
    stackCountDataPoint.setAddressOffset(1);
    stackCountDataPoint.setSunSpecDataType("uint16");
    stackCountDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stackCountDataPoint.name(), stackCountDataPoint);

    SunSpecDataPoint moduleStatusDataPoint;
    moduleStatusDataPoint.setName("ModSt");
    moduleStatusDataPoint.setLabel("Module Status");
    moduleStatusDataPoint.setDescription("Current status of the module.");
    moduleStatusDataPoint.setMandatory(true);
    moduleStatusDataPoint.setSize(2);
    moduleStatusDataPoint.setAddressOffset(2);
    moduleStatusDataPoint.setBlockOffset(0);
    moduleStatusDataPoint.setSunSpecDataType("bitfield32");
    moduleStatusDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(moduleStatusDataPoint.name(), moduleStatusDataPoint);

    SunSpecDataPoint moduleStateOfChargeDataPoint;
    moduleStateOfChargeDataPoint.setName("ModSoC");
    moduleStateOfChargeDataPoint.setLabel("Module State of Charge");
    moduleStateOfChargeDataPoint.setDescription("State of charge for this module.");
    moduleStateOfChargeDataPoint.setUnits("%");
    moduleStateOfChargeDataPoint.setMandatory(true);
    moduleStateOfChargeDataPoint.setSize(1);
    moduleStateOfChargeDataPoint.setAddressOffset(4);
    moduleStateOfChargeDataPoint.setBlockOffset(2);
    moduleStateOfChargeDataPoint.setScaleFactorName("SoC_SF");
    moduleStateOfChargeDataPoint.setSunSpecDataType("uint16");
    moduleStateOfChargeDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(moduleStateOfChargeDataPoint.name(), moduleStateOfChargeDataPoint);

    SunSpecDataPoint openCircuitVoltageDataPoint;
    openCircuitVoltageDataPoint.setName("ModOCV");
    openCircuitVoltageDataPoint.setLabel("Open Circuit Voltage");
    openCircuitVoltageDataPoint.setDescription("Open circuit voltage for this module.");
    openCircuitVoltageDataPoint.setUnits("V");
    openCircuitVoltageDataPoint.setMandatory(true);
    openCircuitVoltageDataPoint.setSize(1);
    openCircuitVoltageDataPoint.setAddressOffset(5);
    openCircuitVoltageDataPoint.setBlockOffset(3);
    openCircuitVoltageDataPoint.setScaleFactorName("OCV_SF");
    openCircuitVoltageDataPoint.setSunSpecDataType("uint16");
    openCircuitVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(openCircuitVoltageDataPoint.name(), openCircuitVoltageDataPoint);

    SunSpecDataPoint externalVoltageDataPoint;
    externalVoltageDataPoint.setName("ModV");
    externalVoltageDataPoint.setLabel("External Voltage");
    externalVoltageDataPoint.setDescription("External voltage fo this module.");
    externalVoltageDataPoint.setUnits("V");
    externalVoltageDataPoint.setMandatory(true);
    externalVoltageDataPoint.setSize(1);
    externalVoltageDataPoint.setAddressOffset(6);
    externalVoltageDataPoint.setBlockOffset(4);
    externalVoltageDataPoint.setScaleFactorName("ModV_SF");
    externalVoltageDataPoint.setSunSpecDataType("uint16");
    externalVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(externalVoltageDataPoint.name(), externalVoltageDataPoint);

    SunSpecDataPoint maximumCellVoltageDataPoint;
    maximumCellVoltageDataPoint.setName("ModCellVMax");
    maximumCellVoltageDataPoint.setLabel("Maximum Cell Voltage");
    maximumCellVoltageDataPoint.setDescription("Maximum voltage for all cells in this module.");
    maximumCellVoltageDataPoint.setUnits("V");
    maximumCellVoltageDataPoint.setSize(1);
    maximumCellVoltageDataPoint.setAddressOffset(7);
    maximumCellVoltageDataPoint.setBlockOffset(5);
    maximumCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    maximumCellVoltageDataPoint.setSunSpecDataType("uint16");
    maximumCellVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maximumCellVoltageDataPoint.name(), maximumCellVoltageDataPoint);

    SunSpecDataPoint maxCellVoltageCellDataPoint;
    maxCellVoltageCellDataPoint.setName("ModCellVMaxCell");
    maxCellVoltageCellDataPoint.setLabel("Max Cell Voltage Cell");
    maxCellVoltageCellDataPoint.setDescription("Cell with the maximum cell voltage.");
    maxCellVoltageCellDataPoint.setSize(1);
    maxCellVoltageCellDataPoint.setAddressOffset(8);
    maxCellVoltageCellDataPoint.setBlockOffset(6);
    maxCellVoltageCellDataPoint.setSunSpecDataType("uint16");
    maxCellVoltageCellDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxCellVoltageCellDataPoint.name(), maxCellVoltageCellDataPoint);

    SunSpecDataPoint minimumCellVoltageDataPoint;
    minimumCellVoltageDataPoint.setName("ModCellVMin");
    minimumCellVoltageDataPoint.setLabel("Minimum Cell Voltage");
    minimumCellVoltageDataPoint.setDescription("Minimum voltage for all cells in this module.");
    minimumCellVoltageDataPoint.setUnits("V");
    minimumCellVoltageDataPoint.setSize(1);
    minimumCellVoltageDataPoint.setAddressOffset(9);
    minimumCellVoltageDataPoint.setBlockOffset(7);
    minimumCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    minimumCellVoltageDataPoint.setSunSpecDataType("uint16");
    minimumCellVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minimumCellVoltageDataPoint.name(), minimumCellVoltageDataPoint);

    SunSpecDataPoint minCellVoltageCellDataPoint;
    minCellVoltageCellDataPoint.setName("ModCellVMinCell");
    minCellVoltageCellDataPoint.setLabel("Min Cell Voltage Cell");
    minCellVoltageCellDataPoint.setDescription("Cell with the minimum cell voltage.");
    minCellVoltageCellDataPoint.setSize(1);
    minCellVoltageCellDataPoint.setAddressOffset(10);
    minCellVoltageCellDataPoint.setBlockOffset(8);
    minCellVoltageCellDataPoint.setSunSpecDataType("uint16");
    minCellVoltageCellDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minCellVoltageCellDataPoint.name(), minCellVoltageCellDataPoint);

    SunSpecDataPoint averageCellVoltageDataPoint;
    averageCellVoltageDataPoint.setName("ModCellVAvg");
    averageCellVoltageDataPoint.setLabel("Average Cell Voltage");
    averageCellVoltageDataPoint.setDescription("Average voltage for all cells in this module.");
    averageCellVoltageDataPoint.setUnits("V");
    averageCellVoltageDataPoint.setSize(1);
    averageCellVoltageDataPoint.setAddressOffset(11);
    averageCellVoltageDataPoint.setBlockOffset(9);
    averageCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    averageCellVoltageDataPoint.setSunSpecDataType("uint16");
    averageCellVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(averageCellVoltageDataPoint.name(), averageCellVoltageDataPoint);

    SunSpecDataPoint anolyteTemperatureDataPoint;
    anolyteTemperatureDataPoint.setName("ModAnoTmp");
    anolyteTemperatureDataPoint.setLabel("Anolyte Temperature");
    anolyteTemperatureDataPoint.setUnits("C");
    anolyteTemperatureDataPoint.setSize(1);
    anolyteTemperatureDataPoint.setAddressOffset(12);
    anolyteTemperatureDataPoint.setBlockOffset(10);
    anolyteTemperatureDataPoint.setScaleFactorName("Tmp_SF");
    anolyteTemperatureDataPoint.setSunSpecDataType("uint16");
    anolyteTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(anolyteTemperatureDataPoint.name(), anolyteTemperatureDataPoint);

    SunSpecDataPoint catholyteTemperatureDataPoint;
    catholyteTemperatureDataPoint.setName("ModCatTmp");
    catholyteTemperatureDataPoint.setLabel("Catholyte Temperature");
    catholyteTemperatureDataPoint.setUnits("C");
    catholyteTemperatureDataPoint.setSize(1);
    catholyteTemperatureDataPoint.setAddressOffset(13);
    catholyteTemperatureDataPoint.setBlockOffset(11);
    catholyteTemperatureDataPoint.setScaleFactorName("Tmp_SF");
    catholyteTemperatureDataPoint.setSunSpecDataType("uint16");
    catholyteTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(catholyteTemperatureDataPoint.name(), catholyteTemperatureDataPoint);

    SunSpecDataPoint contactorStatusDataPoint;
    contactorStatusDataPoint.setName("ModConSt");
    contactorStatusDataPoint.setLabel("Contactor Status");
    contactorStatusDataPoint.setSize(2);
    contactorStatusDataPoint.setAddressOffset(14);
    contactorStatusDataPoint.setBlockOffset(12);
    contactorStatusDataPoint.setSunSpecDataType("bitfield32");
    contactorStatusDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(contactorStatusDataPoint.name(), contactorStatusDataPoint);

    SunSpecDataPoint moduleEvent1DataPoint;
    moduleEvent1DataPoint.setName("ModEvt1");
    moduleEvent1DataPoint.setLabel("Module Event 1");
    moduleEvent1DataPoint.setDescription("Alarms, warnings and status values.  Bit flags.");
    moduleEvent1DataPoint.setMandatory(true);
    moduleEvent1DataPoint.setSize(2);
    moduleEvent1DataPoint.setAddressOffset(16);
    moduleEvent1DataPoint.setBlockOffset(14);
    moduleEvent1DataPoint.setSunSpecDataType("bitfield32");
    moduleEvent1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(moduleEvent1DataPoint.name(), moduleEvent1DataPoint);

    SunSpecDataPoint moduleEvent2DataPoint;
    moduleEvent2DataPoint.setName("ModEvt2");
    moduleEvent2DataPoint.setLabel("Module Event 2");
    moduleEvent2DataPoint.setDescription("Alarms, warnings and status values.  Bit flags.");
    moduleEvent2DataPoint.setMandatory(true);
    moduleEvent2DataPoint.setSize(2);
    moduleEvent2DataPoint.setAddressOffset(18);
    moduleEvent2DataPoint.setBlockOffset(16);
    moduleEvent2DataPoint.setSunSpecDataType("bitfield32");
    moduleEvent2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(moduleEvent2DataPoint.name(), moduleEvent2DataPoint);

    SunSpecDataPoint connectionFailureReasonDataPoint;
    connectionFailureReasonDataPoint.setName("ModConFail");
    connectionFailureReasonDataPoint.setLabel("Connection Failure Reason");
    connectionFailureReasonDataPoint.setSize(1);
    connectionFailureReasonDataPoint.setAddressOffset(20);
    connectionFailureReasonDataPoint.setBlockOffset(18);
    connectionFailureReasonDataPoint.setSunSpecDataType("enum16");
    connectionFailureReasonDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(connectionFailureReasonDataPoint.name(), connectionFailureReasonDataPoint);

    SunSpecDataPoint enableDisableModuleDataPoint;
    enableDisableModuleDataPoint.setName("ModSetEna");
    enableDisableModuleDataPoint.setLabel("Enable/Disable Module");
    enableDisableModuleDataPoint.setDescription("Enables and disables the module.");
    enableDisableModuleDataPoint.setSize(1);
    enableDisableModuleDataPoint.setAddressOffset(21);
    enableDisableModuleDataPoint.setBlockOffset(19);
    enableDisableModuleDataPoint.setSunSpecDataType("enum16");
    enableDisableModuleDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    enableDisableModuleDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(enableDisableModuleDataPoint.name(), enableDisableModuleDataPoint);

    SunSpecDataPoint connectDisconnectModuleDataPoint;
    connectDisconnectModuleDataPoint.setName("ModSetCon");
    connectDisconnectModuleDataPoint.setLabel("Connect/Disconnect Module ");
    connectDisconnectModuleDataPoint.setDescription("Connects and disconnects the module.");
    connectDisconnectModuleDataPoint.setSize(1);
    connectDisconnectModuleDataPoint.setAddressOffset(22);
    connectDisconnectModuleDataPoint.setBlockOffset(20);
    connectDisconnectModuleDataPoint.setSunSpecDataType("enum16");
    connectDisconnectModuleDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    connectDisconnectModuleDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(connectDisconnectModuleDataPoint.name(), connectDisconnectModuleDataPoint);

    SunSpecDataPoint disabledReasonDataPoint;
    disabledReasonDataPoint.setName("ModDisRsn");
    disabledReasonDataPoint.setLabel("Disabled Reason");
    disabledReasonDataPoint.setDescription("Reason why the module is currently disabled.");
    disabledReasonDataPoint.setSize(1);
    disabledReasonDataPoint.setAddressOffset(23);
    disabledReasonDataPoint.setBlockOffset(21);
    disabledReasonDataPoint.setSunSpecDataType("enum16");
    disabledReasonDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(disabledReasonDataPoint.name(), disabledReasonDataPoint);

}

void SunSpecFlowBatteryStringModelRepeatingBlock::processBlockData(const QVector<quint16> blockData)
{
    m_blockData = blockData;

    // Update properties according to the data point type
    if (m_dataPoints.value("ModIdx").isValid())
        m_moduleIndex = m_dataPoints.value("ModIdx").toUInt16();

    if (m_dataPoints.value("ModNStk").isValid())
        m_stackCount = m_dataPoints.value("ModNStk").toUInt16();

    if (m_dataPoints.value("ModSt").isValid())
        m_moduleStatus = static_cast<ModstFlags>(m_dataPoints.value("ModSt").toUInt32());

    if (m_dataPoints.value("ModSoC").isValid())
        m_moduleStateOfCharge = m_dataPoints.value("ModSoC").toFloatWithSSF(m_parentModel->soC_SF());

    if (m_dataPoints.value("ModOCV").isValid())
        m_openCircuitVoltage = m_dataPoints.value("ModOCV").toFloatWithSSF(m_parentModel->oCV_SF());

    if (m_dataPoints.value("ModV").isValid())
        m_externalVoltage = m_dataPoints.value("ModV").toFloatWithSSF(m_parentModel->modV_SF());

    if (m_dataPoints.value("ModCellVMax").isValid())
        m_maximumCellVoltage = m_dataPoints.value("ModCellVMax").toFloatWithSSF(m_parentModel->cellV_SF());

    if (m_dataPoints.value("ModCellVMaxCell").isValid())
        m_maxCellVoltageCell = m_dataPoints.value("ModCellVMaxCell").toUInt16();

    if (m_dataPoints.value("ModCellVMin").isValid())
        m_minimumCellVoltage = m_dataPoints.value("ModCellVMin").toFloatWithSSF(m_parentModel->cellV_SF());

    if (m_dataPoints.value("ModCellVMinCell").isValid())
        m_minCellVoltageCell = m_dataPoints.value("ModCellVMinCell").toUInt16();

    if (m_dataPoints.value("ModCellVAvg").isValid())
        m_averageCellVoltage = m_dataPoints.value("ModCellVAvg").toFloatWithSSF(m_parentModel->cellV_SF());

    if (m_dataPoints.value("ModAnoTmp").isValid())
        m_anolyteTemperature = m_dataPoints.value("ModAnoTmp").toFloatWithSSF(m_parentModel->tmp_SF());

    if (m_dataPoints.value("ModCatTmp").isValid())
        m_catholyteTemperature = m_dataPoints.value("ModCatTmp").toFloatWithSSF(m_parentModel->tmp_SF());

    if (m_dataPoints.value("ModConSt").isValid())
        m_contactorStatus = static_cast<ModconstFlags>(m_dataPoints.value("ModConSt").toUInt32());

    if (m_dataPoints.value("ModEvt1").isValid())
        m_moduleEvent1 = static_cast<Modevt1Flags>(m_dataPoints.value("ModEvt1").toUInt32());

    if (m_dataPoints.value("ModEvt2").isValid())
        m_moduleEvent2 = static_cast<Modevt2Flags>(m_dataPoints.value("ModEvt2").toUInt32());

    if (m_dataPoints.value("ModConFail").isValid())
        m_connectionFailureReason = static_cast<Modconfail>(m_dataPoints.value("ModConFail").toUInt16());

    if (m_dataPoints.value("ModSetEna").isValid())
        m_enableDisableModule = static_cast<Modsetena>(m_dataPoints.value("ModSetEna").toUInt16());

    if (m_dataPoints.value("ModSetCon").isValid())
        m_connectDisconnectModule = static_cast<Modsetcon>(m_dataPoints.value("ModSetCon").toUInt16());

    if (m_dataPoints.value("ModDisRsn").isValid())
        m_disabledReason = static_cast<Moddisrsn>(m_dataPoints.value("ModDisRsn").toUInt16());


    qCDebug(dcSunSpecModelData()) << this;
}


SunSpecFlowBatteryStringModel::SunSpecFlowBatteryStringModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 807, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixedAndRepeating;

    initDataPoints();
}

SunSpecFlowBatteryStringModel::~SunSpecFlowBatteryStringModel()
{

}

QString SunSpecFlowBatteryStringModel::name() const
{
    return "flow_battery_string";
}

QString SunSpecFlowBatteryStringModel::description() const
{
    return QString();
}

QString SunSpecFlowBatteryStringModel::label() const
{
    return "Flow Battery String Model";
}

quint16 SunSpecFlowBatteryStringModel::stringIndex() const
{
    return m_stringIndex;
}
quint16 SunSpecFlowBatteryStringModel::moduleCount() const
{
    return m_moduleCount;
}
quint16 SunSpecFlowBatteryStringModel::connectedModuleCount() const
{
    return m_connectedModuleCount;
}
float SunSpecFlowBatteryStringModel::maxModuleVoltage() const
{
    return m_maxModuleVoltage;
}
quint16 SunSpecFlowBatteryStringModel::maxModuleVoltageModule() const
{
    return m_maxModuleVoltageModule;
}
float SunSpecFlowBatteryStringModel::minModuleVoltage() const
{
    return m_minModuleVoltage;
}
quint16 SunSpecFlowBatteryStringModel::minModuleVoltageModule() const
{
    return m_minModuleVoltageModule;
}
float SunSpecFlowBatteryStringModel::averageModuleVoltage() const
{
    return m_averageModuleVoltage;
}
float SunSpecFlowBatteryStringModel::maxCellVoltage() const
{
    return m_maxCellVoltage;
}
quint16 SunSpecFlowBatteryStringModel::maxCellVoltageModule() const
{
    return m_maxCellVoltageModule;
}
quint16 SunSpecFlowBatteryStringModel::maxCellVoltageStack() const
{
    return m_maxCellVoltageStack;
}
float SunSpecFlowBatteryStringModel::minCellVoltage() const
{
    return m_minCellVoltage;
}
quint16 SunSpecFlowBatteryStringModel::minCellVoltageModule() const
{
    return m_minCellVoltageModule;
}
quint16 SunSpecFlowBatteryStringModel::minCellVoltageStack() const
{
    return m_minCellVoltageStack;
}
float SunSpecFlowBatteryStringModel::averageCellVoltage() const
{
    return m_averageCellVoltage;
}
float SunSpecFlowBatteryStringModel::maxTemperature() const
{
    return m_maxTemperature;
}
quint16 SunSpecFlowBatteryStringModel::maxTemperatureModule() const
{
    return m_maxTemperatureModule;
}
float SunSpecFlowBatteryStringModel::minTemperature() const
{
    return m_minTemperature;
}
quint16 SunSpecFlowBatteryStringModel::minTemperatureModule() const
{
    return m_minTemperatureModule;
}
float SunSpecFlowBatteryStringModel::averageTemperature() const
{
    return m_averageTemperature;
}
SunSpecFlowBatteryStringModel::Evt1Flags SunSpecFlowBatteryStringModel::stringEvent1() const
{
    return m_stringEvent1;
}
SunSpecFlowBatteryStringModel::Evt2Flags SunSpecFlowBatteryStringModel::stringEvent2() const
{
    return m_stringEvent2;
}
quint32 SunSpecFlowBatteryStringModel::vendorEventBitfield1() const
{
    return m_vendorEventBitfield1;
}
quint32 SunSpecFlowBatteryStringModel::vendorEventBitfield2() const
{
    return m_vendorEventBitfield2;
}
qint16 SunSpecFlowBatteryStringModel::modV_SF() const
{
    return m_modV_SF;
}
qint16 SunSpecFlowBatteryStringModel::cellV_SF() const
{
    return m_cellV_SF;
}
qint16 SunSpecFlowBatteryStringModel::tmp_SF() const
{
    return m_tmp_SF;
}
qint16 SunSpecFlowBatteryStringModel::soC_SF() const
{
    return m_soC_SF;
}
qint16 SunSpecFlowBatteryStringModel::oCV_SF() const
{
    return m_oCV_SF;
}
quint16 SunSpecFlowBatteryStringModel::pad1() const
{
    return m_pad1;
}
void SunSpecFlowBatteryStringModel::initDataPoints()
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

    SunSpecDataPoint stringIndexDataPoint;
    stringIndexDataPoint.setName("Idx");
    stringIndexDataPoint.setLabel("String Index");
    stringIndexDataPoint.setDescription("Index of the string within the bank.");
    stringIndexDataPoint.setMandatory(true);
    stringIndexDataPoint.setSize(1);
    stringIndexDataPoint.setAddressOffset(2);
    stringIndexDataPoint.setBlockOffset(0);
    stringIndexDataPoint.setSunSpecDataType("uint16");
    stringIndexDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringIndexDataPoint.name(), stringIndexDataPoint);

    SunSpecDataPoint moduleCountDataPoint;
    moduleCountDataPoint.setName("NMod");
    moduleCountDataPoint.setLabel("Module Count");
    moduleCountDataPoint.setDescription("Number of modules in this string.");
    moduleCountDataPoint.setMandatory(true);
    moduleCountDataPoint.setSize(1);
    moduleCountDataPoint.setAddressOffset(3);
    moduleCountDataPoint.setBlockOffset(1);
    moduleCountDataPoint.setSunSpecDataType("uint16");
    moduleCountDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(moduleCountDataPoint.name(), moduleCountDataPoint);

    SunSpecDataPoint connectedModuleCountDataPoint;
    connectedModuleCountDataPoint.setName("NModCon");
    connectedModuleCountDataPoint.setLabel("Connected Module Count");
    connectedModuleCountDataPoint.setDescription("Number of electrically connected modules in this string.");
    connectedModuleCountDataPoint.setMandatory(true);
    connectedModuleCountDataPoint.setSize(1);
    connectedModuleCountDataPoint.setAddressOffset(4);
    connectedModuleCountDataPoint.setBlockOffset(2);
    connectedModuleCountDataPoint.setSunSpecDataType("uint16");
    connectedModuleCountDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(connectedModuleCountDataPoint.name(), connectedModuleCountDataPoint);

    SunSpecDataPoint maxModuleVoltageDataPoint;
    maxModuleVoltageDataPoint.setName("ModVMax");
    maxModuleVoltageDataPoint.setLabel("Max Module Voltage");
    maxModuleVoltageDataPoint.setDescription("Maximum voltage for all modules in the string.");
    maxModuleVoltageDataPoint.setUnits("V");
    maxModuleVoltageDataPoint.setMandatory(true);
    maxModuleVoltageDataPoint.setSize(1);
    maxModuleVoltageDataPoint.setAddressOffset(5);
    maxModuleVoltageDataPoint.setBlockOffset(3);
    maxModuleVoltageDataPoint.setScaleFactorName("ModV_SF");
    maxModuleVoltageDataPoint.setSunSpecDataType("uint16");
    maxModuleVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxModuleVoltageDataPoint.name(), maxModuleVoltageDataPoint);

    SunSpecDataPoint maxModuleVoltageModuleDataPoint;
    maxModuleVoltageModuleDataPoint.setName("ModVMaxMod");
    maxModuleVoltageModuleDataPoint.setLabel("Max Module Voltage Module");
    maxModuleVoltageModuleDataPoint.setDescription("Module with the maximum voltage.");
    maxModuleVoltageModuleDataPoint.setSize(1);
    maxModuleVoltageModuleDataPoint.setAddressOffset(6);
    maxModuleVoltageModuleDataPoint.setBlockOffset(4);
    maxModuleVoltageModuleDataPoint.setSunSpecDataType("uint16");
    maxModuleVoltageModuleDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxModuleVoltageModuleDataPoint.name(), maxModuleVoltageModuleDataPoint);

    SunSpecDataPoint minModuleVoltageDataPoint;
    minModuleVoltageDataPoint.setName("ModVMin");
    minModuleVoltageDataPoint.setLabel("Min Module Voltage");
    minModuleVoltageDataPoint.setDescription("Minimum voltage for all modules in the string.");
    minModuleVoltageDataPoint.setUnits("V");
    minModuleVoltageDataPoint.setMandatory(true);
    minModuleVoltageDataPoint.setSize(1);
    minModuleVoltageDataPoint.setAddressOffset(7);
    minModuleVoltageDataPoint.setBlockOffset(5);
    minModuleVoltageDataPoint.setScaleFactorName("ModV_SF");
    minModuleVoltageDataPoint.setSunSpecDataType("uint16");
    minModuleVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minModuleVoltageDataPoint.name(), minModuleVoltageDataPoint);

    SunSpecDataPoint minModuleVoltageModuleDataPoint;
    minModuleVoltageModuleDataPoint.setName("ModVMinMod");
    minModuleVoltageModuleDataPoint.setLabel("Min Module Voltage Module");
    minModuleVoltageModuleDataPoint.setDescription("Module with the minimum voltage.");
    minModuleVoltageModuleDataPoint.setSize(1);
    minModuleVoltageModuleDataPoint.setAddressOffset(8);
    minModuleVoltageModuleDataPoint.setBlockOffset(6);
    minModuleVoltageModuleDataPoint.setSunSpecDataType("uint16");
    minModuleVoltageModuleDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minModuleVoltageModuleDataPoint.name(), minModuleVoltageModuleDataPoint);

    SunSpecDataPoint averageModuleVoltageDataPoint;
    averageModuleVoltageDataPoint.setName("ModVAvg");
    averageModuleVoltageDataPoint.setLabel("Average Module Voltage");
    averageModuleVoltageDataPoint.setDescription("Average voltage for all modules in the string.");
    averageModuleVoltageDataPoint.setUnits("V");
    averageModuleVoltageDataPoint.setMandatory(true);
    averageModuleVoltageDataPoint.setSize(1);
    averageModuleVoltageDataPoint.setAddressOffset(9);
    averageModuleVoltageDataPoint.setBlockOffset(7);
    averageModuleVoltageDataPoint.setScaleFactorName("ModV_SF");
    averageModuleVoltageDataPoint.setSunSpecDataType("uint16");
    averageModuleVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(averageModuleVoltageDataPoint.name(), averageModuleVoltageDataPoint);

    SunSpecDataPoint maxCellVoltageDataPoint;
    maxCellVoltageDataPoint.setName("CellVMax");
    maxCellVoltageDataPoint.setLabel("Max Cell Voltage");
    maxCellVoltageDataPoint.setDescription("Maximum voltage for all cells in the string.");
    maxCellVoltageDataPoint.setUnits("V");
    maxCellVoltageDataPoint.setSize(1);
    maxCellVoltageDataPoint.setAddressOffset(10);
    maxCellVoltageDataPoint.setBlockOffset(8);
    maxCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    maxCellVoltageDataPoint.setSunSpecDataType("uint16");
    maxCellVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxCellVoltageDataPoint.name(), maxCellVoltageDataPoint);

    SunSpecDataPoint maxCellVoltageModuleDataPoint;
    maxCellVoltageModuleDataPoint.setName("CellVMaxMod");
    maxCellVoltageModuleDataPoint.setLabel("Max Cell Voltage Module");
    maxCellVoltageModuleDataPoint.setDescription("Module containing the cell with the maximum voltage.");
    maxCellVoltageModuleDataPoint.setSize(1);
    maxCellVoltageModuleDataPoint.setAddressOffset(11);
    maxCellVoltageModuleDataPoint.setBlockOffset(9);
    maxCellVoltageModuleDataPoint.setSunSpecDataType("uint16");
    maxCellVoltageModuleDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxCellVoltageModuleDataPoint.name(), maxCellVoltageModuleDataPoint);

    SunSpecDataPoint maxCellVoltageStackDataPoint;
    maxCellVoltageStackDataPoint.setName("CellVMaxStk");
    maxCellVoltageStackDataPoint.setLabel("Max Cell Voltage Stack");
    maxCellVoltageStackDataPoint.setDescription("Stack containing the cell with the maximum voltage.");
    maxCellVoltageStackDataPoint.setSize(1);
    maxCellVoltageStackDataPoint.setAddressOffset(12);
    maxCellVoltageStackDataPoint.setBlockOffset(10);
    maxCellVoltageStackDataPoint.setSunSpecDataType("uint16");
    maxCellVoltageStackDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxCellVoltageStackDataPoint.name(), maxCellVoltageStackDataPoint);

    SunSpecDataPoint minCellVoltageDataPoint;
    minCellVoltageDataPoint.setName("CellVMin");
    minCellVoltageDataPoint.setLabel("Min Cell Voltage");
    minCellVoltageDataPoint.setDescription("Minimum voltage for all cells in the string.");
    minCellVoltageDataPoint.setUnits("V");
    minCellVoltageDataPoint.setSize(1);
    minCellVoltageDataPoint.setAddressOffset(13);
    minCellVoltageDataPoint.setBlockOffset(11);
    minCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    minCellVoltageDataPoint.setSunSpecDataType("uint16");
    minCellVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minCellVoltageDataPoint.name(), minCellVoltageDataPoint);

    SunSpecDataPoint minCellVoltageModuleDataPoint;
    minCellVoltageModuleDataPoint.setName("CellVMinMod");
    minCellVoltageModuleDataPoint.setLabel("Min Cell Voltage Module");
    minCellVoltageModuleDataPoint.setDescription("Module containing the cell with the minimum voltage.");
    minCellVoltageModuleDataPoint.setSize(1);
    minCellVoltageModuleDataPoint.setAddressOffset(14);
    minCellVoltageModuleDataPoint.setBlockOffset(12);
    minCellVoltageModuleDataPoint.setSunSpecDataType("uint16");
    minCellVoltageModuleDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minCellVoltageModuleDataPoint.name(), minCellVoltageModuleDataPoint);

    SunSpecDataPoint minCellVoltageStackDataPoint;
    minCellVoltageStackDataPoint.setName("CellVMinStk");
    minCellVoltageStackDataPoint.setLabel("Min Cell Voltage Stack");
    minCellVoltageStackDataPoint.setDescription("Stack containing the cell with the minimum voltage.");
    minCellVoltageStackDataPoint.setSize(1);
    minCellVoltageStackDataPoint.setAddressOffset(15);
    minCellVoltageStackDataPoint.setBlockOffset(13);
    minCellVoltageStackDataPoint.setSunSpecDataType("uint16");
    minCellVoltageStackDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minCellVoltageStackDataPoint.name(), minCellVoltageStackDataPoint);

    SunSpecDataPoint averageCellVoltageDataPoint;
    averageCellVoltageDataPoint.setName("CellVAvg");
    averageCellVoltageDataPoint.setLabel("Average Cell Voltage");
    averageCellVoltageDataPoint.setDescription("Average voltage for all cells in the string.");
    averageCellVoltageDataPoint.setUnits("V");
    averageCellVoltageDataPoint.setSize(1);
    averageCellVoltageDataPoint.setAddressOffset(16);
    averageCellVoltageDataPoint.setBlockOffset(14);
    averageCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    averageCellVoltageDataPoint.setSunSpecDataType("uint16");
    averageCellVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(averageCellVoltageDataPoint.name(), averageCellVoltageDataPoint);

    SunSpecDataPoint maxTemperatureDataPoint;
    maxTemperatureDataPoint.setName("TmpMax");
    maxTemperatureDataPoint.setLabel("Max Temperature");
    maxTemperatureDataPoint.setDescription("Maximum electrolyte temperature for all modules in the string.");
    maxTemperatureDataPoint.setUnits("C");
    maxTemperatureDataPoint.setMandatory(true);
    maxTemperatureDataPoint.setSize(1);
    maxTemperatureDataPoint.setAddressOffset(17);
    maxTemperatureDataPoint.setBlockOffset(15);
    maxTemperatureDataPoint.setScaleFactorName("Tmp_SF");
    maxTemperatureDataPoint.setSunSpecDataType("int16");
    maxTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxTemperatureDataPoint.name(), maxTemperatureDataPoint);

    SunSpecDataPoint maxTemperatureModuleDataPoint;
    maxTemperatureModuleDataPoint.setName("TmpMaxMod");
    maxTemperatureModuleDataPoint.setLabel("Max Temperature Module");
    maxTemperatureModuleDataPoint.setDescription("Module with the maximum temperature.");
    maxTemperatureModuleDataPoint.setSize(1);
    maxTemperatureModuleDataPoint.setAddressOffset(18);
    maxTemperatureModuleDataPoint.setBlockOffset(16);
    maxTemperatureModuleDataPoint.setSunSpecDataType("uint16");
    maxTemperatureModuleDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxTemperatureModuleDataPoint.name(), maxTemperatureModuleDataPoint);

    SunSpecDataPoint minTemperatureDataPoint;
    minTemperatureDataPoint.setName("TmpMin");
    minTemperatureDataPoint.setLabel("Min Temperature");
    minTemperatureDataPoint.setDescription("Minimum electrolyte temperature for all modules in the string.");
    minTemperatureDataPoint.setUnits("C");
    minTemperatureDataPoint.setMandatory(true);
    minTemperatureDataPoint.setSize(1);
    minTemperatureDataPoint.setAddressOffset(19);
    minTemperatureDataPoint.setBlockOffset(17);
    minTemperatureDataPoint.setScaleFactorName("Tmp_SF");
    minTemperatureDataPoint.setSunSpecDataType("int16");
    minTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minTemperatureDataPoint.name(), minTemperatureDataPoint);

    SunSpecDataPoint minTemperatureModuleDataPoint;
    minTemperatureModuleDataPoint.setName("TmpMinMod");
    minTemperatureModuleDataPoint.setLabel("Min Temperature Module");
    minTemperatureModuleDataPoint.setDescription("Module with the minimum temperature.");
    minTemperatureModuleDataPoint.setSize(1);
    minTemperatureModuleDataPoint.setAddressOffset(20);
    minTemperatureModuleDataPoint.setBlockOffset(18);
    minTemperatureModuleDataPoint.setSunSpecDataType("uint16");
    minTemperatureModuleDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minTemperatureModuleDataPoint.name(), minTemperatureModuleDataPoint);

    SunSpecDataPoint averageTemperatureDataPoint;
    averageTemperatureDataPoint.setName("TmpAvg");
    averageTemperatureDataPoint.setLabel("Average Temperature");
    averageTemperatureDataPoint.setDescription("Average electrolyte temperature for all modules in the string.");
    averageTemperatureDataPoint.setUnits("C");
    averageTemperatureDataPoint.setMandatory(true);
    averageTemperatureDataPoint.setSize(1);
    averageTemperatureDataPoint.setAddressOffset(21);
    averageTemperatureDataPoint.setBlockOffset(19);
    averageTemperatureDataPoint.setScaleFactorName("Tmp_SF");
    averageTemperatureDataPoint.setSunSpecDataType("int16");
    averageTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(averageTemperatureDataPoint.name(), averageTemperatureDataPoint);

    SunSpecDataPoint stringEvent1DataPoint;
    stringEvent1DataPoint.setName("Evt1");
    stringEvent1DataPoint.setLabel("String Event 1");
    stringEvent1DataPoint.setDescription("Alarms, warnings and status values.  Bit flags.");
    stringEvent1DataPoint.setMandatory(true);
    stringEvent1DataPoint.setSize(2);
    stringEvent1DataPoint.setAddressOffset(22);
    stringEvent1DataPoint.setBlockOffset(20);
    stringEvent1DataPoint.setSunSpecDataType("bitfield32");
    stringEvent1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringEvent1DataPoint.name(), stringEvent1DataPoint);

    SunSpecDataPoint stringEvent2DataPoint;
    stringEvent2DataPoint.setName("Evt2");
    stringEvent2DataPoint.setLabel("String Event 2");
    stringEvent2DataPoint.setDescription("Alarms, warnings and status values.  Bit flags.");
    stringEvent2DataPoint.setMandatory(true);
    stringEvent2DataPoint.setSize(2);
    stringEvent2DataPoint.setAddressOffset(24);
    stringEvent2DataPoint.setBlockOffset(22);
    stringEvent2DataPoint.setSunSpecDataType("bitfield32");
    stringEvent2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringEvent2DataPoint.name(), stringEvent2DataPoint);

    SunSpecDataPoint vendorEventBitfield1DataPoint;
    vendorEventBitfield1DataPoint.setName("EvtVnd1");
    vendorEventBitfield1DataPoint.setLabel("Vendor Event Bitfield 1");
    vendorEventBitfield1DataPoint.setDescription("Vendor defined events.");
    vendorEventBitfield1DataPoint.setMandatory(true);
    vendorEventBitfield1DataPoint.setSize(2);
    vendorEventBitfield1DataPoint.setAddressOffset(26);
    vendorEventBitfield1DataPoint.setBlockOffset(24);
    vendorEventBitfield1DataPoint.setSunSpecDataType("bitfield32");
    vendorEventBitfield1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vendorEventBitfield1DataPoint.name(), vendorEventBitfield1DataPoint);

    SunSpecDataPoint vendorEventBitfield2DataPoint;
    vendorEventBitfield2DataPoint.setName("EvtVnd2");
    vendorEventBitfield2DataPoint.setLabel("Vendor Event Bitfield 2");
    vendorEventBitfield2DataPoint.setDescription("Vendor defined events.");
    vendorEventBitfield2DataPoint.setMandatory(true);
    vendorEventBitfield2DataPoint.setSize(2);
    vendorEventBitfield2DataPoint.setAddressOffset(28);
    vendorEventBitfield2DataPoint.setBlockOffset(26);
    vendorEventBitfield2DataPoint.setSunSpecDataType("bitfield32");
    vendorEventBitfield2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vendorEventBitfield2DataPoint.name(), vendorEventBitfield2DataPoint);

    SunSpecDataPoint modV_SFDataPoint;
    modV_SFDataPoint.setName("ModV_SF");
    modV_SFDataPoint.setMandatory(true);
    modV_SFDataPoint.setSize(1);
    modV_SFDataPoint.setAddressOffset(30);
    modV_SFDataPoint.setBlockOffset(28);
    modV_SFDataPoint.setSunSpecDataType("sunssf");
    modV_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(modV_SFDataPoint.name(), modV_SFDataPoint);

    SunSpecDataPoint cellV_SFDataPoint;
    cellV_SFDataPoint.setName("CellV_SF");
    cellV_SFDataPoint.setDescription("Scale factor for voltage.");
    cellV_SFDataPoint.setMandatory(true);
    cellV_SFDataPoint.setSize(1);
    cellV_SFDataPoint.setAddressOffset(31);
    cellV_SFDataPoint.setBlockOffset(29);
    cellV_SFDataPoint.setSunSpecDataType("sunssf");
    cellV_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(cellV_SFDataPoint.name(), cellV_SFDataPoint);

    SunSpecDataPoint tmp_SFDataPoint;
    tmp_SFDataPoint.setName("Tmp_SF");
    tmp_SFDataPoint.setDescription("Scale factor for temperature.");
    tmp_SFDataPoint.setMandatory(true);
    tmp_SFDataPoint.setSize(1);
    tmp_SFDataPoint.setAddressOffset(32);
    tmp_SFDataPoint.setBlockOffset(30);
    tmp_SFDataPoint.setSunSpecDataType("sunssf");
    tmp_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tmp_SFDataPoint.name(), tmp_SFDataPoint);

    SunSpecDataPoint soC_SFDataPoint;
    soC_SFDataPoint.setName("SoC_SF");
    soC_SFDataPoint.setDescription("Scale factor for state of charge.");
    soC_SFDataPoint.setMandatory(true);
    soC_SFDataPoint.setSize(1);
    soC_SFDataPoint.setAddressOffset(33);
    soC_SFDataPoint.setBlockOffset(31);
    soC_SFDataPoint.setSunSpecDataType("sunssf");
    soC_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(soC_SFDataPoint.name(), soC_SFDataPoint);

    SunSpecDataPoint oCV_SFDataPoint;
    oCV_SFDataPoint.setName("OCV_SF");
    oCV_SFDataPoint.setDescription("Scale factor for open circuit voltage.");
    oCV_SFDataPoint.setMandatory(true);
    oCV_SFDataPoint.setSize(1);
    oCV_SFDataPoint.setAddressOffset(34);
    oCV_SFDataPoint.setBlockOffset(32);
    oCV_SFDataPoint.setSunSpecDataType("sunssf");
    oCV_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(oCV_SFDataPoint.name(), oCV_SFDataPoint);

    SunSpecDataPoint pad1DataPoint;
    pad1DataPoint.setName("Pad1");
    pad1DataPoint.setLabel("Pad");
    pad1DataPoint.setDescription("Pad register.");
    pad1DataPoint.setMandatory(true);
    pad1DataPoint.setSize(1);
    pad1DataPoint.setAddressOffset(35);
    pad1DataPoint.setBlockOffset(33);
    pad1DataPoint.setSunSpecDataType("pad");
    pad1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pad1DataPoint.name(), pad1DataPoint);

}

void SunSpecFlowBatteryStringModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("ModV_SF").isValid())
        m_modV_SF = m_dataPoints.value("ModV_SF").toInt16();

    if (m_dataPoints.value("CellV_SF").isValid())
        m_cellV_SF = m_dataPoints.value("CellV_SF").toInt16();

    if (m_dataPoints.value("Tmp_SF").isValid())
        m_tmp_SF = m_dataPoints.value("Tmp_SF").toInt16();

    if (m_dataPoints.value("SoC_SF").isValid())
        m_soC_SF = m_dataPoints.value("SoC_SF").toInt16();

    if (m_dataPoints.value("OCV_SF").isValid())
        m_oCV_SF = m_dataPoints.value("OCV_SF").toInt16();


    // Update properties according to the data point type
    if (m_dataPoints.value("Idx").isValid())
        m_stringIndex = m_dataPoints.value("Idx").toUInt16();

    if (m_dataPoints.value("NMod").isValid())
        m_moduleCount = m_dataPoints.value("NMod").toUInt16();

    if (m_dataPoints.value("NModCon").isValid())
        m_connectedModuleCount = m_dataPoints.value("NModCon").toUInt16();

    if (m_dataPoints.value("ModVMax").isValid())
        m_maxModuleVoltage = m_dataPoints.value("ModVMax").toFloatWithSSF(m_modV_SF);

    if (m_dataPoints.value("ModVMaxMod").isValid())
        m_maxModuleVoltageModule = m_dataPoints.value("ModVMaxMod").toUInt16();

    if (m_dataPoints.value("ModVMin").isValid())
        m_minModuleVoltage = m_dataPoints.value("ModVMin").toFloatWithSSF(m_modV_SF);

    if (m_dataPoints.value("ModVMinMod").isValid())
        m_minModuleVoltageModule = m_dataPoints.value("ModVMinMod").toUInt16();

    if (m_dataPoints.value("ModVAvg").isValid())
        m_averageModuleVoltage = m_dataPoints.value("ModVAvg").toFloatWithSSF(m_modV_SF);

    if (m_dataPoints.value("CellVMax").isValid())
        m_maxCellVoltage = m_dataPoints.value("CellVMax").toFloatWithSSF(m_cellV_SF);

    if (m_dataPoints.value("CellVMaxMod").isValid())
        m_maxCellVoltageModule = m_dataPoints.value("CellVMaxMod").toUInt16();

    if (m_dataPoints.value("CellVMaxStk").isValid())
        m_maxCellVoltageStack = m_dataPoints.value("CellVMaxStk").toUInt16();

    if (m_dataPoints.value("CellVMin").isValid())
        m_minCellVoltage = m_dataPoints.value("CellVMin").toFloatWithSSF(m_cellV_SF);

    if (m_dataPoints.value("CellVMinMod").isValid())
        m_minCellVoltageModule = m_dataPoints.value("CellVMinMod").toUInt16();

    if (m_dataPoints.value("CellVMinStk").isValid())
        m_minCellVoltageStack = m_dataPoints.value("CellVMinStk").toUInt16();

    if (m_dataPoints.value("CellVAvg").isValid())
        m_averageCellVoltage = m_dataPoints.value("CellVAvg").toFloatWithSSF(m_cellV_SF);

    if (m_dataPoints.value("TmpMax").isValid())
        m_maxTemperature = m_dataPoints.value("TmpMax").toFloatWithSSF(m_tmp_SF);

    if (m_dataPoints.value("TmpMaxMod").isValid())
        m_maxTemperatureModule = m_dataPoints.value("TmpMaxMod").toUInt16();

    if (m_dataPoints.value("TmpMin").isValid())
        m_minTemperature = m_dataPoints.value("TmpMin").toFloatWithSSF(m_tmp_SF);

    if (m_dataPoints.value("TmpMinMod").isValid())
        m_minTemperatureModule = m_dataPoints.value("TmpMinMod").toUInt16();

    if (m_dataPoints.value("TmpAvg").isValid())
        m_averageTemperature = m_dataPoints.value("TmpAvg").toFloatWithSSF(m_tmp_SF);

    if (m_dataPoints.value("Evt1").isValid())
        m_stringEvent1 = static_cast<Evt1Flags>(m_dataPoints.value("Evt1").toUInt32());

    if (m_dataPoints.value("Evt2").isValid())
        m_stringEvent2 = static_cast<Evt2Flags>(m_dataPoints.value("Evt2").toUInt32());

    if (m_dataPoints.value("EvtVnd1").isValid())
        m_vendorEventBitfield1 = m_dataPoints.value("EvtVnd1").toUInt32();

    if (m_dataPoints.value("EvtVnd2").isValid())
        m_vendorEventBitfield2 = m_dataPoints.value("EvtVnd2").toUInt32();

    if (m_dataPoints.value("ModV_SF").isValid())
        m_modV_SF = m_dataPoints.value("ModV_SF").toInt16();

    if (m_dataPoints.value("CellV_SF").isValid())
        m_cellV_SF = m_dataPoints.value("CellV_SF").toInt16();

    if (m_dataPoints.value("Tmp_SF").isValid())
        m_tmp_SF = m_dataPoints.value("Tmp_SF").toInt16();

    if (m_dataPoints.value("SoC_SF").isValid())
        m_soC_SF = m_dataPoints.value("SoC_SF").toInt16();

    if (m_dataPoints.value("OCV_SF").isValid())
        m_oCV_SF = m_dataPoints.value("OCV_SF").toInt16();

    if (m_dataPoints.value("Pad1").isValid())
        m_pad1 = m_dataPoints.value("Pad1").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecFlowBatteryStringModel *model)
{
    debug.nospace().noquote() << "SunSpecFlowBatteryStringModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("Idx") << "-->";
    if (model->dataPoints().value("Idx").isValid()) {
        debug.nospace().noquote() << model->stringIndex() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("NMod") << "-->";
    if (model->dataPoints().value("NMod").isValid()) {
        debug.nospace().noquote() << model->moduleCount() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("NModCon") << "-->";
    if (model->dataPoints().value("NModCon").isValid()) {
        debug.nospace().noquote() << model->connectedModuleCount() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModVMax") << "-->";
    if (model->dataPoints().value("ModVMax").isValid()) {
        debug.nospace().noquote() << model->maxModuleVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModVMaxMod") << "-->";
    if (model->dataPoints().value("ModVMaxMod").isValid()) {
        debug.nospace().noquote() << model->maxModuleVoltageModule() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModVMin") << "-->";
    if (model->dataPoints().value("ModVMin").isValid()) {
        debug.nospace().noquote() << model->minModuleVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModVMinMod") << "-->";
    if (model->dataPoints().value("ModVMinMod").isValid()) {
        debug.nospace().noquote() << model->minModuleVoltageModule() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModVAvg") << "-->";
    if (model->dataPoints().value("ModVAvg").isValid()) {
        debug.nospace().noquote() << model->averageModuleVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVMax") << "-->";
    if (model->dataPoints().value("CellVMax").isValid()) {
        debug.nospace().noquote() << model->maxCellVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVMaxMod") << "-->";
    if (model->dataPoints().value("CellVMaxMod").isValid()) {
        debug.nospace().noquote() << model->maxCellVoltageModule() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVMaxStk") << "-->";
    if (model->dataPoints().value("CellVMaxStk").isValid()) {
        debug.nospace().noquote() << model->maxCellVoltageStack() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVMin") << "-->";
    if (model->dataPoints().value("CellVMin").isValid()) {
        debug.nospace().noquote() << model->minCellVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVMinMod") << "-->";
    if (model->dataPoints().value("CellVMinMod").isValid()) {
        debug.nospace().noquote() << model->minCellVoltageModule() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVMinStk") << "-->";
    if (model->dataPoints().value("CellVMinStk").isValid()) {
        debug.nospace().noquote() << model->minCellVoltageStack() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVAvg") << "-->";
    if (model->dataPoints().value("CellVAvg").isValid()) {
        debug.nospace().noquote() << model->averageCellVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TmpMax") << "-->";
    if (model->dataPoints().value("TmpMax").isValid()) {
        debug.nospace().noquote() << model->maxTemperature() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TmpMaxMod") << "-->";
    if (model->dataPoints().value("TmpMaxMod").isValid()) {
        debug.nospace().noquote() << model->maxTemperatureModule() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TmpMin") << "-->";
    if (model->dataPoints().value("TmpMin").isValid()) {
        debug.nospace().noquote() << model->minTemperature() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TmpMinMod") << "-->";
    if (model->dataPoints().value("TmpMinMod").isValid()) {
        debug.nospace().noquote() << model->minTemperatureModule() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TmpAvg") << "-->";
    if (model->dataPoints().value("TmpAvg").isValid()) {
        debug.nospace().noquote() << model->averageTemperature() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Evt1") << "-->";
    if (model->dataPoints().value("Evt1").isValid()) {
        debug.nospace().noquote() << model->stringEvent1() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Evt2") << "-->";
    if (model->dataPoints().value("Evt2").isValid()) {
        debug.nospace().noquote() << model->stringEvent2() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("EvtVnd1") << "-->";
    if (model->dataPoints().value("EvtVnd1").isValid()) {
        debug.nospace().noquote() << model->vendorEventBitfield1() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("EvtVnd2") << "-->";
    if (model->dataPoints().value("EvtVnd2").isValid()) {
        debug.nospace().noquote() << model->vendorEventBitfield2() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Pad1") << "-->";
    if (model->dataPoints().value("Pad1").isValid()) {
        debug.nospace().noquote() << model->pad1() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
