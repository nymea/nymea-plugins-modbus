#include "sunspecdevice.h"

SunSpecDevice::SunSpecDevice(SunSpec *connection, SunSpec::ModelId modelId, int modbusStartRegister, QObject *parent):
    QObject(parent),
    m_connection(connection),
    m_modelId(modelId),
    m_modbusStartRegister(modbusStartRegister)
{

}

SunSpec *SunSpecDevice::connection() const
{
    return m_connection;
}

SunSpec::ModelId SunSpecDevice::modelId() const
{
    return m_modelId;
}

int SunSpecDevice::modbusStartRegister() const
{
    return m_modbusStartRegister;
}
