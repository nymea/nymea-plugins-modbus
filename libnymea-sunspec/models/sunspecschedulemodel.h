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

#ifndef SUNSPECSCHEDULEMODEL_H
#define SUNSPECSCHEDULEMODEL_H

#include <QObject>

#include "sunspecmodel.h"
#include "sunspecmodelrepeatingblock.h"

class SunSpecConnection;
class SunSpecScheduleModel;

class SunSpecScheduleModelRepeatingBlock : public SunSpecModelRepeatingBlock
{
    Q_OBJECT
public:

    enum Intvtyp {
        IntvtypOnetime = 0,
        IntvtypDaily = 1,
        IntvtypWeekly = 2,
        IntvtypMonthly = 3,
        IntvtypWeekday = 4,
        IntvtypHoliday = 5,
        IntvtypWeekend = 6,
        IntvtypYearly = 7
    };
    Q_ENUM(Intvtyp)

    enum Xtyp {
        XtypUnset = 0,
        XtypTime = 1,
        XtypTemp = 2,
        XtypPrice = 3,
        XtypOther = 99
    };
    Q_ENUM(Xtyp)

    enum Ytyp {
        YtypUnset = 0,
        YtypWMax = 1,
        YtypRsrvd2 = 2,
        YtypPf = 3,
        YtypRsrvd4 = 4,
        YtypWattPrice = 5,
        YtypVarPrice = 6,
        YtypRsrvd7 = 7,
        YtypVoltVarArray = 8,
        YtypWChaGra = 9,
        YtypWDisChaGra = 10,
        YtypVArAval = 11,
        YtypSchedule = 12,
        YtypOther = 99
    };
    Q_ENUM(Ytyp)

    explicit SunSpecScheduleModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecScheduleModel *parent);
    ~SunSpecScheduleModelRepeatingBlock() override = default;

    SunSpecScheduleModel *parentModel() const;

    QString name() const override;

    /* Number of active entries in schedule. */
    quint16 actPts() const;
    QModbusReply *setActPts(quint16 actPts);

    /* Schedule start in seconds since 2000 JAN 01 00:00:00 UTC. [Secs] */
    quint32 strTms() const;
    QModbusReply *setStrTms(quint32 strTms);

    /* The repetition count for time-based schedules (0=repeat forever) */
    quint16 repPer() const;
    QModbusReply *setRepPer(quint16 repPer);

    /* The repetition frequency for time-based schedules: no repeat=0 */
    Intvtyp schdTyp() const;
    QModbusReply *setSchdTyp(Intvtyp schdTyp);

    /* The meaning of the X-values in the array.  */
    Xtyp xTyp() const;
    QModbusReply *setXTyp(Xtyp xTyp);

    /* Scale factor for schedule range values. */
    qint16 xSf() const;
    QModbusReply *setXSf(qint16 xSf);

    /* The meaning of the Y-values in the array. */
    Ytyp yTyp() const;
    QModbusReply *setYTyp(Ytyp yTyp);

    /* Scale factor for schedule target values. */
    qint16 ySf() const;
    QModbusReply *setYSf(qint16 ySf);

    /* Entry 1 range. */
    float x1() const;
    QModbusReply *setX1(float x1);

    /* Entry 1 target. */
    float y1() const;
    QModbusReply *setY1(float y1);

    /* Entry 2 range. */
    float x2() const;
    QModbusReply *setX2(float x2);

    /* Entry 2 target. */
    float y2() const;
    QModbusReply *setY2(float y2);

    /* Entry 3 range. */
    float x3() const;
    QModbusReply *setX3(float x3);

    /* Entry 3 target. */
    float y3() const;
    QModbusReply *setY3(float y3);

    /* Entry 4 range. */
    float x4() const;
    QModbusReply *setX4(float x4);

    /* Entry 4 target. */
    float y4() const;
    QModbusReply *setY4(float y4);

    /* Entry 15range. */
    float x5() const;
    QModbusReply *setX5(float x5);

    /* Entry 5 target. */
    float y5() const;
    QModbusReply *setY5(float y5);

    /* Entry 6 range. */
    float x6() const;
    QModbusReply *setX6(float x6);

    /* Entry 6 target. */
    float y6() const;
    QModbusReply *setY6(float y6);

    /* Entry 7 range. */
    float x7() const;
    QModbusReply *setX7(float x7);

    /* Entry 7 target. */
    float y7() const;
    QModbusReply *setY7(float y7);

    /* Entry 8 range. */
    float x8() const;
    QModbusReply *setX8(float x8);

    /* Entry 8 target. */
    float y8() const;
    QModbusReply *setY8(float y8);

    /* Entry 9 range. */
    float x9() const;
    QModbusReply *setX9(float x9);

    /* Entry 9 target. */
    float y9() const;
    QModbusReply *setY9(float y9);

    /* Entry 10 range. */
    float x10() const;
    QModbusReply *setX10(float x10);

    /* Entry 10 target. */
    float y10() const;
    QModbusReply *setY10(float y10);

    /* Optional description for schedule. */
    QString nam() const;
    QModbusReply *setNam(const QString &nam);

    /* Time window for schedule entry change. [Secs] */
    quint16 winTms() const;
    QModbusReply *setWinTms(quint16 winTms);

    /* Ramp time for moving from current target to new target. [Secs] */
    quint16 rmpTms() const;
    QModbusReply *setRmpTms(quint16 rmpTms);

    /* Index of active entry in the active schedule. */
    quint16 actIndx() const;

    void processBlockData() override;

protected:
    void initDataPoints() override;

private:
    SunSpecScheduleModel *m_parentModel = nullptr;

    quint16 m_actPts = 0;
    quint32 m_strTms = 0;
    quint16 m_repPer = 0;
    Intvtyp m_schdTyp;
    Xtyp m_xTyp;
    qint16 m_xSf = 0;
    Ytyp m_yTyp;
    qint16 m_ySf = 0;
    float m_x1 = 0;
    float m_y1 = 0;
    float m_x2 = 0;
    float m_y2 = 0;
    float m_x3 = 0;
    float m_y3 = 0;
    float m_x4 = 0;
    float m_y4 = 0;
    float m_x5 = 0;
    float m_y5 = 0;
    float m_x6 = 0;
    float m_y6 = 0;
    float m_x7 = 0;
    float m_y7 = 0;
    float m_x8 = 0;
    float m_y8 = 0;
    float m_x9 = 0;
    float m_y9 = 0;
    float m_x10 = 0;
    float m_y10 = 0;
    QString m_nam;
    quint16 m_winTms = 0;
    quint16 m_rmpTms = 0;
    quint16 m_actIndx = 0;

};



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

    explicit SunSpecScheduleModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecScheduleModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;


    /* Bitfield of active schedules */
    ActschdFlags actSchd() const;
    QModbusReply *setActSchd(ActschdFlags actSchd);

    /* Is basic scheduling active. */
    ModenaFlags modEna() const;
    QModbusReply *setModEna(ModenaFlags modEna);

    /* Number of schedules supported (recommend min. 4, max 32) */
    quint16 nSchd() const;
    /* Number of schedule entries supported (maximum of 10). */
    quint16 nPts() const;
    /* Pad register. */
    quint16 pad() const;

protected:
    quint16 m_fixedBlockLength = 6;
    quint16 m_repeatingBlockLength = 60;

    void initDataPoints();
    void processBlockData() override;

private:

    void setupRepeatingBlocks();

    ActschdFlags m_actSchd;
    ModenaFlags m_modEna;
    quint16 m_nSchd = 0;
    quint16 m_nPts = 0;
    quint16 m_pad = 0;


};

QDebug operator<<(QDebug debug, SunSpecScheduleModel *model);

#endif // SUNSPECSCHEDULEMODEL_H
