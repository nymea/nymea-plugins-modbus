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

#ifndef SPEEDWIREINVERTERREPLY_H
#define SPEEDWIREINVERTERREPLY_H

#include <QObject>
#include <QTimer>

#include "speedwireinverterrequest.h"

class SpeedwireInverterReply : public QObject
{
    Q_OBJECT

    friend class SpeedwireInverter;

public:
    enum Error {
        ErrorNoError,       // Response on, no error
        ErrorInverterError, // Inverter returned error
        ErrorTimeout        // Request timeouted
    };
    Q_ENUM(Error)

    // Request
    SpeedwireInverterRequest request() const;

    Error error() const;

    // Response
    QByteArray responseData() const;
    Speedwire::Header responseHeader() const;
    Speedwire::InverterPacket responsePacket() const;
    QByteArray responsePayload() const;

signals:
    void finished();
    void timeout();

private:
    explicit SpeedwireInverterReply(const SpeedwireInverterRequest &request, QObject *parent = nullptr);

    QTimer m_timer;
    Error m_error = ErrorNoError;
    SpeedwireInverterRequest m_request;
    quint8 m_retries = 0;
    quint8 m_maxRetries = 3;
    int m_timeout = 3000;

    QByteArray m_responseData;
    Speedwire::Header m_responseHeader;
    Speedwire::InverterPacket m_responsePacket;
    QByteArray m_responsePayload;


    void finishReply(Error error);
    void startWaiting();

};

#endif // SPEEDWIREINVERTERREPLY_H
