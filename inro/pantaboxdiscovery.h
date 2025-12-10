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

#ifndef PANTABOXDISCOVERY_H
#define PANTABOXDISCOVERY_H

#include <QTimer>
#include <QObject>

#include "pantabox.h"
#include "pantaboxudpdiscovery.h"

class PantaboxDiscovery : public QObject
{
    Q_OBJECT
public:
    explicit PantaboxDiscovery(QObject *parent = nullptr);

    typedef struct Result {
        PantaboxUdpDiscovery::DeviceInfo deviceInfo;
        QString modbusTcpVersion;
    } Result;

    QList<PantaboxDiscovery::Result> results() const;

public slots:
    void startDiscovery();

signals:
    void discoveryFinished();

private:
    PantaboxUdpDiscovery *m_discovery = nullptr;
    quint16 m_port = 502;
    quint16 m_modbusAddress = 1;
    QDateTime m_startDateTime;
    QTimer m_discoveryTimer;

    QList<Pantabox *> m_connections;
    QList<QHostAddress> m_alreadyCheckedHosts;

    QList<Result> m_results;

    void checkNetworkDevice(const PantaboxUdpDiscovery::DeviceInfo &deviceInfo);
    void cleanupConnection(Pantabox *connection);

    void finishDiscovery();
    void addResult(Pantabox *connection, const PantaboxUdpDiscovery::DeviceInfo &deviceInfo);
};

#endif // PANTABOXDISCOVERY_H
