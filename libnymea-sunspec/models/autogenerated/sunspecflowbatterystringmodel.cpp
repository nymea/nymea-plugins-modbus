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

SunSpecFlowBatteryStringModel::SunSpecFlowBatteryStringModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
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

void SunSpecFlowBatteryStringModel::readModelHeader()
{

}

void SunSpecFlowBatteryStringModel::readBlockData()
{

}

void SunSpecFlowBatteryStringModel::initDataPoints()
{
    SunSpecDataPoint modelIdDataPoint;
    modelIdDataPoint.setName("ID");
    modelIdDataPoint.setLabel("Model ID");
    modelIdDataPoint.setDescription("Model identifier");
    modelIdDataPoint.setMandatory(true);
    modelIdDataPoint.setSize(1);
    modelIdDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << modelIdDataPoint;

    SunSpecDataPoint modelLengthDataPoint;
    modelLengthDataPoint.setName("L");
    modelLengthDataPoint.setLabel("Model Length");
    modelLengthDataPoint.setDescription("Model length");
    modelLengthDataPoint.setMandatory(true);
    modelLengthDataPoint.setSize(1);
    modelLengthDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << modelLengthDataPoint;

    SunSpecDataPoint stringIndexDataPoint;
    stringIndexDataPoint.setName("Idx");
    stringIndexDataPoint.setLabel("String Index");
    stringIndexDataPoint.setDescription("Index of the string within the bank.");
    stringIndexDataPoint.setMandatory(true);
    stringIndexDataPoint.setSize(1);
    stringIndexDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << stringIndexDataPoint;

    SunSpecDataPoint moduleCountDataPoint;
    moduleCountDataPoint.setName("NMod");
    moduleCountDataPoint.setLabel("Module Count");
    moduleCountDataPoint.setDescription("Number of modules in this string.");
    moduleCountDataPoint.setMandatory(true);
    moduleCountDataPoint.setSize(1);
    moduleCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << moduleCountDataPoint;

    SunSpecDataPoint connectedModuleCountDataPoint;
    connectedModuleCountDataPoint.setName("NModCon");
    connectedModuleCountDataPoint.setLabel("Connected Module Count");
    connectedModuleCountDataPoint.setDescription("Number of electrically connected modules in this string.");
    connectedModuleCountDataPoint.setMandatory(true);
    connectedModuleCountDataPoint.setSize(1);
    connectedModuleCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << connectedModuleCountDataPoint;

    SunSpecDataPoint maxModuleVoltageDataPoint;
    maxModuleVoltageDataPoint.setName("ModVMax");
    maxModuleVoltageDataPoint.setLabel("Max Module Voltage");
    maxModuleVoltageDataPoint.setDescription("Maximum voltage for all modules in the string.");
    maxModuleVoltageDataPoint.setUnits("V");
    maxModuleVoltageDataPoint.setMandatory(true);
    maxModuleVoltageDataPoint.setSize(1);
    maxModuleVoltageDataPoint.setScaleFactorName("ModV_SF");
    maxModuleVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << maxModuleVoltageDataPoint;

    SunSpecDataPoint maxModuleVoltageModuleDataPoint;
    maxModuleVoltageModuleDataPoint.setName("ModVMaxMod");
    maxModuleVoltageModuleDataPoint.setLabel("Max Module Voltage Module");
    maxModuleVoltageModuleDataPoint.setDescription("Module with the maximum voltage.");
    maxModuleVoltageModuleDataPoint.setSize(1);
    maxModuleVoltageModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << maxModuleVoltageModuleDataPoint;

    SunSpecDataPoint minModuleVoltageDataPoint;
    minModuleVoltageDataPoint.setName("ModVMin");
    minModuleVoltageDataPoint.setLabel("Min Module Voltage");
    minModuleVoltageDataPoint.setDescription("Minimum voltage for all modules in the string.");
    minModuleVoltageDataPoint.setUnits("V");
    minModuleVoltageDataPoint.setMandatory(true);
    minModuleVoltageDataPoint.setSize(1);
    minModuleVoltageDataPoint.setScaleFactorName("ModV_SF");
    minModuleVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << minModuleVoltageDataPoint;

    SunSpecDataPoint minModuleVoltageModuleDataPoint;
    minModuleVoltageModuleDataPoint.setName("ModVMinMod");
    minModuleVoltageModuleDataPoint.setLabel("Min Module Voltage Module");
    minModuleVoltageModuleDataPoint.setDescription("Module with the minimum voltage.");
    minModuleVoltageModuleDataPoint.setSize(1);
    minModuleVoltageModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << minModuleVoltageModuleDataPoint;

    SunSpecDataPoint averageModuleVoltageDataPoint;
    averageModuleVoltageDataPoint.setName("ModVAvg");
    averageModuleVoltageDataPoint.setLabel("Average Module Voltage");
    averageModuleVoltageDataPoint.setDescription("Average voltage for all modules in the string.");
    averageModuleVoltageDataPoint.setUnits("V");
    averageModuleVoltageDataPoint.setMandatory(true);
    averageModuleVoltageDataPoint.setSize(1);
    averageModuleVoltageDataPoint.setScaleFactorName("ModV_SF");
    averageModuleVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << averageModuleVoltageDataPoint;

    SunSpecDataPoint maxCellVoltageDataPoint;
    maxCellVoltageDataPoint.setName("CellVMax");
    maxCellVoltageDataPoint.setLabel("Max Cell Voltage");
    maxCellVoltageDataPoint.setDescription("Maximum voltage for all cells in the string.");
    maxCellVoltageDataPoint.setUnits("V");
    maxCellVoltageDataPoint.setSize(1);
    maxCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    maxCellVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << maxCellVoltageDataPoint;

    SunSpecDataPoint maxCellVoltageModuleDataPoint;
    maxCellVoltageModuleDataPoint.setName("CellVMaxMod");
    maxCellVoltageModuleDataPoint.setLabel("Max Cell Voltage Module");
    maxCellVoltageModuleDataPoint.setDescription("Module containing the cell with the maximum voltage.");
    maxCellVoltageModuleDataPoint.setSize(1);
    maxCellVoltageModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << maxCellVoltageModuleDataPoint;

    SunSpecDataPoint maxCellVoltageStackDataPoint;
    maxCellVoltageStackDataPoint.setName("CellVMaxStk");
    maxCellVoltageStackDataPoint.setLabel("Max Cell Voltage Stack");
    maxCellVoltageStackDataPoint.setDescription("Stack containing the cell with the maximum voltage.");
    maxCellVoltageStackDataPoint.setSize(1);
    maxCellVoltageStackDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << maxCellVoltageStackDataPoint;

    SunSpecDataPoint minCellVoltageDataPoint;
    minCellVoltageDataPoint.setName("CellVMin");
    minCellVoltageDataPoint.setLabel("Min Cell Voltage");
    minCellVoltageDataPoint.setDescription("Minimum voltage for all cells in the string.");
    minCellVoltageDataPoint.setUnits("V");
    minCellVoltageDataPoint.setSize(1);
    minCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    minCellVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << minCellVoltageDataPoint;

    SunSpecDataPoint minCellVoltageModuleDataPoint;
    minCellVoltageModuleDataPoint.setName("CellVMinMod");
    minCellVoltageModuleDataPoint.setLabel("Min Cell Voltage Module");
    minCellVoltageModuleDataPoint.setDescription("Module containing the cell with the minimum voltage.");
    minCellVoltageModuleDataPoint.setSize(1);
    minCellVoltageModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << minCellVoltageModuleDataPoint;

    SunSpecDataPoint minCellVoltageStackDataPoint;
    minCellVoltageStackDataPoint.setName("CellVMinStk");
    minCellVoltageStackDataPoint.setLabel("Min Cell Voltage Stack");
    minCellVoltageStackDataPoint.setDescription("Stack containing the cell with the minimum voltage.");
    minCellVoltageStackDataPoint.setSize(1);
    minCellVoltageStackDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << minCellVoltageStackDataPoint;

    SunSpecDataPoint averageCellVoltageDataPoint;
    averageCellVoltageDataPoint.setName("CellVAvg");
    averageCellVoltageDataPoint.setLabel("Average Cell Voltage");
    averageCellVoltageDataPoint.setDescription("Average voltage for all cells in the string.");
    averageCellVoltageDataPoint.setUnits("V");
    averageCellVoltageDataPoint.setSize(1);
    averageCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    averageCellVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << averageCellVoltageDataPoint;

    SunSpecDataPoint maxTemperatureDataPoint;
    maxTemperatureDataPoint.setName("TmpMax");
    maxTemperatureDataPoint.setLabel("Max Temperature");
    maxTemperatureDataPoint.setDescription("Maximum electrolyte temperature for all modules in the string.");
    maxTemperatureDataPoint.setUnits("C");
    maxTemperatureDataPoint.setMandatory(true);
    maxTemperatureDataPoint.setSize(1);
    maxTemperatureDataPoint.setScaleFactorName("Tmp_SF");
    maxTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << maxTemperatureDataPoint;

    SunSpecDataPoint maxTemperatureModuleDataPoint;
    maxTemperatureModuleDataPoint.setName("TmpMaxMod");
    maxTemperatureModuleDataPoint.setLabel("Max Temperature Module");
    maxTemperatureModuleDataPoint.setDescription("Module with the maximum temperature.");
    maxTemperatureModuleDataPoint.setSize(1);
    maxTemperatureModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << maxTemperatureModuleDataPoint;

    SunSpecDataPoint minTemperatureDataPoint;
    minTemperatureDataPoint.setName("TmpMin");
    minTemperatureDataPoint.setLabel("Min Temperature");
    minTemperatureDataPoint.setDescription("Minimum electrolyte temperature for all modules in the string.");
    minTemperatureDataPoint.setUnits("C");
    minTemperatureDataPoint.setMandatory(true);
    minTemperatureDataPoint.setSize(1);
    minTemperatureDataPoint.setScaleFactorName("Tmp_SF");
    minTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << minTemperatureDataPoint;

    SunSpecDataPoint minTemperatureModuleDataPoint;
    minTemperatureModuleDataPoint.setName("TmpMinMod");
    minTemperatureModuleDataPoint.setLabel("Min Temperature Module");
    minTemperatureModuleDataPoint.setDescription("Module with the minimum temperature.");
    minTemperatureModuleDataPoint.setSize(1);
    minTemperatureModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << minTemperatureModuleDataPoint;

    SunSpecDataPoint averageTemperatureDataPoint;
    averageTemperatureDataPoint.setName("TmpAvg");
    averageTemperatureDataPoint.setLabel("Average Temperature");
    averageTemperatureDataPoint.setDescription("Average electrolyte temperature for all modules in the string.");
    averageTemperatureDataPoint.setUnits("C");
    averageTemperatureDataPoint.setMandatory(true);
    averageTemperatureDataPoint.setSize(1);
    averageTemperatureDataPoint.setScaleFactorName("Tmp_SF");
    averageTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << averageTemperatureDataPoint;

    SunSpecDataPoint stringEvent1DataPoint;
    stringEvent1DataPoint.setName("Evt1");
    stringEvent1DataPoint.setLabel("String Event 1");
    stringEvent1DataPoint.setDescription("Alarms, warnings and status values.  Bit flags.");
    stringEvent1DataPoint.setMandatory(true);
    stringEvent1DataPoint.setSize(2);
    stringEvent1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints << stringEvent1DataPoint;

    SunSpecDataPoint stringEvent2DataPoint;
    stringEvent2DataPoint.setName("Evt2");
    stringEvent2DataPoint.setLabel("String Event 2");
    stringEvent2DataPoint.setDescription("Alarms, warnings and status values.  Bit flags.");
    stringEvent2DataPoint.setMandatory(true);
    stringEvent2DataPoint.setSize(2);
    stringEvent2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints << stringEvent2DataPoint;

    SunSpecDataPoint vendorEventBitfield1DataPoint;
    vendorEventBitfield1DataPoint.setName("EvtVnd1");
    vendorEventBitfield1DataPoint.setLabel("Vendor Event Bitfield 1");
    vendorEventBitfield1DataPoint.setDescription("Vendor defined events.");
    vendorEventBitfield1DataPoint.setMandatory(true);
    vendorEventBitfield1DataPoint.setSize(2);
    vendorEventBitfield1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints << vendorEventBitfield1DataPoint;

    SunSpecDataPoint vendorEventBitfield2DataPoint;
    vendorEventBitfield2DataPoint.setName("EvtVnd2");
    vendorEventBitfield2DataPoint.setLabel("Vendor Event Bitfield 2");
    vendorEventBitfield2DataPoint.setDescription("Vendor defined events.");
    vendorEventBitfield2DataPoint.setMandatory(true);
    vendorEventBitfield2DataPoint.setSize(2);
    vendorEventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints << vendorEventBitfield2DataPoint;

    SunSpecDataPoint ModV_SFDataPoint;
    ModV_SFDataPoint.setName("ModV_SF");
    ModV_SFDataPoint.setMandatory(true);
    ModV_SFDataPoint.setSize(1);
    ModV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << ModV_SFDataPoint;

    SunSpecDataPoint CellV_SFDataPoint;
    CellV_SFDataPoint.setName("CellV_SF");
    CellV_SFDataPoint.setDescription("Scale factor for voltage.");
    CellV_SFDataPoint.setMandatory(true);
    CellV_SFDataPoint.setSize(1);
    CellV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << CellV_SFDataPoint;

    SunSpecDataPoint Tmp_SFDataPoint;
    Tmp_SFDataPoint.setName("Tmp_SF");
    Tmp_SFDataPoint.setDescription("Scale factor for temperature.");
    Tmp_SFDataPoint.setMandatory(true);
    Tmp_SFDataPoint.setSize(1);
    Tmp_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << Tmp_SFDataPoint;

    SunSpecDataPoint SoC_SFDataPoint;
    SoC_SFDataPoint.setName("SoC_SF");
    SoC_SFDataPoint.setDescription("Scale factor for state of charge.");
    SoC_SFDataPoint.setMandatory(true);
    SoC_SFDataPoint.setSize(1);
    SoC_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << SoC_SFDataPoint;

    SunSpecDataPoint OCV_SFDataPoint;
    OCV_SFDataPoint.setName("OCV_SF");
    OCV_SFDataPoint.setDescription("Scale factor for open circuit voltage.");
    OCV_SFDataPoint.setMandatory(true);
    OCV_SFDataPoint.setSize(1);
    OCV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << OCV_SFDataPoint;

    SunSpecDataPoint padDataPoint;
    padDataPoint.setName("Pad1");
    padDataPoint.setLabel("Pad");
    padDataPoint.setDescription("Pad register.");
    padDataPoint.setMandatory(true);
    padDataPoint.setSize(1);
    padDataPoint.setDataType(SunSpecDataPoint::stringToDataType("pad"));
    m_dataPoints << padDataPoint;

}

