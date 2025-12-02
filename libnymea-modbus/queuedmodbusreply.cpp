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

#include "queuedmodbusreply.h"

QueuedModbusReply::QueuedModbusReply(QObject *parent)
    : QObject{parent}
{

}

QueuedModbusReply::QueuedModbusReply(RequestType requestType, QModbusDataUnit dataUnit, QObject *parent)
    : QObject{parent},
    m_requestType{requestType},
    m_dataUnit{dataUnit}
{

}

QueuedModbusReply::~QueuedModbusReply()
{
    if (m_reply)
        m_reply->deleteLater();
}

QueuedModbusReply::RequestType QueuedModbusReply::requestType() const
{
    return m_requestType;
}

void QueuedModbusReply::setRequestType(RequestType requestType)
{
    Q_ASSERT_X(m_reply == nullptr, "QueuedModbusReply", "Changing the request type after the modbus reply has already been sent has no effect.");
    m_requestType = requestType;
}

QModbusDataUnit QueuedModbusReply::dataUnit() const
{
    return m_dataUnit;
}

void QueuedModbusReply::setQMOdbusDataUnit(const QModbusDataUnit &dataUnit)
{
    Q_ASSERT_X(m_reply == nullptr, "QueuedModbusReply", "Changing the data unit after the modbus reply has already been sent has no effect.");
    m_dataUnit = dataUnit;
}

QModbusReply *QueuedModbusReply::reply() const
{
    return m_reply;
}

void QueuedModbusReply::setReply(QModbusReply *reply)
{
    m_reply = reply;
    if (!m_reply)
        return;

    // Note: do not change the parent of the QModbusReply,
    // otherwise the modbus device looses track about it and stops working

    connect(m_reply, &QModbusReply::finished, this, &QueuedModbusReply::finished);
    connect(m_reply, &QModbusReply::errorOccurred, this, &QueuedModbusReply::errorOccurred);
}

QModbusDevice::Error QueuedModbusReply::error() const
{
    if (!m_reply)
        return QModbusDevice::UnknownError;

    return m_reply->error();
}

QString QueuedModbusReply::errorString() const
{
    if (!m_reply)
        return QString();

    return m_reply->errorString();
}
