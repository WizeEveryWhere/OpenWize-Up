/*!
  * @file bsp_uart.h
  * @brief This file define bsp functions to access UART preipheral
  * 
  * @details
  *
  * @copyright 2019, GRDF, Inc.  All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted (subject to the limitations in the disclaimer
  * below) provided that the following conditions are met:
  *    - Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *    - Redistributions in binary form must reproduce the above copyright 
  *      notice, this list of conditions and the following disclaimer in the 
  *      documentation and/or other materials provided with the distribution.
  *    - Neither the name of GRDF, Inc. nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  *
  * @par Revision history
  *
  * @par 1.0.0 : 2020/09/30 [GBI]
  * Initial version
  *
  *
  */

/*!
 * @addtogroup uart
 * @ingroup bsp
 * @{
 */

#ifndef _BSP_UART_H_
#define _BSP_UART_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

/*!
 * @brief This enum define baud rate id
 */
typedef enum
{
	UART_BAUD_ID_1K2,   /*!< */
	UART_BAUD_ID_2K4,   /*!< */
	UART_BAUD_ID_4K8,   /*!< */
	UART_BAUD_ID_9K6,   /*!< */
	UART_BAUD_ID_19K2,  /*!< */
	UART_BAUD_ID_38K4,  /*!< */
	/* Warning : for the following 5 baud rate, with a main clk @48Mhz, the
	error on calculated vs desired is no null */
	UART_BAUD_ID_57K6,  /*!< */
	UART_BAUD_ID_115K2, /*!< */
	UART_BAUD_ID_230K4, /*!< */
	UART_BAUD_ID_460K8, /*!< */
	UART_BAUD_ID_921K6, /*!< */
	// ---
	UART_BAUD_ID_2M,    /*!< */
	UART_BAUD_ID_3M,    /*!< */
	UART_BAUD_ID_4M,    /*!< */
	// ---
	UART_BAUD_ID_NB
} uart_baud_e;

/*!
 * @brief This enum define possible events from UART
 */
typedef enum
{
	UART_EVT_NONE     = 0x00, /*!< None */
	UART_EVT_TX_CPLT  = 0x01, /*!< Transmition is complete */
	UART_EVT_RX_CPLT  = 0x02, /*!< Reception is complete */
	UART_EVT_RX_HCPLT = 0x04, /*!< */
	UART_EVT_RX_ABT   = 0x08, /*!< Timeout */
} uart_evt_e;

/*!
 * @brief This enum define flags ... from UART
 */
typedef enum
{
	UART_FLG_NONE,    /*!< */
	UART_FLG_RX_TMO,  /*!< */
	UART_FLG_RX_OVFL, /*!< */
	UART_FLG_RX_SOB,  /*!< */
	UART_FLG_RX_EOB,  /*!< */
} uart_flag_e;

/*!
 * @brief This enum define the "detection" mode for UART
 */
typedef enum
{
	UART_MODE_NONE, /*!< None (wait until the buffer reach the given size) */
	UART_MODE_EOB,  /*!< Event is sent when character match the end of block */
} uart_mode_e;

/*!
 * @brief This structure define the UART device
 */
typedef struct
{
    uint32_t dev_id;      /*!< Device Id */
    void *hHandle;        /*!< Pointer on HAL UART handle*/
    const struct iomux_s *pIomux;  /*!< Pointer on iomux pin configuration */
    const struct gpio_id_s *pGpio; /*!< Pointer on gpio pin configuration */

    void *pCbParam;       /*!< Pointer on Call-back parameter */
    pfEvtCb_t pfEvent;    /*!< Function pointer on event call-back */

   	uint32_t u32RxTmo;    /*!< Rx Time-out value (0 : disable) */
   	uint32_t u32TxTmo;    /*!< Tx Time-out value (0 : disable) */

    uint8_t u8Mode;       /*!< Current UART device mode */
    uint8_t u8CharMatch;  /*!< Character to match (if mode is enabled) */
    int8_t  i8ItLine;     /*!< Interrupt line id (NVIC) */
    uint8_t u8ItPrio;     /*!< Interrupt priority */

    uint8_t swap;
    uint32_t baud;
} uart_dev_t;

/*!
 * @brief This type define a pointer on UART device structure
 */
typedef uart_dev_t* p_uart_dev_t;

/*******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */

extern const uint32_t baud_rate[UART_BAUD_ID_NB];

/*!
 * @}
 * @endcond
 */

int __io_putchar(int ch);
int __io_getchar(void);

uint8_t BSP_Console_Init(void);
uint8_t BSP_Console_Send(uint8_t *pData, uint16_t u16Length);
uint8_t BSP_Console_Received(uint8_t *pData, uint16_t u16Length);
uint8_t BSP_Console_SetRXTmo(uint32_t u32Tmo);
uint8_t BSP_Console_SetTXTmo(uint32_t u32Tmo);
void BSP_Console_FluxRx(void);

uint8_t BSP_Uart_Open(uint8_t u8DevId);
uint8_t BSP_Uart_Close(uint8_t u8DevId);
uint8_t BSP_Uart_Init(uint8_t u8DevId, uint8_t u8CharMatch, uint8_t u8Mode);
uint8_t BSP_Uart_SetCallback (uint8_t u8DevId, pfEvtCb_t const pfEvtCb, void *pCbParam);
uint8_t BSP_Uart_Transmit(uint8_t u8DevId, uint8_t *pData, uint16_t u16Length);
uint8_t BSP_Uart_Receive(uint8_t u8DevId, uint8_t *pData, uint16_t u16Length);
uint8_t BSP_Uart_AbortReceive(uint8_t u8DevId);
uint16_t BSP_Uart_GetNbReceive(uint8_t u8DevId);
uint16_t BSP_Uart_GetNbTransmit(uint8_t u8DevId);

/*******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif /* _BSP_UART_H_ */

/*! @} */
