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

#ifndef HUAWEISMARTLOGGERDISCOVERY_H
#define HUAWEISMARTLOGGERDISCOVERY_H

#include <QObject>

#include <network/networkdevicediscovery.h>

#include "huaweismartlogger.h"

class HuaweiSmartLoggerDiscovery : public QObject
{
    Q_OBJECT
public:
    explicit HuaweiSmartLoggerDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, quint16 port, QObject *parent = nullptr);

    typedef struct Result {
        QHostAddress address;
        NetworkDeviceInfo networkDeviceInfo;
    } Result;

    QList<Result> results() const;

    void startDiscovery();

signals:
    void discoveryFinished();

private:
    NetworkDeviceDiscovery *m_networkDeviceDiscovery = nullptr;
    quint16 m_port = 502;
    QDateTime m_startDateTime;

    QList<HuaweiSmartLogger *> m_connections;
    QList<Result> m_results;

    NetworkDeviceInfos m_networkDeviceInfos;

private slots:
    void checkNetworkDevice(const QHostAddress &address);
    void cleanupConnection(HuaweiSmartLogger *connection);

    void finishDiscovery();
};

#endif // HUAWEISMARTLOGGERDISCOVERY_H
