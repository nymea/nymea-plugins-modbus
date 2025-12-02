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

#ifndef PCEWALLBOX_H
#define PCEWALLBOX_H

#include <QTimer>
#include <QQueue>
#include <QDebug>
#include <QObject>

#include <queuedmodbusreply.h>

#include "ev11modbustcpconnection.h"

class PceWallbox : public EV11ModbusTcpConnection
{
    Q_OBJECT
public:
    typedef struct ChargingCurrentState {
        bool power = false;
        double maxChargingCurrent = 6;
        uint desiredPhaseCount = 3;
    } ChargingCurrentState;

    explicit PceWallbox(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent = nullptr);

    bool update() override;

    QueuedModbusReply *setChargingCurrentAsync(quint16 chargingCurrent); // mA

    QueuedModbusReply *setLedBrightnessAsync(quint16 percentage);
    QueuedModbusReply *setPhaseAutoSwitchPauseAsync(quint16 seconds);
    QueuedModbusReply *setPhaseAutoSwitchMinChargingTimeAsync(quint16 seconds);
    QueuedModbusReply *setForceChargingResumeAsync(quint16 value);

    QueuedModbusReply *setDigitalInputModeAsync(DigitalInputMode digitalInputMode);


    // Note: the modbus implementation of the wallbox gets stuck if a Modbus request has been sent
    // and we disconnect the socket before the response has arrived. Only a reboot of the wallbox
    // fixes the broken communication afterwards. This method waits for the current request before closing the
    // socket and deletes it self.
    // IMPORTNAT: do not use the object after this call, this is a temporary workaround
    void gracefullDeleteLater();

    static quint16 deriveRegisterFromStates(PceWallbox::ChargingCurrentState state);
    static PceWallbox::ChargingCurrentState deriveStatesFromRegister(quint16 registerValue);

private slots:
    void sendHeartbeat();
    void sendNextRequest();

private:
    QTimer m_timer;
    quint16 m_heartbeat = 1;
    QueuedModbusReply *m_currentReply = nullptr;
    QQueue<QueuedModbusReply *> m_writeQueue;
    QQueue<QueuedModbusReply *> m_readQueue;
    bool m_aboutToDelete = false;

    void enqueueRequest(QueuedModbusReply *reply);

    void cleanupQueues();
};

QDebug operator<<(QDebug debug, const PceWallbox::ChargingCurrentState &chargingCurrentState);


#endif // PCEWALLBOX_H
