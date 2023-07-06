#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx_hal.h"

void Error_Handler(void);

// SWD
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA

// SPI
#define SPI_CLK_Pin GPIO_PIN_5
#define SPI_CLK_GPIO_Port GPIOA
#define SPI_MISO_Pin GPIO_PIN_6
#define SPI_MISO_GPIO_Port GPIOA
#define SPI_MOSI_Pin GPIO_PIN_7
#define SPI_MOSI_GPIO_Port GPIOA

// UART
#define UART_TXD_Pin GPIO_PIN_0
#define UART_TXD_GPIO_Port GPIOA
#define UART_RXD_Pin GPIO_PIN_1
#define UART_RXD_GPIO_Port GPIOA

// I2C
#define SDA_1_INT_Pin GPIO_PIN_7
#define SDA_1_INT_GPIO_Port GPIOB
#define SCL_1_INT_Pin GPIO_PIN_8
#define SCL_1_INT_GPIO_Port GPIOB

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
