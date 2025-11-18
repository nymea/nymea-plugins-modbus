// SPDX-License-Identifier: GPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of nymea-plugins-modbus.
*
* nymea-plugins-modbus is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* nymea-plugins-modbus is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with nymea-plugins-modbus. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef HUAWEIFUSIONSOLAR_H
#define HUAWEIFUSIONSOLAR_H

#include <QObject>
#include <QQueue>

#include "huaweifusionmodbustcpconnection.h"

class HuaweiFusionSolar : public HuaweiFusionModbusTcpConnection
{
    Q_OBJECT
public:
    explicit HuaweiFusionSolar(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent = nullptr);
    ~HuaweiFusionSolar() = default;

    virtual bool update() override;

    quint16 slaveId() const;

private:
    quint16 m_slaveId;
    QQueue<HuaweiFusionModbusTcpConnection::Registers> m_registersQueue;
    QModbusReply *m_initReply = nullptr;

    int m_currentRegisterRequest = -1;
    void finishRequest();

    bool m_battery1Available = false;
    bool m_battery2Available = false;

    double m_actualInverterPower = 0;

    QString exceptionToString(QModbusPdu::ExceptionCode exception);

private slots:
    void readNextRegister();
    bool valuesAreVaild(const QVector<quint16> &values, int readSize);

};

#endif // HUAWEIFUSIONSOLAR_H
