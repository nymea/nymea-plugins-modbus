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

#ifndef ENERGYCONTROLDISCOVERY_H
#define ENERGYCONTROLDISCOVERY_H

#include <QObject>
#include <QTimer>

#include <hardware/modbus/modbusrtuhardwareresource.h>

class EnergyControlDiscovery : public QObject
{
    Q_OBJECT
public:
    explicit EnergyControlDiscovery(ModbusRtuHardwareResource *modbusRtuResource, QObject *parent = nullptr);
    struct Result {
        QUuid modbusRtuMasterId;
        quint16 firmwareVersion;
        quint16 slaveId;
    };

    void startDiscovery();

    QList<Result> discoveryResults() const;

signals:
    void discoveryFinished(bool modbusRtuMasterAvailable);

private slots:
    void tryConnect(ModbusRtuMaster *master, quint16 slaveId);

private:
    ModbusRtuHardwareResource *m_modbusRtuResource = nullptr;

    QList<Result> m_discoveryResults;
};

#endif // ENERGYCONTROLDISCOVERY_H
