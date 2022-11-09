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

#include "integrationplugintechnivolt.h"
#include "plugininfo.h"

#include <network/networkdevicediscovery.h>
#include <types/param.h>
#include <plugintimer.h>

#include <QDebug>
#include <QStringList>
#include <QJsonDocument>
#include <QNetworkInterface>


IntegrationPluginTechnivolt::IntegrationPluginTechnivolt()
{

}

void IntegrationPluginTechnivolt::discoverThings(ThingDiscoveryInfo *info)
{
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcTechnivoltWallbox()) << "The network discovery is not available on this platform.";
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The network device discovery is not available."));
        return;
    }

    qCDebug(dcTechnivoltWallbox()) << "Starting network discovery...";
    NetworkDeviceDiscoveryReply *discoveryReply = hardwareManager()->networkDeviceDiscovery()->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, info, [=](){
        qCDebug(dcTechnivoltWallbox()) << "Discovery finished. Found" << discoveryReply->networkDeviceInfos().count() << "devices";
        foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {
            qCDebug(dcTechnivoltWallbox())  << networkDeviceInfo;

            if (!networkDeviceInfo.macAddressManufacturer().contains("Bender", Qt::CaseSensitivity::CaseInsensitive)) {
                continue;
            }

            ThingClass thingClass = supportedThings().findById(info->thingClassId());
            ParamTypeId macAddressParamType = thingClass.paramTypes().findByName("mac").id();

            ThingDescriptor descriptor(info->thingClassId(), thingClass.displayName(), networkDeviceInfo.address().toString());
            descriptor.setParams({Param(macAddressParamType, networkDeviceInfo.macAddress())});

            // Check if we already have set up this device
            Thing *existingThing = myThings().findByParams(descriptor.params());
            if (existingThing) {
                qCDebug(dcTechnivoltWallbox()) << "Found already existing" << thingClass.name() << "wallbox:" << existingThing->name() << networkDeviceInfo;
                descriptor.setThingId(existingThing->id());
            } else {
                qCDebug(dcTechnivoltWallbox()) << "Found new" << thingClass.name() << "wallbox";
            }

            info->addThingDescriptor(descriptor);
        }
        info->finish(Thing::ThingErrorNoError);
    });
}


void IntegrationPluginTechnivolt::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    if (m_wallboxDevices.contains(thing)) {
        qCDebug(dcTechnivoltWallbox()) << "Reconfiguring existing thing" << thing->name();
        m_wallboxDevices.take(thing)->deleteLater();
    } else {
        qCDebug(dcTechnivoltWallbox()) << "Setting up a new device:" << thing->params();
    }

    MacAddress mac = MacAddress(thing->paramValue("mac").toString());
    if (!mac.isValid()) {
        info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The given MAC address is not valid."));
        return;
    }
    NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(mac);

    uint port = thing->paramValue("port").toUInt();
    quint16 slaveId = thing->paramValue("slaveId").toUInt();
    TechnivoltModbusTcpConnection *technivoltModbusTcpConnection = new TechnivoltModbusTcpConnection(monitor->networkDeviceInfo().address(), port, slaveId, this);
    TechnivoltWallbox *technivoltWallbox = new TechnivoltWallbox(technivoltModbusTcpConnection, this);
    connect(info, &ThingSetupInfo::aborted, technivoltWallbox, &TechnivoltWallbox::deleteLater);
    connect(info, &ThingSetupInfo::aborted, monitor, [monitor, this](){ hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(monitor);});

    // ToDO: Check if this is really a Technivolt wallbox.

    // This wallbox has a register for the minimum current, so it does not need to be entered manually.
    quint16 maxCurrentLimit = thing->paramValue("maxChargeCurrent").toUInt();
    thing->setStateMaxValue("maxChargingCurrent", maxCurrentLimit);

    connect(technivoltModbusTcpConnection, &TechnivoltModbusTcpConnection::reachableChanged, thing, [technivoltModbusTcpConnection, thing](bool reachable){
        qCDebug(dcTechnivoltWallbox()) << "Reachable state changed" << reachable;
        if (reachable) {
            technivoltModbusTcpConnection->initialize();
        } else {
            thing->setStateValue("connected", false);
        }
    });

    // Only during setup
    connect(technivoltModbusTcpConnection, &TechnivoltModbusTcpConnection::initializationFinished, info, [this, thing, technivoltWallbox, monitor, info](bool success){
        if (success) {
            qCDebug(dcTechnivoltWallbox()) << "Schneider wallbox initialized.";
            m_wallboxDevices.insert(thing, technivoltWallbox);
            m_monitors.insert(thing, monitor);
            info->finish(Thing::ThingErrorNoError);
        } else {
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(monitor);
            technivoltWallbox->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("Could not initialize the communication with the wallbox."));
        }
    });

    connect(technivoltModbusTcpConnection, &TechnivoltModbusTcpConnection::updateFinished, thing, [technivoltModbusTcpConnection, thing](){
        qCDebug(dcTechnivoltWallbox()) << "Update finished:" << thing->name() << technivoltModbusTcpConnection;
    });

    connect(technivoltModbusTcpConnection, &TechnivoltModbusTcpConnection::initializationFinished, thing, [thing](bool success){
        if (success) {
            thing->setStateValue("connected", true);
        }
    });

    // Handle property changed signals
    connect(technivoltModbusTcpConnection, &TechnivoltModbusTcpConnection::sessionTimeChanged, this, [thing](quint32 sessionTime){
        quint32 chargeTimeMinutes{sessionTime / 60};
        qCDebug(dcTechnivoltWallbox()) << thing << "charging time changed" << chargeTimeMinutes << "minutes";
        thing->setStateValue("chargeTime", chargeTimeMinutes);
    });
    connect(technivoltModbusTcpConnection, &TechnivoltModbusTcpConnection::ocppStateChanged, this, [thing, this](TechnivoltModbusTcpConnection::OCPPstatus ocppState){
        qCDebug(dcTechnivoltWallbox()) << thing << "OCPP state changed" << ocppState;
        setOcppState(thing, ocppState);
    });
    connect(technivoltModbusTcpConnection, &TechnivoltModbusTcpConnection::chargedEnergySessionChanged, this, [thing](quint32 chargedEnergySessionWattHours){
        double chargedEnergySessionKiloWattHours{(static_cast<double>(chargedEnergySessionWattHours)) / 1000};
        qCDebug(dcTechnivoltWallbox()) << thing << "charged energy session changed" << chargedEnergySessionKiloWattHours << "kWh";
        thing->setStateValue("sessionEnergy", chargedEnergySessionKiloWattHours);
    });
    connect(technivoltModbusTcpConnection, &TechnivoltModbusTcpConnection::totalEnergyChanged, this, [thing](quint32 totalEnergyWattHours){
        double totalEnergyKiloWattHours{(static_cast<double>(totalEnergyWattHours)) / 1000};
        qCDebug(dcTechnivoltWallbox()) << thing << "charged energy total changed" << totalEnergyKiloWattHours << "kWh";
        thing->setStateValue("totalEnergyConsumed", totalEnergyKiloWattHours);
    });
    connect(technivoltModbusTcpConnection, &TechnivoltModbusTcpConnection::totalPowerChanged, this, [thing](quint32 totalPowerWatt){
        qCDebug(dcTechnivoltWallbox()) << thing << "station power changed" << totalPowerWatt << "W";
        if (totalPowerWatt > 20) {  // Use 20 W as lower limit instead of 0. Sometimes small currents are possible without the vehicle actually charging.
            thing->setStateValue("charging", true);
        } else {
            thing->setStateValue("charging", false);
        }
        thing->setStateValue("currentPower", static_cast<double>(totalPowerWatt));
    });
    connect(technivoltModbusTcpConnection, &TechnivoltModbusTcpConnection::minimumCurrentLimitChanged, this, [thing](quint16 minimumCurrentLimit){
        qCDebug(dcTechnivoltWallbox()) << thing << "minimum current limit changed" << minimumCurrentLimit << "A";
        thing->setStateMinValue("maxChargingCurrent", minimumCurrentLimit);
    });

    connect(technivoltModbusTcpConnection, &TechnivoltModbusTcpConnection::errorCodes1Changed, this, [thing, this](quint32 errorCodes1){
        qCDebug(dcTechnivoltWallbox()) << thing << "error codes 1 changed" << errorCodes1;
        m_errorCodes1 = errorCodes1;
        setErrorMessage(thing);
    });
    connect(technivoltModbusTcpConnection, &TechnivoltModbusTcpConnection::errorCodes2Changed, this, [thing, this](quint32 errorCodes2){
        qCDebug(dcTechnivoltWallbox()) << thing << "error codes 2 changed" << errorCodes2;
        m_errorCodes2 = errorCodes2;
        setErrorMessage(thing);
    });
    connect(technivoltModbusTcpConnection, &TechnivoltModbusTcpConnection::errorCodes3Changed, this, [thing, this](quint32 errorCodes3){
        qCDebug(dcTechnivoltWallbox()) << thing << "error codes 3 changed" << errorCodes3;
        m_errorCodes3 = errorCodes3;
        setErrorMessage(thing);
    });
    connect(technivoltModbusTcpConnection, &TechnivoltModbusTcpConnection::errorCodes4Changed, this, [thing, this](quint32 errorCodes4){
        qCDebug(dcTechnivoltWallbox()) << thing << "error codes 4 changed" << errorCodes4;
        m_errorCodes4 = errorCodes4;
        setErrorMessage(thing);
    });

    technivoltModbusTcpConnection->connectDevice();

    connect(technivoltWallbox, &TechnivoltWallbox::phaseCountChanged, this, [thing, this](quint16 phaseCount){
        qCDebug(dcTechnivoltWallbox()) << thing << "Phase count changed" << phaseCount ;
        setPhaseCount(thing, phaseCount);
    });
}


void IntegrationPluginTechnivolt::postSetupThing(Thing *thing)
{
    qCDebug(dcTechnivoltWallbox()) << "Post setup" << thing->name();
    if (thing->thingClassId() != technivoltWallboxThingClassId) {
        qCWarning(dcTechnivoltWallbox()) << "Thing class id not supported" << thing->thingClassId();
        return;
    }

    if (!m_pluginTimer) {
        qCDebug(dcTechnivoltWallbox()) << "Starting plugin timer...";
        m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(3);
        connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
            foreach(Thing *thing, myThings()) {
                if (m_monitors.value(thing)->reachable()) {
                    qCDebug(dcTechnivoltWallbox()) << "Updating" << thing->name();
                    m_wallboxDevices.value(thing)->modbusTcpConnection()->update();
                    m_wallboxDevices.value(thing)->update();
                } else {
                    qCDebug(dcTechnivoltWallbox()) << thing->name() << "isn't reachable. Not updating.";
                }
            }
        });
    }
}

void IntegrationPluginTechnivolt::thingRemoved(Thing *thing)
{
    qCDebug(dcTechnivoltWallbox()) << "Removing device" << thing->name();
    if (m_wallboxDevices.contains(thing)) {
        disconnect(m_wallboxDevices.value(thing)->modbusTcpConnection(), NULL, thing, NULL);  // This is needed to prevent crash on thing remove.
        qCDebug(dcTechnivoltWallbox()) << "Modbus disconnected";
        m_wallboxDevices.take(thing)->deleteLater();
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
    }

    if (myThings().isEmpty()) {
        qCDebug(dcTechnivoltWallbox()) << "Stopping plugin timers ...";
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginTechnivolt::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    TechnivoltModbusTcpConnection *connection = m_wallboxDevices.value(thing)->modbusTcpConnection();
    if (!connection) {
        qCWarning(dcTechnivoltWallbox()) << "Modbus connection not available";
        info->finish(Thing::ThingErrorHardwareFailure);
        return;
    }

    if (!connection->connected()) {
        qCWarning(dcTechnivoltWallbox()) << "Could not execute action. The modbus connection is currently not available.";
        info->finish(Thing::ThingErrorHardwareNotAvailable);
        return;
    }

    TechnivoltWallbox *device = m_wallboxDevices.value(thing);
    ActionType actionType = thing->thingClass().actionTypes().findById(info->action().actionTypeId());
    bool success = false;
    if (actionType.name() == "power") {
        bool onOff = action.paramValue(actionType.id()).toBool();
        success = device->enableOutput(onOff);
        thing->setStateValue("power", onOff);
        if (onOff) {
            // You can turn the wallbox on without specifying a charge current. The thing object saves the last current setpoint, the app displays that.
            // Need to get that saved value and give it to technivoltwallbox.cpp so the displayed value matches the actual setpoint.
            int ampereValue = thing->stateValue("maxChargingCurrent").toUInt();
            success = device->setMaxAmpere(ampereValue);
        }
    } else if (actionType.name() == "maxChargingCurrent") {
        int ampereValue = action.paramValue(actionType.id()).toUInt();
        success = device->setMaxAmpere(ampereValue);
        thing->setStateValue("maxChargingCurrent", ampereValue);
    } else {
        Q_ASSERT_X(false, "executeAction", QString("Unhandled action: %1").arg(actionType.name()).toUtf8());
    }

    if (success) {
        info->finish(Thing::ThingErrorNoError);
    } else {
        qCWarning(dcTechnivoltWallbox()) << "Action execution finished with error.";
        info->finish(Thing::ThingErrorHardwareFailure);
        return;
    }
}

void IntegrationPluginTechnivolt::setOcppState(Thing *thing, TechnivoltModbusTcpConnection::OCPPstatus ocppStatus)
{
    bool isPluggedIn{false};    // ToDo: Wallbox states überprüfen, welche tatsächlich bei "Stecker steckt" angezeigt werden.
    switch (ocppStatus) {
    case TechnivoltModbusTcpConnection::OCPPstatusAvailable:
        thing->setStateValue("ocppStatus", "Available");
        break;
    case TechnivoltModbusTcpConnection::OCPPstatusOccupied:
        thing->setStateValue("ocppStatus", "Occupied");
        isPluggedIn = true;
        break;
    case TechnivoltModbusTcpConnection::OCPPstatusReserved:
        thing->setStateValue("ocppStatus", "Reserved");
        break;
    case TechnivoltModbusTcpConnection::OCPPstatusUnavailable:
        thing->setStateValue("ocppStatus", "Unavailable");
        break;
    case TechnivoltModbusTcpConnection::OCPPstatusFaulted:
        thing->setStateValue("ocppStatus", "Faulted");
        break;
    case TechnivoltModbusTcpConnection::OCPPstatusPreparing:
        thing->setStateValue("ocppStatus", "Preparing");
        break;
    case TechnivoltModbusTcpConnection::OCPPstatusCharging:
        thing->setStateValue("ocppStatus", "Charging");
        isPluggedIn = true;
        break;
    case TechnivoltModbusTcpConnection::OCPPstatusSuspendedEVSE:
        thing->setStateValue("ocppStatus", "Suspended EVSE");
        isPluggedIn = true;
        break;
    case TechnivoltModbusTcpConnection::OCPPstatusSuspendedEV:
        thing->setStateValue("ocppStatus", "Suspended EV");
        break;
    case TechnivoltModbusTcpConnection::OCPPstatusFinishing:
        thing->setStateValue("ocppStatus", "Finishing");
        break;
    default:
        thing->setStateValue("ocppStatus", "Unknown");
    }
    thing->setStateValue("pluggedIn", isPluggedIn);
}

void IntegrationPluginTechnivolt::setPhaseCount(Thing *thing, quint16 phaseCount) {
    thing->setStateValue("phaseCount", phaseCount);
    //qCDebug(dcTechnivoltWallbox()) << "Phase count set:" << phaseCount << " for thing:" << thing;
}

void IntegrationPluginTechnivolt::setErrorMessage(Thing *thing) {

    if (!m_errorCodes1 && !m_errorCodes2 && !m_errorCodes3 && !m_errorCodes4) {
        thing->setStateValue("errorMessage", "No error");
    } else {
        QString errorMessage{""};
        if (m_errorCodes1) {
            errorMessage.append("Error Codes 1: " + QString::number(m_errorCodes1) + ", ");
        }
        if (m_errorCodes2) {
            errorMessage.append("Error Codes 2: " + QString::number(m_errorCodes2) + ", ");
        }
        if (m_errorCodes3) {
            errorMessage.append("Error Codes 3: " + QString::number(m_errorCodes3) + ", ");
        }
        if (m_errorCodes4) {
            errorMessage.append("Error Codes 4: " + QString::number(m_errorCodes4) + ", ");
        }
        int stringLength = errorMessage.length();

        // stringLength should be > 1, but just in case.
        if (stringLength > 1) {
            errorMessage.replace(stringLength - 2, 2, "."); // Replace ", " at the end with ".".
        }
        thing->setStateValue("errorMessage", errorMessage);
    }
}
