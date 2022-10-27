# MENNEKES

Connects nymea to a MENNEKES wallboxes. Currently supported models are:

* Amtron Xtra
* Amtron Premium
* Amtron Professional
* Amtron Charge Control
* Amedio Professional

# Requirements

nymea uses the Modbus TCP connection to connect to the wallbox.

> The Modbus TCP connection needs to be enabled manually on the Wallbox.

For the Amtron Charge Control and Premium models, log in to the wallbox's web interface as operator. The login credentials can be obtained
from the user manual of the wallbox. Once logged in, navigate to the Load Management tab and set the Modbus TCP Server to On.

## More

It is highly recommended to update the wallbox to the latest firmware which can be downloaded from [here](https://www.chargeupyourday.de/services/software-updates/).

