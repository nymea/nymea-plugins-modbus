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

#ifndef SUNSPECSTORAGE_H
#define SUNSPECSTORAGE_H

#include <QObject>
#include "sunspec.h"

class SunSpecStorage : public QObject
{
    Q_OBJECT
public:
    SunSpecStorage(SunSpec *sunspec, SunSpec::ModelId modelId, int modbusAddress);

    SunSpec::ModelId modelId();
    void init();
    void getStorageModelHeader();
    void getStorageModelDataBlock();

    QUuid setGridCharging(bool enabled);
    QUuid setDischargingRate(int rate);
    QUuid setChargingRate(int rate);
    QUuid setStorageControlMode(bool chargingEnabled, bool dischargingEnabled);

    enum StorageControl {
        StorageControlHold      = 0,
        StorageControlCharge    = 1,
        StorageControlDischarge = 2,
    };
    Q_ENUM(StorageControl)

    enum GridCharge {
        PV    = 0,
        Grid  = 1
    };
    Q_ENUM(GridCharge)

    enum ChargingStatus {
        ChargingStatusOff,
        ChargingStatusEmpty,
        ChargingStatusDischarging,
        ChargingStatusCharging,
        ChargingStatusFull,
        ChargingStatusHolding,
        ChargingStatusTesting
    };
    Q_ENUM(ChargingStatus)

    enum Model124 { // Mandatory registers
        Model124SetpointMaximumCharge           = 0,
        Model124SetpointMaximumChargingRate     = 1,
        Model124SetpointMaximumDischargeRate    = 2,
        Model124ActivateStorageControlMode      = 3,
        Model124ScaleFactorMaximumCharge        = 16,
        Model124ScaleFactorMaximumChargeDischargeRate = 17,
    };
    Q_ENUM(Model124)

    enum Model124Optional { // Optional registers
        Model124MaximumChargingVA               = 4, // VAChaMax
        Model124MinimumReserveStoragePercent    = 5, // MinRsvPct
        Model124CurrentlyAvailableEnergyPercent = 6, // ChaState
        Model124StorageAvailableAH              = 7, // StorAval
        Model124InternalBatteryVoltage          = 8, // InBatV
        Model124ChargeStatus                    = 9, // ChaSt
        Model124MaxDischargingRatePercent       = 10, // OutWRte
        Model124MaxChargingRatePercent          = 11,
        Model124ChargeDischargeTimeWindow       = 12,
        Model124ChargeDischargeTimeout          = 13,
        Model124RampTime                        = 14, // InOutWRte_RmpTms
        Model124ChargeGridSet                   = 15,  // ChGriSet
        Model124ScaleFactorMaximumChargingVA    = 18,
        Model124ScaleFactorMinimumReservePercentage = 19,
        Model124ScaleFactorAvailableEnergyPercent = 20,
        Model124ScaleFactorStateCharge          = 21,
        Model124ScaleFactorBatteryVoltage       = 22,
        Model124ScaleFactorPercentChargeDischargeRate = 23
    };
    Q_ENUM(Model124Optional)

    struct StorageData {
        double maxCharge; // [W] Setpoint for maximum charge.
        double maxChargeRate; // [%] Setpoint for maximum charging rate. Default is MaxChaRte.
        double maxDischargeRate; // [%] Setpoint for maximum discharge rate. Default is MaxDisChaRte.
        bool chargingEnabled;
        bool dischargingEnabled;
    };

    struct StorageDataOptional {
        // [VA] Setpoint for maximum charging VA.
        // [& ]Setpoint for minimum reserve for storage as a percentage of the nominal maximum storage.
        double currentlyAvailableEnergy; // [%] Currently available energy as a percent of the capacity rating.
        double storageAvailable; // [Ah] State of charge (ChaState) minus storage reserve (MinRsvPct) times capacity rating (AhrRtg).
        double batteryVoltage; // [V] Internal battery voltage.
        ChargingStatus chargeSatus; // Charge status of storage device. Enumerated value.
        // [%] Percent of max discharge rate.
        // [%] Percent of max charging rate.
        // [s] Time window for charge/discharge rate change.
        // [s] Timeout period for charge/discharge rate.
        // [s] Ramp time for moving from current setpoint to new setpoint.
        bool gridChargingEnabled;
    };

private:
    SunSpec *m_connection = nullptr;
    SunSpec::ModelId m_id = SunSpec::ModelIdEnergyStorageBaseModel;
    uint m_modelLength = 0;
    uint m_modelModbusStartRegister = 40000;
    bool m_initFinishedSuccess = false;

private slots:
    void onModelDataBlockReceived(SunSpec::ModelId modelId, uint length, const QVector<quint16> &data);

signals:
    void initFinished(bool success);
    void storageDataReceived(const StorageData &mandatory, const StorageDataOptional &optional);
};

#endif // SUNSPECSTORAGE_H
