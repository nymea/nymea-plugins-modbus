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

#include "speedwireinverterreply.h"
#include "extern-plugininfo.h"

SpeedwireInverterReply::SpeedwireInverterReply(const SpeedwireInverterRequest &request, QObject *parent) :
    QObject(parent),
    m_request(request)
{
    m_maxRetries = m_request.retries();

    m_timer.setInterval(m_timeout);
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, &SpeedwireInverterReply::timeout);
}

SpeedwireInverterRequest SpeedwireInverterReply::request() const
{
    return m_request;
}

SpeedwireInverterReply::Error SpeedwireInverterReply::error() const
{
    return m_error;
}

QByteArray SpeedwireInverterReply::responseData() const
{
    return m_responseData;
}

Speedwire::Header SpeedwireInverterReply::responseHeader() const
{
    return m_responseHeader;
}

Speedwire::InverterPacket SpeedwireInverterReply::responsePacket() const
{
    return m_responsePacket;
}

QByteArray SpeedwireInverterReply::responsePayload() const
{
    return m_responsePayload;
}

void SpeedwireInverterReply::startWaiting()
{
    m_timer.start();
}

void SpeedwireInverterReply::finishReply(Error error)
{
    m_timer.stop();
    m_error = error;
    emit finished();
}
