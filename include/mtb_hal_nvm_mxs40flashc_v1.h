/***************************************************************************//**
* File Name: mtb_hal_nvm_mxs40flashc_v1.h
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
#if defined (COMPONENT_SECURE_DEVICE)
#define CY_FLASH_D_BASE                   0x32800000UL
#define CY_FLASH_CBUS_D_BASE              0x12800000UL
#else
#define CY_FLASH_D_BASE                   0x22800000UL
#define CY_FLASH_CBUS_D_BASE              0x02800000UL
#endif
#endif
/*******************************************************************************
*       Functions
*******************************************************************************/


//--------------------------------------------------------------------------------------------------
// _mtb_hal_flash_convert_status
//--------------------------------------------------------------------------------------------------
__STATIC_INLINE cy_rslt_t _mtb_hal_flash_convert_status(uint32_t pdl_status)
{
    return (pdl_status == CY_FLASH_DRV_OPERATION_STARTED) ? CY_RSLT_SUCCESS : pdl_status;
}


//--------------------------------------------------------------------------------------------------
// _mtb_hal_flash_is_sram_address
//--------------------------------------------------------------------------------------------------
__STATIC_INLINE bool _mtb_hal_flash_is_sram_address(uint32_t address)
{
    #if defined (CY_SRAM0_BASE)
    bool status = ((CY_SRAM0_BASE <= address) && (address < (CY_SRAM0_BASE + CY_SRAM0_SIZE)));
    #if defined (CY_SRAM1_BASE)
    if (!status)
    {
        status =  ((CY_SRAM1_BASE <= address) && (address < (CY_SRAM1_BASE + CY_SRAM1_SIZE)));
    }
    #endif
    return status;
    #else
    return ((CY_SRAM_BASE <= address) && (address < (CY_SRAM_BASE + CY_SRAM_SIZE)));
    #endif
}


//--------------------------------------------------------------------------------------------------
// _mtb_hal_flash_clear_cache
//--------------------------------------------------------------------------------------------------
__STATIC_INLINE void _mtb_hal_flash_clear_cache(bool clearCache)
{
    #if defined (CY_IP_MXS40FLASHC)
    /* PDL automatically clears cache when necessary */
    CY_UNUSED_PARAMETER(clearCache);
    #else
    if (clearCache)
    {
        Cy_SysLib_ClearFlashCacheAndBuffer();
    }
    #endif
    /* This is for cache on CPU SUBSYSTEM. */
    #if defined(CY_FLASH_CBUS_BASE) && defined(CY_IP_M33SYSCPUSS)
    if (clearCache)
    {
        /* TODO: We will replace it once api is provided by PDL(DRIVERS-18245).
           Becasue we read flash memory using CBUS. So we have to invalidate icache after writing
           flash memory */
        ICACHE0->CMD = ICACHE0->CMD | ICACHE_CMD_INV_Msk;
    }
    #endif
}


//--------------------------------------------------------------------------------------------------
// _mtb_hal_flash_run_operation
//--------------------------------------------------------------------------------------------------
__STATIC_INLINE cy_rslt_t _mtb_hal_flash_run_operation(
    _mtb_hal_flash_operation operation, uint32_t address, const uint32_t* data, bool clearCache)
{
    cy_rslt_t status = (_mtb_hal_flash_is_sram_address((uint32_t)data))
        ? (cy_rslt_t)_mtb_hal_flash_convert_status((cy_rslt_t)operation(address, data))
        : MTB_HAL_NVM_RSLT_ERR_ADDRESS;
    _mtb_hal_flash_clear_cache(clearCache);

    return status;
}


//--------------------------------------------------------------------------------------------------
// _mtb_hal_nvm_is_operation_complete
//--------------------------------------------------------------------------------------------------
__STATIC_INLINE bool _mtb_hal_nvm_is_operation_complete(void)
{
    bool complete = true;

    #if (defined(CY_IP_MXS40SRSS) || (_MTB_HAL_USES_ECT_FLASH) || defined (CY_IP_MXS40FLASHC) || \
    CY_FLASH_NON_BLOCKING_SUPPORTED)
    complete = (CY_FLASH_DRV_SUCCESS == Cy_Flash_IsOperationComplete());
    _mtb_hal_flash_clear_cache(complete);
    #else // if (defined(CY_IP_MXS40SRSS) || (_MTB_HAL_USES_ECT_FLASH) || defined
    // (CY_IP_MXS40FLASHC) || CY_FLASH_NON_BLOCKING_SUPPORTED)
    complete = true;
    #endif \
    /* (defined(CY_IP_MXS40SRSS) || (_MTB_HAL_USES_ECT_FLASH) || defined (CY_IP_MXS40FLASHC) ||
       CY_FLASH_NON_BLOCKING_SUPPORTED) */

    return complete;
}


//--------------------------------------------------------------------------------------------------
// _mtb_hal_nvm_read_helper_flash
//--------------------------------------------------------------------------------------------------
__STATIC_INLINE cy_rslt_t _mtb_hal_nvm_read_helper_flash(uint32_t address, uint8_t* data,
                                                         size_t size)
{
    /* If CBUS with icache, we always use CBUS to read flash memory in order to take advantage of
       icache property. */
    #if defined(CY_FLASH_CBUS_BASE) && defined(CY_IP_M33SYSCPUSS) && defined(SBUS_ALIAS_OFFSET)
    memcpy((void*)data, (void*)(address & ~SBUS_ALIAS_OFFSET), size);
    #else
    memcpy((void*)data, (void*)address, size);
    #endif
    return CY_RSLT_SUCCESS;
}


//--------------------------------------------------------------------------------------------------
// _mtb_hal_nvm_erase_helper_flash
//--------------------------------------------------------------------------------------------------
__STATIC_INLINE cy_rslt_t _mtb_hal_nvm_erase_helper_flash(uint32_t address)
{
    #if (defined(CY_IP_MXS40SRSS) || (_MTB_HAL_USES_ECT_FLASH) || defined (CY_IP_MXS40FLASHC))
    cy_rslt_t status = (cy_rslt_t)_mtb_hal_flash_convert_status(Cy_Flash_EraseRow(address));
    _mtb_hal_flash_clear_cache(true);

    return status;
    #else // if (defined(CY_IP_MXS40SRSS) || (_MTB_HAL_USES_ECT_FLASH) || defined
    // (CY_IP_MXS40FLASHC))
    CY_UNUSED_PARAMETER(address);
    return MTB_HAL_NVM_RSLT_ERR_NOT_SUPPORTED;
    #endif \
    /* (defined(CY_IP_MXS40SRSS) || (_MTB_HAL_USES_ECT_FLASH) || defined (CY_IP_MXS40FLASHC)) */
}


//--------------------------------------------------------------------------------------------------
// _mtb_hal_nvm_write_helper_flash
//--------------------------------------------------------------------------------------------------
__STATIC_INLINE cy_rslt_t _mtb_hal_nvm_write_helper_flash(uint32_t address, const uint32_t* data)
{
    return _mtb_hal_flash_run_operation(Cy_Flash_WriteRow, address, data, true);
}


//--------------------------------------------------------------------------------------------------
// _mtb_hal_nvm_program_helper_flash
//--------------------------------------------------------------------------------------------------
__STATIC_INLINE cy_rslt_t _mtb_hal_nvm_program_helper_flash(uint32_t address, const uint32_t* data)
{
    #if (defined(CY_IP_MXS40SRSS) || (_MTB_HAL_USES_ECT_FLASH) || defined (CY_IP_MXS40FLASHC))
    return _mtb_hal_flash_run_operation(Cy_Flash_ProgramRow, address, data, true);
    #else
    CY_UNUSED_PARAMETER(address);
    CY_UNUSED_PARAMETER(data);
    return CY_RSLT_SUCCESS;
    #endif
}


#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* (MTB_HAL_DRIVER_AVAILABLE_NVM) */
