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
#include "sunspecconnection.h"

SunSpecLithiumIonBankModel::SunSpecLithiumIonBankModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 803, 26, parent)
{
    //Q_ASSERT_X(length == 26,  "SunSpecLithiumIonBankModel", QString("model length %1 given in the constructor does not match the model length from the specs %2.").arg(length).arg(modelLength()).toLatin1());
    Q_UNUSED(length)
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

quint16 SunSpecLithiumIonBankModel::stringCount() const
{
    return m_stringCount;
}
quint16 SunSpecLithiumIonBankModel::connectedStringCount() const
{
    return m_connectedStringCount;
}
float SunSpecLithiumIonBankModel::maxModuleTemperature() const
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
float SunSpecLithiumIonBankModel::minModuleTemperature() const
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
float SunSpecLithiumIonBankModel::maxStringCurrent() const
{
    return m_maxStringCurrent;
}
quint16 SunSpecLithiumIonBankModel::maxStringCurrentString() const
{
    return m_maxStringCurrentString;
}
float SunSpecLithiumIonBankModel::minStringCurrent() const
{
    return m_minStringCurrent;
}
quint16 SunSpecLithiumIonBankModel::minStringCurrentString() const
{
    return m_minStringCurrentString;
}
float SunSpecLithiumIonBankModel::averageStringCurrent() const
{
    return m_averageStringCurrent;
}
quint16 SunSpecLithiumIonBankModel::batteryCellBalancingCount() const
{
    return m_batteryCellBalancingCount;
}
void SunSpecLithiumIonBankModel::processBlockData()
{
    // Scale factors
    m_cellV_SF = m_dataPoints.value("CellV_SF").toInt16();
    m_modTmp_SF = m_dataPoints.value("ModTmp_SF").toInt16();
    m_a_SF = m_dataPoints.value("A_SF").toInt16();
    m_soH_SF = m_dataPoints.value("SoH_SF").toInt16();
    m_soC_SF = m_dataPoints.value("SoC_SF").toInt16();
    m_v_SF = m_dataPoints.value("V_SF").toInt16();

    // Update properties according to the data point type
    m_stringCount = m_dataPoints.value("NStr").toUInt16();
    m_connectedStringCount = m_dataPoints.value("NStrCon").toUInt16();
    m_maxModuleTemperature = m_dataPoints.value("ModTmpMax").toFloatWithSSF(m_modTmp_SF);
    m_maxModuleTemperatureString = m_dataPoints.value("ModTmpMaxStr").toUInt16();
    m_maxModuleTemperatureModule = m_dataPoints.value("ModTmpMaxMod").toUInt16();
    m_minModuleTemperature = m_dataPoints.value("ModTmpMin").toFloatWithSSF(m_modTmp_SF);
    m_minModuleTemperatureString = m_dataPoints.value("ModTmpMinStr").toUInt16();
    m_minModuleTemperatureModule = m_dataPoints.value("ModTmpMinMod").toUInt16();
    m_averageModuleTemperature = m_dataPoints.value("ModTmpAvg").toInt16();
    m_maxStringVoltage = m_dataPoints.value("StrVMax").toFloatWithSSF(m_v_SF);
    m_maxStringVoltageString = m_dataPoints.value("StrVMaxStr").toUInt16();
    m_minStringVoltage = m_dataPoints.value("StrVMin").toFloatWithSSF(m_v_SF);
    m_minStringVoltageString = m_dataPoints.value("StrVMinStr").toUInt16();
    m_averageStringVoltage = m_dataPoints.value("StrVAvg").toFloatWithSSF(m_v_SF);
    m_maxStringCurrent = m_dataPoints.value("StrAMax").toFloatWithSSF(m_a_SF);
    m_maxStringCurrentString = m_dataPoints.value("StrAMaxStr").toUInt16();
    m_minStringCurrent = m_dataPoints.value("StrAMin").toFloatWithSSF(m_a_SF);
    m_minStringCurrentString = m_dataPoints.value("StrAMinStr").toUInt16();
    m_averageStringCurrent = m_dataPoints.value("StrAAvg").toFloatWithSSF(m_a_SF);
    m_batteryCellBalancingCount = m_dataPoints.value("NCellBal").toUInt16();

    qCDebug(dcSunSpec()) << this;
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
    modelIdDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(modelIdDataPoint.name(), modelIdDataPoint);

    SunSpecDataPoint modelLengthDataPoint;
    modelLengthDataPoint.setName("L");
    modelLengthDataPoint.setLabel("Model Length");
    modelLengthDataPoint.setDescription("Model length");
    modelLengthDataPoint.setMandatory(true);
    modelLengthDataPoint.setSize(1);
    modelLengthDataPoint.setAddressOffset(1);
    modelLengthDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(modelLengthDataPoint.name(), modelLengthDataPoint);

    SunSpecDataPoint stringCountDataPoint;
    stringCountDataPoint.setName("NStr");
    stringCountDataPoint.setLabel("String Count");
    stringCountDataPoint.setDescription("Number of strings in the bank.");
    stringCountDataPoint.setMandatory(true);
    stringCountDataPoint.setSize(1);
    stringCountDataPoint.setAddressOffset(2);
    stringCountDataPoint.setBlockOffset(0);
    stringCountDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(stringCountDataPoint.name(), stringCountDataPoint);

    SunSpecDataPoint connectedStringCountDataPoint;
    connectedStringCountDataPoint.setName("NStrCon");
    connectedStringCountDataPoint.setLabel("Connected String Count");
    connectedStringCountDataPoint.setDescription("Number of strings with contactor closed.");
    connectedStringCountDataPoint.setMandatory(true);
    connectedStringCountDataPoint.setSize(1);
    connectedStringCountDataPoint.setAddressOffset(3);
    connectedStringCountDataPoint.setBlockOffset(1);
    connectedStringCountDataPoint.setSunSpecDataType("uint16");
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
    maxModuleTemperatureDataPoint.setSunSpecDataType("int16");
    m_dataPoints.insert(maxModuleTemperatureDataPoint.name(), maxModuleTemperatureDataPoint);

    SunSpecDataPoint maxModuleTemperatureStringDataPoint;
    maxModuleTemperatureStringDataPoint.setName("ModTmpMaxStr");
    maxModuleTemperatureStringDataPoint.setLabel("Max Module Temperature String");
    maxModuleTemperatureStringDataPoint.setDescription("String containing the module with maximum temperature.");
    maxModuleTemperatureStringDataPoint.setSize(1);
    maxModuleTemperatureStringDataPoint.setAddressOffset(5);
    maxModuleTemperatureStringDataPoint.setBlockOffset(3);
    maxModuleTemperatureStringDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(maxModuleTemperatureStringDataPoint.name(), maxModuleTemperatureStringDataPoint);

    SunSpecDataPoint maxModuleTemperatureModuleDataPoint;
    maxModuleTemperatureModuleDataPoint.setName("ModTmpMaxMod");
    maxModuleTemperatureModuleDataPoint.setLabel("Max Module Temperature Module");
    maxModuleTemperatureModuleDataPoint.setDescription("Module with maximum temperature.");
    maxModuleTemperatureModuleDataPoint.setSize(1);
    maxModuleTemperatureModuleDataPoint.setAddressOffset(6);
    maxModuleTemperatureModuleDataPoint.setBlockOffset(4);
    maxModuleTemperatureModuleDataPoint.setSunSpecDataType("uint16");
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
    minModuleTemperatureDataPoint.setSunSpecDataType("int16");
    m_dataPoints.insert(minModuleTemperatureDataPoint.name(), minModuleTemperatureDataPoint);

    SunSpecDataPoint minModuleTemperatureStringDataPoint;
    minModuleTemperatureStringDataPoint.setName("ModTmpMinStr");
    minModuleTemperatureStringDataPoint.setLabel("Min Module Temperature String");
    minModuleTemperatureStringDataPoint.setDescription("String containing the module with minimum temperature.");
    minModuleTemperatureStringDataPoint.setSize(1);
    minModuleTemperatureStringDataPoint.setAddressOffset(8);
    minModuleTemperatureStringDataPoint.setBlockOffset(6);
    minModuleTemperatureStringDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(minModuleTemperatureStringDataPoint.name(), minModuleTemperatureStringDataPoint);

    SunSpecDataPoint minModuleTemperatureModuleDataPoint;
    minModuleTemperatureModuleDataPoint.setName("ModTmpMinMod");
    minModuleTemperatureModuleDataPoint.setLabel("Min Module Temperature Module");
    minModuleTemperatureModuleDataPoint.setDescription("Module with minimum temperature.");
    minModuleTemperatureModuleDataPoint.setSize(1);
    minModuleTemperatureModuleDataPoint.setAddressOffset(9);
    minModuleTemperatureModuleDataPoint.setBlockOffset(7);
    minModuleTemperatureModuleDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(minModuleTemperatureModuleDataPoint.name(), minModuleTemperatureModuleDataPoint);

    SunSpecDataPoint averageModuleTemperatureDataPoint;
    averageModuleTemperatureDataPoint.setName("ModTmpAvg");
    averageModuleTemperatureDataPoint.setLabel("Average Module Temperature");
    averageModuleTemperatureDataPoint.setDescription("Average temperature for all modules in the bank.");
    averageModuleTemperatureDataPoint.setSize(1);
    averageModuleTemperatureDataPoint.setAddressOffset(10);
    averageModuleTemperatureDataPoint.setBlockOffset(8);
    averageModuleTemperatureDataPoint.setSunSpecDataType("int16");
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
    maxStringVoltageDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(maxStringVoltageDataPoint.name(), maxStringVoltageDataPoint);

    SunSpecDataPoint maxStringVoltageStringDataPoint;
    maxStringVoltageStringDataPoint.setName("StrVMaxStr");
    maxStringVoltageStringDataPoint.setLabel("Max String Voltage String");
    maxStringVoltageStringDataPoint.setDescription("String with maximum voltage.");
    maxStringVoltageStringDataPoint.setSize(1);
    maxStringVoltageStringDataPoint.setAddressOffset(12);
    maxStringVoltageStringDataPoint.setBlockOffset(10);
    maxStringVoltageStringDataPoint.setSunSpecDataType("uint16");
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
    minStringVoltageDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(minStringVoltageDataPoint.name(), minStringVoltageDataPoint);

    SunSpecDataPoint minStringVoltageStringDataPoint;
    minStringVoltageStringDataPoint.setName("StrVMinStr");
    minStringVoltageStringDataPoint.setLabel("Min String Voltage String");
    minStringVoltageStringDataPoint.setDescription("String with minimum voltage.");
    minStringVoltageStringDataPoint.setSize(1);
    minStringVoltageStringDataPoint.setAddressOffset(14);
    minStringVoltageStringDataPoint.setBlockOffset(12);
    minStringVoltageStringDataPoint.setSunSpecDataType("uint16");
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
    averageStringVoltageDataPoint.setSunSpecDataType("uint16");
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
    maxStringCurrentDataPoint.setSunSpecDataType("int16");
    m_dataPoints.insert(maxStringCurrentDataPoint.name(), maxStringCurrentDataPoint);

    SunSpecDataPoint maxStringCurrentStringDataPoint;
    maxStringCurrentStringDataPoint.setName("StrAMaxStr");
    maxStringCurrentStringDataPoint.setLabel("Max String Current String");
    maxStringCurrentStringDataPoint.setDescription("String with the maximum current.");
    maxStringCurrentStringDataPoint.setSize(1);
    maxStringCurrentStringDataPoint.setAddressOffset(17);
    maxStringCurrentStringDataPoint.setBlockOffset(15);
    maxStringCurrentStringDataPoint.setSunSpecDataType("uint16");
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
    minStringCurrentDataPoint.setSunSpecDataType("int16");
    m_dataPoints.insert(minStringCurrentDataPoint.name(), minStringCurrentDataPoint);

    SunSpecDataPoint minStringCurrentStringDataPoint;
    minStringCurrentStringDataPoint.setName("StrAMinStr");
    minStringCurrentStringDataPoint.setLabel("Min String Current String");
    minStringCurrentStringDataPoint.setDescription("String with the minimum current.");
    minStringCurrentStringDataPoint.setSize(1);
    minStringCurrentStringDataPoint.setAddressOffset(19);
    minStringCurrentStringDataPoint.setBlockOffset(17);
    minStringCurrentStringDataPoint.setSunSpecDataType("uint16");
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
    averageStringCurrentDataPoint.setSunSpecDataType("int16");
    m_dataPoints.insert(averageStringCurrentDataPoint.name(), averageStringCurrentDataPoint);

    SunSpecDataPoint batteryCellBalancingCountDataPoint;
    batteryCellBalancingCountDataPoint.setName("NCellBal");
    batteryCellBalancingCountDataPoint.setLabel("Battery Cell Balancing Count");
    batteryCellBalancingCountDataPoint.setDescription("Total number of cells that are currently being balanced.");
    batteryCellBalancingCountDataPoint.setSize(1);
    batteryCellBalancingCountDataPoint.setAddressOffset(21);
    batteryCellBalancingCountDataPoint.setBlockOffset(19);
    batteryCellBalancingCountDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(batteryCellBalancingCountDataPoint.name(), batteryCellBalancingCountDataPoint);

    SunSpecDataPoint cellV_SFDataPoint;
    cellV_SFDataPoint.setName("CellV_SF");
    cellV_SFDataPoint.setDescription("Scale factor for cell voltage.");
    cellV_SFDataPoint.setMandatory(true);
    cellV_SFDataPoint.setSize(1);
    cellV_SFDataPoint.setAddressOffset(22);
    cellV_SFDataPoint.setBlockOffset(20);
    cellV_SFDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(cellV_SFDataPoint.name(), cellV_SFDataPoint);

    SunSpecDataPoint modTmp_SFDataPoint;
    modTmp_SFDataPoint.setName("ModTmp_SF");
    modTmp_SFDataPoint.setDescription("Scale factor for module temperatures.");
    modTmp_SFDataPoint.setMandatory(true);
    modTmp_SFDataPoint.setSize(1);
    modTmp_SFDataPoint.setAddressOffset(23);
    modTmp_SFDataPoint.setBlockOffset(21);
    modTmp_SFDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(modTmp_SFDataPoint.name(), modTmp_SFDataPoint);

    SunSpecDataPoint a_SFDataPoint;
    a_SFDataPoint.setName("A_SF");
    a_SFDataPoint.setDescription("Scale factor for string currents.");
    a_SFDataPoint.setMandatory(true);
    a_SFDataPoint.setSize(1);
    a_SFDataPoint.setAddressOffset(24);
    a_SFDataPoint.setBlockOffset(22);
    a_SFDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(a_SFDataPoint.name(), a_SFDataPoint);

    SunSpecDataPoint soH_SFDataPoint;
    soH_SFDataPoint.setName("SoH_SF");
    soH_SFDataPoint.setDescription("Scale factor for string state of health.");
    soH_SFDataPoint.setSize(1);
    soH_SFDataPoint.setAddressOffset(25);
    soH_SFDataPoint.setBlockOffset(23);
    soH_SFDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(soH_SFDataPoint.name(), soH_SFDataPoint);

    SunSpecDataPoint soC_SFDataPoint;
    soC_SFDataPoint.setName("SoC_SF");
    soC_SFDataPoint.setDescription("Scale factor for string state of charge.");
    soC_SFDataPoint.setMandatory(true);
    soC_SFDataPoint.setSize(1);
    soC_SFDataPoint.setAddressOffset(26);
    soC_SFDataPoint.setBlockOffset(24);
    soC_SFDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(soC_SFDataPoint.name(), soC_SFDataPoint);

    SunSpecDataPoint v_SFDataPoint;
    v_SFDataPoint.setName("V_SF");
    v_SFDataPoint.setDescription("Scale factor for string voltage.");
    v_SFDataPoint.setSize(1);
    v_SFDataPoint.setAddressOffset(27);
    v_SFDataPoint.setBlockOffset(25);
    v_SFDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(v_SFDataPoint.name(), v_SFDataPoint);

}

QDebug operator<<(QDebug debug, SunSpecLithiumIonBankModel *model)
{
    debug.nospace().noquote() << "SunSpecLithiumIonBankModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")" << endl;
    if (model->dataPoints().value("NStr").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("NStr") << "--> " << model->stringCount() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("NStr") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("NStrCon").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("NStrCon") << "--> " << model->connectedStringCount() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("NStrCon") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("ModTmpMax").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMax") << "--> " << model->maxModuleTemperature() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMax") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("ModTmpMaxStr").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMaxStr") << "--> " << model->maxModuleTemperatureString() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMaxStr") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("ModTmpMaxMod").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMaxMod") << "--> " << model->maxModuleTemperatureModule() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMaxMod") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("ModTmpMin").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMin") << "--> " << model->minModuleTemperature() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMin") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("ModTmpMinStr").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMinStr") << "--> " << model->minModuleTemperatureString() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMinStr") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("ModTmpMinMod").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMinMod") << "--> " << model->minModuleTemperatureModule() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpMinMod") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("ModTmpAvg").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpAvg") << "--> " << model->averageModuleTemperature() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("ModTmpAvg") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("StrVMax").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StrVMax") << "--> " << model->maxStringVoltage() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StrVMax") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("StrVMaxStr").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StrVMaxStr") << "--> " << model->maxStringVoltageString() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StrVMaxStr") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("StrVMin").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StrVMin") << "--> " << model->minStringVoltage() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StrVMin") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("StrVMinStr").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StrVMinStr") << "--> " << model->minStringVoltageString() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StrVMinStr") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("StrVAvg").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StrVAvg") << "--> " << model->averageStringVoltage() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StrVAvg") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("StrAMax").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StrAMax") << "--> " << model->maxStringCurrent() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StrAMax") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("StrAMaxStr").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StrAMaxStr") << "--> " << model->maxStringCurrentString() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StrAMaxStr") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("StrAMin").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StrAMin") << "--> " << model->minStringCurrent() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StrAMin") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("StrAMinStr").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StrAMinStr") << "--> " << model->minStringCurrentString() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StrAMinStr") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("StrAAvg").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StrAAvg") << "--> " << model->averageStringCurrent() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("StrAAvg") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("NCellBal").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("NCellBal") << "--> " << model->batteryCellBalancingCount() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("NCellBal") << "--> NaN" << endl;
    }


    return debug.space().quote();
}
