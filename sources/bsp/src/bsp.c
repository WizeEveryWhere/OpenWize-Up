/**
  * @file: bsp.c
  * @brief: This file contains functions to initialize the BSP.
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
  * 1.0.0 : 2020/05/13[GBI]
  * Initial version
  *
  *
  */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stm32l4xx_hal.h>
#include "bsp.h"

#include "platform.h"

//inline __attribute__((always_inline))
void msleep(uint32_t milisecond) { HAL_Delay(milisecond); }


extern UART_HandleTypeDef *paUART_BusHandle[UART_ID_MAX];

#ifdef USE_SEMIHOSTING
#warning SEMIHOSTING is defined. 1) You should exclude "syscalls.c" from build.
#warning SEMIHOSTING is defined. 2) Add "rdimon" in link.
#warning SEMIHOSTING is defined. 2) Add "-specs=rdimon.specs" to compiler CFLAGS
#warning SEMIHOSTING is defined. 3) For debugging : Select OpenOCD
#warning SEMIHOSTING is defined. 3) For debugging : add "monitor arm semihosting enable" into "Startup", "Initalization Commands"

extern void initialise_monitor_handles(void);
#else
int __io_putchar(int ch){
	uint16_t nb = 1;
	if ((uint8_t)ch == '\n'){
		nb = 2;
		((uint8_t *)&ch)[1] = '\r';
	}
	HAL_UART_Transmit(paUART_BusHandle[STDOUT_UART_ID], (uint8_t *)&ch, nb, CONSOLE_TX_TIMEOUT);

	return ch;
}

int __io_getchar(void){
	int c;
	HAL_UART_Receive(paUART_BusHandle[STDOUT_UART_ID], (uint8_t*)&c, 1, CONSOLE_RX_TIMEOUT);

	return c;
}
#endif

extern void __init_exception_handlers__(void);
extern void __init_sys_handlers__(void);
extern void __init_sys_calls__(void);

void BSP_Init(void)
{
	__init_exception_handlers__();
	__init_sys_handlers__();
	__init_sys_calls__();
	BSP_Rtc_Setup_Prescaler(RTC_PREDIV_S, RTC_PREDIV_A);
}
#ifdef __cplusplus
}
#endif
