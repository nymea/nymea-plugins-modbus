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

#include "integrationpluginschrack.h"
#include "plugininfo.h"

IntegrationPluginSchrack::IntegrationPluginSchrack()
{
}

void IntegrationPluginSchrack::init()
{

}

void IntegrationPluginSchrack::discoverThings(ThingDiscoveryInfo *info)
{
    qCDebug(dcSchrack()) << "Discovering modbus RTU resources...";
    if (hardwareManager()->modbusRtuResource()->modbusRtuMasters().isEmpty()) {
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("No Modbus RTU interface available. Please set up a Modbus RTU interface first."));
        return;
    }

    uint slaveAddress = info->params().paramValue(cionDiscoverySlaveAddressParamTypeId).toUInt();
    if (slaveAddress > 254 || slaveAddress == 0) {
        info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The Modbus slave address must be a value between 1 and 254."));
        return;
    }

    foreach (ModbusRtuMaster *modbusMaster, hardwareManager()->modbusRtuResource()->modbusRtuMasters()) {
        qCDebug(dcSchrack()) << "Found RTU master resource" << modbusMaster << "connected" << modbusMaster->connected();
        if (!modbusMaster->connected())
            continue;

        ThingDescriptor descriptor(info->thingClassId(), "i-CHARGE CION", QString::number(slaveAddress) + " " + modbusMaster->serialPort());
        ParamList params;
        params << Param(cionThingSlaveAddressParamTypeId, slaveAddress);
        params << Param(cionThingModbusMasterUuidParamTypeId, modbusMaster->modbusUuid());
        descriptor.setParams(params);
        info->addThingDescriptor(descriptor);
    }

    info->finish(Thing::ThingErrorNoError);
}

void IntegrationPluginSchrack::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcSchrack()) << "Setup thing" << thing << thing->params();

    uint address = thing->paramValue(cionThingSlaveAddressParamTypeId).toUInt();
    if (address > 254 || address == 0) {
        qCWarning(dcSchrack()) << "Setup failed, slave address is not valid" << address;
        info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("The Modbus address not valid. It must be a value between 1 and 254."));
        return;
    }

    QUuid uuid = thing->paramValue(cionThingModbusMasterUuidParamTypeId).toUuid();
    if (!hardwareManager()->modbusRtuResource()->hasModbusRtuMaster(uuid)) {
        qCWarning(dcSchrack()) << "Setup failed, hardware manager not available";
        info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("The Modbus RTU resource is not available."));
        return;
    }

    if (m_cionConnections.contains(thing)) {
        qCDebug(dcSchrack()) << "Already have a CION connection for this thing. Cleaning up old connection and initializing new one...";
        delete m_cionConnections.take(thing);
    }

    CionModbusRtuConnection *cionConnection = new CionModbusRtuConnection(hardwareManager()->modbusRtuResource()->getModbusRtuMaster(uuid), address, this);
    connect(cionConnection->modbusRtuMaster(), &ModbusRtuMaster::connectedChanged, thing, [=](bool connected){
        if (connected) {
            qCDebug(dcSchrack()) << "Modbus RTU resource connected" << thing << cionConnection->modbusRtuMaster()->serialPort();
        } else {
            qCWarning(dcSchrack()) << "Modbus RTU resource disconnected" << thing << cionConnection->modbusRtuMaster()->serialPort();
        }

        thing->setStateValue(cionConnectedStateTypeId, connected);
    });

    connect(cionConnection, &CionModbusRtuConnection::chargingEnabledChanged, thing, [=](quint16 charging){
        qCDebug(dcSchrack()) << "Charging enabled changed:" << charging;
        thing->setStateValue(cionPowerStateTypeId, charging == 1);
        thing->setStateValue(cionConnectedStateTypeId, true);
        finishAction(cionPowerStateTypeId);
    });

    // We can write chargingCurrentSetpoint to the preferred charging we want, and the wallbox will take it,
    // however, it may not necessarily *do* it, but will give us the actual value it uses in currentChargingCurrentE3
    // We'll use that for setting our state, just monitoring this one on the logs
    connect(cionConnection, &CionModbusRtuConnection::chargingCurrentSetpointChanged, thing, [=](quint16 chargingCurrentSetpoint){
        qCDebug(dcSchrack()) << "Charging current setpoint changed:" << chargingCurrentSetpoint;
//        thing->setStateValue(cionMaxChargingCurrentStateTypeId, chargingCurrentSetpoint);
//        thing->setStateValue(cionConnectedStateTypeId, true);
//        finishAction(cionMaxChargingCurrentStateTypeId);
    });

    connect(cionConnection, &CionModbusRtuConnection::cpSignalStateChanged, thing, [=](quint16 cpSignalState){
        qCDebug(dcSchrack()) << "CP Signal state changed:" << cpSignalState;
        thing->setStateValue(cionPluggedInStateTypeId, cpSignalState >= 66);
//        thing->setStateValue(cionMaxChargingCurrentStateTypeId, chargingCurrentSetpoint);
//        thing->setStateValue(cionConnectedStateTypeId, true);
//        finishAction(cionMaxChargingCurrentStateTypeId);
    });

    //
    connect(cionConnection, &CionModbusRtuConnection::currentChargingCurrentE3Changed, thing, [=](quint16 currentChargingCurrentE3){
        qCDebug(dcSchrack()) << "Current charging current E3 current changed:" << currentChargingCurrentE3;
        thing->setStateValue(cionMaxChargingCurrentStateTypeId, currentChargingCurrentE3);
        finishAction(cionMaxChargingCurrentStateTypeId);
    });

    // The maxChargingCurrentE3 takes into account the DIP switches and connected cable, so this is effectively
    // our maximum. However, it will go to 0 when unplugged, which is odd, so we'll ignore 0 values.
    connect(cionConnection, &CionModbusRtuConnection::maxChargingCurrentE3Changed, thing, [=](quint16 maxChargingCurrentE3){
        qCDebug(dcSchrack()) << "Maximum charging current E3 current changed:" << maxChargingCurrentE3;
        if (maxChargingCurrentE3 != 0) {
            thing->setStateMaxValue(cionMaxChargingCurrentStateTypeId, maxChargingCurrentE3);
        }
    });

    connect(cionConnection, &CionModbusRtuConnection::statusBitsChanged, thing, [=](quint16 statusBits){
        thing->setStateValue(cionConnectedStateTypeId, true);
        qCDebug(dcSchrack()) << "Status bits changed:" << statusBits;
    });

    connect(cionConnection, &CionModbusRtuConnection::minChargingCurrentChanged, thing, [=](quint16 minChargingCurrent){
        qCDebug(dcSchrack()) << "Minimum charging current changed:" << minChargingCurrent;
        thing->setStateMinValue(cionMaxChargingCurrentStateTypeId, minChargingCurrent);
    });

    connect(cionConnection, &CionModbusRtuConnection::gridVoltageChanged, thing, [=](float gridVoltage){
        qCDebug(dcSchrack()) << "Grid voltage changed:" << gridVoltage;
//        updateCurrentPower(thing);
    });

    connect(cionConnection, &CionModbusRtuConnection::u1VoltageChanged, thing, [=](float u1Voltage){
        qCDebug(dcSchrack()) << "U1 voltage changed:" << u1Voltage;
    });

    connect(cionConnection, &CionModbusRtuConnection::pluggedInDurationChanged, thing, [=](quint32 pluggedInDuration){
        qCDebug(dcSchrack()) << "Plugged in duration changed:" << pluggedInDuration;
        // Not reliable to determine if plugged in!
//        thing->setStateValue(cionPluggedInStateTypeId, pluggedInDuration > 0);
    });

    connect(cionConnection, &CionModbusRtuConnection::chargingDurationChanged, thing, [=](quint32 chargingDuration){
        qCDebug(dcSchrack()) << "Charging duration changed:" << chargingDuration;
        thing->setStateValue(cionChargingStateTypeId, chargingDuration > 0);
    });

    cionConnection->update();

    // Initialize min/max to their defaults. If both, nymea and the wallbox are restarted simultaneously, nymea would cache the min/max while
    // the wallbox would revert to its defaults, and being the default, the modbusconnection also never emits "changed" signals for them.
    // To prevent running out of sync we'll "uncache" min/max values too.
    thing->setStateMinMaxValues(cionMaxChargingCurrentStateTypeId, 6, 32);

    m_cionConnections.insert(thing, cionConnection);
    info->finish(Thing::ThingErrorNoError);
}

void IntegrationPluginSchrack::postSetupThing(Thing *thing)
{
    qCDebug(dcSchrack()) << "Post setup thing" << thing->name();
    if (!m_refreshTimer) {
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(2);
        connect(m_refreshTimer, &PluginTimer::timeout, this, [this] {
            foreach (Thing *thing, myThings()) {

                m_cionConnections.value(thing)->update();
                thing->setStateValue(cionChargingStateTypeId, m_cionConnections.value(thing)->chargingDuration() != m_lastChargingDuration);
                m_lastChargingDuration = m_cionConnections.value(thing)->chargingDuration();
            }
        });

        qCDebug(dcSchrack()) << "Starting refresh timer...";
        m_refreshTimer->start();
    }
}

void IntegrationPluginSchrack::thingRemoved(Thing *thing)
{
    qCDebug(dcSchrack()) << "Thing removed" << thing->name();

    if (m_cionConnections.contains(thing))
        m_cionConnections.take(thing)->deleteLater();

    if (myThings().isEmpty() && m_refreshTimer) {
        qCDebug(dcSchrack()) << "Stopping reconnect timer";
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_refreshTimer);
        m_refreshTimer = nullptr;
    }
}

void IntegrationPluginSchrack::executeAction(ThingActionInfo *info)
{
    CionModbusRtuConnection *cionConnection = m_cionConnections.value(info->thing());
    if (info->action().actionTypeId() == cionPowerActionTypeId) {
        qCDebug(dcSchrack()) << "Setting charging enabled:" << (info->action().paramValue(cionPowerActionPowerParamTypeId).toBool() ? 1 : 0);
        ModbusRtuReply *reply = cionConnection->setChargingEnabled(info->action().paramValue(cionPowerActionPowerParamTypeId).toBool() ? 1 : 0);
        waitForActionFinish(info, reply, cionPowerStateTypeId);
    } else if (info->action().actionTypeId() == cionMaxChargingCurrentActionTypeId) {
        ModbusRtuReply *reply = cionConnection->setChargingCurrentSetpoint(info->action().paramValue(cionMaxChargingCurrentActionMaxChargingCurrentParamTypeId).toUInt());
        waitForActionFinish(info, reply, cionMaxChargingCurrentStateTypeId);
    }


    Q_ASSERT_X(false, "IntegrationPluginSchrack::executeAction", QString("Unhandled action: %1").arg(info->action().actionTypeId().toString()).toLocal8Bit());
}

void IntegrationPluginSchrack::waitForActionFinish(ThingActionInfo *info, ModbusRtuReply *reply, const StateTypeId &stateTypeId)
{
    m_pendingActions.insert(info, stateTypeId);
    connect(info, &ThingActionInfo::destroyed, this, [=](){
        m_pendingActions.remove(info);
    });

    connect(reply, &ModbusRtuReply::finished, info, [=](){
        if (reply->error() != ModbusRtuReply::NoError) {
            m_pendingActions.remove(info);
            info->finish(Thing::ThingErrorHardwareFailure);
        }
    });
}

void IntegrationPluginSchrack::finishAction(const StateTypeId &stateTypeId)
{
    foreach (ThingActionInfo *info, m_pendingActions.keys(stateTypeId)) {
        info->finish(Thing::ThingErrorNoError);
    }
}

