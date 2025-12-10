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

#ifndef SUNNYWEBBOXDISCOVERY_H
#define SUNNYWEBBOXDISCOVERY_H

#include <QObject>

#include <network/networkaccessmanager.h>
#include <network/networkdevicediscovery.h>

class SunnyWebBoxDiscovery : public QObject
{
    Q_OBJECT
public:
    explicit SunnyWebBoxDiscovery(NetworkAccessManager *networkAccessManager, NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent = nullptr);

    void startDiscovery();

    NetworkDeviceInfos discoveryResults() const;

signals:
    void discoveryFinished();

private slots:
    void checkNetworkDevice(const QHostAddress &address);
    void cleanupPendingReplies();
    void finishDiscovery();

private:
    NetworkAccessManager *m_networkAccessManager = nullptr;
    NetworkDeviceDiscovery *m_networkDeviceDiscovery = nullptr;
    NetworkDeviceDiscoveryReply *m_discoveryReply = nullptr;

    QList<QHostAddress> m_discoveredHosts;
    NetworkDeviceInfos m_discoveryResults;
    NetworkDeviceInfos m_networkDeviceInfos;

    QDateTime m_startDateTime;
    QList<QNetworkReply *> m_pendingReplies;

};

#endif // SUNNYWEBBOXDISCOVERY_H
