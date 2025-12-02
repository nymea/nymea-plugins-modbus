// SPDX-License-Identifier: LGPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of libnymea-modbus.
*
* libnymea-modbus is free software: you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* as published by the Free Software Foundation, either version 3
* of the License, or (at your option) any later version.
*
* libnymea-modbus is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with libnymea-modbus. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef MODBUSTCPCONNECTION_H
#define MODBUSTCPCONNECTION_H

#include <QObject>

class ModbusTcpMaster;

class ModbusTcpConnection : public QObject
{
    Q_OBJECT
public:
    explicit ModbusTcpConnection(QObject *parent = nullptr) : QObject{parent} { }
    virtual ~ModbusTcpConnection() = default;

    virtual ModbusTcpMaster *modbusTcpMaster() const = 0;

    virtual bool reachable() const = 0;
    virtual bool initializing() const = 0;

    virtual bool initialize() = 0;
    virtual bool update() = 0;

public slots:
    virtual bool connectDevice() = 0;
    virtual void disconnectDevice() = 0;
    virtual bool reconnectDevice() = 0;

signals:
    void reachableChanged(bool reachable);
    void initializationFinished(bool success);

};

#endif // MODBUSTCPCONNECTION_H
