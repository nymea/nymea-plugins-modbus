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

#ifndef SPEEDWIREINVERTERREQUEST_H
#define SPEEDWIREINVERTERREQUEST_H

#include <QObject>

#include "speedwire.h"

class SpeedwireInverterRequest
{
public:
    explicit SpeedwireInverterRequest();

    Speedwire::Command command() const;
    void setCommand(Speedwire::Command command);

    quint16 packetId() const;
    void setPacketId(quint16 packetId);

    QByteArray requestData() const;
    void setRequestData(const QByteArray &requestData);

    quint8 retries() const;
    void setRetries(quint8 retries);

private:
    Speedwire::Command m_command;
    quint16 m_packetId = 0;
    QByteArray m_requestData;
    quint8 m_retries = 2; // Default try 2 times before timeout
};

#endif // SPEEDWIREINVERTERREQUEST_H
