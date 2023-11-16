/**
  * @file bsp_boot.c
  * @brief Function to boot/reboot the platform
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
  * @par 1.0.0 : 2020/10/05 [GBI]
  * Initial version
  *
  *
  */

/*!
 *  @addtogroup boot
 *  @ingroup bsp
 *  @{
 */

#include "bsp_boot.h"
#include "platform.h"
#include <stm32l4xx_hal.h>

/*!
 * @cond INTERNAL
 * @{
 */

/*!
 * @}
 * @endcond
 */

/*!
  * @brief Reboot
  *
  * @param [in] bReset Reset the backup domain (1 ;yes, 0: no)
  *
  * @return None
  *
  */
void BSP_Boot_Reboot(uint8_t bReset)
{
	if (bReset)
	{
		__HAL_RCC_BACKUPRESET_FORCE();
	}
	__HAL_RCC_BACKUPRESET_RELEASE();
	NVIC_SystemReset();
}

/*!
  * @brief Get the boot info
  *
  * @return the boot into structure (see @link boot_info_t @endlink)
  *
  */
uint32_t BSP_Boot_GetInfo(void)
{
	return RTC->BOOT_INFO_BKPR;
}

/*!
  * @brief Set the boot info
  *
  * @param [in] sBootInfo The boot info to set (see @link boot_info_t @endlink)
  *
  */
void BSP_Boot_SetInfo(uint32_t info)
{
	// Store info
	RTC->BOOT_INFO_BKPR = info;
}

/*!
  * @brief Get the boot info
  *
  * @return the boot into structure (see @link boot_info_t @endlink)
  *
  */
uint32_t BSP_Boot_GetState(void)
{
	return RTC->BOOT_STATE_BKPR;
}

/*!
  * @brief Set the boot info
  *
  * @param [in] sBootInfo The boot info to set (see @link boot_info_t @endlink)
  *
  */
void BSP_Boot_SetSate(uint32_t state)
{
	// Store info
	RTC->BOOT_STATE_BKPR = state;
}

/*! @} */
