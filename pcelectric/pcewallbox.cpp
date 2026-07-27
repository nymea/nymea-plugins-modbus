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

#include "pcewallbox.h"
#include "extern-plugininfo.h"

#include <modbusdatautils.h>

#include <algorithm>

PceWallbox::PceWallbox(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent)
    : EV11ModbusTcpConnection{hostAddress, port, slaveId, parent}
{
    // Timer for resetting the heartbeat register (watchdog)
    m_timer.setInterval(30000);
    m_timer.setSingleShot(false);
    connect(&m_timer, &QTimer::timeout, this, &PceWallbox::sendHeartbeat);

    m_requestTimer.setSingleShot(true);
    connect(&m_requestTimer, &QTimer::timeout, this, &PceWallbox::sendNextRequest);

    m_updateTimer.setInterval(UpdateInterval);
    m_updateTimer.setSingleShot(true);
    connect(&m_updateTimer, &QTimer::timeout, this, [this]() {
        update();
    });

    connect(this, &EV11ModbusTcpConnection::reachableChanged, this, [this](bool reachable) {
        if (!reachable) {
            m_timer.stop();
            m_requestTimer.stop();
            m_updateTimer.stop();
            m_operational = false;
            resetRfidState();

            cleanupQueues();

            if (m_currentReply) {
                m_currentReply = nullptr;
            }

        } else {
            initialize();
        }
    });

    connect(this, &EV11ModbusTcpConnection::initializationFinished, this, [this](bool success) {
        if (success) {
            qCDebug(dcPcElectric()) << "Connection initialized successfully" << m_modbusTcpMaster->hostAddress().toString();
            if (m_operationalStartupEnabled)
                startOperationalMode();

        } else {
            qCWarning(dcPcElectric()) << "Connection initialization failed for" << m_modbusTcpMaster->hostAddress().toString();
        }
    });
}

void PceWallbox::setOperationalStartupEnabled(bool enabled)
{
    m_operationalStartupEnabled = enabled;
    if (!enabled)
        m_timer.stop();
}

bool PceWallbox::operational() const
{
    return m_operational;
}

void PceWallbox::startOperationalMode()
{
    if (!reachable() || m_operational)
        return;
    m_operational = true;
    m_timer.start();
    sendHeartbeat();
    update();
}

void PceWallbox::setRfidEnabled(bool enabled)
{
    m_rfidEnabled = enabled;
    if (!enabled)
        resetRfidState();
}

bool PceWallbox::rfidEnabled() const
{
    return m_rfidEnabled;
}

bool PceWallbox::rfidTransportAvailable() const
{
    return m_rfidEnabled && firmwareRevision() > QStringLiteral("0025")
           && m_modbusTcpMaster->transport() == ModbusTcpMaster::TransportTls;
}

bool PceWallbox::initializeRfidOperatingMode(RfidOperatingMode mode)
{
    if (!reachable() || !rfidTransportAvailable() || m_rfidInitializing)
        return false;

    m_rfidInitializing = true;
    m_rfidModeConfirmed = false;
    QueuedModbusReply *modeReply = new QueuedModbusReply(
        QueuedModbusReply::RequestTypeWrite,
        setRfidOperatingModeDataUnit(mode), this);
    connect(modeReply, &QueuedModbusReply::finished, modeReply, &QueuedModbusReply::deleteLater);
    connect(modeReply, &QueuedModbusReply::finished, this, [this, modeReply, mode]() {
        if (modeReply->error() != QModbusDevice::NoError) {
            m_rfidInitializing = false;
            emit rfidInitializationFinished(false);
            return;
        }

        QueuedModbusReply *readReply = new QueuedModbusReply(
            QueuedModbusReply::RequestTypeRead, rfidOperatingModeDataUnit(), this);
        connect(readReply, &QueuedModbusReply::finished, readReply, &QueuedModbusReply::deleteLater);
        connect(readReply, &QueuedModbusReply::finished, this, [this, readReply, mode]() {
            bool success = false;
            if (readReply->error() == QModbusDevice::NoError) {
                const QVector<quint16> values = readReply->reply()->result().values();
                if (values.size() == 1) {
                    processRfidOperatingModeRegisterValues(values);
                    success = rfidOperatingMode() == mode;
                }
            }
            m_rfidModeConfirmed = success;
            m_rfidInitializing = false;
            emit rfidInitializationFinished(success);
        });
        enqueueRequest(readReply);
    });
    enqueueRequest(modeReply);
    return true;
}

bool PceWallbox::hasPendingRfidTag() const
{
    return (!m_pendingRfidToken.isEmpty() || !m_pendingRfidEnrollmentToken.isEmpty())
            && !m_rfidDecisionInProgress;
}

bool PceWallbox::hasRfidAuthorization() const
{
    return !m_acceptedRfidToken.isEmpty();
}

bool PceWallbox::canSubmitRfidDecision(bool approved) const
{
    if (m_rfidDecisionInProgress || m_rfidSessionWriteInProgress
            || !rfidTransportAvailable())
        return false;

    if (!m_pendingRfidEnrollmentToken.isEmpty())
        return true;

    return approved ? !m_pendingRfidToken.isEmpty()
                    : !m_pendingRfidToken.isEmpty() || !m_acceptedRfidToken.isEmpty();
}

bool PceWallbox::submitRfidDecision(bool approved)
{
    const char *operation = approved ? "tagAccepted" : "tagDenied";
    qCDebug(dcPcElectric()) << "Received RFID operation" << operation
                           << "for charger" << m_modbusTcpMaster->hostAddress().toString()
                           << "slave" << m_slaveId << "charging state" << chargingState();

    if (!canSubmitRfidDecision(approved)) {
        qCWarning(dcPcElectric()) << "Cannot execute RFID operation" << operation
                                  << "for charger" << m_modbusTcpMaster->hostAddress().toString()
                                  << "slave" << m_slaveId << "charging state" << chargingState()
                                  << "because no matching redacted authorization record is available";
        return false;
    }

    m_rfidDecisionInProgress = true;
    if (!m_pendingRfidEnrollmentToken.isEmpty()) {
        const QVector<quint16> enrollmentToken = m_pendingRfidEnrollmentToken;
        const quint64 generation = ++m_rfidLedGeneration;
        writeRfidLed(approved ? RfidLedAccepted : RfidLedRejected,
                     [this, enrollmentToken, generation](bool success) {
            scheduleRfidLedReset(generation);
            if (m_pendingRfidEnrollmentToken == enrollmentToken)
                m_pendingRfidEnrollmentToken.clear();
            if (m_rfidEnrollmentActive) {
                m_rfidEnrollmentActive = false;
                emit rfidEnrollmentActiveChanged(false);
            }
            m_rfidDecisionInProgress = false;
            emit rfidDecisionFinished(success);
        });
        return true;
    }

    const QVector<quint16> decisionToken = m_pendingRfidToken;
    const QVector<quint16> previousAuthorization = m_acceptedRfidToken;
    const auto finishDecision = [this](bool success) {
        m_rfidDecisionInProgress = false;
        emit rfidDecisionFinished(success);
    };

    if (!approved) {
        const bool pendingScan = !decisionToken.isEmpty();
        const bool preserveAuthorization = pendingScan && !previousAuthorization.isEmpty();
        const auto showRejected = [this, decisionToken, pendingScan, preserveAuthorization,
                                   finishDecision]() {
            const quint64 generation = ++m_rfidLedGeneration;
            writeRfidLed(RfidLedRejected,
                         [this, generation, decisionToken, pendingScan,
                          preserveAuthorization, finishDecision](bool success) {
                scheduleRfidLedReset(generation);
                if (success) {
                    if (pendingScan && m_pendingRfidToken == decisionToken)
                        m_pendingRfidToken.clear();
                    if (!preserveAuthorization) {
                        m_acceptedRfidToken.clear();
                        m_acceptedRfidTokenCommitted = false;
                        m_rfidSessionCommitAttempted = false;
                    }
                    qCDebug(dcPcElectric())
                        << (pendingScan ? "Denied pending redacted authorization record"
                                        : "Timed out and cleared held redacted authorization record")
                        << "for charger" << m_modbusTcpMaster->hostAddress().toString()
                        << "slave" << m_slaveId << "charging state" << chargingState()
                        << (preserveAuthorization
                                ? "while preserving the earlier authorization"
                                : "and cleared local authorization");
                } else {
                    qCWarning(dcPcElectric())
                        << "RFID tagDenied feedback failed for charger"
                        << m_modbusTcpMaster->hostAddress().toString()
                        << "slave" << m_slaveId << "charging state" << chargingState()
                        << "and retryable authorization state was retained";
                }
                finishDecision(success);
            });
        };

        if (preserveAuthorization) {
            showRejected();
        } else {
            resetRfidSession(pendingScan ? "tagDenied safeguard" : "tagDenied timeout",
                             [showRejected, finishDecision](bool success) {
                if (success)
                    showRejected();
                else
                    finishDecision(false);
            });
        }
        return true;
    }

    const bool commitNow = isVehiclePluggedIn();
    const auto showAccepted = [this, decisionToken, commitNow, finishDecision]() {
        const quint64 generation = ++m_rfidLedGeneration;
        writeRfidLed(RfidLedAccepted, [this, generation, decisionToken, commitNow,
                                      finishDecision](bool success) {
            scheduleRfidLedReset(generation);
            if (success) {
                const bool replacing = !m_acceptedRfidToken.isEmpty()
                        && m_acceptedRfidToken != decisionToken;
                m_acceptedRfidToken = decisionToken;
                m_acceptedRfidTokenCommitted = commitNow;
                m_rfidSessionCommitAttempted = commitNow;
                if (m_pendingRfidToken == decisionToken)
                    m_pendingRfidToken.clear();
                qCDebug(dcPcElectric())
                    << (replacing ? "Replaced" : "Stored")
                    << "redacted RFID authorization record for charger"
                    << m_modbusTcpMaster->hostAddress().toString()
                    << "slave" << m_slaveId << "charging state" << chargingState()
                    << (commitNow ? "and committed it to the RFID session"
                                  : "without committing it while unplugged");
            } else {
                qCWarning(dcPcElectric())
                    << "RFID tagAccepted feedback failed for charger"
                    << m_modbusTcpMaster->hostAddress().toString()
                    << "slave" << m_slaveId << "charging state" << chargingState()
                    << "and retryable authorization state was retained";
            }
            finishDecision(success);
        });
    };

    if (commitNow) {
        writeRfidSession(decisionToken, "tagAccepted", [showAccepted, finishDecision](bool success) {
            if (success)
                showAccepted();
            else
                finishDecision(false);
        });
    } else {
        qCDebug(dcPcElectric()) << "Deferring RFID session write for charger"
                               << m_modbusTcpMaster->hostAddress().toString()
                               << "slave" << m_slaveId << "charging state" << chargingState()
                               << "because the vehicle is not plugged in";
        showAccepted();
    }
    return true;
}

bool PceWallbox::rfidEnrollmentActive() const
{
    return m_rfidEnrollmentActive;
}

bool PceWallbox::setRfidEnrollmentActive(bool active)
{
    if (m_rfidEnrollmentChangeInProgress || m_rfidDecisionInProgress)
        return false;

    if (active == m_rfidEnrollmentActive
            && (active || m_pendingRfidEnrollmentToken.isEmpty())) {
        emit rfidEnrollmentChangeFinished(active, true);
        return true;
    }

    const quint64 generation = ++m_rfidEnrollmentGeneration;
    m_rfidEnrollmentChangeInProgress = true;
    m_requestedRfidEnrollmentActive = active;
    if (active) {
        if (!reachable() || !rfidTransportAvailable()) {
            m_rfidEnrollmentChangeInProgress = false;
            emit rfidEnrollmentChangeFinished(true, false);
            return true;
        }

        m_rfidEnrollmentArming = true;
        m_rfidEnrollmentDisarming = false;
        m_pendingRfidEnrollmentToken.clear();
        ++m_rfidLedGeneration;
        writeRfidLed(RfidLedLearn, [this, generation](bool success) {
            if (generation != m_rfidEnrollmentGeneration)
                return;
            m_rfidEnrollmentArming = false;
            m_rfidEnrollmentChangeInProgress = false;
            if (success) {
                m_rfidEnrollmentActive = true;
                emit rfidEnrollmentActiveChanged(true);
            }
            emit rfidEnrollmentChangeFinished(true, success);
        });
        return true;
    }

    m_pendingRfidEnrollmentToken.clear();
    m_rfidEnrollmentArming = false;
    const bool writeAvailable = reachable() && rfidTransportAvailable();
    m_rfidEnrollmentDisarming = writeAvailable;
    m_rfidEnrollmentActive = false;
    ++m_rfidLedGeneration;
    if (!writeAvailable) {
        m_rfidEnrollmentDisarming = false;
        m_rfidEnrollmentChangeInProgress = false;
        emit rfidEnrollmentActiveChanged(false);
        emit rfidEnrollmentChangeFinished(false, true);
        return true;
    }

    writeRfidLed(RfidLedNone, [this, generation](bool success) {
        if (generation != m_rfidEnrollmentGeneration)
            return;
        m_rfidEnrollmentDisarming = false;
        m_rfidEnrollmentChangeInProgress = false;
        emit rfidEnrollmentActiveChanged(false);
        emit rfidEnrollmentChangeFinished(false, success);
    });
    return true;
}

bool PceWallbox::update()
{
    if (m_aboutToDelete || !m_operational)
        return false;

    if (!reachable())
        return false;

    // A completed update round owns the one-second cool-down before the next
    // one starts. This also prevents callers from overlapping polling rounds.
    if (m_updateInProgress || m_updateTimer.isActive())
        return true;

    m_updateInProgress = true;

    QueuedModbusReply *reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeRead, readBlockStatusDataUnit(), this);
    connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
    connect(reply, &QueuedModbusReply::finished, this, [this, reply]() {
        if (m_currentReply == reply)
            m_currentReply = nullptr;

        if (reply->error() != QModbusDevice::NoError) {
            return;
        }

        const QModbusDataUnit unit = reply->reply()->result();
        const QVector<quint16> blockValues = unit.values();
        processBlockStatusRegisterValues(blockValues);
        synchronizeRfidAuthorization();

    });

    enqueueRequest(reply, true);

    // Keep the firmware mode synchronized and poll the complete credential block.
    // The credential values are sensitive and must never be logged.
    if (rfidTransportAvailable()) {
        readRfidOperatingModeOnce(true);

        reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeRead, rfidReadDataUnit(), this);
        connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
        connect(reply, &QueuedModbusReply::finished, this, [this, reply]() {
            if (reply->error() != QModbusDevice::NoError)
                return;
            const QVector<quint16> values = reply->reply()->result().values();
            processRfidRead(values);
        });
        enqueueRequest(reply, true);
    }

    // charging current register. Contains
    // - power state
    // - chargingcurrent (if power is true)
    // - phases (if power is true)
    bool chargingCurrentQueued = false;
    foreach (QueuedModbusReply *r, m_readQueue) {
        if (r->dataUnit().startAddress() == chargingCurrentDataUnit().startAddress()) {
            chargingCurrentQueued = true;
            break;
        }
    }

    if (!chargingCurrentQueued) {
        reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeRead, chargingCurrentDataUnit(), this);
        connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
        connect(reply, &QueuedModbusReply::finished, this, [this, reply]() {
            if (m_currentReply == reply)
                m_currentReply = nullptr;

            if (reply->error() != QModbusDevice::NoError) {
                return;
            }

            const QModbusDataUnit unit = reply->reply()->result();
            const QVector<quint16> values = unit.values();
            processChargingCurrentRegisterValues(values);

        });

        enqueueRequest(reply, true);
    }

    // Digital input
    bool digitalInputAlreadyQueued = false;
    foreach (QueuedModbusReply *r, m_readQueue) {
        if (r->dataUnit().startAddress() == digitalInputModeDataUnit().startAddress()) {
            digitalInputAlreadyQueued = true;
            break;
        }
    }

    if (!digitalInputAlreadyQueued) {
        reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeRead, digitalInputModeDataUnit(), this);
        connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
        connect(reply, &QueuedModbusReply::finished, this, [this, reply]() {
            if (m_currentReply == reply)
                m_currentReply = nullptr;

            if (reply->error() != QModbusDevice::NoError) {
                return;
            }

            const QModbusDataUnit unit = reply->reply()->result();
            const QVector<quint16> values = unit.values();
            processDigitalInputModeRegisterValues(values);

        });

        enqueueRequest(reply, true);
    }

    // Led brightness
    bool ledBrightnessAlreadyQueued = false;
    foreach (QueuedModbusReply *r, m_readQueue) {
        if (r->dataUnit().startAddress() == ledBrightnessDataUnit().startAddress()) {
            ledBrightnessAlreadyQueued = true;
            break;
        }
    }

    if (!ledBrightnessAlreadyQueued) {
        reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeRead, ledBrightnessDataUnit(), this);
        connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
        connect(reply, &QueuedModbusReply::finished, this, [this, reply]() {
            if (m_currentReply == reply)
                m_currentReply = nullptr;

            if (reply->error() != QModbusDevice::NoError) {
                return;
            }

            const QModbusDataUnit unit = reply->reply()->result();
            const QVector<quint16> values = unit.values();
            processLedBrightnessRegisterValues(values);

        });

        enqueueRequest(reply, true);
    }

    if (firmwareRevision() < "0025")
        return true;

    // ---------------------------------------------------------------------------------------
    // Registers since 0025 (V 0.25)

    // Make sure we only have one update 2 call in the queue
    bool update2Queued = false;
    foreach (QueuedModbusReply *r, m_readQueue) {
        if (r->dataUnit().startAddress() == readBlockUpdate2DataUnit().startAddress()) {
            update2Queued = true;
            break;
        }
    }

    if (!update2Queued) {
        reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeRead, readBlockUpdate2DataUnit(), this);
        connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
        connect(reply, &QueuedModbusReply::finished, this, [this, reply]() {
            if (m_currentReply == reply)
                m_currentReply = nullptr;

            if (reply->error() != QModbusDevice::NoError) {
                qCWarning(dcPcElectric()) << "Failed to fetch update 2 block" << reply->error() << reply->errorString();
                return;
            }

            const QModbusDataUnit unit = reply->reply()->result();
            const QVector<quint16> blockValues = unit.values();
            processBlockUpdate2RegisterValues(blockValues);

        });

        enqueueRequest(reply, true);
    }

    bool phaseAutoSwitchPauseQueued = false;
    foreach (QueuedModbusReply *r, m_readQueue) {
        if (r->dataUnit().startAddress() == phaseAutoSwitchPauseDataUnit().startAddress()) {
            phaseAutoSwitchPauseQueued = true;
            break;
        }
    }

    if (!phaseAutoSwitchPauseQueued) {
        reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeRead, phaseAutoSwitchPauseDataUnit(), this);
        connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
        connect(reply, &QueuedModbusReply::finished, this, [this, reply]() {
            if (m_currentReply == reply)
                m_currentReply = nullptr;

            if (reply->error() != QModbusDevice::NoError) {
                return;
            }

            const QModbusDataUnit unit = reply->reply()->result();
            const QVector<quint16> values = unit.values();
            processPhaseAutoSwitchPauseRegisterValues(values);

        });

        enqueueRequest(reply, true);
    }

    // Phase auto switch pause (since firmware version 0.25 ...)
    bool phaseAutoSwitchMinChargingTimeQueued = false;
    foreach (QueuedModbusReply *r, m_readQueue) {
        if (r->dataUnit().startAddress() == phaseAutoSwitchMinChargingTimeDataUnit().startAddress()) {
            phaseAutoSwitchMinChargingTimeQueued = true;
            break;
        }
    }

    if (!phaseAutoSwitchMinChargingTimeQueued) {
        reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeRead, phaseAutoSwitchMinChargingTimeDataUnit(), this);
        connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
        connect(reply, &QueuedModbusReply::finished, this, [this, reply]() {
            if (m_currentReply == reply)
                m_currentReply = nullptr;

            if (reply->error() != QModbusDevice::NoError) {
                return;
            }

            const QModbusDataUnit unit = reply->reply()->result();
            const QVector<quint16> values = unit.values();
            processPhaseAutoSwitchMinChargingTimeRegisterValues(values);

        });

        enqueueRequest(reply, true);
    }

    // Phase auto switch pause (since firmware version 0.25 ...)
    bool forceChargingResumeQueued = false;
    foreach (QueuedModbusReply *r, m_readQueue) {
        if (r->dataUnit().startAddress() == forceChargingResumeDataUnit().startAddress()) {
            forceChargingResumeQueued = true;
            break;
        }
    }

    if (!forceChargingResumeQueued) {
        reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeRead, forceChargingResumeDataUnit(), this);
        connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
        connect(reply, &QueuedModbusReply::finished, this, [this, reply]() {
            if (m_currentReply == reply)
                m_currentReply = nullptr;

            if (reply->error() != QModbusDevice::NoError) {
                return;
            }

            const QModbusDataUnit unit = reply->reply()->result();
            const QVector<quint16> values = unit.values();
            processForceChargingResumeRegisterValues(values);

        });

        enqueueRequest(reply, true);
    }

    return true;
}

void PceWallbox::processRfidRead(const QVector<quint16> &values)
{
    if (values.size() != 6)
        return;

    const bool nullBlock = std::all_of(values.cbegin(), values.cend(),
                                       [](quint16 value) { return value == 0; });
    if (nullBlock) {
        m_observedRfidToken.clear();
        return;
    }

    if (!m_rfidModeConfirmed || rfidOperatingMode() != RfidOperatingModeRemote)
        return;

    QString code;
    if (!parseRfidToken(values, &code))
        return;

    if (values == m_observedRfidToken)
        return;

    m_observedRfidToken = values;
    QVector<quint16> &pendingToken = (m_rfidEnrollmentActive || m_rfidEnrollmentArming
                                      || m_rfidEnrollmentDisarming)
            ? m_pendingRfidEnrollmentToken : m_pendingRfidToken;
    if (!pendingToken.isEmpty() && pendingToken != values) {
        qCDebug(dcPcElectric()) << "Replacing pending redacted RFID scan for charger"
                               << m_modbusTcpMaster->hostAddress().toString()
                               << "slave" << m_slaveId << "charging state" << chargingState();
    }
    pendingToken = values;
    emit rfidTagDetected(code);
}

bool PceWallbox::isVehiclePluggedIn() const
{
    return chargingState() == ChargingStateB1 || chargingState() == ChargingStateB2
            || chargingState() == ChargingStateC1 || chargingState() == ChargingStateC2;
}

void PceWallbox::synchronizeRfidAuthorization()
{
    if (m_acceptedRfidToken.isEmpty())
        return;

    if (m_acceptedRfidTokenCommitted
            && (chargingState() == ChargingStateA1 || chargingState() == ChargingStateA2)) {
        qCDebug(dcPcElectric()) << "Clearing committed local RFID authorization for charger"
                               << m_modbusTcpMaster->hostAddress().toString()
                               << "slave" << m_slaveId << "charging state" << chargingState()
                               << "after return to state A; firmware owns session cleanup";
        m_acceptedRfidToken.clear();
        m_acceptedRfidTokenCommitted = false;
        m_rfidSessionCommitAttempted = false;
        return;
    }

    if (m_acceptedRfidTokenCommitted || !isVehiclePluggedIn()
            || m_rfidSessionWriteInProgress)
        return;

    const QVector<quint16> token = m_acceptedRfidToken;
    const bool retry = m_rfidSessionCommitAttempted;
    m_rfidSessionCommitAttempted = true;
    qCDebug(dcPcElectric()) << (retry ? "Retrying" : "Starting delayed")
                           << "RFID session commit for charger"
                           << m_modbusTcpMaster->hostAddress().toString()
                           << "slave" << m_slaveId << "charging state" << chargingState();
    writeRfidSession(token, retry ? "delayed commit retry" : "delayed commit",
                     [this, token](bool success) {
        if (success && m_acceptedRfidToken == token) {
            m_acceptedRfidTokenCommitted = true;
            qCDebug(dcPcElectric()) << "Delayed RFID session commit succeeded for charger"
                                   << m_modbusTcpMaster->hostAddress().toString()
                                   << "slave" << m_slaveId << "charging state" << chargingState();
        } else if (!success) {
            qCWarning(dcPcElectric()) << "Delayed RFID session commit failed for charger"
                                     << m_modbusTcpMaster->hostAddress().toString()
                                     << "slave" << m_slaveId << "charging state" << chargingState()
                                     << "and will be retried during a later update";
        }
    });
}

void PceWallbox::writeRfidSession(const QVector<quint16> &token, const char *operation,
                                  const std::function<void(bool)> &callback)
{
    m_rfidSessionWriteInProgress = true;
    qCDebug(dcPcElectric()) << "Starting RFID session write operation" << operation
                           << "for charger" << m_modbusTcpMaster->hostAddress().toString()
                           << "slave" << m_slaveId << "charging state" << chargingState();
    QueuedModbusReply *reply = new QueuedModbusReply(
        QueuedModbusReply::RequestTypeWrite, setRfidSessionDataUnit(token), this);
    connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
    connect(reply, &QueuedModbusReply::finished, this,
            [this, reply, operation, callback]() {
        m_rfidSessionWriteInProgress = false;
        const bool success = reply->error() == QModbusDevice::NoError;
        if (success) {
            qCDebug(dcPcElectric()) << "RFID session write operation" << operation
                                   << "succeeded for charger"
                                   << m_modbusTcpMaster->hostAddress().toString()
                                   << "slave" << m_slaveId << "charging state" << chargingState();
        } else {
            qCWarning(dcPcElectric()) << "RFID session write operation" << operation
                                     << "failed for charger"
                                     << m_modbusTcpMaster->hostAddress().toString()
                                     << "slave" << m_slaveId << "charging state" << chargingState()
                                     << reply->errorString();
        }
        callback(success);
    });
    enqueueRequest(reply);
}

void PceWallbox::resetRfidSession(const char *operation,
                                  const std::function<void(bool)> &callback)
{
    qCDebug(dcPcElectric()) << "Starting RFID session reset operation" << operation
                           << "for charger" << m_modbusTcpMaster->hostAddress().toString()
                           << "slave" << m_slaveId << "charging state" << chargingState();
    writeRfidSession(QVector<quint16>(6, 0), operation,
                     [this, operation, callback](bool success) {
        qCDebug(dcPcElectric()) << "RFID session reset operation" << operation
                               << (success ? "succeeded" : "failed")
                               << "for charger" << m_modbusTcpMaster->hostAddress().toString()
                               << "slave" << m_slaveId << "charging state" << chargingState();
        callback(success);
    });
}

void PceWallbox::readRfidOperatingModeOnce(bool updateRequest)
{
    QueuedModbusReply *reply = new QueuedModbusReply(
        QueuedModbusReply::RequestTypeRead, rfidOperatingModeDataUnit(), this);
    connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
    connect(reply, &QueuedModbusReply::finished, this, [this, reply]() {
        if (reply->error() != QModbusDevice::NoError) {
            m_rfidModeConfirmed = false;
            return;
        }

        const QVector<quint16> values = reply->reply()->result().values();
        if (values.size() == 1 && values.first() <= RfidOperatingModeLocal) {
            processRfidOperatingModeRegisterValues(values);
            m_rfidModeConfirmed = true;
        } else {
            m_rfidModeConfirmed = false;
        }
    });
    enqueueRequest(reply, updateRequest);
}

void PceWallbox::writeRfidLed(RfidLed led, const std::function<void(bool)> &callback)
{
    QueuedModbusReply *reply = new QueuedModbusReply(
        QueuedModbusReply::RequestTypeWrite, setRfidLedDataUnit(led), this);
    connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
    connect(reply, &QueuedModbusReply::finished, this, [reply, callback]() {
        callback(reply->error() == QModbusDevice::NoError);
    });
    enqueueRequest(reply);
}

void PceWallbox::scheduleRfidLedReset(quint64 generation)
{
    QTimer::singleShot(500, this, [this, generation]() {
        if (generation != m_rfidLedGeneration || !rfidTransportAvailable())
            return;
        writeRfidLed(RfidLedNone, [](bool) {});
    });
}

void PceWallbox::resetRfidState()
{
    const bool enrollmentWasActive = m_rfidEnrollmentActive;
    const bool enrollmentChangeWasInProgress = m_rfidEnrollmentChangeInProgress;
    const bool requestedEnrollmentActive = m_requestedRfidEnrollmentActive;
    if (!m_pendingRfidToken.isEmpty() || !m_pendingRfidEnrollmentToken.isEmpty()
            || !m_acceptedRfidToken.isEmpty()) {
        qCDebug(dcPcElectric()) << "Clearing local redacted RFID authorization state for charger"
                               << m_modbusTcpMaster->hostAddress().toString()
                               << "slave" << m_slaveId << "charging state" << chargingState()
                               << "because the RFID transport is being reset";
    }
    m_rfidInitializing = false;
    m_rfidModeConfirmed = false;
    m_rfidDecisionInProgress = false;
    m_rfidEnrollmentActive = false;
    m_rfidEnrollmentArming = false;
    m_rfidEnrollmentDisarming = false;
    m_rfidEnrollmentChangeInProgress = false;
    m_rfidSessionWriteInProgress = false;
    m_rfidSessionCommitAttempted = false;
    m_observedRfidToken.clear();
    m_pendingRfidToken.clear();
    m_pendingRfidEnrollmentToken.clear();
    m_acceptedRfidToken.clear();
    m_acceptedRfidTokenCommitted = false;
    ++m_rfidLedGeneration;
    ++m_rfidEnrollmentGeneration;
    if (enrollmentWasActive)
        emit rfidEnrollmentActiveChanged(false);
    if (enrollmentChangeWasInProgress)
        emit rfidEnrollmentChangeFinished(requestedEnrollmentActive, false);
}

bool PceWallbox::isSensitiveDataUnit(const QModbusDataUnit &unit) const
{
    const int start = unit.startAddress();
    const int end = start + unit.valueCount();

    // Derived from the generated data units so this stays in sync with whichever
    // registers EV11.3-registers.json marks as "sensitive", instead of duplicating
    // their addresses here.
    const QVector<QModbusDataUnit> sensitiveUnits = {rfidReadDataUnit(), rfidSessionDataUnit()};
    for (const QModbusDataUnit &sensitiveUnit : sensitiveUnits) {
        const int sensitiveStart = sensitiveUnit.startAddress();
        const int sensitiveEnd = sensitiveStart + sensitiveUnit.valueCount();
        if (start < sensitiveEnd && end > sensitiveStart)
            return true;
    }
    return false;
}

bool PceWallbox::parseRfidToken(const QVector<quint16> &values, QString *code)
{
    if (values.size() != 6)
        return false;

    const quint8 version = static_cast<quint8>(values.at(0) >> 8);
    const quint8 uidLength = static_cast<quint8>(values.at(0) & 0xff);
    if (version != 1 || (uidLength != 4 && uidLength != 7 && uidLength != 10))
        return false;

    QByteArray token;
    token.reserve(10);
    for (int index = 1; index < values.size(); ++index) {
        token.append(static_cast<char>(values.at(index) >> 8));
        token.append(static_cast<char>(values.at(index) & 0xff));
    }
    token.truncate(uidLength);
    if (!std::any_of(token.cbegin(), token.cend(), [](char byte) { return byte != 0; }))
        return false;

    if (code)
        *code = QString::fromLatin1(token.toHex());

    qCDebug(dcPcElectric()) << "Parsed redacted RFID authorization record";
    return true;
}

QueuedModbusReply *PceWallbox::setChargingCurrentAsync(quint16 chargingCurrent)
{
    if (m_aboutToDelete)
        return nullptr;

    QueuedModbusReply *reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeWrite, setChargingCurrentDataUnit(chargingCurrent), this);

    connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
    connect(reply, &QueuedModbusReply::finished, this, [this, reply]() {
        if (m_currentReply == reply)
            m_currentReply = nullptr;

        return;
    });

    enqueueRequest(reply);
    return reply;
}

QueuedModbusReply *PceWallbox::setLedBrightnessAsync(quint16 percentage)
{
    if (m_aboutToDelete)
        return nullptr;

    QueuedModbusReply *reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeWrite, setLedBrightnessDataUnit(percentage), this);

    connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
    connect(reply, &QueuedModbusReply::finished, this, [this, reply]() {
        if (m_currentReply == reply)
            m_currentReply = nullptr;

        return;
    });

    enqueueRequest(reply);
    return reply;
}

QueuedModbusReply *PceWallbox::setPhaseAutoSwitchPauseAsync(quint16 seconds)
{
    if (m_aboutToDelete)
        return nullptr;

    QueuedModbusReply *reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeWrite, setPhaseAutoSwitchPauseDataUnit(seconds), this);

    connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
    connect(reply, &QueuedModbusReply::finished, this, [this, reply]() {
        if (m_currentReply == reply)
            m_currentReply = nullptr;

        return;
    });

    enqueueRequest(reply);
    return reply;
}

QueuedModbusReply *PceWallbox::setPhaseAutoSwitchMinChargingTimeAsync(quint16 seconds)
{
    if (m_aboutToDelete)
        return nullptr;

    QueuedModbusReply *reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeWrite, setPhaseAutoSwitchMinChargingTimeDataUnit(seconds), this);

    connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
    connect(reply, &QueuedModbusReply::finished, this, [this, reply]() {
        if (m_currentReply == reply)
            m_currentReply = nullptr;

        return;
    });

    enqueueRequest(reply);
    return reply;
}

QueuedModbusReply *PceWallbox::setForceChargingResumeAsync(quint16 value)
{
    if (m_aboutToDelete)
        return nullptr;

    QueuedModbusReply *reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeWrite, setForceChargingResumeDataUnit(value), this);

    connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
    connect(reply, &QueuedModbusReply::finished, this, [this, reply]() {
        if (m_currentReply == reply)
            m_currentReply = nullptr;

        return;
    });

    enqueueRequest(reply);
    return reply;
}

QueuedModbusReply *PceWallbox::setDigitalInputModeAsync(DigitalInputMode digitalInputMode)
{
    if (m_aboutToDelete)
        return nullptr;

    QueuedModbusReply *reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeWrite, setDigitalInputModeDataUnit(digitalInputMode), this);

    connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
    connect(reply, &QueuedModbusReply::finished, this, [this, reply]() {
        if (m_currentReply == reply)
            m_currentReply = nullptr;

        return;
    });

    enqueueRequest(reply);
    return reply;
}

QueuedModbusReply *PceWallbox::setRfidOperatingModeAsync(RfidOperatingMode mode)
{
    if (m_aboutToDelete)
        return nullptr;

    m_rfidModeConfirmed = false;
    QueuedModbusReply *reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeWrite, setRfidOperatingModeDataUnit(mode), this);

    connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
    connect(reply, &QueuedModbusReply::finished, this, [this, reply]() {
        if (m_currentReply == reply)
            m_currentReply = nullptr;

        if (reply->error() == QModbusDevice::NoError)
            readRfidOperatingModeOnce();
    });

    enqueueRequest(reply);
    return reply;
}

void PceWallbox::gracefullDeleteLater()
{
    // Clean up the queue
    m_aboutToDelete = true;
    cleanupQueues();

    m_timer.stop();
    m_requestTimer.stop();
    m_updateTimer.stop();

    if (!m_currentReply) {
        qCDebug(dcPcElectric()) << "Deleting object without pending request...";
        // No pending request, we can close the connection and delete the object
        disconnect(this, nullptr, nullptr, nullptr);
        disconnectDevice();
        deleteLater();
    } else {
        qCDebug(dcPcElectric()) << "Pending request, deleting object once the request is finished...";
    }
}

quint16 PceWallbox::deriveRegisterFromStates(PceWallbox::ChargingCurrentState state)
{
    quint16 registerValue = 0;
    if (!state.power)
        return registerValue; // 0

    registerValue = state.maxChargingCurrent * 1000; // convert to mA
    if (state.desiredPhaseCount > 1) {
        registerValue |= static_cast<quint16>(1) << 15;
    }

    return registerValue;
}

PceWallbox::ChargingCurrentState PceWallbox::deriveStatesFromRegister(quint16 registerValue)
{
    PceWallbox::ChargingCurrentState chargingCurrentState;
    chargingCurrentState.power = (registerValue != 0);

    // Only set max charging current if power, otherwise we use default 6A
    if (chargingCurrentState.power) {
        bool threePhaseCharging = (registerValue & (1 << 15));
        chargingCurrentState.desiredPhaseCount = (threePhaseCharging ? 3 : 1);

        chargingCurrentState.maxChargingCurrent = (registerValue & 0x7FFF) / 1000.0;
    }

    return chargingCurrentState;
}

void PceWallbox::sendHeartbeat()
{
    if (m_aboutToDelete)
        return;

    QueuedModbusReply *reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeWrite, setHeartbeatDataUnit(m_heartbeat++), this);

    connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
    connect(reply, &QueuedModbusReply::finished, this, [this, reply]() {
        if (m_currentReply == reply)
            m_currentReply = nullptr;

        if (reply->error() != QModbusDevice::NoError) {
            qCWarning(dcPcElectric()) << "Failed to send heartbeat to" << m_modbusTcpMaster->hostAddress().toString() << reply->errorString();
        } else {
            qCDebug(dcPcElectric()) << "Successfully sent heartbeat to" << m_modbusTcpMaster->hostAddress().toString();
        }

        return;
    });

    enqueueRequest(reply);
}

void PceWallbox::sendNextRequest()
{
    if (m_aboutToDelete) {
        disconnect(this, nullptr, nullptr, nullptr);
        disconnectDevice();
        deleteLater();
        return;
    }

    // Ignore immediate dispatch callbacks left by a completed reply while the
    // inter-request delay is active. The timer's timeout performs the dispatch.
    if (m_requestTimer.isActive())
        return;

    if (m_writeQueue.isEmpty() && m_readQueue.isEmpty())
        return;

    if (m_currentReply)
        return;

    // Note: due to the fact that we have one register which controls 3 states,
    // the order of the execution is critical at this point. We have to make sure
    // the register gets written in the same order as they where requested by the action
    // execution (and the dedicated ChargingCurrentState buffer)

    if (!m_writeQueue.isEmpty()) {
        // Prioritize write requests
        m_currentReply = m_writeQueue.dequeue();
        qCDebug(dcPcElectric()) << "Dequeued write request. Queue count: W" << m_writeQueue.length() << "| R:" << m_readQueue.length();
    } else {
        m_currentReply = m_readQueue.dequeue();
        qCDebug(dcPcElectric()) << "Dequeued read request. Queue count: W" << m_writeQueue.length() << "| R:" << m_readQueue.length();
    }

    switch (m_currentReply->requestType()) {
    case QueuedModbusReply::RequestTypeRead:
        qCDebug(dcPcElectric())
            << "--> Reading"
            << ModbusDataUtils::registerTypeToString(m_currentReply->dataUnit().registerType())
            << "register:"
            << m_currentReply->dataUnit().startAddress()
            << "length"
            << m_currentReply->dataUnit().valueCount();
        m_currentReply->setReply(m_modbusTcpMaster->sendReadRequest(m_currentReply->dataUnit(), m_slaveId));
        break;
    case QueuedModbusReply::RequestTypeWrite:
        qCDebug(dcPcElectric())
            << "--> Writing"
            << ModbusDataUtils::registerTypeToString(m_currentReply->dataUnit().registerType())
            << "register:"
            << m_currentReply->dataUnit().startAddress()
            << "length:"
            << m_currentReply->dataUnit().valueCount()
            << "values:"
            << (isSensitiveDataUnit(m_currentReply->dataUnit())
                    ? QVariant(QStringLiteral("[REDACTED]"))
                    : QVariant::fromValue(m_currentReply->dataUnit().values()));
        m_currentReply->setReply(m_modbusTcpMaster->sendWriteRequest(m_currentReply->dataUnit(), m_slaveId));
        break;
    }

    if (!m_currentReply->reply()) {
        qCWarning(dcPcElectric())
            << "Error occurred while sending"
            << m_currentReply->requestType()
            << ModbusDataUtils::registerTypeToString(m_currentReply->dataUnit().registerType())
            << "register:"
            << m_currentReply->dataUnit().startAddress()
            << "length:"
            << m_currentReply->dataUnit().valueCount()
            << "to"
            << m_modbusTcpMaster->hostAddress().toString()
            << m_modbusTcpMaster->errorString();
        QueuedModbusReply *finishedReply = m_currentReply;
        emit finishedReply->finished();
        return;
    }

    if (m_currentReply->reply()->isFinished()) {
        qCWarning(dcPcElectric()) << "Reply immediately finished";
        QueuedModbusReply *finishedReply = m_currentReply;
        emit finishedReply->finished();
        return;
    }
}

void PceWallbox::enqueueRequest(QueuedModbusReply *reply, bool updateRequest)
{
    connect(reply, &QueuedModbusReply::finished, this, [this, reply]() {
        requestFinished(reply);
    });

    if (updateRequest)
        m_updateReplies.insert(reply);

    switch (reply->requestType()) {
    case QueuedModbusReply::RequestTypeRead:
        m_readQueue.enqueue(reply);
        break;
    case QueuedModbusReply::RequestTypeWrite:
        m_writeQueue.enqueue(reply);
        break;
    }

    if (!m_currentReply && !m_requestTimer.isActive())
        m_requestTimer.start(0);
}

void PceWallbox::requestFinished(QueuedModbusReply *reply)
{
    if (m_currentReply == reply)
        m_currentReply = nullptr;

    // The generated connection feeds every completed request into its
    // communication failure counter. Requests sent through this custom queue
    // must do the same so a half-open TCP connection becomes unreachable after
    // repeated Modbus timeouts.
    handleModbusError(reply->error());

    const bool wasUpdateRequest = m_updateReplies.remove(reply);
    if (wasUpdateRequest && m_updateReplies.isEmpty() && m_updateInProgress)
        finishUpdateRound();

    if (m_aboutToDelete) {
        sendNextRequest();
        return;
    }

    if ((!m_operational && !m_rfidInitializing) || !reachable())
        return;

    m_requestTimer.start(RequestInterval);
}

void PceWallbox::finishUpdateRound()
{
    m_updateInProgress = false;
    emit updateFinished();

    if (!m_aboutToDelete && m_operational && reachable())
        m_updateTimer.start();
}

void PceWallbox::cleanupQueues()
{
    qDeleteAll(m_readQueue);
    m_readQueue.clear();

    qDeleteAll(m_writeQueue);
    m_writeQueue.clear();

    m_updateReplies.clear();
    m_updateInProgress = false;
}

QDebug operator<<(QDebug debug, const PceWallbox::ChargingCurrentState &chargingCurrentState)
{
    QDebugStateSaver saver(debug);
    debug.nospace()
        << "ChargingCurrentState("
        << chargingCurrentState.power
        << ", "
        << chargingCurrentState.maxChargingCurrent
        << " [A], "
        << chargingCurrentState.desiredPhaseCount
        << ')';
    return debug;
}
