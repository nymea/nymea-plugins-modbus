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

#include "integrationpluginbgetech.h"
#include "plugininfo.h"

IntegrationPluginBGetech::IntegrationPluginBGetech()
{

}

void IntegrationPluginBGetech::discoverThings(ThingDiscoveryInfo *info)
{
    qCDebug(dcBGetech()) << "Discover things";
    QList<ThingDescriptor> thingDescriptors;

    if (info->thingClassId() == threePhaseThingClassId) {
        Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
            //Serial port is not yet used, create now a new one
            qCDebug(dcBGetech()) << "Found serial port:" << port.portName();
            QString description = port.manufacturer() + " " + port.description();
            ThingDescriptor descriptor(info->thingClassId(), port.portName(), description);
            ParamList parameters;
            parameters.append(Param(modbusConnectionThingSerialPortParamTypeId, port.systemLocation()));
            descriptor.setParams(parameters);
            info->addThingDescriptor(descriptor);
        }
        info->finish(Thing::ThingErrorNoError);
        return;
    } else {
        Q_ASSERT_X(false, "discoverThings", QString("Unhandled thingClassId: %1").arg(info->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginBGetech::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcBGetech()) << "Setup thing" << thing->name();

    if (thing->thingClassId() == threePhaseThingClassId) {
        QString serialPort = thing->paramValue(modbusConnectionThingSerialPortParamTypeId).toString();
        int baudRate = thing->paramValue(modbusConnectionThingBaudRateParamTypeId).toInt();

        ModbusRTUMaster *modbus = new ModbusRTUMaster(serialPort, baudRate, QSerialPort::Parity::NoParity, 8, 1, this);
        connect(modbus, &ModbusRTUMaster::connectionStateChanged, this, &IntegrationPluginBGetech::onConnectionStateChanged);
        connect(modbus, &ModbusRTUMaster::receivedHoldingRegister, this, &IntegrationPluginBGetech::onReceivedHoldingRegister);
        connect(modbus, &ModbusRTUMaster::receivedInputRegister, this, &IntegrationPluginBGetech::onReceivedInputRegister);
        connect(modbus, &ModbusRTUMaster::receivedInputRegister, this, [this]{
                    m_modbusRTUMasters.insert(thing, modbus);
        });
        connect(modbus, &ModbusRTUMaster::destroyed, this, [this] {

        });


        info->finish(Thing::ThingErrorNoError);
        return;
    } else {
        Q_ASSERT_X(false, "setupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginBGetech::postSetupThing(Thing *thing)
{
    qCDebug(dcBGetech) << "Post setup thing" << thing->name();
    if (!m_pluginTimer) {
        qCDebug(dcBGetech()) << "Creating plugin timer";
        m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(60);
        connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
            qCDebug(dcBGetech()) << "Plugin timeout";
        });
    }
}

void IntegrationPluginBGetech::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    if (thing->thingClassId() == singlePhaseThingClassId) {
    } else if (thing->thingClassId() == threePhaseThingClassId) {
        if (action.actionTypeId() == threePhase) {

        } else {
            Q_ASSERT_X(false, "executeAction", QString("Unhandled actionTypeId: %1").arg(action.actionTypeId().toString()).toUtf8());
        }
    } else {
        Q_ASSERT_X(false, "executeAction", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginBGetech::thingRemoved(Thing *thing)
{
    qCDebug(dcBGetech) << "Thing removed" << thing->name();
}

void IntegrationPluginBGetech::readHoldingRegister(Thing *thing)
{
    ModbusRTUMaster *modbus = m_connections.value(thing);
    uint slaveAddress = thing->paramValue(threePhaseThingSlaveIdParamTypeId).toInt();

    modbus->readHoldingRegister(slaveAddress, HoldingRegisters::Pulse1Output);
    modbus->readHoldingRegister(slaveAddress, HoldingRegisters::Pulse1Constant);
    modbus->readHoldingRegister(slaveAddress, HoldingRegisters::MeasurementMode);
    modbus->readHoldingRegister(slaveAddress, HoldingRegisters::MeasurementMode);
    modbus->readHoldingRegister(slaveAddress, HoldingRegisters::MeasurementMode);
    modbus->readHoldingRegister(slaveAddress, HoldingRegisters::MeasurementMode);
}

void IntegrationPluginBGetech::readInputRegister(Thing *thing)
{

}

void IntegrationPluginBGetech::onConnectionStateChanged(bool status)
{
    ModbusRTUMaster *modbus = static_cast<ModbusRTUMaster *>(sender());
}

void IntegrationPluginBGetech::onReceivedHoldingRegister(int slaveAddress, int modbusRegister, const QVector<quint16> &value)
{
    ModbusRTUMaster *modbus = static_cast<ModbusRTUMaster *>(sender());
}

void IntegrationPluginBGetech::onReceivedInputRegister(int slaveAddress, int modbusRegister, const QVector<quint16> &value)
{
    ModbusRTUMaster *modbus = static_cast<ModbusRTUMaster *>(sender());

}

void IntegrationPluginBGetech::onWriteRequestExecuted(const QUuid &requestId, bool success)
{
    //ModbusRTUMaster *modbus = static_cast<ModbusRTUMaster *>(sender());

    if (m_asyncActions.contains(requestId)) {
        ThingActionInfo *info = m_asyncActions.take(requestId);
        if (success) {
            info->finish(Thing::ThingErrorNoError);
        } else {
            info->finish(Thing::ThingErrorHardwareNotAvailable);
        }
    }
}

void IntegrationPluginBGetech::onWriteRequestError(const QUuid &requestId, const QString &error)
{
    //ModbusRTUMaster *modbus = static_cast<ModbusRTUMaster *>(sender());

    if (m_asyncActions.contains(requestId)) {
        ThingActionInfo *info = m_asyncActions.take(requestId);
        info->finish(Thing::ThingErrorHardwareNotAvailable);
    }
}
