/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2025, nymea GmbH
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
