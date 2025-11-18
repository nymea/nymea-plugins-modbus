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

#include "amtroncompact20discovery.h"
#include "extern-plugininfo.h"

#include <modbusdatautils.h>

#include <QRegularExpression>

QList<int> slaveIdCandidates = {50, 11, 12, 13, 14};

AmtronCompact20Discovery::AmtronCompact20Discovery(ModbusRtuHardwareResource *modbusRtuResource, QObject *parent):
    QObject{parent},
    m_modbusRtuResource(modbusRtuResource)
{

}

void AmtronCompact20Discovery::startDiscovery()
{
    qCInfo(dcMennekes()()) << "Discovery: Searching for Amtron Compact 2.0 wallboxes on modbus RTU...";

    QList<ModbusRtuMaster*> candidateMasters;
    foreach (ModbusRtuMaster *master, m_modbusRtuResource->modbusRtuMasters()) {
        if (master->baudrate() == 57600 && master->dataBits() == 8 && master->stopBits() == 2 && master->parity() == QSerialPort::NoParity) {
            candidateMasters.append(master);
        }
    }

    if (candidateMasters.isEmpty()) {
        qCWarning(dcMennekes()) << "No usable modbus RTU master found.";
        emit discoveryFinished(false);
        return;
    }

    foreach (ModbusRtuMaster *master, candidateMasters) {
        if (master->connected()) {
            tryConnect(master, 0);
        } else {
            qCWarning(dcMennekes()) << "Modbus RTU master" << master->modbusUuid().toString() << "is not connected.";
        }
    }
}

QList<AmtronCompact20Discovery::Result> AmtronCompact20Discovery::discoveryResults() const
{
    return m_discoveryResults;
}

void AmtronCompact20Discovery::tryConnect(ModbusRtuMaster *master, quint16 slaveIdIndex)
{
    quint8 slaveId = slaveIdCandidates.at(slaveIdIndex);
    qCDebug(dcMennekes()) << "Scanning modbus RTU master" << master->modbusUuid() << "Slave ID:" << slaveId;

    ModbusRtuReply *reply = master->readInputRegister(slaveId, 0x13, 8);
    connect(reply, &ModbusRtuReply::finished, this, [=](){

        if (reply->error() == ModbusRtuReply::NoError) {

            QString serialNumber = ModbusDataUtils::convertToString(reply->result(), ModbusDataUtils::ByteOrderBigEndian).remove(QRegularExpression("^_*"));
            qCDebug(dcMennekes()) << "Test reply finished!" << reply->error() << serialNumber;

            Result result {master->modbusUuid(), serialNumber, slaveId};
            m_discoveryResults.append(result);

        }

        if (slaveIdIndex < slaveIdCandidates.length() - 1) {
            tryConnect(master, slaveIdIndex+1);
        } else {
            emit discoveryFinished(true);
        }
    });
}
