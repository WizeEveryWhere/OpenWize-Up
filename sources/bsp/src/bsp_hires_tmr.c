/**
  * @file: bsp_hires_tmr.c
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
  * 1.0.0 : 2023/11/03[GBI]
  * Initial version
  *
  *
  */

/*! @addtogroup cal
 *  @ingroup bsp
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_hires_tmr.h"
#include "platform.h"
#include <stm32l4xx_hal.h>

/*******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */
inline static void Tim2_Init(register uint32_t u32ARR, register uint16_t u16PSC);
inline static void Tim2_DeInit(void);
/*!
 * @}
 * @endcond
 */
/******************************************************************************/
/**
  * @brief  This function .
  *
  * @retval HAL status
  */
int32_t BSP_HiResTmr_EnDis(uint8_t bEnable)
{
	if (bEnable)
	{
		register uint16_t u16Psc;
		// Compute the prescaler value to have counter clock equal to 1MHz
		u16Psc = (uint16_t) (( HAL_RCC_GetPCLK1Freq() / 1000000) - 1);

		Tim2_Init(0xFFFFFFFF, u16Psc);
		// Enable Timers
		TIM2->CR1 |= (TIM_CR1_CEN);
	}
	else
	{
		// Disable Timer
		Tim2_DeInit();
	}
	return 0;
}

/**
  * @brief  This function .
  *
  * @retval HAL status
  */
//inline
uint32_t BSP_HiResTmr_Cnt(void)
{
	return (uint32_t)(TIM2->CNT);
}

/**
  * @brief  This function .
  *
  * @retval HAL status
  */
//inline
uint32_t BSP_HiResTmr_Get(register uint8_t id)
{
	id--;
	//return ((uint32_t*)(&(hHiResTim.Instance->CCR1)))[id & 0x3];
	return ((uint32_t*)(&(TIM2->CCR1)))[id & 0x3];
}

/**
  * @brief  This function .
  *
  * @retval HAL status
  */
//inline
void BSP_HiResTmr_Capture(register uint8_t id)
{
	id--;
	//hHiResTim.Instance->EGR = ( 0b10 << (id & 0x3) );
	TIM2->EGR = ( 0b10 << (id & 0x3) );
}

/******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */

// Channel 1 and 2 as Input
#define	Tim2_CCMR1 0x303UL
// Channel 3 and 4 as Input
#define	Tim2_CCMR2 0x303UL
// Enable all channel
#define	Tim2_CCER 0x1111UL

inline
static void Tim2_Init(register uint32_t u32ARR, register uint16_t u16PSC)
{
	// Force Reset and Enable clock
	__HAL_RCC_TIM2_FORCE_RESET();
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_TIM2_RELEASE_RESET();
	// Initialize TIM2
	TIM2->PSC = u16PSC;
	TIM2->ARR = u32ARR;
	TIM2->CCMR1 = Tim2_CCMR1;
	TIM2->CCMR2 = Tim2_CCMR2;
	TIM2->CCER = Tim2_CCER;
	TIM2->EGR = TIM_EGR_UG;
	TIM2->SR = 0;
}

inline
static void Tim2_DeInit(void)
{
	__HAL_RCC_TIM2_FORCE_RESET();
	__HAL_RCC_TIM2_RELEASE_RESET();
	__HAL_RCC_TIM2_CLK_DISABLE();
}


//
#define	Tim2_DCR 0x303UL



/*!
 * @}
 * @endcond
 */

/******************************************************************************/

#ifdef __cplusplus
}
#endif

/*! @} */
