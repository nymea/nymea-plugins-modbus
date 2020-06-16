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

#ifndef MODBUSTCPMASTER_H
#define MODBUSTCPMASTER_H

#include <QObject>
#include <QHostAddress>
#include <QtSerialBus>
#include <QTimer>
#include <QUuid>

class ModbusTCPMaster : public QObject
{
    Q_OBJECT
public:
    explicit ModbusTCPMaster(const QHostAddress &hostAddress, uint port, QObject *parent = nullptr);
    ~ModbusTCPMaster();

    bool connectDevice();

    QUuid readCoil(uint slaveAddress, uint registerAddress);
    QUuid readDiscreteInput(uint slaveAddress, uint registerAddress);
    QUuid readInputRegister(uint slaveAddress, uint registerAddress);
    QUuid readHoldingRegister(uint slaveAddress, uint registerAddress);

    QUuid writeCoil(uint slaveAddress, uint registerAddress, bool status);
    QUuid writeHoldingRegister(uint slaveAddress, uint registerAddress, const QVector<quint16> &values);

    QHostAddress hostAddress();
    uint port();
    bool setHostAddress(const QHostAddress &hostAddress);
    bool setPort(uint port);


private:
    QTimer *m_reconnectTimer = nullptr;
    QModbusTcpClient *m_modbusTcpClient;

private slots:
    void onReconnectTimer();

    void onModbusErrorOccurred(QModbusDevice::Error error);
    void onModbusStateChanged(QModbusDevice::State state);

signals:
    void connectionStateChanged(bool status);

    void writeRequestExecuted(const QUuid &requestId, bool success);
    void writeRequestError(const QUuid &requestId, const QString &error);

    void receivedCoil(uint slaveAddress, uint modbusRegister, bool value);
    void receivedDiscreteInput(uint slaveAddress, uint modbusRegister, bool value);
    void receivedHoldingRegister(uint slaveAddress, uint modbusRegister, const QVector<quint16> &values);
    void receivedInputRegister(uint slaveAddress, uint modbusRegister, uint value);
};

#endif // MODBUSTCPMASTER_H
