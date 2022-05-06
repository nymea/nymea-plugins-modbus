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

#include <hardwaremanager.h>
#include <network/networkdevicediscovery.h>
#include <hardware/modbus/modbusrtumaster.h>
#include <hardware/modbus/modbusrtuhardwareresource.h>

IntegrationPluginModbusCommander::IntegrationPluginModbusCommander()
{
}

void IntegrationPluginModbusCommander::init()
{
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

    // Plugin configuration
    connect(this, &IntegrationPluginModbusCommander::configValueChanged, this, &IntegrationPluginModbusCommander::onPluginConfigurationChanged);

    // Modbus RTU hardware resource
    connect(hardwareManager()->modbusRtuResource(), &ModbusRtuHardwareResource::modbusRtuMasterRemoved, this, [=](const QUuid &modbusUuid){
        qCDebug(dcModbusCommander()) << "Modbus RTU master has been removed" << modbusUuid.toString();

        // Check if there is any device using this resource
        foreach (Thing *thing, m_modbusRtuMasters.keys()) {
            if (m_modbusRtuMasters.value(thing)->modbusUuid() == modbusUuid) {
                qCWarning(dcModbusCommander()) << "Hardware resource removed for" << thing << ". The thing will not be functional any more until a new resource has been configured for it.";
                m_modbusRtuMasters.remove(thing);
                thing->setStateValue(m_connectedStateTypeId[thing->thingClassId()], false);

                // Set all child things disconnected
                foreach (Thing *childThing, myThings()) {
                    if (childThing->parentId() == thing->id()) {
                        thing->setStateValue(m_connectedStateTypeId[childThing->thingClassId()], false);
                    }
                }
            }
        }
    });
}

void IntegrationPluginModbusCommander::discoverThings(ThingDiscoveryInfo *info)
{
    ThingClassId thingClassId = info->thingClassId();
    if (thingClassId == modbusRTUClientThingClassId) {
        foreach (ModbusRtuMaster *modbusMaster, hardwareManager()->modbusRtuResource()->modbusRtuMasters()) {
            qCDebug(dcModbusCommander()) << "Found RTU master resource" << modbusMaster;
            if (modbusMaster->connected()) {
                ParamList parameters;
                ThingDescriptor thingDescriptor(thingClassId, "Modbus RTU master", modbusMaster->serialPort());
                parameters.append(Param(modbusRTUClientThingModbusMasterUuidParamTypeId, modbusMaster->modbusUuid()));
                thingDescriptor.setParams(parameters);
                info->addThingDescriptor(thingDescriptor);
            } else {
                qCWarning(dcModbusCommander()) << "Found configured resource" << modbusMaster << "but it is not connected. Skipping.";
            }
        }

        info->finish(Thing::ThingErrorNoError);
        return;
    } else if (thingClassId == modbusTCPClientThingClassId) {
        if (!hardwareManager()->networkDeviceDiscovery()->available()) {
            qCWarning(dcModbusCommander()) << "Failed to discover network devices. The network device discovery is not available.";
            info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The discovery is not available."));
            return;
        }

        NetworkDeviceDiscoveryReply *discoveryReply = hardwareManager()->networkDeviceDiscovery()->discover();
        connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
            ThingDescriptors descriptors;
            qCDebug(dcModbusCommander()) << "Discovery finished. Found" << discoveryReply->networkDeviceInfos().count() << "devices";
            foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {
                qCDebug(dcModbusCommander()) << networkDeviceInfo;
                QString title;
                if (networkDeviceInfo.hostName().isEmpty()) {
                    title += networkDeviceInfo.address().toString();
                } else {
                    title += networkDeviceInfo.address().toString() + " (" + networkDeviceInfo.hostName() + ")";
                }

                QString description;
                if (networkDeviceInfo.macAddressManufacturer().isEmpty()) {
                    description = networkDeviceInfo.macAddress();
                } else {
                    description = networkDeviceInfo.macAddress() + " (" + networkDeviceInfo.macAddressManufacturer() + ")";
                }

                ThingDescriptor descriptor(modbusTCPClientThingClassId, title, description);

                // Check if we already have set up this device
                Things existingThings = myThings().filterByParam(modbusTCPClientThingIpAddressParamTypeId, networkDeviceInfo.address().toString());
                if (existingThings.count() == 1) {
                    qCDebug(dcModbusCommander()) << "This thing already exists in the system." << existingThings.first() << networkDeviceInfo;
                    descriptor.setThingId(existingThings.first()->id());
                }

                ParamList params;
                params << Param(modbusTCPClientThingIpAddressParamTypeId, networkDeviceInfo.address().toString());
                descriptor.setParams(params);
                info->addThingDescriptor(descriptor);
            }
            info->finish(Thing::ThingErrorNoError);
        });
        return;
    } else if (thingClassId == discreteInputThingClassId) {
        Q_FOREACH(Thing *clientThing, myThings()){
            if (clientThing->thingClassId() == modbusTCPClientThingClassId) {
                ThingDescriptor descriptor(thingClassId, "Discrete input", clientThing->name() + " " + clientThing->paramValue(modbusTCPClientThingIpAddressParamTypeId).toString() + " Port: " + clientThing->paramValue(modbusTCPClientThingPortParamTypeId).toString());
                descriptor.setParentId(clientThing->id());
                info->addThingDescriptor(descriptor);
            }
            if (clientThing->thingClassId() == modbusRTUClientThingClassId) {
                ThingDescriptor descriptor(thingClassId, "Discrete input", clientThing->name() + " " + clientThing->paramValue(modbusRTUClientThingModbusMasterUuidParamTypeId).toString());
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
                ThingDescriptor descriptor(thingClassId, "Coil", clientThing->name() + " " + clientThing->paramValue(modbusRTUClientThingModbusMasterUuidParamTypeId).toString());
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
                ThingDescriptor descriptor(thingClassId, "Holding register", clientThing->name() + " " + clientThing->paramValue(modbusRTUClientThingModbusMasterUuidParamTypeId).toString());
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
                ThingDescriptor descriptor(thingClassId, "Input register", clientThing->name() + " " + clientThing->paramValue(modbusRTUClientThingModbusMasterUuidParamTypeId).toString());
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

        if (m_modbusTCPMasters.contains(thing)) {
            // In case of a rediscovery
            m_modbusTCPMasters.take(thing)->deleteLater();
        }

        foreach (ModbusTCPMaster *modbusTCPMaster, m_modbusTCPMasters.values()) {
            if ((modbusTCPMaster->hostAddress() == hostAddress) && (modbusTCPMaster->port() == port)) {
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
        QUuid modbusUuid = thing->paramValue(modbusRTUClientThingModbusMasterUuidParamTypeId).toUuid();

        if (!hardwareManager()->modbusRtuResource()->available()) {
            qCWarning(dcModbusCommander()) << "Cannot set up thing" << thing << ". The modbus RTU hardware resource is not available.";
            info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The modbus RTU hardware resource is not available"));
            return;
        }

        if (!hardwareManager()->modbusRtuResource()->hasModbusRtuMaster(modbusUuid)) {
            qCWarning(dcModbusCommander()) << "Cannot set up thing" << thing << ". The modbus RTU hardware resource" << modbusUuid.toString() << "does not exist any more. Reconfiguration required.";
            info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("Configured modbus RTU master could not be found. Please reconfigure the client and assign a new valid modbus RTU master."));
            return;
        }

        ModbusRtuMaster *modbusMaster = hardwareManager()->modbusRtuResource()->getModbusRtuMaster(modbusUuid);
        qCDebug(dcModbusCommander()) << "Setting up" << thing << "using" << modbusMaster;
        m_modbusRtuMasters.insert(thing, modbusMaster);

        connect(modbusMaster, &ModbusRtuMaster::connectedChanged, thing, [=](bool connected){
            qCDebug(dcModbusCommander()) << "Modbus RTU client" << modbusMaster << "connected changed" << connected;
            thing->setStateValue(modbusRTUClientConnectedStateTypeId, connected);

            // Note: only set the connected state for the child things if disconnected.
            // The child things will be evaluated upon read requests if the slave is connected or not.
            if (!connected) {
                foreach (Thing *childThing, myThings()) {
                    if (childThing->parentId() == thing->id()) {
                        thing->setStateValue(m_connectedStateTypeId[childThing->thingClassId()], connected);
                    }
                }
            }
        });

        info->finish(Thing::ThingErrorNoError);
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
    if (m_modbusTCPMasters.values().contains(static_cast<ModbusTCPMaster *>(modbus))) {
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
    if (m_modbusTCPMasters.values().contains(static_cast<ModbusTCPMaster *>(modbus))) {
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

    if (m_modbusTCPMasters.values().contains(static_cast<ModbusTCPMaster *>(modbus))) {
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

    if (m_modbusTCPMasters.values().contains(static_cast<ModbusTCPMaster *>(modbus))) {
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

    if (m_modbusTCPMasters.values().contains(static_cast<ModbusTCPMaster *>(modbus))) {
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

        ModbusRtuMaster *modbusMaster = m_modbusRtuMasters.value(parent);
        if (!modbusMaster)
            return;

        if (!modbusMaster->connected())
            return; // Send requests only if the modbus interface is connected

        if (thing->thingClassId() == coilThingClassId) {
            ModbusRtuReply *reply = modbusMaster->readCoil(slaveAddress, registerAddress);
            connect(reply, &ModbusRtuReply::finished, modbusMaster, [=](){
                if (reply->error() != ModbusRtuReply::NoError) {
                    qCWarning(dcModbusCommander()) << "Failed to read coil from" << modbusMaster << "slave:" << slaveAddress << "register:" << registerAddress;
                    thing->setStateValue(m_connectedStateTypeId[thing->thingClassId()], false);
                    return;
                }

                if (!reply->result().isEmpty()) {
                    thing->setStateValue(m_valueStateTypeId.value(thing->thingClassId()), reply->result().at(0));
                }
                thing->setStateValue(m_connectedStateTypeId.value(thing->thingClassId()), true);
            });
        } else if (thing->thingClassId() == discreteInputThingClassId) {
            ModbusRtuReply *reply = modbusMaster->readDiscreteInput(slaveAddress, registerAddress);
            connect(reply, &ModbusRtuReply::finished, modbusMaster, [=](){
                if (reply->error() != ModbusRtuReply::NoError) {
                    qCWarning(dcModbusCommander()) << "Failed to read discrete input from" << modbusMaster << "slave:" << slaveAddress << "register:" << registerAddress;
                    thing->setStateValue(m_connectedStateTypeId[thing->thingClassId()], false);
                    return;
                }

                if (!reply->result().isEmpty()) {
                    thing->setStateValue(m_valueStateTypeId.value(thing->thingClassId()), reply->result().at(0));
                }
                thing->setStateValue(m_connectedStateTypeId.value(thing->thingClassId()), true);
            });
        } else if (thing->thingClassId() == holdingRegisterThingClassId) {
            ModbusRtuReply *reply = modbusMaster->readHoldingRegister(slaveAddress, registerAddress);
            connect(reply, &ModbusRtuReply::finished, modbusMaster, [=](){
                if (reply->error() != ModbusRtuReply::NoError) {
                    qCWarning(dcModbusCommander()) << "Failed to read holding register from" << modbusMaster << "slave:" << slaveAddress << "register:" << registerAddress;
                    thing->setStateValue(m_connectedStateTypeId[thing->thingClassId()], false);
                    return;
                }

                if (!reply->result().isEmpty()) {
                    thing->setStateValue(m_valueStateTypeId.value(thing->thingClassId()), reply->result().at(0));
                }
                thing->setStateValue(m_connectedStateTypeId.value(thing->thingClassId()), true);
            });
        } else if (thing->thingClassId() == inputRegisterThingClassId) {
            ModbusRtuReply *reply = modbusMaster->readInputRegister(slaveAddress, registerAddress);
            connect(reply, &ModbusRtuReply::finished, modbusMaster, [=](){
                if (reply->error() != ModbusRtuReply::NoError) {
                    qCWarning(dcModbusCommander()) << "Failed to read input register from" << modbusMaster << "slave:" << slaveAddress << "register:" << registerAddress;
                    thing->setStateValue(m_connectedStateTypeId[thing->thingClassId()], false);
                    return;
                }

                if (!reply->result().isEmpty()) {
                    thing->setStateValue(m_valueStateTypeId.value(thing->thingClassId()), reply->result().at(0));
                }
                thing->setStateValue(m_connectedStateTypeId.value(thing->thingClassId()), true);
            });
        }

        // Note: we don't want proceed with the method here, since we are not
        // working with the requestId any more on RTU
        return;
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
        info->finish(Thing::ThingErrorHardwareNotAvailable);
        return;
    }

    uint registerAddress = thing->paramValue(m_registerAddressParamTypeId.value(thing->thingClassId())).toUInt();;
    uint slaveAddress = thing->paramValue(m_slaveAddressParamTypeId.value(thing->thingClassId())).toUInt();

    QUuid requestId;
    Action action = info->action();

    if (parent->thingClassId() == modbusTCPClientThingClassId) {
        ModbusTCPMaster *modbus = m_modbusTCPMasters.value(parent);
        if (!modbus) {
            qCWarning(dcModbusCommander()) << "Could not find modbus TCP master for" << thing;
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        if (thing->thingClassId() == coilThingClassId) {
            requestId = modbus->writeCoil(slaveAddress, registerAddress, action.param(coilValueActionValueParamTypeId).value().toBool());
        } else if (thing->thingClassId() == holdingRegisterThingClassId) {
            requestId = modbus->writeHoldingRegister(slaveAddress, registerAddress, action.param(holdingRegisterValueActionValueParamTypeId).value().toUInt());
        }

    } else if (parent->thingClassId() == modbusRTUClientThingClassId) {
        ModbusRtuMaster *modbusMaster = m_modbusRtuMasters.value(parent);
        if (!modbusMaster) {
            qCWarning(dcModbusCommander()) << "Could not find modbus RTU master for" << thing;
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        if (thing->thingClassId() == coilThingClassId) {
            QVector<quint16> values;
            values.append(static_cast<quint16>(action.param(coilValueActionValueParamTypeId).value().toBool()));

            ModbusRtuReply *reply = modbusMaster->writeCoils(slaveAddress, registerAddress, values);
            connect(info, &ThingActionInfo::aborted, reply, &ModbusRtuReply::deleteLater);
            connect(reply, &ModbusRtuReply::finished, modbusMaster, [=](){
                if (reply->error() != ModbusRtuReply::NoError) {
                    qCWarning(dcModbusCommander()) << "Failed to write coils from" << modbusMaster << "slave:" << slaveAddress << "register:" << registerAddress << values << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                    return;
                }

                info->finish(Thing::ThingErrorNoError);
            });
        } else if (thing->thingClassId() == holdingRegisterThingClassId) {
            QVector<quint16> values;
            values.append(static_cast<quint16>(action.param(holdingRegisterValueActionValueParamTypeId).value().toUInt()));

            ModbusRtuReply *reply = modbusMaster->writeHoldingRegisters(slaveAddress, registerAddress, values);
            connect(info, &ThingActionInfo::aborted, reply, &ModbusRtuReply::deleteLater);
            connect(reply, &ModbusRtuReply::finished, modbusMaster, [=](){
                if (reply->error() != ModbusRtuReply::NoError) {
                    qCWarning(dcModbusCommander()) << "Failed to write holding registers from" << modbusMaster << "slave:" << slaveAddress << "register:" << registerAddress << values << reply->errorString();
                    info->finish(Thing::ThingErrorHardwareFailure);
                    return;
                }

                info->finish(Thing::ThingErrorNoError);
            });
        }

        // Note: we don't want proceed with the method here, since we are not
        // working with the requestId any more on RTU
        return;
    }

    if (requestId.toString().isNull()){
        info->finish(Thing::ThingErrorHardwareNotAvailable);
    } else {
        m_asyncActions.insert(requestId, info);
        connect(info, &ThingActionInfo::aborted, this, [requestId, this] {m_asyncActions.remove(requestId);});
    }
}
