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

#include "integrationpluginschneiderIEM.h"
#include "plugininfo.h"

IntegrationPluginSchneiderIEM::IntegrationPluginSchneiderIEM()
{
}

void IntegrationPluginSchneiderIEM::init()
{
    connect(hardwareManager()->modbusRtuResource(), &ModbusRtuHardwareResource::modbusRtuMasterRemoved, this, [=] (const QUuid &modbusUuid){
        qCDebug(dcSchneiderIEM()) << "Modbus RTU master has been removed" << modbusUuid.toString();

        foreach (Thing *thing, myThings()) {
            if (thing->paramValue(iemThingModbusMasterUuidParamTypeId) == modbusUuid) {
                qCWarning(dcSchneiderIEM()) << "Modbus RTU hardware resource removed for" << thing << ". The thing will not be functional any more until a new resource has been configured for it.";
                thing->setStateValue(iemConnectedStateTypeId, false);
                delete m_schneiderIEMConnections.take(thing);
            }
        }
    });
}

void IntegrationPluginSchneiderIEM::discoverThings(ThingDiscoveryInfo *info)
{
    qCDebug(dcSchneiderIEM()) << "Discover modbus RTU resources...";
    if (hardwareManager()->modbusRtuResource()->modbusRtuMasters().isEmpty()) {
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("No Modbus RTU interface available. Please set up the Modbus RTU interface first."));
        return;
    }

    uint slaveAddress = info->params().paramValue(iemDiscoverySlaveAddressParamTypeId).toUInt();
    if (slaveAddress > 254 || slaveAddress == 0) {
        info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The Modbus slave address must be a value between 1 and 254."));
        return;
    }

    foreach (ModbusRtuMaster *modbusMaster, hardwareManager()->modbusRtuResource()->modbusRtuMasters()) {
        qCDebug(dcSchneiderIEM()) << "Found RTU master resource" << modbusMaster << "connected" << modbusMaster->connected();
        if (!modbusMaster->connected())
            continue;

        ThingDescriptor descriptor(info->thingClassId(), "IEM", QString::number(slaveAddress) + " " + modbusMaster->serialPort());
        ParamList params;
        params << Param(iemThingSlaveAddressParamTypeId, slaveAddress);
        params << Param(iemThingModbusMasterUuidParamTypeId, modbusMaster->modbusUuid());
        descriptor.setParams(params);
        info->addThingDescriptor(descriptor);
    }

    info->finish(Thing::ThingErrorNoError);
    return;
}

void IntegrationPluginSchneiderIEM::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcSchneiderIEM()) << "Setup thing" << thing << thing->params();

    uint address = thing->paramValue(iemThingSlaveAddressParamTypeId).toUInt();
    if (address > 254 || address == 0) {
        qCWarning(dcSchneiderIEM()) << "Setup failed, slave address is not valid" << address;
        info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("The Modbus address not valid. It must be a value between 1 and 254."));
        return;
    }

    QUuid uuid = thing->paramValue(iemThingModbusMasterUuidParamTypeId).toUuid();
    if (!hardwareManager()->modbusRtuResource()->hasModbusRtuMaster(uuid)) {
        qCWarning(dcSchneiderIEM()) << "Setup failed, hardware manager not available";
        info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("The Modbus RTU interface not available."));
        return;
    }

    if (m_schneiderIEMConnections.contains(thing)) {
        qCDebug(dcSchneiderIEM()) << "Setup after rediscovery, cleaning up ...";
        m_schneiderIEMConnections.take(thing)->deleteLater();
    }

    SchneiderIEMModbusRtuConnection *iemConnection = new SchneiderIEMModbusRtuConnection(hardwareManager()->modbusRtuResource()->getModbusRtuMaster(uuid), address, this);
    connect(iemConnection->modbusRtuMaster(), &ModbusRtuMaster::connectedChanged, this, [=](bool connected){
        if (connected) {
            qCDebug(dcSchneiderIEM()) << "Modbus RTU resource connected" << thing << iemConnection->modbusRtuMaster()->serialPort();
        } else {
            qCWarning(dcSchneiderIEM()) << "Modbus RTU resource disconnected" << thing << iemConnection->modbusRtuMaster()->serialPort();
        }
    });

    connect(iemConnection, &SchneiderIEMModbusRtuConnection::currentPhaseAChanged, this, [=](float currentPhaseA){
        thing->setStateValue(iemCurrentPhaseAStateTypeId, currentPhaseA);
        thing->setStateValue(iemConnectedStateTypeId, true);
    });

    connect(iemConnection, &SchneiderIEMModbusRtuConnection::currentPhaseBChanged, this, [=](float currentPhaseB){
        thing->setStateValue(iemCurrentPhaseBStateTypeId, currentPhaseB);
    });

    connect(iemConnection, &SchneiderIEMModbusRtuConnection::currentPhaseCChanged, this, [=](float currentPhaseC){
        thing->setStateValue(iemCurrentPhaseCStateTypeId, currentPhaseC);
    });

    connect(iemConnection, &SchneiderIEMModbusRtuConnection::voltagePhaseAChanged, this, [=](float voltagePhaseA){
        thing->setStateValue(iemVoltagePhaseAStateTypeId, voltagePhaseA);
    });

    connect(iemConnection, &SchneiderIEMModbusRtuConnection::voltagePhaseBChanged, this, [=](float voltagePhaseB){
        thing->setStateValue(iemVoltagePhaseBStateTypeId, voltagePhaseB);
    });

    connect(iemConnection, &SchneiderIEMModbusRtuConnection::voltagePhaseCChanged, this, [=](float voltagePhaseC){
        thing->setStateValue(iemVoltagePhaseCStateTypeId, voltagePhaseC);
    });

    connect(iemConnection, &SchneiderIEMModbusRtuConnection::totalCurrentPowerChanged, this, [=](float currentPower){
        thing->setStateValue(iemCurrentPowerStateTypeId, currentPower);
    });

    connect(iemConnection, &SchneiderIEMModbusRtuConnection::powerPhaseAChanged, this, [=](float powerPhaseA){
        thing->setStateValue(iemCurrentPowerPhaseAStateTypeId, powerPhaseA);
    });

    connect(iemConnection, &SchneiderIEMModbusRtuConnection::powerPhaseBChanged, this, [=](float powerPhaseB){
        thing->setStateValue(iemCurrentPowerPhaseBStateTypeId, powerPhaseB);
    });

    connect(iemConnection, &SchneiderIEMModbusRtuConnection::powerPhaseCChanged, this, [=](float powerPhaseC){
        thing->setStateValue(iemCurrentPowerPhaseCStateTypeId, powerPhaseC);
    });

    connect(iemConnection, &SchneiderIEMModbusRtuConnection::frequencyChanged, this, [=](float frequency){
        thing->setStateValue(iemFrequencyStateTypeId, frequency);
    });

    connect(iemConnection, &SchneiderIEMModbusRtuConnection::totalEnergyConsumedChanged, this, [=](float totalEnergyConsumed){
        thing->setStateValue(iemTotalEnergyConsumedStateTypeId, totalEnergyConsumed);
    });

    connect(iemConnection, &SchneiderIEMModbusRtuConnection::totalEnergyProducedChanged, this, [=](float totalEnergyProduced){
        thing->setStateValue(iemTotalEnergyProducedStateTypeId, totalEnergyProduced);
    });

    connect(iemConnection, &SchneiderIEMModbusRtuConnection::energyConsumedPhaseAChanged, this, [=](float energyConsumedPhaseA){
        thing->setStateValue(iemEnergyConsumedPhaseAStateTypeId, energyConsumedPhaseA);
    });

    connect(iemConnection, &SchneiderIEMModbusRtuConnection::energyConsumedPhaseBChanged, this, [=](float energyConsumedPhaseB){
        thing->setStateValue(iemEnergyConsumedPhaseBStateTypeId, energyConsumedPhaseB);
    });

    connect(iemConnection, &SchneiderIEMModbusRtuConnection::energyConsumedPhaseCChanged, this, [=](float energyConsumedPhaseC){
        thing->setStateValue(iemEnergyConsumedPhaseCStateTypeId, energyConsumedPhaseC);
    });

    // FIXME: try to read before setup success
    m_schneiderIEMConnections.insert(thing, iemConnection);
    info->finish(Thing::ThingErrorNoError);
}

void IntegrationPluginSchneiderIEM::postSetupThing(Thing *thing)
{
    qCDebug(dcSchneiderIEM()) << "Post setup thing" << thing->name();
    if (!m_refreshTimer) {
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(2);
        connect(m_refreshTimer, &PluginTimer::timeout, this, [this] {
            foreach (Thing *thing, myThings()) {
                m_schneiderIEMConnections.value(thing)->update();
            }
        });

        qCDebug(dcSchneiderIEM()) << "Starting refresh timer...";
        m_refreshTimer->start();
    }
}

void IntegrationPluginSchneiderIEM::thingRemoved(Thing *thing)
{
    qCDebug(dcSchneiderIEM()) << "Thing removed" << thing->name();

    if (m_schneiderIEMConnections.contains(thing))
        m_schneiderIEMConnections.take(thing)->deleteLater();

    if (myThings().isEmpty() && m_refreshTimer) {
        qCDebug(dcSchneiderIEM()) << "Stopping reconnect timer";
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_refreshTimer);
        m_refreshTimer = nullptr;
    }
}
