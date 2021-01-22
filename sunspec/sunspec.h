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

    enum ModelId {
        ModelIdCommon          = 1,
        ModelIdBasicAggregator = 2,
        ModelIdSecureDatasetReadRequest     = 3,
        ModelIdSecureDatasetReadResponse    = 4,
        ModelIdSecureWriteRequest           = 5,
        ModelIdSecureWriteSequentialRequest = 6,
        ModelIdSecureWriteResponseModel     = 7,
        ModelIdGetDeviceSecurityCertificate = 8,
        ModelIdSetOperatorSecurityCertificate = 9,
        ModelIdCommunicationInterfaceHeader = 10,
        ModelIdEthernetLinkLayer        = 11,
        ModelIdIPv4                     = 12,
        ModelIdIPv6                     = 13,
        ModelIdProxyServer              = 14,
        ModelIdInterfaceCountersModel   = 15,
        ModelIdSimpleIpNetwork          = 16,
        ModelIdSerialInterface          = 17,
        ModelIdCellularLink             = 18,
        ModelIdPPPLink                  = 19,
        ModelIdInverterSinglePhase      = 101,
        ModelIdInverterSplitPhase       = 102,
        ModelIdInverterThreePhase       = 103,
        ModelIdInverterSinglePhaseFloat = 111,
        ModelIdInverterSplitPhaseFloat  = 112,
        ModelIdInverterThreePhaseFloat  = 113,
        ModelIdNameplate                = 120,
        ModelIdBasicSettings            = 121,
        ModelIdMeasurementsStatus       = 122,
        ModelIdImmediateControls        = 123,
        ModelIdStorage                  = 124,
        ModelIdPricing                  = 125,
        ModelIdStaticVoltVAR            = 126,
        ModelIdFreqWattParam            = 127,
        ModelIdDynamicReactiveCurrent   = 128,
        ModelIdLVRTD                    = 129,
        ModelIdHVRTD                    = 130,
        ModelIdWattPF                   = 131,
        ModelIdVoltWatt                 = 132,
        ModelIdBasicScheduling          = 133,
        ModelIdFreqWattCrv              = 134,
        ModelIdLFRT                     = 135,
        ModelIdHFRT                     = 136,
        ModelIdLVRTC                    = 137,
        ModelIdHVRTC                    = 138,
        ModelIdMultipleMPPTInverterExtensionModel = 160,
        ModelIdSinglePhaseMeter                 = 201,
        ModelIdSplitSinglePhaseMeter            = 202,
        ModelIdWyeConnectThreePhaseMeter        = 203,
        ModelIdDeltaConnectThreePhaseMeter      = 204,
        ModelIdSinglePhaseMeterFloat            = 211,
        ModelIdSplitSinglePhaseMeterFloat       = 212,
        ModelIdWyeConnectThreePhaseMeterFloat   = 213,
        ModelIdDeltaConnectThreePhaseMeterFloat = 214,
        ModelIdSecureACMeterSelectedReadings   = 220,
        ModelIdIrradianceModel                 = 302,
        ModelIdBackOfModuleTemperatureModel    = 303,
        ModelIdInclinometerModel               = 304,
        ModelIdGPS                             = 305,
        ModelIdReferencePointModel             = 306,
        ModelIdBaseMet                         = 307,
        ModelIdMiniMetModel                    = 308,
        ModelIdStringCombiner                  = 401,
        ModelIdStringCombinerAdvanced        = 402,
        ModelIdStringCombinerCurrent         = 403,
        ModelIdStringCombinerCurrentAdvanced = 404,
        ModelIdSolarModuleFloat          = 501,
        ModelIdSolarModule               = 502,
        ModelIdTrackerController         = 601,
        ModelIdEnergyStorageBaseModel    = 801,
        ModelIdBatteryBaseModel          = 802,
        ModelIdLithiumIonBatteryModel    = 803,
        ModelIdVerisStatusConfiguration  = 64001,
        ModelIdMersenGreenString         = 64020,
        ModelIdEltekInverterExtension    = 64101,
        ModelIdOutBackAXSDevice          = 64110,
        ModelIdBasicChargeController     = 64111,
        ModelIdOutBackFMChargeController = 64112,
        ModelIdEnd                       = 65535
    };
    Q_ENUM(ModelId)

    explicit SunSpec(const QHostAddress &hostAddress, uint port = 502, uint slaveId = 1, QObject *parent = 0);
    ~SunSpec();
    bool connectModbus();
    void setHostAddress(const QHostAddress &hostAddress);
    void setPort(uint port);
    void setSlaveId(uint slaveId);
    void setTimeout(uint milliSeconds);
    void setNumberOfRetries(uint retries);

    QHostAddress hostAddress() const;
    uint port();
    uint slaveId();
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
    QHash<ModelId, int> m_modelList;

    void findBaseRegister();
    void findSunSpecModels(const QList<ModelId> &modelIds, uint modbusAddressOffset = 2);

    void readCommonModel();
    void readModelHeader(uint modbusAddress);
    void readModelDataBlock(uint modbusAddress, uint modelLength); //modbusAddress = model start address, model length is without header

    float convertValueWithSSF(quint32 rawValue, quint16 sunssf);
    float convertFloatValues(quint16 rawValue0, quint16 rawValue1);
    QByteArray convertModbusRegister(const uint16_t &modbusData);
    QBitArray convertModbusRegisterBits(const uint16_t &modbusData);
    QByteArray convertModbusRegisters(const QVector<quint16> &modbusData, int offset, int size);

    QUuid writeHoldingRegister(uint registerAddress, quint16 value);
    QUuid writeHoldingRegisters(uint registerAddress, const QVector<quint16> &values);

signals:
    void connectionStateChanged(bool status);
    void requestExecuted(const QUuid &requestId, bool success);

    void foundBaseRegister(int modbusAddress);
    void commonModelReceived(const QString &manufacturer, const QString &deviceModel, const QString &serialNumber);

    void foundSunSpecModel(ModelId modelId, int modbusStartRegister);
    void sunspecModelSearchFinished(const QHash<ModelId, int> &mapIds);

    void modelHeaderReceived(uint modbusAddress, ModelId mapId, uint mapLength);
    void modelDataBlockReceived(ModelId id, uint ength, QVector<quint16> data);

private slots:
    void onModbusStateChanged(QModbusDevice::State state);
};

#endif // SUNSPEC_H
