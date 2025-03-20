/***************************************************************************//**
* \file mtb_hal_adc_mxs40mcpass_v1.h
*
* \brief
* Provides common API declarations of the mxs40mcpass_v1 driver
*
********************************************************************************
* \copyright
* Copyright 2024 Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.
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

#include "mtb_hal_adc.h"
#include "mtb_hal_system.h"
#include <stdlib.h>
#include <string.h>
#include "cy_pdl.h"

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/** \addtogroup group_hal_impl_adc ADC (Analog to Digital Converter)
 * \ingroup group_hal_impl
 * \{
 * ADC reads on PSC3 do not automatically clear the read status after
 * reading the result. For performance reasons, this operation is deiberately
 * not performed in \ref mtb_hal_adc_read_latest. However it is applied in
 * \ref mtb_hal_adc_read_u16 as speed is not as critical in this instance.
 *
 * */

/** \} group_hal_impl_adc */

/*******************************************************************************
*                           Defines
*******************************************************************************/

#define _MTB_HAL_SAR_MAX_NUM_CHANNELS           \
    (CY_HPPASS_SAR_DIR_SAMP_NUM + CY_HPPASS_SAR_MUX_SAMP_NUM)


/*******************************************************************************
*                           Typedefs
*******************************************************************************/

/*******************************************************************************
*                        Public Function Prototypes
*******************************************************************************/

/*******************************************************************************
*       Inlined functions
*******************************************************************************/

//--------------------------------------------------------------------------------------------------
// _mtb_hal_adc_setup
//--------------------------------------------------------------------------------------------------
__STATIC_INLINE cy_rslt_t _mtb_hal_adc_setup(mtb_hal_adc_t* obj,
                                             const mtb_hal_adc_configurator_t* config,
                                             mtb_hal_adc_channel_t** channels)
{
    cy_rslt_t status = CY_RSLT_SUCCESS;
    uint32_t enabled_channels = 0;
    uint32_t enabled_groups = 0;
    uint32_t enabled_diff = 0;
    uint32_t enabled_average = 0;
    uint32_t enabled_triggers = 0;
    mtb_hal_adc_channel_t* channel;

    for (uint8_t cnt = 0; cnt < _MTB_HAL_SAR_MAX_NUM_CHANNELS; cnt++)
    {
        if (config->hppass_config->sar->chan[cnt] != NULL)
        {
            enabled_channels |= 1u << cnt;

            if (config->hppass_config->sar->chan[cnt]->diff)
            {
                enabled_diff |= 1u << cnt;
            }

            cy_en_hppass_sar_avg_t avg = config->hppass_config->sar->chan[cnt]->avg;
            if (avg != CY_HPPASS_SAR_AVG_DISABLED)
            {
                enabled_average  |= 1u << cnt;
            }

            // It is possible to have channels array that is smaller than supported channels
            if ((cnt < config->num_channels) && (channels[cnt] != NULL))
            {
                channel = channels[cnt];
                memset(channel, 0, sizeof(mtb_hal_adc_channel_t));
                channel->adc = obj;
                channel->channel_idx = cnt;
                channel->channel_msk = (uint32_t)(1UL << cnt);
                obj->channel_config[cnt] = channel;
            }
            else
            {
                obj->channel_config[cnt] = NULL;
            }
        }
    }

    obj->enabled_channels = enabled_channels;
    obj->enabled_diff = enabled_diff;
    obj->enabled_average = enabled_average;

    for (uint8_t cnt = 0; cnt < CY_HPPASS_SAR_GRP_NUM; cnt++)
    {
        if (config->hppass_config->sar->grp[cnt] != NULL)
        {
            enabled_groups |= 1u << cnt;
            enabled_triggers |= config->hppass_config->sar->grp[cnt]->trig;
            obj->continuous_scanning |= config->hppass_config->sar->grp[cnt]->continuous;
        }
    }

    obj->enabled_groups = enabled_groups;
    obj->enabled_triggers = enabled_triggers;

    return status;
}


//--------------------------------------------------------------------------------------------------
// _mtb_hal_adc_is_conversion_complete
//--------------------------------------------------------------------------------------------------
__STATIC_INLINE bool _mtb_hal_adc_is_conversion_complete(const mtb_hal_adc_channel_t* obj)
{
    uint32_t status = Cy_HPPASS_SAR_Result_GetStatus();
    return ((obj->channel_msk & status) != 0UL);
}


#define mtb_hal_adc_is_conversion_complete(obj)  _mtb_hal_adc_is_conversion_complete(obj)


//--------------------------------------------------------------------------------------------------
// _mtb_hal_adc_read_latest
//--------------------------------------------------------------------------------------------------
__STATIC_INLINE cy_rslt_t _mtb_hal_adc_read_latest(const mtb_hal_adc_channel_t* obj,
                                                   int32_t* result)
{
    *result = Cy_HPPASS_SAR_Result_ChannelRead(obj->channel_idx);
    return CY_RSLT_SUCCESS;
}


#define mtb_hal_adc_read_latest(obj, result) _mtb_hal_adc_read_latest(obj, result)

//--------------------------------------------------------------------------------------------------
// _mtb_hal_adc_start_convert
//--------------------------------------------------------------------------------------------------
__STATIC_INLINE cy_rslt_t _mtb_hal_adc_start_convert(mtb_hal_adc_t* obj)
{
    return Cy_HPPASS_SetFwTrigger(obj->enabled_triggers);
}


#define mtb_hal_adc_start_convert(obj) _mtb_hal_adc_start_convert(obj)

//--------------------------------------------------------------------------------------------------
// _mtb_hal_adc_counts_to_u16
//--------------------------------------------------------------------------------------------------
__STATIC_INLINE uint16_t _mtb_hal_adc_counts_to_u16(const mtb_hal_adc_channel_t* obj,
                                                    int32_t signed_result)
{
    CY_UNUSED_PARAMETER(obj);
    const uint8_t RESULT_SCALING_FACTOR = UINT16_MAX / 0xFFF; // constant 12-bit SAR resolution
    uint16_t unsigned_result = (uint16_t)((uint32_t)(signed_result) & 0xFFFF);

    /* The SAR provides a 12-bit result, but this API is defined to fill a full 16-bit range */
    uint16_t scaled_result = unsigned_result * RESULT_SCALING_FACTOR;
    return scaled_result;
}


//--------------------------------------------------------------------------------------------------
// _mtb_hal_adc_read
//--------------------------------------------------------------------------------------------------
__STATIC_INLINE int32_t _mtb_hal_adc_read(const mtb_hal_adc_channel_t* obj)
{
    int32_t latest;
    uint32_t timeout = 1000UL;

    Cy_HPPASS_SAR_Result_ClearStatus(obj->channel_msk); // Ensure that the status is current
    _mtb_hal_adc_start_convert(obj->adc);
    while ((!_mtb_hal_adc_is_conversion_complete(obj)) && (timeout != 0))
    {
        mtb_hal_system_delay_us(1);
        timeout--;
    }

    _mtb_hal_adc_read_latest(obj, &latest);
    Cy_HPPASS_SAR_Result_ClearStatus(obj->channel_msk); // Cleanup

    // Pseudo-differential mode automatically switches to signed mode and therefore the scale is
    // half of expected
    return (((obj->adc->enabled_diff & obj->channel_msk)) ? (latest << 1u) : latest);
}


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
