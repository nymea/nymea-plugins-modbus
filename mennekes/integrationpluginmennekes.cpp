/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2022, nymea GmbH
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

#include "integrationpluginmennekes.h"
#include "plugininfo.h"
#include "amtronecudiscovery.h"
#include "amtronhcc3discovery.h"

#include <network/networkdevicediscovery.h>
#include <hardwaremanager.h>

IntegrationPluginMennekes::IntegrationPluginMennekes()
{

}

void IntegrationPluginMennekes::discoverThings(ThingDiscoveryInfo *info)
{
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcMennekes()) << "The network discovery is not available on this platform.";
        info->finish(Thing::ThingErrorUnsupportedFeature, QT_TR_NOOP("The network device discovery is not available."));
        return;
    }

    if (info->thingClassId() == amtronECUThingClassId) {
        AmtronECUDiscovery *discovery = new AmtronECUDiscovery(hardwareManager()->networkDeviceDiscovery(), info);
        connect(discovery, &AmtronECUDiscovery::discoveryFinished, info, [=](){
            foreach (const AmtronECUDiscovery::Result &result, discovery->discoveryResults()) {

                QString name = "AMTRON Charge Control/Professional";
                QString description = result.model.isEmpty() ? result.networkDeviceInfo.address().toString() :
                                                               result.model + " (" + result.networkDeviceInfo.address().toString() + ")";
                if (result.model.startsWith("CC")) {
                    name = "AMTRON Charge Control";
                } else if (result.model.startsWith("P")) {
                    name = "AMTRON Professional";
                } else {
                    qCWarning(dcMennekes()) << "Unknown Amtron model:" << result.model;
                    continue;
                }
                ThingDescriptor descriptor(amtronECUThingClassId, name, description);
                qCDebug(dcMennekes()) << "Discovered:" << descriptor.title() << descriptor.description();

                // Check if we already have set up this device
                Things existingThings = myThings().filterByParam(amtronECUThingMacAddressParamTypeId, result.networkDeviceInfo.macAddress());
                if (existingThings.count() == 1) {
                    qCDebug(dcMennekes()) << "This wallbox already exists in the system:" << result.networkDeviceInfo;
                    descriptor.setThingId(existingThings.first()->id());
                }

                ParamList params;
                params << Param(amtronECUThingMacAddressParamTypeId, result.networkDeviceInfo.macAddress());
                // Note: if we discover also the port and modbusaddress, we must fill them in from the discovery here, for now everywhere the defaults...
                descriptor.setParams(params);
                info->addThingDescriptor(descriptor);
            }

            info->finish(Thing::ThingErrorNoError);
        });
        discovery->startDiscovery();

    } else if (info->thingClassId() == amtronHCC3ThingClassId) {
        AmtronHCC3Discovery *discovery = new AmtronHCC3Discovery(hardwareManager()->networkDeviceDiscovery(), info);
        connect(discovery, &AmtronHCC3Discovery::discoveryFinished, info, [=](){
            foreach (const AmtronHCC3Discovery::AmtronDiscoveryResult &result, discovery->discoveryResults()) {

                if (result.serialNumber.isEmpty()) {
                    qCWarning(dcMennekes()) << "Unable to read Amtron serial number:" << result.serialNumber << result.wallboxName;
                    continue;
                }

                ThingDescriptor descriptor(amtronHCC3ThingClassId, result.wallboxName, "Serial: " + result.serialNumber + " - " + result.networkDeviceInfo.address().toString());
                qCDebug(dcMennekes()) << "Discovered:" << descriptor.title() << descriptor.description();

                // Check if we already have set up this device
                Things existingThings = myThings().filterByParam(amtronHCC3ThingMacAddressParamTypeId, result.networkDeviceInfo.macAddress());
                if (existingThings.count() == 1) {
                    qCDebug(dcMennekes()) << "This wallbox already exists in the system:" << result.networkDeviceInfo;
                    descriptor.setThingId(existingThings.first()->id());
                }


                ParamList params;
                params << Param(amtronHCC3ThingMacAddressParamTypeId, result.networkDeviceInfo.macAddress());
                // Note: if we discover also the port and modbusaddress, we must fill them in from the discovery here, for now everywhere the defaults...
                descriptor.setParams(params);
                info->addThingDescriptor(descriptor);
            }

            info->finish(Thing::ThingErrorNoError);
        });
        discovery->startDiscovery();
    }
}

void IntegrationPluginMennekes::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcMennekes()) << "Setup" << thing << thing->params();

    if (thing->thingClassId() == amtronECUThingClassId) {

        if (m_amtronECUConnections.contains(thing)) {
            qCDebug(dcMennekes()) << "Reconfiguring existing thing" << thing->name();
            m_amtronECUConnections.take(thing)->deleteLater();

            if (m_monitors.contains(thing)) {
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        }

        MacAddress macAddress = MacAddress(thing->paramValue(amtronECUThingMacAddressParamTypeId).toString());
        if (!macAddress.isValid()) {
            qCWarning(dcMennekes()) << "The configured mac address is not valid" << thing->params();
            info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The MAC address is not known. Please reconfigure the thing."));
            return;
        }

        NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(macAddress);
        m_monitors.insert(thing, monitor);

        QHostAddress address = monitor->networkDeviceInfo().address();
        if (address.isNull()) {
            qCWarning(dcMennekes()) << "Cannot set up thing. The host address is not known yet...";
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("The host address is not known yet. Trying later again."));
            return;
        }

        connect(info, &ThingSetupInfo::aborted, monitor, [=](){
            if (m_monitors.contains(thing)) {
                qCDebug(dcMennekes()) << "Unregistering monitor because setup has been aborted.";
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        });

        if (monitor->reachable()) {
            setupAmtronECUConnection(info);
        } else {
            qCDebug(dcMennekes()) << "Waiting for the network monitor to get reachable before continue to set up the connection" << thing->name() << address.toString() << "...";
            connect(monitor, &NetworkDeviceMonitor::reachableChanged, info, [=](bool reachable){
                if (reachable) {
                    qCDebug(dcMennekes()) << "The monitor for thing setup" << thing->name() << "is now reachable. Continue setup...";
                    setupAmtronECUConnection(info);
                }
            });
        }

        return;
    }

    if (info->thing()->thingClassId() == amtronHCC3ThingClassId) {
        if (m_amtronHCC3Connections.contains(thing)) {
            qCDebug(dcMennekes()) << "Reconfiguring existing thing" << thing->name();
            m_amtronHCC3Connections.take(thing)->deleteLater();

            if (m_monitors.contains(thing)) {
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        }

        MacAddress macAddress = MacAddress(thing->paramValue(amtronHCC3ThingMacAddressParamTypeId).toString());
        if (!macAddress.isValid()) {
            qCWarning(dcMennekes()) << "The configured mac address is not valid" << thing->params();
            info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The MAC address is not known. Please reconfigure the thing."));
            return;
        }

        NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(macAddress);
        m_monitors.insert(thing, monitor);

        QHostAddress address = monitor->networkDeviceInfo().address();
        if (address.isNull()) {
            qCWarning(dcMennekes()) << "Cannot set up thing. The host address is not known yet...";
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("The host address is not known yet. Trying later again."));
            return;
        }

        connect(info, &ThingSetupInfo::aborted, monitor, [=](){
            if (m_monitors.contains(thing)) {
                qCDebug(dcMennekes()) << "Unregistering monitor because setup has been aborted.";
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        });

        if (monitor->reachable()) {
            setupAmtronHCC3Connection(info);
        } else {
            qCDebug(dcMennekes()) << "Waiting for the network monitor to get reachable before continue to set up the connection" << thing->name() << address.toString() << "...";
            connect(monitor, &NetworkDeviceMonitor::reachableChanged, info, [=](bool reachable){
                if (reachable) {
                    qCDebug(dcMennekes()) << "The monitor for thing setup" << thing->name() << "is now reachable. Continue setup...";
                    setupAmtronHCC3Connection(info);
                }
            });
        }

        return;

    }
}

void IntegrationPluginMennekes::postSetupThing(Thing *thing)
{
    Q_UNUSED(thing)
    if (!m_pluginTimer) {
        qCDebug(dcMennekes()) << "Starting plugin timer...";
        m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(2);
        connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
            foreach(AmtronECUModbusTcpConnection *connection, m_amtronECUConnections) {
                qCDebug(dcMennekes()) << "Updating connection" << connection->hostAddress().toString();
                connection->update();
            }
            foreach(AmtronHCC3ModbusTcpConnection *connection, m_amtronHCC3Connections) {
                qCDebug(dcMennekes()) << "Updating connection" << connection->hostAddress().toString();
                connection->update();
            }
        });

        m_pluginTimer->start();
    }
}

void IntegrationPluginMennekes::executeAction(ThingActionInfo *info)
{
    if (info->thing()->thingClassId() == amtronECUThingClassId) {
        AmtronECUModbusTcpConnection *amtronECUConnection = m_amtronECUConnections.value(info->thing());

        if (info->action().actionTypeId() == amtronECUPowerActionTypeId) {
            bool power = info->action().paramValue(amtronECUPowerActionPowerParamTypeId).toBool();
            QModbusReply *reply = amtronECUConnection->setHemsCurrentLimit(power ? info->thing()->stateValue(amtronECUMaxChargingCurrentStateTypeId).toUInt() : 0);
            connect(reply, &QModbusReply::finished, info, [info, reply, power](){
                if (reply->error() == QModbusDevice::NoError) {
                    info->thing()->setStateValue(amtronECUPowerStateTypeId, power);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    qCWarning(dcMennekes()) << "Error setting cp availability:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
        }
        if (info->action().actionTypeId() == amtronECUMaxChargingCurrentActionTypeId) {
            int maxChargingCurrent = info->action().paramValue(amtronECUMaxChargingCurrentActionMaxChargingCurrentParamTypeId).toInt();
            QModbusReply *reply = amtronECUConnection->setHemsCurrentLimit(maxChargingCurrent);
            connect(reply, &QModbusReply::finished, info, [info, reply, maxChargingCurrent](){
                if (reply->error() == QModbusDevice::NoError) {
                    info->thing()->setStateValue(amtronECUMaxChargingCurrentStateTypeId, maxChargingCurrent);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
        }
    }
}

void IntegrationPluginMennekes::thingRemoved(Thing *thing)
{
    if (thing->thingClassId() == amtronECUThingClassId && m_amtronECUConnections.contains(thing)) {
        AmtronECUModbusTcpConnection *connection = m_amtronECUConnections.take(thing);
        delete connection;
    }

    if (thing->thingClassId() == amtronHCC3ThingClassId && m_amtronHCC3Connections.contains(thing)) {
        AmtronHCC3ModbusTcpConnection *connection = m_amtronHCC3Connections.take(thing);
        delete connection;
    }

    // Unregister related hardware resources
    if (m_monitors.contains(thing))
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));

    if (myThings().isEmpty() && m_pluginTimer) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginMennekes::updateECUPhaseCount(Thing *thing)
{
    AmtronECUModbusTcpConnection *amtronECUConnection = m_amtronECUConnections.value(thing);
    int phaseCount = 0;
    qCDebug(dcMennekes()) << "Phases: L1" << amtronECUConnection->meterCurrentL1() << "L2" << amtronECUConnection->meterCurrentL2() << "L3" << amtronECUConnection->meterCurrentL3();
    // the current idles on some 5 - 10 mA when not charging...
    // We want to detect the phases we're actually charging on. Checking the current flow for that if it's > 500mA
    // If no phase is charging, let's count all phases that are not 0 instead (to determine how many phases are connected at the wallbox)

    if (amtronECUConnection->meterCurrentL1() > 500) {
        phaseCount++;
    }
    if (amtronECUConnection->meterCurrentL2() > 500) {
        phaseCount++;
    }
    if (amtronECUConnection->meterCurrentL3() > 500) {
        phaseCount++;
    }
    qCDebug(dcMennekes()) << "Actively charging phases:" << phaseCount;
    if (phaseCount == 0) {
        if (amtronECUConnection->meterCurrentL1() > 0) {
            phaseCount++;
        }
        if (amtronECUConnection->meterCurrentL2() > 0) {
            phaseCount++;
        }
        if (amtronECUConnection->meterCurrentL3() > 0) {
            phaseCount++;
        }
        qCDebug(dcMennekes()) << "Connected phases:" << phaseCount;
    }

    thing->setStateValue(amtronECUPhaseCountStateTypeId, phaseCount);
}

void IntegrationPluginMennekes::setupAmtronECUConnection(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    QHostAddress address = m_monitors.value(thing)->networkDeviceInfo().address();

    qCDebug(dcMennekes()) << "Setting up amtron wallbox on" << address.toString();
    AmtronECUModbusTcpConnection *amtronECUConnection = new AmtronECUModbusTcpConnection(address, 502, 0xff, this);
    connect(info, &ThingSetupInfo::aborted, amtronECUConnection, &ModbusTCPMaster::deleteLater);

    // Reconnect on monitor reachable changed
    NetworkDeviceMonitor *monitor = m_monitors.value(thing);
    connect(monitor, &NetworkDeviceMonitor::reachableChanged, thing, [=](bool reachable){
        qCDebug(dcMennekes()) << "Network device monitor reachable changed for" << thing->name() << reachable;
        if (!thing->setupComplete())
            return;

        if (reachable && !thing->stateValue("connected").toBool()) {
            amtronECUConnection->setHostAddress(monitor->networkDeviceInfo().address());
            amtronECUConnection->connectDevice();
        } else if (!reachable) {
            // Note: We disable autoreconnect explicitly and we will
            // connect the device once the monitor says it is reachable again
            amtronECUConnection->disconnectDevice();
        }
    });

    connect(amtronECUConnection, &AmtronECUModbusTcpConnection::reachableChanged, thing, [thing, amtronECUConnection](bool reachable){
        qCDebug(dcMennekes()) << "Reachable changed to" << reachable << "for" << thing;
        if (reachable) {
            amtronECUConnection->initialize();
        } else {
            thing->setStateValue(amtronECUConnectedStateTypeId, false);
        }
    });

    connect(amtronECUConnection, &AmtronECUModbusTcpConnection::initializationFinished, thing, [=](bool success){
        if (!thing->setupComplete())
            return;

        if (success) {
            thing->setStateValue(amtronECUConnectedStateTypeId, true);
        } else {
            thing->setStateValue(amtronECUConnectedStateTypeId, false);
            // Try once to reconnect the device
            amtronECUConnection->reconnectDevice();
        }
    });

    connect(amtronECUConnection, &AmtronECUModbusTcpConnection::initializationFinished, info, [=](bool success){
        if (!success) {
            qCWarning(dcMennekes()) << "Connection init finished with errors" << thing->name() << amtronECUConnection->hostAddress().toString();
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(monitor);
            amtronECUConnection->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("Error communicating with the wallbox."));
            return;
        }

        qCDebug(dcMennekes()) << "Connection init finished successfully" << amtronECUConnection;

        QString minimumVersion = "5.22";
        if (!ensureAmtronECUVersion(amtronECUConnection, minimumVersion)) {
            qCWarning(dcMennekes()) << "Firmware version too old:" << QByteArray::fromHex(QByteArray::number(amtronECUConnection->firmwareVersion(), 16)) << "Minimum required:" << minimumVersion;
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(monitor);
            amtronECUConnection->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("The firmware version of this wallbox is too old. Please upgrade the firmware to at least version 5.22."));
            return;
        }

        m_amtronECUConnections.insert(thing, amtronECUConnection);
        info->finish(Thing::ThingErrorNoError);

        thing->setStateValue(amtronECUConnectedStateTypeId, true);

        amtronECUConnection->update();
    });

    connect(amtronECUConnection, &AmtronECUModbusTcpConnection::updateFinished, thing, [this, amtronECUConnection, thing](){
        qCDebug(dcMennekes()) << "Amtron ECU update finished:" << thing->name() << amtronECUConnection;
        updateECUPhaseCount(thing);
    });

    connect(amtronECUConnection, &AmtronECUModbusTcpConnection::cpSignalStateChanged, thing, [thing](AmtronECUModbusTcpConnection::CPSignalState cpSignalState) {
        qCDebug(dcMennekes()) << "CP signal state changed" << cpSignalState;
        if (cpSignalState == AmtronECUModbusTcpConnection::CPSignalStateE) {
            // State E (Off): don't update as the wallbox goes to this state for a few seconds regardless of the actual plugged in state.
            return;
        }
        thing->setStateValue(amtronECUPluggedInStateTypeId, cpSignalState >= AmtronECUModbusTcpConnection::CPSignalStateB);
    });
    connect(amtronECUConnection, &AmtronECUModbusTcpConnection::signalledCurrentChanged, thing, [](quint16 signalledCurrent) {
        qCDebug(dcMennekes()) << "Signalled current changed:" << signalledCurrent;
    });
    connect(amtronECUConnection, &AmtronECUModbusTcpConnection::minCurrentLimitChanged, thing, [thing](quint16 minCurrentLimit) {
        qCDebug(dcMennekes()) << "min current limit changed:" << minCurrentLimit;
        thing->setStateMinValue(amtronECUMaxChargingCurrentStateTypeId, minCurrentLimit);
    });
    connect(amtronECUConnection, &AmtronECUModbusTcpConnection::maxCurrentLimitChanged, thing, [this, thing](quint16 maxCurrentLimit) {
        qCDebug(dcMennekes()) << "max current limit changed:" << maxCurrentLimit;
        // If the vehicle or cable are not capable of reporting the maximum, this will be 0
        // We'll reset to the max defined in the json file in that case
        if (maxCurrentLimit == 0) {
            maxCurrentLimit = supportedThings().findById(amtronECUThingClassId).stateTypes().findById(amtronECUMaxChargingCurrentStateTypeId).maxValue().toUInt();
        }
        thing->setStateMaxValue(amtronECUMaxChargingCurrentStateTypeId, maxCurrentLimit);
    });
    connect(amtronECUConnection, &AmtronECUModbusTcpConnection::hemsCurrentLimitChanged, thing, [thing](quint16 hemsCurrentLimit) {
        qCDebug(dcMennekes()) << "HEMS current limit changed:" << hemsCurrentLimit;
        if (hemsCurrentLimit == 0) {
            thing->setStateValue(amtronECUPowerStateTypeId, false);
        } else {
            thing->setStateValue(amtronECUPowerStateTypeId, true);
            thing->setStateValue(amtronECUMaxChargingCurrentStateTypeId, hemsCurrentLimit);
        }
    });
    connect(amtronECUConnection, &AmtronECUModbusTcpConnection::meterTotoalEnergyChanged, thing, [thing](quint32 meterTotalEnergy) {
        qCDebug(dcMennekes()) << "meter total energy changed:" << meterTotalEnergy;
        thing->setStateValue(amtronECUTotalEnergyConsumedStateTypeId, qRound(meterTotalEnergy / 10.0) / 100.0); // rounded to 2 as it changes on every update
    });
    connect(amtronECUConnection, &AmtronECUModbusTcpConnection::meterTotalPowerChanged, thing, [thing](quint32 meterTotalPower) {
        qCDebug(dcMennekes()) << "meter total power changed:" << meterTotalPower;
        thing->setStateValue(amtronECUCurrentPowerStateTypeId, meterTotalPower);
        thing->setStateValue(amtronECUChargingStateTypeId, meterTotalPower > 0);
    });
    connect(amtronECUConnection, &AmtronECUModbusTcpConnection::chargedEnergyChanged, thing, [thing](quint32 chargedEnergy) {
        qCDebug(dcMennekes()) << "charged energy changed:" << chargedEnergy;
        thing->setStateValue(amtronECUSessionEnergyStateTypeId, qRound(chargedEnergy / 10.0) / 100.0); // rounded to 2 as it changes on every update
    });

    amtronECUConnection->connectDevice();
}

void IntegrationPluginMennekes::setupAmtronHCC3Connection(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    QHostAddress address = m_monitors.value(thing)->networkDeviceInfo().address();

    qCDebug(dcMennekes()) << "Setting up amtron wallbox on" << address.toString();
    AmtronHCC3ModbusTcpConnection *amtronHCC3Connection = new AmtronHCC3ModbusTcpConnection(address, 502, 0xff, this);
    connect(info, &ThingSetupInfo::aborted, amtronHCC3Connection, &ModbusTCPMaster::deleteLater);

    // Reconnect on monitor reachable changed
    NetworkDeviceMonitor *monitor = m_monitors.value(thing);
    connect(monitor, &NetworkDeviceMonitor::reachableChanged, thing, [=](bool reachable){
        qCDebug(dcMennekes()) << "Network device monitor reachable changed for" << thing->name() << reachable;
        if (!thing->setupComplete())
            return;

        if (reachable && !thing->stateValue("connected").toBool()) {
            amtronHCC3Connection->setHostAddress(monitor->networkDeviceInfo().address());
            amtronHCC3Connection->connectDevice();
        } else if (!reachable) {
            // Note: We disable autoreconnect explicitly and we will
            // connect the device once the monitor says it is reachable again
            amtronHCC3Connection->disconnectDevice();
        }
    });

    connect(amtronHCC3Connection, &AmtronHCC3ModbusTcpConnection::reachableChanged, thing, [thing, amtronHCC3Connection](bool reachable){
        qCDebug(dcMennekes()) << "Reachable changed to" << reachable << "for" << thing;
        if (reachable) {
            amtronHCC3Connection->initialize();
        } else {
            thing->setStateValue(amtronHCC3ConnectedStateTypeId, false);
        }
    });

    connect(amtronHCC3Connection, &AmtronHCC3ModbusTcpConnection::initializationFinished, thing, [=](bool success){
        if (!thing->setupComplete())
            return;

        if (success) {
            thing->setStateValue(amtronHCC3ConnectedStateTypeId, true);
        } else {
            thing->setStateValue(amtronHCC3ConnectedStateTypeId, false);
            // Try once to reconnect the device
            amtronHCC3Connection->reconnectDevice();
        }
    });

    connect(amtronHCC3Connection, &AmtronHCC3ModbusTcpConnection::initializationFinished, info, [=](bool success){
        if (!success) {
            qCWarning(dcMennekes()) << "Connection init finished with errors" << thing->name() << amtronHCC3Connection->hostAddress().toString();
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(monitor);
            amtronHCC3Connection->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("Error communicating with the wallbox."));
            return;
        }

        qCDebug(dcMennekes()) << "Connection init finished successfully" << amtronHCC3Connection;
        m_amtronHCC3Connections.insert(thing, amtronHCC3Connection);
        info->finish(Thing::ThingErrorNoError);

        thing->setStateValue(amtronHCC3ConnectedStateTypeId, true);

        amtronHCC3Connection->update();
    });

    amtronHCC3Connection->connectDevice();
}

bool IntegrationPluginMennekes::ensureAmtronECUVersion(AmtronECUModbusTcpConnection *connection, const QString &version)
{
    QByteArray deviceVersion = QByteArray::fromHex(QByteArray::number(connection->firmwareVersion(), 16));
    return deviceVersion >= version;
}
