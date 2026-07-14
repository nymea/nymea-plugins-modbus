# nymea plugins modbus

--------------------------------------------
<p align="center">
  <a  href="https://nymea.io">
    <img src="https://nymea.io/downloads/img/nymea-logo.svg" width=300>
  </a>
</p>


This repository contains modbus integrations for nymea. 

## Modbus TCP over TLS

`ModbusTcpMaster` supports plain TCP by default and an opt-in TLS transport. The
existing API remains source-compatible, so current integrations keep using
plain Modbus TCP until they explicitly select TLS.

```cpp
ModbusTcpMaster *master = new ModbusTcpMaster(address, 802, parent);
master->setTransport(ModbusTcpMaster::TransportTls);

QSslConfiguration tls = QSslConfiguration::defaultConfiguration();
tls.setProtocol(QSsl::TlsV1_2);
// tls.setLocalCertificate(clientCertificate); // Optional mutual TLS
// tls.setPrivateKey(clientPrivateKey);
master->setTlsConfiguration(tls);
master->setAcceptedPeerCertificateFingerprint(serverCertificateSha256);
master->connectDevice();
```

If no fingerprint is configured, Qt's normal CA-chain and hostname validation
is used. A configured fingerprint pins the leaf certificate and intentionally
overrides CA validation errors. `peerCertificateAvailable` is informational;
an asynchronous consumer can verify and store the reported fingerprint, then
call `reconnectDevice()` after applying it. The library never pauses a TLS
handshake waiting for user interaction.

Keeping the same client certificate and private key provides a stable client
identity but does not by itself shorten a TLS 1.2 handshake. Session resumption
would provide the relevant latency improvement; it is deliberately not enabled
or persisted by the library yet. Callers should keep one connection open where
possible instead of reconnecting for each Modbus operation.

nymea (/[n'aiːmea:]/ - is an open source IoT edge server. The plug-in based architecture allows to integrate protocols and APIs. With the build-in rule engine you are able to interconnect devices or services available in the system and create individual scenes and behaviours for your environment.

## Documentation

* A detailed description how to install and getting started with the *nymea* can be found here:

    [nymea | user documentation](https://nymea.io/documentation/users/installation/getting-started).

* A detailed documentation for developers can be found here:

    [nymea | developer documentation](https://nymea.io/documentation/developers/).


## License
--------------------------------------------
The nymea modbus plugins inside this repository are licensed under the GNU General Public License, version 3 or (at your option) any later version. The reusable libraries `libnymea-modbus` and `libnymea-sunspec` are licensed under the GNU Lesser General Public License, version 3 or (at your option) any later version. 

The full license texts can be found in [`LICENSE.GPL3`](LICENSE.GPL3) for the GPL components and [`LICENSE.LGPL3`](LICENSE.LGPL3) for the LGPL libraries.
