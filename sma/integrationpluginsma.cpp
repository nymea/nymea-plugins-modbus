/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2023, nymea GmbH
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

#include "integrationpluginsma.h"
#include "plugininfo.h"

#include "sma.h"
#include "speedwire/speedwirediscovery.h"
#include "sunnywebbox/sunnywebboxdiscovery.h"
#include "modbus/smamodbusdiscovery.h"

#include <network/networkdevicediscovery.h>

IntegrationPluginSma::IntegrationPluginSma()
{

}

void IntegrationPluginSma::init()
{

}

void IntegrationPluginSma::discoverThings(ThingDiscoveryInfo *info)
{
    if (info->thingClassId() == sunnyWebBoxThingClassId) {
        if (!hardwareManager()->networkDeviceDiscovery()->available()) {
            qCWarning(dcSma()) << "Failed to discover network devices. The network device discovery is not available.";
            info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("Unable to discover devices in your network."));
            return;
        }

        qCDebug(dcSma()) << "Starting Sunny WebBox discovery...";
        SunnyWebBoxDiscovery *webBoxDiscovery = new SunnyWebBoxDiscovery(hardwareManager()->networkManager(), hardwareManager()->networkDeviceDiscovery(), info);
        connect(webBoxDiscovery, &SunnyWebBoxDiscovery::discoveryFinished, this, [=](){
            webBoxDiscovery->deleteLater();
            ThingDescriptors descriptors;
            foreach (const NetworkDeviceInfo &networkDeviceInfo, webBoxDiscovery->discoveryResults()) {
                QString title = "SMA Sunny WebBox (" + networkDeviceInfo.address().toString() + ")";
                QString description;
                if (networkDeviceInfo.macAddressManufacturer().isEmpty()) {
                    description = networkDeviceInfo.macAddress();
                } else {
                    description = networkDeviceInfo.macAddress() + " (" + networkDeviceInfo.macAddressManufacturer() + ")";
                }

                ThingDescriptor descriptor(sunnyWebBoxThingClassId, title, description);

                // Check for reconfiguration
                foreach (Thing *existingThing, myThings()) {
                    if (existingThing->paramValue(sunnyWebBoxThingMacAddressParamTypeId).toString() == networkDeviceInfo.macAddress()) {
                        descriptor.setThingId(existingThing->id());
                        break;
                    }
                }

                ParamList params;
                params << Param(sunnyWebBoxThingHostParamTypeId, networkDeviceInfo.address().toString());
                params << Param(sunnyWebBoxThingMacAddressParamTypeId, networkDeviceInfo.macAddress());
                descriptor.setParams(params);
                descriptors.append(descriptor);
            }
            info->addThingDescriptors(descriptors);
            info->finish(Thing::ThingErrorNoError);
        });

        webBoxDiscovery->startDiscovery();

    } else if (info->thingClassId() == speedwireMeterThingClassId) {

        // Note: does not require the network device discovery...
        SpeedwireDiscovery *speedwireDiscovery = new SpeedwireDiscovery(hardwareManager()->networkDeviceDiscovery(), info);
        if (!speedwireDiscovery->initialize(SpeedwireInterface::DeviceTypeMeter)) {
            qCWarning(dcSma()) << "Could not discovery inverter. The speedwire interface initialization failed.";
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("Unable to discover the network."));
            return;
        }

        connect(speedwireDiscovery, &SpeedwireDiscovery::discoveryFinished, this, [=](){
            qCDebug(dcSma()) << "Speed wire discovery finished.";
            speedwireDiscovery->deleteLater();

            ThingDescriptors descriptors;
            foreach (const SpeedwireDiscovery::SpeedwireDiscoveryResult &result, speedwireDiscovery->discoveryResult()) {

                if (result.deviceType != SpeedwireInterface::DeviceTypeMeter)
                    continue;

                if (result.serialNumber == 0)
                    continue;

                ThingDescriptor descriptor(speedwireMeterThingClassId, "SMA Energy Meter (" + QString::number(result.serialNumber) + ")" , result.address.toString());
                // We found an energy meter, let's check if we already added this one
                foreach (Thing *existingThing, myThings()) {
                    if (existingThing->paramValue(speedwireMeterThingSerialNumberParamTypeId).toUInt() == result.serialNumber) {
                        descriptor.setThingId(existingThing->id());
                        break;
                    }
                }

                ParamList params;
                params << Param(speedwireMeterThingSerialNumberParamTypeId, result.serialNumber);
                params << Param(speedwireMeterThingModelIdParamTypeId, result.modelId);
                descriptor.setParams(params);
                descriptors.append(descriptor);
            }

            info->addThingDescriptors(descriptors);
            info->finish(Thing::ThingErrorNoError);
        });

        speedwireDiscovery->startDiscovery();

    } else if (info->thingClassId() == speedwireInverterThingClassId) {
        if (!hardwareManager()->networkDeviceDiscovery()->available()) {
            qCWarning(dcSma()) << "Failed to discover network devices. The network device discovery is not available.";
            info->finish(Thing::ThingErrorHardwareNotAvailable, QT_TR_NOOP("Unable to discover devices in your network."));
            return;
        }

        SpeedwireDiscovery *speedwireDiscovery = new SpeedwireDiscovery(hardwareManager()->networkDeviceDiscovery(), info);
        if (!speedwireDiscovery->initialize(SpeedwireInterface::DeviceTypeInverter)) {
            qCWarning(dcSma()) << "Could not discovery inverter. The speedwire interface initialization failed.";
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("Unable to discover the network."));
            return;
        }

        connect(speedwireDiscovery, &SpeedwireDiscovery::discoveryFinished, this, [=](){
            qCDebug(dcSma()) << "Speed wire discovery finished.";
            speedwireDiscovery->deleteLater();

            ThingDescriptors descriptors;
            foreach (const SpeedwireDiscovery::SpeedwireDiscoveryResult &result, speedwireDiscovery->discoveryResult()) {

                if (result.deviceType != SpeedwireInterface::DeviceTypeInverter)
                    continue;

                if (result.serialNumber == 0)
                    continue;

                ThingDescriptor descriptor(speedwireInverterThingClassId, "SMA inverter (" + QString::number(result.serialNumber) + ")", result.address.toString());
                // We found an inverter, let's check if we already added this one
                foreach (Thing *existingThing, myThings()) {
                    if (existingThing->paramValue(speedwireInverterThingSerialNumberParamTypeId).toUInt() == result.serialNumber) {
                        descriptor.setThingId(existingThing->id());
                        break;
                    }
                }

                ParamList params;
                params << Param(speedwireInverterThingHostParamTypeId, result.address.toString());
                params << Param(speedwireInverterThingMacAddressParamTypeId, result.networkDeviceInfo.macAddress());
                params << Param(speedwireInverterThingSerialNumberParamTypeId, result.serialNumber);
                params << Param(speedwireInverterThingModelIdParamTypeId, result.modelId);
                descriptor.setParams(params);
                descriptors.append(descriptor);
            }

            info->addThingDescriptors(descriptors);
            info->finish(Thing::ThingErrorNoError);
        });

        speedwireDiscovery->startDiscovery();

    } else if (info->thingClassId() == modbusInverterThingClassId) {
        if (!hardwareManager()->networkDeviceDiscovery()->available()) {
            qCWarning(dcSma()) << "The network discovery is not available on this platform.";
            info->finish(Thing::ThingErrorUnsupportedFeature, QT_TR_NOOP("The network device discovery is not available."));
            return;
        }

        // Create a discovery with the info as parent for auto deleting the object once the discovery info is done
        SmaModbusDiscovery *discovery = new SmaModbusDiscovery(hardwareManager()->networkDeviceDiscovery(), 502, 3, info);
        connect(discovery, &SmaModbusDiscovery::discoveryFinished, info, [=](){
            foreach (const SmaModbusDiscovery::SmaModbusDiscoveryResult &result, discovery->discoveryResults()) {

                ThingDescriptor descriptor(modbusInverterThingClassId, "SMA inverter " + result.productName, QT_TR_NOOP("Serial: ") + result.serialNumber + " (" + result.networkDeviceInfo.address().toString() + ")");
                qCDebug(dcSma()) << "Discovered:" << descriptor.title() << descriptor.description();

                // Note: use the serial and not the mac address as identifier because more than one inverter might be behind a network device
                Things existingThings = myThings().filterByParam(modbusInverterThingSerialNumberParamTypeId, result.serialNumber);
                if (existingThings.count() == 1) {
                    qCDebug(dcSma()) << "This SMA inverter already exists in the system:" << result.serialNumber;
                    descriptor.setThingId(existingThings.first()->id());
                }

                ParamList params;
                params << Param(modbusInverterThingMacAddressParamTypeId, result.networkDeviceInfo.macAddress());
                params << Param(modbusInverterThingPortParamTypeId, result.port);
                params << Param(modbusInverterThingSlaveIdParamTypeId, result.modbusAddress);
                params << Param(modbusInverterThingSerialNumberParamTypeId, result.serialNumber);
                descriptor.setParams(params);
                info->addThingDescriptor(descriptor);
            }

            info->finish(Thing::ThingErrorNoError);
        });

        // Start the discovery process
        discovery->startDiscovery();
    }
}

void IntegrationPluginSma::startPairing(ThingPairingInfo *info)
{
    info->finish(Thing::ThingErrorNoError, QT_TR_NOOP("Please enter the password of your inverter. If no password has been explicitly set, leave it empty to use the default password for SMA inverters."));
}

void IntegrationPluginSma::confirmPairing(ThingPairingInfo *info, const QString &username, const QString &secret)
{
    Q_UNUSED(username)

    if (info->thingClassId() == speedwireInverterThingClassId) {
        // On speedwire the password length has a maximum of 12 characters
        if (secret.count() > 12) {
            info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The password can not be longer than 12 characters."));
            return;
        }

        // Init with the default password
        QString password = "0000";
        if (!secret.isEmpty()) {
            qCDebug(dcSma()) << "Pairing: Using password" << secret;
            password = secret;
        } else {
            qCDebug(dcSma()) << "Pairing: The given password is empty. Using default password" << password;
        }

        // Just store details, we'll test the login in setupDevice
        pluginStorage()->beginGroup(info->thingId().toString());
        pluginStorage()->setValue("password", password);
        pluginStorage()->endGroup();

        info->finish(Thing::ThingErrorNoError);
    }
}

void IntegrationPluginSma::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCDebug(dcSma()) << "Setup thing" << thing << thing->params();

    if (thing->thingClassId() == sunnyWebBoxThingClassId) {
        // Check if a Sunny WebBox is already added with this mac address
        foreach (SunnyWebBox *sunnyWebBox, m_sunnyWebBoxes.values()) {
            if (sunnyWebBox->macAddress() == thing->paramValue(sunnyWebBoxThingMacAddressParamTypeId).toString()){
                qCWarning(dcSma()) << "Thing with mac address" << thing->paramValue(sunnyWebBoxThingMacAddressParamTypeId).toString() << " already added!";
                info->finish(Thing::ThingErrorThingInUse);
                return;
            }
        }

        if (m_sunnyWebBoxes.contains(thing)) {
            qCDebug(dcSma()) << "Setup after reconfiguration, cleaning up...";
            m_sunnyWebBoxes.take(thing)->deleteLater();
        }

        SunnyWebBox *sunnyWebBox = new SunnyWebBox(hardwareManager()->networkManager(), QHostAddress(thing->paramValue(sunnyWebBoxThingHostParamTypeId).toString()), this);
        sunnyWebBox->setMacAddress(thing->paramValue(sunnyWebBoxThingMacAddressParamTypeId).toString());

        connect(info, &ThingSetupInfo::aborted, sunnyWebBox, &SunnyWebBox::deleteLater);
        connect(sunnyWebBox, &SunnyWebBox::destroyed, this, [thing, this] { m_sunnyWebBoxes.remove(thing);});

        QString requestId = sunnyWebBox->getPlantOverview();
        connect(sunnyWebBox, &SunnyWebBox::plantOverviewReceived, info, [=] (const QString &messageId, SunnyWebBox::Overview overview) {
            qCDebug(dcSma()) << "Received plant overview" << messageId << "finish setup";
            info->finish(Thing::ThingErrorNoError);
            connect(sunnyWebBox, &SunnyWebBox::connectedChanged, this, &IntegrationPluginSma::onConnectedChanged);
            connect(sunnyWebBox, &SunnyWebBox::plantOverviewReceived, this, &IntegrationPluginSma::onPlantOverviewReceived);
            m_sunnyWebBoxes.insert(info->thing(), sunnyWebBox);
            onPlantOverviewReceived(messageId, overview);
        });

    } else if (thing->thingClassId() == speedwireMeterThingClassId) {

        // Create the multicast interface if not created already.
        if (!m_multicastInterface)
            m_multicastInterface = new SpeedwireInterface(true, this);

        quint32 serialNumber = static_cast<quint32>(thing->paramValue(speedwireMeterThingSerialNumberParamTypeId).toUInt());
        quint16 modelId = static_cast<quint16>(thing->paramValue(speedwireMeterThingModelIdParamTypeId).toUInt());

        // Handle reconfigure
        if (m_speedwireMeters.contains(thing))
            m_speedwireMeters.take(thing)->deleteLater();

        SpeedwireMeter *meter = new SpeedwireMeter(m_multicastInterface, modelId, serialNumber, this);
        if (!meter->initialize()) {
            meter->deleteLater();
            qCWarning(dcSma()) << "Setup failed. Could not initialize meter interface.";
            info->finish(Thing::ThingErrorHardwareFailure);
            return;
        }

        connect(meter, &SpeedwireMeter::reachableChanged, thing, [=](bool reachable){
            thing->setStateValue(speedwireMeterConnectedStateTypeId, reachable);
            if (!reachable) {
                markSpeedwireMeterAsDisconnected(thing);
            }
        });

        connect(meter, &SpeedwireMeter::valuesUpdated, thing, [=](){
            thing->setStateValue(speedwireMeterConnectedStateTypeId, true);
            thing->setStateValue(speedwireMeterCurrentPowerStateTypeId, meter->currentPower());
            thing->setStateValue(speedwireMeterCurrentPowerPhaseAStateTypeId, meter->currentPowerPhaseA());
            thing->setStateValue(speedwireMeterCurrentPowerPhaseBStateTypeId, meter->currentPowerPhaseB());
            thing->setStateValue(speedwireMeterCurrentPowerPhaseCStateTypeId, meter->currentPowerPhaseC());
            thing->setStateValue(speedwireMeterVoltagePhaseAStateTypeId, meter->voltagePhaseA());
            thing->setStateValue(speedwireMeterVoltagePhaseBStateTypeId, meter->voltagePhaseB());
            thing->setStateValue(speedwireMeterVoltagePhaseCStateTypeId, meter->voltagePhaseC());
            thing->setStateValue(speedwireMeterTotalEnergyConsumedStateTypeId, meter->totalEnergyConsumed());
            thing->setStateValue(speedwireMeterTotalEnergyProducedStateTypeId, meter->totalEnergyProduced());
            thing->setStateValue(speedwireMeterEnergyConsumedPhaseAStateTypeId, meter->energyConsumedPhaseA());
            thing->setStateValue(speedwireMeterEnergyConsumedPhaseBStateTypeId, meter->energyConsumedPhaseB());
            thing->setStateValue(speedwireMeterEnergyConsumedPhaseCStateTypeId, meter->energyConsumedPhaseC());
            thing->setStateValue(speedwireMeterEnergyProducedPhaseAStateTypeId, meter->energyProducedPhaseA());
            thing->setStateValue(speedwireMeterEnergyProducedPhaseBStateTypeId, meter->energyProducedPhaseB());
            thing->setStateValue(speedwireMeterEnergyProducedPhaseCStateTypeId, meter->energyProducedPhaseC());
            thing->setStateValue(speedwireMeterCurrentPhaseAStateTypeId, meter->amperePhaseA());
            thing->setStateValue(speedwireMeterCurrentPhaseBStateTypeId, meter->amperePhaseB());
            thing->setStateValue(speedwireMeterCurrentPhaseCStateTypeId, meter->amperePhaseC());
            thing->setStateValue(speedwireMeterFirmwareVersionStateTypeId, meter->softwareVersion());
        });

        m_speedwireMeters.insert(thing, meter);
        info->finish(Thing::ThingErrorNoError);

    } else if (thing->thingClassId() == speedwireInverterThingClassId) {

        QHostAddress address = QHostAddress(thing->paramValue(speedwireInverterThingHostParamTypeId).toString());
        quint32 serialNumber = static_cast<quint32>(thing->paramValue(speedwireInverterThingSerialNumberParamTypeId).toUInt());
        quint16 modelId = static_cast<quint16>(thing->paramValue(speedwireInverterThingModelIdParamTypeId).toUInt());

        if (m_speedwireInverters.contains(thing)) {
            m_speedwireInverters.take(thing)->deleteLater();
        }

        SpeedwireInverter *inverter = new SpeedwireInverter(address, modelId, serialNumber, this);
        if (!inverter->initialize()) {
            qCWarning(dcSma()) << "Setup failed. Could not initialize inverter interface.";
            info->finish(Thing::ThingErrorHardwareFailure);
            return;
        }

        qCDebug(dcSma()) << "Inverter: Interface initialized successfully.";

        QString password;
        pluginStorage()->beginGroup(info->thing()->id().toString());
        password = pluginStorage()->value("password", "0000").toString();
        pluginStorage()->endGroup();

        // Connection exists only as long info exists
        connect(inverter, &SpeedwireInverter::loginFinished, info, [=](bool success){
            if (!success) {
                qCWarning(dcSma()) << "Failed to set up inverter. Wrong password.";

                // Remove invalid password from settings
                pluginStorage()->beginGroup(info->thing()->id().toString());
                pluginStorage()->remove("");
                pluginStorage()->endGroup();

                delete inverter;
                info->finish(Thing::ThingErrorAuthenticationFailure, QT_TR_NOOP("Failed to log in with the given password. Please try again."));
                return;
            }

            qCDebug(dcSma()) << "Inverter set up successfully.";
            m_speedwireInverters.insert(thing, inverter);
            info->finish(Thing::ThingErrorNoError);
            // Note: the data is already refreshing here
        });

        // Make sure an aborted setup will clean up the object
        connect(info, &ThingSetupInfo::aborted, inverter, &SpeedwireInverter::deleteLater);

        // Runtime connections
        connect(inverter, &SpeedwireInverter::reachableChanged, thing, [=](bool reachable){
            thing->setStateValue(speedwireInverterConnectedStateTypeId, reachable);
            if (!reachable) {
                markSpeedwireInverterAsDisconnected(thing);
            }
        });

        connect(inverter, &SpeedwireInverter::valuesUpdated, thing, [=](){
            thing->setStateValue(speedwireInverterConnectedStateTypeId, true);
            thing->setStateValue(speedwireInverterTotalEnergyProducedStateTypeId, inverter->totalEnergyProduced());
            thing->setStateValue(speedwireInverterEnergyProducedTodayStateTypeId, inverter->todayEnergyProduced());
            thing->setStateValue(speedwireInverterCurrentPowerStateTypeId, -inverter->totalAcPower());
            thing->setStateValue(speedwireInverterFrequencyStateTypeId, inverter->gridFrequency());

            thing->setStateValue(speedwireInverterVoltagePhaseAStateTypeId, inverter->voltageAcPhase1());
            thing->setStateValue(speedwireInverterVoltagePhaseBStateTypeId, inverter->voltageAcPhase2());
            thing->setStateValue(speedwireInverterVoltagePhaseCStateTypeId, inverter->voltageAcPhase3());

            thing->setStateValue(speedwireInverterCurrentPhaseAStateTypeId, inverter->currentAcPhase1());
            thing->setStateValue(speedwireInverterCurrentPhaseBStateTypeId, inverter->currentAcPhase2());
            thing->setStateValue(speedwireInverterCurrentPhaseCStateTypeId, inverter->currentAcPhase3());

            thing->setStateValue(speedwireInverterCurrentPowerMpp1StateTypeId, inverter->powerDcMpp1());
            thing->setStateValue(speedwireInverterCurrentPowerMpp2StateTypeId, inverter->powerDcMpp2());
        });

        qCDebug(dcSma()) << "Inverter: Start connecting using password" << password;
        inverter->startConnecting(password);

    } else if (thing->thingClassId() == modbusInverterThingClassId) {

        // Handle reconfigure
        if (m_modbusInverters.contains(thing)) {
            qCDebug(dcSma()) << "Reconfiguring existing thing" << thing->name();
            m_modbusInverters.take(thing)->deleteLater();

            if (m_monitors.contains(thing)) {
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        }

        MacAddress macAddress = MacAddress(thing->paramValue(modbusInverterThingMacAddressParamTypeId).toString());
        if (!macAddress.isValid()) {
            qCWarning(dcSma()) << "The configured mac address is not valid" << thing->params();
            info->finish(Thing::ThingErrorInvalidParameter, QT_TR_NOOP("The MAC address is not known. Please reconfigure the thing."));
            return;
        }

        // Create the monitor
        NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(macAddress);
        m_monitors.insert(thing, monitor);

        QHostAddress address = monitor->networkDeviceInfo().address();
        if (address.isNull()) {
            qCWarning(dcSma()) << "Cannot set up sma modbus inverter. The host address is not known yet. Maybe it will be available in the next run...";
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("The host address is not known yet. Trying again later."));
            return;
        }

        // Clean up in case the setup gets aborted
        connect(info, &ThingSetupInfo::aborted, monitor, [=](){
            if (m_monitors.contains(thing)) {
                qCDebug(dcSma()) << "Unregister monitor because setup has been aborted.";
                hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
            }
        });

        // Wait for the monitor to be ready
        if (monitor->reachable()) {
            // Thing already reachable...let's continue with the setup
            setupModbusInverterConnection(info);
        } else {
            qCDebug(dcSma()) << "Waiting for the network monitor to get reachable before continue to set up the connection" << thing->name() << address.toString() << "...";
            connect(monitor, &NetworkDeviceMonitor::reachableChanged, info, [=](bool reachable){
                if (reachable) {
                    qCDebug(dcSma()) << "The monitor for thing setup" << thing->name() << "is now reachable. Continue setup...";
                    setupModbusInverterConnection(info);
                }
            });
        }

    } else {
        Q_ASSERT_X(false, "setupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginSma::postSetupThing(Thing *thing)
{
    qCDebug(dcSma()) << "Post setup thing" << thing->name();
    if (thing->thingClassId() == sunnyWebBoxThingClassId) {
        SunnyWebBox *sunnyWebBox = m_sunnyWebBoxes.value(thing);
        if (!sunnyWebBox)
            return;

        sunnyWebBox->getPlantOverview();
        thing->setStateValue("connected", true);
        setupRefreshTimer();

    } else if (thing->thingClassId() == speedwireInverterThingClassId) {
        SpeedwireInverter *inverter = m_speedwireInverters.value(thing);
        if (inverter) {
            thing->setStateValue("connected", inverter->reachable());
            if (!inverter->reachable()) {
                markSpeedwireInverterAsDisconnected(thing);
            }
        } else {
            thing->setStateValue("connected", false);
            markSpeedwireInverterAsDisconnected(thing);
        }

        setupRefreshTimer();

    } else if (thing->thingClassId() == modbusInverterThingClassId) {
        SmaInverterModbusTcpConnection *connection = m_modbusInverters.value(thing);
        if (connection) {
            thing->setStateValue("connected", connection->reachable());
            if (!connection->reachable()) {
                markSpeedwireMeterAsDisconnected(thing);
            }
        } else {
            thing->setStateValue("connected", false);
            markSpeedwireMeterAsDisconnected(thing);
        }

        setupRefreshTimer();
    }
}

void IntegrationPluginSma::thingRemoved(Thing *thing)
{
    if (thing->thingClassId() == sunnyWebBoxThingClassId) {
        m_sunnyWebBoxes.take(thing)->deleteLater();
    }

    if (thing->thingClassId() == speedwireMeterThingClassId && m_speedwireMeters.contains(thing)) {
        m_speedwireMeters.take(thing)->deleteLater();
    }

    if (thing->thingClassId() == speedwireInverterThingClassId && m_speedwireInverters.contains(thing)) {
        m_speedwireInverters.take(thing)->deleteLater();
    }

    if (thing->thingClassId() == modbusInverterThingClassId && m_modbusInverters.contains(thing)) {
        m_modbusInverters.take(thing)->deleteLater();
    }

    if (m_monitors.contains(thing)) {
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
    }

    if (myThings().filterByThingClassId(speedwireMeterThingClassId).isEmpty() && m_multicastInterface) {
        // Delete shared multicast socket...
        m_multicastInterface->deleteLater();
        m_multicastInterface = nullptr;
    }

    if (myThings().isEmpty()) {
        qCDebug(dcSma()) << "Stopping timer";
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_refreshTimer);
        m_refreshTimer = nullptr;
    }
}

void IntegrationPluginSma::onConnectedChanged(bool connected)
{
    Thing *thing = m_sunnyWebBoxes.key(static_cast<SunnyWebBox *>(sender()));
    if (!thing)
        return;

    if (!connected)
        thing->setStateValue(sunnyWebBoxCurrentPowerStateTypeId, 0);

    thing->setStateValue(sunnyWebBoxConnectedStateTypeId, connected);
}

void IntegrationPluginSma::onPlantOverviewReceived(const QString &messageId, SunnyWebBox::Overview overview)
{
    Q_UNUSED(messageId)
    Thing *thing = m_sunnyWebBoxes.key(static_cast<SunnyWebBox *>(sender()));
    if (!thing)
        return;

    qCDebug(dcSma()) << "Plant overview received for" << thing;
    thing->setStateValue(sunnyWebBoxCurrentPowerStateTypeId, -overview.power);
    thing->setStateValue(sunnyWebBoxDayEnergyProducedStateTypeId, overview.dailyYield);
    thing->setStateValue(sunnyWebBoxTotalEnergyProducedStateTypeId, overview.totalYield);
    thing->setStateValue(sunnyWebBoxModeStateTypeId, overview.status);
    if (!overview.error.isEmpty()){
        qCDebug(dcSma()) << "Received error" << overview.error;
        thing->setStateValue(sunnyWebBoxErrorStateTypeId, overview.error);
    }
}

void IntegrationPluginSma::setupRefreshTimer()
{
    // If already set up...
    if (m_refreshTimer)
        return;

    m_refreshTimer = hardwareManager()->pluginTimerManager()->registerTimer(5);
    connect(m_refreshTimer, &PluginTimer::timeout, this, [=](){
        foreach (SunnyWebBox *webbox, m_sunnyWebBoxes) {
            // Max refresh rate according to docs should be 30 seconds, will be handled in the webbox class
            webbox->getPlantOverview();
        }

        foreach (SpeedwireInverter *inverter, m_speedwireInverters) {
            // Note: refresh will not be triggered if there is already a refresh process running
            inverter->refresh();
        }

        foreach (SmaInverterModbusTcpConnection *connection, m_modbusInverters) {
            connection->update();
        }
    });

    m_refreshTimer->start();
}

void IntegrationPluginSma::setupModbusInverterConnection(ThingSetupInfo *info)
{
    Thing *thing = info->thing();

    QHostAddress address = m_monitors.value(thing)->networkDeviceInfo().address();
    uint port = thing->paramValue(modbusInverterThingPortParamTypeId).toUInt();
    quint16 slaveId = thing->paramValue(modbusInverterThingSlaveIdParamTypeId).toUInt();

    qCDebug(dcSma()) << "Setting up SMA inverter on" << address.toString() << port << "unit ID:" << slaveId;
    SmaInverterModbusTcpConnection *connection = new SmaInverterModbusTcpConnection(address, port, slaveId, this);
    connect(info, &ThingSetupInfo::aborted, connection, &SmaInverterModbusTcpConnection::deleteLater);

    // Reconnect on monitor reachable changed
    NetworkDeviceMonitor *monitor = m_monitors.value(thing);
    connect(monitor, &NetworkDeviceMonitor::reachableChanged, thing, [=](bool reachable){
        qCDebug(dcSma()) << "Network device monitor reachable changed for" << thing->name() << reachable;
        if (!thing->setupComplete())
            return;

        if (reachable && !thing->stateValue("connected").toBool()) {
            connection->setHostAddress(monitor->networkDeviceInfo().address());
            connection->connectDevice();
        } else if (!reachable) {
            // Note: We disable autoreconnect explicitly and we will
            // connect the device once the monitor says it is reachable again
            connection->disconnectDevice();
            markModbusInverterAsDisconnected(thing);
        }
    });

    connect(connection, &SmaInverterModbusTcpConnection::reachableChanged, thing, [this, thing, connection](bool reachable){
        qCDebug(dcSma()) << "Reachable changed to" << reachable << "for" << thing;
        if (reachable) {
            // Connected true will be set after successfull init
            connection->initialize();
        } else {
            thing->setStateValue("connected", false);
            markModbusInverterAsDisconnected(thing);
            foreach (Thing *childThing, myThings().filterByParentId(thing->id())) {
                childThing->setStateValue("connected", false);
            }
        }
    });

    connect(connection, &SmaInverterModbusTcpConnection::initializationFinished, thing, [=](bool success){
        if (!thing->setupComplete())
            return;

        thing->setStateValue("connected", success);
        foreach (Thing *childThing, myThings().filterByParentId(thing->id())) {
            childThing->setStateValue("connected", success);
        }

        if (!success) {
            // Try once to reconnect the device
            connection->reconnectDevice();
            markModbusInverterAsDisconnected(thing);
        }
    });

    connect(connection, &SmaInverterModbusTcpConnection::initializationFinished, info, [=](bool success){
        if (!success) {
            qCWarning(dcSma()) << "Connection init finished with errors" << thing->name() << connection->hostAddress().toString();
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(monitor);
            connection->deleteLater();
            info->finish(Thing::ThingErrorHardwareFailure, QT_TR_NOOP("Could not initialize the communication with the inverter."));
            return;
        }

        qCDebug(dcSma()) << "Connection init finished successfully" << connection;
        m_modbusInverters.insert(thing, connection);
        info->finish(Thing::ThingErrorNoError);

        // Set connected true
        thing->setStateValue("connected", true);
        foreach (Thing *childThing, myThings().filterByParentId(thing->id())) {
            childThing->setStateValue("connected", true);
        }

        connect(connection, &SmaInverterModbusTcpConnection::updateFinished, thing, [=](){
            qCDebug(dcSma()) << "Updated" << connection;

            // Grid voltage
            if (isModbusValueValid(connection->gridVoltagePhaseA()))
                thing->setStateValue(modbusInverterVoltagePhaseAStateTypeId, connection->gridVoltagePhaseA() / 100.0);

            if (isModbusValueValid(connection->gridVoltagePhaseB()))
                thing->setStateValue(modbusInverterVoltagePhaseBStateTypeId, connection->gridVoltagePhaseB() / 100.0);

            if (isModbusValueValid(connection->gridVoltagePhaseC()))
                thing->setStateValue(modbusInverterVoltagePhaseCStateTypeId, connection->gridVoltagePhaseC() / 100.0);

            // Grid current
            if (isModbusValueValid(connection->gridCurrentPhaseA()))
                thing->setStateValue(modbusInverterCurrentPhaseAStateTypeId, connection->gridCurrentPhaseA() / 1000.0);

            if (isModbusValueValid(connection->gridCurrentPhaseB()))
                thing->setStateValue(modbusInverterCurrentPhaseBStateTypeId, connection->gridCurrentPhaseB() / 1000.0);

            if (isModbusValueValid(connection->gridCurrentPhaseC()))
                thing->setStateValue(modbusInverterCurrentPhaseCStateTypeId, connection->gridCurrentPhaseC() / 1000.0);

            // Phase power
            if (isModbusValueValid(connection->currentPowerPhaseA()))
                thing->setStateValue(modbusInverterCurrentPowerPhaseAStateTypeId, connection->currentPowerPhaseA());

            if (isModbusValueValid(connection->currentPowerPhaseB()))
                thing->setStateValue(modbusInverterCurrentPowerPhaseBStateTypeId, connection->currentPowerPhaseB());

            if (isModbusValueValid(connection->currentPowerPhaseC()))
                thing->setStateValue(modbusInverterCurrentPowerPhaseCStateTypeId, connection->currentPowerPhaseC());

            // Others
            if (isModbusValueValid(connection->totalYield()))
                thing->setStateValue(modbusInverterTotalEnergyProducedStateTypeId, connection->totalYield() / 1000.0); // kWh

            if (isModbusValueValid(connection->dailyYield()))
                thing->setStateValue(modbusInverterEnergyProducedTodayStateTypeId, connection->dailyYield() / 1000.0); // kWh

            // Power
            if (isModbusValueValid(connection->currentPower()))
                thing->setStateValue(modbusInverterCurrentPowerStateTypeId, -connection->currentPower());

            // Version
            thing->setStateValue(modbusInverterFirmwareVersionStateTypeId, Sma::buildSoftwareVersionString(connection->softwarePackage()));
        });

        // Update registers
        connection->update();
    });

    connection->connectDevice();
}

void IntegrationPluginSma::markSpeedwireMeterAsDisconnected(Thing *thing)
{
    thing->setStateValue(speedwireMeterCurrentPowerPhaseAStateTypeId, 0);
    thing->setStateValue(speedwireMeterCurrentPowerPhaseBStateTypeId, 0);
    thing->setStateValue(speedwireMeterCurrentPowerPhaseCStateTypeId, 0);
    thing->setStateValue(speedwireMeterVoltagePhaseAStateTypeId, 0);
    thing->setStateValue(speedwireMeterVoltagePhaseBStateTypeId, 0);
    thing->setStateValue(speedwireMeterVoltagePhaseCStateTypeId, 0);
    thing->setStateValue(speedwireMeterCurrentPhaseAStateTypeId, 0);
    thing->setStateValue(speedwireMeterCurrentPhaseBStateTypeId, 0);
    thing->setStateValue(speedwireMeterCurrentPhaseCStateTypeId, 0);
    thing->setStateValue(speedwireMeterCurrentPowerStateTypeId, 0);
}

void IntegrationPluginSma::markSpeedwireInverterAsDisconnected(Thing *thing)
{
    thing->setStateValue(speedwireInverterVoltagePhaseAStateTypeId, 0);
    thing->setStateValue(speedwireInverterVoltagePhaseBStateTypeId, 0);
    thing->setStateValue(speedwireInverterVoltagePhaseCStateTypeId, 0);
    thing->setStateValue(speedwireInverterCurrentPhaseAStateTypeId, 0);
    thing->setStateValue(speedwireInverterCurrentPhaseBStateTypeId, 0);
    thing->setStateValue(speedwireInverterCurrentPhaseCStateTypeId, 0);
    thing->setStateValue(speedwireInverterCurrentPowerMpp1StateTypeId, 0);
    thing->setStateValue(speedwireInverterCurrentPowerMpp2StateTypeId, 0);
    thing->setStateValue(speedwireInverterCurrentPowerStateTypeId, 0);
}

void IntegrationPluginSma::markModbusInverterAsDisconnected(Thing *thing)
{
    thing->setStateValue(modbusInverterVoltagePhaseAStateTypeId, 0);
    thing->setStateValue(modbusInverterVoltagePhaseBStateTypeId, 0);
    thing->setStateValue(modbusInverterVoltagePhaseCStateTypeId, 0);
    thing->setStateValue(modbusInverterCurrentPhaseAStateTypeId, 0);
    thing->setStateValue(modbusInverterCurrentPhaseBStateTypeId, 0);
    thing->setStateValue(modbusInverterCurrentPhaseCStateTypeId, 0);
    thing->setStateValue(modbusInverterCurrentPowerPhaseAStateTypeId, 0);
    thing->setStateValue(modbusInverterCurrentPowerPhaseBStateTypeId, 0);
    thing->setStateValue(modbusInverterCurrentPowerPhaseCStateTypeId, 0);
    thing->setStateValue(modbusInverterCurrentPowerStateTypeId, 0);
}

bool IntegrationPluginSma::isModbusValueValid(quint32 value)
{
    return value != 0xffffffff;
}

bool IntegrationPluginSma::isModbusValueValid(qint32 value)
{
    return value != static_cast<qint32>(0x80000000);
}

bool IntegrationPluginSma::isModbusValueValid(quint64 value)
{
    return value != 0xffffffffffffffff;
}

