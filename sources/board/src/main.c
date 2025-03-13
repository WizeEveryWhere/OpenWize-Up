/**
  * @file main.c
  * @brief The main programm
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
#include "bsp.h"

/******************************************************************************/
/******************************************************************************/
static void MX_GPIO_Init(void);

extern void app_entry(void);

/******************************************************************************/

/**
  * @brief  The main entry point.
  * @retval int
  */
int main(void)
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
	MX_GPIO_Init();

	// Init the BSP
	BSP_Init();

#ifndef NOT_BOOTABLE // test purpose only (generate a small not bootable FW image)
	app_entry();
#endif
	while (1)
	{
	}
}

/**
  * @static
  * @brief GPIO Initialization Function
  * @retval None
  */
static void MX_GPIO_Init(void)
{
	//GPIO_InitTypeDef GPIO_InitStruct = {0};

	//------------------------------------
	// GPIO Reset values are :
	// ---
	// GPIOA_MODER    = 0xABFF FFFF
	// GPIOB_MODER    = 0xFFFF FEBF
	// GPIOC..E_MODER = 0xFFFF FFFF
	// GPIOH_MODER    = 0x0000 000F
	// ---
	// GPIOx_OTYPER   = 0x0000 0000
	// ---
	// GPIOA_OSPEEDR  = 0x0C00 0000
	// GPIOx_OSPEEDR  = 0x0000 0000
	// ---
	// GPIOA_PUPDR      = 0x6400 0000
	// GPIOB_PUPDR      = 0x0000 0100
	// GPIOC..E,H_PUPDR = 0x0000 0000
	// ---
	// GPIOx_AFRL  = 0x0000 0000
	// ---
	// GPIOx_ODR   = 0x0000 0000

	//------------------------------------
	// So :
	// PORT A :
	// - Analog as default for : FE_TRX_Pin, IO6_Pin, IO5_Pin, IO4_Pin, IO3_Pin
	// - Set by its driver for : ADF7030_RST_Pin, ADF7030_SS_Pin, SPI_CLK_Pin, SPI_MISO_Pin, SPI_MOSI_Pin
	// - Set as output         : FE_EN_Pin
	// PORT B :
	// - Analog as default for : IO2_Pin, IO1_Pin
	// - Set by its driver for : ADF7030_GPIO5_Pin, ADF7030_GPIO4_Pin, ADF7030_GPIO3_Pin, ADF7030_GPIO2_Pin, ADF7030_GPIO1_Pin, ADF7030_GPIO0_Pin
	// - Set by its driver for : SDA_1_INT_Pin, SCL_1_INT_Pin
	// - Set by its driver for : SCL_EXT_Pin, SDA_EXT_Pin
	// - Set as output         : FE_BYP_Pin, EEPROM_CTRL_Pin
	// PORT C :
	// - Set as output         : V_RF_EN_Pin

	//------------------------------------
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	//HAL_GPIO_WritePin(GPIOC, V_RF_EN_Pin, GPIO_PIN_RESET);
	/*Configure GPIO pin Output Level */
	//HAL_GPIO_WritePin(GPIOA, ADF7030_RST_Pin|ADF7030_SS_Pin|FE_EN_Pin, GPIO_PIN_RESET);
	/*Configure GPIO pin Output Level */
	//HAL_GPIO_WritePin(GPIOB, FE_BYP_Pin|EEPROM_CTRL_Pin, GPIO_PIN_RESET);

	// Done in PowerLine
	/*
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	GPIO_InitStruct.Pin = FE_EN_Pin;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = FE_BYP_Pin | EEPROM_CTRL_Pin;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = V_RF_EN_Pin;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	*/
	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0);
	HAL_NVIC_SetPriority(EXTI2_IRQn, 5, 0);
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM6) {
		HAL_IncTick();
	}
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
	for (int i = 0 ; i < 1000; i++)
	{
		if (i == 0)
		{
			fprintf(stdout, "holala\n");
		}
	}
}

#ifdef  USE_FULL_ASSERT
#include <stdio.h>
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
	/* User can add his own implementation to report the file name and line number,
	   tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line)
	*/
	for (int i = 0 ; i < 1000; i++)
	{
		if (i == 0)
		{
			fprintf(stdout, "hooo\n");
		}
	}
}
#endif /* USE_FULL_ASSERT */
