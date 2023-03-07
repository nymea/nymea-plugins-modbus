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
    writeLine(headerFile, '    bool reachable() const;')
    writeLine(headerFile)
    writeLine(headerFile, '    ModbusDataUtils::ByteOrder endianness() const;')
    writeLine(headerFile, '    void setEndianness(ModbusDataUtils::ByteOrder endianness);')
    writeLine(headerFile)
    writeLine(headerFile, '    uint checkReachableRetries() const;')
    writeLine(headerFile, '    void setCheckReachableRetries(uint checkReachableRetries);')
    writeLine(headerFile)

    # Write registers get method declarations
    writePropertyGetSetMethodDeclarationsTcp(headerFile, registerJson['registers'])
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writePropertyGetSetMethodDeclarationsTcp(headerFile, blockDefinition['registers'])

        # Write block get/set method declarations
        writeBlocksUpdateMethodDeclarations(headerFile, registerJson['blocks'])

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

    # Write init and update method declarations
    writeLine(headerFile, '    virtual bool initialize();')
    writeLine(headerFile, '    virtual bool update();')
    writeLine(headerFile)

    # Write registers value changed signals
    writeLine(headerFile, 'signals:')
    writeLine(headerFile, '    void reachableChanged(bool reachable);')
    writeLine(headerFile, '    void checkReachabilityFailed();')
    writeLine(headerFile, '    void checkReachableRetriesChanged(uint checkReachableRetries);')
    writeLine(headerFile)
    writeLine(headerFile, '    void initializationFinished(bool success);')
    writeLine(headerFile, '    void updateFinished();')
    writeLine(headerFile)
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
    writeProtectedPropertyMembers(headerFile, registerJson['registers'])
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writeProtectedPropertyMembers(headerFile, blockDefinition['registers'])

        writeLine(headerFile)

    writePropertyProcessMethodDeclaration(headerFile, registerJson['registers'])
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writePropertyProcessMethodDeclaration(headerFile, blockDefinition['registers'])

    writeLine(headerFile, '    void handleModbusError(QModbusDevice::Error error);')
    writeLine(headerFile, '    void testReachability();')
    writeLine(headerFile)

    # Private members
    writeLine(headerFile, 'private:')
    writeLine(headerFile, '    ModbusDataUtils::ByteOrder m_endianness = ModbusDataUtils::ByteOrder%s;' % endianness)
    writeLine(headerFile, '    quint16 m_slaveId = 1;')
    writeLine(headerFile)
    writeLine(headerFile, '    bool m_reachable = false;')
    writeLine(headerFile, '    QModbusReply *m_checkRechableReply = nullptr;')
    writeLine(headerFile, '    uint m_checkReachableRetries = 0;')
    writeLine(headerFile, '    uint m_checkReachableRetriesCount = 0;')
    writeLine(headerFile, '    bool m_communicationWorking = false;')
    writeLine(headerFile, '    quint8 m_communicationFailedMax = %s;' % (errorLimitUntilNotReachable))
    writeLine(headerFile, '    quint8 m_communicationFailedCounter = 0;')
    writeLine(headerFile)
    writeLine(headerFile, '    QVector<QModbusReply *> m_pendingInitReplies;')
    writeLine(headerFile, '    QVector<QModbusReply *> m_pendingUpdateReplies;')
    writeLine(headerFile)
    writeLine(headerFile, '    QObject *m_initObject = nullptr;')
    writeLine(headerFile, '    void verifyInitFinished();')
    writeLine(headerFile, '    void finishInitialization(bool success);')
    writeLine(headerFile)
    writeLine(headerFile, '    void verifyUpdateFinished();')
    writeLine(headerFile)
    writeLine(headerFile, '    void onReachabilityCheckFailed();')
    writeLine(headerFile, '    void evaluateReachableState();')

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
    writeLine(sourceFile, '#include <math.h>')
    writeLine(sourceFile, '#include <QTimer>')
    writeLine(sourceFile)
    writeLine(sourceFile, 'NYMEA_LOGGING_CATEGORY(dc%s, "%s")' % (className, className))
    writeLine(sourceFile)

    # Constructor
    writeLine(sourceFile, '%s::%s(const QHostAddress &hostAddress, uint port, quint16 slaveId, QObject *parent) :' % (className, className))
    writeLine(sourceFile, '    ModbusTCPMaster(hostAddress, port, parent),')
    writeLine(sourceFile, '    m_slaveId(slaveId)')
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    connect(this, &ModbusTCPMaster::connectionStateChanged, this, [this](bool status){')
    writeLine(sourceFile, '        if (status) {')
    writeLine(sourceFile, '           qCDebug(dc%s()) << "Modbus TCP connection" << m_hostAddress.toString() << "connected. Start testing if the connection is reachable...";' % (className))
    writeLine(sourceFile, '            // Cleanup before starting to initialize')
    writeLine(sourceFile, '            m_pendingInitReplies.clear();')
    writeLine(sourceFile, '            m_pendingUpdateReplies.clear();')
    writeLine(sourceFile, '            m_communicationWorking = false;')
    writeLine(sourceFile, '            m_communicationFailedCounter = 0;')
    writeLine(sourceFile, '            m_checkReachableRetriesCount = 0;')
    writeLine(sourceFile, '            testReachability();')
    writeLine(sourceFile, '        } else {')
    writeLine(sourceFile, '            qCWarning(dc%s()) << "Modbus TCP connection diconnected from" << m_hostAddress.toString() << ". The connection is not reachable any more.";' % (className))
    writeLine(sourceFile, '            m_communicationWorking = false;')
    writeLine(sourceFile, '            m_communicationFailedCounter = 0;')
    writeLine(sourceFile, '            m_checkReachableRetriesCount = 0;')
    writeLine(sourceFile, '        }')
    writeLine(sourceFile)
    writeLine(sourceFile, '        evaluateReachableState();')
    writeLine(sourceFile, '    });')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    writeLine(sourceFile, 'bool %s::reachable() const' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    return m_reachable;')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    writeLine(sourceFile, 'uint %s::checkReachableRetries() const' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    return m_checkReachableRetries;')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    writeLine(sourceFile, 'void %s::setCheckReachableRetries(uint checkReachableRetries)' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    if (m_checkReachableRetries == checkReachableRetries)')
    writeLine(sourceFile, '        return;')
    writeLine(sourceFile)
    writeLine(sourceFile, '    m_checkReachableRetries = checkReachableRetries;')
    writeLine(sourceFile, '    emit checkReachableRetriesChanged(m_checkReachableRetries);')
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
    writeLine(sourceFile)
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
    blocks = []
    if 'blocks' in registerJson:
        blocks = registerJson['blocks']

    writeInitMethodImplementationTcp(sourceFile, className, registerJson['registers'], blocks)
    writeUpdateMethodTcp(sourceFile, className, registerJson['registers'], blocks)

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

    writeLine(sourceFile, 'void %s::handleModbusError(QModbusDevice::Error error)' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    if (error == QModbusDevice::NoError) {')
    writeLine(sourceFile, '        // Reset the communication counter and we know we can reach the device')
    writeLine(sourceFile, '        m_communicationFailedCounter = 0;')
    writeLine(sourceFile, '        if (!m_communicationWorking)')
    writeLine(sourceFile, '            qCDebug(dc%s()) << "Received a reply without any errors. The communication with the device seems to work now.";' % (className))
    writeLine(sourceFile)
    writeLine(sourceFile, '        m_communicationWorking = true;')
    writeLine(sourceFile, '        evaluateReachableState();')
    writeLine(sourceFile, '    } else {')
    writeLine(sourceFile, '        m_communicationFailedCounter++;')
    writeLine(sourceFile, '        if (m_communicationWorking && m_communicationFailedCounter >= m_communicationFailedMax) {')
    writeLine(sourceFile, '            m_communicationWorking = false;')
    writeLine(sourceFile, '            qCWarning(dc%s()) << "Received" << m_communicationFailedCounter << "errors while communicating with the RTU master. Mark as not reachable until the communication works again.";' % (className))
    writeLine(sourceFile, '            evaluateReachableState();')
    writeLine(sourceFile, '        }')
    writeLine(sourceFile, '    }')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    writeTestReachabilityImplementationsTcp(sourceFile, className, registerJson['registers'], checkReachableRegister)

    writeLine(sourceFile, 'void %s::verifyInitFinished()' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    if (m_pendingInitReplies.isEmpty()) {')
    writeLine(sourceFile, '        finishInitialization(true);')
    writeLine(sourceFile, '    }')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    writeLine(sourceFile, 'void %s::finishInitialization(bool success)' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    if (success) {')
    writeLine(sourceFile, '        qCDebug(dc%s()) << "Initialization finished of %s" << hostAddress().toString() << "finished successfully";' % (className, className))
    writeLine(sourceFile, '    } else {')
    writeLine(sourceFile, '        qCWarning(dc%s()) << "Initialization finished of %s" << hostAddress().toString() << "failed.";' % (className, className))
    writeLine(sourceFile, '    }')
    writeLine(sourceFile)
    writeLine(sourceFile, '    // Cleanup init')
    writeLine(sourceFile, '    delete m_initObject;')
    writeLine(sourceFile, '    m_initObject = nullptr;')
    writeLine(sourceFile, '    m_pendingInitReplies.clear();')
    writeLine(sourceFile)
    writeLine(sourceFile, '    emit initializationFinished(success);')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    writeLine(sourceFile, 'void %s::verifyUpdateFinished()' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    if (m_pendingUpdateReplies.isEmpty()) {')
    writeLine(sourceFile, '        emit updateFinished();')
    writeLine(sourceFile, '    }')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    writeLine(sourceFile, 'void %s::onReachabilityCheckFailed()' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    m_checkReachableRetriesCount++;')
    writeLine(sourceFile)
    writeLine(sourceFile, '    if (m_checkReachableRetriesCount <= m_checkReachableRetries) {')
    writeLine(sourceFile, '        qCDebug(dc%s()) << "Reachability test failed. Retry in on second" << m_checkReachableRetriesCount << "/" << m_checkReachableRetries;' % (className))
    writeLine(sourceFile, '        QTimer::singleShot(1000, this, &%s::testReachability);' % (className))
    writeLine(sourceFile, '        return;')
    writeLine(sourceFile, '    }')
    writeLine(sourceFile)
    writeLine(sourceFile, '    // The test reachability method failed, not retrying any more')
    writeLine(sourceFile, '    emit checkReachabilityFailed();')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    writeLine(sourceFile, 'void %s::evaluateReachableState()' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    bool reachable = m_communicationWorking && connected();')
    writeLine(sourceFile, '    if (m_reachable == reachable)')
    writeLine(sourceFile, '        return;')
    writeLine(sourceFile)
    writeLine(sourceFile, '    m_reachable = reachable;')
    writeLine(sourceFile, '    emit reachableChanged(m_reachable);')
    writeLine(sourceFile, '    m_checkReachableRetriesCount = 0;')
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
    writeLine(headerFile, '    bool reachable() const;')
    writeLine(headerFile)
    writeLine(headerFile, '    uint checkReachableRetries() const;')
    writeLine(headerFile, '    void setCheckReachableRetries(uint checkReachableRetries);')
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

    # Write init and update method declarations
    writeLine(headerFile, '    virtual bool initialize();')
    writeLine(headerFile, '    virtual bool update();')
    writeLine(headerFile)

    # Write registers value changed signals
    writeLine(headerFile, 'signals:')
    writeLine(headerFile, '    void reachableChanged(bool reachable);')
    writeLine(headerFile, '    void checkReachabilityFailed();')
    writeLine(headerFile, '    void checkReachableRetriesChanged(uint checkReachableRetries);')
    writeLine(headerFile)
    writeLine(headerFile, '    void initializationFinished(bool success);')
    writeLine(headerFile, '    void updateFinished();')
    writeLine(headerFile)
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

    writeLine(headerFile, '    void handleModbusError(ModbusRtuReply::Error error);')
    writeLine(headerFile, '    void testReachability();')
    writeLine(headerFile)

    # Private members
    writeLine(headerFile, 'private:')
    writeLine(headerFile, '    ModbusRtuMaster *m_modbusRtuMaster = nullptr;')
    writeLine(headerFile, '    ModbusDataUtils::ByteOrder m_endianness = ModbusDataUtils::ByteOrder%s;' % endianness)
    writeLine(headerFile, '    quint16 m_slaveId = 1;')
    writeLine(headerFile)
    writeLine(headerFile, '    bool m_reachable = false;')
    writeLine(headerFile, '    ModbusRtuReply *m_checkRechableReply = nullptr;')
    writeLine(headerFile, '    uint m_checkReachableRetries = 0;')
    writeLine(headerFile, '    uint m_checkReachableRetriesCount = 0;')
    writeLine(headerFile, '    bool m_communicationWorking = false;')
    writeLine(headerFile, '    quint8 m_communicationFailedMax = %s;' % (errorLimitUntilNotReachable))
    writeLine(headerFile, '    quint8 m_communicationFailedCounter = 0;')
    writeLine(headerFile)
    writeLine(headerFile, '    QVector<ModbusRtuReply *> m_pendingInitReplies;')
    writeLine(headerFile, '    QVector<ModbusRtuReply *> m_pendingUpdateReplies;')
    writeLine(headerFile)
    writeLine(headerFile, '    QObject *m_initObject = nullptr;')
    writeLine(headerFile, '    void verifyInitFinished();')
    writeLine(headerFile, '    void finishInitialization(bool success);')
    writeLine(headerFile)
    writeLine(headerFile, '    void verifyUpdateFinished();')
    writeLine(headerFile)
    writeLine(headerFile, '    void onReachabilityCheckFailed();')
    writeLine(headerFile, '    void evaluateReachableState();')

    
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
    writeLine(sourceFile, '#include <QTimer>')
    writeLine(sourceFile)
    writeLine(sourceFile, 'NYMEA_LOGGING_CATEGORY(dc%s, "%s")' % (className, className))
    writeLine(sourceFile)

    # Constructor
    writeLine(sourceFile, '%s::%s(ModbusRtuMaster *modbusRtuMaster, quint16 slaveId, QObject *parent) :' % (className, className))
    writeLine(sourceFile, '    QObject(parent),')
    writeLine(sourceFile, '    m_modbusRtuMaster(modbusRtuMaster),')
    writeLine(sourceFile, '    m_slaveId(slaveId)')
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    connect(m_modbusRtuMaster, &ModbusRtuMaster::connectedChanged, this, [=](bool connected){')
    writeLine(sourceFile, '        if (connected) {')
    writeLine(sourceFile, '            qCDebug(dc%s()) << "Modbus RTU resource" << m_modbusRtuMaster->serialPort() << "connected again. Start testing if the connection is reachable...";' % (className))
    writeLine(sourceFile, '            m_pendingInitReplies.clear();')
    writeLine(sourceFile, '            m_pendingUpdateReplies.clear();')
    writeLine(sourceFile, '            m_communicationWorking = false;')
    writeLine(sourceFile, '            m_communicationFailedCounter = 0;')
    writeLine(sourceFile, '            m_checkReachableRetriesCount = 0;')
    writeLine(sourceFile, '            testReachability();')
    writeLine(sourceFile, '        } else {')
    writeLine(sourceFile, '            qCWarning(dc%s()) << "Modbus RTU resource" << m_modbusRtuMaster->serialPort() << "disconnected. The connection is not reachable any more.";' % (className))
    writeLine(sourceFile, '            m_communicationWorking = false;')
    writeLine(sourceFile, '            m_communicationFailedCounter = 0;')
    writeLine(sourceFile, '            m_checkReachableRetriesCount = 0;')
    writeLine(sourceFile, '        }')
    writeLine(sourceFile)
    writeLine(sourceFile, '        evaluateReachableState();')
    writeLine(sourceFile, '    });')
    writeLine(sourceFile)
    writeLine(sourceFile, '    if (m_modbusRtuMaster->connected()) {')
    writeLine(sourceFile, '        testReachability();')
    writeLine(sourceFile, '    } else {')
    writeLine(sourceFile, '        evaluateReachableState();')
    writeLine(sourceFile, '    }')
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
    writeLine(sourceFile)

    writeLine(sourceFile, 'bool %s::reachable() const' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    return m_reachable;')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    writeLine(sourceFile, 'uint %s::checkReachableRetries() const' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    return m_checkReachableRetries;')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    writeLine(sourceFile, 'void %s::setCheckReachableRetries(uint checkReachableRetries)' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    if (m_checkReachableRetries == checkReachableRetries)')
    writeLine(sourceFile, '        return;')
    writeLine(sourceFile)
    writeLine(sourceFile, '    m_checkReachableRetries = checkReachableRetries;')
    writeLine(sourceFile, '    emit checkReachableRetriesChanged(m_checkReachableRetries);')
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
    writePropertyGetSetMethodImplementationsRtu(sourceFile, className, registerJson['registers'])
    if 'blocks' in registerJson:
        for blockDefinition in registerJson['blocks']:
            writePropertyGetSetMethodImplementationsRtu(sourceFile, className, blockDefinition['registers'])

    # Write init and update method implementation
    blocks = []
    if 'blocks' in registerJson:
        blocks = registerJson['blocks']

    writeInitMethodImplementationRtu(sourceFile, className, registerJson['registers'], blocks)
    writeUpdateMethodRtu(sourceFile, className, registerJson['registers'], blocks)

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


    writeLine(sourceFile, 'void %s::handleModbusError(ModbusRtuReply::Error error)' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    if (error == ModbusRtuReply::NoError) {')
    writeLine(sourceFile, '        // Reset the communication counter and we know we can reach the device')
    writeLine(sourceFile, '        m_communicationFailedCounter = 0;')
    writeLine(sourceFile, '        if (!m_communicationWorking)')
    writeLine(sourceFile, '            qCDebug(dc%s()) << "Received a reply without any errors. The communication with the device seems to work now.";' % (className))
    writeLine(sourceFile)
    writeLine(sourceFile, '        m_communicationWorking = true;')
    writeLine(sourceFile, '        evaluateReachableState();')
    writeLine(sourceFile, '    } else {')
    writeLine(sourceFile, '        m_communicationFailedCounter++;')
    writeLine(sourceFile, '        if (m_communicationWorking && m_communicationFailedCounter >= m_communicationFailedMax) {')
    writeLine(sourceFile, '            m_communicationWorking = false;')
    writeLine(sourceFile, '            qCWarning(dc%s()) << "Received" << m_communicationFailedCounter << "errors while communicating with the RTU master. Mark as not reachable until the communication works again.";' % (className))
    writeLine(sourceFile, '            evaluateReachableState();')
    writeLine(sourceFile, '        }')
    writeLine(sourceFile, '    }')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    writeTestReachabilityImplementationsRtu(sourceFile, className, registerJson['registers'], checkReachableRegister)

    writeLine(sourceFile, 'void %s::verifyInitFinished()' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    if (m_pendingInitReplies.isEmpty()) {')
    writeLine(sourceFile, '        finishInitialization(true);')
    writeLine(sourceFile, '    }')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    writeLine(sourceFile, 'void %s::finishInitialization(bool success)' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    if (success) {')
    writeLine(sourceFile, '        qCDebug(dc%s()) << "Initialization finished of %s finished successfully";' % (className, className))
    writeLine(sourceFile, '    } else {')
    writeLine(sourceFile, '        qCWarning(dc%s()) << "Initialization finished of %s failed.";' % (className, className))
    writeLine(sourceFile, '    }')
    writeLine(sourceFile)
    writeLine(sourceFile, '    // Cleanup init')
    writeLine(sourceFile, '    delete m_initObject;')
    writeLine(sourceFile, '    m_initObject = nullptr;')
    writeLine(sourceFile, '    m_pendingInitReplies.clear();')
    writeLine(sourceFile)
    writeLine(sourceFile, '    emit initializationFinished(success);')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    writeLine(sourceFile, 'void %s::verifyUpdateFinished()' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    if (m_pendingUpdateReplies.isEmpty()) {')
    writeLine(sourceFile, '        emit updateFinished();')
    writeLine(sourceFile, '    }')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    writeLine(sourceFile, 'void %s::onReachabilityCheckFailed()' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    m_checkReachableRetriesCount++;')
    writeLine(sourceFile)
    writeLine(sourceFile, '    if (m_checkReachableRetriesCount <= m_checkReachableRetries) {')
    writeLine(sourceFile, '        qCDebug(dc%s()) << "Reachability test failed. Retry in on second" << m_checkReachableRetriesCount << "/" << m_checkReachableRetries;' % (className))
    writeLine(sourceFile, '        QTimer::singleShot(1000, this, &%s::testReachability);' % (className))
    writeLine(sourceFile, '        return;')
    writeLine(sourceFile, '    }')
    writeLine(sourceFile)
    writeLine(sourceFile, '    // The test reachability method failed, not retrying any more')
    writeLine(sourceFile, '    emit checkReachabilityFailed();')
    writeLine(sourceFile, '}')
    writeLine(sourceFile)

    writeLine(sourceFile, 'void %s::evaluateReachableState()' % (className))
    writeLine(sourceFile, '{')
    writeLine(sourceFile, '    bool reachable = m_communicationWorking && m_modbusRtuMaster->connected();')
    writeLine(sourceFile, '    if (m_reachable == reachable)')
    writeLine(sourceFile, '        return;')
    writeLine(sourceFile)
    writeLine(sourceFile, '    m_reachable = reachable;')
    writeLine(sourceFile, '    emit reachableChanged(m_reachable);')
    writeLine(sourceFile, '    m_checkReachableRetriesCount = 0;')
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
    logger.warning('Error: Class name property missing. Please specify the \"classname\" property in the json file.')
    exit(1)

classNamePrefix = registerJson['className']

endianness = 'BigEndian'
if 'endianness' in registerJson:
    endianness = registerJson['endianness']

errorLimitUntilNotReachable = 10
if 'errorLimitUntilNotReachable' in registerJson:
    errorLimitUntilNotReachable = registerJson['errorLimitUntilNotReachable']

# Check if the developer has specified an
checkReachableRegister = {}
if not 'checkReachableRegister' in registerJson:
    logger.warning('Error: There is no checkReachableRegister specified. Please specify the \"checkReachableRegister\" property in the register JSON and set it to the \"id\" of a mandatory readable register which should be used for testing the communication.')
    exit(1)

checkReachableRegister = registerJson['checkReachableRegister']
registerExists = False
# Make sure this is an existing and readable register
for registerDefinition in registerJson['registers']:
    if registerDefinition['id'] == checkReachableRegister:
        if not 'R' in registerDefinition['access']:
            logger.warning('Error: The specified \"checkReachableRegister\" is not readable. Please select a manadtory readable register as checkReachableRegister.')
            exit(1)

        checkReachableRegister = registerDefinition
        registerExists = True
        break

if 'blocks' in registerJson:
    for blockDefinition in registerJson['blocks']:
        for registerDefinition in blockDefinition['registers']:
            if registerDefinition['id'] == checkReachableRegister:
                if not 'R' in registerDefinition['access']:
                    logger.warning('Error: The specified \"checkReachableRegister\" is not readable. Please select a manadtory readable register as checkReachableRegister.')
                    exit(1)

                checkReachableRegister = registerDefinition
                registerExists = True
                break

if not registerExists:
    logger.warning('Error: Could not find the given \"checkReachableRegister\". Please make sure the specified register matches the \"id\" of a defined register.')
    exit(1)
else:
    logger.debug('Verified successfully checkReachableRegister: %s' % checkReachableRegister['id'])


# Inform about parsed and validated configs if debugging enabled
logger.debug('Script path: %s' % scriptPath)
logger.debug('Output directory: %s' % outputDirectory)
logger.debug('Class name prefix: %s' % classNamePrefix)
logger.debug('Endianness: %s' % endianness)
logger.debug('Error limit until not reachable: %s' % errorLimitUntilNotReachable)
logger.debug('Check reachable register: %s' % checkReachableRegister['id'])

protocol = 'TCP'
if 'protocol' in registerJson:
    protocol = registerJson['protocol']

if 'blocks' in registerJson:
    validateBlocks(registerJson['blocks'])

# Create classes depending on the protocol
writeTcp = protocol in ["TCP", "BOTH"]
writeRtu = protocol in ["RTU", "BOTH"]
if not writeTcp and not writeRtu:
    logger.warning('Error: Invalid protocol definition. Please use TCP, RTU or BOTH in the register JSON file.')
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
# This prevents qt-creator to retrigger qmake runs on it's own if the pri file is open
# by changing the project file which retriggers a qmake run and so on...
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
