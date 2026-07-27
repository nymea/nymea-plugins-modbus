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

#include <QDebug>
#include <QObject>
#include <QQueue>
#include <QSet>
#include <QTimer>

#include <functional>

#include <queuedmodbusreply.h>

#include "ev11modbustcpconnection.h"

class PceWallbox : public EV11ModbusTcpConnection
{
    Q_OBJECT
public:
    typedef struct ChargingCurrentState
    {
        bool power = false;
        double maxChargingCurrent = 6;
        uint desiredPhaseCount = 3;
    } ChargingCurrentState;

    explicit PceWallbox(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent = nullptr);

    bool update() override;

    void setOperationalStartupEnabled(bool enabled);
    bool operational() const;
    void startOperationalMode();

    void setRfidEnabled(bool enabled);
    bool rfidEnabled() const;
    bool initializeRfidOperatingMode(RfidOperatingMode mode);
    bool hasPendingRfidTag() const;
    bool hasRfidAuthorization() const;
    bool canSubmitRfidDecision(bool approved) const;
    bool submitRfidDecision(bool approved);
    bool rfidEnrollmentActive() const;
    bool setRfidEnrollmentActive(bool active);

    QueuedModbusReply *setRfidOperatingModeAsync(RfidOperatingMode mode);

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
    static bool parseRfidToken(const QVector<quint16> &values, QString *code);

signals:
    void rfidInitializationFinished(bool success);
    void rfidTagDetected(const QString &code);
    void rfidDecisionFinished(bool success);
    void rfidEnrollmentActiveChanged(bool active);
    void rfidEnrollmentChangeFinished(bool active, bool success);

private slots:
    void sendHeartbeat();
    void sendNextRequest();

protected:
    virtual bool rfidTransportAvailable() const;

private:
    friend class TestPceWallbox;

    static constexpr int RequestInterval = 300;
    static constexpr int UpdateInterval = 1000;

    QTimer m_timer;
    QTimer m_requestTimer;
    QTimer m_updateTimer;
    quint16 m_heartbeat = 1;
    QueuedModbusReply *m_currentReply = nullptr;
    QQueue<QueuedModbusReply *> m_writeQueue;
    QQueue<QueuedModbusReply *> m_readQueue;
    QSet<QueuedModbusReply *> m_updateReplies;
    bool m_aboutToDelete = false;
    bool m_operationalStartupEnabled = true;
    bool m_operational = false;
    bool m_updateInProgress = false;
    bool m_rfidEnabled = false;
    bool m_rfidInitializing = false;
    bool m_rfidModeConfirmed = false;
    bool m_rfidDecisionInProgress = false;
    bool m_rfidEnrollmentActive = false;
    bool m_rfidEnrollmentArming = false;
    bool m_rfidEnrollmentDisarming = false;
    bool m_rfidEnrollmentChangeInProgress = false;
    bool m_requestedRfidEnrollmentActive = false;
    bool m_rfidSessionWriteInProgress = false;
    bool m_rfidSessionCommitAttempted = false;
    QVector<quint16> m_observedRfidToken;
    QVector<quint16> m_pendingRfidToken;
    QVector<quint16> m_pendingRfidEnrollmentToken;
    QVector<quint16> m_acceptedRfidToken;
    bool m_acceptedRfidTokenCommitted = false;
    quint64 m_rfidLedGeneration = 0;
    quint64 m_rfidEnrollmentGeneration = 0;

    void enqueueRequest(QueuedModbusReply *reply, bool updateRequest = false);
    void requestFinished(QueuedModbusReply *reply);
    void finishUpdateRound();
    void processRfidRead(const QVector<quint16> &values);
    void synchronizeRfidAuthorization();
    bool isVehiclePluggedIn() const;
    void writeRfidSession(const QVector<quint16> &token, const char *operation,
                          const std::function<void(bool)> &callback);
    void resetRfidSession(const char *operation, const std::function<void(bool)> &callback);
    void readRfidOperatingModeOnce(bool updateRequest = false);
    void writeRfidLed(RfidLed led, const std::function<void(bool)> &callback);
    void scheduleRfidLedReset(quint64 generation);
    void resetRfidState();
    bool isSensitiveDataUnit(const QModbusDataUnit &unit) const;

    void cleanupQueues();
};

QDebug operator<<(QDebug debug, const PceWallbox::ChargingCurrentState &chargingCurrentState);

#endif // PCEWALLBOX_H
