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

#include "sunspecinvertermodel.h"

SunSpecInverterModel::SunSpecInverterModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
    m_supportedModelIds << 101 << 102 << 103 << 111 << 112 << 113;
}

SunSpecInverterModel::~SunSpecInverterModel()
{

}

QString SunSpecInverterModel::name() const
{
    return "inverter";
}

QString SunSpecInverterModel::description() const
{
    switch (m_modelId) {
    case 101:
        return "Include this model for single phase inverter monitoring";
    case 102:
        return "Include this model for split phase inverter monitoring";
    case 103:
        return "Include this model for three phase inverter monitoring";
    case 111:
        return "Include this model for single phase inverter monitoring using float values";
    case 112:
        return "Include this model for split phase inverter monitoring using float values";
    case 113:
        return "Include this model for three phase inverter monitoring using float values";
    default:
        return QString();
    }
}

QString SunSpecInverterModel::label() const
{
    switch (m_modelId) {
    case 101:
        return "Inverter (Single Phase)";
    case 102:
        return "Inverter (Split-Phase)";
    case 103:
        return "Inverter (Three Phase)";
    case 111:
        return "Inverter (Single Phase) FLOAT";
    case 112:
        return "Inverter (Split Phase) FLOAT";
    case 113:
        return "Inverter (Three Phase) FLOAT";
    default:
        return QString();
    }
}

void SunSpecInverterModel::readModelHeader()
{

}

void SunSpecInverterModel::readBlockData()
{

}

void SunSpecInverterModel::initDataPoints()
{
    switch (m_modelId) {
    case 101: {
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
        ampsDataPoint.setSize(1);
        ampsDataPoint.setAddressOffset(2);
        ampsDataPoint.setBlockOffset(0);
        ampsDataPoint.setScaleFactorName("A_SF");
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(ampsDataPoint.name(), ampsDataPoint);

        SunSpecDataPoint ampsPhaseaDataPoint;
        ampsPhaseaDataPoint.setName("AphA");
        ampsPhaseaDataPoint.setLabel("Amps PhaseA");
        ampsPhaseaDataPoint.setDescription("Phase A Current");
        ampsPhaseaDataPoint.setUnits("A");
        ampsPhaseaDataPoint.setMandatory(true);
        ampsPhaseaDataPoint.setSize(1);
        ampsPhaseaDataPoint.setAddressOffset(3);
        ampsPhaseaDataPoint.setBlockOffset(1);
        ampsPhaseaDataPoint.setScaleFactorName("A_SF");
        ampsPhaseaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(ampsPhaseaDataPoint.name(), ampsPhaseaDataPoint);

        SunSpecDataPoint ampsPhasebDataPoint;
        ampsPhasebDataPoint.setName("AphB");
        ampsPhasebDataPoint.setLabel("Amps PhaseB");
        ampsPhasebDataPoint.setDescription("Phase B Current");
        ampsPhasebDataPoint.setUnits("A");
        ampsPhasebDataPoint.setSize(1);
        ampsPhasebDataPoint.setAddressOffset(4);
        ampsPhasebDataPoint.setBlockOffset(2);
        ampsPhasebDataPoint.setScaleFactorName("A_SF");
        ampsPhasebDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(ampsPhasebDataPoint.name(), ampsPhasebDataPoint);

        SunSpecDataPoint ampsPhasecDataPoint;
        ampsPhasecDataPoint.setName("AphC");
        ampsPhasecDataPoint.setLabel("Amps PhaseC");
        ampsPhasecDataPoint.setDescription("Phase C Current");
        ampsPhasecDataPoint.setUnits("A");
        ampsPhasecDataPoint.setSize(1);
        ampsPhasecDataPoint.setAddressOffset(5);
        ampsPhasecDataPoint.setBlockOffset(3);
        ampsPhasecDataPoint.setScaleFactorName("A_SF");
        ampsPhasecDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(ampsPhasecDataPoint.name(), ampsPhasecDataPoint);

        SunSpecDataPoint A_SFDataPoint;
        A_SFDataPoint.setName("A_SF");
        A_SFDataPoint.setMandatory(true);
        A_SFDataPoint.setSize(1);
        A_SFDataPoint.setAddressOffset(6);
        A_SFDataPoint.setBlockOffset(4);
        A_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(A_SFDataPoint.name(), A_SFDataPoint);

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PPVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setSize(1);
        phaseVoltageAbDataPoint.setAddressOffset(7);
        phaseVoltageAbDataPoint.setBlockOffset(5);
        phaseVoltageAbDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(phaseVoltageAbDataPoint.name(), phaseVoltageAbDataPoint);

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PPVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setSize(1);
        phaseVoltageBcDataPoint.setAddressOffset(8);
        phaseVoltageBcDataPoint.setBlockOffset(6);
        phaseVoltageBcDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(phaseVoltageBcDataPoint.name(), phaseVoltageBcDataPoint);

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PPVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setSize(1);
        phaseVoltageCaDataPoint.setAddressOffset(9);
        phaseVoltageCaDataPoint.setBlockOffset(7);
        phaseVoltageCaDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(phaseVoltageCaDataPoint.name(), phaseVoltageCaDataPoint);

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
        phaseVoltageAnDataPoint.setMandatory(true);
        phaseVoltageAnDataPoint.setSize(1);
        phaseVoltageAnDataPoint.setAddressOffset(10);
        phaseVoltageAnDataPoint.setBlockOffset(8);
        phaseVoltageAnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(phaseVoltageAnDataPoint.name(), phaseVoltageAnDataPoint);

        SunSpecDataPoint phaseVoltageBnDataPoint;
        phaseVoltageBnDataPoint.setName("PhVphB");
        phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
        phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
        phaseVoltageBnDataPoint.setUnits("V");
        phaseVoltageBnDataPoint.setSize(1);
        phaseVoltageBnDataPoint.setAddressOffset(11);
        phaseVoltageBnDataPoint.setBlockOffset(9);
        phaseVoltageBnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(phaseVoltageBnDataPoint.name(), phaseVoltageBnDataPoint);

        SunSpecDataPoint phaseVoltageCnDataPoint;
        phaseVoltageCnDataPoint.setName("PhVphC");
        phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
        phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
        phaseVoltageCnDataPoint.setUnits("V");
        phaseVoltageCnDataPoint.setSize(1);
        phaseVoltageCnDataPoint.setAddressOffset(12);
        phaseVoltageCnDataPoint.setBlockOffset(10);
        phaseVoltageCnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(phaseVoltageCnDataPoint.name(), phaseVoltageCnDataPoint);

        SunSpecDataPoint V_SFDataPoint;
        V_SFDataPoint.setName("V_SF");
        V_SFDataPoint.setMandatory(true);
        V_SFDataPoint.setSize(1);
        V_SFDataPoint.setAddressOffset(13);
        V_SFDataPoint.setBlockOffset(11);
        V_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(V_SFDataPoint.name(), V_SFDataPoint);

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("AC Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(1);
        wattsDataPoint.setAddressOffset(14);
        wattsDataPoint.setBlockOffset(12);
        wattsDataPoint.setScaleFactorName("W_SF");
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsDataPoint.name(), wattsDataPoint);

        SunSpecDataPoint W_SFDataPoint;
        W_SFDataPoint.setName("W_SF");
        W_SFDataPoint.setMandatory(true);
        W_SFDataPoint.setSize(1);
        W_SFDataPoint.setAddressOffset(15);
        W_SFDataPoint.setBlockOffset(13);
        W_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(W_SFDataPoint.name(), W_SFDataPoint);

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Line Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(1);
        hzDataPoint.setAddressOffset(16);
        hzDataPoint.setBlockOffset(14);
        hzDataPoint.setScaleFactorName("Hz_SF");
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(hzDataPoint.name(), hzDataPoint);

        SunSpecDataPoint Hz_SFDataPoint;
        Hz_SFDataPoint.setName("Hz_SF");
        Hz_SFDataPoint.setMandatory(true);
        Hz_SFDataPoint.setSize(1);
        Hz_SFDataPoint.setAddressOffset(17);
        Hz_SFDataPoint.setBlockOffset(15);
        Hz_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(Hz_SFDataPoint.name(), Hz_SFDataPoint);

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(1);
        vaDataPoint.setAddressOffset(18);
        vaDataPoint.setBlockOffset(16);
        vaDataPoint.setScaleFactorName("VA_SF");
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(vaDataPoint.name(), vaDataPoint);

        SunSpecDataPoint VA_SFDataPoint;
        VA_SFDataPoint.setName("VA_SF");
        VA_SFDataPoint.setSize(1);
        VA_SFDataPoint.setAddressOffset(19);
        VA_SFDataPoint.setBlockOffset(17);
        VA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(VA_SFDataPoint.name(), VA_SFDataPoint);

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAr");
        varDataPoint.setLabel("VAr");
        varDataPoint.setDescription("AC Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(1);
        varDataPoint.setAddressOffset(20);
        varDataPoint.setBlockOffset(18);
        varDataPoint.setScaleFactorName("VAr_SF");
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(varDataPoint.name(), varDataPoint);

        SunSpecDataPoint VAr_SFDataPoint;
        VAr_SFDataPoint.setName("VAr_SF");
        VAr_SFDataPoint.setSize(1);
        VAr_SFDataPoint.setAddressOffset(21);
        VAr_SFDataPoint.setBlockOffset(19);
        VAr_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(VAr_SFDataPoint.name(), VAr_SFDataPoint);

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("AC Power Factor");
        pfDataPoint.setUnits("Pct");
        pfDataPoint.setSize(1);
        pfDataPoint.setAddressOffset(22);
        pfDataPoint.setBlockOffset(20);
        pfDataPoint.setScaleFactorName("PF_SF");
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(pfDataPoint.name(), pfDataPoint);

        SunSpecDataPoint PF_SFDataPoint;
        PF_SFDataPoint.setName("PF_SF");
        PF_SFDataPoint.setSize(1);
        PF_SFDataPoint.setAddressOffset(23);
        PF_SFDataPoint.setBlockOffset(21);
        PF_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(PF_SFDataPoint.name(), PF_SFDataPoint);

        SunSpecDataPoint watthoursDataPoint;
        watthoursDataPoint.setName("WH");
        watthoursDataPoint.setLabel("WattHours");
        watthoursDataPoint.setDescription("AC Energy");
        watthoursDataPoint.setUnits("Wh");
        watthoursDataPoint.setMandatory(true);
        watthoursDataPoint.setSize(2);
        watthoursDataPoint.setAddressOffset(24);
        watthoursDataPoint.setBlockOffset(22);
        watthoursDataPoint.setScaleFactorName("WH_SF");
        watthoursDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(watthoursDataPoint.name(), watthoursDataPoint);

        SunSpecDataPoint WH_SFDataPoint;
        WH_SFDataPoint.setName("WH_SF");
        WH_SFDataPoint.setMandatory(true);
        WH_SFDataPoint.setSize(1);
        WH_SFDataPoint.setAddressOffset(26);
        WH_SFDataPoint.setBlockOffset(24);
        WH_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(WH_SFDataPoint.name(), WH_SFDataPoint);

        SunSpecDataPoint dcAmpsDataPoint;
        dcAmpsDataPoint.setName("DCA");
        dcAmpsDataPoint.setLabel("DC Amps");
        dcAmpsDataPoint.setDescription("DC Current");
        dcAmpsDataPoint.setUnits("A");
        dcAmpsDataPoint.setSize(1);
        dcAmpsDataPoint.setAddressOffset(27);
        dcAmpsDataPoint.setBlockOffset(25);
        dcAmpsDataPoint.setScaleFactorName("DCA_SF");
        dcAmpsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(dcAmpsDataPoint.name(), dcAmpsDataPoint);

        SunSpecDataPoint DCA_SFDataPoint;
        DCA_SFDataPoint.setName("DCA_SF");
        DCA_SFDataPoint.setSize(1);
        DCA_SFDataPoint.setAddressOffset(28);
        DCA_SFDataPoint.setBlockOffset(26);
        DCA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCA_SFDataPoint.name(), DCA_SFDataPoint);

        SunSpecDataPoint dcVoltageDataPoint;
        dcVoltageDataPoint.setName("DCV");
        dcVoltageDataPoint.setLabel("DC Voltage");
        dcVoltageDataPoint.setDescription("DC Voltage");
        dcVoltageDataPoint.setUnits("V");
        dcVoltageDataPoint.setSize(1);
        dcVoltageDataPoint.setAddressOffset(29);
        dcVoltageDataPoint.setBlockOffset(27);
        dcVoltageDataPoint.setScaleFactorName("DCV_SF");
        dcVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(dcVoltageDataPoint.name(), dcVoltageDataPoint);

        SunSpecDataPoint DCV_SFDataPoint;
        DCV_SFDataPoint.setName("DCV_SF");
        DCV_SFDataPoint.setSize(1);
        DCV_SFDataPoint.setAddressOffset(30);
        DCV_SFDataPoint.setBlockOffset(28);
        DCV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCV_SFDataPoint.name(), DCV_SFDataPoint);

        SunSpecDataPoint dcWattsDataPoint;
        dcWattsDataPoint.setName("DCW");
        dcWattsDataPoint.setLabel("DC Watts");
        dcWattsDataPoint.setDescription("DC Power");
        dcWattsDataPoint.setUnits("W");
        dcWattsDataPoint.setSize(1);
        dcWattsDataPoint.setAddressOffset(31);
        dcWattsDataPoint.setBlockOffset(29);
        dcWattsDataPoint.setScaleFactorName("DCW_SF");
        dcWattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(dcWattsDataPoint.name(), dcWattsDataPoint);

        SunSpecDataPoint DCW_SFDataPoint;
        DCW_SFDataPoint.setName("DCW_SF");
        DCW_SFDataPoint.setSize(1);
        DCW_SFDataPoint.setAddressOffset(32);
        DCW_SFDataPoint.setBlockOffset(30);
        DCW_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCW_SFDataPoint.name(), DCW_SFDataPoint);

        SunSpecDataPoint cabinetTemperatureDataPoint;
        cabinetTemperatureDataPoint.setName("TmpCab");
        cabinetTemperatureDataPoint.setLabel("Cabinet Temperature");
        cabinetTemperatureDataPoint.setDescription("Cabinet Temperature");
        cabinetTemperatureDataPoint.setUnits("C");
        cabinetTemperatureDataPoint.setMandatory(true);
        cabinetTemperatureDataPoint.setSize(1);
        cabinetTemperatureDataPoint.setAddressOffset(33);
        cabinetTemperatureDataPoint.setBlockOffset(31);
        cabinetTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        cabinetTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(cabinetTemperatureDataPoint.name(), cabinetTemperatureDataPoint);

        SunSpecDataPoint heatSinkTemperatureDataPoint;
        heatSinkTemperatureDataPoint.setName("TmpSnk");
        heatSinkTemperatureDataPoint.setLabel("Heat Sink Temperature");
        heatSinkTemperatureDataPoint.setDescription("Heat Sink Temperature");
        heatSinkTemperatureDataPoint.setUnits("C");
        heatSinkTemperatureDataPoint.setSize(1);
        heatSinkTemperatureDataPoint.setAddressOffset(34);
        heatSinkTemperatureDataPoint.setBlockOffset(32);
        heatSinkTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        heatSinkTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(heatSinkTemperatureDataPoint.name(), heatSinkTemperatureDataPoint);

        SunSpecDataPoint transformerTemperatureDataPoint;
        transformerTemperatureDataPoint.setName("TmpTrns");
        transformerTemperatureDataPoint.setLabel("Transformer Temperature");
        transformerTemperatureDataPoint.setDescription("Transformer Temperature");
        transformerTemperatureDataPoint.setUnits("C");
        transformerTemperatureDataPoint.setSize(1);
        transformerTemperatureDataPoint.setAddressOffset(35);
        transformerTemperatureDataPoint.setBlockOffset(33);
        transformerTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        transformerTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(transformerTemperatureDataPoint.name(), transformerTemperatureDataPoint);

        SunSpecDataPoint otherTemperatureDataPoint;
        otherTemperatureDataPoint.setName("TmpOt");
        otherTemperatureDataPoint.setLabel("Other Temperature");
        otherTemperatureDataPoint.setDescription("Other Temperature");
        otherTemperatureDataPoint.setUnits("C");
        otherTemperatureDataPoint.setSize(1);
        otherTemperatureDataPoint.setAddressOffset(36);
        otherTemperatureDataPoint.setBlockOffset(34);
        otherTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        otherTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(otherTemperatureDataPoint.name(), otherTemperatureDataPoint);

        SunSpecDataPoint Tmp_SFDataPoint;
        Tmp_SFDataPoint.setName("Tmp_SF");
        Tmp_SFDataPoint.setMandatory(true);
        Tmp_SFDataPoint.setSize(1);
        Tmp_SFDataPoint.setAddressOffset(37);
        Tmp_SFDataPoint.setBlockOffset(35);
        Tmp_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(Tmp_SFDataPoint.name(), Tmp_SFDataPoint);

        SunSpecDataPoint operatingStateDataPoint;
        operatingStateDataPoint.setName("St");
        operatingStateDataPoint.setLabel("Operating State");
        operatingStateDataPoint.setDescription("Enumerated value.  Operating state");
        operatingStateDataPoint.setMandatory(true);
        operatingStateDataPoint.setSize(1);
        operatingStateDataPoint.setAddressOffset(38);
        operatingStateDataPoint.setBlockOffset(36);
        operatingStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints.insert(operatingStateDataPoint.name(), operatingStateDataPoint);

        SunSpecDataPoint vendorOperatingStateDataPoint;
        vendorOperatingStateDataPoint.setName("StVnd");
        vendorOperatingStateDataPoint.setLabel("Vendor Operating State");
        vendorOperatingStateDataPoint.setDescription("Vendor specific operating state code");
        vendorOperatingStateDataPoint.setSize(1);
        vendorOperatingStateDataPoint.setAddressOffset(39);
        vendorOperatingStateDataPoint.setBlockOffset(37);
        vendorOperatingStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints.insert(vendorOperatingStateDataPoint.name(), vendorOperatingStateDataPoint);

        SunSpecDataPoint event1DataPoint;
        event1DataPoint.setName("Evt1");
        event1DataPoint.setLabel("Event1");
        event1DataPoint.setDescription("Bitmask value. Event fields");
        event1DataPoint.setMandatory(true);
        event1DataPoint.setSize(2);
        event1DataPoint.setAddressOffset(40);
        event1DataPoint.setBlockOffset(38);
        event1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(event1DataPoint.name(), event1DataPoint);

        SunSpecDataPoint eventBitfield2DataPoint;
        eventBitfield2DataPoint.setName("Evt2");
        eventBitfield2DataPoint.setLabel("Event Bitfield 2");
        eventBitfield2DataPoint.setDescription("Reserved for future use");
        eventBitfield2DataPoint.setMandatory(true);
        eventBitfield2DataPoint.setSize(2);
        eventBitfield2DataPoint.setAddressOffset(42);
        eventBitfield2DataPoint.setBlockOffset(40);
        eventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(eventBitfield2DataPoint.name(), eventBitfield2DataPoint);

        SunSpecDataPoint vendorEventBitfield1DataPoint;
        vendorEventBitfield1DataPoint.setName("EvtVnd1");
        vendorEventBitfield1DataPoint.setLabel("Vendor Event Bitfield 1");
        vendorEventBitfield1DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield1DataPoint.setSize(2);
        vendorEventBitfield1DataPoint.setAddressOffset(44);
        vendorEventBitfield1DataPoint.setBlockOffset(42);
        vendorEventBitfield1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(vendorEventBitfield1DataPoint.name(), vendorEventBitfield1DataPoint);

        SunSpecDataPoint vendorEventBitfield2DataPoint;
        vendorEventBitfield2DataPoint.setName("EvtVnd2");
        vendorEventBitfield2DataPoint.setLabel("Vendor Event Bitfield 2");
        vendorEventBitfield2DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield2DataPoint.setSize(2);
        vendorEventBitfield2DataPoint.setAddressOffset(46);
        vendorEventBitfield2DataPoint.setBlockOffset(44);
        vendorEventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(vendorEventBitfield2DataPoint.name(), vendorEventBitfield2DataPoint);

        SunSpecDataPoint vendorEventBitfield3DataPoint;
        vendorEventBitfield3DataPoint.setName("EvtVnd3");
        vendorEventBitfield3DataPoint.setLabel("Vendor Event Bitfield 3");
        vendorEventBitfield3DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield3DataPoint.setSize(2);
        vendorEventBitfield3DataPoint.setAddressOffset(48);
        vendorEventBitfield3DataPoint.setBlockOffset(46);
        vendorEventBitfield3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(vendorEventBitfield3DataPoint.name(), vendorEventBitfield3DataPoint);

        SunSpecDataPoint vendorEventBitfield4DataPoint;
        vendorEventBitfield4DataPoint.setName("EvtVnd4");
        vendorEventBitfield4DataPoint.setLabel("Vendor Event Bitfield 4");
        vendorEventBitfield4DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield4DataPoint.setSize(2);
        vendorEventBitfield4DataPoint.setAddressOffset(50);
        vendorEventBitfield4DataPoint.setBlockOffset(48);
        vendorEventBitfield4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(vendorEventBitfield4DataPoint.name(), vendorEventBitfield4DataPoint);

        break;
    }
    case 102: {
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
        ampsDataPoint.setSize(1);
        ampsDataPoint.setAddressOffset(2);
        ampsDataPoint.setBlockOffset(0);
        ampsDataPoint.setScaleFactorName("A_SF");
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(ampsDataPoint.name(), ampsDataPoint);

        SunSpecDataPoint ampsPhaseaDataPoint;
        ampsPhaseaDataPoint.setName("AphA");
        ampsPhaseaDataPoint.setLabel("Amps PhaseA");
        ampsPhaseaDataPoint.setDescription("Phase A Current");
        ampsPhaseaDataPoint.setUnits("A");
        ampsPhaseaDataPoint.setMandatory(true);
        ampsPhaseaDataPoint.setSize(1);
        ampsPhaseaDataPoint.setAddressOffset(3);
        ampsPhaseaDataPoint.setBlockOffset(1);
        ampsPhaseaDataPoint.setScaleFactorName("A_SF");
        ampsPhaseaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(ampsPhaseaDataPoint.name(), ampsPhaseaDataPoint);

        SunSpecDataPoint ampsPhasebDataPoint;
        ampsPhasebDataPoint.setName("AphB");
        ampsPhasebDataPoint.setLabel("Amps PhaseB");
        ampsPhasebDataPoint.setDescription("Phase B Current");
        ampsPhasebDataPoint.setUnits("A");
        ampsPhasebDataPoint.setMandatory(true);
        ampsPhasebDataPoint.setSize(1);
        ampsPhasebDataPoint.setAddressOffset(4);
        ampsPhasebDataPoint.setBlockOffset(2);
        ampsPhasebDataPoint.setScaleFactorName("A_SF");
        ampsPhasebDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(ampsPhasebDataPoint.name(), ampsPhasebDataPoint);

        SunSpecDataPoint ampsPhasecDataPoint;
        ampsPhasecDataPoint.setName("AphC");
        ampsPhasecDataPoint.setLabel("Amps PhaseC");
        ampsPhasecDataPoint.setDescription("Phase C Current");
        ampsPhasecDataPoint.setUnits("A");
        ampsPhasecDataPoint.setSize(1);
        ampsPhasecDataPoint.setAddressOffset(5);
        ampsPhasecDataPoint.setBlockOffset(3);
        ampsPhasecDataPoint.setScaleFactorName("A_SF");
        ampsPhasecDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(ampsPhasecDataPoint.name(), ampsPhasecDataPoint);

        SunSpecDataPoint A_SFDataPoint;
        A_SFDataPoint.setName("A_SF");
        A_SFDataPoint.setMandatory(true);
        A_SFDataPoint.setSize(1);
        A_SFDataPoint.setAddressOffset(6);
        A_SFDataPoint.setBlockOffset(4);
        A_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(A_SFDataPoint.name(), A_SFDataPoint);

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PPVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setSize(1);
        phaseVoltageAbDataPoint.setAddressOffset(7);
        phaseVoltageAbDataPoint.setBlockOffset(5);
        phaseVoltageAbDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(phaseVoltageAbDataPoint.name(), phaseVoltageAbDataPoint);

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PPVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setSize(1);
        phaseVoltageBcDataPoint.setAddressOffset(8);
        phaseVoltageBcDataPoint.setBlockOffset(6);
        phaseVoltageBcDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(phaseVoltageBcDataPoint.name(), phaseVoltageBcDataPoint);

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PPVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setSize(1);
        phaseVoltageCaDataPoint.setAddressOffset(9);
        phaseVoltageCaDataPoint.setBlockOffset(7);
        phaseVoltageCaDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(phaseVoltageCaDataPoint.name(), phaseVoltageCaDataPoint);

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
        phaseVoltageAnDataPoint.setMandatory(true);
        phaseVoltageAnDataPoint.setSize(1);
        phaseVoltageAnDataPoint.setAddressOffset(10);
        phaseVoltageAnDataPoint.setBlockOffset(8);
        phaseVoltageAnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(phaseVoltageAnDataPoint.name(), phaseVoltageAnDataPoint);

        SunSpecDataPoint phaseVoltageBnDataPoint;
        phaseVoltageBnDataPoint.setName("PhVphB");
        phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
        phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
        phaseVoltageBnDataPoint.setUnits("V");
        phaseVoltageBnDataPoint.setMandatory(true);
        phaseVoltageBnDataPoint.setSize(1);
        phaseVoltageBnDataPoint.setAddressOffset(11);
        phaseVoltageBnDataPoint.setBlockOffset(9);
        phaseVoltageBnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(phaseVoltageBnDataPoint.name(), phaseVoltageBnDataPoint);

        SunSpecDataPoint phaseVoltageCnDataPoint;
        phaseVoltageCnDataPoint.setName("PhVphC");
        phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
        phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
        phaseVoltageCnDataPoint.setUnits("V");
        phaseVoltageCnDataPoint.setSize(1);
        phaseVoltageCnDataPoint.setAddressOffset(12);
        phaseVoltageCnDataPoint.setBlockOffset(10);
        phaseVoltageCnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(phaseVoltageCnDataPoint.name(), phaseVoltageCnDataPoint);

        SunSpecDataPoint V_SFDataPoint;
        V_SFDataPoint.setName("V_SF");
        V_SFDataPoint.setMandatory(true);
        V_SFDataPoint.setSize(1);
        V_SFDataPoint.setAddressOffset(13);
        V_SFDataPoint.setBlockOffset(11);
        V_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(V_SFDataPoint.name(), V_SFDataPoint);

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("AC Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(1);
        wattsDataPoint.setAddressOffset(14);
        wattsDataPoint.setBlockOffset(12);
        wattsDataPoint.setScaleFactorName("W_SF");
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsDataPoint.name(), wattsDataPoint);

        SunSpecDataPoint W_SFDataPoint;
        W_SFDataPoint.setName("W_SF");
        W_SFDataPoint.setMandatory(true);
        W_SFDataPoint.setSize(1);
        W_SFDataPoint.setAddressOffset(15);
        W_SFDataPoint.setBlockOffset(13);
        W_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(W_SFDataPoint.name(), W_SFDataPoint);

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Line Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(1);
        hzDataPoint.setAddressOffset(16);
        hzDataPoint.setBlockOffset(14);
        hzDataPoint.setScaleFactorName("Hz_SF");
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(hzDataPoint.name(), hzDataPoint);

        SunSpecDataPoint Hz_SFDataPoint;
        Hz_SFDataPoint.setName("Hz_SF");
        Hz_SFDataPoint.setMandatory(true);
        Hz_SFDataPoint.setSize(1);
        Hz_SFDataPoint.setAddressOffset(17);
        Hz_SFDataPoint.setBlockOffset(15);
        Hz_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(Hz_SFDataPoint.name(), Hz_SFDataPoint);

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(1);
        vaDataPoint.setAddressOffset(18);
        vaDataPoint.setBlockOffset(16);
        vaDataPoint.setScaleFactorName("VA_SF");
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(vaDataPoint.name(), vaDataPoint);

        SunSpecDataPoint VA_SFDataPoint;
        VA_SFDataPoint.setName("VA_SF");
        VA_SFDataPoint.setSize(1);
        VA_SFDataPoint.setAddressOffset(19);
        VA_SFDataPoint.setBlockOffset(17);
        VA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(VA_SFDataPoint.name(), VA_SFDataPoint);

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAr");
        varDataPoint.setLabel("VAr");
        varDataPoint.setDescription("AC Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(1);
        varDataPoint.setAddressOffset(20);
        varDataPoint.setBlockOffset(18);
        varDataPoint.setScaleFactorName("VAr_SF");
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(varDataPoint.name(), varDataPoint);

        SunSpecDataPoint VAr_SFDataPoint;
        VAr_SFDataPoint.setName("VAr_SF");
        VAr_SFDataPoint.setSize(1);
        VAr_SFDataPoint.setAddressOffset(21);
        VAr_SFDataPoint.setBlockOffset(19);
        VAr_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(VAr_SFDataPoint.name(), VAr_SFDataPoint);

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("AC Power Factor");
        pfDataPoint.setUnits("Pct");
        pfDataPoint.setSize(1);
        pfDataPoint.setAddressOffset(22);
        pfDataPoint.setBlockOffset(20);
        pfDataPoint.setScaleFactorName("PF_SF");
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(pfDataPoint.name(), pfDataPoint);

        SunSpecDataPoint PF_SFDataPoint;
        PF_SFDataPoint.setName("PF_SF");
        PF_SFDataPoint.setSize(1);
        PF_SFDataPoint.setAddressOffset(23);
        PF_SFDataPoint.setBlockOffset(21);
        PF_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(PF_SFDataPoint.name(), PF_SFDataPoint);

        SunSpecDataPoint watthoursDataPoint;
        watthoursDataPoint.setName("WH");
        watthoursDataPoint.setLabel("WattHours");
        watthoursDataPoint.setDescription("AC Energy");
        watthoursDataPoint.setUnits("Wh");
        watthoursDataPoint.setMandatory(true);
        watthoursDataPoint.setSize(2);
        watthoursDataPoint.setAddressOffset(24);
        watthoursDataPoint.setBlockOffset(22);
        watthoursDataPoint.setScaleFactorName("WH_SF");
        watthoursDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(watthoursDataPoint.name(), watthoursDataPoint);

        SunSpecDataPoint WH_SFDataPoint;
        WH_SFDataPoint.setName("WH_SF");
        WH_SFDataPoint.setMandatory(true);
        WH_SFDataPoint.setSize(1);
        WH_SFDataPoint.setAddressOffset(26);
        WH_SFDataPoint.setBlockOffset(24);
        WH_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(WH_SFDataPoint.name(), WH_SFDataPoint);

        SunSpecDataPoint dcAmpsDataPoint;
        dcAmpsDataPoint.setName("DCA");
        dcAmpsDataPoint.setLabel("DC Amps");
        dcAmpsDataPoint.setDescription("DC Current");
        dcAmpsDataPoint.setUnits("A");
        dcAmpsDataPoint.setSize(1);
        dcAmpsDataPoint.setAddressOffset(27);
        dcAmpsDataPoint.setBlockOffset(25);
        dcAmpsDataPoint.setScaleFactorName("DCA_SF");
        dcAmpsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(dcAmpsDataPoint.name(), dcAmpsDataPoint);

        SunSpecDataPoint DCA_SFDataPoint;
        DCA_SFDataPoint.setName("DCA_SF");
        DCA_SFDataPoint.setSize(1);
        DCA_SFDataPoint.setAddressOffset(28);
        DCA_SFDataPoint.setBlockOffset(26);
        DCA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCA_SFDataPoint.name(), DCA_SFDataPoint);

        SunSpecDataPoint dcVoltageDataPoint;
        dcVoltageDataPoint.setName("DCV");
        dcVoltageDataPoint.setLabel("DC Voltage");
        dcVoltageDataPoint.setDescription("DC Voltage");
        dcVoltageDataPoint.setUnits("V");
        dcVoltageDataPoint.setSize(1);
        dcVoltageDataPoint.setAddressOffset(29);
        dcVoltageDataPoint.setBlockOffset(27);
        dcVoltageDataPoint.setScaleFactorName("DCV_SF");
        dcVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(dcVoltageDataPoint.name(), dcVoltageDataPoint);

        SunSpecDataPoint DCV_SFDataPoint;
        DCV_SFDataPoint.setName("DCV_SF");
        DCV_SFDataPoint.setSize(1);
        DCV_SFDataPoint.setAddressOffset(30);
        DCV_SFDataPoint.setBlockOffset(28);
        DCV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCV_SFDataPoint.name(), DCV_SFDataPoint);

        SunSpecDataPoint dcWattsDataPoint;
        dcWattsDataPoint.setName("DCW");
        dcWattsDataPoint.setLabel("DC Watts");
        dcWattsDataPoint.setDescription("DC Power");
        dcWattsDataPoint.setUnits("W");
        dcWattsDataPoint.setSize(1);
        dcWattsDataPoint.setAddressOffset(31);
        dcWattsDataPoint.setBlockOffset(29);
        dcWattsDataPoint.setScaleFactorName("DCW_SF");
        dcWattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(dcWattsDataPoint.name(), dcWattsDataPoint);

        SunSpecDataPoint DCW_SFDataPoint;
        DCW_SFDataPoint.setName("DCW_SF");
        DCW_SFDataPoint.setSize(1);
        DCW_SFDataPoint.setAddressOffset(32);
        DCW_SFDataPoint.setBlockOffset(30);
        DCW_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCW_SFDataPoint.name(), DCW_SFDataPoint);

        SunSpecDataPoint cabinetTemperatureDataPoint;
        cabinetTemperatureDataPoint.setName("TmpCab");
        cabinetTemperatureDataPoint.setLabel("Cabinet Temperature");
        cabinetTemperatureDataPoint.setDescription("Cabinet Temperature");
        cabinetTemperatureDataPoint.setUnits("C");
        cabinetTemperatureDataPoint.setMandatory(true);
        cabinetTemperatureDataPoint.setSize(1);
        cabinetTemperatureDataPoint.setAddressOffset(33);
        cabinetTemperatureDataPoint.setBlockOffset(31);
        cabinetTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        cabinetTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(cabinetTemperatureDataPoint.name(), cabinetTemperatureDataPoint);

        SunSpecDataPoint heatSinkTemperatureDataPoint;
        heatSinkTemperatureDataPoint.setName("TmpSnk");
        heatSinkTemperatureDataPoint.setLabel("Heat Sink Temperature");
        heatSinkTemperatureDataPoint.setDescription("Heat Sink Temperature");
        heatSinkTemperatureDataPoint.setUnits("C");
        heatSinkTemperatureDataPoint.setSize(1);
        heatSinkTemperatureDataPoint.setAddressOffset(34);
        heatSinkTemperatureDataPoint.setBlockOffset(32);
        heatSinkTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        heatSinkTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(heatSinkTemperatureDataPoint.name(), heatSinkTemperatureDataPoint);

        SunSpecDataPoint transformerTemperatureDataPoint;
        transformerTemperatureDataPoint.setName("TmpTrns");
        transformerTemperatureDataPoint.setLabel("Transformer Temperature");
        transformerTemperatureDataPoint.setDescription("Transformer Temperature");
        transformerTemperatureDataPoint.setUnits("C");
        transformerTemperatureDataPoint.setSize(1);
        transformerTemperatureDataPoint.setAddressOffset(35);
        transformerTemperatureDataPoint.setBlockOffset(33);
        transformerTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        transformerTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(transformerTemperatureDataPoint.name(), transformerTemperatureDataPoint);

        SunSpecDataPoint otherTemperatureDataPoint;
        otherTemperatureDataPoint.setName("TmpOt");
        otherTemperatureDataPoint.setLabel("Other Temperature");
        otherTemperatureDataPoint.setDescription("Other Temperature");
        otherTemperatureDataPoint.setUnits("C");
        otherTemperatureDataPoint.setSize(1);
        otherTemperatureDataPoint.setAddressOffset(36);
        otherTemperatureDataPoint.setBlockOffset(34);
        otherTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        otherTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(otherTemperatureDataPoint.name(), otherTemperatureDataPoint);

        SunSpecDataPoint Tmp_SFDataPoint;
        Tmp_SFDataPoint.setName("Tmp_SF");
        Tmp_SFDataPoint.setMandatory(true);
        Tmp_SFDataPoint.setSize(1);
        Tmp_SFDataPoint.setAddressOffset(37);
        Tmp_SFDataPoint.setBlockOffset(35);
        Tmp_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(Tmp_SFDataPoint.name(), Tmp_SFDataPoint);

        SunSpecDataPoint operatingStateDataPoint;
        operatingStateDataPoint.setName("St");
        operatingStateDataPoint.setLabel("Operating State");
        operatingStateDataPoint.setDescription("Enumerated value.  Operating state");
        operatingStateDataPoint.setMandatory(true);
        operatingStateDataPoint.setSize(1);
        operatingStateDataPoint.setAddressOffset(38);
        operatingStateDataPoint.setBlockOffset(36);
        operatingStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints.insert(operatingStateDataPoint.name(), operatingStateDataPoint);

        SunSpecDataPoint vendorOperatingStateDataPoint;
        vendorOperatingStateDataPoint.setName("StVnd");
        vendorOperatingStateDataPoint.setLabel("Vendor Operating State");
        vendorOperatingStateDataPoint.setDescription("Vendor specific operating state code");
        vendorOperatingStateDataPoint.setSize(1);
        vendorOperatingStateDataPoint.setAddressOffset(39);
        vendorOperatingStateDataPoint.setBlockOffset(37);
        vendorOperatingStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints.insert(vendorOperatingStateDataPoint.name(), vendorOperatingStateDataPoint);

        SunSpecDataPoint event1DataPoint;
        event1DataPoint.setName("Evt1");
        event1DataPoint.setLabel("Event1");
        event1DataPoint.setDescription("Bitmask value. Event fields");
        event1DataPoint.setMandatory(true);
        event1DataPoint.setSize(2);
        event1DataPoint.setAddressOffset(40);
        event1DataPoint.setBlockOffset(38);
        event1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(event1DataPoint.name(), event1DataPoint);

        SunSpecDataPoint eventBitfield2DataPoint;
        eventBitfield2DataPoint.setName("Evt2");
        eventBitfield2DataPoint.setLabel("Event Bitfield 2");
        eventBitfield2DataPoint.setDescription("Reserved for future use");
        eventBitfield2DataPoint.setMandatory(true);
        eventBitfield2DataPoint.setSize(2);
        eventBitfield2DataPoint.setAddressOffset(42);
        eventBitfield2DataPoint.setBlockOffset(40);
        eventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(eventBitfield2DataPoint.name(), eventBitfield2DataPoint);

        SunSpecDataPoint vendorEventBitfield1DataPoint;
        vendorEventBitfield1DataPoint.setName("EvtVnd1");
        vendorEventBitfield1DataPoint.setLabel("Vendor Event Bitfield 1");
        vendorEventBitfield1DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield1DataPoint.setSize(2);
        vendorEventBitfield1DataPoint.setAddressOffset(44);
        vendorEventBitfield1DataPoint.setBlockOffset(42);
        vendorEventBitfield1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(vendorEventBitfield1DataPoint.name(), vendorEventBitfield1DataPoint);

        SunSpecDataPoint vendorEventBitfield2DataPoint;
        vendorEventBitfield2DataPoint.setName("EvtVnd2");
        vendorEventBitfield2DataPoint.setLabel("Vendor Event Bitfield 2");
        vendorEventBitfield2DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield2DataPoint.setSize(2);
        vendorEventBitfield2DataPoint.setAddressOffset(46);
        vendorEventBitfield2DataPoint.setBlockOffset(44);
        vendorEventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(vendorEventBitfield2DataPoint.name(), vendorEventBitfield2DataPoint);

        SunSpecDataPoint vendorEventBitfield3DataPoint;
        vendorEventBitfield3DataPoint.setName("EvtVnd3");
        vendorEventBitfield3DataPoint.setLabel("Vendor Event Bitfield 3");
        vendorEventBitfield3DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield3DataPoint.setSize(2);
        vendorEventBitfield3DataPoint.setAddressOffset(48);
        vendorEventBitfield3DataPoint.setBlockOffset(46);
        vendorEventBitfield3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(vendorEventBitfield3DataPoint.name(), vendorEventBitfield3DataPoint);

        SunSpecDataPoint vendorEventBitfield4DataPoint;
        vendorEventBitfield4DataPoint.setName("EvtVnd4");
        vendorEventBitfield4DataPoint.setLabel("Vendor Event Bitfield 4");
        vendorEventBitfield4DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield4DataPoint.setSize(2);
        vendorEventBitfield4DataPoint.setAddressOffset(50);
        vendorEventBitfield4DataPoint.setBlockOffset(48);
        vendorEventBitfield4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(vendorEventBitfield4DataPoint.name(), vendorEventBitfield4DataPoint);

        break;
    }
    case 103: {
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
        ampsDataPoint.setSize(1);
        ampsDataPoint.setAddressOffset(2);
        ampsDataPoint.setBlockOffset(0);
        ampsDataPoint.setScaleFactorName("A_SF");
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(ampsDataPoint.name(), ampsDataPoint);

        SunSpecDataPoint ampsPhaseaDataPoint;
        ampsPhaseaDataPoint.setName("AphA");
        ampsPhaseaDataPoint.setLabel("Amps PhaseA");
        ampsPhaseaDataPoint.setDescription("Phase A Current");
        ampsPhaseaDataPoint.setUnits("A");
        ampsPhaseaDataPoint.setMandatory(true);
        ampsPhaseaDataPoint.setSize(1);
        ampsPhaseaDataPoint.setAddressOffset(3);
        ampsPhaseaDataPoint.setBlockOffset(1);
        ampsPhaseaDataPoint.setScaleFactorName("A_SF");
        ampsPhaseaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(ampsPhaseaDataPoint.name(), ampsPhaseaDataPoint);

        SunSpecDataPoint ampsPhasebDataPoint;
        ampsPhasebDataPoint.setName("AphB");
        ampsPhasebDataPoint.setLabel("Amps PhaseB");
        ampsPhasebDataPoint.setDescription("Phase B Current");
        ampsPhasebDataPoint.setUnits("A");
        ampsPhasebDataPoint.setMandatory(true);
        ampsPhasebDataPoint.setSize(1);
        ampsPhasebDataPoint.setAddressOffset(4);
        ampsPhasebDataPoint.setBlockOffset(2);
        ampsPhasebDataPoint.setScaleFactorName("A_SF");
        ampsPhasebDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(ampsPhasebDataPoint.name(), ampsPhasebDataPoint);

        SunSpecDataPoint ampsPhasecDataPoint;
        ampsPhasecDataPoint.setName("AphC");
        ampsPhasecDataPoint.setLabel("Amps PhaseC");
        ampsPhasecDataPoint.setDescription("Phase C Current");
        ampsPhasecDataPoint.setUnits("A");
        ampsPhasecDataPoint.setMandatory(true);
        ampsPhasecDataPoint.setSize(1);
        ampsPhasecDataPoint.setAddressOffset(5);
        ampsPhasecDataPoint.setBlockOffset(3);
        ampsPhasecDataPoint.setScaleFactorName("A_SF");
        ampsPhasecDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(ampsPhasecDataPoint.name(), ampsPhasecDataPoint);

        SunSpecDataPoint A_SFDataPoint;
        A_SFDataPoint.setName("A_SF");
        A_SFDataPoint.setMandatory(true);
        A_SFDataPoint.setSize(1);
        A_SFDataPoint.setAddressOffset(6);
        A_SFDataPoint.setBlockOffset(4);
        A_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(A_SFDataPoint.name(), A_SFDataPoint);

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PPVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setSize(1);
        phaseVoltageAbDataPoint.setAddressOffset(7);
        phaseVoltageAbDataPoint.setBlockOffset(5);
        phaseVoltageAbDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(phaseVoltageAbDataPoint.name(), phaseVoltageAbDataPoint);

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PPVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setSize(1);
        phaseVoltageBcDataPoint.setAddressOffset(8);
        phaseVoltageBcDataPoint.setBlockOffset(6);
        phaseVoltageBcDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(phaseVoltageBcDataPoint.name(), phaseVoltageBcDataPoint);

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PPVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setSize(1);
        phaseVoltageCaDataPoint.setAddressOffset(9);
        phaseVoltageCaDataPoint.setBlockOffset(7);
        phaseVoltageCaDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(phaseVoltageCaDataPoint.name(), phaseVoltageCaDataPoint);

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
        phaseVoltageAnDataPoint.setMandatory(true);
        phaseVoltageAnDataPoint.setSize(1);
        phaseVoltageAnDataPoint.setAddressOffset(10);
        phaseVoltageAnDataPoint.setBlockOffset(8);
        phaseVoltageAnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(phaseVoltageAnDataPoint.name(), phaseVoltageAnDataPoint);

        SunSpecDataPoint phaseVoltageBnDataPoint;
        phaseVoltageBnDataPoint.setName("PhVphB");
        phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
        phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
        phaseVoltageBnDataPoint.setUnits("V");
        phaseVoltageBnDataPoint.setMandatory(true);
        phaseVoltageBnDataPoint.setSize(1);
        phaseVoltageBnDataPoint.setAddressOffset(11);
        phaseVoltageBnDataPoint.setBlockOffset(9);
        phaseVoltageBnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(phaseVoltageBnDataPoint.name(), phaseVoltageBnDataPoint);

        SunSpecDataPoint phaseVoltageCnDataPoint;
        phaseVoltageCnDataPoint.setName("PhVphC");
        phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
        phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
        phaseVoltageCnDataPoint.setUnits("V");
        phaseVoltageCnDataPoint.setMandatory(true);
        phaseVoltageCnDataPoint.setSize(1);
        phaseVoltageCnDataPoint.setAddressOffset(12);
        phaseVoltageCnDataPoint.setBlockOffset(10);
        phaseVoltageCnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(phaseVoltageCnDataPoint.name(), phaseVoltageCnDataPoint);

        SunSpecDataPoint V_SFDataPoint;
        V_SFDataPoint.setName("V_SF");
        V_SFDataPoint.setMandatory(true);
        V_SFDataPoint.setSize(1);
        V_SFDataPoint.setAddressOffset(13);
        V_SFDataPoint.setBlockOffset(11);
        V_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(V_SFDataPoint.name(), V_SFDataPoint);

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("AC Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(1);
        wattsDataPoint.setAddressOffset(14);
        wattsDataPoint.setBlockOffset(12);
        wattsDataPoint.setScaleFactorName("W_SF");
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsDataPoint.name(), wattsDataPoint);

        SunSpecDataPoint W_SFDataPoint;
        W_SFDataPoint.setName("W_SF");
        W_SFDataPoint.setMandatory(true);
        W_SFDataPoint.setSize(1);
        W_SFDataPoint.setAddressOffset(15);
        W_SFDataPoint.setBlockOffset(13);
        W_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(W_SFDataPoint.name(), W_SFDataPoint);

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Line Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(1);
        hzDataPoint.setAddressOffset(16);
        hzDataPoint.setBlockOffset(14);
        hzDataPoint.setScaleFactorName("Hz_SF");
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(hzDataPoint.name(), hzDataPoint);

        SunSpecDataPoint Hz_SFDataPoint;
        Hz_SFDataPoint.setName("Hz_SF");
        Hz_SFDataPoint.setMandatory(true);
        Hz_SFDataPoint.setSize(1);
        Hz_SFDataPoint.setAddressOffset(17);
        Hz_SFDataPoint.setBlockOffset(15);
        Hz_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(Hz_SFDataPoint.name(), Hz_SFDataPoint);

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(1);
        vaDataPoint.setAddressOffset(18);
        vaDataPoint.setBlockOffset(16);
        vaDataPoint.setScaleFactorName("VA_SF");
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(vaDataPoint.name(), vaDataPoint);

        SunSpecDataPoint VA_SFDataPoint;
        VA_SFDataPoint.setName("VA_SF");
        VA_SFDataPoint.setSize(1);
        VA_SFDataPoint.setAddressOffset(19);
        VA_SFDataPoint.setBlockOffset(17);
        VA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(VA_SFDataPoint.name(), VA_SFDataPoint);

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAr");
        varDataPoint.setLabel("VAr");
        varDataPoint.setDescription("AC Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(1);
        varDataPoint.setAddressOffset(20);
        varDataPoint.setBlockOffset(18);
        varDataPoint.setScaleFactorName("VAr_SF");
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(varDataPoint.name(), varDataPoint);

        SunSpecDataPoint VAr_SFDataPoint;
        VAr_SFDataPoint.setName("VAr_SF");
        VAr_SFDataPoint.setSize(1);
        VAr_SFDataPoint.setAddressOffset(21);
        VAr_SFDataPoint.setBlockOffset(19);
        VAr_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(VAr_SFDataPoint.name(), VAr_SFDataPoint);

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("AC Power Factor");
        pfDataPoint.setUnits("Pct");
        pfDataPoint.setSize(1);
        pfDataPoint.setAddressOffset(22);
        pfDataPoint.setBlockOffset(20);
        pfDataPoint.setScaleFactorName("PF_SF");
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(pfDataPoint.name(), pfDataPoint);

        SunSpecDataPoint PF_SFDataPoint;
        PF_SFDataPoint.setName("PF_SF");
        PF_SFDataPoint.setSize(1);
        PF_SFDataPoint.setAddressOffset(23);
        PF_SFDataPoint.setBlockOffset(21);
        PF_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(PF_SFDataPoint.name(), PF_SFDataPoint);

        SunSpecDataPoint watthoursDataPoint;
        watthoursDataPoint.setName("WH");
        watthoursDataPoint.setLabel("WattHours");
        watthoursDataPoint.setDescription("AC Energy");
        watthoursDataPoint.setUnits("Wh");
        watthoursDataPoint.setMandatory(true);
        watthoursDataPoint.setSize(2);
        watthoursDataPoint.setAddressOffset(24);
        watthoursDataPoint.setBlockOffset(22);
        watthoursDataPoint.setScaleFactorName("WH_SF");
        watthoursDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(watthoursDataPoint.name(), watthoursDataPoint);

        SunSpecDataPoint WH_SFDataPoint;
        WH_SFDataPoint.setName("WH_SF");
        WH_SFDataPoint.setMandatory(true);
        WH_SFDataPoint.setSize(1);
        WH_SFDataPoint.setAddressOffset(26);
        WH_SFDataPoint.setBlockOffset(24);
        WH_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(WH_SFDataPoint.name(), WH_SFDataPoint);

        SunSpecDataPoint dcAmpsDataPoint;
        dcAmpsDataPoint.setName("DCA");
        dcAmpsDataPoint.setLabel("DC Amps");
        dcAmpsDataPoint.setDescription("DC Current");
        dcAmpsDataPoint.setUnits("A");
        dcAmpsDataPoint.setSize(1);
        dcAmpsDataPoint.setAddressOffset(27);
        dcAmpsDataPoint.setBlockOffset(25);
        dcAmpsDataPoint.setScaleFactorName("DCA_SF");
        dcAmpsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(dcAmpsDataPoint.name(), dcAmpsDataPoint);

        SunSpecDataPoint DCA_SFDataPoint;
        DCA_SFDataPoint.setName("DCA_SF");
        DCA_SFDataPoint.setSize(1);
        DCA_SFDataPoint.setAddressOffset(28);
        DCA_SFDataPoint.setBlockOffset(26);
        DCA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCA_SFDataPoint.name(), DCA_SFDataPoint);

        SunSpecDataPoint dcVoltageDataPoint;
        dcVoltageDataPoint.setName("DCV");
        dcVoltageDataPoint.setLabel("DC Voltage");
        dcVoltageDataPoint.setDescription("DC Voltage");
        dcVoltageDataPoint.setUnits("V");
        dcVoltageDataPoint.setSize(1);
        dcVoltageDataPoint.setAddressOffset(29);
        dcVoltageDataPoint.setBlockOffset(27);
        dcVoltageDataPoint.setScaleFactorName("DCV_SF");
        dcVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(dcVoltageDataPoint.name(), dcVoltageDataPoint);

        SunSpecDataPoint DCV_SFDataPoint;
        DCV_SFDataPoint.setName("DCV_SF");
        DCV_SFDataPoint.setSize(1);
        DCV_SFDataPoint.setAddressOffset(30);
        DCV_SFDataPoint.setBlockOffset(28);
        DCV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCV_SFDataPoint.name(), DCV_SFDataPoint);

        SunSpecDataPoint dcWattsDataPoint;
        dcWattsDataPoint.setName("DCW");
        dcWattsDataPoint.setLabel("DC Watts");
        dcWattsDataPoint.setDescription("DC Power");
        dcWattsDataPoint.setUnits("W");
        dcWattsDataPoint.setSize(1);
        dcWattsDataPoint.setAddressOffset(31);
        dcWattsDataPoint.setBlockOffset(29);
        dcWattsDataPoint.setScaleFactorName("DCW_SF");
        dcWattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(dcWattsDataPoint.name(), dcWattsDataPoint);

        SunSpecDataPoint DCW_SFDataPoint;
        DCW_SFDataPoint.setName("DCW_SF");
        DCW_SFDataPoint.setSize(1);
        DCW_SFDataPoint.setAddressOffset(32);
        DCW_SFDataPoint.setBlockOffset(30);
        DCW_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(DCW_SFDataPoint.name(), DCW_SFDataPoint);

        SunSpecDataPoint cabinetTemperatureDataPoint;
        cabinetTemperatureDataPoint.setName("TmpCab");
        cabinetTemperatureDataPoint.setLabel("Cabinet Temperature");
        cabinetTemperatureDataPoint.setDescription("Cabinet Temperature");
        cabinetTemperatureDataPoint.setUnits("C");
        cabinetTemperatureDataPoint.setMandatory(true);
        cabinetTemperatureDataPoint.setSize(1);
        cabinetTemperatureDataPoint.setAddressOffset(33);
        cabinetTemperatureDataPoint.setBlockOffset(31);
        cabinetTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        cabinetTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(cabinetTemperatureDataPoint.name(), cabinetTemperatureDataPoint);

        SunSpecDataPoint heatSinkTemperatureDataPoint;
        heatSinkTemperatureDataPoint.setName("TmpSnk");
        heatSinkTemperatureDataPoint.setLabel("Heat Sink Temperature");
        heatSinkTemperatureDataPoint.setDescription("Heat Sink Temperature");
        heatSinkTemperatureDataPoint.setUnits("C");
        heatSinkTemperatureDataPoint.setSize(1);
        heatSinkTemperatureDataPoint.setAddressOffset(34);
        heatSinkTemperatureDataPoint.setBlockOffset(32);
        heatSinkTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        heatSinkTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(heatSinkTemperatureDataPoint.name(), heatSinkTemperatureDataPoint);

        SunSpecDataPoint transformerTemperatureDataPoint;
        transformerTemperatureDataPoint.setName("TmpTrns");
        transformerTemperatureDataPoint.setLabel("Transformer Temperature");
        transformerTemperatureDataPoint.setDescription("Transformer Temperature");
        transformerTemperatureDataPoint.setUnits("C");
        transformerTemperatureDataPoint.setSize(1);
        transformerTemperatureDataPoint.setAddressOffset(35);
        transformerTemperatureDataPoint.setBlockOffset(33);
        transformerTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        transformerTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(transformerTemperatureDataPoint.name(), transformerTemperatureDataPoint);

        SunSpecDataPoint otherTemperatureDataPoint;
        otherTemperatureDataPoint.setName("TmpOt");
        otherTemperatureDataPoint.setLabel("Other Temperature");
        otherTemperatureDataPoint.setDescription("Other Temperature");
        otherTemperatureDataPoint.setUnits("C");
        otherTemperatureDataPoint.setSize(1);
        otherTemperatureDataPoint.setAddressOffset(36);
        otherTemperatureDataPoint.setBlockOffset(34);
        otherTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        otherTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(otherTemperatureDataPoint.name(), otherTemperatureDataPoint);

        SunSpecDataPoint Tmp_SFDataPoint;
        Tmp_SFDataPoint.setName("Tmp_SF");
        Tmp_SFDataPoint.setMandatory(true);
        Tmp_SFDataPoint.setSize(1);
        Tmp_SFDataPoint.setAddressOffset(37);
        Tmp_SFDataPoint.setBlockOffset(35);
        Tmp_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(Tmp_SFDataPoint.name(), Tmp_SFDataPoint);

        SunSpecDataPoint operatingStateDataPoint;
        operatingStateDataPoint.setName("St");
        operatingStateDataPoint.setLabel("Operating State");
        operatingStateDataPoint.setDescription("Enumerated value.  Operating state");
        operatingStateDataPoint.setMandatory(true);
        operatingStateDataPoint.setSize(1);
        operatingStateDataPoint.setAddressOffset(38);
        operatingStateDataPoint.setBlockOffset(36);
        operatingStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints.insert(operatingStateDataPoint.name(), operatingStateDataPoint);

        SunSpecDataPoint vendorOperatingStateDataPoint;
        vendorOperatingStateDataPoint.setName("StVnd");
        vendorOperatingStateDataPoint.setLabel("Vendor Operating State");
        vendorOperatingStateDataPoint.setDescription("Vendor specific operating state code");
        vendorOperatingStateDataPoint.setSize(1);
        vendorOperatingStateDataPoint.setAddressOffset(39);
        vendorOperatingStateDataPoint.setBlockOffset(37);
        vendorOperatingStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints.insert(vendorOperatingStateDataPoint.name(), vendorOperatingStateDataPoint);

        SunSpecDataPoint event1DataPoint;
        event1DataPoint.setName("Evt1");
        event1DataPoint.setLabel("Event1");
        event1DataPoint.setDescription("Bitmask value. Event fields");
        event1DataPoint.setMandatory(true);
        event1DataPoint.setSize(2);
        event1DataPoint.setAddressOffset(40);
        event1DataPoint.setBlockOffset(38);
        event1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(event1DataPoint.name(), event1DataPoint);

        SunSpecDataPoint eventBitfield2DataPoint;
        eventBitfield2DataPoint.setName("Evt2");
        eventBitfield2DataPoint.setLabel("Event Bitfield 2");
        eventBitfield2DataPoint.setDescription("Reserved for future use");
        eventBitfield2DataPoint.setMandatory(true);
        eventBitfield2DataPoint.setSize(2);
        eventBitfield2DataPoint.setAddressOffset(42);
        eventBitfield2DataPoint.setBlockOffset(40);
        eventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(eventBitfield2DataPoint.name(), eventBitfield2DataPoint);

        SunSpecDataPoint vendorEventBitfield1DataPoint;
        vendorEventBitfield1DataPoint.setName("EvtVnd1");
        vendorEventBitfield1DataPoint.setLabel("Vendor Event Bitfield 1");
        vendorEventBitfield1DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield1DataPoint.setSize(2);
        vendorEventBitfield1DataPoint.setAddressOffset(44);
        vendorEventBitfield1DataPoint.setBlockOffset(42);
        vendorEventBitfield1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(vendorEventBitfield1DataPoint.name(), vendorEventBitfield1DataPoint);

        SunSpecDataPoint vendorEventBitfield2DataPoint;
        vendorEventBitfield2DataPoint.setName("EvtVnd2");
        vendorEventBitfield2DataPoint.setLabel("Vendor Event Bitfield 2");
        vendorEventBitfield2DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield2DataPoint.setSize(2);
        vendorEventBitfield2DataPoint.setAddressOffset(46);
        vendorEventBitfield2DataPoint.setBlockOffset(44);
        vendorEventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(vendorEventBitfield2DataPoint.name(), vendorEventBitfield2DataPoint);

        SunSpecDataPoint vendorEventBitfield3DataPoint;
        vendorEventBitfield3DataPoint.setName("EvtVnd3");
        vendorEventBitfield3DataPoint.setLabel("Vendor Event Bitfield 3");
        vendorEventBitfield3DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield3DataPoint.setSize(2);
        vendorEventBitfield3DataPoint.setAddressOffset(48);
        vendorEventBitfield3DataPoint.setBlockOffset(46);
        vendorEventBitfield3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(vendorEventBitfield3DataPoint.name(), vendorEventBitfield3DataPoint);

        SunSpecDataPoint vendorEventBitfield4DataPoint;
        vendorEventBitfield4DataPoint.setName("EvtVnd4");
        vendorEventBitfield4DataPoint.setLabel("Vendor Event Bitfield 4");
        vendorEventBitfield4DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield4DataPoint.setSize(2);
        vendorEventBitfield4DataPoint.setAddressOffset(50);
        vendorEventBitfield4DataPoint.setBlockOffset(48);
        vendorEventBitfield4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(vendorEventBitfield4DataPoint.name(), vendorEventBitfield4DataPoint);

        break;
    }
    case 111: {
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

        SunSpecDataPoint ampsPhaseaDataPoint;
        ampsPhaseaDataPoint.setName("AphA");
        ampsPhaseaDataPoint.setLabel("Amps PhaseA");
        ampsPhaseaDataPoint.setDescription("Phase A Current");
        ampsPhaseaDataPoint.setUnits("A");
        ampsPhaseaDataPoint.setMandatory(true);
        ampsPhaseaDataPoint.setSize(2);
        ampsPhaseaDataPoint.setAddressOffset(4);
        ampsPhaseaDataPoint.setBlockOffset(2);
        ampsPhaseaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(ampsPhaseaDataPoint.name(), ampsPhaseaDataPoint);

        SunSpecDataPoint ampsPhasebDataPoint;
        ampsPhasebDataPoint.setName("AphB");
        ampsPhasebDataPoint.setLabel("Amps PhaseB");
        ampsPhasebDataPoint.setDescription("Phase B Current");
        ampsPhasebDataPoint.setUnits("A");
        ampsPhasebDataPoint.setSize(2);
        ampsPhasebDataPoint.setAddressOffset(6);
        ampsPhasebDataPoint.setBlockOffset(4);
        ampsPhasebDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(ampsPhasebDataPoint.name(), ampsPhasebDataPoint);

        SunSpecDataPoint ampsPhasecDataPoint;
        ampsPhasecDataPoint.setName("AphC");
        ampsPhasecDataPoint.setLabel("Amps PhaseC");
        ampsPhasecDataPoint.setDescription("Phase C Current");
        ampsPhasecDataPoint.setUnits("A");
        ampsPhasecDataPoint.setSize(2);
        ampsPhasecDataPoint.setAddressOffset(8);
        ampsPhasecDataPoint.setBlockOffset(6);
        ampsPhasecDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(ampsPhasecDataPoint.name(), ampsPhasecDataPoint);

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

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAr");
        varDataPoint.setLabel("VAr");
        varDataPoint.setDescription("AC Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(2);
        varDataPoint.setAddressOffset(28);
        varDataPoint.setBlockOffset(26);
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(varDataPoint.name(), varDataPoint);

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

        SunSpecDataPoint watthoursDataPoint;
        watthoursDataPoint.setName("WH");
        watthoursDataPoint.setLabel("WattHours");
        watthoursDataPoint.setDescription("AC Energy");
        watthoursDataPoint.setUnits("Wh");
        watthoursDataPoint.setMandatory(true);
        watthoursDataPoint.setSize(2);
        watthoursDataPoint.setAddressOffset(32);
        watthoursDataPoint.setBlockOffset(30);
        watthoursDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(watthoursDataPoint.name(), watthoursDataPoint);

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

        break;
    }
    case 112: {
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

        SunSpecDataPoint ampsPhaseaDataPoint;
        ampsPhaseaDataPoint.setName("AphA");
        ampsPhaseaDataPoint.setLabel("Amps PhaseA");
        ampsPhaseaDataPoint.setDescription("Phase A Current");
        ampsPhaseaDataPoint.setUnits("A");
        ampsPhaseaDataPoint.setMandatory(true);
        ampsPhaseaDataPoint.setSize(2);
        ampsPhaseaDataPoint.setAddressOffset(4);
        ampsPhaseaDataPoint.setBlockOffset(2);
        ampsPhaseaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(ampsPhaseaDataPoint.name(), ampsPhaseaDataPoint);

        SunSpecDataPoint ampsPhasebDataPoint;
        ampsPhasebDataPoint.setName("AphB");
        ampsPhasebDataPoint.setLabel("Amps PhaseB");
        ampsPhasebDataPoint.setDescription("Phase B Current");
        ampsPhasebDataPoint.setUnits("A");
        ampsPhasebDataPoint.setMandatory(true);
        ampsPhasebDataPoint.setSize(2);
        ampsPhasebDataPoint.setAddressOffset(6);
        ampsPhasebDataPoint.setBlockOffset(4);
        ampsPhasebDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(ampsPhasebDataPoint.name(), ampsPhasebDataPoint);

        SunSpecDataPoint ampsPhasecDataPoint;
        ampsPhasecDataPoint.setName("AphC");
        ampsPhasecDataPoint.setLabel("Amps PhaseC");
        ampsPhasecDataPoint.setDescription("Phase C Current");
        ampsPhasecDataPoint.setUnits("A");
        ampsPhasecDataPoint.setSize(2);
        ampsPhasecDataPoint.setAddressOffset(8);
        ampsPhasecDataPoint.setBlockOffset(6);
        ampsPhasecDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(ampsPhasecDataPoint.name(), ampsPhasecDataPoint);

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

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAr");
        varDataPoint.setLabel("VAr");
        varDataPoint.setDescription("AC Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(2);
        varDataPoint.setAddressOffset(28);
        varDataPoint.setBlockOffset(26);
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(varDataPoint.name(), varDataPoint);

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

        SunSpecDataPoint watthoursDataPoint;
        watthoursDataPoint.setName("WH");
        watthoursDataPoint.setLabel("WattHours");
        watthoursDataPoint.setDescription("AC Energy");
        watthoursDataPoint.setUnits("Wh");
        watthoursDataPoint.setMandatory(true);
        watthoursDataPoint.setSize(2);
        watthoursDataPoint.setAddressOffset(32);
        watthoursDataPoint.setBlockOffset(30);
        watthoursDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(watthoursDataPoint.name(), watthoursDataPoint);

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

        break;
    }
    case 113: {
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

        SunSpecDataPoint ampsPhaseaDataPoint;
        ampsPhaseaDataPoint.setName("AphA");
        ampsPhaseaDataPoint.setLabel("Amps PhaseA");
        ampsPhaseaDataPoint.setDescription("Phase A Current");
        ampsPhaseaDataPoint.setUnits("A");
        ampsPhaseaDataPoint.setMandatory(true);
        ampsPhaseaDataPoint.setSize(2);
        ampsPhaseaDataPoint.setAddressOffset(4);
        ampsPhaseaDataPoint.setBlockOffset(2);
        ampsPhaseaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(ampsPhaseaDataPoint.name(), ampsPhaseaDataPoint);

        SunSpecDataPoint ampsPhasebDataPoint;
        ampsPhasebDataPoint.setName("AphB");
        ampsPhasebDataPoint.setLabel("Amps PhaseB");
        ampsPhasebDataPoint.setDescription("Phase B Current");
        ampsPhasebDataPoint.setUnits("A");
        ampsPhasebDataPoint.setMandatory(true);
        ampsPhasebDataPoint.setSize(2);
        ampsPhasebDataPoint.setAddressOffset(6);
        ampsPhasebDataPoint.setBlockOffset(4);
        ampsPhasebDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(ampsPhasebDataPoint.name(), ampsPhasebDataPoint);

        SunSpecDataPoint ampsPhasecDataPoint;
        ampsPhasecDataPoint.setName("AphC");
        ampsPhasecDataPoint.setLabel("Amps PhaseC");
        ampsPhasecDataPoint.setDescription("Phase C Current");
        ampsPhasecDataPoint.setUnits("A");
        ampsPhasecDataPoint.setMandatory(true);
        ampsPhasecDataPoint.setSize(2);
        ampsPhasecDataPoint.setAddressOffset(8);
        ampsPhasecDataPoint.setBlockOffset(6);
        ampsPhasecDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(ampsPhasecDataPoint.name(), ampsPhasecDataPoint);

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
        phaseVoltageCnDataPoint.setMandatory(true);
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

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAr");
        varDataPoint.setLabel("VAr");
        varDataPoint.setDescription("AC Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(2);
        varDataPoint.setAddressOffset(28);
        varDataPoint.setBlockOffset(26);
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(varDataPoint.name(), varDataPoint);

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

        SunSpecDataPoint watthoursDataPoint;
        watthoursDataPoint.setName("WH");
        watthoursDataPoint.setLabel("WattHours");
        watthoursDataPoint.setDescription("AC Energy");
        watthoursDataPoint.setUnits("Wh");
        watthoursDataPoint.setMandatory(true);
        watthoursDataPoint.setSize(2);
        watthoursDataPoint.setAddressOffset(32);
        watthoursDataPoint.setBlockOffset(30);
        watthoursDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(watthoursDataPoint.name(), watthoursDataPoint);

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

        break;
    }
    default:
        break;
    }
}

