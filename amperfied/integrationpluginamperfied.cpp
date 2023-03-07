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

#include "integrationpluginamperfied.h"
#include "plugininfo.h"
#include "energycontroldiscovery.h"
#include "connecthomediscovery.h"

#include <network/networkdevicediscovery.h>
#include <hardwaremanager.h>
#include <hardware/modbus/modbusrtuhardwareresource.h>

IntegrationPluginAmperfied::IntegrationPluginAmperfied()
{

}

void IntegrationPluginAmperfied::discoverThings(ThingDiscoveryInfo *info)
{
    hardwareManager()->modbusRtuResource();

    if (info->thingClassId() == energyControlThingClassId) {
        EnergyControlDiscovery *discovery = new EnergyControlDiscovery(hardwareManager()->modbusRtuResource(), info);

        connect(discovery, &EnergyControlDiscovery::discoveryFinished, info, [this, info, discovery](bool modbusMasterAvailable){
            if (!modbusMasterAvailable) {
                info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("No modbus RTU master with appropriate settings found. Please set up a modbus RTU master with a baudrate of 19200, 8 data bis, 1 stop bit and even parity first."));
                return;
            }

            qCInfo(dcAmperfied()) << "Discovery results:" << discovery->discoveryResults().count();

            foreach (const EnergyControlDiscovery::Result &result, discovery->discoveryResults()) {
                ThingDescriptor descriptor(energyControlThingClassId, "Amperfied Energy Control", QString("Slave ID: %1").arg(result.slaveId));

                ParamList params{
                    {energyControlThingRtuMasterParamTypeId, result.modbusRtuMasterId},
                    {energyControlThingSlaveIdParamTypeId, result.slaveId}
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

        return;
    }

    if (info->thingClassId() == connectHomeThingClassId) {
        ConnectHomeDiscovery *discovery = new ConnectHomeDiscovery(hardwareManager()->networkDeviceDiscovery(), info);
        connect(discovery, &ConnectHomeDiscovery::discoveryFinished, info, [this, info, discovery](){
            qCInfo(dcAmperfied()) << "Discovery results:" << discovery->discoveryResults().count();

            foreach (const ConnectHomeDiscovery::Result &result, discovery->discoveryResults()) {
                ThingDescriptor descriptor(connectHomeThingClassId, "Amperfied connect.home", QString("MAC: %1").arg(result.networkDeviceInfo.macAddress()));

                ParamList params{
                    {connectHomeThingMacAddressParamTypeId, result.networkDeviceInfo.macAddress()}
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

void IntegrationPluginAmperfied::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcAmperfied()) << "Setup" << thing << thing->params();

    if (thing->thingClassId() == energyControlThingClassId) {

        if (m_rtuConnections.contains(thing)) {
            qCDebug(dcAmperfied()) << "Reconfiguring existing thing" << thing->name();
            m_rtuConnections.take(thing)->deleteLater();
        }

        setupRtuConnection(info);
        return;
    }


    if (info->thing()->thingClassId() == connectHomeThingClassId) {
        if (m_tcpConnections.contains(info->thing())) {
            delete m_tcpConnections.take(info->thing());
        }

        NetworkDeviceMonitor *monitor = m_monitors.value(info->thing());
        if (!monitor) {
            monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(MacAddress(thing->paramValue(connectHomeThingMacAddressParamTypeId).toString()));
            m_monitors.insert(thing, monitor);
        }

        connect(info, &ThingSetupInfo::aborted, monitor, [=](){
            if (m_monitors.contains(thing)) {
                qCDebug(dcAmperfied()) << "Unregistering monitor because setup has been aborted.";
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        });

        qCDebug(dcAmperfied()) << "Monitor reachable" << monitor->reachable() << thing->paramValue(connectHomeThingMacAddressParamTypeId).toString();
        if (monitor->reachable()) {
            setupTcpConnection(info);
        } else {
            connect(monitor, &NetworkDeviceMonitor::reachableChanged, info, [this, info](bool reachable){
                qCDebug(dcAmperfied()) << "Monitor reachable changed!" << reachable;
                if (reachable) {
                    setupTcpConnection(info);
                }
            });
        }
    }
}

void IntegrationPluginAmperfied::postSetupThing(Thing *thing)
{
    Q_UNUSED(thing)
    if (!m_pluginTimer) {
        qCDebug(dcAmperfied()) << "Starting plugin timer...";
        m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(2);
        connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
            foreach(AmperfiedModbusRtuConnection *connection, m_rtuConnections) {
                qCDebug(dcAmperfied()) << "Updating connection" << connection->modbusRtuMaster() << connection->slaveId();
                connection->update();
            }
            foreach(AmperfiedModbusTcpConnection *connection, m_tcpConnections) {
                qCDebug(dcAmperfied()) << "Updating connection" << connection->hostAddress();
                connection->update();
            }
        });
        m_pluginTimer->start();
    }
}

void IntegrationPluginAmperfied::executeAction(ThingActionInfo *info)
{
    if (info->thing()->thingClassId() == energyControlThingClassId) {
        AmperfiedModbusRtuConnection *connection = m_rtuConnections.value(info->thing());

        if (info->action().actionTypeId() == energyControlPowerActionTypeId) {
            bool power = info->action().paramValue(energyControlPowerActionPowerParamTypeId).toBool();
            ModbusRtuReply *reply = connection->setChargingCurrent(power ? info->thing()->stateValue(energyControlMaxChargingCurrentStateTypeId).toUInt() * 10 : 0);
            connect(reply, &ModbusRtuReply::finished, info, [info, reply, power](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    info->thing()->setStateValue(energyControlPowerStateTypeId, power);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    qCWarning(dcAmperfied()) << "Error setting power:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
            return;
        }

        if (info->action().actionTypeId() == energyControlMaxChargingCurrentActionTypeId) {
            bool power = info->thing()->stateValue(energyControlPowerStateTypeId).toBool();
            uint max = info->action().paramValue(energyControlMaxChargingCurrentActionMaxChargingCurrentParamTypeId).toUInt() * 10;
            ModbusRtuReply *reply = connection->setChargingCurrent(power ? max : 0);
            connect(reply, &ModbusRtuReply::finished, info, [info, reply, max](){
                if (reply->error() == ModbusRtuReply::NoError) {
                    info->thing()->setStateValue(energyControlMaxChargingCurrentStateTypeId, max);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    qCWarning(dcAmperfied()) << "Error setting power:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
        }

    }

    if (info->thing()->thingClassId() == connectHomeThingClassId) {
        AmperfiedModbusTcpConnection *connection = m_tcpConnections.value(info->thing());

        if (info->action().actionTypeId() == connectHomePowerActionTypeId) {
            bool power = info->action().paramValue(connectHomePowerActionPowerParamTypeId).toBool();
            QModbusReply *reply = connection->setChargingCurrent(power ? info->thing()->stateValue(connectHomeMaxChargingCurrentStateTypeId).toUInt() * 10 : 0);
            connect(reply, &QModbusReply::finished, info, [info, reply, power](){
                if (reply->error() == QModbusDevice::NoError) {
                    info->thing()->setStateValue(connectHomePowerStateTypeId, power);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    qCWarning(dcAmperfied()) << "Error setting power:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
        }

        if (info->action().actionTypeId() == connectHomeMaxChargingCurrentActionTypeId) {
            bool power = info->thing()->stateValue(connectHomePowerStateTypeId).toBool();
            uint max = info->action().paramValue(connectHomeMaxChargingCurrentActionMaxChargingCurrentParamTypeId).toUInt() * 10;
            QModbusReply *reply = connection->setChargingCurrent(power ? max : 0);
            connect(reply, &QModbusReply::finished, info, [info, reply, max](){
                if (reply->error() == QModbusDevice::NoError) {
                    info->thing()->setStateValue(connectHomeMaxChargingCurrentStateTypeId, max);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    qCWarning(dcAmperfied()) << "Error setting power:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
        }

    }

}

void IntegrationPluginAmperfied::thingRemoved(Thing *thing)
{
    if (thing->thingClassId() == energyControlThingClassId) {
        delete m_rtuConnections.take(thing);
    }

    if (thing->thingClassId() == connectHomeThingClassId) {
        delete m_tcpConnections.take(thing);
    }

    if (myThings().isEmpty() && m_pluginTimer) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginAmperfied::setupRtuConnection(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    ModbusRtuMaster *master = hardwareManager()->modbusRtuResource()->getModbusRtuMaster(thing->paramValue(energyControlThingRtuMasterParamTypeId).toUuid());
    quint16 slaveId = thing->paramValue(energyControlThingSlaveIdParamTypeId).toUInt();
    AmperfiedModbusRtuConnection *connection = new AmperfiedModbusRtuConnection(master, slaveId, thing);

    connect(connection, &AmperfiedModbusRtuConnection::reachableChanged, thing, [connection, thing](bool reachable){
        if (reachable) {
            connection->initialize();
        } else {
            thing->setStateValue(energyControlCurrentPowerStateTypeId, 0);
            thing->setStateValue(energyControlConnectedStateTypeId, false);
        }
    });
    connect(connection, &AmperfiedModbusRtuConnection::initializationFinished, thing, [thing](bool success){
        if (success) {
            thing->setStateValue(energyControlConnectedStateTypeId, true);
        }
    });

    connect(connection, &AmperfiedModbusRtuConnection::initializationFinished, info, [this, info, connection](bool success){
        if (success) {
            if (connection->version() < 0x0107) {
                qCWarning(dcAmperfied()) << "We require at least version 1.0.8.";
                info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("The firmware of this wallbox is too old. Please update the wallbox to at least firmware 1.0.7."));
                delete connection;
                return;
            }
            m_rtuConnections.insert(info->thing(), connection);
            info->finish(Thing::ThingErrorNoError);
        } else {
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("The wallbox is not responding"));
        }
    });

    connect(connection, &AmperfiedModbusRtuConnection::updateFinished, thing, [connection, thing](){
        qCDebug(dcAmperfied()) << "Updated:" << connection;

        if (connection->chargingCurrent() == 0) {
            thing->setStateValue(energyControlPowerStateTypeId, false);
        } else {
            thing->setStateValue(energyControlPowerStateTypeId, true);
            thing->setStateValue(energyControlMaxChargingCurrentStateTypeId, connection->chargingCurrent() / 10);
        }
        thing->setStateMinMaxValues(energyControlMaxChargingCurrentStateTypeId, connection->minChargingCurrent(), connection->maxChargingCurrent());
        thing->setStateValue(energyControlCurrentPowerStateTypeId, connection->currentPower());
        thing->setStateValue(energyControlTotalEnergyConsumedStateTypeId, connection->totalEnergy() / 1000.0);
        thing->setStateValue(energyControlSessionEnergyStateTypeId, connection->sessionEnergy() / 1000.0);
        switch (connection->chargingState()) {
        case AmperfiedModbusRtuConnection::ChargingStateUndefined:
        case AmperfiedModbusRtuConnection::ChargingStateA1:
        case AmperfiedModbusRtuConnection::ChargingStateA2:
            thing->setStateValue(energyControlPluggedInStateTypeId, false);
            break;
        case AmperfiedModbusRtuConnection::ChargingStateB1:
        case AmperfiedModbusRtuConnection::ChargingStateB2:
        case AmperfiedModbusRtuConnection::ChargingStateC1:
        case AmperfiedModbusRtuConnection::ChargingStateC2:
            thing->setStateValue(energyControlPluggedInStateTypeId, true);
            break;
        case AmperfiedModbusRtuConnection::ChargingStateDerating:
        case AmperfiedModbusRtuConnection::ChargingStateE:
        case AmperfiedModbusRtuConnection::ChargingStateError:
        case AmperfiedModbusRtuConnection::ChargingStateF:
            qCWarning(dcAmperfied()) << "Unhandled charging state:" << connection->chargingState();
        }

        int phaseCount = 0;
        if (connection->currentL1() > 1) {
            phaseCount++;
        }
        if (connection->currentL2() > 1) {
            phaseCount++;
        }
        if (connection->currentL3() > 1) {
            phaseCount++;
        }
        if (phaseCount > 0) {
            thing->setStateValue(energyControlPhaseCountStateTypeId, phaseCount);
        }
        thing->setStateValue(energyControlChargingStateTypeId, phaseCount > 0);
    });

    connection->update();

}

void IntegrationPluginAmperfied::setupTcpConnection(ThingSetupInfo *info)
{
    qCDebug(dcAmperfied()) << "setting up TCP connection";
    Thing *thing = info->thing();
    NetworkDeviceMonitor *monitor = m_monitors.value(info->thing());
    AmperfiedModbusTcpConnection *connection = new AmperfiedModbusTcpConnection(monitor->networkDeviceInfo().address(), 502, 1, info->thing());

    connect(connection, &AmperfiedModbusTcpConnection::reachableChanged, thing, [connection, thing](bool reachable){
        if (reachable) {
            connection->initialize();
        } else {
            thing->setStateValue(connectHomeCurrentPowerStateTypeId, 0);
            thing->setStateValue(connectHomeConnectedStateTypeId, false);
        }
    });


    connect(connection, &AmperfiedModbusTcpConnection::initializationFinished, info, [this, info, connection](bool success){
        if (success) {
            if (connection->version() < 0x0107) {
                qCWarning(dcAmperfied()) << "We require at least version 1.0.8.";
                info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("The firmware of this wallbox is too old. Please update the wallbox to at least firmware 1.0.7."));
                delete connection;
                return;
            }
            m_tcpConnections.insert(info->thing(), connection);
            info->finish(Thing::ThingErrorNoError);
            connection->update();
        } else {
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("The wallbox is not responding"));
        }
    });

    connect(connection, &AmperfiedModbusTcpConnection::updateFinished, thing, [connection, thing](){
        qCDebug(dcAmperfied()) << "Updated:" << connection;

        thing->setStateValue(connectHomeConnectedStateTypeId, true);

        if (connection->chargingCurrent() == 0) {
            thing->setStateValue(connectHomePowerStateTypeId, false);
        } else {
            thing->setStateValue(connectHomePowerStateTypeId, true);
            thing->setStateValue(connectHomeMaxChargingCurrentStateTypeId, connection->chargingCurrent() / 10);
        }
        thing->setStateMinMaxValues(connectHomeMaxChargingCurrentStateTypeId, connection->minChargingCurrent(), connection->maxChargingCurrent());
        thing->setStateValue(connectHomeCurrentPowerStateTypeId, connection->currentPower());
        thing->setStateValue(connectHomeTotalEnergyConsumedStateTypeId, connection->totalEnergy() / 1000.0);
        thing->setStateValue(connectHomeSessionEnergyStateTypeId, connection->sessionEnergy() / 1000.0);
        switch (connection->chargingState()) {
        case AmperfiedModbusTcpConnection::ChargingStateUndefined:
        case AmperfiedModbusTcpConnection::ChargingStateA1:
        case AmperfiedModbusTcpConnection::ChargingStateA2:
            thing->setStateValue(connectHomePluggedInStateTypeId, false);
            break;
        case AmperfiedModbusTcpConnection::ChargingStateB1:
        case AmperfiedModbusTcpConnection::ChargingStateB2:
        case AmperfiedModbusTcpConnection::ChargingStateC1:
        case AmperfiedModbusTcpConnection::ChargingStateC2:
            thing->setStateValue(connectHomePluggedInStateTypeId, true);
            break;
        case AmperfiedModbusTcpConnection::ChargingStateDerating:
        case AmperfiedModbusTcpConnection::ChargingStateE:
        case AmperfiedModbusTcpConnection::ChargingStateError:
        case AmperfiedModbusTcpConnection::ChargingStateF:
            qCWarning(dcAmperfied()) << "Unhandled charging state:" << connection->chargingState();
        }

        int phaseCount = 0;
        if (connection->currentL1() > 1) {
            phaseCount++;
        }
        if (connection->currentL2() > 1) {
            phaseCount++;
        }
        if (connection->currentL3() > 1) {
            phaseCount++;
        }
        if (phaseCount > 0) {
            thing->setStateValue(connectHomePhaseCountStateTypeId, phaseCount);
        }
        thing->setStateValue(connectHomeChargingStateTypeId, phaseCount > 0);
    });

    connection->connectDevice();
}


