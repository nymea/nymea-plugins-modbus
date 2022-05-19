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

#include "sunspecconnection.h"
#include "sunspecmodel.h"
#include "models/sunspeccommonmodel.h"
#include "models/sunspecmodelfactory.h"

Q_LOGGING_CATEGORY(dcSunSpec, "SunSpec")

SunSpecConnection::SunSpecConnection(const QHostAddress &hostAddress, uint port, uint slaveId, QObject *parent) :
    QObject(parent),
    m_hostAddress(hostAddress),
    m_port(port),
    m_slaveId(slaveId)
{
    createConnection();
}

SunSpecConnection::SunSpecConnection(const QHostAddress &hostAddress, uint port, uint slaveId, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    QObject(parent),
    m_hostAddress(hostAddress),
    m_port(port),
    m_slaveId(slaveId),
    m_byteOrder(byteOrder)
{
    createConnection();
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

SunSpecDataPoint::ByteOrder SunSpecConnection::byteOrder() const
{
    return m_byteOrder;
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

bool SunSpecConnection::discoveryRunning() const
{
    return m_discoveryRunning;
}

bool SunSpecConnection::connectDevice()
{
    qCDebug(dcSunSpec()) << "Connecting" << this << "...";
    return m_modbusTcpClient->connectDevice();
}

void SunSpecConnection::disconnectDevice()
{
    qCDebug(dcSunSpec()) << "Disconnecting from" << this << "...";
    m_modbusTcpClient->disconnectDevice();
}

quint16 SunSpecConnection::baseRegister() const
{
    return m_baseRegister;
}

QList<SunSpecModel *> SunSpecConnection::models() const
{
    return m_models;
}

bool SunSpecConnection::startDiscovery()
{
    // Verify connection state
    if (!connected()) {
        qCWarning(dcSunSpec()) << "Could not start SunSpec model discovery." << "The" << this << "is not connected.";
        setDiscoveryRunning(false);
        return false;
    }

    if (m_discoveryRunning) {
        qCDebug(dcSunSpec()) << "Start SunSpec discovery requested but already running on" << this;
        return true;
    }

    // Create base register queue
    m_baseRegisterQueue.clear();
    m_baseRegisterQueue.enqueue(40000);
    m_baseRegisterQueue.enqueue(50000);
    m_baseRegisterQueue.enqueue(0);

    qCDebug(dcSunSpec()) << "Starting SunSpec discovery on" << this;
    m_modelDiscoveryResult.clear();
    setDiscoveryRunning(true);
    if (!scanSunspecBaseRegister(m_baseRegisterQueue.dequeue())) {
        setDiscoveryRunning(false);
        return false;
    }

    return true;
}

void SunSpecConnection::createConnection()
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

void SunSpecConnection::processDiscoveryResult()
{
    qCDebug(dcSunSpec()) << "Creating models from the discovery results...";

    if (!m_uninitializedModels.isEmpty()) {
        qCWarning(dcSunSpec()) << "Having still uninitialized models in the pipeline. This should never happen (please report a bug). Removing unintialized models.";
        qDeleteAll(m_uninitializedModels);
        m_uninitializedModels.clear();
    }

    SunSpecModelFactory factory;
    foreach (ModuleDiscoveryResult result, m_modelDiscoveryResult) {
        SunSpecModel *model = factory.createModel(this, result.modbusStartRegister, result.modelId, result.modelLength, m_byteOrder);
        if (model) {
            if (modelAlreadyAdded(model)) {
                qCDebug(dcSunSpec()) << "Detected an already added model" << model << "and keep the already existing one.";
                model->deleteLater();
            } else {
                if (model->initialized()) {
                    m_models.append(model);
                } else {
                    m_uninitializedModels.append(model);
                }
                qCDebug(dcSunSpec()) << "--> [+]" << model;
            }
        } else {
            qCWarning(dcSunSpec()) << "--> [!] Could not create model object for model ID" << result.modelId << "because the model does not get handled yet.";
        }
    }

    // If no models found to initialize, we are done
    if (m_uninitializedModels.isEmpty()) {
        qCDebug(dcSunSpec()) << "No models to initialize. Discovery finished successfully.";
        setDiscoveryRunning(false);
        emit discoveryFinished(true);
        return;
    }

    // Init each model and finish when all uninitialized models are finished.
    foreach (SunSpecModel *model, m_uninitializedModels) {
        connect(model, &SunSpecModel::initFinished, this, [this, model](bool success){
            m_uninitializedModels.removeAll(model);
            if (success) {
                m_models.append(model);
            } else {
                qCWarning(dcSunSpec()) << "Failed to initialize" << model << "Deleting the object.";
                model->deleteLater();
            }

            if (m_uninitializedModels.isEmpty()) {
                // Sort the models according to their modbus start address in order to set the common model for each model
                std::sort(m_models.begin(), m_models.end(), [](const SunSpecModel* a, const SunSpecModel* b) -> bool {
                    return a->modbusStartRegister() < b->modbusStartRegister();
                });

                // Set common model information to each model (until the next common model shows up)
                qCDebug(dcSunSpec()) << "Sorted model list:";
                SunSpecCommonModel *currentCommonModel = nullptr;
                SunSpecModel::CommonModelInfo commonModelInfo;
                for (int i = 0; i < m_models.count(); i++) {
                    SunSpecModel *model = m_models.at(i);
                    qCDebug(dcSunSpec()) << "-->" << model;
                    if (model->modelId() == SunSpecModelFactory::ModelIdCommon) {
                        SunSpecCommonModel *commonModel = qobject_cast<SunSpecCommonModel *>(model);
                        if (commonModel != currentCommonModel) {
                            currentCommonModel = commonModel;
                            commonModelInfo.manufacturerName = currentCommonModel->manufacturer();
                            commonModelInfo.modelName = currentCommonModel->model();
                            commonModelInfo.serialNumber = currentCommonModel->serialNumber();
                            commonModelInfo.versionString = currentCommonModel->version();
                        }
                        continue;
                    }

                    if (currentCommonModel && model->modelId() != SunSpecModelFactory::ModelIdCommon) {
                        model->m_commonModelInfo = commonModelInfo;
                    }
                }

                qCDebug(dcSunSpec()) << "All models initialized. Discovery finished successfully.";
                setDiscoveryRunning(false);
                emit discoveryFinished(true);
            }
        });

        model->init();
    }
}

void SunSpecConnection::setDiscoveryRunning(bool discoveryRunning)
{
    if (m_discoveryRunning == discoveryRunning)
        return;

    m_discoveryRunning = discoveryRunning;
    emit discoveryRunningChanged(m_discoveryRunning);
}

bool SunSpecConnection::modelAlreadyAdded(SunSpecModel *model) const
{
    foreach (SunSpecModel *m, m_models) {
        if (*m == *model) {
            return true;
        }
    }

    return false;
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
        reply->deleteLater(); // broadcast replies return immediately
        return false;
    }

    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
    connect(reply, &QModbusReply::finished, this, [reply, baseRegister, this] {
        if (reply->error() == QModbusDevice::NoError) {
            const QModbusDataUnit unit = reply->result();
            quint32 registerContent = (unit.value(0) << 16 | unit.value(1));
            if (registerContent == 0x53756e53) {
                //Well-known value. Uniquely identifies this as a SunSpec Modbus model
                qCDebug(dcSunSpec()) << "Found 'SunS' identifier register" << baseRegister << "on" << this;
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
        setDiscoveryRunning(false);
        emit discoveryFinished(false);
        return;
    }

    scanSunspecBaseRegister(m_baseRegisterQueue.dequeue());
}

void SunSpecConnection::scanModelsOnBaseRegister(quint16 offset)
{
    quint16 startRegisterAddress = m_baseRegister + offset;
    qCDebug(dcSunSpec()) << "Reading SunSpec model header" << this << "using SunSpec base register" << m_baseRegister << "offset:" << offset << "=" << startRegisterAddress;
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, startRegisterAddress, 2);
    QModbusReply *reply = m_modbusTcpClient->sendReadRequest(request, m_slaveId);

    if (!reply) {
        qCWarning(dcSunSpec()) << "Failed to read SunSpec model header on" << this;
        // TODO: finish scan with error
        return;
    }

    if (reply->isFinished()) {
        reply->deleteLater(); // broadcast replies return immediately
        return;
    }

    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
    connect(reply, &QModbusReply::finished, this, [this, reply, offset, startRegisterAddress] {
        if (reply->error() == QModbusDevice::NoError) {

            const QModbusDataUnit unit = reply->result();
            quint16 modbusStartRegister = unit.startAddress();
            quint16 modelId  = unit.value(0);
            int modelLength = unit.value(1);

            // Check if we reached the end of models
            if (modelId == 0xFFFF) {
                qCDebug(dcSunSpec()) << "Scan for SunSpec models on" << this << m_baseRegister << "finished successfully";
                processDiscoveryResult();
                return;
            }

            qCDebug(dcSunSpec()) << "Discovered SunSpec model on" << this << "[" << startRegisterAddress + 2 << "-" << startRegisterAddress + 2 + modelLength << "]" << "(base: " << m_baseRegister << "offset:" << offset << "length:" << modelLength << ") | Model ID:" << modelId << static_cast<SunSpecModelFactory::ModelId>(modelId);
            ModuleDiscoveryResult result;
            result.modbusStartRegister = modbusStartRegister;
            result.modelId = modelId;
            result.modelLength = modelLength;
            m_modelDiscoveryResult.append(result);

            // Scan next model block, current offset + 2 header bytes + model length
            scanModelsOnBaseRegister(offset + 2 + modelLength);
        } else {
            qCWarning(dcSunSpec()) << "Error occured while reading model header from" << this << "using offset" << offset << m_modbusTcpClient->errorString();
            if (!m_modelDiscoveryResult.isEmpty()) {
                qCWarning(dcSunSpec()) << "Error occured but already discovered" << m_modelDiscoveryResult.count() << "models. Continue with the discovered models, but the discovery may be incomplete due to header reading errors.";
                qCDebug(dcSunSpec()) << "Scan for SunSpec models on" << this << m_baseRegister << "finished successfully";
                processDiscoveryResult();
            } else {
                setDiscoveryRunning(false);
                emit discoveryFinished(false);
            }
        }
    });
}

QDebug operator<<(QDebug debug, SunSpecConnection *connection)
{
    debug.nospace().noquote() << "SunSpecConnection(" << connection->hostAddress().toString() << ":" << connection->port() << ", Slave ID: " << connection->slaveId() << ")";
    return debug.space().quote();
}
