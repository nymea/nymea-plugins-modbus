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

#include "integrationpluginidm.h"
#include "plugininfo.h"

#include <network/networkdevicediscovery.h>

IntegrationPluginIdm::IntegrationPluginIdm()
{

}

void IntegrationPluginIdm::discoverThings(ThingDiscoveryInfo *info)
{
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcIdm()) << "Failed to discover network devices. The network device discovery is not available.";
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The discovery is not available."));
        return;
    }

    qCDebug(dcIdm()) << "Discovering network...";
    NetworkDeviceDiscoveryReply *discoveryReply = hardwareManager()->networkDeviceDiscovery()->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        ThingDescriptors descriptors;
        qCDebug(dcIdm()) << "Discovery finished. Found" << discoveryReply->networkDeviceInfos().count() << "devices";
        foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {
            qCDebug(dcIdm()) << networkDeviceInfo;
            QString title;
            if (networkDeviceInfo.hostName().isEmpty()) {
                title += networkDeviceInfo.address().toString();
            } else {
                title += networkDeviceInfo.address().toString() + " (" + networkDeviceInfo.hostName() + ")";
            }

            QString description;
            if (networkDeviceInfo.macAddressManufacturer().isEmpty()) {
                description = networkDeviceInfo.macAddress();
            } else {
                description = networkDeviceInfo.macAddress() + " (" + networkDeviceInfo.macAddressManufacturer() + ")";
            }

            ThingDescriptor descriptor(navigator2ThingClassId, title, description);

            // Check if we already have set up this device
            Things existingThings = myThings().filterByParam(navigator2ThingMacAddressParamTypeId, networkDeviceInfo.macAddress());
            if (existingThings.count() == 1) {
                qCDebug(dcIdm()) << "This thing already exists in the system." << existingThings.first() << networkDeviceInfo;
                descriptor.setThingId(existingThings.first()->id());
            }

            ParamList params;
            params << Param(navigator2ThingMacAddressParamTypeId, networkDeviceInfo.macAddress());
            params << Param(navigator2ThingIpAddressParamTypeId, networkDeviceInfo.address().toString());
            descriptor.setParams(params);
            info->addThingDescriptor(descriptor);
        }
        info->finish(Thing::ThingErrorNoError);
    });
}

void IntegrationPluginIdm::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcIdm()) << "setupThing called" << thing->name();

    if (thing->thingClassId() == navigator2ThingClassId) {
        QHostAddress hostAddress = QHostAddress(thing->paramValue(navigator2ThingIpAddressParamTypeId).toString());
        if (hostAddress.isNull()) {
            qCWarning(dcIdm()) << "Setup failed, IP address not valid";
            info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("No IP address given"));
            return;
        }

        if (m_idmConnections.contains(thing)) {
            qCDebug(dcIdm()) << "Cleaning up after reconfiguration";
            m_idmConnections.take(thing)->deleteLater();
        }

        qCDebug(dcIdm()) << "User entered address: " << hostAddress.toString();

        /* Check, if address is already in use for another device */
        foreach (Idm *idm, m_idmConnections) {
            if (hostAddress.isEqual(idm->address())) {
                qCWarning(dcIdm()) << "Address already in use";
                info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("IP address already in use"));
                return;
            }
        }

        qCDebug(dcIdm()) << "Creating Idm object";
        /* Create new Idm object and store it in hash table */
        Idm *idm = new Idm(hostAddress, this);
        if (idm->connectDevice()) {
            qCWarning(dcIdm()) << "Could not connect to thing";
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        connect(idm, &Idm::statusUpdated, info, [info, thing, idm, this] (const IdmInfo &idmInfo) {
            if (idmInfo.connected) {
                m_idmConnections.insert(thing, idm);
                connect(idm, &Idm::statusUpdated, this, &IntegrationPluginIdm::onStatusUpdated);
                connect(idm, &Idm::writeRequestExecuted, this, &IntegrationPluginIdm::onWriteRequestExecuted);
                info->finish(Thing::ThingErrorNoError);
            }
        });
        connect(idm, &Idm::destroyed, this, [thing, this] {m_idmConnections.remove(thing);});
        connect(info, &ThingSetupInfo::aborted, idm, &Idm::deleteLater);


    } else {
        Q_ASSERT_X(false, "setupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginIdm::postSetupThing(Thing *thing)
{
    qCDebug(dcIdm()) << "postSetupThing called" << thing->name();

    if (!m_refreshTimer) {
        qCDebug(dcIdm()) << "Starting refresh timer";
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(10);
        connect(m_refreshTimer, &PluginTimer::timeout, this, &IntegrationPluginIdm::onRefreshTimer);
    }

    if (thing->thingClassId() == navigator2ThingClassId) {
        Idm *idm = m_idmConnections.value(thing);
        if (!idm) {
            qCWarning(dcIdm()) << "Could not find any iDM connection for" << thing->name();
            return;
        }

        thing->setStateValue(navigator2ConnectedStateTypeId, true);
        update(thing);
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

    if (myThings().isEmpty()) {
        qCDebug(dcIdm()) << "Stopping refresh timer";
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_refreshTimer);
        m_refreshTimer = nullptr;
    }
}

void IntegrationPluginIdm::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    if (thing->thingClassId() == navigator2ThingClassId) {
        Idm *idm = m_idmConnections.value(thing);
        if (!idm) {
            return info->finish(Thing::ThingErrorHardwareFailure);
        }
        if (action.actionTypeId() == navigator2TargetTemperatureActionTypeId) {
            double targetTemperature = thing->stateValue(navigator2TargetTemperatureStateTypeId).toDouble();
            QUuid requestId = idm->setTargetTemperature(targetTemperature);
            m_asyncActions.insert(requestId, info);
            connect(info, &ThingActionInfo::aborted, [requestId, this] (){ m_asyncActions.remove(requestId); });
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
        qCDebug(dcIdm()) << "Updating thing" << thing->name();
        Idm *idm = m_idmConnections.value(thing);
        if (!idm) { return; };
        idm->getStatus();
    }
}

void IntegrationPluginIdm::onStatusUpdated(const IdmInfo &info)
{
    qCDebug(dcIdm()) << "Received status from heat pump";
    Idm *idm = qobject_cast<Idm *>(sender());
    Thing *thing = m_idmConnections.key(idm);

    if (!thing)
        return;

    /* Received a structure holding the status info of the
     * heat pump. Update the thing states with the individual fields. */
    thing->setStateValue(navigator2ConnectedStateTypeId, info.connected);
    thing->setStateValue(navigator2PowerStateTypeId, info.power);
    thing->setStateValue(navigator2TemperatureStateTypeId, info.roomTemperature);
    thing->setStateValue(navigator2OutsideAirTemperatureStateTypeId, info.outsideTemperature);
    thing->setStateValue(navigator2WaterTemperatureStateTypeId, info.waterTemperature);
    thing->setStateValue(navigator2TargetTemperatureStateTypeId, info.targetRoomTemperature);
    thing->setStateValue(navigator2TargetWaterTemperatureStateTypeId, info.targetWaterTemperature);
    thing->setStateValue(navigator2CurrentPowerConsumptionHeatPumpStateTypeId, info.powerConsumptionHeatPump);
    thing->setStateValue(navigator2ModeStateTypeId, info.mode);
    thing->setStateValue(navigator2ErrorStateTypeId, info.error);
}

void IntegrationPluginIdm::onWriteRequestExecuted(const QUuid &requestId, bool success)
{
    if (m_asyncActions.contains(requestId)) {
        ThingActionInfo *info = m_asyncActions.value(requestId);
        if (success) {
            info->finish(Thing::ThingErrorNoError);
        } else {
            info->finish(Thing::ThingErrorHardwareNotAvailable);
        }
    }
}

void IntegrationPluginIdm::onRefreshTimer()
{
    foreach (Thing *thing, myThings().filterByThingClassId(navigator2ThingClassId)) {
        update(thing);
    }
}

