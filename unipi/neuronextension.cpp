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

#include "neuronextension.h"
#include "extern-plugininfo.h"

#include <QFile>
#include <QTextStream>
#include <QModbusDataUnit>
#include <QStandardPaths>

NeuronExtension::NeuronExtension(ExtensionTypes extensionType, ModbusRtuMaster *modbusInterface, int slaveAddress, QObject *parent) :
    NeuronCommon(modbusInterface, slaveAddress, parent),
    m_extensionType(extensionType)
{
    qCDebug(dcUniPi()) << "Neuron Extension: Creating extension" << extensionType;
}

NeuronExtension::~NeuronExtension()
{
    qCDebug(dcUniPi()) << "Neuron Extension: Deleting extension" << m_extensionType;
}

QString NeuronExtension::type()
{
    return stringFromType(m_extensionType);
}

QString NeuronExtension::stringFromType(NeuronExtension::ExtensionTypes extensionType)
{
    switch(extensionType) {
    case ExtensionTypes::xS10:
        return "xS10";
    case ExtensionTypes::xS20:
        return "xS20";
    case ExtensionTypes::xS30:
        return "xS30";
    case ExtensionTypes::xS40:
        return "xS40";
    case ExtensionTypes::xS50:
        return "xS50";
    case ExtensionTypes::xS11:
        return "xS11";
    case ExtensionTypes::xS51:
        return "xS51";
    default:
        return "Unknown";
    }
}

bool NeuronExtension::loadModbusMap()
{
    qCDebug(dcUniPi()) << "Neuron: Load modbus map";

    QStringList fileCoilList;
    QStringList fileRegisterList;

    switch(m_extensionType) {
    case ExtensionTypes::xS10:
        fileCoilList.append(QString("/Neuron_xS10/Neuron_xS10-Coils-group-1.csv"));
        break;
    case ExtensionTypes::xS20:
        fileCoilList.append(QString("/Neuron_xS20/Neuron_xS20-Coils-group-1.csv"));
        break;
    case ExtensionTypes::xS30:
        fileCoilList.append(QString("/Neuron_xS30/Neuron_xS30-Coils-group-1.csv"));
        break;
    case ExtensionTypes::xS40:
        fileCoilList.append(QString("/Neuron_xS40/Neuron_xS40-Coils-group-1.csv"));
        break;
    case ExtensionTypes::xS50:
        fileCoilList.append(QString("/Neuron_xS50/Neuron_xS50-Coils-group-1.csv"));
        break;
    case ExtensionTypes::xS11:
        fileCoilList.append(QString("/Extension_xS11/Extension_xS11-Coils-group-1.csv"));
        break;
    case ExtensionTypes::xS51:
        fileCoilList.append(QString("/Extension_xS51/Extension_xS51-Coils-group-1.csv"));
        break;
    case ExtensionTypes::Unknown:
        return false;
    }

    foreach (QString relativeFilePath, fileCoilList) {
        QString absoluteFilePath = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).last() + "/nymea/modbus" + relativeFilePath;
        qDebug(dcUniPi()) << "Neuron: Open CSV File:" << absoluteFilePath;
        QFile *csvFile = new QFile(absoluteFilePath);
        if (!csvFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCWarning(dcUniPi()) << csvFile->errorString() << absoluteFilePath;
            csvFile->deleteLater();
            return false;
        }
        QTextStream *textStream = new QTextStream(csvFile);
        while (!textStream->atEnd()) {
            QString line = textStream->readLine();
            QStringList list = line.split(',');
            if (list.length() <= 4) {
                qCWarning(dcUniPi()) << "Neuron Extension: currupted CSV file:" << csvFile->fileName();
                csvFile->deleteLater();
                return false;
            }
            if (list[4] == "Basic") {
                QString circuit = list[3].split(" ").last();
                if (list[3].contains("Digital Input", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusDigitalInputRegisters.insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "Neuron Extension: Found input register" << circuit << list[0].toInt();
                } else if (list[3].contains("Digital Output", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusDigitalOutputRegisters.insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "Neuron Extension: Found output register" << circuit << list[0].toInt();
                } else if (list[3].contains("Relay Output", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusDigitalOutputRegisters.insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "Neuron Extension: Found relay register" << circuit << list[0].toInt();
                }  else if (list[3].contains("User Programmable LED", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusUserLEDRegisters.insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "Neuron Extension: Found user programmable led" << circuit << list[0].toInt();
                }
            }
        }
        csvFile->close();
        csvFile->deleteLater();
    }

    switch(m_extensionType) {
    case ExtensionTypes::xS10:
        fileRegisterList.append(QString("/Neuron_xS10/Neuron_xS10-Registers-group-1.csv"));
        break;
    case ExtensionTypes::xS20:
        fileRegisterList.append(QString("/Neuron_xS20/Neuron_xS20-Registers-group-1.csv"));
        break;
    case ExtensionTypes::xS30:
        fileRegisterList.append(QString("/Neuron_xS30/Neuron_xS30-Registers-group-1.csv"));
        break;
    case ExtensionTypes::xS40:
        fileRegisterList.append(QString("/Neuron_xS40/Neuron_xS40-Registers-group-1.csv"));
        break;
    case ExtensionTypes::xS50:
        fileRegisterList.append(QString("/Neuron_xS50/Neuron_xS50-Registers-group-1.csv"));
        break;
    case ExtensionTypes::xS11:
        fileRegisterList.append(QString("/Extension_xS11/Extension_xS11-Registers-group-1.csv"));
        break;
    case ExtensionTypes::xS51:
        fileRegisterList.append(QString("/Extension_xS51/Extension_xS51-Registers-group-1.csv"));
        break;
    case ExtensionTypes::Unknown:
        return false;
    }

    foreach (QString relativeFilePath, fileRegisterList) {
        QString absoluteFilePath = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).last() + "/nymea/modbus" + relativeFilePath;
        qDebug(dcUniPi()) << "Neuron: Open CSV File:" << absoluteFilePath;
        QFile *csvFile = new QFile(absoluteFilePath);
        if (!csvFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCWarning(dcUniPi()) << csvFile->errorString() << absoluteFilePath;
            csvFile->deleteLater();
            return false;
        }
        QTextStream *textStream = new QTextStream(csvFile);
        while (!textStream->atEnd()) {
            QString line = textStream->readLine();
            QStringList list = line.split(',');
            if (list.length() <= 5) {
                qCWarning(dcUniPi()) << "Neuron: Currupted CSV file:" << csvFile->fileName();
                csvFile->deleteLater();
                return false;
            }
            if (list.last() == "Basic" && list[5].split(" ").length() > 3) {
                if (list[5].split(" ").length() <= 3) {
                    qCWarning(dcUniPi()) << "Neuron: Currupted CSV file:" << csvFile->fileName();
                    csvFile->deleteLater();
                    return false;
                }
                int modbusAddress = list[0].toInt();
                if (list[5].contains("Analog Input Value", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusAnalogInputRegisters.insert(modbusAddress, registerDescriptorFromStringList(list));
                    qDebug(dcUniPi()) << "Neuron: Found analog input register" << modbusAddress;
                } else if (list[5].contains("Analog Output Value", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusAnalogOutputRegisters.insert(modbusAddress, registerDescriptorFromStringList(list));
                    qDebug(dcUniPi()) << "Neuron: Found analog output register" << modbusAddress;
                }
            }
        }
        csvFile->close();
        csvFile->deleteLater();
    }
    return true;
}

NeuronExtensionDiscovery::NeuronExtensionDiscovery(ModbusRtuMaster *modbusRtuMaster, int startAddress, int endAddress) :
    m_modbusRtuMaster(modbusRtuMaster),
    m_startAddress(startAddress),
    m_endAddress(endAddress)
{
}

bool NeuronExtensionDiscovery::startDiscovery()
{
    qCDebug(dcUniPi()) << "NeuronExtensionDiscovery: start Discovery, start Address" << m_startAddress << "end address" << m_endAddress;
    if (!m_modbusRtuMaster->connected()) {
        qCDebug(dcUniPi()) << "NeuronExtensionDiscovery: Modbus RTU interface is not connected";
        return false;
    }

    if (m_discoveryOngoing) {
        qCDebug(dcUniPi()) << "NeuronExtensionDiscovery: Discovery is already in progress";
        return false;
    }
    getNext(m_startAddress);

    m_sweepingAddress = 1;
    m_discoveredExtensions.clear();
    m_discoveryOngoing = true;
    return true;
}

void NeuronExtensionDiscovery::stopDiscovery()
{
    qCDebug(dcUniPi()) << "NeuronExtensionDiscovery: stopping discovery";
    m_sweepingAddress = 1;
    m_discoveryOngoing = false;
}

void NeuronExtensionDiscovery::getNext(int address)
{
    ModbusRtuReply *reply = m_modbusRtuMaster->readHoldingRegister(address, 1000, 7);
    connect(reply, &ModbusRtuReply::finished, reply, &ModbusRtuReply::deleteLater);
    connect(reply, &ModbusRtuReply::finished, this, [this, reply] {

        if (reply->slaveAddress() == m_sweepingAddress) {
            m_sweepingAddress = reply->slaveAddress()+1;
        } else if (reply->slaveAddress() < m_sweepingAddress){
            // A reply returns multiple finish signals depending on the retry
            qCWarning(dcUniPi()) << "NeuronExtensionDiscovery: Got modbus reply from previous request, ignoring";
            return;
        }

        QVector<quint16> result = reply->result();
        if (result.length() == 7) {
            qCDebug(dcUniPi()) << "NeuronExtensionDiscovery: Found Extension";
            qCDebug(dcUniPi()) << "     - Serial port" << m_modbusRtuMaster->serialPort();
            qCDebug(dcUniPi()) << "     - Modbus master uuid" << m_modbusRtuMaster->modbusUuid().toString();
            qCDebug(dcUniPi()) << "     - Slave Address" << reply->slaveAddress();
            qCDebug(dcUniPi()) << "     - Hardware Id" << result[4];
            qCDebug(dcUniPi()) << "     - Serial number" << (static_cast<quint32>(result[6])<<16 | result[5]);

            NeuronExtension::ExtensionTypes model;
            if (result[4] == 1) {
                model = NeuronExtension::ExtensionTypes::xS10;
            } else if (result[4] == 2) {
                model = NeuronExtension::ExtensionTypes::xS20;
            } else if (result[4] == 3) {
                model = NeuronExtension::ExtensionTypes::xS30;
            } else if (result[4] == 4) {
                model = NeuronExtension::ExtensionTypes::xS40;
            } else if (result[4] == 5) {
                model = NeuronExtension::ExtensionTypes::xS50;
            } else if (result[4] == 272) {
                model = NeuronExtension::ExtensionTypes::xS11;
            } else if (result[4] == 273) {
                model = NeuronExtension::ExtensionTypes::xS51;
            } else {
                model = NeuronExtension::ExtensionTypes::Unknown;
            }
            qCDebug(dcUniPi()) << "     - Model" << model;
            emit deviceFound(reply->slaveAddress(), model);
            m_discoveredExtensions.insert(reply->slaveAddress(), model);
        }
        if (reply->slaveAddress() >= m_endAddress) {
            m_discoveryOngoing = false;
            qCWarning(dcUniPi()) << "NeuronExtensionDiscovery: Discovery finished, found" << m_discoveredExtensions.count() << "devices";
            emit finished(m_discoveredExtensions);
        } else {
            if (m_discoveryOngoing)
                getNext(m_sweepingAddress);
        }
    });
}
