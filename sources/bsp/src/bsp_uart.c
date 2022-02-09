/**
  * @file: bsp_uart.c
  * @brief: This file expose public functions of uart devices.
  * 
  *****************************************************************************
  * @Copyright 2019, GRDF, Inc.  All rights reserved.
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
  *****************************************************************************
  *
  * Revision history
  * ----------------
  * 1.0.0 : 2019/12/20[BPI]
  * Initial version
  *
  *
  */

/*!
 * @ingroup Sources
 * @{
 * @ingroup Board
 * @{
 * @ingroup BSP
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_uart.h"
#include "platform.h"

extern UART_HandleTypeDef *paUART_BusHandle[UART_ID_MAX];

/*******************************************************************************/

void BSP_Console_SetTXCallback (pfHandlerCB_t const pfCb)
{
	pfConsoleTXEvent = pfCb;
}

void BSP_Console_SetRXCallback (pfHandlerCB_t const pfCb)
{
	pfConsoleRXEvent = pfCb;
}

void BSP_Console_SetWakupCallback (pfHandlerCB_t const pfCb)
{
	pfConsoleWakupEvent = pfCb;
}

/*
 * TODO:
 *
 * HAL_UARTEx_StopModeWakeUpSourceConfig(UART_HandleTypeDef *huart, UART_WakeUpTypeDef WakeUpSelection)
 * HAL_UARTEx_EnableStopMode(UART_HandleTypeDef *huart)
 * HAL_UARTEx_DisableStopMode(UART_HandleTypeDef *huart)
 */

uint8_t BSP_Console_Init(void)
{
	dev_res_e eRet = DEV_INVALID_PARAM;


	return eRet;
}

uint8_t BSP_Console_Send(uint8_t *pData, uint16_t u16Length)
{
	dev_res_e eRet = DEV_INVALID_PARAM;

	eRet = HAL_UART_Transmit(paUART_BusHandle[UART_ID_CONSOLE], pData, u16Length, CONSOLE_TX_TIMEOUT);
	//eRet = HAL_UART_Transmit_DMA(paUART_BusHandle[UART_ID_CONSOLE], pData, u16Length);
	//eRet = HAL_UART_Transmit_IT(paUART_BusHandle[UART_ID_CONSOLE], pData, u16Length);
	return eRet;
}

uint8_t BSP_Console_Received(uint8_t *pData, uint16_t u16Length)
{
	dev_res_e eRet = DEV_INVALID_PARAM;
	eRet = HAL_UART_Receive(paUART_BusHandle[UART_ID_CONSOLE], pData, u16Length, CONSOLE_RX_TIMEOUT);

	return eRet;
}

/*******************************************************************************/

#ifdef __cplusplus
}
#endif
