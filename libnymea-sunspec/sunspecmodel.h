// SPDX-License-Identifier: LGPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of libnymea-sunspec.
*
* libnymea-sunspec is free software: you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* as published by the Free Software Foundation, either version 3
* of the License, or (at your option) any later version.
*
* libnymea-sunspec is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with libnymea-sunspec. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
