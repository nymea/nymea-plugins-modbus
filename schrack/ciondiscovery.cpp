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

#include "ciondiscovery.h"
#include "extern-plugininfo.h"

#include <modbusdatautils.h>

#include <QRegularExpression>

CionDiscovery::CionDiscovery(ModbusRtuHardwareResource *modbusRtuResource, QObject *parent)
    : QObject{parent},
      m_modbusRtuResource{modbusRtuResource}
{

}

void CionDiscovery::startDiscovery()
{
    qCInfo(dcSchrack()) << "Discovery: Searching for Schrack i-CHARGE wallboxes on modbus RTU...";

    QList<ModbusRtuMaster*> candidateMasters;
    foreach (ModbusRtuMaster *master, m_modbusRtuResource->modbusRtuMasters()) {
        if (master->baudrate() == 57600 && master->dataBits() == 8 && master->stopBits() == 1 && master->parity() == QSerialPort::NoParity) {
            candidateMasters.append(master);
        }
    }

    if (candidateMasters.isEmpty()) {
        qCWarning(dcSchrack()) << "No usable modbus RTU master found.";
        emit discoveryFinished(false);
        return;
    }

    foreach (ModbusRtuMaster *master, candidateMasters) {
        if (master->connected()) {
            tryConnect(master, 1);
        } else {
            qCWarning(dcSchrack()) << "Modbus RTU master" << master->modbusUuid().toString() << "is not connected.";
        }
    }
}

QList<CionDiscovery::Result> CionDiscovery::discoveryResults() const
{
    return m_discoveryResults;
}

void CionDiscovery::tryConnect(ModbusRtuMaster *master, quint16 slaveId)
{
    qCDebug(dcSchrack()) << "Scanning modbus RTU master" << master->modbusUuid() << "Slave ID:" << slaveId;

    ModbusRtuReply *reply = master->readHoldingRegister(slaveId, 832, 16);
    connect(reply, &ModbusRtuReply::finished, this, [=](){

        if (reply->error() == ModbusRtuReply::NoError) {

            QString firmwareVersion = ModbusDataUtils::convertToString(reply->result());
            qCDebug(dcSchrack()) << "Test reply finished!" << reply->error() << firmwareVersion;

            // Version numbers seem to be wild west... We can't really understand what's in there...
            // So let's assume this is a schrack if reading alone succeeded and it is a valid string and 18 to 32 chars long...
            // Examples of how this looks like:
            // EBE 1.2: "V1.2    15.02.2021"
            // ICC:     "003090056-01          20220913"
            if (QRegularExpression("[A-Z0-9\\.- ]{18,32}").match(firmwareVersion).hasMatch()) {
                Result result {master->modbusUuid(), firmwareVersion, slaveId};
                m_discoveryResults.append(result);
            }
        }

        if (slaveId < 10) {
            tryConnect(master, slaveId+1);
        } else {
            emit discoveryFinished(true);
        }
    });
}
