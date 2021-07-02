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

#ifndef SUNSPECMPPTMODEL_H
#define SUNSPECMPPTMODEL_H

#include <QObject>

#include "sunspec.h"
#include "sunspecmodel.h"

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

    explicit SunSpecMpptModel(SunSpec *connection, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecMpptModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    EvtFlags globalEvents() const;
    int numberOfModules() const;
    quint16 timestampPeriod() const;

protected:
    void processBlockData() override;

private:
    qint16 m_currentScaleFactor;
    qint16 m_voltageScaleFactor;
    qint16 m_powerScaleFactor;
    qint16 m_energyScaleFactor;
    EvtFlags m_globalEvents;
    int m_numberOfModules;
    quint16 m_timestampPeriod;

    void initDataPoints();

};

#endif // SUNSPECMPPTMODEL_H
