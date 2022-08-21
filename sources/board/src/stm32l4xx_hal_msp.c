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
	#if defined(HAL_IWDG_MODULE_ENABLED)
		__HAL_DBGMCU_FREEZE_IWDG();
	#endif
	#if defined(HAL_WWDG_MODULE_ENABLED)
		__HAL_DBGMCU_FREEZE_WWDG();
	#endif
	HAL_DBGMCU_EnableDBGStandbyMode();
	HAL_DBGMCU_EnableDBGStopMode();
	HAL_DBGMCU_EnableDBGSleepMode();
#endif

}

/**
* @brief I2C MSP Initialization
* This function configures the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hi2c->Instance==I2C1)
  {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration    
    PB7     ------> I2C1_SDA
    PB8     ------> I2C1_SCL 
    */
    GPIO_InitStruct.Pin = SDA_1_INT_Pin|SCL_1_INT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  }
  else if(hi2c->Instance==I2C2)
  {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C2 GPIO Configuration    
    PB10     ------> I2C2_SCL
    PB11     ------> I2C2_SDA 
    */
    GPIO_InitStruct.Pin = SCL_EXT_Pin|SDA_EXT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();
  }

}

/**
* @brief I2C MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  if(hi2c->Instance==I2C1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();
  
    /**I2C1 GPIO Configuration    
    PB7     ------> I2C1_SDA
    PB8     ------> I2C1_SCL 
    */
    HAL_GPIO_DeInit(GPIOB, SDA_1_INT_Pin|SCL_1_INT_Pin);
  }
  else if(hi2c->Instance==I2C2)
  {
    /* Peripheral clock disable */
    __HAL_RCC_I2C2_CLK_DISABLE();
  
    /**I2C2 GPIO Configuration    
    PB10     ------> I2C2_SCL
    PB11     ------> I2C2_SDA 
    */
    HAL_GPIO_DeInit(GPIOB, SCL_EXT_Pin|SDA_EXT_Pin);
  }

}

/**
* @brief RTC MSP Initialization
* This function configures the hardware resources used in this example
* @param hrtc: RTC handle pointer
* @retval None
*/
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
  if(hrtc->Instance==RTC)
  {
    /* Peripheral clock enable */
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
  if(hrtc->Instance==RTC)
  {
    /* Peripheral clock disable */
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
  if(hspi->Instance==SPI1)
  {
    /* Peripheral clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration    
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI 
    */
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
  if(hspi->Instance==SPI1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();
  
    /**SPI1 GPIO Configuration    
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI 
    */
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
  if(huart->Instance==UART4)
  {
    /* Peripheral clock enable */
    __HAL_RCC_UART4_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**UART4 GPIO Configuration
    PA0     ------> UART4_TX
    PA1     ------> UART4_RX
    */
    GPIO_InitStruct.Pin = UART_TXD_Pin|UART_RXD_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* UART4 interrupt Init */
    HAL_NVIC_SetPriority(UART4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
  }

  else if(huart->Instance==USART1)
  {
    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA10    ------> USART1_TX
    PA9     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = IO5_Pin|IO6_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* UART4 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }
}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==UART4)
  {
    /* Peripheral clock disable */
    __HAL_RCC_UART4_CLK_DISABLE();
  
    /**UART4 GPIO Configuration
    PA0     ------> UART4_TX
    PA1     ------> UART4_RX
    */
    HAL_GPIO_DeInit(GPIOA, UART_TXD_Pin|UART_RXD_Pin);

    /* UART4 interrupt DeInit */
    HAL_NVIC_DisableIRQ(UART4_IRQn);
  }
  else if(huart->Instance==USART1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**UART4 GPIO Configuration
    PA10    ------> USART1_TX
    PA9     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, IO5_Pin|IO6_Pin);

    /* UART4 interrupt DeInit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  }
}
