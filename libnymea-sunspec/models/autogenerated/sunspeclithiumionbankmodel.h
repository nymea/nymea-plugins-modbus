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

#ifndef SUNSPECLITHIUMIONBANKMODEL_H
#define SUNSPECLITHIUMIONBANKMODEL_H

#include <QObject>

#include "sunspec.h"
#include "sunspecmodel.h"

class SunSpecLithiumIonBankModel : public SunSpecModel
{
    Q_OBJECT
public:

    explicit SunSpecLithiumIonBankModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecLithiumIonBankModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    quint16 modelId() const;
    quint16 modelLength() const;
    quint16 stringCount() const;
    quint16 connectedStringCount() const;
    qint16 maxModuleTemperature() const;
    quint16 maxModuleTemperatureString() const;
    quint16 maxModuleTemperatureModule() const;
    qint16 minModuleTemperature() const;
    quint16 minModuleTemperatureString() const;
    quint16 minModuleTemperatureModule() const;
    qint16 averageModuleTemperature() const;
    float maxStringVoltage() const;
    quint16 maxStringVoltageString() const;
    float minStringVoltage() const;
    quint16 minStringVoltageString() const;
    float averageStringVoltage() const;
    qint16 maxStringCurrent() const;
    quint16 maxStringCurrentString() const;
    qint16 minStringCurrent() const;
    quint16 minStringCurrentString() const;
    qint16 averageStringCurrent() const;
    quint16 batteryCellBalancingCount() const;

private:
    quint16 m_modelId;
    quint16 m_modelLength;
    quint16 m_stringCount;
    quint16 m_connectedStringCount;
    qint16 m_maxModuleTemperature;
    quint16 m_maxModuleTemperatureString;
    quint16 m_maxModuleTemperatureModule;
    qint16 m_minModuleTemperature;
    quint16 m_minModuleTemperatureString;
    quint16 m_minModuleTemperatureModule;
    qint16 m_averageModuleTemperature;
    float m_maxStringVoltage;
    quint16 m_maxStringVoltageString;
    float m_minStringVoltage;
    quint16 m_minStringVoltageString;
    float m_averageStringVoltage;
    qint16 m_maxStringCurrent;
    quint16 m_maxStringCurrentString;
    qint16 m_minStringCurrent;
    quint16 m_minStringCurrentString;
    qint16 m_averageStringCurrent;
    quint16 m_batteryCellBalancingCount;
    qint16 m_cellV_SF;
    qint16 m_modTmp_SF;
    qint16 m_a_SF;
    qint16 m_soH_SF;
    qint16 m_soC_SF;
    qint16 m_v_SF;

    void initDataPoints();

};

#endif // SUNSPECLITHIUMIONBANKMODEL_H
