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

#ifndef SPEEDWIREDISCOVERY_H
#define SPEEDWIREDISCOVERY_H

#include <QTimer>
#include <QObject>
#include <QUdpSocket>

#include <network/networkdevicediscovery.h>

#include "speedwire.h"
#include "speedwireinverter.h"
#include "speedwireinterface.h"

class SpeedwireDiscovery : public QObject
{
    Q_OBJECT
public:
    typedef struct SpeedwireDiscoveryResult {
        QHostAddress address;
        NetworkDeviceInfo networkDeviceInfo;
        Speedwire::DeviceType deviceType = Speedwire::DeviceTypeUnknown;
        quint16 modelId = 0;
        quint32 serialNumber = 0;
    } SpeedwireDiscoveryResult;

    explicit SpeedwireDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, SpeedwireInterface *speedwireInterface, quint32 localSerialNumber, QObject *parent = nullptr);
    ~SpeedwireDiscovery();

    bool startDiscovery();
    bool discoveryRunning() const;

    QList<SpeedwireDiscoveryResult> discoveryResult() const;

signals:
    void discoveryFinished();

private:
    NetworkDeviceDiscovery *m_networkDeviceDiscovery = nullptr;
    SpeedwireInterface *m_speedwireInterface = nullptr;
    quint32 m_localSerialNumber = 0;

    // Discovery
    QTimer m_multicastSearchRequestTimer;
    NetworkDeviceInfos m_networkDeviceInfos;
    QList<SpeedwireDiscoveryResult> m_results;
    QHash<QHostAddress, SpeedwireDiscoveryResult> m_resultMeters;
    QHash<QHostAddress, SpeedwireDiscoveryResult> m_resultInverters;
    bool m_unicastRunning = false;
    bool m_multicastRunning = false;

    QHash<QHostAddress, SpeedwireInverter *> m_inverters;
    void sendUnicastDiscoveryRequest(const QHostAddress &targetHostAddress);

private slots:
    void startUnicastDiscovery();
    void startMulticastDiscovery();

    void processDatagram(const QHostAddress &senderAddress, quint16 senderPort, const QByteArray &datagram, bool multicast);

    void sendDiscoveryRequest();
    void evaluateDiscoveryFinished();
    void finishDiscovery();
};

#endif // SPEEDWIREDISCOVERY_H
