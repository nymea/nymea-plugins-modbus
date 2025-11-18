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

#ifndef AMTRONECU_H
#define AMTRONECU_H

#include <QObject>

#include "amtronecumodbustcpconnection.h"

class AmtronECU : public AmtronECUModbusTcpConnection
{
    Q_OBJECT
public:
    enum Version {
        VersionUnknown,
        VersionOld, // < 5.22
        VersionNew // >= 5.22
    };
    Q_ENUM(Version)

    explicit AmtronECU(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent = nullptr);
    explicit AmtronECU(ModbusTcpMaster *modbusTcpMaster, quint16 slaveId, QObject *parent = nullptr);

    bool initialize() override;
    bool update() override;

    Version detectedVersion() const;

private:
    Version m_detectedVersion = VersionUnknown;

};

QDebug operator<<(QDebug debug, AmtronECU *amtronECU);

#endif // AMTRONECU_H
