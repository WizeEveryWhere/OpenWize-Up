/**
  * @file: ext_ref_clk.c
  * @brief: // TODO This file ...
  * 
  *****************************************************************************
  * @Copyright 2025, GRDF, Inc.  All rights reserved.
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
  * 1.0.0 : 2025/02/21[TODO: your name]
  * Initial version
  *
  *
  */
#ifdef __cplusplus
extern "C" {
#endif

#include "ext_ref_clk.h"
#include <stm32l4xx_hal.h>

/******************************************************************************/

/*
 * ARR min = 1
 * PSC min = 0
 *
 * OutFreq = InFreq x
 *              1                1
 *     ( --------------- x -------------- )
 *       (TIM15_PSC + 1)   (TIM15_ARR + 1)
 *
 *
 * Input Frequency (InFreq) is expected signal on GPIO PB14 (at 6.5 MHz from ADF7030)
 */
static const tim_period_t aRefClk[REF_CLK_NB] =
{
	/*
	 * @1Hz : Delta measure 1/2 period = 500ms
	 * Freq. To measure | nb pulse
	 * 100Hz  | 50
	 * 32KHz  | 16000
	*/
	[REF_CLK_2Hz]     = TMR_SETUP(199, 16249),
	/*
	 * @10Hz : Delta measure 1/2 period = 50ms
	 * Freq. To measure | nb pulse
	 * 32KHz  | 1600
	 * 1 MHz  | 50000
	*/
	[REF_CLK_20Hz]    = TMR_SETUP(19,  16249),

	/*
	 * @100Hz : Delta measure 1/2 period = 5ms
	 * Freq. To measure | nb pulse
	 * 32KHz  | 160
	 * 1 MHz  | 5000
	 * 4 MHz  | 20000
	*/
	[REF_CLK_200Hz]   = TMR_SETUP(1,   16249),
	/*
	 * @1KHz : Delta measure 1/2 period = 500µs
	 * Freq. To measure | nb pulse
	 * 4 MHz  | 2000
	 * 24 MHz | 12000
	 * 48 MHz | 24000
	 * 80 MHz | 40000
	*/
	[REF_CLK_2000Hz]  = TMR_SETUP(1,    1624),
	/*
	 * @10KHz : Delta measure 1/2 period = 50µs
	 * Freq. To measure | nb pulse
	 * 4 MHz  | 200
	 * 24 MHz | 1200
	 * 48 MHz | 2400
	 * 80 MHz | 4000
	*/
	[REF_CLK_20000Hz] = TMR_SETUP(4,      64),
#ifdef HAS_REF_xDIV
	[REF_CLK_12500Hz]  = TMR_SETUP(1,     259),
	[REF_CLK_25000Hz] = TMR_SETUP(1,     129),

	/*
	 * @25KHz : Delta measure 1/2 period = 20µs
	 * Freq. To measure | nb pulse
	 * 4 MHz  | 80
	 * 24 MHz | 480
	 * 48 MHz | 960
	 * 80 MHz | 1600
	*/
	[REF_CLK_50000Hz] = TMR_SETUP(1,      64),

	/*
	 * @62.5KHz : Delta measure 1/2 period = 8µs
	 * Freq. To measure | nb pulse
	 * 4 MHz  | 32
	 * 24 MHz | 192
	 * 48 MHz | 384
	 * 80 MHz | 640
	*/
	[REF_CLK_125KHz] = TMR_SETUP(1,      25),
	[REF_CLK_250KHz]  = TMR_SETUP(1,      12),
	[REF_CLK_500KHz]  = TMR_SETUP(12,      0),
	[REF_CLK_3250KHz] = TMR_SETUP(1,       0),
#endif
};

/******************************************************************************/
#include "bsp_gpio.h"
#include "platform.h"

EXT_REF_CLK_GPIO();
EXT_REF_CLK_IOMUX();

static void Tim15_Cfg(uint16_t u16ARR, uint16_t u16PSC);
static void Tim15_Init(uint16_t u16ARR, uint16_t u16PSC);
static void Tim15_DeInit(void);

/******************************************************************************/
void ExtRefClk_Enable(uint8_t eRefClk)
{
	uint8_t ref_clk = REF_CLK_20000Hz;
	if (eRefClk < REF_CLK_NB)
	{
		ref_clk = eRefClk;
	}

	// Gpio input cfg as for TIM15 External CLK
	BSP_Gpio_Config(gpio_cfg_ref[0], io_cfg_ref[0].io);
	// Init Timer 15
	Tim15_Init(aRefClk[ref_clk].arr, aRefClk[ref_clk].psc);
	// Enable Timers
	TIM15->CR1 |= (TIM_CR1_CEN);
}

void ExtRefClk_Disable(void)
{
	// DeInit Timer
	Tim15_DeInit();
	// DeInit Gpio
	struct iomux_s iomux = {IOMUX_DEFAULT_ANALOG()};
	BSP_Gpio_Config(gpio_cfg_ref[0], iomux.io);
}

void ExtRefClk_Trim(uint8_t eRefClk)
{
	if (eRefClk < REF_CLK_NB)
	{
		TIM15->ARR = aRefClk[eRefClk].arr;
		TIM15->PSC = aRefClk[eRefClk].psc;
	}
}

/******************************************************************************/
// Autoreload enable
#define	Tim15_CR1 TIM_AUTORELOAD_PRELOAD_ENABLE
// Master mode is "Update"
#define	Tim15_CR2 TIM_TRGO_UPDATE
// Trigger on TI1_ED, External Clock 1,
#define	Tim15_SMCR (TIM_TS_TI1F_ED | TIM_SLAVEMODE_EXTERNAL1)
// Channel 1 as Input, IC1 is mapped to TI1
#define	Tim15_CCMR1 TIM_ICSELECTION_DIRECTTI
// Capture IC1 to CCR1 is enable
#define	Tim15_CCER (TIM_INPUTCHANNELPOLARITY_RISING | TIM_OUTPUTSTATE_ENABLE)

// Possible output : (AF14)
// CH1N PA1
// CH1 PA9 ????
static void Tim15_Cfg(uint16_t u16ARR, uint16_t u16PSC)
{
	// Initialize TIM15
	register TIM_TypeDef *TIMx = TIM15;
	TIMx->CR1 = Tim15_CR1;
	TIMx->CR2 = Tim15_CR2;
	TIMx->ARR = u16ARR;
	TIMx->PSC = u16PSC;
	TIMx->SMCR = Tim15_SMCR;
	TIMx->CCMR1 = Tim15_CCMR1;
	TIMx->CCER = Tim15_CCER;
	TIMx->OR2 = 0;
}

static void Tim15_Init(uint16_t u16ARR, uint16_t u16PSC)
{
	// Force Reset and Enable clock
	__HAL_RCC_TIM15_FORCE_RESET();
	__HAL_RCC_TIM15_CLK_ENABLE();
	__HAL_RCC_TIM15_RELEASE_RESET();
	Tim15_Cfg(u16ARR, u16PSC);
	// register TIM_TypeDef *TIMx = TIM15;
	//TIM15->EGR = TIM_EGR_UG;
}

static void Tim15_DeInit(void)
{
	__HAL_RCC_TIM15_FORCE_RESET();
	__HAL_RCC_TIM15_RELEASE_RESET();
	__HAL_RCC_TIM15_CLK_DISABLE();
}

/******************************************************************************/

#ifdef __cplusplus
}
#endif
