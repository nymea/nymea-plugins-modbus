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

SunSpecFlowBatteryStringModel::SunSpecFlowBatteryStringModel(SunSpec *connection, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, 807, 34, modbusStartRegister, parent)
{
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
quint16 SunSpecFlowBatteryStringModel::pad() const
{
    return m_pad;
}
void SunSpecFlowBatteryStringModel::processBlockData()
{
    // Scale factors
    m_modV_SF = m_dataPoints.value("ModV_SF").toInt16();
    m_cellV_SF = m_dataPoints.value("CellV_SF").toInt16();
    m_tmp_SF = m_dataPoints.value("Tmp_SF").toInt16();
    m_soC_SF = m_dataPoints.value("SoC_SF").toInt16();
    m_oCV_SF = m_dataPoints.value("OCV_SF").toInt16();

    // Update properties according to the data point type
    m_modelId = m_dataPoints.value("ID").toUInt16();
    m_modelLength = m_dataPoints.value("L").toUInt16();
    m_stringIndex = m_dataPoints.value("Idx").toUInt16();
    m_moduleCount = m_dataPoints.value("NMod").toUInt16();
    m_connectedModuleCount = m_dataPoints.value("NModCon").toUInt16();
    m_maxModuleVoltage = m_dataPoints.value("ModVMax").toFloatWithSSF(m_modV_SF);
    m_maxModuleVoltageModule = m_dataPoints.value("ModVMaxMod").toUInt16();
    m_minModuleVoltage = m_dataPoints.value("ModVMin").toFloatWithSSF(m_modV_SF);
    m_minModuleVoltageModule = m_dataPoints.value("ModVMinMod").toUInt16();
    m_averageModuleVoltage = m_dataPoints.value("ModVAvg").toFloatWithSSF(m_modV_SF);
    m_maxCellVoltage = m_dataPoints.value("CellVMax").toFloatWithSSF(m_cellV_SF);
    m_maxCellVoltageModule = m_dataPoints.value("CellVMaxMod").toUInt16();
    m_maxCellVoltageStack = m_dataPoints.value("CellVMaxStk").toUInt16();
    m_minCellVoltage = m_dataPoints.value("CellVMin").toFloatWithSSF(m_cellV_SF);
    m_minCellVoltageModule = m_dataPoints.value("CellVMinMod").toUInt16();
    m_minCellVoltageStack = m_dataPoints.value("CellVMinStk").toUInt16();
    m_averageCellVoltage = m_dataPoints.value("CellVAvg").toFloatWithSSF(m_cellV_SF);
    m_maxTemperature = m_dataPoints.value("TmpMax").toFloatWithSSF(m_tmp_SF);
    m_maxTemperatureModule = m_dataPoints.value("TmpMaxMod").toUInt16();
    m_minTemperature = m_dataPoints.value("TmpMin").toFloatWithSSF(m_tmp_SF);
    m_minTemperatureModule = m_dataPoints.value("TmpMinMod").toUInt16();
    m_averageTemperature = m_dataPoints.value("TmpAvg").toFloatWithSSF(m_tmp_SF);
    m_stringEvent1 = static_cast<Evt1Flags>(m_dataPoints.value("Evt1").toUInt32());
    m_stringEvent2 = static_cast<Evt2Flags>(m_dataPoints.value("Evt2").toUInt32());
    m_vendorEventBitfield1 = m_dataPoints.value("EvtVnd1").toUInt32();
    m_vendorEventBitfield2 = m_dataPoints.value("EvtVnd2").toUInt32();
    m_pad = m_dataPoints.value("Pad1").toUInt16();
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
    modelIdDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(modelIdDataPoint.name(), modelIdDataPoint);

    SunSpecDataPoint modelLengthDataPoint;
    modelLengthDataPoint.setName("L");
    modelLengthDataPoint.setLabel("Model Length");
    modelLengthDataPoint.setDescription("Model length");
    modelLengthDataPoint.setMandatory(true);
    modelLengthDataPoint.setSize(1);
    modelLengthDataPoint.setAddressOffset(1);
    modelLengthDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(modelLengthDataPoint.name(), modelLengthDataPoint);

    SunSpecDataPoint stringIndexDataPoint;
    stringIndexDataPoint.setName("Idx");
    stringIndexDataPoint.setLabel("String Index");
    stringIndexDataPoint.setDescription("Index of the string within the bank.");
    stringIndexDataPoint.setMandatory(true);
    stringIndexDataPoint.setSize(1);
    stringIndexDataPoint.setAddressOffset(2);
    stringIndexDataPoint.setBlockOffset(0);
    stringIndexDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(stringIndexDataPoint.name(), stringIndexDataPoint);

    SunSpecDataPoint moduleCountDataPoint;
    moduleCountDataPoint.setName("NMod");
    moduleCountDataPoint.setLabel("Module Count");
    moduleCountDataPoint.setDescription("Number of modules in this string.");
    moduleCountDataPoint.setMandatory(true);
    moduleCountDataPoint.setSize(1);
    moduleCountDataPoint.setAddressOffset(3);
    moduleCountDataPoint.setBlockOffset(1);
    moduleCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(moduleCountDataPoint.name(), moduleCountDataPoint);

    SunSpecDataPoint connectedModuleCountDataPoint;
    connectedModuleCountDataPoint.setName("NModCon");
    connectedModuleCountDataPoint.setLabel("Connected Module Count");
    connectedModuleCountDataPoint.setDescription("Number of electrically connected modules in this string.");
    connectedModuleCountDataPoint.setMandatory(true);
    connectedModuleCountDataPoint.setSize(1);
    connectedModuleCountDataPoint.setAddressOffset(4);
    connectedModuleCountDataPoint.setBlockOffset(2);
    connectedModuleCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    maxModuleVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(maxModuleVoltageDataPoint.name(), maxModuleVoltageDataPoint);

    SunSpecDataPoint maxModuleVoltageModuleDataPoint;
    maxModuleVoltageModuleDataPoint.setName("ModVMaxMod");
    maxModuleVoltageModuleDataPoint.setLabel("Max Module Voltage Module");
    maxModuleVoltageModuleDataPoint.setDescription("Module with the maximum voltage.");
    maxModuleVoltageModuleDataPoint.setSize(1);
    maxModuleVoltageModuleDataPoint.setAddressOffset(6);
    maxModuleVoltageModuleDataPoint.setBlockOffset(4);
    maxModuleVoltageModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    minModuleVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(minModuleVoltageDataPoint.name(), minModuleVoltageDataPoint);

    SunSpecDataPoint minModuleVoltageModuleDataPoint;
    minModuleVoltageModuleDataPoint.setName("ModVMinMod");
    minModuleVoltageModuleDataPoint.setLabel("Min Module Voltage Module");
    minModuleVoltageModuleDataPoint.setDescription("Module with the minimum voltage.");
    minModuleVoltageModuleDataPoint.setSize(1);
    minModuleVoltageModuleDataPoint.setAddressOffset(8);
    minModuleVoltageModuleDataPoint.setBlockOffset(6);
    minModuleVoltageModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    averageModuleVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    maxCellVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(maxCellVoltageDataPoint.name(), maxCellVoltageDataPoint);

    SunSpecDataPoint maxCellVoltageModuleDataPoint;
    maxCellVoltageModuleDataPoint.setName("CellVMaxMod");
    maxCellVoltageModuleDataPoint.setLabel("Max Cell Voltage Module");
    maxCellVoltageModuleDataPoint.setDescription("Module containing the cell with the maximum voltage.");
    maxCellVoltageModuleDataPoint.setSize(1);
    maxCellVoltageModuleDataPoint.setAddressOffset(11);
    maxCellVoltageModuleDataPoint.setBlockOffset(9);
    maxCellVoltageModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(maxCellVoltageModuleDataPoint.name(), maxCellVoltageModuleDataPoint);

    SunSpecDataPoint maxCellVoltageStackDataPoint;
    maxCellVoltageStackDataPoint.setName("CellVMaxStk");
    maxCellVoltageStackDataPoint.setLabel("Max Cell Voltage Stack");
    maxCellVoltageStackDataPoint.setDescription("Stack containing the cell with the maximum voltage.");
    maxCellVoltageStackDataPoint.setSize(1);
    maxCellVoltageStackDataPoint.setAddressOffset(12);
    maxCellVoltageStackDataPoint.setBlockOffset(10);
    maxCellVoltageStackDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    minCellVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(minCellVoltageDataPoint.name(), minCellVoltageDataPoint);

    SunSpecDataPoint minCellVoltageModuleDataPoint;
    minCellVoltageModuleDataPoint.setName("CellVMinMod");
    minCellVoltageModuleDataPoint.setLabel("Min Cell Voltage Module");
    minCellVoltageModuleDataPoint.setDescription("Module containing the cell with the minimum voltage.");
    minCellVoltageModuleDataPoint.setSize(1);
    minCellVoltageModuleDataPoint.setAddressOffset(14);
    minCellVoltageModuleDataPoint.setBlockOffset(12);
    minCellVoltageModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(minCellVoltageModuleDataPoint.name(), minCellVoltageModuleDataPoint);

    SunSpecDataPoint minCellVoltageStackDataPoint;
    minCellVoltageStackDataPoint.setName("CellVMinStk");
    minCellVoltageStackDataPoint.setLabel("Min Cell Voltage Stack");
    minCellVoltageStackDataPoint.setDescription("Stack containing the cell with the minimum voltage.");
    minCellVoltageStackDataPoint.setSize(1);
    minCellVoltageStackDataPoint.setAddressOffset(15);
    minCellVoltageStackDataPoint.setBlockOffset(13);
    minCellVoltageStackDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    averageCellVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    maxTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints.insert(maxTemperatureDataPoint.name(), maxTemperatureDataPoint);

    SunSpecDataPoint maxTemperatureModuleDataPoint;
    maxTemperatureModuleDataPoint.setName("TmpMaxMod");
    maxTemperatureModuleDataPoint.setLabel("Max Temperature Module");
    maxTemperatureModuleDataPoint.setDescription("Module with the maximum temperature.");
    maxTemperatureModuleDataPoint.setSize(1);
    maxTemperatureModuleDataPoint.setAddressOffset(18);
    maxTemperatureModuleDataPoint.setBlockOffset(16);
    maxTemperatureModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    minTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints.insert(minTemperatureDataPoint.name(), minTemperatureDataPoint);

    SunSpecDataPoint minTemperatureModuleDataPoint;
    minTemperatureModuleDataPoint.setName("TmpMinMod");
    minTemperatureModuleDataPoint.setLabel("Min Temperature Module");
    minTemperatureModuleDataPoint.setDescription("Module with the minimum temperature.");
    minTemperatureModuleDataPoint.setSize(1);
    minTemperatureModuleDataPoint.setAddressOffset(20);
    minTemperatureModuleDataPoint.setBlockOffset(18);
    minTemperatureModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    averageTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints.insert(averageTemperatureDataPoint.name(), averageTemperatureDataPoint);

    SunSpecDataPoint stringEvent1DataPoint;
    stringEvent1DataPoint.setName("Evt1");
    stringEvent1DataPoint.setLabel("String Event 1");
    stringEvent1DataPoint.setDescription("Alarms, warnings and status values.  Bit flags.");
    stringEvent1DataPoint.setMandatory(true);
    stringEvent1DataPoint.setSize(2);
    stringEvent1DataPoint.setAddressOffset(22);
    stringEvent1DataPoint.setBlockOffset(20);
    stringEvent1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(stringEvent1DataPoint.name(), stringEvent1DataPoint);

    SunSpecDataPoint stringEvent2DataPoint;
    stringEvent2DataPoint.setName("Evt2");
    stringEvent2DataPoint.setLabel("String Event 2");
    stringEvent2DataPoint.setDescription("Alarms, warnings and status values.  Bit flags.");
    stringEvent2DataPoint.setMandatory(true);
    stringEvent2DataPoint.setSize(2);
    stringEvent2DataPoint.setAddressOffset(24);
    stringEvent2DataPoint.setBlockOffset(22);
    stringEvent2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(stringEvent2DataPoint.name(), stringEvent2DataPoint);

    SunSpecDataPoint vendorEventBitfield1DataPoint;
    vendorEventBitfield1DataPoint.setName("EvtVnd1");
    vendorEventBitfield1DataPoint.setLabel("Vendor Event Bitfield 1");
    vendorEventBitfield1DataPoint.setDescription("Vendor defined events.");
    vendorEventBitfield1DataPoint.setMandatory(true);
    vendorEventBitfield1DataPoint.setSize(2);
    vendorEventBitfield1DataPoint.setAddressOffset(26);
    vendorEventBitfield1DataPoint.setBlockOffset(24);
    vendorEventBitfield1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(vendorEventBitfield1DataPoint.name(), vendorEventBitfield1DataPoint);

    SunSpecDataPoint vendorEventBitfield2DataPoint;
    vendorEventBitfield2DataPoint.setName("EvtVnd2");
    vendorEventBitfield2DataPoint.setLabel("Vendor Event Bitfield 2");
    vendorEventBitfield2DataPoint.setDescription("Vendor defined events.");
    vendorEventBitfield2DataPoint.setMandatory(true);
    vendorEventBitfield2DataPoint.setSize(2);
    vendorEventBitfield2DataPoint.setAddressOffset(28);
    vendorEventBitfield2DataPoint.setBlockOffset(26);
    vendorEventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(vendorEventBitfield2DataPoint.name(), vendorEventBitfield2DataPoint);

    SunSpecDataPoint modV_SFDataPoint;
    modV_SFDataPoint.setName("ModV_SF");
    modV_SFDataPoint.setMandatory(true);
    modV_SFDataPoint.setSize(1);
    modV_SFDataPoint.setAddressOffset(30);
    modV_SFDataPoint.setBlockOffset(28);
    modV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(modV_SFDataPoint.name(), modV_SFDataPoint);

    SunSpecDataPoint cellV_SFDataPoint;
    cellV_SFDataPoint.setName("CellV_SF");
    cellV_SFDataPoint.setDescription("Scale factor for voltage.");
    cellV_SFDataPoint.setMandatory(true);
    cellV_SFDataPoint.setSize(1);
    cellV_SFDataPoint.setAddressOffset(31);
    cellV_SFDataPoint.setBlockOffset(29);
    cellV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(cellV_SFDataPoint.name(), cellV_SFDataPoint);

    SunSpecDataPoint tmp_SFDataPoint;
    tmp_SFDataPoint.setName("Tmp_SF");
    tmp_SFDataPoint.setDescription("Scale factor for temperature.");
    tmp_SFDataPoint.setMandatory(true);
    tmp_SFDataPoint.setSize(1);
    tmp_SFDataPoint.setAddressOffset(32);
    tmp_SFDataPoint.setBlockOffset(30);
    tmp_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(tmp_SFDataPoint.name(), tmp_SFDataPoint);

    SunSpecDataPoint soC_SFDataPoint;
    soC_SFDataPoint.setName("SoC_SF");
    soC_SFDataPoint.setDescription("Scale factor for state of charge.");
    soC_SFDataPoint.setMandatory(true);
    soC_SFDataPoint.setSize(1);
    soC_SFDataPoint.setAddressOffset(33);
    soC_SFDataPoint.setBlockOffset(31);
    soC_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(soC_SFDataPoint.name(), soC_SFDataPoint);

    SunSpecDataPoint oCV_SFDataPoint;
    oCV_SFDataPoint.setName("OCV_SF");
    oCV_SFDataPoint.setDescription("Scale factor for open circuit voltage.");
    oCV_SFDataPoint.setMandatory(true);
    oCV_SFDataPoint.setSize(1);
    oCV_SFDataPoint.setAddressOffset(34);
    oCV_SFDataPoint.setBlockOffset(32);
    oCV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(oCV_SFDataPoint.name(), oCV_SFDataPoint);

    SunSpecDataPoint padDataPoint;
    padDataPoint.setName("Pad1");
    padDataPoint.setLabel("Pad");
    padDataPoint.setDescription("Pad register.");
    padDataPoint.setMandatory(true);
    padDataPoint.setSize(1);
    padDataPoint.setAddressOffset(35);
    padDataPoint.setBlockOffset(33);
    padDataPoint.setDataType(SunSpecDataPoint::stringToDataType("pad"));
    m_dataPoints.insert(padDataPoint.name(), padDataPoint);

}

