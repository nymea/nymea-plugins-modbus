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

IntegrationPluginIdm::IntegrationPluginIdm()
{

}

void IntegrationPluginIdm::discoverThings(ThingDiscoveryInfo *info)
{
    if (info->thingClassId() == navigator2ThingClassId) {
        // TODO Is a discovery method actually needed?
        // The plugin has a parameter for the IP address

        QString description = "Navigator 2";
        ThingDescriptor descriptor(info->thingClassId(), "Idm", description);
        info->addThingDescriptor(descriptor);
        
        // Just report no error for now, until the above question
        // is clarified
        info->finish(Thing::ThingErrorNoError);
    }
}

void IntegrationPluginIdm::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    if (thing->thingClassId() == navigator2ThingClassId) {
        QHostAddress hostAddress = QHostAddress(thing->paramValue(navigator2ThingIpAddressParamTypeId).toString());

        /* Create new Idm object and store it in hash table */
        Idm *idm = new Idm(hostAddress, this);
        m_idmConnections.insert(thing, idm);

        /* Store thing info in hash table */
        m_idmInfos.insert(thing, info);

        info->finish(Thing::ThingErrorNoError);
    }

}

void IntegrationPluginIdm::postSetupThing(Thing *thing)
{
    if (!m_refreshTimer) {
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(10);
        connect(m_refreshTimer, &PluginTimer::timeout, this, &IntegrationPluginIdm::onRefreshTimer);
    }

    if (thing->thingClassId() == navigator2ThingClassId) {
        Idm *idm = m_idmConnections.value(thing);

        connect(idm, &Idm::statusUpdated, this, &IntegrationPluginIdm::onStatusUpdated);

        qCDebug(dcIdm()) << "Thing set up, calling update";
        update(thing);

        thing->setStateValue(navigator2ConnectedStateTypeId, true);
    }
}

void IntegrationPluginIdm::thingRemoved(Thing *thing)
{
    if (m_idmConnections.contains(thing)) {
        m_idmConnections.take(thing)->deleteLater();
        m_idmInfos.take(thing)->deleteLater();
    }
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

void IntegrationPluginIdm::update(Thing *thing)
{
    if (thing->thingClassId() == navigator2ThingClassId) {
        qCDebug(dcIdm()) << "Updating thing";

        Idm *idm = m_idmConnections.value(thing);
        Q_UNUSED(idm);

        QVector<quint16> val{};

        idm->onRequestStatus();

        if (m_idmInfos.contains(thing)) {
            ThingSetupInfo *info = m_idmInfos.take(thing);
            info->finish(Thing::ThingErrorNoError);
        }
    }
}

void IntegrationPluginIdm::onStatusUpdated(IdmInfo *info)
{
    if (!info)
        return;

    qCDebug(dcIdm()) << "Received status from heat pump";

    Idm *idm = static_cast<Idm *>(sender());
    Thing *thing = m_idmConnections.key(idm);

    if (!thing)
        return;

    /* Received a structure holding the status info of the
     * heat pump. Update the thing states with the individual fields. */
    thing->setStateValue(navigator2ConnectedStateTypeId, info->m_connected);
    thing->setStateValue(navigator2PowerStateTypeId, info->m_power);
    thing->setStateValue(navigator2TemperatureStateTypeId, info->m_roomTemperature);
    thing->setStateValue(navigator2OutsideAirTemperatureStateTypeId, info->m_outsideTemperature);
    thing->setStateValue(navigator2WaterTemperatureStateTypeId, info->m_waterTemperature);
    thing->setStateValue(navigator2TargetTemperatureStateTypeId, info->m_targetRoomTemperature);
    thing->setStateValue(navigator2TargetWaterTemperatureStateTypeId, info->m_targetWaterTemperature);
    thing->setStateValue(navigator2ModeStateTypeId, info->m_mode);
    thing->setStateValue(navigator2ErrorStateTypeId, info->m_error);
}

void IntegrationPluginIdm::onRefreshTimer()
{
    foreach (Thing *thing, myThings().filterByThingClassId(navigator2ThingClassId)) {
        update(thing);
    }
}

