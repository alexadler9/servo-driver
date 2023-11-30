# Servo driver
Basic driver for communicating with EPS-B1 series servo.

<i>EPS-B1 series servo provides the Modbus communication function with RS485 interface, which can be used to set parameters, to perform monitoring operations and so on.</i>

This driver supports Modbus <b>RTU</b> and <b>ASCII</b> modes for RS-232/485 interfaces:
* Modbus ASCII makes use of ASCII characters for communication. It can be used on lines with high delays and devices with less accurate timers;
* Modbus RTU (Remote Terminal Unit), makes use of a compact, binary representation of the data for communication. Delays in this protocol are critical, but the overhead of data transmission is less than in Modbus ASCII.
