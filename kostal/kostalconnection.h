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

#ifndef KOSTALCONNECTION_H
#define KOSTALCONNECTION_H

#include <QObject>

#include "../modbus/modbusdatautils.h"
#include "../modbus/modbustcpmaster.h"

class KostalConnection : public ModbusTCPMaster
{
    Q_OBJECT
public:
    enum ByteOrder {
        ByteOrderLittleEndian = 0,
        ByteOrderBigEndian = 1
    };
    Q_ENUM(ByteOrder)

    enum InverterState {
        InverterStateOff = 0,
        InverterStateInit = 1,
        InverterStateIsoMeas = 2,
        InverterStateGridCheck = 3,
        InverterStateStartUp = 4,
        InverterStateFeedIn = 6,
        InverterStateThrottled = 7,
        InverterStateExtSwitchOff = 8,
        InverterStateUpdate = 9,
        InverterStateStandby = 10,
        InverterStateGridSync = 11,
        InverterStateGridPreCheck = 12,
        InverterStateGridSwitchOff = 13,
        InverterStateOverheating = 14,
        InverterStateShutdown = 15,
        InverterStateImproperDcVoltage = 16,
        InverterStateEsb = 17,
        InverterStateUnknown = 18
    };
    Q_ENUM(InverterState)

    enum EnergyManagementState {
        EnergyManagementStateIdle = 0,
        EnergyManagementStateEmergencyBatteryCharge = 2,
        EnergyManagementStateWinterModeStep1 = 8,
        EnergyManagementStateWinterModeStep2 = 16
    };
    Q_ENUM(EnergyManagementState)

    enum BatteryType {
        BatteryTypeNoBattery = 0,
        BatteryTypeSonyMurata = 2,
        BatteryTypeBydBbox = 4
    };
    Q_ENUM(BatteryType)

    explicit KostalConnection(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent = nullptr);
    ~KostalConnection() = default;

    /* MODBUS Unit-ID - Address: 4, Size: 1 */
    quint16 modbusUnitId() const;

    /* MODBUS Byte Order Note - Address: 5, Size: 1 */
    ByteOrder modbusByteOrder() const;

    /* Inverter article number - Address: 6, Size: 8 */
    QString inverterArticleNumber() const;

    /* Number of bidirectional converter - Address: 30, Size: 1 */
    quint16 bidirectionalConverterNumber() const;

    /* Number of PV strings - Address: 34, Size: 1 */
    quint16 numberPvStrings() const;

    /* Hardware-Version - Address: 36, Size: 2 */
    quint32 hardwareVersion() const;

    /* Software-Version Maincontroller (MC) - Address: 38, Size: 8 */
    QString softwareVersionMainController() const;

    /* Software-Version IO-Controller (IOC) - Address: 46, Size: 8 */
    QString softwareVersionIoController() const;

    /* Power-ID - Address: 54, Size: 1 */
    quint16 powerId() const;

    /* Inverter state - Address: 56, Size: 1 */
    InverterState inverterState() const;

    /* Total DC power [W] - Address: 100, Size: 2 */
    float totalDcPower() const;

    /* State of energy manager - Address: 104, Size: 2 */
    EnergyManagementState energyManagementState() const;

    /* Home own consumption from battery [W] - Address: 106, Size: 2 */
    float homeOwnConsumptionFromBattery() const;

    /* Home own consumption from grid [W] - Address: 108, Size: 2 */
    float homeOwnConsumptionFromGrid() const;

    /* Total home consumption Battery [Wh] - Address: 110, Size: 2 */
    float totalHomeConsumptionFromBattery() const;

    /* Total home consumption grid [Wh] - Address: 112, Size: 2 */
    float totalHomeConsumptionFromGrid() const;

    /* Total home consumption from PV [Wh] - Address: 114, Size: 2 */
    float totalHomeConsumptionFromPv() const;

    /* Home own consumption from PV [W] - Address: 116, Size: 2 */
    float homeOwnConsumptionPv() const;

    /* Total home consumption [Wh] - Address: 118, Size: 2 */
    float totalHomeConsumption() const;

    /* Grid frequency [Hz] - Address: 152, Size: 2 */
    float gridFrequency() const;

    /* Current phase 1 [A] - Address: 154, Size: 2 */
    float currentPhase1() const;

    /* Active power phase 1 [W] - Address: 156, Size: 2 */
    float activePowerPhase1() const;

    /* Voltage phase 1 [V] - Address: 158, Size: 2 */
    float voltagePhase1() const;

    /* Current phase 2 [A] - Address: 160, Size: 2 */
    float currentPhase2() const;

    /* Active power phase 2 [W] - Address: 162, Size: 2 */
    float activePowerPhase2() const;

    /* Voltage phase 2 [V] - Address: 164, Size: 2 */
    float voltagePhase2() const;

    /* Current phase 3 [A] - Address: 166, Size: 2 */
    float currentPhase3() const;

    /* Active power phase 3 [W] - Address: 168, Size: 2 */
    float activePowerPhase3() const;

    /* Voltage phase 3 [V] - Address: 170, Size: 2 */
    float voltagePhase3() const;

    /* Total AC power [W] - Address: 172, Size: 2 */
    float totalAcPower() const;

    /* Battery charge current [A] - Address: 190, Size: 2 */
    float batteryChargeCurrent() const;

    /* Number of battery cycles - Address: 194, Size: 2 */
    float numberOfBytteryCycles() const;

    /* Actual battery charge (-) / discharge (+) current [A] - Address: 200, Size: 2 */
    float actualBatteryChargeCurrent() const;

    /* Actual state of charge [%] - Address: 210, Size: 2 */
    float batteryStateOfCharge() const;

    /* Battery temperature [°C] - Address: 214, Size: 2 */
    float batteryTemperature() const;

    /* Battery voltage [V] - Address: 216, Size: 2 */
    float batteryVoltage() const;

    /* Current phase 1 (powermeter) [A] - Address: 222, Size: 2 */
    float powerMeterCurrentPhase1() const;

    /* Active power phase 1 (powermeter) [W] - Address: 224, Size: 2 */
    float powerMeterActivePowerPhase1() const;

    /* Voltage phase 1 (powermeter) [V] - Address: 230, Size: 2 */
    float powerMeterVoltagePhase1() const;

    /* Current phase 2 (powermeter) [A] - Address: 232, Size: 2 */
    float powerMeterCurrentPhase2() const;

    /* Active power phase 2 (powermeter) [W] - Address: 234, Size: 2 */
    float powerMeterActivePowerPhase2() const;

    /* Voltage phase 2 (powermeter) [V] - Address: 240, Size: 2 */
    float powerMeterVoltagePhase2() const;

    /* Current phase 3 (powermeter) [A] - Address: 242, Size: 2 */
    float powerMeterCurrentPhase3() const;

    /* Active power phase 3 (powermeter) [W] - Address: 244, Size: 2 */
    float powerMeterActivePowerPhase3() const;

    /* Voltage phase 3 (powermeter) [V] - Address: 250, Size: 2 */
    float powerMeterVoltagePhase3() const;

    /* Total active power (powermeter) [W] - Address: 252, Size: 2 */
    float powerMeterTotalActivePower() const;

    /* Battery Manufacturer - Address: 517, Size: 8 */
    QString batteryManufacturer() const;

    /* Battery model ID - Address: 525, Size: 2 */
    quint32 batteryModelId() const;

    /* Battery serial number - Address: 527, Size: 2 */
    quint32 batterySerialNumber() const;

    /* Inverter manufacturer - Address: 535, Size: 16 */
    QString inverterManufacturer() const;

    /* Inverter serial number - Address: 559, Size: 16 */
    QString inverterSerialNumber() const;

    /* Energy scale factor - Address: 579, Size: 1 */
    qint16 energyScaleFactor() const;

    /* Battery type - Address: 588, Size: 1 */
    BatteryType batteryType() const;

    /* Total energy AC-side to grid [Wh] - Address: 1064, Size: 1 */
    float totalEnergyAcToGrid() const;

    virtual void initialize();
    virtual void update();

    void updateInverterState();
    void updateTotalDcPower();
    void updateEnergyManagementState();
    void updateHomeOwnConsumptionFromBattery();
    void updateHomeOwnConsumptionFromGrid();
    void updateTotalHomeConsumptionFromBattery();
    void updateTotalHomeConsumptionFromGrid();
    void updateTotalHomeConsumptionFromPv();
    void updateHomeOwnConsumptionPv();
    void updateTotalHomeConsumption();
    void updateGridFrequency();
    void updateCurrentPhase1();
    void updateActivePowerPhase1();
    void updateVoltagePhase1();
    void updateCurrentPhase2();
    void updateActivePowerPhase2();
    void updateVoltagePhase2();
    void updateCurrentPhase3();
    void updateActivePowerPhase3();
    void updateVoltagePhase3();
    void updateTotalAcPower();
    void updateBatteryChargeCurrent();
    void updateNumberOfBytteryCycles();
    void updateActualBatteryChargeCurrent();
    void updateBatteryStateOfCharge();
    void updateBatteryTemperature();
    void updateBatteryVoltage();
    void updatePowerMeterCurrentPhase1();
    void updatePowerMeterActivePowerPhase1();
    void updatePowerMeterVoltagePhase1();
    void updatePowerMeterCurrentPhase2();
    void updatePowerMeterActivePowerPhase2();
    void updatePowerMeterVoltagePhase2();
    void updatePowerMeterCurrentPhase3();
    void updatePowerMeterActivePowerPhase3();
    void updatePowerMeterVoltagePhase3();
    void updatePowerMeterTotalActivePower();
    void updateTotalEnergyAcToGrid();

signals:
    void initializationFinished();

    void modbusUnitIdChanged(quint16 modbusUnitId);
    void modbusByteOrderChanged(ByteOrder modbusByteOrder);
    void inverterArticleNumberChanged(const QString &inverterArticleNumber);
    void bidirectionalConverterNumberChanged(quint16 bidirectionalConverterNumber);
    void numberPvStringsChanged(quint16 numberPvStrings);
    void hardwareVersionChanged(quint32 hardwareVersion);
    void softwareVersionMainControllerChanged(const QString &softwareVersionMainController);
    void softwareVersionIoControllerChanged(const QString &softwareVersionIoController);
    void powerIdChanged(quint16 powerId);
    void inverterStateChanged(InverterState inverterState);
    void totalDcPowerChanged(float totalDcPower);
    void energyManagementStateChanged(EnergyManagementState energyManagementState);
    void homeOwnConsumptionFromBatteryChanged(float homeOwnConsumptionFromBattery);
    void homeOwnConsumptionFromGridChanged(float homeOwnConsumptionFromGrid);
    void totalHomeConsumptionFromBatteryChanged(float totalHomeConsumptionFromBattery);
    void totalHomeConsumptionFromGridChanged(float totalHomeConsumptionFromGrid);
    void totalHomeConsumptionFromPvChanged(float totalHomeConsumptionFromPv);
    void homeOwnConsumptionPvChanged(float homeOwnConsumptionPv);
    void totalHomeConsumptionChanged(float totalHomeConsumption);
    void gridFrequencyChanged(float gridFrequency);
    void currentPhase1Changed(float currentPhase1);
    void activePowerPhase1Changed(float activePowerPhase1);
    void voltagePhase1Changed(float voltagePhase1);
    void currentPhase2Changed(float currentPhase2);
    void activePowerPhase2Changed(float activePowerPhase2);
    void voltagePhase2Changed(float voltagePhase2);
    void currentPhase3Changed(float currentPhase3);
    void activePowerPhase3Changed(float activePowerPhase3);
    void voltagePhase3Changed(float voltagePhase3);
    void totalAcPowerChanged(float totalAcPower);
    void batteryChargeCurrentChanged(float batteryChargeCurrent);
    void numberOfBytteryCyclesChanged(float numberOfBytteryCycles);
    void actualBatteryChargeCurrentChanged(float actualBatteryChargeCurrent);
    void batteryStateOfChargeChanged(float batteryStateOfCharge);
    void batteryTemperatureChanged(float batteryTemperature);
    void batteryVoltageChanged(float batteryVoltage);
    void powerMeterCurrentPhase1Changed(float powerMeterCurrentPhase1);
    void powerMeterActivePowerPhase1Changed(float powerMeterActivePowerPhase1);
    void powerMeterVoltagePhase1Changed(float powerMeterVoltagePhase1);
    void powerMeterCurrentPhase2Changed(float powerMeterCurrentPhase2);
    void powerMeterActivePowerPhase2Changed(float powerMeterActivePowerPhase2);
    void powerMeterVoltagePhase2Changed(float powerMeterVoltagePhase2);
    void powerMeterCurrentPhase3Changed(float powerMeterCurrentPhase3);
    void powerMeterActivePowerPhase3Changed(float powerMeterActivePowerPhase3);
    void powerMeterVoltagePhase3Changed(float powerMeterVoltagePhase3);
    void powerMeterTotalActivePowerChanged(float powerMeterTotalActivePower);
    void batteryManufacturerChanged(const QString &batteryManufacturer);
    void batteryModelIdChanged(quint32 batteryModelId);
    void batterySerialNumberChanged(quint32 batterySerialNumber);
    void inverterManufacturerChanged(const QString &inverterManufacturer);
    void inverterSerialNumberChanged(const QString &inverterSerialNumber);
    void energyScaleFactorChanged(qint16 energyScaleFactor);
    void batteryTypeChanged(BatteryType batteryType);
    void totalEnergyAcToGridChanged(float totalEnergyAcToGrid);

private:
    quint16 m_slaveId = 1;
    QVector<QModbusReply *> m_pendingInitReplies;

    quint16 m_modbusUnitId = 1;
    ByteOrder m_modbusByteOrder = ByteOrderBigEndian;
    QString m_inverterArticleNumber;
    quint16 m_bidirectionalConverterNumber = 0;
    quint16 m_numberPvStrings = 0;
    quint32 m_hardwareVersion = 0;
    QString m_softwareVersionMainController;
    QString m_softwareVersionIoController;
    quint16 m_powerId = 0;
    InverterState m_inverterState = InverterStateUnknown;
    float m_totalDcPower = 0;
    EnergyManagementState m_energyManagementState = EnergyManagementStateIdle;
    float m_homeOwnConsumptionFromBattery = 0;
    float m_homeOwnConsumptionFromGrid = 0;
    float m_totalHomeConsumptionFromBattery = 0;
    float m_totalHomeConsumptionFromGrid = 0;
    float m_totalHomeConsumptionFromPv = 0;
    float m_homeOwnConsumptionPv = 0;
    float m_totalHomeConsumption = 0;
    float m_gridFrequency = 0;
    float m_currentPhase1 = 0;
    float m_activePowerPhase1 = 0;
    float m_voltagePhase1 = 0;
    float m_currentPhase2 = 0;
    float m_activePowerPhase2 = 0;
    float m_voltagePhase2 = 0;
    float m_currentPhase3 = 0;
    float m_activePowerPhase3 = 0;
    float m_voltagePhase3 = 0;
    float m_totalAcPower = 0;
    float m_batteryChargeCurrent = 0;
    float m_numberOfBytteryCycles = 0;
    float m_actualBatteryChargeCurrent = 0;
    float m_batteryStateOfCharge = 0;
    float m_batteryTemperature = 0;
    float m_batteryVoltage = 0;
    float m_powerMeterCurrentPhase1 = 0;
    float m_powerMeterActivePowerPhase1 = 0;
    float m_powerMeterVoltagePhase1 = 0;
    float m_powerMeterCurrentPhase2 = 0;
    float m_powerMeterActivePowerPhase2 = 0;
    float m_powerMeterVoltagePhase2 = 0;
    float m_powerMeterCurrentPhase3 = 0;
    float m_powerMeterActivePowerPhase3 = 0;
    float m_powerMeterVoltagePhase3 = 0;
    float m_powerMeterTotalActivePower = 0;
    QString m_batteryManufacturer;
    quint32 m_batteryModelId = 0;
    quint32 m_batterySerialNumber = 0;
    QString m_inverterManufacturer;
    QString m_inverterSerialNumber;
    qint16 m_energyScaleFactor = 0;
    BatteryType m_batteryType = BatteryTypeNoBattery;
    float m_totalEnergyAcToGrid = 0;

    void verifyInitFinished();

    QModbusReply *readModbusUnitId();
    QModbusReply *readModbusByteOrder();
    QModbusReply *readInverterArticleNumber();
    QModbusReply *readBidirectionalConverterNumber();
    QModbusReply *readNumberPvStrings();
    QModbusReply *readHardwareVersion();
    QModbusReply *readSoftwareVersionMainController();
    QModbusReply *readSoftwareVersionIoController();
    QModbusReply *readPowerId();
    QModbusReply *readInverterState();
    QModbusReply *readTotalDcPower();
    QModbusReply *readEnergyManagementState();
    QModbusReply *readHomeOwnConsumptionFromBattery();
    QModbusReply *readHomeOwnConsumptionFromGrid();
    QModbusReply *readTotalHomeConsumptionFromBattery();
    QModbusReply *readTotalHomeConsumptionFromGrid();
    QModbusReply *readTotalHomeConsumptionFromPv();
    QModbusReply *readHomeOwnConsumptionPv();
    QModbusReply *readTotalHomeConsumption();
    QModbusReply *readGridFrequency();
    QModbusReply *readCurrentPhase1();
    QModbusReply *readActivePowerPhase1();
    QModbusReply *readVoltagePhase1();
    QModbusReply *readCurrentPhase2();
    QModbusReply *readActivePowerPhase2();
    QModbusReply *readVoltagePhase2();
    QModbusReply *readCurrentPhase3();
    QModbusReply *readActivePowerPhase3();
    QModbusReply *readVoltagePhase3();
    QModbusReply *readTotalAcPower();
    QModbusReply *readBatteryChargeCurrent();
    QModbusReply *readNumberOfBytteryCycles();
    QModbusReply *readActualBatteryChargeCurrent();
    QModbusReply *readBatteryStateOfCharge();
    QModbusReply *readBatteryTemperature();
    QModbusReply *readBatteryVoltage();
    QModbusReply *readPowerMeterCurrentPhase1();
    QModbusReply *readPowerMeterActivePowerPhase1();
    QModbusReply *readPowerMeterVoltagePhase1();
    QModbusReply *readPowerMeterCurrentPhase2();
    QModbusReply *readPowerMeterActivePowerPhase2();
    QModbusReply *readPowerMeterVoltagePhase2();
    QModbusReply *readPowerMeterCurrentPhase3();
    QModbusReply *readPowerMeterActivePowerPhase3();
    QModbusReply *readPowerMeterVoltagePhase3();
    QModbusReply *readPowerMeterTotalActivePower();
    QModbusReply *readBatteryManufacturer();
    QModbusReply *readBatteryModelId();
    QModbusReply *readBatterySerialNumber();
    QModbusReply *readInverterManufacturer();
    QModbusReply *readInverterSerialNumber();
    QModbusReply *readEnergyScaleFactor();
    QModbusReply *readBatteryType();
    QModbusReply *readTotalEnergyAcToGrid();


};

QDebug operator<<(QDebug debug, KostalConnection *kostalConnection);

#endif // KOSTALCONNECTION_H
