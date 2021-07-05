#include "sunspecmodel.h"
#include "sunspecconnection.h"

SunSpecModel::SunSpecModel(SunSpecConnection *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
    QObject(parent),
    m_connection(connection),
    m_modelId(modelId),
    m_modelLength(modelLength),
    m_modbusStartRegister(modbusStartRegister)
{
    m_initTimer.setSingleShot(true);
    m_initTimer.setInterval(10000);
    connect(&m_initTimer, &QTimer::timeout, this, [this](){
        if (!m_initialized) {
            emit initFinished(false);
        }
    });
}

SunSpecConnection *SunSpecModel::connection() const
{
    return m_connection;
}

quint16 SunSpecModel::modelId() const
{
    return m_modelId;
}

quint16 SunSpecModel::modelLength() const
{
    return m_modelLength;
}

quint16 SunSpecModel::modbusStartRegister() const
{
    return m_modbusStartRegister;
}

QVector<quint16> SunSpecModel::blockData() const
{
    return m_blockData;
}

void SunSpecModel::init()
{
    m_initialized = false;
    readBlockData();
    m_initTimer.start();
}

void SunSpecModel::readBlockData()
{
    // Read the block data, start register + 2 header reisters (id, length)
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + 2, m_modelLength);
    if (QModbusReply *reply = m_connection->modbusTcpClient()->sendReadRequest(request, m_connection->slaveId())) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, this, [=]() {
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcSunSpec()) << name() << description() << "Read block data response error:" << reply->error();
                    return;
                }

                const QModbusDataUnit unit = reply->result();
                qCDebug(dcSunSpec()) << name() << description() << "Received block data" << m_modbusStartRegister << unit.values().count();
                m_blockData = unit.values();
                emit blockDataChanged(m_blockData);

                if (m_blockData.count() != m_modelLength) {
                    qCWarning(dcSunSpec()) << "Received invalid block data count from read block data request. Model lenght:" << m_modelLength << "Response block count:" << m_blockData.count();
                    return;
                }

                // Fill the data points
                foreach (const QString &dataPointName, m_dataPoints.keys()) {
                    m_dataPoints[dataPointName].setRawData(m_blockData.mid(m_dataPoints[dataPointName].addressOffset(), m_dataPoints[dataPointName].size()));
                }

                // Fill the private member data using the data points
                processBlockData();

                // Make sure initialized gets called
                setInitializedFinished();

                // Inform about the new block data
                emit blockUpdated();
            });

            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error) {
                qCWarning(dcSunSpec())  << name() << description() << "Modbus reply while reading block data. Error:" << error;
                emit reply->finished(); // To make sure it will be deleted
            });

        } else {
            qCWarning(dcSunSpec()) << "Read block data error: " << m_connection->modbusTcpClient()->errorString();
            delete reply; // broadcast replies return immediately
            return;
        }
    } else {
        qCWarning(dcSunSpec()) << "Read block data error: " << m_connection->modbusTcpClient()->errorString();
        return;
    }
}

void SunSpecModel::setInitializedFinished()
{
    if (!m_initialized) {
        m_initTimer.stop();
        m_initialized = true;
        emit initFinished(true);
    }
}

QDebug operator<<(QDebug debug, SunSpecModel *model)
{
    debug.nospace().noquote() << "SunSpecModel(" << model->modelId() << ", " << model->name() << ", " << model->label() << ")";
    return debug.space().quote();
}
