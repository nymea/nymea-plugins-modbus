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

    explicit SunSpecWattPfModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecWattPfModel *parent = nullptr);
    ~SunSpecWattPfModelRepeatingBlock() override = default;

    SunSpecWattPfModel *parentModel() const;

    QString name() const override;

    quint16 actPt() const;
    QModbusReply *setActPt(quint16 actPt);

    float w1() const;
    QModbusReply *setW1(float w1);

    float pf1() const;
    QModbusReply *setPf1(float pf1);

    float w2() const;
    QModbusReply *setW2(float w2);

    float pf2() const;
    QModbusReply *setPf2(float pf2);

    float w3() const;
    QModbusReply *setW3(float w3);

    float pf3() const;
    QModbusReply *setPf3(float pf3);

    float w4() const;
    QModbusReply *setW4(float w4);

    float pf4() const;
    QModbusReply *setPf4(float pf4);

    float w5() const;
    QModbusReply *setW5(float w5);

    float pf5() const;
    QModbusReply *setPf5(float pf5);

    float w6() const;
    QModbusReply *setW6(float w6);

    float pf6() const;
    QModbusReply *setPf6(float pf6);

    float w7() const;
    QModbusReply *setW7(float w7);

    float pf7() const;
    QModbusReply *setPf7(float pf7);

    float w8() const;
    QModbusReply *setW8(float w8);

    float pf8() const;
    QModbusReply *setPf8(float pf8);

    float w9() const;
    QModbusReply *setW9(float w9);

    float pf9() const;
    QModbusReply *setPf9(float pf9);

    float w10() const;
    QModbusReply *setW10(float w10);

    float pf10() const;
    QModbusReply *setPf10(float pf10);

    float w11() const;
    QModbusReply *setW11(float w11);

    float pf11() const;
    QModbusReply *setPf11(float pf11);

    float w12() const;
    QModbusReply *setW12(float w12);

    float pf12() const;
    QModbusReply *setPf12(float pf12);

    float w13() const;
    QModbusReply *setW13(float w13);

    float pf13() const;
    QModbusReply *setPf13(float pf13);

    float w14() const;
    QModbusReply *setW14(float w14);

    float pf14() const;
    QModbusReply *setPf14(float pf14);

    float w15() const;
    QModbusReply *setW15(float w15);

    float pf15() const;
    QModbusReply *setPf15(float pf15);

    float w16() const;
    QModbusReply *setW16(float w16);

    float pf16() const;
    QModbusReply *setPf16(float pf16);

    float w17() const;
    QModbusReply *setW17(float w17);

    float pf17() const;
    QModbusReply *setPf17(float pf17);

    float w18() const;
    QModbusReply *setW18(float w18);

    float pf18() const;
    QModbusReply *setPf18(float pf18);

    float w19() const;
    QModbusReply *setW19(float w19);

    float pf19() const;
    QModbusReply *setPf19(float pf19);

    float w20() const;
    QModbusReply *setW20(float w20);

    float pf20() const;
    QModbusReply *setPf20(float pf20);

    QString crvNam() const;
    QModbusReply *setCrvNam(const QString &crvNam);

    quint16 rmpPt1Tms() const;
    QModbusReply *setRmpPt1Tms(quint16 rmpPt1Tms);

    float rmpDecTmm() const;
    QModbusReply *setRmpDecTmm(float rmpDecTmm);

    float rmpIncTmm() const;
    QModbusReply *setRmpIncTmm(float rmpIncTmm);

    Readonly readOnly() const;
    quint16 pad() const;

    void processBlockData(const QVector<quint16> blockData) override;

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

    explicit SunSpecWattPfModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, QObject *parent = nullptr);
    ~SunSpecWattPfModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;


    quint16 actCrv() const;
    QModbusReply *setActCrv(quint16 actCrv);

    ModenaFlags modEna() const;
    QModbusReply *setModEna(ModenaFlags modEna);

    quint16 winTms() const;
    QModbusReply *setWinTms(quint16 winTms);

    quint16 rvrtTms() const;
    QModbusReply *setRvrtTms(quint16 rvrtTms);

    quint16 rmpTms() const;
    QModbusReply *setRmpTms(quint16 rmpTms);

    quint16 nCrv() const;
    quint16 nPt() const;
    qint16 wSf() const;
    qint16 pfSf() const;
    qint16 rmpIncDecSf() const;

protected:
    quint16 m_fixedBlockLength = 10;
    quint16 m_repeatingBlockLength = 54;

    void initDataPoints();
    void processBlockData() override;

private:
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
