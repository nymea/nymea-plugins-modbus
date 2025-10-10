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

#include "sunspecinvertersinglephasefloatmodel.h"
#include "sunspecconnection.h"

SunSpecInverterSinglePhaseFloatModel::SunSpecInverterSinglePhaseFloatModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 111, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixed;

    initDataPoints();
}

SunSpecInverterSinglePhaseFloatModel::~SunSpecInverterSinglePhaseFloatModel()
{

}

QString SunSpecInverterSinglePhaseFloatModel::name() const
{
    return "inverter";
}

QString SunSpecInverterSinglePhaseFloatModel::description() const
{
    return "Include this model for single phase inverter monitoring using float values";
}

QString SunSpecInverterSinglePhaseFloatModel::label() const
{
    return "Inverter (Single Phase) FLOAT";
}

float SunSpecInverterSinglePhaseFloatModel::amps() const
{
    return m_amps;
}
float SunSpecInverterSinglePhaseFloatModel::ampsPhaseA() const
{
    return m_ampsPhaseA;
}
float SunSpecInverterSinglePhaseFloatModel::ampsPhaseB() const
{
    return m_ampsPhaseB;
}
float SunSpecInverterSinglePhaseFloatModel::ampsPhaseC() const
{
    return m_ampsPhaseC;
}
float SunSpecInverterSinglePhaseFloatModel::phaseVoltageAb() const
{
    return m_phaseVoltageAb;
}
float SunSpecInverterSinglePhaseFloatModel::phaseVoltageBc() const
{
    return m_phaseVoltageBc;
}
float SunSpecInverterSinglePhaseFloatModel::phaseVoltageCa() const
{
    return m_phaseVoltageCa;
}
float SunSpecInverterSinglePhaseFloatModel::phaseVoltageAn() const
{
    return m_phaseVoltageAn;
}
float SunSpecInverterSinglePhaseFloatModel::phaseVoltageBn() const
{
    return m_phaseVoltageBn;
}
float SunSpecInverterSinglePhaseFloatModel::phaseVoltageCn() const
{
    return m_phaseVoltageCn;
}
float SunSpecInverterSinglePhaseFloatModel::watts() const
{
    return m_watts;
}
float SunSpecInverterSinglePhaseFloatModel::hz() const
{
    return m_hz;
}
float SunSpecInverterSinglePhaseFloatModel::va() const
{
    return m_va;
}
float SunSpecInverterSinglePhaseFloatModel::vAr() const
{
    return m_vAr;
}
float SunSpecInverterSinglePhaseFloatModel::pf() const
{
    return m_pf;
}
float SunSpecInverterSinglePhaseFloatModel::wattHours() const
{
    return m_wattHours;
}
float SunSpecInverterSinglePhaseFloatModel::dcAmps() const
{
    return m_dcAmps;
}
float SunSpecInverterSinglePhaseFloatModel::dcVoltage() const
{
    return m_dcVoltage;
}
float SunSpecInverterSinglePhaseFloatModel::dcWatts() const
{
    return m_dcWatts;
}
float SunSpecInverterSinglePhaseFloatModel::cabinetTemperature() const
{
    return m_cabinetTemperature;
}
float SunSpecInverterSinglePhaseFloatModel::heatSinkTemperature() const
{
    return m_heatSinkTemperature;
}
float SunSpecInverterSinglePhaseFloatModel::transformerTemperature() const
{
    return m_transformerTemperature;
}
float SunSpecInverterSinglePhaseFloatModel::otherTemperature() const
{
    return m_otherTemperature;
}
SunSpecInverterSinglePhaseFloatModel::St SunSpecInverterSinglePhaseFloatModel::operatingState() const
{
    return m_operatingState;
}
quint16 SunSpecInverterSinglePhaseFloatModel::vendorOperatingState() const
{
    return m_vendorOperatingState;
}
SunSpecInverterSinglePhaseFloatModel::Evt1Flags SunSpecInverterSinglePhaseFloatModel::event1() const
{
    return m_event1;
}
quint32 SunSpecInverterSinglePhaseFloatModel::eventBitfield2() const
{
    return m_eventBitfield2;
}
quint32 SunSpecInverterSinglePhaseFloatModel::vendorEventBitfield1() const
{
    return m_vendorEventBitfield1;
}
quint32 SunSpecInverterSinglePhaseFloatModel::vendorEventBitfield2() const
{
    return m_vendorEventBitfield2;
}
quint32 SunSpecInverterSinglePhaseFloatModel::vendorEventBitfield3() const
{
    return m_vendorEventBitfield3;
}
quint32 SunSpecInverterSinglePhaseFloatModel::vendorEventBitfield4() const
{
    return m_vendorEventBitfield4;
}
void SunSpecInverterSinglePhaseFloatModel::initDataPoints()
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
    ampsDataPoint.setDescription("AC Current");
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

    SunSpecDataPoint phaseVoltageAbDataPoint;
    phaseVoltageAbDataPoint.setName("PPVphAB");
    phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
    phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
    phaseVoltageAbDataPoint.setUnits("V");
    phaseVoltageAbDataPoint.setSize(2);
    phaseVoltageAbDataPoint.setAddressOffset(10);
    phaseVoltageAbDataPoint.setBlockOffset(8);
    phaseVoltageAbDataPoint.setSunSpecDataType("float32");
    phaseVoltageAbDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(phaseVoltageAbDataPoint.name(), phaseVoltageAbDataPoint);

    SunSpecDataPoint phaseVoltageBcDataPoint;
    phaseVoltageBcDataPoint.setName("PPVphBC");
    phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
    phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
    phaseVoltageBcDataPoint.setUnits("V");
    phaseVoltageBcDataPoint.setSize(2);
    phaseVoltageBcDataPoint.setAddressOffset(12);
    phaseVoltageBcDataPoint.setBlockOffset(10);
    phaseVoltageBcDataPoint.setSunSpecDataType("float32");
    phaseVoltageBcDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(phaseVoltageBcDataPoint.name(), phaseVoltageBcDataPoint);

    SunSpecDataPoint phaseVoltageCaDataPoint;
    phaseVoltageCaDataPoint.setName("PPVphCA");
    phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
    phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
    phaseVoltageCaDataPoint.setUnits("V");
    phaseVoltageCaDataPoint.setSize(2);
    phaseVoltageCaDataPoint.setAddressOffset(14);
    phaseVoltageCaDataPoint.setBlockOffset(12);
    phaseVoltageCaDataPoint.setSunSpecDataType("float32");
    phaseVoltageCaDataPoint.setByteOrder(m_byteOrder);
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
    phaseVoltageAnDataPoint.setSunSpecDataType("float32");
    phaseVoltageAnDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(phaseVoltageAnDataPoint.name(), phaseVoltageAnDataPoint);

    SunSpecDataPoint phaseVoltageBnDataPoint;
    phaseVoltageBnDataPoint.setName("PhVphB");
    phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
    phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
    phaseVoltageBnDataPoint.setUnits("V");
    phaseVoltageBnDataPoint.setSize(2);
    phaseVoltageBnDataPoint.setAddressOffset(18);
    phaseVoltageBnDataPoint.setBlockOffset(16);
    phaseVoltageBnDataPoint.setSunSpecDataType("float32");
    phaseVoltageBnDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(phaseVoltageBnDataPoint.name(), phaseVoltageBnDataPoint);

    SunSpecDataPoint phaseVoltageCnDataPoint;
    phaseVoltageCnDataPoint.setName("PhVphC");
    phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
    phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
    phaseVoltageCnDataPoint.setUnits("V");
    phaseVoltageCnDataPoint.setSize(2);
    phaseVoltageCnDataPoint.setAddressOffset(20);
    phaseVoltageCnDataPoint.setBlockOffset(18);
    phaseVoltageCnDataPoint.setSunSpecDataType("float32");
    phaseVoltageCnDataPoint.setByteOrder(m_byteOrder);
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
    wattsDataPoint.setSunSpecDataType("float32");
    wattsDataPoint.setByteOrder(m_byteOrder);
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
    hzDataPoint.setSunSpecDataType("float32");
    hzDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(hzDataPoint.name(), hzDataPoint);

    SunSpecDataPoint vaDataPoint;
    vaDataPoint.setName("VA");
    vaDataPoint.setLabel("VA");
    vaDataPoint.setDescription("AC Apparent Power");
    vaDataPoint.setUnits("VA");
    vaDataPoint.setSize(2);
    vaDataPoint.setAddressOffset(26);
    vaDataPoint.setBlockOffset(24);
    vaDataPoint.setSunSpecDataType("float32");
    vaDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vaDataPoint.name(), vaDataPoint);

    SunSpecDataPoint vArDataPoint;
    vArDataPoint.setName("VAr");
    vArDataPoint.setLabel("VAr");
    vArDataPoint.setDescription("AC Reactive Power");
    vArDataPoint.setUnits("var");
    vArDataPoint.setSize(2);
    vArDataPoint.setAddressOffset(28);
    vArDataPoint.setBlockOffset(26);
    vArDataPoint.setSunSpecDataType("float32");
    vArDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vArDataPoint.name(), vArDataPoint);

    SunSpecDataPoint pfDataPoint;
    pfDataPoint.setName("PF");
    pfDataPoint.setLabel("PF");
    pfDataPoint.setDescription("AC Power Factor");
    pfDataPoint.setUnits("Pct");
    pfDataPoint.setSize(2);
    pfDataPoint.setAddressOffset(30);
    pfDataPoint.setBlockOffset(28);
    pfDataPoint.setSunSpecDataType("float32");
    pfDataPoint.setByteOrder(m_byteOrder);
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
    wattHoursDataPoint.setSunSpecDataType("float32");
    wattHoursDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wattHoursDataPoint.name(), wattHoursDataPoint);

    SunSpecDataPoint dcAmpsDataPoint;
    dcAmpsDataPoint.setName("DCA");
    dcAmpsDataPoint.setLabel("DC Amps");
    dcAmpsDataPoint.setDescription("DC Current");
    dcAmpsDataPoint.setUnits("A");
    dcAmpsDataPoint.setSize(2);
    dcAmpsDataPoint.setAddressOffset(34);
    dcAmpsDataPoint.setBlockOffset(32);
    dcAmpsDataPoint.setSunSpecDataType("float32");
    dcAmpsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(dcAmpsDataPoint.name(), dcAmpsDataPoint);

    SunSpecDataPoint dcVoltageDataPoint;
    dcVoltageDataPoint.setName("DCV");
    dcVoltageDataPoint.setLabel("DC Voltage");
    dcVoltageDataPoint.setDescription("DC Voltage");
    dcVoltageDataPoint.setUnits("V");
    dcVoltageDataPoint.setSize(2);
    dcVoltageDataPoint.setAddressOffset(36);
    dcVoltageDataPoint.setBlockOffset(34);
    dcVoltageDataPoint.setSunSpecDataType("float32");
    dcVoltageDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(dcVoltageDataPoint.name(), dcVoltageDataPoint);

    SunSpecDataPoint dcWattsDataPoint;
    dcWattsDataPoint.setName("DCW");
    dcWattsDataPoint.setLabel("DC Watts");
    dcWattsDataPoint.setDescription("DC Power");
    dcWattsDataPoint.setUnits("W");
    dcWattsDataPoint.setSize(2);
    dcWattsDataPoint.setAddressOffset(38);
    dcWattsDataPoint.setBlockOffset(36);
    dcWattsDataPoint.setSunSpecDataType("float32");
    dcWattsDataPoint.setByteOrder(m_byteOrder);
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
    cabinetTemperatureDataPoint.setSunSpecDataType("float32");
    cabinetTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(cabinetTemperatureDataPoint.name(), cabinetTemperatureDataPoint);

    SunSpecDataPoint heatSinkTemperatureDataPoint;
    heatSinkTemperatureDataPoint.setName("TmpSnk");
    heatSinkTemperatureDataPoint.setLabel("Heat Sink Temperature");
    heatSinkTemperatureDataPoint.setDescription("Heat Sink Temperature");
    heatSinkTemperatureDataPoint.setUnits("C");
    heatSinkTemperatureDataPoint.setSize(2);
    heatSinkTemperatureDataPoint.setAddressOffset(42);
    heatSinkTemperatureDataPoint.setBlockOffset(40);
    heatSinkTemperatureDataPoint.setSunSpecDataType("float32");
    heatSinkTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(heatSinkTemperatureDataPoint.name(), heatSinkTemperatureDataPoint);

    SunSpecDataPoint transformerTemperatureDataPoint;
    transformerTemperatureDataPoint.setName("TmpTrns");
    transformerTemperatureDataPoint.setLabel("Transformer Temperature");
    transformerTemperatureDataPoint.setDescription("Transformer Temperature");
    transformerTemperatureDataPoint.setUnits("C");
    transformerTemperatureDataPoint.setSize(2);
    transformerTemperatureDataPoint.setAddressOffset(44);
    transformerTemperatureDataPoint.setBlockOffset(42);
    transformerTemperatureDataPoint.setSunSpecDataType("float32");
    transformerTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(transformerTemperatureDataPoint.name(), transformerTemperatureDataPoint);

    SunSpecDataPoint otherTemperatureDataPoint;
    otherTemperatureDataPoint.setName("TmpOt");
    otherTemperatureDataPoint.setLabel("Other Temperature");
    otherTemperatureDataPoint.setDescription("Other Temperature");
    otherTemperatureDataPoint.setUnits("C");
    otherTemperatureDataPoint.setSize(2);
    otherTemperatureDataPoint.setAddressOffset(46);
    otherTemperatureDataPoint.setBlockOffset(44);
    otherTemperatureDataPoint.setSunSpecDataType("float32");
    otherTemperatureDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(otherTemperatureDataPoint.name(), otherTemperatureDataPoint);

    SunSpecDataPoint operatingStateDataPoint;
    operatingStateDataPoint.setName("St");
    operatingStateDataPoint.setLabel("Operating State");
    operatingStateDataPoint.setDescription("Enumerated value.  Operating state");
    operatingStateDataPoint.setMandatory(true);
    operatingStateDataPoint.setSize(1);
    operatingStateDataPoint.setAddressOffset(48);
    operatingStateDataPoint.setBlockOffset(46);
    operatingStateDataPoint.setSunSpecDataType("enum16");
    operatingStateDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(operatingStateDataPoint.name(), operatingStateDataPoint);

    SunSpecDataPoint vendorOperatingStateDataPoint;
    vendorOperatingStateDataPoint.setName("StVnd");
    vendorOperatingStateDataPoint.setLabel("Vendor Operating State");
    vendorOperatingStateDataPoint.setDescription("Vendor specific operating state code");
    vendorOperatingStateDataPoint.setSize(1);
    vendorOperatingStateDataPoint.setAddressOffset(49);
    vendorOperatingStateDataPoint.setBlockOffset(47);
    vendorOperatingStateDataPoint.setSunSpecDataType("enum16");
    vendorOperatingStateDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vendorOperatingStateDataPoint.name(), vendorOperatingStateDataPoint);

    SunSpecDataPoint event1DataPoint;
    event1DataPoint.setName("Evt1");
    event1DataPoint.setLabel("Event1");
    event1DataPoint.setDescription("Bitmask value. Event fields");
    event1DataPoint.setMandatory(true);
    event1DataPoint.setSize(2);
    event1DataPoint.setAddressOffset(50);
    event1DataPoint.setBlockOffset(48);
    event1DataPoint.setSunSpecDataType("bitfield32");
    event1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(event1DataPoint.name(), event1DataPoint);

    SunSpecDataPoint eventBitfield2DataPoint;
    eventBitfield2DataPoint.setName("Evt2");
    eventBitfield2DataPoint.setLabel("Event Bitfield 2");
    eventBitfield2DataPoint.setDescription("Reserved for future use");
    eventBitfield2DataPoint.setMandatory(true);
    eventBitfield2DataPoint.setSize(2);
    eventBitfield2DataPoint.setAddressOffset(52);
    eventBitfield2DataPoint.setBlockOffset(50);
    eventBitfield2DataPoint.setSunSpecDataType("bitfield32");
    eventBitfield2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(eventBitfield2DataPoint.name(), eventBitfield2DataPoint);

    SunSpecDataPoint vendorEventBitfield1DataPoint;
    vendorEventBitfield1DataPoint.setName("EvtVnd1");
    vendorEventBitfield1DataPoint.setLabel("Vendor Event Bitfield 1");
    vendorEventBitfield1DataPoint.setDescription("Vendor defined events");
    vendorEventBitfield1DataPoint.setSize(2);
    vendorEventBitfield1DataPoint.setAddressOffset(54);
    vendorEventBitfield1DataPoint.setBlockOffset(52);
    vendorEventBitfield1DataPoint.setSunSpecDataType("bitfield32");
    vendorEventBitfield1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vendorEventBitfield1DataPoint.name(), vendorEventBitfield1DataPoint);

    SunSpecDataPoint vendorEventBitfield2DataPoint;
    vendorEventBitfield2DataPoint.setName("EvtVnd2");
    vendorEventBitfield2DataPoint.setLabel("Vendor Event Bitfield 2");
    vendorEventBitfield2DataPoint.setDescription("Vendor defined events");
    vendorEventBitfield2DataPoint.setSize(2);
    vendorEventBitfield2DataPoint.setAddressOffset(56);
    vendorEventBitfield2DataPoint.setBlockOffset(54);
    vendorEventBitfield2DataPoint.setSunSpecDataType("bitfield32");
    vendorEventBitfield2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vendorEventBitfield2DataPoint.name(), vendorEventBitfield2DataPoint);

    SunSpecDataPoint vendorEventBitfield3DataPoint;
    vendorEventBitfield3DataPoint.setName("EvtVnd3");
    vendorEventBitfield3DataPoint.setLabel("Vendor Event Bitfield 3");
    vendorEventBitfield3DataPoint.setDescription("Vendor defined events");
    vendorEventBitfield3DataPoint.setSize(2);
    vendorEventBitfield3DataPoint.setAddressOffset(58);
    vendorEventBitfield3DataPoint.setBlockOffset(56);
    vendorEventBitfield3DataPoint.setSunSpecDataType("bitfield32");
    vendorEventBitfield3DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vendorEventBitfield3DataPoint.name(), vendorEventBitfield3DataPoint);

    SunSpecDataPoint vendorEventBitfield4DataPoint;
    vendorEventBitfield4DataPoint.setName("EvtVnd4");
    vendorEventBitfield4DataPoint.setLabel("Vendor Event Bitfield 4");
    vendorEventBitfield4DataPoint.setDescription("Vendor defined events");
    vendorEventBitfield4DataPoint.setSize(2);
    vendorEventBitfield4DataPoint.setAddressOffset(60);
    vendorEventBitfield4DataPoint.setBlockOffset(58);
    vendorEventBitfield4DataPoint.setSunSpecDataType("bitfield32");
    vendorEventBitfield4DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vendorEventBitfield4DataPoint.name(), vendorEventBitfield4DataPoint);

}

void SunSpecInverterSinglePhaseFloatModel::processBlockData()
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

    if (m_dataPoints.value("PPVphAB").isValid())
        m_phaseVoltageAb = m_dataPoints.value("PPVphAB").toFloat();

    if (m_dataPoints.value("PPVphBC").isValid())
        m_phaseVoltageBc = m_dataPoints.value("PPVphBC").toFloat();

    if (m_dataPoints.value("PPVphCA").isValid())
        m_phaseVoltageCa = m_dataPoints.value("PPVphCA").toFloat();

    if (m_dataPoints.value("PhVphA").isValid())
        m_phaseVoltageAn = m_dataPoints.value("PhVphA").toFloat();

    if (m_dataPoints.value("PhVphB").isValid())
        m_phaseVoltageBn = m_dataPoints.value("PhVphB").toFloat();

    if (m_dataPoints.value("PhVphC").isValid())
        m_phaseVoltageCn = m_dataPoints.value("PhVphC").toFloat();

    if (m_dataPoints.value("W").isValid())
        m_watts = m_dataPoints.value("W").toFloat();

    if (m_dataPoints.value("Hz").isValid())
        m_hz = m_dataPoints.value("Hz").toFloat();

    if (m_dataPoints.value("VA").isValid())
        m_va = m_dataPoints.value("VA").toFloat();

    if (m_dataPoints.value("VAr").isValid())
        m_vAr = m_dataPoints.value("VAr").toFloat();

    if (m_dataPoints.value("PF").isValid())
        m_pf = m_dataPoints.value("PF").toFloat();

    if (m_dataPoints.value("WH").isValid())
        m_wattHours = m_dataPoints.value("WH").toFloat();

    if (m_dataPoints.value("DCA").isValid())
        m_dcAmps = m_dataPoints.value("DCA").toFloat();

    if (m_dataPoints.value("DCV").isValid())
        m_dcVoltage = m_dataPoints.value("DCV").toFloat();

    if (m_dataPoints.value("DCW").isValid())
        m_dcWatts = m_dataPoints.value("DCW").toFloat();

    if (m_dataPoints.value("TmpCab").isValid())
        m_cabinetTemperature = m_dataPoints.value("TmpCab").toFloat();

    if (m_dataPoints.value("TmpSnk").isValid())
        m_heatSinkTemperature = m_dataPoints.value("TmpSnk").toFloat();

    if (m_dataPoints.value("TmpTrns").isValid())
        m_transformerTemperature = m_dataPoints.value("TmpTrns").toFloat();

    if (m_dataPoints.value("TmpOt").isValid())
        m_otherTemperature = m_dataPoints.value("TmpOt").toFloat();

    if (m_dataPoints.value("St").isValid())
        m_operatingState = static_cast<St>(m_dataPoints.value("St").toUInt16());

    if (m_dataPoints.value("StVnd").isValid())
        m_vendorOperatingState = m_dataPoints.value("StVnd").toUInt16();

    if (m_dataPoints.value("Evt1").isValid())
        m_event1 = static_cast<Evt1Flags>(m_dataPoints.value("Evt1").toUInt32());

    if (m_dataPoints.value("Evt2").isValid())
        m_eventBitfield2 = m_dataPoints.value("Evt2").toUInt32();

    if (m_dataPoints.value("EvtVnd1").isValid())
        m_vendorEventBitfield1 = m_dataPoints.value("EvtVnd1").toUInt32();

    if (m_dataPoints.value("EvtVnd2").isValid())
        m_vendorEventBitfield2 = m_dataPoints.value("EvtVnd2").toUInt32();

    if (m_dataPoints.value("EvtVnd3").isValid())
        m_vendorEventBitfield3 = m_dataPoints.value("EvtVnd3").toUInt32();

    if (m_dataPoints.value("EvtVnd4").isValid())
        m_vendorEventBitfield4 = m_dataPoints.value("EvtVnd4").toUInt32();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecInverterSinglePhaseFloatModel *model)
{
    debug.nospace().noquote() << "SunSpecInverterSinglePhaseFloatModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
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

    debug.nospace().noquote() << "    - " << model->dataPoints().value("W") << "-->";
    if (model->dataPoints().value("W").isValid()) {
        debug.nospace().noquote() << model->watts() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Hz") << "-->";
    if (model->dataPoints().value("Hz").isValid()) {
        debug.nospace().noquote() << model->hz() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VA") << "-->";
    if (model->dataPoints().value("VA").isValid()) {
        debug.nospace().noquote() << model->va() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VAr") << "-->";
    if (model->dataPoints().value("VAr").isValid()) {
        debug.nospace().noquote() << model->vAr() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("PF") << "-->";
    if (model->dataPoints().value("PF").isValid()) {
        debug.nospace().noquote() << model->pf() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WH") << "-->";
    if (model->dataPoints().value("WH").isValid()) {
        debug.nospace().noquote() << model->wattHours() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("DCA") << "-->";
    if (model->dataPoints().value("DCA").isValid()) {
        debug.nospace().noquote() << model->dcAmps() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("DCV") << "-->";
    if (model->dataPoints().value("DCV").isValid()) {
        debug.nospace().noquote() << model->dcVoltage() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("DCW") << "-->";
    if (model->dataPoints().value("DCW").isValid()) {
        debug.nospace().noquote() << model->dcWatts() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TmpCab") << "-->";
    if (model->dataPoints().value("TmpCab").isValid()) {
        debug.nospace().noquote() << model->cabinetTemperature() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TmpSnk") << "-->";
    if (model->dataPoints().value("TmpSnk").isValid()) {
        debug.nospace().noquote() << model->heatSinkTemperature() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TmpTrns") << "-->";
    if (model->dataPoints().value("TmpTrns").isValid()) {
        debug.nospace().noquote() << model->transformerTemperature() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TmpOt") << "-->";
    if (model->dataPoints().value("TmpOt").isValid()) {
        debug.nospace().noquote() << model->otherTemperature() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("St") << "-->";
    if (model->dataPoints().value("St").isValid()) {
        debug.nospace().noquote() << model->operatingState() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("StVnd") << "-->";
    if (model->dataPoints().value("StVnd").isValid()) {
        debug.nospace().noquote() << model->vendorOperatingState() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Evt1") << "-->";
    if (model->dataPoints().value("Evt1").isValid()) {
        debug.nospace().noquote() << model->event1() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Evt2") << "-->";
    if (model->dataPoints().value("Evt2").isValid()) {
        debug.nospace().noquote() << model->eventBitfield2() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("EvtVnd1") << "-->";
    if (model->dataPoints().value("EvtVnd1").isValid()) {
        debug.nospace().noquote() << model->vendorEventBitfield1() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("EvtVnd2") << "-->";
    if (model->dataPoints().value("EvtVnd2").isValid()) {
        debug.nospace().noquote() << model->vendorEventBitfield2() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("EvtVnd3") << "-->";
    if (model->dataPoints().value("EvtVnd3").isValid()) {
        debug.nospace().noquote() << model->vendorEventBitfield3() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("EvtVnd4") << "-->";
    if (model->dataPoints().value("EvtVnd4").isValid()) {
        debug.nospace().noquote() << model->vendorEventBitfield4() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
