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

#ifndef HUAWEIFUSIONSOLARDISCOVERY_H
#define HUAWEIFUSIONSOLARDISCOVERY_H

#include <QObject>

#include <network/networkdevicediscovery.h>

#include "huaweifusionsolar.h"

class HuaweiFusionSolarDiscovery : public QObject
{
    Q_OBJECT
public:
    explicit HuaweiFusionSolarDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, quint16 port, const QList<quint16> &slaveIds, QObject *parent = nullptr);

    typedef struct Result {
        QString modelName;
        QString serialNumber;
        quint16 slaveId;
        QHostAddress address;
        NetworkDeviceInfo networkDeviceInfo;
    } Result;

    void startDiscovery();

    QList<Result> results() const;

signals:
    void discoveryFinished();

private:
    NetworkDeviceDiscovery *m_networkDeviceDiscovery = nullptr;
    quint16 m_port = 502;
    QList<quint16> m_slaveIds;
    QDateTime m_startDateTime;

    QHash<QHostAddress, QQueue<HuaweiFusionSolar *>> m_pendingConnectionAttempts;
    QList<HuaweiFusionSolar *> m_connections;
    QList<Result> m_results;

    NetworkDeviceInfos m_networkDeviceInfos;

    void testNextConnection(const QHostAddress &address);

    void checkNetworkDevice(const QHostAddress &address);
    void cleanupConnection(HuaweiFusionSolar *connection);

    void finishDiscovery();
};

#endif // HUAWEIFUSIONSOLARDISCOVERY_H
