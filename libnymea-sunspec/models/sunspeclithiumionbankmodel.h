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

#ifndef SUNSPECLITHIUMIONBANKMODEL_H
#define SUNSPECLITHIUMIONBANKMODEL_H

#include <QObject>

#include "sunspecmodel.h"
#include "sunspecmodelrepeatingblock.h"

class SunSpecConnection;
class SunSpecLithiumIonBankModel;

class SunSpecLithiumIonBankModelRepeatingBlock : public SunSpecModelRepeatingBlock
{
    Q_OBJECT
public:

    enum Strconfail {
        StrconfailNoFailure = 0,
        StrconfailButtonPushed = 1,
        StrconfailStrGroundFault = 2,
        StrconfailOutsideVoltageRange = 3,
        StrconfailStringNotEnabled = 4,
        StrconfailFuseOpen = 5,
        StrconfailContactorFailure = 6,
        StrconfailPrechargeFailure = 7,
        StrconfailStringFault = 8
    };
    Q_ENUM(Strconfail)

    enum Strdisrsn {
        StrdisrsnNone = 0,
        StrdisrsnFault = 1,
        StrdisrsnMaintenance = 2,
        StrdisrsnExternal = 3,
        StrdisrsnOther = 4
    };
    Q_ENUM(Strdisrsn)

    enum Strsetena {
        StrsetenaEnableString = 1,
        StrsetenaDisableString = 2
    };
    Q_ENUM(Strsetena)

    enum Strsetcon {
        StrsetconConnectString = 1,
        StrsetconDisconnectString = 2
    };
    Q_ENUM(Strsetcon)

    enum Strst {
        StrstStringEnabled = 0x1,
        StrstContactorStatus = 0x2
    };
    Q_DECLARE_FLAGS(StrstFlags, Strst)
    Q_FLAG(Strst)

    enum Strconst {
        StrconstContactor0 = 0x1,
        StrconstContactor1 = 0x2,
        StrconstContactor2 = 0x4,
        StrconstContactor3 = 0x8,
        StrconstContactor4 = 0x10,
        StrconstContactor5 = 0x20,
        StrconstContactor6 = 0x40,
        StrconstContactor7 = 0x80,
        StrconstContactor8 = 0x100,
        StrconstContactor9 = 0x200,
        StrconstContactor10 = 0x400,
        StrconstContactor11 = 0x800,
        StrconstContactor12 = 0x1000,
        StrconstContactor13 = 0x2000,
        StrconstContactor14 = 0x4000,
        StrconstContactor15 = 0x8000,
        StrconstContactor16 = 0x10000,
        StrconstContactor17 = 0x20000,
        StrconstContactor18 = 0x40000,
        StrconstContactor19 = 0x80000,
        StrconstContactor20 = 0x100000,
        StrconstContactor21 = 0x200000,
        StrconstContactor22 = 0x400000,
        StrconstContactor23 = 0x800000,
        StrconstContactor24 = 0x1000000,
        StrconstContactor25 = 0x2000000,
        StrconstContactor26 = 0x4000000,
        StrconstContactor27 = 0x8000000,
        StrconstContactor28 = 0x10000000,
        StrconstContactor29 = 0x20000000,
        StrconstContactor30 = 0x40000000
    };
    Q_DECLARE_FLAGS(StrconstFlags, Strconst)
    Q_FLAG(Strconst)

    enum Strevt1 {
        Strevt1CommunicationError = 0x1,
        Strevt1OverTempAlarm = 0x2,
        Strevt1OverTempWarning = 0x4,
        Strevt1UnderTempAlarm = 0x8,
        Strevt1UnderTempWarning = 0x10,
        Strevt1OverChargeCurrentAlarm = 0x20,
        Strevt1OverChargeCurrentWarning = 0x40,
        Strevt1OverDischargeCurrentAlarm = 0x80,
        Strevt1OverDischargeCurrentWarning = 0x100,
        Strevt1OverVoltAlarm = 0x200,
        Strevt1OverVoltWarning = 0x400,
        Strevt1UnderVoltAlarm = 0x800,
        Strevt1UnderVoltWarning = 0x1000,
        Strevt1UnderSocMinAlarm = 0x2000,
        Strevt1UnderSocMinWarning = 0x4000,
        Strevt1OverSocMaxAlarm = 0x8000,
        Strevt1OverSocMaxWarning = 0x10000,
        Strevt1VoltageImbalanceWarning = 0x20000,
        Strevt1TemperatureImbalanceAlarm = 0x40000,
        Strevt1TemperatureImbalanceWarning = 0x80000,
        Strevt1ContactorError = 0x100000,
        Strevt1FanError = 0x200000,
        Strevt1GroundFault = 0x400000,
        Strevt1OpenDoorError = 0x800000,
        Strevt1Reserved1 = 0x1000000,
        Strevt1OtherAlarm = 0x2000000,
        Strevt1OtherWarning = 0x4000000,
        Strevt1Reserved2 = 0x8000000,
        Strevt1ConfigurationAlarm = 0x10000000,
        Strevt1ConfigurationWarning = 0x20000000
    };
    Q_DECLARE_FLAGS(Strevt1Flags, Strevt1)
    Q_FLAG(Strevt1)

    explicit SunSpecLithiumIonBankModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecLithiumIonBankModel *parent = nullptr);
    ~SunSpecLithiumIonBankModelRepeatingBlock() override; 

    SunSpecLithiumIonBankModel *parentModel() const; 

    QString name() const override;
    quint16 moduleCount() const;
    StrstFlags stringStatus() const;
    Strconfail connectionFailureReason() const;
    float stringStateOfCharge() const;
    float stringStateOfHealth() const;
    float stringCurrent() const;
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
    Strdisrsn disabledReason() const;
    StrconstFlags contactorStatus() const;
    Strevt1Flags stringEvent1() const;
    quint32 stringEvent2() const;
    quint32 vendorStringEventBitfield1() const;
    quint32 vendorStringEventBitfield2() const;

    Strsetena enableDisableString() const;
    QModbusReply *setEnableDisableString(Strsetena enableDisableString);

    Strsetcon connectDisconnectString() const;
    QModbusReply *setConnectDisconnectString(Strsetcon connectDisconnectString);

    quint16 pad1() const;
    quint16 pad2() const;

    void processBlockData(const QVector<quint16> blockData) override;

protected:
    void initDataPoints() override;

private:
    SunSpecLithiumIonBankModel *m_parentModel = nullptr; 

    quint16 m_moduleCount = 0;
    StrstFlags m_stringStatus;
    Strconfail m_connectionFailureReason;
    float m_stringStateOfCharge = 0;
    float m_stringStateOfHealth = 0;
    float m_stringCurrent = 0;
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
    Strdisrsn m_disabledReason;
    StrconstFlags m_contactorStatus;
    Strevt1Flags m_stringEvent1;
    quint32 m_stringEvent2 = 0;
    quint32 m_vendorStringEventBitfield1 = 0;
    quint32 m_vendorStringEventBitfield2 = 0;
    Strsetena m_enableDisableString;
    Strsetcon m_connectDisconnectString;
    quint16 m_pad1 = 0;
    quint16 m_pad2 = 0;

};



class SunSpecLithiumIonBankModel : public SunSpecModel
{
    Q_OBJECT
public:

    explicit SunSpecLithiumIonBankModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, QObject *parent = nullptr);
    ~SunSpecLithiumIonBankModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    quint16 stringCount() const;
    quint16 connectedStringCount() const;
    float maxModuleTemperature() const;
    quint16 maxModuleTemperatureString() const;
    quint16 maxModuleTemperatureModule() const;
    float minModuleTemperature() const;
    quint16 minModuleTemperatureString() const;
    quint16 minModuleTemperatureModule() const;
    qint16 averageModuleTemperature() const;
    float maxStringVoltage() const;
    quint16 maxStringVoltageString() const;
    float minStringVoltage() const;
    quint16 minStringVoltageString() const;
    float averageStringVoltage() const;
    float maxStringCurrent() const;
    quint16 maxStringCurrentString() const;
    float minStringCurrent() const;
    quint16 minStringCurrentString() const;
    float averageStringCurrent() const;
    quint16 batteryCellBalancingCount() const;
    qint16 cellV_SF() const;
    qint16 modTmp_SF() const;
    qint16 a_SF() const;
    qint16 soH_SF() const;
    qint16 soC_SF() const;
    qint16 v_SF() const;

protected:
    quint16 m_fixedBlockLength = 26;
    quint16 m_repeatingBlockLength = 32;

    void initDataPoints() override;
    void processBlockData() override;

private:
    quint16 m_stringCount = 0;
    quint16 m_connectedStringCount = 0;
    float m_maxModuleTemperature = 0;
    quint16 m_maxModuleTemperatureString = 0;
    quint16 m_maxModuleTemperatureModule = 0;
    float m_minModuleTemperature = 0;
    quint16 m_minModuleTemperatureString = 0;
    quint16 m_minModuleTemperatureModule = 0;
    qint16 m_averageModuleTemperature = 0;
    float m_maxStringVoltage = 0;
    quint16 m_maxStringVoltageString = 0;
    float m_minStringVoltage = 0;
    quint16 m_minStringVoltageString = 0;
    float m_averageStringVoltage = 0;
    float m_maxStringCurrent = 0;
    quint16 m_maxStringCurrentString = 0;
    float m_minStringCurrent = 0;
    quint16 m_minStringCurrentString = 0;
    float m_averageStringCurrent = 0;
    quint16 m_batteryCellBalancingCount = 0;
    qint16 m_cellV_SF = 0;
    qint16 m_modTmp_SF = 0;
    qint16 m_a_SF = 0;
    qint16 m_soH_SF = 0;
    qint16 m_soC_SF = 0;
    qint16 m_v_SF = 0;


};

QDebug operator<<(QDebug debug, SunSpecLithiumIonBankModel *model);

#endif // SUNSPECLITHIUMIONBANKMODEL_H
