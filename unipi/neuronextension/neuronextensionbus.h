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

#ifndef NEURONEXTENSIONBUS_H
#define NEURONEXTENSIONBUS_H

#include "modbus/modbus.h"

#include <QObject>
#include <QUuid>
#include <QTimer>
#include <QVector>

class NeuronExtensionRequest;
class NeuronExtensionReply;

class NeuronExtensionBus : public QObject
{
    Q_OBJECT
public:


    explicit NeuronExtensionBus(const QString &port, int baudrate, char parity, QObject *parent = nullptr);
    ~NeuronExtensionBus();
    bool init();

    bool connected();
    QUuid busId();
    QString serialPort() const;

    NeuronExtensionReply *sendRequest(const NeuronExtensionRequest &request);

private:
    QString m_serialPort;
    modbus_t *m_modbus = nullptr;

    QVector<uint8_t> readCoils(int slaveAddress, int startAddres, int count);
    QVector<uint16_t> readRegisters(int slaveAddress, int startAddres, int count);

    bool writeCoils(int slaveAddress, int startAddres, const QVector<uint16_t> &data);
    bool writeRegisters(int slaveAddress, int startAddres, const QVector<uint16_t> &data);

signals:
    void connectedChanged(bool connected);
    void requestExecuted();
};


enum NeuronExtensionRegisterType {
    RegisterTypeCoils,
    RegisterTypeRegisters
};

class NeuronExtensionRequest
{

public:

    explicit NeuronExtensionRequest() {}
    NeuronExtensionRequest(NeuronExtensionRegisterType type, int slaveAddress, int startAddress, int length, bool writeRequest = false) :
        m_slaveAddress(slaveAddress),
        m_startAddress(startAddress),
        m_length(length),
        m_requestType(type),
        m_writeRequest(writeRequest)
    {
    }

    int slaveAddress() const {return m_slaveAddress;}
    int startAddress() const {return m_startAddress;}
    int length() const {return m_length;}
    NeuronExtensionRegisterType type() const {return m_requestType;}
    bool isWriteRequest() const {return m_writeRequest;}
    QVector<quint16> values() const {return m_values;}
    void setValues(const QVector<quint16> values) {
        m_length = values.length();
        m_values = values;
    }

private:
    int m_slaveAddress = -1;
    int m_startAddress = -1;
    int m_length = -1;
    NeuronExtensionRegisterType m_requestType = RegisterTypeCoils;
    bool m_writeRequest = false;
    QVector<quint16> m_values;
};
Q_DECLARE_TYPEINFO(NeuronExtensionRequest, Q_RELOCATABLE_TYPE);


class NeuronExtensionReply : public QObject
{
    Q_OBJECT

public:
    enum Error {
        ErrorNoError,
        ErrorUnkown
    };
    NeuronExtensionReply(QObject *parent = nullptr) : QObject(parent) {}
    NeuronExtensionReply(const NeuronExtensionRequest &request, QObject *parent = nullptr)
        : QObject(parent)
    {
        m_slaveAddress = request.slaveAddress();
        m_startAddress = request.startAddress();
        m_registerType = request.type();
    }

    bool isFinished() const {return m_isFinished;}
    void setFinished()
    {
        m_isFinished = true;
        emit finished();
    }
    int slaveAddress() const {return m_slaveAddress;}
    int startAddress() const {return m_startAddress;}
    NeuronExtensionRegisterType registerType() const {return m_registerType;}
    void setError(Error error) {m_error = error;}
    Error error() {return m_error;}
    void setResult(QVector<uint16_t> result) {m_result = result;}
    void setResult(QVector<uint8_t> result) {
        for (uint8_t value: result) {
            m_result.append(value);
        }
    }
    QVector<uint16_t> result() const {return m_result;}

private:
    int m_slaveAddress = -1;
    int m_startAddress = -1;
    Error m_error = Error::ErrorNoError;
    bool m_isFinished = false;
    NeuronExtensionRegisterType m_registerType = RegisterTypeCoils;
    QVector<uint16_t> m_result;

signals:
    void finished();
};
#endif // NEURONEXTENSIONBUS_H
