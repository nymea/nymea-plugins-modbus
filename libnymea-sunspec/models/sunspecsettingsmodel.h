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

    explicit SunSpecSettingsModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, QObject *parent = nullptr);
    ~SunSpecSettingsModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;


    float wMax() const;
    QModbusReply *setWMax(float wMax);

    float vRef() const;
    QModbusReply *setVRef(float vRef);

    float vRefOfs() const;
    QModbusReply *setVRefOfs(float vRefOfs);

    float vMax() const;
    QModbusReply *setVMax(float vMax);

    float vMin() const;
    QModbusReply *setVMin(float vMin);

    float vaMax() const;
    QModbusReply *setVaMax(float vaMax);

    float vArMaxQ1() const;
    QModbusReply *setVArMaxQ1(float vArMaxQ1);

    float vArMaxQ2() const;
    QModbusReply *setVArMaxQ2(float vArMaxQ2);

    float vArMaxQ3() const;
    QModbusReply *setVArMaxQ3(float vArMaxQ3);

    float vArMaxQ4() const;
    QModbusReply *setVArMaxQ4(float vArMaxQ4);

    float wGra() const;
    QModbusReply *setWGra(float wGra);

    float pfMinQ1() const;
    QModbusReply *setPfMinQ1(float pfMinQ1);

    float pfMinQ2() const;
    QModbusReply *setPfMinQ2(float pfMinQ2);

    float pfMinQ3() const;
    QModbusReply *setPfMinQ3(float pfMinQ3);

    float pfMinQ4() const;
    QModbusReply *setPfMinQ4(float pfMinQ4);

    Varact vArAct() const;
    QModbusReply *setVArAct(Varact vArAct);

    Clctotva clcTotVa() const;
    QModbusReply *setClcTotVa(Clctotva clcTotVa);

    float maxRmpRte() const;
    QModbusReply *setMaxRmpRte(float maxRmpRte);

    float ecpNomHz() const;
    QModbusReply *setEcpNomHz(float ecpNomHz);

    Connph connPh() const;
    QModbusReply *setConnPh(Connph connPh);

    qint16 wMaxSf() const;
    qint16 vRefSf() const;
    qint16 vRefOfsSf() const;
    qint16 vMinMaxSf() const;
    qint16 vaMaxSf() const;
    qint16 vArMaxSf() const;
    qint16 wGraSf() const;
    qint16 pfMinSf() const;
    qint16 maxRmpRteSf() const;
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
