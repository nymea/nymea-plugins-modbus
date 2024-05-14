/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2024, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea.
* This project including source code and documentation is protected by
* copyright law, and remains the property of nymea GmbH. All rights, including
* reproduction, publication, editing and translation, are reserved. The use of
* this project is subject to the terms of a license agreement to be concluded
* with nymea GmbH in accordance with the terms of use of nymea GmbH, available
* under https://nymea.io/license
*
* GNU Lesser General Public License Usage
* Alternatively, this project may be redistributed and/or modified under the
* terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; version 3. This project is distributed in the hope that
* it will be useful, but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this project. If not, see <https://www.gnu.org/licenses/>.
*
* For any further details and any questions please contact us under
* contact@nymea.io or see our FAQ/Licensing Information on
* https://nymea.io/license/faq
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
