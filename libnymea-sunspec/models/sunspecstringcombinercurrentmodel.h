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

#ifndef SUNSPECSTRINGCOMBINERCURRENTMODEL_H
#define SUNSPECSTRINGCOMBINERCURRENTMODEL_H

#include <QObject>

#include "sunspecmodel.h"
#include "sunspecmodelrepeatingblock.h"

class SunSpecConnection;
class SunSpecStringCombinerCurrentModel;

class SunSpecStringCombinerCurrentModelRepeatingBlock : public SunSpecModelRepeatingBlock
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

    explicit SunSpecStringCombinerCurrentModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecStringCombinerCurrentModel *parent = nullptr);
    ~SunSpecStringCombinerCurrentModelRepeatingBlock() override = default;

    SunSpecStringCombinerCurrentModel *parentModel() const;

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

    void processBlockData(const QVector<quint16> blockData) override;

protected:
    void initDataPoints() override;

private:
    SunSpecStringCombinerCurrentModel *m_parentModel = nullptr;

    quint16 m_id = 0;
    InevtFlags m_inputEvent;
    quint32 m_inputEventVendor = 0;
    float m_amps = 0;
    quint32 m_ampHours = 0;

};



class SunSpecStringCombinerCurrentModel : public SunSpecModel
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

    explicit SunSpecStringCombinerCurrentModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecStringCombinerCurrentModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    /* Current scale factor */
    qint16 dCA_SF() const;
    /* Amp-hour scale factor */
    qint16 dCAhr_SF() const;
    /* Voltage scale factor */
    qint16 dCV_SF() const;
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
    /* Current scale factor for inputs */
    qint16 inDCA_SF() const;
    /* Amp-hour scale factor for inputs */
    qint16 inDCAhr_SF() const;

protected:
    quint16 m_fixedBlockLength = 16;
    quint16 m_repeatingBlockLength = 8;

    void initDataPoints();
    void processBlockData() override;

private:
    qint16 m_dCA_SF = 0;
    qint16 m_dCAhr_SF = 0;
    qint16 m_dCV_SF = 0;
    float m_rating = 0;
    int m_n = 0;
    EvtFlags m_eventFlags;
    quint32 m_vendorEvent = 0;
    float m_amps = 0;
    quint32 m_ampHours = 0;
    float m_voltage = 0;
    qint16 m_temp = 0;
    qint16 m_inDCA_SF = 0;
    qint16 m_inDCAhr_SF = 0;


};

QDebug operator<<(QDebug debug, SunSpecStringCombinerCurrentModel *model);

#endif // SUNSPECSTRINGCOMBINERCURRENTMODEL_H
