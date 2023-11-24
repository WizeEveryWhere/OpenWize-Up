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
#include "platform.h"

/******************************************************************************/
/******************************************************************************/
void SystemClock_Config(void);
void PeriphClock_Config(void);
void LSEClock_Config(void);

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

	SystemClock_Config();
	LSEClock_Config();
	BSP_Rtc_Setup_Clk(RCC_RTCCLKSOURCE_LSE);

	/** Enable MSI Auto calibration */ // Must be called after LSEON and LSERDY
	HAL_RCCEx_EnableMSIPLLMode();

#ifndef NOT_BOOTABLE // test purpose only (generate a small not bootable FW image)
	// Init the BSP
	BSP_Init();
	/*
	* The "PeriphClock_Config" call is not required because all "Peripherals
	* independent clock" have expected configuration at Reset.
	*/
	//PeriphClock_Config();

	MX_GPIO_Init();
	BSP_PwrLine_Init();

	BSP_Uart_Init(UART_ID_COM, '\r', UART_MODE_NONE);

	app_entry();
#endif
	while (1)
	{
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	// Initializes the CPU, AHB and APB busses clocks
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11; // 48 Mhz
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	// Initializes the CPU, AHB and APB busses clocks
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
								 |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	// Setup FLASH_LATENCY is only required when HSE or HSI is used. Auto-setup when MSI is used
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) // 48 Mhz OK
	{
		Error_Handler();
	}

	// Configure the main internal regulator output voltage
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
  * @brief Peripheral Clcok Initialization Function
  * @retval None
  */
void PeriphClock_Config(void)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/*
	* The following is not required as default. All RCC_PERIPHCLK_xxxx
	* are defined as 0x00 ("PCLK selected...") at Reset in "Peripherals
	* independent clock configuration register" (RCC_CCIPR).
	* Note that PCLK is defined as default for :
	* USART1, USART2, USART3, UART4, LPUART1, LPTIM1, LPTIM2, I2C1, I2C2, I2C3
	*
	* Nevertheless, the following can be used to change default clocks in
	* RCC_CCIPR register if required.
	*
	*/
	PeriphClkInit.PeriphClockSelection = 0
	/*
#ifdef USE_USART1
		|| RCC_PERIPHCLK_USART1
#endif
#ifdef USE_USART2
		|| RCC_PERIPHCLK_USART2
#endif
#ifdef USE_UART4
		|| RCC_PERIPHCLK_UART4
#endif
#ifdef USE_LPUART1
		|| RCC_PERIPHCLK_LPUART1
#endif
#ifdef USE_I2C
		|| RCC_PERIPHCLK_I2C1
		|| RCC_PERIPHCLK_I2C2
#endif
	*/
	;

	/*
	* The following is not required as default. All RCC_xxxxxCLKSOURCE_PCLK1
	* are defined as 0x00000000U in stm32l4xx_hal_rcc_ex.h and initialization to
	* 0x00000000U is already done with "PeriphClkInit = {0}" just before;
	*/
	/*
	PeriphClkInit.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
	PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
	PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
	PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
	PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
	*/
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
  * @brief LSE Clock Initialization Function
  * @retval None
  */
void LSEClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};

	// Enable Backup Domain access (must be set before accessing RCC_BDCR)
	HAL_PWR_EnableBkUpAccess();

	// Configure LSE Drive Capability
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH);

	// Initializes LSE Oscillator
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	// LSE is ON, so configure LSE Drive Capability
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
}

/**
  * @static
  * @brief GPIO Initialization Function
  * @retval None
  */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

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
}

#ifdef  USE_FULL_ASSERT
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */
