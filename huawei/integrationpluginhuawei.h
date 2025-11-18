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

#ifndef INTEGRATIONPLUGINHUAWEI_H
#define INTEGRATIONPLUGINHUAWEI_H

#include <QHash>
#include <plugintimer.h>
#include <integrations/integrationplugin.h>
#include <network/networkdevicediscovery.h>

#include "extern-plugininfo.h"

#include "huaweifusionsolar.h"
#include "huaweimodbusrtuconnection.h"
#include "huaweismartlogger.h"

class IntegrationPluginHuawei: public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginhuawei.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    explicit IntegrationPluginHuawei();

    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void thingRemoved(Thing *thing) override;

private:
    PluginTimer *m_pluginTimer = nullptr;

    QHash<Thing *, NetworkDeviceMonitor *> m_monitors;
    QHash<Thing *, HuaweiFusionSolar *> m_connections;
    QHash<Thing *, HuaweiSmartLogger *> m_smartLoggerConnections;
    QHash<Thing *, HuaweiModbusRtuConnection *> m_rtuConnections;

    void setupFusionSolar(ThingSetupInfo *info);
    void setupSmartLogger(ThingSetupInfo *info);

    QHash<Thing *, QList<float>> m_inverterEnergyProducedHistory;
    void evaluateEnergyProducedValue(Thing *inverterThing, float energyProduced);
};

#endif // INTEGRATIONPLUGINHUAWEI_H

