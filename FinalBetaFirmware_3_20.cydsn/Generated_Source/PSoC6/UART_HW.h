/***************************************************************************//**
* \file UART_HW.h
* \version 2.0
*
*  This file provides constants and parameter values for the UART component.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(UART_HW_CY_SCB_UART_PDL_H)
#define UART_HW_CY_SCB_UART_PDL_H

#include "cyfitter.h"
#include "scb/cy_scb_uart.h"

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
*   Initial Parameter Constants
****************************************/

#define UART_HW_DIRECTION  (3U)
#define UART_HW_ENABLE_RTS (0U)
#define UART_HW_ENABLE_CTS (0U)

/* UART direction enum */
#define UART_HW_RX    (0x1U)
#define UART_HW_TX    (0x2U)

#define UART_HW_ENABLE_RX  (0UL != (UART_HW_DIRECTION & UART_HW_RX))
#define UART_HW_ENABLE_TX  (0UL != (UART_HW_DIRECTION & UART_HW_TX))


/***************************************
*        Function Prototypes
***************************************/
/**
* \addtogroup group_general
* @{
*/
/* Component specific functions. */
void UART_HW_Start(void);

/* Basic functions */
__STATIC_INLINE cy_en_scb_uart_status_t UART_HW_Init(cy_stc_scb_uart_config_t const *config);
__STATIC_INLINE void UART_HW_DeInit(void);
__STATIC_INLINE void UART_HW_Enable(void);
__STATIC_INLINE void UART_HW_Disable(void);

/* Register callback. */
__STATIC_INLINE void UART_HW_RegisterCallback(cy_cb_scb_uart_handle_events_t callback);

/* Configuration change. */
#if (UART_HW_ENABLE_CTS)
__STATIC_INLINE void UART_HW_EnableCts(void);
__STATIC_INLINE void UART_HW_DisableCts(void);
#endif /* (UART_HW_ENABLE_CTS) */

#if (UART_HW_ENABLE_RTS)
__STATIC_INLINE void     UART_HW_SetRtsFifoLevel(uint32_t level);
__STATIC_INLINE uint32_t UART_HW_GetRtsFifoLevel(void);
#endif /* (UART_HW_ENABLE_RTS) */

__STATIC_INLINE void UART_HW_EnableSkipStart(void);
__STATIC_INLINE void UART_HW_DisableSkipStart(void);

#if (UART_HW_ENABLE_RX)
/* Low level: Receive direction. */
__STATIC_INLINE uint32_t UART_HW_Get(void);
__STATIC_INLINE uint32_t UART_HW_GetArray(void *buffer, uint32_t size);
__STATIC_INLINE void     UART_HW_GetArrayBlocking(void *buffer, uint32_t size);
__STATIC_INLINE uint32_t UART_HW_GetRxFifoStatus(void);
__STATIC_INLINE void     UART_HW_ClearRxFifoStatus(uint32_t clearMask);
__STATIC_INLINE uint32_t UART_HW_GetNumInRxFifo(void);
__STATIC_INLINE void     UART_HW_ClearRxFifo(void);
#endif /* (UART_HW_ENABLE_RX) */

#if (UART_HW_ENABLE_TX)
/* Low level: Transmit direction. */
__STATIC_INLINE uint32_t UART_HW_Put(uint32_t data);
__STATIC_INLINE uint32_t UART_HW_PutArray(void *buffer, uint32_t size);
__STATIC_INLINE void     UART_HW_PutArrayBlocking(void *buffer, uint32_t size);
__STATIC_INLINE void     UART_HW_PutString(char_t const string[]);
__STATIC_INLINE void     UART_HW_SendBreakBlocking(uint32_t breakWidth);
__STATIC_INLINE uint32_t UART_HW_GetTxFifoStatus(void);
__STATIC_INLINE void     UART_HW_ClearTxFifoStatus(uint32_t clearMask);
__STATIC_INLINE uint32_t UART_HW_GetNumInTxFifo(void);
__STATIC_INLINE bool     UART_HW_IsTxComplete(void);
__STATIC_INLINE void     UART_HW_ClearTxFifo(void);
#endif /* (UART_HW_ENABLE_TX) */

#if (UART_HW_ENABLE_RX)
/* High level: Ring buffer functions. */
__STATIC_INLINE void     UART_HW_StartRingBuffer(void *buffer, uint32_t size);
__STATIC_INLINE void     UART_HW_StopRingBuffer(void);
__STATIC_INLINE void     UART_HW_ClearRingBuffer(void);
__STATIC_INLINE uint32_t UART_HW_GetNumInRingBuffer(void);

/* High level: Receive direction functions. */
__STATIC_INLINE cy_en_scb_uart_status_t UART_HW_Receive(void *buffer, uint32_t size);
__STATIC_INLINE void     UART_HW_AbortReceive(void);
__STATIC_INLINE uint32_t UART_HW_GetReceiveStatus(void);
__STATIC_INLINE uint32_t UART_HW_GetNumReceived(void);
#endif /* (UART_HW_ENABLE_RX) */

#if (UART_HW_ENABLE_TX)
/* High level: Transmit direction functions. */
__STATIC_INLINE cy_en_scb_uart_status_t UART_HW_Transmit(void *buffer, uint32_t size);
__STATIC_INLINE void     UART_HW_AbortTransmit(void);
__STATIC_INLINE uint32_t UART_HW_GetTransmitStatus(void);
__STATIC_INLINE uint32_t UART_HW_GetNumLeftToTransmit(void);
#endif /* (UART_HW_ENABLE_TX) */

/* Interrupt handler */
__STATIC_INLINE void UART_HW_Interrupt(void);
/** @} group_general */


/***************************************
*    Variables with External Linkage
***************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t UART_HW_initVar;
extern cy_stc_scb_uart_config_t const UART_HW_config;
extern cy_stc_scb_uart_context_t UART_HW_context;
/** @} group_globals */


/***************************************
*         Preprocessor Macros
***************************************/
/**
* \addtogroup group_macros
* @{
*/
/** The pointer to the base address of the hardware */
#define UART_HW_HW     ((CySCB_Type *) UART_HW_SCB__HW)
/** @} group_macros */


/***************************************
*    In-line Function Implementation
***************************************/

/*******************************************************************************
* Function Name: UART_HW_Init
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_uart_status_t UART_HW_Init(cy_stc_scb_uart_config_t const *config)
{
   return Cy_SCB_UART_Init(UART_HW_HW, config, &UART_HW_context);
}


/*******************************************************************************
* Function Name: UART_HW_DeInit
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_DeInit(void)
{
    Cy_SCB_UART_DeInit(UART_HW_HW);
}


/*******************************************************************************
* Function Name: UART_HW_Enable
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Enable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_Enable(void)
{
    Cy_SCB_UART_Enable(UART_HW_HW);
}


/*******************************************************************************
* Function Name: UART_HW_Disable
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Disable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_Disable(void)
{
    Cy_SCB_UART_Disable(UART_HW_HW, &UART_HW_context);
}


/*******************************************************************************
* Function Name: UART_HW_RegisterCallback
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_RegisterCallback() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_RegisterCallback(cy_cb_scb_uart_handle_events_t callback)
{
    Cy_SCB_UART_RegisterCallback(UART_HW_HW, callback, &UART_HW_context);
}


#if (UART_HW_ENABLE_CTS)
/*******************************************************************************
* Function Name: UART_HW_EnableCts
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_EnableCts() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_EnableCts(void)
{
    Cy_SCB_UART_EnableCts(UART_HW_HW);
}


/*******************************************************************************
* Function Name: Cy_SCB_UART_DisableCts
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_DisableCts() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_DisableCts(void)
{
    Cy_SCB_UART_DisableCts(UART_HW_HW);
}
#endif /* (UART_HW_ENABLE_CTS) */


#if (UART_HW_ENABLE_RTS)
/*******************************************************************************
* Function Name: UART_HW_SetRtsFifoLevel
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_SetRtsFifoLevel() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_SetRtsFifoLevel(uint32_t level)
{
    Cy_SCB_UART_SetRtsFifoLevel(UART_HW_HW, level);
}


/*******************************************************************************
* Function Name: UART_HW_GetRtsFifoLevel
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetRtsFifoLevel() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_HW_GetRtsFifoLevel(void)
{
    return Cy_SCB_UART_GetRtsFifoLevel(UART_HW_HW);
}
#endif /* (UART_HW_ENABLE_RTS) */


/*******************************************************************************
* Function Name: UART_HW_EnableSkipStart
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_EnableSkipStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_EnableSkipStart(void)
{
    Cy_SCB_UART_EnableSkipStart(UART_HW_HW);
}


/*******************************************************************************
* Function Name: UART_HW_DisableSkipStart
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_DisableSkipStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_DisableSkipStart(void)
{
    Cy_SCB_UART_DisableSkipStart(UART_HW_HW);
}


#if (UART_HW_ENABLE_RX)
/*******************************************************************************
* Function Name: UART_HW_Get
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Get() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_HW_Get(void)
{
    return Cy_SCB_UART_Get(UART_HW_HW);
}


/*******************************************************************************
* Function Name: UART_HW_GetArray
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetArray() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_HW_GetArray(void *buffer, uint32_t size)
{
    return Cy_SCB_UART_GetArray(UART_HW_HW, buffer, size);
}


/*******************************************************************************
* Function Name: UART_HW_GetArrayBlocking
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetArrayBlocking() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_GetArrayBlocking(void *buffer, uint32_t size)
{
    Cy_SCB_UART_GetArrayBlocking(UART_HW_HW, buffer, size);
}


/*******************************************************************************
* Function Name: UART_HW_GetRxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetRxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_HW_GetRxFifoStatus(void)
{
    return Cy_SCB_UART_GetRxFifoStatus(UART_HW_HW);
}


/*******************************************************************************
* Function Name: UART_HW_ClearRxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_ClearRxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_ClearRxFifoStatus(uint32_t clearMask)
{
    Cy_SCB_UART_ClearRxFifoStatus(UART_HW_HW, clearMask);
}


/*******************************************************************************
* Function Name: UART_HW_GetNumInRxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetNumInRxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_HW_GetNumInRxFifo(void)
{
    return Cy_SCB_UART_GetNumInRxFifo(UART_HW_HW);
}


/*******************************************************************************
* Function Name: UART_HW_ClearRxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_ClearRxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_ClearRxFifo(void)
{
    Cy_SCB_UART_ClearRxFifo(UART_HW_HW);
}
#endif /* (UART_HW_ENABLE_RX) */


#if (UART_HW_ENABLE_TX)
/*******************************************************************************
* Function Name: UART_HW_Put
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Put() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_HW_Put(uint32_t data)
{
    return Cy_SCB_UART_Put(UART_HW_HW,data);
}


/*******************************************************************************
* Function Name: UART_HW_PutArray
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_PutArray() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_HW_PutArray(void *buffer, uint32_t size)
{
    return Cy_SCB_UART_PutArray(UART_HW_HW, buffer, size);
}


/*******************************************************************************
* Function Name: UART_HW_PutArrayBlocking
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_PutArrayBlocking() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_PutArrayBlocking(void *buffer, uint32_t size)
{
    Cy_SCB_UART_PutArrayBlocking(UART_HW_HW, buffer, size);
}


/*******************************************************************************
* Function Name: UART_HW_PutString
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_PutString() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_PutString(char_t const string[])
{
    Cy_SCB_UART_PutString(UART_HW_HW, string);
}


/*******************************************************************************
* Function Name: UART_HW_SendBreakBlocking
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_SendBreakBlocking() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_SendBreakBlocking(uint32_t breakWidth)
{
    Cy_SCB_UART_SendBreakBlocking(UART_HW_HW, breakWidth);
}


/*******************************************************************************
* Function Name: UART_HW_GetTxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetTxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_HW_GetTxFifoStatus(void)
{
    return Cy_SCB_UART_GetTxFifoStatus(UART_HW_HW);
}


/*******************************************************************************
* Function Name: UART_HW_ClearTxFifoStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_ClearTxFifoStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_ClearTxFifoStatus(uint32_t clearMask)
{
    Cy_SCB_UART_ClearTxFifoStatus(UART_HW_HW, clearMask);
}


/*******************************************************************************
* Function Name: UART_HW_GetNumInTxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetNumInTxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_HW_GetNumInTxFifo(void)
{
    return Cy_SCB_UART_GetNumInTxFifo(UART_HW_HW);
}


/*******************************************************************************
* Function Name: UART_HW_IsTxComplete
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_IsTxComplete() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE bool UART_HW_IsTxComplete(void)
{
    return Cy_SCB_UART_IsTxComplete(UART_HW_HW);
}


/*******************************************************************************
* Function Name: UART_HW_ClearTxFifo
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_ClearTxFifo() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_ClearTxFifo(void)
{
    Cy_SCB_UART_ClearTxFifo(UART_HW_HW);
}
#endif /* (UART_HW_ENABLE_TX) */


#if (UART_HW_ENABLE_RX)
/*******************************************************************************
* Function Name: UART_HW_StartRingBuffer
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_StartRingBuffer() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_StartRingBuffer(void *buffer, uint32_t size)
{
    Cy_SCB_UART_StartRingBuffer(UART_HW_HW, buffer, size, &UART_HW_context);
}


/*******************************************************************************
* Function Name: UART_HW_StopRingBuffer
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_StopRingBuffer() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_StopRingBuffer(void)
{
    Cy_SCB_UART_StopRingBuffer(UART_HW_HW, &UART_HW_context);
}


/*******************************************************************************
* Function Name: UART_HW_ClearRingBuffer
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_ClearRingBuffer() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_ClearRingBuffer(void)
{
    Cy_SCB_UART_ClearRingBuffer(UART_HW_HW, &UART_HW_context);
}


/*******************************************************************************
* Function Name: UART_HW_GetNumInRingBuffer
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetNumInRingBuffer() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_HW_GetNumInRingBuffer(void)
{
    return Cy_SCB_UART_GetNumInRingBuffer(UART_HW_HW, &UART_HW_context);
}


/*******************************************************************************
* Function Name: UART_HW_Receive
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Receive() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_uart_status_t UART_HW_Receive(void *buffer, uint32_t size)
{
    return Cy_SCB_UART_Receive(UART_HW_HW, buffer, size, &UART_HW_context);
}


/*******************************************************************************
* Function Name: UART_HW_GetReceiveStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetReceiveStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_HW_GetReceiveStatus(void)
{
    return Cy_SCB_UART_GetReceiveStatus(UART_HW_HW, &UART_HW_context);
}


/*******************************************************************************
* Function Name: UART_HW_AbortReceive
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_AbortReceive() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_AbortReceive(void)
{
    Cy_SCB_UART_AbortReceive(UART_HW_HW, &UART_HW_context);
}


/*******************************************************************************
* Function Name: UART_HW_GetNumReceived
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetNumReceived() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_HW_GetNumReceived(void)
{
    return Cy_SCB_UART_GetNumReceived(UART_HW_HW, &UART_HW_context);
}
#endif /* (UART_HW_ENABLE_RX) */


#if (UART_HW_ENABLE_TX)
/*******************************************************************************
* Function Name: UART_HW_Transmit
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Transmit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_uart_status_t UART_HW_Transmit(void *buffer, uint32_t size)
{
    return Cy_SCB_UART_Transmit(UART_HW_HW, buffer, size, &UART_HW_context);
}


/*******************************************************************************
* Function Name: UART_HW_GetTransmitStatus
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetTransmitStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_HW_GetTransmitStatus(void)
{
    return Cy_SCB_UART_GetTransmitStatus(UART_HW_HW, &UART_HW_context);
}


/*******************************************************************************
* Function Name: UART_HW_AbortTransmit
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_AbortTransmit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_AbortTransmit(void)
{
    Cy_SCB_UART_AbortTransmit(UART_HW_HW, &UART_HW_context);
}


/*******************************************************************************
* Function Name: UART_HW_GetNumLeftToTransmit
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_GetNumLeftToTransmit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t UART_HW_GetNumLeftToTransmit(void)
{
    return Cy_SCB_UART_GetNumLeftToTransmit(UART_HW_HW, &UART_HW_context);
}
#endif /* (UART_HW_ENABLE_TX) */


/*******************************************************************************
* Function Name: UART_HW_Interrupt
****************************************************************************//**
*
* Invokes the Cy_SCB_UART_Interrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void UART_HW_Interrupt(void)
{
    Cy_SCB_UART_Interrupt(UART_HW_HW, &UART_HW_context);
}

#if defined(__cplusplus)
}
#endif

#endif /* UART_HW_CY_SCB_UART_PDL_H */


/* [] END OF FILE */
