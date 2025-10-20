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

#ifndef SUNSPECAGGREGATORMODEL_H
#define SUNSPECAGGREGATORMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecAggregatorModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum St {
        StOff = 1,
        StOn = 2,
        StFull = 3,
        StFault = 4
    };
    Q_ENUM(St)

    enum Ctl {
        CtlNone = 0,
        CtlAutomatic = 1,
        CtlForceOff = 2,
        CtlTest = 3,
        CtlThrottle = 4
    };
    Q_ENUM(Ctl)

    enum Evt {
        EvtGroundFault = 0x1,
        EvtInputOverVoltage = 0x2,
        EvtReserved2 = 0x4,
        EvtDcDisconnect = 0x8,
        EvtReserved4 = 0x10,
        EvtReserved5 = 0x20,
        EvtManualShutdown = 0x40,
        EvtOverTemperature = 0x80,
        EvtReserved8 = 0x100,
        EvtReserved9 = 0x200,
        EvtReserved10 = 0x400,
        EvtReserved11 = 0x800,
        EvtBlownFuse = 0x1000,
        EvtUnderTemperature = 0x2000,
        EvtMemoryLoss = 0x4000,
        EvtArcDetection = 0x8000,
        EvtTheftDetection = 0x10000,
        EvtOutputOverCurrent = 0x20000,
        EvtOutputOverVoltage = 0x40000,
        EvtOutputUnderVoltage = 0x80000,
        EvtTestFailed = 0x100000
    };
    Q_DECLARE_FLAGS(EvtFlags, Evt)
    Q_FLAG(Evt)

    explicit SunSpecAggregatorModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecAggregatorModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    /* Aggregated model id */
    quint16 aid() const;
    /* Number of aggregated models */
    quint16 n() const;
    /* Update Number.  Incrementing number each time the mapping is changed.  If the number is not changed from the last reading the direct access to a specific offset will result in reading the same logical model as before.  Otherwise the entire model must be read to refresh the changes */
    quint16 un() const;
    /* Enumerated status code */
    St status() const;
    /* Vendor specific status code */
    quint16 vendorStatus() const;
    /* Bitmask event code */
    EvtFlags eventCode() const;
    /* Vendor specific event code */
    quint32 vendorEventCode() const;
    /* Control register for all aggregated devices */
    Ctl control() const;
    /* Vendor control register for all aggregated devices */
    quint32 vendorControl() const;
    /* Numerical value used as a parameter to the control */
    quint32 controlValue() const;

protected:
    quint16 m_fixedBlockLength = 14;

    void initDataPoints();
    void processBlockData() override;

private:
    quint16 m_aid = 0;
    quint16 m_n = 0;
    quint16 m_un = 0;
    St m_status;
    quint16 m_vendorStatus = 0;
    EvtFlags m_eventCode;
    quint32 m_vendorEventCode = 0;
    Ctl m_control;
    quint32 m_vendorControl = 0;
    quint32 m_controlValue = 0;


};

QDebug operator<<(QDebug debug, SunSpecAggregatorModel *model);

#endif // SUNSPECAGGREGATORMODEL_H
