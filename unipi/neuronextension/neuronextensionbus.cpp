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

#include "neuronextensionbus.h"
#include "extern-plugininfo.h"

#include <QElapsedTimer>

NeuronExtensionBus::NeuronExtensionBus(const QString &port, int baudrate, char parity, QObject *parent) :
    QObject(parent),
    m_serialPort(port)
{

    m_modbus = modbus_new_rtu(port.toLocal8Bit(), baudrate, parity, 8, 1);
    if (m_modbus == nullptr) {
        qDebug() << "Could not create modbus rtu";
    }
}

NeuronExtensionBus::~NeuronExtensionBus()
{
    modbus_close(m_modbus);
    modbus_free(m_modbus);
}

bool NeuronExtensionBus::init()
{
    if (modbus_set_response_timeout(m_modbus, 0, 100000) == -1) {
        qCDebug(dcUniPi()) << "NeuronExtensionBus: Could not set response timeout" << modbus_strerror(errno);
        return false;
    }
    if (modbus_connect(m_modbus) == -1) {
        qCDebug(dcUniPi()) << "NeuronExtensionBus: Could not connect" << modbus_strerror(errno);
        return false;
    }
    if (modbus_set_slave(m_modbus, 1) == -1) {
        qCDebug(dcUniPi()) << "Could not set slave" << modbus_strerror(errno);
        return false;
    }

    uint32_t sec, usec;
    modbus_get_response_timeout(m_modbus, &sec, &usec);
    qCDebug(dcUniPi()) << "NeuronExtensionBus: Timeout" << sec*1000 + usec/1000 << "ms";

    return true;
}

bool NeuronExtensionBus::connected()
{
    return (modbus_connect(m_modbus) == 0);
}

QString NeuronExtensionBus::serialPort() const
{
    return m_serialPort;
}

NeuronExtensionReply *NeuronExtensionBus::sendRequest(const NeuronExtensionRequest &request)
{
    auto *reply = new NeuronExtensionReply(request, this);

    if (request.isWriteRequest()) {
        switch (request.type()) {
        case NeuronExtensionRegisterType::RegisterTypeCoils: {
            if (!writeCoils(request.slaveAddress(), request.startAddress(), request.values())) {
                reply->setError(NeuronExtensionReply::Error::ErrorUnkown);
            }
            reply->setFinished();
            return reply;
        } break;
        case NeuronExtensionRegisterType::RegisterTypeRegisters: {
            if(!writeRegisters(request.slaveAddress(), request.startAddress(), request.values())) {
                reply->setError(NeuronExtensionReply::Error::ErrorUnkown);
            }
            reply->setFinished();
            return reply;
        } break;
        }

    } else {
        switch (request.type()) {
        case NeuronExtensionRegisterType::RegisterTypeCoils: {
            QVector<quint8> values = readCoils(request.slaveAddress(), request.startAddress(), request.length());
            if (values.empty()){
                reply->setError(NeuronExtensionReply::Error::ErrorUnkown);
            } else {
                reply->setResult(values);
            }
            reply->setFinished();
            return reply;
        } break;
        case NeuronExtensionRegisterType::RegisterTypeRegisters: {
            QVector<quint16> values = readRegisters(request.slaveAddress(), request.startAddress(), request.length());
            if (values.empty()){
                reply->setError(NeuronExtensionReply::Error::ErrorUnkown);
            } else {
                reply->setResult(values);
            }
            reply->setFinished();
            return reply;
        } break;
        }
    }
    return nullptr;
}

QVector<uint8_t> NeuronExtensionBus::readCoils(int slaveAddress, int startAddres, int count)
{
    if(modbus_set_slave(m_modbus, slaveAddress) == -1) {
        qCWarning(dcUniPi()) << "NeuronExtensionBus: Could not set slave" << slaveAddress;
        return QVector<uint8_t>();
    }

    if (count == 0 || count > 125) {
        qCDebug(dcUniPi()) << "NeuronExtensionBus: Coils count must be [1, 125]";
    } else {
        QVector<uint8_t> values(count);
        if (modbus_read_bits(m_modbus, startAddres, values.length(), values.data()) == -1) {
            qCDebug(dcUniPi()) << "NeuronExtensionBus: Could not read coils" << modbus_strerror(errno);
        } else {
            return values;
        }
    }
    //return empty vector on failure
    return QVector<uint8_t>();
}

QVector<uint16_t> NeuronExtensionBus::readRegisters(int slaveAddress, int startAddres, int count)
{
    qCDebug(dcUniPi()) << "NeuronExtensionBus: read registers, slave address:" << slaveAddress << "start address" << startAddres << "count" << count;
    if(modbus_set_slave(m_modbus, slaveAddress) == -1) {
        qCWarning(dcUniPi()) << "NeuronExtensionBus: Could not set slave" << slaveAddress;
        return QVector<uint16_t>();
    }

    if (count == 0 || count > 125) {
        qCWarning(dcUniPi()) << "NeuronExtensionBus: Register count must be [1, 125]";
    } else  {
        QVector<uint16_t> values(count);
        if (modbus_read_registers(m_modbus, startAddres, values.length(), values.data()) == -1) {
            qCDebug(dcUniPi()) << "NeuronExtensionBus: Could not read registers" << modbus_strerror(errno);
        } else {
            return values;
        }
    }
    //return empty vector on failure
    return QVector<uint16_t>();
}

bool NeuronExtensionBus::writeCoils(int slaveAddress, int startAddress, const QVector<uint16_t> &data)
{
    qCDebug(dcUniPi()) << "NeuronExtensionBus: Write coils" << slaveAddress << startAddress << data.length();
    if (data.isEmpty() || data.length() > 125) {
        return false;
    }
    if (modbus_set_slave(m_modbus, slaveAddress) == -1) {
        return false;
    }
    QVector<uint8_t> writeData;
    for(uint16_t value: data) {
        writeData.append(static_cast<uint8_t>(value & 0xff));
    }

    if (data.length() == 1) {
        if (modbus_write_bit(m_modbus, startAddress, writeData.first()) == -1){
            qCWarning(dcUniPi()) << "Could not write single coil" << modbus_strerror(errno);
            return false;
        }
    } else {
        if (modbus_write_bits(m_modbus, startAddress,  data.length(), writeData.data()) == -1) {
            qCWarning(dcUniPi()) << "Could not write coil" << modbus_strerror(errno);
            return false;
        }
    }
    return true;
}

bool NeuronExtensionBus::writeRegisters(int slaveAddress, int startAddress, const QVector<uint16_t> &data)
{
    qCDebug(dcUniPi()) << "NeuronExtensionBus: Write registers" << slaveAddress << startAddress << data.length();
    if (modbus_set_slave(m_modbus, slaveAddress) == -1) {
        return false;
    }

    if (data.length() == 1) {
        if (modbus_write_register(m_modbus, startAddress, data.first()) == -1){
            qCWarning(dcUniPi()) << "Could not write single coil" << modbus_strerror(errno);
            return false;
        }
    } else {
        if (modbus_write_registers(m_modbus, startAddress,  data.length(), data.data()) == -1) {
            qCWarning(dcUniPi()) << "Could not write coil" << modbus_strerror(errno);
            return false;
        }
    }
    return true;
}
