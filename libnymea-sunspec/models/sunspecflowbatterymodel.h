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

#ifndef SUNSPECFLOWBATTERYMODEL_H
#define SUNSPECFLOWBATTERYMODEL_H

#include <QObject>

#include "sunspecmodel.h"
#include "sunspecmodelrepeatingblock.h"

class SunSpecConnection;
class SunSpecFlowBatteryModel;

class SunSpecFlowBatteryModelRepeatingBlock : public SunSpecModelRepeatingBlock
{
    Q_OBJECT
public:

    explicit SunSpecFlowBatteryModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecFlowBatteryModel *parent);
    ~SunSpecFlowBatteryModelRepeatingBlock() override = default;

    SunSpecFlowBatteryModel *parentModel() const;

    QString name() const override;
    /* Battery String Points To Be Determined */
    quint16 batteryStringPointsToBeDetermined() const;

    void processBlockData() override;

protected:
    void initDataPoints() override;

private:
    SunSpecFlowBatteryModel *m_parentModel = nullptr;

    quint16 m_batteryStringPointsToBeDetermined = 0;

};



class SunSpecFlowBatteryModel : public SunSpecModel
{
    Q_OBJECT
public:

    explicit SunSpecFlowBatteryModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecFlowBatteryModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    /* Battery Points To Be Determined */
    quint16 batteryPointsToBeDetermined() const;

protected:
    quint16 m_fixedBlockLength = 1;
    quint16 m_repeatingBlockLength = 1;

    void initDataPoints();
    void processBlockData() override;

private:

    void setupRepeatingBlocks();

    quint16 m_batteryPointsToBeDetermined = 0;


};

QDebug operator<<(QDebug debug, SunSpecFlowBatteryModel *model);

#endif // SUNSPECFLOWBATTERYMODEL_H
