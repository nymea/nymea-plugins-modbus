/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
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

#include "integrationpluginidm.h"
#include "plugininfo.h"

void IntegrationPluginIdm::discoverThings(ThingDiscoveryInfo *info)
{
    if (info->thingClassId() == navigator2ThingClassId) {
        //TODO add discovery method
    }
}

void IntegrationPluginIdm::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    if (thing->thingClassId() == navigator2ThingClassId) {
        QHostAddress hostAddress = QHostAddress(thing->paramValue(navigator2ThingIpAddressParamTypeId).toString());
        Idm *idm = new Idm(hostAddress, this);
        m_idmConnections.insert(thing, idm);
    }

}

void IntegrationPluginIdm::postSetupThing(Thing *thing)
{
    if (thing->thingClassId() == navigator2ThingClassId) {
        Idm *idm = m_idmConnections.value(thing);

    }
}

void IntegrationPluginIdm::thingRemoved(Thing *thing)
{
    if (m_idmConnections.contains(thing))
        m_idmConnections.take(thing)->deleteLater();
}

void IntegrationPluginIdm::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    if (thing->thingClassId() == navigator2ThingClassId) {
      if (action.actionTypeId() == navigator2PowerActionTypeId) {
        } else {
        Q_ASSERT_X(false, "executeAction", QString("Unhandled action: %1").arg(action.actionTypeId().toString()).toUtf8());
        }
    } else {
        Q_ASSERT_X(false, "executeAction", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}
