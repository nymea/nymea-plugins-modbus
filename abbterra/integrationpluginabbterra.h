#ifndef INTEGRATIONPLUGINABBTERRA_H
#define INTEGRATIONPLUGINABBTERRA_H

#include <plugintimer.h>
#include "integrations/integrationplugin.h"
#include <network/networkdevicemonitor.h>

#include "extern-plugininfo.h"
#include "abbterramodbusrtuconnection.h"
#include "abbterramodbustcpconnection.h"

class IntegrationPluginAbbterra : public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginabbterra.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    explicit IntegrationPluginAbbterra();

    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void thingRemoved(Thing *thing) override;

public slots:
    void executeAction(ThingActionInfo *info) override;

private:
    void setupTcpThing(ThingSetupInfo *info);
    void setupRtuThing(ThingSetupInfo *info);
    void applyTimeoutSetting(Thing *thing, AbbTerraModbusTcpConnection *connection);
    void applyTimeoutSetting(Thing *thing, AbbTerraModbusRtuConnection *connection);
    void updateThing(Thing *thing, AbbTerraModbusTcpConnection *connection);
    void updateThing(Thing *thing, AbbTerraModbusRtuConnection *connection);
    void setDisconnectedState(Thing *thing);

private:
    PluginTimer *m_pluginTimer = nullptr;
    QHash<Thing *, AbbTerraModbusTcpConnection *> m_tcpConnections;
    QHash<Thing *, AbbTerraModbusRtuConnection *> m_rtuConnections;
    QHash<Thing *, NetworkDeviceMonitor *> m_monitors;
};

#endif // INTEGRATIONPLUGINABBTERRA_H
