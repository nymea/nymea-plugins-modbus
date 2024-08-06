#include "pantabox.h"
#include "loggingcategories.h"

NYMEA_LOGGING_CATEGORY(dcPantabox, "Pantabox")

Pantabox::Pantabox(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent) :
    PantaboxModbusTcpConnection(hostAddress, port, slaveId, parent)
{
    // Note: sometimes right after the discovery / setup the check fails the first time due to server busy error,
    // this is a very slow or busy device since it returns quiet often that error. Don't faile with the first busy error...
    setCheckReachableRetries(3);
}

bool Pantabox::initialize() {

    if (!m_reachable) {
            qCWarning(dcPantabox()) << "Tried to initialize but the device is not to be reachable.";
            return false;
        }

        if (m_initObject) {
            qCWarning(dcPantabox()) << "Tried to initialize but the init process is already running.";
            return false;
        }

        m_initializing = true;

        // Parent object for the init process
        m_initObject = new QObject(this);

        QModbusReply *reply = nullptr;

        // Read Serial number (hex)
        qCDebug(dcPantabox()) << "--> Read init \"Serial number (hex)\" register:" << 256 << "size:" << 2;
        reply = readSerialNumber();
        if (!reply) {
            qCWarning(dcPantabox()) << "Error occurred while reading \"Serial number (hex)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
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
            processSerialNumberRegisterValues(unit.values());
            verifyInitFinished();
        });

        connect(reply, &QModbusReply::errorOccurred, m_initObject, [this, reply] (QModbusDevice::Error error){
            QModbusResponse response = reply->rawResult();
            if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
                qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Serial number (hex)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
            } else {
                qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Serial number (hex)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
            }
        });

        // Read ModbusTCP version
        qCDebug(dcPantabox()) << "--> Read init \"ModbusTCP version\" register:" << 258 << "size:" << 2;
        reply = readModbusTcpVersion();
        if (!reply) {
            qCWarning(dcPantabox()) << "Error occurred while reading \"ModbusTCP version\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
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
            processModbusTcpVersionRegisterValues(unit.values());
            verifyInitFinished();
        });

        connect(reply, &QModbusReply::errorOccurred, m_initObject, [this, reply] (QModbusDevice::Error error){
            QModbusResponse response = reply->rawResult();
            if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
                qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"ModbusTCP version\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
            } else {
                qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"ModbusTCP version\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
            }
        });

        // Following Modbus registers are available since ModbusTcpVersion 1.1
        if (m_modbusTcpVersion < 257)
            return true;

        // Read Name of vendor
        qCDebug(dcPantabox()) << "--> Read init \"Name of vendor\" register:" << 260 << "size:" << 2;
        reply = readVendorName();
        if (!reply) {
            qCWarning(dcPantabox()) << "Error occurred while reading \"Name of vendor\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
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
            processVendorNameRegisterValues(unit.values());
            verifyInitFinished();
        });

        connect(reply, &QModbusReply::errorOccurred, m_initObject, [this, reply] (QModbusDevice::Error error){
            QModbusResponse response = reply->rawResult();
            if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
                qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Name of vendor\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
            } else {
                qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Name of vendor\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
            }
        });

        // Read Name of product
        qCDebug(dcPantabox()) << "--> Read init \"Name of product\" register:" << 262 << "size:" << 4;
        reply = readProductName();
        if (!reply) {
            qCWarning(dcPantabox()) << "Error occurred while reading \"Name of product\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
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
            processProductNameRegisterValues(unit.values());
            verifyInitFinished();
        });

        connect(reply, &QModbusReply::errorOccurred, m_initObject, [this, reply] (QModbusDevice::Error error){
            QModbusResponse response = reply->rawResult();
            if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
                qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Name of product\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
            } else {
                qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Name of product\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
            }
        });

        // Read Firmware version
        qCDebug(dcPantabox()) << "--> Read init \"Firmware version\" register:" << 266 << "size:" << 16;
        reply = readFirmwareVersion();
        if (!reply) {
            qCWarning(dcPantabox()) << "Error occurred while reading \"Firmware version\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
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
            processFirmwareVersionRegisterValues(unit.values());
            verifyInitFinished();
        });

        connect(reply, &QModbusReply::errorOccurred, m_initObject, [this, reply] (QModbusDevice::Error error){
            QModbusResponse response = reply->rawResult();
            if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {
                qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Firmware version\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
            } else {
                qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Firmware version\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
            }
        });
        return true;
}

bool Pantabox::update() {
    if (!m_modbusTcpMaster->connected())
            return false;

    if (!m_pendingUpdateReplies.isEmpty()) {
        qCDebug(dcPantabox()) << "Tried to update but there are still some update replies pending. Waiting for them to be finished...";
        return true;
    }

    QModbusReply *reply = nullptr;

    // Read Charging state
    qCDebug(dcPantabox()) << "--> Read \"Charging state\" register:" << 512 << "size:" << 1;
    reply = readChargingState();
    if (!reply) {
        qCWarning(dcPantabox()) << "Error occurred while reading \"Charging state\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
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
            qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Charging state\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
        } else {
            qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Charging state\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
        }
    });

    // Read Current charging power
    qCDebug(dcPantabox()) << "--> Read \"Current charging power\" register:" << 513 << "size:" << 2;
    reply = readCurrentPower();
    if (!reply) {
        qCWarning(dcPantabox()) << "Error occurred while reading \"Current charging power\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
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
            qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Current charging power\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
        } else {
            qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Current charging power\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
        }
    });

    // Read Charged energy
    qCDebug(dcPantabox()) << "--> Read \"Charged energy\" register:" << 515 << "size:" << 2;
    reply = readChargedEnergy();
    if (!reply) {
        qCWarning(dcPantabox()) << "Error occurred while reading \"Charged energy\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
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
            qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Charged energy\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
        } else {
            qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Charged energy\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
        }
    });

    // Read Maximal possible charging current (adapter)
    qCDebug(dcPantabox()) << "--> Read \"Maximal possible charging current (adapter)\" register:" << 517 << "size:" << 1;
    reply = readMaxPossibleChargingCurrent();
    if (!reply) {
        qCWarning(dcPantabox()) << "Error occurred while reading \"Maximal possible charging current (adapter)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
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
            qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Maximal possible charging current (adapter)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
        } else {
            qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Maximal possible charging current (adapter)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
        }
    });

    // Read Actual charging current
    qCDebug(dcPantabox()) << "--> Read \"Actual charging current\" register:" << 518 << "size:" << 1;
    reply = readChargingCurrent();
    if (!reply) {
        qCWarning(dcPantabox()) << "Error occurred while reading \"Actual charging current\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
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
            qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Actual charging current\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
        } else {
            qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Actual charging current\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
        }
    });

    // The Modbus register for absolute charged energy is available since Modbusversion 1.2
    if (m_modbusTcpVersion < 258) {

        // Read Absolute charged energy
        qCDebug(dcPantabox()) << "--> Read \"Absolute charged energy\" register:" << 519 << "size:" << 2;
        reply = readAbsoluteEnergy();
        if (!reply) {
            qCWarning(dcPantabox()) << "Error occurred while reading \"Absolute charged energy\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
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
                qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Absolute charged energy\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
            } else {
                qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Absolute charged energy\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
            }
        });
    }

    // Read Charging enabled (1) / disabled (0)
    qCDebug(dcPantabox()) << "--> Read \"Charging enabled (1) / disabled (0)\" register:" << 768 << "size:" << 1;
    reply = readChargingEnabled();
    if (!reply) {
        qCWarning(dcPantabox()) << "Error occurred while reading \"Charging enabled (1) / disabled (0)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
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
            qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Charging enabled (1) / disabled (0)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
        } else {
            qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Charging enabled (1) / disabled (0)\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
        }
    });

    // Read Max charging current
    qCDebug(dcPantabox()) << "--> Read \"Max charging current\" register:" << 769 << "size:" << 1;
    reply = readMaxChargingCurrent();
    if (!reply) {
        qCWarning(dcPantabox()) << "Error occurred while reading \"Max charging current\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();
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
            qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Max charging current\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());
        } else {
            qCWarning(dcPantabox()) << "Modbus reply error occurred while reading \"Max charging current\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();
        }
    });
    return true;
}
