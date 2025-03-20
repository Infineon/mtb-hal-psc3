/***************************************************************************//**
* \file mtb_hal_hw_types_adc_mcpass.h
*
*********************************************************************************
* \copyright
* Copyright 2024-2025 Cypress Semiconductor Corporation (an Infineon company) or
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

#include "cy_pdl.h"
#include "mtb_hal_hw_types_clock.h"

#if defined(CY_IP_MXS40MCPASS)
 /**
 * \ingroup group_hal_availability
 * \{
 */

#if !defined(MTB_HAL_DRIVER_AVAILABLE_ADC)
/** Macro specifying whether the ADC driver is available for the current device */
#define MTB_HAL_DRIVER_AVAILABLE_ADC (1u)
#endif // !defined(MTB_HAL_DRIVER_AVAILABLE_ADC)

/** \} group_hal_availability */

#if !defined(_MTB_HAL_DRIVER_AVAILABLE_ADC_SAR)
#define _MTB_HAL_DRIVER_AVAILABLE_ADC_SAR (1u)
#endif // !defined(_MTB_HAL_DRIVER_AVAILABLE_ADC_SAR)


#define CY_SAR_MAX_NUM_CHANNELS (CY_HPPASS_SAR_DIR_SAMP_NUM + CY_HPPASS_SAR_MUX_SAMP_NUM)

struct _mtb_hal_adc_channel_s;

/**
 * @brief ADC object
 *
 * Application code should not rely on the specific contents of this struct.
 * They are considered an implementation detail which is subject to change
 * between platforms and/or HAL releases.
 */
typedef struct
{
    struct _mtb_hal_adc_channel_s*      channel_config[CY_SAR_MAX_NUM_CHANNELS];
    HPPASS_Type*                        base;
    uint32_t                            enabled_channels;
    uint32_t                            enabled_groups;
    uint32_t                            enabled_triggers;
    uint32_t                            enabled_diff;
    uint32_t                            enabled_average;
    const mtb_hal_clock_t*              clock;
    bool                                continuous_scanning;
} mtb_hal_adc_t;

/**
 * @brief ADC configurator struct
 *
 * This struct allows a configurator to provide block configuration information
 * to the HAL. Because configurator-generated configurations are platform
 * specific, the contents of this struct is subject to change between platforms
 * and/or HAL releases.
 */
typedef struct
{
    HPPASS_Type*                        base;
    cy_stc_hppass_cfg_t const*          hppass_config;
    const mtb_hal_clock_t*              clock;
    uint8_t                             num_channels;
} mtb_hal_adc_configurator_t;

/**
 * @brief ADC channel object
 *
 * Application code should not rely on the specific contents of this struct.
 * They are considered an implementation detail which is subject to change
 * between platforms and/or HAL releases.
 */
typedef struct _mtb_hal_adc_channel_s   /* Struct given an explicit name to make the forward
                                           declaration above work */
{
    mtb_hal_adc_t*                      adc;
    uint8_t                             channel_idx;
    uint8_t                             channel_msk;
    bool                                diff;
} mtb_hal_adc_channel_t;

#endif // defined(CY_IP_MXS40MCPASS)
