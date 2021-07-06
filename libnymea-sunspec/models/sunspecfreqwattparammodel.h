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

#ifndef SUNSPECFREQWATTPARAMMODEL_H
#define SUNSPECFREQWATTPARAMMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecFreqWattParamModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Hysena {
        HysenaEnabled = 0x1
    };
    Q_DECLARE_FLAGS(HysenaFlags, Hysena)
    Q_FLAG(Hysena)

    enum Modena {
        ModenaEnabled = 0x1
    };
    Q_DECLARE_FLAGS(ModenaFlags, Modena)
    Q_FLAG(Modena)

    explicit SunSpecFreqWattParamModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent = nullptr);
    ~SunSpecFreqWattParamModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;


    float wGra() const;
    QModbusReply *setWGra(float wGra);

    float hzStr() const;
    QModbusReply *setHzStr(float hzStr);

    float hzStop() const;
    QModbusReply *setHzStop(float hzStop);

    HysenaFlags hysEna() const;
    QModbusReply *setHysEna(HysenaFlags hysEna);

    ModenaFlags modEna() const;
    QModbusReply *setModEna(ModenaFlags modEna);

    float hzStopWGra() const;
    QModbusReply *setHzStopWGra(float hzStopWGra);

    quint16 pad() const;

protected:
    void processBlockData() override;

private:
    float m_wGra = 0;
    float m_hzStr = 0;
    float m_hzStop = 0;
    HysenaFlags m_hysEna;
    ModenaFlags m_modEna;
    float m_hzStopWGra = 0;
    qint16 m_wGraSf = 0;
    qint16 m_hzStrStopSf = 0;
    qint16 m_rmpIncDecSf = 0;
    quint16 m_pad = 0;

    void initDataPoints();

};

QDebug operator<<(QDebug debug, SunSpecFreqWattParamModel *model);

#endif // SUNSPECFREQWATTPARAMMODEL_H
