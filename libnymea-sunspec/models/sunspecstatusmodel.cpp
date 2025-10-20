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

#include "sunspecstatusmodel.h"
#include "sunspecconnection.h"

SunSpecStatusModel::SunSpecStatusModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 122, modelLength, byteOrder, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixed;

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
qint16 SunSpecStatusModel::vArAvalSf() const
{
    return m_vArAvalSf;
}
float SunSpecStatusModel::wAval() const
{
    return m_wAval;
}
qint16 SunSpecStatusModel::wAvalSf() const
{
    return m_wAvalSf;
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
qint16 SunSpecStatusModel::risSf() const
{
    return m_risSf;
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

    SunSpecDataPoint pvConnDataPoint;
    pvConnDataPoint.setName("PVConn");
    pvConnDataPoint.setLabel("PVConn");
    pvConnDataPoint.setDescription("PV inverter present/available status. Enumerated value.");
    pvConnDataPoint.setMandatory(true);
    pvConnDataPoint.setSize(1);
    pvConnDataPoint.setAddressOffset(2);
    pvConnDataPoint.setBlockOffset(0);
    pvConnDataPoint.setSunSpecDataType("bitfield16");
    pvConnDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(pvConnDataPoint.name(), pvConnDataPoint);

    SunSpecDataPoint storConnDataPoint;
    storConnDataPoint.setName("StorConn");
    storConnDataPoint.setLabel("StorConn");
    storConnDataPoint.setDescription("Storage inverter present/available status. Enumerated value.");
    storConnDataPoint.setMandatory(true);
    storConnDataPoint.setSize(1);
    storConnDataPoint.setAddressOffset(3);
    storConnDataPoint.setBlockOffset(1);
    storConnDataPoint.setSunSpecDataType("bitfield16");
    storConnDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(storConnDataPoint.name(), storConnDataPoint);

    SunSpecDataPoint ecpConnDataPoint;
    ecpConnDataPoint.setName("ECPConn");
    ecpConnDataPoint.setLabel("ECPConn");
    ecpConnDataPoint.setDescription("ECP connection status: disconnected=0  connected=1.");
    ecpConnDataPoint.setMandatory(true);
    ecpConnDataPoint.setSize(1);
    ecpConnDataPoint.setAddressOffset(4);
    ecpConnDataPoint.setBlockOffset(2);
    ecpConnDataPoint.setSunSpecDataType("bitfield16");
    ecpConnDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(ecpConnDataPoint.name(), ecpConnDataPoint);

    SunSpecDataPoint actWhDataPoint;
    actWhDataPoint.setName("ActWh");
    actWhDataPoint.setLabel("ActWh");
    actWhDataPoint.setDescription("AC lifetime active (real) energy output.");
    actWhDataPoint.setUnits("Wh");
    actWhDataPoint.setSize(4);
    actWhDataPoint.setAddressOffset(5);
    actWhDataPoint.setBlockOffset(3);
    actWhDataPoint.setSunSpecDataType("acc64");
    actWhDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(actWhDataPoint.name(), actWhDataPoint);

    SunSpecDataPoint actVAhDataPoint;
    actVAhDataPoint.setName("ActVAh");
    actVAhDataPoint.setLabel("ActVAh");
    actVAhDataPoint.setDescription("AC lifetime apparent energy output.");
    actVAhDataPoint.setUnits("VAh");
    actVAhDataPoint.setSize(4);
    actVAhDataPoint.setAddressOffset(9);
    actVAhDataPoint.setBlockOffset(7);
    actVAhDataPoint.setSunSpecDataType("acc64");
    actVAhDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(actVAhDataPoint.name(), actVAhDataPoint);

    SunSpecDataPoint actVArhQ1DataPoint;
    actVArhQ1DataPoint.setName("ActVArhQ1");
    actVArhQ1DataPoint.setLabel("ActVArhQ1");
    actVArhQ1DataPoint.setDescription("AC lifetime reactive energy output in quadrant 1.");
    actVArhQ1DataPoint.setUnits("varh");
    actVArhQ1DataPoint.setSize(4);
    actVArhQ1DataPoint.setAddressOffset(13);
    actVArhQ1DataPoint.setBlockOffset(11);
    actVArhQ1DataPoint.setSunSpecDataType("acc64");
    actVArhQ1DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(actVArhQ1DataPoint.name(), actVArhQ1DataPoint);

    SunSpecDataPoint actVArhQ2DataPoint;
    actVArhQ2DataPoint.setName("ActVArhQ2");
    actVArhQ2DataPoint.setLabel("ActVArhQ2");
    actVArhQ2DataPoint.setDescription("AC lifetime reactive energy output in quadrant 2.");
    actVArhQ2DataPoint.setUnits("varh");
    actVArhQ2DataPoint.setSize(4);
    actVArhQ2DataPoint.setAddressOffset(17);
    actVArhQ2DataPoint.setBlockOffset(15);
    actVArhQ2DataPoint.setSunSpecDataType("acc64");
    actVArhQ2DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(actVArhQ2DataPoint.name(), actVArhQ2DataPoint);

    SunSpecDataPoint actVArhQ3DataPoint;
    actVArhQ3DataPoint.setName("ActVArhQ3");
    actVArhQ3DataPoint.setLabel("ActVArhQ3");
    actVArhQ3DataPoint.setDescription("AC lifetime negative energy output  in quadrant 3.");
    actVArhQ3DataPoint.setUnits("varh");
    actVArhQ3DataPoint.setSize(4);
    actVArhQ3DataPoint.setAddressOffset(21);
    actVArhQ3DataPoint.setBlockOffset(19);
    actVArhQ3DataPoint.setSunSpecDataType("acc64");
    actVArhQ3DataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(actVArhQ3DataPoint.name(), actVArhQ3DataPoint);

    SunSpecDataPoint actVArhQ4DataPoint;
    actVArhQ4DataPoint.setName("ActVArhQ4");
    actVArhQ4DataPoint.setLabel("ActVArhQ4");
    actVArhQ4DataPoint.setDescription("AC lifetime reactive energy output  in quadrant 4.");
    actVArhQ4DataPoint.setUnits("varh");
    actVArhQ4DataPoint.setSize(4);
    actVArhQ4DataPoint.setAddressOffset(25);
    actVArhQ4DataPoint.setBlockOffset(23);
    actVArhQ4DataPoint.setSunSpecDataType("acc64");
    actVArhQ4DataPoint.setByteOrder(m_byteOrder);
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
    vArAvalDataPoint.setSunSpecDataType("int16");
    vArAvalDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(vArAvalDataPoint.name(), vArAvalDataPoint);

    SunSpecDataPoint vArAvalSfDataPoint;
    vArAvalSfDataPoint.setName("VArAval_SF");
    vArAvalSfDataPoint.setLabel("VArAval_SF");
    vArAvalSfDataPoint.setDescription("Scale factor for available VARs.");
    vArAvalSfDataPoint.setSize(1);
    vArAvalSfDataPoint.setAddressOffset(30);
    vArAvalSfDataPoint.setBlockOffset(28);
    vArAvalSfDataPoint.setSunSpecDataType("sunssf");
    vArAvalSfDataPoint.setByteOrder(m_byteOrder);
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
    wAvalDataPoint.setSunSpecDataType("uint16");
    wAvalDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wAvalDataPoint.name(), wAvalDataPoint);

    SunSpecDataPoint wAvalSfDataPoint;
    wAvalSfDataPoint.setName("WAval_SF");
    wAvalSfDataPoint.setLabel("WAval_SF");
    wAvalSfDataPoint.setDescription("Scale factor for available Watts.");
    wAvalSfDataPoint.setSize(1);
    wAvalSfDataPoint.setAddressOffset(32);
    wAvalSfDataPoint.setBlockOffset(30);
    wAvalSfDataPoint.setSunSpecDataType("sunssf");
    wAvalSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(wAvalSfDataPoint.name(), wAvalSfDataPoint);

    SunSpecDataPoint stSetLimMskDataPoint;
    stSetLimMskDataPoint.setName("StSetLimMsk");
    stSetLimMskDataPoint.setLabel("StSetLimMsk");
    stSetLimMskDataPoint.setDescription("Bit Mask indicating setpoint limit(s) reached.");
    stSetLimMskDataPoint.setSize(2);
    stSetLimMskDataPoint.setAddressOffset(33);
    stSetLimMskDataPoint.setBlockOffset(31);
    stSetLimMskDataPoint.setSunSpecDataType("bitfield32");
    stSetLimMskDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stSetLimMskDataPoint.name(), stSetLimMskDataPoint);

    SunSpecDataPoint stActCtlDataPoint;
    stActCtlDataPoint.setName("StActCtl");
    stActCtlDataPoint.setLabel("StActCtl");
    stActCtlDataPoint.setDescription("Bit Mask indicating which inverter controls are currently active.");
    stActCtlDataPoint.setSize(2);
    stActCtlDataPoint.setAddressOffset(35);
    stActCtlDataPoint.setBlockOffset(33);
    stActCtlDataPoint.setSunSpecDataType("bitfield32");
    stActCtlDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(stActCtlDataPoint.name(), stActCtlDataPoint);

    SunSpecDataPoint tmSrcDataPoint;
    tmSrcDataPoint.setName("TmSrc");
    tmSrcDataPoint.setLabel("TmSrc");
    tmSrcDataPoint.setDescription("Source of time synchronization.");
    tmSrcDataPoint.setSize(4);
    tmSrcDataPoint.setAddressOffset(37);
    tmSrcDataPoint.setBlockOffset(35);
    tmSrcDataPoint.setSunSpecDataType("string");
    tmSrcDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tmSrcDataPoint.name(), tmSrcDataPoint);

    SunSpecDataPoint tmsDataPoint;
    tmsDataPoint.setName("Tms");
    tmsDataPoint.setLabel("Tms");
    tmsDataPoint.setDescription("Seconds since 01-01-2000 00:00 UTC");
    tmsDataPoint.setUnits("Secs");
    tmsDataPoint.setSize(2);
    tmsDataPoint.setAddressOffset(41);
    tmsDataPoint.setBlockOffset(39);
    tmsDataPoint.setSunSpecDataType("uint32");
    tmsDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(tmsDataPoint.name(), tmsDataPoint);

    SunSpecDataPoint rtStDataPoint;
    rtStDataPoint.setName("RtSt");
    rtStDataPoint.setLabel("RtSt");
    rtStDataPoint.setDescription("Bit Mask indicating active ride-through status.");
    rtStDataPoint.setSize(1);
    rtStDataPoint.setAddressOffset(43);
    rtStDataPoint.setBlockOffset(41);
    rtStDataPoint.setSunSpecDataType("bitfield16");
    rtStDataPoint.setByteOrder(m_byteOrder);
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
    risDataPoint.setSunSpecDataType("uint16");
    risDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(risDataPoint.name(), risDataPoint);

    SunSpecDataPoint risSfDataPoint;
    risSfDataPoint.setName("Ris_SF");
    risSfDataPoint.setLabel("Ris_SF");
    risSfDataPoint.setDescription("Scale factor for isolation resistance.");
    risSfDataPoint.setSize(1);
    risSfDataPoint.setAddressOffset(45);
    risSfDataPoint.setBlockOffset(43);
    risSfDataPoint.setSunSpecDataType("sunssf");
    risSfDataPoint.setByteOrder(m_byteOrder);
    m_dataPoints.insert(risSfDataPoint.name(), risSfDataPoint);

}

void SunSpecStatusModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("VArAval_SF").isValid())
        m_vArAvalSf = m_dataPoints.value("VArAval_SF").toInt16();

    if (m_dataPoints.value("WAval_SF").isValid())
        m_wAvalSf = m_dataPoints.value("WAval_SF").toInt16();

    if (m_dataPoints.value("Ris_SF").isValid())
        m_risSf = m_dataPoints.value("Ris_SF").toInt16();


    // Update properties according to the data point type
    if (m_dataPoints.value("PVConn").isValid())
        m_pvConn = static_cast<PvconnFlags>(m_dataPoints.value("PVConn").toUInt16());

    if (m_dataPoints.value("StorConn").isValid())
        m_storConn = static_cast<StorconnFlags>(m_dataPoints.value("StorConn").toUInt16());

    if (m_dataPoints.value("ECPConn").isValid())
        m_ecpConn = static_cast<EcpconnFlags>(m_dataPoints.value("ECPConn").toUInt16());

    if (m_dataPoints.value("ActWh").isValid())
        m_actWh = m_dataPoints.value("ActWh").toInt64();

    if (m_dataPoints.value("ActVAh").isValid())
        m_actVAh = m_dataPoints.value("ActVAh").toInt64();

    if (m_dataPoints.value("ActVArhQ1").isValid())
        m_actVArhQ1 = m_dataPoints.value("ActVArhQ1").toInt64();

    if (m_dataPoints.value("ActVArhQ2").isValid())
        m_actVArhQ2 = m_dataPoints.value("ActVArhQ2").toInt64();

    if (m_dataPoints.value("ActVArhQ3").isValid())
        m_actVArhQ3 = m_dataPoints.value("ActVArhQ3").toInt64();

    if (m_dataPoints.value("ActVArhQ4").isValid())
        m_actVArhQ4 = m_dataPoints.value("ActVArhQ4").toInt64();

    if (m_dataPoints.value("VArAval").isValid())
        m_vArAval = m_dataPoints.value("VArAval").toFloatWithSSF(m_vArAvalSf);

    if (m_dataPoints.value("VArAval_SF").isValid())
        m_vArAvalSf = m_dataPoints.value("VArAval_SF").toInt16();

    if (m_dataPoints.value("WAval").isValid())
        m_wAval = m_dataPoints.value("WAval").toFloatWithSSF(m_wAvalSf);

    if (m_dataPoints.value("WAval_SF").isValid())
        m_wAvalSf = m_dataPoints.value("WAval_SF").toInt16();

    if (m_dataPoints.value("StSetLimMsk").isValid())
        m_stSetLimMsk = static_cast<StsetlimmskFlags>(m_dataPoints.value("StSetLimMsk").toUInt32());

    if (m_dataPoints.value("StActCtl").isValid())
        m_stActCtl = static_cast<StactctlFlags>(m_dataPoints.value("StActCtl").toUInt32());

    if (m_dataPoints.value("TmSrc").isValid())
        m_tmSrc = m_dataPoints.value("TmSrc").toString();

    if (m_dataPoints.value("Tms").isValid())
        m_tms = m_dataPoints.value("Tms").toUInt32();

    if (m_dataPoints.value("RtSt").isValid())
        m_rtSt = static_cast<RtstFlags>(m_dataPoints.value("RtSt").toUInt16());

    if (m_dataPoints.value("Ris").isValid())
        m_ris = m_dataPoints.value("Ris").toFloatWithSSF(m_risSf);

    if (m_dataPoints.value("Ris_SF").isValid())
        m_risSf = m_dataPoints.value("Ris_SF").toInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecStatusModel *model)
{
    debug.nospace().noquote() << "SunSpecStatusModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\n";
    debug.nospace().noquote() << "    - " << model->dataPoints().value("PVConn") << "-->";
    if (model->dataPoints().value("PVConn").isValid()) {
        debug.nospace().noquote() << model->pvConn() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("StorConn") << "-->";
    if (model->dataPoints().value("StorConn").isValid()) {
        debug.nospace().noquote() << model->storConn() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ECPConn") << "-->";
    if (model->dataPoints().value("ECPConn").isValid()) {
        debug.nospace().noquote() << model->ecpConn() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ActWh") << "-->";
    if (model->dataPoints().value("ActWh").isValid()) {
        debug.nospace().noquote() << model->actWh() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ActVAh") << "-->";
    if (model->dataPoints().value("ActVAh").isValid()) {
        debug.nospace().noquote() << model->actVAh() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ActVArhQ1") << "-->";
    if (model->dataPoints().value("ActVArhQ1").isValid()) {
        debug.nospace().noquote() << model->actVArhQ1() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ActVArhQ2") << "-->";
    if (model->dataPoints().value("ActVArhQ2").isValid()) {
        debug.nospace().noquote() << model->actVArhQ2() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ActVArhQ3") << "-->";
    if (model->dataPoints().value("ActVArhQ3").isValid()) {
        debug.nospace().noquote() << model->actVArhQ3() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("ActVArhQ4") << "-->";
    if (model->dataPoints().value("ActVArhQ4").isValid()) {
        debug.nospace().noquote() << model->actVArhQ4() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("VArAval") << "-->";
    if (model->dataPoints().value("VArAval").isValid()) {
        debug.nospace().noquote() << model->vArAval() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("WAval") << "-->";
    if (model->dataPoints().value("WAval").isValid()) {
        debug.nospace().noquote() << model->wAval() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("StSetLimMsk") << "-->";
    if (model->dataPoints().value("StSetLimMsk").isValid()) {
        debug.nospace().noquote() << model->stSetLimMsk() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("StActCtl") << "-->";
    if (model->dataPoints().value("StActCtl").isValid()) {
        debug.nospace().noquote() << model->stActCtl() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TmSrc") << "-->";
    if (model->dataPoints().value("TmSrc").isValid()) {
        debug.nospace().noquote() << model->tmSrc() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Tms") << "-->";
    if (model->dataPoints().value("Tms").isValid()) {
        debug.nospace().noquote() << model->tms() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("RtSt") << "-->";
    if (model->dataPoints().value("RtSt").isValid()) {
        debug.nospace().noquote() << model->rtSt() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Ris") << "-->";
    if (model->dataPoints().value("Ris").isValid()) {
        debug.nospace().noquote() << model->ris() << "\n";
    } else {
        debug.nospace().noquote() << "NaN\n";
    }


    return debug.space().quote();
}
