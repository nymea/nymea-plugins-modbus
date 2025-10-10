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

#ifndef SUNSPECWATTPFMODEL_H
#define SUNSPECWATTPFMODEL_H

#include <QObject>

#include "sunspecmodel.h"
#include "sunspecmodelrepeatingblock.h"

class SunSpecConnection;
class SunSpecWattPfModel;

class SunSpecWattPfModelRepeatingBlock : public SunSpecModelRepeatingBlock
{
    Q_OBJECT
public:

    enum Readonly {
        ReadonlyReadwrite = 0,
        ReadonlyReadonly = 1
    };
    Q_ENUM(Readonly)

    explicit SunSpecWattPfModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecWattPfModel *parent);
    ~SunSpecWattPfModelRepeatingBlock() override = default;

    SunSpecWattPfModel *parentModel() const;

    QString name() const override;

    /* Number of active points in array. */
    quint16 actPt() const;
    QModbusReply *setActPt(quint16 actPt);

    /* Point 1 Watts. [% WMax] */
    float w1() const;
    QModbusReply *setW1(float w1);

    /* Point 1 PF in EEI notation. [cos()] */
    float pf1() const;
    QModbusReply *setPf1(float pf1);

    /* Point 2 Watts. [% WMax] */
    float w2() const;
    QModbusReply *setW2(float w2);

    /* Point 2 PF in EEI notation. [cos()] */
    float pf2() const;
    QModbusReply *setPf2(float pf2);

    /* Point 3 Watts. [% WMax] */
    float w3() const;
    QModbusReply *setW3(float w3);

    /* Point 3 PF in EEI notation. [cos()] */
    float pf3() const;
    QModbusReply *setPf3(float pf3);

    /* Point 4 Watts. [% WMax] */
    float w4() const;
    QModbusReply *setW4(float w4);

    /* Point 4 PF in EEI notation. [cos()] */
    float pf4() const;
    QModbusReply *setPf4(float pf4);

    /* Point 5 Watts. [% WMax] */
    float w5() const;
    QModbusReply *setW5(float w5);

    /* Point 5 PF in EEI notation. [cos()] */
    float pf5() const;
    QModbusReply *setPf5(float pf5);

    /* Point 6 Watts. [% WMax] */
    float w6() const;
    QModbusReply *setW6(float w6);

    /* Point 6 PF in EEI notation. [cos()] */
    float pf6() const;
    QModbusReply *setPf6(float pf6);

    /* Point 7 Watts. [% WMax] */
    float w7() const;
    QModbusReply *setW7(float w7);

    /* Point 7 PF in EEI notation. [cos()] */
    float pf7() const;
    QModbusReply *setPf7(float pf7);

    /* Point 8 Watts. [% WMax] */
    float w8() const;
    QModbusReply *setW8(float w8);

    /* Point 8 PF in EEI notation. [cos()] */
    float pf8() const;
    QModbusReply *setPf8(float pf8);

    /* Point 9 Watts. [% WMax] */
    float w9() const;
    QModbusReply *setW9(float w9);

    /* Point 9 PF in EEI notation. [cos()] */
    float pf9() const;
    QModbusReply *setPf9(float pf9);

    /* Point 10 Watts. [% WMax] */
    float w10() const;
    QModbusReply *setW10(float w10);

    /* Point 10 PF in EEI notation. [cos()] */
    float pf10() const;
    QModbusReply *setPf10(float pf10);

    /* Point 11 Watts. [% WMax] */
    float w11() const;
    QModbusReply *setW11(float w11);

    /* Point 11 PF in EEI notation. [cos()] */
    float pf11() const;
    QModbusReply *setPf11(float pf11);

    /* Point 12 Watts. [% WMax] */
    float w12() const;
    QModbusReply *setW12(float w12);

    /* Point 12 PF in EEI notation. [cos()] */
    float pf12() const;
    QModbusReply *setPf12(float pf12);

    /* Point 13 Watts. [% WMax] */
    float w13() const;
    QModbusReply *setW13(float w13);

    /* Point 13 PF in EEI notation. [cos()] */
    float pf13() const;
    QModbusReply *setPf13(float pf13);

    /* Point 14 Watts. [% WMax] */
    float w14() const;
    QModbusReply *setW14(float w14);

    /* Point 14 PF in EEI notation. [cos()] */
    float pf14() const;
    QModbusReply *setPf14(float pf14);

    /* Point 15 Watts. [% WMax] */
    float w15() const;
    QModbusReply *setW15(float w15);

    /* Point 15 PF in EEI notation. [cos()] */
    float pf15() const;
    QModbusReply *setPf15(float pf15);

    /* Point 16 Watts. [% WMax] */
    float w16() const;
    QModbusReply *setW16(float w16);

    /* Point 16 PF in EEI notation. [cos()] */
    float pf16() const;
    QModbusReply *setPf16(float pf16);

    /* Point 17 Watts. [% WMax] */
    float w17() const;
    QModbusReply *setW17(float w17);

    /* Point 17 PF in EEI notation. [cos()] */
    float pf17() const;
    QModbusReply *setPf17(float pf17);

    /* Point 18 Watts. [% WMax] */
    float w18() const;
    QModbusReply *setW18(float w18);

    /* Point 18 PF in EEI notation. [cos()] */
    float pf18() const;
    QModbusReply *setPf18(float pf18);

    /* Point 19 Watts. [% WMax] */
    float w19() const;
    QModbusReply *setW19(float w19);

    /* Point 19 PF in EEI notation. [cos()] */
    float pf19() const;
    QModbusReply *setPf19(float pf19);

    /* Point 20 Watts. [% WMax] */
    float w20() const;
    QModbusReply *setW20(float w20);

    /* Point 20 PF in EEI notation. [cos()] */
    float pf20() const;
    QModbusReply *setPf20(float pf20);

    /* Optional description for curve. */
    QString crvNam() const;
    QModbusReply *setCrvNam(const QString &crvNam);

    /* The time of the PT1 in seconds (time to accomplish a change of 95%). [Secs] */
    quint16 rmpPt1Tms() const;
    QModbusReply *setRmpPt1Tms(quint16 rmpPt1Tms);

    /* The maximum rate at which the power factor may be reduced in response to changes in the power value. [% PF/min] */
    float rmpDecTmm() const;
    QModbusReply *setRmpDecTmm(float rmpDecTmm);

    /* The maximum rate at which the power factor may be increased in response to changes in the power value. [% PF/min] */
    float rmpIncTmm() const;
    QModbusReply *setRmpIncTmm(float rmpIncTmm);

    /* Enumerated value indicates if curve is read-only or can be modified. */
    Readonly readOnly() const;
    quint16 pad() const;

    void processBlockData() override;

protected:
    void initDataPoints() override;

private:
    SunSpecWattPfModel *m_parentModel = nullptr;

    quint16 m_actPt = 0;
    float m_w1 = 0;
    float m_pf1 = 0;
    float m_w2 = 0;
    float m_pf2 = 0;
    float m_w3 = 0;
    float m_pf3 = 0;
    float m_w4 = 0;
    float m_pf4 = 0;
    float m_w5 = 0;
    float m_pf5 = 0;
    float m_w6 = 0;
    float m_pf6 = 0;
    float m_w7 = 0;
    float m_pf7 = 0;
    float m_w8 = 0;
    float m_pf8 = 0;
    float m_w9 = 0;
    float m_pf9 = 0;
    float m_w10 = 0;
    float m_pf10 = 0;
    float m_w11 = 0;
    float m_pf11 = 0;
    float m_w12 = 0;
    float m_pf12 = 0;
    float m_w13 = 0;
    float m_pf13 = 0;
    float m_w14 = 0;
    float m_pf14 = 0;
    float m_w15 = 0;
    float m_pf15 = 0;
    float m_w16 = 0;
    float m_pf16 = 0;
    float m_w17 = 0;
    float m_pf17 = 0;
    float m_w18 = 0;
    float m_pf18 = 0;
    float m_w19 = 0;
    float m_pf19 = 0;
    float m_w20 = 0;
    float m_pf20 = 0;
    QString m_crvNam;
    quint16 m_rmpPt1Tms = 0;
    float m_rmpDecTmm = 0;
    float m_rmpIncTmm = 0;
    Readonly m_readOnly;
    quint16 m_pad = 0;

};



class SunSpecWattPfModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Modena {
        ModenaEnabled = 0x1
    };
    Q_DECLARE_FLAGS(ModenaFlags, Modena)
    Q_FLAG(Modena)

    explicit SunSpecWattPfModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecWattPfModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;


    /* Index of active curve. 0=no active curve. */
    quint16 actCrv() const;
    QModbusReply *setActCrv(quint16 actCrv);

    /* Is watt-PF mode active. */
    ModenaFlags modEna() const;
    QModbusReply *setModEna(ModenaFlags modEna);

    /* Time window for watt-PF change. [Secs] */
    quint16 winTms() const;
    QModbusReply *setWinTms(quint16 winTms);

    /* Timeout period for watt-PF curve selection. [Secs] */
    quint16 rvrtTms() const;
    QModbusReply *setRvrtTms(quint16 rvrtTms);

    /* Ramp time for moving from current mode to new mode. [Secs] */
    quint16 rmpTms() const;
    QModbusReply *setRmpTms(quint16 rmpTms);

    /* Number of curves supported (recommend 4). */
    quint16 nCrv() const;
    /* Max number of points in array. */
    quint16 nPt() const;
    /* Scale factor for percent WMax. */
    qint16 wSf() const;
    /* Scale factor for PF. */
    qint16 pfSf() const;
    /* Scale factor for increment and decrement ramps. */
    qint16 rmpIncDecSf() const;

protected:
    quint16 m_fixedBlockLength = 10;
    quint16 m_repeatingBlockLength = 54;

    void initDataPoints();
    void processBlockData() override;

private:

    void setupRepeatingBlocks();

    quint16 m_actCrv = 0;
    ModenaFlags m_modEna;
    quint16 m_winTms = 0;
    quint16 m_rvrtTms = 0;
    quint16 m_rmpTms = 0;
    quint16 m_nCrv = 0;
    quint16 m_nPt = 0;
    qint16 m_wSf = 0;
    qint16 m_pfSf = 0;
    qint16 m_rmpIncDecSf = 0;


};

QDebug operator<<(QDebug debug, SunSpecWattPfModel *model);

#endif // SUNSPECWATTPFMODEL_H
