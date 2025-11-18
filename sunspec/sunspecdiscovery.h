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

#ifndef SUNSPECDISCOVERY_H
#define SUNSPECDISCOVERY_H

#include <QQueue>
#include <QObject>
#include <QDateTime>

#include <sunspecconnection.h>
#include <network/networkdevicediscovery.h>

class SunSpecDiscovery : public QObject
{
    Q_OBJECT
public:
    explicit SunSpecDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, quint16 slaveId, SunSpecDataPoint::ByteOrder byteOrder = SunSpecDataPoint::ByteOrderLittleEndian, QObject *parent = nullptr);
    typedef struct Result {
        QHostAddress address;
        NetworkDeviceInfo networkDeviceInfo;
        quint16 port;
        quint16 slaveId;
        QStringList modelManufacturers;
    } Result;

    QList<Result> results() const;

    void addCustomDiscoveryPort(quint16 port);
    void startDiscovery();

signals:
    void discoveryFinished();

private:
    NetworkDeviceDiscovery *m_networkDeviceDiscovery = nullptr;
    QList<quint16> m_scanPorts;
    quint16 m_slaveId = 1;
    SunSpecDataPoint::ByteOrder m_byteOrder;

    QDateTime m_startDateTime;
    QHash<QHostAddress, QQueue<SunSpecConnection *>> m_pendingConnectionAttempts;
    QHash<SunSpecConnection *, QTimer *> m_connectionTimers;
    QList<SunSpecConnection *> m_connections;
    NetworkDeviceInfos m_networkDeviceInfos;
    QList<Result> m_results;

    void testNextConnection(const QHostAddress &address);

    void checkNetworkDevice(const QHostAddress &address);
    void cleanupConnection(SunSpecConnection *connection);

    void finishDiscovery();
};

#endif // SUNSPECDISCOVERY_H
