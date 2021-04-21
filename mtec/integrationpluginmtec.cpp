/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2021, nymea GmbH
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

#include "integrationpluginmtec.h"
#include "plugininfo.h"

IntegrationPluginMTec::IntegrationPluginMTec()
{

}

void IntegrationPluginMTec::discoverThings(ThingDiscoveryInfo *info)
{
    qCDebug(dcMTec()) << "Discover M-Tec heat pumps";

    if (info->thingClassId() == mtecThingClassId) {
        QString description = "Heatpump";
        ThingDescriptor descriptor(info->thingClassId(), "M-Tec", description);
        info->addThingDescriptor(descriptor);

        // TODO Find out, if a discovery is possible/needed
        // Otherwise, just report no error for now
        info->finish(Thing::ThingErrorNoError);
    }
}

void IntegrationPluginMTec::setupThing(ThingSetupInfo *info)
{
    qCDebug(dcMTec()) << "Setup" << info->thing();

    Thing *thing = info->thing();

    if (thing->thingClassId() == mtecThingClassId) {
        QHostAddress hostAddress = QHostAddress(thing->paramValue(mtecThingIpAddressParamTypeId).toString());

        if (hostAddress.isNull()) {
            info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("No IP address given"));
            return;
        }

        qCDebug(dcMTec()) << "User entered address: " << hostAddress.toString();

        /* Check, if address is already in use for another device */
        /* for (QHash<Thing *, MTec *>::iterator item=m_mtecConnections.begin(); item != m_mtecConnections.end(); item++) { */
        /*     if (hostAddress.isEqual(item.value()->getHostAddress())) { */
        /*         qCDebug(dcMTec()) << "Address of thing: " << item.value()->getHostAddress().toString(); */

        /*         qCDebug(dcMTec()) << "Address in use already"; */
        /*     } else { */
        /*         qCDebug(dcMTec()) << "Different address of other thing: " << item.value()->getHostAddress().toString(); */

        /*     } */
        /* } */

        foreach (MTec *mtecConnection, m_mtecConnections.values()) {
            if (mtecConnection->getHostAddress().isEqual(hostAddress)) {
                qCWarning(dcMTec()) << "Address" << hostAddress.toString() << "already in use by" << m_mtecConnections.key(mtecConnection);
                info->finish(Thing::ThingErrorThingInUse, QT_TR_NOOP("IP address already in use by another thing."));
                return;
            }
        }

        qCDebug(dcMTec()) << "Creating M-Tec object";

        /* Create new MTec object and store it in hash table */
        MTec *mtec = new MTec(hostAddress, this);
        m_mtecConnections.insert(thing, mtec);

        info->thing()->setStateValue(mtecConnectedStateTypeId, true);
        info->finish(Thing::ThingErrorNoError);
    }
}

void IntegrationPluginMTec::postSetupThing(Thing *thing)
{
    qCDebug(dcMTec()) << "PostSetup called for" << thing;

    if (thing->thingClassId() == mtecThingClassId) {
        MTec *mtec = m_mtecConnections.value(thing);

        if (mtec) {
            connect(mtec, &MTec::statusUpdated, this, &IntegrationPluginMTec::onStatusUpdated);
            connect(mtec, &MTec::connectedChanged, this, &IntegrationPluginMTec::onConnectedChanged);

            qCDebug(dcMTec()) << "Thing set up, calling update";
            update(thing);

            thing->setStateValue(mtecConnectedStateTypeId, true);
        }
    }
}

void IntegrationPluginMTec::thingRemoved(Thing *thing)
{
    if (m_mtecConnections.contains(thing)) {
        m_mtecConnections.take(thing)->deleteLater();
    }
}

void IntegrationPluginMTec::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    if (thing->thingClassId() == mtecThingClassId) {
      /* if (action.actionTypeId() == mtecPowerActionTypeId) { */
          
      /* } else { */
      /*   Q_ASSERT_X(false, "executeAction", QString("Unhandled action: %1").arg(action.actionTypeId().toString()).toUtf8()); */
      /*   } */
    } else {
        Q_ASSERT_X(false, "executeAction", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginMTec::update(Thing *thing)
{
    if (thing->thingClassId() == mtecThingClassId) {
        qCDebug(dcMTec()) << "Updating thing" << thing;

        MTec *mtec = m_mtecConnections.value(thing);

        if (mtec) {
            mtec->onRequestStatus();
        }
    }
}

void IntegrationPluginMTec::onConnectedChanged(bool connected)
{
    MTec *mtec = qobject_cast<MTec *>(sender());
    Thing *thing = m_mtecConnections.key(mtec);

    qCDebug(dcMTec()) << "Received connection change event from heat pump" << thing;

    if (!thing)
        return;

    thing->setStateValue(mtecConnectedStateTypeId, connected);
}

void IntegrationPluginMTec::onStatusUpdated(const MTecInfo &info)
{
    MTec *mtec = qobject_cast<MTec *>(sender());
    Thing *thing = m_mtecConnections.key(mtec);

    qCDebug(dcMTec()) << "Received status from heat pump" << thing;

    if (!thing)
        return;

    /* Received a structure holding the status info of the
     * heat pump. Update the thing states with the individual fields. */
    thing->setStateValue(mtecStatusStateTypeId, info.status);
    thing->setStateValue(mtecActualPowerConsumptionStateTypeId, info.actualPowerConsumption);
    thing->setStateValue(mtecActualExcessEnergySmartHomeStateTypeId, info.actualExcessEnergySmartHome);
    thing->setStateValue(mtecActualExcessEnergyElectricityMeterStateTypeId, info.actualExcessEnergyElectricityMeter);
    thing->setStateValue(mtecExternalSetValueScalingStateTypeId, info.externalSetValueScaling);
    thing->setStateValue(mtecRequestExternalHeatSourceStateTypeId, info.requestExternalHeatSource);
}

void IntegrationPluginMTec::onRefreshTimer()
{
    qCDebug(dcMTec()) << "onRefreshTimer called";

    foreach (Thing *thing, myThings().filterByThingClassId(mtecThingClassId)) {
        update(thing);
    }
}


