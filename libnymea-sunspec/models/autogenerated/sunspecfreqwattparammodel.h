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

#include "sunspec.h"
#include "sunspecmodel.h"

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

    explicit SunSpecFreqWattParamModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecFreqWattParamModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    quint16 modelId() const;
    quint16 modelLength() const;
    float wGra() const;
    qint16 hzStr() const;
    qint16 hzStop() const;
    HysenaFlags hysEna() const;
    ModenaFlags modEna() const;
    float hzStopWGra() const;
    quint16 pad() const;

private:
    quint16 m_modelId;
    quint16 m_modelLength;
    float m_wGra;
    qint16 m_hzStr;
    qint16 m_hzStop;
    HysenaFlags m_hysEna;
    ModenaFlags m_modEna;
    float m_hzStopWGra;
    qint16 m_wGraSf;
    qint16 m_hzStrStopSf;
    qint16 m_rmpIncDecSf;
    quint16 m_pad;

    void initDataPoints();

};

#endif // SUNSPECFREQWATTPARAMMODEL_H
