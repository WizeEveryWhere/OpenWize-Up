/*!
  * @file: bsp_uart.h
  * @brief: //TODO:
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
  * 1.0.0 : 2020/09/30[GBI]
  * Initial version
  *
  *
  */
#ifndef _BSP_UART_H_
#define _BSP_UART_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

typedef struct
{
    uint8_t bus_id;
    uint8_t device_id;
} uart_dev_t;

typedef uart_dev_t* p_uart_dev_t;

/*******************************************************************************/
#ifndef CONSOLE_TX_TIMEOUT
#define CONSOLE_TX_TIMEOUT 2000
#endif
#ifndef CONSOLE_RX_TIMEOUT
#define CONSOLE_RX_TIMEOUT 0xFFFF
#endif

extern pfHandlerCB_t pfConsoleTXEvent;
extern pfHandlerCB_t pfConsoleRXEvent;

extern pfHandlerCB_t pfConsoleWakupEvent;

int __io_putchar(int ch);
int __io_getchar(void);

void BSP_Console_SetTXCallback (pfHandlerCB_t const pfCb);
void BSP_Console_SetRXCallback (pfHandlerCB_t const pfCb);
void BSP_Console_SetWakupCallback (pfHandlerCB_t const pfCb);

uint8_t BSP_Console_Send(uint8_t *pData, uint16_t u16Length);

/*******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif /* _BSP_UART_H_ */
