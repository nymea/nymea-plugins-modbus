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

#ifndef WATTSONICINVERTER_H
#define WATTSONICINVERTER_H

#include <QObject>

#include "wattsonicmodbusrtuconnection.h"

class WattsonicInverter : public WattsonicModbusRtuConnection
{
    Q_OBJECT
public:
    struct Info {
        QString type;
        QString model;
    };

    enum InverterGenerationMode {
        InverterGenerationModeAuto,
        InverterGenerationModeGen2,
        InverterGenerationModeGen3
    };
    Q_ENUM(InverterGenerationMode)

    enum Generation {
        GenerationUnknown,
        Generation2,
        Generation3
    };
    Q_ENUM(Generation)

    explicit WattsonicInverter(ModbusRtuMaster *modbusRtuMaster, quint16 slaveId, InverterGenerationMode generationMode = InverterGenerationModeAuto, QObject *parent = nullptr);

    Generation generation() const;
    WattsonicInverter::Info inverterInfo() const;

    // Generation specific registers
    float batteryVoltageDc() const;
    BatteryMode batteryMode() const;
    qint32 batteryPower() const;
    float totalEnergyInjectedToGrid() const;
    float totalEnergyPurchasedFromGrid() const;
    float SOC() const;
    float SOH() const;

    bool update() override;

    static WattsonicInverter::Info getInverterInfo(quint16 equipmentInfo);

signals:
    void generationChanged();
    void customInitializationFinished(bool success);

private:
    WattsonicInverter::Info m_inverterInfo;
    InverterGenerationMode m_generationMode = InverterGenerationModeAuto;
    Generation m_generation = GenerationUnknown;

};

#endif // WATTSONICINVERTER_H
