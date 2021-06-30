#include "sunspecmodel.h"

SunSpecModel::SunSpecModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :
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

SunSpec *SunSpecModel::connection() const
{
    return m_connection;
}

QVector<quint16> SunSpecModel::supportedModelIds() const
{
    return m_supportedModelIds;
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

void SunSpecModel::init()
{
    m_initialized = false;
    readBlockData();
    m_initTimer.start();
}

void SunSpecModel::readBlockData()
{
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister, m_modelLength);
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

                // Fill the data points
                foreach (const QString &dataPointName, m_dataPoints.keys()) {
                    m_dataPoints[dataPointName].setRawData(m_blockData.mid(m_dataPoints[dataPointName].addressOffset(), m_dataPoints[dataPointName].size()));
                }

                setInitializedFinished();
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
