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
}

SunSpecInverterModel::~SunSpecInverterModel()
{

}

QString SunSpecInverterModel::name() const
{
    switch (m_modelId) {
    case 101:
        return "inverter";
    case 102:
        return "inverter";
    case 103:
        return "inverter";
    case 111:
        return "inverter";
    case 112:
        return "inverter";
    case 113:
        return "inverter";
    default:
        return QString();
    }
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

        SunSpecDataPoint ampsDataPoint;
        ampsDataPoint.setName("A");
        ampsDataPoint.setLabel("Amps");
        ampsDataPoint.setDescription("AC Current");
        ampsDataPoint.setUnits("A");
        ampsDataPoint.setMandatory(true);
        ampsDataPoint.setSize(1);
        ampsDataPoint.setScaleFactorName("A_SF");
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << ampsDataPoint;

        SunSpecDataPoint ampsPhaseaDataPoint;
        ampsPhaseaDataPoint.setName("AphA");
        ampsPhaseaDataPoint.setLabel("Amps PhaseA");
        ampsPhaseaDataPoint.setDescription("Phase A Current");
        ampsPhaseaDataPoint.setUnits("A");
        ampsPhaseaDataPoint.setMandatory(true);
        ampsPhaseaDataPoint.setSize(1);
        ampsPhaseaDataPoint.setScaleFactorName("A_SF");
        ampsPhaseaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << ampsPhaseaDataPoint;

        SunSpecDataPoint ampsPhasebDataPoint;
        ampsPhasebDataPoint.setName("AphB");
        ampsPhasebDataPoint.setLabel("Amps PhaseB");
        ampsPhasebDataPoint.setDescription("Phase B Current");
        ampsPhasebDataPoint.setUnits("A");
        ampsPhasebDataPoint.setSize(1);
        ampsPhasebDataPoint.setScaleFactorName("A_SF");
        ampsPhasebDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << ampsPhasebDataPoint;

        SunSpecDataPoint ampsPhasecDataPoint;
        ampsPhasecDataPoint.setName("AphC");
        ampsPhasecDataPoint.setLabel("Amps PhaseC");
        ampsPhasecDataPoint.setDescription("Phase C Current");
        ampsPhasecDataPoint.setUnits("A");
        ampsPhasecDataPoint.setSize(1);
        ampsPhasecDataPoint.setScaleFactorName("A_SF");
        ampsPhasecDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << ampsPhasecDataPoint;

        SunSpecDataPoint A_SFDataPoint;
        A_SFDataPoint.setName("A_SF");
        A_SFDataPoint.setMandatory(true);
        A_SFDataPoint.setSize(1);
        A_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << A_SFDataPoint;

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PPVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setSize(1);
        phaseVoltageAbDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << phaseVoltageAbDataPoint;

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PPVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setSize(1);
        phaseVoltageBcDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << phaseVoltageBcDataPoint;

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PPVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setSize(1);
        phaseVoltageCaDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << phaseVoltageCaDataPoint;

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
        phaseVoltageAnDataPoint.setMandatory(true);
        phaseVoltageAnDataPoint.setSize(1);
        phaseVoltageAnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << phaseVoltageAnDataPoint;

        SunSpecDataPoint phaseVoltageBnDataPoint;
        phaseVoltageBnDataPoint.setName("PhVphB");
        phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
        phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
        phaseVoltageBnDataPoint.setUnits("V");
        phaseVoltageBnDataPoint.setSize(1);
        phaseVoltageBnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << phaseVoltageBnDataPoint;

        SunSpecDataPoint phaseVoltageCnDataPoint;
        phaseVoltageCnDataPoint.setName("PhVphC");
        phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
        phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
        phaseVoltageCnDataPoint.setUnits("V");
        phaseVoltageCnDataPoint.setSize(1);
        phaseVoltageCnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << phaseVoltageCnDataPoint;

        SunSpecDataPoint V_SFDataPoint;
        V_SFDataPoint.setName("V_SF");
        V_SFDataPoint.setMandatory(true);
        V_SFDataPoint.setSize(1);
        V_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << V_SFDataPoint;

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("AC Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(1);
        wattsDataPoint.setScaleFactorName("W_SF");
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << wattsDataPoint;

        SunSpecDataPoint W_SFDataPoint;
        W_SFDataPoint.setName("W_SF");
        W_SFDataPoint.setMandatory(true);
        W_SFDataPoint.setSize(1);
        W_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << W_SFDataPoint;

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Line Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(1);
        hzDataPoint.setScaleFactorName("Hz_SF");
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << hzDataPoint;

        SunSpecDataPoint Hz_SFDataPoint;
        Hz_SFDataPoint.setName("Hz_SF");
        Hz_SFDataPoint.setMandatory(true);
        Hz_SFDataPoint.setSize(1);
        Hz_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << Hz_SFDataPoint;

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(1);
        vaDataPoint.setScaleFactorName("VA_SF");
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << vaDataPoint;

        SunSpecDataPoint VA_SFDataPoint;
        VA_SFDataPoint.setName("VA_SF");
        VA_SFDataPoint.setSize(1);
        VA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << VA_SFDataPoint;

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAr");
        varDataPoint.setLabel("VAr");
        varDataPoint.setDescription("AC Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(1);
        varDataPoint.setScaleFactorName("VAr_SF");
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << varDataPoint;

        SunSpecDataPoint VAr_SFDataPoint;
        VAr_SFDataPoint.setName("VAr_SF");
        VAr_SFDataPoint.setSize(1);
        VAr_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << VAr_SFDataPoint;

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("AC Power Factor");
        pfDataPoint.setUnits("Pct");
        pfDataPoint.setSize(1);
        pfDataPoint.setScaleFactorName("PF_SF");
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << pfDataPoint;

        SunSpecDataPoint PF_SFDataPoint;
        PF_SFDataPoint.setName("PF_SF");
        PF_SFDataPoint.setSize(1);
        PF_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << PF_SFDataPoint;

        SunSpecDataPoint watthoursDataPoint;
        watthoursDataPoint.setName("WH");
        watthoursDataPoint.setLabel("WattHours");
        watthoursDataPoint.setDescription("AC Energy");
        watthoursDataPoint.setUnits("Wh");
        watthoursDataPoint.setMandatory(true);
        watthoursDataPoint.setSize(2);
        watthoursDataPoint.setScaleFactorName("WH_SF");
        watthoursDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << watthoursDataPoint;

        SunSpecDataPoint WH_SFDataPoint;
        WH_SFDataPoint.setName("WH_SF");
        WH_SFDataPoint.setMandatory(true);
        WH_SFDataPoint.setSize(1);
        WH_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << WH_SFDataPoint;

        SunSpecDataPoint dcAmpsDataPoint;
        dcAmpsDataPoint.setName("DCA");
        dcAmpsDataPoint.setLabel("DC Amps");
        dcAmpsDataPoint.setDescription("DC Current");
        dcAmpsDataPoint.setUnits("A");
        dcAmpsDataPoint.setSize(1);
        dcAmpsDataPoint.setScaleFactorName("DCA_SF");
        dcAmpsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << dcAmpsDataPoint;

        SunSpecDataPoint DCA_SFDataPoint;
        DCA_SFDataPoint.setName("DCA_SF");
        DCA_SFDataPoint.setSize(1);
        DCA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << DCA_SFDataPoint;

        SunSpecDataPoint dcVoltageDataPoint;
        dcVoltageDataPoint.setName("DCV");
        dcVoltageDataPoint.setLabel("DC Voltage");
        dcVoltageDataPoint.setDescription("DC Voltage");
        dcVoltageDataPoint.setUnits("V");
        dcVoltageDataPoint.setSize(1);
        dcVoltageDataPoint.setScaleFactorName("DCV_SF");
        dcVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << dcVoltageDataPoint;

        SunSpecDataPoint DCV_SFDataPoint;
        DCV_SFDataPoint.setName("DCV_SF");
        DCV_SFDataPoint.setSize(1);
        DCV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << DCV_SFDataPoint;

        SunSpecDataPoint dcWattsDataPoint;
        dcWattsDataPoint.setName("DCW");
        dcWattsDataPoint.setLabel("DC Watts");
        dcWattsDataPoint.setDescription("DC Power");
        dcWattsDataPoint.setUnits("W");
        dcWattsDataPoint.setSize(1);
        dcWattsDataPoint.setScaleFactorName("DCW_SF");
        dcWattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << dcWattsDataPoint;

        SunSpecDataPoint DCW_SFDataPoint;
        DCW_SFDataPoint.setName("DCW_SF");
        DCW_SFDataPoint.setSize(1);
        DCW_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << DCW_SFDataPoint;

        SunSpecDataPoint cabinetTemperatureDataPoint;
        cabinetTemperatureDataPoint.setName("TmpCab");
        cabinetTemperatureDataPoint.setLabel("Cabinet Temperature");
        cabinetTemperatureDataPoint.setDescription("Cabinet Temperature");
        cabinetTemperatureDataPoint.setUnits("C");
        cabinetTemperatureDataPoint.setMandatory(true);
        cabinetTemperatureDataPoint.setSize(1);
        cabinetTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        cabinetTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << cabinetTemperatureDataPoint;

        SunSpecDataPoint heatSinkTemperatureDataPoint;
        heatSinkTemperatureDataPoint.setName("TmpSnk");
        heatSinkTemperatureDataPoint.setLabel("Heat Sink Temperature");
        heatSinkTemperatureDataPoint.setDescription("Heat Sink Temperature");
        heatSinkTemperatureDataPoint.setUnits("C");
        heatSinkTemperatureDataPoint.setSize(1);
        heatSinkTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        heatSinkTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << heatSinkTemperatureDataPoint;

        SunSpecDataPoint transformerTemperatureDataPoint;
        transformerTemperatureDataPoint.setName("TmpTrns");
        transformerTemperatureDataPoint.setLabel("Transformer Temperature");
        transformerTemperatureDataPoint.setDescription("Transformer Temperature");
        transformerTemperatureDataPoint.setUnits("C");
        transformerTemperatureDataPoint.setSize(1);
        transformerTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        transformerTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << transformerTemperatureDataPoint;

        SunSpecDataPoint otherTemperatureDataPoint;
        otherTemperatureDataPoint.setName("TmpOt");
        otherTemperatureDataPoint.setLabel("Other Temperature");
        otherTemperatureDataPoint.setDescription("Other Temperature");
        otherTemperatureDataPoint.setUnits("C");
        otherTemperatureDataPoint.setSize(1);
        otherTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        otherTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << otherTemperatureDataPoint;

        SunSpecDataPoint Tmp_SFDataPoint;
        Tmp_SFDataPoint.setName("Tmp_SF");
        Tmp_SFDataPoint.setMandatory(true);
        Tmp_SFDataPoint.setSize(1);
        Tmp_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << Tmp_SFDataPoint;

        SunSpecDataPoint operatingStateDataPoint;
        operatingStateDataPoint.setName("St");
        operatingStateDataPoint.setLabel("Operating State");
        operatingStateDataPoint.setDescription("Enumerated value.  Operating state");
        operatingStateDataPoint.setMandatory(true);
        operatingStateDataPoint.setSize(1);
        operatingStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints << operatingStateDataPoint;

        SunSpecDataPoint vendorOperatingStateDataPoint;
        vendorOperatingStateDataPoint.setName("StVnd");
        vendorOperatingStateDataPoint.setLabel("Vendor Operating State");
        vendorOperatingStateDataPoint.setDescription("Vendor specific operating state code");
        vendorOperatingStateDataPoint.setSize(1);
        vendorOperatingStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints << vendorOperatingStateDataPoint;

        SunSpecDataPoint event1DataPoint;
        event1DataPoint.setName("Evt1");
        event1DataPoint.setLabel("Event1");
        event1DataPoint.setDescription("Bitmask value. Event fields");
        event1DataPoint.setMandatory(true);
        event1DataPoint.setSize(2);
        event1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << event1DataPoint;

        SunSpecDataPoint eventBitfield2DataPoint;
        eventBitfield2DataPoint.setName("Evt2");
        eventBitfield2DataPoint.setLabel("Event Bitfield 2");
        eventBitfield2DataPoint.setDescription("Reserved for future use");
        eventBitfield2DataPoint.setMandatory(true);
        eventBitfield2DataPoint.setSize(2);
        eventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << eventBitfield2DataPoint;

        SunSpecDataPoint vendorEventBitfield1DataPoint;
        vendorEventBitfield1DataPoint.setName("EvtVnd1");
        vendorEventBitfield1DataPoint.setLabel("Vendor Event Bitfield 1");
        vendorEventBitfield1DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield1DataPoint.setSize(2);
        vendorEventBitfield1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield1DataPoint;

        SunSpecDataPoint vendorEventBitfield2DataPoint;
        vendorEventBitfield2DataPoint.setName("EvtVnd2");
        vendorEventBitfield2DataPoint.setLabel("Vendor Event Bitfield 2");
        vendorEventBitfield2DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield2DataPoint.setSize(2);
        vendorEventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield2DataPoint;

        SunSpecDataPoint vendorEventBitfield3DataPoint;
        vendorEventBitfield3DataPoint.setName("EvtVnd3");
        vendorEventBitfield3DataPoint.setLabel("Vendor Event Bitfield 3");
        vendorEventBitfield3DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield3DataPoint.setSize(2);
        vendorEventBitfield3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield3DataPoint;

        SunSpecDataPoint vendorEventBitfield4DataPoint;
        vendorEventBitfield4DataPoint.setName("EvtVnd4");
        vendorEventBitfield4DataPoint.setLabel("Vendor Event Bitfield 4");
        vendorEventBitfield4DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield4DataPoint.setSize(2);
        vendorEventBitfield4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield4DataPoint;

        break;
    }
    case 102: {
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

        SunSpecDataPoint ampsDataPoint;
        ampsDataPoint.setName("A");
        ampsDataPoint.setLabel("Amps");
        ampsDataPoint.setDescription("AC Current");
        ampsDataPoint.setUnits("A");
        ampsDataPoint.setMandatory(true);
        ampsDataPoint.setSize(1);
        ampsDataPoint.setScaleFactorName("A_SF");
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << ampsDataPoint;

        SunSpecDataPoint ampsPhaseaDataPoint;
        ampsPhaseaDataPoint.setName("AphA");
        ampsPhaseaDataPoint.setLabel("Amps PhaseA");
        ampsPhaseaDataPoint.setDescription("Phase A Current");
        ampsPhaseaDataPoint.setUnits("A");
        ampsPhaseaDataPoint.setMandatory(true);
        ampsPhaseaDataPoint.setSize(1);
        ampsPhaseaDataPoint.setScaleFactorName("A_SF");
        ampsPhaseaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << ampsPhaseaDataPoint;

        SunSpecDataPoint ampsPhasebDataPoint;
        ampsPhasebDataPoint.setName("AphB");
        ampsPhasebDataPoint.setLabel("Amps PhaseB");
        ampsPhasebDataPoint.setDescription("Phase B Current");
        ampsPhasebDataPoint.setUnits("A");
        ampsPhasebDataPoint.setMandatory(true);
        ampsPhasebDataPoint.setSize(1);
        ampsPhasebDataPoint.setScaleFactorName("A_SF");
        ampsPhasebDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << ampsPhasebDataPoint;

        SunSpecDataPoint ampsPhasecDataPoint;
        ampsPhasecDataPoint.setName("AphC");
        ampsPhasecDataPoint.setLabel("Amps PhaseC");
        ampsPhasecDataPoint.setDescription("Phase C Current");
        ampsPhasecDataPoint.setUnits("A");
        ampsPhasecDataPoint.setSize(1);
        ampsPhasecDataPoint.setScaleFactorName("A_SF");
        ampsPhasecDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << ampsPhasecDataPoint;

        SunSpecDataPoint A_SFDataPoint;
        A_SFDataPoint.setName("A_SF");
        A_SFDataPoint.setMandatory(true);
        A_SFDataPoint.setSize(1);
        A_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << A_SFDataPoint;

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PPVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setSize(1);
        phaseVoltageAbDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << phaseVoltageAbDataPoint;

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PPVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setSize(1);
        phaseVoltageBcDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << phaseVoltageBcDataPoint;

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PPVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setSize(1);
        phaseVoltageCaDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << phaseVoltageCaDataPoint;

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
        phaseVoltageAnDataPoint.setMandatory(true);
        phaseVoltageAnDataPoint.setSize(1);
        phaseVoltageAnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << phaseVoltageAnDataPoint;

        SunSpecDataPoint phaseVoltageBnDataPoint;
        phaseVoltageBnDataPoint.setName("PhVphB");
        phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
        phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
        phaseVoltageBnDataPoint.setUnits("V");
        phaseVoltageBnDataPoint.setMandatory(true);
        phaseVoltageBnDataPoint.setSize(1);
        phaseVoltageBnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << phaseVoltageBnDataPoint;

        SunSpecDataPoint phaseVoltageCnDataPoint;
        phaseVoltageCnDataPoint.setName("PhVphC");
        phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
        phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
        phaseVoltageCnDataPoint.setUnits("V");
        phaseVoltageCnDataPoint.setSize(1);
        phaseVoltageCnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << phaseVoltageCnDataPoint;

        SunSpecDataPoint V_SFDataPoint;
        V_SFDataPoint.setName("V_SF");
        V_SFDataPoint.setMandatory(true);
        V_SFDataPoint.setSize(1);
        V_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << V_SFDataPoint;

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("AC Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(1);
        wattsDataPoint.setScaleFactorName("W_SF");
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << wattsDataPoint;

        SunSpecDataPoint W_SFDataPoint;
        W_SFDataPoint.setName("W_SF");
        W_SFDataPoint.setMandatory(true);
        W_SFDataPoint.setSize(1);
        W_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << W_SFDataPoint;

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Line Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(1);
        hzDataPoint.setScaleFactorName("Hz_SF");
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << hzDataPoint;

        SunSpecDataPoint Hz_SFDataPoint;
        Hz_SFDataPoint.setName("Hz_SF");
        Hz_SFDataPoint.setMandatory(true);
        Hz_SFDataPoint.setSize(1);
        Hz_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << Hz_SFDataPoint;

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(1);
        vaDataPoint.setScaleFactorName("VA_SF");
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << vaDataPoint;

        SunSpecDataPoint VA_SFDataPoint;
        VA_SFDataPoint.setName("VA_SF");
        VA_SFDataPoint.setSize(1);
        VA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << VA_SFDataPoint;

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAr");
        varDataPoint.setLabel("VAr");
        varDataPoint.setDescription("AC Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(1);
        varDataPoint.setScaleFactorName("VAr_SF");
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << varDataPoint;

        SunSpecDataPoint VAr_SFDataPoint;
        VAr_SFDataPoint.setName("VAr_SF");
        VAr_SFDataPoint.setSize(1);
        VAr_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << VAr_SFDataPoint;

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("AC Power Factor");
        pfDataPoint.setUnits("Pct");
        pfDataPoint.setSize(1);
        pfDataPoint.setScaleFactorName("PF_SF");
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << pfDataPoint;

        SunSpecDataPoint PF_SFDataPoint;
        PF_SFDataPoint.setName("PF_SF");
        PF_SFDataPoint.setSize(1);
        PF_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << PF_SFDataPoint;

        SunSpecDataPoint watthoursDataPoint;
        watthoursDataPoint.setName("WH");
        watthoursDataPoint.setLabel("WattHours");
        watthoursDataPoint.setDescription("AC Energy");
        watthoursDataPoint.setUnits("Wh");
        watthoursDataPoint.setMandatory(true);
        watthoursDataPoint.setSize(2);
        watthoursDataPoint.setScaleFactorName("WH_SF");
        watthoursDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << watthoursDataPoint;

        SunSpecDataPoint WH_SFDataPoint;
        WH_SFDataPoint.setName("WH_SF");
        WH_SFDataPoint.setMandatory(true);
        WH_SFDataPoint.setSize(1);
        WH_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << WH_SFDataPoint;

        SunSpecDataPoint dcAmpsDataPoint;
        dcAmpsDataPoint.setName("DCA");
        dcAmpsDataPoint.setLabel("DC Amps");
        dcAmpsDataPoint.setDescription("DC Current");
        dcAmpsDataPoint.setUnits("A");
        dcAmpsDataPoint.setSize(1);
        dcAmpsDataPoint.setScaleFactorName("DCA_SF");
        dcAmpsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << dcAmpsDataPoint;

        SunSpecDataPoint DCA_SFDataPoint;
        DCA_SFDataPoint.setName("DCA_SF");
        DCA_SFDataPoint.setSize(1);
        DCA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << DCA_SFDataPoint;

        SunSpecDataPoint dcVoltageDataPoint;
        dcVoltageDataPoint.setName("DCV");
        dcVoltageDataPoint.setLabel("DC Voltage");
        dcVoltageDataPoint.setDescription("DC Voltage");
        dcVoltageDataPoint.setUnits("V");
        dcVoltageDataPoint.setSize(1);
        dcVoltageDataPoint.setScaleFactorName("DCV_SF");
        dcVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << dcVoltageDataPoint;

        SunSpecDataPoint DCV_SFDataPoint;
        DCV_SFDataPoint.setName("DCV_SF");
        DCV_SFDataPoint.setSize(1);
        DCV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << DCV_SFDataPoint;

        SunSpecDataPoint dcWattsDataPoint;
        dcWattsDataPoint.setName("DCW");
        dcWattsDataPoint.setLabel("DC Watts");
        dcWattsDataPoint.setDescription("DC Power");
        dcWattsDataPoint.setUnits("W");
        dcWattsDataPoint.setSize(1);
        dcWattsDataPoint.setScaleFactorName("DCW_SF");
        dcWattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << dcWattsDataPoint;

        SunSpecDataPoint DCW_SFDataPoint;
        DCW_SFDataPoint.setName("DCW_SF");
        DCW_SFDataPoint.setSize(1);
        DCW_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << DCW_SFDataPoint;

        SunSpecDataPoint cabinetTemperatureDataPoint;
        cabinetTemperatureDataPoint.setName("TmpCab");
        cabinetTemperatureDataPoint.setLabel("Cabinet Temperature");
        cabinetTemperatureDataPoint.setDescription("Cabinet Temperature");
        cabinetTemperatureDataPoint.setUnits("C");
        cabinetTemperatureDataPoint.setMandatory(true);
        cabinetTemperatureDataPoint.setSize(1);
        cabinetTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        cabinetTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << cabinetTemperatureDataPoint;

        SunSpecDataPoint heatSinkTemperatureDataPoint;
        heatSinkTemperatureDataPoint.setName("TmpSnk");
        heatSinkTemperatureDataPoint.setLabel("Heat Sink Temperature");
        heatSinkTemperatureDataPoint.setDescription("Heat Sink Temperature");
        heatSinkTemperatureDataPoint.setUnits("C");
        heatSinkTemperatureDataPoint.setSize(1);
        heatSinkTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        heatSinkTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << heatSinkTemperatureDataPoint;

        SunSpecDataPoint transformerTemperatureDataPoint;
        transformerTemperatureDataPoint.setName("TmpTrns");
        transformerTemperatureDataPoint.setLabel("Transformer Temperature");
        transformerTemperatureDataPoint.setDescription("Transformer Temperature");
        transformerTemperatureDataPoint.setUnits("C");
        transformerTemperatureDataPoint.setSize(1);
        transformerTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        transformerTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << transformerTemperatureDataPoint;

        SunSpecDataPoint otherTemperatureDataPoint;
        otherTemperatureDataPoint.setName("TmpOt");
        otherTemperatureDataPoint.setLabel("Other Temperature");
        otherTemperatureDataPoint.setDescription("Other Temperature");
        otherTemperatureDataPoint.setUnits("C");
        otherTemperatureDataPoint.setSize(1);
        otherTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        otherTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << otherTemperatureDataPoint;

        SunSpecDataPoint Tmp_SFDataPoint;
        Tmp_SFDataPoint.setName("Tmp_SF");
        Tmp_SFDataPoint.setMandatory(true);
        Tmp_SFDataPoint.setSize(1);
        Tmp_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << Tmp_SFDataPoint;

        SunSpecDataPoint operatingStateDataPoint;
        operatingStateDataPoint.setName("St");
        operatingStateDataPoint.setLabel("Operating State");
        operatingStateDataPoint.setDescription("Enumerated value.  Operating state");
        operatingStateDataPoint.setMandatory(true);
        operatingStateDataPoint.setSize(1);
        operatingStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints << operatingStateDataPoint;

        SunSpecDataPoint vendorOperatingStateDataPoint;
        vendorOperatingStateDataPoint.setName("StVnd");
        vendorOperatingStateDataPoint.setLabel("Vendor Operating State");
        vendorOperatingStateDataPoint.setDescription("Vendor specific operating state code");
        vendorOperatingStateDataPoint.setSize(1);
        vendorOperatingStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints << vendorOperatingStateDataPoint;

        SunSpecDataPoint event1DataPoint;
        event1DataPoint.setName("Evt1");
        event1DataPoint.setLabel("Event1");
        event1DataPoint.setDescription("Bitmask value. Event fields");
        event1DataPoint.setMandatory(true);
        event1DataPoint.setSize(2);
        event1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << event1DataPoint;

        SunSpecDataPoint eventBitfield2DataPoint;
        eventBitfield2DataPoint.setName("Evt2");
        eventBitfield2DataPoint.setLabel("Event Bitfield 2");
        eventBitfield2DataPoint.setDescription("Reserved for future use");
        eventBitfield2DataPoint.setMandatory(true);
        eventBitfield2DataPoint.setSize(2);
        eventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << eventBitfield2DataPoint;

        SunSpecDataPoint vendorEventBitfield1DataPoint;
        vendorEventBitfield1DataPoint.setName("EvtVnd1");
        vendorEventBitfield1DataPoint.setLabel("Vendor Event Bitfield 1");
        vendorEventBitfield1DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield1DataPoint.setSize(2);
        vendorEventBitfield1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield1DataPoint;

        SunSpecDataPoint vendorEventBitfield2DataPoint;
        vendorEventBitfield2DataPoint.setName("EvtVnd2");
        vendorEventBitfield2DataPoint.setLabel("Vendor Event Bitfield 2");
        vendorEventBitfield2DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield2DataPoint.setSize(2);
        vendorEventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield2DataPoint;

        SunSpecDataPoint vendorEventBitfield3DataPoint;
        vendorEventBitfield3DataPoint.setName("EvtVnd3");
        vendorEventBitfield3DataPoint.setLabel("Vendor Event Bitfield 3");
        vendorEventBitfield3DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield3DataPoint.setSize(2);
        vendorEventBitfield3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield3DataPoint;

        SunSpecDataPoint vendorEventBitfield4DataPoint;
        vendorEventBitfield4DataPoint.setName("EvtVnd4");
        vendorEventBitfield4DataPoint.setLabel("Vendor Event Bitfield 4");
        vendorEventBitfield4DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield4DataPoint.setSize(2);
        vendorEventBitfield4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield4DataPoint;

        break;
    }
    case 103: {
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

        SunSpecDataPoint ampsDataPoint;
        ampsDataPoint.setName("A");
        ampsDataPoint.setLabel("Amps");
        ampsDataPoint.setDescription("AC Current");
        ampsDataPoint.setUnits("A");
        ampsDataPoint.setMandatory(true);
        ampsDataPoint.setSize(1);
        ampsDataPoint.setScaleFactorName("A_SF");
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << ampsDataPoint;

        SunSpecDataPoint ampsPhaseaDataPoint;
        ampsPhaseaDataPoint.setName("AphA");
        ampsPhaseaDataPoint.setLabel("Amps PhaseA");
        ampsPhaseaDataPoint.setDescription("Phase A Current");
        ampsPhaseaDataPoint.setUnits("A");
        ampsPhaseaDataPoint.setMandatory(true);
        ampsPhaseaDataPoint.setSize(1);
        ampsPhaseaDataPoint.setScaleFactorName("A_SF");
        ampsPhaseaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << ampsPhaseaDataPoint;

        SunSpecDataPoint ampsPhasebDataPoint;
        ampsPhasebDataPoint.setName("AphB");
        ampsPhasebDataPoint.setLabel("Amps PhaseB");
        ampsPhasebDataPoint.setDescription("Phase B Current");
        ampsPhasebDataPoint.setUnits("A");
        ampsPhasebDataPoint.setMandatory(true);
        ampsPhasebDataPoint.setSize(1);
        ampsPhasebDataPoint.setScaleFactorName("A_SF");
        ampsPhasebDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << ampsPhasebDataPoint;

        SunSpecDataPoint ampsPhasecDataPoint;
        ampsPhasecDataPoint.setName("AphC");
        ampsPhasecDataPoint.setLabel("Amps PhaseC");
        ampsPhasecDataPoint.setDescription("Phase C Current");
        ampsPhasecDataPoint.setUnits("A");
        ampsPhasecDataPoint.setMandatory(true);
        ampsPhasecDataPoint.setSize(1);
        ampsPhasecDataPoint.setScaleFactorName("A_SF");
        ampsPhasecDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << ampsPhasecDataPoint;

        SunSpecDataPoint A_SFDataPoint;
        A_SFDataPoint.setName("A_SF");
        A_SFDataPoint.setMandatory(true);
        A_SFDataPoint.setSize(1);
        A_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << A_SFDataPoint;

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PPVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setSize(1);
        phaseVoltageAbDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << phaseVoltageAbDataPoint;

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PPVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setSize(1);
        phaseVoltageBcDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << phaseVoltageBcDataPoint;

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PPVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setSize(1);
        phaseVoltageCaDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << phaseVoltageCaDataPoint;

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
        phaseVoltageAnDataPoint.setMandatory(true);
        phaseVoltageAnDataPoint.setSize(1);
        phaseVoltageAnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << phaseVoltageAnDataPoint;

        SunSpecDataPoint phaseVoltageBnDataPoint;
        phaseVoltageBnDataPoint.setName("PhVphB");
        phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
        phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
        phaseVoltageBnDataPoint.setUnits("V");
        phaseVoltageBnDataPoint.setMandatory(true);
        phaseVoltageBnDataPoint.setSize(1);
        phaseVoltageBnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << phaseVoltageBnDataPoint;

        SunSpecDataPoint phaseVoltageCnDataPoint;
        phaseVoltageCnDataPoint.setName("PhVphC");
        phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
        phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
        phaseVoltageCnDataPoint.setUnits("V");
        phaseVoltageCnDataPoint.setMandatory(true);
        phaseVoltageCnDataPoint.setSize(1);
        phaseVoltageCnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << phaseVoltageCnDataPoint;

        SunSpecDataPoint V_SFDataPoint;
        V_SFDataPoint.setName("V_SF");
        V_SFDataPoint.setMandatory(true);
        V_SFDataPoint.setSize(1);
        V_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << V_SFDataPoint;

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("AC Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(1);
        wattsDataPoint.setScaleFactorName("W_SF");
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << wattsDataPoint;

        SunSpecDataPoint W_SFDataPoint;
        W_SFDataPoint.setName("W_SF");
        W_SFDataPoint.setMandatory(true);
        W_SFDataPoint.setSize(1);
        W_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << W_SFDataPoint;

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Line Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(1);
        hzDataPoint.setScaleFactorName("Hz_SF");
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << hzDataPoint;

        SunSpecDataPoint Hz_SFDataPoint;
        Hz_SFDataPoint.setName("Hz_SF");
        Hz_SFDataPoint.setMandatory(true);
        Hz_SFDataPoint.setSize(1);
        Hz_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << Hz_SFDataPoint;

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(1);
        vaDataPoint.setScaleFactorName("VA_SF");
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << vaDataPoint;

        SunSpecDataPoint VA_SFDataPoint;
        VA_SFDataPoint.setName("VA_SF");
        VA_SFDataPoint.setSize(1);
        VA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << VA_SFDataPoint;

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAr");
        varDataPoint.setLabel("VAr");
        varDataPoint.setDescription("AC Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(1);
        varDataPoint.setScaleFactorName("VAr_SF");
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << varDataPoint;

        SunSpecDataPoint VAr_SFDataPoint;
        VAr_SFDataPoint.setName("VAr_SF");
        VAr_SFDataPoint.setSize(1);
        VAr_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << VAr_SFDataPoint;

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("AC Power Factor");
        pfDataPoint.setUnits("Pct");
        pfDataPoint.setSize(1);
        pfDataPoint.setScaleFactorName("PF_SF");
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << pfDataPoint;

        SunSpecDataPoint PF_SFDataPoint;
        PF_SFDataPoint.setName("PF_SF");
        PF_SFDataPoint.setSize(1);
        PF_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << PF_SFDataPoint;

        SunSpecDataPoint watthoursDataPoint;
        watthoursDataPoint.setName("WH");
        watthoursDataPoint.setLabel("WattHours");
        watthoursDataPoint.setDescription("AC Energy");
        watthoursDataPoint.setUnits("Wh");
        watthoursDataPoint.setMandatory(true);
        watthoursDataPoint.setSize(2);
        watthoursDataPoint.setScaleFactorName("WH_SF");
        watthoursDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << watthoursDataPoint;

        SunSpecDataPoint WH_SFDataPoint;
        WH_SFDataPoint.setName("WH_SF");
        WH_SFDataPoint.setMandatory(true);
        WH_SFDataPoint.setSize(1);
        WH_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << WH_SFDataPoint;

        SunSpecDataPoint dcAmpsDataPoint;
        dcAmpsDataPoint.setName("DCA");
        dcAmpsDataPoint.setLabel("DC Amps");
        dcAmpsDataPoint.setDescription("DC Current");
        dcAmpsDataPoint.setUnits("A");
        dcAmpsDataPoint.setSize(1);
        dcAmpsDataPoint.setScaleFactorName("DCA_SF");
        dcAmpsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << dcAmpsDataPoint;

        SunSpecDataPoint DCA_SFDataPoint;
        DCA_SFDataPoint.setName("DCA_SF");
        DCA_SFDataPoint.setSize(1);
        DCA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << DCA_SFDataPoint;

        SunSpecDataPoint dcVoltageDataPoint;
        dcVoltageDataPoint.setName("DCV");
        dcVoltageDataPoint.setLabel("DC Voltage");
        dcVoltageDataPoint.setDescription("DC Voltage");
        dcVoltageDataPoint.setUnits("V");
        dcVoltageDataPoint.setSize(1);
        dcVoltageDataPoint.setScaleFactorName("DCV_SF");
        dcVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << dcVoltageDataPoint;

        SunSpecDataPoint DCV_SFDataPoint;
        DCV_SFDataPoint.setName("DCV_SF");
        DCV_SFDataPoint.setSize(1);
        DCV_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << DCV_SFDataPoint;

        SunSpecDataPoint dcWattsDataPoint;
        dcWattsDataPoint.setName("DCW");
        dcWattsDataPoint.setLabel("DC Watts");
        dcWattsDataPoint.setDescription("DC Power");
        dcWattsDataPoint.setUnits("W");
        dcWattsDataPoint.setSize(1);
        dcWattsDataPoint.setScaleFactorName("DCW_SF");
        dcWattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << dcWattsDataPoint;

        SunSpecDataPoint DCW_SFDataPoint;
        DCW_SFDataPoint.setName("DCW_SF");
        DCW_SFDataPoint.setSize(1);
        DCW_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << DCW_SFDataPoint;

        SunSpecDataPoint cabinetTemperatureDataPoint;
        cabinetTemperatureDataPoint.setName("TmpCab");
        cabinetTemperatureDataPoint.setLabel("Cabinet Temperature");
        cabinetTemperatureDataPoint.setDescription("Cabinet Temperature");
        cabinetTemperatureDataPoint.setUnits("C");
        cabinetTemperatureDataPoint.setMandatory(true);
        cabinetTemperatureDataPoint.setSize(1);
        cabinetTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        cabinetTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << cabinetTemperatureDataPoint;

        SunSpecDataPoint heatSinkTemperatureDataPoint;
        heatSinkTemperatureDataPoint.setName("TmpSnk");
        heatSinkTemperatureDataPoint.setLabel("Heat Sink Temperature");
        heatSinkTemperatureDataPoint.setDescription("Heat Sink Temperature");
        heatSinkTemperatureDataPoint.setUnits("C");
        heatSinkTemperatureDataPoint.setSize(1);
        heatSinkTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        heatSinkTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << heatSinkTemperatureDataPoint;

        SunSpecDataPoint transformerTemperatureDataPoint;
        transformerTemperatureDataPoint.setName("TmpTrns");
        transformerTemperatureDataPoint.setLabel("Transformer Temperature");
        transformerTemperatureDataPoint.setDescription("Transformer Temperature");
        transformerTemperatureDataPoint.setUnits("C");
        transformerTemperatureDataPoint.setSize(1);
        transformerTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        transformerTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << transformerTemperatureDataPoint;

        SunSpecDataPoint otherTemperatureDataPoint;
        otherTemperatureDataPoint.setName("TmpOt");
        otherTemperatureDataPoint.setLabel("Other Temperature");
        otherTemperatureDataPoint.setDescription("Other Temperature");
        otherTemperatureDataPoint.setUnits("C");
        otherTemperatureDataPoint.setSize(1);
        otherTemperatureDataPoint.setScaleFactorName("Tmp_SF");
        otherTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << otherTemperatureDataPoint;

        SunSpecDataPoint Tmp_SFDataPoint;
        Tmp_SFDataPoint.setName("Tmp_SF");
        Tmp_SFDataPoint.setMandatory(true);
        Tmp_SFDataPoint.setSize(1);
        Tmp_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << Tmp_SFDataPoint;

        SunSpecDataPoint operatingStateDataPoint;
        operatingStateDataPoint.setName("St");
        operatingStateDataPoint.setLabel("Operating State");
        operatingStateDataPoint.setDescription("Enumerated value.  Operating state");
        operatingStateDataPoint.setMandatory(true);
        operatingStateDataPoint.setSize(1);
        operatingStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints << operatingStateDataPoint;

        SunSpecDataPoint vendorOperatingStateDataPoint;
        vendorOperatingStateDataPoint.setName("StVnd");
        vendorOperatingStateDataPoint.setLabel("Vendor Operating State");
        vendorOperatingStateDataPoint.setDescription("Vendor specific operating state code");
        vendorOperatingStateDataPoint.setSize(1);
        vendorOperatingStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints << vendorOperatingStateDataPoint;

        SunSpecDataPoint event1DataPoint;
        event1DataPoint.setName("Evt1");
        event1DataPoint.setLabel("Event1");
        event1DataPoint.setDescription("Bitmask value. Event fields");
        event1DataPoint.setMandatory(true);
        event1DataPoint.setSize(2);
        event1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << event1DataPoint;

        SunSpecDataPoint eventBitfield2DataPoint;
        eventBitfield2DataPoint.setName("Evt2");
        eventBitfield2DataPoint.setLabel("Event Bitfield 2");
        eventBitfield2DataPoint.setDescription("Reserved for future use");
        eventBitfield2DataPoint.setMandatory(true);
        eventBitfield2DataPoint.setSize(2);
        eventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << eventBitfield2DataPoint;

        SunSpecDataPoint vendorEventBitfield1DataPoint;
        vendorEventBitfield1DataPoint.setName("EvtVnd1");
        vendorEventBitfield1DataPoint.setLabel("Vendor Event Bitfield 1");
        vendorEventBitfield1DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield1DataPoint.setSize(2);
        vendorEventBitfield1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield1DataPoint;

        SunSpecDataPoint vendorEventBitfield2DataPoint;
        vendorEventBitfield2DataPoint.setName("EvtVnd2");
        vendorEventBitfield2DataPoint.setLabel("Vendor Event Bitfield 2");
        vendorEventBitfield2DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield2DataPoint.setSize(2);
        vendorEventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield2DataPoint;

        SunSpecDataPoint vendorEventBitfield3DataPoint;
        vendorEventBitfield3DataPoint.setName("EvtVnd3");
        vendorEventBitfield3DataPoint.setLabel("Vendor Event Bitfield 3");
        vendorEventBitfield3DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield3DataPoint.setSize(2);
        vendorEventBitfield3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield3DataPoint;

        SunSpecDataPoint vendorEventBitfield4DataPoint;
        vendorEventBitfield4DataPoint.setName("EvtVnd4");
        vendorEventBitfield4DataPoint.setLabel("Vendor Event Bitfield 4");
        vendorEventBitfield4DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield4DataPoint.setSize(2);
        vendorEventBitfield4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield4DataPoint;

        break;
    }
    case 111: {
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

        SunSpecDataPoint ampsDataPoint;
        ampsDataPoint.setName("A");
        ampsDataPoint.setLabel("Amps");
        ampsDataPoint.setDescription("AC Current");
        ampsDataPoint.setUnits("A");
        ampsDataPoint.setMandatory(true);
        ampsDataPoint.setSize(2);
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << ampsDataPoint;

        SunSpecDataPoint ampsPhaseaDataPoint;
        ampsPhaseaDataPoint.setName("AphA");
        ampsPhaseaDataPoint.setLabel("Amps PhaseA");
        ampsPhaseaDataPoint.setDescription("Phase A Current");
        ampsPhaseaDataPoint.setUnits("A");
        ampsPhaseaDataPoint.setMandatory(true);
        ampsPhaseaDataPoint.setSize(2);
        ampsPhaseaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << ampsPhaseaDataPoint;

        SunSpecDataPoint ampsPhasebDataPoint;
        ampsPhasebDataPoint.setName("AphB");
        ampsPhasebDataPoint.setLabel("Amps PhaseB");
        ampsPhasebDataPoint.setDescription("Phase B Current");
        ampsPhasebDataPoint.setUnits("A");
        ampsPhasebDataPoint.setSize(2);
        ampsPhasebDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << ampsPhasebDataPoint;

        SunSpecDataPoint ampsPhasecDataPoint;
        ampsPhasecDataPoint.setName("AphC");
        ampsPhasecDataPoint.setLabel("Amps PhaseC");
        ampsPhasecDataPoint.setDescription("Phase C Current");
        ampsPhasecDataPoint.setUnits("A");
        ampsPhasecDataPoint.setSize(2);
        ampsPhasecDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << ampsPhasecDataPoint;

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PPVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setSize(2);
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageAbDataPoint;

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PPVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setSize(2);
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageBcDataPoint;

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PPVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setSize(2);
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageCaDataPoint;

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
        phaseVoltageAnDataPoint.setMandatory(true);
        phaseVoltageAnDataPoint.setSize(2);
        phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageAnDataPoint;

        SunSpecDataPoint phaseVoltageBnDataPoint;
        phaseVoltageBnDataPoint.setName("PhVphB");
        phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
        phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
        phaseVoltageBnDataPoint.setUnits("V");
        phaseVoltageBnDataPoint.setSize(2);
        phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageBnDataPoint;

        SunSpecDataPoint phaseVoltageCnDataPoint;
        phaseVoltageCnDataPoint.setName("PhVphC");
        phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
        phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
        phaseVoltageCnDataPoint.setUnits("V");
        phaseVoltageCnDataPoint.setSize(2);
        phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageCnDataPoint;

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("AC Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(2);
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << wattsDataPoint;

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Line Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(2);
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << hzDataPoint;

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(2);
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << vaDataPoint;

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAr");
        varDataPoint.setLabel("VAr");
        varDataPoint.setDescription("AC Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(2);
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << varDataPoint;

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("AC Power Factor");
        pfDataPoint.setUnits("Pct");
        pfDataPoint.setSize(2);
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << pfDataPoint;

        SunSpecDataPoint watthoursDataPoint;
        watthoursDataPoint.setName("WH");
        watthoursDataPoint.setLabel("WattHours");
        watthoursDataPoint.setDescription("AC Energy");
        watthoursDataPoint.setUnits("Wh");
        watthoursDataPoint.setMandatory(true);
        watthoursDataPoint.setSize(2);
        watthoursDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << watthoursDataPoint;

        SunSpecDataPoint dcAmpsDataPoint;
        dcAmpsDataPoint.setName("DCA");
        dcAmpsDataPoint.setLabel("DC Amps");
        dcAmpsDataPoint.setDescription("DC Current");
        dcAmpsDataPoint.setUnits("A");
        dcAmpsDataPoint.setSize(2);
        dcAmpsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << dcAmpsDataPoint;

        SunSpecDataPoint dcVoltageDataPoint;
        dcVoltageDataPoint.setName("DCV");
        dcVoltageDataPoint.setLabel("DC Voltage");
        dcVoltageDataPoint.setDescription("DC Voltage");
        dcVoltageDataPoint.setUnits("V");
        dcVoltageDataPoint.setSize(2);
        dcVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << dcVoltageDataPoint;

        SunSpecDataPoint dcWattsDataPoint;
        dcWattsDataPoint.setName("DCW");
        dcWattsDataPoint.setLabel("DC Watts");
        dcWattsDataPoint.setDescription("DC Power");
        dcWattsDataPoint.setUnits("W");
        dcWattsDataPoint.setSize(2);
        dcWattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << dcWattsDataPoint;

        SunSpecDataPoint cabinetTemperatureDataPoint;
        cabinetTemperatureDataPoint.setName("TmpCab");
        cabinetTemperatureDataPoint.setLabel("Cabinet Temperature");
        cabinetTemperatureDataPoint.setDescription("Cabinet Temperature");
        cabinetTemperatureDataPoint.setUnits("C");
        cabinetTemperatureDataPoint.setMandatory(true);
        cabinetTemperatureDataPoint.setSize(2);
        cabinetTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << cabinetTemperatureDataPoint;

        SunSpecDataPoint heatSinkTemperatureDataPoint;
        heatSinkTemperatureDataPoint.setName("TmpSnk");
        heatSinkTemperatureDataPoint.setLabel("Heat Sink Temperature");
        heatSinkTemperatureDataPoint.setDescription("Heat Sink Temperature");
        heatSinkTemperatureDataPoint.setUnits("C");
        heatSinkTemperatureDataPoint.setSize(2);
        heatSinkTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << heatSinkTemperatureDataPoint;

        SunSpecDataPoint transformerTemperatureDataPoint;
        transformerTemperatureDataPoint.setName("TmpTrns");
        transformerTemperatureDataPoint.setLabel("Transformer Temperature");
        transformerTemperatureDataPoint.setDescription("Transformer Temperature");
        transformerTemperatureDataPoint.setUnits("C");
        transformerTemperatureDataPoint.setSize(2);
        transformerTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << transformerTemperatureDataPoint;

        SunSpecDataPoint otherTemperatureDataPoint;
        otherTemperatureDataPoint.setName("TmpOt");
        otherTemperatureDataPoint.setLabel("Other Temperature");
        otherTemperatureDataPoint.setDescription("Other Temperature");
        otherTemperatureDataPoint.setUnits("C");
        otherTemperatureDataPoint.setSize(2);
        otherTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << otherTemperatureDataPoint;

        SunSpecDataPoint operatingStateDataPoint;
        operatingStateDataPoint.setName("St");
        operatingStateDataPoint.setLabel("Operating State");
        operatingStateDataPoint.setDescription("Enumerated value.  Operating state");
        operatingStateDataPoint.setMandatory(true);
        operatingStateDataPoint.setSize(1);
        operatingStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints << operatingStateDataPoint;

        SunSpecDataPoint vendorOperatingStateDataPoint;
        vendorOperatingStateDataPoint.setName("StVnd");
        vendorOperatingStateDataPoint.setLabel("Vendor Operating State");
        vendorOperatingStateDataPoint.setDescription("Vendor specific operating state code");
        vendorOperatingStateDataPoint.setSize(1);
        vendorOperatingStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints << vendorOperatingStateDataPoint;

        SunSpecDataPoint event1DataPoint;
        event1DataPoint.setName("Evt1");
        event1DataPoint.setLabel("Event1");
        event1DataPoint.setDescription("Bitmask value. Event fields");
        event1DataPoint.setMandatory(true);
        event1DataPoint.setSize(2);
        event1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << event1DataPoint;

        SunSpecDataPoint eventBitfield2DataPoint;
        eventBitfield2DataPoint.setName("Evt2");
        eventBitfield2DataPoint.setLabel("Event Bitfield 2");
        eventBitfield2DataPoint.setDescription("Reserved for future use");
        eventBitfield2DataPoint.setMandatory(true);
        eventBitfield2DataPoint.setSize(2);
        eventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << eventBitfield2DataPoint;

        SunSpecDataPoint vendorEventBitfield1DataPoint;
        vendorEventBitfield1DataPoint.setName("EvtVnd1");
        vendorEventBitfield1DataPoint.setLabel("Vendor Event Bitfield 1");
        vendorEventBitfield1DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield1DataPoint.setSize(2);
        vendorEventBitfield1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield1DataPoint;

        SunSpecDataPoint vendorEventBitfield2DataPoint;
        vendorEventBitfield2DataPoint.setName("EvtVnd2");
        vendorEventBitfield2DataPoint.setLabel("Vendor Event Bitfield 2");
        vendorEventBitfield2DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield2DataPoint.setSize(2);
        vendorEventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield2DataPoint;

        SunSpecDataPoint vendorEventBitfield3DataPoint;
        vendorEventBitfield3DataPoint.setName("EvtVnd3");
        vendorEventBitfield3DataPoint.setLabel("Vendor Event Bitfield 3");
        vendorEventBitfield3DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield3DataPoint.setSize(2);
        vendorEventBitfield3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield3DataPoint;

        SunSpecDataPoint vendorEventBitfield4DataPoint;
        vendorEventBitfield4DataPoint.setName("EvtVnd4");
        vendorEventBitfield4DataPoint.setLabel("Vendor Event Bitfield 4");
        vendorEventBitfield4DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield4DataPoint.setSize(2);
        vendorEventBitfield4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield4DataPoint;

        break;
    }
    case 112: {
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

        SunSpecDataPoint ampsDataPoint;
        ampsDataPoint.setName("A");
        ampsDataPoint.setLabel("Amps");
        ampsDataPoint.setDescription("AC Current");
        ampsDataPoint.setUnits("A");
        ampsDataPoint.setMandatory(true);
        ampsDataPoint.setSize(2);
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << ampsDataPoint;

        SunSpecDataPoint ampsPhaseaDataPoint;
        ampsPhaseaDataPoint.setName("AphA");
        ampsPhaseaDataPoint.setLabel("Amps PhaseA");
        ampsPhaseaDataPoint.setDescription("Phase A Current");
        ampsPhaseaDataPoint.setUnits("A");
        ampsPhaseaDataPoint.setMandatory(true);
        ampsPhaseaDataPoint.setSize(2);
        ampsPhaseaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << ampsPhaseaDataPoint;

        SunSpecDataPoint ampsPhasebDataPoint;
        ampsPhasebDataPoint.setName("AphB");
        ampsPhasebDataPoint.setLabel("Amps PhaseB");
        ampsPhasebDataPoint.setDescription("Phase B Current");
        ampsPhasebDataPoint.setUnits("A");
        ampsPhasebDataPoint.setMandatory(true);
        ampsPhasebDataPoint.setSize(2);
        ampsPhasebDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << ampsPhasebDataPoint;

        SunSpecDataPoint ampsPhasecDataPoint;
        ampsPhasecDataPoint.setName("AphC");
        ampsPhasecDataPoint.setLabel("Amps PhaseC");
        ampsPhasecDataPoint.setDescription("Phase C Current");
        ampsPhasecDataPoint.setUnits("A");
        ampsPhasecDataPoint.setSize(2);
        ampsPhasecDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << ampsPhasecDataPoint;

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PPVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setSize(2);
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageAbDataPoint;

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PPVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setSize(2);
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageBcDataPoint;

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PPVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setSize(2);
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageCaDataPoint;

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
        phaseVoltageAnDataPoint.setMandatory(true);
        phaseVoltageAnDataPoint.setSize(2);
        phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageAnDataPoint;

        SunSpecDataPoint phaseVoltageBnDataPoint;
        phaseVoltageBnDataPoint.setName("PhVphB");
        phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
        phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
        phaseVoltageBnDataPoint.setUnits("V");
        phaseVoltageBnDataPoint.setMandatory(true);
        phaseVoltageBnDataPoint.setSize(2);
        phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageBnDataPoint;

        SunSpecDataPoint phaseVoltageCnDataPoint;
        phaseVoltageCnDataPoint.setName("PhVphC");
        phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
        phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
        phaseVoltageCnDataPoint.setUnits("V");
        phaseVoltageCnDataPoint.setSize(2);
        phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageCnDataPoint;

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("AC Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(2);
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << wattsDataPoint;

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Line Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(2);
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << hzDataPoint;

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(2);
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << vaDataPoint;

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAr");
        varDataPoint.setLabel("VAr");
        varDataPoint.setDescription("AC Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(2);
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << varDataPoint;

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("AC Power Factor");
        pfDataPoint.setUnits("Pct");
        pfDataPoint.setSize(2);
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << pfDataPoint;

        SunSpecDataPoint watthoursDataPoint;
        watthoursDataPoint.setName("WH");
        watthoursDataPoint.setLabel("WattHours");
        watthoursDataPoint.setDescription("AC Energy");
        watthoursDataPoint.setUnits("Wh");
        watthoursDataPoint.setMandatory(true);
        watthoursDataPoint.setSize(2);
        watthoursDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << watthoursDataPoint;

        SunSpecDataPoint dcAmpsDataPoint;
        dcAmpsDataPoint.setName("DCA");
        dcAmpsDataPoint.setLabel("DC Amps");
        dcAmpsDataPoint.setDescription("DC Current");
        dcAmpsDataPoint.setUnits("A");
        dcAmpsDataPoint.setSize(2);
        dcAmpsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << dcAmpsDataPoint;

        SunSpecDataPoint dcVoltageDataPoint;
        dcVoltageDataPoint.setName("DCV");
        dcVoltageDataPoint.setLabel("DC Voltage");
        dcVoltageDataPoint.setDescription("DC Voltage");
        dcVoltageDataPoint.setUnits("V");
        dcVoltageDataPoint.setSize(2);
        dcVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << dcVoltageDataPoint;

        SunSpecDataPoint dcWattsDataPoint;
        dcWattsDataPoint.setName("DCW");
        dcWattsDataPoint.setLabel("DC Watts");
        dcWattsDataPoint.setDescription("DC Power");
        dcWattsDataPoint.setUnits("W");
        dcWattsDataPoint.setSize(2);
        dcWattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << dcWattsDataPoint;

        SunSpecDataPoint cabinetTemperatureDataPoint;
        cabinetTemperatureDataPoint.setName("TmpCab");
        cabinetTemperatureDataPoint.setLabel("Cabinet Temperature");
        cabinetTemperatureDataPoint.setDescription("Cabinet Temperature");
        cabinetTemperatureDataPoint.setUnits("C");
        cabinetTemperatureDataPoint.setMandatory(true);
        cabinetTemperatureDataPoint.setSize(2);
        cabinetTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << cabinetTemperatureDataPoint;

        SunSpecDataPoint heatSinkTemperatureDataPoint;
        heatSinkTemperatureDataPoint.setName("TmpSnk");
        heatSinkTemperatureDataPoint.setLabel("Heat Sink Temperature");
        heatSinkTemperatureDataPoint.setDescription("Heat Sink Temperature");
        heatSinkTemperatureDataPoint.setUnits("C");
        heatSinkTemperatureDataPoint.setSize(2);
        heatSinkTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << heatSinkTemperatureDataPoint;

        SunSpecDataPoint transformerTemperatureDataPoint;
        transformerTemperatureDataPoint.setName("TmpTrns");
        transformerTemperatureDataPoint.setLabel("Transformer Temperature");
        transformerTemperatureDataPoint.setDescription("Transformer Temperature");
        transformerTemperatureDataPoint.setUnits("C");
        transformerTemperatureDataPoint.setSize(2);
        transformerTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << transformerTemperatureDataPoint;

        SunSpecDataPoint otherTemperatureDataPoint;
        otherTemperatureDataPoint.setName("TmpOt");
        otherTemperatureDataPoint.setLabel("Other Temperature");
        otherTemperatureDataPoint.setDescription("Other Temperature");
        otherTemperatureDataPoint.setUnits("C");
        otherTemperatureDataPoint.setSize(2);
        otherTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << otherTemperatureDataPoint;

        SunSpecDataPoint operatingStateDataPoint;
        operatingStateDataPoint.setName("St");
        operatingStateDataPoint.setLabel("Operating State");
        operatingStateDataPoint.setDescription("Enumerated value.  Operating state");
        operatingStateDataPoint.setMandatory(true);
        operatingStateDataPoint.setSize(1);
        operatingStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints << operatingStateDataPoint;

        SunSpecDataPoint vendorOperatingStateDataPoint;
        vendorOperatingStateDataPoint.setName("StVnd");
        vendorOperatingStateDataPoint.setLabel("Vendor Operating State");
        vendorOperatingStateDataPoint.setDescription("Vendor specific operating state code");
        vendorOperatingStateDataPoint.setSize(1);
        vendorOperatingStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints << vendorOperatingStateDataPoint;

        SunSpecDataPoint event1DataPoint;
        event1DataPoint.setName("Evt1");
        event1DataPoint.setLabel("Event1");
        event1DataPoint.setDescription("Bitmask value. Event fields");
        event1DataPoint.setMandatory(true);
        event1DataPoint.setSize(2);
        event1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << event1DataPoint;

        SunSpecDataPoint eventBitfield2DataPoint;
        eventBitfield2DataPoint.setName("Evt2");
        eventBitfield2DataPoint.setLabel("Event Bitfield 2");
        eventBitfield2DataPoint.setDescription("Reserved for future use");
        eventBitfield2DataPoint.setMandatory(true);
        eventBitfield2DataPoint.setSize(2);
        eventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << eventBitfield2DataPoint;

        SunSpecDataPoint vendorEventBitfield1DataPoint;
        vendorEventBitfield1DataPoint.setName("EvtVnd1");
        vendorEventBitfield1DataPoint.setLabel("Vendor Event Bitfield 1");
        vendorEventBitfield1DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield1DataPoint.setSize(2);
        vendorEventBitfield1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield1DataPoint;

        SunSpecDataPoint vendorEventBitfield2DataPoint;
        vendorEventBitfield2DataPoint.setName("EvtVnd2");
        vendorEventBitfield2DataPoint.setLabel("Vendor Event Bitfield 2");
        vendorEventBitfield2DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield2DataPoint.setSize(2);
        vendorEventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield2DataPoint;

        SunSpecDataPoint vendorEventBitfield3DataPoint;
        vendorEventBitfield3DataPoint.setName("EvtVnd3");
        vendorEventBitfield3DataPoint.setLabel("Vendor Event Bitfield 3");
        vendorEventBitfield3DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield3DataPoint.setSize(2);
        vendorEventBitfield3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield3DataPoint;

        SunSpecDataPoint vendorEventBitfield4DataPoint;
        vendorEventBitfield4DataPoint.setName("EvtVnd4");
        vendorEventBitfield4DataPoint.setLabel("Vendor Event Bitfield 4");
        vendorEventBitfield4DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield4DataPoint.setSize(2);
        vendorEventBitfield4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield4DataPoint;

        break;
    }
    case 113: {
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

        SunSpecDataPoint ampsDataPoint;
        ampsDataPoint.setName("A");
        ampsDataPoint.setLabel("Amps");
        ampsDataPoint.setDescription("AC Current");
        ampsDataPoint.setUnits("A");
        ampsDataPoint.setMandatory(true);
        ampsDataPoint.setSize(2);
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << ampsDataPoint;

        SunSpecDataPoint ampsPhaseaDataPoint;
        ampsPhaseaDataPoint.setName("AphA");
        ampsPhaseaDataPoint.setLabel("Amps PhaseA");
        ampsPhaseaDataPoint.setDescription("Phase A Current");
        ampsPhaseaDataPoint.setUnits("A");
        ampsPhaseaDataPoint.setMandatory(true);
        ampsPhaseaDataPoint.setSize(2);
        ampsPhaseaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << ampsPhaseaDataPoint;

        SunSpecDataPoint ampsPhasebDataPoint;
        ampsPhasebDataPoint.setName("AphB");
        ampsPhasebDataPoint.setLabel("Amps PhaseB");
        ampsPhasebDataPoint.setDescription("Phase B Current");
        ampsPhasebDataPoint.setUnits("A");
        ampsPhasebDataPoint.setMandatory(true);
        ampsPhasebDataPoint.setSize(2);
        ampsPhasebDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << ampsPhasebDataPoint;

        SunSpecDataPoint ampsPhasecDataPoint;
        ampsPhasecDataPoint.setName("AphC");
        ampsPhasecDataPoint.setLabel("Amps PhaseC");
        ampsPhasecDataPoint.setDescription("Phase C Current");
        ampsPhasecDataPoint.setUnits("A");
        ampsPhasecDataPoint.setMandatory(true);
        ampsPhasecDataPoint.setSize(2);
        ampsPhasecDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << ampsPhasecDataPoint;

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PPVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setSize(2);
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageAbDataPoint;

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PPVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setSize(2);
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageBcDataPoint;

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PPVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setSize(2);
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageCaDataPoint;

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
        phaseVoltageAnDataPoint.setMandatory(true);
        phaseVoltageAnDataPoint.setSize(2);
        phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageAnDataPoint;

        SunSpecDataPoint phaseVoltageBnDataPoint;
        phaseVoltageBnDataPoint.setName("PhVphB");
        phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
        phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
        phaseVoltageBnDataPoint.setUnits("V");
        phaseVoltageBnDataPoint.setMandatory(true);
        phaseVoltageBnDataPoint.setSize(2);
        phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageBnDataPoint;

        SunSpecDataPoint phaseVoltageCnDataPoint;
        phaseVoltageCnDataPoint.setName("PhVphC");
        phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
        phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
        phaseVoltageCnDataPoint.setUnits("V");
        phaseVoltageCnDataPoint.setMandatory(true);
        phaseVoltageCnDataPoint.setSize(2);
        phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageCnDataPoint;

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("AC Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(2);
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << wattsDataPoint;

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Line Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(2);
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << hzDataPoint;

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(2);
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << vaDataPoint;

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAr");
        varDataPoint.setLabel("VAr");
        varDataPoint.setDescription("AC Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(2);
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << varDataPoint;

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("AC Power Factor");
        pfDataPoint.setUnits("Pct");
        pfDataPoint.setSize(2);
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << pfDataPoint;

        SunSpecDataPoint watthoursDataPoint;
        watthoursDataPoint.setName("WH");
        watthoursDataPoint.setLabel("WattHours");
        watthoursDataPoint.setDescription("AC Energy");
        watthoursDataPoint.setUnits("Wh");
        watthoursDataPoint.setMandatory(true);
        watthoursDataPoint.setSize(2);
        watthoursDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << watthoursDataPoint;

        SunSpecDataPoint dcAmpsDataPoint;
        dcAmpsDataPoint.setName("DCA");
        dcAmpsDataPoint.setLabel("DC Amps");
        dcAmpsDataPoint.setDescription("DC Current");
        dcAmpsDataPoint.setUnits("A");
        dcAmpsDataPoint.setSize(2);
        dcAmpsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << dcAmpsDataPoint;

        SunSpecDataPoint dcVoltageDataPoint;
        dcVoltageDataPoint.setName("DCV");
        dcVoltageDataPoint.setLabel("DC Voltage");
        dcVoltageDataPoint.setDescription("DC Voltage");
        dcVoltageDataPoint.setUnits("V");
        dcVoltageDataPoint.setSize(2);
        dcVoltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << dcVoltageDataPoint;

        SunSpecDataPoint dcWattsDataPoint;
        dcWattsDataPoint.setName("DCW");
        dcWattsDataPoint.setLabel("DC Watts");
        dcWattsDataPoint.setDescription("DC Power");
        dcWattsDataPoint.setUnits("W");
        dcWattsDataPoint.setSize(2);
        dcWattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << dcWattsDataPoint;

        SunSpecDataPoint cabinetTemperatureDataPoint;
        cabinetTemperatureDataPoint.setName("TmpCab");
        cabinetTemperatureDataPoint.setLabel("Cabinet Temperature");
        cabinetTemperatureDataPoint.setDescription("Cabinet Temperature");
        cabinetTemperatureDataPoint.setUnits("C");
        cabinetTemperatureDataPoint.setMandatory(true);
        cabinetTemperatureDataPoint.setSize(2);
        cabinetTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << cabinetTemperatureDataPoint;

        SunSpecDataPoint heatSinkTemperatureDataPoint;
        heatSinkTemperatureDataPoint.setName("TmpSnk");
        heatSinkTemperatureDataPoint.setLabel("Heat Sink Temperature");
        heatSinkTemperatureDataPoint.setDescription("Heat Sink Temperature");
        heatSinkTemperatureDataPoint.setUnits("C");
        heatSinkTemperatureDataPoint.setSize(2);
        heatSinkTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << heatSinkTemperatureDataPoint;

        SunSpecDataPoint transformerTemperatureDataPoint;
        transformerTemperatureDataPoint.setName("TmpTrns");
        transformerTemperatureDataPoint.setLabel("Transformer Temperature");
        transformerTemperatureDataPoint.setDescription("Transformer Temperature");
        transformerTemperatureDataPoint.setUnits("C");
        transformerTemperatureDataPoint.setSize(2);
        transformerTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << transformerTemperatureDataPoint;

        SunSpecDataPoint otherTemperatureDataPoint;
        otherTemperatureDataPoint.setName("TmpOt");
        otherTemperatureDataPoint.setLabel("Other Temperature");
        otherTemperatureDataPoint.setDescription("Other Temperature");
        otherTemperatureDataPoint.setUnits("C");
        otherTemperatureDataPoint.setSize(2);
        otherTemperatureDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << otherTemperatureDataPoint;

        SunSpecDataPoint operatingStateDataPoint;
        operatingStateDataPoint.setName("St");
        operatingStateDataPoint.setLabel("Operating State");
        operatingStateDataPoint.setDescription("Enumerated value.  Operating state");
        operatingStateDataPoint.setMandatory(true);
        operatingStateDataPoint.setSize(1);
        operatingStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints << operatingStateDataPoint;

        SunSpecDataPoint vendorOperatingStateDataPoint;
        vendorOperatingStateDataPoint.setName("StVnd");
        vendorOperatingStateDataPoint.setLabel("Vendor Operating State");
        vendorOperatingStateDataPoint.setDescription("Vendor specific operating state code");
        vendorOperatingStateDataPoint.setSize(1);
        vendorOperatingStateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints << vendorOperatingStateDataPoint;

        SunSpecDataPoint event1DataPoint;
        event1DataPoint.setName("Evt1");
        event1DataPoint.setLabel("Event1");
        event1DataPoint.setDescription("Bitmask value. Event fields");
        event1DataPoint.setMandatory(true);
        event1DataPoint.setSize(2);
        event1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << event1DataPoint;

        SunSpecDataPoint eventBitfield2DataPoint;
        eventBitfield2DataPoint.setName("Evt2");
        eventBitfield2DataPoint.setLabel("Event Bitfield 2");
        eventBitfield2DataPoint.setDescription("Reserved for future use");
        eventBitfield2DataPoint.setMandatory(true);
        eventBitfield2DataPoint.setSize(2);
        eventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << eventBitfield2DataPoint;

        SunSpecDataPoint vendorEventBitfield1DataPoint;
        vendorEventBitfield1DataPoint.setName("EvtVnd1");
        vendorEventBitfield1DataPoint.setLabel("Vendor Event Bitfield 1");
        vendorEventBitfield1DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield1DataPoint.setSize(2);
        vendorEventBitfield1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield1DataPoint;

        SunSpecDataPoint vendorEventBitfield2DataPoint;
        vendorEventBitfield2DataPoint.setName("EvtVnd2");
        vendorEventBitfield2DataPoint.setLabel("Vendor Event Bitfield 2");
        vendorEventBitfield2DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield2DataPoint.setSize(2);
        vendorEventBitfield2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield2DataPoint;

        SunSpecDataPoint vendorEventBitfield3DataPoint;
        vendorEventBitfield3DataPoint.setName("EvtVnd3");
        vendorEventBitfield3DataPoint.setLabel("Vendor Event Bitfield 3");
        vendorEventBitfield3DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield3DataPoint.setSize(2);
        vendorEventBitfield3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield3DataPoint;

        SunSpecDataPoint vendorEventBitfield4DataPoint;
        vendorEventBitfield4DataPoint.setName("EvtVnd4");
        vendorEventBitfield4DataPoint.setLabel("Vendor Event Bitfield 4");
        vendorEventBitfield4DataPoint.setDescription("Vendor defined events");
        vendorEventBitfield4DataPoint.setSize(2);
        vendorEventBitfield4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << vendorEventBitfield4DataPoint;

        break;
    }
    default:
        break;
    }
}

