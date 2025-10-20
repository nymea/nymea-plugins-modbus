/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2025, nymea GmbH
* Contact: contact@nymea.io
*
* This fileDescriptor is part of nymea.
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
