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

#include "sunspeclithiumionmodulemodel.h"
#include "sunspecconnection.h"

SunSpecLithiumIonModuleModel::SunSpecLithiumIonModuleModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent) :
    SunSpecModel(connection, 805, 42, modbusStartRegister, parent)
{
    Q_ASSERT_X(length == 42,  "SunSpecLithiumIonModuleModel", QString("model length %1 given in the constructor does not match the model length from the specs %2.").arg(length).arg(modelLength()).toLatin1());
    Q_UNUSED(length)
    initDataPoints();
}

SunSpecLithiumIonModuleModel::~SunSpecLithiumIonModuleModel()
{

}

QString SunSpecLithiumIonModuleModel::name() const
{
    return "lithium-ion-module";
}

QString SunSpecLithiumIonModuleModel::description() const
{
    return QString();
}

QString SunSpecLithiumIonModuleModel::label() const
{
    return "Lithium-Ion Module Model";
}

quint16 SunSpecLithiumIonModuleModel::stringIndex() const
{
    return m_stringIndex;
}
quint16 SunSpecLithiumIonModuleModel::moduleIndex() const
{
    return m_moduleIndex;
}
quint16 SunSpecLithiumIonModuleModel::moduleCellCount() const
{
    return m_moduleCellCount;
}
float SunSpecLithiumIonModuleModel::moduleSoC() const
{
    return m_moduleSoC;
}
float SunSpecLithiumIonModuleModel::depthOfDischarge() const
{
    return m_depthOfDischarge;
}
float SunSpecLithiumIonModuleModel::moduleSoH() const
{
    return m_moduleSoH;
}
quint32 SunSpecLithiumIonModuleModel::cycleCount() const
{
    return m_cycleCount;
}
float SunSpecLithiumIonModuleModel::moduleVoltage() const
{
    return m_moduleVoltage;
}
float SunSpecLithiumIonModuleModel::maxCellVoltage() const
{
    return m_maxCellVoltage;
}
quint16 SunSpecLithiumIonModuleModel::maxCellVoltageCell() const
{
    return m_maxCellVoltageCell;
}
float SunSpecLithiumIonModuleModel::minCellVoltage() const
{
    return m_minCellVoltage;
}
quint16 SunSpecLithiumIonModuleModel::minCellVoltageCell() const
{
    return m_minCellVoltageCell;
}
float SunSpecLithiumIonModuleModel::averageCellVoltage() const
{
    return m_averageCellVoltage;
}
float SunSpecLithiumIonModuleModel::maxCellTemperature() const
{
    return m_maxCellTemperature;
}
quint16 SunSpecLithiumIonModuleModel::maxCellTemperatureCell() const
{
    return m_maxCellTemperatureCell;
}
float SunSpecLithiumIonModuleModel::minCellTemperature() const
{
    return m_minCellTemperature;
}
quint16 SunSpecLithiumIonModuleModel::minCellTemperatureCell() const
{
    return m_minCellTemperatureCell;
}
float SunSpecLithiumIonModuleModel::averageCellTemperature() const
{
    return m_averageCellTemperature;
}
quint16 SunSpecLithiumIonModuleModel::balancedCellCount() const
{
    return m_balancedCellCount;
}
QString SunSpecLithiumIonModuleModel::serialNumber() const
{
    return m_serialNumber;
}
void SunSpecLithiumIonModuleModel::processBlockData()
{
    // Scale factors
    m_soC_SF = m_dataPoints.value("SoC_SF").toInt16();
    m_soH_SF = m_dataPoints.value("SoH_SF").toInt16();
    m_doD_SF = m_dataPoints.value("DoD_SF").toInt16();
    m_v_SF = m_dataPoints.value("V_SF").toInt16();
    m_cellV_SF = m_dataPoints.value("CellV_SF").toInt16();
    m_tmp_SF = m_dataPoints.value("Tmp_SF").toInt16();

    // Update properties according to the data point type
    m_stringIndex = m_dataPoints.value("StrIdx").toUInt16();
    m_moduleIndex = m_dataPoints.value("ModIdx").toUInt16();
    m_moduleCellCount = m_dataPoints.value("NCell").toUInt16();
    m_moduleSoC = m_dataPoints.value("SoC").toFloatWithSSF(m_soC_SF);
    m_depthOfDischarge = m_dataPoints.value("DoD").toFloatWithSSF(m_doD_SF);
    m_moduleSoH = m_dataPoints.value("SoH").toFloatWithSSF(m_soH_SF);
    m_cycleCount = m_dataPoints.value("NCyc").toUInt32();
    m_moduleVoltage = m_dataPoints.value("V").toFloatWithSSF(m_v_SF);
    m_maxCellVoltage = m_dataPoints.value("CellVMax").toFloatWithSSF(m_cellV_SF);
    m_maxCellVoltageCell = m_dataPoints.value("CellVMaxCell").toUInt16();
    m_minCellVoltage = m_dataPoints.value("CellVMin").toFloatWithSSF(m_cellV_SF);
    m_minCellVoltageCell = m_dataPoints.value("CellVMinCell").toUInt16();
    m_averageCellVoltage = m_dataPoints.value("CellVAvg").toFloatWithSSF(m_cellV_SF);
    m_maxCellTemperature = m_dataPoints.value("CellTmpMax").toFloatWithSSF(m_tmp_SF);
    m_maxCellTemperatureCell = m_dataPoints.value("CellTmpMaxCell").toUInt16();
    m_minCellTemperature = m_dataPoints.value("CellTmpMin").toFloatWithSSF(m_tmp_SF);
    m_minCellTemperatureCell = m_dataPoints.value("CellTmpMinCell").toUInt16();
    m_averageCellTemperature = m_dataPoints.value("CellTmpAvg").toFloatWithSSF(m_tmp_SF);
    m_balancedCellCount = m_dataPoints.value("NCellBal").toUInt16();
    m_serialNumber = m_dataPoints.value("SN").toString();
}

void SunSpecLithiumIonModuleModel::initDataPoints()
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
    stringIndexDataPoint.setName("StrIdx");
    stringIndexDataPoint.setLabel("String Index");
    stringIndexDataPoint.setDescription("Index of the string containing the module.");
    stringIndexDataPoint.setMandatory(true);
    stringIndexDataPoint.setSize(1);
    stringIndexDataPoint.setAddressOffset(2);
    stringIndexDataPoint.setBlockOffset(0);
    stringIndexDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(stringIndexDataPoint.name(), stringIndexDataPoint);

    SunSpecDataPoint moduleIndexDataPoint;
    moduleIndexDataPoint.setName("ModIdx");
    moduleIndexDataPoint.setLabel("Module Index");
    moduleIndexDataPoint.setDescription("Index of the module within the string.");
    moduleIndexDataPoint.setMandatory(true);
    moduleIndexDataPoint.setSize(1);
    moduleIndexDataPoint.setAddressOffset(3);
    moduleIndexDataPoint.setBlockOffset(1);
    moduleIndexDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(moduleIndexDataPoint.name(), moduleIndexDataPoint);

    SunSpecDataPoint moduleCellCountDataPoint;
    moduleCellCountDataPoint.setName("NCell");
    moduleCellCountDataPoint.setLabel("Module Cell Count");
    moduleCellCountDataPoint.setDescription("Count of all cells in the module.");
    moduleCellCountDataPoint.setMandatory(true);
    moduleCellCountDataPoint.setSize(1);
    moduleCellCountDataPoint.setAddressOffset(4);
    moduleCellCountDataPoint.setBlockOffset(2);
    moduleCellCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(moduleCellCountDataPoint.name(), moduleCellCountDataPoint);

    SunSpecDataPoint moduleSoCDataPoint;
    moduleSoCDataPoint.setName("SoC");
    moduleSoCDataPoint.setLabel("Module SoC");
    moduleSoCDataPoint.setDescription("Module state of charge, expressed as a percentage.");
    moduleSoCDataPoint.setUnits("%");
    moduleSoCDataPoint.setSize(1);
    moduleSoCDataPoint.setAddressOffset(5);
    moduleSoCDataPoint.setBlockOffset(3);
    moduleSoCDataPoint.setScaleFactorName("SoC_SF");
    moduleSoCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(moduleSoCDataPoint.name(), moduleSoCDataPoint);

    SunSpecDataPoint depthOfDischargeDataPoint;
    depthOfDischargeDataPoint.setName("DoD");
    depthOfDischargeDataPoint.setLabel("Depth of Discharge");
    depthOfDischargeDataPoint.setDescription("Depth of discharge for the module.");
    depthOfDischargeDataPoint.setUnits("%");
    depthOfDischargeDataPoint.setSize(1);
    depthOfDischargeDataPoint.setAddressOffset(6);
    depthOfDischargeDataPoint.setBlockOffset(4);
    depthOfDischargeDataPoint.setScaleFactorName("DoD_SF");
    depthOfDischargeDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(depthOfDischargeDataPoint.name(), depthOfDischargeDataPoint);

    SunSpecDataPoint moduleSoHDataPoint;
    moduleSoHDataPoint.setName("SoH");
    moduleSoHDataPoint.setLabel("Module SoH");
    moduleSoHDataPoint.setDescription("Module state of health, expressed as a percentage.");
    moduleSoHDataPoint.setUnits("%");
    moduleSoHDataPoint.setSize(1);
    moduleSoHDataPoint.setAddressOffset(7);
    moduleSoHDataPoint.setBlockOffset(5);
    moduleSoHDataPoint.setScaleFactorName("SoH_SF");
    moduleSoHDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(moduleSoHDataPoint.name(), moduleSoHDataPoint);

    SunSpecDataPoint cycleCountDataPoint;
    cycleCountDataPoint.setName("NCyc");
    cycleCountDataPoint.setLabel("Cycle Count");
    cycleCountDataPoint.setDescription("Count of cycles executed.");
    cycleCountDataPoint.setSize(2);
    cycleCountDataPoint.setAddressOffset(8);
    cycleCountDataPoint.setBlockOffset(6);
    cycleCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint32"));
    m_dataPoints.insert(cycleCountDataPoint.name(), cycleCountDataPoint);

    SunSpecDataPoint moduleVoltageDataPoint;
    moduleVoltageDataPoint.setName("V");
    moduleVoltageDataPoint.setLabel("Module Voltage");
    moduleVoltageDataPoint.setDescription("Voltage of the module.");
    moduleVoltageDataPoint.setUnits("V");
    moduleVoltageDataPoint.setMandatory(true);
    moduleVoltageDataPoint.setSize(1);
    moduleVoltageDataPoint.setAddressOffset(10);
    moduleVoltageDataPoint.setBlockOffset(8);
    moduleVoltageDataPoint.setScaleFactorName("V_SF");
    moduleVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(moduleVoltageDataPoint.name(), moduleVoltageDataPoint);

    SunSpecDataPoint maxCellVoltageDataPoint;
    maxCellVoltageDataPoint.setName("CellVMax");
    maxCellVoltageDataPoint.setLabel("Max Cell Voltage");
    maxCellVoltageDataPoint.setDescription("Maximum voltage for all cells in the module.");
    maxCellVoltageDataPoint.setUnits("V");
    maxCellVoltageDataPoint.setMandatory(true);
    maxCellVoltageDataPoint.setSize(1);
    maxCellVoltageDataPoint.setAddressOffset(11);
    maxCellVoltageDataPoint.setBlockOffset(9);
    maxCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    maxCellVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(maxCellVoltageDataPoint.name(), maxCellVoltageDataPoint);

    SunSpecDataPoint maxCellVoltageCellDataPoint;
    maxCellVoltageCellDataPoint.setName("CellVMaxCell");
    maxCellVoltageCellDataPoint.setLabel("Max Cell Voltage Cell");
    maxCellVoltageCellDataPoint.setDescription("Cell with the maximum voltage.");
    maxCellVoltageCellDataPoint.setSize(1);
    maxCellVoltageCellDataPoint.setAddressOffset(12);
    maxCellVoltageCellDataPoint.setBlockOffset(10);
    maxCellVoltageCellDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(maxCellVoltageCellDataPoint.name(), maxCellVoltageCellDataPoint);

    SunSpecDataPoint minCellVoltageDataPoint;
    minCellVoltageDataPoint.setName("CellVMin");
    minCellVoltageDataPoint.setLabel("Min Cell Voltage");
    minCellVoltageDataPoint.setDescription("Minimum voltage for all cells in the module.");
    minCellVoltageDataPoint.setUnits("V");
    minCellVoltageDataPoint.setMandatory(true);
    minCellVoltageDataPoint.setSize(1);
    minCellVoltageDataPoint.setAddressOffset(13);
    minCellVoltageDataPoint.setBlockOffset(11);
    minCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    minCellVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(minCellVoltageDataPoint.name(), minCellVoltageDataPoint);

    SunSpecDataPoint minCellVoltageCellDataPoint;
    minCellVoltageCellDataPoint.setName("CellVMinCell");
    minCellVoltageCellDataPoint.setLabel("Min Cell Voltage Cell");
    minCellVoltageCellDataPoint.setDescription("Cell with the minimum voltage.");
    minCellVoltageCellDataPoint.setSize(1);
    minCellVoltageCellDataPoint.setAddressOffset(14);
    minCellVoltageCellDataPoint.setBlockOffset(12);
    minCellVoltageCellDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(minCellVoltageCellDataPoint.name(), minCellVoltageCellDataPoint);

    SunSpecDataPoint averageCellVoltageDataPoint;
    averageCellVoltageDataPoint.setName("CellVAvg");
    averageCellVoltageDataPoint.setLabel("Average Cell Voltage");
    averageCellVoltageDataPoint.setDescription("Average voltage for all cells in the module.");
    averageCellVoltageDataPoint.setUnits("V");
    averageCellVoltageDataPoint.setMandatory(true);
    averageCellVoltageDataPoint.setSize(1);
    averageCellVoltageDataPoint.setAddressOffset(15);
    averageCellVoltageDataPoint.setBlockOffset(13);
    averageCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    averageCellVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(averageCellVoltageDataPoint.name(), averageCellVoltageDataPoint);

    SunSpecDataPoint maxCellTemperatureDataPoint;
    maxCellTemperatureDataPoint.setName("CellTmpMax");
    maxCellTemperatureDataPoint.setLabel("Max Cell Temperature");
    maxCellTemperatureDataPoint.setDescription("Maximum temperature for all cells in the module.");
    maxCellTemperatureDataPoint.setUnits("C");
    maxCellTemperatureDataPoint.setMandatory(true);
    maxCellTemperatureDataPoint.setSize(1);
    maxCellTemperatureDataPoint.setAddressOffset(16);
    maxCellTemperatureDataPoint.setBlockOffset(14);
    maxCellTemperatureDataPoint.setScaleFactorName("Tmp_SF");
    maxCellTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints.insert(maxCellTemperatureDataPoint.name(), maxCellTemperatureDataPoint);

    SunSpecDataPoint maxCellTemperatureCellDataPoint;
    maxCellTemperatureCellDataPoint.setName("CellTmpMaxCell");
    maxCellTemperatureCellDataPoint.setLabel("Max Cell Temperature Cell");
    maxCellTemperatureCellDataPoint.setDescription("Cell with the maximum cell temperature.");
    maxCellTemperatureCellDataPoint.setSize(1);
    maxCellTemperatureCellDataPoint.setAddressOffset(17);
    maxCellTemperatureCellDataPoint.setBlockOffset(15);
    maxCellTemperatureCellDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(maxCellTemperatureCellDataPoint.name(), maxCellTemperatureCellDataPoint);

    SunSpecDataPoint minCellTemperatureDataPoint;
    minCellTemperatureDataPoint.setName("CellTmpMin");
    minCellTemperatureDataPoint.setLabel("Min Cell Temperature");
    minCellTemperatureDataPoint.setDescription("Minimum temperature for all cells in the module.");
    minCellTemperatureDataPoint.setUnits("C");
    minCellTemperatureDataPoint.setMandatory(true);
    minCellTemperatureDataPoint.setSize(1);
    minCellTemperatureDataPoint.setAddressOffset(18);
    minCellTemperatureDataPoint.setBlockOffset(16);
    minCellTemperatureDataPoint.setScaleFactorName("Tmp_SF");
    minCellTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints.insert(minCellTemperatureDataPoint.name(), minCellTemperatureDataPoint);

    SunSpecDataPoint minCellTemperatureCellDataPoint;
    minCellTemperatureCellDataPoint.setName("CellTmpMinCell");
    minCellTemperatureCellDataPoint.setLabel("Min Cell Temperature Cell");
    minCellTemperatureCellDataPoint.setDescription("Cell with the minimum cell temperature.");
    minCellTemperatureCellDataPoint.setSize(1);
    minCellTemperatureCellDataPoint.setAddressOffset(19);
    minCellTemperatureCellDataPoint.setBlockOffset(17);
    minCellTemperatureCellDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(minCellTemperatureCellDataPoint.name(), minCellTemperatureCellDataPoint);

    SunSpecDataPoint averageCellTemperatureDataPoint;
    averageCellTemperatureDataPoint.setName("CellTmpAvg");
    averageCellTemperatureDataPoint.setLabel("Average Cell Temperature");
    averageCellTemperatureDataPoint.setDescription("Average temperature for all cells in the module.");
    averageCellTemperatureDataPoint.setUnits("C");
    averageCellTemperatureDataPoint.setMandatory(true);
    averageCellTemperatureDataPoint.setSize(1);
    averageCellTemperatureDataPoint.setAddressOffset(20);
    averageCellTemperatureDataPoint.setBlockOffset(18);
    averageCellTemperatureDataPoint.setScaleFactorName("Tmp_SF");
    averageCellTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints.insert(averageCellTemperatureDataPoint.name(), averageCellTemperatureDataPoint);

    SunSpecDataPoint balancedCellCountDataPoint;
    balancedCellCountDataPoint.setName("NCellBal");
    balancedCellCountDataPoint.setLabel("Balanced Cell Count");
    balancedCellCountDataPoint.setDescription("Number of cells currently being balanced in the module.");
    balancedCellCountDataPoint.setSize(1);
    balancedCellCountDataPoint.setAddressOffset(21);
    balancedCellCountDataPoint.setBlockOffset(19);
    balancedCellCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(balancedCellCountDataPoint.name(), balancedCellCountDataPoint);

    SunSpecDataPoint serialNumberDataPoint;
    serialNumberDataPoint.setName("SN");
    serialNumberDataPoint.setLabel("Serial Number");
    serialNumberDataPoint.setDescription("Serial number for the module.");
    serialNumberDataPoint.setSize(16);
    serialNumberDataPoint.setAddressOffset(22);
    serialNumberDataPoint.setBlockOffset(20);
    serialNumberDataPoint.setDataType(SunSpecDataPoint::stringToDataType("string"));
    m_dataPoints.insert(serialNumberDataPoint.name(), serialNumberDataPoint);

    SunSpecDataPoint soC_SFDataPoint;
    soC_SFDataPoint.setName("SoC_SF");
    soC_SFDataPoint.setDescription("Scale factor for module state of charge.");
    soC_SFDataPoint.setSize(1);
    soC_SFDataPoint.setAddressOffset(38);
    soC_SFDataPoint.setBlockOffset(36);
    soC_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(soC_SFDataPoint.name(), soC_SFDataPoint);

    SunSpecDataPoint soH_SFDataPoint;
    soH_SFDataPoint.setName("SoH_SF");
    soH_SFDataPoint.setDescription("Scale factor for module state of health.");
    soH_SFDataPoint.setSize(1);
    soH_SFDataPoint.setAddressOffset(39);
    soH_SFDataPoint.setBlockOffset(37);
    soH_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(soH_SFDataPoint.name(), soH_SFDataPoint);

    SunSpecDataPoint doD_SFDataPoint;
    doD_SFDataPoint.setName("DoD_SF");
    doD_SFDataPoint.setDescription("Scale factor for module depth of discharge.");
    doD_SFDataPoint.setSize(1);
    doD_SFDataPoint.setAddressOffset(40);
    doD_SFDataPoint.setBlockOffset(38);
    doD_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(doD_SFDataPoint.name(), doD_SFDataPoint);

    SunSpecDataPoint v_SFDataPoint;
    v_SFDataPoint.setName("V_SF");
    v_SFDataPoint.setDescription("Scale factor for module voltage.");
    v_SFDataPoint.setMandatory(true);
    v_SFDataPoint.setSize(1);
    v_SFDataPoint.setAddressOffset(41);
    v_SFDataPoint.setBlockOffset(39);
    v_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(v_SFDataPoint.name(), v_SFDataPoint);

    SunSpecDataPoint cellV_SFDataPoint;
    cellV_SFDataPoint.setName("CellV_SF");
    cellV_SFDataPoint.setDescription("Scale factor for cell voltage.");
    cellV_SFDataPoint.setMandatory(true);
    cellV_SFDataPoint.setSize(1);
    cellV_SFDataPoint.setAddressOffset(42);
    cellV_SFDataPoint.setBlockOffset(40);
    cellV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(cellV_SFDataPoint.name(), cellV_SFDataPoint);

    SunSpecDataPoint tmp_SFDataPoint;
    tmp_SFDataPoint.setName("Tmp_SF");
    tmp_SFDataPoint.setDescription("Scale factor for module temperature.");
    tmp_SFDataPoint.setMandatory(true);
    tmp_SFDataPoint.setSize(1);
    tmp_SFDataPoint.setAddressOffset(43);
    tmp_SFDataPoint.setBlockOffset(41);
    tmp_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(tmp_SFDataPoint.name(), tmp_SFDataPoint);

}

