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
}

SunSpecAcMeterModel::~SunSpecAcMeterModel()
{

}

QString SunSpecAcMeterModel::name() const
{
    switch (m_modelId) {
    case 201:
        return "ac_meter";
    case 202:
        return "ac_meter";
    case 203:
        return "ac_meter";
    case 204:
        return "ac_meter";
    case 211:
        return "ac_meter";
    case 212:
        return "ac_meter";
    case 213:
        return "ac_meter";
    case 214:
        return "ac_meter";
    case 220:
        return "ac_meter";
    default:
        return QString();
    }
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
        ampsDataPoint.setDescription("Total AC Current");
        ampsDataPoint.setUnits("A");
        ampsDataPoint.setMandatory(true);
        ampsDataPoint.setSize(1);
        ampsDataPoint.setScaleFactorName("A_SF");
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << ampsDataPoint;

        SunSpecDataPoint ampsPhaseaDataPoint;
        ampsPhaseaDataPoint.setName("AphA");
        ampsPhaseaDataPoint.setLabel("Amps PhaseA");
        ampsPhaseaDataPoint.setDescription("Phase A Current");
        ampsPhaseaDataPoint.setUnits("A");
        ampsPhaseaDataPoint.setMandatory(true);
        ampsPhaseaDataPoint.setSize(1);
        ampsPhaseaDataPoint.setScaleFactorName("A_SF");
        ampsPhaseaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << ampsPhaseaDataPoint;

        SunSpecDataPoint ampsPhasebDataPoint;
        ampsPhasebDataPoint.setName("AphB");
        ampsPhasebDataPoint.setLabel("Amps PhaseB");
        ampsPhasebDataPoint.setDescription("Phase B Current");
        ampsPhasebDataPoint.setUnits("A");
        ampsPhasebDataPoint.setSize(1);
        ampsPhasebDataPoint.setScaleFactorName("A_SF");
        ampsPhasebDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << ampsPhasebDataPoint;

        SunSpecDataPoint ampsPhasecDataPoint;
        ampsPhasecDataPoint.setName("AphC");
        ampsPhasecDataPoint.setLabel("Amps PhaseC");
        ampsPhasecDataPoint.setDescription("Phase C Current");
        ampsPhasecDataPoint.setUnits("A");
        ampsPhasecDataPoint.setSize(1);
        ampsPhasecDataPoint.setScaleFactorName("A_SF");
        ampsPhasecDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << ampsPhasecDataPoint;

        SunSpecDataPoint A_SFDataPoint;
        A_SFDataPoint.setName("A_SF");
        A_SFDataPoint.setDescription("Current scale factor");
        A_SFDataPoint.setMandatory(true);
        A_SFDataPoint.setSize(1);
        A_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << A_SFDataPoint;

        SunSpecDataPoint voltageLnDataPoint;
        voltageLnDataPoint.setName("PhV");
        voltageLnDataPoint.setLabel("Voltage LN");
        voltageLnDataPoint.setDescription("Line to Neutral AC Voltage (average of active phases)");
        voltageLnDataPoint.setUnits("V");
        voltageLnDataPoint.setSize(1);
        voltageLnDataPoint.setScaleFactorName("V_SF");
        voltageLnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << voltageLnDataPoint;

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
        phaseVoltageAnDataPoint.setSize(1);
        phaseVoltageAnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageAnDataPoint;

        SunSpecDataPoint phaseVoltageBnDataPoint;
        phaseVoltageBnDataPoint.setName("PhVphB");
        phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
        phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
        phaseVoltageBnDataPoint.setUnits("V");
        phaseVoltageBnDataPoint.setSize(1);
        phaseVoltageBnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageBnDataPoint;

        SunSpecDataPoint phaseVoltageCnDataPoint;
        phaseVoltageCnDataPoint.setName("PhVphC");
        phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
        phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
        phaseVoltageCnDataPoint.setUnits("V");
        phaseVoltageCnDataPoint.setSize(1);
        phaseVoltageCnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageCnDataPoint;

        SunSpecDataPoint voltageLlDataPoint;
        voltageLlDataPoint.setName("PPV");
        voltageLlDataPoint.setLabel("Voltage LL");
        voltageLlDataPoint.setDescription("Line to Line AC Voltage (average of active phases)");
        voltageLlDataPoint.setUnits("V");
        voltageLlDataPoint.setSize(1);
        voltageLlDataPoint.setScaleFactorName("V_SF");
        voltageLlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << voltageLlDataPoint;

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PPVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setSize(1);
        phaseVoltageAbDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageAbDataPoint;

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PPVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setSize(1);
        phaseVoltageBcDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageBcDataPoint;

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PPVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setSize(1);
        phaseVoltageCaDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageCaDataPoint;

        SunSpecDataPoint V_SFDataPoint;
        V_SFDataPoint.setName("V_SF");
        V_SFDataPoint.setDescription("Voltage scale factor");
        V_SFDataPoint.setMandatory(true);
        V_SFDataPoint.setSize(1);
        V_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << V_SFDataPoint;

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(1);
        hzDataPoint.setScaleFactorName("Hz_SF");
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << hzDataPoint;

        SunSpecDataPoint Hz_SFDataPoint;
        Hz_SFDataPoint.setName("Hz_SF");
        Hz_SFDataPoint.setDescription("Frequency scale factor");
        Hz_SFDataPoint.setSize(1);
        Hz_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << Hz_SFDataPoint;

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("Total Real Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(1);
        wattsDataPoint.setScaleFactorName("W_SF");
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << wattsDataPoint;

        SunSpecDataPoint wattsPhaseADataPoint;
        wattsPhaseADataPoint.setName("WphA");
        wattsPhaseADataPoint.setLabel("Watts phase A");
        wattsPhaseADataPoint.setUnits("W");
        wattsPhaseADataPoint.setSize(1);
        wattsPhaseADataPoint.setScaleFactorName("W_SF");
        wattsPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << wattsPhaseADataPoint;

        SunSpecDataPoint wattsPhaseBDataPoint;
        wattsPhaseBDataPoint.setName("WphB");
        wattsPhaseBDataPoint.setLabel("Watts phase B");
        wattsPhaseBDataPoint.setUnits("W");
        wattsPhaseBDataPoint.setSize(1);
        wattsPhaseBDataPoint.setScaleFactorName("W_SF");
        wattsPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << wattsPhaseBDataPoint;

        SunSpecDataPoint wattsPhaseCDataPoint;
        wattsPhaseCDataPoint.setName("WphC");
        wattsPhaseCDataPoint.setLabel("Watts phase C");
        wattsPhaseCDataPoint.setUnits("W");
        wattsPhaseCDataPoint.setSize(1);
        wattsPhaseCDataPoint.setScaleFactorName("W_SF");
        wattsPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << wattsPhaseCDataPoint;

        SunSpecDataPoint W_SFDataPoint;
        W_SFDataPoint.setName("W_SF");
        W_SFDataPoint.setDescription("Real Power scale factor");
        W_SFDataPoint.setMandatory(true);
        W_SFDataPoint.setSize(1);
        W_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << W_SFDataPoint;

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(1);
        vaDataPoint.setScaleFactorName("VA_SF");
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << vaDataPoint;

        SunSpecDataPoint vaPhaseADataPoint;
        vaPhaseADataPoint.setName("VAphA");
        vaPhaseADataPoint.setLabel("VA phase A");
        vaPhaseADataPoint.setUnits("VA");
        vaPhaseADataPoint.setSize(1);
        vaPhaseADataPoint.setScaleFactorName("VA_SF");
        vaPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << vaPhaseADataPoint;

        SunSpecDataPoint vaPhaseBDataPoint;
        vaPhaseBDataPoint.setName("VAphB");
        vaPhaseBDataPoint.setLabel("VA phase B");
        vaPhaseBDataPoint.setUnits("VA");
        vaPhaseBDataPoint.setSize(1);
        vaPhaseBDataPoint.setScaleFactorName("VA_SF");
        vaPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << vaPhaseBDataPoint;

        SunSpecDataPoint vaPhaseCDataPoint;
        vaPhaseCDataPoint.setName("VAphC");
        vaPhaseCDataPoint.setLabel("VA phase C");
        vaPhaseCDataPoint.setUnits("VA");
        vaPhaseCDataPoint.setSize(1);
        vaPhaseCDataPoint.setScaleFactorName("VA_SF");
        vaPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << vaPhaseCDataPoint;

        SunSpecDataPoint VA_SFDataPoint;
        VA_SFDataPoint.setName("VA_SF");
        VA_SFDataPoint.setDescription("Apparent Power scale factor");
        VA_SFDataPoint.setSize(1);
        VA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << VA_SFDataPoint;

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAR");
        varDataPoint.setLabel("VAR");
        varDataPoint.setDescription("Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(1);
        varDataPoint.setScaleFactorName("VAR_SF");
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << varDataPoint;

        SunSpecDataPoint varPhaseADataPoint;
        varPhaseADataPoint.setName("VARphA");
        varPhaseADataPoint.setLabel("VAR phase A");
        varPhaseADataPoint.setUnits("var");
        varPhaseADataPoint.setSize(1);
        varPhaseADataPoint.setScaleFactorName("VAR_SF");
        varPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << varPhaseADataPoint;

        SunSpecDataPoint varPhaseBDataPoint;
        varPhaseBDataPoint.setName("VARphB");
        varPhaseBDataPoint.setLabel("VAR phase B");
        varPhaseBDataPoint.setUnits("var");
        varPhaseBDataPoint.setSize(1);
        varPhaseBDataPoint.setScaleFactorName("VAR_SF");
        varPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << varPhaseBDataPoint;

        SunSpecDataPoint varPhaseCDataPoint;
        varPhaseCDataPoint.setName("VARphC");
        varPhaseCDataPoint.setLabel("VAR phase C");
        varPhaseCDataPoint.setUnits("var");
        varPhaseCDataPoint.setSize(1);
        varPhaseCDataPoint.setScaleFactorName("VAR_SF");
        varPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << varPhaseCDataPoint;

        SunSpecDataPoint VAR_SFDataPoint;
        VAR_SFDataPoint.setName("VAR_SF");
        VAR_SFDataPoint.setDescription("Reactive Power scale factor");
        VAR_SFDataPoint.setSize(1);
        VAR_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << VAR_SFDataPoint;

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("Power Factor");
        pfDataPoint.setUnits("Pct");
        pfDataPoint.setSize(1);
        pfDataPoint.setScaleFactorName("PF_SF");
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << pfDataPoint;

        SunSpecDataPoint pfPhaseADataPoint;
        pfPhaseADataPoint.setName("PFphA");
        pfPhaseADataPoint.setLabel("PF phase A");
        pfPhaseADataPoint.setUnits("Pct");
        pfPhaseADataPoint.setSize(1);
        pfPhaseADataPoint.setScaleFactorName("PF_SF");
        pfPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << pfPhaseADataPoint;

        SunSpecDataPoint pfPhaseBDataPoint;
        pfPhaseBDataPoint.setName("PFphB");
        pfPhaseBDataPoint.setLabel("PF phase B");
        pfPhaseBDataPoint.setUnits("Pct");
        pfPhaseBDataPoint.setSize(1);
        pfPhaseBDataPoint.setScaleFactorName("PF_SF");
        pfPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << pfPhaseBDataPoint;

        SunSpecDataPoint pfPhaseCDataPoint;
        pfPhaseCDataPoint.setName("PFphC");
        pfPhaseCDataPoint.setLabel("PF phase C");
        pfPhaseCDataPoint.setUnits("Pct");
        pfPhaseCDataPoint.setSize(1);
        pfPhaseCDataPoint.setScaleFactorName("PF_SF");
        pfPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << pfPhaseCDataPoint;

        SunSpecDataPoint PF_SFDataPoint;
        PF_SFDataPoint.setName("PF_SF");
        PF_SFDataPoint.setDescription("Power Factor scale factor");
        PF_SFDataPoint.setSize(1);
        PF_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << PF_SFDataPoint;

        SunSpecDataPoint totalWattHoursExportedDataPoint;
        totalWattHoursExportedDataPoint.setName("TotWhExp");
        totalWattHoursExportedDataPoint.setLabel("Total Watt-hours Exported");
        totalWattHoursExportedDataPoint.setDescription("Total Real Energy Exported");
        totalWattHoursExportedDataPoint.setUnits("Wh");
        totalWattHoursExportedDataPoint.setMandatory(true);
        totalWattHoursExportedDataPoint.setSize(2);
        totalWattHoursExportedDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursExportedDataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseADataPoint;
        totalWattHoursExportedPhaseADataPoint.setName("TotWhExpPhA");
        totalWattHoursExportedPhaseADataPoint.setLabel("Total Watt-hours Exported phase A");
        totalWattHoursExportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseADataPoint.setSize(2);
        totalWattHoursExportedPhaseADataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursExportedPhaseADataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseBDataPoint;
        totalWattHoursExportedPhaseBDataPoint.setName("TotWhExpPhB");
        totalWattHoursExportedPhaseBDataPoint.setLabel("Total Watt-hours Exported phase B");
        totalWattHoursExportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseBDataPoint.setSize(2);
        totalWattHoursExportedPhaseBDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursExportedPhaseBDataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseCDataPoint;
        totalWattHoursExportedPhaseCDataPoint.setName("TotWhExpPhC");
        totalWattHoursExportedPhaseCDataPoint.setLabel("Total Watt-hours Exported phase C");
        totalWattHoursExportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseCDataPoint.setSize(2);
        totalWattHoursExportedPhaseCDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursExportedPhaseCDataPoint;

        SunSpecDataPoint totalWattHoursImportedDataPoint;
        totalWattHoursImportedDataPoint.setName("TotWhImp");
        totalWattHoursImportedDataPoint.setLabel("Total Watt-hours Imported");
        totalWattHoursImportedDataPoint.setDescription("Total Real Energy Imported");
        totalWattHoursImportedDataPoint.setUnits("Wh");
        totalWattHoursImportedDataPoint.setMandatory(true);
        totalWattHoursImportedDataPoint.setSize(2);
        totalWattHoursImportedDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursImportedDataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseADataPoint;
        totalWattHoursImportedPhaseADataPoint.setName("TotWhImpPhA");
        totalWattHoursImportedPhaseADataPoint.setLabel("Total Watt-hours Imported phase A");
        totalWattHoursImportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseADataPoint.setSize(2);
        totalWattHoursImportedPhaseADataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursImportedPhaseADataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseBDataPoint;
        totalWattHoursImportedPhaseBDataPoint.setName("TotWhImpPhB");
        totalWattHoursImportedPhaseBDataPoint.setLabel("Total Watt-hours Imported phase B");
        totalWattHoursImportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseBDataPoint.setSize(2);
        totalWattHoursImportedPhaseBDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursImportedPhaseBDataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseCDataPoint;
        totalWattHoursImportedPhaseCDataPoint.setName("TotWhImpPhC");
        totalWattHoursImportedPhaseCDataPoint.setLabel("Total Watt-hours Imported phase C");
        totalWattHoursImportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseCDataPoint.setSize(2);
        totalWattHoursImportedPhaseCDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursImportedPhaseCDataPoint;

        SunSpecDataPoint TotWh_SFDataPoint;
        TotWh_SFDataPoint.setName("TotWh_SF");
        TotWh_SFDataPoint.setDescription("Real Energy scale factor");
        TotWh_SFDataPoint.setMandatory(true);
        TotWh_SFDataPoint.setSize(1);
        TotWh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << TotWh_SFDataPoint;

        SunSpecDataPoint totalVaHoursExportedDataPoint;
        totalVaHoursExportedDataPoint.setName("TotVAhExp");
        totalVaHoursExportedDataPoint.setLabel("Total VA-hours Exported");
        totalVaHoursExportedDataPoint.setDescription("Total Apparent Energy Exported");
        totalVaHoursExportedDataPoint.setUnits("VAh");
        totalVaHoursExportedDataPoint.setSize(2);
        totalVaHoursExportedDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursExportedDataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseADataPoint;
        totalVaHoursExportedPhaseADataPoint.setName("TotVAhExpPhA");
        totalVaHoursExportedPhaseADataPoint.setLabel("Total VA-hours Exported phase A");
        totalVaHoursExportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseADataPoint.setSize(2);
        totalVaHoursExportedPhaseADataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursExportedPhaseADataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseBDataPoint;
        totalVaHoursExportedPhaseBDataPoint.setName("TotVAhExpPhB");
        totalVaHoursExportedPhaseBDataPoint.setLabel("Total VA-hours Exported phase B");
        totalVaHoursExportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseBDataPoint.setSize(2);
        totalVaHoursExportedPhaseBDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursExportedPhaseBDataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseCDataPoint;
        totalVaHoursExportedPhaseCDataPoint.setName("TotVAhExpPhC");
        totalVaHoursExportedPhaseCDataPoint.setLabel("Total VA-hours Exported phase C");
        totalVaHoursExportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseCDataPoint.setSize(2);
        totalVaHoursExportedPhaseCDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursExportedPhaseCDataPoint;

        SunSpecDataPoint totalVaHoursImportedDataPoint;
        totalVaHoursImportedDataPoint.setName("TotVAhImp");
        totalVaHoursImportedDataPoint.setLabel("Total VA-hours Imported");
        totalVaHoursImportedDataPoint.setDescription("Total Apparent Energy Imported");
        totalVaHoursImportedDataPoint.setUnits("VAh");
        totalVaHoursImportedDataPoint.setSize(2);
        totalVaHoursImportedDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursImportedDataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseADataPoint;
        totalVaHoursImportedPhaseADataPoint.setName("TotVAhImpPhA");
        totalVaHoursImportedPhaseADataPoint.setLabel("Total VA-hours Imported phase A");
        totalVaHoursImportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseADataPoint.setSize(2);
        totalVaHoursImportedPhaseADataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursImportedPhaseADataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseBDataPoint;
        totalVaHoursImportedPhaseBDataPoint.setName("TotVAhImpPhB");
        totalVaHoursImportedPhaseBDataPoint.setLabel("Total VA-hours Imported phase B");
        totalVaHoursImportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseBDataPoint.setSize(2);
        totalVaHoursImportedPhaseBDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursImportedPhaseBDataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseCDataPoint;
        totalVaHoursImportedPhaseCDataPoint.setName("TotVAhImpPhC");
        totalVaHoursImportedPhaseCDataPoint.setLabel("Total VA-hours Imported phase C");
        totalVaHoursImportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseCDataPoint.setSize(2);
        totalVaHoursImportedPhaseCDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursImportedPhaseCDataPoint;

        SunSpecDataPoint TotVAh_SFDataPoint;
        TotVAh_SFDataPoint.setName("TotVAh_SF");
        TotVAh_SFDataPoint.setDescription("Apparent Energy scale factor");
        TotVAh_SFDataPoint.setSize(1);
        TotVAh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << TotVAh_SFDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1DataPoint;
        totalVarHoursImportedQ1DataPoint.setName("TotVArhImpQ1");
        totalVarHoursImportedQ1DataPoint.setLabel("Total VAR-hours Imported Q1");
        totalVarHoursImportedQ1DataPoint.setDescription("Total Reactive Energy Imported Quadrant 1");
        totalVarHoursImportedQ1DataPoint.setUnits("varh");
        totalVarHoursImportedQ1DataPoint.setSize(2);
        totalVarHoursImportedQ1DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ1DataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseADataPoint;
        totalVarHoursImportedQ1PhaseADataPoint.setName("TotVArhImpQ1PhA");
        totalVarHoursImportedQ1PhaseADataPoint.setLabel("Total VAr-hours Imported Q1 phase A");
        totalVarHoursImportedQ1PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseADataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseBDataPoint;
        totalVarHoursImportedQ1PhaseBDataPoint.setName("TotVArhImpQ1PhB");
        totalVarHoursImportedQ1PhaseBDataPoint.setLabel("Total VAr-hours Imported Q1 phase B");
        totalVarHoursImportedQ1PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseCDataPoint;
        totalVarHoursImportedQ1PhaseCDataPoint.setName("TotVArhImpQ1PhC");
        totalVarHoursImportedQ1PhaseCDataPoint.setLabel("Total VAr-hours Imported Q1 phase C");
        totalVarHoursImportedQ1PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2DataPoint;
        totalVarHoursImportedQ2DataPoint.setName("TotVArhImpQ2");
        totalVarHoursImportedQ2DataPoint.setLabel("Total VAr-hours Imported Q2");
        totalVarHoursImportedQ2DataPoint.setDescription("Total Reactive Power Imported Quadrant 2");
        totalVarHoursImportedQ2DataPoint.setUnits("varh");
        totalVarHoursImportedQ2DataPoint.setSize(2);
        totalVarHoursImportedQ2DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ2DataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseADataPoint;
        totalVarHoursImportedQ2PhaseADataPoint.setName("TotVArhImpQ2PhA");
        totalVarHoursImportedQ2PhaseADataPoint.setLabel("Total VAr-hours Imported Q2 phase A");
        totalVarHoursImportedQ2PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseADataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseBDataPoint;
        totalVarHoursImportedQ2PhaseBDataPoint.setName("TotVArhImpQ2PhB");
        totalVarHoursImportedQ2PhaseBDataPoint.setLabel("Total VAr-hours Imported Q2 phase B");
        totalVarHoursImportedQ2PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseCDataPoint;
        totalVarHoursImportedQ2PhaseCDataPoint.setName("TotVArhImpQ2PhC");
        totalVarHoursImportedQ2PhaseCDataPoint.setLabel("Total VAr-hours Imported Q2 phase C");
        totalVarHoursImportedQ2PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3DataPoint;
        totalVarHoursExportedQ3DataPoint.setName("TotVArhExpQ3");
        totalVarHoursExportedQ3DataPoint.setLabel("Total VAr-hours Exported Q3");
        totalVarHoursExportedQ3DataPoint.setDescription("Total Reactive Power Exported Quadrant 3");
        totalVarHoursExportedQ3DataPoint.setUnits("varh");
        totalVarHoursExportedQ3DataPoint.setSize(2);
        totalVarHoursExportedQ3DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ3DataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseADataPoint;
        totalVarHoursExportedQ3PhaseADataPoint.setName("TotVArhExpQ3PhA");
        totalVarHoursExportedQ3PhaseADataPoint.setLabel("Total VAr-hours Exported Q3 phase A");
        totalVarHoursExportedQ3PhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseADataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseADataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseBDataPoint;
        totalVarHoursExportedQ3PhaseBDataPoint.setName("TotVArhExpQ3PhB");
        totalVarHoursExportedQ3PhaseBDataPoint.setLabel("Total VAr-hours Exported Q3 phase B");
        totalVarHoursExportedQ3PhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseCDataPoint;
        totalVarHoursExportedQ3PhaseCDataPoint.setName("TotVArhExpQ3PhC");
        totalVarHoursExportedQ3PhaseCDataPoint.setLabel("Total VAr-hours Exported Q3 phase C");
        totalVarHoursExportedQ3PhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4DataPoint;
        totalVarHoursExportedQ4DataPoint.setName("TotVArhExpQ4");
        totalVarHoursExportedQ4DataPoint.setLabel("Total VAr-hours Exported Q4");
        totalVarHoursExportedQ4DataPoint.setDescription("Total Reactive Power Exported Quadrant 4");
        totalVarHoursExportedQ4DataPoint.setUnits("varh");
        totalVarHoursExportedQ4DataPoint.setSize(2);
        totalVarHoursExportedQ4DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ4DataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseADataPoint;
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setName("TotVArhExpQ4PhA");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase A");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseADataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseBDataPoint;
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setName("TotVArhExpQ4PhB");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase B");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseBDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseCDataPoint;
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setName("TotVArhExpQ4PhC");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase C");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseCDataPoint;

        SunSpecDataPoint TotVArh_SFDataPoint;
        TotVArh_SFDataPoint.setName("TotVArh_SF");
        TotVArh_SFDataPoint.setDescription("Reactive Energy scale factor");
        TotVArh_SFDataPoint.setSize(1);
        TotVArh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << TotVArh_SFDataPoint;

        SunSpecDataPoint eventsDataPoint;
        eventsDataPoint.setName("Evt");
        eventsDataPoint.setLabel("Events");
        eventsDataPoint.setDescription("Meter Event Flags");
        eventsDataPoint.setMandatory(true);
        eventsDataPoint.setSize(2);
        eventsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << eventsDataPoint;

        break;
    }
    case 202: {
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
        ampsDataPoint.setDescription("Total AC Current");
        ampsDataPoint.setUnits("A");
        ampsDataPoint.setMandatory(true);
        ampsDataPoint.setSize(1);
        ampsDataPoint.setScaleFactorName("A_SF");
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << ampsDataPoint;

        SunSpecDataPoint ampsPhaseaDataPoint;
        ampsPhaseaDataPoint.setName("AphA");
        ampsPhaseaDataPoint.setLabel("Amps PhaseA");
        ampsPhaseaDataPoint.setDescription("Phase A Current");
        ampsPhaseaDataPoint.setUnits("A");
        ampsPhaseaDataPoint.setSize(1);
        ampsPhaseaDataPoint.setScaleFactorName("A_SF");
        ampsPhaseaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << ampsPhaseaDataPoint;

        SunSpecDataPoint ampsPhasebDataPoint;
        ampsPhasebDataPoint.setName("AphB");
        ampsPhasebDataPoint.setLabel("Amps PhaseB");
        ampsPhasebDataPoint.setDescription("Phase B Current");
        ampsPhasebDataPoint.setUnits("A");
        ampsPhasebDataPoint.setMandatory(true);
        ampsPhasebDataPoint.setSize(1);
        ampsPhasebDataPoint.setScaleFactorName("A_SF");
        ampsPhasebDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << ampsPhasebDataPoint;

        SunSpecDataPoint ampsPhasecDataPoint;
        ampsPhasecDataPoint.setName("AphC");
        ampsPhasecDataPoint.setLabel("Amps PhaseC");
        ampsPhasecDataPoint.setDescription("Phase C Current");
        ampsPhasecDataPoint.setUnits("A");
        ampsPhasecDataPoint.setMandatory(true);
        ampsPhasecDataPoint.setSize(1);
        ampsPhasecDataPoint.setScaleFactorName("A_SF");
        ampsPhasecDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << ampsPhasecDataPoint;

        SunSpecDataPoint A_SFDataPoint;
        A_SFDataPoint.setName("A_SF");
        A_SFDataPoint.setDescription("Current scale factor");
        A_SFDataPoint.setMandatory(true);
        A_SFDataPoint.setSize(1);
        A_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << A_SFDataPoint;

        SunSpecDataPoint voltageLnDataPoint;
        voltageLnDataPoint.setName("PhV");
        voltageLnDataPoint.setLabel("Voltage LN");
        voltageLnDataPoint.setDescription("Line to Neutral AC Voltage (average of active phases)");
        voltageLnDataPoint.setUnits("V");
        voltageLnDataPoint.setMandatory(true);
        voltageLnDataPoint.setSize(1);
        voltageLnDataPoint.setScaleFactorName("V_SF");
        voltageLnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << voltageLnDataPoint;

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
        phaseVoltageAnDataPoint.setMandatory(true);
        phaseVoltageAnDataPoint.setSize(1);
        phaseVoltageAnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageAnDataPoint;

        SunSpecDataPoint phaseVoltageBnDataPoint;
        phaseVoltageBnDataPoint.setName("PhVphB");
        phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
        phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
        phaseVoltageBnDataPoint.setUnits("V");
        phaseVoltageBnDataPoint.setMandatory(true);
        phaseVoltageBnDataPoint.setSize(1);
        phaseVoltageBnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageBnDataPoint;

        SunSpecDataPoint phaseVoltageCnDataPoint;
        phaseVoltageCnDataPoint.setName("PhVphC");
        phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
        phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
        phaseVoltageCnDataPoint.setUnits("V");
        phaseVoltageCnDataPoint.setSize(1);
        phaseVoltageCnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageCnDataPoint;

        SunSpecDataPoint voltageLlDataPoint;
        voltageLlDataPoint.setName("PPV");
        voltageLlDataPoint.setLabel("Voltage LL");
        voltageLlDataPoint.setDescription("Line to Line AC Voltage (average of active phases)");
        voltageLlDataPoint.setUnits("V");
        voltageLlDataPoint.setMandatory(true);
        voltageLlDataPoint.setSize(1);
        voltageLlDataPoint.setScaleFactorName("V_SF");
        voltageLlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << voltageLlDataPoint;

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PhVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setMandatory(true);
        phaseVoltageAbDataPoint.setSize(1);
        phaseVoltageAbDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageAbDataPoint;

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PhVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setSize(1);
        phaseVoltageBcDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageBcDataPoint;

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PhVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setSize(1);
        phaseVoltageCaDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageCaDataPoint;

        SunSpecDataPoint V_SFDataPoint;
        V_SFDataPoint.setName("V_SF");
        V_SFDataPoint.setDescription("Voltage scale factor");
        V_SFDataPoint.setMandatory(true);
        V_SFDataPoint.setSize(1);
        V_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << V_SFDataPoint;

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(1);
        hzDataPoint.setScaleFactorName("Hz_SF");
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << hzDataPoint;

        SunSpecDataPoint Hz_SFDataPoint;
        Hz_SFDataPoint.setName("Hz_SF");
        Hz_SFDataPoint.setDescription("Frequency scale factor");
        Hz_SFDataPoint.setSize(1);
        Hz_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << Hz_SFDataPoint;

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("Total Real Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(1);
        wattsDataPoint.setScaleFactorName("W_SF");
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << wattsDataPoint;

        SunSpecDataPoint wattsPhaseADataPoint;
        wattsPhaseADataPoint.setName("WphA");
        wattsPhaseADataPoint.setLabel("Watts phase A");
        wattsPhaseADataPoint.setUnits("W");
        wattsPhaseADataPoint.setSize(1);
        wattsPhaseADataPoint.setScaleFactorName("W_SF");
        wattsPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << wattsPhaseADataPoint;

        SunSpecDataPoint wattsPhaseBDataPoint;
        wattsPhaseBDataPoint.setName("WphB");
        wattsPhaseBDataPoint.setLabel("Watts phase B");
        wattsPhaseBDataPoint.setUnits("W");
        wattsPhaseBDataPoint.setSize(1);
        wattsPhaseBDataPoint.setScaleFactorName("W_SF");
        wattsPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << wattsPhaseBDataPoint;

        SunSpecDataPoint wattsPhaseCDataPoint;
        wattsPhaseCDataPoint.setName("WphC");
        wattsPhaseCDataPoint.setLabel("Watts phase C");
        wattsPhaseCDataPoint.setUnits("W");
        wattsPhaseCDataPoint.setSize(1);
        wattsPhaseCDataPoint.setScaleFactorName("W_SF");
        wattsPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << wattsPhaseCDataPoint;

        SunSpecDataPoint W_SFDataPoint;
        W_SFDataPoint.setName("W_SF");
        W_SFDataPoint.setDescription("Real Power scale factor");
        W_SFDataPoint.setMandatory(true);
        W_SFDataPoint.setSize(1);
        W_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << W_SFDataPoint;

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(1);
        vaDataPoint.setScaleFactorName("VA_SF");
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << vaDataPoint;

        SunSpecDataPoint vaPhaseADataPoint;
        vaPhaseADataPoint.setName("VAphA");
        vaPhaseADataPoint.setLabel("VA phase A");
        vaPhaseADataPoint.setUnits("VA");
        vaPhaseADataPoint.setSize(1);
        vaPhaseADataPoint.setScaleFactorName("VA_SF");
        vaPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << vaPhaseADataPoint;

        SunSpecDataPoint vaPhaseBDataPoint;
        vaPhaseBDataPoint.setName("VAphB");
        vaPhaseBDataPoint.setLabel("VA phase B");
        vaPhaseBDataPoint.setUnits("VA");
        vaPhaseBDataPoint.setSize(1);
        vaPhaseBDataPoint.setScaleFactorName("VA_SF");
        vaPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << vaPhaseBDataPoint;

        SunSpecDataPoint vaPhaseCDataPoint;
        vaPhaseCDataPoint.setName("VAphC");
        vaPhaseCDataPoint.setLabel("VA phase C");
        vaPhaseCDataPoint.setUnits("VA");
        vaPhaseCDataPoint.setSize(1);
        vaPhaseCDataPoint.setScaleFactorName("VA_SF");
        vaPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << vaPhaseCDataPoint;

        SunSpecDataPoint VA_SFDataPoint;
        VA_SFDataPoint.setName("VA_SF");
        VA_SFDataPoint.setDescription("Apparent Power scale factor");
        VA_SFDataPoint.setSize(1);
        VA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << VA_SFDataPoint;

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAR");
        varDataPoint.setLabel("VAR");
        varDataPoint.setDescription("Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(1);
        varDataPoint.setScaleFactorName("VAR_SF");
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << varDataPoint;

        SunSpecDataPoint varPhaseADataPoint;
        varPhaseADataPoint.setName("VARphA");
        varPhaseADataPoint.setLabel("VAR phase A");
        varPhaseADataPoint.setUnits("var");
        varPhaseADataPoint.setSize(1);
        varPhaseADataPoint.setScaleFactorName("VAR_SF");
        varPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << varPhaseADataPoint;

        SunSpecDataPoint varPhaseBDataPoint;
        varPhaseBDataPoint.setName("VARphB");
        varPhaseBDataPoint.setLabel("VAR phase B");
        varPhaseBDataPoint.setUnits("var");
        varPhaseBDataPoint.setSize(1);
        varPhaseBDataPoint.setScaleFactorName("VAR_SF");
        varPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << varPhaseBDataPoint;

        SunSpecDataPoint varPhaseCDataPoint;
        varPhaseCDataPoint.setName("VARphC");
        varPhaseCDataPoint.setLabel("VAR phase C");
        varPhaseCDataPoint.setUnits("var");
        varPhaseCDataPoint.setSize(1);
        varPhaseCDataPoint.setScaleFactorName("VAR_SF");
        varPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << varPhaseCDataPoint;

        SunSpecDataPoint VAR_SFDataPoint;
        VAR_SFDataPoint.setName("VAR_SF");
        VAR_SFDataPoint.setDescription("Reactive Power scale factor");
        VAR_SFDataPoint.setSize(1);
        VAR_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << VAR_SFDataPoint;

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("Power Factor");
        pfDataPoint.setUnits("Pct");
        pfDataPoint.setSize(1);
        pfDataPoint.setScaleFactorName("PF_SF");
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << pfDataPoint;

        SunSpecDataPoint pfPhaseADataPoint;
        pfPhaseADataPoint.setName("PFphA");
        pfPhaseADataPoint.setLabel("PF phase A");
        pfPhaseADataPoint.setUnits("Pct");
        pfPhaseADataPoint.setSize(1);
        pfPhaseADataPoint.setScaleFactorName("PF_SF");
        pfPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << pfPhaseADataPoint;

        SunSpecDataPoint pfPhaseBDataPoint;
        pfPhaseBDataPoint.setName("PFphB");
        pfPhaseBDataPoint.setLabel("PF phase B");
        pfPhaseBDataPoint.setUnits("Pct");
        pfPhaseBDataPoint.setSize(1);
        pfPhaseBDataPoint.setScaleFactorName("PF_SF");
        pfPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << pfPhaseBDataPoint;

        SunSpecDataPoint pfPhaseCDataPoint;
        pfPhaseCDataPoint.setName("PFphC");
        pfPhaseCDataPoint.setLabel("PF phase C");
        pfPhaseCDataPoint.setUnits("Pct");
        pfPhaseCDataPoint.setSize(1);
        pfPhaseCDataPoint.setScaleFactorName("PF_SF");
        pfPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << pfPhaseCDataPoint;

        SunSpecDataPoint PF_SFDataPoint;
        PF_SFDataPoint.setName("PF_SF");
        PF_SFDataPoint.setDescription("Power Factor scale factor");
        PF_SFDataPoint.setSize(1);
        PF_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << PF_SFDataPoint;

        SunSpecDataPoint totalWattHoursExportedDataPoint;
        totalWattHoursExportedDataPoint.setName("TotWhExp");
        totalWattHoursExportedDataPoint.setLabel("Total Watt-hours Exported");
        totalWattHoursExportedDataPoint.setDescription("Total Real Energy Exported");
        totalWattHoursExportedDataPoint.setUnits("Wh");
        totalWattHoursExportedDataPoint.setMandatory(true);
        totalWattHoursExportedDataPoint.setSize(2);
        totalWattHoursExportedDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursExportedDataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseADataPoint;
        totalWattHoursExportedPhaseADataPoint.setName("TotWhExpPhA");
        totalWattHoursExportedPhaseADataPoint.setLabel("Total Watt-hours Exported phase A");
        totalWattHoursExportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseADataPoint.setSize(2);
        totalWattHoursExportedPhaseADataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursExportedPhaseADataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseBDataPoint;
        totalWattHoursExportedPhaseBDataPoint.setName("TotWhExpPhB");
        totalWattHoursExportedPhaseBDataPoint.setLabel("Total Watt-hours Exported phase B");
        totalWattHoursExportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseBDataPoint.setSize(2);
        totalWattHoursExportedPhaseBDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursExportedPhaseBDataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseCDataPoint;
        totalWattHoursExportedPhaseCDataPoint.setName("TotWhExpPhC");
        totalWattHoursExportedPhaseCDataPoint.setLabel("Total Watt-hours Exported phase C");
        totalWattHoursExportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseCDataPoint.setSize(2);
        totalWattHoursExportedPhaseCDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursExportedPhaseCDataPoint;

        SunSpecDataPoint totalWattHoursImportedDataPoint;
        totalWattHoursImportedDataPoint.setName("TotWhImp");
        totalWattHoursImportedDataPoint.setLabel("Total Watt-hours Imported");
        totalWattHoursImportedDataPoint.setDescription("Total Real Energy Imported");
        totalWattHoursImportedDataPoint.setUnits("Wh");
        totalWattHoursImportedDataPoint.setMandatory(true);
        totalWattHoursImportedDataPoint.setSize(2);
        totalWattHoursImportedDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursImportedDataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseADataPoint;
        totalWattHoursImportedPhaseADataPoint.setName("TotWhImpPhA");
        totalWattHoursImportedPhaseADataPoint.setLabel("Total Watt-hours Imported phase A");
        totalWattHoursImportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseADataPoint.setSize(2);
        totalWattHoursImportedPhaseADataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursImportedPhaseADataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseBDataPoint;
        totalWattHoursImportedPhaseBDataPoint.setName("TotWhImpPhB");
        totalWattHoursImportedPhaseBDataPoint.setLabel("Total Watt-hours Imported phase B");
        totalWattHoursImportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseBDataPoint.setSize(2);
        totalWattHoursImportedPhaseBDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursImportedPhaseBDataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseCDataPoint;
        totalWattHoursImportedPhaseCDataPoint.setName("TotWhImpPhC");
        totalWattHoursImportedPhaseCDataPoint.setLabel("Total Watt-hours Imported phase C");
        totalWattHoursImportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseCDataPoint.setSize(2);
        totalWattHoursImportedPhaseCDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursImportedPhaseCDataPoint;

        SunSpecDataPoint TotWh_SFDataPoint;
        TotWh_SFDataPoint.setName("TotWh_SF");
        TotWh_SFDataPoint.setDescription("Real Energy scale factor");
        TotWh_SFDataPoint.setMandatory(true);
        TotWh_SFDataPoint.setSize(1);
        TotWh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << TotWh_SFDataPoint;

        SunSpecDataPoint totalVaHoursExportedDataPoint;
        totalVaHoursExportedDataPoint.setName("TotVAhExp");
        totalVaHoursExportedDataPoint.setLabel("Total VA-hours Exported");
        totalVaHoursExportedDataPoint.setDescription("Total Apparent Energy Exported");
        totalVaHoursExportedDataPoint.setUnits("VAh");
        totalVaHoursExportedDataPoint.setSize(2);
        totalVaHoursExportedDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursExportedDataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseADataPoint;
        totalVaHoursExportedPhaseADataPoint.setName("TotVAhExpPhA");
        totalVaHoursExportedPhaseADataPoint.setLabel("Total VA-hours Exported phase A");
        totalVaHoursExportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseADataPoint.setSize(2);
        totalVaHoursExportedPhaseADataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursExportedPhaseADataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseBDataPoint;
        totalVaHoursExportedPhaseBDataPoint.setName("TotVAhExpPhB");
        totalVaHoursExportedPhaseBDataPoint.setLabel("Total VA-hours Exported phase B");
        totalVaHoursExportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseBDataPoint.setSize(2);
        totalVaHoursExportedPhaseBDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursExportedPhaseBDataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseCDataPoint;
        totalVaHoursExportedPhaseCDataPoint.setName("TotVAhExpPhC");
        totalVaHoursExportedPhaseCDataPoint.setLabel("Total VA-hours Exported phase C");
        totalVaHoursExportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseCDataPoint.setSize(2);
        totalVaHoursExportedPhaseCDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursExportedPhaseCDataPoint;

        SunSpecDataPoint totalVaHoursImportedDataPoint;
        totalVaHoursImportedDataPoint.setName("TotVAhImp");
        totalVaHoursImportedDataPoint.setLabel("Total VA-hours Imported");
        totalVaHoursImportedDataPoint.setDescription("Total Apparent Energy Imported");
        totalVaHoursImportedDataPoint.setUnits("VAh");
        totalVaHoursImportedDataPoint.setSize(2);
        totalVaHoursImportedDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursImportedDataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseADataPoint;
        totalVaHoursImportedPhaseADataPoint.setName("TotVAhImpPhA");
        totalVaHoursImportedPhaseADataPoint.setLabel("Total VA-hours Imported phase A");
        totalVaHoursImportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseADataPoint.setSize(2);
        totalVaHoursImportedPhaseADataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursImportedPhaseADataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseBDataPoint;
        totalVaHoursImportedPhaseBDataPoint.setName("TotVAhImpPhB");
        totalVaHoursImportedPhaseBDataPoint.setLabel("Total VA-hours Imported phase B");
        totalVaHoursImportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseBDataPoint.setSize(2);
        totalVaHoursImportedPhaseBDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursImportedPhaseBDataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseCDataPoint;
        totalVaHoursImportedPhaseCDataPoint.setName("TotVAhImpPhC");
        totalVaHoursImportedPhaseCDataPoint.setLabel("Total VA-hours Imported phase C");
        totalVaHoursImportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseCDataPoint.setSize(2);
        totalVaHoursImportedPhaseCDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursImportedPhaseCDataPoint;

        SunSpecDataPoint TotVAh_SFDataPoint;
        TotVAh_SFDataPoint.setName("TotVAh_SF");
        TotVAh_SFDataPoint.setDescription("Apparent Energy scale factor");
        TotVAh_SFDataPoint.setSize(1);
        TotVAh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << TotVAh_SFDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1DataPoint;
        totalVarHoursImportedQ1DataPoint.setName("TotVArhImpQ1");
        totalVarHoursImportedQ1DataPoint.setLabel("Total VAR-hours Imported Q1");
        totalVarHoursImportedQ1DataPoint.setDescription("Total Reactive Energy Imported Quadrant 1");
        totalVarHoursImportedQ1DataPoint.setUnits("varh");
        totalVarHoursImportedQ1DataPoint.setSize(2);
        totalVarHoursImportedQ1DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ1DataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseADataPoint;
        totalVarHoursImportedQ1PhaseADataPoint.setName("TotVArhImpQ1PhA");
        totalVarHoursImportedQ1PhaseADataPoint.setLabel("Total VAr-hours Imported Q1 phase A");
        totalVarHoursImportedQ1PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseADataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseBDataPoint;
        totalVarHoursImportedQ1PhaseBDataPoint.setName("TotVArhImpQ1PhB");
        totalVarHoursImportedQ1PhaseBDataPoint.setLabel("Total VAr-hours Imported Q1 phase B");
        totalVarHoursImportedQ1PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseCDataPoint;
        totalVarHoursImportedQ1PhaseCDataPoint.setName("TotVArhImpQ1PhC");
        totalVarHoursImportedQ1PhaseCDataPoint.setLabel("Total VAr-hours Imported Q1 phase C");
        totalVarHoursImportedQ1PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2DataPoint;
        totalVarHoursImportedQ2DataPoint.setName("TotVArhImpQ2");
        totalVarHoursImportedQ2DataPoint.setLabel("Total VAr-hours Imported Q2");
        totalVarHoursImportedQ2DataPoint.setDescription("Total Reactive Power Imported Quadrant 2");
        totalVarHoursImportedQ2DataPoint.setUnits("varh");
        totalVarHoursImportedQ2DataPoint.setSize(2);
        totalVarHoursImportedQ2DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ2DataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseADataPoint;
        totalVarHoursImportedQ2PhaseADataPoint.setName("TotVArhImpQ2PhA");
        totalVarHoursImportedQ2PhaseADataPoint.setLabel("Total VAr-hours Imported Q2 phase A");
        totalVarHoursImportedQ2PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseADataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseBDataPoint;
        totalVarHoursImportedQ2PhaseBDataPoint.setName("TotVArhImpQ2PhB");
        totalVarHoursImportedQ2PhaseBDataPoint.setLabel("Total VAr-hours Imported Q2 phase B");
        totalVarHoursImportedQ2PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseCDataPoint;
        totalVarHoursImportedQ2PhaseCDataPoint.setName("TotVArhImpQ2PhC");
        totalVarHoursImportedQ2PhaseCDataPoint.setLabel("Total VAr-hours Imported Q2 phase C");
        totalVarHoursImportedQ2PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3DataPoint;
        totalVarHoursExportedQ3DataPoint.setName("TotVArhExpQ3");
        totalVarHoursExportedQ3DataPoint.setLabel("Total VAr-hours Exported Q3");
        totalVarHoursExportedQ3DataPoint.setDescription("Total Reactive Power Exported Quadrant 3");
        totalVarHoursExportedQ3DataPoint.setUnits("varh");
        totalVarHoursExportedQ3DataPoint.setSize(2);
        totalVarHoursExportedQ3DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ3DataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseADataPoint;
        totalVarHoursExportedQ3PhaseADataPoint.setName("TotVArhExpQ3PhA");
        totalVarHoursExportedQ3PhaseADataPoint.setLabel("Total VAr-hours Exported Q3 phase A");
        totalVarHoursExportedQ3PhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseADataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseADataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseBDataPoint;
        totalVarHoursExportedQ3PhaseBDataPoint.setName("TotVArhExpQ3PhB");
        totalVarHoursExportedQ3PhaseBDataPoint.setLabel("Total VAr-hours Exported Q3 phase B");
        totalVarHoursExportedQ3PhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseCDataPoint;
        totalVarHoursExportedQ3PhaseCDataPoint.setName("TotVArhExpQ3PhC");
        totalVarHoursExportedQ3PhaseCDataPoint.setLabel("Total VAr-hours Exported Q3 phase C");
        totalVarHoursExportedQ3PhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4DataPoint;
        totalVarHoursExportedQ4DataPoint.setName("TotVArhExpQ4");
        totalVarHoursExportedQ4DataPoint.setLabel("Total VAr-hours Exported Q4");
        totalVarHoursExportedQ4DataPoint.setDescription("Total Reactive Power Exported Quadrant 4");
        totalVarHoursExportedQ4DataPoint.setUnits("varh");
        totalVarHoursExportedQ4DataPoint.setSize(2);
        totalVarHoursExportedQ4DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ4DataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseADataPoint;
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setName("TotVArhExpQ4PhA");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase A");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseADataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseBDataPoint;
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setName("TotVArhExpQ4PhB");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase B");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseBDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseCDataPoint;
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setName("TotVArhExpQ4PhC");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase C");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseCDataPoint;

        SunSpecDataPoint TotVArh_SFDataPoint;
        TotVArh_SFDataPoint.setName("TotVArh_SF");
        TotVArh_SFDataPoint.setDescription("Reactive Energy scale factor");
        TotVArh_SFDataPoint.setSize(1);
        TotVArh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << TotVArh_SFDataPoint;

        SunSpecDataPoint eventsDataPoint;
        eventsDataPoint.setName("Evt");
        eventsDataPoint.setLabel("Events");
        eventsDataPoint.setDescription("Meter Event Flags");
        eventsDataPoint.setMandatory(true);
        eventsDataPoint.setSize(2);
        eventsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << eventsDataPoint;

        break;
    }
    case 203: {
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
        ampsDataPoint.setDescription("Total AC Current");
        ampsDataPoint.setUnits("A");
        ampsDataPoint.setMandatory(true);
        ampsDataPoint.setSize(1);
        ampsDataPoint.setScaleFactorName("A_SF");
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << ampsDataPoint;

        SunSpecDataPoint ampsPhaseaDataPoint;
        ampsPhaseaDataPoint.setName("AphA");
        ampsPhaseaDataPoint.setLabel("Amps PhaseA");
        ampsPhaseaDataPoint.setDescription("Phase A Current");
        ampsPhaseaDataPoint.setUnits("A");
        ampsPhaseaDataPoint.setMandatory(true);
        ampsPhaseaDataPoint.setSize(1);
        ampsPhaseaDataPoint.setScaleFactorName("A_SF");
        ampsPhaseaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << ampsPhaseaDataPoint;

        SunSpecDataPoint ampsPhasebDataPoint;
        ampsPhasebDataPoint.setName("AphB");
        ampsPhasebDataPoint.setLabel("Amps PhaseB");
        ampsPhasebDataPoint.setDescription("Phase B Current");
        ampsPhasebDataPoint.setUnits("A");
        ampsPhasebDataPoint.setMandatory(true);
        ampsPhasebDataPoint.setSize(1);
        ampsPhasebDataPoint.setScaleFactorName("A_SF");
        ampsPhasebDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << ampsPhasebDataPoint;

        SunSpecDataPoint ampsPhasecDataPoint;
        ampsPhasecDataPoint.setName("AphC");
        ampsPhasecDataPoint.setLabel("Amps PhaseC");
        ampsPhasecDataPoint.setDescription("Phase C Current");
        ampsPhasecDataPoint.setUnits("A");
        ampsPhasecDataPoint.setMandatory(true);
        ampsPhasecDataPoint.setSize(1);
        ampsPhasecDataPoint.setScaleFactorName("A_SF");
        ampsPhasecDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << ampsPhasecDataPoint;

        SunSpecDataPoint A_SFDataPoint;
        A_SFDataPoint.setName("A_SF");
        A_SFDataPoint.setDescription("Current scale factor");
        A_SFDataPoint.setMandatory(true);
        A_SFDataPoint.setSize(1);
        A_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << A_SFDataPoint;

        SunSpecDataPoint voltageLnDataPoint;
        voltageLnDataPoint.setName("PhV");
        voltageLnDataPoint.setLabel("Voltage LN");
        voltageLnDataPoint.setDescription("Line to Neutral AC Voltage (average of active phases)");
        voltageLnDataPoint.setUnits("V");
        voltageLnDataPoint.setMandatory(true);
        voltageLnDataPoint.setSize(1);
        voltageLnDataPoint.setScaleFactorName("V_SF");
        voltageLnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << voltageLnDataPoint;

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
        phaseVoltageAnDataPoint.setMandatory(true);
        phaseVoltageAnDataPoint.setSize(1);
        phaseVoltageAnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageAnDataPoint;

        SunSpecDataPoint phaseVoltageBnDataPoint;
        phaseVoltageBnDataPoint.setName("PhVphB");
        phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
        phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
        phaseVoltageBnDataPoint.setUnits("V");
        phaseVoltageBnDataPoint.setMandatory(true);
        phaseVoltageBnDataPoint.setSize(1);
        phaseVoltageBnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageBnDataPoint;

        SunSpecDataPoint phaseVoltageCnDataPoint;
        phaseVoltageCnDataPoint.setName("PhVphC");
        phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
        phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
        phaseVoltageCnDataPoint.setUnits("V");
        phaseVoltageCnDataPoint.setMandatory(true);
        phaseVoltageCnDataPoint.setSize(1);
        phaseVoltageCnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageCnDataPoint;

        SunSpecDataPoint voltageLlDataPoint;
        voltageLlDataPoint.setName("PPV");
        voltageLlDataPoint.setLabel("Voltage LL");
        voltageLlDataPoint.setDescription("Line to Line AC Voltage (average of active phases)");
        voltageLlDataPoint.setUnits("V");
        voltageLlDataPoint.setMandatory(true);
        voltageLlDataPoint.setSize(1);
        voltageLlDataPoint.setScaleFactorName("V_SF");
        voltageLlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << voltageLlDataPoint;

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PhVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setMandatory(true);
        phaseVoltageAbDataPoint.setSize(1);
        phaseVoltageAbDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageAbDataPoint;

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PhVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setMandatory(true);
        phaseVoltageBcDataPoint.setSize(1);
        phaseVoltageBcDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageBcDataPoint;

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PhVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setMandatory(true);
        phaseVoltageCaDataPoint.setSize(1);
        phaseVoltageCaDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageCaDataPoint;

        SunSpecDataPoint V_SFDataPoint;
        V_SFDataPoint.setName("V_SF");
        V_SFDataPoint.setDescription("Voltage scale factor");
        V_SFDataPoint.setMandatory(true);
        V_SFDataPoint.setSize(1);
        V_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << V_SFDataPoint;

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(1);
        hzDataPoint.setScaleFactorName("Hz_SF");
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << hzDataPoint;

        SunSpecDataPoint Hz_SFDataPoint;
        Hz_SFDataPoint.setName("Hz_SF");
        Hz_SFDataPoint.setDescription("Frequency scale factor");
        Hz_SFDataPoint.setSize(1);
        Hz_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << Hz_SFDataPoint;

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("Total Real Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(1);
        wattsDataPoint.setScaleFactorName("W_SF");
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << wattsDataPoint;

        SunSpecDataPoint wattsPhaseADataPoint;
        wattsPhaseADataPoint.setName("WphA");
        wattsPhaseADataPoint.setLabel("Watts phase A");
        wattsPhaseADataPoint.setUnits("W");
        wattsPhaseADataPoint.setSize(1);
        wattsPhaseADataPoint.setScaleFactorName("W_SF");
        wattsPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << wattsPhaseADataPoint;

        SunSpecDataPoint wattsPhaseBDataPoint;
        wattsPhaseBDataPoint.setName("WphB");
        wattsPhaseBDataPoint.setLabel("Watts phase B");
        wattsPhaseBDataPoint.setUnits("W");
        wattsPhaseBDataPoint.setSize(1);
        wattsPhaseBDataPoint.setScaleFactorName("W_SF");
        wattsPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << wattsPhaseBDataPoint;

        SunSpecDataPoint wattsPhaseCDataPoint;
        wattsPhaseCDataPoint.setName("WphC");
        wattsPhaseCDataPoint.setLabel("Watts phase C");
        wattsPhaseCDataPoint.setUnits("W");
        wattsPhaseCDataPoint.setSize(1);
        wattsPhaseCDataPoint.setScaleFactorName("W_SF");
        wattsPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << wattsPhaseCDataPoint;

        SunSpecDataPoint W_SFDataPoint;
        W_SFDataPoint.setName("W_SF");
        W_SFDataPoint.setDescription("Real Power scale factor");
        W_SFDataPoint.setMandatory(true);
        W_SFDataPoint.setSize(1);
        W_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << W_SFDataPoint;

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(1);
        vaDataPoint.setScaleFactorName("VA_SF");
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << vaDataPoint;

        SunSpecDataPoint vaPhaseADataPoint;
        vaPhaseADataPoint.setName("VAphA");
        vaPhaseADataPoint.setLabel("VA phase A");
        vaPhaseADataPoint.setUnits("VA");
        vaPhaseADataPoint.setSize(1);
        vaPhaseADataPoint.setScaleFactorName("VA_SF");
        vaPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << vaPhaseADataPoint;

        SunSpecDataPoint vaPhaseBDataPoint;
        vaPhaseBDataPoint.setName("VAphB");
        vaPhaseBDataPoint.setLabel("VA phase B");
        vaPhaseBDataPoint.setUnits("VA");
        vaPhaseBDataPoint.setSize(1);
        vaPhaseBDataPoint.setScaleFactorName("VA_SF");
        vaPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << vaPhaseBDataPoint;

        SunSpecDataPoint vaPhaseCDataPoint;
        vaPhaseCDataPoint.setName("VAphC");
        vaPhaseCDataPoint.setLabel("VA phase C");
        vaPhaseCDataPoint.setUnits("VA");
        vaPhaseCDataPoint.setSize(1);
        vaPhaseCDataPoint.setScaleFactorName("VA_SF");
        vaPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << vaPhaseCDataPoint;

        SunSpecDataPoint VA_SFDataPoint;
        VA_SFDataPoint.setName("VA_SF");
        VA_SFDataPoint.setDescription("Apparent Power scale factor");
        VA_SFDataPoint.setSize(1);
        VA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << VA_SFDataPoint;

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAR");
        varDataPoint.setLabel("VAR");
        varDataPoint.setDescription("Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(1);
        varDataPoint.setScaleFactorName("VAR_SF");
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << varDataPoint;

        SunSpecDataPoint varPhaseADataPoint;
        varPhaseADataPoint.setName("VARphA");
        varPhaseADataPoint.setLabel("VAR phase A");
        varPhaseADataPoint.setUnits("var");
        varPhaseADataPoint.setSize(1);
        varPhaseADataPoint.setScaleFactorName("VAR_SF");
        varPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << varPhaseADataPoint;

        SunSpecDataPoint varPhaseBDataPoint;
        varPhaseBDataPoint.setName("VARphB");
        varPhaseBDataPoint.setLabel("VAR phase B");
        varPhaseBDataPoint.setUnits("var");
        varPhaseBDataPoint.setSize(1);
        varPhaseBDataPoint.setScaleFactorName("VAR_SF");
        varPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << varPhaseBDataPoint;

        SunSpecDataPoint varPhaseCDataPoint;
        varPhaseCDataPoint.setName("VARphC");
        varPhaseCDataPoint.setLabel("VAR phase C");
        varPhaseCDataPoint.setUnits("var");
        varPhaseCDataPoint.setSize(1);
        varPhaseCDataPoint.setScaleFactorName("VAR_SF");
        varPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << varPhaseCDataPoint;

        SunSpecDataPoint VAR_SFDataPoint;
        VAR_SFDataPoint.setName("VAR_SF");
        VAR_SFDataPoint.setDescription("Reactive Power scale factor");
        VAR_SFDataPoint.setSize(1);
        VAR_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << VAR_SFDataPoint;

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("Power Factor");
        pfDataPoint.setUnits("Pct");
        pfDataPoint.setSize(1);
        pfDataPoint.setScaleFactorName("PF_SF");
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << pfDataPoint;

        SunSpecDataPoint pfPhaseADataPoint;
        pfPhaseADataPoint.setName("PFphA");
        pfPhaseADataPoint.setLabel("PF phase A");
        pfPhaseADataPoint.setUnits("Pct");
        pfPhaseADataPoint.setSize(1);
        pfPhaseADataPoint.setScaleFactorName("PF_SF");
        pfPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << pfPhaseADataPoint;

        SunSpecDataPoint pfPhaseBDataPoint;
        pfPhaseBDataPoint.setName("PFphB");
        pfPhaseBDataPoint.setLabel("PF phase B");
        pfPhaseBDataPoint.setUnits("Pct");
        pfPhaseBDataPoint.setSize(1);
        pfPhaseBDataPoint.setScaleFactorName("PF_SF");
        pfPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << pfPhaseBDataPoint;

        SunSpecDataPoint pfPhaseCDataPoint;
        pfPhaseCDataPoint.setName("PFphC");
        pfPhaseCDataPoint.setLabel("PF phase C");
        pfPhaseCDataPoint.setUnits("Pct");
        pfPhaseCDataPoint.setSize(1);
        pfPhaseCDataPoint.setScaleFactorName("PF_SF");
        pfPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << pfPhaseCDataPoint;

        SunSpecDataPoint PF_SFDataPoint;
        PF_SFDataPoint.setName("PF_SF");
        PF_SFDataPoint.setDescription("Power Factor scale factor");
        PF_SFDataPoint.setSize(1);
        PF_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << PF_SFDataPoint;

        SunSpecDataPoint totalWattHoursExportedDataPoint;
        totalWattHoursExportedDataPoint.setName("TotWhExp");
        totalWattHoursExportedDataPoint.setLabel("Total Watt-hours Exported");
        totalWattHoursExportedDataPoint.setDescription("Total Real Energy Exported");
        totalWattHoursExportedDataPoint.setUnits("Wh");
        totalWattHoursExportedDataPoint.setMandatory(true);
        totalWattHoursExportedDataPoint.setSize(2);
        totalWattHoursExportedDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursExportedDataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseADataPoint;
        totalWattHoursExportedPhaseADataPoint.setName("TotWhExpPhA");
        totalWattHoursExportedPhaseADataPoint.setLabel("Total Watt-hours Exported phase A");
        totalWattHoursExportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseADataPoint.setSize(2);
        totalWattHoursExportedPhaseADataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursExportedPhaseADataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseBDataPoint;
        totalWattHoursExportedPhaseBDataPoint.setName("TotWhExpPhB");
        totalWattHoursExportedPhaseBDataPoint.setLabel("Total Watt-hours Exported phase B");
        totalWattHoursExportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseBDataPoint.setSize(2);
        totalWattHoursExportedPhaseBDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursExportedPhaseBDataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseCDataPoint;
        totalWattHoursExportedPhaseCDataPoint.setName("TotWhExpPhC");
        totalWattHoursExportedPhaseCDataPoint.setLabel("Total Watt-hours Exported phase C");
        totalWattHoursExportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseCDataPoint.setSize(2);
        totalWattHoursExportedPhaseCDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursExportedPhaseCDataPoint;

        SunSpecDataPoint totalWattHoursImportedDataPoint;
        totalWattHoursImportedDataPoint.setName("TotWhImp");
        totalWattHoursImportedDataPoint.setLabel("Total Watt-hours Imported");
        totalWattHoursImportedDataPoint.setDescription("Total Real Energy Imported");
        totalWattHoursImportedDataPoint.setUnits("Wh");
        totalWattHoursImportedDataPoint.setMandatory(true);
        totalWattHoursImportedDataPoint.setSize(2);
        totalWattHoursImportedDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursImportedDataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseADataPoint;
        totalWattHoursImportedPhaseADataPoint.setName("TotWhImpPhA");
        totalWattHoursImportedPhaseADataPoint.setLabel("Total Watt-hours Imported phase A");
        totalWattHoursImportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseADataPoint.setSize(2);
        totalWattHoursImportedPhaseADataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursImportedPhaseADataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseBDataPoint;
        totalWattHoursImportedPhaseBDataPoint.setName("TotWhImpPhB");
        totalWattHoursImportedPhaseBDataPoint.setLabel("Total Watt-hours Imported phase B");
        totalWattHoursImportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseBDataPoint.setSize(2);
        totalWattHoursImportedPhaseBDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursImportedPhaseBDataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseCDataPoint;
        totalWattHoursImportedPhaseCDataPoint.setName("TotWhImpPhC");
        totalWattHoursImportedPhaseCDataPoint.setLabel("Total Watt-hours Imported phase C");
        totalWattHoursImportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseCDataPoint.setSize(2);
        totalWattHoursImportedPhaseCDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursImportedPhaseCDataPoint;

        SunSpecDataPoint TotWh_SFDataPoint;
        TotWh_SFDataPoint.setName("TotWh_SF");
        TotWh_SFDataPoint.setDescription("Real Energy scale factor");
        TotWh_SFDataPoint.setMandatory(true);
        TotWh_SFDataPoint.setSize(1);
        TotWh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << TotWh_SFDataPoint;

        SunSpecDataPoint totalVaHoursExportedDataPoint;
        totalVaHoursExportedDataPoint.setName("TotVAhExp");
        totalVaHoursExportedDataPoint.setLabel("Total VA-hours Exported");
        totalVaHoursExportedDataPoint.setDescription("Total Apparent Energy Exported");
        totalVaHoursExportedDataPoint.setUnits("VAh");
        totalVaHoursExportedDataPoint.setSize(2);
        totalVaHoursExportedDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursExportedDataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseADataPoint;
        totalVaHoursExportedPhaseADataPoint.setName("TotVAhExpPhA");
        totalVaHoursExportedPhaseADataPoint.setLabel("Total VA-hours Exported phase A");
        totalVaHoursExportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseADataPoint.setSize(2);
        totalVaHoursExportedPhaseADataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursExportedPhaseADataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseBDataPoint;
        totalVaHoursExportedPhaseBDataPoint.setName("TotVAhExpPhB");
        totalVaHoursExportedPhaseBDataPoint.setLabel("Total VA-hours Exported phase B");
        totalVaHoursExportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseBDataPoint.setSize(2);
        totalVaHoursExportedPhaseBDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursExportedPhaseBDataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseCDataPoint;
        totalVaHoursExportedPhaseCDataPoint.setName("TotVAhExpPhC");
        totalVaHoursExportedPhaseCDataPoint.setLabel("Total VA-hours Exported phase C");
        totalVaHoursExportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseCDataPoint.setSize(2);
        totalVaHoursExportedPhaseCDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursExportedPhaseCDataPoint;

        SunSpecDataPoint totalVaHoursImportedDataPoint;
        totalVaHoursImportedDataPoint.setName("TotVAhImp");
        totalVaHoursImportedDataPoint.setLabel("Total VA-hours Imported");
        totalVaHoursImportedDataPoint.setDescription("Total Apparent Energy Imported");
        totalVaHoursImportedDataPoint.setUnits("VAh");
        totalVaHoursImportedDataPoint.setSize(2);
        totalVaHoursImportedDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursImportedDataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseADataPoint;
        totalVaHoursImportedPhaseADataPoint.setName("TotVAhImpPhA");
        totalVaHoursImportedPhaseADataPoint.setLabel("Total VA-hours Imported phase A");
        totalVaHoursImportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseADataPoint.setSize(2);
        totalVaHoursImportedPhaseADataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursImportedPhaseADataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseBDataPoint;
        totalVaHoursImportedPhaseBDataPoint.setName("TotVAhImpPhB");
        totalVaHoursImportedPhaseBDataPoint.setLabel("Total VA-hours Imported phase B");
        totalVaHoursImportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseBDataPoint.setSize(2);
        totalVaHoursImportedPhaseBDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursImportedPhaseBDataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseCDataPoint;
        totalVaHoursImportedPhaseCDataPoint.setName("TotVAhImpPhC");
        totalVaHoursImportedPhaseCDataPoint.setLabel("Total VA-hours Imported phase C");
        totalVaHoursImportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseCDataPoint.setSize(2);
        totalVaHoursImportedPhaseCDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursImportedPhaseCDataPoint;

        SunSpecDataPoint TotVAh_SFDataPoint;
        TotVAh_SFDataPoint.setName("TotVAh_SF");
        TotVAh_SFDataPoint.setDescription("Apparent Energy scale factor");
        TotVAh_SFDataPoint.setSize(1);
        TotVAh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << TotVAh_SFDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1DataPoint;
        totalVarHoursImportedQ1DataPoint.setName("TotVArhImpQ1");
        totalVarHoursImportedQ1DataPoint.setLabel("Total VAR-hours Imported Q1");
        totalVarHoursImportedQ1DataPoint.setDescription("Total Reactive Energy Imported Quadrant 1");
        totalVarHoursImportedQ1DataPoint.setUnits("varh");
        totalVarHoursImportedQ1DataPoint.setSize(2);
        totalVarHoursImportedQ1DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ1DataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseADataPoint;
        totalVarHoursImportedQ1PhaseADataPoint.setName("TotVArhImpQ1PhA");
        totalVarHoursImportedQ1PhaseADataPoint.setLabel("Total VAr-hours Imported Q1 phase A");
        totalVarHoursImportedQ1PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseADataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseBDataPoint;
        totalVarHoursImportedQ1PhaseBDataPoint.setName("TotVArhImpQ1PhB");
        totalVarHoursImportedQ1PhaseBDataPoint.setLabel("Total VAr-hours Imported Q1 phase B");
        totalVarHoursImportedQ1PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseCDataPoint;
        totalVarHoursImportedQ1PhaseCDataPoint.setName("TotVArhImpQ1PhC");
        totalVarHoursImportedQ1PhaseCDataPoint.setLabel("Total VAr-hours Imported Q1 phase C");
        totalVarHoursImportedQ1PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2DataPoint;
        totalVarHoursImportedQ2DataPoint.setName("TotVArhImpQ2");
        totalVarHoursImportedQ2DataPoint.setLabel("Total VAr-hours Imported Q2");
        totalVarHoursImportedQ2DataPoint.setDescription("Total Reactive Power Imported Quadrant 2");
        totalVarHoursImportedQ2DataPoint.setUnits("varh");
        totalVarHoursImportedQ2DataPoint.setSize(2);
        totalVarHoursImportedQ2DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ2DataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseADataPoint;
        totalVarHoursImportedQ2PhaseADataPoint.setName("TotVArhImpQ2PhA");
        totalVarHoursImportedQ2PhaseADataPoint.setLabel("Total VAr-hours Imported Q2 phase A");
        totalVarHoursImportedQ2PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseADataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseBDataPoint;
        totalVarHoursImportedQ2PhaseBDataPoint.setName("TotVArhImpQ2PhB");
        totalVarHoursImportedQ2PhaseBDataPoint.setLabel("Total VAr-hours Imported Q2 phase B");
        totalVarHoursImportedQ2PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseCDataPoint;
        totalVarHoursImportedQ2PhaseCDataPoint.setName("TotVArhImpQ2PhC");
        totalVarHoursImportedQ2PhaseCDataPoint.setLabel("Total VAr-hours Imported Q2 phase C");
        totalVarHoursImportedQ2PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3DataPoint;
        totalVarHoursExportedQ3DataPoint.setName("TotVArhExpQ3");
        totalVarHoursExportedQ3DataPoint.setLabel("Total VAr-hours Exported Q3");
        totalVarHoursExportedQ3DataPoint.setDescription("Total Reactive Power Exported Quadrant 3");
        totalVarHoursExportedQ3DataPoint.setUnits("varh");
        totalVarHoursExportedQ3DataPoint.setSize(2);
        totalVarHoursExportedQ3DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ3DataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseADataPoint;
        totalVarHoursExportedQ3PhaseADataPoint.setName("TotVArhExpQ3PhA");
        totalVarHoursExportedQ3PhaseADataPoint.setLabel("Total VAr-hours Exported Q3 phase A");
        totalVarHoursExportedQ3PhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseADataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseADataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseBDataPoint;
        totalVarHoursExportedQ3PhaseBDataPoint.setName("TotVArhExpQ3PhB");
        totalVarHoursExportedQ3PhaseBDataPoint.setLabel("Total VAr-hours Exported Q3 phase B");
        totalVarHoursExportedQ3PhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseCDataPoint;
        totalVarHoursExportedQ3PhaseCDataPoint.setName("TotVArhExpQ3PhC");
        totalVarHoursExportedQ3PhaseCDataPoint.setLabel("Total VAr-hours Exported Q3 phase C");
        totalVarHoursExportedQ3PhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4DataPoint;
        totalVarHoursExportedQ4DataPoint.setName("TotVArhExpQ4");
        totalVarHoursExportedQ4DataPoint.setLabel("Total VAr-hours Exported Q4");
        totalVarHoursExportedQ4DataPoint.setDescription("Total Reactive Power Exported Quadrant 4");
        totalVarHoursExportedQ4DataPoint.setUnits("varh");
        totalVarHoursExportedQ4DataPoint.setSize(2);
        totalVarHoursExportedQ4DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ4DataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseADataPoint;
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setName("TotVArhExpQ4PhA");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase A");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseADataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseBDataPoint;
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setName("TotVArhExpQ4PhB");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase B");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseBDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseCDataPoint;
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setName("TotVArhExpQ4PhC");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase C");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseCDataPoint;

        SunSpecDataPoint TotVArh_SFDataPoint;
        TotVArh_SFDataPoint.setName("TotVArh_SF");
        TotVArh_SFDataPoint.setDescription("Reactive Energy scale factor");
        TotVArh_SFDataPoint.setSize(1);
        TotVArh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << TotVArh_SFDataPoint;

        SunSpecDataPoint eventsDataPoint;
        eventsDataPoint.setName("Evt");
        eventsDataPoint.setLabel("Events");
        eventsDataPoint.setDescription("Meter Event Flags");
        eventsDataPoint.setMandatory(true);
        eventsDataPoint.setSize(2);
        eventsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << eventsDataPoint;

        break;
    }
    case 204: {
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
        ampsDataPoint.setDescription("Total AC Current");
        ampsDataPoint.setUnits("A");
        ampsDataPoint.setMandatory(true);
        ampsDataPoint.setSize(1);
        ampsDataPoint.setScaleFactorName("A_SF");
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << ampsDataPoint;

        SunSpecDataPoint ampsPhaseaDataPoint;
        ampsPhaseaDataPoint.setName("AphA");
        ampsPhaseaDataPoint.setLabel("Amps PhaseA");
        ampsPhaseaDataPoint.setDescription("Phase A Current");
        ampsPhaseaDataPoint.setUnits("A");
        ampsPhaseaDataPoint.setMandatory(true);
        ampsPhaseaDataPoint.setSize(1);
        ampsPhaseaDataPoint.setScaleFactorName("A_SF");
        ampsPhaseaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << ampsPhaseaDataPoint;

        SunSpecDataPoint ampsPhasebDataPoint;
        ampsPhasebDataPoint.setName("AphB");
        ampsPhasebDataPoint.setLabel("Amps PhaseB");
        ampsPhasebDataPoint.setDescription("Phase B Current");
        ampsPhasebDataPoint.setUnits("A");
        ampsPhasebDataPoint.setMandatory(true);
        ampsPhasebDataPoint.setSize(1);
        ampsPhasebDataPoint.setScaleFactorName("A_SF");
        ampsPhasebDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << ampsPhasebDataPoint;

        SunSpecDataPoint ampsPhasecDataPoint;
        ampsPhasecDataPoint.setName("AphC");
        ampsPhasecDataPoint.setLabel("Amps PhaseC");
        ampsPhasecDataPoint.setDescription("Phase C Current");
        ampsPhasecDataPoint.setUnits("A");
        ampsPhasecDataPoint.setMandatory(true);
        ampsPhasecDataPoint.setSize(1);
        ampsPhasecDataPoint.setScaleFactorName("A_SF");
        ampsPhasecDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << ampsPhasecDataPoint;

        SunSpecDataPoint A_SFDataPoint;
        A_SFDataPoint.setName("A_SF");
        A_SFDataPoint.setDescription("Current scale factor");
        A_SFDataPoint.setMandatory(true);
        A_SFDataPoint.setSize(1);
        A_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << A_SFDataPoint;

        SunSpecDataPoint voltageLnDataPoint;
        voltageLnDataPoint.setName("PhV");
        voltageLnDataPoint.setLabel("Voltage LN");
        voltageLnDataPoint.setDescription("Line to Neutral AC Voltage (average of active phases)");
        voltageLnDataPoint.setUnits("V");
        voltageLnDataPoint.setSize(1);
        voltageLnDataPoint.setScaleFactorName("V_SF");
        voltageLnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << voltageLnDataPoint;

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
        phaseVoltageAnDataPoint.setSize(1);
        phaseVoltageAnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageAnDataPoint;

        SunSpecDataPoint phaseVoltageBnDataPoint;
        phaseVoltageBnDataPoint.setName("PhVphB");
        phaseVoltageBnDataPoint.setLabel("Phase Voltage BN");
        phaseVoltageBnDataPoint.setDescription("Phase Voltage BN");
        phaseVoltageBnDataPoint.setUnits("V");
        phaseVoltageBnDataPoint.setSize(1);
        phaseVoltageBnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageBnDataPoint;

        SunSpecDataPoint phaseVoltageCnDataPoint;
        phaseVoltageCnDataPoint.setName("PhVphC");
        phaseVoltageCnDataPoint.setLabel("Phase Voltage CN");
        phaseVoltageCnDataPoint.setDescription("Phase Voltage CN");
        phaseVoltageCnDataPoint.setUnits("V");
        phaseVoltageCnDataPoint.setSize(1);
        phaseVoltageCnDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageCnDataPoint;

        SunSpecDataPoint voltageLlDataPoint;
        voltageLlDataPoint.setName("PPV");
        voltageLlDataPoint.setLabel("Voltage LL");
        voltageLlDataPoint.setDescription("Line to Line AC Voltage (average of active phases)");
        voltageLlDataPoint.setUnits("V");
        voltageLlDataPoint.setMandatory(true);
        voltageLlDataPoint.setSize(1);
        voltageLlDataPoint.setScaleFactorName("V_SF");
        voltageLlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << voltageLlDataPoint;

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PhVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setMandatory(true);
        phaseVoltageAbDataPoint.setSize(1);
        phaseVoltageAbDataPoint.setScaleFactorName("V_SF");
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageAbDataPoint;

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PhVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setMandatory(true);
        phaseVoltageBcDataPoint.setSize(1);
        phaseVoltageBcDataPoint.setScaleFactorName("V_SF");
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageBcDataPoint;

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PhVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setMandatory(true);
        phaseVoltageCaDataPoint.setSize(1);
        phaseVoltageCaDataPoint.setScaleFactorName("V_SF");
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << phaseVoltageCaDataPoint;

        SunSpecDataPoint V_SFDataPoint;
        V_SFDataPoint.setName("V_SF");
        V_SFDataPoint.setDescription("Voltage scale factor");
        V_SFDataPoint.setMandatory(true);
        V_SFDataPoint.setSize(1);
        V_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << V_SFDataPoint;

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(1);
        hzDataPoint.setScaleFactorName("Hz_SF");
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << hzDataPoint;

        SunSpecDataPoint Hz_SFDataPoint;
        Hz_SFDataPoint.setName("Hz_SF");
        Hz_SFDataPoint.setDescription("Frequency scale factor");
        Hz_SFDataPoint.setSize(1);
        Hz_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << Hz_SFDataPoint;

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("Total Real Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(1);
        wattsDataPoint.setScaleFactorName("W_SF");
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << wattsDataPoint;

        SunSpecDataPoint wattsPhaseADataPoint;
        wattsPhaseADataPoint.setName("WphA");
        wattsPhaseADataPoint.setLabel("Watts phase A");
        wattsPhaseADataPoint.setUnits("W");
        wattsPhaseADataPoint.setSize(1);
        wattsPhaseADataPoint.setScaleFactorName("W_SF");
        wattsPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << wattsPhaseADataPoint;

        SunSpecDataPoint wattsPhaseBDataPoint;
        wattsPhaseBDataPoint.setName("WphB");
        wattsPhaseBDataPoint.setLabel("Watts phase B");
        wattsPhaseBDataPoint.setUnits("W");
        wattsPhaseBDataPoint.setSize(1);
        wattsPhaseBDataPoint.setScaleFactorName("W_SF");
        wattsPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << wattsPhaseBDataPoint;

        SunSpecDataPoint wattsPhaseCDataPoint;
        wattsPhaseCDataPoint.setName("WphC");
        wattsPhaseCDataPoint.setLabel("Watts phase C");
        wattsPhaseCDataPoint.setUnits("W");
        wattsPhaseCDataPoint.setSize(1);
        wattsPhaseCDataPoint.setScaleFactorName("W_SF");
        wattsPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << wattsPhaseCDataPoint;

        SunSpecDataPoint W_SFDataPoint;
        W_SFDataPoint.setName("W_SF");
        W_SFDataPoint.setDescription("Real Power scale factor");
        W_SFDataPoint.setMandatory(true);
        W_SFDataPoint.setSize(1);
        W_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << W_SFDataPoint;

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(1);
        vaDataPoint.setScaleFactorName("VA_SF");
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << vaDataPoint;

        SunSpecDataPoint vaPhaseADataPoint;
        vaPhaseADataPoint.setName("VAphA");
        vaPhaseADataPoint.setLabel("VA phase A");
        vaPhaseADataPoint.setUnits("VA");
        vaPhaseADataPoint.setSize(1);
        vaPhaseADataPoint.setScaleFactorName("VA_SF");
        vaPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << vaPhaseADataPoint;

        SunSpecDataPoint vaPhaseBDataPoint;
        vaPhaseBDataPoint.setName("VAphB");
        vaPhaseBDataPoint.setLabel("VA phase B");
        vaPhaseBDataPoint.setUnits("VA");
        vaPhaseBDataPoint.setSize(1);
        vaPhaseBDataPoint.setScaleFactorName("VA_SF");
        vaPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << vaPhaseBDataPoint;

        SunSpecDataPoint vaPhaseCDataPoint;
        vaPhaseCDataPoint.setName("VAphC");
        vaPhaseCDataPoint.setLabel("VA phase C");
        vaPhaseCDataPoint.setUnits("VA");
        vaPhaseCDataPoint.setSize(1);
        vaPhaseCDataPoint.setScaleFactorName("VA_SF");
        vaPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << vaPhaseCDataPoint;

        SunSpecDataPoint VA_SFDataPoint;
        VA_SFDataPoint.setName("VA_SF");
        VA_SFDataPoint.setDescription("Apparent Power scale factor");
        VA_SFDataPoint.setSize(1);
        VA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << VA_SFDataPoint;

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAR");
        varDataPoint.setLabel("VAR");
        varDataPoint.setDescription("Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(1);
        varDataPoint.setScaleFactorName("VAR_SF");
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << varDataPoint;

        SunSpecDataPoint varPhaseADataPoint;
        varPhaseADataPoint.setName("VARphA");
        varPhaseADataPoint.setLabel("VAR phase A");
        varPhaseADataPoint.setUnits("var");
        varPhaseADataPoint.setSize(1);
        varPhaseADataPoint.setScaleFactorName("VAR_SF");
        varPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << varPhaseADataPoint;

        SunSpecDataPoint varPhaseBDataPoint;
        varPhaseBDataPoint.setName("VARphB");
        varPhaseBDataPoint.setLabel("VAR phase B");
        varPhaseBDataPoint.setUnits("var");
        varPhaseBDataPoint.setSize(1);
        varPhaseBDataPoint.setScaleFactorName("VAR_SF");
        varPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << varPhaseBDataPoint;

        SunSpecDataPoint varPhaseCDataPoint;
        varPhaseCDataPoint.setName("VARphC");
        varPhaseCDataPoint.setLabel("VAR phase C");
        varPhaseCDataPoint.setUnits("var");
        varPhaseCDataPoint.setSize(1);
        varPhaseCDataPoint.setScaleFactorName("VAR_SF");
        varPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << varPhaseCDataPoint;

        SunSpecDataPoint VAR_SFDataPoint;
        VAR_SFDataPoint.setName("VAR_SF");
        VAR_SFDataPoint.setDescription("Reactive Power scale factor");
        VAR_SFDataPoint.setSize(1);
        VAR_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << VAR_SFDataPoint;

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("Power Factor");
        pfDataPoint.setUnits("Pct");
        pfDataPoint.setSize(1);
        pfDataPoint.setScaleFactorName("PF_SF");
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << pfDataPoint;

        SunSpecDataPoint pfPhaseADataPoint;
        pfPhaseADataPoint.setName("PFphA");
        pfPhaseADataPoint.setLabel("PF phase A");
        pfPhaseADataPoint.setUnits("Pct");
        pfPhaseADataPoint.setSize(1);
        pfPhaseADataPoint.setScaleFactorName("PF_SF");
        pfPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << pfPhaseADataPoint;

        SunSpecDataPoint pfPhaseBDataPoint;
        pfPhaseBDataPoint.setName("PFphB");
        pfPhaseBDataPoint.setLabel("PF phase B");
        pfPhaseBDataPoint.setUnits("Pct");
        pfPhaseBDataPoint.setSize(1);
        pfPhaseBDataPoint.setScaleFactorName("PF_SF");
        pfPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << pfPhaseBDataPoint;

        SunSpecDataPoint pfPhaseCDataPoint;
        pfPhaseCDataPoint.setName("PFphC");
        pfPhaseCDataPoint.setLabel("PF phase C");
        pfPhaseCDataPoint.setUnits("Pct");
        pfPhaseCDataPoint.setSize(1);
        pfPhaseCDataPoint.setScaleFactorName("PF_SF");
        pfPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << pfPhaseCDataPoint;

        SunSpecDataPoint PF_SFDataPoint;
        PF_SFDataPoint.setName("PF_SF");
        PF_SFDataPoint.setDescription("Power Factor scale factor");
        PF_SFDataPoint.setSize(1);
        PF_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << PF_SFDataPoint;

        SunSpecDataPoint totalWattHoursExportedDataPoint;
        totalWattHoursExportedDataPoint.setName("TotWhExp");
        totalWattHoursExportedDataPoint.setLabel("Total Watt-hours Exported");
        totalWattHoursExportedDataPoint.setDescription("Total Real Energy Exported");
        totalWattHoursExportedDataPoint.setUnits("Wh");
        totalWattHoursExportedDataPoint.setMandatory(true);
        totalWattHoursExportedDataPoint.setSize(2);
        totalWattHoursExportedDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursExportedDataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseADataPoint;
        totalWattHoursExportedPhaseADataPoint.setName("TotWhExpPhA");
        totalWattHoursExportedPhaseADataPoint.setLabel("Total Watt-hours Exported phase A");
        totalWattHoursExportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseADataPoint.setSize(2);
        totalWattHoursExportedPhaseADataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursExportedPhaseADataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseBDataPoint;
        totalWattHoursExportedPhaseBDataPoint.setName("TotWhExpPhB");
        totalWattHoursExportedPhaseBDataPoint.setLabel("Total Watt-hours Exported phase B");
        totalWattHoursExportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseBDataPoint.setSize(2);
        totalWattHoursExportedPhaseBDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursExportedPhaseBDataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseCDataPoint;
        totalWattHoursExportedPhaseCDataPoint.setName("TotWhExpPhC");
        totalWattHoursExportedPhaseCDataPoint.setLabel("Total Watt-hours Exported phase C");
        totalWattHoursExportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseCDataPoint.setSize(2);
        totalWattHoursExportedPhaseCDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursExportedPhaseCDataPoint;

        SunSpecDataPoint totalWattHoursImportedDataPoint;
        totalWattHoursImportedDataPoint.setName("TotWhImp");
        totalWattHoursImportedDataPoint.setLabel("Total Watt-hours Imported");
        totalWattHoursImportedDataPoint.setDescription("Total Real Energy Imported");
        totalWattHoursImportedDataPoint.setUnits("Wh");
        totalWattHoursImportedDataPoint.setMandatory(true);
        totalWattHoursImportedDataPoint.setSize(2);
        totalWattHoursImportedDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursImportedDataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseADataPoint;
        totalWattHoursImportedPhaseADataPoint.setName("TotWhImpPhA");
        totalWattHoursImportedPhaseADataPoint.setLabel("Total Watt-hours Imported phase A");
        totalWattHoursImportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseADataPoint.setSize(2);
        totalWattHoursImportedPhaseADataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursImportedPhaseADataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseBDataPoint;
        totalWattHoursImportedPhaseBDataPoint.setName("TotWhImpPhB");
        totalWattHoursImportedPhaseBDataPoint.setLabel("Total Watt-hours Imported phase B");
        totalWattHoursImportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseBDataPoint.setSize(2);
        totalWattHoursImportedPhaseBDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursImportedPhaseBDataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseCDataPoint;
        totalWattHoursImportedPhaseCDataPoint.setName("TotWhImpPhC");
        totalWattHoursImportedPhaseCDataPoint.setLabel("Total Watt-hours Imported phase C");
        totalWattHoursImportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseCDataPoint.setSize(2);
        totalWattHoursImportedPhaseCDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursImportedPhaseCDataPoint;

        SunSpecDataPoint TotWh_SFDataPoint;
        TotWh_SFDataPoint.setName("TotWh_SF");
        TotWh_SFDataPoint.setDescription("Real Energy scale factor");
        TotWh_SFDataPoint.setMandatory(true);
        TotWh_SFDataPoint.setSize(1);
        TotWh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << TotWh_SFDataPoint;

        SunSpecDataPoint totalVaHoursExportedDataPoint;
        totalVaHoursExportedDataPoint.setName("TotVAhExp");
        totalVaHoursExportedDataPoint.setLabel("Total VA-hours Exported");
        totalVaHoursExportedDataPoint.setDescription("Total Apparent Energy Exported");
        totalVaHoursExportedDataPoint.setUnits("VAh");
        totalVaHoursExportedDataPoint.setSize(2);
        totalVaHoursExportedDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursExportedDataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseADataPoint;
        totalVaHoursExportedPhaseADataPoint.setName("TotVAhExpPhA");
        totalVaHoursExportedPhaseADataPoint.setLabel("Total VA-hours Exported phase A");
        totalVaHoursExportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseADataPoint.setSize(2);
        totalVaHoursExportedPhaseADataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursExportedPhaseADataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseBDataPoint;
        totalVaHoursExportedPhaseBDataPoint.setName("TotVAhExpPhB");
        totalVaHoursExportedPhaseBDataPoint.setLabel("Total VA-hours Exported phase B");
        totalVaHoursExportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseBDataPoint.setSize(2);
        totalVaHoursExportedPhaseBDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursExportedPhaseBDataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseCDataPoint;
        totalVaHoursExportedPhaseCDataPoint.setName("TotVAhExpPhC");
        totalVaHoursExportedPhaseCDataPoint.setLabel("Total VA-hours Exported phase C");
        totalVaHoursExportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseCDataPoint.setSize(2);
        totalVaHoursExportedPhaseCDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursExportedPhaseCDataPoint;

        SunSpecDataPoint totalVaHoursImportedDataPoint;
        totalVaHoursImportedDataPoint.setName("TotVAhImp");
        totalVaHoursImportedDataPoint.setLabel("Total VA-hours Imported");
        totalVaHoursImportedDataPoint.setDescription("Total Apparent Energy Imported");
        totalVaHoursImportedDataPoint.setUnits("VAh");
        totalVaHoursImportedDataPoint.setSize(2);
        totalVaHoursImportedDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursImportedDataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseADataPoint;
        totalVaHoursImportedPhaseADataPoint.setName("TotVAhImpPhA");
        totalVaHoursImportedPhaseADataPoint.setLabel("Total VA-hours Imported phase A");
        totalVaHoursImportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseADataPoint.setSize(2);
        totalVaHoursImportedPhaseADataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursImportedPhaseADataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseBDataPoint;
        totalVaHoursImportedPhaseBDataPoint.setName("TotVAhImpPhB");
        totalVaHoursImportedPhaseBDataPoint.setLabel("Total VA-hours Imported phase B");
        totalVaHoursImportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseBDataPoint.setSize(2);
        totalVaHoursImportedPhaseBDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursImportedPhaseBDataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseCDataPoint;
        totalVaHoursImportedPhaseCDataPoint.setName("TotVAhImpPhC");
        totalVaHoursImportedPhaseCDataPoint.setLabel("Total VA-hours Imported phase C");
        totalVaHoursImportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseCDataPoint.setSize(2);
        totalVaHoursImportedPhaseCDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursImportedPhaseCDataPoint;

        SunSpecDataPoint TotVAh_SFDataPoint;
        TotVAh_SFDataPoint.setName("TotVAh_SF");
        TotVAh_SFDataPoint.setDescription("Apparent Energy scale factor");
        TotVAh_SFDataPoint.setSize(1);
        TotVAh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << TotVAh_SFDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1DataPoint;
        totalVarHoursImportedQ1DataPoint.setName("TotVArhImpQ1");
        totalVarHoursImportedQ1DataPoint.setLabel("Total VAR-hours Imported Q1");
        totalVarHoursImportedQ1DataPoint.setDescription("Total Reactive Energy Imported Quadrant 1");
        totalVarHoursImportedQ1DataPoint.setUnits("varh");
        totalVarHoursImportedQ1DataPoint.setSize(2);
        totalVarHoursImportedQ1DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ1DataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseADataPoint;
        totalVarHoursImportedQ1PhaseADataPoint.setName("TotVArhImpQ1PhA");
        totalVarHoursImportedQ1PhaseADataPoint.setLabel("Total VAr-hours Imported Q1 phase A");
        totalVarHoursImportedQ1PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseADataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseBDataPoint;
        totalVarHoursImportedQ1PhaseBDataPoint.setName("TotVArhImpQ1PhB");
        totalVarHoursImportedQ1PhaseBDataPoint.setLabel("Total VAr-hours Imported Q1 phase B");
        totalVarHoursImportedQ1PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseCDataPoint;
        totalVarHoursImportedQ1PhaseCDataPoint.setName("TotVArhImpQ1PhC");
        totalVarHoursImportedQ1PhaseCDataPoint.setLabel("Total VAr-hours Imported Q1 phase C");
        totalVarHoursImportedQ1PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2DataPoint;
        totalVarHoursImportedQ2DataPoint.setName("TotVArhImpQ2");
        totalVarHoursImportedQ2DataPoint.setLabel("Total VAr-hours Imported Q2");
        totalVarHoursImportedQ2DataPoint.setDescription("Total Reactive Power Imported Quadrant 2");
        totalVarHoursImportedQ2DataPoint.setUnits("varh");
        totalVarHoursImportedQ2DataPoint.setSize(2);
        totalVarHoursImportedQ2DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ2DataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseADataPoint;
        totalVarHoursImportedQ2PhaseADataPoint.setName("TotVArhImpQ2PhA");
        totalVarHoursImportedQ2PhaseADataPoint.setLabel("Total VAr-hours Imported Q2 phase A");
        totalVarHoursImportedQ2PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseADataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseBDataPoint;
        totalVarHoursImportedQ2PhaseBDataPoint.setName("TotVArhImpQ2PhB");
        totalVarHoursImportedQ2PhaseBDataPoint.setLabel("Total VAr-hours Imported Q2 phase B");
        totalVarHoursImportedQ2PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseCDataPoint;
        totalVarHoursImportedQ2PhaseCDataPoint.setName("TotVArhImpQ2PhC");
        totalVarHoursImportedQ2PhaseCDataPoint.setLabel("Total VAr-hours Imported Q2 phase C");
        totalVarHoursImportedQ2PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3DataPoint;
        totalVarHoursExportedQ3DataPoint.setName("TotVArhExpQ3");
        totalVarHoursExportedQ3DataPoint.setLabel("Total VAr-hours Exported Q3");
        totalVarHoursExportedQ3DataPoint.setDescription("Total Reactive Power Exported Quadrant 3");
        totalVarHoursExportedQ3DataPoint.setUnits("varh");
        totalVarHoursExportedQ3DataPoint.setSize(2);
        totalVarHoursExportedQ3DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ3DataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseADataPoint;
        totalVarHoursExportedQ3PhaseADataPoint.setName("TotVArhExpQ3PhA");
        totalVarHoursExportedQ3PhaseADataPoint.setLabel("Total VAr-hours Exported Q3 phase A");
        totalVarHoursExportedQ3PhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseADataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseADataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseBDataPoint;
        totalVarHoursExportedQ3PhaseBDataPoint.setName("TotVArhExpQ3PhB");
        totalVarHoursExportedQ3PhaseBDataPoint.setLabel("Total VAr-hours Exported Q3 phase B");
        totalVarHoursExportedQ3PhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseCDataPoint;
        totalVarHoursExportedQ3PhaseCDataPoint.setName("TotVArhExpQ3PhC");
        totalVarHoursExportedQ3PhaseCDataPoint.setLabel("Total VAr-hours Exported Q3 phase C");
        totalVarHoursExportedQ3PhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4DataPoint;
        totalVarHoursExportedQ4DataPoint.setName("TotVArhExpQ4");
        totalVarHoursExportedQ4DataPoint.setLabel("Total VAr-hours Exported Q4");
        totalVarHoursExportedQ4DataPoint.setDescription("Total Reactive Power Exported Quadrant 4");
        totalVarHoursExportedQ4DataPoint.setUnits("varh");
        totalVarHoursExportedQ4DataPoint.setSize(2);
        totalVarHoursExportedQ4DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ4DataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseADataPoint;
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setName("TotVArhExpQ4PhA");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase A");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseADataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseBDataPoint;
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setName("TotVArhExpQ4PhB");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase B");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseBDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseCDataPoint;
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setName("TotVArhExpQ4PhC");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase C");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseCDataPoint;

        SunSpecDataPoint TotVArh_SFDataPoint;
        TotVArh_SFDataPoint.setName("TotVArh_SF");
        TotVArh_SFDataPoint.setDescription("Reactive Energy scale factor");
        TotVArh_SFDataPoint.setSize(1);
        TotVArh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << TotVArh_SFDataPoint;

        SunSpecDataPoint eventsDataPoint;
        eventsDataPoint.setName("Evt");
        eventsDataPoint.setLabel("Events");
        eventsDataPoint.setDescription("Meter Event Flags");
        eventsDataPoint.setMandatory(true);
        eventsDataPoint.setSize(2);
        eventsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << eventsDataPoint;

        break;
    }
    case 211: {
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
        ampsDataPoint.setDescription("Total AC Current");
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

        SunSpecDataPoint voltageLnDataPoint;
        voltageLnDataPoint.setName("PhV");
        voltageLnDataPoint.setLabel("Voltage LN");
        voltageLnDataPoint.setDescription("Line to Neutral AC Voltage (average of active phases)");
        voltageLnDataPoint.setUnits("V");
        voltageLnDataPoint.setSize(2);
        voltageLnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << voltageLnDataPoint;

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
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

        SunSpecDataPoint voltageLlDataPoint;
        voltageLlDataPoint.setName("PPV");
        voltageLlDataPoint.setLabel("Voltage LL");
        voltageLlDataPoint.setDescription("Line to Line AC Voltage (average of active phases)");
        voltageLlDataPoint.setUnits("V");
        voltageLlDataPoint.setSize(2);
        voltageLlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << voltageLlDataPoint;

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

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(2);
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << hzDataPoint;

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("Total Real Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(2);
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << wattsDataPoint;

        SunSpecDataPoint wattsPhaseADataPoint;
        wattsPhaseADataPoint.setName("WphA");
        wattsPhaseADataPoint.setLabel("Watts phase A");
        wattsPhaseADataPoint.setUnits("W");
        wattsPhaseADataPoint.setSize(2);
        wattsPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << wattsPhaseADataPoint;

        SunSpecDataPoint wattsPhaseBDataPoint;
        wattsPhaseBDataPoint.setName("WphB");
        wattsPhaseBDataPoint.setLabel("Watts phase B");
        wattsPhaseBDataPoint.setUnits("W");
        wattsPhaseBDataPoint.setSize(2);
        wattsPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << wattsPhaseBDataPoint;

        SunSpecDataPoint wattsPhaseCDataPoint;
        wattsPhaseCDataPoint.setName("WphC");
        wattsPhaseCDataPoint.setLabel("Watts phase C");
        wattsPhaseCDataPoint.setUnits("W");
        wattsPhaseCDataPoint.setSize(2);
        wattsPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << wattsPhaseCDataPoint;

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(2);
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << vaDataPoint;

        SunSpecDataPoint vaPhaseADataPoint;
        vaPhaseADataPoint.setName("VAphA");
        vaPhaseADataPoint.setLabel("VA phase A");
        vaPhaseADataPoint.setUnits("VA");
        vaPhaseADataPoint.setSize(2);
        vaPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << vaPhaseADataPoint;

        SunSpecDataPoint vaPhaseBDataPoint;
        vaPhaseBDataPoint.setName("VAphB");
        vaPhaseBDataPoint.setLabel("VA phase B");
        vaPhaseBDataPoint.setUnits("VA");
        vaPhaseBDataPoint.setSize(2);
        vaPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << vaPhaseBDataPoint;

        SunSpecDataPoint vaPhaseCDataPoint;
        vaPhaseCDataPoint.setName("VAphC");
        vaPhaseCDataPoint.setLabel("VA phase C");
        vaPhaseCDataPoint.setUnits("VA");
        vaPhaseCDataPoint.setSize(2);
        vaPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << vaPhaseCDataPoint;

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAR");
        varDataPoint.setLabel("VAR");
        varDataPoint.setDescription("Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(2);
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << varDataPoint;

        SunSpecDataPoint varPhaseADataPoint;
        varPhaseADataPoint.setName("VARphA");
        varPhaseADataPoint.setLabel("VAR phase A");
        varPhaseADataPoint.setUnits("var");
        varPhaseADataPoint.setSize(2);
        varPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << varPhaseADataPoint;

        SunSpecDataPoint varPhaseBDataPoint;
        varPhaseBDataPoint.setName("VARphB");
        varPhaseBDataPoint.setLabel("VAR phase B");
        varPhaseBDataPoint.setUnits("var");
        varPhaseBDataPoint.setSize(2);
        varPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << varPhaseBDataPoint;

        SunSpecDataPoint varPhaseCDataPoint;
        varPhaseCDataPoint.setName("VARphC");
        varPhaseCDataPoint.setLabel("VAR phase C");
        varPhaseCDataPoint.setUnits("var");
        varPhaseCDataPoint.setSize(2);
        varPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << varPhaseCDataPoint;

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("Power Factor");
        pfDataPoint.setUnits("PF");
        pfDataPoint.setSize(2);
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << pfDataPoint;

        SunSpecDataPoint pfPhaseADataPoint;
        pfPhaseADataPoint.setName("PFphA");
        pfPhaseADataPoint.setLabel("PF phase A");
        pfPhaseADataPoint.setUnits("PF");
        pfPhaseADataPoint.setSize(2);
        pfPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << pfPhaseADataPoint;

        SunSpecDataPoint pfPhaseBDataPoint;
        pfPhaseBDataPoint.setName("PFphB");
        pfPhaseBDataPoint.setLabel("PF phase B");
        pfPhaseBDataPoint.setUnits("PF");
        pfPhaseBDataPoint.setSize(2);
        pfPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << pfPhaseBDataPoint;

        SunSpecDataPoint pfPhaseCDataPoint;
        pfPhaseCDataPoint.setName("PFphC");
        pfPhaseCDataPoint.setLabel("PF phase C");
        pfPhaseCDataPoint.setUnits("PF");
        pfPhaseCDataPoint.setSize(2);
        pfPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << pfPhaseCDataPoint;

        SunSpecDataPoint totalWattHoursExportedDataPoint;
        totalWattHoursExportedDataPoint.setName("TotWhExp");
        totalWattHoursExportedDataPoint.setLabel("Total Watt-hours Exported");
        totalWattHoursExportedDataPoint.setDescription("Total Real Energy Exported");
        totalWattHoursExportedDataPoint.setUnits("Wh");
        totalWattHoursExportedDataPoint.setMandatory(true);
        totalWattHoursExportedDataPoint.setSize(2);
        totalWattHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursExportedDataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseADataPoint;
        totalWattHoursExportedPhaseADataPoint.setName("TotWhExpPhA");
        totalWattHoursExportedPhaseADataPoint.setLabel("Total Watt-hours Exported phase A");
        totalWattHoursExportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseADataPoint.setSize(2);
        totalWattHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursExportedPhaseADataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseBDataPoint;
        totalWattHoursExportedPhaseBDataPoint.setName("TotWhExpPhB");
        totalWattHoursExportedPhaseBDataPoint.setLabel("Total Watt-hours Exported phase B");
        totalWattHoursExportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseBDataPoint.setSize(2);
        totalWattHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursExportedPhaseBDataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseCDataPoint;
        totalWattHoursExportedPhaseCDataPoint.setName("TotWhExpPhC");
        totalWattHoursExportedPhaseCDataPoint.setLabel("Total Watt-hours Exported phase C");
        totalWattHoursExportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseCDataPoint.setSize(2);
        totalWattHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursExportedPhaseCDataPoint;

        SunSpecDataPoint totalWattHoursImportedDataPoint;
        totalWattHoursImportedDataPoint.setName("TotWhImp");
        totalWattHoursImportedDataPoint.setLabel("Total Watt-hours Imported");
        totalWattHoursImportedDataPoint.setDescription("Total Real Energy Imported");
        totalWattHoursImportedDataPoint.setUnits("Wh");
        totalWattHoursImportedDataPoint.setMandatory(true);
        totalWattHoursImportedDataPoint.setSize(2);
        totalWattHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursImportedDataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseADataPoint;
        totalWattHoursImportedPhaseADataPoint.setName("TotWhImpPhA");
        totalWattHoursImportedPhaseADataPoint.setLabel("Total Watt-hours Imported phase A");
        totalWattHoursImportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseADataPoint.setSize(2);
        totalWattHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursImportedPhaseADataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseBDataPoint;
        totalWattHoursImportedPhaseBDataPoint.setName("TotWhImpPhB");
        totalWattHoursImportedPhaseBDataPoint.setLabel("Total Watt-hours Imported phase B");
        totalWattHoursImportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseBDataPoint.setSize(2);
        totalWattHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursImportedPhaseBDataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseCDataPoint;
        totalWattHoursImportedPhaseCDataPoint.setName("TotWhImpPhC");
        totalWattHoursImportedPhaseCDataPoint.setLabel("Total Watt-hours Imported phase C");
        totalWattHoursImportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseCDataPoint.setSize(2);
        totalWattHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursImportedPhaseCDataPoint;

        SunSpecDataPoint totalVaHoursExportedDataPoint;
        totalVaHoursExportedDataPoint.setName("TotVAhExp");
        totalVaHoursExportedDataPoint.setLabel("Total VA-hours Exported");
        totalVaHoursExportedDataPoint.setDescription("Total Apparent Energy Exported");
        totalVaHoursExportedDataPoint.setUnits("VAh");
        totalVaHoursExportedDataPoint.setSize(2);
        totalVaHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursExportedDataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseADataPoint;
        totalVaHoursExportedPhaseADataPoint.setName("TotVAhExpPhA");
        totalVaHoursExportedPhaseADataPoint.setLabel("Total VA-hours Exported phase A");
        totalVaHoursExportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseADataPoint.setSize(2);
        totalVaHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursExportedPhaseADataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseBDataPoint;
        totalVaHoursExportedPhaseBDataPoint.setName("TotVAhExpPhB");
        totalVaHoursExportedPhaseBDataPoint.setLabel("Total VA-hours Exported phase B");
        totalVaHoursExportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseBDataPoint.setSize(2);
        totalVaHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursExportedPhaseBDataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseCDataPoint;
        totalVaHoursExportedPhaseCDataPoint.setName("TotVAhExpPhC");
        totalVaHoursExportedPhaseCDataPoint.setLabel("Total VA-hours Exported phase C");
        totalVaHoursExportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseCDataPoint.setSize(2);
        totalVaHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursExportedPhaseCDataPoint;

        SunSpecDataPoint totalVaHoursImportedDataPoint;
        totalVaHoursImportedDataPoint.setName("TotVAhImp");
        totalVaHoursImportedDataPoint.setLabel("Total VA-hours Imported");
        totalVaHoursImportedDataPoint.setDescription("Total Apparent Energy Imported");
        totalVaHoursImportedDataPoint.setUnits("VAh");
        totalVaHoursImportedDataPoint.setSize(2);
        totalVaHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursImportedDataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseADataPoint;
        totalVaHoursImportedPhaseADataPoint.setName("TotVAhImpPhA");
        totalVaHoursImportedPhaseADataPoint.setLabel("Total VA-hours Imported phase A");
        totalVaHoursImportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseADataPoint.setSize(2);
        totalVaHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursImportedPhaseADataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseBDataPoint;
        totalVaHoursImportedPhaseBDataPoint.setName("TotVAhImpPhB");
        totalVaHoursImportedPhaseBDataPoint.setLabel("Total VA-hours Imported phase B");
        totalVaHoursImportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseBDataPoint.setSize(2);
        totalVaHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursImportedPhaseBDataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseCDataPoint;
        totalVaHoursImportedPhaseCDataPoint.setName("TotVAhImpPhC");
        totalVaHoursImportedPhaseCDataPoint.setLabel("Total VA-hours Imported phase C");
        totalVaHoursImportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseCDataPoint.setSize(2);
        totalVaHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursImportedPhaseCDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1DataPoint;
        totalVarHoursImportedQ1DataPoint.setName("TotVArhImpQ1");
        totalVarHoursImportedQ1DataPoint.setLabel("Total VAR-hours Imported Q1");
        totalVarHoursImportedQ1DataPoint.setDescription("Total Reactive Energy Imported Quadrant 1");
        totalVarHoursImportedQ1DataPoint.setUnits("varh");
        totalVarHoursImportedQ1DataPoint.setSize(2);
        totalVarHoursImportedQ1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ1DataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseADataPoint;
        totalVarHoursImportedQ1PhaseADataPoint.setName("TotVArhImpQ1phA");
        totalVarHoursImportedQ1PhaseADataPoint.setLabel("Total VAr-hours Imported Q1 phase A");
        totalVarHoursImportedQ1PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseADataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseBDataPoint;
        totalVarHoursImportedQ1PhaseBDataPoint.setName("TotVArhImpQ1phB");
        totalVarHoursImportedQ1PhaseBDataPoint.setLabel("Total VAr-hours Imported Q1 phase B");
        totalVarHoursImportedQ1PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseCDataPoint;
        totalVarHoursImportedQ1PhaseCDataPoint.setName("TotVArhImpQ1phC");
        totalVarHoursImportedQ1PhaseCDataPoint.setLabel("Total VAr-hours Imported Q1 phase C");
        totalVarHoursImportedQ1PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2DataPoint;
        totalVarHoursImportedQ2DataPoint.setName("TotVArhImpQ2");
        totalVarHoursImportedQ2DataPoint.setLabel("Total VAr-hours Imported Q2");
        totalVarHoursImportedQ2DataPoint.setDescription("Total Reactive Power Imported Quadrant 2");
        totalVarHoursImportedQ2DataPoint.setUnits("varh");
        totalVarHoursImportedQ2DataPoint.setSize(2);
        totalVarHoursImportedQ2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ2DataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseADataPoint;
        totalVarHoursImportedQ2PhaseADataPoint.setName("TotVArhImpQ2phA");
        totalVarHoursImportedQ2PhaseADataPoint.setLabel("Total VAr-hours Imported Q2 phase A");
        totalVarHoursImportedQ2PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseADataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseBDataPoint;
        totalVarHoursImportedQ2PhaseBDataPoint.setName("TotVArhImpQ2phB");
        totalVarHoursImportedQ2PhaseBDataPoint.setLabel("Total VAr-hours Imported Q2 phase B");
        totalVarHoursImportedQ2PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseCDataPoint;
        totalVarHoursImportedQ2PhaseCDataPoint.setName("TotVArhImpQ2phC");
        totalVarHoursImportedQ2PhaseCDataPoint.setLabel("Total VAr-hours Imported Q2 phase C");
        totalVarHoursImportedQ2PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3DataPoint;
        totalVarHoursExportedQ3DataPoint.setName("TotVArhExpQ3");
        totalVarHoursExportedQ3DataPoint.setLabel("Total VAr-hours Exported Q3");
        totalVarHoursExportedQ3DataPoint.setDescription("Total Reactive Power Exported Quadrant 3");
        totalVarHoursExportedQ3DataPoint.setUnits("varh");
        totalVarHoursExportedQ3DataPoint.setSize(2);
        totalVarHoursExportedQ3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ3DataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseADataPoint;
        totalVarHoursExportedQ3PhaseADataPoint.setName("TotVArhExpQ3phA");
        totalVarHoursExportedQ3PhaseADataPoint.setLabel("Total VAr-hours Exported Q3 phase A");
        totalVarHoursExportedQ3PhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseADataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseADataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseBDataPoint;
        totalVarHoursExportedQ3PhaseBDataPoint.setName("TotVArhExpQ3phB");
        totalVarHoursExportedQ3PhaseBDataPoint.setLabel("Total VAr-hours Exported Q3 phase B");
        totalVarHoursExportedQ3PhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseCDataPoint;
        totalVarHoursExportedQ3PhaseCDataPoint.setName("TotVArhExpQ3phC");
        totalVarHoursExportedQ3PhaseCDataPoint.setLabel("Total VAr-hours Exported Q3 phase C");
        totalVarHoursExportedQ3PhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4DataPoint;
        totalVarHoursExportedQ4DataPoint.setName("TotVArhExpQ4");
        totalVarHoursExportedQ4DataPoint.setLabel("Total VAr-hours Exported Q4");
        totalVarHoursExportedQ4DataPoint.setDescription("Total Reactive Power Exported Quadrant 4");
        totalVarHoursExportedQ4DataPoint.setUnits("varh");
        totalVarHoursExportedQ4DataPoint.setSize(2);
        totalVarHoursExportedQ4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ4DataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseADataPoint;
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setName("TotVArhExpQ4phA");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase A");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseADataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseBDataPoint;
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setName("TotVArhExpQ4phB");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase B");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseBDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseCDataPoint;
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setName("TotVArhExpQ4phC");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase C");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseCDataPoint;

        SunSpecDataPoint eventsDataPoint;
        eventsDataPoint.setName("Evt");
        eventsDataPoint.setLabel("Events");
        eventsDataPoint.setDescription("Meter Event Flags");
        eventsDataPoint.setMandatory(true);
        eventsDataPoint.setSize(2);
        eventsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << eventsDataPoint;

        break;
    }
    case 212: {
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
        ampsDataPoint.setDescription("Total AC Current");
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

        SunSpecDataPoint voltageLnDataPoint;
        voltageLnDataPoint.setName("PhV");
        voltageLnDataPoint.setLabel("Voltage LN");
        voltageLnDataPoint.setDescription("Line to Neutral AC Voltage (average of active phases)");
        voltageLnDataPoint.setUnits("V");
        voltageLnDataPoint.setMandatory(true);
        voltageLnDataPoint.setSize(2);
        voltageLnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << voltageLnDataPoint;

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

        SunSpecDataPoint voltageLlDataPoint;
        voltageLlDataPoint.setName("PPV");
        voltageLlDataPoint.setLabel("Voltage LL");
        voltageLlDataPoint.setDescription("Line to Line AC Voltage (average of active phases)");
        voltageLlDataPoint.setUnits("V");
        voltageLlDataPoint.setMandatory(true);
        voltageLlDataPoint.setSize(2);
        voltageLlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << voltageLlDataPoint;

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PPVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setMandatory(true);
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

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(2);
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << hzDataPoint;

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("Total Real Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(2);
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << wattsDataPoint;

        SunSpecDataPoint wattsPhaseADataPoint;
        wattsPhaseADataPoint.setName("WphA");
        wattsPhaseADataPoint.setLabel("Watts phase A");
        wattsPhaseADataPoint.setUnits("W");
        wattsPhaseADataPoint.setSize(2);
        wattsPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << wattsPhaseADataPoint;

        SunSpecDataPoint wattsPhaseBDataPoint;
        wattsPhaseBDataPoint.setName("WphB");
        wattsPhaseBDataPoint.setLabel("Watts phase B");
        wattsPhaseBDataPoint.setUnits("W");
        wattsPhaseBDataPoint.setSize(2);
        wattsPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << wattsPhaseBDataPoint;

        SunSpecDataPoint wattsPhaseCDataPoint;
        wattsPhaseCDataPoint.setName("WphC");
        wattsPhaseCDataPoint.setLabel("Watts phase C");
        wattsPhaseCDataPoint.setUnits("W");
        wattsPhaseCDataPoint.setSize(2);
        wattsPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << wattsPhaseCDataPoint;

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(2);
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << vaDataPoint;

        SunSpecDataPoint vaPhaseADataPoint;
        vaPhaseADataPoint.setName("VAphA");
        vaPhaseADataPoint.setLabel("VA phase A");
        vaPhaseADataPoint.setUnits("VA");
        vaPhaseADataPoint.setSize(2);
        vaPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << vaPhaseADataPoint;

        SunSpecDataPoint vaPhaseBDataPoint;
        vaPhaseBDataPoint.setName("VAphB");
        vaPhaseBDataPoint.setLabel("VA phase B");
        vaPhaseBDataPoint.setUnits("VA");
        vaPhaseBDataPoint.setSize(2);
        vaPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << vaPhaseBDataPoint;

        SunSpecDataPoint vaPhaseCDataPoint;
        vaPhaseCDataPoint.setName("VAphC");
        vaPhaseCDataPoint.setLabel("VA phase C");
        vaPhaseCDataPoint.setUnits("VA");
        vaPhaseCDataPoint.setSize(2);
        vaPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << vaPhaseCDataPoint;

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAR");
        varDataPoint.setLabel("VAR");
        varDataPoint.setDescription("Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(2);
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << varDataPoint;

        SunSpecDataPoint varPhaseADataPoint;
        varPhaseADataPoint.setName("VARphA");
        varPhaseADataPoint.setLabel("VAR phase A");
        varPhaseADataPoint.setUnits("var");
        varPhaseADataPoint.setSize(2);
        varPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << varPhaseADataPoint;

        SunSpecDataPoint varPhaseBDataPoint;
        varPhaseBDataPoint.setName("VARphB");
        varPhaseBDataPoint.setLabel("VAR phase B");
        varPhaseBDataPoint.setUnits("var");
        varPhaseBDataPoint.setSize(2);
        varPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << varPhaseBDataPoint;

        SunSpecDataPoint varPhaseCDataPoint;
        varPhaseCDataPoint.setName("VARphC");
        varPhaseCDataPoint.setLabel("VAR phase C");
        varPhaseCDataPoint.setUnits("var");
        varPhaseCDataPoint.setSize(2);
        varPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << varPhaseCDataPoint;

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("Power Factor");
        pfDataPoint.setUnits("PF");
        pfDataPoint.setSize(2);
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << pfDataPoint;

        SunSpecDataPoint pfPhaseADataPoint;
        pfPhaseADataPoint.setName("PFphA");
        pfPhaseADataPoint.setLabel("PF phase A");
        pfPhaseADataPoint.setUnits("PF");
        pfPhaseADataPoint.setSize(2);
        pfPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << pfPhaseADataPoint;

        SunSpecDataPoint pfPhaseBDataPoint;
        pfPhaseBDataPoint.setName("PFphB");
        pfPhaseBDataPoint.setLabel("PF phase B");
        pfPhaseBDataPoint.setUnits("PF");
        pfPhaseBDataPoint.setSize(2);
        pfPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << pfPhaseBDataPoint;

        SunSpecDataPoint pfPhaseCDataPoint;
        pfPhaseCDataPoint.setName("PFphC");
        pfPhaseCDataPoint.setLabel("PF phase C");
        pfPhaseCDataPoint.setUnits("PF");
        pfPhaseCDataPoint.setSize(2);
        pfPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << pfPhaseCDataPoint;

        SunSpecDataPoint totalWattHoursExportedDataPoint;
        totalWattHoursExportedDataPoint.setName("TotWhExp");
        totalWattHoursExportedDataPoint.setLabel("Total Watt-hours Exported");
        totalWattHoursExportedDataPoint.setDescription("Total Real Energy Exported");
        totalWattHoursExportedDataPoint.setUnits("Wh");
        totalWattHoursExportedDataPoint.setMandatory(true);
        totalWattHoursExportedDataPoint.setSize(2);
        totalWattHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursExportedDataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseADataPoint;
        totalWattHoursExportedPhaseADataPoint.setName("TotWhExpPhA");
        totalWattHoursExportedPhaseADataPoint.setLabel("Total Watt-hours Exported phase A");
        totalWattHoursExportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseADataPoint.setSize(2);
        totalWattHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursExportedPhaseADataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseBDataPoint;
        totalWattHoursExportedPhaseBDataPoint.setName("TotWhExpPhB");
        totalWattHoursExportedPhaseBDataPoint.setLabel("Total Watt-hours Exported phase B");
        totalWattHoursExportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseBDataPoint.setSize(2);
        totalWattHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursExportedPhaseBDataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseCDataPoint;
        totalWattHoursExportedPhaseCDataPoint.setName("TotWhExpPhC");
        totalWattHoursExportedPhaseCDataPoint.setLabel("Total Watt-hours Exported phase C");
        totalWattHoursExportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseCDataPoint.setSize(2);
        totalWattHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursExportedPhaseCDataPoint;

        SunSpecDataPoint totalWattHoursImportedDataPoint;
        totalWattHoursImportedDataPoint.setName("TotWhImp");
        totalWattHoursImportedDataPoint.setLabel("Total Watt-hours Imported");
        totalWattHoursImportedDataPoint.setDescription("Total Real Energy Imported");
        totalWattHoursImportedDataPoint.setUnits("Wh");
        totalWattHoursImportedDataPoint.setMandatory(true);
        totalWattHoursImportedDataPoint.setSize(2);
        totalWattHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursImportedDataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseADataPoint;
        totalWattHoursImportedPhaseADataPoint.setName("TotWhImpPhA");
        totalWattHoursImportedPhaseADataPoint.setLabel("Total Watt-hours Imported phase A");
        totalWattHoursImportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseADataPoint.setSize(2);
        totalWattHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursImportedPhaseADataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseBDataPoint;
        totalWattHoursImportedPhaseBDataPoint.setName("TotWhImpPhB");
        totalWattHoursImportedPhaseBDataPoint.setLabel("Total Watt-hours Imported phase B");
        totalWattHoursImportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseBDataPoint.setSize(2);
        totalWattHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursImportedPhaseBDataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseCDataPoint;
        totalWattHoursImportedPhaseCDataPoint.setName("TotWhImpPhC");
        totalWattHoursImportedPhaseCDataPoint.setLabel("Total Watt-hours Imported phase C");
        totalWattHoursImportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseCDataPoint.setSize(2);
        totalWattHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursImportedPhaseCDataPoint;

        SunSpecDataPoint totalVaHoursExportedDataPoint;
        totalVaHoursExportedDataPoint.setName("TotVAhExp");
        totalVaHoursExportedDataPoint.setLabel("Total VA-hours Exported");
        totalVaHoursExportedDataPoint.setDescription("Total Apparent Energy Exported");
        totalVaHoursExportedDataPoint.setUnits("VAh");
        totalVaHoursExportedDataPoint.setSize(2);
        totalVaHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursExportedDataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseADataPoint;
        totalVaHoursExportedPhaseADataPoint.setName("TotVAhExpPhA");
        totalVaHoursExportedPhaseADataPoint.setLabel("Total VA-hours Exported phase A");
        totalVaHoursExportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseADataPoint.setSize(2);
        totalVaHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursExportedPhaseADataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseBDataPoint;
        totalVaHoursExportedPhaseBDataPoint.setName("TotVAhExpPhB");
        totalVaHoursExportedPhaseBDataPoint.setLabel("Total VA-hours Exported phase B");
        totalVaHoursExportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseBDataPoint.setSize(2);
        totalVaHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursExportedPhaseBDataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseCDataPoint;
        totalVaHoursExportedPhaseCDataPoint.setName("TotVAhExpPhC");
        totalVaHoursExportedPhaseCDataPoint.setLabel("Total VA-hours Exported phase C");
        totalVaHoursExportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseCDataPoint.setSize(2);
        totalVaHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursExportedPhaseCDataPoint;

        SunSpecDataPoint totalVaHoursImportedDataPoint;
        totalVaHoursImportedDataPoint.setName("TotVAhImp");
        totalVaHoursImportedDataPoint.setLabel("Total VA-hours Imported");
        totalVaHoursImportedDataPoint.setDescription("Total Apparent Energy Imported");
        totalVaHoursImportedDataPoint.setUnits("VAh");
        totalVaHoursImportedDataPoint.setSize(2);
        totalVaHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursImportedDataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseADataPoint;
        totalVaHoursImportedPhaseADataPoint.setName("TotVAhImpPhA");
        totalVaHoursImportedPhaseADataPoint.setLabel("Total VA-hours Imported phase A");
        totalVaHoursImportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseADataPoint.setSize(2);
        totalVaHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursImportedPhaseADataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseBDataPoint;
        totalVaHoursImportedPhaseBDataPoint.setName("TotVAhImpPhB");
        totalVaHoursImportedPhaseBDataPoint.setLabel("Total VA-hours Imported phase B");
        totalVaHoursImportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseBDataPoint.setSize(2);
        totalVaHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursImportedPhaseBDataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseCDataPoint;
        totalVaHoursImportedPhaseCDataPoint.setName("TotVAhImpPhC");
        totalVaHoursImportedPhaseCDataPoint.setLabel("Total VA-hours Imported phase C");
        totalVaHoursImportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseCDataPoint.setSize(2);
        totalVaHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursImportedPhaseCDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1DataPoint;
        totalVarHoursImportedQ1DataPoint.setName("TotVArhImpQ1");
        totalVarHoursImportedQ1DataPoint.setLabel("Total VAR-hours Imported Q1");
        totalVarHoursImportedQ1DataPoint.setDescription("Total Reactive Energy Imported Quadrant 1");
        totalVarHoursImportedQ1DataPoint.setUnits("varh");
        totalVarHoursImportedQ1DataPoint.setSize(2);
        totalVarHoursImportedQ1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ1DataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseADataPoint;
        totalVarHoursImportedQ1PhaseADataPoint.setName("TotVArhImpQ1phA");
        totalVarHoursImportedQ1PhaseADataPoint.setLabel("Total VAr-hours Imported Q1 phase A");
        totalVarHoursImportedQ1PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseADataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseBDataPoint;
        totalVarHoursImportedQ1PhaseBDataPoint.setName("TotVArhImpQ1phB");
        totalVarHoursImportedQ1PhaseBDataPoint.setLabel("Total VAr-hours Imported Q1 phase B");
        totalVarHoursImportedQ1PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseCDataPoint;
        totalVarHoursImportedQ1PhaseCDataPoint.setName("TotVArhImpQ1phC");
        totalVarHoursImportedQ1PhaseCDataPoint.setLabel("Total VAr-hours Imported Q1 phase C");
        totalVarHoursImportedQ1PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2DataPoint;
        totalVarHoursImportedQ2DataPoint.setName("TotVArhImpQ2");
        totalVarHoursImportedQ2DataPoint.setLabel("Total VAr-hours Imported Q2");
        totalVarHoursImportedQ2DataPoint.setDescription("Total Reactive Power Imported Quadrant 2");
        totalVarHoursImportedQ2DataPoint.setUnits("varh");
        totalVarHoursImportedQ2DataPoint.setSize(2);
        totalVarHoursImportedQ2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ2DataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseADataPoint;
        totalVarHoursImportedQ2PhaseADataPoint.setName("TotVArhImpQ2phA");
        totalVarHoursImportedQ2PhaseADataPoint.setLabel("Total VAr-hours Imported Q2 phase A");
        totalVarHoursImportedQ2PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseADataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseBDataPoint;
        totalVarHoursImportedQ2PhaseBDataPoint.setName("TotVArhImpQ2phB");
        totalVarHoursImportedQ2PhaseBDataPoint.setLabel("Total VAr-hours Imported Q2 phase B");
        totalVarHoursImportedQ2PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseCDataPoint;
        totalVarHoursImportedQ2PhaseCDataPoint.setName("TotVArhImpQ2phC");
        totalVarHoursImportedQ2PhaseCDataPoint.setLabel("Total VAr-hours Imported Q2 phase C");
        totalVarHoursImportedQ2PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3DataPoint;
        totalVarHoursExportedQ3DataPoint.setName("TotVArhExpQ3");
        totalVarHoursExportedQ3DataPoint.setLabel("Total VAr-hours Exported Q3");
        totalVarHoursExportedQ3DataPoint.setDescription("Total Reactive Power Exported Quadrant 3");
        totalVarHoursExportedQ3DataPoint.setUnits("varh");
        totalVarHoursExportedQ3DataPoint.setSize(2);
        totalVarHoursExportedQ3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ3DataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseADataPoint;
        totalVarHoursExportedQ3PhaseADataPoint.setName("TotVArhExpQ3phA");
        totalVarHoursExportedQ3PhaseADataPoint.setLabel("Total VAr-hours Exported Q3 phase A");
        totalVarHoursExportedQ3PhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseADataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseADataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseBDataPoint;
        totalVarHoursExportedQ3PhaseBDataPoint.setName("TotVArhExpQ3phB");
        totalVarHoursExportedQ3PhaseBDataPoint.setLabel("Total VAr-hours Exported Q3 phase B");
        totalVarHoursExportedQ3PhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseCDataPoint;
        totalVarHoursExportedQ3PhaseCDataPoint.setName("TotVArhExpQ3phC");
        totalVarHoursExportedQ3PhaseCDataPoint.setLabel("Total VAr-hours Exported Q3 phase C");
        totalVarHoursExportedQ3PhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4DataPoint;
        totalVarHoursExportedQ4DataPoint.setName("TotVArhExpQ4");
        totalVarHoursExportedQ4DataPoint.setLabel("Total VAr-hours Exported Q4");
        totalVarHoursExportedQ4DataPoint.setDescription("Total Reactive Power Exported Quadrant 4");
        totalVarHoursExportedQ4DataPoint.setUnits("varh");
        totalVarHoursExportedQ4DataPoint.setSize(2);
        totalVarHoursExportedQ4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ4DataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseADataPoint;
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setName("TotVArhExpQ4phA");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase A");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseADataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseBDataPoint;
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setName("TotVArhExpQ4phB");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase B");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseBDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseCDataPoint;
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setName("TotVArhExpQ4phC");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase C");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseCDataPoint;

        SunSpecDataPoint eventsDataPoint;
        eventsDataPoint.setName("Evt");
        eventsDataPoint.setLabel("Events");
        eventsDataPoint.setDescription("Meter Event Flags");
        eventsDataPoint.setMandatory(true);
        eventsDataPoint.setSize(2);
        eventsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << eventsDataPoint;

        break;
    }
    case 213: {
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
        ampsDataPoint.setDescription("Total AC Current");
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

        SunSpecDataPoint voltageLnDataPoint;
        voltageLnDataPoint.setName("PhV");
        voltageLnDataPoint.setLabel("Voltage LN");
        voltageLnDataPoint.setDescription("Line to Neutral AC Voltage (average of active phases)");
        voltageLnDataPoint.setUnits("V");
        voltageLnDataPoint.setMandatory(true);
        voltageLnDataPoint.setSize(2);
        voltageLnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << voltageLnDataPoint;

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

        SunSpecDataPoint voltageLlDataPoint;
        voltageLlDataPoint.setName("PPV");
        voltageLlDataPoint.setLabel("Voltage LL");
        voltageLlDataPoint.setDescription("Line to Line AC Voltage (average of active phases)");
        voltageLlDataPoint.setUnits("V");
        voltageLlDataPoint.setMandatory(true);
        voltageLlDataPoint.setSize(2);
        voltageLlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << voltageLlDataPoint;

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PPVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setMandatory(true);
        phaseVoltageAbDataPoint.setSize(2);
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageAbDataPoint;

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PPVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setMandatory(true);
        phaseVoltageBcDataPoint.setSize(2);
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageBcDataPoint;

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PPVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setMandatory(true);
        phaseVoltageCaDataPoint.setSize(2);
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageCaDataPoint;

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(2);
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << hzDataPoint;

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("Total Real Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(2);
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << wattsDataPoint;

        SunSpecDataPoint wattsPhaseADataPoint;
        wattsPhaseADataPoint.setName("WphA");
        wattsPhaseADataPoint.setLabel("Watts phase A");
        wattsPhaseADataPoint.setUnits("W");
        wattsPhaseADataPoint.setSize(2);
        wattsPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << wattsPhaseADataPoint;

        SunSpecDataPoint wattsPhaseBDataPoint;
        wattsPhaseBDataPoint.setName("WphB");
        wattsPhaseBDataPoint.setLabel("Watts phase B");
        wattsPhaseBDataPoint.setUnits("W");
        wattsPhaseBDataPoint.setSize(2);
        wattsPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << wattsPhaseBDataPoint;

        SunSpecDataPoint wattsPhaseCDataPoint;
        wattsPhaseCDataPoint.setName("WphC");
        wattsPhaseCDataPoint.setLabel("Watts phase C");
        wattsPhaseCDataPoint.setUnits("W");
        wattsPhaseCDataPoint.setSize(2);
        wattsPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << wattsPhaseCDataPoint;

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(2);
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << vaDataPoint;

        SunSpecDataPoint vaPhaseADataPoint;
        vaPhaseADataPoint.setName("VAphA");
        vaPhaseADataPoint.setLabel("VA phase A");
        vaPhaseADataPoint.setUnits("VA");
        vaPhaseADataPoint.setSize(2);
        vaPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << vaPhaseADataPoint;

        SunSpecDataPoint vaPhaseBDataPoint;
        vaPhaseBDataPoint.setName("VAphB");
        vaPhaseBDataPoint.setLabel("VA phase B");
        vaPhaseBDataPoint.setUnits("VA");
        vaPhaseBDataPoint.setSize(2);
        vaPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << vaPhaseBDataPoint;

        SunSpecDataPoint vaPhaseCDataPoint;
        vaPhaseCDataPoint.setName("VAphC");
        vaPhaseCDataPoint.setLabel("VA phase C");
        vaPhaseCDataPoint.setUnits("VA");
        vaPhaseCDataPoint.setSize(2);
        vaPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << vaPhaseCDataPoint;

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAR");
        varDataPoint.setLabel("VAR");
        varDataPoint.setDescription("Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(2);
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << varDataPoint;

        SunSpecDataPoint varPhaseADataPoint;
        varPhaseADataPoint.setName("VARphA");
        varPhaseADataPoint.setLabel("VAR phase A");
        varPhaseADataPoint.setUnits("var");
        varPhaseADataPoint.setSize(2);
        varPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << varPhaseADataPoint;

        SunSpecDataPoint varPhaseBDataPoint;
        varPhaseBDataPoint.setName("VARphB");
        varPhaseBDataPoint.setLabel("VAR phase B");
        varPhaseBDataPoint.setUnits("var");
        varPhaseBDataPoint.setSize(2);
        varPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << varPhaseBDataPoint;

        SunSpecDataPoint varPhaseCDataPoint;
        varPhaseCDataPoint.setName("VARphC");
        varPhaseCDataPoint.setLabel("VAR phase C");
        varPhaseCDataPoint.setUnits("var");
        varPhaseCDataPoint.setSize(2);
        varPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << varPhaseCDataPoint;

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("Power Factor");
        pfDataPoint.setUnits("PF");
        pfDataPoint.setSize(2);
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << pfDataPoint;

        SunSpecDataPoint pfPhaseADataPoint;
        pfPhaseADataPoint.setName("PFphA");
        pfPhaseADataPoint.setLabel("PF phase A");
        pfPhaseADataPoint.setUnits("PF");
        pfPhaseADataPoint.setSize(2);
        pfPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << pfPhaseADataPoint;

        SunSpecDataPoint pfPhaseBDataPoint;
        pfPhaseBDataPoint.setName("PFphB");
        pfPhaseBDataPoint.setLabel("PF phase B");
        pfPhaseBDataPoint.setUnits("PF");
        pfPhaseBDataPoint.setSize(2);
        pfPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << pfPhaseBDataPoint;

        SunSpecDataPoint pfPhaseCDataPoint;
        pfPhaseCDataPoint.setName("PFphC");
        pfPhaseCDataPoint.setLabel("PF phase C");
        pfPhaseCDataPoint.setUnits("PF");
        pfPhaseCDataPoint.setSize(2);
        pfPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << pfPhaseCDataPoint;

        SunSpecDataPoint totalWattHoursExportedDataPoint;
        totalWattHoursExportedDataPoint.setName("TotWhExp");
        totalWattHoursExportedDataPoint.setLabel("Total Watt-hours Exported");
        totalWattHoursExportedDataPoint.setDescription("Total Real Energy Exported");
        totalWattHoursExportedDataPoint.setUnits("Wh");
        totalWattHoursExportedDataPoint.setMandatory(true);
        totalWattHoursExportedDataPoint.setSize(2);
        totalWattHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursExportedDataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseADataPoint;
        totalWattHoursExportedPhaseADataPoint.setName("TotWhExpPhA");
        totalWattHoursExportedPhaseADataPoint.setLabel("Total Watt-hours Exported phase A");
        totalWattHoursExportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseADataPoint.setSize(2);
        totalWattHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursExportedPhaseADataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseBDataPoint;
        totalWattHoursExportedPhaseBDataPoint.setName("TotWhExpPhB");
        totalWattHoursExportedPhaseBDataPoint.setLabel("Total Watt-hours Exported phase B");
        totalWattHoursExportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseBDataPoint.setSize(2);
        totalWattHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursExportedPhaseBDataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseCDataPoint;
        totalWattHoursExportedPhaseCDataPoint.setName("TotWhExpPhC");
        totalWattHoursExportedPhaseCDataPoint.setLabel("Total Watt-hours Exported phase C");
        totalWattHoursExportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseCDataPoint.setSize(2);
        totalWattHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursExportedPhaseCDataPoint;

        SunSpecDataPoint totalWattHoursImportedDataPoint;
        totalWattHoursImportedDataPoint.setName("TotWhImp");
        totalWattHoursImportedDataPoint.setLabel("Total Watt-hours Imported");
        totalWattHoursImportedDataPoint.setDescription("Total Real Energy Imported");
        totalWattHoursImportedDataPoint.setUnits("Wh");
        totalWattHoursImportedDataPoint.setMandatory(true);
        totalWattHoursImportedDataPoint.setSize(2);
        totalWattHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursImportedDataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseADataPoint;
        totalWattHoursImportedPhaseADataPoint.setName("TotWhImpPhA");
        totalWattHoursImportedPhaseADataPoint.setLabel("Total Watt-hours Imported phase A");
        totalWattHoursImportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseADataPoint.setSize(2);
        totalWattHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursImportedPhaseADataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseBDataPoint;
        totalWattHoursImportedPhaseBDataPoint.setName("TotWhImpPhB");
        totalWattHoursImportedPhaseBDataPoint.setLabel("Total Watt-hours Imported phase B");
        totalWattHoursImportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseBDataPoint.setSize(2);
        totalWattHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursImportedPhaseBDataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseCDataPoint;
        totalWattHoursImportedPhaseCDataPoint.setName("TotWhImpPhC");
        totalWattHoursImportedPhaseCDataPoint.setLabel("Total Watt-hours Imported phase C");
        totalWattHoursImportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseCDataPoint.setSize(2);
        totalWattHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursImportedPhaseCDataPoint;

        SunSpecDataPoint totalVaHoursExportedDataPoint;
        totalVaHoursExportedDataPoint.setName("TotVAhExp");
        totalVaHoursExportedDataPoint.setLabel("Total VA-hours Exported");
        totalVaHoursExportedDataPoint.setDescription("Total Apparent Energy Exported");
        totalVaHoursExportedDataPoint.setUnits("VAh");
        totalVaHoursExportedDataPoint.setSize(2);
        totalVaHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursExportedDataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseADataPoint;
        totalVaHoursExportedPhaseADataPoint.setName("TotVAhExpPhA");
        totalVaHoursExportedPhaseADataPoint.setLabel("Total VA-hours Exported phase A");
        totalVaHoursExportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseADataPoint.setSize(2);
        totalVaHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursExportedPhaseADataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseBDataPoint;
        totalVaHoursExportedPhaseBDataPoint.setName("TotVAhExpPhB");
        totalVaHoursExportedPhaseBDataPoint.setLabel("Total VA-hours Exported phase B");
        totalVaHoursExportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseBDataPoint.setSize(2);
        totalVaHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursExportedPhaseBDataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseCDataPoint;
        totalVaHoursExportedPhaseCDataPoint.setName("TotVAhExpPhC");
        totalVaHoursExportedPhaseCDataPoint.setLabel("Total VA-hours Exported phase C");
        totalVaHoursExportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseCDataPoint.setSize(2);
        totalVaHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursExportedPhaseCDataPoint;

        SunSpecDataPoint totalVaHoursImportedDataPoint;
        totalVaHoursImportedDataPoint.setName("TotVAhImp");
        totalVaHoursImportedDataPoint.setLabel("Total VA-hours Imported");
        totalVaHoursImportedDataPoint.setDescription("Total Apparent Energy Imported");
        totalVaHoursImportedDataPoint.setUnits("VAh");
        totalVaHoursImportedDataPoint.setSize(2);
        totalVaHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursImportedDataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseADataPoint;
        totalVaHoursImportedPhaseADataPoint.setName("TotVAhImpPhA");
        totalVaHoursImportedPhaseADataPoint.setLabel("Total VA-hours Imported phase A");
        totalVaHoursImportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseADataPoint.setSize(2);
        totalVaHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursImportedPhaseADataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseBDataPoint;
        totalVaHoursImportedPhaseBDataPoint.setName("TotVAhImpPhB");
        totalVaHoursImportedPhaseBDataPoint.setLabel("Total VA-hours Imported phase B");
        totalVaHoursImportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseBDataPoint.setSize(2);
        totalVaHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursImportedPhaseBDataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseCDataPoint;
        totalVaHoursImportedPhaseCDataPoint.setName("TotVAhImpPhC");
        totalVaHoursImportedPhaseCDataPoint.setLabel("Total VA-hours Imported phase C");
        totalVaHoursImportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseCDataPoint.setSize(2);
        totalVaHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursImportedPhaseCDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1DataPoint;
        totalVarHoursImportedQ1DataPoint.setName("TotVArhImpQ1");
        totalVarHoursImportedQ1DataPoint.setLabel("Total VAR-hours Imported Q1");
        totalVarHoursImportedQ1DataPoint.setDescription("Total Reactive Energy Imported Quadrant 1");
        totalVarHoursImportedQ1DataPoint.setUnits("varh");
        totalVarHoursImportedQ1DataPoint.setSize(2);
        totalVarHoursImportedQ1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ1DataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseADataPoint;
        totalVarHoursImportedQ1PhaseADataPoint.setName("TotVArhImpQ1phA");
        totalVarHoursImportedQ1PhaseADataPoint.setLabel("Total VAr-hours Imported Q1 phase A");
        totalVarHoursImportedQ1PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseADataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseBDataPoint;
        totalVarHoursImportedQ1PhaseBDataPoint.setName("TotVArhImpQ1phB");
        totalVarHoursImportedQ1PhaseBDataPoint.setLabel("Total VAr-hours Imported Q1 phase B");
        totalVarHoursImportedQ1PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseCDataPoint;
        totalVarHoursImportedQ1PhaseCDataPoint.setName("TotVArhImpQ1phC");
        totalVarHoursImportedQ1PhaseCDataPoint.setLabel("Total VAr-hours Imported Q1 phase C");
        totalVarHoursImportedQ1PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2DataPoint;
        totalVarHoursImportedQ2DataPoint.setName("TotVArhImpQ2");
        totalVarHoursImportedQ2DataPoint.setLabel("Total VAr-hours Imported Q2");
        totalVarHoursImportedQ2DataPoint.setDescription("Total Reactive Power Imported Quadrant 2");
        totalVarHoursImportedQ2DataPoint.setUnits("varh");
        totalVarHoursImportedQ2DataPoint.setSize(2);
        totalVarHoursImportedQ2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ2DataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseADataPoint;
        totalVarHoursImportedQ2PhaseADataPoint.setName("TotVArhImpQ2phA");
        totalVarHoursImportedQ2PhaseADataPoint.setLabel("Total VAr-hours Imported Q2 phase A");
        totalVarHoursImportedQ2PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseADataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseBDataPoint;
        totalVarHoursImportedQ2PhaseBDataPoint.setName("TotVArhImpQ2phB");
        totalVarHoursImportedQ2PhaseBDataPoint.setLabel("Total VAr-hours Imported Q2 phase B");
        totalVarHoursImportedQ2PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseCDataPoint;
        totalVarHoursImportedQ2PhaseCDataPoint.setName("TotVArhImpQ2phC");
        totalVarHoursImportedQ2PhaseCDataPoint.setLabel("Total VAr-hours Imported Q2 phase C");
        totalVarHoursImportedQ2PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3DataPoint;
        totalVarHoursExportedQ3DataPoint.setName("TotVArhExpQ3");
        totalVarHoursExportedQ3DataPoint.setLabel("Total VAr-hours Exported Q3");
        totalVarHoursExportedQ3DataPoint.setDescription("Total Reactive Power Exported Quadrant 3");
        totalVarHoursExportedQ3DataPoint.setUnits("varh");
        totalVarHoursExportedQ3DataPoint.setSize(2);
        totalVarHoursExportedQ3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ3DataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseADataPoint;
        totalVarHoursExportedQ3PhaseADataPoint.setName("TotVArhExpQ3phA");
        totalVarHoursExportedQ3PhaseADataPoint.setLabel("Total VAr-hours Exported Q3 phase A");
        totalVarHoursExportedQ3PhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseADataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseADataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseBDataPoint;
        totalVarHoursExportedQ3PhaseBDataPoint.setName("TotVArhExpQ3phB");
        totalVarHoursExportedQ3PhaseBDataPoint.setLabel("Total VAr-hours Exported Q3 phase B");
        totalVarHoursExportedQ3PhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseCDataPoint;
        totalVarHoursExportedQ3PhaseCDataPoint.setName("TotVArhExpQ3phC");
        totalVarHoursExportedQ3PhaseCDataPoint.setLabel("Total VAr-hours Exported Q3 phase C");
        totalVarHoursExportedQ3PhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4DataPoint;
        totalVarHoursExportedQ4DataPoint.setName("TotVArhExpQ4");
        totalVarHoursExportedQ4DataPoint.setLabel("Total VAr-hours Exported Q4");
        totalVarHoursExportedQ4DataPoint.setDescription("Total Reactive Power Exported Quadrant 4");
        totalVarHoursExportedQ4DataPoint.setUnits("varh");
        totalVarHoursExportedQ4DataPoint.setSize(2);
        totalVarHoursExportedQ4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ4DataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseADataPoint;
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setName("TotVArhExpQ4phA");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase A");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseADataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseBDataPoint;
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setName("TotVArhExpQ4phB");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase B");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseBDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseCDataPoint;
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setName("TotVArhExpQ4phC");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase C");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseCDataPoint;

        SunSpecDataPoint eventsDataPoint;
        eventsDataPoint.setName("Evt");
        eventsDataPoint.setLabel("Events");
        eventsDataPoint.setDescription("Meter Event Flags");
        eventsDataPoint.setMandatory(true);
        eventsDataPoint.setSize(2);
        eventsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << eventsDataPoint;

        break;
    }
    case 214: {
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
        ampsDataPoint.setDescription("Total AC Current");
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

        SunSpecDataPoint voltageLnDataPoint;
        voltageLnDataPoint.setName("PhV");
        voltageLnDataPoint.setLabel("Voltage LN");
        voltageLnDataPoint.setDescription("Line to Neutral AC Voltage (average of active phases)");
        voltageLnDataPoint.setUnits("V");
        voltageLnDataPoint.setSize(2);
        voltageLnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << voltageLnDataPoint;

        SunSpecDataPoint phaseVoltageAnDataPoint;
        phaseVoltageAnDataPoint.setName("PhVphA");
        phaseVoltageAnDataPoint.setLabel("Phase Voltage AN");
        phaseVoltageAnDataPoint.setDescription("Phase Voltage AN");
        phaseVoltageAnDataPoint.setUnits("V");
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

        SunSpecDataPoint voltageLlDataPoint;
        voltageLlDataPoint.setName("PPV");
        voltageLlDataPoint.setLabel("Voltage LL");
        voltageLlDataPoint.setDescription("Line to Line AC Voltage (average of active phases)");
        voltageLlDataPoint.setUnits("V");
        voltageLlDataPoint.setMandatory(true);
        voltageLlDataPoint.setSize(2);
        voltageLlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << voltageLlDataPoint;

        SunSpecDataPoint phaseVoltageAbDataPoint;
        phaseVoltageAbDataPoint.setName("PPVphAB");
        phaseVoltageAbDataPoint.setLabel("Phase Voltage AB");
        phaseVoltageAbDataPoint.setDescription("Phase Voltage AB");
        phaseVoltageAbDataPoint.setUnits("V");
        phaseVoltageAbDataPoint.setMandatory(true);
        phaseVoltageAbDataPoint.setSize(2);
        phaseVoltageAbDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageAbDataPoint;

        SunSpecDataPoint phaseVoltageBcDataPoint;
        phaseVoltageBcDataPoint.setName("PPVphBC");
        phaseVoltageBcDataPoint.setLabel("Phase Voltage BC");
        phaseVoltageBcDataPoint.setDescription("Phase Voltage BC");
        phaseVoltageBcDataPoint.setUnits("V");
        phaseVoltageBcDataPoint.setMandatory(true);
        phaseVoltageBcDataPoint.setSize(2);
        phaseVoltageBcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageBcDataPoint;

        SunSpecDataPoint phaseVoltageCaDataPoint;
        phaseVoltageCaDataPoint.setName("PPVphCA");
        phaseVoltageCaDataPoint.setLabel("Phase Voltage CA");
        phaseVoltageCaDataPoint.setDescription("Phase Voltage CA");
        phaseVoltageCaDataPoint.setUnits("V");
        phaseVoltageCaDataPoint.setMandatory(true);
        phaseVoltageCaDataPoint.setSize(2);
        phaseVoltageCaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << phaseVoltageCaDataPoint;

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(2);
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << hzDataPoint;

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("Total Real Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(2);
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << wattsDataPoint;

        SunSpecDataPoint wattsPhaseADataPoint;
        wattsPhaseADataPoint.setName("WphA");
        wattsPhaseADataPoint.setLabel("Watts phase A");
        wattsPhaseADataPoint.setUnits("W");
        wattsPhaseADataPoint.setSize(2);
        wattsPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << wattsPhaseADataPoint;

        SunSpecDataPoint wattsPhaseBDataPoint;
        wattsPhaseBDataPoint.setName("WphB");
        wattsPhaseBDataPoint.setLabel("Watts phase B");
        wattsPhaseBDataPoint.setUnits("W");
        wattsPhaseBDataPoint.setSize(2);
        wattsPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << wattsPhaseBDataPoint;

        SunSpecDataPoint wattsPhaseCDataPoint;
        wattsPhaseCDataPoint.setName("WphC");
        wattsPhaseCDataPoint.setLabel("Watts phase C");
        wattsPhaseCDataPoint.setUnits("W");
        wattsPhaseCDataPoint.setSize(2);
        wattsPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << wattsPhaseCDataPoint;

        SunSpecDataPoint vaDataPoint;
        vaDataPoint.setName("VA");
        vaDataPoint.setLabel("VA");
        vaDataPoint.setDescription("AC Apparent Power");
        vaDataPoint.setUnits("VA");
        vaDataPoint.setSize(2);
        vaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << vaDataPoint;

        SunSpecDataPoint vaPhaseADataPoint;
        vaPhaseADataPoint.setName("VAphA");
        vaPhaseADataPoint.setLabel("VA phase A");
        vaPhaseADataPoint.setUnits("VA");
        vaPhaseADataPoint.setSize(2);
        vaPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << vaPhaseADataPoint;

        SunSpecDataPoint vaPhaseBDataPoint;
        vaPhaseBDataPoint.setName("VAphB");
        vaPhaseBDataPoint.setLabel("VA phase B");
        vaPhaseBDataPoint.setUnits("VA");
        vaPhaseBDataPoint.setSize(2);
        vaPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << vaPhaseBDataPoint;

        SunSpecDataPoint vaPhaseCDataPoint;
        vaPhaseCDataPoint.setName("VAphC");
        vaPhaseCDataPoint.setLabel("VA phase C");
        vaPhaseCDataPoint.setUnits("VA");
        vaPhaseCDataPoint.setSize(2);
        vaPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << vaPhaseCDataPoint;

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAR");
        varDataPoint.setLabel("VAR");
        varDataPoint.setDescription("Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(2);
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << varDataPoint;

        SunSpecDataPoint varPhaseADataPoint;
        varPhaseADataPoint.setName("VARphA");
        varPhaseADataPoint.setLabel("VAR phase A");
        varPhaseADataPoint.setUnits("var");
        varPhaseADataPoint.setSize(2);
        varPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << varPhaseADataPoint;

        SunSpecDataPoint varPhaseBDataPoint;
        varPhaseBDataPoint.setName("VARphB");
        varPhaseBDataPoint.setLabel("VAR phase B");
        varPhaseBDataPoint.setUnits("var");
        varPhaseBDataPoint.setSize(2);
        varPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << varPhaseBDataPoint;

        SunSpecDataPoint varPhaseCDataPoint;
        varPhaseCDataPoint.setName("VARphC");
        varPhaseCDataPoint.setLabel("VAR phase C");
        varPhaseCDataPoint.setUnits("var");
        varPhaseCDataPoint.setSize(2);
        varPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << varPhaseCDataPoint;

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("Power Factor");
        pfDataPoint.setUnits("PF");
        pfDataPoint.setSize(2);
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << pfDataPoint;

        SunSpecDataPoint pfPhaseADataPoint;
        pfPhaseADataPoint.setName("PFphA");
        pfPhaseADataPoint.setLabel("PF phase A");
        pfPhaseADataPoint.setUnits("PF");
        pfPhaseADataPoint.setSize(2);
        pfPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << pfPhaseADataPoint;

        SunSpecDataPoint pfPhaseBDataPoint;
        pfPhaseBDataPoint.setName("PFphB");
        pfPhaseBDataPoint.setLabel("PF phase B");
        pfPhaseBDataPoint.setUnits("PF");
        pfPhaseBDataPoint.setSize(2);
        pfPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << pfPhaseBDataPoint;

        SunSpecDataPoint pfPhaseCDataPoint;
        pfPhaseCDataPoint.setName("PFphC");
        pfPhaseCDataPoint.setLabel("PF phase C");
        pfPhaseCDataPoint.setUnits("PF");
        pfPhaseCDataPoint.setSize(2);
        pfPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << pfPhaseCDataPoint;

        SunSpecDataPoint totalWattHoursExportedDataPoint;
        totalWattHoursExportedDataPoint.setName("TotWhExp");
        totalWattHoursExportedDataPoint.setLabel("Total Watt-hours Exported");
        totalWattHoursExportedDataPoint.setDescription("Total Real Energy Exported");
        totalWattHoursExportedDataPoint.setUnits("Wh");
        totalWattHoursExportedDataPoint.setMandatory(true);
        totalWattHoursExportedDataPoint.setSize(2);
        totalWattHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursExportedDataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseADataPoint;
        totalWattHoursExportedPhaseADataPoint.setName("TotWhExpPhA");
        totalWattHoursExportedPhaseADataPoint.setLabel("Total Watt-hours Exported phase A");
        totalWattHoursExportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseADataPoint.setSize(2);
        totalWattHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursExportedPhaseADataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseBDataPoint;
        totalWattHoursExportedPhaseBDataPoint.setName("TotWhExpPhB");
        totalWattHoursExportedPhaseBDataPoint.setLabel("Total Watt-hours Exported phase B");
        totalWattHoursExportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseBDataPoint.setSize(2);
        totalWattHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursExportedPhaseBDataPoint;

        SunSpecDataPoint totalWattHoursExportedPhaseCDataPoint;
        totalWattHoursExportedPhaseCDataPoint.setName("TotWhExpPhC");
        totalWattHoursExportedPhaseCDataPoint.setLabel("Total Watt-hours Exported phase C");
        totalWattHoursExportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursExportedPhaseCDataPoint.setSize(2);
        totalWattHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursExportedPhaseCDataPoint;

        SunSpecDataPoint totalWattHoursImportedDataPoint;
        totalWattHoursImportedDataPoint.setName("TotWhImp");
        totalWattHoursImportedDataPoint.setLabel("Total Watt-hours Imported");
        totalWattHoursImportedDataPoint.setDescription("Total Real Energy Imported");
        totalWattHoursImportedDataPoint.setUnits("Wh");
        totalWattHoursImportedDataPoint.setMandatory(true);
        totalWattHoursImportedDataPoint.setSize(2);
        totalWattHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursImportedDataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseADataPoint;
        totalWattHoursImportedPhaseADataPoint.setName("TotWhImpPhA");
        totalWattHoursImportedPhaseADataPoint.setLabel("Total Watt-hours Imported phase A");
        totalWattHoursImportedPhaseADataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseADataPoint.setSize(2);
        totalWattHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursImportedPhaseADataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseBDataPoint;
        totalWattHoursImportedPhaseBDataPoint.setName("TotWhImpPhB");
        totalWattHoursImportedPhaseBDataPoint.setLabel("Total Watt-hours Imported phase B");
        totalWattHoursImportedPhaseBDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseBDataPoint.setSize(2);
        totalWattHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursImportedPhaseBDataPoint;

        SunSpecDataPoint totalWattHoursImportedPhaseCDataPoint;
        totalWattHoursImportedPhaseCDataPoint.setName("TotWhImpPhC");
        totalWattHoursImportedPhaseCDataPoint.setLabel("Total Watt-hours Imported phase C");
        totalWattHoursImportedPhaseCDataPoint.setUnits("Wh");
        totalWattHoursImportedPhaseCDataPoint.setSize(2);
        totalWattHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalWattHoursImportedPhaseCDataPoint;

        SunSpecDataPoint totalVaHoursExportedDataPoint;
        totalVaHoursExportedDataPoint.setName("TotVAhExp");
        totalVaHoursExportedDataPoint.setLabel("Total VA-hours Exported");
        totalVaHoursExportedDataPoint.setDescription("Total Apparent Energy Exported");
        totalVaHoursExportedDataPoint.setUnits("VAh");
        totalVaHoursExportedDataPoint.setSize(2);
        totalVaHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursExportedDataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseADataPoint;
        totalVaHoursExportedPhaseADataPoint.setName("TotVAhExpPhA");
        totalVaHoursExportedPhaseADataPoint.setLabel("Total VA-hours Exported phase A");
        totalVaHoursExportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseADataPoint.setSize(2);
        totalVaHoursExportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursExportedPhaseADataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseBDataPoint;
        totalVaHoursExportedPhaseBDataPoint.setName("TotVAhExpPhB");
        totalVaHoursExportedPhaseBDataPoint.setLabel("Total VA-hours Exported phase B");
        totalVaHoursExportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseBDataPoint.setSize(2);
        totalVaHoursExportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursExportedPhaseBDataPoint;

        SunSpecDataPoint totalVaHoursExportedPhaseCDataPoint;
        totalVaHoursExportedPhaseCDataPoint.setName("TotVAhExpPhC");
        totalVaHoursExportedPhaseCDataPoint.setLabel("Total VA-hours Exported phase C");
        totalVaHoursExportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursExportedPhaseCDataPoint.setSize(2);
        totalVaHoursExportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursExportedPhaseCDataPoint;

        SunSpecDataPoint totalVaHoursImportedDataPoint;
        totalVaHoursImportedDataPoint.setName("TotVAhImp");
        totalVaHoursImportedDataPoint.setLabel("Total VA-hours Imported");
        totalVaHoursImportedDataPoint.setDescription("Total Apparent Energy Imported");
        totalVaHoursImportedDataPoint.setUnits("VAh");
        totalVaHoursImportedDataPoint.setSize(2);
        totalVaHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursImportedDataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseADataPoint;
        totalVaHoursImportedPhaseADataPoint.setName("TotVAhImpPhA");
        totalVaHoursImportedPhaseADataPoint.setLabel("Total VA-hours Imported phase A");
        totalVaHoursImportedPhaseADataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseADataPoint.setSize(2);
        totalVaHoursImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursImportedPhaseADataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseBDataPoint;
        totalVaHoursImportedPhaseBDataPoint.setName("TotVAhImpPhB");
        totalVaHoursImportedPhaseBDataPoint.setLabel("Total VA-hours Imported phase B");
        totalVaHoursImportedPhaseBDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseBDataPoint.setSize(2);
        totalVaHoursImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursImportedPhaseBDataPoint;

        SunSpecDataPoint totalVaHoursImportedPhaseCDataPoint;
        totalVaHoursImportedPhaseCDataPoint.setName("TotVAhImpPhC");
        totalVaHoursImportedPhaseCDataPoint.setLabel("Total VA-hours Imported phase C");
        totalVaHoursImportedPhaseCDataPoint.setUnits("VAh");
        totalVaHoursImportedPhaseCDataPoint.setSize(2);
        totalVaHoursImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVaHoursImportedPhaseCDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1DataPoint;
        totalVarHoursImportedQ1DataPoint.setName("TotVArhImpQ1");
        totalVarHoursImportedQ1DataPoint.setLabel("Total VAR-hours Imported Q1");
        totalVarHoursImportedQ1DataPoint.setDescription("Total Reactive Energy Imported Quadrant 1");
        totalVarHoursImportedQ1DataPoint.setUnits("varh");
        totalVarHoursImportedQ1DataPoint.setSize(2);
        totalVarHoursImportedQ1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ1DataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseADataPoint;
        totalVarHoursImportedQ1PhaseADataPoint.setName("TotVArhImpQ1phA");
        totalVarHoursImportedQ1PhaseADataPoint.setLabel("Total VAr-hours Imported Q1 phase A");
        totalVarHoursImportedQ1PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseADataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseBDataPoint;
        totalVarHoursImportedQ1PhaseBDataPoint.setName("TotVArhImpQ1phB");
        totalVarHoursImportedQ1PhaseBDataPoint.setLabel("Total VAr-hours Imported Q1 phase B");
        totalVarHoursImportedQ1PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1PhaseCDataPoint;
        totalVarHoursImportedQ1PhaseCDataPoint.setName("TotVArhImpQ1phC");
        totalVarHoursImportedQ1PhaseCDataPoint.setLabel("Total VAr-hours Imported Q1 phase C");
        totalVarHoursImportedQ1PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ1PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ1PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ1PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2DataPoint;
        totalVarHoursImportedQ2DataPoint.setName("TotVArhImpQ2");
        totalVarHoursImportedQ2DataPoint.setLabel("Total VAr-hours Imported Q2");
        totalVarHoursImportedQ2DataPoint.setDescription("Total Reactive Power Imported Quadrant 2");
        totalVarHoursImportedQ2DataPoint.setUnits("varh");
        totalVarHoursImportedQ2DataPoint.setSize(2);
        totalVarHoursImportedQ2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ2DataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseADataPoint;
        totalVarHoursImportedQ2PhaseADataPoint.setName("TotVArhImpQ2phA");
        totalVarHoursImportedQ2PhaseADataPoint.setLabel("Total VAr-hours Imported Q2 phase A");
        totalVarHoursImportedQ2PhaseADataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseADataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseADataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseBDataPoint;
        totalVarHoursImportedQ2PhaseBDataPoint.setName("TotVArhImpQ2phB");
        totalVarHoursImportedQ2PhaseBDataPoint.setLabel("Total VAr-hours Imported Q2 phase B");
        totalVarHoursImportedQ2PhaseBDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseBDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2PhaseCDataPoint;
        totalVarHoursImportedQ2PhaseCDataPoint.setName("TotVArhImpQ2phC");
        totalVarHoursImportedQ2PhaseCDataPoint.setLabel("Total VAr-hours Imported Q2 phase C");
        totalVarHoursImportedQ2PhaseCDataPoint.setUnits("varh");
        totalVarHoursImportedQ2PhaseCDataPoint.setSize(2);
        totalVarHoursImportedQ2PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursImportedQ2PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3DataPoint;
        totalVarHoursExportedQ3DataPoint.setName("TotVArhExpQ3");
        totalVarHoursExportedQ3DataPoint.setLabel("Total VAr-hours Exported Q3");
        totalVarHoursExportedQ3DataPoint.setDescription("Total Reactive Power Exported Quadrant 3");
        totalVarHoursExportedQ3DataPoint.setUnits("varh");
        totalVarHoursExportedQ3DataPoint.setSize(2);
        totalVarHoursExportedQ3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ3DataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseADataPoint;
        totalVarHoursExportedQ3PhaseADataPoint.setName("TotVArhExpQ3phA");
        totalVarHoursExportedQ3PhaseADataPoint.setLabel("Total VAr-hours Exported Q3 phase A");
        totalVarHoursExportedQ3PhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseADataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseADataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseBDataPoint;
        totalVarHoursExportedQ3PhaseBDataPoint.setName("TotVArhExpQ3phB");
        totalVarHoursExportedQ3PhaseBDataPoint.setLabel("Total VAr-hours Exported Q3 phase B");
        totalVarHoursExportedQ3PhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseBDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3PhaseCDataPoint;
        totalVarHoursExportedQ3PhaseCDataPoint.setName("TotVArhExpQ3phC");
        totalVarHoursExportedQ3PhaseCDataPoint.setLabel("Total VAr-hours Exported Q3 phase C");
        totalVarHoursExportedQ3PhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ3PhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ3PhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ3PhaseCDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4DataPoint;
        totalVarHoursExportedQ4DataPoint.setName("TotVArhExpQ4");
        totalVarHoursExportedQ4DataPoint.setLabel("Total VAr-hours Exported Q4");
        totalVarHoursExportedQ4DataPoint.setDescription("Total Reactive Power Exported Quadrant 4");
        totalVarHoursExportedQ4DataPoint.setUnits("varh");
        totalVarHoursExportedQ4DataPoint.setSize(2);
        totalVarHoursExportedQ4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ4DataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseADataPoint;
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setName("TotVArhExpQ4phA");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase A");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseADataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseADataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseBDataPoint;
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setName("TotVArhExpQ4phB");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase B");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseBDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseBDataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4ImportedPhaseCDataPoint;
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setName("TotVArhExpQ4phC");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setLabel("Total VAr-hours Exported Q4 Imported phase C");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setUnits("varh");
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setSize(2);
        totalVarHoursExportedQ4ImportedPhaseCDataPoint.setDataType(SunSpecDataPoint::stringToDataType("float32"));
        m_dataPoints << totalVarHoursExportedQ4ImportedPhaseCDataPoint;

        SunSpecDataPoint eventsDataPoint;
        eventsDataPoint.setName("Evt");
        eventsDataPoint.setLabel("Events");
        eventsDataPoint.setDescription("Meter Event Flags");
        eventsDataPoint.setMandatory(true);
        eventsDataPoint.setSize(2);
        eventsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << eventsDataPoint;

        break;
    }
    case 220: {
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
        ampsDataPoint.setDescription("Total AC Current");
        ampsDataPoint.setUnits("A");
        ampsDataPoint.setMandatory(true);
        ampsDataPoint.setSize(1);
        ampsDataPoint.setScaleFactorName("A_SF");
        ampsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << ampsDataPoint;

        SunSpecDataPoint A_SFDataPoint;
        A_SFDataPoint.setName("A_SF");
        A_SFDataPoint.setDescription("Current scale factor");
        A_SFDataPoint.setMandatory(true);
        A_SFDataPoint.setSize(1);
        A_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << A_SFDataPoint;

        SunSpecDataPoint voltageDataPoint;
        voltageDataPoint.setName("PhV");
        voltageDataPoint.setLabel("Voltage");
        voltageDataPoint.setDescription("Average phase or line voltage");
        voltageDataPoint.setUnits("V");
        voltageDataPoint.setSize(1);
        voltageDataPoint.setScaleFactorName("V_SF");
        voltageDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << voltageDataPoint;

        SunSpecDataPoint V_SFDataPoint;
        V_SFDataPoint.setName("V_SF");
        V_SFDataPoint.setDescription("Voltage scale factor");
        V_SFDataPoint.setMandatory(true);
        V_SFDataPoint.setSize(1);
        V_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << V_SFDataPoint;

        SunSpecDataPoint hzDataPoint;
        hzDataPoint.setName("Hz");
        hzDataPoint.setLabel("Hz");
        hzDataPoint.setDescription("Frequency");
        hzDataPoint.setUnits("Hz");
        hzDataPoint.setMandatory(true);
        hzDataPoint.setSize(1);
        hzDataPoint.setScaleFactorName("Hz_SF");
        hzDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << hzDataPoint;

        SunSpecDataPoint Hz_SFDataPoint;
        Hz_SFDataPoint.setName("Hz_SF");
        Hz_SFDataPoint.setDescription("Frequency scale factor");
        Hz_SFDataPoint.setSize(1);
        Hz_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << Hz_SFDataPoint;

        SunSpecDataPoint wattsDataPoint;
        wattsDataPoint.setName("W");
        wattsDataPoint.setLabel("Watts");
        wattsDataPoint.setDescription("Total Real Power");
        wattsDataPoint.setUnits("W");
        wattsDataPoint.setMandatory(true);
        wattsDataPoint.setSize(1);
        wattsDataPoint.setScaleFactorName("W_SF");
        wattsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << wattsDataPoint;

        SunSpecDataPoint W_SFDataPoint;
        W_SFDataPoint.setName("W_SF");
        W_SFDataPoint.setDescription("Real Power scale factor");
        W_SFDataPoint.setMandatory(true);
        W_SFDataPoint.setSize(1);
        W_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << W_SFDataPoint;

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
        VA_SFDataPoint.setDescription("Apparent Power scale factor");
        VA_SFDataPoint.setSize(1);
        VA_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << VA_SFDataPoint;

        SunSpecDataPoint varDataPoint;
        varDataPoint.setName("VAR");
        varDataPoint.setLabel("VAR");
        varDataPoint.setDescription("Reactive Power");
        varDataPoint.setUnits("var");
        varDataPoint.setSize(1);
        varDataPoint.setScaleFactorName("VAR_SF");
        varDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << varDataPoint;

        SunSpecDataPoint VAR_SFDataPoint;
        VAR_SFDataPoint.setName("VAR_SF");
        VAR_SFDataPoint.setDescription("Reactive Power scale factor");
        VAR_SFDataPoint.setSize(1);
        VAR_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << VAR_SFDataPoint;

        SunSpecDataPoint pfDataPoint;
        pfDataPoint.setName("PF");
        pfDataPoint.setLabel("PF");
        pfDataPoint.setDescription("Power Factor");
        pfDataPoint.setUnits("Pct");
        pfDataPoint.setSize(1);
        pfDataPoint.setScaleFactorName("PF_SF");
        pfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
        m_dataPoints << pfDataPoint;

        SunSpecDataPoint PF_SFDataPoint;
        PF_SFDataPoint.setName("PF_SF");
        PF_SFDataPoint.setDescription("Power Factor scale factor");
        PF_SFDataPoint.setSize(1);
        PF_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << PF_SFDataPoint;

        SunSpecDataPoint totalWattHoursExportedDataPoint;
        totalWattHoursExportedDataPoint.setName("TotWhExp");
        totalWattHoursExportedDataPoint.setLabel("Total Watt-hours Exported");
        totalWattHoursExportedDataPoint.setDescription("Total Real Energy Exported");
        totalWattHoursExportedDataPoint.setUnits("Wh");
        totalWattHoursExportedDataPoint.setMandatory(true);
        totalWattHoursExportedDataPoint.setSize(2);
        totalWattHoursExportedDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursExportedDataPoint;

        SunSpecDataPoint totalWattHoursImportedDataPoint;
        totalWattHoursImportedDataPoint.setName("TotWhImp");
        totalWattHoursImportedDataPoint.setLabel("Total Watt-hours Imported");
        totalWattHoursImportedDataPoint.setDescription("Total Real Energy Imported");
        totalWattHoursImportedDataPoint.setUnits("Wh");
        totalWattHoursImportedDataPoint.setMandatory(true);
        totalWattHoursImportedDataPoint.setSize(2);
        totalWattHoursImportedDataPoint.setScaleFactorName("TotWh_SF");
        totalWattHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalWattHoursImportedDataPoint;

        SunSpecDataPoint TotWh_SFDataPoint;
        TotWh_SFDataPoint.setName("TotWh_SF");
        TotWh_SFDataPoint.setDescription("Real Energy scale factor");
        TotWh_SFDataPoint.setMandatory(true);
        TotWh_SFDataPoint.setSize(1);
        TotWh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << TotWh_SFDataPoint;

        SunSpecDataPoint totalVaHoursExportedDataPoint;
        totalVaHoursExportedDataPoint.setName("TotVAhExp");
        totalVaHoursExportedDataPoint.setLabel("Total VA-hours Exported");
        totalVaHoursExportedDataPoint.setDescription("Total Apparent Energy Exported");
        totalVaHoursExportedDataPoint.setUnits("VAh");
        totalVaHoursExportedDataPoint.setSize(2);
        totalVaHoursExportedDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursExportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursExportedDataPoint;

        SunSpecDataPoint totalVaHoursImportedDataPoint;
        totalVaHoursImportedDataPoint.setName("TotVAhImp");
        totalVaHoursImportedDataPoint.setLabel("Total VA-hours Imported");
        totalVaHoursImportedDataPoint.setDescription("Total Apparent Energy Imported");
        totalVaHoursImportedDataPoint.setUnits("VAh");
        totalVaHoursImportedDataPoint.setSize(2);
        totalVaHoursImportedDataPoint.setScaleFactorName("TotVAh_SF");
        totalVaHoursImportedDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVaHoursImportedDataPoint;

        SunSpecDataPoint TotVAh_SFDataPoint;
        TotVAh_SFDataPoint.setName("TotVAh_SF");
        TotVAh_SFDataPoint.setDescription("Apparent Energy scale factor");
        TotVAh_SFDataPoint.setSize(1);
        TotVAh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << TotVAh_SFDataPoint;

        SunSpecDataPoint totalVarHoursImportedQ1DataPoint;
        totalVarHoursImportedQ1DataPoint.setName("TotVArhImpQ1");
        totalVarHoursImportedQ1DataPoint.setLabel("Total VAR-hours Imported Q1");
        totalVarHoursImportedQ1DataPoint.setDescription("Total Reactive Energy Imported Quadrant 1");
        totalVarHoursImportedQ1DataPoint.setUnits("varh");
        totalVarHoursImportedQ1DataPoint.setSize(2);
        totalVarHoursImportedQ1DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ1DataPoint;

        SunSpecDataPoint totalVarHoursImportedQ2DataPoint;
        totalVarHoursImportedQ2DataPoint.setName("TotVArhImpQ2");
        totalVarHoursImportedQ2DataPoint.setLabel("Total VAr-hours Imported Q2");
        totalVarHoursImportedQ2DataPoint.setDescription("Total Reactive Power Imported Quadrant 2");
        totalVarHoursImportedQ2DataPoint.setUnits("varh");
        totalVarHoursImportedQ2DataPoint.setSize(2);
        totalVarHoursImportedQ2DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursImportedQ2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursImportedQ2DataPoint;

        SunSpecDataPoint totalVarHoursExportedQ3DataPoint;
        totalVarHoursExportedQ3DataPoint.setName("TotVArhExpQ3");
        totalVarHoursExportedQ3DataPoint.setLabel("Total VAr-hours Exported Q3");
        totalVarHoursExportedQ3DataPoint.setDescription("Total Reactive Power Exported Quadrant 3");
        totalVarHoursExportedQ3DataPoint.setUnits("varh");
        totalVarHoursExportedQ3DataPoint.setSize(2);
        totalVarHoursExportedQ3DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ3DataPoint;

        SunSpecDataPoint totalVarHoursExportedQ4DataPoint;
        totalVarHoursExportedQ4DataPoint.setName("TotVArhExpQ4");
        totalVarHoursExportedQ4DataPoint.setLabel("Total VAr-hours Exported Q4");
        totalVarHoursExportedQ4DataPoint.setDescription("Total Reactive Power Exported Quadrant 4");
        totalVarHoursExportedQ4DataPoint.setUnits("varh");
        totalVarHoursExportedQ4DataPoint.setSize(2);
        totalVarHoursExportedQ4DataPoint.setScaleFactorName("TotVArh_SF");
        totalVarHoursExportedQ4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc32"));
        m_dataPoints << totalVarHoursExportedQ4DataPoint;

        SunSpecDataPoint TotVArh_SFDataPoint;
        TotVArh_SFDataPoint.setName("TotVArh_SF");
        TotVArh_SFDataPoint.setDescription("Reactive Energy scale factor");
        TotVArh_SFDataPoint.setSize(1);
        TotVArh_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
        m_dataPoints << TotVArh_SFDataPoint;

        SunSpecDataPoint eventsDataPoint;
        eventsDataPoint.setName("Evt");
        eventsDataPoint.setLabel("Events");
        eventsDataPoint.setDescription("Meter Event Flags");
        eventsDataPoint.setMandatory(true);
        eventsDataPoint.setSize(2);
        eventsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
        m_dataPoints << eventsDataPoint;

        SunSpecDataPoint RsrvdDataPoint;
        RsrvdDataPoint.setName("Rsrvd");
        RsrvdDataPoint.setMandatory(true);
        RsrvdDataPoint.setSize(1);
        RsrvdDataPoint.setDataType(SunSpecDataPoint::stringToDataType("pad"));
        m_dataPoints << RsrvdDataPoint;

        SunSpecDataPoint timestampDataPoint;
        timestampDataPoint.setName("Ts");
        timestampDataPoint.setLabel("Timestamp");
        timestampDataPoint.setDescription("Timestamp value is the number of seconds since January 1, 2000");
        timestampDataPoint.setMandatory(true);
        timestampDataPoint.setSize(2);
        timestampDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint32"));
        m_dataPoints << timestampDataPoint;

        SunSpecDataPoint millisecondsDataPoint;
        millisecondsDataPoint.setName("Ms");
        millisecondsDataPoint.setLabel("Milliseconds");
        millisecondsDataPoint.setDescription("Millisecond counter 0-999");
        millisecondsDataPoint.setMandatory(true);
        millisecondsDataPoint.setSize(1);
        millisecondsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << millisecondsDataPoint;

        SunSpecDataPoint sequenceDataPoint;
        sequenceDataPoint.setName("Seq");
        sequenceDataPoint.setLabel("Sequence");
        sequenceDataPoint.setDescription("Sequence number of request");
        sequenceDataPoint.setMandatory(true);
        sequenceDataPoint.setSize(1);
        sequenceDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << sequenceDataPoint;

        SunSpecDataPoint algorithmDataPoint;
        algorithmDataPoint.setName("Alg");
        algorithmDataPoint.setLabel("Algorithm");
        algorithmDataPoint.setDescription("Algorithm used to compute the digital signature");
        algorithmDataPoint.setMandatory(true);
        algorithmDataPoint.setSize(1);
        algorithmDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
        m_dataPoints << algorithmDataPoint;

        SunSpecDataPoint nDataPoint;
        nDataPoint.setName("N");
        nDataPoint.setLabel("N");
        nDataPoint.setDescription("Number of registers comprising the digital signature.");
        nDataPoint.setMandatory(true);
        nDataPoint.setSize(1);
        nDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
        m_dataPoints << nDataPoint;

        break;
    }
    default:
        break;
    }
}

