# Huawei FusionSolar

Connects to a Huawei Inverters and Meters using Modbus TCP/RTU.

## Huawei FusionSolar

The Huawei FusionSolar can be connected using the Huawei SmartDongle.

The following devices are supported:

* Huawei FusionSolar Inverter (all model supported by the SmartDongle)
* Huawei Meter (connected internally to the Inverter)
* Luna2000 battery units 1 and 2 if connected

### Huawei SmartDongle

The [SmartDongle](https://solar.huawei.com/-/media/Solar/attachment/pdf/apac/datasheet/SmartDongle-WLAN-FE.pdf) can be used to communicate 
with the Huawei Solar Inverter. In order to allow nymea to read from the device please make sure following requirements are met:

* SmartDongle software version must be at least `SP133`.
* Connect the Huawei SmartDongle to the network
* Use the official FusonSolar App to enable Modbus TCP access for the Dongle (full access). You can find more informations [here](https://forum.huawei.com/enterprise/en/modbus-tcp-guide/thread/789585-100027?page=1#comments-area).

You can also contact the [official Huawei support](mailto:eu_inverter_support@huawei.com) in order to get the update files and instructions, or get it from [here](https://support.huawei.com/enterprise/en/digital-power/sdongle-pid-23826585/software).

## Huawei Smartlogger

The Huawei Smartlogger can be connected if the Modbus TCP protocol has been enabled on the device. By default, this option is disabled.
In order to connect to the Smartlogger, the Modbus slave ID of the Meter has to be known. The ID is the same as for the Modbus RTU network of the Meter.

Once successfully connected, one meter and one inverter will show in the system summing up all individual inverters of the system.
