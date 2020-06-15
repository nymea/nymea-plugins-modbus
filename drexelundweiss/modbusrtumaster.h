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

#ifndef MODBUSRTUMASTER_H
#define MODBUSRTUMASTER_H

#include <QObject>
#include <QtSerialBus>
#include <QSerialPort>
#include <QTimer>
#include <QUuid>

class ModbusRTUMaster : public QObject
{
    Q_OBJECT
public:
    explicit ModbusRTUMaster(QString serialPort, int baudrate, QSerialPort::Parity parity, int dataBits, int stopBits, QObject *parent = nullptr);
    ~ModbusRTUMaster();

    bool connectDevice();

    bool readCoil(int slaveAddress, int registerAddress);
    bool readDiscreteInput(int slaveAddress, int registerAddress);
    bool readInputRegister(int slaveAddress, int registerAddress);
    bool readHoldingRegister(int slaveAddress, int registerAddress);

    QUuid writeCoil(int slaveAddress, int registerAddress, bool status);
    QUuid writeHoldingRegister(int slaveAddress, int registerAddress, int data);

    QString serialPort();

private:
    QModbusRtuSerialMaster *m_modbusRtuSerialMaster;
    QTimer *m_reconnectTimer = nullptr;

private slots:
    void onReplyFinished();
    void onReplyErrorOccured(QModbusDevice::Error error);
    void onReconnectTimer();

    void onModbusErrorOccurred(QModbusDevice::Error error);
    void onModbusStateChanged(QModbusDevice::State state);

signals:
    void connectionStateChanged(bool status);
    void receivedCoil(int slaveAddress, int modbusRegister, bool value);
    void receivedDiscreteInput(int slaveAddress, int modbusRegister, bool value);
    void receivedHoldingRegister(int slaveAddress, int modbusRegister, int value);
    void receivedInputRegister(int slaveAddress, int modbusRegister, int value);

    void writeRequestFinished(QUuid requestId, bool success);
};

#endif // MODBUSRTUMASTER_H
