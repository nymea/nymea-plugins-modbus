// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INTEGRATIONPLUGINBENDER_H
#define INTEGRATIONPLUGINBENDER_H

#include <plugintimer.h>
#include <integrations/integrationplugin.h>
#include <network/networkdevicemonitor.h>

#include "extern-plugininfo.h"
#include "benderomccimodbustcpconnection.h"
#include "bendertqdm100modbustcpconnection.h"

class IntegrationPluginBender : public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginbender.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    explicit IntegrationPluginBender();

    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void executeAction(ThingActionInfo *info) override;
    void thingRemoved(Thing *thing) override;

private:
    void setupTqDm100Connection(ThingSetupInfo *info);
    void setupOmcciConnection(ThingSetupInfo *info);
    void updateTqDm100MaxCurrent(Thing *thing);
    void updateOmcciMaxCurrent(Thing *thing);
    void clearLivePowerStates(Thing *thing);

    PluginTimer *m_pluginTimer = nullptr;
    QHash<Thing *, NetworkDeviceMonitor *> m_monitors;
    QHash<Thing *, BenderTqDm100ModbusTcpConnection *> m_tqDm100Connections;
    QHash<Thing *, BenderOmcciModbusTcpConnection *> m_omcciConnections;
    QHash<Thing *, quint16> m_lifeBits;
};

#endif // INTEGRATIONPLUGINBENDER_H
