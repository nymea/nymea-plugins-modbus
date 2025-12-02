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

#ifndef I2CPORT_H
#define I2CPORT_H

#include <QObject>

class I2CPortPrivate;

class I2CPort : public QObject
{
    Q_OBJECT
public:
    explicit I2CPort(const QString &portName, QObject *parent = nullptr);

    static QStringList availablePorts();

    QList<int> scanRegirsters();

    int deviceDescriptor() const;
    int address() const;
    QString portName() const;
    QString portDeviceName() const;

    bool isOpen() const;
    bool isValid() const;

public slots:
    bool openPort(int i2cAddress = 0);
    void closePort();

private:
    I2CPortPrivate *d_ptr = nullptr;

};

#endif // I2CPORT_H
