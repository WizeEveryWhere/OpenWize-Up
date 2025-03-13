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
//#include "pin_cfg.h"

#include <string.h>

#ifndef PERM_SECTION
#define PERM_SECTION(psection) __attribute__(( section(psection) )) __attribute__((used))
#endif

/*!
 * @cond INTERNAL
 * @{
 */
/******************************************************************************/
uint8_t BSP_Serial_Bind(uint8_t u8SerialId, uint32_t u32DevId);
void Serial_IRQHandler(uint8_t id);

uint8_t BSP_SpiBus_Bind(void);

void BSP_LowPower_SaveRccClock(void);
void BSP_LowPower_RestoreRccClock(void);
void BSP_LowPower_DisableRccClock(void);
void BSP_LowPower_SetWakeUp(struct gpio_id_s wake_up_gpio);
void BSP_LowPower_OnStopEnter(lp_mode_e eLpMode);
void BSP_LowPower_OnStopExit(lp_mode_e eLpMode);

/******************************************************************************/
// IO related

struct iocfg_s {
	struct iomux_s iomux;
	struct gpio_id_s gpio_id;
};

typedef enum {
	PIN_ROLE_TX,
	PIN_ROLE_RX,
	PIN_ROLE_CK,
	PIN_ROLE_CTS = PIN_ROLE_CK,
	PIN_ROLE_RTS,
} pin_role_e;

/* -------------------------------------------------------------------------- */

static const struct gpio_id_s gpio_uart[4][2] =
{
	[0] = {// UART4
		[PIN_ROLE_TX] = {.port = GPIO_PORT_ID_A, .pin = GPIO_PIN_ID_0}, // UART4_TX
		[PIN_ROLE_RX] = {.port = GPIO_PORT_ID_A, .pin = GPIO_PIN_ID_1}, // UART4_RX
	},
	[1] = {// LPUART1
		[PIN_ROLE_TX] = {.port = GPIO_PORT_ID_B, .pin = GPIO_PIN_ID_11}, // LPUART1_TX
		[PIN_ROLE_RX] = {.port = GPIO_PORT_ID_B, .pin = GPIO_PIN_ID_10}, // LPUART1_RX
	},
	[2] = {// USART1
		[PIN_ROLE_TX] = {.port = GPIO_PORT_ID_A, .pin = GPIO_PIN_ID_9},  // USART1_TX
		[PIN_ROLE_RX] = {.port = GPIO_PORT_ID_A, .pin = GPIO_PIN_ID_10}, // USART1_RX
	},
	[3] = {// USART3
		[PIN_ROLE_TX] = {.port = GPIO_PORT_ID_B, .pin = GPIO_PIN_ID_10}, // USART3_TX
		[PIN_ROLE_RX] = {.port = GPIO_PORT_ID_B, .pin = GPIO_PIN_ID_11}, // USART3_RX
	},
};

static const struct iomux_s io_cfg_uart[4][2] =
{
	[0] = {// UART4
		{ IOMUX_INIT(GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM, GPIO_MODE_AF_PP, 0, GPIO_AF8_UART4) }, // PA0 IO8 : UART4_TX
		{ IOMUX_INIT(GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM, GPIO_MODE_AF_PP, 0, GPIO_AF8_UART4) }, // PA1 IO7 : UART4_RX
	},
	[1] = {// LPUART1
		{ IOMUX_INIT(GPIO_PULLUP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_MODE_AF_PP, 0, GPIO_AF8_LPUART1) }, // PB11 IOx0 : LPUART1_TX
		{ IOMUX_INIT(GPIO_PULLUP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_MODE_AF_PP, 0, GPIO_AF8_LPUART1) }, // PB10 IOx1 : LPUART1_RX
	},
	[2] = {// USART1
		{ IOMUX_INIT(GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM, GPIO_MODE_AF_PP, 0, GPIO_AF7_USART1) }, // PA9 IO6  : USART1_TX
		{ IOMUX_INIT(GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM, GPIO_MODE_AF_PP, 0, GPIO_AF7_USART1) }, // PA10 IO5 : USART1_RX
	},
	[3] = {// USART3
		{ IOMUX_INIT(GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM, GPIO_MODE_AF_PP, 0, GPIO_AF7_USART3) }, // PB10 IOx1 : USART3_TX
		{ IOMUX_INIT(GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM, GPIO_MODE_AF_PP, 0, GPIO_AF7_USART3) }, // PB11 IOx0 : USART3_RX
	},
};

/* -------------------------------------------------------------------------- */

#ifdef USE_SPI
static const struct gpio_id_s gpio_spi[SPI_ID_MAX][3] =
{
	[SPI_ID_MAIN] = {// SPI1
		{.port = GPIO_PORT_ID_A, .pin = GPIO_PIN_ID_5},  // SPI1_SCK
		{.port = GPIO_PORT_ID_A, .pin = GPIO_PIN_ID_6},  // SPI1_MISO
		{.port = GPIO_PORT_ID_A, .pin = GPIO_PIN_ID_7},  // SPI1_MOSI
	},
};

static const struct iomux_s io_cfg_spi[SPI_ID_MAX][3] =
{
	[SPI_ID_MAIN] = {// SPI1
		{ IOMUX_INIT(GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_AF_PP, 0, GPIO_AF5_SPI1) }, // PA5 SPI1_SCK
		{ IOMUX_INIT(GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_AF_PP, 0, GPIO_AF5_SPI1) }, // PA6 SPI1_MISO
		{ IOMUX_INIT(GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_AF_PP, 0, GPIO_AF5_SPI1) }, // PA7 SPI1_MOSI
	},
};
#endif

/* -------------------------------------------------------------------------- */
#ifdef USE_I2C

#endif

/*******************************************************************************/
// UART related

//__attribute__((noinit)) UART_HandleTypeDef huart[4];
//__attribute__((noinit)) uart_dev_t aDevUart[SERIAL_ID_MAX];

PERM_SECTION(".noinit") UART_HandleTypeDef huart[4];
PERM_SECTION(".noinit") uart_dev_t aDevUart[SERIAL_ID_MAX];

#define DEV_0_CFG_HANDLE &huart[DEV_ID_0]
#define DEV_0_CFG_INSTANCE UART4
#define DEV_0_CFG_IOMUX io_cfg_uart[DEV_ID_0]
#define DEV_0_CFG_GPIO  gpio_uart[DEV_ID_0]
#define DEV_0_CFG_IT_LINE UART4_IRQn
#define DEV_0_CFG_IT_PRIO 5
#define DEV_0_CFG_GPIO_NB sizeof(DEV_0_CFG_GPIO)

#define DEV_1_CFG_HANDLE &huart[DEV_ID_1]
#define DEV_1_CFG_INSTANCE LPUART1
#define DEV_1_CFG_IOMUX io_cfg_uart[DEV_ID_1]
#define DEV_1_CFG_GPIO  gpio_uart[DEV_ID_1]
#define DEV_1_CFG_IT_LINE LPUART1_IRQn
#define DEV_1_CFG_IT_PRIO 5
#define DEV_1_CFG_GPIO_NB sizeof(DEV_1_CFG_GPIO)

#define DEV_2_CFG_HANDLE &huart[DEV_ID_2]
#define DEV_2_CFG_INSTANCE USART1
#define DEV_2_CFG_IOMUX io_cfg_uart[DEV_ID_2]
#define DEV_2_CFG_GPIO  gpio_uart[DEV_ID_2]
#define DEV_2_CFG_IT_LINE USART1_IRQn
#define DEV_2_CFG_IT_PRIO 5
#define DEV_2_CFG_GPIO_NB sizeof(DEV_2_CFG_GPIO)

#define DEV_3_CFG_HANDLE &huart[DEV_ID_3]
#define DEV_3_CFG_INSTANCE USART3
#define DEV_3_CFG_IOMUX io_cfg_uart[DEV_ID_3]
#define DEV_3_CFG_GPIO  gpio_uart[DEV_ID_3]
#define DEV_3_CFG_IT_LINE USART3_IRQn
#define DEV_3_CFG_IT_PRIO 5
#define DEV_3_CFG_GPIO_NB sizeof(DEV_3_CFG_GPIO)


#define DEV_x3_CFG_HANDLE &huart[DEV_ID_2]
#define DEV_x3_CFG_INSTANCE USART1
#define DEV_x3_CFG_IOMUX io_cfg_usart1_hwflow
#define DEV_x3_CFG_GPIO  gpio_io3_io4_io5_io6
#define DEV_x3_CFG_IT_LINE USART1_IRQn
#define DEV_x3_CFG_IT_PRIO 5
#define DEV_x3_CFG_GPIO_NB sizeof(DEV_x3_CFG_GPIO)

#define DEV_x4_CFG_HANDLE &huart[DEV_ID_2]
#define DEV_x4_CFG_INSTANCE USART1
#define DEV_x4_CFG_IOMUX io_cfg_usart1_sync
#define DEV_x4_CFG_GPIO  gpio_io2_io5_io6
#define DEV_x4_CFG_IT_LINE USART1_IRQn
#define DEV_x4_CFG_IT_PRIO 5
#define DEV_x4_CFG_GPIO_NB sizeof(DEV_x4_CFG_GPIO)

#define XSTR(x) STR(x)
#define STR(x) #x


uint8_t BSP_Serial_Bind(uint8_t u8SerialId, uint32_t u32DevId)
{
	if (u8SerialId >= SERIAL_ID_MAX)
	{
		return DEV_INVALID_PARAM;
	}
	if (u32DevId >= DEV_ID_MAX)
	{
		return DEV_INVALID_PARAM;
	}

	memset(&aDevUart[u8SerialId], 0, sizeof(uart_dev_t));

	if (u8SerialId == SERIAL_ID_LOG)
	{
		aDevUart[u8SerialId].u32RxTmo = LOGGER_RX_TIMEOUT;
		aDevUart[u8SerialId].u32TxTmo = LOGGER_TX_TIMEOUT;
		aDevUart[u8SerialId].swap     = LOGGER_SWAP;
		aDevUart[u8SerialId].baud     = LOGGER_BAUD;
	}
	else if (u8SerialId == SERIAL_ID_COM)
	{
		aDevUart[u8SerialId].u32RxTmo = CONSOLE_RX_TIMEOUT;
		aDevUart[u8SerialId].u32TxTmo = CONSOLE_TX_TIMEOUT;
		aDevUart[u8SerialId].swap     = CONSOLE_SWAP;
		aDevUart[u8SerialId].baud     = CONSOLE_BAUD;
	}
	else
	{
		aDevUart[u8SerialId].u32RxTmo = SERIAL_TX_TIMEOUT;
		aDevUart[u8SerialId].u32TxTmo = SERIAL_RX_TIMEOUT;
		aDevUart[u8SerialId].swap     = SERIAL_SWAP;
		aDevUart[u8SerialId].baud     = SERIAL_BAUD;
	}

	if (u32DevId == DEV_ID_0)
	{
		aDevUart[u8SerialId].dev_id   = DEV_ID_0;
		aDevUart[u8SerialId].hHandle  = DEV_0_CFG_HANDLE;
		aDevUart[u8SerialId].i8ItLine = DEV_0_CFG_IT_LINE,
		aDevUart[u8SerialId].u8ItPrio = DEV_0_CFG_IT_PRIO,
		aDevUart[u8SerialId].pIomux   = DEV_0_CFG_IOMUX;
		aDevUart[u8SerialId].pGpio    = DEV_0_CFG_GPIO;
		((UART_HandleTypeDef*)(aDevUart[u8SerialId].hHandle))->Instance = DEV_0_CFG_INSTANCE;
	}
	else if (u32DevId == DEV_ID_1)
	{
		aDevUart[u8SerialId].dev_id   = DEV_ID_1;
		aDevUart[u8SerialId].hHandle  = DEV_1_CFG_HANDLE;
		aDevUart[u8SerialId].i8ItLine = DEV_1_CFG_IT_LINE,
		aDevUart[u8SerialId].u8ItPrio = DEV_1_CFG_IT_PRIO,
		aDevUart[u8SerialId].pIomux   = DEV_1_CFG_IOMUX;
		aDevUart[u8SerialId].pGpio    = DEV_1_CFG_GPIO;
		((UART_HandleTypeDef*)(aDevUart[u8SerialId].hHandle))->Instance = DEV_1_CFG_INSTANCE;
	}
	else if (u32DevId == DEV_ID_2)
	{
		aDevUart[u8SerialId].dev_id   = DEV_ID_2;
		aDevUart[u8SerialId].hHandle  = DEV_2_CFG_HANDLE;
		aDevUart[u8SerialId].i8ItLine = DEV_2_CFG_IT_LINE,
		aDevUart[u8SerialId].u8ItPrio = DEV_2_CFG_IT_PRIO,
		aDevUart[u8SerialId].pIomux   = DEV_2_CFG_IOMUX;
		aDevUart[u8SerialId].pGpio    = DEV_2_CFG_GPIO;
		((UART_HandleTypeDef*)(aDevUart[u8SerialId].hHandle))->Instance = DEV_2_CFG_INSTANCE;
	}
	else if (u32DevId == DEV_ID_3)
	{
		aDevUart[u8SerialId].dev_id   = DEV_ID_3;
		aDevUart[u8SerialId].hHandle  = DEV_3_CFG_HANDLE;
		aDevUart[u8SerialId].i8ItLine = DEV_3_CFG_IT_LINE,
		aDevUart[u8SerialId].u8ItPrio = DEV_3_CFG_IT_PRIO,
		aDevUart[u8SerialId].pIomux   = DEV_3_CFG_IOMUX;
		aDevUart[u8SerialId].pGpio    = DEV_3_CFG_GPIO;
		((UART_HandleTypeDef*)(aDevUart[u8SerialId].hHandle))->Instance = DEV_3_CFG_INSTANCE;
	}
	else
	{
		return DEV_INVALID_PARAM;
	}

	// Set the Wake-up pin
	if (u8SerialId == SERIAL_ID_COM)
	{
		uint8_t pin = (aDevUart[u8SerialId].swap)?(PIN_ROLE_TX):(PIN_ROLE_RX);
		BSP_LowPower_SetWakeUp(aDevUart[u8SerialId].pGpio[pin]);
	}

	return DEV_SUCCESS;
}

void Serial_IRQHandler(uint8_t id)
{
	if ( huart[id].Instance->ISR & USART_ISR_RTOF)
	{
		huart[id].RxISR(&huart[id]);
	}
	else
	{
		HAL_UART_IRQHandler(&huart[id]);
	}
}

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
	for (id = 0; id < SERIAL_ID_MAX; id++)
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
#ifdef USE_SPI
#if defined(HAL_SPI_MODULE_ENABLED)

//__attribute__((noinit)) SPI_HandleTypeDef hspi[SPI_ID_MAX];
//__attribute__((noinit)) spi_bus_t aSpiBus[SPI_ID_MAX];

PERM_SECTION(".noinit") SPI_HandleTypeDef hspi[SPI_ID_MAX];
PERM_SECTION(".noinit") spi_bus_t aSpiBus[SPI_ID_MAX];

uint8_t BSP_SpiBus_Bind(void)
{
	aSpiBus[SPI_ID_MAIN].hHandle = &hspi[SPI_ID_MAIN];
	aSpiBus[SPI_ID_MAIN].pGpio   = gpio_spi[SPI_ID_MAIN];
	aSpiBus[SPI_ID_MAIN].pIomux  = io_cfg_spi[SPI_ID_MAIN];
	((SPI_HandleTypeDef*)(aSpiBus[SPI_ID_MAIN].hHandle))->Instance = SPI1;
	return DEV_SUCCESS;
}

const struct gpio_id_s gpio_ADF7030_SS = { .port = GPIO_PORT_ID_A, .pin = GPIO_PIN_ID_4 };
const struct iomux_s io_cfg_ADF7030_SS = { IOMUX_INIT(GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_MODE_OUTPUT_PP, 1, 0), };

zz_spi_dev_t _spi_ADF7030 =
{
	.bus_id  = 0,
	.cs = {
		.pGpio   = &gpio_ADF7030_SS,
		.pIomux  = &io_cfg_ADF7030_SS,
	},
};

//SPI_HandleTypeDef hspi1 = {.Instance = SPI1};
/*
SPI_HandleTypeDef *paSPI_BusHandle[SPI_ID_MAX] =
{
	[SPI_ID_MAIN] = &hspi1,
};
*/
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

const pwr_line_t pwr_lines[MAX_NB_POWER] =
{
	[FE_ON]      = { PWR_LINE_INIT(FE_EN) },
	[PA_ON]      = { PWR_LINE_INIT(FE_BYP) },
	[RF_ON]      = { PWR_LINE_INIT(V_RF_EN) },
	[INT_EEPROM] = { PWR_LINE_INIT(EEPROM_CTRL) },
};

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

struct rcc_clk_state_s
{
	uint32_t ahb1_clk;
	uint32_t ahb2_clk;
	uint32_t ahb3_clk;
	uint32_t apb1r1_clk;
	uint32_t apb1r2_clk;
	uint32_t apb2_clk;
	union {
		uint32_t sys_clk;
		struct {
			uint32_t pll:1;
			uint32_t hse:1;
			uint32_t msi:1;
		};
	};
};

static struct rcc_clk_state_s _rcc_clk_state_;

uint32_t u32WakeUpPort;
uint16_t u16WakeUpPin;

void BSP_LowPower_SaveRccClock(void)
{
	// Save the current rcc clock state
	_rcc_clk_state_.ahb2_clk = RCC->AHB2ENR;
	_rcc_clk_state_.apb1r1_clk = RCC->APB1ENR1;
	_rcc_clk_state_.apb1r2_clk = RCC->APB1ENR2;
	_rcc_clk_state_.apb2_clk = RCC->APB2ENR;


	_rcc_clk_state_.sys_clk = 0;

	if (__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_PLLCLK)
	{
		if (READ_BIT(RCC->CR, RCC_CR_PLLRDY))
		{
			// Only HSE is accepted as PLL source
			if (__HAL_RCC_GET_PLL_OSCSOURCE() == RCC_PLLSOURCE_HSE)
			{
				_rcc_clk_state_.hse = 1;
				_rcc_clk_state_.pll = 1;
			}
		}
	}
	else
	{
		// Here, the other accepted OSC is MSI only (RCC_SYSCLKSOURCE_STATUS_MSI)
		_rcc_clk_state_.msi = 1;
		if ( RCC->CR & RCC_CR_MSIPLLEN )
		{
			_rcc_clk_state_.pll = 1;
		}
	}
}

void BSP_LowPower_RestoreRccClock(void)
{
	if (_rcc_clk_state_.pll)
	{
		if (_rcc_clk_state_.hse)
		{
			// Enable HSE
			RCC->CR |= RCC_CR_HSEON;
		    /* Wait until HSE is ready */
		    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) != RCC_CR_HSERDY)
		    {
		    }

			// Enable the main PLL.
			__HAL_RCC_PLL_ENABLE();
		    /* Wait until PLL is ready */
		    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) != RCC_CR_PLLRDY)
		    {
		    }

		    // Set SYSCLK
		    __HAL_RCC_SYSCLK_CONFIG(RCC_SYSCLKSOURCE_PLLCLK);
		    /* Wait until SWS is PLL_SRC */
			while (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_CFGR_SWS_PLL)
			{
			}
		}
		else // _rcc_clk_state_.msi == 1;
		{
			if (_rcc_clk_state_.pll)
			{
				HAL_RCCEx_EnableMSIPLLMode();
			}
		}
	}


	// Restore the current rcc clock state
	RCC->AHB2ENR = _rcc_clk_state_.ahb2_clk;
	RCC->APB1ENR1 = _rcc_clk_state_.apb1r1_clk;
	RCC->APB1ENR2 = _rcc_clk_state_.apb1r2_clk;
	RCC->APB2ENR = _rcc_clk_state_.apb2_clk;
}

void BSP_LowPower_DisableRccClock(void)
{
    // Disable all clock
	RCC->APB1ENR1 = 0;
	RCC->APB1ENR2 = 0;
	RCC->APB2ENR = 0;
	RCC->AHB2ENR = 0;
	// Disable the FLASH => require run code and remap vector in SRAM
}

void BSP_LowPower_SetWakeUp(struct gpio_id_s wake_up_gpio)
{
    u32WakeUpPort = BSP_Gpio_GetPort(wake_up_gpio.port);
    u16WakeUpPin = BSP_Gpio_GetPin(wake_up_gpio.pin);
}

void BSP_LowPower_OnStopEnter(lp_mode_e eLpMode)
{
	(void)eLpMode;
	// Disable all peripheral except SRAM1/2,

	// Save the current rcc clock state
	BSP_LowPower_SaveRccClock();

    /*
     *  Warning :
     *  it is assumed that function "BSP_LowPower_SetWakeUp(wake_up_gpio);" has
     *  been previously called to setup the wake-up pin
    */

    // Set all ETXI intended to wake-up from STOP (RTC_WKUP, RTC_ALM, PHY_IT, COM_IT)
	BSP_Gpio_InputEnable(u32WakeUpPort, u16WakeUpPin, 1);
    BSP_GpioIt_ConfigLine(u32WakeUpPort, u16WakeUpPin, GPIO_IRQ_FALLING_EDGE);
    BSP_GpioIt_SetCallback(u32WakeUpPort, u16WakeUpPin, NULL, NULL );
    BSP_GpioIt_SetLine(u32WakeUpPort, u16WakeUpPin, 1);

    // Disable all clock
    BSP_LowPower_DisableRccClock();
}

void BSP_LowPower_OnStopExit(lp_mode_e eLpMode)
{
	(void)eLpMode;
	// Restore the current rcc clock state
	BSP_LowPower_RestoreRccClock();
	// Disable IT line
	BSP_GpioIt_SetLine(u32WakeUpPort, u16WakeUpPin, 0);
}

/*******************************************************************************/
/*!
 * @}
 * @endcond
 */

#ifdef __cplusplus
}
#endif

/*! @} */
