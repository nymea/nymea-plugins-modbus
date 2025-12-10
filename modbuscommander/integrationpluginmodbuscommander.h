// SPDX-License-Identifier: GPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of nymea-plugins-modbus.
*
* nymea-plugins-modbus is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* nymea-plugins-modbus is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with nymea-plugins-modbus. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef INTEGRATIONPLUGINMODBUSCOMMANDER_H
#define INTEGRATIONPLUGINMODBUSCOMMANDER_H

#include <plugintimer.h>
#include <integrations/integrationplugin.h>
#include <hardware/modbus/modbusrtumaster.h>

#include <modbustcpmaster.h>

#include <QUuid>
#include <QSerialPort>
#include <QSerialPortInfo>

class IntegrationPluginModbusCommander: public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginmodbuscommander.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    explicit IntegrationPluginModbusCommander();

    void init() override;
    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *info) override;
    void executeAction(ThingActionInfo *info) override;
    void thingRemoved(Thing *thing) override;

private:
    PluginTimer *m_refreshTimer = nullptr;

    QHash<Thing*, ModbusTcpMaster*> m_modbusTCPMasters;
    QHash<Thing *, ModbusRtuMaster *> m_modbusRtuMasters;
    QHash<QUuid, ThingActionInfo*> m_asyncActions;
    QHash<QUuid, Thing*> m_readRequests;

    void readRegister(Thing *thing);
    void writeRegister(Thing *thing, ThingActionInfo *info);

    QHash<ThingClassId, ParamTypeId> m_slaveAddressParamTypeId;
    QHash<ThingClassId, ParamTypeId> m_registerAddressParamTypeId;
    QHash<ThingClassId, StateTypeId> m_connectedStateTypeId;
    QHash<ThingClassId, StateTypeId> m_valueStateTypeId;

private slots:
    void onPluginConfigurationChanged(const ParamTypeId &paramTypeId, const QVariant &value);

    void onConnectionStateChanged(bool status);
    void onRequestExecuted(QUuid requestId, bool success);
    void onRequestError(QUuid requestId, const QString &error);

    void onReceivedCoil(quint32 slaveAddress, quint32 modbusRegister, const QVector<quint16> &values);
    void onReceivedDiscreteInput(quint32 slaveAddress, quint32 modbusRegister, const QVector<quint16> &values);
    void onReceivedHoldingRegister(uint slaveAddress, uint modbusRegister, const QVector<quint16> &values);
    void onReceivedInputRegister(quint32 slaveAddress, quint32 modbusRegister, const QVector<quint16> &values);
};

#endif // INTEGRATIONPLUGINMODBUSCOMMANDER_H
