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

class SunSpecStorage : public SunSpec
{
    Q_OBJECT
public:
    SunSpecStorage(const QHostAddress &hostAddress, uint port = 502, QObject *parent = 0);

    QUuid setGridCharging(bool enabled);
    QUuid setDischargingRate(int rate);
    QUuid setChargingRate(int rate);
    QUuid setStorageControlMode(bool chargingEnabled, bool dischargingEnabled);


    enum StorageControlBitField {
        StorageControlBitFieldCharge    = 0,
        StorageControlBitFieldDischarge = 1
    };
    Q_ENUM(StorageControlBitField)

    enum GridCharge {
        PV    = 0,
        Grid  = 1
    };
    Q_ENUM(GridCharge)

    enum ChargingState {
        ChargingStateOff,
        ChargingStateEmpty,
        ChargingStateDischarging,
        ChargingStateCharging,
        ChargingStateFull,
        ChargingStateHolding,
        ChargingStateTesting
    };
    Q_ENUM(ChargingState)

    enum Model124 { // Mandatory register
        Model124SetpointMaximumCharge           = 0,
        Model124SetpointMaximumChargingRate     = 1,
        Model124SetpointMaximumDischargeRate    = 2,
        Model124ActivateStorageControlMode      = 3,
        Model124CurrentlyAvailableEnergy        = 6,
        Model124ChargeStatus                    = 9,
        Model124ChaGriSet                       = 15,
        Model124ScaleFactorMaximumCharge        = 16,
        Model124ScaleFactorMaximumChargeDischargeRate = 17,
        Model124ScaleFactorAvailableEnergyPercent = 20
    };
    Q_ENUM(Model124)

    struct StorageData {
        ChargingState chargingState;
        bool gridChargingEnabled;
    };

    void getStorageMap();

private:
    BlockId m_id = BlockId::EnergyStorageBaseModel;
    uint m_mapLength = 0;
    uint m_mapModbusStartRegister = 40000;

    void readStorageBlockHeader();

private slots:
    void onConnectionStateChanged();
    void onModbusMapReceived(BlockId mapId, uint mapLength, const QVector<quint16> &data);

signals:
    void initFinished();
    void storageDataReceived(const StorageData &data);
};

#endif // SUNSPECSTORAGE_H
