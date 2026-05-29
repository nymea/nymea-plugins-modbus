# Growatt WIT

Connects Growatt WIT hybrid inverters to nymea using Modbus RTU over RS485.

The plugin creates one inverter thing and, if available, auto-detects the connected energy meter and battery values exposed by the inverter.

## RS485 connection

To connect the inverter to nymea:

1. Use a Modbus RTU capable RS485 interface on the nymea host, for example a USB to RS485 adapter.
2. Wire the RS485 adapter to the inverter RS485/Modbus terminals.
   Connect `A` to `A` and `B` to `B`.
   If your installation guide requires reference ground, connect `GND` as well.
3. Enable Modbus RTU communication on the inverter and note the configured Modbus slave address.
4. In nymea, create a Modbus RTU interface first and configure it to match the serial settings of the inverter.
5. Add the Growatt WIT inverter in nymea and use the inverter slave address during discovery.

If communication does not work, first verify the RS485 polarity, the inverter Modbus settings, and the serial parameters configured in the nymea Modbus RTU interface.
