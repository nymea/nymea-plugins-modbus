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

#ifndef SUNSPECTRACKERCONTROLLERMODEL_H
#define SUNSPECTRACKERCONTROLLERMODEL_H

#include <QObject>

#include "sunspecmodel.h"
#include "sunspecmodelrepeatingblock.h"

class SunSpecConnection;
class SunSpecTrackerControllerModel;

class SunSpecTrackerControllerModelRepeatingBlock : public SunSpecModelRepeatingBlock
{
    Q_OBJECT
public:

    enum Ctl {
        CtlAutomatic = 0,
        CtlManual = 1,
        CtlCalibrate = 2
    };
    Q_ENUM(Ctl)

    enum Alm {
        AlmSetPoint = 0x1,
        AlmObsEl = 0x2,
        AlmObsAz = 0x4
    };
    Q_DECLARE_FLAGS(AlmFlags, Alm)
    Q_FLAG(Alm)

    explicit SunSpecTrackerControllerModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecTrackerControllerModel *parent);
    ~SunSpecTrackerControllerModelRepeatingBlock() override = default;

    SunSpecTrackerControllerModel *parentModel() const;

    QString name() const override;
    /* Descriptive name for this tracker unit */
    QString tracker() const;
    /* Auto target elevation in degrees from horizontal.  Unimplemented for single axis azimuth tracker type [Degrees] */
    float targetElevation() const;
    /* Auto target azimuth  in degrees from true north towards east.  Unimplemented for single axis horizontal tracker type [Degrees] */
    float targetAzimuth() const;
    /* Actual elevation position  in degrees from horizontal.  Unimplemented for single axis azimuth tracker type [Degrees] */
    float elevation() const;
    /* Actual azimuth position  in degrees from true north towards east.  Unimplemented for single axis horizontal tracker type [Degrees] */
    float azimuth() const;

    /* Manual override target position of elevation in degrees from horizontal.  Unimplemented for single axis azimuth tracker type [Degrees] */
    float manualElevation() const;
    QModbusReply *setManualElevation(float manualElevation);

    /* Manual override target position of azimuth in degrees from true north towards east.  Unimplemented for single axis azimuth tracker type [Degrees] */
    float manualAzimuth() const;
    QModbusReply *setManualAzimuth(float manualAzimuth);

    /* Control register. Normal operation is automatic.  Operator can override the position by setting the ElCtl, AzCtl and enabling Manual operation. Entering calibration mode will revert to automatic operation after calibration is complete. */
    Ctl mode() const;
    QModbusReply *setMode(Ctl mode);

    /* Tracker alarm conditions */
    AlmFlags alarm() const;

    void processBlockData() override;

protected:
    void initDataPoints() override;

private:
    SunSpecTrackerControllerModel *m_parentModel = nullptr;

    QString m_tracker;
    float m_targetElevation = 0;
    float m_targetAzimuth = 0;
    float m_elevation = 0;
    float m_azimuth = 0;
    float m_manualElevation = 0;
    float m_manualAzimuth = 0;
    Ctl m_mode;
    AlmFlags m_alarm;

};



class SunSpecTrackerControllerModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Typ {
        TypUnknown = 0,
        TypFixed = 1,
        TypHorizontal = 2,
        TypTilted = 3,
        TypAzimuth = 4,
        TypDual = 5,
        TypOther = 99
    };
    Q_ENUM(Typ)

    enum Glblctl {
        GlblctlAutomatic = 0,
        GlblctlManual = 1,
        GlblctlCalibrate = 2
    };
    Q_ENUM(Glblctl)

    enum Glblalm {
        GlblalmSetPoint = 0x1,
        GlblalmObsEl = 0x2,
        GlblalmObsAz = 0x4
    };
    Q_DECLARE_FLAGS(GlblalmFlags, Glblalm)
    Q_FLAG(Glblalm)

    explicit SunSpecTrackerControllerModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecTrackerControllerModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    /* Descriptive name for this control unit */
    QString controller() const;
    /* Type of tracker */
    Typ type() const;
    /* Local date in YYYYMMDD format [YYYYMMDD] */
    QString date() const;
    /* 24 hour local time stamp to second [hhmmss] */
    QString time() const;
    /* Number of the day in the year (1-366) */
    quint16 day() const;

    /* Global manual override target position of elevation in degrees from horizontal.  Unimplemented for single axis azimuth tracker type [Degrees] */
    float manualElevation() const;
    QModbusReply *setManualElevation(float manualElevation);

    /* Global manual override target position of azimuth in degrees from true north towards east.  Unimplemented for single axis azimuth tracker type [Degrees] */
    float manualAzimuth() const;
    QModbusReply *setManualAzimuth(float manualAzimuth);

    /* Global Control register operates on all trackers. Normal operation is automatic.  Operator can override the position by setting the ElCtl, AzCtl and enabling Manual operation. Entering calibration mode will revert to automatic operation after calibration is complete. */
    Glblctl globalMode() const;
    QModbusReply *setGlobalMode(Glblctl globalMode);

    /* Global tracker alarm conditions */
    GlblalmFlags globalAlarm() const;
    /* Scale Factor for targets and position measurements in degrees */
    qint16 sf() const;
    /* Number of trackers being controlled.  Size of repeating block. */
    quint16 trackers() const;

protected:
    quint16 m_fixedBlockLength = 26;
    quint16 m_repeatingBlockLength = 22;

    void initDataPoints();
    void processBlockData() override;

private:

    void setupRepeatingBlocks();

    QString m_controller;
    Typ m_type;
    QString m_date;
    QString m_time;
    quint16 m_day = 0;
    float m_manualElevation = 0;
    float m_manualAzimuth = 0;
    Glblctl m_globalMode;
    GlblalmFlags m_globalAlarm;
    qint16 m_sf = 0;
    quint16 m_trackers = 0;


};

QDebug operator<<(QDebug debug, SunSpecTrackerControllerModel *model);

#endif // SUNSPECTRACKERCONTROLLERMODEL_H
