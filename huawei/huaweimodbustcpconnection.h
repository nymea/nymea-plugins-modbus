/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2022, nymea GmbH
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

#ifndef HUAWEIMODBUSTCPCONNECTION_H
#define HUAWEIMODBUSTCPCONNECTION_H

#include <QObject>

#include "../modbus/modbusdatautils.h"
#include "../modbus/modbustcpmaster.h"

class HuaweiModbusTcpConnection : public ModbusTCPMaster
{
    Q_OBJECT
public:
    enum Registers {
        RegisterInverterActivePower = 32080,
        RegisterInverterDeviceStatus = 32089,
        RegisterInverterEnergyProduced = 32106,
        RegisterLunaBattery1Status = 37000,
        RegisterLunaBattery1Power = 37001,
        RegisterLunaBattery1Soc = 37004,
        RegisterPowerMeterActivePower = 37113,
        RegisterLunaBattery2Soc = 37738,
        RegisterLunaBattery2Status = 37741,
        RegisterLunaBattery2Power = 37743
    };
    Q_ENUM(Registers)

    enum InverterDeviceStatus {
        InverterDeviceStatusStandbyInitializing = 0,
        InverterDeviceStatusStandbyDetectingInsulationResistance = 1,
        InverterDeviceStatusStandbyDetectingIrradiation = 2,
        InverterDeviceStatusStandbyDridDetecting = 3,
        InverterDeviceStatusStarting = 256,
        InverterDeviceStatusOnGrid = 512,
        InverterDeviceStatusPowerLimited = 513,
        InverterDeviceStatusSelfDerating = 514,
        InverterDeviceStatusShutdownFault = 768,
        InverterDeviceStatusShutdownCommand = 769,
        InverterDeviceStatusShutdownOVGR = 770,
        InverterDeviceStatusShutdownCommunicationDisconnected = 771,
        InverterDeviceStatusShutdownPowerLimit = 772,
        InverterDeviceStatusShutdownManualStartupRequired = 773,
        InverterDeviceStatusShutdownInputUnderpower = 774,
        InverterDeviceStatusGridSchedulingPCurve = 1025,
        InverterDeviceStatusGridSchedulingQUCurve = 1026,
        InverterDeviceStatusGridSchedulingPFUCurve = 1027,
        InverterDeviceStatusGridSchedulingDryContact = 1028,
        InverterDeviceStatusGridSchedulingQPCurve = 1029,
        InverterDeviceStatusSpotCheckReady = 1280,
        InverterDeviceStatusSpotChecking = 1281,
        InverterDeviceStatusInspecting = 1536,
        InverterDeviceStatusAfciSelfCheck = 1792,
        InverterDeviceStatusIVScanning = 2048,
        InverterDeviceStatusDCInputDetection = 2304,
        InverterDeviceStatusRunningOffGridCharging = 2560,
        InverterDeviceStatusStandbyNoIrradiation = 40960
    };
    Q_ENUM(InverterDeviceStatus)

    enum BatteryDeviceStatus {
        BatteryDeviceStatusOffline = 0,
        BatteryDeviceStatusStandby = 1,
        BatteryDeviceStatusRunning = 1,
        BatteryDeviceStatusFault = 1,
        BatteryDeviceStatusSleepMode = 1
    };
    Q_ENUM(BatteryDeviceStatus)

    explicit HuaweiModbusTcpConnection(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent = nullptr);
    ~HuaweiModbusTcpConnection() = default;

    /* Inverter active power [kW] - Address: 32080, Size: 2 */
    float inverterActivePower() const;

    /* Inverter device status - Address: 32089, Size: 1 */
    InverterDeviceStatus inverterDeviceStatus() const;

    /* Inverter energy produced [kWh] - Address: 32106, Size: 2 */
    float inverterEnergyProduced() const;

    /* Power meter active power [W] - Address: 37113, Size: 2 */
    qint32 powerMeterActivePower() const;

    /* Luna 2000 Battery 1 status - Address: 37000, Size: 1 */
    BatteryDeviceStatus lunaBattery1Status() const;

    /* Luna 2000 Battery 1 power [W] - Address: 37001, Size: 2 */
    qint32 lunaBattery1Power() const;

    /* Luna 2000 Battery 1 state of charge [%] - Address: 37004, Size: 1 */
    float lunaBattery1Soc() const;

    /* Luna 2000 Battery 2 status - Address: 37741, Size: 1 */
    BatteryDeviceStatus lunaBattery2Status() const;

    /* Luna 2000 Battery 2 power [W] - Address: 37743, Size: 2 */
    qint32 lunaBattery2Power() const;

    /* Luna 2000 Battery 2 state of charge [%] - Address: 37738, Size: 1 */
    float lunaBattery2Soc() const;


    virtual void initialize();
    virtual void update();

    void updateInverterActivePower();
    void updateInverterDeviceStatus();
    void updateInverterEnergyProduced();
    void updatePowerMeterActivePower();
    void updateLunaBattery1Status();
    void updateLunaBattery1Power();
    void updateLunaBattery1Soc();
    void updateLunaBattery2Status();
    void updateLunaBattery2Power();
    void updateLunaBattery2Soc();

signals:
    void initializationFinished();

    void inverterActivePowerChanged(float inverterActivePower);
    void inverterDeviceStatusChanged(InverterDeviceStatus inverterDeviceStatus);
    void inverterEnergyProducedChanged(float inverterEnergyProduced);
    void powerMeterActivePowerChanged(qint32 powerMeterActivePower);
    void lunaBattery1StatusChanged(BatteryDeviceStatus lunaBattery1Status);
    void lunaBattery1PowerChanged(qint32 lunaBattery1Power);
    void lunaBattery1SocChanged(float lunaBattery1Soc);
    void lunaBattery2StatusChanged(BatteryDeviceStatus lunaBattery2Status);
    void lunaBattery2PowerChanged(qint32 lunaBattery2Power);
    void lunaBattery2SocChanged(float lunaBattery2Soc);

protected:
    QModbusReply *readInverterActivePower();
    QModbusReply *readInverterDeviceStatus();
    QModbusReply *readInverterEnergyProduced();
    QModbusReply *readPowerMeterActivePower();
    QModbusReply *readLunaBattery1Status();
    QModbusReply *readLunaBattery1Power();
    QModbusReply *readLunaBattery1Soc();
    QModbusReply *readLunaBattery2Status();
    QModbusReply *readLunaBattery2Power();
    QModbusReply *readLunaBattery2Soc();

    float m_inverterActivePower = 0;
    InverterDeviceStatus m_inverterDeviceStatus = InverterDeviceStatusStandbyInitializing;
    float m_inverterEnergyProduced = 0;
    qint32 m_powerMeterActivePower = 0;
    BatteryDeviceStatus m_lunaBattery1Status = BatteryDeviceStatusOffline;
    qint32 m_lunaBattery1Power = 0;
    float m_lunaBattery1Soc = 0;
    BatteryDeviceStatus m_lunaBattery2Status = BatteryDeviceStatusOffline;
    qint32 m_lunaBattery2Power = 0;
    float m_lunaBattery2Soc = 0;

private:
    quint16 m_slaveId = 1;
    QVector<QModbusReply *> m_pendingInitReplies;

    void verifyInitFinished();


};

QDebug operator<<(QDebug debug, HuaweiModbusTcpConnection *huaweiModbusTcpConnection);

#endif // HUAWEIMODBUSTCPCONNECTION_H
