/*******************************************************************************
* File Name: mtb_hal_drivers.h
*
* Description:
* Implementation specific HAL header file that pulls in all driver files that are
* applicable for this implementation.
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

#pragma once

#if (MTB_HAL_DRIVER_AVAILABLE_ADC)
#include "mtb_hal_adc.h"
#endif // (MTB_HAL_DRIVER_AVAILABLE_ADC)
#if (MTB_HAL_DRIVER_AVAILABLE_COMP)
#include "mtb_hal_comp.h"
#endif //(MTB_HAL_DRIVER_AVAILABLE_COMP)
#if (MTB_HAL_DRIVER_AVAILABLE_DMA)
#include "mtb_hal_dma.h"
#endif //(MTB_HAL_DRIVER_AVAILABLE_DMA)
#include "mtb_hal_gpio.h"
#if (MTB_HAL_DRIVER_AVAILABLE_LPTIMER)
#include "mtb_hal_lptimer.h"
#endif //(MTB_HAL_DRIVER_AVAILABLE_LPTIMER)
#if (MTB_HAL_DRIVER_AVAILABLE_MEMORYSPI) && (CY_IP_MXSMIF_VERSION > 4)
#include "mtb_hal_memoryspi.h"
#endif //(MTB_HAL_DRIVER_AVAILABLE_MEMORYSPI)
#if (MTB_HAL_DRIVER_AVAILABLE_NVM)
#include "mtb_hal_nvm.h"
#endif // (MTB_HAL_DRIVER_AVAILABLE_NVM)
#if (MTB_HAL_DRIVER_AVAILABLE_PWM)
#include "mtb_hal_pwm.h"
#endif //(MTB_HAL_DRIVER_AVAILABLE_PWM)
#if (MTB_HAL_DRIVER_AVAILABLE_RTC)
#include "mtb_hal_rtc.h"
#endif //(MTB_HAL_DRIVER_AVAILABLE_RTC)
#if (MTB_HAL_DRIVER_AVAILABLE_SDHC)
#include "mtb_hal_sdhc.h"
#endif \
    // (MTB_HAL_DRIVER_AVAILABLE_SDHC)
#if (MTB_HAL_DRIVER_AVAILABLE_SDIO)
#include "mtb_hal_sdio.h"
#endif \
    // (MTB_HAL_DRIVER_AVAILABLE_SDIO)
#if (MTB_HAL_DRIVER_AVAILABLE_UART)
#include "mtb_hal_uart.h"
#endif // (MTB_HAL_DRIVER_AVAILABLE_UART)
#if (MTB_HAL_DRIVER_AVAILABLE_I2C)
#include "mtb_hal_i2c.h"
#endif // (MTB_HAL_DRIVER_AVAILABLE_I2C)
#if (MTB_HAL_DRIVER_AVAILABLE_SPI)
#include "mtb_hal_spi.h"
#endif // (MTB_HAL_DRIVER_AVAILABLE_SPI)
#if (MTB_HAL_DRIVER_AVAILABLE_SYSTEM)
#include "mtb_hal_system.h"
#endif // (MTB_HAL_DRIVER_AVAILABLE_SYSTEM)
#if (MTB_HAL_DRIVER_AVAILABLE_TIMER)
#include "mtb_hal_timer.h"
#endif // (MTB_HAL_DRIVER_AVAILABLE_TIMER)
#if (MTB_HAL_DRIVER_AVAILABLE_TRNG)
#include "mtb_hal_trng.h"
#endif // (MTB_HAL_DRIVER_AVAILABLE_TRNG)

#if defined(HAL_NEXT_TODO)
//TODO: include progressively when HAL Next is implemented
#include "mtb_hal_clock.h"
#include "mtb_hal_ipc.h"
#include "mtb_hal_syspm.h"
#endif // defined(HAL_NEXT_TODO)

#if defined(COMPONENT_MTB_PWRLIB_ENABLE)
#include "mtb_hal_pwrlib_impl.h"
#endif // defined(COMPONENT_MTB_PWRLIB_ENABLE)
