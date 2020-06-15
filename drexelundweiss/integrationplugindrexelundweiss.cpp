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

#include "integrationplugindrexelundweiss.h"
#include "plugininfo.h"
#include "modbusdegisterdefinition.h"

IntegrationPluginDrexelUndWeiss::IntegrationPluginDrexelUndWeiss()
{

}

IntegrationPluginDrexelUndWeiss::~IntegrationPluginDrexelUndWeiss()
{

}

void IntegrationPluginDrexelUndWeiss::init()
{
    connect(this, &IntegrationPluginDrexelUndWeiss::configValueChanged, this, &IntegrationPluginDrexelUndWeiss::onPluginConfigurationChanged);
}

void IntegrationPluginDrexelUndWeiss::discoverThings(ThingDiscoveryInfo *info)
{
    // Create the list of available serial interfaces
    QList<ThingDescriptor> thingDescriptors;

    if (info->thingClassId() == modbusConnectionThingClassId) {
        Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
            if (m_usedSerialPorts.contains(port.systemLocation())){
                //device already in use
                qCDebug(dcDrexelUndWeiss()) << "Found serial port that is already used:" << port.portName();
            } else {
                //Serial port is not yet used, create now a new one
                qCDebug(dcDrexelUndWeiss()) << "Found serial port:" << port.portName();
                QString description = port.manufacturer() + " " + port.description();
                ThingDescriptor descriptor(info->thingClassId(), port.portName(), description);
                ParamList parameters;
                parameters.append(Param(modbusConnectionThingSerialPortParamTypeId, port.systemLocation()));
                descriptor.setParams(parameters);
                info->addThingDescriptor(descriptor);
            }
        }
        info->finish(Thing::ThingErrorNoError);
        return;
    }

    if (info->thingClassId() == x2luThingClassId) {
        info->finish(Thing::ThingErrorNoError);
        return;
    }

    if (info->thingClassId() == x2wpThingClassId) {
        info->finish(Thing::ThingErrorNoError);
        return;
    }

    info->finish(Thing::ThingErrorDeviceClassNotFound);
    return;
}

void IntegrationPluginDrexelUndWeiss::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    if (thing->thingClassId() == modbusConnectionThingClassId) {

        QString serialPort = thing->paramValue(modbusConnectionThingSerialPortParamTypeId).toString();
        int baudRate = thing->paramValue(modbusConnectionThingBaudRateParamTypeId).toInt();

        ModbusRTUMaster *modbus = new ModbusRTUMaster(serialPort, baudRate, QSerialPort::Parity::NoParity, 8, 1, this);
        connect(modbus, &ModbusRTUMaster::connectionStateChanged, this, &IntegrationPluginDrexelUndWeiss::onConnectionStateChanged);
        connect(modbus, &ModbusRTUMaster::receivedCoil, this, &IntegrationPluginDrexelUndWeiss::onReceivedCoil);
        connect(modbus, &ModbusRTUMaster::receivedDiscreteInput, this, &IntegrationPluginDrexelUndWeiss::onReceivedDiscreteInput);
        connect(modbus, &ModbusRTUMaster::receivedHoldingRegister, this, &IntegrationPluginDrexelUndWeiss::onReceivedHoldingRegister);
        connect(modbus, &ModbusRTUMaster::receivedInputRegister, this, &IntegrationPluginDrexelUndWeiss::onReceivedInputRegister);


        m_modbusRTUMasters.insert(thing, modbus);
        m_usedSerialPorts.append(serialPort);
        info->finish(Thing::ThingErrorNoError);
        return;
    }

    if (thing->thingClassId() == x2luThingClassId) {
        info->finish(Thing::ThingErrorNoError);
        return;
    }

    if (thing->thingClassId() == x2wpThingClassId) {
        info->finish(Thing::ThingErrorNoError);
        return;
    }
    info->finish(Thing::ThingErrorDeviceClassNotFound);
    return;
}

void IntegrationPluginDrexelUndWeiss::postSetupDevice(Thing *thing)
{
    if (!m_refreshTimer) {
        // Refresh timer for TCP read
        int refreshTime = configValue(drexelUndWeissPluginUpdateIntervalParamTypeId).toInt();
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(refreshTime);
        connect(m_refreshTimer, &PluginTimer::timeout, this, &IntegrationPluginDrexelUndWeiss::onRefreshTimer);
    }

    if (thing->thingClassId() == modbusConnectionThingClassId) {
        // read Register 5000 and emit auto-device
        ModbusRTUMaster *modbus = m_modbusRTUMasters.value(device);
        if (!modbus){
            qCWarning(dcDrexelUndWeiss()) << "No modbus master available";
        }
        thing->setStateValue(modbusConnectionConnectedStateTypeId, true);
    }

    if ((thing->thingClassId() == x2luThingClassId) || (thing->thingClassId() == x2wpThingClassId)) {
        Device *parentDevice = myThings().findById(thing->parentId());
        if (!parentDevice) {
            qWarning(dcDrexelUndWeiss()) << "Could not find the parent device";
            return;
        }
        ModbusRTUMaster *modbus = m_modbusRTUMasters.value(parentDevice);
        if (!modbus){
            qCWarning(dcDrexelUndWeiss()) << "No modbus interface available";
        }
        updateStates(device);
        // Update states
    }
}

void IntegrationPluginDrexelUndWeiss::executeAction(DeviceActionInfo *info)
{
    Thing *thing = info->device();
    Action action = info->action();

    if (thing->thingClassId() == modbusConnectionThingClassId) {

        ModbusRTUMaster *modbus = m_modbusRTUMasters.value(device);
        if (!modbus){
            qCWarning(dcDrexelUndWeiss()) << "No modbus interface available";
            info->finish(Thing::ThingErrorHardwareFailure);
            return;
        }

        if (action.actionTypeId() == modbusConnectionDiscoverDevicesActionTypeId) {
            int slave = action.param(modbusConnectionDiscoverDevicesActionSlaveAddressParamTypeId).value().toInt();
            discoverModbusSlaves(modbus, slave);
            info->finish(Thing::ThingErrorNoError);
            return;
        } else {
            info->finish(Thing::ThingErrorActionTypeNotFound);
        }
    } else if (thing->thingClassId() == x2luThingClassId) {
        Device *parentDevice = myThings().findById(thing->parentId());
        if (!parentDevice) {
            qWarning(dcDrexelUndWeiss()) << "Could not find the parent device";
            info->finish(Thing::ThingErrorHardwareFailure);
            return;
        }
        ModbusRTUMaster *modbus = m_modbusRTUMasters.value(parentDevice);
        int slave = thing->paramValue(x2luThingSlaveAddressParamTypeId).toInt();
        if (!modbus){
            qCWarning(dcDrexelUndWeiss()) << "No modbus interface available";
            info->finish(Thing::ThingErrorHardwareFailure);
            return;
        }

        if (action.actionTypeId() == x2luVentilationModeActionTypeId) {
            QString mode = action.param(x2luVentilationModeActionVentilationModeParamTypeId).value().toString();
            int data = 0;

            if (mode == "Manual level 0") {
                data = VentialtionMode::ManuellStufe0;
            } else if(mode == "Manual level 1") {
                data = VentialtionMode::ManuellStufe1;
            } else if(mode == "Manual level 2") {
                data = VentialtionMode::ManuellStufe2;
            }else if(mode == "Manual level 3") {
                data = VentialtionMode::ManuellStufe3;
            } else if(mode == "Automatic") {
                data = VentialtionMode::Automatikbetrieb;
            } else if(mode == "Party") {
                data = VentialtionMode::Party;
            }
            m_pendingActions.insert(modbus->writeHoldingRegister(slave, ModbusRegisterX2::Betriebsart, data), info);
            return;
        } else {
            info->finish(Thing::ThingErrorActionTypeNotFound);
        }
    } else if (thing->thingClassId() == x2wpThingClassId) {
        Device *parentDevice = myThings().findById(thing->parentId());
        if (!parentDevice) {
            qWarning(dcDrexelUndWeiss()) << "Could not find modbus interface";
            info->finish(Thing::ThingErrorHardwareFailure);
            return;
        }
        ModbusRTUMaster *modbus = m_modbusRTUMasters.value(parentDevice);
        int slave = thing->paramValue(x2wpThingSlaveAddressParamTypeId).toInt();
        if (!modbus){
            qCWarning(dcDrexelUndWeiss()) << "No modbus master available";
            info->finish(Thing::ThingErrorHardwareFailure);
            return;
        }

        if (action.actionTypeId() == x2wpTargetTemperatureActionTypeId) {
            qreal targetTemp = (action.param(x2wpTargetTemperatureActionTargetTemperatureParamTypeId).value().toDouble());
            int data = static_cast<int>(qRound(targetTemp * 1000));
            m_pendingActions.insert(modbus->writeHoldingRegister(slave,ModbusRegisterX2::RaumSoll, data), info);
            return;
        } else if (action.actionTypeId() == x2wpTargetWaterTemperatureActionTypeId) {
            qreal targetWaterTemp = action.param(x2wpTargetWaterTemperatureActionTargetWaterTemperatureParamTypeId).value().toDouble();
            int data = static_cast<int>(qRound(targetWaterTemp * 1000));
            m_pendingActions.insert(modbus->writeHoldingRegister(slave, ModbusRegisterX2::BrauchwasserSolltermperatur, data), info);
            return;
        } else {
            info->finish(Thing::ThingErrorActionTypeNotFound);
        }
    } else {
        info->finish(Thing::ThingErrorDeviceClassNotFound);
    }
}


void IntegrationPluginDrexelUndWeiss::deviceRemoved(Thing *thing)
{
    if (thing->thingClassId() == modbusConnectionThingClassId) {

        ModbusRTUMaster *modbus = m_modbusRTUMasters.take(device);
        if (!modbus){
            qCWarning(dcDrexelUndWeiss()) << "No modbus interface available";
            return;
        }
        m_usedSerialPorts.removeAll(modbus->serialPort());
        modbus->deleteLater();
    }
}

void IntegrationPluginDrexelUndWeiss::onRefreshTimer()
{
    foreach (Thing *thing, myThings()) {

        if (thing->thingClassId() == modbusConnectionThingClassId) {
            ModbusRTUMaster *modbus = m_modbusRTUMasters.value(device);

            if (!modbus) {
                qCWarning(dcDrexelUndWeiss()) << "No modbus master available";
                return;
            }
        } else if (thing->thingClassId() == x2luThingClassId || thing->thingClassId() == x2wpThingClassId){
            updateStates(device);
        }
    }
}

void IntegrationPluginDrexelUndWeiss::updateStates(Thing *thing)
{
    if (thing->thingClassId() == x2luThingClassId) {
        Device *parent = myThings().findById(thing->parentId());
        ModbusRTUMaster *modbus = m_modbusRTUMasters.value(parent);
        int slave = thing->paramValue(x2luThingSlaveAddressParamTypeId).toInt();

        modbus->readHoldingRegister(slave, ModbusRegisterX2::AktiveLuefterstufe);
        modbus->readHoldingRegister(slave, ModbusRegisterX2::Betriebsart); // Ventilation mode
        modbus->readHoldingRegister(slave, ModbusRegisterX2::CO2);
    }

    if (thing->thingClassId() == x2wpThingClassId) {
        Device *parent = myThings().findById(thing->parentId());
        ModbusRTUMaster *modbus = m_modbusRTUMasters.value(parent);
        int slave = thing->paramValue(x2wpThingSlaveAddressParamTypeId).toInt();

        modbus->readHoldingRegister(slave, ModbusRegisterX2::Waermepumpe);
        modbus->readHoldingRegister(slave, ModbusRegisterX2::RaumSoll);
        modbus->readHoldingRegister(slave, ModbusRegisterX2::Raum);
        modbus->readHoldingRegister(slave, ModbusRegisterX2::TemperaturWarmwasserspeicherUnten);
        modbus->readHoldingRegister(slave, ModbusRegisterX2::BrauchwasserSolltermperatur);
        modbus->readHoldingRegister(slave, ModbusRegisterX2::Auszenluft);
        modbus->readHoldingRegister(slave, ModbusRegisterX2::Summenstoerung);
        modbus->readHoldingRegister(slave, ModbusRegisterX2::LeistungKompressor);
        modbus->readHoldingRegister(slave, ModbusRegisterX2::LeistungWarmwasser);
        modbus->readHoldingRegister(slave, ModbusRegisterX2::LeistungRaumheizung);
        modbus->readHoldingRegister(slave, ModbusRegisterX2::LeistungLuftvorwaermung);
        modbus->readHoldingRegister(slave, ModbusRegisterX2::EnergieKompressor);
        modbus->readHoldingRegister(slave, ModbusRegisterX2::EnergieWarmwasser);
        modbus->readHoldingRegister(slave, ModbusRegisterX2::EnergieRaumheizung);
        modbus->readHoldingRegister(slave, ModbusRegisterX2::EnergieLuftvorerwarrmung);
    }
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
    Q_UNUSED(status)
}

void IntegrationPluginDrexelUndWeiss::onReceivedCoil(int slaveAddress, int modbusRegister, bool value)
{
    Q_UNUSED(slaveAddress)
    Q_UNUSED(modbusRegister)
    Q_UNUSED(value)
}

void IntegrationPluginDrexelUndWeiss::onReceivedDiscreteInput(int slaveAddress, int modbusRegister, bool value)
{
    Q_UNUSED(slaveAddress)
    Q_UNUSED(modbusRegister)
    Q_UNUSED(value)
}

void IntegrationPluginDrexelUndWeiss::onReceivedHoldingRegister(int slaveAddress, int modbusRegister, int value)
{
    ModbusRTUMaster *modbus = static_cast<ModbusRTUMaster *>(sender());

    if (m_modbusRTUMasters.values().contains(modbus) ){
        Device *parentDevice = m_modbusRTUMasters.key(static_cast<ModbusRTUMaster *>(modbus));

        foreach(Thing *thing, myThings().filterByParentDeviceId(parentthing->id())) {
            if (thing->thingClassId() == x2luThingClassId && thing->paramValue(x2luThingSlaveAddressParamTypeId) == slaveAddress) {
                switch (modbusRegister) {
                case ModbusRegisterX2::Waermepumpe:
                    thing->setStateValue(x2wpPowerStateTypeId, value);
                    break;

                case ModbusRegisterX2::RaumSoll:
                    thing->setStateValue(x2wpTargetTemperatureStateTypeId, value/1000.00);
                    break;

                case ModbusRegisterX2::Raum:
                    thing->setStateValue(x2wpTemperatureStateTypeId, value/1000.00);
                    break;

                case ModbusRegisterX2::TemperaturWarmwasserspeicherUnten:
                    thing->setStateValue(x2wpWaterTemperatureStateTypeId, value/1000.00);
                    break;
                case ModbusRegisterX2::BrauchwasserSolltermperatur:
                    thing->setStateValue(x2wpTargetWaterTemperatureStateTypeId, value/1000.00);
                    break;
                case ModbusRegisterX2::Auszenluft:
                    thing->setStateValue(x2wpOutsideAirTemperatureStateTypeId, value/1000.00);
                    break;

                case ModbusRegisterX2::Summenstoerung:
                    if (value != 0) {
                        //get actual error
                    } else {
                        thing->setStateValue(x2wpErrorStateTypeId, "No Error");
                    }
                    break;

                case ModbusRegisterX2::StoerungAbluftventilator:
                    if (value != 0)
                        thing->setStateValue(x2wpErrorStateTypeId, "Exhaust fan");
                    break;

                case ModbusRegisterX2::StoerungBoilerfuehlerElektroheizstab:
                    if (value != 0)
                        thing->setStateValue(x2wpErrorStateTypeId, "Boiler sensor electric heating element");
                    break;

                case ModbusRegisterX2::StoerungBoilerfuehlerSolar:
                    if (value != 0)
                        thing->setStateValue(x2wpErrorStateTypeId, "Boiler sensor solar");
                    break;

                case  ModbusRegisterX2::StoerungBoilerfuehlerWaermepumpe:
                    if (value != 0)
                        thing->setStateValue(x2wpErrorStateTypeId, "Boiler sensor heat pump");
                    break;

                case  ModbusRegisterX2::StoerungBoileruebertemperatur:
                    if (value != 0)
                        thing->setStateValue(x2wpErrorStateTypeId, "Boiler overtemperature");
                    break;

                case  ModbusRegisterX2::StoerungCO2Sensor:
                    if (value != 0)
                        thing->setStateValue(x2wpErrorStateTypeId, "CO2-Sensor");
                    break;

                case  ModbusRegisterX2::StoerungDruckverlustAbluftZuGrosz:
                    if (value != 0)
                        thing->setStateValue(x2wpErrorStateTypeId, "Pressure loss exhaust air too big");
                    break;

                case ModbusRegisterX2::StoerungDruckverlustZuluftZuGrosz:
                    if (value != 0)
                        thing->setStateValue(x2wpErrorStateTypeId, "Pressure loss supply air too large");
                    break;

                case ModbusRegisterX2::StoerungDurchflussmengeHeizgkreis:
                    if (value != 0)
                        thing->setStateValue(x2wpErrorStateTypeId, "Flow rate of heating circuit");
                    break;

                case ModbusRegisterX2::StoerungDurchflussmengeSolekreis:
                    if (value != 0)
                        thing->setStateValue(x2wpErrorStateTypeId, "Flow rate brine circuit");
                    break;

                case ModbusRegisterX2::StoerungTeilnehmerNichtErreichbar:
                    if (value != 0)
                        thing->setStateValue(x2wpErrorStateTypeId, "Participant not available");
                    break;

                case ModbusRegisterX2::StoerungTemperaturfuehlerAuszenluft:
                    if (value != 0)
                        thing->setStateValue(x2wpErrorStateTypeId, "Temperature sensor outside air");
                    break;

                case ModbusRegisterX2::StoerungTemperaturfuehlerHeizkreisVorlauf:
                    if (value != 0)
                        thing->setStateValue(x2wpErrorStateTypeId, "Temperature sensor heating circuit flow");
                    break;

                case ModbusRegisterX2::StoerungTemperaturfuehlerRaum:
                    if (value != 0)
                        thing->setStateValue(x2wpErrorStateTypeId, "Temperature sensor room");
                    break;

                case ModbusRegisterX2::StoerungTemperaturfuehlerSolarkollektor:
                    if (value != 0)
                        thing->setStateValue(x2wpErrorStateTypeId, "Temperature sensor solar collector");
                    break;

                case ModbusRegisterX2::StoerungTemperaturfuehlerSole:
                    if (value != 0)
                        thing->setStateValue(x2wpErrorStateTypeId, "Temperature sensor brine");
                    break;

                case ModbusRegisterX2::StoerungTemperaturfuehlerSoleAuszenluft:
                    if (value != 0)
                        thing->setStateValue(x2wpErrorStateTypeId, "Temperature sensor brine outside air");
                    break;

                case ModbusRegisterX2::StoerungWaermepumpeHochdruck:
                    if (value != 0)
                        thing->setStateValue(x2wpErrorStateTypeId, "Heat pump high pressure");
                    break;

                case ModbusRegisterX2::StoerungWaermepumpeNiederdruck:
                    if (value != 0)
                        thing->setStateValue(x2wpErrorStateTypeId, "Heat pump low pressure");
                    break;

                case ModbusRegisterX2::StoerungWertNichtZulaessig:
                    if (value != 0)
                        thing->setStateValue(x2wpErrorStateTypeId, "Value not allowed");
                    break;

                case ModbusRegisterX2::StoerungZuluftventilator:
                    if (value != 0)
                        thing->setStateValue(x2wpErrorStateTypeId, "Supply air fan");
                    break;

                case ModbusRegisterX2::LeistungKompressor:
                    thing->setStateValue(x2wpPowerCompressorStateTypeId, value/1000.00);
                    break;
                case ModbusRegisterX2::LeistungWarmwasser:
                    thing->setStateValue(x2wpPowerWaterHeatingStateTypeId, value/1000.00);
                    break;

                case ModbusRegisterX2::LeistungRaumheizung:
                    thing->setStateValue(x2wpPowerRoomHeatingStateTypeId, value/1000.00);
                    break;

                case ModbusRegisterX2::LeistungLuftvorwaermung:
                    thing->setStateValue(x2wpPowerAirPreheatingStateTypeId, value/1000.00);
                    break;

                case ModbusRegisterX2::EnergieKompressor:
                    thing->setStateValue(x2wpEnergyCompressorStateTypeId, value/1000.00);
                    break;

                case ModbusRegisterX2::EnergieWarmwasser:
                    thing->setStateValue(x2wpEnergyWaterHeatingStateTypeId, value/1000.00);
                    break;

                case ModbusRegisterX2::EnergieRaumheizung:
                    thing->setStateValue(x2wpEnergyRoomHeatingStateTypeId, value/1000.00);
                    break;

                case ModbusRegisterX2::EnergieLuftvorerwarrmung:
                    thing->setStateValue(x2wpEnergyAirPreheatingStateTypeId, value/1000.00);
                    break;
                default:
                    break;
                }
            } else if (thing->thingClassId() == x2wpThingClassId && thing->paramValue(x2wpThingSlaveAddressParamTypeId) == slaveAddress) {

                switch (modbusRegister) {
                case ModbusRegisterX2::Betriebsart:
                    if (value == VentialtionMode::ManuellStufe0) {
                        thing->setStateValue(x2luVentilationModeStateTypeId, "Manual level 0");
                    } else if (value == VentialtionMode::ManuellStufe1) {
                        thing->setStateValue(x2luVentilationModeStateTypeId, "Manual level 1");
                    } else if (value == VentialtionMode::ManuellStufe2) {
                        thing->setStateValue(x2luVentilationModeStateTypeId, "Manual level 2");
                    } else if (value == VentialtionMode::ManuellStufe3) {
                        thing->setStateValue(x2luVentilationModeStateTypeId, "Manual level 3");
                    } else if (value == VentialtionMode::Automatikbetrieb) {
                        thing->setStateValue(x2luVentilationModeStateTypeId, "Automatic");
                    } else if (value == VentialtionMode::Party) {
                        thing->setStateValue(x2luVentilationModeStateTypeId, "Party");
                    }
                    break;
                case ModbusRegisterX2::AktiveLuefterstufe:
                    thing->setStateValue(x2luActiveVentilationLevelStateTypeId, value);
                    break;
                case ModbusRegisterX2::CO2:
                    thing->setStateValue(x2luCo2StateTypeId, value);
                    break;
                }
            }
        }

        if (modbusRegister == ModbusRegisterX2::Geraetetyp) {
            switch (value) {
            case DeviceType::X2_WP: {
                qDebug(dcDrexelUndWeiss()) << "Discovered X2 heat pump";
                QList<ThingDescriptor> thingDescriptors;
                ThingDescriptor descriptor(x2wpThingClassId, "X2 WP", "Drexel und Weiss", parentthing->id());
                ParamList params;

                //modbus->readHoldingRegister(slaveAddress, ModbusRegisterX2::SoftwareVersion);
                //params.append(Param(x2wpThingSofwareVersionParamTypeId, data));
                params.append(Param(x2wpThingSlaveAddressParamTypeId, slaveAddress));
                descriptor.setParams(params);
                thingDescriptors.append(descriptor);
                emit autoDevicesAppeared(thingDescriptors);
                break;
            }
            case DeviceType::X2_LU: {
                qDebug(dcDrexelUndWeiss()) << "Discovered X2 ventilation unit";
                QList<ThingDescriptor> thingDescriptors;
                ThingDescriptor descriptor(x2luThingClassId, "X2 LU", "Drexel und Weiss", parentthing->id());
                ParamList params;

                //modbus->readHoldingRegister(slaveAddress, ModbusRegisterX2::SoftwareVersion);
                //params.append(Param(x2luThingSofwareVersionParamTypeId, data));
                params.append(Param(x2luThingSlaveAddressParamTypeId, slaveAddress));
                descriptor.setParams(params);
                thingDescriptors.append(descriptor);
                emit autoDevicesAppeared(thingDescriptors);
                break;
            }
            case DeviceType::AerosilentBianco:
                //Just a test
                qDebug(dcDrexelUndWeiss()) << "Discovered Aerosilent Bianco";
                break;
            default:
                qDebug(dcDrexelUndWeiss()) << "Unkown Devicetype" << value;
            }
        }
    }
}

void IntegrationPluginDrexelUndWeiss::onReceivedInputRegister(int slaveAddress, int modbusRegister, int value)
{
    Q_UNUSED(slaveAddress)
    Q_UNUSED(modbusRegister)
    Q_UNUSED(value)
}

void IntegrationPluginDrexelUndWeiss::onWriteRequestFinished(QUuid requestId, bool success)
{
    DeviceActionInfo *info = m_pendingActions.take(requestId);
    if (!info)
        return;

    if (success) {
        info->finish(Thing::ThingErrorNoError);
    } else {
        info->finish(Thing::ThingErrorHardwareFailure);
    }
}

void IntegrationPluginDrexelUndWeiss::discoverModbusSlaves(ModbusRTUMaster *modbus, int slaveAddress)
{
    foreach (Thing *thing, myThings()) {
        if (thing->thingClassId() == x2luThingClassId) {
            if (thing->paramValue(x2luThingSlaveAddressParamTypeId).toInt() == slaveAddress) {
                qWarning(dcDrexelUndWeiss()) << "Device with slave address" << slaveAddress << "already added";
                return;
            }
        }
        if (thing->thingClassId() == x2wpThingClassId) {
            if (thing->paramValue(x2wpThingSlaveAddressParamTypeId).toInt() == slaveAddress) {
                qWarning(dcDrexelUndWeiss()) << "Thing with slave address" << slaveAddress << "already added";
                return;
            }
        }
    }
    modbus->readHoldingRegister(slaveAddress, ModbusRegisterX2::Geraetetyp);
}
