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

extern RTC_HandleTypeDef hrtc;

extern UART_HandleTypeDef husart1;
extern UART_HandleTypeDef huart4;

uart_dev_t aDevUart[UART_ID_MAX] =
{
	[UART_ID_CONSOLE] = {
			//.hHandle = &husart1,
			.hHandle = &huart4,
			.pfEvent = NULL
	},
	[UART_ID_COM]     = {
			.hHandle = &huart4,
			.pfEvent = NULL
	},
};

/*******************************************************************************/
// RTC related call-back handler
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

/*******************************************************************************/
// UART related call-back handler

static void _send_event_to_cb_(UART_HandleTypeDef *huart, uint32_t evt);

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
			break;
		}
	}
}

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

/******************************************************************************/
// LPTIM related call-back handler

#if defined(HAL_LPTIM_MODULE_ENABLED)
#if defined (LPTIM1)
extern LPTIM_HandleTypeDef hlptim1;
#endif
#if defined (LPTIM2)
extern LPTIM_HandleTypeDef hlptim2;
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

/*******************************************************************************/
extern SPI_HandleTypeDef hspi1;

spi_dev_t spi_ADF7030 =
{
	.bus_id  = SPI_ID_MAIN,
	.ss_port = GPIO_PORT(ADF7030_SS),
	.ss_pin  = GPIO_PIN(ADF7030_SS)
};

SPI_HandleTypeDef *paSPI_BusHandle[SPI_ID_MAX] =
{
	[SPI_ID_MAIN] = &hspi1,
};

/*******************************************************************************/
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

i2c_dev_t i2c_EEPROM =
{
	.bus_id = I2C_ID_INT,
	.device_id = EEPROM_ADDRESS,
};

I2C_HandleTypeDef *paI2C_BusHandle[I2C_ID_MAX] =
{
	[I2C_ID_INT] = &hi2c1,
	[I2C_ID_EXT] = &hi2c2,
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
