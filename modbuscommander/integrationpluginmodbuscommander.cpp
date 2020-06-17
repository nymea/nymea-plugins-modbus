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
    //QLoggingCategory::setFilterRules(QStringLiteral("qt.modbus* = false"));

    m_slaveAddressParamTypeId.insert(coilThingClassId, coilThingSlaveAddressParamTypeId);
    m_slaveAddressParamTypeId.insert(inputRegisterThingClassId, inputRegisterThingSlaveAddressParamTypeId);
    m_slaveAddressParamTypeId.insert(discreteInputThingClassId, discreteInputThingSlaveAddressParamTypeId);
    m_slaveAddressParamTypeId.insert(holdingRegisterThingClassId, holdingRegisterThingSlaveAddressParamTypeId);

    m_registerAddressParamTypeId.insert(coilThingClassId, coilThingRegisterAddressParamTypeId);
    m_registerAddressParamTypeId.insert(inputRegisterThingClassId, inputRegisterThingRegisterAddressParamTypeId);
    m_registerAddressParamTypeId.insert(discreteInputThingClassId, discreteInputThingRegisterAddressParamTypeId);
    m_registerAddressParamTypeId.insert(holdingRegisterThingClassId, holdingRegisterThingRegisterAddressParamTypeId);

    m_connectedStateTypeId.insert(coilThingClassId, coilConnectedStateTypeId);
    m_connectedStateTypeId.insert(inputRegisterThingClassId, inputRegisterConnectedStateTypeId);
    m_connectedStateTypeId.insert(discreteInputThingClassId, discreteInputConnectedStateTypeId);
    m_connectedStateTypeId.insert(holdingRegisterThingClassId, holdingRegisterConnectedStateTypeId);

    m_valueStateTypeId.insert(coilThingClassId, coilValueStateTypeId);
    m_valueStateTypeId.insert(inputRegisterThingClassId, inputRegisterValueStateTypeId);
    m_valueStateTypeId.insert(discreteInputThingClassId, discreteInputValueStateTypeId);
    m_valueStateTypeId.insert(holdingRegisterThingClassId, holdingRegisterValueStateTypeId);
}


void IntegrationPluginModbusCommander::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    if (thing->thingClassId() == modbusTCPClientThingClassId) {
        QHostAddress hostAddress = QHostAddress(thing->paramValue(modbusTCPClientThingIpv4addressParamTypeId).toString());
        uint port = thing->paramValue(modbusTCPClientThingPortParamTypeId).toUInt();

        foreach (ModbusTCPMaster *modbusTCPMaster, m_modbusTCPMasters.values()) {
            if ((modbusTCPMaster->hostAddress() == hostAddress) && (modbusTCPMaster->port() == port)){
                m_modbusTCPMasters.insert(thing, modbusTCPMaster);
                return info->finish(Thing::ThingErrorNoError);
            }
        }

        ModbusTCPMaster *modbusTCPMaster = new ModbusTCPMaster(hostAddress, port, this);
        connect(modbusTCPMaster, &ModbusTCPMaster::connectionStateChanged, this, &IntegrationPluginModbusCommander::onConnectionStateChanged);
        connect(modbusTCPMaster, &ModbusTCPMaster::writeRequestExecuted, this, &IntegrationPluginModbusCommander::onRequestExecuted);
        connect(modbusTCPMaster, &ModbusTCPMaster::writeRequestError, this, &IntegrationPluginModbusCommander::onRequestError);
        connect(modbusTCPMaster, &ModbusTCPMaster::receivedCoil, this, &IntegrationPluginModbusCommander::onReceivedCoil);
        connect(modbusTCPMaster, &ModbusTCPMaster::receivedDiscreteInput, this, &IntegrationPluginModbusCommander::onReceivedDiscreteInput);
        connect(modbusTCPMaster, &ModbusTCPMaster::receivedHoldingRegister, this, &IntegrationPluginModbusCommander::onReceivedHoldingRegister);
        connect(modbusTCPMaster, &ModbusTCPMaster::receivedInputRegister, this, &IntegrationPluginModbusCommander::onReceivedInputRegister);
        modbusTCPMaster->connectDevice();
        m_modbusTCPMasters.insert(thing, modbusTCPMaster);
        m_asyncTCPSetup.insert(modbusTCPMaster, info);
        return;

    } else if (thing->thingClassId() == modbusRTUClientThingClassId) {

        QString serialPort = thing->paramValue(modbusRTUClientThingSerialPortParamTypeId).toString();
        uint baudrate = thing->paramValue(modbusRTUClientThingBaudRateParamTypeId).toUInt();
        uint stopBits = thing->paramValue(modbusRTUClientThingStopBitsParamTypeId).toUInt();
        uint dataBits = thing->paramValue(modbusRTUClientThingDataBitsParamTypeId).toUInt();
        QSerialPort::Parity parity = QSerialPort::Parity::NoParity;
        if (thing->paramValue(modbusRTUClientThingParityParamTypeId).toString().contains("No")) {
            parity = QSerialPort::Parity::NoParity;
        } else if (thing->paramValue(modbusRTUClientThingParityParamTypeId).toString().contains("Even")) {
            parity = QSerialPort::Parity::EvenParity;
        } else if (thing->paramValue(modbusRTUClientThingParityParamTypeId).toString().contains("Odd")) {
            parity = QSerialPort::Parity::OddParity;
        }

        ModbusRTUMaster *modbusRTUMaster = new ModbusRTUMaster(serialPort, baudrate, parity, dataBits, stopBits, this);
        connect(modbusRTUMaster, &ModbusRTUMaster::connectionStateChanged, this, &IntegrationPluginModbusCommander::onConnectionStateChanged);
        connect(modbusRTUMaster, &ModbusRTUMaster::requestExecuted, this, &IntegrationPluginModbusCommander::onRequestExecuted);
        connect(modbusRTUMaster, &ModbusRTUMaster::requestError, this, &IntegrationPluginModbusCommander::onRequestError);
        connect(modbusRTUMaster, &ModbusRTUMaster::receivedCoil, this, &IntegrationPluginModbusCommander::onReceivedCoil);
        connect(modbusRTUMaster, &ModbusRTUMaster::receivedDiscreteInput, this, &IntegrationPluginModbusCommander::onReceivedDiscreteInput);
        connect(modbusRTUMaster, &ModbusRTUMaster::receivedHoldingRegister, this, &IntegrationPluginModbusCommander::onReceivedHoldingRegister);
        connect(modbusRTUMaster, &ModbusRTUMaster::receivedInputRegister, this, &IntegrationPluginModbusCommander::onReceivedInputRegister);
        modbusRTUMaster->connectDevice();
        m_modbusRTUMasters.insert(thing, modbusRTUMaster);
        m_asyncRTUSetup.insert(modbusRTUMaster, info);
        return;

    } else if ((thing->thingClassId() == coilThingClassId)
               || (thing->thingClassId() == discreteInputThingClassId)
               || (thing->thingClassId() == holdingRegisterThingClassId)
               || (thing->thingClassId() == inputRegisterThingClassId)) {
        info->finish(Thing::ThingErrorNoError);
        return;
    }
    qCWarning(dcModbusCommander()) << "Unhandled thing class in setupDevice!";
    info->finish(Thing::ThingErrorSetupFailed);
}

void IntegrationPluginModbusCommander::discoverThings(ThingDiscoveryInfo *info)
{
    ThingClassId thingClassId = info->thingClassId();

    if (thingClassId == modbusRTUClientThingClassId) {
        Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
            //Serial port is not yet used, create now a new one
            qCDebug(dcModbusCommander()) << "Found serial port:" << port.systemLocation();
            QString description = port.manufacturer() + " " + port.description();
            ThingDescriptor thingDescriptor(thingClassId, port.portName(), description);
            ParamList parameters;
            QString serialPort = port.systemLocation();
            foreach (Thing *existingThing, myThings()) {
                if (existingThing->paramValue(modbusRTUClientThingSerialPortParamTypeId).toString() == serialPort) {
                    thingDescriptor.setThingId(existingThing->id());
                    break;
                }
            }
            parameters.append(Param(modbusRTUClientThingSerialPortParamTypeId, serialPort));
            thingDescriptor.setParams(parameters);
            info->addThingDescriptor(thingDescriptor);
        }
        info->finish(Thing::ThingErrorNoError);
        return;
    } else if (thingClassId == discreteInputThingClassId) {
        Q_FOREACH(Thing *clientThing, myThings()){
            if (clientThing->thingClassId() == modbusTCPClientThingClassId) {
                ThingDescriptor descriptor(thingClassId, "Discrete input", clientThing->name() + " " + clientThing->paramValue(modbusTCPClientThingIpv4addressParamTypeId).toString() + " Port: " + clientThing->paramValue(modbusTCPClientThingPortParamTypeId).toString());
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
                ThingDescriptor descriptor(thingClassId, "Coil", clientThing->name() + " " + clientThing->paramValue(modbusTCPClientThingIpv4addressParamTypeId).toString() + " Port: " + clientThing->paramValue(modbusTCPClientThingPortParamTypeId).toString());
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
                ThingDescriptor descriptor(thingClassId, "Holding register", clientThing->name() + " " + clientThing->paramValue(modbusTCPClientThingIpv4addressParamTypeId).toString() + " Port: " + clientThing->paramValue(modbusTCPClientThingPortParamTypeId).toString());
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
                ThingDescriptor descriptor(thingClassId, "Input register", clientThing->name() + " " + clientThing->paramValue(modbusTCPClientThingIpv4addressParamTypeId).toString() + " Port: " + clientThing->paramValue(modbusTCPClientThingPortParamTypeId).toString());
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
    }
    info->finish(Thing::ThingErrorThingClassNotFound);
    qCWarning(dcModbusCommander()) << "Unhandled device class in discovery!";
}

void IntegrationPluginModbusCommander::postSetupThing(Thing *info)
{
    if (!m_refreshTimer) {
        // Refresh timer for TCP read
        int refreshTime = configValue(modbusCommanderPluginUpdateIntervalParamTypeId).toInt();
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(refreshTime);
        connect(m_refreshTimer, &PluginTimer::timeout, this, &IntegrationPluginModbusCommander::onRefreshTimer);
    }

    if ((info->thingClassId() == coilThingClassId) ||
            (info->thingClassId() == discreteInputThingClassId) ||
            (info->thingClassId() == holdingRegisterThingClassId) ||
            (info->thingClassId() == inputRegisterThingClassId)) {
        readRegister(info);
    }
}


void IntegrationPluginModbusCommander::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();

    if (thing->thingClassId() == coilThingClassId) {

        if (info->action().actionTypeId() == coilValueActionTypeId) {
            writeRegister(thing, info);
            return;
        }
    } else if (thing->thingClassId() == holdingRegisterThingClassId) {

        if (info->action().actionTypeId() == holdingRegisterValueActionTypeId) {
            writeRegister(thing, info);
            return;
        }
    }
    qCWarning(dcModbusCommander()) << "Unhandled deviceclass/actiontype in executeAction!";
    info->finish(Thing::ThingErrorThingClassNotFound);
}


void IntegrationPluginModbusCommander::thingRemoved(Thing *thing)
{
    if (thing->thingClassId() == modbusTCPClientThingClassId) {
        ModbusTCPMaster *modbus = m_modbusTCPMasters.take(thing);
        modbus->deleteLater();
    }

    if (thing->thingClassId() == modbusRTUClientThingClassId) {
        ModbusRTUMaster *modbus = m_modbusRTUMasters.take(thing);
        modbus->deleteLater();
    }

    if (myThings().empty()) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_refreshTimer);
        m_refreshTimer = nullptr;
    }
}

void IntegrationPluginModbusCommander::onRefreshTimer()
{
    foreach (Thing *thing, myThings()) {
        if ((thing->thingClassId() == coilThingClassId) ||
                (thing->thingClassId() == discreteInputThingClassId) ||
                (thing->thingClassId() == holdingRegisterThingClassId) ||
                (thing->thingClassId() == inputRegisterThingClassId)) {
            readRegister(thing);
        }
    }
}

void IntegrationPluginModbusCommander::onPluginConfigurationChanged(const ParamTypeId &paramTypeId, const QVariant &value)
{
    // Check refresh schedule
    if (paramTypeId == modbusCommanderPluginUpdateIntervalParamTypeId) {;
        if (m_refreshTimer) {
            uint refreshTime = value.toUInt();
            m_refreshTimer->stop();
            m_refreshTimer->startTimer(refreshTime);
        }
    }
}

void IntegrationPluginModbusCommander::onConnectionStateChanged(bool status)
{
    auto modbus = sender();

    if (m_asyncRTUSetup.contains(static_cast<ModbusRTUMaster *>(modbus))) {
        ThingSetupInfo *info = m_asyncRTUSetup.take(static_cast<ModbusRTUMaster *>(modbus));
        info->finish(Thing::ThingErrorNoError);

    } else if (m_asyncTCPSetup.contains(static_cast<ModbusTCPMaster *>(modbus))) {
        ThingSetupInfo *info = m_asyncTCPSetup.take(static_cast<ModbusTCPMaster *>(modbus));
        info->finish(Thing::ThingErrorNoError);
    }

    if (m_modbusRTUMasters.values().contains(static_cast<ModbusRTUMaster *>(modbus))) {
        Thing *thing = m_modbusRTUMasters.key(static_cast<ModbusRTUMaster *>(modbus));
        thing->setStateValue(modbusRTUClientConnectedStateTypeId, status);
    } else if (m_modbusTCPMasters.values().contains(static_cast<ModbusTCPMaster *>(modbus))) {
        Thing *thing = m_modbusTCPMasters.key(static_cast<ModbusTCPMaster *>(modbus));
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
        qCWarning(dcModbusCommander()) << "Could not find parente device" << thing->name();
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
