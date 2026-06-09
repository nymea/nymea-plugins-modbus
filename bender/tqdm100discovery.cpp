// SPDX-License-Identifier: GPL-3.0-or-later

#include "tqdm100discovery.h"
#include "extern-plugininfo.h"

TqDm100Discovery::TqDm100Discovery(NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent) :
    QObject{parent},
    m_networkDeviceDiscovery{networkDeviceDiscovery}
{
    m_gracePeriodTimer.setSingleShot(true);
    m_gracePeriodTimer.setInterval(3000);
    connect(&m_gracePeriodTimer, &QTimer::timeout, this, [this]() {
        qCDebug(dcBender()) << "TQ-DM100 discovery: Grace period timer triggered.";
        finishDiscovery();
    });
}

void TqDm100Discovery::startDiscovery()
{
    qCInfo(dcBender()) << "TQ-DM100 discovery: Searching for Bender charge controllers in the network...";
    m_startDateTime = QDateTime::currentDateTime();

    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::hostAddressDiscovered, this, &TqDm100Discovery::checkNetworkDevice);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=]() {
        qCDebug(dcBender()) << "TQ-DM100 discovery: Network discovery finished. Found" << discoveryReply->networkDeviceInfos().length() << "network devices.";
        m_networkDeviceInfos = discoveryReply->networkDeviceInfos();
        m_gracePeriodTimer.start();
    });
}

QList<TqDm100Discovery::Result> TqDm100Discovery::discoveryResults() const
{
    return m_discoveryResults;
}

void TqDm100Discovery::checkNetworkDevice(const QHostAddress &address)
{
    const int port = 502;
    const int slaveId = 0xff;
    qCDebug(dcBender()) << "TQ-DM100 discovery: Checking network device:" << address.toString() << "Port:" << port << "Slave ID:" << slaveId;

    BenderTqDm100ModbusTcpConnection *connection = new BenderTqDm100ModbusTcpConnection(address, port, slaveId, this);
    m_connections.append(connection);

    connect(connection, &BenderTqDm100ModbusTcpConnection::reachableChanged, this, [=](bool reachable) {
        if (!reachable) {
            cleanupConnection(connection);
            return;
        }

        connect(connection, &BenderTqDm100ModbusTcpConnection::initializationFinished, this, [=](bool success) {
            if (!success) {
                qCDebug(dcBender()) << "TQ-DM100 discovery: Initialization failed on" << address.toString();
                cleanupConnection(connection);
                return;
            }

            Result result;
            result.address = address;
            qCDebug(dcBender()) << "TQ-DM100 discovery: Found Bender controller on" << address.toString();
            m_discoveryResults.append(result);
            cleanupConnection(connection);
        });

        if (!connection->initialize()) {
            qCDebug(dcBender()) << "TQ-DM100 discovery: Unable to initialize connection on" << address.toString();
            cleanupConnection(connection);
        }
    });

    connect(connection, &BenderTqDm100ModbusTcpConnection::checkReachabilityFailed, this, [=]() {
        qCDebug(dcBender()) << "TQ-DM100 discovery: Reachability check failed on" << address.toString();
        cleanupConnection(connection);
    });

    connection->connectDevice();
}

void TqDm100Discovery::cleanupConnection(BenderTqDm100ModbusTcpConnection *connection)
{
    m_connections.removeAll(connection);
    connection->disconnectDevice();
    connection->deleteLater();
}

void TqDm100Discovery::finishDiscovery()
{
    const qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    for (int i = 0; i < m_discoveryResults.length(); ++i)
        m_discoveryResults[i].networkDeviceInfo = m_networkDeviceInfos.get(m_discoveryResults.at(i).address);

    foreach (BenderTqDm100ModbusTcpConnection *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcBender()) << "TQ-DM100 discovery: Finished. Found" << m_discoveryResults.length()
                       << "Bender charge controllers in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    m_gracePeriodTimer.stop();

    emit discoveryFinished();
}
