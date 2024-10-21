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

#include "pcewallbox.h"
#include "extern-plugininfo.h"

#include <modbusdatautils.h>

PceWallbox::PceWallbox(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent)
    : EV11ModbusTcpConnection{hostAddress, port, slaveId, parent}
{
    // Timer for resetting the heartbeat register (watchdog)
    m_timer.setInterval(30000);
    m_timer.setSingleShot(false);
    connect(&m_timer, &QTimer::timeout, this, &PceWallbox::sendHeartbeat);

    connect(this, &EV11ModbusTcpConnection::reachableChanged, this, [this](bool reachable){
        if (!reachable) {
            m_timer.stop();

            qDeleteAll(m_queue);
            m_queue.clear();

            if (m_currentReply) {
                m_currentReply = nullptr;
            }

        } else {
            initialize();
        }
    });

    connect(this, &EV11ModbusTcpConnection::initializationFinished, this, [this](bool success){
        if (success) {
            qCDebug(dcPcElectric()) << "Connection initialized successfully" << m_modbusTcpMaster->hostAddress().toString();
            m_timer.start();

            sendHeartbeat();
            update();

        } else {
            qCWarning(dcPcElectric()) << "Connection initialization failed for" << m_modbusTcpMaster->hostAddress().toString();
        }
    });
}

bool PceWallbox::update()
{
    if (m_aboutToDelete)
        return false;

    if (!reachable())
        return false;

    // Make sure we only have one update call in the queue
    foreach (QueuedModbusReply *r, m_queue) {
        if (r->dataUnit().startAddress() == readBlockInitInfosDataUnit().startAddress()) {
            return true;
        }
    }

    QueuedModbusReply *reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeRead, readBlockStatusDataUnit(), this);
    connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
    connect(reply, &QueuedModbusReply::finished, this, [this, reply](){

        if (m_currentReply == reply)
            m_currentReply = nullptr;

        if (reply->error() != QModbusDevice::NoError) {
            QTimer::singleShot(0, this, &PceWallbox::sendNextRequest);
            return;
        }

        const QModbusDataUnit unit = reply->reply()->result();
        const QVector<quint16> blockValues = unit.values();
        processBlockStatusRegisterValues(blockValues);

        QTimer::singleShot(0, this, &PceWallbox::sendNextRequest);
    });

    enqueueRequest(reply);

    // charging current register. Contains
    // - power state
    // - chargingcurrent (if power is true)
    // - phases (if power is true)
    bool chargingCurrentQueued = false;
    foreach (QueuedModbusReply *r, m_queue) {
        if (r->dataUnit().startAddress() == chargingCurrentDataUnit().startAddress()) {
            chargingCurrentQueued = true;
            break;
        }
    }

    if (!chargingCurrentQueued) {
        reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeRead, chargingCurrentDataUnit(), this);
        connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
        connect(reply, &QueuedModbusReply::finished, this, [this, reply](){

            if (m_currentReply == reply)
                m_currentReply = nullptr;

            if (reply->error() != QModbusDevice::NoError) {
                QTimer::singleShot(0, this, &PceWallbox::sendNextRequest);
                return;
            }

            const QModbusDataUnit unit = reply->reply()->result();
            const QVector<quint16> values = unit.values();
            processChargingCurrentRegisterValues(values);

            QTimer::singleShot(0, this, &PceWallbox::sendNextRequest);
        });

        enqueueRequest(reply);
    }

    // Digital input
    bool digitalInputAlreadyQueued = false;
    foreach (QueuedModbusReply *r, m_queue) {
        if (r->dataUnit().startAddress() == digitalInputModeDataUnit().startAddress()) {
            digitalInputAlreadyQueued = true;
            break;
        }
    }

    if (!digitalInputAlreadyQueued) {
        reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeRead, digitalInputModeDataUnit(), this);
        connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
        connect(reply, &QueuedModbusReply::finished, this, [this, reply](){

            if (m_currentReply == reply)
                m_currentReply = nullptr;

            if (reply->error() != QModbusDevice::NoError) {
                QTimer::singleShot(0, this, &PceWallbox::sendNextRequest);
                return;
            }

            const QModbusDataUnit unit = reply->reply()->result();
            const QVector<quint16> values = unit.values();
            processDigitalInputModeRegisterValues(values);

            QTimer::singleShot(0, this, &PceWallbox::sendNextRequest);
        });

        enqueueRequest(reply);
    }


    // Led brightness
    bool ledBrightnessAlreadyQueued = false;
    foreach (QueuedModbusReply *r, m_queue) {
        if (r->dataUnit().startAddress() == ledBrightnessDataUnit().startAddress()) {
            ledBrightnessAlreadyQueued = true;
            break;
        }
    }

    if (!ledBrightnessAlreadyQueued) {
        reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeRead, ledBrightnessDataUnit(), this);
        connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
        connect(reply, &QueuedModbusReply::finished, this, [this, reply](){

            if (m_currentReply == reply)
                m_currentReply = nullptr;

            if (reply->error() != QModbusDevice::NoError) {
                QTimer::singleShot(0, this, &PceWallbox::sendNextRequest);
                return;
            }

            const QModbusDataUnit unit = reply->reply()->result();
            const QVector<quint16> values = unit.values();
            processLedBrightnessRegisterValues(values);

            emit updateFinished();
            QTimer::singleShot(0, this, &PceWallbox::sendNextRequest);
        });

        enqueueRequest(reply);
    }
    return true;
}

QueuedModbusReply *PceWallbox::setChargingCurrent(quint16 chargingCurrent)
{
    if (m_aboutToDelete)
        return nullptr;

    QueuedModbusReply *reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeWrite, setChargingCurrentDataUnit(chargingCurrent), this);

    connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
    connect(reply, &QueuedModbusReply::finished, this, [this, reply](){
        if (m_currentReply == reply)
            m_currentReply = nullptr;

        QTimer::singleShot(0, this, &PceWallbox::sendNextRequest);
        return;
    });

    enqueueRequest(reply, true);
    return reply;
}

QueuedModbusReply *PceWallbox::setLedBrightness(quint16 percentage)
{
    if (m_aboutToDelete)
        return nullptr;

    QueuedModbusReply *reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeWrite, setLedBrightnessDataUnit(percentage), this);

    connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
    connect(reply, &QueuedModbusReply::finished, this, [this, reply](){
        if (m_currentReply == reply)
            m_currentReply = nullptr;

        QTimer::singleShot(0, this, &PceWallbox::sendNextRequest);
        return;
    });

    enqueueRequest(reply, true);
    return reply;
}

QueuedModbusReply *PceWallbox::setDigitalInputMode(DigitalInputMode digitalInputMode)
{
    if (m_aboutToDelete)
        return nullptr;

    QueuedModbusReply *reply = new QueuedModbusReply(QueuedModbusReply::RequestTypeWrite, setDigitalInputModeDataUnit(digitalInputMode), this);

    connect(reply, &QueuedModbusReply::finished, reply, &QueuedModbusReply::deleteLater);
    connect(reply, &QueuedModbusReply::finished, this, [this, reply](){
        if (m_currentReply == reply)
            m_currentReply = nullptr;

        QTimer::singleShot(0, this, &PceWallbox::sendNextRequest);
        return;
    });

    enqueueRequest(reply, true);
    return reply;

}

void PceWallbox::gracefullDeleteLater()
{
    // Clean up the queue
    m_aboutToDelete = true;
    cleanupQueue();

    m_timer.stop();

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
    connect(reply, &QueuedModbusReply::finished, this, [this, reply](){
        if (m_currentReply == reply)
            m_currentReply = nullptr;

        if (reply->error() != QModbusDevice::NoError) {
            qCWarning(dcPcElectric()) << "Failed to send heartbeat to" << m_modbusTcpMaster->hostAddress().toString() << reply->errorString();
        } else {
            qCDebug(dcPcElectric()) << "Successfully sent heartbeat to" << m_modbusTcpMaster->hostAddress().toString();
        }

        QTimer::singleShot(0, this, &PceWallbox::sendNextRequest);
        return;
    });

    enqueueRequest(reply, true);
}

void PceWallbox::sendNextRequest()
{
    if (m_queue.isEmpty())
        return;

    if (m_currentReply)
        return;

    if (m_aboutToDelete) {
        disconnect(this, nullptr, nullptr, nullptr);
        disconnectDevice();
        deleteLater();
        return;
    }

    m_currentReply = m_queue.dequeue();
    switch(m_currentReply->requestType()) {
    case QueuedModbusReply::RequestTypeRead:
        qCDebug(dcPcElectric()) << "--> Reading" << ModbusDataUtils::registerTypeToString(m_currentReply->dataUnit().registerType())
                                << "register:" << m_currentReply->dataUnit().startAddress()
                                << "length" << m_currentReply->dataUnit().valueCount();
        m_currentReply->setReply(m_modbusTcpMaster->sendReadRequest(m_currentReply->dataUnit(), m_slaveId));
        break;
    case QueuedModbusReply::RequestTypeWrite:
        qCDebug(dcPcElectric()) << "--> Writing" << ModbusDataUtils::registerTypeToString(m_currentReply->dataUnit().registerType())
                                << "register:" << m_currentReply->dataUnit().startAddress()
                                << "length:" << m_currentReply->dataUnit().valueCount()
                                << "values:" << m_currentReply->dataUnit().values();
        m_currentReply->setReply(m_modbusTcpMaster->sendWriteRequest(m_currentReply->dataUnit(), m_slaveId));
        break;
    }

    if (!m_currentReply->reply()) {
        qCWarning(dcPcElectric()) << "Error occurred while sending" << m_currentReply->requestType()
        << ModbusDataUtils::registerTypeToString(m_currentReply->dataUnit().registerType())
        << "register:" << m_currentReply->dataUnit().startAddress()
        << "length:" << m_currentReply->dataUnit().valueCount()
        << "to" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
        m_currentReply->deleteLater();
        m_currentReply = nullptr;
        QTimer::singleShot(0, this, &PceWallbox::sendNextRequest);
        return;
    }

    if (m_currentReply->reply()->isFinished()) {
        qCWarning(dcPcElectric()) << "Reply immediatly finished";
        m_currentReply->deleteLater();
        m_currentReply = nullptr;
        QTimer::singleShot(0, this, &PceWallbox::sendNextRequest);
        return;
    }
}

void PceWallbox::enqueueRequest(QueuedModbusReply *reply, bool prepend)
{
    if (prepend) {
        m_queue.prepend(reply);
    } else {
        m_queue.enqueue(reply);
    }

    QTimer::singleShot(0, this, &PceWallbox::sendNextRequest);
}

void PceWallbox::cleanupQueue()
{
    qDeleteAll(m_queue);
    m_queue.clear();
}

QDebug operator<<(QDebug debug, const PceWallbox::ChargingCurrentState &chargingCurrentState)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "ChargingCurrentState(" << chargingCurrentState.power << ", " << chargingCurrentState.maxChargingCurrent << " [A], " << chargingCurrentState.desiredPhaseCount << ')';
    return debug;
}
