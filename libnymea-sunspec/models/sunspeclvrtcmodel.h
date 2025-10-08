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

#ifndef SUNSPECLVRTCMODEL_H
#define SUNSPECLVRTCMODEL_H

#include <QObject>

#include "sunspecmodel.h"
#include "sunspecmodelrepeatingblock.h"

class SunSpecConnection;
class SunSpecLvrtcModel;

class SunSpecLvrtcModelRepeatingBlock : public SunSpecModelRepeatingBlock
{
    Q_OBJECT
public:

    enum Readonly {
        ReadonlyReadwrite = 0,
        ReadonlyReadonly = 1
    };
    Q_ENUM(Readonly)

    explicit SunSpecLvrtcModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecLvrtcModel *parent);
    ~SunSpecLvrtcModelRepeatingBlock() override = default;

    SunSpecLvrtcModel *parentModel() const;

    QString name() const override;

    /* Number of active points in array. */
    quint16 actPt() const;
    QModbusReply *setActPt(quint16 actPt);

    /* Point 1 must remain connected duration. [Secs] */
    float tms1() const;
    QModbusReply *setTms1(float tms1);

    /* Point 1 must remain connected voltage. [% VRef] */
    float v1() const;
    QModbusReply *setV1(float v1);

    /* Point 2 must remain connected duration. [Secs] */
    float tms2() const;
    QModbusReply *setTms2(float tms2);

    /* Point 2 must remain connected voltage. [% VRef] */
    float v2() const;
    QModbusReply *setV2(float v2);

    /* Point 3 must remain connected duration. [Secs] */
    float tms3() const;
    QModbusReply *setTms3(float tms3);

    /* Point 3 must remain connected voltage. [% VRef] */
    float v3() const;
    QModbusReply *setV3(float v3);

    /* Point 4 must remain connected duration. [Secs] */
    float tms4() const;
    QModbusReply *setTms4(float tms4);

    /* Point 4 must remain connected voltage. [% VRef] */
    float v4() const;
    QModbusReply *setV4(float v4);

    /* Point 5 must remain connected duration. [Secs] */
    float tms5() const;
    QModbusReply *setTms5(float tms5);

    /* Point 5 must remain connected voltage. [% VRef] */
    float v5() const;
    QModbusReply *setV5(float v5);

    /* Point 6 must remain connected duration. [Secs] */
    float tms6() const;
    QModbusReply *setTms6(float tms6);

    /* Point 6 must remain connected voltage. [% VRef] */
    float v6() const;
    QModbusReply *setV6(float v6);

    /* Point 7 must remain connected duration. [Secs] */
    float tms7() const;
    QModbusReply *setTms7(float tms7);

    /* Point 7 must remain connected voltage. [% VRef] */
    float v7() const;
    QModbusReply *setV7(float v7);

    /* Point 8 must remain connected duration. [Secs] */
    float tms8() const;
    QModbusReply *setTms8(float tms8);

    /* Point 8 must remain connected voltage. [% VRef] */
    float v8() const;
    QModbusReply *setV8(float v8);

    /* Point 9 must remain connected duration. [Secs] */
    float tms9() const;
    QModbusReply *setTms9(float tms9);

    /* Point 9 must remain connected voltage. [% VRef] */
    float v9() const;
    QModbusReply *setV9(float v9);

    /* Point 10 must remain connected duration. [Secs] */
    float tms10() const;
    QModbusReply *setTms10(float tms10);

    /* Point 10 must remain connected voltage. [% VRef] */
    float v10() const;
    QModbusReply *setV10(float v10);

    /* Point 11 must remain connected duration. [Secs] */
    float tms11() const;
    QModbusReply *setTms11(float tms11);

    /* Point 11 must remain connected voltage. [% VRef] */
    float v11() const;
    QModbusReply *setV11(float v11);

    /* Point 12 must remain connected duration. [Secs] */
    float tms12() const;
    QModbusReply *setTms12(float tms12);

    /* Point 12 must remain connected voltage. [% VRef] */
    float v12() const;
    QModbusReply *setV12(float v12);

    /* Point 13 must remain connected duration. [Secs] */
    float tms13() const;
    QModbusReply *setTms13(float tms13);

    /* Point 13 must remain connected voltage. [% VRef] */
    float v13() const;
    QModbusReply *setV13(float v13);

    /* Point 14 must remain connected duration. [Secs] */
    float tms14() const;
    QModbusReply *setTms14(float tms14);

    /* Point 14 must remain connected voltage. [% VRef] */
    float v14() const;
    QModbusReply *setV14(float v14);

    /* Point 15 must remain connected duration. [Secs] */
    float tms15() const;
    QModbusReply *setTms15(float tms15);

    /* Point 15 must remain connected voltage. [% VRef] */
    float v15() const;
    QModbusReply *setV15(float v15);

    /* Point 16 must remain connected duration. [Secs] */
    float tms16() const;
    QModbusReply *setTms16(float tms16);

    /* Point 16 must remain connected voltage. [% VRef] */
    float v16() const;
    QModbusReply *setV16(float v16);

    /* Point 17 must remain connected duration. [Secs] */
    float tms17() const;
    QModbusReply *setTms17(float tms17);

    /* Point 17 must remain connected voltage. [% VRef] */
    float v17() const;
    QModbusReply *setV17(float v17);

    /* Point 18 must remain connected duration. [Secs] */
    float tms18() const;
    QModbusReply *setTms18(float tms18);

    /* Point 18 must remain connected voltage. [% VRef] */
    float v18() const;
    QModbusReply *setV18(float v18);

    /* Point 19 must remain connected duration. [Secs] */
    float tms19() const;
    QModbusReply *setTms19(float tms19);

    /* Point 19 must remain connected voltage. [% VRef] */
    float v19() const;
    QModbusReply *setV19(float v19);

    /* Point 20 must remain connected duration. [Secs] */
    float tms20() const;
    QModbusReply *setTms20(float tms20);

    /* Point 20 must remain connected voltage. [% VRef] */
    float v20() const;
    QModbusReply *setV20(float v20);

    /* Optional description for curve. */
    QString crvNam() const;
    QModbusReply *setCrvNam(const QString &crvNam);

    /* Enumerated value indicates if curve is read-only or can be modified. */
    Readonly readOnly() const;

    void processBlockData() override;

protected:
    void initDataPoints() override;

private:
    SunSpecLvrtcModel *m_parentModel = nullptr;

    quint16 m_actPt = 0;
    float m_tms1 = 0;
    float m_v1 = 0;
    float m_tms2 = 0;
    float m_v2 = 0;
    float m_tms3 = 0;
    float m_v3 = 0;
    float m_tms4 = 0;
    float m_v4 = 0;
    float m_tms5 = 0;
    float m_v5 = 0;
    float m_tms6 = 0;
    float m_v6 = 0;
    float m_tms7 = 0;
    float m_v7 = 0;
    float m_tms8 = 0;
    float m_v8 = 0;
    float m_tms9 = 0;
    float m_v9 = 0;
    float m_tms10 = 0;
    float m_v10 = 0;
    float m_tms11 = 0;
    float m_v11 = 0;
    float m_tms12 = 0;
    float m_v12 = 0;
    float m_tms13 = 0;
    float m_v13 = 0;
    float m_tms14 = 0;
    float m_v14 = 0;
    float m_tms15 = 0;
    float m_v15 = 0;
    float m_tms16 = 0;
    float m_v16 = 0;
    float m_tms17 = 0;
    float m_v17 = 0;
    float m_tms18 = 0;
    float m_v18 = 0;
    float m_tms19 = 0;
    float m_v19 = 0;
    float m_tms20 = 0;
    float m_v20 = 0;
    QString m_crvNam;
    Readonly m_readOnly;

};



class SunSpecLvrtcModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Modena {
        ModenaEnabled = 0x1
    };
    Q_DECLARE_FLAGS(ModenaFlags, Modena)
    Q_FLAG(Modena)

    explicit SunSpecLvrtcModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecLvrtcModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;


    /* Index of active curve. 0=no active curve. */
    quint16 actCrv() const;
    QModbusReply *setActCrv(quint16 actCrv);

    /* LVRT control mode. Enable active curve.  Bitfield value. */
    ModenaFlags modEna() const;
    QModbusReply *setModEna(ModenaFlags modEna);

    /* Time window for LVRT change. [Secs] */
    quint16 winTms() const;
    QModbusReply *setWinTms(quint16 winTms);

    /* Timeout period for LVRT curve selection. [Secs] */
    quint16 rvrtTms() const;
    QModbusReply *setRvrtTms(quint16 rvrtTms);

    /* Ramp time for moving from current mode to new mode. [Secs] */
    quint16 rmpTms() const;
    QModbusReply *setRmpTms(quint16 rmpTms);

    /* Number of curves supported (recommend 4). */
    quint16 nCrv() const;
    /* Number of curve points supported (maximum of 20). */
    quint16 nPt() const;
    /* Scale factor for duration. */
    qint16 tmsSf() const;
    /* Scale factor for percent VRef. */
    qint16 vSf() const;
    quint16 pad() const;

protected:
    quint16 m_fixedBlockLength = 10;
    quint16 m_repeatingBlockLength = 50;

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
    qint16 m_tmsSf = 0;
    qint16 m_vSf = 0;
    quint16 m_pad = 0;


};

QDebug operator<<(QDebug debug, SunSpecLvrtcModel *model);

#endif // SUNSPECLVRTCMODEL_H
