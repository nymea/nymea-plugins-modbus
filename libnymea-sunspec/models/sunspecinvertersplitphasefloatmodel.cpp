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

#include "sunspecinvertersplitphasefloatmodel.h"
#include "sunspecconnection.h"

SunSpecInverterSplitPhaseFloatModel::SunSpecInverterSplitPhaseFloatModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent) :
    SunSpecModel(connection, 112, 60, modbusStartRegister, parent)
{
    Q_ASSERT_X(length == 60,  "SunSpecInverterSplitPhaseFloatModel", QString("model length %1 given in the constructor does not match the model length from the specs %2.").arg(length).arg(modelLength()).toLatin1());
    Q_UNUSED(length)
    initDataPoints();
}

SunSpecInverterSplitPhaseFloatModel::~SunSpecInverterSplitPhaseFloatModel()
{

}

QString SunSpecInverterSplitPhaseFloatModel::name() const
{
    return "inverter";
}

QString SunSpecInverterSplitPhaseFloatModel::description() const
{
    return "Include this model for split phase inverter monitoring using float values";
}

QString SunSpecInverterSplitPhaseFloatModel::label() const
{
    return "Inverter (Split Phase) FLOAT";
}

float SunSpecInverterSplitPhaseFloatModel::amps() const
{
    return m_amps;
}
float SunSpecInverterSplitPhaseFloatModel::ampsPhaseA() const
{
    return m_ampsPhaseA;
}
float SunSpecInverterSplitPhaseFloatModel::ampsPhaseB() const
{
    return m_ampsPhaseB;
}
float SunSpecInverterSplitPhaseFloatModel::ampsPhaseC() const
{
    return m_ampsPhaseC;
}
float SunSpecInverterSplitPhaseFloatModel::phaseVoltageAb() const
{
    return m_phaseVoltageAb;
}
float SunSpecInverterSplitPhaseFloatModel::phaseVoltageBc() const
{
    return m_phaseVoltageBc;
}
float SunSpecInverterSplitPhaseFloatModel::phaseVoltageCa() const
{
    return m_phaseVoltageCa;
}
float SunSpecInverterSplitPhaseFloatModel::phaseVoltageAn() const
{
    return m_phaseVoltageAn;
}
float SunSpecInverterSplitPhaseFloatModel::phaseVoltageBn() const
{
    return m_phaseVoltageBn;
}
float SunSpecInverterSplitPhaseFloatModel::phaseVoltageCn() const
{
    return m_phaseVoltageCn;
}
float SunSpecInverterSplitPhaseFloatModel::watts() const
{
    return m_watts;
}
float SunSpecInverterSplitPhaseFloatModel::hz() const
{
    return m_hz;
}
float SunSpecInverterSplitPhaseFloatModel::va() const
{
    return m_va;
}
float SunSpecInverterSplitPhaseFloatModel::vAr() const
{
    return m_vAr;
}
float SunSpecInverterSplitPhaseFloatModel::pf() const
{
    return m_pf;
}
float SunSpecInverterSplitPhaseFloatModel::wattHours() const
{
    return m_wattHours;
}
float SunSpecInverterSplitPhaseFloatModel::dcAmps() const
{
    return m_dcAmps;
}
float SunSpecInverterSplitPhaseFloatModel::dcVoltage() const
{
    return m_dcVoltage;
}
float SunSpecInverterSplitPhaseFloatModel::dcWatts() const
{
    return m_dcWatts;
}
float SunSpecInverterSplitPhaseFloatModel::cabinetTemperature() const
{
    return m_cabinetTemperature;
}
float SunSpecInverterSplitPhaseFloatModel::heatSinkTemperature() const
{
    return m_heatSinkTemperature;
}
float SunSpecInverterSplitPhaseFloatModel::transformerTemperature() const
{
    return m_transformerTemperature;
}
float SunSpecInverterSplitPhaseFloatModel::otherTemperature() const
{
    return m_otherTemperature;
}
SunSpecInverterSplitPhaseFloatModel::St SunSpecInverterSplitPhaseFloatModel::operatingState() const
{
    return m_operatingState;
}
quint16 SunSpecInverterSplitPhaseFloatModel::vendorOperatingState() const
{
    return m_vendorOperatingState;
}
SunSpecInverterSplitPhaseFloatModel::Evt1Flags SunSpecInverterSplitPhaseFloatModel::event1() const
{
    return m_event1;
}
quint32 SunSpecInverterSplitPhaseFloatModel::eventBitfield2() const
{
    return m_eventBitfield2;
}
quint32 SunSpecInverterSplitPhaseFloatModel::vendorEventBitfield1() const
{
    return m_vendorEventBitfield1;
}
quint32 SunSpecInverterSplitPhaseFloatModel::vendorEventBitfield2() const
{
    return m_vendorEventBitfield2;
}
quint32 SunSpecInverterSplitPhaseFloatModel::vendorEventBitfield3() const
{
    return m_vendorEventBitfield3;
}
quint32 SunSpecInverterSplitPhaseFloatModel::vendorEventBitfield4() const
{
    return m_vendorEventBitfield4;
}
void SunSpecInverterSplitPhaseFloatModel::processBlockData()
{
    // Update properties according to the data point type
    m_amps = m_dataPoints.value("A").toFloat();
    m_ampsPhaseA = m_dataPoints.value("AphA").toFloat();
    m_ampsPhaseB = m_dataPoints.value("AphB").toFloat();
    m_ampsPhaseC = m_dataPoints.value("AphC").toFloat();
    m_phaseVoltageAb = m_dataPoints.value("PPVphAB").toFloat();
    m_phaseVoltageBc = m_dataPoints.value("PPVphBC").toFloat();
    m_phaseVoltageCa = m_dataPoints.value("PPVphCA").toFloat();
    m_phaseVoltageAn = m_dataPoints.value("PhVphA").toFloat();
    m_phaseVoltageBn = m_dataPoints.value("PhVphB").toFloat();
    m_phaseVoltageCn = m_dataPoints.value("PhVphC").toFloat();
    m_watts = m_dataPoints.value("W").toFloat();
    m_hz = m_dataPoints.value("Hz").toFloat();
    m_va = m_dataPoints.value("VA").toFloat();
    m_vAr = m_dataPoints.value("VAr").toFloat();
    m_pf = m_dataPoints.value("PF").toFloat();
    m_wattHours = m_dataPoints.value("WH").toFloat();
    m_dcAmps = m_dataPoints.value("DCA").toFloat();
    m_dcVoltage = m_dataPoints.value("DCV").toFloat();
    m_dcWatts = m_dataPoints.value("DCW").toFloat();
    m_cabinetTemperature = m_dataPoints.value("TmpCab").toFloat();
    m_heatSinkTemperature = m_dataPoints.value("TmpSnk").toFloat();
    m_transformerTemperature = m_dataPoints.value("TmpTrns").toFloat();
    m_otherTemperature = m_dataPoints.value("TmpOt").toFloat();
    m_operatingState = static_cast<St>(m_dataPoints.value("St").toUInt16());
    m_vendorOperatingState = m_dataPoints.value("StVnd").toUInt16();
    m_event1 = static_cast<Evt1Flags>(m_dataPoints.value("Evt1").toUInt32());
    m_eventBitfield2 = m_dataPoints.value("Evt2").toUInt32();
    m_vendorEventBitfield1 = m_dataPoints.value("EvtVnd1").toUInt32();
    m_vendorEventBitfield2 = m_dataPoints.value("EvtVnd2").toUInt32();
    m_vendorEventBitfield3 = m_dataPoints.value("EvtVnd3").toUInt32();
    m_vendorEventBitfield4 = m_dataPoints.value("EvtVnd4").toUInt32();
}

void SunSpecInverterSplitPhaseFloatModel::initDataPoints()
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

    SunSpecDataPoint ampsDataPoint;
    ampsDataPoint.setName("A");
    ampsDataPoint.setLabel("Amps");
    ampsDataPoint.setDescription("AC Current");
    ampsDataPoint.setUnits("A");
    ampsDataPoint.setMandatory(true);
    ampsDataPoint.setSize(2);
    ampsDataPoint.setAddressOffset(2);
    ampsDataPoint.setBlockOffset(0);
    ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
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
    ampsPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(ampsPhaseADataPoint.name(), ampsPhaseADataPoint);

    SunSpecDataPoint ampsPhaseBDataPoint;
    ampsPhaseBDataPoint.setName("AphB");
    ampsPhaseBDataPoint.setLabel("Amps PhaseB");
    ampsPhaseBDataPoint.setDescription("Phase B Current");
    ampsPhaseBDataPoint.setUnits("A");
    ampsPhaseBDataPoint.setMandatory(true);
    ampsPhaseBDataPoint.setSize(2);
    ampsPhaseBDataPoint.setAddressOffset(6);
    ampsPhaseBDataPoint.setBlockOffset(4);
    ampsPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(ampsPhaseBDataPoint.name(), ampsPhaseBDataPoint);

    SunSpecDataPoint ampsPhaseCDataPoint;
    ampsPhaseCDataPoint.setName("AphC");
    ampsPhaseCDataPoint.setLabel("Amps PhaseC");
    ampsPhaseCDataPoint.setDescription("Phase C Current");
    ampsPhaseCDataPoint.setUnits("A");
    ampsPhaseCDataPoint.setSize(2);
    ampsPhaseCDataPoint.setAddressOffset(8);
    ampsPhaseCDataPoint.setBlockOffset(6);
    ampsPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(ampsPhaseCDataPoint.name(), ampsPhaseCDataPoint);

    SunSpecDataPoint phaseVoltageAbDataPoint;
    phaseVoltageAbDataPoint.setName("PPVphAB");
    phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
    phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
    phaseVoltageAbDataPoint.setUnits("V");
    phaseVoltageAbDataPoint.setSize(2);
    phaseVoltageAbDataPoint.setAddressOffset(10);
    phaseVoltageAbDataPoint.setBlockOffset(8);
    phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(phaseVoltageAbDataPoint.name(), phaseVoltageAbDataPoint);

    SunSpecDataPoint phaseVoltageBcDataPoint;
    phaseVoltageBcDataPoint.setName("PPVphBC");
    phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
    phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
    phaseVoltageBcDataPoint.setUnits("V");
    phaseVoltageBcDataPoint.setSize(2);
    phaseVoltageBcDataPoint.setAddressOffset(12);
    phaseVoltageBcDataPoint.setBlockOffset(10);
    phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(phaseVoltageBcDataPoint.name(), phaseVoltageBcDataPoint);

    SunSpecDataPoint phaseVoltageCaDataPoint;
    phaseVoltageCaDataPoint.setName("PPVphCA");
    phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
    phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
    phaseVoltageCaDataPoint.setUnits("V");
    phaseVoltageCaDataPoint.setSize(2);
    phaseVoltageCaDataPoint.setAddressOffset(14);
    phaseVoltageCaDataPoint.setBlockOffset(12);
    phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(phaseVoltageCaDataPoint.name(), phaseVoltageCaDataPoint);

    SunSpecDataPoint phaseVoltageAnDataPoint;
    phaseVoltageAnDataPoint.setName("PhVphA");
    phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
    phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
    phaseVoltageAnDataPoint.setUnits("V");
    phaseVoltageAnDataPoint.setMandatory(true);
    phaseVoltageAnDataPoint.setSize(2);
    phaseVoltageAnDataPoint.setAddressOffset(16);
    phaseVoltageAnDataPoint.setBlockOffset(14);
    phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(phaseVoltageAnDataPoint.name(), phaseVoltageAnDataPoint);

    SunSpecDataPoint phaseVoltageBnDataPoint;
    phaseVoltageBnDataPoint.setName("PhVphB");
    phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
    phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
    phaseVoltageBnDataPoint.setUnits("V");
    phaseVoltageBnDataPoint.setMandatory(true);
    phaseVoltageBnDataPoint.setSize(2);
    phaseVoltageBnDataPoint.setAddressOffset(18);
    phaseVoltageBnDataPoint.setBlockOffset(16);
    phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(phaseVoltageBnDataPoint.name(), phaseVoltageBnDataPoint);

    SunSpecDataPoint phaseVoltageCnDataPoint;
    phaseVoltageCnDataPoint.setName("PhVphC");
    phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
    phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
    phaseVoltageCnDataPoint.setUnits("V");
    phaseVoltageCnDataPoint.setSize(2);
    phaseVoltageCnDataPoint.setAddressOffset(20);
    phaseVoltageCnDataPoint.setBlockOffset(18);
    phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(phaseVoltageCnDataPoint.name(), phaseVoltageCnDataPoint);

    SunSpecDataPoint wattsDataPoint;
    wattsDataPoint.setName("W");
    wattsDataPoint.setLabel("Watts");
    wattsDataPoint.setDescription("AC Power");
    wattsDataPoint.setUnits("W");
    wattsDataPoint.setMandatory(true);
    wattsDataPoint.setSize(2);
    wattsDataPoint.setAddressOffset(22);
    wattsDataPoint.setBlockOffset(20);
    wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(wattsDataPoint.name(), wattsDataPoint);

    SunSpecDataPoint hzDataPoint;
    hzDataPoint.setName("Hz");
    hzDataPoint.setLabel("Hz");
    hzDataPoint.setDescription("Line Frequency");
    hzDataPoint.setUnits("Hz");
    hzDataPoint.setMandatory(true);
    hzDataPoint.setSize(2);
    hzDataPoint.setAddressOffset(24);
    hzDataPoint.setBlockOffset(22);
    hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(hzDataPoint.name(), hzDataPoint);

    SunSpecDataPoint vaDataPoint;
    vaDataPoint.setName("VA");
    vaDataPoint.setLabel("VA");
    vaDataPoint.setDescription("AC Apparent Power");
    vaDataPoint.setUnits("VA");
    vaDataPoint.setSize(2);
    vaDataPoint.setAddressOffset(26);
    vaDataPoint.setBlockOffset(24);
    vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(vaDataPoint.name(), vaDataPoint);

    SunSpecDataPoint vArDataPoint;
    vArDataPoint.setName("VAr");
    vArDataPoint.setLabel("VAr");
    vArDataPoint.setDescription("AC Reactive Power");
    vArDataPoint.setUnits("var");
    vArDataPoint.setSize(2);
    vArDataPoint.setAddressOffset(28);
    vArDataPoint.setBlockOffset(26);
    vArDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(vArDataPoint.name(), vArDataPoint);

    SunSpecDataPoint pfDataPoint;
    pfDataPoint.setName("PF");
    pfDataPoint.setLabel("PF");
    pfDataPoint.setDescription("AC Power Factor");
    pfDataPoint.setUnits("Pct");
    pfDataPoint.setSize(2);
    pfDataPoint.setAddressOffset(30);
    pfDataPoint.setBlockOffset(28);
    pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(pfDataPoint.name(), pfDataPoint);

    SunSpecDataPoint wattHoursDataPoint;
    wattHoursDataPoint.setName("WH");
    wattHoursDataPoint.setLabel("WattHours");
    wattHoursDataPoint.setDescription("AC Energy");
    wattHoursDataPoint.setUnits("Wh");
    wattHoursDataPoint.setMandatory(true);
    wattHoursDataPoint.setSize(2);
    wattHoursDataPoint.setAddressOffset(32);
    wattHoursDataPoint.setBlockOffset(30);
    wattHoursDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(wattHoursDataPoint.name(), wattHoursDataPoint);

    SunSpecDataPoint dcAmpsDataPoint;
    dcAmpsDataPoint.setName("DCA");
    dcAmpsDataPoint.setLabel("DC Amps");
    dcAmpsDataPoint.setDescription("DC Current");
    dcAmpsDataPoint.setUnits("A");
    dcAmpsDataPoint.setSize(2);
    dcAmpsDataPoint.setAddressOffset(34);
    dcAmpsDataPoint.setBlockOffset(32);
    dcAmpsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(dcAmpsDataPoint.name(), dcAmpsDataPoint);

    SunSpecDataPoint dcVoltageDataPoint;
    dcVoltageDataPoint.setName("DCV");
    dcVoltageDataPoint.setLabel("DC Voltage");
    dcVoltageDataPoint.setDescription("DC Voltage");
    dcVoltageDataPoint.setUnits("V");
    dcVoltageDataPoint.setSize(2);
    dcVoltageDataPoint.setAddressOffset(36);
    dcVoltageDataPoint.setBlockOffset(34);
    dcVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(dcVoltageDataPoint.name(), dcVoltageDataPoint);

    SunSpecDataPoint dcWattsDataPoint;
    dcWattsDataPoint.setName("DCW");
    dcWattsDataPoint.setLabel("DC Watts");
    dcWattsDataPoint.setDescription("DC Power");
    dcWattsDataPoint.setUnits("W");
    dcWattsDataPoint.setSize(2);
    dcWattsDataPoint.setAddressOffset(38);
    dcWattsDataPoint.setBlockOffset(36);
    dcWattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(dcWattsDataPoint.name(), dcWattsDataPoint);

    SunSpecDataPoint cabinetTemperatureDataPoint;
    cabinetTemperatureDataPoint.setName("TmpCab");
    cabinetTemperatureDataPoint.setLabel("Cabinet Temperature");
    cabinetTemperatureDataPoint.setDescription("Cabinet Temperature");
    cabinetTemperatureDataPoint.setUnits("C");
    cabinetTemperatureDataPoint.setMandatory(true);
    cabinetTemperatureDataPoint.setSize(2);
    cabinetTemperatureDataPoint.setAddressOffset(40);
    cabinetTemperatureDataPoint.setBlockOffset(38);
    cabinetTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(cabinetTemperatureDataPoint.name(), cabinetTemperatureDataPoint);

    SunSpecDataPoint heatSinkTemperatureDataPoint;
    heatSinkTemperatureDataPoint.setName("TmpSnk");
    heatSinkTemperatureDataPoint.setLabel("Heat Sink Temperature");
    heatSinkTemperatureDataPoint.setDescription("Heat Sink Temperature");
    heatSinkTemperatureDataPoint.setUnits("C");
    heatSinkTemperatureDataPoint.setSize(2);
    heatSinkTemperatureDataPoint.setAddressOffset(42);
    heatSinkTemperatureDataPoint.setBlockOffset(40);
    heatSinkTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(heatSinkTemperatureDataPoint.name(), heatSinkTemperatureDataPoint);

    SunSpecDataPoint transformerTemperatureDataPoint;
    transformerTemperatureDataPoint.setName("TmpTrns");
    transformerTemperatureDataPoint.setLabel("Transformer Temperature");
    transformerTemperatureDataPoint.setDescription("Transformer Temperature");
    transformerTemperatureDataPoint.setUnits("C");
    transformerTemperatureDataPoint.setSize(2);
    transformerTemperatureDataPoint.setAddressOffset(44);
    transformerTemperatureDataPoint.setBlockOffset(42);
    transformerTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(transformerTemperatureDataPoint.name(), transformerTemperatureDataPoint);

    SunSpecDataPoint otherTemperatureDataPoint;
    otherTemperatureDataPoint.setName("TmpOt");
    otherTemperatureDataPoint.setLabel("Other Temperature");
    otherTemperatureDataPoint.setDescription("Other Temperature");
    otherTemperatureDataPoint.setUnits("C");
    otherTemperatureDataPoint.setSize(2);
    otherTemperatureDataPoint.setAddressOffset(46);
    otherTemperatureDataPoint.setBlockOffset(44);
    otherTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
    m_dataPoints.insert(otherTemperatureDataPoint.name(), otherTemperatureDataPoint);

    SunSpecDataPoint operatingStateDataPoint;
    operatingStateDataPoint.setName("St");
    operatingStateDataPoint.setLabel("Operating State");
    operatingStateDataPoint.setDescription("Enumerated value.  Operating state");
    operatingStateDataPoint.setMandatory(true);
    operatingStateDataPoint.setSize(1);
    operatingStateDataPoint.setAddressOffset(48);
    operatingStateDataPoint.setBlockOffset(46);
    operatingStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    m_dataPoints.insert(operatingStateDataPoint.name(), operatingStateDataPoint);

    SunSpecDataPoint vendorOperatingStateDataPoint;
    vendorOperatingStateDataPoint.setName("StVnd");
    vendorOperatingStateDataPoint.setLabel("Vendor Operating State");
    vendorOperatingStateDataPoint.setDescription("Vendor specific operating state code");
    vendorOperatingStateDataPoint.setSize(1);
    vendorOperatingStateDataPoint.setAddressOffset(49);
    vendorOperatingStateDataPoint.setBlockOffset(47);
    vendorOperatingStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    m_dataPoints.insert(vendorOperatingStateDataPoint.name(), vendorOperatingStateDataPoint);

    SunSpecDataPoint event1DataPoint;
    event1DataPoint.setName("Evt1");
    event1DataPoint.setLabel("Event1");
    event1DataPoint.setDescription("Bitmask value. Event fields");
    event1DataPoint.setMandatory(true);
    event1DataPoint.setSize(2);
    event1DataPoint.setAddressOffset(50);
    event1DataPoint.setBlockOffset(48);
    event1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(event1DataPoint.name(), event1DataPoint);

    SunSpecDataPoint eventBitfield2DataPoint;
    eventBitfield2DataPoint.setName("Evt2");
    eventBitfield2DataPoint.setLabel("Event Bitfield 2");
    eventBitfield2DataPoint.setDescription("Reserved for future use");
    eventBitfield2DataPoint.setMandatory(true);
    eventBitfield2DataPoint.setSize(2);
    eventBitfield2DataPoint.setAddressOffset(52);
    eventBitfield2DataPoint.setBlockOffset(50);
    eventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(eventBitfield2DataPoint.name(), eventBitfield2DataPoint);

    SunSpecDataPoint vendorEventBitfield1DataPoint;
    vendorEventBitfield1DataPoint.setName("EvtVnd1");
    vendorEventBitfield1DataPoint.setLabel("Vendor Event Bitfield 1");
    vendorEventBitfield1DataPoint.setDescription("Vendor defined events");
    vendorEventBitfield1DataPoint.setSize(2);
    vendorEventBitfield1DataPoint.setAddressOffset(54);
    vendorEventBitfield1DataPoint.setBlockOffset(52);
    vendorEventBitfield1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(vendorEventBitfield1DataPoint.name(), vendorEventBitfield1DataPoint);

    SunSpecDataPoint vendorEventBitfield2DataPoint;
    vendorEventBitfield2DataPoint.setName("EvtVnd2");
    vendorEventBitfield2DataPoint.setLabel("Vendor Event Bitfield 2");
    vendorEventBitfield2DataPoint.setDescription("Vendor defined events");
    vendorEventBitfield2DataPoint.setSize(2);
    vendorEventBitfield2DataPoint.setAddressOffset(56);
    vendorEventBitfield2DataPoint.setBlockOffset(54);
    vendorEventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(vendorEventBitfield2DataPoint.name(), vendorEventBitfield2DataPoint);

    SunSpecDataPoint vendorEventBitfield3DataPoint;
    vendorEventBitfield3DataPoint.setName("EvtVnd3");
    vendorEventBitfield3DataPoint.setLabel("Vendor Event Bitfield 3");
    vendorEventBitfield3DataPoint.setDescription("Vendor defined events");
    vendorEventBitfield3DataPoint.setSize(2);
    vendorEventBitfield3DataPoint.setAddressOffset(58);
    vendorEventBitfield3DataPoint.setBlockOffset(56);
    vendorEventBitfield3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(vendorEventBitfield3DataPoint.name(), vendorEventBitfield3DataPoint);

    SunSpecDataPoint vendorEventBitfield4DataPoint;
    vendorEventBitfield4DataPoint.setName("EvtVnd4");
    vendorEventBitfield4DataPoint.setLabel("Vendor Event Bitfield 4");
    vendorEventBitfield4DataPoint.setDescription("Vendor defined events");
    vendorEventBitfield4DataPoint.setSize(2);
    vendorEventBitfield4DataPoint.setAddressOffset(60);
    vendorEventBitfield4DataPoint.setBlockOffset(58);
    vendorEventBitfield4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(vendorEventBitfield4DataPoint.name(), vendorEventBitfield4DataPoint);

}

