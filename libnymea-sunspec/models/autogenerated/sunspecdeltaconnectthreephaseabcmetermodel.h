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

#ifndef SUNSPECDELTACONNECTTHREEPHASEABCMETERMODEL_H
#define SUNSPECDELTACONNECTTHREEPHASEABCMETERMODEL_H

#include <QObject>

#include "sunspec.h"
#include "sunspecmodel.h"

class SunSpecDeltaConnectThreePhaseAbcMeterModel : public SunSpecModel
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

    explicit SunSpecDeltaConnectThreePhaseAbcMeterModel(SunSpec *connection, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecDeltaConnectThreePhaseAbcMeterModel() override; 

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
    float totalWattHoursExported() const;
    float totalWattHoursExportedPhaseA() const;
    float totalWattHoursExportedPhaseB() const;
    float totalWattHoursExportedPhaseC() const;
    float totalWattHoursImported() const;
    float totalWattHoursImportedPhaseA() const;
    float totalWattHoursImportedPhaseB() const;
    float totalWattHoursImportedPhaseC() const;
    float totalVaHoursExported() const;
    float totalVaHoursExportedPhaseA() const;
    float totalVaHoursExportedPhaseB() const;
    float totalVaHoursExportedPhaseC() const;
    float totalVaHoursImported() const;
    float totalVaHoursImportedPhaseA() const;
    float totalVaHoursImportedPhaseB() const;
    float totalVaHoursImportedPhaseC() const;
    float totalVarHoursImportedQ1() const;
    float totalVArHoursImportedQ1PhaseA() const;
    float totalVArHoursImportedQ1PhaseB() const;
    float totalVArHoursImportedQ1PhaseC() const;
    float totalVArHoursImportedQ2() const;
    float totalVArHoursImportedQ2PhaseA() const;
    float totalVArHoursImportedQ2PhaseB() const;
    float totalVArHoursImportedQ2PhaseC() const;
    float totalVArHoursExportedQ3() const;
    float totalVArHoursExportedQ3PhaseA() const;
    float totalVArHoursExportedQ3PhaseB() const;
    float totalVArHoursExportedQ3PhaseC() const;
    float totalVArHoursExportedQ4() const;
    float totalVArHoursExportedQ4ImportedPhaseA() const;
    float totalVArHoursExportedQ4ImportedPhaseB() const;
    float totalVArHoursExportedQ4ImportedPhaseC() const;
    EvtFlags events() const;

protected:
    void processBlockData() override;

private:
    float m_amps;
    float m_ampsPhaseA;
    float m_ampsPhaseB;
    float m_ampsPhaseC;
    float m_voltageLn;
    float m_phaseVoltageAn;
    float m_phaseVoltageBn;
    float m_phaseVoltageCn;
    float m_voltageLl;
    float m_phaseVoltageAb;
    float m_phaseVoltageBc;
    float m_phaseVoltageCa;
    float m_hz;
    float m_watts;
    float m_wattsPhaseA;
    float m_wattsPhaseB;
    float m_wattsPhaseC;
    float m_va;
    float m_vaPhaseA;
    float m_vaPhaseB;
    float m_vaPhaseC;
    float m_var;
    float m_varPhaseA;
    float m_varPhaseB;
    float m_varPhaseC;
    float m_pf;
    float m_pfPhaseA;
    float m_pfPhaseB;
    float m_pfPhaseC;
    float m_totalWattHoursExported;
    float m_totalWattHoursExportedPhaseA;
    float m_totalWattHoursExportedPhaseB;
    float m_totalWattHoursExportedPhaseC;
    float m_totalWattHoursImported;
    float m_totalWattHoursImportedPhaseA;
    float m_totalWattHoursImportedPhaseB;
    float m_totalWattHoursImportedPhaseC;
    float m_totalVaHoursExported;
    float m_totalVaHoursExportedPhaseA;
    float m_totalVaHoursExportedPhaseB;
    float m_totalVaHoursExportedPhaseC;
    float m_totalVaHoursImported;
    float m_totalVaHoursImportedPhaseA;
    float m_totalVaHoursImportedPhaseB;
    float m_totalVaHoursImportedPhaseC;
    float m_totalVarHoursImportedQ1;
    float m_totalVArHoursImportedQ1PhaseA;
    float m_totalVArHoursImportedQ1PhaseB;
    float m_totalVArHoursImportedQ1PhaseC;
    float m_totalVArHoursImportedQ2;
    float m_totalVArHoursImportedQ2PhaseA;
    float m_totalVArHoursImportedQ2PhaseB;
    float m_totalVArHoursImportedQ2PhaseC;
    float m_totalVArHoursExportedQ3;
    float m_totalVArHoursExportedQ3PhaseA;
    float m_totalVArHoursExportedQ3PhaseB;
    float m_totalVArHoursExportedQ3PhaseC;
    float m_totalVArHoursExportedQ4;
    float m_totalVArHoursExportedQ4ImportedPhaseA;
    float m_totalVArHoursExportedQ4ImportedPhaseB;
    float m_totalVArHoursExportedQ4ImportedPhaseC;
    EvtFlags m_events;

    void initDataPoints();

};

#endif // SUNSPECDELTACONNECTTHREEPHASEABCMETERMODEL_H
