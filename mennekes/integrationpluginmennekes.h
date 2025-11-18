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

#ifndef INTEGRATIONPLUGINMENNEKES_H
#define INTEGRATIONPLUGINMENNEKES_H

#include <plugintimer.h>
#include <integrations/integrationplugin.h>
#include <network/networkdevicemonitor.h>

#include "extern-plugininfo.h"

#include "amtronecu.h"
#include "amtronhcc3modbustcpconnection.h"
#include "amtroncompact20modbusrtuconnection.h"

class IntegrationPluginMennekes: public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginmennekes.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    explicit IntegrationPluginMennekes();

    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void executeAction(ThingActionInfo *info) override;
    void thingRemoved(Thing *thing) override;

private slots:
    void updateECUPhaseCount(Thing *thing);
    void updateCompact20PhaseCount(Thing *thing);

private:
    void setupAmtronECUConnection(ThingSetupInfo *info);
    void setupAmtronHCC3Connection(ThingSetupInfo *info);
    void setupAmtronCompact20Connection(ThingSetupInfo *info);

    PluginTimer *m_pluginTimer = nullptr;
    QHash<Thing *, AmtronECU *> m_amtronECUConnections;
    QHash<Thing *, AmtronHCC3ModbusTcpConnection *> m_amtronHCC3Connections;
    QHash<Thing *, AmtronCompact20ModbusRtuConnection *> m_amtronCompact20Connections;
    QHash<Thing *, NetworkDeviceMonitor *> m_monitors;

};

#endif // INTEGRATIONPLUGINMENNEKES_H


