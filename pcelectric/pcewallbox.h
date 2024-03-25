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

#ifndef PCEWALLBOX_H
#define PCEWALLBOX_H

#include <QTimer>
#include <QQueue>
#include <QObject>

#include <queuedmodbusreply.h>

#include "ev11modbustcpconnection.h"

class PceWallbox : public EV11ModbusTcpConnection
{
    Q_OBJECT
public:
    explicit PceWallbox(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent = nullptr);

    bool update() override;

    QueuedModbusReply *setChargingCurrent(quint16 chargingCurrent); // mA

    QueuedModbusReply *setLedBrightness(quint16 percentage);

    // Note: the modbus implementation of the wallbox gets stuck if a Modbus request has been sent
    // and we disconnect the socket before the response has arrived. Only a reboot of the wallbox
    // fixes the broken communication afterwards. This method waits for the current request before closing the
    // socket and deletes it self.
    // IMPORTNAT: do not use the object after this call, this is a temporary workaround
    void gracefullDeleteLater();

private slots:
    void sendHeartbeat();

private:
    QTimer m_timer;
    quint16 m_heartbeat = 1;
    QueuedModbusReply *m_currentReply = nullptr;
    QQueue<QueuedModbusReply *> m_queue;
    bool m_aboutToDelete = false;

    void sendNextRequest();
    void enqueueRequest(QueuedModbusReply *reply, bool prepend = false);

    void cleanupQueue();
};

#endif // PCEWALLBOX_H
