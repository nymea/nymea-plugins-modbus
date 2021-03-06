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

#include "neuronextensiondiscovery.h"
#include "extern-plugininfo.h"

NeuronExtensionDiscovery::NeuronExtensionDiscovery(NeuronExtensionBus *neuronExtensionBus, int startAddress, int endAddress) :
    m_neuronExtensionBus(neuronExtensionBus),
    m_startAddress(startAddress),
    m_endAddress(endAddress)
{
}

bool NeuronExtensionDiscovery::startDiscovery()
{
    qCDebug(dcUniPi()) << "NeuronExtensionDiscovery: start Discovery, start Address" << m_startAddress << "end address" << m_endAddress;

    if (m_discoveryOngoing) {
        qCDebug(dcUniPi()) << "NeuronExtensionDiscovery: Discovery is already in progress";
        return false;
    }
    m_sweepingAddress = m_startAddress;
    m_discoveredExtensions.clear();
    m_discoveryOngoing = true;
    getNext(m_startAddress);
    return true;
}

void NeuronExtensionDiscovery::stopDiscovery()
{
    qCDebug(dcUniPi()) << "NeuronExtensionDiscovery: stopping discovery";
    m_sweepingAddress = m_startAddress;
    m_discoveryOngoing = false;
}

void NeuronExtensionDiscovery::getNext(int address)
{
    qCDebug(dcUniPi()) << "NeuronExtensionDiscovery: get next" << address;
    NeuronExtensionRequest request(NeuronExtensionRegisterType::RegisterTypeRegisters, address, 1000, 7, false);
    auto *reply = m_neuronExtensionBus->sendRequest(request);
    if (reply->isFinished()) {
        processResponse(reply);
        reply->deleteLater();
    } else {
        connect(reply, &NeuronExtensionReply::finished, reply, &NeuronExtensionReply::deleteLater);
        connect(reply, &NeuronExtensionReply::finished, this, [this, reply] {
            processResponse(reply);
        });
    }
}

void NeuronExtensionDiscovery::processResponse(NeuronExtensionReply *reply)
{
     qCDebug(dcUniPi()) << "NeuronExtensionDiscovery: process response";

    if (reply->slaveAddress() == m_sweepingAddress) {
        m_sweepingAddress = reply->slaveAddress()+1;
    } else if (reply->slaveAddress() < m_sweepingAddress){
        // A reply returns multiple finish signals depending on the retry
        qCWarning(dcUniPi()) << "NeuronExtensionDiscovery: Got modbus reply from previous request, ignoring";
        return;
    }

    if (reply->error() == NeuronExtensionReply::Error::ErrorNoError) {
        QVector<quint16> result = reply->result();
        if (result.length() == 7) {
            qCDebug(dcUniPi()) << "NeuronExtensionDiscovery: Found Extension";
            qCDebug(dcUniPi()) << "     - Serial port" << m_neuronExtensionBus->serialPort();
            qCDebug(dcUniPi()) << "     - Slave Address" << reply->slaveAddress();
            qCDebug(dcUniPi()) << "     - Hardware Id" << result[4];
            qCDebug(dcUniPi()) << "     - Serial number" << (static_cast<quint32>(result[6])<<16 | result[5]);

            NeuronExtension::ExtensionTypes model = NeuronExtension::ExtensionTypes::Unknown;
            if (result[4] == 1) {
                model = NeuronExtension::ExtensionTypes::xS10;
            } else if (result[4] == 784) {
                model = NeuronExtension::ExtensionTypes::xS30;
            } else if (result[4] == 528) {
                model = NeuronExtension::ExtensionTypes::xS40;
            } else if (result[4] == 5) {
                model = NeuronExtension::ExtensionTypes::xS50;
            } else if (result[4] == 272) {
                model = NeuronExtension::ExtensionTypes::xS11;
            } else if (result[4] == 273) {
                model = NeuronExtension::ExtensionTypes::xS51;
            } else {
                qCWarning(dcUniPi()) << "NeuronExtensionDiscovery: Neuron extension model unknown";
            }

            if (model != NeuronExtension::ExtensionTypes::Unknown) {
                qCDebug(dcUniPi()) << "     - Model" << model;
                m_discoveredExtensions.insert(reply->slaveAddress(), model);
                emit deviceFound(reply->slaveAddress(), model);
            } else {
                qCWarning(dcUniPi()) << "NeuronExtensionDiscovery: Skipping unknown Neuron extension model";
            }
        }
    }
    if (reply->slaveAddress() >= m_endAddress) {
        m_discoveryOngoing = false;
        qCWarning(dcUniPi()) << "NeuronExtensionDiscovery: Discovery finished, found" << m_discoveredExtensions.count() << "devices";
        emit finished(m_discoveredExtensions);
    } else {
        if (m_discoveryOngoing)
            getNext(m_sweepingAddress);
    }
}
