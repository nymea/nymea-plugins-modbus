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

#ifndef SUNSPECFREQWATTPARAMMODEL_H
#define SUNSPECFREQWATTPARAMMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecFreqWattParamModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Hysena {
        HysenaEnabled = 0x1
    };
    Q_DECLARE_FLAGS(HysenaFlags, Hysena)
    Q_FLAG(Hysena)

    enum Modena {
        ModenaEnabled = 0x1
    };
    Q_DECLARE_FLAGS(ModenaFlags, Modena)
    Q_FLAG(Modena)

    explicit SunSpecFreqWattParamModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecFreqWattParamModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;


    /* The slope of the reduction in the maximum allowed watts output as a function of frequency. [% PM/Hz] */
    float wGra() const;
    QModbusReply *setWGra(float wGra);

    /* The frequency deviation from nominal frequency (ECPNomHz) at which a snapshot of the instantaneous power output is taken to act as the CAPPED power level (PM) and above which reduction in power output occurs. [Hz] */
    float hzStr() const;
    QModbusReply *setHzStr(float hzStr);

    /* The frequency deviation from nominal frequency (ECPNomHz) at which curtailed power output may return to normal and the cap on the power level value is removed. [Hz] */
    float hzStop() const;
    QModbusReply *setHzStop(float hzStop);

    /* Enable hysteresis */
    HysenaFlags hysEna() const;
    QModbusReply *setHysEna(HysenaFlags hysEna);

    /* Is Parameterized Frequency-Watt control active. */
    ModenaFlags modEna() const;
    QModbusReply *setModEna(ModenaFlags modEna);

    /* The maximum time-based rate of change at which power output returns to normal after having been capped by an over frequency event. [% WMax/min] */
    float hzStopWGra() const;
    QModbusReply *setHzStopWGra(float hzStopWGra);

    /* Scale factor for output gradient. */
    qint16 wGraSf() const;
    /* Scale factor for frequency deviations. */
    qint16 hzStrStopSf() const;
    /* Scale factor for increment and decrement ramps. */
    qint16 rmpIncDecSf() const;
    quint16 pad() const;

protected:
    quint16 m_fixedBlockLength = 10;

    void initDataPoints();
    void processBlockData() override;

private:
    float m_wGra = 0;
    float m_hzStr = 0;
    float m_hzStop = 0;
    HysenaFlags m_hysEna;
    ModenaFlags m_modEna;
    float m_hzStopWGra = 0;
    qint16 m_wGraSf = 0;
    qint16 m_hzStrStopSf = 0;
    qint16 m_rmpIncDecSf = 0;
    quint16 m_pad = 0;


};

QDebug operator<<(QDebug debug, SunSpecFreqWattParamModel *model);

#endif // SUNSPECFREQWATTPARAMMODEL_H
