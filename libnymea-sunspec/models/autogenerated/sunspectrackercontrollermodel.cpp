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

#include "sunspectrackercontrollermodel.h"

SunSpecTrackerControllerModel::SunSpecTrackerControllerModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)
{
    initDataPoints();
    m_supportedModelIds << 601;
}

SunSpecTrackerControllerModel::~SunSpecTrackerControllerModel()
{

}

QString SunSpecTrackerControllerModel::name() const
{
    return "tracker_controller";
}

QString SunSpecTrackerControllerModel::description() const
{
    return "Monitors and controls multiple trackers";
}

QString SunSpecTrackerControllerModel::label() const
{
    return "Tracker Controller DRAFT 2";
}

quint16 SunSpecTrackerControllerModel::modelId() const
{
    return m_modelId;
}
quint16 SunSpecTrackerControllerModel::modelLength() const
{
    return m_modelLength;
}
QString SunSpecTrackerControllerModel::controller() const
{
    return m_controller;
}
SunSpecTrackerControllerModel::Typ SunSpecTrackerControllerModel::type() const
{
    return m_type;
}
QString SunSpecTrackerControllerModel::date() const
{
    return m_date;
}
QString SunSpecTrackerControllerModel::time() const
{
    return m_time;
}
quint16 SunSpecTrackerControllerModel::day() const
{
    return m_day;
}
qint32 SunSpecTrackerControllerModel::manualElevation() const
{
    return m_manualElevation;
}
qint32 SunSpecTrackerControllerModel::manualAzimuth() const
{
    return m_manualAzimuth;
}
SunSpecTrackerControllerModel::Glblctl SunSpecTrackerControllerModel::globalMode() const
{
    return m_globalMode;
}
SunSpecTrackerControllerModel::GlblalmFlags SunSpecTrackerControllerModel::globalAlarm() const
{
    return m_globalAlarm;
}
quint16 SunSpecTrackerControllerModel::trackers() const
{
    return m_trackers;
}
void SunSpecTrackerControllerModel::initDataPoints()
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

    SunSpecDataPoint controllerDataPoint;
    controllerDataPoint.setName("Nam");
    controllerDataPoint.setLabel("Controller");
    controllerDataPoint.setDescription("Descriptive name for this control unit");
    controllerDataPoint.setSize(8);
    controllerDataPoint.setAddressOffset(2);
    controllerDataPoint.setBlockOffset(0);
    controllerDataPoint.setDataType(SunSpecDataPoint::stringToDataType("string"));
    m_dataPoints.insert(controllerDataPoint.name(), controllerDataPoint);

    SunSpecDataPoint typeDataPoint;
    typeDataPoint.setName("Typ");
    typeDataPoint.setLabel("Type");
    typeDataPoint.setDescription("Type of tracker");
    typeDataPoint.setMandatory(true);
    typeDataPoint.setSize(1);
    typeDataPoint.setAddressOffset(10);
    typeDataPoint.setBlockOffset(8);
    typeDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    m_dataPoints.insert(typeDataPoint.name(), typeDataPoint);

    SunSpecDataPoint dateDataPoint;
    dateDataPoint.setName("DtLoc");
    dateDataPoint.setLabel("Date");
    dateDataPoint.setDescription("Local date in YYYYMMDD format");
    dateDataPoint.setUnits("YYYYMMDD");
    dateDataPoint.setSize(5);
    dateDataPoint.setAddressOffset(11);
    dateDataPoint.setBlockOffset(9);
    dateDataPoint.setDataType(SunSpecDataPoint::stringToDataType("string"));
    m_dataPoints.insert(dateDataPoint.name(), dateDataPoint);

    SunSpecDataPoint timeDataPoint;
    timeDataPoint.setName("TmLoc");
    timeDataPoint.setLabel("Time");
    timeDataPoint.setDescription("24 hour local time stamp to second");
    timeDataPoint.setUnits("hhmmss");
    timeDataPoint.setSize(3);
    timeDataPoint.setAddressOffset(16);
    timeDataPoint.setBlockOffset(14);
    timeDataPoint.setDataType(SunSpecDataPoint::stringToDataType("string"));
    m_dataPoints.insert(timeDataPoint.name(), timeDataPoint);

    SunSpecDataPoint dayDataPoint;
    dayDataPoint.setName("Day");
    dayDataPoint.setLabel("Day");
    dayDataPoint.setDescription("Number of the day in the year (1-366)");
    dayDataPoint.setSize(1);
    dayDataPoint.setAddressOffset(19);
    dayDataPoint.setBlockOffset(17);
    dayDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(dayDataPoint.name(), dayDataPoint);

    SunSpecDataPoint manualElevationDataPoint;
    manualElevationDataPoint.setName("GlblElCtl");
    manualElevationDataPoint.setLabel("Manual Elevation");
    manualElevationDataPoint.setDescription("Global manual override target position of elevation in degrees from horizontal.  Unimplemented for single axis azimuth tracker type");
    manualElevationDataPoint.setUnits("Degrees");
    manualElevationDataPoint.setSize(2);
    manualElevationDataPoint.setAddressOffset(20);
    manualElevationDataPoint.setBlockOffset(18);
    manualElevationDataPoint.setScaleFactorName("Dgr_SF");
    manualElevationDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int32"));
    manualElevationDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(manualElevationDataPoint.name(), manualElevationDataPoint);

    SunSpecDataPoint manualAzimuthDataPoint;
    manualAzimuthDataPoint.setName("GlblAzCtl");
    manualAzimuthDataPoint.setLabel("Manual Azimuth");
    manualAzimuthDataPoint.setDescription("Global manual override target position of azimuth in degrees from true north towards east.  Unimplemented for single axis azimuth tracker type");
    manualAzimuthDataPoint.setUnits("Degrees");
    manualAzimuthDataPoint.setSize(2);
    manualAzimuthDataPoint.setAddressOffset(22);
    manualAzimuthDataPoint.setBlockOffset(20);
    manualAzimuthDataPoint.setScaleFactorName("Dgr_SF");
    manualAzimuthDataPoint.setDataType(SunSpecDataPoint::stringToDataType("int32"));
    manualAzimuthDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(manualAzimuthDataPoint.name(), manualAzimuthDataPoint);

    SunSpecDataPoint globalModeDataPoint;
    globalModeDataPoint.setName("GlblCtl");
    globalModeDataPoint.setLabel("Global Mode");
    globalModeDataPoint.setDescription("Global Control register operates on all trackers. Normal operation is automatic.  Operator can override the position by setting the ElCtl, AzCtl and enabling Manual operation. Entering calibration mode will revert to automatic operation after calibration is complete.");
    globalModeDataPoint.setSize(1);
    globalModeDataPoint.setAddressOffset(24);
    globalModeDataPoint.setBlockOffset(22);
    globalModeDataPoint.setDataType(SunSpecDataPoint::stringToDataType("enum16"));
    globalModeDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(globalModeDataPoint.name(), globalModeDataPoint);

    SunSpecDataPoint globalAlarmDataPoint;
    globalAlarmDataPoint.setName("GlblAlm");
    globalAlarmDataPoint.setLabel("Global Alarm");
    globalAlarmDataPoint.setDescription("Global tracker alarm conditions");
    globalAlarmDataPoint.setSize(1);
    globalAlarmDataPoint.setAddressOffset(25);
    globalAlarmDataPoint.setBlockOffset(23);
    globalAlarmDataPoint.setDataType(SunSpecDataPoint::stringToDataType("bitfield16"));
    m_dataPoints.insert(globalAlarmDataPoint.name(), globalAlarmDataPoint);

    SunSpecDataPoint sfDataPoint;
    sfDataPoint.setName("Dgr_SF");
    sfDataPoint.setLabel("SF");
    sfDataPoint.setDescription("Scale Factor for targets and position measurements in degrees");
    sfDataPoint.setMandatory(true);
    sfDataPoint.setSize(1);
    sfDataPoint.setAddressOffset(26);
    sfDataPoint.setBlockOffset(24);
    sfDataPoint.setDataType(SunSpecDataPoint::stringToDataType("sunssf"));
    m_dataPoints.insert(sfDataPoint.name(), sfDataPoint);

    SunSpecDataPoint trackersDataPoint;
    trackersDataPoint.setName("N");
    trackersDataPoint.setLabel("Trackers");
    trackersDataPoint.setDescription("Number of trackers being controlled.  Size of repeating block.");
    trackersDataPoint.setMandatory(true);
    trackersDataPoint.setSize(1);
    trackersDataPoint.setAddressOffset(27);
    trackersDataPoint.setBlockOffset(25);
    trackersDataPoint.setDataType(SunSpecDataPoint::stringToDataType("uint16"));
    m_dataPoints.insert(trackersDataPoint.name(), trackersDataPoint);

}

