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

#ifndef SUNSPECLITHIUMIONMODULEMODEL_H
#define SUNSPECLITHIUMIONMODULEMODEL_H

#include <QObject>

#include "sunspec.h"
#include "sunspecmodel.h"

class SunSpecLithiumIonModuleModel : public SunSpecModel
{
    Q_OBJECT
public:

    explicit SunSpecLithiumIonModuleModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecLithiumIonModuleModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    quint16 modelId() const;
    quint16 modelLength() const;
    quint16 stringIndex() const;
    quint16 moduleIndex() const;
    quint16 moduleCellCount() const;
    float moduleSoC() const;
    float depthOfDischarge() const;
    float moduleSoH() const;
    quint32 cycleCount() const;
    float moduleVoltage() const;
    float maxCellVoltage() const;
    quint16 maxCellVoltageCell() const;
    float minCellVoltage() const;
    quint16 minCellVoltageCell() const;
    float averageCellVoltage() const;
    qint16 maxCellTemperature() const;
    quint16 maxCellTemperatureCell() const;
    qint16 minCellTemperature() const;
    quint16 minCellTemperatureCell() const;
    qint16 averageCellTemperature() const;
    quint16 balancedCellCount() const;
    QString serialNumber() const;

private:
    quint16 m_modelId;
    quint16 m_modelLength;
    quint16 m_stringIndex;
    quint16 m_moduleIndex;
    quint16 m_moduleCellCount;
    float m_moduleSoC;
    float m_depthOfDischarge;
    float m_moduleSoH;
    quint32 m_cycleCount;
    float m_moduleVoltage;
    float m_maxCellVoltage;
    quint16 m_maxCellVoltageCell;
    float m_minCellVoltage;
    quint16 m_minCellVoltageCell;
    float m_averageCellVoltage;
    qint16 m_maxCellTemperature;
    quint16 m_maxCellTemperatureCell;
    qint16 m_minCellTemperature;
    quint16 m_minCellTemperatureCell;
    qint16 m_averageCellTemperature;
    quint16 m_balancedCellCount;
    QString m_serialNumber;
    qint16 m_soC_SF;
    qint16 m_soH_SF;
    qint16 m_doD_SF;
    qint16 m_v_SF;
    qint16 m_cellV_SF;
    qint16 m_tmp_SF;

    void initDataPoints();

};

#endif // SUNSPECLITHIUMIONMODULEMODEL_H
