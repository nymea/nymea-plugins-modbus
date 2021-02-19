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

NeuronExtension::NeuronExtension(ExtensionTypes extensionType, QModbusClient *modbusInterface, int slaveAddress, QObject *parent) :
    NeuronCommon(modbusInterface, slaveAddress, parent),
    m_extensionType(extensionType)
{
    qCDebug(dcUniPi()) << "Neuron: Creating extension" << extensionType;
}

NeuronExtension::~NeuronExtension()
{
    qCDebug(dcUniPi()) << "Neuron: Deleting extension" << m_extensionType;
}



QString NeuronExtension::type()
{
    switch(m_extensionType) {
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
