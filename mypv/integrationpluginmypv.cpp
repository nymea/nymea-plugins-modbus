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

#include "plugininfo.h"
#include "integrationpluginmypv.h"

#include <QUdpSocket>
#include <QHostAddress>

IntegrationPluginMyPv::IntegrationPluginMyPv()
{
}


void IntegrationPluginMyPv::discoverThings(ThingDiscoveryInfo *info)
{
    if (info->thingClassId() == elwaThingClassId) {
        QUdpSocket *searchSocket = new QUdpSocket(this);

        // Note: This will fail, and it's not a problem, but it is required to force the socket to stick to IPv4...
        searchSocket->bind(QHostAddress::AnyIPv4, 16124);

        QByteArray discoveryString;
        discoveryString.resize(19);
        discoveryString.fill(0);
        discoveryString.insert(0, QByteArray::fromHex("86d93efc"));

        discoveryString.insert(4, "AC ELWA-E");
        qCDebug(dcMypv()) << "Send datagram:" << discoveryString << "length: " << discoveryString.length();
        qint64 len = searchSocket->writeDatagram(discoveryString, QHostAddress("255.255.255.255"), 16124);
        if (len != discoveryString.length()) {
            searchSocket->deleteLater();
            info->finish(Thing::ThingErrorHardwareNotAvailable , tr("Error starting device discovery"));
            return;
        }

        QTimer::singleShot(2000, this, [this, searchSocket, info](){
            QList<ThingDescriptor> descriptorList;
            while(searchSocket->hasPendingDatagrams()) {
                char buffer[1024];
                QHostAddress senderAddress;
                int len = searchSocket->readDatagram(buffer, 1024, &senderAddress);
                QByteArray data = QByteArray::fromRawData(buffer, len);
                qCDebug(dcMypv()) << "Have datagram:" << data;
                if (data.length() < 64) {
                    continue;
                }

                //Device Id AC•THOR = 0x4e84
                //Device Id Power  = 0x4e8e
                //Device Id AC ELWA-E = 0x3efc
                qCDebug(dcMypv()) << "device Id:" << data.mid(2, 2);
                if (data.mid(2, 2) == QByteArray::fromHex("3efc")) {
                    qCDebug(dcMypv()) << "Found Device: AC ElWA-E";
                } else if (data.mid(2, 2) == QByteArray::fromHex("0x4e8e")) {
                    qCDebug(dcMypv()) << "Found Device: Powermeter";
                } else if (data.mid(2, 2) == QByteArray::fromHex("0x4e84")) {
                    qCDebug(dcMypv()) << "Found Device: AC Thor";
                } else {
                    qCDebug(dcMypv()) << "Failed to parse discovery datagram from" << senderAddress << data;
                    continue;
                }

                ThingDescriptor thingDescriptors(info->thingClassId(), "AC ELWA-E", senderAddress.toString());
                QByteArray serialNumber = data.mid(8, 16);

                foreach (Thing *existingThing, myThings()) {
                    if (serialNumber == existingThing->paramValue(elwaThingSerialNumberParamTypeId).toString()) {
                        qCDebug(dcMypv()) << "Rediscovered device " << existingThing->name();
                        thingDescriptors.setThingId(existingThing->id());
                        break;
                    }
                }

                ParamList params;
                params << Param(elwaThingIpAddressParamTypeId, senderAddress.toString());
                params << Param(elwaThingSerialNumberParamTypeId, serialNumber);
                thingDescriptors.setParams(params);
                descriptorList << thingDescriptors;
            }
            info->addThingDescriptors(descriptorList);;
            searchSocket->deleteLater();
            info->finish(Thing::ThingErrorNoError);
        });
    } else {
        Q_ASSERT_X(false, "discoverThings", QString("Unhandled thingClassId: %1").arg(info->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginMyPv::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    if(thing->thingClassId() == elwaThingClassId) {
        QHostAddress address = QHostAddress(thing->paramValue(elwaThingIpAddressParamTypeId).toString());
        ModbusTcpMaster *modbusTcpMaster = new ModbusTcpMaster(address, 502, this);
        connect(modbusTcpMaster, &ModbusTcpMaster::connectionStateChanged, this, &IntegrationPluginMyPv::onConnectionStateChanged);
        connect(modbusTcpMaster, &ModbusTcpMaster::receivedHoldingRegister, this, &IntegrationPluginMyPv::onReceivedHoldingRegister);
        connect(modbusTcpMaster, &ModbusTcpMaster::writeRequestExecuted, this, &IntegrationPluginMyPv::onWriteRequestExecuted);
        connect(modbusTcpMaster, &ModbusTcpMaster::writeRequestError, this, &IntegrationPluginMyPv::onWriteRequestError);

        m_modbusTcpMasters.insert(thing, modbusTcpMaster);
    } else {
        Q_ASSERT_X(false, "setupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginMyPv::postSetupThing(Thing *thing)
{
    if (!m_refreshTimer) {
        m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(10);
        connect(m_refreshTimer, &PluginTimer::timeout, this, &IntegrationPluginMyPv::onRefreshTimer);
    }

    if (thing->thingClassId() == elwaThingClassId) {
        update(thing);
    }
}

void IntegrationPluginMyPv::thingRemoved(Thing *thing)
{
    if (thing->thingClassId() == elwaThingClassId) {
        ModbusTcpMaster *modbusTCPMaster = m_modbusTcpMasters.take(thing);
        modbusTCPMaster->deleteLater();
    }

    if (myThings().isEmpty()) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_refreshTimer);
        m_refreshTimer = nullptr;
    }
}

void IntegrationPluginMyPv::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    if (thing->thingClassId() == elwaThingClassId) {

        ModbusTcpMaster *modbusTCPMaster = m_modbusTcpMasters.value(thing);
        if (action.actionTypeId() == elwaHeatingPowerActionTypeId) {
            int heatingPower = action.param(elwaHeatingPowerActionHeatingPowerParamTypeId).value().toInt();
            QUuid requestId = modbusTCPMaster->writeHoldingRegister(0xff, ElwaModbusRegisters::Power, heatingPower);
            if (requestId.isNull()) {
                info->finish(Thing::ThingErrorHardwareNotAvailable);
            } else {
                m_asyncActions.insert(requestId, info);
                connect(info, &ThingActionInfo::aborted, this, [this, requestId] {m_asyncActions.remove(requestId);});
            }
        } else if (action.actionTypeId() == elwaPowerActionTypeId) {
            bool power = action.param(elwaHeatingPowerActionHeatingPowerParamTypeId).value().toBool();
            if(power) {
                QUuid requestId = modbusTCPMaster->writeHoldingRegister(0xff, ElwaModbusRegisters::ManuelStart, 1);
                if (requestId.isNull()) {
                    info->finish(Thing::ThingErrorHardwareNotAvailable);
                } else {
                    m_asyncActions.insert(requestId, info);
                    connect(info, &ThingActionInfo::aborted, this, [this, requestId] {m_asyncActions.remove(requestId);});
                }
            }
        } else {
            Q_ASSERT_X(false, "executeAction", QString("Unhandled actionTypeId: %1").arg(action.actionTypeId().toString()).toUtf8());
        }
    } else {
        Q_ASSERT_X(false, "executeAction", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginMyPv::onRefreshTimer()
{
    foreach (Thing *thing, myThings()) {
        update(thing);
    }
}

void IntegrationPluginMyPv::onConnectionStateChanged(bool status)
{
    ModbusTcpMaster *modbusTcpMaster = static_cast<ModbusTcpMaster *>(sender());
    Thing *thing = m_modbusTcpMasters.key(modbusTcpMaster);
    if (!thing)
        return;
    thing->setStateValue(elwaConnectedStateTypeId, status);
}

void IntegrationPluginMyPv::onWriteRequestExecuted(QUuid requestId, bool success)
{
    if (m_asyncActions.contains(requestId)) {
        ThingActionInfo *info = m_asyncActions.value(requestId);
        if (success) {
            info->finish(Thing::ThingErrorNoError);
        } else {
            info->finish(Thing::ThingErrorHardwareNotAvailable);
        }
    }
}

void IntegrationPluginMyPv::onWriteRequestError(QUuid requestId, const QString &error)
{
    Q_UNUSED(requestId)
    qCWarning(dcMypv()) << "Modbus error "<< error;
}

void IntegrationPluginMyPv::onReceivedHoldingRegister(quint32 slaveAddress, quint32 modbusRegister, const QVector<quint16> &value)
{
    Q_UNUSED(slaveAddress)
    ModbusTcpMaster *modbusTcpMaster = static_cast<ModbusTcpMaster *>(sender());
    Thing *thing = m_modbusTcpMasters.key(modbusTcpMaster);
    if (!thing)
        return;

    if(modbusRegister == ElwaModbusRegisters::Status) {
        switch (ElwaStatus(value[0])) {
        case ElwaStatus::Heating: {
            thing->setStateValue(elwaStatusStateTypeId, "Heating");
            thing->setStateValue(elwaPowerStateTypeId, true);
            break;
        }
        case ElwaStatus::Standby:{
            thing->setStateValue(elwaStatusStateTypeId, "Standby");
            thing->setStateValue(elwaPowerStateTypeId, false);
            break;
        }
        case ElwaStatus::Boosted:{
            thing->setStateValue(elwaStatusStateTypeId, "Boosted");
            thing->setStateValue(elwaPowerStateTypeId, true);
            break;
        }
        case ElwaStatus::HeatFinished:{
            thing->setStateValue(elwaStatusStateTypeId, "Heat finished");
            thing->setStateValue(elwaPowerStateTypeId, false);
            break;
        }
        case ElwaStatus::Setup:{
            thing->setStateValue(elwaStatusStateTypeId, "Setup");
            thing->setStateValue(elwaPowerStateTypeId, false);
            break;
        }
        case ElwaStatus::ErrorOvertempFuseBlown:{
            thing->setStateValue(elwaStatusStateTypeId, "Error Overtemp Fuse blown");
            break;
        }
        case ElwaStatus::ErrorOvertempMeasured:{
            thing->setStateValue(elwaStatusStateTypeId, "Error Overtemp measured");
            break;
        }
        case ElwaStatus::ErrorOvertempElectronics:{
            thing->setStateValue(elwaStatusStateTypeId, "Error Overtemp Electronics");
            break;
        }
        case ElwaStatus::ErrorHardwareFault:{
            thing->setStateValue(elwaStatusStateTypeId, "Error Hardware Fault");
            break;
        }
        case ElwaStatus::ErrorTempSensor:{
            thing->setStateValue(elwaStatusStateTypeId, "Error Temp Sensor");
            break;
        }
        default:
            thing->setStateValue(elwaStatusStateTypeId, "Unknown");
        }
    } else if(modbusRegister == ElwaModbusRegisters::WaterTemperature) {
        thing->setStateValue(elwaTemperatureStateTypeId, value[0]/10.00);
    } else if(modbusRegister == ElwaModbusRegisters::TargetWaterTemperature) {
        thing->setStateValue(elwaTargetWaterTemperatureStateTypeId, value[0]/10.00);
    } else if(modbusRegister == ElwaModbusRegisters::Power) {
        thing->setStateValue(elwaHeatingPowerStateTypeId, value[0]);
    } else {
        qCWarning(dcMypv()) << "Received unhandled modbus register";
    }
}

void IntegrationPluginMyPv::update(Thing *thing)
{
    if (thing->thingClassId() == elwaThingClassId) {
        ModbusTcpMaster *modbusTCPMaster = m_modbusTcpMasters.value(thing);

        modbusTCPMaster->readHoldingRegister(0xff, ElwaModbusRegisters::Status);
        modbusTCPMaster->readHoldingRegister(0xff, ElwaModbusRegisters::WaterTemperature);
        modbusTCPMaster->readHoldingRegister(0xff, ElwaModbusRegisters::TargetWaterTemperature);
        modbusTCPMaster->readHoldingRegister(0xff, ElwaModbusRegisters::Power);
    }
}

