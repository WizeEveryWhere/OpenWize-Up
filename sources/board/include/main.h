/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define V_RF_EN_Pin GPIO_PIN_13
#define V_RF_EN_GPIO_Port GPIOC
#define UART_TXD_Pin GPIO_PIN_0
#define UART_TXD_GPIO_Port GPIOA
#define UART_RXD_Pin GPIO_PIN_1
#define UART_RXD_GPIO_Port GPIOA
#define FE_TRX_Pin GPIO_PIN_2
#define FE_TRX_GPIO_Port GPIOA
#define ADF7030_RST_Pin GPIO_PIN_3
#define ADF7030_RST_GPIO_Port GPIOA
#define ADF7030_SS_Pin GPIO_PIN_4
#define ADF7030_SS_GPIO_Port GPIOA
#define SPI_CLK_Pin GPIO_PIN_5
#define SPI_CLK_GPIO_Port GPIOA
#define SPI_MISO_Pin GPIO_PIN_6
#define SPI_MISO_GPIO_Port GPIOA
#define SPI_MOSI_Pin GPIO_PIN_7
#define SPI_MOSI_GPIO_Port GPIOA
#define ADF7030_GPIO5_Pin GPIO_PIN_0
#define ADF7030_GPIO5_GPIO_Port GPIOB
#define ADF7030_GPIO4_Pin GPIO_PIN_1
#define ADF7030_GPIO4_GPIO_Port GPIOB
#define ADF7030_GPIO3_Pin GPIO_PIN_2
#define ADF7030_GPIO3_GPIO_Port GPIOB
#define ADF7030_GPIO3_EXTI_IRQn EXTI2_IRQn
#define SCL_EXT_Pin GPIO_PIN_10
#define SCL_EXT_GPIO_Port GPIOB
#define SDA_EXT_Pin GPIO_PIN_11
#define SDA_EXT_GPIO_Port GPIOB
#define ADF7030_GPIO2_Pin GPIO_PIN_12
#define ADF7030_GPIO2_GPIO_Port GPIOB
#define ADF7030_GPIO1_Pin GPIO_PIN_13
#define ADF7030_GPIO1_GPIO_Port GPIOB
#define ADF7030_GPIO0_Pin GPIO_PIN_14
#define ADF7030_GPIO0_GPIO_Port GPIOB
#define FE_BYP_Pin GPIO_PIN_15
#define FE_BYP_GPIO_Port GPIOB
#define FE_EN_Pin GPIO_PIN_8
#define FE_EN_GPIO_Port GPIOA
#define IO6_Pin GPIO_PIN_9
#define IO6_GPIO_Port GPIOA
#define IO5_Pin GPIO_PIN_10
#define IO5_GPIO_Port GPIOA
#define IO4_Pin GPIO_PIN_11
#define IO4_GPIO_Port GPIOA
#define IO3_Pin GPIO_PIN_12
#define IO3_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define IO2_Pin GPIO_PIN_5
#define IO2_GPIO_Port GPIOB
#define IO1_Pin GPIO_PIN_6
#define IO1_GPIO_Port GPIOB
#define SDA_1_INT_Pin GPIO_PIN_7
#define SDA_1_INT_GPIO_Port GPIOB
#define SCL_1_INT_Pin GPIO_PIN_8
#define SCL_1_INT_GPIO_Port GPIOB
#define EEPROM_CTRL_Pin GPIO_PIN_9
#define EEPROM_CTRL_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
