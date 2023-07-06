/**
  * @file platform.c
  * @brief This file contains some specific platform constants and call-back..
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

#ifdef __cplusplus
extern "C" {
#endif

#include "platform.h"
#include "bsp.h"
#include <stm32l4xx_hal.h>
#include "pin_cfg.h"

/*!
 * @cond INTERNAL
 * @{
 */

/******************************************************************************/
/*******************************************************************************/
// UART related call-back handler

#if USE_UART_LOG_ID == 0
	#define USE_UART4 1
	#define UART_LOG_ID huart4
#else
	#define USE_LPUART1 1
	#define UART_LOG_ID lphuart1
#endif

#if USE_UART_COM_ID == 0
	#define USE_UART4 1
	#define UART_COM_ID huart4
#else
	#define USE_LPUART1 1
	#define UART_COM_ID lphuart1
#endif

#ifdef USE_LPUART1
UART_HandleTypeDef lphuart1 = { .Instance = LPUART1};
#endif
#ifdef USE_UART4
UART_HandleTypeDef huart4 = { .Instance = UART4};
#endif

uart_dev_t aDevUart[UART_ID_MAX] =
{
	[UART_ID_LOG] = {
			//.hHandle = &huart2,
			.hHandle = &UART_LOG_ID,
			.pfEvent = NULL,
			.u32RxTmo = LOGGER_RX_TIMEOUT,
			.u32TxTmo = LOGGER_TX_TIMEOUT
	},
	[UART_ID_COM]     = {
			.hHandle = &UART_COM_ID,
			.pfEvent = NULL,
			.u32RxTmo = CONSOLE_RX_TIMEOUT,
			.u32TxTmo = CONSOLE_TX_TIMEOUT
	},
};

static void _send_event_to_cb_(UART_HandleTypeDef *huart, uint32_t evt);

__weak void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	_send_event_to_cb_(huart, UART_EVT_TX_CPLT);
}

__weak void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	_send_event_to_cb_(huart, UART_EVT_RX_CPLT);
}

__weak void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	_send_event_to_cb_(huart, UART_EVT_RX_HCPLT);
}

__weak void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)
{
	_send_event_to_cb_(huart, UART_EVT_RX_ABT);
}

static void _send_event_to_cb_(UART_HandleTypeDef *huart, uint32_t evt)
{
	register uint8_t id;
	for (id = 0; id < UART_ID_MAX; id++)
	{
		if (aDevUart[id].hHandle == huart)
		{
			if (aDevUart[id].pfEvent != NULL)
			{
				aDevUart[id].pfEvent(aDevUart[id].pCbParam, evt);
			}
			//break;
		}
	}
}

/*******************************************************************************/
// RTC related call-back handler

RTC_HandleTypeDef hrtc = { .Instance = RTC};

pfHandlerCB_t pfWakeUpTimerEvent = NULL;
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
  UNUSED(hrtc);
  if (pfWakeUpTimerEvent)
  {
	  pfWakeUpTimerEvent();
  }
}

pfHandlerCB_t pfAlarmAEvent = NULL;
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	UNUSED(hrtc);
	if (pfAlarmAEvent)
	{
		pfAlarmAEvent();
	}
}

pfHandlerCB_t pfAlarmBEvent = NULL;
void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc)
{
	UNUSED(hrtc);
	if (pfAlarmBEvent)
	{
		pfAlarmBEvent();
	}
}

/******************************************************************************/
// STDBY and SHUTDOWN LP modes related

// This define the current wake-up pin(s) and polarity
const uint8_t u8WkupPinEn = (LP_WAKEUP_PIN2_EN | LP_WAKEUP_PIN1_EN) & AVAILABLE_WKUP_PIN;
const uint8_t u8WkupPinPoll = (LP_WAKEUP_PIN2_POL_LOW) & AVAILABLE_WKUP_PIN; // | LP_WAKEUP_PIN1_POL_LOW;

// PU or PD during LP Standby/Shutdown modes
const uint16_t u16LpPuPortA = 0 & AVAILABLE_PIN_PORTA_MSK;
const uint16_t u16LpPdPortA = 0 & AVAILABLE_PIN_PORTA_MSK;
const uint16_t u16LpPuPortB = 0 & AVAILABLE_PIN_PORTB_MSK;
const uint16_t u16LpPdPortB = 0 & AVAILABLE_PIN_PORTB_MSK;
const uint16_t u16LpPuPortC = 0 & AVAILABLE_PIN_PORTC_MSK;
const uint16_t u16LpPdPortC = 0 & AVAILABLE_PIN_PORTC_MSK;

// STOP 0, 1, 2 LP modes related
#define COM_TXD_Pin GPIO_PIN_0
#define COM_TXD_GPIO_Port GPIOA

#define COM_RXD_Pin GPIO_PIN_1
#define COM_RXD_GPIO_Port GPIOA

#ifdef COM_SWAP_PINS
	#define WKUP_PIN_NAME COM_TXD
#else
	#define WKUP_PIN_NAME COM_RXD
#endif

struct rcc_clk_state_s
{
	uint32_t ahb1_clk;
	uint32_t ahb2_clk;
	uint32_t ahb3_clk;
	uint32_t apb1r1_clk;
	uint32_t apb1r2_clk;
	uint32_t apb2_clk;
};

static struct rcc_clk_state_s _rcc_clk_state_;


void BSP_LowPower_OnStopEnter(lp_mode_e eLpMode)
{
	(void)eLpMode;
	int8_t i8LineId;
	// Disable all peripheral except SRAM1/2,

	// Save the current rcc clock state
	_rcc_clk_state_.ahb2_clk = RCC->AHB2ENR;
	_rcc_clk_state_.apb1r1_clk = RCC->APB1ENR1;
	_rcc_clk_state_.apb1r2_clk = RCC->APB1ENR2;
	_rcc_clk_state_.apb2_clk = RCC->APB2ENR;

	// Set all ETXI intended to wake-up from STOP (RTC_WKUP, RTC_ALM, PHY_IT, COM_IT)

	BSP_Gpio_InputEnable( LINE_INIT(WKUP_PIN_NAME), 1);
    BSP_GpioIt_ConfigLine( LINE_INIT(WKUP_PIN_NAME), GPIO_IRQ_FALLING_EDGE);
    BSP_GpioIt_SetCallback( LINE_INIT(WKUP_PIN_NAME), NULL, NULL );
    BSP_GpioIt_SetLine( LINE_INIT(WKUP_PIN_NAME), 1);

    // Disable all clock
	RCC->APB1ENR1 = 0;
	RCC->APB1ENR2 = 0;
	RCC->APB2ENR = 0;
	RCC->AHB2ENR = 0;
	// Disable the FLASH => require run code and remap vector in SRAM

}

void BSP_LowPower_OnStopExit(lp_mode_e eLpMode)
{
	(void)eLpMode;
	// Restore the current rcc clock state
	RCC->AHB2ENR = _rcc_clk_state_.ahb2_clk;
	RCC->APB1ENR1 = _rcc_clk_state_.apb1r1_clk;
	RCC->APB1ENR2 = _rcc_clk_state_.apb1r2_clk;
	RCC->APB2ENR = _rcc_clk_state_.apb2_clk;

	BSP_GpioIt_SetLine( LINE_INIT(WKUP_PIN_NAME), 0);
}

/*
 * Flash option register (FLASH_OPTR)
 * OB_USER_nRST_STOP
 * OB_USER_nRST_STDBY
 * OB_USER_nRST_SHDW
 *
 * OB_USER_IWDG_SW
 * OB_USER_IWDG_STOP
 * OB_USER_IWDG_STDBY
 * OB_USER_WWDG_SW
 */

/******************************************************************************/
#ifdef USE_SPI
#if defined(HAL_SPI_MODULE_ENABLED)

SPI_HandleTypeDef hspi1 = {.Instance = SPI1};

SPI_HandleTypeDef *paSPI_BusHandle[SPI_ID_MAX] =
{
	[SPI_ID_MAIN] = &hspi1,
};

spi_dev_t spi_ADF7030 =
{
	.bus_id  = SPI_ID_MAIN,
	.ss_port = GPIO_PORT(ADF7030_SS),
	.ss_pin  = GPIO_PIN(ADF7030_SS)
};

#endif
#endif

/*******************************************************************************/
#ifdef USE_I2C
#if defined(HAL_I2C_MODULE_ENABLED)

I2C_HandleTypeDef hi2c1 = {.Instance = I2C1};
I2C_HandleTypeDef hi2c2 = {.Instance = I2C2};

I2C_HandleTypeDef *paI2C_BusHandle[I2C_ID_MAX] =
{
	[I2C_ID_INT] = &hi2c1,
	[I2C_ID_EXT] = &hi2c2,
};

i2c_dev_t i2c_EEPROM =
{
	.bus_id = I2C_ID_INT,
	.device_id = EEPROM_ADDRESS,
};

#endif
#endif

/******************************************************************************/
// LPTIM related call-back handler

#ifdef USE_LPTIMER

#if defined(HAL_LPTIM_MODULE_ENABLED)

#if defined (LPTIM1)
LPTIM_HandleTypeDef hlptim1;
#endif

#if defined (LPTIM2)
LPTIM_HandleTypeDef hlptim2;
#endif

#endif

#if defined(HAL_LPTIM_MODULE_ENABLED)

#if defined (LPTIM1)
pfHandlerCB_t pfLptim1Event = NULL;
#endif

#if defined (LPTIM2)
pfHandlerCB_t pfLptim2Event = NULL;
#endif

void HAL_LPTIM_CompareMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
#if defined (LPTIM1)
	if ( (hlptim == &hlptim1) && (pfLptim1Event) )
	{
		pfLptim1Event();
	}
#endif
#if defined (LPTIM2)
	if ( (hlptim == &hlptim2) && (pfLptim2Event) )
	{
		pfLptim2Event();
	}
#endif
}
#endif
#endif

/*******************************************************************************/
// PowerLine related
#define PWR_LINE_INIT(name) GP_PORT(name), GP_PIN(name)

const pwr_line_t pwr_lines[MAX_NB_POWER] =
{
	[FE_ON]      = { PWR_LINE_INIT(FE_EN) },
	[PA_ON]      = { PWR_LINE_INIT(FE_BYP) },
	[RF_ON]      = { PWR_LINE_INIT(V_RF_EN) },
	[INT_EEPROM] = { PWR_LINE_INIT(EEPROM_CTRL) },
};

/*******************************************************************************/
/*!
 * @}
 * @endcond
 */

#ifdef __cplusplus
}
#endif

/*! @} */
