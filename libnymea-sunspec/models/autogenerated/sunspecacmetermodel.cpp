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

#include "sunspecacmetermodel.h"

SunSpecAcMeterModel::SunSpecAcMeterModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
    m_supportedModelIds << 201 << 202 << 203 << 204 << 211 << 212 << 213 << 214 << 220;
}

SunSpecAcMeterModel::~SunSpecAcMeterModel()
{

}

QString SunSpecAcMeterModel::name() const
{
    return "ac_meter";
}

QString SunSpecAcMeterModel::description() const
{
    switch (m_modelId) {
    case 201:
        return "Include this model for single phase (AN or AB) metering";
    case 202:
        return QString();
    case 203:
        return QString();
    case 204:
        return QString();
    case 211:
        return QString();
    case 212:
        return QString();
    case 213:
        return QString();
    case 214:
        return QString();
    case 220:
        return "Include this model for secure metering";
    default:
        return QString();
    }
}

QString SunSpecAcMeterModel::label() const
{
    switch (m_modelId) {
    case 201:
        return "Meter (Single Phase)single phase (AN or AB) meter";
    case 202:
        return "split single phase (ABN) meter";
    case 203:
        return "wye-connect three phase (abcn) meter";
    case 204:
        return "delta-connect three phase (abc) meter";
    case 211:
        return "single phase (AN or AB) meter";
    case 212:
        return "split single phase (ABN) meter";
    case 213:
        return "wye-connect three phase (abcn) meter";
    case 214:
        return "delta-connect three phase (abc) meter";
    case 220:
        return "Secure AC Meter Selected Readings";
    default:
        return QString();
    }
}

void SunSpecAcMeterModel::readModelHeader()
{

}

void SunSpecAcMeterModel::readBlockData()
{

}

void SunSpecAcMeterModel::initDataPoints()
{
    switch (m_modelId) {
    case 201: {
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
        ampsDataPoint.setDescription("Total AC Current");
        ampsDataPoint.setUnits("A");
        ampsDataPoint.setMandatory(true);
        ampsDataPoint.setSize(1);
        ampsDataPoint.setAddressOffset(2);
        ampsDataPoint.setBlockOffset(0);
        ampsDataPoint.setScaleFactorName("A_SF");
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
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
        ampsPhaseaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
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
        ampsPhasebDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
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
        ampsPhasecDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(ampsPhasecDataPoint.name(), ampsPhasecDataPoint);

        SunSpecDataPoint A_SFDataPoint;
        A_SFDataPoint.setName("A_SF");
        A_SFDataPoint.setDescription("Current scale factor");
        A_SFDataPoint.setMandatory(true);
        A_SFDataPoint.setSize(1);
        A_SFDataPoint.setAddressOffset(6);
        A_SFDataPoint.setBlockOffset(4);
        A_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(A_SFDataPoint.name(), A_SFDataPoint);

        SunSpecDataPoint voltageLnDataPoint;
        voltageLnDataPoint.setName("PhV");
        voltageLnDataPoint.setLabel("Voltage LN");
        voltageLnDataPoint.setDescription("Line to Neutral AC Voltage (average of active phases)");
        voltageLnDataPoint.setUnits("V");
        voltageLnDataPoint.setSize(1);
        voltageLnDataPoint.setAddressOffset(7);
        voltageLnDataPoint.setBlockOffset(5);
        voltageLnDataPoint.setScaleFactorName("V_SF");
        voltageLnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(voltageLnDataPoint.name(), voltageLnDataPoint);

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
        phaseVoltageAnDataPoint.setSize(1);
        phaseVoltageAnDataPoint.setAddressOffset(8);
        phaseVoltageAnDataPoint.setBlockOffset(6);
        phaseVoltageAnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageAnDataPoint.name(), phaseVoltageAnDataPoint);

        SunSpecDataPoint phaseVoltageBnDataPoint;
        phaseVoltageBnDataPoint.setName("PhVphB");
        phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
        phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
        phaseVoltageBnDataPoint.setUnits("V");
        phaseVoltageBnDataPoint.setSize(1);
        phaseVoltageBnDataPoint.setAddressOffset(9);
        phaseVoltageBnDataPoint.setBlockOffset(7);
        phaseVoltageBnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageBnDataPoint.name(), phaseVoltageBnDataPoint);

        SunSpecDataPoint phaseVoltageCnDataPoint;
        phaseVoltageCnDataPoint.setName("PhVphC");
        phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
        phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
        phaseVoltageCnDataPoint.setUnits("V");
        phaseVoltageCnDataPoint.setSize(1);
        phaseVoltageCnDataPoint.setAddressOffset(10);
        phaseVoltageCnDataPoint.setBlockOffset(8);
        phaseVoltageCnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageCnDataPoint.name(), phaseVoltageCnDataPoint);

        SunSpecDataPoint voltageLlDataPoint;
        voltageLlDataPoint.setName("PPV");
        voltageLlDataPoint.setLabel("Voltage LL");
        voltageLlDataPoint.setDescription("Line to Line AC Voltage (average of active phases)");
        voltageLlDataPoint.setUnits("V");
        voltageLlDataPoint.setSize(1);
        voltageLlDataPoint.setAddressOffset(11);
        voltageLlDataPoint.setBlockOffset(9);
        voltageLlDataPoint.setScaleFactorName("V_SF");
        voltageLlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(voltageLlDataPoint.name(), voltageLlDataPoint);

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PPVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setSize(1);
        phaseVoltageAbDataPoint.setAddressOffset(12);
        phaseVoltageAbDataPoint.setBlockOffset(10);
        phaseVoltageAbDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageAbDataPoint.name(), phaseVoltageAbDataPoint);

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PPVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setSize(1);
        phaseVoltageBcDataPoint.setAddressOffset(13);
        phaseVoltageBcDataPoint.setBlockOffset(11);
        phaseVoltageBcDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageBcDataPoint.name(), phaseVoltageBcDataPoint);

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PPVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setSize(1);
        phaseVoltageCaDataPoint.setAddressOffset(14);
        phaseVoltageCaDataPoint.setBlockOffset(12);
        phaseVoltageCaDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageCaDataPoint.name(), phaseVoltageCaDataPoint);

        SunSpecDataPoint V_SFDataPoint;
        V_SFDataPoint.setName("V_SF");
        V_SFDataPoint.setDescription("Voltage scale factor");
        V_SFDataPoint.setMandatory(true);
        V_SFDataPoint.setSize(1);
        V_SFDataPoint.setAddressOffset(15);
        V_SFDataPoint.setBlockOffset(13);
        V_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(V_SFDataPoint.name(), V_SFDataPoint);

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(1);
        hzDataPoint.setAddressOffset(16);
        hzDataPoint.setBlockOffset(14);
        hzDataPoint.setScaleFactorName("Hz_SF");
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(hzDataPoint.name(), hzDataPoint);

        SunSpecDataPoint Hz_SFDataPoint;
        Hz_SFDataPoint.setName("Hz_SF");
        Hz_SFDataPoint.setDescription("Frequency scale factor");
        Hz_SFDataPoint.setSize(1);
        Hz_SFDataPoint.setAddressOffset(17);
        Hz_SFDataPoint.setBlockOffset(15);
        Hz_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(Hz_SFDataPoint.name(), Hz_SFDataPoint);

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("Total Real Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(1);
        wattsDataPoint.setAddressOffset(18);
        wattsDataPoint.setBlockOffset(16);
        wattsDataPoint.setScaleFactorName("W_SF");
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsDataPoint.name(), wattsDataPoint);

        SunSpecDataPoint wattsPhaseADataPoint;
        wattsPhaseADataPoint.setName("WphA");
        wattsPhaseADataPoint.setLabel("Watts phase A");
        wattsPhaseADataPoint.setUnits("W");
        wattsPhaseADataPoint.setSize(1);
        wattsPhaseADataPoint.setAddressOffset(19);
        wattsPhaseADataPoint.setBlockOffset(17);
        wattsPhaseADataPoint.setScaleFactorName("W_SF");
        wattsPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsPhaseADataPoint.name(), wattsPhaseADataPoint);

        SunSpecDataPoint wattsPhaseBDataPoint;
        wattsPhaseBDataPoint.setName("WphB");
        wattsPhaseBDataPoint.setLabel("Watts phase B");
        wattsPhaseBDataPoint.setUnits("W");
        wattsPhaseBDataPoint.setSize(1);
        wattsPhaseBDataPoint.setAddressOffset(20);
        wattsPhaseBDataPoint.setBlockOffset(18);
        wattsPhaseBDataPoint.setScaleFactorName("W_SF");
        wattsPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsPhaseBDataPoint.name(), wattsPhaseBDataPoint);

        SunSpecDataPoint wattsPhaseCDataPoint;
        wattsPhaseCDataPoint.setName("WphC");
        wattsPhaseCDataPoint.setLabel("Watts phase C");
        wattsPhaseCDataPoint.setUnits("W");
        wattsPhaseCDataPoint.setSize(1);
        wattsPhaseCDataPoint.setAddressOffset(21);
        wattsPhaseCDataPoint.setBlockOffset(19);
        wattsPhaseCDataPoint.setScaleFactorName("W_SF");
        wattsPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsPhaseCDataPoint.name(), wattsPhaseCDataPoint);

        SunSpecDataPoint W_SFDataPoint;
        W_SFDataPoint.setName("W_SF");
        W_SFDataPoint.setDescription("Real Power scale factor");
        W_SFDataPoint.setMandatory(true);
        W_SFDataPoint.setSize(1);
        W_SFDataPoint.setAddressOffset(22);
        W_SFDataPoint.setBlockOffset(20);
        W_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(W_SFDataPoint.name(), W_SFDataPoint);

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(1);
        vaDataPoint.setAddressOffset(23);
        vaDataPoint.setBlockOffset(21);
        vaDataPoint.setScaleFactorName("VA_SF");
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(vaDataPoint.name(), vaDataPoint);

        SunSpecDataPoint vaPhaseADataPoint;
        vaPhaseADataPoint.setName("VAphA");
        vaPhaseADataPoint.setLabel("VA phase A");
        vaPhaseADataPoint.setUnits("VA");
        vaPhaseADataPoint.setSize(1);
        vaPhaseADataPoint.setAddressOffset(24);
        vaPhaseADataPoint.setBlockOffset(22);
        vaPhaseADataPoint.setScaleFactorName("VA_SF");
        vaPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(vaPhaseADataPoint.name(), vaPhaseADataPoint);

        SunSpecDataPoint vaPhaseBDataPoint;
        vaPhaseBDataPoint.setName("VAphB");
        vaPhaseBDataPoint.setLabel("VA phase B");
        vaPhaseBDataPoint.setUnits("VA");
        vaPhaseBDataPoint.setSize(1);
        vaPhaseBDataPoint.setAddressOffset(25);
        vaPhaseBDataPoint.setBlockOffset(23);
        vaPhaseBDataPoint.setScaleFactorName("VA_SF");
        vaPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(vaPhaseBDataPoint.name(), vaPhaseBDataPoint);

        SunSpecDataPoint vaPhaseCDataPoint;
        vaPhaseCDataPoint.setName("VAphC");
        vaPhaseCDataPoint.setLabel("VA phase C");
        vaPhaseCDataPoint.setUnits("VA");
        vaPhaseCDataPoint.setSize(1);
        vaPhaseCDataPoint.setAddressOffset(26);
        vaPhaseCDataPoint.setBlockOffset(24);
        vaPhaseCDataPoint.setScaleFactorName("VA_SF");
        vaPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(vaPhaseCDataPoint.name(), vaPhaseCDataPoint);

        SunSpecDataPoint VA_SFDataPoint;
        VA_SFDataPoint.setName("VA_SF");
        VA_SFDataPoint.setDescription("Apparent Power scale factor");
        VA_SFDataPoint.setSize(1);
        VA_SFDataPoint.setAddressOffset(27);
        VA_SFDataPoint.setBlockOffset(25);
        VA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(VA_SFDataPoint.name(), VA_SFDataPoint);

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAR");
        varDataPoint.setLabel("VAR");
        varDataPoint.setDescription("Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(1);
        varDataPoint.setAddressOffset(28);
        varDataPoint.setBlockOffset(26);
        varDataPoint.setScaleFactorName("VAR_SF");
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(varDataPoint.name(), varDataPoint);

        SunSpecDataPoint varPhaseADataPoint;
        varPhaseADataPoint.setName("VARphA");
        varPhaseADataPoint.setLabel("VAR phase A");
        varPhaseADataPoint.setUnits("var");
        varPhaseADataPoint.setSize(1);
        varPhaseADataPoint.setAddressOffset(29);
        varPhaseADataPoint.setBlockOffset(27);
        varPhaseADataPoint.setScaleFactorName("VAR_SF");
        varPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(varPhaseADataPoint.name(), varPhaseADataPoint);

        SunSpecDataPoint varPhaseBDataPoint;
        varPhaseBDataPoint.setName("VARphB");
        varPhaseBDataPoint.setLabel("VAR phase B");
        varPhaseBDataPoint.setUnits("var");
        varPhaseBDataPoint.setSize(1);
        varPhaseBDataPoint.setAddressOffset(30);
        varPhaseBDataPoint.setBlockOffset(28);
        varPhaseBDataPoint.setScaleFactorName("VAR_SF");
        varPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(varPhaseBDataPoint.name(), varPhaseBDataPoint);

        SunSpecDataPoint varPhaseCDataPoint;
        varPhaseCDataPoint.setName("VARphC");
        varPhaseCDataPoint.setLabel("VAR phase C");
        varPhaseCDataPoint.setUnits("var");
        varPhaseCDataPoint.setSize(1);
        varPhaseCDataPoint.setAddressOffset(31);
        varPhaseCDataPoint.setBlockOffset(29);
        varPhaseCDataPoint.setScaleFactorName("VAR_SF");
        varPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(varPhaseCDataPoint.name(), varPhaseCDataPoint);

        SunSpecDataPoint VAR_SFDataPoint;
        VAR_SFDataPoint.setName("VAR_SF");
        VAR_SFDataPoint.setDescription("Reactive Power scale factor");
        VAR_SFDataPoint.setSize(1);
        VAR_SFDataPoint.setAddressOffset(32);
        VAR_SFDataPoint.setBlockOffset(30);
        VAR_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(VAR_SFDataPoint.name(), VAR_SFDataPoint);

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("Power Factor");
        pfDataPoint.setUnits("Pct");
        pfDataPoint.setSize(1);
        pfDataPoint.setAddressOffset(33);
        pfDataPoint.setBlockOffset(31);
        pfDataPoint.setScaleFactorName("PF_SF");
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(pfDataPoint.name(), pfDataPoint);

        SunSpecDataPoint pfPhaseADataPoint;
        pfPhaseADataPoint.setName("PFphA");
        pfPhaseADataPoint.setLabel("PF phase A");
        pfPhaseADataPoint.setUnits("Pct");
        pfPhaseADataPoint.setSize(1);
        pfPhaseADataPoint.setAddressOffset(34);
        pfPhaseADataPoint.setBlockOffset(32);
        pfPhaseADataPoint.setScaleFactorName("PF_SF");
        pfPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(pfPhaseADataPoint.name(), pfPhaseADataPoint);

        SunSpecDataPoint pfPhaseBDataPoint;
        pfPhaseBDataPoint.setName("PFphB");
        pfPhaseBDataPoint.setLabel("PF phase B");
        pfPhaseBDataPoint.setUnits("Pct");
        pfPhaseBDataPoint.setSize(1);
        pfPhaseBDataPoint.setAddressOffset(35);
        pfPhaseBDataPoint.setBlockOffset(33);
        pfPhaseBDataPoint.setScaleFactorName("PF_SF");
        pfPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(pfPhaseBDataPoint.name(), pfPhaseBDataPoint);

        SunSpecDataPoint pfPhaseCDataPoint;
        pfPhaseCDataPoint.setName("PFphC");
        pfPhaseCDataPoint.setLabel("PF phase C");
        pfPhaseCDataPoint.setUnits("Pct");
        pfPhaseCDataPoint.setSize(1);
        pfPhaseCDataPoint.setAddressOffset(36);
        pfPhaseCDataPoint.setBlockOffset(34);
        pfPhaseCDataPoint.setScaleFactorName("PF_SF");
        pfPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(pfPhaseCDataPoint.name(), pfPhaseCDataPoint);

        SunSpecDataPoint PF_SFDataPoint;
        PF_SFDataPoint.setName("PF_SF");
        PF_SFDataPoint.setDescription("Power Factor scale factor");
        PF_SFDataPoint.setSize(1);
        PF_SFDataPoint.setAddressOffset(37);
        PF_SFDataPoint.setBlockOffset(35);
        PF_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(PF_SFDataPoint.name(), PF_SFDataPoint);

        SunSpecDataPoint totalWattHoursExportedDataPoint;
        totalWattHoursExportedDataPoint.setName("TotWhExp");
        totalWattHoursExportedDataPoint.setLabel("Total Watt-hours Exported");
        totalWattHoursExportedDataPoint.setDescription("Total Real Energy Exported");
        totalWattHoursExportedDataPoint.setUnits("Wh");
        totalWattHoursExportedDataPoint.setMandatory(true);
        totalWattHoursExportedDataPoint.setSize(2);
        totalWattHoursExportedDataPoint.setAddressOffset(38);
        totalWattHoursExportedDataPoint.setBlockOffset(36);
        totalWattHoursExportedDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursExportedDataPoint.name(), totalWattHoursExportedDataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseADataPoint;
        totalWattHoursExportedPhaseADataPoint.setName("TotWhExpPhA");
        totalWattHoursExportedPhaseADataPoint.setLabel("Total Watt-hours Exported phase A");
        totalWattHoursExportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseADataPoint.setSize(2);
        totalWattHoursExportedPhaseADataPoint.setAddressOffset(40);
        totalWattHoursExportedPhaseADataPoint.setBlockOffset(38);
        totalWattHoursExportedPhaseADataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursExportedPhaseADataPoint.name(), totalWattHoursExportedPhaseADataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseBDataPoint;
        totalWattHoursExportedPhaseBDataPoint.setName("TotWhExpPhB");
        totalWattHoursExportedPhaseBDataPoint.setLabel("Total Watt-hours Exported phase B");
        totalWattHoursExportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseBDataPoint.setSize(2);
        totalWattHoursExportedPhaseBDataPoint.setAddressOffset(42);
        totalWattHoursExportedPhaseBDataPoint.setBlockOffset(40);
        totalWattHoursExportedPhaseBDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursExportedPhaseBDataPoint.name(), totalWattHoursExportedPhaseBDataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseCDataPoint;
        totalWattHoursExportedPhaseCDataPoint.setName("TotWhExpPhC");
        totalWattHoursExportedPhaseCDataPoint.setLabel("Total Watt-hours Exported phase C");
        totalWattHoursExportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseCDataPoint.setSize(2);
        totalWattHoursExportedPhaseCDataPoint.setAddressOffset(44);
        totalWattHoursExportedPhaseCDataPoint.setBlockOffset(42);
        totalWattHoursExportedPhaseCDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursExportedPhaseCDataPoint.name(), totalWattHoursExportedPhaseCDataPoint);

        SunSpecDataPoint totalWattHoursImportedDataPoint;
        totalWattHoursImportedDataPoint.setName("TotWhImp");
        totalWattHoursImportedDataPoint.setLabel("Total Watt-hours Imported");
        totalWattHoursImportedDataPoint.setDescription("Total Real Energy Imported");
        totalWattHoursImportedDataPoint.setUnits("Wh");
        totalWattHoursImportedDataPoint.setMandatory(true);
        totalWattHoursImportedDataPoint.setSize(2);
        totalWattHoursImportedDataPoint.setAddressOffset(46);
        totalWattHoursImportedDataPoint.setBlockOffset(44);
        totalWattHoursImportedDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursImportedDataPoint.name(), totalWattHoursImportedDataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseADataPoint;
        totalWattHoursImportedPhaseADataPoint.setName("TotWhImpPhA");
        totalWattHoursImportedPhaseADataPoint.setLabel("Total Watt-hours Imported phase A");
        totalWattHoursImportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseADataPoint.setSize(2);
        totalWattHoursImportedPhaseADataPoint.setAddressOffset(48);
        totalWattHoursImportedPhaseADataPoint.setBlockOffset(46);
        totalWattHoursImportedPhaseADataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseADataPoint.name(), totalWattHoursImportedPhaseADataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseBDataPoint;
        totalWattHoursImportedPhaseBDataPoint.setName("TotWhImpPhB");
        totalWattHoursImportedPhaseBDataPoint.setLabel("Total Watt-hours Imported phase B");
        totalWattHoursImportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseBDataPoint.setSize(2);
        totalWattHoursImportedPhaseBDataPoint.setAddressOffset(50);
        totalWattHoursImportedPhaseBDataPoint.setBlockOffset(48);
        totalWattHoursImportedPhaseBDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseBDataPoint.name(), totalWattHoursImportedPhaseBDataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseCDataPoint;
        totalWattHoursImportedPhaseCDataPoint.setName("TotWhImpPhC");
        totalWattHoursImportedPhaseCDataPoint.setLabel("Total Watt-hours Imported phase C");
        totalWattHoursImportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseCDataPoint.setSize(2);
        totalWattHoursImportedPhaseCDataPoint.setAddressOffset(52);
        totalWattHoursImportedPhaseCDataPoint.setBlockOffset(50);
        totalWattHoursImportedPhaseCDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseCDataPoint.name(), totalWattHoursImportedPhaseCDataPoint);

        SunSpecDataPoint TotWh_SFDataPoint;
        TotWh_SFDataPoint.setName("TotWh_SF");
        TotWh_SFDataPoint.setDescription("Real Energy scale factor");
        TotWh_SFDataPoint.setMandatory(true);
        TotWh_SFDataPoint.setSize(1);
        TotWh_SFDataPoint.setAddressOffset(54);
        TotWh_SFDataPoint.setBlockOffset(52);
        TotWh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(TotWh_SFDataPoint.name(), TotWh_SFDataPoint);

        SunSpecDataPoint totalVaHoursExportedDataPoint;
        totalVaHoursExportedDataPoint.setName("TotVAhExp");
        totalVaHoursExportedDataPoint.setLabel("Total VA-hours Exported");
        totalVaHoursExportedDataPoint.setDescription("Total Apparent Energy Exported");
        totalVaHoursExportedDataPoint.setUnits("VAh");
        totalVaHoursExportedDataPoint.setSize(2);
        totalVaHoursExportedDataPoint.setAddressOffset(55);
        totalVaHoursExportedDataPoint.setBlockOffset(53);
        totalVaHoursExportedDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursExportedDataPoint.name(), totalVaHoursExportedDataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseADataPoint;
        totalVaHoursExportedPhaseADataPoint.setName("TotVAhExpPhA");
        totalVaHoursExportedPhaseADataPoint.setLabel("Total VA-hours Exported phase A");
        totalVaHoursExportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseADataPoint.setSize(2);
        totalVaHoursExportedPhaseADataPoint.setAddressOffset(57);
        totalVaHoursExportedPhaseADataPoint.setBlockOffset(55);
        totalVaHoursExportedPhaseADataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseADataPoint.name(), totalVaHoursExportedPhaseADataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseBDataPoint;
        totalVaHoursExportedPhaseBDataPoint.setName("TotVAhExpPhB");
        totalVaHoursExportedPhaseBDataPoint.setLabel("Total VA-hours Exported phase B");
        totalVaHoursExportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseBDataPoint.setSize(2);
        totalVaHoursExportedPhaseBDataPoint.setAddressOffset(59);
        totalVaHoursExportedPhaseBDataPoint.setBlockOffset(57);
        totalVaHoursExportedPhaseBDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseBDataPoint.name(), totalVaHoursExportedPhaseBDataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseCDataPoint;
        totalVaHoursExportedPhaseCDataPoint.setName("TotVAhExpPhC");
        totalVaHoursExportedPhaseCDataPoint.setLabel("Total VA-hours Exported phase C");
        totalVaHoursExportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseCDataPoint.setSize(2);
        totalVaHoursExportedPhaseCDataPoint.setAddressOffset(61);
        totalVaHoursExportedPhaseCDataPoint.setBlockOffset(59);
        totalVaHoursExportedPhaseCDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseCDataPoint.name(), totalVaHoursExportedPhaseCDataPoint);

        SunSpecDataPoint totalVaHoursImportedDataPoint;
        totalVaHoursImportedDataPoint.setName("TotVAhImp");
        totalVaHoursImportedDataPoint.setLabel("Total VA-hours Imported");
        totalVaHoursImportedDataPoint.setDescription("Total Apparent Energy Imported");
        totalVaHoursImportedDataPoint.setUnits("VAh");
        totalVaHoursImportedDataPoint.setSize(2);
        totalVaHoursImportedDataPoint.setAddressOffset(63);
        totalVaHoursImportedDataPoint.setBlockOffset(61);
        totalVaHoursImportedDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursImportedDataPoint.name(), totalVaHoursImportedDataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseADataPoint;
        totalVaHoursImportedPhaseADataPoint.setName("TotVAhImpPhA");
        totalVaHoursImportedPhaseADataPoint.setLabel("Total VA-hours Imported phase A");
        totalVaHoursImportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseADataPoint.setSize(2);
        totalVaHoursImportedPhaseADataPoint.setAddressOffset(65);
        totalVaHoursImportedPhaseADataPoint.setBlockOffset(63);
        totalVaHoursImportedPhaseADataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseADataPoint.name(), totalVaHoursImportedPhaseADataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseBDataPoint;
        totalVaHoursImportedPhaseBDataPoint.setName("TotVAhImpPhB");
        totalVaHoursImportedPhaseBDataPoint.setLabel("Total VA-hours Imported phase B");
        totalVaHoursImportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseBDataPoint.setSize(2);
        totalVaHoursImportedPhaseBDataPoint.setAddressOffset(67);
        totalVaHoursImportedPhaseBDataPoint.setBlockOffset(65);
        totalVaHoursImportedPhaseBDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseBDataPoint.name(), totalVaHoursImportedPhaseBDataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseCDataPoint;
        totalVaHoursImportedPhaseCDataPoint.setName("TotVAhImpPhC");
        totalVaHoursImportedPhaseCDataPoint.setLabel("Total VA-hours Imported phase C");
        totalVaHoursImportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseCDataPoint.setSize(2);
        totalVaHoursImportedPhaseCDataPoint.setAddressOffset(69);
        totalVaHoursImportedPhaseCDataPoint.setBlockOffset(67);
        totalVaHoursImportedPhaseCDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseCDataPoint.name(), totalVaHoursImportedPhaseCDataPoint);

        SunSpecDataPoint TotVAh_SFDataPoint;
        TotVAh_SFDataPoint.setName("TotVAh_SF");
        TotVAh_SFDataPoint.setDescription("Apparent Energy scale factor");
        TotVAh_SFDataPoint.setSize(1);
        TotVAh_SFDataPoint.setAddressOffset(71);
        TotVAh_SFDataPoint.setBlockOffset(69);
        TotVAh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(TotVAh_SFDataPoint.name(), TotVAh_SFDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1DataPoint;
        totalVarHoursImportedQ1DataPoint.setName("TotVArhImpQ1");
        totalVarHoursImportedQ1DataPoint.setLabel("Total VAR-hours Imported Q1");
        totalVarHoursImportedQ1DataPoint.setDescription("Total Reactive Energy Imported Quadrant 1");
        totalVarHoursImportedQ1DataPoint.setUnits("varh");
        totalVarHoursImportedQ1DataPoint.setSize(2);
        totalVarHoursImportedQ1DataPoint.setAddressOffset(72);
        totalVarHoursImportedQ1DataPoint.setBlockOffset(70);
        totalVarHoursImportedQ1DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ1DataPoint.name(), totalVarHoursImportedQ1DataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseADataPoint;
        totalVarHoursImportedQ1PhaseADataPoint.setName("TotVArhImpQ1PhA");
        totalVarHoursImportedQ1PhaseADataPoint.setLabel("Total VAr-hours Imported Q1 phase A");
        totalVarHoursImportedQ1PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseADataPoint.setAddressOffset(74);
        totalVarHoursImportedQ1PhaseADataPoint.setBlockOffset(72);
        totalVarHoursImportedQ1PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseADataPoint.name(), totalVarHoursImportedQ1PhaseADataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseBDataPoint;
        totalVarHoursImportedQ1PhaseBDataPoint.setName("TotVArhImpQ1PhB");
        totalVarHoursImportedQ1PhaseBDataPoint.setLabel("Total VAr-hours Imported Q1 phase B");
        totalVarHoursImportedQ1PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseBDataPoint.setAddressOffset(76);
        totalVarHoursImportedQ1PhaseBDataPoint.setBlockOffset(74);
        totalVarHoursImportedQ1PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseBDataPoint.name(), totalVarHoursImportedQ1PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseCDataPoint;
        totalVarHoursImportedQ1PhaseCDataPoint.setName("TotVArhImpQ1PhC");
        totalVarHoursImportedQ1PhaseCDataPoint.setLabel("Total VAr-hours Imported Q1 phase C");
        totalVarHoursImportedQ1PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseCDataPoint.setAddressOffset(78);
        totalVarHoursImportedQ1PhaseCDataPoint.setBlockOffset(76);
        totalVarHoursImportedQ1PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseCDataPoint.name(), totalVarHoursImportedQ1PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2DataPoint;
        totalVarHoursImportedQ2DataPoint.setName("TotVArhImpQ2");
        totalVarHoursImportedQ2DataPoint.setLabel("Total VAr-hours Imported Q2");
        totalVarHoursImportedQ2DataPoint.setDescription("Total Reactive Power Imported Quadrant 2");
        totalVarHoursImportedQ2DataPoint.setUnits("varh");
        totalVarHoursImportedQ2DataPoint.setSize(2);
        totalVarHoursImportedQ2DataPoint.setAddressOffset(80);
        totalVarHoursImportedQ2DataPoint.setBlockOffset(78);
        totalVarHoursImportedQ2DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ2DataPoint.name(), totalVarHoursImportedQ2DataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseADataPoint;
        totalVarHoursImportedQ2PhaseADataPoint.setName("TotVArhImpQ2PhA");
        totalVarHoursImportedQ2PhaseADataPoint.setLabel("Total VAr-hours Imported Q2 phase A");
        totalVarHoursImportedQ2PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseADataPoint.setAddressOffset(82);
        totalVarHoursImportedQ2PhaseADataPoint.setBlockOffset(80);
        totalVarHoursImportedQ2PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseADataPoint.name(), totalVarHoursImportedQ2PhaseADataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseBDataPoint;
        totalVarHoursImportedQ2PhaseBDataPoint.setName("TotVArhImpQ2PhB");
        totalVarHoursImportedQ2PhaseBDataPoint.setLabel("Total VAr-hours Imported Q2 phase B");
        totalVarHoursImportedQ2PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseBDataPoint.setAddressOffset(84);
        totalVarHoursImportedQ2PhaseBDataPoint.setBlockOffset(82);
        totalVarHoursImportedQ2PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseBDataPoint.name(), totalVarHoursImportedQ2PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseCDataPoint;
        totalVarHoursImportedQ2PhaseCDataPoint.setName("TotVArhImpQ2PhC");
        totalVarHoursImportedQ2PhaseCDataPoint.setLabel("Total VAr-hours Imported Q2 phase C");
        totalVarHoursImportedQ2PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseCDataPoint.setAddressOffset(86);
        totalVarHoursImportedQ2PhaseCDataPoint.setBlockOffset(84);
        totalVarHoursImportedQ2PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseCDataPoint.name(), totalVarHoursImportedQ2PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3DataPoint;
        totalVarHoursExportedQ3DataPoint.setName("TotVArhExpQ3");
        totalVarHoursExportedQ3DataPoint.setLabel("Total VAr-hours Exported Q3");
        totalVarHoursExportedQ3DataPoint.setDescription("Total Reactive Power Exported Quadrant 3");
        totalVarHoursExportedQ3DataPoint.setUnits("varh");
        totalVarHoursExportedQ3DataPoint.setSize(2);
        totalVarHoursExportedQ3DataPoint.setAddressOffset(88);
        totalVarHoursExportedQ3DataPoint.setBlockOffset(86);
        totalVarHoursExportedQ3DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ3DataPoint.name(), totalVarHoursExportedQ3DataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseADataPoint;
        totalVarHoursExportedQ3PhaseADataPoint.setName("TotVArhExpQ3PhA");
        totalVarHoursExportedQ3PhaseADataPoint.setLabel("Total VAr-hours Exported Q3 phase A");
        totalVarHoursExportedQ3PhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseADataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseADataPoint.setAddressOffset(90);
        totalVarHoursExportedQ3PhaseADataPoint.setBlockOffset(88);
        totalVarHoursExportedQ3PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseADataPoint.name(), totalVarHoursExportedQ3PhaseADataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseBDataPoint;
        totalVarHoursExportedQ3PhaseBDataPoint.setName("TotVArhExpQ3PhB");
        totalVarHoursExportedQ3PhaseBDataPoint.setLabel("Total VAr-hours Exported Q3 phase B");
        totalVarHoursExportedQ3PhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseBDataPoint.setAddressOffset(92);
        totalVarHoursExportedQ3PhaseBDataPoint.setBlockOffset(90);
        totalVarHoursExportedQ3PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseBDataPoint.name(), totalVarHoursExportedQ3PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseCDataPoint;
        totalVarHoursExportedQ3PhaseCDataPoint.setName("TotVArhExpQ3PhC");
        totalVarHoursExportedQ3PhaseCDataPoint.setLabel("Total VAr-hours Exported Q3 phase C");
        totalVarHoursExportedQ3PhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseCDataPoint.setAddressOffset(94);
        totalVarHoursExportedQ3PhaseCDataPoint.setBlockOffset(92);
        totalVarHoursExportedQ3PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseCDataPoint.name(), totalVarHoursExportedQ3PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4DataPoint;
        totalVarHoursExportedQ4DataPoint.setName("TotVArhExpQ4");
        totalVarHoursExportedQ4DataPoint.setLabel("Total VAr-hours Exported Q4");
        totalVarHoursExportedQ4DataPoint.setDescription("Total Reactive Power Exported Quadrant 4");
        totalVarHoursExportedQ4DataPoint.setUnits("varh");
        totalVarHoursExportedQ4DataPoint.setSize(2);
        totalVarHoursExportedQ4DataPoint.setAddressOffset(96);
        totalVarHoursExportedQ4DataPoint.setBlockOffset(94);
        totalVarHoursExportedQ4DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ4DataPoint.name(), totalVarHoursExportedQ4DataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseADataPoint;
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setName("TotVArhExpQ4PhA");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase A");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setAddressOffset(98);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setBlockOffset(96);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseADataPoint.name(), totalVarHoursExportedQ4ImportedPhaseADataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseBDataPoint;
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setName("TotVArhExpQ4PhB");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase B");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setAddressOffset(100);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setBlockOffset(98);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseBDataPoint.name(), totalVarHoursExportedQ4ImportedPhaseBDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseCDataPoint;
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setName("TotVArhExpQ4PhC");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase C");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setAddressOffset(102);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setBlockOffset(100);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseCDataPoint.name(), totalVarHoursExportedQ4ImportedPhaseCDataPoint);

        SunSpecDataPoint TotVArh_SFDataPoint;
        TotVArh_SFDataPoint.setName("TotVArh_SF");
        TotVArh_SFDataPoint.setDescription("Reactive Energy scale factor");
        TotVArh_SFDataPoint.setSize(1);
        TotVArh_SFDataPoint.setAddressOffset(104);
        TotVArh_SFDataPoint.setBlockOffset(102);
        TotVArh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(TotVArh_SFDataPoint.name(), TotVArh_SFDataPoint);

        SunSpecDataPoint eventsDataPoint;
        eventsDataPoint.setName("Evt");
        eventsDataPoint.setLabel("Events");
        eventsDataPoint.setDescription("Meter Event Flags");
        eventsDataPoint.setMandatory(true);
        eventsDataPoint.setSize(2);
        eventsDataPoint.setAddressOffset(105);
        eventsDataPoint.setBlockOffset(103);
        eventsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(eventsDataPoint.name(), eventsDataPoint);

        break;
    }
    case 202: {
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
        ampsDataPoint.setDescription("Total AC Current");
        ampsDataPoint.setUnits("A");
        ampsDataPoint.setMandatory(true);
        ampsDataPoint.setSize(1);
        ampsDataPoint.setAddressOffset(2);
        ampsDataPoint.setBlockOffset(0);
        ampsDataPoint.setScaleFactorName("A_SF");
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(ampsDataPoint.name(), ampsDataPoint);

        SunSpecDataPoint ampsPhaseaDataPoint;
        ampsPhaseaDataPoint.setName("AphA");
        ampsPhaseaDataPoint.setLabel("Amps PhaseA");
        ampsPhaseaDataPoint.setDescription("Phase A Current");
        ampsPhaseaDataPoint.setUnits("A");
        ampsPhaseaDataPoint.setSize(1);
        ampsPhaseaDataPoint.setAddressOffset(3);
        ampsPhaseaDataPoint.setBlockOffset(1);
        ampsPhaseaDataPoint.setScaleFactorName("A_SF");
        ampsPhaseaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
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
        ampsPhasebDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
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
        ampsPhasecDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(ampsPhasecDataPoint.name(), ampsPhasecDataPoint);

        SunSpecDataPoint A_SFDataPoint;
        A_SFDataPoint.setName("A_SF");
        A_SFDataPoint.setDescription("Current scale factor");
        A_SFDataPoint.setMandatory(true);
        A_SFDataPoint.setSize(1);
        A_SFDataPoint.setAddressOffset(6);
        A_SFDataPoint.setBlockOffset(4);
        A_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(A_SFDataPoint.name(), A_SFDataPoint);

        SunSpecDataPoint voltageLnDataPoint;
        voltageLnDataPoint.setName("PhV");
        voltageLnDataPoint.setLabel("Voltage LN");
        voltageLnDataPoint.setDescription("Line to Neutral AC Voltage (average of active phases)");
        voltageLnDataPoint.setUnits("V");
        voltageLnDataPoint.setMandatory(true);
        voltageLnDataPoint.setSize(1);
        voltageLnDataPoint.setAddressOffset(7);
        voltageLnDataPoint.setBlockOffset(5);
        voltageLnDataPoint.setScaleFactorName("V_SF");
        voltageLnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(voltageLnDataPoint.name(), voltageLnDataPoint);

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
        phaseVoltageAnDataPoint.setMandatory(true);
        phaseVoltageAnDataPoint.setSize(1);
        phaseVoltageAnDataPoint.setAddressOffset(8);
        phaseVoltageAnDataPoint.setBlockOffset(6);
        phaseVoltageAnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageAnDataPoint.name(), phaseVoltageAnDataPoint);

        SunSpecDataPoint phaseVoltageBnDataPoint;
        phaseVoltageBnDataPoint.setName("PhVphB");
        phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
        phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
        phaseVoltageBnDataPoint.setUnits("V");
        phaseVoltageBnDataPoint.setMandatory(true);
        phaseVoltageBnDataPoint.setSize(1);
        phaseVoltageBnDataPoint.setAddressOffset(9);
        phaseVoltageBnDataPoint.setBlockOffset(7);
        phaseVoltageBnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageBnDataPoint.name(), phaseVoltageBnDataPoint);

        SunSpecDataPoint phaseVoltageCnDataPoint;
        phaseVoltageCnDataPoint.setName("PhVphC");
        phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
        phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
        phaseVoltageCnDataPoint.setUnits("V");
        phaseVoltageCnDataPoint.setSize(1);
        phaseVoltageCnDataPoint.setAddressOffset(10);
        phaseVoltageCnDataPoint.setBlockOffset(8);
        phaseVoltageCnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageCnDataPoint.name(), phaseVoltageCnDataPoint);

        SunSpecDataPoint voltageLlDataPoint;
        voltageLlDataPoint.setName("PPV");
        voltageLlDataPoint.setLabel("Voltage LL");
        voltageLlDataPoint.setDescription("Line to Line AC Voltage (average of active phases)");
        voltageLlDataPoint.setUnits("V");
        voltageLlDataPoint.setMandatory(true);
        voltageLlDataPoint.setSize(1);
        voltageLlDataPoint.setAddressOffset(11);
        voltageLlDataPoint.setBlockOffset(9);
        voltageLlDataPoint.setScaleFactorName("V_SF");
        voltageLlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(voltageLlDataPoint.name(), voltageLlDataPoint);

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PhVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setMandatory(true);
        phaseVoltageAbDataPoint.setSize(1);
        phaseVoltageAbDataPoint.setAddressOffset(12);
        phaseVoltageAbDataPoint.setBlockOffset(10);
        phaseVoltageAbDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageAbDataPoint.name(), phaseVoltageAbDataPoint);

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PhVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setSize(1);
        phaseVoltageBcDataPoint.setAddressOffset(13);
        phaseVoltageBcDataPoint.setBlockOffset(11);
        phaseVoltageBcDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageBcDataPoint.name(), phaseVoltageBcDataPoint);

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PhVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setSize(1);
        phaseVoltageCaDataPoint.setAddressOffset(14);
        phaseVoltageCaDataPoint.setBlockOffset(12);
        phaseVoltageCaDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageCaDataPoint.name(), phaseVoltageCaDataPoint);

        SunSpecDataPoint V_SFDataPoint;
        V_SFDataPoint.setName("V_SF");
        V_SFDataPoint.setDescription("Voltage scale factor");
        V_SFDataPoint.setMandatory(true);
        V_SFDataPoint.setSize(1);
        V_SFDataPoint.setAddressOffset(15);
        V_SFDataPoint.setBlockOffset(13);
        V_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(V_SFDataPoint.name(), V_SFDataPoint);

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(1);
        hzDataPoint.setAddressOffset(16);
        hzDataPoint.setBlockOffset(14);
        hzDataPoint.setScaleFactorName("Hz_SF");
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(hzDataPoint.name(), hzDataPoint);

        SunSpecDataPoint Hz_SFDataPoint;
        Hz_SFDataPoint.setName("Hz_SF");
        Hz_SFDataPoint.setDescription("Frequency scale factor");
        Hz_SFDataPoint.setSize(1);
        Hz_SFDataPoint.setAddressOffset(17);
        Hz_SFDataPoint.setBlockOffset(15);
        Hz_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(Hz_SFDataPoint.name(), Hz_SFDataPoint);

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("Total Real Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(1);
        wattsDataPoint.setAddressOffset(18);
        wattsDataPoint.setBlockOffset(16);
        wattsDataPoint.setScaleFactorName("W_SF");
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsDataPoint.name(), wattsDataPoint);

        SunSpecDataPoint wattsPhaseADataPoint;
        wattsPhaseADataPoint.setName("WphA");
        wattsPhaseADataPoint.setLabel("Watts phase A");
        wattsPhaseADataPoint.setUnits("W");
        wattsPhaseADataPoint.setSize(1);
        wattsPhaseADataPoint.setAddressOffset(19);
        wattsPhaseADataPoint.setBlockOffset(17);
        wattsPhaseADataPoint.setScaleFactorName("W_SF");
        wattsPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsPhaseADataPoint.name(), wattsPhaseADataPoint);

        SunSpecDataPoint wattsPhaseBDataPoint;
        wattsPhaseBDataPoint.setName("WphB");
        wattsPhaseBDataPoint.setLabel("Watts phase B");
        wattsPhaseBDataPoint.setUnits("W");
        wattsPhaseBDataPoint.setSize(1);
        wattsPhaseBDataPoint.setAddressOffset(20);
        wattsPhaseBDataPoint.setBlockOffset(18);
        wattsPhaseBDataPoint.setScaleFactorName("W_SF");
        wattsPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsPhaseBDataPoint.name(), wattsPhaseBDataPoint);

        SunSpecDataPoint wattsPhaseCDataPoint;
        wattsPhaseCDataPoint.setName("WphC");
        wattsPhaseCDataPoint.setLabel("Watts phase C");
        wattsPhaseCDataPoint.setUnits("W");
        wattsPhaseCDataPoint.setSize(1);
        wattsPhaseCDataPoint.setAddressOffset(21);
        wattsPhaseCDataPoint.setBlockOffset(19);
        wattsPhaseCDataPoint.setScaleFactorName("W_SF");
        wattsPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsPhaseCDataPoint.name(), wattsPhaseCDataPoint);

        SunSpecDataPoint W_SFDataPoint;
        W_SFDataPoint.setName("W_SF");
        W_SFDataPoint.setDescription("Real Power scale factor");
        W_SFDataPoint.setMandatory(true);
        W_SFDataPoint.setSize(1);
        W_SFDataPoint.setAddressOffset(22);
        W_SFDataPoint.setBlockOffset(20);
        W_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(W_SFDataPoint.name(), W_SFDataPoint);

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(1);
        vaDataPoint.setAddressOffset(23);
        vaDataPoint.setBlockOffset(21);
        vaDataPoint.setScaleFactorName("VA_SF");
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(vaDataPoint.name(), vaDataPoint);

        SunSpecDataPoint vaPhaseADataPoint;
        vaPhaseADataPoint.setName("VAphA");
        vaPhaseADataPoint.setLabel("VA phase A");
        vaPhaseADataPoint.setUnits("VA");
        vaPhaseADataPoint.setSize(1);
        vaPhaseADataPoint.setAddressOffset(24);
        vaPhaseADataPoint.setBlockOffset(22);
        vaPhaseADataPoint.setScaleFactorName("VA_SF");
        vaPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(vaPhaseADataPoint.name(), vaPhaseADataPoint);

        SunSpecDataPoint vaPhaseBDataPoint;
        vaPhaseBDataPoint.setName("VAphB");
        vaPhaseBDataPoint.setLabel("VA phase B");
        vaPhaseBDataPoint.setUnits("VA");
        vaPhaseBDataPoint.setSize(1);
        vaPhaseBDataPoint.setAddressOffset(25);
        vaPhaseBDataPoint.setBlockOffset(23);
        vaPhaseBDataPoint.setScaleFactorName("VA_SF");
        vaPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(vaPhaseBDataPoint.name(), vaPhaseBDataPoint);

        SunSpecDataPoint vaPhaseCDataPoint;
        vaPhaseCDataPoint.setName("VAphC");
        vaPhaseCDataPoint.setLabel("VA phase C");
        vaPhaseCDataPoint.setUnits("VA");
        vaPhaseCDataPoint.setSize(1);
        vaPhaseCDataPoint.setAddressOffset(26);
        vaPhaseCDataPoint.setBlockOffset(24);
        vaPhaseCDataPoint.setScaleFactorName("VA_SF");
        vaPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(vaPhaseCDataPoint.name(), vaPhaseCDataPoint);

        SunSpecDataPoint VA_SFDataPoint;
        VA_SFDataPoint.setName("VA_SF");
        VA_SFDataPoint.setDescription("Apparent Power scale factor");
        VA_SFDataPoint.setSize(1);
        VA_SFDataPoint.setAddressOffset(27);
        VA_SFDataPoint.setBlockOffset(25);
        VA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(VA_SFDataPoint.name(), VA_SFDataPoint);

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAR");
        varDataPoint.setLabel("VAR");
        varDataPoint.setDescription("Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(1);
        varDataPoint.setAddressOffset(28);
        varDataPoint.setBlockOffset(26);
        varDataPoint.setScaleFactorName("VAR_SF");
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(varDataPoint.name(), varDataPoint);

        SunSpecDataPoint varPhaseADataPoint;
        varPhaseADataPoint.setName("VARphA");
        varPhaseADataPoint.setLabel("VAR phase A");
        varPhaseADataPoint.setUnits("var");
        varPhaseADataPoint.setSize(1);
        varPhaseADataPoint.setAddressOffset(29);
        varPhaseADataPoint.setBlockOffset(27);
        varPhaseADataPoint.setScaleFactorName("VAR_SF");
        varPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(varPhaseADataPoint.name(), varPhaseADataPoint);

        SunSpecDataPoint varPhaseBDataPoint;
        varPhaseBDataPoint.setName("VARphB");
        varPhaseBDataPoint.setLabel("VAR phase B");
        varPhaseBDataPoint.setUnits("var");
        varPhaseBDataPoint.setSize(1);
        varPhaseBDataPoint.setAddressOffset(30);
        varPhaseBDataPoint.setBlockOffset(28);
        varPhaseBDataPoint.setScaleFactorName("VAR_SF");
        varPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(varPhaseBDataPoint.name(), varPhaseBDataPoint);

        SunSpecDataPoint varPhaseCDataPoint;
        varPhaseCDataPoint.setName("VARphC");
        varPhaseCDataPoint.setLabel("VAR phase C");
        varPhaseCDataPoint.setUnits("var");
        varPhaseCDataPoint.setSize(1);
        varPhaseCDataPoint.setAddressOffset(31);
        varPhaseCDataPoint.setBlockOffset(29);
        varPhaseCDataPoint.setScaleFactorName("VAR_SF");
        varPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(varPhaseCDataPoint.name(), varPhaseCDataPoint);

        SunSpecDataPoint VAR_SFDataPoint;
        VAR_SFDataPoint.setName("VAR_SF");
        VAR_SFDataPoint.setDescription("Reactive Power scale factor");
        VAR_SFDataPoint.setSize(1);
        VAR_SFDataPoint.setAddressOffset(32);
        VAR_SFDataPoint.setBlockOffset(30);
        VAR_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(VAR_SFDataPoint.name(), VAR_SFDataPoint);

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("Power Factor");
        pfDataPoint.setUnits("Pct");
        pfDataPoint.setSize(1);
        pfDataPoint.setAddressOffset(33);
        pfDataPoint.setBlockOffset(31);
        pfDataPoint.setScaleFactorName("PF_SF");
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(pfDataPoint.name(), pfDataPoint);

        SunSpecDataPoint pfPhaseADataPoint;
        pfPhaseADataPoint.setName("PFphA");
        pfPhaseADataPoint.setLabel("PF phase A");
        pfPhaseADataPoint.setUnits("Pct");
        pfPhaseADataPoint.setSize(1);
        pfPhaseADataPoint.setAddressOffset(34);
        pfPhaseADataPoint.setBlockOffset(32);
        pfPhaseADataPoint.setScaleFactorName("PF_SF");
        pfPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(pfPhaseADataPoint.name(), pfPhaseADataPoint);

        SunSpecDataPoint pfPhaseBDataPoint;
        pfPhaseBDataPoint.setName("PFphB");
        pfPhaseBDataPoint.setLabel("PF phase B");
        pfPhaseBDataPoint.setUnits("Pct");
        pfPhaseBDataPoint.setSize(1);
        pfPhaseBDataPoint.setAddressOffset(35);
        pfPhaseBDataPoint.setBlockOffset(33);
        pfPhaseBDataPoint.setScaleFactorName("PF_SF");
        pfPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(pfPhaseBDataPoint.name(), pfPhaseBDataPoint);

        SunSpecDataPoint pfPhaseCDataPoint;
        pfPhaseCDataPoint.setName("PFphC");
        pfPhaseCDataPoint.setLabel("PF phase C");
        pfPhaseCDataPoint.setUnits("Pct");
        pfPhaseCDataPoint.setSize(1);
        pfPhaseCDataPoint.setAddressOffset(36);
        pfPhaseCDataPoint.setBlockOffset(34);
        pfPhaseCDataPoint.setScaleFactorName("PF_SF");
        pfPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(pfPhaseCDataPoint.name(), pfPhaseCDataPoint);

        SunSpecDataPoint PF_SFDataPoint;
        PF_SFDataPoint.setName("PF_SF");
        PF_SFDataPoint.setDescription("Power Factor scale factor");
        PF_SFDataPoint.setSize(1);
        PF_SFDataPoint.setAddressOffset(37);
        PF_SFDataPoint.setBlockOffset(35);
        PF_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(PF_SFDataPoint.name(), PF_SFDataPoint);

        SunSpecDataPoint totalWattHoursExportedDataPoint;
        totalWattHoursExportedDataPoint.setName("TotWhExp");
        totalWattHoursExportedDataPoint.setLabel("Total Watt-hours Exported");
        totalWattHoursExportedDataPoint.setDescription("Total Real Energy Exported");
        totalWattHoursExportedDataPoint.setUnits("Wh");
        totalWattHoursExportedDataPoint.setMandatory(true);
        totalWattHoursExportedDataPoint.setSize(2);
        totalWattHoursExportedDataPoint.setAddressOffset(38);
        totalWattHoursExportedDataPoint.setBlockOffset(36);
        totalWattHoursExportedDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursExportedDataPoint.name(), totalWattHoursExportedDataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseADataPoint;
        totalWattHoursExportedPhaseADataPoint.setName("TotWhExpPhA");
        totalWattHoursExportedPhaseADataPoint.setLabel("Total Watt-hours Exported phase A");
        totalWattHoursExportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseADataPoint.setSize(2);
        totalWattHoursExportedPhaseADataPoint.setAddressOffset(40);
        totalWattHoursExportedPhaseADataPoint.setBlockOffset(38);
        totalWattHoursExportedPhaseADataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursExportedPhaseADataPoint.name(), totalWattHoursExportedPhaseADataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseBDataPoint;
        totalWattHoursExportedPhaseBDataPoint.setName("TotWhExpPhB");
        totalWattHoursExportedPhaseBDataPoint.setLabel("Total Watt-hours Exported phase B");
        totalWattHoursExportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseBDataPoint.setSize(2);
        totalWattHoursExportedPhaseBDataPoint.setAddressOffset(42);
        totalWattHoursExportedPhaseBDataPoint.setBlockOffset(40);
        totalWattHoursExportedPhaseBDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursExportedPhaseBDataPoint.name(), totalWattHoursExportedPhaseBDataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseCDataPoint;
        totalWattHoursExportedPhaseCDataPoint.setName("TotWhExpPhC");
        totalWattHoursExportedPhaseCDataPoint.setLabel("Total Watt-hours Exported phase C");
        totalWattHoursExportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseCDataPoint.setSize(2);
        totalWattHoursExportedPhaseCDataPoint.setAddressOffset(44);
        totalWattHoursExportedPhaseCDataPoint.setBlockOffset(42);
        totalWattHoursExportedPhaseCDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursExportedPhaseCDataPoint.name(), totalWattHoursExportedPhaseCDataPoint);

        SunSpecDataPoint totalWattHoursImportedDataPoint;
        totalWattHoursImportedDataPoint.setName("TotWhImp");
        totalWattHoursImportedDataPoint.setLabel("Total Watt-hours Imported");
        totalWattHoursImportedDataPoint.setDescription("Total Real Energy Imported");
        totalWattHoursImportedDataPoint.setUnits("Wh");
        totalWattHoursImportedDataPoint.setMandatory(true);
        totalWattHoursImportedDataPoint.setSize(2);
        totalWattHoursImportedDataPoint.setAddressOffset(46);
        totalWattHoursImportedDataPoint.setBlockOffset(44);
        totalWattHoursImportedDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursImportedDataPoint.name(), totalWattHoursImportedDataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseADataPoint;
        totalWattHoursImportedPhaseADataPoint.setName("TotWhImpPhA");
        totalWattHoursImportedPhaseADataPoint.setLabel("Total Watt-hours Imported phase A");
        totalWattHoursImportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseADataPoint.setSize(2);
        totalWattHoursImportedPhaseADataPoint.setAddressOffset(48);
        totalWattHoursImportedPhaseADataPoint.setBlockOffset(46);
        totalWattHoursImportedPhaseADataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseADataPoint.name(), totalWattHoursImportedPhaseADataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseBDataPoint;
        totalWattHoursImportedPhaseBDataPoint.setName("TotWhImpPhB");
        totalWattHoursImportedPhaseBDataPoint.setLabel("Total Watt-hours Imported phase B");
        totalWattHoursImportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseBDataPoint.setSize(2);
        totalWattHoursImportedPhaseBDataPoint.setAddressOffset(50);
        totalWattHoursImportedPhaseBDataPoint.setBlockOffset(48);
        totalWattHoursImportedPhaseBDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseBDataPoint.name(), totalWattHoursImportedPhaseBDataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseCDataPoint;
        totalWattHoursImportedPhaseCDataPoint.setName("TotWhImpPhC");
        totalWattHoursImportedPhaseCDataPoint.setLabel("Total Watt-hours Imported phase C");
        totalWattHoursImportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseCDataPoint.setSize(2);
        totalWattHoursImportedPhaseCDataPoint.setAddressOffset(52);
        totalWattHoursImportedPhaseCDataPoint.setBlockOffset(50);
        totalWattHoursImportedPhaseCDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseCDataPoint.name(), totalWattHoursImportedPhaseCDataPoint);

        SunSpecDataPoint TotWh_SFDataPoint;
        TotWh_SFDataPoint.setName("TotWh_SF");
        TotWh_SFDataPoint.setDescription("Real Energy scale factor");
        TotWh_SFDataPoint.setMandatory(true);
        TotWh_SFDataPoint.setSize(1);
        TotWh_SFDataPoint.setAddressOffset(54);
        TotWh_SFDataPoint.setBlockOffset(52);
        TotWh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(TotWh_SFDataPoint.name(), TotWh_SFDataPoint);

        SunSpecDataPoint totalVaHoursExportedDataPoint;
        totalVaHoursExportedDataPoint.setName("TotVAhExp");
        totalVaHoursExportedDataPoint.setLabel("Total VA-hours Exported");
        totalVaHoursExportedDataPoint.setDescription("Total Apparent Energy Exported");
        totalVaHoursExportedDataPoint.setUnits("VAh");
        totalVaHoursExportedDataPoint.setSize(2);
        totalVaHoursExportedDataPoint.setAddressOffset(55);
        totalVaHoursExportedDataPoint.setBlockOffset(53);
        totalVaHoursExportedDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursExportedDataPoint.name(), totalVaHoursExportedDataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseADataPoint;
        totalVaHoursExportedPhaseADataPoint.setName("TotVAhExpPhA");
        totalVaHoursExportedPhaseADataPoint.setLabel("Total VA-hours Exported phase A");
        totalVaHoursExportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseADataPoint.setSize(2);
        totalVaHoursExportedPhaseADataPoint.setAddressOffset(57);
        totalVaHoursExportedPhaseADataPoint.setBlockOffset(55);
        totalVaHoursExportedPhaseADataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseADataPoint.name(), totalVaHoursExportedPhaseADataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseBDataPoint;
        totalVaHoursExportedPhaseBDataPoint.setName("TotVAhExpPhB");
        totalVaHoursExportedPhaseBDataPoint.setLabel("Total VA-hours Exported phase B");
        totalVaHoursExportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseBDataPoint.setSize(2);
        totalVaHoursExportedPhaseBDataPoint.setAddressOffset(59);
        totalVaHoursExportedPhaseBDataPoint.setBlockOffset(57);
        totalVaHoursExportedPhaseBDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseBDataPoint.name(), totalVaHoursExportedPhaseBDataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseCDataPoint;
        totalVaHoursExportedPhaseCDataPoint.setName("TotVAhExpPhC");
        totalVaHoursExportedPhaseCDataPoint.setLabel("Total VA-hours Exported phase C");
        totalVaHoursExportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseCDataPoint.setSize(2);
        totalVaHoursExportedPhaseCDataPoint.setAddressOffset(61);
        totalVaHoursExportedPhaseCDataPoint.setBlockOffset(59);
        totalVaHoursExportedPhaseCDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseCDataPoint.name(), totalVaHoursExportedPhaseCDataPoint);

        SunSpecDataPoint totalVaHoursImportedDataPoint;
        totalVaHoursImportedDataPoint.setName("TotVAhImp");
        totalVaHoursImportedDataPoint.setLabel("Total VA-hours Imported");
        totalVaHoursImportedDataPoint.setDescription("Total Apparent Energy Imported");
        totalVaHoursImportedDataPoint.setUnits("VAh");
        totalVaHoursImportedDataPoint.setSize(2);
        totalVaHoursImportedDataPoint.setAddressOffset(63);
        totalVaHoursImportedDataPoint.setBlockOffset(61);
        totalVaHoursImportedDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursImportedDataPoint.name(), totalVaHoursImportedDataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseADataPoint;
        totalVaHoursImportedPhaseADataPoint.setName("TotVAhImpPhA");
        totalVaHoursImportedPhaseADataPoint.setLabel("Total VA-hours Imported phase A");
        totalVaHoursImportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseADataPoint.setSize(2);
        totalVaHoursImportedPhaseADataPoint.setAddressOffset(65);
        totalVaHoursImportedPhaseADataPoint.setBlockOffset(63);
        totalVaHoursImportedPhaseADataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseADataPoint.name(), totalVaHoursImportedPhaseADataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseBDataPoint;
        totalVaHoursImportedPhaseBDataPoint.setName("TotVAhImpPhB");
        totalVaHoursImportedPhaseBDataPoint.setLabel("Total VA-hours Imported phase B");
        totalVaHoursImportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseBDataPoint.setSize(2);
        totalVaHoursImportedPhaseBDataPoint.setAddressOffset(67);
        totalVaHoursImportedPhaseBDataPoint.setBlockOffset(65);
        totalVaHoursImportedPhaseBDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseBDataPoint.name(), totalVaHoursImportedPhaseBDataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseCDataPoint;
        totalVaHoursImportedPhaseCDataPoint.setName("TotVAhImpPhC");
        totalVaHoursImportedPhaseCDataPoint.setLabel("Total VA-hours Imported phase C");
        totalVaHoursImportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseCDataPoint.setSize(2);
        totalVaHoursImportedPhaseCDataPoint.setAddressOffset(69);
        totalVaHoursImportedPhaseCDataPoint.setBlockOffset(67);
        totalVaHoursImportedPhaseCDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseCDataPoint.name(), totalVaHoursImportedPhaseCDataPoint);

        SunSpecDataPoint TotVAh_SFDataPoint;
        TotVAh_SFDataPoint.setName("TotVAh_SF");
        TotVAh_SFDataPoint.setDescription("Apparent Energy scale factor");
        TotVAh_SFDataPoint.setSize(1);
        TotVAh_SFDataPoint.setAddressOffset(71);
        TotVAh_SFDataPoint.setBlockOffset(69);
        TotVAh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(TotVAh_SFDataPoint.name(), TotVAh_SFDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1DataPoint;
        totalVarHoursImportedQ1DataPoint.setName("TotVArhImpQ1");
        totalVarHoursImportedQ1DataPoint.setLabel("Total VAR-hours Imported Q1");
        totalVarHoursImportedQ1DataPoint.setDescription("Total Reactive Energy Imported Quadrant 1");
        totalVarHoursImportedQ1DataPoint.setUnits("varh");
        totalVarHoursImportedQ1DataPoint.setSize(2);
        totalVarHoursImportedQ1DataPoint.setAddressOffset(72);
        totalVarHoursImportedQ1DataPoint.setBlockOffset(70);
        totalVarHoursImportedQ1DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ1DataPoint.name(), totalVarHoursImportedQ1DataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseADataPoint;
        totalVarHoursImportedQ1PhaseADataPoint.setName("TotVArhImpQ1PhA");
        totalVarHoursImportedQ1PhaseADataPoint.setLabel("Total VAr-hours Imported Q1 phase A");
        totalVarHoursImportedQ1PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseADataPoint.setAddressOffset(74);
        totalVarHoursImportedQ1PhaseADataPoint.setBlockOffset(72);
        totalVarHoursImportedQ1PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseADataPoint.name(), totalVarHoursImportedQ1PhaseADataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseBDataPoint;
        totalVarHoursImportedQ1PhaseBDataPoint.setName("TotVArhImpQ1PhB");
        totalVarHoursImportedQ1PhaseBDataPoint.setLabel("Total VAr-hours Imported Q1 phase B");
        totalVarHoursImportedQ1PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseBDataPoint.setAddressOffset(76);
        totalVarHoursImportedQ1PhaseBDataPoint.setBlockOffset(74);
        totalVarHoursImportedQ1PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseBDataPoint.name(), totalVarHoursImportedQ1PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseCDataPoint;
        totalVarHoursImportedQ1PhaseCDataPoint.setName("TotVArhImpQ1PhC");
        totalVarHoursImportedQ1PhaseCDataPoint.setLabel("Total VAr-hours Imported Q1 phase C");
        totalVarHoursImportedQ1PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseCDataPoint.setAddressOffset(78);
        totalVarHoursImportedQ1PhaseCDataPoint.setBlockOffset(76);
        totalVarHoursImportedQ1PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseCDataPoint.name(), totalVarHoursImportedQ1PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2DataPoint;
        totalVarHoursImportedQ2DataPoint.setName("TotVArhImpQ2");
        totalVarHoursImportedQ2DataPoint.setLabel("Total VAr-hours Imported Q2");
        totalVarHoursImportedQ2DataPoint.setDescription("Total Reactive Power Imported Quadrant 2");
        totalVarHoursImportedQ2DataPoint.setUnits("varh");
        totalVarHoursImportedQ2DataPoint.setSize(2);
        totalVarHoursImportedQ2DataPoint.setAddressOffset(80);
        totalVarHoursImportedQ2DataPoint.setBlockOffset(78);
        totalVarHoursImportedQ2DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ2DataPoint.name(), totalVarHoursImportedQ2DataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseADataPoint;
        totalVarHoursImportedQ2PhaseADataPoint.setName("TotVArhImpQ2PhA");
        totalVarHoursImportedQ2PhaseADataPoint.setLabel("Total VAr-hours Imported Q2 phase A");
        totalVarHoursImportedQ2PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseADataPoint.setAddressOffset(82);
        totalVarHoursImportedQ2PhaseADataPoint.setBlockOffset(80);
        totalVarHoursImportedQ2PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseADataPoint.name(), totalVarHoursImportedQ2PhaseADataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseBDataPoint;
        totalVarHoursImportedQ2PhaseBDataPoint.setName("TotVArhImpQ2PhB");
        totalVarHoursImportedQ2PhaseBDataPoint.setLabel("Total VAr-hours Imported Q2 phase B");
        totalVarHoursImportedQ2PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseBDataPoint.setAddressOffset(84);
        totalVarHoursImportedQ2PhaseBDataPoint.setBlockOffset(82);
        totalVarHoursImportedQ2PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseBDataPoint.name(), totalVarHoursImportedQ2PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseCDataPoint;
        totalVarHoursImportedQ2PhaseCDataPoint.setName("TotVArhImpQ2PhC");
        totalVarHoursImportedQ2PhaseCDataPoint.setLabel("Total VAr-hours Imported Q2 phase C");
        totalVarHoursImportedQ2PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseCDataPoint.setAddressOffset(86);
        totalVarHoursImportedQ2PhaseCDataPoint.setBlockOffset(84);
        totalVarHoursImportedQ2PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseCDataPoint.name(), totalVarHoursImportedQ2PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3DataPoint;
        totalVarHoursExportedQ3DataPoint.setName("TotVArhExpQ3");
        totalVarHoursExportedQ3DataPoint.setLabel("Total VAr-hours Exported Q3");
        totalVarHoursExportedQ3DataPoint.setDescription("Total Reactive Power Exported Quadrant 3");
        totalVarHoursExportedQ3DataPoint.setUnits("varh");
        totalVarHoursExportedQ3DataPoint.setSize(2);
        totalVarHoursExportedQ3DataPoint.setAddressOffset(88);
        totalVarHoursExportedQ3DataPoint.setBlockOffset(86);
        totalVarHoursExportedQ3DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ3DataPoint.name(), totalVarHoursExportedQ3DataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseADataPoint;
        totalVarHoursExportedQ3PhaseADataPoint.setName("TotVArhExpQ3PhA");
        totalVarHoursExportedQ3PhaseADataPoint.setLabel("Total VAr-hours Exported Q3 phase A");
        totalVarHoursExportedQ3PhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseADataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseADataPoint.setAddressOffset(90);
        totalVarHoursExportedQ3PhaseADataPoint.setBlockOffset(88);
        totalVarHoursExportedQ3PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseADataPoint.name(), totalVarHoursExportedQ3PhaseADataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseBDataPoint;
        totalVarHoursExportedQ3PhaseBDataPoint.setName("TotVArhExpQ3PhB");
        totalVarHoursExportedQ3PhaseBDataPoint.setLabel("Total VAr-hours Exported Q3 phase B");
        totalVarHoursExportedQ3PhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseBDataPoint.setAddressOffset(92);
        totalVarHoursExportedQ3PhaseBDataPoint.setBlockOffset(90);
        totalVarHoursExportedQ3PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseBDataPoint.name(), totalVarHoursExportedQ3PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseCDataPoint;
        totalVarHoursExportedQ3PhaseCDataPoint.setName("TotVArhExpQ3PhC");
        totalVarHoursExportedQ3PhaseCDataPoint.setLabel("Total VAr-hours Exported Q3 phase C");
        totalVarHoursExportedQ3PhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseCDataPoint.setAddressOffset(94);
        totalVarHoursExportedQ3PhaseCDataPoint.setBlockOffset(92);
        totalVarHoursExportedQ3PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseCDataPoint.name(), totalVarHoursExportedQ3PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4DataPoint;
        totalVarHoursExportedQ4DataPoint.setName("TotVArhExpQ4");
        totalVarHoursExportedQ4DataPoint.setLabel("Total VAr-hours Exported Q4");
        totalVarHoursExportedQ4DataPoint.setDescription("Total Reactive Power Exported Quadrant 4");
        totalVarHoursExportedQ4DataPoint.setUnits("varh");
        totalVarHoursExportedQ4DataPoint.setSize(2);
        totalVarHoursExportedQ4DataPoint.setAddressOffset(96);
        totalVarHoursExportedQ4DataPoint.setBlockOffset(94);
        totalVarHoursExportedQ4DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ4DataPoint.name(), totalVarHoursExportedQ4DataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseADataPoint;
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setName("TotVArhExpQ4PhA");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase A");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setAddressOffset(98);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setBlockOffset(96);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseADataPoint.name(), totalVarHoursExportedQ4ImportedPhaseADataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseBDataPoint;
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setName("TotVArhExpQ4PhB");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase B");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setAddressOffset(100);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setBlockOffset(98);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseBDataPoint.name(), totalVarHoursExportedQ4ImportedPhaseBDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseCDataPoint;
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setName("TotVArhExpQ4PhC");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase C");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setAddressOffset(102);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setBlockOffset(100);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseCDataPoint.name(), totalVarHoursExportedQ4ImportedPhaseCDataPoint);

        SunSpecDataPoint TotVArh_SFDataPoint;
        TotVArh_SFDataPoint.setName("TotVArh_SF");
        TotVArh_SFDataPoint.setDescription("Reactive Energy scale factor");
        TotVArh_SFDataPoint.setSize(1);
        TotVArh_SFDataPoint.setAddressOffset(104);
        TotVArh_SFDataPoint.setBlockOffset(102);
        TotVArh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(TotVArh_SFDataPoint.name(), TotVArh_SFDataPoint);

        SunSpecDataPoint eventsDataPoint;
        eventsDataPoint.setName("Evt");
        eventsDataPoint.setLabel("Events");
        eventsDataPoint.setDescription("Meter Event Flags");
        eventsDataPoint.setMandatory(true);
        eventsDataPoint.setSize(2);
        eventsDataPoint.setAddressOffset(105);
        eventsDataPoint.setBlockOffset(103);
        eventsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(eventsDataPoint.name(), eventsDataPoint);

        break;
    }
    case 203: {
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
        ampsDataPoint.setDescription("Total AC Current");
        ampsDataPoint.setUnits("A");
        ampsDataPoint.setMandatory(true);
        ampsDataPoint.setSize(1);
        ampsDataPoint.setAddressOffset(2);
        ampsDataPoint.setBlockOffset(0);
        ampsDataPoint.setScaleFactorName("A_SF");
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
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
        ampsPhaseaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
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
        ampsPhasebDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
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
        ampsPhasecDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(ampsPhasecDataPoint.name(), ampsPhasecDataPoint);

        SunSpecDataPoint A_SFDataPoint;
        A_SFDataPoint.setName("A_SF");
        A_SFDataPoint.setDescription("Current scale factor");
        A_SFDataPoint.setMandatory(true);
        A_SFDataPoint.setSize(1);
        A_SFDataPoint.setAddressOffset(6);
        A_SFDataPoint.setBlockOffset(4);
        A_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(A_SFDataPoint.name(), A_SFDataPoint);

        SunSpecDataPoint voltageLnDataPoint;
        voltageLnDataPoint.setName("PhV");
        voltageLnDataPoint.setLabel("Voltage LN");
        voltageLnDataPoint.setDescription("Line to Neutral AC Voltage (average of active phases)");
        voltageLnDataPoint.setUnits("V");
        voltageLnDataPoint.setMandatory(true);
        voltageLnDataPoint.setSize(1);
        voltageLnDataPoint.setAddressOffset(7);
        voltageLnDataPoint.setBlockOffset(5);
        voltageLnDataPoint.setScaleFactorName("V_SF");
        voltageLnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(voltageLnDataPoint.name(), voltageLnDataPoint);

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
        phaseVoltageAnDataPoint.setMandatory(true);
        phaseVoltageAnDataPoint.setSize(1);
        phaseVoltageAnDataPoint.setAddressOffset(8);
        phaseVoltageAnDataPoint.setBlockOffset(6);
        phaseVoltageAnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageAnDataPoint.name(), phaseVoltageAnDataPoint);

        SunSpecDataPoint phaseVoltageBnDataPoint;
        phaseVoltageBnDataPoint.setName("PhVphB");
        phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
        phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
        phaseVoltageBnDataPoint.setUnits("V");
        phaseVoltageBnDataPoint.setMandatory(true);
        phaseVoltageBnDataPoint.setSize(1);
        phaseVoltageBnDataPoint.setAddressOffset(9);
        phaseVoltageBnDataPoint.setBlockOffset(7);
        phaseVoltageBnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageBnDataPoint.name(), phaseVoltageBnDataPoint);

        SunSpecDataPoint phaseVoltageCnDataPoint;
        phaseVoltageCnDataPoint.setName("PhVphC");
        phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
        phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
        phaseVoltageCnDataPoint.setUnits("V");
        phaseVoltageCnDataPoint.setMandatory(true);
        phaseVoltageCnDataPoint.setSize(1);
        phaseVoltageCnDataPoint.setAddressOffset(10);
        phaseVoltageCnDataPoint.setBlockOffset(8);
        phaseVoltageCnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageCnDataPoint.name(), phaseVoltageCnDataPoint);

        SunSpecDataPoint voltageLlDataPoint;
        voltageLlDataPoint.setName("PPV");
        voltageLlDataPoint.setLabel("Voltage LL");
        voltageLlDataPoint.setDescription("Line to Line AC Voltage (average of active phases)");
        voltageLlDataPoint.setUnits("V");
        voltageLlDataPoint.setMandatory(true);
        voltageLlDataPoint.setSize(1);
        voltageLlDataPoint.setAddressOffset(11);
        voltageLlDataPoint.setBlockOffset(9);
        voltageLlDataPoint.setScaleFactorName("V_SF");
        voltageLlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(voltageLlDataPoint.name(), voltageLlDataPoint);

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PhVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setMandatory(true);
        phaseVoltageAbDataPoint.setSize(1);
        phaseVoltageAbDataPoint.setAddressOffset(12);
        phaseVoltageAbDataPoint.setBlockOffset(10);
        phaseVoltageAbDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageAbDataPoint.name(), phaseVoltageAbDataPoint);

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PhVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setMandatory(true);
        phaseVoltageBcDataPoint.setSize(1);
        phaseVoltageBcDataPoint.setAddressOffset(13);
        phaseVoltageBcDataPoint.setBlockOffset(11);
        phaseVoltageBcDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageBcDataPoint.name(), phaseVoltageBcDataPoint);

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PhVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setMandatory(true);
        phaseVoltageCaDataPoint.setSize(1);
        phaseVoltageCaDataPoint.setAddressOffset(14);
        phaseVoltageCaDataPoint.setBlockOffset(12);
        phaseVoltageCaDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageCaDataPoint.name(), phaseVoltageCaDataPoint);

        SunSpecDataPoint V_SFDataPoint;
        V_SFDataPoint.setName("V_SF");
        V_SFDataPoint.setDescription("Voltage scale factor");
        V_SFDataPoint.setMandatory(true);
        V_SFDataPoint.setSize(1);
        V_SFDataPoint.setAddressOffset(15);
        V_SFDataPoint.setBlockOffset(13);
        V_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(V_SFDataPoint.name(), V_SFDataPoint);

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(1);
        hzDataPoint.setAddressOffset(16);
        hzDataPoint.setBlockOffset(14);
        hzDataPoint.setScaleFactorName("Hz_SF");
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(hzDataPoint.name(), hzDataPoint);

        SunSpecDataPoint Hz_SFDataPoint;
        Hz_SFDataPoint.setName("Hz_SF");
        Hz_SFDataPoint.setDescription("Frequency scale factor");
        Hz_SFDataPoint.setSize(1);
        Hz_SFDataPoint.setAddressOffset(17);
        Hz_SFDataPoint.setBlockOffset(15);
        Hz_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(Hz_SFDataPoint.name(), Hz_SFDataPoint);

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("Total Real Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(1);
        wattsDataPoint.setAddressOffset(18);
        wattsDataPoint.setBlockOffset(16);
        wattsDataPoint.setScaleFactorName("W_SF");
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsDataPoint.name(), wattsDataPoint);

        SunSpecDataPoint wattsPhaseADataPoint;
        wattsPhaseADataPoint.setName("WphA");
        wattsPhaseADataPoint.setLabel("Watts phase A");
        wattsPhaseADataPoint.setUnits("W");
        wattsPhaseADataPoint.setSize(1);
        wattsPhaseADataPoint.setAddressOffset(19);
        wattsPhaseADataPoint.setBlockOffset(17);
        wattsPhaseADataPoint.setScaleFactorName("W_SF");
        wattsPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsPhaseADataPoint.name(), wattsPhaseADataPoint);

        SunSpecDataPoint wattsPhaseBDataPoint;
        wattsPhaseBDataPoint.setName("WphB");
        wattsPhaseBDataPoint.setLabel("Watts phase B");
        wattsPhaseBDataPoint.setUnits("W");
        wattsPhaseBDataPoint.setSize(1);
        wattsPhaseBDataPoint.setAddressOffset(20);
        wattsPhaseBDataPoint.setBlockOffset(18);
        wattsPhaseBDataPoint.setScaleFactorName("W_SF");
        wattsPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsPhaseBDataPoint.name(), wattsPhaseBDataPoint);

        SunSpecDataPoint wattsPhaseCDataPoint;
        wattsPhaseCDataPoint.setName("WphC");
        wattsPhaseCDataPoint.setLabel("Watts phase C");
        wattsPhaseCDataPoint.setUnits("W");
        wattsPhaseCDataPoint.setSize(1);
        wattsPhaseCDataPoint.setAddressOffset(21);
        wattsPhaseCDataPoint.setBlockOffset(19);
        wattsPhaseCDataPoint.setScaleFactorName("W_SF");
        wattsPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsPhaseCDataPoint.name(), wattsPhaseCDataPoint);

        SunSpecDataPoint W_SFDataPoint;
        W_SFDataPoint.setName("W_SF");
        W_SFDataPoint.setDescription("Real Power scale factor");
        W_SFDataPoint.setMandatory(true);
        W_SFDataPoint.setSize(1);
        W_SFDataPoint.setAddressOffset(22);
        W_SFDataPoint.setBlockOffset(20);
        W_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(W_SFDataPoint.name(), W_SFDataPoint);

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(1);
        vaDataPoint.setAddressOffset(23);
        vaDataPoint.setBlockOffset(21);
        vaDataPoint.setScaleFactorName("VA_SF");
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(vaDataPoint.name(), vaDataPoint);

        SunSpecDataPoint vaPhaseADataPoint;
        vaPhaseADataPoint.setName("VAphA");
        vaPhaseADataPoint.setLabel("VA phase A");
        vaPhaseADataPoint.setUnits("VA");
        vaPhaseADataPoint.setSize(1);
        vaPhaseADataPoint.setAddressOffset(24);
        vaPhaseADataPoint.setBlockOffset(22);
        vaPhaseADataPoint.setScaleFactorName("VA_SF");
        vaPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(vaPhaseADataPoint.name(), vaPhaseADataPoint);

        SunSpecDataPoint vaPhaseBDataPoint;
        vaPhaseBDataPoint.setName("VAphB");
        vaPhaseBDataPoint.setLabel("VA phase B");
        vaPhaseBDataPoint.setUnits("VA");
        vaPhaseBDataPoint.setSize(1);
        vaPhaseBDataPoint.setAddressOffset(25);
        vaPhaseBDataPoint.setBlockOffset(23);
        vaPhaseBDataPoint.setScaleFactorName("VA_SF");
        vaPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(vaPhaseBDataPoint.name(), vaPhaseBDataPoint);

        SunSpecDataPoint vaPhaseCDataPoint;
        vaPhaseCDataPoint.setName("VAphC");
        vaPhaseCDataPoint.setLabel("VA phase C");
        vaPhaseCDataPoint.setUnits("VA");
        vaPhaseCDataPoint.setSize(1);
        vaPhaseCDataPoint.setAddressOffset(26);
        vaPhaseCDataPoint.setBlockOffset(24);
        vaPhaseCDataPoint.setScaleFactorName("VA_SF");
        vaPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(vaPhaseCDataPoint.name(), vaPhaseCDataPoint);

        SunSpecDataPoint VA_SFDataPoint;
        VA_SFDataPoint.setName("VA_SF");
        VA_SFDataPoint.setDescription("Apparent Power scale factor");
        VA_SFDataPoint.setSize(1);
        VA_SFDataPoint.setAddressOffset(27);
        VA_SFDataPoint.setBlockOffset(25);
        VA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(VA_SFDataPoint.name(), VA_SFDataPoint);

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAR");
        varDataPoint.setLabel("VAR");
        varDataPoint.setDescription("Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(1);
        varDataPoint.setAddressOffset(28);
        varDataPoint.setBlockOffset(26);
        varDataPoint.setScaleFactorName("VAR_SF");
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(varDataPoint.name(), varDataPoint);

        SunSpecDataPoint varPhaseADataPoint;
        varPhaseADataPoint.setName("VARphA");
        varPhaseADataPoint.setLabel("VAR phase A");
        varPhaseADataPoint.setUnits("var");
        varPhaseADataPoint.setSize(1);
        varPhaseADataPoint.setAddressOffset(29);
        varPhaseADataPoint.setBlockOffset(27);
        varPhaseADataPoint.setScaleFactorName("VAR_SF");
        varPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(varPhaseADataPoint.name(), varPhaseADataPoint);

        SunSpecDataPoint varPhaseBDataPoint;
        varPhaseBDataPoint.setName("VARphB");
        varPhaseBDataPoint.setLabel("VAR phase B");
        varPhaseBDataPoint.setUnits("var");
        varPhaseBDataPoint.setSize(1);
        varPhaseBDataPoint.setAddressOffset(30);
        varPhaseBDataPoint.setBlockOffset(28);
        varPhaseBDataPoint.setScaleFactorName("VAR_SF");
        varPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(varPhaseBDataPoint.name(), varPhaseBDataPoint);

        SunSpecDataPoint varPhaseCDataPoint;
        varPhaseCDataPoint.setName("VARphC");
        varPhaseCDataPoint.setLabel("VAR phase C");
        varPhaseCDataPoint.setUnits("var");
        varPhaseCDataPoint.setSize(1);
        varPhaseCDataPoint.setAddressOffset(31);
        varPhaseCDataPoint.setBlockOffset(29);
        varPhaseCDataPoint.setScaleFactorName("VAR_SF");
        varPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(varPhaseCDataPoint.name(), varPhaseCDataPoint);

        SunSpecDataPoint VAR_SFDataPoint;
        VAR_SFDataPoint.setName("VAR_SF");
        VAR_SFDataPoint.setDescription("Reactive Power scale factor");
        VAR_SFDataPoint.setSize(1);
        VAR_SFDataPoint.setAddressOffset(32);
        VAR_SFDataPoint.setBlockOffset(30);
        VAR_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(VAR_SFDataPoint.name(), VAR_SFDataPoint);

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("Power Factor");
        pfDataPoint.setUnits("Pct");
        pfDataPoint.setSize(1);
        pfDataPoint.setAddressOffset(33);
        pfDataPoint.setBlockOffset(31);
        pfDataPoint.setScaleFactorName("PF_SF");
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(pfDataPoint.name(), pfDataPoint);

        SunSpecDataPoint pfPhaseADataPoint;
        pfPhaseADataPoint.setName("PFphA");
        pfPhaseADataPoint.setLabel("PF phase A");
        pfPhaseADataPoint.setUnits("Pct");
        pfPhaseADataPoint.setSize(1);
        pfPhaseADataPoint.setAddressOffset(34);
        pfPhaseADataPoint.setBlockOffset(32);
        pfPhaseADataPoint.setScaleFactorName("PF_SF");
        pfPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(pfPhaseADataPoint.name(), pfPhaseADataPoint);

        SunSpecDataPoint pfPhaseBDataPoint;
        pfPhaseBDataPoint.setName("PFphB");
        pfPhaseBDataPoint.setLabel("PF phase B");
        pfPhaseBDataPoint.setUnits("Pct");
        pfPhaseBDataPoint.setSize(1);
        pfPhaseBDataPoint.setAddressOffset(35);
        pfPhaseBDataPoint.setBlockOffset(33);
        pfPhaseBDataPoint.setScaleFactorName("PF_SF");
        pfPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(pfPhaseBDataPoint.name(), pfPhaseBDataPoint);

        SunSpecDataPoint pfPhaseCDataPoint;
        pfPhaseCDataPoint.setName("PFphC");
        pfPhaseCDataPoint.setLabel("PF phase C");
        pfPhaseCDataPoint.setUnits("Pct");
        pfPhaseCDataPoint.setSize(1);
        pfPhaseCDataPoint.setAddressOffset(36);
        pfPhaseCDataPoint.setBlockOffset(34);
        pfPhaseCDataPoint.setScaleFactorName("PF_SF");
        pfPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(pfPhaseCDataPoint.name(), pfPhaseCDataPoint);

        SunSpecDataPoint PF_SFDataPoint;
        PF_SFDataPoint.setName("PF_SF");
        PF_SFDataPoint.setDescription("Power Factor scale factor");
        PF_SFDataPoint.setSize(1);
        PF_SFDataPoint.setAddressOffset(37);
        PF_SFDataPoint.setBlockOffset(35);
        PF_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(PF_SFDataPoint.name(), PF_SFDataPoint);

        SunSpecDataPoint totalWattHoursExportedDataPoint;
        totalWattHoursExportedDataPoint.setName("TotWhExp");
        totalWattHoursExportedDataPoint.setLabel("Total Watt-hours Exported");
        totalWattHoursExportedDataPoint.setDescription("Total Real Energy Exported");
        totalWattHoursExportedDataPoint.setUnits("Wh");
        totalWattHoursExportedDataPoint.setMandatory(true);
        totalWattHoursExportedDataPoint.setSize(2);
        totalWattHoursExportedDataPoint.setAddressOffset(38);
        totalWattHoursExportedDataPoint.setBlockOffset(36);
        totalWattHoursExportedDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursExportedDataPoint.name(), totalWattHoursExportedDataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseADataPoint;
        totalWattHoursExportedPhaseADataPoint.setName("TotWhExpPhA");
        totalWattHoursExportedPhaseADataPoint.setLabel("Total Watt-hours Exported phase A");
        totalWattHoursExportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseADataPoint.setSize(2);
        totalWattHoursExportedPhaseADataPoint.setAddressOffset(40);
        totalWattHoursExportedPhaseADataPoint.setBlockOffset(38);
        totalWattHoursExportedPhaseADataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursExportedPhaseADataPoint.name(), totalWattHoursExportedPhaseADataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseBDataPoint;
        totalWattHoursExportedPhaseBDataPoint.setName("TotWhExpPhB");
        totalWattHoursExportedPhaseBDataPoint.setLabel("Total Watt-hours Exported phase B");
        totalWattHoursExportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseBDataPoint.setSize(2);
        totalWattHoursExportedPhaseBDataPoint.setAddressOffset(42);
        totalWattHoursExportedPhaseBDataPoint.setBlockOffset(40);
        totalWattHoursExportedPhaseBDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursExportedPhaseBDataPoint.name(), totalWattHoursExportedPhaseBDataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseCDataPoint;
        totalWattHoursExportedPhaseCDataPoint.setName("TotWhExpPhC");
        totalWattHoursExportedPhaseCDataPoint.setLabel("Total Watt-hours Exported phase C");
        totalWattHoursExportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseCDataPoint.setSize(2);
        totalWattHoursExportedPhaseCDataPoint.setAddressOffset(44);
        totalWattHoursExportedPhaseCDataPoint.setBlockOffset(42);
        totalWattHoursExportedPhaseCDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursExportedPhaseCDataPoint.name(), totalWattHoursExportedPhaseCDataPoint);

        SunSpecDataPoint totalWattHoursImportedDataPoint;
        totalWattHoursImportedDataPoint.setName("TotWhImp");
        totalWattHoursImportedDataPoint.setLabel("Total Watt-hours Imported");
        totalWattHoursImportedDataPoint.setDescription("Total Real Energy Imported");
        totalWattHoursImportedDataPoint.setUnits("Wh");
        totalWattHoursImportedDataPoint.setMandatory(true);
        totalWattHoursImportedDataPoint.setSize(2);
        totalWattHoursImportedDataPoint.setAddressOffset(46);
        totalWattHoursImportedDataPoint.setBlockOffset(44);
        totalWattHoursImportedDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursImportedDataPoint.name(), totalWattHoursImportedDataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseADataPoint;
        totalWattHoursImportedPhaseADataPoint.setName("TotWhImpPhA");
        totalWattHoursImportedPhaseADataPoint.setLabel("Total Watt-hours Imported phase A");
        totalWattHoursImportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseADataPoint.setSize(2);
        totalWattHoursImportedPhaseADataPoint.setAddressOffset(48);
        totalWattHoursImportedPhaseADataPoint.setBlockOffset(46);
        totalWattHoursImportedPhaseADataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseADataPoint.name(), totalWattHoursImportedPhaseADataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseBDataPoint;
        totalWattHoursImportedPhaseBDataPoint.setName("TotWhImpPhB");
        totalWattHoursImportedPhaseBDataPoint.setLabel("Total Watt-hours Imported phase B");
        totalWattHoursImportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseBDataPoint.setSize(2);
        totalWattHoursImportedPhaseBDataPoint.setAddressOffset(50);
        totalWattHoursImportedPhaseBDataPoint.setBlockOffset(48);
        totalWattHoursImportedPhaseBDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseBDataPoint.name(), totalWattHoursImportedPhaseBDataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseCDataPoint;
        totalWattHoursImportedPhaseCDataPoint.setName("TotWhImpPhC");
        totalWattHoursImportedPhaseCDataPoint.setLabel("Total Watt-hours Imported phase C");
        totalWattHoursImportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseCDataPoint.setSize(2);
        totalWattHoursImportedPhaseCDataPoint.setAddressOffset(52);
        totalWattHoursImportedPhaseCDataPoint.setBlockOffset(50);
        totalWattHoursImportedPhaseCDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseCDataPoint.name(), totalWattHoursImportedPhaseCDataPoint);

        SunSpecDataPoint TotWh_SFDataPoint;
        TotWh_SFDataPoint.setName("TotWh_SF");
        TotWh_SFDataPoint.setDescription("Real Energy scale factor");
        TotWh_SFDataPoint.setMandatory(true);
        TotWh_SFDataPoint.setSize(1);
        TotWh_SFDataPoint.setAddressOffset(54);
        TotWh_SFDataPoint.setBlockOffset(52);
        TotWh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(TotWh_SFDataPoint.name(), TotWh_SFDataPoint);

        SunSpecDataPoint totalVaHoursExportedDataPoint;
        totalVaHoursExportedDataPoint.setName("TotVAhExp");
        totalVaHoursExportedDataPoint.setLabel("Total VA-hours Exported");
        totalVaHoursExportedDataPoint.setDescription("Total Apparent Energy Exported");
        totalVaHoursExportedDataPoint.setUnits("VAh");
        totalVaHoursExportedDataPoint.setSize(2);
        totalVaHoursExportedDataPoint.setAddressOffset(55);
        totalVaHoursExportedDataPoint.setBlockOffset(53);
        totalVaHoursExportedDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursExportedDataPoint.name(), totalVaHoursExportedDataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseADataPoint;
        totalVaHoursExportedPhaseADataPoint.setName("TotVAhExpPhA");
        totalVaHoursExportedPhaseADataPoint.setLabel("Total VA-hours Exported phase A");
        totalVaHoursExportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseADataPoint.setSize(2);
        totalVaHoursExportedPhaseADataPoint.setAddressOffset(57);
        totalVaHoursExportedPhaseADataPoint.setBlockOffset(55);
        totalVaHoursExportedPhaseADataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseADataPoint.name(), totalVaHoursExportedPhaseADataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseBDataPoint;
        totalVaHoursExportedPhaseBDataPoint.setName("TotVAhExpPhB");
        totalVaHoursExportedPhaseBDataPoint.setLabel("Total VA-hours Exported phase B");
        totalVaHoursExportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseBDataPoint.setSize(2);
        totalVaHoursExportedPhaseBDataPoint.setAddressOffset(59);
        totalVaHoursExportedPhaseBDataPoint.setBlockOffset(57);
        totalVaHoursExportedPhaseBDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseBDataPoint.name(), totalVaHoursExportedPhaseBDataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseCDataPoint;
        totalVaHoursExportedPhaseCDataPoint.setName("TotVAhExpPhC");
        totalVaHoursExportedPhaseCDataPoint.setLabel("Total VA-hours Exported phase C");
        totalVaHoursExportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseCDataPoint.setSize(2);
        totalVaHoursExportedPhaseCDataPoint.setAddressOffset(61);
        totalVaHoursExportedPhaseCDataPoint.setBlockOffset(59);
        totalVaHoursExportedPhaseCDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseCDataPoint.name(), totalVaHoursExportedPhaseCDataPoint);

        SunSpecDataPoint totalVaHoursImportedDataPoint;
        totalVaHoursImportedDataPoint.setName("TotVAhImp");
        totalVaHoursImportedDataPoint.setLabel("Total VA-hours Imported");
        totalVaHoursImportedDataPoint.setDescription("Total Apparent Energy Imported");
        totalVaHoursImportedDataPoint.setUnits("VAh");
        totalVaHoursImportedDataPoint.setSize(2);
        totalVaHoursImportedDataPoint.setAddressOffset(63);
        totalVaHoursImportedDataPoint.setBlockOffset(61);
        totalVaHoursImportedDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursImportedDataPoint.name(), totalVaHoursImportedDataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseADataPoint;
        totalVaHoursImportedPhaseADataPoint.setName("TotVAhImpPhA");
        totalVaHoursImportedPhaseADataPoint.setLabel("Total VA-hours Imported phase A");
        totalVaHoursImportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseADataPoint.setSize(2);
        totalVaHoursImportedPhaseADataPoint.setAddressOffset(65);
        totalVaHoursImportedPhaseADataPoint.setBlockOffset(63);
        totalVaHoursImportedPhaseADataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseADataPoint.name(), totalVaHoursImportedPhaseADataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseBDataPoint;
        totalVaHoursImportedPhaseBDataPoint.setName("TotVAhImpPhB");
        totalVaHoursImportedPhaseBDataPoint.setLabel("Total VA-hours Imported phase B");
        totalVaHoursImportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseBDataPoint.setSize(2);
        totalVaHoursImportedPhaseBDataPoint.setAddressOffset(67);
        totalVaHoursImportedPhaseBDataPoint.setBlockOffset(65);
        totalVaHoursImportedPhaseBDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseBDataPoint.name(), totalVaHoursImportedPhaseBDataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseCDataPoint;
        totalVaHoursImportedPhaseCDataPoint.setName("TotVAhImpPhC");
        totalVaHoursImportedPhaseCDataPoint.setLabel("Total VA-hours Imported phase C");
        totalVaHoursImportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseCDataPoint.setSize(2);
        totalVaHoursImportedPhaseCDataPoint.setAddressOffset(69);
        totalVaHoursImportedPhaseCDataPoint.setBlockOffset(67);
        totalVaHoursImportedPhaseCDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseCDataPoint.name(), totalVaHoursImportedPhaseCDataPoint);

        SunSpecDataPoint TotVAh_SFDataPoint;
        TotVAh_SFDataPoint.setName("TotVAh_SF");
        TotVAh_SFDataPoint.setDescription("Apparent Energy scale factor");
        TotVAh_SFDataPoint.setSize(1);
        TotVAh_SFDataPoint.setAddressOffset(71);
        TotVAh_SFDataPoint.setBlockOffset(69);
        TotVAh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(TotVAh_SFDataPoint.name(), TotVAh_SFDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1DataPoint;
        totalVarHoursImportedQ1DataPoint.setName("TotVArhImpQ1");
        totalVarHoursImportedQ1DataPoint.setLabel("Total VAR-hours Imported Q1");
        totalVarHoursImportedQ1DataPoint.setDescription("Total Reactive Energy Imported Quadrant 1");
        totalVarHoursImportedQ1DataPoint.setUnits("varh");
        totalVarHoursImportedQ1DataPoint.setSize(2);
        totalVarHoursImportedQ1DataPoint.setAddressOffset(72);
        totalVarHoursImportedQ1DataPoint.setBlockOffset(70);
        totalVarHoursImportedQ1DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ1DataPoint.name(), totalVarHoursImportedQ1DataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseADataPoint;
        totalVarHoursImportedQ1PhaseADataPoint.setName("TotVArhImpQ1PhA");
        totalVarHoursImportedQ1PhaseADataPoint.setLabel("Total VAr-hours Imported Q1 phase A");
        totalVarHoursImportedQ1PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseADataPoint.setAddressOffset(74);
        totalVarHoursImportedQ1PhaseADataPoint.setBlockOffset(72);
        totalVarHoursImportedQ1PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseADataPoint.name(), totalVarHoursImportedQ1PhaseADataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseBDataPoint;
        totalVarHoursImportedQ1PhaseBDataPoint.setName("TotVArhImpQ1PhB");
        totalVarHoursImportedQ1PhaseBDataPoint.setLabel("Total VAr-hours Imported Q1 phase B");
        totalVarHoursImportedQ1PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseBDataPoint.setAddressOffset(76);
        totalVarHoursImportedQ1PhaseBDataPoint.setBlockOffset(74);
        totalVarHoursImportedQ1PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseBDataPoint.name(), totalVarHoursImportedQ1PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseCDataPoint;
        totalVarHoursImportedQ1PhaseCDataPoint.setName("TotVArhImpQ1PhC");
        totalVarHoursImportedQ1PhaseCDataPoint.setLabel("Total VAr-hours Imported Q1 phase C");
        totalVarHoursImportedQ1PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseCDataPoint.setAddressOffset(78);
        totalVarHoursImportedQ1PhaseCDataPoint.setBlockOffset(76);
        totalVarHoursImportedQ1PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseCDataPoint.name(), totalVarHoursImportedQ1PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2DataPoint;
        totalVarHoursImportedQ2DataPoint.setName("TotVArhImpQ2");
        totalVarHoursImportedQ2DataPoint.setLabel("Total VAr-hours Imported Q2");
        totalVarHoursImportedQ2DataPoint.setDescription("Total Reactive Power Imported Quadrant 2");
        totalVarHoursImportedQ2DataPoint.setUnits("varh");
        totalVarHoursImportedQ2DataPoint.setSize(2);
        totalVarHoursImportedQ2DataPoint.setAddressOffset(80);
        totalVarHoursImportedQ2DataPoint.setBlockOffset(78);
        totalVarHoursImportedQ2DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ2DataPoint.name(), totalVarHoursImportedQ2DataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseADataPoint;
        totalVarHoursImportedQ2PhaseADataPoint.setName("TotVArhImpQ2PhA");
        totalVarHoursImportedQ2PhaseADataPoint.setLabel("Total VAr-hours Imported Q2 phase A");
        totalVarHoursImportedQ2PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseADataPoint.setAddressOffset(82);
        totalVarHoursImportedQ2PhaseADataPoint.setBlockOffset(80);
        totalVarHoursImportedQ2PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseADataPoint.name(), totalVarHoursImportedQ2PhaseADataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseBDataPoint;
        totalVarHoursImportedQ2PhaseBDataPoint.setName("TotVArhImpQ2PhB");
        totalVarHoursImportedQ2PhaseBDataPoint.setLabel("Total VAr-hours Imported Q2 phase B");
        totalVarHoursImportedQ2PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseBDataPoint.setAddressOffset(84);
        totalVarHoursImportedQ2PhaseBDataPoint.setBlockOffset(82);
        totalVarHoursImportedQ2PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseBDataPoint.name(), totalVarHoursImportedQ2PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseCDataPoint;
        totalVarHoursImportedQ2PhaseCDataPoint.setName("TotVArhImpQ2PhC");
        totalVarHoursImportedQ2PhaseCDataPoint.setLabel("Total VAr-hours Imported Q2 phase C");
        totalVarHoursImportedQ2PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseCDataPoint.setAddressOffset(86);
        totalVarHoursImportedQ2PhaseCDataPoint.setBlockOffset(84);
        totalVarHoursImportedQ2PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseCDataPoint.name(), totalVarHoursImportedQ2PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3DataPoint;
        totalVarHoursExportedQ3DataPoint.setName("TotVArhExpQ3");
        totalVarHoursExportedQ3DataPoint.setLabel("Total VAr-hours Exported Q3");
        totalVarHoursExportedQ3DataPoint.setDescription("Total Reactive Power Exported Quadrant 3");
        totalVarHoursExportedQ3DataPoint.setUnits("varh");
        totalVarHoursExportedQ3DataPoint.setSize(2);
        totalVarHoursExportedQ3DataPoint.setAddressOffset(88);
        totalVarHoursExportedQ3DataPoint.setBlockOffset(86);
        totalVarHoursExportedQ3DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ3DataPoint.name(), totalVarHoursExportedQ3DataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseADataPoint;
        totalVarHoursExportedQ3PhaseADataPoint.setName("TotVArhExpQ3PhA");
        totalVarHoursExportedQ3PhaseADataPoint.setLabel("Total VAr-hours Exported Q3 phase A");
        totalVarHoursExportedQ3PhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseADataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseADataPoint.setAddressOffset(90);
        totalVarHoursExportedQ3PhaseADataPoint.setBlockOffset(88);
        totalVarHoursExportedQ3PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseADataPoint.name(), totalVarHoursExportedQ3PhaseADataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseBDataPoint;
        totalVarHoursExportedQ3PhaseBDataPoint.setName("TotVArhExpQ3PhB");
        totalVarHoursExportedQ3PhaseBDataPoint.setLabel("Total VAr-hours Exported Q3 phase B");
        totalVarHoursExportedQ3PhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseBDataPoint.setAddressOffset(92);
        totalVarHoursExportedQ3PhaseBDataPoint.setBlockOffset(90);
        totalVarHoursExportedQ3PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseBDataPoint.name(), totalVarHoursExportedQ3PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseCDataPoint;
        totalVarHoursExportedQ3PhaseCDataPoint.setName("TotVArhExpQ3PhC");
        totalVarHoursExportedQ3PhaseCDataPoint.setLabel("Total VAr-hours Exported Q3 phase C");
        totalVarHoursExportedQ3PhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseCDataPoint.setAddressOffset(94);
        totalVarHoursExportedQ3PhaseCDataPoint.setBlockOffset(92);
        totalVarHoursExportedQ3PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseCDataPoint.name(), totalVarHoursExportedQ3PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4DataPoint;
        totalVarHoursExportedQ4DataPoint.setName("TotVArhExpQ4");
        totalVarHoursExportedQ4DataPoint.setLabel("Total VAr-hours Exported Q4");
        totalVarHoursExportedQ4DataPoint.setDescription("Total Reactive Power Exported Quadrant 4");
        totalVarHoursExportedQ4DataPoint.setUnits("varh");
        totalVarHoursExportedQ4DataPoint.setSize(2);
        totalVarHoursExportedQ4DataPoint.setAddressOffset(96);
        totalVarHoursExportedQ4DataPoint.setBlockOffset(94);
        totalVarHoursExportedQ4DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ4DataPoint.name(), totalVarHoursExportedQ4DataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseADataPoint;
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setName("TotVArhExpQ4PhA");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase A");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setAddressOffset(98);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setBlockOffset(96);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseADataPoint.name(), totalVarHoursExportedQ4ImportedPhaseADataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseBDataPoint;
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setName("TotVArhExpQ4PhB");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase B");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setAddressOffset(100);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setBlockOffset(98);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseBDataPoint.name(), totalVarHoursExportedQ4ImportedPhaseBDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseCDataPoint;
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setName("TotVArhExpQ4PhC");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase C");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setAddressOffset(102);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setBlockOffset(100);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseCDataPoint.name(), totalVarHoursExportedQ4ImportedPhaseCDataPoint);

        SunSpecDataPoint TotVArh_SFDataPoint;
        TotVArh_SFDataPoint.setName("TotVArh_SF");
        TotVArh_SFDataPoint.setDescription("Reactive Energy scale factor");
        TotVArh_SFDataPoint.setSize(1);
        TotVArh_SFDataPoint.setAddressOffset(104);
        TotVArh_SFDataPoint.setBlockOffset(102);
        TotVArh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(TotVArh_SFDataPoint.name(), TotVArh_SFDataPoint);

        SunSpecDataPoint eventsDataPoint;
        eventsDataPoint.setName("Evt");
        eventsDataPoint.setLabel("Events");
        eventsDataPoint.setDescription("Meter Event Flags");
        eventsDataPoint.setMandatory(true);
        eventsDataPoint.setSize(2);
        eventsDataPoint.setAddressOffset(105);
        eventsDataPoint.setBlockOffset(103);
        eventsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(eventsDataPoint.name(), eventsDataPoint);

        break;
    }
    case 204: {
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
        ampsDataPoint.setDescription("Total AC Current");
        ampsDataPoint.setUnits("A");
        ampsDataPoint.setMandatory(true);
        ampsDataPoint.setSize(1);
        ampsDataPoint.setAddressOffset(2);
        ampsDataPoint.setBlockOffset(0);
        ampsDataPoint.setScaleFactorName("A_SF");
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
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
        ampsPhaseaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
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
        ampsPhasebDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
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
        ampsPhasecDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(ampsPhasecDataPoint.name(), ampsPhasecDataPoint);

        SunSpecDataPoint A_SFDataPoint;
        A_SFDataPoint.setName("A_SF");
        A_SFDataPoint.setDescription("Current scale factor");
        A_SFDataPoint.setMandatory(true);
        A_SFDataPoint.setSize(1);
        A_SFDataPoint.setAddressOffset(6);
        A_SFDataPoint.setBlockOffset(4);
        A_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(A_SFDataPoint.name(), A_SFDataPoint);

        SunSpecDataPoint voltageLnDataPoint;
        voltageLnDataPoint.setName("PhV");
        voltageLnDataPoint.setLabel("Voltage LN");
        voltageLnDataPoint.setDescription("Line to Neutral AC Voltage (average of active phases)");
        voltageLnDataPoint.setUnits("V");
        voltageLnDataPoint.setSize(1);
        voltageLnDataPoint.setAddressOffset(7);
        voltageLnDataPoint.setBlockOffset(5);
        voltageLnDataPoint.setScaleFactorName("V_SF");
        voltageLnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(voltageLnDataPoint.name(), voltageLnDataPoint);

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
        phaseVoltageAnDataPoint.setSize(1);
        phaseVoltageAnDataPoint.setAddressOffset(8);
        phaseVoltageAnDataPoint.setBlockOffset(6);
        phaseVoltageAnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageAnDataPoint.name(), phaseVoltageAnDataPoint);

        SunSpecDataPoint phaseVoltageBnDataPoint;
        phaseVoltageBnDataPoint.setName("PhVphB");
        phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
        phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
        phaseVoltageBnDataPoint.setUnits("V");
        phaseVoltageBnDataPoint.setSize(1);
        phaseVoltageBnDataPoint.setAddressOffset(9);
        phaseVoltageBnDataPoint.setBlockOffset(7);
        phaseVoltageBnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageBnDataPoint.name(), phaseVoltageBnDataPoint);

        SunSpecDataPoint phaseVoltageCnDataPoint;
        phaseVoltageCnDataPoint.setName("PhVphC");
        phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
        phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
        phaseVoltageCnDataPoint.setUnits("V");
        phaseVoltageCnDataPoint.setSize(1);
        phaseVoltageCnDataPoint.setAddressOffset(10);
        phaseVoltageCnDataPoint.setBlockOffset(8);
        phaseVoltageCnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageCnDataPoint.name(), phaseVoltageCnDataPoint);

        SunSpecDataPoint voltageLlDataPoint;
        voltageLlDataPoint.setName("PPV");
        voltageLlDataPoint.setLabel("Voltage LL");
        voltageLlDataPoint.setDescription("Line to Line AC Voltage (average of active phases)");
        voltageLlDataPoint.setUnits("V");
        voltageLlDataPoint.setMandatory(true);
        voltageLlDataPoint.setSize(1);
        voltageLlDataPoint.setAddressOffset(11);
        voltageLlDataPoint.setBlockOffset(9);
        voltageLlDataPoint.setScaleFactorName("V_SF");
        voltageLlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(voltageLlDataPoint.name(), voltageLlDataPoint);

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PhVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setMandatory(true);
        phaseVoltageAbDataPoint.setSize(1);
        phaseVoltageAbDataPoint.setAddressOffset(12);
        phaseVoltageAbDataPoint.setBlockOffset(10);
        phaseVoltageAbDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageAbDataPoint.name(), phaseVoltageAbDataPoint);

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PhVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setMandatory(true);
        phaseVoltageBcDataPoint.setSize(1);
        phaseVoltageBcDataPoint.setAddressOffset(13);
        phaseVoltageBcDataPoint.setBlockOffset(11);
        phaseVoltageBcDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageBcDataPoint.name(), phaseVoltageBcDataPoint);

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PhVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setMandatory(true);
        phaseVoltageCaDataPoint.setSize(1);
        phaseVoltageCaDataPoint.setAddressOffset(14);
        phaseVoltageCaDataPoint.setBlockOffset(12);
        phaseVoltageCaDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(phaseVoltageCaDataPoint.name(), phaseVoltageCaDataPoint);

        SunSpecDataPoint V_SFDataPoint;
        V_SFDataPoint.setName("V_SF");
        V_SFDataPoint.setDescription("Voltage scale factor");
        V_SFDataPoint.setMandatory(true);
        V_SFDataPoint.setSize(1);
        V_SFDataPoint.setAddressOffset(15);
        V_SFDataPoint.setBlockOffset(13);
        V_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(V_SFDataPoint.name(), V_SFDataPoint);

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(1);
        hzDataPoint.setAddressOffset(16);
        hzDataPoint.setBlockOffset(14);
        hzDataPoint.setScaleFactorName("Hz_SF");
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(hzDataPoint.name(), hzDataPoint);

        SunSpecDataPoint Hz_SFDataPoint;
        Hz_SFDataPoint.setName("Hz_SF");
        Hz_SFDataPoint.setDescription("Frequency scale factor");
        Hz_SFDataPoint.setSize(1);
        Hz_SFDataPoint.setAddressOffset(17);
        Hz_SFDataPoint.setBlockOffset(15);
        Hz_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(Hz_SFDataPoint.name(), Hz_SFDataPoint);

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("Total Real Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(1);
        wattsDataPoint.setAddressOffset(18);
        wattsDataPoint.setBlockOffset(16);
        wattsDataPoint.setScaleFactorName("W_SF");
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsDataPoint.name(), wattsDataPoint);

        SunSpecDataPoint wattsPhaseADataPoint;
        wattsPhaseADataPoint.setName("WphA");
        wattsPhaseADataPoint.setLabel("Watts phase A");
        wattsPhaseADataPoint.setUnits("W");
        wattsPhaseADataPoint.setSize(1);
        wattsPhaseADataPoint.setAddressOffset(19);
        wattsPhaseADataPoint.setBlockOffset(17);
        wattsPhaseADataPoint.setScaleFactorName("W_SF");
        wattsPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsPhaseADataPoint.name(), wattsPhaseADataPoint);

        SunSpecDataPoint wattsPhaseBDataPoint;
        wattsPhaseBDataPoint.setName("WphB");
        wattsPhaseBDataPoint.setLabel("Watts phase B");
        wattsPhaseBDataPoint.setUnits("W");
        wattsPhaseBDataPoint.setSize(1);
        wattsPhaseBDataPoint.setAddressOffset(20);
        wattsPhaseBDataPoint.setBlockOffset(18);
        wattsPhaseBDataPoint.setScaleFactorName("W_SF");
        wattsPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsPhaseBDataPoint.name(), wattsPhaseBDataPoint);

        SunSpecDataPoint wattsPhaseCDataPoint;
        wattsPhaseCDataPoint.setName("WphC");
        wattsPhaseCDataPoint.setLabel("Watts phase C");
        wattsPhaseCDataPoint.setUnits("W");
        wattsPhaseCDataPoint.setSize(1);
        wattsPhaseCDataPoint.setAddressOffset(21);
        wattsPhaseCDataPoint.setBlockOffset(19);
        wattsPhaseCDataPoint.setScaleFactorName("W_SF");
        wattsPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsPhaseCDataPoint.name(), wattsPhaseCDataPoint);

        SunSpecDataPoint W_SFDataPoint;
        W_SFDataPoint.setName("W_SF");
        W_SFDataPoint.setDescription("Real Power scale factor");
        W_SFDataPoint.setMandatory(true);
        W_SFDataPoint.setSize(1);
        W_SFDataPoint.setAddressOffset(22);
        W_SFDataPoint.setBlockOffset(20);
        W_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(W_SFDataPoint.name(), W_SFDataPoint);

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(1);
        vaDataPoint.setAddressOffset(23);
        vaDataPoint.setBlockOffset(21);
        vaDataPoint.setScaleFactorName("VA_SF");
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(vaDataPoint.name(), vaDataPoint);

        SunSpecDataPoint vaPhaseADataPoint;
        vaPhaseADataPoint.setName("VAphA");
        vaPhaseADataPoint.setLabel("VA phase A");
        vaPhaseADataPoint.setUnits("VA");
        vaPhaseADataPoint.setSize(1);
        vaPhaseADataPoint.setAddressOffset(24);
        vaPhaseADataPoint.setBlockOffset(22);
        vaPhaseADataPoint.setScaleFactorName("VA_SF");
        vaPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(vaPhaseADataPoint.name(), vaPhaseADataPoint);

        SunSpecDataPoint vaPhaseBDataPoint;
        vaPhaseBDataPoint.setName("VAphB");
        vaPhaseBDataPoint.setLabel("VA phase B");
        vaPhaseBDataPoint.setUnits("VA");
        vaPhaseBDataPoint.setSize(1);
        vaPhaseBDataPoint.setAddressOffset(25);
        vaPhaseBDataPoint.setBlockOffset(23);
        vaPhaseBDataPoint.setScaleFactorName("VA_SF");
        vaPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(vaPhaseBDataPoint.name(), vaPhaseBDataPoint);

        SunSpecDataPoint vaPhaseCDataPoint;
        vaPhaseCDataPoint.setName("VAphC");
        vaPhaseCDataPoint.setLabel("VA phase C");
        vaPhaseCDataPoint.setUnits("VA");
        vaPhaseCDataPoint.setSize(1);
        vaPhaseCDataPoint.setAddressOffset(26);
        vaPhaseCDataPoint.setBlockOffset(24);
        vaPhaseCDataPoint.setScaleFactorName("VA_SF");
        vaPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(vaPhaseCDataPoint.name(), vaPhaseCDataPoint);

        SunSpecDataPoint VA_SFDataPoint;
        VA_SFDataPoint.setName("VA_SF");
        VA_SFDataPoint.setDescription("Apparent Power scale factor");
        VA_SFDataPoint.setSize(1);
        VA_SFDataPoint.setAddressOffset(27);
        VA_SFDataPoint.setBlockOffset(25);
        VA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(VA_SFDataPoint.name(), VA_SFDataPoint);

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAR");
        varDataPoint.setLabel("VAR");
        varDataPoint.setDescription("Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(1);
        varDataPoint.setAddressOffset(28);
        varDataPoint.setBlockOffset(26);
        varDataPoint.setScaleFactorName("VAR_SF");
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(varDataPoint.name(), varDataPoint);

        SunSpecDataPoint varPhaseADataPoint;
        varPhaseADataPoint.setName("VARphA");
        varPhaseADataPoint.setLabel("VAR phase A");
        varPhaseADataPoint.setUnits("var");
        varPhaseADataPoint.setSize(1);
        varPhaseADataPoint.setAddressOffset(29);
        varPhaseADataPoint.setBlockOffset(27);
        varPhaseADataPoint.setScaleFactorName("VAR_SF");
        varPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(varPhaseADataPoint.name(), varPhaseADataPoint);

        SunSpecDataPoint varPhaseBDataPoint;
        varPhaseBDataPoint.setName("VARphB");
        varPhaseBDataPoint.setLabel("VAR phase B");
        varPhaseBDataPoint.setUnits("var");
        varPhaseBDataPoint.setSize(1);
        varPhaseBDataPoint.setAddressOffset(30);
        varPhaseBDataPoint.setBlockOffset(28);
        varPhaseBDataPoint.setScaleFactorName("VAR_SF");
        varPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(varPhaseBDataPoint.name(), varPhaseBDataPoint);

        SunSpecDataPoint varPhaseCDataPoint;
        varPhaseCDataPoint.setName("VARphC");
        varPhaseCDataPoint.setLabel("VAR phase C");
        varPhaseCDataPoint.setUnits("var");
        varPhaseCDataPoint.setSize(1);
        varPhaseCDataPoint.setAddressOffset(31);
        varPhaseCDataPoint.setBlockOffset(29);
        varPhaseCDataPoint.setScaleFactorName("VAR_SF");
        varPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(varPhaseCDataPoint.name(), varPhaseCDataPoint);

        SunSpecDataPoint VAR_SFDataPoint;
        VAR_SFDataPoint.setName("VAR_SF");
        VAR_SFDataPoint.setDescription("Reactive Power scale factor");
        VAR_SFDataPoint.setSize(1);
        VAR_SFDataPoint.setAddressOffset(32);
        VAR_SFDataPoint.setBlockOffset(30);
        VAR_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(VAR_SFDataPoint.name(), VAR_SFDataPoint);

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("Power Factor");
        pfDataPoint.setUnits("Pct");
        pfDataPoint.setSize(1);
        pfDataPoint.setAddressOffset(33);
        pfDataPoint.setBlockOffset(31);
        pfDataPoint.setScaleFactorName("PF_SF");
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(pfDataPoint.name(), pfDataPoint);

        SunSpecDataPoint pfPhaseADataPoint;
        pfPhaseADataPoint.setName("PFphA");
        pfPhaseADataPoint.setLabel("PF phase A");
        pfPhaseADataPoint.setUnits("Pct");
        pfPhaseADataPoint.setSize(1);
        pfPhaseADataPoint.setAddressOffset(34);
        pfPhaseADataPoint.setBlockOffset(32);
        pfPhaseADataPoint.setScaleFactorName("PF_SF");
        pfPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(pfPhaseADataPoint.name(), pfPhaseADataPoint);

        SunSpecDataPoint pfPhaseBDataPoint;
        pfPhaseBDataPoint.setName("PFphB");
        pfPhaseBDataPoint.setLabel("PF phase B");
        pfPhaseBDataPoint.setUnits("Pct");
        pfPhaseBDataPoint.setSize(1);
        pfPhaseBDataPoint.setAddressOffset(35);
        pfPhaseBDataPoint.setBlockOffset(33);
        pfPhaseBDataPoint.setScaleFactorName("PF_SF");
        pfPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(pfPhaseBDataPoint.name(), pfPhaseBDataPoint);

        SunSpecDataPoint pfPhaseCDataPoint;
        pfPhaseCDataPoint.setName("PFphC");
        pfPhaseCDataPoint.setLabel("PF phase C");
        pfPhaseCDataPoint.setUnits("Pct");
        pfPhaseCDataPoint.setSize(1);
        pfPhaseCDataPoint.setAddressOffset(36);
        pfPhaseCDataPoint.setBlockOffset(34);
        pfPhaseCDataPoint.setScaleFactorName("PF_SF");
        pfPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(pfPhaseCDataPoint.name(), pfPhaseCDataPoint);

        SunSpecDataPoint PF_SFDataPoint;
        PF_SFDataPoint.setName("PF_SF");
        PF_SFDataPoint.setDescription("Power Factor scale factor");
        PF_SFDataPoint.setSize(1);
        PF_SFDataPoint.setAddressOffset(37);
        PF_SFDataPoint.setBlockOffset(35);
        PF_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(PF_SFDataPoint.name(), PF_SFDataPoint);

        SunSpecDataPoint totalWattHoursExportedDataPoint;
        totalWattHoursExportedDataPoint.setName("TotWhExp");
        totalWattHoursExportedDataPoint.setLabel("Total Watt-hours Exported");
        totalWattHoursExportedDataPoint.setDescription("Total Real Energy Exported");
        totalWattHoursExportedDataPoint.setUnits("Wh");
        totalWattHoursExportedDataPoint.setMandatory(true);
        totalWattHoursExportedDataPoint.setSize(2);
        totalWattHoursExportedDataPoint.setAddressOffset(38);
        totalWattHoursExportedDataPoint.setBlockOffset(36);
        totalWattHoursExportedDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursExportedDataPoint.name(), totalWattHoursExportedDataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseADataPoint;
        totalWattHoursExportedPhaseADataPoint.setName("TotWhExpPhA");
        totalWattHoursExportedPhaseADataPoint.setLabel("Total Watt-hours Exported phase A");
        totalWattHoursExportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseADataPoint.setSize(2);
        totalWattHoursExportedPhaseADataPoint.setAddressOffset(40);
        totalWattHoursExportedPhaseADataPoint.setBlockOffset(38);
        totalWattHoursExportedPhaseADataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursExportedPhaseADataPoint.name(), totalWattHoursExportedPhaseADataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseBDataPoint;
        totalWattHoursExportedPhaseBDataPoint.setName("TotWhExpPhB");
        totalWattHoursExportedPhaseBDataPoint.setLabel("Total Watt-hours Exported phase B");
        totalWattHoursExportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseBDataPoint.setSize(2);
        totalWattHoursExportedPhaseBDataPoint.setAddressOffset(42);
        totalWattHoursExportedPhaseBDataPoint.setBlockOffset(40);
        totalWattHoursExportedPhaseBDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursExportedPhaseBDataPoint.name(), totalWattHoursExportedPhaseBDataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseCDataPoint;
        totalWattHoursExportedPhaseCDataPoint.setName("TotWhExpPhC");
        totalWattHoursExportedPhaseCDataPoint.setLabel("Total Watt-hours Exported phase C");
        totalWattHoursExportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseCDataPoint.setSize(2);
        totalWattHoursExportedPhaseCDataPoint.setAddressOffset(44);
        totalWattHoursExportedPhaseCDataPoint.setBlockOffset(42);
        totalWattHoursExportedPhaseCDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursExportedPhaseCDataPoint.name(), totalWattHoursExportedPhaseCDataPoint);

        SunSpecDataPoint totalWattHoursImportedDataPoint;
        totalWattHoursImportedDataPoint.setName("TotWhImp");
        totalWattHoursImportedDataPoint.setLabel("Total Watt-hours Imported");
        totalWattHoursImportedDataPoint.setDescription("Total Real Energy Imported");
        totalWattHoursImportedDataPoint.setUnits("Wh");
        totalWattHoursImportedDataPoint.setMandatory(true);
        totalWattHoursImportedDataPoint.setSize(2);
        totalWattHoursImportedDataPoint.setAddressOffset(46);
        totalWattHoursImportedDataPoint.setBlockOffset(44);
        totalWattHoursImportedDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursImportedDataPoint.name(), totalWattHoursImportedDataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseADataPoint;
        totalWattHoursImportedPhaseADataPoint.setName("TotWhImpPhA");
        totalWattHoursImportedPhaseADataPoint.setLabel("Total Watt-hours Imported phase A");
        totalWattHoursImportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseADataPoint.setSize(2);
        totalWattHoursImportedPhaseADataPoint.setAddressOffset(48);
        totalWattHoursImportedPhaseADataPoint.setBlockOffset(46);
        totalWattHoursImportedPhaseADataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseADataPoint.name(), totalWattHoursImportedPhaseADataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseBDataPoint;
        totalWattHoursImportedPhaseBDataPoint.setName("TotWhImpPhB");
        totalWattHoursImportedPhaseBDataPoint.setLabel("Total Watt-hours Imported phase B");
        totalWattHoursImportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseBDataPoint.setSize(2);
        totalWattHoursImportedPhaseBDataPoint.setAddressOffset(50);
        totalWattHoursImportedPhaseBDataPoint.setBlockOffset(48);
        totalWattHoursImportedPhaseBDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseBDataPoint.name(), totalWattHoursImportedPhaseBDataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseCDataPoint;
        totalWattHoursImportedPhaseCDataPoint.setName("TotWhImpPhC");
        totalWattHoursImportedPhaseCDataPoint.setLabel("Total Watt-hours Imported phase C");
        totalWattHoursImportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseCDataPoint.setSize(2);
        totalWattHoursImportedPhaseCDataPoint.setAddressOffset(52);
        totalWattHoursImportedPhaseCDataPoint.setBlockOffset(50);
        totalWattHoursImportedPhaseCDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseCDataPoint.name(), totalWattHoursImportedPhaseCDataPoint);

        SunSpecDataPoint TotWh_SFDataPoint;
        TotWh_SFDataPoint.setName("TotWh_SF");
        TotWh_SFDataPoint.setDescription("Real Energy scale factor");
        TotWh_SFDataPoint.setMandatory(true);
        TotWh_SFDataPoint.setSize(1);
        TotWh_SFDataPoint.setAddressOffset(54);
        TotWh_SFDataPoint.setBlockOffset(52);
        TotWh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(TotWh_SFDataPoint.name(), TotWh_SFDataPoint);

        SunSpecDataPoint totalVaHoursExportedDataPoint;
        totalVaHoursExportedDataPoint.setName("TotVAhExp");
        totalVaHoursExportedDataPoint.setLabel("Total VA-hours Exported");
        totalVaHoursExportedDataPoint.setDescription("Total Apparent Energy Exported");
        totalVaHoursExportedDataPoint.setUnits("VAh");
        totalVaHoursExportedDataPoint.setSize(2);
        totalVaHoursExportedDataPoint.setAddressOffset(55);
        totalVaHoursExportedDataPoint.setBlockOffset(53);
        totalVaHoursExportedDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursExportedDataPoint.name(), totalVaHoursExportedDataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseADataPoint;
        totalVaHoursExportedPhaseADataPoint.setName("TotVAhExpPhA");
        totalVaHoursExportedPhaseADataPoint.setLabel("Total VA-hours Exported phase A");
        totalVaHoursExportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseADataPoint.setSize(2);
        totalVaHoursExportedPhaseADataPoint.setAddressOffset(57);
        totalVaHoursExportedPhaseADataPoint.setBlockOffset(55);
        totalVaHoursExportedPhaseADataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseADataPoint.name(), totalVaHoursExportedPhaseADataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseBDataPoint;
        totalVaHoursExportedPhaseBDataPoint.setName("TotVAhExpPhB");
        totalVaHoursExportedPhaseBDataPoint.setLabel("Total VA-hours Exported phase B");
        totalVaHoursExportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseBDataPoint.setSize(2);
        totalVaHoursExportedPhaseBDataPoint.setAddressOffset(59);
        totalVaHoursExportedPhaseBDataPoint.setBlockOffset(57);
        totalVaHoursExportedPhaseBDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseBDataPoint.name(), totalVaHoursExportedPhaseBDataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseCDataPoint;
        totalVaHoursExportedPhaseCDataPoint.setName("TotVAhExpPhC");
        totalVaHoursExportedPhaseCDataPoint.setLabel("Total VA-hours Exported phase C");
        totalVaHoursExportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseCDataPoint.setSize(2);
        totalVaHoursExportedPhaseCDataPoint.setAddressOffset(61);
        totalVaHoursExportedPhaseCDataPoint.setBlockOffset(59);
        totalVaHoursExportedPhaseCDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseCDataPoint.name(), totalVaHoursExportedPhaseCDataPoint);

        SunSpecDataPoint totalVaHoursImportedDataPoint;
        totalVaHoursImportedDataPoint.setName("TotVAhImp");
        totalVaHoursImportedDataPoint.setLabel("Total VA-hours Imported");
        totalVaHoursImportedDataPoint.setDescription("Total Apparent Energy Imported");
        totalVaHoursImportedDataPoint.setUnits("VAh");
        totalVaHoursImportedDataPoint.setSize(2);
        totalVaHoursImportedDataPoint.setAddressOffset(63);
        totalVaHoursImportedDataPoint.setBlockOffset(61);
        totalVaHoursImportedDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursImportedDataPoint.name(), totalVaHoursImportedDataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseADataPoint;
        totalVaHoursImportedPhaseADataPoint.setName("TotVAhImpPhA");
        totalVaHoursImportedPhaseADataPoint.setLabel("Total VA-hours Imported phase A");
        totalVaHoursImportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseADataPoint.setSize(2);
        totalVaHoursImportedPhaseADataPoint.setAddressOffset(65);
        totalVaHoursImportedPhaseADataPoint.setBlockOffset(63);
        totalVaHoursImportedPhaseADataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseADataPoint.name(), totalVaHoursImportedPhaseADataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseBDataPoint;
        totalVaHoursImportedPhaseBDataPoint.setName("TotVAhImpPhB");
        totalVaHoursImportedPhaseBDataPoint.setLabel("Total VA-hours Imported phase B");
        totalVaHoursImportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseBDataPoint.setSize(2);
        totalVaHoursImportedPhaseBDataPoint.setAddressOffset(67);
        totalVaHoursImportedPhaseBDataPoint.setBlockOffset(65);
        totalVaHoursImportedPhaseBDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseBDataPoint.name(), totalVaHoursImportedPhaseBDataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseCDataPoint;
        totalVaHoursImportedPhaseCDataPoint.setName("TotVAhImpPhC");
        totalVaHoursImportedPhaseCDataPoint.setLabel("Total VA-hours Imported phase C");
        totalVaHoursImportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseCDataPoint.setSize(2);
        totalVaHoursImportedPhaseCDataPoint.setAddressOffset(69);
        totalVaHoursImportedPhaseCDataPoint.setBlockOffset(67);
        totalVaHoursImportedPhaseCDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseCDataPoint.name(), totalVaHoursImportedPhaseCDataPoint);

        SunSpecDataPoint TotVAh_SFDataPoint;
        TotVAh_SFDataPoint.setName("TotVAh_SF");
        TotVAh_SFDataPoint.setDescription("Apparent Energy scale factor");
        TotVAh_SFDataPoint.setSize(1);
        TotVAh_SFDataPoint.setAddressOffset(71);
        TotVAh_SFDataPoint.setBlockOffset(69);
        TotVAh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(TotVAh_SFDataPoint.name(), TotVAh_SFDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1DataPoint;
        totalVarHoursImportedQ1DataPoint.setName("TotVArhImpQ1");
        totalVarHoursImportedQ1DataPoint.setLabel("Total VAR-hours Imported Q1");
        totalVarHoursImportedQ1DataPoint.setDescription("Total Reactive Energy Imported Quadrant 1");
        totalVarHoursImportedQ1DataPoint.setUnits("varh");
        totalVarHoursImportedQ1DataPoint.setSize(2);
        totalVarHoursImportedQ1DataPoint.setAddressOffset(72);
        totalVarHoursImportedQ1DataPoint.setBlockOffset(70);
        totalVarHoursImportedQ1DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ1DataPoint.name(), totalVarHoursImportedQ1DataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseADataPoint;
        totalVarHoursImportedQ1PhaseADataPoint.setName("TotVArhImpQ1PhA");
        totalVarHoursImportedQ1PhaseADataPoint.setLabel("Total VAr-hours Imported Q1 phase A");
        totalVarHoursImportedQ1PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseADataPoint.setAddressOffset(74);
        totalVarHoursImportedQ1PhaseADataPoint.setBlockOffset(72);
        totalVarHoursImportedQ1PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseADataPoint.name(), totalVarHoursImportedQ1PhaseADataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseBDataPoint;
        totalVarHoursImportedQ1PhaseBDataPoint.setName("TotVArhImpQ1PhB");
        totalVarHoursImportedQ1PhaseBDataPoint.setLabel("Total VAr-hours Imported Q1 phase B");
        totalVarHoursImportedQ1PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseBDataPoint.setAddressOffset(76);
        totalVarHoursImportedQ1PhaseBDataPoint.setBlockOffset(74);
        totalVarHoursImportedQ1PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseBDataPoint.name(), totalVarHoursImportedQ1PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseCDataPoint;
        totalVarHoursImportedQ1PhaseCDataPoint.setName("TotVArhImpQ1PhC");
        totalVarHoursImportedQ1PhaseCDataPoint.setLabel("Total VAr-hours Imported Q1 phase C");
        totalVarHoursImportedQ1PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseCDataPoint.setAddressOffset(78);
        totalVarHoursImportedQ1PhaseCDataPoint.setBlockOffset(76);
        totalVarHoursImportedQ1PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseCDataPoint.name(), totalVarHoursImportedQ1PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2DataPoint;
        totalVarHoursImportedQ2DataPoint.setName("TotVArhImpQ2");
        totalVarHoursImportedQ2DataPoint.setLabel("Total VAr-hours Imported Q2");
        totalVarHoursImportedQ2DataPoint.setDescription("Total Reactive Power Imported Quadrant 2");
        totalVarHoursImportedQ2DataPoint.setUnits("varh");
        totalVarHoursImportedQ2DataPoint.setSize(2);
        totalVarHoursImportedQ2DataPoint.setAddressOffset(80);
        totalVarHoursImportedQ2DataPoint.setBlockOffset(78);
        totalVarHoursImportedQ2DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ2DataPoint.name(), totalVarHoursImportedQ2DataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseADataPoint;
        totalVarHoursImportedQ2PhaseADataPoint.setName("TotVArhImpQ2PhA");
        totalVarHoursImportedQ2PhaseADataPoint.setLabel("Total VAr-hours Imported Q2 phase A");
        totalVarHoursImportedQ2PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseADataPoint.setAddressOffset(82);
        totalVarHoursImportedQ2PhaseADataPoint.setBlockOffset(80);
        totalVarHoursImportedQ2PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseADataPoint.name(), totalVarHoursImportedQ2PhaseADataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseBDataPoint;
        totalVarHoursImportedQ2PhaseBDataPoint.setName("TotVArhImpQ2PhB");
        totalVarHoursImportedQ2PhaseBDataPoint.setLabel("Total VAr-hours Imported Q2 phase B");
        totalVarHoursImportedQ2PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseBDataPoint.setAddressOffset(84);
        totalVarHoursImportedQ2PhaseBDataPoint.setBlockOffset(82);
        totalVarHoursImportedQ2PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseBDataPoint.name(), totalVarHoursImportedQ2PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseCDataPoint;
        totalVarHoursImportedQ2PhaseCDataPoint.setName("TotVArhImpQ2PhC");
        totalVarHoursImportedQ2PhaseCDataPoint.setLabel("Total VAr-hours Imported Q2 phase C");
        totalVarHoursImportedQ2PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseCDataPoint.setAddressOffset(86);
        totalVarHoursImportedQ2PhaseCDataPoint.setBlockOffset(84);
        totalVarHoursImportedQ2PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseCDataPoint.name(), totalVarHoursImportedQ2PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3DataPoint;
        totalVarHoursExportedQ3DataPoint.setName("TotVArhExpQ3");
        totalVarHoursExportedQ3DataPoint.setLabel("Total VAr-hours Exported Q3");
        totalVarHoursExportedQ3DataPoint.setDescription("Total Reactive Power Exported Quadrant 3");
        totalVarHoursExportedQ3DataPoint.setUnits("varh");
        totalVarHoursExportedQ3DataPoint.setSize(2);
        totalVarHoursExportedQ3DataPoint.setAddressOffset(88);
        totalVarHoursExportedQ3DataPoint.setBlockOffset(86);
        totalVarHoursExportedQ3DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ3DataPoint.name(), totalVarHoursExportedQ3DataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseADataPoint;
        totalVarHoursExportedQ3PhaseADataPoint.setName("TotVArhExpQ3PhA");
        totalVarHoursExportedQ3PhaseADataPoint.setLabel("Total VAr-hours Exported Q3 phase A");
        totalVarHoursExportedQ3PhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseADataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseADataPoint.setAddressOffset(90);
        totalVarHoursExportedQ3PhaseADataPoint.setBlockOffset(88);
        totalVarHoursExportedQ3PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseADataPoint.name(), totalVarHoursExportedQ3PhaseADataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseBDataPoint;
        totalVarHoursExportedQ3PhaseBDataPoint.setName("TotVArhExpQ3PhB");
        totalVarHoursExportedQ3PhaseBDataPoint.setLabel("Total VAr-hours Exported Q3 phase B");
        totalVarHoursExportedQ3PhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseBDataPoint.setAddressOffset(92);
        totalVarHoursExportedQ3PhaseBDataPoint.setBlockOffset(90);
        totalVarHoursExportedQ3PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseBDataPoint.name(), totalVarHoursExportedQ3PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseCDataPoint;
        totalVarHoursExportedQ3PhaseCDataPoint.setName("TotVArhExpQ3PhC");
        totalVarHoursExportedQ3PhaseCDataPoint.setLabel("Total VAr-hours Exported Q3 phase C");
        totalVarHoursExportedQ3PhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseCDataPoint.setAddressOffset(94);
        totalVarHoursExportedQ3PhaseCDataPoint.setBlockOffset(92);
        totalVarHoursExportedQ3PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseCDataPoint.name(), totalVarHoursExportedQ3PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4DataPoint;
        totalVarHoursExportedQ4DataPoint.setName("TotVArhExpQ4");
        totalVarHoursExportedQ4DataPoint.setLabel("Total VAr-hours Exported Q4");
        totalVarHoursExportedQ4DataPoint.setDescription("Total Reactive Power Exported Quadrant 4");
        totalVarHoursExportedQ4DataPoint.setUnits("varh");
        totalVarHoursExportedQ4DataPoint.setSize(2);
        totalVarHoursExportedQ4DataPoint.setAddressOffset(96);
        totalVarHoursExportedQ4DataPoint.setBlockOffset(94);
        totalVarHoursExportedQ4DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ4DataPoint.name(), totalVarHoursExportedQ4DataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseADataPoint;
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setName("TotVArhExpQ4PhA");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase A");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setAddressOffset(98);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setBlockOffset(96);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseADataPoint.name(), totalVarHoursExportedQ4ImportedPhaseADataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseBDataPoint;
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setName("TotVArhExpQ4PhB");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase B");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setAddressOffset(100);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setBlockOffset(98);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseBDataPoint.name(), totalVarHoursExportedQ4ImportedPhaseBDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseCDataPoint;
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setName("TotVArhExpQ4PhC");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase C");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setAddressOffset(102);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setBlockOffset(100);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseCDataPoint.name(), totalVarHoursExportedQ4ImportedPhaseCDataPoint);

        SunSpecDataPoint TotVArh_SFDataPoint;
        TotVArh_SFDataPoint.setName("TotVArh_SF");
        TotVArh_SFDataPoint.setDescription("Reactive Energy scale factor");
        TotVArh_SFDataPoint.setSize(1);
        TotVArh_SFDataPoint.setAddressOffset(104);
        TotVArh_SFDataPoint.setBlockOffset(102);
        TotVArh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(TotVArh_SFDataPoint.name(), TotVArh_SFDataPoint);

        SunSpecDataPoint eventsDataPoint;
        eventsDataPoint.setName("Evt");
        eventsDataPoint.setLabel("Events");
        eventsDataPoint.setDescription("Meter Event Flags");
        eventsDataPoint.setMandatory(true);
        eventsDataPoint.setSize(2);
        eventsDataPoint.setAddressOffset(105);
        eventsDataPoint.setBlockOffset(103);
        eventsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(eventsDataPoint.name(), eventsDataPoint);

        break;
    }
    case 211: {
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
        ampsDataPoint.setDescription("Total AC Current");
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

        SunSpecDataPoint voltageLnDataPoint;
        voltageLnDataPoint.setName("PhV");
        voltageLnDataPoint.setLabel("Voltage LN");
        voltageLnDataPoint.setDescription("Line to Neutral AC Voltage (average of active phases)");
        voltageLnDataPoint.setUnits("V");
        voltageLnDataPoint.setSize(2);
        voltageLnDataPoint.setAddressOffset(10);
        voltageLnDataPoint.setBlockOffset(8);
        voltageLnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(voltageLnDataPoint.name(), voltageLnDataPoint);

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
        phaseVoltageAnDataPoint.setSize(2);
        phaseVoltageAnDataPoint.setAddressOffset(12);
        phaseVoltageAnDataPoint.setBlockOffset(10);
        phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(phaseVoltageAnDataPoint.name(), phaseVoltageAnDataPoint);

        SunSpecDataPoint phaseVoltageBnDataPoint;
        phaseVoltageBnDataPoint.setName("PhVphB");
        phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
        phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
        phaseVoltageBnDataPoint.setUnits("V");
        phaseVoltageBnDataPoint.setSize(2);
        phaseVoltageBnDataPoint.setAddressOffset(14);
        phaseVoltageBnDataPoint.setBlockOffset(12);
        phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(phaseVoltageBnDataPoint.name(), phaseVoltageBnDataPoint);

        SunSpecDataPoint phaseVoltageCnDataPoint;
        phaseVoltageCnDataPoint.setName("PhVphC");
        phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
        phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
        phaseVoltageCnDataPoint.setUnits("V");
        phaseVoltageCnDataPoint.setSize(2);
        phaseVoltageCnDataPoint.setAddressOffset(16);
        phaseVoltageCnDataPoint.setBlockOffset(14);
        phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(phaseVoltageCnDataPoint.name(), phaseVoltageCnDataPoint);

        SunSpecDataPoint voltageLlDataPoint;
        voltageLlDataPoint.setName("PPV");
        voltageLlDataPoint.setLabel("Voltage LL");
        voltageLlDataPoint.setDescription("Line to Line AC Voltage (average of active phases)");
        voltageLlDataPoint.setUnits("V");
        voltageLlDataPoint.setSize(2);
        voltageLlDataPoint.setAddressOffset(18);
        voltageLlDataPoint.setBlockOffset(16);
        voltageLlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(voltageLlDataPoint.name(), voltageLlDataPoint);

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PPVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setSize(2);
        phaseVoltageAbDataPoint.setAddressOffset(20);
        phaseVoltageAbDataPoint.setBlockOffset(18);
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(phaseVoltageAbDataPoint.name(), phaseVoltageAbDataPoint);

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PPVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setSize(2);
        phaseVoltageBcDataPoint.setAddressOffset(22);
        phaseVoltageBcDataPoint.setBlockOffset(20);
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(phaseVoltageBcDataPoint.name(), phaseVoltageBcDataPoint);

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PPVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setSize(2);
        phaseVoltageCaDataPoint.setAddressOffset(24);
        phaseVoltageCaDataPoint.setBlockOffset(22);
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
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
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
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
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(wattsDataPoint.name(), wattsDataPoint);

        SunSpecDataPoint wattsPhaseADataPoint;
        wattsPhaseADataPoint.setName("WphA");
        wattsPhaseADataPoint.setLabel("Watts phase A");
        wattsPhaseADataPoint.setUnits("W");
        wattsPhaseADataPoint.setSize(2);
        wattsPhaseADataPoint.setAddressOffset(30);
        wattsPhaseADataPoint.setBlockOffset(28);
        wattsPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(wattsPhaseADataPoint.name(), wattsPhaseADataPoint);

        SunSpecDataPoint wattsPhaseBDataPoint;
        wattsPhaseBDataPoint.setName("WphB");
        wattsPhaseBDataPoint.setLabel("Watts phase B");
        wattsPhaseBDataPoint.setUnits("W");
        wattsPhaseBDataPoint.setSize(2);
        wattsPhaseBDataPoint.setAddressOffset(32);
        wattsPhaseBDataPoint.setBlockOffset(30);
        wattsPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(wattsPhaseBDataPoint.name(), wattsPhaseBDataPoint);

        SunSpecDataPoint wattsPhaseCDataPoint;
        wattsPhaseCDataPoint.setName("WphC");
        wattsPhaseCDataPoint.setLabel("Watts phase C");
        wattsPhaseCDataPoint.setUnits("W");
        wattsPhaseCDataPoint.setSize(2);
        wattsPhaseCDataPoint.setAddressOffset(34);
        wattsPhaseCDataPoint.setBlockOffset(32);
        wattsPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(wattsPhaseCDataPoint.name(), wattsPhaseCDataPoint);

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(2);
        vaDataPoint.setAddressOffset(36);
        vaDataPoint.setBlockOffset(34);
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(vaDataPoint.name(), vaDataPoint);

        SunSpecDataPoint vaPhaseADataPoint;
        vaPhaseADataPoint.setName("VAphA");
        vaPhaseADataPoint.setLabel("VA phase A");
        vaPhaseADataPoint.setUnits("VA");
        vaPhaseADataPoint.setSize(2);
        vaPhaseADataPoint.setAddressOffset(38);
        vaPhaseADataPoint.setBlockOffset(36);
        vaPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(vaPhaseADataPoint.name(), vaPhaseADataPoint);

        SunSpecDataPoint vaPhaseBDataPoint;
        vaPhaseBDataPoint.setName("VAphB");
        vaPhaseBDataPoint.setLabel("VA phase B");
        vaPhaseBDataPoint.setUnits("VA");
        vaPhaseBDataPoint.setSize(2);
        vaPhaseBDataPoint.setAddressOffset(40);
        vaPhaseBDataPoint.setBlockOffset(38);
        vaPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(vaPhaseBDataPoint.name(), vaPhaseBDataPoint);

        SunSpecDataPoint vaPhaseCDataPoint;
        vaPhaseCDataPoint.setName("VAphC");
        vaPhaseCDataPoint.setLabel("VA phase C");
        vaPhaseCDataPoint.setUnits("VA");
        vaPhaseCDataPoint.setSize(2);
        vaPhaseCDataPoint.setAddressOffset(42);
        vaPhaseCDataPoint.setBlockOffset(40);
        vaPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(vaPhaseCDataPoint.name(), vaPhaseCDataPoint);

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAR");
        varDataPoint.setLabel("VAR");
        varDataPoint.setDescription("Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(2);
        varDataPoint.setAddressOffset(44);
        varDataPoint.setBlockOffset(42);
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(varDataPoint.name(), varDataPoint);

        SunSpecDataPoint varPhaseADataPoint;
        varPhaseADataPoint.setName("VARphA");
        varPhaseADataPoint.setLabel("VAR phase A");
        varPhaseADataPoint.setUnits("var");
        varPhaseADataPoint.setSize(2);
        varPhaseADataPoint.setAddressOffset(46);
        varPhaseADataPoint.setBlockOffset(44);
        varPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(varPhaseADataPoint.name(), varPhaseADataPoint);

        SunSpecDataPoint varPhaseBDataPoint;
        varPhaseBDataPoint.setName("VARphB");
        varPhaseBDataPoint.setLabel("VAR phase B");
        varPhaseBDataPoint.setUnits("var");
        varPhaseBDataPoint.setSize(2);
        varPhaseBDataPoint.setAddressOffset(48);
        varPhaseBDataPoint.setBlockOffset(46);
        varPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(varPhaseBDataPoint.name(), varPhaseBDataPoint);

        SunSpecDataPoint varPhaseCDataPoint;
        varPhaseCDataPoint.setName("VARphC");
        varPhaseCDataPoint.setLabel("VAR phase C");
        varPhaseCDataPoint.setUnits("var");
        varPhaseCDataPoint.setSize(2);
        varPhaseCDataPoint.setAddressOffset(50);
        varPhaseCDataPoint.setBlockOffset(48);
        varPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(varPhaseCDataPoint.name(), varPhaseCDataPoint);

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("Power Factor");
        pfDataPoint.setUnits("PF");
        pfDataPoint.setSize(2);
        pfDataPoint.setAddressOffset(52);
        pfDataPoint.setBlockOffset(50);
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(pfDataPoint.name(), pfDataPoint);

        SunSpecDataPoint pfPhaseADataPoint;
        pfPhaseADataPoint.setName("PFphA");
        pfPhaseADataPoint.setLabel("PF phase A");
        pfPhaseADataPoint.setUnits("PF");
        pfPhaseADataPoint.setSize(2);
        pfPhaseADataPoint.setAddressOffset(54);
        pfPhaseADataPoint.setBlockOffset(52);
        pfPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(pfPhaseADataPoint.name(), pfPhaseADataPoint);

        SunSpecDataPoint pfPhaseBDataPoint;
        pfPhaseBDataPoint.setName("PFphB");
        pfPhaseBDataPoint.setLabel("PF phase B");
        pfPhaseBDataPoint.setUnits("PF");
        pfPhaseBDataPoint.setSize(2);
        pfPhaseBDataPoint.setAddressOffset(56);
        pfPhaseBDataPoint.setBlockOffset(54);
        pfPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(pfPhaseBDataPoint.name(), pfPhaseBDataPoint);

        SunSpecDataPoint pfPhaseCDataPoint;
        pfPhaseCDataPoint.setName("PFphC");
        pfPhaseCDataPoint.setLabel("PF phase C");
        pfPhaseCDataPoint.setUnits("PF");
        pfPhaseCDataPoint.setSize(2);
        pfPhaseCDataPoint.setAddressOffset(58);
        pfPhaseCDataPoint.setBlockOffset(56);
        pfPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
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
        totalWattHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursExportedDataPoint.name(), totalWattHoursExportedDataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseADataPoint;
        totalWattHoursExportedPhaseADataPoint.setName("TotWhExpPhA");
        totalWattHoursExportedPhaseADataPoint.setLabel("Total Watt-hours Exported phase A");
        totalWattHoursExportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseADataPoint.setSize(2);
        totalWattHoursExportedPhaseADataPoint.setAddressOffset(62);
        totalWattHoursExportedPhaseADataPoint.setBlockOffset(60);
        totalWattHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursExportedPhaseADataPoint.name(), totalWattHoursExportedPhaseADataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseBDataPoint;
        totalWattHoursExportedPhaseBDataPoint.setName("TotWhExpPhB");
        totalWattHoursExportedPhaseBDataPoint.setLabel("Total Watt-hours Exported phase B");
        totalWattHoursExportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseBDataPoint.setSize(2);
        totalWattHoursExportedPhaseBDataPoint.setAddressOffset(64);
        totalWattHoursExportedPhaseBDataPoint.setBlockOffset(62);
        totalWattHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursExportedPhaseBDataPoint.name(), totalWattHoursExportedPhaseBDataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseCDataPoint;
        totalWattHoursExportedPhaseCDataPoint.setName("TotWhExpPhC");
        totalWattHoursExportedPhaseCDataPoint.setLabel("Total Watt-hours Exported phase C");
        totalWattHoursExportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseCDataPoint.setSize(2);
        totalWattHoursExportedPhaseCDataPoint.setAddressOffset(66);
        totalWattHoursExportedPhaseCDataPoint.setBlockOffset(64);
        totalWattHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
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
        totalWattHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursImportedDataPoint.name(), totalWattHoursImportedDataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseADataPoint;
        totalWattHoursImportedPhaseADataPoint.setName("TotWhImpPhA");
        totalWattHoursImportedPhaseADataPoint.setLabel("Total Watt-hours Imported phase A");
        totalWattHoursImportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseADataPoint.setSize(2);
        totalWattHoursImportedPhaseADataPoint.setAddressOffset(70);
        totalWattHoursImportedPhaseADataPoint.setBlockOffset(68);
        totalWattHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseADataPoint.name(), totalWattHoursImportedPhaseADataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseBDataPoint;
        totalWattHoursImportedPhaseBDataPoint.setName("TotWhImpPhB");
        totalWattHoursImportedPhaseBDataPoint.setLabel("Total Watt-hours Imported phase B");
        totalWattHoursImportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseBDataPoint.setSize(2);
        totalWattHoursImportedPhaseBDataPoint.setAddressOffset(72);
        totalWattHoursImportedPhaseBDataPoint.setBlockOffset(70);
        totalWattHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseBDataPoint.name(), totalWattHoursImportedPhaseBDataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseCDataPoint;
        totalWattHoursImportedPhaseCDataPoint.setName("TotWhImpPhC");
        totalWattHoursImportedPhaseCDataPoint.setLabel("Total Watt-hours Imported phase C");
        totalWattHoursImportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseCDataPoint.setSize(2);
        totalWattHoursImportedPhaseCDataPoint.setAddressOffset(74);
        totalWattHoursImportedPhaseCDataPoint.setBlockOffset(72);
        totalWattHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseCDataPoint.name(), totalWattHoursImportedPhaseCDataPoint);

        SunSpecDataPoint totalVaHoursExportedDataPoint;
        totalVaHoursExportedDataPoint.setName("TotVAhExp");
        totalVaHoursExportedDataPoint.setLabel("Total VA-hours Exported");
        totalVaHoursExportedDataPoint.setDescription("Total Apparent Energy Exported");
        totalVaHoursExportedDataPoint.setUnits("VAh");
        totalVaHoursExportedDataPoint.setSize(2);
        totalVaHoursExportedDataPoint.setAddressOffset(76);
        totalVaHoursExportedDataPoint.setBlockOffset(74);
        totalVaHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursExportedDataPoint.name(), totalVaHoursExportedDataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseADataPoint;
        totalVaHoursExportedPhaseADataPoint.setName("TotVAhExpPhA");
        totalVaHoursExportedPhaseADataPoint.setLabel("Total VA-hours Exported phase A");
        totalVaHoursExportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseADataPoint.setSize(2);
        totalVaHoursExportedPhaseADataPoint.setAddressOffset(78);
        totalVaHoursExportedPhaseADataPoint.setBlockOffset(76);
        totalVaHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseADataPoint.name(), totalVaHoursExportedPhaseADataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseBDataPoint;
        totalVaHoursExportedPhaseBDataPoint.setName("TotVAhExpPhB");
        totalVaHoursExportedPhaseBDataPoint.setLabel("Total VA-hours Exported phase B");
        totalVaHoursExportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseBDataPoint.setSize(2);
        totalVaHoursExportedPhaseBDataPoint.setAddressOffset(80);
        totalVaHoursExportedPhaseBDataPoint.setBlockOffset(78);
        totalVaHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseBDataPoint.name(), totalVaHoursExportedPhaseBDataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseCDataPoint;
        totalVaHoursExportedPhaseCDataPoint.setName("TotVAhExpPhC");
        totalVaHoursExportedPhaseCDataPoint.setLabel("Total VA-hours Exported phase C");
        totalVaHoursExportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseCDataPoint.setSize(2);
        totalVaHoursExportedPhaseCDataPoint.setAddressOffset(82);
        totalVaHoursExportedPhaseCDataPoint.setBlockOffset(80);
        totalVaHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseCDataPoint.name(), totalVaHoursExportedPhaseCDataPoint);

        SunSpecDataPoint totalVaHoursImportedDataPoint;
        totalVaHoursImportedDataPoint.setName("TotVAhImp");
        totalVaHoursImportedDataPoint.setLabel("Total VA-hours Imported");
        totalVaHoursImportedDataPoint.setDescription("Total Apparent Energy Imported");
        totalVaHoursImportedDataPoint.setUnits("VAh");
        totalVaHoursImportedDataPoint.setSize(2);
        totalVaHoursImportedDataPoint.setAddressOffset(84);
        totalVaHoursImportedDataPoint.setBlockOffset(82);
        totalVaHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursImportedDataPoint.name(), totalVaHoursImportedDataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseADataPoint;
        totalVaHoursImportedPhaseADataPoint.setName("TotVAhImpPhA");
        totalVaHoursImportedPhaseADataPoint.setLabel("Total VA-hours Imported phase A");
        totalVaHoursImportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseADataPoint.setSize(2);
        totalVaHoursImportedPhaseADataPoint.setAddressOffset(86);
        totalVaHoursImportedPhaseADataPoint.setBlockOffset(84);
        totalVaHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseADataPoint.name(), totalVaHoursImportedPhaseADataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseBDataPoint;
        totalVaHoursImportedPhaseBDataPoint.setName("TotVAhImpPhB");
        totalVaHoursImportedPhaseBDataPoint.setLabel("Total VA-hours Imported phase B");
        totalVaHoursImportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseBDataPoint.setSize(2);
        totalVaHoursImportedPhaseBDataPoint.setAddressOffset(88);
        totalVaHoursImportedPhaseBDataPoint.setBlockOffset(86);
        totalVaHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseBDataPoint.name(), totalVaHoursImportedPhaseBDataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseCDataPoint;
        totalVaHoursImportedPhaseCDataPoint.setName("TotVAhImpPhC");
        totalVaHoursImportedPhaseCDataPoint.setLabel("Total VA-hours Imported phase C");
        totalVaHoursImportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseCDataPoint.setSize(2);
        totalVaHoursImportedPhaseCDataPoint.setAddressOffset(90);
        totalVaHoursImportedPhaseCDataPoint.setBlockOffset(88);
        totalVaHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseCDataPoint.name(), totalVaHoursImportedPhaseCDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1DataPoint;
        totalVarHoursImportedQ1DataPoint.setName("TotVArhImpQ1");
        totalVarHoursImportedQ1DataPoint.setLabel("Total VAR-hours Imported Q1");
        totalVarHoursImportedQ1DataPoint.setDescription("Total Reactive Energy Imported Quadrant 1");
        totalVarHoursImportedQ1DataPoint.setUnits("varh");
        totalVarHoursImportedQ1DataPoint.setSize(2);
        totalVarHoursImportedQ1DataPoint.setAddressOffset(92);
        totalVarHoursImportedQ1DataPoint.setBlockOffset(90);
        totalVarHoursImportedQ1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ1DataPoint.name(), totalVarHoursImportedQ1DataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseADataPoint;
        totalVarHoursImportedQ1PhaseADataPoint.setName("TotVArhImpQ1phA");
        totalVarHoursImportedQ1PhaseADataPoint.setLabel("Total VAr-hours Imported Q1 phase A");
        totalVarHoursImportedQ1PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseADataPoint.setAddressOffset(94);
        totalVarHoursImportedQ1PhaseADataPoint.setBlockOffset(92);
        totalVarHoursImportedQ1PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseADataPoint.name(), totalVarHoursImportedQ1PhaseADataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseBDataPoint;
        totalVarHoursImportedQ1PhaseBDataPoint.setName("TotVArhImpQ1phB");
        totalVarHoursImportedQ1PhaseBDataPoint.setLabel("Total VAr-hours Imported Q1 phase B");
        totalVarHoursImportedQ1PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseBDataPoint.setAddressOffset(96);
        totalVarHoursImportedQ1PhaseBDataPoint.setBlockOffset(94);
        totalVarHoursImportedQ1PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseBDataPoint.name(), totalVarHoursImportedQ1PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseCDataPoint;
        totalVarHoursImportedQ1PhaseCDataPoint.setName("TotVArhImpQ1phC");
        totalVarHoursImportedQ1PhaseCDataPoint.setLabel("Total VAr-hours Imported Q1 phase C");
        totalVarHoursImportedQ1PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseCDataPoint.setAddressOffset(98);
        totalVarHoursImportedQ1PhaseCDataPoint.setBlockOffset(96);
        totalVarHoursImportedQ1PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseCDataPoint.name(), totalVarHoursImportedQ1PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2DataPoint;
        totalVarHoursImportedQ2DataPoint.setName("TotVArhImpQ2");
        totalVarHoursImportedQ2DataPoint.setLabel("Total VAr-hours Imported Q2");
        totalVarHoursImportedQ2DataPoint.setDescription("Total Reactive Power Imported Quadrant 2");
        totalVarHoursImportedQ2DataPoint.setUnits("varh");
        totalVarHoursImportedQ2DataPoint.setSize(2);
        totalVarHoursImportedQ2DataPoint.setAddressOffset(100);
        totalVarHoursImportedQ2DataPoint.setBlockOffset(98);
        totalVarHoursImportedQ2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ2DataPoint.name(), totalVarHoursImportedQ2DataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseADataPoint;
        totalVarHoursImportedQ2PhaseADataPoint.setName("TotVArhImpQ2phA");
        totalVarHoursImportedQ2PhaseADataPoint.setLabel("Total VAr-hours Imported Q2 phase A");
        totalVarHoursImportedQ2PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseADataPoint.setAddressOffset(102);
        totalVarHoursImportedQ2PhaseADataPoint.setBlockOffset(100);
        totalVarHoursImportedQ2PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseADataPoint.name(), totalVarHoursImportedQ2PhaseADataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseBDataPoint;
        totalVarHoursImportedQ2PhaseBDataPoint.setName("TotVArhImpQ2phB");
        totalVarHoursImportedQ2PhaseBDataPoint.setLabel("Total VAr-hours Imported Q2 phase B");
        totalVarHoursImportedQ2PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseBDataPoint.setAddressOffset(104);
        totalVarHoursImportedQ2PhaseBDataPoint.setBlockOffset(102);
        totalVarHoursImportedQ2PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseBDataPoint.name(), totalVarHoursImportedQ2PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseCDataPoint;
        totalVarHoursImportedQ2PhaseCDataPoint.setName("TotVArhImpQ2phC");
        totalVarHoursImportedQ2PhaseCDataPoint.setLabel("Total VAr-hours Imported Q2 phase C");
        totalVarHoursImportedQ2PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseCDataPoint.setAddressOffset(106);
        totalVarHoursImportedQ2PhaseCDataPoint.setBlockOffset(104);
        totalVarHoursImportedQ2PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseCDataPoint.name(), totalVarHoursImportedQ2PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3DataPoint;
        totalVarHoursExportedQ3DataPoint.setName("TotVArhExpQ3");
        totalVarHoursExportedQ3DataPoint.setLabel("Total VAr-hours Exported Q3");
        totalVarHoursExportedQ3DataPoint.setDescription("Total Reactive Power Exported Quadrant 3");
        totalVarHoursExportedQ3DataPoint.setUnits("varh");
        totalVarHoursExportedQ3DataPoint.setSize(2);
        totalVarHoursExportedQ3DataPoint.setAddressOffset(108);
        totalVarHoursExportedQ3DataPoint.setBlockOffset(106);
        totalVarHoursExportedQ3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ3DataPoint.name(), totalVarHoursExportedQ3DataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseADataPoint;
        totalVarHoursExportedQ3PhaseADataPoint.setName("TotVArhExpQ3phA");
        totalVarHoursExportedQ3PhaseADataPoint.setLabel("Total VAr-hours Exported Q3 phase A");
        totalVarHoursExportedQ3PhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseADataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseADataPoint.setAddressOffset(110);
        totalVarHoursExportedQ3PhaseADataPoint.setBlockOffset(108);
        totalVarHoursExportedQ3PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseADataPoint.name(), totalVarHoursExportedQ3PhaseADataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseBDataPoint;
        totalVarHoursExportedQ3PhaseBDataPoint.setName("TotVArhExpQ3phB");
        totalVarHoursExportedQ3PhaseBDataPoint.setLabel("Total VAr-hours Exported Q3 phase B");
        totalVarHoursExportedQ3PhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseBDataPoint.setAddressOffset(112);
        totalVarHoursExportedQ3PhaseBDataPoint.setBlockOffset(110);
        totalVarHoursExportedQ3PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseBDataPoint.name(), totalVarHoursExportedQ3PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseCDataPoint;
        totalVarHoursExportedQ3PhaseCDataPoint.setName("TotVArhExpQ3phC");
        totalVarHoursExportedQ3PhaseCDataPoint.setLabel("Total VAr-hours Exported Q3 phase C");
        totalVarHoursExportedQ3PhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseCDataPoint.setAddressOffset(114);
        totalVarHoursExportedQ3PhaseCDataPoint.setBlockOffset(112);
        totalVarHoursExportedQ3PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseCDataPoint.name(), totalVarHoursExportedQ3PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4DataPoint;
        totalVarHoursExportedQ4DataPoint.setName("TotVArhExpQ4");
        totalVarHoursExportedQ4DataPoint.setLabel("Total VAr-hours Exported Q4");
        totalVarHoursExportedQ4DataPoint.setDescription("Total Reactive Power Exported Quadrant 4");
        totalVarHoursExportedQ4DataPoint.setUnits("varh");
        totalVarHoursExportedQ4DataPoint.setSize(2);
        totalVarHoursExportedQ4DataPoint.setAddressOffset(116);
        totalVarHoursExportedQ4DataPoint.setBlockOffset(114);
        totalVarHoursExportedQ4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ4DataPoint.name(), totalVarHoursExportedQ4DataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseADataPoint;
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setName("TotVArhExpQ4phA");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase A");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setAddressOffset(118);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setBlockOffset(116);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseADataPoint.name(), totalVarHoursExportedQ4ImportedPhaseADataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseBDataPoint;
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setName("TotVArhExpQ4phB");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase B");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setAddressOffset(120);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setBlockOffset(118);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseBDataPoint.name(), totalVarHoursExportedQ4ImportedPhaseBDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseCDataPoint;
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setName("TotVArhExpQ4phC");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase C");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setAddressOffset(122);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setBlockOffset(120);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseCDataPoint.name(), totalVarHoursExportedQ4ImportedPhaseCDataPoint);

        SunSpecDataPoint eventsDataPoint;
        eventsDataPoint.setName("Evt");
        eventsDataPoint.setLabel("Events");
        eventsDataPoint.setDescription("Meter Event Flags");
        eventsDataPoint.setMandatory(true);
        eventsDataPoint.setSize(2);
        eventsDataPoint.setAddressOffset(124);
        eventsDataPoint.setBlockOffset(122);
        eventsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(eventsDataPoint.name(), eventsDataPoint);

        break;
    }
    case 212: {
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
        ampsDataPoint.setDescription("Total AC Current");
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

        SunSpecDataPoint voltageLnDataPoint;
        voltageLnDataPoint.setName("PhV");
        voltageLnDataPoint.setLabel("Voltage LN");
        voltageLnDataPoint.setDescription("Line to Neutral AC Voltage (average of active phases)");
        voltageLnDataPoint.setUnits("V");
        voltageLnDataPoint.setMandatory(true);
        voltageLnDataPoint.setSize(2);
        voltageLnDataPoint.setAddressOffset(10);
        voltageLnDataPoint.setBlockOffset(8);
        voltageLnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(voltageLnDataPoint.name(), voltageLnDataPoint);

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
        phaseVoltageAnDataPoint.setMandatory(true);
        phaseVoltageAnDataPoint.setSize(2);
        phaseVoltageAnDataPoint.setAddressOffset(12);
        phaseVoltageAnDataPoint.setBlockOffset(10);
        phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(phaseVoltageAnDataPoint.name(), phaseVoltageAnDataPoint);

        SunSpecDataPoint phaseVoltageBnDataPoint;
        phaseVoltageBnDataPoint.setName("PhVphB");
        phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
        phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
        phaseVoltageBnDataPoint.setUnits("V");
        phaseVoltageBnDataPoint.setMandatory(true);
        phaseVoltageBnDataPoint.setSize(2);
        phaseVoltageBnDataPoint.setAddressOffset(14);
        phaseVoltageBnDataPoint.setBlockOffset(12);
        phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(phaseVoltageBnDataPoint.name(), phaseVoltageBnDataPoint);

        SunSpecDataPoint phaseVoltageCnDataPoint;
        phaseVoltageCnDataPoint.setName("PhVphC");
        phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
        phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
        phaseVoltageCnDataPoint.setUnits("V");
        phaseVoltageCnDataPoint.setSize(2);
        phaseVoltageCnDataPoint.setAddressOffset(16);
        phaseVoltageCnDataPoint.setBlockOffset(14);
        phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(phaseVoltageCnDataPoint.name(), phaseVoltageCnDataPoint);

        SunSpecDataPoint voltageLlDataPoint;
        voltageLlDataPoint.setName("PPV");
        voltageLlDataPoint.setLabel("Voltage LL");
        voltageLlDataPoint.setDescription("Line to Line AC Voltage (average of active phases)");
        voltageLlDataPoint.setUnits("V");
        voltageLlDataPoint.setMandatory(true);
        voltageLlDataPoint.setSize(2);
        voltageLlDataPoint.setAddressOffset(18);
        voltageLlDataPoint.setBlockOffset(16);
        voltageLlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(voltageLlDataPoint.name(), voltageLlDataPoint);

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PPVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setMandatory(true);
        phaseVoltageAbDataPoint.setSize(2);
        phaseVoltageAbDataPoint.setAddressOffset(20);
        phaseVoltageAbDataPoint.setBlockOffset(18);
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(phaseVoltageAbDataPoint.name(), phaseVoltageAbDataPoint);

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PPVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setSize(2);
        phaseVoltageBcDataPoint.setAddressOffset(22);
        phaseVoltageBcDataPoint.setBlockOffset(20);
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(phaseVoltageBcDataPoint.name(), phaseVoltageBcDataPoint);

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PPVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setSize(2);
        phaseVoltageCaDataPoint.setAddressOffset(24);
        phaseVoltageCaDataPoint.setBlockOffset(22);
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
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
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
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
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(wattsDataPoint.name(), wattsDataPoint);

        SunSpecDataPoint wattsPhaseADataPoint;
        wattsPhaseADataPoint.setName("WphA");
        wattsPhaseADataPoint.setLabel("Watts phase A");
        wattsPhaseADataPoint.setUnits("W");
        wattsPhaseADataPoint.setSize(2);
        wattsPhaseADataPoint.setAddressOffset(30);
        wattsPhaseADataPoint.setBlockOffset(28);
        wattsPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(wattsPhaseADataPoint.name(), wattsPhaseADataPoint);

        SunSpecDataPoint wattsPhaseBDataPoint;
        wattsPhaseBDataPoint.setName("WphB");
        wattsPhaseBDataPoint.setLabel("Watts phase B");
        wattsPhaseBDataPoint.setUnits("W");
        wattsPhaseBDataPoint.setSize(2);
        wattsPhaseBDataPoint.setAddressOffset(32);
        wattsPhaseBDataPoint.setBlockOffset(30);
        wattsPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(wattsPhaseBDataPoint.name(), wattsPhaseBDataPoint);

        SunSpecDataPoint wattsPhaseCDataPoint;
        wattsPhaseCDataPoint.setName("WphC");
        wattsPhaseCDataPoint.setLabel("Watts phase C");
        wattsPhaseCDataPoint.setUnits("W");
        wattsPhaseCDataPoint.setSize(2);
        wattsPhaseCDataPoint.setAddressOffset(34);
        wattsPhaseCDataPoint.setBlockOffset(32);
        wattsPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(wattsPhaseCDataPoint.name(), wattsPhaseCDataPoint);

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(2);
        vaDataPoint.setAddressOffset(36);
        vaDataPoint.setBlockOffset(34);
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(vaDataPoint.name(), vaDataPoint);

        SunSpecDataPoint vaPhaseADataPoint;
        vaPhaseADataPoint.setName("VAphA");
        vaPhaseADataPoint.setLabel("VA phase A");
        vaPhaseADataPoint.setUnits("VA");
        vaPhaseADataPoint.setSize(2);
        vaPhaseADataPoint.setAddressOffset(38);
        vaPhaseADataPoint.setBlockOffset(36);
        vaPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(vaPhaseADataPoint.name(), vaPhaseADataPoint);

        SunSpecDataPoint vaPhaseBDataPoint;
        vaPhaseBDataPoint.setName("VAphB");
        vaPhaseBDataPoint.setLabel("VA phase B");
        vaPhaseBDataPoint.setUnits("VA");
        vaPhaseBDataPoint.setSize(2);
        vaPhaseBDataPoint.setAddressOffset(40);
        vaPhaseBDataPoint.setBlockOffset(38);
        vaPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(vaPhaseBDataPoint.name(), vaPhaseBDataPoint);

        SunSpecDataPoint vaPhaseCDataPoint;
        vaPhaseCDataPoint.setName("VAphC");
        vaPhaseCDataPoint.setLabel("VA phase C");
        vaPhaseCDataPoint.setUnits("VA");
        vaPhaseCDataPoint.setSize(2);
        vaPhaseCDataPoint.setAddressOffset(42);
        vaPhaseCDataPoint.setBlockOffset(40);
        vaPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(vaPhaseCDataPoint.name(), vaPhaseCDataPoint);

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAR");
        varDataPoint.setLabel("VAR");
        varDataPoint.setDescription("Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(2);
        varDataPoint.setAddressOffset(44);
        varDataPoint.setBlockOffset(42);
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(varDataPoint.name(), varDataPoint);

        SunSpecDataPoint varPhaseADataPoint;
        varPhaseADataPoint.setName("VARphA");
        varPhaseADataPoint.setLabel("VAR phase A");
        varPhaseADataPoint.setUnits("var");
        varPhaseADataPoint.setSize(2);
        varPhaseADataPoint.setAddressOffset(46);
        varPhaseADataPoint.setBlockOffset(44);
        varPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(varPhaseADataPoint.name(), varPhaseADataPoint);

        SunSpecDataPoint varPhaseBDataPoint;
        varPhaseBDataPoint.setName("VARphB");
        varPhaseBDataPoint.setLabel("VAR phase B");
        varPhaseBDataPoint.setUnits("var");
        varPhaseBDataPoint.setSize(2);
        varPhaseBDataPoint.setAddressOffset(48);
        varPhaseBDataPoint.setBlockOffset(46);
        varPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(varPhaseBDataPoint.name(), varPhaseBDataPoint);

        SunSpecDataPoint varPhaseCDataPoint;
        varPhaseCDataPoint.setName("VARphC");
        varPhaseCDataPoint.setLabel("VAR phase C");
        varPhaseCDataPoint.setUnits("var");
        varPhaseCDataPoint.setSize(2);
        varPhaseCDataPoint.setAddressOffset(50);
        varPhaseCDataPoint.setBlockOffset(48);
        varPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(varPhaseCDataPoint.name(), varPhaseCDataPoint);

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("Power Factor");
        pfDataPoint.setUnits("PF");
        pfDataPoint.setSize(2);
        pfDataPoint.setAddressOffset(52);
        pfDataPoint.setBlockOffset(50);
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(pfDataPoint.name(), pfDataPoint);

        SunSpecDataPoint pfPhaseADataPoint;
        pfPhaseADataPoint.setName("PFphA");
        pfPhaseADataPoint.setLabel("PF phase A");
        pfPhaseADataPoint.setUnits("PF");
        pfPhaseADataPoint.setSize(2);
        pfPhaseADataPoint.setAddressOffset(54);
        pfPhaseADataPoint.setBlockOffset(52);
        pfPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(pfPhaseADataPoint.name(), pfPhaseADataPoint);

        SunSpecDataPoint pfPhaseBDataPoint;
        pfPhaseBDataPoint.setName("PFphB");
        pfPhaseBDataPoint.setLabel("PF phase B");
        pfPhaseBDataPoint.setUnits("PF");
        pfPhaseBDataPoint.setSize(2);
        pfPhaseBDataPoint.setAddressOffset(56);
        pfPhaseBDataPoint.setBlockOffset(54);
        pfPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(pfPhaseBDataPoint.name(), pfPhaseBDataPoint);

        SunSpecDataPoint pfPhaseCDataPoint;
        pfPhaseCDataPoint.setName("PFphC");
        pfPhaseCDataPoint.setLabel("PF phase C");
        pfPhaseCDataPoint.setUnits("PF");
        pfPhaseCDataPoint.setSize(2);
        pfPhaseCDataPoint.setAddressOffset(58);
        pfPhaseCDataPoint.setBlockOffset(56);
        pfPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
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
        totalWattHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursExportedDataPoint.name(), totalWattHoursExportedDataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseADataPoint;
        totalWattHoursExportedPhaseADataPoint.setName("TotWhExpPhA");
        totalWattHoursExportedPhaseADataPoint.setLabel("Total Watt-hours Exported phase A");
        totalWattHoursExportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseADataPoint.setSize(2);
        totalWattHoursExportedPhaseADataPoint.setAddressOffset(62);
        totalWattHoursExportedPhaseADataPoint.setBlockOffset(60);
        totalWattHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursExportedPhaseADataPoint.name(), totalWattHoursExportedPhaseADataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseBDataPoint;
        totalWattHoursExportedPhaseBDataPoint.setName("TotWhExpPhB");
        totalWattHoursExportedPhaseBDataPoint.setLabel("Total Watt-hours Exported phase B");
        totalWattHoursExportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseBDataPoint.setSize(2);
        totalWattHoursExportedPhaseBDataPoint.setAddressOffset(64);
        totalWattHoursExportedPhaseBDataPoint.setBlockOffset(62);
        totalWattHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursExportedPhaseBDataPoint.name(), totalWattHoursExportedPhaseBDataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseCDataPoint;
        totalWattHoursExportedPhaseCDataPoint.setName("TotWhExpPhC");
        totalWattHoursExportedPhaseCDataPoint.setLabel("Total Watt-hours Exported phase C");
        totalWattHoursExportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseCDataPoint.setSize(2);
        totalWattHoursExportedPhaseCDataPoint.setAddressOffset(66);
        totalWattHoursExportedPhaseCDataPoint.setBlockOffset(64);
        totalWattHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
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
        totalWattHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursImportedDataPoint.name(), totalWattHoursImportedDataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseADataPoint;
        totalWattHoursImportedPhaseADataPoint.setName("TotWhImpPhA");
        totalWattHoursImportedPhaseADataPoint.setLabel("Total Watt-hours Imported phase A");
        totalWattHoursImportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseADataPoint.setSize(2);
        totalWattHoursImportedPhaseADataPoint.setAddressOffset(70);
        totalWattHoursImportedPhaseADataPoint.setBlockOffset(68);
        totalWattHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseADataPoint.name(), totalWattHoursImportedPhaseADataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseBDataPoint;
        totalWattHoursImportedPhaseBDataPoint.setName("TotWhImpPhB");
        totalWattHoursImportedPhaseBDataPoint.setLabel("Total Watt-hours Imported phase B");
        totalWattHoursImportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseBDataPoint.setSize(2);
        totalWattHoursImportedPhaseBDataPoint.setAddressOffset(72);
        totalWattHoursImportedPhaseBDataPoint.setBlockOffset(70);
        totalWattHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseBDataPoint.name(), totalWattHoursImportedPhaseBDataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseCDataPoint;
        totalWattHoursImportedPhaseCDataPoint.setName("TotWhImpPhC");
        totalWattHoursImportedPhaseCDataPoint.setLabel("Total Watt-hours Imported phase C");
        totalWattHoursImportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseCDataPoint.setSize(2);
        totalWattHoursImportedPhaseCDataPoint.setAddressOffset(74);
        totalWattHoursImportedPhaseCDataPoint.setBlockOffset(72);
        totalWattHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseCDataPoint.name(), totalWattHoursImportedPhaseCDataPoint);

        SunSpecDataPoint totalVaHoursExportedDataPoint;
        totalVaHoursExportedDataPoint.setName("TotVAhExp");
        totalVaHoursExportedDataPoint.setLabel("Total VA-hours Exported");
        totalVaHoursExportedDataPoint.setDescription("Total Apparent Energy Exported");
        totalVaHoursExportedDataPoint.setUnits("VAh");
        totalVaHoursExportedDataPoint.setSize(2);
        totalVaHoursExportedDataPoint.setAddressOffset(76);
        totalVaHoursExportedDataPoint.setBlockOffset(74);
        totalVaHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursExportedDataPoint.name(), totalVaHoursExportedDataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseADataPoint;
        totalVaHoursExportedPhaseADataPoint.setName("TotVAhExpPhA");
        totalVaHoursExportedPhaseADataPoint.setLabel("Total VA-hours Exported phase A");
        totalVaHoursExportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseADataPoint.setSize(2);
        totalVaHoursExportedPhaseADataPoint.setAddressOffset(78);
        totalVaHoursExportedPhaseADataPoint.setBlockOffset(76);
        totalVaHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseADataPoint.name(), totalVaHoursExportedPhaseADataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseBDataPoint;
        totalVaHoursExportedPhaseBDataPoint.setName("TotVAhExpPhB");
        totalVaHoursExportedPhaseBDataPoint.setLabel("Total VA-hours Exported phase B");
        totalVaHoursExportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseBDataPoint.setSize(2);
        totalVaHoursExportedPhaseBDataPoint.setAddressOffset(80);
        totalVaHoursExportedPhaseBDataPoint.setBlockOffset(78);
        totalVaHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseBDataPoint.name(), totalVaHoursExportedPhaseBDataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseCDataPoint;
        totalVaHoursExportedPhaseCDataPoint.setName("TotVAhExpPhC");
        totalVaHoursExportedPhaseCDataPoint.setLabel("Total VA-hours Exported phase C");
        totalVaHoursExportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseCDataPoint.setSize(2);
        totalVaHoursExportedPhaseCDataPoint.setAddressOffset(82);
        totalVaHoursExportedPhaseCDataPoint.setBlockOffset(80);
        totalVaHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseCDataPoint.name(), totalVaHoursExportedPhaseCDataPoint);

        SunSpecDataPoint totalVaHoursImportedDataPoint;
        totalVaHoursImportedDataPoint.setName("TotVAhImp");
        totalVaHoursImportedDataPoint.setLabel("Total VA-hours Imported");
        totalVaHoursImportedDataPoint.setDescription("Total Apparent Energy Imported");
        totalVaHoursImportedDataPoint.setUnits("VAh");
        totalVaHoursImportedDataPoint.setSize(2);
        totalVaHoursImportedDataPoint.setAddressOffset(84);
        totalVaHoursImportedDataPoint.setBlockOffset(82);
        totalVaHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursImportedDataPoint.name(), totalVaHoursImportedDataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseADataPoint;
        totalVaHoursImportedPhaseADataPoint.setName("TotVAhImpPhA");
        totalVaHoursImportedPhaseADataPoint.setLabel("Total VA-hours Imported phase A");
        totalVaHoursImportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseADataPoint.setSize(2);
        totalVaHoursImportedPhaseADataPoint.setAddressOffset(86);
        totalVaHoursImportedPhaseADataPoint.setBlockOffset(84);
        totalVaHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseADataPoint.name(), totalVaHoursImportedPhaseADataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseBDataPoint;
        totalVaHoursImportedPhaseBDataPoint.setName("TotVAhImpPhB");
        totalVaHoursImportedPhaseBDataPoint.setLabel("Total VA-hours Imported phase B");
        totalVaHoursImportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseBDataPoint.setSize(2);
        totalVaHoursImportedPhaseBDataPoint.setAddressOffset(88);
        totalVaHoursImportedPhaseBDataPoint.setBlockOffset(86);
        totalVaHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseBDataPoint.name(), totalVaHoursImportedPhaseBDataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseCDataPoint;
        totalVaHoursImportedPhaseCDataPoint.setName("TotVAhImpPhC");
        totalVaHoursImportedPhaseCDataPoint.setLabel("Total VA-hours Imported phase C");
        totalVaHoursImportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseCDataPoint.setSize(2);
        totalVaHoursImportedPhaseCDataPoint.setAddressOffset(90);
        totalVaHoursImportedPhaseCDataPoint.setBlockOffset(88);
        totalVaHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseCDataPoint.name(), totalVaHoursImportedPhaseCDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1DataPoint;
        totalVarHoursImportedQ1DataPoint.setName("TotVArhImpQ1");
        totalVarHoursImportedQ1DataPoint.setLabel("Total VAR-hours Imported Q1");
        totalVarHoursImportedQ1DataPoint.setDescription("Total Reactive Energy Imported Quadrant 1");
        totalVarHoursImportedQ1DataPoint.setUnits("varh");
        totalVarHoursImportedQ1DataPoint.setSize(2);
        totalVarHoursImportedQ1DataPoint.setAddressOffset(92);
        totalVarHoursImportedQ1DataPoint.setBlockOffset(90);
        totalVarHoursImportedQ1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ1DataPoint.name(), totalVarHoursImportedQ1DataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseADataPoint;
        totalVarHoursImportedQ1PhaseADataPoint.setName("TotVArhImpQ1phA");
        totalVarHoursImportedQ1PhaseADataPoint.setLabel("Total VAr-hours Imported Q1 phase A");
        totalVarHoursImportedQ1PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseADataPoint.setAddressOffset(94);
        totalVarHoursImportedQ1PhaseADataPoint.setBlockOffset(92);
        totalVarHoursImportedQ1PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseADataPoint.name(), totalVarHoursImportedQ1PhaseADataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseBDataPoint;
        totalVarHoursImportedQ1PhaseBDataPoint.setName("TotVArhImpQ1phB");
        totalVarHoursImportedQ1PhaseBDataPoint.setLabel("Total VAr-hours Imported Q1 phase B");
        totalVarHoursImportedQ1PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseBDataPoint.setAddressOffset(96);
        totalVarHoursImportedQ1PhaseBDataPoint.setBlockOffset(94);
        totalVarHoursImportedQ1PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseBDataPoint.name(), totalVarHoursImportedQ1PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseCDataPoint;
        totalVarHoursImportedQ1PhaseCDataPoint.setName("TotVArhImpQ1phC");
        totalVarHoursImportedQ1PhaseCDataPoint.setLabel("Total VAr-hours Imported Q1 phase C");
        totalVarHoursImportedQ1PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseCDataPoint.setAddressOffset(98);
        totalVarHoursImportedQ1PhaseCDataPoint.setBlockOffset(96);
        totalVarHoursImportedQ1PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseCDataPoint.name(), totalVarHoursImportedQ1PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2DataPoint;
        totalVarHoursImportedQ2DataPoint.setName("TotVArhImpQ2");
        totalVarHoursImportedQ2DataPoint.setLabel("Total VAr-hours Imported Q2");
        totalVarHoursImportedQ2DataPoint.setDescription("Total Reactive Power Imported Quadrant 2");
        totalVarHoursImportedQ2DataPoint.setUnits("varh");
        totalVarHoursImportedQ2DataPoint.setSize(2);
        totalVarHoursImportedQ2DataPoint.setAddressOffset(100);
        totalVarHoursImportedQ2DataPoint.setBlockOffset(98);
        totalVarHoursImportedQ2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ2DataPoint.name(), totalVarHoursImportedQ2DataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseADataPoint;
        totalVarHoursImportedQ2PhaseADataPoint.setName("TotVArhImpQ2phA");
        totalVarHoursImportedQ2PhaseADataPoint.setLabel("Total VAr-hours Imported Q2 phase A");
        totalVarHoursImportedQ2PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseADataPoint.setAddressOffset(102);
        totalVarHoursImportedQ2PhaseADataPoint.setBlockOffset(100);
        totalVarHoursImportedQ2PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseADataPoint.name(), totalVarHoursImportedQ2PhaseADataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseBDataPoint;
        totalVarHoursImportedQ2PhaseBDataPoint.setName("TotVArhImpQ2phB");
        totalVarHoursImportedQ2PhaseBDataPoint.setLabel("Total VAr-hours Imported Q2 phase B");
        totalVarHoursImportedQ2PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseBDataPoint.setAddressOffset(104);
        totalVarHoursImportedQ2PhaseBDataPoint.setBlockOffset(102);
        totalVarHoursImportedQ2PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseBDataPoint.name(), totalVarHoursImportedQ2PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseCDataPoint;
        totalVarHoursImportedQ2PhaseCDataPoint.setName("TotVArhImpQ2phC");
        totalVarHoursImportedQ2PhaseCDataPoint.setLabel("Total VAr-hours Imported Q2 phase C");
        totalVarHoursImportedQ2PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseCDataPoint.setAddressOffset(106);
        totalVarHoursImportedQ2PhaseCDataPoint.setBlockOffset(104);
        totalVarHoursImportedQ2PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseCDataPoint.name(), totalVarHoursImportedQ2PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3DataPoint;
        totalVarHoursExportedQ3DataPoint.setName("TotVArhExpQ3");
        totalVarHoursExportedQ3DataPoint.setLabel("Total VAr-hours Exported Q3");
        totalVarHoursExportedQ3DataPoint.setDescription("Total Reactive Power Exported Quadrant 3");
        totalVarHoursExportedQ3DataPoint.setUnits("varh");
        totalVarHoursExportedQ3DataPoint.setSize(2);
        totalVarHoursExportedQ3DataPoint.setAddressOffset(108);
        totalVarHoursExportedQ3DataPoint.setBlockOffset(106);
        totalVarHoursExportedQ3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ3DataPoint.name(), totalVarHoursExportedQ3DataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseADataPoint;
        totalVarHoursExportedQ3PhaseADataPoint.setName("TotVArhExpQ3phA");
        totalVarHoursExportedQ3PhaseADataPoint.setLabel("Total VAr-hours Exported Q3 phase A");
        totalVarHoursExportedQ3PhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseADataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseADataPoint.setAddressOffset(110);
        totalVarHoursExportedQ3PhaseADataPoint.setBlockOffset(108);
        totalVarHoursExportedQ3PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseADataPoint.name(), totalVarHoursExportedQ3PhaseADataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseBDataPoint;
        totalVarHoursExportedQ3PhaseBDataPoint.setName("TotVArhExpQ3phB");
        totalVarHoursExportedQ3PhaseBDataPoint.setLabel("Total VAr-hours Exported Q3 phase B");
        totalVarHoursExportedQ3PhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseBDataPoint.setAddressOffset(112);
        totalVarHoursExportedQ3PhaseBDataPoint.setBlockOffset(110);
        totalVarHoursExportedQ3PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseBDataPoint.name(), totalVarHoursExportedQ3PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseCDataPoint;
        totalVarHoursExportedQ3PhaseCDataPoint.setName("TotVArhExpQ3phC");
        totalVarHoursExportedQ3PhaseCDataPoint.setLabel("Total VAr-hours Exported Q3 phase C");
        totalVarHoursExportedQ3PhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseCDataPoint.setAddressOffset(114);
        totalVarHoursExportedQ3PhaseCDataPoint.setBlockOffset(112);
        totalVarHoursExportedQ3PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseCDataPoint.name(), totalVarHoursExportedQ3PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4DataPoint;
        totalVarHoursExportedQ4DataPoint.setName("TotVArhExpQ4");
        totalVarHoursExportedQ4DataPoint.setLabel("Total VAr-hours Exported Q4");
        totalVarHoursExportedQ4DataPoint.setDescription("Total Reactive Power Exported Quadrant 4");
        totalVarHoursExportedQ4DataPoint.setUnits("varh");
        totalVarHoursExportedQ4DataPoint.setSize(2);
        totalVarHoursExportedQ4DataPoint.setAddressOffset(116);
        totalVarHoursExportedQ4DataPoint.setBlockOffset(114);
        totalVarHoursExportedQ4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ4DataPoint.name(), totalVarHoursExportedQ4DataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseADataPoint;
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setName("TotVArhExpQ4phA");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase A");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setAddressOffset(118);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setBlockOffset(116);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseADataPoint.name(), totalVarHoursExportedQ4ImportedPhaseADataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseBDataPoint;
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setName("TotVArhExpQ4phB");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase B");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setAddressOffset(120);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setBlockOffset(118);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseBDataPoint.name(), totalVarHoursExportedQ4ImportedPhaseBDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseCDataPoint;
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setName("TotVArhExpQ4phC");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase C");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setAddressOffset(122);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setBlockOffset(120);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseCDataPoint.name(), totalVarHoursExportedQ4ImportedPhaseCDataPoint);

        SunSpecDataPoint eventsDataPoint;
        eventsDataPoint.setName("Evt");
        eventsDataPoint.setLabel("Events");
        eventsDataPoint.setDescription("Meter Event Flags");
        eventsDataPoint.setMandatory(true);
        eventsDataPoint.setSize(2);
        eventsDataPoint.setAddressOffset(124);
        eventsDataPoint.setBlockOffset(122);
        eventsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(eventsDataPoint.name(), eventsDataPoint);

        break;
    }
    case 213: {
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
        ampsDataPoint.setDescription("Total AC Current");
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

        SunSpecDataPoint voltageLnDataPoint;
        voltageLnDataPoint.setName("PhV");
        voltageLnDataPoint.setLabel("Voltage LN");
        voltageLnDataPoint.setDescription("Line to Neutral AC Voltage (average of active phases)");
        voltageLnDataPoint.setUnits("V");
        voltageLnDataPoint.setMandatory(true);
        voltageLnDataPoint.setSize(2);
        voltageLnDataPoint.setAddressOffset(10);
        voltageLnDataPoint.setBlockOffset(8);
        voltageLnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(voltageLnDataPoint.name(), voltageLnDataPoint);

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
        phaseVoltageAnDataPoint.setMandatory(true);
        phaseVoltageAnDataPoint.setSize(2);
        phaseVoltageAnDataPoint.setAddressOffset(12);
        phaseVoltageAnDataPoint.setBlockOffset(10);
        phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(phaseVoltageAnDataPoint.name(), phaseVoltageAnDataPoint);

        SunSpecDataPoint phaseVoltageBnDataPoint;
        phaseVoltageBnDataPoint.setName("PhVphB");
        phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
        phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
        phaseVoltageBnDataPoint.setUnits("V");
        phaseVoltageBnDataPoint.setMandatory(true);
        phaseVoltageBnDataPoint.setSize(2);
        phaseVoltageBnDataPoint.setAddressOffset(14);
        phaseVoltageBnDataPoint.setBlockOffset(12);
        phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(phaseVoltageBnDataPoint.name(), phaseVoltageBnDataPoint);

        SunSpecDataPoint phaseVoltageCnDataPoint;
        phaseVoltageCnDataPoint.setName("PhVphC");
        phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
        phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
        phaseVoltageCnDataPoint.setUnits("V");
        phaseVoltageCnDataPoint.setMandatory(true);
        phaseVoltageCnDataPoint.setSize(2);
        phaseVoltageCnDataPoint.setAddressOffset(16);
        phaseVoltageCnDataPoint.setBlockOffset(14);
        phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(phaseVoltageCnDataPoint.name(), phaseVoltageCnDataPoint);

        SunSpecDataPoint voltageLlDataPoint;
        voltageLlDataPoint.setName("PPV");
        voltageLlDataPoint.setLabel("Voltage LL");
        voltageLlDataPoint.setDescription("Line to Line AC Voltage (average of active phases)");
        voltageLlDataPoint.setUnits("V");
        voltageLlDataPoint.setMandatory(true);
        voltageLlDataPoint.setSize(2);
        voltageLlDataPoint.setAddressOffset(18);
        voltageLlDataPoint.setBlockOffset(16);
        voltageLlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(voltageLlDataPoint.name(), voltageLlDataPoint);

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PPVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setMandatory(true);
        phaseVoltageAbDataPoint.setSize(2);
        phaseVoltageAbDataPoint.setAddressOffset(20);
        phaseVoltageAbDataPoint.setBlockOffset(18);
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(phaseVoltageAbDataPoint.name(), phaseVoltageAbDataPoint);

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PPVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setMandatory(true);
        phaseVoltageBcDataPoint.setSize(2);
        phaseVoltageBcDataPoint.setAddressOffset(22);
        phaseVoltageBcDataPoint.setBlockOffset(20);
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(phaseVoltageBcDataPoint.name(), phaseVoltageBcDataPoint);

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PPVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setMandatory(true);
        phaseVoltageCaDataPoint.setSize(2);
        phaseVoltageCaDataPoint.setAddressOffset(24);
        phaseVoltageCaDataPoint.setBlockOffset(22);
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
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
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
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
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(wattsDataPoint.name(), wattsDataPoint);

        SunSpecDataPoint wattsPhaseADataPoint;
        wattsPhaseADataPoint.setName("WphA");
        wattsPhaseADataPoint.setLabel("Watts phase A");
        wattsPhaseADataPoint.setUnits("W");
        wattsPhaseADataPoint.setSize(2);
        wattsPhaseADataPoint.setAddressOffset(30);
        wattsPhaseADataPoint.setBlockOffset(28);
        wattsPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(wattsPhaseADataPoint.name(), wattsPhaseADataPoint);

        SunSpecDataPoint wattsPhaseBDataPoint;
        wattsPhaseBDataPoint.setName("WphB");
        wattsPhaseBDataPoint.setLabel("Watts phase B");
        wattsPhaseBDataPoint.setUnits("W");
        wattsPhaseBDataPoint.setSize(2);
        wattsPhaseBDataPoint.setAddressOffset(32);
        wattsPhaseBDataPoint.setBlockOffset(30);
        wattsPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(wattsPhaseBDataPoint.name(), wattsPhaseBDataPoint);

        SunSpecDataPoint wattsPhaseCDataPoint;
        wattsPhaseCDataPoint.setName("WphC");
        wattsPhaseCDataPoint.setLabel("Watts phase C");
        wattsPhaseCDataPoint.setUnits("W");
        wattsPhaseCDataPoint.setSize(2);
        wattsPhaseCDataPoint.setAddressOffset(34);
        wattsPhaseCDataPoint.setBlockOffset(32);
        wattsPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(wattsPhaseCDataPoint.name(), wattsPhaseCDataPoint);

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(2);
        vaDataPoint.setAddressOffset(36);
        vaDataPoint.setBlockOffset(34);
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(vaDataPoint.name(), vaDataPoint);

        SunSpecDataPoint vaPhaseADataPoint;
        vaPhaseADataPoint.setName("VAphA");
        vaPhaseADataPoint.setLabel("VA phase A");
        vaPhaseADataPoint.setUnits("VA");
        vaPhaseADataPoint.setSize(2);
        vaPhaseADataPoint.setAddressOffset(38);
        vaPhaseADataPoint.setBlockOffset(36);
        vaPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(vaPhaseADataPoint.name(), vaPhaseADataPoint);

        SunSpecDataPoint vaPhaseBDataPoint;
        vaPhaseBDataPoint.setName("VAphB");
        vaPhaseBDataPoint.setLabel("VA phase B");
        vaPhaseBDataPoint.setUnits("VA");
        vaPhaseBDataPoint.setSize(2);
        vaPhaseBDataPoint.setAddressOffset(40);
        vaPhaseBDataPoint.setBlockOffset(38);
        vaPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(vaPhaseBDataPoint.name(), vaPhaseBDataPoint);

        SunSpecDataPoint vaPhaseCDataPoint;
        vaPhaseCDataPoint.setName("VAphC");
        vaPhaseCDataPoint.setLabel("VA phase C");
        vaPhaseCDataPoint.setUnits("VA");
        vaPhaseCDataPoint.setSize(2);
        vaPhaseCDataPoint.setAddressOffset(42);
        vaPhaseCDataPoint.setBlockOffset(40);
        vaPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(vaPhaseCDataPoint.name(), vaPhaseCDataPoint);

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAR");
        varDataPoint.setLabel("VAR");
        varDataPoint.setDescription("Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(2);
        varDataPoint.setAddressOffset(44);
        varDataPoint.setBlockOffset(42);
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(varDataPoint.name(), varDataPoint);

        SunSpecDataPoint varPhaseADataPoint;
        varPhaseADataPoint.setName("VARphA");
        varPhaseADataPoint.setLabel("VAR phase A");
        varPhaseADataPoint.setUnits("var");
        varPhaseADataPoint.setSize(2);
        varPhaseADataPoint.setAddressOffset(46);
        varPhaseADataPoint.setBlockOffset(44);
        varPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(varPhaseADataPoint.name(), varPhaseADataPoint);

        SunSpecDataPoint varPhaseBDataPoint;
        varPhaseBDataPoint.setName("VARphB");
        varPhaseBDataPoint.setLabel("VAR phase B");
        varPhaseBDataPoint.setUnits("var");
        varPhaseBDataPoint.setSize(2);
        varPhaseBDataPoint.setAddressOffset(48);
        varPhaseBDataPoint.setBlockOffset(46);
        varPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(varPhaseBDataPoint.name(), varPhaseBDataPoint);

        SunSpecDataPoint varPhaseCDataPoint;
        varPhaseCDataPoint.setName("VARphC");
        varPhaseCDataPoint.setLabel("VAR phase C");
        varPhaseCDataPoint.setUnits("var");
        varPhaseCDataPoint.setSize(2);
        varPhaseCDataPoint.setAddressOffset(50);
        varPhaseCDataPoint.setBlockOffset(48);
        varPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(varPhaseCDataPoint.name(), varPhaseCDataPoint);

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("Power Factor");
        pfDataPoint.setUnits("PF");
        pfDataPoint.setSize(2);
        pfDataPoint.setAddressOffset(52);
        pfDataPoint.setBlockOffset(50);
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(pfDataPoint.name(), pfDataPoint);

        SunSpecDataPoint pfPhaseADataPoint;
        pfPhaseADataPoint.setName("PFphA");
        pfPhaseADataPoint.setLabel("PF phase A");
        pfPhaseADataPoint.setUnits("PF");
        pfPhaseADataPoint.setSize(2);
        pfPhaseADataPoint.setAddressOffset(54);
        pfPhaseADataPoint.setBlockOffset(52);
        pfPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(pfPhaseADataPoint.name(), pfPhaseADataPoint);

        SunSpecDataPoint pfPhaseBDataPoint;
        pfPhaseBDataPoint.setName("PFphB");
        pfPhaseBDataPoint.setLabel("PF phase B");
        pfPhaseBDataPoint.setUnits("PF");
        pfPhaseBDataPoint.setSize(2);
        pfPhaseBDataPoint.setAddressOffset(56);
        pfPhaseBDataPoint.setBlockOffset(54);
        pfPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(pfPhaseBDataPoint.name(), pfPhaseBDataPoint);

        SunSpecDataPoint pfPhaseCDataPoint;
        pfPhaseCDataPoint.setName("PFphC");
        pfPhaseCDataPoint.setLabel("PF phase C");
        pfPhaseCDataPoint.setUnits("PF");
        pfPhaseCDataPoint.setSize(2);
        pfPhaseCDataPoint.setAddressOffset(58);
        pfPhaseCDataPoint.setBlockOffset(56);
        pfPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
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
        totalWattHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursExportedDataPoint.name(), totalWattHoursExportedDataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseADataPoint;
        totalWattHoursExportedPhaseADataPoint.setName("TotWhExpPhA");
        totalWattHoursExportedPhaseADataPoint.setLabel("Total Watt-hours Exported phase A");
        totalWattHoursExportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseADataPoint.setSize(2);
        totalWattHoursExportedPhaseADataPoint.setAddressOffset(62);
        totalWattHoursExportedPhaseADataPoint.setBlockOffset(60);
        totalWattHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursExportedPhaseADataPoint.name(), totalWattHoursExportedPhaseADataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseBDataPoint;
        totalWattHoursExportedPhaseBDataPoint.setName("TotWhExpPhB");
        totalWattHoursExportedPhaseBDataPoint.setLabel("Total Watt-hours Exported phase B");
        totalWattHoursExportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseBDataPoint.setSize(2);
        totalWattHoursExportedPhaseBDataPoint.setAddressOffset(64);
        totalWattHoursExportedPhaseBDataPoint.setBlockOffset(62);
        totalWattHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursExportedPhaseBDataPoint.name(), totalWattHoursExportedPhaseBDataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseCDataPoint;
        totalWattHoursExportedPhaseCDataPoint.setName("TotWhExpPhC");
        totalWattHoursExportedPhaseCDataPoint.setLabel("Total Watt-hours Exported phase C");
        totalWattHoursExportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseCDataPoint.setSize(2);
        totalWattHoursExportedPhaseCDataPoint.setAddressOffset(66);
        totalWattHoursExportedPhaseCDataPoint.setBlockOffset(64);
        totalWattHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
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
        totalWattHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursImportedDataPoint.name(), totalWattHoursImportedDataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseADataPoint;
        totalWattHoursImportedPhaseADataPoint.setName("TotWhImpPhA");
        totalWattHoursImportedPhaseADataPoint.setLabel("Total Watt-hours Imported phase A");
        totalWattHoursImportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseADataPoint.setSize(2);
        totalWattHoursImportedPhaseADataPoint.setAddressOffset(70);
        totalWattHoursImportedPhaseADataPoint.setBlockOffset(68);
        totalWattHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseADataPoint.name(), totalWattHoursImportedPhaseADataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseBDataPoint;
        totalWattHoursImportedPhaseBDataPoint.setName("TotWhImpPhB");
        totalWattHoursImportedPhaseBDataPoint.setLabel("Total Watt-hours Imported phase B");
        totalWattHoursImportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseBDataPoint.setSize(2);
        totalWattHoursImportedPhaseBDataPoint.setAddressOffset(72);
        totalWattHoursImportedPhaseBDataPoint.setBlockOffset(70);
        totalWattHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseBDataPoint.name(), totalWattHoursImportedPhaseBDataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseCDataPoint;
        totalWattHoursImportedPhaseCDataPoint.setName("TotWhImpPhC");
        totalWattHoursImportedPhaseCDataPoint.setLabel("Total Watt-hours Imported phase C");
        totalWattHoursImportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseCDataPoint.setSize(2);
        totalWattHoursImportedPhaseCDataPoint.setAddressOffset(74);
        totalWattHoursImportedPhaseCDataPoint.setBlockOffset(72);
        totalWattHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseCDataPoint.name(), totalWattHoursImportedPhaseCDataPoint);

        SunSpecDataPoint totalVaHoursExportedDataPoint;
        totalVaHoursExportedDataPoint.setName("TotVAhExp");
        totalVaHoursExportedDataPoint.setLabel("Total VA-hours Exported");
        totalVaHoursExportedDataPoint.setDescription("Total Apparent Energy Exported");
        totalVaHoursExportedDataPoint.setUnits("VAh");
        totalVaHoursExportedDataPoint.setSize(2);
        totalVaHoursExportedDataPoint.setAddressOffset(76);
        totalVaHoursExportedDataPoint.setBlockOffset(74);
        totalVaHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursExportedDataPoint.name(), totalVaHoursExportedDataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseADataPoint;
        totalVaHoursExportedPhaseADataPoint.setName("TotVAhExpPhA");
        totalVaHoursExportedPhaseADataPoint.setLabel("Total VA-hours Exported phase A");
        totalVaHoursExportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseADataPoint.setSize(2);
        totalVaHoursExportedPhaseADataPoint.setAddressOffset(78);
        totalVaHoursExportedPhaseADataPoint.setBlockOffset(76);
        totalVaHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseADataPoint.name(), totalVaHoursExportedPhaseADataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseBDataPoint;
        totalVaHoursExportedPhaseBDataPoint.setName("TotVAhExpPhB");
        totalVaHoursExportedPhaseBDataPoint.setLabel("Total VA-hours Exported phase B");
        totalVaHoursExportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseBDataPoint.setSize(2);
        totalVaHoursExportedPhaseBDataPoint.setAddressOffset(80);
        totalVaHoursExportedPhaseBDataPoint.setBlockOffset(78);
        totalVaHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseBDataPoint.name(), totalVaHoursExportedPhaseBDataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseCDataPoint;
        totalVaHoursExportedPhaseCDataPoint.setName("TotVAhExpPhC");
        totalVaHoursExportedPhaseCDataPoint.setLabel("Total VA-hours Exported phase C");
        totalVaHoursExportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseCDataPoint.setSize(2);
        totalVaHoursExportedPhaseCDataPoint.setAddressOffset(82);
        totalVaHoursExportedPhaseCDataPoint.setBlockOffset(80);
        totalVaHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseCDataPoint.name(), totalVaHoursExportedPhaseCDataPoint);

        SunSpecDataPoint totalVaHoursImportedDataPoint;
        totalVaHoursImportedDataPoint.setName("TotVAhImp");
        totalVaHoursImportedDataPoint.setLabel("Total VA-hours Imported");
        totalVaHoursImportedDataPoint.setDescription("Total Apparent Energy Imported");
        totalVaHoursImportedDataPoint.setUnits("VAh");
        totalVaHoursImportedDataPoint.setSize(2);
        totalVaHoursImportedDataPoint.setAddressOffset(84);
        totalVaHoursImportedDataPoint.setBlockOffset(82);
        totalVaHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursImportedDataPoint.name(), totalVaHoursImportedDataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseADataPoint;
        totalVaHoursImportedPhaseADataPoint.setName("TotVAhImpPhA");
        totalVaHoursImportedPhaseADataPoint.setLabel("Total VA-hours Imported phase A");
        totalVaHoursImportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseADataPoint.setSize(2);
        totalVaHoursImportedPhaseADataPoint.setAddressOffset(86);
        totalVaHoursImportedPhaseADataPoint.setBlockOffset(84);
        totalVaHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseADataPoint.name(), totalVaHoursImportedPhaseADataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseBDataPoint;
        totalVaHoursImportedPhaseBDataPoint.setName("TotVAhImpPhB");
        totalVaHoursImportedPhaseBDataPoint.setLabel("Total VA-hours Imported phase B");
        totalVaHoursImportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseBDataPoint.setSize(2);
        totalVaHoursImportedPhaseBDataPoint.setAddressOffset(88);
        totalVaHoursImportedPhaseBDataPoint.setBlockOffset(86);
        totalVaHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseBDataPoint.name(), totalVaHoursImportedPhaseBDataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseCDataPoint;
        totalVaHoursImportedPhaseCDataPoint.setName("TotVAhImpPhC");
        totalVaHoursImportedPhaseCDataPoint.setLabel("Total VA-hours Imported phase C");
        totalVaHoursImportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseCDataPoint.setSize(2);
        totalVaHoursImportedPhaseCDataPoint.setAddressOffset(90);
        totalVaHoursImportedPhaseCDataPoint.setBlockOffset(88);
        totalVaHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseCDataPoint.name(), totalVaHoursImportedPhaseCDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1DataPoint;
        totalVarHoursImportedQ1DataPoint.setName("TotVArhImpQ1");
        totalVarHoursImportedQ1DataPoint.setLabel("Total VAR-hours Imported Q1");
        totalVarHoursImportedQ1DataPoint.setDescription("Total Reactive Energy Imported Quadrant 1");
        totalVarHoursImportedQ1DataPoint.setUnits("varh");
        totalVarHoursImportedQ1DataPoint.setSize(2);
        totalVarHoursImportedQ1DataPoint.setAddressOffset(92);
        totalVarHoursImportedQ1DataPoint.setBlockOffset(90);
        totalVarHoursImportedQ1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ1DataPoint.name(), totalVarHoursImportedQ1DataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseADataPoint;
        totalVarHoursImportedQ1PhaseADataPoint.setName("TotVArhImpQ1phA");
        totalVarHoursImportedQ1PhaseADataPoint.setLabel("Total VAr-hours Imported Q1 phase A");
        totalVarHoursImportedQ1PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseADataPoint.setAddressOffset(94);
        totalVarHoursImportedQ1PhaseADataPoint.setBlockOffset(92);
        totalVarHoursImportedQ1PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseADataPoint.name(), totalVarHoursImportedQ1PhaseADataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseBDataPoint;
        totalVarHoursImportedQ1PhaseBDataPoint.setName("TotVArhImpQ1phB");
        totalVarHoursImportedQ1PhaseBDataPoint.setLabel("Total VAr-hours Imported Q1 phase B");
        totalVarHoursImportedQ1PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseBDataPoint.setAddressOffset(96);
        totalVarHoursImportedQ1PhaseBDataPoint.setBlockOffset(94);
        totalVarHoursImportedQ1PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseBDataPoint.name(), totalVarHoursImportedQ1PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseCDataPoint;
        totalVarHoursImportedQ1PhaseCDataPoint.setName("TotVArhImpQ1phC");
        totalVarHoursImportedQ1PhaseCDataPoint.setLabel("Total VAr-hours Imported Q1 phase C");
        totalVarHoursImportedQ1PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseCDataPoint.setAddressOffset(98);
        totalVarHoursImportedQ1PhaseCDataPoint.setBlockOffset(96);
        totalVarHoursImportedQ1PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseCDataPoint.name(), totalVarHoursImportedQ1PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2DataPoint;
        totalVarHoursImportedQ2DataPoint.setName("TotVArhImpQ2");
        totalVarHoursImportedQ2DataPoint.setLabel("Total VAr-hours Imported Q2");
        totalVarHoursImportedQ2DataPoint.setDescription("Total Reactive Power Imported Quadrant 2");
        totalVarHoursImportedQ2DataPoint.setUnits("varh");
        totalVarHoursImportedQ2DataPoint.setSize(2);
        totalVarHoursImportedQ2DataPoint.setAddressOffset(100);
        totalVarHoursImportedQ2DataPoint.setBlockOffset(98);
        totalVarHoursImportedQ2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ2DataPoint.name(), totalVarHoursImportedQ2DataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseADataPoint;
        totalVarHoursImportedQ2PhaseADataPoint.setName("TotVArhImpQ2phA");
        totalVarHoursImportedQ2PhaseADataPoint.setLabel("Total VAr-hours Imported Q2 phase A");
        totalVarHoursImportedQ2PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseADataPoint.setAddressOffset(102);
        totalVarHoursImportedQ2PhaseADataPoint.setBlockOffset(100);
        totalVarHoursImportedQ2PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseADataPoint.name(), totalVarHoursImportedQ2PhaseADataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseBDataPoint;
        totalVarHoursImportedQ2PhaseBDataPoint.setName("TotVArhImpQ2phB");
        totalVarHoursImportedQ2PhaseBDataPoint.setLabel("Total VAr-hours Imported Q2 phase B");
        totalVarHoursImportedQ2PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseBDataPoint.setAddressOffset(104);
        totalVarHoursImportedQ2PhaseBDataPoint.setBlockOffset(102);
        totalVarHoursImportedQ2PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseBDataPoint.name(), totalVarHoursImportedQ2PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseCDataPoint;
        totalVarHoursImportedQ2PhaseCDataPoint.setName("TotVArhImpQ2phC");
        totalVarHoursImportedQ2PhaseCDataPoint.setLabel("Total VAr-hours Imported Q2 phase C");
        totalVarHoursImportedQ2PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseCDataPoint.setAddressOffset(106);
        totalVarHoursImportedQ2PhaseCDataPoint.setBlockOffset(104);
        totalVarHoursImportedQ2PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseCDataPoint.name(), totalVarHoursImportedQ2PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3DataPoint;
        totalVarHoursExportedQ3DataPoint.setName("TotVArhExpQ3");
        totalVarHoursExportedQ3DataPoint.setLabel("Total VAr-hours Exported Q3");
        totalVarHoursExportedQ3DataPoint.setDescription("Total Reactive Power Exported Quadrant 3");
        totalVarHoursExportedQ3DataPoint.setUnits("varh");
        totalVarHoursExportedQ3DataPoint.setSize(2);
        totalVarHoursExportedQ3DataPoint.setAddressOffset(108);
        totalVarHoursExportedQ3DataPoint.setBlockOffset(106);
        totalVarHoursExportedQ3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ3DataPoint.name(), totalVarHoursExportedQ3DataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseADataPoint;
        totalVarHoursExportedQ3PhaseADataPoint.setName("TotVArhExpQ3phA");
        totalVarHoursExportedQ3PhaseADataPoint.setLabel("Total VAr-hours Exported Q3 phase A");
        totalVarHoursExportedQ3PhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseADataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseADataPoint.setAddressOffset(110);
        totalVarHoursExportedQ3PhaseADataPoint.setBlockOffset(108);
        totalVarHoursExportedQ3PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseADataPoint.name(), totalVarHoursExportedQ3PhaseADataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseBDataPoint;
        totalVarHoursExportedQ3PhaseBDataPoint.setName("TotVArhExpQ3phB");
        totalVarHoursExportedQ3PhaseBDataPoint.setLabel("Total VAr-hours Exported Q3 phase B");
        totalVarHoursExportedQ3PhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseBDataPoint.setAddressOffset(112);
        totalVarHoursExportedQ3PhaseBDataPoint.setBlockOffset(110);
        totalVarHoursExportedQ3PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseBDataPoint.name(), totalVarHoursExportedQ3PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseCDataPoint;
        totalVarHoursExportedQ3PhaseCDataPoint.setName("TotVArhExpQ3phC");
        totalVarHoursExportedQ3PhaseCDataPoint.setLabel("Total VAr-hours Exported Q3 phase C");
        totalVarHoursExportedQ3PhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseCDataPoint.setAddressOffset(114);
        totalVarHoursExportedQ3PhaseCDataPoint.setBlockOffset(112);
        totalVarHoursExportedQ3PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseCDataPoint.name(), totalVarHoursExportedQ3PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4DataPoint;
        totalVarHoursExportedQ4DataPoint.setName("TotVArhExpQ4");
        totalVarHoursExportedQ4DataPoint.setLabel("Total VAr-hours Exported Q4");
        totalVarHoursExportedQ4DataPoint.setDescription("Total Reactive Power Exported Quadrant 4");
        totalVarHoursExportedQ4DataPoint.setUnits("varh");
        totalVarHoursExportedQ4DataPoint.setSize(2);
        totalVarHoursExportedQ4DataPoint.setAddressOffset(116);
        totalVarHoursExportedQ4DataPoint.setBlockOffset(114);
        totalVarHoursExportedQ4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ4DataPoint.name(), totalVarHoursExportedQ4DataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseADataPoint;
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setName("TotVArhExpQ4phA");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase A");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setAddressOffset(118);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setBlockOffset(116);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseADataPoint.name(), totalVarHoursExportedQ4ImportedPhaseADataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseBDataPoint;
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setName("TotVArhExpQ4phB");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase B");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setAddressOffset(120);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setBlockOffset(118);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseBDataPoint.name(), totalVarHoursExportedQ4ImportedPhaseBDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseCDataPoint;
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setName("TotVArhExpQ4phC");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase C");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setAddressOffset(122);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setBlockOffset(120);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseCDataPoint.name(), totalVarHoursExportedQ4ImportedPhaseCDataPoint);

        SunSpecDataPoint eventsDataPoint;
        eventsDataPoint.setName("Evt");
        eventsDataPoint.setLabel("Events");
        eventsDataPoint.setDescription("Meter Event Flags");
        eventsDataPoint.setMandatory(true);
        eventsDataPoint.setSize(2);
        eventsDataPoint.setAddressOffset(124);
        eventsDataPoint.setBlockOffset(122);
        eventsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(eventsDataPoint.name(), eventsDataPoint);

        break;
    }
    case 214: {
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
        ampsDataPoint.setDescription("Total AC Current");
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

        SunSpecDataPoint voltageLnDataPoint;
        voltageLnDataPoint.setName("PhV");
        voltageLnDataPoint.setLabel("Voltage LN");
        voltageLnDataPoint.setDescription("Line to Neutral AC Voltage (average of active phases)");
        voltageLnDataPoint.setUnits("V");
        voltageLnDataPoint.setSize(2);
        voltageLnDataPoint.setAddressOffset(10);
        voltageLnDataPoint.setBlockOffset(8);
        voltageLnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(voltageLnDataPoint.name(), voltageLnDataPoint);

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
        phaseVoltageAnDataPoint.setSize(2);
        phaseVoltageAnDataPoint.setAddressOffset(12);
        phaseVoltageAnDataPoint.setBlockOffset(10);
        phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(phaseVoltageAnDataPoint.name(), phaseVoltageAnDataPoint);

        SunSpecDataPoint phaseVoltageBnDataPoint;
        phaseVoltageBnDataPoint.setName("PhVphB");
        phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
        phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
        phaseVoltageBnDataPoint.setUnits("V");
        phaseVoltageBnDataPoint.setSize(2);
        phaseVoltageBnDataPoint.setAddressOffset(14);
        phaseVoltageBnDataPoint.setBlockOffset(12);
        phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(phaseVoltageBnDataPoint.name(), phaseVoltageBnDataPoint);

        SunSpecDataPoint phaseVoltageCnDataPoint;
        phaseVoltageCnDataPoint.setName("PhVphC");
        phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
        phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
        phaseVoltageCnDataPoint.setUnits("V");
        phaseVoltageCnDataPoint.setSize(2);
        phaseVoltageCnDataPoint.setAddressOffset(16);
        phaseVoltageCnDataPoint.setBlockOffset(14);
        phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(phaseVoltageCnDataPoint.name(), phaseVoltageCnDataPoint);

        SunSpecDataPoint voltageLlDataPoint;
        voltageLlDataPoint.setName("PPV");
        voltageLlDataPoint.setLabel("Voltage LL");
        voltageLlDataPoint.setDescription("Line to Line AC Voltage (average of active phases)");
        voltageLlDataPoint.setUnits("V");
        voltageLlDataPoint.setMandatory(true);
        voltageLlDataPoint.setSize(2);
        voltageLlDataPoint.setAddressOffset(18);
        voltageLlDataPoint.setBlockOffset(16);
        voltageLlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(voltageLlDataPoint.name(), voltageLlDataPoint);

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PPVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setMandatory(true);
        phaseVoltageAbDataPoint.setSize(2);
        phaseVoltageAbDataPoint.setAddressOffset(20);
        phaseVoltageAbDataPoint.setBlockOffset(18);
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(phaseVoltageAbDataPoint.name(), phaseVoltageAbDataPoint);

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PPVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setMandatory(true);
        phaseVoltageBcDataPoint.setSize(2);
        phaseVoltageBcDataPoint.setAddressOffset(22);
        phaseVoltageBcDataPoint.setBlockOffset(20);
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(phaseVoltageBcDataPoint.name(), phaseVoltageBcDataPoint);

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PPVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setMandatory(true);
        phaseVoltageCaDataPoint.setSize(2);
        phaseVoltageCaDataPoint.setAddressOffset(24);
        phaseVoltageCaDataPoint.setBlockOffset(22);
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
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
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
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
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(wattsDataPoint.name(), wattsDataPoint);

        SunSpecDataPoint wattsPhaseADataPoint;
        wattsPhaseADataPoint.setName("WphA");
        wattsPhaseADataPoint.setLabel("Watts phase A");
        wattsPhaseADataPoint.setUnits("W");
        wattsPhaseADataPoint.setSize(2);
        wattsPhaseADataPoint.setAddressOffset(30);
        wattsPhaseADataPoint.setBlockOffset(28);
        wattsPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(wattsPhaseADataPoint.name(), wattsPhaseADataPoint);

        SunSpecDataPoint wattsPhaseBDataPoint;
        wattsPhaseBDataPoint.setName("WphB");
        wattsPhaseBDataPoint.setLabel("Watts phase B");
        wattsPhaseBDataPoint.setUnits("W");
        wattsPhaseBDataPoint.setSize(2);
        wattsPhaseBDataPoint.setAddressOffset(32);
        wattsPhaseBDataPoint.setBlockOffset(30);
        wattsPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(wattsPhaseBDataPoint.name(), wattsPhaseBDataPoint);

        SunSpecDataPoint wattsPhaseCDataPoint;
        wattsPhaseCDataPoint.setName("WphC");
        wattsPhaseCDataPoint.setLabel("Watts phase C");
        wattsPhaseCDataPoint.setUnits("W");
        wattsPhaseCDataPoint.setSize(2);
        wattsPhaseCDataPoint.setAddressOffset(34);
        wattsPhaseCDataPoint.setBlockOffset(32);
        wattsPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(wattsPhaseCDataPoint.name(), wattsPhaseCDataPoint);

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(2);
        vaDataPoint.setAddressOffset(36);
        vaDataPoint.setBlockOffset(34);
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(vaDataPoint.name(), vaDataPoint);

        SunSpecDataPoint vaPhaseADataPoint;
        vaPhaseADataPoint.setName("VAphA");
        vaPhaseADataPoint.setLabel("VA phase A");
        vaPhaseADataPoint.setUnits("VA");
        vaPhaseADataPoint.setSize(2);
        vaPhaseADataPoint.setAddressOffset(38);
        vaPhaseADataPoint.setBlockOffset(36);
        vaPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(vaPhaseADataPoint.name(), vaPhaseADataPoint);

        SunSpecDataPoint vaPhaseBDataPoint;
        vaPhaseBDataPoint.setName("VAphB");
        vaPhaseBDataPoint.setLabel("VA phase B");
        vaPhaseBDataPoint.setUnits("VA");
        vaPhaseBDataPoint.setSize(2);
        vaPhaseBDataPoint.setAddressOffset(40);
        vaPhaseBDataPoint.setBlockOffset(38);
        vaPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(vaPhaseBDataPoint.name(), vaPhaseBDataPoint);

        SunSpecDataPoint vaPhaseCDataPoint;
        vaPhaseCDataPoint.setName("VAphC");
        vaPhaseCDataPoint.setLabel("VA phase C");
        vaPhaseCDataPoint.setUnits("VA");
        vaPhaseCDataPoint.setSize(2);
        vaPhaseCDataPoint.setAddressOffset(42);
        vaPhaseCDataPoint.setBlockOffset(40);
        vaPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(vaPhaseCDataPoint.name(), vaPhaseCDataPoint);

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAR");
        varDataPoint.setLabel("VAR");
        varDataPoint.setDescription("Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(2);
        varDataPoint.setAddressOffset(44);
        varDataPoint.setBlockOffset(42);
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(varDataPoint.name(), varDataPoint);

        SunSpecDataPoint varPhaseADataPoint;
        varPhaseADataPoint.setName("VARphA");
        varPhaseADataPoint.setLabel("VAR phase A");
        varPhaseADataPoint.setUnits("var");
        varPhaseADataPoint.setSize(2);
        varPhaseADataPoint.setAddressOffset(46);
        varPhaseADataPoint.setBlockOffset(44);
        varPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(varPhaseADataPoint.name(), varPhaseADataPoint);

        SunSpecDataPoint varPhaseBDataPoint;
        varPhaseBDataPoint.setName("VARphB");
        varPhaseBDataPoint.setLabel("VAR phase B");
        varPhaseBDataPoint.setUnits("var");
        varPhaseBDataPoint.setSize(2);
        varPhaseBDataPoint.setAddressOffset(48);
        varPhaseBDataPoint.setBlockOffset(46);
        varPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(varPhaseBDataPoint.name(), varPhaseBDataPoint);

        SunSpecDataPoint varPhaseCDataPoint;
        varPhaseCDataPoint.setName("VARphC");
        varPhaseCDataPoint.setLabel("VAR phase C");
        varPhaseCDataPoint.setUnits("var");
        varPhaseCDataPoint.setSize(2);
        varPhaseCDataPoint.setAddressOffset(50);
        varPhaseCDataPoint.setBlockOffset(48);
        varPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(varPhaseCDataPoint.name(), varPhaseCDataPoint);

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("Power Factor");
        pfDataPoint.setUnits("PF");
        pfDataPoint.setSize(2);
        pfDataPoint.setAddressOffset(52);
        pfDataPoint.setBlockOffset(50);
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(pfDataPoint.name(), pfDataPoint);

        SunSpecDataPoint pfPhaseADataPoint;
        pfPhaseADataPoint.setName("PFphA");
        pfPhaseADataPoint.setLabel("PF phase A");
        pfPhaseADataPoint.setUnits("PF");
        pfPhaseADataPoint.setSize(2);
        pfPhaseADataPoint.setAddressOffset(54);
        pfPhaseADataPoint.setBlockOffset(52);
        pfPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(pfPhaseADataPoint.name(), pfPhaseADataPoint);

        SunSpecDataPoint pfPhaseBDataPoint;
        pfPhaseBDataPoint.setName("PFphB");
        pfPhaseBDataPoint.setLabel("PF phase B");
        pfPhaseBDataPoint.setUnits("PF");
        pfPhaseBDataPoint.setSize(2);
        pfPhaseBDataPoint.setAddressOffset(56);
        pfPhaseBDataPoint.setBlockOffset(54);
        pfPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(pfPhaseBDataPoint.name(), pfPhaseBDataPoint);

        SunSpecDataPoint pfPhaseCDataPoint;
        pfPhaseCDataPoint.setName("PFphC");
        pfPhaseCDataPoint.setLabel("PF phase C");
        pfPhaseCDataPoint.setUnits("PF");
        pfPhaseCDataPoint.setSize(2);
        pfPhaseCDataPoint.setAddressOffset(58);
        pfPhaseCDataPoint.setBlockOffset(56);
        pfPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
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
        totalWattHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursExportedDataPoint.name(), totalWattHoursExportedDataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseADataPoint;
        totalWattHoursExportedPhaseADataPoint.setName("TotWhExpPhA");
        totalWattHoursExportedPhaseADataPoint.setLabel("Total Watt-hours Exported phase A");
        totalWattHoursExportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseADataPoint.setSize(2);
        totalWattHoursExportedPhaseADataPoint.setAddressOffset(62);
        totalWattHoursExportedPhaseADataPoint.setBlockOffset(60);
        totalWattHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursExportedPhaseADataPoint.name(), totalWattHoursExportedPhaseADataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseBDataPoint;
        totalWattHoursExportedPhaseBDataPoint.setName("TotWhExpPhB");
        totalWattHoursExportedPhaseBDataPoint.setLabel("Total Watt-hours Exported phase B");
        totalWattHoursExportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseBDataPoint.setSize(2);
        totalWattHoursExportedPhaseBDataPoint.setAddressOffset(64);
        totalWattHoursExportedPhaseBDataPoint.setBlockOffset(62);
        totalWattHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursExportedPhaseBDataPoint.name(), totalWattHoursExportedPhaseBDataPoint);

        SunSpecDataPoint totalWattHoursExportedPhaseCDataPoint;
        totalWattHoursExportedPhaseCDataPoint.setName("TotWhExpPhC");
        totalWattHoursExportedPhaseCDataPoint.setLabel("Total Watt-hours Exported phase C");
        totalWattHoursExportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseCDataPoint.setSize(2);
        totalWattHoursExportedPhaseCDataPoint.setAddressOffset(66);
        totalWattHoursExportedPhaseCDataPoint.setBlockOffset(64);
        totalWattHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
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
        totalWattHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursImportedDataPoint.name(), totalWattHoursImportedDataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseADataPoint;
        totalWattHoursImportedPhaseADataPoint.setName("TotWhImpPhA");
        totalWattHoursImportedPhaseADataPoint.setLabel("Total Watt-hours Imported phase A");
        totalWattHoursImportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseADataPoint.setSize(2);
        totalWattHoursImportedPhaseADataPoint.setAddressOffset(70);
        totalWattHoursImportedPhaseADataPoint.setBlockOffset(68);
        totalWattHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseADataPoint.name(), totalWattHoursImportedPhaseADataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseBDataPoint;
        totalWattHoursImportedPhaseBDataPoint.setName("TotWhImpPhB");
        totalWattHoursImportedPhaseBDataPoint.setLabel("Total Watt-hours Imported phase B");
        totalWattHoursImportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseBDataPoint.setSize(2);
        totalWattHoursImportedPhaseBDataPoint.setAddressOffset(72);
        totalWattHoursImportedPhaseBDataPoint.setBlockOffset(70);
        totalWattHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseBDataPoint.name(), totalWattHoursImportedPhaseBDataPoint);

        SunSpecDataPoint totalWattHoursImportedPhaseCDataPoint;
        totalWattHoursImportedPhaseCDataPoint.setName("TotWhImpPhC");
        totalWattHoursImportedPhaseCDataPoint.setLabel("Total Watt-hours Imported phase C");
        totalWattHoursImportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseCDataPoint.setSize(2);
        totalWattHoursImportedPhaseCDataPoint.setAddressOffset(74);
        totalWattHoursImportedPhaseCDataPoint.setBlockOffset(72);
        totalWattHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalWattHoursImportedPhaseCDataPoint.name(), totalWattHoursImportedPhaseCDataPoint);

        SunSpecDataPoint totalVaHoursExportedDataPoint;
        totalVaHoursExportedDataPoint.setName("TotVAhExp");
        totalVaHoursExportedDataPoint.setLabel("Total VA-hours Exported");
        totalVaHoursExportedDataPoint.setDescription("Total Apparent Energy Exported");
        totalVaHoursExportedDataPoint.setUnits("VAh");
        totalVaHoursExportedDataPoint.setSize(2);
        totalVaHoursExportedDataPoint.setAddressOffset(76);
        totalVaHoursExportedDataPoint.setBlockOffset(74);
        totalVaHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursExportedDataPoint.name(), totalVaHoursExportedDataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseADataPoint;
        totalVaHoursExportedPhaseADataPoint.setName("TotVAhExpPhA");
        totalVaHoursExportedPhaseADataPoint.setLabel("Total VA-hours Exported phase A");
        totalVaHoursExportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseADataPoint.setSize(2);
        totalVaHoursExportedPhaseADataPoint.setAddressOffset(78);
        totalVaHoursExportedPhaseADataPoint.setBlockOffset(76);
        totalVaHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseADataPoint.name(), totalVaHoursExportedPhaseADataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseBDataPoint;
        totalVaHoursExportedPhaseBDataPoint.setName("TotVAhExpPhB");
        totalVaHoursExportedPhaseBDataPoint.setLabel("Total VA-hours Exported phase B");
        totalVaHoursExportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseBDataPoint.setSize(2);
        totalVaHoursExportedPhaseBDataPoint.setAddressOffset(80);
        totalVaHoursExportedPhaseBDataPoint.setBlockOffset(78);
        totalVaHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseBDataPoint.name(), totalVaHoursExportedPhaseBDataPoint);

        SunSpecDataPoint totalVaHoursExportedPhaseCDataPoint;
        totalVaHoursExportedPhaseCDataPoint.setName("TotVAhExpPhC");
        totalVaHoursExportedPhaseCDataPoint.setLabel("Total VA-hours Exported phase C");
        totalVaHoursExportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseCDataPoint.setSize(2);
        totalVaHoursExportedPhaseCDataPoint.setAddressOffset(82);
        totalVaHoursExportedPhaseCDataPoint.setBlockOffset(80);
        totalVaHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursExportedPhaseCDataPoint.name(), totalVaHoursExportedPhaseCDataPoint);

        SunSpecDataPoint totalVaHoursImportedDataPoint;
        totalVaHoursImportedDataPoint.setName("TotVAhImp");
        totalVaHoursImportedDataPoint.setLabel("Total VA-hours Imported");
        totalVaHoursImportedDataPoint.setDescription("Total Apparent Energy Imported");
        totalVaHoursImportedDataPoint.setUnits("VAh");
        totalVaHoursImportedDataPoint.setSize(2);
        totalVaHoursImportedDataPoint.setAddressOffset(84);
        totalVaHoursImportedDataPoint.setBlockOffset(82);
        totalVaHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursImportedDataPoint.name(), totalVaHoursImportedDataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseADataPoint;
        totalVaHoursImportedPhaseADataPoint.setName("TotVAhImpPhA");
        totalVaHoursImportedPhaseADataPoint.setLabel("Total VA-hours Imported phase A");
        totalVaHoursImportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseADataPoint.setSize(2);
        totalVaHoursImportedPhaseADataPoint.setAddressOffset(86);
        totalVaHoursImportedPhaseADataPoint.setBlockOffset(84);
        totalVaHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseADataPoint.name(), totalVaHoursImportedPhaseADataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseBDataPoint;
        totalVaHoursImportedPhaseBDataPoint.setName("TotVAhImpPhB");
        totalVaHoursImportedPhaseBDataPoint.setLabel("Total VA-hours Imported phase B");
        totalVaHoursImportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseBDataPoint.setSize(2);
        totalVaHoursImportedPhaseBDataPoint.setAddressOffset(88);
        totalVaHoursImportedPhaseBDataPoint.setBlockOffset(86);
        totalVaHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseBDataPoint.name(), totalVaHoursImportedPhaseBDataPoint);

        SunSpecDataPoint totalVaHoursImportedPhaseCDataPoint;
        totalVaHoursImportedPhaseCDataPoint.setName("TotVAhImpPhC");
        totalVaHoursImportedPhaseCDataPoint.setLabel("Total VA-hours Imported phase C");
        totalVaHoursImportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseCDataPoint.setSize(2);
        totalVaHoursImportedPhaseCDataPoint.setAddressOffset(90);
        totalVaHoursImportedPhaseCDataPoint.setBlockOffset(88);
        totalVaHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVaHoursImportedPhaseCDataPoint.name(), totalVaHoursImportedPhaseCDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1DataPoint;
        totalVarHoursImportedQ1DataPoint.setName("TotVArhImpQ1");
        totalVarHoursImportedQ1DataPoint.setLabel("Total VAR-hours Imported Q1");
        totalVarHoursImportedQ1DataPoint.setDescription("Total Reactive Energy Imported Quadrant 1");
        totalVarHoursImportedQ1DataPoint.setUnits("varh");
        totalVarHoursImportedQ1DataPoint.setSize(2);
        totalVarHoursImportedQ1DataPoint.setAddressOffset(92);
        totalVarHoursImportedQ1DataPoint.setBlockOffset(90);
        totalVarHoursImportedQ1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ1DataPoint.name(), totalVarHoursImportedQ1DataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseADataPoint;
        totalVarHoursImportedQ1PhaseADataPoint.setName("TotVArhImpQ1phA");
        totalVarHoursImportedQ1PhaseADataPoint.setLabel("Total VAr-hours Imported Q1 phase A");
        totalVarHoursImportedQ1PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseADataPoint.setAddressOffset(94);
        totalVarHoursImportedQ1PhaseADataPoint.setBlockOffset(92);
        totalVarHoursImportedQ1PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseADataPoint.name(), totalVarHoursImportedQ1PhaseADataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseBDataPoint;
        totalVarHoursImportedQ1PhaseBDataPoint.setName("TotVArhImpQ1phB");
        totalVarHoursImportedQ1PhaseBDataPoint.setLabel("Total VAr-hours Imported Q1 phase B");
        totalVarHoursImportedQ1PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseBDataPoint.setAddressOffset(96);
        totalVarHoursImportedQ1PhaseBDataPoint.setBlockOffset(94);
        totalVarHoursImportedQ1PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseBDataPoint.name(), totalVarHoursImportedQ1PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1PhaseCDataPoint;
        totalVarHoursImportedQ1PhaseCDataPoint.setName("TotVArhImpQ1phC");
        totalVarHoursImportedQ1PhaseCDataPoint.setLabel("Total VAr-hours Imported Q1 phase C");
        totalVarHoursImportedQ1PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseCDataPoint.setAddressOffset(98);
        totalVarHoursImportedQ1PhaseCDataPoint.setBlockOffset(96);
        totalVarHoursImportedQ1PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ1PhaseCDataPoint.name(), totalVarHoursImportedQ1PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2DataPoint;
        totalVarHoursImportedQ2DataPoint.setName("TotVArhImpQ2");
        totalVarHoursImportedQ2DataPoint.setLabel("Total VAr-hours Imported Q2");
        totalVarHoursImportedQ2DataPoint.setDescription("Total Reactive Power Imported Quadrant 2");
        totalVarHoursImportedQ2DataPoint.setUnits("varh");
        totalVarHoursImportedQ2DataPoint.setSize(2);
        totalVarHoursImportedQ2DataPoint.setAddressOffset(100);
        totalVarHoursImportedQ2DataPoint.setBlockOffset(98);
        totalVarHoursImportedQ2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ2DataPoint.name(), totalVarHoursImportedQ2DataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseADataPoint;
        totalVarHoursImportedQ2PhaseADataPoint.setName("TotVArhImpQ2phA");
        totalVarHoursImportedQ2PhaseADataPoint.setLabel("Total VAr-hours Imported Q2 phase A");
        totalVarHoursImportedQ2PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseADataPoint.setAddressOffset(102);
        totalVarHoursImportedQ2PhaseADataPoint.setBlockOffset(100);
        totalVarHoursImportedQ2PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseADataPoint.name(), totalVarHoursImportedQ2PhaseADataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseBDataPoint;
        totalVarHoursImportedQ2PhaseBDataPoint.setName("TotVArhImpQ2phB");
        totalVarHoursImportedQ2PhaseBDataPoint.setLabel("Total VAr-hours Imported Q2 phase B");
        totalVarHoursImportedQ2PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseBDataPoint.setAddressOffset(104);
        totalVarHoursImportedQ2PhaseBDataPoint.setBlockOffset(102);
        totalVarHoursImportedQ2PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseBDataPoint.name(), totalVarHoursImportedQ2PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2PhaseCDataPoint;
        totalVarHoursImportedQ2PhaseCDataPoint.setName("TotVArhImpQ2phC");
        totalVarHoursImportedQ2PhaseCDataPoint.setLabel("Total VAr-hours Imported Q2 phase C");
        totalVarHoursImportedQ2PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseCDataPoint.setAddressOffset(106);
        totalVarHoursImportedQ2PhaseCDataPoint.setBlockOffset(104);
        totalVarHoursImportedQ2PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursImportedQ2PhaseCDataPoint.name(), totalVarHoursImportedQ2PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3DataPoint;
        totalVarHoursExportedQ3DataPoint.setName("TotVArhExpQ3");
        totalVarHoursExportedQ3DataPoint.setLabel("Total VAr-hours Exported Q3");
        totalVarHoursExportedQ3DataPoint.setDescription("Total Reactive Power Exported Quadrant 3");
        totalVarHoursExportedQ3DataPoint.setUnits("varh");
        totalVarHoursExportedQ3DataPoint.setSize(2);
        totalVarHoursExportedQ3DataPoint.setAddressOffset(108);
        totalVarHoursExportedQ3DataPoint.setBlockOffset(106);
        totalVarHoursExportedQ3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ3DataPoint.name(), totalVarHoursExportedQ3DataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseADataPoint;
        totalVarHoursExportedQ3PhaseADataPoint.setName("TotVArhExpQ3phA");
        totalVarHoursExportedQ3PhaseADataPoint.setLabel("Total VAr-hours Exported Q3 phase A");
        totalVarHoursExportedQ3PhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseADataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseADataPoint.setAddressOffset(110);
        totalVarHoursExportedQ3PhaseADataPoint.setBlockOffset(108);
        totalVarHoursExportedQ3PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseADataPoint.name(), totalVarHoursExportedQ3PhaseADataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseBDataPoint;
        totalVarHoursExportedQ3PhaseBDataPoint.setName("TotVArhExpQ3phB");
        totalVarHoursExportedQ3PhaseBDataPoint.setLabel("Total VAr-hours Exported Q3 phase B");
        totalVarHoursExportedQ3PhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseBDataPoint.setAddressOffset(112);
        totalVarHoursExportedQ3PhaseBDataPoint.setBlockOffset(110);
        totalVarHoursExportedQ3PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseBDataPoint.name(), totalVarHoursExportedQ3PhaseBDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3PhaseCDataPoint;
        totalVarHoursExportedQ3PhaseCDataPoint.setName("TotVArhExpQ3phC");
        totalVarHoursExportedQ3PhaseCDataPoint.setLabel("Total VAr-hours Exported Q3 phase C");
        totalVarHoursExportedQ3PhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseCDataPoint.setAddressOffset(114);
        totalVarHoursExportedQ3PhaseCDataPoint.setBlockOffset(112);
        totalVarHoursExportedQ3PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ3PhaseCDataPoint.name(), totalVarHoursExportedQ3PhaseCDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4DataPoint;
        totalVarHoursExportedQ4DataPoint.setName("TotVArhExpQ4");
        totalVarHoursExportedQ4DataPoint.setLabel("Total VAr-hours Exported Q4");
        totalVarHoursExportedQ4DataPoint.setDescription("Total Reactive Power Exported Quadrant 4");
        totalVarHoursExportedQ4DataPoint.setUnits("varh");
        totalVarHoursExportedQ4DataPoint.setSize(2);
        totalVarHoursExportedQ4DataPoint.setAddressOffset(116);
        totalVarHoursExportedQ4DataPoint.setBlockOffset(114);
        totalVarHoursExportedQ4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ4DataPoint.name(), totalVarHoursExportedQ4DataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseADataPoint;
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setName("TotVArhExpQ4phA");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase A");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setAddressOffset(118);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setBlockOffset(116);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseADataPoint.name(), totalVarHoursExportedQ4ImportedPhaseADataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseBDataPoint;
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setName("TotVArhExpQ4phB");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase B");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setAddressOffset(120);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setBlockOffset(118);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseBDataPoint.name(), totalVarHoursExportedQ4ImportedPhaseBDataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseCDataPoint;
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setName("TotVArhExpQ4phC");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase C");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setAddressOffset(122);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setBlockOffset(120);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints.insert(totalVarHoursExportedQ4ImportedPhaseCDataPoint.name(), totalVarHoursExportedQ4ImportedPhaseCDataPoint);

        SunSpecDataPoint eventsDataPoint;
        eventsDataPoint.setName("Evt");
        eventsDataPoint.setLabel("Events");
        eventsDataPoint.setDescription("Meter Event Flags");
        eventsDataPoint.setMandatory(true);
        eventsDataPoint.setSize(2);
        eventsDataPoint.setAddressOffset(124);
        eventsDataPoint.setBlockOffset(122);
        eventsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(eventsDataPoint.name(), eventsDataPoint);

        break;
    }
    case 220: {
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
        ampsDataPoint.setDescription("Total AC Current");
        ampsDataPoint.setUnits("A");
        ampsDataPoint.setMandatory(true);
        ampsDataPoint.setSize(1);
        ampsDataPoint.setAddressOffset(2);
        ampsDataPoint.setBlockOffset(0);
        ampsDataPoint.setScaleFactorName("A_SF");
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(ampsDataPoint.name(), ampsDataPoint);

        SunSpecDataPoint A_SFDataPoint;
        A_SFDataPoint.setName("A_SF");
        A_SFDataPoint.setDescription("Current scale factor");
        A_SFDataPoint.setMandatory(true);
        A_SFDataPoint.setSize(1);
        A_SFDataPoint.setAddressOffset(3);
        A_SFDataPoint.setBlockOffset(1);
        A_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(A_SFDataPoint.name(), A_SFDataPoint);

        SunSpecDataPoint voltageDataPoint;
        voltageDataPoint.setName("PhV");
        voltageDataPoint.setLabel("Voltage");
        voltageDataPoint.setDescription("Average phase or line voltage");
        voltageDataPoint.setUnits("V");
        voltageDataPoint.setSize(1);
        voltageDataPoint.setAddressOffset(4);
        voltageDataPoint.setBlockOffset(2);
        voltageDataPoint.setScaleFactorName("V_SF");
        voltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(voltageDataPoint.name(), voltageDataPoint);

        SunSpecDataPoint V_SFDataPoint;
        V_SFDataPoint.setName("V_SF");
        V_SFDataPoint.setDescription("Voltage scale factor");
        V_SFDataPoint.setMandatory(true);
        V_SFDataPoint.setSize(1);
        V_SFDataPoint.setAddressOffset(5);
        V_SFDataPoint.setBlockOffset(3);
        V_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(V_SFDataPoint.name(), V_SFDataPoint);

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(1);
        hzDataPoint.setAddressOffset(6);
        hzDataPoint.setBlockOffset(4);
        hzDataPoint.setScaleFactorName("Hz_SF");
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(hzDataPoint.name(), hzDataPoint);

        SunSpecDataPoint Hz_SFDataPoint;
        Hz_SFDataPoint.setName("Hz_SF");
        Hz_SFDataPoint.setDescription("Frequency scale factor");
        Hz_SFDataPoint.setSize(1);
        Hz_SFDataPoint.setAddressOffset(7);
        Hz_SFDataPoint.setBlockOffset(5);
        Hz_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(Hz_SFDataPoint.name(), Hz_SFDataPoint);

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("Total Real Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(1);
        wattsDataPoint.setAddressOffset(8);
        wattsDataPoint.setBlockOffset(6);
        wattsDataPoint.setScaleFactorName("W_SF");
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(wattsDataPoint.name(), wattsDataPoint);

        SunSpecDataPoint W_SFDataPoint;
        W_SFDataPoint.setName("W_SF");
        W_SFDataPoint.setDescription("Real Power scale factor");
        W_SFDataPoint.setMandatory(true);
        W_SFDataPoint.setSize(1);
        W_SFDataPoint.setAddressOffset(9);
        W_SFDataPoint.setBlockOffset(7);
        W_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(W_SFDataPoint.name(), W_SFDataPoint);

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(1);
        vaDataPoint.setAddressOffset(10);
        vaDataPoint.setBlockOffset(8);
        vaDataPoint.setScaleFactorName("VA_SF");
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(vaDataPoint.name(), vaDataPoint);

        SunSpecDataPoint VA_SFDataPoint;
        VA_SFDataPoint.setName("VA_SF");
        VA_SFDataPoint.setDescription("Apparent Power scale factor");
        VA_SFDataPoint.setSize(1);
        VA_SFDataPoint.setAddressOffset(11);
        VA_SFDataPoint.setBlockOffset(9);
        VA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(VA_SFDataPoint.name(), VA_SFDataPoint);

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAR");
        varDataPoint.setLabel("VAR");
        varDataPoint.setDescription("Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(1);
        varDataPoint.setAddressOffset(12);
        varDataPoint.setBlockOffset(10);
        varDataPoint.setScaleFactorName("VAR_SF");
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(varDataPoint.name(), varDataPoint);

        SunSpecDataPoint VAR_SFDataPoint;
        VAR_SFDataPoint.setName("VAR_SF");
        VAR_SFDataPoint.setDescription("Reactive Power scale factor");
        VAR_SFDataPoint.setSize(1);
        VAR_SFDataPoint.setAddressOffset(13);
        VAR_SFDataPoint.setBlockOffset(11);
        VAR_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(VAR_SFDataPoint.name(), VAR_SFDataPoint);

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("Power Factor");
        pfDataPoint.setUnits("Pct");
        pfDataPoint.setSize(1);
        pfDataPoint.setAddressOffset(14);
        pfDataPoint.setBlockOffset(12);
        pfDataPoint.setScaleFactorName("PF_SF");
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints.insert(pfDataPoint.name(), pfDataPoint);

        SunSpecDataPoint PF_SFDataPoint;
        PF_SFDataPoint.setName("PF_SF");
        PF_SFDataPoint.setDescription("Power Factor scale factor");
        PF_SFDataPoint.setSize(1);
        PF_SFDataPoint.setAddressOffset(15);
        PF_SFDataPoint.setBlockOffset(13);
        PF_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(PF_SFDataPoint.name(), PF_SFDataPoint);

        SunSpecDataPoint totalWattHoursExportedDataPoint;
        totalWattHoursExportedDataPoint.setName("TotWhExp");
        totalWattHoursExportedDataPoint.setLabel("Total Watt-hours Exported");
        totalWattHoursExportedDataPoint.setDescription("Total Real Energy Exported");
        totalWattHoursExportedDataPoint.setUnits("Wh");
        totalWattHoursExportedDataPoint.setMandatory(true);
        totalWattHoursExportedDataPoint.setSize(2);
        totalWattHoursExportedDataPoint.setAddressOffset(16);
        totalWattHoursExportedDataPoint.setBlockOffset(14);
        totalWattHoursExportedDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursExportedDataPoint.name(), totalWattHoursExportedDataPoint);

        SunSpecDataPoint totalWattHoursImportedDataPoint;
        totalWattHoursImportedDataPoint.setName("TotWhImp");
        totalWattHoursImportedDataPoint.setLabel("Total Watt-hours Imported");
        totalWattHoursImportedDataPoint.setDescription("Total Real Energy Imported");
        totalWattHoursImportedDataPoint.setUnits("Wh");
        totalWattHoursImportedDataPoint.setMandatory(true);
        totalWattHoursImportedDataPoint.setSize(2);
        totalWattHoursImportedDataPoint.setAddressOffset(18);
        totalWattHoursImportedDataPoint.setBlockOffset(16);
        totalWattHoursImportedDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalWattHoursImportedDataPoint.name(), totalWattHoursImportedDataPoint);

        SunSpecDataPoint TotWh_SFDataPoint;
        TotWh_SFDataPoint.setName("TotWh_SF");
        TotWh_SFDataPoint.setDescription("Real Energy scale factor");
        TotWh_SFDataPoint.setMandatory(true);
        TotWh_SFDataPoint.setSize(1);
        TotWh_SFDataPoint.setAddressOffset(20);
        TotWh_SFDataPoint.setBlockOffset(18);
        TotWh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(TotWh_SFDataPoint.name(), TotWh_SFDataPoint);

        SunSpecDataPoint totalVaHoursExportedDataPoint;
        totalVaHoursExportedDataPoint.setName("TotVAhExp");
        totalVaHoursExportedDataPoint.setLabel("Total VA-hours Exported");
        totalVaHoursExportedDataPoint.setDescription("Total Apparent Energy Exported");
        totalVaHoursExportedDataPoint.setUnits("VAh");
        totalVaHoursExportedDataPoint.setSize(2);
        totalVaHoursExportedDataPoint.setAddressOffset(21);
        totalVaHoursExportedDataPoint.setBlockOffset(19);
        totalVaHoursExportedDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursExportedDataPoint.name(), totalVaHoursExportedDataPoint);

        SunSpecDataPoint totalVaHoursImportedDataPoint;
        totalVaHoursImportedDataPoint.setName("TotVAhImp");
        totalVaHoursImportedDataPoint.setLabel("Total VA-hours Imported");
        totalVaHoursImportedDataPoint.setDescription("Total Apparent Energy Imported");
        totalVaHoursImportedDataPoint.setUnits("VAh");
        totalVaHoursImportedDataPoint.setSize(2);
        totalVaHoursImportedDataPoint.setAddressOffset(23);
        totalVaHoursImportedDataPoint.setBlockOffset(21);
        totalVaHoursImportedDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVaHoursImportedDataPoint.name(), totalVaHoursImportedDataPoint);

        SunSpecDataPoint TotVAh_SFDataPoint;
        TotVAh_SFDataPoint.setName("TotVAh_SF");
        TotVAh_SFDataPoint.setDescription("Apparent Energy scale factor");
        TotVAh_SFDataPoint.setSize(1);
        TotVAh_SFDataPoint.setAddressOffset(25);
        TotVAh_SFDataPoint.setBlockOffset(23);
        TotVAh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(TotVAh_SFDataPoint.name(), TotVAh_SFDataPoint);

        SunSpecDataPoint totalVarHoursImportedQ1DataPoint;
        totalVarHoursImportedQ1DataPoint.setName("TotVArhImpQ1");
        totalVarHoursImportedQ1DataPoint.setLabel("Total VAR-hours Imported Q1");
        totalVarHoursImportedQ1DataPoint.setDescription("Total Reactive Energy Imported Quadrant 1");
        totalVarHoursImportedQ1DataPoint.setUnits("varh");
        totalVarHoursImportedQ1DataPoint.setSize(2);
        totalVarHoursImportedQ1DataPoint.setAddressOffset(26);
        totalVarHoursImportedQ1DataPoint.setBlockOffset(24);
        totalVarHoursImportedQ1DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ1DataPoint.name(), totalVarHoursImportedQ1DataPoint);

        SunSpecDataPoint totalVarHoursImportedQ2DataPoint;
        totalVarHoursImportedQ2DataPoint.setName("TotVArhImpQ2");
        totalVarHoursImportedQ2DataPoint.setLabel("Total VAr-hours Imported Q2");
        totalVarHoursImportedQ2DataPoint.setDescription("Total Reactive Power Imported Quadrant 2");
        totalVarHoursImportedQ2DataPoint.setUnits("varh");
        totalVarHoursImportedQ2DataPoint.setSize(2);
        totalVarHoursImportedQ2DataPoint.setAddressOffset(28);
        totalVarHoursImportedQ2DataPoint.setBlockOffset(26);
        totalVarHoursImportedQ2DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursImportedQ2DataPoint.name(), totalVarHoursImportedQ2DataPoint);

        SunSpecDataPoint totalVarHoursExportedQ3DataPoint;
        totalVarHoursExportedQ3DataPoint.setName("TotVArhExpQ3");
        totalVarHoursExportedQ3DataPoint.setLabel("Total VAr-hours Exported Q3");
        totalVarHoursExportedQ3DataPoint.setDescription("Total Reactive Power Exported Quadrant 3");
        totalVarHoursExportedQ3DataPoint.setUnits("varh");
        totalVarHoursExportedQ3DataPoint.setSize(2);
        totalVarHoursExportedQ3DataPoint.setAddressOffset(30);
        totalVarHoursExportedQ3DataPoint.setBlockOffset(28);
        totalVarHoursExportedQ3DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ3DataPoint.name(), totalVarHoursExportedQ3DataPoint);

        SunSpecDataPoint totalVarHoursExportedQ4DataPoint;
        totalVarHoursExportedQ4DataPoint.setName("TotVArhExpQ4");
        totalVarHoursExportedQ4DataPoint.setLabel("Total VAr-hours Exported Q4");
        totalVarHoursExportedQ4DataPoint.setDescription("Total Reactive Power Exported Quadrant 4");
        totalVarHoursExportedQ4DataPoint.setUnits("varh");
        totalVarHoursExportedQ4DataPoint.setSize(2);
        totalVarHoursExportedQ4DataPoint.setAddressOffset(32);
        totalVarHoursExportedQ4DataPoint.setBlockOffset(30);
        totalVarHoursExportedQ4DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints.insert(totalVarHoursExportedQ4DataPoint.name(), totalVarHoursExportedQ4DataPoint);

        SunSpecDataPoint TotVArh_SFDataPoint;
        TotVArh_SFDataPoint.setName("TotVArh_SF");
        TotVArh_SFDataPoint.setDescription("Reactive Energy scale factor");
        TotVArh_SFDataPoint.setSize(1);
        TotVArh_SFDataPoint.setAddressOffset(34);
        TotVArh_SFDataPoint.setBlockOffset(32);
        TotVArh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints.insert(TotVArh_SFDataPoint.name(), TotVArh_SFDataPoint);

        SunSpecDataPoint eventsDataPoint;
        eventsDataPoint.setName("Evt");
        eventsDataPoint.setLabel("Events");
        eventsDataPoint.setDescription("Meter Event Flags");
        eventsDataPoint.setMandatory(true);
        eventsDataPoint.setSize(2);
        eventsDataPoint.setAddressOffset(35);
        eventsDataPoint.setBlockOffset(33);
        eventsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints.insert(eventsDataPoint.name(), eventsDataPoint);

        SunSpecDataPoint RsrvdDataPoint;
        RsrvdDataPoint.setName("Rsrvd");
        RsrvdDataPoint.setMandatory(true);
        RsrvdDataPoint.setSize(1);
        RsrvdDataPoint.setAddressOffset(37);
        RsrvdDataPoint.setBlockOffset(35);
        RsrvdDataPoint.setDataType(SunSpecDataPoint::stringToDataType("pad"));
        m_dataPoints.insert(RsrvdDataPoint.name(), RsrvdDataPoint);

        SunSpecDataPoint timestampDataPoint;
        timestampDataPoint.setName("Ts");
        timestampDataPoint.setLabel("Timestamp");
        timestampDataPoint.setDescription("Timestamp value is the number of seconds since January 1, 2000");
        timestampDataPoint.setMandatory(true);
        timestampDataPoint.setSize(2);
        timestampDataPoint.setAddressOffset(38);
        timestampDataPoint.setBlockOffset(36);
        timestampDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint32"));
        m_dataPoints.insert(timestampDataPoint.name(), timestampDataPoint);

        SunSpecDataPoint millisecondsDataPoint;
        millisecondsDataPoint.setName("Ms");
        millisecondsDataPoint.setLabel("Milliseconds");
        millisecondsDataPoint.setDescription("Millisecond counter 0-999");
        millisecondsDataPoint.setMandatory(true);
        millisecondsDataPoint.setSize(1);
        millisecondsDataPoint.setAddressOffset(40);
        millisecondsDataPoint.setBlockOffset(38);
        millisecondsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(millisecondsDataPoint.name(), millisecondsDataPoint);

        SunSpecDataPoint sequenceDataPoint;
        sequenceDataPoint.setName("Seq");
        sequenceDataPoint.setLabel("Sequence");
        sequenceDataPoint.setDescription("Sequence number of request");
        sequenceDataPoint.setMandatory(true);
        sequenceDataPoint.setSize(1);
        sequenceDataPoint.setAddressOffset(41);
        sequenceDataPoint.setBlockOffset(39);
        sequenceDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(sequenceDataPoint.name(), sequenceDataPoint);

        SunSpecDataPoint algorithmDataPoint;
        algorithmDataPoint.setName("Alg");
        algorithmDataPoint.setLabel("Algorithm");
        algorithmDataPoint.setDescription("Algorithm used to compute the digital signature");
        algorithmDataPoint.setMandatory(true);
        algorithmDataPoint.setSize(1);
        algorithmDataPoint.setAddressOffset(42);
        algorithmDataPoint.setBlockOffset(40);
        algorithmDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints.insert(algorithmDataPoint.name(), algorithmDataPoint);

        SunSpecDataPoint nDataPoint;
        nDataPoint.setName("N");
        nDataPoint.setLabel("N");
        nDataPoint.setDescription("Number of registers comprising the digital signature.");
        nDataPoint.setMandatory(true);
        nDataPoint.setSize(1);
        nDataPoint.setAddressOffset(43);
        nDataPoint.setBlockOffset(41);
        nDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints.insert(nDataPoint.name(), nDataPoint);

        break;
    }
    default:
        break;
    }
}

