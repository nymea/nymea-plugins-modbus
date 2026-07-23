// SPDX-License-Identifier: GPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2026, chargebyte austria GmbH
*
* This file is part of nymea-plugins-modbus.
*
* nymea-plugins-modbus is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* nymea-plugins-modbus is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with nymea-plugins-modbus. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "integrationpluginpcelectric.h"
#include "pcelectricdiscovery.h"
#include "plugininfo.h"

#include <hardware/electricity.h>
#include <hardwaremanager.h>
#include <platform/platformzeroconfcontroller.h>
#include <network/zeroconf/zeroconfservicebrowser.h>
#include <nymeasettings.h>

#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QSaveFile>
#include <QSslCipher>
#include <QTemporaryDir>

#include <algorithm>
#include <memory>

namespace {
constexpr int addressRetryIntervalMs = 5000;

bool certificateIsUsable(const QSslCertificate &certificate)
{
    const QDateTime now = QDateTime::currentDateTimeUtc();
    return !certificate.isNull() && certificate.effectiveDate() <= now && certificate.expiryDate() > now;
}

bool pathExists(const QString &path)
{
    const QFileInfo info(path);
    return info.exists() || info.isSymLink();
}

bool writeIdentityFile(const QString &path, const QByteArray &data)
{
    if (!QDir().mkpath(QFileInfo(path).absolutePath()))
        return false;

    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly) || file.write(data) != data.size() || !file.commit())
        return false;

    return QFile::setPermissions(path, QFileDevice::ReadOwner | QFileDevice::WriteOwner);
}

bool readFile(const QString &path, QByteArray *data)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    *data = file.readAll();
    return true;
}

QString spkiSha256Fingerprint(const QSslCertificate &certificate)
{
    return QString::fromLatin1(QCryptographicHash::hash(certificate.publicKey().toDer(),
                                                        QCryptographicHash::Sha256).toHex());
}

bool rfidOperatingModeFromSettingValue(const QString &value, EV11ModbusTcpConnection::RfidOperatingMode *mode)
{
    if (value == QStringLiteral("0 | Disabled")) {
        *mode = EV11ModbusTcpConnection::RfidOperatingModeDisabled;
    } else if (value == QStringLiteral("1 | Remote")) {
        *mode = EV11ModbusTcpConnection::RfidOperatingModeRemote;
    } else if (value == QStringLiteral("2 | Local")) {
        *mode = EV11ModbusTcpConnection::RfidOperatingModeLocal;
    } else {
        return false;
    }
    return true;
}
}

IntegrationPluginPcElectric::IntegrationPluginPcElectric() {}

void IntegrationPluginPcElectric::init()
{
    //qCCritical(dcPcElectric()) << QString("%1").arg(QString::number(49155, 2));

    m_addressParamTypes[ev11ThingClassId] = ev11ThingAddressParamTypeId;
    m_addressParamTypes[ev11RfidThingClassId] = ev11RfidThingAddressParamTypeId;
    m_addressParamTypes[ev11NoMeterThingClassId] = ev11NoMeterThingAddressParamTypeId;

    m_hostNameParamTypes[ev11ThingClassId] = ev11ThingHostNameParamTypeId;
    m_hostNameParamTypes[ev11RfidThingClassId] = ev11RfidThingHostNameParamTypeId;
    m_hostNameParamTypes[ev11NoMeterThingClassId] = ev11NoMeterThingHostNameParamTypeId;

    m_macParamTypes[ev11ThingClassId] = ev11ThingMacAddressParamTypeId;
    m_macParamTypes[ev11RfidThingClassId] = ev11RfidThingMacAddressParamTypeId;
    m_macParamTypes[ev11NoMeterThingClassId] = ev11NoMeterThingMacAddressParamTypeId;

    m_serialNumberParamTypes[ev11ThingClassId] = ev11ThingSerialNumberParamTypeId;
    m_serialNumberParamTypes[ev11RfidThingClassId] = ev11RfidThingSerialNumberParamTypeId;
    m_serialNumberParamTypes[ev11NoMeterThingClassId] = ev11NoMeterThingSerialNumberParamTypeId;

    m_modbusServiceBrowser = hardwareManager()->zeroConfController()->createServiceBrowser("_modbus._tcp");
    connect(m_modbusServiceBrowser, &ZeroConfServiceBrowser::serviceEntryAdded, this, &IntegrationPluginPcElectric::handleZeroConfServiceAdded);
    connect(m_modbusServiceBrowser, &ZeroConfServiceBrowser::serviceEntryRemoved, this, &IntegrationPluginPcElectric::handleZeroConfServiceRemoved);
    connect(this, &IntegrationPlugin::configValueChanged, this, [this](const ParamTypeId &paramTypeId) {
        if (paramTypeId == pcElectricPluginClientCertificatePathParamTypeId
            || paramTypeId == pcElectricPluginClientKeyPathParamTypeId) {
            m_clientCertificate = QSslCertificate();
            m_clientPrivateKey = QSslKey();
        }
    });
}

void IntegrationPluginPcElectric::discoverThings(ThingDiscoveryInfo *info)
{
    if (!hardwareManager()->networkDeviceDiscovery()->available() && !hardwareManager()->zeroConfController()->available()) {
        qCWarning(dcPcElectric()) << "Neither network discovery nor ZeroConf is available on this platform.";
        info->finish(Thing::ThingErrorUnsupportedFeature, QT_TR_NOOP("Network and ZeroConf discovery are not available."));
        return;
    }

    // Create a discovery with the info as parent for auto deleting the object once the discovery info is done
    PcElectricDiscovery *discovery = new PcElectricDiscovery(hardwareManager()->networkDeviceDiscovery(),
                                                             m_modbusServiceBrowser,
                                                             502,
                                                             1,
                                                             info);
    connect(discovery, &PcElectricDiscovery::discoveryFinished, info, [=]() {
        foreach (const PcElectricDiscovery::Result &result, discovery->results()) {
            if (info->thingClassId() != result.thingClassId)
                continue;

            ThingDescriptor descriptor(result.thingClassId,
                                       "PCE EV11.3 (" + result.serialNumber + ")",
                                       "Version: " + result.firmwareRevision + " - " + result.networkDeviceInfo.address().toString());
            qCInfo(dcPcElectric()) << "Discovered:" << descriptor.title() << descriptor.description();

            // Check if we already have set up this device
            Things existingThings = myThings().filterByParam(m_serialNumberParamTypes.value(result.thingClassId), result.serialNumber);
            if (existingThings.length() == 1) {
                qCDebug(dcPcElectric()) << "This PCE wallbox already exists in the system:" << result.serialNumber << result.networkDeviceInfo;
                descriptor.setThingId(existingThings.first()->id());
            }

            ParamList params;
            params << Param(m_macParamTypes.value(result.thingClassId), result.registerMacAddress.toString());
            params << Param(m_hostNameParamTypes.value(result.thingClassId), result.networkDeviceInfo.thingParamValueHostName());
            // An address explicitly configured by the user selects static mode.
            // Discovered devices stay dynamic and obtain their current address
            // independently from the MAC monitor or ZeroConf.
            params << Param(m_addressParamTypes.value(result.thingClassId), QString());
            params << Param(m_serialNumberParamTypes.value(result.thingClassId), result.serialNumber);
            // Note: if we discover also the port and modbusaddress, we must fill them in from the discovery here, for now everywhere the defaults...
            descriptor.setParams(params);
            info->addThingDescriptor(descriptor);
        }

        info->finish(Thing::ThingErrorNoError);
    });

    // Start the discovery process
    discovery->startDiscovery();
}

void IntegrationPluginPcElectric::setupThing(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    qCInfo(dcPcElectric()) << "Setup thing" << thing << thing->params();

    const QString serialNumber = thing->paramValue(m_serialNumberParamTypes.value(thing->thingClassId())).toString();
    QString previousSerialNumber = m_configuredSerialNumbers.value(thing);
    if (previousSerialNumber.isEmpty())
        previousSerialNumber = pluginStorage()->value(storagePrefix(thing) + "/configuredSerialNumber").toString();
    if (!previousSerialNumber.isEmpty() && previousSerialNumber != serialNumber) {
        qCWarning(dcPcElectric()) << "The configured serial number changed for" << thing->name()
                                  << "from" << previousSerialNumber << "to" << serialNumber
                                  << "Clearing the wallbox TLS identity.";
        pluginStorage()->remove(storagePrefix(thing) + "/serverFingerprint");
        pluginStorage()->remove(storagePrefix(thing) + "/tlsRequired");
        pluginStorage()->sync();
    }
    m_configuredSerialNumbers.insert(thing, serialNumber);

    const QString addressModeKey = storagePrefix(thing) + "/addressMode";
    const QString configuredAddressKey = storagePrefix(thing) + "/configuredAddress";
    const QString storedAddressMode = pluginStorage()->value(addressModeKey).toString();
    const QString storedConfiguredAddress = pluginStorage()->value(configuredAddressKey).toString();
    const QHostAddress configuredAddress(thing->paramValue(m_addressParamTypes.value(thing->thingClassId())).toString());
    const QString configuredAddressString = configuredAddress.toString();
    const bool hasConfiguredAddress = !configuredAddress.isNull();
    bool staticAddressMode = false;
    if (info->isReconfigure() && !storedAddressMode.isEmpty()
        && configuredAddressString == storedConfiguredAddress) {
        staticAddressMode = storedAddressMode == "static";
    } else if (info->isInitialSetup() || info->isReconfigure()) {
        staticAddressMode = hasConfiguredAddress;
    } else if (!storedAddressMode.isEmpty()) {
        staticAddressMode = storedAddressMode == "static";
    } else {
        // Before address modes were persisted, network-discovered Things
        // contained address, MAC and serial parameters. Preserve those as
        // dynamic; an address without a complete discovered identity remains
        // a manual/static configuration.
        const MacAddress configuredMac(thing->paramValue(m_macParamTypes.value(thing->thingClassId())).toString());
        staticAddressMode = hasConfiguredAddress && (serialNumber.isEmpty() || configuredMac.isNull());
    }
    m_staticAddressModes.insert(thing, staticAddressMode);
    pluginStorage()->setValue(addressModeKey, staticAddressMode ? "static" : "dynamic");
    pluginStorage()->setValue(configuredAddressKey, configuredAddressString);

    if (!info->isInitialSetup()) {
        if (serialNumber.isEmpty())
            pluginStorage()->remove(storagePrefix(thing) + "/configuredSerialNumber");
        else
            pluginStorage()->setValue(storagePrefix(thing) + "/configuredSerialNumber", serialNumber);
        pluginStorage()->sync();
    }

    if (m_connections.contains(thing)) {
        qCInfo(dcPcElectric()) << "Reconfiguring existing thing" << thing->name();
        PceWallbox *oldConnection = m_connections.take(thing);
        oldConnection->disconnectDevice();
        oldConnection->deleteLater();

        if (m_monitors.contains(thing)) {
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
        }
        clearAddressState(thing);
    } else {
        connect(thing, &Thing::stateValueChanged, this, [thing](const StateTypeId &stateTypeId, const QVariant &value, const QVariant &minValue, const QVariant &maxValue, const QVariantList &possibleValues){
            Q_UNUSED(minValue)
            Q_UNUSED(maxValue)
            Q_UNUSED(possibleValues)

            QStringList blackList;
            blackList << "temperature";

            StateType stateType = thing->thingClass().getStateType(stateTypeId);
            if (!blackList.contains(stateType.name())) {
                qCInfo(dcPcElectric()) << "State changed of" << thing->name() << "->" << stateType.name() << value;
            }
        });
    }

    NetworkDeviceMonitor *monitor = hardwareManager()->networkDeviceDiscovery()->registerMonitor(thing);
    if (monitor) {
        m_monitors.insert(thing, monitor);
        if (!isStaticThing(thing) && monitor->reachable() && !monitor->networkDeviceInfo().address().isNull())
            m_monitorAddresses.insert(thing, monitor->networkDeviceInfo().address());

        connect(monitor, &NetworkDeviceMonitor::reachableChanged, thing, [this, thing, monitor](bool reachable) {
            qCDebug(dcPcElectric()) << "Network device monitor for" << thing->name()
                                    << (reachable ? "is now reachable" : "is not reachable any more");
            if (isStaticThing(thing))
                return;
            if (reachable && !monitor->networkDeviceInfo().address().isNull())
                m_monitorAddresses.insert(thing, monitor->networkDeviceInfo().address());
            else
                m_monitorAddresses.remove(thing);
            providerAddressesChanged(thing);
        });
        connect(monitor, &NetworkDeviceMonitor::networkDeviceInfoChanged, thing, [this, thing, monitor](const NetworkDeviceInfo &networkDeviceInfo) {
            if (isStaticThing(thing) || !monitor->reachable())
                return;
            if (networkDeviceInfo.address().isNull())
                m_monitorAddresses.remove(thing);
            else
                m_monitorAddresses.insert(thing, networkDeviceInfo.address());
            providerAddressesChanged(thing);
        });
    } else {
        qCWarning(dcPcElectric()) << "Could not create a network device monitor for" << thing->name()
                                  << "Continuing with the configured address and ZeroConf.";
    }

    refreshZeroConfAddresses(thing);
    setupConnection(info);
}

void IntegrationPluginPcElectric::postSetupThing(Thing *thing)
{
    qCDebug(dcPcElectric()) << "Post setup thing" << thing->name();

    PceWallbox::ChargingCurrentState chargingCurrentState;
    chargingCurrentState.power = thing->stateValue("power").toBool();
    chargingCurrentState.maxChargingCurrent = thing->stateValue("maxChargingCurrent").toDouble();
    chargingCurrentState.desiredPhaseCount = thing->stateValue("desiredPhaseCount").toUInt();
    qCDebug(dcPcElectric()) << "Initialize charging current state with cached values" << chargingCurrentState;
    m_chargingCurrentStateBuffer[thing] = chargingCurrentState;
}

void IntegrationPluginPcElectric::thingRemoved(Thing *thing)
{
    qCInfo(dcPcElectric()) << "Thing removed" << thing->name();

    if (m_connections.contains(thing)) {
        PceWallbox *connection = m_connections.take(thing);
        connection->disconnectDevice();
        connection->deleteLater();
    }

    if (m_initialUpdate.contains(thing))
        m_initialUpdate.remove(thing);

    if (m_chargingCurrentStateBuffer.contains(thing))
        m_chargingCurrentStateBuffer.remove(thing);

    clearAddressState(thing);
    m_configuredSerialNumbers.remove(thing);
    m_staticAddressModes.remove(thing);

    pluginStorage()->remove(storagePrefix(thing));
    pluginStorage()->sync();

    // Unregister related hardware resources
    if (m_monitors.contains(thing))
        hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));

}

void IntegrationPluginPcElectric::executeAction(ThingActionInfo *info)
{
    Thing *thing = info->thing();

    if (info->action().actionTypeId() == ev11RefreshClientCertificateActionTypeId
        || info->action().actionTypeId() == ev11RfidRefreshClientCertificateActionTypeId
        || info->action().actionTypeId() == ev11NoMeterRefreshClientCertificateActionTypeId) {
        qCInfo(dcPcElectric()) << "Refreshing the PCE TLS client certificate.";
        QPointer<ThingActionInfo> guardedInfo(info);
        ensureClientIdentityAsync([this, guardedInfo](bool success, const QString &errorString) {
            if (!guardedInfo)
                return;
            if (!success) {
                qCWarning(dcPcElectric()) << "Could not refresh the PCE TLS client certificate:" << errorString;
                guardedInfo->finish(Thing::ThingErrorHardwareFailure);
                return;
            }

            bool connectionsConfigured = true;
            foreach (Thing *configuredThing, m_connections.keys()) {
                PceWallbox *configuredConnection = m_connections.value(configuredThing);
                if (configuredConnection->modbusTcpMaster()->transport() != ModbusTcpMaster::TransportTls)
                    continue;
                if (!configureTls(configuredThing, configuredConnection)) {
                    connectionsConfigured = false;
                    continue;
                }
                configuredConnection->disconnectDevice();
                configuredConnection->modbusTcpMaster()->reconnectDevice();
            }

            guardedInfo->finish(connectionsConfigured ? Thing::ThingErrorNoError
                                                       : Thing::ThingErrorHardwareFailure);
        }, true);
        return;
    }

    PceWallbox *connection = m_connections.value(thing);
    if (!connection || !connection->operational()) {
        qCWarning(dcPcElectric()) << "Could not execute action because the connection is not available.";
        info->finish(Thing::ThingErrorHardwareNotAvailable);
        return;
    }

    if (info->action().actionTypeId() == ev11RfidTagAcceptedActionTypeId
        || info->action().actionTypeId() == ev11RfidTagRejectedActionTypeId) {
        // The externally defined tagRejected action is the RFID manager's tagDenied
        // operation. It can deny a pending scan or time out a held authorization.
        const bool approved = info->action().actionTypeId() == ev11RfidTagAcceptedActionTypeId;
        if (!connection->canSubmitRfidDecision(approved)) {
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }
        const auto decisionConnection = std::make_shared<QMetaObject::Connection>();
        *decisionConnection = connect(connection, &PceWallbox::rfidDecisionFinished, info,
                                      [info, decisionConnection](bool success) {
            QObject::disconnect(*decisionConnection);
            info->finish(success ? Thing::ThingErrorNoError : Thing::ThingErrorHardwareFailure);
        });
        if (!connection->submitRfidDecision(approved)) {
            QObject::disconnect(*decisionConnection);
            info->finish(Thing::ThingErrorHardwareNotAvailable);
        }

        return;
    }

    if (info->action().actionTypeId() == ev11PowerActionTypeId || info->action().actionTypeId() == ev11RfidPowerActionTypeId || info->action().actionTypeId() == ev11NoMeterPowerActionTypeId) {
        bool power = false;
        if (info->action().actionTypeId() == ev11PowerActionTypeId) {
            power = info->action().paramValue(ev11PowerActionPowerParamTypeId).toBool();
        } else if (info->action().actionTypeId() == ev11RfidPowerActionTypeId) {
            power = info->action().paramValue(ev11RfidPowerActionPowerParamTypeId).toBool();
        } else if (info->action().actionTypeId() == ev11NoMeterPowerActionTypeId) {
            power = info->action().paramValue(ev11NoMeterPowerActionPowerParamTypeId).toBool();
        }

        if (info->action().triggeredBy() == Action::TriggeredByUser)
            qCInfo(dcPcElectric()) << "User: Set charging enabled of" << thing->name() << "to" << power;

        qCDebug(dcPcElectric()) << "Setting charging enabled to" << power;
        // Update buffer
        m_chargingCurrentStateBuffer[thing].power = power;

        quint16 registerValue = PceWallbox::deriveRegisterFromStates(m_chargingCurrentStateBuffer.value(thing));
        qCDebug(dcPcElectric()) << "Writing charging current register" << registerValue;
        QueuedModbusReply *reply = connection->setChargingCurrentAsync(registerValue);
        connect(reply, &QueuedModbusReply::finished, info, [reply, info, thing, power, registerValue]() {
            if (reply->error() != QModbusDevice::NoError) {
                qCWarning(dcPcElectric()) << "Could not set power state to" << power << "(" << registerValue << ")" << reply->errorString();
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }

            qCDebug(dcPcElectric()) << "Successfully set power state to" << power << "(" << registerValue << ")";
            thing->setStateValue("power", power);
            info->finish(Thing::ThingErrorNoError);
        });

        return;

    } else if (info->action().actionTypeId() == ev11MaxChargingCurrentActionTypeId || info->action().actionTypeId() == ev11RfidMaxChargingCurrentActionTypeId || info->action().actionTypeId() == ev11NoMeterMaxChargingCurrentActionTypeId) {
        double desiredChargingCurrent = 6;
        if (info->action().actionTypeId() == ev11MaxChargingCurrentActionTypeId) {
            desiredChargingCurrent = info->action().paramValue(ev11MaxChargingCurrentActionMaxChargingCurrentParamTypeId).toDouble();
        } else if (info->action().actionTypeId() == ev11RfidMaxChargingCurrentActionTypeId) {
            desiredChargingCurrent = info->action().paramValue(ev11RfidMaxChargingCurrentActionMaxChargingCurrentParamTypeId).toDouble();
        } else if (info->action().actionTypeId() == ev11NoMeterMaxChargingCurrentActionTypeId) {
            desiredChargingCurrent = info->action().paramValue(ev11NoMeterMaxChargingCurrentActionMaxChargingCurrentParamTypeId).toDouble();
        }

        if (info->action().triggeredBy() == Action::TriggeredByUser)
            qCInfo(dcPcElectric()) << "User: Set max charging current of" << thing->name() << "to" << desiredChargingCurrent << "A";

        qCDebug(dcPcElectric()) << "Setting max charging current to" << desiredChargingCurrent << "A";

        // Update buffer
        m_chargingCurrentStateBuffer[thing].maxChargingCurrent = desiredChargingCurrent;

        quint16 registerValue = PceWallbox::deriveRegisterFromStates(m_chargingCurrentStateBuffer.value(thing));
        qCDebug(dcPcElectric()) << "Writing charging current register" << registerValue;
        QueuedModbusReply *reply = connection->setChargingCurrentAsync(registerValue);
        connect(reply, &QueuedModbusReply::finished, info, [reply, info, thing, desiredChargingCurrent]() {
            if (reply->error() != QModbusDevice::NoError) {
                qCWarning(dcPcElectric()) << "Could not set charging current to" << desiredChargingCurrent << reply->errorString();
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }

            qCDebug(dcPcElectric()) << "Successfully set charging current (" << desiredChargingCurrent << ")";
            thing->setStateValue("maxChargingCurrent", desiredChargingCurrent);
            info->finish(Thing::ThingErrorNoError);
        });

        return;

    } else if (info->action().actionTypeId() == ev11DesiredPhaseCountActionTypeId || info->action().actionTypeId() == ev11RfidDesiredPhaseCountActionTypeId || info->action().actionTypeId() == ev11NoMeterDesiredPhaseCountActionTypeId) {
        uint desiredPhaseCount = 1;
        if (info->action().actionTypeId() == ev11DesiredPhaseCountActionTypeId) {
            desiredPhaseCount = info->action().paramValue(ev11DesiredPhaseCountActionDesiredPhaseCountParamTypeId).toUInt();

        } else if (info->action().actionTypeId() == ev11RfidDesiredPhaseCountActionTypeId) {
            desiredPhaseCount = info->action().paramValue(ev11RfidDesiredPhaseCountActionDesiredPhaseCountParamTypeId).toUInt();

        } else if (info->action().actionTypeId() == ev11NoMeterDesiredPhaseCountActionTypeId) {
            desiredPhaseCount = info->action().paramValue(ev11NoMeterDesiredPhaseCountActionDesiredPhaseCountParamTypeId).toUInt();
        }

        if (info->action().triggeredBy() == Action::TriggeredByUser)
            qCInfo(dcPcElectric()) << "User: Set desried phase count of" << thing->name() << "to" << desiredPhaseCount;

        qCDebug(dcPcElectric()) << "Setting desried phase count to" << desiredPhaseCount;

        // Update buffer
        m_chargingCurrentStateBuffer[thing].desiredPhaseCount = desiredPhaseCount;

        quint16 registerValue = PceWallbox::deriveRegisterFromStates(m_chargingCurrentStateBuffer.value(thing));
        qCDebug(dcPcElectric()) << "Writing charging current register" << registerValue;
        QueuedModbusReply *reply = connection->setChargingCurrentAsync(registerValue);
        connect(reply, &QueuedModbusReply::finished, info, [reply, info, thing, desiredPhaseCount]() {
            if (reply->error() != QModbusDevice::NoError) {
                qCWarning(dcPcElectric()) << "Could not set desired phase count to" << desiredPhaseCount << reply->errorString();
                info->finish(Thing::ThingErrorHardwareFailure);
                return;
            }

            qCDebug(dcPcElectric()) << "Successfully set phase count (" << desiredPhaseCount << ")";
            thing->setStateValue("desiredPhaseCount", desiredPhaseCount);
            info->finish(Thing::ThingErrorNoError);
        });

        return;
    }

    Q_ASSERT_X(false, "IntegrationPluginPcElectric::executeAction", QString("Unhandled action: %1").arg(info->action().actionTypeId().toString()).toLocal8Bit());
}

void IntegrationPluginPcElectric::setupConnection(ThingSetupInfo *info)
{
    Thing *thing = info->thing();
    const QHostAddress address = isStaticThing(thing)
                                     ? QHostAddress(thing->paramValue(m_addressParamTypes.value(thing->thingClassId())).toString())
                                     : QHostAddress();

    qCDebug(dcPcElectric()) << "Setting up PCE wallbox using" << address.toString();

    PceWallbox *connection = new PceWallbox(address, 502, 1, this);
    connection->setOperationalStartupEnabled(false);
    connection->setRfidEnabled(thing->thingClassId() == ev11RfidThingClassId);
    connect(connection, &QObject::destroyed, this, [this, connection]() {
        m_tlsUpgradesInProgress.remove(connection);
    });
    connect(info, &ThingSetupInfo::aborted, this, [this, thing, connection]() {
        if (m_connections.value(thing) == connection)
            m_connections.remove(thing);
        connection->disconnectDevice();
        connection->deleteLater();
        if (m_monitors.contains(thing))
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
        clearAddressState(thing);
    });

    // Connection reachability
    connect(connection, &PceWallbox::reachableChanged, thing, [this, thing, connection](bool reachable) {
        qCInfo(dcPcElectric()) << "Reachable changed to" << reachable << "for" << thing;
        const bool wasConnected = thing->stateValue("connected").toBool();
        m_initialUpdate[thing] = true;
        thing->setStateValue("connected", reachable && connection->operational());

        // Reset energy related information if not reachable
        if (!reachable && (thing->thingClassId() == ev11ThingClassId || thing->thingClassId() == ev11RfidThingClassId)) {
            thing->setStateValue("currentPower", 0);
            thing->setStateValue("currentPowerPhaseA", 0);
            thing->setStateValue("currentPowerPhaseB", 0);
            thing->setStateValue("currentPowerPhaseC", 0);
            thing->setStateValue("voltagePhaseA", 0);
            thing->setStateValue("voltagePhaseB", 0);
            thing->setStateValue("voltagePhaseC", 0);
            thing->setStateValue("currentPhaseA", 0);
            thing->setStateValue("currentPhaseB", 0);
            thing->setStateValue("currentPhaseC", 0);
        }

        if (!reachable && wasConnected && !isStaticThing(thing)) {
            const QHostAddress failedAddress = connection->modbusTcpMaster()->hostAddress();
            connection->disconnectDevice();
            m_addressAttemptsInProgress.remove(thing);
            m_attemptedAddresses.remove(thing);
            if (!failedAddress.isNull())
                m_attemptedAddresses[thing].insert(failedAddress);
            QTimer::singleShot(0, thing, [this, thing]() {
                tryNextAddress(thing);
            });
        }
    });

    connect(connection->modbusTcpMaster(), &ModbusTcpMaster::connectionErrorOccurred, thing, [this, thing](QModbusDevice::Error error) {
        if (error == QModbusDevice::ConnectionError && m_addressAttemptsInProgress.contains(thing))
            addressAttemptFailed(thing);
    });

    connect(connection, &PceWallbox::initializationFinished, thing, [this, thing, connection](bool success) {
        if (!success) {
            addressAttemptFailed(thing);
            return;
        }

        const QString expectedSerial = thing->paramValue(m_serialNumberParamTypes.value(thing->thingClassId())).toString();
        const QString actualSerial = wallboxSerialNumber(connection);
        if (actualSerial.isEmpty() || actualSerial == "0") {
            qCWarning(dcPcElectric()) << "Rejecting PCE endpoint with invalid serial number on"
                                      << connection->modbusTcpMaster()->hostAddress();
            connection->disconnectDevice();
            addressAttemptFailed(thing);
            return;
        }
        if (!expectedSerial.isEmpty() && actualSerial != expectedSerial) {
            qCWarning(dcPcElectric()) << "Rejecting PCE endpoint on"
                                      << connection->modbusTcpMaster()->hostAddress()
                                      << "Expected serial:" << expectedSerial
                                      << "actual serial:" << actualSerial;
            if (pluginStorage()->value(storagePrefix(thing) + "/serverFingerprint").toString().isEmpty())
                connection->modbusTcpMaster()->setAcceptedPeerCertificateFingerprint(QString());
            connection->disconnectDevice();
            addressAttemptFailed(thing, actualSerial);
            return;
        }

        if (expectedSerial.isEmpty()) {
            qCInfo(dcPcElectric()) << "Learned serial number" << actualSerial << "for" << thing->name();
            thing->setParamValue(m_serialNumberParamTypes.value(thing->thingClassId()), actualSerial);
            m_configuredSerialNumbers.insert(thing, actualSerial);
            refreshZeroConfAddresses(thing);
        }
        pluginStorage()->setValue(storagePrefix(thing) + "/configuredSerialNumber", actualSerial);
        pluginStorage()->sync();
        finishInitialSetup(thing, Thing::ThingErrorNoError);

        ModbusTcpMaster *master = connection->modbusTcpMaster();
        if (master->transport() == ModbusTcpMaster::TransportTcp && connection->firmwareRevision() > "0025") {
            if (m_tlsUpgradesInProgress.contains(connection))
                return;

            m_tlsUpgradesInProgress.insert(connection);
            QPointer<PceWallbox> guardedConnection(connection);
            QPointer<Thing> guardedThing(thing);
            ensureClientIdentityAsync([this, guardedThing, guardedConnection](bool identityAvailable, const QString &errorString) {
                if (!guardedThing || !guardedConnection)
                    return;
                m_tlsUpgradesInProgress.remove(guardedConnection);
                if (m_connections.value(guardedThing) != guardedConnection)
                    return;
                if (!identityAvailable || !configureTls(guardedThing, guardedConnection)) {
                    qCWarning(dcPcElectric()) << "Could not upgrade the PCE wallbox to mandatory TLS:" << errorString;
                    guardedConnection->disconnectDevice();
                    addressAttemptFailed(guardedThing);
                    return;
                }

                qCInfo(dcPcElectric()) << "Firmware" << guardedConnection->firmwareRevision()
                                       << "requires TLS. Connecting to port 802.";
                guardedConnection->disconnectDevice();
                guardedConnection->modbusTcpMaster()->reconnectDevice();
            });
            return;
        }

        if (master->transport() == ModbusTcpMaster::TransportTls) {
            const QString fingerprint = master->peerCertificateFingerprint();
            qCDebug(dcPcElectric()) << "PCE TLS fingerprint verification after initialization"
                                    << "pin fingerprint:" << master->acceptedPeerCertificateFingerprint()
                                    << "server fingerprint:" << fingerprint
                                    << "match:" << (!fingerprint.isEmpty()
                                                     && fingerprint == master->acceptedPeerCertificateFingerprint());
            if (fingerprint.isEmpty()) {
                connection->disconnectDevice();
                addressAttemptFailed(thing);
                return;
            }
            pluginStorage()->remove(storagePrefix(thing) + "/tlsRequired");
            pluginStorage()->setValue(storagePrefix(thing) + "/serverFingerprint", fingerprint);
            pluginStorage()->sync();
        }

        if (thing->thingClassId() == ev11RfidThingClassId) {
            const auto initializationConnection = std::make_shared<QMetaObject::Connection>();
            *initializationConnection = connect(connection, &PceWallbox::rfidInitializationFinished, thing,
                    [thing, connection, initializationConnection](bool rfidReady) {
                QObject::disconnect(*initializationConnection);
                if (!rfidReady) {
                    qCWarning(dcPcElectric()) << "Could not confirm the requested RFID operating mode.";
                    thing->setStateValue("connected", false);
                    return;
                }
                connection->startOperationalMode();
                thing->setStateValue("connected", true);
            });

            EV11ModbusTcpConnection::RfidOperatingMode operatingMode = EV11ModbusTcpConnection::RfidOperatingModeRemote;
            rfidOperatingModeFromSettingValue(thing->setting("rfidOperatingMode").toString(), &operatingMode);
            if (!connection->initializeRfidOperatingMode(operatingMode)) {
                QObject::disconnect(*initializationConnection);
                qCWarning(dcPcElectric()) << "Could not initialize the RFID operating mode.";
                thing->setStateValue("connected", false);
            }
            return;
        }

        connection->startOperationalMode();
        m_addressAttemptsInProgress.remove(thing);
        m_attemptedAddresses.remove(thing);
        m_unexpectedSerialNumbers.remove(thing);
        thing->setStateValue("connected", true);
    });

    connect(connection, &PceWallbox::rfidTagDetected, thing, [thing](const QString &code) {
        qCInfo(dcPcElectric()) << "RFID tag detected" << QString(code.length(), '*');
        thing->emitEvent(ev11RfidTagDetectedEventTypeId, {Param(ev11RfidTagDetectedEventCodeParamTypeId, code)});
    });

    connect(connection, &PceWallbox::updateFinished, thing, [this, thing, connection]() {
        qCDebug(dcPcElectric()) << "Update finished for" << thing;
        qCDebug(dcPcElectric()) << connection;

        if (connection->phaseAutoSwitch()) {
            thing->setStatePossibleValues("desiredPhaseCount", {1, 3}); // Enable phase switching

            switch (connection->chargingRelayState()) {
            case EV11ModbusTcpConnection::ChargingRelayStateNoCharging:
                // Not charging, assuming phase count matches the desired phase count...
                thing->setStateValue("phaseCount", thing->stateValue("desiredPhaseCount"));
                break;
            case EV11ModbusTcpConnection::ChargingRelayStateSinglePhase:
                thing->setStateValue("phaseCount", 1);
                break;
            case EV11ModbusTcpConnection::ChargingRelayStateTheePhase:
                thing->setStateValue("phaseCount", 3);
                break;
            }
        } else {
            // Note: if auto phase switching is disabled, the wallbox forces 3 phase charging
            thing->setStatePossibleValues("desiredPhaseCount", {3}); // Disable phase switching (default 3)
            thing->setStateValue("desiredPhaseCount", 3);
            thing->setStateValue("phaseCount", 3);
        }

        thing->setStateMaxValue("maxChargingCurrent", connection->maxChargingCurrentDip() / 1000);
        thing->setStateValue("pluggedIn", connection->chargingState() >= PceWallbox::ChargingStateB1 && connection->chargingState() < PceWallbox::ChargingStateError);
        thing->setStateValue("charging", connection->chargingState() == PceWallbox::ChargingStateC2);

        switch (connection->chargingState()) {
        case PceWallbox::ChargingStateInitializing:
            thing->setStateValue("status", "Init");
            break;
        case PceWallbox::ChargingStateA1:
            thing->setStateValue("status", "A1");
            break;
        case PceWallbox::ChargingStateA2:
            thing->setStateValue("status", "A2");
            break;
        case PceWallbox::ChargingStateB1:
            thing->setStateValue("status", "B1");
            break;
        case PceWallbox::ChargingStateB2:
            thing->setStateValue("status", "B2");
            break;
        case PceWallbox::ChargingStateC1:
            thing->setStateValue("status", "C1");
            break;
        case PceWallbox::ChargingStateC2:
            thing->setStateValue("status", "C2");
            break;
        case PceWallbox::ChargingStateError:
            thing->setStateValue("status", "F");
            break;
        }

        thing->setStateValue("currentVersion", connection->firmwareRevision());
        thing->setStateValue("temperature", connection->temperature());

        switch (connection->error()) {
        case EV11ModbusTcpConnection::ErrorNoError:
            thing->setStateValue("error", "Kein Fehler aktiv");
            break;
        case EV11ModbusTcpConnection::ErrorOverheating:
            thing->setStateValue("error", "1: Übertemperatur. Ladevorgang wird automatisch fortgesetzt.");
            break;
        case EV11ModbusTcpConnection::ErrorDCFaultCurrent:
            thing->setStateValue("error", "2: DC Fehlerstromsensor ausgelöst.");
            break;
        case EV11ModbusTcpConnection::ErrorChargingWithVentilation:
            thing->setStateValue("error", "3: Ladeanforderung mit Belüftung.");
            break;
        case EV11ModbusTcpConnection::ErrorCPErrorEF:
            thing->setStateValue("error", "4: CP Signal, Fehlercode E oder F.");
            break;
        case EV11ModbusTcpConnection::ErrorCPErrorBypass:
            thing->setStateValue("error", "5: CP Signal, bypass.");
            break;
        case EV11ModbusTcpConnection::ErrorCPErrorDiodFault:
            thing->setStateValue("error", "6: CP Signal, Diode defekt.");
            break;
        case EV11ModbusTcpConnection::ErrorDCFaultCurrentCalibrating:
            thing->setStateValue("error", "7: DC Fehlerstromsensor, Kalibrirung.");
            break;
        case EV11ModbusTcpConnection::ErrorDCFaultCurrentCommunication:
            thing->setStateValue("error", "8: DC Fehlerstromsensor, Kommunikationsfehler.");
            break;
        case EV11ModbusTcpConnection::ErrorDCFaultCurrentError:
            thing->setStateValue("error", "9: DC Fehlerstromsensor, Fehler.");
            break;
        case EV11ModbusTcpConnection::ErrorGridMonitoringError:
            thing->setStateValue("error", "11: Netzüberwachung ausgelöst.");
            break;
        }

        if (connection->firmwareRevision() >= "0025") {
            thing->setStateValue("digitalInputFlag", QString("0b%1").arg(connection->digitalInputFlag(), 16, 2, QLatin1Char('0')));
            thing->setStateValue("modeR37", connection->modeR37());

            // Energy information only available with meter and 0025
            if (thing->thingClassId() == ev11ThingClassId || thing->thingClassId() == ev11RfidThingClassId) {
                thing->setStateValue("currentPower", connection->currentPower());
                thing->setStateValue("sessionEnergy", connection->powerMeter0());
                thing->setStateValue("totalEnergyConsumed", connection->totalEnergyConsumed());
                thing->setStateValue("currentPowerPhaseA", connection->activePowerL1());
                thing->setStateValue("currentPowerPhaseB", connection->activePowerL2());
                thing->setStateValue("currentPowerPhaseC", connection->activePowerL3());
                thing->setStateValue("voltagePhaseA", connection->voltageL1());
                thing->setStateValue("voltagePhaseB", connection->voltageL2());
                thing->setStateValue("voltagePhaseC", connection->voltageL3());
                thing->setStateValue("currentPhaseA", connection->currentL1());
                thing->setStateValue("currentPhaseB", connection->currentL2());
                thing->setStateValue("currentPhaseC", connection->currentL3());
            }

        } else {
            // In firmware 0019 there is no current power register, depending on the CP state we can assume the car is consuming the amount
            // we adjusted, if the car is full, the CP state will change back to B2
            if (thing->thingClassId() == ev11ThingClassId || thing->thingClassId() == ev11RfidThingClassId) {
                thing->setStateValue("sessionEnergy", connection->powerMeter0());
                if (connection->chargingState() == PceWallbox::ChargingStateC2 && connection->currentPower() == 0) {
                    // We are currently chargin, but the wallbox reports 0 W (which is expected), let's calculate the theoretical power...
                    double assumedCurrentPower = thing->stateValue("phaseCount").toInt() * 230 * thing->stateValue("maxChargingCurrent").toDouble();
                    qCDebug(dcPcElectric())
                        << "Assuming current power"
                        << assumedCurrentPower
                        << "W ("
                        << thing->stateValue("phaseCount").toInt()
                        << "phases * 230 V *"
                        << thing->stateValue("maxChargingCurrent").toDouble()
                        << "A )";
                    thing->setStateValue("currentPower", assumedCurrentPower);
                } else {
                    thing->setStateValue("currentPower", 0);
                }
            }
        }

        if (thing->thingClassId() == ev11RfidThingClassId) {
            switch (connection->rfidOperatingMode()) {
            case EV11ModbusTcpConnection::RfidOperatingModeDisabled:
                thing->setSettingValue("rfidOperatingMode", "0 | Disabled");
                break;
            case EV11ModbusTcpConnection::RfidOperatingModeRemote:
                thing->setSettingValue("rfidOperatingMode", "1 | Remote");
                break;
            case EV11ModbusTcpConnection::RfidOperatingModeLocal:
                thing->setSettingValue("rfidOperatingMode", "2 | Local");
                break;
            }
        }

        if (m_initialUpdate.value(thing)) {
            m_initialUpdate[thing] = false;

            qCDebug(dcPcElectric()) << "Update initial charger states from charging current register...";

            PceWallbox::ChargingCurrentState chargingCurrentState = PceWallbox::deriveStatesFromRegister(connection->chargingCurrent());
            qCDebug(dcPcElectric()) << chargingCurrentState;
            thing->setStateValue("power", chargingCurrentState.power);
            thing->setStateValue("desiredPhaseCount", chargingCurrentState.desiredPhaseCount);
            if (chargingCurrentState.power)
                thing->setStateValue("maxChargingCurrent", chargingCurrentState.maxChargingCurrent);

            m_chargingCurrentStateBuffer[thing] = chargingCurrentState;

            qCDebug(dcPcElectric()) << "Updating initial settings after connecting...";

            thing->setSettingValue("ledBrightness", connection->ledBrightness());

            switch (connection->digitalInputMode()) {
            case EV11ModbusTcpConnection::DigitalInputModeEnableCharging:
                thing->setSettingValue("digitalInputMode", "0 | Charging allowed");
                break;
            case EV11ModbusTcpConnection::DigitalInputModeEnableChargingInverted:
                thing->setSettingValue("digitalInputMode", "1 | Charging allowed inverted");
                break;
            case EV11ModbusTcpConnection::DigitalInputModePwmS0Enabled:
                thing->setSettingValue("digitalInputMode", "2 | PWM and S0 signaling");
                break;
            case EV11ModbusTcpConnection::DigitalInputModeLimitS0Enabled:
                thing->setSettingValue("digitalInputMode", "3 | Limit and S0 signaling");
                break;
            }

            thing->setStateValue("digitalInputMode", connection->digitalInputMode());

            if (connection->firmwareRevision() >= "0025") {
                thing->setSettingValue("phaseAutoSwitchPause", connection->phaseAutoSwitchPause());
                thing->setSettingValue("phaseAutoSwitchMinChargingTime", connection->phaseAutoSwitchMinChargingTime());
                thing->setSettingValue("forceChargingResume", connection->forceChargingResume() == 1 ? true : false);
            }

        }
    });

    connect(thing, &Thing::settingChanged, connection, [thing, connection](const ParamTypeId &paramTypeId, const QVariant &value) {
        if (!connection->operational()) {
            qCWarning(dcPcElectric()) << "Ignoring setting change while the wallbox connection is not operational.";
            return;
        }
        if (paramTypeId == ev11SettingsLedBrightnessParamTypeId || paramTypeId == ev11RfidSettingsLedBrightnessParamTypeId || paramTypeId == ev11NoMeterSettingsLedBrightnessParamTypeId) {
            quint16 percentage = value.toUInt();

            qCDebug(dcPcElectric()) << "Setting LED brightness to" << percentage << "%";
            QueuedModbusReply *reply = connection->setLedBrightnessAsync(percentage);
            connect(reply, &QueuedModbusReply::finished, thing, [reply, percentage]() {
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcPcElectric()) << "Could not set led brightness to" << percentage << "%" << reply->errorString();
                    return;
                }

                qCDebug(dcPcElectric()) << "Successfully set led brightness to" << percentage << "%";
            });
        } else if (paramTypeId == ev11SettingsDigitalInputModeParamTypeId || paramTypeId == ev11RfidSettingsDigitalInputModeParamTypeId || paramTypeId == ev11NoMeterSettingsDigitalInputModeParamTypeId) {
            QString mode = value.toString();
            qCDebug(dcPcElectric()) << "Setting Digital input mode to" << mode;

            EV11ModbusTcpConnection::DigitalInputMode modeValue;
            if (mode == "0 | Charging allowed") {
                modeValue = EV11ModbusTcpConnection::DigitalInputModeEnableCharging;
            } else if (mode == "1 | Charging allowed inverted") {
                modeValue = EV11ModbusTcpConnection::DigitalInputModeEnableChargingInverted;
            } else if (mode == "2 | PWM and S0 signaling") {
                modeValue = EV11ModbusTcpConnection::DigitalInputModePwmS0Enabled;
            } else if (mode == "3 | Limit and S0 signaling") {
                modeValue = EV11ModbusTcpConnection::DigitalInputModeLimitS0Enabled;
            } else {
                qCWarning(dcPcElectric()) << "Unknown digital input mode value" << mode;
                return;
            }

            QueuedModbusReply *reply = connection->setDigitalInputModeAsync(modeValue);
            connect(reply, &QueuedModbusReply::finished, thing, [thing, reply, modeValue]() {
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcPcElectric()) << "Could not set digital input mode to" << modeValue << reply->errorString();
                    return;
                }

                qCDebug(dcPcElectric()) << "Successfully set digital input mode to" << modeValue;
                thing->setStateValue("digitalInputMode", modeValue);
            });
        } else if (paramTypeId == ev11SettingsPhaseAutoSwitchPauseParamTypeId || paramTypeId == ev11RfidSettingsPhaseAutoSwitchPauseParamTypeId || paramTypeId == ev11NoMeterSettingsPhaseAutoSwitchPauseParamTypeId) {
            quint16 registerValue = value.toUInt();

            qCDebug(dcPcElectric()) << "Setting phase auto switch pause to" << registerValue << "s";
            QueuedModbusReply *reply = connection->setPhaseAutoSwitchPauseAsync(registerValue);
            connect(reply, &QueuedModbusReply::finished, thing, [reply, registerValue]() {
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcPcElectric()) << "Could not set phase auto switch pause to" << registerValue << "s" << reply->errorString();
                    return;
                }

                qCDebug(dcPcElectric()) << "Successfully set phase auto switch pause to" << registerValue << "s";
            });
        } else if (paramTypeId == ev11SettingsPhaseAutoSwitchMinChargingTimeParamTypeId || paramTypeId == ev11RfidSettingsPhaseAutoSwitchMinChargingTimeParamTypeId || paramTypeId == ev11NoMeterSettingsPhaseAutoSwitchMinChargingTimeParamTypeId) {
            quint16 registerValue = value.toUInt();

            qCDebug(dcPcElectric()) << "Setting phase auto switch min charging current" << registerValue << "s";
            QueuedModbusReply *reply = connection->setPhaseAutoSwitchMinChargingTimeAsync(registerValue);
            connect(reply, &QueuedModbusReply::finished, thing, [reply, registerValue]() {
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcPcElectric()) << "Could not set phase auto switch min charging current to" << registerValue << "s" << reply->errorString();
                    return;
                }

                qCDebug(dcPcElectric()) << "Successfully set phase auto switch min charging current to" << registerValue << "s";
                //thing->setSettingValue("phaseAutoSwitchMinChargingTime", registerValue);
            });
        } else if (paramTypeId == ev11SettingsForceChargingResumeParamTypeId || paramTypeId == ev11RfidSettingsForceChargingResumeParamTypeId || paramTypeId == ev11NoMeterSettingsForceChargingResumeParamTypeId) {
            quint16 registerValue = value.toBool() ? 1 : 0;

            qCDebug(dcPcElectric()) << "Setting force charging resume to" << registerValue;
            QueuedModbusReply *reply = connection->setForceChargingResumeAsync(registerValue);
            connect(reply, &QueuedModbusReply::finished, thing, [reply, registerValue]() {
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcPcElectric()) << "Could not set force charging resume to" << registerValue << reply->errorString();
                    return;
                }

                qCDebug(dcPcElectric()) << "Successfully set force charging resume to" << registerValue;
                //thing->setSettingValue("forceChargingResume", registerValue == 1 ? true : false);
            });
        } else if (paramTypeId == ev11RfidSettingsRfidOperatingModeParamTypeId) {
            QString mode = value.toString();
            qCDebug(dcPcElectric()) << "Setting RFID operating mode to" << mode;

            EV11ModbusTcpConnection::RfidOperatingMode modeValue;
            if (!rfidOperatingModeFromSettingValue(mode, &modeValue)) {
                qCWarning(dcPcElectric()) << "Unknown RFID operating mode value" << mode;
                return;
            }

            QueuedModbusReply *reply = connection->setRfidOperatingModeAsync(modeValue);
            connect(reply, &QueuedModbusReply::finished, thing, [reply, modeValue]() {
                if (reply->error() != QModbusDevice::NoError) {
                    qCWarning(dcPcElectric()) << "Could not set RFID operating mode to" << modeValue << reply->errorString();
                    return;
                }

                qCDebug(dcPcElectric()) << "Successfully set RFID operating mode to" << modeValue;
            });
        }
    });

    m_connections.insert(thing, connection);
    if (info->isInitialSetup()) {
        m_pendingInitialSetups.insert(thing, info);
    } else {
        info->finish(Thing::ThingErrorNoError);
    }

    if (availableAddresses(thing).isEmpty()) {
        qCInfo(dcPcElectric()) << "No address is currently available for" << thing->name()
                               << "Waiting for the network monitor or ZeroConf.";
        return;
    }

    tryNextAddress(thing);
}

bool IntegrationPluginPcElectric::isStaticThing(Thing *thing) const
{
    if (m_staticAddressModes.contains(thing))
        return m_staticAddressModes.value(thing);
    return !QHostAddress(thing->paramValue(m_addressParamTypes.value(thing->thingClassId())).toString()).isNull();
}

bool IntegrationPluginPcElectric::isMatchingZeroConfService(Thing *thing, const ZeroConfServiceEntry &entry) const
{
    const QString serialNumber = thing->paramValue(m_serialNumberParamTypes.value(thing->thingClassId())).toString();
    return !serialNumber.isEmpty() && entry.protocol() == QAbstractSocket::IPv4Protocol
           && entry.serviceType() == "_modbus._tcp" && entry.port() == 502
           && entry.name().startsWith("EV11")
           && entry.txt("serial") == serialNumber
           && !MacAddress(entry.txt("mac")).isNull();
}

QSet<QHostAddress> IntegrationPluginPcElectric::availableAddresses(Thing *thing) const
{
    if (isStaticThing(thing)) {
        return {QHostAddress(thing->paramValue(m_addressParamTypes.value(thing->thingClassId())).toString())};
    }

    QSet<QHostAddress> addresses = m_zeroConfAddresses.value(thing);
    const QHostAddress monitorAddress = m_monitorAddresses.value(thing);
    if (!monitorAddress.isNull())
        addresses.insert(monitorAddress);
    return addresses;
}

void IntegrationPluginPcElectric::refreshZeroConfAddresses(Thing *thing)
{
    QSet<QHostAddress> addresses;
    foreach (const ZeroConfServiceEntry &entry, m_modbusServiceBrowser->serviceEntries()) {
        if (!isMatchingZeroConfService(thing, entry) || entry.hostAddress().isNull())
            continue;
        if (isStaticThing(thing)) {
            const QHostAddress configuredAddress(thing->paramValue(m_addressParamTypes.value(thing->thingClassId())).toString());
            if (entry.hostAddress() != configuredAddress) {
                qCWarning(dcPcElectric()) << "ZeroConf advertises serial" << entry.txt("serial")
                                          << "on" << entry.hostAddress()
                                          << "but" << thing->name()
                                          << "is pinned to" << configuredAddress;
            }
        } else {
            addresses.insert(entry.hostAddress());
        }
    }
    m_zeroConfAddresses.insert(thing, addresses);
}

void IntegrationPluginPcElectric::providerAddressesChanged(Thing *thing)
{
    if (!m_connections.contains(thing))
        return;

    m_attemptedAddresses.remove(thing);
    if (m_addressAttemptsInProgress.contains(thing)) {
        const QHostAddress activeAddress = m_connections.value(thing)->modbusTcpMaster()->hostAddress();
        if (!activeAddress.isNull())
            m_attemptedAddresses[thing].insert(activeAddress);
    }
    m_unexpectedSerialNumbers.remove(thing);
    tryNextAddress(thing);
}

void IntegrationPluginPcElectric::tryNextAddress(Thing *thing)
{
    PceWallbox *connection = m_connections.value(thing);
    if (!connection || connection->operational() || m_addressAttemptsInProgress.contains(thing)
        || m_tlsUpgradesInProgress.contains(connection)) {
        return;
    }

    QList<QHostAddress> addresses = availableAddresses(thing).values();
    const QHostAddress currentAddress = connection->modbusTcpMaster()->hostAddress();
    std::sort(addresses.begin(), addresses.end(), [&currentAddress](const QHostAddress &left, const QHostAddress &right) {
        if (left == currentAddress)
            return false;
        if (right == currentAddress)
            return true;
        return left.toString() < right.toString();
    });

    QHostAddress nextAddress;
    foreach (const QHostAddress &address, addresses) {
        if (!address.isNull() && !m_attemptedAddresses.value(thing).contains(address)) {
            nextAddress = address;
            break;
        }
    }

    if (nextAddress.isNull()) {
        if (m_pendingInitialSetups.contains(thing)) {
            const QString unexpectedSerial = m_unexpectedSerialNumbers.value(thing);
            if (!unexpectedSerial.isEmpty()) {
                finishInitialSetup(
                    thing,
                    Thing::ThingErrorInvalidParameter,
                    tr("The PCE wallbox serial number does not match. The endpoint reported %1.").arg(unexpectedSerial));
            } else {
                finishInitialSetup(thing,
                                   Thing::ThingErrorHardwareNotAvailable,
                                   QT_TR_NOOP("None of the available PCE wallbox addresses could be reached."));
            }
        } else {
            if (addresses.isEmpty()) {
                qCInfo(dcPcElectric()) << "No address is currently available for" << thing->name()
                                       << "Waiting for the network monitor or ZeroConf.";
            } else if (!m_addressRetriesScheduled.contains(thing)) {
                qCInfo(dcPcElectric()) << "All currently known addresses have failed for" << thing->name()
                                       << "Retrying them in" << addressRetryIntervalMs << "ms.";
                m_addressRetriesScheduled.insert(thing);
                QTimer::singleShot(addressRetryIntervalMs, thing, [this, thing]() {
                    m_addressRetriesScheduled.remove(thing);
                    if (!m_connections.contains(thing))
                        return;
                    m_attemptedAddresses.remove(thing);
                    m_unexpectedSerialNumbers.remove(thing);
                    tryNextAddress(thing);
                });
            }
        }
        return;
    }

    qCInfo(dcPcElectric()) << "Trying PCE wallbox address" << nextAddress << "for" << thing->name();
    m_attemptedAddresses[thing].insert(nextAddress);
    m_addressAttemptsInProgress.insert(thing);
    const bool addressChanged = !currentAddress.isNull() && currentAddress != nextAddress;
    connection->modbusTcpMaster()->setHostAddress(nextAddress);
    if (addressChanged)
        connection->modbusTcpMaster()->reconnectDevice();
    else
        connection->connectDevice();
}

void IntegrationPluginPcElectric::addressAttemptFailed(Thing *thing, const QString &unexpectedSerial)
{
    if (!m_connections.contains(thing) || !m_addressAttemptsInProgress.remove(thing))
        return;

    if (!unexpectedSerial.isEmpty())
        m_unexpectedSerialNumbers.insert(thing, unexpectedSerial);

    PceWallbox *connection = m_connections.value(thing);
    if (isStaticThing(thing) && !m_pendingInitialSetups.contains(thing)) {
        qCWarning(dcPcElectric()) << "Static PCE wallbox connection failed on"
                                  << connection->modbusTcpMaster()->hostAddress()
                                  << "Keeping the configured address.";
        return;
    }

    connection->disconnectDevice();
    QTimer::singleShot(0, thing, [this, thing]() {
        tryNextAddress(thing);
    });
}

void IntegrationPluginPcElectric::finishInitialSetup(Thing *thing, Thing::ThingError error, const QString &message)
{
    QPointer<ThingSetupInfo> info = m_pendingInitialSetups.take(thing);
    if (!info)
        return;
    if (error != Thing::ThingErrorNoError) {
        if (m_connections.contains(thing)) {
            PceWallbox *connection = m_connections.take(thing);
            connection->disconnectDevice();
            connection->deleteLater();
        }
        if (m_monitors.contains(thing))
            hardwareManager()->networkDeviceDiscovery()->unregisterMonitor(m_monitors.take(thing));
        clearAddressState(thing);
        m_configuredSerialNumbers.remove(thing);
        m_staticAddressModes.remove(thing);
    }
    info->finish(error, message);
}

void IntegrationPluginPcElectric::clearAddressState(Thing *thing)
{
    m_monitorAddresses.remove(thing);
    m_zeroConfAddresses.remove(thing);
    m_attemptedAddresses.remove(thing);
    m_pendingInitialSetups.remove(thing);
    m_unexpectedSerialNumbers.remove(thing);
    m_addressAttemptsInProgress.remove(thing);
    m_addressRetriesScheduled.remove(thing);
}

void IntegrationPluginPcElectric::handleZeroConfServiceAdded(const ZeroConfServiceEntry &entry)
{
    foreach (Thing *thing, m_connections.keys()) {
        if (!isMatchingZeroConfService(thing, entry))
            continue;

        if (isStaticThing(thing)) {
            const QHostAddress configuredAddress(thing->paramValue(m_addressParamTypes.value(thing->thingClassId())).toString());
            if (entry.hostAddress() != configuredAddress) {
                qCWarning(dcPcElectric()) << "ZeroConf advertises serial" << entry.txt("serial")
                                          << "on" << entry.hostAddress()
                                          << "but" << thing->name()
                                          << "is pinned to" << configuredAddress;
            }
            continue;
        }

        qCInfo(dcPcElectric()) << "ZeroConf provides address" << entry.hostAddress() << "for" << thing->name();
        m_zeroConfAddresses[thing].insert(entry.hostAddress());
        providerAddressesChanged(thing);
    }
}

void IntegrationPluginPcElectric::handleZeroConfServiceRemoved(const ZeroConfServiceEntry &entry)
{
    foreach (Thing *thing, m_connections.keys()) {
        if (isStaticThing(thing) || !isMatchingZeroConfService(thing, entry))
            continue;

        qCInfo(dcPcElectric()) << "ZeroConf address disappeared for" << thing->name() << entry.hostAddress();
        QTimer::singleShot(0, thing, [this, thing]() {
            refreshZeroConfAddresses(thing);
            providerAddressesChanged(thing);
        });
    }
}

QString IntegrationPluginPcElectric::storagePrefix(Thing *thing) const
{
    return QStringLiteral("pcelectric/%1").arg(thing->id().toString());
}

QString IntegrationPluginPcElectric::wallboxSerialNumber(PceWallbox *connection) const
{
    QByteArray serialRawData;
    QDataStream stream(&serialRawData, QIODevice::WriteOnly);
    stream << static_cast<quint16>(0);
    for (int i = 0; i < connection->serialNumber().length(); ++i)
        stream << connection->serialNumber().at(i);
    return QString::number(serialRawData.toHex().toULongLong(nullptr, 16));
}

QString IntegrationPluginPcElectric::resolvedIdentityPath(const QString &configuredPath) const
{
    const QFileInfo info(configuredPath);
    return info.isAbsolute() ? info.absoluteFilePath()
                             : QDir(NymeaSettings::settingsPath()).absoluteFilePath(configuredPath);
}

bool IntegrationPluginPcElectric::ensureClientIdentity(QString *errorString)
{
    if (!m_clientCertificate.isNull() && !m_clientPrivateKey.isNull())
        return true;

    QString configuredCertificatePath = configValue(pcElectricPluginClientCertificatePathParamTypeId).toString();
    QString configuredKeyPath = configValue(pcElectricPluginClientKeyPathParamTypeId).toString();
    if (configuredCertificatePath.isEmpty())
        configuredCertificatePath = QStringLiteral("pcelectric/client-certificate.pem");
    if (configuredKeyPath.isEmpty())
        configuredKeyPath = QStringLiteral("pcelectric/client-key.pem");
    const QString certificatePath = resolvedIdentityPath(configuredCertificatePath);
    const QString keyPath = resolvedIdentityPath(configuredKeyPath);
    const bool certificateExists = pathExists(certificatePath);
    const bool keyExists = pathExists(keyPath);
    if (certificateExists != keyExists) {
        if (errorString)
            *errorString = tr("Only one PCE TLS identity file exists; refusing to replace it.");
        return false;
    }

    if (!certificateExists)
        return false;

    QFile certificateFile(certificatePath);
    QFile keyFile(keyPath);
    if (!certificateFile.open(QIODevice::ReadOnly) || !keyFile.open(QIODevice::ReadOnly))
        return false;
    const QSslCertificate certificate(certificateFile.readAll(), QSsl::Pem);
    const QByteArray keyData = keyFile.readAll();
    QSslKey key(keyData, QSsl::Ec, QSsl::Pem, QSsl::PrivateKey);
    if (key.isNull())
        key = QSslKey(keyData, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey);
    if (!certificateIsUsable(certificate) || key.isNull() || certificate.publicKey().algorithm() != key.algorithm()) {
        if (errorString)
            *errorString = tr("The PCE TLS certificate or key is invalid or uses a different algorithm.");
        return false;
    }
    m_clientCertificate = certificate;
    m_clientPrivateKey = key;
    return true;
}

void IntegrationPluginPcElectric::ensureClientIdentityAsync(const std::function<void(bool, const QString &)> &callback, bool refresh)
{
    QString errorString;
    if (!refresh && ensureClientIdentity(&errorString)) {
        callback(true, QString());
        return;
    }

    QString configuredCertificatePath = configValue(pcElectricPluginClientCertificatePathParamTypeId).toString();
    QString configuredKeyPath = configValue(pcElectricPluginClientKeyPathParamTypeId).toString();
    if (configuredCertificatePath.isEmpty())
        configuredCertificatePath = QStringLiteral("pcelectric/client-certificate.pem");
    if (configuredKeyPath.isEmpty())
        configuredKeyPath = QStringLiteral("pcelectric/client-key.pem");
    const QString certificatePath = resolvedIdentityPath(configuredCertificatePath);
    const QString keyPath = resolvedIdentityPath(configuredKeyPath);
    if (!refresh && (pathExists(certificatePath) || pathExists(keyPath))) {
        callback(false, errorString);
        return;
    }

    QSslKey previousKey;
    QString previousSpkiFingerprint;
    if (refresh) {
        QByteArray certificateData;
        QByteArray keyData;
        if (!readFile(certificatePath, &certificateData) || !readFile(keyPath, &keyData)) {
            callback(false, tr("The existing PCE TLS client certificate or private key could not be read."));
            return;
        }
        const QSslCertificate previousCertificate(certificateData, QSsl::Pem);
        previousKey = QSslKey(keyData, QSsl::Ec, QSsl::Pem, QSsl::PrivateKey);
        if (previousCertificate.isNull() || previousKey.isNull()
            || previousCertificate.publicKey().algorithm() != QSsl::Ec) {
            callback(false, tr("The existing PCE TLS identity is not a valid EC certificate and private key."));
            return;
        }
        previousSpkiFingerprint = spkiSha256Fingerprint(previousCertificate);
        if (previousSpkiFingerprint.isEmpty()) {
            callback(false, tr("The public key fingerprint of the existing PCE TLS certificate could not be calculated."));
            return;
        }
    }

    if (m_identityProcess) {
        if (m_identityRefreshInProgress != refresh) {
            callback(false, tr("Another PCE TLS identity operation is already in progress."));
            return;
        }
        m_identityCallbacks.append(callback);
        return;
    }
    m_identityCallbacks.append(callback);
    m_identityRefreshInProgress = refresh;

    m_identityTemporaryDirectory = new QTemporaryDir();
    if (!m_identityTemporaryDirectory->isValid()) {
        delete m_identityTemporaryDirectory;
        m_identityTemporaryDirectory = nullptr;
        m_identityRefreshInProgress = false;
        const auto callbacks = m_identityCallbacks;
        m_identityCallbacks.clear();
        for (const auto &pendingCallback : callbacks)
            pendingCallback(false, tr("Could not create temporary files for the PCE TLS identity."));
        return;
    }

    const QString temporaryCertificate = m_identityTemporaryDirectory->filePath("client-certificate.pem");
    const QString temporaryKey = m_identityTemporaryDirectory->filePath("client-key.pem");
    m_identityProcess = new QProcess(this);
    connect(m_identityProcess, &QProcess::errorOccurred, this, [this](QProcess::ProcessError processError) {
        if (processError != QProcess::FailedToStart)
            return;
        const QString generationError = tr("OpenSSL could not be started: %1").arg(m_identityProcess->errorString());
        m_identityProcess->deleteLater();
        m_identityProcess = nullptr;
        m_identityRefreshInProgress = false;
        delete m_identityTemporaryDirectory;
        m_identityTemporaryDirectory = nullptr;
        const auto callbacks = m_identityCallbacks;
        m_identityCallbacks.clear();
        for (const auto &pendingCallback : callbacks)
            pendingCallback(false, generationError);
    });
    connect(m_identityProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            [this, refresh, certificatePath, keyPath, temporaryCertificate, temporaryKey,
             previousKey, previousSpkiFingerprint](int exitCode, QProcess::ExitStatus exitStatus) {
        bool success = exitStatus == QProcess::NormalExit && exitCode == 0;
        QString generationError;
        if (!success) {
            generationError = tr("OpenSSL could not generate the PCE TLS identity: %1")
                                  .arg(QString::fromLocal8Bit(m_identityProcess->readAllStandardError()));
        } else {
            QFile generatedCertificate(temporaryCertificate);
            success = generatedCertificate.open(QIODevice::ReadOnly);
            const QByteArray certificateData = success ? generatedCertificate.readAll() : QByteArray();
            const QSslCertificate certificate(certificateData, QSsl::Pem);
            if (refresh) {
                success = success && certificateIsUsable(certificate)
                          && certificate.publicKey().algorithm() == QSsl::Ec
                          && spkiSha256Fingerprint(certificate) == previousSpkiFingerprint;
                QByteArray previousCertificateData;
                success = success && readFile(certificatePath, &previousCertificateData);
                if (success)
                    success = writeIdentityFile(certificatePath, certificateData);
                if (!success && !previousCertificateData.isEmpty()
                    && !writeIdentityFile(certificatePath, previousCertificateData)) {
                    qCCritical(dcPcElectric()) << "Could not restore the previous PCE TLS client certificate.";
                }
                if (success) {
                    m_clientCertificate = certificate;
                    m_clientPrivateKey = previousKey;
                    qCInfo(dcPcElectric()) << "Refreshed PCE TLS client certificate at" << certificatePath
                                           << "while retaining private key" << keyPath
                                           << "SPKI-SHA256:" << previousSpkiFingerprint;
                }
            } else {
                QFile generatedKey(temporaryKey);
                success = success && generatedKey.open(QIODevice::ReadOnly);
                const QByteArray keyData = success ? generatedKey.readAll() : QByteArray();
                const QSslKey key(keyData, QSsl::Ec, QSsl::Pem, QSsl::PrivateKey);
                success = success && certificateIsUsable(certificate) && !key.isNull()
                          && certificate.publicKey().algorithm() == key.algorithm();

                const bool certificateExisted = pathExists(certificatePath);
                const bool keyExisted = pathExists(keyPath);
                QByteArray previousCertificateData;
                QByteArray previousKeyData;
                success = success && (!certificateExisted || readFile(certificatePath, &previousCertificateData))
                          && (!keyExisted || readFile(keyPath, &previousKeyData));
                if (success)
                    success = writeIdentityFile(keyPath, keyData)
                              && writeIdentityFile(certificatePath, certificateData);
                if (!success) {
                    const bool certificateRestored = certificateExisted
                                                         ? writeIdentityFile(certificatePath, previousCertificateData)
                                                         : (!pathExists(certificatePath) || QFile::remove(certificatePath));
                    const bool keyRestored = keyExisted
                                                 ? writeIdentityFile(keyPath, previousKeyData)
                                                 : (!pathExists(keyPath) || QFile::remove(keyPath));
                    if (!certificateRestored || !keyRestored)
                        qCCritical(dcPcElectric()) << "Could not fully restore the previous PCE TLS client identity.";
                } else {
                    m_clientCertificate = certificate;
                    m_clientPrivateKey = key;
                    qCInfo(dcPcElectric()) << "Generated PCE TLS client certificate at" << certificatePath
                                           << "and private key at" << keyPath
                                           << "SPKI-SHA256:" << spkiSha256Fingerprint(certificate);
                }
            }
            if (!success && generationError.isEmpty())
                generationError = refresh
                                      ? tr("Could not refresh the PCE TLS certificate without changing its public key.")
                                      : tr("Could not store the generated PCE TLS identity.");
        }

        m_identityProcess->deleteLater();
        m_identityProcess = nullptr;
        m_identityRefreshInProgress = false;
        delete m_identityTemporaryDirectory;
        m_identityTemporaryDirectory = nullptr;
        const auto callbacks = m_identityCallbacks;
        m_identityCallbacks.clear();
        for (const auto &pendingCallback : callbacks)
            pendingCallback(success, generationError);
    });
    QStringList arguments = {
        QStringLiteral("req"), QStringLiteral("-x509"), QStringLiteral("-new"),
        QStringLiteral("-sha256"), QStringLiteral("-days"), QStringLiteral("3650"),
        QStringLiteral("-subj"), QStringLiteral("/CN=nymea PCE Modbus client"),
        QStringLiteral("-addext"), QStringLiteral("basicConstraints=critical,CA:FALSE"),
        QStringLiteral("-addext"), QStringLiteral("keyUsage=critical,digitalSignature"),
        QStringLiteral("-addext"), QStringLiteral("extendedKeyUsage=clientAuth"),
    };
    if (refresh) {
        arguments << QStringLiteral("-key") << keyPath;
    } else {
        arguments << QStringLiteral("-newkey") << QStringLiteral("ec")
                  << QStringLiteral("-pkeyopt") << QStringLiteral("ec_paramgen_curve:P-256")
                  << QStringLiteral("-nodes") << QStringLiteral("-keyout") << temporaryKey;
    }
    arguments << QStringLiteral("-out") << temporaryCertificate;
    m_identityProcess->start(QStringLiteral("openssl"), arguments);
}

bool IntegrationPluginPcElectric::configureTls(Thing *thing, PceWallbox *connection)
{
    QString errorString;
    if (!ensureClientIdentity(&errorString)) {
        qCWarning(dcPcElectric()) << errorString;
        return false;
    }

    QString configuredCertificatePath = configValue(pcElectricPluginClientCertificatePathParamTypeId).toString();
    if (configuredCertificatePath.isEmpty())
        configuredCertificatePath = QStringLiteral("pcelectric/client-certificate.pem");
    qCInfo(dcPcElectric()) << "Using PCE TLS client certificate"
                           << resolvedIdentityPath(configuredCertificatePath)
                           << "subject CN:" << m_clientCertificate.subjectInfo(QSslCertificate::CommonName).join(", ")
                           << "valid from:" << m_clientCertificate.effectiveDate().toString(Qt::ISODate)
                           << "until:" << m_clientCertificate.expiryDate().toString(Qt::ISODate)
                           << "SPKI-SHA256:" << spkiSha256Fingerprint(m_clientCertificate)
                           << "certificate SHA-256:"
                           << QString::fromLatin1(m_clientCertificate.digest(QCryptographicHash::Sha256).toHex());

    ModbusTcpMaster *master = connection->modbusTcpMaster();
    const QString storedFingerprint = pluginStorage()->value(storagePrefix(thing) + "/serverFingerprint").toString();
    if (!storedFingerprint.isEmpty() && !master->setAcceptedPeerCertificateFingerprint(storedFingerprint))
        return false;
    qCDebug(dcPcElectric()) << "Configuring PCE TLS fingerprint pinning"
                            << "pin fingerprint:"
                            << (storedFingerprint.isEmpty() ? QStringLiteral("<TOFU: pending server certificate>")
                                                            : master->acceptedPeerCertificateFingerprint());

    master->setTransport(ModbusTcpMaster::TransportTls);
    master->setPort(802);
    master->setTlsServerName(QString());
    QSslConfiguration configuration = QSslConfiguration::defaultConfiguration();
    configuration.setProtocol(QSsl::TlsV1_2);
    configuration.setLocalCertificate(m_clientCertificate);
    configuration.setPrivateKey(m_clientPrivateKey);
    master->setTlsConfiguration(configuration);

    connect(master, &ModbusTcpMaster::tlsHandshakeFinished, connection,
            [master](const QSslConfiguration &negotiatedConfiguration) {
        qCInfo(dcPcElectric()) << "PCE TLS handshake finished with"
                               << master->connectionUrl()
                               << "protocol:" << negotiatedConfiguration.sessionProtocol()
                               << "cipher:" << negotiatedConfiguration.sessionCipher().name();
    });
    connect(master, &ModbusTcpMaster::tlsErrors, connection, [](const QList<QSslError> &errors) {
        QStringList errorStrings;
        for (const QSslError &error : errors)
            errorStrings.append(error.errorString());
        qCDebug(dcPcElectric()) << "PCE TLS handshake reported SSL errors:" << errorStrings;
    });

    if (!storedFingerprint.isEmpty()) {
        connect(master, &ModbusTcpMaster::peerCertificateAvailable, connection,
                [master, storedFingerprint](const QSslCertificate &certificate,
                                            const QString &spkiFingerprint) {
            const QString legacyCertificateFingerprint = QString::fromLatin1(
                certificate.digest(QCryptographicHash::Sha256).toHex());
            if (storedFingerprint != legacyCertificateFingerprint
                || storedFingerprint == spkiFingerprint) {
                return;
            }

            qCInfo(dcPcElectric()) << "Migrating stored PCE TLS server pin from certificate SHA-256"
                                   << storedFingerprint << "to SPKI-SHA256" << spkiFingerprint;
            master->setAcceptedPeerCertificateFingerprint(spkiFingerprint);
        }, Qt::DirectConnection);
    }

    if (storedFingerprint.isEmpty()) {
        connect(master, &ModbusTcpMaster::peerCertificateAvailable, connection,
                [master](const QSslCertificate &, const QString &fingerprint) {
            if (master->acceptedPeerCertificateFingerprint().isEmpty())
                master->setAcceptedPeerCertificateFingerprint(fingerprint);
        }, Qt::DirectConnection);
    }
    connect(master, &ModbusTcpMaster::peerCertificateAvailable, connection,
            [master](const QSslCertificate &, const QString &serverFingerprint) {
        const QString pinFingerprint = master->acceptedPeerCertificateFingerprint();
        qCDebug(dcPcElectric()) << "PCE TLS server SPKI fingerprint received"
                                << "pin SPKI-SHA256:" << pinFingerprint
                                << "server SPKI-SHA256:" << serverFingerprint
                                << "match:" << (!pinFingerprint.isEmpty()
                                                 && pinFingerprint == serverFingerprint);
    });
    return true;
}
