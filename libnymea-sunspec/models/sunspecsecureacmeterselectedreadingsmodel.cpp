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

#include "sunspecsecureacmeterselectedreadingsmodel.h"
#include "sunspecconnection.h"

SunSpecSecureAcMeterSelectedReadingsModel::SunSpecSecureAcMeterSelectedReadingsModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 220, 42, parent)
{
    //Q_ASSERT_X(length == 42,  "SunSpecSecureAcMeterSelectedReadingsModel", QString("model length %1 given in the constructor does not match the model length from the specs %2.").arg(length).arg(modelLength()).toLatin1());
    Q_UNUSED(length)
    initDataPoints();
}

SunSpecSecureAcMeterSelectedReadingsModel::~SunSpecSecureAcMeterSelectedReadingsModel()
{

}

QString SunSpecSecureAcMeterSelectedReadingsModel::name() const
{
    return "ac_meter";
}

QString SunSpecSecureAcMeterSelectedReadingsModel::description() const
{
    return "Include this model for secure metering";
}

QString SunSpecSecureAcMeterSelectedReadingsModel::label() const
{
    return "Secure AC Meter Selected Readings";
}

float SunSpecSecureAcMeterSelectedReadingsModel::amps() const
{
    return m_amps;
}
float SunSpecSecureAcMeterSelectedReadingsModel::voltage() const
{
    return m_voltage;
}
float SunSpecSecureAcMeterSelectedReadingsModel::hz() const
{
    return m_hz;
}
float SunSpecSecureAcMeterSelectedReadingsModel::watts() const
{
    return m_watts;
}
float SunSpecSecureAcMeterSelectedReadingsModel::va() const
{
    return m_va;
}
float SunSpecSecureAcMeterSelectedReadingsModel::var() const
{
    return m_var;
}
float SunSpecSecureAcMeterSelectedReadingsModel::pf() const
{
    return m_pf;
}
quint32 SunSpecSecureAcMeterSelectedReadingsModel::totalWattHoursExported() const
{
    return m_totalWattHoursExported;
}
quint32 SunSpecSecureAcMeterSelectedReadingsModel::totalWattHoursImported() const
{
    return m_totalWattHoursImported;
}
quint32 SunSpecSecureAcMeterSelectedReadingsModel::totalVaHoursExported() const
{
    return m_totalVaHoursExported;
}
quint32 SunSpecSecureAcMeterSelectedReadingsModel::totalVaHoursImported() const
{
    return m_totalVaHoursImported;
}
quint32 SunSpecSecureAcMeterSelectedReadingsModel::totalVarHoursImportedQ1() const
{
    return m_totalVarHoursImportedQ1;
}
quint32 SunSpecSecureAcMeterSelectedReadingsModel::totalVArHoursImportedQ2() const
{
    return m_totalVArHoursImportedQ2;
}
quint32 SunSpecSecureAcMeterSelectedReadingsModel::totalVArHoursExportedQ3() const
{
    return m_totalVArHoursExportedQ3;
}
quint32 SunSpecSecureAcMeterSelectedReadingsModel::totalVArHoursExportedQ4() const
{
    return m_totalVArHoursExportedQ4;
}
SunSpecSecureAcMeterSelectedReadingsModel::EvtFlags SunSpecSecureAcMeterSelectedReadingsModel::events() const
{
    return m_events;
}
quint16 SunSpecSecureAcMeterSelectedReadingsModel::rsrvd() const
{
    return m_rsrvd;
}
quint32 SunSpecSecureAcMeterSelectedReadingsModel::timestamp() const
{
    return m_timestamp;
}
quint16 SunSpecSecureAcMeterSelectedReadingsModel::milliseconds() const
{
    return m_milliseconds;
}
quint16 SunSpecSecureAcMeterSelectedReadingsModel::sequence() const
{
    return m_sequence;
}
SunSpecSecureAcMeterSelectedReadingsModel::Alg SunSpecSecureAcMeterSelectedReadingsModel::algorithm() const
{
    return m_algorithm;
}
quint16 SunSpecSecureAcMeterSelectedReadingsModel::n() const
{
    return m_n;
}
void SunSpecSecureAcMeterSelectedReadingsModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("A_SF").isValid())
        m_a_SF = m_dataPoints.value("A_SF").toInt16();

    if (m_dataPoints.value("V_SF").isValid())
        m_v_SF = m_dataPoints.value("V_SF").toInt16();

    if (m_dataPoints.value("Hz_SF").isValid())
        m_hz_SF = m_dataPoints.value("Hz_SF").toInt16();

    if (m_dataPoints.value("W_SF").isValid())
        m_w_SF = m_dataPoints.value("W_SF").toInt16();

    if (m_dataPoints.value("VA_SF").isValid())
        m_vA_SF = m_dataPoints.value("VA_SF").toInt16();

    if (m_dataPoints.value("VAR_SF").isValid())
        m_vAR_SF = m_dataPoints.value("VAR_SF").toInt16();

    if (m_dataPoints.value("PF_SF").isValid())
        m_pF_SF = m_dataPoints.value("PF_SF").toInt16();

    if (m_dataPoints.value("TotWh_SF").isValid())
        m_totWh_SF = m_dataPoints.value("TotWh_SF").toInt16();

    if (m_dataPoints.value("TotVAh_SF").isValid())
        m_totVAh_SF = m_dataPoints.value("TotVAh_SF").toInt16();

    if (m_dataPoints.value("TotVArh_SF").isValid())
        m_totVArh_SF = m_dataPoints.value("TotVArh_SF").toInt16();


    // Update properties according to the data point type
    if (m_dataPoints.value("A").isValid())
        m_amps = m_dataPoints.value("A").toFloatWithSSF(m_a_SF);

    if (m_dataPoints.value("PhV").isValid())
        m_voltage = m_dataPoints.value("PhV").toFloatWithSSF(m_v_SF);

    if (m_dataPoints.value("Hz").isValid())
        m_hz = m_dataPoints.value("Hz").toFloatWithSSF(m_hz_SF);

    if (m_dataPoints.value("W").isValid())
        m_watts = m_dataPoints.value("W").toFloatWithSSF(m_w_SF);

    if (m_dataPoints.value("VA").isValid())
        m_va = m_dataPoints.value("VA").toFloatWithSSF(m_vA_SF);

    if (m_dataPoints.value("VAR").isValid())
        m_var = m_dataPoints.value("VAR").toFloatWithSSF(m_vAR_SF);

    if (m_dataPoints.value("PF").isValid())
        m_pf = m_dataPoints.value("PF").toFloatWithSSF(m_pF_SF);

    if (m_dataPoints.value("TotWhExp").isValid())
        m_totalWattHoursExported = m_dataPoints.value("TotWhExp").toFloatWithSSF(m_totWh_SF);

    if (m_dataPoints.value("TotWhImp").isValid())
        m_totalWattHoursImported = m_dataPoints.value("TotWhImp").toFloatWithSSF(m_totWh_SF);

    if (m_dataPoints.value("TotVAhExp").isValid())
        m_totalVaHoursExported = m_dataPoints.value("TotVAhExp").toFloatWithSSF(m_totVAh_SF);

    if (m_dataPoints.value("TotVAhImp").isValid())
        m_totalVaHoursImported = m_dataPoints.value("TotVAhImp").toFloatWithSSF(m_totVAh_SF);

    if (m_dataPoints.value("TotVArhImpQ1").isValid())
        m_totalVarHoursImportedQ1 = m_dataPoints.value("TotVArhImpQ1").toFloatWithSSF(m_totVArh_SF);

    if (m_dataPoints.value("TotVArhImpQ2").isValid())
        m_totalVArHoursImportedQ2 = m_dataPoints.value("TotVArhImpQ2").toFloatWithSSF(m_totVArh_SF);

    if (m_dataPoints.value("TotVArhExpQ3").isValid())
        m_totalVArHoursExportedQ3 = m_dataPoints.value("TotVArhExpQ3").toFloatWithSSF(m_totVArh_SF);

    if (m_dataPoints.value("TotVArhExpQ4").isValid())
        m_totalVArHoursExportedQ4 = m_dataPoints.value("TotVArhExpQ4").toFloatWithSSF(m_totVArh_SF);

    if (m_dataPoints.value("Evt").isValid())
        m_events = static_cast<EvtFlags>(m_dataPoints.value("Evt").toUInt32());

    if (m_dataPoints.value("Rsrvd").isValid())
        m_rsrvd = m_dataPoints.value("Rsrvd").toUInt16();

    if (m_dataPoints.value("Ts").isValid())
        m_timestamp = m_dataPoints.value("Ts").toUInt32();

    if (m_dataPoints.value("Ms").isValid())
        m_milliseconds = m_dataPoints.value("Ms").toUInt16();

    if (m_dataPoints.value("Seq").isValid())
        m_sequence = m_dataPoints.value("Seq").toUInt16();

    if (m_dataPoints.value("Alg").isValid())
        m_algorithm = static_cast<Alg>(m_dataPoints.value("Alg").toUInt16());

    if (m_dataPoints.value("N").isValid())
        m_n = m_dataPoints.value("N").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

void SunSpecSecureAcMeterSelectedReadingsModel::initDataPoints()
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
    ampsDataPoint.setSunSpecDataType("int16");
    m_dataPoints.insert(ampsDataPoint.name(), ampsDataPoint);

    SunSpecDataPoint a_SFDataPoint;
    a_SFDataPoint.setName("A_SF");
    a_SFDataPoint.setDescription("Current scale factor");
    a_SFDataPoint.setMandatory(true);
    a_SFDataPoint.setSize(1);
    a_SFDataPoint.setAddressOffset(3);
    a_SFDataPoint.setBlockOffset(1);
    a_SFDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(a_SFDataPoint.name(), a_SFDataPoint);

    SunSpecDataPoint voltageDataPoint;
    voltageDataPoint.setName("PhV");
    voltageDataPoint.setLabel("Voltage");
    voltageDataPoint.setDescription("Average phase or line voltage");
    voltageDataPoint.setUnits("V");
    voltageDataPoint.setSize(1);
    voltageDataPoint.setAddressOffset(4);
    voltageDataPoint.setBlockOffset(2);
    voltageDataPoint.setScaleFactorName("V_SF");
    voltageDataPoint.setSunSpecDataType("int16");
    m_dataPoints.insert(voltageDataPoint.name(), voltageDataPoint);

    SunSpecDataPoint v_SFDataPoint;
    v_SFDataPoint.setName("V_SF");
    v_SFDataPoint.setDescription("Voltage scale factor");
    v_SFDataPoint.setMandatory(true);
    v_SFDataPoint.setSize(1);
    v_SFDataPoint.setAddressOffset(5);
    v_SFDataPoint.setBlockOffset(3);
    v_SFDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(v_SFDataPoint.name(), v_SFDataPoint);

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
    hzDataPoint.setSunSpecDataType("int16");
    m_dataPoints.insert(hzDataPoint.name(), hzDataPoint);

    SunSpecDataPoint hz_SFDataPoint;
    hz_SFDataPoint.setName("Hz_SF");
    hz_SFDataPoint.setDescription("Frequency scale factor");
    hz_SFDataPoint.setSize(1);
    hz_SFDataPoint.setAddressOffset(7);
    hz_SFDataPoint.setBlockOffset(5);
    hz_SFDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(hz_SFDataPoint.name(), hz_SFDataPoint);

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
    wattsDataPoint.setSunSpecDataType("int16");
    m_dataPoints.insert(wattsDataPoint.name(), wattsDataPoint);

    SunSpecDataPoint w_SFDataPoint;
    w_SFDataPoint.setName("W_SF");
    w_SFDataPoint.setDescription("Real Power scale factor");
    w_SFDataPoint.setMandatory(true);
    w_SFDataPoint.setSize(1);
    w_SFDataPoint.setAddressOffset(9);
    w_SFDataPoint.setBlockOffset(7);
    w_SFDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(w_SFDataPoint.name(), w_SFDataPoint);

    SunSpecDataPoint vaDataPoint;
    vaDataPoint.setName("VA");
    vaDataPoint.setLabel("VA");
    vaDataPoint.setDescription("AC Apparent Power");
    vaDataPoint.setUnits("VA");
    vaDataPoint.setSize(1);
    vaDataPoint.setAddressOffset(10);
    vaDataPoint.setBlockOffset(8);
    vaDataPoint.setScaleFactorName("VA_SF");
    vaDataPoint.setSunSpecDataType("int16");
    m_dataPoints.insert(vaDataPoint.name(), vaDataPoint);

    SunSpecDataPoint vA_SFDataPoint;
    vA_SFDataPoint.setName("VA_SF");
    vA_SFDataPoint.setDescription("Apparent Power scale factor");
    vA_SFDataPoint.setSize(1);
    vA_SFDataPoint.setAddressOffset(11);
    vA_SFDataPoint.setBlockOffset(9);
    vA_SFDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(vA_SFDataPoint.name(), vA_SFDataPoint);

    SunSpecDataPoint varDataPoint;
    varDataPoint.setName("VAR");
    varDataPoint.setLabel("VAR");
    varDataPoint.setDescription("Reactive Power");
    varDataPoint.setUnits("var");
    varDataPoint.setSize(1);
    varDataPoint.setAddressOffset(12);
    varDataPoint.setBlockOffset(10);
    varDataPoint.setScaleFactorName("VAR_SF");
    varDataPoint.setSunSpecDataType("int16");
    m_dataPoints.insert(varDataPoint.name(), varDataPoint);

    SunSpecDataPoint vAR_SFDataPoint;
    vAR_SFDataPoint.setName("VAR_SF");
    vAR_SFDataPoint.setDescription("Reactive Power scale factor");
    vAR_SFDataPoint.setSize(1);
    vAR_SFDataPoint.setAddressOffset(13);
    vAR_SFDataPoint.setBlockOffset(11);
    vAR_SFDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(vAR_SFDataPoint.name(), vAR_SFDataPoint);

    SunSpecDataPoint pfDataPoint;
    pfDataPoint.setName("PF");
    pfDataPoint.setLabel("PF");
    pfDataPoint.setDescription("Power Factor");
    pfDataPoint.setUnits("Pct");
    pfDataPoint.setSize(1);
    pfDataPoint.setAddressOffset(14);
    pfDataPoint.setBlockOffset(12);
    pfDataPoint.setScaleFactorName("PF_SF");
    pfDataPoint.setSunSpecDataType("int16");
    m_dataPoints.insert(pfDataPoint.name(), pfDataPoint);

    SunSpecDataPoint pF_SFDataPoint;
    pF_SFDataPoint.setName("PF_SF");
    pF_SFDataPoint.setDescription("Power Factor scale factor");
    pF_SFDataPoint.setSize(1);
    pF_SFDataPoint.setAddressOffset(15);
    pF_SFDataPoint.setBlockOffset(13);
    pF_SFDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(pF_SFDataPoint.name(), pF_SFDataPoint);

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
    totalWattHoursExportedDataPoint.setSunSpecDataType("acc32");
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
    totalWattHoursImportedDataPoint.setSunSpecDataType("acc32");
    m_dataPoints.insert(totalWattHoursImportedDataPoint.name(), totalWattHoursImportedDataPoint);

    SunSpecDataPoint totWh_SFDataPoint;
    totWh_SFDataPoint.setName("TotWh_SF");
    totWh_SFDataPoint.setDescription("Real Energy scale factor");
    totWh_SFDataPoint.setMandatory(true);
    totWh_SFDataPoint.setSize(1);
    totWh_SFDataPoint.setAddressOffset(20);
    totWh_SFDataPoint.setBlockOffset(18);
    totWh_SFDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(totWh_SFDataPoint.name(), totWh_SFDataPoint);

    SunSpecDataPoint totalVaHoursExportedDataPoint;
    totalVaHoursExportedDataPoint.setName("TotVAhExp");
    totalVaHoursExportedDataPoint.setLabel("Total VA-hours Exported");
    totalVaHoursExportedDataPoint.setDescription("Total Apparent Energy Exported");
    totalVaHoursExportedDataPoint.setUnits("VAh");
    totalVaHoursExportedDataPoint.setSize(2);
    totalVaHoursExportedDataPoint.setAddressOffset(21);
    totalVaHoursExportedDataPoint.setBlockOffset(19);
    totalVaHoursExportedDataPoint.setScaleFactorName("TotVAh_SF");
    totalVaHoursExportedDataPoint.setSunSpecDataType("acc32");
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
    totalVaHoursImportedDataPoint.setSunSpecDataType("acc32");
    m_dataPoints.insert(totalVaHoursImportedDataPoint.name(), totalVaHoursImportedDataPoint);

    SunSpecDataPoint totVAh_SFDataPoint;
    totVAh_SFDataPoint.setName("TotVAh_SF");
    totVAh_SFDataPoint.setDescription("Apparent Energy scale factor");
    totVAh_SFDataPoint.setSize(1);
    totVAh_SFDataPoint.setAddressOffset(25);
    totVAh_SFDataPoint.setBlockOffset(23);
    totVAh_SFDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(totVAh_SFDataPoint.name(), totVAh_SFDataPoint);

    SunSpecDataPoint totalVarHoursImportedQ1DataPoint;
    totalVarHoursImportedQ1DataPoint.setName("TotVArhImpQ1");
    totalVarHoursImportedQ1DataPoint.setLabel("Total VAR-hours Imported Q1");
    totalVarHoursImportedQ1DataPoint.setDescription("Total Reactive Energy Imported Quadrant 1");
    totalVarHoursImportedQ1DataPoint.setUnits("varh");
    totalVarHoursImportedQ1DataPoint.setSize(2);
    totalVarHoursImportedQ1DataPoint.setAddressOffset(26);
    totalVarHoursImportedQ1DataPoint.setBlockOffset(24);
    totalVarHoursImportedQ1DataPoint.setScaleFactorName("TotVArh_SF");
    totalVarHoursImportedQ1DataPoint.setSunSpecDataType("acc32");
    m_dataPoints.insert(totalVarHoursImportedQ1DataPoint.name(), totalVarHoursImportedQ1DataPoint);

    SunSpecDataPoint totalVArHoursImportedQ2DataPoint;
    totalVArHoursImportedQ2DataPoint.setName("TotVArhImpQ2");
    totalVArHoursImportedQ2DataPoint.setLabel("Total VAr-hours Imported Q2");
    totalVArHoursImportedQ2DataPoint.setDescription("Total Reactive Power Imported Quadrant 2");
    totalVArHoursImportedQ2DataPoint.setUnits("varh");
    totalVArHoursImportedQ2DataPoint.setSize(2);
    totalVArHoursImportedQ2DataPoint.setAddressOffset(28);
    totalVArHoursImportedQ2DataPoint.setBlockOffset(26);
    totalVArHoursImportedQ2DataPoint.setScaleFactorName("TotVArh_SF");
    totalVArHoursImportedQ2DataPoint.setSunSpecDataType("acc32");
    m_dataPoints.insert(totalVArHoursImportedQ2DataPoint.name(), totalVArHoursImportedQ2DataPoint);

    SunSpecDataPoint totalVArHoursExportedQ3DataPoint;
    totalVArHoursExportedQ3DataPoint.setName("TotVArhExpQ3");
    totalVArHoursExportedQ3DataPoint.setLabel("Total VAr-hours Exported Q3");
    totalVArHoursExportedQ3DataPoint.setDescription("Total Reactive Power Exported Quadrant 3");
    totalVArHoursExportedQ3DataPoint.setUnits("varh");
    totalVArHoursExportedQ3DataPoint.setSize(2);
    totalVArHoursExportedQ3DataPoint.setAddressOffset(30);
    totalVArHoursExportedQ3DataPoint.setBlockOffset(28);
    totalVArHoursExportedQ3DataPoint.setScaleFactorName("TotVArh_SF");
    totalVArHoursExportedQ3DataPoint.setSunSpecDataType("acc32");
    m_dataPoints.insert(totalVArHoursExportedQ3DataPoint.name(), totalVArHoursExportedQ3DataPoint);

    SunSpecDataPoint totalVArHoursExportedQ4DataPoint;
    totalVArHoursExportedQ4DataPoint.setName("TotVArhExpQ4");
    totalVArHoursExportedQ4DataPoint.setLabel("Total VAr-hours Exported Q4");
    totalVArHoursExportedQ4DataPoint.setDescription("Total Reactive Power Exported Quadrant 4");
    totalVArHoursExportedQ4DataPoint.setUnits("varh");
    totalVArHoursExportedQ4DataPoint.setSize(2);
    totalVArHoursExportedQ4DataPoint.setAddressOffset(32);
    totalVArHoursExportedQ4DataPoint.setBlockOffset(30);
    totalVArHoursExportedQ4DataPoint.setScaleFactorName("TotVArh_SF");
    totalVArHoursExportedQ4DataPoint.setSunSpecDataType("acc32");
    m_dataPoints.insert(totalVArHoursExportedQ4DataPoint.name(), totalVArHoursExportedQ4DataPoint);

    SunSpecDataPoint totVArh_SFDataPoint;
    totVArh_SFDataPoint.setName("TotVArh_SF");
    totVArh_SFDataPoint.setDescription("Reactive Energy scale factor");
    totVArh_SFDataPoint.setSize(1);
    totVArh_SFDataPoint.setAddressOffset(34);
    totVArh_SFDataPoint.setBlockOffset(32);
    totVArh_SFDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(totVArh_SFDataPoint.name(), totVArh_SFDataPoint);

    SunSpecDataPoint eventsDataPoint;
    eventsDataPoint.setName("Evt");
    eventsDataPoint.setLabel("Events");
    eventsDataPoint.setDescription("Meter Event Flags");
    eventsDataPoint.setMandatory(true);
    eventsDataPoint.setSize(2);
    eventsDataPoint.setAddressOffset(35);
    eventsDataPoint.setBlockOffset(33);
    eventsDataPoint.setSunSpecDataType("bitfield32");
    m_dataPoints.insert(eventsDataPoint.name(), eventsDataPoint);

    SunSpecDataPoint rsrvdDataPoint;
    rsrvdDataPoint.setName("Rsrvd");
    rsrvdDataPoint.setMandatory(true);
    rsrvdDataPoint.setSize(1);
    rsrvdDataPoint.setAddressOffset(37);
    rsrvdDataPoint.setBlockOffset(35);
    rsrvdDataPoint.setSunSpecDataType("pad");
    m_dataPoints.insert(rsrvdDataPoint.name(), rsrvdDataPoint);

    SunSpecDataPoint timestampDataPoint;
    timestampDataPoint.setName("Ts");
    timestampDataPoint.setLabel("Timestamp");
    timestampDataPoint.setDescription("Timestamp value is the number of seconds since January 1, 2000");
    timestampDataPoint.setMandatory(true);
    timestampDataPoint.setSize(2);
    timestampDataPoint.setAddressOffset(38);
    timestampDataPoint.setBlockOffset(36);
    timestampDataPoint.setSunSpecDataType("uint32");
    m_dataPoints.insert(timestampDataPoint.name(), timestampDataPoint);

    SunSpecDataPoint millisecondsDataPoint;
    millisecondsDataPoint.setName("Ms");
    millisecondsDataPoint.setLabel("Milliseconds");
    millisecondsDataPoint.setDescription("Millisecond counter 0-999");
    millisecondsDataPoint.setMandatory(true);
    millisecondsDataPoint.setSize(1);
    millisecondsDataPoint.setAddressOffset(40);
    millisecondsDataPoint.setBlockOffset(38);
    millisecondsDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(millisecondsDataPoint.name(), millisecondsDataPoint);

    SunSpecDataPoint sequenceDataPoint;
    sequenceDataPoint.setName("Seq");
    sequenceDataPoint.setLabel("Sequence");
    sequenceDataPoint.setDescription("Sequence number of request");
    sequenceDataPoint.setMandatory(true);
    sequenceDataPoint.setSize(1);
    sequenceDataPoint.setAddressOffset(41);
    sequenceDataPoint.setBlockOffset(39);
    sequenceDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(sequenceDataPoint.name(), sequenceDataPoint);

    SunSpecDataPoint algorithmDataPoint;
    algorithmDataPoint.setName("Alg");
    algorithmDataPoint.setLabel("Algorithm");
    algorithmDataPoint.setDescription("Algorithm used to compute the digital signature");
    algorithmDataPoint.setMandatory(true);
    algorithmDataPoint.setSize(1);
    algorithmDataPoint.setAddressOffset(42);
    algorithmDataPoint.setBlockOffset(40);
    algorithmDataPoint.setSunSpecDataType("enum16");
    m_dataPoints.insert(algorithmDataPoint.name(), algorithmDataPoint);

    SunSpecDataPoint nDataPoint;
    nDataPoint.setName("N");
    nDataPoint.setLabel("N");
    nDataPoint.setDescription("Number of registers comprising the digital signature.");
    nDataPoint.setMandatory(true);
    nDataPoint.setSize(1);
    nDataPoint.setAddressOffset(43);
    nDataPoint.setBlockOffset(41);
    nDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(nDataPoint.name(), nDataPoint);

}

QDebug operator<<(QDebug debug, SunSpecSecureAcMeterSelectedReadingsModel *model)
{
    debug.nospace().noquote() << "SunSpecSecureAcMeterSelectedReadingsModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")" << endl;
    if (model->dataPoints().value("A").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("A") << "--> " << model->amps() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("A") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("PhV").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("PhV") << "--> " << model->voltage() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("PhV") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("Hz").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Hz") << "--> " << model->hz() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Hz") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("W").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("W") << "--> " << model->watts() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("W") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("VA").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("VA") << "--> " << model->va() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("VA") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("VAR").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("VAR") << "--> " << model->var() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("VAR") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("PF").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("PF") << "--> " << model->pf() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("PF") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("TotWhExp").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("TotWhExp") << "--> " << model->totalWattHoursExported() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("TotWhExp") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("TotWhImp").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("TotWhImp") << "--> " << model->totalWattHoursImported() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("TotWhImp") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("TotVAhExp").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVAhExp") << "--> " << model->totalVaHoursExported() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVAhExp") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("TotVAhImp").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVAhImp") << "--> " << model->totalVaHoursImported() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVAhImp") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("TotVArhImpQ1").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhImpQ1") << "--> " << model->totalVarHoursImportedQ1() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhImpQ1") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("TotVArhImpQ2").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhImpQ2") << "--> " << model->totalVArHoursImportedQ2() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhImpQ2") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("TotVArhExpQ3").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhExpQ3") << "--> " << model->totalVArHoursExportedQ3() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhExpQ3") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("TotVArhExpQ4").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhExpQ4") << "--> " << model->totalVArHoursExportedQ4() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("TotVArhExpQ4") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("Evt").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Evt") << "--> " << model->events() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Evt") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("Rsrvd").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Rsrvd") << "--> " << model->rsrvd() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Rsrvd") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("Ts").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Ts") << "--> " << model->timestamp() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Ts") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("Ms").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Ms") << "--> " << model->milliseconds() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Ms") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("Seq").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Seq") << "--> " << model->sequence() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Seq") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("Alg").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Alg") << "--> " << model->algorithm() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("Alg") << "--> NaN" << endl;
    }

    if (model->dataPoints().value("N").isValid()) {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("N") << "--> " << model->n() << endl;
    } else {
        debug.nospace().noquote() << "    - " << model->dataPoints().value("N") << "--> NaN" << endl;
    }


    return debug.space().quote();
}
