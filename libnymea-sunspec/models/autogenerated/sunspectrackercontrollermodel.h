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

#ifndef SUNSPECTRACKERCONTROLLERMODEL_H
#define SUNSPECTRACKERCONTROLLERMODEL_H

#include <QObject>

#include "sunspec.h"
#include "sunspecmodel.h"

class SunSpecTrackerControllerModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Typ {
        TypUnknown = 0,
        TypFixed = 1,
        TypHorizontal = 2,
        TypTilted = 3,
        TypAzimuth = 4,
        TypDual = 5,
        TypOther = 99
    };
    Q_ENUM(Typ)

    enum GlblCtl {
        GlblCtlAutomatic = 0,
        GlblCtlManual = 1,
        GlblCtlCalibrate = 2
    };
    Q_ENUM(GlblCtl)

    explicit SunSpecTrackerControllerModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecTrackerControllerModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    void readModelHeader() override;
    void readBlockData() override;

private:
    void initDataPoints();

};

#endif // SUNSPECTRACKERCONTROLLERMODEL_H
