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
#include "amtroncompact20discovery.h"

#include <network/networkdevicediscovery.h>
#include <hardwaremanager.h>

IntegrationPluginMennekes::IntegrationPluginMennekes()
{

}

void IntegrationPluginMennekes::discoverThings(ThingDiscoveryInfo *info)
{

    if (info->thingClassId() == amtronECUThingClassId) {
        if (!hardwareManager()->networkDeviceDiscovery()->available()) {
            qCWarning(dcMennekes()) << "The network discovery is not available on this platform.";
            info->finish(Thing::ThingErrorUnsupportedFeature, QT_TR_NOOP("The network device discovery is not available."));
            return;
        }

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
        if (!hardwareManager()->networkDeviceDiscovery()->available()) {
            qCWarning(dcMennekes()) << "The network discovery is not available on this platform.";
            info->finish(Thing::ThingErrorUnsupportedFeature, QT_TR_NOOP("The network device discovery is not available."));
            return;
        }

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
    } else if (info->thingClassId() == amtronCompact20ThingClassId) {
        AmtronCompact20Discovery *discovery = new AmtronCompact20Discovery(hardwareManager()->modbusRtuResource(), info);
        connect(discovery, &AmtronCompact20Discovery::discoveryFinished, info, [this, info, discovery](bool modbusMasterAvailable){
            if (!modbusMasterAvailable) {
                info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("No modbus RTU master with appropriate settings found. Please set up a modbus RTU master with a baudrate of 57600, 8 data bis, 2 stop bits and no parity first."));
                return;
            }

            qCInfo(dcMennekes()) << "Discovery results:" << discovery->discoveryResults().count();

            foreach (const AmtronCompact20Discovery::Result &result, discovery->discoveryResults()) {
                ThingDescriptor descriptor(amtronCompact20ThingClassId, "AMTRON Compact 2.0s", QString("Slave ID: %1, Serial: %2").arg(result.slaveId).arg(result.serialNumber));

                ParamList params{
                    {amtronCompact20ThingModbusMasterUuidParamTypeId, result.modbusRtuMasterId},
                    {amtronCompact20ThingSlaveAddressParamTypeId, result.slaveId}
                };
                descriptor.setParams(params);

                Thing *existingThing = myThings().findByParams(params);
                if (existingThing) {
                    descriptor.setThingId(existingThing->id());
                }
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

    if (info->thing()->thingClassId() == amtronCompact20ThingClassId) {
        if (m_amtronCompact20Connections.contains(thing)) {
            qCDebug(dcMennekes()) << "Reconfiguring existing thing" << thing->name();
            m_amtronCompact20Connections.take(thing)->deleteLater();
        }

        setupAmtronCompact20Connection(info);
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
            foreach(AmtronCompact20ModbusRtuConnection *connection, m_amtronCompact20Connections) {
                qCDebug(dcMennekes()) << "Updating connection" << connection->modbusRtuMaster()->serialPort() << connection->slaveId();
                connection->update();
                connection->setHeartbeat(0x55AA);
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
            int maxChargingCurrent = info->thing()->stateValue(amtronECUMaxChargingCurrentStateTypeId).toUInt();
            int effectiveCurrent = power ? maxChargingCurrent : 0;
            qCInfo(dcMennekes()) << "Executing power action:" << power << "max current:" << maxChargingCurrent << "-> effective current" << effectiveCurrent;
            QModbusReply *reply = amtronECUConnection->setHemsCurrentLimit(effectiveCurrent);
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
            bool power = info->thing()->stateValue(amtronECUPowerStateTypeId).toBool();
            int maxChargingCurrent = info->action().paramValue(amtronECUMaxChargingCurrentActionMaxChargingCurrentParamTypeId).toInt();
            int effectiveCurrent = power ? maxChargingCurrent : 0;
            qCInfo(dcMennekes()) << "Executing max current action:" << maxChargingCurrent << "Power is" << power << "-> effective:" << effectiveCurrent;
            QModbusReply *reply = amtronECUConnection->setHemsCurrentLimit(effectiveCurrent);
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

    if (info->thing()->thingClassId() == amtronHCC3ThingClassId) {
        AmtronHCC3ModbusTcpConnection *amtronHCC3Connection = m_amtronHCC3Connections.value(info->thing());

        if (info->action().actionTypeId() == amtronHCC3PowerActionTypeId) {
            bool power = info->action().paramValue(amtronHCC3PowerActionPowerParamTypeId).toBool();

            AmtronHCC3ModbusTcpConnection::ChargeState chargeState;
            if (power) {
                // When turning on, we'll need to either start or resume, depending on the current state
                if (amtronHCC3Connection->amtronState() == AmtronHCC3ModbusTcpConnection::AmtronStatePaused) {
                    chargeState = AmtronHCC3ModbusTcpConnection::ChargeStateContinue;
                } else {
                    chargeState = AmtronHCC3ModbusTcpConnection::ChargeStateStart;
                }
            } else {
                // We'll just use Pause as a Terminate command would requre an EV re-plug in order to resume.
                chargeState = AmtronHCC3ModbusTcpConnection::ChargeStatePause;
            }

            QModbusReply *reply = amtronHCC3Connection->setChangeChargeState(chargeState);
            connect(reply, &QModbusReply::finished, info, [info, reply, power](){
                if (reply->error() == QModbusDevice::NoError) {
                    info->thing()->setStateValue(amtronHCC3PowerStateTypeId, power);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    qCWarning(dcMennekes()) << "Error setting charge state:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
        }
        if (info->action().actionTypeId() == amtronHCC3MaxChargingCurrentActionTypeId) {
            int maxChargingCurrent = info->action().paramValue(amtronHCC3MaxChargingCurrentActionMaxChargingCurrentParamTypeId).toInt();
            QModbusReply *reply = amtronHCC3Connection->setCustomerCurrentLimitation(maxChargingCurrent);
            connect(reply, &QModbusReply::finished, info, [info, reply, maxChargingCurrent](){
                if (reply->error() == QModbusDevice::NoError) {
                    info->thing()->setStateValue(amtronHCC3MaxChargingCurrentStateTypeId, maxChargingCurrent);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
        }
    }

    if (info->thing()->thingClassId() == amtronCompact20ThingClassId) {
        AmtronCompact20ModbusRtuConnection *amtronCompact20Connection = m_amtronCompact20Connections.value(info->thing());

        if (info->action().actionTypeId() == amtronCompact20PowerActionTypeId) {
            bool power = info->action().paramValue(amtronCompact20PowerActionPowerParamTypeId).toBool();

            ModbusRtuReply *reply = amtronCompact20Connection->setChargingReleaseEnergyManager(power ? 1 : 0);
            connect(reply, &ModbusRtuReply::finished, info, [info, reply, power](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    info->thing()->setStateValue(amtronCompact20PowerStateTypeId, power);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    qCWarning(dcMennekes()) << "Error setting charge state:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
        }
        if (info->action().actionTypeId() == amtronCompact20MaxChargingCurrentActionTypeId) {
            int maxChargingCurrent = info->action().paramValue(amtronCompact20MaxChargingCurrentActionMaxChargingCurrentParamTypeId).toInt();
            float value = maxChargingCurrent;

            // Note: in firmwares up to 1.0.2 there's an issue that it cannot be exactly 6A, must be 6.01 or so
            if (maxChargingCurrent == 6) {
                value = 6.01;
            }

            ModbusRtuReply *reply = amtronCompact20Connection->setChargingCurrentEnergyManager(value);
            connect(reply, &ModbusRtuReply::finished, info, [info, reply, maxChargingCurrent](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    info->thing()->setStateValue(amtronCompact20MaxChargingCurrentStateTypeId, maxChargingCurrent);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
        }
        if (info->action().actionTypeId() == amtronCompact20DesiredPhaseCountActionTypeId) {
            int desiredPhaseCount = info->action().paramValue(amtronCompact20DesiredPhaseCountActionDesiredPhaseCountParamTypeId).toInt();

            ModbusRtuReply *reply = amtronCompact20Connection->setRequestedPhases(desiredPhaseCount == 1 ? AmtronCompact20ModbusRtuConnection::PhaseModeSingle : AmtronCompact20ModbusRtuConnection::PhaseModeAll);
            connect(reply, &ModbusRtuReply::finished, info, [info, reply, desiredPhaseCount](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    info->thing()->setStateValue(amtronCompact20DesiredPhaseCountStateTypeId, desiredPhaseCount);
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

    if (thing->thingClassId() == amtronCompact20ThingClassId && m_amtronCompact20Connections.contains(thing)) {
        AmtronCompact20ModbusRtuConnection *connection = m_amtronCompact20Connections.take(thing);
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
        if (amtronECUConnection->meterVoltageL1() > 0) {
            phaseCount++;
        }
        if (amtronECUConnection->meterVoltageL2() > 0) {
            phaseCount++;
        }
        if (amtronECUConnection->meterVoltageL3() > 0) {
            phaseCount++;
        }
        qCDebug(dcMennekes()) << "Connected phases:" << phaseCount;
    }

    thing->setStateValue(amtronECUPhaseCountStateTypeId, phaseCount);
}

void IntegrationPluginMennekes::updateCompact20PhaseCount(Thing *thing)
{
    AmtronCompact20ModbusRtuConnection *connection = m_amtronCompact20Connections.value(thing);

    // Using detected phases if detection completed (0 = undetected). Could be 1, 2 or 3
    if (connection->detectedEVPhases() > 0) {
        thing->setStateValue(amtronCompact20PhaseCountStateTypeId, connection->detectedEVPhases());
    } else {
        thing->setStateValue(amtronCompact20PhaseCountStateTypeId, connection->switchedPhases() == AmtronCompact20ModbusRtuConnection::PhaseModeAll ? 3 : 1);
    }
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

    connect(amtronHCC3Connection, &AmtronHCC3ModbusTcpConnection::updateFinished, thing, [amtronHCC3Connection, thing](){
        qCDebug(dcMennekes()) << "Amtron HCC3 update finished:" << thing->name() << amtronHCC3Connection;
        thing->setStateMaxValue(amtronHCC3MaxChargingCurrentStateTypeId, amtronHCC3Connection->installationCurrent());
    });

    connect(amtronHCC3Connection, &AmtronHCC3ModbusTcpConnection::cpSignalStateChanged, thing, [thing](AmtronHCC3ModbusTcpConnection::CPSignalState cpSignalState) {
        qCInfo(dcMennekes()) << "CP signal state changed" << cpSignalState;
        thing->setStateValue(amtronHCC3PluggedInStateTypeId, cpSignalState >= AmtronHCC3ModbusTcpConnection::CPSignalStateB1);
    });

    connect(amtronHCC3Connection, &AmtronHCC3ModbusTcpConnection::phaseCountChanged, thing, [thing](quint16 phaseCount) {
        qCInfo(dcMennekes()) << "Phase count changed:" << phaseCount;
        if (phaseCount > 0) {
            thing->setStateValue(amtronHCC3PhaseCountStateTypeId, phaseCount);
        }
    });

    connect(amtronHCC3Connection, &AmtronHCC3ModbusTcpConnection::amtronStateChanged, thing, [thing](AmtronHCC3ModbusTcpConnection::AmtronState amtronState) {
        qCInfo(dcMennekes()) << "Amtron state changed:" << amtronState;
        switch (amtronState) {
        case AmtronHCC3ModbusTcpConnection::AmtronStateIdle:
        case AmtronHCC3ModbusTcpConnection::AmtronStateStandByAuthorize:
        case AmtronHCC3ModbusTcpConnection::AmtronStateStandbyConnect:
        case AmtronHCC3ModbusTcpConnection::AmtronStatePaused:
        case AmtronHCC3ModbusTcpConnection::AmtronStateTerminated:
        case AmtronHCC3ModbusTcpConnection::AmtronStateError:
            thing->setStateValue(amtronHCC3ChargingStateTypeId, false);
            break;
        case AmtronHCC3ModbusTcpConnection::AmtronStateCharging:
            thing->setStateValue(amtronHCC3ChargingStateTypeId, true);
            break;
        }
    });

    connect(amtronHCC3Connection, &AmtronHCC3ModbusTcpConnection::actualPowerConsumptionChanged, thing, [thing](quint32 actualPowerConsumption) {
        qCInfo(dcMennekes()) << "Actual power consumption changed:" << actualPowerConsumption;
        thing->setStateValue(amtronHCC3CurrentPowerStateTypeId, actualPowerConsumption);
    });

    connect(amtronHCC3Connection, &AmtronHCC3ModbusTcpConnection::chargingSessionMeterChanged, thing, [thing](quint32 chargingSessionMeter) {
        thing->setStateValue(amtronHCC3SessionEnergyStateTypeId, chargingSessionMeter / 1000.0);
        // Don't have a total... still providing it for the interface, the energy experience will deal with the value resetting frequently
        thing->setStateValue(amtronHCC3TotalEnergyConsumedStateTypeId, chargingSessionMeter / 1000.0);
    });

    connect(amtronHCC3Connection, &AmtronHCC3ModbusTcpConnection::customerCurrentLimitationChanged, thing, [thing](quint16 customerCurrentLimitation) {
        thing->setStateValue(amtronHCC3MaxChargingCurrentStateTypeId, customerCurrentLimitation);
    });


    amtronHCC3Connection->connectDevice();
}

void IntegrationPluginMennekes::setupAmtronCompact20Connection(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    uint slaveId = thing->paramValue(amtronCompact20ThingSlaveAddressParamTypeId).toUInt();
    if (slaveId > 254 || slaveId == 0) {
        qCWarning(dcMennekes()) << "Setup failed, slave ID is not valid" << slaveId;
        info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("The Modbus address not valid. It must be a value between 1 and 254."));
        return;
    }

    QUuid uuid = thing->paramValue(amtronCompact20ThingModbusMasterUuidParamTypeId).toUuid();
    if (!hardwareManager()->modbusRtuResource()->hasModbusRtuMaster(uuid)) {
        qCWarning(dcMennekes()) << "Setup failed, hardware manager not available";
        info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("The Modbus RTU resource is not available."));
        return;
    }

    AmtronCompact20ModbusRtuConnection *compact20Connection = new AmtronCompact20ModbusRtuConnection(hardwareManager()->modbusRtuResource()->getModbusRtuMaster(uuid), slaveId, this);
    connect(info, &ThingSetupInfo::aborted, compact20Connection, &ModbusRtuMaster::deleteLater);
    m_amtronCompact20Connections.insert(thing, compact20Connection);

    connect(info, &ThingSetupInfo::aborted, this, [=](){
        m_amtronCompact20Connections.take(info->thing())->deleteLater();
    });

    connect(compact20Connection, &AmtronCompact20ModbusRtuConnection::reachableChanged, thing, [compact20Connection, thing](bool reachable){
        qCDebug(dcMennekes()) << "Reachable state changed" << reachable;
        if (reachable) {
            compact20Connection->initialize();
        } else {
            thing->setStateValue("connected", false);
        }
    });

    connect(compact20Connection, &AmtronCompact20ModbusRtuConnection::initializationFinished, info, [=](bool success){
        qCDebug(dcMennekes()) << "Initialisation finished" << success;
        if (info->isInitialSetup() && !success) {
            m_amtronCompact20Connections.take(info->thing())->deleteLater();
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        info->finish(Thing::ThingErrorNoError);

        if (success) {
            qCDebug(dcMennekes) << "Firmware version:" << compact20Connection->firmwareVersion();
            info->thing()->setStateValue(amtronCompact20CurrentVersionStateTypeId, compact20Connection->firmwareVersion());
            info->thing()->setStateValue(amtronCompact20PowerStateTypeId, compact20Connection->chargingReleaseEnergyManager() == 1);
            info->thing()->setStateValue(amtronCompact20MaxChargingCurrentStateTypeId, qRound(compact20Connection->chargingCurrentEnergyManager()));
        }
    });

    connect(compact20Connection, &AmtronCompact20ModbusRtuConnection::reachableChanged, thing, [=](bool reachable){
        thing->setStateValue(amtronCompact20ConnectedStateTypeId, reachable);
    });


    connect(compact20Connection, &AmtronCompact20ModbusRtuConnection::updateFinished, thing, [this, compact20Connection, thing](){
        qCDebug(dcMennekes()) << "Update finished:" << thing->name() << compact20Connection;
        updateCompact20PhaseCount(thing);

        quint16 maxCurrentEVSE = compact20Connection->maxCurrentEVSE();
        quint16 maxCurrentSession = compact20Connection->maxCurrentSession();
        if (maxCurrentSession > 0 && maxCurrentEVSE > 0) {
            thing->setStateMaxValue(amtronCompact20MaxChargingCurrentStateTypeId, qMin(maxCurrentEVSE, maxCurrentSession));
        } else if (maxCurrentSession > 0){
            thing->setStateMaxValue(amtronCompact20MaxChargingCurrentStateTypeId, maxCurrentSession);
        } else if (maxCurrentEVSE) {
            thing->setStateMaxValue(amtronCompact20MaxChargingCurrentStateTypeId, maxCurrentEVSE);
        }
    });

    connect(compact20Connection, &AmtronCompact20ModbusRtuConnection::cpSignalStateChanged, thing, [thing](AmtronCompact20ModbusRtuConnection::CPSignalState cpSignalState){
        qCDebug(dcMennekes()) << "CP signal state changed:" << thing->name() << cpSignalState;
        // Note: using EVSE state register instead
//        switch (cpSignalState) {
//        case AmtronCompact20ModbusRtuConnection::CPSignalStateA1:
//        case AmtronCompact20ModbusRtuConnection::CPSignalStateA2:
//            thing->setStateValue(amtronCompact20PluggedInStateTypeId, false);
//            thing->setStateValue(amtronCompact20ChargingStateTypeId, false);
//            break;
//        case AmtronCompact20ModbusRtuConnection::CPSignalStateB1:
//        case AmtronCompact20ModbusRtuConnection::CPSignalStateB2:
//            thing->setStateValue(amtronCompact20PluggedInStateTypeId, true);
//            thing->setStateValue(amtronCompact20ChargingStateTypeId, false);
//            break;
//        case AmtronCompact20ModbusRtuConnection::CPSignalStateC1:
//        case AmtronCompact20ModbusRtuConnection::CPSignalStateC2:
//        case AmtronCompact20ModbusRtuConnection::CPSignalStateD1:
//        case AmtronCompact20ModbusRtuConnection::CPSignalStateD2:
//            thing->setStateValue(amtronCompact20PluggedInStateTypeId, true);
//            thing->setStateValue(amtronCompact20ChargingStateTypeId, true);
//            break;
//        case AmtronCompact20ModbusRtuConnection::CPSignalStateE:
//        case AmtronCompact20ModbusRtuConnection::CPSignalStateF:
//            qCWarning(dcMennekes()) << "Wallbox in Error state!";
//            thing->setStateValue(amtronCompact20PluggedInStateTypeId, false);
//            thing->setStateValue(amtronCompact20ChargingStateTypeId, false);
//            break;
//        }
    });

    connect(compact20Connection, &AmtronCompact20ModbusRtuConnection::evseStateChanged, thing, [thing](AmtronCompact20ModbusRtuConnection::EvseState evseState){
        qCDebug(dcMennekes()) << "EVSE state changed:" << thing->name() << evseState;
        switch (evseState) {
        case AmtronCompact20ModbusRtuConnection::EvseStateNotInitialized:
        case AmtronCompact20ModbusRtuConnection::EvseStateIdle:
            thing->setStateValue(amtronCompact20PluggedInStateTypeId, false);
            thing->setStateValue(amtronCompact20ChargingStateTypeId, false);
            break;
        case AmtronCompact20ModbusRtuConnection::EvseStateEvConnected:
        case AmtronCompact20ModbusRtuConnection::EvseStatePreconditionsValidButNotCharging:
        case AmtronCompact20ModbusRtuConnection::EvseStateReadyToCharge:
            thing->setStateValue(amtronCompact20PluggedInStateTypeId, true);
            thing->setStateValue(amtronCompact20ChargingStateTypeId, false);
            break;
        case AmtronCompact20ModbusRtuConnection::EvseStateCharging:
            thing->setStateValue(amtronCompact20PluggedInStateTypeId, true);
            thing->setStateValue(amtronCompact20ChargingStateTypeId, true);
            break;
        case AmtronCompact20ModbusRtuConnection::EvseStateError:
        case AmtronCompact20ModbusRtuConnection::EvseStateServiceMode:
            thing->setStateValue(amtronCompact20PluggedInStateTypeId, false);
            thing->setStateValue(amtronCompact20ChargingStateTypeId, false);
            break;
        }
    });

    connect(compact20Connection, &AmtronCompact20ModbusRtuConnection::chargingCurrentEnergyManagerChanged, thing, [thing](float chargingCurrent){
        qCInfo(dcMennekes()) << thing->name() << "charging current energy manager changed:" << chargingCurrent;
        if (chargingCurrent >= 6) {
            thing->setStateValue(amtronCompact20MaxChargingCurrentStateTypeId, qRound(chargingCurrent));
        }
    });

    connect(compact20Connection, &AmtronCompact20ModbusRtuConnection::switchedPhasesChanged, thing, [thing](AmtronCompact20ModbusRtuConnection::PhaseMode phaseMode){
        qCInfo(dcMennekes()) << thing->name() << "switched EV phases changed:" << phaseMode;
    });
    connect(compact20Connection, &AmtronCompact20ModbusRtuConnection::detectedEVPhasesChanged, thing, [thing](quint16 detectedEvPhases){
        qCInfo(dcMennekes()) << thing->name() << "detected EV phases changed:" << detectedEvPhases;
    });

    connect(compact20Connection, &AmtronCompact20ModbusRtuConnection::requestedPhasesChanged, thing, [thing](AmtronCompact20ModbusRtuConnection::PhaseMode phaseMode){
        thing->setStateValue(amtronCompact20DesiredPhaseCountStateTypeId, phaseMode == AmtronCompact20ModbusRtuConnection::PhaseModeAll ? 3 : 1);
    });

    connect(compact20Connection, &AmtronCompact20ModbusRtuConnection::powerOverallChanged, thing, [thing](double powerOverall){
        // The wallbox may measure non
        if (powerOverall < 20) {
            powerOverall = 0;
        }
        thing->setStateValue(amtronCompact20CurrentPowerStateTypeId, powerOverall);
    });
    connect(compact20Connection, &AmtronCompact20ModbusRtuConnection::chargedEnergyTotalChanged, thing, [thing](double chargedEnergyTotal){
        thing->setStateValue(amtronCompact20TotalEnergyConsumedStateTypeId, chargedEnergyTotal);
    });
    connect(compact20Connection, &AmtronCompact20ModbusRtuConnection::chargedEnergySessionChanged, thing, [thing](double chargedEnergySession){
        thing->setStateValue(amtronCompact20SessionEnergyStateTypeId, chargedEnergySession);
    });

    connect(compact20Connection, &AmtronCompact20ModbusRtuConnection::chargingReleaseEnergyManagerChanged, this, [thing](quint16 chargingReleaseEnergyManager){
        thing->setStateValue(amtronCompact20PowerStateTypeId, chargingReleaseEnergyManager == 1);
    });

}

bool IntegrationPluginMennekes::ensureAmtronECUVersion(AmtronECUModbusTcpConnection *connection, const QString &version)
{
    QByteArray deviceVersion = QByteArray::fromHex(QByteArray::number(connection->firmwareVersion(), 16));
    return deviceVersion >= version;
}
