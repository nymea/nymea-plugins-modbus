/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea.
* This project including source code and documentation is protected by copyright law, and
* remains the property of nymea GmbH. All rights, including reproduction, publication,
* editing and translation, are reserved. The use of this project is subject to the terms of a
* license agreement to be concluded with nymea GmbH in accordance with the terms
* of use of nymea GmbH, available under https://nymea.io/license
*
* GNU Lesser General Public License Usage
* Alternatively, this project may be redistributed and/or modified under the terms of the GNU
* Lesser General Public License as published by the Free Software Foundation; version 3.
* this project is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License along with this project.
* If not, see <https://www.gnu.org/licenses/>.
*
* For any further details and any questions please contact us under contact@nymea.io
* or see our FAQ/Licensing Information on https://nymea.io/license/faq
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "wallbe.h"
#include "extern-plugininfo.h"

WallBe::WallBe(QHostAddress address, int port, QObject *parent) : QObject(parent)
{
    // TCP connction to target device
    m_device = modbus_new_tcp(QVariant(address.toIPv4Address()).toByteArray(), port);

    if(m_device == nullptr){
        qCWarning(dcWallbe) << "Error Modbus TCP";
        free((void*)(m_device));
        return;
    }

    if(modbus_connect(m_device) == -1){
        qCWarning(dcWallbe) << "Error Connecting Modbus";
        free((void*)(m_device));
        return;
    }

    if(modbus_set_slave(m_device, 180) == -1){
        qCWarning(dcWallbe) << "Error Setting Slave ID";
        free((void*)(m_device));
        return;
    }

    m_macAddress = getMacAddress();
}

/*
// Extract Input Register 104 - DIP  - 16bit integer
if(!(tab_reg[4] && 0x0200)){
    // DIP Switch 10 has to be "ON" to enable remote charge control.
    // DIP Switch 1 = LSB
    qCWarning(dcWallbe) << "DIP switch 10 not on:" << tab_reg[4];
}
*/

WallBe::~WallBe()
{
    if (m_device){
        modbus_close(m_device);
        modbus_free(m_device);
    }
}

bool WallBe::isAvailable()
{
    uint16_t reg;

    if(!m_device)
        return false;

    // Read random Register to check if the device is reachable
    if (modbus_read_input_registers(m_device, 100, 1, &reg) == -1){
        qDebug(dcWallbe) << "Connection Failed:" << modbus_strerror(errno) ;
        return false;
    }
    return true;
}

bool WallBe::connect()
{
    if(!m_device)
        return false;

    // Conenct ot the device
    if (modbus_connect(m_device) == -1) {
        qCDebug(dcWallbe) << "Connection failed: " << modbus_strerror(errno);
        return false;
    }
    return true;
}

QString WallBe::getMacAddress()
{
    QString mac;
    uint16_t reg[3];

    if(!isAvailable()){
        if(!connect()){
            return "";
        }
    }

    int ret = modbus_read_registers(m_device, 301, 3, reg);
    if (ret == -1){
        qDebug(dcWallbe) << "Connection Failed:" << modbus_strerror(errno) ;
        return "";
    }
   // for(){
    mac = (reg[0] && 0x00ff) + (reg[0] >> 8);// + ":" + (reg[1] && 0x00ff) + (reg[1] >> 8) + ":" + (reg[2] && 0x00ff) + (reg[2] >> 8));

    //}
    qDebug(dcWallbe) << "Device Mac Address:" << mac ;
    return mac;
}

int WallBe::getEvStatus()
{
    uint16_t reg;

    if(modbus_read_input_registers(m_device, 100, 1, &reg) == -1)
        return 0;

    return (int)reg;
}

int WallBe::getErrorCode()
{
    uint16_t reg;

    if(modbus_read_input_registers(m_device, 107, 1, &reg) == -1)
        return 0;

    return (int)reg;
}

int WallBe::getChargingCurrent()
{
    uint16_t reg;

    if(modbus_read_input_registers(m_device, 300, 1, &reg) == -1)
        return 0;

    return (int)reg;
}

bool WallBe::getChargingStatus()
{
    uint8_t reg;

    // Read if the charging is enabled
    if(modbus_read_bits(m_device, 400, 1, &reg) == -1)
        return false;

    return (int)reg;
}

int WallBe::getChargingTime()
{
    uint16_t reg[2];

    if(modbus_read_registers(m_device, 102, 2, reg) == -1)
        return 0;

    return (((uint32_t)(reg[2]<<16)|(uint32_t)(reg[3]))/60); //Converts to minutes
}

void WallBe::setChargingCurrent(int current)
{
    if(modbus_write_register(m_device, 300, current) == -1){ //TODO
        qDebug(dcWallbe) << "Could not set Current" ;
        return;
    }
}

void WallBe::setChargingStatus(bool enable)
{
    if(modbus_write_bit(m_device, 400, enable) == -1){ //TODO
        return;
    }
}
