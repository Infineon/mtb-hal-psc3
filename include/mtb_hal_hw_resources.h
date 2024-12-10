/***************************************************************************//**
* \file mtb_hal_hw_resources.h
*
* \brief
* Provides struct definitions for configuration resources in the PDL.
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
 * \addtogroup group_hal_impl_availability HAL Driver Availability Macros
 * \ingroup group_hal_impl
 * \{
 */

#pragma once

#include "cy_pdl.h"

#ifdef __cplusplus
extern "C" {
#endif

// Documented in mtb_hal.h
#define MTB_HAL_API_VERSION                         (3)

/** \cond INTERNAL */
#define _MTB_HAL_DRIVER_AVAILABLE_SCB         \
    (((CY_IP_MXSCB_INSTANCES) > 0) || ((CY_IP_MXS22SCB_INSTANCES) > 0))

#define _MTB_HAL_DRIVER_AVAILABLE_TCPWM       \
    ((CY_IP_MXTCPWM_INSTANCES) > 0) || ((CY_IP_MXS40TCPWM_INSTANCES) > 0)

#if defined(CY_IP_MXUDB_INSTANCES) && defined(MTB_HAL_UDB_SDIO)
#define _MTB_HAL_DRIVER_AVAILABLE_SDIO_UDB          (1)
#else
#define _MTB_HAL_DRIVER_AVAILABLE_SDIO_UDB          (0)
#endif

#define _MTB_HAL_DRIVER_AVAILABLE_NVM_FLASH   \
    (((CY_IP_MXS40FLASHC) > 0) || ((FLASHC_BASE) > 0) || ((CPUSS_FLASHC_PRESENT) > 0))
#define _MTB_HAL_DRIVER_AVAILABLE_NVM_OTP     ((CY_IP_MXS22RRAMC_INSTANCES) > 0)
#define _MTB_HAL_DRIVER_AVAILABLE_NVM_RRAM    ((CY_IP_MXS22RRAMC_INSTANCES) > 0)

#define _MTB_HAL_DRIVER_AVAILABLE_DMA_DW      \
    (((CY_IP_M4CPUSS_DMA_INSTANCES) > 0) || ((CY_IP_M7CPUSS_DMA_INSTANCES) > 0) || ((CY_IP_MXDW_INSTANCES) > 0))
#define _MTB_HAL_DRIVER_AVAILABLE_DMA_DMAC    \
    (((CY_IP_M4CPUSS_DMAC_INSTANCES) > 0) || ((CY_IP_M7CPUSS_DMAC_INSTANCES) > 0) || ((CY_IP_MXAHBDMAC_INSTANCES) > 0) || ((CY_IP_MXSAXIDMAC_INSTANCES) > 0))

#if !defined(CY_DEVICE_SERIES_PSOCE84)

#define _MTB_HAL_DRIVER_AVAILABLE_ADC_SAR     \
    ((CY_IP_MXS40PASS_SAR_INSTANCES) > 0) || ((CY_IP_MXS40EPASS_ESAR_INSTANCES) > 0) ||                                           \
                                            ((CY_IP_MXS40MCPASS_INSTANCES) > 0)|| ((CY_IP_MXS22LPPASS_SAR_INSTANCES) > 0)
#define _MTB_HAL_DRIVER_AVAILABLE_ADC_MIC     ((CY_IP_MXS40ADCMIC_INSTANCES) > 0)

#define _MTB_HAL_DRIVER_AVAILABLE_COMP_LP     \
    ((CY_IP_MXLPCOMP_INSTANCES) > 0) || ((CY_IP_MXS40LPCOMP_INSTANCES) > 0)
#define _MTB_HAL_DRIVER_AVAILABLE_COMP_CSG    ((CY_IP_MXS40MCPASS_INSTANCES) > 0)
#define _MTB_HAL_DRIVER_AVAILABLE_COMP_PTC    ((CY_IP_MXS22LPPASS_PTC_INSTANCES) > 0)
#define _MTB_HAL_DRIVER_AVAILABLE_COMP_CTB_PASS   \
    (((CY_IP_MXS40PASS_INSTANCES) > 0) && ((CY_IP_MXS40PASS_CTB_INSTANCES) > 0))
#define _MTB_HAL_DRIVER_AVAILABLE_COMP_CTB_LPASS  \
    (((CY_IP_MXS22LPPASS_INSTANCES) > 0) && ((CY_IP_MXS22LPPASS_CTB_INSTANCES) > 0))
#define _MTB_HAL_DRIVER_AVAILABLE_COMP_CTB        \
    (_MTB_HAL_DRIVER_AVAILABLE_COMP_CTB_PASS || _MTB_HAL_DRIVER_AVAILABLE_COMP_CTB_LPASS)

/* MXCRYPTOLITE is not yet supported */
// #define _MTB_HAL_DRIVER_AVAILABLE_CRYPTO   (((CY_IP_MXCRYPTO_INSTANCES) > 0) ||
// ((CY_IP_MXCRYPTOLITE_INSTANCES) > 0))
#define _MTB_HAL_DRIVER_AVAILABLE_CRYPTO      ((CY_IP_MXCRYPTO_INSTANCES) > 0)

#else /* !defined(CY_DEVICE_SERIES_PSOCE84) */
#define _MTB_HAL_DRIVER_AVAILABLE_ADC_SAR           (0)
#define _MTB_HAL_DRIVER_AVAILABLE_ADC_MIC           (0)

#define _MTB_HAL_DRIVER_AVAILABLE_COMP_LP           (0)
#define _MTB_HAL_DRIVER_AVAILABLE_COMP_CSG          (0)
#define _MTB_HAL_DRIVER_AVAILABLE_COMP_PTC          (0)
#define _MTB_HAL_DRIVER_AVAILABLE_COMP_CTB_PASS     (0)
#define _MTB_HAL_DRIVER_AVAILABLE_COMP_CTB_LPASS    (0)
#define _MTB_HAL_DRIVER_AVAILABLE_COMP_CTB          (0)

#define _MTB_HAL_DRIVER_AVAILABLE_CRYPTO            (0)
#endif /* if !defined(CY_DEVICE_SERIES_PSOCE84) */

/* Alignment for DMA descriptors */
#if (CY_IP_MXSAXIDMAC)
/* AXI DMA controller has a 64-bit AXI master interface */
    #define _MTB_HAL_DMA_ALIGN        CY_ALIGN(8)
#elif (CY_CPU_CORTEX_M7) && defined(ENABLE_CM7_DATA_CACHE)
    #define _MTB_HAL_DMA_ALIGN        CY_ALIGN(__SCB_DCACHE_LINE_SIZE)
#else
    #define _MTB_HAL_DMA_ALIGN
#endif

/** \endcond */

// Documented in mtb_hal.h
#define MTB_HAL_DRIVER_AVAILABLE_GPIO         (1)
#define MTB_HAL_DRIVER_AVAILABLE_GPIO_PORT    (1)
#define MTB_HAL_DRIVER_AVAILABLE_CLOCK        (1)
#define MTB_HAL_DRIVER_AVAILABLE_SYSTEM       (1)
#define MTB_HAL_DRIVER_AVAILABLE_I2C          (1)
#define MTB_HAL_DRIVER_AVAILABLE_SPI          (1)
#define MTB_HAL_DRIVER_AVAILABLE_UART         (1)
#define MTB_HAL_DRIVER_AVAILABLE_NVM          (1)

/* Not used */
#define MTB_HAL_DRIVER_AVAILABLE_RTC          (0)
#define MTB_HAL_DRIVER_AVAILABLE_SYSPM        (0)
#define MTB_HAL_DRIVER_AVAILABLE_ADC          (0)
#define MTB_HAL_DRIVER_AVAILABLE_COMP         (0)
#define MTB_HAL_DRIVER_AVAILABLE_TIMER        (0)
#define MTB_HAL_DRIVER_AVAILABLE_PWM          (0)
#define MTB_HAL_DRIVER_AVAILABLE_DMA          (0)
#define MTB_HAL_DRIVER_AVAILABLE_MEMORYSPI    (0)
#define MTB_HAL_DRIVER_AVAILABLE_LPTIMER      (0)
#define MTB_HAL_DRIVER_AVAILABLE_SDHC         (0)
#define MTB_HAL_DRIVER_AVAILABLE_SDIO         (0)
#define MTB_HAL_DRIVER_AVAILABLE_TRNG         (0)
#define MTB_HAL_DRIVER_AVAILABLE_CORDIC       (0)
#define MTB_HAL_DRIVER_AVAILABLE_IPC          (0)
#define MTB_HAL_DRIVER_AVAILABLE_SDIO_HOST    (0)
#define MTB_HAL_DRIVER_AVAILABLE_SDIO_DEV     (0)

/** \} group_hal_impl_availability */
/**
 * \addtogroup group_hal_impl_hw_types
 * \ingroup group_hal_impl
 * \{
 */


/* NOTE: Any changes made to this enum must also be made to the hardware manager resource tracking
 */
/** Resource types that the hardware manager supports */
typedef enum
{
    MTB_HAL_RSC_ADC,       /*!< Analog to digital converter */
    MTB_HAL_RSC_ADCMIC,    /*!< Analog to digital converter with Analog Mic support */
    MTB_HAL_RSC_BLESS,     /*!< Bluetooth communications block */
    MTB_HAL_RSC_CAN,       /*!< CAN communication block */
    MTB_HAL_RSC_CLKPATH,   /*!< Clock Path. DEPRECATED. */
    MTB_HAL_RSC_CLOCK,     /*!< Clock */
    MTB_HAL_RSC_CORDIC,    /*!< CORDIC */
    MTB_HAL_RSC_CRYPTO,    /*!< Crypto hardware accelerator */
    MTB_HAL_RSC_CSG,       /*!< Comparator and slope generator (CSG) */
    MTB_HAL_RSC_DAC,       /*!< Digital to analog converter */
    MTB_HAL_RSC_DMA,       /*!< DMA controller */
    MTB_HAL_RSC_DW,        /*!< Datawire DMA controller */
    MTB_HAL_RSC_ETH,       /*!< Ethernet communications block */
    MTB_HAL_RSC_GPIO,      /*!< General purpose I/O pin */
    MTB_HAL_RSC_I2S,       /*!< I2S communications block */
    MTB_HAL_RSC_I3C,       /*!< I3C communications block */
    MTB_HAL_RSC_KEYSCAN,   /*!< KeyScan block */
    MTB_HAL_RSC_LCD,       /*!< Segment LCD controller */
    MTB_HAL_RSC_LIN,       /*!< LIN communications block */
    MTB_HAL_RSC_LPCOMP,    /*!< Low power comparator */
    MTB_HAL_RSC_LPTIMER,   /*!< Low power timer */
    MTB_HAL_RSC_MOTIF,     /*!< MOTIF */
    MTB_HAL_RSC_OPAMP,     /*!< Opamp */
    MTB_HAL_RSC_PDM,       /*!< PCM/PDM communications block */
    MTB_HAL_RSC_PTC,       /*!< Programmable Threshold comparator */
    MTB_HAL_RSC_SMIF,      /*!< Quad-SPI communications block */
    MTB_HAL_RSC_RTC,       /*!< Real time clock */
    MTB_HAL_RSC_SCB,       /*!< Serial Communications Block */
    MTB_HAL_RSC_SDHC,      /*!< SD Host Controller */
    MTB_HAL_RSC_SDIODEV,   /*!< SDIO Device Block */
    MTB_HAL_RSC_TCPWM,     /*!< Timer/Counter/PWM block */
    MTB_HAL_RSC_TDM,       /*!< TDM block */
    MTB_HAL_RSC_UDB,       /*!< UDB Array */
    MTB_HAL_RSC_USB,       /*!< USB communication block */
    MTB_HAL_RSC_INVALID    /*!< Placeholder for invalid type */
} mtb_hal_resource_t;

/** Clock Reference Structure in case of peri divider */
typedef struct
{
    en_clk_dst_t          clk_dst;  /* PDL uses the target IP to identify the peri group */
    cy_en_divider_types_t div_type; /* Clock divider type */
    uint32_t              div_num;  /* Clock divider number */
} mtb_hal_peri_div_t;

/** Clock Reference Structure in case of high frequency divider */
typedef struct
{
    uint32_t              inst_num;  /* Clock number */
} mtb_hal_hf_clock_t;

/** Get the clock frequency */
typedef uint32_t (* mtb_hal_clock_get_frequency_hz_t)(const void* clock_ref);
/** Set the clock frequency */
typedef cy_rslt_t (* mtb_hal_clock_set_frequency_hz_t)(const void* clock_ref,
                                                       uint32_t desired_frequency_hz,
                                                       uint32_t tolerance_ppm);
/** Enable or Disable the clock */
typedef cy_rslt_t (* mtb_hal_clock_set_enabled_t)(const void* clock_ref, bool enabled);

/** Clock Interface structure for clocks */
typedef struct
{
    mtb_hal_clock_get_frequency_hz_t get_frequency_hz; /* Get the clock frequency */
    mtb_hal_clock_set_frequency_hz_t set_frequency_hz; /* Set the clock frequency */
    mtb_hal_clock_set_enabled_t      set_enabled;      /* Enable or Disable the clock */
} mtb_hal_clock_interface_t;


/** @brief Clock object
 * Application shall provide implementations for the functions needed by the clock
 * object. They are considered an implementation detail which is subject to change
 * between platforms and/or devices. */
typedef struct
{
    const void*                          clock_ref; // In a typical case, this points to either
                                                    // mtb_hal_peri_div_t or mtb_hal_hf_clock_t
                                                    // types
    const mtb_hal_clock_interface_t*     interface;
} mtb_hal_clock_t;

/**
 * @brief Represents a particular instance of a resource on the chip.
 * Application code should not rely on the specific contents of this struct.
 * They are considered an implementation detail which is subject to change
 * between platforms and/or HAL releases.
 */
typedef struct
{
    mtb_hal_resource_t type;    //!< The resource block type
    uint8_t          block_num; //!< The resource block index
    /**
     * The channel number, if the resource type defines multiple channels
     * per block instance. Otherwise, 0 */
    uint8_t          channel_num;
} mtb_hal_resource_inst_t;

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/** \} group_hal_impl_hw_types */
