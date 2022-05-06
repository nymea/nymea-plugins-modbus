/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2021, nymea GmbH
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

#include "modbushelpers.h"

float ModbusHelpers::convertRegisterToFloat(const quint16 *reg) {

    float result = 0.0;

    if (reg != nullptr) {
        /* low-order byte is sent first, so swap order */
        quint32 tmp = 0;

        tmp |= ((quint32)(reg[1]) << 16) & 0xFFFF0000;
        tmp |= reg[0];

        /* copy value over to float variable without any conversion */
        /* needs to be done with char * to avoid pedantic compiler errors */
        memcpy((char *)&result, (char *)&tmp, sizeof(result));
    }
    return result;
}

QVector<quint16> ModbusHelpers::convertFloatToRegister(float value)
{
    quint32 tmp = 0;
    memcpy((char *)&tmp, (char *)&value, sizeof(value));

    QVector<quint16> reg;
    reg.append((quint16)(tmp));
    reg.append((quint16)((tmp & 0xFFFF0000) >> 16));
    return reg;
}

