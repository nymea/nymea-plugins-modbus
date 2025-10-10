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

#ifndef SUNSPECINVERTERSPLITPHASEFLOATMODEL_H
#define SUNSPECINVERTERSPLITPHASEFLOATMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecInverterSplitPhaseFloatModel : public SunSpecModel
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

    explicit SunSpecInverterSplitPhaseFloatModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecInverterSplitPhaseFloatModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    /* AC Current [A] */
    float amps() const;
    /* Phase A Current [A] */
    float ampsPhaseA() const;
    /* Phase B Current [A] */
    float ampsPhaseB() const;
    /* Phase C Current [A] */
    float ampsPhaseC() const;
    /* Phase Voltage AB [V] */
    float phaseVoltageAb() const;
    /* Phase Voltage BC [V] */
    float phaseVoltageBc() const;
    /* Phase Voltage CA [V] */
    float phaseVoltageCa() const;
    /* Phase Voltage AN [V] */
    float phaseVoltageAn() const;
    /* Phase Voltage BN [V] */
    float phaseVoltageBn() const;
    /* Phase Voltage CN [V] */
    float phaseVoltageCn() const;
    /* AC Power [W] */
    float watts() const;
    /* Line Frequency [Hz] */
    float hz() const;
    /* AC Apparent Power [VA] */
    float va() const;
    /* AC Reactive Power [var] */
    float vAr() const;
    /* AC Power Factor [Pct] */
    float pf() const;
    /* AC Energy [Wh] */
    float wattHours() const;
    /* DC Current [A] */
    float dcAmps() const;
    /* DC Voltage [V] */
    float dcVoltage() const;
    /* DC Power [W] */
    float dcWatts() const;
    /* Cabinet Temperature [C] */
    float cabinetTemperature() const;
    /* Heat Sink Temperature [C] */
    float heatSinkTemperature() const;
    /* Transformer Temperature [C] */
    float transformerTemperature() const;
    /* Other Temperature [C] */
    float otherTemperature() const;
    /* Enumerated value.  Operating state */
    St operatingState() const;
    /* Vendor specific operating state code */
    quint16 vendorOperatingState() const;
    /* Bitmask value. Event fields */
    Evt1Flags event1() const;
    /* Reserved for future use */
    quint32 eventBitfield2() const;
    /* Vendor defined events */
    quint32 vendorEventBitfield1() const;
    /* Vendor defined events */
    quint32 vendorEventBitfield2() const;
    /* Vendor defined events */
    quint32 vendorEventBitfield3() const;
    /* Vendor defined events */
    quint32 vendorEventBitfield4() const;

protected:
    quint16 m_fixedBlockLength = 60;

    void initDataPoints();
    void processBlockData() override;

private:
    float m_amps = 0;
    float m_ampsPhaseA = 0;
    float m_ampsPhaseB = 0;
    float m_ampsPhaseC = 0;
    float m_phaseVoltageAb = 0;
    float m_phaseVoltageBc = 0;
    float m_phaseVoltageCa = 0;
    float m_phaseVoltageAn = 0;
    float m_phaseVoltageBn = 0;
    float m_phaseVoltageCn = 0;
    float m_watts = 0;
    float m_hz = 0;
    float m_va = 0;
    float m_vAr = 0;
    float m_pf = 0;
    float m_wattHours = 0;
    float m_dcAmps = 0;
    float m_dcVoltage = 0;
    float m_dcWatts = 0;
    float m_cabinetTemperature = 0;
    float m_heatSinkTemperature = 0;
    float m_transformerTemperature = 0;
    float m_otherTemperature = 0;
    St m_operatingState;
    quint16 m_vendorOperatingState = 0;
    Evt1Flags m_event1;
    quint32 m_eventBitfield2 = 0;
    quint32 m_vendorEventBitfield1 = 0;
    quint32 m_vendorEventBitfield2 = 0;
    quint32 m_vendorEventBitfield3 = 0;
    quint32 m_vendorEventBitfield4 = 0;


};

QDebug operator<<(QDebug debug, SunSpecInverterSplitPhaseFloatModel *model);

#endif // SUNSPECINVERTERSPLITPHASEFLOATMODEL_H
