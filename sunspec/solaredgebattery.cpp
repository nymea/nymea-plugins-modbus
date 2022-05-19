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

#include <sunspecdatapoint.h>
#include <sunspecconnection.h>

SolarEdgeBattery::SolarEdgeBattery(Thing *thing, SunSpecConnection *connection, int modbusStartRegister, QObject *parent) :
    SunSpecThing(thing, nullptr, parent),
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

SunSpecConnection *SolarEdgeBattery::connection() const
{
    return m_connection;
}

quint16 SolarEdgeBattery::modbusStartRegister() const
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
    qCDebug(dcSunSpec()) << "SolarEdgeBattery: Read block 1 from modbus address" << m_modbusStartRegister << "length" << 107<< ", Slave ID" << m_connection->slaveId();

    // Total possible block size is 0xE19A - 0xE100 = 0x9A = 153 registers

    // First block request 0x00 - 0x4C
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister, 0x4C);
    if (QModbusReply *reply = m_connection->modbusTcpClient()->sendReadRequest(request, m_connection->slaveId())) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [=]() {
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcSunSpec()) << "SolarEdgeBattery: Read response error:" << reply->error();
                    if (!m_initFinishedSuccess) {
                        m_timer.stop();
                        emit initFinished(false);
                    }
                    return;
                }

                // Example data:
                //  "(0x3438, 0x565f, 0x4c47, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4c47, 0x4320, 0x5245, 0x5355, 0x2031, 0x3000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00ff, 0x0000, 0xffff, 0xff7f, 0xffff, 0xff7f, 0xffff, 0xff7f, 0xffff, 0xff7f, 0xffff, 0xff7f)"
                //  255 "48V_LG" "LGC RESU 10" "" ""
                //  "(0x3438, 0x565f, 0x4c47, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4c47, 0x4320, 0x5245, 0x5355, 0x2031, 0x3000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x3438, 0x5620, 0x4443, 0x4443, 0x2032, 0x2e32, 0x2e39, 0x3120, 0x424d, 0x5320, 0x302e, 0x302e, 0x3000, 0x0000, 0x0000, 0x0000, 0x3745, 0x3034, 0x3432, 0x4543, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0070, 0x0000, 0x2000, 0x4619, 0x4000, 0x459c, 0x4000, 0x459c, 0x4000, 0x44ce, 0x4000, 0x459c)"
                //  112 "48V_LG" "LGC RESU 10" "48V DCDC 2.2.91 BMS 0.0.0" "7E0442EC"


                const QModbusDataUnit unit = reply->result();
                QVector<quint16> values = unit.values();
                qCDebug(dcSunSpec()) << "SolarEdgeBattery: Received first block data" << m_modbusStartRegister << values.count();
                qCDebug(dcSunSpec()) << "SolarEdgeBattery:" << SunSpecDataPoint::registersToString(values);

                m_batteryData.manufacturerName = SunSpecDataPoint::convertToString(values.mid(ManufacturerName, 16));
                m_batteryData.model = SunSpecDataPoint::convertToString(values.mid(Model, 16));
                m_batteryData.firmwareVersion = SunSpecDataPoint::convertToString(values.mid(FirmwareVersion, 16));
                m_batteryData.serialNumber = SunSpecDataPoint::convertToString(values.mid(SerialNumber, 16));
                m_batteryData.batteryDeviceId = values[BatteryDeviceId];
                qCDebug(dcSunSpec()) << "SolarEdgeBattery:" << m_batteryData.batteryDeviceId << m_batteryData.manufacturerName << m_batteryData.model << m_batteryData.firmwareVersion << m_batteryData.serialNumber;

                // Check if there is a battery connected, if so, one of the string must contain vaild data...
                if (m_batteryData.manufacturerName.isEmpty() && m_batteryData.model.isEmpty() && m_batteryData.serialNumber.isEmpty() && m_batteryData.firmwareVersion.isEmpty()) {
                    qCWarning(dcSunSpec()) << "SolarEdgeBattery: No valid information detected about the battery. Probably no battery connected at register" << m_modbusStartRegister;
                    if (!m_initFinishedSuccess) {
                        m_timer.stop();
                        emit initFinished(false);
                    }
                    return;
                }

                // For some reason, there might be even data in there but no battery connected, let's check if there are invalid registers

                // Check if there is a battery connected, if so, one of the string must contain vaild data...
                const QVector<quint16> invalidRegisters = { 0xffff, 0xff7f };
                if (values.mid(RatedEnergy, 2) == invalidRegisters && values.mid(MaxChargeContinuesPower, 2) == invalidRegisters &&
                        values.mid(MaxDischargeContinuesPower, 2) == invalidRegisters && values.mid(MaxChargePeakPower, 2) == invalidRegisters &&
                        values.mid(MaxDischargePeakPower, 2) == invalidRegisters) {
                    qCWarning(dcSunSpec()) << "SolarEdgeBattery: No valid information detected about the battery. Probably no battery connected at register" << m_modbusStartRegister;
                    if (!m_initFinishedSuccess) {
                        m_timer.stop();
                        emit initFinished(false);
                    }
                    return;
                }

                m_batteryData.ratedEnergy = SunSpecDataPoint::convertToFloat32(values.mid(RatedEnergy, 2));
                m_batteryData.maxChargeContinuesPower = SunSpecDataPoint::convertToFloat32(values.mid(MaxChargeContinuesPower, 2));
                m_batteryData.maxDischargeContinuesPower = SunSpecDataPoint::convertToFloat32(values.mid(MaxDischargeContinuesPower, 2));
                m_batteryData.maxChargePeakPower = SunSpecDataPoint::convertToFloat32(values.mid(MaxChargePeakPower, 2));
                m_batteryData.maxDischargePeakPower = SunSpecDataPoint::convertToFloat32(values.mid(MaxDischargePeakPower, 2));

                // First block looks good, continue with second block

                // 8192 17945 536888857 536888857 1.08652e-19
                // 0x2000 0x4619

                // Read from 0x6c to 0x86
                int offset = 0x6c;
                QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + offset, 28);
                if (QModbusReply *reply = m_connection->modbusTcpClient()->sendReadRequest(request, m_connection->slaveId())) {
                    if (!reply->isFinished()) {
                        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
                        connect(reply, &QModbusReply::finished, this, [=]() {
                            if (reply->error() != QModbusDevice::NoError) {
                                qCWarning(dcSunSpec()) << "SolarEdgeBattery: Read response error:" << reply->error();
                                if (!m_initFinishedSuccess) {
                                    m_timer.stop();
                                    emit initFinished(false);
                                }
                                return;
                            }

                            const QModbusDataUnit unit = reply->result();
                            QVector<quint16> values = unit.values();

                            qCDebug(dcSunSpec()) << "SolarEdgeBattery: Received second block data" << m_modbusStartRegister + offset << values.count();
                            qCDebug(dcSunSpec()) << "SolarEdgeBattery:" << SunSpecDataPoint::registersToString(values);
                            QVector<quint16> valueRegisters;

                            valueRegisters = values.mid(BatteryAverageTemperature - offset, 2);
                            m_batteryData.averageTemperature = SunSpecDataPoint::convertToFloat32(valueRegisters);
                            qCDebug(dcSunSpec()) << "SolarEdgeBattery: Average temperature:" << SunSpecDataPoint::registersToString(valueRegisters) << m_batteryData.averageTemperature;

                            m_batteryData.maxTemperature = SunSpecDataPoint::convertToFloat32(values.mid(BatteryMaxTemperature - offset, 2));
                            m_batteryData.instantaneousVoltage = SunSpecDataPoint::convertToFloat32(values.mid(InstantaneousVoltage - offset, 2));
                            m_batteryData.instantaneousCurrent = SunSpecDataPoint::convertToFloat32(values.mid(InstantaneousCurrent - offset, 2));
                            m_batteryData.instantaneousPower = SunSpecDataPoint::convertToFloat32(values.mid(InstantaneousPower - offset, 2));
                            m_batteryData.maxEnergy = SunSpecDataPoint::convertToFloat32(values.mid(MaxEnergy - offset, 2));

                            valueRegisters = values.mid(AvailableEnergy - offset, 2);
                            m_batteryData.availableEnergy = SunSpecDataPoint::convertToFloat32(valueRegisters);
                            qCDebug(dcSunSpec()) << "SolarEdgeBattery: Available energy:" << (AvailableEnergy - offset) << SunSpecDataPoint::registersToString(valueRegisters) << m_batteryData.availableEnergy;

                            m_batteryData.stateOfHealth = SunSpecDataPoint::convertToFloat32(values.mid(StateOfHealth - offset, 2));
                            m_batteryData.stateOfEnergy = SunSpecDataPoint::convertToFloat32(values.mid(StateOfEnergy - offset, 2));
                            m_batteryData.batteryStatus = static_cast<BatteryStatus>(SunSpecDataPoint::convertToUInt32(values.mid(Status - offset, 2)));

                            if (!m_initFinishedSuccess) {
                                m_timer.stop();
                                m_initFinishedSuccess = true;
                                emit initFinished(true);
                            }

                            emit blockDataUpdated();
                        });

                        connect(reply, &QModbusReply::errorOccurred, this, [] (QModbusDevice::Error error) {
                            qCWarning(dcSunSpec()) << "SolarEdgeBattery: Modbus reply error:" << error;
                        });
                    } else {
                        qCWarning(dcSunSpec()) << "SolarEdgeBattery: Read error: " << m_connection->modbusTcpClient()->errorString();
                        reply->deleteLater(); // broadcast replies return immediately
                        if (!m_initFinishedSuccess) {
                            m_timer.stop();
                            emit initFinished(false);
                        }
                        return;
                    }
                } else {
                    qCWarning(dcSunSpec()) << "SolarEdgeBattery: Read error: " << m_connection->modbusTcpClient()->errorString();
                    return;
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [] (QModbusDevice::Error error) {
                qCWarning(dcSunSpec()) << "SolarEdgeBattery: Modbus reply error:" << error;
            });
        } else {
            qCWarning(dcSunSpec()) << "SolarEdgeBattery: Read error: " << m_connection->modbusTcpClient()->errorString();
            reply->deleteLater(); // broadcast replies return immediately
            if (!m_initFinishedSuccess) {
                m_timer.stop();
                emit initFinished(false);
            }
            return;
        }
    } else {
        qCWarning(dcSunSpec()) << "SolarEdgeBattery: Read error: " << m_connection->modbusTcpClient()->errorString();
        return;
    }
}

QDebug operator<<(QDebug debug, const SolarEdgeBattery::BatteryData &batteryData)
{
    debug << "SolarEdgeBatteryData(" << batteryData.manufacturerName << "-" << batteryData.model << ")" << endl;
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
    debug << "    - Max energy" << batteryData.maxEnergy << "W * H" << endl;
    debug << "    - Available energy" << batteryData.availableEnergy << "W * H" << endl;
    debug << "    - State of health" << batteryData.stateOfHealth << "%" << endl;
    debug << "    - State of energy" << batteryData.stateOfEnergy << "%" << endl;
    debug << "    - Battery status" << batteryData.batteryStatus << endl;
    return debug;
}
