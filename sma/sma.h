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

#ifndef SMA_H
#define SMA_H

#include <QString>
#include <QDebug>
#include <QMetaObject>
#include <QDataStream>

class Sma
{
    Q_GADGET

public:
    enum DeviceClass {
        DeviceClassUnknown = 0x0000,
        DeviceClassAllDevices = 0x1f40,
        DeviceClassSolarInverter = 0x1f41,
        DeviceClassWindTurbine = 0x1f42,
        DeviceClassBatteryInverter = 0x1f47,
        DeviceClassConsumer = 0x1f61,
        DeviceClassSensorSystem = 0x1f80,
        DeviceClassElectricityMeter = 0x1f81,
        DeviceClassCommunicationProduct = 0x1fc0
    };
    Q_ENUM(DeviceClass)

    inline static QString buildSoftwareVersionString(quint32 versionData) {

        // Software version
        QByteArray rawData;
        QDataStream stream(&rawData, QIODevice::ReadWrite);
        stream << versionData;

        quint8 major = static_cast<quint8>(rawData.at(0));
        quint8 minor = static_cast<quint8>(rawData.at(1));
        quint8 build = static_cast<quint8>(rawData.at(2));
        quint8 revision = static_cast<quint8>(rawData.at(3));;

        // Revision types:
        //  0 -> N: No revision
        //  1 -> E: Experimental version
        //  2 -> A: Alpha version
        //  3 -> B: Beta version
        //  4 -> R: Release version
        //  5 -> S: Special version
        QChar revisionCharacter;
        switch (revision) {
        case 0:
            revisionCharacter = 'N';
            break;
        case 1:
            revisionCharacter = 'E';
            break;
        case 2:
            revisionCharacter = 'A';
            break;
        case 3:
            revisionCharacter = 'B';
            break;
        case 4:
            revisionCharacter = 'R';
            break;
        case 5:
            revisionCharacter = 'S';
            break;
        default:
            revisionCharacter = QChar(revision);
        }

        return QString("%1.%2.%3-%4").arg(major).arg(minor).arg(build).arg(revisionCharacter);
    }

    inline static QString getModelName(quint16 modelIdentifier) {
        switch (modelIdentifier) {
        // Modbus
        case 9225: return "SB 5000SE-10";
        case 9226: return "SB 3600SE-10";
        case 9165: return "SB 3600TL-21";
        case 9075: return "SB 4000TL-21";
        case 9076: return "SB 5000TL-21";
        case 9162: return "SB 3500TL-JP-22";
        case 9164: return "SB 4500TL-JP-22";
        case 9198: return "SB 3000TL-US-22";
        case 9199: return "SB 3800TL-US-22";
        case 9200: return "SB 4000TL-US-22";
        case 9201: return "SB 5000TL-US-22";
        case 9274: return "SB 6000TL-US-22";
        case 9275: return "SB 7000TL-US-22";
        case 9293: return "SB 7700TL-US-22";
        case 9222: return "STP 10000TLEE-JP-10";
        case 9194: return "STP 12000TL-US-10";
        case 9195: return "STP 15000TL-US-10";
        case 9196: return "STP 20000TL-US-10";
        case 9197: return "STP 24000TL-US-10";
        case 9310: return "STP 30000TL-US-10";
        case 9271: return "STP 20000TLEE-JP-11";
        case 9272: return "STP 10000TLEE-JP-11";
        case 9354: return "STP 24500TL-JP-30";
        case 9311: return "STP 25000TL-JP-30";
        case 9223: return "SI6.0H-11";
        case 9224: return "SI8.0H-11";

            // Speedwire / Modbus
        case 9015: return "SB 700";
        case 9016: return "SB 700U";
        case 9017: return "SB 1100";
        case 9018: return "SB 1100U";
        case 9019: return "SB 1100LV";
        case 9020: return "SB 1700";
        case 9021: return "SB 1900TLJ";
        case 9022: return "SB 2100TL";
        case 9023: return "SB 2500";
        case 9024: return "SB 2800";
        case 9025: return "SB 2800i";
        case 9026: return "SB 3000";
        case 9027: return "SB 3000US";
        case 9028: return "SB 3300";
        case 9029: return "SB 3300U";
        case 9030: return "SB 3300TL";
        case 9031: return "SB 3300TL HC";
        case 9032: return "SB 3800";
        case 9033: return "SB 3800U";
        case 9034: return "SB 4000US";
        case 9035: return "SB 4200TL";
        case 9036: return "SB 4200TL HC";
        case 9037: return "SB 5000TL";
        case 9038: return "SB 5000TLW";
        case 9039: return "SB 5000TL HC";
        case 9066: return "SB 1200";
        case 9067: return "STP 10000TL-10";
        case 9068: return "STP 12000TL-10";
        case 9069: return "STP 15000TL-10";
        case 9070: return "STP 17000TL-10";
        case 9074: return "SB 3000TL-21";
        case 9084: return "WB 3600TL-20";
        case 9085: return "WB 5000TL-20";
        case 9086: return "SB 3800US-10";
        case 9098: return "STP 5000TL-20";
        case 9099: return "STP 6000TL-20";
        case 9100: return "STP 7000TL-20";
        case 9101: return "STP 8000TL-10";
        case 9102: return "STPcase 9000TL-20";
        case 9103: return "STP 8000TL-20";
        case 9104: return "SB 3000TL-JP-21";
        case 9105: return "SB 3500TL-JP-21";
        case 9106: return "SB 4000TL-JP-21";
        case 9107: return "SB 4500TL-JP-21";
        case 9108: return "SCSMC";
        case 9109: return "SB 1600TL-10";
        case 9131: return "STP 20000TL-10";
        case 9139: return "STP 20000TLHE-10";
        case 9140: return "STP 15000TLHE-10";
        case 9157: return "Sunny Island 2012";
        case 9158: return "Sunny Island 2224";
        case 9159: return "Sunny Island 5048";
        case 9160: return "SB 3600TL-20";
        case 9168: return "SC630HE-11";
        case 9169: return "SC500HE-11";
        case 9170: return "SC400HE-11";
        case 9171: return "WB 3000TL-21";
        case 9172: return "WB 3600TL-21";
        case 9173: return "WB 4000TL-21";
        case 9174: return "WB 5000TL-21";
        case 9175: return "SC 250";
        case 9176: return "SMA Meteo Station";
        case 9177: return "SB 240-10";
        case 9179: return "Multigate-10";
        case 9180: return "Multigate-US-10";
        case 9181: return "STP 20000TLEE-10";
        case 9182: return "STP 15000TLEE-10";
        case 9183: return "SB 2000TLST-21";
        case 9184: return "SB 2500TLST-21";
        case 9185: return "SB 3000TLST-21";
        case 9186: return "WB 2000TLST-21";
        case 9187: return "WB 2500TLST-21";
        case 9188: return "WB 3000TLST-21";
        case 9189: return "WTP 5000TL-20";
        case 9190: return "WTP 6000TL-20";
        case 9191: return "WTP 7000TL-20";
        case 9192: return "WTP 8000TL-20";
        case 9193: return "WTPcase 9000TL-20";
        case 9254: return "Sunny Island 3324";
        case 9255: return "Sunny Island 4.0M";
        case 9256: return "Sunny Island 4248";
        case 9257: return "Sunny Island 4248U";
        case 9258: return "Sunny Island 4500";
        case 9259: return "Sunny Island 4548U";
        case 9260: return "Sunny Island 5.4M";
        case 9261: return "Sunny Island 5048U";
        case 9262: return "Sunny Island 6048U";
        case 9278: return "Sunny Island 3.0M";
        case 9279: return "Sunny Island 4.4M";
        case 9281: return "STP 10000TL-20";
        case 9282: return "STP 11000TL-20";
        case 9283: return "STP 12000TL-20";
        case 9284: return "STP 20000TL-30";
        case 9285: return "STP 25000TL-30";
        case 9301: return "SB1.5-1VL-40";
        case 9302: return "SB2.5-1VL-40";
        case 9303: return "SB2.0-1VL-40";
        case 9304: return "SB5.0-1SP-US-40";
        case 9305: return "SB6.0-1SP-US-40";
        case 9306: return "SB8.0-1SP-US-40";
        case 9307: return "Energy Meter";
        default:
            return "Unknown";
        }
    };
};

#endif // SMA_H
