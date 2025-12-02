// SPDX-License-Identifier: GPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of nymea-plugins-modbus.
*
* nymea-plugins-modbus is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* nymea-plugins-modbus is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with nymea-plugins-modbus. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SOLAREDGEBATTERY_H
#define SOLAREDGEBATTERY_H

#include <QObject>
#include <QDebug>
#include <QTimer>

#include "sunspecthing.h"

/* Note: this is actually not a sunspec thing, since SolarEdge provide battery information using a non SunSpec modbus register section.
 * For keeping the code as simple as possible we make it a SunSpec thing */

class SolarEdgeBattery : public SunSpecThing
{
    Q_OBJECT
public:
    enum BatteryRegister {
        ManufacturerName = 0x00,
        Model = 0x10,
        FirmwareVersion = 0x20,
        SerialNumber = 0x30,
        BatteryDeviceId = 0x40,
        RatedEnergy = 0x42, // W * H
        MaxChargeContinuesPower = 0x44, // W
        MaxDischargeContinuesPower = 0x46, // W
        MaxChargePeakPower = 0x48, // W
        MaxDischargePeakPower = 0x4A, // W
        // 32 byte reserved
        BatteryAverageTemperature = 0x6c, // °C
        BatteryMaxTemperature = 0x6E,
        InstantaneousVoltage = 0x70, // V
        InstantaneousCurrent = 0x72, // A
        InstantaneousPower = 0x74, // W
        LifetimeExportEnergyCounter = 0x76, // W * H
        LifetimeImportEnergyCounter = 0x7A, // W * H
        MaxEnergy = 0x7E, // W * H
        AvailableEnergy = 0x80, // W * H
        StateOfHealth = 0x82,
        StateOfEnergy = 0x84,
        Status = 0x86
    };
    Q_ENUM(BatteryRegister)

    enum BatteryStatus {
        Off = 0,
        Standby = 1,
        Init = 2,
        Charge = 3,
        Discharge = 4,
        Fault = 5,
        Holding = 6,
        Idle = 7
    };
    Q_ENUM(BatteryStatus)

    struct BatteryData {
        QString manufacturerName;
        QString model;
        QString firmwareVersion;
        QString serialNumber;
        quint16 batteryDeviceId;
        float ratedEnergy;
        float maxChargeContinuesPower;
        float maxDischargeContinuesPower;
        float maxChargePeakPower;
        float maxDischargePeakPower;
        float averageTemperature;
        float maxTemperature;
        float instantaneousVoltage;
        float instantaneousCurrent;
        float instantaneousPower;
        quint64 lifetimeExportEnergyCounter;
        quint64 lifetimeImportEnergyCounter;
        float maxEnergy;
        float availableEnergy;
        float stateOfHealth;
        float stateOfEnergy;
        BatteryStatus batteryStatus;
    };

    explicit SolarEdgeBattery(Thing *thing, SunSpecConnection *connection, int modbusStartRegister, QObject *parent = nullptr);

    SunSpecConnection *connection() const override;

    quint16 modbusStartRegister() const override;

    BatteryData batteryData() const;

    void init();
    void readBlockData() override;

signals:
    void initFinished(bool success);
    void blockDataUpdated();

private:
    QTimer m_timer;
    SunSpecConnection *m_connection = nullptr;
    int m_modbusStartRegister;
    bool m_initFinishedSuccess = false;
    BatteryData m_batteryData;

};

QDebug operator<<(QDebug debug, const SolarEdgeBattery::BatteryData &batteryData);

#endif // SOLAREDGEBATTERY_H
