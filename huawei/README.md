# Huawei FusionSolar

Connects to a Huawei FusionSolar using Modbus RTU or TCP.

## Huawei FusionSolar

In order to communicate with the Huawei FusionSolar inverter, a working communikation must be provided. This can be done with a Huawei SmartDongle or with a direct modbus RTU connection.

Once nymea has connected successfully to the inverter, following devices will be supported:

* Huawei FusionSolar Inverter (all model supported by the SmartDongle)
* Huawei Meter (connected internally to the Inverter)
* Luna2000 battery units 1 and 2 if connected

### Huawei SmartDongle

The [SmartDongle](https://solar.huawei.com/-/media/Solar/attachment/pdf/apac/datasheet/SmartDongle-WLAN-FE.pdf) can be used to communicate 
with the Huawei Solar Inverter. In order to allow nymea to read from the device please make sure following requirements are met:

* SmartDongle software version must be at least `SP130`.
* Connect the Huawei SmartDongle to the network
* Use the official FusonSolar App to enable Modbus TCP access for the Dongle (full access). You can find more informations [here](https://forum.huawei.com/enterprise/en/modbus-tcp-guide/thread/789585-100027?page=1#comments-area).


You can also contact the [official Huawei support](mailto:eu_inverter_support@huawei.com) in order to get the update files and instructions, or get it from [here](https://support.huawei.com/enterprise/en/digital-power/sdongle-pid-23826585/software).

> The SmartDongle provides only access to the registers specified in the Huawei `openAPI`. Full modbus register access requires a modbus RTU connction.


### Direct modbus RTU connection (RS485).

If you want to communicate directly with the inverter using modbus RTU, you need to wire up the connection correctly and set up the Modbus RTU interface in the nymea settings using the right configuations. Then you can discover the Modbus RTU interface while adding the Huawei Modbus RTU inverter in nymea.

The Modbus RTU offers a full access to all modbus registers available on the inverter.

Please contact your installer how to enable a modbus RTU connection for your setup.

## More
https://solar.huawei.com/eu/

