// SPDX-License-Identifier: GPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
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

#include "wattsonicdiscovery.h"
#include "extern-plugininfo.h"

#include <modbusdatautils.h>

WattsonicDiscovery::WattsonicDiscovery(ModbusRtuHardwareResource *modbusRtuResource, QObject *parent):
    QObject{parent},
    m_modbusRtuResource(modbusRtuResource)
{

}

void WattsonicDiscovery::startDiscovery(quint16 slaveId)
{
    qCInfo(dcWattsonic()) << "Discovery: Searching for Wattsonic device on modbus RTU...";
    m_candidateMasters.clear();
    m_verifiedMasters.clear();

    foreach (ModbusRtuMaster *master, m_modbusRtuResource->modbusRtuMasters()) {
        if (master->baudrate() == 9600 && master->dataBits() == 8 && master->stopBits() == 1 && master->parity() == QSerialPort::NoParity) {
            m_candidateMasters.append(master);
        }
    }

    if (m_candidateMasters.isEmpty()) {
        qCWarning(dcWattsonic()) << "Discovery: No usable modbus RTU master found.";
        emit discoveryFinished(false);
        return;
    }

    foreach (ModbusRtuMaster *master, m_candidateMasters) {
        if (master->connected()) {
            tryConnect(master, slaveId);
        } else {
            qCWarning(dcWattsonic()) << "Discovery: Modbus RTU master" << master->modbusUuid().toString() << "is not connected.";
        }
    }
}

QList<WattsonicDiscovery::Result> WattsonicDiscovery::discoveryResults() const
{
    return m_discoveryResults;
}

void WattsonicDiscovery::tryConnect(ModbusRtuMaster *master, quint16 slaveId)
{
    qCDebug(dcWattsonic()) << "Discovery: Scanning modbus RTU master" << master->modbusUuid() << "Slave ID:" << slaveId;
    m_verifiedMasters.append(master);

    WattsonicInverter *connection = new WattsonicInverter(master, slaveId, WattsonicInverter::InverterGenerationModeAuto, this);
    connect(connection, &WattsonicInverter::reachableChanged, this, [connection](bool reachable){
        if (reachable) {
            qCDebug(dcWattsonic()) << "Discovery: The connection is now reachable. Starting the initialization";
            connection->initialize();
        } else {
            connection->deleteLater();
        }
    });


    connect(connection, &WattsonicInverter::customInitializationFinished, this, [this, connection, master, slaveId](bool success){

        if (!success) {
            qCDebug(dcWattsonic()) << "Initialization failed on" << master << "skip ";
            return;
        }

        qCDebug(dcWattsonic()) << "Discovery: Init finished successfully:" << connection->serialNumber()
                               << connection->inverterInfo().type << connection->inverterInfo().model << connection->generation();

        Result result {master->modbusUuid(), connection->serialNumber(), slaveId, connection->generation(), connection->inverterInfo()};
        m_discoveryResults.append(result);

        if (m_verifiedMasters.length() == m_candidateMasters.length()) {
            qCDebug(dcWattsonic()) << "Discovery: Verified all masters. Finish discovery with" << m_discoveryResults.length() << "discovered inverters.";
            emit discoveryFinished(true);
        }
    });
}
