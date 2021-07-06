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

#ifndef SUNSPECMETERTHREEPHASEDELTACONNECTMODEL_H
#define SUNSPECMETERTHREEPHASEDELTACONNECTMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecMeterThreePhaseDeltaConnectModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Evt {
        EvtMEventPowerFailure = 0x4,
        EvtMEventUnderVoltage = 0x8,
        EvtMEventLowPf = 0x10,
        EvtMEventOverCurrent = 0x20,
        EvtMEventOverVoltage = 0x40,
        EvtMEventMissingSensor = 0x80,
        EvtMEventReserved1 = 0x100,
        EvtMEventReserved2 = 0x200,
        EvtMEventReserved3 = 0x400,
        EvtMEventReserved4 = 0x800,
        EvtMEventReserved5 = 0x1000,
        EvtMEventReserved6 = 0x2000,
        EvtMEventReserved7 = 0x4000,
        EvtMEventReserved8 = 0x8000,
        EvtMEventOem01 = 0x10000,
        EvtMEventOem02 = 0x20000,
        EvtMEventOem03 = 0x40000,
        EvtMEventOem04 = 0x80000,
        EvtMEventOem05 = 0x100000,
        EvtMEventOem06 = 0x200000,
        EvtMEventOem07 = 0x400000,
        EvtMEventOem08 = 0x800000,
        EvtMEventOem09 = 0x1000000,
        EvtMEventOem10 = 0x2000000,
        EvtMEventOem11 = 0x4000000,
        EvtMEventOem12 = 0x8000000,
        EvtMEventOem13 = 0x10000000,
        EvtMEventOem14 = 0x20000000,
        EvtMEventOem15 = 0x40000000
    };
    Q_DECLARE_FLAGS(EvtFlags, Evt)
    Q_FLAG(Evt)

    explicit SunSpecMeterThreePhaseDeltaConnectModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent = nullptr);
    ~SunSpecMeterThreePhaseDeltaConnectModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    float amps() const;
    float ampsPhaseA() const;
    float ampsPhaseB() const;
    float ampsPhaseC() const;
    float voltageLn() const;
    float phaseVoltageAn() const;
    float phaseVoltageBn() const;
    float phaseVoltageCn() const;
    float voltageLl() const;
    float phaseVoltageAb() const;
    float phaseVoltageBc() const;
    float phaseVoltageCa() const;
    float hz() const;
    float watts() const;
    float wattsPhaseA() const;
    float wattsPhaseB() const;
    float wattsPhaseC() const;
    float va() const;
    float vaPhaseA() const;
    float vaPhaseB() const;
    float vaPhaseC() const;
    float var() const;
    float varPhaseA() const;
    float varPhaseB() const;
    float varPhaseC() const;
    float pf() const;
    float pfPhaseA() const;
    float pfPhaseB() const;
    float pfPhaseC() const;
    quint32 totalWattHoursExported() const;
    quint32 totalWattHoursExportedPhaseA() const;
    quint32 totalWattHoursExportedPhaseB() const;
    quint32 totalWattHoursExportedPhaseC() const;
    quint32 totalWattHoursImported() const;
    quint32 totalWattHoursImportedPhaseA() const;
    quint32 totalWattHoursImportedPhaseB() const;
    quint32 totalWattHoursImportedPhaseC() const;
    quint32 totalVaHoursExported() const;
    quint32 totalVaHoursExportedPhaseA() const;
    quint32 totalVaHoursExportedPhaseB() const;
    quint32 totalVaHoursExportedPhaseC() const;
    quint32 totalVaHoursImported() const;
    quint32 totalVaHoursImportedPhaseA() const;
    quint32 totalVaHoursImportedPhaseB() const;
    quint32 totalVaHoursImportedPhaseC() const;
    quint32 totalVarHoursImportedQ1() const;
    quint32 totalVArHoursImportedQ1PhaseA() const;
    quint32 totalVArHoursImportedQ1PhaseB() const;
    quint32 totalVArHoursImportedQ1PhaseC() const;
    quint32 totalVArHoursImportedQ2() const;
    quint32 totalVArHoursImportedQ2PhaseA() const;
    quint32 totalVArHoursImportedQ2PhaseB() const;
    quint32 totalVArHoursImportedQ2PhaseC() const;
    quint32 totalVArHoursExportedQ3() const;
    quint32 totalVArHoursExportedQ3PhaseA() const;
    quint32 totalVArHoursExportedQ3PhaseB() const;
    quint32 totalVArHoursExportedQ3PhaseC() const;
    quint32 totalVArHoursExportedQ4() const;
    quint32 totalVArHoursExportedQ4ImportedPhaseA() const;
    quint32 totalVArHoursExportedQ4ImportedPhaseB() const;
    quint32 totalVArHoursExportedQ4ImportedPhaseC() const;
    EvtFlags events() const;

protected:
    void processBlockData() override;

private:
    float m_amps = 0;
    float m_ampsPhaseA = 0;
    float m_ampsPhaseB = 0;
    float m_ampsPhaseC = 0;
    qint16 m_a_SF = 0;
    float m_voltageLn = 0;
    float m_phaseVoltageAn = 0;
    float m_phaseVoltageBn = 0;
    float m_phaseVoltageCn = 0;
    float m_voltageLl = 0;
    float m_phaseVoltageAb = 0;
    float m_phaseVoltageBc = 0;
    float m_phaseVoltageCa = 0;
    qint16 m_v_SF = 0;
    float m_hz = 0;
    qint16 m_hz_SF = 0;
    float m_watts = 0;
    float m_wattsPhaseA = 0;
    float m_wattsPhaseB = 0;
    float m_wattsPhaseC = 0;
    qint16 m_w_SF = 0;
    float m_va = 0;
    float m_vaPhaseA = 0;
    float m_vaPhaseB = 0;
    float m_vaPhaseC = 0;
    qint16 m_vA_SF = 0;
    float m_var = 0;
    float m_varPhaseA = 0;
    float m_varPhaseB = 0;
    float m_varPhaseC = 0;
    qint16 m_vAR_SF = 0;
    float m_pf = 0;
    float m_pfPhaseA = 0;
    float m_pfPhaseB = 0;
    float m_pfPhaseC = 0;
    qint16 m_pF_SF = 0;
    quint32 m_totalWattHoursExported = 0;
    quint32 m_totalWattHoursExportedPhaseA = 0;
    quint32 m_totalWattHoursExportedPhaseB = 0;
    quint32 m_totalWattHoursExportedPhaseC = 0;
    quint32 m_totalWattHoursImported = 0;
    quint32 m_totalWattHoursImportedPhaseA = 0;
    quint32 m_totalWattHoursImportedPhaseB = 0;
    quint32 m_totalWattHoursImportedPhaseC = 0;
    qint16 m_totWh_SF = 0;
    quint32 m_totalVaHoursExported = 0;
    quint32 m_totalVaHoursExportedPhaseA = 0;
    quint32 m_totalVaHoursExportedPhaseB = 0;
    quint32 m_totalVaHoursExportedPhaseC = 0;
    quint32 m_totalVaHoursImported = 0;
    quint32 m_totalVaHoursImportedPhaseA = 0;
    quint32 m_totalVaHoursImportedPhaseB = 0;
    quint32 m_totalVaHoursImportedPhaseC = 0;
    qint16 m_totVAh_SF = 0;
    quint32 m_totalVarHoursImportedQ1 = 0;
    quint32 m_totalVArHoursImportedQ1PhaseA = 0;
    quint32 m_totalVArHoursImportedQ1PhaseB = 0;
    quint32 m_totalVArHoursImportedQ1PhaseC = 0;
    quint32 m_totalVArHoursImportedQ2 = 0;
    quint32 m_totalVArHoursImportedQ2PhaseA = 0;
    quint32 m_totalVArHoursImportedQ2PhaseB = 0;
    quint32 m_totalVArHoursImportedQ2PhaseC = 0;
    quint32 m_totalVArHoursExportedQ3 = 0;
    quint32 m_totalVArHoursExportedQ3PhaseA = 0;
    quint32 m_totalVArHoursExportedQ3PhaseB = 0;
    quint32 m_totalVArHoursExportedQ3PhaseC = 0;
    quint32 m_totalVArHoursExportedQ4 = 0;
    quint32 m_totalVArHoursExportedQ4ImportedPhaseA = 0;
    quint32 m_totalVArHoursExportedQ4ImportedPhaseB = 0;
    quint32 m_totalVArHoursExportedQ4ImportedPhaseC = 0;
    qint16 m_totVArh_SF = 0;
    EvtFlags m_events;

    void initDataPoints();

};

QDebug operator<<(QDebug debug, SunSpecMeterThreePhaseDeltaConnectModel *model);

#endif // SUNSPECMETERTHREEPHASEDELTACONNECTMODEL_H
