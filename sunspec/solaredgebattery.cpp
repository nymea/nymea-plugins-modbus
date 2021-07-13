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
    m_timer.setSingleShot(true);
    m_timer.setInterval(10000);
    connect(&m_timer, &QTimer::timeout, this, [this](){
        if (!m_initFinishedSuccess) {
            emit initFinished(false);
        }
    });
}

SunSpec *SolarEdgeBattery::connection() const
{
    return m_connection;
}

int SolarEdgeBattery::modbusStartRegister() const
{
    return m_modbusStartRegister;
}

SolarEdgeBattery::BatteryData SolarEdgeBattery::batteryData() const
{
    return m_batteryData;
}

void SolarEdgeBattery::init()
{
    qCDebug(dcSunSpec()) << "Initializing battery on" << m_modbusStartRegister;
    m_initFinishedSuccess = false;
    readBlockData();
    m_timer.start();
}

void SolarEdgeBattery::readBlockData()
{
    // Read the data in 2 block requests
    qCDebug(dcSunSpec()) << "Battery: Read block 1 from modbus address" << m_modbusStartRegister << "length" << 107<< ", Slave ID" << m_connection->slaveId();

    // Total possible block size is 0xE19A - 0xE100 = 0x9A = 153 registers

    // First block request 0x00 - 0x4C
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister, 0x4C);
    if (QModbusReply *reply = m_connection->modbusTcpClient()->sendReadRequest(request, m_connection->slaveId())) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [=]() {

                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcSunSpec()) << "Battery: Read response error:" << reply->error();
                    return;
                }

                const QModbusDataUnit unit = reply->result();
                qCDebug(dcSunSpec()) << "Battery: Received first block data" << m_modbusStartRegister << unit.values().count();
                QVector<quint16> values = unit.values();

                m_batteryData.manufacturerName = QString::fromUtf8(SunSpec::convertModbusRegisters(values, ManufacturerName, 16)).trimmed();
                m_batteryData.model = QString::fromUtf8(SunSpec::convertModbusRegisters(values, Model, 16)).trimmed();
                m_batteryData.firmwareVersion = QString::fromUtf8(SunSpec::convertModbusRegisters(values, FirmwareVersion, 16)).trimmed();
                m_batteryData.serialNumber = QString::fromUtf8(SunSpec::convertModbusRegisters(values, SerialNumber, 16)).trimmed();
                m_batteryData.batteryDeviceId = values[BatteryDeviceId];

                // 8192 17945 536888857 536888857 1.08652e-19
                // 0x2000 0x4619

                qCDebug(dcSunSpec()) << "Battery: " << m_batteryData.batteryDeviceId << m_batteryData.manufacturerName << m_batteryData.model << m_batteryData.firmwareVersion << m_batteryData.serialNumber;
                m_batteryData.ratedEnergy = SunSpec::convertToFloat32(values[RatedEnergy], values[RatedEnergy + 1]);
                m_batteryData.maxChargeContinuesPower = SunSpec::convertToFloat32(values[MaxChargeContinuesPower], values[MaxChargeContinuesPower + 1]);
                m_batteryData.maxDischargeContinuesPower = SunSpec::convertToFloat32(values[MaxDischargeContinuesPower], values[MaxDischargeContinuesPower + 1]);
                m_batteryData.maxChargePeakPower = SunSpec::convertToFloat32(values[MaxChargePeakPower], values[MaxChargePeakPower + 1]);
                m_batteryData.maxDischargePeakPower = SunSpec::convertToFloat32(values[MaxDischargePeakPower], values[MaxDischargePeakPower + 1]);


                // Read from 0x6c to 0x86
                int offset = 0x6c;
                QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + offset, 28);
                if (QModbusReply *reply = m_connection->modbusTcpClient()->sendReadRequest(request, m_connection->slaveId())) {
                    if (!reply->isFinished()) {
                        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
                        connect(reply, &QModbusReply::finished, this, [=]() {
                            if (reply->error() != QModbusDevice::NoError) {
                                qCWarning(dcSunSpec()) << "Battery: Read response error:" << reply->error();
                                return;
                            }

                            const QModbusDataUnit unit = reply->result();
                            QVector<quint16> values = unit.values();

                            qCDebug(dcSunSpec()) << "Battery: Received second block data" << offset << values.count();
                            m_batteryData.averageTemperature = SunSpec::convertToFloat32(values[BatteryAverageTemperature - offset], values[BatteryAverageTemperature - offset + 1]);
                            m_batteryData.maxTemperature = SunSpec::convertToFloat32(values[BatteryMaxTemperature - offset], values[BatteryMaxTemperature - offset + 1]);
                            m_batteryData.instantaneousVoltage = SunSpec::convertToFloat32(values[InstantaneousVoltage - offset], values[InstantaneousVoltage - offset + 1]);
                            m_batteryData.instantaneousCurrent = SunSpec::convertToFloat32(values[InstantaneousCurrent - offset], values[InstantaneousCurrent - offset + 1]);
                            m_batteryData.instantaneousPower = SunSpec::convertToFloat32(values[InstantaneousPower - offset], values[InstantaneousPower - offset + 1]);
                            m_batteryData.maxEnergy = SunSpec::convertToFloat32(values[MaxEnergy - offset], values[MaxEnergy - offset + 1]);
                            m_batteryData.availableEnergy = SunSpec::convertToFloat32(values[AvailableEnergy - offset], values[AvailableEnergy - offset + 1]);
                            m_batteryData.stateOfHealth = SunSpec::convertToFloat32(values[StateOfHealth - offset], values[StateOfHealth - offset + 1]);
                            m_batteryData.stateOfEnergy = SunSpec::convertToFloat32(values[StateOfEnergy - offset], values[StateOfEnergy - offset + 1]);
                            m_batteryData.batteryStatus = static_cast<BatteryStatus>(static_cast<quint32>(values[Status - offset + 1]) << 16 | values[Status - offset]);

                            qCDebug(dcSunSpec()) << m_batteryData;
                            emit batteryDataReceived(m_batteryData);

                            if (!m_initFinishedSuccess) {
                                m_timer.stop();
                                m_initFinishedSuccess = true;
                                emit initFinished(true);
                            }
                        });

                        connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error) {
                            qCWarning(dcSunSpec()) << "Battery: Modbus reply error:" << error;
                            emit reply->finished(); // To make sure it will be deleted
                        });
                    } else {
                        qCWarning(dcSunSpec()) << "Battery: Read error: " << m_connection->modbusTcpClient()->errorString();
                        delete reply; // broadcast replies return immediately
                        return;
                    }
                } else {
                    qCWarning(dcSunSpec()) << "Battery: Read error: " << m_connection->modbusTcpClient()->errorString();
                    return;
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error) {
                qCWarning(dcSunSpec()) << "Battery: Modbus reply error:" << error;
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            qCWarning(dcSunSpec()) << "Battery: Read error: " << m_connection->modbusTcpClient()->errorString();
            delete reply; // broadcast replies return immediately
            return;
        }
    } else {
        qCWarning(dcSunSpec()) << "Battery: Read error: " << m_connection->modbusTcpClient()->errorString();
        return;
    }
}


QDebug operator<<(QDebug debug, const SolarEdgeBattery::BatteryData &batteryData)
{
    debug << "BatteryData(" << batteryData.manufacturerName << "-" << batteryData.model << ")" << endl;
    debug << "    - Battery Device ID" << batteryData.batteryDeviceId << endl;
    debug << "    - Firmware version" << batteryData.firmwareVersion << endl;
    debug << "    - Serial number" << batteryData.serialNumber << endl;
    debug << "    - Rated Energy" << batteryData.ratedEnergy << "W * H" << endl;
    debug << "    - Max charging continues power" << batteryData.maxChargeContinuesPower << "W" << endl;
    debug << "    - Max discharging continues power" << batteryData.maxDischargeContinuesPower << "W" << endl;
    debug << "    - Max charging peak power" << batteryData.maxChargePeakPower << "W" << endl;
    debug << "    - Max discharging peak power" << batteryData.maxDischargePeakPower << "W" << endl;
    debug << "    - Average temperature" << batteryData.averageTemperature << "°C" << endl;
    debug << "    - Max temperature" << batteryData.maxTemperature << "°C" << endl;
    debug << "    - Instantuouse Voltage" << batteryData.instantaneousVoltage << "V" << endl;
    debug << "    - Instantuouse Current" << batteryData.instantaneousCurrent << "A" << endl;
    debug << "    - Instantuouse Power" << batteryData.instantaneousPower << "W" << endl;
    debug << "    - Max energy" << batteryData.maxEnergy << "W" << endl;
    debug << "    - Available energy" << batteryData.availableEnergy << "W" << endl;
    debug << "    - State of health" << batteryData.stateOfHealth << "%" << endl;
    debug << "    - State of energy" << batteryData.stateOfEnergy << "%" << endl;
    debug << "    - Battery status" << batteryData.batteryStatus << endl;
    return debug;
}
