// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef MODBUSTLSTUNNEL_P_H
#define MODBUSTLSTUNNEL_P_H

#include <QHostAddress>
#include <QSslCertificate>
#include <QSslConfiguration>
#include <QSslError>
#include <QTcpServer>
#include <QTimer>

class QSslSocket;
class QTcpSocket;

class ModbusTlsTunnel : public QObject
{
    Q_OBJECT
public:
    explicit ModbusTlsTunnel(QObject *parent = nullptr);

    void start(const QHostAddress &address, quint16 port, const QString &serverName,
               const QSslConfiguration &configuration, const QString &acceptedFingerprint);
    void stop();
    void setAcceptedFingerprint(const QString &fingerprint);

signals:
    void tcpConnected();
    void peerCertificateAvailable(const QSslCertificate &certificate, const QString &sha256Fingerprint);
    void sslErrors(const QList<QSslError> &errors);
    void peerVerificationFailed(const QString &expectedFingerprint, const QString &actualFingerprint);
    void encrypted(const QHostAddress &loopbackAddress, quint16 loopbackPort, const QSslConfiguration &configuration);
    void sessionTicketReceived(const QByteArray &ticket);
    void tunnelError(const QString &errorString);
    void disconnected();

private:
    QString certificateFingerprint() const;
    bool verifyPeerCertificate();
    void createLoopbackServer();
    void acceptLoopbackConnection();

    QTcpServer m_server;
    QTimer m_handshakeTimer;
    QSslSocket *m_sslSocket = nullptr;
    QTcpSocket *m_loopbackSocket = nullptr;
    QString m_acceptedFingerprint;
    bool m_stopping = false;
    bool m_peerCertificateReported = false;
    bool m_sslErrorsEncountered = false;
    bool m_peerVerificationFailureReported = false;
};

#endif // MODBUSTLSTUNNEL_P_H
