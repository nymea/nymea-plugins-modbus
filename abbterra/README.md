# ABB Terra AC

This plugin integrates ABB Terra AC chargers via Modbus TCP and Modbus RTU.

Implemented features:

- network discovery for Modbus TCP chargers
- Modbus RTU discovery using nymea's managed RTU hardware resource
- connection state
- plugged-in and charging state detection
- charging enable/disable via current-limit control
- writable maximum charging current
- active power and session energy
- firmware version, serial number, and error code
- communication timeout setting

The register model is based on:

- `ABB_Terra_AC_Charger_ModbusCommunication_v1.7.pdf`
