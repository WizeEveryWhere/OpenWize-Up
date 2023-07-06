/**
  * @file bsp_pwrlines.c
  * @brief This file contains functions to enable/disable power board peripherals
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
  * @par 1.0.0 : 2020/09/22 [GBI]
  * Initial version
  *
  *
  */

/*!
 * @addtogroup power_lines
 * @ingroup bsp
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_pwrlines.h"
#include "platform.h"
#include <stm32l4xx_hal.h>

/*!
 * @cond INTERNAL
 * @{
 */

#if defined (USE_BSP_PWRLINE_TRACE)
#ifndef TRACE_BSP_PWRLINE
#define TRACE_BSP_PWRLINE(...) fprintf (stdout, __VA_ARGS__ )
#endif
#else
#define TRACE_BSP_PWRLINE(...)
#endif

/*!
 * @}
 * @endcond
 */

/******************************************************************************/

/*!
 * @cond INTERNAL
 * @{
 */
#if defined (USE_BSP_PWRLINE_TRACE)
static const char pwr_name[MAX_NB_POWER][12] = {
	[FE_ON]      = "FE_EN",
	[PA_ON]      = "FE_BYP",
	[RF_ON]      = "RF_EN",
	[INT_EEPROM] = "INT_EEPROM",
};
#endif

extern const pwr_line_t pwr_lines[MAX_NB_POWER];

/*!
 * @}
 * @endcond
 */

/*!
  * @static
  * @brief This variable is use to keep track enabled power lines
  */
static uint16_t _pwr_lines;

/*!
  * @static
  * @brief Set the given power line
  *
  * @param [in] u16PwrLineId Power line id
  *
  */
static void _set_pwr(uint16_t u16PwrLineId)
{
	TRACE_BSP_PWRLINE("Enable %s Power\n", pwr_name[u16PwrLineId]);
	HAL_GPIO_WritePin((GPIO_TypeDef*)(pwr_lines[u16PwrLineId].u32Port), pwr_lines[u16PwrLineId].u16Pin, (GPIO_PinState)1);
}

/*!
  * @static
  * @brief Clear the given power line
  *
  * @param [in] u16PwrLineId Power line id
  *
  */
static void _clr_pwr(uint16_t u16PwrLineId)
{
	TRACE_BSP_PWRLINE("Disable %s Power\n", pwr_name[u16PwrLineId]);
	HAL_GPIO_WritePin((GPIO_TypeDef*)(pwr_lines[u16PwrLineId].u32Port), pwr_lines[u16PwrLineId].u16Pin, (GPIO_PinState)0);
}

/*!
  * @brief clear the given power line
  *
  * @param [in] u16PwrLines Power line mask
  *
  */
void BSP_PwrLine_Clr (uint16_t u16PwrLines)
{
	//uint16_t pwr_lines = _pwr_lines ^ u16PwrLines;

	if( u16PwrLines & RF_EN_MSK )
	{
		_clr_pwr(RF_ON);
		_clr_pwr(FE_ON);
	}
	if( u16PwrLines & PA_EN_MSK )
	{
		_clr_pwr(PA_ON);
	}
	if( u16PwrLines & INT_EEPROM_EN_MSK )
	{
		_clr_pwr(INT_EEPROM);
	}

	_pwr_lines &= ~u16PwrLines;
}

/*!
  * @brief Set to one the given power line
  *
  * @param [in] u16PwrLines Power line mask
  *
  */
void BSP_PwrLine_Set (uint16_t u16PwrLines)
{
	if( u16PwrLines & RF_EN_MSK )
	{
		_set_pwr(RF_ON);
		_set_pwr(FE_ON);
	}
	if( u16PwrLines & PA_EN_MSK )
	{
		_set_pwr(PA_ON);
	}
	if( u16PwrLines & INT_EEPROM_EN_MSK )
	{
		_set_pwr(INT_EEPROM);
	}
	_pwr_lines |= u16PwrLines;
}

/*!
  * @brief Get the power line state
  *
  * @param [in] u16PwrLines Power line mask
  *
  */
uint16_t BSP_PwrLine_Get (uint16_t u16PwrLines)
{
	return _pwr_lines & u16PwrLines;
}

/*!
  * @brief Initialize  (clear) the power lines
  */
void BSP_PwrLine_Init (void)
{
	_pwr_lines = 0;
	TRACE_BSP_PWRLINE("\n[BSP_PwrLine_Init]\n");
	uint8_t i;
	for (i = 0; i < MAX_NB_POWER; i++)
	{
		BSP_Gpio_OutputEnable(pwr_lines[i].u32Port, pwr_lines[i].u16Pin, 1);
	}
	BSP_PwrLine_Clr((uint16_t)0xFFFF);
}

/******************************************************************************/

#ifdef __cplusplus
}
#endif

/*! @} */
