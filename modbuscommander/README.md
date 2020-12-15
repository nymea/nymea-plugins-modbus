# Modbus Commander

A nymea integration plugin to send Modbus RTU or TCP requests.

## Usage

Set up one Modbus RTU or TCP client first. Afterwards a register **thing** can be set-up
and during the setup process the modbus client can be selected.

## Supported Things

* Modbus TCP client
   * Modbus TCP interface
   * IP Address and Port of the target device are requried.
* Modbus RTU client
   * Modbus RTU interface
   * All available serial ports will be discovered.
   * Baudrate, data bits, stop bits and parity are required.
* Coil
   * Writes and reads a single Modbus Coil
   * Modbus device- and register address are required.
* Discrete input
   * Reads a single Modbus discrete input
   * Modbus device- and register address are required.
* Input register
   * Reads a single Modbus input register
   * Modbus device- and register address are required.
* Holding register
   * Writes and reads a single Modbus holding register
   * Modbus device- and register address are required.

## Requirements

* The package 'nymea-plugin-modbuscommander' must be installed.
* For Modbus RTU, a serial port must be available.
* For Modbus TCP, the target deviecc must be in the same network.

## Settings 

### Plug-In Configuration 

*Update interval* 

Defines the interval for register polling in seconds.
Minimum and default value is 1 second.

### Modbus TCP & RTU Client Settings 

*Timeout*

Default value is 100 ms, the minimum timeout is 10 ms.
The timeout is used by the Modbus client to determine how long it waits 
for a response from the server. The request failes if the response 
is not received within the given timeout.

*Number of retries*

Sets the number of retries a client will perform before a request fails. 
The default value is set to 3.

## More

http://www.modbus.org/
