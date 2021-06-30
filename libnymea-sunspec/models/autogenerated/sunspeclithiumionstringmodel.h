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

#ifndef SUNSPECLITHIUMIONSTRINGMODEL_H
#define SUNSPECLITHIUMIONSTRINGMODEL_H

#include <QObject>

#include "sunspec.h"
#include "sunspecmodel.h"

class SunSpecLithiumIonStringModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Confail {
        ConfailNoFailure = 0,
        ConfailButtonPushed = 1,
        ConfailStrGroundFault = 2,
        ConfailOutsideVoltageRange = 3,
        ConfailStringNotEnabled = 4,
        ConfailFuseOpen = 5,
        ConfailContactorFailure = 6,
        ConfailPrechargeFailure = 7,
        ConfailStringFault = 8
    };
    Q_ENUM(Confail)

    enum Setcon {
        SetconConnectString = 1,
        SetconDisconnectString = 2
    };
    Q_ENUM(Setcon)

    enum St {
        StStringEnabled = 0x1,
        StContactorStatus = 0x2
    };
    Q_DECLARE_FLAGS(StFlags, St)
    Q_FLAG(St)

    enum Const {
        ConstContactor0 = 0x1,
        ConstContactor1 = 0x2,
        ConstContactor2 = 0x4,
        ConstContactor3 = 0x8,
        ConstContactor4 = 0x10,
        ConstContactor5 = 0x20,
        ConstContactor6 = 0x40,
        ConstContactor7 = 0x80,
        ConstContactor8 = 0x100,
        ConstContactor9 = 0x200,
        ConstContactor10 = 0x400,
        ConstContactor11 = 0x800,
        ConstContactor12 = 0x1000,
        ConstContactor13 = 0x2000,
        ConstContactor14 = 0x4000,
        ConstContactor15 = 0x8000,
        ConstContactor16 = 0x10000,
        ConstContactor17 = 0x20000,
        ConstContactor18 = 0x40000,
        ConstContactor19 = 0x80000,
        ConstContactor20 = 0x100000,
        ConstContactor21 = 0x200000,
        ConstContactor22 = 0x400000,
        ConstContactor23 = 0x800000,
        ConstContactor24 = 0x1000000,
        ConstContactor25 = 0x2000000,
        ConstContactor26 = 0x4000000,
        ConstContactor27 = 0x8000000,
        ConstContactor28 = 0x10000000,
        ConstContactor29 = 0x20000000,
        ConstContactor30 = 0x40000000
    };
    Q_DECLARE_FLAGS(ConstFlags, Const)
    Q_FLAG(Const)

    enum Evt1 {
        Evt1CommunicationError = 0x1,
        Evt1OverTempAlarm = 0x2,
        Evt1OverTempWarning = 0x4,
        Evt1UnderTempAlarm = 0x8,
        Evt1UnderTempWarning = 0x10,
        Evt1OverChargeCurrentAlarm = 0x20,
        Evt1OverChargeCurrentWarning = 0x40,
        Evt1OverDischargeCurrentAlarm = 0x80,
        Evt1OverDischargeCurrentWarning = 0x100,
        Evt1OverVoltAlarm = 0x200,
        Evt1OverVoltWarning = 0x400,
        Evt1UnderVoltAlarm = 0x800,
        Evt1UnderVoltWarning = 0x1000,
        Evt1UnderSocMinAlarm = 0x2000,
        Evt1UnderSocMinWarning = 0x4000,
        Evt1OverSocMaxAlarm = 0x8000,
        Evt1OverSocMaxWarning = 0x10000,
        Evt1VoltageImbalanceWarning = 0x20000,
        Evt1TemperatureImbalanceAlarm = 0x40000,
        Evt1TemperatureImbalanceWarning = 0x80000,
        Evt1ContactorError = 0x100000,
        Evt1FanError = 0x200000,
        Evt1GroundFault = 0x400000,
        Evt1OpenDoorError = 0x800000,
        Evt1Reserved1 = 0x1000000,
        Evt1OtherAlarm = 0x2000000,
        Evt1OtherWarning = 0x4000000,
        Evt1Reserved2 = 0x8000000,
        Evt1ConfigurationAlarm = 0x10000000,
        Evt1ConfigurationWarning = 0x20000000
    };
    Q_DECLARE_FLAGS(Evt1Flags, Evt1)
    Q_FLAG(Evt1)

    explicit SunSpecLithiumIonStringModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecLithiumIonStringModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    quint16 modelId() const;
    quint16 modelLength() const;
    quint16 stringIndex() const;
    quint16 moduleCount() const;
    StFlags stringStatus() const;
    Confail connectionFailureReason() const;
    quint16 stringCellBalancingCount() const;
    float stringStateOfCharge() const;
    float stringDepthOfDischarge() const;
    quint32 stringCycleCount() const;
    float stringStateOfHealth() const;
    qint16 stringCurrent() const;
    float stringVoltage() const;
    float maxCellVoltage() const;
    quint16 maxCellVoltageModule() const;
    float minCellVoltage() const;
    quint16 minCellVoltageModule() const;
    float averageCellVoltage() const;
    qint16 maxModuleTemperature() const;
    quint16 maxModuleTemperatureModule() const;
    qint16 minModuleTemperature() const;
    quint16 minModuleTemperatureModule() const;
    qint16 averageModuleTemperature() const;
    quint16 pad() const;
    ConstFlags contactorStatus() const;
    Evt1Flags stringEvent1() const;
    quint32 stringEvent2() const;
    quint32 vendorEventBitfield1() const;
    quint32 vendorEventBitfield2() const;
    quint16 enableDisableString() const;
    Setcon connectDisconnectString() const;
    quint16 pad2() const;
    quint16 pad3() const;
    quint16 pad4() const;

private:
    quint16 m_modelId;
    quint16 m_modelLength;
    quint16 m_stringIndex;
    quint16 m_moduleCount;
    StFlags m_stringStatus;
    Confail m_connectionFailureReason;
    quint16 m_stringCellBalancingCount;
    float m_stringStateOfCharge;
    float m_stringDepthOfDischarge;
    quint32 m_stringCycleCount;
    float m_stringStateOfHealth;
    qint16 m_stringCurrent;
    float m_stringVoltage;
    float m_maxCellVoltage;
    quint16 m_maxCellVoltageModule;
    float m_minCellVoltage;
    quint16 m_minCellVoltageModule;
    float m_averageCellVoltage;
    qint16 m_maxModuleTemperature;
    quint16 m_maxModuleTemperatureModule;
    qint16 m_minModuleTemperature;
    quint16 m_minModuleTemperatureModule;
    qint16 m_averageModuleTemperature;
    quint16 m_pad;
    ConstFlags m_contactorStatus;
    Evt1Flags m_stringEvent1;
    quint32 m_stringEvent2;
    quint32 m_vendorEventBitfield1;
    quint32 m_vendorEventBitfield2;
    quint16 m_enableDisableString;
    Setcon m_connectDisconnectString;
    qint16 m_soC_SF;
    qint16 m_soH_SF;
    qint16 m_doD_SF;
    qint16 m_a_SF;
    qint16 m_v_SF;
    qint16 m_cellV_SF;
    qint16 m_modTmp_SF;
    quint16 m_pad2;
    quint16 m_pad3;
    quint16 m_pad4;

    void initDataPoints();

};

#endif // SUNSPECLITHIUMIONSTRINGMODEL_H
