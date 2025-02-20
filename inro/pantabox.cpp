/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2024, nymea GmbH
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

#include "pantabox.h"
#include "pantaboxmodbustcpconnection.cpp"

Pantabox::Pantabox(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent) :
    PantaboxModbusTcpConnection(hostAddress, port, slaveId, parent)
{
    modbusTcpMaster()->setNumberOfRetries(3);
    modbusTcpMaster()->setTimeout(500);
}

QString Pantabox::modbusVersionToString(quint32 value)
{
    quint16 modbusVersionMinor = (value >> 16) & 0xffff;
    quint16 modbusVersionMajor = value & 0xffff;
    return QString("%1.%2").arg(modbusVersionMajor).arg(modbusVersionMinor);
}

bool Pantabox::update() {
    if (!m_modbusTcpMaster->connected())
        return false;

    if (!m_pendingUpdateReplies.isEmpty()) {
        qCDebug(dcPantaboxModbusTcpConnection()) << "Tried to update but there are still some update replies pending. Waiting for them to be finished...";
        return true;
    }

    QModbusReply *reply = nullptr;

    // Read Charging state
    qCDebug(dcPantaboxModbusTcpConnection()) << "--> Read \"Charging state\" register:" << 512 << "size:" << 1;
    reply = readChargingState();
    if (!reply) {
        qCWarning(dcPantaboxModbusTcpConnection()) << "Error occurred while reading \"Charging state\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
        return false;
    }

    if (reply->isFinished()) {
        reply->deleteLater(); // Broadcast reply returns immediatly
        return false;
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
    connect(reply, &QModbusReply::finished, this, [this, reply](){
        m_pendingUpdateReplies.removeAll(reply);
        handleModbusError(reply->error());
        if (reply->error() != QModbusDevice::NoError) {
            verifyUpdateFinished();
            return;
        }

        const QModbusDataUnit unit = reply->result();
        processChargingStateRegisterValues(unit.values());
        verifyUpdateFinished();
    });

    connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
        QModbusResponse response = reply->rawResult();
        if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
            qCWarning(dcPantaboxModbusTcpConnection()) << "Modbus reply error occurred while reading \"Charging state\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
        } else {
            qCWarning(dcPantaboxModbusTcpConnection()) << "Modbus reply error occurred while reading \"Charging state\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
        }
    });

    // Read Current charging power
    qCDebug(dcPantaboxModbusTcpConnection()) << "--> Read \"Current charging power\" register:" << 513 << "size:" << 2;
    reply = readCurrentPower();
    if (!reply) {
        qCWarning(dcPantaboxModbusTcpConnection()) << "Error occurred while reading \"Current charging power\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
        return false;
    }

    if (reply->isFinished()) {
        reply->deleteLater(); // Broadcast reply returns immediatly
        return false;
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
    connect(reply, &QModbusReply::finished, this, [this, reply](){
        m_pendingUpdateReplies.removeAll(reply);
        handleModbusError(reply->error());
        if (reply->error() != QModbusDevice::NoError) {
            verifyUpdateFinished();
            return;
        }

        const QModbusDataUnit unit = reply->result();
        processCurrentPowerRegisterValues(unit.values());
        verifyUpdateFinished();
    });

    connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
        QModbusResponse response = reply->rawResult();
        if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
            qCWarning(dcPantaboxModbusTcpConnection()) << "Modbus reply error occurred while reading \"Current charging power\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
        } else {
            qCWarning(dcPantaboxModbusTcpConnection()) << "Modbus reply error occurred while reading \"Current charging power\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
        }
    });

    // Read Charged energy
    qCDebug(dcPantaboxModbusTcpConnection()) << "--> Read \"Charged energy\" register:" << 515 << "size:" << 2;
    reply = readChargedEnergy();
    if (!reply) {
        qCWarning(dcPantaboxModbusTcpConnection()) << "Error occurred while reading \"Charged energy\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
        return false;
    }

    if (reply->isFinished()) {
        reply->deleteLater(); // Broadcast reply returns immediatly
        return false;
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
    connect(reply, &QModbusReply::finished, this, [this, reply](){
        m_pendingUpdateReplies.removeAll(reply);
        handleModbusError(reply->error());
        if (reply->error() != QModbusDevice::NoError) {
            verifyUpdateFinished();
            return;
        }

        const QModbusDataUnit unit = reply->result();
        processChargedEnergyRegisterValues(unit.values());
        verifyUpdateFinished();
    });

    connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
        QModbusResponse response = reply->rawResult();
        if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
            qCWarning(dcPantaboxModbusTcpConnection()) << "Modbus reply error occurred while reading \"Charged energy\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
        } else {
            qCWarning(dcPantaboxModbusTcpConnection()) << "Modbus reply error occurred while reading \"Charged energy\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
        }
    });

    // Read Maximal possible charging current (adapter)
    qCDebug(dcPantaboxModbusTcpConnection()) << "--> Read \"Maximal possible charging current (adapter)\" register:" << 517 << "size:" << 1;
    reply = readMaxPossibleChargingCurrent();
    if (!reply) {
        qCWarning(dcPantaboxModbusTcpConnection()) << "Error occurred while reading \"Maximal possible charging current (adapter)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
        return false;
    }

    if (reply->isFinished()) {
        reply->deleteLater(); // Broadcast reply returns immediatly
        return false;
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
    connect(reply, &QModbusReply::finished, this, [this, reply](){
        m_pendingUpdateReplies.removeAll(reply);
        handleModbusError(reply->error());
        if (reply->error() != QModbusDevice::NoError) {
            verifyUpdateFinished();
            return;
        }

        const QModbusDataUnit unit = reply->result();
        processMaxPossibleChargingCurrentRegisterValues(unit.values());
        verifyUpdateFinished();
    });

    connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
        QModbusResponse response = reply->rawResult();
        if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
            qCWarning(dcPantaboxModbusTcpConnection()) << "Modbus reply error occurred while reading \"Maximal possible charging current (adapter)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
        } else {
            qCWarning(dcPantaboxModbusTcpConnection()) << "Modbus reply error occurred while reading \"Maximal possible charging current (adapter)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
        }
    });

    // Read Actual charging current
    qCDebug(dcPantaboxModbusTcpConnection()) << "--> Read \"Actual charging current\" register:" << 518 << "size:" << 1;
    reply = readChargingCurrent();
    if (!reply) {
        qCWarning(dcPantaboxModbusTcpConnection()) << "Error occurred while reading \"Actual charging current\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
        return false;
    }

    if (reply->isFinished()) {
        reply->deleteLater(); // Broadcast reply returns immediatly
        return false;
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
    connect(reply, &QModbusReply::finished, this, [this, reply](){
        m_pendingUpdateReplies.removeAll(reply);
        handleModbusError(reply->error());
        if (reply->error() != QModbusDevice::NoError) {
            verifyUpdateFinished();
            return;
        }

        const QModbusDataUnit unit = reply->result();
        processChargingCurrentRegisterValues(unit.values());
        verifyUpdateFinished();
    });

    connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
        QModbusResponse response = reply->rawResult();
        if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
            qCWarning(dcPantaboxModbusTcpConnection()) << "Modbus reply error occurred while reading \"Actual charging current\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
        } else {
            qCWarning(dcPantaboxModbusTcpConnection()) << "Modbus reply error occurred while reading \"Actual charging current\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
        }
    });

    // Read Charging enabled (1) / disabled (0)
    qCDebug(dcPantaboxModbusTcpConnection()) << "--> Read \"Charging enabled (1) / disabled (0)\" register:" << 768 << "size:" << 1;
    reply = readChargingEnabled();
    if (!reply) {
        qCWarning(dcPantaboxModbusTcpConnection()) << "Error occurred while reading \"Charging enabled (1) / disabled (0)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
        return false;
    }

    if (reply->isFinished()) {
        reply->deleteLater(); // Broadcast reply returns immediatly
        return false;
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
    connect(reply, &QModbusReply::finished, this, [this, reply](){
        m_pendingUpdateReplies.removeAll(reply);
        handleModbusError(reply->error());
        if (reply->error() != QModbusDevice::NoError) {
            verifyUpdateFinished();
            return;
        }

        const QModbusDataUnit unit = reply->result();
        processChargingEnabledRegisterValues(unit.values());
        verifyUpdateFinished();
    });

    connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
        QModbusResponse response = reply->rawResult();
        if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
            qCWarning(dcPantaboxModbusTcpConnection()) << "Modbus reply error occurred while reading \"Charging enabled (1) / disabled (0)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
        } else {
            qCWarning(dcPantaboxModbusTcpConnection()) << "Modbus reply error occurred while reading \"Charging enabled (1) / disabled (0)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
        }
    });

    // Read Max charging current
    qCDebug(dcPantaboxModbusTcpConnection()) << "--> Read \"Max charging current\" register:" << 769 << "size:" << 1;
    reply = readMaxChargingCurrent();
    if (!reply) {
        qCWarning(dcPantaboxModbusTcpConnection()) << "Error occurred while reading \"Max charging current\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
        return false;
    }

    if (reply->isFinished()) {
        reply->deleteLater(); // Broadcast reply returns immediatly
        return false;
    }

    m_pendingUpdateReplies.append(reply);
    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
    connect(reply, &QModbusReply::finished, this, [this, reply](){
        m_pendingUpdateReplies.removeAll(reply);
        handleModbusError(reply->error());
        if (reply->error() != QModbusDevice::NoError) {
            verifyUpdateFinished();
            return;
        }

        const QModbusDataUnit unit = reply->result();
        processMaxChargingCurrentRegisterValues(unit.values());
        verifyUpdateFinished();
    });

    connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
        QModbusResponse response = reply->rawResult();
        if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
            qCWarning(dcPantaboxModbusTcpConnection()) << "Modbus reply error occurred while reading \"Max charging current\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
        } else {
            qCWarning(dcPantaboxModbusTcpConnection()) << "Modbus reply error occurred while reading \"Max charging current\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
        }
    });


    // Following Modbus registers depend on the modbus TCP protocol version and require compatibility checks

    // Firmware version registers are available since modbus TCP version 1.1 (0x0001 0x0001) 0x10001 = 65537
    if (m_modbusTcpVersion >= 65537) {

        // Read Firmware version
        qCDebug(dcPantaboxModbusTcpConnection()) << "--> Read \"Firmware version\" register:" << 266 << "size:" << 16;
        reply = readFirmwareVersion();
        if (!reply) {
            qCWarning(dcPantaboxModbusTcpConnection()) << "Error occurred while reading \"Firmware version\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
            return false;
        }

        if (reply->isFinished()) {
            reply->deleteLater(); // Broadcast reply returns immediatly
            return false;
        }

        m_pendingUpdateReplies.append(reply);
        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        connect(reply, &QModbusReply::finished, this, [this, reply](){
            m_pendingUpdateReplies.removeAll(reply);
            handleModbusError(reply->error());
            if (reply->error() != QModbusDevice::NoError) {
                verifyUpdateFinished();
                return;
            }

            const QModbusDataUnit unit = reply->result();
            processFirmwareVersionRegisterValues(unit.values());
            verifyUpdateFinished();
        });

        connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
            QModbusResponse response = reply->rawResult();
            if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
                qCWarning(dcPantaboxModbusTcpConnection()) << "Modbus reply error occurred while reading \"Firmware version\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
            } else {
                qCWarning(dcPantaboxModbusTcpConnection()) << "Modbus reply error occurred while reading \"Firmware version\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
            }
        });
    }

    //  Absolute charged energy is available since modbus TCP version 1.2 (0x0001 0x0002) 0x10002 = 65538
    if (m_modbusTcpVersion >= 65538) {

        // Read Absolute charged energy
        qCDebug(dcPantaboxModbusTcpConnection()) << "--> Read \"Absolute charged energy\" register:" << 519 << "size:" << 2;
        reply = readAbsoluteEnergy();
        if (!reply) {
            qCWarning(dcPantaboxModbusTcpConnection()) << "Error occurred while reading \"Absolute charged energy\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
            return false;
        }

        if (reply->isFinished()) {
            reply->deleteLater(); // Broadcast reply returns immediatly
            return false;
        }

        m_pendingUpdateReplies.append(reply);
        connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
        connect(reply, &QModbusReply::finished, this, [this, reply](){
            m_pendingUpdateReplies.removeAll(reply);
            handleModbusError(reply->error());
            if (reply->error() != QModbusDevice::NoError) {
                verifyUpdateFinished();
                return;
            }

            const QModbusDataUnit unit = reply->result();
            processAbsoluteEnergyRegisterValues(unit.values());
            verifyUpdateFinished();
        });

        connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
            QModbusResponse response = reply->rawResult();
            if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
                qCWarning(dcPantaboxModbusTcpConnection()) << "Modbus reply error occurred while reading \"Absolute charged energy\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
            } else {
                qCWarning(dcPantaboxModbusTcpConnection()) << "Modbus reply error occurred while reading \"Absolute charged energy\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
            }
        });
    }

    return true;
}
