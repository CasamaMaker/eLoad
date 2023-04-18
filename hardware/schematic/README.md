# Schematic description modul by modul

## Table of contents
- [Microcontroller](#microcontroller)
    - [Essentials](#essentials)
    - [Antenna design](#antena-design)
- [Sensing](#sensing)

---

## Microcontroller

### Essentials
This is the essential part to work with this uC

![image](/hardware/schematic/uC1.png "Microcontroller")

Crystal configuration, capacitors value always will depend of the choosed Xtal

![image](/hardware/schematic/uC2.png)

HW boot mode configuratinos pins for this uC

![image](/hardware/schematic/uC3.png)

Pinout for normal UART programing

![image](/hardware/schematic/uC4.png)

### Antena design
It needs a special comment due it is delicated performance. There are differents designs types. You can found more information is these documents of [Cypress](https://www.infineon.com/dgdl/Infineon-AN91445_Antenna_Design_and_RF_Layout_Guidelines-ApplicationNotes-v09_00-EN.pdf?fileId=8ac78c8c7cdc391c017d073e054f6227&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-files&redirId=File_1_3_1152) and [NXP](https://www.nxp.com/docs/en/nxp/application-notes/AN11994.pdf). 

![image](/hardware/schematic/Antena.png)

We choose Chip Antenna.

![image](/hardware/schematic/uC5.png)

It is important to respect pcb design, like this

![image](/hardware/schematic/Antena2.png)

## Sensing
The temperature sensing of the mosfet, to prevent it archive the maximum temperature and be damaged, also with that information we can manage the fan. For now it is just a voltage divider directo to uC ADC input, but ___It need to be improve___ with opam configuration.

![image](/hardware/schematic/Sense1.png)

