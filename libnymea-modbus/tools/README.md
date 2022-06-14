# Generate a modbus read class

In order to make the plugin development for modbus TCP devices much easier and faster, a small tool has been developed to generate a modbus TCP master based class providing get and set methods for the registers and property changed signals.

The workflow looks like this:

* Write the `registers.json` file containing all register information you are interested to.
* Run the script and provide the class name, output directory and the path to the JSON file
* Include the generated class in your plugin, connect the `<propertyName>Changed()` signal and update the thing state within the plugin.


The class will provide 2 main methods for fetching information from the modbus device:

* `initialize()` will read all registers with `"readSchedule": "init"` and emits the signal `initializationFinished()` once all replies returned.
* `update()` can be used to update all registers with `"readSchedule": "update"`. The class will then fetch each register and update the specified value internally. If the value has changed, the `<propertyName>Changed()` signal will be emitted.

The resulting class will inhert from the `ModbusTCPMaster` class, providing easy access to all possible modbus operations and inform about the connected state.


# JSON format

The basic structure of the modbus register JSON looks like following example:

```
{
    "className": "MyConnection",
    "protocol": "BOTH",
    "endianness": "BigEndian",
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


## Endianness

When converting multiple registers to one data type (i.e. 2 registers uint16 values to one uint32), the order of the registers are important to align with the endianness of the data receiving. 

There are 2 possibilities:

* `BigEndian`: default if not specified: register bytes come in following order `[0, 1, 2, 3]`: `ABCD`
* `LittleEndian`: register bytes come in following order `[0, 1, 2, 3]`: `CDAB`

## Enums

Many modbus devices provide inforation using `Enums`, indicating a special state trough a defined list of values. If a register implements an enum, you can define it in the `enums` section. The `name` property defines the name of the enum, and the script will generate a c++ enum definition from this section. Each enum value will then be generated using `<EnumName><EnumValueName> = <value>`.

If a register represets an enum, you simply add the property `"enum": "NameOfEnum"` in the register map and the property will be defined using the resulting enum type. All convertion between enum and resulting modbus register value will be done automatically.

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
* `readSchedule`: Optional. Defines when the register needs to be fetched. If no read schedule has been defined, the class will provide only the update methods, but will not read the value during `initialize()` or `update()` calls. Possible values are:
    * `init`: The register will be fetched during initialization. Once all `init `registers have been fetched, the `initializationFinished()` signal will be emitted.
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
