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

#include "neuronextensionbus.h"

#include <QObject>
#include <QHash>
#include <QTimer>
#include <QUuid>
#include <QElapsedTimer>

class OutputRegisters;

class NeuronExtension : public QObject
{
    Q_OBJECT
public:

    enum ExtensionTypes {
        xS10,
        xS30,
        xS40,
        xS50,
        xS11,
        xS51
    };
    Q_ENUM(ExtensionTypes)

    explicit NeuronExtension(ExtensionTypes extensionType, NeuronExtensionBus *neuronExtensionBus, int slaveAddress, QObject *parent = nullptr);
    ~NeuronExtension();

    enum AnalogIOConfiguration {
        Voltage,
        Current,
        Resistance
    };
    Q_ENUM(AnalogIOConfiguration)

    struct DeviceInformation {
        int firmwareVersion;
        int numberOfDIs;
        int numberOfDOs;
        int numberOfPeripherals;
        int numberOfInternalRS485Lines;
        int numberOfAOs;
        int numberOfAIs;
        int firmwareId;
        int hardwareId;
        int serialNumber;
    };

    bool init();
    void startPolling(uint milliseconds = 200);
    int slaveAddress();
    void setSlaveAddress(int slaveAddress);

    QList<QString> digitalInputs();
    QList<QString> relayOutputs();
    QList<QString> digitalOutputs();
    QList<QString> analogInputs();
    QList<QString> analogOutputs();

    void setRelayOutput(const QString &circuit, bool value);
    void setDigitalOutput(const QString &circuit, bool value);
    void setAnalogOutput(const QString &circuit, double value);

    bool relayOutputValue(const QString &circuit);
    bool digitalOutputValue(const QString &circuit);
    bool digitalInputValue(const QString &circuit);
    double analogOutputValue(const QString &circuit);
    double analogInputValue(const QString &circuit);

    void setAnalogOutputConfiguration(const QString &circuit, AnalogIOConfiguration value);
    void setAnalogInputConfiguration(const QString &circuit, AnalogIOConfiguration value);

    void getDeviceInformation();

    QString type();
    static QString stringFromType(ExtensionTypes extensionType);

private:
    ExtensionTypes m_extensionType = ExtensionTypes::xS10;
    QElapsedTimer m_elapsedTimer;
    int m_readRequestCounter = 0; //Needed to calculate the average read request response time
    int m_averageTime = 0;

    enum RWPermission {
        RWPermissionNone,
        RWPermissionRead,
        RWPermissionReadWrite,
        RWPermissionWrite
    };

    struct RegisterDescriptor {
        int address;
        uint count;
        QString circuit;
        RWPermission readWrite;
        QString category;
        NeuronExtensionRegisterType registerType;
    };

    RegisterDescriptor registerDescriptorFromStringList(const QStringList &data);

    OutputRegisters *m_modbusRelayOutputRegisters;
    OutputRegisters *m_modbusDigitalOutputRegisters;

    QHash<QString, int> m_modbusDigitalInputRegisters;
    QHash<int, RegisterDescriptor> m_modbusAnalogInputRegisters;
    QHash<int, RegisterDescriptor> m_modbusAnalogOutputRegisters;
    QHash<QString, int> m_modbusAnalogOutputConfigurationRegisters;
    QHash<QString, int> m_modbusAnalogInputConfigurationRegisters;

    int m_slaveAddress = 0;
    uint m_responseTimeoutTime = 2000;
    QTimer *m_watchdog = nullptr;

    NeuronExtensionBus *m_neuronExtensionBus = nullptr;

    QHash<QString, uint16_t> m_previousDigitalInputValue;
    QHash<QString, uint16_t> m_previousDigitalOutputValue;
    QHash<QString, uint16_t> m_previousRelayOutputValue;
    QHash<QString, uint32_t> m_previousAnalogInputValue;

    bool loadModbusMap();
    bool circuitValueChanged(const QString &circuit, quint32 value);

    void getCoils();
    void getRegisters();
    void sendWriteRequest(const NeuronExtensionRequest &request);

signals:
    void connectionStateChanged(bool state);
    void requestExecuted(const QUuid &requestId, bool success);
    void requestError(const QUuid &requestId, const QString &error);

    void deviceInformationReceived(DeviceInformation info);

    void digitalInputStatusChanged(const QString &circuit, bool value);
    void analogInputStatusChanged(const QString &circuit, double value);
    void analogOutputStatusChanged(const QString &circuit, double value);

    void digitalOutputStatusChanged(const QString &circuit, bool value);
    void relayOutputStatusChanged(const QString &circuit, bool value);

private slots:
    void onReplyFinished(NeuronExtensionReply *reply);
    void onDeviceInformationReplyFinished(NeuronExtensionReply *reply);
};

class OutputRegisters : public QObject
{
    Q_OBJECT

public:
    explicit OutputRegisters(int startAddress, int count, QObject *parent = nullptr);
    void insert(const QString &circuit, int address);

    int address(const QString &circuit);
    bool containsAddress(int address);
    QString circuit(int address);
    bool containsCircuit(const QString &circuit);
    QStringList circuits();

    QVector<uint16_t> values();
    uint16_t value(const QString &circuit);
    uint16_t value(int address);
    void setValue(const QString &circuit, uint16_t value);
    void setValue(int address, uint16_t value);

    NeuronExtensionRequest createWriteRequest(int slaveAddress);

private:
    int m_startAddress = 0;
    int m_registerCount = 0;

    QHash<QString, int> m_modbusRegisters; // Name and address
    QVector<uint16_t> m_values;

signals:
    void valuesChanged();
};
#endif // NEURONEXTENSION_H
