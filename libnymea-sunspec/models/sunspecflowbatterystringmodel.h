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

#ifndef SUNSPECFLOWBATTERYSTRINGMODEL_H
#define SUNSPECFLOWBATTERYSTRINGMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

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

    explicit SunSpecFlowBatteryStringModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent = nullptr);
    ~SunSpecFlowBatteryStringModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    quint16 stringIndex() const;
    quint16 moduleCount() const;
    quint16 connectedModuleCount() const;
    float maxModuleVoltage() const;
    quint16 maxModuleVoltageModule() const;
    float minModuleVoltage() const;
    quint16 minModuleVoltageModule() const;
    float averageModuleVoltage() const;
    float maxCellVoltage() const;
    quint16 maxCellVoltageModule() const;
    quint16 maxCellVoltageStack() const;
    float minCellVoltage() const;
    quint16 minCellVoltageModule() const;
    quint16 minCellVoltageStack() const;
    float averageCellVoltage() const;
    float maxTemperature() const;
    quint16 maxTemperatureModule() const;
    float minTemperature() const;
    quint16 minTemperatureModule() const;
    float averageTemperature() const;
    Evt1Flags stringEvent1() const;
    Evt2Flags stringEvent2() const;
    quint32 vendorEventBitfield1() const;
    quint32 vendorEventBitfield2() const;
    quint16 pad() const;

protected:
    void processBlockData() override;

private:
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
    quint16 m_pad = 0;

    void initDataPoints();

};

#endif // SUNSPECFLOWBATTERYSTRINGMODEL_H
