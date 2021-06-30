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

#ifndef SUNSPECSETTINGSMODEL_H
#define SUNSPECSETTINGSMODEL_H

#include <QObject>

#include "sunspec.h"
#include "sunspecmodel.h"

class SunSpecSettingsModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Varact {
        VaractSwitch = 1,
        VaractMaintain = 2
    };
    Q_ENUM(Varact)

    enum Clctotva {
        ClctotvaVector = 1,
        ClctotvaArithmetic = 2
    };
    Q_ENUM(Clctotva)

    enum Connph {
        ConnphA = 1,
        ConnphB = 2,
        ConnphC = 3
    };
    Q_ENUM(Connph)

    explicit SunSpecSettingsModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecSettingsModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    quint16 modelId() const;
    quint16 modelLength() const;
    float wMax() const;
    float vRef() const;
    qint16 vRefOfs() const;
    float vMax() const;
    float vMin() const;
    float vaMax() const;
    qint16 vArMaxQ1() const;
    qint16 vArMaxQ2() const;
    qint16 vArMaxQ3() const;
    qint16 vArMaxQ4() const;
    float wGra() const;
    qint16 pfMinQ1() const;
    qint16 pfMinQ2() const;
    qint16 pfMinQ3() const;
    qint16 pfMinQ4() const;
    Varact vArAct() const;
    Clctotva clcTotVa() const;
    float maxRmpRte() const;
    float ecpNomHz() const;
    Connph connPh() const;

private:
    quint16 m_modelId;
    quint16 m_modelLength;
    float m_wMax;
    float m_vRef;
    qint16 m_vRefOfs;
    float m_vMax;
    float m_vMin;
    float m_vaMax;
    qint16 m_vArMaxQ1;
    qint16 m_vArMaxQ2;
    qint16 m_vArMaxQ3;
    qint16 m_vArMaxQ4;
    float m_wGra;
    qint16 m_pfMinQ1;
    qint16 m_pfMinQ2;
    qint16 m_pfMinQ3;
    qint16 m_pfMinQ4;
    Varact m_vArAct;
    Clctotva m_clcTotVa;
    float m_maxRmpRte;
    float m_ecpNomHz;
    Connph m_connPh;
    qint16 m_wMaxSf;
    qint16 m_vRefSf;
    qint16 m_vRefOfsSf;
    qint16 m_vMinMaxSf;
    qint16 m_vaMaxSf;
    qint16 m_vArMaxSf;
    qint16 m_wGraSf;
    qint16 m_pfMinSf;
    qint16 m_maxRmpRteSf;
    qint16 m_ecpNomHzSf;

    void initDataPoints();

};

#endif // SUNSPECSETTINGSMODEL_H
