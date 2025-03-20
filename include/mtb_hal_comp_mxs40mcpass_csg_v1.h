/***************************************************************************//**
* \file mtb_hal_comp_mxs40mcpass_csg_v1.h
*
* \brief
* Provides common API declarations of the mxs40mcpass_csg_v1 driver
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

#include "mtb_hal_comp.h"
// Needed for VDDA information
#include "cycfg_system.h"

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
*       Defines
*******************************************************************************/
#if defined(CY_CFG_PWR_VDDA_MV)
#define _MTB_HAL_COMP_CSG_VDDA_VREF CY_CFG_PWR_VDDA_MV
#else
#define _MTB_HAL_COMP_CSG_VDDA_VREF 3300UL // Default VDDA vref
#endif

/*******************************************************************************
*       Functions
*******************************************************************************/

//--------------------------------------------------------------------------------------------------
// _mtb_hal_comp_csg_setup
//--------------------------------------------------------------------------------------------------
__STATIC_INLINE cy_rslt_t _mtb_hal_comp_csg_setup(mtb_hal_comp_t* obj,
                                                  const mtb_hal_comp_configurator_t* config)
{
    CY_ASSERT(obj != NULL);
    CY_ASSERT(config != NULL);
    obj->slice_csg = config->channel_num;
    return CY_RSLT_SUCCESS;
}


//--------------------------------------------------------------------------------------------------
// _mtb_hal_comp_csg_read
//--------------------------------------------------------------------------------------------------
__STATIC_INLINE bool _mtb_hal_comp_csg_read(mtb_hal_comp_t* obj)
{
    CY_ASSERT(obj != NULL);
    return (Cy_HPPASS_Comp_GetStatus(obj->slice_csg));
}


//--------------------------------------------------------------------------------------------------
// _mtb_hal_comp_csg_set_ref
//--------------------------------------------------------------------------------------------------
__STATIC_INLINE cy_rslt_t _mtb_hal_comp_csg_set_ref(mtb_hal_comp_t* obj, uint16_t ref_mv)
{
    CY_ASSERT(obj != NULL);
    cy_rslt_t result = CY_RSLT_SUCCESS;
    #if defined(MTB_HAL_DISABLE_ERR_CHECK)
    CY_ASSERT_AND_RETURN(ref_mv < _MTB_HAL_COMP_CSG_VDDA_VREF, MTB_HAL_COMP_RSLT_ERR_BAD_ARGUMENT);
    #else
    if (ref_mv > _MTB_HAL_COMP_CSG_VDDA_VREF)
    {
        result = MTB_HAL_COMP_RSLT_ERR_BAD_ARGUMENT;
    }
    else
    #endif // defined(MTB_HAL_DISABLE_ERR_CHECK)
    {
        uint32_t dac_val_a = ref_mv * CY_HPPASS_CSG_DAC_VAL_MAX / _MTB_HAL_COMP_CSG_VDDA_VREF;
        // Note: DAC mode does not matter as we only update dac_val_a
        Cy_HPPASS_DAC_SetValue(obj->slice_csg, dac_val_a);
    }
    return result;
}


#if defined(__cplusplus)
}
#endif
