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

#ifndef SUNSPECVOLTWATTMODEL_H
#define SUNSPECVOLTWATTMODEL_H

#include <QObject>

#include "sunspecmodel.h"
#include "sunspecmodelrepeatingblock.h"

class SunSpecConnection;
class SunSpecVoltWattModel;

class SunSpecVoltWattModelRepeatingBlock : public SunSpecModelRepeatingBlock
{
    Q_OBJECT
public:

    enum Deptref {
        DeptrefWMax = 1,
        DeptrefWAval = 2
    };
    Q_ENUM(Deptref)

    enum Readonly {
        ReadonlyReadwrite = 0,
        ReadonlyReadonly = 1
    };
    Q_ENUM(Readonly)

    explicit SunSpecVoltWattModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecVoltWattModel *parent = nullptr);
    ~SunSpecVoltWattModelRepeatingBlock() override = default;

    SunSpecVoltWattModel *parentModel() const;

    QString name() const override;

    quint16 actPt() const;
    QModbusReply *setActPt(quint16 actPt);

    Deptref deptRef() const;
    QModbusReply *setDeptRef(Deptref deptRef);

    float v1() const;
    QModbusReply *setV1(float v1);

    float w1() const;
    QModbusReply *setW1(float w1);

    float v2() const;
    QModbusReply *setV2(float v2);

    float w2() const;
    QModbusReply *setW2(float w2);

    float v3() const;
    QModbusReply *setV3(float v3);

    float w3() const;
    QModbusReply *setW3(float w3);

    float v4() const;
    QModbusReply *setV4(float v4);

    float w4() const;
    QModbusReply *setW4(float w4);

    float v5() const;
    QModbusReply *setV5(float v5);

    float w5() const;
    QModbusReply *setW5(float w5);

    float v6() const;
    QModbusReply *setV6(float v6);

    float w6() const;
    QModbusReply *setW6(float w6);

    float v7() const;
    QModbusReply *setV7(float v7);

    float w7() const;
    QModbusReply *setW7(float w7);

    float v8() const;
    QModbusReply *setV8(float v8);

    float w8() const;
    QModbusReply *setW8(float w8);

    float v9() const;
    QModbusReply *setV9(float v9);

    float w9() const;
    QModbusReply *setW9(float w9);

    float v10() const;
    QModbusReply *setV10(float v10);

    float w10() const;
    QModbusReply *setW10(float w10);

    float v11() const;
    QModbusReply *setV11(float v11);

    float w11() const;
    QModbusReply *setW11(float w11);

    float v12() const;
    QModbusReply *setV12(float v12);

    float w12() const;
    QModbusReply *setW12(float w12);

    float v13() const;
    QModbusReply *setV13(float v13);

    float w13() const;
    QModbusReply *setW13(float w13);

    float v14() const;
    QModbusReply *setV14(float v14);

    float w14() const;
    QModbusReply *setW14(float w14);

    float v15() const;
    QModbusReply *setV15(float v15);

    float w15() const;
    QModbusReply *setW15(float w15);

    float v16() const;
    QModbusReply *setV16(float v16);

    float w16() const;
    QModbusReply *setW16(float w16);

    float v17() const;
    QModbusReply *setV17(float v17);

    float w17() const;
    QModbusReply *setW17(float w17);

    float v18() const;
    QModbusReply *setV18(float v18);

    float w18() const;
    QModbusReply *setW18(float w18);

    float v19() const;
    QModbusReply *setV19(float v19);

    float w19() const;
    QModbusReply *setW19(float w19);

    float v20() const;
    QModbusReply *setV20(float v20);

    float w20() const;
    QModbusReply *setW20(float w20);

    QString crvNam() const;
    QModbusReply *setCrvNam(const QString &crvNam);

    quint16 rmpPt1Tms() const;
    QModbusReply *setRmpPt1Tms(quint16 rmpPt1Tms);

    float rmpDecTmm() const;
    QModbusReply *setRmpDecTmm(float rmpDecTmm);

    float rmpIncTmm() const;
    QModbusReply *setRmpIncTmm(float rmpIncTmm);

    Readonly readOnly() const;

    void processBlockData(const QVector<quint16> blockData) override;

protected:
    void initDataPoints() override;

private:
    SunSpecVoltWattModel *m_parentModel = nullptr;

    quint16 m_actPt = 0;
    Deptref m_deptRef;
    float m_v1 = 0;
    float m_w1 = 0;
    float m_v2 = 0;
    float m_w2 = 0;
    float m_v3 = 0;
    float m_w3 = 0;
    float m_v4 = 0;
    float m_w4 = 0;
    float m_v5 = 0;
    float m_w5 = 0;
    float m_v6 = 0;
    float m_w6 = 0;
    float m_v7 = 0;
    float m_w7 = 0;
    float m_v8 = 0;
    float m_w8 = 0;
    float m_v9 = 0;
    float m_w9 = 0;
    float m_v10 = 0;
    float m_w10 = 0;
    float m_v11 = 0;
    float m_w11 = 0;
    float m_v12 = 0;
    float m_w12 = 0;
    float m_v13 = 0;
    float m_w13 = 0;
    float m_v14 = 0;
    float m_w14 = 0;
    float m_v15 = 0;
    float m_w15 = 0;
    float m_v16 = 0;
    float m_w16 = 0;
    float m_v17 = 0;
    float m_w17 = 0;
    float m_v18 = 0;
    float m_w18 = 0;
    float m_v19 = 0;
    float m_w19 = 0;
    float m_v20 = 0;
    float m_w20 = 0;
    QString m_crvNam;
    quint16 m_rmpPt1Tms = 0;
    float m_rmpDecTmm = 0;
    float m_rmpIncTmm = 0;
    Readonly m_readOnly;

};



class SunSpecVoltWattModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Modena {
        ModenaEnabled = 0x1
    };
    Q_DECLARE_FLAGS(ModenaFlags, Modena)
    Q_FLAG(Modena)

    explicit SunSpecVoltWattModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, QObject *parent = nullptr);
    ~SunSpecVoltWattModel() override; 

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
    qint16 vSf() const;
    qint16 deptRefSf() const;
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
    qint16 m_vSf = 0;
    qint16 m_deptRefSf = 0;
    qint16 m_rmpIncDecSf = 0;


};

QDebug operator<<(QDebug debug, SunSpecVoltWattModel *model);

#endif // SUNSPECVOLTWATTMODEL_H
