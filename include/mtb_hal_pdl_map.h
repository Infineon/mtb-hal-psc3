/*******************************************************************************
* File Name: mtb_hal_pdl_map.h
*
* Description:
* This file contains maps from various HAL enums to PDL enums.  Using direct value
* mapping avoids converting values at run-time
*
********************************************************************************
* \copyright
* Copyright 2018-2024 Cypress Semiconductor Corporation (an Infineon company) or
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

#include "mtb_hal_hw_resources.h"

#if defined(__cplusplus)
extern "C"
{
#endif

#if defined(MTB_HAL_DRIVER_AVAILABLE_GPIO)
/** GPIO HAL to PDL enum map for pin drive modes. */
#define MTB_HAL_MAP_GPIO_DRIVE_ANALOG                         (CY_GPIO_DM_ANALOG)
#define MTB_HAL_MAP_GPIO_DRIVE_NONE                           (CY_GPIO_DM_HIGHZ)
#define MTB_HAL_MAP_GPIO_DRIVE_PULLUP                         (CY_GPIO_DM_PULLUP)
#define MTB_HAL_MAP_GPIO_DRIVE_PULLDOWN                       (CY_GPIO_DM_PULLDOWN)
#define MTB_HAL_MAP_GPIO_DRIVE_OPENDRAINDRIVESLOW             (CY_GPIO_DM_OD_DRIVESLOW)
#define MTB_HAL_MAP_GPIO_DRIVE_OPENDRAINDRIVESHIGH            (CY_GPIO_DM_OD_DRIVESHIGH)
#define MTB_HAL_MAP_GPIO_DRIVE_STRONG                         (CY_GPIO_DM_STRONG)
#define MTB_HAL_MAP_GPIO_DRIVE_PULLUPDOWN                     (CY_GPIO_DM_PULLUP_DOWN)
#define MTB_HAL_MAP_GPIO_DRIVE_PULL_NONE                      (CY_GPIO_DM_PULLUP_DOWN << 1)
#endif /* defined(MTB_HAL_DRIVER_AVAILABLE_GPIO) */

#if defined(MTB_HAL_DRIVER_AVAILABLE_UART)
/** UART HAL to PDL enum map to enable/disable/report interrupt cause flags. */
#define MTB_HAL_MAP_UART_IRQ_TX_TRANSMIT_IN_FIFO              (CY_SCB_UART_TRANSMIT_IN_FIFO_EVENT)
#define MTB_HAL_MAP_UART_IRQ_TX_DONE                          (CY_SCB_UART_TRANSMIT_DONE_EVENT)
#define MTB_HAL_MAP_UART_IRQ_RX_DONE                          (CY_SCB_UART_RECEIVE_DONE_EVENT)
#define MTB_HAL_MAP_UART_IRQ_RX_FULL                          (CY_SCB_UART_RB_FULL_EVENT)
#define MTB_HAL_MAP_UART_IRQ_RX_ERROR                         (CY_SCB_UART_RECEIVE_ERR_EVENT)
#define MTB_HAL_MAP_UART_IRQ_TX_ERROR                         (CY_SCB_UART_TRANSMIT_ERR_EVENT)
#define MTB_HAL_MAP_UART_IRQ_RX_NOT_EMPTY                     (CY_SCB_UART_RECEIVE_NOT_EMTPY)
#define MTB_HAL_MAP_UART_IRQ_TX_EMPTY                         (CY_SCB_UART_TRANSMIT_EMTPY)
#define MTB_HAL_MAP_UART_IRQ_TX_FIFO                          (CY_SCB_UART_TRANSMIT_EMTPY << 1)
#define MTB_HAL_MAP_UART_IRQ_RX_FIFO                          (CY_SCB_UART_TRANSMIT_EMTPY << 2)
#endif /* defined(MTB_HAL_DRIVER_AVAILABLE_UART) */

#if defined(MTB_HAL_DRIVER_AVAILABLE_I2C)
/** I2C HAL to PDL enum map to enable/disable/report interrupt cause flags. */
#define MTB_HAL_MAP_I2C_TARGET_READ_EVENT                     (CY_SCB_I2C_SLAVE_READ_EVENT)
#define MTB_HAL_MAP_I2C_TARGET_WRITE_EVENT                    (CY_SCB_I2C_SLAVE_WRITE_EVENT)
#define MTB_HAL_MAP_I2C_TARGET_RD_IN_FIFO_EVENT               (CY_SCB_I2C_SLAVE_RD_IN_FIFO_EVENT)
#define MTB_HAL_MAP_I2C_TARGET_RD_BUF_EMPTY_EVENT             (CY_SCB_I2C_SLAVE_RD_BUF_EMPTY_EVENT)
#define MTB_HAL_MAP_I2C_TARGET_RD_CMPLT_EVENT                 (CY_SCB_I2C_SLAVE_RD_CMPLT_EVENT)
#define MTB_HAL_MAP_I2C_TARGET_WR_CMPLT_EVENT                 (CY_SCB_I2C_SLAVE_WR_CMPLT_EVENT)
#define MTB_HAL_MAP_I2C_TARGET_ERR_EVENT                      (CY_SCB_I2C_SLAVE_ERR_EVENT)
#define MTB_HAL_MAP_I2C_CONTROLLER_ERR_EVENT                  (CY_SCB_I2C_MASTER_ERR_EVENT)

/** I2C HAL to PDL enum map to enable/disable/report address interrupt cause flags. */
#define MTB_HAL_MAP_I2C_GENERAL_CALL_EVENT                    (CY_SCB_I2C_GENERAL_CALL_EVENT)
#define MTB_HAL_MAP_I2C_ADDR_IN_FIFO_EVENT                    (CY_SCB_I2C_ADDR_IN_FIFO_EVENT)
#endif /* defined(MTB_HAL_DRIVER_AVAILABLE_I2C) */

#if defined(MTB_HAL_DRIVER_AVAILABLE_SPI)
/** SPI HAL to PDL enum map to enable/disable/report interrupt cause flags. */
#define MTB_HAL_MAP_SPI_IRQ_DATA_IN_FIFO                      (CY_SCB_SPI_TRANSFER_IN_FIFO_EVENT)
#define MTB_HAL_MAP_SPI_IRQ_DONE                              (CY_SCB_SPI_TRANSFER_CMPLT_EVENT)
#define MTB_HAL_MAP_SPI_IRQ_ERROR                             (CY_SCB_SPI_TRANSFER_ERR_EVENT)
#endif /* defined(MTB_HAL_DRIVER_AVAILABLE_SPI) */

#if defined(MTB_HAL_DRIVER_AVAILABLE_TIMER)
/** Timer HAL to PDL enum map for defining timer events */
#define MTB_HAL_MAP_TIMER_EVENT_NONE                          (CY_TCPWM_INT_NONE)
#define MTB_HAL_MAP_TIMER_EVENT_TERMINAL_COUNT                (CY_TCPWM_INT_ON_TC)
#define MTB_HAL_MAP_TIMER_EVENT_COMPARE_CC0                   (CY_TCPWM_INT_ON_CC0)
#define MTB_HAL_MAP_TIMER_EVENT_COMPARE_CC0_OR_TERMINAL_COUNT (CY_TCPWM_INT_ON_CC0_OR_TC)
#define MTB_HAL_MAP_TIMER_EVENT_COMPARE_CC1                   (CY_TCPWM_INT_ON_CC1)
#define MTB_HAL_MAP_TIMER_EVENT_ALL                           (CY_TCPWM_INT_ON_TC | \
    CY_TCPWM_INT_ON_CC0 | CY_TCPWM_INT_ON_CC0_OR_TC | CY_TCPWM_INT_ON_CC1)
#endif /* defined(MTB_HAL_DRIVER_AVAILABLE_TIMER) */

#if defined(__cplusplus)
}
#endif
