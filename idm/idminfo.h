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

#ifndef IDMINFO_H
#define IDMINFO_H

#include <QMetaType>
#include <QString>

/** This struct holds the status information that is read from the IDM device
 *  and passed to the nymea framework within this plugin.
 */
struct IdmInfo {
    /** Set to true, if register values can be read,
     *  false in case of communication problems */
    bool    m_connected;

    bool    m_power;

    /** RegisterList::OutsideTemperature */
    double  m_roomTemperature;

    /** RegisterList::ExternalOutsideTemperature */
    double  m_outsideTemperature;

    /** RegisterList::HeatStorageTemperature */
    double  m_waterTemperature;

    /** RegisterList::TargetRoomTemperatureZ1R1 (zone 1, room 1) */
    double  m_targetRoomTemperature;

    /** RegisterList::TargetHotWaterTemperature */
    double  m_targetWaterTemperature;

    /** RegisterList::HumiditySensor */
    double  m_humidity;

    /** RegisterList::CurrentPowerConsumptionHeatPump */
    double  m_powerConsumptionHeatPump;

    /** RegisterList::OperationModeSystem */
    QString m_mode;

    /** True if there is an error code set 
     *  (RegisterList::CurrentFaultNumber != 0) */
    bool    m_error;
};

Q_DECLARE_METATYPE(IdmInfo);

#endif

