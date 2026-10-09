# PhoenixConnect

nymea plugin for EV-chargers by PhoenixConnect. Those wallboxes are sold rebranded as
Wallbe, Compleo and Scapo.

## Supported Things

* Wallbe ECO 2.0
* Wallbe PRO
* Compleo ECOs
* Compleo PRO
* Compleo eBOX professional
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

## Compleo eBOX professional

The eBOX professional uses a different Modbus TCP register map than the Phoenix Contact
controllers in the other supported wallboxes. Firmware 1.3.0 or newer is required. Connect
the wallbox to the local network using LAN2 and enable Modbus in WebConfig under
`LDP1 -> Load management`. Automatic discovery probes both the standard Modbus port 502
and the Compleo-documented port 5555 using Modbus unit ID 1. Manual setup defaults to port
502, but the port can be changed in nymea for installations using port 5555.

The published Modbus interface provides local charging availability, a 6 A to 32 A current
limit, Mode 3 status and phase configuration. It does not provide voltage, active power,
charging duration or accumulated energy values.

Charging is paused by writing 0 A to all three operating-current registers. Register 1028
controls the wallbox's service availability and is not used as the normal charging switch.
The desired 6 A to 32 A current is buffered independently from the power state, and all three
phase values are written atomically. Changing the current while paused therefore does not
resume charging, and enabling charging restores the most recently requested current.

The phase count is derived from a valid phase mapping reported by the wallbox. If the
wallbox reports an invalid or empty mapping, the configurable phase count is used instead
and defaults to three phases.
