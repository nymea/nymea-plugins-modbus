/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2022, nymea GmbH
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

    void testNextConnection(const QHostAddress &address);

    void checkNetworkDevice(const NetworkDeviceInfo &networkDeviceInfo);
    void cleanupConnection(HuaweiFusionSolar *connection);

    void finishDiscovery();

};

#endif // HUAWEIFUSIONSOLARDISCOVERY_H
