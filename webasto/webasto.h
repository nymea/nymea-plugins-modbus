/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea.
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

#ifndef WEBASTO_H
#define WEBASTO_H

#include <QObject>
#include <QHostAddress>
#include <QTimer>
#include <QModbusTcpClient>
#include <QUuid>

class Webasto : public QObject
{
    Q_OBJECT
public:
    enum ChargePointState {
        ChargePointStateNoVehicleAttached = 0,
        ChargePointStateVehicleAttachedNoPermission,
        ChargePointStateChargingAuthorized,
        ChargePointStateCharging,
        ChargePointStateChargingPaused,
        ChargePointStateChargeSuccessfulCarStillAttached,
        ChargePointStateChargingStoppedByUserCarStillAttached,
        ChargePointStateChargingErrorCarStillAttached,
        ChargePointStateChargingStationReservedNorCarAttached,
        ChargePointStateUserNotAuthorizedCarAttached
    };
    Q_ENUM(ChargePointState)

    enum CableState {
        CableStateNoCableAttached = 0,
        CableStateCableAttachedNoCarAttached,
        CableStateCableAttachedCarAttached,
        CableStateCableAttachedCarAttachedLockActive
    };
    Q_ENUM(CableState)

    enum EvseSate {
        EvseSateStarting = 0,
        EvseSateRunning,
        EvseSateError
    };
    Q_ENUM(EvseSate)

    struct BasicInformation {
        double currentL1; // [A]
        double currentL2; // [A]
        double currentL3; // [A]
        double activePower; // [W]
        uint activePowerL1;
        uint activePowerL2;
        uint activePowerL3;
        double energyMeter; // [kWh]
    };

    struct CurrentLimitations {
        double maxCurrent;
        double minimumCurrentLimit;
        double maxCurrentFromEvse;
        double maxCurrentFromCable;
        double maxCurrentFromEV;
    };

    explicit Webasto(const QHostAddress &address, uint port = 502, QObject *parent = nullptr);

    void setAddress(const QHostAddress &address);
    QHostAddress address() const;
    bool connected();

private:
    enum TqModbusRegister {
        // Get Basic Information polls Register 1000 to 1037
        TqChargePointState          = 1000, // State of the charging device
        TqChargeState               = 1001, // Charging
        TqEVSEState                 = 1002, // State of the charging station
        TqCableState                = 1004, // State of the charging cable
        TqEVSEError                 = 1006, // Error code of the charging station
        TqCurrentL1                 = 1008, // Charging current L1
        TqCurrentL2                 = 1010, // Charging current L2
        TqCurrentL3                 = 1012, // Charging current L3
        TqActivePower               = 1020, // Electric Power that can be changed to f.e. mechanical, chemical, thermic power
        TqActivePowerL1             = 1024, // Active power L1
        TqActivePowerL2             = 1028, // Active power L2
        TqActivePowerL3             = 1032, // Active power L3
        TqEnergyMeter               = 1036, // Meter reading of the charging station
        // Get Current Limitatoins polls register 1100 to 1110
        TqMaxCurrent                = 1100, // Maximal charging current UINT of the hardware (EVSE, cable, EV)
        TqMinimumCurrentLimit       = 1102, // Minimal charging current of the hardware (EVSE, cable, EV)
        TqMaxCurrentFromEVSE        = 1104, // Maximal charging current of the charging station
        TqMaxCurrentFromCable       = 1106, // Maximal charging current of the cable
        TqMaxCurrentFromEV          = 1108, // Maximal charging current of the EV
        // Get User Priority
        TqUserPriority              = 1200, // Priorities of the user 0: not defined 1: high priority - 10: low priority
        // Get Battery state
        TqEVBatteryState            = 1300, // Returns an estimate of the SoC
        TqEVBatteryCapacity         = 1302, // Returns an estimate of the EV Battery Capacity
        // Get Schedule polls register 1400 to 1414
        TqScheduleType              = 1400, // Type/information of traveling 0: energy that has to be charged, 1: Specification of the desired battery charge (Needs: state of the battery)
        TqRequiredEnergy            = 1402, // Desired energy
        TqRequiredBatteryState      = 1404, // Desired state of the battery
        TqScheduledTime             = 1408, // Departure time
        TqScheduledDate             = 1412, // Departure date
        // Set session polls register 1500 to 15014
        TqChargedEnergy             = 1502, // Sum of charged energy for the current session
        TqStartTime                 = 1504, // Start time of charging process
        TqChargingTime              = 1508, // Duration since beginning of charge
        TqEndTime                   = 1512, // End time of charging process
        // Get user id polls register 1600 to 1620
        TqUserId                    = 1600, // 24 Bytes long User ID (OCPP IdTag) from the current session
        TqSmartVehicleDetected      = 1620, //Returns 1 if an EV currently connected is a smart vehicle, or 0 if no EV connected or it is not a smart vehicle,
        // Get failsafe polls register 2000 to 2004
        TqSafeCurrent               = 2000, // Max. charge current under communication failure
        TqComTimeout                = 2002, // Communication timeout
        // Get Charge power polls register 5000 to 5002
        TqChargePower               = 5000, // Charge power
        TqChargeCurrent             = 5001, // Charge current
        TqLifeBit                   = 6000  // Communication monitoring 0/1 Toggle-Bit EM writes 1, Live deletes it and puts it on 0.
    };

    QModbusTcpClient *m_modbusConnection = nullptr;
    QHostAddress m_address;
    uint m_unitId = 255;

    void getBasicInformation();
    void getUserId();
    void getCurrentLimitations();
    void getSessionInformation();
    void getFailsafeSpecs();
    void getChargeCurrentAndPower();
    void getUserPriority();
    void getBatteryState();
    void setLiveBit();

private:
    QTimer *m_lifeBitTimer = nullptr;
    QUuid writeHoldingRegister();

signals:
    void connectionChanged(bool connected);
    void userIdReceived(const QByteArray &userId);
    void currentLimitationsReceived(const CurrentLimitations &limitations);
    void userPriorityReceived(uint userPriority); // 0 lowest - 10 highest
};

#endif // WEBASTO_H
