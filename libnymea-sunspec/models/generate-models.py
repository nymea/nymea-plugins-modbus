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
    print('Convert camel case:', text, '-->', finalText)
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


def writeClassEnums(fileDescriptor, className, modelIds):
    print('--> Writing enums for', className)
    # name, symbols[]
    enumDefinitions = {}
    # Get unique enums
    for i in range(len(modelIds)):
        modelId = int(modelIds[i])
        modelData = models[modelId]
        points = modelData['group']['points']
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


def writeClassFlags(fileDescriptor, className, modelIds):
    print('--> Writing flags for', className)
    # name, symbols[]
    enumDefinitions = {}
    # Get unique flags
    for i in range(len(modelIds)):
        modelId = int(modelIds[i])
        modelData = models[modelId]
        points = modelData['group']['points']
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
        writeLine(fileDescriptor, linePrepend + '%s.setDataType(SunSpecDataPoint::stringToDataType("%s"));' % (propertyName, dataPoint['type']))

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

    return propertyName[0].lower() + propertyName[1:] 


def getCppType(dataPoint):
    if dataPoint['type'] == 'uint16':
        # If we have a scale factor set, this is gonna be a float value
        if 'sf' in dataPoint:
            return 'float'
        else:
            return 'quint16'
    elif dataPoint['type'] == 'acc16' or dataPoint['type'] == 'pad' or dataPoint['type'] == 'raq16':
        return 'quint16'
    elif dataPoint['type'] == 'int16':
        return 'qint16'
    elif dataPoint['type'] == 'uint32' or dataPoint['type'] == 'acc32':
        return 'quint32'
    elif dataPoint['type'] == 'int32':
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



def addPropertiesMethodDeclaration(fileDescriptor, modelIds):
    print('Write member get method declarations')
    # Picking the first model for now, since they should work all the same within one group
    modelData = models[modelIds[0]]
    dataPoints = modelData['group']['points']
    for dataPoint in dataPoints:
        propertyName = getPropertyName(dataPoint)
        typeName = dataPoint['type']
        # Note: we don't want to provide public access to scale factors
        if typeName == 'sunssf':
            continue
        
        writeLine(fileDescriptor, '    %s %s() const;' %(getCppType(dataPoint), propertyName))


def addPropertiesMethodImplementation(fileDescriptor, className, modelIds):
    print('Write member get method implementations')
    # Picking the first model for now, since they should work all the same within one group
    modelData = models[modelIds[0]]
    dataPoints = modelData['group']['points']
    for dataPoint in dataPoints:
        propertyName = getPropertyName(dataPoint)
        typeName = dataPoint['type']
        # Note: we don't want to provide public access to scale factors
        if typeName == 'sunssf':
            continue
        
        cppType = getCppType(dataPoint)
        if cppType[0].isupper() and cppType[0] != 'Q':
            writeLine(fileDescriptor, '%s::%s %s::%s() const' %(className, cppType, className, propertyName))
        else:
            writeLine(fileDescriptor, '%s %s::%s() const' %(cppType, className, propertyName))

        writeLine(fileDescriptor, '{')
        writeLine(fileDescriptor, '    return m_%s;' % propertyName)
        writeLine(fileDescriptor, '}')

def addPropertiesDeclaration(fileDescriptor, modelIds):
    print('Write private members')
    # Picking the first model for now, since they should work all the same within one group
    modelData = models[modelIds[0]]
    dataPoints = modelData['group']['points']
    for dataPoint in dataPoints:
        propertyName = getPropertyName(dataPoint)

        cppType = getCppType(dataPoint)
        writeLine(fileDescriptor, '    %s m_%s;' %(cppType, propertyName))



def writeHeaderFile(headerFileName, className, modelIds):
    headerFileBaseName = os.path.basename(headerFileName)
    headerFiles.append(headerFileBaseName)
    print('Writing header file', headerFileBaseName)

    fileDescriptor = open(headerFileName, 'w')
    writeLicenseHeader(fileDescriptor)
    writeLine(fileDescriptor, '#ifndef %s_H' % className.upper())
    writeLine(fileDescriptor, '#define %s_H' % className.upper())
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '#include <QObject>')
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '#include "sunspec.h"')
    writeLine(fileDescriptor, '#include "sunspecmodel.h"')
    writeLine(fileDescriptor)

    # Begin of class
    writeLine(fileDescriptor, 'class %s : public SunSpecModel' % className)
    writeLine(fileDescriptor, '{')
    writeLine(fileDescriptor, '    Q_OBJECT')
    
    # Public members
    writeLine(fileDescriptor, 'public:')

    # Enum declarations
    writeLine(fileDescriptor)
    writeClassEnums(fileDescriptor, className, modelIds)
    writeClassFlags(fileDescriptor, className, modelIds)

    # Constructor
    writeLine(fileDescriptor, '    explicit %s(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent = nullptr);' % className)
    writeLine(fileDescriptor, '    ~%s() override; ' % className)
    writeLine(fileDescriptor)

    # Overrides from base class
    writeLine(fileDescriptor, '    QString name() const override;')
    writeLine(fileDescriptor, '    QString description() const override;')
    writeLine(fileDescriptor, '    QString label() const override;')
    writeLine(fileDescriptor)

    # Properties
    addPropertiesMethodDeclaration(fileDescriptor, modelIds)

    # Private members
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, 'private:')
    addPropertiesDeclaration(fileDescriptor, modelIds)
    writeLine(fileDescriptor)

    writeLine(fileDescriptor, '    void initDataPoints();')

    # End of class
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '};')
    writeLine(fileDescriptor)
    writeLine(fileDescriptor, '#endif // %s_H' % className.upper())
    fileDescriptor.close()


def writeSourceFile(sourceFileName, className, modelIds):
    sourceFileBaseName = os.path.basename(sourceFileName)
    sourceFiles.append(str(sourceFileBaseName))
    print('Writing cpp file', sourceFileBaseName)
    fileDescriptor = open(sourceFileName, 'w')
    writeLicenseHeader(fileDescriptor)
    writeLine(fileDescriptor, '#include "%s.h"' % className.lower())
    writeLine(fileDescriptor)
    
    # Constructor
    writeLine(fileDescriptor, '%s::%s(SunSpec *connection, quint16 modelId, quint16 modelLength, quint16 modbusStartRegister, QObject *parent) :' % (className, className))
    writeLine(fileDescriptor, '    SunSpecModel(connection, modelId, modelLength, modbusStartRegister, parent)')
    writeLine(fileDescriptor, '{')
    writeLine(fileDescriptor, '    initDataPoints();')
    # Write supported model ids
    modelIdsLine = ''
    for i in range(len(modelIds)):
        modelIdsLine += (' << %s' % modelIds[i])

    writeLine(fileDescriptor, '    m_supportedModelIds' + modelIdsLine + ';')
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
    if len(modelIds) == 1:
        modelId = modelIds[0]
        if 'name' in models[modelId]['group']:
            writeLine(fileDescriptor, '    return "%s";' % models[modelId]['group']['name'])
        else:
            writeLine(fileDescriptor, '    return QString();')
    else:
        # Check if all names are the same
        name = ''
        requireSwitch = False
        for i in range(len(modelIds)):
            modelId = modelIds[i]
            if i == 0: 
                name = models[modelId]['group']['name']
                continue
            
            if name != models[modelId]['group']['name']:
                requireSwitch = True

        if not requireSwitch:
            writeLine(fileDescriptor, '    return "%s";' % models[modelId]['group']['name'])
        else:
            # Return value depending on the modelId
            writeLine(fileDescriptor, '    switch (m_modelId) {')
            for i in range(len(modelIds)):
                modelId = modelIds[i]
                writeLine(fileDescriptor, '    case %s:' % modelId)
                if 'name' in models[modelId]['group']:
                    writeLine(fileDescriptor, '        return "%s";' % models[modelId]['group']['name'])
                else:
                    writeLine(fileDescriptor, '        return QString();')

            writeLine(fileDescriptor, '    default:')
            writeLine(fileDescriptor, '        return QString();')
            writeLine(fileDescriptor, '    }')

    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)

    # Description
    writeLine(fileDescriptor, 'QString %s::description() const' % className)
    writeLine(fileDescriptor, '{')
    if len(modelIds) == 1:
        modelId = modelIds[0]
        if 'desc' in models[modelId]['group']:
            writeLine(fileDescriptor, '    return "%s";' % models[modelId]['group']['desc'])
        else:
            writeLine(fileDescriptor, '    return QString();')
    else:
        # Return value depending on the modelId
        writeLine(fileDescriptor, '    switch (m_modelId) {')
        for i in range(len(modelIds)):
            modelId = modelIds[i]
            writeLine(fileDescriptor, '    case %s:' % modelId)
            if 'desc' in models[modelId]['group']:
                writeLine(fileDescriptor, '        return "%s";' % models[modelId]['group']['desc'])
            else:
                writeLine(fileDescriptor, '        return QString();')

        writeLine(fileDescriptor, '    default:')
        writeLine(fileDescriptor, '        return QString();')
        writeLine(fileDescriptor, '    }')

    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)


    # Label
    writeLine(fileDescriptor, 'QString %s::label() const' % className)
    writeLine(fileDescriptor, '{')
    if len(modelIds) == 1:
        modelId = modelIds[0]
        if 'label' in models[modelId]['group']:
            writeLine(fileDescriptor, '    return "%s";' % models[modelId]['group']['label'])
        else:
            writeLine(fileDescriptor, '    return QString();')
    else:
        # Return value depending on the modelId
        writeLine(fileDescriptor, '    switch (m_modelId) {')
        for i in range(len(modelIds)):
            modelId = modelIds[i]
            writeLine(fileDescriptor, '    case %s:' % modelId)
            if 'label' in models[modelId]['group']:
                writeLine(fileDescriptor, '        return "%s";' % models[modelId]['group']['label'])
            else:
                writeLine(fileDescriptor, '        return QString();')

        writeLine(fileDescriptor, '    default:')
        writeLine(fileDescriptor, '        return QString();')
        writeLine(fileDescriptor, '    }')

    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)

    # Property get methods
    addPropertiesMethodImplementation(fileDescriptor, className, modelIds)

    # Init data points
    writeLine(fileDescriptor, 'void %s::initDataPoints()' % className)
    writeLine(fileDescriptor, '{')
    if len(modelIds) == 1:
        modelId = modelIds[0]
        dataPoints = models[modelId]['group']['points']
        addressOffset = 0
        for dataPoint in dataPoints:
            dataSize = addDataPointInitialization(fileDescriptor, dataPoint, addressOffset)
            addressOffset += dataSize
    else:
        # Return value depending on the modelId
        writeLine(fileDescriptor, '    switch (m_modelId) {')
        for i in range(len(modelIds)):
            modelId = modelIds[i]
            dataPoints = models[modelId]['group']['points']
            writeLine(fileDescriptor, '    case %s: {' % modelId)
            addressOffset = 0
            for dataPoint in dataPoints:
                dataSize = addDataPointInitialization(fileDescriptor, dataPoint, addressOffset, 2)
                addressOffset += dataSize
            
            writeLine(fileDescriptor, '        break;')
            writeLine(fileDescriptor, '    }')

        writeLine(fileDescriptor, '    default:')
        writeLine(fileDescriptor, '        break;')
        writeLine(fileDescriptor, '    }')
    writeLine(fileDescriptor, '}')
    writeLine(fileDescriptor)

    fileDescriptor.close()


def createClass(groupName, modelIds):
    className = 'SunSpec'
    groupNameCamelCase = convertToCamelCase(groupName, True)
    if groupNameCamelCase.endswith('Model'):
        className += groupNameCamelCase
    else:
        className += groupNameCamelCase + 'Model'

    print('Creating class for group', groupName, '-->', className, 'using model ids', modelIds)
    headerFileName = os.path.join(outputDirectory, className.lower() + '.h')
    sourceFileName = os.path.join(outputDirectory, className.lower() + '.cpp')
    writeHeaderFile(headerFileName, className, modelIds)
    writeSourceFile(sourceFileName, className, modelIds)
        


############################################################################################
# Main
############################################################################################

#testCamelCase()
#exit(0)

# Paths
scriptPath = os.path.dirname(os.path.realpath(sys.argv[0]))
modelJsonFolder = os.path.join(scriptPath, 'models/json')
outputDirectory = os.path.join(scriptPath, 'autogenerated')

# id, {name, lable}
models = {}

init()
loadModels()

headerFiles = []
sourceFiles = []

# Create a class for each group
groups = loadGroups()
for group in groups.keys():
    createClass(group, groups[group])

print('Generated header files')
for headerFile in headerFiles:
    print('  ', headerFile)

print('Generated source files')
for sourceFile in sourceFiles:
    print('  ', sourceFile)


# Write include files
includeFileBaseName = 'autogenerated.pri'
print('--> Writing', includeFileBaseName)
includeFileName = os.path.join(outputDirectory, includeFileBaseName)
includeFile = open(includeFileName, 'w')

# Add and copy base class for all models
#shutil.copyfile(os.path.join(scriptPath, 'sunspecmodel.h'), os.path.join(outputDirectory, 'sunspecmodel.h'))
#shutil.copyfile(os.path.join(scriptPath, 'sunspecmodel.cpp'), os.path.join(outputDirectory, 'sunspecmodel.cpp'))
#headerFiles.append('sunspecmodel.h')
#sourceFiles.append('sunspecmodel.cpp')

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
