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
    m_supportedModelIds << 804;
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
    moduleCountDataPoint.setDescription("Count of modules in the string.");
    moduleCountDataPoint.setMandatory(true);
    moduleCountDataPoint.setSize(1);
    moduleCountDataPoint.setAddressOffset(3);
    moduleCountDataPoint.setBlockOffset(1);
    moduleCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(moduleCountDataPoint.name(), moduleCountDataPoint);

    SunSpecDataPoint stringStatusDataPoint;
    stringStatusDataPoint.setName("St");
    stringStatusDataPoint.setLabel("String Status");
    stringStatusDataPoint.setDescription("Current status of the string.");
    stringStatusDataPoint.setMandatory(true);
    stringStatusDataPoint.setSize(2);
    stringStatusDataPoint.setAddressOffset(4);
    stringStatusDataPoint.setBlockOffset(2);
    stringStatusDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(stringStatusDataPoint.name(), stringStatusDataPoint);

    SunSpecDataPoint connectionFailureReasonDataPoint;
    connectionFailureReasonDataPoint.setName("ConFail");
    connectionFailureReasonDataPoint.setLabel("Connection Failure Reason");
    connectionFailureReasonDataPoint.setSize(1);
    connectionFailureReasonDataPoint.setAddressOffset(6);
    connectionFailureReasonDataPoint.setBlockOffset(4);
    connectionFailureReasonDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    m_dataPoints.insert(connectionFailureReasonDataPoint.name(), connectionFailureReasonDataPoint);

    SunSpecDataPoint stringCellBalancingCountDataPoint;
    stringCellBalancingCountDataPoint.setName("NCellBal");
    stringCellBalancingCountDataPoint.setLabel("String Cell Balancing Count");
    stringCellBalancingCountDataPoint.setDescription("Number of cells currently being balanced in the string.");
    stringCellBalancingCountDataPoint.setSize(1);
    stringCellBalancingCountDataPoint.setAddressOffset(7);
    stringCellBalancingCountDataPoint.setBlockOffset(5);
    stringCellBalancingCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(stringCellBalancingCountDataPoint.name(), stringCellBalancingCountDataPoint);

    SunSpecDataPoint stringStateOfChargeDataPoint;
    stringStateOfChargeDataPoint.setName("SoC");
    stringStateOfChargeDataPoint.setLabel("String State of Charge");
    stringStateOfChargeDataPoint.setDescription("Battery string state of charge, expressed as a percentage.");
    stringStateOfChargeDataPoint.setUnits("%");
    stringStateOfChargeDataPoint.setMandatory(true);
    stringStateOfChargeDataPoint.setSize(1);
    stringStateOfChargeDataPoint.setAddressOffset(8);
    stringStateOfChargeDataPoint.setBlockOffset(6);
    stringStateOfChargeDataPoint.setScaleFactorName("SoC_SF");
    stringStateOfChargeDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(stringStateOfChargeDataPoint.name(), stringStateOfChargeDataPoint);

    SunSpecDataPoint stringDepthOfDischargeDataPoint;
    stringDepthOfDischargeDataPoint.setName("DoD");
    stringDepthOfDischargeDataPoint.setLabel("String Depth of Discharge");
    stringDepthOfDischargeDataPoint.setDescription("Depth of discharge for the string, expressed as a percentage.");
    stringDepthOfDischargeDataPoint.setUnits("%");
    stringDepthOfDischargeDataPoint.setSize(1);
    stringDepthOfDischargeDataPoint.setAddressOffset(9);
    stringDepthOfDischargeDataPoint.setBlockOffset(7);
    stringDepthOfDischargeDataPoint.setScaleFactorName("DoD_SF");
    stringDepthOfDischargeDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(stringDepthOfDischargeDataPoint.name(), stringDepthOfDischargeDataPoint);

    SunSpecDataPoint stringCycleCountDataPoint;
    stringCycleCountDataPoint.setName("NCyc");
    stringCycleCountDataPoint.setLabel("String Cycle Count");
    stringCycleCountDataPoint.setDescription("Number of discharge cycles executed upon the string.");
    stringCycleCountDataPoint.setSize(2);
    stringCycleCountDataPoint.setAddressOffset(10);
    stringCycleCountDataPoint.setBlockOffset(8);
    stringCycleCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint32"));
    m_dataPoints.insert(stringCycleCountDataPoint.name(), stringCycleCountDataPoint);

    SunSpecDataPoint stringStateOfHealthDataPoint;
    stringStateOfHealthDataPoint.setName("SoH");
    stringStateOfHealthDataPoint.setLabel("String State of Health");
    stringStateOfHealthDataPoint.setDescription("Battery string state of health, expressed as a percentage.");
    stringStateOfHealthDataPoint.setUnits("%");
    stringStateOfHealthDataPoint.setSize(1);
    stringStateOfHealthDataPoint.setAddressOffset(12);
    stringStateOfHealthDataPoint.setBlockOffset(10);
    stringStateOfHealthDataPoint.setScaleFactorName("SoH_SF");
    stringStateOfHealthDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(stringStateOfHealthDataPoint.name(), stringStateOfHealthDataPoint);

    SunSpecDataPoint stringCurrentDataPoint;
    stringCurrentDataPoint.setName("A");
    stringCurrentDataPoint.setLabel("String Current");
    stringCurrentDataPoint.setDescription("String current measurement.");
    stringCurrentDataPoint.setUnits("A");
    stringCurrentDataPoint.setMandatory(true);
    stringCurrentDataPoint.setSize(1);
    stringCurrentDataPoint.setAddressOffset(13);
    stringCurrentDataPoint.setBlockOffset(11);
    stringCurrentDataPoint.setScaleFactorName("A_SF");
    stringCurrentDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints.insert(stringCurrentDataPoint.name(), stringCurrentDataPoint);

    SunSpecDataPoint stringVoltageDataPoint;
    stringVoltageDataPoint.setName("V");
    stringVoltageDataPoint.setLabel("String Voltage");
    stringVoltageDataPoint.setDescription("String voltage measurement.");
    stringVoltageDataPoint.setUnits("V");
    stringVoltageDataPoint.setSize(1);
    stringVoltageDataPoint.setAddressOffset(14);
    stringVoltageDataPoint.setBlockOffset(12);
    stringVoltageDataPoint.setScaleFactorName("V_SF");
    stringVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(stringVoltageDataPoint.name(), stringVoltageDataPoint);

    SunSpecDataPoint maxCellVoltageDataPoint;
    maxCellVoltageDataPoint.setName("CellVMax");
    maxCellVoltageDataPoint.setLabel("Max Cell Voltage");
    maxCellVoltageDataPoint.setDescription("Maximum voltage for all cells in the string.");
    maxCellVoltageDataPoint.setUnits("V");
    maxCellVoltageDataPoint.setMandatory(true);
    maxCellVoltageDataPoint.setSize(1);
    maxCellVoltageDataPoint.setAddressOffset(15);
    maxCellVoltageDataPoint.setBlockOffset(13);
    maxCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    maxCellVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(maxCellVoltageDataPoint.name(), maxCellVoltageDataPoint);

    SunSpecDataPoint maxCellVoltageModuleDataPoint;
    maxCellVoltageModuleDataPoint.setName("CellVMaxMod");
    maxCellVoltageModuleDataPoint.setLabel("Max Cell Voltage Module");
    maxCellVoltageModuleDataPoint.setDescription("Module containing the cell with maximum cell voltage.");
    maxCellVoltageModuleDataPoint.setSize(1);
    maxCellVoltageModuleDataPoint.setAddressOffset(16);
    maxCellVoltageModuleDataPoint.setBlockOffset(14);
    maxCellVoltageModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(maxCellVoltageModuleDataPoint.name(), maxCellVoltageModuleDataPoint);

    SunSpecDataPoint minCellVoltageDataPoint;
    minCellVoltageDataPoint.setName("CellVMin");
    minCellVoltageDataPoint.setLabel("Min Cell Voltage");
    minCellVoltageDataPoint.setDescription("Minimum voltage for all cells in the string.");
    minCellVoltageDataPoint.setUnits("V");
    minCellVoltageDataPoint.setMandatory(true);
    minCellVoltageDataPoint.setSize(1);
    minCellVoltageDataPoint.setAddressOffset(17);
    minCellVoltageDataPoint.setBlockOffset(15);
    minCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    minCellVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(minCellVoltageDataPoint.name(), minCellVoltageDataPoint);

    SunSpecDataPoint minCellVoltageModuleDataPoint;
    minCellVoltageModuleDataPoint.setName("CellVMinMod");
    minCellVoltageModuleDataPoint.setLabel("Min Cell Voltage Module");
    minCellVoltageModuleDataPoint.setDescription("Module containing the cell with minimum cell voltage.");
    minCellVoltageModuleDataPoint.setSize(1);
    minCellVoltageModuleDataPoint.setAddressOffset(18);
    minCellVoltageModuleDataPoint.setBlockOffset(16);
    minCellVoltageModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(minCellVoltageModuleDataPoint.name(), minCellVoltageModuleDataPoint);

    SunSpecDataPoint averageCellVoltageDataPoint;
    averageCellVoltageDataPoint.setName("CellVAvg");
    averageCellVoltageDataPoint.setLabel("Average Cell Voltage");
    averageCellVoltageDataPoint.setDescription("Average voltage for all cells in the string.");
    averageCellVoltageDataPoint.setUnits("V");
    averageCellVoltageDataPoint.setMandatory(true);
    averageCellVoltageDataPoint.setSize(1);
    averageCellVoltageDataPoint.setAddressOffset(19);
    averageCellVoltageDataPoint.setBlockOffset(17);
    averageCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    averageCellVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(averageCellVoltageDataPoint.name(), averageCellVoltageDataPoint);

    SunSpecDataPoint maxModuleTemperatureDataPoint;
    maxModuleTemperatureDataPoint.setName("ModTmpMax");
    maxModuleTemperatureDataPoint.setLabel("Max Module Temperature");
    maxModuleTemperatureDataPoint.setDescription("Maximum temperature for all modules in the string.");
    maxModuleTemperatureDataPoint.setUnits("C");
    maxModuleTemperatureDataPoint.setMandatory(true);
    maxModuleTemperatureDataPoint.setSize(1);
    maxModuleTemperatureDataPoint.setAddressOffset(20);
    maxModuleTemperatureDataPoint.setBlockOffset(18);
    maxModuleTemperatureDataPoint.setScaleFactorName("ModTmp_SF");
    maxModuleTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints.insert(maxModuleTemperatureDataPoint.name(), maxModuleTemperatureDataPoint);

    SunSpecDataPoint maxModuleTemperatureModuleDataPoint;
    maxModuleTemperatureModuleDataPoint.setName("ModTmpMaxMod");
    maxModuleTemperatureModuleDataPoint.setLabel("Max Module Temperature Module");
    maxModuleTemperatureModuleDataPoint.setDescription("Module with the maximum temperature.");
    maxModuleTemperatureModuleDataPoint.setMandatory(true);
    maxModuleTemperatureModuleDataPoint.setSize(1);
    maxModuleTemperatureModuleDataPoint.setAddressOffset(21);
    maxModuleTemperatureModuleDataPoint.setBlockOffset(19);
    maxModuleTemperatureModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(maxModuleTemperatureModuleDataPoint.name(), maxModuleTemperatureModuleDataPoint);

    SunSpecDataPoint minModuleTemperatureDataPoint;
    minModuleTemperatureDataPoint.setName("ModTmpMin");
    minModuleTemperatureDataPoint.setLabel("Min Module Temperature");
    minModuleTemperatureDataPoint.setDescription("Minimum temperature for all modules in the string.");
    minModuleTemperatureDataPoint.setUnits("C");
    minModuleTemperatureDataPoint.setMandatory(true);
    minModuleTemperatureDataPoint.setSize(1);
    minModuleTemperatureDataPoint.setAddressOffset(22);
    minModuleTemperatureDataPoint.setBlockOffset(20);
    minModuleTemperatureDataPoint.setScaleFactorName("ModTmp_SF");
    minModuleTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints.insert(minModuleTemperatureDataPoint.name(), minModuleTemperatureDataPoint);

    SunSpecDataPoint minModuleTemperatureModuleDataPoint;
    minModuleTemperatureModuleDataPoint.setName("ModTmpMinMod");
    minModuleTemperatureModuleDataPoint.setLabel("Min Module Temperature Module");
    minModuleTemperatureModuleDataPoint.setDescription("Module with the minimum temperature.");
    minModuleTemperatureModuleDataPoint.setMandatory(true);
    minModuleTemperatureModuleDataPoint.setSize(1);
    minModuleTemperatureModuleDataPoint.setAddressOffset(23);
    minModuleTemperatureModuleDataPoint.setBlockOffset(21);
    minModuleTemperatureModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(minModuleTemperatureModuleDataPoint.name(), minModuleTemperatureModuleDataPoint);

    SunSpecDataPoint averageModuleTemperatureDataPoint;
    averageModuleTemperatureDataPoint.setName("ModTmpAvg");
    averageModuleTemperatureDataPoint.setLabel("Average Module Temperature");
    averageModuleTemperatureDataPoint.setDescription("Average temperature for all modules in the string.");
    averageModuleTemperatureDataPoint.setUnits("C");
    averageModuleTemperatureDataPoint.setMandatory(true);
    averageModuleTemperatureDataPoint.setSize(1);
    averageModuleTemperatureDataPoint.setAddressOffset(24);
    averageModuleTemperatureDataPoint.setBlockOffset(22);
    averageModuleTemperatureDataPoint.setScaleFactorName("ModTmp_SF");
    averageModuleTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints.insert(averageModuleTemperatureDataPoint.name(), averageModuleTemperatureDataPoint);

    SunSpecDataPoint padDataPoint;
    padDataPoint.setName("Pad1");
    padDataPoint.setLabel("Pad");
    padDataPoint.setDescription("Pad register.");
    padDataPoint.setMandatory(true);
    padDataPoint.setSize(1);
    padDataPoint.setAddressOffset(25);
    padDataPoint.setBlockOffset(23);
    padDataPoint.setDataType(SunSpecDataPoint::stringToDataType("pad"));
    m_dataPoints.insert(padDataPoint.name(), padDataPoint);

    SunSpecDataPoint contactorStatusDataPoint;
    contactorStatusDataPoint.setName("ConSt");
    contactorStatusDataPoint.setLabel("Contactor Status");
    contactorStatusDataPoint.setDescription("Status of the contactor(s) for the string.");
    contactorStatusDataPoint.setSize(2);
    contactorStatusDataPoint.setAddressOffset(26);
    contactorStatusDataPoint.setBlockOffset(24);
    contactorStatusDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(contactorStatusDataPoint.name(), contactorStatusDataPoint);

    SunSpecDataPoint stringEvent1DataPoint;
    stringEvent1DataPoint.setName("Evt1");
    stringEvent1DataPoint.setLabel("String Event 1");
    stringEvent1DataPoint.setDescription("Alarms, warnings and status values.  Bit flags.");
    stringEvent1DataPoint.setMandatory(true);
    stringEvent1DataPoint.setSize(2);
    stringEvent1DataPoint.setAddressOffset(28);
    stringEvent1DataPoint.setBlockOffset(26);
    stringEvent1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(stringEvent1DataPoint.name(), stringEvent1DataPoint);

    SunSpecDataPoint stringEvent2DataPoint;
    stringEvent2DataPoint.setName("Evt2");
    stringEvent2DataPoint.setLabel("String Event 2");
    stringEvent2DataPoint.setDescription("Alarms, warnings and status values.  Bit flags.");
    stringEvent2DataPoint.setSize(2);
    stringEvent2DataPoint.setAddressOffset(30);
    stringEvent2DataPoint.setBlockOffset(28);
    stringEvent2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(stringEvent2DataPoint.name(), stringEvent2DataPoint);

    SunSpecDataPoint vendorEventBitfield1DataPoint;
    vendorEventBitfield1DataPoint.setName("EvtVnd1");
    vendorEventBitfield1DataPoint.setLabel("Vendor Event Bitfield 1");
    vendorEventBitfield1DataPoint.setDescription("Vendor defined events.");
    vendorEventBitfield1DataPoint.setSize(2);
    vendorEventBitfield1DataPoint.setAddressOffset(32);
    vendorEventBitfield1DataPoint.setBlockOffset(30);
    vendorEventBitfield1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(vendorEventBitfield1DataPoint.name(), vendorEventBitfield1DataPoint);

    SunSpecDataPoint vendorEventBitfield2DataPoint;
    vendorEventBitfield2DataPoint.setName("EvtVnd2");
    vendorEventBitfield2DataPoint.setLabel("Vendor Event Bitfield 2");
    vendorEventBitfield2DataPoint.setDescription("Vendor defined events.");
    vendorEventBitfield2DataPoint.setSize(2);
    vendorEventBitfield2DataPoint.setAddressOffset(34);
    vendorEventBitfield2DataPoint.setBlockOffset(32);
    vendorEventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(vendorEventBitfield2DataPoint.name(), vendorEventBitfield2DataPoint);

    SunSpecDataPoint enableDisableStringDataPoint;
    enableDisableStringDataPoint.setName("SetEna");
    enableDisableStringDataPoint.setLabel("Enable/Disable String");
    enableDisableStringDataPoint.setDescription("Enables and disables the string.  Should reset to 0 upon completion.");
    enableDisableStringDataPoint.setSize(1);
    enableDisableStringDataPoint.setAddressOffset(36);
    enableDisableStringDataPoint.setBlockOffset(34);
    enableDisableStringDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    enableDisableStringDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(enableDisableStringDataPoint.name(), enableDisableStringDataPoint);

    SunSpecDataPoint connectDisconnectStringDataPoint;
    connectDisconnectStringDataPoint.setName("SetCon");
    connectDisconnectStringDataPoint.setLabel("Connect/Disconnect String");
    connectDisconnectStringDataPoint.setDescription("Connects and disconnects the string.");
    connectDisconnectStringDataPoint.setSize(1);
    connectDisconnectStringDataPoint.setAddressOffset(37);
    connectDisconnectStringDataPoint.setBlockOffset(35);
    connectDisconnectStringDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    connectDisconnectStringDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(connectDisconnectStringDataPoint.name(), connectDisconnectStringDataPoint);

    SunSpecDataPoint SoC_SFDataPoint;
    SoC_SFDataPoint.setName("SoC_SF");
    SoC_SFDataPoint.setDescription("Scale factor for string state of charge.");
    SoC_SFDataPoint.setMandatory(true);
    SoC_SFDataPoint.setSize(1);
    SoC_SFDataPoint.setAddressOffset(38);
    SoC_SFDataPoint.setBlockOffset(36);
    SoC_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(SoC_SFDataPoint.name(), SoC_SFDataPoint);

    SunSpecDataPoint SoH_SFDataPoint;
    SoH_SFDataPoint.setName("SoH_SF");
    SoH_SFDataPoint.setDescription("Scale factor for string state of health.");
    SoH_SFDataPoint.setSize(1);
    SoH_SFDataPoint.setAddressOffset(39);
    SoH_SFDataPoint.setBlockOffset(37);
    SoH_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(SoH_SFDataPoint.name(), SoH_SFDataPoint);

    SunSpecDataPoint DoD_SFDataPoint;
    DoD_SFDataPoint.setName("DoD_SF");
    DoD_SFDataPoint.setDescription("Scale factor for string depth of discharge.");
    DoD_SFDataPoint.setSize(1);
    DoD_SFDataPoint.setAddressOffset(40);
    DoD_SFDataPoint.setBlockOffset(38);
    DoD_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(DoD_SFDataPoint.name(), DoD_SFDataPoint);

    SunSpecDataPoint A_SFDataPoint;
    A_SFDataPoint.setName("A_SF");
    A_SFDataPoint.setDescription("Scale factor for string current.");
    A_SFDataPoint.setMandatory(true);
    A_SFDataPoint.setSize(1);
    A_SFDataPoint.setAddressOffset(41);
    A_SFDataPoint.setBlockOffset(39);
    A_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(A_SFDataPoint.name(), A_SFDataPoint);

    SunSpecDataPoint V_SFDataPoint;
    V_SFDataPoint.setName("V_SF");
    V_SFDataPoint.setDescription("Scale factor for string voltage.");
    V_SFDataPoint.setSize(1);
    V_SFDataPoint.setAddressOffset(42);
    V_SFDataPoint.setBlockOffset(40);
    V_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(V_SFDataPoint.name(), V_SFDataPoint);

    SunSpecDataPoint CellV_SFDataPoint;
    CellV_SFDataPoint.setName("CellV_SF");
    CellV_SFDataPoint.setDescription("Scale factor for cell voltage.");
    CellV_SFDataPoint.setMandatory(true);
    CellV_SFDataPoint.setSize(1);
    CellV_SFDataPoint.setAddressOffset(43);
    CellV_SFDataPoint.setBlockOffset(41);
    CellV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(CellV_SFDataPoint.name(), CellV_SFDataPoint);

    SunSpecDataPoint ModTmp_SFDataPoint;
    ModTmp_SFDataPoint.setName("ModTmp_SF");
    ModTmp_SFDataPoint.setDescription("Scale factor for module temperature.");
    ModTmp_SFDataPoint.setMandatory(true);
    ModTmp_SFDataPoint.setSize(1);
    ModTmp_SFDataPoint.setAddressOffset(44);
    ModTmp_SFDataPoint.setBlockOffset(42);
    ModTmp_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(ModTmp_SFDataPoint.name(), ModTmp_SFDataPoint);

    SunSpecDataPoint pad2DataPoint;
    pad2DataPoint.setName("Pad2");
    pad2DataPoint.setLabel("Pad2");
    pad2DataPoint.setDescription("Pad register.");
    pad2DataPoint.setMandatory(true);
    pad2DataPoint.setSize(1);
    pad2DataPoint.setAddressOffset(45);
    pad2DataPoint.setBlockOffset(43);
    pad2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("pad"));
    m_dataPoints.insert(pad2DataPoint.name(), pad2DataPoint);

    SunSpecDataPoint pad3DataPoint;
    pad3DataPoint.setName("Pad3");
    pad3DataPoint.setLabel("Pad3");
    pad3DataPoint.setDescription("Pad register.");
    pad3DataPoint.setMandatory(true);
    pad3DataPoint.setSize(1);
    pad3DataPoint.setAddressOffset(46);
    pad3DataPoint.setBlockOffset(44);
    pad3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("pad"));
    m_dataPoints.insert(pad3DataPoint.name(), pad3DataPoint);

    SunSpecDataPoint pad4DataPoint;
    pad4DataPoint.setName("Pad4");
    pad4DataPoint.setLabel("Pad4");
    pad4DataPoint.setDescription("Pad register.");
    pad4DataPoint.setMandatory(true);
    pad4DataPoint.setSize(1);
    pad4DataPoint.setAddressOffset(47);
    pad4DataPoint.setBlockOffset(45);
    pad4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("pad"));
    m_dataPoints.insert(pad4DataPoint.name(), pad4DataPoint);

}

