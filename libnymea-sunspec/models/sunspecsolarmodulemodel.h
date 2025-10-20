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

#ifndef SUNSPECSOLARMODULEMODEL_H
#define SUNSPECSOLARMODULEMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecSolarModuleModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Stat {
        StatOff = 1,
        StatSleeping = 2,
        StatStarting = 3,
        StatMppt = 4,
        StatThrottled = 5,
        StatShuttingDown = 6,
        StatFault = 7,
        StatStandby = 8,
        StatTest = 9,
        StatOther = 10
    };
    Q_ENUM(Stat)

    enum Evt {
        EvtGroundFault = 0x1,
        EvtInputOverVoltage = 0x2,
        EvtReserved2 = 0x4,
        EvtDcDisconnect = 0x8,
        EvtReserved4 = 0x10,
        EvtReserved5 = 0x20,
        EvtManualShutdown = 0x40,
        EvtOverTemperature = 0x80,
        EvtReserved8 = 0x100,
        EvtReserved9 = 0x200,
        EvtReserved10 = 0x400,
        EvtReserved11 = 0x800,
        EvtBlownFuse = 0x1000,
        EvtUnderTemperature = 0x2000,
        EvtMemoryLoss = 0x4000,
        EvtArcDetection = 0x8000,
        EvtTheftDetection = 0x10000,
        EvtOutputOverCurrent = 0x20000,
        EvtOutputOverVoltage = 0x40000,
        EvtOutputUnderVoltage = 0x80000,
        EvtTestFailed = 0x100000
    };
    Q_DECLARE_FLAGS(EvtFlags, Evt)
    Q_FLAG(Evt)

    explicit SunSpecSolarModuleModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecSolarModuleModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    /* Current scale factor */
    qint16 a_SF() const;
    /* Voltage scale factor */
    qint16 v_SF() const;
    /* Power scale factor */
    qint16 w_SF() const;
    /* Energy scale factor */
    qint16 wh_SF() const;
    /* Enumerated value.  Module Status Code */
    Stat status() const;
    /* Module Vendor Status Code */
    quint16 vendorStatus() const;
    /* Bitmask value.  Module Event Flags */
    EvtFlags events() const;
    /* Vendor specific flags */
    quint32 vendorModuleEventFlags() const;

    /* Module Control */
    quint16 control() const;
    QModbusReply *setControl(quint16 control);

    /* Vendor Module Control */
    quint32 vendorControl() const;
    QModbusReply *setVendorControl(quint32 vendorControl);

    /* Module Control Value */
    qint32 controlValue() const;
    QModbusReply *setControlValue(qint32 controlValue);

    /* Time in seconds since 2000 epoch [Secs] */
    quint32 timestamp() const;
    /* Output Current [A] */
    float outputCurrent() const;
    /* Output Voltage [V] */
    float outputVoltage() const;
    /* Output Energy [Wh] */
    quint32 outputEnergy() const;
    /* Output Power [W] */
    float outputPower() const;
    /* Module Temperature [C] */
    qint16 temp() const;
    /* Input Current [A] */
    float inputCurrent() const;
    /* Input Voltage [V] */
    float inputVoltage() const;
    /* Input Energy [Wh] */
    quint32 inputEnergy() const;
    /* Input Power [W] */
    float inputPower() const;

protected:
    quint16 m_fixedBlockLength = 28;

    void initDataPoints();
    void processBlockData() override;

private:
    qint16 m_a_SF = 0;
    qint16 m_v_SF = 0;
    qint16 m_w_SF = 0;
    qint16 m_wh_SF = 0;
    Stat m_status;
    quint16 m_vendorStatus = 0;
    EvtFlags m_events;
    quint32 m_vendorModuleEventFlags = 0;
    quint16 m_control = 0;
    quint32 m_vendorControl = 0;
    qint32 m_controlValue = 0;
    quint32 m_timestamp = 0;
    float m_outputCurrent = 0;
    float m_outputVoltage = 0;
    quint32 m_outputEnergy = 0;
    float m_outputPower = 0;
    qint16 m_temp = 0;
    float m_inputCurrent = 0;
    float m_inputVoltage = 0;
    quint32 m_inputEnergy = 0;
    float m_inputPower = 0;


};

QDebug operator<<(QDebug debug, SunSpecSolarModuleModel *model);

#endif // SUNSPECSOLARMODULEMODEL_H
