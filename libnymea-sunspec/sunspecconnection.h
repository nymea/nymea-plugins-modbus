#ifndef SUNSPECCONNECTION_H
#define SUNSPECCONNECTION_H

#include <QObject>
#include <QHostAddress>
#include <QModbusTcpClient>

#include "sunspec.h"

class SunSpecConnection : public QObject
{
    Q_OBJECT
public:
    explicit SunSpecConnection(const QHostAddress &hostAddress, uint port = 502, uint slaveId = 1, QObject *parent = nullptr);
    ~SunSpecConnection() = default;

    QModbusTcpClient *modbusTcpClient() const;

    QHostAddress hostAddress() const;
    void setHostAddress(const QHostAddress &hostAddress);

    uint port() const;
    void setPort(uint port);

    uint slaveId() const;
    void setSlaveId(uint slaveId);

    int timeout() const;
    void setTimeout(int milliSeconds);

    uint numberOfRetries() const;
    void setNumberOfRetries(uint retries);

    bool connected() const;

    bool connectDevice();
    void disconnectDevice();

public slots:
    bool startSunSpecDiscovery();

signals:
    void connectedChanged(bool connected);
    void sunspecBaseRegisterFound(quint16 baseRegister);
    void sunSpecDiscoveryFinished(bool success);

private:
    QModbusTcpClient *m_modbusTcpClient = nullptr;
    QHostAddress m_hostAddress;
    uint m_port;
    int m_slaveId = 1;
    QTimer m_reconnectTimer;

    quint16 m_baseRegister = 40000;
    QQueue<quint16> m_baseRegisterQueue;
    QHash<quint16, quint16> m_modelDiscoveryResult;

    bool scanSunspecBaseRegister(quint16 baseRegister);
    void scanNextSunspecBaseRegister();

    void scanModelsOnBaseRegister(quint16 offset = 2);

};

QDebug operator<<(QDebug debug, SunSpecConnection *connection);


#endif // SUNSPECCONNECTION_H
