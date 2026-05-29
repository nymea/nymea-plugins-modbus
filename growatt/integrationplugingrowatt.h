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

#ifndef INTEGRATIONPLUGINGROWATT_H
#define INTEGRATIONPLUGINGROWATT_H

#include <QHash>
#include <integrations/integrationplugin.h>
#include <plugintimer.h>

#include "extern-plugininfo.h"
#include "growattmodbusrtuconnection.h"

class IntegrationPluginGrowatt : public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationplugingrowatt.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    explicit IntegrationPluginGrowatt();

    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void thingRemoved(Thing *thing) override;

public slots:
    void executeAction(ThingActionInfo *info) override;

private:
    PluginTimer *m_pluginTimer = nullptr;
    QHash<Thing *, GrowattModbusRtuConnection *> m_connections;

    void setupInverter(ThingSetupInfo *info);
    void setConnectedStates(Thing *inverterThing, bool connected);
    Thing *meterThing(Thing *inverterThing) const;
    Thing *batteryThing(Thing *inverterThing) const;
    void updateInverterState(Thing *thing, GrowattModbusRtuConnection *connection);
    void updateMeterState(Thing *thing, GrowattModbusRtuConnection *connection);
    void updateBatteryState(Thing *thing, GrowattModbusRtuConnection *connection);
    bool batteryAvailable(GrowattModbusRtuConnection *connection) const;
};

#endif // INTEGRATIONPLUGINGROWATT_H
