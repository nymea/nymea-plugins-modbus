/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2024, nymea GmbH
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

#include "sunnywebboxdiscovery.h"
#include "sunnywebbox.h"

#include "extern-plugininfo.h"

#include <QJsonDocument>

SunnyWebBoxDiscovery::SunnyWebBoxDiscovery(NetworkAccessManager *networkAccessManager, NetworkDeviceDiscovery *networkDeviceDiscovery, QObject *parent) :
    QObject(parent),
    m_networkAccessManager(networkAccessManager),
    m_networkDeviceDiscovery(networkDeviceDiscovery)
{

}

void SunnyWebBoxDiscovery::startDiscovery()
{
    // Clean up
    m_discoveredHosts.clear();
    m_discoveryResults.clear();

    m_startDateTime = QDateTime::currentDateTime();

    qCInfo(dcSma()) << "Discovery: SunnyWebBox: Starting network discovery...";
    m_discoveryReply = m_networkDeviceDiscovery->discover();

    // Test any network device beeing discovered
    connect(m_discoveryReply, &NetworkDeviceDiscoveryReply::hostAddressDiscovered, this, &SunnyWebBoxDiscovery::checkNetworkDevice);
    connect(m_discoveryReply, &NetworkDeviceDiscoveryReply::finished, this, [=](){
        // The network device discovery is done
        m_networkDeviceInfos = m_discoveryReply->networkDeviceInfos();

        m_discoveryReply->deleteLater();
        m_discoveryReply = nullptr;

        // If there might be some response after the grace period time,
        // we don't care any more since there might just waiting for some timeouts...
        // If there would be a device, if would have responded.
        QTimer::singleShot(3000, this, [this](){
            qCDebug(dcSma()) << "Discovery: SunnyWebBox: Grace period timer triggered.";
            finishDiscovery();
        });
    });
}

NetworkDeviceInfos SunnyWebBoxDiscovery::discoveryResults() const
{
    return m_discoveryResults;
}

void SunnyWebBoxDiscovery::checkNetworkDevice(const QHostAddress &address)
{
    // Make a simple request and verify if it worked and the expected data gets returned.
    SunnyWebBox webBox(m_networkAccessManager, address, this);
    QNetworkReply *reply = webBox.sendRequest(address, "GetPlantOverview");
    m_pendingReplies.append(reply);
    connect(reply, &QNetworkReply::finished, this, [=](){
        m_pendingReplies.removeAll(reply);
        reply->deleteLater();

        // Check HTTP reply
        if (reply->error() != QNetworkReply::NoError) {
            qCDebug(dcSma()) << "Discovery: SunnyWebBox: Checked" << address.toString()
                             << "and a HTTP error occurred:" << reply->errorString() << "Continue...";
            return;
        }

        QByteArray data = reply->readAll();

        // Check JSON
        QJsonParseError error;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &error);
        if (error.error != QJsonParseError::NoError) {
            qCDebug(dcSma()) << "Discovery: SunnyWebBox: Checked" << address.toString()
                             << "and received invalid JSON data:" << error.errorString() << "Continue...";
            return;
        }

        if (!jsonDoc.isObject()) {
            qCDebug(dcSma()) << "Discovery: SunnyWebBox: Response JSON is not an Object" << address.toString() << "Continue...";
            return;
        }

        QVariantMap map = jsonDoc.toVariant().toMap();
        if (map["version"] != "1.0") {
            qCDebug(dcSma()) << "Discovery: SunnyWebBox: API version not supported on" << address.toString() << "Continue...";;
            return;
        }

        if (map.contains("proc") && map.contains("result")) {
            // Ok, seems to be a Sunny WebBox we are talking to...add to the discovery results...
            qCDebug(dcSma()) << "Discovery: SunnyWebBox: --> Found Sunny WebBox on" << address;
            m_discoveredHosts.append(address);
        } else {
            qCDebug(dcSma()) << "Discovery: SunnyWebBox: Missing proc or result value in response from" << address.toString() << "Continue...";
            return;
        }
    });
}

void SunnyWebBoxDiscovery::cleanupPendingReplies()
{
    foreach (QNetworkReply *reply, m_pendingReplies) {
        reply->abort();
    }
}

void SunnyWebBoxDiscovery::finishDiscovery()
{
    qint64 durationMilliSeconds = QDateTime::currentMSecsSinceEpoch() - m_startDateTime.toMSecsSinceEpoch();

    foreach (const QHostAddress &address, m_discoveredHosts)
        m_discoveryResults.append(m_networkDeviceInfos.get(address));

    qCInfo(dcSma()) << "Discovery: SunnyWebBox: Finished the discovery process. Found" << m_discoveryResults.length()
                    << "Sunny WebBoxes in" << QTime::fromMSecsSinceStartOfDay(durationMilliSeconds).toString("mm:ss.zzz");

    cleanupPendingReplies();
    emit discoveryFinished();
}
