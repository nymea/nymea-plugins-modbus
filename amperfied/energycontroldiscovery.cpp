/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2023, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea.
* This project including source code and documentation is protected by
* copyright law, and remains the property of nymea GmbH. All rights, including
* reproduction, publication, editing and translation, are reserved. The use of
* this project is subject to the terms of a license agreement to be concluded
* with nymea GmbH in accordance with the terms of use of nymea GmbH, available
* under https://nymea.io/license
*
* GNU Lesser General Public License Usage
* Alternatively, this project may be redistributed and/or modified under the
* terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; version 3. This project is distributed in the hope that
* it will be useful, but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this project. If not, see <https://www.gnu.org/licenses/>.
*
* For any further details and any questions please contact us under
* contact@nymea.io or see our FAQ/Licensing Information on
* https://nymea.io/license/faq
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "energycontroldiscovery.h"
#include "extern-plugininfo.h"

EnergyControlDiscovery::EnergyControlDiscovery(ModbusRtuHardwareResource *modbusRtuResource, QObject *parent) :
    QObject{parent},
    m_modbusRtuResource{modbusRtuResource}
{
}

void EnergyControlDiscovery::startDiscovery()
{
    qCInfo(dcAmperfied()) << "Discovery: Searching for Amperfied EnergyControl wallboxes on modbus RTU...";

    QList<ModbusRtuMaster*> candidateMasters;
    foreach (ModbusRtuMaster *master, m_modbusRtuResource->modbusRtuMasters()) {
        if (master->baudrate() == 19200 && master->dataBits() == 8 && master->stopBits() == 1 && master->parity() == QSerialPort::EvenParity) {
            candidateMasters.append(master);
        }
    }

    if (candidateMasters.isEmpty()) {
        qCWarning(dcAmperfied()) << "No usable modbus RTU master found.";
        emit discoveryFinished(false);
        return;
    }

    foreach (ModbusRtuMaster *master, candidateMasters) {
        if (master->connected()) {
            tryConnect(master, 1);
        } else {
            qCWarning(dcAmperfied()) << "Modbus RTU master" << master->modbusUuid().toString() << "is not connected.";
        }
    }
}

QList<EnergyControlDiscovery::Result> EnergyControlDiscovery::discoveryResults() const
{
    return m_discoveryResults;
}

void EnergyControlDiscovery::tryConnect(ModbusRtuMaster *master, quint16 slaveId)
{
    qCDebug(dcAmperfied()) << "Scanning modbus RTU master" << master->modbusUuid() << "Slave ID:" << slaveId;

    ModbusRtuReply *reply = master->readInputRegister(slaveId, 4);
    connect(reply, &ModbusRtuReply::finished, this, [=](){
        qCDebug(dcAmperfied()) << "Test reply finished!" << reply->error() << reply->result();
        if (reply->error() == ModbusRtuReply::NoError && reply->result().length() > 0) {
            quint16 version = reply->result().first();
            if (version >= 0x0100) {
                qCDebug(dcAmperfied()) << QString("Version is 0x%1").arg(version, 0, 16);
                Result result {master->modbusUuid(), version, slaveId};
                m_discoveryResults.append(result);
            } else {
                qCDebug(dcAmperfied()) << "Version must be at least 1.0.0 (0x0100)";
            }
        }
        if (slaveId < 20) {
            tryConnect(master, slaveId+1);
        } else {
            emit discoveryFinished(true);
        }
    });
}

