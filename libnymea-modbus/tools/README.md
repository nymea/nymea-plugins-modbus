# Generate a modbus read class

In order to make the plugin development for modbus TCP devices much easier and faster, a small tool has been developed to generate a modbus TCP master based class providing get and set methods for the registers and property changed signals. The entire connection handling and modbus parsing will be covered by the resulting connection class ready to use.

The basic workflow looks like this:

* Write the `my-registers.json` file containing all register information you are interested to according to this documentation.
* Include the register files into your plugin project file using: `MODBUS_CONNECTIONS += my-registers.json`
* Include the modbus library project include file *after* the connections definition: `include(../modbus.pri)`
* Run qmake on your project. The generated connection classes can be found in the build directory and will be included automatically into your project.

The easiest way to see how to use the generated connection classes is to look at existing implemntations.

# JSON format

The basic structure of the modbus register JSON looks like following example:

```
{
    "className": "MyConnection",
    "protocol": "BOTH",
    "endianness": "BigEndian",
    "errorLimitUntilNotReachable": 10,
    "checkReachableRegister": "registerPropertyName",
    "enums": [
        {
            "name": "NameOfEnum",
            "values": [
                {
                    "key": "EnumValue1",
                    "value": 0
                },
                {
                    "key": "EnumValue2",
                    "value": 1
                },
                ....
            ]
        }
    ],
    "registers": [
        {
            "id": "registerPropertyName",
            "address": 4,
            "size": 1,
            "type": "uint16",
            "readSchedule": "init",
            "description": "Description of the register",
            "unit": "V",
            "defaultValue": "0",
            "access": "RO"
        },
        {
            "id": "registerWithEnumValues",
            "address": 5,
            "size": 1,
            "type": "uint16",
            "readSchedule": "update",
            "enum": "NameOfEnum",
            "defaultValue": "NameOfEnumEnumValue1",
            "description": "Description of the enum register like states",
            "access": "RO"
        },
        ...
    ],
    "blocks": [
        {
            "id": "blockName",
            "readSchedule": "update",
            "registers": [
                {
                    "id": "registerOne",
                    "address": 0,
                    "size": 2,
                    ...
                },
                {
                    "id": "registerOne",
                    "address": 0,
                    "size": 2,
                    ...
                },
                ...
            ]
        }
    ]
}   

```

## Class name

If no name class name has been passed to the generator script, the classname defined in the JSON file will be used. 

The naming convention for the classname and the resulting source code files looks like this:
    
The class will be defined as
    
    * `<ClassName><Protocol>Connection`.

The source code files will be calld:

    * `classnameprotocolconnection.h`
    * `classnameprotocolconnection.cpp`


## Protocol

Depending on the communication protocol, a different base class will be used for the resulting output class.

There are 2 protocol types:

* `RTU`: a communication based on the RS485 serial RTU transport protocol
* `TCP`: a communication based on the TCP transport protocol

If the modbus device supports both protocols and you want to generate a class for each protocol you can set the protocol to `BOTH` and a class for `RTU` and one for `TCP` will be generated.  

    ...
    "protocol": "TCP",
    ...

# RTU reachable

For modbus RTU it can be the case that the hardware resource is connected, but the target device is not connected on the serial line or does not respond. For those situations a mechanis has been introduce which will mark a device as not reachable if a certain amount of error occurred in a row without any successfull communication. Both, the RTU hardware resource `connected` state and the communication working state get represented by the `reachable` property of the RTU connection class.

Depending on your device the amount of errors in a row can vary and can be specified using the `errorLimitUntilNotReachable` property. If not specified, a default of `10` will be assumend.

In order to check if the device is reachable, a register can be defined by the developer as a test register.
For this purpose the `checkReachableRegister` property has been introduced. The property describes the `id` of the register which will be used for testing the communication. The register should be mandatory on the device
and only one register in size to speed up things. During the check the response data will be ignored, only the communication will be tested. The register must be *readable* and be defined in the `registers` section of your JSON file or in a block.

## Endianness

When converting multiple registers to one data type (i.e. 2 registers uint16 values to one uint32), the order of the registers are important to align with the endianness of the data receiving. 

There are 2 possibilities:

* `BigEndian`: default if not specified: register bytes come in following order `[0, 1, 2, 3]`: `ABCD`
* `LittleEndian`: register bytes come in following order `[0, 1, 2, 3]`: `CDAB`

## Enums

Many modbus devices provide inforation using `Enums`, indicating a special state trough a defined list of values. If a register implements an enum, you can define it in the `enums` section. The `name` property defines the name of the enum, and the script will generate a c++ enum definition from this section. Each enum value will then be generated using `<EnumName><EnumValueName> = <value>`.

If a register represets an enum, you simply add the property `"enum": "NameOfEnum"` in the register map and the property will be defined using the resulting enum type. All convertion between enum and resulting modbus register value will be done automatically.


## Read schedules

### init

In most plugins you have the situation where you need to read some registers only once like serialnumbers or product identifiers right after beeing connected or even before you set up the thing in the plugin. 

For this purpose the `initialize()` method has been provided. If you call `initialize()` the connection will start reading all registers and blocks with `"readSchedule": "init"` defined and emits the signal `initializationFinished(bool success)` once all registers and blocks have been read successfully or on the first occured error. If the `success` parameter is `false`, something went wrong during intialization. Since any error will make the initialization process fail, it is important that the `init` registers are *mandatory* on the device.

This method can also be used to identify the device if implemented properly, or to check if the device has the expected registers available with the given datatype.

### update

In order to make the poll process as easy as possible, you can define the `readSchedule` as `update` for all registers and blocks you requier a preiodical update. If you call the `update()` method the connection will start reading all registers and blocks with `"readSchedule": "update"` and the properties will be updated internally. If a property value has changed, the `<propertyName>Changed()` signal will be emitted. If the property has been read (independet if changed or not) the `<propertyName>ReadFinished()` signal will be emitted.


## Registers

Earch register will be defined as a property in the resulting class modbus TCP class providing easy access to the register data.

* `id`: Mandatory. The id defines the name of the property used in the resulting class.
* `address`: Mandatory. The modbus address of the register.
* `size`: Mandatory. The amount of registers to read for the property.
* `type`: Mandatory. The data type of this property. Available data types are:
    * `uint16` : will be converted to `quint16`
    * `int16`  : will be converted to `qint16`
    * `uint32` : will be converted to `quint32`
    * `int32`  : will be converted to `qint32`
    * `uint64` : will be converted to `quint64`
    * `int64`  : will be converted to `qint64`
    * `float`: will be converted to `float`
    * `float64`: will be converted to `double`
    * `string` : will be converted to `QString`
* `readSchedule`: Optional. Defines when the register needs to be fetched. If no read schedule has been defined, the class will provide only the default access methods, but will not read the value during `initialize()` or `update()` calls. See [#read-schedules](Read schedules) for more information. Possible values are:
    * `init`: The register will be fetched during initialization.
    * `update`: The register will be feched each time the `update()` method will be called.
* `enum`: Optional: If the given data type represents an enum value, this propery can be set to the name of the used enum from the `enum` definition. The class will take care internally about the data convertion from and to the enum values.
* `description`: Mandatory. A clear description of the register.
* `unit`: Optional. Represents the unit of this register value.
* `registerType`: Optional. Represents the type of the register and how to read/write it. Default is `holdingRegister`. Possible values are:
    * `holdingRegister`
    * `inputRegister`
    * `coils`
    * `discreteInputs`
* `access`: Mandatory. Describes the access to this register. Possible valies are:
    * `RO`: Read only access. Only the get method and the changed singal will be defined.
    * `RW`: Read and write access. Also a set mehtod will be defined.
    * `WO`: Write only. Only the set method will be defined.
* `scaleFactor`: Optional. The name of the scale factor register to convert this value to float. `floatValue = intValue * 10^scaleFactor value`. The scale factor value is normally a `int16` value, i.e. -10 or 10
* `staticScaleFactor`: Optional. Use this static scale factor to convert this register value to float. `floatValue = registerValue * 10^staticScaleFactor`. The scale factor value is normally a `int16` value, i.e. -10 or 10
* `defaultValue`: Optional. The value for initializing the property.

# Register blocks

On many device it is possible to read multiple registers in one modbus call. This can improve speed significantly when reading many register addresses which are in a row. 

> Important: all registers within the block must exist, be in a row with no gaps inbetween and from the same function type!

A block sequence looks like this and will define a read method for reading the entwire block. Writing multiple blocks is currently not supported since not needed so far, but could be added too. In any case, all registers must be read or written, never have combinations.

* `id`: Mandatory. The id defines the name of the block used in the resulting class.
* `readSchedule`: Optional. Defines when the register needs to be fetched. If no read schedule has been defined, the class will provide only the update methods, but will not read the value during `initialize()` or `update()` calls. Possible values are:
    * `init`: The register will be fetched during initialization. Once all `init `registers have been fetched, the `initializationFinished()` signal will be emitted.
    * `update`: The register will be feched each time the `update()` method will be called.
* `registers`: Mandatory. The list of registers within the block. Please see the [Registers](#register) definition for more details about registers. The must be from the same register type, the same access type and there are no gaps allowed.

Example block:

    "blocks": [
        {
            "id": "meaningFullName",
            "readSchedule": "update",
            "registers": [
                {
                    "id": "registerOne",
                    "address": 0,
                    "size": 2,
                    ...
                },
                {
                    "id": "registerTwo",
                    "address": 2,
                    "size": 1,
                    ...
                },
                {
                    "id": "registerThree",
                    "address": 3,
                    "size": 2,
                    ...
                },
            ]
        }
    ]


# Autogenerate modbus classes

In order to get always the latest generated code from this tool, the entire process can be automated.

Assuming you have defined the registers in the `my-registers.json` within your plugin directory, and following lines to your plugin project file and run `qmake`.

    # Generate modbus connection
    MODBUS_CONNECTIONS += my-registers.json
    include(../modbus.pri)

If you want to get information about the autogenerating class process, you can add `MODBUS_TOOLS_CONFIG += VERBOSE` in order to get much more information of the process. 

Once you run qmake, in the build directory the autogenerated classes can be found. Also in your project you can find the generated classes for inspection.
