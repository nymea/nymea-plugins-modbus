// SPDX-License-Identifier: GPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2026, chargebyte austria GmbH
*
* This file is part of nymea-plugins-modbus.
*
* nymea-plugins-modbus is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* nymea-plugins-modbus is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with nymea-plugins-modbus. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "pcelectricdiscovery.h"
#include "extern-plugininfo.h"

PcElectricDiscovery::PcElectricDiscovery(NetworkDeviceDiscovery *networkDeviceDiscovery, quint16 port, quint16 modbusAddress, QObject *parent)
    : QObject{parent}
    , m_networkDeviceDiscovery{networkDeviceDiscovery}
    , m_port{port}
    , m_modbusAddress{modbusAddress}
{}

QList<PcElectricDiscovery::Result> PcElectricDiscovery::results() const
{
    return m_results;
}

void PcElectricDiscovery::startDiscovery()
{
    qCInfo(dcPcElectric()) << "Discovery: Start searching for PCE wallboxes in the network...";
    m_startDateTime = QDateTime::currentDateTime();

    NetworkDeviceDiscoveryReply *discoveryReply = m_networkDeviceDiscovery->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::hostAddressDiscovered, this, &PcElectricDiscovery::checkNetworkDevice);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=]() {
        m_networkDeviceInfos = discoveryReply->networkDeviceInfos();

        // Finish with some delay so the last added network device information objects still can be checked.
        QTimer::singleShot(3000, this, [this]() {
            qCDebug(dcPcElectric()) << "Discovery: Grace period timer triggered.";
            finishDiscovery();
        });
    });
}

void PcElectricDiscovery::checkNetworkDevice(const QHostAddress &address)
{
    EV11ModbusTcpConnection *connection = new EV11ModbusTcpConnection(address, m_port, m_modbusAddress, this);
    m_connections.append(connection);

    connect(connection, &EV11ModbusTcpConnection::reachableChanged, this, [=](bool reachable) {
        if (!reachable) {
            // Disconnected ... done with this connection
            cleanupConnection(connection);
            return;
        }

        // Modbus TCP connected...ok, let's try to initialize it!
        connect(connection, &EV11ModbusTcpConnection::initializationFinished, this, [=](bool success) {
            if (!success) {
                qCDebug(dcPcElectric()) << "Discovery: Initialization failed on" << address.toString() << "Continue...";
                cleanupConnection(connection);
                return;
            }

            // Parse the mac address from the registers and compair with the network device info mac address.
            // If they match, we most likly found a PCE wallbox

            QByteArray macRawData;
            QDataStream stream(&macRawData, QIODevice::WriteOnly);
            for (int i = 0; i < connection->macAddress().length(); i++)
                stream << connection->macAddress().at(i);

            MacAddress registerMacAddress(macRawData);
            qCDebug(dcPcElectric()) << "Fetched mac address" << macRawData.toHex() << registerMacAddress;

            // According to PCE the HW revision must be 0
            if (!registerMacAddress.isNull() && connection->hardwareRevision() == 0) {
                // Parse the serial number
                QByteArray serialRawData;
                QDataStream stream(&serialRawData, QIODevice::ReadWrite);
                stream << static_cast<quint16>(0);
                for (int i = 0; i < connection->serialNumber().length(); i++)
                    stream << connection->serialNumber().at(i);

                quint64 serialNumber = serialRawData.toHex().toULongLong(nullptr, 16);
                qCDebug(dcPcElectric()) << "Discovery: Serial number" << serialRawData.toHex() << serialNumber;

                Result result;
                result.serialNumber = QString::number(serialNumber);
                result.firmwareRevision = connection->firmwareRevision();
                result.address = address;
                result.registerMacAddress = registerMacAddress;

                m_runningVerifications.insert(connection, result);

                // This seems to be a PCE wallbox, let's try to figure out if there is meter or not.

                // Depending on the firmware revision read the MODE_DIGITAL_IN and the R37_MODE register,
                // decide based on the result which thing class we have discovered.

                // [1] 0019, DI = 0|1 -> no meter
                // [2] 0019, DI = 2   -> meter

                // [3] 0025, DI = 0|1,  R37 = 0 -> no meter
                // [4] 0025, DI = 2|3,  R37 = 0 -> meter
                // [5] 0025, DI = 1?,   R37 = 2 -> no meter
                // [6] 0025, DI = 0|1?, R37 = 1 -> meter

                connect(connection, &EV11ModbusTcpConnection::digitalInputModeReadFinished, this, [this, connection](EV11ModbusTcpConnection::DigitalInputMode digitalInputMode) {
                    qCDebug(dcPcElectric()) << "Discovery: Digital input mode read finished for" << connection->modbusTcpMaster()->hostAddress().toString() << digitalInputMode;
                    m_runningVerifications[connection].digitalInputMode = digitalInputMode;

                    if (connection->firmwareRevision() < "0025") {
                        if (digitalInputMode == EV11ModbusTcpConnection::DigitalInputModeEnableCharging
                            || digitalInputMode == EV11ModbusTcpConnection::DigitalInputModeEnableChargingInverted) {
                            // [1] 0019, DI = 0|1 -> no meter
                            qCDebug(dcPcElectric())
                                << "Discovery: 0019, DI = 0|1 -> The connection does not seem to have a meter"
                                << connection->modbusTcpMaster()->hostAddress().toString();
                            m_runningVerifications[connection].thingClassId = ev11NoMeterThingClassId;
                        } else {
                            // [2] 0019, DI = 2   -> meter
                            qCDebug(dcPcElectric()) << "Discovery: 0019, DI = >1 -> The connection seems to have a meter" << connection->modbusTcpMaster()->hostAddress().toString();
                            m_runningVerifications[connection].thingClassId = ev11ThingClassId;
                        }

                        // Done with this connection
                        m_potentialResults.append(m_runningVerifications.take(connection));
                        cleanupConnection(connection);

                    } else {
                        // Firmware version 0.25, read also the R37 register for meter detection
                        connect(connection,
                                &EV11ModbusTcpConnection::modeR37ReadFinished,
                                this,
                                [this, connection](EV11ModbusTcpConnection::R37Mode r37Mode) {
                                    // We have now the R37 mode and the digital in mode, all informtion fetched
                                    qCDebug(dcPcElectric()) << "Discovery: R37 mode read finished for" << connection->modbusTcpMaster()->hostAddress().toString() << r37Mode;
                                    m_runningVerifications[connection].r37Mode = r37Mode;

                                    const EV11ModbusTcpConnection::DigitalInputMode digitalInputMode = m_runningVerifications.value(connection).digitalInputMode;
                                    if ((digitalInputMode == EV11ModbusTcpConnection::DigitalInputModeEnableCharging
                                         || digitalInputMode == EV11ModbusTcpConnection::DigitalInputModeEnableChargingInverted)
                                        && r37Mode == EV11ModbusTcpConnection::R37ModeNoMonitoring) {
                                        // [3] 0025, DI = 0|1, R37 = 0 -> no meter
                                        qCDebug(dcPcElectric())
                                            << "Discovery: 0025, DI = 0|1, R37 = 0 -> The connection does not seem to have a meter"
                                            << connection->modbusTcpMaster()->hostAddress().toString();
                                        m_runningVerifications[connection].thingClassId = ev11NoMeterThingClassId;
                                    } else if ((digitalInputMode == EV11ModbusTcpConnection::DigitalInputModePwmS0Enabled
                                                || digitalInputMode == EV11ModbusTcpConnection::DigitalInputModeLimitS0Enabled)
                                               && r37Mode == EV11ModbusTcpConnection::R37ModeNoMonitoring) {
                                        // [4] 0025, DI = 2|3, R37 = 0 -> meter
                                        qCDebug(dcPcElectric())
                                            << "Discovery: 0025, DI = 2|3, R37 = 0 -> The connection seems to have a meter"
                                            << connection->modbusTcpMaster()->hostAddress().toString();
                                        m_runningVerifications[connection].thingClassId = ev11ThingClassId;
                                    } else if (r37Mode == EV11ModbusTcpConnection::R37ModeDigitalInputMonitoring) {
                                        // [5] 0025, DI = 0|1?, R37 = 2 -> meter
                                        qCDebug(dcPcElectric())
                                            << "Discovery: 0025, DI = 0|1?, R37 = 1 -> The connection does not seem to have a meter"
                                            << connection->modbusTcpMaster()->hostAddress().toString();
                                        m_runningVerifications[connection].thingClassId = ev11NoMeterThingClassId;
                                    } else if (r37Mode == EV11ModbusTcpConnection::R37ModeMeterMonitoring) {
                                        // [6] 0025, DI = 0|1?, R37 = 1 -> meter
                                        qCDebug(dcPcElectric())
                                            << "Discovery: 0025, DI = 0|1?, R37 = 1 -> The connection seems to have a meter"
                                            << connection->modbusTcpMaster()->hostAddress().toString();
                                        m_runningVerifications[connection].thingClassId = ev11ThingClassId;
                                    } else {
                                        // No defined case, default to ev11 with meter
                                        qCWarning(dcPcElectric())
                                            << "Discovery: Cannot tell if this connection has a meter. Assuming there is one:"
                                            << m_runningVerifications.value(connection).r37Mode
                                            << m_runningVerifications.value(connection).digitalInputMode
                                            << connection->modbusTcpMaster()->hostAddress().toString();

                                        m_runningVerifications[connection].thingClassId = ev11ThingClassId;
                                    }

                                    // Done with this connection
                                    m_potentialResults.append(m_runningVerifications.take(connection));
                                    cleanupConnection(connection);
                                });

                        // Get R37
                        connection->updateModeR37();
                    }
                });

                // Get DI
                connection->updateDigitalInputMode();
            }
        });

        // Initializing...
        if (!connection->initialize()) {
            qCDebug(dcPcElectric()) << "Discovery: Unable to initialize connection on" << address.toString() << "Continue...";
            cleanupConnection(connection);
        }
    });

    // If we get any error...skip this host...
    connect(connection->modbusTcpMaster(), &ModbusTcpMaster::connectionErrorOccurred, this, [=](QModbusDevice::Error error) {
        if (error != QModbusDevice::NoError) {
            qCDebug(dcPcElectric()) << "Discovery: Connection error on" << address.toString() << "Continue...";
            cleanupConnection(connection);
        }
    });

    // If check reachability failed...skip this host...
    connect(connection, &EV11ModbusTcpConnection::checkReachabilityFailed, this, [=]() {
        qCDebug(dcPcElectric()) << "Discovery: Check reachability failed on" << address.toString() << "Continue...";
        cleanupConnection(connection);
    });

    // Try to connect, maybe it works, maybe not...
    connection->connectDevice();
}

void PcElectricDiscovery::cleanupConnection(EV11ModbusTcpConnection *connection)
{
    m_connections.removeAll(connection);
    m_runningVerifications.remove(connection);

    connection->disconnectDevice();
    connection->deleteLater();
}

void PcElectricDiscovery::finishDiscovery()
{
    qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    for (int i = 0; i < m_potentialResults.length(); i++) {
        const NetworkDeviceInfo networkDeviceInfo = m_networkDeviceInfos.get(m_potentialResults.at(i).address);
        m_potentialResults[i].networkDeviceInfo = networkDeviceInfo;

        Result result = m_potentialResults.at(i);
        if (networkDeviceInfo.macAddressInfos().hasMacAddress(result.registerMacAddress)) {
            qCInfo(dcPcElectric())
                << "Discovery: --> Found EV11.3"
                << (result.thingClassId == ev11NoMeterThingClassId ? "(No meter)" : "with meter")
                << "Serial number:"
                << result.serialNumber
                << "Firmware revision:"
                << result.firmwareRevision
                << result.networkDeviceInfo
                << result.digitalInputMode
                << result.r37Mode;
            m_results.append(result);
        } else {
            qCWarning(dcPcElectric())
                << "Discovery: --> Found potential EV11.3, but not adding to the results due to imcomplete MAC address check:"
                << "Serial number:"
                << result.serialNumber
                << "Firmware revision:"
                << result.firmwareRevision
                << result.networkDeviceInfo;
        }
    }

    m_potentialResults.clear();
    m_networkDeviceInfos.clear();

    // Cleanup any leftovers...we don't care any more
    foreach (EV11ModbusTcpConnection *connection, m_connections)
        cleanupConnection(connection);

    qCInfo(dcPcElectric())
        << "Discovery: Finished the discovery process. Found"
        << m_results.length()
        << "PCE wallboxes in"
        << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");
    emit discoveryFinished();
}
