# iDM

## Supported Things

## More

https://www.idm-energie.at/en/

** Modbus TCP communication not working **
	* Is "Modbus TCP" selected in the "Building Management System" menu? 
	* Is the Modbus TCP device and the heat pump in the same network?
	* Is there an IP address conflict?
	* Has the heat pump set the IP address manually? IP address should be set manually, because with "DHCP" the IP address can change (e.g. after a power failure).
	* Was the connection made via a switch, possibly blocking this communication? If so, integrate the Modbus TCP device directly (without a switch).

