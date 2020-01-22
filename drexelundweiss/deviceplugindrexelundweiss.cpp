/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea.
* This project including source code and documentation is protected by copyright law, and
* remains the property of nymea GmbH. All rights, including reproduction, publication,
* editing and translation, are reserved. The use of this project is subject to the terms of a
* license agreement to be concluded with nymea GmbH in accordance with the terms
* of use of nymea GmbH, available under https://nymea.io/license
*
* GNU Lesser General Public License Usage
* Alternatively, this project may be redistributed and/or modified under the terms of the GNU
* Lesser General Public License as published by the Free Software Foundation; version 3.
* this project is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License along with this project.
* If not, see <https://www.gnu.org/licenses/>.
*
* For any further details and any questions please contact us under contact@nymea.io
* or see our FAQ/Licensing Information on https://nymea.io/license/faq
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "deviceplugindrexelundweiss.h"
#include "plugininfo.h"
#include "modbusdegisterdefinition.h"

DevicePluginDrexelUndWeiss::DevicePluginDrexelUndWeiss()
{

}

DevicePluginDrexelUndWeiss::~DevicePluginDrexelUndWeiss()
{

}

void DevicePluginDrexelUndWeiss::init()
{
    connect(this, &DevicePluginDrexelUndWeiss::configValueChanged, this, &DevicePluginDrexelUndWeiss::onPluginConfigurationChanged);
}

void DevicePluginDrexelUndWeiss::discoverDevices(DeviceDiscoveryInfo *info)
{
    // Create the list of available serial interfaces
    QList<DeviceDescriptor> deviceDescriptors;

    if (info->deviceClassId() == modbusConnectionDeviceClassId) {
        Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
            if (m_usedSerialPorts.contains(port.systemLocation())){
                //device already in use
                qCDebug(dcDrexelUndWeiss()) << "Found serial port that is already used:" << port.portName();
            } else {
                //Serial port is not yet used, create now a new one
                qCDebug(dcDrexelUndWeiss()) << "Found serial port:" << port.portName();
                QString description = port.manufacturer() + " " + port.description();
                DeviceDescriptor descriptor(info->deviceClassId(), port.portName(), description);
                ParamList parameters;
                parameters.append(Param(modbusConnectionDeviceSerialPortParamTypeId, port.systemLocation()));
                descriptor.setParams(parameters);
                info->addDeviceDescriptor(descriptor);
            }
        }
        info->finish(Device::DeviceErrorNoError);
        return;
    }

    if (info->deviceClassId() == x2luDeviceClassId) {
        info->finish(Device::DeviceErrorNoError);
        return;
    }

    if (info->deviceClassId() == x2wpDeviceClassId) {
        info->finish(Device::DeviceErrorNoError);
        return;
    }

    info->finish(Device::DeviceErrorDeviceClassNotFound);
    return;
}

void DevicePluginDrexelUndWeiss::setupDevice(DeviceSetupInfo *info)
{
    Device *device = info->device();

    if (device->deviceClassId() == modbusConnectionDeviceClassId) {

        QString serialPort = device->paramValue(modbusConnectionDeviceSerialPortParamTypeId).toString();
        int baudRate = device->paramValue(modbusConnectionDeviceBaudRateParamTypeId).toInt();

        ModbusRTUMaster *modbus = new ModbusRTUMaster(serialPort, baudRate, QSerialPort::Parity::NoParity, 8, 1, this);
        connect(modbus, &ModbusRTUMaster::connectionStateChanged, this, &DevicePluginDrexelUndWeiss::onConnectionStateChanged);
        connect(modbus, &ModbusRTUMaster::receivedCoil, this, &DevicePluginDrexelUndWeiss::onReceivedCoil);
        connect(modbus, &ModbusRTUMaster::receivedDiscreteInput, this, &DevicePluginDrexelUndWeiss::onReceivedDiscreteInput);
        connect(modbus, &ModbusRTUMaster::receivedHoldingRegister, this, &DevicePluginDrexelUndWeiss::onReceivedHoldingRegister);
        connect(modbus, &ModbusRTUMaster::receivedInputRegister, this, &DevicePluginDrexelUndWeiss::onReceivedInputRegister);


        m_modbusRTUMasters.insert(device, modbus);
        m_usedSerialPorts.append(serialPort);
        info->finish(Device::DeviceErrorNoError);
        return;
    }

    if (device->deviceClassId() == x2luDeviceClassId) {
        info->finish(Device::DeviceErrorNoError);
        return;
    }

    if (device->deviceClassId() == x2wpDeviceClassId) {
        info->finish(Device::DeviceErrorNoError);
        return;
    }
    info->finish(Device::DeviceErrorDeviceClassNotFound);
    return;
}

void DevicePluginDrexelUndWeiss::postSetupDevice(Device *device)
{
    if (!m_refreshTimer) {
        // Refresh timer for TCP read
        int refreshTime = configValue(drexelUndWeissPluginUpdateIntervalParamTypeId).toInt();
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(refreshTime);
        connect(m_refreshTimer, &PluginTimer::timeout, this, &DevicePluginDrexelUndWeiss::onRefreshTimer);
    }

    if (device->deviceClassId() == modbusConnectionDeviceClassId) {
        // read Register 5000 and emit auto-device
        ModbusRTUMaster *modbus = m_modbusRTUMasters.value(device);
        if (!modbus){
            qCWarning(dcDrexelUndWeiss()) << "No modbus master available";
        }
        device->setStateValue(modbusConnectionConnectedStateTypeId, true);
    }

    if ((device->deviceClassId() == x2luDeviceClassId) || (device->deviceClassId() == x2wpDeviceClassId)) {
        Device *parentDevice = myDevices().findById(device->parentId());
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

void DevicePluginDrexelUndWeiss::executeAction(DeviceActionInfo *info)
{
    Device *device = info->device();
    Action action = info->action();

    if (device->deviceClassId() == modbusConnectionDeviceClassId) {

        ModbusRTUMaster *modbus = m_modbusRTUMasters.value(device);
        if (!modbus){
            qCWarning(dcDrexelUndWeiss()) << "No modbus interface available";
            info->finish(Device::DeviceErrorHardwareFailure);
            return;
        }

        if (action.actionTypeId() == modbusConnectionDiscoverDevicesActionTypeId) {
            int slave = action.param(modbusConnectionDiscoverDevicesActionSlaveAddressParamTypeId).value().toInt();
            discoverModbusSlaves(modbus, slave);
            info->finish(Device::DeviceErrorNoError);
            return;
        } else {
            info->finish(Device::DeviceErrorActionTypeNotFound);
        }
    } else if (device->deviceClassId() == x2luDeviceClassId) {
        Device *parentDevice = myDevices().findById(device->parentId());
        if (!parentDevice) {
            qWarning(dcDrexelUndWeiss()) << "Could not find the parent device";
            info->finish(Device::DeviceErrorHardwareFailure);
            return;
        }
        ModbusRTUMaster *modbus = m_modbusRTUMasters.value(parentDevice);
        int slave = device->paramValue(x2luDeviceSlaveAddressParamTypeId).toInt();
        if (!modbus){
            qCWarning(dcDrexelUndWeiss()) << "No modbus interface available";
            info->finish(Device::DeviceErrorHardwareFailure);
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
            info->finish(Device::DeviceErrorActionTypeNotFound);
        }
    } else if (device->deviceClassId() == x2wpDeviceClassId) {
        Device *parentDevice = myDevices().findById(device->parentId());
        if (!parentDevice) {
            qWarning(dcDrexelUndWeiss()) << "Could not find modbus interface";
            info->finish(Device::DeviceErrorHardwareFailure);
            return;
        }
        ModbusRTUMaster *modbus = m_modbusRTUMasters.value(parentDevice);
        int slave = device->paramValue(x2wpDeviceSlaveAddressParamTypeId).toInt();
        if (!modbus){
            qCWarning(dcDrexelUndWeiss()) << "No modbus master available";
            info->finish(Device::DeviceErrorHardwareFailure);
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
            info->finish(Device::DeviceErrorActionTypeNotFound);
        }
    } else {
        info->finish(Device::DeviceErrorDeviceClassNotFound);
    }
}


void DevicePluginDrexelUndWeiss::deviceRemoved(Device *device)
{
    if (device->deviceClassId() == modbusConnectionDeviceClassId) {

        ModbusRTUMaster *modbus = m_modbusRTUMasters.take(device);
        if (!modbus){
            qCWarning(dcDrexelUndWeiss()) << "No modbus interface available";
            return;
        }
        m_usedSerialPorts.removeAll(modbus->serialPort());
        modbus->deleteLater();
    }
}

void DevicePluginDrexelUndWeiss::onRefreshTimer()
{
    foreach (Device *device, myDevices()) {

        if (device->deviceClassId() == modbusConnectionDeviceClassId) {
            ModbusRTUMaster *modbus = m_modbusRTUMasters.value(device);

            if (!modbus) {
                qCWarning(dcDrexelUndWeiss()) << "No modbus master available";
                return;
            }
        } else if (device->deviceClassId() == x2luDeviceClassId || device->deviceClassId() == x2wpDeviceClassId){
            updateStates(device);
        }
    }
}

void DevicePluginDrexelUndWeiss::updateStates(Device *device)
{
    if (device->deviceClassId() == x2luDeviceClassId) {
        Device *parent = myDevices().findById(device->parentId());
        ModbusRTUMaster *modbus = m_modbusRTUMasters.value(parent);
        int slave = device->paramValue(x2luDeviceSlaveAddressParamTypeId).toInt();

        modbus->readHoldingRegister(slave, ModbusRegisterX2::AktiveLuefterstufe);
        modbus->readHoldingRegister(slave, ModbusRegisterX2::Betriebsart); // Ventilation mode
        modbus->readHoldingRegister(slave, ModbusRegisterX2::CO2);
    }

    if (device->deviceClassId() == x2wpDeviceClassId) {
        Device *parent = myDevices().findById(device->parentId());
        ModbusRTUMaster *modbus = m_modbusRTUMasters.value(parent);
        int slave = device->paramValue(x2wpDeviceSlaveAddressParamTypeId).toInt();

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


void DevicePluginDrexelUndWeiss::onPluginConfigurationChanged(const ParamTypeId &paramTypeId, const QVariant &value)
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

void DevicePluginDrexelUndWeiss::onConnectionStateChanged(bool status)
{
    Q_UNUSED(status)
}

void DevicePluginDrexelUndWeiss::onReceivedCoil(int slaveAddress, int modbusRegister, bool value)
{
    Q_UNUSED(slaveAddress)
    Q_UNUSED(modbusRegister)
    Q_UNUSED(value)
}

void DevicePluginDrexelUndWeiss::onReceivedDiscreteInput(int slaveAddress, int modbusRegister, bool value)
{
    Q_UNUSED(slaveAddress)
    Q_UNUSED(modbusRegister)
    Q_UNUSED(value)
}

void DevicePluginDrexelUndWeiss::onReceivedHoldingRegister(int slaveAddress, int modbusRegister, int value)
{
    ModbusRTUMaster *modbus = static_cast<ModbusRTUMaster *>(sender());

    if (m_modbusRTUMasters.values().contains(modbus) ){
        Device *parentDevice = m_modbusRTUMasters.key(static_cast<ModbusRTUMaster *>(modbus));

        foreach(Device *device, myDevices().filterByParentDeviceId(parentDevice->id())) {
            if (device->deviceClassId() == x2luDeviceClassId && device->paramValue(x2luDeviceSlaveAddressParamTypeId) == slaveAddress) {
                switch (modbusRegister) {
                case ModbusRegisterX2::Waermepumpe:
                    device->setStateValue(x2wpPowerStateTypeId, value);
                    break;

                case ModbusRegisterX2::RaumSoll:
                    device->setStateValue(x2wpTargetTemperatureStateTypeId, value/1000.00);
                    break;

                case ModbusRegisterX2::Raum:
                    device->setStateValue(x2wpTemperatureStateTypeId, value/1000.00);
                    break;

                case ModbusRegisterX2::TemperaturWarmwasserspeicherUnten:
                    device->setStateValue(x2wpWaterTemperatureStateTypeId, value/1000.00);
                    break;
                case ModbusRegisterX2::BrauchwasserSolltermperatur:
                    device->setStateValue(x2wpTargetWaterTemperatureStateTypeId, value/1000.00);
                    break;
                case ModbusRegisterX2::Auszenluft:
                    device->setStateValue(x2wpOutsideAirTemperatureStateTypeId, value/1000.00);
                    break;

                case ModbusRegisterX2::Summenstoerung:
                    if (value != 0) {
                        //get actual error
                    } else {
                        device->setStateValue(x2wpErrorStateTypeId, "No Error");
                    }
                    break;

                case ModbusRegisterX2::StoerungAbluftventilator:
                    if (value != 0)
                        device->setStateValue(x2wpErrorStateTypeId, "Exhaust fan");
                    break;

                case ModbusRegisterX2::StoerungBoilerfuehlerElektroheizstab:
                    if (value != 0)
                        device->setStateValue(x2wpErrorStateTypeId, "Boiler sensor electric heating element");
                    break;

                case ModbusRegisterX2::StoerungBoilerfuehlerSolar:
                    if (value != 0)
                        device->setStateValue(x2wpErrorStateTypeId, "Boiler sensor solar");
                    break;

                case  ModbusRegisterX2::StoerungBoilerfuehlerWaermepumpe:
                    if (value != 0)
                        device->setStateValue(x2wpErrorStateTypeId, "Boiler sensor heat pump");
                    break;

                case  ModbusRegisterX2::StoerungBoileruebertemperatur:
                    if (value != 0)
                        device->setStateValue(x2wpErrorStateTypeId, "Boiler overtemperature");
                    break;

                case  ModbusRegisterX2::StoerungCO2Sensor:
                    if (value != 0)
                        device->setStateValue(x2wpErrorStateTypeId, "CO2-Sensor");
                    break;

                case  ModbusRegisterX2::StoerungDruckverlustAbluftZuGrosz:
                    if (value != 0)
                        device->setStateValue(x2wpErrorStateTypeId, "Pressure loss exhaust air too big");
                    break;

                case ModbusRegisterX2::StoerungDruckverlustZuluftZuGrosz:
                    if (value != 0)
                        device->setStateValue(x2wpErrorStateTypeId, "Pressure loss supply air too large");
                    break;

                case ModbusRegisterX2::StoerungDurchflussmengeHeizgkreis:
                    if (value != 0)
                        device->setStateValue(x2wpErrorStateTypeId, "Flow rate of heating circuit");
                    break;

                case ModbusRegisterX2::StoerungDurchflussmengeSolekreis:
                    if (value != 0)
                        device->setStateValue(x2wpErrorStateTypeId, "Flow rate brine circuit");
                    break;

                case ModbusRegisterX2::StoerungTeilnehmerNichtErreichbar:
                    if (value != 0)
                        device->setStateValue(x2wpErrorStateTypeId, "Participant not available");
                    break;

                case ModbusRegisterX2::StoerungTemperaturfuehlerAuszenluft:
                    if (value != 0)
                        device->setStateValue(x2wpErrorStateTypeId, "Temperature sensor outside air");
                    break;

                case ModbusRegisterX2::StoerungTemperaturfuehlerHeizkreisVorlauf:
                    if (value != 0)
                        device->setStateValue(x2wpErrorStateTypeId, "Temperature sensor heating circuit flow");
                    break;

                case ModbusRegisterX2::StoerungTemperaturfuehlerRaum:
                    if (value != 0)
                        device->setStateValue(x2wpErrorStateTypeId, "Temperature sensor room");
                    break;

                case ModbusRegisterX2::StoerungTemperaturfuehlerSolarkollektor:
                    if (value != 0)
                        device->setStateValue(x2wpErrorStateTypeId, "Temperature sensor solar collector");
                    break;

                case ModbusRegisterX2::StoerungTemperaturfuehlerSole:
                    if (value != 0)
                        device->setStateValue(x2wpErrorStateTypeId, "Temperature sensor brine");
                    break;

                case ModbusRegisterX2::StoerungTemperaturfuehlerSoleAuszenluft:
                    if (value != 0)
                        device->setStateValue(x2wpErrorStateTypeId, "Temperature sensor brine outside air");
                    break;

                case ModbusRegisterX2::StoerungWaermepumpeHochdruck:
                    if (value != 0)
                        device->setStateValue(x2wpErrorStateTypeId, "Heat pump high pressure");
                    break;

                case ModbusRegisterX2::StoerungWaermepumpeNiederdruck:
                    if (value != 0)
                        device->setStateValue(x2wpErrorStateTypeId, "Heat pump low pressure");
                    break;

                case ModbusRegisterX2::StoerungWertNichtZulaessig:
                    if (value != 0)
                        device->setStateValue(x2wpErrorStateTypeId, "Value not allowed");
                    break;

                case ModbusRegisterX2::StoerungZuluftventilator:
                    if (value != 0)
                        device->setStateValue(x2wpErrorStateTypeId, "Supply air fan");
                    break;

                case ModbusRegisterX2::LeistungKompressor:
                    device->setStateValue(x2wpPowerCompressorStateTypeId, value/1000.00);
                    break;
                case ModbusRegisterX2::LeistungWarmwasser:
                    device->setStateValue(x2wpPowerWaterHeatingStateTypeId, value/1000.00);
                    break;

                case ModbusRegisterX2::LeistungRaumheizung:
                    device->setStateValue(x2wpPowerRoomHeatingStateTypeId, value/1000.00);
                    break;

                case ModbusRegisterX2::LeistungLuftvorwaermung:
                    device->setStateValue(x2wpPowerAirPreheatingStateTypeId, value/1000.00);
                    break;

                case ModbusRegisterX2::EnergieKompressor:
                    device->setStateValue(x2wpEnergyCompressorStateTypeId, value/1000.00);
                    break;

                case ModbusRegisterX2::EnergieWarmwasser:
                    device->setStateValue(x2wpEnergyWaterHeatingStateTypeId, value/1000.00);
                    break;

                case ModbusRegisterX2::EnergieRaumheizung:
                    device->setStateValue(x2wpEnergyRoomHeatingStateTypeId, value/1000.00);
                    break;

                case ModbusRegisterX2::EnergieLuftvorerwarrmung:
                    device->setStateValue(x2wpEnergyAirPreheatingStateTypeId, value/1000.00);
                    break;
                default:
                    break;
                }
            } else if (device->deviceClassId() == x2wpDeviceClassId && device->paramValue(x2wpDeviceSlaveAddressParamTypeId) == slaveAddress) {

                switch (modbusRegister) {
                case ModbusRegisterX2::Betriebsart:
                    if (value == VentialtionMode::ManuellStufe0) {
                        device->setStateValue(x2luVentilationModeStateTypeId, "Manual level 0");
                    } else if (value == VentialtionMode::ManuellStufe1) {
                        device->setStateValue(x2luVentilationModeStateTypeId, "Manual level 1");
                    } else if (value == VentialtionMode::ManuellStufe2) {
                        device->setStateValue(x2luVentilationModeStateTypeId, "Manual level 2");
                    } else if (value == VentialtionMode::ManuellStufe3) {
                        device->setStateValue(x2luVentilationModeStateTypeId, "Manual level 3");
                    } else if (value == VentialtionMode::Automatikbetrieb) {
                        device->setStateValue(x2luVentilationModeStateTypeId, "Automatic");
                    } else if (value == VentialtionMode::Party) {
                        device->setStateValue(x2luVentilationModeStateTypeId, "Party");
                    }
                    break;
                case ModbusRegisterX2::AktiveLuefterstufe:
                    device->setStateValue(x2luActiveVentilationLevelStateTypeId, value);
                    break;
                case ModbusRegisterX2::CO2:
                    device->setStateValue(x2luCo2StateTypeId, value);
                    break;
                }
            }
        }

        if (modbusRegister == ModbusRegisterX2::Geraetetyp) {
            switch (value) {
            case DeviceType::X2_WP: {
                qDebug(dcDrexelUndWeiss()) << "Discovered X2 heat pump";
                QList<DeviceDescriptor> deviceDescriptors;
                DeviceDescriptor descriptor(x2wpDeviceClassId, "X2 WP", "Drexel und Weiss", parentDevice->id());
                ParamList params;

                //modbus->readHoldingRegister(slaveAddress, ModbusRegisterX2::SoftwareVersion);
                //params.append(Param(x2wpDeviceSofwareVersionParamTypeId, data));
                params.append(Param(x2wpDeviceSlaveAddressParamTypeId, slaveAddress));
                descriptor.setParams(params);
                deviceDescriptors.append(descriptor);
                emit autoDevicesAppeared(deviceDescriptors);
                break;
            }
            case DeviceType::X2_LU: {
                qDebug(dcDrexelUndWeiss()) << "Discovered X2 ventilation unit";
                QList<DeviceDescriptor> deviceDescriptors;
                DeviceDescriptor descriptor(x2luDeviceClassId, "X2 LU", "Drexel und Weiss", parentDevice->id());
                ParamList params;

                //modbus->readHoldingRegister(slaveAddress, ModbusRegisterX2::SoftwareVersion);
                //params.append(Param(x2luDeviceSofwareVersionParamTypeId, data));
                params.append(Param(x2luDeviceSlaveAddressParamTypeId, slaveAddress));
                descriptor.setParams(params);
                deviceDescriptors.append(descriptor);
                emit autoDevicesAppeared(deviceDescriptors);
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

void DevicePluginDrexelUndWeiss::onReceivedInputRegister(int slaveAddress, int modbusRegister, int value)
{
    Q_UNUSED(slaveAddress)
    Q_UNUSED(modbusRegister)
    Q_UNUSED(value)
}

void DevicePluginDrexelUndWeiss::onWriteRequestFinished(QUuid requestId, bool success)
{
    DeviceActionInfo *info = m_pendingActions.take(requestId);
    if (!info)
        return;

    if (success) {
        info->finish(Device::DeviceErrorNoError);
    } else {
        info->finish(Device::DeviceErrorHardwareFailure);
    }
}

void DevicePluginDrexelUndWeiss::discoverModbusSlaves(ModbusRTUMaster *modbus, int slaveAddress)
{
    foreach (Device *device, myDevices()) {
        if (device->deviceClassId() == x2luDeviceClassId) {
            if (device->paramValue(x2luDeviceSlaveAddressParamTypeId).toInt() == slaveAddress) {
                qWarning(dcDrexelUndWeiss()) << "Device with slave address" << slaveAddress << "already added";
                return;
            }
        }
        if (device->deviceClassId() == x2wpDeviceClassId) {
            if (device->paramValue(x2wpDeviceSlaveAddressParamTypeId).toInt() == slaveAddress) {
                qWarning(dcDrexelUndWeiss()) << "Device with slave address" << slaveAddress << "already added";
                return;
            }
        }
    }
    modbus->readHoldingRegister(slaveAddress, ModbusRegisterX2::Geraetetyp);
}
