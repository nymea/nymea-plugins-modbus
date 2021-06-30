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
    m_supportedModelIds << 122;
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

    SunSpecDataPoint pvconnDataPoint;
    pvconnDataPoint.setName("PVConn");
    pvconnDataPoint.setLabel("PVConn");
    pvconnDataPoint.setDescription("PV inverter present/available status. Enumerated value.");
    pvconnDataPoint.setMandatory(true);
    pvconnDataPoint.setSize(1);
    pvconnDataPoint.setAddressOffset(2);
    pvconnDataPoint.setBlockOffset(0);
    pvconnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    m_dataPoints.insert(pvconnDataPoint.name(), pvconnDataPoint);

    SunSpecDataPoint storconnDataPoint;
    storconnDataPoint.setName("StorConn");
    storconnDataPoint.setLabel("StorConn");
    storconnDataPoint.setDescription("Storage inverter present/available status. Enumerated value.");
    storconnDataPoint.setMandatory(true);
    storconnDataPoint.setSize(1);
    storconnDataPoint.setAddressOffset(3);
    storconnDataPoint.setBlockOffset(1);
    storconnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    m_dataPoints.insert(storconnDataPoint.name(), storconnDataPoint);

    SunSpecDataPoint ecpconnDataPoint;
    ecpconnDataPoint.setName("ECPConn");
    ecpconnDataPoint.setLabel("ECPConn");
    ecpconnDataPoint.setDescription("ECP connection status: disconnected=0  connected=1.");
    ecpconnDataPoint.setMandatory(true);
    ecpconnDataPoint.setSize(1);
    ecpconnDataPoint.setAddressOffset(4);
    ecpconnDataPoint.setBlockOffset(2);
    ecpconnDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    m_dataPoints.insert(ecpconnDataPoint.name(), ecpconnDataPoint);

    SunSpecDataPoint actwhDataPoint;
    actwhDataPoint.setName("ActWh");
    actwhDataPoint.setLabel("ActWh");
    actwhDataPoint.setDescription("AC lifetime active (real) energy output.");
    actwhDataPoint.setUnits("Wh");
    actwhDataPoint.setSize(4);
    actwhDataPoint.setAddressOffset(5);
    actwhDataPoint.setBlockOffset(3);
    actwhDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc64"));
    m_dataPoints.insert(actwhDataPoint.name(), actwhDataPoint);

    SunSpecDataPoint actvahDataPoint;
    actvahDataPoint.setName("ActVAh");
    actvahDataPoint.setLabel("ActVAh");
    actvahDataPoint.setDescription("AC lifetime apparent energy output.");
    actvahDataPoint.setUnits("VAh");
    actvahDataPoint.setSize(4);
    actvahDataPoint.setAddressOffset(9);
    actvahDataPoint.setBlockOffset(7);
    actvahDataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc64"));
    m_dataPoints.insert(actvahDataPoint.name(), actvahDataPoint);

    SunSpecDataPoint actvarhq1DataPoint;
    actvarhq1DataPoint.setName("ActVArhQ1");
    actvarhq1DataPoint.setLabel("ActVArhQ1");
    actvarhq1DataPoint.setDescription("AC lifetime reactive energy output in quadrant 1.");
    actvarhq1DataPoint.setUnits("varh");
    actvarhq1DataPoint.setSize(4);
    actvarhq1DataPoint.setAddressOffset(13);
    actvarhq1DataPoint.setBlockOffset(11);
    actvarhq1DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc64"));
    m_dataPoints.insert(actvarhq1DataPoint.name(), actvarhq1DataPoint);

    SunSpecDataPoint actvarhq2DataPoint;
    actvarhq2DataPoint.setName("ActVArhQ2");
    actvarhq2DataPoint.setLabel("ActVArhQ2");
    actvarhq2DataPoint.setDescription("AC lifetime reactive energy output in quadrant 2.");
    actvarhq2DataPoint.setUnits("varh");
    actvarhq2DataPoint.setSize(4);
    actvarhq2DataPoint.setAddressOffset(17);
    actvarhq2DataPoint.setBlockOffset(15);
    actvarhq2DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc64"));
    m_dataPoints.insert(actvarhq2DataPoint.name(), actvarhq2DataPoint);

    SunSpecDataPoint actvarhq3DataPoint;
    actvarhq3DataPoint.setName("ActVArhQ3");
    actvarhq3DataPoint.setLabel("ActVArhQ3");
    actvarhq3DataPoint.setDescription("AC lifetime negative energy output  in quadrant 3.");
    actvarhq3DataPoint.setUnits("varh");
    actvarhq3DataPoint.setSize(4);
    actvarhq3DataPoint.setAddressOffset(21);
    actvarhq3DataPoint.setBlockOffset(19);
    actvarhq3DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc64"));
    m_dataPoints.insert(actvarhq3DataPoint.name(), actvarhq3DataPoint);

    SunSpecDataPoint actvarhq4DataPoint;
    actvarhq4DataPoint.setName("ActVArhQ4");
    actvarhq4DataPoint.setLabel("ActVArhQ4");
    actvarhq4DataPoint.setDescription("AC lifetime reactive energy output  in quadrant 4.");
    actvarhq4DataPoint.setUnits("varh");
    actvarhq4DataPoint.setSize(4);
    actvarhq4DataPoint.setAddressOffset(25);
    actvarhq4DataPoint.setBlockOffset(23);
    actvarhq4DataPoint.setDataType(SunSpecDataPoint::stringToDataType("acc64"));
    m_dataPoints.insert(actvarhq4DataPoint.name(), actvarhq4DataPoint);

    SunSpecDataPoint varavalDataPoint;
    varavalDataPoint.setName("VArAval");
    varavalDataPoint.setLabel("VArAval");
    varavalDataPoint.setDescription("Amount of VARs available without impacting watts output.");
    varavalDataPoint.setUnits("var");
    varavalDataPoint.setSize(1);
    varavalDataPoint.setAddressOffset(29);
    varavalDataPoint.setBlockOffset(27);
    varavalDataPoint.setScaleFactorName("VArAval_SF");
    varavalDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int16"));
    m_dataPoints.insert(varavalDataPoint.name(), varavalDataPoint);

    SunSpecDataPoint varavalSfDataPoint;
    varavalSfDataPoint.setName("VArAval_SF");
    varavalSfDataPoint.setLabel("VArAval_SF");
    varavalSfDataPoint.setDescription("Scale factor for available VARs.");
    varavalSfDataPoint.setSize(1);
    varavalSfDataPoint.setAddressOffset(30);
    varavalSfDataPoint.setBlockOffset(28);
    varavalSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(varavalSfDataPoint.name(), varavalSfDataPoint);

    SunSpecDataPoint wavalDataPoint;
    wavalDataPoint.setName("WAval");
    wavalDataPoint.setLabel("WAval");
    wavalDataPoint.setDescription("Amount of Watts available.");
    wavalDataPoint.setUnits("var");
    wavalDataPoint.setSize(1);
    wavalDataPoint.setAddressOffset(31);
    wavalDataPoint.setBlockOffset(29);
    wavalDataPoint.setScaleFactorName("WAval_SF");
    wavalDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(wavalDataPoint.name(), wavalDataPoint);

    SunSpecDataPoint wavalSfDataPoint;
    wavalSfDataPoint.setName("WAval_SF");
    wavalSfDataPoint.setLabel("WAval_SF");
    wavalSfDataPoint.setDescription("Scale factor for available Watts.");
    wavalSfDataPoint.setSize(1);
    wavalSfDataPoint.setAddressOffset(32);
    wavalSfDataPoint.setBlockOffset(30);
    wavalSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(wavalSfDataPoint.name(), wavalSfDataPoint);

    SunSpecDataPoint stsetlimmskDataPoint;
    stsetlimmskDataPoint.setName("StSetLimMsk");
    stsetlimmskDataPoint.setLabel("StSetLimMsk");
    stsetlimmskDataPoint.setDescription("Bit Mask indicating setpoint limit(s) reached.");
    stsetlimmskDataPoint.setSize(2);
    stsetlimmskDataPoint.setAddressOffset(33);
    stsetlimmskDataPoint.setBlockOffset(31);
    stsetlimmskDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(stsetlimmskDataPoint.name(), stsetlimmskDataPoint);

    SunSpecDataPoint stactctlDataPoint;
    stactctlDataPoint.setName("StActCtl");
    stactctlDataPoint.setLabel("StActCtl");
    stactctlDataPoint.setDescription("Bit Mask indicating which inverter controls are currently active.");
    stactctlDataPoint.setSize(2);
    stactctlDataPoint.setAddressOffset(35);
    stactctlDataPoint.setBlockOffset(33);
    stactctlDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield32"));
    m_dataPoints.insert(stactctlDataPoint.name(), stactctlDataPoint);

    SunSpecDataPoint tmsrcDataPoint;
    tmsrcDataPoint.setName("TmSrc");
    tmsrcDataPoint.setLabel("TmSrc");
    tmsrcDataPoint.setDescription("Source of time synchronization.");
    tmsrcDataPoint.setSize(4);
    tmsrcDataPoint.setAddressOffset(37);
    tmsrcDataPoint.setBlockOffset(35);
    tmsrcDataPoint.setDataType(SunSpecDataPoint::stringToDataType("string"));
    m_dataPoints.insert(tmsrcDataPoint.name(), tmsrcDataPoint);

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

    SunSpecDataPoint rtstDataPoint;
    rtstDataPoint.setName("RtSt");
    rtstDataPoint.setLabel("RtSt");
    rtstDataPoint.setDescription("Bit Mask indicating active ride-through status.");
    rtstDataPoint.setSize(1);
    rtstDataPoint.setAddressOffset(43);
    rtstDataPoint.setBlockOffset(41);
    rtstDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    m_dataPoints.insert(rtstDataPoint.name(), rtstDataPoint);

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

