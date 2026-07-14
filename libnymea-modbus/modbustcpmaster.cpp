// SPDX-License-Identifier: LGPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of libnymea-modbus.
*
* libnymea-modbus is free software: you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* as published by the Free Software Foundation, either version 3
* of the License, or (at your option) any later version.
*
* libnymea-modbus is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with libnymea-modbus. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "modbustcpmaster.h"
#include "modbustlstunnel_p.h"

#include <QRegularExpression>

Q_LOGGING_CATEGORY(dcModbusTcpMaster, "ModbusTcpMaster")

ModbusTcpMaster::ModbusTcpMaster(const QHostAddress &hostAddress, uint port, QObject *parent) :
    QObject(parent),
    m_hostAddress(hostAddress),
    m_port(port)
{
    m_modbusTcpClient = new QModbusTcpClient(this);
    m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, m_port);
    m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, m_hostAddress.toString());
    m_modbusTcpClient->setTimeout(m_timeout);
    m_modbusTcpClient->setNumberOfRetries(m_numberOfRetries);

    connect(m_modbusTcpClient, &QModbusTcpClient::stateChanged, this, &ModbusTcpMaster::onModbusStateChanged);
    connect(m_modbusTcpClient, &QModbusTcpClient::errorOccurred, this, &ModbusTcpMaster::onModbusErrorOccurred);

    setupTlsTunnel();

    m_reconnectTimer = new QTimer(this);
    m_reconnectTimer->setSingleShot(true);
    m_reconnectTimer->setInterval(4000);
    connect(m_reconnectTimer, &QTimer::timeout, this, [this]() {
        // QTimer::start(int) changes the persistent interval. Restore the
        // normal backoff before attempting a connection after an immediate
        // reconnect request.
        m_reconnectTimer->setInterval(4000);
        connectDevice();
    });
}

ModbusTcpMaster::~ModbusTcpMaster()
{
    if (m_reconnectTimer) {
        m_reconnectTimer->stop();
    }

    disconnectDevice();
}

QHostAddress ModbusTcpMaster::hostAddress() const
{
    return m_hostAddress;
}

uint ModbusTcpMaster::port() const
{
    return m_port;
}

void ModbusTcpMaster::setPort(uint port)
{
    m_port = port;
}

QString ModbusTcpMaster::connectionUrl() const
{
    return QString("%1%2:%3")
        .arg(m_transport == TransportTls ? QStringLiteral("tls://") : QString())
        .arg(m_hostAddress.toString()).arg(m_port);
}

void ModbusTcpMaster::setHostAddress(const QHostAddress &hostAddress)
{
    m_hostAddress = hostAddress;
}

ModbusTcpMaster::Transport ModbusTcpMaster::transport() const
{
    return m_transport;
}

void ModbusTcpMaster::setTransport(ModbusTcpMaster::Transport transport)
{
    m_transport = transport;
}

QSslConfiguration ModbusTcpMaster::tlsConfiguration() const
{
    return m_tlsConfiguration;
}

void ModbusTcpMaster::setTlsConfiguration(const QSslConfiguration &configuration)
{
    m_tlsConfiguration = configuration;
}

QString ModbusTcpMaster::tlsServerName() const
{
    return m_tlsServerName;
}

void ModbusTcpMaster::setTlsServerName(const QString &serverName)
{
    m_tlsServerName = serverName;
}

QString ModbusTcpMaster::acceptedPeerCertificateFingerprint() const
{
    return m_acceptedPeerCertificateFingerprint;
}

bool ModbusTcpMaster::setAcceptedPeerCertificateFingerprint(const QString &fingerprint)
{
    QString normalized = fingerprint.toLower();
    normalized.remove(QRegularExpression(QStringLiteral("[:\\s]")));
    if (!normalized.isEmpty() && !QRegularExpression(QStringLiteral("^[0-9a-f]{64}$")).match(normalized).hasMatch())
        return false;

    m_acceptedPeerCertificateFingerprint = normalized;
    if (m_tlsTunnel)
        m_tlsTunnel->setAcceptedFingerprint(normalized);
    return true;
}

QString ModbusTcpMaster::peerCertificateFingerprint() const
{
    return m_peerCertificateFingerprint;
}

QSslCertificate ModbusTcpMaster::peerCertificate() const
{
    return m_peerCertificate;
}

QSslConfiguration ModbusTcpMaster::negotiatedTlsConfiguration() const
{
    return m_negotiatedTlsConfiguration;
}

bool ModbusTcpMaster::connectDevice()
{
    if (!m_modbusTcpClient)
        return false;

    m_connectionRequested = true;
    m_tlsErrorString.clear();

    if (m_tlsConnecting || m_modbusTcpClient->state() != QModbusDevice::UnconnectedState) {
        qCWarning(dcModbusTcpMaster()) << "Connect modbus TCP device" << connectionUrl() << "called, but the socket is currently in the" << m_modbusTcpClient->state();
        return false;
    }

    qCDebug(dcModbusTcpMaster()) << "Connecting modbus client to" << connectionUrl();
    if (m_transport == TransportTls) {
        m_tlsConnecting = true;
        m_peerCertificate = QSslCertificate();
        m_peerCertificateFingerprint.clear();
        m_negotiatedTlsConfiguration = QSslConfiguration();
        m_tlsTunnel->start(m_hostAddress, static_cast<quint16>(m_port), m_tlsServerName,
                           m_tlsConfiguration, m_acceptedPeerCertificateFingerprint);
        return true;
    }

    connectModbusClient(m_hostAddress, static_cast<quint16>(m_port));
    return m_modbusTcpClient->connectDevice();
}

void ModbusTcpMaster::disconnectDevice()
{
    m_connectionRequested = false;
    m_tlsConnecting = false;
    m_immediateReconnectRequested = false;
    m_reconnectTimer->stop();
    m_reconnectTimer->setInterval(4000);
    m_modbusTcpClient->disconnectDevice();
    if (m_tlsTunnel)
        m_tlsTunnel->stop();
}

bool ModbusTcpMaster::reconnectDevice()
{
    qCWarning(dcModbusTcpMaster()) << "Reconnecting modbus TCP device" << connectionUrl();
    m_connectionRequested = true;
    m_tlsConnecting = false;
    m_immediateReconnectRequested = true;
    m_reconnectTimer->stop();
    if (m_tlsTunnel)
        m_tlsTunnel->stop();
    if (m_modbusTcpClient->state() == QModbusDevice::UnconnectedState) {
        m_immediateReconnectRequested = false;
        scheduleReconnect(0);
    } else {
        m_modbusTcpClient->disconnectDevice();
    }
    return true;
}

bool ModbusTcpMaster::connected() const
{
    return m_connected;
}

int ModbusTcpMaster::numberOfRetries() const
{
    return m_modbusTcpClient->numberOfRetries();
}

void ModbusTcpMaster::setNumberOfRetries(int number)
{
    m_numberOfRetries = number;
    m_modbusTcpClient->setNumberOfRetries(number);
}

int ModbusTcpMaster::timeout() const
{
    return m_modbusTcpClient->timeout();
}

void ModbusTcpMaster::setTimeout(int timeout)
{
    m_timeout = timeout;
    m_modbusTcpClient->setTimeout(timeout);
}

QString ModbusTcpMaster::errorString() const
{
    if (!m_tlsErrorString.isEmpty())
        return m_tlsErrorString;
    return m_modbusTcpClient->errorString();
}

QModbusDevice::Error ModbusTcpMaster::error() const
{
    if (!m_tlsErrorString.isEmpty())
        return QModbusDevice::ConnectionError;
    return m_modbusTcpClient->error();
}

QUuid ModbusTcpMaster::readCoil(uint slaveAddress, uint registerAddress, uint size)
{
    QUuid requestId = QUuid::createUuid();
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, registerAddress, size);

    if (QModbusReply *reply = m_modbusTcpClient->sendReadRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {
                if (reply->error() == QModbusDevice::NoError) {
                    emit readRequestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedCoil(reply->serverAddress(), modbusAddress, unit.values());
                } else {
                    emit readRequestExecuted(requestId, false);
                    qCWarning(dcModbusTcpMaster()) << "Read response error for device" << connectionUrl() << ":" << reply->error();
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){
                qCWarning(dcModbusTcpMaster()) << "Modbus reply error for device" << connectionUrl() << ":" << error;
                emit readRequestError(requestId, reply->errorString());
            });

            QTimer::singleShot(200, reply, &QModbusReply::deleteLater);
        } else {
            reply->deleteLater(); // broadcast replies return immediately
            return QUuid();
        }
    } else {
        qCWarning(dcModbusTcpMaster()) << "Read error for device" << connectionUrl() << ":" << m_modbusTcpClient->errorString();
        return QUuid();
    }
    return requestId;
}

QUuid ModbusTcpMaster::writeHoldingRegisters(uint slaveAddress, uint registerAddress, const QVector<quint16> &values)
{
    QUuid requestId = QUuid::createUuid();
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, registerAddress, values.length());
    request.setValues(values);
    if (QModbusReply *reply = m_modbusTcpClient->sendWriteRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {
                if (reply->error() == QModbusDevice::NoError) {
                    emit writeRequestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedHoldingRegister(reply->serverAddress(), modbusAddress, unit.values());
                } else {
                    emit writeRequestExecuted(requestId, false);
                    qCWarning(dcModbusTcpMaster()) << "Read response error for device" << connectionUrl() << ":" << reply->error();
                }
                reply->deleteLater();
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){
                qCWarning(dcModbusTcpMaster()) << "Modbus replay error for device" << connectionUrl() << ":" << error;
                emit writeRequestError(requestId, reply->errorString());
            });

            QTimer::singleShot(2000, reply, &QModbusReply::deleteLater);
        } else {
            reply->deleteLater(); // broadcast replies return immediately
            return QUuid();
        }
    } else {
        qCWarning(dcModbusTcpMaster()) << "Read error for device" << connectionUrl() << ":" << m_modbusTcpClient->errorString();
        return QUuid();
    }
    return requestId;
}

QModbusReply *ModbusTcpMaster::sendRawRequest(const QModbusRequest &request, int serverAddress)
{
    return m_modbusTcpClient->sendRawRequest(request, serverAddress);
}

QModbusReply *ModbusTcpMaster::sendReadRequest(const QModbusDataUnit &read, int serverAddress)
{
    return m_modbusTcpClient->sendReadRequest(read, serverAddress);
}

QModbusReply *ModbusTcpMaster::sendReadWriteRequest(const QModbusDataUnit &read, const QModbusDataUnit &write, int serverAddress)
{
    return m_modbusTcpClient->sendReadWriteRequest(read, write, serverAddress);
}

QModbusReply *ModbusTcpMaster::sendWriteRequest(const QModbusDataUnit &write, int serverAddress)
{
    return m_modbusTcpClient->sendWriteRequest(write, serverAddress);
}

QUuid ModbusTcpMaster::readDiscreteInput(uint slaveAddress, uint registerAddress, uint size)
{
    QUuid requestId = QUuid::createUuid();

    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::DiscreteInputs, registerAddress, size);

    if (QModbusReply *reply = m_modbusTcpClient->sendReadRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {
                if (reply->error() == QModbusDevice::NoError) {
                    emit readRequestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedDiscreteInput(reply->serverAddress(), modbusAddress, unit.values());
                } else {
                    emit readRequestExecuted(requestId, false);
                    qCWarning(dcModbusTcpMaster()) << "Read response error for device" << connectionUrl() << ":" << reply->error();
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [requestId, reply, this] (QModbusDevice::Error error){
                qCWarning(dcModbusTcpMaster()) << "Modbus replay error for device" << connectionUrl() << ":" << error;
                emit readRequestError(requestId, reply->errorString());
            });

            QTimer::singleShot(2000, reply, &QModbusReply::deleteLater);
        } else {
            reply->deleteLater(); // broadcast replies return immediately
            return QUuid();
        }
    } else {
        qCWarning(dcModbusTcpMaster()) << "Read error for device" << connectionUrl() << ":" << m_modbusTcpClient->errorString();
        return QUuid();
    }
    return requestId;
}

QUuid ModbusTcpMaster::readInputRegister(uint slaveAddress, uint registerAddress, uint size)
{
    if (!m_modbusTcpClient) {
        return QUuid();
    }

    QUuid requestId = QUuid::createUuid();
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, registerAddress, size);

    if (QModbusReply *reply = m_modbusTcpClient->sendReadRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {
                reply->deleteLater();
                if (reply->error() == QModbusDevice::NoError) {
                    emit readRequestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedInputRegister(reply->serverAddress(), modbusAddress, unit.values());
                } else {
                    emit readRequestExecuted(requestId, false);
                    qCWarning(dcModbusTcpMaster()) << "Read response error for device" << connectionUrl() << ":" << reply->error();
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){
                qCWarning(dcModbusTcpMaster()) << "Modbus reply error for device" << connectionUrl() << ":" << error;
                emit readRequestError(requestId, reply->errorString());
            });

            QTimer::singleShot(2000, reply, &QModbusReply::deleteLater);

        } else {
            reply->deleteLater(); // broadcast replies return immediately
            return QUuid();
        }
    } else {
        qCWarning(dcModbusTcpMaster()) << "Read error for device" << connectionUrl() << ":" << m_modbusTcpClient->errorString();
        return QUuid();
    }
    return requestId;
}

QUuid ModbusTcpMaster::readHoldingRegister(uint slaveAddress, uint registerAddress, uint size)
{
    QUuid requestId = QUuid::createUuid();
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, registerAddress, size);

    if (QModbusReply *reply = m_modbusTcpClient->sendReadRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] {

                if (reply->error() == QModbusDevice::NoError) {
                    emit writeRequestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedHoldingRegister(reply->serverAddress(), modbusAddress, unit.values());

                } else {
                    emit writeRequestExecuted(requestId, false);
                    qCWarning(dcModbusTcpMaster()) << "Read response error for device" << connectionUrl() << ":" << reply->error();
                    emit readRequestError(requestId, reply->errorString());
                }
                reply->deleteLater();
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){

                qCWarning(dcModbusTcpMaster()) << "Modbus reply error for device" << connectionUrl() << ":" << error;
                emit readRequestError(requestId, reply->errorString());
            });

            QTimer::singleShot(2000, reply, &QModbusReply::deleteLater);
        } else {
            reply->deleteLater(); // broadcast replies return immediately
            return QUuid();
        }
    } else {
        qCWarning(dcModbusTcpMaster()) << "Read error for device" << connectionUrl() << ":" << m_modbusTcpClient->errorString();
        return QUuid();
    }
    return requestId;
}

QUuid ModbusTcpMaster::writeCoil(uint slaveAddress, uint registerAddress, bool value)
{
    return writeCoils(slaveAddress, registerAddress, QVector<quint16>() << static_cast<quint16>(value));
}

QUuid ModbusTcpMaster::writeCoils(uint slaveAddress, uint registerAddress, const QVector<quint16> &values)
{
    QUuid requestId = QUuid::createUuid();
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, registerAddress, values.length());
    request.setValues(values);

    if (QModbusReply *reply = m_modbusTcpClient->sendWriteRequest(request, slaveAddress)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [reply, requestId, this] () {

                if (reply->error() == QModbusDevice::NoError) {
                    emit writeRequestExecuted(requestId, true);
                    const QModbusDataUnit unit = reply->result();
                    uint modbusAddress = unit.startAddress();
                    emit receivedCoil(reply->serverAddress(), modbusAddress, unit.values());

                } else {
                    emit writeRequestExecuted(requestId, false);
                    qCWarning(dcModbusTcpMaster()) << "Write response error for device" << connectionUrl() << ":" << reply->error();
                }
                reply->deleteLater();
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply, requestId, this] (QModbusDevice::Error error){
                qCWarning(dcModbusTcpMaster()) << "Modbus reply error for device" << connectionUrl() << ":" << error;
                emit writeRequestError(requestId, reply->errorString());
            });

            QTimer::singleShot(2000, reply, &QModbusReply::deleteLater);
        } else {
            reply->deleteLater(); // broadcast replies return immediately
            return QUuid();
        }
    } else {
        qCWarning(dcModbusTcpMaster()) << "Read error for device" << connectionUrl() << ":" << m_modbusTcpClient->errorString();
        return QUuid();
    }
    return requestId;
}

QUuid ModbusTcpMaster::writeHoldingRegister(uint slaveAddress, uint registerAddress, quint16 value)
{
    return writeHoldingRegisters(slaveAddress, registerAddress, QVector<quint16>() << value);
}

void ModbusTcpMaster::onModbusErrorOccurred(QModbusDevice::Error error)
{
    qCWarning(dcModbusTcpMaster()) << "An error occurred for device" << connectionUrl() << ":" << error;
    emit connectionErrorOccurred(error);
}

void ModbusTcpMaster::onModbusStateChanged(QModbusDevice::State state)
{
    qCDebug(dcModbusTcpMaster()) << "Connection state changed for" << m_hostAddress << state;
    bool connected = (state == QModbusDevice::ConnectedState);
    if (m_connected != connected) {
        m_connected = connected;
        emit connectionStateChanged(m_connected);
    }

    // If the socket is connected, stop the reconnect timer...
    // If the socket is unconnected (not connecting and not closing), start the reconnect timer
    if (m_connected) {
        m_reconnectTimer->stop();
        if (m_transport == TransportTcp)
            emit tcpConnectionEstablished();
    } else if (state == QModbusDevice::UnconnectedState && m_connectionRequested) {
        const int delay = m_immediateReconnectRequested ? 0 : 4000;
        m_immediateReconnectRequested = false;
        scheduleReconnect(delay);
    }
}

void ModbusTcpMaster::setupTlsTunnel()
{
    m_tlsTunnel = new ModbusTlsTunnel(this);
    connect(m_tlsTunnel, &ModbusTlsTunnel::tcpConnected, this, &ModbusTcpMaster::tcpConnectionEstablished);
    connect(m_tlsTunnel, &ModbusTlsTunnel::peerCertificateAvailable, this,
            [this](const QSslCertificate &certificate, const QString &fingerprint) {
        m_peerCertificate = certificate;
        m_peerCertificateFingerprint = fingerprint;
        emit peerCertificateAvailable(certificate, fingerprint);
    });
    connect(m_tlsTunnel, &ModbusTlsTunnel::sslErrors, this, &ModbusTcpMaster::tlsErrors);
    connect(m_tlsTunnel, &ModbusTlsTunnel::peerVerificationFailed, this,
            [this](const QString &expected, const QString &actual) {
        m_tlsErrorString = expected.isEmpty()
            ? tr("The TLS peer certificate has not been accepted.")
            : tr("The TLS peer certificate fingerprint does not match the accepted fingerprint.");
        emit tlsPeerVerificationFailed(expected, actual);
    });
    connect(m_tlsTunnel, &ModbusTlsTunnel::encrypted, this,
            [this](const QHostAddress &address, quint16 port, const QSslConfiguration &configuration) {
        m_negotiatedTlsConfiguration = configuration;
        emit tlsHandshakeFinished(configuration);
        connectModbusClient(address, port);
        const bool modbusConnectStarted = m_modbusTcpClient->connectDevice();
        m_tlsConnecting = false;
        if (!modbusConnectStarted) {
            m_tlsErrorString = tr("Could not connect the Modbus client to the local TLS bridge.");
            emit connectionErrorOccurred(QModbusDevice::ConnectionError);
            m_tlsTunnel->stop();
            scheduleReconnect();
        }
    });
    connect(m_tlsTunnel, &ModbusTlsTunnel::tunnelError, this, [this](const QString &errorString) {
        m_tlsConnecting = false;
        if (m_tlsErrorString.isEmpty())
            m_tlsErrorString = errorString;
        emit connectionErrorOccurred(QModbusDevice::ConnectionError);
        scheduleReconnect();
    });
    connect(m_tlsTunnel, &ModbusTlsTunnel::disconnected, this, &ModbusTcpMaster::handleTransportDisconnected);
}

void ModbusTcpMaster::connectModbusClient(const QHostAddress &address, quint16 port)
{
    m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
    m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, address.toString());
    m_modbusTcpClient->setTimeout(m_timeout);
    m_modbusTcpClient->setNumberOfRetries(m_numberOfRetries);
}

void ModbusTcpMaster::handleTransportDisconnected()
{
    m_tlsConnecting = false;
    if (m_modbusTcpClient->state() != QModbusDevice::UnconnectedState) {
        m_modbusTcpClient->disconnectDevice();
        return;
    }

    if (m_connected) {
        m_connected = false;
        emit connectionStateChanged(false);
    }
    if (m_connectionRequested)
        scheduleReconnect();
}

void ModbusTcpMaster::scheduleReconnect(int delay)
{
    if (!m_connectionRequested)
        return;

    m_reconnectTimer->setInterval(delay);
    m_reconnectTimer->start();
}
