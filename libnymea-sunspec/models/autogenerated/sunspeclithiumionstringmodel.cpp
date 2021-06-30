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

#include "sunspeclithiumionstringmodel.h"

SunSpecLithiumIonStringModel::SunSpecLithiumIonStringModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
}

SunSpecLithiumIonStringModel::~SunSpecLithiumIonStringModel()
{

}

QString SunSpecLithiumIonStringModel::name() const
{
    return "lithium_ion_string";
}

QString SunSpecLithiumIonStringModel::description() const
{
    return QString();
}

QString SunSpecLithiumIonStringModel::label() const
{
    return "Lithium-Ion String Model";
}

void SunSpecLithiumIonStringModel::readModelHeader()
{

}

void SunSpecLithiumIonStringModel::readBlockData()
{

}

void SunSpecLithiumIonStringModel::initDataPoints()
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
    moduleCountDataPoint.setDescription("Count of modules in the string.");
    moduleCountDataPoint.setMandatory(true);
    moduleCountDataPoint.setSize(1);
    moduleCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << moduleCountDataPoint;

    SunSpecDataPoint stringStatusDataPoint;
    stringStatusDataPoint.setName("St");
    stringStatusDataPoint.setLabel("String Status");
    stringStatusDataPoint.setDescription("Current status of the string.");
    stringStatusDataPoint.setMandatory(true);
    stringStatusDataPoint.setSize(2);
    stringStatusDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints << stringStatusDataPoint;

    SunSpecDataPoint connectionFailureReasonDataPoint;
    connectionFailureReasonDataPoint.setName("ConFail");
    connectionFailureReasonDataPoint.setLabel("Connection Failure Reason");
    connectionFailureReasonDataPoint.setSize(1);
    connectionFailureReasonDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    m_dataPoints << connectionFailureReasonDataPoint;

    SunSpecDataPoint stringCellBalancingCountDataPoint;
    stringCellBalancingCountDataPoint.setName("NCellBal");
    stringCellBalancingCountDataPoint.setLabel("String Cell Balancing Count");
    stringCellBalancingCountDataPoint.setDescription("Number of cells currently being balanced in the string.");
    stringCellBalancingCountDataPoint.setSize(1);
    stringCellBalancingCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << stringCellBalancingCountDataPoint;

    SunSpecDataPoint stringStateOfChargeDataPoint;
    stringStateOfChargeDataPoint.setName("SoC");
    stringStateOfChargeDataPoint.setLabel("String State of Charge");
    stringStateOfChargeDataPoint.setDescription("Battery string state of charge, expressed as a percentage.");
    stringStateOfChargeDataPoint.setUnits("%");
    stringStateOfChargeDataPoint.setMandatory(true);
    stringStateOfChargeDataPoint.setSize(1);
    stringStateOfChargeDataPoint.setScaleFactorName("SoC_SF");
    stringStateOfChargeDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << stringStateOfChargeDataPoint;

    SunSpecDataPoint stringDepthOfDischargeDataPoint;
    stringDepthOfDischargeDataPoint.setName("DoD");
    stringDepthOfDischargeDataPoint.setLabel("String Depth of Discharge");
    stringDepthOfDischargeDataPoint.setDescription("Depth of discharge for the string, expressed as a percentage.");
    stringDepthOfDischargeDataPoint.setUnits("%");
    stringDepthOfDischargeDataPoint.setSize(1);
    stringDepthOfDischargeDataPoint.setScaleFactorName("DoD_SF");
    stringDepthOfDischargeDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << stringDepthOfDischargeDataPoint;

    SunSpecDataPoint stringCycleCountDataPoint;
    stringCycleCountDataPoint.setName("NCyc");
    stringCycleCountDataPoint.setLabel("String Cycle Count");
    stringCycleCountDataPoint.setDescription("Number of discharge cycles executed upon the string.");
    stringCycleCountDataPoint.setSize(2);
    stringCycleCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint32"));
    m_dataPoints << stringCycleCountDataPoint;

    SunSpecDataPoint stringStateOfHealthDataPoint;
    stringStateOfHealthDataPoint.setName("SoH");
    stringStateOfHealthDataPoint.setLabel("String State of Health");
    stringStateOfHealthDataPoint.setDescription("Battery string state of health, expressed as a percentage.");
    stringStateOfHealthDataPoint.setUnits("%");
    stringStateOfHealthDataPoint.setSize(1);
    stringStateOfHealthDataPoint.setScaleFactorName("SoH_SF");
    stringStateOfHealthDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << stringStateOfHealthDataPoint;

    SunSpecDataPoint stringCurrentDataPoint;
    stringCurrentDataPoint.setName("A");
    stringCurrentDataPoint.setLabel("String Current");
    stringCurrentDataPoint.setDescription("String current measurement.");
    stringCurrentDataPoint.setUnits("A");
    stringCurrentDataPoint.setMandatory(true);
    stringCurrentDataPoint.setSize(1);
    stringCurrentDataPoint.setScaleFactorName("A_SF");
    stringCurrentDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << stringCurrentDataPoint;

    SunSpecDataPoint stringVoltageDataPoint;
    stringVoltageDataPoint.setName("V");
    stringVoltageDataPoint.setLabel("String Voltage");
    stringVoltageDataPoint.setDescription("String voltage measurement.");
    stringVoltageDataPoint.setUnits("V");
    stringVoltageDataPoint.setSize(1);
    stringVoltageDataPoint.setScaleFactorName("V_SF");
    stringVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << stringVoltageDataPoint;

    SunSpecDataPoint maxCellVoltageDataPoint;
    maxCellVoltageDataPoint.setName("CellVMax");
    maxCellVoltageDataPoint.setLabel("Max Cell Voltage");
    maxCellVoltageDataPoint.setDescription("Maximum voltage for all cells in the string.");
    maxCellVoltageDataPoint.setUnits("V");
    maxCellVoltageDataPoint.setMandatory(true);
    maxCellVoltageDataPoint.setSize(1);
    maxCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    maxCellVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << maxCellVoltageDataPoint;

    SunSpecDataPoint maxCellVoltageModuleDataPoint;
    maxCellVoltageModuleDataPoint.setName("CellVMaxMod");
    maxCellVoltageModuleDataPoint.setLabel("Max Cell Voltage Module");
    maxCellVoltageModuleDataPoint.setDescription("Module containing the cell with maximum cell voltage.");
    maxCellVoltageModuleDataPoint.setSize(1);
    maxCellVoltageModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << maxCellVoltageModuleDataPoint;

    SunSpecDataPoint minCellVoltageDataPoint;
    minCellVoltageDataPoint.setName("CellVMin");
    minCellVoltageDataPoint.setLabel("Min Cell Voltage");
    minCellVoltageDataPoint.setDescription("Minimum voltage for all cells in the string.");
    minCellVoltageDataPoint.setUnits("V");
    minCellVoltageDataPoint.setMandatory(true);
    minCellVoltageDataPoint.setSize(1);
    minCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    minCellVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << minCellVoltageDataPoint;

    SunSpecDataPoint minCellVoltageModuleDataPoint;
    minCellVoltageModuleDataPoint.setName("CellVMinMod");
    minCellVoltageModuleDataPoint.setLabel("Min Cell Voltage Module");
    minCellVoltageModuleDataPoint.setDescription("Module containing the cell with minimum cell voltage.");
    minCellVoltageModuleDataPoint.setSize(1);
    minCellVoltageModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << minCellVoltageModuleDataPoint;

    SunSpecDataPoint averageCellVoltageDataPoint;
    averageCellVoltageDataPoint.setName("CellVAvg");
    averageCellVoltageDataPoint.setLabel("Average Cell Voltage");
    averageCellVoltageDataPoint.setDescription("Average voltage for all cells in the string.");
    averageCellVoltageDataPoint.setUnits("V");
    averageCellVoltageDataPoint.setMandatory(true);
    averageCellVoltageDataPoint.setSize(1);
    averageCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    averageCellVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << averageCellVoltageDataPoint;

    SunSpecDataPoint maxModuleTemperatureDataPoint;
    maxModuleTemperatureDataPoint.setName("ModTmpMax");
    maxModuleTemperatureDataPoint.setLabel("Max Module Temperature");
    maxModuleTemperatureDataPoint.setDescription("Maximum temperature for all modules in the string.");
    maxModuleTemperatureDataPoint.setUnits("C");
    maxModuleTemperatureDataPoint.setMandatory(true);
    maxModuleTemperatureDataPoint.setSize(1);
    maxModuleTemperatureDataPoint.setScaleFactorName("ModTmp_SF");
    maxModuleTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << maxModuleTemperatureDataPoint;

    SunSpecDataPoint maxModuleTemperatureModuleDataPoint;
    maxModuleTemperatureModuleDataPoint.setName("ModTmpMaxMod");
    maxModuleTemperatureModuleDataPoint.setLabel("Max Module Temperature Module");
    maxModuleTemperatureModuleDataPoint.setDescription("Module with the maximum temperature.");
    maxModuleTemperatureModuleDataPoint.setMandatory(true);
    maxModuleTemperatureModuleDataPoint.setSize(1);
    maxModuleTemperatureModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << maxModuleTemperatureModuleDataPoint;

    SunSpecDataPoint minModuleTemperatureDataPoint;
    minModuleTemperatureDataPoint.setName("ModTmpMin");
    minModuleTemperatureDataPoint.setLabel("Min Module Temperature");
    minModuleTemperatureDataPoint.setDescription("Minimum temperature for all modules in the string.");
    minModuleTemperatureDataPoint.setUnits("C");
    minModuleTemperatureDataPoint.setMandatory(true);
    minModuleTemperatureDataPoint.setSize(1);
    minModuleTemperatureDataPoint.setScaleFactorName("ModTmp_SF");
    minModuleTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << minModuleTemperatureDataPoint;

    SunSpecDataPoint minModuleTemperatureModuleDataPoint;
    minModuleTemperatureModuleDataPoint.setName("ModTmpMinMod");
    minModuleTemperatureModuleDataPoint.setLabel("Min Module Temperature Module");
    minModuleTemperatureModuleDataPoint.setDescription("Module with the minimum temperature.");
    minModuleTemperatureModuleDataPoint.setMandatory(true);
    minModuleTemperatureModuleDataPoint.setSize(1);
    minModuleTemperatureModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << minModuleTemperatureModuleDataPoint;

    SunSpecDataPoint averageModuleTemperatureDataPoint;
    averageModuleTemperatureDataPoint.setName("ModTmpAvg");
    averageModuleTemperatureDataPoint.setLabel("Average Module Temperature");
    averageModuleTemperatureDataPoint.setDescription("Average temperature for all modules in the string.");
    averageModuleTemperatureDataPoint.setUnits("C");
    averageModuleTemperatureDataPoint.setMandatory(true);
    averageModuleTemperatureDataPoint.setSize(1);
    averageModuleTemperatureDataPoint.setScaleFactorName("ModTmp_SF");
    averageModuleTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << averageModuleTemperatureDataPoint;

    SunSpecDataPoint padDataPoint;
    padDataPoint.setName("Pad1");
    padDataPoint.setLabel("Pad");
    padDataPoint.setDescription("Pad register.");
    padDataPoint.setMandatory(true);
    padDataPoint.setSize(1);
    padDataPoint.setDataType(SunSpecDataPoint::stringToDataType("pad"));
    m_dataPoints << padDataPoint;

    SunSpecDataPoint contactorStatusDataPoint;
    contactorStatusDataPoint.setName("ConSt");
    contactorStatusDataPoint.setLabel("Contactor Status");
    contactorStatusDataPoint.setDescription("Status of the contactor(s) for the string.");
    contactorStatusDataPoint.setSize(2);
    contactorStatusDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints << contactorStatusDataPoint;

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
    stringEvent2DataPoint.setSize(2);
    stringEvent2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints << stringEvent2DataPoint;

    SunSpecDataPoint vendorEventBitfield1DataPoint;
    vendorEventBitfield1DataPoint.setName("EvtVnd1");
    vendorEventBitfield1DataPoint.setLabel("Vendor Event Bitfield 1");
    vendorEventBitfield1DataPoint.setDescription("Vendor defined events.");
    vendorEventBitfield1DataPoint.setSize(2);
    vendorEventBitfield1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints << vendorEventBitfield1DataPoint;

    SunSpecDataPoint vendorEventBitfield2DataPoint;
    vendorEventBitfield2DataPoint.setName("EvtVnd2");
    vendorEventBitfield2DataPoint.setLabel("Vendor Event Bitfield 2");
    vendorEventBitfield2DataPoint.setDescription("Vendor defined events.");
    vendorEventBitfield2DataPoint.setSize(2);
    vendorEventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints << vendorEventBitfield2DataPoint;

    SunSpecDataPoint enableDisableStringDataPoint;
    enableDisableStringDataPoint.setName("SetEna");
    enableDisableStringDataPoint.setLabel("Enable/Disable String");
    enableDisableStringDataPoint.setDescription("Enables and disables the string.  Should reset to 0 upon completion.");
    enableDisableStringDataPoint.setSize(1);
    enableDisableStringDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    enableDisableStringDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << enableDisableStringDataPoint;

    SunSpecDataPoint connectDisconnectStringDataPoint;
    connectDisconnectStringDataPoint.setName("SetCon");
    connectDisconnectStringDataPoint.setLabel("Connect/Disconnect String");
    connectDisconnectStringDataPoint.setDescription("Connects and disconnects the string.");
    connectDisconnectStringDataPoint.setSize(1);
    connectDisconnectStringDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    connectDisconnectStringDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << connectDisconnectStringDataPoint;

    SunSpecDataPoint SoC_SFDataPoint;
    SoC_SFDataPoint.setName("SoC_SF");
    SoC_SFDataPoint.setDescription("Scale factor for string state of charge.");
    SoC_SFDataPoint.setMandatory(true);
    SoC_SFDataPoint.setSize(1);
    SoC_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << SoC_SFDataPoint;

    SunSpecDataPoint SoH_SFDataPoint;
    SoH_SFDataPoint.setName("SoH_SF");
    SoH_SFDataPoint.setDescription("Scale factor for string state of health.");
    SoH_SFDataPoint.setSize(1);
    SoH_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << SoH_SFDataPoint;

    SunSpecDataPoint DoD_SFDataPoint;
    DoD_SFDataPoint.setName("DoD_SF");
    DoD_SFDataPoint.setDescription("Scale factor for string depth of discharge.");
    DoD_SFDataPoint.setSize(1);
    DoD_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << DoD_SFDataPoint;

    SunSpecDataPoint A_SFDataPoint;
    A_SFDataPoint.setName("A_SF");
    A_SFDataPoint.setDescription("Scale factor for string current.");
    A_SFDataPoint.setMandatory(true);
    A_SFDataPoint.setSize(1);
    A_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << A_SFDataPoint;

    SunSpecDataPoint V_SFDataPoint;
    V_SFDataPoint.setName("V_SF");
    V_SFDataPoint.setDescription("Scale factor for string voltage.");
    V_SFDataPoint.setSize(1);
    V_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << V_SFDataPoint;

    SunSpecDataPoint CellV_SFDataPoint;
    CellV_SFDataPoint.setName("CellV_SF");
    CellV_SFDataPoint.setDescription("Scale factor for cell voltage.");
    CellV_SFDataPoint.setMandatory(true);
    CellV_SFDataPoint.setSize(1);
    CellV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << CellV_SFDataPoint;

    SunSpecDataPoint ModTmp_SFDataPoint;
    ModTmp_SFDataPoint.setName("ModTmp_SF");
    ModTmp_SFDataPoint.setDescription("Scale factor for module temperature.");
    ModTmp_SFDataPoint.setMandatory(true);
    ModTmp_SFDataPoint.setSize(1);
    ModTmp_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << ModTmp_SFDataPoint;

    SunSpecDataPoint pad2DataPoint;
    pad2DataPoint.setName("Pad2");
    pad2DataPoint.setLabel("Pad2");
    pad2DataPoint.setDescription("Pad register.");
    pad2DataPoint.setMandatory(true);
    pad2DataPoint.setSize(1);
    pad2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("pad"));
    m_dataPoints << pad2DataPoint;

    SunSpecDataPoint pad3DataPoint;
    pad3DataPoint.setName("Pad3");
    pad3DataPoint.setLabel("Pad3");
    pad3DataPoint.setDescription("Pad register.");
    pad3DataPoint.setMandatory(true);
    pad3DataPoint.setSize(1);
    pad3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("pad"));
    m_dataPoints << pad3DataPoint;

    SunSpecDataPoint pad4DataPoint;
    pad4DataPoint.setName("Pad4");
    pad4DataPoint.setLabel("Pad4");
    pad4DataPoint.setDescription("Pad register.");
    pad4DataPoint.setMandatory(true);
    pad4DataPoint.setSize(1);
    pad4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("pad"));
    m_dataPoints << pad4DataPoint;

}

