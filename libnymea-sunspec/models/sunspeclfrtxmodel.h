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

#ifndef SUNSPECLFRTXMODEL_H
#define SUNSPECLFRTXMODEL_H

#include <QObject>

#include "sunspecmodel.h"
#include "sunspecmodelrepeatingblock.h"

class SunSpecConnection;
class SunSpecLfrtxModel;

class SunSpecLfrtxModelRepeatingBlock : public SunSpecModelRepeatingBlock
{
    Q_OBJECT
public:

    enum Readonly {
        ReadonlyReadwrite = 0,
        ReadonlyReadonly = 1
    };
    Q_ENUM(Readonly)

    explicit SunSpecLfrtxModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecLfrtxModel *parent);
    ~SunSpecLfrtxModelRepeatingBlock() override = default;

    SunSpecLfrtxModel *parentModel() const;

    QString name() const override;

    /* Number of active points in array. */
    quint16 actPt() const;
    QModbusReply *setActPt(quint16 actPt);

    /* Point 1 duration. [Secs] */
    float tms1() const;
    QModbusReply *setTms1(float tms1);

    /* Point 1 frequency. [Hz] */
    float hz1() const;
    QModbusReply *setHz1(float hz1);

    /* Point 2 duration. [Secs] */
    float tms2() const;
    QModbusReply *setTms2(float tms2);

    /* Point 2 frequency. [Hz] */
    float hz2() const;
    QModbusReply *setHz2(float hz2);

    /* Point 3 duration. [Secs] */
    float tms3() const;
    QModbusReply *setTms3(float tms3);

    /* Point 3 frequency. [Hz] */
    float hz3() const;
    QModbusReply *setHz3(float hz3);

    /* Point 4 duration. [Secs] */
    float tms4() const;
    QModbusReply *setTms4(float tms4);

    /* Point 4 frequency. [Hz] */
    float hz4() const;
    QModbusReply *setHz4(float hz4);

    /* Point 5 duration. [Secs] */
    float tms5() const;
    QModbusReply *setTms5(float tms5);

    /* Point 5 frequency. [Hz] */
    float hz5() const;
    QModbusReply *setHz5(float hz5);

    /* Point 6 duration. [Secs] */
    float tms6() const;
    QModbusReply *setTms6(float tms6);

    /* Point 6 frequency. [Hz] */
    float hz6() const;
    QModbusReply *setHz6(float hz6);

    /* Point 7 duration. [Secs] */
    float tms7() const;
    QModbusReply *setTms7(float tms7);

    /* Point 7 frequency. [Hz] */
    float hz7() const;
    QModbusReply *setHz7(float hz7);

    /* Point 8 duration. [Secs] */
    float tms8() const;
    QModbusReply *setTms8(float tms8);

    /* Point 8 frequency. [Hz] */
    float hz8() const;
    QModbusReply *setHz8(float hz8);

    /* Point 9 duration. [Secs] */
    float tms9() const;
    QModbusReply *setTms9(float tms9);

    /* Point 9 frequency. [Hz] */
    float hz9() const;
    QModbusReply *setHz9(float hz9);

    /* Point 10 duration. [Secs] */
    float tms10() const;
    QModbusReply *setTms10(float tms10);

    /* Point 10 frequency. [Hz] */
    float hz10() const;
    QModbusReply *setHz10(float hz10);

    /* Point 11 duration. [Secs] */
    float tms11() const;
    QModbusReply *setTms11(float tms11);

    /* Point 11 frequency. [Hz] */
    float hz11() const;
    QModbusReply *setHz11(float hz11);

    /* Point 12 duration. [Secs] */
    float tms12() const;
    QModbusReply *setTms12(float tms12);

    /* Point 12 frequency. [Hz] */
    float hz12() const;
    QModbusReply *setHz12(float hz12);

    /* Point 13 duration. [Secs] */
    float tms13() const;
    QModbusReply *setTms13(float tms13);

    /* Point 13 frequency. [Hz] */
    float hz13() const;
    QModbusReply *setHz13(float hz13);

    /* Point 14 duration. [Secs] */
    float tms14() const;
    QModbusReply *setTms14(float tms14);

    /* Point 14 frequency. [Hz] */
    float hz14() const;
    QModbusReply *setHz14(float hz14);

    /* Point 15 duration. [Secs] */
    float tms15() const;
    QModbusReply *setTms15(float tms15);

    /* Point 15 frequency. [Hz] */
    float hz15() const;
    QModbusReply *setHz15(float hz15);

    /* Point 16 duration. [Secs] */
    float tms16() const;
    QModbusReply *setTms16(float tms16);

    /* Point 16 frequency. [Hz] */
    float hz16() const;
    QModbusReply *setHz16(float hz16);

    /* Point 17 duration. [Secs] */
    float tms17() const;
    QModbusReply *setTms17(float tms17);

    /* Point 17 frequency. [Hz] */
    float hz17() const;
    QModbusReply *setHz17(float hz17);

    /* Point 18 duration. [Secs] */
    float tms18() const;
    QModbusReply *setTms18(float tms18);

    /* Point 18 frequency. [Hz] */
    float hz18() const;
    QModbusReply *setHz18(float hz18);

    /* Point 19 duration. [Secs] */
    float tms19() const;
    QModbusReply *setTms19(float tms19);

    /* Point 19 frequency. [Hz] */
    float hz19() const;
    QModbusReply *setHz19(float hz19);

    /* Point 20 duration. [Secs] */
    float tms20() const;
    QModbusReply *setTms20(float tms20);

    /* Point 20 frequency. [Hz] */
    float hz20() const;
    QModbusReply *setHz20(float hz20);

    /* Optional description for curve. */
    QString crvNam() const;
    QModbusReply *setCrvNam(const QString &crvNam);

    /* Enumerated value indicates if curve is read-only or can be modified. */
    Readonly readOnly() const;

    void processBlockData() override;

protected:
    void initDataPoints() override;

private:
    SunSpecLfrtxModel *m_parentModel = nullptr;

    quint16 m_actPt = 0;
    float m_tms1 = 0;
    float m_hz1 = 0;
    float m_tms2 = 0;
    float m_hz2 = 0;
    float m_tms3 = 0;
    float m_hz3 = 0;
    float m_tms4 = 0;
    float m_hz4 = 0;
    float m_tms5 = 0;
    float m_hz5 = 0;
    float m_tms6 = 0;
    float m_hz6 = 0;
    float m_tms7 = 0;
    float m_hz7 = 0;
    float m_tms8 = 0;
    float m_hz8 = 0;
    float m_tms9 = 0;
    float m_hz9 = 0;
    float m_tms10 = 0;
    float m_hz10 = 0;
    float m_tms11 = 0;
    float m_hz11 = 0;
    float m_tms12 = 0;
    float m_hz12 = 0;
    float m_tms13 = 0;
    float m_hz13 = 0;
    float m_tms14 = 0;
    float m_hz14 = 0;
    float m_tms15 = 0;
    float m_hz15 = 0;
    float m_tms16 = 0;
    float m_hz16 = 0;
    float m_tms17 = 0;
    float m_hz17 = 0;
    float m_tms18 = 0;
    float m_hz18 = 0;
    float m_tms19 = 0;
    float m_hz19 = 0;
    float m_tms20 = 0;
    float m_hz20 = 0;
    QString m_crvNam;
    Readonly m_readOnly;

};



class SunSpecLfrtxModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Crvtype {
        CrvtypeCeaseToEnergize = 1
    };
    Q_ENUM(Crvtype)

    enum Modena {
        ModenaEnabled = 0x1
    };
    Q_DECLARE_FLAGS(ModenaFlags, Modena)
    Q_FLAG(Modena)

    explicit SunSpecLfrtxModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecLfrtxModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;


    /* Index of active curve. 0=no active curve. */
    quint16 actCrv() const;
    QModbusReply *setActCrv(quint16 actCrv);

    /* LHzRT control mode. Enable active curve.  Bitfield value. */
    ModenaFlags modEna() const;
    QModbusReply *setModEna(ModenaFlags modEna);

    /* Time window for LFRT change. [Secs] */
    quint16 winTms() const;
    QModbusReply *setWinTms(quint16 winTms);

    /* Timeout period for LFRT curve selection. [Secs] */
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
    /* Scale factor for frequency. */
    qint16 hzSf() const;
    Crvtype crvType() const;

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
    qint16 m_hzSf = 0;
    Crvtype m_crvType;


};

QDebug operator<<(QDebug debug, SunSpecLfrtxModel *model);

#endif // SUNSPECLFRTXMODEL_H
