# Copyright (C) 2021 - 2022 nymea GmbH <developer@nymea.io>
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

import os
import re
import sys
import json
import shutil
import datetime
import logging

logger = logging.getLogger('modbus-tools')

def convertToAlphaNumeric(text):
    finalText = ''
    for character in text:
        if character.isalnum():
            finalText += character
        else:
            finalText += ' '
    return finalText


def splitCamelCase(text):
    return re.sub('([A-Z][a-z]+)', r' \1', re.sub('([A-Z]+)', r' \1', text)).split()


def convertToCamelCase(text, capitalize = False):
    s = convertToAlphaNumeric(text)
    s = s.replace("-", " ").replace("_", " ")
    words = s.split()
    logger.debug('--> words', words)
    finalWords = []

    for i in range(len(words)):
        camelCaseSplit = splitCamelCase(words[i])
        if len(camelCaseSplit) == 0:
            finalWords.append(words[i])  
        else:
            logging.debug('Camel calse split words', camelCaseSplit)
            for j in range(len(camelCaseSplit)):
                finalWords.append(camelCaseSplit[j])  

    if len(finalWords) == 0:
        return text

    finalText = ''
    if capitalize:
        finalText = finalWords[0].capitalize() + ''.join(i.capitalize() for i in finalWords[1:])
    else:
        finalText = finalWords[0].lower() + ''.join(i.capitalize() for i in finalWords[1:])
    logging.debug('Convert camel case:', text, '-->', finalText)
    return finalText


def loadJsonFile(filePath):
    logger.info('Loading JSON file %s', filePath)
    jsonFile = open(filePath, 'r')
    return json.load(jsonFile)


def writeLine(fileDescriptor, line = ''):
    fileDescriptor.write(line + '\n')


def writeLicenseHeader(fileDescriptor):
    writeLine(fileDescriptor, '/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *')
    writeLine(fileDescriptor, '*')
    writeLine(fileDescriptor, '* Copyright 2013 - %s, nymea GmbH' % datetime.datetime.now().year)
    writeLine(fileDescriptor, '* Contact: contact@nymea.io')
    writeLine(fileDescriptor, '*')
    writeLine(fileDescriptor, '* This fileDescriptor is part of nymea.')
    writeLine(fileDescriptor, '* This project including source code and documentation is protected by')
    writeLine(fileDescriptor, '* copyright law, and remains the property of nymea GmbH. All rights, including')
    writeLine(fileDescriptor, '* reproduction, publication, editing and translation, are reserved. The use of')
    writeLine(fileDescriptor, '* this project is subject to the terms of a license agreement to be concluded')
    writeLine(fileDescriptor, '* with nymea GmbH in accordance with the terms of use of nymea GmbH, available')
    writeLine(fileDescriptor, '* under https://nymea.io/license')
    writeLine(fileDescriptor, '*')
    writeLine(fileDescriptor, '* GNU Lesser General Public License Usage')
    writeLine(fileDescriptor, '* Alternatively, this project may be redistributed and/or modified under the')
    writeLine(fileDescriptor, '* terms of the GNU Lesser General Public License as published by the Free')
    writeLine(fileDescriptor, '* Software Foundation; version 3. This project is distributed in the hope that')
    writeLine(fileDescriptor, '* it will be useful, but WITHOUT ANY WARRANTY; without even the implied')
    writeLine(fileDescriptor, '* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU')
    writeLine(fileDescriptor, '* Lesser General Public License for more details.')
    writeLine(fileDescriptor, '*')
    writeLine(fileDescriptor, '* You should have received a copy of the GNU Lesser General Public License')
    writeLine(fileDescriptor, '* along with this project. If not, see <https://www.gnu.org/licenses/>.')
    writeLine(fileDescriptor, '*')
    writeLine(fileDescriptor, '* For any further details and any questions please contact us under')
    writeLine(fileDescriptor, '* contact@nymea.io or see our FAQ/Licensing Information on')
    writeLine(fileDescriptor, '* https://nymea.io/license/faq')
    writeLine(fileDescriptor, '*')
    writeLine(fileDescriptor, '* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */')
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *')
    writeLine(fileDescriptor, '*')
    writeLine(fileDescriptor, '* WARNING')
    writeLine(fileDescriptor, '*')
    writeLine(fileDescriptor, '* This file has been autogenerated. Any changes in this file may be overwritten.')
    writeLine(fileDescriptor, '* If you want to change something, update the register json or the tool.')
    writeLine(fileDescriptor, '*')
    writeLine(fileDescriptor, '* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */')
    writeLine(fileDescriptor)


def writeRegistersEnum(fileDescriptor, registerJson):
    logger.debug('Writing enum for all registers')

    registerEnums = {}

    # Read all register names and addresses
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            blockRegisters = blockDefinition['registers']
            for blockRegister in blockRegisters:
                registerName = blockRegister['id']        
                registerAddress = blockRegister['address']        
                registerEnums[registerAddress] = registerName

    for registerDefinition in registerJson['registers']:
        registerName = registerDefinition['id']        
        registerAddress = registerDefinition['address']        
        registerEnums[registerAddress] = registerName


    # Sort the enum map
    registersKeys = registerEnums.keys()
    sortedRegistersKeys = sorted(registersKeys)
    sortedRegisterEnumList = []

    logger.debug('Sorted registers')
    for registerAddress in sortedRegistersKeys:
        logger.debug('--> %s : %s' % (registerAddress, registerEnums[registerAddress]))
        enumData = {}
        enumData['key'] = registerEnums[registerAddress]
        enumData['value'] = registerAddress
        sortedRegisterEnumList.append(enumData)

    enumName = 'Registers'
    writeLine(fileDescriptor, '    enum %s {' % enumName)
    for i in range(len(sortedRegisterEnumList)):
        enumData = sortedRegisterEnumList[i]
        line = ('        Register%s = %s' % (enumData['key'][0].upper() + enumData['key'][1:] , enumData['value']))
        if i < (len(sortedRegisterEnumList) - 1):
            line += ','

        writeLine(fileDescriptor, line)

    writeLine(fileDescriptor, '    };')
    writeLine(fileDescriptor, '    Q_ENUM(%s)' % enumName)
    writeLine(fileDescriptor)


def writeEnumDefinition(fileDescriptor, enumDefinition):
    logger.debug('Writing enum %s', enumDefinition)
    enumName = enumDefinition['name']
    enumValues = enumDefinition['values']
    writeLine(fileDescriptor, '    enum %s {' % enumName)
    for i in range(len(enumValues)):
        enumData = enumValues[i]
        line = ('        %s%s = %s' % (enumName, enumData['key'], enumData['value']))
        if i < (len(enumValues) - 1):
            line += ','

        writeLine(fileDescriptor, line)

    writeLine(fileDescriptor, '    };')
    writeLine(fileDescriptor, '    Q_ENUM(%s)' % enumName)
    writeLine(fileDescriptor)


def getCppDataType(registerDefinition, rawType = False):
    if not rawType:
        if 'enum' in registerDefinition:
            return registerDefinition['enum']

        if 'scaleFactor' in registerDefinition or 'staticScaleFactor' in registerDefinition:
            return 'float'

    if registerDefinition['type'] == 'uint16':
        return 'quint16'

    if registerDefinition['type'] == 'int16':
        return 'qint16'

    if registerDefinition['type'] == 'uint32':
        return 'quint32'

    if registerDefinition['type'] == 'int32':
        return 'qint32'

    if registerDefinition['type'] == 'uint64':
        return 'quint64'

    if registerDefinition['type'] == 'int64':
        return 'qint64'

    if registerDefinition['type'] == 'float':
        return 'float'

    if registerDefinition['type'] == 'float64':
        return 'double'

    if registerDefinition['type'] == 'string':
        return 'QString'


def getConversionToValueMethod(registerDefinition):
    # Handle enums
    propertyName = registerDefinition['id']
    propertyTyp = getCppDataType(registerDefinition, True)

    if 'enum' in registerDefinition:
        enumName = registerDefinition['enum']
        if registerDefinition['type'] == 'uint16':
            return ('ModbusDataUtils::convertFromUInt16(static_cast<%s>(%s))' % (propertyTyp, propertyName))
        elif registerDefinition['type'] == 'int16':
            return ('ModbusDataUtils::convertFromInt16(static_cast<%s>(%s))' % (propertyTyp, propertyName))
        elif registerDefinition['type'] == 'uint32':
            return ('ModbusDataUtils::convertFromUInt32(static_cast<%s>(%s), m_endianness)' % (propertyTyp, propertyName))
        elif registerDefinition['type'] == 'int32':
            return ('ModbusDataUtils::convertFromInt32(static_cast<%s>(%s), m_endianness)' % (propertyTyp, propertyName))

    # Handle scale factors
    if 'scaleFactor' in registerDefinition:
        scaleFactorProperty = 'm_%s' % registerDefinition['scaleFactor']
        if registerDefinition['type'] == 'uint16':
            return ('ModbusDataUtils::convertFromUInt16(static_cast<%s>(%s  * 1.0 / pow(10, %s)))' % (propertyTyp, propertyName, scaleFactorProperty))
        elif registerDefinition['type'] == 'int16':
            return ('ModbusDataUtils::convertFromInt16(static_cast<%s>(%s  * 1.0 / pow(10, %s)))' % (propertyTyp, propertyName, scaleFactorProperty))
        elif registerDefinition['type'] == 'uint32':
            return ('ModbusDataUtils::convertFromUInt32(static_cast<%s>(%s  * 1.0 / pow(10, %s)), m_endianness)' % (propertyTyp, propertyName, scaleFactorProperty))
        elif registerDefinition['type'] == 'int32':
            return ('ModbusDataUtils::convertFromInt32(static_cast<%s>(%s  * 1.0 / pow(10, %s)), m_endianness)' % (propertyTyp, propertyName, scaleFactorProperty))

    elif 'staticScaleFactor' in registerDefinition:
        scaleFactor = registerDefinition['staticScaleFactor']
        if registerDefinition['type'] == 'uint16':
            return ('ModbusDataUtils::convertFromUInt16(static_cast<%s>(%s  * 1.0 / pow(10, %s)))' % (propertyTyp, propertyName, scaleFactor))
        elif registerDefinition['type'] == 'int16':
            return ('ModbusDataUtils::convertFromInt16(static_cast<%s>(%s  * 1.0 / pow(10, %s)))' % (propertyTyp, propertyName, scaleFactor))
        elif registerDefinition['type'] == 'uint32':
            return ('ModbusDataUtils::convertFromUInt32(static_cast<%s>(%s  * 1.0 / pow(10, %s)), m_endianness)' % (propertyTyp, propertyName, scaleFactor))
        elif registerDefinition['type'] == 'int32':
            return ('ModbusDataUtils::convertFromInt32(static_cast<%s>(%s  * 1.0 / pow(10, %s)), m_endianness)' % (propertyTyp, propertyName, scaleFactor))

    # Handle default types
    elif registerDefinition['type'] == 'uint16':
        return ('ModbusDataUtils::convertFromUInt16(%s)' % propertyName)
    elif registerDefinition['type'] == 'int16':
        return ('ModbusDataUtils::convertFromInt16(%s)' % propertyName)
    elif registerDefinition['type'] == 'uint32':
        return ('ModbusDataUtils::convertFromUInt32(%s, m_endianness)' % (propertyName))
    elif registerDefinition['type'] == 'int32':
        return ('ModbusDataUtils::convertFromInt32(%s, m_endianness)' % (propertyName))
    elif registerDefinition['type'] == 'uint64':
        return ('ModbusDataUtils::convertFromUInt64(%s, m_endianness)' % (propertyName))
    elif registerDefinition['type'] == 'int64':
        return ('ModbusDataUtils::convertFromInt64(%s, m_endianness)' % (propertyName))
    elif registerDefinition['type'] == 'float':
        return ('ModbusDataUtils::convertFromFloat32(%s, m_endianness)' % propertyName)
    elif registerDefinition['type'] == 'float64':
        return ('ModbusDataUtils::convertFromFloat64(%s, m_endianness)' % propertyName)
    elif registerDefinition['type'] == 'string':
        return ('ModbusDataUtils::convertFromString(%s)' % propertyName)    


def getValueConversionMethod(registerDefinition):
    # Handle enums
    if 'enum' in registerDefinition:
        enumName = registerDefinition['enum']
        if registerDefinition['type'] == 'uint16':
            return ('static_cast<%s>(ModbusDataUtils::convertToUInt16(values))' % (enumName))
        elif registerDefinition['type'] == 'int16':
            return ('static_cast<%s>(ModbusDataUtils::convertToInt16(values))' % (enumName))
        elif registerDefinition['type'] == 'uint32':
            return ('static_cast<%s>(ModbusDataUtils::convertToUInt32(values, m_endianness))' % (enumName))
        elif registerDefinition['type'] == 'int32':
            return ('static_cast<%s>(ModbusDataUtils::convertToInt32(values, m_endianness))' % (enumName))

    # Handle scale factors
    if 'scaleFactor' in registerDefinition:
        scaleFactorProperty = 'm_%s' % registerDefinition['scaleFactor']
        if registerDefinition['type'] == 'uint16':
            return ('ModbusDataUtils::convertToUInt16(values) * 1.0 * pow(10, %s)' % (scaleFactorProperty))
        elif registerDefinition['type'] == 'int16':
            return ('ModbusDataUtils::convertToInt16(values) * 1.0 * pow(10, %s)' % (scaleFactorProperty))
        elif registerDefinition['type'] == 'uint32':
            return ('ModbusDataUtils::convertToUInt32(values, m_endianness) * 1.0 * pow(10, %s)' % (scaleFactorProperty))
        elif registerDefinition['type'] == 'int32':
            return ('ModbusDataUtils::convertToInt32(values, m_endianness) * 1.0 * pow(10, %s)' % (scaleFactorProperty))

    elif 'staticScaleFactor' in registerDefinition:
        scaleFactor = registerDefinition['staticScaleFactor']
        if registerDefinition['type'] == 'uint16':
            return ('ModbusDataUtils::convertToUInt16(values) * 1.0 * pow(10, %s)' % (scaleFactor))
        elif registerDefinition['type'] == 'int16':
            return ('ModbusDataUtils::convertToInt16(values) * 1.0 * pow(10, %s)' % (scaleFactor))
        elif registerDefinition['type'] == 'uint32':
            return ('ModbusDataUtils::convertToUInt32(values, m_endianness) * 1.0 * pow(10, %s)' % (scaleFactor))
        elif registerDefinition['type'] == 'int32':
            return ('ModbusDataUtils::convertToInt32(values, m_endianness) * 1.0 * pow(10, %s)' % (scaleFactor))

    # Handle default types
    elif registerDefinition['type'] == 'uint16':
        return ('ModbusDataUtils::convertToUInt16(values)')
    elif registerDefinition['type'] == 'int16':
        return ('ModbusDataUtils::convertToInt16(values)')
    elif registerDefinition['type'] == 'uint32':
        return ('ModbusDataUtils::convertToUInt32(values, m_endianness)')
    elif registerDefinition['type'] == 'int32':
        return ('ModbusDataUtils::convertToInt32(values, m_endianness)')
    elif registerDefinition['type'] == 'uint64':
        return ('ModbusDataUtils::convertToUInt64(values, m_endianness)')
    elif registerDefinition['type'] == 'int64':
        return ('ModbusDataUtils::convertToInt64(values, m_endianness)')
    elif registerDefinition['type'] == 'float':
        return ('ModbusDataUtils::convertToFloat32(values, m_endianness)')
    elif registerDefinition['type'] == 'float64':
        return ('ModbusDataUtils::convertToFloat64(values, m_endianness)')
    elif registerDefinition['type'] == 'string':
        return ('ModbusDataUtils::convertToString(values)')


def writeBlockGetMethodDeclarations(fileDescriptor, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)
        if 'unit' in registerDefinition and registerDefinition['unit'] != '':
            writeLine(fileDescriptor, '    /* %s [%s] - Address: %s, Size: %s */' % (registerDefinition['description'], registerDefinition['unit'], registerDefinition['address'], registerDefinition['size']))
        else:
            writeLine(fileDescriptor, '    /* %s - Address: %s, Size: %s */' % (registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))

        writeLine(fileDescriptor, '    %s %s() const;' % (propertyTyp, propertyName))
        writeLine(fileDescriptor)


def writePropertyUpdateMethodDeclarations(fileDescriptor, registerDefinitions):
    for registerDefinition in registerDefinitions:
        if 'readSchedule' in registerDefinition and registerDefinition['readSchedule'] == 'init':
            continue

        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)
        writeLine(fileDescriptor, '    void update%s();' % (propertyName[0].upper() + propertyName[1:]))


def validateBlocks(blockDefinitions):
    for blockDefinition in blockDefinitions:
        blockName = blockDefinition['id']
        blockRegisters = blockDefinition['registers']
        
        blockStartAddress = 0
        registerCount = 0
        blockSize = 0
        registerAccess = ""
        registerType = ""

        for i, blockRegister in enumerate(blockRegisters):
            if i == 0:
                blockStartAddress = blockRegister['address']
                registerAccess = blockRegister['access']
                registerType = blockRegister['registerType']
            else:
                previouseRegisterAddress = blockRegisters[i - 1]['address']
                previouseRegisterSize = blockRegisters[i - 1]['size']
                previouseRegisterType = blockRegisters[i - 1]['registerType']
                if previouseRegisterAddress + previouseRegisterSize != blockRegister['address']:
                    logger.warning('Error: block %s has invalid register order in register %s. There seems to be a gap between the registers.' % (blockName, blockRegister['id']))
                    exit(1)

                if blockRegister['access'] != registerAccess:
                    logger.warning('Error: block %s has inconsistent register access in register %s. The block registers dont seem to have the same access rights.' % (blockName, blockRegister['id']))
                    exit(1)

                if blockRegister['registerType'] != registerType:
                    logger.warning('Error: block %s has inconsistent register type in register %s. The block registers dont seem to be from the same type.' % (blockName, blockRegister['id']))
                    exit(1)

            registerCount += 1
            blockSize += blockRegister['size']

        logger.debug('Define valid block \"%s\" starting at %s with length %s containing %s properties to read.' % (blockName, blockStartAddress, blockSize, registerCount))


def writeBlocksUpdateMethodDeclarations(fileDescriptor, blockDefinitions):
    for blockDefinition in blockDefinitions:
        blockName = blockDefinition['id']
        blockRegisters = blockDefinition['registers']
        blockStartAddress = 0
        blockSize = 0
        registerCount = 0

        for i, blockRegister in enumerate(blockRegisters):
            if i == 0:
                blockStartAddress = blockRegister['address']

            registerCount += 1
            blockSize += blockRegister['size']

        # Write the block update method
        writeLine(fileDescriptor, '    /* Read block from start addess %s with size of %s registers containing following %s properties:' % (blockStartAddress, blockSize, registerCount))
        for i, registerDefinition in enumerate(blockRegisters):
            if 'unit' in registerDefinition and registerDefinition['unit'] != '':
                writeLine(fileDescriptor, '      - %s [%s] - Address: %s, Size: %s' % (registerDefinition['description'], registerDefinition['unit'], registerDefinition['address'], registerDefinition['size']))
            else:
                writeLine(fileDescriptor, '      - %s - Address: %s, Size: %s' % (registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
        writeLine(fileDescriptor, '    */' )
        writeLine(fileDescriptor, '    void update%sBlock();' % (blockName[0].upper() + blockName[1:]))
        writeLine(fileDescriptor)


def writeRegistersDebugLine(fileDescriptor, debugObjectParamName, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)
        line = ('"    - %s:" << %s->%s()' % (registerDefinition['description'], debugObjectParamName, propertyName))
        if 'unit' in registerDefinition and registerDefinition['unit'] != '':
            line += (' << " [%s]"' % registerDefinition['unit'])
        writeLine(fileDescriptor, '    debug.nospace().noquote() << %s << "\\n";' % (line))


def writeUpdateMethod(fileDescriptor, className, registerDefinitions, blockDefinitions):
    writeLine(fileDescriptor, 'void %s::update()' % (className))
    writeLine(fileDescriptor, '{')
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        if 'readSchedule' in registerDefinition and registerDefinition['readSchedule'] == 'update':
            writeLine(fileDescriptor, '    update%s();' % (propertyName[0].upper() + propertyName[1:]))

    # Add the update block methods
    for blockDefinition in blockDefinitions:
        blockName = blockDefinition['id']
        if 'readSchedule' in blockDefinition and blockDefinition['readSchedule'] == 'update':
            writeLine(fileDescriptor, '    update%sBlock();' % (blockName[0].upper() + blockName[1:]))

    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)


def writePropertyChangedSignals(fileDescriptor, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)
        if propertyTyp == 'QString':
            writeLine(fileDescriptor, '    void %sChanged(const %s &%s);' % (propertyName, propertyTyp, propertyName))
        else:
            writeLine(fileDescriptor, '    void %sChanged(%s %s);' % (propertyName, propertyTyp, propertyName))


def writeProtectedPropertyMembers(fileDescriptor, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)
        if 'defaultValue' in registerDefinition:
            writeLine(fileDescriptor, '    %s m_%s = %s;' % (propertyTyp, propertyName, registerDefinition['defaultValue']))
        else:
            writeLine(fileDescriptor, '    %s m_%s;' % (propertyTyp, propertyName))


def writePropertyProcessMethodDeclaration(fileDescriptor, registerDefinitions):
    propertyVariables = []
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        writeLine(fileDescriptor, '    void process%sRegisterValues(const QVector<quint16> values);' % (propertyName[0].upper() + propertyName[1:]))

    writeLine(fileDescriptor)
    

def writePropertyProcessMethodImplementations(fileDescriptor, className, registerDefinitions):
    propertyVariables = []
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)

        writeLine(fileDescriptor, 'void %s::process%sRegisterValues(const QVector<quint16> values)' % (className, propertyName[0].upper() + propertyName[1:]))
        writeLine(fileDescriptor, '{')
        writeLine(fileDescriptor, '    %s received%s = %s;' % (propertyTyp, propertyName[0].upper() + propertyName[1:], getValueConversionMethod(registerDefinition)))
        writeLine(fileDescriptor, '    if (m_%s != received%s) {' % (propertyName, propertyName[0].upper() + propertyName[1:]))
        writeLine(fileDescriptor, '        m_%s = received%s;' % (propertyName, propertyName[0].upper() + propertyName[1:]))
        writeLine(fileDescriptor, '        emit %sChanged(m_%s);' % (propertyName, propertyName))
        writeLine(fileDescriptor, '    }')
        writeLine(fileDescriptor, '}')
        writeLine(fileDescriptor)
