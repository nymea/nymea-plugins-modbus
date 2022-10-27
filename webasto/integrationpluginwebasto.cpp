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

#include <network/networkdevicediscovery.h>
#include <types/param.h>

#include <QDebug>
#include <QStringList>
#include <QJsonDocument>
#include <QNetworkInterface>


IntegrationPluginWebasto::IntegrationPluginWebasto()
{
}

void IntegrationPluginWebasto::init()
{
    // FIXME: make use of the internal network discovery if the device gets unavailable. For now, commented out since it has not been used
    // at the moment of changing this.

//    m_discovery = new Discovery(this);
//    connect(m_discovery, &Discovery::finished, this, [this](const QList<Host> &hosts) {

//        foreach (const Host &host, hosts) {
//            if (!host.hostName().contains("webasto", Qt::CaseSensitivity::CaseInsensitive))
//                continue;

//            foreach (Thing *existingThing, myThings()) {
//                if (existingThing->paramValue(liveWallboxThingMacAddressParamTypeId).toString().isEmpty()) {
//                    //This device got probably manually setup, to enable auto rediscovery the MAC address needs to setup
//                    if (existingThing->paramValue(liveWallboxThingIpAddressParamTypeId).toString() == host.address()) {
//                        qCDebug(dcWebasto()) << "Wallbox MAC Address has been discovered" << existingThing->name() << host.macAddress();
//                        existingThing->setParamValue(liveWallboxThingMacAddressParamTypeId, host.macAddress());

//                    }
//                } else if (existingThing->paramValue(liveWallboxThingMacAddressParamTypeId).toString() == host.macAddress())  {
//                    if (existingThing->paramValue(liveWallboxThingIpAddressParamTypeId).toString() != host.address()) {
//                        qCDebug(dcWebasto()) << "Wallbox IP Address has changed, from"  << existingThing->paramValue(liveWallboxThingIpAddressParamTypeId).toString() << "to" << host.address();
//                        existingThing->setParamValue(liveWallboxThingIpAddressParamTypeId, host.address());

//                    } else {
//                        qCDebug(dcWebasto()) << "Wallbox" << existingThing->name() << "IP address has not changed" << host.address();
//                    }
//                    break;
//                }
//            }
//        }
//    });
}

void IntegrationPluginWebasto::discoverThings(ThingDiscoveryInfo *info)
{
    if (info->thingClassId() == liveWallboxThingClassId) {
        if (!hardwareManager()->networkDeviceDiscovery()->available()) {
            qCWarning(dcWebasto()) << "Failed to discover network devices. The network device discovery is not available.";
            info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The discovery is not available."));
            return;
        }

        qCDebug(dcWebasto()) << "Discover things";
        NetworkDeviceDiscoveryReply *discoveryReply = hardwareManager()->networkDeviceDiscovery()->discover();
        connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, discoveryReply, &NetworkDeviceDiscoveryReply::deleteLater);
        connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
            ThingDescriptors descriptors;
            qCDebug(dcWebasto()) << "Discovery finished. Found" << discoveryReply->networkDeviceInfos().count() << "devices";
            foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {
                qCDebug(dcWebasto()) << networkDeviceInfo;
                if (!networkDeviceInfo.hostName().contains("webasto", Qt::CaseSensitivity::CaseInsensitive))
                    continue;

                QString title = "Wallbox ";
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

                ThingDescriptor descriptor(liveWallboxThingClassId, title, description);

                // Check if we already have set up this device
                Things existingThings = myThings().filterByParam(liveWallboxThingIpAddressParamTypeId, networkDeviceInfo.address().toString());
                if (existingThings.count() == 1) {
                    qCDebug(dcWebasto()) << "This thing already exists in the system." << existingThings.first() << networkDeviceInfo;
                    descriptor.setThingId(existingThings.first()->id());
                }

                ParamList params;
                params << Param(liveWallboxThingIpAddressParamTypeId, networkDeviceInfo.address().toString());
                params << Param(liveWallboxThingMacAddressParamTypeId, networkDeviceInfo.macAddress());
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
        connect(webasto, &Webasto::connectionStateChanged, this, &IntegrationPluginWebasto::onConnectionChanged);
        connect(webasto, &Webasto::receivedRegister, this, &IntegrationPluginWebasto::onReceivedRegister);
        connect(webasto, &Webasto::writeRequestError, this, &IntegrationPluginWebasto::onWriteRequestError);
        connect(webasto, &Webasto::writeRequestExecuted, this, &IntegrationPluginWebasto::onWriteRequestExecuted);
        if (!webasto->connectDevice()) {
            qCWarning(dcWebasto()) << "Could not connect to device";
            info->finish(Thing::ThingErrorSetupFailed);
        }
        connect(webasto, &Webasto::connectionStateChanged, info, [info] (bool connected) {
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
    if (!m_pluginTimer) {
        m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(1);
        connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
            Q_FOREACH(Webasto *connection, m_webastoConnections) {
                if (connection->connected())
                    update(connection);
            }
        });
    }

    if (thing->thingClassId() == liveWallboxThingClassId) {
        Webasto *connection = m_webastoConnections.value(thing);
        if (!connection) {
            qCWarning(dcWebasto()) << "Can't find connection to thing";
        }
        update(connection);

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
            bool enabled = action.paramValue(liveWallboxPowerActionPowerParamTypeId).toBool();
            thing->setStateValue(liveWallboxPowerActionTypeId, enabled);
            int ampere = 0;
            if (enabled) {
                ampere = thing->stateValue(liveWallboxMaxChargingCurrentStateTypeId).toUInt();
            }
            QUuid requestId = connection->setChargeCurrent(ampere);
            if (requestId.isNull()) {
                info->finish(Thing::ThingErrorHardwareFailure);
            } else {
                m_asyncActions.insert(requestId, info);
            }
        } else if (action.actionTypeId() == liveWallboxMaxChargingCurrentActionTypeId) {
            int ampere = action.paramValue(liveWallboxMaxChargingCurrentActionMaxChargingCurrentParamTypeId).toUInt();
            thing->setStateValue(liveWallboxMaxChargingCurrentStateTypeId, ampere);
            QUuid requestId = connection->setChargeCurrent(ampere);
            if (requestId.isNull()) {
                info->finish(Thing::ThingErrorHardwareFailure);
            } else {
                m_asyncActions.insert(requestId, info);
            }
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
    uint amperePhase1 = thing->stateValue(liveWallboxCurrentPhase1StateTypeId).toUInt();
    uint amperePhase2 = thing->stateValue(liveWallboxCurrentPhase2StateTypeId).toUInt();
    uint amperePhase3 = thing->stateValue(liveWallboxCurrentPhase3StateTypeId).toUInt();
    // Check how many phases are actually charging, and update the phase count only if something happens on the phases (current or power)
    if (!(amperePhase1 == 0 && amperePhase2 == 0 && amperePhase3 == 0)) {
        uint phaseCount = 0;
        if (amperePhase1 != 0)
            phaseCount += 1;

        if (amperePhase2 != 0)
            phaseCount += 1;

        if (amperePhase3 != 0)
            phaseCount += 1;

        thing->setStateValue(liveWallboxPhaseCountStateTypeId, phaseCount);
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
    Thing *thing = m_webastoConnections.key(connection);
    if (!thing) {
        qCWarning(dcWebasto()) << "On basic information received, thing not found for connection";
        return;
    }
    if (thing->thingClassId() == liveWallboxThingClassId) {
        switch (modbusRegister) {
        case Webasto::TqChargePointState:
            qCDebug(dcWebasto()) << "   - Charge point state:" << Webasto::ChargePointState(data[0]);
            switch (Webasto::ChargePointState(data[0])) {
            case Webasto::ChargePointStateNoVehicleAttached:
                thing->setStateValue(liveWallboxChargePointStateStateTypeId, "No vehicle attached");
                break;
            case Webasto::ChargePointStateVehicleAttachedNoPermission:
                thing->setStateValue(liveWallboxChargePointStateStateTypeId, "Vehicle attached, no permission");
                break;
            case Webasto::ChargePointStateChargingAuthorized:
                thing->setStateValue(liveWallboxChargePointStateStateTypeId, "Charging authorized");
                break;
            case Webasto::ChargePointStateCharging:
                thing->setStateValue(liveWallboxChargePointStateStateTypeId, "Charging");
                break;
            case Webasto::ChargePointStateChargingPaused:
                thing->setStateValue(liveWallboxChargePointStateStateTypeId, "Charging paused");
                break;
            case Webasto::ChargePointStateChargeSuccessfulCarStillAttached:
                thing->setStateValue(liveWallboxChargePointStateStateTypeId, "Charge successful (car still attached)");
                break;
            case Webasto::ChargePointStateChargingStoppedByUserCarStillAttached:
                thing->setStateValue(liveWallboxChargePointStateStateTypeId, "Charging stopped by user (car still attached)");
                break;
            case Webasto::ChargePointStateChargingErrorCarStillAttached:
                thing->setStateValue(liveWallboxChargePointStateStateTypeId,  "Charging error (car still attached)");
                break;
            case Webasto::ChargePointStateChargingStationReservedNorCarAttached:
                thing->setStateValue(liveWallboxChargePointStateStateTypeId, "Charging station reserved (No car attached)");
                break;
            case Webasto::ChargePointStateUserNotAuthorizedCarAttached:
                thing->setStateValue(liveWallboxChargePointStateStateTypeId, "User not authorized (car attached)");
                break;
            }

            thing->setStateValue(liveWallboxChargingStateTypeId, Webasto::ChargePointState(data[0]) == Webasto::ChargePointStateCharging);
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
                thing->setStateValue(liveWallboxCableStateStateTypeId, "No cable attached");
                thing->setStateValue(liveWallboxPluggedInStateTypeId, false);
                break;
            case Webasto::CableStateCableAttachedNoCarAttached:
                thing->setStateValue(liveWallboxCableStateStateTypeId, "Cable attached but no car attached)");
                thing->setStateValue(liveWallboxPluggedInStateTypeId, false);
                break;
            case Webasto::CableStateCableAttachedCarAttached:
                thing->setStateValue(liveWallboxCableStateStateTypeId, "Cable attached and car attached");
                thing->setStateValue(liveWallboxPluggedInStateTypeId, true);
                break;
            case Webasto::CableStateCableAttachedCarAttachedLockActive:
                thing->setStateValue(liveWallboxCableStateStateTypeId, "Cable attached, car attached and lock active");
                thing->setStateValue(liveWallboxPluggedInStateTypeId, true);
                break;
            }
            break;
        case Webasto::TqEVSEError:
            qCDebug(dcWebasto()) << "   - EVSE error:" << data[0];
            thing->setStateValue(liveWallboxErrorStateTypeId, data[0]);
            break;
        case Webasto::TqCurrentL1:
            qCDebug(dcWebasto()) << "   - Current L1:" << data[0];
            thing->setStateValue(liveWallboxCurrentPhase1StateTypeId, data[0]);
            evaluatePhaseCount(thing);
            break;
        case Webasto::TqCurrentL2:
            qCDebug(dcWebasto()) << "   - Current L2:" << data[0];
            thing->setStateValue(liveWallboxCurrentPhase2StateTypeId, data[0]);
            evaluatePhaseCount(thing);
            break;
        case Webasto::TqCurrentL3:
            qCDebug(dcWebasto()) << "   - Current L3:" << data[0];
            thing->setStateValue(liveWallboxCurrentPhase3StateTypeId, data[0]);
            evaluatePhaseCount(thing);
            break;
        case Webasto::TqActivePower: {
            if (data.count() < 2)
                return;
            int power = (static_cast<quint32>(data[0])<<16 | data[1]);
            qCDebug(dcWebasto()) << "   - Active power:" << power;
            thing->setStateValue(liveWallboxCurrentPowerStateTypeId, power);
        } break;
        case Webasto::TqEnergyMeter: {
            if (data.count() < 2)
                return;
            int energy = (static_cast<quint32>(data[0])<<16 | data[1]);
            qCDebug(dcWebasto()) << "   - Energy meter:" << energy << "Wh";
            thing->setStateValue(liveWallboxTotalEnergyConsumedStateTypeId, energy);
        } break;
        case Webasto::TqMaxCurrent:
            qCDebug(dcWebasto()) << "   - Max. Current" << data[0];
            thing->setStateValue(liveWallboxMaxPossibleChargingCurrentStateTypeId, data[0]);
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
            if (data.count() < 2)
                return;
            uint batteryCapacity = (static_cast<quint32>(data[0])<<16 | data[1]);
            qCDebug(dcWebasto()) << "   - Battery capacity" << batteryCapacity << "Wh";
        } break;
        case Webasto::TqScheduleType:
            qCDebug(dcWebasto()) << "   - Schedule type" << data[0];
            break;
        case Webasto::TqRequiredEnergy: {
            if (data.count() < 2)
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
            thing->setStateValue(liveWallboxSessionEnergyStateTypeId, data[0]/1000.00); // Charged energy in kWh
            break;
        case Webasto::TqStartTime:
            qCDebug(dcWebasto()) << "   - Start time" << (static_cast<quint32>(data[0])<<16 | data[1]);
            break;
        case Webasto::TqChargingTime: {
            if (data.count() < 2)
                return;
            uint seconds = (static_cast<quint32>(data[0])<<16 | data[1]);
            qCDebug(dcWebasto()) << "   - Charging time" << seconds << "s";
            thing->setStateValue(liveWallboxSessionTimeStateTypeId, seconds/60.00); // Charging time in minutes
        } break;
        case Webasto::TqEndTime: {
            if (data.count() < 2)
                return;
            uint hour =    ((static_cast<quint32>(data[0])<<16 | data[1])&0xff0000)>>16;
            uint minutes = ((static_cast<quint32>(data[0])<<16 | data[1])&0x00ff00)>>8;
            uint seconds=  (static_cast<quint32>(data[0])<<16 | data[1])&0x0000ff;
            qCDebug(dcWebasto()) << "   - End time" << hour << "h" << minutes << "m" << seconds << "s";
        } break;
        case Webasto::TqUserId: {
            if (data.count() < 10)
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
