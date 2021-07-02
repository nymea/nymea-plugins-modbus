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

#ifndef SUNSPECSECUREACMETERSELECTEDREADINGSMODEL_H
#define SUNSPECSECUREACMETERSELECTEDREADINGSMODEL_H

#include <QObject>

#include "sunspec.h"
#include "sunspecmodel.h"

class SunSpecSecureAcMeterSelectedReadingsModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Alg {
        AlgNone = 0,
        AlgAesGmac64 = 1,
        AlgEcc256 = 2
    };
    Q_ENUM(Alg)

    enum Evt {
        EvtPowerFailure = 0x4,
        EvtUnderVoltage = 0x8,
        EvtLowPf = 0x10,
        EvtOverCurrent = 0x20,
        EvtOverVoltage = 0x40,
        EvtMissingSensor = 0x80,
        EvtOem01 = 0x10000,
        EvtOem02 = 0x20000,
        EvtOem03 = 0x40000,
        EvtOem04 = 0x80000,
        EvtOem05 = 0x100000,
        EvtOem06 = 0x200000,
        EvtOem07 = 0x400000,
        EvtOem08 = 0x800000,
        EvtOem09 = 0x1000000,
        EvtOem10 = 0x2000000,
        EvtOem11 = 0x4000000,
        EvtOem12 = 0x8000000,
        EvtOem13 = 0x10000000,
        EvtOem14 = 0x20000000,
        EvtOem15 = 0x40000000
    };
    Q_DECLARE_FLAGS(EvtFlags, Evt)
    Q_FLAG(Evt)

    explicit SunSpecSecureAcMeterSelectedReadingsModel(SunSpec *connection, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecSecureAcMeterSelectedReadingsModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    float amps() const;
    float voltage() const;
    float hz() const;
    float watts() const;
    float va() const;
    float var() const;
    float pf() const;
    quint32 totalWattHoursExported() const;
    quint32 totalWattHoursImported() const;
    quint32 totalVaHoursExported() const;
    quint32 totalVaHoursImported() const;
    quint32 totalVarHoursImportedQ1() const;
    quint32 totalVArHoursImportedQ2() const;
    quint32 totalVArHoursExportedQ3() const;
    quint32 totalVArHoursExportedQ4() const;
    EvtFlags events() const;
    quint16 rsrvd() const;
    quint32 timestamp() const;
    quint16 milliseconds() const;
    quint16 sequence() const;
    Alg algorithm() const;
    quint16 n() const;

protected:
    void processBlockData() override;

private:
    float m_amps;
    qint16 m_a_SF;
    float m_voltage;
    qint16 m_v_SF;
    float m_hz;
    qint16 m_hz_SF;
    float m_watts;
    qint16 m_w_SF;
    float m_va;
    qint16 m_vA_SF;
    float m_var;
    qint16 m_vAR_SF;
    float m_pf;
    qint16 m_pF_SF;
    quint32 m_totalWattHoursExported;
    quint32 m_totalWattHoursImported;
    qint16 m_totWh_SF;
    quint32 m_totalVaHoursExported;
    quint32 m_totalVaHoursImported;
    qint16 m_totVAh_SF;
    quint32 m_totalVarHoursImportedQ1;
    quint32 m_totalVArHoursImportedQ2;
    quint32 m_totalVArHoursExportedQ3;
    quint32 m_totalVArHoursExportedQ4;
    qint16 m_totVArh_SF;
    EvtFlags m_events;
    quint16 m_rsrvd;
    quint32 m_timestamp;
    quint16 m_milliseconds;
    quint16 m_sequence;
    Alg m_algorithm;
    quint16 m_n;

    void initDataPoints();

};

#endif // SUNSPECSECUREACMETERSELECTEDREADINGSMODEL_H
