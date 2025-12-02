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

#ifndef INTEGRATIONPLUGINPHOENIXCONNECT_H
#define INTEGRATIONPLUGINPHOENIXCONNECT_H

#include <integrations/integrationplugin.h>
#include "extern-plugininfo.h"

#include <QObject>
#include <QHostAddress>

class PhoenixModbusTcpConnection;
class NetworkDeviceMonitor;
class PluginTimer;

class IntegrationPluginPhoenixConnect : public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginphoenixconnect.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    explicit IntegrationPluginPhoenixConnect();
    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void executeAction(ThingActionInfo *info) override;
    void thingRemoved(Thing *thing) override;

private slots:
    void updatePhaseCount(Thing *thing);

private:
    void evaluateChargingState(Thing *thing);

private:
    QHash<Thing*, PhoenixModbusTcpConnection*> m_connections;
    QHash<Thing*, NetworkDeviceMonitor*> m_monitors;
    PluginTimer *m_pluginTimer = nullptr;
};

#endif // INTEGRATIONPLUGINPHOENIXCONNECT_H
