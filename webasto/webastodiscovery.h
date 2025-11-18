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

#ifndef WEBASTODISCOVERY_H
#define WEBASTODISCOVERY_H

#include <QObject>

#include <network/networkdevicediscovery.h>
#include "webastonextmodbustcpconnection.h"

class WebastoDiscovery : public QObject
{
    Q_OBJECT
public:
    enum Type {
        TypeWebastoLive,
        TypeWebastoNext
    };
    Q_ENUM(Type)

    typedef struct Result {
        QString productName;
        Type type;
        QHostAddress address;
        NetworkDeviceInfo networkDeviceInfo;
    } Result;

    explicit WebastoDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent = nullptr);

    void startDiscovery();

    QList<Result> results() const;

signals:
    void discoveryFinished();

private:
    NetworkDeviceDiscovery *m_networkDeviceDiscovery = nullptr;
    QDateTime m_startDateTime;

    NetworkDeviceInfos m_networkDeviceInfos;
    QList<WebastoNextModbusTcpConnection *> m_connections;
    QList<Result> m_temporaryResults;
    QList<Result> m_results;

    void checkNetworkDevice(const QHostAddress &address);
    void cleanupConnection(WebastoNextModbusTcpConnection *connection);

    void finishDiscovery();
};

#endif // WEBASTODISCOVERY_H
