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

#ifndef SUNSPECFLOWBATTERYMODULEMODEL_H
#define SUNSPECFLOWBATTERYMODULEMODEL_H

#include <QObject>

#include "sunspecmodel.h"
#include "sunspecmodelrepeatingblock.h"

class SunSpecConnection;
class SunSpecFlowBatteryModuleModel;

class SunSpecFlowBatteryModuleModelRepeatingBlock : public SunSpecModelRepeatingBlock
{
    Q_OBJECT
public:

    explicit SunSpecFlowBatteryModuleModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecFlowBatteryModuleModel *parent);
    ~SunSpecFlowBatteryModuleModelRepeatingBlock() override = default;

    SunSpecFlowBatteryModuleModel *parentModel() const;

    QString name() const override;
    /* Stack Points To Be Determined */
    quint16 stackPointsToBeDetermined() const;

    void processBlockData() override;

protected:
    void initDataPoints() override;

private:
    SunSpecFlowBatteryModuleModel *m_parentModel = nullptr;

    quint16 m_stackPointsToBeDetermined = 0;

};



class SunSpecFlowBatteryModuleModel : public SunSpecModel
{
    Q_OBJECT
public:

    explicit SunSpecFlowBatteryModuleModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecFlowBatteryModuleModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    /* Module Points To Be Determined */
    quint16 modulePointsToBeDetermined() const;

protected:
    quint16 m_fixedBlockLength = 1;
    quint16 m_repeatingBlockLength = 1;

    void initDataPoints();
    void processBlockData() override;

private:

    void setupRepeatingBlocks();

    quint16 m_modulePointsToBeDetermined = 0;


};

QDebug operator<<(QDebug debug, SunSpecFlowBatteryModuleModel *model);

#endif // SUNSPECFLOWBATTERYMODULEMODEL_H
