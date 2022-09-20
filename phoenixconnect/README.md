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

Depending on the usage, DIP switches may be configured: DIP switch 10 is required to
be set for nymea to be able to operate the wallbox. DIP switch 7 may be enabled in 
addition to enable the key lock. In this mode, the key lock needs to be unlocked before
the wallbox will charge.
