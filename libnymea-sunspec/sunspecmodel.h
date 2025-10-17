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

#ifndef SUNSPECMODEL_H
#define SUNSPECMODEL_H

#include <QTimer>
#include <QObject>
#include <QLoggingCategory>
#include <QModbusReply>

#include "sunspecdatapoint.h"

Q_DECLARE_LOGGING_CATEGORY(dcSunSpecModelData)

class SunSpecConnection;
class SunSpecModelRepeatingBlock;

class SunSpecModel : public QObject
{
    Q_OBJECT

    friend class SunSpecConnection;

public:
    enum ModelBlockType {
        ModelBlockTypeFixed,
        ModelBlockTypeRepeating,
        ModelBlockTypeFixedAndRepeating
    };
    Q_ENUM(ModelBlockType)

    typedef struct CommonModelInfo {
        QString manufacturerName;
        QString modelName;
        QString serialNumber;
        QString versionString;
    } CommonModelInfo;

    explicit SunSpecModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelId, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent = nullptr);
    virtual ~SunSpecModel() = default;

    SunSpecConnection *connection() const;

    bool initialized() const;

    // Model information
    virtual QString name() const = 0;
    virtual QString description() const = 0;
    virtual QString label() const = 0;

    quint16 modelId() const;
    quint16 modelLength() const;
    quint16 modbusStartRegister() const;

    SunSpecDataPoint::ByteOrder byteOrder() const;
    QHash<QString, SunSpecDataPoint> dataPoints() const;

    QVector<quint16> blockData() const;

    CommonModelInfo commonModelInfo() const;

    QList<SunSpecModelRepeatingBlock *> repeatingBlocks() const;

    virtual void init();
    virtual void readBlockData();

    bool operator==(const SunSpecModel &other) const;

protected:
    SunSpecConnection *m_connection = nullptr;
    QTimer m_initTimer;

    quint16 m_modbusStartRegister = 0;
    quint16 m_modelId;
    quint16 m_modelLength = 0;

    ModelBlockType m_modelBlockType = ModelBlockTypeFixed;
    SunSpecDataPoint::ByteOrder m_byteOrder = SunSpecDataPoint::ByteOrderLittleEndian;

    bool m_initialized = false;

    QVector<quint16> m_blockData;
    QHash<QString, SunSpecDataPoint> m_dataPoints;

    CommonModelInfo m_commonModelInfo;

    QList<SunSpecModelRepeatingBlock *> m_repeatingBlocks;

    void setInitializedFinished();

    virtual void processBlockData() = 0;

signals:
    void initFinished(bool success);
    void blockDataChanged(const QVector<quint16> blockData);

    void blockUpdated();

};

QDebug operator<<(QDebug debug, SunSpecModel *model);

#endif // SUNSPECMODEL_H
