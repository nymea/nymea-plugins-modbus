/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2024, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea.
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

#ifndef INTEGRATIONPLUGINPCELECTRIC_H
#define INTEGRATIONPLUGINPCELECTRIC_H

#include <QObject>
#include <QDebug>

#include <integrations/integrationplugin.h>
#include <network/networkdevicediscovery.h>
#include <plugintimer.h>

#include "pcewallbox.h"
#include "extern-plugininfo.h"

class IntegrationPluginPcElectric : public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginpcelectric.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    explicit IntegrationPluginPcElectric();
    void init() override;

    void discoverThings(ThingDiscoveryInfo *info) override;
    void setupThing(ThingSetupInfo *info) override;
    void postSetupThing(Thing *thing) override;
    void thingRemoved(Thing *thing) override;
    void executeAction(ThingActionInfo *info) override;

private:
    PluginTimer *m_refreshTimer = nullptr;

    QHash<Thing *, PceWallbox *> m_connections;
    QHash<Thing *, NetworkDeviceMonitor *> m_monitors;
    QHash<Thing *, bool> m_initialUpdate;

    // We need to buffer the desired power / current / phase count states because all 3 states
    // will be represented by one register (200 - chaegingCurrent). If all 3 actions get executed, they might
    // overwrite each other, since the action gets started right the way, but the request gets queued.
    // If the actions would be queued, there would be still the issue with the order of the actions
    // (set power to false and then set charging current would always enable charging in the end).
    QHash<Thing *, PceWallbox::ChargingCurrentState> m_chargingCurrentStateBuffer;

    void setupConnection(ThingSetupInfo *info);

};



#endif // INTEGRATIONPLUGINPCELECTRIC_H
