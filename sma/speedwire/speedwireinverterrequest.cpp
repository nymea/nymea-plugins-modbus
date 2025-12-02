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

#include "speedwireinverterrequest.h"

SpeedwireInverterRequest::SpeedwireInverterRequest()
{

}

Speedwire::Command SpeedwireInverterRequest::command() const
{
    return m_command;
}

void SpeedwireInverterRequest::setCommand(Speedwire::Command command)
{
    m_command = command;
}

quint16 SpeedwireInverterRequest::packetId() const
{
    return m_packetId;
}

void SpeedwireInverterRequest::setPacketId(quint16 packetId)
{
    m_packetId = packetId;
}

QByteArray SpeedwireInverterRequest::requestData() const
{
    return m_requestData;
}

void SpeedwireInverterRequest::setRequestData(const QByteArray &requestData)
{
    m_requestData = requestData;
}

quint8 SpeedwireInverterRequest::retries() const
{
    return m_retries;
}

void SpeedwireInverterRequest::setRetries(quint8 retries)
{
    m_retries = retries;
}
