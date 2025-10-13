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

#ifndef SUNSPECSETTINGSMODEL_H
#define SUNSPECSETTINGSMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

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

    explicit SunSpecSettingsModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecSettingsModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;


    /* Setting for maximum power output. Default to WRtg. [W] */
    float wMax() const;
    QModbusReply *setWMax(float wMax);

    /* Voltage at the PCC. [V] */
    float vRef() const;
    QModbusReply *setVRef(float vRef);

    /* Offset  from PCC to inverter. [V] */
    float vRefOfs() const;
    QModbusReply *setVRefOfs(float vRefOfs);

    /* Setpoint for maximum voltage. [V] */
    float vMax() const;
    QModbusReply *setVMax(float vMax);

    /* Setpoint for minimum voltage. [V] */
    float vMin() const;
    QModbusReply *setVMin(float vMin);

    /* Setpoint for maximum apparent power. Default to VARtg. [VA] */
    float vaMax() const;
    QModbusReply *setVaMax(float vaMax);

    /* Setting for maximum reactive power in quadrant 1. Default to VArRtgQ1. [var] */
    float vArMaxQ1() const;
    QModbusReply *setVArMaxQ1(float vArMaxQ1);

    /* Setting for maximum reactive power in quadrant 2. Default to VArRtgQ2. [var] */
    float vArMaxQ2() const;
    QModbusReply *setVArMaxQ2(float vArMaxQ2);

    /* Setting for maximum reactive power in quadrant 3. Default to VArRtgQ3. [var] */
    float vArMaxQ3() const;
    QModbusReply *setVArMaxQ3(float vArMaxQ3);

    /* Setting for maximum reactive power in quadrant 4. Default to VArRtgQ4. [var] */
    float vArMaxQ4() const;
    QModbusReply *setVArMaxQ4(float vArMaxQ4);

    /* Default ramp rate of change of active power due to command or internal action. [% WMax/sec] */
    float wGra() const;
    QModbusReply *setWGra(float wGra);

    /* Setpoint for minimum power factor value in quadrant 1. Default to PFRtgQ1. [cos()] */
    float pfMinQ1() const;
    QModbusReply *setPfMinQ1(float pfMinQ1);

    /* Setpoint for minimum power factor value in quadrant 2. Default to PFRtgQ2. [cos()] */
    float pfMinQ2() const;
    QModbusReply *setPfMinQ2(float pfMinQ2);

    /* Setpoint for minimum power factor value in quadrant 3. Default to PFRtgQ3. [cos()] */
    float pfMinQ3() const;
    QModbusReply *setPfMinQ3(float pfMinQ3);

    /* Setpoint for minimum power factor value in quadrant 4. Default to PFRtgQ4. [cos()] */
    float pfMinQ4() const;
    QModbusReply *setPfMinQ4(float pfMinQ4);

    /* VAR action on change between charging and discharging: 1=switch 2=maintain VAR characterization. */
    Varact vArAct() const;
    QModbusReply *setVArAct(Varact vArAct);

    /* Calculation method for total apparent power. 1=vector 2=arithmetic. */
    Clctotva clcTotVa() const;
    QModbusReply *setClcTotVa(Clctotva clcTotVa);

    /* Setpoint for maximum ramp rate as percentage of nominal maximum ramp rate. This setting will limit the rate that watts delivery to the grid can increase or decrease in response to intermittent PV generation. [% WGra] */
    float maxRmpRte() const;
    QModbusReply *setMaxRmpRte(float maxRmpRte);

    /* Setpoint for nominal frequency at the ECP. [Hz] */
    float ecpNomHz() const;
    QModbusReply *setEcpNomHz(float ecpNomHz);

    /* Identity of connected phase for single phase inverters. A=1 B=2 C=3. */
    Connph connPh() const;
    QModbusReply *setConnPh(Connph connPh);

    /* Scale factor for real power. */
    qint16 wMaxSf() const;
    /* Scale factor for voltage at the PCC. */
    qint16 vRefSf() const;
    /* Scale factor for offset voltage. */
    qint16 vRefOfsSf() const;
    /* Scale factor for min/max voltages. */
    qint16 vMinMaxSf() const;
    /* Scale factor for apparent power. */
    qint16 vaMaxSf() const;
    /* Scale factor for reactive power. */
    qint16 vArMaxSf() const;
    /* Scale factor for default ramp rate. */
    qint16 wGraSf() const;
    /* Scale factor for minimum power factor. */
    qint16 pfMinSf() const;
    /* Scale factor for maximum ramp percentage. */
    qint16 maxRmpRteSf() const;
    /* Scale factor for nominal frequency. */
    qint16 ecpNomHzSf() const;

protected:
    quint16 m_fixedBlockLength = 30;

    void initDataPoints();
    void processBlockData() override;

private:
    float m_wMax = 0;
    float m_vRef = 0;
    float m_vRefOfs = 0;
    float m_vMax = 0;
    float m_vMin = 0;
    float m_vaMax = 0;
    float m_vArMaxQ1 = 0;
    float m_vArMaxQ2 = 0;
    float m_vArMaxQ3 = 0;
    float m_vArMaxQ4 = 0;
    float m_wGra = 0;
    float m_pfMinQ1 = 0;
    float m_pfMinQ2 = 0;
    float m_pfMinQ3 = 0;
    float m_pfMinQ4 = 0;
    Varact m_vArAct;
    Clctotva m_clcTotVa;
    float m_maxRmpRte = 0;
    float m_ecpNomHz = 0;
    Connph m_connPh;
    qint16 m_wMaxSf = 0;
    qint16 m_vRefSf = 0;
    qint16 m_vRefOfsSf = 0;
    qint16 m_vMinMaxSf = 0;
    qint16 m_vaMaxSf = 0;
    qint16 m_vArMaxSf = 0;
    qint16 m_wGraSf = 0;
    qint16 m_pfMinSf = 0;
    qint16 m_maxRmpRteSf = 0;
    qint16 m_ecpNomHzSf = 0;


};

QDebug operator<<(QDebug debug, SunSpecSettingsModel *model);

#endif // SUNSPECSETTINGSMODEL_H
