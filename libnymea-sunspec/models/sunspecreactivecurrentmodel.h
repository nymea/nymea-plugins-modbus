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

#ifndef SUNSPECREACTIVECURRENTMODEL_H
#define SUNSPECREACTIVECURRENTMODEL_H

#include <QObject>

#include "sunspecmodel.h"

class SunSpecConnection;

class SunSpecReactiveCurrentModel : public SunSpecModel
{
    Q_OBJECT
public:

    enum Argramod {
        ArgramodEdge = 0,
        ArgramodCenter = 1
    };
    Q_ENUM(Argramod)

    enum Modena {
        ModenaEnabled = 0x1
    };
    Q_DECLARE_FLAGS(ModenaFlags, Modena)
    Q_FLAG(Modena)

    explicit SunSpecReactiveCurrentModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 length, QObject *parent = nullptr);
    ~SunSpecReactiveCurrentModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;


    Argramod arGraMod() const;
    QModbusReply *setArGraMod(Argramod arGraMod);

    float arGraSag() const;
    QModbusReply *setArGraSag(float arGraSag);

    float arGraSwell() const;
    QModbusReply *setArGraSwell(float arGraSwell);

    ModenaFlags modEna() const;
    QModbusReply *setModEna(ModenaFlags modEna);

    quint16 filTms() const;
    QModbusReply *setFilTms(quint16 filTms);

    float dbVMin() const;
    QModbusReply *setDbVMin(float dbVMin);

    float dbVMax() const;
    QModbusReply *setDbVMax(float dbVMax);

    float blkZnV() const;
    QModbusReply *setBlkZnV(float blkZnV);

    float hysBlkZnV() const;
    QModbusReply *setHysBlkZnV(float hysBlkZnV);

    quint16 blkZnTmms() const;
    QModbusReply *setBlkZnTmms(quint16 blkZnTmms);

    quint16 holdTmms() const;
    QModbusReply *setHoldTmms(quint16 holdTmms);

    quint16 pad() const;

protected:
    void processBlockData() override;

private:
    Argramod m_arGraMod;
    float m_arGraSag = 0;
    float m_arGraSwell = 0;
    ModenaFlags m_modEna;
    quint16 m_filTms = 0;
    float m_dbVMin = 0;
    float m_dbVMax = 0;
    float m_blkZnV = 0;
    float m_hysBlkZnV = 0;
    quint16 m_blkZnTmms = 0;
    quint16 m_holdTmms = 0;
    qint16 m_arGraSf = 0;
    qint16 m_vRefPctSf = 0;
    quint16 m_pad = 0;

    void initDataPoints();

};

#endif // SUNSPECREACTIVECURRENTMODEL_H
