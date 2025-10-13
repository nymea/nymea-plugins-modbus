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

    explicit SunSpecReactiveCurrentModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    ~SunSpecReactiveCurrentModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;


    /* Indicates if gradients trend toward zero at the edges of the deadband or trend toward zero at the center of the deadband. */
    Argramod arGraMod() const;
    QModbusReply *setArGraMod(Argramod arGraMod);

    /* The gradient used to increase capacitive dynamic current. A value of 0 indicates no additional reactive current support. [%ARtg/%dV] */
    float arGraSag() const;
    QModbusReply *setArGraSag(float arGraSag);

    /* The gradient used to increase inductive dynamic current.  A value of 0 indicates no additional reactive current support. [%ARtg/%dV] */
    float arGraSwell() const;
    QModbusReply *setArGraSwell(float arGraSwell);

    /* Activate dynamic reactive current model */
    ModenaFlags modEna() const;
    QModbusReply *setModEna(ModenaFlags modEna);

    /* The time window used to calculate the moving average voltage. [Secs] */
    quint16 filTms() const;
    QModbusReply *setFilTms(quint16 filTms);

    /* The lower delta voltage limit for which negative voltage deviations less than this value no dynamic vars are produced. [% VRef] */
    float dbVMin() const;
    QModbusReply *setDbVMin(float dbVMin);

    /* The upper delta voltage limit for which positive voltage deviations less than this value no dynamic current produced. [% VRef] */
    float dbVMax() const;
    QModbusReply *setDbVMax(float dbVMax);

    /* Block zone voltage which defines a lower voltage boundary below which no dynamic current is produced. [% VRef] */
    float blkZnV() const;
    QModbusReply *setBlkZnV(float blkZnV);

    /* Hysteresis voltage used with BlkZnV. [% VRef] */
    float hysBlkZnV() const;
    QModbusReply *setHysBlkZnV(float hysBlkZnV);

    /* Block zone time the time before which reactive current support remains active regardless of how low the voltage drops. [mSecs] */
    quint16 blkZnTmms() const;
    QModbusReply *setBlkZnTmms(quint16 blkZnTmms);

    /* Hold time during which reactive current support continues after the average voltage has entered the dead zone. [mSecs] */
    quint16 holdTmms() const;
    QModbusReply *setHoldTmms(quint16 holdTmms);

    /* Scale factor for the gradients. */
    qint16 arGraSf() const;
    /* Scale factor for the voltage zone and limit settings. */
    qint16 vRefPctSf() const;
    quint16 pad() const;

protected:
    quint16 m_fixedBlockLength = 14;

    void initDataPoints();
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


};

QDebug operator<<(QDebug debug, SunSpecReactiveCurrentModel *model);

#endif // SUNSPECREACTIVECURRENTMODEL_H
