// SPDX-License-Identifier: GPL-3.0-or-later

#include "abbterrartudiscovery.h"
#include "abbterrautils.h"
#include "extern-plugininfo.h"

AbbTerraRtuDiscovery::AbbTerraRtuDiscovery(ModbusRtuHardwareResource *modbusRtuResource, QObject *parent)
    : QObject(parent),
      m_modbusRtuResource(modbusRtuResource)
{
}

void AbbTerraRtuDiscovery::startDiscovery()
{
    qCInfo(dcAbbTerra()) << "Discovery: Searching for ABB Terra AC chargers on Modbus RTU...";

    m_candidateMasters.clear();
    m_results.clear();
    m_masterIndex = 0;
    m_slaveId = 1;

    foreach (ModbusRtuMaster *master, m_modbusRtuResource->modbusRtuMasters()) {
        if (master->connected()) {
            m_candidateMasters.append(master);
        }
    }

    if (m_candidateMasters.isEmpty()) {
        qCWarning(dcAbbTerra()) << "No connected Modbus RTU master available for ABB Terra AC discovery.";
        emit discoveryFinished(false);
        return;
    }

    scanNext();
}

QList<AbbTerraRtuDiscovery::Result> AbbTerraRtuDiscovery::results() const
{
    return m_results;
}

void AbbTerraRtuDiscovery::scanNext()
{
    if (m_masterIndex >= m_candidateMasters.count()) {
        emit discoveryFinished(true);
        return;
    }

    if (m_slaveId > 247) {
        m_masterIndex++;
        m_slaveId = 1;
        scanNext();
        return;
    }

    ModbusRtuMaster *master = m_candidateMasters.at(m_masterIndex);
    const quint16 currentSlaveId = m_slaveId++;

    ModbusRtuReply *reply = master->readHoldingRegister(currentSlaveId, 0x4000, 8);
    connect(reply, &ModbusRtuReply::finished, this, [this, master, currentSlaveId, reply]() {
        if (reply->error() == ModbusRtuReply::NoError) {
            const AbbTerraUtils::DeviceInfo deviceInfo = AbbTerraUtils::deviceInfoFromRegisters(reply->result());
            if (deviceInfo.valid) {
                Result result;
                result.modbusRtuMasterId = master->modbusUuid();
                result.slaveId = currentSlaveId;
                result.serialNumber = deviceInfo.serialNumber;
                result.productName = deviceInfo.productName;
                result.firmwareVersion = deviceInfo.firmwareVersion;
                m_results.append(result);
            }
        }

        scanNext();
    });
}
