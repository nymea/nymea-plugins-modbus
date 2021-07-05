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

#include "sunspecstatusmodel.h"
#include "sunspecconnection.h"

SunSpecStatusModel::SunSpecStatusModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent) :
    SunSpecModel(connection, 122, 44, modbusStartRegister, parent)
{
    Q_ASSERT_X(length == 44,  "SunSpecStatusModel", QString("model length %1 given in the constructor does not match the model length from the specs %2.").arg(length).arg(modelLength()).toLatin1());
    Q_UNUSED(length)
    initDataPoints();
}

SunSpecStatusModel::~SunSpecStatusModel()
{

}

QString SunSpecStatusModel::name() const
{
    return "status";
}

QString SunSpecStatusModel::description() const
{
    return "Inverter Controls Extended Measurements and Status ";
}

QString SunSpecStatusModel::label() const
{
    return "Measurements_Status";
}

SunSpecStatusModel::PvconnFlags SunSpecStatusModel::pvConn() const
{
    return m_pvConn;
}
SunSpecStatusModel::StorconnFlags SunSpecStatusModel::storConn() const
{
    return m_storConn;
}
SunSpecStatusModel::EcpconnFlags SunSpecStatusModel::ecpConn() const
{
    return m_ecpConn;
}
quint64 SunSpecStatusModel::actWh() const
{
    return m_actWh;
}
quint64 SunSpecStatusModel::actVAh() const
{
    return m_actVAh;
}
quint64 SunSpecStatusModel::actVArhQ1() const
{
    return m_actVArhQ1;
}
quint64 SunSpecStatusModel::actVArhQ2() const
{
    return m_actVArhQ2;
}
quint64 SunSpecStatusModel::actVArhQ3() const
{
    return m_actVArhQ3;
}
quint64 SunSpecStatusModel::actVArhQ4() const
{
    return m_actVArhQ4;
}
float SunSpecStatusModel::vArAval() const
{
    return m_vArAval;
}
float SunSpecStatusModel::wAval() const
{
    return m_wAval;
}
SunSpecStatusModel::StsetlimmskFlags SunSpecStatusModel::stSetLimMsk() const
{
    return m_stSetLimMsk;
}
SunSpecStatusModel::StactctlFlags SunSpecStatusModel::stActCtl() const
{
    return m_stActCtl;
}
QString SunSpecStatusModel::tmSrc() const
{
    return m_tmSrc;
}
quint32 SunSpecStatusModel::tms() const
{
    return m_tms;
}
SunSpecStatusModel::RtstFlags SunSpecStatusModel::rtSt() const
{
    return m_rtSt;
}
float SunSpecStatusModel::ris() const
{
    return m_ris;
}
void SunSpecStatusModel::processBlockData()
{
    // Scale factors
    m_vArAvalSf = m_dataPoints.value("VArAval_SF").toInt16();
    m_wAvalSf = m_dataPoints.value("WAval_SF").toInt16();
    m_risSf = m_dataPoints.value("Ris_SF").toInt16();

    // Update properties according to the data point type
    m_pvConn = static_cast<PvconnFlags>(m_dataPoints.value("PVConn").toUInt16());
    m_storConn = static_cast<StorconnFlags>(m_dataPoints.value("StorConn").toUInt16());
    m_ecpConn = static_cast<EcpconnFlags>(m_dataPoints.value("ECPConn").toUInt16());
    m_actWh = m_dataPoints.value("ActWh").toInt64();
    m_actVAh = m_dataPoints.value("ActVAh").toInt64();
    m_actVArhQ1 = m_dataPoints.value("ActVArhQ1").toInt64();
    m_actVArhQ2 = m_dataPoints.value("ActVArhQ2").toInt64();
    m_actVArhQ3 = m_dataPoints.value("ActVArhQ3").toInt64();
    m_actVArhQ4 = m_dataPoints.value("ActVArhQ4").toInt64();
    m_vArAval = m_dataPoints.value("VArAval").toFloatWithSSF(m_vArAvalSf);
    m_wAval = m_dataPoints.value("WAval").toFloatWithSSF(m_wAvalSf);
    m_stSetLimMsk = static_cast<StsetlimmskFlags>(m_dataPoints.value("StSetLimMsk").toUInt32());
    m_stActCtl = static_cast<StactctlFlags>(m_dataPoints.value("StActCtl").toUInt32());
    m_tmSrc = m_dataPoints.value("TmSrc").toString();
    m_tms = m_dataPoints.value("Tms").toUInt32();
    m_rtSt = static_cast<RtstFlags>(m_dataPoints.value("RtSt").toUInt16());
    m_ris = m_dataPoints.value("Ris").toFloatWithSSF(m_risSf);
}

void SunSpecStatusModel::initDataPoints()
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

    SunSpecDataPoint pvConnDataPoint;
    pvConnDataPoint.setName("PVConn");
    pvConnDataPoint.setLabel("PVConn");
    pvConnDataPoint.setDescription("PV inverter present/available status. Enumerated value.");
    pvConnDataPoint.setMandatory(true);
    pvConnDataPoint.setSize(1);
    pvConnDataPoint.setAddressOffset(2);
    pvConnDataPoint.setBlockOffset(0);
    pvConnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    m_dataPoints.insert(pvConnDataPoint.name(), pvConnDataPoint);

    SunSpecDataPoint storConnDataPoint;
    storConnDataPoint.setName("StorConn");
    storConnDataPoint.setLabel("StorConn");
    storConnDataPoint.setDescription("Storage inverter present/available status. Enumerated value.");
    storConnDataPoint.setMandatory(true);
    storConnDataPoint.setSize(1);
    storConnDataPoint.setAddressOffset(3);
    storConnDataPoint.setBlockOffset(1);
    storConnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    m_dataPoints.insert(storConnDataPoint.name(), storConnDataPoint);

    SunSpecDataPoint ecpConnDataPoint;
    ecpConnDataPoint.setName("ECPConn");
    ecpConnDataPoint.setLabel("ECPConn");
    ecpConnDataPoint.setDescription("ECP connection status: disconnected=0  connected=1.");
    ecpConnDataPoint.setMandatory(true);
    ecpConnDataPoint.setSize(1);
    ecpConnDataPoint.setAddressOffset(4);
    ecpConnDataPoint.setBlockOffset(2);
    ecpConnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    m_dataPoints.insert(ecpConnDataPoint.name(), ecpConnDataPoint);

    SunSpecDataPoint actWhDataPoint;
    actWhDataPoint.setName("ActWh");
    actWhDataPoint.setLabel("ActWh");
    actWhDataPoint.setDescription("AC lifetime active (real) energy output.");
    actWhDataPoint.setUnits("Wh");
    actWhDataPoint.setSize(4);
    actWhDataPoint.setAddressOffset(5);
    actWhDataPoint.setBlockOffset(3);
    actWhDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc64"));
    m_dataPoints.insert(actWhDataPoint.name(), actWhDataPoint);

    SunSpecDataPoint actVAhDataPoint;
    actVAhDataPoint.setName("ActVAh");
    actVAhDataPoint.setLabel("ActVAh");
    actVAhDataPoint.setDescription("AC lifetime apparent energy output.");
    actVAhDataPoint.setUnits("VAh");
    actVAhDataPoint.setSize(4);
    actVAhDataPoint.setAddressOffset(9);
    actVAhDataPoint.setBlockOffset(7);
    actVAhDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc64"));
    m_dataPoints.insert(actVAhDataPoint.name(), actVAhDataPoint);

    SunSpecDataPoint actVArhQ1DataPoint;
    actVArhQ1DataPoint.setName("ActVArhQ1");
    actVArhQ1DataPoint.setLabel("ActVArhQ1");
    actVArhQ1DataPoint.setDescription("AC lifetime reactive energy output in quadrant 1.");
    actVArhQ1DataPoint.setUnits("varh");
    actVArhQ1DataPoint.setSize(4);
    actVArhQ1DataPoint.setAddressOffset(13);
    actVArhQ1DataPoint.setBlockOffset(11);
    actVArhQ1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc64"));
    m_dataPoints.insert(actVArhQ1DataPoint.name(), actVArhQ1DataPoint);

    SunSpecDataPoint actVArhQ2DataPoint;
    actVArhQ2DataPoint.setName("ActVArhQ2");
    actVArhQ2DataPoint.setLabel("ActVArhQ2");
    actVArhQ2DataPoint.setDescription("AC lifetime reactive energy output in quadrant 2.");
    actVArhQ2DataPoint.setUnits("varh");
    actVArhQ2DataPoint.setSize(4);
    actVArhQ2DataPoint.setAddressOffset(17);
    actVArhQ2DataPoint.setBlockOffset(15);
    actVArhQ2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc64"));
    m_dataPoints.insert(actVArhQ2DataPoint.name(), actVArhQ2DataPoint);

    SunSpecDataPoint actVArhQ3DataPoint;
    actVArhQ3DataPoint.setName("ActVArhQ3");
    actVArhQ3DataPoint.setLabel("ActVArhQ3");
    actVArhQ3DataPoint.setDescription("AC lifetime negative energy output  in quadrant 3.");
    actVArhQ3DataPoint.setUnits("varh");
    actVArhQ3DataPoint.setSize(4);
    actVArhQ3DataPoint.setAddressOffset(21);
    actVArhQ3DataPoint.setBlockOffset(19);
    actVArhQ3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc64"));
    m_dataPoints.insert(actVArhQ3DataPoint.name(), actVArhQ3DataPoint);

    SunSpecDataPoint actVArhQ4DataPoint;
    actVArhQ4DataPoint.setName("ActVArhQ4");
    actVArhQ4DataPoint.setLabel("ActVArhQ4");
    actVArhQ4DataPoint.setDescription("AC lifetime reactive energy output  in quadrant 4.");
    actVArhQ4DataPoint.setUnits("varh");
    actVArhQ4DataPoint.setSize(4);
    actVArhQ4DataPoint.setAddressOffset(25);
    actVArhQ4DataPoint.setBlockOffset(23);
    actVArhQ4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc64"));
    m_dataPoints.insert(actVArhQ4DataPoint.name(), actVArhQ4DataPoint);

    SunSpecDataPoint vArAvalDataPoint;
    vArAvalDataPoint.setName("VArAval");
    vArAvalDataPoint.setLabel("VArAval");
    vArAvalDataPoint.setDescription("Amount of VARs available without impacting watts output.");
    vArAvalDataPoint.setUnits("var");
    vArAvalDataPoint.setSize(1);
    vArAvalDataPoint.setAddressOffset(29);
    vArAvalDataPoint.setBlockOffset(27);
    vArAvalDataPoint.setScaleFactorName("VArAval_SF");
    vArAvalDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints.insert(vArAvalDataPoint.name(), vArAvalDataPoint);

    SunSpecDataPoint vArAvalSfDataPoint;
    vArAvalSfDataPoint.setName("VArAval_SF");
    vArAvalSfDataPoint.setLabel("VArAval_SF");
    vArAvalSfDataPoint.setDescription("Scale factor for available VARs.");
    vArAvalSfDataPoint.setSize(1);
    vArAvalSfDataPoint.setAddressOffset(30);
    vArAvalSfDataPoint.setBlockOffset(28);
    vArAvalSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(vArAvalSfDataPoint.name(), vArAvalSfDataPoint);

    SunSpecDataPoint wAvalDataPoint;
    wAvalDataPoint.setName("WAval");
    wAvalDataPoint.setLabel("WAval");
    wAvalDataPoint.setDescription("Amount of Watts available.");
    wAvalDataPoint.setUnits("var");
    wAvalDataPoint.setSize(1);
    wAvalDataPoint.setAddressOffset(31);
    wAvalDataPoint.setBlockOffset(29);
    wAvalDataPoint.setScaleFactorName("WAval_SF");
    wAvalDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(wAvalDataPoint.name(), wAvalDataPoint);

    SunSpecDataPoint wAvalSfDataPoint;
    wAvalSfDataPoint.setName("WAval_SF");
    wAvalSfDataPoint.setLabel("WAval_SF");
    wAvalSfDataPoint.setDescription("Scale factor for available Watts.");
    wAvalSfDataPoint.setSize(1);
    wAvalSfDataPoint.setAddressOffset(32);
    wAvalSfDataPoint.setBlockOffset(30);
    wAvalSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(wAvalSfDataPoint.name(), wAvalSfDataPoint);

    SunSpecDataPoint stSetLimMskDataPoint;
    stSetLimMskDataPoint.setName("StSetLimMsk");
    stSetLimMskDataPoint.setLabel("StSetLimMsk");
    stSetLimMskDataPoint.setDescription("Bit Mask indicating setpoint limit(s) reached.");
    stSetLimMskDataPoint.setSize(2);
    stSetLimMskDataPoint.setAddressOffset(33);
    stSetLimMskDataPoint.setBlockOffset(31);
    stSetLimMskDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(stSetLimMskDataPoint.name(), stSetLimMskDataPoint);

    SunSpecDataPoint stActCtlDataPoint;
    stActCtlDataPoint.setName("StActCtl");
    stActCtlDataPoint.setLabel("StActCtl");
    stActCtlDataPoint.setDescription("Bit Mask indicating which inverter controls are currently active.");
    stActCtlDataPoint.setSize(2);
    stActCtlDataPoint.setAddressOffset(35);
    stActCtlDataPoint.setBlockOffset(33);
    stActCtlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(stActCtlDataPoint.name(), stActCtlDataPoint);

    SunSpecDataPoint tmSrcDataPoint;
    tmSrcDataPoint.setName("TmSrc");
    tmSrcDataPoint.setLabel("TmSrc");
    tmSrcDataPoint.setDescription("Source of time synchronization.");
    tmSrcDataPoint.setSize(4);
    tmSrcDataPoint.setAddressOffset(37);
    tmSrcDataPoint.setBlockOffset(35);
    tmSrcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("string"));
    m_dataPoints.insert(tmSrcDataPoint.name(), tmSrcDataPoint);

    SunSpecDataPoint tmsDataPoint;
    tmsDataPoint.setName("Tms");
    tmsDataPoint.setLabel("Tms");
    tmsDataPoint.setDescription("Seconds since 01-01-2000 00:00 UTC");
    tmsDataPoint.setUnits("Secs");
    tmsDataPoint.setSize(2);
    tmsDataPoint.setAddressOffset(41);
    tmsDataPoint.setBlockOffset(39);
    tmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint32"));
    m_dataPoints.insert(tmsDataPoint.name(), tmsDataPoint);

    SunSpecDataPoint rtStDataPoint;
    rtStDataPoint.setName("RtSt");
    rtStDataPoint.setLabel("RtSt");
    rtStDataPoint.setDescription("Bit Mask indicating active ride-through status.");
    rtStDataPoint.setSize(1);
    rtStDataPoint.setAddressOffset(43);
    rtStDataPoint.setBlockOffset(41);
    rtStDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    m_dataPoints.insert(rtStDataPoint.name(), rtStDataPoint);

    SunSpecDataPoint risDataPoint;
    risDataPoint.setName("Ris");
    risDataPoint.setLabel("Ris");
    risDataPoint.setDescription("Isolation resistance.");
    risDataPoint.setUnits("ohms");
    risDataPoint.setSize(1);
    risDataPoint.setAddressOffset(44);
    risDataPoint.setBlockOffset(42);
    risDataPoint.setScaleFactorName("Ris_SF");
    risDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(risDataPoint.name(), risDataPoint);

    SunSpecDataPoint risSfDataPoint;
    risSfDataPoint.setName("Ris_SF");
    risSfDataPoint.setLabel("Ris_SF");
    risSfDataPoint.setDescription("Scale factor for isolation resistance.");
    risSfDataPoint.setSize(1);
    risSfDataPoint.setAddressOffset(45);
    risSfDataPoint.setBlockOffset(43);
    risSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(risSfDataPoint.name(), risSfDataPoint);

}

