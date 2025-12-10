// SPDX-License-Identifier: GPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
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

#ifndef SUNNYWEBBOX_H
#define SUNNYWEBBOX_H

#include <integrations/thing.h>
#include <network/networkaccessmanager.h>

#include <QObject>
#include <QJsonObject>
#include <QHostAddress>
#include <QUdpSocket>
#include <QDateTime>

class SunnyWebBox : public QObject
{
    Q_OBJECT
public:
    struct Overview {
        double power;
        double dailyYield;
        double totalYield;
        QString status;
        QString error;
    };

    struct Device {
        QString key;
        QString name;
        QList<Device> childrens;
    };

    struct Channel {
        QString meta;
        QString name;
        QVariant value;
        QString unit;
    };

    struct Parameter {
        QString meta;
        QString name;
        QString unit;
        double min;
        double max;
        double value;
    };

    explicit SunnyWebBox(NetworkAccessManager *networkAccessManager, const QHostAddress &hostAddress, QObject *parrent = 0);
    ~SunnyWebBox();

    QString getPlantOverview(); // Returns an object with the following plant data: PAC, E-TODAY, E-TOTAL, MODE, ERROR
    QString getDevices();       // Returns a hierarchical list of all detected plant devices.
    QString getProcessDataChannels(const QString &deviceKey); //Returns a list with the meta names of the available process data channels for a particular device type.
    QString getProcessData(const QStringList &deviceKeys);    //Returns process data for up to 5 devices per request.
    QString getParameterChannels(const QString &deviceKey);   //Returns a list with the meta names of the available parameter channels for a particular device type
    QString getParameters(const QStringList &deviceKeys);     //Returns the parameter values of up to 5 devices
    QString setParameters(const QString &deviceKeys, const QHash<QString, QVariant> &channels); //Sets parameter values

    QHostAddress hostAddress() const;
    void setHostAddress(const QHostAddress &address);

    QNetworkReply *sendRequest(const QHostAddress &address, const QString &procedure, const QJsonObject &params = QJsonObject(), const QString &requestId = QString());

    static QString generateRequestId();

private:
    NetworkAccessManager *m_networkManager = nullptr;
    bool m_connected = false;
    QHostAddress m_hostAddresss;
    QDateTime m_lastRequest;

    QString sendMessage(const QHostAddress &address, const QString &procedure);
    QString sendMessage(const QHostAddress &address, const QString &procedure, const QJsonObject &params);
    void parseMessage(const QString &messageId, const QString &messageType, const QVariantMap &result);
    void setConnectionStatus(bool connected);

signals:
    void connectedChanged(bool connected);

    void plantOverviewReceived(const QString &messageId, Overview overview);
    void devicesReceived(const QString &messageId, QList<Device> devices);
    void processDataChannelsReceived(const QString &messageId, const QString &deviceKey, QStringList processDataChanels);
    void processDataReceived(const QString &messageId, const QString &deviceKey, const QHash<QString, QVariant> &channels);
    void parameterChannelsReceived(const QString &messageId, const QString &deviceKey, QStringList parameterChannels);
    void parametersReceived(const QString &messageId, const QString &deviceKey, const QList<Parameter> &parameters);
};

#endif // SUNNYWEBBOX_H
