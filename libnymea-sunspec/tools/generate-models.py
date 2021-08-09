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


def testCamelCase():
    convertToCamelCase('Test-foo-Bar')
    convertToCamelCase('BLA_FOO_BAR')
    convertToCamelCase('BLA_FOO_BAR', True)
    convertToCamelCase('ac_meter', True)
    convertToCamelCase('model_123', True)
    convertToCamelCase('model 12abc ASDnndnDksndf', True)
    convertToCamelCase('AbcDef_GhiJklMnoPqr Stu-vw', True)
    convertToCamelCase('Inverter (Single Phase) FLOAT', True)
    convertToCamelCase('Inverter (Single Phase) FLOAT 345', True)


def init():
    print('Initializing model class generator...')
    if os.path.exists(outputDirectory):
        print('Clean up output directory', outputDirectory)
        shutil.rmtree(outputDirectory)

    print('Creating output directory', outputDirectory)
    os.mkdir(outputDirectory)


def loadModel(modelFilePath):
    print('--> Loading model', modelFilePath)
    modelFile = open(modelFilePath, 'r')
    modelData = json.load(modelFile)
    models[modelData['id']] = modelData
    

def loadModels():
    modelFiles = os.listdir(modelJsonFolder)
    for modelFileName in modelFiles:
        if modelFileName.startswith('model_'):
            modelFilePath = modelJsonFolder + '/' + modelFileName
            loadModel(modelFilePath)

    for key, value in sorted(models.items(), key=lambda item: int(item[0])):
        print(key, value['group']['name'], value['group']['label'])


def loadGroups():
    groups = {}
    print('--> Read groups...')
    for key, value in sorted(models.items(), key=lambda item: int(item[0])):
        groupName = value['group']['name']

        # Filter out unspecific / unneeded models...
        if groupName.startswith('model_') or groupName.startswith('DER'):
            continue

        if value['group']['name'] == 'bom_temp' or value['group']['name'] == 'inclinometer':
            continue

        if groupName in groups.keys():
            ids = []
            for i in range(len(groups[groupName])):
                ids.append(groups[groupName][i])

            ids.append(key)
            groups[groupName] = ids
        else:
            ids = []
            ids.append(key)
            groups[groupName] = ids
    
    for key, value in sorted(groups.items(), key=lambda item: item[0]):
        print(convertToCamelCase(key, True), '-->', value)

    return groups


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


def writeClassEnums(fileDescriptor, className, points):
    print('--> Writing enums for', className)
    enumDefinitions = {} # name, symbols[]

    # Get unique enums
    for pointData in points:
        if 'type' in pointData and pointData['type'].startswith('enum') and 'symbols' in pointData:
            #print('Found enum in point data:', pointData['name'], pointData['symbols'])
            enumName = getEnumName(pointData)
            if enumName in enumDefinitions.keys():
                continue
            else:
                enumDefinitions[enumName] = pointData['symbols']

    # Write actual enum declaration
    for enumDefinition in enumDefinitions:
        symbolList = enumDefinitions[enumDefinition]
        #print('--> ######## ENUM for', modelData['id'], '-->', enumDefinition, symbolList)
        writeLine(fileDescriptor, '    enum %s {' % enumDefinition)
        for i in range(len(symbolList)):
            symbol = symbolList[i]
            valueName = convertToCamelCase(symbol['name'], True)
            line = ('        %s%s = %s' % (enumDefinition, valueName, symbol['value']))
            if i < (len(symbolList) - 1):
                line += ','

            writeLine(fileDescriptor, line)

        writeLine(fileDescriptor, '    };')
        writeLine(fileDescriptor, '    Q_ENUM(%s)' % enumDefinition)
        writeLine(fileDescriptor)


def writeClassFlags(fileDescriptor, className, points):
    print('--> Writing flags for', className)
    enumDefinitions = {} # name, symbols[]

    # Get unique flags
    for pointData in points:
        if 'type' in pointData and pointData['type'].startswith('bitfield') and 'symbols' in pointData:
            #print('Found enum in point data:', pointData['name'], pointData['symbols'])
            enumName = getEnumName(pointData)
            if enumName in enumDefinitions.keys():
                continue
            else:
                enumDefinitions[enumName] = pointData['symbols']

    # Write actual enum declaration
    for enumDefinition in enumDefinitions:
        symbolList = enumDefinitions[enumDefinition]
        #print('--> ######## ENUM for', modelData['id'], '-->', enumDefinition, symbolList)
        writeLine(fileDescriptor, '    enum %s {' % enumDefinition)
        for i in range(len(symbolList)):
            symbol = symbolList[i]
            valueName = convertToCamelCase(symbol['name'], True)
            value = 0 | (1 << symbol['value'])
            line = ('        %s%s = %s' % (enumDefinition, valueName, hex(value)))
            if i < (len(symbolList) - 1):
                line += ','

            writeLine(fileDescriptor, line)

        writeLine(fileDescriptor, '    };')
        writeLine(fileDescriptor, '    Q_DECLARE_FLAGS(%sFlags, %s)' % (enumDefinition, enumDefinition))
        writeLine(fileDescriptor, '    Q_FLAG(%s)' % enumDefinition)
        writeLine(fileDescriptor)


def addDataPointInitialization(fileDescriptor, dataPoint, addressOffset, indentation = 1):
    linePrepend = ''
    for i in range(indentation):
        linePrepend += '    '

    # Get property name
    propertyName = getPropertyName(dataPoint) + 'DataPoint'

    writeLine(fileDescriptor, linePrepend + 'SunSpecDataPoint %s;' % propertyName)
    writeLine(fileDescriptor, linePrepend + '%s.setName("%s");' % (propertyName, dataPoint['name']))
    
    if 'label' in dataPoint:
        writeLine(fileDescriptor, linePrepend + '%s.setLabel("%s");' % (propertyName, dataPoint['label']))

    if 'desc' in dataPoint:
        writeLine(fileDescriptor, linePrepend + '%s.setDescription("%s");' % (propertyName, dataPoint['desc']))

    if 'detail' in dataPoint:
        writeLine(fileDescriptor, linePrepend + '%s.setDetail("%s");' % (propertyName, dataPoint['detail']))

    if 'units' in dataPoint:
        writeLine(fileDescriptor, linePrepend + '%s.setUnits("%s");' % (propertyName, dataPoint['units']))

    if 'mandatory' in dataPoint and dataPoint['mandatory'] == 'M':
        writeLine(fileDescriptor, linePrepend + '%s.setMandatory(true);' % (propertyName))

    size = 0
    if 'size' in dataPoint:
        size = int(dataPoint['size'])
        writeLine(fileDescriptor, linePrepend + '%s.setSize(%s);' % (propertyName, size))

    writeLine(fileDescriptor, linePrepend + '%s.setAddressOffset(%s);' % (propertyName, addressOffset))
    blockOffset = addressOffset - 2
    if blockOffset >= 0:
        writeLine(fileDescriptor, linePrepend + '%s.setBlockOffset(%s);' % (propertyName, blockOffset))

    if 'sf' in dataPoint:
        writeLine(fileDescriptor, linePrepend + '%s.setScaleFactorName("%s");' % (propertyName, dataPoint['sf']))

    if 'type' in dataPoint:
        writeLine(fileDescriptor, linePrepend + '%s.setSunSpecDataType("%s");' % (propertyName, dataPoint['type']))

    if 'access' in dataPoint and dataPoint['access'] == 'RW':
        writeLine(fileDescriptor, linePrepend + '%s.setAccess(SunSpecDataPoint::AccessReadWrite);' % (propertyName))

    writeLine(fileDescriptor, linePrepend + 'm_dataPoints.insert(%s.name(), %s);' % (propertyName, propertyName))
    writeLine(fileDescriptor)
    return size


def getEnumName(dataPoint):
    return dataPoint['name'].capitalize()


def getPropertyName(dataPoint):
    # Get property name
    #if 'desc' in dataPoint and 'type' in dataPoint and not (dataPoint['type'].startswith('enum') or dataPoint['type'].startswith('bitfield')):
    #    propertyName = convertToCamelCase(dataPoint['desc'])
    if 'label' in dataPoint:
        propertyName = convertToCamelCase(dataPoint['label'])
    else:
        propertyName = dataPoint['name']

    # Correction for key words
    if propertyName == 'long':
        propertyName = 'longitude'

    if propertyName == 'pad':
        propertyName = dataPoint['name']


    if propertyName == 'event':
        propertyName = 'eventFlags'

    return propertyName[0].lower() + propertyName[1:] 


def getModelLength(modelData):
    dataPoints = modelData['group']['points']
    modelLength = 0
    for dataPoint in dataPoints:
        modelLength += dataPoint['size']    

    return modelLength - modelHeaderSize


def getGroupBlockLength(dataPoints):
    modelLength = 0
    for dataPoint in dataPoints:
        modelLength += dataPoint['size']    

    return modelLength


def getCppType(dataPoint):
    if dataPoint['type'] == 'uint16':
        # If we have a scale factor set, this is gonna be a float value
        if 'sf' in dataPoint:
            return 'float'
        else:
            return 'quint16'
    elif dataPoint['type'] == 'acc16' or dataPoint['type'] == 'pad' or dataPoint['type'] == 'raw16':
        return 'quint16'
    elif dataPoint['type'] == 'int16':
        if 'sf' in dataPoint:
            return 'float'
        else:
            return 'qint16'
    elif dataPoint['type'] == 'uint32':
        if 'sf' in dataPoint:
            return 'float'
        else:
            return 'quint32'
    elif dataPoint['type'] == 'acc32':
        return 'quint32'
    elif dataPoint['type'] == 'int32':
        if 'sf' in dataPoint:
            return 'float'
        else:
            return 'qint32'
    elif dataPoint['type'] == 'uint64' or dataPoint['type'] == 'acc64':
        return 'quint64'
    elif dataPoint['type'] == 'int64':
        return 'qint64'
    elif dataPoint['type'] == 'sunssf':
        return 'qint16'
    elif dataPoint['type'] == 'string':
        return 'QString'
    elif dataPoint['type'] == 'count':
        return 'int'
    elif dataPoint['type'] == 'float32':
        return 'float'
    elif dataPoint['type'] == 'float64':
        return 'double'
    elif dataPoint['type'].startswith('enum'):
        if 'symbols' in dataPoint:
            return getEnumName(dataPoint)
        else:
            if dataPoint['type'] == 'enum16':
                return 'quint16'
            elif dataPoint['type'] == 'enum32':
                return 'quint32'

    elif dataPoint['type'].startswith('bitfield'):
        if 'symbols' in dataPoint:
            return getEnumName(dataPoint) + 'Flags'
        else:
            if dataPoint['type'] == 'bitfield16':
                return 'quint16'
            elif dataPoint['type'] == 'bitfield32':
                return 'quint32'
            elif dataPoint['type'] == 'bitfield64':
                return 'quint64'
    else:
        return dataPoint['type']


def getConvertionMethodFromSunspecType(dataPoint, scaleFactorProperty):
    typeString = dataPoint['type']
    if typeString == 'uint16' or typeString == 'acc16' or typeString == 'pad' or typeString == 'raw16':
        # If we have a scale factor set, this is gonna be a float value
        if 'sf' in dataPoint:
            # This is a float value with scale factor
            if scaleFactorProperty == '':
                return ('m_dataPoints.value("%s").toFloatWithSSF(%s)' % (dataPoint['name'], dataPoint['sf']))
            else:
                return ('m_dataPoints.value("%s").toFloatWithSSF(%s)' % (dataPoint['name'], scaleFactorProperty))
        else:
            return ('m_dataPoints.value("%s").toUInt16()' % dataPoint['name'])

    elif typeString == 'int16':
        # If we have a scale factor set, this is gonna be a float value
        if 'sf' in dataPoint:
            # This is a float value with scale factor
            if scaleFactorProperty == '':
                return ('m_dataPoints.value("%s").toFloatWithSSF(%s)' % (dataPoint['name'], dataPoint['sf']))
            else:
                return ('m_dataPoints.value("%s").toFloatWithSSF(%s)' % (dataPoint['name'], scaleFactorProperty))
        else:
            return ('m_dataPoints.value("%s").toInt16()' % dataPoint['name'])
    
    elif typeString == 'uint32' or typeString == 'acc32' or typeString == 'pad32':
        # If we have a scale factor set, this is gonna be a float value
        if 'sf' in dataPoint:
            # This is a float value with scale factor
            if scaleFactorProperty == '':
                return ('m_dataPoints.value("%s").toFloatWithSSF(%s)' % (dataPoint['name'], dataPoint['sf']))
            else:
                return ('m_dataPoints.value("%s").toFloatWithSSF(%s)' % (dataPoint['name'], scaleFactorProperty))
        else:
            return ('m_dataPoints.value("%s").toUInt32()' % dataPoint['name'])

    elif typeString == 'int32':
        # If we have a scale factor set, this is gonna be a float value
        if 'sf' in dataPoint:
            # This is a float value with scale factor
            if scaleFactorProperty == '':
                return ('m_dataPoints.value("%s").toFloatWithSSF(%s)' % (dataPoint['name'], dataPoint['sf']))
            else:
                return ('m_dataPoints.value("%s").toFloatWithSSF(%s)' % (dataPoint['name'], scaleFactorProperty))
        else:
            return ('m_dataPoints.value("%s").toInt32()' % dataPoint['name'])

    elif typeString == 'uint64':
        # If we have a scale factor set, this is gonna be a float value
        if 'sf' in dataPoint:
            # This is a float value with scale factor
            if scaleFactorProperty == '':
                return ('m_dataPoints.value("%s").toFloatWithSSF(%s)' % (dataPoint['name'], dataPoint['sf']))
            else:
                return ('m_dataPoints.value("%s").toFloatWithSSF(%s)' % (dataPoint['name'], scaleFactorProperty))
        else:
            return ('m_dataPoints.value("%s").toUInt64()' % dataPoint['name'])

    elif typeString == 'int64' or typeString == 'acc64':
        # If we have a scale factor set, this is gonna be a float value
        if 'sf' in dataPoint:
            # This is a float value with scale factor
            if scaleFactorProperty == '':
                return ('m_dataPoints.value("%s").toFloatWithSSF(%s)' % (dataPoint['name'], dataPoint['sf']))
            else:
                return ('m_dataPoints.value("%s").toFloatWithSSF(%s)' % (dataPoint['name'], scaleFactorProperty))
        else:
            return ('m_dataPoints.value("%s").toInt64()' % dataPoint['name'])

    elif typeString == 'sunssf':
        return ('m_dataPoints.value("%s").toInt16()' % dataPoint['name'])

    elif typeString == 'count':
        return ('m_dataPoints.value("%s").toUInt16()' % dataPoint['name'])

    elif typeString == 'string':
        return ('m_dataPoints.value("%s").toString()' % dataPoint['name'])

    elif typeString == 'float32':
        return ('m_dataPoints.value("%s").toFloat()' % dataPoint['name'])

    elif typeString == 'float64':
        return ('m_dataPoints.value("%s").toDouble()' % dataPoint['name'])

    elif typeString == 'enum16':
        if 'symbols' in dataPoint:
            return ('static_cast<%s>(m_dataPoints.value("%s").toUInt16())' % (getEnumName(dataPoint), dataPoint['name']))
        else:
            return ('m_dataPoints.value("%s").toUInt16()' % dataPoint['name'])

    elif typeString == 'enum32':
        if 'symbols' in dataPoint:
            return ('static_cast<%s>(m_dataPoints.value("%s").toUInt32())' % (getEnumName(dataPoint), dataPoint['name']))
        else:
            return ('m_dataPoints.value("%s").toUInt32()' % dataPoint['name'])

    elif typeString == 'bitfield16':
        if 'symbols' in dataPoint:
            return ('static_cast<%sFlags>(m_dataPoints.value("%s").toUInt16())' % (getEnumName(dataPoint), dataPoint['name']))
        else:
            return ('m_dataPoints.value("%s").toUInt16()' % dataPoint['name'])

    elif typeString == 'bitfield32':
        if 'symbols' in dataPoint:
            return ('static_cast<%sFlags>(m_dataPoints.value("%s").toUInt32())' % (getEnumName(dataPoint), dataPoint['name']))
        else:
            return ('m_dataPoints.value("%s").toUInt32()' % dataPoint['name'])

    elif typeString == 'bitfield64':
        if 'symbols' in dataPoint:
            return ('static_cast<%sFlags>(m_dataPoints.value("%s").toUInt64())' % (getEnumName(dataPoint), dataPoint['name']))
        else:
            return ('m_dataPoints.value("%s").toUInt64()' % dataPoint['name'])


def getConvertionMethodToSunspecType(dataPoint, scaleFactorProperty):
    typeString = dataPoint['type']
    cppType = getCppType(dataPoint)
    propertyName = getPropertyName(dataPoint)
    line = 'QVector<quint16> registers = SunSpecDataPoint::'
    if typeString == 'uint16' or typeString == 'acc16' or typeString == 'pad' or typeString == 'raw16':
        # If we have a scale factor set, this is gonna be a float value
        if 'sf' in dataPoint:
            # This is a float value with scale factor
            scaleFactor = ''
            if scaleFactorProperty == '':
                scaleFactor = dataPoint['sf']
            else:
                scaleFactor = scaleFactorProperty

            # Float with scale factor
            line += ('convertFromFloatWithSSF(%s, %s, dp.dataType());' % (propertyName, scaleFactor))
        else:
            line +=  ('convertFromUInt16(%s);' % propertyName)

    elif typeString == 'int16':
        # If we have a scale factor set, this is gonna be a float value
        if 'sf' in dataPoint:
            # This is a float value with scale factor
            scaleFactor = ''
            if scaleFactorProperty == '':
                scaleFactor = dataPoint['sf']
            else:
                scaleFactor = scaleFactorProperty

            # Float with scale factor
            line += ('convertFromFloatWithSSF(%s, %s, dp.dataType());' % (propertyName, scaleFactor))
        else:
            line +=  ('convertFromInt16(%s);' % propertyName)


    elif typeString == 'enum16' or typeString == 'bitfield16':
        line += ('convertFromUInt16(static_cast<quint16>(%s));' % propertyName)
    
    elif typeString == 'enum32' or typeString == 'bitfield32':
        line += ('convertFromUInt32(static_cast<quint32>(%s));' % propertyName)

    elif typeString == 'uint32':
        line += ('convertFromUInt32(%s);' % propertyName)

    elif typeString == 'int32':
        line += ('convertFromInt32(%s);' % propertyName)

    elif typeString == 'int64':
        line += ('convertFromInt64(%s);' % propertyName)

    elif typeString == 'float32':
        line += ('convertFromFloat32(%s);' % propertyName)

    elif typeString == 'float64':
        line += ('convertFromFloat64(%s);' % propertyName)

    elif typeString == 'string':
        line += ('convertFromString(%s, dp.size());' % (propertyName, ))

    else:
        line = 'QVector<quint16> registers;'
    
    return line


def addPropertiesMethodDeclaration(fileDescriptor, dataPoints):
    print('Write member get method declarations')
    previouseHadWriteMethod = False
    for dataPoint in dataPoints:
        defineWriteMethod = ('access' in dataPoint and dataPoint['access'] == 'RW')
        if defineWriteMethod and not previouseHadWriteMethod:
            previouseHadWriteMethod = True
            writeLine(fileDescriptor)

        propertyName = getPropertyName(dataPoint)
        
        # Those properties are static defined from this script
        if propertyName == 'modelId' or propertyName == 'modelLength':
            continue

        typeName = dataPoint['type']
        writeLine(fileDescriptor, '    %s %s() const;' %(getCppType(dataPoint), propertyName))

        if defineWriteMethod:
            if dataPoint['type'] == 'string':
                writeLine(fileDescriptor, '    QModbusReply *set%s(const %s &%s);' % (convertToCamelCase(propertyName, True), getCppType(dataPoint), propertyName))
            else:
                writeLine(fileDescriptor, '    QModbusReply *set%s(%s %s);' % (convertToCamelCase(propertyName, True), getCppType(dataPoint), propertyName))

            writeLine(fileDescriptor)


def addPropertiesMethodImplementation(fileDescriptor, className, dataPoints):
    print('Write member get method implementations')
    for dataPoint in dataPoints:
        defineWriteMethod = ('access' in dataPoint and dataPoint['access'] == 'RW')
        propertyName = getPropertyName(dataPoint)

        # Those properties are static defined from this script
        if propertyName == 'modelId' or propertyName == 'modelLength':
            continue

        typeName = dataPoint['type']

        cppType = getCppType(dataPoint)
        if cppType[0].isupper() and cppType[0] != 'Q':
            writeLine(fileDescriptor, '%s::%s %s::%s() const' % (className, cppType, className, propertyName))
        else:
            writeLine(fileDescriptor, '%s %s::%s() const' % (cppType, className, propertyName))

        writeLine(fileDescriptor, '{')
        writeLine(fileDescriptor, '    return m_%s;' % propertyName)
        writeLine(fileDescriptor, '}')
        
        if defineWriteMethod:
            writeLine(fileDescriptor)
            if dataPoint['type'] == 'string':
                writeLine(fileDescriptor, 'QModbusReply *%s::set%s(const %s &%s)' % (className, convertToCamelCase(propertyName, True), getCppType(dataPoint), propertyName))
            else:
                writeLine(fileDescriptor, 'QModbusReply *%s::set%s(%s %s)' % (className,convertToCamelCase(propertyName, True), getCppType(dataPoint), propertyName))

            writeLine(fileDescriptor, '{')
            writeLine(fileDescriptor, '    if (!m_initialized)')
            writeLine(fileDescriptor, '        return nullptr;')
            writeLine(fileDescriptor)
            writeLine(fileDescriptor, '    SunSpecDataPoint dp = m_dataPoints.value("%s");' % dataPoint['name'])
            scaleFactorProperty = ''
            if 'sf' in dataPoint:
                # Get the scale factor propery
                for dp in dataPoints:
                    if dp['name'] == dataPoint['sf']:
                        scaleFactorProperty = 'm_' + getPropertyName(dp)


            writeLine(fileDescriptor, '    ' + getConvertionMethodToSunspecType(dataPoint, scaleFactorProperty))
            writeLine(fileDescriptor)
            writeLine(fileDescriptor, '    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());')
            writeLine(fileDescriptor, '    request.setValues(registers);')
            writeLine(fileDescriptor)
            writeLine(fileDescriptor, '    return m_connection->modbusTcpClient()->sendWriteRequest(request, m_connection->slaveId());')
            writeLine(fileDescriptor, '}')


def addPropertiesMethodImplementationRepeatingBlock(fileDescriptor, className, dataPoints, parentDataPoints):
    print('Write property method implementation for repeating blocks')
    for dataPoint in dataPoints:
        defineWriteMethod = ('access' in dataPoint and dataPoint['access'] == 'RW')
        propertyName = getPropertyName(dataPoint)

        # Those properties are static defined from this script
        if propertyName == 'modelId' or propertyName == 'modelLength':
            continue

        typeName = dataPoint['type']
        # Note: we don't want to provide public access to scale factors
        if typeName == 'sunssf':
            continue
        
        cppType = getCppType(dataPoint)
        if cppType[0].isupper() and cppType[0] != 'Q':
            writeLine(fileDescriptor, '%s::%s %s::%s() const' % (className, cppType, className, propertyName))
        else:
            writeLine(fileDescriptor, '%s %s::%s() const' % (cppType, className, propertyName))

        writeLine(fileDescriptor, '{')
        writeLine(fileDescriptor, '    return m_%s;' % propertyName)
        writeLine(fileDescriptor, '}')
        
        if defineWriteMethod:
            writeLine(fileDescriptor)
            if dataPoint['type'] == 'string':
                writeLine(fileDescriptor, 'QModbusReply *%s::set%s(const %s &%s)' % (className, convertToCamelCase(propertyName, True), getCppType(dataPoint), propertyName))
            else:
                writeLine(fileDescriptor, 'QModbusReply *%s::set%s(%s %s)' % (className,convertToCamelCase(propertyName, True), getCppType(dataPoint), propertyName))

            writeLine(fileDescriptor, '{')
            writeLine(fileDescriptor, '    SunSpecDataPoint dp = m_dataPoints.value("%s");' % dataPoint['name'])
            scaleFactorProperty = ''
            if 'sf' in dataPoint:
                # First check if we have the scale factor in the block
                # Get the scale factor propery
                for dp in dataPoints:
                    if dp['name'] == dataPoint['sf']:
                        scaleFactorProperty = 'm_' + getPropertyName(dp)

                if scaleFactorProperty == '':
                    # Lets pick the scale factor from the parent model class
                    for dp in parentDataPoints:
                        if dp['name'] == dataPoint['sf']:
                            scaleFactorProperty = ('m_parentModel->%s()' % getPropertyName(dp))

            writeLine(fileDescriptor, '    ' + getConvertionMethodToSunspecType(dataPoint, scaleFactorProperty))
            writeLine(fileDescriptor)
            writeLine(fileDescriptor, '    QModbusDataUnit request = QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters, m_modbusStartRegister + dp.addressOffset(), registers.length());')
            writeLine(fileDescriptor, '    request.setValues(registers);')
            writeLine(fileDescriptor)
            writeLine(fileDescriptor, '    return m_parentModel->connection()->modbusTcpClient()->sendWriteRequest(request, m_parentModel->connection()->slaveId());')
            writeLine(fileDescriptor, '}')
            writeLine(fileDescriptor)







def addPropertiesDeclaration(fileDescriptor, dataPoints):
    print('Write private members')
    for dataPoint in dataPoints:
        propertyName = getPropertyName(dataPoint)

        # Those properties are static defined from this script
        if propertyName == 'modelId' or propertyName == 'modelLength':
            continue

        cppType = getCppType(dataPoint)
        initValue = ''
        if cppType.startswith('q') or cppType == 'float' or cppType == 'double' or cppType == 'int':
             initValue = ' = 0'

        writeLine(fileDescriptor, '    %s m_%s%s;' %(cppType, propertyName, initValue))


def writeProcessDataImplementation(fileDescriptor, className, dataPoints, parentDataPoints = None):
    print('Write processData() implementation')

    # Parse first the scale factors:
    hasScaleFactors = False
    for dataPoint in dataPoints:
        if dataPoint['type'] == 'sunssf':
            hasScaleFactors = True
            break

    if hasScaleFactors:
        writeLine(fileDescriptor, '    // Scale factors')
        for dataPoint in dataPoints:
            if dataPoint['type'] == 'sunssf':
                convertionMethod = getConvertionMethodFromSunspecType(dataPoint, '')
                writeLine(fileDescriptor, '    if (m_dataPoints.value("%s").isValid())' % (dataPoint['name']))
                writeLine(fileDescriptor, '        m_%s = %s;' % (getPropertyName(dataPoint), convertionMethod))
                writeLine(fileDescriptor)
                
        writeLine(fileDescriptor)
    
    writeLine(fileDescriptor, '    // Update properties according to the data point type')
    for dataPoint in dataPoints:
        propertyName = getPropertyName(dataPoint)
        # Those properties are static defined from this script
        if propertyName == 'modelId' or propertyName == 'modelLength':
            continue

        writeLine(fileDescriptor, '    if (m_dataPoints.value("%s").isValid())' % (dataPoint['name']))

        if 'sf' in dataPoint:
            # Get the scale factor propery
            scaleFactorProperty = ''
            for dp in dataPoints:
                if dp['name'] == dataPoint['sf']:
                    scaleFactorProperty = 'm_' + getPropertyName(dp)

            if scaleFactorProperty == '' and parentDataPoints != None:
                # Lets pick the scale factor from the parent model class
                for dp in parentDataPoints:
                    if dp['name'] == dataPoint['sf']:
                        scaleFactorProperty = ('m_parentModel->%s()' % getPropertyName(dp))



            convertionMethod = getConvertionMethodFromSunspecType(dataPoint, scaleFactorProperty)
            writeLine(fileDescriptor, '        m_%s = %s;' % (getPropertyName(dataPoint), convertionMethod))
        else:
            convertionMethod = getConvertionMethodFromSunspecType(dataPoint, '')
            writeLine(fileDescriptor, '        m_%s = %s;' % (getPropertyName(dataPoint), convertionMethod))
        
        writeLine(fileDescriptor)
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '    qCDebug(dcSunSpecModelData()) << this;')


def writePropertyDebugLine(fileDescriptor, className, modelId):
    print('Write class debug properties')
    modelData = models[modelId]
    dataPoints = modelData['group']['points']
    for dataPoint in dataPoints:
        propertyName = getPropertyName(dataPoint)
        if propertyName == 'modelId' or propertyName == 'modelLength':
            continue

        typeName = dataPoint['type']
        # Note: we don't want to provide public access to scale factors
        if typeName == 'sunssf':
            continue

        propertyName = getPropertyName(dataPoint)
        dataPointString = ('model->dataPoints().value("%s")' % dataPoint['name'])
        writeLine(fileDescriptor, '    debug.nospace().noquote() << "    - " << %s << "-->";' % dataPointString)
        writeLine(fileDescriptor, '    if (%s.isValid()) {' % (dataPointString))
        writeLine(fileDescriptor, '        debug.nospace().noquote() << model->%s() << "\\n";' % (propertyName))
        writeLine(fileDescriptor, '    } else {')
        writeLine(fileDescriptor, '        debug.nospace().noquote() << "NaN\\n";')
        writeLine(fileDescriptor, '    }')
        writeLine(fileDescriptor)
    
    writeLine(fileDescriptor)


def writeRepeatingBlockClassDefinition(fileDescriptor, className, modelId):
    print('Write repeating block class definition')
    modelData = models[modelId]
    blockData = {}
    if 'group' in modelData and 'groups' in modelData['group']:
        blockData = modelData['group']['groups'][0]
    elif not 'group' in modelData and 'groups' in modelData:
        blockData = modelData['groups']
    else:
        return

    blockClassName = ("%sRepeatingBlock" % className)
    dataPoints = blockData['points']

    # Begin of class
    writeLine(fileDescriptor, 'class %s;' % className)
    writeLine(fileDescriptor)

    writeLine(fileDescriptor, 'class %s : public SunSpecModelRepeatingBlock' % blockClassName)
    writeLine(fileDescriptor, '{')
    writeLine(fileDescriptor, '    Q_OBJECT')
    
    # Public members
    writeLine(fileDescriptor, 'public:')

    # Enum declarations
    writeLine(fileDescriptor)
    writeClassEnums(fileDescriptor, className, dataPoints)
    writeClassFlags(fileDescriptor, className, dataPoints)

   # Constructor
    writeLine(fileDescriptor, '    explicit %s(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, %s *parent = nullptr);' % (blockClassName, className))
    writeLine(fileDescriptor, '    ~%s() override = default;' % blockClassName)
    writeLine(fileDescriptor)

    writeLine(fileDescriptor, '    %s *parentModel() const;' % className)
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '    QString name() const override;')

    # Properties
    addPropertiesMethodDeclaration(fileDescriptor, dataPoints)

    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '    void processBlockData(const QVector<quint16> blockData) override;')

    # Protected members
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, 'protected:')
    writeLine(fileDescriptor, '    void initDataPoints() override;')
    
    # Private members
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, 'private:')
    writeLine(fileDescriptor, '    %s *m_parentModel = nullptr;' % className)
    writeLine(fileDescriptor)

    addPropertiesDeclaration(fileDescriptor, dataPoints)
    writeLine(fileDescriptor)

    writeLine(fileDescriptor, '};')
    writeLine(fileDescriptor)









def writeRepeatingBlockClassImplementation(fileDescriptor, className, modelId):
    print('Write repeating block class implementation')
    modelData = models[modelId]
    blockData = {}
    if 'group' in modelData and 'groups' in modelData['group']:
        blockData = modelData['group']['groups'][0]
    elif not 'group' in modelData and 'groups' in modelData:
        blockData = modelData['groups']
    else:
        return

    blockClassName = ("%sRepeatingBlock" % className)
    dataPoints = blockData['points']

    # Constructor
    writeLine(fileDescriptor, '%s::%s(quint16 blockIndex, quint16 blockSize, quint16 modbusStartRegister, %s *parent) :' % (blockClassName, blockClassName, className))
    writeLine(fileDescriptor, '    SunSpecModelRepeatingBlock(blockIndex, blockSize, modbusStartRegister, parent)')
    writeLine(fileDescriptor, '{')
    writeLine(fileDescriptor, '    initDataPoints();')
    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)

   # Name
    writeLine(fileDescriptor, 'QString %s::name() const' % blockClassName)
    writeLine(fileDescriptor, '{')
    if 'name' in modelData['group']:
        writeLine(fileDescriptor, '    return "%s";' % blockData['name'])
    else:
        writeLine(fileDescriptor, '    return QString();')
    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)

    # Parent model
    writeLine(fileDescriptor, '%s *%s::parentModel() const' % (className, blockClassName))
    writeLine(fileDescriptor, '{')
    writeLine(fileDescriptor, '    return m_parentModel;')
    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)

    # Properties
    addPropertiesMethodImplementationRepeatingBlock(fileDescriptor, blockClassName, dataPoints, modelData['group']['points'])
    writeLine(fileDescriptor)


    # Init data points
    writeLine(fileDescriptor, 'void %s::initDataPoints()' % blockClassName)
    writeLine(fileDescriptor, '{')
    addressOffset = 0
    for dataPoint in dataPoints:
        dataSize = addDataPointInitialization(fileDescriptor, dataPoint, addressOffset)
        addressOffset += dataSize

    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)

    # Process data 
    writeLine(fileDescriptor, 'void %s::processBlockData(const QVector<quint16> blockData)' % blockClassName)
    writeLine(fileDescriptor, '{')
    writeLine(fileDescriptor, '    m_blockData = blockData;')
    writeLine(fileDescriptor)
    writeProcessDataImplementation(fileDescriptor, blockClassName, dataPoints, modelData['group']['points'])
    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)

    writeLine(fileDescriptor)









def writeHeaderFile(headerFileName, className, modelId):
    headerFileBaseName = os.path.basename(headerFileName)
    headerFiles.append(headerFileBaseName)
    print('Writing header file', headerFileBaseName)

    modelData = models[modelId]
    containingRepeatingBlock = ('group' in modelData and 'groups' in modelData['group']) or (not 'group' in modelData and 'groups' in modelData)

    fileDescriptor = open(headerFileName, 'w')
    writeLicenseHeader(fileDescriptor)
    writeLine(fileDescriptor, '#ifndef %s_H' % className.upper())
    writeLine(fileDescriptor, '#define %s_H' % className.upper())
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '#include <QObject>')
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '#include "sunspecmodel.h"')
    if containingRepeatingBlock:
        writeLine(fileDescriptor, '#include "sunspecmodelrepeatingblock.h"')

    writeLine(fileDescriptor)

    writeLine(fileDescriptor, 'class SunSpecConnection;')

    # Write the repeating block definition if required
    if containingRepeatingBlock:
        writeRepeatingBlockClassDefinition(fileDescriptor, className, modelId)
        writeLine(fileDescriptor)


    # Begin of class
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, 'class %s : public SunSpecModel' % className)
    writeLine(fileDescriptor, '{')
    writeLine(fileDescriptor, '    Q_OBJECT')
    
    # Public members
    writeLine(fileDescriptor, 'public:')

    # Enum declarations
    dataPoints = modelData['group']['points']
    writeLine(fileDescriptor)
    writeClassEnums(fileDescriptor, className, dataPoints)
    writeClassFlags(fileDescriptor, className, dataPoints)

    # Constructor
    writeLine(fileDescriptor, '    explicit %s(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, QObject *parent = nullptr);' % className)
    writeLine(fileDescriptor, '    ~%s() override; ' % className)
    writeLine(fileDescriptor)

    # Overrides from base class
    writeLine(fileDescriptor, '    QString name() const override;')
    writeLine(fileDescriptor, '    QString description() const override;')
    writeLine(fileDescriptor, '    QString label() const override;')
    writeLine(fileDescriptor)

    # Properties
    addPropertiesMethodDeclaration(fileDescriptor, dataPoints)

    # Protected members
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, 'protected:')
    writeLine(fileDescriptor, '    quint16 m_fixedBlockLength = %s;' % getModelLength(models[modelId]))

    # Check if there are repeating blocks within the module
    modelData = models[modelId]
    if 'group' in modelData and 'groups' in modelData['group']:
        print('Repearing block detected')
        writeLine(fileDescriptor, '    quint16 m_repeatingBlockLength = %s;' % getGroupBlockLength(modelData['group']['groups'][0]['points']))
    elif 'groups' in modelData and not 'group' in modelData:
        print('Repearing block detected')
        writeLine(fileDescriptor, '    quint16 m_repeatingBlockLength = %s;' % getGroupBlockLength(modelData['groups'][0]['points']))
        writeLine(fileDescriptor)

    writeLine(fileDescriptor)

    writeLine(fileDescriptor, '    void initDataPoints();')
    writeLine(fileDescriptor, '    void processBlockData() override;')

    # Private members
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, 'private:')
    addPropertiesDeclaration(fileDescriptor, dataPoints)
    writeLine(fileDescriptor)

    # End of class
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '};')
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, 'QDebug operator<<(QDebug debug, %s *model);' % className)
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '#endif // %s_H' % className.upper())
    fileDescriptor.close()


def writeSourceFile(sourceFileName, className, modelId):
    sourceFileBaseName = os.path.basename(sourceFileName)
    sourceFiles.append(str(sourceFileBaseName))
    print('Writing cpp file', sourceFileBaseName)
    fileDescriptor = open(sourceFileName, 'w')
    writeLicenseHeader(fileDescriptor)
    writeLine(fileDescriptor, '#include "%s.h"' % className.lower())
    writeLine(fileDescriptor, '#include "sunspecconnection.h"')
    writeLine(fileDescriptor)
    
    modelData = models[modelId]
    containingRepeatingBlock = ('group' in modelData and 'groups' in modelData['group']) or (not 'group' in modelData and 'groups' in modelData)
    if containingRepeatingBlock:
        writeRepeatingBlockClassImplementation(fileDescriptor, className, modelId)

    # Constructor
    writeLine(fileDescriptor, '%s::%s(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelLength, QObject *parent) :' % (className, className))
    writeLine(fileDescriptor, '    SunSpecModel(connection, modbusStartRegister, %s, modelLength, parent)' % modelId)
    writeLine(fileDescriptor, '{')
    #assertMessage = 'QString("model length %1 given in the constructor does not match the model length from the specs %2.").arg(modelLength).arg(m_modelLengthSunSpec).toLatin1()'
    #writeLine(fileDescriptor, '    //Q_ASSERT_X(modelLength == m_modelLengthSunSpec,  "%s", %s);' % (className, assertMessage))

    if 'group' in modelData and 'groups' in modelData['group']:
        writeLine(fileDescriptor, '    m_modelBlockType = SunSpecModel::ModelBlockTypeFixedAndRepeating;')
        writeLine(fileDescriptor)
    elif not 'group' in modelData and 'groups' in modelData:
        writeLine(fileDescriptor, '    m_modelBlockType = SunSpecModel::ModelBlockTypeRepeating;')
        writeLine(fileDescriptor)
    elif 'group' in modelData and not 'groups' in modelData['group']:
        writeLine(fileDescriptor, '    m_modelBlockType = SunSpecModel::ModelBlockTypeFixed;')
        writeLine(fileDescriptor)


    #writeLine(fileDescriptor, '    if (modelLength != m_modelLengthSunSpec)')
    #writeLine(fileDescriptor, '        qCWarning(dcSunSpecModelData()) << qobject_cast<SunSpecModel *>(this) << "does not have the same length as specified from SunSpec" << m_modelLengthSunSpec;')
    writeLine(fileDescriptor, '    initDataPoints();')
    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)

    # Destructor
    writeLine(fileDescriptor, '%s::~%s()' % (className, className))
    writeLine(fileDescriptor, '{')
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)

    # Methods
    # Override methods

    # Name
    writeLine(fileDescriptor, 'QString %s::name() const' % className)
    writeLine(fileDescriptor, '{')
    if 'name' in modelData['group']:
        writeLine(fileDescriptor, '    return "%s";' % modelData['group']['name'])
    else:
        writeLine(fileDescriptor, '    return QString();')
    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)

    # Description
    writeLine(fileDescriptor, 'QString %s::description() const' % className)
    writeLine(fileDescriptor, '{')
    if 'desc' in modelData['group']:
        writeLine(fileDescriptor, '    return "%s";' % modelData['group']['desc'])
    else:
        writeLine(fileDescriptor, '    return QString();')
    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)


    # Label
    writeLine(fileDescriptor, 'QString %s::label() const' % className)
    writeLine(fileDescriptor, '{')
    if 'label' in modelData['group']:
        writeLine(fileDescriptor, '    return "%s";' % modelData['group']['label'])
    else:
        writeLine(fileDescriptor, '    return QString();')

    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)

    # Property get methods
    dataPoints = modelData['group']['points']
    addPropertiesMethodImplementation(fileDescriptor, className, dataPoints)

    # Init data points
    writeLine(fileDescriptor, 'void %s::initDataPoints()' % className)
    writeLine(fileDescriptor, '{')
    addressOffset = 0
    for dataPoint in dataPoints:
        dataSize = addDataPointInitialization(fileDescriptor, dataPoint, addressOffset)
        addressOffset += dataSize

    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)

    # Process data 
    writeLine(fileDescriptor, 'void %s::processBlockData()' % className)
    writeLine(fileDescriptor, '{')  
    writeProcessDataImplementation(fileDescriptor, className, dataPoints)
    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)

    # Debug operator
    writeLine(fileDescriptor, 'QDebug operator<<(QDebug debug, %s *model)' % className)
    writeLine(fileDescriptor, '{')
    writeLine(fileDescriptor, '    debug.nospace().noquote() << "%s(Model: " << model->modelId() << ", Register: " << model->modbusStartRegister() << ", Length: " << model->modelLength() << ")\\n";' % className)
    writePropertyDebugLine(fileDescriptor, className, modelId)
    writeLine(fileDescriptor, '    return debug.space().quote();')
    writeLine(fileDescriptor, '}')

    fileDescriptor.close()


def getClassName(groupName):
    className = 'SunSpec'
    groupNameCamelCase = convertToCamelCase(groupName, True)
    if groupNameCamelCase.endswith('Model'):
        className += groupNameCamelCase
    else:
        className += groupNameCamelCase + 'Model'

    return className


def createClass(className, modelId):
    print('Creating class -->', className, 'using model id', modelId)
    headerFileName = os.path.join(outputDirectory, className.lower() + '.h')
    sourceFileName = os.path.join(outputDirectory, className.lower() + '.cpp')
    writeHeaderFile(headerFileName, className, modelId)
    writeSourceFile(sourceFileName, className, modelId)
        

def writeModelFactoryHeader(fileDescriptor):
    print('Writing model factory header')
    writeLicenseHeader(fileDescriptor)
    className = 'SunSpecModelFactory'
    writeLine(fileDescriptor, '#ifndef %s_H' % className.upper())
    writeLine(fileDescriptor, '#define %s_H' % className.upper())
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '#include <QObject>')
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '#include "sunspecmodel.h"')
    writeLine(fileDescriptor)

    writeLine(fileDescriptor, 'class SunSpecConnection;')
    writeLine(fileDescriptor)

    for cn in classes.keys():
        writeLine(fileDescriptor, 'class %s;' % cn)

    writeLine(fileDescriptor)

    # Begin of class
    writeLine(fileDescriptor, 'class %s : public QObject' % className)
    writeLine(fileDescriptor, '{')
    writeLine(fileDescriptor, '    Q_OBJECT')
    
    # Public members
    writeLine(fileDescriptor, 'public:')

    # Enum declarations
    writeLine(fileDescriptor, '    enum ModelId {')
    classIndex = 0
    for cn, mid in classes.items():
        print('Name', cn, mid)
        classIndex += 1
        line = ('    ModelId%s = %s' % (cn.replace('Model', '').replace('SunSpec', ''), mid)) 
        if classIndex < len(classes):
            writeLine(fileDescriptor, '    %s,' % line)
        else:
            writeLine(fileDescriptor, '    %s' % line)

    writeLine(fileDescriptor, '    };')
    writeLine(fileDescriptor, '    Q_ENUM(ModelId)')
    writeLine(fileDescriptor)

    # Constructor
    writeLine(fileDescriptor, '    explicit %s(QObject *parent = nullptr);' % className)
    writeLine(fileDescriptor, '    ~%s() = default;' % className)
    writeLine(fileDescriptor)

    writeLine(fileDescriptor, '    SunSpecModel *createModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelId, quint16 modelLength);')

    # Private members
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, 'private:')
    writeLine(fileDescriptor)

    # End of class
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '};')
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '#endif // %s_H' % className.upper())



def writeModelFactorySource(fileDescriptor):
    print('Writing model factory source')
    writeLicenseHeader(fileDescriptor)
    className = 'SunSpecModelFactory'
    writeLine(fileDescriptor, '#include "%s.h"' % className.lower())
    writeLine(fileDescriptor, '#include "sunspecconnection.h"')
    writeLine(fileDescriptor)
    for classHeader in headerFiles:
        writeLine(fileDescriptor, '#include "%s"' % classHeader)

    writeLine(fileDescriptor)

    # Constructor
    writeLine(fileDescriptor, '%s::%s(QObject *parent) :' % (className, className))
    writeLine(fileDescriptor, '    QObject(parent)')
    writeLine(fileDescriptor, '{')
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)

    # Write factory class
    writeLine(fileDescriptor, 'SunSpecModel *%s::createModel(SunSpecConnection *connection, quint16 modbusStartRegister, quint16 modelId, quint16 modelLength)' % className)
    writeLine(fileDescriptor, '{')  
    writeLine(fileDescriptor, '    switch(modelId) {')

    for cn in classes:
        enumName = 'ModelId' + cn.replace('Model', '').replace('SunSpec', '')
        writeLine(fileDescriptor, '    case %s:' % enumName)
        writeLine(fileDescriptor, '        return new %s(connection, modbusStartRegister, modelLength, connection);' % cn)

    writeLine(fileDescriptor, '    default:')
    writeLine(fileDescriptor, '        return nullptr;')
    writeLine(fileDescriptor, '    }')

    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)

############################################################################################
# Main
############################################################################################

#testCamelCase()
#exit(0)

# Paths
scriptPath = os.path.dirname(os.path.realpath(sys.argv[0]))
modelJsonFolder = os.path.join(scriptPath, 'models/json')
outputDirectory = os.path.join(scriptPath, '../models')

# id, {name, lable}
models = {}
modelHeaderSize = 2

init()
loadModels()

headerFiles = []
sourceFiles = []

# Create a class for each group
groups = loadGroups()

# name, modelId
classes = {}
for group in groups.keys():
    if len(groups[group]) > 1:
        for modelId in groups[group]:
            modelData = models[modelId]
            lable = modelData['group']['label']
            className = getClassName(lable)
            classes[className] = modelId
            # Use the lable instead of the group name for the class name
    else:
        className = getClassName(group)
        classes[className] = groups[group][0]

# Print defined class names for models
for className in classes.keys():
    print(classes[className], '-->', className)
    createClass(className, classes[className])

print('Generated header files')
for headerFile in headerFiles:
    print('  ', headerFile)

print('Generated source files')
for sourceFile in sourceFiles:
    print('  ', sourceFile)


# Write model factory class
modelFactoryHeaderFileName = 'sunspecmodelfactory.h'
print('--> Writing', modelFactoryHeaderFileName)
modelFactoryHeaderFilePath = os.path.join(outputDirectory, modelFactoryHeaderFileName)
modelFactoryHeaderFile = open(modelFactoryHeaderFilePath, 'w')
writeModelFactoryHeader(modelFactoryHeaderFile)
modelFactoryHeaderFile.close()

modelFactorySourceFileName = 'sunspecmodelfactory.cpp'
print('--> Writing', modelFactorySourceFileName)
modelFactorySourceFilePath = os.path.join(outputDirectory, modelFactorySourceFileName)
modelFactorySourceFile = open(modelFactorySourceFilePath, 'w')
writeModelFactorySource(modelFactorySourceFile)
modelFactorySourceFile.close()


headerFiles.append(modelFactoryHeaderFileName)
sourceFiles.append(modelFactorySourceFileName)

# Write include files
includeFileBaseName = 'models.pri'
print('--> Writing', includeFileBaseName)
includeFileName = os.path.join(outputDirectory, includeFileBaseName)
includeFile = open(includeFileName, 'w')

writeLine(includeFile, 'HEADERS += \\')
headersCount = len(headerFiles)
for i in range(headersCount):
    line = '    $$PWD/' + headerFiles[i]
    if i < (headersCount - 1):
        line += ' \\'
    
    writeLine(includeFile, line)

writeLine(includeFile)
writeLine(includeFile, 'SOURCES += \\')
sourcesCount = len(sourceFiles)
for i in range(sourcesCount):
    line = '    $$PWD/' + sourceFiles[i]
    if i < (sourcesCount - 1):
        line += ' \\'
    
    writeLine(includeFile, line)

includeFile.close()
