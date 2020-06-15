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

#include "integrationpluginwallbe.h"
#include "plugininfo.h"

#include "types/param.h"

#include <QDebug>
#include <QStringList>
#include <QJsonDocument>
#include <QNetworkInterface>


IntegrationPluginWallbe::IntegrationPluginWallbe()
{
}


void IntegrationPluginWallbe::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcWallbe) << "Setting up a new device:" << thing->params();

    QHostAddress address(thing->paramValue(wallbeEcoThingIpParamTypeId).toString());

    if (address.isNull()){
        qCWarning(dcWallbe) << "IP address is null";
        info->finish(Thing::ThingErrorSetupFailed, tr("IP address parameter not valid"));
        return;
    }

    WallBe *wallbe = new WallBe(address, 502, this);
    m_connections.insert(thing, wallbe);

    info->finish(Thing::ThingErrorNoError);
}


void IntegrationPluginWallbe::discoverThings(ThingDiscoveryInfo *info)
{
    if (info->thingClassId() == wallbeEcoThingClassId){

        Discover *discover = new Discover(QStringList("-xO" "-p 502"));
        connect(discover, &Discover::devicesDiscovered, this, [info, this](QList<Host> hosts){
            foreach (Host host, hosts) {
                ThingDescriptor descriptor;
                foreach(Thing *thing, myThings().filterByParam(wallbeEcoThingMacParamTypeId, host.macAddress())) {
                    descriptor.setThingId(thing->id());
                    break;
                }
                descriptor.setTitle(host.hostName());
                ParamList params;
                params.append(Param(wallbeEcoThingIpParamTypeId, host.address()));
                params.append(Param(wallbeEcoThingMacParamTypeId, host.macAddress()));
                descriptor.setParams(params);
                info->addThingDescriptor(descriptor);
            }
            info->finish(Thing::ThingErrorNoError);
        });
        return;
    }
    Q_ASSERT_X(false, "discoverThings", QString("Unhandled thingClassId: %1").arg(info->thingClassId().toString()).toUtf8());
}


void IntegrationPluginWallbe::postSetupThing(Thing *thing)
{
    if (!m_pluginTimer) {
        m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(120);
        connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
            foreach(Thing *thing, m_connections.keys()) {
                update(thing);
            }
        });
    }

    if (thing->thingClassId() == wallbeEcoThingClassId){
        update(thing);
    } else {
        Q_ASSERT_X(false, "postSetupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}


void IntegrationPluginWallbe::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    WallBe *wallbe = m_connections.value(thing);
    if (!wallbe) {
        qCWarning(dcWallbe()) << "Wallbe object not available";
        info->finish(Thing::ThingErrorHardwareFailure);
        return;
    }

    if (thing->thingClassId() == wallbeEcoThingClassId){

        // check if this is the "set power" action
        if (action.actionTypeId() == wallbeEcoPowerActionTypeId) {

            // get the param value of the charging action
            bool charging =  action.param(wallbeEcoPowerActionPowerParamTypeId).value().toBool();
            qCDebug(dcWallbe) << "start Charging button" << thing->name() << "set power to" << charging;
            wallbe->setChargingStatus(charging);
            // Set the "power" state
            thing->setStateValue(wallbeEcoPowerStateTypeId, charging);
            info->finish(Thing::ThingErrorNoError);
            return;

        } else if(action.actionTypeId() == wallbeEcoChargeCurrentActionTypeId){

            uint16_t current = action.param(wallbeEcoChargeCurrentEventChargeCurrentParamTypeId).value().toUInt();
            qCDebug(dcWallbe) << "Charging power set to" << current;
            wallbe->setChargingCurrent(current);
            thing->setStateValue(wallbeEcoChargeCurrentStateTypeId, current);
            info->finish(Thing::ThingErrorNoError);
            return;
        }
        Q_ASSERT_X(false, "executeAction", QString("Unhandled action: %1").arg(action.actionTypeId().toString()).toUtf8());
    }
    Q_ASSERT_X(false, "executeAction", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
}


void IntegrationPluginWallbe::thingRemoved(Thing *thing)
{
    m_address.removeOne(QHostAddress(thing->paramValue(wallbeEcoThingIpParamTypeId).toString()));
    WallBe *wallbe = m_connections.take(thing);
    if (wallbe) {
        qCDebug(dcWallbe) << "Remove device" << thing->name();
        wallbe->deleteLater();
    }

    if (myThings().isEmpty()) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginWallbe::update(Thing *thing)
{    
    WallBe * wallbe = m_connections.value(thing);
    if(!wallbe->isAvailable())
        return;

    thing->setStateValue(wallbeEcoConnectedStateTypeId, true);

    //EV state - 16 bit ASCII (8bit)
    switch (wallbe->getEvStatus()) {
    case 65:
        thing->setStateValue(wallbeEcoEvStatusStateTypeId, "A - No car plugged in");
        break;
    case 66:
        thing->setStateValue(wallbeEcoEvStatusStateTypeId, "B - Supply equipment not yet ready");
        break;
    case 67:
        thing->setStateValue(wallbeEcoEvStatusStateTypeId, "C - Ready to charge");
        break;
    case 68:
        thing->setStateValue(wallbeEcoEvStatusStateTypeId, "D - Ready to charge, ventilation needed");
        break;
    case 69:
        thing->setStateValue(wallbeEcoEvStatusStateTypeId, "E - Short circuit detected");
        break;
    case 70:
        thing->setStateValue(wallbeEcoEvStatusStateTypeId, "F - Supply equipment not available");
        break;
    default:
        thing->setStateValue(wallbeEcoEvStatusStateTypeId, "F - Supply equipment not available");
    }

    qCDebug(dcWallbe) << "EV State:" << thing->stateValue(wallbeEcoEvStatusStateTypeId).toString();

    // Extract Input Register 102 - load time - 32bit integer
    thing->setStateValue(wallbeEcoChargeTimeStateTypeId, wallbe->getChargingTime());

    // Read the charge current state
    thing->setStateValue(wallbeEcoChargeCurrentStateTypeId, wallbe->getChargingCurrent());
    thing->setStateValue(wallbeEcoPowerStateTypeId,  wallbe->getChargingStatus());
}
