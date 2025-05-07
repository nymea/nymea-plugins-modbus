# Copyright 2021 - 2025, nymea GmbH
# Contact: contact@nymea.io
#
# This file is part of nymea.
# This project including source code and documentation is protected by
# copyright law, and remains the property of nymea GmbH. All rights, including
# reproduction, publication, editing and translation, are reserved. The use of
# this project is subject to the terms of a license agreement to be concluded
# with nymea GmbH in accordance with the terms of use of nymea GmbH, available
# under https://nymea.io/license
#
# GNU General Public License Usage
# Alternatively, this project may be redistributed and/or modified under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, GNU version 3. This project is distributed in the hope that it
# will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
# Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# this project. If not, see <https://www.gnu.org/licenses/>.
#
# For any further details and any questions please contact us under
# contact@nymea.io or see our FAQ/Licensing Information on
# https://nymea.io/license/faq

import logging

from .toolcommon import * 

##############################################################

def writePropertyGetSetMethodDeclarationsRtu(fileDescriptor, registerDefinitions):
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
            writeLine(fileDescriptor, '    ModbusRtuReply *set%s(%s %s);' % (propertyName[0].upper() + propertyName[1:], propertyTyp, propertyName))

        writeLine(fileDescriptor)


def writePropertyGetSetMethodImplementationsRtu(fileDescriptor, className, registerDefinitions):
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
            writeLine(fileDescriptor, 'ModbusRtuReply *%s::set%s(%s %s)' % (className, propertyName[0].upper() + propertyName[1:], propertyTyp, propertyName))
            writeLine(fileDescriptor, '{')

            writeLine(fileDescriptor, '    QVector<quint16> values = %s;' % getConversionToValueMethod(registerDefinition))
            writeLine(fileDescriptor, '    qCDebug(dc%s()) << "--> Write \\"%s\\" register:" << %s << "size:" << %s << values;' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
            if registerDefinition['registerType'] == 'holdingRegister':
                writeLine(fileDescriptor, '    return m_modbusRtuMaster->writeHoldingRegisters(m_slaveId, %s, values);' % (registerDefinition['address']))
            elif registerDefinition['registerType'] == 'coils':
                writeLine(fileDescriptor, '    return m_modbusRtuMaster->writeCoils(m_slaveId, %s, values);' % (registerDefinition['address']))
            else:
                logger.warning('Error: invalid register type for writing.')
                exit(1)

            writeLine(fileDescriptor, '}')
            writeLine(fileDescriptor)

##############################################################

def writePropertyUpdateMethodImplementationsRtu(fileDescriptor, className, registerDefinitions):
    for registerDefinition in registerDefinitions:
        if not 'readSchedule' in registerDefinition or registerDefinition['readSchedule'] == 'init':
            continue

        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)
        writeLine(fileDescriptor, 'void %s::update%s()' % (className, propertyName[0].upper() + propertyName[1:]))
        writeLine(fileDescriptor, '{')
        writeLine(fileDescriptor, '    // Update registers from %s' % registerDefinition['description'])
        writeLine(fileDescriptor, '    qCDebug(dc%s()) << "--> Read \\"%s\\" register:" << %s << "size:" << %s;' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
        writeLine(fileDescriptor, '    ModbusRtuReply *reply = read%s();' % (propertyName[0].upper() + propertyName[1:]))
        writeLine(fileDescriptor, '    if (!reply) {')
        writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Error occurred while reading \\"%s\\" registers";' % (className, registerDefinition['description']))
        writeLine(fileDescriptor, '        return;')
        writeLine(fileDescriptor, '    }')
        writeLine(fileDescriptor)
        writeLine(fileDescriptor, '    if (!reply->isFinished()) {')
        writeLine(fileDescriptor, '        connect(reply, &ModbusRtuReply::finished, this, [this, reply](){')
        writeLine(fileDescriptor, '            handleModbusError(reply->error());')
        writeLine(fileDescriptor, '            if (reply->error() == ModbusRtuReply::NoError) {')
        writeLine(fileDescriptor, '                QVector<quint16> values = reply->result();')
        writeLine(fileDescriptor, '                process%sRegisterValues(values);' % (propertyName[0].upper() + propertyName[1:]))
        writeLine(fileDescriptor, '            }')
        writeLine(fileDescriptor, '        });')
        writeLine(fileDescriptor)
        writeLine(fileDescriptor, '        connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){')
        writeLine(fileDescriptor, '            qCWarning(dc%s()) << "ModbusRtu reply error occurred while updating \\"%s\\" registers" << error << reply->errorString();' % (className, registerDefinition['description']))
        writeLine(fileDescriptor, '        });')
        writeLine(fileDescriptor, '    }')
        writeLine(fileDescriptor, '}')
        writeLine(fileDescriptor)

##############################################################

def writeBlockUpdateMethodImplementationsRtu(fileDescriptor, className, blockDefinitions):
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

        writeLine(fileDescriptor, 'void %s::update%sBlock()' % (className, blockName[0].upper() + blockName[1:]))
        writeLine(fileDescriptor, '{')
        writeLine(fileDescriptor, '    // Update register block \"%s\"' % blockName)
        writeLine(fileDescriptor, '    qCDebug(dc%s()) << "--> Read block \\"%s\\" registers from:" << %s << "size:" << %s;' % (className, blockName, blockStartAddress, blockSize))


        # Build request depending on the register type
        if registerType == 'inputRegister':
            writeLine(fileDescriptor, '    ModbusRtuReply *reply = m_modbusRtuMaster->readInputRegister(m_slaveId, %s, %s);' % (blockStartAddress, blockSize))
        elif registerType == 'discreteInputs':
            writeLine(fileDescriptor, '    ModbusRtuReply *reply = m_modbusRtuMaster->readDiscreteInput(m_slaveId, %s, %s);' % (blockStartAddress, blockSize))
        elif registerType == 'coils':
            writeLine(fileDescriptor, '    ModbusRtuReply *reply = m_modbusRtuMaster->readCoil(m_slaveId, %s, %s);' % (blockStartAddress, blockSize))
        else:
            #Default to holdingRegister
            writeLine(fileDescriptor, '    ModbusRtuReply *reply = m_modbusRtuMaster->readHoldingRegister(m_slaveId, %s, %s);' % (blockStartAddress, blockSize))

        writeLine(fileDescriptor, '    if (!reply) {')
        writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Error occurred while reading block \\"%s\\" registers";' % (className, blockName))
        writeLine(fileDescriptor, '        return;')
        writeLine(fileDescriptor, '    }')
        writeLine(fileDescriptor)
        writeLine(fileDescriptor, '    if (!reply->isFinished()) {')
        writeLine(fileDescriptor, '        connect(reply, &ModbusRtuReply::finished, this, [this, reply](){')
        writeLine(fileDescriptor, '            handleModbusError(reply->error());')
        writeLine(fileDescriptor, '            if (reply->error() == ModbusRtuReply::NoError) {')
        writeLine(fileDescriptor, '                QVector<quint16> blockValues = reply->result();')
        writeLine(fileDescriptor, '                qCDebug(dc%s()) << "<-- Response from reading block \\"%s\\" register" << %s << "size:" << %s << blockValues;' % (className, blockName, blockStartAddress, blockSize))
        writeLine(fileDescriptor, '                if (blockValues.size() == %s) {' % (blockSize))

        # Start parsing the registers using offsets
        offset = 0
        for i, blockRegister in enumerate(blockRegisters):
            propertyName = blockRegister['id']
            writeLine(fileDescriptor, '                    process%sRegisterValues(blockValues.mid(%s, %s));' % (propertyName[0].upper() + propertyName[1:], offset, blockSize))
            offset += blockRegister['size']

        writeLine(fileDescriptor, '                } else {')
        writeLine(fileDescriptor, '                    qCWarning(dc%s()) << "Reading from \\"%s\\" register" << %s << "size:" << %s << "returned different size than requested. Ignoring incomplete data" << blockValues;' % (className, blockName, blockStartAddress, blockSize))
        writeLine(fileDescriptor, '                }')
        writeLine(fileDescriptor, '            }')
        writeLine(fileDescriptor, '        });')
        writeLine(fileDescriptor)
        writeLine(fileDescriptor, '        connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){')
        writeLine(fileDescriptor, '            qCWarning(dc%s()) << "ModbusRtu reply error occurred while updating block \\"%s\\" registers" << error << reply->errorString();' % (className, blockName))
        writeLine(fileDescriptor, '        });')
        writeLine(fileDescriptor, '    }')
        writeLine(fileDescriptor, '}')
        writeLine(fileDescriptor)

##############################################################

def writeInternalPropertyReadMethodDeclarationsRtu(fileDescriptor, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        writeLine(fileDescriptor, '    ModbusRtuReply *read%s();' % (propertyName[0].upper() + propertyName[1:]))


def writeInternalPropertyReadMethodImplementationsRtu(fileDescriptor, className, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        writeLine(fileDescriptor, 'ModbusRtuReply *%s::read%s()' % (className, propertyName[0].upper() + propertyName[1:]))
        writeLine(fileDescriptor, '{')

        # Build request depending on the register type
        if registerDefinition['registerType'] == 'inputRegister':
            writeLine(fileDescriptor, '    return m_modbusRtuMaster->readInputRegister(m_slaveId, %s, %s);' % (registerDefinition['address'], registerDefinition['size']))
        elif registerDefinition['registerType'] == 'discreteInputs':
            writeLine(fileDescriptor, '    return m_modbusRtuMaster->readDiscreteInput(m_slaveId, %s, %s);' % (registerDefinition['address'], registerDefinition['size']))
        elif registerDefinition['registerType'] == 'coils':
            writeLine(fileDescriptor, '    return m_modbusRtuMaster->readCoil(m_slaveId, %s, %s);' % (registerDefinition['address'], registerDefinition['size']))
        else:
            #Default to holdingRegister
            writeLine(fileDescriptor, '    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, %s, %s);' % (registerDefinition['address'], registerDefinition['size']))

        writeLine(fileDescriptor, '}')
        writeLine(fileDescriptor)

##############################################################

def writeInternalBlockReadMethodDeclarationsRtu(fileDescriptor, blockDefinitions):
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

        writeLine(fileDescriptor, '    /* Read block from start address %s with size of %s registers containing following %s properties:' % (blockStartAddress, blockSize, registerCount))
        for i, registerDefinition in enumerate(blockRegisters):
            if 'unit' in registerDefinition and registerDefinition['unit'] != '':
                writeLine(fileDescriptor, '      - %s [%s] - Address: %s, Size: %s' % (registerDefinition['description'], registerDefinition['unit'], registerDefinition['address'], registerDefinition['size']))
            else:
                writeLine(fileDescriptor, '     - %s - Address: %s, Size: %s' % (registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
        writeLine(fileDescriptor, '    */' )
        writeLine(fileDescriptor, '    ModbusRtuReply *readBlock%s();' % (blockName[0].upper() + blockName[1:]))
        writeLine(fileDescriptor)


def writeInternalBlockReadMethodImplementationsRtu(fileDescriptor, className, blockDefinitions):
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


        writeLine(fileDescriptor, 'ModbusRtuReply *%s::readBlock%s()' % (className, blockName[0].upper() + blockName[1:]))
        writeLine(fileDescriptor, '{')

        # Build request depending on the register type
        if registerType == 'inputRegister':
            writeLine(fileDescriptor, '    return m_modbusRtuMaster->readInputRegister(m_slaveId, %s, %s);' % (blockStartAddress, blockSize))
        elif registerType == 'discreteInputs':
            writeLine(fileDescriptor, '    return m_modbusRtuMaster->readDiscreteInput(m_slaveId, %s, %s);' % (blockStartAddress, blockSize))
        elif registerType == 'coils':
            writeLine(fileDescriptor, '    return m_modbusRtuMaster->readCoil(m_slaveId, %s, %s);' % (blockStartAddress, blockSize))
        else:
            #Default to holdingRegister
            writeLine(fileDescriptor, '    return m_modbusRtuMaster->readHoldingRegister(m_slaveId, %s, %s);' % (blockStartAddress, blockSize))

        writeLine(fileDescriptor, '}')
        writeLine(fileDescriptor)

##############################################################

def writeTestReachabilityImplementationsRtu(fileDescriptor, className, registerDefinitions, checkReachableRegister):

    propertyName = checkReachableRegister['id']
    propertyTyp = getCppDataType(checkReachableRegister)

    writeLine(fileDescriptor, 'void %s::testReachability()' % (className))
    writeLine(fileDescriptor, '{')
    writeLine(fileDescriptor, '    if (m_checkReachableReply)')
    writeLine(fileDescriptor, '        return;')
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '    // Try to read the check reachability register %s in order to verify if the communication is working or not.' % checkReachableRegister['id'])
    writeLine(fileDescriptor, '    qCDebug(dc%s()) << "--> Test reachability by reading \\"%s\\" register:" << %s << "size:" << %s;' % (className, checkReachableRegister['description'], checkReachableRegister['address'], checkReachableRegister['size']))
    writeLine(fileDescriptor, '    m_checkReachableReply = read%s();' % (propertyName[0].upper() + propertyName[1:]))
    writeLine(fileDescriptor, '    if (!m_checkReachableReply) {')
    writeLine(fileDescriptor, '        qCDebug(dc%s()) << "Error occurred verifying reachability by reading \\"%s\\" register";' % (className, checkReachableRegister['description']))
    writeLine(fileDescriptor, '        onReachabilityCheckFailed();')
    writeLine(fileDescriptor, '        return;')
    writeLine(fileDescriptor, '    }')
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '    if (m_checkReachableReply->isFinished()) {')
    writeLine(fileDescriptor, '        m_checkReachableReply = nullptr;')
    writeLine(fileDescriptor, '        onReachabilityCheckFailed();')
    writeLine(fileDescriptor, '        return;')
    writeLine(fileDescriptor, '    }')
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '    connect(m_checkReachableReply, &ModbusRtuReply::finished, this, [this](){')
    writeLine(fileDescriptor, '        // Note: we don\'t care about the result here, only the error')
    writeLine(fileDescriptor, '        handleModbusError(m_checkReachableReply->error());')
    writeLine(fileDescriptor, '        if (m_checkReachableReply->error() != ModbusRtuReply::NoError)')
    writeLine(fileDescriptor, '            onReachabilityCheckFailed();')
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '        m_checkReachableReply = nullptr;')
    writeLine(fileDescriptor, '    });')
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '    connect(m_checkReachableReply, &ModbusRtuReply::errorOccurred, this, [this] (ModbusRtuReply::Error error){')
    writeLine(fileDescriptor, '        qCDebug(dc%s()) << "ModbusRtu reply error occurred while verifying reachability by reading \\"%s\\" register" << error << m_checkReachableReply->errorString();' % (className, checkReachableRegister['description']))
    writeLine(fileDescriptor, '    });')
    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)

##############################################################

def writeInitMethodImplementationRtu(fileDescriptor, className, registerDefinitions, blockDefinitions):
    writeLine(fileDescriptor, 'bool %s::initialize()' % (className))
    writeLine(fileDescriptor, '{')
    writeLine(fileDescriptor, '    if (!m_reachable) {')
    writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Tried to initialize but the device is not to be reachable.";' % className)
    writeLine(fileDescriptor, '        return false;')
    writeLine(fileDescriptor, '    }')

    # First check if there are any init registers
    initRequired = False
    for registerDefinition in registerDefinitions:
        if registerDefinition['readSchedule'] == 'init':
            initRequired = True
            break

    for blockDefinition in blockDefinitions:  
        if 'readSchedule' in blockDefinition and blockDefinition['readSchedule'] == 'init':
            initRequired = True
            break

    if initRequired:
        writeLine(fileDescriptor, '    if (m_initObject) {')
        writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Tried to initialize but the init process is already running.";' % className)
        writeLine(fileDescriptor, '        return false;')
        writeLine(fileDescriptor, '    }')
        writeLine(fileDescriptor)
        writeLine(fileDescriptor, '    // Parent object for the init process')
        writeLine(fileDescriptor, '    m_initObject = new QObject(this);')
        writeLine(fileDescriptor)
        writeLine(fileDescriptor, '    ModbusRtuReply *reply = nullptr;')

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
                writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Error occurred while reading \\"%s\\" registers";' % (className, registerDefinition['description']))
                writeLine(fileDescriptor, '        finishInitialization(false);')
                writeLine(fileDescriptor, '        return false;')
                writeLine(fileDescriptor, '    }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    if (reply->isFinished()) {')
                writeLine(fileDescriptor, '        finishInitialization(false); // Broadcast reply returns immediately')
                writeLine(fileDescriptor, '        return false;')
                writeLine(fileDescriptor, '    }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    m_pendingInitReplies.append(reply);')
                writeLine(fileDescriptor, '    connect(reply, &ModbusRtuReply::finished, m_initObject, [this, reply](){')
                writeLine(fileDescriptor, '        handleModbusError(reply->error());')
                writeLine(fileDescriptor, '        m_pendingInitReplies.removeAll(reply);')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        if (reply->error() != ModbusRtuReply::NoError) {')
                writeLine(fileDescriptor, '            finishInitialization(false);')
                writeLine(fileDescriptor, '            return;')
                writeLine(fileDescriptor, '        }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        QVector<quint16> values = reply->result();')
                writeLine(fileDescriptor, '        process%sRegisterValues(values);' % (propertyName[0].upper() + propertyName[1:]))
                writeLine(fileDescriptor, '        verifyInitFinished();')
                writeLine(fileDescriptor, '    });')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    connect(reply, &ModbusRtuReply::errorOccurred, m_initObject, [reply] (ModbusRtuReply::Error error){')
                writeLine(fileDescriptor, '        qCWarning(dc%s()) << "ModbusRtu reply error occurred while updating \\"%s\\" registers" << error << reply->errorString();' % (className, registerDefinition['description']))
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
                writeLine(fileDescriptor, '        finishInitialization(false); // Broadcast reply returns immediately')
                writeLine(fileDescriptor, '        return false;')
                writeLine(fileDescriptor, '    }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    m_pendingInitReplies.append(reply);')
                writeLine(fileDescriptor, '    connect(reply, &ModbusRtuReply::finished, m_initObject, [this, reply](){')
                writeLine(fileDescriptor, '        handleModbusError(reply->error());')
                writeLine(fileDescriptor, '        m_pendingInitReplies.removeAll(reply);')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        if (reply->error() != ModbusRtuReply::NoError) {')
                writeLine(fileDescriptor, '            finishInitialization(false);')
                writeLine(fileDescriptor, '            return;')
                writeLine(fileDescriptor, '        }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        QVector<quint16> blockValues = reply->result();')
                writeLine(fileDescriptor, '        qCDebug(dc%s()) << "<-- Response from reading init block \\"%s\\" register" << %s << "size:" << %s << blockValues;' % (className, blockName, blockStartAddress, blockSize))
                writeLine(fileDescriptor, '        if (blockValues.size() == %s) {' % (blockSize))

                # Start parsing the registers using offsets
                offset = 0
                for i, blockRegister in enumerate(blockRegisters):
                    propertyName = blockRegister['id']
                    propertyTyp = getCppDataType(blockRegister)
                    writeLine(fileDescriptor, '        process%sRegisterValues(blockValues.mid(%s, %s));' % (propertyName[0].upper() + propertyName[1:], offset, blockRegister['size']))
                    offset += blockRegister['size']

                writeLine(fileDescriptor, '        } else {')
                writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Reading from \\"%s\\" register" << %s << "size:" << %s << "returned different size than requested. Ignoring incomplete data" << blockValues;' % (className, blockName, blockStartAddress, blockSize))
                writeLine(fileDescriptor, '        }')  
                writeLine(fileDescriptor, '        verifyInitFinished();')
                writeLine(fileDescriptor, '    });')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    connect(reply, &ModbusRtuReply::errorOccurred, m_initObject, [reply] (ModbusRtuReply::Error error){')
                writeLine(fileDescriptor, '        qCWarning(dc%s()) << "ModbusRtu reply error occurred while updating block \\"%s\\" registers" << error << reply->errorString();' % (className, blockName))
                writeLine(fileDescriptor, '    });')
                writeLine(fileDescriptor)

    else:
        writeLine(fileDescriptor, '    // No init registers defined. Nothing to be done and we are finished.')
        writeLine(fileDescriptor, '    emit initializationFinished(true);')

    writeLine(fileDescriptor, '    return true;')
    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)


def writeUpdateMethodRtu(fileDescriptor, className, registerDefinitions, blockDefinitions):
    writeLine(fileDescriptor, 'bool %s::update()' % (className))
    writeLine(fileDescriptor, '{')

    # First check if there are any init registers
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
        writeLine(fileDescriptor, '    if (!m_modbusRtuMaster->connected()) {')
        writeLine(fileDescriptor, '        qCDebug(dc%s()) << "Tried to update the registers but the hardware resource seems not to be connected.";' % className)
        writeLine(fileDescriptor, '        return false;')
        writeLine(fileDescriptor, '    }')
        writeLine(fileDescriptor)
        writeLine(fileDescriptor, '    if (!m_pendingUpdateReplies.isEmpty()) {')
        writeLine(fileDescriptor, '        qCDebug(dc%s()) << "Tried to update the registers but there are still some update replies pending. Waiting for them to be finished...";' % className)
        writeLine(fileDescriptor, '        return true;')
        writeLine(fileDescriptor, '    }')
        writeLine(fileDescriptor)

        writeLine(fileDescriptor, '    // Hardware resource available but communication not working. ')
        writeLine(fileDescriptor, '    // Try to read the check reachability register to re-evaluate the communication... ')
        writeLine(fileDescriptor, '    if (m_modbusRtuMaster->connected() && !m_communicationWorking) {')
        writeLine(fileDescriptor, '        testReachability();')
        writeLine(fileDescriptor, '        return false;')
        writeLine(fileDescriptor, '    }')
        writeLine(fileDescriptor)

        writeLine(fileDescriptor, '    ModbusRtuReply *reply = nullptr;')

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
                writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Error occurred while reading \\"%s\\" registers";' % (className, registerDefinition['description']))
                writeLine(fileDescriptor, '        return false;')
                writeLine(fileDescriptor, '    }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    if (reply->isFinished()) {')
                writeLine(fileDescriptor, '        return false; // Broadcast reply returns immediately')
                writeLine(fileDescriptor, '    }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    m_pendingUpdateReplies.append(reply);')
                writeLine(fileDescriptor, '    connect(reply, &ModbusRtuReply::finished, this, [this, reply](){')
                writeLine(fileDescriptor, '        handleModbusError(reply->error());')
                writeLine(fileDescriptor, '        m_pendingUpdateReplies.removeAll(reply);')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        if (reply->error() != ModbusRtuReply::NoError) {')
                writeLine(fileDescriptor, '            verifyUpdateFinished();')
                writeLine(fileDescriptor, '            return;')
                writeLine(fileDescriptor, '        }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        QVector<quint16> values = reply->result();')
                writeLine(fileDescriptor, '        process%sRegisterValues(values);' % (propertyName[0].upper() + propertyName[1:]))
                writeLine(fileDescriptor, '        verifyUpdateFinished();')
                writeLine(fileDescriptor, '    });')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){')
                writeLine(fileDescriptor, '        qCWarning(dc%s()) << "ModbusRtu reply error occurred while updating \\"%s\\" registers" << error << reply->errorString();' % (className, registerDefinition['description']))
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
                writeLine(fileDescriptor, '    qCDebug(dc%s()) << "--> Read block \\"%s\\" registers from:" << %s << "size:" << %s;' % (className, blockName, blockStartAddress, blockSize))
                writeLine(fileDescriptor, '    reply = readBlock%s();' % (blockName[0].upper() + blockName[1:]))
                writeLine(fileDescriptor, '    if (!reply) {')
                writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Error occurred while reading block \\"%s\\" registers";' % (className, blockName))
                writeLine(fileDescriptor, '        return false;')
                writeLine(fileDescriptor, '    }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    if (reply->isFinished()) {')
                writeLine(fileDescriptor, '        return false; // Broadcast reply returns immediately')
                writeLine(fileDescriptor, '    }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    m_pendingUpdateReplies.append(reply);')
                writeLine(fileDescriptor, '    connect(reply, &ModbusRtuReply::finished, this, [this, reply](){')
                writeLine(fileDescriptor, '        handleModbusError(reply->error());')
                writeLine(fileDescriptor, '        m_pendingUpdateReplies.removeAll(reply);')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        if (reply->error() != ModbusRtuReply::NoError) {')
                writeLine(fileDescriptor, '            verifyUpdateFinished();')
                writeLine(fileDescriptor, '            return;')
                writeLine(fileDescriptor, '        }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '        QVector<quint16> blockValues = reply->result();')
                writeLine(fileDescriptor, '        qCDebug(dc%s()) << "<-- Response from reading block \\"%s\\" register" << %s << "size:" << %s << blockValues;' % (className, blockName, blockStartAddress, blockSize))
                writeLine(fileDescriptor, '        if (blockValues.size() == %s) {' % (blockSize))

                # Start parsing the registers using offsets
                offset = 0
                for i, blockRegister in enumerate(blockRegisters):
                    propertyName = blockRegister['id']
                    propertyTyp = getCppDataType(blockRegister)
                    writeLine(fileDescriptor, '        process%sRegisterValues(blockValues.mid(%s, %s));' % (propertyName[0].upper() + propertyName[1:], offset, blockRegister['size']))
                    offset += blockRegister['size']

                writeLine(fileDescriptor, '        } else {')
                writeLine(fileDescriptor, '            qCWarning(dc%s()) << "Reading from \\"%s\\" register" << %s << "size:" << %s << "returned different size than requested. Ignoring incomplete data" << blockValues;' % (className, blockName, blockStartAddress, blockSize))
                writeLine(fileDescriptor, '        }')
                writeLine(fileDescriptor, '        verifyUpdateFinished();')
                writeLine(fileDescriptor, '    });')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){')
                writeLine(fileDescriptor, '        qCWarning(dc%s()) << "ModbusRtu reply error occurred while updating block \\"%s\\" registers" << error << reply->errorString();' % (className, blockName))
                writeLine(fileDescriptor, '    });')
                writeLine(fileDescriptor)

    else:
        writeLine(fileDescriptor, '    // No update registers defined. Nothing to be done and we are finished.')
        writeLine(fileDescriptor, '    emit updateFinished();')

    writeLine(fileDescriptor, '    return true;')
    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)
