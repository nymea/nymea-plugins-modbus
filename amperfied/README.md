# Amperfied - Heidelberg

Connects nymea to a Amperfied/Heidelberg wallboxes. Currently supported models are:

* Amperfeid Energy Control
* Amperfeid connect.home

# Requirements

nymea requires the use of a firmware greator or equal version 1.0.7 on the wallbox.

## Amperfeid Energy Control
The Amperfeid Energy Control is a Modbus RTU device. This means it must be connected to the nymea system using an RS485 port.
In order to allow nymea to automatically discover the wallbox on the bus, the Modbus slave ID must be in the range of 1 - 20.
If a higher slave ID number is required, the manual setup is to be used.

## Amperfied connect.home
The Amperfeid connect.home is a Modbus TCP device. This means it must be connected to the same network the nymea system is in.

