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

#include "sunspecmodel.h"
#include "sunspecconnection.h"
#include "sunspecmodelrepeatingblock.h"

Q_LOGGING_CATEGORY(dcSunSpecModelData, "SunSpecModelData")

SunSpecModel::SunSpecModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelId, quint16 modelLength, SunSpecDataPoint::ByteOrder byteOrder, QObject *parent) :
    QObject(parent),
    m_connection(connection),
    m_modbusStartRegister(modbusStartRegister),
    m_modelId(modelId),
    m_modelLength(modelLength),
    m_byteOrder(byteOrder)
{
    m_initTimer.setSingleShot(true);
    m_initTimer.setInterval(10000);
    connect(&m_initTimer, &QTimer::timeout, this, [this](){
        if (!m_initialized) {
            emit initFinished(false);
        }
    });
}

SunSpecConnection *SunSpecModel::connection() const
{
    return m_connection;
}

bool SunSpecModel::initialized() const
{
    return m_initialized;
}

quint16 SunSpecModel::modelId() const
{
    return m_modelId;
}

quint16 SunSpecModel::modelLength() const
{
    return m_modelLength;
}

quint16 SunSpecModel::modbusStartRegister() const
{
    return m_modbusStartRegister;
}

SunSpecDataPoint::ByteOrder SunSpecModel::byteOrder() const
{
    return m_byteOrder;
}

QHash<QString, SunSpecDataPoint> SunSpecModel::dataPoints() const
{
    return m_dataPoints;
}

QVector<quint16> SunSpecModel::blockData() const
{
    return m_blockData;
}

SunSpecModel::CommonModelInfo SunSpecModel::commonModelInfo() const
{
    return m_commonModelInfo;
}

QList<SunSpecModelRepeatingBlock *> SunSpecModel::repeatingBlocks() const
{
    return m_repeatingBlocks;
}

void SunSpecModel::init()
{
    m_initialized = false;
    readBlockData();
    m_initTimer.start();
}

void SunSpecModel::readBlockData()
{
    qCDebug(dcSunSpecModelData())
            << "Start reading SunSpec model data (start address:"
            << m_modbusStartRegister
            << ", length:"
            << m_modelLength + 2
            << ")";
    m_partialBlockData.clear();
    m_lastStartAddress = 0;
    m_lastReadSize = 0;
    readNextBlockDataPart();
}

bool SunSpecModel::operator ==(const SunSpecModel &other) const
{
    return m_connection == other.connection() &&
            m_modbusStartRegister == other.modbusStartRegister() &&
            m_modelId == other.modelId() &&
            m_modelLength == other.modelLength();
}

void SunSpecModel::setInitializedFinished()
{
    if (!m_initialized) {
        m_initTimer.stop();
        m_initialized = true;
        emit initFinished(true);
    }
}

void SunSpecModel::readNextBlockDataPart()
{
    const auto startAddress = m_modbusStartRegister + m_partialBlockData.size();
    // Model header has length 2.
    const auto readSize = static_cast<quint16>(std::min(125, m_modelLength + 2 - m_partialBlockData.size()));

    if (startAddress == m_lastStartAddress && readSize == m_lastReadSize) {
        qCWarning(dcSunSpecModelData()) << "Last read yielded no data. Not trying to read the same data again.";
        return;
    }

    qCDebug(dcSunSpecModelData())
            << "Trying to read data block (start address:"
            << startAddress
            << ", length:"
            << readSize
            << ")";
    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters,
                                              startAddress,
                                              readSize);
    QModbusReply *reply = m_connection->sendReadRequest(request, m_connection->slaveId());
    if (!reply) {
        qCDebug(dcSunSpecModelData()) << "Read data block error: " << m_connection->modbusTcpClient()->errorString();
        return;
    }

    if (reply->isFinished()) {
        qCWarning(dcSunSpecModelData()) << "Read data block error: " << m_connection->modbusTcpClient()->errorString();
        reply->deleteLater(); // broadcast replies return immediately
        return;
    }

    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);
    connect(reply, &QModbusReply::finished, this, [=]() {
        if (reply->error() != QModbusDevice::NoError) {
            qCWarning(dcSunSpec()) << name() << description() << "Read data block response error:" << reply->error();
            return;
        }

        const QModbusDataUnit unit = reply->result();
        qCDebug(dcSunSpecModelData()) << "-->" << "Received data block" << this << unit.values().count() << SunSpecDataPoint::registersToString(unit.values());
        m_partialBlockData.append(unit.values());
        if (m_partialBlockData.size() < m_modelLength + 2) {
            readNextBlockDataPart();
        } else { // finished reading
            handleNewBlockData();
        }
    });

    connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error) {
        qCWarning(dcSunSpecModelData())  << name() << description() << "Modbus reply while reading data block. Error:" << error << reply->errorString();
    });
}

void SunSpecModel::handleNewBlockData()
{
    m_blockData = m_partialBlockData;
    emit blockDataChanged(m_blockData);

    // Fill the data points
    foreach (const QString &dataPointName, m_dataPoints.keys()) {
        QVector<quint16> rawData = m_blockData.mid(m_dataPoints[dataPointName].addressOffset(), m_dataPoints[dataPointName].size());
        m_dataPoints[dataPointName].setRawData(rawData);
        qCDebug(dcSunSpecModelData()) << "Set raw data:" << m_dataPoints[dataPointName] << SunSpecDataPoint::registersToString(rawData) << (m_dataPoints[dataPointName].isValid() ? "Valid" : "Invalid");
    }

    // Fill the private member data using the data points
    processBlockData();

    // Handle repeating blocks
    if (!m_repeatingBlocks.isEmpty()) {
        auto repeatingBlocksTotalSize = 0;
        foreach (SunSpecModelRepeatingBlock *block, m_repeatingBlocks) {
            repeatingBlocksTotalSize += block->blockSize();
        }
        const auto repeatingBlocksDataOffset = m_blockData.size() - repeatingBlocksTotalSize;
        foreach (SunSpecModelRepeatingBlock *block, m_repeatingBlocks) {
            qCDebug(dcSunSpecModelData()) << "Block" << block->blockIndex();
            // Fill the data points
            const auto blockOffset = repeatingBlocksDataOffset + block->blockIndex() * block->blockSize();
            foreach (const QString &dataPointName, block->m_dataPoints.keys()) {
                const auto dataPointOffset = blockOffset + block->m_dataPoints[dataPointName].addressOffset();
                QVector<quint16> rawData = m_blockData.mid(dataPointOffset, block->m_dataPoints[dataPointName].size());
                block->m_dataPoints[dataPointName].setRawData(rawData);
                qCDebug(dcSunSpecModelData())
                        << "Set raw data (offset:"
                        << dataPointOffset
                        << ", size:"
                        << block->m_dataPoints[dataPointName].size()
                        << "):"
                        << block->m_dataPoints[dataPointName]
                        << SunSpecDataPoint::registersToString(rawData)
                        << (block->m_dataPoints[dataPointName].isValid() ? "Valid" : "Invalid");
            }

            // Fill the private member data using the data points
            block->processBlockData();
        }
    }

    // Make sure initialized gets called
    setInitializedFinished();

    // Inform about the new block data
    emit blockUpdated();
}

QDebug operator<<(QDebug debug, SunSpecModel *model)
{
    debug.nospace().noquote() << "SunSpecModel(Model: " << model->modelId() << ", " << model->name() << ", " << model->label() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")";
    return debug.space().quote();
}
