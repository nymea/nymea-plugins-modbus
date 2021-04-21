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

#include "mtechelpers.h"

float MTecHelpers::convertRegisterToFloat(const quint16 reg)
{
    float value = 0.0;

    value = reg/100.0;

    return value;
}

void MTecHelpers::convertFloatToRegister(quint16 &reg, float value)
{
    reg = qRound(value * 100.0);
}

QString MTecHelpers::externalHeatSourceRequestToString(quint16 value)
{
    QString str{};

    switch (value) {
    case 0:
        str = "No request, external heat source must be turned off";
        break;
    case 1:
        str = "External heat source is released and can be switched on";
        break;
    case 2:
        str = "External heat source is required and must be turned on";
        break;
    }

    return str;
}

