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

#ifndef SUNSPECFLOWBATTERYSTRINGMODEL_H
#define SUNSPECFLOWBATTERYSTRINGMODEL_H

#include <QObject>

#include "sunspecmodel.h"
#include "sunspecmodelrepeatingblock.h"

class SunSpecConnection;
class SunSpecFlowBatteryStringModel;

class SunSpecFlowBatteryStringModelRepeatingBlock : public SunSpecModelRepeatingBlock
{
    Q_OBJECT
public:

    enum Modconfail {
        ModconfailNoFailure = 0,
        ModconfailButtonPushed = 1,
        ModconfailModuleGroundFault = 2,
        ModconfailOutsideVoltageRange = 3,
        ModconfailModuleNotEnabled = 4,
        ModconfailFuseOpen = 5,
        ModconfailContactorFailure = 6,
        ModconfailPrechargeFailure = 7,
        ModconfailModuleFault = 8
    };
    Q_ENUM(Modconfail)

    enum Modsetena {
        ModsetenaEnableModule = 1,
        ModsetenaDisableModule = 2
    };
    Q_ENUM(Modsetena)

    enum Modsetcon {
        ModsetconConnectModule = 1,
        ModsetconDisconnectModule = 2
    };
    Q_ENUM(Modsetcon)

    enum Moddisrsn {
        ModdisrsnNone = 0,
        ModdisrsnFault = 1,
        ModdisrsnMaintenance = 2,
        ModdisrsnExternal = 3,
        ModdisrsnOther = 4
    };
    Q_ENUM(Moddisrsn)

    enum Modst {
        ModstModuleEnabled = 0x1,
        ModstContactorStatus = 0x2
    };
    Q_DECLARE_FLAGS(ModstFlags, Modst)
    Q_FLAG(Modst)

    enum Modconst {
        ModconstContactor0 = 0x1,
        ModconstContactor1 = 0x2,
        ModconstContactor2 = 0x4,
        ModconstContactor3 = 0x8,
        ModconstContactor4 = 0x10,
        ModconstContactor5 = 0x20,
        ModconstContactor6 = 0x40,
        ModconstContactor7 = 0x80,
        ModconstContactor8 = 0x100,
        ModconstContactor9 = 0x200,
        ModconstContactor10 = 0x400,
        ModconstContactor11 = 0x800,
        ModconstContactor12 = 0x1000,
        ModconstContactor13 = 0x2000,
        ModconstContactor14 = 0x4000,
        ModconstContactor15 = 0x8000,
        ModconstContactor16 = 0x10000,
        ModconstContactor17 = 0x20000,
        ModconstContactor18 = 0x40000,
        ModconstContactor19 = 0x80000,
        ModconstContactor20 = 0x100000,
        ModconstContactor21 = 0x200000,
        ModconstContactor22 = 0x400000,
        ModconstContactor23 = 0x800000,
        ModconstContactor24 = 0x1000000,
        ModconstContactor25 = 0x2000000,
        ModconstContactor26 = 0x4000000,
        ModconstContactor27 = 0x8000000,
        ModconstContactor28 = 0x10000000,
        ModconstContactor29 = 0x20000000,
        ModconstContactor30 = 0x40000000
    };
    Q_DECLARE_FLAGS(ModconstFlags, Modconst)
    Q_FLAG(Modconst)

    enum Modevt1 {
        Modevt1CommunicationError = 0x1,
        Modevt1OverTempAlarm = 0x2,
        Modevt1OverTempWarning = 0x4,
        Modevt1UnderTempAlarm = 0x8,
        Modevt1UnderTempWarning = 0x10,
        Modevt1OverChargeCurrentAlarm = 0x20,
        Modevt1OverChargeCurrentWarning = 0x40,
        Modevt1OverDischargeCurrentAlarm = 0x80,
        Modevt1OverDischargeCurrentWarning = 0x100,
        Modevt1OverVoltAlarm = 0x200,
        Modevt1OverVoltWarning = 0x400,
        Modevt1UnderVoltAlarm = 0x800,
        Modevt1UnderVoltWarning = 0x1000,
        Modevt1UnderSocMinAlarm = 0x2000,
        Modevt1UnderSocMinWarning = 0x4000,
        Modevt1OverSocMaxAlarm = 0x8000,
        Modevt1OverSocMaxWarning = 0x10000,
        Modevt1VoltageImbalanceWarning = 0x20000,
        Modevt1Reserved1 = 0x40000,
        Modevt1Reserved2 = 0x80000,
        Modevt1ContactorError = 0x100000,
        Modevt1FanError = 0x200000,
        Modevt1GroundFault = 0x400000,
        Modevt1OpenDoorError = 0x800000,
        Modevt1Reserved3 = 0x1000000,
        Modevt1Reserved4 = 0x2000000,
        Modevt1Reserved5 = 0x4000000,
        Modevt1FireAlarm = 0x8000000,
        Modevt1ModuleConfigurationAlarm = 0x10000000,
        Modevt1ModuleConfigurationWarning = 0x20000000
    };
    Q_DECLARE_FLAGS(Modevt1Flags, Modevt1)
    Q_FLAG(Modevt1)

    enum Modevt2 {
        Modevt2LeakAlarm = 0x1,
        Modevt2PumpAlarm = 0x2,
        Modevt2HighPressureAlarm = 0x4,
        Modevt2HighPressureWarning = 0x8,
        Modevt2LowFlowAlarm = 0x10,
        Modevt2LowFlowWarning = 0x20
    };
    Q_DECLARE_FLAGS(Modevt2Flags, Modevt2)
    Q_FLAG(Modevt2)

    explicit SunSpecFlowBatteryStringModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecFlowBatteryStringModel *parent);
    ~SunSpecFlowBatteryStringModelRepeatingBlock() override = default;

    SunSpecFlowBatteryStringModel *parentModel() const;

    QString name() const override;
    /* Index of the module within the string. */
    quint16 moduleIndex() const;
    /* Number of stacks in this module. */
    quint16 stackCount() const;
    /* Current status of the module. */
    ModstFlags moduleStatus() const;
    /* State of charge for this module. [%] */
    float moduleStateOfCharge() const;
    /* Open circuit voltage for this module. [V] */
    float openCircuitVoltage() const;
    /* External voltage fo this module. [V] */
    float externalVoltage() const;
    /* Maximum voltage for all cells in this module. [V] */
    float maximumCellVoltage() const;
    /* Cell with the maximum cell voltage. */
    quint16 maxCellVoltageCell() const;
    /* Minimum voltage for all cells in this module. [V] */
    float minimumCellVoltage() const;
    /* Cell with the minimum cell voltage. */
    quint16 minCellVoltageCell() const;
    /* Average voltage for all cells in this module. [V] */
    float averageCellVoltage() const;
    /* Anolyte Temperature [C] */
    float anolyteTemperature() const;
    /* Catholyte Temperature [C] */
    float catholyteTemperature() const;
    /* Contactor Status */
    ModconstFlags contactorStatus() const;
    /* Alarms, warnings and status values.  Bit flags. */
    Modevt1Flags moduleEvent1() const;
    /* Alarms, warnings and status values.  Bit flags. */
    Modevt2Flags moduleEvent2() const;
    /* Connection Failure Reason */
    Modconfail connectionFailureReason() const;

    /* Enables and disables the module. */
    Modsetena enableDisableModule() const;
    QModbusReply *setEnableDisableModule(Modsetena enableDisableModule);

    /* Connects and disconnects the module. */
    Modsetcon connectDisconnectModule() const;
    QModbusReply *setConnectDisconnectModule(Modsetcon connectDisconnectModule);

    /* Reason why the module is currently disabled. */
    Moddisrsn disabledReason() const;

    void processBlockData() override;

protected:
    void initDataPoints() override;

private:
    SunSpecFlowBatteryStringModel *m_parentModel = nullptr;

    quint16 m_moduleIndex = 0;
    quint16 m_stackCount = 0;
    ModstFlags m_moduleStatus;
    float m_moduleStateOfCharge = 0;
    float m_openCircuitVoltage = 0;
    float m_externalVoltage = 0;
    float m_maximumCellVoltage = 0;
    quint16 m_maxCellVoltageCell = 0;
    float m_minimumCellVoltage = 0;
    quint16 m_minCellVoltageCell = 0;
    float m_averageCellVoltage = 0;
    float m_anolyteTemperature = 0;
    float m_catholyteTemperature = 0;
    ModconstFlags m_contactorStatus;
    Modevt1Flags m_moduleEvent1;
    Modevt2Flags m_moduleEvent2;
    Modconfail m_connectionFailureReason;
    Modsetena m_enableDisableModule;
    Modsetcon m_connectDisconnectModule;
    Moddisrsn m_disabledReason;

};



class SunSpecFlowBatteryStringModel : public SunSpecModel
{
    Q_OBJECT
public:

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
        Evt1Reserved1 = 0x40000,
        Evt1Reserved2 = 0x80000,
        Evt1ContactorError = 0x100000,
        Evt1FanError = 0x200000,
        Evt1GroundFault = 0x400000,
        Evt1OpenDoorError = 0x800000,
        Evt1Reserved3 = 0x1000000,
        Evt1OtherAlarm = 0x2000000,
        Evt1OtherWarning = 0x4000000,
        Evt1FireAlarm = 0x8000000,
        Evt1ConfigurationAlarm = 0x10000000,
        Evt1ConfigurationWarning = 0x20000000
    };
    Q_DECLARE_FLAGS(Evt1Flags, Evt1)
    Q_FLAG(Evt1)

    enum Evt2 {
        Evt2LeakAlarm = 0x1,
        Evt2PumpAlarm = 0x2,
        Evt2HighPressureAlarm = 0x4,
        Evt2HighPressureWarning = 0x8,
        Evt2LowFlowAlarm = 0x10,
        Evt2LowFlowWarning = 0x20
    };
    Q_DECLARE_FLAGS(Evt2Flags, Evt2)
    Q_FLAG(Evt2)

    explicit SunSpecFlowBatteryStringModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecFlowBatteryStringModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    /* Index of the string within the bank. */
    quint16 stringIndex() const;
    /* Number of modules in this string. */
    quint16 moduleCount() const;
    /* Number of electrically connected modules in this string. */
    quint16 connectedModuleCount() const;
    /* Maximum voltage for all modules in the string. [V] */
    float maxModuleVoltage() const;
    /* Module with the maximum voltage. */
    quint16 maxModuleVoltageModule() const;
    /* Minimum voltage for all modules in the string. [V] */
    float minModuleVoltage() const;
    /* Module with the minimum voltage. */
    quint16 minModuleVoltageModule() const;
    /* Average voltage for all modules in the string. [V] */
    float averageModuleVoltage() const;
    /* Maximum voltage for all cells in the string. [V] */
    float maxCellVoltage() const;
    /* Module containing the cell with the maximum voltage. */
    quint16 maxCellVoltageModule() const;
    /* Stack containing the cell with the maximum voltage. */
    quint16 maxCellVoltageStack() const;
    /* Minimum voltage for all cells in the string. [V] */
    float minCellVoltage() const;
    /* Module containing the cell with the minimum voltage. */
    quint16 minCellVoltageModule() const;
    /* Stack containing the cell with the minimum voltage. */
    quint16 minCellVoltageStack() const;
    /* Average voltage for all cells in the string. [V] */
    float averageCellVoltage() const;
    /* Maximum electrolyte temperature for all modules in the string. [C] */
    float maxTemperature() const;
    /* Module with the maximum temperature. */
    quint16 maxTemperatureModule() const;
    /* Minimum electrolyte temperature for all modules in the string. [C] */
    float minTemperature() const;
    /* Module with the minimum temperature. */
    quint16 minTemperatureModule() const;
    /* Average electrolyte temperature for all modules in the string. [C] */
    float averageTemperature() const;
    /* Alarms, warnings and status values.  Bit flags. */
    Evt1Flags stringEvent1() const;
    /* Alarms, warnings and status values.  Bit flags. */
    Evt2Flags stringEvent2() const;
    /* Vendor defined events. */
    quint32 vendorEventBitfield1() const;
    /* Vendor defined events. */
    quint32 vendorEventBitfield2() const;
    qint16 modV_SF() const;
    /* Scale factor for voltage. */
    qint16 cellV_SF() const;
    /* Scale factor for temperature. */
    qint16 tmp_SF() const;
    /* Scale factor for state of charge. */
    qint16 soC_SF() const;
    /* Scale factor for open circuit voltage. */
    qint16 oCV_SF() const;
    /* Pad register. */
    quint16 pad1() const;

protected:
    quint16 m_fixedBlockLength = 34;
    quint16 m_repeatingBlockLength = 24;

    void initDataPoints();
    void processBlockData() override;

private:

    void setupRepeatingBlocks();

    quint16 m_stringIndex = 0;
    quint16 m_moduleCount = 0;
    quint16 m_connectedModuleCount = 0;
    float m_maxModuleVoltage = 0;
    quint16 m_maxModuleVoltageModule = 0;
    float m_minModuleVoltage = 0;
    quint16 m_minModuleVoltageModule = 0;
    float m_averageModuleVoltage = 0;
    float m_maxCellVoltage = 0;
    quint16 m_maxCellVoltageModule = 0;
    quint16 m_maxCellVoltageStack = 0;
    float m_minCellVoltage = 0;
    quint16 m_minCellVoltageModule = 0;
    quint16 m_minCellVoltageStack = 0;
    float m_averageCellVoltage = 0;
    float m_maxTemperature = 0;
    quint16 m_maxTemperatureModule = 0;
    float m_minTemperature = 0;
    quint16 m_minTemperatureModule = 0;
    float m_averageTemperature = 0;
    Evt1Flags m_stringEvent1;
    Evt2Flags m_stringEvent2;
    quint32 m_vendorEventBitfield1 = 0;
    quint32 m_vendorEventBitfield2 = 0;
    qint16 m_modV_SF = 0;
    qint16 m_cellV_SF = 0;
    qint16 m_tmp_SF = 0;
    qint16 m_soC_SF = 0;
    qint16 m_oCV_SF = 0;
    quint16 m_pad1 = 0;


};

QDebug operator<<(QDebug debug, SunSpecFlowBatteryStringModel *model);

#endif // SUNSPECFLOWBATTERYSTRINGMODEL_H
