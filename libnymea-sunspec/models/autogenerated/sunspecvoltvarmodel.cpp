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

#include "sunspecvoltvarmodel.h"

SunSpecVoltVarModel::SunSpecVoltVarModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
}

SunSpecVoltVarModel::~SunSpecVoltVarModel()
{

}

QString SunSpecVoltVarModel::name() const
{
    return "volt_var";
}

QString SunSpecVoltVarModel::description() const
{
    return "Static Volt-VAR Arrays ";
}

QString SunSpecVoltVarModel::label() const
{
    return "Static Volt-VAR";
}

void SunSpecVoltVarModel::readModelHeader()
{

}

void SunSpecVoltVarModel::readBlockData()
{

}

void SunSpecVoltVarModel::initDataPoints()
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
    modenaDataPoint.setDescription("Is Volt-VAR control active.");
    modenaDataPoint.setMandatory(true);
    modenaDataPoint.setSize(1);
    modenaDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    modenaDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << modenaDataPoint;

    SunSpecDataPoint wintmsDataPoint;
    wintmsDataPoint.setName("WinTms");
    wintmsDataPoint.setLabel("WinTms");
    wintmsDataPoint.setDescription("Time window for volt-VAR change.");
    wintmsDataPoint.setUnits("Secs");
    wintmsDataPoint.setSize(1);
    wintmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    wintmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << wintmsDataPoint;

    SunSpecDataPoint rvrttmsDataPoint;
    rvrttmsDataPoint.setName("RvrtTms");
    rvrttmsDataPoint.setLabel("RvrtTms");
    rvrttmsDataPoint.setDescription("Timeout period for volt-VAR curve selection.");
    rvrttmsDataPoint.setUnits("Secs");
    rvrttmsDataPoint.setSize(1);
    rvrttmsDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    rvrttmsDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints << rvrttmsDataPoint;

    SunSpecDataPoint rmptmsDataPoint;
    rmptmsDataPoint.setName("RmpTms");
    rmptmsDataPoint.setLabel("RmpTms");
    rmptmsDataPoint.setDescription("The time of the PT1 in seconds (time to accomplish a change of 95%).");
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
    nptDataPoint.setDescription("Number of curve points supported (maximum of 20).");
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
    deptrefSfDataPoint.setDescription("scale factor for dependent variable.");
    deptrefSfDataPoint.setMandatory(true);
    deptrefSfDataPoint.setSize(1);
    deptrefSfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << deptrefSfDataPoint;

    SunSpecDataPoint RmpIncDec_SFDataPoint;
    RmpIncDec_SFDataPoint.setName("RmpIncDec_SF");
    RmpIncDec_SFDataPoint.setSize(1);
    RmpIncDec_SFDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints << RmpIncDec_SFDataPoint;

}

