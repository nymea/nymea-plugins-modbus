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
    m_supportedModelIds << 803;
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

quint16 SunSpecLithiumIonBankModel::modelId() const
{
    return m_modelId;
}
quint16 SunSpecLithiumIonBankModel::modelLength() const
{
    return m_modelLength;
}
quint16 SunSpecLithiumIonBankModel::stringCount() const
{
    return m_stringCount;
}
quint16 SunSpecLithiumIonBankModel::connectedStringCount() const
{
    return m_connectedStringCount;
}
qint16 SunSpecLithiumIonBankModel::maxModuleTemperature() const
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
qint16 SunSpecLithiumIonBankModel::minModuleTemperature() const
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
qint16 SunSpecLithiumIonBankModel::maxStringCurrent() const
{
    return m_maxStringCurrent;
}
quint16 SunSpecLithiumIonBankModel::maxStringCurrentString() const
{
    return m_maxStringCurrentString;
}
qint16 SunSpecLithiumIonBankModel::minStringCurrent() const
{
    return m_minStringCurrent;
}
quint16 SunSpecLithiumIonBankModel::minStringCurrentString() const
{
    return m_minStringCurrentString;
}
qint16 SunSpecLithiumIonBankModel::averageStringCurrent() const
{
    return m_averageStringCurrent;
}
quint16 SunSpecLithiumIonBankModel::batteryCellBalancingCount() const
{
    return m_batteryCellBalancingCount;
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

    SunSpecDataPoint stringCountDataPoint;
    stringCountDataPoint.setName("NStr");
    stringCountDataPoint.setLabel("String Count");
    stringCountDataPoint.setDescription("Number of strings in the bank.");
    stringCountDataPoint.setMandatory(true);
    stringCountDataPoint.setSize(1);
    stringCountDataPoint.setAddressOffset(2);
    stringCountDataPoint.setBlockOffset(0);
    stringCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(stringCountDataPoint.name(), stringCountDataPoint);

    SunSpecDataPoint connectedStringCountDataPoint;
    connectedStringCountDataPoint.setName("NStrCon");
    connectedStringCountDataPoint.setLabel("Connected String Count");
    connectedStringCountDataPoint.setDescription("Number of strings with contactor closed.");
    connectedStringCountDataPoint.setMandatory(true);
    connectedStringCountDataPoint.setSize(1);
    connectedStringCountDataPoint.setAddressOffset(3);
    connectedStringCountDataPoint.setBlockOffset(1);
    connectedStringCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    maxModuleTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints.insert(maxModuleTemperatureDataPoint.name(), maxModuleTemperatureDataPoint);

    SunSpecDataPoint maxModuleTemperatureStringDataPoint;
    maxModuleTemperatureStringDataPoint.setName("ModTmpMaxStr");
    maxModuleTemperatureStringDataPoint.setLabel("Max Module Temperature String");
    maxModuleTemperatureStringDataPoint.setDescription("String containing the module with maximum temperature.");
    maxModuleTemperatureStringDataPoint.setSize(1);
    maxModuleTemperatureStringDataPoint.setAddressOffset(5);
    maxModuleTemperatureStringDataPoint.setBlockOffset(3);
    maxModuleTemperatureStringDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(maxModuleTemperatureStringDataPoint.name(), maxModuleTemperatureStringDataPoint);

    SunSpecDataPoint maxModuleTemperatureModuleDataPoint;
    maxModuleTemperatureModuleDataPoint.setName("ModTmpMaxMod");
    maxModuleTemperatureModuleDataPoint.setLabel("Max Module Temperature Module");
    maxModuleTemperatureModuleDataPoint.setDescription("Module with maximum temperature.");
    maxModuleTemperatureModuleDataPoint.setSize(1);
    maxModuleTemperatureModuleDataPoint.setAddressOffset(6);
    maxModuleTemperatureModuleDataPoint.setBlockOffset(4);
    maxModuleTemperatureModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    minModuleTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints.insert(minModuleTemperatureDataPoint.name(), minModuleTemperatureDataPoint);

    SunSpecDataPoint minModuleTemperatureStringDataPoint;
    minModuleTemperatureStringDataPoint.setName("ModTmpMinStr");
    minModuleTemperatureStringDataPoint.setLabel("Min Module Temperature String");
    minModuleTemperatureStringDataPoint.setDescription("String containing the module with minimum temperature.");
    minModuleTemperatureStringDataPoint.setSize(1);
    minModuleTemperatureStringDataPoint.setAddressOffset(8);
    minModuleTemperatureStringDataPoint.setBlockOffset(6);
    minModuleTemperatureStringDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(minModuleTemperatureStringDataPoint.name(), minModuleTemperatureStringDataPoint);

    SunSpecDataPoint minModuleTemperatureModuleDataPoint;
    minModuleTemperatureModuleDataPoint.setName("ModTmpMinMod");
    minModuleTemperatureModuleDataPoint.setLabel("Min Module Temperature Module");
    minModuleTemperatureModuleDataPoint.setDescription("Module with minimum temperature.");
    minModuleTemperatureModuleDataPoint.setSize(1);
    minModuleTemperatureModuleDataPoint.setAddressOffset(9);
    minModuleTemperatureModuleDataPoint.setBlockOffset(7);
    minModuleTemperatureModuleDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(minModuleTemperatureModuleDataPoint.name(), minModuleTemperatureModuleDataPoint);

    SunSpecDataPoint averageModuleTemperatureDataPoint;
    averageModuleTemperatureDataPoint.setName("ModTmpAvg");
    averageModuleTemperatureDataPoint.setLabel("Average Module Temperature");
    averageModuleTemperatureDataPoint.setDescription("Average temperature for all modules in the bank.");
    averageModuleTemperatureDataPoint.setSize(1);
    averageModuleTemperatureDataPoint.setAddressOffset(10);
    averageModuleTemperatureDataPoint.setBlockOffset(8);
    averageModuleTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
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
    maxStringVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(maxStringVoltageDataPoint.name(), maxStringVoltageDataPoint);

    SunSpecDataPoint maxStringVoltageStringDataPoint;
    maxStringVoltageStringDataPoint.setName("StrVMaxStr");
    maxStringVoltageStringDataPoint.setLabel("Max String Voltage String");
    maxStringVoltageStringDataPoint.setDescription("String with maximum voltage.");
    maxStringVoltageStringDataPoint.setSize(1);
    maxStringVoltageStringDataPoint.setAddressOffset(12);
    maxStringVoltageStringDataPoint.setBlockOffset(10);
    maxStringVoltageStringDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    minStringVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(minStringVoltageDataPoint.name(), minStringVoltageDataPoint);

    SunSpecDataPoint minStringVoltageStringDataPoint;
    minStringVoltageStringDataPoint.setName("StrVMinStr");
    minStringVoltageStringDataPoint.setLabel("Min String Voltage String");
    minStringVoltageStringDataPoint.setDescription("String with minimum voltage.");
    minStringVoltageStringDataPoint.setSize(1);
    minStringVoltageStringDataPoint.setAddressOffset(14);
    minStringVoltageStringDataPoint.setBlockOffset(12);
    minStringVoltageStringDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    averageStringVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    maxStringCurrentDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints.insert(maxStringCurrentDataPoint.name(), maxStringCurrentDataPoint);

    SunSpecDataPoint maxStringCurrentStringDataPoint;
    maxStringCurrentStringDataPoint.setName("StrAMaxStr");
    maxStringCurrentStringDataPoint.setLabel("Max String Current String");
    maxStringCurrentStringDataPoint.setDescription("String with the maximum current.");
    maxStringCurrentStringDataPoint.setSize(1);
    maxStringCurrentStringDataPoint.setAddressOffset(17);
    maxStringCurrentStringDataPoint.setBlockOffset(15);
    maxStringCurrentStringDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    minStringCurrentDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints.insert(minStringCurrentDataPoint.name(), minStringCurrentDataPoint);

    SunSpecDataPoint minStringCurrentStringDataPoint;
    minStringCurrentStringDataPoint.setName("StrAMinStr");
    minStringCurrentStringDataPoint.setLabel("Min String Current String");
    minStringCurrentStringDataPoint.setDescription("String with the minimum current.");
    minStringCurrentStringDataPoint.setSize(1);
    minStringCurrentStringDataPoint.setAddressOffset(19);
    minStringCurrentStringDataPoint.setBlockOffset(17);
    minStringCurrentStringDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
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
    averageStringCurrentDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints.insert(averageStringCurrentDataPoint.name(), averageStringCurrentDataPoint);

    SunSpecDataPoint batteryCellBalancingCountDataPoint;
    batteryCellBalancingCountDataPoint.setName("NCellBal");
    batteryCellBalancingCountDataPoint.setLabel("Battery Cell Balancing Count");
    batteryCellBalancingCountDataPoint.setDescription("Total number of cells that are currently being balanced.");
    batteryCellBalancingCountDataPoint.setSize(1);
    batteryCellBalancingCountDataPoint.setAddressOffset(21);
    batteryCellBalancingCountDataPoint.setBlockOffset(19);
    batteryCellBalancingCountDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(batteryCellBalancingCountDataPoint.name(), batteryCellBalancingCountDataPoint);

    SunSpecDataPoint cellV_SFDataPoint;
    cellV_SFDataPoint.setName("CellV_SF");
    cellV_SFDataPoint.setDescription("Scale factor for cell voltage.");
    cellV_SFDataPoint.setMandatory(true);
    cellV_SFDataPoint.setSize(1);
    cellV_SFDataPoint.setAddressOffset(22);
    cellV_SFDataPoint.setBlockOffset(20);
    cellV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(cellV_SFDataPoint.name(), cellV_SFDataPoint);

    SunSpecDataPoint modTmp_SFDataPoint;
    modTmp_SFDataPoint.setName("ModTmp_SF");
    modTmp_SFDataPoint.setDescription("Scale factor for module temperatures.");
    modTmp_SFDataPoint.setMandatory(true);
    modTmp_SFDataPoint.setSize(1);
    modTmp_SFDataPoint.setAddressOffset(23);
    modTmp_SFDataPoint.setBlockOffset(21);
    modTmp_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(modTmp_SFDataPoint.name(), modTmp_SFDataPoint);

    SunSpecDataPoint a_SFDataPoint;
    a_SFDataPoint.setName("A_SF");
    a_SFDataPoint.setDescription("Scale factor for string currents.");
    a_SFDataPoint.setMandatory(true);
    a_SFDataPoint.setSize(1);
    a_SFDataPoint.setAddressOffset(24);
    a_SFDataPoint.setBlockOffset(22);
    a_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(a_SFDataPoint.name(), a_SFDataPoint);

    SunSpecDataPoint soH_SFDataPoint;
    soH_SFDataPoint.setName("SoH_SF");
    soH_SFDataPoint.setDescription("Scale factor for string state of health.");
    soH_SFDataPoint.setSize(1);
    soH_SFDataPoint.setAddressOffset(25);
    soH_SFDataPoint.setBlockOffset(23);
    soH_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(soH_SFDataPoint.name(), soH_SFDataPoint);

    SunSpecDataPoint soC_SFDataPoint;
    soC_SFDataPoint.setName("SoC_SF");
    soC_SFDataPoint.setDescription("Scale factor for string state of charge.");
    soC_SFDataPoint.setMandatory(true);
    soC_SFDataPoint.setSize(1);
    soC_SFDataPoint.setAddressOffset(26);
    soC_SFDataPoint.setBlockOffset(24);
    soC_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(soC_SFDataPoint.name(), soC_SFDataPoint);

    SunSpecDataPoint v_SFDataPoint;
    v_SFDataPoint.setName("V_SF");
    v_SFDataPoint.setDescription("Scale factor for string voltage.");
    v_SFDataPoint.setSize(1);
    v_SFDataPoint.setAddressOffset(27);
    v_SFDataPoint.setBlockOffset(25);
    v_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(v_SFDataPoint.name(), v_SFDataPoint);

}

