/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2022, nymea GmbH
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


#include "stiebeleltronmodbusconnection.h"
#include "loggingcategories.h"

NYMEA_LOGGING_CATEGORY(dcStiebelEltronModbusConnection, "StiebelEltronModbusConnection")

StiebelEltronModbusConnection::StiebelEltronModbusConnection(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent) :
    ModbusTCPMaster(hostAddress, port, parent),
    m_slaveId(slaveId)
{
    
}

float StiebelEltronModbusConnection::outdoorTemperature() const
{
    return m_outdoorTemperature;
}

float StiebelEltronModbusConnection::flowTemperature() const
{
    return m_flowTemperature;
}

float StiebelEltronModbusConnection::hotWaterTemperature() const
{
    return m_hotWaterTemperature;
}

float StiebelEltronModbusConnection::hotGasTemperature1() const
{
    return m_hotGasTemperature1;
}

float StiebelEltronModbusConnection::hotGasTemperature2() const
{
    return m_hotGasTemperature2;
}

float StiebelEltronModbusConnection::SourceTemperature() const
{
    return m_SourceTemperature;
}

float StiebelEltronModbusConnection::roomTemperatureFEK() const
{
    return m_roomTemperatureFEK;
}

float StiebelEltronModbusConnection::returnTemperature() const
{
    return m_returnTemperature;
}

float StiebelEltronModbusConnection::solarCollectorTemperature() const
{
    return m_solarCollectorTemperature;
}

float StiebelEltronModbusConnection::solarStorageTankTemperature() const
{
    return m_solarStorageTankTemperature;
}

float StiebelEltronModbusConnection::storageTankTemperature() const
{
    return m_storageTankTemperature;
}

float StiebelEltronModbusConnection::externalHeatSourceTemperature() const
{
    return m_externalHeatSourceTemperature;
}

quint32 StiebelEltronModbusConnection::heatingEnergy() const
{
    return m_heatingEnergy;
}

quint32 StiebelEltronModbusConnection::hotWaterEnergy() const
{
    return m_hotWaterEnergy;
}

quint32 StiebelEltronModbusConnection::consumedEnergyHeating() const
{
    return m_consumedEnergyHeating;
}

quint32 StiebelEltronModbusConnection::consumedEnergyHotWater() const
{
    return m_consumedEnergyHotWater;
}

StiebelEltronModbusConnection::OperatingMode StiebelEltronModbusConnection::operatingMode() const
{
    return m_operatingMode;
}

quint16 StiebelEltronModbusConnection::systemStatus() const
{
    return m_systemStatus;
}

quint16 StiebelEltronModbusConnection::sgReadyStateRO() const
{
    return m_sgReadyStateRO;
}

quint16 StiebelEltronModbusConnection::sgReadyActive() const
{
    return m_sgReadyActive;
}

QModbusReply *StiebelEltronModbusConnection::setSgReadyActive(quint16 sgReadyActive)
{
    QVector<quint16> values = ModbusDataUtils::convertFromUInt16(sgReadyActive);
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Write \"SG ready active\" register:" << 4000 << "size:" << 1 << values;
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 4000, values.count());
    request.setValues(values);
    return sendWriteRequest(request, m_slaveId);
}

StiebelEltronModbusConnection::SmartGridState StiebelEltronModbusConnection::sgReadyState() const
{
    return m_sgReadyState;
}

QModbusReply *StiebelEltronModbusConnection::setSgReadyState(SmartGridState sgReadyState)
{
    QVector<quint16> values = ModbusDataUtils::convertFromUInt32(static_cast<quint32>(sgReadyState), ModbusDataUtils::ByteOrderBigEndian);
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Write \"SG Ready mode\" register:" << 4001 << "size:" << 2 << values;
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 4001, values.count());
    request.setValues(values);
    return sendWriteRequest(request, m_slaveId);
}

void StiebelEltronModbusConnection::initialize()
{
    // No init registers defined. Nothing to be done and we are finished.
    emit initializationFinished();
}

void StiebelEltronModbusConnection::update()
{
    updateOutdoorTemperature();
    updateFlowTemperature();
    updateHotWaterTemperature();
    updateHotGasTemperature1();
    updateHotGasTemperature2();
    updateSourceTemperature();
    updateRoomTemperatureFEK();
    updateReturnTemperature();
    updateSolarCollectorTemperature();
    updateSolarStorageTankTemperature();
    updateStorageTankTemperature();
    updateExternalHeatSourceTemperature();
    updateHeatingEnergy();
    updateHotWaterEnergy();
    updateConsumedEnergyHeating();
    updateConsumedEnergyHotWater();
    updateOperatingMode();
    updateSystemStatus();
    updateSgReadyStateRO();
    updateSgReadyActive();
    updateSgReadyState();
}

void StiebelEltronModbusConnection::updateOutdoorTemperature()
{
    // Update registers from Outdoor temperature
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"Outdoor temperature\" register:" << 506 << "size:" << 1;
    QModbusReply *reply = readOutdoorTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"Outdoor temperature\" register" << 506 << "size:" << 1 << values;
                    float receivedOutdoorTemperature = ModbusDataUtils::convertToInt16(values) * 1.0 * pow(10, -1);
                    if (m_outdoorTemperature != receivedOutdoorTemperature) {
                        m_outdoorTemperature = receivedOutdoorTemperature;
                        emit outdoorTemperatureChanged(m_outdoorTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"Outdoor temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"Outdoor temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void StiebelEltronModbusConnection::updateFlowTemperature()
{
    // Update registers from Flow temperature
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"Flow temperature\" register:" << 514 << "size:" << 1;
    QModbusReply *reply = readFlowTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"Flow temperature\" register" << 514 << "size:" << 1 << values;
                    float receivedFlowTemperature = ModbusDataUtils::convertToInt16(values) * 1.0 * pow(10, -1);
                    if (m_flowTemperature != receivedFlowTemperature) {
                        m_flowTemperature = receivedFlowTemperature;
                        emit flowTemperatureChanged(m_flowTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"Flow temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"Flow temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void StiebelEltronModbusConnection::updateHotWaterTemperature()
{
    // Update registers from Hot water temperature
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"Hot water temperature\" register:" << 521 << "size:" << 1;
    QModbusReply *reply = readHotWaterTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"Hot water temperature\" register" << 521 << "size:" << 1 << values;
                    float receivedHotWaterTemperature = ModbusDataUtils::convertToUInt16(values) * 1.0 * pow(10, -1);
                    if (m_hotWaterTemperature != receivedHotWaterTemperature) {
                        m_hotWaterTemperature = receivedHotWaterTemperature;
                        emit hotWaterTemperatureChanged(m_hotWaterTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"Hot water temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"Hot water temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void StiebelEltronModbusConnection::updateHotGasTemperature1()
{
    // Update registers from Hot gas temperature HP 1
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"Hot gas temperature HP 1\" register:" << 543 << "size:" << 1;
    QModbusReply *reply = readHotGasTemperature1();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"Hot gas temperature HP 1\" register" << 543 << "size:" << 1 << values;
                    float receivedHotGasTemperature1 = ModbusDataUtils::convertToUInt16(values) * 1.0 * pow(10, -1);
                    if (m_hotGasTemperature1 != receivedHotGasTemperature1) {
                        m_hotGasTemperature1 = receivedHotGasTemperature1;
                        emit hotGasTemperature1Changed(m_hotGasTemperature1);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"Hot gas temperature HP 1\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"Hot gas temperature HP 1\" registers from" << hostAddress().toString() << errorString();
    }
}

void StiebelEltronModbusConnection::updateHotGasTemperature2()
{
    // Update registers from Hot gas temperature HP 2
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"Hot gas temperature HP 2\" register:" << 550 << "size:" << 1;
    QModbusReply *reply = readHotGasTemperature2();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"Hot gas temperature HP 2\" register" << 550 << "size:" << 1 << values;
                    float receivedHotGasTemperature2 = ModbusDataUtils::convertToUInt16(values) * 1.0 * pow(10, -1);
                    if (m_hotGasTemperature2 != receivedHotGasTemperature2) {
                        m_hotGasTemperature2 = receivedHotGasTemperature2;
                        emit hotGasTemperature2Changed(m_hotGasTemperature2);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"Hot gas temperature HP 2\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"Hot gas temperature HP 2\" registers from" << hostAddress().toString() << errorString();
    }
}

void StiebelEltronModbusConnection::updateSourceTemperature()
{
    // Update registers from Source temperature
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"Source temperature\" register:" << 562 << "size:" << 1;
    QModbusReply *reply = readSourceTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"Source temperature\" register" << 562 << "size:" << 1 << values;
                    float receivedSourceTemperature = ModbusDataUtils::convertToUInt16(values) * 1.0 * pow(10, -1);
                    if (m_SourceTemperature != receivedSourceTemperature) {
                        m_SourceTemperature = receivedSourceTemperature;
                        emit SourceTemperatureChanged(m_SourceTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"Source temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"Source temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void StiebelEltronModbusConnection::updateRoomTemperatureFEK()
{
    // Update registers from Room temperature FEK
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"Room temperature FEK\" register:" << 502 << "size:" << 1;
    QModbusReply *reply = readRoomTemperatureFEK();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"Room temperature FEK\" register" << 502 << "size:" << 1 << values;
                    float receivedRoomTemperatureFEK = ModbusDataUtils::convertToUInt16(values) * 1.0 * pow(10, -1);
                    if (m_roomTemperatureFEK != receivedRoomTemperatureFEK) {
                        m_roomTemperatureFEK = receivedRoomTemperatureFEK;
                        emit roomTemperatureFEKChanged(m_roomTemperatureFEK);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"Room temperature FEK\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"Room temperature FEK\" registers from" << hostAddress().toString() << errorString();
    }
}

void StiebelEltronModbusConnection::updateReturnTemperature()
{
    // Update registers from Return temperature
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"Return temperature\" register:" << 515 << "size:" << 1;
    QModbusReply *reply = readReturnTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"Return temperature\" register" << 515 << "size:" << 1 << values;
                    float receivedReturnTemperature = ModbusDataUtils::convertToInt16(values) * 1.0 * pow(10, -1);
                    if (m_returnTemperature != receivedReturnTemperature) {
                        m_returnTemperature = receivedReturnTemperature;
                        emit returnTemperatureChanged(m_returnTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"Return temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"Return temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void StiebelEltronModbusConnection::updateSolarCollectorTemperature()
{
    // Update registers from Solar collector temperature
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"Solar collector temperature\" register:" << 527 << "size:" << 1;
    QModbusReply *reply = readSolarCollectorTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"Solar collector temperature\" register" << 527 << "size:" << 1 << values;
                    float receivedSolarCollectorTemperature = ModbusDataUtils::convertToUInt16(values) * 1.0 * pow(10, -1);
                    if (m_solarCollectorTemperature != receivedSolarCollectorTemperature) {
                        m_solarCollectorTemperature = receivedSolarCollectorTemperature;
                        emit solarCollectorTemperatureChanged(m_solarCollectorTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"Solar collector temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"Solar collector temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void StiebelEltronModbusConnection::updateSolarStorageTankTemperature()
{
    // Update registers from Solar storage tank temperature
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"Solar storage tank temperature\" register:" << 528 << "size:" << 1;
    QModbusReply *reply = readSolarStorageTankTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"Solar storage tank temperature\" register" << 528 << "size:" << 1 << values;
                    float receivedSolarStorageTankTemperature = ModbusDataUtils::convertToUInt16(values) * 1.0 * pow(10, -1);
                    if (m_solarStorageTankTemperature != receivedSolarStorageTankTemperature) {
                        m_solarStorageTankTemperature = receivedSolarStorageTankTemperature;
                        emit solarStorageTankTemperatureChanged(m_solarStorageTankTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"Solar storage tank temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"Solar storage tank temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void StiebelEltronModbusConnection::updateStorageTankTemperature()
{
    // Update registers from Storage tank temperature
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"Storage tank temperature\" register:" << 517 << "size:" << 1;
    QModbusReply *reply = readStorageTankTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"Storage tank temperature\" register" << 517 << "size:" << 1 << values;
                    float receivedStorageTankTemperature = ModbusDataUtils::convertToUInt16(values) * 1.0 * pow(10, -1);
                    if (m_storageTankTemperature != receivedStorageTankTemperature) {
                        m_storageTankTemperature = receivedStorageTankTemperature;
                        emit storageTankTemperatureChanged(m_storageTankTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"Storage tank temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"Storage tank temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void StiebelEltronModbusConnection::updateExternalHeatSourceTemperature()
{
    // Update registers from External heat source temperature
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"External heat source temperature\" register:" << 530 << "size:" << 1;
    QModbusReply *reply = readExternalHeatSourceTemperature();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"External heat source temperature\" register" << 530 << "size:" << 1 << values;
                    float receivedExternalHeatSourceTemperature = ModbusDataUtils::convertToUInt16(values) * 1.0 * pow(10, -1);
                    if (m_externalHeatSourceTemperature != receivedExternalHeatSourceTemperature) {
                        m_externalHeatSourceTemperature = receivedExternalHeatSourceTemperature;
                        emit externalHeatSourceTemperatureChanged(m_externalHeatSourceTemperature);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"External heat source temperature\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"External heat source temperature\" registers from" << hostAddress().toString() << errorString();
    }
}

void StiebelEltronModbusConnection::updateHeatingEnergy()
{
    // Update registers from Heating energy
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"Heating energy\" register:" << 3501 << "size:" << 2;
    QModbusReply *reply = readHeatingEnergy();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"Heating energy\" register" << 3501 << "size:" << 2 << values;
                    quint32 receivedHeatingEnergy = ModbusDataUtils::convertToUInt32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_heatingEnergy != receivedHeatingEnergy) {
                        m_heatingEnergy = receivedHeatingEnergy;
                        emit heatingEnergyChanged(m_heatingEnergy);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"Heating energy\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"Heating energy\" registers from" << hostAddress().toString() << errorString();
    }
}

void StiebelEltronModbusConnection::updateHotWaterEnergy()
{
    // Update registers from Hot water energy
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"Hot water energy\" register:" << 3504 << "size:" << 2;
    QModbusReply *reply = readHotWaterEnergy();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"Hot water energy\" register" << 3504 << "size:" << 2 << values;
                    quint32 receivedHotWaterEnergy = ModbusDataUtils::convertToUInt32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_hotWaterEnergy != receivedHotWaterEnergy) {
                        m_hotWaterEnergy = receivedHotWaterEnergy;
                        emit hotWaterEnergyChanged(m_hotWaterEnergy);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"Hot water energy\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"Hot water energy\" registers from" << hostAddress().toString() << errorString();
    }
}

void StiebelEltronModbusConnection::updateConsumedEnergyHeating()
{
    // Update registers from Consumed energy heating
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"Consumed energy heating\" register:" << 3511 << "size:" << 2;
    QModbusReply *reply = readConsumedEnergyHeating();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"Consumed energy heating\" register" << 3511 << "size:" << 2 << values;
                    quint32 receivedConsumedEnergyHeating = ModbusDataUtils::convertToUInt32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_consumedEnergyHeating != receivedConsumedEnergyHeating) {
                        m_consumedEnergyHeating = receivedConsumedEnergyHeating;
                        emit consumedEnergyHeatingChanged(m_consumedEnergyHeating);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"Consumed energy heating\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"Consumed energy heating\" registers from" << hostAddress().toString() << errorString();
    }
}

void StiebelEltronModbusConnection::updateConsumedEnergyHotWater()
{
    // Update registers from Consumed energy hot water
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"Consumed energy hot water\" register:" << 3514 << "size:" << 2;
    QModbusReply *reply = readConsumedEnergyHotWater();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"Consumed energy hot water\" register" << 3514 << "size:" << 2 << values;
                    quint32 receivedConsumedEnergyHotWater = ModbusDataUtils::convertToUInt32(values, ModbusDataUtils::ByteOrderBigEndian);
                    if (m_consumedEnergyHotWater != receivedConsumedEnergyHotWater) {
                        m_consumedEnergyHotWater = receivedConsumedEnergyHotWater;
                        emit consumedEnergyHotWaterChanged(m_consumedEnergyHotWater);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"Consumed energy hot water\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"Consumed energy hot water\" registers from" << hostAddress().toString() << errorString();
    }
}

void StiebelEltronModbusConnection::updateOperatingMode()
{
    // Update registers from Operating mode
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"Operating mode\" register:" << 1500 << "size:" << 1;
    QModbusReply *reply = readOperatingMode();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"Operating mode\" register" << 1500 << "size:" << 1 << values;
                    OperatingMode receivedOperatingMode = static_cast<OperatingMode>(ModbusDataUtils::convertToUInt16(values));
                    if (m_operatingMode != receivedOperatingMode) {
                        m_operatingMode = receivedOperatingMode;
                        emit operatingModeChanged(m_operatingMode);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"Operating mode\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"Operating mode\" registers from" << hostAddress().toString() << errorString();
    }
}

void StiebelEltronModbusConnection::updateSystemStatus()
{
    // Update registers from System status
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"System status\" register:" << 2500 << "size:" << 1;
    QModbusReply *reply = readSystemStatus();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"System status\" register" << 2500 << "size:" << 1 << values;
                    quint16 receivedSystemStatus = ModbusDataUtils::convertToUInt16(values);
                    if (m_systemStatus != receivedSystemStatus) {
                        m_systemStatus = receivedSystemStatus;
                        emit systemStatusChanged(m_systemStatus);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"System status\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"System status\" registers from" << hostAddress().toString() << errorString();
    }
}

void StiebelEltronModbusConnection::updateSgReadyStateRO()
{
    // Update registers from Smart grid status
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"Smart grid status\" register:" << 5000 << "size:" << 1;
    QModbusReply *reply = readSgReadyStateRO();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"Smart grid status\" register" << 5000 << "size:" << 1 << values;
                    quint16 receivedSgReadyStateRO = ModbusDataUtils::convertToUInt16(values);
                    if (m_sgReadyStateRO != receivedSgReadyStateRO) {
                        m_sgReadyStateRO = receivedSgReadyStateRO;
                        emit sgReadyStateROChanged(m_sgReadyStateRO);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"Smart grid status\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"Smart grid status\" registers from" << hostAddress().toString() << errorString();
    }
}

void StiebelEltronModbusConnection::updateSgReadyActive()
{
    // Update registers from SG ready active
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"SG ready active\" register:" << 4000 << "size:" << 1;
    QModbusReply *reply = readSgReadyActive();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"SG ready active\" register" << 4000 << "size:" << 1 << values;
                    quint16 receivedSgReadyActive = ModbusDataUtils::convertToUInt16(values);
                    if (m_sgReadyActive != receivedSgReadyActive) {
                        m_sgReadyActive = receivedSgReadyActive;
                        emit sgReadyActiveChanged(m_sgReadyActive);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"SG ready active\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"SG ready active\" registers from" << hostAddress().toString() << errorString();
    }
}

void StiebelEltronModbusConnection::updateSgReadyState()
{
    // Update registers from SG Ready mode
    qCDebug(dcStiebelEltronModbusConnection()) << "--> Read \"SG Ready mode\" register:" << 4001 << "size:" << 2;
    QModbusReply *reply = readSgReadyState();
    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [this, reply](){
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    const QVector<quint16> values = unit.values();
                    qCDebug(dcStiebelEltronModbusConnection()) << "<-- Response from \"SG Ready mode\" register" << 4001 << "size:" << 2 << values;
                    SmartGridState receivedSgReadyState = static_cast<SmartGridState>(ModbusDataUtils::convertToUInt32(values, ModbusDataUtils::ByteOrderBigEndian));
                    if (m_sgReadyState != receivedSgReadyState) {
                        m_sgReadyState = receivedSgReadyState;
                        emit sgReadyStateChanged(m_sgReadyState);
                    }
                }
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                qCWarning(dcStiebelEltronModbusConnection()) << "Modbus reply error occurred while updating \"SG Ready mode\" registers from" << hostAddress().toString() << error << reply->errorString();
                emit reply->finished(); // To make sure it will be deleted
            });
        } else {
            delete reply; // Broadcast reply returns immediatly
        }
    } else {
        qCWarning(dcStiebelEltronModbusConnection()) << "Error occurred while reading \"SG Ready mode\" registers from" << hostAddress().toString() << errorString();
    }
}

QModbusReply *StiebelEltronModbusConnection::readOutdoorTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 506, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *StiebelEltronModbusConnection::readFlowTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 514, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *StiebelEltronModbusConnection::readHotWaterTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 521, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *StiebelEltronModbusConnection::readHotGasTemperature1()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 543, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *StiebelEltronModbusConnection::readHotGasTemperature2()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 550, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *StiebelEltronModbusConnection::readSourceTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 562, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *StiebelEltronModbusConnection::readRoomTemperatureFEK()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 502, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *StiebelEltronModbusConnection::readReturnTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 515, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *StiebelEltronModbusConnection::readSolarCollectorTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 527, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *StiebelEltronModbusConnection::readSolarStorageTankTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 528, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *StiebelEltronModbusConnection::readStorageTankTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 517, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *StiebelEltronModbusConnection::readExternalHeatSourceTemperature()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 530, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *StiebelEltronModbusConnection::readHeatingEnergy()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 3501, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *StiebelEltronModbusConnection::readHotWaterEnergy()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 3504, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *StiebelEltronModbusConnection::readConsumedEnergyHeating()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 3511, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *StiebelEltronModbusConnection::readConsumedEnergyHotWater()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 3514, 2);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *StiebelEltronModbusConnection::readOperatingMode()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 1500, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *StiebelEltronModbusConnection::readSystemStatus()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 2500, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *StiebelEltronModbusConnection::readSgReadyStateRO()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, 5000, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *StiebelEltronModbusConnection::readSgReadyActive()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 4000, 1);
    return sendReadRequest(request, m_slaveId);
}

QModbusReply *StiebelEltronModbusConnection::readSgReadyState()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, 4001, 2);
    return sendReadRequest(request, m_slaveId);
}

void StiebelEltronModbusConnection::verifyInitFinished()
{
    if (m_pendingInitReplies.isEmpty()) {
        qCDebug(dcStiebelEltronModbusConnection()) << "Initialization finished of StiebelEltronModbusConnection" << hostAddress().toString();
        emit initializationFinished();
    }
}

QDebug operator<<(QDebug debug, StiebelEltronModbusConnection *stiebelEltronModbusConnection)
{
    debug.nospace().noquote() << "StiebelEltronModbusConnection(" << stiebelEltronModbusConnection->hostAddress().toString() << ":" << stiebelEltronModbusConnection->port() << ")" << "\n";
    debug.nospace().noquote() << "    - Outdoor temperature:" << stiebelEltronModbusConnection->outdoorTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Flow temperature:" << stiebelEltronModbusConnection->flowTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Hot water temperature:" << stiebelEltronModbusConnection->hotWaterTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Hot gas temperature HP 1:" << stiebelEltronModbusConnection->hotGasTemperature1() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Hot gas temperature HP 2:" << stiebelEltronModbusConnection->hotGasTemperature2() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Source temperature:" << stiebelEltronModbusConnection->SourceTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Room temperature FEK:" << stiebelEltronModbusConnection->roomTemperatureFEK() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Return temperature:" << stiebelEltronModbusConnection->returnTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Solar collector temperature:" << stiebelEltronModbusConnection->solarCollectorTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Solar storage tank temperature:" << stiebelEltronModbusConnection->solarStorageTankTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Storage tank temperature:" << stiebelEltronModbusConnection->storageTankTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - External heat source temperature:" << stiebelEltronModbusConnection->externalHeatSourceTemperature() << " [°C]" << "\n";
    debug.nospace().noquote() << "    - Heating energy:" << stiebelEltronModbusConnection->heatingEnergy() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Hot water energy:" << stiebelEltronModbusConnection->hotWaterEnergy() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Consumed energy heating:" << stiebelEltronModbusConnection->consumedEnergyHeating() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Consumed energy hot water:" << stiebelEltronModbusConnection->consumedEnergyHotWater() << " [kWh]" << "\n";
    debug.nospace().noquote() << "    - Operating mode:" << stiebelEltronModbusConnection->operatingMode() << "\n";
    debug.nospace().noquote() << "    - System status:" << stiebelEltronModbusConnection->systemStatus() << "\n";
    debug.nospace().noquote() << "    - Smart grid status:" << stiebelEltronModbusConnection->sgReadyStateRO() << "\n";
    debug.nospace().noquote() << "    - SG ready active:" << stiebelEltronModbusConnection->sgReadyActive() << "\n";
    debug.nospace().noquote() << "    - SG Ready mode:" << stiebelEltronModbusConnection->sgReadyState() << "\n";
    return debug.quote().space();
}

