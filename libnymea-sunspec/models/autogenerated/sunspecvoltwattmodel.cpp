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

#include "sunspecvoltwattmodel.h"

SunSpecVoltWattModel::SunSpecVoltWattModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
}

SunSpecVoltWattModel::~SunSpecVoltWattModel()
{

}

QString SunSpecVoltWattModel::name() const
{
    return "volt_watt";
}

QString SunSpecVoltWattModel::description() const
{
    return "Volt-Watt ";
}

QString SunSpecVoltWattModel::label() const
{
    return "Volt-Watt";
}

void SunSpecVoltWattModel::readModelHeader()
{

}

void SunSpecVoltWattModel::readBlockData()
{

}

void SunSpecVoltWattModel::initDataPoints()
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
    modenaDataPoint.setDescription("Is Volt-Watt control active.");
    modenaDataPoint.setMandatory(true);
    modenaDataPoint.setSize(1);
    modenaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    modenaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << modenaDataPoint;

    SunSpecDataPoint wintmsDataPoint;
    wintmsDataPoint.setName("WinTms");
    wintmsDataPoint.setLabel("WinTms");
    wintmsDataPoint.setDescription("Time window for volt-watt change.");
    wintmsDataPoint.setUnits("Secs");
    wintmsDataPoint.setSize(1);
    wintmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wintmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << wintmsDataPoint;

    SunSpecDataPoint rvrttmsDataPoint;
    rvrttmsDataPoint.setName("RvrtTms");
    rvrttmsDataPoint.setLabel("RvrtTms");
    rvrttmsDataPoint.setDescription("Timeout period for volt-watt curve selection.");
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
    ncrvDataPoint.setDescription("Number of curves supported (recommend min. 4).");
    ncrvDataPoint.setMandatory(true);
    ncrvDataPoint.setSize(1);
    ncrvDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << ncrvDataPoint;

    SunSpecDataPoint nptDataPoint;
    nptDataPoint.setName("NPt");
    nptDataPoint.setLabel("NPt");
    nptDataPoint.setDescription("Number of points in array (maximum 20).");
    nptDataPoint.setMandatory(true);
    nptDataPoint.setSize(1);
    nptDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints << nptDataPoint;

    SunSpecDataPoint vSfDataPoint;
    vSfDataPoint.setName("V_SF");
    vSfDataPoint.setLabel("V_SF");
    vSfDataPoint.setDescription("Scale factor for percent VRef.");
    vSfDataPoint.setMandatory(true);
    vSfDataPoint.setSize(1);
    vSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << vSfDataPoint;

    SunSpecDataPoint deptrefSfDataPoint;
    deptrefSfDataPoint.setName("DeptRef_SF");
    deptrefSfDataPoint.setLabel("DeptRef_SF");
    deptrefSfDataPoint.setDescription("Scale Factor for % DeptRef");
    deptrefSfDataPoint.setMandatory(true);
    deptrefSfDataPoint.setSize(1);
    deptrefSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << deptrefSfDataPoint;

    SunSpecDataPoint rmpincdecSfDataPoint;
    rmpincdecSfDataPoint.setName("RmpIncDec_SF");
    rmpincdecSfDataPoint.setLabel("RmpIncDec_SF");
    rmpincdecSfDataPoint.setDescription("Scale factor for increment and decrement ramps.");
    rmpincdecSfDataPoint.setSize(1);
    rmpincdecSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << rmpincdecSfDataPoint;

}

