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

#include "integrationplugininro.h"
#include "plugininfo.h"

#include <network/networkdevicediscovery.h>
#include <hardwaremanager.h>
#include <hardware/electricity.h>

#include "pantaboxdiscovery.h"

IntegrationPluginInro::IntegrationPluginInro()
{

}

void IntegrationPluginInro::discoverThings(ThingDiscoveryInfo *info)
{

    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcInro()) << "The network discovery is not available on this platform.";
        info->finish(Thing::ThingErrorUnsupportedFeature, QT_TR_NOOP("The network device discovery is not available."));
        return;
    }

    PantaboxDiscovery *discovery = new PantaboxDiscovery(hardwareManager()->networkDeviceDiscovery(), info);
    connect(discovery, &PantaboxDiscovery::discoveryFinished, info, [this, info, discovery](){

        foreach (const PantaboxDiscovery::Result &result, discovery->results()) {
            QString title = QString("PANTABOX - %1").arg(result.serialNumber);
            QString description = QString("%1 (%2)").arg(result.networkDeviceInfo.macAddress(), result.networkDeviceInfo.address().toString());
            ThingDescriptor descriptor(pantaboxThingClassId, title, description);

            // Check if we already have set up this device
            Things existingThings = myThings().filterByParam(pantaboxThingMacAddressParamTypeId, result.networkDeviceInfo.macAddress());
            if (existingThings.count() == 1) {
                qCDebug(dcInro()) << "This PANTABOX already exists in the system:" << result.networkDeviceInfo;
                descriptor.setThingId(existingThings.first()->id());
            }

            ParamList params;
            params << Param(pantaboxThingMacAddressParamTypeId, result.networkDeviceInfo.macAddress());
            params << Param(pantaboxThingSerialNumberParamTypeId, result.serialNumber);
            descriptor.setParams(params);
            info->addThingDescriptor(descriptor);
        }

        info->finish(Thing::ThingErrorNoError);
    });

    discovery->startDiscovery();
}

void IntegrationPluginInro::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcInro()) << "Setup thing" << thing << thing->params();

    if (m_connections.contains(thing)) {
        qCDebug(dcInro()) << "Reconfiguring existing thing" << thing->name();
        m_connections.take(thing)->deleteLater();

        if (m_monitors.contains(thing)) {
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
        }
    }

    MacAddress macAddress = MacAddress(thing->paramValue(pantaboxThingMacAddressParamTypeId).toString());
    if (!macAddress.isValid()) {
        qCWarning(dcInro()) << "The configured mac address is not valid" << thing->params();
        info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The MAC address is not known. Please reconfigure the thing."));
        return;
    }

    NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(macAddress);
    m_monitors.insert(thing, monitor);

    connect(info, &ThingSetupInfo::aborted, monitor, [=](){
        if (m_monitors.contains(thing)) {
            qCDebug(dcInro()) << "Unregistering monitor because setup has been aborted.";
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
        }
    });

    // Only make sure the connection is working in the initial setup, otherwise we let the monitor do the work
    if (info->isInitialSetup()) {
        // Continue with setup only if we know that the network device is reachable
        if (monitor->reachable()) {
            setupConnection(info);
        } else {
            // otherwise wait until we reach the networkdevice before setting up the device
            qCDebug(dcInro()) << "Network device" << thing->name() << "is not reachable yet. Continue with the setup once reachable.";
            connect(monitor, &NetworkDeviceMonitor::reachableChanged, info, [=](bool reachable){
                if (reachable) {
                    qCDebug(dcInro()) << "Network device" << thing->name() << "is now reachable. Continue with the setup...";
                    setupConnection(info);
                }
            });
        }
    } else {
        setupConnection(info);
    }
}

void IntegrationPluginInro::postSetupThing(Thing *thing)
{
    qCDebug(dcInro()) << "Post setup thing" << thing->name();
    if (!m_refreshTimer) {
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(2);
        connect(m_refreshTimer, &PluginTimer::timeout, this, [this] {
            foreach (Pantabox *connection, m_connections) {
                if (connection->reachable()) {
                    connection->update();
                }
            }
        });

        qCDebug(dcInro()) << "Starting refresh timer...";
        m_refreshTimer->start();
    }
}

void IntegrationPluginInro::executeAction(ThingActionInfo *info)
{
    if (info->thing()->thingClassId() == pantaboxThingClassId) {

        Pantabox *connection = m_connections.value(info->thing());

        if (!connection->reachable()) {
            qCWarning(dcInro()) << "Cannot execute action. The PANTABOX is not reachable";
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        if (info->action().actionTypeId() == pantaboxPowerActionTypeId) {
            bool power = info->action().paramValue(pantaboxPowerActionPowerParamTypeId).toBool();

            if (info->action().triggeredBy() == Action::TriggeredByUser) {

                // When power is set by user, charging is going to stop or start depending on setting
                qCDebug(dcInro()) << "PANTABOX: Set power by user" << (power ? 1 : 0);
                QModbusReply *reply = connection->setChargingEnabled(power ? 1 : 0);

                if (!reply) {
                    qCWarning(dcInro()) << "Execute action failed because the reply could not be created.";
                    info->finish(Thing::ThingErrorHardwareFailure);
                    return;
                }

                connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
                connect(reply, &QModbusReply::finished, info, [info, reply, power](){
                    if (reply->error() == QModbusDevice::NoError) {
                        info->thing()->setStateValue(pantaboxPowerStateTypeId, power);
                        qCDebug(dcInro()) << "PANTABOX: Set power by user finished successfully";
                        info->finish(Thing::ThingErrorNoError);
                    } else {
                        qCWarning(dcInro()) << "Error setting power by user:" << reply->error() << reply->errorString();
                        info->finish(Thing::ThingErrorHardwareFailure);
                    }
                });
                return;
            } else {

                // When power is set to 0 by energy manager, max charging current is set to 0 otherwise take the configured max charging current
                qCDebug(dcInro()) << "PANTABOX: Pause session by energy manager";

                quint16 chargingCurrent = power ? info->action().paramValue(pantaboxMaxChargingCurrentActionMaxChargingCurrentParamTypeId).toUInt() : 0;
                QModbusReply *reply = connection->setMaxChargingCurrent(chargingCurrent);

                if (!reply) {
                    qCWarning(dcInro()) << "Execute action failed because the reply could not be created.";
                    info->finish(Thing::ThingErrorHardwareFailure);
                    return;
                }

                connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
                connect(reply, &QModbusReply::finished, info, [info, reply, chargingCurrent](){
                    if (reply->error() == QModbusDevice::NoError) {
                        info->thing()->setStateValue(pantaboxMaxChargingCurrentStateTypeId, chargingCurrent);
                        qCDebug(dcInro()) << "PANTABOX: Set max charging current finished successfully";
                        info->finish(Thing::ThingErrorNoError);
                    } else {
                        qCWarning(dcInro()) << "Error setting charging current:" << reply->error() << reply->errorString();
                        info->finish(Thing::ThingErrorHardwareFailure);
                    }
                });
                return;
            }
        }

        if (info->action().actionTypeId() == pantaboxMaxChargingCurrentActionTypeId) {
            quint16 chargingCurrent = info->action().paramValue(pantaboxMaxChargingCurrentActionMaxChargingCurrentParamTypeId).toUInt();
            qCDebug(dcInro()) << "PANTABOX: Set max charging current" << chargingCurrent << "A";

            QModbusReply *reply = connection->setMaxChargingCurrent(chargingCurrent);
            if (!reply) {
                qCWarning(dcInro()) << "Execute action failed because the reply could not be created.";
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }

            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
            connect(reply, &QModbusReply::finished, info, [info, reply, chargingCurrent](){
                if (reply->error() == QModbusDevice::NoError) {
                    info->thing()->setStateValue(pantaboxMaxChargingCurrentStateTypeId, chargingCurrent);
                    qCDebug(dcInro()) << "PANTABOX: Set max charging current finished successfully";
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    qCWarning(dcInro()) << "Error setting charging current:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
            return;
        }
    }
}

void IntegrationPluginInro::thingRemoved(Thing *thing)
{
    qCDebug(dcInro()) << "Thing removed" << thing->name();

    if (m_connections.contains(thing)) {
        Pantabox *connection = m_connections.take(thing);
        connection->disconnectDevice();
        connection->deleteLater();
    }

    // Unregister related hardware resources
    if (m_monitors.contains(thing))
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));

    if (myThings().isEmpty() && m_refreshTimer) {
        qCDebug(dcInro()) << "Stopping reconnect timer";
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_refreshTimer);
        m_refreshTimer = nullptr;
    }
}

void IntegrationPluginInro::setupConnection(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    NetworkDeviceMonitor *monitor = m_monitors.value(thing);

    Pantabox *connection = new Pantabox(monitor->networkDeviceInfo().address(), 502, 1, this);
    connect(info, &ThingSetupInfo::aborted, connection, &Pantabox::deleteLater);

    // Monitor reachability
    connect(monitor, &NetworkDeviceMonitor::reachableChanged, thing, [=](bool reachable){
        if (!thing->setupComplete())
            return;

        qCDebug(dcInro()) << "Network device monitor for" << thing->name() << (reachable ? "is now reachable" : "is not reachable any more" );
        if (reachable && !thing->stateValue("connected").toBool()) {
            connection->modbusTcpMaster()->setHostAddress(monitor->networkDeviceInfo().address());
            connection->connectDevice();
        } else if (!reachable) {
            // Note: We disable autoreconnect explicitly and we will
            // connect the device once the monitor says it is reachable again
            connection->disconnectDevice();
        }
    });

    // Connection reachability
    connect(connection, &Pantabox::reachableChanged, thing, [thing, connection](bool reachable){
        qCInfo(dcInro()) << "Reachable changed to" << reachable << "for" << thing;
        thing->setStateValue("connected", reachable);

        if (!reachable) {
            // Reset energy live values on disconnected
            thing->setStateValue(pantaboxCurrentPowerStateTypeId, 0);
        } else {
            thing->setStateValue(pantaboxModbusTcpVersionStateTypeId, PantaboxDiscovery::modbusVersionToString(connection->modbusTcpVersion()));
        }
    });

    connect(connection, &Pantabox::updateFinished, thing, [thing, connection](){
        qCDebug(dcInro()) << "Update finished for" << thing;
        qCDebug(dcInro()) << connection;

        QString chargingStateString;
        switch(connection->chargingState()) {
        case Pantabox::ChargingStateA:
            chargingStateString = "A";
            break;
        case Pantabox::ChargingStateB:
            chargingStateString = "B";
            break;
        case Pantabox::ChargingStateC:
            chargingStateString = "C";
            break;
        case Pantabox::ChargingStateD:
            chargingStateString = "D";
            break;
        case Pantabox::ChargingStateE:
            chargingStateString = "E";
            break;
        case Pantabox::ChargingStateF:
            chargingStateString = "F";
            break;
        }
        thing->setStateValue(pantaboxChargingStateStateTypeId, chargingStateString);

        // A: not connected
        // B: connected, not charging
        // C: connected, charging
        // D: ventilation required
        // E: F: fault/error
        thing->setStateValue(pantaboxPluggedInStateTypeId, connection->chargingState() >= Pantabox::ChargingStateB);
        thing->setStateValue(pantaboxChargingStateTypeId, connection->chargingState() >= Pantabox::ChargingStateC);
        thing->setStateValue(pantaboxCurrentPowerStateTypeId, connection->currentPower()); // W
        thing->setStateValue(pantaboxTotalEnergyConsumedStateTypeId, connection->chargedEnergy() / 1000.0); // Wh
        thing->setStateMaxValue(pantaboxMaxChargingCurrentActionTypeId, connection->maxPossibleChargingCurrent());

        // Phase count is a setting, since we don't get the information from the device.
        // Maybe we could assume the from current power and set charging current how many phases get used,
        // but we could not tell which pashes are active.

        Electricity::Phases phases = Electricity::convertPhasesFromString(thing->setting(pantaboxSettingsPhasesParamTypeId).toString());
        thing->setStateValue(pantaboxPhaseCountStateTypeId, Electricity::getPhaseCount(phases));
        thing->setStateValue(pantaboxUsedPhasesStateTypeId, thing->setting(pantaboxSettingsPhasesParamTypeId).toString());

    });

    m_connections.insert(thing, connection);
    info->finish(Thing::ThingErrorNoError);

    qCDebug(dcInro()) << "Setting up PANTABOX finished successfully" << monitor->networkDeviceInfo().address().toString();

    // Connect reight the way if the monitor indicates reachable, otherwise the connect will handle the connect later
    if (monitor->reachable())
        connection->connectDevice();
}
