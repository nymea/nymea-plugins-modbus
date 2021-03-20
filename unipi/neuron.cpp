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

#include "neuron.h"
#include "extern-plugininfo.h"

#include <QFile>
#include <QTextStream>
#include <QStandardPaths>

Neuron::Neuron(NeuronTypes neuronType, QModbusClient *modbusInterface, QObject *parent) :
    NeuronCommon(modbusInterface, 0, parent),
    m_neuronType(neuronType)
{
    qCDebug(dcUniPi()) << "Neuron: Creating Neuron connection" << neuronType;

}

Neuron::~Neuron()
{
    qCDebug(dcUniPi()) << "Neuron: Deleting Neuron connection" << m_neuronType;
}

QString Neuron::type()
{
    switch (m_neuronType) {
    case NeuronTypes::S103:
        return  "S103";
    case NeuronTypes::M103:
        return  "M103";
    case NeuronTypes::M203:
        return  "M203";
    case NeuronTypes::M303:
        return  "M303";
    case NeuronTypes::M403:
        return  "M403";
    case NeuronTypes::M503:
        return  "M503";
    case NeuronTypes::M523:
        return  "M523";
    case NeuronTypes::L203:
        return  "L203";
    case NeuronTypes::L303:
        return  "L303";
    case NeuronTypes::L403:
        return  "L403";
    case NeuronTypes::L503:
        return  "L503";
    case NeuronTypes::L513:
        return  "L513";
    case NeuronTypes::L523:
        return  "L523";
    case NeuronTypes::L533:
        return  "L533";
    }
    return "Unknown";
}


bool Neuron::loadModbusMap()
{
    qCDebug(dcUniPi()) << "Neuron: Load modbus map";

    QStringList fileCoilList;
    QStringList fileRegisterList;

    switch (m_neuronType) {
    case NeuronTypes::S103:
        fileCoilList.append(QString("/Neuron_S103/Neuron_S103-Coils-group-1.csv"));
        break;
    case NeuronTypes::M103:
        fileCoilList.append(QString("/Neuron_M103/Neuron_M103-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_M103/Neuron_M103-Coils-group-2.csv"));
        break;
    case NeuronTypes::M203:
        fileCoilList.append(QString("/Neuron_M203/Neuron_M203-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_M203/Neuron_M203-Coils-group-2.csv"));
        break;
    case NeuronTypes::M303:
        fileCoilList.append(QString("/Neuron_M303/Neuron_M303-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_M303/Neuron_M303-Coils-group-2.csv"));
        break;
    case NeuronTypes::M403:
        fileCoilList.append(QString("/Neuron_M403/Neuron_M403-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_M403/Neuron_M403-Coils-group-2.csv"));
        break;
    case NeuronTypes::M503:
        fileCoilList.append(QString("/Neuron_M503/Neuron_M503-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_M503/Neuron_M503-Coils-group-2.csv"));
        break;
    case NeuronTypes::M523:
        fileCoilList.append(QString("/Neuron_M523/Neuron_M523-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_M523/Neuron_M523-Coils-group-2.csv"));
        break;
    case NeuronTypes::L203:
        fileCoilList.append(QString("/Neuron_L203/Neuron_L203-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_L203/Neuron_L203-Coils-group-2.csv"));
        fileCoilList.append(QString("/Neuron_L203/Neuron_L203-Coils-group-3.csv"));
        break;
    case NeuronTypes::L303:
        fileCoilList.append(QString("/Neuron_L303/Neuron_L303-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_L303/Neuron_L303-Coils-group-2.csv"));
        fileCoilList.append(QString("/Neuron_L303/Neuron_L303-Coils-group-3.csv"));
        break;
    case NeuronTypes::L403:
        fileCoilList.append(QString("/Neuron_L403/Neuron_L403-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_L403/Neuron_L403-Coils-group-2.csv"));
        fileCoilList.append(QString("/Neuron_L403/Neuron_L403-Coils-group-3.csv"));
        break;
    case NeuronTypes::L503:
        fileCoilList.append(QString("/Neuron_L503/Neuron_L503-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_L503/Neuron_L503-Coils-group-2.csv"));
        fileCoilList.append(QString("/Neuron_L503/Neuron_L503-Coils-group-3.csv"));
        break;
    case NeuronTypes::L513:
        fileCoilList.append(QString("/Neuron_L513/Neuron_L513-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_L513/Neuron_L513-Coils-group-2.csv"));
        fileCoilList.append(QString("/Neuron_L513/Neuron_L513-Coils-group-3.csv"));
        break;
    case NeuronTypes::L523:
        fileCoilList.append(QString("/Neuron_L523/Neuron_L523-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_L523/Neuron_L523-Coils-group-2.csv"));
        fileCoilList.append(QString("/Neuron_L523/Neuron_L523-Coils-group-3.csv"));
        break;
    case NeuronTypes::L533:
        fileCoilList.append(QString("/Neuron_L533/Neuron_L533-Coils-group-1.csv"));
        fileCoilList.append(QString("/Neuron_L533/Neuron_L533-Coils-group-2.csv"));
        fileCoilList.append(QString("/Neuron_L533/Neuron_L533-Coils-group-3.csv"));
        break;
    }

    foreach(QString relativeFilePath, fileCoilList) {
        QString absoluteFilePath = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).last() + "/nymea/modbus" + relativeFilePath;
        qDebug(dcUniPi()) << "Neuron: Open CSV File:" << absoluteFilePath;
        QFile *csvFile = new QFile(absoluteFilePath);
        if (!csvFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCWarning(dcUniPi()) << csvFile->errorString() << "Path:" << absoluteFilePath;
            csvFile->deleteLater();
            return false;
        }
        QTextStream *textStream = new QTextStream(csvFile);
        while (!textStream->atEnd()) {
            QString line = textStream->readLine();
            QStringList list = line.split(',');
            if (list.length() <= 4) {
                qCWarning(dcUniPi()) << "Neuron: Currupted CSV file:" << csvFile->fileName();
                csvFile->deleteLater();
                return false;
            }
            if (list[4] == "Basic") {
                QString circuit = list[3].split(" ").last();
                if (list[3].contains("Digital Input", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusDigitalInputRegisters.insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "Neuron: Found input register" << circuit << list[0].toInt();
                } else if (list[3].contains("Digital Output", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusDigitalOutputRegisters.insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "Neuron: Found output register" << circuit << list[0].toInt();
                } else if (list[3].contains("Relay Output", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusDigitalOutputRegisters.insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "Neuron: Found relay register" << circuit << list[0].toInt();
                } else if (list[3].contains("User Programmable LED", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusUserLEDRegisters.insert(circuit, list[0].toInt());
                    qDebug(dcUniPi()) << "Neuron: Found user programmable led" << circuit << list[0].toInt();
                }
            }
        }
        csvFile->close();
        csvFile->deleteLater();
    }

    switch (m_neuronType) {
    case NeuronTypes::S103:
        fileRegisterList.append(QString("/Neuron_S103/Neuron_S103-Registers-group-1.csv"));
        break;
    case NeuronTypes::M103:
        fileRegisterList.append(QString("/Neuron_M103/Neuron_M103-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_M103/Neuron_M103-Registers-group-2.csv"));
        break;
    case NeuronTypes::M203:
        fileRegisterList.append(QString("/Neuron_M203/Neuron_M203-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_M203/Neuron_M203-Registers-group-2.csv"));
        break;
    case NeuronTypes::M303:
        fileRegisterList.append(QString("/Neuron_M303/Neuron_M303-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_M303/Neuron_M303-Registers-group-2.csv"));
        break;
    case NeuronTypes::M403:
        fileRegisterList.append(QString("/Neuron_M403/Neuron_M403-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_M403/Neuron_M403-Registers-group-2.csv"));
        break;
    case NeuronTypes::M503:
        fileRegisterList.append(QString("/Neuron_M503/Neuron_M503-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_M503/Neuron_M503-Registers-group-2.csv"));
        break;
    case NeuronTypes::M523:
        fileRegisterList.append(QString("/Neuron_M523/Neuron_M523-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_M523/Neuron_M523-Registers-group-2.csv"));
        break;
    case NeuronTypes::L203:
        fileRegisterList.append(QString("/Neuron_L203/Neuron_L203-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_L203/Neuron_L203-Registers-group-2.csv"));
        fileRegisterList.append(QString("/Neuron_L203/Neuron_L203-Registers-group-3.csv"));
        break;
    case NeuronTypes::L303:
        fileRegisterList.append(QString("/Neuron_L303/Neuron_L303-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_L303/Neuron_L303-Registers-group-2.csv"));
        fileRegisterList.append(QString("/Neuron_L303/Neuron_L303-Registers-group-3.csv"));
        break;
    case NeuronTypes::L403:
        fileRegisterList.append(QString("/Neuron_L403/Neuron_L403-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_L403/Neuron_L403-Registers-group-2.csv"));
        fileRegisterList.append(QString("/Neuron_L403/Neuron_L403-Registers-group-3.csv"));
        break;
    case NeuronTypes::L503:
        fileRegisterList.append(QString("/Neuron_L503/Neuron_L503-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_L503/Neuron_L503-Registers-group-2.csv"));
        fileRegisterList.append(QString("/Neuron_L503/Neuron_L503-Registers-group-3.csv"));
        break;
    case NeuronTypes::L513:
        fileRegisterList.append(QString("/Neuron_L513/Neuron_L513-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_L513/Neuron_L513-Registers-group-2.csv"));
        fileRegisterList.append(QString("/Neuron_L513/Neuron_L513-Registers-group-3.csv"));
        break;
    case NeuronTypes::L523:
        fileRegisterList.append(QString("/Neuron_L523/Neuron_L523-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_L523/Neuron_L523-Registers-group-2.csv"));
        fileRegisterList.append(QString("/Neuron_L523/Neuron_L523-Registers-group-3.csv"));
        break;
    case NeuronTypes::L533:
        fileRegisterList.append(QString("/Neuron_L533/Neuron_L533-Registers-group-1.csv"));
        fileRegisterList.append(QString("/Neuron_L533/Neuron_L533-Registers-group-2.csv"));
        fileRegisterList.append(QString("/Neuron_L533/Neuron_L533-Registers-group-3.csv"));
        break;
    }
    foreach (QString relativeFilePath, fileRegisterList) {
        QString absoluteFilePath = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).last() + "/nymea/modbus" + relativeFilePath;
        qDebug(dcUniPi()) << "Neuron: Open CSV File:" << absoluteFilePath;
        QFile *csvFile = new QFile(absoluteFilePath);
        if (!csvFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCWarning(dcUniPi()) << "Neuron:" << csvFile->errorString() << "Path:" << absoluteFilePath;
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
            if (list.last() == "Basic") {
                int modbusAddress = list[0].toInt();
                if (list[5].contains("Analog Input Value", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusAnalogInputRegisters.insert(modbusAddress, registerDescriptorFromStringList(list));
                    qDebug(dcUniPi()) << "Neuron: Found analog input register" << modbusAddress;
                } else if (list[5].contains("Analog Output Value", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusAnalogOutputRegisters.insert(modbusAddress, registerDescriptorFromStringList(list));
                    qDebug(dcUniPi()) << "Neuron: Found analog output register" << modbusAddress;
                } else if (list[5].contains("Analog Output Configuration", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusAnalogOutputConfigurationRegisters.insert(list[5].split(" ").last(), modbusAddress);
                    qDebug(dcUniPi()) << "Neuron: Found analog output configuration register" << list[5].split(" ").last() << modbusAddress;
                } else if (list[5].contains("Analog Input Configuration", Qt::CaseSensitivity::CaseInsensitive)) {
                    m_modbusAnalogInputConfigurationRegisters.insert(list[5].split(" ").last(), modbusAddress);
                    qDebug(dcUniPi()) << "Neuron: Found analog input configuration register" << list[5].split(" ").last() << modbusAddress;
                }
            }
        }
        csvFile->close();
        csvFile->deleteLater();
    }
    return true;
}
