# PC Electric EV11.3

This integration connects PC Electric EV11.3 wallboxes to nymea over Modbus
TCP. Depending on the wallbox firmware, communication uses either plain Modbus
TCP or mutually authenticated Modbus TCP over TLS.

## Supported wallboxes

The following EV11.3 models are supported:

- EV11.3 Wallbox Basic (`37031110-ss`)
- EV11.3 Wallbox Basic Z (`37031111-ss`)

Nymea exposes the detected wallbox as one of two device types:

| Device type | Intended configuration | Additional data |
| --- | --- | --- |
| **PCE EV11.3** | Wallbox with an energy meter | Active power, session energy, phase voltages and phase currents |
| **PCE EV11.3 (no meter)** | Wallbox without an energy meter | Charging and wallbox status without meter measurements |

Discovery determines the device type from the firmware revision, digital-input
mode, and R37 monitoring mode. It does not rely only on the advertised product
name. If a configuration is ambiguous, discovery selects the meter-equipped
device type.

Both device types support the common wallbox functions, including:

- enabling and disabling charging;
- setting the maximum charging current from 6 A to 16 A;
- reporting the plug, charging, error, and temperature states;
- selecting one-phase or three-phase charging when automatic phase switching
  is enabled by the wallbox hardware configuration;
- configuring LED brightness, digital-input mode, phase-switch pause, and
  minimum charging time where supported by the firmware;
- refreshing the TLS client certificate without changing the paired key.

## Requirements

- The wallbox and nymea must be reachable on the same local network.
- TCP port 502 must be reachable for discovery and bootstrap communication.
- Wallboxes with firmware newer than 0.25 also require TCP port 802 for TLS.
- `openssl` must be installed on the nymea system if the plugin needs to create
  or refresh its TLS client certificate.
- Automatic phase switching must be enabled using the wallbox DIP switches
  before nymea can select between one and three phases.

## Discovery and setup

The plugin discovers EV11.3 wallboxes through the plain `_modbus._tcp` mDNS
service or through network discovery. A wallbox can also be added manually by
IP address.

Every setup starts with plain Modbus TCP on port 502. The firmware revision is
always read directly from the wallbox and is the only source used to select the
operational transport. TLS information advertised through ZeroConf is ignored.

The transport selection is:

| Firmware revision | Operational connection |
| --- | --- |
| 0.25 and older | Plain Modbus TCP on port 502 |
| Newer than 0.25 | Modbus TCP over TLS 1.2 on port 802 |

For TLS wallboxes, port 502 remains a restricted bootstrap and diagnostic
endpoint. Only these registers are expected to be readable there:

- `MODE_DIGITAL_IN`
- `FIRMWARE_REVISION`
- `HARDWARE_REVISION`
- `SERIAL_NUMBER`
- `MAC`
- `R37_MODE`

All other operational registers are accessed only after the TLS connection on
port 802 has completed. A TLS handshake can take approximately seven seconds;
the plugin waits for the handshake rather than treating the earlier TCP connect
as an operational connection.

## TLS identity and certificate pinning

Firmware newer than 0.25 uses mutual TLS 1.2: the wallbox authenticates nymea,
and nymea authenticates the wallbox. When no client identity exists, the plugin
creates a self-signed P-256 EC client certificate with a validity of ten years.
The default files are:

- `pcelectric/client-certificate.pem`
- `pcelectric/client-key.pem`

Relative paths are resolved below the nymea settings directory. Alternative
paths can be configured using the plugin settings **TLS client certificate
path** and **TLS client key path**. The private key must remain private and must
be included in system backups; losing it changes nymea's TLS identity.

The wallbox and nymea use Trust On First Use (TOFU). The pinned identity is the
SHA-256 hash of the DER-encoded SubjectPublicKeyInfo, commonly called
**SPKI-SHA256**. It is a public-key pin, not a hash of the complete certificate:

- the wallbox stores the SPKI pin of nymea's client key;
- nymea stores an individual wallbox SPKI pin for each configured wallbox;
- later connections are accepted only when the peer presents the pinned key;
- certificate renewal remains possible as long as the underlying key is
  unchanged.

The first TLS connection must be performed on a trusted local network. TOFU
cannot detect an attacker impersonating the wallbox during the first pairing.
The wallbox normally permits initial pairing for 60 minutes after power-up or
after a local pairing reset.

For diagnosis, debug logging shows the configured pin, the server SPKI-SHA256,
whether both fingerprints match, and the client certificate validity and
fingerprints. Older installations that stored a complete-certificate SHA-256
pin are migrated to SPKI-SHA256 when that same wallbox certificate is seen
during the first connection after upgrading.

## Refreshing the client certificate

Use the wallbox action **Refresh client certificate** before the current client
certificate expires. The action:

1. reads the existing P-256 private key;
2. generates a new self-signed certificate valid for another ten years;
3. verifies that the old and new SPKI-SHA256 fingerprints are identical;
4. atomically replaces only the certificate file;
5. reconnects configured PCE wallboxes that use TLS.

The private key and all existing pins remain unchanged, so the wallbox continues
to recognize nymea. If certificate generation, fingerprint verification, or
file replacement fails, the previous identity is retained.

Do not manually replace or delete the client private key on a paired system. A
new key has a different SPKI fingerprint and will be rejected by every wallbox
that pinned the previous key. If the key is lost or intentionally replaced,
each wallbox must be unpaired locally before it can learn the new nymea key. On
wallboxes implementing the supplied TOFU/RFID specification, this is done while
not charging by holding the RFID Master Tag at the reader for 15 to 20 seconds
and removing it within that interval. Successful unpairing opens a new
60-minute pairing window.

## Troubleshooting

### Port 502 works, but the wallbox stays disconnected

For firmware newer than 0.25, a successful connection to port 502 only proves
that the bootstrap endpoint is reachable. Check that port 802 is reachable and
allow enough time for the mutual TLS handshake.

### TLS connections are repeatedly rejected

Check the debug log for the pinned and server SPKI-SHA256 values. Avoid rapid
manual retries: the wallbox may temporarily rate-limit an address after several
failed handshakes. Also verify that the configured client certificate and key
belong together and have not been replaced since pairing.

### A replacement wallbox or a new wallbox key is rejected

The stored server pin deliberately prevents silently accepting a different
wallbox key. Verify the physical device and its serial number before removing
and adding the wallbox again to establish a new TOFU pin.

### Phase selection is fixed to three phases

Verify that automatic phase switching is enabled by the wallbox DIP switches.
When the feature is disabled, the integration exposes only three-phase
operation.

## Further information

Product and operating documentation is available from
[PC Electric](https://www.pcelectric.at/).
