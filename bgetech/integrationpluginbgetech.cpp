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

#include "integrationpluginbgetech.h"
#include "plugininfo.h"

IntegrationPluginBGETech::IntegrationPluginBGETech()
{
}

void IntegrationPluginBGETech::init()
{
    connect(hardwareManager()->modbusRtuResource(), &ModbusRtuHardwareResource::modbusRtuMasterRemoved, this, [=] (const QUuid &modbusUuid){
        qCDebug(dcBgeTech()) << "Modbus RTU master has been removed" << modbusUuid.toString();

        foreach (Thing *thing, myThings()) {
            if (thing->paramValue(sdm630ThingModbusMasterUuidParamTypeId) == modbusUuid) {
                qCWarning(dcBgeTech()) << "Modbus RTU hardware resource removed for" << thing << ". The thing will not be functional any more until a new resource has been configured for it.";
                thing->setStateValue(sdm630ConnectedStateTypeId, false);
                delete m_sdmConnections.take(thing);
            }
        }
    });
}

void IntegrationPluginBGETech::discoverThings(ThingDiscoveryInfo *info)
{
    qCDebug(dcBgeTech()) << "Discover modbus RTU resources...";
    if (hardwareManager()->modbusRtuResource()->modbusRtuMasters().isEmpty()) {
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("No Modbus RTU interface available. Please set up the Modbus RTU interface first."));
        return;
    }

    uint slaveAddress = info->params().paramValue(sdm630DiscoverySlaveAddressParamTypeId).toUInt();
    if (slaveAddress > 254 || slaveAddress == 0) {
        info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The Modbus slave address must be a value between 1 and 254."));
        return;
    }

    foreach (ModbusRtuMaster *modbusMaster, hardwareManager()->modbusRtuResource()->modbusRtuMasters()) {
        qCDebug(dcBgeTech()) << "Found RTU master resource" << modbusMaster << "connected" << modbusMaster->connected();
        if (!modbusMaster->connected())
            continue;

        ThingDescriptor descriptor(info->thingClassId(), "SDM630", QString::number(slaveAddress) + " " + modbusMaster->serialPort());
        ParamList params;
        params << Param(sdm630ThingSlaveAddressParamTypeId, slaveAddress);
        params << Param(sdm630ThingModbusMasterUuidParamTypeId, modbusMaster->modbusUuid());
        descriptor.setParams(params);
        info->addThingDescriptor(descriptor);
    }

    info->finish(Thing::ThingErrorNoError);
    return;
}

void IntegrationPluginBGETech::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcBgeTech()) << "Setup thing" << thing << thing->params();

    uint address = thing->paramValue(sdm630ThingSlaveAddressParamTypeId).toUInt();
    if (address > 254 || address == 0) {
        qCWarning(dcBgeTech()) << "Setup failed, slave address is not valid" << address;
        info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("The Modbus address not valid. It must be a value between 1 and 254."));
        return;
    }

    QUuid uuid = thing->paramValue(sdm630ThingModbusMasterUuidParamTypeId).toUuid();
    if (!hardwareManager()->modbusRtuResource()->hasModbusRtuMaster(uuid)) {
        qCWarning(dcBgeTech()) << "Setup failed, hardware manager not available";
        info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("The Modbus RTU interface not available."));
        return;
    }

    if (m_sdmConnections.contains(thing)) {
        qCDebug(dcBgeTech()) << "Setup after rediscovery, cleaning up ...";
        m_sdmConnections.take(thing)->deleteLater();
    }

    Sdm630ModbusRtuConnection *sdmConnection = new Sdm630ModbusRtuConnection(hardwareManager()->modbusRtuResource()->getModbusRtuMaster(uuid), address, this);
    connect(sdmConnection->modbusRtuMaster(), &ModbusRtuMaster::connectedChanged, this, [=](bool connected){
        if (connected) {
            qCDebug(dcBgeTech()) << "Modbus RTU resource connected" << thing << sdmConnection->modbusRtuMaster()->serialPort();
        } else {
            qCWarning(dcBgeTech()) << "Modbus RTU resource disconnected" << thing << sdmConnection->modbusRtuMaster()->serialPort();
        }
    });

    connect(sdmConnection, &Sdm630ModbusRtuConnection::currentPhaseAChanged, this, [=](float currentPhaseA){
        thing->setStateValue(sdm630CurrentPhaseAStateTypeId, currentPhaseA);
        thing->setStateValue(sdm630ConnectedStateTypeId, true);
    });

    connect(sdmConnection, &Sdm630ModbusRtuConnection::currentPhaseBChanged, this, [=](float currentPhaseB){
        thing->setStateValue(sdm630CurrentPhaseBStateTypeId, currentPhaseB);
    });

    connect(sdmConnection, &Sdm630ModbusRtuConnection::currentPhaseCChanged, this, [=](float currentPhaseC){
        thing->setStateValue(sdm630CurrentPhaseCStateTypeId, currentPhaseC);
    });

    connect(sdmConnection, &Sdm630ModbusRtuConnection::voltagePhaseAChanged, this, [=](float voltagePhaseA){
        thing->setStateValue(sdm630VoltagePhaseAStateTypeId, voltagePhaseA);
    });

    connect(sdmConnection, &Sdm630ModbusRtuConnection::voltagePhaseBChanged, this, [=](float voltagePhaseB){
        thing->setStateValue(sdm630VoltagePhaseBStateTypeId, voltagePhaseB);
    });

    connect(sdmConnection, &Sdm630ModbusRtuConnection::voltagePhaseCChanged, this, [=](float voltagePhaseC){
        thing->setStateValue(sdm630VoltagePhaseCStateTypeId, voltagePhaseC);
    });

    connect(sdmConnection, &Sdm630ModbusRtuConnection::totalCurrentPowerChanged, this, [=](float currentPower){
        thing->setStateValue(sdm630CurrentPowerStateTypeId, currentPower);
    });

    connect(sdmConnection, &Sdm630ModbusRtuConnection::powerPhaseAChanged, this, [=](float powerPhaseA){
        thing->setStateValue(sdm630CurrentPowerPhaseAStateTypeId, powerPhaseA);
    });

    connect(sdmConnection, &Sdm630ModbusRtuConnection::powerPhaseBChanged, this, [=](float powerPhaseB){
        thing->setStateValue(sdm630CurrentPowerPhaseBStateTypeId, powerPhaseB);
    });

    connect(sdmConnection, &Sdm630ModbusRtuConnection::powerPhaseCChanged, this, [=](float powerPhaseC){
        thing->setStateValue(sdm630CurrentPowerPhaseCStateTypeId, powerPhaseC);
    });

    connect(sdmConnection, &Sdm630ModbusRtuConnection::frequencyChanged, this, [=](float frequency){
        thing->setStateValue(sdm630FrequencyStateTypeId, frequency);
    });

    connect(sdmConnection, &Sdm630ModbusRtuConnection::totalEnergyConsumedChanged, this, [=](float totalEnergyConsumed){
        thing->setStateValue(sdm630TotalEnergyConsumedStateTypeId, totalEnergyConsumed);
    });

    connect(sdmConnection, &Sdm630ModbusRtuConnection::totalEnergyProducedChanged, this, [=](float totalEnergyProduced){
        thing->setStateValue(sdm630TotalEnergyProducedStateTypeId, totalEnergyProduced);
    });

    connect(sdmConnection, &Sdm630ModbusRtuConnection::energyProducedPhaseAChanged, this, [=](float energyProducedPhaseA){
        thing->setStateValue(sdm630EnergyProducedPhaseAStateTypeId, energyProducedPhaseA);
    });

    connect(sdmConnection, &Sdm630ModbusRtuConnection::energyProducedPhaseBChanged, this, [=](float energyProducedPhaseB){
        thing->setStateValue(sdm630EnergyProducedPhaseBStateTypeId, energyProducedPhaseB);
    });

    connect(sdmConnection, &Sdm630ModbusRtuConnection::energyProducedPhaseCChanged, this, [=](float energyProducedPhaseC){
        thing->setStateValue(sdm630EnergyProducedPhaseCStateTypeId, energyProducedPhaseC);
    });

    connect(sdmConnection, &Sdm630ModbusRtuConnection::energyConsumedPhaseAChanged, this, [=](float energyConsumedPhaseA){
        thing->setStateValue(sdm630EnergyConsumedPhaseAStateTypeId, energyConsumedPhaseA);
    });

    connect(sdmConnection, &Sdm630ModbusRtuConnection::energyConsumedPhaseBChanged, this, [=](float energyConsumedPhaseB){
        thing->setStateValue(sdm630EnergyConsumedPhaseBStateTypeId, energyConsumedPhaseB);
    });

    connect(sdmConnection, &Sdm630ModbusRtuConnection::energyConsumedPhaseCChanged, this, [=](float energyConsumedPhaseC){
        thing->setStateValue(sdm630EnergyConsumedPhaseCStateTypeId, energyConsumedPhaseC);
    });

    // FIXME: try to read before setup success
    m_sdmConnections.insert(thing, sdmConnection);
    info->finish(Thing::ThingErrorNoError);
}

void IntegrationPluginBGETech::postSetupThing(Thing *thing)
{
    qCDebug(dcBgeTech) << "Post setup thing" << thing->name();
    if (!m_refreshTimer) {
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(2);
        connect(m_refreshTimer, &PluginTimer::timeout, this, [this] {
            foreach (Thing *thing, myThings()) {
                m_sdmConnections.value(thing)->update();
            }
        });

        qCDebug(dcBgeTech()) << "Starting refresh timer...";
        m_refreshTimer->start();
    }
}

void IntegrationPluginBGETech::thingRemoved(Thing *thing)
{
    qCDebug(dcBgeTech()) << "Thing removed" << thing->name();

    if (m_sdmConnections.contains(thing))
        m_sdmConnections.take(thing)->deleteLater();

    if (myThings().isEmpty() && m_refreshTimer) {
        qCDebug(dcBgeTech()) << "Stopping reconnect timer";
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_refreshTimer);
        m_refreshTimer = nullptr;
    }
}
