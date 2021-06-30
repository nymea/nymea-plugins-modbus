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

#include "sunspeclithiumionbankmodel.h"

SunSpecLithiumIonBankModel::SunSpecLithiumIonBankModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
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

void SunSpecLithiumIonBankModel::readModelHeader()
{

}

void SunSpecLithiumIonBankModel::readBlockData()
{

}

void SunSpecLithiumIonBankModel::initDataPoints()
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

    SunSpecDataPoint stringCountDataPoint;
    stringCountDataPoint.setName("NStr");
    stringCountDataPoint.setLabel("String Count");
    stringCountDataPoint.setDescription("Number of strings in the bank.");
    stringCountDataPoint.setMandatory(true);
    stringCountDataPoint.setSize(1);
    stringCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << stringCountDataPoint;

    SunSpecDataPoint connectedStringCountDataPoint;
    connectedStringCountDataPoint.setName("NStrCon");
    connectedStringCountDataPoint.setLabel("Connected String Count");
    connectedStringCountDataPoint.setDescription("Number of strings with contactor closed.");
    connectedStringCountDataPoint.setMandatory(true);
    connectedStringCountDataPoint.setSize(1);
    connectedStringCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << connectedStringCountDataPoint;

    SunSpecDataPoint maxModuleTemperatureDataPoint;
    maxModuleTemperatureDataPoint.setName("ModTmpMax");
    maxModuleTemperatureDataPoint.setLabel("Max Module Temperature");
    maxModuleTemperatureDataPoint.setDescription("Maximum temperature for all modules in the bank.");
    maxModuleTemperatureDataPoint.setMandatory(true);
    maxModuleTemperatureDataPoint.setSize(1);
    maxModuleTemperatureDataPoint.setScaleFactorName("ModTmp_SF");
    maxModuleTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << maxModuleTemperatureDataPoint;

    SunSpecDataPoint maxModuleTemperatureStringDataPoint;
    maxModuleTemperatureStringDataPoint.setName("ModTmpMaxStr");
    maxModuleTemperatureStringDataPoint.setLabel("Max Module Temperature String");
    maxModuleTemperatureStringDataPoint.setDescription("String containing the module with maximum temperature.");
    maxModuleTemperatureStringDataPoint.setSize(1);
    maxModuleTemperatureStringDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << maxModuleTemperatureStringDataPoint;

    SunSpecDataPoint maxModuleTemperatureModuleDataPoint;
    maxModuleTemperatureModuleDataPoint.setName("ModTmpMaxMod");
    maxModuleTemperatureModuleDataPoint.setLabel("Max Module Temperature Module");
    maxModuleTemperatureModuleDataPoint.setDescription("Module with maximum temperature.");
    maxModuleTemperatureModuleDataPoint.setSize(1);
    maxModuleTemperatureModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << maxModuleTemperatureModuleDataPoint;

    SunSpecDataPoint minModuleTemperatureDataPoint;
    minModuleTemperatureDataPoint.setName("ModTmpMin");
    minModuleTemperatureDataPoint.setLabel("Min Module Temperature");
    minModuleTemperatureDataPoint.setDescription("Minimum temperature for all modules in the bank.");
    minModuleTemperatureDataPoint.setUnits("C");
    minModuleTemperatureDataPoint.setMandatory(true);
    minModuleTemperatureDataPoint.setSize(1);
    minModuleTemperatureDataPoint.setScaleFactorName("ModTmp_SF");
    minModuleTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << minModuleTemperatureDataPoint;

    SunSpecDataPoint minModuleTemperatureStringDataPoint;
    minModuleTemperatureStringDataPoint.setName("ModTmpMinStr");
    minModuleTemperatureStringDataPoint.setLabel("Min Module Temperature String");
    minModuleTemperatureStringDataPoint.setDescription("String containing the module with minimum temperature.");
    minModuleTemperatureStringDataPoint.setSize(1);
    minModuleTemperatureStringDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << minModuleTemperatureStringDataPoint;

    SunSpecDataPoint minModuleTemperatureModuleDataPoint;
    minModuleTemperatureModuleDataPoint.setName("ModTmpMinMod");
    minModuleTemperatureModuleDataPoint.setLabel("Min Module Temperature Module");
    minModuleTemperatureModuleDataPoint.setDescription("Module with minimum temperature.");
    minModuleTemperatureModuleDataPoint.setSize(1);
    minModuleTemperatureModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << minModuleTemperatureModuleDataPoint;

    SunSpecDataPoint averageModuleTemperatureDataPoint;
    averageModuleTemperatureDataPoint.setName("ModTmpAvg");
    averageModuleTemperatureDataPoint.setLabel("Average Module Temperature");
    averageModuleTemperatureDataPoint.setDescription("Average temperature for all modules in the bank.");
    averageModuleTemperatureDataPoint.setSize(1);
    averageModuleTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << averageModuleTemperatureDataPoint;

    SunSpecDataPoint maxStringVoltageDataPoint;
    maxStringVoltageDataPoint.setName("StrVMax");
    maxStringVoltageDataPoint.setLabel("Max String Voltage");
    maxStringVoltageDataPoint.setDescription("Maximum string voltage for all strings in the bank.");
    maxStringVoltageDataPoint.setUnits("V");
    maxStringVoltageDataPoint.setSize(1);
    maxStringVoltageDataPoint.setScaleFactorName("V_SF");
    maxStringVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << maxStringVoltageDataPoint;

    SunSpecDataPoint maxStringVoltageStringDataPoint;
    maxStringVoltageStringDataPoint.setName("StrVMaxStr");
    maxStringVoltageStringDataPoint.setLabel("Max String Voltage String");
    maxStringVoltageStringDataPoint.setDescription("String with maximum voltage.");
    maxStringVoltageStringDataPoint.setSize(1);
    maxStringVoltageStringDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << maxStringVoltageStringDataPoint;

    SunSpecDataPoint minStringVoltageDataPoint;
    minStringVoltageDataPoint.setName("StrVMin");
    minStringVoltageDataPoint.setLabel("Min String Voltage");
    minStringVoltageDataPoint.setDescription("Minimum string voltage for all strings in the bank.");
    minStringVoltageDataPoint.setUnits("V");
    minStringVoltageDataPoint.setSize(1);
    minStringVoltageDataPoint.setScaleFactorName("V_SF");
    minStringVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << minStringVoltageDataPoint;

    SunSpecDataPoint minStringVoltageStringDataPoint;
    minStringVoltageStringDataPoint.setName("StrVMinStr");
    minStringVoltageStringDataPoint.setLabel("Min String Voltage String");
    minStringVoltageStringDataPoint.setDescription("String with minimum voltage.");
    minStringVoltageStringDataPoint.setSize(1);
    minStringVoltageStringDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << minStringVoltageStringDataPoint;

    SunSpecDataPoint averageStringVoltageDataPoint;
    averageStringVoltageDataPoint.setName("StrVAvg");
    averageStringVoltageDataPoint.setLabel("Average String Voltage");
    averageStringVoltageDataPoint.setDescription("Average string voltage for all strings in the bank.");
    averageStringVoltageDataPoint.setUnits("V");
    averageStringVoltageDataPoint.setSize(1);
    averageStringVoltageDataPoint.setScaleFactorName("V_SF");
    averageStringVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << averageStringVoltageDataPoint;

    SunSpecDataPoint maxStringCurrentDataPoint;
    maxStringCurrentDataPoint.setName("StrAMax");
    maxStringCurrentDataPoint.setLabel("Max String Current");
    maxStringCurrentDataPoint.setDescription("Maximum current of any string in the bank.");
    maxStringCurrentDataPoint.setUnits("A");
    maxStringCurrentDataPoint.setSize(1);
    maxStringCurrentDataPoint.setScaleFactorName("A_SF");
    maxStringCurrentDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << maxStringCurrentDataPoint;

    SunSpecDataPoint maxStringCurrentStringDataPoint;
    maxStringCurrentStringDataPoint.setName("StrAMaxStr");
    maxStringCurrentStringDataPoint.setLabel("Max String Current String");
    maxStringCurrentStringDataPoint.setDescription("String with the maximum current.");
    maxStringCurrentStringDataPoint.setSize(1);
    maxStringCurrentStringDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << maxStringCurrentStringDataPoint;

    SunSpecDataPoint minStringCurrentDataPoint;
    minStringCurrentDataPoint.setName("StrAMin");
    minStringCurrentDataPoint.setLabel("Min String Current");
    minStringCurrentDataPoint.setDescription("Minimum current of any string in the bank.");
    minStringCurrentDataPoint.setUnits("A");
    minStringCurrentDataPoint.setSize(1);
    minStringCurrentDataPoint.setScaleFactorName("A_SF");
    minStringCurrentDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << minStringCurrentDataPoint;

    SunSpecDataPoint minStringCurrentStringDataPoint;
    minStringCurrentStringDataPoint.setName("StrAMinStr");
    minStringCurrentStringDataPoint.setLabel("Min String Current String");
    minStringCurrentStringDataPoint.setDescription("String with the minimum current.");
    minStringCurrentStringDataPoint.setSize(1);
    minStringCurrentStringDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << minStringCurrentStringDataPoint;

    SunSpecDataPoint averageStringCurrentDataPoint;
    averageStringCurrentDataPoint.setName("StrAAvg");
    averageStringCurrentDataPoint.setLabel("Average String Current");
    averageStringCurrentDataPoint.setDescription("Average string current for all strings in the bank.");
    averageStringCurrentDataPoint.setUnits("A");
    averageStringCurrentDataPoint.setSize(1);
    averageStringCurrentDataPoint.setScaleFactorName("A_SF");
    averageStringCurrentDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << averageStringCurrentDataPoint;

    SunSpecDataPoint batteryCellBalancingCountDataPoint;
    batteryCellBalancingCountDataPoint.setName("NCellBal");
    batteryCellBalancingCountDataPoint.setLabel("Battery Cell Balancing Count");
    batteryCellBalancingCountDataPoint.setDescription("Total number of cells that are currently being balanced.");
    batteryCellBalancingCountDataPoint.setSize(1);
    batteryCellBalancingCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << batteryCellBalancingCountDataPoint;

    SunSpecDataPoint CellV_SFDataPoint;
    CellV_SFDataPoint.setName("CellV_SF");
    CellV_SFDataPoint.setDescription("Scale factor for cell voltage.");
    CellV_SFDataPoint.setMandatory(true);
    CellV_SFDataPoint.setSize(1);
    CellV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << CellV_SFDataPoint;

    SunSpecDataPoint ModTmp_SFDataPoint;
    ModTmp_SFDataPoint.setName("ModTmp_SF");
    ModTmp_SFDataPoint.setDescription("Scale factor for module temperatures.");
    ModTmp_SFDataPoint.setMandatory(true);
    ModTmp_SFDataPoint.setSize(1);
    ModTmp_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << ModTmp_SFDataPoint;

    SunSpecDataPoint A_SFDataPoint;
    A_SFDataPoint.setName("A_SF");
    A_SFDataPoint.setDescription("Scale factor for string currents.");
    A_SFDataPoint.setMandatory(true);
    A_SFDataPoint.setSize(1);
    A_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << A_SFDataPoint;

    SunSpecDataPoint SoH_SFDataPoint;
    SoH_SFDataPoint.setName("SoH_SF");
    SoH_SFDataPoint.setDescription("Scale factor for string state of health.");
    SoH_SFDataPoint.setSize(1);
    SoH_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << SoH_SFDataPoint;

    SunSpecDataPoint SoC_SFDataPoint;
    SoC_SFDataPoint.setName("SoC_SF");
    SoC_SFDataPoint.setDescription("Scale factor for string state of charge.");
    SoC_SFDataPoint.setMandatory(true);
    SoC_SFDataPoint.setSize(1);
    SoC_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << SoC_SFDataPoint;

    SunSpecDataPoint V_SFDataPoint;
    V_SFDataPoint.setName("V_SF");
    V_SFDataPoint.setDescription("Scale factor for string voltage.");
    V_SFDataPoint.setSize(1);
    V_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << V_SFDataPoint;

}

