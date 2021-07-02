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

#ifndef SUNSPECMETERSINGLEPHASEMODEL_H
#define SUNSPECMETERSINGLEPHASEMODEL_H

#include <QObject>

#include "sunspec.h"
#include "sunspecmodel.h"

class SunSpecMeterSinglePhaseModel : public SunSpecModel
{
    Q_OBJECT
public:

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

    explicit SunSpecMeterSinglePhaseModel(SunSpec *connection, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecMeterSinglePhaseModel() override; 

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
    float m_amps;
    float m_ampsPhaseA;
    float m_ampsPhaseB;
    float m_ampsPhaseC;
    qint16 m_a_SF;
    float m_voltageLn;
    float m_phaseVoltageAn;
    float m_phaseVoltageBn;
    float m_phaseVoltageCn;
    float m_voltageLl;
    float m_phaseVoltageAb;
    float m_phaseVoltageBc;
    float m_phaseVoltageCa;
    qint16 m_v_SF;
    float m_hz;
    qint16 m_hz_SF;
    float m_watts;
    float m_wattsPhaseA;
    float m_wattsPhaseB;
    float m_wattsPhaseC;
    qint16 m_w_SF;
    float m_va;
    float m_vaPhaseA;
    float m_vaPhaseB;
    float m_vaPhaseC;
    qint16 m_vA_SF;
    float m_var;
    float m_varPhaseA;
    float m_varPhaseB;
    float m_varPhaseC;
    qint16 m_vAR_SF;
    float m_pf;
    float m_pfPhaseA;
    float m_pfPhaseB;
    float m_pfPhaseC;
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

#endif // SUNSPECMETERSINGLEPHASEMODEL_H
