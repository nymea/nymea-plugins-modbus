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

#include "sunspecmodel.h"
#include "sunspecmodelrepeatingblock.h"

class SunSpecConnection;
class SunSpecLithiumIonStringModel;

class SunSpecLithiumIonStringModelRepeatingBlock : public SunSpecModelRepeatingBlock
{
    Q_OBJECT
public:

    explicit SunSpecLithiumIonStringModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecLithiumIonStringModel *parent = nullptr);
    ~SunSpecLithiumIonStringModelRepeatingBlock() override = default;

    SunSpecLithiumIonStringModel *parentModel() const;

    QString name() const override;
    quint16 moduleCellCount() const;
    float moduleSoC() const;
    float moduleSoH() const;
    float maxCellVoltage() const;
    quint16 maxCellVoltageCell() const;
    float minCellVoltage() const;
    float minCellVoltageCell() const;
    float averageCellVoltage() const;
    float maxCellTemperature() const;
    quint16 maxCellTemperatureCell() const;
    float minCellTemperature() const;
    quint16 minCellTemperatureCell() const;
    float averageCellTemperature() const;
    quint16 pad5() const;
    quint16 pad6() const;
    quint16 pad7() const;

    void processBlockData(const QVector<quint16> blockData) override;

protected:
    void initDataPoints();

private:
    SunSpecLithiumIonStringModel *m_parentModel = nullptr;

    quint16 m_moduleCellCount = 0;
    float m_moduleSoC = 0;
    float m_moduleSoH = 0;
    float m_maxCellVoltage = 0;
    quint16 m_maxCellVoltageCell = 0;
    float m_minCellVoltage = 0;
    float m_minCellVoltageCell = 0;
    float m_averageCellVoltage = 0;
    float m_maxCellTemperature = 0;
    quint16 m_maxCellTemperatureCell = 0;
    float m_minCellTemperature = 0;
    quint16 m_minCellTemperatureCell = 0;
    float m_averageCellTemperature = 0;
    quint16 m_pad5 = 0;
    quint16 m_pad6 = 0;
    quint16 m_pad7 = 0;

};



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

    explicit SunSpecLithiumIonStringModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, QObject *parent = nullptr);
    ~SunSpecLithiumIonStringModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    quint16 stringIndex() const;
    quint16 moduleCount() const;
    StFlags stringStatus() const;
    Confail connectionFailureReason() const;
    quint16 stringCellBalancingCount() const;
    float stringStateOfCharge() const;
    float stringDepthOfDischarge() const;
    quint32 stringCycleCount() const;
    float stringStateOfHealth() const;
    float stringCurrent() const;
    float stringVoltage() const;
    float maxCellVoltage() const;
    quint16 maxCellVoltageModule() const;
    float minCellVoltage() const;
    quint16 minCellVoltageModule() const;
    float averageCellVoltage() const;
    float maxModuleTemperature() const;
    quint16 maxModuleTemperatureModule() const;
    float minModuleTemperature() const;
    quint16 minModuleTemperatureModule() const;
    float averageModuleTemperature() const;
    quint16 pad1() const;
    ConstFlags contactorStatus() const;
    Evt1Flags stringEvent1() const;
    quint32 stringEvent2() const;
    quint32 vendorEventBitfield1() const;
    quint32 vendorEventBitfield2() const;

    quint16 enableDisableString() const;
    QModbusReply *setEnableDisableString(quint16 enableDisableString);

    Setcon connectDisconnectString() const;
    QModbusReply *setConnectDisconnectString(Setcon connectDisconnectString);

    qint16 soC_SF() const;
    qint16 soH_SF() const;
    qint16 doD_SF() const;
    qint16 a_SF() const;
    qint16 v_SF() const;
    qint16 cellV_SF() const;
    qint16 modTmp_SF() const;
    quint16 pad2() const;
    quint16 pad3() const;
    quint16 pad4() const;

protected:
    quint16 m_fixedBlockLength = 46;
    quint16 m_repeatingBlockLength = 16;

    void initDataPoints();
    void processBlockData() override;

private:
    quint16 m_stringIndex = 0;
    quint16 m_moduleCount = 0;
    StFlags m_stringStatus;
    Confail m_connectionFailureReason;
    quint16 m_stringCellBalancingCount = 0;
    float m_stringStateOfCharge = 0;
    float m_stringDepthOfDischarge = 0;
    quint32 m_stringCycleCount = 0;
    float m_stringStateOfHealth = 0;
    float m_stringCurrent = 0;
    float m_stringVoltage = 0;
    float m_maxCellVoltage = 0;
    quint16 m_maxCellVoltageModule = 0;
    float m_minCellVoltage = 0;
    quint16 m_minCellVoltageModule = 0;
    float m_averageCellVoltage = 0;
    float m_maxModuleTemperature = 0;
    quint16 m_maxModuleTemperatureModule = 0;
    float m_minModuleTemperature = 0;
    quint16 m_minModuleTemperatureModule = 0;
    float m_averageModuleTemperature = 0;
    quint16 m_pad1 = 0;
    ConstFlags m_contactorStatus;
    Evt1Flags m_stringEvent1;
    quint32 m_stringEvent2 = 0;
    quint32 m_vendorEventBitfield1 = 0;
    quint32 m_vendorEventBitfield2 = 0;
    quint16 m_enableDisableString = 0;
    Setcon m_connectDisconnectString;
    qint16 m_soC_SF = 0;
    qint16 m_soH_SF = 0;
    qint16 m_doD_SF = 0;
    qint16 m_a_SF = 0;
    qint16 m_v_SF = 0;
    qint16 m_cellV_SF = 0;
    qint16 m_modTmp_SF = 0;
    quint16 m_pad2 = 0;
    quint16 m_pad3 = 0;
    quint16 m_pad4 = 0;


};

QDebug operator<<(QDebug debug, SunSpecLithiumIonStringModel *model);

#endif // SUNSPECLITHIUMIONSTRINGMODEL_H
