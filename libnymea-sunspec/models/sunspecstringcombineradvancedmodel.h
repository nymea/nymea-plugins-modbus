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

#ifndef SUNSPECSTRINGCOMBINERADVANCEDMODEL_H
#define SUNSPECSTRINGCOMBINERADVANCEDMODEL_H

#include <QObject>

#include "sunspecmodel.h"
#include "sunspecmodelrepeatingblock.h"

class SunSpecConnection;
class SunSpecStringCombinerAdvancedModel;

class SunSpecStringCombinerAdvancedModelRepeatingBlock : public SunSpecModelRepeatingBlock
{
    Q_OBJECT
public:

    enum Inevt {
        InevtLowVoltage = 0x1,
        InevtLowPower = 0x2,
        InevtLowEfficiency = 0x4,
        InevtCurrent = 0x8,
        InevtVoltage = 0x10,
        InevtPower = 0x20,
        InevtPr = 0x40,
        InevtDisconnected = 0x80,
        InevtFuseFault = 0x100,
        InevtCombinerFuseFault = 0x200,
        InevtCombinerCabinetOpen = 0x400,
        InevtTemp = 0x800,
        InevtGroundfault = 0x1000,
        InevtReversedPolarity = 0x2000,
        InevtIncompatible = 0x4000,
        InevtCommError = 0x8000,
        InevtInternalError = 0x10000,
        InevtTheft = 0x20000,
        InevtArcDetected = 0x40000
    };
    Q_DECLARE_FLAGS(InevtFlags, Inevt)
    Q_FLAG(Inevt)

    explicit SunSpecStringCombinerAdvancedModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecStringCombinerAdvancedModel *parent);
    ~SunSpecStringCombinerAdvancedModelRepeatingBlock() override = default;

    SunSpecStringCombinerAdvancedModel *parentModel() const;

    QString name() const override;
    /* Uniquely identifies this input set */
    quint16 id() const;
    /* String Input Event Flags */
    InevtFlags inputEvent() const;
    /* String Input Vendor Event Flags */
    quint32 inputEventVendor() const;
    /* String Input Current [A] */
    float amps() const;
    /* String Input Amp-Hours [Ah] */
    quint32 ampHours() const;
    /* String Input Voltage [V] */
    float voltage() const;
    /* String Input Power [W] */
    float watts() const;
    /* String Input Energy [Wh] */
    quint32 wattHours() const;
    /* String Performance Ratio [Pct] */
    quint16 pr() const;
    /* Number of modules in this input string */
    quint16 n() const;

    void processBlockData() override;

protected:
    void initDataPoints() override;

private:
    SunSpecStringCombinerAdvancedModel *m_parentModel = nullptr;

    quint16 m_id = 0;
    InevtFlags m_inputEvent;
    quint32 m_inputEventVendor = 0;
    float m_amps = 0;
    quint32 m_ampHours = 0;
    float m_voltage = 0;
    float m_watts = 0;
    quint32 m_wattHours = 0;
    quint16 m_pr = 0;
    quint16 m_n = 0;

};



class SunSpecStringCombinerAdvancedModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Evt {
        EvtLowVoltage = 0x1,
        EvtLowPower = 0x2,
        EvtLowEfficiency = 0x4,
        EvtCurrent = 0x8,
        EvtVoltage = 0x10,
        EvtPower = 0x20,
        EvtPr = 0x40,
        EvtDisconnected = 0x80,
        EvtFuseFault = 0x100,
        EvtCombinerFuseFault = 0x200,
        EvtCombinerCabinetOpen = 0x400,
        EvtTemp = 0x800,
        EvtGroundfault = 0x1000,
        EvtReversedPolarity = 0x2000,
        EvtIncompatible = 0x4000,
        EvtCommError = 0x8000,
        EvtInternalError = 0x10000,
        EvtTheft = 0x20000,
        EvtArcDetected = 0x40000
    };
    Q_DECLARE_FLAGS(EvtFlags, Evt)
    Q_FLAG(Evt)

    explicit SunSpecStringCombinerAdvancedModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecStringCombinerAdvancedModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    /* Current scale factor */
    qint16 dCA_SF() const;
    /* Amp-hour scale factor */
    qint16 dCAhr_SF() const;
    /* Voltage scale factor */
    qint16 dCV_SF() const;
    /* Power scale factor */
    qint16 dCW_SF() const;
    /* Energy scale factor */
    qint16 dCWh_SF() const;
    /* Maximum DC Current Rating [A] */
    float rating() const;
    /* Number of Inputs */
    int n() const;
    /* Bitmask value.  Events */
    EvtFlags eventFlags() const;
    /* Bitmask value.  Vendor defined events */
    quint32 vendorEvent() const;
    /* Total measured current [A] */
    float amps() const;
    /* Total metered Amp-hours [Ah] */
    quint32 ampHours() const;
    /* Output Voltage [V] */
    float voltage() const;
    /* Internal operating temperature [C] */
    qint16 temp() const;
    /* Output power [W] */
    float watts() const;
    /* DC Performance ratio value [Pct] */
    qint16 pr() const;
    /* Output energy [Wh] */
    quint32 wattHours() const;
    /* Current scale factor for inputs */
    qint16 inDCA_SF() const;
    /* Amp-hour scale factor for inputs */
    qint16 inDCAhr_SF() const;
    /* Voltage scale factor for inputs */
    qint16 inDCV_SF() const;
    /* Power scale factor for inputs */
    qint16 inDCW_SF() const;
    /* Energy scale factor for inputs */
    qint16 inDCWh_SF() const;

protected:
    quint16 m_fixedBlockLength = 25;
    quint16 m_repeatingBlockLength = 14;

    void initDataPoints();
    void processBlockData() override;

private:

    void setupRepeatingBlocks();

    qint16 m_dCA_SF = 0;
    qint16 m_dCAhr_SF = 0;
    qint16 m_dCV_SF = 0;
    qint16 m_dCW_SF = 0;
    qint16 m_dCWh_SF = 0;
    float m_rating = 0;
    int m_n = 0;
    EvtFlags m_eventFlags;
    quint32 m_vendorEvent = 0;
    float m_amps = 0;
    quint32 m_ampHours = 0;
    float m_voltage = 0;
    qint16 m_temp = 0;
    float m_watts = 0;
    qint16 m_pr = 0;
    quint32 m_wattHours = 0;
    qint16 m_inDCA_SF = 0;
    qint16 m_inDCAhr_SF = 0;
    qint16 m_inDCV_SF = 0;
    qint16 m_inDCW_SF = 0;
    qint16 m_inDCWh_SF = 0;


};

QDebug operator<<(QDebug debug, SunSpecStringCombinerAdvancedModel *model);

#endif // SUNSPECSTRINGCOMBINERADVANCEDMODEL_H
