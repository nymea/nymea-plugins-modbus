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

#ifndef QUEUEDMODBUSREPLY_H
#define QUEUEDMODBUSREPLY_H

#include <QObject>
#include <QModbusReply>
#include <QModbusDataUnit>

class QueuedModbusReply : public QObject
{
    Q_OBJECT
public:
    enum RequestType {
        RequestTypeRead,
        RequestTypeWrite
    };
    Q_ENUM(RequestType)

    explicit QueuedModbusReply(QObject *parent = nullptr);
    explicit QueuedModbusReply(RequestType requestType, QModbusDataUnit dataUnit, QObject *parent = nullptr);

    ~QueuedModbusReply();

    RequestType requestType() const;
    void setRequestType(RequestType requestType);

    QModbusDataUnit dataUnit() const;
    void setQMOdbusDataUnit(const QModbusDataUnit &dataUnit);

    // Available once the request will be sent to the modbus slave
    QModbusReply *reply() const;
    void setReply(QModbusReply *reply);

    QModbusDevice::Error error() const;
    QString errorString() const;

signals:
    void finished();
    void errorOccurred(QModbusDevice::Error error);

private:
    RequestType m_requestType = RequestTypeRead;
    QModbusDataUnit m_dataUnit;
    QModbusReply *m_reply = nullptr;

};

#endif // QUEUEDMODBUSREPLY_H
