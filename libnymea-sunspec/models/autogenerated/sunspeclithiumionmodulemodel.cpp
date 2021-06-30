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

SunSpecLithiumIonModuleModel::SunSpecLithiumIonModuleModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
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

void SunSpecLithiumIonModuleModel::readModelHeader()
{

}

void SunSpecLithiumIonModuleModel::readBlockData()
{

}

void SunSpecLithiumIonModuleModel::initDataPoints()
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
    stringIndexDataPoint.setName("StrIdx");
    stringIndexDataPoint.setLabel("String Index");
    stringIndexDataPoint.setDescription("Index of the string containing the module.");
    stringIndexDataPoint.setMandatory(true);
    stringIndexDataPoint.setSize(1);
    stringIndexDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << stringIndexDataPoint;

    SunSpecDataPoint moduleIndexDataPoint;
    moduleIndexDataPoint.setName("ModIdx");
    moduleIndexDataPoint.setLabel("Module Index");
    moduleIndexDataPoint.setDescription("Index of the module within the string.");
    moduleIndexDataPoint.setMandatory(true);
    moduleIndexDataPoint.setSize(1);
    moduleIndexDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << moduleIndexDataPoint;

    SunSpecDataPoint moduleCellCountDataPoint;
    moduleCellCountDataPoint.setName("NCell");
    moduleCellCountDataPoint.setLabel("Module Cell Count");
    moduleCellCountDataPoint.setDescription("Count of all cells in the module.");
    moduleCellCountDataPoint.setMandatory(true);
    moduleCellCountDataPoint.setSize(1);
    moduleCellCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << moduleCellCountDataPoint;

    SunSpecDataPoint moduleSocDataPoint;
    moduleSocDataPoint.setName("SoC");
    moduleSocDataPoint.setLabel("Module SoC");
    moduleSocDataPoint.setDescription("Module state of charge, expressed as a percentage.");
    moduleSocDataPoint.setUnits("%");
    moduleSocDataPoint.setSize(1);
    moduleSocDataPoint.setScaleFactorName("SoC_SF");
    moduleSocDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << moduleSocDataPoint;

    SunSpecDataPoint depthOfDischargeDataPoint;
    depthOfDischargeDataPoint.setName("DoD");
    depthOfDischargeDataPoint.setLabel("Depth of Discharge");
    depthOfDischargeDataPoint.setDescription("Depth of discharge for the module.");
    depthOfDischargeDataPoint.setUnits("%");
    depthOfDischargeDataPoint.setSize(1);
    depthOfDischargeDataPoint.setScaleFactorName("DoD_SF");
    depthOfDischargeDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << depthOfDischargeDataPoint;

    SunSpecDataPoint moduleSohDataPoint;
    moduleSohDataPoint.setName("SoH");
    moduleSohDataPoint.setLabel("Module SoH");
    moduleSohDataPoint.setDescription("Module state of health, expressed as a percentage.");
    moduleSohDataPoint.setUnits("%");
    moduleSohDataPoint.setSize(1);
    moduleSohDataPoint.setScaleFactorName("SoH_SF");
    moduleSohDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << moduleSohDataPoint;

    SunSpecDataPoint cycleCountDataPoint;
    cycleCountDataPoint.setName("NCyc");
    cycleCountDataPoint.setLabel("Cycle Count");
    cycleCountDataPoint.setDescription("Count of cycles executed.");
    cycleCountDataPoint.setSize(2);
    cycleCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint32"));
    m_dataPoints << cycleCountDataPoint;

    SunSpecDataPoint moduleVoltageDataPoint;
    moduleVoltageDataPoint.setName("V");
    moduleVoltageDataPoint.setLabel("Module Voltage");
    moduleVoltageDataPoint.setDescription("Voltage of the module.");
    moduleVoltageDataPoint.setUnits("V");
    moduleVoltageDataPoint.setMandatory(true);
    moduleVoltageDataPoint.setSize(1);
    moduleVoltageDataPoint.setScaleFactorName("V_SF");
    moduleVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << moduleVoltageDataPoint;

    SunSpecDataPoint maxCellVoltageDataPoint;
    maxCellVoltageDataPoint.setName("CellVMax");
    maxCellVoltageDataPoint.setLabel("Max Cell Voltage");
    maxCellVoltageDataPoint.setDescription("Maximum voltage for all cells in the module.");
    maxCellVoltageDataPoint.setUnits("V");
    maxCellVoltageDataPoint.setMandatory(true);
    maxCellVoltageDataPoint.setSize(1);
    maxCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    maxCellVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << maxCellVoltageDataPoint;

    SunSpecDataPoint maxCellVoltageCellDataPoint;
    maxCellVoltageCellDataPoint.setName("CellVMaxCell");
    maxCellVoltageCellDataPoint.setLabel("Max Cell Voltage Cell");
    maxCellVoltageCellDataPoint.setDescription("Cell with the maximum voltage.");
    maxCellVoltageCellDataPoint.setSize(1);
    maxCellVoltageCellDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << maxCellVoltageCellDataPoint;

    SunSpecDataPoint minCellVoltageDataPoint;
    minCellVoltageDataPoint.setName("CellVMin");
    minCellVoltageDataPoint.setLabel("Min Cell Voltage");
    minCellVoltageDataPoint.setDescription("Minimum voltage for all cells in the module.");
    minCellVoltageDataPoint.setUnits("V");
    minCellVoltageDataPoint.setMandatory(true);
    minCellVoltageDataPoint.setSize(1);
    minCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    minCellVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << minCellVoltageDataPoint;

    SunSpecDataPoint minCellVoltageCellDataPoint;
    minCellVoltageCellDataPoint.setName("CellVMinCell");
    minCellVoltageCellDataPoint.setLabel("Min Cell Voltage Cell");
    minCellVoltageCellDataPoint.setDescription("Cell with the minimum voltage.");
    minCellVoltageCellDataPoint.setSize(1);
    minCellVoltageCellDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << minCellVoltageCellDataPoint;

    SunSpecDataPoint averageCellVoltageDataPoint;
    averageCellVoltageDataPoint.setName("CellVAvg");
    averageCellVoltageDataPoint.setLabel("Average Cell Voltage");
    averageCellVoltageDataPoint.setDescription("Average voltage for all cells in the module.");
    averageCellVoltageDataPoint.setUnits("V");
    averageCellVoltageDataPoint.setMandatory(true);
    averageCellVoltageDataPoint.setSize(1);
    averageCellVoltageDataPoint.setScaleFactorName("CellV_SF");
    averageCellVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << averageCellVoltageDataPoint;

    SunSpecDataPoint maxCellTemperatureDataPoint;
    maxCellTemperatureDataPoint.setName("CellTmpMax");
    maxCellTemperatureDataPoint.setLabel("Max Cell Temperature");
    maxCellTemperatureDataPoint.setDescription("Maximum temperature for all cells in the module.");
    maxCellTemperatureDataPoint.setUnits("C");
    maxCellTemperatureDataPoint.setMandatory(true);
    maxCellTemperatureDataPoint.setSize(1);
    maxCellTemperatureDataPoint.setScaleFactorName("Tmp_SF");
    maxCellTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << maxCellTemperatureDataPoint;

    SunSpecDataPoint maxCellTemperatureCellDataPoint;
    maxCellTemperatureCellDataPoint.setName("CellTmpMaxCell");
    maxCellTemperatureCellDataPoint.setLabel("Max Cell Temperature Cell");
    maxCellTemperatureCellDataPoint.setDescription("Cell with the maximum cell temperature.");
    maxCellTemperatureCellDataPoint.setSize(1);
    maxCellTemperatureCellDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << maxCellTemperatureCellDataPoint;

    SunSpecDataPoint minCellTemperatureDataPoint;
    minCellTemperatureDataPoint.setName("CellTmpMin");
    minCellTemperatureDataPoint.setLabel("Min Cell Temperature");
    minCellTemperatureDataPoint.setDescription("Minimum temperature for all cells in the module.");
    minCellTemperatureDataPoint.setUnits("C");
    minCellTemperatureDataPoint.setMandatory(true);
    minCellTemperatureDataPoint.setSize(1);
    minCellTemperatureDataPoint.setScaleFactorName("Tmp_SF");
    minCellTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << minCellTemperatureDataPoint;

    SunSpecDataPoint minCellTemperatureCellDataPoint;
    minCellTemperatureCellDataPoint.setName("CellTmpMinCell");
    minCellTemperatureCellDataPoint.setLabel("Min Cell Temperature Cell");
    minCellTemperatureCellDataPoint.setDescription("Cell with the minimum cell temperature.");
    minCellTemperatureCellDataPoint.setSize(1);
    minCellTemperatureCellDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << minCellTemperatureCellDataPoint;

    SunSpecDataPoint averageCellTemperatureDataPoint;
    averageCellTemperatureDataPoint.setName("CellTmpAvg");
    averageCellTemperatureDataPoint.setLabel("Average Cell Temperature");
    averageCellTemperatureDataPoint.setDescription("Average temperature for all cells in the module.");
    averageCellTemperatureDataPoint.setUnits("C");
    averageCellTemperatureDataPoint.setMandatory(true);
    averageCellTemperatureDataPoint.setSize(1);
    averageCellTemperatureDataPoint.setScaleFactorName("Tmp_SF");
    averageCellTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << averageCellTemperatureDataPoint;

    SunSpecDataPoint balancedCellCountDataPoint;
    balancedCellCountDataPoint.setName("NCellBal");
    balancedCellCountDataPoint.setLabel("Balanced Cell Count");
    balancedCellCountDataPoint.setDescription("Number of cells currently being balanced in the module.");
    balancedCellCountDataPoint.setSize(1);
    balancedCellCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << balancedCellCountDataPoint;

    SunSpecDataPoint serialNumberDataPoint;
    serialNumberDataPoint.setName("SN");
    serialNumberDataPoint.setLabel("Serial Number");
    serialNumberDataPoint.setDescription("Serial number for the module.");
    serialNumberDataPoint.setSize(16);
    serialNumberDataPoint.setDataType(SunSpecDataPoint::stringToDataType("string"));
    m_dataPoints << serialNumberDataPoint;

    SunSpecDataPoint SoC_SFDataPoint;
    SoC_SFDataPoint.setName("SoC_SF");
    SoC_SFDataPoint.setDescription("Scale factor for module state of charge.");
    SoC_SFDataPoint.setSize(1);
    SoC_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << SoC_SFDataPoint;

    SunSpecDataPoint SoH_SFDataPoint;
    SoH_SFDataPoint.setName("SoH_SF");
    SoH_SFDataPoint.setDescription("Scale factor for module state of health.");
    SoH_SFDataPoint.setSize(1);
    SoH_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << SoH_SFDataPoint;

    SunSpecDataPoint DoD_SFDataPoint;
    DoD_SFDataPoint.setName("DoD_SF");
    DoD_SFDataPoint.setDescription("Scale factor for module depth of discharge.");
    DoD_SFDataPoint.setSize(1);
    DoD_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << DoD_SFDataPoint;

    SunSpecDataPoint V_SFDataPoint;
    V_SFDataPoint.setName("V_SF");
    V_SFDataPoint.setDescription("Scale factor for module voltage.");
    V_SFDataPoint.setMandatory(true);
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

    SunSpecDataPoint Tmp_SFDataPoint;
    Tmp_SFDataPoint.setName("Tmp_SF");
    Tmp_SFDataPoint.setDescription("Scale factor for module temperature.");
    Tmp_SFDataPoint.setMandatory(true);
    Tmp_SFDataPoint.setSize(1);
    Tmp_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << Tmp_SFDataPoint;

}

