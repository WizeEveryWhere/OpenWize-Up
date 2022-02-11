/**
  * @file: bsp_pwrlines.c
  * @brief: This file contains functions to enable/disable power board peripherals
  * 
  *****************************************************************************
  * @Copyright 2019, GRDF, Inc.  All rights reserved.
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
  *****************************************************************************
  *
  * Revision history
  * ----------------
  * 1.0.0 : 2020/09/22[GBI]
  * Initial version
  *
  *
  */
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_pwrlines.h"
#include "platform.h"

#if defined (USE_BSP_PWRLINE_TRACE)
#ifndef TRACE_BSP_PWRLINE
#define TRACE_BSP_PWRLINE(...) fprintf (stdout, __VA_ARGS__ )
#endif
#else
#define TRACE_BSP_PWRLINE(...)
#endif

#define GP_PORT_NAME(name) name ##_GPIO_Port
#define GP_PIN_NAME(name) name##_Pin

#define GP_PORT(name) (uint32_t)(GP_PORT_NAME(name))
#define GP_PIN(name)  GP_PIN_NAME(name)
#define PWR_LINE_INIT(name) GP_PORT(name), GP_PIN(name)

/******************************************************************************/
typedef enum {
	//            9876543210
	FE_ON     ,
	PA_ON     ,
	RF_ON     ,
	INT_EEPROM,
	//
	MAX_NB_POWER
}pwr_id;

typedef struct pwr_line_s {
	uint32_t u32Port;
	uint16_t u16Pin;
}pwr_line_t;

static uint16_t _pwr_lines;

static const pwr_line_t pwr_lines[MAX_NB_POWER] =
{
	[FE_ON]      = { PWR_LINE_INIT(FE_EN) },
	[PA_ON]      = { PWR_LINE_INIT(FE_BYP) },
	[RF_ON]      = { PWR_LINE_INIT(PA_V_EN) },
	[INT_EEPROM] = { PWR_LINE_INIT(EEPROM_CTRL) },
};

static const char pwr_name[MAX_NB_POWER][12] = {
	[FE_ON]      = "FE_EN",
	[PA_ON]      = "FE_BYP",
	[RF_ON]      = "RF_EN",
	[INT_EEPROM] = "INT_EEPROM",
};

static void _set_pwr(uint16_t u16PwrLineId)
{
	TRACE_BSP_PWRLINE("Enable %s Power\n", pwr_name[u16PwrLineId]);
	HAL_GPIO_WritePin((GPIO_TypeDef*)(pwr_lines[u16PwrLineId].u32Port), pwr_lines[u16PwrLineId].u16Pin, (GPIO_PinState)1);
}

static void _clr_pwr(uint16_t u16PwrLineId)
{
	TRACE_BSP_PWRLINE("Disable %s Power\n", pwr_name[u16PwrLineId]);
	HAL_GPIO_WritePin((GPIO_TypeDef*)(pwr_lines[u16PwrLineId].u32Port), pwr_lines[u16PwrLineId].u16Pin, (GPIO_PinState)0);
}

void BSP_PwrLine_Clr (uint16_t u16PwrLines)
{
	uint16_t pwr_lines = _pwr_lines ^ u16PwrLines;
	uint16_t u16Msk;

	if( u16PwrLines & RF_EN_MSK )
	{
		_clr_pwr(RF_ON);
		_clr_pwr(FE_ON);
	}
	if( u16PwrLines & PA_EN_MSK )
	{
		_clr_pwr(PA_ON);
	}
	if( u16PwrLines & INT_EEPROM_EN_MSK )
	{
		_clr_pwr(INT_EEPROM);
	}

	_pwr_lines &= ~u16PwrLines;
}

void BSP_PwrLine_Set (uint16_t u16PwrLines)
{
	if( u16PwrLines & RF_EN_MSK )
	{
		_set_pwr(RF_ON);
		_set_pwr(FE_ON);
	}
	if( u16PwrLines & PA_EN_MSK )
	{
		_set_pwr(PA_ON);
	}
	if( u16PwrLines & INT_EEPROM_EN_MSK )
	{
		_set_pwr(INT_EEPROM);
	}
	_pwr_lines |= u16PwrLines;
}

uint16_t BSP_PwrLine_Get (uint16_t u16PwrLines)
{
	return _pwr_lines & u16PwrLines;
}

void BSP_PwrLine_Init (void)
{
	_pwr_lines = 0;
	TRACE_BSP_PWRLINE("\n[BSP_PwrLine_Init]\n");
	BSP_PwrLine_Clr((uint16_t)0xFFFF);
}

/******************************************************************************/

#include "bsp_gpio_it.h"
extern RTC_HandleTypeDef hrtc;
extern UART_HandleTypeDef *paUART_BusHandle[UART_ID_MAX];

#define WKUP_PIN_NAME DEBUG_RXD
#define WKUP_UART_ID UART_ID_CONSOLE
//#define UART_IS_DEINIT

static inline void _lp_clk_cfg_ (void);

//#define CLK_CFG_HSE_PLL_HSI
//#define CLK_CFG_MSI_HSI
static inline void _lp_clk_cfg_ (void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	uint32_t pFLatency = 0;

#if defined( CLK_CFG_HSE_PLL_HSI )
	HAL_RCC_GetOscConfig(&RCC_OscInitStruct);
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;

	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	//RCC_OscInitStruct.OscillatorType |= RCC_OSCILLATORTYPE_HSI;
	//RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	//RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK)
	{
		Error_Handler();
	}

#elif defined (CLK_CFG_MSI_HSI )
	HAL_RCC_GetOscConfig(&RCC_OscInitStruct);
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI | RCC_OSCILLATORTYPE_HSI;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK)
	{
		Error_Handler();
	}

#else // (CLK_CFG_MSI)
	// nothing to do
#endif
}

static void _wkup_cb_(void *pParam, void *pArg)
{

}

static void _lp_entry_ (void)
{
	int8_t i8LineId;

#ifdef UART_IS_DEINIT
	if (HAL_UART_DeInit( paUART_BusHandle[WKUP_UART_ID] ) != HAL_OK)
	{
		Error_Handler();
	}
	BSP_Gpio_InputEnable( PWR_LINE_INIT(WKUP_PIN_NAME), 1);
#endif
    BSP_GpioIt_ConfigLine( PWR_LINE_INIT(WKUP_PIN_NAME), GPIO_IRQ_FALLING_EDGE);
    BSP_GpioIt_SetLine( PWR_LINE_INIT(WKUP_PIN_NAME), 1);
    BSP_GpioIt_SetCallback( PWR_LINE_INIT(WKUP_PIN_NAME), &_wkup_cb_, NULL );


    i8LineId = BSP_GpioIt_GetLineId( GP_PIN(WKUP_PIN_NAME));
    if (i8LineId < 5)
    {
    	// IT line from 0, 1, 2, 3 and 4
    	HAL_NVIC_EnableIRQ(EXTI0_IRQn + i8LineId);
    }
    else
    {
        if(i8LineId < 10)
        {
        	// IT line from 5 to 9
        	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
        }
        else
        {
        	// IT line from 10 to 15
        	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
        }
    }
#if defined (CLK_CFG_HSE_PLL_HSI) || defined (CLK_CFG_MSI_HSI)
    // Auto enable HSI when MSI is ready
    SET_BIT(RCC->CR, RCC_CR_HSIASFS );
#endif
}

static void _lp_exit_ (void)
{
	BSP_GpioIt_SetLine( PWR_LINE_INIT(WKUP_PIN_NAME), 0);

	_lp_clk_cfg_();

#ifdef UART_IS_DEINIT
	if (HAL_UART_Init( paUART_BusHandle[WKUP_UART_ID] ) != HAL_OK)
	{
		Error_Handler();
	}
#endif
}

void BSP_LowPower_Enter (lp_mode_e eLpMode)
{
	HAL_SuspendTick();
	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

	_lp_entry_();
	switch(eLpMode)
	{
		case LP_SHTDWN_MODE:
			HAL_PWREx_EnterSHUTDOWNMode();
			break;
		case LP_STDBY_MODE:
			HAL_PWR_EnterSTANDBYMode();
			break;
		case LP_STOP1_MODE:
			HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);
			break;
		case LP_STOP2_MODE:
			HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
			break;
		case LP_SLEEP_MODE:
		default:
			HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
			break;
	}
	_lp_exit_();
	HAL_ResumeTick();
}

/******************************************************************************/

#ifdef __cplusplus
}
#endif
