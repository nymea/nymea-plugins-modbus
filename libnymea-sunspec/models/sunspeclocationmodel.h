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

#ifndef SUNSPECLOCATIONMODEL_H
#define SUNSPECLOCATIONMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecLocationModel : public SunSpecModel
{
    Q_OBJECT
public:

    explicit SunSpecLocationModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecLocationModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    /* UTC 24 hour time stamp to millisecond hhmmss.sssZ format [hhmmss.sssZ] */
    QString tm() const;
    /* UTC Date string YYYYMMDD format [YYYYMMDD] */
    QString date() const;
    /* Location string (40 chars max) [text] */
    QString location() const;
    /* Latitude with seven degrees of precision [Degrees] */
    float lat() const;
    /* Longitude with seven degrees of precision [Degrees] */
    float longitude() const;
    /* Altitude measurement in meters [meters] */
    qint32 altitude() const;

protected:
    quint16 m_fixedBlockLength = 36;

    void initDataPoints();
    void processBlockData() override;

private:
    QString m_tm;
    QString m_date;
    QString m_location;
    float m_lat = 0;
    float m_longitude = 0;
    qint32 m_altitude = 0;


};

QDebug operator<<(QDebug debug, SunSpecLocationModel *model);

#endif // SUNSPECLOCATIONMODEL_H
