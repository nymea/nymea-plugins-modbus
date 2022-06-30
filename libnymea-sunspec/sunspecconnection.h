/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2021, nymea GmbH
* Contact: contact@nymea.io
*
* This fileDescriptor is part of nymea.
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

#ifndef SUNSPECCONNECTION_H
#define SUNSPECCONNECTION_H

#include <QObject>
#include <QtSerialBus>
#include <QHostAddress>
#include <QModbusTcpClient>
#include <QLoggingCategory>

#include "sunspecdatapoint.h"

Q_DECLARE_LOGGING_CATEGORY(dcSunSpec)

class SunSpecModel;

class SunSpecConnection : public QObject
{
    Q_OBJECT
public:
    explicit SunSpecConnection(const QHostAddress &hostAddress, uint port = 502, uint slaveId = 1, QObject *parent = nullptr);
    explicit SunSpecConnection(const QHostAddress &hostAddress, uint port = 502, uint slaveId = 1, SunSpecDataPoint::ByteOrder byteOrder = SunSpecDataPoint::ByteOrderLittleEndian, QObject *parent = nullptr);
    ~SunSpecConnection() = default;

    QModbusTcpClient *modbusTcpClient() const;

    QHostAddress hostAddress() const;
    void setHostAddress(const QHostAddress &hostAddress);

    uint port() const;
    void setPort(uint port);

    uint slaveId() const;
    void setSlaveId(uint slaveId);

    SunSpecDataPoint::ByteOrder byteOrder() const;

    int timeout() const;
    void setTimeout(int milliSeconds);

    uint numberOfRetries() const;
    void setNumberOfRetries(uint retries);

    bool connected() const;
    bool discoveryRunning() const;

    quint16 baseRegister() const;

    QList<SunSpecModel *> models() const;

    // Helper methods for internal queue handling if enabled
    QModbusReply *sendReadRequest(const QModbusDataUnit &read, int serverAddress);
    QModbusReply *sendWriteRequest(const QModbusDataUnit &write, int serverAddress);
    QModbusReply *sendRawRequest(const QModbusRequest &request, int serverAddress);

public slots:
    bool startDiscovery();
    bool connectDevice();
    void disconnectDevice();
    bool reconnectDevice();

signals:
    void connectedChanged(bool connected);
    void discoveryRunningChanged(bool discoveryRunning);
    void sunspecBaseRegisterFound(quint16 baseRegister);
    void discoveryFinished(bool success);

private:
    QModbusTcpClient *m_modbusTcpClient = nullptr;
    QHostAddress m_hostAddress;
    uint m_port;
    int m_slaveId = 1;
    QTimer m_reconnectTimer;
    bool m_connected = false;

    quint16 m_baseRegister = 40000;
    QQueue<quint16> m_baseRegisterQueue;

    // SunSpec discovery
    typedef struct ModuleDiscoveryResult {
        quint16 modbusStartRegister;
        quint16 modelId;
        quint16 modelLength;
    } ModuleDiscoveryResult;

    bool m_discoveryRunning = false;
    QList<ModuleDiscoveryResult> m_modelDiscoveryResult;
    QList<SunSpecModel *> m_models;
    QList<SunSpecModel *> m_uninitializedModels;
    SunSpecDataPoint::ByteOrder m_byteOrder = SunSpecDataPoint::ByteOrderLittleEndian;

    int m_timoutReplyCounter = 0;
    int m_timoutReplyCounterLimit = 16;

    void createConnection();

    void processDiscoveryResult();

    void setDiscoveryRunning(bool discoveryRunning);
    bool modelAlreadyAdded(SunSpecModel *model) const;

    bool scanSunspecBaseRegister(quint16 baseRegister);
    void scanNextSunspecBaseRegister();

    void scanModelsOnBaseRegister(quint16 offset = 2);

    void monitorTimoutErrors(QModbusReply *reply);
};

QDebug operator<<(QDebug debug, SunSpecConnection *connection);


#endif // SUNSPECCONNECTION_H
