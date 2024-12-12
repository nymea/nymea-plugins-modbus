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

#ifndef AMPERFIEDCONNECTDISCOVERY_H
#define AMPERFIEDCONNECTDISCOVERY_H

#include <QObject>
#include <QTimer>

#include <network/networkdevicediscovery.h>

#include "amperfiedmodbustcpconnection.h"

class AmperfiedConnectDiscovery : public QObject
{
    Q_OBJECT
public:
    explicit AmperfiedConnectDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent = nullptr);
    struct Result {
        quint16 firmwareVersion;
        quint16 slaveId;
        QString modelName;
        QHostAddress address;
        NetworkDeviceInfo networkDeviceInfo;
    };

    void startDiscovery(const QString &nameFilter);

    QList<Result> discoveryResults() const;

signals:
    void discoveryFinished();

private:
    NetworkDeviceDiscovery *m_networkDeviceDiscovery = nullptr;

    QTimer m_gracePeriodTimer;
    QDateTime m_startDateTime;
    QString m_nameFilter;

    QList<AmperfiedModbusTcpConnection *> m_connections;
    NetworkDeviceInfos m_networkDeviceInfos;

    QList<Result> m_discoveryResults;

    void checkNetworkDevice(const QHostAddress &address);
    void cleanupConnection(AmperfiedModbusTcpConnection *connection);

    void finishDiscovery();
};

#endif // AMPERFIEDCONNECTDISCOVERY_H
