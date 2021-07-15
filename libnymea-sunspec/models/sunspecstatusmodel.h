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

#ifndef SUNSPECSTATUSMODEL_H
#define SUNSPECSTATUSMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecStatusModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Pvconn {
        PvconnConnected = 0x1,
        PvconnAvailable = 0x2,
        PvconnOperating = 0x4,
        PvconnTest = 0x8
    };
    Q_DECLARE_FLAGS(PvconnFlags, Pvconn)
    Q_FLAG(Pvconn)

    enum Storconn {
        StorconnConnected = 0x1,
        StorconnAvailable = 0x2,
        StorconnOperating = 0x4,
        StorconnTest = 0x8
    };
    Q_DECLARE_FLAGS(StorconnFlags, Storconn)
    Q_FLAG(Storconn)

    enum Ecpconn {
        EcpconnConnected = 0x1
    };
    Q_DECLARE_FLAGS(EcpconnFlags, Ecpconn)
    Q_FLAG(Ecpconn)

    enum Stsetlimmsk {
        StsetlimmskWMax = 0x1,
        StsetlimmskVaMax = 0x2,
        StsetlimmskVArAval = 0x4,
        StsetlimmskVArMaxQ1 = 0x8,
        StsetlimmskVArMaxQ2 = 0x10,
        StsetlimmskVArMaxQ3 = 0x20,
        StsetlimmskVArMaxQ4 = 0x40,
        StsetlimmskPfMinQ1 = 0x80,
        StsetlimmskPfMinQ2 = 0x100,
        StsetlimmskPfMinQ3 = 0x200,
        StsetlimmskPfMinQ4 = 0x400
    };
    Q_DECLARE_FLAGS(StsetlimmskFlags, Stsetlimmsk)
    Q_FLAG(Stsetlimmsk)

    enum Stactctl {
        StactctlFixedW = 0x1,
        StactctlFixedVar = 0x2,
        StactctlFixedPf = 0x4,
        StactctlVoltVAr = 0x8,
        StactctlFreqWattParam = 0x10,
        StactctlFreqWattCurve = 0x20,
        StactctlDynReactiveCurrent = 0x40,
        StactctlLvrt = 0x80,
        StactctlHvrt = 0x100,
        StactctlWattPf = 0x200,
        StactctlVoltWatt = 0x400,
        StactctlScheduled = 0x1000,
        StactctlLfrt = 0x2000,
        StactctlHfrt = 0x4000
    };
    Q_DECLARE_FLAGS(StactctlFlags, Stactctl)
    Q_FLAG(Stactctl)

    enum Rtst {
        RtstLvrtActive = 0x1,
        RtstHvrtActive = 0x2,
        RtstLfrtActive = 0x4,
        RtstHfrtActive = 0x8
    };
    Q_DECLARE_FLAGS(RtstFlags, Rtst)
    Q_FLAG(Rtst)

    explicit SunSpecStatusModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, QObject *parent = nullptr);
    ~SunSpecStatusModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    PvconnFlags pvConn() const;
    StorconnFlags storConn() const;
    EcpconnFlags ecpConn() const;
    quint64 actWh() const;
    quint64 actVAh() const;
    quint64 actVArhQ1() const;
    quint64 actVArhQ2() const;
    quint64 actVArhQ3() const;
    quint64 actVArhQ4() const;
    float vArAval() const;
    qint16 vArAvalSf() const;
    float wAval() const;
    qint16 wAvalSf() const;
    StsetlimmskFlags stSetLimMsk() const;
    StactctlFlags stActCtl() const;
    QString tmSrc() const;
    quint32 tms() const;
    RtstFlags rtSt() const;
    float ris() const;
    qint16 risSf() const;

protected:
    quint16 m_fixedBlockLength = 44;

    void initDataPoints() override;
    void processBlockData() override;

private:
    PvconnFlags m_pvConn;
    StorconnFlags m_storConn;
    EcpconnFlags m_ecpConn;
    quint64 m_actWh = 0;
    quint64 m_actVAh = 0;
    quint64 m_actVArhQ1 = 0;
    quint64 m_actVArhQ2 = 0;
    quint64 m_actVArhQ3 = 0;
    quint64 m_actVArhQ4 = 0;
    float m_vArAval = 0;
    qint16 m_vArAvalSf = 0;
    float m_wAval = 0;
    qint16 m_wAvalSf = 0;
    StsetlimmskFlags m_stSetLimMsk;
    StactctlFlags m_stActCtl;
    QString m_tmSrc;
    quint32 m_tms = 0;
    RtstFlags m_rtSt;
    float m_ris = 0;
    qint16 m_risSf = 0;


};

QDebug operator<<(QDebug debug, SunSpecStatusModel *model);

#endif // SUNSPECSTATUSMODEL_H
