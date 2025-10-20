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

#ifndef SUNSPECDISCOVERY_H
#define SUNSPECDISCOVERY_H

#include <QQueue>
#include <QObject>
#include <QDateTime>

#include <sunspecconnection.h>
#include <network/networkdevicediscovery.h>

class SunSpecDiscovery : public QObject
{
    Q_OBJECT
public:
    explicit SunSpecDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, quint16 slaveId, SunSpecDataPoint::ByteOrder byteOrder = SunSpecDataPoint::ByteOrderLittleEndian, QObject *parent = nullptr);
    typedef struct Result {
        QHostAddress address;
        NetworkDeviceInfo networkDeviceInfo;
        quint16 port;
        quint16 slaveId;
        QStringList modelManufacturers;
    } Result;

    QList<Result> results() const;

    void addCustomDiscoveryPort(quint16 port);
    void startDiscovery();

signals:
    void discoveryFinished();

private:
    NetworkDeviceDiscovery *m_networkDeviceDiscovery = nullptr;
    QList<quint16> m_scanPorts;
    quint16 m_slaveId = 1;
    SunSpecDataPoint::ByteOrder m_byteOrder;

    QDateTime m_startDateTime;
    QHash<QHostAddress, QQueue<SunSpecConnection *>> m_pendingConnectionAttempts;
    QHash<SunSpecConnection *, QTimer *> m_connectionTimers;
    QList<SunSpecConnection *> m_connections;
    NetworkDeviceInfos m_networkDeviceInfos;
    QList<Result> m_results;

    void testNextConnection(const QHostAddress &address);

    void checkNetworkDevice(const QHostAddress &address);
    void cleanupConnection(SunSpecConnection *connection);

    void finishDiscovery();
};

#endif // SUNSPECDISCOVERY_H
