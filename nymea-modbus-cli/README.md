# nymea-modbus-cli

The nymea-modbus-cli tool tests Modbus TCP, Modbus TCP over TLS, and Modbus RTU communication.

The tool allows to read or write registers in a generic way.

## TLS

Use `--tls` for Modbus TCP over TLS. Port 802 is used when no explicit port is
given. A server certificate can be pinned using its SHA-256 fingerprint:

```
nymea-modbus-cli -a 192.168.0.10 --tls --tls-version 1.2 \
    --tls-fingerprint <sha256> -r 1000 -l 2
```

For a server requiring mutual TLS, provide the PEM client certificate and its
private key. The certificate file may contain the leaf certificate followed by
intermediate certificates:

```
nymea-modbus-cli -a 192.168.0.10 --tls --tls-version 1.2 \
    --tls-server-name device.example.local \
    --tls-fingerprint <sha256> \
    --tls-client-certificate client-certificate.pem \
    --tls-client-key client-key.pem \
    -r 1000 -l 2
```

For an encrypted private key, put only its passphrase in a file readable by the
current user and add `--tls-client-key-passphrase-file <file>`. Keeping the
passphrase out of the command line prevents it from being exposed in process
listings.

### Self-signed certificate for TOFU

The following commands create an EC P-256 private key and a self-signed client
certificate using ECDSA with SHA-256. This is suitable for a server requesting
an `ECDSA+SHA256` client certificate and enrolling it using trust on first use
(TOFU):

```
umask 077
openssl genpkey -algorithm EC -pkeyopt ec_paramgen_curve:P-256 \
    -out client-key.pem
openssl pkey -in client-key.pem -pubout -out client-public-key.pem
openssl req -new -x509 -sha256 -days 3650 \
    -key client-key.pem \
    -out client-certificate.pem \
    -subj "/CN=nymea-modbus-client" \
    -addext "basicConstraints=critical,CA:FALSE" \
    -addext "keyUsage=critical,digitalSignature" \
    -addext "extendedKeyUsage=clientAuth"
```

Keep both files stable after the device accepts the certificate for the first
time. Replacing either the key or certificate changes the client identity and
normally requires clearing or repeating the device's enrollment. Inspect the
generated certificate with:

```
openssl x509 -in client-certificate.pem -noout -subject -issuer -dates -text
openssl verify -CAfile client-certificate.pem client-certificate.pem
```

### CA-signed certificate

If the server requires a certificate issued by an operator or manufacturer CA,
generate the EC P-256 private key as above but create a certificate signing
request instead of the self-signed certificate:

```
openssl req -new -sha256 \
    -key client-key.pem \
    -out client.csr \
    -subj "/CN=nymea-modbus-client" \
    -addext "basicConstraints=critical,CA:FALSE" \
    -addext "keyUsage=critical,digitalSignature" \
    -addext "extendedKeyUsage=clientAuth"
```

Send `client.csr` to the appropriate CA. Store the returned leaf certificate as
`client-certificate.pem`; intermediate certificates may follow it in the same
PEM file. Inspect and verify the returned certificate with:

```
openssl x509 -in client-certificate.pem -noout -subject -issuer -dates -text
openssl verify -CAfile client-ca.pem client-certificate.pem
```

To generate an encrypted key instead, add `-aes-256-cbc` to the
`openssl genpkey` command and use `--tls-client-key-passphrase-file` when invoking the
CLI.

`--tls-info` performs one TLS handshake, prints the negotiated protocol and
cipher, timing, certificate chain, public keys, SANs, and extensions, and exits
without sending Modbus traffic:

```
nymea-modbus-cli -a 192.168.0.10 --tls-info --tls-version 1.2
```

Without `--tls-fingerprint`, normal CA and hostname validation is used. If that
fails, the tool prints the observed leaf fingerprint so the operator can verify
it out of band and retry with an explicit pin. It never saves certificates or
TLS sessions and does not block the TLS handshake waiting for terminal input.

Following options are available:

```
Usage: nymea-modbus-cli [options]

Tool for testing and reading Modbus TCP or RTU registers.

Copyright © 2016 - 2023 nymea GmbH <contact@nymea.io>

TCP
-----------------------------------------
Example reading 2 holding registers from address 1000:
nymea-modbus-cli -a 192.168.0.10 -p 502 -r 1000 -l 2

RTU
-----------------------------------------

Typical baudrates:
- 1200
- 2400
- 4800
- 9600
- 19200
- 38400
- 57600
- 115200

Example reading 2 holding registers from address 1000:
nymea-modbus-cli --serial /dev/ttyUSB0 --baudrate 9600 -r 1000 -l 2



Options:
  -h, --help                                    Displays help on commandline
                                                options.
  --help-all                                    Displays help including Qt
                                                specific options.
  -v, --version                                 Displays version information.
  -a, --address <address>                       TCP: The IP address of the
                                                modbus TCP server.
  -p, --port <port>                             TCP: The port of the modbus TCP
                                                server. Default is 502.
  --serial <port>                               RTU: The serial port to use for
                                                the RTU communication.
  --baudrate <baudrate>                         RTU: The baudrate for the RTU
                                                communication. Default is 19200.
  --parity <parity>                             RTU: The parity for the RTU
                                                communication. Allowed values
                                                are [none, even, odd, space,
                                                mark]. Default is none.
  --databits <databits>                         RTU: The amount of data bits
                                                for the RTU communication.
                                                Allowed values are [5, 6, 7, 8].
                                                Default is 8.
  --stopbits <stopbits>                         RTU: The amount of stop bits
                                                for the RTU communication.
                                                Allowed values are [1, 1.5, 2].
                                                Default is 1.
  --list-serials                                List the available serial ports
                                                on this host.
  -m, --modbus-address <id>                     The modbus server address on
                                                the bus (slave ID). Default is
                                                1.
  -t, --type <input, holding, discrete, coils>  The type of the modbus
                                                register. Default is holding.
  -r, --register <register>                     The number of the modbus
                                                register.
  -l, --length <length>                         The number of registers to
                                                read. Default is 1.
  -w, --write <data>                            The data to be written to the
                                                given register.
  -d, --debug                                   Print more information.
```
