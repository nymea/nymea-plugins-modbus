# Schrack CION

This plugin adds support for the Schrack CION wallbox to nymea.

The wallbox needs to be connected to the nymea system using an RS485 interface which needs to be configured as a Modbus RTU interface with the following parameters:
* Baudrate: 57600 
* Data bits: 8
* Stop bit: 1
* Parity: None

Once the Modbus RTU interface is configured, the Wallbox can be added to nymea. nymea will try to discover the wallbox on the first 10 slave ids. This means, the DIP switches on the Wallbox are required to be configred for a modbus Slave ID from 1 to 10.

