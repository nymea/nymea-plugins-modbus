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

#include "sunspec.h"
#include "sunspecmodel.h"

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

    explicit SunSpecReactiveCurrentModel(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent = nullptr);
    ~SunSpecReactiveCurrentModel() override; 

    QString name() const override;
    QString description() const override;
    QString label() const override;

    quint16 modelId() const;
    quint16 modelLength() const;
    Argramod arGraMod() const;
    float arGraSag() const;
    float arGraSwell() const;
    ModenaFlags modEna() const;
    quint16 filTms() const;
    float dbVMin() const;
    float dbVMax() const;
    float blkZnV() const;
    float hysBlkZnV() const;
    quint16 blkZnTmms() const;
    quint16 holdTmms() const;
    quint16 pad() const;

private:
    quint16 m_modelId;
    quint16 m_modelLength;
    Argramod m_arGraMod;
    float m_arGraSag;
    float m_arGraSwell;
    ModenaFlags m_modEna;
    quint16 m_filTms;
    float m_dbVMin;
    float m_dbVMax;
    float m_blkZnV;
    float m_hysBlkZnV;
    quint16 m_blkZnTmms;
    quint16 m_holdTmms;
    qint16 m_arGraSf;
    qint16 m_vRefPctSf;
    quint16 m_pad;

    void initDataPoints();

};

#endif // SUNSPECREACTIVECURRENTMODEL_H
