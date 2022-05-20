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
RTC_HandleTypeDef hrtc;
SPI_HandleTypeDef hspi1;
UART_HandleTypeDef huart4;

/******************************************************************************/
void SystemClock_Config(void);
void PeriphClock_Config(void);
void LSEClock_Config(void);

/******************************************************************************/
static void MX_GPIO_Init(void);

static void MX_UART4_Init(void);
static void MX_SPI1_Init(void);

extern void app_entry(void);

/******************************************************************************/

/**
  * @brief  The main entry point.
  * @retval int
  */
int main(void)
{
#define MAX_BOOT_CNT 5
  //uint32_t u32PrevBootState;
  uint32_t u32BootCnt;
  uint32_t u32UnauthAcc;
  uint32_t u32BootState;

  hrtc.Instance = RTC;

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  SystemClock_Config();
  LSEClock_Config();
  BSP_Rtc_Setup_Clk(RCC_RTCCLKSOURCE_LSE);

  /** Enable MSI Auto calibration */ // Must be called after LSEON and LSERDY
  HAL_RCCEx_EnableMSIPLLMode();

  // Get boot state
  u32BootState = BSP_Boot_GetState();

#define MAX_BOOT_CNT 5
  // check if instability
  if(u32BootState & INSTAB_DETECT)
  {
	  // increment boot_cnt
	  u32BootCnt++;
	  if (u32BootCnt > MAX_BOOT_CNT)
	  {
	  	//swap to the previous sw slot (if any)
	    //(option) reboot
	  }
  }
  // check if unauth access
  if(u32BootState & UNAUTH_ACCESS)
  {
	  // increment unauth_cnt
	  u32UnauthAcc++;
  }

  BSP_Init(u32BootState);

  PeriphClock_Config();
  MX_GPIO_Init();

  MX_UART4_Init();
  MX_SPI1_Init();
  BSP_PwrLine_Init();

  app_entry();
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

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11; // 48 Mhz
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
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

  /** Configure the main internal regulator output voltage
  */
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

  PeriphClkInit.PeriphClockSelection =
		  RCC_PERIPHCLK_UART4|
		  RCC_PERIPHCLK_I2C1|
		  RCC_PERIPHCLK_I2C2;

  PeriphClkInit.Uart4ClockSelection   = RCC_UART4CLKSOURCE_PCLK1;
  
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;

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
  * @brief SPI1 Initialization Function
  * @retval None
  */
static void MX_SPI1_Init(void)
{
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @static
  * @brief UART4 Initialization Function
  * @retval None
  */
static void MX_UART4_Init(void)
{
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT;
  huart4.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;

  huart4.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;

  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @static
  * @brief GPIO Initialization Function
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(V_RF_EN_GPIO_Port, V_RF_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ADF7030_RST_Pin|ADF7030_SS_Pin|FE_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, FE_BYP_Pin|EEPROM_CTRL_Pin|IO1_Pin|IO6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : V_RF_EN_Pin */
  GPIO_InitStruct.Pin = V_RF_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(V_RF_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PH0 PH1 PH3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : FE_TRX_Pin IO6_Pin IO5_Pin IO4_Pin 
                           IO3_Pin PA15 */
  GPIO_InitStruct.Pin = FE_TRX_Pin|IO6_Pin|IO5_Pin|IO4_Pin 
                          |IO3_Pin|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ADF7030_RST_Pin ADF7030_SS_Pin FE_EN_Pin */
  GPIO_InitStruct.Pin = ADF7030_RST_Pin|ADF7030_SS_Pin|FE_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ADF7030_GPIO5_Pin ADF7030_GPIO4_Pin ADF7030_GPIO2_Pin ADF7030_GPIO1_Pin 
                           ADF7030_GPIO0_Pin PB3 PB4 IO2_Pin 
                           IO1_Pin */
  GPIO_InitStruct.Pin = ADF7030_GPIO5_Pin|ADF7030_GPIO4_Pin|ADF7030_GPIO2_Pin|ADF7030_GPIO1_Pin 
                          |ADF7030_GPIO0_Pin|GPIO_PIN_3|GPIO_PIN_4|IO2_Pin|IO1_Pin
						  |SCL_EXT_Pin|SDA_EXT_Pin|SDA_1_INT_Pin|SCL_1_INT_Pin ;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : ADF7030_GPIO3_Pin */
  GPIO_InitStruct.Pin = ADF7030_GPIO3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADF7030_GPIO3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : FE_BYP_Pin EEPROM_CTRL_Pin */
  GPIO_InitStruct.Pin = FE_BYP_Pin|EEPROM_CTRL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
