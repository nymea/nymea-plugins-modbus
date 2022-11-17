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

#ifndef SUNSPECEXTSETTINGSMODEL_H
#define SUNSPECEXTSETTINGSMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecExtSettingsModel : public SunSpecModel
{
    Q_OBJECT
public:

    explicit SunSpecExtSettingsModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, QObject *parent = nullptr);
    ~SunSpecExtSettingsModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;


    float rampUpRate() const;
    QModbusReply *setRampUpRate(float rampUpRate);

    float nomRmpDnRte() const;
    QModbusReply *setNomRmpDnRte(float nomRmpDnRte);

    float emergencyRampUpRate() const;
    QModbusReply *setEmergencyRampUpRate(float emergencyRampUpRate);

    float emergencyRampDownRate() const;
    QModbusReply *setEmergencyRampDownRate(float emergencyRampDownRate);

    float connectRampUpRate() const;
    QModbusReply *setConnectRampUpRate(float connectRampUpRate);

    float connectRampDownRate() const;
    QModbusReply *setConnectRampDownRate(float connectRampDownRate);

    float defaultRampRate() const;
    QModbusReply *setDefaultRampRate(float defaultRampRate);

    qint16 rampRateScaleFactor() const;

protected:
    quint16 m_fixedBlockLength = 8;

    void initDataPoints() override;
    void processBlockData() override;

private:
    float m_rampUpRate = 0;
    float m_nomRmpDnRte = 0;
    float m_emergencyRampUpRate = 0;
    float m_emergencyRampDownRate = 0;
    float m_connectRampUpRate = 0;
    float m_connectRampDownRate = 0;
    float m_defaultRampRate = 0;
    qint16 m_rampRateScaleFactor = 0;


};

QDebug operator<<(QDebug debug, SunSpecExtSettingsModel *model);

#endif // SUNSPECEXTSETTINGSMODEL_H
