# Stiebel Eltron

Connect nymea to Stiebel Eltron heat pumps. 

This plugin communicates via Modbus/TCP with a Stiebel Eltron Internet-Service-Gateway (ISG). The ISG is usually connected to a WPM/WPM3/WPM3i heatpump controller which again is connected via CAN to the heatpump.
Make sure the ISG firmware is up to date to ensure the Modbus/TCP connection is working. Contact Stiebel Eltron service for a remote update of your ISG.


## Supported Things

* Stiebel Eltron Heatpump

## Requirements

* The package `nymea-plugin-stiebeleltron` must be installed
* Both devices must be in the same local area network.
* Modbus enabled (may conflict with an installed SMA EMI).

## More

https://www.stiebel-eltron.de/
