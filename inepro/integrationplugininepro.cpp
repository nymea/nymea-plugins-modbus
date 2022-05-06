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

#include "integrationplugininepro.h"
#include "plugininfo.h"

IntegrationPluginInepro::IntegrationPluginInepro()
{
}

void IntegrationPluginInepro::init()
{
    connect(hardwareManager()->modbusRtuResource(), &ModbusRtuHardwareResource::modbusRtuMasterRemoved, this, [=] (const QUuid &modbusUuid){
        qCDebug(dcInepro()) << "Modbus RTU master has been removed" << modbusUuid.toString();

        foreach (Thing *thing, myThings()) {
            if (thing->paramValue(pro380ThingModbusMasterUuidParamTypeId) == modbusUuid) {
                qCWarning(dcInepro()) << "Modbus RTU hardware resource removed for" << thing << ". The thing will not be functional any more until a new resource has been configured for it.";
                thing->setStateValue(pro380ConnectedStateTypeId, false);

                delete m_connections.take(thing);
            }
        }
    });
}

void IntegrationPluginInepro::discoverThings(ThingDiscoveryInfo *info)
{
    qCDebug(dcInepro()) << "Discover modbus RTU resources...";
    if (hardwareManager()->modbusRtuResource()->modbusRtuMasters().isEmpty()) {
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("No Modbus RTU interface available. Please set up the Modbus RTU interface first."));
        return;
    }

    uint slaveAddress = info->params().paramValue(pro380DiscoverySlaveAddressParamTypeId).toUInt();
    if (slaveAddress > 254 || slaveAddress == 0) {
        info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The Modbus slave address must be a value between 1 and 254."));
        return;
    }

    foreach (ModbusRtuMaster *modbusMaster, hardwareManager()->modbusRtuResource()->modbusRtuMasters()) {
        qCDebug(dcInepro()) << "Found RTU master resource" << modbusMaster << "connected" << modbusMaster->connected();
        if (!modbusMaster->connected())
            continue;

        ThingDescriptor descriptor(info->thingClassId(), "PRO380", QString::number(slaveAddress) + " " + modbusMaster->serialPort());
        ParamList params;
        params << Param(pro380ThingSlaveAddressParamTypeId, slaveAddress);
        params << Param(pro380ThingModbusMasterUuidParamTypeId, modbusMaster->modbusUuid());
        descriptor.setParams(params);
        info->addThingDescriptor(descriptor);
    }

    info->finish(Thing::ThingErrorNoError);
    return;
}

void IntegrationPluginInepro::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcInepro()) << "Setup thing" << thing << thing->params();

    uint address = thing->paramValue(pro380ThingSlaveAddressParamTypeId).toUInt();
    if (address > 254 || address == 0) {
        qCWarning(dcInepro()) << "Setup failed, slave address is not valid" << address;
        info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("The Modbus address not valid. It must be a Value between 1 and 254."));
        return;
    }

    QUuid uuid = thing->paramValue(pro380ThingModbusMasterUuidParamTypeId).toUuid();
    if (!hardwareManager()->modbusRtuResource()->hasModbusRtuMaster(uuid)) {
        qCWarning(dcInepro()) << "Setup failed, hardware manager not available";
        info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("The Modbus RTU interface is not available."));
        return;
    }

    if (m_connections.contains(thing)) {
        qCDebug(dcInepro()) << "Setup after rediscovery, cleaning up ...";
        m_connections.take(thing)->deleteLater();
    }

    Pro380ModbusRtuConnection *proConnection = new Pro380ModbusRtuConnection(hardwareManager()->modbusRtuResource()->getModbusRtuMaster(uuid), address, this);
    connect(proConnection->modbusRtuMaster(), &ModbusRtuMaster::connectedChanged, this, [=](bool connected){
        if (connected) {
            qCDebug(dcInepro()) << "Modbus RTU resource connected" << thing << proConnection->modbusRtuMaster()->serialPort();
        } else {
            qCWarning(dcInepro()) << "Modbus RTU resource disconnected" << thing << proConnection->modbusRtuMaster()->serialPort();
        }
    });

    connect(proConnection, &Pro380ModbusRtuConnection::currentPhaseAChanged, this, [=](float currentPhaseA){
        thing->setStateValue(pro380CurrentPhaseAStateTypeId, currentPhaseA);
        thing->setStateValue(pro380ConnectedStateTypeId, true);
    });

    connect(proConnection, &Pro380ModbusRtuConnection::currentPhaseBChanged, this, [=](float currentPhaseB){
        thing->setStateValue(pro380CurrentPhaseBStateTypeId, currentPhaseB);
    });

    connect(proConnection, &Pro380ModbusRtuConnection::currentPhaseCChanged, this, [=](float currentPhaseC){
        thing->setStateValue(pro380CurrentPhaseCStateTypeId, currentPhaseC);
    });

    connect(proConnection, &Pro380ModbusRtuConnection::voltagePhaseAChanged, this, [=](float voltagePhaseA){
        thing->setStateValue(pro380VoltagePhaseAStateTypeId, voltagePhaseA);
    });

    connect(proConnection, &Pro380ModbusRtuConnection::voltagePhaseBChanged, this, [=](float voltagePhaseB){
        thing->setStateValue(pro380VoltagePhaseBStateTypeId, voltagePhaseB);
    });

    connect(proConnection, &Pro380ModbusRtuConnection::voltagePhaseCChanged, this, [=](float voltagePhaseC){
        thing->setStateValue(pro380VoltagePhaseCStateTypeId, voltagePhaseC);
    });

    connect(proConnection, &Pro380ModbusRtuConnection::totalCurrentPowerChanged, this, [=](float currentPower){
        thing->setStateValue(pro380CurrentPowerStateTypeId, currentPower * 1000); // kW
    });

    connect(proConnection, &Pro380ModbusRtuConnection::powerPhaseAChanged, this, [=](float powerPhaseA){
        thing->setStateValue(pro380CurrentPowerPhaseAStateTypeId, powerPhaseA * 1000); // kW
    });

    connect(proConnection, &Pro380ModbusRtuConnection::powerPhaseBChanged, this, [=](float powerPhaseB){
        thing->setStateValue(pro380CurrentPowerPhaseBStateTypeId, powerPhaseB * 1000); // kW
    });

    connect(proConnection, &Pro380ModbusRtuConnection::powerPhaseCChanged, this, [=](float powerPhaseC){
        thing->setStateValue(pro380CurrentPowerPhaseCStateTypeId, powerPhaseC * 1000); // kW
    });

    connect(proConnection, &Pro380ModbusRtuConnection::frequencyChanged, this, [=](float frequency){
        thing->setStateValue(pro380FrequencyStateTypeId, frequency);
    });

    connect(proConnection, &Pro380ModbusRtuConnection::totalEnergyConsumedChanged, this, [=](float totalEnergyConsumed){
        thing->setStateValue(pro380TotalEnergyConsumedStateTypeId, totalEnergyConsumed);
    });

    connect(proConnection, &Pro380ModbusRtuConnection::totalEnergyProducedChanged, this, [=](float totalEnergyProduced){
        thing->setStateValue(pro380TotalEnergyProducedStateTypeId, totalEnergyProduced);
    });

    connect(proConnection, &Pro380ModbusRtuConnection::energyProducedPhaseAChanged, this, [=](float energyProducedPhaseA){
        thing->setStateValue(pro380EnergyProducedPhaseAStateTypeId, energyProducedPhaseA);
    });

    connect(proConnection, &Pro380ModbusRtuConnection::energyProducedPhaseBChanged, this, [=](float energyProducedPhaseB){
        thing->setStateValue(pro380EnergyProducedPhaseBStateTypeId, energyProducedPhaseB);
    });

    connect(proConnection, &Pro380ModbusRtuConnection::energyProducedPhaseCChanged, this, [=](float energyProducedPhaseC){
        thing->setStateValue(pro380EnergyProducedPhaseCStateTypeId, energyProducedPhaseC);
    });

    connect(proConnection, &Pro380ModbusRtuConnection::energyConsumedPhaseAChanged, this, [=](float energyConsumedPhaseA){
        thing->setStateValue(pro380EnergyConsumedPhaseAStateTypeId, energyConsumedPhaseA);
    });

    connect(proConnection, &Pro380ModbusRtuConnection::energyConsumedPhaseBChanged, this, [=](float energyConsumedPhaseB){
        thing->setStateValue(pro380EnergyConsumedPhaseBStateTypeId, energyConsumedPhaseB);
    });

    connect(proConnection, &Pro380ModbusRtuConnection::energyConsumedPhaseCChanged, this, [=](float energyConsumedPhaseC){
        thing->setStateValue(pro380EnergyConsumedPhaseCStateTypeId, energyConsumedPhaseC);
    });


    // FIXME: try to read before setup success
    m_connections.insert(thing, proConnection);
    info->finish(Thing::ThingErrorNoError);
}

void IntegrationPluginInepro::postSetupThing(Thing *thing)
{
    qCDebug(dcInepro) << "Post setup thing" << thing->name();
    if (!m_refreshTimer) {
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(2);
        connect(m_refreshTimer, &PluginTimer::timeout, this, [this] {
            foreach (Thing *thing, myThings()) {
                m_connections.value(thing)->update();
            }
        });

        qCDebug(dcInepro()) << "Starting refresh timer...";
        m_refreshTimer->start();
    }
}

void IntegrationPluginInepro::thingRemoved(Thing *thing)
{
    qCDebug(dcInepro()) << "Thing removed" << thing->name();

    if (m_connections.contains(thing))
        m_connections.take(thing)->deleteLater();

    if (myThings().isEmpty() && m_refreshTimer) {
        qCDebug(dcInepro()) << "Stopping reconnect timer";
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_refreshTimer);
        m_refreshTimer = nullptr;
    }
}
