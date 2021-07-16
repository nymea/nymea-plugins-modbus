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

#include "sunspecmodel.h"
#include "sunspecmodelrepeatingblock.h"

class SunSpecConnection;
class SunSpecLithiumIonModuleModel;

class SunSpecLithiumIonModuleModelRepeatingBlock : public SunSpecModelRepeatingBlock
{
    Q_OBJECT
public:

    enum Cellst {
        CellstCellIsBalancing = 0x1
    };
    Q_DECLARE_FLAGS(CellstFlags, Cellst)
    Q_FLAG(Cellst)

    explicit SunSpecLithiumIonModuleModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecLithiumIonModuleModel *parent = nullptr);
    ~SunSpecLithiumIonModuleModelRepeatingBlock() override = default;

    SunSpecLithiumIonModuleModel *parentModel() const;

    QString name() const override;
    float cellVoltage() const;
    float cellTemperature() const;
    CellstFlags cellStatus() const;

    void processBlockData(const QVector<quint16> blockData) override;

protected:
    void initDataPoints();

private:
    SunSpecLithiumIonModuleModel *m_parentModel = nullptr;

    float m_cellVoltage = 0;
    float m_cellTemperature = 0;
    CellstFlags m_cellStatus;

};



class SunSpecLithiumIonModuleModel : public SunSpecModel
{
    Q_OBJECT
public:

    explicit SunSpecLithiumIonModuleModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, QObject *parent = nullptr);
    ~SunSpecLithiumIonModuleModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

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
    float maxCellTemperature() const;
    quint16 maxCellTemperatureCell() const;
    float minCellTemperature() const;
    quint16 minCellTemperatureCell() const;
    float averageCellTemperature() const;
    quint16 balancedCellCount() const;
    QString serialNumber() const;
    qint16 soC_SF() const;
    qint16 soH_SF() const;
    qint16 doD_SF() const;
    qint16 v_SF() const;
    qint16 cellV_SF() const;
    qint16 tmp_SF() const;

protected:
    quint16 m_fixedBlockLength = 42;
    quint16 m_repeatingBlockLength = 4;

    void initDataPoints();
    void processBlockData() override;

private:
    quint16 m_stringIndex = 0;
    quint16 m_moduleIndex = 0;
    quint16 m_moduleCellCount = 0;
    float m_moduleSoC = 0;
    float m_depthOfDischarge = 0;
    float m_moduleSoH = 0;
    quint32 m_cycleCount = 0;
    float m_moduleVoltage = 0;
    float m_maxCellVoltage = 0;
    quint16 m_maxCellVoltageCell = 0;
    float m_minCellVoltage = 0;
    quint16 m_minCellVoltageCell = 0;
    float m_averageCellVoltage = 0;
    float m_maxCellTemperature = 0;
    quint16 m_maxCellTemperatureCell = 0;
    float m_minCellTemperature = 0;
    quint16 m_minCellTemperatureCell = 0;
    float m_averageCellTemperature = 0;
    quint16 m_balancedCellCount = 0;
    QString m_serialNumber;
    qint16 m_soC_SF = 0;
    qint16 m_soH_SF = 0;
    qint16 m_doD_SF = 0;
    qint16 m_v_SF = 0;
    qint16 m_cellV_SF = 0;
    qint16 m_tmp_SF = 0;


};

QDebug operator<<(QDebug debug, SunSpecLithiumIonModuleModel *model);

#endif // SUNSPECLITHIUMIONMODULEMODEL_H
