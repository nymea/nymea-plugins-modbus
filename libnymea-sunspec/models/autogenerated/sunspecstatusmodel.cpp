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

SunSpecStatusModel::SunSpecStatusModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
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

void SunSpecStatusModel::readModelHeader()
{

}

void SunSpecStatusModel::readBlockData()
{

}

void SunSpecStatusModel::initDataPoints()
{
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

    SunSpecDataPoint pvconnDataPoint;
    pvconnDataPoint.setName("PVConn");
    pvconnDataPoint.setLabel("PVConn");
    pvconnDataPoint.setDescription("PV inverter present/available status. Enumerated value.");
    pvconnDataPoint.setMandatory(true);
    pvconnDataPoint.setSize(1);
    pvconnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    m_dataPoints << pvconnDataPoint;

    SunSpecDataPoint storconnDataPoint;
    storconnDataPoint.setName("StorConn");
    storconnDataPoint.setLabel("StorConn");
    storconnDataPoint.setDescription("Storage inverter present/available status. Enumerated value.");
    storconnDataPoint.setMandatory(true);
    storconnDataPoint.setSize(1);
    storconnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    m_dataPoints << storconnDataPoint;

    SunSpecDataPoint ecpconnDataPoint;
    ecpconnDataPoint.setName("ECPConn");
    ecpconnDataPoint.setLabel("ECPConn");
    ecpconnDataPoint.setDescription("ECP connection status: disconnected=0  connected=1.");
    ecpconnDataPoint.setMandatory(true);
    ecpconnDataPoint.setSize(1);
    ecpconnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    m_dataPoints << ecpconnDataPoint;

    SunSpecDataPoint actwhDataPoint;
    actwhDataPoint.setName("ActWh");
    actwhDataPoint.setLabel("ActWh");
    actwhDataPoint.setDescription("AC lifetime active (real) energy output.");
    actwhDataPoint.setUnits("Wh");
    actwhDataPoint.setSize(4);
    actwhDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc64"));
    m_dataPoints << actwhDataPoint;

    SunSpecDataPoint actvahDataPoint;
    actvahDataPoint.setName("ActVAh");
    actvahDataPoint.setLabel("ActVAh");
    actvahDataPoint.setDescription("AC lifetime apparent energy output.");
    actvahDataPoint.setUnits("VAh");
    actvahDataPoint.setSize(4);
    actvahDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc64"));
    m_dataPoints << actvahDataPoint;

    SunSpecDataPoint actvarhq1DataPoint;
    actvarhq1DataPoint.setName("ActVArhQ1");
    actvarhq1DataPoint.setLabel("ActVArhQ1");
    actvarhq1DataPoint.setDescription("AC lifetime reactive energy output in quadrant 1.");
    actvarhq1DataPoint.setUnits("varh");
    actvarhq1DataPoint.setSize(4);
    actvarhq1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc64"));
    m_dataPoints << actvarhq1DataPoint;

    SunSpecDataPoint actvarhq2DataPoint;
    actvarhq2DataPoint.setName("ActVArhQ2");
    actvarhq2DataPoint.setLabel("ActVArhQ2");
    actvarhq2DataPoint.setDescription("AC lifetime reactive energy output in quadrant 2.");
    actvarhq2DataPoint.setUnits("varh");
    actvarhq2DataPoint.setSize(4);
    actvarhq2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc64"));
    m_dataPoints << actvarhq2DataPoint;

    SunSpecDataPoint actvarhq3DataPoint;
    actvarhq3DataPoint.setName("ActVArhQ3");
    actvarhq3DataPoint.setLabel("ActVArhQ3");
    actvarhq3DataPoint.setDescription("AC lifetime negative energy output  in quadrant 3.");
    actvarhq3DataPoint.setUnits("varh");
    actvarhq3DataPoint.setSize(4);
    actvarhq3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc64"));
    m_dataPoints << actvarhq3DataPoint;

    SunSpecDataPoint actvarhq4DataPoint;
    actvarhq4DataPoint.setName("ActVArhQ4");
    actvarhq4DataPoint.setLabel("ActVArhQ4");
    actvarhq4DataPoint.setDescription("AC lifetime reactive energy output  in quadrant 4.");
    actvarhq4DataPoint.setUnits("varh");
    actvarhq4DataPoint.setSize(4);
    actvarhq4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc64"));
    m_dataPoints << actvarhq4DataPoint;

    SunSpecDataPoint varavalDataPoint;
    varavalDataPoint.setName("VArAval");
    varavalDataPoint.setLabel("VArAval");
    varavalDataPoint.setDescription("Amount of VARs available without impacting watts output.");
    varavalDataPoint.setUnits("var");
    varavalDataPoint.setSize(1);
    varavalDataPoint.setScaleFactorName("VArAval_SF");
    varavalDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints << varavalDataPoint;

    SunSpecDataPoint varavalSfDataPoint;
    varavalSfDataPoint.setName("VArAval_SF");
    varavalSfDataPoint.setLabel("VArAval_SF");
    varavalSfDataPoint.setDescription("Scale factor for available VARs.");
    varavalSfDataPoint.setSize(1);
    varavalSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << varavalSfDataPoint;

    SunSpecDataPoint wavalDataPoint;
    wavalDataPoint.setName("WAval");
    wavalDataPoint.setLabel("WAval");
    wavalDataPoint.setDescription("Amount of Watts available.");
    wavalDataPoint.setUnits("var");
    wavalDataPoint.setSize(1);
    wavalDataPoint.setScaleFactorName("WAval_SF");
    wavalDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << wavalDataPoint;

    SunSpecDataPoint wavalSfDataPoint;
    wavalSfDataPoint.setName("WAval_SF");
    wavalSfDataPoint.setLabel("WAval_SF");
    wavalSfDataPoint.setDescription("Scale factor for available Watts.");
    wavalSfDataPoint.setSize(1);
    wavalSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << wavalSfDataPoint;

    SunSpecDataPoint stsetlimmskDataPoint;
    stsetlimmskDataPoint.setName("StSetLimMsk");
    stsetlimmskDataPoint.setLabel("StSetLimMsk");
    stsetlimmskDataPoint.setDescription("Bit Mask indicating setpoint limit(s) reached.");
    stsetlimmskDataPoint.setSize(2);
    stsetlimmskDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints << stsetlimmskDataPoint;

    SunSpecDataPoint stactctlDataPoint;
    stactctlDataPoint.setName("StActCtl");
    stactctlDataPoint.setLabel("StActCtl");
    stactctlDataPoint.setDescription("Bit Mask indicating which inverter controls are currently active.");
    stactctlDataPoint.setSize(2);
    stactctlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints << stactctlDataPoint;

    SunSpecDataPoint tmsrcDataPoint;
    tmsrcDataPoint.setName("TmSrc");
    tmsrcDataPoint.setLabel("TmSrc");
    tmsrcDataPoint.setDescription("Source of time synchronization.");
    tmsrcDataPoint.setSize(4);
    tmsrcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("string"));
    m_dataPoints << tmsrcDataPoint;

    SunSpecDataPoint tmsDataPoint;
    tmsDataPoint.setName("Tms");
    tmsDataPoint.setLabel("Tms");
    tmsDataPoint.setDescription("Seconds since 01-01-2000 00:00 UTC");
    tmsDataPoint.setUnits("Secs");
    tmsDataPoint.setSize(2);
    tmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint32"));
    m_dataPoints << tmsDataPoint;

    SunSpecDataPoint rtstDataPoint;
    rtstDataPoint.setName("RtSt");
    rtstDataPoint.setLabel("RtSt");
    rtstDataPoint.setDescription("Bit Mask indicating active ride-through status.");
    rtstDataPoint.setSize(1);
    rtstDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    m_dataPoints << rtstDataPoint;

    SunSpecDataPoint risDataPoint;
    risDataPoint.setName("Ris");
    risDataPoint.setLabel("Ris");
    risDataPoint.setDescription("Isolation resistance.");
    risDataPoint.setUnits("ohms");
    risDataPoint.setSize(1);
    risDataPoint.setScaleFactorName("Ris_SF");
    risDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << risDataPoint;

    SunSpecDataPoint risSfDataPoint;
    risSfDataPoint.setName("Ris_SF");
    risSfDataPoint.setLabel("Ris_SF");
    risSfDataPoint.setDescription("Scale factor for isolation resistance.");
    risSfDataPoint.setSize(1);
    risSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << risSfDataPoint;

}

