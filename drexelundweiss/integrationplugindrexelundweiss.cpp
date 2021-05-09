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

#include "integrationplugindrexelundweiss.h"
#include "plugininfo.h"

#include "hardwaremanager.h"
#include "hardware/modbus/modbusrtumaster.h"
#include "hardware/modbus/modbusrtuhardwareresource.h"

IntegrationPluginDrexelUndWeiss::IntegrationPluginDrexelUndWeiss()
{
    m_connectedStateTypeIds.insert(x2luThingClassId, x2luConnectedStateTypeId);
    m_connectedStateTypeIds.insert(x2wpThingClassId, x2wpConnectedStateTypeId);

    m_discoverySlaveAddressParamTypeIds.insert(x2luThingClassId, x2luDiscoverySlaveAddressParamTypeId);
    m_discoverySlaveAddressParamTypeIds.insert(x2wpThingClassId, x2wpDiscoverySlaveAddressParamTypeId);

    m_slaveIdParamTypeIds.insert(x2luThingClassId, x2luThingSlaveAddressParamTypeId);
    m_slaveIdParamTypeIds.insert(x2wpThingClassId, x2wpThingSlaveAddressParamTypeId);

    m_modbusUuidParamTypeIds.insert(x2luThingClassId, x2luThingModbusMasterUuidParamTypeId);
    m_modbusUuidParamTypeIds.insert(x2wpThingClassId, x2wpThingModbusMasterUuidParamTypeId);
}

void IntegrationPluginDrexelUndWeiss::init()
{
    connect(this, &IntegrationPluginDrexelUndWeiss::configValueChanged, this, &IntegrationPluginDrexelUndWeiss::onPluginConfigurationChanged);

    connect(hardwareManager()->modbusRtuResource(), &ModbusRtuHardwareResource::modbusRtuMasterRemoved, this, [=](const QUuid &modbusUuid){
        qCDebug(dcDrexelUndWeiss()) << "Modbus RTU master has been removed" << modbusUuid.toString();

        // Check if there is any device using this resource
        foreach (Thing *thing, m_modbusRtuMasters.keys()) {
            if (m_modbusRtuMasters.value(thing)->modbusUuid() == modbusUuid) {
                qCWarning(dcDrexelUndWeiss()) << "Hardware resource removed for" << thing << ". The thing will not be functional any more until a new resource has been configured for it.";
                m_modbusRtuMasters.remove(thing);
                thing->setStateValue(m_connectedStateTypeIds[thing->thingClassId()], false);
            }
        }
    });
}

void IntegrationPluginDrexelUndWeiss::discoverThings(ThingDiscoveryInfo *info)
{
    qCDebug(dcDrexelUndWeiss()) << "Discover things";

    if (hardwareManager()->modbusRtuResource()->modbusRtuMasters().isEmpty()) {
        info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("No Modbus RTU interface available."));
        return;
    }

    uint slaveAddress = info->params().paramValue(m_discoverySlaveAddressParamTypeIds.value(info->thingClassId())).toUInt();
    if (slaveAddress > 254 || slaveAddress == 0) {
        info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("Modbus slave address must be between 1 and 254"));
        return;
    }
    Q_FOREACH(ModbusRtuMaster *modbusMaster, hardwareManager()->modbusRtuResource()->modbusRtuMasters()) {
        qCDebug(dcDrexelUndWeiss()) << "Found RTU master resource" << modbusMaster << "connected" << modbusMaster->connected();
        if (!modbusMaster->connected()) {
            continue;
        }
        QString name;
        if (info->thingClassId() == x2wpThingClassId) {
            name = QT_TR_NOOP("X2 Heat pump");
        } else if (info->thingClassId() == x2luThingClassId) {
            name = QT_TR_NOOP("X2 Ventilation unit");
        }
        ThingDescriptor descriptor(info->thingClassId(), name, QT_TR_NOOP("Slave address ") +QString::number(slaveAddress)+" "+modbusMaster->serialPort());
        ParamList params;
        params << Param(m_slaveIdParamTypeIds.value(info->thingClassId()), slaveAddress);
        params << Param(m_modbusUuidParamTypeIds.value(info->thingClassId()), modbusMaster->modbusUuid());
        descriptor.setParams(params);
        info->addThingDescriptor(descriptor);
    }
    info->finish(Thing::ThingErrorNoError);
    return;
}

void IntegrationPluginDrexelUndWeiss::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcDrexelUndWeiss()) << "Setup thing" << thing->name();

    if (!m_connectedStateTypeIds.contains(thing->thingClassId())) {
        Q_ASSERT_X(false, "setupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }

    QUuid modbusMasterUuid = thing->paramValue(m_modbusUuidParamTypeIds.value(thing->thingClassId())).toUuid();
    uint slaveAddress = thing->paramValue(m_slaveIdParamTypeIds.value(thing->thingClassId())).toUInt();

    if (!hardwareManager()->modbusRtuResource()->hasModbusRtuMaster(modbusMasterUuid)) {
        return info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("Modbus RTU interface not available."));
    }
    ModbusRtuMaster *modbus = hardwareManager()->modbusRtuResource()->getModbusRtuMaster(modbusMasterUuid);
    if (!modbus->connected()) {
        return info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("Modbus RTU interface is not connected."));
    }

    ModbusRtuReply *reply = modbus->readHoldingRegister(slaveAddress, ModbusRegisterX2::Geraetetyp);
    connect(reply, &ModbusRtuReply::finished, reply, &ModbusRtuReply::deleteLater);
    connect(reply, &ModbusRtuReply::finished, info, [reply, modbus, info, thing, this] {
        if (reply->error() != ModbusRtuReply::Error::NoError) {
            qCWarning(dcDrexelUndWeiss()) << "Setup failed, received modbus error" << reply->errorString();
            return info->finish(Thing::ThingErrorHardwareNotAvailable);
        }

        if (reply->result().length() != 1) {
            qCWarning(dcDrexelUndWeiss()) << "Setup failed, received reply has an illegal length";
            return info->finish(Thing::ThingErrorHardwareNotAvailable);
        }
        if (thing->thingClassId() == x2luThingClassId && reply->result().first() == DeviceType::X2_LU) {
            info->finish(Thing::ThingErrorNoError);
            m_modbusRtuMasters.insert(thing, modbus);
        } else if (thing->thingClassId() == x2wpThingClassId && reply->result().first() == DeviceType::X2_WP) {
            info->finish(Thing::ThingErrorNoError);
            m_modbusRtuMasters.insert(thing, modbus);
        } else {
            qCWarning(dcDrexelUndWeiss()) << "Device on slave addresss" << reply->slaveAddress() << "is not the wanted one.";
            return info->finish(Thing::ThingErrorHardwareNotAvailable);
        }

    });
    connect(modbus, &ModbusRtuMaster::connectedChanged, this, &IntegrationPluginDrexelUndWeiss::onConnectionStateChanged);
}

void IntegrationPluginDrexelUndWeiss::postSetupThing(Thing *thing)
{
    qCDebug(dcDrexelUndWeiss()) << "Post setup thing" << thing->name();
    if (!m_refreshTimer) {
        qCDebug(dcDrexelUndWeiss()) << "Creating refresh timer";
        int refreshTime = configValue(drexelUndWeissPluginUpdateIntervalParamTypeId).toInt();
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(refreshTime);
        connect(m_refreshTimer, &PluginTimer::timeout, this, &IntegrationPluginDrexelUndWeiss::onRefreshTimer);
    }

    if ((thing->thingClassId() == x2luThingClassId) || (thing->thingClassId() == x2wpThingClassId)) {
        Thing *parentThing = myThings().findById(thing->parentId());
        if (!parentThing) {
            qCWarning(dcDrexelUndWeiss()) << "Could not find the parent Thing";
            return;
        }
        ModbusRtuMaster *modbus = m_modbusRtuMasters.value(parentThing);
        if (!modbus){
            qCWarning(dcDrexelUndWeiss()) << "No modbus interface available";
        }
        updateStates(thing);
        // Update states
    } else {
        Q_ASSERT_X(false, "postSetupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginDrexelUndWeiss::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    if (thing->thingClassId() == x2luThingClassId) {

        uint slaveAddress = thing->paramValue(x2luThingSlaveAddressParamTypeId).toUInt();
        if (action.actionTypeId() == x2luPowerActionTypeId) {
            bool power = action.paramValue(x2luPowerActionPowerParamTypeId).toBool();
            uint16_t data = 0;
            if (power) {
                data = getVentilationModeFromString(thing->stateValue(x2luVentilationModeStateTypeId).toString());
            } else {
                data = VentilationMode::ManuellStufe0;
            }
            sendWriteRequest(info, slaveAddress, ModbusRegisterX2::Betriebsart, data);

        } else if (action.actionTypeId() == x2luVentilationModeActionTypeId) {
            QString mode = action.param(x2luVentilationModeActionVentilationModeParamTypeId).value().toString();
            uint16_t data = getVentilationModeFromString(mode);
            sendWriteRequest(info, slaveAddress, ModbusRegisterX2::Betriebsart, data);

        } else {
            Q_ASSERT_X(false, "executeAction", QString("Unhandled ActionTypeId: %1").arg(action.actionTypeId().toString()).toUtf8());
        }
    } else if (thing->thingClassId() == x2wpThingClassId) {

        uint slaveAddress = thing->paramValue(x2wpThingSlaveAddressParamTypeId).toUInt();
        if (action.actionTypeId() == x2wpTargetTemperatureActionTypeId) {
            qreal targetTemp = (action.param(x2wpTargetTemperatureActionTargetTemperatureParamTypeId).value().toDouble());
            uint16_t data = static_cast<uint16_t>(qRound(targetTemp * 1000));
            sendWriteRequest(info, slaveAddress, ModbusRegisterX2::RaumSoll, data);

        } else if (action.actionTypeId() == x2wpTargetWaterTemperatureActionTypeId) {
            qreal targetWaterTemp = action.param(x2wpTargetWaterTemperatureActionTargetWaterTemperatureParamTypeId).value().toDouble();
            int data = static_cast<int>(qRound(targetWaterTemp * 1000));
            sendWriteRequest(info, slaveAddress, ModbusRegisterX2::BrauchwasserSolltermperatur, data);

        } else {
            Q_ASSERT_X(false, "executeAction", QString("Unhandled ActionTypeId: %1").arg(action.actionTypeId().toString()).toUtf8());
        }
    } else {
        Q_ASSERT_X(false, "executeAction", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginDrexelUndWeiss::sendWriteRequest(ThingActionInfo *info, uint slaveAddress, uint modbusRegister, uint16_t value)
{
    ModbusRtuMaster *modbus = m_modbusRtuMasters.value(info->thing());

    if (!modbus){
        qCWarning(dcDrexelUndWeiss()) << "Modbus RTU interface available";
        info->finish(Thing::ThingErrorHardwareFailure, tr("Modbus RTU interface not available."));
        return;
    }

    if (!modbus->connected()) {
        qCWarning(dcDrexelUndWeiss()) << "Modbus RTU interface not connected";
        info->finish(Thing::ThingErrorHardwareFailure, tr("Modbus RTU interface not connected."));
        return;
    }

    ModbusRtuReply *reply = modbus->writeHoldingRegisters(slaveAddress, modbusRegister, QVector<uint16_t>() << value);
    connect(reply, &ModbusRtuReply::finished, reply, &ModbusRtuReply::deleteLater);
    connect(reply, &ModbusRtuReply::finished, info, [info, reply] {

        if (reply->error() != ModbusRtuReply::Error::NoError) {
            return info->finish(Thing::ThingErrorHardwareFailure);
        }
        //TODO update thing state after successfull write requests
        info->finish(Thing::ThingErrorNoError);
    });
}


void IntegrationPluginDrexelUndWeiss::thingRemoved(Thing *thing)
{

    if (thing->thingClassId() == x2luThingClassId || thing->thingClassId() == x2luThingClassId) {
        m_modbusRtuMasters.remove(thing);
    }

    if (myThings().isEmpty()) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_refreshTimer);
        m_refreshTimer = nullptr;
    }
}

void IntegrationPluginDrexelUndWeiss::onRefreshTimer()
{
    foreach (Thing *thing, myThings()) {
        if (thing->thingClassId() == x2luThingClassId || thing->thingClassId() == x2wpThingClassId){
            updateStates(thing);
        }
    }
}

void IntegrationPluginDrexelUndWeiss::updateStates(Thing *thing)
{
    if (thing->thingClassId() == x2luThingClassId) {
        ModbusRtuMaster *modbus = m_modbusRtuMasters.value(thing);
        if (!modbus) {
            return;
        }
        uint slaveAddress = thing->paramValue(x2luThingSlaveAddressParamTypeId).toUInt();

        readHoldingRegister(thing, modbus, slaveAddress, ModbusRegisterX2::AktiveLuefterstufe);
        readHoldingRegister(thing, modbus, slaveAddress, ModbusRegisterX2::Betriebsart); // Ventilation mode
        readHoldingRegister(thing, modbus, slaveAddress, ModbusRegisterX2::CO2);
    }

    if (thing->thingClassId() == x2wpThingClassId) {
        ModbusRtuMaster *modbus = m_modbusRtuMasters.value(thing);
        if (!modbus) {
            return;
        }
        int slaveAddress = thing->paramValue(x2wpThingSlaveAddressParamTypeId).toUInt();

        readHoldingRegister(thing, modbus, slaveAddress, ModbusRegisterX2::Waermepumpe);
        readHoldingRegister(thing, modbus, slaveAddress, ModbusRegisterX2::RaumSoll);
        readHoldingRegister(thing, modbus, slaveAddress, ModbusRegisterX2::Raum);
        readHoldingRegister(thing, modbus, slaveAddress, ModbusRegisterX2::TemperaturWarmwasserspeicherUnten);
        readHoldingRegister(thing, modbus, slaveAddress, ModbusRegisterX2::BrauchwasserSolltermperatur);
        readHoldingRegister(thing, modbus, slaveAddress, ModbusRegisterX2::Auszenluft);
        readHoldingRegister(thing, modbus, slaveAddress, ModbusRegisterX2::Summenstoerung);
        readHoldingRegister(thing, modbus, slaveAddress, ModbusRegisterX2::LeistungKompressor);
        readHoldingRegister(thing, modbus, slaveAddress, ModbusRegisterX2::LeistungWarmwasser);
        readHoldingRegister(thing, modbus, slaveAddress, ModbusRegisterX2::LeistungRaumheizung);
        readHoldingRegister(thing, modbus, slaveAddress, ModbusRegisterX2::LeistungLuftvorwaermung);
        readHoldingRegister(thing, modbus, slaveAddress, ModbusRegisterX2::EnergieKompressor);
        readHoldingRegister(thing, modbus, slaveAddress, ModbusRegisterX2::EnergieWarmwasser);
        readHoldingRegister(thing, modbus, slaveAddress, ModbusRegisterX2::EnergieRaumheizung);
        readHoldingRegister(thing, modbus, slaveAddress, ModbusRegisterX2::EnergieLuftvorerwarrmung);
    }
}

void IntegrationPluginDrexelUndWeiss::readHoldingRegister(Thing *thing, ModbusRtuMaster *modbus, uint slaveAddress, uint modbusRegister)
{
    ModbusRtuReply *reply = modbus->readHoldingRegister(slaveAddress, modbusRegister);
    connect(reply, &ModbusRtuReply::finished, reply, &ModbusRtuReply::deleteLater);
    connect(reply, &ModbusRtuReply::finished, this, [reply, thing, this] {
        if (reply->error() != ModbusRtuReply::Error::NoError) {
            qCWarning(dcDrexelUndWeiss()) << "Modbus error" << reply->errorString();
            thing->setStateValue(m_connectedStateTypeIds.value(thing->thingClassId()), false);
            return;
        }
        thing->setStateValue(m_connectedStateTypeIds.value(thing->thingClassId()), true);
        QVector<uint16_t> values = reply->result();

        if (thing->thingClassId() == x2luThingClassId) {
            switch (reply->registerAddress()) {
            case ModbusRegisterX2::Waermepumpe:
                thing->setStateValue(x2wpPowerStateTypeId, values[0]);
                break;

            case ModbusRegisterX2::RaumSoll:
                thing->setStateValue(x2wpTargetTemperatureStateTypeId, values[0]/1000.00);
                break;

            case ModbusRegisterX2::Raum:
                thing->setStateValue(x2wpTemperatureStateTypeId, values[0]/1000.00);
                break;

            case ModbusRegisterX2::TemperaturWarmwasserspeicherUnten:
                thing->setStateValue(x2wpWaterTemperatureStateTypeId, values[0]/1000.00);
                break;
            case ModbusRegisterX2::BrauchwasserSolltermperatur:
                thing->setStateValue(x2wpTargetWaterTemperatureStateTypeId, values[0]/1000.00);
                break;
            case ModbusRegisterX2::Auszenluft:
                thing->setStateValue(x2wpOutsideAirTemperatureStateTypeId, values[0]/1000.00);
                break;

            case ModbusRegisterX2::Summenstoerung:
                if (values[0] != 0) {
                    //get actual error
                } else {
                    thing->setStateValue(x2wpErrorStateTypeId, "No Error");
                }
                break;

            case ModbusRegisterX2::StoerungAbluftventilator:
                if (values[0] != 0)
                    thing->setStateValue(x2wpErrorStateTypeId, "Exhaust fan");
                break;

            case ModbusRegisterX2::StoerungBoilerfuehlerElektroheizstab:
                if (values[0] != 0)
                    thing->setStateValue(x2wpErrorStateTypeId, "Boiler sensor electric heating element");
                break;

            case ModbusRegisterX2::StoerungBoilerfuehlerSolar:
                if (values[0] != 0)
                    thing->setStateValue(x2wpErrorStateTypeId, "Boiler sensor solar");
                break;

            case  ModbusRegisterX2::StoerungBoilerfuehlerWaermepumpe:
                if (values[0] != 0)
                    thing->setStateValue(x2wpErrorStateTypeId, "Boiler sensor heat pump");
                break;

            case  ModbusRegisterX2::StoerungBoileruebertemperatur:
                if (values[0] != 0)
                    thing->setStateValue(x2wpErrorStateTypeId, "Boiler overtemperature");
                break;

            case  ModbusRegisterX2::StoerungCO2Sensor:
                if (values[0] != 0)
                    thing->setStateValue(x2wpErrorStateTypeId, "CO2-Sensor");
                break;

            case  ModbusRegisterX2::StoerungDruckverlustAbluftZuGrosz:
                if (values[0] != 0)
                    thing->setStateValue(x2wpErrorStateTypeId, "Pressure loss exhaust air too big");
                break;

            case ModbusRegisterX2::StoerungDruckverlustZuluftZuGrosz:
                if (values[0] != 0)
                    thing->setStateValue(x2wpErrorStateTypeId, "Pressure loss supply air too large");
                break;

            case ModbusRegisterX2::StoerungDurchflussmengeHeizgkreis:
                if (values[0] != 0)
                    thing->setStateValue(x2wpErrorStateTypeId, "Flow rate of heating circuit");
                break;

            case ModbusRegisterX2::StoerungDurchflussmengeSolekreis:
                if (values[0] != 0)
                    thing->setStateValue(x2wpErrorStateTypeId, "Flow rate brine circuit");
                break;

            case ModbusRegisterX2::StoerungTeilnehmerNichtErreichbar:
                if (values[0] != 0)
                    thing->setStateValue(x2wpErrorStateTypeId, "Participant not available");
                break;

            case ModbusRegisterX2::StoerungTemperaturfuehlerAuszenluft:
                if (values[0] != 0)
                    thing->setStateValue(x2wpErrorStateTypeId, "Temperature sensor outside air");
                break;

            case ModbusRegisterX2::StoerungTemperaturfuehlerHeizkreisVorlauf:
                if (values[0] != 0)
                    thing->setStateValue(x2wpErrorStateTypeId, "Temperature sensor heating circuit flow");
                break;

            case ModbusRegisterX2::StoerungTemperaturfuehlerRaum:
                if (values[0] != 0)
                    thing->setStateValue(x2wpErrorStateTypeId, "Temperature sensor room");
                break;

            case ModbusRegisterX2::StoerungTemperaturfuehlerSolarkollektor:
                if (values[0] != 0)
                    thing->setStateValue(x2wpErrorStateTypeId, "Temperature sensor solar collector");
                break;

            case ModbusRegisterX2::StoerungTemperaturfuehlerSole:
                if (values[0] != 0)
                    thing->setStateValue(x2wpErrorStateTypeId, "Temperature sensor brine");
                break;

            case ModbusRegisterX2::StoerungTemperaturfuehlerSoleAuszenluft:
                if (values[0] != 0)
                    thing->setStateValue(x2wpErrorStateTypeId, "Temperature sensor brine outside air");
                break;

            case ModbusRegisterX2::StoerungWaermepumpeHochdruck:
                if (values[0] != 0)
                    thing->setStateValue(x2wpErrorStateTypeId, "Heat pump high pressure");
                break;

            case ModbusRegisterX2::StoerungWaermepumpeNiederdruck:
                if (values[0] != 0)
                    thing->setStateValue(x2wpErrorStateTypeId, "Heat pump low pressure");
                break;

            case ModbusRegisterX2::StoerungWertNichtZulaessig:
                if (values[0] != 0)
                    thing->setStateValue(x2wpErrorStateTypeId, "Value not allowed");
                break;

            case ModbusRegisterX2::StoerungZuluftventilator:
                if (values[0] != 0)
                    thing->setStateValue(x2wpErrorStateTypeId, "Supply air fan");
                break;

            case ModbusRegisterX2::LeistungKompressor:
                thing->setStateValue(x2wpPowerCompressorStateTypeId, values[0]/1000.00);
                break;
            case ModbusRegisterX2::LeistungWarmwasser:
                thing->setStateValue(x2wpPowerWaterHeatingStateTypeId, values[0]/1000.00);
                break;

            case ModbusRegisterX2::LeistungRaumheizung:
                thing->setStateValue(x2wpPowerRoomHeatingStateTypeId, values[0]/1000.00);
                break;

            case ModbusRegisterX2::LeistungLuftvorwaermung:
                thing->setStateValue(x2wpPowerAirPreheatingStateTypeId, values[0]/1000.00);
                break;

            case ModbusRegisterX2::EnergieKompressor:
                thing->setStateValue(x2wpEnergyCompressorStateTypeId, values[0]/1000.00);
                break;

            case ModbusRegisterX2::EnergieWarmwasser:
                thing->setStateValue(x2wpEnergyWaterHeatingStateTypeId, values[0]/1000.00);
                break;

            case ModbusRegisterX2::EnergieRaumheizung:
                thing->setStateValue(x2wpEnergyRoomHeatingStateTypeId, values[0]/1000.00);
                break;

            case ModbusRegisterX2::EnergieLuftvorerwarrmung:
                thing->setStateValue(x2wpEnergyAirPreheatingStateTypeId, values[0]/1000.00);
                break;
            default:
                break;
            }
        } else if (thing->thingClassId() == x2wpThingClassId) {

            switch (reply->registerAddress()) {
            case ModbusRegisterX2::Betriebsart:
                if (values[0] == VentilationMode::ManuellStufe0) {
                    thing->setStateValue(x2luVentilationModeStateTypeId, "Manual level 0");
                } else if (values[0] == VentilationMode::ManuellStufe1) {
                    thing->setStateValue(x2luVentilationModeStateTypeId, "Manual level 1");
                } else if (values[0] == VentilationMode::ManuellStufe2) {
                    thing->setStateValue(x2luVentilationModeStateTypeId, "Manual level 2");
                } else if (values[0] == VentilationMode::ManuellStufe3) {
                    thing->setStateValue(x2luVentilationModeStateTypeId, "Manual level 3");
                } else if (values[0] == VentilationMode::Automatikbetrieb) {
                    thing->setStateValue(x2luVentilationModeStateTypeId, "Automatic");
                } else if (values[0] == VentilationMode::Party) {
                    thing->setStateValue(x2luVentilationModeStateTypeId, "Party");
                }
                if (values[0] == VentilationMode::ManuellStufe0) {
                    thing->setStateValue(x2luPowerStateTypeId, false);
                } else {
                    thing->setStateValue(x2luPowerStateTypeId, true);
                }
                break;
            case ModbusRegisterX2::AktiveLuefterstufe:
                thing->setStateValue(x2luActiveVentilationLevelStateTypeId, values[0]);
                break;
            case ModbusRegisterX2::CO2:
                thing->setStateValue(x2luCo2StateTypeId, values[0]);
                break;
            }
        }
    });
}

VentilationMode IntegrationPluginDrexelUndWeiss::getVentilationModeFromString(const QString &modeString)
{
    if (modeString == "Manual level 0") {
        return VentilationMode::ManuellStufe0;
    } else if(modeString == "Manual level 1") {
        return VentilationMode::ManuellStufe1;
    } else if(modeString == "Manual level 2") {
        return VentilationMode::ManuellStufe2;
    }else if(modeString == "Manual level 3") {
        return VentilationMode::ManuellStufe3;
    } else if(modeString == "Automatic") {
        return VentilationMode::Automatikbetrieb;
    } else if(modeString == "Party") {
        return VentilationMode::Party;
    } else {
        qCWarning(dcDrexelUndWeiss()) << "Unknown ventilation mode string" << modeString;
    }
    return VentilationMode::ManuellStufe0;
}


void IntegrationPluginDrexelUndWeiss::onPluginConfigurationChanged(const ParamTypeId &paramTypeId, const QVariant &value)
{
    // Check refresh schedule
    if (paramTypeId == drexelUndWeissPluginUpdateIntervalParamTypeId) {
        if (m_refreshTimer) {
            int refreshTime = value.toInt();
            m_refreshTimer->stop();
            m_refreshTimer->startTimer(refreshTime);
        }
    }
}

void IntegrationPluginDrexelUndWeiss::onConnectionStateChanged(bool status)
{
    ModbusRtuMaster *modbusRtuMaster = static_cast<ModbusRtuMaster *>(sender());
    Thing *thing = m_modbusRtuMasters.key(modbusRtuMaster);
    if (!thing)
        return;
    thing->setStateValue(m_connectedStateTypeIds.value(thing->thingClassId()), status);
}
