# MENNEKES

Connects nymea to a MENNEKES wallboxes. Currently supported models are:

* AMTRON Xtra
* AMTRON Premium
* AMTRON Professional
* AMTRON Charge Control
* Amedio Professional
* AMTRON Compact 2.0s

# Requirements

nymea uses the Modbus RTU (AMTRON Compact 2.0s) or Modbus TCP (all other models) connection to connect to the wallbox.

> The Modbus TCP connection needs to be enabled manually on the Wallbox.

For the Amtron Charge Control and Premium models, log in to the wallbox's web interface as operator. The login credentials can be obtained
from the user manual of the wallbox. Once logged in, navigate to the Load Management tab and set the Modbus TCP Server to On.

For the AMTRON Compact 2.0s, DIP switch 4 and 5 on Bank 51 must be set to ON in order to allow nymea controlling the wallbox. With this DIP switch configuration, the solar charging mode will merely control the LED display of the wallbox.

## More

It is highly recommended to update the wallbox to the latest firmware which can be downloaded from [here](https://www.chargeupyourday.de/services/software-updates/).

