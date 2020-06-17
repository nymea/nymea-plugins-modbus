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

#include <QMetaObject>
#include <QMetaEnum>

#include "sunspecthing.h"

#include "extern-plugininfo.h"

SunspecThing::SunspecThing(Thing *thing, QObject *parent) :
    QObject(parent),
    m_thing(thing)
{

}

SunspecThing::~SunspecThing()
{

}

void SunspecThing::readCommonBlock()
{
    if (!m_modbusTcpMaster)
        return;

    int address = 40001 - 1;
    // Read common block
    m_modbusTcpMaster->readHoldingRegister(m_slaveId, address, 70);
}

void SunspecThing::readStorageBlock()
{
    if (!m_modbusTcpMaster)
        return;

    int address;
    if (m_floatingPointRepresentation) {
        address = 40313;
    } else {
        address = 40303;
    }
    /* Startadresse:
        - bei Einstellung „float“:  40313
        - bei Einstellung „int+SF“: 40303
    */
    m_modbusTcpMaster->readHoldingRegister(m_slaveId, address, 26);
}

QByteArray SunspecThing::convertModbusRegister(const uint16_t &modbusData)
{
    uint8_t data[2];
    data[0] = modbusData >> 8;
    data[1] = modbusData & 0xFF;
    //qCDebug(dcFronius()) << (char)data[0] << (char)data[1];
    return QByteArray().append((char)data[0]).append((char)data[1]);
}

QBitArray SunspecThing::convertModbusRegisterBits(const uint16_t &modbusData)
{
    QByteArray data = convertModbusRegister(modbusData);
    QBitArray bits(data.count() * 8);

    // Convert from QByteArray to QBitArray
    for(int i = 0; i < data.count(); ++i) {
        for(int b = 0; b < 8; b++) {
            bits.setBit(i * 8 + b, data.at(i) & (1 << ( 7 - b)));
        }
    }
    return bits;
}

QByteArray SunspecThing::convertModbusRegisters(const QVector<quint16> &modbusData, int offset, int size)
{
    QByteArray bytes;
    for (int i = offset; i < offset + size; i++)
        bytes.append(convertModbusRegister(modbusData[i]));

    return bytes;
}

QString SunspecThing::storageStateToString(const SunspecThing::StorageState &state)
{
    QMetaObject metaObject ;
    metaObject= SunspecThing::staticMetaObject;
    QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("StorageState" ) );
    return QString(metaEnum.valueToKey(state));
}

bool SunspecThing::connectModbus()
{
    if (m_thing->paramValue(sunspecStorageThingModbusHostParamTypeId).toString().isEmpty()) {
        qCWarning(dcFronius()) << "Empty ip address";
        return false;
    }

    m_modbusTcpMaster = new ModbusTCPMaster(QHostAddress(m_thing->paramValue(sunspecStorageThingModbusHostParamTypeId).toString()), 502, this);
    connect(m_modbusTcpMaster, &ModbusTCPMaster::connectionStateChanged, this, &SunspecThing::connectionStateChanged);
    connect(m_modbusTcpMaster, &ModbusTCPMaster::writeRequestExecuted, this, &SunspecThing::requestExecuted);
    connect(m_modbusTcpMaster, &ModbusTCPMaster::receivedHoldingRegister, this, &SunspecThing::onReceivedHoldingRegister);

    m_modbusTcpMaster->setTimeout(3 * 1000); // 3 seconds
    m_modbusTcpMaster->setNumberOfRetries(3);

    qCDebug(dcFronius()) << "Connected successfully" << m_thing->paramValue(sunspecStorageThingModbusHostParamTypeId).toString();

    readCommonBlock();

    return true;
}

QUuid SunspecThing::setGridCharging(const bool &charging)
{
    if (!m_modbusTcpMaster) {
        return "";
    }

    /* Start address:
    - for “float” setting: 40313
    - for “int+SF” setting: 40303 */

    // 40313 + Offset 18 - 1
    // Name ChaGriSet
    /* Setpoint to enable/dis-
    able charging from grid
    PV (charging from grid 0 disabled)
    GRID (charging from 1 grid enabled*/

    int registerAddress;
    if (m_floatingPointRepresentation) {
        registerAddress = 40313 + 18 - 1;
    } else {
        registerAddress = 40303 + 18 - 1;
    }

    quint16 value = charging;
    return m_modbusTcpMaster->writeHoldingRegister(m_slaveId, registerAddress, value);
}

QUuid SunspecThing::setChargingRate(const int &charging)
{
    // 40313 + Offset 14 - 1
    //Register Name InWRte
    /* Defines the maximum charge rate (charge limit). Default is 100% */

    if (!m_modbusTcpMaster) {
        return "";
    }

    int registerAddress;
    if (m_floatingPointRepresentation) {
        registerAddress = 40313 + 14 - 1;
    } else {
        registerAddress = 40303 + 14 - 1;
    }

    int16_t value = charging * 100;
    return m_modbusTcpMaster->writeHoldingRegister(m_slaveId, registerAddress, value);
}

QUuid SunspecThing::setStorageControlMode(const int &charging)
{
    // 40313 + Offset 6 - 1
    // Set charge bit to enable charge limit, set discharge bit to enable discharge limit, set both bits to enable both limits

    if (!m_modbusTcpMaster) {
        return "";
    }

    int registerAddress;
    if (m_floatingPointRepresentation) {
        registerAddress = 40313 + 6 - 1;
    } else {
        registerAddress = 40303 + 6 - 1;
    }

    uint16_t value = charging;
    return m_modbusTcpMaster->writeHoldingRegister(m_slaveId, registerAddress, value);
}

QUuid SunspecThing::setDischargingRate(const int &charging)
{
    // 40313 + Offset 13 - 1
    //Register Name OutWRte
    /*Defines the maximum discharge rate (discharge limit). Default is 100% */

    if (!m_modbusTcpMaster) {
        return "";
    }

    int registerAddress;
    if (m_floatingPointRepresentation) {
        registerAddress = 40313 + 13 - 1;
    } else {
        registerAddress = 40303 + 13 - 1;
    }

    int16_t value = charging * 100;
    return m_modbusTcpMaster->writeHoldingRegister(m_slaveId, registerAddress, value);
}


void SunspecThing::update()
{
    if (!m_modbusTcpMaster) {
        return;
    }

    readCommonBlock();
    readStorageBlock();
}

void SunspecThing::onReceivedHoldingRegister(quint32 slaveAddress, quint32 modbusRegister, const QVector<quint16> &data)
{
    if (!m_thing)
        return;

    switch (modbusRegister) {
    case 40000:  {// Common block, 70 registers long

        qCDebug(dcFronius()) << "Sunspec Identification:" << convertModbusRegisters(data, 0, 2);

        if (convertModbusRegisters(data, 0, 2) != "SunS") {
            qCWarning(dcFronius()) << "Could not find SunS value at" << modbusRegister << "on" << slaveAddress;
            return;
        }

        // ID: 40003
        qCDebug(dcFronius()) << "Id:" << data[2];

        // Manufacturer: 40005 | 16
        qCDebug(dcFronius()) << "Manufacturer:" << QString::fromLatin1(convertModbusRegisters(data, 4, 16));

        // Thing model: 40021 | 16
        qCDebug(dcFronius()) << "Thing model:" << QString::fromLatin1(convertModbusRegisters(data, 20, 16));

        // Data manager version: 40037 | 8
        qCDebug(dcFronius()) << "Data manager version:" << QString::fromLatin1(convertModbusRegisters(data, 36, 8));

        // Inverter Version: 40045 | 8
        qCDebug(dcFronius()) << "Inverter version:" << QString::fromLatin1(convertModbusRegisters(data, 44, 8));

        // Serial Number: 40053 | 16
        qCDebug(dcFronius()) << "Serial number:" << QString::fromLatin1(convertModbusRegisters(data, 52, 16));

        // Modbus thing address : 40069 | 1
        qCDebug(dcFronius()) << "Thing modbus address:" << data[67];

        /*Sunspec Model Type
        zum Auswählen des Datentyps von Datenmodellen für Wechselrichter
        (3d) float
        Darstellung als Gleitkommazahlen
        SunSpec Inverter Model I111, I112 oder I113
        (3e) int+SF
        Darstellung als ganze Zahlen mit Skalierungsfaktoren
        SunSpec Inverter Model I101, I102 oder I103
        WICHTIG! Da die verschiedenen Modelle über unterschiedliche Anzahlen an Re-
        gistern verfügen, ändern sich durch den Wechsel des Datentyps auch die Regis-
        teradressen aller nachfolgenden Modelle.*/
        qCDebug(dcFronius()) << "SunSpec Inverter Modbus Map:" << data[69];
        if (data[69] > 110){
            m_floatingPointRepresentation = true;
        } else {
            m_floatingPointRepresentation = false;
        }
    } break;
    case 40303:
    case 40313: { //Storage Block
        // ID
        qCDebug(dcFronius()) << "Id:" << data[0];

        if (data[0] != 124) {
            qCWarning(dcFronius()) << "Invalid id in register address" << modbusRegister << ":" << data[0];
        }

        // L
        qCDebug(dcFronius()) << "Register count:" << data[1];

        // WchaMax
        qCDebug(dcFronius()) << "Setpoint of maximum charge:" << data[2] << "W";

        // WchaGra
        qCDebug(dcFronius()) << "Setpoint for maximum charge:" << data[3] << "[s]";

        // WdisChaGra
        qCDebug(dcFronius()) << "Setpoint for maximum discharge rate:" << data[4] << "[s]";

        // StorCtl_Mod: Activate hold/discharge/charge storage control mode. Bit0: charge, Bit 1: discharge
        QBitArray storageControlBits = convertModbusRegisterBits(data[5]);

        qCDebug(dcFronius()) << "Charging control mode:" << (storageControlBits.testBit(0) ? "On" : "Off");
        m_thing->setStateValue(sunspecStorageEnableChargingLimitStateTypeId, storageControlBits.testBit(0));

        qCDebug(dcFronius()) << "Discharging control mode:" << (storageControlBits.testBit(1) ? "On" : "Off");
        m_thing->setStateValue(sunspecStorageEnableDischargingLimitStateTypeId, storageControlBits.testBit(1));

        // MinRsvPct
        qCDebug(dcFronius()) << "Setpoint for minimum reserve:" << data[7] << "[%]";

        // ChaState
        qCDebug(dcFronius()) << "Current energy:" << ((double)data[8] / 100.0) << "[%]";
        int percentage = data[8] / 100.00;
        m_thing->setStateValue(sunspecStorageBatteryLevelEventTypeId, percentage);
        m_thing->setStateValue(sunspecStorageBatteryCriticalStateTypeId, (percentage < 10));
        // ChaSt
        //Charge status of storage thing. Enumerated
        qCDebug(dcFronius()) << "Charge state" << storageStateToString(static_cast<StorageState>(data[11]));
        m_thing->setStateValue(sunspecStorageStorageStateStateTypeId, storageStateToString(static_cast<StorageState>(data[11])));

        // OutWRte
        m_thing->setStateValue(sunspecStorageDischargingRateStateTypeId, (int16_t)data[12] / 100);
        qCDebug(dcFronius()) << "Percent of max. discharge rate:" << ((int16_t)data[12] / 100) << "[%]";

        // InWRte
        m_thing->setStateValue(sunspecStorageChargingRateStateTypeId, (int16_t)data[13] / 100);
        qCDebug(dcFronius()) << "Percent of max. charge rate:" << ((int16_t)data[13] / 100) << "[%]";

        // ChaGriSet
        m_thing->setStateValue(sunspecStorageGridChargingStateTypeId, data[18]);
        qCDebug(dcFronius()) << "Charging from grid:" << (data[18] == 0 ? "disabled" : "enabled");
    } break;
    default:
        break;
    };
}
