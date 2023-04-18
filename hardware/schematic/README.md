# Schematic description modul by modul

## Table of contents
- [Microcontroller](#microcontroller)
    - [Essentials](#essentials)
    - [Antenna design](#antena-design)
- [Drive](#drive)
- [Sensing](#sensing)

---

## Microcontroller

### Essentials
This is the essential part to work with this uC

![image](/hardware/schematic/pictures/uC1.png "Microcontroller")

Crystal configuration, capacitors value always will depend of the choosed Xtal

![image](/hardware/schematic/pictures/uC2.png "Xtal")

HW boot mode configuratinos pins for this uC

![image](/hardware/schematic/pictures/uC3.png "Essential HW configuration")

Pinout for normal UART programing

![image](/hardware/schematic/pictures/uC4.png "UART flashing")

### Antena design
It needs a special comment due it is delicated performance. There are differents designs types. You can found more information is these documents of [Cypress](https://www.infineon.com/dgdl/Infineon-AN91445_Antenna_Design_and_RF_Layout_Guidelines-ApplicationNotes-v09_00-EN.pdf?fileId=8ac78c8c7cdc391c017d073e054f6227&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-files&redirId=File_1_3_1152) and [NXP](https://www.nxp.com/docs/en/nxp/application-notes/AN11994.pdf). 

![image](/hardware/schematic/pictures/Antena.png "Antennas types")

We choose Chip Antenna.

![image](/hardware/schematic/pictures/uC5.png)

It is important to respect pcb design, like this

![image](/hardware/schematic/pictures/Antena2.png "Reference PCB design")

## Drive
__Mosfet driver__

![image](/hardware/schematic/pictures/Drive1.png)

## Sensing
The __temperature sensing__ of the mosfet, to prevent it archive the maximum temperature and be damaged, also with that information we can manage the fan. For now it is just a voltage divider directo to uC ADC input, but _It need to be improve_ with opam configuration.

![image](/hardware/schematic/pictures/Sense1.png)

The are 3 parts in the __Load voltage sensing__ a simple voltage divider, an overvoltage protection and optional external ADC chip. As it is works fine but with a little distorcion when load is working. _Will be improved with opam circuit_

![image](/hardware/schematic/pictures/Sense2.png)

This is an optional extanal ADC chip for Current sense, but for the moment we do not use.

![image](/hardware/schematic/pictures/Sense3.png)

