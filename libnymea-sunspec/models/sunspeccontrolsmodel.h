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

    explicit SunSpecControlsModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, QObject *parent = nullptr);
    ~SunSpecControlsModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;


    quint16 connWinTms() const;
    QModbusReply *setConnWinTms(quint16 connWinTms);

    quint16 connRvrtTms() const;
    QModbusReply *setConnRvrtTms(quint16 connRvrtTms);

    Conn conn() const;
    QModbusReply *setConn(Conn conn);

    float wMaxLimPct() const;
    QModbusReply *setWMaxLimPct(float wMaxLimPct);

    quint16 wMaxLimPctWinTms() const;
    QModbusReply *setWMaxLimPctWinTms(quint16 wMaxLimPctWinTms);

    quint16 wMaxLimPctRvrtTms() const;
    QModbusReply *setWMaxLimPctRvrtTms(quint16 wMaxLimPctRvrtTms);

    quint16 wMaxLimPctRmpTms() const;
    QModbusReply *setWMaxLimPctRmpTms(quint16 wMaxLimPctRmpTms);

    Wmaxlim_ena wMaxLimEna() const;
    QModbusReply *setWMaxLimEna(Wmaxlim_ena wMaxLimEna);

    float outPfSet() const;
    QModbusReply *setOutPfSet(float outPfSet);

    quint16 outPfSetWinTms() const;
    QModbusReply *setOutPfSetWinTms(quint16 outPfSetWinTms);

    quint16 outPfSetRvrtTms() const;
    QModbusReply *setOutPfSetRvrtTms(quint16 outPfSetRvrtTms);

    quint16 outPfSetRmpTms() const;
    QModbusReply *setOutPfSetRmpTms(quint16 outPfSetRmpTms);

    Outpfset_ena outPfSetEna() const;
    QModbusReply *setOutPfSetEna(Outpfset_ena outPfSetEna);

    float vArWMaxPct() const;
    QModbusReply *setVArWMaxPct(float vArWMaxPct);

    float vArMaxPct() const;
    QModbusReply *setVArMaxPct(float vArMaxPct);

    float vArAvalPct() const;
    QModbusReply *setVArAvalPct(float vArAvalPct);

    quint16 vArPctWinTms() const;
    QModbusReply *setVArPctWinTms(quint16 vArPctWinTms);

    quint16 vArPctRvrtTms() const;
    QModbusReply *setVArPctRvrtTms(quint16 vArPctRvrtTms);

    quint16 vArPctRmpTms() const;
    QModbusReply *setVArPctRmpTms(quint16 vArPctRmpTms);

    Varpct_mod vArPctMod() const;
    QModbusReply *setVArPctMod(Varpct_mod vArPctMod);

    Varpct_ena vArPctEna() const;
    QModbusReply *setVArPctEna(Varpct_ena vArPctEna);

    qint16 wMaxLimPctSf() const;
    qint16 outPfSetSf() const;
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
