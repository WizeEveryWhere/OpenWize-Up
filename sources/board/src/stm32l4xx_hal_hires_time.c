/**
  * @file stm32l4xx_hal_hires_time.c
  * @brief // TODO This file ...
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
  * @par 1.0.0 : 2023/04/09 [TODO: your name]
  * Initial version
  *
  */

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
/******************************************************************************/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_tim.h"

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/**
  * @brief  This function .
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_HiEesTime_Init(void)
{
	RCC_ClkInitTypeDef    clkconfig;
	uint32_t              uwTimclock = 0;
	uint32_t              uwPrescalerValue = 0;
	uint32_t              pFLatency;

	// Get clock configuration
	HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);
	// Compute TIM clock
	uwTimclock = HAL_RCC_GetPCLK1Freq();
	// Compute the prescaler value to have counter clock equal to 1MHz
	uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000) - 1);

	// Reset and Enable clock
	__HAL_RCC_TIM2_FORCE_RESET();
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_TIM2_RELEASE_RESET();
	// Initialize TIM2
	htim2.Instance = TIM2;

	// Set the Prescaler value
	htim2.Instance->PSC = uwPrescalerValue;
	htim2.Instance->EGR = TIM_EGR_UG;

	// Configure channels as IC
	htim2.Instance->CCMR1 = (uint32_t)0x303;
	htim2.Instance->CCMR2 = (uint32_t)0x303;
	// Enable channels as IC
	htim2.Instance->CCER = 0x1111;
	// Start TIM2
	__HAL_TIM_ENABLE(&htim2);
	/* Return function status */
	return HAL_OK;

}

/**
  * @brief  This function .
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_HiResTime_DeInit(void)
{
	__HAL_RCC_TIM2_CLK_DISABLE();
	//__HAL_RCC_TIM3_CLK_DISABLE();

	return HAL_OK;
}

/**
  * @brief  This function .
  *
  * @retval HAL status
  */
int32_t HiResTime_EnDis(uint8_t bEnable)
{
	if (bEnable)
	{
		if( HAL_HiEesTime_Init())
		{
			return -1;
		}
	}
	else
	{
		HAL_HiResTime_DeInit();
	}
	return 0;
}

/**
  * @brief  This function .
  *
  * @retval HAL status
  */
//inline
uint32_t HiResTime_Get(register uint8_t id)
{
	//return HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1);
	id--;
	return ((uint32_t*)(&(htim2.Instance->CCR1)))[id & 0x3];
}

/**
  * @brief  This function .
  *
  * @retval HAL status
  */
//inline
void HiResTime_Capture(register uint8_t id)
{
	//HAL_TIM_GenerateEvent(&htim2, TIM_EVENTSOURCE_CC3);
	id--;
	htim2.Instance->EGR = ( 0b10 << (id & 0x3) );
}

#ifdef __cplusplus
}
#endif
