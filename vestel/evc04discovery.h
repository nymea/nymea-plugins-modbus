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

#ifndef AMTRONECUDISCOVERY_H
#define AMTRONECUDISCOVERY_H

#include <QObject>
#include <QTimer>
#include <QLoggingCategory>

#include <network/networkdevicediscovery.h>

#include "evc04modbustcpconnection.h"

class EVC04Discovery : public QObject
{
    Q_OBJECT
public:
    explicit EVC04Discovery(NetworkDeviceDiscovery *networkDeviceDiscovery, const QLoggingCategory &dc, QObject *parent = nullptr);
    struct Result {
        QString chargepointId;
        QString firmwareVersion;
        QString brand;
        QString model;
        QHostAddress address;
        NetworkDeviceInfo networkDeviceInfo;
    };

    void startDiscovery();

    QList<Result> discoveryResults() const;

signals:
    void discoveryFinished();

private:
    NetworkDeviceDiscovery *m_networkDeviceDiscovery = nullptr;

    QLoggingCategory m_dc;
    QTimer m_gracePeriodTimer;
    QDateTime m_startDateTime;

    NetworkDeviceInfos m_networkDeviceInfos;
    QList<EVC04ModbusTcpConnection *> m_connections;

    QList<Result> m_discoveryResults;

    void checkNetworkDevice(const QHostAddress &address);
    void cleanupConnection(EVC04ModbusTcpConnection *connection);

    void finishDiscovery();
};

#endif // AMTRONECUDISCOVERY_H
