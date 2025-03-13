/**
  * @file pin_cfg.h
  * @brief TODO
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
  * @par 1.0.0 : 2019/11/20 [GBI]
  * Initial version
  *
  *
  */

/*!
 * @addtogroup platform
 * @ingroup bsp
 * @{
 */

#ifndef _PIN_CFG_H_
#define _PIN_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stm32l4xx_hal.h>

/*!
 * @cond INTERNAL
 * @{
 */

/**************************** GPIO pinout *************************************/

/******************************************************************************/
// Communication - SPI
#if 0
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

// PA2 has WKUP4, LSCO, TIM15_CH1, TIM2_CH3
#define FE_TRX_Pin GPIO_PIN_2
#define FE_TRX_GPIO_Port GPIOA

#if 0
/******************************************************************************/
// Communication - I2C

// PB7 has LPTIM1_IN2
#define SDA_1_INT_Pin GPIO_PIN_7
#define SDA_1_INT_GPIO_Port GPIOB
// PB8 has TIM16_CH1
#define SCL_1_INT_Pin GPIO_PIN_8
#define SCL_1_INT_GPIO_Port GPIOB

/******************************************************************************/
// Communication - UART
#define UART_TXD_Pin GPIO_PIN_0
#define UART_TXD_GPIO_Port GPIOA
#define UART_RXD_Pin GPIO_PIN_1
#define UART_RXD_GPIO_Port GPIOA
#endif
/******************************************************************************/
#if 0
// PowerLine

// PC13 has RTC_TAMP1, RTC_TS, RTC_OUT, WKUP2
#define V_RF_EN_Pin GPIO_PIN_13
#define V_RF_EN_GPIO_Port GPIOC

// PB9 has IR_OUT, SAI1_FS_A
#define EEPROM_CTRL_Pin GPIO_PIN_9
#define EEPROM_CTRL_GPIO_Port GPIOB

// PA8 has MCO, TIM1_CH1, LPTIM2_OUT, SAI1_SCK_A
#define FE_EN_Pin GPIO_PIN_8
#define FE_EN_GPIO_Port GPIOA

// PB15 has RTC_REFIN, TIM1_CH3N, TIM15_CH2, SPI2_MOSI, SAI1_SD_A
#define FE_BYP_Pin GPIO_PIN_15
#define FE_BYP_GPIO_Port GPIOB
#endif
/******************************************************************************/
#if 0
// Transceiver

// PA2 has WKUP4, LSCO, TIM15_CH1, TIM2_CH3
#define FE_TRX_Pin GPIO_PIN_2
#define FE_TRX_GPIO_Port GPIOA

// PA3 has TIM2_CH4, TIM15_CH2, SAI1_MCLK_A
#define ADF7030_RST_Pin GPIO_PIN_3
#define ADF7030_RST_GPIO_Port GPIOA

// PA4 has LPTIM2_OUT, SAI1_FS_B
#define ADF7030_SS_Pin GPIO_PIN_4
#define ADF7030_SS_GPIO_Port GPIOA

// PB0 has TIM1_CH2N, TIM3_CH3, SPI1_NSS, SAI1_EXTCLK
#define ADF7030_GPIO5_Pin GPIO_PIN_0
#define ADF7030_GPIO5_GPIO_Port GPIOB

// PB1 has TIM1_CH3N, TIM3_CH4, LPTIM2_IN1
#define ADF7030_GPIO4_Pin GPIO_PIN_1
#define ADF7030_GPIO4_GPIO_Port GPIOB

// PB2 has RTC_OUT, LPTIM1_OUT
#define ADF7030_GPIO3_Pin GPIO_PIN_2
#define ADF7030_GPIO3_GPIO_Port GPIOB

// PB12 has TIM1_BKIN, TIM15_BKIN, SPI2_NSS, SAI1_FS_A
#define ADF7030_GPIO2_Pin GPIO_PIN_12
#define ADF7030_GPIO2_GPIO_Port GPIOB

// PB13 has TIM1_CH1N, TIM15_CH1N, SPI2_SCK, SAI1_SCK_A
#define ADF7030_GPIO1_Pin GPIO_PIN_13
#define ADF7030_GPIO1_GPIO_Port GPIOB

// PB14 has TIM1_CH2N, TIM15_CH1, SPI2_MISO, SAI1_MCLK_A
#define ADF7030_GPIO0_Pin GPIO_PIN_14
#define ADF7030_GPIO0_GPIO_Port GPIOB

/******************************************************************************/

#define IO8_Pin GPIO_PIN_0
#define IO8_GPIO_Port GPIOA

#define IO7_Pin GPIO_PIN_1
#define IO7_GPIO_Port GPIOA

#define IO6_Pin GPIO_PIN_9
#define IO6_GPIO_Port GPIOA

#define IO5_Pin GPIO_PIN_10
#define IO5_GPIO_Port GPIOA

#define IO4_Pin GPIO_PIN_11
#define IO4_GPIO_Port GPIOA

#define IO3_Pin GPIO_PIN_12
#define IO3_GPIO_Port GPIOA

#define IO2_Pin GPIO_PIN_5
#define IO2_GPIO_Port GPIOB

#define IO1_Pin GPIO_PIN_6
#define IO1_GPIO_Port GPIOB

#define IOx0_Pin GPIO_PIN_11
#define IOx0_GPIO_Port GPIOB

#define IOx1_Pin GPIO_PIN_10
#define IOx1_GPIO_Port GPIOB


#define IOz4_Pin GPIO_PIN_13
#define IOz4_GPIO_Port GPIOA

#define IOz3_Pin GPIO_PIN_14
#define IOz3_GPIO_Port GPIOA

#define IOz2_Pin GPIO_PIN_15
#define IOz2_GPIO_Port GPIOA

#define IOz1_Pin GPIO_PIN_3
#define IOz1_GPIO_Port GPIOB

#define IOz0_Pin GPIO_PIN_4
#define IOz0_GPIO_Port GPIOB
#endif

/******************************************************************************/

/* IOs

These "could be" used as :
|      |      |    Function 1    | Function 2      |     Function 3    |   Function 4    |       Timers      |       Other     |

| PA0  | IO8  | UART4_TX (AF8)   | ADC1_IN5        | SAI_EXTCLK (AF13) |                 | TIM2_CH1 (AF1)    | SYS_WKUP1       |
|      |      |                  | (analog gpio)   |                   |                 | TIM2_ETR (AF14)   | RTC_TAMP2       |
|------|------|------------------|-----------------|-------------------|-----------------|-------------------|-----------------|
| PA1  | IO7  | UART4_RX (AF8)   | ADC1_IN6        |                   |                 | TIM2_CH2 (AF1)    |                 |
|      |      |                  | (analog gpio)   |                   |                 | TIM15_CH1N (AF14) |                 |
|------|------|------------------|-----------------|-------------------|-----------------|-------------------|-----------------|
| PA9  | IO6  | USART1_TX (AF7)  |                 | SAI1_FS_A (AF13)  |  I2C1_SCK (AF4) | TIM1_CH2 (AF1)    |                 |
|      |      |                  |                 |                   |                 | TIM15_BKIN (AF14) |                 |
|      |      |                  |                 |                   |                 | ?TIM15_CH1 (AF14)  |                 |
|------|------|------------------|-----------------|-------------------|-----------------|-------------------|-----------------|
| PA10 | IO5  | USART1_RX (AF7)  |                 | SAI1_SD_A (AF13)  |  I2C1_SDA (AF4) | TIM1_CH3 (AF1)    |                 |
|------|------|------------------|-----------------|-------------------|-----------------|-------------------|-----------------|
| PA11 | IO4  | USART1_CTS (AF7) | CAN1_RX (AF9)   | SPI1_MISO (AF5)   |                 | TIM1_CH4 (AF1)    |                 |
|      |      |                  |                 |                   |                 | TIM1_BKIN2 (AF12) |                 |
|------|------|------------------|-----------------|-------------------|-----------------|-------------------|-----------------|
| PA12 | IO3  | USART1_RTS (AF7) | CAN1_TX (AF9)   | SPI1_MOSI (AF5)   |                 | TIM1_ETR (AF1)    |                 |
|------|------|------------------|-----------------|-------------------|-----------------|-------------------|-----------------|
| PB5  | IO2  | USART1_CK (AF7)  | SPI3_MOSI (AF6) | SPI1_MOSI (AF5)   | I2C1_SMBA (AF4) | TIM3_CH2  (AF2)   |                 |
|      |      |                  |                 |                   |                 | TIM3_TI2FP2       |                 |
|      |      |                  |                 |                   |                 | TIM16_BKIN (AF14) |                 |
|      |      |                  |                 |                   |                 | LPTIM1_IN1 (AF1)  |                 |
|------|------|------------------|-----------------|-------------------|-----------------|-------------------|-----------------|
| PB6  | IO1  | USART1_TX (AF7)  |                 |                   |  I2C4_SCL (AF5) | TIM16_CH1N (AF14) |                 |
|      |      |                  |                 |                   |                 | LPTIM1_ETR (AF1)  |                 |
|------|------|------------------|-----------------|-------------------|-----------------|-------------------|-----------------|
| PB10 | IOx1 | USART3_TX (AF7)  | LPUART1_RX (AF8)| SAI1_SCK_A (AF13) |  I2C4_SCL (AF3) | TIM2_CH3 (AF1)    |                 |
|------|------|------------------|-----------------|-------------------|-----------------|-------------------|-----------------|
| PB11 | IOx0 | USART3_RX (AF7)  | LPUART1_TX (AF8)|                   |  I2C4_SDA (AF3) | TIM2_CH4 (AF1)    |                 |
|------|------|------------------|-----------------|-------------------|-----------------|-------------------|-----------------|
....
|------|------|------------------|-----------------|-------------------|-----------------|-------------------|-----------------|
| PA13 | IOz4 |                  |                 | SAI1_SD_B (AF13)  |                 | IR_OUT (AF1)      | SWDIO (AF0)     |
|      |      |                  |                 |                   |                 |                   | JTMS (AF0)      |
|------|------|------------------|-----------------|-------------------|-----------------|-------------------|-----------------|
| PA14 | IOz3 |                  |                 | SAI1_FS_B (AF13)  |                 | LPTIM1_OUT (AF1)  | SWCK (AF0)      |
|      |      |                  |                 |                   |                 |                   | JTCK (AF0)      |
|------|------|------------------|-----------------|-------------------|-----------------|-------------------|-----------------|
| PA15 | IOz2 |                  |  SPI3_NSS (AF6) | SPI1_NSS (AF5)    |                 | TIM2_CH1 (AF1)    | JTDI (AF0)      |
|      |      |                  |                 |                   |                 | TIM2_ETR (AF2)    |                 |
|      |      |                  |                 |                   |                 | TIM2_TI1_ED       |                 |
|------|------|------------------|-----------------|-------------------|-----------------|-------------------|-----------------|
| PB3  | IOz1 | USART1_RTS (AF7) | SPI3_SCK (AF6)  | SAI1_SCK_B (AF13) |                 | TIM2_CH2 (AF1)    | TRACE_SWO (AF0) |
|      |      |                  |                 | SPI1_SCK (AF5)    |                 |                   | JTDO (AF0)      |
|------|------|------------------|-----------------|-------------------|-----------------|-------------------|-----------------|
| PB4  | IOz0 | USART1_CTS (AF7) | SPI3_MISO (AF6) | SAI1_MCK_B (AF13) |                 | TIM3_CH1 (AF2)    | NRST (AF0)      |
|      |      |                  |                 | SPI1_MISO (AF5)   |                 | TIM2_TI1_ED       | (NJTRST)        |
|------|------|------------------|-----------------|-------------------|-----------------|-------------------|-----------------|

*/
/*
Some typical use cases :

Use case 1.1 :
Use of : UART4,
Free :
- PA9  | IO6 : TIM1_CH2, TIM15_CH1, TIM15_BKIN
- PA10 | IO5 : TIM1_CH3
- PA11 | IO4 : TIM1_CH4, TIM1_BKIN2
- PA12 | IO3 : TIM1_ETR
- PB5  | IO2 : TIM3_CH2, TIM3_TI2FP2, TIM16_BKIN, LPTIM1_IN1
- PB6  | IO1 : TIM16_CH1N, LPTIM1_ETR
- PB10 | IOx1: TIM2_CH3
- PB11 | IOx0: TIM2_CH4

Use case 1.2 :
Use of : LPUART1,
Free :
- PA0  | IO8 : TIM2_CH1
- PA1  | IO7 : TIM2_ETR
- PA9  | IO6 : TIM1_CH2, TIM15_CH1, TIM15_BKIN
- PA10 | IO5 : TIM1_CH3
- PA11 | IO4 : TIM1_CH4, TIM1_BKIN2
- PA12 | IO3 : TIM1_ETR
- PB5  | IO2 : TIM3_CH2, TIM3_TI2FP2, TIM16_BKIN, LPTIM1_IN1
- PB6  | IO1 : TIM16_CH1N, LPTIM1_ETR

Use case 2.1 :
Use of : UART4, USART1
Free :
- PA11 | IO4 : TIM1_CH4, TIM1_BKIN2
- PA12 | IO3 : TIM1_ETR
- PB6  | IO1 : TIM16_CH1N, LPTIM1_ETR
- PB5  | IO2 : TIM3_CH2, TIM3_TI2FP2, TIM16_BKIN, LPTIM1_IN1
- PB10 | IOx1 : TIM2_CH3
- PB11 | IOx0 : TIM2_CH4

Use case 2.2 :
Use of : LPUART1, UART4
Free :
- PA9  | IO6 : TIM1_CH2, TIM15_CH1, TIM15_BKIN
- PA10 | IO5 : TIM1_CH3
- PA11 | IO4 : TIM1_CH4, TIM1_BKIN2
- PA12 | IO3 : TIM1_ETR
- PB5  | IO2 : TIM3_CH2, TIM3_TI2FP2, TIM16_BKIN, LPTIM1_IN1
- PB6  | IO1 : TIM16_CH1N, LPTIM1_ETR

Use case 3.1 :
Use of : UART4, USART1, I2C4
Free :
- PA11 | IO4 : TIM1_CH4, TIM1_BKIN2
- PA12 | IO3 : TIM1_ETR
- PB6  | IO1 : TIM16_CH1N, LPTIM1_ETR
- PB5  | IO2 : TIM3_CH2, TIM3_TI2FP2, TIM16_BKIN, LPTIM1_IN1

Use case 3.2 :
Use of : LPUART1, UART4, I2C1
Free :
- PA11 | IO4 : TIM1_CH4, TIM1_BKIN2
- PA12 | IO3 : TIM1_ETR
- PB5  | IO2 : TIM3_CH2, TIM3_TI2FP2, TIM16_BKIN, LPTIM1_IN1
- PB6  | IO1 : TIM16_CH1N, LPTIM1_ETR

Use case 4.1 :
Use of : UART4, USART1, I2C4, SPI3
Free :
- PA11 | IO4 : TIM1_CH4, TIM1_BKIN2
- PA12 | IO3 : TIM1_ETR
- PB6  | IO1 : TIM16_CH1N, LPTIM1_ETR

Use case 4.2 :
Use of : UART4, USART1 + HShake, I2C4, SPI3
Free :
- PB6  | IO1 : TIM16_CH1N, LPTIM1_ETR

Use case 4.3 :
Use of : UART4, USART1 + sync, I2C4, CAN1
Free :
- PB6  | IO1 : TIM16_CH1N, LPTIM1_ETR



Base scenario :
Use case 1.1 (Use of : UART4) or Use case 1.2 (Use of : LPUART1)



*/


/*

33 available IOs
- 3  SPI (internal)
- 2  I2C (internal)
- 4  Power (internal)
- 9  Transceiver
- 10 Available
- 5  JTAG/SWDO

Form them, 15 configurable IOs
10 available
5  JTAG (under conditions)

IO is defined with (at least) 12 bits
- PU/PD     : 2 bits
- Speed     : 2 bits
- Direction / mode : 2 bits
- Out Type  : 1 bit
- AF : 4 bits

*/


// LPTIM clock source (as independent clock source)
// PCLK, LSI, HSI16, LSE

// TIM16 Input capture 1 remap (input capture 1 is connected to)
// I/O, LSI, LSE, RTC wakeup interrupt, MSI, HSE/32(2), MCO
// (2) To use this input the RTC must be enable and the HSE/32 must be selected as RTC clock source.

// TIM15 Trigger selection
// Slave TIM | ITR0 (TS = 000) | ITR1 (TS = 001)   | ITR2 (TS = 010)   | ITR3 (TS = 011)
// TIM15     | TIM1            | Reserved / (TIM3) | TIM16 OC1         | Reserved / (TIM17_OC1)

// TIM3 Trigger selection
// Slave TIM | ITR0 (TS = 000) | ITR1 (TS = 001)   | ITR2 (TS = 010)   | ITR3 (TS = 011)
// TIM3      | TIM1            | TIM2              | TIM15             | Reserved / (TIM4)

// TIM2 Trigger selection
// Slave TIM | ITR0 (TS = 000) | ITR1 (TS = 001)   | ITR2 (TS = 010)   | ITR3 (TS = 011)
// TIM2      | TIM1            | USB(1) / (TIM8)   | Reserved / (TIM3) | Reserved / (TIM4)

// TIM1 Trigger selection
// Slave TIM | ITR0 (TS = 000) | ITR1 (TS = 001)   | ITR2 (TS = 010) | ITR3 (TS = 011)
// TIM1      | TIM15           | TIM2              | NC / (TIM3)     | NC / (TIM4)

#if 0
typedef enum {
	// 3  SPI (internal)
	IO_ID_SPI_CLK,
	IO_ID_SPI_MISO,
	IO_ID_SPI_MOSI,
	// 2  I2C (internal)
	IO_ID_I2C_SCK,
	IO_ID_I2C_SDA,
	// 4  Power (internal)
	IO_ID_PWR0,
	IO_ID_PWR1,
	IO_ID_PWR2,
	IO_ID_PWR3,

	// 9  Transceiver (internal)
	IO_ID_TRV_RST,
	IO_ID_TRV_SS,
	IO_ID_TRV_TRX,

	IO_ID_TRV0,
	IO_ID_TRV1,
	IO_ID_TRV2,
	IO_ID_TRV3,
	IO_ID_TRV4,
	IO_ID_TRV5,

	// 5 under condition
	IO_ID_IOz0,
	IO_ID_IOz1,
	IO_ID_IOz2,
	IO_ID_IOz3,
	IO_ID_IOz4,

	// 10 available
	IO_ID_IOx0,
	IO_ID_IOx1,
	IO_ID_IO1,
	IO_ID_IO2,
	IO_ID_IO3,
	IO_ID_IO4,
	IO_ID_IO5,
	IO_ID_IO6,
	IO_ID_IO7,
	IO_ID_IO8,

	// ---
	IO_ID_NB
} io_id_e;

#endif

/*!
 * @}
 * @endcond
 */

#ifdef __cplusplus
}
#endif

#endif /* _PIN_CFG_H_ */

/*! @} */
