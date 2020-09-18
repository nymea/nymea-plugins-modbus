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

#ifndef SUNSPEC_H
#define SUNSPEC_H

#include <QObject>
#include <QUuid>
#include <QHostAddress>
#include <QtSerialBus>

class SunSpec : public QObject
{
    Q_OBJECT

public:

    enum MandatoryRegistersModel1 {
        Manufacturer = 2,
        Model = 18,
        SerialNumber = 50
    };

    enum OptionalRegistersModel1 {
        Options = 34,
        Version = 40,
        DeviceAddress = 64,
        ForceAlignment = 65
    };

    enum SunSpecOperatingState {
        Off = 1,
        Sleeping,
        Starting,
        MPPT,
        Throttled,
        ShuttingDown,
        Fault,
        Standby
    };
    Q_ENUM(SunSpecOperatingState)

    enum SunSpecEvent1 {
        GroundFault = 0,
        DcOverVolatage,
        AC_DISCONNECT,
        DC_DISCONNECT,
        GRID_DISCONNECT,
        CABINET_OPEN,
        MANUAL_SHUTDOWN,
        OVER_TEMP,
        OVER_FREQUENCY,
        UNDER_FREQUENCY,
        AC_OVER_VOLT,
        AC_UNDER_VOLT,
        BLOWN_STRING_FUSE,
        UNDER_TEMP,
        MEMORY_LOSS,
        HW_TEST_FAILURE
    };
    Q_ENUM(SunSpecEvent1)

    enum BlockId {
        Common          = 1,
        BasicAggregator = 2,
        SecureDatasetReadRequest     = 3,
        SecureDatasetReadResponse    = 4,
        SecureWriteRequest           = 5,
        SecureWriteSequentialRequest = 6,
        SecureWriteResponseModel     = 7,
        GetDeviceSecurityCertificate = 8,
        SetOperatorSecurityCertificate = 9,
        CommunicationInterfaceHeader = 10,
        EthernetLinkLayer        = 11,
        IPv4                     = 12,
        IPv6                     = 13,
        ProxyServer              = 14,
        InterfaceCountersModel   = 15,
        SimpleIpNetwork          = 16,
        SerialInterface          = 17,
        CellularLink             = 18,
        PPPLink                  = 19,
        InverterSinglePhase      = 101,
        InverterSplitPhase       = 102,
        InverterThreePhase       = 103,
        InverterSinglePhaseFloat = 111,
        InverterSplitPhaseFloat  = 112,
        InverterThreePhaseFloat  = 113,
        Nameplate                = 120,
        BasicSettings            = 121,
        MeasurementsStatus       = 122,
        ImmediateControls        = 123,
        Storage                  = 124,
        Pricing                  = 125,
        StaticVoltVAR            = 126,
        FreqWattParam            = 127,
        DynamicReactiveCurrent   = 128,
        LVRTD                    = 129,
        HVRTD                    = 130,
        WattPF                   = 131,
        VoltWatt                 = 132,
        BasicScheduling          = 133,
        FreqWattCrv              = 134,
        LFRT                     = 135,
        HFRT                     = 136,
        LVRTC                    = 137,
        HVRTC                    = 138,
        MultipleMPPTInverterExtensionModel = 160,
        SinglePhaseMeter                 = 201,
        SplitSinglePhaseMeter            = 202,
        WyeConnectThreePhaseMeter        = 203,
        DeltaConnectThreePhaseMeter      = 204,
        SinglePhaseMeterFloat            = 211,
        SplitSinglePhaseMeterFloat       = 212,
        WyeConnectThreePhaseMeterFloat   = 213,
        DeltaConnectThreePhaseMeterFloat = 214,
        SecureACMeterSelectedReadings   = 220,
        IrradianceModel                 = 302,
        BackOfModuleTemperatureModel    = 303,
        InclinometerModel               = 304,
        GPS                             = 305,
        ReferencePointModel             = 306,
        BaseMet                         = 307,
        MiniMetModel                    = 308,
        StringCombiner                  = 401,
        StringCombinerAdvanced        = 402,
        StringCombinerCurrent         = 403,
        StringCombinerCurrentAdvanced = 404,
        SolarModuleFloat          = 501,
        SolarModule               = 502,
        TrackerController         = 601,
        EnergyStorageBaseModel    = 801,
        BatteryBaseModel          = 802,
        LithiumIonBatteryModel    = 803,
        VerisStatusConfiguration  = 64001,
        MersenGreenString         = 64020,
        EltekInverterExtension    = 64101,
        OutBackAXSDevice          = 64110,
        BasicChargeController     = 64111,
        OutBackFMChargeController = 64112,
        End                       = 65535
    };
    Q_ENUM(BlockId)

    explicit SunSpec(const QHostAddress &hostAddress, uint port = 502, QObject *parent = 0);
    ~SunSpec();
    bool connectModbus();
    QString manufacturer();
    QString deviceModel();
    QString serialNumber();

    QHostAddress m_hostAddress;
    uint m_port;
    QModbusTcpClient *m_modbusTcpClient = nullptr;
    int m_slaveId = 1;
    int m_baseRegister = 40000;
    bool m_floatingPointRepresentation = false;
    QString m_manufacturer = "unkown";
    QString m_deviceModel  = "unknown";
    QString m_serialNumber = "unknown";

    void findBaseRegister();
    void findModbusMap(const QList<BlockId> &mapIds, uint modbusAddressOffset = 69);

    void readMapHeader(uint modbusAddress);
    void readMap(uint modbusAddress, uint modelLength); //modbusAddress = model start address, model length is without header
    void readCommonMap();

    float convertValueWithSSF(quint16 rawValue, quint16 sunssf);
    float convertFloatValues(quint16 rawValue0, quint16 rawValue1);
    QByteArray convertModbusRegister(const uint16_t &modbusData);
    QBitArray convertModbusRegisterBits(const uint16_t &modbusData);
    QByteArray convertModbusRegisters(const QVector<quint16> &modbusData, int offset, int size);

    QUuid writeHoldingRegister(uint slaveAddress, uint registerAddress, quint16 value);
    QUuid writeHoldingRegisters(uint slaveAddress, uint registerAddress, const QVector<quint16> &values);

signals:
    void connectionStateChanged(bool status);
    void requestExecuted(QUuid requetId, bool success);

    void foundModbusMap(BlockId mapId, int modbusStartRegister);
    void modbusMapSearchFinished(const QList<BlockId> &mapIds, uint modbusStartRegister, const QString &error);

    void mapHeaderReceived(uint modbusAddress, BlockId mapId, uint mapLength);
    void mapReceived(BlockId mapId, uint mapLength, QVector<quint16> data);

private slots:
    void onModbusStateChanged(QModbusDevice::State state);

    void onRequestExecuted(QUuid requestId, bool success);
    void onRequestError(QUuid requestId, const QString &error);
    void onReceivedHoldingRegister(quint32 slaveAddress, quint32 modbusRegister, const QVector<quint16> &values);
};

#endif // SUNSPEC_H
