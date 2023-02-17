/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2022, nymea GmbH
* Contact: contact@nymea.io
*
* This fileDescriptor is part of nymea.
* This project including source code and documentation is protected by
* copyright law, and remains the property of nymea GmbH. All rights, including
* reproduction, publication, editing and translation, are reserved. The use of
* this project is subject to the terms of a license agreement to be concluded
* with nymea GmbH in accordance with the terms of use of nymea GmbH, available
* under https://nymea.io/license
*
* GNU Lesser General Public License Usage
* Alternatively, this project may be redistributed and/or modified under the
* terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; version 3. This project is distributed in the hope that
* it will be useful, but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this project. If not, see <https://www.gnu.org/licenses/>.
*
* For any further details and any questions please contact us under
* contact@nymea.io or see our FAQ/Licensing Information on
* https://nymea.io/license/faq
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
    QHash<Thing *, HuaweiModbusRtuConnection *> m_rtuConnections;

    void setupFusionSolar(ThingSetupInfo *info);

    QHash<Thing *, QList<float>> m_inverterEnergyProducedHistory;
    void evaluateEnergyProducedValue(Thing *inverterThing, float energyProduced);
};

#endif // INTEGRATIONPLUGINHUAWEI_H

