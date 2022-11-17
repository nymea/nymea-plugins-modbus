/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2021, nymea GmbH
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
