#include "huaweifusionsolar.h"
#include "extern-plugininfo.h"

HuaweiFusionSolar::HuaweiFusionSolar(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent) :
    HuaweiModbusTcpConnection(hostAddress, port, slaveId, parent)
{

}

void HuaweiFusionSolar::initialize()
{
    // No init registers defined. Nothing to be done and we are finished.
    emit initializationFinished();
}

void HuaweiFusionSolar::update()
{
    // Make sure there is not an update still running
    if (!m_registersQueue.isEmpty())
        return;

    // Add the requests
    m_registersQueue.enqueue(HuaweiModbusTcpConnection::RegisterInverterDCPower);
    m_registersQueue.enqueue(HuaweiModbusTcpConnection::RegisterInverterActivePower);
    m_registersQueue.enqueue(HuaweiModbusTcpConnection::RegisterLunaBattery1Power);
    m_registersQueue.enqueue(HuaweiModbusTcpConnection::RegisterLunaBattery1Soc);
    m_registersQueue.enqueue(HuaweiModbusTcpConnection::RegisterPowerMeterActivePower);
    m_registersQueue.enqueue(HuaweiModbusTcpConnection::RegisterLunaBattery2Soc);
    m_registersQueue.enqueue(HuaweiModbusTcpConnection::RegisterLunaBattery2Power);

    // Note: since huawei can only process one request at the time, we need to queue the requests

    readNextRegister();
}

void HuaweiFusionSolar::readNextRegister()
{
    // Check if currently a reply is pending
    if (m_currentRegisterRequest >= 0)
        return;

    switch (m_currentRegisterRequest) {
    case HuaweiModbusTcpConnection::RegisterInverterDCPower:
        // Update registers from Inverter DC power
        qCDebug(dcHuawei()) << "--> Read \"Inverter DC power\" register:" << 32064 << "size:" << 2;
        QModbusReply *reply = readInverterDCPower();
        if (reply) {
            if (!reply->isFinished()) {
                connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
                connect(reply, &QModbusReply::finished, this, [this, reply](){
                    if (reply->error() == QModbusDevice::NoError) {
                        const QModbusDataUnit unit = reply->result();
                        const QVector<quint16> values = unit.values();
                        qCDebug(dcHuawei()) << "<-- Response from \"Inverter DC power\" register" << 32064 << "size:" << 2 << values;
                        float receivedInverterDCPower = ModbusDataUtils::convertToInt32(values, ModbusDataUtils::ByteOrderBigEndian) * 1.0 * pow(10, -3);
                        if (m_inverterDCPower != receivedInverterDCPower) {
                            m_inverterDCPower = receivedInverterDCPower;
                            emit inverterDCPowerChanged(m_inverterDCPower);
                        }
                    }
                });

                connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){
                    qCWarning(dcHuawei()) << "Modbus reply error occurred while updating \"Inverter DC power\" registers from" << hostAddress().toString() << error << reply->errorString();
                    emit reply->finished(); // To make sure it will be deleted
                });
            } else {
                delete reply; // Broadcast reply returns immediatly
            }
        } else {
            qCWarning(dcHuawei()) << "Error occurred while reading \"Inverter DC power\" registers from" << hostAddress().toString() << errorString();
        }


        break;
    }
}
