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

#ifndef SUNSPECFLOWBATTERYSTACKMODEL_H
#define SUNSPECFLOWBATTERYSTACKMODEL_H

#include <QObject>

#include "sunspecmodel.h"
#include "sunspecmodelrepeatingblock.h"

class SunSpecConnection;
class SunSpecFlowBatteryStackModel;

class SunSpecFlowBatteryStackModelRepeatingBlock : public SunSpecModelRepeatingBlock
{
    Q_OBJECT
public:

    explicit SunSpecFlowBatteryStackModelRepeatingBlock(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, SunSpecFlowBatteryStackModel *parent = nullptr);
    ~SunSpecFlowBatteryStackModelRepeatingBlock() override = default;

    SunSpecFlowBatteryStackModel *parentModel() const;

    QString name() const override;
    quint16 cellPointsToBeDetermined() const;

    void processBlockData(const QVector<quint16> blockData) override;

protected:
    void initDataPoints();

private:
    SunSpecFlowBatteryStackModel *m_parentModel = nullptr;

    quint16 m_cellPointsToBeDetermined = 0;

};



class SunSpecFlowBatteryStackModel : public SunSpecModel
{
    Q_OBJECT
public:

    explicit SunSpecFlowBatteryStackModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, QObject *parent = nullptr);
    ~SunSpecFlowBatteryStackModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    quint16 stackPointsToBeDetermined() const;

protected:
    quint16 m_fixedBlockLength = 1;
    quint16 m_repeatingBlockLength = 1;

    void initDataPoints();
    void processBlockData() override;

private:
    quint16 m_stackPointsToBeDetermined = 0;


};

QDebug operator<<(QDebug debug, SunSpecFlowBatteryStackModel *model);

#endif // SUNSPECFLOWBATTERYSTACKMODEL_H
