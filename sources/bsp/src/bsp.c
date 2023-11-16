/**
  * @file bsp.c
  * @brief This file contains functions to initialize the BSP.
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
  * @par 1.0.0 : 2021/09/09 [GBI]
  * Initial version
  *
  *
  */

/*!
 * @addtogroup common
 * @ingroup bsp
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp.h"
#include "platform.h"
#include <stdint.h>
#include <stm32l4xx_hal.h>

/******************************************************************************/
/* Usefull  */
/******************************************************************************/
/*!
  * @brief Function convert a hexa represented as 2 bytes char (Little Endian) into hexa value (1 byte).
  *
  * @param [in] u16Char Two byte char to convert
  * 
  * @return converted one byte hex value
  */
uint8_t ascii2hex(uint16_t u16Char)
{
	register uint8_t t;
	register uint8_t hex;

	t = (uint8_t)(u16Char >> 8);
	t -= (t > 57)?(55):(48);
	hex = t << 4;

	t = (uint8_t)(u16Char & 0xFF);
	t -= (t > 57)?(55):(48);
	hex |= t & 0xF;

	return hex;
}

/*!
  * @brief Function convert a hexa value (1 byte) into its 2 bytes char representation (Little Endian).
  *
  * @param [in] u8Hex One byte hexa value to convert
  * 
  * @return converted two bytes char representation
  */
uint16_t hex2ascii(uint8_t u8Hex)
{
	uint8_t t;
	uint16_t c;

	t = ((u8Hex >> 4) & 0xF);
	t += (t > 9)?(55):(48);
	c = t << 8;

	t = (u8Hex & 0xF);
	t += (t > 9)?(55):(48);
	c |= t;

	return c;
}

/******************************************************************************/
/* Alias for HAL */
/******************************************************************************/
/*!
  * @brief Alias for HAL_Delay function
  *
  * @param [in] milisecond Number of milisecond to wait
  */
//inline __attribute__((always_inline))
void msleep(uint32_t milisecond) { HAL_Delay(milisecond); }

/*!
  * @brief Wait for (inaccurate) microsecond
  *
  * @param [in] microsecond Number of microsecond to wait
  */
void usleep(uint32_t microsecond)
{
	uint32_t cnt = microsecond * ( SystemCoreClock / 1000000);
	while(cnt) { cnt--; }
}

/*!
  * @brief Get the Unique Identifier (CPU ID)
  *
  * @return 8 bytes UID (Big endian)
  * - b[63:40] : Lot number (lower 24 bits, ASCII encoded)
  * - b[39:32] : Wafer number
  * - b[31:0]  : X and Y coordinate on the Wafer
  *
  */
uint64_t BSP_GetUid(void)
{
	uint32_t uuid[2];
	uuid[1] = HAL_GetUIDw1();
	uuid[0] = HAL_GetUIDw0();
	//uint32_t lot;
	//lot = HAL_GetUIDw2();
	return *(uint64_t*)(uuid);
}

/******************************************************************************/
/* Libc print wrapper functions */
/******************************************************************************/

/*! @cond INTERNAL @{ */

extern uart_dev_t aDevUart[UART_ID_MAX];

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
		((uint8_t *)&ch)[0] = '\r';
		((uint8_t *)&ch)[1] = '\n';
	}
	HAL_UART_Transmit(aDevUart[STDOUT_UART_ID].hHandle, (uint8_t *)&ch, nb, aDevUart[STDOUT_UART_ID].u32TxTmo);
	return ch;
}

int __io_getchar(void){
	int c;
	HAL_UART_Receive(aDevUart[STDOUT_UART_ID].hHandle, (uint8_t*)&c, 1, aDevUart[STDOUT_UART_ID].u32RxTmo);
	return c;
}
#endif

extern void __init_exception_handlers__(void);
extern void __init_sys_handlers__(void);
extern void __init_sys_calls__(void);

/*! @} @endcond */


boot_info_t gBootInfo;
boot_state_t gBootState;
/*!
  * @static
  * @brief Check the boot info
  *
  */
static
void _bsp_check_boot_info_(void)
{
	gBootInfo = (boot_info_t)BSP_Boot_GetInfo();
	gBootState = (boot_state_t)BSP_Boot_GetState();

	// Check if required to init calendar
	if ( !(gBootState.calendar) )
	{
		// Reset the SRAM2
		__HAL_SYSCFG_SRAM2_WRP_UNLOCK();
		__HAL_SYSCFG_SRAM2_ERASE();

		// Configure RTC Calendar
		//BSP_Rtc_Time_Write((time_t)EPOCH_UNIX_TO_OURS);
		BSP_Rtc_Time_Write((time_t)(1356998400U));
		gBootState.calendar = 1;
		// Save the BootState
		BSP_Boot_SetSate(gBootState.state);

		gBootInfo.unstab_cnt = 0;
		gBootInfo.unauth_cnt = 0;
	}

	// Clear the boot info but keep counters
	BSP_Boot_SetInfo(gBootInfo.info & 0xFFFF0000);

	// check wake-up internal : RTC ALARM or RTC WAKEUP TIMER
	if (gBootInfo.internal)
	{
		// Disable RTC protect
		RTC->WPR = 0xCAU;
		RTC->WPR = 0x53U;
		// case RTC WAKEUP TIMER
		if ( RTC->ISR & RTC_FLAG_WUTF )
		{
			gBootState.wkup_timer = 1;
		}
		// case RTC ALARM A // SHOULD never happen, in SHUTDOWN
		if ( RTC->ISR & RTC_FLAG_ALRAF )
		{
			gBootState.wkup_alra = 1;
		}
		// case RTC ALARM B // SHOULD never happen, in SHUTDOWN
		if ( RTC->ISR & RTC_FLAG_ALRBF )
		{
			gBootState.wkup_alrb = 1;
		}

		// Disable Timer and Alarms
		RTC->CR &= ~(RTC_CR_WUTE | RTC_CR_ALRAE | RTC_CR_ALRBE);
		// Disable Timer and Alarms interrupt
		RTC->CR &= ~(RTC_CR_WUTIE | RTC_CR_ALRAIE | RTC_CR_ALRBIE);
		// Clear flags
		RTC->ISR &= ~(RTC_ISR_WUTF | RTC_ISR_ALRAF | RTC_ISR_ALRBF);
		// Enable RTC protect
		RTC->WPR = 0xFFU;
	}

}


/*!
  * @brief This function initialize the bsp
  *
  * @return None
  */
void BSP_Init(void)
{
	__init_exception_handlers__();
	__init_sys_handlers__();
	__init_sys_calls__();

	// Setup the RTC
	//BSP_Rtc_Setup(255, 127);
	BSP_Rtc_Setup(RTC_PREDIV_S, RTC_PREDIV_A);

	// Check the boot info
	_bsp_check_boot_info_();
}
#ifdef __cplusplus
}
#endif

/*! @} */
