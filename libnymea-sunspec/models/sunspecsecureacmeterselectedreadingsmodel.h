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

#ifndef SUNSPECSECUREACMETERSELECTEDREADINGSMODEL_H
#define SUNSPECSECUREACMETERSELECTEDREADINGSMODEL_H

#include <QObject>

#include "sunspecmodel.h"
#include "sunspecmodelrepeatingblock.h"

class SunSpecConnection;
class SunSpecSecureAcMeterSelectedReadingsModel;

class SunSpecSecureAcMeterSelectedReadingsModelRepeatingBlock : public SunSpecModelRepeatingBlock
{
    Q_OBJECT
public:

    explicit SunSpecSecureAcMeterSelectedReadingsModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecSecureAcMeterSelectedReadingsModel *parent);
    ~SunSpecSecureAcMeterSelectedReadingsModelRepeatingBlock() override = default;

    SunSpecSecureAcMeterSelectedReadingsModel *parentModel() const;

    QString name() const override;
    quint16 dS() const;

    void processBlockData() override;

protected:
    void initDataPoints() override;

private:
    SunSpecSecureAcMeterSelectedReadingsModel *m_parentModel = nullptr;

    quint16 m_dS = 0;

};



class SunSpecSecureAcMeterSelectedReadingsModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Alg {
        AlgNone = 0,
        AlgAesGmac64 = 1,
        AlgEcc256 = 2
    };
    Q_ENUM(Alg)

    enum Evt {
        EvtPowerFailure = 0x4,
        EvtUnderVoltage = 0x8,
        EvtLowPf = 0x10,
        EvtOverCurrent = 0x20,
        EvtOverVoltage = 0x40,
        EvtMissingSensor = 0x80,
        EvtOem01 = 0x10000,
        EvtOem02 = 0x20000,
        EvtOem03 = 0x40000,
        EvtOem04 = 0x80000,
        EvtOem05 = 0x100000,
        EvtOem06 = 0x200000,
        EvtOem07 = 0x400000,
        EvtOem08 = 0x800000,
        EvtOem09 = 0x1000000,
        EvtOem10 = 0x2000000,
        EvtOem11 = 0x4000000,
        EvtOem12 = 0x8000000,
        EvtOem13 = 0x10000000,
        EvtOem14 = 0x20000000,
        EvtOem15 = 0x40000000
    };
    Q_DECLARE_FLAGS(EvtFlags, Evt)
    Q_FLAG(Evt)

    explicit SunSpecSecureAcMeterSelectedReadingsModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecSecureAcMeterSelectedReadingsModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    /* Total AC Current [A] */
    float amps() const;
    /* Current scale factor */
    qint16 a_SF() const;
    /* Average phase or line voltage [V] */
    float voltage() const;
    /* Voltage scale factor */
    qint16 v_SF() const;
    /* Frequency [Hz] */
    float hz() const;
    /* Frequency scale factor */
    qint16 hz_SF() const;
    /* Total Real Power [W] */
    float watts() const;
    /* Real Power scale factor */
    qint16 w_SF() const;
    /* AC Apparent Power [VA] */
    float va() const;
    /* Apparent Power scale factor */
    qint16 vA_SF() const;
    /* Reactive Power [var] */
    float var() const;
    /* Reactive Power scale factor */
    qint16 vAR_SF() const;
    /* Power Factor [Pct] */
    float pf() const;
    /* Power Factor scale factor */
    qint16 pF_SF() const;
    /* Total Real Energy Exported [Wh] */
    quint32 totalWattHoursExported() const;
    /* Total Real Energy Imported [Wh] */
    quint32 totalWattHoursImported() const;
    /* Real Energy scale factor */
    qint16 totWh_SF() const;
    /* Total Apparent Energy Exported [VAh] */
    quint32 totalVaHoursExported() const;
    /* Total Apparent Energy Imported [VAh] */
    quint32 totalVaHoursImported() const;
    /* Apparent Energy scale factor */
    qint16 totVAh_SF() const;
    /* Total Reactive Energy Imported Quadrant 1 [varh] */
    quint32 totalVarHoursImportedQ1() const;
    /* Total Reactive Power Imported Quadrant 2 [varh] */
    quint32 totalVArHoursImportedQ2() const;
    /* Total Reactive Power Exported Quadrant 3 [varh] */
    quint32 totalVArHoursExportedQ3() const;
    /* Total Reactive Power Exported Quadrant 4 [varh] */
    quint32 totalVArHoursExportedQ4() const;
    /* Reactive Energy scale factor */
    qint16 totVArh_SF() const;
    /* Meter Event Flags */
    EvtFlags events() const;
    quint16 rsrvd() const;
    /* Timestamp value is the number of seconds since January 1, 2000 */
    quint32 timestamp() const;
    /* Millisecond counter 0-999 */
    quint16 milliseconds() const;
    /* Sequence number of request */
    quint16 sequence() const;
    /* Algorithm used to compute the digital signature */
    Alg algorithm() const;
    /* Number of registers comprising the digital signature. */
    quint16 n() const;

protected:
    quint16 m_fixedBlockLength = 42;
    quint16 m_repeatingBlockLength = 1;

    void initDataPoints();
    void processBlockData() override;

private:

    void setupRepeatingBlocks();

    float m_amps = 0;
    qint16 m_a_SF = 0;
    float m_voltage = 0;
    qint16 m_v_SF = 0;
    float m_hz = 0;
    qint16 m_hz_SF = 0;
    float m_watts = 0;
    qint16 m_w_SF = 0;
    float m_va = 0;
    qint16 m_vA_SF = 0;
    float m_var = 0;
    qint16 m_vAR_SF = 0;
    float m_pf = 0;
    qint16 m_pF_SF = 0;
    quint32 m_totalWattHoursExported = 0;
    quint32 m_totalWattHoursImported = 0;
    qint16 m_totWh_SF = 0;
    quint32 m_totalVaHoursExported = 0;
    quint32 m_totalVaHoursImported = 0;
    qint16 m_totVAh_SF = 0;
    quint32 m_totalVarHoursImportedQ1 = 0;
    quint32 m_totalVArHoursImportedQ2 = 0;
    quint32 m_totalVArHoursExportedQ3 = 0;
    quint32 m_totalVArHoursExportedQ4 = 0;
    qint16 m_totVArh_SF = 0;
    EvtFlags m_events;
    quint16 m_rsrvd = 0;
    quint32 m_timestamp = 0;
    quint16 m_milliseconds = 0;
    quint16 m_sequence = 0;
    Alg m_algorithm;
    quint16 m_n = 0;


};

QDebug operator<<(QDebug debug, SunSpecSecureAcMeterSelectedReadingsModel *model);

#endif // SUNSPECSECUREACMETERSELECTEDREADINGSMODEL_H
