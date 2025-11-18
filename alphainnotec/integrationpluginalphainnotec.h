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

#ifndef INTEGRATIONPLUGINALPHAINNOTEC_H
#define INTEGRATIONPLUGINALPHAINNOTEC_H

#include <plugintimer.h>
#include <integrations/integrationplugin.h>

class NetworkDeviceMonitor;
class AlphaInnotecModbusTcpConnection;

class IntegrationPluginAlphaInnotec: public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginalphainnotec.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    explicit IntegrationPluginAlphaInnotec();

    void discoverThings(ThingDiscoveryInfo *info) override;
    void startMonitoringAutoThings() override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void thingRemoved(Thing *thing) override;
    void executeAction(ThingActionInfo *info) override;

private:
    PluginTimer *m_pluginTimer = nullptr;
    QHash<Thing *, AlphaInnotecModbusTcpConnection *> m_connections;
    QHash<Thing *, NetworkDeviceMonitor *> m_monitors;
};

#endif // INTEGRATIONPLUGINALPHAINNOTEC_H


