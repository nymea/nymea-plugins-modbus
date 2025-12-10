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

#ifndef PANTABOX_H
#define PANTABOX_H

#include "pantaboxmodbustcpconnection.h"

class Pantabox : public PantaboxModbusTcpConnection
{
    Q_OBJECT
public:
    explicit Pantabox(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent = nullptr);
    explicit Pantabox(ModbusTcpMaster *modbusTcpMaster, quint16 slaveId, QObject *parent = nullptr);
    ~Pantabox() override = default ;

    static QString modbusVersionToString(quint32 value);

    bool update() override;
};

#endif // PANTABOX_H
