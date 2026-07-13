// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef EBOXPROFESSIONALDISCOVERY_H
#define EBOXPROFESSIONALDISCOVERY_H

#include <QObject>
#include <QTimer>

#include <network/networkdevicediscovery.h>

#include "eboxprofessionalmodbustcpconnection.h"

class EBoxProfessionalDiscovery : public QObject
{
    Q_OBJECT

public:
    struct Result {
        QString chargeboxId;
        QString serialNumber;
        QString firmwareVersion;
        quint16 modbusTableVersion = 0;
        quint16 port = 0;
        QHostAddress address;
        NetworkDeviceInfo networkDeviceInfo;
    };

    explicit EBoxProfessionalDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent = nullptr);

    void startDiscovery();
    QList<Result> results() const;

signals:
    void discoveryFinished();

private:
    void checkNetworkDevice(const QHostAddress &address);
    void checkNetworkDevice(const QHostAddress &address, quint16 port);
    void cleanupConnection(EBoxProfessionalModbusTcpConnection *connection);
    void finishDiscovery();

    NetworkDeviceDiscovery *m_networkDeviceDiscovery = nullptr;
    const QList<quint16> m_ports = {502, 5555};
    QTimer m_gracePeriodTimer;
    QDateTime m_startDateTime;
    NetworkDeviceInfos m_networkDeviceInfos;
    QList<EBoxProfessionalModbusTcpConnection *> m_connections;
    QList<Result> m_results;
};

#endif // EBOXPROFESSIONALDISCOVERY_H
