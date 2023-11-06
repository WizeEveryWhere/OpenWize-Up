/**
  * @file stm32l4xx_hal_msp.c
  * @brief This provides code for the MSP Initialization
  *
  * @details
  *
  * @copyright 2022, GRDF, Inc.  All rights reserved.
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
  * @par 1.0.0 : 2022/05/20[GBI]
  * Initial version
  *
  *
  */

/******************************************************************************/
#include "main.h"
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/

#ifdef DEBUG
	#if defined(HAL_RTC_MODULE_ENABLED)
  		__HAL_DBGMCU_FREEZE_RTC();
  	#endif
	#if defined(HAL_LPTIM_MODULE_ENABLED)
  		#if defined (LPTIM1)
	  		__HAL_DBGMCU_FREEZE_LPTIM1();
		#endif
		#if defined (LPTIM2)
	  		__HAL_DBGMCU_FREEZE_LPTIM2();
	  	#endif
	#endif
	#if defined (TIM1)
	  	__HAL_DBGMCU_FREEZE_TIM1();
	#endif
	#if defined (TIM6)
	  	__HAL_DBGMCU_FREEZE_TIM6();
	#endif
	#if defined (TIM2)
		__HAL_DBGMCU_FREEZE_TIM2();
	#endif
	#if defined (TIM3)
		__HAL_DBGMCU_FREEZE_TIM3();
	#endif
	#if defined (TIM15)
		__HAL_DBGMCU_FREEZE_TIM15();
	#endif
	#if defined(HAL_IWDG_MODULE_ENABLED)
		__HAL_DBGMCU_FREEZE_IWDG();
	#endif
	#if defined(HAL_WWDG_MODULE_ENABLED)
		__HAL_DBGMCU_FREEZE_WWDG();
	#endif
#endif

#ifdef LOWPOWER_DEBUG
	HAL_DBGMCU_EnableDBGStandbyMode();
	HAL_DBGMCU_EnableDBGStopMode();
	HAL_DBGMCU_EnableDBGSleepMode();
#endif

}

/**
* @brief RTC MSP Initialization
* This function configures the hardware resources used in this example
* @param hrtc: RTC handle pointer
* @retval None
*/
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
	if (hrtc->Instance == RTC)
	{
		__HAL_RCC_RTC_ENABLE();
		/* RTC interrupt Init */
		HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 5, 0);
		HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
		HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 5, 0);
		HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
	}
}

/**
* @brief RTC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hrtc: RTC handle pointer
* @retval None
*/
void HAL_RTC_MspDeInit(RTC_HandleTypeDef* hrtc)
{
	if (hrtc->Instance == RTC)
	{
		__HAL_RCC_RTC_DISABLE();
		/* RTC interrupt DeInit */
		HAL_NVIC_DisableIRQ(RTC_WKUP_IRQn);
		HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);
	}
}

/**
* @brief SPI MSP Initialization
* This function configures the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if (hspi->Instance == SPI1)
	{
		__HAL_RCC_SPI1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();

		GPIO_InitStruct.Pin = SPI_CLK_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
		HAL_GPIO_Init(SPI_CLK_GPIO_Port, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = SPI_MISO_Pin|SPI_MOSI_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}

/**
* @brief SPI MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
	if (hspi->Instance == SPI1)
	{
		__HAL_RCC_SPI1_CLK_DISABLE();
		HAL_GPIO_DeInit(GPIOA, SPI_CLK_Pin|SPI_MISO_Pin|SPI_MOSI_Pin);
	}
}

/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

#ifdef USE_UART4
	if (huart->Instance == UART4)
	{
		__HAL_RCC_UART4_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();

		GPIO_InitStruct.Pin = UART_TXD_Pin|UART_RXD_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* UART4 interrupt priority */
		HAL_NVIC_SetPriority(UART4_IRQn, 5, 0);
	}
#endif

#ifdef USE_LPUART1
	if (huart->Instance == LPUART1)
	{
		__HAL_RCC_LPUART1_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();

		GPIO_InitStruct.Pin = IOx0_Pin|IOx1_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* LPUART1 interrupt priority */
		HAL_NVIC_SetPriority(LPUART1_IRQn, 5, 0);
	}
#endif
}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
#ifdef USE_UART4
	if (huart->Instance == UART4)
	{
		__HAL_RCC_UART4_CLK_DISABLE();
		HAL_GPIO_DeInit(GPIOA, UART_TXD_Pin|UART_RXD_Pin);
	}
#endif

#ifdef USE_LPUART1
	if (huart->Instance == LPUART1)
	{
		__HAL_RCC_LPUART1_CLK_DISABLE();
		HAL_GPIO_DeInit(GPIOB, IOx0_Pin|IOx1_Pin);
	}
#endif
}
