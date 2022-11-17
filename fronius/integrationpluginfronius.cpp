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

#include "integrationpluginfronius.h"
#include "plugintimer.h"
#include "network/networkaccessmanager.h"

#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QPointer>
#include "plugininfo.h"
#include <QDebug>

// Notes: Test IPs: 93.82.221.82 | 88.117.152.99

IntegrationPluginFronius::IntegrationPluginFronius(QObject *parent): IntegrationPlugin(parent){

}

void IntegrationPluginFronius::setupThing(ThingSetupInfo *info)
{
    qCDebug(dcFronius()) << "Setting up a new thing:" << info->thing()->name();

    Thing *thing = info->thing();

    if (thing->thingClassId() == dataloggerThingClassId) {
        //check if a data logger is already added with this IPv4Address
        foreach(FroniusLogger *logger, m_froniusLoggers.keys()){
            if(logger->hostAddress() == thing->paramValue(dataloggerThingLoggerHostParamTypeId).toString()){
                //this logger at this IPv4 address is already added
                qCWarning(dcFronius()) << "thing at " << thing->paramValue(dataloggerThingLoggerHostParamTypeId).toString() << " already added!";
                info->finish(Thing::ThingErrorThingInUse);
                return;
            }
        }

        // Perform a HTTP request on the given IPv4Address to find things
        QUrl requestUrl;
        requestUrl.setScheme("http");
        requestUrl.setHost(thing->paramValue(dataloggerThingLoggerHostParamTypeId).toString());
        requestUrl.setPath("/solar_api/GetAPIVersion.cgi");

        qCDebug(dcFronius()) << "Search at address" << requestUrl.toString();

        QNetworkReply *reply = hardwareManager()->networkManager()->get(QNetworkRequest(requestUrl));
        connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
        connect(reply, &QNetworkReply::finished, [this, info, thing, reply]() {
            QByteArray data = reply->readAll();

            if (reply->error() != QNetworkReply::NoError) {
                qCWarning(dcFronius()) << "Fronius: Network request error:" << reply->error() << reply->errorString() << reply->url();
                info->finish(Thing::ThingErrorHardwareNotAvailable, tr("Device not reachable"));
                return;
            }

            // Convert the rawdata to a JSON document
            QJsonParseError error;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &error);
            if (error.error != QJsonParseError::NoError) {
                qCWarning(dcFronius()) << "Fronius: Failed to parse JSON data" << data << ":" << error.errorString() << data;
                info->finish(Thing::ThingErrorHardwareFailure, tr("Please try again"));
                return;
            }

            FroniusLogger *newLogger = new FroniusLogger(thing, this);
            newLogger->setBaseUrl(jsonDoc.toVariant().toMap().value("BaseURL").toString());
            newLogger->setHostAddress(thing->paramValue(dataloggerThingLoggerHostParamTypeId).toString());
            m_froniusLoggers.insert(newLogger, thing);

            info->finish(Thing::ThingErrorNoError);
        });
        //Async Setup
    } else if (thing->thingClassId() == inverterThingClassId) {

        FroniusInverter *newInverter = new FroniusInverter(thing,this);
        newInverter->setDeviceId(thing->paramValue(inverterThingIdParamTypeId).toString());
        newInverter->setBaseUrl(thing->paramValue(inverterThingBaseParamTypeId).toString());
        newInverter->setHostAddress(thing->paramValue(inverterThingHostParamTypeId).toString());

        m_froniusInverters.insert(newInverter,thing);

        // get inverter unique ID
        QUrl requestUrl;
        requestUrl.setScheme("http");
        requestUrl.setHost(newInverter->hostAddress());
        requestUrl.setPath(newInverter->baseUrl() + "GetInverterInfo.cgi");

        QNetworkReply *reply = hardwareManager()->networkManager()->get(QNetworkRequest(requestUrl));
        connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
        connect(reply, &QNetworkReply::finished, [this, info, newInverter, reply]() {
            QByteArray data = reply->readAll();
            reply->deleteLater();

            if (reply->error() != QNetworkReply::NoError) {
                qCWarning(dcFronius()) << "Fronius: Network request error:" << reply->error() << reply->errorString();
                info->finish(Thing::ThingErrorHardwareNotAvailable, "Device not reachable");
                return;
            }

            // Convert the rawdata to a json document
            QJsonParseError error;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &error);
            if (error.error != QJsonParseError::NoError) {
                qCWarning(dcFronius()) << "Fronius: Failed to parse JSON data" << data << ":" << error.errorString();
                info->finish(Thing::ThingErrorHardwareNotAvailable, "Please try again");
                return;
            }

            // Check reply information
            QVariantMap dataMap = jsonDoc.toVariant().toMap().value("Body").toMap().value("Data").toMap();
            // check for thing id in reply
            if (dataMap.contains(newInverter->deviceId())) {
                qCDebug(dcFronius()) << "Found Thing with unique:" << dataMap.value(newInverter->deviceId()).toMap().value("UniqueID").toString();
                newInverter->setUniqueId(dataMap.value(newInverter->deviceId()).toMap().value("UniqueID").toString());
                newInverter->pluginThing()->setParamValue(inverterThingUniqueIdParamTypeId,newInverter->uniqueId());
                qCDebug(dcFronius()) << "Stored unique ID:" << newInverter->uniqueId();
            }
            info->finish(Thing::ThingErrorNoError);
        });
        // Async Setup
    } else if (thing->thingClassId() == storageThingClassId) {

        FroniusStorage *newStorage = new FroniusStorage(thing, this);
        newStorage->setDeviceId(thing->paramValue(storageThingIdParamTypeId).toString());
        newStorage->setBaseUrl(thing->paramValue(storageThingBaseParamTypeId).toString());
        newStorage->setHostAddress(thing->paramValue(storageThingHostParamTypeId).toString());
        m_froniusStorages.insert(newStorage,thing);

        // Get storage manufacturer and maximum capacity
        QUrlQuery query;
        QUrl requestUrl;
        requestUrl.setScheme("http");
        requestUrl.setHost(newStorage->hostAddress());
        requestUrl.setPath(newStorage->baseUrl() + "GetStorageRealtimeData.cgi");
        query.addQueryItem("Scope","Device");
        query.addQueryItem("DeviceId", newStorage->deviceId());
        requestUrl.setQuery(query);
        qCDebug(dcFronius()) << "Get Storage Data at address" << requestUrl.toString();
        QNetworkReply *reply = hardwareManager()->networkManager()->get(QNetworkRequest(requestUrl));
        connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
        connect(reply, &QNetworkReply::finished, [this, info, newStorage, reply]() {
            QByteArray data = reply->readAll();

            if (reply->error() != QNetworkReply::NoError) {
                qCWarning(dcFronius()) << "Fronius: Network request error:" << reply->error() << reply->errorString();
                info->finish(Thing::ThingErrorNoError);
                return;
            }

            // Convert the rawdata to a json document
            QJsonParseError error;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &error);
            if (error.error != QJsonParseError::NoError) {
                qCWarning(dcFronius()) << "Fronius: Failed to parse JSON data" << data << ":" << error.errorString();
                info->finish(Thing::ThingErrorHardwareFailure, tr("Please try again"));
                return;
            }

            // Create StorageInfo list map
            QVariantMap storageInfoMap = jsonDoc.toVariant().toMap().value("Body").toMap().value("Data").toMap().value("Controller").toMap();

            newStorage->pluginThing()->setParamValue(storageThingManufacturerParamTypeId, storageInfoMap.value("Details").toMap().value("Manufacturer").toString());
            newStorage->pluginThing()->setParamValue(storageThingCapacityParamTypeId, storageInfoMap.value("Capacity_Maximum").toInt());
            info->finish(Thing::ThingErrorNoError);
        });
        //Async Setup
    } else if (thing->thingClassId() == meterThingClassId) {

        FroniusMeter *newMeter = new FroniusMeter(thing, this);;
        newMeter->setDeviceId(thing->paramValue(meterThingIdParamTypeId).toString());
        newMeter->setBaseUrl(thing->paramValue(meterThingBaseParamTypeId).toString());
        newMeter->setHostAddress(thing->paramValue(meterThingHostParamTypeId).toString());

        m_froniusMeters.insert(newMeter, thing);
        info->finish(Thing::ThingErrorNoError);
        //Async setup
    } else if (thing->thingClassId() == sunspecStorageThingClassId) {
        SunspecThing *sunspecThings = new SunspecThing(thing, this);
        m_sunspecThings.insert(sunspecThings, thing);
        info->finish(Thing::ThingErrorNoError);
    } else {
        Q_ASSERT_X(false, "setupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginFronius::postSetupThing(Thing *thing)
{
    qCDebug(dcFronius()) << "Post setup" << thing->name();

    if (!m_pluginTimer) {
        m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(30);
        connect(m_pluginTimer, &PluginTimer::timeout, this, [this]() {
            foreach (Thing *logger, m_froniusLoggers)
                updateThingStates(logger);

            foreach (Thing *inverter, m_froniusInverters)
                updateThingStates(inverter);

            foreach (Thing *meter, m_froniusMeters)
                updateThingStates(meter);

            foreach (Thing *storage, m_froniusStorages)
                updateThingStates(storage);

            foreach (SunspecThing *sunspecThing, m_sunspecThings.keys())
                sunspecThing->update();
        });
    }

    if (thing->thingClassId() == dataloggerThingClassId) {
        searchNewThings(m_froniusLoggers.key(thing));
        updateThingStates(thing);
    } else if (thing->thingClassId() == inverterThingClassId) {
        updateThingStates(thing);
    } else if (thing->thingClassId() == meterThingClassId) {
        updateThingStates(thing);
    } else if (thing->thingClassId() == storageThingClassId) {
        updateThingStates(thing);
    } else if (thing->thingClassId() == sunspecStorageThingClassId) {
        SunspecThing *sunspecThing = m_sunspecThings.key(thing);
        sunspecThing->update();
    } else {
        Q_ASSERT_X(false, "postSetupThing", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginFronius::thingRemoved(Thing *thing)
{
    if (thing->thingClassId() == dataloggerThingClassId) {
        FroniusLogger *logger = m_froniusLoggers.key(thing);
        m_froniusLoggers.remove(logger);
        logger->deleteLater();
        return;
    } else if (thing->thingClassId() == inverterThingClassId) {
        FroniusInverter *inverter = m_froniusInverters.key(thing);
        m_froniusInverters.remove(inverter);
        inverter->deleteLater();
        return;
    } else if (thing->thingClassId() == meterThingClassId) {
        FroniusMeter *meter = m_froniusMeters.key(thing);
        m_froniusMeters.remove(meter);
        meter->deleteLater();
        return;
    } else if (thing->thingClassId() == storageThingClassId) {
        FroniusStorage *storage = m_froniusStorages.key(thing);
        m_froniusStorages.remove(storage);
        storage->deleteLater();
        return;
    } else if (thing->thingClassId() == sunspecStorageThingClassId) {
        qCDebug(dcFronius()) << "Remove sunspec thing";
        SunspecThing *sunspecThing = m_sunspecThings.key(thing);
        m_sunspecThings.remove(sunspecThing);
        delete sunspecThing;
        qCDebug(dcFronius()) << "Sunspec thing deleted";
        return;
    } else {
        Q_ASSERT_X(false, "thingRemoved", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }

    if (myThings().isEmpty()) {
        hardwareManager()->pluginTimerManager()->unregisterTimer(m_pluginTimer);
        m_pluginTimer = nullptr;
    }
}


void IntegrationPluginFronius::executeAction(ThingActionInfo *info)
{
    Action action = info->action();
    Thing *thing = info->thing();
    qCDebug(dcFronius()) << "Execute action" << thing->name() << action.actionTypeId().toString();

    if (thing->thingClassId() == dataloggerThingClassId) {

        if (action.actionTypeId() == dataloggerSearchDevicesActionTypeId) {
            searchNewThings(m_froniusLoggers.key(thing));
            info->finish(Thing::ThingErrorNoError);
        } else {
            Q_ASSERT_X(false, "executeAction", QString("Unhandled action: %1").arg(action.actionTypeId().toString()).toUtf8());
        }
    } else if (thing->thingClassId() == sunspecStorageThingClassId) {
        SunspecThing *sunspecThing = m_sunspecThings.key(thing);
        if (!sunspecThing) {
            qWarning(dcFronius()) << "Could not find sunspec instance for thing";
            info->finish(Thing::ThingErrorHardwareNotAvailable);
            return;
        }

        if (action.actionTypeId() == sunspecStorageGridChargingActionTypeId) {
            QUuid requestId = sunspecThing->setGridCharging(action.param(sunspecStorageGridChargingActionGridChargingParamTypeId).value().toBool());
            if (requestId.isNull()) {
                info->finish(Thing::ThingErrorHardwareFailure);
            } else {
                m_asyncActions.insert(requestId, info);
                connect(info, &ThingActionInfo::aborted, this, [requestId, this] {m_asyncActions.remove(requestId);});
            }
        } else if (action.actionTypeId() == sunspecStorageEnableChargingLimitActionTypeId) {
            int value = (action.param(sunspecStorageEnableChargingLimitActionEnableChargingLimitParamTypeId).value().toBool() << 1) | thing->stateValue(sunspecStorageEnableDischargingLimitStateTypeId).toBool();
            QUuid requestId = sunspecThing->setStorageControlMode(value);
            if (requestId.isNull()) {
                info->finish(Thing::ThingErrorHardwareFailure);
            } else {
                m_asyncActions.insert(requestId, info);
                connect(info, &ThingActionInfo::aborted, this, [requestId, this] {m_asyncActions.remove(requestId);});
            }
        } else if (action.actionTypeId() == sunspecStorageChargingRateActionTypeId) {
            QUuid requestId = sunspecThing->setChargingRate(action.param(sunspecStorageChargingRateActionChargingRateParamTypeId).value().toInt());
            if (requestId.isNull()) {
                info->finish(Thing::ThingErrorHardwareFailure);
            } else {
                m_asyncActions.insert(requestId, info);
                connect(info, &ThingActionInfo::aborted, this, [requestId, this] {m_asyncActions.remove(requestId);});
            }
        } else if (action.actionTypeId() == sunspecStorageEnableDischargingLimitActionTypeId) {
            int value = (action.param(sunspecStorageEnableDischargingLimitActionEnableDischargingLimitParamTypeId).value().toBool() << 1) | thing->stateValue(sunspecStorageEnableChargingLimitStateTypeId).toBool();
            QUuid requestId = sunspecThing->setStorageControlMode(value);
            if (requestId.isNull()) {
                info->finish(Thing::ThingErrorHardwareFailure);
            } else {
                m_asyncActions.insert(requestId, info);
                connect(info, &ThingActionInfo::aborted, this, [requestId, this] {m_asyncActions.remove(requestId);});
            }
        } else if (action.actionTypeId() == sunspecStorageDischargingRateActionTypeId) {
            QUuid requestId = sunspecThing->setDischargingRate(action.param(sunspecStorageDischargingRateActionDischargingRateParamTypeId).value().toInt());
            if (requestId.isNull()) {
                info->finish(Thing::ThingErrorHardwareFailure);
            } else {
                m_asyncActions.insert(requestId, info);
                connect(info, &ThingActionInfo::aborted, this, [requestId, this] {m_asyncActions.remove(requestId);});
            }
        } else {
            Q_ASSERT_X(false, "executeAction", QString("Unhandled action: %1").arg(action.actionTypeId().toString()).toUtf8());
        }
    } else {
        Q_ASSERT_X(false, "executeAction", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());
    }
}

void IntegrationPluginFronius::updateThingStates(Thing *thing)
{
    qCDebug(dcFronius()) << "Update thing values for" << thing->name();

    if(thing->thingClassId() == inverterThingClassId) {
        QNetworkReply *reply = hardwareManager()->networkManager()->get(QNetworkRequest(m_froniusInverters.key(thing)->updateUrl()));
        connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
        connect(reply, &QNetworkReply::finished, [this, thing, reply]() {

            if (reply->error() != QNetworkReply::NoError) {
                qCWarning(dcFronius()) << "Network request error:" << reply->error() << reply->errorString() << reply->request().url();
                return;
            }
            QByteArray data = reply->readAll();
            if(m_froniusInverters.values().contains(thing)){ // check if thing was not removed before reply was received
                m_froniusInverters.key(thing)->updateThingInfo(data);
            }
        });
        QNetworkReply *next_reply = hardwareManager()->networkManager()->get(QNetworkRequest(m_froniusInverters.key(thing)->activityUrl()));
        connect(next_reply, &QNetworkReply::finished, next_reply, &QNetworkReply::deleteLater);
        connect(next_reply, &QNetworkReply::finished, [this, thing, next_reply]() {
            if (next_reply->error() != QNetworkReply::NoError) {
                qCWarning(dcFronius()) << "Network request error:" << next_reply->error() << next_reply->errorString() << next_reply->request().url();
                return;
            }
            QByteArray data = next_reply->readAll();
            if(m_froniusInverters.values().contains(thing)){ // check if thing was not removed before reply was received
                m_froniusInverters.key(thing)->updateActivityInfo(data);
            }
        });
    } else if (thing->thingClassId() == dataloggerThingClassId) {
        QNetworkReply *reply = hardwareManager()->networkManager()->get(QNetworkRequest(m_froniusLoggers.key(thing)->updateUrl()));
        connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
        connect(reply, &QNetworkReply::finished, [this, thing, reply]() {

            if (reply->error() != QNetworkReply::NoError) {
                qCWarning(dcFronius()) << "Network request error:" << reply->error() << reply->errorString() << reply->request().url();
                return;
            }
            QByteArray data = reply->readAll();
            if(m_froniusLoggers.values().contains(thing)){ // check if thing was not removed before reply was received
                m_froniusLoggers.key(thing)->updateThingInfo(data);
            }
        });

    } else if (thing->thingClassId() == meterThingClassId) {
        QNetworkReply *reply = hardwareManager()->networkManager()->get(QNetworkRequest(m_froniusMeters.key(thing)->updateUrl()));
        connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
        connect(reply, &QNetworkReply::finished, [this, thing, reply]() {
            if (reply->error() != QNetworkReply::NoError) {
                qCWarning(dcFronius()) << "Network request error:" << reply->error() << reply->errorString() << reply->request().url();
                return;
            }
            QByteArray data = reply->readAll();
            if(m_froniusMeters.values().contains(thing)){ // check if thing was not removed before reply was received
                m_froniusMeters.key(thing)->updateThingInfo(data);
            }
        });
        QNetworkReply *next_reply = hardwareManager()->networkManager()->get(QNetworkRequest(m_froniusMeters.key(thing)->activityUrl()));
        connect(next_reply, &QNetworkReply::finished, next_reply, &QNetworkReply::deleteLater);
        connect(next_reply, &QNetworkReply::finished, [this, thing, next_reply]() {

            if (next_reply->error() != QNetworkReply::NoError) {
                qCWarning(dcFronius()) << "Network request error:" << next_reply->error() << next_reply->errorString() << next_reply->request().url();
                return;
            }
            QByteArray data = next_reply->readAll();
            if(m_froniusMeters.values().contains(thing)){ // check if thing was not removed before reply was received
                m_froniusMeters.key(thing)->updateActivityInfo(data);
            }
        });
    } else if (thing->thingClassId() == storageThingClassId) {
        QNetworkReply *reply = hardwareManager()->networkManager()->get(QNetworkRequest(m_froniusStorages.key(thing)->updateUrl()));
        connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
        connect(reply, &QNetworkReply::finished, [this, thing, reply]() {

            if (reply->error() != QNetworkReply::NoError) {
                qCWarning(dcFronius()) << "Network request error:" << reply->error() << reply->errorString() << reply->request().url();
                return;
            }
            QByteArray data = reply->readAll();
            if(m_froniusStorages.values().contains(thing)){ // check if thing was not removed before reply was received
                m_froniusStorages.key(thing)->updateThingInfo(data);
            }
        });
        QNetworkReply *next_reply = hardwareManager()->networkManager()->get(QNetworkRequest(m_froniusStorages.key(thing)->activityUrl()));
        connect(next_reply, &QNetworkReply::finished, next_reply, &QNetworkReply::deleteLater);
        connect(next_reply, &QNetworkReply::finished, [this, thing, next_reply]() {
            next_reply->deleteLater();
            if (next_reply->error() != QNetworkReply::NoError) {
                qCWarning(dcFronius()) << "Network request error:" << next_reply->error() << next_reply->errorString() << next_reply->request().url();
                return;
            }
            QByteArray data = next_reply->readAll();
            if(m_froniusStorages.values().contains(thing)){ // check if thing was not removed before reply was received
                m_froniusStorages.key(thing)->updateActivityInfo(data);
            }
        });
    } else {
        Q_ASSERT_X(false, "updateThingState", QString("Unhandled thingClassId: %1").arg(thing->thingClassId().toString()).toUtf8());

    }
}

void IntegrationPluginFronius::searchNewThings(FroniusLogger *logger)
{
    QUrl url; QUrlQuery query;
    query.addQueryItem("DeviceClass", "System");
    url.setScheme("http");
    url.setHost(logger->hostAddress());
    url.setPath(logger->baseUrl() + "GetActiveDeviceInfo.cgi");
    url.setQuery(query);

    qCDebug(dcFronius()) << "Search Things at address" << url.toString();
    QNetworkRequest request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");

    QNetworkReply *reply = hardwareManager()->networkManager()->get(request);
    connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
    connect(reply, &QNetworkReply::finished, [this, logger, reply]() {

        if (reply->error() != QNetworkReply::NoError) {
            qCWarning(dcFronius()) << "Network request error:" << reply->error() << reply->errorString() << reply->request().url();
            return;
        }

        Thing *loggerThing = m_froniusLoggers.value(logger);
        if (!loggerThing)
            return;

        QByteArray data = reply->readAll();

        // Convert the rawdata to a json document
        QJsonParseError error;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &error);
        if (error.error != QJsonParseError::NoError) {
            qCWarning(dcFronius()) << "Fronius: Failed to parse JSON data" << data << ":" << error.errorString();
            return;
        }

        // Parse the data for thing information
        QList<ThingDescriptor> thingDescriptors;

        // Check reply information
        QVariantMap bodyMap = jsonDoc.toVariant().toMap().value("Body").toMap();

        // Parse reply for inverters at the host address
        QVariantMap inverterMap = bodyMap.value("Data").toMap().value("Inverter").toMap();
        foreach (QString inverterId, inverterMap.keys()) {
            //check if thing already connected to logger
            if(!existingThing(inverterThingIdParamTypeId,inverterId)) {
                QString thingName = loggerThing->name() + " Inverter " + inverterId;
                ThingDescriptor descriptor(inverterThingClassId, thingName, "Fronius Solar Inverter", loggerThing->id());
                ParamList params;
                params.append(Param(inverterThingHostParamTypeId, m_froniusLoggers.key(loggerThing)->hostAddress()));
                params.append(Param(inverterThingBaseParamTypeId, m_froniusLoggers.key(loggerThing)->baseUrl()));
                params.append(Param(inverterThingIdParamTypeId, inverterId));
                params.append(Param(inverterThingUniqueIdParamTypeId, ""));
                descriptor.setParams(params);
                thingDescriptors.append(descriptor);
            }
        }

        // parse reply for meter things at the host address
        QVariantMap meterMap = bodyMap.value("Data").toMap().value("Meter").toMap();
        foreach (QString meterId, meterMap.keys()) {
            //check if thing already connected to logger
            if(!existingThing(meterThingIdParamTypeId, meterId)) {
                QString thingName = loggerThing->name() + " Meter " + meterId;
                ThingDescriptor descriptor(meterThingClassId, thingName, "Fronius Solar Meter", loggerThing->id());
                ParamList params;
                params.append(Param(meterThingHostParamTypeId, m_froniusLoggers.key(loggerThing)->hostAddress()));
                params.append(Param(meterThingBaseParamTypeId, m_froniusLoggers.key(loggerThing)->baseUrl()));
                params.append(Param(meterThingIdParamTypeId, meterId));
                params.append(Param(meterThingUniqueIdParamTypeId, meterMap.value(meterId).toMap().value("Serial").toString()));
                descriptor.setParams(params);
                thingDescriptors.append(descriptor);
            }
        }

        // parse reply for storage things at the host address
        QVariantMap storageMap = bodyMap.value("Data").toMap().value("Storage").toMap();
        foreach (QString storageId, storageMap.keys()) {
            //check if thing already connected to logger
            if(!existingThing(storageThingIdParamTypeId,storageId)) {
                QString thingName = loggerThing->name() + " Storage " + storageId;
                ThingDescriptor descriptor(storageThingClassId, thingName, "Fronius Solar Storage", loggerThing->id());
                ParamList params;
                params.append(Param(storageThingManufacturerParamTypeId, ""));
                params.append(Param(storageThingCapacityParamTypeId, ""));
                params.append(Param(storageThingHostParamTypeId, m_froniusLoggers.key(loggerThing)->hostAddress()));
                params.append(Param(storageThingBaseParamTypeId, m_froniusLoggers.key(loggerThing)->baseUrl()));
                params.append(Param(storageThingIdParamTypeId, storageId));
                params.append(Param(storageThingUniqueIdParamTypeId, storageMap.value(storageId).toMap().value("Serial").toString()));
                descriptor.setParams(params);
                thingDescriptors.append(descriptor);
            }
        }

        if (!thingDescriptors.empty()) {
            emit autoThingsAppeared(thingDescriptors);
            thingDescriptors.clear();
        }
    });
}

bool IntegrationPluginFronius::existingThing(ParamTypeId thingParamId, QString thingId)
{
    foreach(Thing *thing, myThings()) {
        if(thing->paramValue(thingParamId).toString() == thingId) {
            return true;
        }
    }
    return false;
}

