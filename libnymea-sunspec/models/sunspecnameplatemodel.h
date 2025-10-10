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

    explicit SunSpecNameplateModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecNameplateModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    /* Type of DER device. Default value is 4 to indicate PV device. */
    Dertyp derTyp() const;
    /* Continuous power output capability of the inverter. [W] */
    float wRtg() const;
    /* Scale factor */
    qint16 wRtgSf() const;
    /* Continuous Volt-Ampere capability of the inverter. [VA] */
    float vaRtg() const;
    /* Scale factor */
    qint16 vaRtgSf() const;
    /* Continuous VAR capability of the inverter in quadrant 1. [var] */
    float vArRtgQ1() const;
    /* Continuous VAR capability of the inverter in quadrant 2. [var] */
    float vArRtgQ2() const;
    /* Continuous VAR capability of the inverter in quadrant 3. [var] */
    float vArRtgQ3() const;
    /* Continuous VAR capability of the inverter in quadrant 4. [var] */
    float vArRtgQ4() const;
    /* Scale factor */
    qint16 vArRtgSf() const;
    /* Maximum RMS AC current level capability of the inverter. [A] */
    float aRtg() const;
    /* Scale factor */
    qint16 aRtgSf() const;
    /* Minimum power factor capability of the inverter in quadrant 1. [cos()] */
    float pfRtgQ1() const;
    /* Minimum power factor capability of the inverter in quadrant 2. [cos()] */
    float pfRtgQ2() const;
    /* Minimum power factor capability of the inverter in quadrant 3. [cos()] */
    float pfRtgQ3() const;
    /* Minimum power factor capability of the inverter in quadrant 4. [cos()] */
    float pfRtgQ4() const;
    /* Scale factor */
    qint16 pfRtgSf() const;
    /* Nominal energy rating of storage device. [Wh] */
    float whRtg() const;
    /* Scale factor */
    qint16 whRtgSf() const;
    /* The usable capacity of the battery.  Maximum charge minus minimum charge from a technology capability perspective (Amp-hour capacity rating). [AH] */
    float ahrRtg() const;
    /* Scale factor for amp-hour rating. */
    qint16 ahrRtgSf() const;
    /* Maximum rate of energy transfer into the storage device. [W] */
    float maxChaRte() const;
    /* Scale factor */
    qint16 maxChaRteSf() const;
    /* Maximum rate of energy transfer out of the storage device. [W] */
    float maxDisChaRte() const;
    /* Scale factor */
    qint16 maxDisChaRteSf() const;
    /* Pad register. */
    quint16 pad() const;

protected:
    quint16 m_fixedBlockLength = 26;

    void initDataPoints();
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


};

QDebug operator<<(QDebug debug, SunSpecNameplateModel *model);

#endif // SUNSPECNAMEPLATEMODEL_H
