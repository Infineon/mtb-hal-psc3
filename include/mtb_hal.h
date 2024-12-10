/*******************************************************************************
* File Name: mtb_hal.h
*
* Description:
* Top-level HAL header file that can be referenced to pull in all relevant
* drivers for the current device architecture. Not all of these may be
* supported in the current target device. The implementation must provide a
* mtb_hal_hw_types.h and a mtb_hal_drivers.h file in the include path for this
* to depend on.
* The mtb_hal_hw_types.h file must provide the following:
*   1) definitions for each of the resource types consumed by the HAL driver
*      functions.
*   2) OPTIONAL: Implementation specific header files can be used by creating
*      a #define in mtb_hal_hw_types.h with a name of MTB_HAL_<DRIVER>_IMPL_HEADER
*      and the value being the name of the header file.  eg:
*      #define MTB_HAL_GPIO_IMPL_HEADER "mtb_hal_gpio_impl.h"
* The mtb_hal_drivers.h file must simply be a list of include directives to pull
* in the relevant driver header files.
*
********************************************************************************
* \copyright
* Copyright 2018-2022 Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/**
 * \addtogroup group_hal_types HAL General Types/Macros
 * This section documents the basic types and macros that are used by multiple HAL drivers.
 */

/**
 * \addtogroup group_result Result Type
 * \ingroup group_hal_types
 */

/**
 * \addtogroup group_hal_availability HAL Driver Availability
 * This section documents the macros that can be used to check if a specific driver is available
 * for the current device. This includes macros for all HAL drivers that exist for any device, even
 * those that are not applicable (included) with this library. This is intended to help write code
 * that can port between device categories that might not have the same drivers available.
 */

/**
 * \addtogroup group_hal HAL Drivers
 * This section documents the drivers which form the stable API of the ModusToolbox™ HAL.
 * In order to remain portable across platforms and HAL versions, applications should
 * rely only on functionality documented in this section.
 */

/**
 * \addtogroup group_hal_impl
 */

#pragma once

#include "mtb_hal_general_types.h"
#include "mtb_hal_hw_types.h"
#include "mtb_hal_drivers.h"

/** Macro specifying the major version number of the HAL API. Since there are multiple HAL library
 * implementations, this is not necessarily the same as the major version number of the library. It
 * is instead intended as a single version number that can be used across implementation libraries
 * to know what signatures to use in the case an API is updated in the future. Once an API is
 * available, its API will remain consistant across libraries sharing the same version here.
 * \note Support for new drivers/functionality may be added without changing the version number, to
 * check for whether a specific driver is available, the MTB_HAL_DRIVER_AVAILABLE_X macros defined
 * in
 * \ref group_hal_availability should be used.
 * \def MTB_HAL_API_VERSION
 */

/**
 * \ingroup group_hal_availability
 * \{
 */

/** Macro specifying whether the ADC driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_ADC
 */
/** Macro specifying whether the Clock driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_CLOCK
 */
/** Macro specifying whether the COMP driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_COMP
 */
/** Macro specifying whether the CORDIC driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_CORDIC
 */
/** Macro specifying whether the DMA driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_DMA
 */
/** Macro specifying whether the NVM driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_NVM
 */
/** Macro specifying whether the GPIO driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_GPIO
 */
/** Macro specifying whether the GPIO Port driver is available for the current device
 *  \def MTB_HAL_DRIVER_AVAILABLE_GPIO_PORT
 */
/** Macro specifying whether the I2C driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_I2C
 */
/** Macro specifying whether the IPC driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_IPC
 */
/** Macro specifying whether the LPTimer driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_LPTIMER
 */
/** Macro specifying whether the MEMORYSPI driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_MEMORYSPI
 */
/** Macro specifying whether the PWM driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_PWM
 */
/** Macro specifying whether the RTC driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_RTC
 */
/** Macro specifying whether the SDHC driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_SDHC
 */
/** Macro specifying whether the SDIO driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_SDIO
 */
/** Macro specifying whether the SDIO host driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_SDIO_HOST
 */
/** Macro specifying whether the SDIO dev driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_SDIO_DEV
 */
/** Macro specifying whether the SPI driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_SPI
 */
/** Macro specifying whether the SysPM driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_SYSPM
 */
/** Macro specifying whether the System driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_SYSTEM
 */
/** Macro specifying whether the Timer driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_TIMER
 */
/** Macro specifying whether the TRNG driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_TRNG
 */
/** Macro specifying whether the UART driver is available for the current device
 * \def MTB_HAL_DRIVER_AVAILABLE_UART
 */
/** \} group_hal_availability */
