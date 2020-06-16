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
        qCWarning(dcMypv()) << "Error sending discovery";
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

            quint32 uiAddress = 0;
            for (int i=0; i<4; i++) {
                uiAddress |= data.at(7-i) << (i*8);
            }
            QHostAddress address(uiAddress);
            QByteArray serialNumber = data.mid(8, 16);

            bool existing = false;
            foreach (Thing *existingThing, myThings()) {
                if (existingThing->thingClassId() == info->thingClassId() && existingThing->paramValue(elwaThingIpAddressParamTypeId).toString() == address.toString()) {
                    existing = true;
                }
            }
            if (existing) {
                qCDebug(dcMypv()) << "Already have device" << senderAddress << "in configured devices. Skipping...";
                continue;
            }
            ThingDescriptor thingDescriptors(info->thingClassId(), "AC ELWA-E", address.toString());
            ParamList params;
            params << Param(elwaThingIpAddressParamTypeId, address.toString());
            params << Param(elwaThingSerialNumberParamTypeId, serialNumber);
            thingDescriptors.setParams(params);
            descriptorList << thingDescriptors;
        }
        info->addThingDescriptors(descriptorList);;
        searchSocket->deleteLater();
    });
}

void IntegrationPluginMyPv::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    if(thing->thingClassId() == elwaThingClassId) {
        QHostAddress address = QHostAddress(thing->paramValue(elwaThingIpAddressParamTypeId).toString());
        ModbusTCPMaster *modbusTcpMaster = new ModbusTCPMaster(address, 502, this);

        if (!modbusTcpMaster->createInterface()) {
            modbusTcpMaster->deleteLater();

            return;
        }
        m_modbusTcpMasters.insert(thing, modbusTcpMaster);
    } else {
        Q_ASSERT_X(false, "setupDevice", QString("Unhandled deviceClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginMyPv::postSetupThing(Thing *thing)
{
    if (thing->thingClassId() == elwaThingClassId) {
        update(thing);
    }
}

void IntegrationPluginMyPv::thingRemoved(Thing *thing)
{
    if (thing->thingClassId() == elwaThingClassId) {
        ModbusTCPMaster *modbusTCPMaster = m_modbusTcpMasters.take(thing);
        modbusTCPMaster->deleteLater();
    }
}

void IntegrationPluginMyPv::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    if (thing->thingClassId() == elwaThingClassId) {

        ModbusTCPMaster *modbusTCPMaster = m_modbusTcpMasters.value(thing);
        if (action.actionTypeId() == elwaHeatingPowerActionTypeId) {
            int heatingPower = action.param(elwaHeatingPowerActionHeatingPowerParamTypeId).value().toInt();

            if(!modbusTCPMaster->writeHoldingRegister(0xff, ElwaModbusRegisters::Power, heatingPower)){
                return info->finish(Thing::ThingErrorHardwareFailure);
            }
            return;
        } else if (action.actionTypeId() == elwaPowerActionTypeId) {
            bool power = action.param(elwaHeatingPowerActionHeatingPowerParamTypeId).value().toBool();
            if(power) {
                if(!modbusTCPMaster->writeHoldingRegister(0xff, ElwaModbusRegisters::ManuelStart, 1)){
                    return info->finish(Thing::ThingErrorHardwareFailure);
                }
            }
        } else {
            Q_ASSERT_X(false, "executeAction", QString("Unhandled actionTypeId: %1").arg(action.actionTypeId().toString()).toUtf8());
        }
    } else {
        Q_ASSERT_X(false, "executeAction", QString("Unhandled deviceClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginMyPv::onRefreshTimer(){

    foreach (Thing *thing, myThings()) {
        update(thing);
    }
}

void IntegrationPluginMyPv::onConnectionStateChanged(bool status)
{
//TODO set device connected state
}

void IntegrationPluginMyPv::update(Thing *thing) {
    if (thing->thingClassId() == elwaThingClassId)
    {
        ModbusTCPMaster *modbusTCPMaster = m_modbusTcpMasters.value(thing);

        int data;
        if (modbusTCPMaster->readHoldingRegister(0xff, ElwaModbusRegisters::Status, &data)) {
            switch (data) {
            case Heating: {
                thing->setStateValue(elwaStatusStateTypeId, "Heating");
                thing->setStateValue(elwaPowerStateTypeId, true);
                break;
            }
            case Standby:{
                thing->setStateValue(elwaStatusStateTypeId, "Standby");
                thing->setStateValue(elwaPowerStateTypeId, false);
                break;
            }
            case Boosted:{
                thing->setStateValue(elwaStatusStateTypeId, "Boosted");
                thing->setStateValue(elwaPowerStateTypeId, true);
                break;
            }
            case HeatFinished:{
                thing->setStateValue(elwaStatusStateTypeId, "Heat finished");
                thing->setStateValue(elwaPowerStateTypeId, false);
                break;
            }
            case Setup:{
                thing->setStateValue(elwaStatusStateTypeId, "Setup");
                thing->setStateValue(elwaPowerStateTypeId, false);
                break;
            }
            case ErrorOvertempFuseBlown:{
                thing->setStateValue(elwaStatusStateTypeId, "Error Overtemp Fuse blown");
                break;
            }
            case ErrorOvertempMeasured:{
                thing->setStateValue(elwaStatusStateTypeId, "Error Overtemp measured");
                break;
            }
            case  ErrorOvertempElectronics:{
                thing->setStateValue(elwaStatusStateTypeId, "Error Overtemp Electronics");
                break;
            }
            case ErrorHardwareFault:{
                thing->setStateValue(elwaStatusStateTypeId, "Error Hardware Fault");
                break;
            }
            case ErrorTempSensor:{
                thing->setStateValue(elwaStatusStateTypeId, "Error Temp Sensor");
                break;
            }
            default:
                thing->setStateValue(elwaStatusStateTypeId, "Unknown");
            }

            thing->setStateValue(elwaConnectedStateTypeId, true);
        } else {
            thing->setStateValue(elwaConnectedStateTypeId, false);
        }

        if (modbusTCPMaster->getRegister(0xff, ElwaModbusRegisters::WaterTemperature, &data)) {
            thing->setStateValue(elwaTemperatureStateTypeId, data/10.00);
        }
        if (modbusTCPMaster->getRegister(0xff, ElwaModbusRegisters::TargetWaterTemperature, &data)) {
            thing->setStateValue(elwaTargetWaterTemperatureStateTypeId, data/10.00);
        }
        if (modbusTCPMaster->getRegister(0xff, ElwaModbusRegisters::Power, &data)) {
            thing->setStateValue(elwaHeatingPowerStateTypeId, data);
        }
    }
}

