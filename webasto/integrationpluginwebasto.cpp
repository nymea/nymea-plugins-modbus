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

#include "integrationpluginwebasto.h"
#include "plugininfo.h"

#include "types/param.h"

#include <QDebug>
#include <QStringList>
#include <QJsonDocument>
#include <QNetworkInterface>


IntegrationPluginWebasto::IntegrationPluginWebasto()
{
}

void IntegrationPluginWebasto::init()
{
    m_discovery = new Discovery(this);
    connect(m_discovery, &Discovery::finished, this, [this](const QList<Host> &hosts) {

        foreach (const Host &host, hosts) {
            if (!host.hostName().contains("webasto", Qt::CaseSensitivity::CaseInsensitive))
                continue;

            foreach (Thing *existingThing, myThings()) {
                if (existingThing->paramValue(liveWallboxThingMacAddressParamTypeId).toString().isEmpty()) {
                    //This device got probably manually setup, to enable auto rediscovery the MAC address needs to setup
                    if (existingThing->paramValue(liveWallboxThingIpAddressParamTypeId).toString() == host.address()) {
                        qCDebug(dcWebasto()) << "Wallbox MAC Address has been discovered" << existingThing->name() << host.macAddress();
                        existingThing->setParamValue(liveWallboxThingMacAddressParamTypeId, host.macAddress());

                    }
                } else if (existingThing->paramValue(liveWallboxThingMacAddressParamTypeId).toString() == host.macAddress())  {
                    if (existingThing->paramValue(liveWallboxThingIpAddressParamTypeId).toString() != host.address()) {
                        qCDebug(dcWebasto()) << "Wallbox IP Address has changed, from"  << existingThing->paramValue(liveWallboxThingIpAddressParamTypeId).toString() << "to" << host.address();
                        existingThing->setParamValue(liveWallboxThingIpAddressParamTypeId, host.address());

                    } else {
                        qCDebug(dcWebasto()) << "Wallbox" << existingThing->name() << "IP address has not changed" << host.address();
                    }
                    break;
                }
            }
        }
    });
}

void IntegrationPluginWebasto::discoverThings(ThingDiscoveryInfo *info)
{
    qCDebug(dcWebasto()) << "Discover things";
    if (info->thingClassId() == liveWallboxThingClassId) {
        m_discovery->discoverHosts(25);
        connect(m_discovery, &Discovery::finished, info, [this, info] (const QList<Host> &hosts) {

            foreach (const Host &host, hosts) {
                if (!host.hostName().contains("webasto", Qt::CaseSensitivity::CaseInsensitive))
                    continue;

                qCDebug(dcWebasto()) << "     - " << host.hostName() << host.address() << host.macAddress();
                ThingDescriptor descriptor(liveWallboxThingClassId, "Wallbox", host.address() + " (" + host.macAddress() + ")");

                // Rediscovery
                foreach (Thing *existingThing, myThings()) {
                    if (existingThing->paramValue(liveWallboxThingMacAddressParamTypeId).toString() == host.macAddress()) {
                        qCDebug(dcWebasto()) << "     - Device is already added";
                        descriptor.setThingId(existingThing->id());
                        break;
                    }
                }
                ParamList params;
                params << Param(liveWallboxThingMacAddressParamTypeId, host.macAddress());
                params << Param(liveWallboxThingIpAddressParamTypeId, host.address());
                descriptor.setParams(params);
                info->addThingDescriptor(descriptor);
            }
            info->finish(Thing::ThingErrorNoError);
        });
    } else {
        Q_ASSERT_X(false, "discoverThings", QString("Unhandled thingClassId: %1").arg(info->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginWebasto::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcWebasto()) << "Setup thing" << thing->name();

    if (thing->thingClassId() == liveWallboxThingClassId) {

        if (m_webastoConnections.contains(thing)) {
            // Clean up after reconfiguration
            m_webastoConnections.take(thing)->deleteLater();
        }
        QHostAddress address = QHostAddress(thing->paramValue(liveWallboxThingIpAddressParamTypeId).toString());
        Webasto *webasto = new Webasto(address, 502, thing);
        m_webastoConnections.insert(thing, webasto);
        connect(webasto, &Webasto::destroyed, this, [thing, this] {m_webastoConnections.remove(thing);});
        //TODO signal socket
        //TODO emit setup finished
        connect(webasto, &Webasto::connectionChanged, info, [info] (bool connected) {
            if (connected)
                info->finish(Thing::ThingErrorNoError);
        });
    } else {
        Q_ASSERT_X(false, "setupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginWebasto::postSetupThing(Thing *thing)
{
    qCDebug(dcWebasto()) << "Post setup thing" << thing->name();

    if (thing->thingClassId() == liveWallboxThingClassId) {
        Webasto *connection = m_webastoConnections.value(thing);
        if (!connection) {
            qCWarning(dcWebasto()) << "Can't find connection to thing";
        }

    } else {
        Q_ASSERT_X(false, "postSetupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginWebasto::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    if (thing->thingClassId() == liveWallboxThingClassId) {

        Webasto *connection = m_webastoConnections.value(thing);
        if (!connection) {
            qCWarning(dcWebasto()) << "Can't find connection to thing";
            return info->finish(Thing::ThingErrorHardwareNotAvailable);
        }

        if (action.actionTypeId() == liveWallboxPowerActionTypeId) {
            // Enable/Disable the charging process
        } else if (action.actionTypeId() == liveWallboxChargeCurrentActionTypeId) {

        } else {
            Q_ASSERT_X(false, "executeAction", QString("Unhandled actionTypeId: %1").arg(action.actionTypeId().toString()).toUtf8());
        }
    } else {
        Q_ASSERT_X(false, "executeAction", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginWebasto::thingRemoved(Thing *thing)
{
    qCDebug(dcWebasto()) << "Delete thing" << thing->name();
    if (thing->thingClassId() == liveWallboxThingClassId) {
    }

    if (myThings().isEmpty()) {
        //Stop timer
    }
}

void IntegrationPluginWebasto::onConnectionChanged(bool connected)
{
    Webasto *connection = static_cast<Webasto *>(sender());
    Thing *thing = m_webastoConnections.key(connection);
    if (!thing) {
        qCWarning(dcWebasto()) << "On connection changed, thing not found for connection";
        return;
    }
    thing->setStateValue(liveWallboxConnectedStateTypeId, connected);
}

