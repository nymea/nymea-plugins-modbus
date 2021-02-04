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
        ChargingStatusOff = 1,
        ChargingStatusEmpty,
        ChargingStatusDischarging,
        ChargingStatusCharging,
        ChargingStatusFull,
        ChargingStatusHolding,
        ChargingStatusTesting
    };
    Q_ENUM(ChargingStatus)

    enum Model124 { // Mandatory registers
        Model124WChaMax      = 0,
        Model124WChaGra      = 1,
        Model124WDisChaGra   = 2,
        Model124StorCtl_Mod  = 3,
        Model124WChaMax_SF        = 16,
        Model124WChaDisChaGra_SF = 17,
    };

    enum Model124Optional { // Optional registers
        Model124VAChaMax                  = 4,
        Model124MinRsvPct                 = 5,
        Model124ChaState                  = 6,
        Model124StorAval                  = 7,
        Model124InBatV                    = 8,
        Model124ChaSt                     = 9,
        Model124OutWRte                   = 10,
        Model124InWRte                    = 11,
        Model124InOutWRte_WinTms          = 12,
        Model124InOutWRte_RvrtTms         = 13,
        Model124InOutWRte_RmpTms          = 14,
        Model124ChaGriSet                 = 15,
        Model124VAChaMax_SF               = 18,
        Model124MinRsvPct_SF              = 19,
        Model124ChaState_SF               = 20,
        Model124StorAval_SF               = 21,
        Model124InBatV_SF                 = 22,
        Model124InOutWRte_SF              = 23
    };
    Q_ENUM(Model124Optional)

    struct StorageData {
        double WChaMax;         // [W] Setpoint for maximum charge.
        double WChaGra;         // [%] Setpoint for maximum charging rate. Default is MaxChaRte.
        double WDisChaGra;      // [%] Setpoint for maximum discharge rate. Default is MaxDisChaRte.
        bool StorCtl_Mod_ChargingEnabled;
        bool StorCtl_Mod_DischargingEnabled;
    };

    struct StorageDataOptional {
        double VAChaMax;        // [VA] Setpoint for maximum charging VA.
        double MinRsvPct;       // [%]Setpoint for minimum reserve for storage as a percentage of the nominal maximum storage.
        double ChaState;        // [%] Currently available energy as a percent of the capacity rating.
        double StorAval;        // [Ah] State of charge (ChaState) minus storage reserve (MinRsvPct) times capacity rating (AhrRtg).
        double InBatV;          // [V] Internal battery voltage.
        ChargingStatus ChaSt;   // Charge status of storage device. Enumerated value.
        double OutWRte;         // [%] Percent of max discharge rate.
        double InWRte;          // [%] Percent of max charging rate.
        uint InOutWRte_WinTms;  // [s] Time window for charge/discharge rate change.
        uint InOutWRte_RvrtTms; // [s] Timeout period for charge/discharge rate.
        uint InOutWRte_RmpTms;  // [s] Ramp time for moving from current setpoint to new setpoint.
        GridCharge ChaGriSet;   // 0 = PV, 1 = Grid
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
