// SPDX-License-Identifier: GPL-3.0-or-later

#include "eboxprofessionaldiscovery.h"
#include "extern-plugininfo.h"

namespace {

QString normalizedString(QString value)
{
    value.remove(QChar('\0'));
    return value.trimmed();
}

}

EBoxProfessionalDiscovery::EBoxProfessionalDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent) :
    QObject(parent),
    m_networkDeviceDiscovery(networkDeviceDiscovery)
{
    m_gracePeriodTimer.setSingleShot(true);
    m_gracePeriodTimer.setInterval(3000);
    connect(&m_gracePeriodTimer, &QTimer::timeout, this, &EBoxProfessionalDiscovery::finishDiscovery);
}

void EBoxProfessionalDiscovery::startDiscovery()
{
    qCInfo(dcPhoenixConnect()) << "Discovery: Searching for Compleo eBOX professional wallboxes on ports" << m_ports;
    m_startDateTime = QDateTime::currentDateTime();

    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::hostAddressDiscovered, this,
            [this](const QHostAddress &address) { checkNetworkDevice(address); });
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [this, discoveryReply]() {
        m_networkDeviceInfos = discoveryReply->networkDeviceInfos();
        discoveryReply->deleteLater();
        m_gracePeriodTimer.start();
    });
}

QList<EBoxProfessionalDiscovery::Result> EBoxProfessionalDiscovery::results() const
{
    return m_results;
}

void EBoxProfessionalDiscovery::checkNetworkDevice(const QHostAddress &address)
{
    for (quint16 port : m_ports)
        checkNetworkDevice(address, port);
}

void EBoxProfessionalDiscovery::checkNetworkDevice(const QHostAddress &address, quint16 port)
{
    constexpr quint16 slaveId = 1;
    EBoxProfessionalModbusTcpConnection *connection = new EBoxProfessionalModbusTcpConnection(address, port, slaveId, this);
    m_connections.append(connection);

    connect(connection, &EBoxProfessionalModbusTcpConnection::reachableChanged, this, [this, connection, address, port](bool reachable) {
        if (!reachable) {
            cleanupConnection(connection);
            return;
        }

        connect(connection, &EBoxProfessionalModbusTcpConnection::initializationFinished, this, [this, connection, address, port](bool success) {
            const QString chargeboxId = normalizedString(connection->chargeboxId());
            const QString serialNumber = normalizedString(connection->serialNumber());
            if (!success || connection->modbusTableVersion() == 0 || chargeboxId.isEmpty() || serialNumber.isEmpty()) {
                qCDebug(dcPhoenixConnect()) << "Discovery: Device on" << address << "port" << port
                                            << "does not match the Compleo eBOX register map";
                cleanupConnection(connection);
                return;
            }

            Result result;
            result.chargeboxId = chargeboxId;
            result.serialNumber = serialNumber;
            result.firmwareVersion = normalizedString(connection->firmwareVersion());
            result.modbusTableVersion = connection->modbusTableVersion();
            result.port = port;
            result.address = address;

            // Some installations expose the same Modbus endpoint on both ports. Keep one
            // descriptor per device and prefer the standard Modbus port when both work.
            int existingResultIndex = -1;
            for (int i = 0; i < m_results.size(); ++i) {
                if (m_results.at(i).address == address || m_results.at(i).serialNumber == serialNumber) {
                    existingResultIndex = i;
                    break;
                }
            }
            if (existingResultIndex < 0) {
                m_results.append(result);
            } else if (port == 502) {
                m_results[existingResultIndex] = result;
            }

            qCInfo(dcPhoenixConnect()) << "Discovery: Found Compleo eBOX professional" << result.chargeboxId
                                      << result.serialNumber << "at" << address << "port" << port;
            cleanupConnection(connection);
        });

        if (!connection->initialize())
            cleanupConnection(connection);
    });

    connect(connection, &EBoxProfessionalModbusTcpConnection::checkReachabilityFailed, this, [this, connection]() {
        cleanupConnection(connection);
    });

    connection->connectDevice();
}

void EBoxProfessionalDiscovery::cleanupConnection(EBoxProfessionalModbusTcpConnection *connection)
{
    if (!m_connections.removeOne(connection))
        return;

    connection->disconnectDevice();
    connection->deleteLater();
}

void EBoxProfessionalDiscovery::finishDiscovery()
{
    for (int i = 0; i < m_results.size(); ++i)
        m_results[i].networkDeviceInfo = m_networkDeviceInfos.get(m_results.at(i).address);

    const auto connections = m_connections;
    for (EBoxProfessionalModbusTcpConnection *connection : connections)
        cleanupConnection(connection);

    m_gracePeriodTimer.stop();
    emit discoveryFinished();
}
