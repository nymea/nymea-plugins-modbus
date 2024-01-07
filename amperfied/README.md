# Amperfied - Heidelberg

Connects nymea to a Amperfied/Heidelberg wallboxes. Currently supported models are:

* Amperfied Energy Control
* Amperfied connect.home
* Amperfied connect.solar
* Amperfied connect.business

# Requirements

nymea requires the use of a firmware greator or equal version 1.0.7 on the wallbox.

## Amperfied Energy Control
The Amperfied Energy Control is a Modbus RTU device. This means it must be connected to the nymea system using an RS485 port.
The Modbus RTU must be configured with a baud rate of 19200, even parity, 8 data bits and 1 stop bit.
In order to allow nymea to automatically discover the wallbox on the bus, the Modbus slave ID must be in the range of 1 - 20.
If a higher slave ID number is required, the manual setup is to be used.

## Amperfied connect.home/solar/business

The Amperfied connect.home is a Modbus TCP device. This means it must be connected to the same network the nymea system is in.

