/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
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

#include "solaredgebattery.h"
#include "extern-plugininfo.h"

SolarEdgeBattery::SolarEdgeBattery(SunSpec *connection, int modbusStartRegister, QObject *parent) :
    QObject(parent),
    m_connection(connection),
    m_modbusStartRegister(modbusStartRegister)
{

}

SunSpec *SolarEdgeBattery::connection() const
{
    return m_connection;
}

int SolarEdgeBattery::modbusStartRegister() const
{
    return m_modbusStartRegister;
}

void SolarEdgeBattery::init()
{
    m_initFinishedSuccess = false;
    readBlockData();

    QTimer::singleShot(10000, this, [this] {
        if (!m_initFinishedSuccess) {
            emit initFinished(false);
        }
    });
}

void SolarEdgeBattery::readBlockData()
{
    // Read the data in 2 block requests
    qCDebug(dcSolarEdge()) << "Battery: Read block 1 from modbus address" << m_modbusStartRegister << "length" << 107<< ", Slave ID" << m_connection->slaveId();
    m_blockBuffer.clear();

    // Total possible block size is 0xE19A - 0xE100 = 0x9A = 153 registers

    // First request 107 bytes
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister, 107);
    if (QModbusReply *reply = m_connection->modbusTcpClient()->sendReadRequest(request, m_connection->slaveId())) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [=]() {

                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcSolarEdge()) << "Battery: Read response error:" << reply->error();
                    return;
                }

                const QModbusDataUnit unit = reply->result();
                qCDebug(dcSolarEdge()) << "Battery: Received first block data" << unit.values().count();
                m_blockBuffer.append(unit.values());


                QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + 107, 46);
                if (QModbusReply *reply = m_connection->modbusTcpClient()->sendReadRequest(request, m_connection->slaveId())) {
                    if (!reply->isFinished()) {
                        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
                        connect(reply, &QModbusReply::finished, this, [=]() {
                            if (reply->error() != QModbusDevice::NoError) {
                                qCWarning(dcSolarEdge()) << "Battery: Read response error:" << reply->error();
                                return;
                            }

                            const QModbusDataUnit unit = reply->result();
                            qCDebug(dcSolarEdge()) << "Battery: Received first block data" << unit.values().count();
                            m_blockBuffer.append(unit.values());

                            processBlockBuffer();
                        });

                        connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error) {
                            qCWarning(dcSolarEdge()) << "Battery: Modbus reply error:" << error;
                            emit reply->finished(); // To make sure it will be deleted
                        });
                    } else {
                        qCWarning(dcSolarEdge()) << "Battery: Read error: " << m_connection->modbusTcpClient()->errorString();
                        delete reply; // broadcast replies return immediately
                        return;
                    }
                } else {
                    qCWarning(dcSolarEdge()) << "Battery: Read error: " << m_connection->modbusTcpClient()->errorString();
                    return;
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error) {
                qCWarning(dcSolarEdge()) << "Battery: Modbus reply error:" << error;
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            qCWarning(dcSolarEdge()) << "Battery: Read error: " << m_connection->modbusTcpClient()->errorString();
            delete reply; // broadcast replies return immediately
            return;
        }
    } else {
        qCWarning(dcSolarEdge()) << "Battery: Read error: " << m_connection->modbusTcpClient()->errorString();
        return;
    }

}

void SolarEdgeBattery::processBlockBuffer()
{
    qCDebug(dcSolarEdge()) << "Battery: reading data finished. Fetched" << m_blockBuffer.count() << "registers.";

    BatteryData data;
    data.manufacturerName = QString::fromUtf8(SunSpec::convertModbusRegisters(m_blockBuffer, ManufacturerName, 16)).trimmed();
    data.model = QString::fromUtf8(SunSpec::convertModbusRegisters(m_blockBuffer, Model, 16)).trimmed();
    data.firmwareVersion = QString::fromUtf8(SunSpec::convertModbusRegisters(m_blockBuffer, FirmwareVersion, 16)).trimmed();
    data.serialNumber = QString::fromUtf8(SunSpec::convertModbusRegisters(m_blockBuffer, SerialNumber, 16)).trimmed();

    qCDebug(dcSolarEdge()) << "Battery" << data.manufacturerName << data.model << data.firmwareVersion << data.serialNumber;

    if (!m_initFinishedSuccess) {
        m_initFinishedSuccess = true;
        emit initFinished(true);
    }

}
