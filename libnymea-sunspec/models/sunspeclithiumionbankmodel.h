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

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecLithiumIonBankModel : public SunSpecModel
{
    Q_OBJECT
public:

    explicit SunSpecLithiumIonBankModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent = nullptr);
    ~SunSpecLithiumIonBankModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    quint16 stringCount() const;
    quint16 connectedStringCount() const;
    float maxModuleTemperature() const;
    quint16 maxModuleTemperatureString() const;
    quint16 maxModuleTemperatureModule() const;
    float minModuleTemperature() const;
    quint16 minModuleTemperatureString() const;
    quint16 minModuleTemperatureModule() const;
    qint16 averageModuleTemperature() const;
    float maxStringVoltage() const;
    quint16 maxStringVoltageString() const;
    float minStringVoltage() const;
    quint16 minStringVoltageString() const;
    float averageStringVoltage() const;
    float maxStringCurrent() const;
    quint16 maxStringCurrentString() const;
    float minStringCurrent() const;
    quint16 minStringCurrentString() const;
    float averageStringCurrent() const;
    quint16 batteryCellBalancingCount() const;

protected:
    void processBlockData() override;

private:
    quint16 m_stringCount = 0;
    quint16 m_connectedStringCount = 0;
    float m_maxModuleTemperature = 0;
    quint16 m_maxModuleTemperatureString = 0;
    quint16 m_maxModuleTemperatureModule = 0;
    float m_minModuleTemperature = 0;
    quint16 m_minModuleTemperatureString = 0;
    quint16 m_minModuleTemperatureModule = 0;
    qint16 m_averageModuleTemperature = 0;
    float m_maxStringVoltage = 0;
    quint16 m_maxStringVoltageString = 0;
    float m_minStringVoltage = 0;
    quint16 m_minStringVoltageString = 0;
    float m_averageStringVoltage = 0;
    float m_maxStringCurrent = 0;
    quint16 m_maxStringCurrentString = 0;
    float m_minStringCurrent = 0;
    quint16 m_minStringCurrentString = 0;
    float m_averageStringCurrent = 0;
    quint16 m_batteryCellBalancingCount = 0;
    qint16 m_cellV_SF = 0;
    qint16 m_modTmp_SF = 0;
    qint16 m_a_SF = 0;
    qint16 m_soH_SF = 0;
    qint16 m_soC_SF = 0;
    qint16 m_v_SF = 0;

    void initDataPoints();

};

#endif // SUNSPECLITHIUMIONBANKMODEL_H
