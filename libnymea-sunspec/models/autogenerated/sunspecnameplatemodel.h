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

#ifndef SUNSPECNAMEPLATEMODEL_H
#define SUNSPECNAMEPLATEMODEL_H

#include <QObject>

#include "sunspec.h"
#include "sunspecmodel.h"

class SunSpecNameplateModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Dertyp {
        DertypPv = 4,
        DertypPvStor = 82
    };
    Q_ENUM(Dertyp)

    explicit SunSpecNameplateModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecNameplateModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    quint16 modelId() const;
    quint16 modelLength() const;
    Dertyp derTyp() const;
    float wRtg() const;
    float vaRtg() const;
    qint16 vArRtgQ1() const;
    qint16 vArRtgQ2() const;
    qint16 vArRtgQ3() const;
    qint16 vArRtgQ4() const;
    float aRtg() const;
    qint16 pfRtgQ1() const;
    qint16 pfRtgQ2() const;
    qint16 pfRtgQ3() const;
    qint16 pfRtgQ4() const;
    float whRtg() const;
    float ahrRtg() const;
    float maxChaRte() const;
    float maxDisChaRte() const;
    quint16 pad() const;

private:
    quint16 m_modelId;
    quint16 m_modelLength;
    Dertyp m_derTyp;
    float m_wRtg;
    qint16 m_wRtgSf;
    float m_vaRtg;
    qint16 m_vaRtgSf;
    qint16 m_vArRtgQ1;
    qint16 m_vArRtgQ2;
    qint16 m_vArRtgQ3;
    qint16 m_vArRtgQ4;
    qint16 m_vArRtgSf;
    float m_aRtg;
    qint16 m_aRtgSf;
    qint16 m_pfRtgQ1;
    qint16 m_pfRtgQ2;
    qint16 m_pfRtgQ3;
    qint16 m_pfRtgQ4;
    qint16 m_pfRtgSf;
    float m_whRtg;
    qint16 m_whRtgSf;
    float m_ahrRtg;
    qint16 m_ahrRtgSf;
    float m_maxChaRte;
    qint16 m_maxChaRteSf;
    float m_maxDisChaRte;
    qint16 m_maxDisChaRteSf;
    quint16 m_pad;

    void initDataPoints();

};

#endif // SUNSPECNAMEPLATEMODEL_H
