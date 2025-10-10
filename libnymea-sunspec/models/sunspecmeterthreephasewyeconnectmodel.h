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

#ifndef SUNSPECMETERTHREEPHASEWYECONNECTMODEL_H
#define SUNSPECMETERTHREEPHASEWYECONNECTMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecMeterThreePhaseWyeConnectModel : public SunSpecModel
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

    explicit SunSpecMeterThreePhaseWyeConnectModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecMeterThreePhaseWyeConnectModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    /* Total AC Current [A] */
    float amps() const;
    /* Phase A Current [A] */
    float ampsPhaseA() const;
    /* Phase B Current [A] */
    float ampsPhaseB() const;
    /* Phase C Current [A] */
    float ampsPhaseC() const;
    /* Line to Neutral AC Voltage (average of active phases) [V] */
    float voltageLn() const;
    /* Phase Voltage AN [V] */
    float phaseVoltageAn() const;
    /* Phase Voltage BN [V] */
    float phaseVoltageBn() const;
    /* Phase Voltage CN [V] */
    float phaseVoltageCn() const;
    /* Line to Line AC Voltage (average of active phases) [V] */
    float voltageLl() const;
    /* Phase Voltage AB [V] */
    float phaseVoltageAb() const;
    /* Phase Voltage BC [V] */
    float phaseVoltageBc() const;
    /* Phase Voltage CA [V] */
    float phaseVoltageCa() const;
    /* Frequency [Hz] */
    float hz() const;
    /* Total Real Power [W] */
    float watts() const;
    /* Watts phase A [W] */
    float wattsPhaseA() const;
    /* Watts phase B [W] */
    float wattsPhaseB() const;
    /* Watts phase C [W] */
    float wattsPhaseC() const;
    /* AC Apparent Power [VA] */
    float va() const;
    /* VA phase A [VA] */
    float vaPhaseA() const;
    /* VA phase B [VA] */
    float vaPhaseB() const;
    /* VA phase C [VA] */
    float vaPhaseC() const;
    /* Reactive Power [var] */
    float var() const;
    /* VAR phase A [var] */
    float varPhaseA() const;
    /* VAR phase B [var] */
    float varPhaseB() const;
    /* VAR phase C [var] */
    float varPhaseC() const;
    /* Power Factor [PF] */
    float pf() const;
    /* PF phase A [PF] */
    float pfPhaseA() const;
    /* PF phase B [PF] */
    float pfPhaseB() const;
    /* PF phase C [PF] */
    float pfPhaseC() const;
    /* Total Real Energy Exported [Wh] */
    float totalWattHoursExported() const;
    /* Total Watt-hours Exported phase A [Wh] */
    float totalWattHoursExportedPhaseA() const;
    /* Total Watt-hours Exported phase B [Wh] */
    float totalWattHoursExportedPhaseB() const;
    /* Total Watt-hours Exported phase C [Wh] */
    float totalWattHoursExportedPhaseC() const;
    /* Total Real Energy Imported [Wh] */
    float totalWattHoursImported() const;
    /* Total Watt-hours Imported phase A [Wh] */
    float totalWattHoursImportedPhaseA() const;
    /* Total Watt-hours Imported phase B [Wh] */
    float totalWattHoursImportedPhaseB() const;
    /* Total Watt-hours Imported phase C [Wh] */
    float totalWattHoursImportedPhaseC() const;
    /* Total Apparent Energy Exported [VAh] */
    float totalVaHoursExported() const;
    /* Total VA-hours Exported phase A [VAh] */
    float totalVaHoursExportedPhaseA() const;
    /* Total VA-hours Exported phase B [VAh] */
    float totalVaHoursExportedPhaseB() const;
    /* Total VA-hours Exported phase C [VAh] */
    float totalVaHoursExportedPhaseC() const;
    /* Total Apparent Energy Imported [VAh] */
    float totalVaHoursImported() const;
    /* Total VA-hours Imported phase A [VAh] */
    float totalVaHoursImportedPhaseA() const;
    /* Total VA-hours Imported phase B [VAh] */
    float totalVaHoursImportedPhaseB() const;
    /* Total VA-hours Imported phase C [VAh] */
    float totalVaHoursImportedPhaseC() const;
    /* Total Reactive Energy Imported Quadrant 1 [varh] */
    float totalVarHoursImportedQ1() const;
    /* Total VAr-hours Imported Q1 phase A [varh] */
    float totalVArHoursImportedQ1PhaseA() const;
    /* Total VAr-hours Imported Q1 phase B [varh] */
    float totalVArHoursImportedQ1PhaseB() const;
    /* Total VAr-hours Imported Q1 phase C [varh] */
    float totalVArHoursImportedQ1PhaseC() const;
    /* Total Reactive Power Imported Quadrant 2 [varh] */
    float totalVArHoursImportedQ2() const;
    /* Total VAr-hours Imported Q2 phase A [varh] */
    float totalVArHoursImportedQ2PhaseA() const;
    /* Total VAr-hours Imported Q2 phase B [varh] */
    float totalVArHoursImportedQ2PhaseB() const;
    /* Total VAr-hours Imported Q2 phase C [varh] */
    float totalVArHoursImportedQ2PhaseC() const;
    /* Total Reactive Power Exported Quadrant 3 [varh] */
    float totalVArHoursExportedQ3() const;
    /* Total VAr-hours Exported Q3 phase A [varh] */
    float totalVArHoursExportedQ3PhaseA() const;
    /* Total VAr-hours Exported Q3 phase B [varh] */
    float totalVArHoursExportedQ3PhaseB() const;
    /* Total VAr-hours Exported Q3 phase C [varh] */
    float totalVArHoursExportedQ3PhaseC() const;
    /* Total Reactive Power Exported Quadrant 4 [varh] */
    float totalVArHoursExportedQ4() const;
    /* Total VAr-hours Exported Q4 Imported phase A [varh] */
    float totalVArHoursExportedQ4ImportedPhaseA() const;
    /* Total VAr-hours Exported Q4 Imported phase B [varh] */
    float totalVArHoursExportedQ4ImportedPhaseB() const;
    /* Total VAr-hours Exported Q4 Imported phase C [varh] */
    float totalVArHoursExportedQ4ImportedPhaseC() const;
    /* Meter Event Flags */
    EvtFlags events() const;

protected:
    quint16 m_fixedBlockLength = 124;

    void initDataPoints();
    void processBlockData() override;

private:
    float m_amps = 0;
    float m_ampsPhaseA = 0;
    float m_ampsPhaseB = 0;
    float m_ampsPhaseC = 0;
    float m_voltageLn = 0;
    float m_phaseVoltageAn = 0;
    float m_phaseVoltageBn = 0;
    float m_phaseVoltageCn = 0;
    float m_voltageLl = 0;
    float m_phaseVoltageAb = 0;
    float m_phaseVoltageBc = 0;
    float m_phaseVoltageCa = 0;
    float m_hz = 0;
    float m_watts = 0;
    float m_wattsPhaseA = 0;
    float m_wattsPhaseB = 0;
    float m_wattsPhaseC = 0;
    float m_va = 0;
    float m_vaPhaseA = 0;
    float m_vaPhaseB = 0;
    float m_vaPhaseC = 0;
    float m_var = 0;
    float m_varPhaseA = 0;
    float m_varPhaseB = 0;
    float m_varPhaseC = 0;
    float m_pf = 0;
    float m_pfPhaseA = 0;
    float m_pfPhaseB = 0;
    float m_pfPhaseC = 0;
    float m_totalWattHoursExported = 0;
    float m_totalWattHoursExportedPhaseA = 0;
    float m_totalWattHoursExportedPhaseB = 0;
    float m_totalWattHoursExportedPhaseC = 0;
    float m_totalWattHoursImported = 0;
    float m_totalWattHoursImportedPhaseA = 0;
    float m_totalWattHoursImportedPhaseB = 0;
    float m_totalWattHoursImportedPhaseC = 0;
    float m_totalVaHoursExported = 0;
    float m_totalVaHoursExportedPhaseA = 0;
    float m_totalVaHoursExportedPhaseB = 0;
    float m_totalVaHoursExportedPhaseC = 0;
    float m_totalVaHoursImported = 0;
    float m_totalVaHoursImportedPhaseA = 0;
    float m_totalVaHoursImportedPhaseB = 0;
    float m_totalVaHoursImportedPhaseC = 0;
    float m_totalVarHoursImportedQ1 = 0;
    float m_totalVArHoursImportedQ1PhaseA = 0;
    float m_totalVArHoursImportedQ1PhaseB = 0;
    float m_totalVArHoursImportedQ1PhaseC = 0;
    float m_totalVArHoursImportedQ2 = 0;
    float m_totalVArHoursImportedQ2PhaseA = 0;
    float m_totalVArHoursImportedQ2PhaseB = 0;
    float m_totalVArHoursImportedQ2PhaseC = 0;
    float m_totalVArHoursExportedQ3 = 0;
    float m_totalVArHoursExportedQ3PhaseA = 0;
    float m_totalVArHoursExportedQ3PhaseB = 0;
    float m_totalVArHoursExportedQ3PhaseC = 0;
    float m_totalVArHoursExportedQ4 = 0;
    float m_totalVArHoursExportedQ4ImportedPhaseA = 0;
    float m_totalVArHoursExportedQ4ImportedPhaseB = 0;
    float m_totalVArHoursExportedQ4ImportedPhaseC = 0;
    EvtFlags m_events;


};

QDebug operator<<(QDebug debug, SunSpecMeterThreePhaseWyeConnectModel *model);

#endif // SUNSPECMETERTHREEPHASEWYECONNECTMODEL_H
