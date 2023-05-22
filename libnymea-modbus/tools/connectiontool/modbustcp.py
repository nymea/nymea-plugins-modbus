# Copyright (C) 2021 - 2023 nymea GmbH <developer@nymea.io>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

from .toolcommon import * 

##############################################################

def writePropertyGetSetMethodDeclarationsTcp(fileDescriptor, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)
        if 'unit' in registerDefinition and registerDefinition['unit'] != '':
            writeLine(fileDescriptor, '    /* %s [%s] - Address: %s, Size: %s */' % (registerDefinition['description'], registerDefinition['unit'], registerDefinition['address'], registerDefinition['size']))
        else:
            writeLine(fileDescriptor, '    /* %s - Address: %s, Size: %s */' % (registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))

        # Check if we require a read method
        if 'R' in registerDefinition['access']:
            writeLine(fileDescriptor, '    %s %s() const;' % (propertyTyp, propertyName))

        # Check if we require a write method
        if 'W' in registerDefinition['access']:
            writeLine(fileDescriptor, '    QModbusReply *set%s(%s %s);' % (propertyName[0].upper() + propertyName[1:], propertyTyp, propertyName))

        writeLine(fileDescriptor)


def writePropertyGetSetMethodImplementationsTcp(fileDescriptor, className, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)

        # Check if we require a read method
        if 'R' in registerDefinition['access']:
            if 'enum' in registerDefinition:
                writeLine(fileDescriptor, '%s::%s %s::%s() const' % (className, propertyTyp, className, propertyName))
            else:
                writeLine(fileDescriptor, '%s %s::%s() const' % (propertyTyp, className, propertyName))

            writeLine(fileDescriptor, '{')
            writeLine(fileDescriptor, '    return m_%s;' % propertyName)
            writeLine(fileDescriptor, '}')
            writeLine(fileDescriptor)

        # Check if we require a write method
        if 'W' in registerDefinition['access']:
            writeLine(fileDescriptor, 'QModbusReply *%s::set%s(%s %s)' % (className, propertyName[0].upper() + propertyName[1:], propertyTyp, propertyName))
            writeLine(fileDescriptor, '{')

            writeLine(fileDescriptor, '    QVector<quint16> values = %s;' % getConversionToValueMethod(registerDefinition))
            writeLine(fileDescriptor, '    qCDebug(dc%s()) << "--> Write \\"%s\\" register:" << %s << "size:" << %s << values;' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
            if registerDefinition['registerType'] == 'holdingRegister':
                writeLine(fileDescriptor, '    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, %s, values.count());' % (registerDefinition['address']))
            elif registerDefinition['registerType'] == 'coils':
                writeLine(fileDescriptor, '    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, %s, values.count());' % (registerDefinition['address']))
            else:
                logger.warning('Error: invalid register type for writing.')
                exit(1)

            writeLine(fileDescriptor, '    request.setValues(values);')
            writeLine(fileDescriptor, '    return m_modbusTcpMaster->sendWriteRequest(request, m_slaveId);')
            writeLine(fileDescriptor, '}')
            writeLine(fileDescriptor)

##############################################################

def writePropertyUpdateMethodImplementationsTcp(fileDescriptor, className, registerDefinitions, queuedRequests, queuedRequestsDelay):
    for registerDefinition in registerDefinitions:
        
        if 'access' in registerDefinition:
            if not 'R' in registerDefinition['access']:
                continue


        propertyName = registerDefinition['id']
        writeLine(fileDescriptor, 'void %s::update%s()' % (className, propertyName[0].upper() + propertyName[1:]))
        writeLine(fileDescriptor, '{')
        writeLine(fileDescriptor, '    // Update registers from %s' % registerDefinition['description'])
        writeLine(fileDescriptor, '    qCDebug(dc%s()) << "--> Read \\"%s\\" register:" << %s << "size:" << %s;' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))

        if queuedRequests:
            if 'readSchedule' in registerDefinition and registerDefinition['readSchedule'] == 'init':
                writeLine(fileDescriptor, '    if (m_currentInitReply)')
                writeLine(fileDescriptor, '        return;')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    m_currentInitReply = read%s();' % (propertyName[0].upper() + propertyName[1:]))
                writeLine(fileDescriptor, '    if (!m_currentInitReply) {')
                writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Error occurred while reading \\"%s\\" init register from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();' % (className, registerDefinition['description']))
                writeLine(fileDescriptor, '        finishInitialization(false);')
                writeLine(fileDescriptor, '        return;')
                writeLine(fileDescriptor, '    }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    if (m_currentInitReply->isFinished()) {')
                writeLine(fileDescriptor, '        m_currentInitReply->deleteLater(); // Broadcast reply returns immediatly')
                writeLine(fileDescriptor, '        m_currentInitReply = nullptr;')
                writeLine(fileDescriptor, '        if (!verifyInitFinished())')
                writeLine(fileDescriptor, '            QTimer::singleShot(%s, this, &%s::sendNextQueuedInitRequest);' % (queuedRequestsDelay, className))
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        return;')
                writeLine(fileDescriptor, '    }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    connect(m_currentInitReply, &QModbusReply::finished, m_currentInitReply, &QModbusReply::deleteLater);')
                writeLine(fileDescriptor, '    connect(m_currentInitReply, &QModbusReply::finished, this, [this](){')
                writeLine(fileDescriptor, '        handleModbusError(m_currentInitReply->error());')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        if (m_currentInitReply->error() != QModbusDevice::NoError) {')
                writeLine(fileDescriptor, '            QModbusResponse response = m_currentInitReply->rawResult();')
                writeLine(fileDescriptor, '            if (m_currentInitReply->error() == QModbusDevice::ProtocolError && response.isException()) {')
                writeLine(fileDescriptor, '                qCWarning(dc%s()) << "Modbus reply error occurred while updating init \\"%s\\" registers" << m_currentInitReply->error() << m_currentInitReply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());' % (className, blockName))
                writeLine(fileDescriptor, '            } else {')
                writeLine(fileDescriptor, '                qCWarning(dc%s()) << "Modbus reply error occurred while updating init \\"%s\\" registers" << m_currentInitReply->error() << m_currentInitReply->errorString();' % (className, blockName))
                writeLine(fileDescriptor, '            }')
                writeLine(fileDescriptor, '            finishInitialization(false);')
                writeLine(fileDescriptor, '            m_currentInitReply = nullptr;')
                writeLine(fileDescriptor, '            return;')
                writeLine(fileDescriptor, '        }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        const QModbusDataUnit unit = m_currentInitReply->result();')
                writeLine(fileDescriptor, '        m_currentInitReply = nullptr;')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        qCDebug(dc%s()) << "<-- Response from \\"%s\\" register" << %s << "size:" << %s << unit.values();' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
                writeLine(fileDescriptor, '        if (unit.values().size() == %s) {' % (registerDefinition['size']))
                writeLine(fileDescriptor, '            process%sRegisterValues(unit.values());' % (propertyName[0].upper() + propertyName[1:]))
                writeLine(fileDescriptor, '        } else {')
                writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Reading from \\"%s\\" init registers" << %s << "size:" << %s << "returned different size than requested. Ignoring incomplete data" << unit.values();' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
                writeLine(fileDescriptor, '        }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        if (!verifyInitFinished())')
                writeLine(fileDescriptor, '            QTimer::singleShot(%s, this, &%s::sendNextQueuedInitRequest);' % (queuedRequestsDelay, className))
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    });')
                writeLine(fileDescriptor)
            else:
                writeLine(fileDescriptor, '    if (m_currentUpdateReply)')
                writeLine(fileDescriptor, '        return;')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    m_currentUpdateReply = read%s();' % (propertyName[0].upper() + propertyName[1:]))
                writeLine(fileDescriptor, '    if (!m_currentUpdateReply) {')
                writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Error occurred while reading \\"%s\\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();' % (className, registerDefinition['description']))
                writeLine(fileDescriptor, '        if (!verifyUpdateFinished())')
                writeLine(fileDescriptor, '            QTimer::singleShot(%s, this, &%s::sendNextQueuedRequest);' % (queuedRequestsDelay, className))
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        return;')
                writeLine(fileDescriptor, '    }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    if (m_currentUpdateReply->isFinished()) {')
                writeLine(fileDescriptor, '        m_currentUpdateReply->deleteLater(); // Broadcast reply returns immediatly')
                writeLine(fileDescriptor, '        m_currentUpdateReply = nullptr;')
                writeLine(fileDescriptor, '        if (!verifyUpdateFinished())')
                writeLine(fileDescriptor, '            QTimer::singleShot(%s, this, &%s::sendNextQueuedRequest);' % (queuedRequestsDelay, className))
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        return;')
                writeLine(fileDescriptor, '    }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    connect(m_currentUpdateReply, &QModbusReply::finished, this, [this](){')
                writeLine(fileDescriptor, '        handleModbusError(m_currentUpdateReply->error());')
                writeLine(fileDescriptor, '        if (m_currentUpdateReply->error() == QModbusDevice::NoError) {')
                writeLine(fileDescriptor, '            const QModbusDataUnit unit = m_currentUpdateReply->result();')
                writeLine(fileDescriptor, '            qCDebug(dc%s()) << "<-- Response from \\"%s\\" register" << %s << "size:" << %s << unit.values();' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
                writeLine(fileDescriptor, '            if (unit.values().size() == %s) {' % (registerDefinition['size']))
                writeLine(fileDescriptor, '                process%sRegisterValues(unit.values());' % (propertyName[0].upper() + propertyName[1:]))
                writeLine(fileDescriptor, '            } else {')
                writeLine(fileDescriptor, '                qCWarning(dc%s()) << "Reading from \\"%s\\" registers" << %s << "size:" << %s << "returned different size than requested. Ignoring incomplete data" << unit.values();' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
                writeLine(fileDescriptor, '            }')
                writeLine(fileDescriptor, '        }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        m_currentUpdateReply->deleteLater();')
                writeLine(fileDescriptor, '        m_currentUpdateReply = nullptr;')
                writeLine(fileDescriptor, '        if (!verifyUpdateFinished())')
                writeLine(fileDescriptor, '            QTimer::singleShot(%s, this, &%s::sendNextQueuedRequest);' % (queuedRequestsDelay, className))
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    });')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    connect(m_currentUpdateReply, &QModbusReply::errorOccurred, this, [this] (QModbusDevice::Error error){')
                writeLine(fileDescriptor, '        QModbusResponse response = m_currentUpdateReply->rawResult();')
                writeLine(fileDescriptor, '        if (m_currentUpdateReply->error() == QModbusDevice::ProtocolError && response.isException()) {')
                writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Modbus reply error occurred while updating \\"%s\\" registers from" << m_modbusTcpMaster->hostAddress().toString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());' % (className, registerDefinition['description']))
                writeLine(fileDescriptor, '        } else {')
                writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Modbus reply error occurred while updating \\"%s\\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << m_currentUpdateReply->errorString();' % (className, registerDefinition['description']))
                writeLine(fileDescriptor, '        }')
                writeLine(fileDescriptor, '    });')
        else:
            writeLine(fileDescriptor, '    QModbusReply *reply = read%s();' % (propertyName[0].upper() + propertyName[1:]))
            writeLine(fileDescriptor, '    if (!reply) {')
            writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Error occurred while reading \\"%s\\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();' % (className, registerDefinition['description']))
            writeLine(fileDescriptor, '        return;')
            writeLine(fileDescriptor, '    }')
            writeLine(fileDescriptor)
            writeLine(fileDescriptor, '    if (reply->isFinished()) {')
            writeLine(fileDescriptor, '        reply->deleteLater(); // Broadcast reply returns immediatly')
            writeLine(fileDescriptor, '        return;')
            writeLine(fileDescriptor, '    }')
            writeLine(fileDescriptor)
            writeLine(fileDescriptor, '    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);')
            writeLine(fileDescriptor, '    connect(reply, &QModbusReply::finished, this, [this, reply](){')
            writeLine(fileDescriptor, '        handleModbusError(reply->error());')
            writeLine(fileDescriptor, '        if (reply->error() == QModbusDevice::NoError) {')
            writeLine(fileDescriptor, '            const QModbusDataUnit unit = reply->result();')
            writeLine(fileDescriptor, '            qCDebug(dc%s()) << "<-- Response from \\"%s\\" register" << %s << "size:" << %s << unit.values();' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
            writeLine(fileDescriptor, '            if (unit.values().size() == %s) {' % (registerDefinition['size']))
            writeLine(fileDescriptor, '                process%sRegisterValues(unit.values());' % (propertyName[0].upper() + propertyName[1:]))
            writeLine(fileDescriptor, '            } else {')
            writeLine(fileDescriptor, '                qCWarning(dc%s()) << "Reading from \\"%s\\" registers" << %s << "size:" << %s << "returned different size than requested. Ignoring incomplete data" << unit.values();' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
            writeLine(fileDescriptor, '            }')
            writeLine(fileDescriptor, '        }')
            writeLine(fileDescriptor, '    });')
            writeLine(fileDescriptor)
            writeLine(fileDescriptor, '    connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){')
            writeLine(fileDescriptor, '        QModbusResponse response = reply->rawResult();')
            writeLine(fileDescriptor, '        if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {')
            writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Modbus reply error occurred while updating \\"%s\\" registers from" << m_modbusTcpMaster->hostAddress().toString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());' % (className, registerDefinition['description']))
            writeLine(fileDescriptor, '        } else {')
            writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Modbus reply error occurred while updating \\"%s\\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();' % (className, registerDefinition['description']))
            writeLine(fileDescriptor, '        }')
            writeLine(fileDescriptor, '    });')
 
        writeLine(fileDescriptor, '}')
        writeLine(fileDescriptor)

##############################################################

def writeBlockUpdateMethodImplementationsTcp(fileDescriptor, className, blockDefinitions, queuedRequests, queuedRequestsDelay):
    for blockDefinition in blockDefinitions:
        blockName = blockDefinition['id']
        blockRegisters = blockDefinition['registers']
        blockStartAddress = 0
        registerCount = 0
        blockSize = 0

        for i, blockRegister in enumerate(blockRegisters):
            if i == 0:
                blockStartAddress = blockRegister['address']

            registerCount += 1
            blockSize += blockRegister['size']

        writeLine(fileDescriptor, 'void %s::update%sBlock()' % (className, blockName[0].upper() + blockName[1:]))
        writeLine(fileDescriptor, '{')
        writeLine(fileDescriptor, '    // Update register block \"%s\"' % blockName)
        writeLine(fileDescriptor, '    qCDebug(dc%s()) << "--> Read block \\"%s\\" registers from:" << %s << "size:" << %s;' % (className, blockName, blockStartAddress, blockSize))

        if queuedRequests:
            
            if 'readSchedule' in blockDefinition and blockDefinition['readSchedule'] == 'init':

                writeLine(fileDescriptor, '    m_currentInitReply = readBlock%s();' % (blockName[0].upper() + blockName[1:]))
                writeLine(fileDescriptor, '    if (!m_currentInitReply) {')
                writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Error occurred while reading init block \\"%s\\" registers";' % (className, blockName))
                writeLine(fileDescriptor, '        finishInitialization(false);')
                writeLine(fileDescriptor, '        return;')
                writeLine(fileDescriptor, '    }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    if (m_currentInitReply->isFinished()) {')
                writeLine(fileDescriptor, '        m_currentInitReply->deleteLater(); // Broadcast reply returns immediatly')
                writeLine(fileDescriptor, '        m_currentInitReply = nullptr;')
                writeLine(fileDescriptor, '        if (!verifyInitFinished())')
                writeLine(fileDescriptor, '            QTimer::singleShot(%s, this, &%s::sendNextQueuedInitRequest);' % (queuedRequestsDelay, className))
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        return;')
                writeLine(fileDescriptor, '    }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    connect(m_currentInitReply, &QModbusReply::finished, m_currentInitReply, &QModbusReply::deleteLater);')
                writeLine(fileDescriptor, '    connect(m_currentInitReply, &QModbusReply::finished, this, [this](){')
                writeLine(fileDescriptor, '        handleModbusError(m_currentInitReply->error());')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        if (m_currentInitReply->error() != QModbusDevice::NoError) {')
                writeLine(fileDescriptor, '            QModbusResponse response = m_currentInitReply->rawResult();')
                writeLine(fileDescriptor, '            if (m_currentInitReply->error() == QModbusDevice::ProtocolError && response.isException()) {')
                writeLine(fileDescriptor, '                qCWarning(dc%s()) << "Modbus reply error occurred while updating init block \\"%s\\" registers" << m_currentInitReply->error() << m_currentInitReply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());' % (className, blockName))
                writeLine(fileDescriptor, '            } else {')
                writeLine(fileDescriptor, '                qCWarning(dc%s()) << "Modbus reply error occurred while updating init block \\"%s\\" registers" << m_currentInitReply->error() << m_currentInitReply->errorString();' % (className, blockName))
                writeLine(fileDescriptor, '            }')
                writeLine(fileDescriptor, '            m_currentInitReply = nullptr;')
                writeLine(fileDescriptor, '            finishInitialization(false);')
                writeLine(fileDescriptor, '            return;')
                writeLine(fileDescriptor, '        }') 
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        const QModbusDataUnit unit = m_currentInitReply->result();')
                writeLine(fileDescriptor, '        m_currentInitReply = nullptr;')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        const QVector<quint16> blockValues = unit.values();')
                writeLine(fileDescriptor, '        qCDebug(dc%s()) << "<-- Response from reading init block \\"%s\\" register" << %s << "size:" << %s << blockValues;' % (className, blockName, blockStartAddress, blockSize))
                writeLine(fileDescriptor, '        if (blockValues.size() == %s) {' % (blockSize))

                # Start parsing the registers using offsets
                offset = 0
                for i, blockRegister in enumerate(blockRegisters):
                    propertyName = blockRegister['id']
                    writeLine(fileDescriptor, '            process%sRegisterValues(blockValues.mid(%s, %s));' % (propertyName[0].upper() + propertyName[1:], offset, blockRegister['size']))
                    offset += blockRegister['size']

                writeLine(fileDescriptor, '        } else {')
                writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Reading from \\"%s\\" block registers" << %s << "size:" << %s << "returned different size than requested. Ignoring incomplete data" << blockValues;' % (className, blockName, blockStartAddress, blockSize))
                writeLine(fileDescriptor, '        }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        if (!verifyInitFinished())')
                writeLine(fileDescriptor, '            QTimer::singleShot(%s, this, &%s::sendNextQueuedInitRequest);' % (queuedRequestsDelay, className))
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    });')
                writeLine(fileDescriptor)
            else:
                writeLine(fileDescriptor, '    m_currentUpdateReply = readBlock%s();' % (blockName[0].upper() + blockName[1:]))
                writeLine(fileDescriptor, '    if (!m_currentUpdateReply) {')
                writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Error occurred while reading block \\"%s\\" registers";' % (className, blockName))
                writeLine(fileDescriptor, '        if (!verifyUpdateFinished())')
                writeLine(fileDescriptor, '            QTimer::singleShot(%s, this, &%s::sendNextQueuedRequest);' % (queuedRequestsDelay, className))
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        return;')
                writeLine(fileDescriptor, '    }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    if (m_currentUpdateReply->isFinished()) {')
                writeLine(fileDescriptor, '        m_currentUpdateReply->deleteLater(); // Broadcast reply returns immediatly')
                writeLine(fileDescriptor, '        m_currentUpdateReply = nullptr;')
                writeLine(fileDescriptor, '        if (!verifyUpdateFinished())')
                writeLine(fileDescriptor, '            QTimer::singleShot(%s, this, &%s::sendNextQueuedRequest);' % (queuedRequestsDelay, className))
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        return;')
                writeLine(fileDescriptor, '    }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    connect(m_currentUpdateReply, &QModbusReply::finished, this, [this](){')
                writeLine(fileDescriptor, '        handleModbusError(m_currentUpdateReply->error());')
                writeLine(fileDescriptor, '        if (m_currentUpdateReply->error() == QModbusDevice::NoError) {')
                writeLine(fileDescriptor, '            const QModbusDataUnit unit = m_currentUpdateReply->result();')
                writeLine(fileDescriptor, '            const QVector<quint16> blockValues = unit.values();')
                writeLine(fileDescriptor, '            qCDebug(dc%s()) << "<-- Response from reading block \\"%s\\" register" << %s << "size:" << %s << blockValues;' % (className, blockName, blockStartAddress, blockSize))
                writeLine(fileDescriptor, '            if (blockValues.size() == %s) {' % (blockSize))

                # Start parsing the registers using offsets
                offset = 0
                for i, blockRegister in enumerate(blockRegisters):
                    propertyName = blockRegister['id']
                    writeLine(fileDescriptor, '                process%sRegisterValues(blockValues.mid(%s, %s));' % (propertyName[0].upper() + propertyName[1:], offset, blockRegister['size']))
                    offset += blockRegister['size']

                writeLine(fileDescriptor, '            } else {')
                writeLine(fileDescriptor, '                qCWarning(dc%s()) << "Reading from \\"%s\\" block registers" << %s << "size:" << %s << "returned different size than requested. Ignoring incomplete data" << blockValues;' % (className, blockName, blockStartAddress, blockSize))
                writeLine(fileDescriptor, '            }')
                writeLine(fileDescriptor, '        m_currentUpdateReply->deleteLater(); // Broadcast reply returns immediatly')
                writeLine(fileDescriptor, '        m_currentUpdateReply = nullptr;')
                writeLine(fileDescriptor, '        if (!verifyUpdateFinished())')
                writeLine(fileDescriptor, '            QTimer::singleShot(%s, this, &%s::sendNextQueuedRequest);' % (queuedRequestsDelay, className))
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        }')
                writeLine(fileDescriptor, '    });')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    connect(m_currentUpdateReply, &QModbusReply::errorOccurred, this, [this] (QModbusDevice::Error error){')
                writeLine(fileDescriptor, '        QModbusResponse response = m_currentUpdateReply->rawResult();')
                writeLine(fileDescriptor, '        if (m_currentUpdateReply->error() == QModbusDevice::ProtocolError && response.isException()) {')
                writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Modbus reply error occurred while updating block \\"%s\\" registers" << error << m_currentUpdateReply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());' % (className, blockName))
                writeLine(fileDescriptor, '        } else {')
                writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Modbus reply error occurred while updating block \\"%s\\" registers" << error << m_currentUpdateReply->errorString();' % (className, blockName))
                writeLine(fileDescriptor, '        }')
                writeLine(fileDescriptor, '    });')
        else:
            writeLine(fileDescriptor, '    QModbusReply *reply = readBlock%s();' % (blockName[0].upper() + blockName[1:]))
            writeLine(fileDescriptor, '    if (!reply) {')
            writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Error occurred while reading block \\"%s\\" registers";' % (className, blockName))
            writeLine(fileDescriptor, '        return;')
            writeLine(fileDescriptor, '    }')
            writeLine(fileDescriptor)
            writeLine(fileDescriptor, '    if (reply->isFinished()) {')
            writeLine(fileDescriptor, '        reply->deleteLater(); // Broadcast reply returns immediatly')
            writeLine(fileDescriptor, '        return;')
            writeLine(fileDescriptor, '    }')
            writeLine(fileDescriptor)
            writeLine(fileDescriptor, '    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);')
            writeLine(fileDescriptor, '    connect(reply, &QModbusReply::finished, this, [this, reply](){')
            writeLine(fileDescriptor, '        handleModbusError(reply->error());')
            writeLine(fileDescriptor, '        if (reply->error() == QModbusDevice::NoError) {')
            writeLine(fileDescriptor, '            const QModbusDataUnit unit = reply->result();')
            writeLine(fileDescriptor, '            const QVector<quint16> blockValues = unit.values();')
            writeLine(fileDescriptor, '            qCDebug(dc%s()) << "<-- Response from reading block \\"%s\\" register" << %s << "size:" << %s << blockValues;' % (className, blockName, blockStartAddress, blockSize))
            writeLine(fileDescriptor, '            if (blockValues.size() == %s) {' % (blockSize))

            # Start parsing the registers using offsets
            offset = 0
            for i, blockRegister in enumerate(blockRegisters):
                propertyName = blockRegister['id']
                writeLine(fileDescriptor, '                process%sRegisterValues(blockValues.mid(%s, %s));' % (propertyName[0].upper() + propertyName[1:], offset, blockRegister['size']))
                offset += blockRegister['size']

            writeLine(fileDescriptor, '            } else {')
            writeLine(fileDescriptor, '                qCWarning(dc%s()) << "Reading from \\"%s\\" block registers" << %s << "size:" << %s << "returned different size than requested. Ignoring incomplete data" << blockValues;' % (className, blockName, blockStartAddress, blockSize))
            writeLine(fileDescriptor, '            }')
            writeLine(fileDescriptor, '        }')
            writeLine(fileDescriptor, '    });')
            writeLine(fileDescriptor)
            writeLine(fileDescriptor, '    connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error){')
            writeLine(fileDescriptor, '        QModbusResponse response = reply->rawResult();')
            writeLine(fileDescriptor, '        if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {')
            writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Modbus reply error occurred while updating block \\"%s\\" registers" << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());' % (className, blockName))
            writeLine(fileDescriptor, '        } else {')
            writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Modbus reply error occurred while updating block \\"%s\\" registers" << error << reply->errorString();' % (className, blockName))
            writeLine(fileDescriptor, '        }')
            writeLine(fileDescriptor, '    });')

        writeLine(fileDescriptor, '}')
        writeLine(fileDescriptor)

##############################################################

def writeInternalPropertyReadMethodDeclarationsTcp(fileDescriptor, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        writeLine(fileDescriptor, '    QModbusReply *read%s();' % (propertyName[0].upper() + propertyName[1:]))

##############################################################

def writeInternalPropertyReadMethodImplementationsTcp(fileDescriptor, className, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        writeLine(fileDescriptor, 'QModbusReply *%s::read%s()' % (className, propertyName[0].upper() + propertyName[1:]))
        writeLine(fileDescriptor, '{')

        # Build request depending on the register type
        if registerDefinition['registerType'] == 'inputRegister':
            writeLine(fileDescriptor, '    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, %s, %s);' % (registerDefinition['address'], registerDefinition['size']))
        elif registerDefinition['registerType'] == 'discreteInputs':
            writeLine(fileDescriptor, '    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::DiscreteInputs, %s, %s);' % (registerDefinition['address'], registerDefinition['size']))
        elif registerDefinition['registerType'] == 'coils':
            writeLine(fileDescriptor, '    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, %s, %s);' % (registerDefinition['address'], registerDefinition['size']))
        else:
            #Default to holdingRegister
            writeLine(fileDescriptor, '    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, %s, %s);' % (registerDefinition['address'], registerDefinition['size']))

        writeLine(fileDescriptor, '    return m_modbusTcpMaster->sendReadRequest(request, m_slaveId);')
        writeLine(fileDescriptor, '}')
        writeLine(fileDescriptor)


##############################################################

def writeInternalBlockReadMethodDeclarationsTcp(fileDescriptor, blockDefinitions):
    for blockDefinition in blockDefinitions:
        blockName = blockDefinition['id']
        blockRegisters = blockDefinition['registers']
        blockStartAddress = 0
        registerCount = 0
        blockSize = 0
        registerType = ""

        for i, blockRegister in enumerate(blockRegisters):
            if i == 0:
                blockStartAddress = blockRegister['address']
                registerType = blockRegister['registerType']

            registerCount += 1
            blockSize += blockRegister['size']

        writeLine(fileDescriptor, '    /* Read block from start addess %s with size of %s registers containing following %s properties:' % (blockStartAddress, blockSize, registerCount))
        for i, registerDefinition in enumerate(blockRegisters):
            if 'unit' in registerDefinition and registerDefinition['unit'] != '':
                writeLine(fileDescriptor, '     - %s [%s] - Address: %s, Size: %s' % (registerDefinition['description'], registerDefinition['unit'], registerDefinition['address'], registerDefinition['size']))
            else:
                writeLine(fileDescriptor, '     - %s - Address: %s, Size: %s' % (registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
        writeLine(fileDescriptor, '    */' )
        writeLine(fileDescriptor, '    QModbusReply *readBlock%s();' % (blockName[0].upper() + blockName[1:]))
        writeLine(fileDescriptor)

##############################################################

def writeInternalBlockReadMethodImplementationsTcp(fileDescriptor, className, blockDefinitions):
    for blockDefinition in blockDefinitions:
        blockName = blockDefinition['id']
        blockRegisters = blockDefinition['registers']
        blockStartAddress = 0
        registerCount = 0
        blockSize = 0
        registerType = ""

        for i, blockRegister in enumerate(blockRegisters):
            if i == 0:
                blockStartAddress = blockRegister['address']
                registerType = blockRegister['registerType']

            registerCount += 1
            blockSize += blockRegister['size']


        writeLine(fileDescriptor, 'QModbusReply *%s::readBlock%s()' % (className, blockName[0].upper() + blockName[1:]))
        writeLine(fileDescriptor, '{')

        # Build request depending on the register type
        if registerType == 'inputRegister':
            writeLine(fileDescriptor, '    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::InputRegisters, %s, %s);' % (blockStartAddress, blockSize))
        elif registerType == 'discreteInputs':
            writeLine(fileDescriptor, '    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::DiscreteInputs, %s, %s);' % (blockStartAddress, blockSize))
        elif registerType == 'coils':
            writeLine(fileDescriptor, '    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, %s, %s);' % (blockStartAddress, blockSize))
        else:
            #Default to holdingRegister
            writeLine(fileDescriptor, '    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, %s, %s);' % (blockStartAddress, blockSize))

        writeLine(fileDescriptor, '    return m_modbusTcpMaster->sendReadRequest(request, m_slaveId);')

        writeLine(fileDescriptor, '}')
        writeLine(fileDescriptor)

##############################################################

def writeTestReachabilityImplementationsTcp(fileDescriptor, className, registerDefinitions, checkReachableRegister):

    propertyName = checkReachableRegister['id']

    writeLine(fileDescriptor, 'void %s::testReachability()' % (className))
    writeLine(fileDescriptor, '{')
    writeLine(fileDescriptor, '    if (m_checkRechableReply)')
    writeLine(fileDescriptor, '        return;')
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '    // Try to read the check reachability register %s in order to verify if the communication is working or not.' % checkReachableRegister['id'])
    writeLine(fileDescriptor, '    qCDebug(dc%s()) << "--> Test reachability by reading \\"%s\\" register:" << %s << "size:" << %s;' % (className, checkReachableRegister['description'], checkReachableRegister['address'], checkReachableRegister['size']))
    writeLine(fileDescriptor, '    m_checkRechableReply = read%s();' % (propertyName[0].upper() + propertyName[1:]))
    writeLine(fileDescriptor, '    if (!m_checkRechableReply) {')
    writeLine(fileDescriptor, '        qCDebug(dc%s()) << "Error occurred verifying reachability by reading \\"%s\\" register";' % (className, checkReachableRegister['description']))
    writeLine(fileDescriptor, '        onReachabilityCheckFailed();')
    writeLine(fileDescriptor, '        return;')
    writeLine(fileDescriptor, '    }')
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '    if (m_checkRechableReply->isFinished()) {')
    writeLine(fileDescriptor, '        m_checkRechableReply->deleteLater(); // Broadcast reply returns immediatly')
    writeLine(fileDescriptor, '        m_checkRechableReply = nullptr;')
    writeLine(fileDescriptor, '        onReachabilityCheckFailed();')
    writeLine(fileDescriptor, '        return;')
    writeLine(fileDescriptor, '    }')
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '    connect(m_checkRechableReply, &QModbusReply::finished, this, [this](){')
    writeLine(fileDescriptor, '        handleModbusError(m_checkRechableReply->error());')
    writeLine(fileDescriptor, '        if (m_checkRechableReply->error() != QModbusDevice::NoError)')
    writeLine(fileDescriptor, '            onReachabilityCheckFailed();')
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '        m_checkRechableReply->deleteLater();')
    writeLine(fileDescriptor, '        m_checkRechableReply = nullptr;')
    writeLine(fileDescriptor, '    });')
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '    connect(m_checkRechableReply, &QModbusReply::errorOccurred, this, [this] (QModbusDevice::Error error){')
    writeLine(fileDescriptor, '        QModbusResponse response = m_checkRechableReply->rawResult();')
    writeLine(fileDescriptor, '        if (m_checkRechableReply->error() == QModbusDevice::ProtocolError && response.isException()) {')
    writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Modbus reply error occurred while verifying reachability by reading \\"%s\\" register" << error << m_checkRechableReply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());' % (className, checkReachableRegister['description']))
    writeLine(fileDescriptor, '            // Note: if we get an exception on the reachability register, the modbus server is probably not ready')
    writeLine(fileDescriptor, '            // For some reasons on some devices the reply will never be finished on exception response. A reconnect might fix it.')
    writeLine(fileDescriptor, '            QTimer::singleShot(2000, m_modbusTcpMaster, &ModbusTcpMaster::reconnectDevice);')
    writeLine(fileDescriptor, '        } else {')
    writeLine(fileDescriptor, '            qCDebug(dc%s()) << "Modbus reply error occurred while verifying reachability by reading \\"%s\\" register" << error << m_checkRechableReply->errorString();' % (className, checkReachableRegister['description']))
    writeLine(fileDescriptor, '        }')
    writeLine(fileDescriptor, '    });')
    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)

##############################################################

def writeInitMethodImplementationTcp(fileDescriptor, className, registerDefinitions, blockDefinitions, queuedRequests):
    writeLine(fileDescriptor, 'bool %s::initialize()' % (className))
    writeLine(fileDescriptor, '{')
    writeLine(fileDescriptor, '    if (!m_reachable) {')
    writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Tried to initialize but the device is not to be reachable.";' % className)
    writeLine(fileDescriptor, '        return false;')
    writeLine(fileDescriptor, '    }')

    # First check if there are any init registers
    initRequired = False
    for registerDefinition in registerDefinitions:
        if 'readSchedule' in registerDefinition and registerDefinition['readSchedule'] == 'init':
            initRequired = True
            break

    for blockDefinition in blockDefinitions:
        if 'readSchedule' in blockDefinition and blockDefinition['readSchedule'] == 'init':
            initRequired = True
            break

    if initRequired:

        if queuedRequests:
            writeLine(fileDescriptor)
            writeLine(fileDescriptor, '    if (!m_modbusTcpMaster->connected()) {')
            writeLine(fileDescriptor, '        m_initRequestQueue.clear();')
            writeLine(fileDescriptor, '        return false;')
            writeLine(fileDescriptor, '    }')
            writeLine(fileDescriptor)

            # Read individual registers
            for registerDefinition in registerDefinitions:
                propertyName = registerDefinition['id']
                
                if 'readSchedule' in registerDefinition and registerDefinition['readSchedule'] == 'init':
                    writeLine(fileDescriptor, '    enqueueInitRequest(&%s::update%s);' % (className, propertyName[0].upper() + propertyName[1:]))

            # Read init blocks
            writeLine(fileDescriptor)
            for blockDefinition in blockDefinitions:
                blockName = blockDefinition['id']
                if 'readSchedule' in blockDefinition and blockDefinition['readSchedule'] == 'init':
                    writeLine(fileDescriptor, '    enqueueInitRequest(&%s::update%sBlock);' % (className, blockName[0].upper() + blockName[1:]))

            writeLine(fileDescriptor, '    sendNextQueuedInitRequest();');

        else:
            writeLine(fileDescriptor)
            writeLine(fileDescriptor, '    if (m_initObject) {')
            writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Tried to initialize but the init process is already running.";' % className)
            writeLine(fileDescriptor, '        return false;')
            writeLine(fileDescriptor, '    }')
            writeLine(fileDescriptor)
            writeLine(fileDescriptor, '    // Parent object for the init process')
            writeLine(fileDescriptor, '    m_initObject = new QObject(this);')
            writeLine(fileDescriptor)
            writeLine(fileDescriptor, '    QModbusReply *reply = nullptr;')

            # Read individual registers
            for registerDefinition in registerDefinitions:
                propertyName = registerDefinition['id']
                propertyTyp = getCppDataType(registerDefinition)

                if 'readSchedule' in registerDefinition and registerDefinition['readSchedule'] == 'init':
                    writeLine(fileDescriptor)
                    writeLine(fileDescriptor, '    // Read %s' % registerDefinition['description'])
                    writeLine(fileDescriptor, '    qCDebug(dc%s()) << "--> Read init \\"%s\\" register:" << %s << "size:" << %s;' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
                    writeLine(fileDescriptor, '    reply = read%s();' % (propertyName[0].upper() + propertyName[1:]))
                    writeLine(fileDescriptor, '    if (!reply) {')
                    writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Error occurred while reading \\"%s\\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();' % (className, registerDefinition['description']))
                    writeLine(fileDescriptor, '        finishInitialization(false);')
                    writeLine(fileDescriptor, '        return false;')
                    writeLine(fileDescriptor, '    }')
                    writeLine(fileDescriptor)
                    writeLine(fileDescriptor, '    if (reply->isFinished()) {')
                    writeLine(fileDescriptor, '        reply->deleteLater(); // Broadcast reply returns immediatly')
                    writeLine(fileDescriptor, '        return false;')
                    writeLine(fileDescriptor, '    }')
                    writeLine(fileDescriptor)
                    writeLine(fileDescriptor, '    m_pendingInitReplies.append(reply);')
                    writeLine(fileDescriptor, '    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);')
                    writeLine(fileDescriptor, '    connect(reply, &QModbusReply::finished, m_initObject, [this, reply](){')
                    writeLine(fileDescriptor, '        handleModbusError(reply->error());')
                    writeLine(fileDescriptor, '        m_pendingInitReplies.removeAll(reply);')
                    writeLine(fileDescriptor, '        if (reply->error() != QModbusDevice::NoError) {')
                    writeLine(fileDescriptor, '            finishInitialization(false);')
                    writeLine(fileDescriptor, '            return;')
                    writeLine(fileDescriptor, '        }')
                    writeLine(fileDescriptor)
                    writeLine(fileDescriptor, '        const QModbusDataUnit unit = reply->result();')
                    writeLine(fileDescriptor, '        qCDebug(dc%s()) << "<-- Response from init \\"%s\\" register" << %s << "size:" << %s << unit.values();' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
                    writeLine(fileDescriptor, '        if (unit.values().size() == %s) {' % (registerDefinition['size']))
                    writeLine(fileDescriptor, '            process%sRegisterValues(unit.values());' % (propertyName[0].upper() + propertyName[1:]))
                    writeLine(fileDescriptor, '        } else {')
                    writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Reading from \\"%s\\" registers" << %s << "size:" << %s << "returned different size than requested. Ignoring incomplete data" << unit.values();' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
                    writeLine(fileDescriptor, '        }')
                    writeLine(fileDescriptor, '        verifyInitFinished();')
                    writeLine(fileDescriptor, '    });')
                    writeLine(fileDescriptor)
                    writeLine(fileDescriptor, '    connect(reply, &QModbusReply::errorOccurred, m_initObject, [this, reply] (QModbusDevice::Error error){')
                    writeLine(fileDescriptor, '        QModbusResponse response = reply->rawResult();')
                    writeLine(fileDescriptor, '        if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {')
                    writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Modbus reply error occurred while reading \\"%s\\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());' % (className, registerDefinition['description']))
                    writeLine(fileDescriptor, '        } else {')
                    writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Modbus reply error occurred while reading \\"%s\\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();' % (className, registerDefinition['description']))
                    writeLine(fileDescriptor, '        }')
                    writeLine(fileDescriptor, '    });')

            # Read init blocks
            for blockDefinition in blockDefinitions:
                blockName = blockDefinition['id']
                blockRegisters = blockDefinition['registers']

                if 'readSchedule' in blockDefinition and blockDefinition['readSchedule'] == 'init':
                    blockStartAddress = 0
                    registerCount = 0
                    blockSize = 0
                    registerType = ""

                    for i, blockRegister in enumerate(blockRegisters):
                        if i == 0:
                            blockStartAddress = blockRegister['address']
                            registerType = blockRegister['registerType']

                        registerCount += 1
                        blockSize += blockRegister['size']

                    writeLine(fileDescriptor)
                    writeLine(fileDescriptor, '    // Read %s' % blockName)
                    writeLine(fileDescriptor, '    qCDebug(dc%s()) << "--> Read init block \\"%s\\" registers from:" << %s << "size:" << %s;' % (className, blockName, blockStartAddress, blockSize))
                    writeLine(fileDescriptor, '    reply = readBlock%s();' % (blockName[0].upper() + blockName[1:]))
                    writeLine(fileDescriptor, '    if (!reply) {')
                    writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Error occurred while reading block \\"%s\\" registers";' % (className, blockName))
                    writeLine(fileDescriptor, '        finishInitialization(false);')
                    writeLine(fileDescriptor, '        return false;')
                    writeLine(fileDescriptor, '    }')
                    writeLine(fileDescriptor)
                    writeLine(fileDescriptor, '    if (reply->isFinished()) {')
                    writeLine(fileDescriptor, '        reply->deleteLater(); // Broadcast reply returns immediatly')
                    writeLine(fileDescriptor, '        return false;')
                    writeLine(fileDescriptor, '    }')
                    writeLine(fileDescriptor)
                    writeLine(fileDescriptor, '    m_pendingInitReplies.append(reply);')
                    writeLine(fileDescriptor, '    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);')
                    writeLine(fileDescriptor, '    connect(reply, &QModbusReply::finished, m_initObject, [this, reply](){')
                    writeLine(fileDescriptor, '        m_pendingInitReplies.removeAll(reply);')
                    writeLine(fileDescriptor, '        handleModbusError(reply->error());')
                    writeLine(fileDescriptor, '        if (reply->error() != QModbusDevice::NoError) {')
                    writeLine(fileDescriptor, '            finishInitialization(false);')
                    writeLine(fileDescriptor, '            return;')
                    writeLine(fileDescriptor, '        }')
                    writeLine(fileDescriptor)
                    writeLine(fileDescriptor, '        const QModbusDataUnit unit = reply->result();')
                    writeLine(fileDescriptor, '        const QVector<quint16> blockValues = unit.values();')
                    writeLine(fileDescriptor, '        qCDebug(dc%s()) << "<-- Response from reading init block \\"%s\\" register" << %s << "size:" << %s << blockValues;' % (className, blockName, blockStartAddress, blockSize))
                    writeLine(fileDescriptor, '        if (blockValues.size() == %s) {' % (blockSize))

                    # Start parsing the registers using offsets
                    offset = 0
                    for i, blockRegister in enumerate(blockRegisters):
                        propertyName = blockRegister['id']
                        propertyTyp = getCppDataType(blockRegister)
                        writeLine(fileDescriptor, '            process%sRegisterValues(blockValues.mid(%s, %s));' % (propertyName[0].upper() + propertyName[1:], offset, blockRegister['size']))
                        offset += blockRegister['size']

                    writeLine(fileDescriptor, '        } else {')
                    writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Reading from \\"%s\\" block registers" << %s << "size:" << %s << "returned different size than requested. Ignoring incomplete data" << blockValues;' % (className, blockName, blockStartAddress, blockSize))
                    writeLine(fileDescriptor, '        }')
                    writeLine(fileDescriptor, '        verifyInitFinished();')
                    writeLine(fileDescriptor, '    });')
                    writeLine(fileDescriptor)
                    writeLine(fileDescriptor, '    connect(reply, &QModbusReply::errorOccurred, m_initObject, [reply] (QModbusDevice::Error error){')
                    writeLine(fileDescriptor, '        QModbusResponse response = reply->rawResult();')
                    writeLine(fileDescriptor, '        if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {')
                    writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Modbus reply error occurred while updating block \\"%s\\" registers" << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());' % (className, blockName))
                    writeLine(fileDescriptor, '        } else {')
                    writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Modbus reply error occurred while updating block \\"%s\\" registers" << error << reply->errorString();' % (className, blockName))
                    writeLine(fileDescriptor, '        }')
                    writeLine(fileDescriptor, '    });')
                    writeLine(fileDescriptor)

    else:
        writeLine(fileDescriptor, '    // No init registers defined. Nothing to be done and we are finished.')
        writeLine(fileDescriptor, '    emit initializationFinished(true);')

    writeLine(fileDescriptor, '    return true;')
    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)

##############################################################

def writeUpdateMethodTcp(fileDescriptor, className, registerDefinitions, blockDefinitions, queuedRequests):
    writeLine(fileDescriptor, 'bool %s::update()' % (className))
    writeLine(fileDescriptor, '{')

    # First check if there are any update registers
    updateRequired = False
    for registerDefinition in registerDefinitions:
        if registerDefinition['readSchedule'] == 'update':
            updateRequired = True
            break

    for blockDefinition in blockDefinitions:  
        if 'readSchedule' in blockDefinition and blockDefinition['readSchedule'] == 'update':
            updateRequired = True
            break

    if updateRequired:
        if queuedRequests:
            writeLine(fileDescriptor, '    if (!m_modbusTcpMaster->connected()) {')
            writeLine(fileDescriptor, '        m_updateRequestQueue.clear();')
            writeLine(fileDescriptor, '        return false;')
            writeLine(fileDescriptor, '    }')
            writeLine(fileDescriptor)

            writeLine(fileDescriptor, '    if (!m_updateRequestQueue.isEmpty()) {')
            writeLine(fileDescriptor, '        qCDebug(dc%s()) << "Tried to update but there are still some update requests pending. Waiting for them to be finished..." << m_updateRequestQueue.count();' % className)
            writeLine(fileDescriptor, '        return true;')
            writeLine(fileDescriptor, '    }')
            writeLine(fileDescriptor)

            # Read individual registers
            for registerDefinition in registerDefinitions:
                propertyName = registerDefinition['id']
                
                if 'readSchedule' in registerDefinition and registerDefinition['readSchedule'] == 'update':
                    writeLine(fileDescriptor, '    enqueueRequest(&%s::update%s);' % (className, propertyName[0].upper() + propertyName[1:]))

            # Read init blocks
            writeLine(fileDescriptor)
            for blockDefinition in blockDefinitions:
                blockName = blockDefinition['id']
                if 'readSchedule' in blockDefinition and blockDefinition['readSchedule'] == 'update':
                    writeLine(fileDescriptor, '    enqueueRequest(&%s::update%sBlock);' % (className, blockName[0].upper() + blockName[1:]))

            writeLine(fileDescriptor, '    sendNextQueuedRequest();');
        else:
            writeLine(fileDescriptor, '    if (!m_modbusTcpMaster->connected())')
            writeLine(fileDescriptor, '        return false;')
            writeLine(fileDescriptor)

            writeLine(fileDescriptor, '    if (!m_pendingUpdateReplies.isEmpty()) {')
            writeLine(fileDescriptor, '        qCDebug(dc%s()) << "Tried to update but there are still some update replies pending. Waiting for them to be finished...";' % className)
            writeLine(fileDescriptor, '        return true;')
            writeLine(fileDescriptor, '    }')
            writeLine(fileDescriptor)
            writeLine(fileDescriptor, '    QModbusReply *reply = nullptr;')

            # Read individual registers
            for registerDefinition in registerDefinitions:
                propertyName = registerDefinition['id']
                propertyTyp = getCppDataType(registerDefinition)

                if 'readSchedule' in registerDefinition and registerDefinition['readSchedule'] == 'update':
                    writeLine(fileDescriptor)
                    writeLine(fileDescriptor, '    // Read %s' % registerDefinition['description'])
                    writeLine(fileDescriptor, '    qCDebug(dc%s()) << "--> Read \\"%s\\" register:" << %s << "size:" << %s;' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
                    writeLine(fileDescriptor, '    reply = read%s();' % (propertyName[0].upper() + propertyName[1:]))
                    writeLine(fileDescriptor, '    if (!reply) {')
                    writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Error occurred while reading \\"%s\\" registers from" << m_modbusTcpMaster->hostAddress().toString() << m_modbusTcpMaster->errorString();' % (className, registerDefinition['description']))
                    writeLine(fileDescriptor, '        return false;')
                    writeLine(fileDescriptor, '    }')
                    writeLine(fileDescriptor)
                    writeLine(fileDescriptor, '    if (reply->isFinished()) {')
                    writeLine(fileDescriptor, '        reply->deleteLater(); // Broadcast reply returns immediatly')
                    writeLine(fileDescriptor, '        return false;')
                    writeLine(fileDescriptor, '    }')
                    writeLine(fileDescriptor)
                    writeLine(fileDescriptor, '    m_pendingUpdateReplies.append(reply);')
                    writeLine(fileDescriptor, '    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);')
                    writeLine(fileDescriptor, '    connect(reply, &QModbusReply::finished, this, [this, reply](){')
                    writeLine(fileDescriptor, '        m_pendingUpdateReplies.removeAll(reply);')
                    writeLine(fileDescriptor, '        handleModbusError(reply->error());')
                    writeLine(fileDescriptor, '        if (reply->error() != QModbusDevice::NoError) {')
                    writeLine(fileDescriptor, '            verifyUpdateFinished();')
                    writeLine(fileDescriptor, '            return;')
                    writeLine(fileDescriptor, '        }')
                    writeLine(fileDescriptor)
                    writeLine(fileDescriptor, '        const QModbusDataUnit unit = reply->result();')
                    writeLine(fileDescriptor, '        qCDebug(dc%s()) << "<-- Response from \\"%s\\" register" << %s << "size:" << %s << unit.values();' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
                    writeLine(fileDescriptor, '        if (unit.values().size() == %s) {' % (registerDefinition['size']))
                    writeLine(fileDescriptor, '            process%sRegisterValues(unit.values());' % (propertyName[0].upper() + propertyName[1:]))
                    writeLine(fileDescriptor, '        } else {')
                    writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Reading from \\"%s\\" registers" << %s << "size:" << %s << "returned different size than requested. Ignoring incomplete data" << unit.values();' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
                    writeLine(fileDescriptor, '        }')
                    writeLine(fileDescriptor, '        verifyUpdateFinished();')
                    writeLine(fileDescriptor, '    });')
                    writeLine(fileDescriptor)
                    writeLine(fileDescriptor, '    connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){')
                    writeLine(fileDescriptor, '        QModbusResponse response = reply->rawResult();')
                    writeLine(fileDescriptor, '        if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {')
                    writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Modbus reply error occurred while reading \\"%s\\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());' % (className, registerDefinition['description']))
                    writeLine(fileDescriptor, '        } else {')
                    writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Modbus reply error occurred while reading \\"%s\\" registers from" << m_modbusTcpMaster->hostAddress().toString() << error << reply->errorString();' % (className, registerDefinition['description']))
                    writeLine(fileDescriptor, '        }')
                    writeLine(fileDescriptor, '    });')

            # Read init blocks
            for blockDefinition in blockDefinitions:
                blockName = blockDefinition['id']
                blockRegisters = blockDefinition['registers']

                if 'readSchedule' in blockDefinition and blockDefinition['readSchedule'] == 'update':
                    blockStartAddress = 0
                    registerCount = 0
                    blockSize = 0
                    registerType = ""

                    for i, blockRegister in enumerate(blockRegisters):
                        if i == 0:
                            blockStartAddress = blockRegister['address']
                            registerType = blockRegister['registerType']

                        registerCount += 1
                        blockSize += blockRegister['size']

                    writeLine(fileDescriptor)
                    writeLine(fileDescriptor, '    // Read %s' % blockName)
                    writeLine(fileDescriptor, '    reply = readBlock%s();' % (blockName[0].upper() + blockName[1:]))
                    writeLine(fileDescriptor, '    qCDebug(dc%s()) << "--> Read block \\"%s\\" registers from:" << %s << "size:" << %s;' % (className, blockName, blockStartAddress, blockSize))
                    writeLine(fileDescriptor, '    if (!reply) {')
                    writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Error occurred while reading block \\"%s\\" registers";' % (className, blockName))
                    writeLine(fileDescriptor, '        return false;')
                    writeLine(fileDescriptor, '    }')
                    writeLine(fileDescriptor)
                    writeLine(fileDescriptor, '    if (reply->isFinished()) {')
                    writeLine(fileDescriptor, '        reply->deleteLater(); // Broadcast reply returns immediatly')
                    writeLine(fileDescriptor, '        return false;')
                    writeLine(fileDescriptor, '    }')
                    writeLine(fileDescriptor)
                    writeLine(fileDescriptor, '    m_pendingUpdateReplies.append(reply);')
                    writeLine(fileDescriptor, '    connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);')
                    writeLine(fileDescriptor, '    connect(reply, &QModbusReply::finished, this, [this, reply](){')
                    writeLine(fileDescriptor, '        m_pendingUpdateReplies.removeAll(reply);')
                    writeLine(fileDescriptor, '        handleModbusError(reply->error());')
                    writeLine(fileDescriptor, '        if (reply->error() != QModbusDevice::NoError) {')
                    writeLine(fileDescriptor, '            verifyUpdateFinished();')
                    writeLine(fileDescriptor, '            return;')
                    writeLine(fileDescriptor, '        }')
                    writeLine(fileDescriptor)
                    writeLine(fileDescriptor, '        const QModbusDataUnit unit = reply->result();')
                    writeLine(fileDescriptor, '        const QVector<quint16> blockValues = unit.values();')
                    writeLine(fileDescriptor, '        qCDebug(dc%s()) << "<-- Response from reading block \\"%s\\" register" << %s << "size:" << %s << blockValues;' % (className, blockName, blockStartAddress, blockSize))
                    writeLine(fileDescriptor, '        if (blockValues.size() == %s) {' % (blockSize))

                    # Start parsing the registers using offsets
                    offset = 0
                    for i, blockRegister in enumerate(blockRegisters):
                        propertyName = blockRegister['id']
                        propertyTyp = getCppDataType(blockRegister)
                        writeLine(fileDescriptor, '            process%sRegisterValues(blockValues.mid(%s, %s));' % (propertyName[0].upper() + propertyName[1:], offset, blockRegister['size']))
                        offset += blockRegister['size']

                    writeLine(fileDescriptor, '        } else {')
                    writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Reading from \\"%s\\" block registers" << %s << "size:" << %s << "returned different size than requested. Ignoring incomplete data" << blockValues;' % (className, blockName, blockStartAddress, blockSize))
                    writeLine(fileDescriptor, '        }')

                    writeLine(fileDescriptor, '        verifyUpdateFinished();')
                    writeLine(fileDescriptor, '    });')
                    writeLine(fileDescriptor)
                    writeLine(fileDescriptor, '    connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error){')
                    writeLine(fileDescriptor, '        QModbusResponse response = reply->rawResult();')
                    writeLine(fileDescriptor, '        if (reply->error() == QModbusDevice::ProtocolError && response.isException()) {')
                    writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Modbus reply error occurred while updating block \\"%s\\" registers" << error << reply->errorString() << ModbusDataUtils::exceptionCodeToString(response.exceptionCode());' % (className, blockName))
                    writeLine(fileDescriptor, '        } else {')
                    writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Modbus reply error occurred while updating block \\"%s\\" registers" << error << reply->errorString();' % (className, blockName))
                    writeLine(fileDescriptor, '        }')
                    writeLine(fileDescriptor, '    });')
                    writeLine(fileDescriptor)

    else:
        writeLine(fileDescriptor, '    // No update registers defined. Nothing to be done and we are finished.')
        writeLine(fileDescriptor, '    emit updateFinished();')

    writeLine(fileDescriptor, '    return true;')
    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)
