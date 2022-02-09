/**
  * @file: bsp_boot.c
  * @brief: // TODO This file ...
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
  * 1.0.0 : 2020/10/05[GBI]
  * Initial version
  *
  *
  */

#include "platform.h"
#include "bsp_boot.h"

/*
 * Boot count
 * Boot reason
 * Boot Headers
 */

uint8_t BSP_Boot_GetReason(void)
{
	register uint8_t u8BootReason = 0;
	// not yet implemented

	// boot reason
	/* Reset
	 * - Power reset
	 *   1. Brown-out reset (BOR)      => all domains (except Backup) registers are reset
	 *   2. exiting from Standby mode  => VCORE domain registers are reset
	 *   3. exiting from Shutdown mode => same as BOR
	 *
	 * - System reset (check RCC_CSR reset flags)
	 *   1. NRST (external pin)
	 *   2. WWDG reset
	 *   3. IWDG reset
	 *   4. FIREWALL reset
	 *   5. SW (software) reset (SYSRESETREQ bit into Cortex-M4 AIRCR register)
	 *   6. Low-power mode security reset
	 *      1. Entering Standby mode: this type of reset is enabled by resetting
	 *      nRST_STDBY bit in User option Bytes. In this case, whenever a
	 *      Standby mode entry sequence is successfully executed, the device is
	 *      reset instead of entering Standby mode.
	 *      2. Entering Stop mode: this type of reset is enabled by resetting
	 *      nRST_STOP bit in User option bytes. In this case, whenever a Stop
	 *      mode entry sequence is successfully executed, the device is reset
	 *      instead of entering Stop mode.
	 *      3. Entering Shutdown mode: this type of reset is enabled by
	 *      resetting nRST_SHDW bit in User option bytes. In this case, whenever
	 *      a Shutdown mode entry sequence is successfully executed, the device
	 *      is reset instead of entering Shutdown mode.
	 *   7. Option byte loader reset
	 *   8. BOR
	 *
	 * - Backup domain reset
	 *   1. Software reset, triggered by setting the BDRST bit in the Backup
	 *   domain control register (RCC_BDCR).
	 *   2. VDD or VBAT power on, if both supplies have previously been powered
	 *   off.
	 *
	 *
	 */

	// 1. NRST (external pin)
	if (RCC->CSR & RCC_CSR_PINRSTF)
	{
		// Cold reset
		u8BootReason |= COLD_RESET;
	}
	// 2. WWDG reset
	if (RCC->CSR & RCC_CSR_WWDGRSTF)
	{
		// Instability detected
		u8BootReason |= INSTAB_DETECT;
	}
	// 3. IWDG reset
	if (RCC->CSR & RCC_CSR_IWDGRSTF)
	{
		// Application Instability detected
		u8BootReason |= INSTAB_DETECT;
	}
	// 4. FIREWALL reset
	if (RCC->CSR & RCC_CSR_FWRSTF)
	{
		// Unauthorized access to protected areas
		u8BootReason |= UNAUTH_ACCESS;
	}
	// 5. SW reset
	if (RCC->CSR & RCC_CSR_SFTRSTF)
	{
		// Warm reset
		u8BootReason |= WARM_RESET;
	}
	// 6. Low-power mode security reset
	if (RCC->CSR & RCC_CSR_LPWRRSTF)
	{
		// Unauthorized low power mode
		u8BootReason |= UNAUTH_ACCESS;
	}
	// 7. Option byte loader reset
	if (RCC->CSR & RCC_CSR_OBLRSTF)
	{
		// Unauthorized option byte access
		u8BootReason |= UNAUTH_ACCESS;
	}
	// 8. BOR
	if (RCC->CSR & RCC_CSR_BORRSTF)
	{
		// TODO:
		// Power pin (power was lost)
		u8BootReason |= BOR_RESET;
	}
	// clear reset flags
	RCC->CSR |= RCC_CSR_RMVF;
	// #include "stm32l4xx_ll_rcc.h"
	// LL_RCC_ClearResetFlags();
	return u8BootReason;
}

void BSP_Boot_Reboot(uint8_t bReset)
{
	if (bReset)
	{
		SET_BIT(RCC->BDCR, RCC_BDCR_BDRST);
	}
	CLEAR_BIT(RCC->BDCR, RCC_BDCR_BDRST);
	NVIC_SystemReset();
}

/*
 * STM32L4 System Memory
 * Supported serial peripherals :
 * - USART1/USART2/USART3
 * - I2C1/I2C2/I2C3/
 * - CAN1
 * - DFU (USB device FS)
 * - SPI1/SPI2
 * Boot loader :
 * - ID : 0x92
 * - Memory location :0x1FFF6FFE
 * Boot loader (protocol) version
 * - USART (V3.1)
 * - I2C (V1.2)
 * - CAN (V2.0)
 * - DFU (V2.2)
 * - SPI (V1.1)
 *
 */


