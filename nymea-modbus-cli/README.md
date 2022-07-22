# nymea-modbus-cli

The nymea-modbus-cli tools is a command line tool for tsting modbus TCP communication in the local network.

As of now, the tool allows to establish a modbus TCP connection to a server and read any type of register from the server.

Following options are available:

    Usage: nymea-modbus-cli [options]
                                                                                                                                                                                     
    Tool for testing and reading Modbus TCP registers.                                                                                                                               
                                                                                                                                                                                     
    Copyright © 2016 - 2022 nymea GmbH <contact@nymea.io>                                                                                                                            
                                                                                                                                                                                     
                                                                                                                                                                                     
    Options:                                                                                                                                                                         
      -h, --help                                    Displays this help.                                                                                                              
      -v, --version                                 Displays version information.
      -a, --address <address>                       The IP address of the modbus
                                                    TCP server.
      -p, --port <port>                             The port of the modbus TCP
                                                    server. Default is 502.
      -m, --modbus-address <id>                     The modbus server address on
                                                    the bus (slave ID). Default is
                                                    1.
      -t, --type <input, holding, discrete, coils>  The type of the modbus
                                                    register. Default is holding.
      -r, --register <register>                     The number of the modbus
                                                    register.
      -l, --length <length>                         The number of registers to
                                                    read. Default is 1.
      -d, --debug                                   Print more information.
    