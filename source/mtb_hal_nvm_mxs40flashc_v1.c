/***************************************************************************//**
* File Name: mtb_hal_nvm_mxs40flashc_v1.c
*
* Description:
* Provides a high level interface for interacting with the Infineon embedded
* non-volatile memory (NVM). This is wrapper around the lower level PDL API.
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

#include "mtb_hal_hw_types.h"
#include "mtb_hal_nvm.h"
#include "cy_utils.h"
#include "mtb_hal_utils_impl.h"
#include <string.h>


#if (MTB_HAL_DRIVER_AVAILABLE_NVM)

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#if defined(COMPONENT_PSC3)
#define _MTB_HAL_NVM_MEMORY_BLOCKS_COUNT  (_MTB_HAL_INTERNAL_FLASH_MEMORY_BLOCKS + \
        _MTB_HAL_INTERNAL_RRAM_MEMORY_BLOCKS + \
        _MTB_HAL_INTERNAL_OTP_MEMORY_BLOCKS + \
        1)
#else
#define _MTB_HAL_NVM_MEMORY_BLOCKS_COUNT  (_MTB_HAL_INTERNAL_FLASH_MEMORY_BLOCKS + \
                                         _MTB_HAL_INTERNAL_RRAM_MEMORY_BLOCKS + \
                                         _MTB_HAL_INTERNAL_OTP_MEMORY_BLOCKS)
#endif

static const mtb_hal_nvm_region_info_t _mtb_hal_nvm_mem_regions[_MTB_HAL_NVM_MEMORY_BLOCKS_COUNT] =
{
    #if (_MTB_HAL_DRIVER_AVAILABLE_NVM_FLASH)
    // Main Flash
    {
        .nvm_type = MTB_HAL_NVM_TYPE_FLASH,
        #if defined(CY_FLASH_CBUS_BASE)
        .start_address = CY_FLASH_CBUS_BASE,
        #else
        .start_address = CY_FLASH_BASE,
        #endif
        .size = CY_FLASH_SIZE,
        .sector_size = CY_FLASH_SIZEOF_ROW,
        .block_size = CY_FLASH_SIZEOF_ROW,
        .is_erase_required = true,
        .erase_value = 0x00U,
    },
    #if defined(COMPONENT_PSC3)
    // Alternative Main Flash Bank for Dual Bank (Boy2 Specific)
    {
        .nvm_type = MTB_HAL_NVM_TYPE_FLASH,
        #if defined(CY_FLASH_CBUS_BASE)
        .start_address = CY_FLASH_CBUS_D_BASE,
        #else
        .start_address = CY_FLASH_D_BASE,
        #endif
        .size = CY_FLASH_SIZE,
        .sector_size = CY_FLASH_SIZEOF_ROW,
        .block_size = CY_FLASH_SIZEOF_ROW,
        .is_erase_required = true,
        .erase_value = 0x00U,
    },
    #endif // if defined(COMPONENT_PSC3)
    // Working Flash
    #if (CY_EM_EEPROM_SIZE > 0)
    {
        .nvm_type = MTB_HAL_NVM_TYPE_FLASH,
        .start_address = CY_EM_EEPROM_BASE,
        .size = CY_EM_EEPROM_SIZE,
        .sector_size = CY_FLASH_SIZEOF_ROW,
        .block_size = CY_FLASH_SIZEOF_ROW,
        .is_erase_required = true,
        .erase_value = 0x00U,
    },
    #endif
    #endif /* (_MTB_HAL_DRIVER_AVAILABLE_NVM_FLASH) */
};

/*******************************************************************************
*       Functions
*******************************************************************************/

//--------------------------------------------------------------------------------------------------
// _mtb_hal_flash_get_mem_region_count
//--------------------------------------------------------------------------------------------------
uint8_t _mtb_hal_flash_get_mem_region_count(void)
{
    return _MTB_HAL_NVM_MEMORY_BLOCKS_COUNT;
}


//--------------------------------------------------------------------------------------------------
// _mtb_hal_flash_get_mem_region
//--------------------------------------------------------------------------------------------------
const mtb_hal_nvm_region_info_t* _mtb_hal_flash_get_mem_region(void)
{
    return &_mtb_hal_nvm_mem_regions[0];
}


#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* (MTB_HAL_DRIVER_AVAILABLE_NVM) */
