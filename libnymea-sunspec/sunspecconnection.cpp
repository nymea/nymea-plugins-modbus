#include "sunspecconnection.h"
#include "models/sunspecmodelfactory.h"

SunSpecConnection::SunSpecConnection(const QHostAddress &hostAddress, uint port, uint slaveId, QObject *parent) :
    QObject(parent),
    m_hostAddress(hostAddress),
    m_port(port),
    m_slaveId(slaveId)
{
    qCDebug(dcSunSpec()) << "Creating connection for" << QString("%1:%2").arg(m_hostAddress.toString()).arg(m_port);
    m_modbusTcpClient = new QModbusTcpClient(this);
    m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, m_port);
    m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, m_hostAddress.toString());
    m_modbusTcpClient->setTimeout(2000);
    m_modbusTcpClient->setNumberOfRetries(3);


    m_reconnectTimer.setInterval(10000);
    m_reconnectTimer.setSingleShot(false);
    connect(&m_reconnectTimer, &QTimer::timeout, this, [=](){
        if (connected()) {
            m_reconnectTimer.stop();
            return;
        }

        if (!m_modbusTcpClient->connectDevice()) {
            m_reconnectTimer.start();
        }

    });

    connect(m_modbusTcpClient, &QModbusTcpClient::stateChanged, this, [this](QModbusDevice::State state){
        bool connected = (state == QModbusDevice::ConnectedState);
        bool disconnected = (state == QModbusDevice::UnconnectedState);

        if (connected) {
            m_reconnectTimer.stop();
            emit connectedChanged(true);
            return;
        }

        if (disconnected) {
            // Try to reconnect in 10 seconds
            m_reconnectTimer.start();
            emit connectedChanged(false);
            return;
        }
    });
}

QModbusTcpClient *SunSpecConnection::modbusTcpClient() const
{
    return m_modbusTcpClient;
}

QHostAddress SunSpecConnection::hostAddress() const
{
    return m_hostAddress;
}

void SunSpecConnection::setHostAddress(const QHostAddress &hostAddress)
{
    m_hostAddress = hostAddress;
    m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, m_hostAddress.toString());

    // TODO: reconnect if different
}

uint SunSpecConnection::port() const
{
    return m_port;
}

void SunSpecConnection::setPort(uint port)
{
    m_port = port;
    m_modbusTcpClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, m_port);

    // TODO: reconnect if different

}

uint SunSpecConnection::slaveId() const
{
    return m_slaveId;
}

void SunSpecConnection::setSlaveId(uint slaveId)
{
    m_slaveId = slaveId;
}

int SunSpecConnection::timeout() const
{
    return m_modbusTcpClient->timeout();
}

void SunSpecConnection::setTimeout(int milliSeconds)
{
    m_modbusTcpClient->setTimeout(milliSeconds);
}

uint SunSpecConnection::numberOfRetries() const
{
    return m_modbusTcpClient->numberOfRetries();
}

void SunSpecConnection::setNumberOfRetries(uint retries)
{
    m_modbusTcpClient->setNumberOfRetries(retries);
}

bool SunSpecConnection::connected() const
{
    return m_modbusTcpClient->state() == QModbusDevice::ConnectedState;
}

bool SunSpecConnection::connectDevice()
{
    qCDebug(dcSunSpec()) << "Connecting" << this << "...";
    return m_modbusTcpClient->connectDevice();
}

void SunSpecConnection::disconnectDevice()
{
    qCDebug(dcSunSpec()) << "Disconnecting from" << this;
    m_modbusTcpClient->disconnectDevice();
}

bool SunSpecConnection::startSunSpecDiscovery()
{
    // Verify connection state
    if (!connected()) {
        qCWarning(dcSunSpec()) << "Could not start SunSpec model discovery." << "The" << this << "is not connected.";
        return false;
    }

    // Create base register queue
    m_baseRegisterQueue.clear();
    m_baseRegisterQueue.enqueue(40000);
    m_baseRegisterQueue.enqueue(50000);
    m_baseRegisterQueue.enqueue(0);

    qCDebug(dcSunSpec()) << "Starting SunSpec discovery on" << this;
    return scanSunspecBaseRegister(m_baseRegisterQueue.dequeue());
}

bool SunSpecConnection::scanSunspecBaseRegister(quint16 baseRegister)
{
    qCDebug(dcSunSpec()) << "--> Scan for 'SunS' on" << this << baseRegister;

    // Read 2 register and check if it contains 'SunS' 0x53756e53
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, baseRegister, 2);
    QModbusReply *reply = m_modbusTcpClient->sendReadRequest(request, m_slaveId);
    if (!reply) {
        qCWarning(dcSunSpec()) << "Failed to send SunSpec base register request on" << this;
        return false;
    }

    if (reply->isFinished()) {
        delete reply; // broadcast replies return immediately
        return false;
    }

    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
    connect(reply, &QModbusReply::finished, this, [reply, baseRegister, this] {
        if (reply->error() == QModbusDevice::NoError) {
            const QModbusDataUnit unit = reply->result();
            quint32 registerContent = (unit.value(0) << 16 | unit.value(1));
            if (registerContent == 0x53756e53) {
                //Well-known value. Uniquely identifies this as a SunSpec Modbus model
                qCDebug(dcSunSpec()) << "Found 'SunS' identifier on register on" << this << baseRegister;
                m_baseRegisterQueue.clear();

                m_baseRegister = baseRegister;
                emit sunspecBaseRegisterFound(m_baseRegister);

                // Found successfull "SunS", do not continue with the alternative registers
                qCDebug(dcSunSpec()) << "Start scanning for SunSpec models on" << this << "using SunSpec base register" << m_baseRegister;
                scanModelsOnBaseRegister();
            } else {
                qCWarning(dcSunSpec()) << "Got reply on base register" << baseRegister << ", but value did not match the sunspec code 'SunS' 0x53756e53";
                scanNextSunspecBaseRegister();
            }
        } else {
            qCDebug(dcSunSpec()) << "Base register" << baseRegister << "not found on" << this;
            scanNextSunspecBaseRegister();
        }
    });

    return true;
}

void SunSpecConnection::scanNextSunspecBaseRegister()
{
    if (m_baseRegisterQueue.isEmpty()) {
        qCDebug(dcSunSpec()) << "Finished with SunSpec discovery. No SunSpec register has been found on" << this;
        emit sunSpecDiscoveryFinished(false);
        return;
    }

    scanSunspecBaseRegister(m_baseRegisterQueue.dequeue());
}

void SunSpecConnection::scanModelsOnBaseRegister(quint16 offset)
{
    qCDebug(dcSunSpec()) << "Reading SunSpec models header" << this << "using SunSpec base register" << m_baseRegister << "offset:" << offset;
    quint16 startRegisterAddress = m_baseRegister + offset;
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, startRegisterAddress, 2);
    QModbusReply *reply = m_modbusTcpClient->sendReadRequest(request, m_slaveId);

    if (!reply) {
        qCWarning(dcSunSpec()) << "Failed to read SunSpec model header on" << this;
        // TODO: finish scan with error
        return;
    }

    if (reply->isFinished()) {
        delete reply; // broadcast replies return immediately
        return;
    }

    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
    connect(reply, &QModbusReply::finished, this, [this, reply, offset] {
        if (reply->error() == QModbusDevice::NoError) {

            const QModbusDataUnit unit = reply->result();
            quint16 modelId  = unit.value(0);
            int modelLength = unit.value(1);

            // Check if we reached the end of models
            if (modelId == 0xFFFF) {
                qCDebug(dcSunSpec()) << "Scan for SunSpec models on" << this << m_baseRegister << "finished successfully";
                //emit sunspecModelSearchFinished(m_modelList);

                SunSpecModelFactory factory;
                foreach (quint16 modelId, m_modelDiscoveryResult.keys()) {
                    SunSpecModel *model = factory.createModel(this, modelId, m_modelDiscoveryResult[modelId]);
                    if (model) {
                        qCDebug(dcSunSpec()) << "-->" << model;
                    } else {
                        qCWarning(dcSunSpec()) << "--> Could not create model object for model ID" << modelId;
                    }
                }

                return;
            }

            qCDebug(dcSunSpec()) << "Discovered SunSpec model" << modelId << "with length" << modelLength;
            m_modelDiscoveryResult.insert(modelId, modelLength);

            // Scan next model block, current offset + 2 header bytes + model length
            scanModelsOnBaseRegister(offset + 2 + modelLength);
        } else {
            qCWarning(dcSunSpec()) << "Error occured while reading model header from" << this << "using offset" << offset << m_modbusTcpClient->errorString();
            // TODO: finish scan with error
        }
    });
}

QDebug operator<<(QDebug debug, SunSpecConnection *connection)
{
    debug.nospace().noquote() << "SunSpecConnection(" << connection->hostAddress().toString() << ":" << connection->port() << ", Slave ID: " << connection->slaveId() << ")";
    return debug.space().quote();
}
