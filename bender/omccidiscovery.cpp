// SPDX-License-Identifier: GPL-3.0-or-later

#include "omccidiscovery.h"
#include "extern-plugininfo.h"

OmcciDiscovery::OmcciDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent) :
    QObject{parent},
    m_networkDeviceDiscovery{networkDeviceDiscovery}
{
    m_gracePeriodTimer.setSingleShot(true);
    m_gracePeriodTimer.setInterval(3000);
    connect(&m_gracePeriodTimer, &QTimer::timeout, this, [this]() {
        qCDebug(dcBender()) << "OMCCI discovery: Grace period timer triggered.";
        finishDiscovery();
    });
}

void OmcciDiscovery::startDiscovery()
{
    qCInfo(dcBender()) << "OMCCI discovery: Searching for Bender charge controllers in the network...";
    m_startDateTime = QDateTime::currentDateTime();

    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::hostAddressDiscovered, this, &OmcciDiscovery::checkNetworkDevice);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=]() {
        qCDebug(dcBender()) << "OMCCI discovery: Network discovery finished. Found" << discoveryReply->networkDeviceInfos().length() << "network devices.";
        m_networkDeviceInfos = discoveryReply->networkDeviceInfos();
        m_gracePeriodTimer.start();
    });
}

QList<OmcciDiscovery::Result> OmcciDiscovery::discoveryResults() const
{
    return m_discoveryResults;
}

void OmcciDiscovery::checkNetworkDevice(const QHostAddress &address)
{
    const int port = 502;
    const int slaveId = 0xff;
    qCDebug(dcBender()) << "OMCCI discovery: Checking network device:" << address.toString() << "Port:" << port << "Slave ID:" << slaveId;

    BenderOmcciModbusTcpConnection *connection = new BenderOmcciModbusTcpConnection(address, port, slaveId, this);
    m_connections.append(connection);

    connect(connection, &BenderOmcciModbusTcpConnection::reachableChanged, this, [=](bool reachable) {
        if (!reachable) {
            cleanupConnection(connection);
            return;
        }

        connect(connection, &BenderOmcciModbusTcpConnection::initializationFinished, this, [=](bool success) {
            if (!success) {
                qCDebug(dcBender()) << "OMCCI discovery: Initialization failed on" << address.toString();
                cleanupConnection(connection);
                return;
            }

            Result result;
            result.address = address;
            result.serialNumber = connection->serialNumber().trimmed();
            result.firmwareVersion = connection->firmwareVersion().trimmed();

            if (result.serialNumber.isEmpty() && result.firmwareVersion.isEmpty()) {
                qCDebug(dcBender()) << "OMCCI discovery: Found a Modbus device but no Bender identity registers on" << address.toString();
                cleanupConnection(connection);
                return;
            }

            qCDebug(dcBender()) << "OMCCI discovery: Found Bender controller:" << result.serialNumber << result.firmwareVersion << address.toString();
            m_discoveryResults.append(result);
            cleanupConnection(connection);
        });

        if (!connection->initialize()) {
            qCDebug(dcBender()) << "OMCCI discovery: Unable to initialize connection on" << address.toString();
            cleanupConnection(connection);
        }
    });

    connect(connection, &BenderOmcciModbusTcpConnection::checkReachabilityFailed, this, [=]() {
        qCDebug(dcBender()) << "OMCCI discovery: Reachability check failed on" << address.toString();
        cleanupConnection(connection);
    });

    connection->connectDevice();
}

void OmcciDiscovery::cleanupConnection(BenderOmcciModbusTcpConnection *connection)
{
    m_connections.removeAll(connection);
    connection->disconnectDevice();
    connection->deleteLater();
}

void OmcciDiscovery::finishDiscovery()
{
    const qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    for (int i = 0; i < m_discoveryResults.length(); ++i)
        m_discoveryResults[i].networkDeviceInfo = m_networkDeviceInfos.get(m_discoveryResults.at(i).address);

    foreach (BenderOmcciModbusTcpConnection *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcBender()) << "OMCCI discovery: Finished. Found" << m_discoveryResults.length()
                       << "Bender charge controllers in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    m_gracePeriodTimer.stop();

    emit discoveryFinished();
}
