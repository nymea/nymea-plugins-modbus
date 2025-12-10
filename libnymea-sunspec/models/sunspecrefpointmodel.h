// SPDX-License-Identifier: LGPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of libnymea-sunspec.
*
* libnymea-sunspec is free software: you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* as published by the Free Software Foundation, either version 3
* of the License, or (at your option) any later version.
*
* libnymea-sunspec is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with libnymea-sunspec. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef SUNSPECREFPOINTMODEL_H
#define SUNSPECREFPOINTMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecRefPointModel : public SunSpecModel
{
    Q_OBJECT
public:

    explicit SunSpecRefPointModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecRefPointModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    /* Global Horizontal Irradiance [W/m2] */
    quint16 ghi() const;
    /* Current measurement at reference point [W/m2] */
    quint16 amps() const;
    /* Voltage  measurement at reference point [W/m2] */
    quint16 voltage() const;
    /* Temperature measurement at reference point [W/m2] */
    quint16 temperature() const;

protected:
    quint16 m_fixedBlockLength = 4;

    void initDataPoints();
    void processBlockData() override;

private:
    quint16 m_ghi = 0;
    quint16 m_amps = 0;
    quint16 m_voltage = 0;
    quint16 m_temperature = 0;


};

QDebug operator<<(QDebug debug, SunSpecRefPointModel *model);

#endif // SUNSPECREFPOINTMODEL_H
