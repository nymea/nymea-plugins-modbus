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


def getCppDataType(registerDefinition):
    if 'enum' in registerDefinition:
        return registerDefinition['enum']

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


def getValueConversionMethod(registerDefinition):
    if 'enum' in registerDefinition:
        enumName = registerDefinition['enum']
        if registerDefinition['type'] == 'uint16':
            return ('static_cast<%s>(ModbusDataUtils::convertToUInt16(registers))' % (enumName))
        elif registerDefinition['type'] == 'int16':
            return ('static_cast<%s>(ModbusDataUtils::convertToInt16(registers))' % (enumName))
        elif registerDefinition['type'] == 'uint32':
            return ('static_cast<%s>(ModbusDataUtils::convertToUInt32(registers))' % (enumName))
        elif registerDefinition['type'] == 'int32':
            return ('static_cast<%s>(ModbusDataUtils::convertToInt32(registers))' % (enumName))

    elif registerDefinition['type'] == 'uint16':
        return ('ModbusDataUtils::convertToUInt16(registers)')
    elif registerDefinition['type'] == 'int16':
        return ('ModbusDataUtils::convertToInt16(registers)')
    elif registerDefinition['type'] == 'uint32':
        return ('ModbusDataUtils::convertToUInt32(registers)')
    elif registerDefinition['type'] == 'int32':
        return ('ModbusDataUtils::convertToInt32(registers)')
    elif registerDefinition['type'] == 'uint64':
        return ('ModbusDataUtils::convertToUInt64(registers)')
    elif registerDefinition['type'] == 'int64':
        return ('ModbusDataUtils::convertToInt64(registers)')
    elif registerDefinition['type'] == 'float':
        return ('ModbusDataUtils::convertToFloat32(registers)')
    elif registerDefinition['type'] == 'float64':
        return ('ModbusDataUtils::convertToFloat64(registers)')
    elif registerDefinition['type'] == 'string':
        return ('ModbusDataUtils::convertToString(registers)')


def writePropertyGetMethodDeclarations(fileDescriptor, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)
        if 'unit' in registerDefinition and registerDefinition['unit'] != '':
            writeLine(fileDescriptor, '    /* %s [%s] - Address: %s, Size: %s */' % (registerDefinition['description'], registerDefinition['unit'], registerDefinition['address'], registerDefinition['size']))
        else:
            writeLine(fileDescriptor, '    /* %s - Address: %s, Size: %s */' % (registerDefinition['description'], registerDefinition['address'], registerDefinition['size']))

        writeLine(fileDescriptor, '    %s %s() const;' % (propertyTyp, propertyName))
        writeLine(fileDescriptor)


def writePropertyGetMethodImplementations(fileDescriptor, className, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)
        if 'enum' in registerDefinition:
            writeLine(fileDescriptor, '%s::%s %s::%s() const' % (className, propertyTyp, className, propertyName))
        else:
            writeLine(fileDescriptor, '%s %s::%s() const' % (propertyTyp, className, propertyName))
    
        writeLine(fileDescriptor, '{')
        writeLine(fileDescriptor, '    return m_%s;' % propertyName)
        writeLine(fileDescriptor, '}')
        writeLine(fileDescriptor)


def writePropertyUpdateMethodDeclarations(fileDescriptor, registerDefinitions):
    for registerDefinition in registerDefinitions:
        if registerDefinition['readSchedule'] == 'init':
            continue

        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)
        writeLine(fileDescriptor, '    void update%s();' % (propertyName[0].upper() + propertyName[1:]))


def writePropertyUpdateMethodImplementations(fileDescriptor, className, registerDefinitions):
    for registerDefinition in registerDefinitions:
        if registerDefinition['readSchedule'] == 'init':
            continue

        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)
        writeLine(fileDescriptor, 'void %s::update%s()' % (className, propertyName[0].upper() + propertyName[1:]))
        writeLine(fileDescriptor, '{')
        writeLine(fileDescriptor, '    // Update registers from %s' % registerDefinition['description'])
        writeLine(fileDescriptor, '    QModbusReply *reply = read%s();' % (propertyName[0].upper() + propertyName[1:]))
        writeLine(fileDescriptor, '    if (reply) {')
        writeLine(fileDescriptor, '        if (!reply->isFinished()) {')
        writeLine(fileDescriptor, '            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);')
        writeLine(fileDescriptor, '            connect(reply, &QModbusReply::finished, this, [this, reply](){')
        writeLine(fileDescriptor, '                if (reply->error() == QModbusDevice::NoError) {')
        writeLine(fileDescriptor, '                    const QModbusDataUnit unit = reply->result();')
        writeLine(fileDescriptor, '                    const QVector<quint16> registers = unit.values();')
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


def writeInternalPropertyReadMethodDeclarations(fileDescriptor, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        writeLine(fileDescriptor, '    QModbusReply *read%s();' % (propertyName[0].upper() + propertyName[1:]))


def writeInternalPropertyReadMethodImplementations(fileDescriptor, className, registerDefinitions):
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        writeLine(fileDescriptor, 'QModbusReply *%s::read%s()' % (className, propertyName[0].upper() + propertyName[1:]))
        writeLine(fileDescriptor, '{')
        writeLine(fileDescriptor, '    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, %s, %s);' % (registerDefinition['address'], registerDefinition['size']))
        writeLine(fileDescriptor, '    return sendReadRequest(request, m_slaveId);')
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
        writeLine(fileDescriptor, '    %s m_%s;' % (propertyTyp, propertyName))


def writeInitializeMethod(fileDescriptor, className, registerDefinitions):
    writeLine(fileDescriptor, 'void %s::initialize()' % (className))
    writeLine(fileDescriptor, '{')

    writeLine(fileDescriptor, '    QModbusReply *reply = nullptr;')
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '    if (!m_pendingInitReplies.isEmpty()) {')
    writeLine(fileDescriptor, '        qCWarning(dc%s()) << "Tried to initialize but there are still some init replies pending.";' % className)
    writeLine(fileDescriptor, '        return;')
    writeLine(fileDescriptor, '    }')
    writeLine(fileDescriptor)

    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        propertyTyp = getCppDataType(registerDefinition)

        if registerDefinition['readSchedule'] != 'init':
            continue

        writeLine(fileDescriptor, '    // Read %s' % registerDefinition['description'])
        writeLine(fileDescriptor, '    reply = read%s();' % (propertyName[0].upper() + propertyName[1:]))
        writeLine(fileDescriptor, '    if (reply) {')
        writeLine(fileDescriptor, '        if (!reply->isFinished()) {')
        writeLine(fileDescriptor, '            m_pendingInitReplies.append(reply);')
        writeLine(fileDescriptor, '            connect(reply, &QModbusReply::finished, reply, &QModbusReply::deleteLater);')
        writeLine(fileDescriptor, '            connect(reply, &QModbusReply::finished, this, [this, reply](){')
        writeLine(fileDescriptor, '                if (reply->error() == QModbusDevice::NoError) {')
        writeLine(fileDescriptor, '                    const QModbusDataUnit unit = reply->result();')
        writeLine(fileDescriptor, '                    const QVector<quint16> registers = unit.values();')
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
        writeLine(fileDescriptor)

    writeLine(fileDescriptor, '    ')
    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)


def writeUpdateMethod(fileDescriptor, className, registerDefinitions):
    writeLine(fileDescriptor, 'void %s::update()' % (className))
    writeLine(fileDescriptor, '{')
    for registerDefinition in registerDefinitions:
        propertyName = registerDefinition['id']
        if registerDefinition['readSchedule'] == 'update':
            writeLine(fileDescriptor, '    update%s();' % (propertyName[0].upper() + propertyName[1:]))
  
    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)


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

#############################################################################
# Write header file
#############################################################################

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
for enumDefinition in registerJson['enums']:
    writeEnumDefinition(headerFile, enumDefinition)

# Constructor
writeLine(headerFile, '    explicit %s(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent = nullptr);' % className)
writeLine(headerFile, '    ~%s() = default;' % className)
writeLine(headerFile)

# Write registers get method declarations
writePropertyGetMethodDeclarations(headerFile, registerJson['registers'])

# Write init and update method declarations
writeLine(headerFile, '    void initialize();')
writeLine(headerFile, '    void update();')
writeLine(headerFile)

writePropertyUpdateMethodDeclarations(headerFile, registerJson['registers'])
writeLine(headerFile)

# Write registers value changed signals
writeLine(headerFile, 'signals:')
writeLine(headerFile, '    void initializationFinished();')
writeLine(headerFile)
writePropertyChangedSignals(headerFile, registerJson['registers'])
writeLine(headerFile)

# Private members
writeLine(headerFile, 'private:')
writeLine(headerFile, '    quint16 m_slaveId = 1;')
writeLine(headerFile, '    QVector<QModbusReply *> m_pendingInitReplies;')
writeLine(headerFile)
writePrivatePropertyMembers(headerFile, registerJson['registers'])
writeLine(headerFile)
writeLine(headerFile, '    void verifyInitFinished();')
writeLine(headerFile)
writeInternalPropertyReadMethodDeclarations(headerFile, registerJson['registers'])
writeLine(headerFile)

# End of class
writeLine(headerFile)
writeLine(headerFile, '};')
writeLine(headerFile)
writeLine(headerFile, '#endif // %s_H' % className.upper())

headerFile.close()



#############################################################################
# Write source file
#############################################################################

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
writePropertyGetMethodImplementations(sourceFile, className, registerJson['registers'])

# Write init and update method implementation
writeInitializeMethod(sourceFile, className, registerJson['registers'])
writeUpdateMethod(sourceFile, className, registerJson['registers'])

# Write update methods
writePropertyUpdateMethodImplementations(sourceFile, className, registerJson['registers'])

# Write property read method implementations
writeInternalPropertyReadMethodImplementations(sourceFile, className, registerJson['registers'])

writeLine(sourceFile, 'void %s::verifyInitFinished()' % (className))
writeLine(sourceFile, '{')
writeLine(sourceFile, '    if (m_pendingInitReplies.isEmpty()) {')
writeLine(sourceFile, '        qCDebug(dc%s()) << "Initialization finished of %s" << hostAddress().toString();' % (className, className))
writeLine(sourceFile, '        emit initializationFinished();')
writeLine(sourceFile, '    }')
writeLine(sourceFile, '}')
writeLine(sourceFile)


sourceFile.close()