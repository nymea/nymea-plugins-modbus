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

#ifndef SUNSPECSTRINGCOMBINERADVANCEDMODEL_H
#define SUNSPECSTRINGCOMBINERADVANCEDMODEL_H

#include <QObject>

#include "sunspec.h"
#include "sunspecmodel.h"

class SunSpecStringCombinerAdvancedModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Evt {
        EvtLowVoltage = 0x1,
        EvtLowPower = 0x2,
        EvtLowEfficiency = 0x4,
        EvtCurrent = 0x8,
        EvtVoltage = 0x10,
        EvtPower = 0x20,
        EvtPr = 0x40,
        EvtDisconnected = 0x80,
        EvtFuseFault = 0x100,
        EvtCombinerFuseFault = 0x200,
        EvtCombinerCabinetOpen = 0x400,
        EvtTemp = 0x800,
        EvtGroundfault = 0x1000,
        EvtReversedPolarity = 0x2000,
        EvtIncompatible = 0x4000,
        EvtCommError = 0x8000,
        EvtInternalError = 0x10000,
        EvtTheft = 0x20000,
        EvtArcDetected = 0x40000
    };
    Q_DECLARE_FLAGS(EvtFlags, Evt)
    Q_FLAG(Evt)

    explicit SunSpecStringCombinerAdvancedModel(SunSpec *connection, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecStringCombinerAdvancedModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    float rating() const;
    int n() const;
    EvtFlags event() const;
    quint32 vendorEvent() const;
    float amps() const;
    quint32 ampHours() const;
    float voltage() const;
    qint16 temp() const;
    float watts() const;
    qint16 pr() const;
    quint32 wattHours() const;

protected:
    void processBlockData() override;

private:
    qint16 m_dCA_SF;
    qint16 m_dCAhr_SF;
    qint16 m_dCV_SF;
    qint16 m_dCW_SF;
    qint16 m_dCWh_SF;
    float m_rating;
    int m_n;
    EvtFlags m_event;
    quint32 m_vendorEvent;
    float m_amps;
    quint32 m_ampHours;
    float m_voltage;
    qint16 m_temp;
    float m_watts;
    qint16 m_pr;
    quint32 m_wattHours;
    qint16 m_inDCA_SF;
    qint16 m_inDCAhr_SF;
    qint16 m_inDCV_SF;
    qint16 m_inDCW_SF;
    qint16 m_inDCWh_SF;

    void initDataPoints();

};

#endif // SUNSPECSTRINGCOMBINERADVANCEDMODEL_H
