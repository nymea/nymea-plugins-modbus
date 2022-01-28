#!/usr/bin/env python3

# Copyright (C) 2021  nymea GmbH <developer@nymea.io>
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

# To lazy to type all those register plugins, let's make live much easier and generate code from a json register definition 

import os
import re
import sys
import json
import shutil
import argparse
import datetime

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
    #print('--> words', words)
    finalWords = []

    for i in range(len(words)):
        camelCaseSplit = splitCamelCase(words[i])
        if len(camelCaseSplit) == 0:
            finalWords.append(words[i])  
        else:
            #print('--> camel split words', camelCaseSplit)
            for j in range(len(camelCaseSplit)):
                finalWords.append(camelCaseSplit[j])  

    if len(finalWords) == 0:
        return text

    finalText = ''
    if capitalize:
        finalText = finalWords[0].capitalize() + ''.join(i.capitalize() for i in finalWords[1:])
    else:
        finalText = finalWords[0].lower() + ''.join(i.capitalize() for i in finalWords[1:])
    #print('Convert camel case:', text, '-->', finalText)
    return finalText


def loadJsonFile(filePath):
    print('--> Loading JSON file', filePath)
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


def writeEnumDefinition(fileDescriptor, enumDefinition):
    print('Writing enum', enumDefinition)
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
            return ('ModbusDataUtils::convertFromUInt32(static_cast<%s>(%s), ModbusDataUtils::ByteOrder%s)' % (propertyTyp, propertyName, endianness))
        elif registerDefinition['type'] == 'int32':
            return ('ModbusDataUtils::convertFromInt32(static_cast<%s>(%s), ModbusDataUtils::ByteOrder%s)' % (propertyTyp, propertyName, endianness))

    # Handle scale factors
    if 'scaleFactor' in registerDefinition:
        scaleFactorProperty = 'm_%s' % registerDefinition['scaleFactor']
        if registerDefinition['type'] == 'uint16':
            return ('ModbusDataUtils::convertFromUInt16(static_cast<%s>(%s  * 1.0 / pow(10, %s)))' % (propertyTyp, propertyName, scaleFactorProperty))
        elif registerDefinition['type'] == 'int16':
            return ('ModbusDataUtils::convertFromInt16(static_cast<%s>(%s  * 1.0 / pow(10, %s)))' % (propertyTyp, propertyName, scaleFactorProperty))
        elif registerDefinition['type'] == 'uint32':
            return ('ModbusDataUtils::convertFromUInt32(static_cast<%s>(%s  * 1.0 / pow(10, %s)), ModbusDataUtils::ByteOrder%s)' % (propertyTyp, propertyName, scaleFactorProperty, endianness))
        elif registerDefinition['type'] == 'int32':
            return ('ModbusDataUtils::convertFromInt32(static_cast<%s>(%s  * 1.0 / pow(10, %s)), ModbusDataUtils::ByteOrder%s)' % (propertyTyp, propertyName, scaleFactorProperty, endianness))

    elif 'staticScaleFactor' in registerDefinition:
        scaleFactor = registerDefinition['staticScaleFactor']
        if registerDefinition['type'] == 'uint16':
            return ('ModbusDataUtils::convertFromUInt16(static_cast<%s>(%s  * 1.0 / pow(10, %s)))' % (propertyTyp, propertyName, scaleFactor))
        elif registerDefinition['type'] == 'int16':
            return ('ModbusDataUtils::convertFromInt16(static_cast<%s>(%s  * 1.0 / pow(10, %s)))' % (propertyTyp, propertyName, scaleFactor))
        elif registerDefinition['type'] == 'uint32':
            return ('ModbusDataUtils::convertFromUInt32(static_cast<%s>(%s  * 1.0 / pow(10, %s)), ModbusDataUtils::ByteOrder%s)' % (propertyTyp, propertyName, scaleFactor, endianness))
        elif registerDefinition['type'] == 'int32':
            return ('ModbusDataUtils::convertFromInt32(static_cast<%s>(%s  * 1.0 / pow(10, %s)), ModbusDataUtils::ByteOrder%s)' % (propertyTyp, propertyName, scaleFactor, endianness))

    # Handle default types
    elif registerDefinition['type'] == 'uint16':
        return ('ModbusDataUtils::convertFromUInt16(%s)' % propertyName)
    elif registerDefinition['type'] == 'int16':
        return ('ModbusDataUtils::convertFromInt16(%s)' % propertyName)
    elif registerDefinition['type'] == 'uint32':
        return ('ModbusDataUtils::convertFromUInt32(%s, ModbusDataUtils::ByteOrder%s)' % (propertyName, endianness))
    elif registerDefinition['type'] == 'int32':
        return ('ModbusDataUtils::convertFromInt32(%s, ModbusDataUtils::ByteOrder%s)' % (propertyName, endianness))
    elif registerDefinition['type'] == 'uint64':
        return ('ModbusDataUtils::convertFromUInt64(%s, ModbusDataUtils::ByteOrder%s)' % (propertyName, endianness))
    elif registerDefinition['type'] == 'int64':
        return ('ModbusDataUtils::convertFromInt64(%s, ModbusDataUtils::ByteOrder%s)' % (propertyName, endianness))
    elif registerDefinition['type'] == 'float':
        return ('ModbusDataUtils::convertFromFloat32(%s, ModbusDataUtils::ByteOrder%s)' % propertyName, endianness)
    elif registerDefinition['type'] == 'float64':
        return ('ModbusDataUtils::convertFromFloat64(%s, ModbusDataUtils::ByteOrder%s)' % propertyName, endianness)
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
            return ('static_cast<%s>(ModbusDataUtils::convertToUInt32(values, ModbusDataUtils::ByteOrder%s))' % (enumName, endianness))
        elif registerDefinition['type'] == 'int32':
            return ('static_cast<%s>(ModbusDataUtils::convertToInt32(values, ModbusDataUtils::ByteOrder%s))' % (enumName, endianness))

    # Handle scale factors
    if 'scaleFactor' in registerDefinition:
        scaleFactorProperty = 'm_%s' % registerDefinition['scaleFactor']
        if registerDefinition['type'] == 'uint16':
            return ('ModbusDataUtils::convertToUInt16(values) * 1.0 * pow(10, %s)' % (scaleFactorProperty))
        elif registerDefinition['type'] == 'int16':
            return ('ModbusDataUtils::convertToInt16(values) * 1.0 * pow(10, %s)' % (scaleFactorProperty))
        elif registerDefinition['type'] == 'uint32':
            return ('ModbusDataUtils::convertToUInt32(values, ModbusDataUtils::ByteOrder%s) * 1.0 * pow(10, %s)' % (endianness, scaleFactorProperty))
        elif registerDefinition['type'] == 'int32':
            return ('ModbusDataUtils::convertToInt32(values, ModbusDataUtils::ByteOrder%s) * 1.0 * pow(10, %s)' % (endianness, scaleFactorProperty))

    elif 'staticScaleFactor' in registerDefinition:
        scaleFactor = registerDefinition['staticScaleFactor']
        if registerDefinition['type'] == 'uint16':
            return ('ModbusDataUtils::convertToUInt16(values) * 1.0 * pow(10, %s)' % (scaleFactor))
        elif registerDefinition['type'] == 'int16':
            return ('ModbusDataUtils::convertToInt16(values) * 1.0 * pow(10, %s)' % (scaleFactor))
        elif registerDefinition['type'] == 'uint32':
            return ('ModbusDataUtils::convertToUInt32(values, ModbusDataUtils::ByteOrder%s) * 1.0 * pow(10, %s)' % (endianness, scaleFactor))
        elif registerDefinition['type'] == 'int32':
            return ('ModbusDataUtils::convertToInt32(values, ModbusDataUtils::ByteOrder%s) * 1.0 * pow(10, %s)' % (endianness, scaleFactor))

    # Handle default types
    elif registerDefinition['type'] == 'uint16':
        return ('ModbusDataUtils::convertToUInt16(values)')
    elif registerDefinition['type'] == 'int16':
        return ('ModbusDataUtils::convertToInt16(values)')
    elif registerDefinition['type'] == 'uint32':
        return ('ModbusDataUtils::convertToUInt32(values, ModbusDataUtils::ByteOrder%s)' % endianness)
    elif registerDefinition['type'] == 'int32':
        return ('ModbusDataUtils::convertToInt32(values, ModbusDataUtils::ByteOrder%s)' % endianness)
    elif registerDefinition['type'] == 'uint64':
        return ('ModbusDataUtils::convertToUInt64(values, ModbusDataUtils::ByteOrder%s)' % endianness)
    elif registerDefinition['type'] == 'int64':
        return ('ModbusDataUtils::convertToInt64(values, ModbusDataUtils::ByteOrder%s)' % endianness)
    elif registerDefinition['type'] == 'float':
        return ('ModbusDataUtils::convertToFloat32(values, ModbusDataUtils::ByteOrder%s)' % endianness)
    elif registerDefinition['type'] == 'float64':
        return ('ModbusDataUtils::convertToFloat64(values, ModbusDataUtils::ByteOrder%s)' % endianness)
    elif registerDefinition['type'] == 'string':
        return ('ModbusDataUtils::convertToString(values)')


def writePropertyGetSetMethodDeclarationsTcp(fileDescriptor, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)
        if 'unit' in registerDefinition and registerDefinition['unit'] != '':
            writeLine(fileDescriptor, '    /* %s [%s] - Address: %s, Size: %s */' % (registerDefinition['description'], registerDefinition['unit'], registerDefinition['address'], registerDefinition['size']))
        else:
            writeLine(fileDescriptor, '    /* %s - Address: %s, Size: %s */' % (registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))

        writeLine(fileDescriptor, '    %s %s() const;' % (propertyTyp, propertyName))

        # Check if we require a set method
        if registerDefinition['access'] == 'RW' or registerDefinition['access'] == 'WO':
            writeLine(fileDescriptor, '    QModbusReply *set%s(%s %s);' % (propertyName[0].upper() + propertyName[1:], propertyTyp, propertyName))

        writeLine(fileDescriptor)



def writePropertyGetSetMethodDeclarationsRtu(fileDescriptor, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)
        if 'unit' in registerDefinition and registerDefinition['unit'] != '':
            writeLine(fileDescriptor, '    /* %s [%s] - Address: %s, Size: %s */' % (registerDefinition['description'], registerDefinition['unit'], registerDefinition['address'], registerDefinition['size']))
        else:
            writeLine(fileDescriptor, '    /* %s - Address: %s, Size: %s */' % (registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))

        writeLine(fileDescriptor, '    %s %s() const;' % (propertyTyp, propertyName))

        # Check if we require a set method
        if registerDefinition['access'] == 'RW' or registerDefinition['access'] == 'WO':
            writeLine(fileDescriptor, '    ModbusRtuReply *set%s(%s %s);' % (propertyName[0].upper() + propertyName[1:], propertyTyp, propertyName))

        writeLine(fileDescriptor)


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


def writePropertyGetSetMethodImplementationsTcp(fileDescriptor, className, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)
        # Get
        if 'enum' in registerDefinition:
            writeLine(fileDescriptor, '%s::%s %s::%s() const' % (className, propertyTyp, className, propertyName))
        else:
            writeLine(fileDescriptor, '%s %s::%s() const' % (propertyTyp, className, propertyName))
    
        writeLine(fileDescriptor, '{')
        writeLine(fileDescriptor, '    return m_%s;' % propertyName)
        writeLine(fileDescriptor, '}')
        writeLine(fileDescriptor)

        # Check if we require a set method
        if registerDefinition['access'] == 'RW' or registerDefinition['access'] == 'WO':
            writeLine(fileDescriptor, 'QModbusReply *%s::set%s(%s %s)' % (className, propertyName[0].upper() + propertyName[1:], propertyTyp, propertyName))
            writeLine(fileDescriptor, '{')

            writeLine(fileDescriptor, '    QVector<quint16> values = %s;' % getConversionToValueMethod(registerDefinition))
            writeLine(fileDescriptor, '    qCDebug(dc%s()) << "--> Write \\"%s\\" register:" << %s << "size:" << %s << values;' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
            if registerDefinition['registerType'] == 'holdingRegister':
                writeLine(fileDescriptor, '    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, %s, values.count());' % (registerDefinition['address']))
            elif registerDefinition['registerType'] == 'coils':
                writeLine(fileDescriptor, '    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::Coils, %s, values.count());' % (registerDefinition['address']))
            else:
                print('Error: invalid register type for writing.')
                exit(1)

            writeLine(fileDescriptor, '    request.setValues(values);')
            writeLine(fileDescriptor, '    return sendWriteRequest(request, m_slaveId);')
            writeLine(fileDescriptor, '}')
            writeLine(fileDescriptor)


def writePropertyGetSetMethodImplementationsRtu(fileDescriptor, className, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)
        # Get
        if 'enum' in registerDefinition:
            writeLine(fileDescriptor, '%s::%s %s::%s() const' % (className, propertyTyp, className, propertyName))
        else:
            writeLine(fileDescriptor, '%s %s::%s() const' % (propertyTyp, className, propertyName))
    
        writeLine(fileDescriptor, '{')
        writeLine(fileDescriptor, '    return m_%s;' % propertyName)
        writeLine(fileDescriptor, '}')
        writeLine(fileDescriptor)

        # Check if we require a set method
        if registerDefinition['access'] == 'RW' or registerDefinition['access'] == 'WO':
            writeLine(fileDescriptor, 'ModbusRtuReply *%s::set%s(%s %s)' % (className, propertyName[0].upper() + propertyName[1:], propertyTyp, propertyName))
            writeLine(fileDescriptor, '{')

            writeLine(fileDescriptor, '    QVector<quint16> values = %s;' % getConversionToValueMethod(registerDefinition))
            writeLine(fileDescriptor, '    qCDebug(dc%s()) << "--> Write \\"%s\\" register:" << %s << "size:" << %s << values;' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
            if registerDefinition['registerType'] == 'holdingRegister':
                writeLine(fileDescriptor, '    return m_modbusRtuMaster->writeHoldingRegisters(m_slaveId, %s, values);' % (registerDefinition['address']))
            elif registerDefinition['registerType'] == 'coils':
                writeLine(fileDescriptor, '    return m_modbusRtuMaster->writeCoils(m_slaveId, %s, values);' % (registerDefinition['address']))
            else:
                print('Error: invalid register type for writing.')
                exit(1)

            writeLine(fileDescriptor, '}')
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
                    print('Error: block %s has invalid register order in register %s. There seems to be a gap between the registers.' % (blockName, blockRegister['id']))
                    exit(1)

                if blockRegister['access'] != registerAccess:
                    print('Error: block %s has inconsistent register access in register %s. The block registers dont seem to have the same access rights.' % (blockName, blockRegister['id']))
                    exit(1)

                if blockRegister['registerType'] != registerType:
                    print('Error: block %s has inconsistent register type in register %s. The block registers dont seem to be from the same type.' % (blockName, blockRegister['id']))
                    exit(1)

            registerCount += 1
            blockSize += blockRegister['size']

        print('Define valid block \"%s\" starting at %s with length %s containing %s properties to read.' % (blockName, blockStartAddress, blockSize, registerCount))


def writeBlocksUpdateMethodDeclarations(fileDescriptor, blockDefinitions):
    for blockDefinition in blockDefinitions:
        blockName = blockDefinition['id']
        blockRegisters = blockDefinition['registers']

        # Write the property get / set methods for the block registers
        writeBlockGetMethodDeclarations(fileDescriptor, blockRegisters)
        
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
                writeLine(fileDescriptor, '    -- %s - Address: %s, Size: %s' % (registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
        writeLine(fileDescriptor, '    */ ' )
        writeLine(fileDescriptor, '    void update%sBlock();' % (blockName[0].upper() + blockName[1:]))
    writeLine(fileDescriptor)


def writePropertyUpdateMethodImplementationsTcp(fileDescriptor, className, registerDefinitions):
    for registerDefinition in registerDefinitions:
        if 'readSchedule' in registerDefinition and registerDefinition['readSchedule'] == 'init':
            continue

        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)
        writeLine(fileDescriptor, 'void %s::update%s()' % (className, propertyName[0].upper() + propertyName[1:]))
        writeLine(fileDescriptor, '{')
        writeLine(fileDescriptor, '    // Update registers from %s' % registerDefinition['description'])
        writeLine(fileDescriptor, '    qCDebug(dc%s()) << "--> Read \\"%s\\" register:" << %s << "size:" << %s;' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
        writeLine(fileDescriptor, '    QModbusReply *reply = read%s();' % (propertyName[0].upper() + propertyName[1:]))
        writeLine(fileDescriptor, '    if (reply) {')
        writeLine(fileDescriptor, '        if (!reply->isFinished()) {')
        writeLine(fileDescriptor, '            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);')
        writeLine(fileDescriptor, '            connect(reply, &QModbusReply::finished, this, [this, reply](){')
        writeLine(fileDescriptor, '                if (reply->error() == QModbusDevice::NoError) {')
        writeLine(fileDescriptor, '                    const QModbusDataUnit unit = reply->result();')
        writeLine(fileDescriptor, '                    const QVector<quint16> values = unit.values();')
        writeLine(fileDescriptor, '                    qCDebug(dc%s()) << "<-- Response from \\"%s\\" register" << %s << "size:" << %s << values;' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
        writeLine(fileDescriptor, '                    %s received%s = %s;' % (propertyTyp, propertyName[0].upper() + propertyName[1:], getValueConversionMethod(registerDefinition)))
        writeLine(fileDescriptor, '                    if (m_%s != received%s) {' % (propertyName, propertyName[0].upper() + propertyName[1:]))
        writeLine(fileDescriptor, '                        m_%s = received%s;' % (propertyName, propertyName[0].upper() + propertyName[1:]))
        writeLine(fileDescriptor, '                        emit %sChanged(m_%s);' % (propertyName, propertyName))
        writeLine(fileDescriptor, '                    }')
        writeLine(fileDescriptor, '                }')
        writeLine(fileDescriptor, '            });')
        writeLine(fileDescriptor)
        writeLine(fileDescriptor, '            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){')
        writeLine(fileDescriptor, '                qCWarning(dc%s()) << "Modbus reply error occurred while updating \\"%s\\" registers from" << hostAddress().toString() << error << reply->errorString();' % (className, registerDefinition['description']))
        writeLine(fileDescriptor, '                emit reply->finished(); // To make sure it will be deleted')
        writeLine(fileDescriptor, '            });')
        writeLine(fileDescriptor, '        } else {')
        writeLine(fileDescriptor, '            delete reply; // Broadcast reply returns immediatly')
        writeLine(fileDescriptor, '        }')
        writeLine(fileDescriptor, '    } else {')
        writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Error occurred while reading \\"%s\\" registers from" << hostAddress().toString() << errorString();' % (className, registerDefinition['description']))
        writeLine(fileDescriptor, '    }')
        writeLine(fileDescriptor, '}')
        writeLine(fileDescriptor)


def writePropertyUpdateMethodImplementationsRtu(fileDescriptor, className, registerDefinitions):
    for registerDefinition in registerDefinitions:
        if 'readSchedule' in registerDefinition and registerDefinition['readSchedule'] == 'init':
            continue

        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)
        writeLine(fileDescriptor, 'void %s::update%s()' % (className, propertyName[0].upper() + propertyName[1:]))
        writeLine(fileDescriptor, '{')
        writeLine(fileDescriptor, '    // Update registers from %s' % registerDefinition['description'])
        writeLine(fileDescriptor, '    qCDebug(dc%s()) << "--> Read \\"%s\\" register:" << %s << "size:" << %s;' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))
        writeLine(fileDescriptor, '    ModbusRtuReply *reply = read%s();' % (propertyName[0].upper() + propertyName[1:]))
        writeLine(fileDescriptor, '    if (reply) {')
        writeLine(fileDescriptor, '        if (!reply->isFinished()) {')
        writeLine(fileDescriptor, '            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){')
        writeLine(fileDescriptor, '                if (reply->error() == ModbusRtuReply::NoError) {')
        writeLine(fileDescriptor, '                    QVector<quint16> values = reply->result();')
        writeLine(fileDescriptor, '                    qCDebug(dc%s()) << "<-- Response from \\"%s\\" register" << %s << "size:" << %s << values;' % (className, registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))

        # FIXME: introduce bool and check register type for parsing
        writeLine(fileDescriptor, '                    %s received%s = %s;' % (propertyTyp, propertyName[0].upper() + propertyName[1:], getValueConversionMethod(registerDefinition)))
        writeLine(fileDescriptor, '                    if (m_%s != received%s) {' % (propertyName, propertyName[0].upper() + propertyName[1:]))
        writeLine(fileDescriptor, '                        m_%s = received%s;' % (propertyName, propertyName[0].upper() + propertyName[1:]))
        writeLine(fileDescriptor, '                        emit %sChanged(m_%s);' % (propertyName, propertyName))
        writeLine(fileDescriptor, '                    }')
        writeLine(fileDescriptor, '                }')
        writeLine(fileDescriptor, '            });')
        writeLine(fileDescriptor)
        writeLine(fileDescriptor, '            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){')
        writeLine(fileDescriptor, '                qCWarning(dc%s()) << "ModbusRtu reply error occurred while updating \\"%s\\" registers" << error << reply->errorString();' % (className, registerDefinition['description']))
        writeLine(fileDescriptor, '                emit reply->finished();')
        writeLine(fileDescriptor, '            });')
        writeLine(fileDescriptor, '        }')
        writeLine(fileDescriptor, '    } else {')
        writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Error occurred while reading \\"%s\\" registers";' % (className, registerDefinition['description']))
        writeLine(fileDescriptor, '    }')
        writeLine(fileDescriptor, '}')
        writeLine(fileDescriptor)


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

        writeLine(fileDescriptor, '    if (reply) {')
        writeLine(fileDescriptor, '        if (!reply->isFinished()) {')
        writeLine(fileDescriptor, '            connect(reply, &ModbusRtuReply::finished, this, [this, reply](){')
        writeLine(fileDescriptor, '                if (reply->error() == ModbusRtuReply::NoError) {')
        writeLine(fileDescriptor, '                    QVector<quint16> blockValues = reply->result();')
        writeLine(fileDescriptor, '                    QVector<quint16> values;')
        writeLine(fileDescriptor, '                    qCDebug(dc%s()) << "<-- Response from reading block \\"%s\\" register" << %s << "size:" << %s << blockValues;' % (className, blockName, blockStartAddress, blockSize))
        
        # Start parsing the registers using offsets
        offset = 0
        for i, blockRegister in enumerate(blockRegisters):
            propertyName = blockRegister['id']
            propertyTyp = getCppDataType(blockRegister)
            writeLine(fileDescriptor, '                    values = blockValues.mid(%s, %s);' % (offset, blockRegister['size']))
            writeLine(fileDescriptor, '                    %s received%s = %s;' % (propertyTyp, propertyName[0].upper() + propertyName[1:], getValueConversionMethod(blockRegister)))
            writeLine(fileDescriptor, '                    if (m_%s != received%s) {' % (propertyName, propertyName[0].upper() + propertyName[1:]))
            writeLine(fileDescriptor, '                        m_%s = received%s;' % (propertyName, propertyName[0].upper() + propertyName[1:]))
            writeLine(fileDescriptor, '                        emit %sChanged(m_%s);' % (propertyName, propertyName))
            writeLine(fileDescriptor, '                    }')
            writeLine(fileDescriptor)
            offset += blockRegister['size']

        writeLine(fileDescriptor, '                }')
        writeLine(fileDescriptor, '            });')
        writeLine(fileDescriptor)
        writeLine(fileDescriptor, '            connect(reply, &ModbusRtuReply::errorOccurred, this, [reply] (ModbusRtuReply::Error error){')
        writeLine(fileDescriptor, '                qCWarning(dc%s()) << "ModbusRtu reply error occurred while updating block \\"%s\\" registers" << error << reply->errorString();' % (className, blockName))
        writeLine(fileDescriptor, '                emit reply->finished();')
        writeLine(fileDescriptor, '            });')
        writeLine(fileDescriptor, '        }')
        writeLine(fileDescriptor, '    } else {')
        writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Error occurred while reading block \\"%s\\" registers";' % (className, blockName))
        writeLine(fileDescriptor, '    }')
        writeLine(fileDescriptor, '}')
        writeLine(fileDescriptor)      


def writeBlockUpdateMethodImplementationsTcp(fileDescriptor, className, blockDefinitions):
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

        writeLine(fileDescriptor, '    QModbusReply *reply = sendReadRequest(request, m_slaveId);')

        writeLine(fileDescriptor, '    if (reply) {')
        writeLine(fileDescriptor, '        if (!reply->isFinished()) {')
        writeLine(fileDescriptor, '            connect(reply, &QModbusReply::finished, this, [this, reply](){')
        writeLine(fileDescriptor, '                if (reply->error() == QModbusDevice::NoError) {')
        writeLine(fileDescriptor, '                    const QModbusDataUnit unit = reply->result();')
        writeLine(fileDescriptor, '                    const QVector<quint16> blockValues = unit.values();')
        writeLine(fileDescriptor, '                    QVector<quint16> values;')
        writeLine(fileDescriptor, '                    qCDebug(dc%s()) << "<-- Response from reading block \\"%s\\" register" << %s << "size:" << %s << blockValues;' % (className, blockName, blockStartAddress, blockSize))

        # Start parsing the registers using offsets
        offset = 0
        for i, blockRegister in enumerate(blockRegisters):
            propertyName = blockRegister['id']
            propertyTyp = getCppDataType(blockRegister)
            writeLine(fileDescriptor, '                    values = blockValues.mid(%s, %s);' % (offset, blockRegister['size']))
            writeLine(fileDescriptor, '                    %s received%s = %s;' % (propertyTyp, propertyName[0].upper() + propertyName[1:], getValueConversionMethod(blockRegister)))
            writeLine(fileDescriptor, '                    if (m_%s != received%s) {' % (propertyName, propertyName[0].upper() + propertyName[1:]))
            writeLine(fileDescriptor, '                        m_%s = received%s;' % (propertyName, propertyName[0].upper() + propertyName[1:]))
            writeLine(fileDescriptor, '                        emit %sChanged(m_%s);' % (propertyName, propertyName))
            writeLine(fileDescriptor, '                    }')
            writeLine(fileDescriptor)
            offset += blockRegister['size']

        writeLine(fileDescriptor, '                }')
        writeLine(fileDescriptor, '            });')
        writeLine(fileDescriptor)
        writeLine(fileDescriptor, '            connect(reply, &QModbusReply::errorOccurred, this, [reply] (QModbusDevice::Error error){')
        writeLine(fileDescriptor, '                qCWarning(dc%s()) << "Modbus reply error occurred while updating block \\"%s\\" registers" << error << reply->errorString();' % (className, blockName))
        writeLine(fileDescriptor, '                emit reply->finished();')
        writeLine(fileDescriptor, '            });')
        writeLine(fileDescriptor, '        }')
        writeLine(fileDescriptor, '    } else {')
        writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Error occurred while reading block \\"%s\\" registers";' % (className, blockName))
        writeLine(fileDescriptor, '    }')
        writeLine(fileDescriptor, '}')
        writeLine(fileDescriptor)

def writeInternalPropertyReadMethodDeclarationsTcp(fileDescriptor, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        writeLine(fileDescriptor, '    QModbusReply *read%s();' % (propertyName[0].upper() + propertyName[1:]))


def writeInternalPropertyReadMethodDeclarationsRtu(fileDescriptor, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        writeLine(fileDescriptor, '    ModbusRtuReply *read%s();' % (propertyName[0].upper() + propertyName[1:]))


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

        writeLine(fileDescriptor, '    return sendReadRequest(request, m_slaveId);')
        writeLine(fileDescriptor, '}')
        writeLine(fileDescriptor)


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



def writePropertyChangedSignals(fileDescriptor, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)
        if propertyTyp == 'QString':
            writeLine(fileDescriptor, '    void %sChanged(const %s &%s);' % (propertyName, propertyTyp, propertyName))
        else:
            writeLine(fileDescriptor, '    void %sChanged(%s %s);' % (propertyName, propertyTyp, propertyName))


def writePrivatePropertyMembers(fileDescriptor, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)
        if 'defaultValue' in registerDefinition:
            writeLine(fileDescriptor, '    %s m_%s = %s;' % (propertyTyp, propertyName, registerDefinition['defaultValue']))
        else:
            writeLine(fileDescriptor, '    %s m_%s;' % (propertyTyp, propertyName))


def writeInitializeMethod(fileDescriptor, className, registerDefinitions):
    writeLine(fileDescriptor, 'void %s::initialize()' % (className))
    writeLine(fileDescriptor, '{')

    # First check if there are any init registers
    initRequired = False
    for registerDefinition in registerDefinitions:
        if registerDefinition['readSchedule'] == 'init':
            initRequired = True
            break

    if initRequired:
        # FIXME: distinguish between RTU and TCP 
        writeLine(fileDescriptor, '    QModbusReply *reply = nullptr;')
        writeLine(fileDescriptor)
        writeLine(fileDescriptor, '    if (!m_pendingInitReplies.isEmpty()) {')
        writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Tried to initialize but there are still some init replies pending.";' % className)
        writeLine(fileDescriptor, '        return;')
        writeLine(fileDescriptor, '    }')

        for registerDefinition in registerDefinitions:
            propertyName = registerDefinition['id']
            propertyTyp = getCppDataType(registerDefinition)

            if 'readSchedule' in registerDefinition and registerDefinition['readSchedule'] == 'init':
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '    // Read %s' % registerDefinition['description'])
                writeLine(fileDescriptor, '    reply = read%s();' % (propertyName[0].upper() + propertyName[1:]))
                writeLine(fileDescriptor, '    if (reply) {')
                writeLine(fileDescriptor, '        if (!reply->isFinished()) {')
                writeLine(fileDescriptor, '            m_pendingInitReplies.append(reply);')
                writeLine(fileDescriptor, '            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);')
                writeLine(fileDescriptor, '            connect(reply, &QModbusReply::finished, this, [this, reply](){')
                writeLine(fileDescriptor, '                if (reply->error() == QModbusDevice::NoError) {')
                writeLine(fileDescriptor, '                    const QModbusDataUnit unit = reply->result();')
                writeLine(fileDescriptor, '                    %s received%s = %s;' % (propertyTyp, propertyName[0].upper() + propertyName[1:], getValueConversionMethod(registerDefinition)))
                writeLine(fileDescriptor, '                    if (m_%s != received%s) {' % (propertyName, propertyName[0].upper() + propertyName[1:]))
                writeLine(fileDescriptor, '                        m_%s = received%s;' % (propertyName, propertyName[0].upper() + propertyName[1:]))
                writeLine(fileDescriptor, '                        emit %sChanged(m_%s);' % (propertyName, propertyName))
                writeLine(fileDescriptor, '                    }')
                writeLine(fileDescriptor, '                }')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '                m_pendingInitReplies.removeAll(reply);')
                writeLine(fileDescriptor, '                verifyInitFinished();')
                writeLine(fileDescriptor, '            });')
                writeLine(fileDescriptor)
                writeLine(fileDescriptor, '            connect(reply, &QModbusReply::errorOccurred, this, [this, reply] (QModbusDevice::Error error){')
                writeLine(fileDescriptor, '                qCWarning(dc%s()) << "Modbus reply error occurred while reading \\"%s\\" registers from" << hostAddress().toString() << error << reply->errorString();' % (className, registerDefinition['description']))
                writeLine(fileDescriptor, '                emit reply->finished(); // To make sure it will be deleted')
                writeLine(fileDescriptor, '            });')
                writeLine(fileDescriptor, '        } else {')
                writeLine(fileDescriptor, '            delete reply; // Broadcast reply returns immediatly')
                writeLine(fileDescriptor, '        }')
                writeLine(fileDescriptor, '    } else {')
                writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Error occurred while reading \\"%s\\" registers from" << hostAddress().toString() << errorString();' % (className, registerDefinition['description']))
                writeLine(fileDescriptor, '    }')


    else:
        writeLine(fileDescriptor, '    // No init registers defined. Nothing to be done and we are finished.')
        writeLine(fileDescriptor, '    emit initializationFinished();')

    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)


def writeUpdateMethod(fileDescriptor, className, registerDefinitions):
    writeLine(fileDescriptor, 'void %s::update()' % (className))
    writeLine(fileDescriptor, '{')
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        if 'readSchedule' in registerDefinition and registerDefinition['readSchedule'] == 'update':
            writeLine(fileDescriptor, '    update%s();' % (propertyName[0].upper() + propertyName[1:]))

    # Add the update block methods
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            blockName = blockDefinition['id']
            writeLine(fileDescriptor, '    update%sBlock();' % (blockName[0].upper() + blockName[1:]))

    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)


def writeRegistersDebugLine(fileDescriptor, debugObjectParamName, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)
        line = ('"    - %s:" << %s->%s()' % (registerDefinition['description'], debugObjectParamName, propertyName))
        if 'unit' in registerDefinition and registerDefinition['unit'] != '':
            line += (' << " [%s]"' % registerDefinition['unit'])
        writeLine(fileDescriptor, '    debug.nospace().noquote() << %s << "\\n";' % (line))


def writeTcpHeaderFile():
    print('Writing modbus TCP hader file %s' % headerFilePath)
    headerFile = open(headerFilePath, 'w')

    writeLicenseHeader(headerFile)
    writeLine(headerFile, '#ifndef %s_H' % className.upper())
    writeLine(headerFile, '#define %s_H' % className.upper())
    writeLine(headerFile)
    writeLine(headerFile, '#include <QObject>')
    writeLine(headerFile)
    writeLine(headerFile, '#include "../modbus/modbusdatautils.h"')
    writeLine(headerFile, '#include "../modbus/modbustcpmaster.h"')

    writeLine(headerFile)

    # Begin of class
    writeLine(headerFile, 'class %s : public ModbusTCPMaster' % className)
    writeLine(headerFile, '{')
    writeLine(headerFile, '    Q_OBJECT')

    # Public members
    writeLine(headerFile, 'public:')

    # Enum declarations
    if 'enums' in registerJson:
        for enumDefinition in registerJson['enums']:
            writeEnumDefinition(headerFile, enumDefinition)

    # Constructor
    writeLine(headerFile, '    explicit %s(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent = nullptr);' % className)
    writeLine(headerFile, '    ~%s() = default;' % className)
    writeLine(headerFile)

    # Write registers get method declarations
    writePropertyGetSetMethodDeclarationsTcp(headerFile, registerJson['registers'])

    # Write block get/set method declarations
    writeBlocksUpdateMethodDeclarations(headerFile, registerJson['blocks'])

    # Write init and update method declarations
    writeLine(headerFile, '    virtual void initialize();')
    writeLine(headerFile, '    virtual void update();')
    writeLine(headerFile)

    writePropertyUpdateMethodDeclarations(headerFile, registerJson['registers'])
    writeLine(headerFile)

    # Write registers value changed signals
    writeLine(headerFile, 'signals:')
    writeLine(headerFile, '    void initializationFinished();')
    writeLine(headerFile)
    writePropertyChangedSignals(headerFile, registerJson['registers'])
    for blockDefinition in registerJson['blocks']:
        writePropertyChangedSignals(headerFile, blockDefinition['registers'])
    writeLine(headerFile)

    # Protected members
    writeLine(headerFile, 'protected:')
    writeInternalPropertyReadMethodDeclarationsTcp(headerFile, registerJson['registers'])
    writeLine(headerFile)

    # Private members
    writeLine(headerFile, 'private:')
    writeLine(headerFile, '    quint16 m_slaveId = 1;')
    writeLine(headerFile, '    QVector<QModbusReply *> m_pendingInitReplies;')
    writeLine(headerFile)
    writePrivatePropertyMembers(headerFile, registerJson['registers'])
    for blockDefinition in registerJson['blocks']:
        writePrivatePropertyMembers(headerFile, blockDefinition['registers'])
    writeLine(headerFile)
    writeLine(headerFile, '    void verifyInitFinished();')
    writeLine(headerFile)


    # End of class
    writeLine(headerFile)
    writeLine(headerFile, '};')
    writeLine(headerFile)
    writeLine(headerFile, 'QDebug operator<<(QDebug debug, %s *%s);' % (className, className[0].lower() + className[1:]))
    writeLine(headerFile)
    writeLine(headerFile, '#endif // %s_H' % className.upper())

    headerFile.close()


def writeTcpSourceFile():
    print('Writing modbus TCP source file %s' % sourceFilePath)
    sourceFile = open(sourceFilePath, 'w')
    writeLicenseHeader(sourceFile)
    writeLine(sourceFile)
    writeLine(sourceFile, '#include "%s"' % headerFileName)
    writeLine(sourceFile, '#include "loggingcategories.h"')
    writeLine(sourceFile)
    writeLine(sourceFile, 'NYMEA_LOGGING_CATEGORY(dc%s, "%s")' % (className, className))
    writeLine(sourceFile)

    # Constructor
    writeLine(sourceFile, '%s::%s(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent) :' % (className, className))
    writeLine(sourceFile, '    ModbusTCPMaster(hostAddress, port, parent),')
    writeLine(sourceFile, '    m_slaveId(slaveId)')
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    ')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    # Property get methods
    writePropertyGetSetMethodImplementationsTcp(sourceFile, className, registerJson['registers'])

    # Block property get methods
    for blockDefinition in registerJson['blocks']:
        writePropertyGetSetMethodImplementationsTcp(sourceFile, className, blockDefinition['registers'])

    # Write init and update method implementation
    writeInitializeMethod(sourceFile, className, registerJson['registers'])
    writeUpdateMethod(sourceFile, className, registerJson['registers'])

    # Write update methods
    writePropertyUpdateMethodImplementationsTcp(sourceFile, className, registerJson['registers'])

   # Write block update method
    writeBlockUpdateMethodImplementationsTcp(sourceFile, className, registerJson['blocks'])

    # Write internal protected property read method implementations
    writeInternalPropertyReadMethodImplementationsTcp(sourceFile, className, registerJson['registers'])

    writeLine(sourceFile, 'void %s::verifyInitFinished()' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    if (m_pendingInitReplies.isEmpty()) {')
    writeLine(sourceFile, '        qCDebug(dc%s()) << "Initialization finished of %s" << hostAddress().toString();' % (className, className))
    writeLine(sourceFile, '        emit initializationFinished();')
    writeLine(sourceFile, '    }')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    # Write the debug print
    debugObjectParamName = className[0].lower() + className[1:]
    writeLine(sourceFile, 'QDebug operator<<(QDebug debug, %s *%s)' % (className, debugObjectParamName))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    debug.nospace().noquote() << "%s(" << %s->hostAddress().toString() << ":" << %s->port() << ")" << "\\n";' % (className, debugObjectParamName, debugObjectParamName))
    writeRegistersDebugLine(sourceFile, debugObjectParamName, registerJson['registers'])

    for blockDefinition in registerJson['blocks']:
        writeRegistersDebugLine(sourceFile, debugObjectParamName, blockDefinition['registers'])

    writeLine(sourceFile, '    return debug.quote().space();')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    sourceFile.close()


##########################################################################################################
def writeRtuHeaderFile():
    print('Writing modbus TCP hader file %s' % headerFilePath)
    headerFile = open(headerFilePath, 'w')

    writeLicenseHeader(headerFile)
    writeLine(headerFile, '#ifndef %s_H' % className.upper())
    writeLine(headerFile, '#define %s_H' % className.upper())
    writeLine(headerFile)
    writeLine(headerFile, '#include <QObject>')
    writeLine(headerFile)
    writeLine(headerFile, '#include "../modbus/modbusdatautils.h"')
    writeLine(headerFile, '#include <hardware/modbus/modbusrtumaster.h>')

    writeLine(headerFile)

    # Begin of class
    writeLine(headerFile, 'class %s : public QObject' % className)
    writeLine(headerFile, '{')
    writeLine(headerFile, '    Q_OBJECT')

    # Public members
    writeLine(headerFile, 'public:')

    # Enum declarations
    if 'enums' in registerJson:
        for enumDefinition in registerJson['enums']:
            writeEnumDefinition(headerFile, enumDefinition)

    # Constructor
    writeLine(headerFile, '    explicit %s(ModbusRtuMaster *modbusRtuMaster, quint16 slaveId, QObject *parent = nullptr);' % className)
    writeLine(headerFile, '    ~%s() = default;' % className)
    writeLine(headerFile)

    writeLine(headerFile, '    ModbusRtuMaster *modbusRtuMaster() const;')
    writeLine(headerFile, '    quint16 slaveId() const;')
    writeLine(headerFile)

    # Write registers get/set method declarations
    writePropertyGetSetMethodDeclarationsRtu(headerFile, registerJson['registers'])

    # Write block get/set method declarations
    writeBlocksUpdateMethodDeclarations(headerFile, registerJson['blocks'])

    writePropertyUpdateMethodDeclarations(headerFile, registerJson['registers'])
    writeLine(headerFile)

    # Write init and update method declarations
    writeLine(headerFile, '    virtual void initialize();')
    writeLine(headerFile, '    virtual void update();')
    writeLine(headerFile)

    # Write registers value changed signals
    writeLine(headerFile, 'signals:')
    writeLine(headerFile, '    void initializationFinished();')
    writeLine(headerFile)
    writePropertyChangedSignals(headerFile, registerJson['registers'])
    for blockDefinition in registerJson['blocks']:
        writePropertyChangedSignals(headerFile, blockDefinition['registers'])
    writeLine(headerFile)

    # Protected members
    writeLine(headerFile, 'protected:')
    writeInternalPropertyReadMethodDeclarationsRtu(headerFile, registerJson['registers'])
    writeLine(headerFile)

    # Private members
    writeLine(headerFile, 'private:')
    writeLine(headerFile, '    ModbusRtuMaster *m_modbusRtuMaster = nullptr;')
    writeLine(headerFile, '    quint16 m_slaveId = 1;')
    writeLine(headerFile, '    QVector<ModbusRtuReply *> m_pendingInitReplies;')
    writeLine(headerFile)
    writePrivatePropertyMembers(headerFile, registerJson['registers'])
    for blockDefinition in registerJson['blocks']:
        writePrivatePropertyMembers(headerFile, blockDefinition['registers'])

    writeLine(headerFile)
    writeLine(headerFile, '    void verifyInitFinished();')
    writeLine(headerFile)
    
    # End of class
    writeLine(headerFile)
    writeLine(headerFile, '};')
    writeLine(headerFile)
    writeLine(headerFile, 'QDebug operator<<(QDebug debug, %s *%s);' % (className, className[0].lower() + className[1:]))
    writeLine(headerFile)
    writeLine(headerFile, '#endif // %s_H' % className.upper())

    headerFile.close()


def writeRtuSourceFile():
    print('Writing modbus RTU source file %s' % sourceFilePath)
    sourceFile = open(sourceFilePath, 'w')
    writeLicenseHeader(sourceFile)

    writeLine(sourceFile, '#include "%s"' % headerFileName)
    writeLine(sourceFile, '#include "loggingcategories.h"')
    writeLine(sourceFile)
    writeLine(sourceFile, 'NYMEA_LOGGING_CATEGORY(dc%s, "%s")' % (className, className))
    writeLine(sourceFile)

    # Constructor
    writeLine(sourceFile, '%s::%s(ModbusRtuMaster *modbusRtuMaster, quint16 slaveId, QObject *parent) :' % (className, className))
    writeLine(sourceFile, '    QObject(parent),')
    writeLine(sourceFile, '    m_modbusRtuMaster(modbusRtuMaster),')
    writeLine(sourceFile, '    m_slaveId(slaveId)')
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    ')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    writeLine(sourceFile, 'ModbusRtuMaster *%s::modbusRtuMaster() const' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    return m_modbusRtuMaster;')
    writeLine(sourceFile, '}')

    writeLine(sourceFile, 'quint16 %s::slaveId() const' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    return m_slaveId;')
    writeLine(sourceFile, '}')


    # Property get methods
    writePropertyGetSetMethodImplementationsRtu(sourceFile, className, registerJson['registers'])

    # Block property get methods
    for blockDefinition in registerJson['blocks']:
        writePropertyGetSetMethodImplementationsRtu(sourceFile, className, blockDefinition['registers'])

    # Write init and update method implementation
    writeInitializeMethod(sourceFile, className, registerJson['registers'])
    writeUpdateMethod(sourceFile, className, registerJson['registers'])

    # Write update methods
    writePropertyUpdateMethodImplementationsRtu(sourceFile, className, registerJson['registers'])

    # Write block update method
    writeBlockUpdateMethodImplementationsRtu(sourceFile, className, registerJson['blocks'])

    # Write internal protected property read method implementations
    writeInternalPropertyReadMethodImplementationsRtu(sourceFile, className, registerJson['registers'])

    writeLine(sourceFile, 'void %s::verifyInitFinished()' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    if (m_pendingInitReplies.isEmpty()) {')
    writeLine(sourceFile, '        qCDebug(dc%s()) << "Initialization finished of %s";' % (className, className))
    writeLine(sourceFile, '        emit initializationFinished();')
    writeLine(sourceFile, '    }')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    # Write the debug print
    debugObjectParamName = className[0].lower() + className[1:]
    writeLine(sourceFile, 'QDebug operator<<(QDebug debug, %s *%s)' % (className, debugObjectParamName))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    debug.nospace().noquote() << "%s(" << %s->modbusRtuMaster()->modbusUuid().toString() << ", " << %s->modbusRtuMaster()->serialPort() << ", slave ID:" << %s->slaveId() << ")" << "\\n";' % (className, debugObjectParamName, debugObjectParamName, debugObjectParamName))
    writeRegistersDebugLine(sourceFile, debugObjectParamName, registerJson['registers'])

    for blockDefinition in registerJson['blocks']:
        writeRegistersDebugLine(sourceFile, debugObjectParamName, blockDefinition['registers'])

    writeLine(sourceFile, '    return debug.quote().space();')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    sourceFile.close()


############################################################################################
# Main
############################################################################################

parser = argparse.ArgumentParser(description='Generate modbus tcp connection class from JSON register definitions file.')
parser.add_argument('-j', '--json', metavar='<file>', help='The JSON file containing the register definitions.')
parser.add_argument('-o', '--output-directory', metavar='<directory>', help='The output directory for the resulting class.')
parser.add_argument('-c', '--class-name', metavar='<name>', help='The name of the resulting class.')
args = parser.parse_args()

registerJson = loadJsonFile(args.json)
scriptPath = os.path.dirname(os.path.realpath(sys.argv[0]))
outputDirectory = os.path.realpath(args.output_directory)
className = args.class_name

headerFileName = className.lower() + '.h'
sourceFileName = className.lower() + '.cpp'

headerFilePath = os.path.join(outputDirectory, headerFileName)
sourceFilePath = os.path.join(outputDirectory, sourceFileName)

print('Scrip path: %s' % scriptPath)
print('Output directory: %s' % outputDirectory)
print('Class name: %s' % className)
print('Header file: %s' % headerFileName)
print('Source file: %s' % sourceFileName)
print('Header file path: %s' % headerFilePath)
print('Source file path: %s' % sourceFilePath)

endianness = 'BigEndian'
if 'endianness' in registerJson:
    endianness = registerJson['endianness']

protocol = 'TCP'
if 'protocol' in registerJson:
    protocol = registerJson['protocol']

validateBlocks(registerJson['blocks'])

if protocol == 'TCP':
    writeTcpHeaderFile()
    writeTcpSourceFile()
else:
    writeRtuHeaderFile()
    writeRtuSourceFile()



