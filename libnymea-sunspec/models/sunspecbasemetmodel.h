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

#ifndef SUNSPECBASEMETMODEL_H
#define SUNSPECBASEMETMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecBaseMetModel : public SunSpecModel
{
    Q_OBJECT
public:

    explicit SunSpecBaseMetModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecBaseMetModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    /* Ambient Temperature [C] */
    float ambientTemperature() const;
    /* Relative Humidity [Pct] */
    qint16 relativeHumidity() const;
    /* Barometric Pressure [HPa] */
    qint16 barometricPressure() const;
    /* Wind Speed [mps] */
    qint16 windSpeed() const;
    /* Wind Direction [deg] */
    qint16 windDirection() const;
    /* Rainfall [mm] */
    qint16 rainfall() const;
    /* Snow Depth [mm] */
    qint16 snowDepth() const;
    /* Precipitation Type (WMO 4680 SYNOP code reference) */
    qint16 precipitationType() const;
    /* Electric Field [Vm] */
    qint16 electricField() const;
    /* Surface Wetness [kO] */
    qint16 surfaceWetness() const;
    /* Soil Wetness [Pct] */
    qint16 soilWetness() const;

protected:
    quint16 m_fixedBlockLength = 11;

    void initDataPoints();
    void processBlockData() override;

private:
    float m_ambientTemperature = 0;
    qint16 m_relativeHumidity = 0;
    qint16 m_barometricPressure = 0;
    qint16 m_windSpeed = 0;
    qint16 m_windDirection = 0;
    qint16 m_rainfall = 0;
    qint16 m_snowDepth = 0;
    qint16 m_precipitationType = 0;
    qint16 m_electricField = 0;
    qint16 m_surfaceWetness = 0;
    qint16 m_soilWetness = 0;


};

QDebug operator<<(QDebug debug, SunSpecBaseMetModel *model);

#endif // SUNSPECBASEMETMODEL_H
