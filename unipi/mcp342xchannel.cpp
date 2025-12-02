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

#include "mcp342xchannel.h"
#include "extern-plugininfo.h"

#include <unistd.h>

#define GENERAL_CALL_RESET      0x0006
#define GENERAL_CALL_LATCH      0x0004
#define GENERAL_CALL_CONVERSION 0x0008

#define CONFIGURATION_REGISTER


MCP342XChannel::MCP342XChannel(const QString &portName, int address, int channel, Gain gain, QObject *parent) :
    I2CDevice(portName, address, parent),
    m_channel(channel),
    m_gain(gain)
{

}

QByteArray MCP342XChannel::readData(int fd)
{
    // Wair for previous messurements to be completed
    char readBuf[3] = {0};
    do {
        if (read(fd, readBuf, 3) != 3) {
            qCWarning(dcUniPi()) << "MCP342X: could not read ADC data";
            return QByteArray();
        }
    } while (!(readBuf[2] & (1 << ConfRegisterBits::RDY)));

    // Start a configuration conversation
    unsigned char writeBuf[1] = {0};
    writeBuf[0] |= (m_channel & 0x0003) << ConfRegisterBits::C0;
    writeBuf[0] |= m_gain << ConfRegisterBits::G0;
    //writeBuf[0] |= SampleRateSelectionBit::Bits18 << ConfRegisterBits::S0;
    writeBuf[0] |= 1 << ConfRegisterBits::OC;   // one shot
    writeBuf[0] |= 1 << ConfRegisterBits::RDY;  // start conversoin
    if (write(fd, writeBuf, 1) != 1) {
        qCWarning(dcUniPi()) << "MCP342X: could not write config register";
        return QByteArray();
    }

    // Wait again for the device to finish measurement
    do {
        if (read(fd, readBuf, 3) != 3) {
            qCWarning(dcUniPi()) << "MCP342X: could not read ADC data";
            return QByteArray();
        }
    } while (!(readBuf[2] & (1 << ConfRegisterBits::RDY)));

    if ((readBuf[2] & (0x03 << ConfRegisterBits::C0)) != ((m_channel & 0x0003) << ConfRegisterBits::C0))
        return QByteArray();

    return QByteArray(readBuf, 3);
}

