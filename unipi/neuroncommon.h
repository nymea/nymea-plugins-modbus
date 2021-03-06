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

#ifndef NEURONCOMMON_H
#define NEURONCOMMON_H

#include <QObject>
#include <QtSerialBus>

class NeuronCommon : public QObject
{
    Q_OBJECT
public:
    explicit NeuronCommon(QModbusClient *modbusInterface, int slaveAddress, QObject *parent = nullptr);
    bool init();
    int slaveAddress();
    void setSlaveAddress(int slaveAddress);

    QList<QString> digitalInputs();
    QList<QString> digitalOutputs();
    QList<QString> analogInputs();
    QList<QString> analogOutputs();
    QList<QString> userLEDs();

    QUuid setDigitalOutput(const QString &circuit, bool value);
    QUuid setAnalogOutput(const QString &circuit, double value);
    QUuid setUserLED(const QString &circuit, bool value);

    bool getDigitalOutput(const QString &circuit);
    bool getDigitalInput(const QString &circuit);

    bool getAnalogOutput(const QString &circuit);
    bool getAnalogInput(const QString &circuit);

    void getAllDigitalOutputs();
    void getAllDigitalInputs();
    void getAllAnalogInputs();
    void getAllAnalogOutputs();

    bool getUserLED(const QString &circuit);

protected:
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
        QModbusDataUnit::RegisterType registerType;
    };

    virtual bool loadModbusMap() = 0;
    RegisterDescriptor registerDescriptorFromStringList(const QStringList &data);

    QHash<QString, int> m_modbusDigitalOutputRegisters;
    QHash<QString, int> m_modbusDigitalInputRegisters;
    QHash<QString, int> m_modbusUserLEDRegisters;
    QHash<int, RegisterDescriptor> m_modbusAnalogInputRegisters;
    QHash<int, RegisterDescriptor> m_modbusAnalogOutputRegisters;

private:
    struct Request {
        QUuid id;
        QModbusDataUnit data;
    };

    int m_slaveAddress = 0;
    uint m_responseTimeoutTime = 2000;
    QModbusClient *m_modbusInterface = nullptr;

    QTimer *m_inputPollingTimer = nullptr;
    QTimer *m_outputPollingTimer = nullptr;

    QList<Request> m_writeRequestQueue;
    QList<QModbusDataUnit> m_readRequestQueue;

    QHash<QString, uint16_t> m_previousCircuitValue;

    bool circuitValueChanged(const QString &circuit, quint32 value);
    bool getAnalogIO(const RegisterDescriptor &descriptor);

    bool readRequest(const QModbusDataUnit &request);
    bool sendModbusReadRequest(const QModbusDataUnit &request);

    bool writeRequest(const Request &request);
    bool sendModbusWriteRequest(const Request &request);
    void getCoils(QList<int> registers);

signals:
    void requestExecuted(const QUuid &requestId, bool success);
    void requestError(const QUuid &requestId, const QString &error);
    void digitalInputStatusChanged(const QString &circuit, bool value);
    void digitalOutputStatusChanged(const QString &circuit, bool value);

    void analogInputStatusChanged(const QString &circuit, double value);
    void analogOutputStatusChanged(const QString &circuit, double value);

    void userLEDStatusChanged(const QString &circuit, bool value);

    void connectionStateChanged(bool state);

private slots:
    void onOutputPollingTimer();
    void onInputPollingTimer();
};

#endif // NEURONCOMMON_H
