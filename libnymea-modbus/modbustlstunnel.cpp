// SPDX-License-Identifier: LGPL-3.0-or-later

#include "modbustlstunnel_p.h"

#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QSslSocket>
#include <QTcpSocket>

ModbusTlsTunnel::ModbusTlsTunnel(QObject *parent)
    : QObject(parent)
{
    connect(&m_server, &QTcpServer::newConnection, this, &ModbusTlsTunnel::acceptLoopbackConnection);
}

void ModbusTlsTunnel::start(const QHostAddress &address, quint16 port, const QString &serverName,
                            const QSslConfiguration &configuration, const QString &acceptedFingerprint)
{
    stop();
    m_stopping = false;
    m_acceptedFingerprint = acceptedFingerprint;
    m_peerCertificateReported = false;
    m_sslErrorsEncountered = false;
    m_peerVerificationFailureReported = false;

    m_sslSocket = new QSslSocket(this);
    QSslConfiguration sslConfiguration = configuration;
    sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyPeer);
    m_sslSocket->setSslConfiguration(sslConfiguration);
    if (!serverName.isEmpty())
        m_sslSocket->setPeerVerifyName(serverName);

    connect(m_sslSocket, &QSslSocket::connected, this, &ModbusTlsTunnel::tcpConnected);
    connect(m_sslSocket, QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors),
            this, [this](const QList<QSslError> &errors) {
        m_sslErrorsEncountered = true;
        emit sslErrors(errors);
        if (verifyPeerCertificate())
            m_sslSocket->ignoreSslErrors(errors);
    });
    connect(m_sslSocket, &QSslSocket::encrypted, this, [this]() {
        if (!verifyPeerCertificate()) {
            emit tunnelError(tr("The TLS peer certificate was not accepted."));
            m_sslSocket->abort();
            return;
        }
        createLoopbackServer();
    });
    connect(m_sslSocket, &QSslSocket::disconnected, this, [this]() {
        if (m_loopbackSocket)
            m_loopbackSocket->disconnectFromHost();
        m_server.close();
        if (!m_stopping)
            emit disconnected();
    });
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    connect(m_sslSocket, &QSslSocket::errorOccurred, this, [this](QAbstractSocket::SocketError) {
#else
    connect(m_sslSocket, QOverload<QAbstractSocket::SocketError>::of(&QSslSocket::error), this, [this](QAbstractSocket::SocketError) {
#endif
        if (!m_stopping)
            emit tunnelError(m_sslSocket->errorString());
    });

    const QString hostName = address.toString();
    m_sslSocket->connectToHostEncrypted(hostName, port, serverName.isEmpty() ? hostName : serverName);
}

void ModbusTlsTunnel::stop()
{
    m_stopping = true;
    m_server.close();
    if (m_loopbackSocket) {
        m_loopbackSocket->disconnect(this);
        m_loopbackSocket->abort();
        m_loopbackSocket->deleteLater();
        m_loopbackSocket = nullptr;
    }
    if (m_sslSocket) {
        m_sslSocket->disconnect(this);
        m_sslSocket->abort();
        m_sslSocket->deleteLater();
        m_sslSocket = nullptr;
    }
}

void ModbusTlsTunnel::setAcceptedFingerprint(const QString &fingerprint)
{
    m_acceptedFingerprint = fingerprint;
}

QString ModbusTlsTunnel::certificateFingerprint() const
{
    if (!m_sslSocket)
        return QString();
    const QSslCertificate certificate = m_sslSocket->peerCertificate();
    return QString::fromLatin1(certificate.digest(QCryptographicHash::Sha256).toHex());
}

bool ModbusTlsTunnel::verifyPeerCertificate()
{
    if (!m_sslSocket)
        return false;

    const QSslCertificate certificate = m_sslSocket->peerCertificate();
    const QString fingerprint = certificateFingerprint();
    if (!certificate.isNull() && !m_peerCertificateReported) {
        m_peerCertificateReported = true;
        emit peerCertificateAvailable(certificate, fingerprint);
    }

    if (!certificate.isNull()) {
        // With no pin, use Qt's normal CA and hostname validation. Reaching
        // encrypted() without sslErrors means that validation succeeded.
        if (m_acceptedFingerprint.isEmpty() && !m_sslErrorsEncountered)
            return true;

        // A configured leaf-certificate pin deliberately overrides CA errors.
        if (!m_acceptedFingerprint.isEmpty() && fingerprint == m_acceptedFingerprint)
            return true;
    }

    if (!m_peerVerificationFailureReported) {
        m_peerVerificationFailureReported = true;
        emit peerVerificationFailed(m_acceptedFingerprint, fingerprint);
    }
    return false;
}

void ModbusTlsTunnel::createLoopbackServer()
{
    // QModbusTcpClient does not expose a socket injection or local-bind API,
    // so the TLS adapter must use a TCP bridge. Bind to an unpredictable
    // address within 127/8 as well as an ephemeral port to make blind races
    // against the short-lived listener impractical.
    bool listening = false;
    for (int attempt = 0; attempt < 8 && !listening; ++attempt) {
        const quint32 address = (127u << 24)
            | (QRandomGenerator::global()->bounded(1u, 255u) << 16)
            | (QRandomGenerator::global()->generate() & 0xffffu);
        listening = m_server.listen(QHostAddress(address), 0);
    }
    if (!listening) {
        emit tunnelError(tr("Could not create the local Modbus TLS bridge: %1").arg(m_server.errorString()));
        m_sslSocket->abort();
        return;
    }
    emit encrypted(m_server.serverAddress(), m_server.serverPort(), m_sslSocket->sslConfiguration());
}

void ModbusTlsTunnel::acceptLoopbackConnection()
{
    if (m_loopbackSocket || !m_sslSocket) {
        while (m_server.hasPendingConnections())
            m_server.nextPendingConnection()->deleteLater();
        return;
    }

    m_loopbackSocket = m_server.nextPendingConnection();
    m_server.close();
    connect(m_loopbackSocket, &QTcpSocket::readyRead, this, [this]() {
        if (m_sslSocket)
            m_sslSocket->write(m_loopbackSocket->readAll());
    });
    connect(m_sslSocket, &QSslSocket::readyRead, this, [this]() {
        if (m_loopbackSocket)
            m_loopbackSocket->write(m_sslSocket->readAll());
    });
    connect(m_loopbackSocket, &QTcpSocket::disconnected, this, [this]() {
        if (m_sslSocket)
            m_sslSocket->disconnectFromHost();
    });
}
