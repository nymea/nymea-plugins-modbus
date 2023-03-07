#include "ciondiscovery.h"

#include"extern-plugininfo.h"

CionDiscovery::CionDiscovery(ModbusRtuHardwareResource *modbusRtuResource, QObject *parent)
    : QObject{parent},
      m_modbusRtuResource{modbusRtuResource}
{

}

void CionDiscovery::startDiscovery()
{
    qCInfo(dcSchrack()) << "Discovery: Searching for Schrack i-CHARGE wallboxes on modbus RTU...";

    QList<ModbusRtuMaster*> candidateMasters;
    foreach (ModbusRtuMaster *master, m_modbusRtuResource->modbusRtuMasters()) {
        if (master->baudrate() == 57600 && master->dataBits() == 8 && master->stopBits() == 1 && master->parity() == QSerialPort::NoParity) {
            candidateMasters.append(master);
        }
    }

    if (candidateMasters.isEmpty()) {
        qCWarning(dcSchrack()) << "No usable modbus RTU master found.";
        emit discoveryFinished(false);
        return;
    }

    foreach (ModbusRtuMaster *master, candidateMasters) {
        if (master->connected()) {
            tryConnect(master, 1);
        } else {
            qCWarning(dcSchrack()) << "Modbus RTU master" << master->modbusUuid().toString() << "is not connected.";
        }
    }
}

QList<CionDiscovery::Result> CionDiscovery::discoveryResults() const
{
    return m_discoveryResults;
}

void CionDiscovery::tryConnect(ModbusRtuMaster *master, quint16 slaveId)
{
    qCDebug(dcSchrack()) << "Scanning modbus RTU master" << master->modbusUuid() << "Slave ID:" << slaveId;

    ModbusRtuReply *reply = master->readInputRegister(slaveId, 4);
    connect(reply, &ModbusRtuReply::finished, this, [=](){
        qCDebug(dcSchrack()) << "Test reply finished!" << reply->error() << reply->result();
        if (reply->error() == ModbusRtuReply::NoError && reply->result().length() > 0) {
            quint16 version = reply->result().first();
            if (version >= 0x0100) {
                qCDebug(dcSchrack()) << QString("Version is 0x%1").arg(version, 0, 16);
                Result result {master->modbusUuid(), version, slaveId};
                m_discoveryResults.append(result);
            } else {
                qCDebug(dcAmperfied()) << "Version must be at least 1.0.0 (0x0100)";
            }
        }
        if (slaveId < 20) {
            tryConnect(master, slaveId+1);
        } else {
            emit discoveryFinished(true);
        }
    });
}
