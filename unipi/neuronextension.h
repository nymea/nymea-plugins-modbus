/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2020, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea.
* This project including source code and documentation is protected by
* copyright law, and remains the property of nymea GmbH. All rights, including
* reproduction, publication, editing and translation, are reserved. The use of
* this project is subject to the terms of a license agreement to be concluded
* with nymea GmbH in accordance with the terms of use of nymea GmbH, available
* under https://nymea.io/license
*
* GNU Lesser General Public License Usage
* Alternatively, this project may be redistributed and/or modified under the
* terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; version 3. This project is distributed in the hope that
* it will be useful, but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this project. If not, see <https://www.gnu.org/licenses/>.
*
* For any further details and any questions please contact us under
* contact@nymea.io or see our FAQ/Licensing Information on
* https://nymea.io/license/faq
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef NEURONEXTENSION_H
#define NEURONEXTENSION_H

#include "neuroncommon.h"
#include "hardware/modbus/modbusrtumaster.h"

#include <QObject>
#include <QHash>
#include <QTimer>
#include <QUuid>

class NeuronExtension : public NeuronCommon
{
    Q_OBJECT
public:

    enum ExtensionTypes {
        xS10,
        xS20,
        xS30,
        xS40,
        xS50,
        xS11,
        xS51,
        Unknown
    };
    Q_ENUM(ExtensionTypes)

    explicit NeuronExtension(ExtensionTypes extensionType, ModbusRtuMaster *modbusInterface, int slaveAddress, QObject *parent = nullptr);
    ~NeuronExtension();
    QString type();
    static QString stringFromType(ExtensionTypes extensionType);

private:
    ExtensionTypes m_extensionType = ExtensionTypes::xS10;
    bool loadModbusMap() override;
};


class NeuronExtensionDiscovery : public QObject
{
    Q_OBJECT
public:
    NeuronExtensionDiscovery(ModbusRtuMaster *modbusRtuMaster, int startAddress = 1, int endAddress = 15);
    void setModbusRtuMaster(ModbusRtuMaster *modbusRtuMaster);

    bool startDiscovery();
    void stopDiscovery();

private:
    ModbusRtuMaster *m_modbusRtuMaster;
    QHash<int, NeuronExtension::ExtensionTypes> m_discoveredExtensions;

    int m_startAddress = 1;
    int m_endAddress = 15;
    int m_sweepingAddress = 1;
    bool m_discoveryOngoing = false;

    void getNext(int address);

signals:
    void deviceFound(int address, NeuronExtension::ExtensionTypes model);
    void finished(QHash<int, NeuronExtension::ExtensionTypes> devices);
};
#endif // NEURONEXTENSION_H
