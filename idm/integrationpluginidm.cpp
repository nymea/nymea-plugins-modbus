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
    qCDebug(dcIdm()) << "discoverThings called";

    if (info->thingClassId() == navigator2ThingClassId) {
        // TODO Is a discovery method actually possible?
        // The plugin has a parameter for the IP address

        QString description = "Navigator 2";
        ThingDescriptor descriptor(info->thingClassId(), "Idm", description);
        info->addThingDescriptor(descriptor);
        
        // Just report no error for now, until the above question
        // is clarified
        info->finish(Thing::ThingErrorNoError);
    } else {
        Q_ASSERT_X(false, "discoverThings", QString("Unhandled thingClassId: %1").arg(info->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginIdm::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcIdm()) << "setupThing called" << thing->name();

    if (thing->thingClassId() == navigator2ThingClassId) {
        QHostAddress hostAddress = QHostAddress(thing->paramValue(navigator2ThingIpAddressParamTypeId).toString());

        if (hostAddress.isNull()) {
            qCWarning(dcIdm()) << "Setup failed, IP address not valid";
            info->finish(Thing::ThingErrorInvalidParameter, "No IP address given");
            return;
        }

        if (m_idmConnections.contains(thing)) {
            qCDebug(dcIdm()) << "Cleaning up after reconfiguration";
            m_idmConnections.take(thing)->deleteLater();
        }

        qCDebug(dcIdm()) << "User entered address: " << hostAddress.toString();

        /* Check, if address is already in use for another device */
        Q_FOREACH (Idm *idm, m_idmConnections) {
            if (hostAddress.isEqual(idm->getIdmAddress())) {

                qCWarning(dcIdm()) << "Address already in use";
                info->finish(Thing::ThingErrorSetupFailed, "IP address already in use");
                return;
            }
        }

        qCDebug(dcIdm()) << "Creating Idm object";
        /* Create new Idm object and store it in hash table */
        Idm *idm = new Idm(hostAddress, this);
        m_idmConnections.insert(thing, idm);
        connect(idm, &Idm::statusUpdated, info, [info] (IdmInfo *idmInfo) {
            if (idmInfo->connected) {
                info->finish(Thing::ThingErrorNoError);
            }
        });
        connect(idm, &Idm::destroyed, this, [thing, this] {m_idmConnections.remove(thing);});
        connect(info, &ThingSetupInfo::aborted, idm, &Idm::deleteLater);
        connect(idm, &Idm::statusUpdated, this, &IntegrationPluginIdm::onStatusUpdated);

    } else {
        Q_ASSERT_X(false, "setupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginIdm::postSetupThing(Thing *thing)
{
    qCDebug(dcIdm()) << "postSetupThing called" << thing->name();

    if (!m_refreshTimer) {
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(10);
        connect(m_refreshTimer, &PluginTimer::timeout, this, &IntegrationPluginIdm::onRefreshTimer);
    }

    if (thing->thingClassId() == navigator2ThingClassId) {
        qCDebug(dcIdm()) << "Thing id: " << thing->id();
        Idm *idm = m_idmConnections.value(thing);

        if (idm != nullptr) {

            qCDebug(dcIdm()) << "Thing set up, calling update";
            update(thing);

            thing->setStateValue(navigator2ConnectedStateTypeId, true);
        }
    }
}

void IntegrationPluginIdm::thingRemoved(Thing *thing)
{
    qCDebug(dcIdm()) << "thingRemoved called" << thing->name();

    if (thing->thingClassId() == navigator2ThingClassId) {
        if (m_idmConnections.contains(thing)) {
            m_idmConnections.take(thing)->deleteLater();
        }
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

        if (idm != nullptr) {
            idm->onRequestStatus();
        }
<<<<<<< HEAD

        if (m_idmInfos.contains(thing)) {
<<<<<<< HEAD
            ThingSetupInfo *info = m_idmInfos.take(thing);
            info->finish(Thing::ThingErrorNoError);
=======
            /* ThingSetupInfo *info = m_idmInfos.take(thing); */
            /* qCDebug(dcIdm()) << "Finishing setup 4!"; */
            /* info->finish(Thing::ThingErrorNoError); */
>>>>>>> 3b5ab5c... Another fix for previous commit
        }
=======
>>>>>>> 27a88b6... Removed thread delay and used Timer instead
    }
}

void IntegrationPluginIdm::onStatusUpdated(IdmInfo *info)
{
    /* qCDebug(dcIdm()) << "onStatusUpdated"; */
    if (!info)
        return;

    qCDebug(dcIdm()) << "Received status from heat pump";

    Idm *idm = static_cast<Idm *>(sender());
    Thing *thing = m_idmConnections.key(idm);

    if (!thing)
        return;

    /* Received a structure holding the status info of the
     * heat pump. Update the thing states with the individual fields. */
    thing->setStateValue(navigator2ConnectedStateTypeId, info->connected);
    thing->setStateValue(navigator2PowerStateTypeId, info->power);
    thing->setStateValue(navigator2TemperatureStateTypeId, info->roomTemperature);
    thing->setStateValue(navigator2OutsideAirTemperatureStateTypeId, info->outsideTemperature);
    thing->setStateValue(navigator2WaterTemperatureStateTypeId, info->waterTemperature);
    thing->setStateValue(navigator2TargetTemperatureStateTypeId, info->targetRoomTemperature);
    thing->setStateValue(navigator2TargetWaterTemperatureStateTypeId, info->targetWaterTemperature);
    thing->setStateValue(navigator2CurrentPowerConsumptionHeatPumpStateTypeId, info->powerConsumptionHeatPump);
    thing->setStateValue(navigator2ModeStateTypeId, info->mode);
    thing->setStateValue(navigator2ErrorStateTypeId, info->error);
}

void IntegrationPluginIdm::onRefreshTimer()
{
    qCDebug(dcIdm()) << "onRefreshTimer called";

    foreach (Thing *thing, myThings().filterByThingClassId(navigator2ThingClassId)) {
        update(thing);
    }
}

