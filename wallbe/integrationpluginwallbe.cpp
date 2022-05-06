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

#include "integrationpluginwallbe.h"
#include "plugininfo.h"

#include <network/networkdevicediscovery.h>
#include <types/param.h>

#include <QDebug>
#include <QStringList>
#include <QJsonDocument>
#include <QNetworkInterface>


IntegrationPluginWallbe::IntegrationPluginWallbe()
{

}

void IntegrationPluginWallbe::init()
{
    // FIXME: make use of the internal network discovery if the device gets unavailable. For now, commented out since it has not been used
    // at the moment of changing this.

    //    m_discovery = new Discovery();
    //    connect(m_discovery, &Discovery::finished, this, [this](const QList<Host> &hosts) {
    //        foreach (const Host &host, hosts) {
    //            if (!host.vendor().contains("Phoenix", Qt::CaseSensitivity::CaseInsensitive))
    //                continue;

    //            Q_FOREACH(Thing *existingThing, myThings()) {
    //                if (existingThing->paramValue(wallbeEcoThingMacParamTypeId).toString().isEmpty()) {
    //                    //This device got probably manually setup, to enable auto rediscovery the MAC address needs to setup
    //                    if (existingThing->paramValue(wallbeEcoThingIpParamTypeId).toString() == host.address()) {
    //                        qCDebug(dcWallbe()) << "Wallbe Wallbox MAC Address has been discovered" << existingThing->name() << host.macAddress();
    //                        existingThing->setParamValue(wallbeEcoThingMacParamTypeId, host.macAddress());

    //                    }
    //                } else if (existingThing->paramValue(wallbeEcoThingMacParamTypeId).toString() == host.macAddress())  {
    //                    if (existingThing->paramValue(wallbeEcoThingIpParamTypeId).toString() != host.address()) {
    //                        qCDebug(dcWallbe()) << "Wallbe Wallbox IP Address has changed, from"  << existingThing->paramValue(wallbeEcoThingIpParamTypeId).toString() << "to" << host.address();
    //                        existingThing->setParamValue(wallbeEcoThingIpParamTypeId, host.address());

    //                    } else {
    //                        qCDebug(dcWallbe()) << "Wallbe Wallbox" << existingThing->name() << "IP address has not changed" << host.address();
    //                    }
    //                    break;
    //                }
    //            }
    //        }
    //    });
}


void IntegrationPluginWallbe::discoverThings(ThingDiscoveryInfo *info)
{
    if (info->thingClassId() == wallbeEcoThingClassId) {
        if (!hardwareManager()->networkDeviceDiscovery()->available()) {
            qCWarning(dcWallbe()) << "Failed to discover network devices. The network device discovery is not available.";
            info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("The discovery is not available."));
            return;
        }

        qCDebug(dcWallbe()) << "Start Wallbe eco discovery";
        NetworkDeviceDiscoveryReply *discoveryReply = hardwareManager()->networkDeviceDiscovery()->discover();
        connect(discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
            ThingDescriptors descriptors;
            qCDebug(dcWallbe()) << "Discovery finished. Found" << discoveryReply->networkDeviceInfos().count() << "devices";
            foreach (const NetworkDeviceInfo &networkDeviceInfo, discoveryReply->networkDeviceInfos()) {
                qCDebug(dcWallbe()) << networkDeviceInfo;
                if (!networkDeviceInfo.macAddressManufacturer().contains("Phoenix", Qt::CaseSensitivity::CaseInsensitive))
                    continue;

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

                ThingDescriptor descriptor(wallbeEcoThingClassId, title, description);

                // Check if we already have set up this device
                Things existingThings = myThings().filterByParam(wallbeEcoThingIpParamTypeId, networkDeviceInfo.address().toString());
                if (existingThings.count() == 1) {
                    qCDebug(dcWallbe()) << "This thing already exists in the system." << existingThings.first() << networkDeviceInfo;
                    descriptor.setThingId(existingThings.first()->id());
                }

                ParamList params;
                params << Param(wallbeEcoThingIpParamTypeId, networkDeviceInfo.address().toString());
                params << Param(wallbeEcoThingMacParamTypeId, networkDeviceInfo.macAddress());
                descriptor.setParams(params);
                info->addThingDescriptor(descriptor);
            }
            info->finish(Thing::ThingErrorNoError);
        });
    } else {
        Q_ASSERT_X(false, "discoverThings", QString("Unhandled thingClassId: %1").arg(info->thingClassId().toString()).toUtf8());
    }
}


void IntegrationPluginWallbe::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcWallbe) << "Setting up a new device:" << thing->params();

    if (thing->thingClassId() == wallbeEcoThingClassId) {
        QHostAddress address(thing->paramValue(wallbeEcoThingIpParamTypeId).toString());

        if (m_connections.contains(thing)) {
            qCDebug(dcWallbe()) << "Setup after reconfiguration, cleaning up ...";
            m_connections.take(thing)->deleteLater();
        }
        if (address.isNull()){
            qCWarning(dcWallbe) << "IP address is not valid";
            info->finish(Thing::ThingErrorSetupFailed, tr("Invalid IP address"));
            return;
        }
        ModbusTCPMaster *modbusTcpMaster = new ModbusTCPMaster(address, 502, this);
        connect(modbusTcpMaster, &ModbusTCPMaster::connectionStateChanged, this, &IntegrationPluginWallbe::onConnectionStateChanged);
        connect(modbusTcpMaster, &ModbusTCPMaster::receivedCoil, this, &IntegrationPluginWallbe::onReceivedCoil);
        connect(modbusTcpMaster, &ModbusTCPMaster::receivedInputRegister, this, &IntegrationPluginWallbe::onReceivedInputRegister);
        connect(modbusTcpMaster, &ModbusTCPMaster::receivedHoldingRegister, this, &IntegrationPluginWallbe::onReceivedHoldingRegister);
        connect(modbusTcpMaster, &ModbusTCPMaster::writeRequestExecuted, this, &IntegrationPluginWallbe::onWriteRequestExecuted);
        connect(modbusTcpMaster, &ModbusTCPMaster::writeRequestError, this, &IntegrationPluginWallbe::onWriteRequestError);
        connect(info, &ThingSetupInfo::aborted, modbusTcpMaster, &ModbusTCPMaster::deleteLater);

        connect(modbusTcpMaster, &ModbusTCPMaster::connectionStateChanged, info, [this, info, modbusTcpMaster](bool connected) {
            qCDebug(dcWallbe()) << "Modbus TCP connection changed, connected" << connected;
            if(connected) {
                m_connections.insert(info->thing(), modbusTcpMaster);
                info->finish(Thing::ThingErrorNoError);
            }
        });
        if (!modbusTcpMaster->connectDevice()) {
            qCWarning(dcWallbe()) << "Could not connect device";
            return info->finish(Thing::ThingErrorHardwareNotAvailable);
        }
    }
}


void IntegrationPluginWallbe::postSetupThing(Thing *thing)
{
    qCDebug(dcWallbe()) << "Post setup thing" << thing->name();

    if (thing->thingClassId() == wallbeEcoThingClassId){
        if (!m_pluginTimer) {
            qCDebug(dcWallbe()) << "Starting plugin timer";
            m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(10);
            connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
                foreach(Thing *thing, m_connections.keys()) {
                    update(thing);
                }
            });
        }
        thing->setStateValue(wallbeEcoConnectedStateTypeId, true);
        update(thing);
    } else {
        Q_ASSERT_X(false, "postSetupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}


void IntegrationPluginWallbe::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();
    Action action = info->action();

    ModbusTCPMaster *modbusTcpMaster = m_connections.value(thing);
    if (!modbusTcpMaster) {
        qCWarning(dcWallbe()) << "Modbus connection not available";
        info->finish(Thing::ThingErrorHardwareFailure);
        return;
    }

    if (thing->thingClassId() == wallbeEcoThingClassId){

        // check if this is the "set power" action
        if (action.actionTypeId() == wallbeEcoPowerActionTypeId) {

            // get the param value of the charging action
            bool charging =  action.param(wallbeEcoPowerActionPowerParamTypeId).value().toBool();
            qCDebug(dcWallbe) << "Start Charging button" << thing->name() << "set power to" << charging;
            QUuid requestId = modbusTcpMaster->writeCoil(m_slaveAddress, WallbeRegisterAddress::EnableCharging, charging);
            m_asyncActions.insert(requestId, info);
            connect(info, &ThingActionInfo::aborted, this, [this, requestId] {m_asyncActions.remove(requestId);});

        } else if(action.actionTypeId() == wallbeEcoMaxChargingCurrentActionTypeId){

            uint16_t current = action.param(wallbeEcoMaxChargingCurrentActionMaxChargingCurrentParamTypeId).value().toUInt();
            qCDebug(dcWallbe) << "Charging power set to" << current;
            QUuid requestId = modbusTcpMaster->writeHoldingRegister(m_slaveAddress, WallbeRegisterAddress::ChargingCurrent, current);
            m_asyncActions.insert(requestId, info);
            connect(info, &ThingActionInfo::aborted, this, [this, requestId] {m_asyncActions.remove(requestId);});

        } else {
            Q_ASSERT_X(false, "executeAction", QString("Unhandled action: %1").arg(action.actionTypeId().toString()).toUtf8());
        }
    } else {
        Q_ASSERT_X(false, "executeAction", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginWallbe::thingRemoved(Thing *thing)
{
    if (thing->thingClassId() == wallbeEcoThingClassId) {
        if (m_connections.contains(thing)) {
            ModbusTCPMaster *modbusTcpMaster = m_connections.take(thing);
            qCDebug(dcWallbe) << "Remove device" << thing->name();
            modbusTcpMaster->deleteLater();
        }
    }

    if (myThings().isEmpty()) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}

void IntegrationPluginWallbe::update(Thing *thing)
{
    ModbusTCPMaster *modbusTCPMaster = m_connections.value(thing);
    if(!modbusTCPMaster) {
        qCWarning(dcWallbe()) << "Modbus TCP connection not found for" << thing->name();
        return;
    }
    modbusTCPMaster->readInputRegister(m_slaveAddress, WallbeRegisterAddress::EVStatus, 1);
    modbusTCPMaster->readInputRegister(m_slaveAddress, WallbeRegisterAddress::FirmwareVersion, 2);
    modbusTCPMaster->readInputRegister(m_slaveAddress, WallbeRegisterAddress::ChargingTime, 2);

    modbusTCPMaster->readHoldingRegister(m_slaveAddress, WallbeRegisterAddress::ChargingCurrent, 1);
    modbusTCPMaster->readCoil(m_slaveAddress, WallbeRegisterAddress::EnableCharging, 1);
}

void IntegrationPluginWallbe::onConnectionStateChanged(bool status)
{
    ModbusTCPMaster *modbusTCPMaster = static_cast<ModbusTCPMaster *>(sender());
    Thing *thing = m_connections.key(modbusTCPMaster);
    if (!thing)
        return;
    thing->setStateValue(wallbeEcoConnectedStateTypeId, status);
}

void IntegrationPluginWallbe::onReceivedInputRegister(int slaveAddress, int modbusRegister, const QVector<quint16> &value)
{
    Q_UNUSED(slaveAddress)
    ModbusTCPMaster *modbusTCPMaster = static_cast<ModbusTCPMaster *>(sender());
    Thing *thing = m_connections.key(modbusTCPMaster);
    if (!thing)
        return;
    if (WallbeRegisterAddress(modbusRegister) == WallbeRegisterAddress::EVStatus) {
        //EV state - 16 bit ASCII (8bit)
        switch (value[0]) {
        case 65:
            thing->setStateValue(wallbeEcoEvStatusStateTypeId, "A - No car plugged in");
            break;
        case 66:
            thing->setStateValue(wallbeEcoEvStatusStateTypeId, "B - Supply equipment not yet ready");
            break;
        case 67:
            thing->setStateValue(wallbeEcoEvStatusStateTypeId, "C - Ready to charge");
            break;
        case 68:
            thing->setStateValue(wallbeEcoEvStatusStateTypeId, "D - Ready to charge, ventilation needed");
            break;
        case 69:
            thing->setStateValue(wallbeEcoEvStatusStateTypeId, "E - Short circuit detected");
            break;
        case 70:
            thing->setStateValue(wallbeEcoEvStatusStateTypeId, "F - Supply equipment not available");
            break;
        default:
            thing->setStateValue(wallbeEcoEvStatusStateTypeId, "F - Supply equipment not available");
        }
    } else if (WallbeRegisterAddress(modbusRegister) == WallbeRegisterAddress::ChargingTime) {
        // Extract Input Register 102 - load time - 32bit integer

        if (value.length() >= 2) {
            int minutes = (((uint32_t)(value[1]<<16)|(uint32_t)(value[0]))/60); //Converts to minutes
            qCDebug(dcWallbe()) << "    - Charging time:"  << minutes << "[min]";
            thing->setStateValue(wallbeEcoChargeTimeStateTypeId, minutes);
        }
    } else if (WallbeRegisterAddress(modbusRegister) == WallbeRegisterAddress::FirmwareVersion) {
        int firmware = (uint32_t)(value[1]<<16)|(uint32_t)(value[0]);
        uint major = firmware/10000;
        uint minor = (firmware%10000)/100;
        uint patch = firmware%100;
        QString firmwarestring = QString::number(major)+'.'+QString::number(minor)+'.'+QString::number(patch);
        thing->setStateValue(wallbeEcoFirmwareVersionStateTypeId, firmwarestring);
    }
}

void IntegrationPluginWallbe::onReceivedCoil(int slaveAddress, int modbusRegister, const QVector<quint16> &value)
{
    Q_UNUSED(slaveAddress)
    ModbusTCPMaster *modbusTCPMaster = static_cast<ModbusTCPMaster *>(sender());
    Thing *thing = m_connections.key(modbusTCPMaster);
    if (!thing)
        return;
    if (WallbeRegisterAddress(modbusRegister) == WallbeRegisterAddress::EnableCharging) {
        qCDebug(dcWallbe()) << "    - Enable charging:" << (value[0] != 0);
        thing->setStateValue(wallbeEcoPowerStateTypeId, (value[0] != 0));
    }
}

void IntegrationPluginWallbe::onReceivedHoldingRegister(int slaveAddress, int modbusRegister, const QVector<quint16> &value)
{
    Q_UNUSED(slaveAddress)
    ModbusTCPMaster *modbusTCPMaster = static_cast<ModbusTCPMaster *>(sender());
    Thing *thing = m_connections.key(modbusTCPMaster);
    if (!thing)
        return;

    switch (WallbeRegisterAddress(modbusRegister)) {

    case WallbeRegisterAddress::ChargingCurrent: {
        qCDebug(dcWallbe()) << "    - Charging current:" << value[0] << "[A]";
        thing->setStateValue(wallbeEcoMaxChargingCurrentStateTypeId, value[0]);
    } break;
    case WallbeRegisterAddress::ErrorCode: {
        qCDebug(dcWallbe()) << "Received Error Code modbus register" << value[0];
        break;
    }
    default:
        break;
    }
}

void IntegrationPluginWallbe::onWriteRequestExecuted(const QUuid &requestId, bool success)
{
    if (m_asyncActions.contains(requestId)) {
        qCDebug(dcWallbe()) << "Write request executed" << requestId << success;
        ThingActionInfo *info = m_asyncActions.value(requestId);
        if (success) {
            info->finish(Thing::ThingErrorNoError);
        } else {
            info->finish(Thing::ThingErrorHardwareFailure);
        }
    }
}

void IntegrationPluginWallbe::onWriteRequestError(const QUuid &requestId, const QString &error)
{
    Q_UNUSED(requestId)
    qCWarning(dcWallbe()) << "Could not execute write request" << error;
}
