/*******************************************************************************
* File Name: mtb_hal_uart.c
*
* Description:
* Provides a high level interface for interacting with the Infineon UART. This is
* a wrapper around the lower level PDL API.
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

#include <stdlib.h>
#include <string.h>

#include "mtb_hal_uart.h"
#include "mtb_hal_gpio.h"
#include "mtb_hal_system_impl.h"
#include "mtb_hal_utils.h"
#include "mtb_hal_irq_impl.h"
#if defined(HAL_NEXT_TODO)
#if (MTB_HAL_DRIVER_AVAILABLE_DMA)
#include "mtb_hal_dma.h"
#endif /* (MTB_HAL_DRIVER_AVAILABLE_DMA */
#endif //defined(HAL_NEXT_TODO)
#if defined(COMPONENT_MW_ASYNC_TRANSFER)
#include "mtb_async_transfer.h"
#endif // defined(COMPONENT_MW_ASYNC_TRANSFER)

#if (MTB_HAL_DRIVER_AVAILABLE_UART)

/** \addtogroup group_hal_uart UART
 * \ingroup group_hal
 * \{
 * \section group_hal_uart_dma_transfers Asynchronous DMA Transfers
 * Asynchronous transfers can be performed using DMA to load data into
 * and from the FIFOs. The purpose of this is to minimize CPU load on
 * large transfers.
 *
 * \section group_hal_uart_pm_strategy UART at different power modes
 * In order to allow UART to maintain the specified baud rate in all
 * power modes as the clock inputs can change when switching power modes,
 * it is necessary to adjust the baud rate. The UART HAL cannot automatically
 * update the baud rate  because it does not own the source clock.
 * The application must adjust the UART source clock itself to retain
 * the desired frequency.
 * \} group_hal_uart
 */


#if defined(__cplusplus)
extern "C"
{
#endif

/*******************************************************************************
*                           Private Defines
*******************************************************************************/


/*******************************************************************************
*                           Private Variables
*******************************************************************************/

/* Keep track of the current UART object to ensure we are referencing the
 * correct one, as we don't directly pass the object through the PDL callback
 * indirection.  This also protects against a nested callback situation. */
static volatile mtb_hal_uart_t* _mtb_hal_uart_irq_obj = NULL;

/*******************************************************************************
*                       Private Function Definitions
*******************************************************************************/

//HAL_NEXT_TODO Propose to move to the PDL
/** Returns the UART's mode of operation */
__STATIC_INLINE uint32_t Cy_SCB_UART_GetMode(CySCB_Type const* base)
{
    return _FLD2VAL(SCB_UART_CTRL_MODE, SCB_UART_CTRL(base));
}


//--------------------------------------------------------------------------------------------------
// _mtb_hal_uart_irq_handler
//--------------------------------------------------------------------------------------------------
static void _mtb_hal_uart_irq_handler(mtb_hal_uart_t* obj)
{
    /* Save the old value and store it aftewards in case we get into a nested IRQ situation */
    /* Safe to cast away volatile because we don't expect this pointer to be changed while we're in
       here, they
     * just might change where the original pointer points */
    mtb_hal_uart_t* old_irq_obj = (mtb_hal_uart_t*)_mtb_hal_uart_irq_obj;

    if (NULL == obj)
    {
        return;  /* The interrupt object is not valid */
    }

    _mtb_hal_uart_irq_obj = obj;

    /* Cy_SCB_UART_Interrupt() manipulates the interrupt masks. Save a copy to work around it. */
    uint32_t txMasked = Cy_SCB_GetTxInterruptStatusMasked(obj->base);
    uint32_t rxMasked = Cy_SCB_GetRxInterruptStatusMasked(obj->base);

    /* SCB high-level API interrupt handler. Must be called as high-level API is used in the HAL */
    Cy_SCB_UART_Interrupt(obj->base, obj->context);

    /* Custom handling for TX overflow (cannot occur using HAL API but can occur if user makes
       custom modifications)
        Note: This is partially handled in Cy_SCB_UART_Interrupt()
        but it only takes care of NACK and ARB_LOST errors. */
    if (0UL != (CY_SCB_UART_TX_OVERFLOW & txMasked))
    {
        Cy_SCB_ClearTxInterrupt(obj->base, CY_SCB_UART_TX_OVERFLOW);

        if (NULL != obj->context->cbEvents)
        {
            obj->context->cbEvents(CY_SCB_UART_TRANSMIT_ERR_EVENT);
        }
    }

    /* Custom handling for TX underflow (cannot occur using HAL API but can occur if user makes
       custom modifications)
        Note: This is partially handled in Cy_SCB_UART_Interrupt()
        but it only takes care of NACK and ARB_LOST errors. */
    if (0UL != (CY_SCB_UART_TX_UNDERFLOW & txMasked))
    {
        Cy_SCB_ClearTxInterrupt(obj->base, CY_SCB_UART_TX_UNDERFLOW);

        if (NULL != obj->context->cbEvents)
        {
            obj->context->cbEvents(CY_SCB_UART_TRANSMIT_ERR_EVENT);
        }
    }

    /* Custom handling for TX FIFO trigger.
        Note: This is partially handled in Cy_SCB_UART_Interrupt()
        when processing CY_SCB_TX_INTR_LEVEL. Do not clear the interrupt. */
    if (0UL != (CY_SCB_UART_TX_TRIGGER & txMasked))
    {
        if (NULL != obj->context->cbEvents)
        {
            obj->context->cbEvents((uint32_t)MTB_HAL_UART_IRQ_TX_FIFO);
        }
    }

    /* Manually clear the tx done interrupt and re-enable the interrupt mask */
    if (0UL != (CY_SCB_UART_TX_DONE & txMasked))
    {
        Cy_SCB_ClearTxInterrupt(obj->base, CY_SCB_UART_TX_DONE);
        Cy_SCB_SetTxInterruptMask(obj->base, Cy_SCB_GetTxInterruptMask(
                                      obj->base) | CY_SCB_UART_TX_DONE);
    }

    /* Custom handling for RX underflow (cannot occur using HAL API but can occur if user makes
       custom modifications)
        Note: This is partially handled in Cy_SCB_UART_Interrupt()
        which takes care of overflow, frame and parity errors. */
    if (0UL != (CY_SCB_RX_INTR_UNDERFLOW & rxMasked))
    {
        Cy_SCB_ClearRxInterrupt(obj->base, CY_SCB_RX_INTR_UNDERFLOW);

        if (NULL != obj->context->cbEvents)
        {
            obj->context->cbEvents(CY_SCB_UART_RECEIVE_ERR_EVENT);
        }
    }

    /* Custom handling for RX FIFO trigger
        Note: This is partially handled in Cy_SCB_UART_Interrupt()
        when processing CY_SCB_RX_INTR_LEVEL. Do not clear the interrupt. */
    if (0UL != (CY_SCB_UART_RX_TRIGGER & rxMasked))
    {
        if (NULL != obj->context->cbEvents)
        {
            obj->context->cbEvents((uint32_t)MTB_HAL_UART_IRQ_RX_FIFO);
        }
    }
    _mtb_hal_uart_irq_obj = old_irq_obj;
}


#if defined(HAL_NEXT_TODO)
#if defined(BCM55500)
// Interrupts are implemented oddly in PDL: they auto-disable themselves after firing.  So
// re-enable
static void _mtb_hal_uart0_irq_handler(void)
{
    _mtb_hal_uart_irq_handler(scb_0_interrupt_IRQn);
    Cy_SCB_EnableInterrupt(SCB0);
}


//--------------------------------------------------------------------------------------------------
// _mtb_hal_uart1_irq_handler
//--------------------------------------------------------------------------------------------------
static void _mtb_hal_uart1_irq_handler(void)
{
    _mtb_hal_uart_irq_handler(scb_1_interrupt_IRQn);
    Cy_SCB_EnableInterrupt(SCB1);
}


//--------------------------------------------------------------------------------------------------
// _mtb_hal_uart2_irq_handler
//--------------------------------------------------------------------------------------------------
static void _mtb_hal_uart2_irq_handler(void)
{
    _mtb_hal_uart_irq_handler(scb_2_interrupt_IRQn);
    Cy_SCB_EnableInterrupt(SCB2);
}


static CY_SCB_IRQ_THREAD_CB_t _mtb_hal_irq_cb[3] =
    { _mtb_hal_uart0_irq_handler, _mtb_hal_uart1_irq_handler, _mtb_hal_uart2_irq_handler };
#endif // defined(BCM55500)
#endif // if defined(HAL_NEXT_TODO)

//--------------------------------------------------------------------------------------------------
// _mtb_hal_uart_cb_wrapper
//--------------------------------------------------------------------------------------------------
static void _mtb_hal_uart_cb_wrapper(uint32_t event)
{
    /* Safe to cast away volatile because we don't expect this pointer to be changed while we're in
       here, they
     * just might change where the original pointer points */
    mtb_hal_uart_t* obj = (mtb_hal_uart_t*)_mtb_hal_uart_irq_obj;
    mtb_hal_uart_event_t anded_events = (mtb_hal_uart_event_t)(obj->irq_cause & event);

    if (anded_events)
    {
        mtb_hal_uart_event_callback_t callback =
            (mtb_hal_uart_event_callback_t)obj->callback_data.callback;
        if (NULL != callback)
        {
            callback(obj->callback_data.callback_arg, anded_events);
        }
    }
}


#if defined(COMPONENT_MW_ASYNC_TRANSFER)

/** Handles the UART async transfer Invokes the fifo level event processing functions */
cy_rslt_t _mtb_hal_uart_async_transfer_handler(void* obj, uint32_t event)
{
    uint32_t      direction = 0;
    cy_rslt_t     result = CY_RSLT_SUCCESS;
    mtb_hal_uart_t* uart_obj = (mtb_hal_uart_t*)obj;

    /* RX FIFO level event  */
    if (0u != (CY_SCB_UART_RX_TRIGGER & event))
    {
        direction = MTB_ASYNC_TRANSFER_DIRECTION_READ;
    }
    /* TX FIFO level event  */
    if (0u != (CY_SCB_UART_TX_TRIGGER & event))
    {
        direction |= MTB_ASYNC_TRANSFER_DIRECTION_WRITE;
    }
    if (direction)
    {
        result =
            mtb_async_transfer_process_fifo_level_event(uart_obj->async_ctx,
                                                        (mtb_async_transfer_direction_t)direction);
    }
    return result;
}


/** Returns the number of elements that can be written to the TX FIFO */
uint32_t _mtb_hal_uart_async_transfer_get_num_tx_fifo(void* inst_ref)
{
    return (Cy_SCB_GetFifoSize((CySCB_Type*)inst_ref) -
            Cy_SCB_UART_GetNumInTxFifo((CySCB_Type*)inst_ref));
}


/** Enable/disable the UART RX Interrupt event */
void _mtb_hal_uart_async_transfer_enable_rx_event(void* inst_ref, bool enable)
{
    uint32_t rx_mask = Cy_SCB_GetRxInterruptMask((CySCB_Type*)inst_ref);
    rx_mask = (enable ? (rx_mask | CY_SCB_UART_RX_TRIGGER) : (rx_mask & ~CY_SCB_UART_RX_TRIGGER));
    Cy_SCB_SetRxInterruptMask((CySCB_Type*)inst_ref, rx_mask);
}


/** Enable/disable the UART TX Interrupt event */
void _mtb_hal_uart_async_transfer_enable_tx_event(void* inst_ref, bool enable)
{
    uint32_t tx_mask = Cy_SCB_GetTxInterruptMask((CySCB_Type*)inst_ref);
    //Disable the TX done interrupt. Re-enable once the tx data is transferred to the TX FIFO
    tx_mask &= ~CY_SCB_UART_TX_DONE;
    tx_mask = (enable ? (tx_mask | CY_SCB_UART_TX_TRIGGER) : (tx_mask & ~CY_SCB_UART_TX_TRIGGER));
    Cy_SCB_SetTxInterruptMask((CySCB_Type*)inst_ref, tx_mask);
}


/** Configure the UART interface for async transfer */
cy_rslt_t _mtb_hal_uart_config_async_common(mtb_hal_uart_t* obj,
                                            mtb_async_transfer_context_t* context,
                                            mtb_async_transfer_interface_t* interface)
{
    /* Set the UART peripheral interface */
    memset(interface, 0, sizeof(mtb_async_transfer_interface_t));

    interface->rx_addr = (uint32_t*)&((obj->base)->RX_FIFO_RD);
    interface->tx_addr = (uint32_t*)&((obj->base)->TX_FIFO_WR);
    /*Set this directly to the communications peripheral interface base address.*/
    interface->inst_ref = obj->base;
    interface->get_num_rx_fifo = (mtb_async_transfer_get_num_fifo_t)Cy_SCB_UART_GetNumInRxFifo;
    interface->get_num_tx_fifo =
        (mtb_async_transfer_get_num_fifo_t)_mtb_hal_uart_async_transfer_get_num_tx_fifo;
    interface->set_rx_fifo_level = (mtb_async_transfer_set_fifo_level_t)Cy_SCB_SetRxFifoLevel;
    interface->set_tx_fifo_level = (mtb_async_transfer_set_fifo_level_t)Cy_SCB_SetTxFifoLevel;
    interface->enable_rx_event = _mtb_hal_uart_async_transfer_enable_rx_event;
    interface->enable_tx_event = _mtb_hal_uart_async_transfer_enable_tx_event;
    interface->rx_fifo_depth = Cy_SCB_GetFifoSize(obj->base);
    interface->tx_fifo_depth = Cy_SCB_GetFifoSize(obj->base);
    interface->transfer_width = _mtb_hal_scb_get_transfer_width(obj->base); /* Read from config
                                                                               registers */

    obj->async_handler = _mtb_hal_uart_async_transfer_handler;
    obj->async_ctx = context;

    return CY_RSLT_SUCCESS;
}


//--------------------------------------------------------------------------------------------------
// _mtb_hal_uart_async_transfer_event_callback
//--------------------------------------------------------------------------------------------------
void _mtb_hal_uart_async_transfer_event_callback(void* callback_arg,
                                                 mtb_async_transfer_direction_t direction)
{
    mtb_hal_uart_t*       obj    = (mtb_hal_uart_t*)callback_arg;
    mtb_hal_uart_event_t  event  =
        (direction ==
         MTB_ASYNC_TRANSFER_DIRECTION_READ) ? MTB_HAL_UART_IRQ_RX_DONE :
        MTB_HAL_UART_IRQ_TX_TRANSMIT_IN_FIFO;
    event &= (mtb_hal_uart_event_t)(obj->irq_cause);

    //Re-enable the tx done interrupt.
    if (obj->irq_cause & MTB_HAL_UART_IRQ_TX_DONE)
    {
        mtb_hal_uart_enable_event(obj, MTB_HAL_UART_IRQ_TX_DONE, true);
    }
    if (event)
    {
        mtb_hal_uart_event_callback_t callback =
            (mtb_hal_uart_event_callback_t)obj->callback_data.callback;
        if (NULL != callback)
        {
            callback(obj->callback_data.callback_arg, event);
        }
    }
}


#endif // defined(COMPONENT_MW_ASYNC_TRANSFER)

/*******************************************************************************
*                        Public Function Definitions
*******************************************************************************/
/** Initialize the UART Object */
cy_rslt_t mtb_hal_uart_setup(mtb_hal_uart_t* obj, const mtb_hal_uart_configurator_t* cfg,
                             cy_stc_scb_uart_context_t* context, const mtb_hal_clock_t* clock)
{
    CY_ASSERT(NULL != obj);
    CY_ASSERT(NULL != cfg);
    CY_ASSERT(NULL != context);

    memset(obj, 0, sizeof(mtb_hal_uart_t));
    obj->base    = cfg->base;
    obj->clock   = (clock == NULL) ? cfg->clock : clock;
    obj->context = context;
    obj->irq_cause = MTB_HAL_UART_IRQ_NONE;

    return CY_RSLT_SUCCESS;
}


/** Configure the baud rate */
cy_rslt_t mtb_hal_uart_set_baud(mtb_hal_uart_t* obj, uint32_t baudrate, uint32_t* actualbaud)
{
    CY_ASSERT(NULL != obj->clock->interface->get_frequency_hz);
    CY_ASSERT(NULL != obj->clock->interface->set_frequency_hz);

    cy_rslt_t result;
    uint32_t  original_freq;
    uint32_t  desired_freq;
    uint32_t  actual_freq;
    uint32_t  tolerance;
    uint32_t  oversample;

    Cy_SCB_UART_Disable(obj->base, NULL);

    original_freq = obj->clock->interface->get_frequency_hz(obj->clock->clock_ref);
    oversample = Cy_SCB_UART_GetOverSample(obj->base); /* User-controlled oversample value */
    desired_freq =  baudrate * oversample;

    /* Check if the baudrate is achievable */
    result = obj->clock->interface->set_frequency_hz(obj->clock->clock_ref, desired_freq,
                                                     MTB_HAL_UART_CLOCK_FREQ_MAX_TOLERANCE_PPM);

    if (result == CY_RSLT_SUCCESS)
    {
        actual_freq = obj->clock->interface->get_frequency_hz(obj->clock->clock_ref);
        tolerance   =
            (uint32_t)abs(_mtb_hal_utils_calculate_tolerance(MTB_HAL_TOLERANCE_PPM,
                                                             desired_freq,
                                                             actual_freq));

        if (tolerance > MTB_HAL_UART_CLOCK_FREQ_MAX_TOLERANCE_PPM)
        {
            result = MTB_HAL_UART_RSLT_ERR_CLOCK_FREQ_TOLERANCE;
        }
    }

    if (result != CY_RSLT_SUCCESS)
    {
        /* Revert back to original */
        actual_freq = original_freq;
        obj->clock->interface->set_frequency_hz(obj->clock->clock_ref, original_freq,
                                                MTB_HAL_UART_CLOCK_FREQ_MAX_TOLERANCE_PPM);
    }

    if (actualbaud != NULL)
    {
        *actualbaud = actual_freq/oversample;
    }

    Cy_SCB_UART_Enable(obj->base);
    return result;
}


/** Check the number of bytes available to read from the receive buffers */
uint32_t mtb_hal_uart_readable(mtb_hal_uart_t* obj)
{
    uint32_t number_available = Cy_SCB_UART_GetNumInRxFifo(obj->base);

    if (obj->context->rxRingBuf != NULL)
    {
        number_available += Cy_SCB_UART_GetNumInRingBuffer(obj->base, obj->context);
    }
    return number_available;
}


/** Check the number of bytes than can be written to the transmit buffer */
uint32_t mtb_hal_uart_writable(mtb_hal_uart_t* obj)
{
    return Cy_SCB_GetFifoSize(obj->base) - Cy_SCB_GetNumInTxFifo(obj->base);
}


/** Clear the UART buffers */
cy_rslt_t mtb_hal_uart_clear(mtb_hal_uart_t* obj)
{
    Cy_SCB_UART_ClearRxFifo(obj->base);
    Cy_SCB_UART_ClearTxFifo(obj->base);

    if (obj->context->rxRingBuf != NULL)
    {
        Cy_SCB_UART_ClearRingBuffer(obj->base, obj->context);
    }

    return CY_RSLT_SUCCESS;
}


/** Configure the UART CTS for flow control */
cy_rslt_t mtb_hal_uart_enable_cts_flow_control(mtb_hal_uart_t* obj, bool enable)
{
    enable ? Cy_SCB_UART_EnableCts(obj->base) : Cy_SCB_UART_DisableCts(obj->base);
    return CY_RSLT_SUCCESS;
}


/** Begin synchronous TX transfer. */
cy_rslt_t mtb_hal_uart_write(mtb_hal_uart_t* obj, void* tx, size_t* tx_length)
{
    *tx_length = Cy_SCB_UART_PutArray(obj->base, tx, *tx_length);
    return CY_RSLT_SUCCESS;
}


/** Begin synchronous RX transfer */
cy_rslt_t mtb_hal_uart_read(mtb_hal_uart_t* obj, void* rx, size_t* rx_length)
{
    *rx_length = Cy_SCB_UART_GetArray(obj->base, rx, *rx_length);
    return CY_RSLT_SUCCESS;
}


/** Determines if the UART peripheral is currently in use for TX */
bool mtb_hal_uart_is_tx_active(mtb_hal_uart_t* obj)
{
    return (0UL != (obj->context->txStatus & CY_SCB_UART_TRANSMIT_ACTIVE)) || !Cy_SCB_IsTxComplete(
        obj->base)
           #if defined(COMPONENT_MW_ASYNC_TRANSFER)
           || ((NULL != obj->async_ctx) && (!mtb_async_transfer_available_write(obj->async_ctx)))
           #endif // defined(COMPONENT_MW_ASYNC_TRANSFER)
    ;
}


/** Register a uart callback handler */
void mtb_hal_uart_register_callback(mtb_hal_uart_t* obj, mtb_hal_uart_event_callback_t callback,
                                    void* callback_arg)
{
    uint32_t savedIntrStatus = mtb_hal_system_critical_section_enter();
    obj->callback_data.callback = (cy_israddress)callback;
    obj->callback_data.callback_arg = callback_arg;
    mtb_hal_system_critical_section_exit(savedIntrStatus);
    Cy_SCB_UART_RegisterCallback(obj->base, _mtb_hal_uart_cb_wrapper, obj->context);
}


/** Enable or disable specified UART events.*/
void mtb_hal_uart_enable_event(mtb_hal_uart_t* obj, mtb_hal_uart_event_t event, bool enable)
{
    #if defined(HAL_NEXT_TODO)
    #if defined(BCM55500)
    Cy_SCB_DisableInterrupt(obj->base);
    #endif
    #endif //defined(HAL_NEXT_TODO)

    uint32_t tx_mask = 0x0;
    uint32_t rx_mask = 0x0;

    if (event & MTB_HAL_UART_IRQ_TX_EMPTY)
    {
        tx_mask |= CY_SCB_UART_TX_EMPTY;
    }
    if (event & MTB_HAL_UART_IRQ_TX_DONE)
    {
        tx_mask |= CY_SCB_UART_TX_DONE;
    }
    if (event & MTB_HAL_UART_IRQ_TX_ERROR)
    {
        // Omit underflow condition as the interrupt perpetually triggers
        //Standard mode only uses OVERFLOW irq
        if (Cy_SCB_UART_GetMode(obj->base) == CY_SCB_UART_STANDARD)
        {
            tx_mask |= (CY_SCB_UART_TX_OVERFLOW | CY_SCB_UART_TRANSMIT_ERR);
        }
        //SMARTCARD mode uses OVERFLOW, NACK, and ARB_LOST irq's
        else if (Cy_SCB_UART_GetMode(obj->base) == CY_SCB_UART_SMARTCARD)
        {
            tx_mask |=
                (CY_SCB_UART_TX_OVERFLOW | CY_SCB_TX_INTR_UART_NACK | CY_SCB_TX_INTR_UART_ARB_LOST |
                 CY_SCB_UART_TRANSMIT_ERR);
        }
        //LIN Mode only uses OVERFLOW, ARB_LOST irq's
        else
        {
            tx_mask |=
                (CY_SCB_UART_TX_OVERFLOW | CY_SCB_TX_INTR_UART_ARB_LOST | CY_SCB_UART_TRANSMIT_ERR);
        }
    }
    if (event & MTB_HAL_UART_IRQ_TX_FIFO)
    {
        tx_mask |= CY_SCB_UART_TX_TRIGGER;
    }

    if (event & MTB_HAL_UART_IRQ_RX_NOT_EMPTY)
    {
        rx_mask |= CY_SCB_UART_RX_NOT_EMPTY;
    }
    if (event & MTB_HAL_UART_IRQ_RX_ERROR)
    {
        // Omit underflow condition as the interrupt perpetually triggers.
        rx_mask |= CY_SCB_UART_RECEIVE_ERR;
    }
    if (event & MTB_HAL_UART_IRQ_RX_FIFO)
    {
        rx_mask |= CY_SCB_UART_RX_TRIGGER;
    }

    if (enable)
    {
        obj->irq_cause |= event;
    }
    else
    {
        obj->irq_cause &= ~event;
    }
    uint32_t current_tx_mask = Cy_SCB_GetTxInterruptMask(obj->base);
    uint32_t current_rx_mask = Cy_SCB_GetRxInterruptMask(obj->base);

    if (event == MTB_HAL_UART_IRQ_NONE)
    {
        /* "No interrupt" is equivalent for both "enable" and "disable" */
        enable = false;
        tx_mask = CY_SCB_TX_INTR_MASK;
        rx_mask = CY_SCB_RX_INTR_MASK;
    }
    if (enable && tx_mask)
    {
        Cy_SCB_ClearTxInterrupt(obj->base, tx_mask);
    }
    if (enable && rx_mask)
    {
        Cy_SCB_ClearRxInterrupt(obj->base, rx_mask);
    }
    Cy_SCB_SetTxInterruptMask(obj->base,
                              (enable ? (current_tx_mask | tx_mask) : (current_tx_mask &
                                                                       ~tx_mask)));
    Cy_SCB_SetRxInterruptMask(obj->base,
                              (enable ? (current_rx_mask | rx_mask) : (current_rx_mask &
                                                                       ~rx_mask)));

    #if defined(HAL_NEXT_TODO)
    #if defined(BCM55500)
    // The above Cy_SCB_DisableInterrupt also disconnects all the callback functions. They need to
    // be registered again
    Cy_SCB_RegisterInterruptCallback(obj->base, _mtb_hal_irq_cb[_MTB_HAL_SCB_IRQ_N[scb_arr_index]]);
    Cy_SCB_EnableInterrupt(obj->base);
    #endif
    #endif //defined(HAL_NEXT_TODO)
}


/** Process interrupts related related to a UART instance.*/
cy_rslt_t mtb_hal_uart_process_interrupt(mtb_hal_uart_t* obj)
{
    #if defined(COMPONENT_MW_ASYNC_TRANSFER)
    if (NULL != obj->async_handler)
    {
        obj->async_handler(obj, Cy_SCB_GetTxInterruptStatusMasked(
                               obj->base)|Cy_SCB_GetRxInterruptStatusMasked(obj->base));
    }
    #endif
    _mtb_hal_uart_irq_handler(obj);
    return CY_RSLT_SUCCESS;
}


#if defined(COMPONENT_MW_ASYNC_TRANSFER)
/**Configure the UART async transfer interface */
cy_rslt_t mtb_hal_uart_config_async(mtb_hal_uart_t* obj, mtb_async_transfer_context_t* context)
{
    mtb_async_transfer_interface_t interface;
    cy_rslt_t result = _mtb_hal_uart_config_async_common(obj, context, &interface);
    if (CY_RSLT_SUCCESS == result)
    {
        result = mtb_async_transfer_init(context, &interface);
        if (CY_RSLT_SUCCESS == result)
        {
            obj->async_event_callback = _mtb_hal_uart_async_transfer_event_callback;
            result = mtb_async_transfer_register_callback(context, obj->async_event_callback, obj);
        }
    }
    return result;
}


/** Note: Before calling any of the below  functions, either mtb_hal_uart_config_async or
   mtb_hal_uart_config_async_dma must have been called.*/

/** Begin asynchronous RX transfer. */
cy_rslt_t mtb_hal_uart_read_async(mtb_hal_uart_t* obj, void* rx, size_t length)
{
    CY_ASSERT(NULL != obj->async_ctx);
    return mtb_async_transfer_read(obj->async_ctx, rx, length);
}


/** Begin asynchronous TX transfer.*/
cy_rslt_t mtb_hal_uart_write_async(mtb_hal_uart_t* obj, void* tx, size_t length)
{
    CY_ASSERT(NULL != obj->async_ctx);
    return mtb_async_transfer_write(obj->async_ctx, tx, length);
}


/** Checks whether the UART interface is available to start a write transfer */
bool mtb_hal_uart_is_async_tx_available(mtb_hal_uart_t* obj)
{
    CY_ASSERT(NULL != obj->async_ctx);
    return mtb_async_transfer_available_write(obj->async_ctx);
}


/** Checks whether the UART interface is available to start a read transfer */
bool mtb_hal_uart_is_async_rx_available(mtb_hal_uart_t* obj)
{
    CY_ASSERT(NULL != obj->async_ctx);
    return mtb_async_transfer_available_read(obj->async_ctx);
}


/** Abort the ongoing read transaction.*/
cy_rslt_t mtb_hal_uart_read_abort(mtb_hal_uart_t* obj)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;

    //Clear the RX FIFO
    Cy_SCB_UART_ClearRxFifo(obj->base);
    if ((NULL != obj->async_ctx) && (!mtb_async_transfer_available_read(obj->async_ctx)))
    {
        result = mtb_async_transfer_abort_read(obj->async_ctx);
    }
    return result;
}


/** Abort the ongoing write transaction. */
cy_rslt_t mtb_hal_uart_write_abort(mtb_hal_uart_t* obj)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;

    //Clear the TX FIFO
    Cy_SCB_UART_ClearTxFifo(obj->base);
    if ((NULL != obj->async_ctx) && (!mtb_async_transfer_available_write(obj->async_ctx)))
    {
        result = mtb_async_transfer_abort_write(obj->async_ctx);
    }
    return result;
}


#endif // defined(COMPONENT_MW_ASYNC_TRANSFER)

#if defined(__cplusplus)
}
#endif

#endif /* MTB_HAL_DRIVER_AVAILABLE_UART */
