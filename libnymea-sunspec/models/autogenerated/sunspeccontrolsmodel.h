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

#ifndef SUNSPECCONTROLSMODEL_H
#define SUNSPECCONTROLSMODEL_H

#include <QObject>

#include "sunspec.h"
#include "sunspecmodel.h"

class SunSpecControlsModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Conn {
        ConnDisconnect = 0,
        ConnConnect = 1
    };
    Q_ENUM(Conn)

    enum WMaxLim_Ena {
        WMaxLim_EnaDisabled = 0,
        WMaxLim_EnaEnabled = 1
    };
    Q_ENUM(WMaxLim_Ena)

    enum OutPFSet_Ena {
        OutPFSet_EnaDisabled = 0,
        OutPFSet_EnaEnabled = 1
    };
    Q_ENUM(OutPFSet_Ena)

    enum VArPct_Mod {
        VArPct_ModNone = 0,
        VArPct_ModWmax = 1,
        VArPct_ModVarmax = 2,
        VArPct_ModVaraval = 3
    };
    Q_ENUM(VArPct_Mod)

    enum VArPct_Ena {
        VArPct_EnaDisabled = 0,
        VArPct_EnaEnabled = 1
    };
    Q_ENUM(VArPct_Ena)

    explicit SunSpecControlsModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecControlsModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    void readModelHeader() override;
    void readBlockData() override;

private:
    void initDataPoints();

};

#endif // SUNSPECCONTROLSMODEL_H
