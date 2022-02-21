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
 * @addtogroup OpenWize'Up_bsp
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_pwrlines.h"
#include "platform.h"

#include "pin_cfg.h"

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

#define GP_PORT_NAME(name) name ##_GPIO_Port
#define GP_PIN_NAME(name) name##_Pin

#define GP_PORT(name) (uint32_t)(GP_PORT_NAME(name))
#define GP_PIN(name)  GP_PIN_NAME(name)
#define PWR_LINE_INIT(name) GP_PORT(name), GP_PIN(name)

/*!
 * @}
 * @endcond
 */

/******************************************************************************/
/*!
  * @brief This enum define the power line id
  */
typedef enum {
	//            9876543210
	FE_ON     , /*!< FE */
	PA_ON     , /*!< PA */
	RF_ON     , /*!< RF */
	INT_EEPROM, /*!< Internal EEPROM */
	//
	MAX_NB_POWER
} pwr_id;

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
/*!
 * @}
 * @endcond
 */

/*!
  * @brief This struct define a power line as gpio port and pin
  */
typedef struct pwr_line_s
{
	uint32_t u32Port; /*!< gpio port  */
	uint16_t u16Pin;  /*!< gpio pin */
} pwr_line_t;

/*!
  * @static
  * @brief This variable is use to keep track enabled power lines
  */
static uint16_t _pwr_lines;

/*!
  * @static
  * @brief Table that hold each power lien as gpio port and pin
  */
static const pwr_line_t pwr_lines[MAX_NB_POWER] =
{
	[FE_ON]      = { PWR_LINE_INIT(FE_EN) },
	[PA_ON]      = { PWR_LINE_INIT(FE_BYP) },
	[RF_ON]      = { PWR_LINE_INIT(PA_V_EN) },
	[INT_EEPROM] = { PWR_LINE_INIT(EEPROM_CTRL) },
};

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
	BSP_PwrLine_Clr((uint16_t)0xFFFF);
}

/******************************************************************************/

#ifdef __cplusplus
}
#endif

/*! @} */
