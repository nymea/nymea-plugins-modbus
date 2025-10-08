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

#ifndef SUNSPECMPPTMODEL_H
#define SUNSPECMPPTMODEL_H

#include <QObject>

#include "sunspecmodel.h"
#include "sunspecmodelrepeatingblock.h"

class SunSpecConnection;
class SunSpecMpptModel;

class SunSpecMpptModelRepeatingBlock : public SunSpecModelRepeatingBlock
{
    Q_OBJECT
public:

    enum Dcst {
        DcstOff = 1,
        DcstSleeping = 2,
        DcstStarting = 3,
        DcstMppt = 4,
        DcstThrottled = 5,
        DcstShuttingDown = 6,
        DcstFault = 7,
        DcstStandby = 8,
        DcstTest = 9,
        DcstReserved10 = 10
    };
    Q_ENUM(Dcst)

    enum Dcevt {
        DcevtGroundFault = 0x1,
        DcevtInputOverVoltage = 0x2,
        DcevtReserved2 = 0x4,
        DcevtDcDisconnect = 0x8,
        DcevtReserved4 = 0x10,
        DcevtCabinetOpen = 0x20,
        DcevtManualShutdown = 0x40,
        DcevtOverTemp = 0x80,
        DcevtReserved8 = 0x100,
        DcevtReserved9 = 0x200,
        DcevtReserved10 = 0x400,
        DcevtReserved11 = 0x800,
        DcevtBlownFuse = 0x1000,
        DcevtUnderTemp = 0x2000,
        DcevtMemoryLoss = 0x4000,
        DcevtArcDetection = 0x8000,
        DcevtReserved16 = 0x10000,
        DcevtReserved17 = 0x20000,
        DcevtReserved18 = 0x40000,
        DcevtReserved19 = 0x80000,
        DcevtTestFailed = 0x100000,
        DcevtInputUnderVoltage = 0x200000,
        DcevtInputOverCurrent = 0x400000
    };
    Q_DECLARE_FLAGS(DcevtFlags, Dcevt)
    Q_FLAG(Dcevt)

    explicit SunSpecMpptModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecMpptModel *parent);
    ~SunSpecMpptModelRepeatingBlock() override = default;

    SunSpecMpptModel *parentModel() const;

    QString name() const override;
    /* Input ID */
    quint16 inputId() const;
    /* Input ID Sting */
    QString inputIdSting() const;
    /* DC Current [A] */
    float dcCurrent() const;
    /* DC Voltage [V] */
    float dcVoltage() const;
    /* DC Power [W] */
    float dcPower() const;
    /* Lifetime Energy [Wh] */
    quint32 lifetimeEnergy() const;
    /* Timestamp [Secs] */
    quint32 timestamp() const;
    /* Temperature [C] */
    qint16 temperature() const;
    /* Operating State */
    Dcst operatingState() const;
    /* Module Events */
    DcevtFlags moduleEvents() const;

    void processBlockData() override;

protected:
    void initDataPoints() override;

private:
    SunSpecMpptModel *m_parentModel = nullptr;

    quint16 m_inputId = 0;
    QString m_inputIdSting;
    float m_dcCurrent = 0;
    float m_dcVoltage = 0;
    float m_dcPower = 0;
    quint32 m_lifetimeEnergy = 0;
    quint32 m_timestamp = 0;
    qint16 m_temperature = 0;
    Dcst m_operatingState;
    DcevtFlags m_moduleEvents;

};



class SunSpecMpptModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Evt {
        EvtGroundFault = 0x1,
        EvtInputOverVoltage = 0x2,
        EvtReserved2 = 0x4,
        EvtDcDisconnect = 0x8,
        EvtReserved4 = 0x10,
        EvtCabinetOpen = 0x20,
        EvtManualShutdown = 0x40,
        EvtOverTemp = 0x80,
        EvtReserved8 = 0x100,
        EvtReserved9 = 0x200,
        EvtReserved10 = 0x400,
        EvtReserved11 = 0x800,
        EvtBlownFuse = 0x1000,
        EvtUnderTemp = 0x2000,
        EvtMemoryLoss = 0x4000,
        EvtArcDetection = 0x8000,
        EvtReserved16 = 0x10000,
        EvtReserved17 = 0x20000,
        EvtReserved18 = 0x40000,
        EvtReserved19 = 0x80000,
        EvtTestFailed = 0x100000,
        EvtInputUnderVoltage = 0x200000,
        EvtInputOverCurrent = 0x400000
    };
    Q_DECLARE_FLAGS(EvtFlags, Evt)
    Q_FLAG(Evt)

    explicit SunSpecMpptModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecMpptModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    /* Current Scale Factor */
    qint16 currentScaleFactor() const;
    /* Voltage Scale Factor */
    qint16 voltageScaleFactor() const;
    /* Power Scale Factor */
    qint16 powerScaleFactor() const;
    /* Energy Scale Factor */
    qint16 energyScaleFactor() const;
    /* Global Events */
    EvtFlags globalEvents() const;
    /* Number of Modules */
    int numberOfModules() const;
    /* Timestamp Period */
    quint16 timestampPeriod() const;

protected:
    quint16 m_fixedBlockLength = 8;
    quint16 m_repeatingBlockLength = 20;

    void initDataPoints();
    void processBlockData() override;

private:

    void setupRepeatingBlocks();

    qint16 m_currentScaleFactor = 0;
    qint16 m_voltageScaleFactor = 0;
    qint16 m_powerScaleFactor = 0;
    qint16 m_energyScaleFactor = 0;
    EvtFlags m_globalEvents;
    int m_numberOfModules = 0;
    quint16 m_timestampPeriod = 0;


};

QDebug operator<<(QDebug debug, SunSpecMpptModel *model);

#endif // SUNSPECMPPTMODEL_H
