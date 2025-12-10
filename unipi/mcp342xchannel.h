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

#ifndef MCP342X_H
#define MCP342X_H

#include <QThread>
#include <QMutex>

#include <hardware/i2c/i2cdevice.h>

class MCP342XChannel: public I2CDevice
{
    Q_OBJECT
public:

    /* Resolution  |  LSB
     * ---------------------
     * 12 bits     |  1 mV
     * 14 bits     |  250 μV
     * 16 bits     |  62.5 μV
     * 18 bits     |  15.625 μ
     */

    enum Gain {
        Gain_1 = 0,
        Gain_2 = 1,
        Gain_4 = 2,
        Gain_8 = 3
    };

    enum ConfRegisterBits {
        G0 = 0, // Gain Selection
        G1, // Gain Selection
        S0, // Sample Rate
        S1, // Sample Rate
        OC, // Conversion Mode Bit
        C0, // Channel Selection
        C1, // Channel Selection
        RDY // Ready Bit
    };

    enum SampleRateSelectionBit {
        Bits12 = 0,
        Bits14 = 1,
        Bits16 = 2,
        Bits18 = 3
    };

    explicit MCP342XChannel(const QString &portName, int address, int channel, Gain gain, QObject *parent = nullptr);

    QByteArray readData(int fd) override;

private:
    int m_channel = 0;
    Gain m_gain = Gain_1;
};

#endif // MCP342X_H
