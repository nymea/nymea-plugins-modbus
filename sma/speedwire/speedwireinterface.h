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

#ifndef SPEEDWIREINTERFACE_H
#define SPEEDWIREINTERFACE_H

#include <QObject>
#include <QUdpSocket>
#include <QDataStream>
#include <QTimer>

#include "speedwire.h"

class SpeedwireInterface : public QObject
{
    Q_OBJECT
public:

    explicit SpeedwireInterface(quint32 sourceSerialNumber, QObject *parent = nullptr);
    ~SpeedwireInterface();

    bool available() const;

    static bool isOwnInterface(const QHostAddress &hostAddress);

    quint32 sourceSerialNumber() const;

    bool initialize();

public slots:
    void sendDataUnicast(const QHostAddress &address, const QByteArray &data);
    void sendDataMulticast(const QByteArray &data);

signals:
    void dataReceived(const QHostAddress &senderAddress, quint16 senderPort, const QByteArray &data, bool multicast = false);

private slots:
    void reconfigureMulticastGroup();

private:
    QUdpSocket *m_unicast = nullptr;
    QUdpSocket *m_multicast = nullptr;
    quint32 m_sourceSerialNumber = 0;
    bool m_available = false;
    QTimer m_multicastReconfigureationTimer;
    uint m_multicastWarningPrintCount = 0;
};


#endif // SPEEDWIREINTERFACE_H
