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

#include "integrationpluginenergymeters.h"
#include "plugininfo.h"

IntegrationPluginEnergyMeters::IntegrationPluginEnergyMeters()
{
    m_slaveIdParamTypeIds.insert(pro380ThingClassId, pro380ThingSlaveAddressParamTypeId);
    m_slaveIdParamTypeIds.insert(sdm630ThingClassId, sdm630ThingSlaveAddressParamTypeId);

    m_modbusUuidParamTypeIds.insert(pro380ThingClassId, pro380ThingModbusMasterUuidParamTypeId);
    m_modbusUuidParamTypeIds.insert(sdm630ThingClassId, sdm630ThingModbusMasterUuidParamTypeId);

    m_discoverySlaveAddressParamTypeIds.insert(pro380ThingClassId, pro380DiscoverySlaveAddressParamTypeId);
    m_discoverySlaveAddressParamTypeIds.insert(sdm630ThingClassId, sdm630DiscoverySlaveAddressParamTypeId);

    m_connectionStateTypeIds.insert(pro380ThingClassId, pro380ConnectedStateTypeId);
    m_connectionStateTypeIds.insert(sdm630ThingClassId, sdm630ConnectedStateTypeId);
}

void IntegrationPluginEnergyMeters::init()
{
    connect(hardwareManager()->modbusRtuResource(), &ModbusRtuHardwareResource::modbusRtuMasterRemoved, this, [=] (const QUuid &modbusUuid){
        qCDebug(dcEnergyMeters()) << "Modbus RTU master has been removed" << modbusUuid.toString();

        foreach (Thing *thing, myThings()) {
            if (m_modbusUuidParamTypeIds.contains(thing->thingClassId())) {
                if (thing->paramValue(m_modbusUuidParamTypeIds.value(thing->thingClassId())) == modbusUuid) {
                    qCWarning(dcEnergyMeters()) << "Modbus RTU hardware resource removed for" << thing << ". The thing will not be functional any more until a new resource has been configured for it.";
                    thing->setStateValue(m_connectionStateTypeIds[thing->thingClassId()], false);

                    if (thing->thingClassId() == sdm630ThingClassId) {
                        delete m_sdmConnections.take(thing);
                    } else if (thing->thingClassId() == pro380ThingClassId) {
                        delete m_ineproConnections.take(thing);
                    }
                }
            }
        }
    });
}

void IntegrationPluginEnergyMeters::discoverThings(ThingDiscoveryInfo *info)
{
    qCDebug(dcEnergyMeters()) << "Discover modbus RTU resources...";
    if (hardwareManager()->modbusRtuResource()->modbusRtuMasters().isEmpty()) {
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("No Modbus RTU interface available."));
        return;
    }

    Q_ASSERT_X(m_connectionStateTypeIds.contains(info->thingClassId()), "discoverThings", QString("Unhandled thingClassId: %1").arg(info->thingClassId().toString()).toUtf8());

    uint slaveAddress = info->params().paramValue(m_discoverySlaveAddressParamTypeIds.value(info->thingClassId())).toUInt();
    if (slaveAddress > 254 || slaveAddress == 0) {
        info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("Modbus slave address must be between 1 and 254"));
        return;
    }

    foreach (ModbusRtuMaster *modbusMaster, hardwareManager()->modbusRtuResource()->modbusRtuMasters()) {
        qCDebug(dcEnergyMeters()) << "Found RTU master resource" << modbusMaster << "connected" << modbusMaster->connected();
        if (!modbusMaster->connected())
            continue;

        ThingDescriptor descriptor(info->thingClassId(), QT_TR_NOOP("Energy meter"), QT_TR_NOOP("Slave address ") + QString::number(slaveAddress) + " " + modbusMaster->serialPort());
        ParamList params;
        params << Param(m_slaveIdParamTypeIds.value(info->thingClassId()), slaveAddress);
        params << Param(m_modbusUuidParamTypeIds.value(info->thingClassId()), modbusMaster->modbusUuid());
        descriptor.setParams(params);
        info->addThingDescriptor(descriptor);
    }

    info->finish(Thing::ThingErrorNoError);
    return;
}

void IntegrationPluginEnergyMeters::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcEnergyMeters()) << "Setup thing" << thing << thing->params();
    Q_ASSERT_X(m_connectionStateTypeIds.contains(thing->thingClassId()), "setupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());

    uint address = thing->paramValue(m_slaveIdParamTypeIds.value(thing->thingClassId())).toUInt();
    if (address > 254 || address == 0) {
        qCWarning(dcEnergyMeters()) << "Setup failed, slave address is not valid" << address;
        info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("Slave address not valid, must be between 1 and 254"));
        return;
    }

    QUuid uuid = thing->paramValue(m_modbusUuidParamTypeIds.value(thing->thingClassId())).toUuid();
    if (!hardwareManager()->modbusRtuResource()->hasModbusRtuMaster(uuid)) {
        qCWarning(dcEnergyMeters()) << "Setup failed, hardware manager not available";
        info->finish(Thing::ThingErrorSetupFailed, QT_TR_NOOP("Modbus RTU resource not available."));
        return;
    }

    if (thing->thingClassId() == sdm630ThingClassId) {
        if (m_sdmConnections.contains(thing)) {
            qCDebug(dcEnergyMeters()) << "Setup after rediscovery, cleaning up ...";
            m_sdmConnections.take(thing)->deleteLater();
        }

        Sdm630ModbusRtuConnection *sdmConnection = new Sdm630ModbusRtuConnection(hardwareManager()->modbusRtuResource()->getModbusRtuMaster(uuid), address, this);
        connect(sdmConnection->modbusRtuMaster(), &ModbusRtuMaster::connectedChanged, this, [=](bool connected){
            if (connected) {
                qCDebug(dcEnergyMeters()) << "Modbus RTU resource connected" << thing << sdmConnection->modbusRtuMaster()->serialPort();
            } else {
                qCWarning(dcEnergyMeters()) << "Modbus RTU resource disconnected" << thing << sdmConnection->modbusRtuMaster()->serialPort();
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

    } else if (thing->thingClassId() == pro380ThingClassId) {
        if (m_ineproConnections.contains(thing)) {
            qCDebug(dcEnergyMeters()) << "Setup after rediscovery, cleaning up ...";
            m_ineproConnections.take(thing)->deleteLater();
        }

        Pro380ModbusRtuConnection *proConnection = new Pro380ModbusRtuConnection(hardwareManager()->modbusRtuResource()->getModbusRtuMaster(uuid), address, this);
        connect(proConnection->modbusRtuMaster(), &ModbusRtuMaster::connectedChanged, this, [=](bool connected){
            if (connected) {
                qCDebug(dcEnergyMeters()) << "Modbus RTU resource connected" << thing << proConnection->modbusRtuMaster()->serialPort();
            } else {
                qCWarning(dcEnergyMeters()) << "Modbus RTU resource disconnected" << thing << proConnection->modbusRtuMaster()->serialPort();
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
        m_ineproConnections.insert(thing, proConnection);
        info->finish(Thing::ThingErrorNoError);
    }
}

void IntegrationPluginEnergyMeters::postSetupThing(Thing *thing)
{
    qCDebug(dcEnergyMeters) << "Post setup thing" << thing->name();
    if (!m_refreshTimer) {
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(2);
        connect(m_refreshTimer, &PluginTimer::timeout, this, [this] {
            foreach (Thing *thing, myThings().filterByThingClassId(sdm630ThingClassId)) {
                m_sdmConnections.value(thing)->update();
            }

            foreach (Thing *thing, myThings().filterByThingClassId(pro380ThingClassId)) {
                m_sdmConnections.value(thing)->update();
            }
        });

        qCDebug(dcEnergyMeters()) << "Starting refresh timer...";
        m_refreshTimer->start();
    }
}

void IntegrationPluginEnergyMeters::thingRemoved(Thing *thing)
{
    qCDebug(dcEnergyMeters()) << "Thing removed" << thing->name();

    if (m_sdmConnections.contains(thing))
        m_sdmConnections.take(thing)->deleteLater();

    if (m_ineproConnections.contains(thing))
        m_ineproConnections.take(thing)->deleteLater();

    if (myThings().isEmpty() && m_refreshTimer) {
        qCDebug(dcEnergyMeters()) << "Stopping reconnect timer";
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_refreshTimer);
        m_refreshTimer = nullptr;
    }
}
