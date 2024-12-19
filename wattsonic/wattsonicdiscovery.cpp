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

    QList<ModbusRtuMaster*> candidateMasters;
    foreach (ModbusRtuMaster *master, m_modbusRtuResource->modbusRtuMasters()) {
        if (master->baudrate() == 9600 && master->dataBits() == 8 && master->stopBits() == 1 && master->parity() == QSerialPort::NoParity) {
            candidateMasters.append(master);
        }
    }

    if (candidateMasters.isEmpty()) {
        qCWarning(dcWattsonic()) << "Discovery: No usable modbus RTU master found.";
        emit discoveryFinished(false);
        return;
    }

    foreach (ModbusRtuMaster *master, candidateMasters) {
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

    ModbusRtuReply *reply = master->readHoldingRegister(slaveId, 10000, 8);
    connect(reply, &ModbusRtuReply::finished, this, [this, master, reply, slaveId](){

        if (reply->error() == ModbusRtuReply::NoError) {
            QString serialNumber = ModbusDataUtils::convertToString(reply->result(), ModbusDataUtils::ByteOrderBigEndian);
            qCDebug(dcWattsonic()) << "Discovery: Test reply finished!" << reply->error() << reply->result() << serialNumber;

            Result result {master->modbusUuid(), serialNumber, slaveId};
            m_discoveryResults.append(result);
        }

        emit discoveryFinished(true);
    });
}
