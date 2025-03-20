/***************************************************************************//**
* \file mtb_hal_rtc_mxs40ssrss.h
*
* \brief
* Provides common API declarations of the mtb_hal_rtc_mxs40ssrss driver
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

#include "cy_pdl.h"


#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*******************************************************************************
*                           Defines
*******************************************************************************/
#if defined(SRSS_BACKUP_NUM_BREG3) && (SRSS_BACKUP_NUM_BREG3 > 0)
#define _MTB_HAL_RTC_BREG (BACKUP->BREG_SET3[SRSS_BACKUP_NUM_BREG3-1])
#elif defined(SRSS_BACKUP_NUM_BREG2) && (SRSS_BACKUP_NUM_BREG2 > 0)
#define _MTB_HAL_RTC_BREG (BACKUP->BREG_SET2[SRSS_BACKUP_NUM_BREG2-1])
#elif defined(SRSS_BACKUP_NUM_BREG1) && (SRSS_BACKUP_NUM_BREG1 > 0)
#define _MTB_HAL_RTC_BREG (BACKUP->BREG_SET1[SRSS_BACKUP_NUM_BREG1-1])
#elif defined(SRSS_BACKUP_NUM_BREG0) && (SRSS_BACKUP_NUM_BREG0 > 0)
#define _MTB_HAL_RTC_BREG (BACKUP->BREG_SET0[SRSS_BACKUP_NUM_BREG0-1])
#endif
/*******************************************************************************
*                           Typedefs
*******************************************************************************/


/*******************************************************************************
*                        Public Function Prototypes
*******************************************************************************/

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
