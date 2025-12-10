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

#ifndef INTEGRATIONPLUGINMYPV_H
#define INTEGRATIONPLUGINMYPV_H

#include <integrations/integrationplugin.h>
#include <plugintimer.h>

#include <modbustcpmaster.h>

#include <QUdpSocket>
#include <QUuid>

class IntegrationPluginMyPv: public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginmypv.json")
    Q_INTERFACES(IntegrationPlugin)


public:
    explicit IntegrationPluginMyPv();

    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void thingRemoved(Thing *thing) override;
    void executeAction(ThingActionInfo *info) override;

private:

    enum ElwaModbusRegisters {
        Power= 1000,
        WaterTemperature = 1001,
        TargetWaterTemperature = 1002,
        Status = 1003,
        ManuelStart = 1012
    };

    enum ElwaStatus {
        Heating = 2,
        Standby = 3,
        Boosted = 4,
        HeatFinished = 5,
        Setup = 9,
        ErrorOvertempFuseBlown = 201,
        ErrorOvertempMeasured = 202,
        ErrorOvertempElectronics = 203,
        ErrorHardwareFault = 204,
        ErrorTempSensor = 205
    };

    PluginTimer *m_refreshTimer = nullptr;
    QHash<Thing *, ModbusTcpMaster *> m_modbusTcpMasters;
    QHash<QUuid, ThingActionInfo *> m_asyncActions;

    void update(Thing *thing);

private slots:
    void onRefreshTimer();

    void onConnectionStateChanged(bool status);
    void onWriteRequestExecuted(QUuid requestId, bool success);
    void onWriteRequestError(QUuid requestId, const QString &error);

    void onReceivedHoldingRegister(quint32 slaveAddress, quint32 modbusRegister, const QVector<quint16> &values);
};

#endif // INTEGRATIONPLUGINMYPV_H

