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

#include "unipi.h"
#include "extern-plugininfo.h"
#include <QProcess>
#include <QTimer>

UniPi::UniPi(I2CManager *i2cManager, UniPiType unipiType, QObject *parent) :
    QObject(parent),
    m_i2cManager(i2cManager),
    m_unipiType(unipiType)
{
    m_mcp23008 = new MCP23008("i2c-1", 0x20, this);
    m_analogInputChannel1 = new MCP342XChannel("i2c-1", 0x68, 0, MCP342XChannel::Gain_1, this);
    m_analogInputChannel2 = new MCP342XChannel("i2c-1", 0x68, 1, MCP342XChannel::Gain_1, this);

    m_analogOutput = new UniPiPwm(0, this);
}

UniPi::~UniPi()
{
    m_mcp23008->deleteLater();

    //m_i2cManager->close(m_analogInputChannel1);
    m_analogInputChannel1->deleteLater();
    //m_i2cManager->close(m_analogInputChannel2);
    m_analogInputChannel2->deleteLater();

    Q_FOREACH (GpioMonitor *gpio, m_monitorGpios.keys()) {
        gpio->disable();
        gpio->deleteLater();
    }

    m_analogOutput->disable();
    m_analogOutput->deleteLater();
}

bool UniPi::init()
{
    //init MCP23008 Outputs
    if (m_mcp23008->init()) {
        m_mcp23008->writeRegister(MCP23008::RegisterAddress::IODIR, 0x00); //set all pins as outputs
        m_mcp23008->writeRegister(MCP23008::RegisterAddress::IPOL, 0x00);  //set all pins to non inverted mode 1 = high
        m_mcp23008->writeRegister(MCP23008::RegisterAddress::GPPU, 0x00);  //disable all pull up resistors
        m_mcp23008->writeRegister(MCP23008::RegisterAddress::OLAT, 0x00);  //Set all outputs to low
    } else {
        qCWarning(dcUniPi()) << "Could not init MCP23008";
        return false;
    }

    // In case of re-init
    if (!m_monitorGpios.isEmpty()) {
        foreach (GpioMonitor *gpio, m_monitorGpios.keys()) {
            m_monitorGpios.remove(gpio);
            gpio->deleteLater();
        }
    }

    //Init Raspberry Pi Inputs
    foreach (QString circuit, digitalInputs()){

        int pin = getPinFromCircuit(circuit);

        GpioMonitor *gpioMonitor = new GpioMonitor(pin, this);
        if (!gpioMonitor->enable()) {
            qCWarning(dcUniPi()) << "Could not enable gpio monitor for pin" << pin;
            return false;
        } else {
            QProcess::execute(QString("gpio -g mode %1 up").arg(pin));
            QTimer::singleShot(1000, this, [gpioMonitor, circuit, this]() {
                emit digitalInputStatusChanged(circuit, gpioMonitor->value()); //set initial status
                connect(gpioMonitor, &GpioMonitor::valueChanged, this, &UniPi::onInputValueChanged);
                m_monitorGpios.insert(gpioMonitor, circuit);
            });
        }
    }
    //Init Raspberry Pi PWM output
    if (!m_analogOutput->exportPwm()) {
        qCWarning(dcUniPi()) << "Error could not export analog output";
        return false;
    }

    if (!m_analogOutput->enable()) {
        qCWarning(dcUniPi()) << "Error could not enable analog output";
        return false;
    }
    m_analogOutput->setPolarity(UniPiPwm::PolarityNormal);
    m_analogOutput->setFrequency(400);
    m_analogOutput->setPercentage(0);

    if (!m_i2cManager->open(m_analogInputChannel1)) {
        qCDebug(dcUniPi()) << "Failed to open analog channel 1";
        return false;
    }
    connect(m_analogInputChannel1, &MCP342XChannel::readingAvailable, this, [this] (const QByteArray &data){

        if (data.length() < 3) {
            qCWarning(dcUniPi()) << "Error reading data from analog channel 1" << data;
            return;
        }
        qint16 rawValue = (static_cast<quint16>(data[0]) << 8) | data[1];
        double voltage = (rawValue * 0.001 * 5.51)/2.00;
        emit analogInputStatusChanged("AI1", voltage);
    });
    m_i2cManager->startReading(m_analogInputChannel1, 5000);

    if (!m_i2cManager->open(m_analogInputChannel2)) {
        qCDebug(dcUniPi()) << "Failed to open analog channel 2";
        return false;
    }
    connect(m_analogInputChannel2, &MCP342XChannel::readingAvailable, this, [this] (const QByteArray &data){

        if (data.length() < 3) {
            qCWarning(dcUniPi()) << "Error reading data from analog channel 2" << data;
            return;
        }
        qint16 rawValue = (static_cast<quint16>(data[0]) << 8) | data[1];
        double voltage = (rawValue * 0.001 * 5.51)/2.00;
        emit analogInputStatusChanged("AI2", voltage);
    });
    m_i2cManager->startReading(m_analogInputChannel2, 5000);
    return true;
}

QString UniPi::type()
{
    QString type;
    switch (m_unipiType) {
    case UniPiType::UniPi1:
        type = "UniPi 1";
        break;
    case UniPiType::UniPi1Lite:
        type = "UniPi 1 Lite";
        break;
    }
    return type;
}

QList<QString> UniPi::digitalInputs()
{
    QList<QString> inputs;
    switch (m_unipiType) {
    case UniPiType::UniPi1:
        for (int i = 1; i < 15; ++i) {
            inputs.append(QString("DI%1").arg(i));
        }
        break;
    case UniPiType::UniPi1Lite:
        for (int i = 1; i < 7; ++i) {
            inputs.append(QString("DI%1").arg(i));
        }
        break;
    }
    return inputs;
}

QList<QString> UniPi::digitalOutputs()
{
    QList<QString> outputs;
    switch (m_unipiType) {
    case UniPiType::UniPi1:
        for (int i = 1; i < 9; ++i) {
            outputs.append(QString("DO%1").arg(i));
        }
        break;
    case UniPiType::UniPi1Lite:
        for (int i = 1; i < 7; ++i) {
            outputs.append(QString("DO%1").arg(i));
        }
        break;
    }
    return outputs;
}

QList<QString> UniPi::analogInputs()
{
    QList<QString> inputs;
    switch (m_unipiType) {
    case UniPiType::UniPi1:
        for (int i = 1; i < 3; ++i) {
            inputs.append(QString("AI%1").arg(i));
        }
        break;
    case UniPiType::UniPi1Lite:
        for (int i = 1; i < 3; ++i) {
            inputs.append(QString("AI%1").arg(i));
        }
        break;
    }
    return inputs;
}

QList<QString> UniPi::analogOutputs()
{
    QList<QString> outputs;
    outputs.append("AO");
    return outputs;
}

int UniPi::getPinFromCircuit(const QString &circuit)
{
    int pin = -1;
    if (circuit.startsWith("DI")) { //Raspberry Pi Input Pins
        switch (circuit.mid(2, 2).toInt()) {
        case 1: //DI01 GPIO04 Digital input
            pin = 4;
            break;
        case 2: //DI02 GPIO17 Digital input
            pin = 17;
            break;
        case 3: //DI03 GPIO27 Digital input
            pin = 27;
            break;
        case 4: //DI04 GPIO23 Digital input
            pin = 23;
            break;
        case 5: //DI05 GPIO22 Digital input
            pin = 22;
            break;
        case 6: //DI06 GPIO24 Digital input
            pin = 24;
            break;
        case 7: //I07 GPIO11 Digital Input
            pin = 11;
            break;
        case 8: //I08 GPIO07 Digital Input
            pin = 7;
            break;
        case 9: //I09 GPIO08 Digital Input
            pin = 8;
            break;
        case 10: //I10 GPIO09 Digital Input
            pin = 9;
            break;
        case 11: //I11 GPIO25 Digital Input
            pin = 25;
            break;
        case 12: //DI12 GPIO10 Digital input
            pin = 10;
            break;
        case 13: //DI13 GPIO31 Digital input
            pin = 31;
            break;
        case 14: //DI14 GPIO30 Digital input
            pin = 30;
            break;
        default:
            return -1;
        }
    }
    if (circuit.startsWith("DO")) { //MCP23008 Output Pins
        switch (circuit.mid(2, 2).toInt()) {
        case 1: //DO1
            pin = 7;
            break;
        case 2: //DO2
            pin = 6;
            break;
        case 3: //DO3
            pin = 5;
            break;
        case 4: //DO4
            pin = 4;
            break;
        case 5: //DO5
            pin = 3;
            break;
        case 6: //DO6
            pin = 2;
            break;
        case 7: //DO7
            pin = 1;
            break;
        case 8: //DO8
            pin = 0;
            break;
        default:
            return -1;
        }
    }

    if (circuit.startsWith("AO")) { //Raspberry Pi Analog Output
        switch (circuit.mid(2, 2).toInt()) {
        case 0: //AO GPIO18 PWM Analog Output 0-10V
            pin = 18;
            break;
        default:
            return -1;
        }
    }

    if (circuit.startsWith("AI")) { //MCP3422 analog input channels
        switch (circuit.mid(2, 2).toInt()) {
        case 1:
            pin = 1; //MCP3422 Channel 1
            break;
        case 2:
            pin = 2; //MCP3422 Channel 2
            break;
        default:
            return -1;
        }
    }
    return pin;
}

bool UniPi::setDigitalOutput(const QString &circuit, bool status)
{
    int pin = getPinFromCircuit(circuit);
    if (pin == -1) {
        qWarning(dcUniPi()) << "Out of range pin number";
        return false;
    }

    quint8 registerValue;
    if(!m_mcp23008->readRegister(MCP23008::RegisterAddress::OLAT, &registerValue))
        return false;
    if (status) {
        registerValue |= (1 << pin);
    } else {
        registerValue &= ~(1 << pin);
    }
    //write output register
    if(!m_mcp23008->writeRegister(MCP23008::RegisterAddress::OLAT, registerValue))
        return false;

    getDigitalOutput(circuit);
    return true;
}

bool UniPi::getDigitalOutput(const QString &circuit)
{
    int pin = getPinFromCircuit(circuit);
    if (pin == -1)
        return false;

    uint8_t registerValue;
    if(!m_mcp23008->readRegister(MCP23008::RegisterAddress::OLAT, &registerValue))
        return false;

    emit digitalOutputStatusChanged(circuit, ( registerValue & (1 << pin)));
    return  true;
}

bool UniPi::getDigitalInput(const QString &circuit)
{
    int pin = getPinFromCircuit(circuit);
    if (pin == -1) {
        qCWarning(dcUniPi()) << "Out of range pin number";
        return false;
    }
    if (!m_monitorGpios.values().contains(circuit)) {
        qCWarning(dcUniPi()) << "Could not read digital inpu, GPIO not initialized" << circuit;
        return false;
    }
    //Read RPi pins
    GpioMonitor *gpio = m_monitorGpios.key(circuit);
    digitalInputStatusChanged(circuit, gpio->value());
    return true;
}

bool UniPi::setAnalogOutput(double value)
{
    int percentage = value * 10;     //convert volt to percentage
    if(!m_analogOutput->setPercentage(percentage))
        return false;

    getAnalogOutput();
    return true;
}

bool UniPi::getAnalogOutput()
{
    double voltage = m_analogOutput->percentage()/10.0;
    emit analogOutputStatusChanged(voltage);

    return true;
}

void UniPi::onInputValueChanged(const bool &value)
{
    GpioMonitor *monitor = static_cast<GpioMonitor *>(sender());
    if (!m_monitorGpios.contains(monitor))
        return;

    QString circuit = m_monitorGpios.value(monitor);
    emit digitalInputStatusChanged(circuit, value);
}
