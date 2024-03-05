# nymea-modbus-cli

The nymea-modbus-cli tools is a command line tool for testing modbus TCP/RTU communication.

The tool allows to read or write registers in a generic way.

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
