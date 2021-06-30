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

#include "sunspec.h"
#include "sunspecmodel.h"

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

    explicit SunSpecControlsModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecControlsModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    quint16 modelId() const;
    quint16 modelLength() const;
    quint16 connWinTms() const;
    quint16 connRvrtTms() const;
    Conn conn() const;
    float wMaxLimPct() const;
    quint16 wMaxLimPctWinTms() const;
    quint16 wMaxLimPctRvrtTms() const;
    quint16 wMaxLimPctRmpTms() const;
    Wmaxlim_ena wMaxLimEna() const;
    qint16 outPfSet() const;
    quint16 outPfSetWinTms() const;
    quint16 outPfSetRvrtTms() const;
    quint16 outPfSetRmpTms() const;
    Outpfset_ena outPfSetEna() const;
    qint16 vArWMaxPct() const;
    qint16 vArMaxPct() const;
    qint16 vArAvalPct() const;
    quint16 vArPctWinTms() const;
    quint16 vArPctRvrtTms() const;
    quint16 vArPctRmpTms() const;
    Varpct_mod vArPctMod() const;
    Varpct_ena vArPctEna() const;

private:
    quint16 m_modelId;
    quint16 m_modelLength;
    quint16 m_connWinTms;
    quint16 m_connRvrtTms;
    Conn m_conn;
    float m_wMaxLimPct;
    quint16 m_wMaxLimPctWinTms;
    quint16 m_wMaxLimPctRvrtTms;
    quint16 m_wMaxLimPctRmpTms;
    Wmaxlim_ena m_wMaxLimEna;
    qint16 m_outPfSet;
    quint16 m_outPfSetWinTms;
    quint16 m_outPfSetRvrtTms;
    quint16 m_outPfSetRmpTms;
    Outpfset_ena m_outPfSetEna;
    qint16 m_vArWMaxPct;
    qint16 m_vArMaxPct;
    qint16 m_vArAvalPct;
    quint16 m_vArPctWinTms;
    quint16 m_vArPctRvrtTms;
    quint16 m_vArPctRmpTms;
    Varpct_mod m_vArPctMod;
    Varpct_ena m_vArPctEna;
    qint16 m_wMaxLimPctSf;
    qint16 m_outPfSetSf;
    qint16 m_vArPctSf;

    void initDataPoints();

};

#endif // SUNSPECCONTROLSMODEL_H
