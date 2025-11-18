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

#include "sunspeclithiumionmodulemodel.h"
#include "sunspecconnection.h"

SunSpecLithiumIonModuleModelRepeatingBlock::SunSpecLithiumIonModuleModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecLithiumIonModuleModel *parent) :
    SunSpecModelRepeatingBlock(blockIndex, blockSize, modbusStartRegister, parent)
{
    m_parentModel = parent;
    m_byteOrder = parent->byteOrder();
    initDataPoints();
}

QString SunSpecLithiumIonModuleModelRepeatingBlock::name() const
{
    return "lithium-ion-module-cell";
}

SunSpecLithiumIonModuleModel *SunSpecLithiumIonModuleModelRepeatingBlock::parentModel() const
{
    return m_parentModel;
}

float SunSpecLithiumIonModuleModelRepeatingBlock::cellVoltage() const
{
    return m_cellVoltage;
}
float SunSpecLithiumIonModuleModelRepeatingBlock::cellTemperature() const
{
    return m_cellTemperature;
}
SunSpecLithiumIonModuleModelRepeatingBlock::CellstFlags SunSpecLithiumIonModuleModelRepeatingBlock::cellStatus() const
{
    return m_cellStatus;
}

void SunSpecLithiumIonModuleModelRepeatingBlock::initDataPoints()
{
    SunSpecDataPoint cellVoltageDataPoint;
    cellVoltageDataPoint.setName("CellV");
    cellVoltageDataPoint.setLabel("Cell Voltage");
    cellVoltageDataPoint.setDescription("Cell terminal voltage.");
    cellVoltageDataPoint.setUnits("V");
    cellVoltageDataPoint.setMandatory(true);
    cellVoltageDataPoint.setSize(1);
    cellVoltageDataPoint.setAddressOffset(0);
    cellVoltageDataPoint.setScaleFactorName("CellV_SF");
    cellVoltageDataPoint.setSunSpecDataType("uint16");
    cellVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(cellVoltageDataPoint.name(), cellVoltageDataPoint);

    SunSpecDataPoint cellTemperatureDataPoint;
    cellTemperatureDataPoint.setName("CellTmp");
    cellTemperatureDataPoint.setLabel("Cell Temperature");
    cellTemperatureDataPoint.setDescription("Cell temperature.");
    cellTemperatureDataPoint.setUnits("C");
    cellTemperatureDataPoint.setMandatory(true);
    cellTemperatureDataPoint.setSize(1);
    cellTemperatureDataPoint.setAddressOffset(1);
    cellTemperatureDataPoint.setScaleFactorName("Tmp_SF");
    cellTemperatureDataPoint.setSunSpecDataType("int16");
    cellTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(cellTemperatureDataPoint.name(), cellTemperatureDataPoint);

    SunSpecDataPoint cellStatusDataPoint;
    cellStatusDataPoint.setName("CellSt");
    cellStatusDataPoint.setLabel("Cell Status");
    cellStatusDataPoint.setDescription("Status of the cell.");
    cellStatusDataPoint.setSize(2);
    cellStatusDataPoint.setAddressOffset(2);
    cellStatusDataPoint.setBlockOffset(0);
    cellStatusDataPoint.setSunSpecDataType("bitfield32");
    cellStatusDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(cellStatusDataPoint.name(), cellStatusDataPoint);

}

void SunSpecLithiumIonModuleModelRepeatingBlock::processBlockData()
{
    // Update properties according to the data point type
    if (m_dataPoints.value("CellV").isValid())
        m_cellVoltage = m_dataPoints.value("CellV").toFloatWithSSF(m_parentModel->cellV_SF());

    if (m_dataPoints.value("CellTmp").isValid())
        m_cellTemperature = m_dataPoints.value("CellTmp").toFloatWithSSF(m_parentModel->tmp_SF());

    if (m_dataPoints.value("CellSt").isValid())
        m_cellStatus = static_cast<CellstFlags>(m_dataPoints.value("CellSt").toUInt32());


    qCDebug(dcSunSpecModelData()) << this;
}


SunSpecLithiumIonModuleModel::SunSpecLithiumIonModuleModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 805, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixedAndRepeating;

    initDataPoints();

    connect(this, &SunSpecModel::initFinished, this, &SunSpecLithiumIonModuleModel::setupRepeatingBlocks);
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
qint16 SunSpecLithiumIonModuleModel::soC_SF() const
{
    return m_soC_SF;
}
qint16 SunSpecLithiumIonModuleModel::soH_SF() const
{
    return m_soH_SF;
}
qint16 SunSpecLithiumIonModuleModel::doD_SF() const
{
    return m_doD_SF;
}
qint16 SunSpecLithiumIonModuleModel::v_SF() const
{
    return m_v_SF;
}
qint16 SunSpecLithiumIonModuleModel::cellV_SF() const
{
    return m_cellV_SF;
}
qint16 SunSpecLithiumIonModuleModel::tmp_SF() const
{
    return m_tmp_SF;
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
    stringIndexDataPoint.setName("StrIdx");
    stringIndexDataPoint.setLabel("String Index");
    stringIndexDataPoint.setDescription("Index of the string containing the module.");
    stringIndexDataPoint.setMandatory(true);
    stringIndexDataPoint.setSize(1);
    stringIndexDataPoint.setAddressOffset(2);
    stringIndexDataPoint.setBlockOffset(0);
    stringIndexDataPoint.setSunSpecDataType("uint16");
    stringIndexDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stringIndexDataPoint.name(), stringIndexDataPoint);

    SunSpecDataPoint moduleIndexDataPoint;
    moduleIndexDataPoint.setName("ModIdx");
    moduleIndexDataPoint.setLabel("Module Index");
    moduleIndexDataPoint.setDescription("Index of the module within the string.");
    moduleIndexDataPoint.setMandatory(true);
    moduleIndexDataPoint.setSize(1);
    moduleIndexDataPoint.setAddressOffset(3);
    moduleIndexDataPoint.setBlockOffset(1);
    moduleIndexDataPoint.setSunSpecDataType("uint16");
    moduleIndexDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(moduleIndexDataPoint.name(), moduleIndexDataPoint);

    SunSpecDataPoint moduleCellCountDataPoint;
    moduleCellCountDataPoint.setName("NCell");
    moduleCellCountDataPoint.setLabel("Module Cell Count");
    moduleCellCountDataPoint.setDescription("Count of all cells in the module.");
    moduleCellCountDataPoint.setMandatory(true);
    moduleCellCountDataPoint.setSize(1);
    moduleCellCountDataPoint.setAddressOffset(4);
    moduleCellCountDataPoint.setBlockOffset(2);
    moduleCellCountDataPoint.setSunSpecDataType("uint16");
    moduleCellCountDataPoint.setByteOrder(m_byteOrder);
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
    moduleSoCDataPoint.setSunSpecDataType("uint16");
    moduleSoCDataPoint.setByteOrder(m_byteOrder);
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
    depthOfDischargeDataPoint.setSunSpecDataType("uint16");
    depthOfDischargeDataPoint.setByteOrder(m_byteOrder);
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
    moduleSoHDataPoint.setSunSpecDataType("uint16");
    moduleSoHDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(moduleSoHDataPoint.name(), moduleSoHDataPoint);

    SunSpecDataPoint cycleCountDataPoint;
    cycleCountDataPoint.setName("NCyc");
    cycleCountDataPoint.setLabel("Cycle Count");
    cycleCountDataPoint.setDescription("Count of cycles executed.");
    cycleCountDataPoint.setSize(2);
    cycleCountDataPoint.setAddressOffset(8);
    cycleCountDataPoint.setBlockOffset(6);
    cycleCountDataPoint.setSunSpecDataType("uint32");
    cycleCountDataPoint.setByteOrder(m_byteOrder);
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
    moduleVoltageDataPoint.setSunSpecDataType("uint16");
    moduleVoltageDataPoint.setByteOrder(m_byteOrder);
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
    maxCellVoltageDataPoint.setSunSpecDataType("uint16");
    maxCellVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxCellVoltageDataPoint.name(), maxCellVoltageDataPoint);

    SunSpecDataPoint maxCellVoltageCellDataPoint;
    maxCellVoltageCellDataPoint.setName("CellVMaxCell");
    maxCellVoltageCellDataPoint.setLabel("Max Cell Voltage Cell");
    maxCellVoltageCellDataPoint.setDescription("Cell with the maximum voltage.");
    maxCellVoltageCellDataPoint.setSize(1);
    maxCellVoltageCellDataPoint.setAddressOffset(12);
    maxCellVoltageCellDataPoint.setBlockOffset(10);
    maxCellVoltageCellDataPoint.setSunSpecDataType("uint16");
    maxCellVoltageCellDataPoint.setByteOrder(m_byteOrder);
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
    minCellVoltageDataPoint.setSunSpecDataType("uint16");
    minCellVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minCellVoltageDataPoint.name(), minCellVoltageDataPoint);

    SunSpecDataPoint minCellVoltageCellDataPoint;
    minCellVoltageCellDataPoint.setName("CellVMinCell");
    minCellVoltageCellDataPoint.setLabel("Min Cell Voltage Cell");
    minCellVoltageCellDataPoint.setDescription("Cell with the minimum voltage.");
    minCellVoltageCellDataPoint.setSize(1);
    minCellVoltageCellDataPoint.setAddressOffset(14);
    minCellVoltageCellDataPoint.setBlockOffset(12);
    minCellVoltageCellDataPoint.setSunSpecDataType("uint16");
    minCellVoltageCellDataPoint.setByteOrder(m_byteOrder);
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
    averageCellVoltageDataPoint.setSunSpecDataType("uint16");
    averageCellVoltageDataPoint.setByteOrder(m_byteOrder);
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
    maxCellTemperatureDataPoint.setSunSpecDataType("int16");
    maxCellTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(maxCellTemperatureDataPoint.name(), maxCellTemperatureDataPoint);

    SunSpecDataPoint maxCellTemperatureCellDataPoint;
    maxCellTemperatureCellDataPoint.setName("CellTmpMaxCell");
    maxCellTemperatureCellDataPoint.setLabel("Max Cell Temperature Cell");
    maxCellTemperatureCellDataPoint.setDescription("Cell with the maximum cell temperature.");
    maxCellTemperatureCellDataPoint.setSize(1);
    maxCellTemperatureCellDataPoint.setAddressOffset(17);
    maxCellTemperatureCellDataPoint.setBlockOffset(15);
    maxCellTemperatureCellDataPoint.setSunSpecDataType("uint16");
    maxCellTemperatureCellDataPoint.setByteOrder(m_byteOrder);
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
    minCellTemperatureDataPoint.setSunSpecDataType("int16");
    minCellTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(minCellTemperatureDataPoint.name(), minCellTemperatureDataPoint);

    SunSpecDataPoint minCellTemperatureCellDataPoint;
    minCellTemperatureCellDataPoint.setName("CellTmpMinCell");
    minCellTemperatureCellDataPoint.setLabel("Min Cell Temperature Cell");
    minCellTemperatureCellDataPoint.setDescription("Cell with the minimum cell temperature.");
    minCellTemperatureCellDataPoint.setSize(1);
    minCellTemperatureCellDataPoint.setAddressOffset(19);
    minCellTemperatureCellDataPoint.setBlockOffset(17);
    minCellTemperatureCellDataPoint.setSunSpecDataType("uint16");
    minCellTemperatureCellDataPoint.setByteOrder(m_byteOrder);
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
    averageCellTemperatureDataPoint.setSunSpecDataType("int16");
    averageCellTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(averageCellTemperatureDataPoint.name(), averageCellTemperatureDataPoint);

    SunSpecDataPoint balancedCellCountDataPoint;
    balancedCellCountDataPoint.setName("NCellBal");
    balancedCellCountDataPoint.setLabel("Balanced Cell Count");
    balancedCellCountDataPoint.setDescription("Number of cells currently being balanced in the module.");
    balancedCellCountDataPoint.setSize(1);
    balancedCellCountDataPoint.setAddressOffset(21);
    balancedCellCountDataPoint.setBlockOffset(19);
    balancedCellCountDataPoint.setSunSpecDataType("uint16");
    balancedCellCountDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(balancedCellCountDataPoint.name(), balancedCellCountDataPoint);

    SunSpecDataPoint serialNumberDataPoint;
    serialNumberDataPoint.setName("SN");
    serialNumberDataPoint.setLabel("Serial Number");
    serialNumberDataPoint.setDescription("Serial number for the module.");
    serialNumberDataPoint.setSize(16);
    serialNumberDataPoint.setAddressOffset(22);
    serialNumberDataPoint.setBlockOffset(20);
    serialNumberDataPoint.setSunSpecDataType("string");
    serialNumberDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(serialNumberDataPoint.name(), serialNumberDataPoint);

    SunSpecDataPoint soC_SFDataPoint;
    soC_SFDataPoint.setName("SoC_SF");
    soC_SFDataPoint.setDescription("Scale factor for module state of charge.");
    soC_SFDataPoint.setSize(1);
    soC_SFDataPoint.setAddressOffset(38);
    soC_SFDataPoint.setBlockOffset(36);
    soC_SFDataPoint.setSunSpecDataType("sunssf");
    soC_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(soC_SFDataPoint.name(), soC_SFDataPoint);

    SunSpecDataPoint soH_SFDataPoint;
    soH_SFDataPoint.setName("SoH_SF");
    soH_SFDataPoint.setDescription("Scale factor for module state of health.");
    soH_SFDataPoint.setSize(1);
    soH_SFDataPoint.setAddressOffset(39);
    soH_SFDataPoint.setBlockOffset(37);
    soH_SFDataPoint.setSunSpecDataType("sunssf");
    soH_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(soH_SFDataPoint.name(), soH_SFDataPoint);

    SunSpecDataPoint doD_SFDataPoint;
    doD_SFDataPoint.setName("DoD_SF");
    doD_SFDataPoint.setDescription("Scale factor for module depth of discharge.");
    doD_SFDataPoint.setSize(1);
    doD_SFDataPoint.setAddressOffset(40);
    doD_SFDataPoint.setBlockOffset(38);
    doD_SFDataPoint.setSunSpecDataType("sunssf");
    doD_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(doD_SFDataPoint.name(), doD_SFDataPoint);

    SunSpecDataPoint v_SFDataPoint;
    v_SFDataPoint.setName("V_SF");
    v_SFDataPoint.setDescription("Scale factor for module voltage.");
    v_SFDataPoint.setMandatory(true);
    v_SFDataPoint.setSize(1);
    v_SFDataPoint.setAddressOffset(41);
    v_SFDataPoint.setBlockOffset(39);
    v_SFDataPoint.setSunSpecDataType("sunssf");
    v_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(v_SFDataPoint.name(), v_SFDataPoint);

    SunSpecDataPoint cellV_SFDataPoint;
    cellV_SFDataPoint.setName("CellV_SF");
    cellV_SFDataPoint.setDescription("Scale factor for cell voltage.");
    cellV_SFDataPoint.setMandatory(true);
    cellV_SFDataPoint.setSize(1);
    cellV_SFDataPoint.setAddressOffset(42);
    cellV_SFDataPoint.setBlockOffset(40);
    cellV_SFDataPoint.setSunSpecDataType("sunssf");
    cellV_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(cellV_SFDataPoint.name(), cellV_SFDataPoint);

    SunSpecDataPoint tmp_SFDataPoint;
    tmp_SFDataPoint.setName("Tmp_SF");
    tmp_SFDataPoint.setDescription("Scale factor for module temperature.");
    tmp_SFDataPoint.setMandatory(true);
    tmp_SFDataPoint.setSize(1);
    tmp_SFDataPoint.setAddressOffset(43);
    tmp_SFDataPoint.setBlockOffset(41);
    tmp_SFDataPoint.setSunSpecDataType("sunssf");
    tmp_SFDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tmp_SFDataPoint.name(), tmp_SFDataPoint);

}

void SunSpecLithiumIonModuleModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("SoC_SF").isValid())
        m_soC_SF = m_dataPoints.value("SoC_SF").toInt16();

    if (m_dataPoints.value("SoH_SF").isValid())
        m_soH_SF = m_dataPoints.value("SoH_SF").toInt16();

    if (m_dataPoints.value("DoD_SF").isValid())
        m_doD_SF = m_dataPoints.value("DoD_SF").toInt16();

    if (m_dataPoints.value("V_SF").isValid())
        m_v_SF = m_dataPoints.value("V_SF").toInt16();

    if (m_dataPoints.value("CellV_SF").isValid())
        m_cellV_SF = m_dataPoints.value("CellV_SF").toInt16();

    if (m_dataPoints.value("Tmp_SF").isValid())
        m_tmp_SF = m_dataPoints.value("Tmp_SF").toInt16();


    // Update properties according to the data point type
    if (m_dataPoints.value("StrIdx").isValid())
        m_stringIndex = m_dataPoints.value("StrIdx").toUInt16();

    if (m_dataPoints.value("ModIdx").isValid())
        m_moduleIndex = m_dataPoints.value("ModIdx").toUInt16();

    if (m_dataPoints.value("NCell").isValid())
        m_moduleCellCount = m_dataPoints.value("NCell").toUInt16();

    if (m_dataPoints.value("SoC").isValid())
        m_moduleSoC = m_dataPoints.value("SoC").toFloatWithSSF(m_soC_SF);

    if (m_dataPoints.value("DoD").isValid())
        m_depthOfDischarge = m_dataPoints.value("DoD").toFloatWithSSF(m_doD_SF);

    if (m_dataPoints.value("SoH").isValid())
        m_moduleSoH = m_dataPoints.value("SoH").toFloatWithSSF(m_soH_SF);

    if (m_dataPoints.value("NCyc").isValid())
        m_cycleCount = m_dataPoints.value("NCyc").toUInt32();

    if (m_dataPoints.value("V").isValid())
        m_moduleVoltage = m_dataPoints.value("V").toFloatWithSSF(m_v_SF);

    if (m_dataPoints.value("CellVMax").isValid())
        m_maxCellVoltage = m_dataPoints.value("CellVMax").toFloatWithSSF(m_cellV_SF);

    if (m_dataPoints.value("CellVMaxCell").isValid())
        m_maxCellVoltageCell = m_dataPoints.value("CellVMaxCell").toUInt16();

    if (m_dataPoints.value("CellVMin").isValid())
        m_minCellVoltage = m_dataPoints.value("CellVMin").toFloatWithSSF(m_cellV_SF);

    if (m_dataPoints.value("CellVMinCell").isValid())
        m_minCellVoltageCell = m_dataPoints.value("CellVMinCell").toUInt16();

    if (m_dataPoints.value("CellVAvg").isValid())
        m_averageCellVoltage = m_dataPoints.value("CellVAvg").toFloatWithSSF(m_cellV_SF);

    if (m_dataPoints.value("CellTmpMax").isValid())
        m_maxCellTemperature = m_dataPoints.value("CellTmpMax").toFloatWithSSF(m_tmp_SF);

    if (m_dataPoints.value("CellTmpMaxCell").isValid())
        m_maxCellTemperatureCell = m_dataPoints.value("CellTmpMaxCell").toUInt16();

    if (m_dataPoints.value("CellTmpMin").isValid())
        m_minCellTemperature = m_dataPoints.value("CellTmpMin").toFloatWithSSF(m_tmp_SF);

    if (m_dataPoints.value("CellTmpMinCell").isValid())
        m_minCellTemperatureCell = m_dataPoints.value("CellTmpMinCell").toUInt16();

    if (m_dataPoints.value("CellTmpAvg").isValid())
        m_averageCellTemperature = m_dataPoints.value("CellTmpAvg").toFloatWithSSF(m_tmp_SF);

    if (m_dataPoints.value("NCellBal").isValid())
        m_balancedCellCount = m_dataPoints.value("NCellBal").toUInt16();

    if (m_dataPoints.value("SN").isValid())
        m_serialNumber = m_dataPoints.value("SN").toString();

    if (m_dataPoints.value("SoC_SF").isValid())
        m_soC_SF = m_dataPoints.value("SoC_SF").toInt16();

    if (m_dataPoints.value("SoH_SF").isValid())
        m_soH_SF = m_dataPoints.value("SoH_SF").toInt16();

    if (m_dataPoints.value("DoD_SF").isValid())
        m_doD_SF = m_dataPoints.value("DoD_SF").toInt16();

    if (m_dataPoints.value("V_SF").isValid())
        m_v_SF = m_dataPoints.value("V_SF").toInt16();

    if (m_dataPoints.value("CellV_SF").isValid())
        m_cellV_SF = m_dataPoints.value("CellV_SF").toInt16();

    if (m_dataPoints.value("Tmp_SF").isValid())
        m_tmp_SF = m_dataPoints.value("Tmp_SF").toInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

void SunSpecLithiumIonModuleModel::setupRepeatingBlocks()
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
        const auto block = new SunSpecLithiumIonModuleModelRepeatingBlock(i, m_repeatingBlockLength, blockStartRegister, this);
        m_repeatingBlocks.append(block);
    }
}

QDebug operator<<(QDebug debug, SunSpecLithiumIonModuleModel *model)
{
    debug.nospace().noquote() << "SunSpecLithiumIonModuleModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("StrIdx") << "-->";
    if (model->dataPoints().value("StrIdx").isValid()) {
        debug.nospace().noquote() << model->stringIndex() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ModIdx") << "-->";
    if (model->dataPoints().value("ModIdx").isValid()) {
        debug.nospace().noquote() << model->moduleIndex() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("NCell") << "-->";
    if (model->dataPoints().value("NCell").isValid()) {
        debug.nospace().noquote() << model->moduleCellCount() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("SoC") << "-->";
    if (model->dataPoints().value("SoC").isValid()) {
        debug.nospace().noquote() << model->moduleSoC() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("DoD") << "-->";
    if (model->dataPoints().value("DoD").isValid()) {
        debug.nospace().noquote() << model->depthOfDischarge() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("SoH") << "-->";
    if (model->dataPoints().value("SoH").isValid()) {
        debug.nospace().noquote() << model->moduleSoH() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("NCyc") << "-->";
    if (model->dataPoints().value("NCyc").isValid()) {
        debug.nospace().noquote() << model->cycleCount() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("V") << "-->";
    if (model->dataPoints().value("V").isValid()) {
        debug.nospace().noquote() << model->moduleVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVMax") << "-->";
    if (model->dataPoints().value("CellVMax").isValid()) {
        debug.nospace().noquote() << model->maxCellVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVMaxCell") << "-->";
    if (model->dataPoints().value("CellVMaxCell").isValid()) {
        debug.nospace().noquote() << model->maxCellVoltageCell() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVMin") << "-->";
    if (model->dataPoints().value("CellVMin").isValid()) {
        debug.nospace().noquote() << model->minCellVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVMinCell") << "-->";
    if (model->dataPoints().value("CellVMinCell").isValid()) {
        debug.nospace().noquote() << model->minCellVoltageCell() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellVAvg") << "-->";
    if (model->dataPoints().value("CellVAvg").isValid()) {
        debug.nospace().noquote() << model->averageCellVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellTmpMax") << "-->";
    if (model->dataPoints().value("CellTmpMax").isValid()) {
        debug.nospace().noquote() << model->maxCellTemperature() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellTmpMaxCell") << "-->";
    if (model->dataPoints().value("CellTmpMaxCell").isValid()) {
        debug.nospace().noquote() << model->maxCellTemperatureCell() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellTmpMin") << "-->";
    if (model->dataPoints().value("CellTmpMin").isValid()) {
        debug.nospace().noquote() << model->minCellTemperature() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellTmpMinCell") << "-->";
    if (model->dataPoints().value("CellTmpMinCell").isValid()) {
        debug.nospace().noquote() << model->minCellTemperatureCell() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("CellTmpAvg") << "-->";
    if (model->dataPoints().value("CellTmpAvg").isValid()) {
        debug.nospace().noquote() << model->averageCellTemperature() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("NCellBal") << "-->";
    if (model->dataPoints().value("NCellBal").isValid()) {
        debug.nospace().noquote() << model->balancedCellCount() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("SN") << "-->";
    if (model->dataPoints().value("SN").isValid()) {
        debug.nospace().noquote() << model->serialNumber() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
