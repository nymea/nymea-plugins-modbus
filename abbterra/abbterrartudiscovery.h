// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ABBTERRARTUDISCOVERY_H
#define ABBTERRARTUDISCOVERY_H

#include <QObject>

#include <hardware/modbus/modbusrtuhardwareresource.h>

class AbbTerraRtuDiscovery : public QObject
{
    Q_OBJECT
public:
    struct Result {
        QUuid modbusRtuMasterId;
        quint16 slaveId;
        QString serialNumber;
        QString productName;
        QString firmwareVersion;
    };

    explicit AbbTerraRtuDiscovery(ModbusRtuHardwareResource *modbusRtuResource, QObject *parent = nullptr);

    void startDiscovery();
    QList<Result> results() const;

signals:
    void discoveryFinished(bool modbusRtuMasterAvailable);

private:
    void scanNext();

private:
    ModbusRtuHardwareResource *m_modbusRtuResource = nullptr;
    QList<ModbusRtuMaster *> m_candidateMasters;
    QList<Result> m_results;
    int m_masterIndex = 0;
    quint16 m_slaveId = 1;
};

#endif // ABBTERRARTUDISCOVERY_H
