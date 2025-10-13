/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2025, nymea GmbH
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

#include "sunspecmetersinglephasefloatmodel.h"
#include "sunspecconnection.h"

SunSpecMeterSinglePhaseFloatModel::SunSpecMeterSinglePhaseFloatModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 211, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixed;

    initDataPoints();
}

SunSpecMeterSinglePhaseFloatModel::~SunSpecMeterSinglePhaseFloatModel()
{

}

QString SunSpecMeterSinglePhaseFloatModel::name() const
{
    return "ac_meter";
}

QString SunSpecMeterSinglePhaseFloatModel::description() const
{
    return QString();
}

QString SunSpecMeterSinglePhaseFloatModel::label() const
{
    return "Meter (Single Phase) FLOAT";
}

float SunSpecMeterSinglePhaseFloatModel::amps() const
{
    return m_amps;
}
float SunSpecMeterSinglePhaseFloatModel::ampsPhaseA() const
{
    return m_ampsPhaseA;
}
float SunSpecMeterSinglePhaseFloatModel::ampsPhaseB() const
{
    return m_ampsPhaseB;
}
float SunSpecMeterSinglePhaseFloatModel::ampsPhaseC() const
{
    return m_ampsPhaseC;
}
float SunSpecMeterSinglePhaseFloatModel::voltageLn() const
{
    return m_voltageLn;
}
float SunSpecMeterSinglePhaseFloatModel::phaseVoltageAn() const
{
    return m_phaseVoltageAn;
}
float SunSpecMeterSinglePhaseFloatModel::phaseVoltageBn() const
{
    return m_phaseVoltageBn;
}
float SunSpecMeterSinglePhaseFloatModel::phaseVoltageCn() const
{
    return m_phaseVoltageCn;
}
float SunSpecMeterSinglePhaseFloatModel::voltageLl() const
{
    return m_voltageLl;
}
float SunSpecMeterSinglePhaseFloatModel::phaseVoltageAb() const
{
    return m_phaseVoltageAb;
}
float SunSpecMeterSinglePhaseFloatModel::phaseVoltageBc() const
{
    return m_phaseVoltageBc;
}
float SunSpecMeterSinglePhaseFloatModel::phaseVoltageCa() const
{
    return m_phaseVoltageCa;
}
float SunSpecMeterSinglePhaseFloatModel::hz() const
{
    return m_hz;
}
float SunSpecMeterSinglePhaseFloatModel::watts() const
{
    return m_watts;
}
float SunSpecMeterSinglePhaseFloatModel::wattsPhaseA() const
{
    return m_wattsPhaseA;
}
float SunSpecMeterSinglePhaseFloatModel::wattsPhaseB() const
{
    return m_wattsPhaseB;
}
float SunSpecMeterSinglePhaseFloatModel::wattsPhaseC() const
{
    return m_wattsPhaseC;
}
float SunSpecMeterSinglePhaseFloatModel::va() const
{
    return m_va;
}
float SunSpecMeterSinglePhaseFloatModel::vaPhaseA() const
{
    return m_vaPhaseA;
}
float SunSpecMeterSinglePhaseFloatModel::vaPhaseB() const
{
    return m_vaPhaseB;
}
float SunSpecMeterSinglePhaseFloatModel::vaPhaseC() const
{
    return m_vaPhaseC;
}
float SunSpecMeterSinglePhaseFloatModel::var() const
{
    return m_var;
}
float SunSpecMeterSinglePhaseFloatModel::varPhaseA() const
{
    return m_varPhaseA;
}
float SunSpecMeterSinglePhaseFloatModel::varPhaseB() const
{
    return m_varPhaseB;
}
float SunSpecMeterSinglePhaseFloatModel::varPhaseC() const
{
    return m_varPhaseC;
}
float SunSpecMeterSinglePhaseFloatModel::pf() const
{
    return m_pf;
}
float SunSpecMeterSinglePhaseFloatModel::pfPhaseA() const
{
    return m_pfPhaseA;
}
float SunSpecMeterSinglePhaseFloatModel::pfPhaseB() const
{
    return m_pfPhaseB;
}
float SunSpecMeterSinglePhaseFloatModel::pfPhaseC() const
{
    return m_pfPhaseC;
}
float SunSpecMeterSinglePhaseFloatModel::totalWattHoursExported() const
{
    return m_totalWattHoursExported;
}
float SunSpecMeterSinglePhaseFloatModel::totalWattHoursExportedPhaseA() const
{
    return m_totalWattHoursExportedPhaseA;
}
float SunSpecMeterSinglePhaseFloatModel::totalWattHoursExportedPhaseB() const
{
    return m_totalWattHoursExportedPhaseB;
}
float SunSpecMeterSinglePhaseFloatModel::totalWattHoursExportedPhaseC() const
{
    return m_totalWattHoursExportedPhaseC;
}
float SunSpecMeterSinglePhaseFloatModel::totalWattHoursImported() const
{
    return m_totalWattHoursImported;
}
float SunSpecMeterSinglePhaseFloatModel::totalWattHoursImportedPhaseA() const
{
    return m_totalWattHoursImportedPhaseA;
}
float SunSpecMeterSinglePhaseFloatModel::totalWattHoursImportedPhaseB() const
{
    return m_totalWattHoursImportedPhaseB;
}
float SunSpecMeterSinglePhaseFloatModel::totalWattHoursImportedPhaseC() const
{
    return m_totalWattHoursImportedPhaseC;
}
float SunSpecMeterSinglePhaseFloatModel::totalVaHoursExported() const
{
    return m_totalVaHoursExported;
}
float SunSpecMeterSinglePhaseFloatModel::totalVaHoursExportedPhaseA() const
{
    return m_totalVaHoursExportedPhaseA;
}
float SunSpecMeterSinglePhaseFloatModel::totalVaHoursExportedPhaseB() const
{
    return m_totalVaHoursExportedPhaseB;
}
float SunSpecMeterSinglePhaseFloatModel::totalVaHoursExportedPhaseC() const
{
    return m_totalVaHoursExportedPhaseC;
}
float SunSpecMeterSinglePhaseFloatModel::totalVaHoursImported() const
{
    return m_totalVaHoursImported;
}
float SunSpecMeterSinglePhaseFloatModel::totalVaHoursImportedPhaseA() const
{
    return m_totalVaHoursImportedPhaseA;
}
float SunSpecMeterSinglePhaseFloatModel::totalVaHoursImportedPhaseB() const
{
    return m_totalVaHoursImportedPhaseB;
}
float SunSpecMeterSinglePhaseFloatModel::totalVaHoursImportedPhaseC() const
{
    return m_totalVaHoursImportedPhaseC;
}
float SunSpecMeterSinglePhaseFloatModel::totalVarHoursImportedQ1() const
{
    return m_totalVarHoursImportedQ1;
}
float SunSpecMeterSinglePhaseFloatModel::totalVArHoursImportedQ1PhaseA() const
{
    return m_totalVArHoursImportedQ1PhaseA;
}
float SunSpecMeterSinglePhaseFloatModel::totalVArHoursImportedQ1PhaseB() const
{
    return m_totalVArHoursImportedQ1PhaseB;
}
float SunSpecMeterSinglePhaseFloatModel::totalVArHoursImportedQ1PhaseC() const
{
    return m_totalVArHoursImportedQ1PhaseC;
}
float SunSpecMeterSinglePhaseFloatModel::totalVArHoursImportedQ2() const
{
    return m_totalVArHoursImportedQ2;
}
float SunSpecMeterSinglePhaseFloatModel::totalVArHoursImportedQ2PhaseA() const
{
    return m_totalVArHoursImportedQ2PhaseA;
}
float SunSpecMeterSinglePhaseFloatModel::totalVArHoursImportedQ2PhaseB() const
{
    return m_totalVArHoursImportedQ2PhaseB;
}
float SunSpecMeterSinglePhaseFloatModel::totalVArHoursImportedQ2PhaseC() const
{
    return m_totalVArHoursImportedQ2PhaseC;
}
float SunSpecMeterSinglePhaseFloatModel::totalVArHoursExportedQ3() const
{
    return m_totalVArHoursExportedQ3;
}
float SunSpecMeterSinglePhaseFloatModel::totalVArHoursExportedQ3PhaseA() const
{
    return m_totalVArHoursExportedQ3PhaseA;
}
float SunSpecMeterSinglePhaseFloatModel::totalVArHoursExportedQ3PhaseB() const
{
    return m_totalVArHoursExportedQ3PhaseB;
}
float SunSpecMeterSinglePhaseFloatModel::totalVArHoursExportedQ3PhaseC() const
{
    return m_totalVArHoursExportedQ3PhaseC;
}
float SunSpecMeterSinglePhaseFloatModel::totalVArHoursExportedQ4() const
{
    return m_totalVArHoursExportedQ4;
}
float SunSpecMeterSinglePhaseFloatModel::totalVArHoursExportedQ4ImportedPhaseA() const
{
    return m_totalVArHoursExportedQ4ImportedPhaseA;
}
float SunSpecMeterSinglePhaseFloatModel::totalVArHoursExportedQ4ImportedPhaseB() const
{
    return m_totalVArHoursExportedQ4ImportedPhaseB;
}
float SunSpecMeterSinglePhaseFloatModel::totalVArHoursExportedQ4ImportedPhaseC() const
{
    return m_totalVArHoursExportedQ4ImportedPhaseC;
}
SunSpecMeterSinglePhaseFloatModel::EvtFlags SunSpecMeterSinglePhaseFloatModel::events() const
{
    return m_events;
}
void SunSpecMeterSinglePhaseFloatModel::initDataPoints()
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

    SunSpecDataPoint ampsDataPoint;
    ampsDataPoint.setName("A");
    ampsDataPoint.setLabel("Amps");
    ampsDataPoint.setDescription("Total AC Current");
    ampsDataPoint.setUnits("A");
    ampsDataPoint.setMandatory(true);
    ampsDataPoint.setSize(2);
    ampsDataPoint.setAddressOffset(2);
    ampsDataPoint.setBlockOffset(0);
    ampsDataPoint.setSunSpecDataType("float32");
    ampsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(ampsDataPoint.name(), ampsDataPoint);

    SunSpecDataPoint ampsPhaseADataPoint;
    ampsPhaseADataPoint.setName("AphA");
    ampsPhaseADataPoint.setLabel("Amps PhaseA");
    ampsPhaseADataPoint.setDescription("Phase A Current");
    ampsPhaseADataPoint.setUnits("A");
    ampsPhaseADataPoint.setMandatory(true);
    ampsPhaseADataPoint.setSize(2);
    ampsPhaseADataPoint.setAddressOffset(4);
    ampsPhaseADataPoint.setBlockOffset(2);
    ampsPhaseADataPoint.setSunSpecDataType("float32");
    ampsPhaseADataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(ampsPhaseADataPoint.name(), ampsPhaseADataPoint);

    SunSpecDataPoint ampsPhaseBDataPoint;
    ampsPhaseBDataPoint.setName("AphB");
    ampsPhaseBDataPoint.setLabel("Amps PhaseB");
    ampsPhaseBDataPoint.setDescription("Phase B Current");
    ampsPhaseBDataPoint.setUnits("A");
    ampsPhaseBDataPoint.setSize(2);
    ampsPhaseBDataPoint.setAddressOffset(6);
    ampsPhaseBDataPoint.setBlockOffset(4);
    ampsPhaseBDataPoint.setSunSpecDataType("float32");
    ampsPhaseBDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(ampsPhaseBDataPoint.name(), ampsPhaseBDataPoint);

    SunSpecDataPoint ampsPhaseCDataPoint;
    ampsPhaseCDataPoint.setName("AphC");
    ampsPhaseCDataPoint.setLabel("Amps PhaseC");
    ampsPhaseCDataPoint.setDescription("Phase C Current");
    ampsPhaseCDataPoint.setUnits("A");
    ampsPhaseCDataPoint.setSize(2);
    ampsPhaseCDataPoint.setAddressOffset(8);
    ampsPhaseCDataPoint.setBlockOffset(6);
    ampsPhaseCDataPoint.setSunSpecDataType("float32");
    ampsPhaseCDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(ampsPhaseCDataPoint.name(), ampsPhaseCDataPoint);

    SunSpecDataPoint voltageLnDataPoint;
    voltageLnDataPoint.setName("PhV");
    voltageLnDataPoint.setLabel("Voltage LN");
    voltageLnDataPoint.setDescription("Line to Neutral AC Voltage (average of active phases)");
    voltageLnDataPoint.setUnits("V");
    voltageLnDataPoint.setSize(2);
    voltageLnDataPoint.setAddressOffset(10);
    voltageLnDataPoint.setBlockOffset(8);
    voltageLnDataPoint.setSunSpecDataType("float32");
    voltageLnDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(voltageLnDataPoint.name(), voltageLnDataPoint);

    SunSpecDataPoint phaseVoltageAnDataPoint;
    phaseVoltageAnDataPoint.setName("PhVphA");
    phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
    phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
    phaseVoltageAnDataPoint.setUnits("V");
    phaseVoltageAnDataPoint.setSize(2);
    phaseVoltageAnDataPoint.setAddressOffset(12);
    phaseVoltageAnDataPoint.setBlockOffset(10);
    phaseVoltageAnDataPoint.setSunSpecDataType("float32");
    phaseVoltageAnDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(phaseVoltageAnDataPoint.name(), phaseVoltageAnDataPoint);

    SunSpecDataPoint phaseVoltageBnDataPoint;
    phaseVoltageBnDataPoint.setName("PhVphB");
    phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
    phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
    phaseVoltageBnDataPoint.setUnits("V");
    phaseVoltageBnDataPoint.setSize(2);
    phaseVoltageBnDataPoint.setAddressOffset(14);
    phaseVoltageBnDataPoint.setBlockOffset(12);
    phaseVoltageBnDataPoint.setSunSpecDataType("float32");
    phaseVoltageBnDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(phaseVoltageBnDataPoint.name(), phaseVoltageBnDataPoint);

    SunSpecDataPoint phaseVoltageCnDataPoint;
    phaseVoltageCnDataPoint.setName("PhVphC");
    phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
    phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
    phaseVoltageCnDataPoint.setUnits("V");
    phaseVoltageCnDataPoint.setSize(2);
    phaseVoltageCnDataPoint.setAddressOffset(16);
    phaseVoltageCnDataPoint.setBlockOffset(14);
    phaseVoltageCnDataPoint.setSunSpecDataType("float32");
    phaseVoltageCnDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(phaseVoltageCnDataPoint.name(), phaseVoltageCnDataPoint);

    SunSpecDataPoint voltageLlDataPoint;
    voltageLlDataPoint.setName("PPV");
    voltageLlDataPoint.setLabel("Voltage LL");
    voltageLlDataPoint.setDescription("Line to Line AC Voltage (average of active phases)");
    voltageLlDataPoint.setUnits("V");
    voltageLlDataPoint.setSize(2);
    voltageLlDataPoint.setAddressOffset(18);
    voltageLlDataPoint.setBlockOffset(16);
    voltageLlDataPoint.setSunSpecDataType("float32");
    voltageLlDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(voltageLlDataPoint.name(), voltageLlDataPoint);

    SunSpecDataPoint phaseVoltageAbDataPoint;
    phaseVoltageAbDataPoint.setName("PPVphAB");
    phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
    phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
    phaseVoltageAbDataPoint.setUnits("V");
    phaseVoltageAbDataPoint.setSize(2);
    phaseVoltageAbDataPoint.setAddressOffset(20);
    phaseVoltageAbDataPoint.setBlockOffset(18);
    phaseVoltageAbDataPoint.setSunSpecDataType("float32");
    phaseVoltageAbDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(phaseVoltageAbDataPoint.name(), phaseVoltageAbDataPoint);

    SunSpecDataPoint phaseVoltageBcDataPoint;
    phaseVoltageBcDataPoint.setName("PPVphBC");
    phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
    phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
    phaseVoltageBcDataPoint.setUnits("V");
    phaseVoltageBcDataPoint.setSize(2);
    phaseVoltageBcDataPoint.setAddressOffset(22);
    phaseVoltageBcDataPoint.setBlockOffset(20);
    phaseVoltageBcDataPoint.setSunSpecDataType("float32");
    phaseVoltageBcDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(phaseVoltageBcDataPoint.name(), phaseVoltageBcDataPoint);

    SunSpecDataPoint phaseVoltageCaDataPoint;
    phaseVoltageCaDataPoint.setName("PPVphCA");
    phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
    phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
    phaseVoltageCaDataPoint.setUnits("V");
    phaseVoltageCaDataPoint.setSize(2);
    phaseVoltageCaDataPoint.setAddressOffset(24);
    phaseVoltageCaDataPoint.setBlockOffset(22);
    phaseVoltageCaDataPoint.setSunSpecDataType("float32");
    phaseVoltageCaDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(phaseVoltageCaDataPoint.name(), phaseVoltageCaDataPoint);

    SunSpecDataPoint hzDataPoint;
    hzDataPoint.setName("Hz");
    hzDataPoint.setLabel("Hz");
    hzDataPoint.setDescription("Frequency");
    hzDataPoint.setUnits("Hz");
    hzDataPoint.setMandatory(true);
    hzDataPoint.setSize(2);
    hzDataPoint.setAddressOffset(26);
    hzDataPoint.setBlockOffset(24);
    hzDataPoint.setSunSpecDataType("float32");
    hzDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hzDataPoint.name(), hzDataPoint);

    SunSpecDataPoint wattsDataPoint;
    wattsDataPoint.setName("W");
    wattsDataPoint.setLabel("Watts");
    wattsDataPoint.setDescription("Total Real Power");
    wattsDataPoint.setUnits("W");
    wattsDataPoint.setMandatory(true);
    wattsDataPoint.setSize(2);
    wattsDataPoint.setAddressOffset(28);
    wattsDataPoint.setBlockOffset(26);
    wattsDataPoint.setSunSpecDataType("float32");
    wattsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wattsDataPoint.name(), wattsDataPoint);

    SunSpecDataPoint wattsPhaseADataPoint;
    wattsPhaseADataPoint.setName("WphA");
    wattsPhaseADataPoint.setLabel("Watts phase A");
    wattsPhaseADataPoint.setUnits("W");
    wattsPhaseADataPoint.setSize(2);
    wattsPhaseADataPoint.setAddressOffset(30);
    wattsPhaseADataPoint.setBlockOffset(28);
    wattsPhaseADataPoint.setSunSpecDataType("float32");
    wattsPhaseADataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wattsPhaseADataPoint.name(), wattsPhaseADataPoint);

    SunSpecDataPoint wattsPhaseBDataPoint;
    wattsPhaseBDataPoint.setName("WphB");
    wattsPhaseBDataPoint.setLabel("Watts phase B");
    wattsPhaseBDataPoint.setUnits("W");
    wattsPhaseBDataPoint.setSize(2);
    wattsPhaseBDataPoint.setAddressOffset(32);
    wattsPhaseBDataPoint.setBlockOffset(30);
    wattsPhaseBDataPoint.setSunSpecDataType("float32");
    wattsPhaseBDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wattsPhaseBDataPoint.name(), wattsPhaseBDataPoint);

    SunSpecDataPoint wattsPhaseCDataPoint;
    wattsPhaseCDataPoint.setName("WphC");
    wattsPhaseCDataPoint.setLabel("Watts phase C");
    wattsPhaseCDataPoint.setUnits("W");
    wattsPhaseCDataPoint.setSize(2);
    wattsPhaseCDataPoint.setAddressOffset(34);
    wattsPhaseCDataPoint.setBlockOffset(32);
    wattsPhaseCDataPoint.setSunSpecDataType("float32");
    wattsPhaseCDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wattsPhaseCDataPoint.name(), wattsPhaseCDataPoint);

    SunSpecDataPoint vaDataPoint;
    vaDataPoint.setName("VA");
    vaDataPoint.setLabel("VA");
    vaDataPoint.setDescription("AC Apparent Power");
    vaDataPoint.setUnits("VA");
    vaDataPoint.setSize(2);
    vaDataPoint.setAddressOffset(36);
    vaDataPoint.setBlockOffset(34);
    vaDataPoint.setSunSpecDataType("float32");
    vaDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vaDataPoint.name(), vaDataPoint);

    SunSpecDataPoint vaPhaseADataPoint;
    vaPhaseADataPoint.setName("VAphA");
    vaPhaseADataPoint.setLabel("VA phase A");
    vaPhaseADataPoint.setUnits("VA");
    vaPhaseADataPoint.setSize(2);
    vaPhaseADataPoint.setAddressOffset(38);
    vaPhaseADataPoint.setBlockOffset(36);
    vaPhaseADataPoint.setSunSpecDataType("float32");
    vaPhaseADataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vaPhaseADataPoint.name(), vaPhaseADataPoint);

    SunSpecDataPoint vaPhaseBDataPoint;
    vaPhaseBDataPoint.setName("VAphB");
    vaPhaseBDataPoint.setLabel("VA phase B");
    vaPhaseBDataPoint.setUnits("VA");
    vaPhaseBDataPoint.setSize(2);
    vaPhaseBDataPoint.setAddressOffset(40);
    vaPhaseBDataPoint.setBlockOffset(38);
    vaPhaseBDataPoint.setSunSpecDataType("float32");
    vaPhaseBDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vaPhaseBDataPoint.name(), vaPhaseBDataPoint);

    SunSpecDataPoint vaPhaseCDataPoint;
    vaPhaseCDataPoint.setName("VAphC");
    vaPhaseCDataPoint.setLabel("VA phase C");
    vaPhaseCDataPoint.setUnits("VA");
    vaPhaseCDataPoint.setSize(2);
    vaPhaseCDataPoint.setAddressOffset(42);
    vaPhaseCDataPoint.setBlockOffset(40);
    vaPhaseCDataPoint.setSunSpecDataType("float32");
    vaPhaseCDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vaPhaseCDataPoint.name(), vaPhaseCDataPoint);

    SunSpecDataPoint varDataPoint;
    varDataPoint.setName("VAR");
    varDataPoint.setLabel("VAR");
    varDataPoint.setDescription("Reactive Power");
    varDataPoint.setUnits("var");
    varDataPoint.setSize(2);
    varDataPoint.setAddressOffset(44);
    varDataPoint.setBlockOffset(42);
    varDataPoint.setSunSpecDataType("float32");
    varDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(varDataPoint.name(), varDataPoint);

    SunSpecDataPoint varPhaseADataPoint;
    varPhaseADataPoint.setName("VARphA");
    varPhaseADataPoint.setLabel("VAR phase A");
    varPhaseADataPoint.setUnits("var");
    varPhaseADataPoint.setSize(2);
    varPhaseADataPoint.setAddressOffset(46);
    varPhaseADataPoint.setBlockOffset(44);
    varPhaseADataPoint.setSunSpecDataType("float32");
    varPhaseADataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(varPhaseADataPoint.name(), varPhaseADataPoint);

    SunSpecDataPoint varPhaseBDataPoint;
    varPhaseBDataPoint.setName("VARphB");
    varPhaseBDataPoint.setLabel("VAR phase B");
    varPhaseBDataPoint.setUnits("var");
    varPhaseBDataPoint.setSize(2);
    varPhaseBDataPoint.setAddressOffset(48);
    varPhaseBDataPoint.setBlockOffset(46);
    varPhaseBDataPoint.setSunSpecDataType("float32");
    varPhaseBDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(varPhaseBDataPoint.name(), varPhaseBDataPoint);

    SunSpecDataPoint varPhaseCDataPoint;
    varPhaseCDataPoint.setName("VARphC");
    varPhaseCDataPoint.setLabel("VAR phase C");
    varPhaseCDataPoint.setUnits("var");
    varPhaseCDataPoint.setSize(2);
    varPhaseCDataPoint.setAddressOffset(50);
    varPhaseCDataPoint.setBlockOffset(48);
    varPhaseCDataPoint.setSunSpecDataType("float32");
    varPhaseCDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(varPhaseCDataPoint.name(), varPhaseCDataPoint);

    SunSpecDataPoint pfDataPoint;
    pfDataPoint.setName("PF");
    pfDataPoint.setLabel("PF");
    pfDataPoint.setDescription("Power Factor");
    pfDataPoint.setUnits("PF");
    pfDataPoint.setSize(2);
    pfDataPoint.setAddressOffset(52);
    pfDataPoint.setBlockOffset(50);
    pfDataPoint.setSunSpecDataType("float32");
    pfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pfDataPoint.name(), pfDataPoint);

    SunSpecDataPoint pfPhaseADataPoint;
    pfPhaseADataPoint.setName("PFphA");
    pfPhaseADataPoint.setLabel("PF phase A");
    pfPhaseADataPoint.setUnits("PF");
    pfPhaseADataPoint.setSize(2);
    pfPhaseADataPoint.setAddressOffset(54);
    pfPhaseADataPoint.setBlockOffset(52);
    pfPhaseADataPoint.setSunSpecDataType("float32");
    pfPhaseADataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pfPhaseADataPoint.name(), pfPhaseADataPoint);

    SunSpecDataPoint pfPhaseBDataPoint;
    pfPhaseBDataPoint.setName("PFphB");
    pfPhaseBDataPoint.setLabel("PF phase B");
    pfPhaseBDataPoint.setUnits("PF");
    pfPhaseBDataPoint.setSize(2);
    pfPhaseBDataPoint.setAddressOffset(56);
    pfPhaseBDataPoint.setBlockOffset(54);
    pfPhaseBDataPoint.setSunSpecDataType("float32");
    pfPhaseBDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pfPhaseBDataPoint.name(), pfPhaseBDataPoint);

    SunSpecDataPoint pfPhaseCDataPoint;
    pfPhaseCDataPoint.setName("PFphC");
    pfPhaseCDataPoint.setLabel("PF phase C");
    pfPhaseCDataPoint.setUnits("PF");
    pfPhaseCDataPoint.setSize(2);
    pfPhaseCDataPoint.setAddressOffset(58);
    pfPhaseCDataPoint.setBlockOffset(56);
    pfPhaseCDataPoint.setSunSpecDataType("float32");
    pfPhaseCDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pfPhaseCDataPoint.name(), pfPhaseCDataPoint);

    SunSpecDataPoint totalWattHoursExportedDataPoint;
    totalWattHoursExportedDataPoint.setName("TotWhExp");
    totalWattHoursExportedDataPoint.setLabel("Total Watt-hours Exported");
    totalWattHoursExportedDataPoint.setDescription("Total Real Energy Exported");
    totalWattHoursExportedDataPoint.setUnits("Wh");
    totalWattHoursExportedDataPoint.setMandatory(true);
    totalWattHoursExportedDataPoint.setSize(2);
    totalWattHoursExportedDataPoint.setAddressOffset(60);
    totalWattHoursExportedDataPoint.setBlockOffset(58);
    totalWattHoursExportedDataPoint.setSunSpecDataType("float32");
    totalWattHoursExportedDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalWattHoursExportedDataPoint.name(), totalWattHoursExportedDataPoint);

    SunSpecDataPoint totalWattHoursExportedPhaseADataPoint;
    totalWattHoursExportedPhaseADataPoint.setName("TotWhExpPhA");
    totalWattHoursExportedPhaseADataPoint.setLabel("Total Watt-hours Exported phase A");
    totalWattHoursExportedPhaseADataPoint.setUnits("Wh");
    totalWattHoursExportedPhaseADataPoint.setSize(2);
    totalWattHoursExportedPhaseADataPoint.setAddressOffset(62);
    totalWattHoursExportedPhaseADataPoint.setBlockOffset(60);
    totalWattHoursExportedPhaseADataPoint.setSunSpecDataType("float32");
    totalWattHoursExportedPhaseADataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalWattHoursExportedPhaseADataPoint.name(), totalWattHoursExportedPhaseADataPoint);

    SunSpecDataPoint totalWattHoursExportedPhaseBDataPoint;
    totalWattHoursExportedPhaseBDataPoint.setName("TotWhExpPhB");
    totalWattHoursExportedPhaseBDataPoint.setLabel("Total Watt-hours Exported phase B");
    totalWattHoursExportedPhaseBDataPoint.setUnits("Wh");
    totalWattHoursExportedPhaseBDataPoint.setSize(2);
    totalWattHoursExportedPhaseBDataPoint.setAddressOffset(64);
    totalWattHoursExportedPhaseBDataPoint.setBlockOffset(62);
    totalWattHoursExportedPhaseBDataPoint.setSunSpecDataType("float32");
    totalWattHoursExportedPhaseBDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalWattHoursExportedPhaseBDataPoint.name(), totalWattHoursExportedPhaseBDataPoint);

    SunSpecDataPoint totalWattHoursExportedPhaseCDataPoint;
    totalWattHoursExportedPhaseCDataPoint.setName("TotWhExpPhC");
    totalWattHoursExportedPhaseCDataPoint.setLabel("Total Watt-hours Exported phase C");
    totalWattHoursExportedPhaseCDataPoint.setUnits("Wh");
    totalWattHoursExportedPhaseCDataPoint.setSize(2);
    totalWattHoursExportedPhaseCDataPoint.setAddressOffset(66);
    totalWattHoursExportedPhaseCDataPoint.setBlockOffset(64);
    totalWattHoursExportedPhaseCDataPoint.setSunSpecDataType("float32");
    totalWattHoursExportedPhaseCDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalWattHoursExportedPhaseCDataPoint.name(), totalWattHoursExportedPhaseCDataPoint);

    SunSpecDataPoint totalWattHoursImportedDataPoint;
    totalWattHoursImportedDataPoint.setName("TotWhImp");
    totalWattHoursImportedDataPoint.setLabel("Total Watt-hours Imported");
    totalWattHoursImportedDataPoint.setDescription("Total Real Energy Imported");
    totalWattHoursImportedDataPoint.setUnits("Wh");
    totalWattHoursImportedDataPoint.setMandatory(true);
    totalWattHoursImportedDataPoint.setSize(2);
    totalWattHoursImportedDataPoint.setAddressOffset(68);
    totalWattHoursImportedDataPoint.setBlockOffset(66);
    totalWattHoursImportedDataPoint.setSunSpecDataType("float32");
    totalWattHoursImportedDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalWattHoursImportedDataPoint.name(), totalWattHoursImportedDataPoint);

    SunSpecDataPoint totalWattHoursImportedPhaseADataPoint;
    totalWattHoursImportedPhaseADataPoint.setName("TotWhImpPhA");
    totalWattHoursImportedPhaseADataPoint.setLabel("Total Watt-hours Imported phase A");
    totalWattHoursImportedPhaseADataPoint.setUnits("Wh");
    totalWattHoursImportedPhaseADataPoint.setSize(2);
    totalWattHoursImportedPhaseADataPoint.setAddressOffset(70);
    totalWattHoursImportedPhaseADataPoint.setBlockOffset(68);
    totalWattHoursImportedPhaseADataPoint.setSunSpecDataType("float32");
    totalWattHoursImportedPhaseADataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalWattHoursImportedPhaseADataPoint.name(), totalWattHoursImportedPhaseADataPoint);

    SunSpecDataPoint totalWattHoursImportedPhaseBDataPoint;
    totalWattHoursImportedPhaseBDataPoint.setName("TotWhImpPhB");
    totalWattHoursImportedPhaseBDataPoint.setLabel("Total Watt-hours Imported phase B");
    totalWattHoursImportedPhaseBDataPoint.setUnits("Wh");
    totalWattHoursImportedPhaseBDataPoint.setSize(2);
    totalWattHoursImportedPhaseBDataPoint.setAddressOffset(72);
    totalWattHoursImportedPhaseBDataPoint.setBlockOffset(70);
    totalWattHoursImportedPhaseBDataPoint.setSunSpecDataType("float32");
    totalWattHoursImportedPhaseBDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalWattHoursImportedPhaseBDataPoint.name(), totalWattHoursImportedPhaseBDataPoint);

    SunSpecDataPoint totalWattHoursImportedPhaseCDataPoint;
    totalWattHoursImportedPhaseCDataPoint.setName("TotWhImpPhC");
    totalWattHoursImportedPhaseCDataPoint.setLabel("Total Watt-hours Imported phase C");
    totalWattHoursImportedPhaseCDataPoint.setUnits("Wh");
    totalWattHoursImportedPhaseCDataPoint.setSize(2);
    totalWattHoursImportedPhaseCDataPoint.setAddressOffset(74);
    totalWattHoursImportedPhaseCDataPoint.setBlockOffset(72);
    totalWattHoursImportedPhaseCDataPoint.setSunSpecDataType("float32");
    totalWattHoursImportedPhaseCDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalWattHoursImportedPhaseCDataPoint.name(), totalWattHoursImportedPhaseCDataPoint);

    SunSpecDataPoint totalVaHoursExportedDataPoint;
    totalVaHoursExportedDataPoint.setName("TotVAhExp");
    totalVaHoursExportedDataPoint.setLabel("Total VA-hours Exported");
    totalVaHoursExportedDataPoint.setDescription("Total Apparent Energy Exported");
    totalVaHoursExportedDataPoint.setUnits("VAh");
    totalVaHoursExportedDataPoint.setSize(2);
    totalVaHoursExportedDataPoint.setAddressOffset(76);
    totalVaHoursExportedDataPoint.setBlockOffset(74);
    totalVaHoursExportedDataPoint.setSunSpecDataType("float32");
    totalVaHoursExportedDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVaHoursExportedDataPoint.name(), totalVaHoursExportedDataPoint);

    SunSpecDataPoint totalVaHoursExportedPhaseADataPoint;
    totalVaHoursExportedPhaseADataPoint.setName("TotVAhExpPhA");
    totalVaHoursExportedPhaseADataPoint.setLabel("Total VA-hours Exported phase A");
    totalVaHoursExportedPhaseADataPoint.setUnits("VAh");
    totalVaHoursExportedPhaseADataPoint.setSize(2);
    totalVaHoursExportedPhaseADataPoint.setAddressOffset(78);
    totalVaHoursExportedPhaseADataPoint.setBlockOffset(76);
    totalVaHoursExportedPhaseADataPoint.setSunSpecDataType("float32");
    totalVaHoursExportedPhaseADataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVaHoursExportedPhaseADataPoint.name(), totalVaHoursExportedPhaseADataPoint);

    SunSpecDataPoint totalVaHoursExportedPhaseBDataPoint;
    totalVaHoursExportedPhaseBDataPoint.setName("TotVAhExpPhB");
    totalVaHoursExportedPhaseBDataPoint.setLabel("Total VA-hours Exported phase B");
    totalVaHoursExportedPhaseBDataPoint.setUnits("VAh");
    totalVaHoursExportedPhaseBDataPoint.setSize(2);
    totalVaHoursExportedPhaseBDataPoint.setAddressOffset(80);
    totalVaHoursExportedPhaseBDataPoint.setBlockOffset(78);
    totalVaHoursExportedPhaseBDataPoint.setSunSpecDataType("float32");
    totalVaHoursExportedPhaseBDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVaHoursExportedPhaseBDataPoint.name(), totalVaHoursExportedPhaseBDataPoint);

    SunSpecDataPoint totalVaHoursExportedPhaseCDataPoint;
    totalVaHoursExportedPhaseCDataPoint.setName("TotVAhExpPhC");
    totalVaHoursExportedPhaseCDataPoint.setLabel("Total VA-hours Exported phase C");
    totalVaHoursExportedPhaseCDataPoint.setUnits("VAh");
    totalVaHoursExportedPhaseCDataPoint.setSize(2);
    totalVaHoursExportedPhaseCDataPoint.setAddressOffset(82);
    totalVaHoursExportedPhaseCDataPoint.setBlockOffset(80);
    totalVaHoursExportedPhaseCDataPoint.setSunSpecDataType("float32");
    totalVaHoursExportedPhaseCDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVaHoursExportedPhaseCDataPoint.name(), totalVaHoursExportedPhaseCDataPoint);

    SunSpecDataPoint totalVaHoursImportedDataPoint;
    totalVaHoursImportedDataPoint.setName("TotVAhImp");
    totalVaHoursImportedDataPoint.setLabel("Total VA-hours Imported");
    totalVaHoursImportedDataPoint.setDescription("Total Apparent Energy Imported");
    totalVaHoursImportedDataPoint.setUnits("VAh");
    totalVaHoursImportedDataPoint.setSize(2);
    totalVaHoursImportedDataPoint.setAddressOffset(84);
    totalVaHoursImportedDataPoint.setBlockOffset(82);
    totalVaHoursImportedDataPoint.setSunSpecDataType("float32");
    totalVaHoursImportedDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVaHoursImportedDataPoint.name(), totalVaHoursImportedDataPoint);

    SunSpecDataPoint totalVaHoursImportedPhaseADataPoint;
    totalVaHoursImportedPhaseADataPoint.setName("TotVAhImpPhA");
    totalVaHoursImportedPhaseADataPoint.setLabel("Total VA-hours Imported phase A");
    totalVaHoursImportedPhaseADataPoint.setUnits("VAh");
    totalVaHoursImportedPhaseADataPoint.setSize(2);
    totalVaHoursImportedPhaseADataPoint.setAddressOffset(86);
    totalVaHoursImportedPhaseADataPoint.setBlockOffset(84);
    totalVaHoursImportedPhaseADataPoint.setSunSpecDataType("float32");
    totalVaHoursImportedPhaseADataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVaHoursImportedPhaseADataPoint.name(), totalVaHoursImportedPhaseADataPoint);

    SunSpecDataPoint totalVaHoursImportedPhaseBDataPoint;
    totalVaHoursImportedPhaseBDataPoint.setName("TotVAhImpPhB");
    totalVaHoursImportedPhaseBDataPoint.setLabel("Total VA-hours Imported phase B");
    totalVaHoursImportedPhaseBDataPoint.setUnits("VAh");
    totalVaHoursImportedPhaseBDataPoint.setSize(2);
    totalVaHoursImportedPhaseBDataPoint.setAddressOffset(88);
    totalVaHoursImportedPhaseBDataPoint.setBlockOffset(86);
    totalVaHoursImportedPhaseBDataPoint.setSunSpecDataType("float32");
    totalVaHoursImportedPhaseBDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVaHoursImportedPhaseBDataPoint.name(), totalVaHoursImportedPhaseBDataPoint);

    SunSpecDataPoint totalVaHoursImportedPhaseCDataPoint;
    totalVaHoursImportedPhaseCDataPoint.setName("TotVAhImpPhC");
    totalVaHoursImportedPhaseCDataPoint.setLabel("Total VA-hours Imported phase C");
    totalVaHoursImportedPhaseCDataPoint.setUnits("VAh");
    totalVaHoursImportedPhaseCDataPoint.setSize(2);
    totalVaHoursImportedPhaseCDataPoint.setAddressOffset(90);
    totalVaHoursImportedPhaseCDataPoint.setBlockOffset(88);
    totalVaHoursImportedPhaseCDataPoint.setSunSpecDataType("float32");
    totalVaHoursImportedPhaseCDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVaHoursImportedPhaseCDataPoint.name(), totalVaHoursImportedPhaseCDataPoint);

    SunSpecDataPoint totalVarHoursImportedQ1DataPoint;
    totalVarHoursImportedQ1DataPoint.setName("TotVArhImpQ1");
    totalVarHoursImportedQ1DataPoint.setLabel("Total VAR-hours Imported Q1");
    totalVarHoursImportedQ1DataPoint.setDescription("Total Reactive Energy Imported Quadrant 1");
    totalVarHoursImportedQ1DataPoint.setUnits("varh");
    totalVarHoursImportedQ1DataPoint.setSize(2);
    totalVarHoursImportedQ1DataPoint.setAddressOffset(92);
    totalVarHoursImportedQ1DataPoint.setBlockOffset(90);
    totalVarHoursImportedQ1DataPoint.setSunSpecDataType("float32");
    totalVarHoursImportedQ1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVarHoursImportedQ1DataPoint.name(), totalVarHoursImportedQ1DataPoint);

    SunSpecDataPoint totalVArHoursImportedQ1PhaseADataPoint;
    totalVArHoursImportedQ1PhaseADataPoint.setName("TotVArhImpQ1phA");
    totalVArHoursImportedQ1PhaseADataPoint.setLabel("Total VAr-hours Imported Q1 phase A");
    totalVArHoursImportedQ1PhaseADataPoint.setUnits("varh");
    totalVArHoursImportedQ1PhaseADataPoint.setSize(2);
    totalVArHoursImportedQ1PhaseADataPoint.setAddressOffset(94);
    totalVArHoursImportedQ1PhaseADataPoint.setBlockOffset(92);
    totalVArHoursImportedQ1PhaseADataPoint.setSunSpecDataType("float32");
    totalVArHoursImportedQ1PhaseADataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVArHoursImportedQ1PhaseADataPoint.name(), totalVArHoursImportedQ1PhaseADataPoint);

    SunSpecDataPoint totalVArHoursImportedQ1PhaseBDataPoint;
    totalVArHoursImportedQ1PhaseBDataPoint.setName("TotVArhImpQ1phB");
    totalVArHoursImportedQ1PhaseBDataPoint.setLabel("Total VAr-hours Imported Q1 phase B");
    totalVArHoursImportedQ1PhaseBDataPoint.setUnits("varh");
    totalVArHoursImportedQ1PhaseBDataPoint.setSize(2);
    totalVArHoursImportedQ1PhaseBDataPoint.setAddressOffset(96);
    totalVArHoursImportedQ1PhaseBDataPoint.setBlockOffset(94);
    totalVArHoursImportedQ1PhaseBDataPoint.setSunSpecDataType("float32");
    totalVArHoursImportedQ1PhaseBDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVArHoursImportedQ1PhaseBDataPoint.name(), totalVArHoursImportedQ1PhaseBDataPoint);

    SunSpecDataPoint totalVArHoursImportedQ1PhaseCDataPoint;
    totalVArHoursImportedQ1PhaseCDataPoint.setName("TotVArhImpQ1phC");
    totalVArHoursImportedQ1PhaseCDataPoint.setLabel("Total VAr-hours Imported Q1 phase C");
    totalVArHoursImportedQ1PhaseCDataPoint.setUnits("varh");
    totalVArHoursImportedQ1PhaseCDataPoint.setSize(2);
    totalVArHoursImportedQ1PhaseCDataPoint.setAddressOffset(98);
    totalVArHoursImportedQ1PhaseCDataPoint.setBlockOffset(96);
    totalVArHoursImportedQ1PhaseCDataPoint.setSunSpecDataType("float32");
    totalVArHoursImportedQ1PhaseCDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVArHoursImportedQ1PhaseCDataPoint.name(), totalVArHoursImportedQ1PhaseCDataPoint);

    SunSpecDataPoint totalVArHoursImportedQ2DataPoint;
    totalVArHoursImportedQ2DataPoint.setName("TotVArhImpQ2");
    totalVArHoursImportedQ2DataPoint.setLabel("Total VAr-hours Imported Q2");
    totalVArHoursImportedQ2DataPoint.setDescription("Total Reactive Power Imported Quadrant 2");
    totalVArHoursImportedQ2DataPoint.setUnits("varh");
    totalVArHoursImportedQ2DataPoint.setSize(2);
    totalVArHoursImportedQ2DataPoint.setAddressOffset(100);
    totalVArHoursImportedQ2DataPoint.setBlockOffset(98);
    totalVArHoursImportedQ2DataPoint.setSunSpecDataType("float32");
    totalVArHoursImportedQ2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVArHoursImportedQ2DataPoint.name(), totalVArHoursImportedQ2DataPoint);

    SunSpecDataPoint totalVArHoursImportedQ2PhaseADataPoint;
    totalVArHoursImportedQ2PhaseADataPoint.setName("TotVArhImpQ2phA");
    totalVArHoursImportedQ2PhaseADataPoint.setLabel("Total VAr-hours Imported Q2 phase A");
    totalVArHoursImportedQ2PhaseADataPoint.setUnits("varh");
    totalVArHoursImportedQ2PhaseADataPoint.setSize(2);
    totalVArHoursImportedQ2PhaseADataPoint.setAddressOffset(102);
    totalVArHoursImportedQ2PhaseADataPoint.setBlockOffset(100);
    totalVArHoursImportedQ2PhaseADataPoint.setSunSpecDataType("float32");
    totalVArHoursImportedQ2PhaseADataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVArHoursImportedQ2PhaseADataPoint.name(), totalVArHoursImportedQ2PhaseADataPoint);

    SunSpecDataPoint totalVArHoursImportedQ2PhaseBDataPoint;
    totalVArHoursImportedQ2PhaseBDataPoint.setName("TotVArhImpQ2phB");
    totalVArHoursImportedQ2PhaseBDataPoint.setLabel("Total VAr-hours Imported Q2 phase B");
    totalVArHoursImportedQ2PhaseBDataPoint.setUnits("varh");
    totalVArHoursImportedQ2PhaseBDataPoint.setSize(2);
    totalVArHoursImportedQ2PhaseBDataPoint.setAddressOffset(104);
    totalVArHoursImportedQ2PhaseBDataPoint.setBlockOffset(102);
    totalVArHoursImportedQ2PhaseBDataPoint.setSunSpecDataType("float32");
    totalVArHoursImportedQ2PhaseBDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVArHoursImportedQ2PhaseBDataPoint.name(), totalVArHoursImportedQ2PhaseBDataPoint);

    SunSpecDataPoint totalVArHoursImportedQ2PhaseCDataPoint;
    totalVArHoursImportedQ2PhaseCDataPoint.setName("TotVArhImpQ2phC");
    totalVArHoursImportedQ2PhaseCDataPoint.setLabel("Total VAr-hours Imported Q2 phase C");
    totalVArHoursImportedQ2PhaseCDataPoint.setUnits("varh");
    totalVArHoursImportedQ2PhaseCDataPoint.setSize(2);
    totalVArHoursImportedQ2PhaseCDataPoint.setAddressOffset(106);
    totalVArHoursImportedQ2PhaseCDataPoint.setBlockOffset(104);
    totalVArHoursImportedQ2PhaseCDataPoint.setSunSpecDataType("float32");
    totalVArHoursImportedQ2PhaseCDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVArHoursImportedQ2PhaseCDataPoint.name(), totalVArHoursImportedQ2PhaseCDataPoint);

    SunSpecDataPoint totalVArHoursExportedQ3DataPoint;
    totalVArHoursExportedQ3DataPoint.setName("TotVArhExpQ3");
    totalVArHoursExportedQ3DataPoint.setLabel("Total VAr-hours Exported Q3");
    totalVArHoursExportedQ3DataPoint.setDescription("Total Reactive Power Exported Quadrant 3");
    totalVArHoursExportedQ3DataPoint.setUnits("varh");
    totalVArHoursExportedQ3DataPoint.setSize(2);
    totalVArHoursExportedQ3DataPoint.setAddressOffset(108);
    totalVArHoursExportedQ3DataPoint.setBlockOffset(106);
    totalVArHoursExportedQ3DataPoint.setSunSpecDataType("float32");
    totalVArHoursExportedQ3DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVArHoursExportedQ3DataPoint.name(), totalVArHoursExportedQ3DataPoint);

    SunSpecDataPoint totalVArHoursExportedQ3PhaseADataPoint;
    totalVArHoursExportedQ3PhaseADataPoint.setName("TotVArhExpQ3phA");
    totalVArHoursExportedQ3PhaseADataPoint.setLabel("Total VAr-hours Exported Q3 phase A");
    totalVArHoursExportedQ3PhaseADataPoint.setUnits("varh");
    totalVArHoursExportedQ3PhaseADataPoint.setSize(2);
    totalVArHoursExportedQ3PhaseADataPoint.setAddressOffset(110);
    totalVArHoursExportedQ3PhaseADataPoint.setBlockOffset(108);
    totalVArHoursExportedQ3PhaseADataPoint.setSunSpecDataType("float32");
    totalVArHoursExportedQ3PhaseADataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVArHoursExportedQ3PhaseADataPoint.name(), totalVArHoursExportedQ3PhaseADataPoint);

    SunSpecDataPoint totalVArHoursExportedQ3PhaseBDataPoint;
    totalVArHoursExportedQ3PhaseBDataPoint.setName("TotVArhExpQ3phB");
    totalVArHoursExportedQ3PhaseBDataPoint.setLabel("Total VAr-hours Exported Q3 phase B");
    totalVArHoursExportedQ3PhaseBDataPoint.setUnits("varh");
    totalVArHoursExportedQ3PhaseBDataPoint.setSize(2);
    totalVArHoursExportedQ3PhaseBDataPoint.setAddressOffset(112);
    totalVArHoursExportedQ3PhaseBDataPoint.setBlockOffset(110);
    totalVArHoursExportedQ3PhaseBDataPoint.setSunSpecDataType("float32");
    totalVArHoursExportedQ3PhaseBDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVArHoursExportedQ3PhaseBDataPoint.name(), totalVArHoursExportedQ3PhaseBDataPoint);

    SunSpecDataPoint totalVArHoursExportedQ3PhaseCDataPoint;
    totalVArHoursExportedQ3PhaseCDataPoint.setName("TotVArhExpQ3phC");
    totalVArHoursExportedQ3PhaseCDataPoint.setLabel("Total VAr-hours Exported Q3 phase C");
    totalVArHoursExportedQ3PhaseCDataPoint.setUnits("varh");
    totalVArHoursExportedQ3PhaseCDataPoint.setSize(2);
    totalVArHoursExportedQ3PhaseCDataPoint.setAddressOffset(114);
    totalVArHoursExportedQ3PhaseCDataPoint.setBlockOffset(112);
    totalVArHoursExportedQ3PhaseCDataPoint.setSunSpecDataType("float32");
    totalVArHoursExportedQ3PhaseCDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVArHoursExportedQ3PhaseCDataPoint.name(), totalVArHoursExportedQ3PhaseCDataPoint);

    SunSpecDataPoint totalVArHoursExportedQ4DataPoint;
    totalVArHoursExportedQ4DataPoint.setName("TotVArhExpQ4");
    totalVArHoursExportedQ4DataPoint.setLabel("Total VAr-hours Exported Q4");
    totalVArHoursExportedQ4DataPoint.setDescription("Total Reactive Power Exported Quadrant 4");
    totalVArHoursExportedQ4DataPoint.setUnits("varh");
    totalVArHoursExportedQ4DataPoint.setSize(2);
    totalVArHoursExportedQ4DataPoint.setAddressOffset(116);
    totalVArHoursExportedQ4DataPoint.setBlockOffset(114);
    totalVArHoursExportedQ4DataPoint.setSunSpecDataType("float32");
    totalVArHoursExportedQ4DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVArHoursExportedQ4DataPoint.name(), totalVArHoursExportedQ4DataPoint);

    SunSpecDataPoint totalVArHoursExportedQ4ImportedPhaseADataPoint;
    totalVArHoursExportedQ4ImportedPhaseADataPoint.setName("TotVArhExpQ4phA");
    totalVArHoursExportedQ4ImportedPhaseADataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase A");
    totalVArHoursExportedQ4ImportedPhaseADataPoint.setUnits("varh");
    totalVArHoursExportedQ4ImportedPhaseADataPoint.setSize(2);
    totalVArHoursExportedQ4ImportedPhaseADataPoint.setAddressOffset(118);
    totalVArHoursExportedQ4ImportedPhaseADataPoint.setBlockOffset(116);
    totalVArHoursExportedQ4ImportedPhaseADataPoint.setSunSpecDataType("float32");
    totalVArHoursExportedQ4ImportedPhaseADataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVArHoursExportedQ4ImportedPhaseADataPoint.name(), totalVArHoursExportedQ4ImportedPhaseADataPoint);

    SunSpecDataPoint totalVArHoursExportedQ4ImportedPhaseBDataPoint;
    totalVArHoursExportedQ4ImportedPhaseBDataPoint.setName("TotVArhExpQ4phB");
    totalVArHoursExportedQ4ImportedPhaseBDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase B");
    totalVArHoursExportedQ4ImportedPhaseBDataPoint.setUnits("varh");
    totalVArHoursExportedQ4ImportedPhaseBDataPoint.setSize(2);
    totalVArHoursExportedQ4ImportedPhaseBDataPoint.setAddressOffset(120);
    totalVArHoursExportedQ4ImportedPhaseBDataPoint.setBlockOffset(118);
    totalVArHoursExportedQ4ImportedPhaseBDataPoint.setSunSpecDataType("float32");
    totalVArHoursExportedQ4ImportedPhaseBDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVArHoursExportedQ4ImportedPhaseBDataPoint.name(), totalVArHoursExportedQ4ImportedPhaseBDataPoint);

    SunSpecDataPoint totalVArHoursExportedQ4ImportedPhaseCDataPoint;
    totalVArHoursExportedQ4ImportedPhaseCDataPoint.setName("TotVArhExpQ4phC");
    totalVArHoursExportedQ4ImportedPhaseCDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase C");
    totalVArHoursExportedQ4ImportedPhaseCDataPoint.setUnits("varh");
    totalVArHoursExportedQ4ImportedPhaseCDataPoint.setSize(2);
    totalVArHoursExportedQ4ImportedPhaseCDataPoint.setAddressOffset(122);
    totalVArHoursExportedQ4ImportedPhaseCDataPoint.setBlockOffset(120);
    totalVArHoursExportedQ4ImportedPhaseCDataPoint.setSunSpecDataType("float32");
    totalVArHoursExportedQ4ImportedPhaseCDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(totalVArHoursExportedQ4ImportedPhaseCDataPoint.name(), totalVArHoursExportedQ4ImportedPhaseCDataPoint);

    SunSpecDataPoint eventsDataPoint;
    eventsDataPoint.setName("Evt");
    eventsDataPoint.setLabel("Events");
    eventsDataPoint.setDescription("Meter Event Flags");
    eventsDataPoint.setMandatory(true);
    eventsDataPoint.setSize(2);
    eventsDataPoint.setAddressOffset(124);
    eventsDataPoint.setBlockOffset(122);
    eventsDataPoint.setSunSpecDataType("bitfield32");
    eventsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(eventsDataPoint.name(), eventsDataPoint);

}

void SunSpecMeterSinglePhaseFloatModel::processBlockData()
{
    // Update properties according to the data point type
    if (m_dataPoints.value("A").isValid())
        m_amps = m_dataPoints.value("A").toFloat();

    if (m_dataPoints.value("AphA").isValid())
        m_ampsPhaseA = m_dataPoints.value("AphA").toFloat();

    if (m_dataPoints.value("AphB").isValid())
        m_ampsPhaseB = m_dataPoints.value("AphB").toFloat();

    if (m_dataPoints.value("AphC").isValid())
        m_ampsPhaseC = m_dataPoints.value("AphC").toFloat();

    if (m_dataPoints.value("PhV").isValid())
        m_voltageLn = m_dataPoints.value("PhV").toFloat();

    if (m_dataPoints.value("PhVphA").isValid())
        m_phaseVoltageAn = m_dataPoints.value("PhVphA").toFloat();

    if (m_dataPoints.value("PhVphB").isValid())
        m_phaseVoltageBn = m_dataPoints.value("PhVphB").toFloat();

    if (m_dataPoints.value("PhVphC").isValid())
        m_phaseVoltageCn = m_dataPoints.value("PhVphC").toFloat();

    if (m_dataPoints.value("PPV").isValid())
        m_voltageLl = m_dataPoints.value("PPV").toFloat();

    if (m_dataPoints.value("PPVphAB").isValid())
        m_phaseVoltageAb = m_dataPoints.value("PPVphAB").toFloat();

    if (m_dataPoints.value("PPVphBC").isValid())
        m_phaseVoltageBc = m_dataPoints.value("PPVphBC").toFloat();

    if (m_dataPoints.value("PPVphCA").isValid())
        m_phaseVoltageCa = m_dataPoints.value("PPVphCA").toFloat();

    if (m_dataPoints.value("Hz").isValid())
        m_hz = m_dataPoints.value("Hz").toFloat();

    if (m_dataPoints.value("W").isValid())
        m_watts = m_dataPoints.value("W").toFloat();

    if (m_dataPoints.value("WphA").isValid())
        m_wattsPhaseA = m_dataPoints.value("WphA").toFloat();

    if (m_dataPoints.value("WphB").isValid())
        m_wattsPhaseB = m_dataPoints.value("WphB").toFloat();

    if (m_dataPoints.value("WphC").isValid())
        m_wattsPhaseC = m_dataPoints.value("WphC").toFloat();

    if (m_dataPoints.value("VA").isValid())
        m_va = m_dataPoints.value("VA").toFloat();

    if (m_dataPoints.value("VAphA").isValid())
        m_vaPhaseA = m_dataPoints.value("VAphA").toFloat();

    if (m_dataPoints.value("VAphB").isValid())
        m_vaPhaseB = m_dataPoints.value("VAphB").toFloat();

    if (m_dataPoints.value("VAphC").isValid())
        m_vaPhaseC = m_dataPoints.value("VAphC").toFloat();

    if (m_dataPoints.value("VAR").isValid())
        m_var = m_dataPoints.value("VAR").toFloat();

    if (m_dataPoints.value("VARphA").isValid())
        m_varPhaseA = m_dataPoints.value("VARphA").toFloat();

    if (m_dataPoints.value("VARphB").isValid())
        m_varPhaseB = m_dataPoints.value("VARphB").toFloat();

    if (m_dataPoints.value("VARphC").isValid())
        m_varPhaseC = m_dataPoints.value("VARphC").toFloat();

    if (m_dataPoints.value("PF").isValid())
        m_pf = m_dataPoints.value("PF").toFloat();

    if (m_dataPoints.value("PFphA").isValid())
        m_pfPhaseA = m_dataPoints.value("PFphA").toFloat();

    if (m_dataPoints.value("PFphB").isValid())
        m_pfPhaseB = m_dataPoints.value("PFphB").toFloat();

    if (m_dataPoints.value("PFphC").isValid())
        m_pfPhaseC = m_dataPoints.value("PFphC").toFloat();

    if (m_dataPoints.value("TotWhExp").isValid())
        m_totalWattHoursExported = m_dataPoints.value("TotWhExp").toFloat();

    if (m_dataPoints.value("TotWhExpPhA").isValid())
        m_totalWattHoursExportedPhaseA = m_dataPoints.value("TotWhExpPhA").toFloat();

    if (m_dataPoints.value("TotWhExpPhB").isValid())
        m_totalWattHoursExportedPhaseB = m_dataPoints.value("TotWhExpPhB").toFloat();

    if (m_dataPoints.value("TotWhExpPhC").isValid())
        m_totalWattHoursExportedPhaseC = m_dataPoints.value("TotWhExpPhC").toFloat();

    if (m_dataPoints.value("TotWhImp").isValid())
        m_totalWattHoursImported = m_dataPoints.value("TotWhImp").toFloat();

    if (m_dataPoints.value("TotWhImpPhA").isValid())
        m_totalWattHoursImportedPhaseA = m_dataPoints.value("TotWhImpPhA").toFloat();

    if (m_dataPoints.value("TotWhImpPhB").isValid())
        m_totalWattHoursImportedPhaseB = m_dataPoints.value("TotWhImpPhB").toFloat();

    if (m_dataPoints.value("TotWhImpPhC").isValid())
        m_totalWattHoursImportedPhaseC = m_dataPoints.value("TotWhImpPhC").toFloat();

    if (m_dataPoints.value("TotVAhExp").isValid())
        m_totalVaHoursExported = m_dataPoints.value("TotVAhExp").toFloat();

    if (m_dataPoints.value("TotVAhExpPhA").isValid())
        m_totalVaHoursExportedPhaseA = m_dataPoints.value("TotVAhExpPhA").toFloat();

    if (m_dataPoints.value("TotVAhExpPhB").isValid())
        m_totalVaHoursExportedPhaseB = m_dataPoints.value("TotVAhExpPhB").toFloat();

    if (m_dataPoints.value("TotVAhExpPhC").isValid())
        m_totalVaHoursExportedPhaseC = m_dataPoints.value("TotVAhExpPhC").toFloat();

    if (m_dataPoints.value("TotVAhImp").isValid())
        m_totalVaHoursImported = m_dataPoints.value("TotVAhImp").toFloat();

    if (m_dataPoints.value("TotVAhImpPhA").isValid())
        m_totalVaHoursImportedPhaseA = m_dataPoints.value("TotVAhImpPhA").toFloat();

    if (m_dataPoints.value("TotVAhImpPhB").isValid())
        m_totalVaHoursImportedPhaseB = m_dataPoints.value("TotVAhImpPhB").toFloat();

    if (m_dataPoints.value("TotVAhImpPhC").isValid())
        m_totalVaHoursImportedPhaseC = m_dataPoints.value("TotVAhImpPhC").toFloat();

    if (m_dataPoints.value("TotVArhImpQ1").isValid())
        m_totalVarHoursImportedQ1 = m_dataPoints.value("TotVArhImpQ1").toFloat();

    if (m_dataPoints.value("TotVArhImpQ1phA").isValid())
        m_totalVArHoursImportedQ1PhaseA = m_dataPoints.value("TotVArhImpQ1phA").toFloat();

    if (m_dataPoints.value("TotVArhImpQ1phB").isValid())
        m_totalVArHoursImportedQ1PhaseB = m_dataPoints.value("TotVArhImpQ1phB").toFloat();

    if (m_dataPoints.value("TotVArhImpQ1phC").isValid())
        m_totalVArHoursImportedQ1PhaseC = m_dataPoints.value("TotVArhImpQ1phC").toFloat();

    if (m_dataPoints.value("TotVArhImpQ2").isValid())
        m_totalVArHoursImportedQ2 = m_dataPoints.value("TotVArhImpQ2").toFloat();

    if (m_dataPoints.value("TotVArhImpQ2phA").isValid())
        m_totalVArHoursImportedQ2PhaseA = m_dataPoints.value("TotVArhImpQ2phA").toFloat();

    if (m_dataPoints.value("TotVArhImpQ2phB").isValid())
        m_totalVArHoursImportedQ2PhaseB = m_dataPoints.value("TotVArhImpQ2phB").toFloat();

    if (m_dataPoints.value("TotVArhImpQ2phC").isValid())
        m_totalVArHoursImportedQ2PhaseC = m_dataPoints.value("TotVArhImpQ2phC").toFloat();

    if (m_dataPoints.value("TotVArhExpQ3").isValid())
        m_totalVArHoursExportedQ3 = m_dataPoints.value("TotVArhExpQ3").toFloat();

    if (m_dataPoints.value("TotVArhExpQ3phA").isValid())
        m_totalVArHoursExportedQ3PhaseA = m_dataPoints.value("TotVArhExpQ3phA").toFloat();

    if (m_dataPoints.value("TotVArhExpQ3phB").isValid())
        m_totalVArHoursExportedQ3PhaseB = m_dataPoints.value("TotVArhExpQ3phB").toFloat();

    if (m_dataPoints.value("TotVArhExpQ3phC").isValid())
        m_totalVArHoursExportedQ3PhaseC = m_dataPoints.value("TotVArhExpQ3phC").toFloat();

    if (m_dataPoints.value("TotVArhExpQ4").isValid())
        m_totalVArHoursExportedQ4 = m_dataPoints.value("TotVArhExpQ4").toFloat();

    if (m_dataPoints.value("TotVArhExpQ4phA").isValid())
        m_totalVArHoursExportedQ4ImportedPhaseA = m_dataPoints.value("TotVArhExpQ4phA").toFloat();

    if (m_dataPoints.value("TotVArhExpQ4phB").isValid())
        m_totalVArHoursExportedQ4ImportedPhaseB = m_dataPoints.value("TotVArhExpQ4phB").toFloat();

    if (m_dataPoints.value("TotVArhExpQ4phC").isValid())
        m_totalVArHoursExportedQ4ImportedPhaseC = m_dataPoints.value("TotVArhExpQ4phC").toFloat();

    if (m_dataPoints.value("Evt").isValid())
        m_events = static_cast<EvtFlags>(m_dataPoints.value("Evt").toUInt32());


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecMeterSinglePhaseFloatModel *model)
{
    debug.nospace().noquote() << "SunSpecMeterSinglePhaseFloatModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("A") << "-->";
    if (model->dataPoints().value("A").isValid()) {
        debug.nospace().noquote() << model->amps() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("AphA") << "-->";
    if (model->dataPoints().value("AphA").isValid()) {
        debug.nospace().noquote() << model->ampsPhaseA() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("AphB") << "-->";
    if (model->dataPoints().value("AphB").isValid()) {
        debug.nospace().noquote() << model->ampsPhaseB() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("AphC") << "-->";
    if (model->dataPoints().value("AphC").isValid()) {
        debug.nospace().noquote() << model->ampsPhaseC() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("PhV") << "-->";
    if (model->dataPoints().value("PhV").isValid()) {
        debug.nospace().noquote() << model->voltageLn() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("PhVphA") << "-->";
    if (model->dataPoints().value("PhVphA").isValid()) {
        debug.nospace().noquote() << model->phaseVoltageAn() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("PhVphB") << "-->";
    if (model->dataPoints().value("PhVphB").isValid()) {
        debug.nospace().noquote() << model->phaseVoltageBn() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("PhVphC") << "-->";
    if (model->dataPoints().value("PhVphC").isValid()) {
        debug.nospace().noquote() << model->phaseVoltageCn() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("PPV") << "-->";
    if (model->dataPoints().value("PPV").isValid()) {
        debug.nospace().noquote() << model->voltageLl() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("PPVphAB") << "-->";
    if (model->dataPoints().value("PPVphAB").isValid()) {
        debug.nospace().noquote() << model->phaseVoltageAb() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("PPVphBC") << "-->";
    if (model->dataPoints().value("PPVphBC").isValid()) {
        debug.nospace().noquote() << model->phaseVoltageBc() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("PPVphCA") << "-->";
    if (model->dataPoints().value("PPVphCA").isValid()) {
        debug.nospace().noquote() << model->phaseVoltageCa() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Hz") << "-->";
    if (model->dataPoints().value("Hz").isValid()) {
        debug.nospace().noquote() << model->hz() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("W") << "-->";
    if (model->dataPoints().value("W").isValid()) {
        debug.nospace().noquote() << model->watts() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WphA") << "-->";
    if (model->dataPoints().value("WphA").isValid()) {
        debug.nospace().noquote() << model->wattsPhaseA() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WphB") << "-->";
    if (model->dataPoints().value("WphB").isValid()) {
        debug.nospace().noquote() << model->wattsPhaseB() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WphC") << "-->";
    if (model->dataPoints().value("WphC").isValid()) {
        debug.nospace().noquote() << model->wattsPhaseC() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VA") << "-->";
    if (model->dataPoints().value("VA").isValid()) {
        debug.nospace().noquote() << model->va() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VAphA") << "-->";
    if (model->dataPoints().value("VAphA").isValid()) {
        debug.nospace().noquote() << model->vaPhaseA() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VAphB") << "-->";
    if (model->dataPoints().value("VAphB").isValid()) {
        debug.nospace().noquote() << model->vaPhaseB() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VAphC") << "-->";
    if (model->dataPoints().value("VAphC").isValid()) {
        debug.nospace().noquote() << model->vaPhaseC() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VAR") << "-->";
    if (model->dataPoints().value("VAR").isValid()) {
        debug.nospace().noquote() << model->var() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VARphA") << "-->";
    if (model->dataPoints().value("VARphA").isValid()) {
        debug.nospace().noquote() << model->varPhaseA() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VARphB") << "-->";
    if (model->dataPoints().value("VARphB").isValid()) {
        debug.nospace().noquote() << model->varPhaseB() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VARphC") << "-->";
    if (model->dataPoints().value("VARphC").isValid()) {
        debug.nospace().noquote() << model->varPhaseC() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("PF") << "-->";
    if (model->dataPoints().value("PF").isValid()) {
        debug.nospace().noquote() << model->pf() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("PFphA") << "-->";
    if (model->dataPoints().value("PFphA").isValid()) {
        debug.nospace().noquote() << model->pfPhaseA() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("PFphB") << "-->";
    if (model->dataPoints().value("PFphB").isValid()) {
        debug.nospace().noquote() << model->pfPhaseB() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("PFphC") << "-->";
    if (model->dataPoints().value("PFphC").isValid()) {
        debug.nospace().noquote() << model->pfPhaseC() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotWhExp") << "-->";
    if (model->dataPoints().value("TotWhExp").isValid()) {
        debug.nospace().noquote() << model->totalWattHoursExported() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotWhExpPhA") << "-->";
    if (model->dataPoints().value("TotWhExpPhA").isValid()) {
        debug.nospace().noquote() << model->totalWattHoursExportedPhaseA() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotWhExpPhB") << "-->";
    if (model->dataPoints().value("TotWhExpPhB").isValid()) {
        debug.nospace().noquote() << model->totalWattHoursExportedPhaseB() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotWhExpPhC") << "-->";
    if (model->dataPoints().value("TotWhExpPhC").isValid()) {
        debug.nospace().noquote() << model->totalWattHoursExportedPhaseC() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotWhImp") << "-->";
    if (model->dataPoints().value("TotWhImp").isValid()) {
        debug.nospace().noquote() << model->totalWattHoursImported() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotWhImpPhA") << "-->";
    if (model->dataPoints().value("TotWhImpPhA").isValid()) {
        debug.nospace().noquote() << model->totalWattHoursImportedPhaseA() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotWhImpPhB") << "-->";
    if (model->dataPoints().value("TotWhImpPhB").isValid()) {
        debug.nospace().noquote() << model->totalWattHoursImportedPhaseB() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotWhImpPhC") << "-->";
    if (model->dataPoints().value("TotWhImpPhC").isValid()) {
        debug.nospace().noquote() << model->totalWattHoursImportedPhaseC() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVAhExp") << "-->";
    if (model->dataPoints().value("TotVAhExp").isValid()) {
        debug.nospace().noquote() << model->totalVaHoursExported() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVAhExpPhA") << "-->";
    if (model->dataPoints().value("TotVAhExpPhA").isValid()) {
        debug.nospace().noquote() << model->totalVaHoursExportedPhaseA() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVAhExpPhB") << "-->";
    if (model->dataPoints().value("TotVAhExpPhB").isValid()) {
        debug.nospace().noquote() << model->totalVaHoursExportedPhaseB() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVAhExpPhC") << "-->";
    if (model->dataPoints().value("TotVAhExpPhC").isValid()) {
        debug.nospace().noquote() << model->totalVaHoursExportedPhaseC() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVAhImp") << "-->";
    if (model->dataPoints().value("TotVAhImp").isValid()) {
        debug.nospace().noquote() << model->totalVaHoursImported() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVAhImpPhA") << "-->";
    if (model->dataPoints().value("TotVAhImpPhA").isValid()) {
        debug.nospace().noquote() << model->totalVaHoursImportedPhaseA() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVAhImpPhB") << "-->";
    if (model->dataPoints().value("TotVAhImpPhB").isValid()) {
        debug.nospace().noquote() << model->totalVaHoursImportedPhaseB() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVAhImpPhC") << "-->";
    if (model->dataPoints().value("TotVAhImpPhC").isValid()) {
        debug.nospace().noquote() << model->totalVaHoursImportedPhaseC() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhImpQ1") << "-->";
    if (model->dataPoints().value("TotVArhImpQ1").isValid()) {
        debug.nospace().noquote() << model->totalVarHoursImportedQ1() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhImpQ1phA") << "-->";
    if (model->dataPoints().value("TotVArhImpQ1phA").isValid()) {
        debug.nospace().noquote() << model->totalVArHoursImportedQ1PhaseA() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhImpQ1phB") << "-->";
    if (model->dataPoints().value("TotVArhImpQ1phB").isValid()) {
        debug.nospace().noquote() << model->totalVArHoursImportedQ1PhaseB() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhImpQ1phC") << "-->";
    if (model->dataPoints().value("TotVArhImpQ1phC").isValid()) {
        debug.nospace().noquote() << model->totalVArHoursImportedQ1PhaseC() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhImpQ2") << "-->";
    if (model->dataPoints().value("TotVArhImpQ2").isValid()) {
        debug.nospace().noquote() << model->totalVArHoursImportedQ2() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhImpQ2phA") << "-->";
    if (model->dataPoints().value("TotVArhImpQ2phA").isValid()) {
        debug.nospace().noquote() << model->totalVArHoursImportedQ2PhaseA() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhImpQ2phB") << "-->";
    if (model->dataPoints().value("TotVArhImpQ2phB").isValid()) {
        debug.nospace().noquote() << model->totalVArHoursImportedQ2PhaseB() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhImpQ2phC") << "-->";
    if (model->dataPoints().value("TotVArhImpQ2phC").isValid()) {
        debug.nospace().noquote() << model->totalVArHoursImportedQ2PhaseC() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhExpQ3") << "-->";
    if (model->dataPoints().value("TotVArhExpQ3").isValid()) {
        debug.nospace().noquote() << model->totalVArHoursExportedQ3() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhExpQ3phA") << "-->";
    if (model->dataPoints().value("TotVArhExpQ3phA").isValid()) {
        debug.nospace().noquote() << model->totalVArHoursExportedQ3PhaseA() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhExpQ3phB") << "-->";
    if (model->dataPoints().value("TotVArhExpQ3phB").isValid()) {
        debug.nospace().noquote() << model->totalVArHoursExportedQ3PhaseB() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhExpQ3phC") << "-->";
    if (model->dataPoints().value("TotVArhExpQ3phC").isValid()) {
        debug.nospace().noquote() << model->totalVArHoursExportedQ3PhaseC() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhExpQ4") << "-->";
    if (model->dataPoints().value("TotVArhExpQ4").isValid()) {
        debug.nospace().noquote() << model->totalVArHoursExportedQ4() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhExpQ4phA") << "-->";
    if (model->dataPoints().value("TotVArhExpQ4phA").isValid()) {
        debug.nospace().noquote() << model->totalVArHoursExportedQ4ImportedPhaseA() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhExpQ4phB") << "-->";
    if (model->dataPoints().value("TotVArhExpQ4phB").isValid()) {
        debug.nospace().noquote() << model->totalVArHoursExportedQ4ImportedPhaseB() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhExpQ4phC") << "-->";
    if (model->dataPoints().value("TotVArhExpQ4phC").isValid()) {
        debug.nospace().noquote() << model->totalVArHoursExportedQ4ImportedPhaseC() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Evt") << "-->";
    if (model->dataPoints().value("Evt").isValid()) {
        debug.nospace().noquote() << model->events() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
