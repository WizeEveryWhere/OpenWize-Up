/*!
  * @file platform.h
  * @brief This file defines some specific platform constants..
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
  * @par 1.0.0 : 2021/09/09 [GBI]
  * Initial version
  *
  *
  */

/*!
 * @addtogroup platform
 * @ingroup bsp
 * @{
 */

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

/*!
 * @cond INTERNAL
 * @{
 */

/******************************************************************************/

/*
 * Global clock default config
 */
#ifndef OSC_SOURCE
	#define OSC_SOURCE OSC_MSI
#endif

#ifndef OSC_TRIM
	#define OSC_TRIM 1
#endif

#ifndef OSC_BYPASS
	#define OSC_BYPASS 0
#endif

#ifndef OSC_CSS
	#define OSC_CSS 0
#endif

#ifndef OSC_MSI_TRIM
	#define OSC_MSI_TRIM 0x0000
#endif

#ifndef OSC_HSI_TRIM
	#define OSC_HSI_TRIM 0x0000
#endif

#ifndef LSC_DRIVE
	#define LSC_DRIVE LSE_DRIVE_LOW
#endif

#ifndef LSC_TOGGLE
	#define LSC_TOGGLE 1
#endif

// ----------------------------------------------------------------------------
/*
 * Serial port default config
 */
#ifndef SERIAL_BAUD
	#define SERIAL_BAUD 115200
#endif

#ifndef SERIAL_BASE_CLK
	#define SERIAL_BASE_CLK UART_CLK_ID_PCLK
#endif

#ifndef SERIAL_TX_PU
	#define SERIAL_TX_PU 1
#endif

#ifndef SERIAL_RX_PU
	#define SERIAL_RX_PU 1
#endif

#ifndef SERIAL_SWAP
	#define SERIAL_SWAP 0
#endif

#ifndef SERIAL_TX_TIMEOUT
	#define SERIAL_TX_TIMEOUT 2000
#endif
#ifndef SERIAL_RX_TIMEOUT
	#define SERIAL_RX_TIMEOUT 0xFFFFFFFF
#endif

// ----------------------------------------------------------------------------
/*
 * Logger Serial port default config
 */
#ifndef LOGGER_BAUD
	#define LOGGER_BAUD 115200
#endif

#ifndef LOGGER_BASE_CLK
	#define LOGGER_BASE_CLK UART_CLK_ID_PCLK
#endif

#ifndef LOGGER_TX_PU
	#define LOGGER_TX_PU 1
#endif

#ifndef LOGGER_RX_PU
	#define LOGGER_RX_PU 1
#endif

#ifndef LOGGER_SWAP
	#define LOGGER_SWAP 0
#endif

#ifndef LOGGER_TX_TIMEOUT
	#define LOGGER_TX_TIMEOUT 2000
#endif
#ifndef LOGGER_RX_TIMEOUT
	#define LOGGER_RX_TIMEOUT 0xFFFFFFFF
#endif

#ifndef LOGGER_DEV_MAP
	#define LOGGER_DEV_MAP DEV_ID_0
#endif

// ----------------------------------------------------------------------------
/*
 * Console Serial port default config
 */
#ifndef CONSOLE_BAUD
	#define CONSOLE_BAUD 115200
#endif

#ifndef CONSOLE_BASE_CLK
	#define CONSOLE_BASE_CLK UART_CLK_ID_PCLK
#endif

#ifndef CONSOLE_TX_PU
	#define CONSOLE_TX_PU 1
#endif

#ifndef CONSOLE_RX_PU
	#define CONSOLE_RX_PU 1
#endif

#ifndef CONSOLE_SWAP
	#define CONSOLE_SWAP 0
#endif

#ifndef CONSOLE_TX_TIMEOUT
	#define CONSOLE_TX_TIMEOUT 2000
#endif
#ifndef CONSOLE_RX_TIMEOUT
	#define CONSOLE_RX_TIMEOUT 5000
#endif

#ifndef CONSOLE_DEV_MAP
	#define CONSOLE_DEV_MAP DEV_ID_0
#endif

/******************************************************************************/
/*
 * STDOUT default config
 */
#ifndef STDOUT_UART_ID
	#define STDOUT_UART_ID UART_ID_LOG
#endif
/*
 * LowPower Wake-up default config
 */
// Wize-up available wake-up pins
#define AVAILABLE_WKUP_PIN 0b00000
// Wize-up (mask to be used to setup PU/PD during STDBY and SHUTDWN)
#define AVAILABLE_PIN_PORTA_MSK 0b0001111111111111
#define AVAILABLE_PIN_PORTB_MSK 0b1111111111100111
#define AVAILABLE_PIN_PORTC_MSK 0b0011111111111111

// ------------------




/*!
 * @brief This enum define the serial id
 */
typedef enum
{
	SERIAL_ID_LOG, /*!< Logger id */
	SERIAL_ID_COM, /*!< Communication Id */
	//
	SERIAL_ID_MAX
} serial_id_e;

#ifdef USE_SPI
typedef enum
{
	SPI_ID_MAIN,
	//
	SPI_ID_MAX
} spi_id_e;
#endif

#ifdef USE_I2C

#define EEPROM_ADDRESS 0b10100000 // 0xA0

typedef enum
{
	I2C_ID_INT,
	I2C_ID_EXT,
	//
	I2C_ID_MAX
} i2c_id_e;
#endif

/*!
  * @brief This enum define the power line id
  */
typedef enum {
	//            9876543210
	FE_ON     , /*!< FE */
	PA_ON     , /*!< PA */
	RF_ON     , /*!< RF */
	INT_EEPROM, /*!< Internal EEPROM */
	//
	MAX_NB_POWER
} pwr_id_e;

/******************************************************************************/

#define EXT_REF_CLK_GPIO() \
static const struct gpio_id_s gpio_cfg_ref[1] = \
{ \
	{.port = GPIO_PORT_ID_B, .pin = GPIO_PIN_ID_14}, /* TIM15_CH1 */ \
};

#define EXT_REF_CLK_IOMUX() \
static const struct iomux_s io_cfg_ref[1] = \
{ \
	/* Ref clock to TIM15 from ADF */ \
	/* PB14 ADF7030_GPIO0 TIM15_CH1 */ \
	{ IOMUX_INIT(GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM, GPIO_MODE_AF_PP, 0, GPIO_AF14_TIM15) }, \
};

/******************************************************************************/
#define ADF7030_RST_Pin GPIO_PIN_3
#define ADF7030_RST_GPIO_Port GPIOA

#define ADF7030_SS_Pin GPIO_PIN_4
#define ADF7030_SS_GPIO_Port GPIOA

#define ADF7030_GPIO5_Pin GPIO_PIN_0
#define ADF7030_GPIO5_GPIO_Port GPIOB

#define ADF7030_GPIO4_Pin GPIO_PIN_1
#define ADF7030_GPIO4_GPIO_Port GPIOB

#define ADF7030_GPIO3_Pin GPIO_PIN_2
#define ADF7030_GPIO3_GPIO_Port GPIOB

#define ADF7030_GPIO2_Pin GPIO_PIN_12
#define ADF7030_GPIO2_GPIO_Port GPIOB

#define ADF7030_GPIO1_Pin GPIO_PIN_13
#define ADF7030_GPIO1_GPIO_Port GPIOB

#define ADF7030_GPIO0_Pin GPIO_PIN_14
#define ADF7030_GPIO0_GPIO_Port GPIOB

#define IOx0_Pin GPIO_PIN_11
#define IOx0_GPIO_Port GPIOB

#define IOx1_Pin GPIO_PIN_10
#define IOx1_GPIO_Port GPIOB

/******************************************************************************/

/*!
 * @}
 * @endcond
 */

/******************************************************************************/

#ifdef __cplusplus
}
#endif
#endif /* _PLATFORM_H_ */

/*! @} */
