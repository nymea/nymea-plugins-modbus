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

#include "plugininfo.h"
#include "devicepluginmypv.h"

#include <QUdpSocket>
#include <QHostAddress>

DevicePluginMyPv::DevicePluginMyPv()
{

}


void DevicePluginMyPv::discoverDevices(DeviceDiscoveryInfo *info)
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
        QList<DeviceDescriptor> descriptorList;
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
            foreach (Device *existingDev, myDevices()) {
                if (existingDev->deviceClassId() == info->deviceClassId() && existingDev->paramValue(elwaDeviceIpAddressParamTypeId).toString() == address.toString()) {
                    existing = true;
                }
            }
            if (existing) {
                qCDebug(dcMypv()) << "Already have device" << senderAddress << "in configured devices. Skipping...";
                continue;
            }
            DeviceDescriptor deviceDescriptors(info->deviceClassId(), "AC ELWA-E", address.toString());
            ParamList params;
            params << Param(elwaDeviceIpAddressParamTypeId, address.toString());
            params << Param(elwaDeviceSerialNumberParamTypeId, serialNumber);
            deviceDescriptors.setParams(params);
            descriptorList << deviceDescriptors;
        }
        info->addDeviceDescriptors(descriptorList);;
        searchSocket->deleteLater();
    });
}

void DevicePluginMyPv::setupDevice(DeviceSetupInfo *info)
{
    Device *device = info->device();

    if(device->deviceClassId() == elwaDeviceClassId) {
        QHostAddress address = QHostAddress(device->paramValue(elwaDeviceIpAddressParamTypeId).toString());
        ModbusTCPMaster *modbusTcpMaster = new ModbusTCPMaster(address, 502, this);

        if (!modbusTcpMaster->createInterface()) {
            modbusTcpMaster->deleteLater();

            return;
        }
        m_modbusTcpMasters.insert(device, modbusTcpMaster);
    } else {
        Q_ASSERT_X(false, "setupDevice", QString("Unhandled deviceClassId: %1").arg(device->deviceClassId().toString()).toUtf8());
    }
}

void DevicePluginMyPv::postSetupDevice(Device *device)
{
    if (device->deviceClassId() == elwaDeviceClassId) {
        update(device);
    }
}

void DevicePluginMyPv::deviceRemoved(Device *device)
{
    if (device->deviceClassId() == elwaDeviceClassId) {
        ModbusTCPMaster *modbusTCPMaster = m_modbusTcpMasters.take(device);
        modbusTCPMaster->deleteLater();
    }
}

void DevicePluginMyPv::executeAction(DeviceActionInfo *info)
{
    Device *device = info->device();
    Action action = info->action();

    if (device->deviceClassId() == elwaDeviceClassId) {

        ModbusTCPMaster *modbusTCPMaster = m_modbusTcpMasters.value(device);
        if (action.actionTypeId() == elwaHeatingPowerActionTypeId) {
            int heatingPower = action.param(elwaHeatingPowerActionHeatingPowerParamTypeId).value().toInt();

            if(!modbusTCPMaster->setRegister(0xff, ElwaModbusRegisters::Power, heatingPower)){
                return info->finish(Device::DeviceErrorHardwareFailure);
            }
            return;
        } else if (action.actionTypeId() == elwaPowerActionTypeId) {
            bool power = action.param(elwaHeatingPowerActionHeatingPowerParamTypeId).value().toBool();
            if(power) {
                if(!modbusTCPMaster->setRegister(0xff, ElwaModbusRegisters::ManuelStart, 1)){
                    return info->finish(Device::DeviceErrorHardwareFailure);
                }
            }
        } else {
            Q_ASSERT_X(false, "executeAction", QString("Unhandled actionTypeId: %1").arg(action.actionTypeId().toString()).toUtf8());
        }
    } else {
        Q_ASSERT_X(false, "executeAction", QString("Unhandled deviceClassId: %1").arg(device->deviceClassId().toString()).toUtf8());
    }
}

void DevicePluginMyPv::onRefreshTimer(){

    foreach (Device *device, myDevices()) {
        update(device);
    }
}

void DevicePluginMyPv::update(Device *device) {
    if (device->deviceClassId() == elwaDeviceClassId)
    {
        ModbusTCPMaster *modbusTCPMaster = m_modbusTcpMasters.value(device);

        int data;
        if (modbusTCPMaster->getRegister(0xff, ElwaModbusRegisters::Status, &data)) {
            switch (data) {
            case Heating: {
                device->setStateValue(elwaStatusStateTypeId, "Heating");
                device->setStateValue(elwaPowerStateTypeId, true);
                break;
            }
            case Standby:{
                device->setStateValue(elwaStatusStateTypeId, "Standby");
                device->setStateValue(elwaPowerStateTypeId, false);
                break;
            }
            case Boosted:{
                device->setStateValue(elwaStatusStateTypeId, "Boosted");
                device->setStateValue(elwaPowerStateTypeId, true);
                break;
            }
            case HeatFinished:{
                device->setStateValue(elwaStatusStateTypeId, "Heat finished");
                device->setStateValue(elwaPowerStateTypeId, false);
                break;
            }
            case Setup:{
                device->setStateValue(elwaStatusStateTypeId, "Setup");
                device->setStateValue(elwaPowerStateTypeId, false);
                break;
            }
            case ErrorOvertempFuseBlown:{
                device->setStateValue(elwaStatusStateTypeId, "Error Overtemp Fuse blown");
                break;
            }
            case ErrorOvertempMeasured:{
                device->setStateValue(elwaStatusStateTypeId, "Error Overtemp measured");
                break;
            }
            case  ErrorOvertempElectronics:{
                device->setStateValue(elwaStatusStateTypeId, "Error Overtemp Electronics");
                break;
            }
            case ErrorHardwareFault:{
                device->setStateValue(elwaStatusStateTypeId, "Error Hardware Fault");
                break;
            }
            case ErrorTempSensor:{
                device->setStateValue(elwaStatusStateTypeId, "Error Temp Sensor");
                break;
            }
            default:
                device->setStateValue(elwaStatusStateTypeId, "Unknown");
            }

            device->setStateValue(elwaConnectedStateTypeId, true);
        } else {
            device->setStateValue(elwaConnectedStateTypeId, false);
        }

        if (modbusTCPMaster->getRegister(0xff, ElwaModbusRegisters::WaterTemperature, &data)) {
            device->setStateValue(elwaTemperatureStateTypeId, data/10.00);
        }
        if (modbusTCPMaster->getRegister(0xff, ElwaModbusRegisters::TargetWaterTemperature, &data)) {
            device->setStateValue(elwaTargetWaterTemperatureStateTypeId, data/10.00);
        }
        if (modbusTCPMaster->getRegister(0xff, ElwaModbusRegisters::Power, &data)) {
            device->setStateValue(elwaHeatingPowerStateTypeId, data);
        }
    }
}

