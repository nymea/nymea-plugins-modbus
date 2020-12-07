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

#include "integrationpluginmodbuscommander.h"
#include "plugininfo.h"

#include <QSerialPort>

IntegrationPluginModbusCommander::IntegrationPluginModbusCommander()
{
}

void IntegrationPluginModbusCommander::init()
{
    connect(this, &IntegrationPluginModbusCommander::configValueChanged, this, &IntegrationPluginModbusCommander::onPluginConfigurationChanged);

    m_slaveAddressParamTypeId.insert(coilThingClassId, coilThingSlaveAddressParamTypeId);
    m_slaveAddressParamTypeId.insert(inputRegisterThingClassId, inputRegisterThingSlaveAddressParamTypeId);
    m_slaveAddressParamTypeId.insert(discreteInputThingClassId, discreteInputThingSlaveAddressParamTypeId);
    m_slaveAddressParamTypeId.insert(holdingRegisterThingClassId, holdingRegisterThingSlaveAddressParamTypeId);

    m_registerAddressParamTypeId.insert(coilThingClassId, coilThingRegisterAddressParamTypeId);
    m_registerAddressParamTypeId.insert(inputRegisterThingClassId, inputRegisterThingRegisterAddressParamTypeId);
    m_registerAddressParamTypeId.insert(discreteInputThingClassId, discreteInputThingRegisterAddressParamTypeId);
    m_registerAddressParamTypeId.insert(holdingRegisterThingClassId, holdingRegisterThingRegisterAddressParamTypeId);

    m_connectedStateTypeId.insert(modbusRTUClientThingClassId, modbusRTUClientConnectedStateTypeId);
    m_connectedStateTypeId.insert(modbusTCPClientThingClassId, modbusTCPClientConnectedStateTypeId);
    m_connectedStateTypeId.insert(coilThingClassId, coilConnectedStateTypeId);
    m_connectedStateTypeId.insert(inputRegisterThingClassId, inputRegisterConnectedStateTypeId);
    m_connectedStateTypeId.insert(discreteInputThingClassId, discreteInputConnectedStateTypeId);
    m_connectedStateTypeId.insert(holdingRegisterThingClassId, holdingRegisterConnectedStateTypeId);

    m_valueStateTypeId.insert(coilThingClassId, coilValueStateTypeId);
    m_valueStateTypeId.insert(inputRegisterThingClassId, inputRegisterValueStateTypeId);
    m_valueStateTypeId.insert(discreteInputThingClassId, discreteInputValueStateTypeId);
    m_valueStateTypeId.insert(holdingRegisterThingClassId, holdingRegisterValueStateTypeId);
}

void IntegrationPluginModbusCommander::discoverThings(ThingDiscoveryInfo *info)
{
    ThingClassId thingClassId = info->thingClassId();

    if (thingClassId == modbusRTUClientThingClassId) {
        Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
            qCDebug(dcModbusCommander()) << "Found serial port:" << port.systemLocation() << "manufacturer" << port.manufacturer() << "description" << port.description() << "serial number" << port.serialNumber();
            if (port.isBusy()) {
                qCDebug(dcModbusCommander()) << "Serial port ist busy, skipping.";
                continue;
            }
            QString manufacturer = port.manufacturer();
            if (manufacturer.isEmpty()) {
                manufacturer = "unknown";
            }
            QString description = port.description()+" Manufacturer: "+port.manufacturer();
            ThingDescriptor thingDescriptor(thingClassId, "Modbus RTU interface", description);
            ParamList parameters;
            QString serialPort = port.systemLocation();
            QString serialnumber = port.serialNumber();
            if (serialnumber.isEmpty()) {
                serialnumber = port.manufacturer()+QString::number(port.productIdentifier(), 16);

            }
            qCDebug(dcModbusCommander()) << "    - Serial number" << serialnumber;
            Q_FOREACH (Thing *exisingThing, myThings().filterByParam(modbusRTUClientThingClassId)) {
                thingDescriptor.setThingId(exisingThing->id());
                // Rediscovery is broken because of an missing unique device id
                // This is a workaround and doesnt work if multiple uart converters are attached.
                // ThingDiscoveryInfo may be extended to distinquish between discovery and rediscovery
                break;
            }
            parameters.append(Param(modbusRTUClientThingSerialPortParamTypeId, serialPort));
            parameters.append(Param(modbusRTUClientThingSerialnumberParamTypeId, serialnumber));
            thingDescriptor.setParams(parameters);
            info->addThingDescriptor(thingDescriptor);
        }

        //FIXME missing info if it is a rediscovery
        info->finish(Thing::ThingErrorNoError);
        return;
    } else if (thingClassId == discreteInputThingClassId) {
        Q_FOREACH(Thing *clientThing, myThings()){
            if (clientThing->thingClassId() == modbusTCPClientThingClassId) {
                ThingDescriptor descriptor(thingClassId, "Discrete input", clientThing->name() + " " + clientThing->paramValue(modbusTCPClientThingIpAddressParamTypeId).toString() + " Port: " + clientThing->paramValue(modbusTCPClientThingPortParamTypeId).toString());
                descriptor.setParentId(clientThing->id());
                info->addThingDescriptor(descriptor);
            }
            if (clientThing->thingClassId() == modbusRTUClientThingClassId) {
                ThingDescriptor descriptor(thingClassId, "Discrete input", clientThing->name() + " " + clientThing->paramValue(modbusRTUClientThingSerialPortParamTypeId).toString());
                descriptor.setParentId(clientThing->id());
                info->addThingDescriptor(descriptor);
            }
        }
        info->finish(Thing::ThingErrorNoError);
        return;

    } else if (thingClassId == coilThingClassId) {
        Q_FOREACH(Thing *clientThing, myThings()){
            if (clientThing->thingClassId() == modbusTCPClientThingClassId) {
                ThingDescriptor descriptor(thingClassId, "Coil", clientThing->name() + " " + clientThing->paramValue(modbusTCPClientThingIpAddressParamTypeId).toString() + " Port: " + clientThing->paramValue(modbusTCPClientThingPortParamTypeId).toString());
                descriptor.setParentId(clientThing->id());
                info->addThingDescriptor(descriptor);
            }
            if (clientThing->thingClassId() == modbusRTUClientThingClassId) {
                ThingDescriptor descriptor(thingClassId, "Coil", clientThing->name() + " " + clientThing->paramValue(modbusRTUClientThingSerialPortParamTypeId).toString());
                descriptor.setParentId(clientThing->id());
                info->addThingDescriptor(descriptor);
            }
        }
        info->finish(Thing::ThingErrorNoError);
        return;
    } else if (thingClassId == holdingRegisterThingClassId) {
        Q_FOREACH(Thing *clientThing, myThings()){
            if (clientThing->thingClassId() == modbusTCPClientThingClassId) {
                ThingDescriptor descriptor(thingClassId, "Holding register", clientThing->name() + " " + clientThing->paramValue(modbusTCPClientThingIpAddressParamTypeId).toString() + " Port: " + clientThing->paramValue(modbusTCPClientThingPortParamTypeId).toString());
                descriptor.setParentId(clientThing->id());
                info->addThingDescriptor(descriptor);
            }
            if (clientThing->thingClassId() == modbusRTUClientThingClassId) {
                ThingDescriptor descriptor(thingClassId, "Holding register", clientThing->name() + " " + clientThing->paramValue(modbusRTUClientThingSerialPortParamTypeId).toString());
                descriptor.setParentId(clientThing->id());
                info->addThingDescriptor(descriptor);
            }
        }
        info->finish(Thing::ThingErrorNoError);
        return;

    } else if (thingClassId == inputRegisterThingClassId) {
        Q_FOREACH(Thing *clientThing, myThings()){
            if (clientThing->thingClassId() == modbusTCPClientThingClassId) {
                ThingDescriptor descriptor(thingClassId, "Input register", clientThing->name() + " " + clientThing->paramValue(modbusTCPClientThingIpAddressParamTypeId).toString() + " Port: " + clientThing->paramValue(modbusTCPClientThingPortParamTypeId).toString());
                descriptor.setParentId(clientThing->id());
                info->addThingDescriptor(descriptor);
            }
            if (clientThing->thingClassId() == modbusRTUClientThingClassId) {
                ThingDescriptor descriptor(thingClassId, "Input register", clientThing->name() + " " + clientThing->paramValue(modbusRTUClientThingSerialPortParamTypeId).toString());
                descriptor.setParentId(clientThing->id());
                info->addThingDescriptor(descriptor);
            }
        }
        info->finish(Thing::ThingErrorNoError);
        return;
    } else {
        Q_ASSERT_X(false, "discoverThings", QString("Unhandled thingClassId: %1").arg(info->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginModbusCommander::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    if (thing->thingClassId() == modbusTCPClientThingClassId) {

        QHostAddress hostAddress = QHostAddress(thing->paramValue(modbusTCPClientThingIpAddressParamTypeId).toString());
        uint port = thing->paramValue(modbusTCPClientThingPortParamTypeId).toUInt();
        uint numberOfRetries = thing->setting(modbusTCPClientSettingsNumberOfRetriesParamTypeId).toUInt();
        uint timeout = thing->setting(modbusTCPClientSettingsTimeoutParamTypeId).toUInt();

        foreach (ModbusTCPMaster *modbusTCPMaster, m_modbusTCPMasters.values()) {
            if ((modbusTCPMaster->hostAddress() == hostAddress) && (modbusTCPMaster->port() == port)){
                m_modbusTCPMasters.insert(thing, modbusTCPMaster);
                return info->finish(Thing::ThingErrorNoError);
            }
        }

        qCDebug(dcModbusCommander()) << "Setting up TCP client" << thing->name();
        qCDebug(dcModbusCommander()) << "      address:" << hostAddress.toString();
        qCDebug(dcModbusCommander()) << "      port:" << port;
        qCDebug(dcModbusCommander()) << "      number of retries:" << numberOfRetries;
        qCDebug(dcModbusCommander()) << "      timeout:" << timeout;

        ModbusTCPMaster *modbusTCPMaster = new ModbusTCPMaster(hostAddress, port, this);
        connect(modbusTCPMaster, &ModbusTCPMaster::connectionStateChanged, this, &IntegrationPluginModbusCommander::onConnectionStateChanged);
        connect(modbusTCPMaster, &ModbusTCPMaster::writeRequestExecuted, this, &IntegrationPluginModbusCommander::onRequestExecuted);
        connect(modbusTCPMaster, &ModbusTCPMaster::writeRequestError, this, &IntegrationPluginModbusCommander::onRequestError);
        connect(modbusTCPMaster, &ModbusTCPMaster::receivedCoil, this, &IntegrationPluginModbusCommander::onReceivedCoil);
        connect(modbusTCPMaster, &ModbusTCPMaster::receivedDiscreteInput, this, &IntegrationPluginModbusCommander::onReceivedDiscreteInput);
        connect(modbusTCPMaster, &ModbusTCPMaster::receivedHoldingRegister, this, &IntegrationPluginModbusCommander::onReceivedHoldingRegister);
        connect(modbusTCPMaster, &ModbusTCPMaster::receivedInputRegister, this, &IntegrationPluginModbusCommander::onReceivedInputRegister);
        connect(modbusTCPMaster, &ModbusTCPMaster::connectionStateChanged, info, [info, modbusTCPMaster, this] (bool connected) {
            if (connected) {
                info->finish(Thing::ThingErrorNoError);
                m_modbusTCPMasters.insert(info->thing(), modbusTCPMaster);
            }
        });
        connect(thing, &Thing::settingChanged, thing, [thing, modbusTCPMaster] (const ParamTypeId &paramTypeId, const QVariant &value) {
             if (paramTypeId == modbusTCPClientSettingsNumberOfRetriesParamTypeId) {
                 qCDebug(dcModbusCommander()) << "Set number of retries" << thing->name() << value.toUInt();
                 modbusTCPMaster->setNumberOfRetries(value.toUInt());
             } else if (paramTypeId == modbusTCPClientSettingsTimeoutParamTypeId) {
                 qCDebug(dcModbusCommander()) << "Set timeout " << thing->name() << value.toUInt();
                 modbusTCPMaster->setTimeout(value.toUInt());
             }
         });
        modbusTCPMaster->connectDevice();

    } else if (thing->thingClassId() == modbusRTUClientThingClassId) {

        QString serialPort = thing->paramValue(modbusRTUClientThingSerialPortParamTypeId).toString();
        uint baudrate = thing->paramValue(modbusRTUClientThingBaudRateParamTypeId).toUInt();
        uint stopBits = thing->paramValue(modbusRTUClientThingStopBitsParamTypeId).toUInt();
        uint dataBits = thing->paramValue(modbusRTUClientThingDataBitsParamTypeId).toUInt();
        uint numberOfRetries = thing->setting(modbusRTUClientSettingsNumberOfRetriesParamTypeId).toUInt();
        uint timeout = thing->setting(modbusRTUClientSettingsTimeoutParamTypeId).toUInt();
        QSerialPort::Parity parity = QSerialPort::Parity::NoParity;
        QString parityString = thing->paramValue(modbusRTUClientThingParityParamTypeId).toString();
        if (parityString.contains("No")) {
            parity = QSerialPort::Parity::NoParity;
        } else if (parityString.contains("Even")) {
            parity = QSerialPort::Parity::EvenParity;
        } else if (parityString.contains("Odd")) {
            parity = QSerialPort::Parity::OddParity;
        }
        qCDebug(dcModbusCommander()) << "Setting up RTU client" << thing->name();
        qCDebug(dcModbusCommander()) << "      baud:" << baudrate;
        qCDebug(dcModbusCommander()) << "      stop bits:" << stopBits;
        qCDebug(dcModbusCommander()) << "      data bits:" << dataBits;
        qCDebug(dcModbusCommander()) << "      parity:" << parityString;
        qCDebug(dcModbusCommander()) << "      number of retries:" << numberOfRetries;
        qCDebug(dcModbusCommander()) << "      timeout:" << timeout;

        ModbusRTUMaster *modbusRTUMaster = new ModbusRTUMaster(serialPort, baudrate, parity, dataBits, stopBits, this);
        modbusRTUMaster->setTimeout(timeout);
        modbusRTUMaster->setNumberOfRetries(numberOfRetries);
        connect(modbusRTUMaster, &ModbusRTUMaster::connectionStateChanged, this, &IntegrationPluginModbusCommander::onConnectionStateChanged);
        connect(modbusRTUMaster, &ModbusRTUMaster::requestExecuted, this, &IntegrationPluginModbusCommander::onRequestExecuted);
        connect(modbusRTUMaster, &ModbusRTUMaster::requestError, this, &IntegrationPluginModbusCommander::onRequestError);
        connect(modbusRTUMaster, &ModbusRTUMaster::receivedCoil, this, &IntegrationPluginModbusCommander::onReceivedCoil);
        connect(modbusRTUMaster, &ModbusRTUMaster::receivedDiscreteInput, this, &IntegrationPluginModbusCommander::onReceivedDiscreteInput);
        connect(modbusRTUMaster, &ModbusRTUMaster::receivedHoldingRegister, this, &IntegrationPluginModbusCommander::onReceivedHoldingRegister);
        connect(modbusRTUMaster, &ModbusRTUMaster::receivedInputRegister, this, &IntegrationPluginModbusCommander::onReceivedInputRegister);
        connect(modbusRTUMaster, &ModbusRTUMaster::connectionStateChanged, info, [info, modbusRTUMaster, this] (bool connected) {
            if (connected) {
                info->finish(Thing::ThingErrorNoError);
                m_modbusRTUMasters.insert(info->thing(), modbusRTUMaster);
            }
        });
        connect(thing, &Thing::settingChanged, thing, [thing, modbusRTUMaster] (const ParamTypeId &paramTypeId, const QVariant &value) {
             if (paramTypeId == modbusRTUClientSettingsNumberOfRetriesParamTypeId) {
                 qCDebug(dcModbusCommander()) << "Set number of retries" << thing->name() << value.toUInt();
                 modbusRTUMaster->setNumberOfRetries(value.toUInt());
             } else if (paramTypeId == modbusRTUClientSettingsTimeoutParamTypeId) {
                 qCDebug(dcModbusCommander()) << "Set timeout " << thing->name() << value.toUInt();
                 modbusRTUMaster->setTimeout(value.toUInt());
             }
         });
        modbusRTUMaster->connectDevice();

    } else if ((thing->thingClassId() == coilThingClassId)
               || (thing->thingClassId() == discreteInputThingClassId)
               || (thing->thingClassId() == holdingRegisterThingClassId)
               || (thing->thingClassId() == inputRegisterThingClassId)) {
        qCDebug(dcModbusCommander()) << "Setting up modbus register" << thing->name();
        info->finish(Thing::ThingErrorNoError);

    } else {
        Q_ASSERT_X(false, "setupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}


void IntegrationPluginModbusCommander::postSetupThing(Thing *thing)
{
    qCDebug(dcModbusCommander()) << "Post setup thing" << thing->name();
    if (!m_refreshTimer) {
        int refreshTime = configValue(modbusCommanderPluginUpdateIntervalParamTypeId).toInt();
        qCDebug(dcModbusCommander()) << "Starting refresh timer with interval" << refreshTime << "s";
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(refreshTime);
        connect(m_refreshTimer, &PluginTimer::timeout, this, [this] {
            foreach (Thing *thing, myThings()) {
                if ((thing->thingClassId() == coilThingClassId) ||
                        (thing->thingClassId() == discreteInputThingClassId) ||
                        (thing->thingClassId() == holdingRegisterThingClassId) ||
                        (thing->thingClassId() == inputRegisterThingClassId)) {
                    readRegister(thing);
                }
            }
        });
    }

    if ((thing->thingClassId() == modbusRTUClientThingClassId) ||
            (thing->thingClassId() == modbusTCPClientThingClassId)) {
        thing->setStateValue(m_connectedStateTypeId.value(thing->thingClassId()), true);
    } else if ((thing->thingClassId() == coilThingClassId) ||
               (thing->thingClassId() == discreteInputThingClassId) ||
               (thing->thingClassId() == holdingRegisterThingClassId) ||
               (thing->thingClassId() == inputRegisterThingClassId)) {
        readRegister(thing);
    } else {
        Q_ASSERT_X(false, "postSetupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}


void IntegrationPluginModbusCommander::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    if (thing->thingClassId() == coilThingClassId) {

        if (action.actionTypeId() == coilValueActionTypeId) {
            writeRegister(thing, info);
            return;
        } else {
            Q_ASSERT_X(false, "Execute action", QString("Unhandled action type id: %1").arg(action.actionTypeId().toString()).toUtf8());
        }
    } else if (thing->thingClassId() == holdingRegisterThingClassId) {

        if (action.actionTypeId() == holdingRegisterValueActionTypeId) {
            writeRegister(thing, info);
            return;
        } else {
            Q_ASSERT_X(false, "Execute action", QString("Unhandled action type id: %1").arg(action.actionTypeId().toString()).toUtf8());
        }
    } else {
        Q_ASSERT_X(false, "Execute action", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}


void IntegrationPluginModbusCommander::thingRemoved(Thing *thing)
{
    qCDebug(dcModbusCommander()) << "Removing thing" << thing->name();
    if (thing->thingClassId() == modbusTCPClientThingClassId) {
        ModbusTCPMaster *modbus = m_modbusTCPMasters.take(thing);
        modbus->deleteLater();
    } else if (thing->thingClassId() == modbusRTUClientThingClassId) {
        ModbusRTUMaster *modbus = m_modbusRTUMasters.take(thing);
        modbus->deleteLater();
    }

    if (myThings().empty()) {
        qCDebug(dcModbusCommander()) << "No more Modbus commander things, stopping timer";
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_refreshTimer);
        m_refreshTimer = nullptr;
    }
}

void IntegrationPluginModbusCommander::onPluginConfigurationChanged(const ParamTypeId &paramTypeId, const QVariant &value)
{
    // Check refresh schedule
    if (paramTypeId == modbusCommanderPluginUpdateIntervalParamTypeId) {;
        qCDebug(dcModbusCommander()) << "Update interval has changed to" << value.toUInt() << "s";
        if (m_refreshTimer) {
            uint refreshTime = value.toUInt();
            m_refreshTimer->stop();
            m_refreshTimer->startTimer(refreshTime);
        } else {
            qCWarning(dcModbusCommander()) << "Update interval changed but refresh timer is not initialized";
        }
    }
}

void IntegrationPluginModbusCommander::onConnectionStateChanged(bool status)
{
    auto modbus = sender();

    if (m_modbusRTUMasters.values().contains(static_cast<ModbusRTUMaster *>(modbus))) {
        Thing *thing = m_modbusRTUMasters.key(static_cast<ModbusRTUMaster *>(modbus));
        qCDebug(dcModbusCommander()) << "Connections state changed" << thing->name() << status;
        thing->setStateValue(modbusRTUClientConnectedStateTypeId, status);
    } else if (m_modbusTCPMasters.values().contains(static_cast<ModbusTCPMaster *>(modbus))) {
        Thing *thing = m_modbusTCPMasters.key(static_cast<ModbusTCPMaster *>(modbus));
        qCDebug(dcModbusCommander()) << "Connections state changed" << thing->name() << status;
        thing->setStateValue(modbusTCPClientConnectedStateTypeId, status);
    }
}

void IntegrationPluginModbusCommander::onRequestExecuted(QUuid requestId, bool success)
{
    if (m_asyncActions.contains(requestId)){
        ThingActionInfo *info = m_asyncActions.take(requestId);
        if (success){
            info->finish(Thing::ThingErrorNoError);
        } else {
            info->finish(Thing::ThingErrorHardwareNotAvailable);
        }
        info->thing()->setStateValue(m_connectedStateTypeId.value(info->thing()->thingClassId()), success);
    }

    if (m_readRequests.contains(requestId)){
        Thing *thing = m_readRequests.take(requestId);
        thing->setStateValue(m_connectedStateTypeId.value(thing->thingClassId()), success);
    }
}

void IntegrationPluginModbusCommander::onRequestError(QUuid requestId, const QString &error)
{
    if (m_asyncActions.contains(requestId)){
        ThingActionInfo *info = m_asyncActions.take(requestId);
        info->finish(Thing::ThingErrorHardwareNotAvailable, error);
        info->thing()->setStateValue(m_connectedStateTypeId.value(info->thing()->thingClassId()), false);
    }

    if (m_readRequests.contains(requestId)){
        Thing *thing = m_readRequests.take(requestId);
        thing->setStateValue(m_connectedStateTypeId.value(thing->thingClassId()), false);
    }
}

void IntegrationPluginModbusCommander::onReceivedCoil(quint32 slaveAddress, quint32 modbusRegister, const QVector<quint16> &values)
{
    auto modbus = sender();

    if (m_modbusRTUMasters.values().contains(static_cast<ModbusRTUMaster *>(modbus))) {
        Thing *parent = m_modbusRTUMasters.key(static_cast<ModbusRTUMaster *>(modbus));
        foreach (Thing *thing, myThings().filterByParentId(parent->id())) {
            if (thing->thingClassId() == coilThingClassId) {
                if ((thing->paramValue(m_slaveAddressParamTypeId.value(thing->thingClassId())) == slaveAddress)
                        && (thing->paramValue(m_registerAddressParamTypeId.value(thing->thingClassId())) == modbusRegister)) {
                    thing->setStateValue(m_valueStateTypeId.value(thing->thingClassId()), values[0]);
                    thing->setStateValue(m_connectedStateTypeId.value(thing->thingClassId()), true);
                    return;
                }
            }
        }
    } else if (m_modbusTCPMasters.values().contains(static_cast<ModbusTCPMaster *>(modbus))) {
        Thing *parent = m_modbusTCPMasters.key(static_cast<ModbusTCPMaster *>(modbus));
        foreach (Thing *thing, myThings().filterByParentId(parent->id())) {
            if (thing->thingClassId() == coilThingClassId) {
                if ((thing->paramValue(m_slaveAddressParamTypeId.value(thing->thingClassId())) == slaveAddress)
                        && (thing->paramValue(m_registerAddressParamTypeId.value(thing->thingClassId())) == modbusRegister)) {
                    thing->setStateValue(m_valueStateTypeId.value(thing->thingClassId()), values[0]);
                    thing->setStateValue(m_connectedStateTypeId.value(thing->thingClassId()), true);
                    return;
                }
            }
        }
    }
}

void IntegrationPluginModbusCommander::onReceivedDiscreteInput(quint32 slaveAddress, quint32 modbusRegister, const QVector<quint16> &values)
{
    auto modbus = sender();

    if (m_modbusRTUMasters.values().contains(static_cast<ModbusRTUMaster *>(modbus))) {
        Thing *parent = m_modbusRTUMasters.key(static_cast<ModbusRTUMaster *>(modbus));
        foreach (Thing *thing, myThings().filterByParentId(parent->id())) {
            if (thing->thingClassId() == discreteInputThingClassId) {
                if ((thing->paramValue(m_slaveAddressParamTypeId.value(thing->thingClassId())) == slaveAddress)
                        && (thing->paramValue(m_registerAddressParamTypeId.value(thing->thingClassId())) == modbusRegister)) {
                    thing->setStateValue(m_valueStateTypeId.value(thing->thingClassId()), values[0]);
                    thing->setStateValue(m_connectedStateTypeId.value(thing->thingClassId()), true);
                    return;
                }
            }
        }
    } else if (m_modbusTCPMasters.values().contains(static_cast<ModbusTCPMaster *>(modbus))) {
        Thing *parent = m_modbusTCPMasters.key(static_cast<ModbusTCPMaster *>(modbus));
        foreach (Thing *thing, myThings().filterByParentId(parent->id())) {
            if (thing->thingClassId() == discreteInputThingClassId) {
                if ((thing->paramValue(m_slaveAddressParamTypeId.value(thing->thingClassId())) == slaveAddress)
                        && (thing->paramValue(m_registerAddressParamTypeId.value(thing->thingClassId())) == modbusRegister)) {
                    thing->setStateValue(m_valueStateTypeId.value(thing->thingClassId()), values[0]);
                    thing->setStateValue(m_connectedStateTypeId.value(thing->thingClassId()), true);
                    return;
                }
            }
        }
    }
}

void IntegrationPluginModbusCommander::onReceivedHoldingRegister(uint slaveAddress, uint modbusRegister, const QVector<quint16> &values)
{
    auto modbus = sender();

    if (m_modbusRTUMasters.values().contains(static_cast<ModbusRTUMaster *>(modbus))) {
        Thing *parent = m_modbusRTUMasters.key(static_cast<ModbusRTUMaster *>(modbus));
        foreach (Thing *thing, myThings().filterByParentId(parent->id())) {
            if (thing->thingClassId() == holdingRegisterThingClassId) {
                if ((thing->paramValue(m_slaveAddressParamTypeId.value(thing->thingClassId())) == slaveAddress)
                        && (thing->paramValue(m_registerAddressParamTypeId.value(thing->thingClassId())) == modbusRegister)) {
                    thing->setStateValue(m_valueStateTypeId.value(thing->thingClassId()), values[0]);
                    thing->setStateValue(m_connectedStateTypeId.value(thing->thingClassId()), true);
                    return;
                }
            }
        }
    } else if (m_modbusTCPMasters.values().contains(static_cast<ModbusTCPMaster *>(modbus))) {
        Thing *parent = m_modbusTCPMasters.key(static_cast<ModbusTCPMaster *>(modbus));
        foreach (Thing *thing, myThings().filterByParentId(parent->id())) {
            if (thing->thingClassId() == holdingRegisterThingClassId) {
                if ((thing->paramValue(m_slaveAddressParamTypeId.value(thing->thingClassId())) == slaveAddress)
                        && (thing->paramValue(m_registerAddressParamTypeId.value(thing->thingClassId())) == modbusRegister)) {
                    thing->setStateValue(m_valueStateTypeId.value(thing->thingClassId()), values[0]);
                    thing->setStateValue(m_connectedStateTypeId.value(thing->thingClassId()), true);
                    return;
                }
            }
        }
    }
}

void IntegrationPluginModbusCommander::onReceivedInputRegister(uint slaveAddress, uint modbusRegister, const QVector<quint16> &values)
{
    auto modbus = sender();

    if (m_modbusRTUMasters.values().contains(static_cast<ModbusRTUMaster *>(modbus))) {
        Thing *parent = m_modbusRTUMasters.key(static_cast<ModbusRTUMaster *>(modbus));
        foreach (Thing *thing, myThings().filterByParentId(parent->id())) {
            if (thing->thingClassId() == inputRegisterThingClassId) {
                if ((thing->paramValue(m_slaveAddressParamTypeId.value(thing->thingClassId())) == slaveAddress)
                        && (thing->paramValue(m_registerAddressParamTypeId.value(thing->thingClassId())) == modbusRegister)) {
                    thing->setStateValue(m_valueStateTypeId.value(thing->thingClassId()), values[0]);
                    thing->setStateValue(m_connectedStateTypeId.value(thing->thingClassId()), true);
                    return;
                }
            }
        }
    } else if (m_modbusTCPMasters.values().contains(static_cast<ModbusTCPMaster *>(modbus))) {
        Thing *parent = m_modbusTCPMasters.key(static_cast<ModbusTCPMaster *>(modbus));
        foreach (Thing *thing, myThings().filterByParentId(parent->id())) {
            if (thing->thingClassId() == inputRegisterThingClassId) {
                if ((thing->paramValue(m_slaveAddressParamTypeId.value(thing->thingClassId())) == slaveAddress)
                        && (thing->paramValue(m_registerAddressParamTypeId.value(thing->thingClassId())) == modbusRegister)) {
                    thing->setStateValue(m_valueStateTypeId.value(thing->thingClassId()), values[0]);
                    thing->setStateValue(m_connectedStateTypeId.value(thing->thingClassId()), true);
                    return;
                }
            }
        }
    }
}

void IntegrationPluginModbusCommander::readRegister(Thing *thing)
{
    Thing *parent = myThings().findById(thing->parentId());
    if (!parent) {
        qCWarning(dcModbusCommander()) << "Could not find parent device" << thing->name();
        return;
    }

    uint registerAddress = thing->paramValue(m_registerAddressParamTypeId.value(thing->thingClassId())).toUInt();;
    uint slaveAddress = thing->paramValue(m_slaveAddressParamTypeId.value(thing->thingClassId())).toUInt();

    QUuid requestId;

    if (parent->thingClassId() == modbusTCPClientThingClassId) {
        ModbusTCPMaster *modbus = m_modbusTCPMasters.value(parent);
        if (!modbus)
            return;

        if (!modbus->connected())
            return; // Send requests only if the modbus interface is connected

        if (thing->thingClassId() == coilThingClassId) {
            requestId = modbus->readCoil(slaveAddress, registerAddress);
        } else if (thing->thingClassId() == discreteInputThingClassId) {
            requestId = modbus->readDiscreteInput(slaveAddress, registerAddress);
        } else if (thing->thingClassId() == holdingRegisterThingClassId) {
            requestId = modbus->readHoldingRegister(slaveAddress, registerAddress);
        } else if (thing->thingClassId() == inputRegisterThingClassId) {
            requestId = modbus->readInputRegister(slaveAddress, registerAddress);
        }
    } else if (parent->thingClassId() == modbusRTUClientThingClassId) {

        ModbusRTUMaster *modbus = m_modbusRTUMasters.value(parent);
        if (!modbus)
            return;

        if (!modbus->connected())
            return; // Send requests only if the modbus interface is connected

        if (thing->thingClassId() == coilThingClassId) {
            requestId = modbus->readCoil(slaveAddress, registerAddress);
        } else if (thing->thingClassId() == discreteInputThingClassId) {
            requestId = modbus->readDiscreteInput(slaveAddress, registerAddress);
        } else if (thing->thingClassId() == holdingRegisterThingClassId) {
            requestId = modbus->readHoldingRegister(slaveAddress, registerAddress);
        } else if (thing->thingClassId() == inputRegisterThingClassId) {
            requestId = modbus->readInputRegister(slaveAddress, registerAddress);
        }
    }
    if (!requestId.isNull()) {
        m_readRequests.insert(requestId, thing);
        QTimer::singleShot(5000, this, [requestId, this] {m_readRequests.remove(requestId);});
    } else {
        // Request returned without an id
        thing->setStateValue(m_connectedStateTypeId.value(thing->thingClassId()), false);
    }
}

void IntegrationPluginModbusCommander::writeRegister(Thing *thing, ThingActionInfo *info)
{
    Thing *parent = myThings().findById(thing->parentId());
    if (!parent) {
        qCWarning(dcModbusCommander()) << "Could not find parent device" << thing->name();
        return;
    }
    uint registerAddress = thing->paramValue(m_registerAddressParamTypeId.value(thing->thingClassId())).toUInt();;
    uint slaveAddress = thing->paramValue(m_slaveAddressParamTypeId.value(thing->thingClassId())).toUInt();

    QUuid requestId;
    Action action = info->action();

    if (parent->thingClassId() == modbusTCPClientThingClassId) {
        ModbusTCPMaster *modbus = m_modbusTCPMasters.value(parent);
        if (!modbus)
            return;

        if (thing->thingClassId() == coilThingClassId) {
            requestId = modbus->writeCoil(slaveAddress, registerAddress, action.param(coilValueActionValueParamTypeId).value().toBool());
        } else if (thing->thingClassId() == holdingRegisterThingClassId) {
            requestId = modbus->writeHoldingRegister(slaveAddress, registerAddress, action.param(holdingRegisterValueActionValueParamTypeId).value().toUInt());
        }

    } else if (parent->thingClassId() == modbusRTUClientThingClassId) {
        ModbusRTUMaster *modbus = m_modbusRTUMasters.value(parent);
        if (!modbus)
            return;

        if (thing->thingClassId() == coilThingClassId) {
            requestId = modbus->writeCoil(slaveAddress, registerAddress, action.param(coilValueActionValueParamTypeId).value().toBool());
        } else if (thing->thingClassId() == holdingRegisterThingClassId) {
            requestId = modbus->writeHoldingRegister(slaveAddress, registerAddress, action.param(holdingRegisterValueActionValueParamTypeId).value().toUInt());
        }
    }

    if (requestId.toString().isNull()){
        info->finish(Thing::ThingErrorHardwareNotAvailable);
    } else {
        m_asyncActions.insert(requestId, info);
        connect(info, &ThingActionInfo::aborted, this, [requestId, this] {m_asyncActions.remove(requestId);});
    }
}
