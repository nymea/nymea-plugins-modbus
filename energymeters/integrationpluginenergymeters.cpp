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

#include "integrationpluginenergymeters.h"
#include "plugininfo.h"

#include "bg-etechmodbusregister.h"
#include "inepromodbusregister.h"

IntegrationPluginEnergyMeters::IntegrationPluginEnergyMeters()
{
    /*
    * NOTE:
    * To add an new device model, the integrationplugin json file must be extended with the new model and vendor.
    * Then add the new states and params to the lists here, also add the modbus register configuration file
*/
    m_slaveIdParamTypeIds.insert(pro380ThingClassId, pro380ThingSlaveAddressParamTypeId);
    m_slaveIdParamTypeIds.insert(sdm630ThingClassId, sdm630ThingSlaveAddressParamTypeId);

    m_modbusUuidParamTypeIds.insert(pro380ThingClassId, pro380ThingModbusMasterUuidParamTypeId);
    m_modbusUuidParamTypeIds.insert(sdm630ThingClassId, sdm630ThingModbusMasterUuidParamTypeId);

    m_connectionStateTypeIds.insert(pro380ThingClassId, pro380ConnectedStateTypeId);
    m_connectionStateTypeIds.insert(sdm630ThingClassId, sdm630ConnectedStateTypeId);

    m_voltageStateTypeIds.insert(pro380ThingClassId, pro380VoltageStateTypeId);
    m_voltageStateTypeIds.insert(sdm630ThingClassId, sdm630VoltageStateTypeId);

    m_currentStateTypeIds.insert(pro380ThingClassId, pro380CurrentStateTypeId);
    m_currentStateTypeIds.insert(sdm630ThingClassId, sdm630CurrentStateTypeId);

    m_activePowerStateTypeIds.insert(pro380ThingClassId, pro380CurrentPowerEventTypeId);
    m_activePowerStateTypeIds.insert(sdm630ThingClassId, sdm630CurrentPowerStateTypeId);

    m_frequencyStateTypeIds.insert(pro380ThingClassId, pro380FrequencyStateTypeId);
    m_frequencyStateTypeIds.insert(sdm630ThingClassId, sdm630FrequencyStateTypeId);

    m_powerFactorStateTypeIds.insert(pro380ThingClassId, pro380PowerFactorStateTypeId);
    m_powerFactorStateTypeIds.insert(sdm630ThingClassId, sdm630PowerFactorStateTypeId);

    m_discoverySlaveAddressParamTypeIds.insert(pro380ThingClassId, pro380DiscoverySlaveAddressParamTypeId);
    m_discoverySlaveAddressParamTypeIds.insert(sdm630ThingClassId, sdm630DiscoverySlaveAddressParamTypeId);

    m_registerMaps.insert(pro380ThingClassId, &pro380RegisterMap);
    m_registerMaps.insert(sdm630ThingClassId, &sdm630RegisterMap);

    // Modbus RTU hardware resource
    connect(hardwareManager()->modbusRtuResource(), &ModbusRtuHardwareResource::modbusRtuMasterRemoved, this, [=](const QUuid &modbusUuid){
        qCDebug(dcEnergyMeters()) << "Modbus RTU master has been removed" << modbusUuid.toString();

        // Check if there is any device using this resource
        foreach (Thing *thing, m_modbusRtuMasters.keys()) {
            if (m_modbusRtuMasters.value(thing)->modbusUuid() == modbusUuid) {
                qCWarning(dcEnergyMeters()) << "Hardware resource removed for" << thing << ". The thing will not be functional any more until a new resource has been configured for it.";
                m_modbusRtuMasters.remove(thing);
                thing->setStateValue(m_connectionStateTypeIds[thing->thingClassId()], false);

                // Set all child things disconnected
                foreach (Thing *childThing, myThings()) {
                    if (childThing->parentId() == thing->id()) {
                        thing->setStateValue(m_connectionStateTypeIds[childThing->thingClassId()], false);
                    }
                }
            }
        }
    });
}

void IntegrationPluginEnergyMeters::discoverThings(ThingDiscoveryInfo *info)
{
    qCDebug(dcEnergyMeters()) << "Discover things";
    QList<ThingDescriptor> thingDescriptors;

    if (hardwareManager()->modbusRtuResource()->modbusRtuMasters().isEmpty()) {
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("No Modbus RTU interface available."));
        return;
    }

    if (m_connectionStateTypeIds.contains(info->thingClassId())) {
        int slaveAddress = info->params().paramValue(m_discoverySlaveAddressParamTypeIds.value(info->thingClassId())).toInt();
        if (slaveAddress > 254 || slaveAddress == 0) {
            info->finish(Thing::ThingErrorInvalidParameter, tr("Modbus slave address must be between 1 and 254"));
            return;
        }
        Q_FOREACH(ModbusRtuMaster *modbusMaster, hardwareManager()->modbusRtuResource()->modbusRtuMasters()) {
            qCDebug(dcEnergyMeters()) << "Found RTU master resource" << modbusMaster << "connected" << modbusMaster->connected();
            if (!modbusMaster->connected()) {
                continue;
            }
            ThingDescriptor descriptor(info->thingClassId(), "Modbus interface "+modbusMaster->serialPort(), modbusMaster->modbusUuid().toString());
            ParamList params;
            params << Param(m_slaveIdParamTypeIds.value(info->thingClassId()), slaveAddress);
            params << Param(m_modbusUuidParamTypeIds.value(info->thingClassId()), modbusMaster->modbusUuid());
            descriptor.setParams(params);
            info->addThingDescriptor(descriptor);
        }
        info->finish(Thing::ThingErrorNoError);
        return;
    } else {
        Q_ASSERT_X(false, "discoverThings", QString("Unhandled thingClassId: %1").arg(info->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginEnergyMeters::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcEnergyMeters()) << "Setup thing" << thing->name();

    if (m_connectionStateTypeIds.contains(thing->thingClassId())) {

        if (m_energyMeters.contains(thing)) {
            qCDebug(dcEnergyMeters()) << "Setup after rediscovery, cleaning up ...";
            m_energyMeters.take(thing)->deleteLater();
        }
        int address = thing->paramValue(m_slaveIdParamTypeIds.value(thing->thingClassId())).toInt();
        if (address > 254 || address == 0) {
            qCWarning(dcEnergyMeters()) << "Setup failed, slave address is not valid" << address;
            info->finish(Thing::ThingErrorSetupFailed, tr("Slave address not valid, must be between 1 and 254"));
            return;
        }
        QUuid uuid = thing->paramValue(m_modbusUuidParamTypeIds.value(thing->thingClassId())).toString();
        if (!hardwareManager()->modbusRtuResource()->hasModbusRtuMaster(uuid)) {
            qCWarning(dcEnergyMeters()) << "Setup failed, hardware manager not available";
            info->finish(Thing::ThingErrorSetupFailed, tr("Modbus RTU resource not available."));
            return;
        }

        EnergyMeter *meter = new EnergyMeter(hardwareManager()->modbusRtuResource()->getModbusRtuMaster(uuid), address, *m_registerMaps.value(thing->thingClassId()), this);
        connect(info, &ThingSetupInfo::aborted, meter, &EnergyMeter::deleteLater);
        connect(meter, &EnergyMeter::consumedEnergyReceived, info, [this, info, meter] {
            qCDebug(dcEnergyMeters()) << "Reply received, setup finished";
            connect(meter, &EnergyMeter::connectedChanged, this, &IntegrationPluginEnergyMeters::onConnectionStateChanged);
            connect(meter, &EnergyMeter::voltageReceived, this, &IntegrationPluginEnergyMeters::onVoltageReceived);
            connect(meter, &EnergyMeter::currentReceived, this, &IntegrationPluginEnergyMeters::onCurrentReceived);
            connect(meter, &EnergyMeter::activePowerReceived, this, &IntegrationPluginEnergyMeters::onActivePowerReceived);
            connect(meter, &EnergyMeter::powerFactorReceived, this, &IntegrationPluginEnergyMeters::onPowerFactorReceived);
            connect(meter, &EnergyMeter::frequencyReceived, this, &IntegrationPluginEnergyMeters::onActivePowerReceived);
            connect(meter, &EnergyMeter::producedEnergyReceived, this, &IntegrationPluginEnergyMeters::onProducedEnergyReceived);
            connect(meter, &EnergyMeter::consumedEnergyReceived, this, &IntegrationPluginEnergyMeters::onConsumedEnergyReceived);

            m_energyMeters.insert(info->thing(), meter);
            info->finish(Thing::ThingErrorNoError);
        });
        return;
    } else {
        Q_ASSERT_X(false, "setupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginEnergyMeters::postSetupThing(Thing *thing)
{
    qCDebug(dcEnergyMeters) << "Post setup thing" << thing->name();
    if (m_connectionStateTypeIds.contains(thing->thingClassId())) {
        thing->setStateValue(m_connectionStateTypeIds.value(thing->thingClassId()), true);
    }

    if (!m_updateTimer) {
        qCDebug(dcEnergyMeters()) << "Creating update timer";
        m_updateTimer = new QTimer(this);
        m_updateTimer->start(configValue(energyMetersPluginUpdateIntervalParamTypeId).toInt());
        connect(m_updateTimer, &QTimer::timeout, this, [this] {
            foreach (EnergyMeter *meter, m_energyMeters) {
                meter->getVoltage();
            }
        });
        connect(this, &IntegrationPlugin::configValueChanged, [this] (const ParamTypeId &paramTypeId, const QVariant value) {
            if (m_updateTimer && (paramTypeId == energyMetersPluginUpdateIntervalParamTypeId)) {
                qCDebug(dcEnergyMeters()) << "Updating update interval to" << value;
                m_updateTimer->setInterval(value.toInt());
            }
        });
    }
}

void IntegrationPluginEnergyMeters::thingRemoved(Thing *thing)
{
    qCDebug(dcEnergyMeters()) << "Thing removed" << thing->name();

    if (m_energyMeters.contains(thing)) {
        m_energyMeters.take(thing)->deleteLater();
    }

    if (myThings().isEmpty() && !m_updateTimer) {
        qCDebug(dcEnergyMeters()) << "Deleting update timer";
        m_updateTimer->deleteLater();
        m_updateTimer = nullptr;
    }
}

void IntegrationPluginEnergyMeters::onConnectionStateChanged(bool status)
{
    EnergyMeter *meter = static_cast<EnergyMeter *>(sender());
    Thing *thing = m_energyMeters.key(meter);
    if (!thing)
        return;

    thing->setStateValue(m_connectionStateTypeIds.value(thing->thingClassId()), status);
}

void IntegrationPluginEnergyMeters::onVoltageReceived(double voltage)
{
    EnergyMeter *meter = static_cast<EnergyMeter *>(sender());
    Thing *thing = m_energyMeters.key(meter);
    if (!thing)
        return;

    thing->setStateValue(m_voltageStateTypeIds.value(thing->thingClassId()), voltage);
}

void IntegrationPluginEnergyMeters::onCurrentReceived(double current)
{
    EnergyMeter *meter = static_cast<EnergyMeter *>(sender());
    Thing *thing = m_energyMeters.key(meter);
    if (!thing)
        return;

    thing->setStateValue(m_currentStateTypeIds.value(thing->thingClassId()), current);
}

void IntegrationPluginEnergyMeters::onActivePowerReceived(double power)
{
    EnergyMeter *meter = static_cast<EnergyMeter *>(sender());
    Thing *thing = m_energyMeters.key(meter);
    if (!thing)
        return;

    thing->setStateValue(m_activePowerStateTypeIds.value(thing->thingClassId()), power);
}

void IntegrationPluginEnergyMeters::onFrequencyReceived(double frequency)
{
    EnergyMeter *meter = static_cast<EnergyMeter *>(sender());
    Thing *thing = m_energyMeters.key(meter);
    if (!thing)
        return;

    thing->setStateValue(m_frequencyStateTypeIds.value(thing->thingClassId()), frequency);
}

void IntegrationPluginEnergyMeters::onPowerFactorReceived(double powerFactor)
{
    EnergyMeter *meter = static_cast<EnergyMeter *>(sender());
    Thing *thing = m_energyMeters.key(meter);
    if (!thing)
        return;

    thing->setStateValue(m_powerFactorStateTypeIds.value(thing->thingClassId()), powerFactor);
}

void IntegrationPluginEnergyMeters::onProducedEnergyReceived(double energy)
{
    EnergyMeter *meter = static_cast<EnergyMeter *>(sender());
    Thing *thing = m_energyMeters.key(meter);
    if (!thing)
        return;

    thing->setStateValue(m_totalEnergyProducedStateTypeIds.value(thing->thingClassId()), energy);
}

void IntegrationPluginEnergyMeters::onConsumedEnergyReceived(double energy)
{
    EnergyMeter *meter = static_cast<EnergyMeter *>(sender());
    Thing *thing = m_energyMeters.key(meter);
    if (!thing)
        return;

    thing->setStateValue(m_totalEnergyConsumedStateTypeIds.value(thing->thingClassId()), energy);
}