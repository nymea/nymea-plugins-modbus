# nymea-modbus-cli

The nymea-modbus-cli tool tests Modbus TCP, Modbus TCP over TLS, and Modbus RTU communication.

The tool allows to read or write registers in a generic way.

## Reading registers

Reads use zero-based register addresses. The default type is `holding`, and the
default length is one value:

```
nymea-modbus-cli -a 192.168.0.10 -r 100 -l 2
```

Holding and input registers are printed as a diagnostic table containing raw
hex, signed and unsigned 16-bit values in both byte orders, and escaped ASCII.
The complete response is also shown as bulk raw hex and ASCII. Coils and
discrete inputs use a simpler address/raw/Boolean table.

Wider values can be decoded by repeating `--decode` with any of `uint16`,
`int16`, `uint32`, `int32`, `uint64`, `int64`, `float32`, or `float64`:

```
nymea-modbus-cli -a 192.168.0.10 -r 100 -l 2 \
    --decode uint32 --decode float32
```

`--byte-order big|little` controls byte order inside each 16-bit register.
`--word-order forward|reverse` controls word order inside each decoded value.
Both default to the normal Modbus representation: big-byte order with the
first register as the first word.

Use `--output json` for stable machine-readable output or `--output legacy` for
the previous one-line-per-register output.

## Writing registers and coils

The write count is inferred from `--write`. Decimal and hexadecimal word lists
may be mixed:

```
nymea-modbus-cli -a 192.168.0.10 -r 100 --write 1,0x0002,65535
```

A contiguous hexadecimal string is split into consecutive 16-bit registers,
left to right. For example, this writes `0x0001` to register 100 and `0x0002`
to register 101:

```
nymea-modbus-cli -a 192.168.0.10 -r 100 --write 0x00010002
```

An explicitly supplied `--length` must match the inferred count. Raw compact
hex always represents wire/register order and is not affected by decoding
endianness options.

Coils accept Boolean lists:

```
nymea-modbus-cli -a 192.168.0.10 -t coils -r 20 \
    --write true,false,1,0
```

Input registers and discrete inputs are read-only. Broadcast requests are
supported for writes only.

## TLS

Use `--tls` for Modbus TCP over TLS. Port 802 is used when no explicit port is
given. A server certificate can be pinned using the SHA-256 fingerprint of its
DER-encoded SubjectPublicKeyInfo (SPKI):

```
nymea-modbus-cli -a 192.168.0.10 --tls --tls-version 1.2 \
    --tls-fingerprint <sha256> -r 1000 -l 2
```

For diagnostics in a controlled network, server identity verification can be
disabled explicitly:

```
nymea-modbus-cli -a 192.168.0.10 --tls --tls-version 1.2 \
    --tls-accept-any-fingerprint -r 1000 -l 2
```

`--tls-accept-any-fingerprint` accepts the SPKI fingerprint presented during
each handshake and ignores CA, hostname, and pin mismatches. It cannot be used
together with `--tls-fingerprint`. This option still encrypts the connection,
but it does not authenticate the server and is therefore vulnerable to a
man-in-the-middle attack. Use it only for temporary diagnostics, never as a
substitute for verifying and pinning the displayed fingerprint.

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

Keep the private key stable after the device accepts it for the first time. A
certificate may be renewed using that same key when the device pins SPKI, but
replacing the key changes the client identity and normally requires clearing or
repeating the device's enrollment. Inspect the generated certificate with:

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
fails, the tool prints the observed leaf SPKI fingerprint so the operator can verify
it out of band and retry with an explicit pin. It never saves certificates or
TLS sessions and does not block the TLS handshake waiting for terminal input.

Run `nymea-modbus-cli --help` or `man nymea-modbus-cli` for the complete option
reference.
