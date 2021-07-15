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
#include "sunspecconnection.h"

SunSpecTrackerControllerModelRepeatingBlock::SunSpecTrackerControllerModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecTrackerControllerModel *parent) :
    SunSpecModelRepeatingBlock(blockIndex, blockSize, modbusStartRegister, parent)
{
    initDataPoints();
}

QString SunSpecTrackerControllerModelRepeatingBlock::name() const
{
    return "tracker";
}

SunSpecTrackerControllerModel *SunSpecTrackerControllerModelRepeatingBlock::parentModel() const
{
    return m_parentModel;
}

QString SunSpecTrackerControllerModelRepeatingBlock::tracker() const
{
    return m_tracker;
}
float SunSpecTrackerControllerModelRepeatingBlock::targetElevation() const
{
    return m_targetElevation;
}
float SunSpecTrackerControllerModelRepeatingBlock::targetAzimuth() const
{
    return m_targetAzimuth;
}
float SunSpecTrackerControllerModelRepeatingBlock::elevation() const
{
    return m_elevation;
}
float SunSpecTrackerControllerModelRepeatingBlock::azimuth() const
{
    return m_azimuth;
}
float SunSpecTrackerControllerModelRepeatingBlock::manualElevation() const
{
    return m_manualElevation;
}

QModbusReply *SunSpecTrackerControllerModelRepeatingBlock::setManualElevation(float manualElevation)
{
    SunSpecDataPoint dp = m_dataPoints.value("ElCtl");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(manualElevation);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

float SunSpecTrackerControllerModelRepeatingBlock::manualAzimuth() const
{
    return m_manualAzimuth;
}

QModbusReply *SunSpecTrackerControllerModelRepeatingBlock::setManualAzimuth(float manualAzimuth)
{
    SunSpecDataPoint dp = m_dataPoints.value("AzCtl");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(manualAzimuth);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

SunSpecTrackerControllerModelRepeatingBlock::Ctl SunSpecTrackerControllerModelRepeatingBlock::mode() const
{
    return m_mode;
}

QModbusReply *SunSpecTrackerControllerModelRepeatingBlock::setMode(Ctl mode)
{
    SunSpecDataPoint dp = m_dataPoints.value("Ctl");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(mode));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());
}

SunSpecTrackerControllerModelRepeatingBlock::AlmFlags SunSpecTrackerControllerModelRepeatingBlock::alarm() const
{
    return m_alarm;
}

void SunSpecTrackerControllerModelRepeatingBlock::initDataPoints()
{
    SunSpecDataPoint trackerDataPoint;
    trackerDataPoint.setName("Id");
    trackerDataPoint.setLabel("Tracker");
    trackerDataPoint.setDescription("Descriptive name for this tracker unit");
    trackerDataPoint.setSize(8);
    trackerDataPoint.setAddressOffset(0);
    trackerDataPoint.setSunSpecDataType("string");
    m_dataPoints.insert(trackerDataPoint.name(), trackerDataPoint);

    SunSpecDataPoint targetElevationDataPoint;
    targetElevationDataPoint.setName("ElTrgt");
    targetElevationDataPoint.setLabel("Target Elevation");
    targetElevationDataPoint.setDescription("Auto target elevation in degrees from horizontal.  Unimplemented for single axis azimuth tracker type");
    targetElevationDataPoint.setUnits("Degrees");
    targetElevationDataPoint.setSize(2);
    targetElevationDataPoint.setAddressOffset(8);
    targetElevationDataPoint.setBlockOffset(6);
    targetElevationDataPoint.setScaleFactorName("Dgr_SF");
    targetElevationDataPoint.setSunSpecDataType("int32");
    m_dataPoints.insert(targetElevationDataPoint.name(), targetElevationDataPoint);

    SunSpecDataPoint targetAzimuthDataPoint;
    targetAzimuthDataPoint.setName("AzTrgt");
    targetAzimuthDataPoint.setLabel("Target Azimuth");
    targetAzimuthDataPoint.setDescription("Auto target azimuth  in degrees from true north towards east.  Unimplemented for single axis horizontal tracker type");
    targetAzimuthDataPoint.setUnits("Degrees");
    targetAzimuthDataPoint.setSize(2);
    targetAzimuthDataPoint.setAddressOffset(10);
    targetAzimuthDataPoint.setBlockOffset(8);
    targetAzimuthDataPoint.setScaleFactorName("Dgr_SF");
    targetAzimuthDataPoint.setSunSpecDataType("int32");
    m_dataPoints.insert(targetAzimuthDataPoint.name(), targetAzimuthDataPoint);

    SunSpecDataPoint elevationDataPoint;
    elevationDataPoint.setName("ElPos");
    elevationDataPoint.setLabel("Elevation");
    elevationDataPoint.setDescription("Actual elevation position  in degrees from horizontal.  Unimplemented for single axis azimuth tracker type");
    elevationDataPoint.setUnits("Degrees");
    elevationDataPoint.setSize(2);
    elevationDataPoint.setAddressOffset(12);
    elevationDataPoint.setBlockOffset(10);
    elevationDataPoint.setScaleFactorName("Dgr_SF");
    elevationDataPoint.setSunSpecDataType("int32");
    m_dataPoints.insert(elevationDataPoint.name(), elevationDataPoint);

    SunSpecDataPoint azimuthDataPoint;
    azimuthDataPoint.setName("AzPos");
    azimuthDataPoint.setLabel("Azimuth");
    azimuthDataPoint.setDescription("Actual azimuth position  in degrees from true north towards east.  Unimplemented for single axis horizontal tracker type");
    azimuthDataPoint.setUnits("Degrees");
    azimuthDataPoint.setSize(2);
    azimuthDataPoint.setAddressOffset(14);
    azimuthDataPoint.setBlockOffset(12);
    azimuthDataPoint.setScaleFactorName("Dgr_SF");
    azimuthDataPoint.setSunSpecDataType("int32");
    m_dataPoints.insert(azimuthDataPoint.name(), azimuthDataPoint);

    SunSpecDataPoint manualElevationDataPoint;
    manualElevationDataPoint.setName("ElCtl");
    manualElevationDataPoint.setLabel("Manual Elevation");
    manualElevationDataPoint.setDescription("Manual override target position of elevation in degrees from horizontal.  Unimplemented for single axis azimuth tracker type");
    manualElevationDataPoint.setUnits("Degrees");
    manualElevationDataPoint.setSize(2);
    manualElevationDataPoint.setAddressOffset(16);
    manualElevationDataPoint.setBlockOffset(14);
    manualElevationDataPoint.setScaleFactorName("Dgr_SF");
    manualElevationDataPoint.setSunSpecDataType("int32");
    manualElevationDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(manualElevationDataPoint.name(), manualElevationDataPoint);

    SunSpecDataPoint manualAzimuthDataPoint;
    manualAzimuthDataPoint.setName("AzCtl");
    manualAzimuthDataPoint.setLabel("Manual Azimuth");
    manualAzimuthDataPoint.setDescription("Manual override target position of azimuth in degrees from true north towards east.  Unimplemented for single axis azimuth tracker type");
    manualAzimuthDataPoint.setUnits("Degrees");
    manualAzimuthDataPoint.setSize(2);
    manualAzimuthDataPoint.setAddressOffset(18);
    manualAzimuthDataPoint.setBlockOffset(16);
    manualAzimuthDataPoint.setScaleFactorName("Dgr_SF");
    manualAzimuthDataPoint.setSunSpecDataType("int32");
    manualAzimuthDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(manualAzimuthDataPoint.name(), manualAzimuthDataPoint);

    SunSpecDataPoint modeDataPoint;
    modeDataPoint.setName("Ctl");
    modeDataPoint.setLabel("Mode");
    modeDataPoint.setDescription("Control register. Normal operation is automatic.  Operator can override the position by setting the ElCtl, AzCtl and enabling Manual operation. Entering calibration mode will revert to automatic operation after calibration is complete.");
    modeDataPoint.setSize(1);
    modeDataPoint.setAddressOffset(20);
    modeDataPoint.setBlockOffset(18);
    modeDataPoint.setSunSpecDataType("enum16");
    modeDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(modeDataPoint.name(), modeDataPoint);

    SunSpecDataPoint alarmDataPoint;
    alarmDataPoint.setName("Alm");
    alarmDataPoint.setLabel("Alarm");
    alarmDataPoint.setDescription("Tracker alarm conditions");
    alarmDataPoint.setSize(1);
    alarmDataPoint.setAddressOffset(21);
    alarmDataPoint.setBlockOffset(19);
    alarmDataPoint.setSunSpecDataType("bitfield16");
    m_dataPoints.insert(alarmDataPoint.name(), alarmDataPoint);

}

void SunSpecTrackerControllerModelRepeatingBlock::processBlockData(const QVector<quint16> blockData)
{
    m_blockData = blockData;

    // Update properties according to the data point type
    if (m_dataPoints.value("Id").isValid())
        m_tracker = m_dataPoints.value("Id").toString();

    if (m_dataPoints.value("ElTrgt").isValid())
        m_targetElevation = m_dataPoints.value("ElTrgt").toFloatWithSSF(m_parentModel->sf());

    if (m_dataPoints.value("AzTrgt").isValid())
        m_targetAzimuth = m_dataPoints.value("AzTrgt").toFloatWithSSF(m_parentModel->sf());

    if (m_dataPoints.value("ElPos").isValid())
        m_elevation = m_dataPoints.value("ElPos").toFloatWithSSF(m_parentModel->sf());

    if (m_dataPoints.value("AzPos").isValid())
        m_azimuth = m_dataPoints.value("AzPos").toFloatWithSSF(m_parentModel->sf());

    if (m_dataPoints.value("ElCtl").isValid())
        m_manualElevation = m_dataPoints.value("ElCtl").toFloatWithSSF(m_parentModel->sf());

    if (m_dataPoints.value("AzCtl").isValid())
        m_manualAzimuth = m_dataPoints.value("AzCtl").toFloatWithSSF(m_parentModel->sf());

    if (m_dataPoints.value("Ctl").isValid())
        m_mode = static_cast<Ctl>(m_dataPoints.value("Ctl").toUInt16());

    if (m_dataPoints.value("Alm").isValid())
        m_alarm = static_cast<AlmFlags>(m_dataPoints.value("Alm").toUInt16());


    qCDebug(dcSunSpecModelData()) << this;
}


SunSpecTrackerControllerModel::SunSpecTrackerControllerModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, QObject *parent) :
    SunSpecModel(connection, modbusStartRegister, 601, modelLength, parent)
{
    m_modelBlockType = SunSpecModel::ModelBlockTypeFixedAndRepeating;

    initDataPoints();
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
float SunSpecTrackerControllerModel::manualElevation() const
{
    return m_manualElevation;
}

QModbusReply *SunSpecTrackerControllerModel::setManualElevation(float manualElevation)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("GlblElCtl");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(manualElevation);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
float SunSpecTrackerControllerModel::manualAzimuth() const
{
    return m_manualAzimuth;
}

QModbusReply *SunSpecTrackerControllerModel::setManualAzimuth(float manualAzimuth)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("GlblAzCtl");
    QVector<quint16> registers = SunSpecDataPoint::convertFromInt32(manualAzimuth);

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecTrackerControllerModel::Glblctl SunSpecTrackerControllerModel::globalMode() const
{
    return m_globalMode;
}

QModbusReply *SunSpecTrackerControllerModel::setGlobalMode(Glblctl globalMode)
{
    if (!m_initialized)
        return nullptr;

    SunSpecDataPoint dp = m_dataPoints.value("GlblCtl");
    QVector<quint16> registers = SunSpecDataPoint::convertFromUInt16(static_cast<quint16>(globalMode));

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());
    request.setValues(registers);

    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());
}
SunSpecTrackerControllerModel::GlblalmFlags SunSpecTrackerControllerModel::globalAlarm() const
{
    return m_globalAlarm;
}
qint16 SunSpecTrackerControllerModel::sf() const
{
    return m_sf;
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

    SunSpecDataPoint controllerDataPoint;
    controllerDataPoint.setName("Nam");
    controllerDataPoint.setLabel("Controller");
    controllerDataPoint.setDescription("Descriptive name for this control unit");
    controllerDataPoint.setSize(8);
    controllerDataPoint.setAddressOffset(2);
    controllerDataPoint.setBlockOffset(0);
    controllerDataPoint.setSunSpecDataType("string");
    m_dataPoints.insert(controllerDataPoint.name(), controllerDataPoint);

    SunSpecDataPoint typeDataPoint;
    typeDataPoint.setName("Typ");
    typeDataPoint.setLabel("Type");
    typeDataPoint.setDescription("Type of tracker");
    typeDataPoint.setMandatory(true);
    typeDataPoint.setSize(1);
    typeDataPoint.setAddressOffset(10);
    typeDataPoint.setBlockOffset(8);
    typeDataPoint.setSunSpecDataType("enum16");
    m_dataPoints.insert(typeDataPoint.name(), typeDataPoint);

    SunSpecDataPoint dateDataPoint;
    dateDataPoint.setName("DtLoc");
    dateDataPoint.setLabel("Date");
    dateDataPoint.setDescription("Local date in YYYYMMDD format");
    dateDataPoint.setUnits("YYYYMMDD");
    dateDataPoint.setSize(5);
    dateDataPoint.setAddressOffset(11);
    dateDataPoint.setBlockOffset(9);
    dateDataPoint.setSunSpecDataType("string");
    m_dataPoints.insert(dateDataPoint.name(), dateDataPoint);

    SunSpecDataPoint timeDataPoint;
    timeDataPoint.setName("TmLoc");
    timeDataPoint.setLabel("Time");
    timeDataPoint.setDescription("24 hour local time stamp to second");
    timeDataPoint.setUnits("hhmmss");
    timeDataPoint.setSize(3);
    timeDataPoint.setAddressOffset(16);
    timeDataPoint.setBlockOffset(14);
    timeDataPoint.setSunSpecDataType("string");
    m_dataPoints.insert(timeDataPoint.name(), timeDataPoint);

    SunSpecDataPoint dayDataPoint;
    dayDataPoint.setName("Day");
    dayDataPoint.setLabel("Day");
    dayDataPoint.setDescription("Number of the day in the year (1-366)");
    dayDataPoint.setSize(1);
    dayDataPoint.setAddressOffset(19);
    dayDataPoint.setBlockOffset(17);
    dayDataPoint.setSunSpecDataType("uint16");
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
    manualElevationDataPoint.setSunSpecDataType("int32");
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
    manualAzimuthDataPoint.setSunSpecDataType("int32");
    manualAzimuthDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(manualAzimuthDataPoint.name(), manualAzimuthDataPoint);

    SunSpecDataPoint globalModeDataPoint;
    globalModeDataPoint.setName("GlblCtl");
    globalModeDataPoint.setLabel("Global Mode");
    globalModeDataPoint.setDescription("Global Control register operates on all trackers. Normal operation is automatic.  Operator can override the position by setting the ElCtl, AzCtl and enabling Manual operation. Entering calibration mode will revert to automatic operation after calibration is complete.");
    globalModeDataPoint.setSize(1);
    globalModeDataPoint.setAddressOffset(24);
    globalModeDataPoint.setBlockOffset(22);
    globalModeDataPoint.setSunSpecDataType("enum16");
    globalModeDataPoint.setAccess(SunSpecDataPoint::AccessReadWrite);
    m_dataPoints.insert(globalModeDataPoint.name(), globalModeDataPoint);

    SunSpecDataPoint globalAlarmDataPoint;
    globalAlarmDataPoint.setName("GlblAlm");
    globalAlarmDataPoint.setLabel("Global Alarm");
    globalAlarmDataPoint.setDescription("Global tracker alarm conditions");
    globalAlarmDataPoint.setSize(1);
    globalAlarmDataPoint.setAddressOffset(25);
    globalAlarmDataPoint.setBlockOffset(23);
    globalAlarmDataPoint.setSunSpecDataType("bitfield16");
    m_dataPoints.insert(globalAlarmDataPoint.name(), globalAlarmDataPoint);

    SunSpecDataPoint sfDataPoint;
    sfDataPoint.setName("Dgr_SF");
    sfDataPoint.setLabel("SF");
    sfDataPoint.setDescription("Scale Factor for targets and position measurements in degrees");
    sfDataPoint.setMandatory(true);
    sfDataPoint.setSize(1);
    sfDataPoint.setAddressOffset(26);
    sfDataPoint.setBlockOffset(24);
    sfDataPoint.setSunSpecDataType("sunssf");
    m_dataPoints.insert(sfDataPoint.name(), sfDataPoint);

    SunSpecDataPoint trackersDataPoint;
    trackersDataPoint.setName("N");
    trackersDataPoint.setLabel("Trackers");
    trackersDataPoint.setDescription("Number of trackers being controlled.  Size of repeating block.");
    trackersDataPoint.setMandatory(true);
    trackersDataPoint.setSize(1);
    trackersDataPoint.setAddressOffset(27);
    trackersDataPoint.setBlockOffset(25);
    trackersDataPoint.setSunSpecDataType("uint16");
    m_dataPoints.insert(trackersDataPoint.name(), trackersDataPoint);

}

void SunSpecTrackerControllerModel::processBlockData()
{
    // Scale factors
    if (m_dataPoints.value("Dgr_SF").isValid())
        m_sf = m_dataPoints.value("Dgr_SF").toInt16();


    // Update properties according to the data point type
    if (m_dataPoints.value("Nam").isValid())
        m_controller = m_dataPoints.value("Nam").toString();

    if (m_dataPoints.value("Typ").isValid())
        m_type = static_cast<Typ>(m_dataPoints.value("Typ").toUInt16());

    if (m_dataPoints.value("DtLoc").isValid())
        m_date = m_dataPoints.value("DtLoc").toString();

    if (m_dataPoints.value("TmLoc").isValid())
        m_time = m_dataPoints.value("TmLoc").toString();

    if (m_dataPoints.value("Day").isValid())
        m_day = m_dataPoints.value("Day").toUInt16();

    if (m_dataPoints.value("GlblElCtl").isValid())
        m_manualElevation = m_dataPoints.value("GlblElCtl").toFloatWithSSF(m_sf);

    if (m_dataPoints.value("GlblAzCtl").isValid())
        m_manualAzimuth = m_dataPoints.value("GlblAzCtl").toFloatWithSSF(m_sf);

    if (m_dataPoints.value("GlblCtl").isValid())
        m_globalMode = static_cast<Glblctl>(m_dataPoints.value("GlblCtl").toUInt16());

    if (m_dataPoints.value("GlblAlm").isValid())
        m_globalAlarm = static_cast<GlblalmFlags>(m_dataPoints.value("GlblAlm").toUInt16());

    if (m_dataPoints.value("Dgr_SF").isValid())
        m_sf = m_dataPoints.value("Dgr_SF").toInt16();

    if (m_dataPoints.value("N").isValid())
        m_trackers = m_dataPoints.value("N").toUInt16();


    qCDebug(dcSunSpecModelData()) << this;
}

QDebug operator<<(QDebug debug, SunSpecTrackerControllerModel *model)
{
    debug.nospace().noquote() << "SunSpecTrackerControllerModel(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")" << endl;
    debug.nospace().noquote() << "    - " << model->dataPoints().value("Nam") << "-->";
    if (model->dataPoints().value("Nam").isValid()) {
        debug.nospace().noquote() << model->controller() << endl;
    } else {
        debug.nospace().noquote() << "NaN" << endl;
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Typ") << "-->";
    if (model->dataPoints().value("Typ").isValid()) {
        debug.nospace().noquote() << model->type() << endl;
    } else {
        debug.nospace().noquote() << "NaN" << endl;
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("DtLoc") << "-->";
    if (model->dataPoints().value("DtLoc").isValid()) {
        debug.nospace().noquote() << model->date() << endl;
    } else {
        debug.nospace().noquote() << "NaN" << endl;
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("TmLoc") << "-->";
    if (model->dataPoints().value("TmLoc").isValid()) {
        debug.nospace().noquote() << model->time() << endl;
    } else {
        debug.nospace().noquote() << "NaN" << endl;
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("Day") << "-->";
    if (model->dataPoints().value("Day").isValid()) {
        debug.nospace().noquote() << model->day() << endl;
    } else {
        debug.nospace().noquote() << "NaN" << endl;
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("GlblElCtl") << "-->";
    if (model->dataPoints().value("GlblElCtl").isValid()) {
        debug.nospace().noquote() << model->manualElevation() << endl;
    } else {
        debug.nospace().noquote() << "NaN" << endl;
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("GlblAzCtl") << "-->";
    if (model->dataPoints().value("GlblAzCtl").isValid()) {
        debug.nospace().noquote() << model->manualAzimuth() << endl;
    } else {
        debug.nospace().noquote() << "NaN" << endl;
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("GlblCtl") << "-->";
    if (model->dataPoints().value("GlblCtl").isValid()) {
        debug.nospace().noquote() << model->globalMode() << endl;
    } else {
        debug.nospace().noquote() << "NaN" << endl;
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("GlblAlm") << "-->";
    if (model->dataPoints().value("GlblAlm").isValid()) {
        debug.nospace().noquote() << model->globalAlarm() << endl;
    } else {
        debug.nospace().noquote() << "NaN" << endl;
    }

    debug.nospace().noquote() << "    - " << model->dataPoints().value("N") << "-->";
    if (model->dataPoints().value("N").isValid()) {
        debug.nospace().noquote() << model->trackers() << endl;
    } else {
        debug.nospace().noquote() << "NaN" << endl;
    }


    return debug.space().quote();
}
