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

#ifndef SUNSPECBATTERYMODEL_H
#define SUNSPECBATTERYMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecBatteryModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Chast {
        ChastOff = 1,
        ChastEmpty = 2,
        ChastDischarging = 3,
        ChastCharging = 4,
        ChastFull = 5,
        ChastHolding = 6,
        ChastTesting = 7
    };
    Q_ENUM(Chast)

    enum Locremctl {
        LocremctlRemote = 0,
        LocremctlLocal = 1
    };
    Q_ENUM(Locremctl)

    enum Typ {
        TypNotApplicableUnknown = 0,
        TypLeadAcid = 1,
        TypNickelMetalHydrate = 2,
        TypNickelCadmium = 3,
        TypLithiumIon = 4,
        TypCarbonZinc = 5,
        TypZincChloride = 6,
        TypAlkaline = 7,
        TypRechargeableAlkaline = 8,
        TypSodiumSulfur = 9,
        TypFlow = 10,
        TypOther = 99
    };
    Q_ENUM(Typ)

    enum State {
        StateDisconnected = 1,
        StateInitializing = 2,
        StateConnected = 3,
        StateStandby = 4,
        StateSocProtection = 5,
        StateSuspending = 6,
        StateFault = 99
    };
    Q_ENUM(State)

    enum Reqinvstate {
        ReqinvstateNoRequest = 0,
        ReqinvstateStart = 1,
        ReqinvstateStop = 2
    };
    Q_ENUM(Reqinvstate)

    enum Setop {
        SetopConnect = 1,
        SetopDisconnect = 2
    };
    Q_ENUM(Setop)

    enum Setinvstate {
        SetinvstateInverterStopped = 1,
        SetinvstateInverterStandby = 2,
        SetinvstateInverterStarted = 3
    };
    Q_ENUM(Setinvstate)

    enum Evt1 {
        Evt1CommunicationError = 0x1,
        Evt1OverTempAlarm = 0x2,
        Evt1OverTempWarning = 0x4,
        Evt1UnderTempAlarm = 0x8,
        Evt1UnderTempWarning = 0x10,
        Evt1OverChargeCurrentAlarm = 0x20,
        Evt1OverChargeCurrentWarning = 0x40,
        Evt1OverDischargeCurrentAlarm = 0x80,
        Evt1OverDischargeCurrentWarning = 0x100,
        Evt1OverVoltAlarm = 0x200,
        Evt1OverVoltWarning = 0x400,
        Evt1UnderVoltAlarm = 0x800,
        Evt1UnderVoltWarning = 0x1000,
        Evt1UnderSocMinAlarm = 0x2000,
        Evt1UnderSocMinWarning = 0x4000,
        Evt1OverSocMaxAlarm = 0x8000,
        Evt1OverSocMaxWarning = 0x10000,
        Evt1VoltageImbalanceWarning = 0x20000,
        Evt1TemperatureImbalanceAlarm = 0x40000,
        Evt1TemperatureImbalanceWarning = 0x80000,
        Evt1ContactorError = 0x100000,
        Evt1FanError = 0x200000,
        Evt1GroundFault = 0x400000,
        Evt1OpenDoorError = 0x800000,
        Evt1CurrentImbalanceWarning = 0x1000000,
        Evt1OtherAlarm = 0x2000000,
        Evt1OtherWarning = 0x4000000,
        Evt1Reserved1 = 0x8000000,
        Evt1ConfigurationAlarm = 0x10000000,
        Evt1ConfigurationWarning = 0x20000000
    };
    Q_DECLARE_FLAGS(Evt1Flags, Evt1)
    Q_FLAG(Evt1)

    explicit SunSpecBatteryModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecBatteryModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    /* Nameplate charge capacity in amp-hours. [Ah] */
    float nameplateChargeCapacity() const;
    /* Nameplate energy capacity in DC watt-hours. [Wh] */
    float nameplateEnergyCapacity() const;
    /* Maximum rate of energy transfer into the storage device in DC watts. [W] */
    float nameplateMaxChargeRate() const;
    /* Maximum rate of energy transfer out of the storage device in DC watts. [W] */
    float nameplateMaxDischargeRate() const;
    /* Self discharge rate.  Percentage of capacity (WHRtg) discharged per day. [%WHRtg] */
    float selfDischargeRate() const;
    /* Manufacturer maximum state of charge, expressed as a percentage. [%WHRtg] */
    float nameplateMaxSoC() const;
    /* Manufacturer minimum state of charge, expressed as a percentage. [%WHRtg] */
    float nameplateMinSoC() const;

    /* Setpoint for maximum reserve for storage as a percentage of the nominal maximum storage. [%WHRtg] */
    float maxReservePercent() const;
    QModbusReply *setMaxReservePercent(float maxReservePercent);

    /* Setpoint for minimum reserve for storage as a percentage of the nominal maximum storage. [%WHRtg] */
    float minReservePercent() const;
    QModbusReply *setMinReservePercent(float minReservePercent);

    /* State of charge, expressed as a percentage. [%WHRtg] */
    float stateOfCharge() const;
    /* Depth of discharge, expressed as a percentage. [%] */
    float depthOfDischarge() const;
    /* Percentage of battery life remaining. [%] */
    float stateOfHealth() const;
    /* Number of cycles executed in the battery. */
    quint32 cycleCount() const;
    /* Charge status of storage device. Enumeration. */
    Chast chargeStatus() const;
    /* Battery control mode. Enumeration. */
    Locremctl controlMode() const;
    /* Value is incremented every second with periodic resets to zero. */
    quint16 batteryHeartbeat() const;
    /* Value is incremented every second with periodic resets to zero. */
    quint16 controllerHeartbeat() const;
    QModbusReply *setControllerHeartbeat(quint16 controllerHeartbeat);

    /* Used to reset any latched alarms.  1 = Reset. */
    quint16 alarmReset() const;
    QModbusReply *setAlarmReset(quint16 alarmReset);

    /* Type of battery. Enumeration. */
    Typ batteryType() const;
    /* State of the battery bank.  Enumeration. */
    State stateOfTheBatteryBank() const;
    /* Vendor specific battery bank state.  Enumeration. */
    quint16 vendorBatteryBankState() const;
    /* Date the device warranty expires. */
    quint32 warrantyDate() const;
    /* Alarms and warnings.  Bit flags. */
    Evt1Flags batteryEvent1Bitfield() const;
    /* Alarms and warnings.  Bit flags. */
    quint32 batteryEvent2Bitfield() const;
    /* Vendor defined events. */
    quint32 vendorEventBitfield1() const;
    /* Vendor defined events. */
    quint32 vendorEventBitfield2() const;
    /* DC Bus Voltage. [V] */
    float externalBatteryVoltage() const;
    /* Instantaneous maximum battery voltage. [V] */
    float maxBatteryVoltage() const;
    /* Instantaneous minimum battery voltage. [V] */
    float minBatteryVoltage() const;
    /* Maximum voltage for all cells in the bank. [V] */
    float maxCellVoltage() const;
    /* String containing the cell with maximum voltage. */
    quint16 maxCellVoltageString() const;
    /* Module containing the cell with maximum voltage. */
    quint16 maxCellVoltageModule() const;
    /* Minimum voltage for all cells in the bank. [V] */
    float minCellVoltage() const;
    /* String containing the cell with minimum voltage. */
    quint16 minCellVoltageString() const;
    /* Module containing the cell with minimum voltage. */
    quint16 minCellVoltageModule() const;
    /* Average cell voltage for all cells in the bank. [V] */
    float averageCellVoltage() const;
    /* Total DC current flowing to/from the battery bank. [A] */
    float totalDcCurrent() const;
    /* Instantaneous maximum DC charge current. [A] */
    float maxChargeCurrent() const;
    /* Instantaneous maximum DC discharge current. [A] */
    float maxDischargeCurrent() const;
    /* Total power flowing to/from the battery bank. [W] */
    float totalPower() const;
    /* Request from battery to start or stop the inverter.  Enumeration. */
    Reqinvstate inverterStateRequest() const;
    /* AC Power requested by battery. [W] */
    float batteryPowerRequest() const;
    /* Instruct the battery bank to perform an operation such as connecting.  Enumeration. */
    Setop setOperation() const;
    QModbusReply *setSetOperation(Setop setOperation);

    /* Set the current state of the inverter. */
    Setinvstate setInverterState() const;
    QModbusReply *setSetInverterState(Setinvstate setInverterState);

    /* Scale factor for charge capacity. */
    qint16 aHRtg_SF() const;
    /* Scale factor for energy capacity. */
    qint16 wHRtg_SF() const;
    /* Scale factor for maximum charge and discharge rate. */
    qint16 wChaDisChaMax_SF() const;
    /* Scale factor for self discharge rate. */
    qint16 disChaRte_SF() const;
    /* Scale factor for state of charge values. */
    qint16 soC_SF() const;
    /* Scale factor for depth of discharge. */
    qint16 doD_SF() const;
    /* Scale factor for state of health. */
    qint16 soH_SF() const;
    /* Scale factor for DC bus voltage. */
    qint16 v_SF() const;
    /* Scale factor for cell voltage. */
    qint16 cellV_SF() const;
    /* Scale factor for DC current. */
    qint16 a_SF() const;
    /* Scale factor for instantaneous DC charge/discharge current. */
    qint16 aMax_SF() const;
    /* Scale factor for AC power request. */
    qint16 w_SF() const;

protected:
    quint16 m_fixedBlockLength = 62;

    void initDataPoints();
    void processBlockData() override;

private:
    float m_nameplateChargeCapacity = 0;
    float m_nameplateEnergyCapacity = 0;
    float m_nameplateMaxChargeRate = 0;
    float m_nameplateMaxDischargeRate = 0;
    float m_selfDischargeRate = 0;
    float m_nameplateMaxSoC = 0;
    float m_nameplateMinSoC = 0;
    float m_maxReservePercent = 0;
    float m_minReservePercent = 0;
    float m_stateOfCharge = 0;
    float m_depthOfDischarge = 0;
    float m_stateOfHealth = 0;
    quint32 m_cycleCount = 0;
    Chast m_chargeStatus;
    Locremctl m_controlMode;
    quint16 m_batteryHeartbeat = 0;
    quint16 m_controllerHeartbeat = 0;
    quint16 m_alarmReset = 0;
    Typ m_batteryType;
    State m_stateOfTheBatteryBank;
    quint16 m_vendorBatteryBankState = 0;
    quint32 m_warrantyDate = 0;
    Evt1Flags m_batteryEvent1Bitfield;
    quint32 m_batteryEvent2Bitfield = 0;
    quint32 m_vendorEventBitfield1 = 0;
    quint32 m_vendorEventBitfield2 = 0;
    float m_externalBatteryVoltage = 0;
    float m_maxBatteryVoltage = 0;
    float m_minBatteryVoltage = 0;
    float m_maxCellVoltage = 0;
    quint16 m_maxCellVoltageString = 0;
    quint16 m_maxCellVoltageModule = 0;
    float m_minCellVoltage = 0;
    quint16 m_minCellVoltageString = 0;
    quint16 m_minCellVoltageModule = 0;
    float m_averageCellVoltage = 0;
    float m_totalDcCurrent = 0;
    float m_maxChargeCurrent = 0;
    float m_maxDischargeCurrent = 0;
    float m_totalPower = 0;
    Reqinvstate m_inverterStateRequest;
    float m_batteryPowerRequest = 0;
    Setop m_setOperation;
    Setinvstate m_setInverterState;
    qint16 m_aHRtg_SF = 0;
    qint16 m_wHRtg_SF = 0;
    qint16 m_wChaDisChaMax_SF = 0;
    qint16 m_disChaRte_SF = 0;
    qint16 m_soC_SF = 0;
    qint16 m_doD_SF = 0;
    qint16 m_soH_SF = 0;
    qint16 m_v_SF = 0;
    qint16 m_cellV_SF = 0;
    qint16 m_a_SF = 0;
    qint16 m_aMax_SF = 0;
    qint16 m_w_SF = 0;


};

QDebug operator<<(QDebug debug, SunSpecBatteryModel *model);

#endif // SUNSPECBATTERYMODEL_H
