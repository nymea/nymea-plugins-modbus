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

#ifndef SUNSPECPRICINGMODEL_H
#define SUNSPECPRICINGMODEL_H

#include <QObject>

#include "sunspec.h"
#include "sunspecmodel.h"

class SunSpecPricingModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Sigtype {
        SigtypeUnknown = 0,
        SigtypeAbsolute = 1,
        SigtypeRelative = 2,
        SigtypeMultiplier = 3,
        SigtypeLevel = 4
    };
    Q_ENUM(Sigtype)

    enum Modena {
        ModenaEnable = 0x1
    };
    Q_DECLARE_FLAGS(ModenaFlags, Modena)
    Q_FLAG(Modena)

    explicit SunSpecPricingModel(SunSpec *connection, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecPricingModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    ModenaFlags modEna() const;
    Sigtype sigType() const;
    float sig() const;
    quint16 winTms() const;
    quint16 rvtTms() const;
    quint16 rmpTms() const;
    quint16 pad() const;

protected:
    void processBlockData() override;

private:
    ModenaFlags m_modEna;
    Sigtype m_sigType;
    float m_sig;
    quint16 m_winTms;
    quint16 m_rvtTms;
    quint16 m_rmpTms;
    qint16 m_sigSf;
    quint16 m_pad;

    void initDataPoints();

};

#endif // SUNSPECPRICINGMODEL_H
