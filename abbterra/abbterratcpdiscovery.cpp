// SPDX-License-Identifier: GPL-3.0-or-later

#include "abbterratcpdiscovery.h"
#include "abbterrautils.h"
#include "extern-plugininfo.h"

#include <QTime>
#include <QTimer>
#include <utility>

AbbTerraTcpDiscovery::AbbTerraTcpDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent)
    : QObject(parent),
      m_networkDeviceDiscovery(networkDeviceDiscovery)
{
}

void AbbTerraTcpDiscovery::startDiscovery()
{
    qCInfo(dcAbbTerra()) << "Discovery: Starting to search for ABB Terra AC chargers on the network...";

    m_startDateTime = QDateTime::currentDateTime();
    m_networkDeviceInfos.clear();
    m_temporaryResults.clear();
    m_results.clear();

    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::hostAddressDiscovered, this, &AbbTerraTcpDiscovery::checkNetworkDevice);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [this, discoveryReply]() {
        m_networkDeviceInfos = discoveryReply->networkDeviceInfos();
        QTimer::singleShot(3000, this, &AbbTerraTcpDiscovery::finishDiscovery);
    });
}

QList<AbbTerraTcpDiscovery::Result> AbbTerraTcpDiscovery::results() const
{
    return m_results;
}

void AbbTerraTcpDiscovery::checkNetworkDevice(const QHostAddress &address)
{
    AbbTerraModbusTcpConnection *connection = new AbbTerraModbusTcpConnection(address, 502, 1, this);
    m_connections.append(connection);

    connect(connection, &AbbTerraModbusTcpConnection::reachableChanged, this, [this, connection](bool reachable) {
        if (!reachable) {
            cleanupConnection(connection);
            return;
        }

        connection->initialize();
    });

    connect(connection, &AbbTerraModbusTcpConnection::initializationFinished, this, [this, connection, address](bool success) {
        if (!success) {
            cleanupConnection(connection);
            return;
        }

        const AbbTerraUtils::DeviceInfo deviceInfo = AbbTerraUtils::deviceInfoFromValues(connection->serialNumber(),
                                                                                         connection->firmwareVersionRaw(),
                                                                                         connection->userSettableMaxCurrent());
        if (deviceInfo.valid) {
            Result result;
            result.serialNumber = deviceInfo.serialNumber;
            result.productName = deviceInfo.productName;
            result.firmwareVersion = deviceInfo.firmwareVersion;
            result.networkDeviceInfo = m_networkDeviceInfos.get(address);
            if (result.networkDeviceInfo.address().isNull()) {
                NetworkDeviceInfo info;
                info.setAddress(address);
                result.networkDeviceInfo = info;
            }
            m_temporaryResults.append(result);
        }

        cleanupConnection(connection);
    });

    connect(connection->modbusTcpMaster(), &ModbusTcpMaster::connectionErrorOccurred, this, [this, connection](QModbusDevice::Error error) {
        if (error != QModbusDevice::NoError) {
            cleanupConnection(connection);
        }
    });

    connect(connection, &AbbTerraModbusTcpConnection::checkReachabilityFailed, this, [this, connection]() {
        cleanupConnection(connection);
    });

    connection->connectDevice();
}

void AbbTerraTcpDiscovery::cleanupConnection(AbbTerraModbusTcpConnection *connection)
{
    m_connections.removeAll(connection);
    connection->disconnectDevice();
    connection->deleteLater();
}

void AbbTerraTcpDiscovery::finishDiscovery()
{
    foreach (const Result &result, m_temporaryResults) {
        bool known = false;
        foreach (const Result &existing, m_results) {
            if (existing.serialNumber == result.serialNumber || existing.networkDeviceInfo.address() == result.networkDeviceInfo.address()) {
                known = true;
                break;
            }
        }

        if (!known) {
            qCDebug(dcAbbTerra()) << "Discovery: Found" << result.productName << result.networkDeviceInfo;
            m_results.append(result);
        }
    }

    const QList<AbbTerraModbusTcpConnection *> leftoverConnections = m_connections;
    foreach (AbbTerraModbusTcpConnection *connection, leftoverConnections) {
        cleanupConnection(connection);
    }

    const qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();
    qCInfo(dcAbbTerra()) << "Discovery: Finished ABB Terra AC network discovery in"
                         << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz")
                         << "with" << m_results.count() << "result(s).";
    emit discoveryFinished();
}
