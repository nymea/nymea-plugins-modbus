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

#include "integrationpluginschneiderwallbox.h"
#include "plugininfo.h"

#include <network/networkdevicediscovery.h>
#include <types/param.h>
#include <plugintimer.h>

#include <QDebug>
#include <QStringList>
#include <QJsonDocument>
#include <QNetworkInterface>


IntegrationPluginSchneiderWallbox::IntegrationPluginSchneiderWallbox()
{

}

void IntegrationPluginSchneiderWallbox::discoverThings(ThingDiscoveryInfo *info)
{
    if (!hardwareManager()->networkDeviceDiscovery()->available()) {
        qCWarning(dcSchneiderElectric()) << "Failed to discover network devices. The network device discovery is not available.";
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The discovery is not available. Please enter the IP address manually."));
        return;
    }

    qCDebug(dcSchneiderElectric()) << "Starting network discovery...";
    NetworkDeviceDiscoveryReply *discoveryReply = hardwareManager()->networkDeviceDiscovery()->discover();
    connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, info, [=](){
        qCDebug(dcSchneiderElectric()) << "Discovery finished. Found" << discoveryReply->networkDeviceInfos().count() << "devices";
        foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {
            qCDebug(dcSchneiderElectric()) << networkDeviceInfo;

            if (!networkDeviceInfo.macAddressManufacturer().contains("TELEMECANIQUE", Qt::CaseSensitivity::CaseInsensitive)) {
                continue;
            }

            ThingClass thingClass = supportedThings().findById(info->thingClassId());
            ParamTypeId macAddressParamType = thingClass.paramTypes().findByName("mac").id();

            ThingDescriptor descriptor(info->thingClassId(), thingClass.displayName(), networkDeviceInfo.address().toString());
            descriptor.setParams({Param(macAddressParamType, networkDeviceInfo.macAddress())});

            // Check if we already have set up this device
            Thing *existingThing = myThings().findByParams(descriptor.params());
            if (existingThing) {
                qCDebug(dcSchneiderElectric()) << "Found already existing" << thingClass.name() << "wallbox:" << existingThing->name() << networkDeviceInfo;
                descriptor.setThingId(existingThing->id());
            } else {
                qCDebug(dcSchneiderElectric()) << "Found new" << thingClass.name() << "wallbox";
            }

            info->addThingDescriptor(descriptor);
        }
        info->finish(Thing::ThingErrorNoError);
    });
}


void IntegrationPluginSchneiderWallbox::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    /* Probably not needed.
    if (thing->thingClassId() != schneiderEVlinkThingClassId) {
        qCWarning(dcSchneiderElectric()) << "Could not setup thing: unhandled device class" << thing->thingClass();
        info->finish(Thing::ThingErrorThingClassNotFound);
        return;
    }
    */

    if (m_schneiderDevices.contains(thing)) {
        qCDebug(dcSchneiderElectric()) << "Reconfiguring existing thing" << thing->name();
        m_schneiderDevices.take(thing)->deleteLater();
    } else {
        qCDebug(dcSchneiderElectric()) << "Setting up a new device:" << thing->params();
    }

    MacAddress mac = MacAddress(thing->paramValue("mac").toString());
    if (!mac.isValid()) {
        info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The given MAC address is not valid."));
        return;
    }
    NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(mac);

    uint port = thing->paramValue("port").toUInt();
    quint16 slaveId = thing->paramValue("slaveId").toUInt();
    SchneiderWallboxModbusTcpConnection *schneiderWallboxTcpConnection = new SchneiderWallboxModbusTcpConnection(monitor->networkDeviceInfo().address(), port, slaveId, this);
    SchneiderWallbox *schneiderWallbox = new SchneiderWallbox(schneiderWallboxTcpConnection, this);
    connect(info, &ThingSetupInfo::aborted, schneiderWallbox, &SchneiderWallbox::deleteLater);
    connect(info, &ThingSetupInfo::aborted, monitor, [monitor, this](){ hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(monitor);});

    // ToDO: Check if this is really a Schneider wallbox.

    quint16 minCurrentLimit = thing->paramValue("minChargeCurrent").toUInt();
    quint16 maxCurrentLimit = thing->paramValue("maxChargeCurrent").toUInt();
    thing->setStateMinMaxValues("maxChargingCurrent", minCurrentLimit, maxCurrentLimit);

    connect(schneiderWallboxTcpConnection, &SchneiderWallboxModbusTcpConnection::reachableChanged, thing, [schneiderWallboxTcpConnection, thing](bool reachable){
        qCDebug(dcSchneiderElectric()) << "Reachable state changed" << reachable;
        if (reachable) {
            schneiderWallboxTcpConnection->initialize();
        } else {
            thing->setStateValue("connected", false);
        }
    });

    // Only during setup
    connect(schneiderWallboxTcpConnection, &SchneiderWallboxModbusTcpConnection::initializationFinished, info, [this, thing, schneiderWallbox, monitor, info](bool success){
        if (success) {
            qCDebug(dcSchneiderElectric()) << "Schneider wallbox initialized.";
            m_schneiderDevices.insert(thing, schneiderWallbox);
            m_monitors.insert(thing, monitor);
            info->finish(Thing::ThingErrorNoError);
        } else {
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(monitor);
            schneiderWallbox->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("Could not initialize the communication with the wallbox."));
        }
    });

    connect(schneiderWallboxTcpConnection, &SchneiderWallboxModbusTcpConnection::updateFinished, thing, [schneiderWallboxTcpConnection, thing](){
        qCDebug(dcSchneiderElectric()) << "Update finished:" << thing->name() << schneiderWallboxTcpConnection;
    });

    connect(schneiderWallboxTcpConnection, &SchneiderWallboxModbusTcpConnection::initializationFinished, thing, [thing](bool success){
        if (success) {
            thing->setStateValue("connected", true);
        }
    });

    // Handle property changed signals
    connect(schneiderWallboxTcpConnection, &SchneiderWallboxModbusTcpConnection::chargeTimeChanged, this, [thing](quint32 chargeTime){
        quint32 chargeTimeMinutes{chargeTime / 60};
        qCDebug(dcSchneiderElectric()) << thing << "charging time changed" << chargeTimeMinutes << "minutes";
        thing->setStateValue("chargeTime", chargeTimeMinutes);
    });

    connect(schneiderWallboxTcpConnection, &SchneiderWallboxModbusTcpConnection::cpwStateChanged, this, [thing, this](SchneiderWallboxModbusTcpConnection::CPWState cpwState){
        qCDebug(dcSchneiderElectric()) << thing << "CPW state changed" << cpwState;
        setCpwState(thing, cpwState);
    });

    connect(schneiderWallboxTcpConnection, &SchneiderWallboxModbusTcpConnection::lastChargeStatusChanged, this, [thing, this](SchneiderWallboxModbusTcpConnection::LastChargeStatus lastChargeStatus){
        qCDebug(dcSchneiderElectric()) << thing << "Last charge status changed" << lastChargeStatus;
        setLastChargeStatus(thing, lastChargeStatus);
    });

    connect(schneiderWallboxTcpConnection, &SchneiderWallboxModbusTcpConnection::stationEnergyChanged, this, [thing](quint32 stationEnergyInWattHours){
        double stationEnergyInKilowattHours{(static_cast<double>(stationEnergyInWattHours)) / 1000.0};
        qCDebug(dcSchneiderElectric()) << thing << "station energy changed" << stationEnergyInKilowattHours << "kWh";
        thing->setStateValue("totalEnergyConsumed", stationEnergyInKilowattHours);
    });

    connect(schneiderWallboxTcpConnection, &SchneiderWallboxModbusTcpConnection::stationPowerTotalChanged, this, [thing](float stationPowerTotalKilowatt){
        double stationPowerTotalWatt{stationPowerTotalKilowatt * 1000};
        qCDebug(dcSchneiderElectric()) << thing << "station total power changed" << stationPowerTotalWatt << "W";
        thing->setStateValue("currentPower", stationPowerTotalWatt);
        thing->setStateValue("charging", stationPowerTotalWatt > 20);
    });

    connect(schneiderWallboxTcpConnection, &SchneiderWallboxModbusTcpConnection::errorStatusChanged, this, [thing, this](quint32 errorBits){
        qCDebug(dcSchneiderElectric()) << thing << "Error state changed" << errorBits ;
        setErrorMessage(thing, errorBits);
    });

    schneiderWallboxTcpConnection->connectDevice();

    connect(schneiderWallbox, &SchneiderWallbox::phaseCountChanged, this, [thing, this](quint16 phaseCount){
        qCDebug(dcSchneiderElectric()) << thing << "Phase count changed" << phaseCount ;
        setPhaseCount(thing, phaseCount);
    });

    /* Probably not needed.
    connect(schneiderWallbox, &SchneiderWallbox::currentPowerChanged, this, [thing, this](double currentPower){
        qCDebug(dcSchneiderElectric()) << thing << "Current power changed" << currentPower ;
        setCurrentPower(thing, currentPower);
        thing->setStateValue("currentPower", currentPower);
        thing->setStateValue("charging", currentPower > 20);
    });
    */
}


void IntegrationPluginSchneiderWallbox::postSetupThing(Thing *thing)
{
    qCDebug(dcSchneiderElectric()) << "Post setup" << thing->name();
    if (thing->thingClassId() != schneiderEVlinkThingClassId) {
        qCWarning(dcSchneiderElectric()) << "Thing class id not supported" << thing->thingClassId();
        return;
    }

    if (!m_pluginTimer) {
        qCDebug(dcSchneiderElectric()) << "Starting plugin timer...";
        m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(3);
        connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
            foreach(Thing *thing, myThings()) {
                if (m_monitors.value(thing)->reachable()) {
                    qCDebug(dcSchneiderElectric()) << "Updating" << thing->name();
                    m_schneiderDevices.value(thing)->modbusTcpConnection()->update();
                    m_schneiderDevices.value(thing)->update();
                } else {
                    qCDebug(dcSchneiderElectric()) << thing->name() << "isn't reachable. Not updating.";
                }
            }
        });

        //m_pluginTimer->start();  apparently not needed anymore.
    }
}

void IntegrationPluginSchneiderWallbox::thingRemoved(Thing *thing)
{
    qCDebug(dcSchneiderElectric()) << "Removing device" << thing->name();
    if (m_schneiderDevices.contains(thing)) {
        disconnect(m_schneiderDevices.value(thing)->modbusTcpConnection(), NULL, thing, NULL);  // This is needed to prevent crash on thing remove.
        // m_schneiderDevices.value(thing)->modbusTcpConnection()->disconnect(thing);  // This should do exactly the same thing as the line above. Somehow it does not. This line does not prevent the crash.
        qCDebug(dcSchneiderElectric()) << "Modbus disconnected";
        m_schneiderDevices.take(thing)->deleteLater();
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
    }

    if (myThings().isEmpty()) {
        qCDebug(dcSchneiderElectric()) << "Stopping plugin timers ...";
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginSchneiderWallbox::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    SchneiderWallboxModbusTcpConnection *connection = m_schneiderDevices.value(thing)->modbusTcpConnection();
    if (!connection) {
        qCWarning(dcSchneiderElectric()) << "Modbus connection not available";
        info->finish(Thing::ThingErrorHardwareFailure);
        return;
    }

    if (!connection->connected()) {
        qCWarning(dcSchneiderElectric()) << "Could not execute action. The modbus connection is currently not available.";
        info->finish(Thing::ThingErrorHardwareNotAvailable);
        return;
    }


    SchneiderWallbox *device = m_schneiderDevices.value(thing);
    ActionType actionType = thing->thingClass().actionTypes().findById(info->action().actionTypeId());
    bool success = false;
    if (actionType.name() == "power") {
        bool onOff = action.paramValue(actionType.id()).toBool();
        success = device->enableOutput(onOff);
        thing->setStateValue("power", onOff);
        if (onOff) {
            // You can turn the wallbox on without specifying a charge current. The thing object saves the last current setpoint, the app displays that.
            // Need to get that saved value and give it to schneiderwallbox.cpp so the displayed value matches the actual setpoint.
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
        qCWarning(dcSchneiderElectric()) << "Action execution finished with error.";
        info->finish(Thing::ThingErrorHardwareFailure);
        return;
    }
}

void IntegrationPluginSchneiderWallbox::setCpwState(Thing *thing, SchneiderWallboxModbusTcpConnection::CPWState state)
{
    bool isPluggedIn{false};    // ToDo: Wallbox states überprüfen, welche tatsächlich bei "Stecker steckt" angezeigt werden.
    switch (state) {
    case SchneiderWallboxModbusTcpConnection::CPWStateEvseNotAvailable:
        thing->setStateValue("cpwState", "EVSE not available");
        break;
    case SchneiderWallboxModbusTcpConnection::CPWStateEvseAvailable:
        thing->setStateValue("cpwState", "EVSE available");
        break;
    case SchneiderWallboxModbusTcpConnection::CPWStatePlugDetected:
        thing->setStateValue("cpwState", "Plug detected");
        break;
    case SchneiderWallboxModbusTcpConnection::CPWStateEvConnected:
        thing->setStateValue("cpwState", "EV connected");
        isPluggedIn = true;
        break;
    case SchneiderWallboxModbusTcpConnection::CPWStateEvConnected2:
        thing->setStateValue("cpwState", "EV connected 2");
        isPluggedIn = true;
        break;
    case SchneiderWallboxModbusTcpConnection::CPWStateEvConnectedVentilationRequired:
        thing->setStateValue("cpwState", "EV connected ventilation required");
        isPluggedIn = true;
        break;
    case SchneiderWallboxModbusTcpConnection::CPWStateEvseReady:
        thing->setStateValue("cpwState", "EVSE ready");
        isPluggedIn = true;
        break;
    case SchneiderWallboxModbusTcpConnection::CPWStateEvReady:
        thing->setStateValue("cpwState", "EV ready");
        isPluggedIn = true;
        break;
    case SchneiderWallboxModbusTcpConnection::CPWStateCharging:
        thing->setStateValue("cpwState", "Charging");
        isPluggedIn = true;
        break;
    case SchneiderWallboxModbusTcpConnection::CPWStateEvReadyVentilationRequired:
        thing->setStateValue("cpwState", "EV ready ventilation required");
        isPluggedIn = true;
        break;
    case SchneiderWallboxModbusTcpConnection::CPWStateChargingVentilationRequired:
        thing->setStateValue("cpwState", "Charging ventilation required");
        isPluggedIn = true;
        break;
    case SchneiderWallboxModbusTcpConnection::CPWStateStopCharging:
        thing->setStateValue("cpwState", "Stop charging");
        break;
    case SchneiderWallboxModbusTcpConnection::CPWStateAlarm:
        thing->setStateValue("cpwState", "Alarm");
        break;
    case SchneiderWallboxModbusTcpConnection::CPWStateShortcut:
        thing->setStateValue("cpwState", "Short circuit");
        break;
    case SchneiderWallboxModbusTcpConnection::CPWStateDigitalComByEvseState:
        thing->setStateValue("cpwState", "Digital com by EVSE state");
        break;
    default:
        thing->setStateValue("cpwState", "EVSE not available");
    }
    thing->setStateValue("pluggedIn", isPluggedIn);
}

void IntegrationPluginSchneiderWallbox::setLastChargeStatus(Thing *thing, SchneiderWallboxModbusTcpConnection::LastChargeStatus status)
{
    switch (status) {
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusCircuitBreakerEnabled:
        thing->setStateValue("lastChargeStatus", "Circuit breaker enabled (emergency)");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusOk:
        thing->setStateValue("lastChargeStatus", "OK (ended by EV in mode 3)");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusEndedByClusterManagerLoss:
        thing->setStateValue("lastChargeStatus", "Ended by cluster manager loss (lifebit timeout 10s)");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusEndOfChargeInSm3:
        thing->setStateValue("lastChargeStatus", "End of charge in SM3 (low current)");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusCommunicationError:
        thing->setStateValue("lastChargeStatus", "Communication error (PWM lost)");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusDisconnectionCable:
        thing->setStateValue("lastChargeStatus", "Disconnection cable (Electric Vehicle Supply Equipment plug off)");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusDisconnectionEv:
        thing->setStateValue("lastChargeStatus", "Disconnection EV (EV plug off)");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusShortcut:
        thing->setStateValue("lastChargeStatus", "Shortcut");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusOverload:
        thing->setStateValue("lastChargeStatus", "Overload");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusCanceledBySupervisor:
        thing->setStateValue("lastChargeStatus", "Canceled by supervisor (external command)");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusVentilationNotAllowed:
        thing->setStateValue("lastChargeStatus", "Ventillation not allowed");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusUnexpectedContactorOpen:
        thing->setStateValue("lastChargeStatus", "Unexpected contactor open");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusSimplifiedMode3NotAllowed:
        thing->setStateValue("lastChargeStatus", "Simplified mode 3 not allowed");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusPowerSupplyInternalError:
        thing->setStateValue("lastChargeStatus", "Power supply internal error(contactor not able to close)");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusUnexpectedPlugUnlock:
        thing->setStateValue("lastChargeStatus", "Unexpected plug unlock");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusDefaultNbPhases:
        thing->setStateValue("lastChargeStatus", "Default Nb Phases (triphase not allowed)");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusDiDefaultSurgeArrestor:
        thing->setStateValue("lastChargeStatus", "DI default Surge arrestor");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusDiDefaultAntiIntrusion:
        thing->setStateValue("lastChargeStatus", "DI default Anti Intrusion");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusDiDefaultShutterUnlock:
        thing->setStateValue("lastChargeStatus", "DI default Shutter Unlock");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusDiDefaultFlsi:
        thing->setStateValue("lastChargeStatus", "DI default FLSI (Force Load Shedding Input)");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusDiDefaultEmergencyStop:
        thing->setStateValue("lastChargeStatus", "DI default Emergency Stop");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusDiDefaultUndervoltage:
        thing->setStateValue("lastChargeStatus", "DI default Undervoltage");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusDiDefaultCi:
        thing->setStateValue("lastChargeStatus", "DI default CI (Conditional Input)");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusOther:
        thing->setStateValue("lastChargeStatus", "Other");
        break;
    case SchneiderWallboxModbusTcpConnection::LastChargeStatusUndefined:
    default:
        thing->setStateValue("lastChargeStatus", "Undefined");
    }
}

void IntegrationPluginSchneiderWallbox::setPhaseCount(Thing *thing, quint16 phaseCount) {
    thing->setStateValue("phaseCount", phaseCount);
    //qCDebug(dcSchneiderElectric()) << "Phase count set:" << phaseCount << " for thing:" << thing;
}

void IntegrationPluginSchneiderWallbox::setErrorMessage(Thing *thing, quint32 errorBits) {
    if (!errorBits) {
        thing->setStateValue("errorMessage", "No error");
    } else {
        QString errorMessage{""};
        quint32 testBit{0b01};
        if (errorBits & testBit) {
            errorMessage.append("Lost communication with RFID reader, ");
        }
        if (errorBits & (testBit << 1)) {
            errorMessage.append("Lost communication with display, ");
        }
        if (errorBits & (testBit << 2)) {
            errorMessage.append("Cannot connect to master board, ");
        }
        if (errorBits & (testBit << 3)) {
            errorMessage.append("Incorrect plug lock state, ");
        }
        if (errorBits & (testBit << 4)) {
            errorMessage.append("Incorrect contactor state, ");
        }
        if (errorBits & (testBit << 5)) {
            errorMessage.append("Incorrect surge arrestor state, ");
        }
        if (errorBits & (testBit << 6)) {
            errorMessage.append("Incorrect anti intrusion state, ");
        }
        if (errorBits & (testBit << 7)) {
            errorMessage.append("Cannot connect to US daughter board, ");
        }
        if (errorBits & (testBit << 8)) {
            errorMessage.append("Configuration file missing, corrupted or already open, ");
        }
        if (errorBits & (testBit << 9)) {
            errorMessage.append("Incorrect shutter lock state, ");
        }
        if (errorBits & (testBit << 10)) {
            errorMessage.append("Incorrect circuit breaker state, ");
        }
        if (errorBits & (testBit << 11)) {
            errorMessage.append("Lost communication with powermeter, ");
        }
        if (errorBits & (testBit << 12)) {
            errorMessage.append("remote controller lost, ");
        }
        if (errorBits & (testBit << 13)) {
            errorMessage.append("Incorrect socket state, ");
        }
        if (errorBits & (testBit << 14)) {
            errorMessage.append("Incorrect charging phase number, ");
        }
        if (errorBits & (testBit << 15)) {
            errorMessage.append("Lost communication with cluster manager, ");
        }
        if (errorBits & (testBit << 16)) {
            errorMessage.append("Mode3 communication error (CP error), ");
        }

        if (errorBits & (testBit << 17)) {
            errorMessage.append("Incorrect cable state (PP error), ");
        }

        if (errorBits & (testBit << 18)) {
            errorMessage.append("Default EV charging cable disconnection, ");
        }

        if (errorBits & (testBit << 19)) {
            errorMessage.append("Short circuit FP1, ");
        }

        if (errorBits & (testBit << 20)) {
            errorMessage.append("Overcurrent, ");
        }
        if (errorBits & (testBit << 21)) {
            errorMessage.append("No energy available for charging, ");
        }
        if (errorBits >> 22) {
            // Test if there are any bits that are not in the list.
            errorMessage.append("Unknown error, ");
        }
        int stringLength = errorMessage.length();

        // stringLength should be > 1, but just in case.
        if (stringLength > 1) {
            errorMessage.replace(stringLength - 2, 2, "."); // Replace ", " at the end with ".".
        }
        thing->setStateValue("errorMessage", errorMessage);
    }
}
