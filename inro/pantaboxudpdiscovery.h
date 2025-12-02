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

#ifndef PANTABOXUDPDISCOVERY_H
#define PANTABOXUDPDISCOVERY_H

#include <QObject>
#include <QUdpSocket>

#include <network/macaddress.h>

class PantaboxUdpDiscovery : public QObject
{
    Q_OBJECT
public:
    explicit PantaboxUdpDiscovery(QObject *parent = nullptr);

    typedef struct DeviceInfo {
        QString serialNumber;
        MacAddress macAddress;
        QHostAddress ipAddress;
    } DeviceInfo;

    bool available() const;

    QHash<QString, DeviceInfo> results() const;

signals:
    void pantaboxDiscovered(const PantaboxUdpDiscovery::DeviceInfo &deviceInfo);

private slots:
    void readPendingDatagrams();

private:
    QUdpSocket *m_socket = nullptr;
    bool m_available = false;

    QHash<QHostAddress, QByteArray> m_buffers;
    QHash<QHostAddress, bool> m_prefixStartDiscovered;

    quint8 calculateCrc8(const QByteArray &data);
    void processDataBuffer(const QHostAddress &address);

    QHash<QString, DeviceInfo> m_results;
};

#endif // PANTABOXUDPDISCOVERY_H
