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

#ifndef SUNSPECTRACKERCONTROLLERMODEL_H
#define SUNSPECTRACKERCONTROLLERMODEL_H

#include <QObject>

#include "sunspec.h"
#include "sunspecmodel.h"

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

    explicit SunSpecTrackerControllerModel(SunSpec *connection, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecTrackerControllerModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    QString controller() const;
    Typ type() const;
    QString date() const;
    QString time() const;
    quint16 day() const;
    float manualElevation() const;
    float manualAzimuth() const;
    Glblctl globalMode() const;
    GlblalmFlags globalAlarm() const;
    quint16 trackers() const;

protected:
    void processBlockData() override;

private:
    QString m_controller;
    Typ m_type;
    QString m_date;
    QString m_time;
    quint16 m_day;
    float m_manualElevation;
    float m_manualAzimuth;
    Glblctl m_globalMode;
    GlblalmFlags m_globalAlarm;
    qint16 m_sf;
    quint16 m_trackers;

    void initDataPoints();

};

#endif // SUNSPECTRACKERCONTROLLERMODEL_H
