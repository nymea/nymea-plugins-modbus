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

#include "integrationpluginvestel.h"
#include "plugininfo.h"
#include "evc04discovery.h"

#include <network/networkdevicediscovery.h>
#include <hardwaremanager.h>

IntegrationPluginVestel::IntegrationPluginVestel()
{

}

void IntegrationPluginVestel::discoverThings(ThingDiscoveryInfo *info)
{
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcVestel()) << "The network discovery is not available on this platform.";
        info->finish(Thing::ThingErrorUnsupportedFeature, QT_TR_NOOP("Unable to discover devices in the network. The system may not be installed correctly."));
        return;
    }

    if (info->thingClassId() == evc04ThingClassId) {
        EVC04Discovery *discovery = new EVC04Discovery(hardwareManager()->networkDeviceDiscovery(), dcVestel(), info);
        connect(discovery, &EVC04Discovery::discoveryFinished, info, [=](){
            foreach (const EVC04Discovery::Result &result, discovery->discoveryResults()) {

                QString name = result.chargepointId;
                QString description = result.brand + " " + result.model;
                ThingDescriptor descriptor(evc04ThingClassId, name, description);
                qCDebug(dcVestel()) << "Discovered:" << descriptor.title() << descriptor.description();

                // Check if we already have set up this device
                Things existingThings = myThings().filterByParam(evc04ThingMacAddressParamTypeId, result.networkDeviceInfo.macAddress());
                if (existingThings.count() == 1) {
                    qCDebug(dcVestel()) << "This wallbox already exists in the system:" << result.networkDeviceInfo;
                    descriptor.setThingId(existingThings.first()->id());
                }

                ParamList params;
                params << Param(evc04ThingMacAddressParamTypeId, result.networkDeviceInfo.macAddress());
                descriptor.setParams(params);
                info->addThingDescriptor(descriptor);
            }

            info->finish(Thing::ThingErrorNoError);
        });
        discovery->startDiscovery();
    }
}

void IntegrationPluginVestel::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcVestel()) << "Setup" << thing << thing->params();

    if (thing->thingClassId() == evc04ThingClassId) {

        if (m_evc04Connections.contains(thing)) {
            qCDebug(dcVestel()) << "Reconfiguring existing thing" << thing->name();
            m_evc04Connections.take(thing)->deleteLater();

            if (m_monitors.contains(thing)) {
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        }

        MacAddress macAddress = MacAddress(thing->paramValue(evc04ThingMacAddressParamTypeId).toString());
        if (!macAddress.isValid()) {
            qCWarning(dcVestel()) << "The configured mac address is not valid" << thing->params();
            info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The MAC address is not known. Please reconfigure the thing."));
            return;
        }

        NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(macAddress);
        m_monitors.insert(thing, monitor);

        connect(info, &ThingSetupInfo::aborted, monitor, [=](){
            if (m_monitors.contains(thing)) {
                qCDebug(dcVestel()) << "Unregistering monitor because setup has been aborted.";
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        });

        if (monitor->reachable()) {
            setupEVC04Connection(info);
        } else {
            qCDebug(dcVestel()) << "Waiting for the network monitor to get reachable before continuing to set up the connection" << thing->name() << "...";
            connect(monitor, &NetworkDeviceMonitor::reachableChanged, info, [=](bool reachable){
                if (reachable) {
                    qCDebug(dcVestel()) << "The monitor for thing setup" << thing->name() << "is now reachable. Continuing setup on" << monitor->networkDeviceInfo().address().toString();
                    setupEVC04Connection(info);
                }
            });
        }

        return;
    }
}

void IntegrationPluginVestel::postSetupThing(Thing *thing)
{
    Q_UNUSED(thing)
    if (!m_pluginTimer) {
        qCDebug(dcVestel()) << "Starting plugin timer...";
        m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(2);
        connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
            foreach(EVC04ModbusTcpConnection *connection, m_evc04Connections) {
                qCDebug(dcVestel()) << "Updating connection" << connection->modbusTcpMaster()->hostAddress().toString();
                connection->update();
                connection->setAliveRegister(1);
            }
        });

        m_pluginTimer->start();
    }
}

void IntegrationPluginVestel::executeAction(ThingActionInfo *info)
{
    if (info->thing()->thingClassId() == evc04ThingClassId) {
        EVC04ModbusTcpConnection *evc04Connection = m_evc04Connections.value(info->thing());

        if (info->action().actionTypeId() == evc04PowerActionTypeId) {
            bool power = info->action().paramValue(evc04PowerActionPowerParamTypeId).toBool();

            // If the car is *not* connected, writing a 0 to the charging current register will cause it to go to 6 A instead of 0
            // Because of this, we we're not connected, we'll do nothing, but once it get's connected, we'll sync the state over (see below in cableStateChanged)
            if (!power && evc04Connection->cableState() < EVC04ModbusTcpConnection::CableStateCableConnectedVehicleConnected) {
                qCDebug(dcVestel()) << "Set state to" << false << "but do nothing since the car is not connected";
                info->thing()->setStateValue(evc04PowerStateTypeId, false);
                info->finish(Thing::ThingErrorNoError);
                return;
            }

            uint chargingCurrent = (power ? info->thing()->stateValue(evc04MaxChargingCurrentStateTypeId).toUInt() : 0);
            qCDebug(dcVestel()) << "Write max charging current" << chargingCurrent;
            QModbusReply *reply = evc04Connection->setChargingCurrent(chargingCurrent);
            connect(reply, &QModbusReply::finished, info, [info, reply, power](){
                if (reply->error() == QModbusDevice::NoError) {
                    info->thing()->setStateValue(evc04PowerStateTypeId, power);
                    info->finish(Thing::ThingErrorNoError);
                } else {
                    qCWarning(dcVestel()) << "Error setting power:" << reply->error() << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                }
            });
        }

        if (info->action().actionTypeId() == evc04MaxChargingCurrentActionTypeId) {

            // Note: only write the register if power is true, otherwise we would start charging. The state represents the desired current,
            // once the power is true, the current will be written to the corresponding current.

            int maxChargingCurrent = info->action().paramValue(evc04MaxChargingCurrentActionMaxChargingCurrentParamTypeId).toInt();

            if (info->thing()->stateValue(evc04PowerStateTypeId).toBool()) {
                qCDebug(dcVestel()) << "Write max charging current" << maxChargingCurrent;
                QModbusReply *reply = evc04Connection->setChargingCurrent(maxChargingCurrent);
                connect(reply, &QModbusReply::finished, info, [info, reply, maxChargingCurrent](){
                    if (reply->error() == QModbusDevice::NoError) {
                        info->thing()->setStateValue(evc04MaxChargingCurrentStateTypeId, maxChargingCurrent);
                        info->finish(Thing::ThingErrorNoError);
                    } else {
                        info->finish(Thing::ThingErrorHardwareFailure);
                    }
                });
            } else {
                qCDebug(dcVestel()) << "Set state to" << maxChargingCurrent << "but do nothing since the power is false";
                info->thing()->setStateValue(evc04MaxChargingCurrentStateTypeId, maxChargingCurrent);
            }
        }
    }
}

void IntegrationPluginVestel::thingRemoved(Thing *thing)
{
    if (thing->thingClassId() == evc04ThingClassId && m_evc04Connections.contains(thing)) {
        EVC04ModbusTcpConnection *connection = m_evc04Connections.take(thing);
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

void IntegrationPluginVestel::setupEVC04Connection(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    QHostAddress address = m_monitors.value(thing)->networkDeviceInfo().address();

    qCDebug(dcVestel()) << "Setting up EVC04 wallbox on" << address.toString();
    EVC04ModbusTcpConnection *evc04Connection = new EVC04ModbusTcpConnection(address, 502, 0xff, this);
    connect(info, &ThingSetupInfo::aborted, evc04Connection, &EVC04ModbusTcpConnection::deleteLater);

    // Reconnect on monitor reachable changed
    NetworkDeviceMonitor *monitor = m_monitors.value(thing);
    connect(monitor, &NetworkDeviceMonitor::reachableChanged, thing, [=](bool reachable){
        qCDebug(dcVestel()) << "Network device monitor reachable changed for" << thing->name() << reachable;
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
        qCDebug(dcVestel()) << "Reachable changed to" << reachable << "for" << thing;
        if (reachable) {
            evc04Connection->initialize();
        } else {
            thing->setStateValue(evc04ConnectedStateTypeId, false);
            thing->setStateValue(evc04CurrentPowerStateTypeId, 0);
        }
    });

    connect(evc04Connection, &EVC04ModbusTcpConnection::initializationFinished, thing, [=](bool success){
        if (!thing->setupComplete())
            return;

        if (success) {
            thing->setStateValue(evc04ConnectedStateTypeId, true);
        } else {
            thing->setStateValue(evc04ConnectedStateTypeId, false);
            thing->setStateValue(evc04CurrentPowerStateTypeId, 0);

            // Try once to reconnect the device
            evc04Connection->reconnectDevice();
        }
    });

    connect(evc04Connection, &EVC04ModbusTcpConnection::initializationFinished, info, [=](bool success){
        if (!success) {
            qCWarning(dcVestel()) << "Connection init finished with errors" << thing->name() << evc04Connection->modbusTcpMaster()->hostAddress().toString();
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(monitor);
            evc04Connection->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("Error communicating with the wallbox."));
            return;
        }

        qCDebug(dcVestel()) << "Connection init finished successfully" << evc04Connection;

        m_evc04Connections.insert(thing, evc04Connection);
        info->finish(Thing::ThingErrorNoError);

        thing->setStateValue(evc04ConnectedStateTypeId, true);
        thing->setStateValue(evc04VersionStateTypeId, QString(QString::fromUtf16(evc04Connection->firmwareVersion().data(), evc04Connection->firmwareVersion().length()).toUtf8()).trimmed());

        evc04Connection->update();
    });

    connect(evc04Connection, &EVC04ModbusTcpConnection::updateFinished, thing, [this, evc04Connection, thing](){
        qCDebug(dcVestel()) << "EVC04 update finished:" << thing->name() << evc04Connection;

        qCDebug(dcVestel()) << "Serial:" << QString(QString::fromUtf16(evc04Connection->serialNumber().data(), evc04Connection->serialNumber().length()).toUtf8()).trimmed();
        qCDebug(dcVestel()) << "ChargePoint ID:" << QString(QString::fromUtf16(evc04Connection->chargepointId().data(), evc04Connection->chargepointId().length()).toUtf8()).trimmed();
        qCDebug(dcVestel()) << "Brand:" << QString(QString::fromUtf16(evc04Connection->brand().data(), evc04Connection->brand().length()).toUtf8()).trimmed();
        qCDebug(dcVestel()) << "Model:" << QString(QString::fromUtf16(evc04Connection->model().data(), evc04Connection->model().length()).toUtf8()).trimmed();

        updateEVC04MaxCurrent(thing);

        // I've been observing the wallbox getting stuck on modbus. It is still functional, but modbus keeps on returning the same old values
        // until the TCP connection is closed and reopened. Checking the wallbox time register to detect that and auto-reconnect.
        if (m_lastWallboxTime[thing] == evc04Connection->time()) {
            qCWarning(dcVestel()) << "Wallbox seems stuck and returning outdated values. Reconnecting...";
            evc04Connection->reconnectDevice();
        }
        m_lastWallboxTime[thing] = evc04Connection->time();
    });

    connect(evc04Connection, &EVC04ModbusTcpConnection::chargepointStateChanged, thing, [thing](EVC04ModbusTcpConnection::ChargePointState chargePointState) {
        qCDebug(dcVestel()) << "Chargepoint state changed" << thing->name() << chargePointState;
        //        switch (chargePointState) {
        //        case EVC04ModbusTcpConnection::ChargePointStateAvailable:
        //        case EVC04ModbusTcpConnection::ChargePointStatePreparing:
        //        case EVC04ModbusTcpConnection::ChargePointStateReserved:
        //        case EVC04ModbusTcpConnection::ChargePointStateUnavailable:
        //        case EVC04ModbusTcpConnection::ChargePointStateFaulted:
        //            thing->setStateValue(evc04PluggedInStateTypeId, false);
        //            break;
        //        case EVC04ModbusTcpConnection::ChargePointStateCharging:
        //        case EVC04ModbusTcpConnection::ChargePointStateSuspendedEVSE:
        //        case EVC04ModbusTcpConnection::ChargePointStateSuspendedEV:
        //        case EVC04ModbusTcpConnection::ChargePointStateFinishing:
        //            thing->setStateValue(evc04PluggedInStateTypeId, true);
        //            break;
        //        }
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::chargingStateChanged, thing, [thing](EVC04ModbusTcpConnection::ChargingState chargingState) {
        qCDebug(dcVestel()) << "Charging state changed:" << chargingState;
        thing->setStateValue(evc04ChargingStateTypeId, chargingState == EVC04ModbusTcpConnection::ChargingStateCharging);
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::activePowerTotalChanged, thing, [thing](quint16 activePowerTotal) {
        qCDebug(dcVestel()) << "Total active power:" << activePowerTotal;
        // The wallbox reports some 5-6W even when there's nothing connected. Let's hide that if we're not charging
        if (thing->stateValue(evc04ChargingStateTypeId).toBool() == true) {
            thing->setStateValue(evc04CurrentPowerStateTypeId, activePowerTotal);
        } else {
            thing->setStateValue(evc04CurrentPowerStateTypeId, 0);
        }
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::meterReadingChanged, thing, [thing](quint32 meterReading) {
        qCDebug(dcVestel()) << "Meter reading changed:" << meterReading;
        thing->setStateValue(evc04TotalEnergyConsumedStateTypeId, meterReading / 10.0);
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::sessionMaxCurrentChanged, thing, [](quint16 sessionMaxCurrent) {
        // This mostly just reflects what we've been writing to cargingCurrent, so not of much use...
        qCDebug(dcVestel()) << "Session max current changed:" << sessionMaxCurrent;
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::cableMaxCurrentChanged, thing, [this, thing](quint16 cableMaxCurrent) {
        qCDebug(dcVestel()) << "Cable max current changed:" << cableMaxCurrent;
        updateEVC04MaxCurrent(thing);
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::evseMinCurrentChanged, thing, [thing](quint16 evseMinCurrent) {
        qCDebug(dcVestel()) << "EVSE min current changed:" << evseMinCurrent;
        thing->setStateMinValue(evc04MaxChargingCurrentStateTypeId, evseMinCurrent);
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::evseMaxCurrentChanged, thing, [this, thing](quint16 evseMaxCurrent) {
        qCDebug(dcVestel()) << "EVSE max current changed:" << evseMaxCurrent;
        updateEVC04MaxCurrent(thing);
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::sessionEnergyChanged, thing, [thing](quint32 sessionEnergy) {
        qCDebug(dcVestel()) << "Session energy changed:" << sessionEnergy;
        thing->setStateValue(evc04SessionEnergyStateTypeId, sessionEnergy / 1000.0);
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::chargingCurrentChanged, thing, [thing](quint16 chargingCurrent) {
        qCDebug(dcVestel()) << "Charging current changed:" << chargingCurrent;
        if (chargingCurrent > 0) {
            thing->setStateValue(evc04PowerStateTypeId, true);
            thing->setStateValue(evc04MaxChargingCurrentStateTypeId, chargingCurrent);
        } else {
            thing->setStateValue(evc04PowerStateTypeId, false);
        }
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::numPhasesChanged, thing, [thing](EVC04ModbusTcpConnection::NumPhases numPhases) {
        switch (numPhases) {
        case EVC04ModbusTcpConnection::NumPhases1:
            thing->setStateValue(evc04PhaseCountStateTypeId, 1);
            break;
        case EVC04ModbusTcpConnection::NumPhases3:
            thing->setStateValue(evc04PhaseCountStateTypeId, 3);
            break;
        }
    });
    connect(evc04Connection, &EVC04ModbusTcpConnection::cableStateChanged, thing, [evc04Connection, thing](EVC04ModbusTcpConnection::CableState cableState) {
        switch (cableState) {
        case EVC04ModbusTcpConnection::CableStateNotConnected:
        case EVC04ModbusTcpConnection::CableStateCableConnectedVehicleNotConnected:
            thing->setStateValue(evc04PluggedInStateTypeId, false);
            break;
        case EVC04ModbusTcpConnection::CableStateCableConnectedVehicleConnected:
        case EVC04ModbusTcpConnection::CableStateCableConnectedVehicleConnectedCableLocked:
            thing->setStateValue(evc04PluggedInStateTypeId, true);

            // The car was plugged in, sync the power state now as the wallbox only allows to set that when the car is connected
            if (thing->stateValue(evc04PowerStateTypeId).toBool() == false) {
                qCInfo(dcVestel()) << "Car plugged in. Syncing cached power off state to wallbox";
                evc04Connection->setChargingCurrent(0);
            } else {
                qCInfo(dcVestel()) << "Car plugged in. Syncing cached current valie to wallbox";
                evc04Connection->setChargingCurrent(static_cast<quint16>(thing->stateValue(evc04MaxChargingCurrentStateTypeId).toUInt()));
            }

            break;
        }
    });

    evc04Connection->connectDevice();
}

void IntegrationPluginVestel::updateEVC04MaxCurrent(Thing *thing)
{
    EVC04ModbusTcpConnection *connection = m_evc04Connections.value(thing);
    quint16 wallboxMax = connection->maxChargePointPower() > 0 ? connection->maxChargePointPower() / 230 : 32;
    quint16 evseMax = connection->evseMaxCurrent() > 0 ? connection->evseMaxCurrent() : wallboxMax;
    quint16 cableMax = connection->cableMaxCurrent() > 0 ? connection->cableMaxCurrent() : wallboxMax;


    quint8 overallMax = qMin(qMin(wallboxMax, evseMax), cableMax);
    qCDebug(dcVestel()) << "Adjusting max current: Wallbox max:" << wallboxMax << "EVSE max:" << evseMax << "cable max:" << cableMax << "Overall:" << overallMax;
    thing->setStateMinMaxValues(evc04MaxChargingCurrentStateTypeId, 6, overallMax);
}
