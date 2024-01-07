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
};


#endif // SPEEDWIREINTERFACE_H
