#include "ciondiscovery.h"

#include"extern-plugininfo.h"

#include <modbusdatautils.h>


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

    ModbusRtuReply *reply = master->readHoldingRegister(slaveId, 832, 16);
    connect(reply, &ModbusRtuReply::finished, this, [=](){

        if (reply->error() == ModbusRtuReply::NoError) {

            QString firmwareVersion = ModbusDataUtils::convertToString(reply->result());
            qCDebug(dcSchrack()) << "Test reply finished!" << reply->error() << firmwareVersion;

            // Version numbers seem to be wild west... We can't really understand what's in there...
            // So let's assume this is a schrack if reading alone succeeded and it is a valid string and 18 to 32 chars long...
            // Examples of how this looks like:
            // EBE 1.2: "V1.2    15.02.2021"
            // ICC:     "003090056-01          20220913"
            QRegExp re = QRegExp("[A-Z0-9\\.- ]{18,32}");
            if (re.exactMatch(firmwareVersion)) {
                Result result {master->modbusUuid(), firmwareVersion, slaveId};
                m_discoveryResults.append(result);
            }
        }

        if (slaveId < 10) {
            tryConnect(master, slaveId+1);
        } else {
            emit discoveryFinished(true);
        }
    });
}
