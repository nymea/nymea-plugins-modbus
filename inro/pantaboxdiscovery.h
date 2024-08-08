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

#ifndef PANTABOXDISCOVERY_H
#define PANTABOXDISCOVERY_H

#include <QObject>

#include <network/networkdevicediscovery.h>
#include "pantabox.h"

class PantaboxDiscovery : public QObject
{
    Q_OBJECT
public:
    explicit PantaboxDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent = nullptr);

    typedef struct Result {
        QString serialNumber;
        QString modbusTcpVersion;
        NetworkDeviceInfo networkDeviceInfo;
    } Result;

    QList<PantaboxDiscovery::Result> results() const;

public slots:
    void startDiscovery();

signals:
    void discoveryFinished();

private:
    NetworkDeviceDiscovery *m_networkDeviceDiscovery = nullptr;
    quint16 m_port = 502;
    quint16 m_modbusAddress = 1;

    QDateTime m_startDateTime;

    QList<Pantabox *> m_connections;

    QList<Result> m_results;

    void checkNetworkDevice(const NetworkDeviceInfo &networkDeviceInfo);
    void cleanupConnection(Pantabox *connection);

    void finishDiscovery();
    void addResult(Pantabox *connection, const NetworkDeviceInfo &networkDeviceInfo);
};

#endif // PANTABOXDISCOVERY_H
