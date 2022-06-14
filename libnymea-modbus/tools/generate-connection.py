#!/usr/bin/env python3

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

# To lazy to type all those register plugins, let's make live much easier and generate code from a json register definition 

import os
import re
import sys
import json
import time
import shutil
import argparse
import datetime
import logging

from connectiontool.toolcommon import * 
from connectiontool.modbusrtu import * 
from connectiontool.modbustcp import * 

def writeTcpHeaderFile():
    logger.info('Writing modbus TCP header file %s' % headerFilePath)
    headerFile = open(headerFilePath, 'w')

    writeLicenseHeader(headerFile)
    writeLine(headerFile, '#ifndef %s_H' % className.upper())
    writeLine(headerFile, '#define %s_H' % className.upper())
    writeLine(headerFile)
    writeLine(headerFile, '#include <QObject>')
    writeLine(headerFile)
    writeLine(headerFile, '#include <modbusdatautils.h>')
    writeLine(headerFile, '#include <modbustcpmaster.h>')

    writeLine(headerFile)

    # Begin of class
    writeLine(headerFile, 'class %s : public ModbusTCPMaster' % className)
    writeLine(headerFile, '{')
    writeLine(headerFile, '    Q_OBJECT')

    # Public members
    writeLine(headerFile, 'public:')

    # Write enum for all register values
    writeRegistersEnum(headerFile, registerJson)

    # Enum declarations
    if 'enums' in registerJson:
        for enumDefinition in registerJson['enums']:
            writeEnumDefinition(headerFile, enumDefinition)

    # Constructor
    writeLine(headerFile, '    explicit %s(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent = nullptr);' % className)
    writeLine(headerFile, '    ~%s() = default;' % className)
    writeLine(headerFile)
    writeLine(headerFile, '    ModbusDataUtils::ByteOrder endianness() const;')
    writeLine(headerFile, '    void setEndianness(ModbusDataUtils::ByteOrder endianness);')
    writeLine(headerFile)

    # Write registers get method declarations
    writePropertyGetSetMethodDeclarationsTcp(headerFile, registerJson['registers'])
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writePropertyGetSetMethodDeclarationsTcp(headerFile, blockDefinition['registers'])

        # Write block get/set method declarations
        writeBlocksUpdateMethodDeclarations(headerFile, registerJson['blocks'])

    # Write init and update method declarations
    writeLine(headerFile, '    virtual void initialize();')
    writeLine(headerFile, '    virtual void update();')
    writeLine(headerFile)

    writePropertyUpdateMethodDeclarations(headerFile, registerJson['registers'])
    writeLine(headerFile)
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writePropertyUpdateMethodDeclarations(headerFile, blockDefinition['registers'])

    writeLine(headerFile)

    writeInternalPropertyReadMethodDeclarationsTcp(headerFile, registerJson['registers'])
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writeInternalPropertyReadMethodDeclarationsTcp(headerFile, blockDefinition['registers'])

        writeLine(headerFile)
        writeInternalBlockReadMethodDeclarationsTcp(headerFile, registerJson['blocks'])

    writeLine(headerFile)

    # Write registers value changed signals
    writeLine(headerFile, 'signals:')
    writeLine(headerFile, '    void initializationFinished();')
    writeLine(headerFile, '    void endiannessChanged(ModbusDataUtils::ByteOrder endianness);')
    writeLine(headerFile)
    writePropertyChangedSignals(headerFile, registerJson['registers'])
    writeLine(headerFile)
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writePropertyChangedSignals(headerFile, blockDefinition['registers'])

    writeLine(headerFile)

    # Protected members
    writeLine(headerFile, 'protected:')

    writeLine(headerFile)
    writeProtectedPropertyMembers(headerFile, registerJson['registers'])
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writeProtectedPropertyMembers(headerFile, blockDefinition['registers'])

    writeLine(headerFile)

    writePropertyProcessMethodDeclaration(headerFile, registerJson['registers'])
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writePropertyProcessMethodDeclaration(headerFile, blockDefinition['registers'])

    writeLine(headerFile)

    # Private members
    writeLine(headerFile, 'private:')
    writeLine(headerFile, '    quint16 m_slaveId = 1;')
    writeLine(headerFile, '    QVector<QModbusReply *> m_pendingInitReplies;')
    writeLine(headerFile, '    ModbusDataUtils::ByteOrder m_endianness = ModbusDataUtils::ByteOrder%s;' % endianness)
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
    logger.info('Writing modbus TCP source file %s' % sourceFilePath)
    sourceFile = open(sourceFilePath, 'w')
    writeLicenseHeader(sourceFile)
    writeLine(sourceFile)
    writeLine(sourceFile, '#include "%s"' % headerFileName)
    writeLine(sourceFile, '#include <loggingcategories.h>')
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

    writeLine(sourceFile, 'ModbusDataUtils::ByteOrder %s::endianness() const' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    return m_endianness;')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    writeLine(sourceFile, 'void %s::setEndianness(ModbusDataUtils::ByteOrder endianness)' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    if (m_endianness == endianness)')
    writeLine(sourceFile, '        return;')
    writeLine(sourceFile,)
    writeLine(sourceFile, '    m_endianness = endianness;')
    writeLine(sourceFile, '    emit endiannessChanged(m_endianness);')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    # Property get methods
    writePropertyGetSetMethodImplementationsTcp(sourceFile, className, registerJson['registers'])
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writePropertyGetSetMethodImplementationsTcp(sourceFile, className, blockDefinition['registers'])

    # Write init and update method implementation
    writeInitMethodImplementationTcp(sourceFile, className, registerJson['registers'], registerJson['blocks'])
    writeUpdateMethod(sourceFile, className, registerJson['registers'], registerJson['blocks'])

    # Write update methods
    writePropertyUpdateMethodImplementationsTcp(sourceFile, className, registerJson['registers'])
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writePropertyUpdateMethodImplementationsTcp(sourceFile, className, blockDefinition['registers'])

        # Write block update method
        writeBlockUpdateMethodImplementationsTcp(sourceFile, className, registerJson['blocks'])

    # Write internal protected property read method implementations
    writeInternalPropertyReadMethodImplementationsTcp(sourceFile, className, registerJson['registers'])
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writeInternalPropertyReadMethodImplementationsTcp(sourceFile, className, blockDefinition['registers'])

        writeInternalBlockReadMethodImplementationsTcp(sourceFile, className, registerJson['blocks'])

    # Write internal processors of properties
    writePropertyProcessMethodImplementations(sourceFile, className, registerJson['registers'])
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writePropertyProcessMethodImplementations(sourceFile, className, blockDefinition['registers'])

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
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writeRegistersDebugLine(sourceFile, debugObjectParamName, blockDefinition['registers'])

    writeLine(sourceFile, '    return debug.quote().space();')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    sourceFile.close()


##########################################################################################################
def writeRtuHeaderFile():
    logger.info('Writing modbus RTU header file %s' % headerFilePath)
    headerFile = open(headerFilePath, 'w')

    writeLicenseHeader(headerFile)
    writeLine(headerFile, '#ifndef %s_H' % className.upper())
    writeLine(headerFile, '#define %s_H' % className.upper())
    writeLine(headerFile)
    writeLine(headerFile, '#include <QObject>')
    writeLine(headerFile)
    writeLine(headerFile, '#include <modbusdatautils.h>')
    writeLine(headerFile, '#include <hardware/modbus/modbusrtumaster.h>')

    writeLine(headerFile)

    # Begin of class
    writeLine(headerFile, 'class %s : public QObject' % className)
    writeLine(headerFile, '{')
    writeLine(headerFile, '    Q_OBJECT')

    # Public members
    writeLine(headerFile, 'public:')

    # Write enum for all register values
    writeRegistersEnum(headerFile, registerJson)

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
    writeLine(headerFile, '    ModbusDataUtils::ByteOrder endianness() const;')
    writeLine(headerFile, '    void setEndianness(ModbusDataUtils::ByteOrder endianness);')
    writeLine(headerFile)

    # Write registers get method declarations
    writePropertyGetSetMethodDeclarationsRtu(headerFile, registerJson['registers'])
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writePropertyGetSetMethodDeclarationsRtu(headerFile, blockDefinition['registers'])

        # Write block get/set method declarations
        writeBlocksUpdateMethodDeclarations(headerFile, registerJson['blocks'])

    # Write init and update method declarations
    writeLine(headerFile, '    virtual void initialize();')
    writeLine(headerFile, '    virtual void update();')
    writeLine(headerFile)

    writePropertyUpdateMethodDeclarations(headerFile, registerJson['registers'])
    writeLine(headerFile)
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writePropertyUpdateMethodDeclarations(headerFile, blockDefinition['registers'])

    writeLine(headerFile)

    writeInternalPropertyReadMethodDeclarationsRtu(headerFile, registerJson['registers'])
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writeInternalPropertyReadMethodDeclarationsRtu(headerFile, blockDefinition['registers'])

        writeLine(headerFile)
        writeInternalBlockReadMethodDeclarationsRtu(headerFile, registerJson['blocks'])


    # Write registers value changed signals
    writeLine(headerFile, 'signals:')
    writeLine(headerFile, '    void initializationFinished();')
    writeLine(headerFile, '    void endiannessChanged(ModbusDataUtils::ByteOrder endianness);')
    writeLine(headerFile)
    writePropertyChangedSignals(headerFile, registerJson['registers'])
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writePropertyChangedSignals(headerFile, blockDefinition['registers'])

    writeLine(headerFile)

    # Protected members
    writeLine(headerFile, 'protected:')

    writeProtectedPropertyMembers(headerFile, registerJson['registers'])
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writeProtectedPropertyMembers(headerFile, blockDefinition['registers'])

    writeLine(headerFile)

    writePropertyProcessMethodDeclaration(headerFile, registerJson['registers'])
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writePropertyProcessMethodDeclaration(headerFile, blockDefinition['registers'])

    writeLine(headerFile)

    # Private members
    writeLine(headerFile, 'private:')
    writeLine(headerFile, '    ModbusRtuMaster *m_modbusRtuMaster = nullptr;')
    writeLine(headerFile, '    quint16 m_slaveId = 1;')
    writeLine(headerFile, '    QVector<ModbusRtuReply *> m_pendingInitReplies;')
    writeLine(headerFile, '    ModbusDataUtils::ByteOrder m_endianness = ModbusDataUtils::ByteOrder%s;' % endianness)
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
    logger.info('Writing modbus RTU source file %s' % sourceFilePath)
    sourceFile = open(sourceFilePath, 'w')
    writeLicenseHeader(sourceFile)

    writeLine(sourceFile, '#include "%s"' % headerFileName)
    writeLine(sourceFile, '#include <loggingcategories.h>')
    writeLine(sourceFile, '#include <math.h>')
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

    writeLine(sourceFile, 'ModbusDataUtils::ByteOrder %s::endianness() const' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    return m_endianness;')
    writeLine(sourceFile, '}')

    writeLine(sourceFile, 'void %s::setEndianness(ModbusDataUtils::ByteOrder endianness)' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    if (m_endianness == endianness)')
    writeLine(sourceFile, '        return;')
    writeLine(sourceFile,)
    writeLine(sourceFile, '    m_endianness = endianness;')
    writeLine(sourceFile, '    emit endiannessChanged(m_endianness);')
    writeLine(sourceFile, '}')

    # Property get methods
    writePropertyGetSetMethodImplementationsRtu(sourceFile, className, registerJson['registers'])
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writePropertyGetSetMethodImplementationsRtu(sourceFile, className, blockDefinition['registers'])

    # Write init and update method implementation
    writeInitMethodImplementationRtu(sourceFile, className, registerJson['registers'], registerJson['blocks'])
    writeUpdateMethod(sourceFile, className, registerJson['registers'], registerJson['blocks'])

    # Write update methods
    writePropertyUpdateMethodImplementationsRtu(sourceFile, className, registerJson['registers'])
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writePropertyUpdateMethodImplementationsRtu(sourceFile, className, blockDefinition['registers'])

        # Write block update method
        writeBlockUpdateMethodImplementationsRtu(sourceFile, className, registerJson['blocks'])

    # Write internal protected property read method implementations
    writeInternalPropertyReadMethodImplementationsRtu(sourceFile, className, registerJson['registers'])
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writeInternalPropertyReadMethodImplementationsRtu(sourceFile, className, blockDefinition['registers'])

        writeInternalBlockReadMethodImplementationsRtu(sourceFile, className, registerJson['blocks'])

    # Write internal processors of properties
    writePropertyProcessMethodImplementations(sourceFile, className, registerJson['registers'])
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writePropertyProcessMethodImplementations(sourceFile, className, blockDefinition['registers'])

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
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writeRegistersDebugLine(sourceFile, debugObjectParamName, blockDefinition['registers'])

    writeLine(sourceFile, '    return debug.quote().space();')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    sourceFile.close()


############################################################################################
# Main
############################################################################################

logger = logging.getLogger('modbus-tools')
logger.setLevel(logging.INFO)
ch = logging.StreamHandler(sys.stdout)
ch.setLevel(logging.INFO)
formatter = logging.Formatter('%(name)s: %(message)s')
ch.setFormatter(formatter)
logger.addHandler(ch)

parser = argparse.ArgumentParser(description='Generate modbus tcp connection class from JSON register definitions file.')
parser.add_argument('-j', '--json', metavar='<file>', help='The JSON file containing the register definitions.')
parser.add_argument('-o', '--output-directory', metavar='<directory>', help='The output directory for the resulting class.')
parser.add_argument('-v', '--verbose', dest='verboseOutput', action='store_true', help='More verbose output.')
args = parser.parse_args()

registerJsonFilePath = os.path.realpath(args.json)
registerJson = loadJsonFile(registerJsonFilePath)
scriptPath = os.path.dirname(os.path.realpath(sys.argv[0]))
outputDirectory = os.path.realpath(args.output_directory)

if not os.path.exists(outputDirectory):
    logger.debug("Output directory does not exist. Creating directory %s", outputDirectory)
    os.makedirs(outputDirectory)

if args.verboseOutput:
    logger.setLevel(logging.DEBUG)
    ch.setLevel(logging.DEBUG)

logger.debug("Verbose output enabled")

if not 'className' in registerJson:
    logger.warning('Classname missing. Please specify the classname in the json file or pass it to the generatori using -c .')
    exit(1)

classNamePrefix = registerJson['className']

endianness = 'BigEndian'
if 'endianness' in registerJson:
    endianness = registerJson['endianness']

logger.debug('Scrip path: %s' % scriptPath)
logger.debug('Output directory: %s' % outputDirectory)
logger.debug('Class name prefix: %s' % classNamePrefix)
logger.debug('Endianness: %s' % endianness)

protocol = 'TCP'
if 'protocol' in registerJson:
    protocol = registerJson['protocol']

if 'blocks' in registerJson:
    validateBlocks(registerJson['blocks'])

# Create classes depending on the protocol
writeTcp = protocol in ["TCP", "BOTH"]
writeRtu = protocol in ["RTU", "BOTH"]
if not writeTcp and not writeRtu:
    logger.warning('Invalid protocol definition. Please use TCP, RTU or BOTH.')
    exit(1)

headerFiles = []
sourceFiles = []

if writeTcp:
    className = classNamePrefix + 'ModbusTcpConnection'
    headerFileName = className.lower() + '.h'
    headerFiles.append(headerFileName)
    sourceFileName = className.lower() + '.cpp'
    sourceFiles.append(sourceFileName)

    headerFilePath = os.path.join(outputDirectory, headerFileName)
    sourceFilePath = os.path.join(outputDirectory, sourceFileName)
    logger.debug('=======================================================')
    logger.debug('Class name: %s' % className)
    logger.debug('Header file: %s' % headerFileName)
    logger.debug('Source file: %s' % sourceFileName)
    logger.debug('Header file path: %s' % headerFilePath)
    logger.debug('Source file path: %s' % sourceFilePath)
    writeTcpHeaderFile()
    writeTcpSourceFile()

if writeRtu:
    className = classNamePrefix + 'ModbusRtuConnection'
    headerFileName = className.lower() + '.h'
    headerFiles.append(headerFileName)
    sourceFileName = className.lower() + '.cpp'
    sourceFiles.append(sourceFileName)
    headerFilePath = os.path.join(outputDirectory, headerFileName)
    sourceFilePath = os.path.join(outputDirectory, sourceFileName)
    logger.debug('=======================================================')
    logger.debug('Class name: %s' % className)
    logger.debug('Header file: %s' % headerFileName)
    logger.debug('Source file: %s' % sourceFileName)
    logger.debug('Header file path: %s' % headerFilePath)
    logger.debug('Source file path: %s' % sourceFilePath)
    writeRtuHeaderFile()
    writeRtuSourceFile()

# Write pri file
projectIncludeFileName = classNamePrefix.lower() + '.pri'
projectIncludeFilePath = os.path.join(outputDirectory, projectIncludeFileName)

# Note: we write the project file only if the registers
# file has been modified since the project has been modified the last time.
# This prevents qt-creator to retrigger qmake runs on it's own by changing the
# project file which retriggers a qmake run and so on...
if os.path.exists(projectIncludeFilePath):
    timestampRegistersJson = os.path.getmtime(registerJsonFilePath)
    timestampProjectInclude = os.path.getmtime(projectIncludeFilePath)
    if timestampRegistersJson > timestampProjectInclude:
        logger.debug('Registers modified %s' % time.ctime(timestampRegistersJson))
        logger.debug('Project file modified %s' % time.ctime(timestampProjectInclude))
        logger.debug('Register JSON file has changed since last project file update. %s' % time.ctime(timestampRegistersJson))
        logger.debug('Regenerating project file %s ...' % projectIncludeFileName)
    else:
        logger.debug('The register JSON file has not changed since the last run. Skip writing the project file %s ...' % projectIncludeFileName)
        exit(0)

logger.info('Writing connection project include file %s' % projectIncludeFileName)
projectIncludeFile = open(projectIncludeFilePath, 'w')
writeLine(projectIncludeFile, '# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #')
writeLine(projectIncludeFile, '#')
writeLine(projectIncludeFile, '# This file has been autogenerated.')
writeLine(projectIncludeFile, '# Any changes in this file may be overwritten from qmake.')
writeLine(projectIncludeFile, '#')
writeLine(projectIncludeFile, '# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #')
writeLine(projectIncludeFile)
writeLine(projectIncludeFile, 'HEADERS += \\')
for generatedHeaderFileName in headerFiles:
    writeLine(projectIncludeFile, '    $${PWD}/%s \\' % generatedHeaderFileName)
    
writeLine(projectIncludeFile)
writeLine(projectIncludeFile, "SOURCES += \\")
for generatedSourceFileName in sourceFiles:
    writeLine(projectIncludeFile, '    $${PWD}/%s \\' % generatedSourceFileName)
