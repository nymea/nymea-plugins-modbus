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

#ifndef SUNSPECINVERTERSPLITPHASEMODEL_H
#define SUNSPECINVERTERSPLITPHASEMODEL_H

#include <QObject>

#include "sunspec.h"
#include "sunspecmodel.h"

class SunSpecInverterSplitPhaseModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum St {
        StOff = 1,
        StSleeping = 2,
        StStarting = 3,
        StMppt = 4,
        StThrottled = 5,
        StShuttingDown = 6,
        StFault = 7,
        StStandby = 8
    };
    Q_ENUM(St)

    enum Evt1 {
        Evt1GroundFault = 0x1,
        Evt1DcOverVolt = 0x2,
        Evt1AcDisconnect = 0x4,
        Evt1DcDisconnect = 0x8,
        Evt1GridDisconnect = 0x10,
        Evt1CabinetOpen = 0x20,
        Evt1ManualShutdown = 0x40,
        Evt1OverTemp = 0x80,
        Evt1OverFrequency = 0x100,
        Evt1UnderFrequency = 0x200,
        Evt1AcOverVolt = 0x400,
        Evt1AcUnderVolt = 0x800,
        Evt1BlownStringFuse = 0x1000,
        Evt1UnderTemp = 0x2000,
        Evt1MemoryLoss = 0x4000,
        Evt1HwTestFailure = 0x8000
    };
    Q_DECLARE_FLAGS(Evt1Flags, Evt1)
    Q_FLAG(Evt1)

    explicit SunSpecInverterSplitPhaseModel(SunSpec *connection, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecInverterSplitPhaseModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    float amps() const;
    float ampsPhaseA() const;
    float ampsPhaseB() const;
    float ampsPhaseC() const;
    float phaseVoltageAb() const;
    float phaseVoltageBc() const;
    float phaseVoltageCa() const;
    float phaseVoltageAn() const;
    float phaseVoltageBn() const;
    float phaseVoltageCn() const;
    float watts() const;
    float hz() const;
    float va() const;
    float vAr() const;
    float pf() const;
    quint32 wattHours() const;
    float dcAmps() const;
    float dcVoltage() const;
    float dcWatts() const;
    float cabinetTemperature() const;
    float heatSinkTemperature() const;
    float transformerTemperature() const;
    float otherTemperature() const;
    St operatingState() const;
    quint16 vendorOperatingState() const;
    Evt1Flags event1() const;
    quint32 eventBitfield2() const;
    quint32 vendorEventBitfield1() const;
    quint32 vendorEventBitfield2() const;
    quint32 vendorEventBitfield3() const;
    quint32 vendorEventBitfield4() const;

protected:
    void processBlockData() override;

private:
    float m_amps;
    float m_ampsPhaseA;
    float m_ampsPhaseB;
    float m_ampsPhaseC;
    qint16 m_a_SF;
    float m_phaseVoltageAb;
    float m_phaseVoltageBc;
    float m_phaseVoltageCa;
    float m_phaseVoltageAn;
    float m_phaseVoltageBn;
    float m_phaseVoltageCn;
    qint16 m_v_SF;
    float m_watts;
    qint16 m_w_SF;
    float m_hz;
    qint16 m_hz_SF;
    float m_va;
    qint16 m_vA_SF;
    float m_vAr;
    qint16 m_vAr_SF;
    float m_pf;
    qint16 m_pF_SF;
    quint32 m_wattHours;
    qint16 m_wH_SF;
    float m_dcAmps;
    qint16 m_dCA_SF;
    float m_dcVoltage;
    qint16 m_dCV_SF;
    float m_dcWatts;
    qint16 m_dCW_SF;
    float m_cabinetTemperature;
    float m_heatSinkTemperature;
    float m_transformerTemperature;
    float m_otherTemperature;
    qint16 m_tmp_SF;
    St m_operatingState;
    quint16 m_vendorOperatingState;
    Evt1Flags m_event1;
    quint32 m_eventBitfield2;
    quint32 m_vendorEventBitfield1;
    quint32 m_vendorEventBitfield2;
    quint32 m_vendorEventBitfield3;
    quint32 m_vendorEventBitfield4;

    void initDataPoints();

};

#endif // SUNSPECINVERTERSPLITPHASEMODEL_H
