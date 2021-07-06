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

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecNameplateModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Dertyp {
        DertypPv = 4,
        DertypPvStor = 82
    };
    Q_ENUM(Dertyp)

    explicit SunSpecNameplateModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent = nullptr);
    ~SunSpecNameplateModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    Dertyp derTyp() const;
    float wRtg() const;
    float vaRtg() const;
    float vArRtgQ1() const;
    float vArRtgQ2() const;
    float vArRtgQ3() const;
    float vArRtgQ4() const;
    float aRtg() const;
    float pfRtgQ1() const;
    float pfRtgQ2() const;
    float pfRtgQ3() const;
    float pfRtgQ4() const;
    float whRtg() const;
    float ahrRtg() const;
    float maxChaRte() const;
    float maxDisChaRte() const;
    quint16 pad() const;

protected:
    void processBlockData() override;

private:
    Dertyp m_derTyp;
    float m_wRtg = 0;
    qint16 m_wRtgSf = 0;
    float m_vaRtg = 0;
    qint16 m_vaRtgSf = 0;
    float m_vArRtgQ1 = 0;
    float m_vArRtgQ2 = 0;
    float m_vArRtgQ3 = 0;
    float m_vArRtgQ4 = 0;
    qint16 m_vArRtgSf = 0;
    float m_aRtg = 0;
    qint16 m_aRtgSf = 0;
    float m_pfRtgQ1 = 0;
    float m_pfRtgQ2 = 0;
    float m_pfRtgQ3 = 0;
    float m_pfRtgQ4 = 0;
    qint16 m_pfRtgSf = 0;
    float m_whRtg = 0;
    qint16 m_whRtgSf = 0;
    float m_ahrRtg = 0;
    qint16 m_ahrRtgSf = 0;
    float m_maxChaRte = 0;
    qint16 m_maxChaRteSf = 0;
    float m_maxDisChaRte = 0;
    qint16 m_maxDisChaRteSf = 0;
    quint16 m_pad = 0;

    void initDataPoints();

};

QDebug operator<<(QDebug debug, SunSpecNameplateModel *model);

#endif // SUNSPECNAMEPLATEMODEL_H
