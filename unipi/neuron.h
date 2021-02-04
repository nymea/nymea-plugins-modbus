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

#ifndef NEURON_H
#define NEURON_H

#include <QObject>
#include <QHash>
#include <QTimer>
#include <QtSerialBus>
#include <QUuid>

class Neuron : public QObject
{
    Q_OBJECT
public:

    struct Request {
        QUuid id;
        QModbusDataUnit data;
    };

    enum NeuronTypes {
        S103,
        M103,
        M203,
        M303,
        M403,
        M503,
        M523,
        L203,
        L303,
        L403,
        L503,
        L513,
        L523,
        L533
    };
    Q_ENUM(NeuronTypes)

    explicit Neuron(NeuronTypes neuronType, QModbusTcpClient *modbusInterface, QObject *parent = nullptr);
    ~Neuron();

    bool init();
    QString type();

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

    bool getAllDigitalOutputs();
    bool getAllDigitalInputs();
    bool getAllAnalogInputs();
    bool getAllAnalogOutputs();

    bool getUserLED(const QString &circuit);
private:
    int m_slaveAddress = 0;
    uint m_responseTimeoutTime = 2000;

    QTimer *m_inputPollingTimer = nullptr;
    QTimer *m_outputPollingTimer = nullptr;

    QModbusTcpClient *m_modbusInterface = nullptr;

    QHash<QString, int> m_modbusDigitalOutputRegisters;
    QHash<QString, int> m_modbusDigitalInputRegisters;
    QHash<QString, int> m_modbusAnalogInputRegisters;
    QHash<QString, int> m_modbusAnalogOutputRegisters;
    QHash<QString, int> m_modbusUserLEDRegisters;
    QList<Request> m_writeRequestQueue;
    QList<QModbusDataUnit> m_readRequestQueue;

    NeuronTypes m_neuronType = NeuronTypes::S103;

    QHash<QString, uint16_t> m_previousCircuitValue;

    bool loadModbusMap();
    bool modbusReadRequest(const QModbusDataUnit &request);
    bool modbusWriteRequest(const Request &request);

    bool getInputRegisters(QList<int> registers);
    bool getHoldingRegisters(QList<int> registers);
    bool getCoils(QList<int> registers);

    bool circuitValueChanged(const QString &circuit, quint32 value);

signals:
    void requestExecuted(const QUuid &requestId, bool success);
    void requestError(const QUuid &requestId, const QString &error);
    void digitalInputStatusChanged(const QString &circuit, bool value);
    void digitalOutputStatusChanged(const QString &circuit, bool value);
    void analogInputStatusChanged(const QString &circuit, double value);
    void analogOutputStatusChanged(const QString &circuit, double value);
    void userLEDStatusChanged(const QString &circuit, bool value);
    void connectionStateChanged(bool state);

public slots:
    void onOutputPollingTimer();
    void onInputPollingTimer();
};

#endif // NEURON_H
