# Hardware Abstraction Layer (HAL) Release Notes
The Hardware Abstraction Layer (HAL) provides an implementation of the Hardware Abstraction Layer for the PSC3 family of chips. This API provides convenience methods for initializing and manipulating different hardware peripherals. Depending on the specific chip being used, not all features may be supported.

On devices which contain multiple cores, this library is supported on all cores. If HAL is used on multiple cores at the same time, the application is responsible for ensuring that each peripheral is only used on one core at a given time.

### What's Included?
This release of the HAL includes support for the following drivers:
* ADC
* Clock
* Comp
* DMA
* GPIO
* I2C
* LPTimer
* NVM
* PWM
* RTC
* SPI
* SysPm
* System
* TRNG
* Timer
* UART

### What Changed?
#### v1.1.2
* DMA (DW): Fixed the compatibility issues encountered when building projects in EWARM using the "C++ only" option.

#### v1.1.1
* UART: Populate Tx/RTS pin from configurator-provided value. Avoids glitch when changing baudrate.
* LPTimer: Populate lfclock frequency from configurator-provided value. Improves DeepSleep entry speed.
* This release requires mtb-pdl-cat1 version 3.18.0 or later

#### v1.1.0
* Added support for ADC, Clock, Comp, DMA, I2C, LPTimer, PWM, RTC, SPI, Syspm, Timer, TRNG drivers.

#### v1.0.0
* Initial release for PSC3 support.


### Supported Software and Tools
This version of the Hardware Abstraction Layer was validated for compatibility with the following Software and Tools:

| Software and Tools                        | Version |
| :---                                      | :----:  |
| ModusToolbox™ Software Environment        | 3.6.0   |
| GCC Compiler                              | 14.2.1  |
| IAR Compiler                              | 9.50.2  |
| ARM Compiler                              | 6.22    |

Minimum required ModusToolbox™ Software Environment: v3.3

### More information
Use the following links for more information, as needed:
* [API Reference Guide](https://infineon.github.io/mtb-hal-psc3/html/modules.html)
* [Cypress Semiconductor, an Infineon Technologies Company](http://www.cypress.com)
* [Infineon GitHub](https://github.com/infineon)
* [ModusToolbox™](https://www.cypress.com/products/modustoolbox-software-environment)

---
© Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation, 2019-2025.
