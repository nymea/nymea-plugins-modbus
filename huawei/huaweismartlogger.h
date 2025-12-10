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

#ifndef HUAWEISMARTLOGGER_H
#define HUAWEISMARTLOGGER_H

#include <QObject>

#include "huaweismartloggermodbustcpconnection.h"

class HuaweiSmartLogger : public HuaweiSmartLoggerModbusTcpConnection
{
    Q_OBJECT
public:
    explicit HuaweiSmartLogger(const QHostAddress &hostAddress, uint port, quint16 meterSlaveId, QObject *parent = nullptr);

    bool update() override;

signals:

private:
    quint16 m_meterSlaveId = 5;
};

#endif // HUAWEISMARTLOGGER_H
