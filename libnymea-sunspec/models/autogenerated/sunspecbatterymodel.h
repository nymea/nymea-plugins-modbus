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

#ifndef SUNSPECBATTERYMODEL_H
#define SUNSPECBATTERYMODEL_H

#include <QObject>

#include "sunspec.h"
#include "sunspecmodel.h"

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

    explicit SunSpecBatteryModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecBatteryModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    quint16 modelId() const;
    quint16 modelLength() const;
    float nameplateChargeCapacity() const;
    float nameplateEnergyCapacity() const;
    float nameplateMaxChargeRate() const;
    float nameplateMaxDischargeRate() const;
    float selfDischargeRate() const;
    float nameplateMaxSoC() const;
    float nameplateMinSoC() const;
    float maxReservePercent() const;
    float minReservePercent() const;
    float stateOfCharge() const;
    float depthOfDischarge() const;
    float stateOfHealth() const;
    quint32 cycleCount() const;
    Chast chargeStatus() const;
    Locremctl controlMode() const;
    quint16 batteryHeartbeat() const;
    quint16 controllerHeartbeat() const;
    quint16 alarmReset() const;
    Typ batteryType() const;
    State stateOfTheBatteryBank() const;
    quint16 vendorBatteryBankState() const;
    quint32 warrantyDate() const;
    Evt1Flags batteryEvent1Bitfield() const;
    quint32 batteryEvent2Bitfield() const;
    quint32 vendorEventBitfield1() const;
    quint32 vendorEventBitfield2() const;
    float externalBatteryVoltage() const;
    float maxBatteryVoltage() const;
    float minBatteryVoltage() const;
    float maxCellVoltage() const;
    quint16 maxCellVoltageString() const;
    quint16 maxCellVoltageModule() const;
    float minCellVoltage() const;
    quint16 minCellVoltageString() const;
    quint16 minCellVoltageModule() const;
    float averageCellVoltage() const;
    qint16 totalDcCurrent() const;
    float maxChargeCurrent() const;
    float maxDischargeCurrent() const;
    qint16 totalPower() const;
    Reqinvstate inverterStateRequest() const;
    qint16 batteryPowerRequest() const;
    Setop setOperation() const;
    Setinvstate setInverterState() const;

private:
    quint16 m_modelId;
    quint16 m_modelLength;
    float m_nameplateChargeCapacity;
    float m_nameplateEnergyCapacity;
    float m_nameplateMaxChargeRate;
    float m_nameplateMaxDischargeRate;
    float m_selfDischargeRate;
    float m_nameplateMaxSoC;
    float m_nameplateMinSoC;
    float m_maxReservePercent;
    float m_minReservePercent;
    float m_stateOfCharge;
    float m_depthOfDischarge;
    float m_stateOfHealth;
    quint32 m_cycleCount;
    Chast m_chargeStatus;
    Locremctl m_controlMode;
    quint16 m_batteryHeartbeat;
    quint16 m_controllerHeartbeat;
    quint16 m_alarmReset;
    Typ m_batteryType;
    State m_stateOfTheBatteryBank;
    quint16 m_vendorBatteryBankState;
    quint32 m_warrantyDate;
    Evt1Flags m_batteryEvent1Bitfield;
    quint32 m_batteryEvent2Bitfield;
    quint32 m_vendorEventBitfield1;
    quint32 m_vendorEventBitfield2;
    float m_externalBatteryVoltage;
    float m_maxBatteryVoltage;
    float m_minBatteryVoltage;
    float m_maxCellVoltage;
    quint16 m_maxCellVoltageString;
    quint16 m_maxCellVoltageModule;
    float m_minCellVoltage;
    quint16 m_minCellVoltageString;
    quint16 m_minCellVoltageModule;
    float m_averageCellVoltage;
    qint16 m_totalDcCurrent;
    float m_maxChargeCurrent;
    float m_maxDischargeCurrent;
    qint16 m_totalPower;
    Reqinvstate m_inverterStateRequest;
    qint16 m_batteryPowerRequest;
    Setop m_setOperation;
    Setinvstate m_setInverterState;
    qint16 m_aHRtg_SF;
    qint16 m_wHRtg_SF;
    qint16 m_wChaDisChaMax_SF;
    qint16 m_disChaRte_SF;
    qint16 m_soC_SF;
    qint16 m_doD_SF;
    qint16 m_soH_SF;
    qint16 m_v_SF;
    qint16 m_cellV_SF;
    qint16 m_a_SF;
    qint16 m_aMax_SF;
    qint16 m_w_SF;

    void initDataPoints();

};

#endif // SUNSPECBATTERYMODEL_H
