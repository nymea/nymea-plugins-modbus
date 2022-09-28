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
    explicit HuaweiFusionSolarDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, quint16 port = 502, quint16 modbusAddress = 1, QObject *parent = nullptr);

    void startDiscovery();

    NetworkDeviceInfos discoveryResults() const;

signals:
    void discoveryFinished();

private:
    NetworkDeviceDiscovery *m_networkDeviceDiscovery = nullptr;
    quint16 m_port;
    quint16 m_modbusAddress;

    QDateTime m_startDateTime;
    QList<HuaweiFusionSolar *> m_connections;
    NetworkDeviceInfos m_discoveryResults;

    void checkNetworkDevice(const NetworkDeviceInfo &networkDeviceInfo);
    void cleanupConnection(HuaweiFusionSolar *connection);

    void finishDiscovery();

};

#endif // HUAWEIFUSIONSOLARDISCOVERY_H
