# PhoenixConnect

nymea plugin for EV-chargers by PhoenixConnect. Those wallboxes are sold rebranded as
Wallbe, Compleo and Scapo.

## Supported Things

* Wallbe ECO 2.0
* Wallbe PRO
* Compleo ECOs
* Compleo PRO
* Scapo Economy
* Scapo Vision

## Requirements

By default, the wallbox is configured to the IP address 192.168.0.8/24 on the builtin
ethernet port and can be configured to use DHCP from the web interface. The password
for the web interface varies between brandings and may be obtained from the product
manual.

Once the wallbox is connected to the network, it can be added to nymea using the regular
thing setup wizard.

Depending on the usage, the DIP switches may be configured to all off, which puts the
wallbox in the plug-and-charge mode, allowing the easiest integration within nymea. If
there are security concerns, DIP switch 7 can be turned on to enable the lock mode. In
this mode, the key lock needs to be turned for the wallbox to actually start charging.
If the model is capable of RFID, DIP switch 10 can be used to enable/diable that feature.
