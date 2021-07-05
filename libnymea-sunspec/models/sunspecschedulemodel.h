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

#ifndef SUNSPECSCHEDULEMODEL_H
#define SUNSPECSCHEDULEMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecScheduleModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Actschd {
        ActschdSched1 = 0x1,
        ActschdSched2 = 0x2,
        ActschdSched3 = 0x4,
        ActschdSched4 = 0x8,
        ActschdSched5 = 0x10,
        ActschdSched6 = 0x20,
        ActschdSched7 = 0x40,
        ActschdSched8 = 0x80,
        ActschdSched9 = 0x100,
        ActschdSched10 = 0x200,
        ActschdSched12 = 0x400,
        ActschdSched13 = 0x800,
        ActschdSched14 = 0x1000,
        ActschdSched15 = 0x2000,
        ActschdSched16 = 0x4000,
        ActschdSched17 = 0x8000,
        ActschdSched18 = 0x10000,
        ActschdSched19 = 0x20000,
        ActschdSched20 = 0x40000,
        ActschdSched21 = 0x80000,
        ActschdSched22 = 0x200000,
        ActschdSched23 = 0x400000,
        ActschdSched24 = 0x800000,
        ActschdSched25 = 0x1000000,
        ActschdSched26 = 0x2000000,
        ActschdSched27 = 0x4000000,
        ActschdSched28 = 0x8000000,
        ActschdSched29 = 0x10000000,
        ActschdSched30 = 0x20000000,
        ActschdSched31 = 0x40000000,
        ActschdSched32 = 0x80000000
    };
    Q_DECLARE_FLAGS(ActschdFlags, Actschd)
    Q_FLAG(Actschd)

    enum Modena {
        ModenaEnabled = 0x1
    };
    Q_DECLARE_FLAGS(ModenaFlags, Modena)
    Q_FLAG(Modena)

    explicit SunSpecScheduleModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent = nullptr);
    ~SunSpecScheduleModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;


    ActschdFlags actSchd() const;
    QModbusReply *setActSchd(ActschdFlags actSchd);

    ModenaFlags modEna() const;
    QModbusReply *setModEna(ModenaFlags modEna);

    quint16 nSchd() const;
    quint16 nPts() const;
    quint16 pad() const;

protected:
    void processBlockData() override;

private:
    ActschdFlags m_actSchd;
    ModenaFlags m_modEna;
    quint16 m_nSchd = 0;
    quint16 m_nPts = 0;
    quint16 m_pad = 0;

    void initDataPoints();

};

#endif // SUNSPECSCHEDULEMODEL_H
