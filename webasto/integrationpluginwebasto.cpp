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

#include "integrationpluginwebasto.h"
#include "webastodiscovery.h"
#include "plugininfo.h"

#include <types/param.h>
#include <hardware/electricity.h>
#include <network/networkdevicediscovery.h>
#include <network/networkaccessmanager.h>

#include <QDebug>
#include <QStringList>
#include <QJsonDocument>
#include <QNetworkInterface>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "../vestel/evc04discovery.h"

IntegrationPluginWebasto::IntegrationPluginWebasto()
{
}

void IntegrationPluginWebasto::init()
{

}

void IntegrationPluginWebasto::discoverThings(ThingDiscoveryInfo *info)
{
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcWebasto()) << "Failed to discover network devices. The network device discovery is not available.";
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The discovery is not available."));
        return;
    }

    if (info->thingClassId() == webastoLiveThingClassId) {
        qCInfo(dcWebasto()) << "Start discovering webasto live in the local network...";
        NetworkDeviceDiscoveryReply *discoveryReply = hardwareManager()->networkDeviceDiscovery()->discover();
        connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
        connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [this, discoveryReply, info](){
            ThingDescriptors descriptors;
            qCDebug(dcWebasto()) << "Discovery finished. Found" << discoveryReply->networkDeviceInfos().length() << "devices";
            foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {
                qCDebug(dcWebasto()) << networkDeviceInfo;
                if (!networkDeviceInfo.hostName().contains("webasto", Qt::CaseSensitivity::CaseInsensitive))
                    continue;

                QString title = "Webasto Live";
                if (networkDeviceInfo.hostName().isEmpty()) {
                    title += networkDeviceInfo.address().toString();
                } else {
                    title += networkDeviceInfo.address().toString() + " (" + networkDeviceInfo.hostName() + ")";
                }

                QString description;
                MacAddressInfo macInfo;
                switch (networkDeviceInfo.monitorMode()) {
                case NetworkDeviceInfo::MonitorModeMac:
                    macInfo = networkDeviceInfo.macAddressInfos().constFirst();
                    description = macInfo.macAddress().toString();
                    if (!macInfo.vendorName().isEmpty())
                        description += " - " + macInfo.vendorName();

                    break;
                case NetworkDeviceInfo::MonitorModeHostName:
                    description = networkDeviceInfo.hostName();
                    break;
                case NetworkDeviceInfo::MonitorModeIp:
                    description = "Interface: " + networkDeviceInfo.networkInterface().name();
                    break;
                }

                ThingDescriptor descriptor(webastoLiveThingClassId, title, description);

                ParamList params;
                params << Param(webastoLiveThingMacAddressParamTypeId, networkDeviceInfo.thingParamValueMacAddress());
                params << Param(webastoLiveThingAddressParamTypeId, networkDeviceInfo.thingParamValueAddress());
                params << Param(webastoLiveThingHostNameParamTypeId, networkDeviceInfo.thingParamValueHostName());
                descriptor.setParams(params);

                // Check if we already have set up this device
                Thing *existingThing = myThings().findByParams(params);
                if (existingThing) {
                    qCDebug(dcWebasto()) << "This thing already exists in the system:" << networkDeviceInfo;
                    descriptor.setThingId(existingThing->id());
                }

                info->addThingDescriptor(descriptor);
            }
            info->finish(Thing::ThingErrorNoError);
        });

        return;
    }


    if (info->thingClassId() == webastoNextThingClassId) {

        qCInfo(dcWebasto()) << "Start discovering Webasto NEXT in the local network...";

        // Create a discovery with the info as parent for auto deleting the object once the discovery info is done
        WebastoDiscovery *discovery = new WebastoDiscovery(hardwareManager()->networkDeviceDiscovery(), info);
        connect(discovery, &WebastoDiscovery::discoveryFinished, info, [this, discovery, info](){
            foreach (const WebastoDiscovery::Result &result, discovery->results()) {

                QString title = "Webasto Next";
                if (!result.networkDeviceInfo.hostName().isEmpty()){
                    title.append(" (" + result.networkDeviceInfo.hostName() + ")");
                }

                QString description;
                MacAddressInfo macInfo;
                switch (result.networkDeviceInfo.monitorMode()) {
                case NetworkDeviceInfo::MonitorModeMac:
                    macInfo = result.networkDeviceInfo.macAddressInfos().constFirst();
                    description = macInfo.macAddress().toString();
                    if (!macInfo.vendorName().isEmpty())
                        description += " - " + macInfo.vendorName();

                    break;
                case NetworkDeviceInfo::MonitorModeHostName:
                    description = result.networkDeviceInfo.hostName();
                    break;
                case NetworkDeviceInfo::MonitorModeIp:
                    description = "Interface: " + result.networkDeviceInfo.networkInterface().name();
                    break;
                }

                ThingDescriptor descriptor(webastoNextThingClassId, title, description);

                ParamList params;
                params << Param(webastoNextThingMacAddressParamTypeId, result.networkDeviceInfo.thingParamValueMacAddress());
                params << Param(webastoNextThingHostNameParamTypeId, result.networkDeviceInfo.thingParamValueHostName());
                params << Param(webastoNextThingAddressParamTypeId, result.networkDeviceInfo.thingParamValueAddress());
                descriptor.setParams(params);

                // Check if we already have set up this device
                Thing *existingThing = myThings().findByParams(params);
                if (existingThing) {
                    qCDebug(dcWebasto()) << "This thing already exists in the system:" << result.networkDeviceInfo;
                    descriptor.setThingId(existingThing->id());
                }

                info->addThingDescriptor(descriptor);
            }

            info->finish(Thing::ThingErrorNoError);
        });

        discovery->startDiscovery();
        return;
    }

    if (info->thingClassId() == webastoUniteThingClassId) {
        EVC04Discovery *discovery = new EVC04Discovery(hardwareManager()->networkDeviceDiscovery(), dcWebasto(), info);
        connect(discovery, &EVC04Discovery::discoveryFinished, info, [=](){
            foreach (const EVC04Discovery::Result &result, discovery->discoveryResults()) {

                if (result.brand != "Webasto") {
                    qCDebug(dcWebasto()) << "Skipping Vestel wallbox without Webasto branding...";
                    continue;
                }
                QString name = result.chargepointId;
                QString description = result.brand + " " + result.model;
                ThingDescriptor descriptor(webastoUniteThingClassId, name, description);
                qCDebug(dcWebasto()) << "Discovered:" << descriptor.title() << descriptor.description();

                ParamList params;
                params << Param(webastoUniteThingMacAddressParamTypeId, result.networkDeviceInfo.thingParamValueMacAddress());
                params << Param(webastoUniteThingHostNameParamTypeId, result.networkDeviceInfo.thingParamValueHostName());
                params << Param(webastoUniteThingAddressParamTypeId, result.networkDeviceInfo.thingParamValueAddress());
                descriptor.setParams(params);

                // Check if we already have set up this device
                Thing *existingThing = myThings().findByParams(params);
                if (existingThing) {
                    qCDebug(dcWebasto()) << "This thing already exists in the system:" << result.networkDeviceInfo;
                    descriptor.setThingId(existingThing->id());
                }

                info->addThingDescriptor(descriptor);
            }

            info->finish(Thing::ThingErrorNoError);
        });

        discovery->startDiscovery();
        return;
    }

    Q_ASSERT_X(false, "discoverThings", QString("Unhandled thingClassId: %1").arg(info->thingClassId().toString()).toUtf8());
}

void IntegrationPluginWebasto::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcWebasto()) << "Setup thing" << thing->name();

    if (thing->thingClassId() == webastoLiveThingClassId) {

        if (m_webastoLiveConnections.contains(thing)) {
            // Clean up after reconfiguration
            m_webastoLiveConnections.take(thing)->deleteLater();

            if (m_monitors.contains(thing)) {
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        }

        NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(thing);
        if (!monitor) {
            qCWarning(dcWebasto()) << "Unable to register monitor with the given params" << thing->params();
            info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("Unable to set up the connection with this configuration, please reconfigure the connection."));
            return;
        }

        m_monitors.insert(thing, monitor);

        connect(info, &ThingSetupInfo::aborted, monitor, [this, thing](){
            if (m_monitors.contains(thing)) {
                qCDebug(dcWebasto()) << "Unregistering monitor because setup has been aborted.";
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }

            if (m_webastoLiveConnections.contains(thing)) {
                qCDebug(dcWebasto()) << "Clean up connection because setup has been aborted.";
                m_webastoLiveConnections.take(thing)->deleteLater();
            }
        });

        QHostAddress address = monitor->networkDeviceInfo().address();
        if (address.isNull()) {
            qCWarning(dcWebasto()) << "Cannot set up thing. The host address is not known yet. Maybe it will be available in the next run...";
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("The host address is not known yet. Trying later again."));
            return;
        }

        Webasto *webasto = new Webasto(address, 502, thing);
        m_webastoLiveConnections.insert(thing, webasto);

        connect(monitor, &NetworkDeviceMonitor::reachableChanged, thing, [monitor, webasto, thing](bool reachable){
            qCDebug(dcWebasto()) << "Network device monitor reachable changed for" << thing->name() << reachable;
            if (!thing->setupComplete())
                return;

            if (reachable && !thing->stateValue("connected").toBool()) {
                webasto->modbusTcpMaster()->setHostAddress(monitor->networkDeviceInfo().address());
                webasto->connectDevice();
            } else if (!reachable) {
                // Note: We disable autoreconnect explicitly and we will
                // connect the device once the monitor says it is reachable again
                webasto->modbusTcpMaster()->disconnectDevice();
            }
        });

        connect(webasto, &Webasto::connectionStateChanged, this, [thing](bool state){
            thing->setStateValue(webastoLiveConnectedStateTypeId, state);
        });

        connect(webasto, &Webasto::receivedRegister, this, &IntegrationPluginWebasto::onReceivedRegister);
        connect(webasto, &Webasto::writeRequestError, this, &IntegrationPluginWebasto::onWriteRequestError);
        connect(webasto, &Webasto::writeRequestExecuted, this, &IntegrationPluginWebasto::onWriteRequestExecuted);

        if (!webasto->connectDevice()) {
            qCWarning(dcWebasto()) << "Could not connect to device";
            info->finish(Thing::ThingErrorSetupFailed);
        }

        info->finish(Thing::ThingErrorNoError);
        return;
    }

    if (thing->thingClassId() == webastoNextThingClassId) {

        // Handle reconfigure
        if (m_webastoNextConnections.contains(thing)) {
            qCDebug(dcWebasto()) << "Reconfiguring existing thing" << thing->name();
            m_webastoNextConnections.take(thing)->deleteLater();

            if (m_monitors.contains(thing)) {
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        }

        // Create the monitor
        NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(thing);
        if (!monitor) {
            qCWarning(dcWebasto()) << "Unable to register monitor with the given params" << thing->params();
            info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("Unable to set up the connection with this configuration, please reconfigure the connection."));
            return;
        }

        m_monitors.insert(thing, monitor);

        QHostAddress address = monitor->networkDeviceInfo().address();
        if (address.isNull()) {
            qCWarning(dcWebasto()) << "Cannot set up thing. The host address is not known yet. Maybe it will be available in the next run...";
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("The host address is not known yet. Trying later again."));
            return;
        }

        // Clean up in case the setup gets aborted
        connect(info, &ThingSetupInfo::aborted, monitor, [=](){
            if (m_monitors.contains(thing)) {
                qCDebug(dcWebasto()) << "Unregister monitor because setup has been aborted.";
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        });

        // If this is the first setup, the monitor must become reachable before we finish the setup
        if (info->isInitialSetup()) {
            // Wait for the monitor to be ready
            if (monitor->reachable()) {
                // Thing already reachable...let's continue with the setup
                setupWebastoNextConnection(info);
            } else {
                qCDebug(dcWebasto()) << "Waiting for the network monitor to get reachable before continue to set up the connection" << thing->name() << address.toString() << "...";
                connect(monitor, &NetworkDeviceMonitor::reachableChanged, info, [=](bool reachable){
                    if (reachable) {
                        qCDebug(dcWebasto()) << "The monitor for thing setup" << thing->name() << "is now reachable. Continue setup...";
                        setupWebastoNextConnection(info);
                    }
                });
            }
        } else {
            // Not the first setup, just add and let the monitor do the check reachable work
            setupWebastoNextConnection(info);
        }

        return;
    }

    if (thing->thingClassId() == webastoUniteThingClassId) {

        if (m_evc04Connections.contains(thing)) {
            qCDebug(dcWebasto()) << "Reconfiguring existing thing" << thing->name();
            m_evc04Connections.take(thing)->deleteLater();

            if (m_monitors.contains(thing)) {
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        }

        NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(thing);
        if (!monitor) {
            qCWarning(dcWebasto()) << "Unable to register monitor with the given params" << thing->params();
            info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("Unable to set up the connection with this configuration, please reconfigure the connection."));
            return;
        }

        m_monitors.insert(thing, monitor);

        connect(info, &ThingSetupInfo::aborted, monitor, [=](){
            if (m_monitors.contains(thing)) {
                qCDebug(dcWebasto()) << "Unregistering monitor because setup has been aborted.";
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        });

        // If this is the first setup, the monitor must become reachable before we finish the setup
        if (info->isInitialSetup()) {
            // Wait for the monitor to be ready
            if (monitor->reachable()) {
                setupEVC04Connection(info);
            } else {
                qCDebug(dcWebasto()) << "Waiting for the network monitor to get reachable before continuing to set up the connection" << thing->name() << "...";
                connect(monitor, &NetworkDeviceMonitor::reachableChanged, info, [=](bool reachable){
                    if (reachable) {
                        qCDebug(dcWebasto()) << "The monitor for thing setup" << thing->name() << "is now reachable. Continuing setup on" << monitor->networkDeviceInfo().address().toString();
                        setupEVC04Connection(info);
                    }
                });
            }
        } else {
            // Not the first setup, just add and let the monitor do the check reachable work
            setupEVC04Connection(info);
        }
        return;
    }

    Q_ASSERT_X(false, "setupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
}

void IntegrationPluginWebasto::postSetupThing(Thing *thing)
{
    qCDebug(dcWebasto()) << "Post setup thing" << thing->name();
    if (!m_pluginTimer) {
        qCDebug(dcWebasto()) << "Setting up refresh timer for Webasto connections";
        m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(2);
        connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {

            foreach(Webasto *connection, m_webastoLiveConnections) {
                if (connection->connected()) {
                    update(connection);
                }
            }

            foreach(WebastoNextModbusTcpConnection *webastoNext, m_webastoNextConnections) {
                if (webastoNext->reachable()) {
                    webastoNext->update();
                }
            }

            foreach(EVC04ModbusTcpConnection *connection, m_evc04Connections) {
                qCDebug(dcWebasto()) << "Updating connection" << connection->modbusTcpMaster()->hostAddress().toString();
                connection->update();
                connection->setAliveRegister(1);
            }

        });

        m_pluginTimer->start();
    }

    if (thing->thingClassId() == webastoLiveThingClassId) {
        Webasto *connection = m_webastoLiveConnections.value(thing);
        update(connection);
        return;
    }

    if (thing->thingClassId() == webastoNextThingClassId) {
        WebastoNextModbusTcpConnection *connection = m_webastoNextConnections.value(thing);
        if (connection->reachable()) {
            thing->setStateValue(webastoNextConnectedStateTypeId, true);
            connection->update();
        } else {
            // We start the connection mechanism only if the monitor says the thing is reachable
            if (m_monitors.value(thing)->reachable()) {
                connection->connectDevice();
            }
        }
        return;
    }
}

void IntegrationPluginWebasto::thingRemoved(Thing *thing)
{
    qCDebug(dcWebasto()) << "Delete thing" << thing->name();

    if (thing->thingClassId() == webastoNextThingClassId) {
        WebastoNextModbusTcpConnection *connection = m_webastoNextConnections.take(thing);
        connection->disconnectDevice();
        connection->deleteLater();
    }

    if (thing->thingClassId() == webastoUniteThingClassId && m_evc04Connections.contains(thing)) {
        EVC04ModbusTcpConnection *connection = m_evc04Connections.take(thing);
        delete connection;
    }


    // Unregister related hardware resources
    if (m_monitors.contains(thing))
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));

    if (m_pluginTimer && myThings().isEmpty()) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}


void IntegrationPluginWebasto::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    if (thing->thingClassId() == webastoLiveThingClassId) {

        Webasto *connection = m_webastoLiveConnections.value(thing);
        if (!connection) {
            qCWarning(dcWebasto()) << "Can't find connection to thing";
            return info->finish(Thing::ThingErrorHardwareNotAvailable);
        }

        if (action.actionTypeId() == webastoLivePowerActionTypeId) {
            bool enabled = action.paramValue(webastoLivePowerActionPowerParamTypeId).toBool();
            thing->setStateValue(webastoLivePowerActionTypeId, enabled);
            int ampere = 0;
            if (enabled) {
                ampere = thing->stateValue(webastoLiveMaxChargingCurrentStateTypeId).toUInt();
            }
            QUuid requestId = connection->setChargeCurrent(ampere);
            if (requestId.isNull()) {
                info->finish(Thing::ThingErrorHardwareFailure);
            } else {
                m_asyncActions.insert(requestId, info);
            }
        } else if (action.actionTypeId() == webastoLiveMaxChargingCurrentActionTypeId) {
            int ampere = action.paramValue(webastoLiveMaxChargingCurrentActionMaxChargingCurrentParamTypeId).toUInt();
            thing->setStateValue(webastoLiveMaxChargingCurrentStateTypeId, ampere);
            QUuid requestId = connection->setChargeCurrent(ampere);
            if (requestId.isNull()) {
                info->finish(Thing::ThingErrorHardwareFailure);
            } else {
                m_asyncActions.insert(requestId, info);
            }
        } else {
            Q_ASSERT_X(false, "executeAction", QString("Unhandled actionTypeId: %1").arg(action.actionTypeId().toString()).toUtf8());
        }

        return;
    }

    if (thing->thingClassId() == webastoNextThingClassId) {

        WebastoNextModbusTcpConnection *connection = m_webastoNextConnections.value(thing);
        if (!connection) {
            qCWarning(dcWebasto()) << "Can't find modbus connection for" << thing;
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        if (!connection->reachable()) {
            qCWarning(dcWebasto()) << "Cannot execute action because the connection of" << thing << "is not reachable.";
            info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The charging station is not reachable."));
            return;
        }

        if (action.actionTypeId() == webastoNextPowerActionTypeId) {
            bool power = action.paramValue(webastoNextPowerActionPowerParamTypeId).toBool();

            // If this action was executed by the user, we start a new session, otherwise we assume it was a some charging logic
            // and we keep the current session.

            if (power && action.triggeredBy() == Action::TriggeredByUser) {
                // First send 0 ChargingActionNoAction before sending 1 start session
                qCDebug(dcWebasto()) << "Enable charging action triggered by user. Restarting the session.";
                QModbusReply *reply = connection->setChargingAction(WebastoNextModbusTcpConnection::ChargingActionNoAction);
                connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
                connect(reply, &QModbusReply::finished, info, [this, info, reply, power](){
                    if (reply->error() == QModbusDevice::NoError) {
                        info->thing()->setStateValue(webastoNextPowerStateTypeId, power);
                        qCDebug(dcWebasto()) << "Restart charging session request finished successfully.";
                        info->finish(Thing::ThingErrorNoError);
                    } else {
                        qCWarning(dcWebasto()) << "Restart charging session request finished with error:" << reply->errorString();
                        info->finish(Thing::ThingErrorHardwareFailure);
                    }

                    // Note: even if "NoAction" failed, we try to send the start charging action and report the error there just in case
                    executeWebastoNextPowerAction(info, power);
                });
            } else {
                executeWebastoNextPowerAction(info, power);
            }
        } else if (action.actionTypeId() == webastoNextMaxChargingCurrentActionTypeId) {
            quint16 chargingCurrent = action.paramValue(webastoNextMaxChargingCurrentActionMaxChargingCurrentParamTypeId).toUInt();
            qCDebug(dcWebasto()) << "Set max charging current of" << thing << "to" << chargingCurrent << "ampere";
            QModbusReply *reply = connection->setChargeCurrent(chargingCurrent);
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, info, [info, reply, chargingCurrent](){
                if (reply->error() == QModbusDevice::NoError) {
                    qCDebug(dcWebasto()) << "Set max charging current finished successfully.";
                    info->thing()->setStateValue(webastoNextMaxChargingCurrentStateTypeId, chargingCurrent);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    qCWarning(dcWebasto()) << "Set max charging current request finished with error:" << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });

        } else {
            Q_ASSERT_X(false, "executeAction", QString("Unhandled actionTypeId: %1").arg(action.actionTypeId().toString()).toUtf8());
        }

        return;
    }

    if (info->thing()->thingClassId() == webastoUniteThingClassId) {

        EVC04ModbusTcpConnection *evc04Connection = m_evc04Connections.value(info->thing());

        if (info->action().actionTypeId() == webastoUnitePowerActionTypeId) {
            bool power = info->action().paramValue(webastoUnitePowerActionPowerParamTypeId).toBool();

            // If the car is *not* connected, writing a 0 to the charging current register will cause it to go to 6 A instead of 0
            // Because of this, we we're not connected, we'll do nothing, but once it get's connected, we'll sync the state over (see below in cableStateChanged)
            if (!power && evc04Connection->cableState() < EVC04ModbusTcpConnection::CableStateCableConnectedVehicleConnected) {
                info->thing()->setStateValue(webastoUnitePowerStateTypeId, false);
                info->finish(Thing::ThingErrorNoError);
                return;
            }

            QModbusReply *reply = evc04Connection->setChargingCurrent(power ? info->thing()->stateValue(webastoUniteMaxChargingCurrentStateTypeId).toUInt() : 0);
            connect(reply, &QModbusReply::finished, info, [info, reply, power](){
                if (reply->error() == QModbusDevice::NoError) {
                    info->thing()->setStateValue(webastoUnitePowerStateTypeId, power);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    qCWarning(dcWebasto()) << "Error setting power:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
        }
        if (info->action().actionTypeId() == webastoUniteMaxChargingCurrentActionTypeId) {
            int maxChargingCurrent = info->action().paramValue(webastoUniteMaxChargingCurrentActionMaxChargingCurrentParamTypeId).toInt();
            QModbusReply *reply = evc04Connection->setChargingCurrent(maxChargingCurrent);
            connect(reply, &QModbusReply::finished, info, [info, reply, maxChargingCurrent](){
                if (reply->error() == QModbusDevice::NoError) {
                    info->thing()->setStateValue(webastoUniteMaxChargingCurrentStateTypeId, maxChargingCurrent);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
        }
        if (info->action().actionTypeId() == webastoUniteDesiredPhaseCountActionTypeId) {
            if (validTokenAvailable(thing)) {
                executeWebastoUnitePhaseCountAction(info);
            } else {
                qCDebug(dcWebasto()) << "HTTP: Authentication required. Update access token for" << thing->name();
                QNetworkReply *loginReply = requestWebstoUniteAccessToken(evc04Connection->modbusTcpMaster()->hostAddress());
                connect(loginReply, &QNetworkReply::finished, evc04Connection, [=](){
                    if (loginReply->error() != QNetworkReply::NoError) {
                        info->finish(Thing::ThingErrorAuthenticationFailure);
                        qCWarning(dcWebasto()) << "HTTP: Authentication request failed for" << evc04Connection->modbusTcpMaster()->hostAddress() << loginReply->error() << loginReply->errorString();
                        return;
                    }

                    QByteArray response = loginReply->readAll();

                    QJsonParseError error;
                    QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &error);
                    if (error.error != QJsonParseError::NoError) {
                        info->finish(Thing::ThingErrorAuthenticationFailure);
                        qCWarning(dcWebasto()) << "HTTP: Authentication response JSON error:" << error.errorString();
                        return;
                    }

                    QVariantMap responseMap = jsonDoc.toVariant().toMap();
                    QString accessToken = responseMap.value("access_token").toString();
                    QDateTime accessTokenExpireDateTime = QDateTime::fromString(responseMap.value("access_token_exp").toString(), Qt::ISODate);

                    QStringList tokenParts = accessToken.split('.');
                    if (tokenParts.length() != 3) {
                        qCWarning(dcWebasto()) << "HTTP: Could not read expiration timestamp. Invalid JWT token formatting. Does not contain 3 parts separated by dot.";
                        return;
                    }

                    qCDebug(dcWebasto()) << "HTTP: Header" << QByteArray::fromBase64(tokenParts.at(0).toUtf8());
                    qCDebug(dcWebasto()) << "HTTP: Payload" << QByteArray::fromBase64(tokenParts.at(1).toUtf8());
                    qCDebug(dcWebasto()) << "HTTP: Signature" << tokenParts.at(2);
                    QJsonDocument payloadJsonDoc = QJsonDocument::fromJson(QByteArray::fromBase64(tokenParts.at(1).toUtf8()));
                    QVariantMap payloadMap = payloadJsonDoc.toVariant().toMap();
                    QDateTime expirationDateTime = QDateTime::fromString(payloadMap.value("access_token_exp").toString(), Qt::ISODate);
                    qCDebug(dcWebasto()) << "HTTP: Token payload:" << qUtf8Printable(payloadJsonDoc.toJson());
                    qCDebug(dcWebasto()) << "HTTP: Token expires:" << expirationDateTime.toString("dd.MM.yyyy hh:mm:ss");
                    qCDebug(dcWebasto()) << "HTTP: Authentication finished successfully. Token:" << accessToken << "Expires:" << accessTokenExpireDateTime.toString("dd.MM.yyyy hh:mm:ss");
                    m_webastoUniteTokens[thing] = QPair<QString, QDateTime>(accessToken, accessTokenExpireDateTime);
                    executeWebastoUnitePhaseCountAction(info);
                });
            }
        }
        return;
    }

    Q_ASSERT_X(false, "executeAction", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
}


void IntegrationPluginWebasto::setupWebastoNextConnection(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    QHostAddress address = m_monitors.value(thing)->networkDeviceInfo().address();
    uint port = thing->paramValue(webastoNextThingPortParamTypeId).toUInt();
    quint16 slaveId = thing->paramValue(webastoNextThingSlaveIdParamTypeId).toUInt();

    qCDebug(dcWebasto()) << "Setting up webasto next connection on" << QString("%1:%2").arg(address.toString()).arg(port) << "slave ID:" << slaveId;
    WebastoNextModbusTcpConnection *webastoNextConnection = new WebastoNextModbusTcpConnection(address, port, slaveId, this);
    webastoNextConnection->modbusTcpMaster()->setTimeout(500);
    webastoNextConnection->modbusTcpMaster()->setNumberOfRetries(3);
    m_webastoNextConnections.insert(thing, webastoNextConnection);
    connect(info, &ThingSetupInfo::aborted, webastoNextConnection, [=](){
        webastoNextConnection->deleteLater();
        m_webastoNextConnections.remove(thing);
    });

    // Reconnect on monitor reachable changed
    NetworkDeviceMonitor *monitor = m_monitors.value(thing);
    connect(monitor, &NetworkDeviceMonitor::reachableChanged, thing, [=](bool reachable){

        if (reachable) {
            qCDebug(dcWebasto()) << "Network device is now reachable for" << thing << monitor->networkDeviceInfo();
        } else {
            qCDebug(dcWebasto()) << "Network device not reachable any more" << thing;
        }

        if (!thing->setupComplete())
            return;

        if (reachable) {
            webastoNextConnection->modbusTcpMaster()->setHostAddress(monitor->networkDeviceInfo().address());
            webastoNextConnection->reconnectDevice();
        } else {
            // Note: We disable autoreconnect explicitly and we will
            // connect the device once the monitor says it is reachable again
            webastoNextConnection->disconnectDevice();
        }
    });

    connect(webastoNextConnection, &WebastoNextModbusTcpConnection::reachableChanged, thing, [thing, webastoNextConnection, monitor](bool reachable){
        qCDebug(dcWebasto()) << "Reachable changed to" << reachable << "for" << thing;
        thing->setStateValue(webastoNextConnectedStateTypeId, reachable);
        if (reachable) {
            // Connected true will be set after successfull init
            webastoNextConnection->update();
        } else {
            thing->setStateValue(webastoNextCurrentPowerStateTypeId, 0);
            thing->setStateValue(webastoNextCurrentPowerPhaseAStateTypeId, 0);
            thing->setStateValue(webastoNextCurrentPowerPhaseBStateTypeId, 0);
            thing->setStateValue(webastoNextCurrentPowerPhaseCStateTypeId, 0);
            thing->setStateValue(webastoNextCurrentPhaseAStateTypeId, 0);
            thing->setStateValue(webastoNextCurrentPhaseBStateTypeId, 0);
            thing->setStateValue(webastoNextCurrentPhaseCStateTypeId, 0);

            if (monitor->reachable()) {
                webastoNextConnection->reconnectDevice();
            }
        }
    });

    connect(webastoNextConnection, &WebastoNextModbusTcpConnection::updateFinished, thing, [thing, webastoNextConnection](){

        // Note: we get the update finished also if all calles failed...
        if (!webastoNextConnection->reachable()) {
            thing->setStateValue(webastoNextConnectedStateTypeId, false);
            return;
        }

        thing->setStateValue(webastoNextConnectedStateTypeId, true);

        qCDebug(dcWebasto()) << "Update finished" << webastoNextConnection;
        // States
        switch (webastoNextConnection->chargeState()) {
        case WebastoNextModbusTcpConnection::ChargeStateIdle:
            thing->setStateValue(webastoNextChargingStateTypeId, false);
            break;
        case WebastoNextModbusTcpConnection::ChargeStateCharging:
            thing->setStateValue(webastoNextChargingStateTypeId, true);
            break;
        }

        switch (webastoNextConnection->chargerState()) {
        case WebastoNextModbusTcpConnection::ChargerStateNoVehicle:
            thing->setStateValue(webastoNextChargingStateTypeId, false);
            thing->setStateValue(webastoNextPluggedInStateTypeId, false);
            break;
        case WebastoNextModbusTcpConnection::ChargerStateVehicleAttachedNoPermission:
            thing->setStateValue(webastoNextPluggedInStateTypeId, true);
            break;
        case WebastoNextModbusTcpConnection::ChargerStateCharging:
            thing->setStateValue(webastoNextChargingStateTypeId, true);
            thing->setStateValue(webastoNextPluggedInStateTypeId, true);
            break;
        case WebastoNextModbusTcpConnection::ChargerStateChargingPaused:
            thing->setStateValue(webastoNextPluggedInStateTypeId, true);
            break;
        default:
            break;
        }

        // Meter values
        thing->setStateValue(webastoNextCurrentPowerPhaseAStateTypeId, webastoNextConnection->activePowerL1());
        thing->setStateValue(webastoNextCurrentPowerPhaseBStateTypeId, webastoNextConnection->activePowerL2());
        thing->setStateValue(webastoNextCurrentPowerPhaseCStateTypeId, webastoNextConnection->activePowerL3());

        double currentPhaseA = webastoNextConnection->currentL1() / 1000.0;
        double currentPhaseB = webastoNextConnection->currentL2() / 1000.0;
        double currentPhaseC = webastoNextConnection->currentL3() / 1000.0;
        thing->setStateValue(webastoNextCurrentPhaseAStateTypeId, currentPhaseA);
        thing->setStateValue(webastoNextCurrentPhaseBStateTypeId, currentPhaseB);
        thing->setStateValue(webastoNextCurrentPhaseCStateTypeId, currentPhaseC);

        // Note: we do not use the active phase power, because we have sometimes a few watts on inactive phases
        Electricity::Phases phases = Electricity::PhaseNone;
        phases.setFlag(Electricity::PhaseA, currentPhaseA > 0);
        phases.setFlag(Electricity::PhaseB, currentPhaseB > 0);
        phases.setFlag(Electricity::PhaseC, currentPhaseC > 0);
        if (phases != Electricity::PhaseNone) {
            thing->setStateValue(webastoNextUsedPhasesStateTypeId, Electricity::convertPhasesToString(phases));
            thing->setStateValue(webastoNextPhaseCountStateTypeId, Electricity::getPhaseCount(phases));
        }


        thing->setStateValue(webastoNextCurrentPowerStateTypeId, webastoNextConnection->totalActivePower());

        thing->setStateValue(webastoNextTotalEnergyConsumedStateTypeId, webastoNextConnection->energyConsumed() / 1000.0);
        thing->setStateValue(webastoNextSessionEnergyStateTypeId, webastoNextConnection->sessionEnergy() / 1000.0);

        // Min / Max charging current^
        thing->setStateValue(webastoNextMinCurrentTotalStateTypeId, webastoNextConnection->minChargingCurrent());
        thing->setStateValue(webastoNextMaxCurrentTotalStateTypeId, webastoNextConnection->maxChargingCurrent());
        thing->setStateMinValue(webastoNextMaxChargingCurrentStateTypeId, webastoNextConnection->minChargingCurrent());
        thing->setStateMaxValue(webastoNextMaxChargingCurrentStateTypeId, webastoNextConnection->maxChargingCurrent());

        thing->setStateValue(webastoNextMaxCurrentChargerStateTypeId, webastoNextConnection->maxChargingCurrentStation());
        thing->setStateValue(webastoNextMaxCurrentCableStateTypeId, webastoNextConnection->maxChargingCurrentCable());
        thing->setStateValue(webastoNextMaxCurrentElectricVehicleStateTypeId, webastoNextConnection->maxChargingCurrentEv());

        if (webastoNextConnection->evseErrorCode() == 0) {
            thing->setStateValue(webastoNextErrorStateTypeId, "");
        } else {
            uint errorCode = webastoNextConnection->evseErrorCode() - 1;
            switch (errorCode) {
            case 1:
                // Note: also PB61 has the same mapping and the same reason for the error.
                // We inform only about the PB02 since it does not make any difference regarding the action
                thing->setStateValue(webastoNextErrorStateTypeId, "PB02 - PowerSwitch Failure");
                break;
            case 2:
                thing->setStateValue(webastoNextErrorStateTypeId, "PB07 - InternalError (Aux Voltage)");
                break;
            case 3:
                thing->setStateValue(webastoNextErrorStateTypeId, "PB09 - EV Communication Error");
                break;
            case 4:
                thing->setStateValue(webastoNextErrorStateTypeId, "PB17 - OverVoltage");
                break;
            case 5:
                thing->setStateValue(webastoNextErrorStateTypeId, "PB18 - UnderVoltage");
                break;
            case 6:
                thing->setStateValue(webastoNextErrorStateTypeId, "PB23 - OverCurrent Failure");
                break;
            case 7:
                thing->setStateValue(webastoNextErrorStateTypeId, "PB24 - OtherError");
                break;
            case 8:
                thing->setStateValue(webastoNextErrorStateTypeId, "PB27 - GroundFailure");
                break;
            case 9:
                thing->setStateValue(webastoNextErrorStateTypeId, "PB28 - InternalError (Selftest)");
                break;
            case 10:
                thing->setStateValue(webastoNextErrorStateTypeId, "PB29 - High Temperature");
                break;
            case 11:
                thing->setStateValue(webastoNextErrorStateTypeId, "PB52 - Proximity Pilot Error");
                break;
            case 12:
                thing->setStateValue(webastoNextErrorStateTypeId, "PB53 - Shutter Error");
                break;
            case 13:
                thing->setStateValue(webastoNextErrorStateTypeId, "PB57 - Error Three Phase Check");
                break;
            case 14:
                thing->setStateValue(webastoNextErrorStateTypeId, "PB59 - PWR internal error");
                break;
            case 15:
                thing->setStateValue(webastoNextErrorStateTypeId, "PB60 - EV Communication Error - Negative control pilot voltage");
                break;
            case 16:
                thing->setStateValue(webastoNextErrorStateTypeId, "PB62- DC residual current (Vehicle)");
                break;
            default:
                thing->setStateValue(webastoNextErrorStateTypeId, QString("Unknwon error code %1").arg(errorCode));
                break;
            }
        }

        // Handle life bit (keep alive mechanism if there is a HEMS activated)
        if (webastoNextConnection->lifeBit() == 0) {
            // Let's reset the life bit so the wallbox knows we are still here,
            // otherwise the wallbox goes into the failsave mode and limits the charging to the configured
            QModbusReply *reply = webastoNextConnection->setLifeBit(1);
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, webastoNextConnection, [reply, webastoNextConnection](){
                if (reply->error() == QModbusDevice::NoError) {
                    qCDebug(dcWebasto()) << "Resetted life bit watchdog on" << webastoNextConnection << "finished successfully";
                } else {
                    qCWarning(dcWebasto()) << "Resetted life bit watchdog on" << webastoNextConnection << "finished with error:" << reply->errorString();
                }
            });
        }
    });

    connect(thing, &Thing::settingChanged, webastoNextConnection, [webastoNextConnection](const ParamTypeId &paramTypeId, const QVariant &value){
        if (paramTypeId == webastoNextSettingsCommunicationTimeoutParamTypeId) {
            QModbusReply *reply = webastoNextConnection->setComTimeout(value.toUInt());
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, webastoNextConnection, [reply, webastoNextConnection, value](){
                if (reply->error() == QModbusDevice::NoError) {
                    qCDebug(dcWebasto()) << "Setting communication timout to" << value.toUInt() << "on" << webastoNextConnection << "finished successfully.";
                } else {
                    qCWarning(dcWebasto()) << "Setting communication timout to" << value.toUInt() << "on" << webastoNextConnection << "finished with error:" << reply->errorString();
                    if (webastoNextConnection->reachable()) {
                        webastoNextConnection->updateComTimeout();
                    }
                }
            });
        } else if (paramTypeId == webastoNextSettingsSafeCurrentParamTypeId) {
            QModbusReply *reply = webastoNextConnection->setSafeCurrent(value.toUInt());
            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, webastoNextConnection, [reply, webastoNextConnection, value](){
                if (reply->error() == QModbusDevice::NoError) {
                    qCDebug(dcWebasto()) << "Setting save current to" << value.toUInt() << "on" << webastoNextConnection << "finished successfully.";
                } else {
                    qCWarning(dcWebasto()) << "Setting save current to" << value.toUInt() << "on" << webastoNextConnection << "finished with error:" << reply->errorString();
                    if (webastoNextConnection->reachable()) {
                        webastoNextConnection->updateSafeCurrent();
                    }
                }
            });
        } else {
            qCWarning(dcWebasto()) << "Unhandled setting changed for" << webastoNextConnection;
        }
    });

    connect(webastoNextConnection, &WebastoNextModbusTcpConnection::comTimeoutChanged, thing, [thing](quint16 comTimeout){
        thing->setSettingValue(webastoNextSettingsCommunicationTimeoutParamTypeId, comTimeout);
    });

    connect(webastoNextConnection, &WebastoNextModbusTcpConnection::safeCurrentChanged, thing, [thing](quint16 safeCurrent){
        thing->setSettingValue(webastoNextSettingsSafeCurrentParamTypeId, safeCurrent);
    });

    qCInfo(dcWebasto()) << "Setup finished successfully for Webasto NEXT" << thing << monitor;
    info->finish(Thing::ThingErrorNoError);
}

void IntegrationPluginWebasto::update(Webasto *webasto)
{
    webasto->getRegister(Webasto::TqChargePointState);
    webasto->getRegister(Webasto::TqCableState);
    webasto->getRegister(Webasto::TqEVSEError);

    webasto->getRegister(Webasto::TqCurrentL1);
    webasto->getRegister(Webasto::TqCurrentL2);
    webasto->getRegister(Webasto::TqCurrentL3);

    webasto->getRegister(Webasto::TqActivePower, 2);
    webasto->getRegister(Webasto::TqEnergyMeter, 2);

    webasto->getRegister(Webasto::TqMaxCurrent);

    webasto->getRegister(Webasto::TqChargedEnergy);
    webasto->getRegister(Webasto::TqChargingTime, 2);

    webasto->getRegister(Webasto::TqUserId, 10);
}

void IntegrationPluginWebasto::evaluatePhaseCount(Thing *thing)
{
    uint amperePhase1 = thing->stateValue(webastoLiveCurrentPhase1StateTypeId).toUInt();
    uint amperePhase2 = thing->stateValue(webastoLiveCurrentPhase2StateTypeId).toUInt();
    uint amperePhase3 = thing->stateValue(webastoLiveCurrentPhase3StateTypeId).toUInt();
    // Check how many phases are actually charging, and update the phase count only if something happens on the phases (current or power)
    if (!(amperePhase1 == 0 && amperePhase2 == 0 && amperePhase3 == 0)) {
        uint phaseCount = 0;
        if (amperePhase1 != 0)
            phaseCount += 1;

        if (amperePhase2 != 0)
            phaseCount += 1;

        if (amperePhase3 != 0)
            phaseCount += 1;

        thing->setStateValue(webastoLivePhaseCountStateTypeId, phaseCount);
    }
}

void IntegrationPluginWebasto::executeWebastoNextPowerAction(ThingActionInfo *info, bool power)
{
    qCDebug(dcWebasto()) << (power ? "Enabling": "Disabling") << "charging on" << info->thing();

    WebastoNextModbusTcpConnection *connection = m_webastoNextConnections.value(info->thing());
    QModbusReply *reply = nullptr;
    if (power) {
        reply = connection->setChargingAction(WebastoNextModbusTcpConnection::ChargingActionStartSession);
    } else {
        reply = connection->setChargingAction(WebastoNextModbusTcpConnection::ChargingActionCancelSession);
    }

    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
    connect(reply, &QModbusReply::finished, info, [info, reply, power](){
        if (reply->error() == QModbusDevice::NoError) {
            info->thing()->setStateValue(webastoNextPowerStateTypeId, power);
            qCDebug(dcWebasto()) << "Enabling/disabling charging request finished successfully.";
            info->finish(Thing::ThingErrorNoError);
        } else {
            qCWarning(dcWebasto()) << "Enabling/disabling charging request finished with error:" << reply->errorString();
            info->finish(Thing::ThingErrorHardwareFailure);
        }
    });
}

void IntegrationPluginWebasto::onWriteRequestExecuted(const QUuid &requestId, bool success)
{
    if (m_asyncActions.contains(requestId)) {
        ThingActionInfo *info = m_asyncActions.take(requestId);
        if (success) {
            info->finish(Thing::ThingErrorNoError);
        } else {
            info->finish(Thing::ThingErrorHardwareFailure);
        }
    }
}

void IntegrationPluginWebasto::onWriteRequestError(const QUuid &requestId, const QString &error)
{
    Q_UNUSED(requestId);
    qCWarning(dcWebasto()) << "Write request error" << error;
}

void IntegrationPluginWebasto::onReceivedRegister(Webasto::TqModbusRegister modbusRegister, const QVector<quint16> &data)
{
    Webasto *connection = static_cast<Webasto *>(sender());
    Thing *thing = m_webastoLiveConnections.key(connection);
    if (!thing) {
        qCWarning(dcWebasto()) << "On basic information received, thing not found for connection";
        return;
    }
    if (thing->thingClassId() == webastoLiveThingClassId) {
        switch (modbusRegister) {
        case Webasto::TqChargePointState:
            qCDebug(dcWebasto()) << "   - Charge point state:" << Webasto::ChargePointState(data[0]);
            switch (Webasto::ChargePointState(data[0])) {
            case Webasto::ChargePointStateNoVehicleAttached:
                thing->setStateValue(webastoLiveChargePointStateStateTypeId, "No vehicle attached");
                break;
            case Webasto::ChargePointStateVehicleAttachedNoPermission:
                thing->setStateValue(webastoLiveChargePointStateStateTypeId, "Vehicle attached, no permission");
                break;
            case Webasto::ChargePointStateChargingAuthorized:
                thing->setStateValue(webastoLiveChargePointStateStateTypeId, "Charging authorized");
                break;
            case Webasto::ChargePointStateCharging:
                thing->setStateValue(webastoLiveChargePointStateStateTypeId, "Charging");
                break;
            case Webasto::ChargePointStateChargingPaused:
                thing->setStateValue(webastoLiveChargePointStateStateTypeId, "Charging paused");
                break;
            case Webasto::ChargePointStateChargeSuccessfulCarStillAttached:
                thing->setStateValue(webastoLiveChargePointStateStateTypeId, "Charge successful (car still attached)");
                break;
            case Webasto::ChargePointStateChargingStoppedByUserCarStillAttached:
                thing->setStateValue(webastoLiveChargePointStateStateTypeId, "Charging stopped by user (car still attached)");
                break;
            case Webasto::ChargePointStateChargingErrorCarStillAttached:
                thing->setStateValue(webastoLiveChargePointStateStateTypeId,  "Charging error (car still attached)");
                break;
            case Webasto::ChargePointStateChargingStationReservedNorCarAttached:
                thing->setStateValue(webastoLiveChargePointStateStateTypeId, "Charging station reserved (No car attached)");
                break;
            case Webasto::ChargePointStateUserNotAuthorizedCarAttached:
                thing->setStateValue(webastoLiveChargePointStateStateTypeId, "User not authorized (car attached)");
                break;
            }

            thing->setStateValue(webastoLiveChargingStateTypeId, Webasto::ChargePointState(data[0]) == Webasto::ChargePointStateCharging);
            break;
        case Webasto::TqChargeState:
            qCDebug(dcWebasto()) << "   - Charge state:" << data[0];
            break;
        case Webasto::TqEVSEState:
            qCDebug(dcWebasto()) << "   - EVSE state:" << data[0];
            break;
        case Webasto::TqCableState:
            qCDebug(dcWebasto()) << "   - Cable state:" << Webasto::CableState(data[0]);
            switch (Webasto::CableState(data[0])) {
            case Webasto::CableStateNoCableAttached:
                thing->setStateValue(webastoLiveCableStateStateTypeId, "No cable attached");
                thing->setStateValue(webastoLivePluggedInStateTypeId, false);
                break;
            case Webasto::CableStateCableAttachedNoCarAttached:
                thing->setStateValue(webastoLiveCableStateStateTypeId, "Cable attached but no car attached)");
                thing->setStateValue(webastoLivePluggedInStateTypeId, false);
                break;
            case Webasto::CableStateCableAttachedCarAttached:
                thing->setStateValue(webastoLiveCableStateStateTypeId, "Cable attached and car attached");
                thing->setStateValue(webastoLivePluggedInStateTypeId, true);
                break;
            case Webasto::CableStateCableAttachedCarAttachedLockActive:
                thing->setStateValue(webastoLiveCableStateStateTypeId, "Cable attached, car attached and lock active");
                thing->setStateValue(webastoLivePluggedInStateTypeId, true);
                break;
            }
            break;
        case Webasto::TqEVSEError:
            qCDebug(dcWebasto()) << "   - EVSE error:" << data[0];
            thing->setStateValue(webastoLiveErrorStateTypeId, data[0]);
            break;
        case Webasto::TqCurrentL1:
            qCDebug(dcWebasto()) << "   - Current L1:" << data[0];
            thing->setStateValue(webastoLiveCurrentPhase1StateTypeId, data[0]);
            evaluatePhaseCount(thing);
            break;
        case Webasto::TqCurrentL2:
            qCDebug(dcWebasto()) << "   - Current L2:" << data[0];
            thing->setStateValue(webastoLiveCurrentPhase2StateTypeId, data[0]);
            evaluatePhaseCount(thing);
            break;
        case Webasto::TqCurrentL3:
            qCDebug(dcWebasto()) << "   - Current L3:" << data[0];
            thing->setStateValue(webastoLiveCurrentPhase3StateTypeId, data[0]);
            evaluatePhaseCount(thing);
            break;
        case Webasto::TqActivePower: {
            if (data.length() < 2)
                return;
            int power = (static_cast<quint32>(data[0])<<16 | data[1]);
            qCDebug(dcWebasto()) << "   - Active power:" << power;
            thing->setStateValue(webastoLiveCurrentPowerStateTypeId, power);
        } break;
        case Webasto::TqEnergyMeter: {
            if (data.length() < 2)
                return;
            int energy = (static_cast<quint32>(data[0])<<16 | data[1]);
            qCDebug(dcWebasto()) << "   - Energy meter:" << energy << "Wh";
            thing->setStateValue(webastoLiveTotalEnergyConsumedStateTypeId, energy);
        } break;
        case Webasto::TqMaxCurrent:
            qCDebug(dcWebasto()) << "   - Max. Current" << data[0];
            thing->setStateValue(webastoLiveMaxPossibleChargingCurrentStateTypeId, data[0]);
            break;
        case Webasto::TqMinimumCurrentLimit:
            qCDebug(dcWebasto()) << "   - Min. Current" << data[0];
            break;
        case Webasto::TqMaxCurrentFromEVSE:
            qCDebug(dcWebasto()) << "   - Max. Current EVSE" << data[0];
            break;
        case Webasto::TqMaxCurrentFromCable:
            qCDebug(dcWebasto()) << "   - Max. Current Cable" << data[0];
            break;
        case Webasto::TqMaxCurrentFromEV:
            qCDebug(dcWebasto()) << "   - Max. Current EV" << data[0];
            break;
        case Webasto::TqUserPriority:
            qCDebug(dcWebasto()) << "   - User priority" << data[0];
            break;
        case Webasto::TqEVBatteryState:
            qCDebug(dcWebasto()) << "   - Battery state" << data[0];
            break;
        case Webasto::TqEVBatteryCapacity: {
            if (data.length() < 2)
                return;
            uint batteryCapacity = (static_cast<quint32>(data[0])<<16 | data[1]);
            qCDebug(dcWebasto()) << "   - Battery capacity" << batteryCapacity << "Wh";
        } break;
        case Webasto::TqScheduleType:
            qCDebug(dcWebasto()) << "   - Schedule type" << data[0];
            break;
        case Webasto::TqRequiredEnergy: {
            if (data.length() < 2)
                return;
            uint requiredEnergy = (static_cast<quint32>(data[0])<<16 | data[1]);
            qCDebug(dcWebasto()) << "   - Required energy" << requiredEnergy;
        } break;
        case Webasto::TqRequiredBatteryState:
            qCDebug(dcWebasto()) << "   - Required battery state" << data[0];
            break;
        case Webasto::TqScheduledTime:
            qCDebug(dcWebasto()) << "   - Scheduled time" << data[0];
            break;
        case Webasto::TqScheduledDate:
            qCDebug(dcWebasto()) << "   - Scheduled date" << data[0];
            break;
        case Webasto::TqChargedEnergy:
            qCDebug(dcWebasto()) << "   - Charged energy" << data[0];
            thing->setStateValue(webastoLiveSessionEnergyStateTypeId, data[0]/1000.00); // Charged energy in kWh
            break;
        case Webasto::TqStartTime:
            qCDebug(dcWebasto()) << "   - Start time" << (static_cast<quint32>(data[0])<<16 | data[1]);
            break;
        case Webasto::TqChargingTime: {
            if (data.length() < 2)
                return;
            uint seconds = (static_cast<quint32>(data[0])<<16 | data[1]);
            qCDebug(dcWebasto()) << "   - Charging time" << seconds << "s";
            thing->setStateValue(webastoLiveSessionTimeStateTypeId, seconds/60.00); // Charging time in minutes
        } break;
        case Webasto::TqEndTime: {
            if (data.length() < 2)
                return;
            uint hour =    ((static_cast<quint32>(data[0])<<16 | data[1])&0xff0000)>>16;
            uint minutes = ((static_cast<quint32>(data[0])<<16 | data[1])&0x00ff00)>>8;
            uint seconds=  (static_cast<quint32>(data[0])<<16 | data[1])&0x0000ff;
            qCDebug(dcWebasto()) << "   - End time" << hour << "h" << minutes << "m" << seconds << "s";
        } break;
        case Webasto::TqUserId: {
            if (data.length() < 10)
                return;
            QByteArray userID;
            Q_FOREACH(quint16 i, data) {
                userID.append(i>>16);
                userID.append(i&0xff);
            }
            qCDebug(dcWebasto()) << "   - User ID:" << userID;
        } break;
        case Webasto::TqSmartVehicleDetected:
            qCDebug(dcWebasto()) << "   - Smart vehicle detected:" << data[0];
            break;
        case Webasto::TqSafeCurrent:
            qCDebug(dcWebasto()) << "   - Safe current:" << data[0];
            break;
        case Webasto::TqComTimeout:
            qCDebug(dcWebasto()) << "   - Com timeout:" << data[0];
            break;
        default:
            break;
        }
    }
}

void IntegrationPluginWebasto::setupEVC04Connection(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    QHostAddress address = m_monitors.value(thing)->networkDeviceInfo().address();

    EVC04ModbusTcpConnection *evc04Connection = new EVC04ModbusTcpConnection(address, 502, 0xff, this);
    connect(info, &ThingSetupInfo::aborted, evc04Connection, &EVC04ModbusTcpConnection::deleteLater);

    // Reconnect on monitor reachable changed
    NetworkDeviceMonitor *monitor = m_monitors.value(thing);
    connect(monitor, &NetworkDeviceMonitor::reachableChanged, thing, [=](bool reachable){
        qCDebug(dcWebasto()) << "Network device monitor reachable changed for" << thing->name() << reachable;
        if (!thing->setupComplete())
            return;

        if (reachable && !thing->stateValue("connected").toBool()) {
            evc04Connection->modbusTcpMaster()->setHostAddress(monitor->networkDeviceInfo().address());
            evc04Connection->connectDevice();
        } else if (!reachable) {
            // Note: We disable autoreconnect explicitly and we will
            // connect the device once the monitor says it is reachable again
            evc04Connection->disconnectDevice();
        }
    });

    connect(evc04Connection, &EVC04ModbusTcpConnection::reachableChanged, thing, [thing, evc04Connection](bool reachable){
        qCDebug(dcWebasto()) << "Reachable changed to" << reachable << "for" << thing;
        if (reachable) {
            evc04Connection->initialize();
        } else {
            thing->setStateValue(webastoUniteConnectedStateTypeId, false);
            thing->setStateValue(webastoUniteCurrentPowerStateTypeId, 0);
        }
    });

    connect(evc04Connection, &EVC04ModbusTcpConnection::initializationFinished, thing, [=](bool success){
        if (!thing->setupComplete())
            return;

        if (success) {
            thing->setStateValue(webastoUniteConnectedStateTypeId, true);
        } else {
            thing->setStateValue(webastoUniteConnectedStateTypeId, false);
            thing->setStateValue(webastoUniteCurrentPowerStateTypeId, 0);

            // Try once to reconnect the device
            evc04Connection->reconnectDevice();
        }
    });

    connect(evc04Connection, &EVC04ModbusTcpConnection::initializationFinished, info, [=](bool success){
        if (!success) {
            qCWarning(dcWebasto()) << "Connection init finished with errors" << thing->name() << evc04Connection->modbusTcpMaster()->hostAddress().toString();
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(monitor);
            evc04Connection->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("Error communicating with the wallbox."));
            return;
        }

        qCDebug(dcWebasto()) << "Connection init finished successfully" << evc04Connection;

        m_evc04Connections.insert(thing, evc04Connection);
        info->finish(Thing::ThingErrorNoError);

        thing->setStateValue(webastoUniteConnectedStateTypeId, true);
        thing->setStateValue(webastoUniteVersionStateTypeId, QString(QString::fromUtf16(evc04Connection->firmwareVersion().data(), evc04Connection->firmwareVersion().length()).toUtf8()).trimmed());

        evc04Connection->update();
    });

    connect(evc04Connection, &EVC04ModbusTcpConnection::updateFinished, thing, [this, evc04Connection, thing](){
        qCDebug(dcWebasto()) << "EVC04 update finished:" << thing->name() << evc04Connection;
        qCDebug(dcWebasto()) << "Serial:" << QString(QString::fromUtf16(evc04Connection->serialNumber().data(), evc04Connection->serialNumber().length()).toUtf8()).trimmed();
        qCDebug(dcWebasto()) << "ChargePoint ID:" << QString(QString::fromUtf16(evc04Connection->chargepointId().data(), evc04Connection->chargepointId().length()).toUtf8()).trimmed();
        qCDebug(dcWebasto()) << "Brand:" << QString(QString::fromUtf16(evc04Connection->brand().data(), evc04Connection->brand().length()).toUtf8()).trimmed();
        qCDebug(dcWebasto()) << "Model:" << QString(QString::fromUtf16(evc04Connection->model().data(), evc04Connection->model().length()).toUtf8()).trimmed();

        updateEVC04MaxCurrent(thing);

        // I've been observing the wallbox getting stuck on modbus. It is still functional, but modbus keeps on returning the same old values
        // until the TCP connection is closed and reopened. Checking the wallbox time register to detect that and auto-reconnect.
        if (m_lastWallboxTime.contains(thing) && m_lastWallboxTime[thing] == evc04Connection->time()) {
            qCWarning(dcWebasto()) << "Wallbox seems stuck and returning outdated values. Reconnecting...";
            evc04Connection->disconnectDevice();
            QTimer::singleShot(1000, evc04Connection, &EVC04ModbusTcpConnection::reconnectDevice);
        } else {
            m_lastWallboxTime[thing] = evc04Connection->time();
        }
    });

    connect(evc04Connection, &EVC04ModbusTcpConnection::chargepointStateChanged, thing, [thing](EVC04ModbusTcpConnection::ChargePointState chargePointState) {
        qCDebug(dcWebasto()) << "Chargepoint state changed" << thing->name() << chargePointState;
        // switch (chargePointState) {
        // case EVC04ModbusTcpConnection::ChargePointStateAvailable:
        // case EVC04ModbusTcpConnection::ChargePointStatePreparing:
        // case EVC04ModbusTcpConnection::ChargePointStateReserved:
        // case EVC04ModbusTcpConnection::ChargePointStateUnavailable:
        // case EVC04ModbusTcpConnection::ChargePointStateFaulted:
        //     thing->setStateValue(evc04PluggedInStateTypeId, false);
        //     break;
        // case EVC04ModbusTcpConnection::ChargePointStateCharging:
        // case EVC04ModbusTcpConnection::ChargePointStateSuspendedEVSE:
        // case EVC04ModbusTcpConnection::ChargePointStateSuspendedEV:
        // case EVC04ModbusTcpConnection::ChargePointStateFinishing:
        //     thing->setStateValue(evc04PluggedInStateTypeId, true);
        //     break;
        // }
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::chargingStateChanged, thing, [thing](EVC04ModbusTcpConnection::ChargingState chargingState) {
        qCDebug(dcWebasto()) << "Charging state changed:" << chargingState;
        thing->setStateValue(webastoUniteChargingStateTypeId, chargingState == EVC04ModbusTcpConnection::ChargingStateCharging);
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::activePowerTotalChanged, thing, [thing](quint16 activePowerTotal) {
        qCDebug(dcWebasto()) << "Total active power:" << activePowerTotal;
        // The wallbox reports some 5-6W even when there's nothing connected. Let's hide that if we're not charging
        if (thing->stateValue(webastoUniteChargingStateTypeId).toBool() == true) {
            thing->setStateValue(webastoUniteCurrentPowerStateTypeId, activePowerTotal);
        } else {
            thing->setStateValue(webastoUniteCurrentPowerStateTypeId, 0);
        }
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::meterReadingChanged, thing, [thing](quint32 meterReading) {
        qCDebug(dcWebasto()) << "Meter reading changed:" << meterReading;
        thing->setStateValue(webastoUniteTotalEnergyConsumedStateTypeId, meterReading / 10.0);
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::sessionMaxCurrentChanged, thing, [](quint16 sessionMaxCurrent) {
        // This mostly just reflects what we've been writing to cargingCurrent, so not of much use...
        qCDebug(dcWebasto()) << "Session max current changed:" << sessionMaxCurrent;
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::cableMaxCurrentChanged, thing, [this, thing](quint16 cableMaxCurrent) {
        qCDebug(dcWebasto()) << "Cable max current changed:" << cableMaxCurrent;
        updateEVC04MaxCurrent(thing);
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::evseMinCurrentChanged, thing, [thing](quint16 evseMinCurrent) {
        qCDebug(dcWebasto()) << "EVSE min current changed:" << evseMinCurrent;
        thing->setStateMinValue(webastoUniteMaxChargingCurrentStateTypeId, evseMinCurrent);
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::evseMaxCurrentChanged, thing, [this, thing](quint16 evseMaxCurrent) {
        qCDebug(dcWebasto()) << "EVSE max current changed:" << evseMaxCurrent;
        updateEVC04MaxCurrent(thing);
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::sessionEnergyChanged, thing, [thing](quint32 sessionEnergy) {
        qCDebug(dcWebasto()) << "Session energy changed:" << sessionEnergy;
        thing->setStateValue(webastoUniteSessionEnergyStateTypeId, sessionEnergy / 1000.0);
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::chargingCurrentChanged, thing, [thing](quint16 chargingCurrent) {
        qCDebug(dcWebasto()) << "Charging current changed:" << chargingCurrent;
        if (chargingCurrent > 0) {
            thing->setStateValue(webastoUnitePowerStateTypeId, true);
            thing->setStateValue(webastoUniteMaxChargingCurrentStateTypeId, chargingCurrent);
        } else {
            thing->setStateValue(webastoUnitePowerStateTypeId, false);
        }
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::numPhasesChanged, thing, [thing](EVC04ModbusTcpConnection::NumPhases numPhases) {
        switch (numPhases) {
        case EVC04ModbusTcpConnection::NumPhases1:
            thing->setStateValue(webastoUnitePhaseCountStateTypeId, 1);
            break;
        case EVC04ModbusTcpConnection::NumPhases3:
            thing->setStateValue(webastoUnitePhaseCountStateTypeId, 3);
            break;
        }
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::cableStateChanged, thing, [evc04Connection, thing](EVC04ModbusTcpConnection::CableState cableState) {
        switch (cableState) {
        case EVC04ModbusTcpConnection::CableStateNotConnected:
        case EVC04ModbusTcpConnection::CableStateCableConnectedVehicleNotConnected:
            thing->setStateValue(webastoUnitePluggedInStateTypeId, false);
            break;
        case EVC04ModbusTcpConnection::CableStateCableConnectedVehicleConnected:
        case EVC04ModbusTcpConnection::CableStateCableConnectedVehicleConnectedCableLocked:
            thing->setStateValue(webastoUnitePluggedInStateTypeId, true);

            // The car was plugged in, sync the power state now as the wallbox only allows to set that when the car is connected
            if (thing->stateValue(webastoUnitePowerStateTypeId).toBool() == false) {
                qCInfo(dcWebasto()) << "Car plugged in. Syncing cached power off state to wallbox";
                evc04Connection->setChargingCurrent(0);
            }

            break;
        }
    });

    evc04Connection->connectDevice();
}

void IntegrationPluginWebasto::updateEVC04MaxCurrent(Thing *thing)
{
    EVC04ModbusTcpConnection *connection = m_evc04Connections.value(thing);
    quint16 wallboxMax = connection->maxChargePointPower() > 0 ? connection->maxChargePointPower() / 230 : 32;
    quint16 evseMax = connection->evseMaxCurrent() > 0 ? connection->evseMaxCurrent() : wallboxMax;
    quint16 cableMax = connection->cableMaxCurrent() > 0 ? connection->cableMaxCurrent() : wallboxMax;

    quint8 overallMax = qMin(qMin(wallboxMax, evseMax), cableMax);
    qCDebug(dcWebasto()) << "Adjusting max current: Wallbox max:" << wallboxMax << "EVSE max:" << evseMax << "cable max:" << cableMax << "Overall:" << overallMax;
    thing->setStateMinMaxValues(webastoUniteMaxChargingCurrentStateTypeId, 6, overallMax);
}

bool IntegrationPluginWebasto::validTokenAvailable(Thing *thing)
{
    if (m_webastoUniteTokens.contains(thing)) {
        QPair<QString, QDateTime> tokenInfo = m_webastoUniteTokens.value(thing);
        if (!tokenInfo.first.isEmpty() && tokenInfo.second > QDateTime::currentDateTimeUtc().addSecs(60)) {
            qCDebug(dcWebasto()) << "HTTP: Valid access token found for" << thing->name();
            return true;
        } else {
            qCDebug(dcWebasto()) << "HTTP: Token need to be refreshed. The current token for" << thing->name() << "is expired:" << tokenInfo.second.toString("dd.MM.yyyy hh:mm:ss") << QDateTime::currentDateTimeUtc().toString();

        }
    } else {
        qCDebug(dcWebasto()) << "HTTP: Token need to be refreshed. There is no token for" << thing->name();
    }

    return false;
}

QNetworkReply *IntegrationPluginWebasto::requestWebstoUniteAccessToken(const QHostAddress &address)
{
    // Note: these credentials are documented in the Websto Unite installation manual and also provided using QR code.
    QVariantMap requestMap;
    requestMap.insert("username", "admin");
    requestMap.insert("password", "0#54&8eV%c+e2y(P2%h0");

    QJsonDocument jsonDoc = QJsonDocument::fromVariant(requestMap);

    QUrl url;
    url.setScheme("https"); // we have to use ssl and ignore the endpoint error
    url.setHost(address.toString());
    url.setPath("/api/login");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    qCDebug(dcWebasto()) << "HTTP: Requesting access token" << url.toString() << qUtf8Printable(jsonDoc.toJson(QJsonDocument::Compact));;
    QNetworkReply *reply = hardwareManager()->networkManager()->post(request, QJsonDocument::fromVariant(requestMap).toJson());
    connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
    connect(reply, &QNetworkReply::sslErrors, this, [reply](const QList<QSslError> &){
        // We ignore SSL errors in the LAN... quiet useless against MITM attacks
        reply->ignoreSslErrors();
    });

    return reply;
}

QNetworkReply *IntegrationPluginWebasto::requestWebstoUnitePhaseCountChange(const QHostAddress &address, const QString &accessToken, uint desiredPhaseCount)
{
    QVariantList settingsList;
    QVariantMap settingMap;
    settingMap.insert("fieldKey", "installationSettings.currentLimiterPhase");
    settingMap.insert("value", QString("%1").arg(desiredPhaseCount == 3 ? 1 : 0));
    settingsList.append(settingMap);

    QJsonDocument jsonDoc = QJsonDocument::fromVariant(settingsList);

    QUrl url;
    url.setScheme("https"); // we have to use ssl and ignore the endpoint error
    url.setHost(address.toString());
    url.setPath("/api/configuration-updates");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + accessToken.toLocal8Bit());
    qCDebug(dcWebasto()) << "HTTP: Requesting phase count change" << url.toString() << qUtf8Printable(jsonDoc.toJson(QJsonDocument::Compact));

    QNetworkReply *reply = hardwareManager()->networkManager()->post(request, jsonDoc.toJson());
    connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
    connect(reply, &QNetworkReply::sslErrors, this, [reply](const QList<QSslError> &){
        // We ignore SSL errors in the LAN... quiet useless against MITM attacks
        reply->ignoreSslErrors();
    });

    return reply;
}

void IntegrationPluginWebasto::executeWebastoUnitePhaseCountAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    quint8 desiredPhaseCount = info->action().paramValue(webastoUniteDesiredPhaseCountActionDesiredPhaseCountParamTypeId).toUInt();

    QNetworkReply *reply = requestWebstoUnitePhaseCountChange(m_evc04Connections.value(thing)->modbusTcpMaster()->hostAddress(), m_webastoUniteTokens.value(thing).first, desiredPhaseCount);
    connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
    connect(reply, &QNetworkReply::finished, info, [=](){
        if (reply->error() != QNetworkReply::NoError) {
            qCWarning(dcWebasto()) << "HTTP: Error setting desired phase count:" << reply->error() << reply->errorString();
            info->finish(Thing::ThingErrorHardwareFailure);
            return;
        }

        QByteArray response = reply->readAll();

        QJsonParseError error;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &error);
        if (error.error != QJsonParseError::NoError) {
            info->finish(Thing::ThingErrorAuthenticationFailure);
            qCWarning(dcWebasto()) << "HTTP: Set desired phase count response JSON error:" << error.errorString();
            return;
        }

        qCDebug(dcWebasto()) << "HTTP: Response:" << qUtf8Printable(jsonDoc.toJson(QJsonDocument::Compact));

        QVariantMap responseMap = jsonDoc.toVariant().toMap();
        if (responseMap.value("status").toString() != "SUCCESS") {
            info->finish(Thing::ThingErrorHardwareFailure);
            qCWarning(dcWebasto()) << "HTTP: Could not set desired phase count for" << thing->name();
            return;
        }

        qCDebug(dcWebasto()) << "HTTP: Set webasto unite phase count to" << desiredPhaseCount << "finished successfully.";
        thing->setStateValue(webastoUniteDesiredPhaseCountStateTypeId, desiredPhaseCount);
        info->finish(Thing::ThingErrorNoError);
    });
}
