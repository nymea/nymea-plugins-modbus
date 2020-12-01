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
        BlockIdCommon          = 1,
        BlockIdBasicAggregator = 2,
        BlockIdSecureDatasetReadRequest     = 3,
        BlockIdSecureDatasetReadResponse    = 4,
        BlockIdSecureWriteRequest           = 5,
        BlockIdSecureWriteSequentialRequest = 6,
        BlockIdSecureWriteResponseModel     = 7,
        BlockIdGetDeviceSecurityCertificate = 8,
        BlockIdSetOperatorSecurityCertificate = 9,
        BlockIdCommunicationInterfaceHeader = 10,
        BlockIdEthernetLinkLayer        = 11,
        BlockIdIPv4                     = 12,
        BlockIdIPv6                     = 13,
        BlockIdProxyServer              = 14,
        BlockIdInterfaceCountersModel   = 15,
        BlockIdSimpleIpNetwork          = 16,
        BlockIdSerialInterface          = 17,
        BlockIdCellularLink             = 18,
        BlockIdPPPLink                  = 19,
        BlockIdInverterSinglePhase      = 101,
        BlockIdInverterSplitPhase       = 102,
        BlockIdInverterThreePhase       = 103,
        BlockIdInverterSinglePhaseFloat = 111,
        BlockIdInverterSplitPhaseFloat  = 112,
        BlockIdInverterThreePhaseFloat  = 113,
        BlockIdNameplate                = 120,
        BlockIdBasicSettings            = 121,
        BlockIdMeasurementsStatus       = 122,
        BlockIdImmediateControls        = 123,
        BlockIdStorage                  = 124,
        BlockIdPricing                  = 125,
        BlockIdStaticVoltVAR            = 126,
        BlockIdFreqWattParam            = 127,
        BlockIdDynamicReactiveCurrent   = 128,
        BlockIdLVRTD                    = 129,
        BlockIdHVRTD                    = 130,
        BlockIdWattPF                   = 131,
        BlockIdVoltWatt                 = 132,
        BlockIdBasicScheduling          = 133,
        BlockIdFreqWattCrv              = 134,
        BlockIdLFRT                     = 135,
        BlockIdHFRT                     = 136,
        BlockIdLVRTC                    = 137,
        BlockIdHVRTC                    = 138,
        BlockIdMultipleMPPTInverterExtensionModel = 160,
        BlockIdSinglePhaseMeter                 = 201,
        BlockIdSplitSinglePhaseMeter            = 202,
        BlockIdWyeConnectThreePhaseMeter        = 203,
        BlockIdDeltaConnectThreePhaseMeter      = 204,
        BlockIdSinglePhaseMeterFloat            = 211,
        BlockIdSplitSinglePhaseMeterFloat       = 212,
        BlockIdWyeConnectThreePhaseMeterFloat   = 213,
        BlockIdDeltaConnectThreePhaseMeterFloat = 214,
        BlockIdSecureACMeterSelectedReadings   = 220,
        BlockIdIrradianceModel                 = 302,
        BlockIdBackOfModuleTemperatureModel    = 303,
        BlockIdInclinometerModel               = 304,
        BlockIdGPS                             = 305,
        BlockIdReferencePointModel             = 306,
        BlockIdBaseMet                         = 307,
        BlockIdMiniMetModel                    = 308,
        BlockIdStringCombiner                  = 401,
        BlockIdStringCombinerAdvanced        = 402,
        BlockIdStringCombinerCurrent         = 403,
        BlockIdStringCombinerCurrentAdvanced = 404,
        BlockIdSolarModuleFloat          = 501,
        BlockIdSolarModule               = 502,
        BlockIdTrackerController         = 601,
        BlockIdEnergyStorageBaseModel    = 801,
        BlockIdBatteryBaseModel          = 802,
        BlockIdLithiumIonBatteryModel    = 803,
        BlockIdVerisStatusConfiguration  = 64001,
        BlockIdMersenGreenString         = 64020,
        BlockIdEltekInverterExtension    = 64101,
        BlockIdOutBackAXSDevice          = 64110,
        BlockIdBasicChargeController     = 64111,
        BlockIdOutBackFMChargeController = 64112,
        BlockIdEnd                       = 65535
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
    QString m_manufacturer = "Unknown";
    QString m_deviceModel  = "Unknown";
    QString m_serialNumber = "Unknown";

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
    void onReceivedHoldingRegister(quint32 slaveAddress, quint32 modbusRegister, const QVector<quint16> &values);
};

#endif // SUNSPEC_H
