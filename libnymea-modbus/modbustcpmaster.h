// SPDX-License-Identifier: LGPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of libnymea-modbus.
*
* libnymea-modbus is free software: you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* as published by the Free Software Foundation, either version 3
* of the License, or (at your option) any later version.
*
* libnymea-modbus is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with libnymea-modbus. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef MODBUSTCPMASTER_H
#define MODBUSTCPMASTER_H

#include <QUuid>
#include <QTimer>
#include <QObject>
#include <QHostAddress>
#include <QtSerialBus>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(dcModbusTcpMaster)

class ModbusTcpMaster : public QObject
{
    Q_OBJECT
public:
    explicit ModbusTcpMaster(const QHostAddress &hostAddress, uint port, QObject *parent = nullptr);
    ~ModbusTcpMaster();

    // If you change the hostaddress, make sure to reconnectDevice afterwards
    QHostAddress hostAddress() const;
    void setHostAddress(const QHostAddress &hostAddress);

    // If you change the port, make sure to reconnectDevice afterwards
    uint port() const;
    void setPort(uint port);

    QString connectionUrl() const;

    bool connected() const;

    int numberOfRetries() const;
    void setNumberOfRetries(int number);

    int timeout() const;
    void setTimeout(int timeout);

    QString errorString() const;
    QModbusDevice::Error error() const;

    QUuid readCoil(uint slaveAddress, uint registerAddress, uint size = 1);
    QUuid readDiscreteInput(uint slaveAddress, uint registerAddress, uint size = 1);
    QUuid readInputRegister(uint slaveAddress, uint registerAddress, uint size = 1);
    QUuid readHoldingRegister(uint slaveAddress, uint registerAddress, uint size = 1);

    QUuid writeCoil(uint slaveAddress, uint registerAddress, bool status);
    QUuid writeCoils(uint slaveAddress, uint registerAddress, const QVector<quint16> &values);

    QUuid writeHoldingRegister(uint slaveAddress, uint registerAddress, quint16 value);
    QUuid writeHoldingRegisters(uint slaveAddress, uint registerAddress, const QVector<quint16> &values);

    // Generic requests
    QModbusReply *sendRawRequest(const QModbusRequest &request, int serverAddress);
    QModbusReply *sendReadRequest(const QModbusDataUnit &read, int serverAddress);
    QModbusReply *sendReadWriteRequest(const QModbusDataUnit &read, const QModbusDataUnit &write, int serverAddress);
    QModbusReply *sendWriteRequest(const QModbusDataUnit &write, int serverAddress);

public slots:
    bool connectDevice();
    void disconnectDevice();
    bool reconnectDevice();

protected:
    QTimer *m_reconnectTimer = nullptr;
    QModbusTcpClient *m_modbusTcpClient = nullptr;

    QHostAddress m_hostAddress;
    uint m_port;
    int m_timeout = 1000;
    int m_numberOfRetries = 3;
    bool m_connected = false;

private slots:
    void onModbusErrorOccurred(QModbusDevice::Error error);
    void onModbusStateChanged(QModbusDevice::State state);

signals:
    void connectionStateChanged(bool status);
    void connectionErrorOccurred(QModbusDevice::Error error);

    void writeRequestExecuted(const QUuid &requestId, bool success);
    void writeRequestError(const QUuid &requestId, const QString &error);

    void readRequestExecuted(const QUuid &requestId, bool success);
    void readRequestError(const QUuid &requestId, const QString &error);

    void receivedCoil(uint slaveAddress, uint modbusRegister, const QVector<quint16> &values);
    void receivedDiscreteInput(uint slaveAddress, uint modbusRegister, const QVector<quint16> &values);
    void receivedHoldingRegister(uint slaveAddress, uint modbusRegister, const QVector<quint16> &values);
    void receivedInputRegister(uint slaveAddress, uint modbusRegister, const QVector<quint16> &values);

};

#endif // MODBUSTCPMASTER_H
