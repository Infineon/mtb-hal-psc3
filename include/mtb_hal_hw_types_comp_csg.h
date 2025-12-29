/***************************************************************************//**
* \file mtb_hal_hw_types_comp_csg.h
*
*********************************************************************************
* \copyright
* Copyright(c) 2024-2025 Infineon Technologies AG or an affiliate of
* Infineon Technologies AG
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

#if ((CY_IP_MXS40MCPASS_INSTANCES) > 0)
#if !defined(_MTB_HAL_DRIVER_AVAILABLE_COMP_CSG)
#define _MTB_HAL_DRIVER_AVAILABLE_COMP_CSG (1u)
#endif // !defined(_MTB_HAL_DRIVER_AVAILABLE_COMP_CSG)


typedef uint8_t _mtb_hal_csg_slice_t;
typedef cy_stc_hppass_cfg_t _mtb_hal_csg_pdl_config_t;

#endif // ((CY_IP_MXS40MCPASS_INSTANCES) > 0)
