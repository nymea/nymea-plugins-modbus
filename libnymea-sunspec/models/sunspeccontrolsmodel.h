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

#ifndef SUNSPECCONTROLSMODEL_H
#define SUNSPECCONTROLSMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecControlsModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Conn {
        ConnDisconnect = 0,
        ConnConnect = 1
    };
    Q_ENUM(Conn)

    enum Wmaxlim_ena {
        Wmaxlim_enaDisabled = 0,
        Wmaxlim_enaEnabled = 1
    };
    Q_ENUM(Wmaxlim_ena)

    enum Outpfset_ena {
        Outpfset_enaDisabled = 0,
        Outpfset_enaEnabled = 1
    };
    Q_ENUM(Outpfset_ena)

    enum Varpct_mod {
        Varpct_modNone = 0,
        Varpct_modWMax = 1,
        Varpct_modVArMax = 2,
        Varpct_modVArAval = 3
    };
    Q_ENUM(Varpct_mod)

    enum Varpct_ena {
        Varpct_enaDisabled = 0,
        Varpct_enaEnabled = 1
    };
    Q_ENUM(Varpct_ena)

    explicit SunSpecControlsModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecControlsModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;


    /* Time window for connect/disconnect. [Secs] */
    quint16 connWinTms() const;
    QModbusReply *setConnWinTms(quint16 connWinTms);

    /* Timeout period for connect/disconnect. [Secs] */
    quint16 connRvrtTms() const;
    QModbusReply *setConnRvrtTms(quint16 connRvrtTms);

    /* Enumerated valued.  Connection control. */
    Conn conn() const;
    QModbusReply *setConn(Conn conn);

    /* Set power output to specified level. [% WMax] */
    float wMaxLimPct() const;
    QModbusReply *setWMaxLimPct(float wMaxLimPct);

    /* Time window for power limit change. [Secs] */
    quint16 wMaxLimPctWinTms() const;
    QModbusReply *setWMaxLimPctWinTms(quint16 wMaxLimPctWinTms);

    /* Timeout period for power limit. [Secs] */
    quint16 wMaxLimPctRvrtTms() const;
    QModbusReply *setWMaxLimPctRvrtTms(quint16 wMaxLimPctRvrtTms);

    /* Ramp time for moving from current setpoint to new setpoint. [Secs] */
    quint16 wMaxLimPctRmpTms() const;
    QModbusReply *setWMaxLimPctRmpTms(quint16 wMaxLimPctRmpTms);

    /* Enumerated valued.  Throttle enable/disable control. */
    Wmaxlim_ena wMaxLimEna() const;
    QModbusReply *setWMaxLimEna(Wmaxlim_ena wMaxLimEna);

    /* Set power factor to specific value - cosine of angle. [cos()] */
    float outPfSet() const;
    QModbusReply *setOutPfSet(float outPfSet);

    /* Time window for power factor change. [Secs] */
    quint16 outPfSetWinTms() const;
    QModbusReply *setOutPfSetWinTms(quint16 outPfSetWinTms);

    /* Timeout period for power factor. [Secs] */
    quint16 outPfSetRvrtTms() const;
    QModbusReply *setOutPfSetRvrtTms(quint16 outPfSetRvrtTms);

    /* Ramp time for moving from current setpoint to new setpoint. [Secs] */
    quint16 outPfSetRmpTms() const;
    QModbusReply *setOutPfSetRmpTms(quint16 outPfSetRmpTms);

    /* Enumerated valued.  Fixed power factor enable/disable control. */
    Outpfset_ena outPfSetEna() const;
    QModbusReply *setOutPfSetEna(Outpfset_ena outPfSetEna);

    /* Reactive power in percent of WMax. [% WMax] */
    float vArWMaxPct() const;
    QModbusReply *setVArWMaxPct(float vArWMaxPct);

    /* Reactive power in percent of VArMax. [% VArMax] */
    float vArMaxPct() const;
    QModbusReply *setVArMaxPct(float vArMaxPct);

    /* Reactive power in percent of VArAval. [% VArAval] */
    float vArAvalPct() const;
    QModbusReply *setVArAvalPct(float vArAvalPct);

    /* Time window for VAR limit change. [Secs] */
    quint16 vArPctWinTms() const;
    QModbusReply *setVArPctWinTms(quint16 vArPctWinTms);

    /* Timeout period for VAR limit. [Secs] */
    quint16 vArPctRvrtTms() const;
    QModbusReply *setVArPctRvrtTms(quint16 vArPctRvrtTms);

    /* Ramp time for moving from current setpoint to new setpoint. [Secs] */
    quint16 vArPctRmpTms() const;
    QModbusReply *setVArPctRmpTms(quint16 vArPctRmpTms);

    /* Enumerated value. VAR percent limit mode. */
    Varpct_mod vArPctMod() const;
    QModbusReply *setVArPctMod(Varpct_mod vArPctMod);

    /* Enumerated valued.  Percent limit VAr enable/disable control. */
    Varpct_ena vArPctEna() const;
    QModbusReply *setVArPctEna(Varpct_ena vArPctEna);

    /* Scale factor for power output percent. */
    qint16 wMaxLimPctSf() const;
    /* Scale factor for power factor. */
    qint16 outPfSetSf() const;
    /* Scale factor for reactive power percent. */
    qint16 vArPctSf() const;

protected:
    quint16 m_fixedBlockLength = 24;

    void initDataPoints();
    void processBlockData() override;

private:
    quint16 m_connWinTms = 0;
    quint16 m_connRvrtTms = 0;
    Conn m_conn;
    float m_wMaxLimPct = 0;
    quint16 m_wMaxLimPctWinTms = 0;
    quint16 m_wMaxLimPctRvrtTms = 0;
    quint16 m_wMaxLimPctRmpTms = 0;
    Wmaxlim_ena m_wMaxLimEna;
    float m_outPfSet = 0;
    quint16 m_outPfSetWinTms = 0;
    quint16 m_outPfSetRvrtTms = 0;
    quint16 m_outPfSetRmpTms = 0;
    Outpfset_ena m_outPfSetEna;
    float m_vArWMaxPct = 0;
    float m_vArMaxPct = 0;
    float m_vArAvalPct = 0;
    quint16 m_vArPctWinTms = 0;
    quint16 m_vArPctRvrtTms = 0;
    quint16 m_vArPctRmpTms = 0;
    Varpct_mod m_vArPctMod;
    Varpct_ena m_vArPctEna;
    qint16 m_wMaxLimPctSf = 0;
    qint16 m_outPfSetSf = 0;
    qint16 m_vArPctSf = 0;


};

QDebug operator<<(QDebug debug, SunSpecControlsModel *model);

#endif // SUNSPECCONTROLSMODEL_H
