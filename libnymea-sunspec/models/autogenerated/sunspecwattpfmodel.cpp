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

#include "sunspecwattpfmodel.h"

SunSpecWattPfModel::SunSpecWattPfModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
}

SunSpecWattPfModel::~SunSpecWattPfModel()
{

}

QString SunSpecWattPfModel::name() const
{
    return "watt_pf";
}

QString SunSpecWattPfModel::description() const
{
    return "Watt-Power Factor ";
}

QString SunSpecWattPfModel::label() const
{
    return "Watt-PF";
}

void SunSpecWattPfModel::readModelHeader()
{

}

void SunSpecWattPfModel::readBlockData()
{

}

void SunSpecWattPfModel::initDataPoints()
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

    SunSpecDataPoint actcrvDataPoint;
    actcrvDataPoint.setName("ActCrv");
    actcrvDataPoint.setLabel("ActCrv");
    actcrvDataPoint.setDescription("Index of active curve. 0=no active curve.");
    actcrvDataPoint.setMandatory(true);
    actcrvDataPoint.setSize(1);
    actcrvDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    actcrvDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << actcrvDataPoint;

    SunSpecDataPoint modenaDataPoint;
    modenaDataPoint.setName("ModEna");
    modenaDataPoint.setLabel("ModEna");
    modenaDataPoint.setDescription("Is watt-PF mode active.");
    modenaDataPoint.setMandatory(true);
    modenaDataPoint.setSize(1);
    modenaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    modenaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << modenaDataPoint;

    SunSpecDataPoint wintmsDataPoint;
    wintmsDataPoint.setName("WinTms");
    wintmsDataPoint.setLabel("WinTms");
    wintmsDataPoint.setDescription("Time window for watt-PF change.");
    wintmsDataPoint.setUnits("Secs");
    wintmsDataPoint.setSize(1);
    wintmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wintmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << wintmsDataPoint;

    SunSpecDataPoint rvrttmsDataPoint;
    rvrttmsDataPoint.setName("RvrtTms");
    rvrttmsDataPoint.setLabel("RvrtTms");
    rvrttmsDataPoint.setDescription("Timeout period for watt-PF curve selection.");
    rvrttmsDataPoint.setUnits("Secs");
    rvrttmsDataPoint.setSize(1);
    rvrttmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    rvrttmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << rvrttmsDataPoint;

    SunSpecDataPoint rmptmsDataPoint;
    rmptmsDataPoint.setName("RmpTms");
    rmptmsDataPoint.setLabel("RmpTms");
    rmptmsDataPoint.setDescription("Ramp time for moving from current mode to new mode.");
    rmptmsDataPoint.setUnits("Secs");
    rmptmsDataPoint.setSize(1);
    rmptmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    rmptmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << rmptmsDataPoint;

    SunSpecDataPoint ncrvDataPoint;
    ncrvDataPoint.setName("NCrv");
    ncrvDataPoint.setLabel("NCrv");
    ncrvDataPoint.setDescription("Number of curves supported (recommend 4).");
    ncrvDataPoint.setMandatory(true);
    ncrvDataPoint.setSize(1);
    ncrvDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << ncrvDataPoint;

    SunSpecDataPoint nptDataPoint;
    nptDataPoint.setName("NPt");
    nptDataPoint.setLabel("NPt");
    nptDataPoint.setDescription("Max number of points in array.");
    nptDataPoint.setMandatory(true);
    nptDataPoint.setSize(1);
    nptDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << nptDataPoint;

    SunSpecDataPoint wSfDataPoint;
    wSfDataPoint.setName("W_SF");
    wSfDataPoint.setLabel("W_SF");
    wSfDataPoint.setDescription("Scale factor for percent WMax.");
    wSfDataPoint.setMandatory(true);
    wSfDataPoint.setSize(1);
    wSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << wSfDataPoint;

    SunSpecDataPoint pfSfDataPoint;
    pfSfDataPoint.setName("PF_SF");
    pfSfDataPoint.setLabel("PF_SF");
    pfSfDataPoint.setDescription("Scale factor for PF.");
    pfSfDataPoint.setMandatory(true);
    pfSfDataPoint.setSize(1);
    pfSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << pfSfDataPoint;

    SunSpecDataPoint rmpincdecSfDataPoint;
    rmpincdecSfDataPoint.setName("RmpIncDec_SF");
    rmpincdecSfDataPoint.setLabel("RmpIncDec_SF");
    rmpincdecSfDataPoint.setDescription("Scale factor for increment and decrement ramps.");
    rmpincdecSfDataPoint.setSize(1);
    rmpincdecSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << rmpincdecSfDataPoint;

}

