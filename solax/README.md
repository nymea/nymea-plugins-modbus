# Solax

Connects Solax inverters to nymea.

Currently supported models:

* X1 models
* X3 models
* G4 inverters
* Meters
* Batteries

## Connecting a second inverter

There is the possibility to connect an other solar inverter to the meter 2 interface of your Solax installation. In order to have both possibilities you can switch between a second meter or a second inverter in the settings of the main inverter within nymea.

# Requirements

nymea uses the modbus TCP connection in order to connect to the Solax inverter. Therefore the inverter must be reachable using the local network. The inverter allows only to have **one TCP connection**, please make sure there is no other service or device using modbus TCP with your inverter besides nymea.