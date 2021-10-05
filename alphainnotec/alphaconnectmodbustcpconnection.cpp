/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2021, nymea GmbH
* Contact: contact@nymea.io
*
* This fileDescriptor is part of nymea.
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


#include "alphaconnectmodbustcpconnection.h"
#include "loggingcategories.h"

NYMEA_LOGGING_CATEGORY(dcAlphaConnectModbusTcpConnection, "AlphaConnectModbusTcpConnection")

AlphaConnectModbusTcpConnection::AlphaConnectModbusTcpConnection(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent) :
    ModbusTCPMaster(hostAddress, port, parent),
    m_slaveId(slaveId)
{
    
}

float AlphaConnectModbusTcpConnection::flowTemperature() const
{
    return m_flowTemperature;
}

float AlphaConnectModbusTcpConnection::returnTemperature() const
{
    return m_returnTemperature;
}

float AlphaConnectModbusTcpConnection::externalReturnTemperature() const
{
    return m_externalReturnTemperature;
}

float AlphaConnectModbusTcpConnection::hotWaterTemperature() const
{
    return m_hotWaterTemperature;
}

float AlphaConnectModbusTcpConnection::hotGasTemperature() const
{
    return m_hotGasTemperature;
}

float AlphaConnectModbusTcpConnection::heatSourceInletTemperature() const
{
    return m_heatSourceInletTemperature;
}

float AlphaConnectModbusTcpConnection::heatSourceOutletTemperature() const
{
    return m_heatSourceOutletTemperature;
}

float AlphaConnectModbusTcpConnection::roomTemperature1() const
{
    return m_roomTemperature1;
}

float AlphaConnectModbusTcpConnection::roomTemperature2() const
{
    return m_roomTemperature2;
}

float AlphaConnectModbusTcpConnection::roomTemperature3() const
{
    return m_roomTemperature3;
}

float AlphaConnectModbusTcpConnection::solarCollectorTemperature() const
{
    return m_solarCollectorTemperature;
}

float AlphaConnectModbusTcpConnection::solarStorageTankTemperature() const
{
    return m_solarStorageTankTemperature;
}

float AlphaConnectModbusTcpConnection::externalEnergySourceTemperature() const
{
    return m_externalEnergySourceTemperature;
}

float AlphaConnectModbusTcpConnection::supplyAirTemperature() const
{
    return m_supplyAirTemperature;
}

float AlphaConnectModbusTcpConnection::externalAirTemperature() const
{
    return m_externalAirTemperature;
}

float AlphaConnectModbusTcpConnection::rbeRoomActualTemperature() const
{
    return m_rbeRoomActualTemperature;
}

float AlphaConnectModbusTcpConnection::rbeRoomSetpointTemperature() const
{
    return m_rbeRoomSetpointTemperature;
}

quint16 AlphaConnectModbusTcpConnection::heatingPumpOperatingHours() const
{
    return m_heatingPumpOperatingHours;
}

AlphaConnectModbusTcpConnection::SystemStatus AlphaConnectModbusTcpConnection::systemStatus() const
{
    return m_systemStatus;
}

float AlphaConnectModbusTcpConnection::heatingEnergy() const
{
    return m_heatingEnergy;
}

float AlphaConnectModbusTcpConnection::waterHeatEnergy() const
{
    return m_waterHeatEnergy;
}

float AlphaConnectModbusTcpConnection::totalHeatEnergy() const
{
    return m_totalHeatEnergy;
}

float AlphaConnectModbusTcpConnection::outdoorTemperature() const
{
    return m_outdoorTemperature;
}

QModbusReply *AlphaConnectModbusTcpConnection::setOutdoorTemperature(float outdoorTemperature)
{
    QVector<quint16> values = ModbusDataUtils::convertFromUInt16(static_cast<quint16>(outdoorTemperature  * 1.0 / pow(10, -1)));
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Write \"Outdoor temperature\" register:" << 0 << "size:" << 1 << values;
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 0, values.count());
    request.setValues(values);
    return sendWriteRequest(request, m_slaveId);
}

float AlphaConnectModbusTcpConnection::returnSetpointTemperature() const
{
    return m_returnSetpointTemperature;
}

QModbusReply *AlphaConnectModbusTcpConnection::setReturnSetpointTemperature(float returnSetpointTemperature)
{
    QVector<quint16> values = ModbusDataUtils::convertFromUInt16(static_cast<quint16>(returnSetpointTemperature  * 1.0 / pow(10, -1)));
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Write \"Return setpoint temperature\" register:" << 1 << "size:" << 1 << values;
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 1, values.count());
    request.setValues(values);
    return sendWriteRequest(request, m_slaveId);
}

float AlphaConnectModbusTcpConnection::hotWaterSetpointTemperature() const
{
    return m_hotWaterSetpointTemperature;
}

QModbusReply *AlphaConnectModbusTcpConnection::setHotWaterSetpointTemperature(float hotWaterSetpointTemperature)
{
    QVector<quint16> values = ModbusDataUtils::convertFromUInt16(static_cast<quint16>(hotWaterSetpointTemperature  * 1.0 / pow(10, -1)));
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Write \"Hot water setpoint temperature\" register:" << 5 << "size:" << 1 << values;
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 5, values.count());
    request.setValues(values);
    return sendWriteRequest(request, m_slaveId);
}

AlphaConnectModbusTcpConnection::SmartGridState AlphaConnectModbusTcpConnection::smartGrid() const
{
    return m_smartGrid;
}

QModbusReply *AlphaConnectModbusTcpConnection::setSmartGrid(SmartGridState smartGrid)
{
    QVector<quint16> values = ModbusDataUtils::convertFromUInt16(static_cast<quint16>(smartGrid));
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Write \"Smart grid control\" register:" << 14 << "size:" << 1 << values;
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 14, values.count());
    request.setValues(values);
    return sendWriteRequest(request, m_slaveId);
}

void AlphaConnectModbusTcpConnection::initialize()
{
    // No init registers defined. Nothing to be done and we are finished.
    emit initializationFinished();
}

void AlphaConnectModbusTcpConnection::update()
{
    updateFlowTemperature();
    updateReturnTemperature();
    updateExternalReturnTemperature();
    updateHotWaterTemperature();
    updateHotGasTemperature();
    updateHeatSourceInletTemperature();
    updateHeatSourceOutletTemperature();
    updateRoomTemperature1();
    updateRoomTemperature2();
    updateRoomTemperature3();
    updateSolarCollectorTemperature();
    updateSolarStorageTankTemperature();
    updateExternalEnergySourceTemperature();
    updateSupplyAirTemperature();
    updateExternalAirTemperature();
    updateRbeRoomActualTemperature();
    updateRbeRoomSetpointTemperature();
    updateHeatingPumpOperatingHours();
    updateSystemStatus();
    updateHeatingEnergy();
    updateWaterHeatEnergy();
    updateTotalHeatEnergy();
    updateOutdoorTemperature();
    updateReturnSetpointTemperature();
    updateHotWaterSetpointTemperature();
    updateSmartGrid();
}

void AlphaConnectModbusTcpConnection::updateFlowTemperature()
{
    // Update registers from Flow
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"Flow\" register:" << 1 << "size:" << 1;
    QModbusReply *reply = readFlowTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"Flow\" register" << 1 << "size:" << 1 << unit.values();
                    float receivedFlowTemperature = ModbusDataUtils::convertToUInt16(unit.values()) * 1.0 * pow(10, -1);
                    if (m_flowTemperature != receivedFlowTemperature) {
                        m_flowTemperature = receivedFlowTemperature;
                        emit flowTemperatureChanged(m_flowTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"Flow\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"Flow\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateReturnTemperature()
{
    // Update registers from Return
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"Return\" register:" << 2 << "size:" << 1;
    QModbusReply *reply = readReturnTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"Return\" register" << 2 << "size:" << 1 << unit.values();
                    float receivedReturnTemperature = ModbusDataUtils::convertToUInt16(unit.values()) * 1.0 * pow(10, -1);
                    if (m_returnTemperature != receivedReturnTemperature) {
                        m_returnTemperature = receivedReturnTemperature;
                        emit returnTemperatureChanged(m_returnTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"Return\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"Return\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateExternalReturnTemperature()
{
    // Update registers from External return
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"External return\" register:" << 3 << "size:" << 1;
    QModbusReply *reply = readExternalReturnTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"External return\" register" << 3 << "size:" << 1 << unit.values();
                    float receivedExternalReturnTemperature = ModbusDataUtils::convertToUInt16(unit.values()) * 1.0 * pow(10, -1);
                    if (m_externalReturnTemperature != receivedExternalReturnTemperature) {
                        m_externalReturnTemperature = receivedExternalReturnTemperature;
                        emit externalReturnTemperatureChanged(m_externalReturnTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"External return\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"External return\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateHotWaterTemperature()
{
    // Update registers from Hot water temperature
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"Hot water temperature\" register:" << 4 << "size:" << 1;
    QModbusReply *reply = readHotWaterTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"Hot water temperature\" register" << 4 << "size:" << 1 << unit.values();
                    float receivedHotWaterTemperature = ModbusDataUtils::convertToUInt16(unit.values()) * 1.0 * pow(10, -1);
                    if (m_hotWaterTemperature != receivedHotWaterTemperature) {
                        m_hotWaterTemperature = receivedHotWaterTemperature;
                        emit hotWaterTemperatureChanged(m_hotWaterTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"Hot water temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"Hot water temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateHotGasTemperature()
{
    // Update registers from Hot gas temperature
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"Hot gas temperature\" register:" << 8 << "size:" << 1;
    QModbusReply *reply = readHotGasTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"Hot gas temperature\" register" << 8 << "size:" << 1 << unit.values();
                    float receivedHotGasTemperature = ModbusDataUtils::convertToUInt16(unit.values()) * 1.0 * pow(10, -1);
                    if (m_hotGasTemperature != receivedHotGasTemperature) {
                        m_hotGasTemperature = receivedHotGasTemperature;
                        emit hotGasTemperatureChanged(m_hotGasTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"Hot gas temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"Hot gas temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateHeatSourceInletTemperature()
{
    // Update registers from Heat source inlet temperature
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"Heat source inlet temperature\" register:" << 9 << "size:" << 1;
    QModbusReply *reply = readHeatSourceInletTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"Heat source inlet temperature\" register" << 9 << "size:" << 1 << unit.values();
                    float receivedHeatSourceInletTemperature = ModbusDataUtils::convertToUInt16(unit.values()) * 1.0 * pow(10, -1);
                    if (m_heatSourceInletTemperature != receivedHeatSourceInletTemperature) {
                        m_heatSourceInletTemperature = receivedHeatSourceInletTemperature;
                        emit heatSourceInletTemperatureChanged(m_heatSourceInletTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"Heat source inlet temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"Heat source inlet temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateHeatSourceOutletTemperature()
{
    // Update registers from Heat source outlet temperature
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"Heat source outlet temperature\" register:" << 10 << "size:" << 1;
    QModbusReply *reply = readHeatSourceOutletTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"Heat source outlet temperature\" register" << 10 << "size:" << 1 << unit.values();
                    float receivedHeatSourceOutletTemperature = ModbusDataUtils::convertToUInt16(unit.values()) * 1.0 * pow(10, -1);
                    if (m_heatSourceOutletTemperature != receivedHeatSourceOutletTemperature) {
                        m_heatSourceOutletTemperature = receivedHeatSourceOutletTemperature;
                        emit heatSourceOutletTemperatureChanged(m_heatSourceOutletTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"Heat source outlet temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"Heat source outlet temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateRoomTemperature1()
{
    // Update registers from Room remote adjuster 1 temperature
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"Room remote adjuster 1 temperature\" register:" << 11 << "size:" << 1;
    QModbusReply *reply = readRoomTemperature1();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"Room remote adjuster 1 temperature\" register" << 11 << "size:" << 1 << unit.values();
                    float receivedRoomTemperature1 = ModbusDataUtils::convertToUInt16(unit.values()) * 1.0 * pow(10, -1);
                    if (m_roomTemperature1 != receivedRoomTemperature1) {
                        m_roomTemperature1 = receivedRoomTemperature1;
                        emit roomTemperature1Changed(m_roomTemperature1);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"Room remote adjuster 1 temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"Room remote adjuster 1 temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateRoomTemperature2()
{
    // Update registers from Room remote adjuster 2 temperature
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"Room remote adjuster 2 temperature\" register:" << 12 << "size:" << 1;
    QModbusReply *reply = readRoomTemperature2();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"Room remote adjuster 2 temperature\" register" << 12 << "size:" << 1 << unit.values();
                    float receivedRoomTemperature2 = ModbusDataUtils::convertToUInt16(unit.values()) * 1.0 * pow(10, -1);
                    if (m_roomTemperature2 != receivedRoomTemperature2) {
                        m_roomTemperature2 = receivedRoomTemperature2;
                        emit roomTemperature2Changed(m_roomTemperature2);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"Room remote adjuster 2 temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"Room remote adjuster 2 temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateRoomTemperature3()
{
    // Update registers from Room remote adjuster 3 temperature
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"Room remote adjuster 3 temperature\" register:" << 13 << "size:" << 1;
    QModbusReply *reply = readRoomTemperature3();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"Room remote adjuster 3 temperature\" register" << 13 << "size:" << 1 << unit.values();
                    float receivedRoomTemperature3 = ModbusDataUtils::convertToUInt16(unit.values()) * 1.0 * pow(10, -1);
                    if (m_roomTemperature3 != receivedRoomTemperature3) {
                        m_roomTemperature3 = receivedRoomTemperature3;
                        emit roomTemperature3Changed(m_roomTemperature3);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"Room remote adjuster 3 temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"Room remote adjuster 3 temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateSolarCollectorTemperature()
{
    // Update registers from Solar collector temperature
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"Solar collector temperature\" register:" << 14 << "size:" << 1;
    QModbusReply *reply = readSolarCollectorTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"Solar collector temperature\" register" << 14 << "size:" << 1 << unit.values();
                    float receivedSolarCollectorTemperature = ModbusDataUtils::convertToUInt16(unit.values()) * 1.0 * pow(10, -1);
                    if (m_solarCollectorTemperature != receivedSolarCollectorTemperature) {
                        m_solarCollectorTemperature = receivedSolarCollectorTemperature;
                        emit solarCollectorTemperatureChanged(m_solarCollectorTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"Solar collector temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"Solar collector temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateSolarStorageTankTemperature()
{
    // Update registers from Solar storage tank temperature
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"Solar storage tank temperature\" register:" << 15 << "size:" << 1;
    QModbusReply *reply = readSolarStorageTankTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"Solar storage tank temperature\" register" << 15 << "size:" << 1 << unit.values();
                    float receivedSolarStorageTankTemperature = ModbusDataUtils::convertToUInt16(unit.values()) * 1.0 * pow(10, -1);
                    if (m_solarStorageTankTemperature != receivedSolarStorageTankTemperature) {
                        m_solarStorageTankTemperature = receivedSolarStorageTankTemperature;
                        emit solarStorageTankTemperatureChanged(m_solarStorageTankTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"Solar storage tank temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"Solar storage tank temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateExternalEnergySourceTemperature()
{
    // Update registers from External energy source temperature
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"External energy source temperature\" register:" << 16 << "size:" << 1;
    QModbusReply *reply = readExternalEnergySourceTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"External energy source temperature\" register" << 16 << "size:" << 1 << unit.values();
                    float receivedExternalEnergySourceTemperature = ModbusDataUtils::convertToUInt16(unit.values()) * 1.0 * pow(10, -1);
                    if (m_externalEnergySourceTemperature != receivedExternalEnergySourceTemperature) {
                        m_externalEnergySourceTemperature = receivedExternalEnergySourceTemperature;
                        emit externalEnergySourceTemperatureChanged(m_externalEnergySourceTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"External energy source temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"External energy source temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateSupplyAirTemperature()
{
    // Update registers from Supply air temperature
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"Supply air temperature\" register:" << 17 << "size:" << 1;
    QModbusReply *reply = readSupplyAirTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"Supply air temperature\" register" << 17 << "size:" << 1 << unit.values();
                    float receivedSupplyAirTemperature = ModbusDataUtils::convertToUInt16(unit.values()) * 1.0 * pow(10, -1);
                    if (m_supplyAirTemperature != receivedSupplyAirTemperature) {
                        m_supplyAirTemperature = receivedSupplyAirTemperature;
                        emit supplyAirTemperatureChanged(m_supplyAirTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"Supply air temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"Supply air temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateExternalAirTemperature()
{
    // Update registers from External air temperature
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"External air temperature\" register:" << 18 << "size:" << 1;
    QModbusReply *reply = readExternalAirTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"External air temperature\" register" << 18 << "size:" << 1 << unit.values();
                    float receivedExternalAirTemperature = ModbusDataUtils::convertToUInt16(unit.values()) * 1.0 * pow(10, -1);
                    if (m_externalAirTemperature != receivedExternalAirTemperature) {
                        m_externalAirTemperature = receivedExternalAirTemperature;
                        emit externalAirTemperatureChanged(m_externalAirTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"External air temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"External air temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateRbeRoomActualTemperature()
{
    // Update registers from RBE actual room temperature
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"RBE actual room temperature\" register:" << 24 << "size:" << 1;
    QModbusReply *reply = readRbeRoomActualTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"RBE actual room temperature\" register" << 24 << "size:" << 1 << unit.values();
                    float receivedRbeRoomActualTemperature = ModbusDataUtils::convertToUInt16(unit.values()) * 1.0 * pow(10, -1);
                    if (m_rbeRoomActualTemperature != receivedRbeRoomActualTemperature) {
                        m_rbeRoomActualTemperature = receivedRbeRoomActualTemperature;
                        emit rbeRoomActualTemperatureChanged(m_rbeRoomActualTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"RBE actual room temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"RBE actual room temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateRbeRoomSetpointTemperature()
{
    // Update registers from RBE room temperature setpoint
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"RBE room temperature setpoint\" register:" << 24 << "size:" << 1;
    QModbusReply *reply = readRbeRoomSetpointTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"RBE room temperature setpoint\" register" << 24 << "size:" << 1 << unit.values();
                    float receivedRbeRoomSetpointTemperature = ModbusDataUtils::convertToUInt16(unit.values()) * 1.0 * pow(10, -1);
                    if (m_rbeRoomSetpointTemperature != receivedRbeRoomSetpointTemperature) {
                        m_rbeRoomSetpointTemperature = receivedRbeRoomSetpointTemperature;
                        emit rbeRoomSetpointTemperatureChanged(m_rbeRoomSetpointTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"RBE room temperature setpoint\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"RBE room temperature setpoint\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateHeatingPumpOperatingHours()
{
    // Update registers from Heating pump operating hours
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"Heating pump operating hours\" register:" << 33 << "size:" << 1;
    QModbusReply *reply = readHeatingPumpOperatingHours();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"Heating pump operating hours\" register" << 33 << "size:" << 1 << unit.values();
                    quint16 receivedHeatingPumpOperatingHours = ModbusDataUtils::convertToUInt16(unit.values());
                    if (m_heatingPumpOperatingHours != receivedHeatingPumpOperatingHours) {
                        m_heatingPumpOperatingHours = receivedHeatingPumpOperatingHours;
                        emit heatingPumpOperatingHoursChanged(m_heatingPumpOperatingHours);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"Heating pump operating hours\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"Heating pump operating hours\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateSystemStatus()
{
    // Update registers from System status
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"System status\" register:" << 37 << "size:" << 1;
    QModbusReply *reply = readSystemStatus();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"System status\" register" << 37 << "size:" << 1 << unit.values();
                    SystemStatus receivedSystemStatus = static_cast<SystemStatus>(ModbusDataUtils::convertToUInt16(unit.values()));
                    if (m_systemStatus != receivedSystemStatus) {
                        m_systemStatus = receivedSystemStatus;
                        emit systemStatusChanged(m_systemStatus);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"System status\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"System status\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateHeatingEnergy()
{
    // Update registers from Heating energy
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"Heating energy\" register:" << 38 << "size:" << 2;
    QModbusReply *reply = readHeatingEnergy();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"Heating energy\" register" << 38 << "size:" << 2 << unit.values();
                    float receivedHeatingEnergy = ModbusDataUtils::convertToUInt32(unit.values(), ModbusDataUtils::ByteOrderBigEndian) * 1.0 * pow(10, -1);
                    if (m_heatingEnergy != receivedHeatingEnergy) {
                        m_heatingEnergy = receivedHeatingEnergy;
                        emit heatingEnergyChanged(m_heatingEnergy);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"Heating energy\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"Heating energy\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateWaterHeatEnergy()
{
    // Update registers from Water heat energy
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"Water heat energy\" register:" << 40 << "size:" << 2;
    QModbusReply *reply = readWaterHeatEnergy();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"Water heat energy\" register" << 40 << "size:" << 2 << unit.values();
                    float receivedWaterHeatEnergy = ModbusDataUtils::convertToUInt32(unit.values(), ModbusDataUtils::ByteOrderBigEndian) * 1.0 * pow(10, -1);
                    if (m_waterHeatEnergy != receivedWaterHeatEnergy) {
                        m_waterHeatEnergy = receivedWaterHeatEnergy;
                        emit waterHeatEnergyChanged(m_waterHeatEnergy);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"Water heat energy\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"Water heat energy\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateTotalHeatEnergy()
{
    // Update registers from Total energy
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"Total energy\" register:" << 44 << "size:" << 2;
    QModbusReply *reply = readTotalHeatEnergy();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"Total energy\" register" << 44 << "size:" << 2 << unit.values();
                    float receivedTotalHeatEnergy = ModbusDataUtils::convertToUInt32(unit.values(), ModbusDataUtils::ByteOrderBigEndian) * 1.0 * pow(10, -1);
                    if (m_totalHeatEnergy != receivedTotalHeatEnergy) {
                        m_totalHeatEnergy = receivedTotalHeatEnergy;
                        emit totalHeatEnergyChanged(m_totalHeatEnergy);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"Total energy\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"Total energy\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateOutdoorTemperature()
{
    // Update registers from Outdoor temperature
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"Outdoor temperature\" register:" << 0 << "size:" << 1;
    QModbusReply *reply = readOutdoorTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"Outdoor temperature\" register" << 0 << "size:" << 1 << unit.values();
                    float receivedOutdoorTemperature = ModbusDataUtils::convertToUInt16(unit.values()) * 1.0 * pow(10, -1);
                    if (m_outdoorTemperature != receivedOutdoorTemperature) {
                        m_outdoorTemperature = receivedOutdoorTemperature;
                        emit outdoorTemperatureChanged(m_outdoorTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"Outdoor temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"Outdoor temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateReturnSetpointTemperature()
{
    // Update registers from Return setpoint temperature
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"Return setpoint temperature\" register:" << 1 << "size:" << 1;
    QModbusReply *reply = readReturnSetpointTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"Return setpoint temperature\" register" << 1 << "size:" << 1 << unit.values();
                    float receivedReturnSetpointTemperature = ModbusDataUtils::convertToUInt16(unit.values()) * 1.0 * pow(10, -1);
                    if (m_returnSetpointTemperature != receivedReturnSetpointTemperature) {
                        m_returnSetpointTemperature = receivedReturnSetpointTemperature;
                        emit returnSetpointTemperatureChanged(m_returnSetpointTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"Return setpoint temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"Return setpoint temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateHotWaterSetpointTemperature()
{
    // Update registers from Hot water setpoint temperature
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"Hot water setpoint temperature\" register:" << 5 << "size:" << 1;
    QModbusReply *reply = readHotWaterSetpointTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"Hot water setpoint temperature\" register" << 5 << "size:" << 1 << unit.values();
                    float receivedHotWaterSetpointTemperature = ModbusDataUtils::convertToUInt16(unit.values()) * 1.0 * pow(10, -1);
                    if (m_hotWaterSetpointTemperature != receivedHotWaterSetpointTemperature) {
                        m_hotWaterSetpointTemperature = receivedHotWaterSetpointTemperature;
                        emit hotWaterSetpointTemperatureChanged(m_hotWaterSetpointTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"Hot water setpoint temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"Hot water setpoint temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void AlphaConnectModbusTcpConnection::updateSmartGrid()
{
    // Update registers from Smart grid control
    qCDebug(dcAlphaConnectModbusTcpConnection()) << "--> Read \"Smart grid control\" register:" << 14 << "size:" << 1;
    QModbusReply *reply = readSmartGrid();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    qCDebug(dcAlphaConnectModbusTcpConnection()) << "<-- Response from \"Smart grid control\" register" << 14 << "size:" << 1 << unit.values();
                    SmartGridState receivedSmartGrid = static_cast<SmartGridState>(ModbusDataUtils::convertToUInt16(unit.values()));
                    if (m_smartGrid != receivedSmartGrid) {
                        m_smartGrid = receivedSmartGrid;
                        emit smartGridChanged(m_smartGrid);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcAlphaConnectModbusTcpConnection()) << "Modbus reply error occurred while updating \"Smart grid control\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcAlphaConnectModbusTcpConnection()) << "Error occurred while reading \"Smart grid control\" registers from" << hostAddress().toString() << errorString();
    }
}

QModbusReply *AlphaConnectModbusTcpConnection::readFlowTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 1, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readReturnTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 2, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readExternalReturnTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 3, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readHotWaterTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 4, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readHotGasTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 8, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readHeatSourceInletTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 9, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readHeatSourceOutletTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 10, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readRoomTemperature1()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 11, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readRoomTemperature2()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 12, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readRoomTemperature3()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 13, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readSolarCollectorTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 14, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readSolarStorageTankTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 15, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readExternalEnergySourceTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 16, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readSupplyAirTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 17, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readExternalAirTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 18, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readRbeRoomActualTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 24, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readRbeRoomSetpointTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 24, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readHeatingPumpOperatingHours()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 33, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readSystemStatus()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 37, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readHeatingEnergy()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 38, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readWaterHeatEnergy()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 40, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readTotalHeatEnergy()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 44, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readOutdoorTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 0, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readReturnSetpointTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 1, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readHotWaterSetpointTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 5, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *AlphaConnectModbusTcpConnection::readSmartGrid()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 14, 1);
    return sendReadRequest(request, m_slaveId);
}

void AlphaConnectModbusTcpConnection::verifyInitFinished()
{
    if (m_pendingInitReplies.isEmpty()) {
        qCDebug(dcAlphaConnectModbusTcpConnection()) << "Initialization finished of AlphaConnectModbusTcpConnection" << hostAddress().toString();
        emit initializationFinished();
    }
}

QDebug operator<<(QDebug debug, AlphaConnectModbusTcpConnection *alphaConnectModbusTcpConnection)
{
    debug.nospace().noquote() << "AlphaConnectModbusTcpConnection(" << alphaConnectModbusTcpConnection->hostAddress().toString() << ":" << alphaConnectModbusTcpConnection->port() << ")" << "\n";
    debug.nospace().noquote() << "    - Flow:" << alphaConnectModbusTcpConnection->flowTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Return:" << alphaConnectModbusTcpConnection->returnTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - External return:" << alphaConnectModbusTcpConnection->externalReturnTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Hot water temperature:" << alphaConnectModbusTcpConnection->hotWaterTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Hot gas temperature:" << alphaConnectModbusTcpConnection->hotGasTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Heat source inlet temperature:" << alphaConnectModbusTcpConnection->heatSourceInletTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Heat source outlet temperature:" << alphaConnectModbusTcpConnection->heatSourceOutletTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Room remote adjuster 1 temperature:" << alphaConnectModbusTcpConnection->roomTemperature1() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Room remote adjuster 2 temperature:" << alphaConnectModbusTcpConnection->roomTemperature2() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Room remote adjuster 3 temperature:" << alphaConnectModbusTcpConnection->roomTemperature3() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Solar collector temperature:" << alphaConnectModbusTcpConnection->solarCollectorTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Solar storage tank temperature:" << alphaConnectModbusTcpConnection->solarStorageTankTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - External energy source temperature:" << alphaConnectModbusTcpConnection->externalEnergySourceTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Supply air temperature:" << alphaConnectModbusTcpConnection->supplyAirTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - External air temperature:" << alphaConnectModbusTcpConnection->externalAirTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - RBE actual room temperature:" << alphaConnectModbusTcpConnection->rbeRoomActualTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - RBE room temperature setpoint:" << alphaConnectModbusTcpConnection->rbeRoomSetpointTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Heating pump operating hours:" << alphaConnectModbusTcpConnection->heatingPumpOperatingHours() << " [h]" << "\n";
    debug.nospace().noquote() << "    - System status:" << alphaConnectModbusTcpConnection->systemStatus() << "\n";
    debug.nospace().noquote() << "    - Heating energy:" << alphaConnectModbusTcpConnection->heatingEnergy() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Water heat energy:" << alphaConnectModbusTcpConnection->waterHeatEnergy() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Total energy:" << alphaConnectModbusTcpConnection->totalHeatEnergy() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Outdoor temperature:" << alphaConnectModbusTcpConnection->outdoorTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Return setpoint temperature:" << alphaConnectModbusTcpConnection->returnSetpointTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Hot water setpoint temperature:" << alphaConnectModbusTcpConnection->hotWaterSetpointTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Smart grid control:" << alphaConnectModbusTcpConnection->smartGrid() << "\n";
    return debug.quote().space();
}

