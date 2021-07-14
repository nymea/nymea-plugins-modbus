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

#ifndef SUNSPECVOLTVARMODEL_H
#define SUNSPECVOLTVARMODEL_H

#include <QObject>

#include "sunspecmodel.h"
#include "sunspecmodelrepeatingblock.h"

class SunSpecConnection;
class SunSpecVoltVarModel;

class SunSpecVoltVarModelRepeatingBlock : public SunSpecModelRepeatingBlock
{
    Q_OBJECT
public:

    enum Deptref {
        DeptrefWMax = 1,
        DeptrefVArMax = 2,
        DeptrefVArAval = 3
    };
    Q_ENUM(Deptref)

    enum Readonly {
        ReadonlyReadwrite = 0,
        ReadonlyReadonly = 1
    };
    Q_ENUM(Readonly)

    explicit SunSpecVoltVarModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecVoltVarModel *parent = nullptr);
    ~SunSpecVoltVarModelRepeatingBlock() override; 

    SunSpecVoltVarModel *parentModel() const; 

    QString name() const override;

    quint16 actPt() const;
    QModbusReply *setActPt(quint16 actPt);

    Deptref deptRef() const;
    QModbusReply *setDeptRef(Deptref deptRef);

    float v1() const;
    QModbusReply *setV1(float v1);

    float vAr1() const;
    QModbusReply *setVAr1(float vAr1);

    float v2() const;
    QModbusReply *setV2(float v2);

    float vAr2() const;
    QModbusReply *setVAr2(float vAr2);

    float v3() const;
    QModbusReply *setV3(float v3);

    float vAr3() const;
    QModbusReply *setVAr3(float vAr3);

    float v4() const;
    QModbusReply *setV4(float v4);

    float vAr4() const;
    QModbusReply *setVAr4(float vAr4);

    float v5() const;
    QModbusReply *setV5(float v5);

    float vAr5() const;
    QModbusReply *setVAr5(float vAr5);

    float v6() const;
    QModbusReply *setV6(float v6);

    float vAr6() const;
    QModbusReply *setVAr6(float vAr6);

    float v7() const;
    QModbusReply *setV7(float v7);

    float vAr7() const;
    QModbusReply *setVAr7(float vAr7);

    float v8() const;
    QModbusReply *setV8(float v8);

    float vAr8() const;
    QModbusReply *setVAr8(float vAr8);

    float v9() const;
    QModbusReply *setV9(float v9);

    float vAr9() const;
    QModbusReply *setVAr9(float vAr9);

    float v10() const;
    QModbusReply *setV10(float v10);

    float vAr10() const;
    QModbusReply *setVAr10(float vAr10);

    float v11() const;
    QModbusReply *setV11(float v11);

    float vAr11() const;
    QModbusReply *setVAr11(float vAr11);

    float v12() const;
    QModbusReply *setV12(float v12);

    float vAr12() const;
    QModbusReply *setVAr12(float vAr12);

    float v13() const;
    QModbusReply *setV13(float v13);

    float vAr13() const;
    QModbusReply *setVAr13(float vAr13);

    float v14() const;
    QModbusReply *setV14(float v14);

    float vAr14() const;
    QModbusReply *setVAr14(float vAr14);

    float v15() const;
    QModbusReply *setV15(float v15);

    float vAr15() const;
    QModbusReply *setVAr15(float vAr15);

    float v16() const;
    QModbusReply *setV16(float v16);

    float vAr16() const;
    QModbusReply *setVAr16(float vAr16);

    float v17() const;
    QModbusReply *setV17(float v17);

    float vAr17() const;
    QModbusReply *setVAr17(float vAr17);

    float v18() const;
    QModbusReply *setV18(float v18);

    float vAr18() const;
    QModbusReply *setVAr18(float vAr18);

    float v19() const;
    QModbusReply *setV19(float v19);

    float vAr19() const;
    QModbusReply *setVAr19(float vAr19);

    float v20() const;
    QModbusReply *setV20(float v20);

    float vAr20() const;
    QModbusReply *setVAr20(float vAr20);

    QString crvNam() const;
    QModbusReply *setCrvNam(const QString &crvNam);

    quint16 rmpTms() const;
    QModbusReply *setRmpTms(quint16 rmpTms);

    float rmpDecTmm() const;
    QModbusReply *setRmpDecTmm(float rmpDecTmm);

    float rmpIncTmm() const;
    QModbusReply *setRmpIncTmm(float rmpIncTmm);

    Readonly readOnly() const;

    void processBlockData(const QVector<quint16> blockData) override;

protected:
    void initDataPoints() override;

private:
    SunSpecVoltVarModel *m_parentModel = nullptr; 

    quint16 m_actPt = 0;
    Deptref m_deptRef;
    float m_v1 = 0;
    float m_vAr1 = 0;
    float m_v2 = 0;
    float m_vAr2 = 0;
    float m_v3 = 0;
    float m_vAr3 = 0;
    float m_v4 = 0;
    float m_vAr4 = 0;
    float m_v5 = 0;
    float m_vAr5 = 0;
    float m_v6 = 0;
    float m_vAr6 = 0;
    float m_v7 = 0;
    float m_vAr7 = 0;
    float m_v8 = 0;
    float m_vAr8 = 0;
    float m_v9 = 0;
    float m_vAr9 = 0;
    float m_v10 = 0;
    float m_vAr10 = 0;
    float m_v11 = 0;
    float m_vAr11 = 0;
    float m_v12 = 0;
    float m_vAr12 = 0;
    float m_v13 = 0;
    float m_vAr13 = 0;
    float m_v14 = 0;
    float m_vAr14 = 0;
    float m_v15 = 0;
    float m_vAr15 = 0;
    float m_v16 = 0;
    float m_vAr16 = 0;
    float m_v17 = 0;
    float m_vAr17 = 0;
    float m_v18 = 0;
    float m_vAr18 = 0;
    float m_v19 = 0;
    float m_vAr19 = 0;
    float m_v20 = 0;
    float m_vAr20 = 0;
    QString m_crvNam;
    quint16 m_rmpTms = 0;
    float m_rmpDecTmm = 0;
    float m_rmpIncTmm = 0;
    Readonly m_readOnly;

};



class SunSpecVoltVarModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Modena {
        ModenaEnabled = 0x1
    };
    Q_DECLARE_FLAGS(ModenaFlags, Modena)
    Q_FLAG(Modena)

    explicit SunSpecVoltVarModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, QObject *parent = nullptr);
    ~SunSpecVoltVarModel() override; 

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
    qint16 rmpIncDec_SF() const;

protected:
    quint16 m_fixedBlockLength = 10;
    quint16 m_repeatingBlockLength = 54;

    void initDataPoints() override;
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
    qint16 m_rmpIncDec_SF = 0;


};

QDebug operator<<(QDebug debug, SunSpecVoltVarModel *model);

#endif // SUNSPECVOLTVARMODEL_H
