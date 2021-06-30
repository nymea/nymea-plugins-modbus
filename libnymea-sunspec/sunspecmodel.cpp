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

void SunSpecModel::setInitializedFinished()
{
    if (!m_initialized) {
        m_initTimer.stop();
        m_initialized = true;
        emit initFinished(true);
    }
}
