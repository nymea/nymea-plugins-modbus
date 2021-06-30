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

#ifndef SUNSPECACMETERMODEL_H
#define SUNSPECACMETERMODEL_H

#include <QObject>

#include "sunspec.h"
#include "sunspecmodel.h"

class SunSpecAcMeterModel : public SunSpecModel
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

    explicit SunSpecAcMeterModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecAcMeterModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    quint16 modelId() const;
    quint16 modelLength() const;
    qint16 amps() const;
    qint16 ampsPhaseA() const;
    qint16 ampsPhaseB() const;
    qint16 ampsPhaseC() const;
    qint16 voltageLn() const;
    qint16 phaseVoltageAn() const;
    qint16 phaseVoltageBn() const;
    qint16 phaseVoltageCn() const;
    qint16 voltageLl() const;
    qint16 phaseVoltageAb() const;
    qint16 phaseVoltageBc() const;
    qint16 phaseVoltageCa() const;
    qint16 hz() const;
    qint16 watts() const;
    qint16 wattsPhaseA() const;
    qint16 wattsPhaseB() const;
    qint16 wattsPhaseC() const;
    qint16 va() const;
    qint16 vaPhaseA() const;
    qint16 vaPhaseB() const;
    qint16 vaPhaseC() const;
    qint16 var() const;
    qint16 varPhaseA() const;
    qint16 varPhaseB() const;
    qint16 varPhaseC() const;
    qint16 pf() const;
    qint16 pfPhaseA() const;
    qint16 pfPhaseB() const;
    qint16 pfPhaseC() const;
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

private:
    quint16 m_modelId;
    quint16 m_modelLength;
    qint16 m_amps;
    qint16 m_ampsPhaseA;
    qint16 m_ampsPhaseB;
    qint16 m_ampsPhaseC;
    qint16 m_a_SF;
    qint16 m_voltageLn;
    qint16 m_phaseVoltageAn;
    qint16 m_phaseVoltageBn;
    qint16 m_phaseVoltageCn;
    qint16 m_voltageLl;
    qint16 m_phaseVoltageAb;
    qint16 m_phaseVoltageBc;
    qint16 m_phaseVoltageCa;
    qint16 m_v_SF;
    qint16 m_hz;
    qint16 m_hz_SF;
    qint16 m_watts;
    qint16 m_wattsPhaseA;
    qint16 m_wattsPhaseB;
    qint16 m_wattsPhaseC;
    qint16 m_w_SF;
    qint16 m_va;
    qint16 m_vaPhaseA;
    qint16 m_vaPhaseB;
    qint16 m_vaPhaseC;
    qint16 m_vA_SF;
    qint16 m_var;
    qint16 m_varPhaseA;
    qint16 m_varPhaseB;
    qint16 m_varPhaseC;
    qint16 m_vAR_SF;
    qint16 m_pf;
    qint16 m_pfPhaseA;
    qint16 m_pfPhaseB;
    qint16 m_pfPhaseC;
    qint16 m_pF_SF;
    quint32 m_totalWattHoursExported;
    quint32 m_totalWattHoursExportedPhaseA;
    quint32 m_totalWattHoursExportedPhaseB;
    quint32 m_totalWattHoursExportedPhaseC;
    quint32 m_totalWattHoursImported;
    quint32 m_totalWattHoursImportedPhaseA;
    quint32 m_totalWattHoursImportedPhaseB;
    quint32 m_totalWattHoursImportedPhaseC;
    qint16 m_totWh_SF;
    quint32 m_totalVaHoursExported;
    quint32 m_totalVaHoursExportedPhaseA;
    quint32 m_totalVaHoursExportedPhaseB;
    quint32 m_totalVaHoursExportedPhaseC;
    quint32 m_totalVaHoursImported;
    quint32 m_totalVaHoursImportedPhaseA;
    quint32 m_totalVaHoursImportedPhaseB;
    quint32 m_totalVaHoursImportedPhaseC;
    qint16 m_totVAh_SF;
    quint32 m_totalVarHoursImportedQ1;
    quint32 m_totalVArHoursImportedQ1PhaseA;
    quint32 m_totalVArHoursImportedQ1PhaseB;
    quint32 m_totalVArHoursImportedQ1PhaseC;
    quint32 m_totalVArHoursImportedQ2;
    quint32 m_totalVArHoursImportedQ2PhaseA;
    quint32 m_totalVArHoursImportedQ2PhaseB;
    quint32 m_totalVArHoursImportedQ2PhaseC;
    quint32 m_totalVArHoursExportedQ3;
    quint32 m_totalVArHoursExportedQ3PhaseA;
    quint32 m_totalVArHoursExportedQ3PhaseB;
    quint32 m_totalVArHoursExportedQ3PhaseC;
    quint32 m_totalVArHoursExportedQ4;
    quint32 m_totalVArHoursExportedQ4ImportedPhaseA;
    quint32 m_totalVArHoursExportedQ4ImportedPhaseB;
    quint32 m_totalVArHoursExportedQ4ImportedPhaseC;
    qint16 m_totVArh_SF;
    EvtFlags m_events;

    void initDataPoints();

};

#endif // SUNSPECACMETERMODEL_H
