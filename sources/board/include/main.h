#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx_hal.h"

void Error_Handler(void);

// JTAG / SWD
#define NRST_Pin GPIO_PIN_4
#define NRST_GPIO_Port GPIOB

// SWD
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA

// JTAG
#define JTMS_Pin GPIO_PIN_13
#define JTMS_GPIO_Port GPIOA
#define JTCK_Pin GPIO_PIN_14
#define JTCK_GPIO_Port GPIOA
#define JTDI_Pin GPIO_PIN_15
#define JTDI_GPIO_Port GPIOA
#define JTDO_Pin GPIO_PIN_3
#define JTDO_GPIO_Port GPIOB

#ifdef OLD_SPI
// SPI

// PA5 has TIM2_CH1, TIM2_ETR, LPTIM2_ETR
#define SPI_CLK_Pin GPIO_PIN_5
#define SPI_CLK_GPIO_Port GPIOA

// PA6 has TIM1_BKIN, TIM3_CH1, TIM1_BKIN_COMP2, TIM16_CH1
#define SPI_MISO_Pin GPIO_PIN_6
#define SPI_MISO_GPIO_Port GPIOA

// PA7 has TIM1_CH1N, TIM3_CH2,
#define SPI_MOSI_Pin GPIO_PIN_7
#define SPI_MOSI_GPIO_Port GPIOA
#endif

#ifdef OLD_UART
// UART
#define UART_TXD_Pin GPIO_PIN_0
#define UART_TXD_GPIO_Port GPIOA
#define UART_RXD_Pin GPIO_PIN_1
#define UART_RXD_GPIO_Port GPIOA
#endif

#if 0
// I2C
// PB7 has LPTIM1_IN2
#define SDA_1_INT_Pin GPIO_PIN_7
#define SDA_1_INT_GPIO_Port GPIOB
// PB8 has TIM16_CH1
#define SCL_1_INT_Pin GPIO_PIN_8
#define SCL_1_INT_GPIO_Port GPIOB

// LPUART1
#define IOx1_Pin GPIO_PIN_10
#define IOx1_GPIO_Port GPIOB
#define IOx0_Pin GPIO_PIN_11
#define IOx0_GPIO_Port GPIOB
#endif

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
