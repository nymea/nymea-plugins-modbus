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

#include "amtronecu.h"
#include "extern-plugininfo.h"

Q_DECLARE_LOGGING_CATEGORY(dcAmtronECUModbusTcpConnection)

AmtronECU::AmtronECU(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent)
    : AmtronECUModbusTcpConnection{hostAddress, port, slaveId, parent}
{

}

AmtronECU::AmtronECU(ModbusTcpMaster *modbusTcpMaster, quint16 slaveId, QObject *parent)
    : AmtronECUModbusTcpConnection{modbusTcpMaster, slaveId, parent}
{

}

bool AmtronECU::initialize()
{
    if (!m_reachable) {
        qCWarning(dcAmtronECUModbusTcpConnection()) << "Tried to initialize but the device is not to be reachable.";
        return false;
    }

    if (m_initObject) {
        qCWarning(dcAmtronECUModbusTcpConnection()) << "Tried to initialize but the init process is already running.";
        return false;
    }

    m_initializing = true;

    // Parent object for the init process
    m_initObject = new QObject(this);

    QModbusReply *reply = nullptr;


    // This is a 2 Step initialization. First read the firmware version, if the firmware version is smaller than 5.22 we are done
    // otherwise read also the model since we use that for detecting the exact manufacturer.

    // Some useres have to stay on 5.12 due to calibration law which is not available on 5.22

    // Read Firmware version
    qCDebug(dcAmtronECUModbusTcpConnection()) << "--> Read init \"Firmware version\" register:" << 100 << "size:" << 2;
    reply = readFirmwareVersion();
    if (!reply) {
        qCWarning(dcAmtronECUModbusTcpConnection()) << "Error occurred while reading \"Firmware version\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
        finishInitialization(false);
        return false;
    }

    if (reply->isFinished()) {
        reply->deleteLater(); // Broadcast reply returns immediatly
        return false;
    }

    m_pendingInitReplies.append(reply);
    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
    connect(reply, &QModbusReply::finished, m_initObject, [this, reply](){
        handleModbusError(reply->error());
        m_pendingInitReplies.removeAll(reply);
        if (reply->error() != QModbusDevice::NoError) {
            finishInitialization(false);
            return;
        }

        const QModbusDataUnit unit = reply->result();
        qCDebug(dcAmtronECUModbusTcpConnection()) << "<-- Response from init \"Firmware version\" register" << 100 << "size:" << 2 << unit.values();
        if (unit.values().size() == 2) {
            processFirmwareVersionRegisterValues(unit.values());
        } else {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Reading from \"Firmware version\" registers" << 100 << "size:" << 2 << "returned different size than requested. Ignoring incomplete data" << unit.values();
            finishInitialization(false);
            return;
        }


        // Make sure this is the correct firmware version format (0x-- 0x2e 0x-- 0x--)
        if ((m_firmwareVersion & 0x00ff0000) != 0x002e0000) {
            qCWarning(dcMennekes()) << "Invalid firmware version format. Could not determin the firmware format from uint32" << QByteArray::fromHex(QByteArray::number(m_firmwareVersion, 16));
            m_detectedVersion = VersionUnknown;
            finishInitialization(false);
            return;
        }

        // If the firmware version is smaller than 5.22 (Ascii of the uint32 firmware version property),
        // we are done since the model registers have been introduced in Version 5.22
        if (m_firmwareVersion < 0x352e3232) {
            qCDebug(dcMennekes()) << "Detected firmware version" << QByteArray::fromHex(QByteArray::number(m_firmwareVersion, 16)) << ". Some features are only available from version >= 5.22.";
            m_detectedVersion = VersionOld;
        } else {
            qCDebug(dcMennekes()) << "Detected firmware version" <<  QByteArray::fromHex(QByteArray::number(m_firmwareVersion, 16)) << "All features available.";
            m_detectedVersion = VersionNew;

            // Read Device model (>= 5.22)
            qCDebug(dcAmtronECUModbusTcpConnection()) << "--> Read init \"Device model (>= 5.22)\" register:" << 142 << "size:" << 10;
            QModbusReply *reply = readModel();
            if (!reply) {
                qCWarning(dcAmtronECUModbusTcpConnection()) << "Error occurred while reading \"Device model (>= 5.22)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
                finishInitialization(false);
                return;
            }

            if (reply->isFinished()) {
                reply->deleteLater(); // Broadcast reply returns immediatly
                return;
            }

            m_pendingInitReplies.append(reply);
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, m_initObject, [this, reply](){
                handleModbusError(reply->error());
                m_pendingInitReplies.removeAll(reply);
                if (reply->error() != QModbusDevice::NoError) {
                    finishInitialization(false);
                    return;
                }

                const QModbusDataUnit unit = reply->result();
                qCDebug(dcAmtronECUModbusTcpConnection()) << "<-- Response from init \"Device model (>= 5.22)\" register" << 142 << "size:" << 10 << unit.values();
                if (unit.values().size() == 10) {
                    processModelRegisterValues(unit.values());
                } else {
                    qCWarning(dcAmtronECUModbusTcpConnection()) << "Reading from \"Device model (>= 5.22)\" registers" << 142 << "size:" << 10 << "returned different size than requested. Ignoring incomplete data" << unit.values();
                }
                verifyInitFinished();
            });

            connect(reply, &QModbusReply::errorOccurred, m_initObject, [this, reply] (QModbusDevice::Error error){
                QModbusResponse response = reply->rawResult();
                if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
                    qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while reading \"Device model (>= 5.22)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
                } else {
                    qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while reading \"Device model (>= 5.22)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
                }
            });
        }

        verifyInitFinished();
    });

    connect(reply, &QModbusReply::errorOccurred, m_initObject, [this, reply] (QModbusDevice::Error error){
        QModbusResponse response = reply->rawResult();
        if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while reading \"Firmware version\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
        } else {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while reading \"Firmware version\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
        }
    });

    return true;
}

bool AmtronECU::update()
{
    if (!reachable()) {
        qCWarning(dcAmtronECUModbusTcpConnection()) << "Tried to update but the device is not to be reachable.";
        return false;
    }

    if (!m_modbusTcpMaster->connected())
        return false;

    if (!m_pendingUpdateReplies.isEmpty()) {
        qCDebug(dcAmtronECUModbusTcpConnection()) << "Tried to update but there are still some update replies pending. Waiting for them to be finished...";
        return true;
    }

    // First update common registers
    // Then update registers only available for > 5.22

    QModbusReply *reply = nullptr;

    // Read CP signal state
    qCDebug(dcAmtronECUModbusTcpConnection()) << "--> Read \"CP signal state\" register:" << 122 << "size:" << 1;
    reply = readCpSignalState();
    if (!reply) {
        qCWarning(dcAmtronECUModbusTcpConnection()) << "Error occurred while reading \"CP signal state\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
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
        qCDebug(dcAmtronECUModbusTcpConnection()) << "<-- Response from \"CP signal state\" register" << 122 << "size:" << 1 << unit.values();
        if (unit.values().size() == 1) {
            processCpSignalStateRegisterValues(unit.values());
        } else {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Reading from \"CP signal state\" registers" << 122 << "size:" << 1 << "returned different size than requested. Ignoring incomplete data" << unit.values();
        }
        verifyUpdateFinished();
    });

    connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
        QModbusResponse response = reply->rawResult();
        if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while reading \"CP signal state\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
        } else {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while reading \"CP signal state\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
        }
    });


    // Read Signalled current to EV
    qCDebug(dcAmtronECUModbusTcpConnection()) << "--> Read \"Signalled current to EV\" register:" << 706 << "size:" << 1;
    reply = readSignalledCurrent();
    if (!reply) {
        qCWarning(dcAmtronECUModbusTcpConnection()) << "Error occurred while reading \"Signalled current to EV\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
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
        qCDebug(dcAmtronECUModbusTcpConnection()) << "<-- Response from \"Signalled current to EV\" register" << 706 << "size:" << 1 << unit.values();
        if (unit.values().size() == 1) {
            processSignalledCurrentRegisterValues(unit.values());
        } else {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Reading from \"Signalled current to EV\" registers" << 706 << "size:" << 1 << "returned different size than requested. Ignoring incomplete data" << unit.values();
        }
        verifyUpdateFinished();
    });

    connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
        QModbusResponse response = reply->rawResult();
        if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while reading \"Signalled current to EV\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
        } else {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while reading \"Signalled current to EV\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
        }
    });

    // Read Minimum current limit
    qCDebug(dcAmtronECUModbusTcpConnection()) << "--> Read \"Minimum current limit\" register:" << 712 << "size:" << 1;
    reply = readMinCurrentLimit();
    if (!reply) {
        qCWarning(dcAmtronECUModbusTcpConnection()) << "Error occurred while reading \"Minimum current limit\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
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
        qCDebug(dcAmtronECUModbusTcpConnection()) << "<-- Response from \"Minimum current limit\" register" << 712 << "size:" << 1 << unit.values();
        if (unit.values().size() == 1) {
            processMinCurrentLimitRegisterValues(unit.values());
        } else {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Reading from \"Minimum current limit\" registers" << 712 << "size:" << 1 << "returned different size than requested. Ignoring incomplete data" << unit.values();
        }
        verifyUpdateFinished();
    });

    connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
        QModbusResponse response = reply->rawResult();
        if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while reading \"Minimum current limit\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
        } else {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while reading \"Minimum current limit\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
        }
    });


    // Read consumptions
    reply = readBlockConsumptions();
    qCDebug(dcAmtronECUModbusTcpConnection()) << "--> Read block \"consumptions\" registers from:" << 200 << "size:" << 18;
    if (!reply) {
        qCWarning(dcAmtronECUModbusTcpConnection()) << "Error occurred while reading block \"consumptions\" registers";
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
        const QVector<quint16> blockValues = unit.values();
        qCDebug(dcAmtronECUModbusTcpConnection()) << "<-- Response from reading block \"consumptions\" register" << 200 << "size:" << 18 << blockValues;
        if (blockValues.size() == 18) {
            processMeterEnergyL1RegisterValues(blockValues.mid(0, 2));
            processMeterEnergyL2RegisterValues(blockValues.mid(2, 2));
            processMeterEnergyL3RegisterValues(blockValues.mid(4, 2));
            processMeterPowerL1RegisterValues(blockValues.mid(6, 2));
            processMeterPowerL2RegisterValues(blockValues.mid(8, 2));
            processMeterPowerL3RegisterValues(blockValues.mid(10, 2));
            processMeterCurrentL1RegisterValues(blockValues.mid(12, 2));
            processMeterCurrentL2RegisterValues(blockValues.mid(14, 2));
            processMeterCurrentL3RegisterValues(blockValues.mid(16, 2));
        } else {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Reading from \"consumptions\" block registers" << 200 << "size:" << 18 << "returned different size than requested. Ignoring incomplete data" << blockValues;
        }
        verifyUpdateFinished();
    });

    connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error){
        QModbusResponse response = reply->rawResult();
        if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while updating block \"consumptions\" registers" << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
        } else {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while updating block \"consumptions\" registers" << error << reply->errorString();
        }
    });


    // Read Charge Point availability ( Version < 5.22: 0 available, 1 unavailable | Version >= 5.22 1 available, 0 unavailable )
    qCDebug(dcAmtronECUModbusTcpConnection()) << "--> Read \"Charge Point availability ( Version < 5.22: 0 available, 1 unavailable | Version >= 5.22 1 available, 0 unavailable )\" register:" << 124 << "size:" << 1;
    reply = readCpAvailability();
    if (!reply) {
        qCWarning(dcAmtronECUModbusTcpConnection()) << "Error occurred while reading \"Charge Point availability ( Version < 5.22: 0 available, 1 unavailable | Version >= 5.22 1 available, 0 unavailable )\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
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
        qCDebug(dcAmtronECUModbusTcpConnection()) << "<-- Response from \"Charge Point availability ( Version < 5.22: 0 available, 1 unavailable | Version >= 5.22 1 available, 0 unavailable )\" register" << 124 << "size:" << 1 << unit.values();
        if (unit.values().size() == 1) {
            processCpAvailabilityRegisterValues(unit.values());
        } else {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Reading from \"Charge Point availability ( Version < 5.22: 0 available, 1 unavailable | Version >= 5.22 1 available, 0 unavailable )\" registers" << 124 << "size:" << 1 << "returned different size than requested. Ignoring incomplete data" << unit.values();
        }
        verifyUpdateFinished();
    });

    connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
        QModbusResponse response = reply->rawResult();
        if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while reading \"Charge Point availability ( Version < 5.22: 0 available, 1 unavailable | Version >= 5.22 1 available, 0 unavailable )\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
        } else {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while reading \"Charge Point availability ( Version < 5.22: 0 available, 1 unavailable | Version >= 5.22 1 available, 0 unavailable )\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
        }
    });



    // Read HEMS current limit
    qCDebug(dcAmtronECUModbusTcpConnection()) << "--> Read \"HEMS current limit\" register:" << 1000 << "size:" << 1;
    reply = readHemsCurrentLimit();
    if (!reply) {
        qCWarning(dcAmtronECUModbusTcpConnection()) << "Error occurred while reading \"HEMS current limit\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
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
        qCDebug(dcAmtronECUModbusTcpConnection()) << "<-- Response from \"HEMS current limit\" register" << 1000 << "size:" << 1 << unit.values();
        if (unit.values().size() == 1) {
            processHemsCurrentLimitRegisterValues(unit.values());
        } else {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Reading from \"HEMS current limit\" registers" << 1000 << "size:" << 1 << "returned different size than requested. Ignoring incomplete data" << unit.values();
        }
        verifyUpdateFinished();
    });

    connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
        QModbusResponse response = reply->rawResult();
        if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while reading \"HEMS current limit\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
        } else {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while reading \"HEMS current limit\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
        }
    });



    // Only available on >= 5.22
    if (m_detectedVersion == VersionNew) {

        // Read Maximum current limit (>= 5.22)
        qCDebug(dcAmtronECUModbusTcpConnection()) << "--> Read \"Maximum current limit (>= 5.22)\" register:" << 715 << "size:" << 1;
        reply = readMaxCurrentLimit();
        if (!reply) {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Error occurred while reading \"Maximum current limit (>= 5.22)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
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
            qCDebug(dcAmtronECUModbusTcpConnection()) << "<-- Response from \"Maximum current limit (>= 5.22)\" register" << 715 << "size:" << 1 << unit.values();
            if (unit.values().size() == 1) {
                processMaxCurrentLimitRegisterValues(unit.values());
            } else {
                qCWarning(dcAmtronECUModbusTcpConnection()) << "Reading from \"Maximum current limit (>= 5.22)\" registers" << 715 << "size:" << 1 << "returned different size than requested. Ignoring incomplete data" << unit.values();
            }
            verifyUpdateFinished();
        });

        connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
            QModbusResponse response = reply->rawResult();
            if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
                qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while reading \"Maximum current limit (>= 5.22)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
            } else {
                qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while reading \"Maximum current limit (>= 5.22)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
            }
        });

        // Read Charged energy for current session (>= 5.22)
        qCDebug(dcAmtronECUModbusTcpConnection()) << "--> Read \"Charged energy for current session (>= 5.22)\" register:" << 716 << "size:" << 2;
        reply = readChargedEnergy();
        if (!reply) {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Error occurred while reading \"Charged energy for current session (>= 5.22)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
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
            qCDebug(dcAmtronECUModbusTcpConnection()) << "<-- Response from \"Charged energy for current session (>= 5.22)\" register" << 716 << "size:" << 2 << unit.values();
            if (unit.values().size() == 2) {
                processChargedEnergyRegisterValues(unit.values());
            } else {
                qCWarning(dcAmtronECUModbusTcpConnection()) << "Reading from \"Charged energy for current session (>= 5.22)\" registers" << 716 << "size:" << 2 << "returned different size than requested. Ignoring incomplete data" << unit.values();
            }
            verifyUpdateFinished();
        });

        connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
            QModbusResponse response = reply->rawResult();
            if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
                qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while reading \"Charged energy for current session (>= 5.22)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
            } else {
                qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while reading \"Charged energy for current session (>= 5.22)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
            }
        });


        // Read consumptionsTotals
        reply = readBlockConsumptionsTotals();
        qCDebug(dcAmtronECUModbusTcpConnection()) << "--> Read block \"consumptionsTotals\" registers from:" << 218 << "size:" << 10;
        if (!reply) {
            qCWarning(dcAmtronECUModbusTcpConnection()) << "Error occurred while reading block \"consumptionsTotals\" registers";
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
            const QVector<quint16> blockValues = unit.values();
            qCDebug(dcAmtronECUModbusTcpConnection()) << "<-- Response from reading block \"consumptionsTotals\" register" << 218 << "size:" << 10 << blockValues;
            if (blockValues.size() == 10) {
                processMeterTotoalEnergyRegisterValues(blockValues.mid(0, 2));
                processMeterTotalPowerRegisterValues(blockValues.mid(2, 2));
                processMeterVoltageL1RegisterValues(blockValues.mid(4, 2));
                processMeterVoltageL2RegisterValues(blockValues.mid(6, 2));
                processMeterVoltageL3RegisterValues(blockValues.mid(8, 2));
            } else {
                qCWarning(dcAmtronECUModbusTcpConnection()) << "Reading from \"consumptionsTotals\" block registers" << 218 << "size:" << 10 << "returned different size than requested. Ignoring incomplete data" << blockValues;
            }
            verifyUpdateFinished();
        });

        connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error){
            QModbusResponse response = reply->rawResult();
            if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
                qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while updating block \"consumptionsTotals\" registers" << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
            } else {
                qCWarning(dcAmtronECUModbusTcpConnection()) << "Modbus reply error occurred while updating block \"consumptionsTotals\" registers" << error << reply->errorString();
            }
        });
    }
    return true;
}

AmtronECU::Version AmtronECU::detectedVersion() const
{
    return m_detectedVersion;
}

QDebug operator<<(QDebug debug, AmtronECU *amtronECU)
{
    debug.nospace().noquote() << "AmtronECU(" << amtronECU->modbusTcpMaster()->hostAddress().toString() << ":" << amtronECU->modbusTcpMaster()->port() << ")" << "\n";
    debug.nospace().noquote() << "    - holding  100 | Firmware version: " << amtronECU->firmwareVersion() << "\n";
    debug.nospace().noquote() << "    - holding  122 | CP signal state: " << amtronECU->cpSignalState() << "\n";
    debug.nospace().noquote() << "    - holding  124 | Charge Point availability ( Version < 5.22: 0 available, 1 unavailable | Version >= 5.22 1 available, 0 unavailable ): " << amtronECU->cpAvailability() << "\n";
    debug.nospace().noquote() << "    - holding  200 | Meter energy L1: " << amtronECU->meterEnergyL1() << " [Wh]" << "\n";
    debug.nospace().noquote() << "    - holding  202 | Meter energy L2: " << amtronECU->meterEnergyL2() << " [Wh]" << "\n";
    debug.nospace().noquote() << "    - holding  204 | Meter energy L3: " << amtronECU->meterEnergyL3() << " [Wh]" << "\n";
    debug.nospace().noquote() << "    - holding  206 | Meter power L1: " << amtronECU->meterPowerL1() << " [W]" << "\n";
    debug.nospace().noquote() << "    - holding  208 | Meter power L2: " << amtronECU->meterPowerL2() << " [W]" << "\n";
    debug.nospace().noquote() << "    - holding  210 | Meter power L3: " << amtronECU->meterPowerL3() << " [W]" << "\n";
    debug.nospace().noquote() << "    - holding  212 | Meter current L1: " << amtronECU->meterCurrentL1() << " [mA]" << "\n";
    debug.nospace().noquote() << "    - holding  214 | Meter current L2: " << amtronECU->meterCurrentL2() << " [mA]" << "\n";
    debug.nospace().noquote() << "    - holding  216 | Meter current L3: " << amtronECU->meterCurrentL3() << " [mA]" << "\n";
    debug.nospace().noquote() << "    - holding  706 | Signalled current to EV: " << amtronECU->signalledCurrent() << " [A]" << "\n";
    debug.nospace().noquote() << "    - holding  712 | Minimum current limit: " << amtronECU->minCurrentLimit() << " [A]" << "\n";
    debug.nospace().noquote() << "    - holding  1000 | HEMS current limit: " << amtronECU->hemsCurrentLimit() << " [A]" << "\n";

    if (amtronECU->detectedVersion() == AmtronECU::VersionNew) {
        debug.nospace().noquote() << "    - holding  142 | Device model (>= 5.22): " << amtronECU->model() << "\n";
        debug.nospace().noquote() << "    - holding  218 | Meter total energy (>= 5.22): " << amtronECU->meterTotoalEnergy() << " [Wh]" << "\n";
        debug.nospace().noquote() << "    - holding  220 | Meter total power (>= 5.22): " << amtronECU->meterTotalPower() << " [W]" << "\n";
        debug.nospace().noquote() << "    - holding  222 | Meter voltage L1 (>= 5.22): " << amtronECU->meterVoltageL1() << " [V]" << "\n";
        debug.nospace().noquote() << "    - holding  224 | Meter voltage L2 (>= 5.22): " << amtronECU->meterVoltageL2() << " [V]" << "\n";
        debug.nospace().noquote() << "    - holding  226 | Meter voltage L3 (>= 5.22): " << amtronECU->meterVoltageL3() << " [V]" << "\n";
        debug.nospace().noquote() << "    - holding  715 | Maximum current limit (>= 5.22): " << amtronECU->maxCurrentLimit() << " [A]" << "\n";
        debug.nospace().noquote() << "    - holding  716 | Charged energy for current session (>= 5.22): " << amtronECU->chargedEnergy() << " [Wh]" << "\n";
    }
    return debug.quote().space();
}

