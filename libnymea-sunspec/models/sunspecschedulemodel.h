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

    explicit SunSpecScheduleModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecScheduleModel *parent = nullptr);
    ~SunSpecScheduleModelRepeatingBlock() override = default;

    SunSpecScheduleModel *parentModel() const;

    QString name() const override;

    quint16 actPts() const;
    QModbusReply *setActPts(quint16 actPts);

    quint32 strTms() const;
    QModbusReply *setStrTms(quint32 strTms);

    quint16 repPer() const;
    QModbusReply *setRepPer(quint16 repPer);

    Intvtyp schdTyp() const;
    QModbusReply *setSchdTyp(Intvtyp schdTyp);

    Xtyp xTyp() const;
    QModbusReply *setXTyp(Xtyp xTyp);

    qint16 xSf() const;
    QModbusReply *setXSf(qint16 xSf);

    Ytyp yTyp() const;
    QModbusReply *setYTyp(Ytyp yTyp);

    qint16 ySf() const;
    QModbusReply *setYSf(qint16 ySf);

    float x1() const;
    QModbusReply *setX1(float x1);

    float y1() const;
    QModbusReply *setY1(float y1);

    float x2() const;
    QModbusReply *setX2(float x2);

    float y2() const;
    QModbusReply *setY2(float y2);

    float x3() const;
    QModbusReply *setX3(float x3);

    float y3() const;
    QModbusReply *setY3(float y3);

    float x4() const;
    QModbusReply *setX4(float x4);

    float y4() const;
    QModbusReply *setY4(float y4);

    float x5() const;
    QModbusReply *setX5(float x5);

    float y5() const;
    QModbusReply *setY5(float y5);

    float x6() const;
    QModbusReply *setX6(float x6);

    float y6() const;
    QModbusReply *setY6(float y6);

    float x7() const;
    QModbusReply *setX7(float x7);

    float y7() const;
    QModbusReply *setY7(float y7);

    float x8() const;
    QModbusReply *setX8(float x8);

    float y8() const;
    QModbusReply *setY8(float y8);

    float x9() const;
    QModbusReply *setX9(float x9);

    float y9() const;
    QModbusReply *setY9(float y9);

    float x10() const;
    QModbusReply *setX10(float x10);

    float y10() const;
    QModbusReply *setY10(float y10);

    QString nam() const;
    QModbusReply *setNam(const QString &nam);

    quint16 winTms() const;
    QModbusReply *setWinTms(quint16 winTms);

    quint16 rmpTms() const;
    QModbusReply *setRmpTms(quint16 rmpTms);

    quint16 actIndx() const;

    void processBlockData(const QVector<quint16> blockData) override;

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

    explicit SunSpecScheduleModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, QObject *parent = nullptr);
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
    quint16 m_fixedBlockLength = 6;
    quint16 m_repeatingBlockLength = 60;

    void initDataPoints() override;
    void processBlockData() override;

private:
    ActschdFlags m_actSchd;
    ModenaFlags m_modEna;
    quint16 m_nSchd = 0;
    quint16 m_nPts = 0;
    quint16 m_pad = 0;


};

QDebug operator<<(QDebug debug, SunSpecScheduleModel *model);

#endif // SUNSPECSCHEDULEMODEL_H
