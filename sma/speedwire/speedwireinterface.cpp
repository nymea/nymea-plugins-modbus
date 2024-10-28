/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2022, nymea GmbH
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

#include "speedwireinterface.h"
#include "extern-plugininfo.h"

#include <QNetworkInterface>

SpeedwireInterface::SpeedwireInterface(quint32 sourceSerialNumber, QObject *parent) :
    QObject(parent),
    m_sourceSerialNumber(sourceSerialNumber)
{
    m_unicast = new QUdpSocket(this);
    connect(m_unicast, &QUdpSocket::readyRead, this, [=](){
        QByteArray datagram;
        QHostAddress senderAddress;
        quint16 senderPort;

        while (m_unicast->hasPendingDatagrams()) {
            datagram.resize(m_unicast->pendingDatagramSize());
            m_unicast->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);

            qCDebug(dcSma()).noquote() << "SpeedwireInterface: Unicast socket received data from" << QString("%1:%2").arg(senderAddress.toString()).arg(senderPort);
            qCDebug(dcSma()) << "SpeedwireInterface: " << datagram.toHex();
            emit dataReceived(senderAddress, senderPort, datagram, false);
        }
    });

    connect(m_unicast, &QUdpSocket::stateChanged, this, [=](QAbstractSocket::SocketState socketState){
        qCDebug(dcSma()) << "SpeedwireInterface: Unicast socket state changed" << socketState;
    });

    connect(m_unicast, static_cast<void (QUdpSocket::*)( QAbstractSocket::SocketError )>(&QAbstractSocket::error), this, [=](QAbstractSocket::SocketError error){
        qCWarning(dcSma()) << "SpeedwireInterface: Unicast socket error occurred" << error << m_unicast->errorString();
    });


    m_multicast = new QUdpSocket(this);
    connect(m_multicast, &QUdpSocket::readyRead, this, [=](){
        QByteArray datagram;
        QHostAddress senderAddress;
        quint16 senderPort;

        while (m_multicast->hasPendingDatagrams()) {
            datagram.resize(m_multicast->pendingDatagramSize());
            m_multicast->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);

            // Filter our own requests on the multicast
            if (isOwnInterface(senderAddress))
                return;

            qCDebug(dcSma()).noquote() << "SpeedwireInterface: Multicast socket received data from" << QString("%1:%2").arg(senderAddress.toString()).arg(senderPort);
            //qCDebug(dcSma()) << "SpeedwireInterface: " << datagram.toHex();
            emit dataReceived(senderAddress, senderPort, datagram, true);
        }
    });

    connect(m_multicast, &QUdpSocket::stateChanged, this, [=](QAbstractSocket::SocketState socketState){
        qCDebug(dcSma()) << "SpeedwireInterface: Multicast socket state changed" << socketState;
    });
    connect(m_multicast, static_cast<void (QUdpSocket::*)( QAbstractSocket::SocketError )>(&QAbstractSocket::error), this, [=](QAbstractSocket::SocketError error){
        qCWarning(dcSma()) << "SpeedwireInterface: Multicast socket error occurred" << error << m_multicast->errorString();
    });

    if (initialize()) {
        qCDebug(dcSma()) << "SpeedwireInterface: Initialized sucessfully unicast and multicast interface.";
    } else {
        qCWarning(dcSma()) << "SpeedwireInterface: Failed to initialize.";
    }
}

SpeedwireInterface::~SpeedwireInterface()
{
    if (m_unicast)
        m_unicast->close();

    if (m_multicast) {
        if (!m_multicast->leaveMulticastGroup(Speedwire::multicastAddress())) {
            qCWarning(dcSma()) << "SpeedwireInterface: Failed to leave multicast group" << Speedwire::multicastAddress().toString();
        }

        m_multicast->close();
    }
}

bool SpeedwireInterface::available() const
{
    return m_available;
}

bool SpeedwireInterface::isOwnInterface(const QHostAddress &hostAddress)
{
    foreach (const QNetworkInterface &networkInterface, QNetworkInterface::allInterfaces()) {
        if (networkInterface.flags().testFlag(QNetworkInterface::IsLoopBack))
            continue;

        if (!networkInterface.flags().testFlag(QNetworkInterface::IsUp))
            continue;

        if (!networkInterface.flags().testFlag(QNetworkInterface::IsRunning))
            continue;

        foreach (const QNetworkAddressEntry &entry, networkInterface.addressEntries()) {

            // Only IPv4
            if (entry.ip().protocol() != QAbstractSocket::IPv4Protocol)
                continue;

            if (entry.ip() == hostAddress) {
                return true;
            }
        }
    }

    return false;
}

void SpeedwireInterface::reconfigureMulticastGroup()
{
    qCDebug(dcSma()) << "Reconfigure multicast interfaces";
    if (m_multicast->joinMulticastGroup(Speedwire::multicastAddress())) {
        qCDebug(dcSma()) << "SpeedwireInterface: Joined successfully multicast group" << Speedwire::multicastAddress().toString();
        m_multicastWarningPrintCount = 0;
    } else {
        // FIXME: It would probably be better to monitor the network interfaces and re-join if necessary
        uint mod = m_multicastWarningPrintCount % 120;

        if (m_multicastWarningPrintCount < 12) {
            qCWarning(dcSma()) << "SpeedwireInterface: Failed to join multicast group" << Speedwire::multicastAddress().toString() << m_multicast->errorString() << "Retrying in 5 seconds...";
        }

        if (m_multicastWarningPrintCount >= 12 && mod == 0) {
            qCWarning(dcSma()) << "SpeedwireInterface: Failed to join multicast group" << Speedwire::multicastAddress().toString() << m_multicast->errorString() << "Retrying in 10 minutes...";
        }

        QTimer::singleShot(5000, this, &SpeedwireInterface::reconfigureMulticastGroup);
        m_multicastWarningPrintCount++;
    }

    //    foreach (const QNetworkInterface &interface, QNetworkInterface::allInterfaces()) {
    //        if(interface.isValid() && !interface.flags().testFlag(QNetworkInterface::IsLoopBack)
    //                && interface.flags().testFlag(QNetworkInterface::CanMulticast)
    //                && interface.flags().testFlag(QNetworkInterface::IsRunning)) {

    //            QList<QNetworkAddressEntry> addressEntries = interface.addressEntries();
    //            for (int i = 0; i < addressEntries.length(); i++) {
    //                if (addressEntries.at(i).ip().protocol() == QAbstractSocket::IPv4Protocol) {

    //                    if (!m_multicast->joinMulticastGroup(Speedwire::multicastAddress(), interface)) {
    //                        qCWarning(dcSma()) << "SpeedwireInterface: Could not join multicast group" << Speedwire::multicastAddress().toString() << "on interface" << interface << m_multicast->errorString();
    //                    } else {
    //                        qCDebug(dcSma()) << "SpeedwireInterface: Joined successfully multicast group" << Speedwire::multicastAddress().toString() << "on interface" << interface ;
    //                    }
    //                }
    //            }
    //        }
    //    }

    //    qCDebug(dcSma()) << "Multicast outgoing interface" << m_multicast->multicastInterface();
}

quint32 SpeedwireInterface::sourceSerialNumber() const
{
    return m_sourceSerialNumber;
}

bool SpeedwireInterface::initialize()
{
    bool success = true;
    if (m_unicast->state() != QUdpSocket::BoundState) {
        m_unicast->close();
        if (!m_unicast->bind(QHostAddress::AnyIPv4, Speedwire::port(), QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint)) {
            qCWarning(dcSma()) << "SpeedwireInterface: Unicast socket could not be bound to port" << Speedwire::port();
            success = false;
        }
    }

    if (m_multicast->state() != QUdpSocket::BoundState) {
        if (!m_multicast->bind(QHostAddress::AnyIPv4, Speedwire::port(), QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint)) {
            qCWarning(dcSma()) << "SpeedwireInterface: Unicast socket could not be bound to port" << Speedwire::port();
            success = false;
        }
    }


    reconfigureMulticastGroup();
    m_available = success;
    return success;
}

void SpeedwireInterface::sendDataUnicast(const QHostAddress &address, const QByteArray &data)
{
    qCDebug(dcSma()) << "SpeedwireInterface: Unicast -->" << address.toString() << Speedwire::port() << data.toHex();

    if (!m_unicast) {
        qCWarning(dcSma()) << "SpeedwireInterface: Failed to send unicast data, the socket is not available";
        return;
    }

    if (m_unicast->writeDatagram(data, address, Speedwire::port()) < 0) {
        qCWarning(dcSma()) << "SpeedwireInterface: Failed to send unicast data to" << address.toString() << m_unicast->errorString();
    }
}

void SpeedwireInterface::sendDataMulticast(const QByteArray &data)
{
    qCDebug(dcSma()) << "SpeedwireInterface: Multicast -->" << Speedwire::multicastAddress().toString() << Speedwire::port() << data.toHex();
    if (!m_multicast) {
        qCWarning(dcSma()) << "SpeedwireInterface: Failed to send multicast data, the socket is not available";
        return;
    }

    if (m_multicast->writeDatagram(data, Speedwire::multicastAddress(), Speedwire::port()) < 0) {
        qCWarning(dcSma()) << "SpeedwireInterface: Failed to send multicast data to" << Speedwire::multicastAddress().toString() << m_multicast->errorString();
    }
}
