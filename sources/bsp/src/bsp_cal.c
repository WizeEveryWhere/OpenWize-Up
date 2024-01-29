/**
  * @file: bsp_cal.c
  * @brief: // TODO This file ...
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
  * @par 1.0.0 : 2023/10/30[GBI]
  * Initial version
  *
  *
  */

/*! @addtogroup cal
 *  @ingroup bsp
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_cal.h"
#include "platform.h"
#include <stm32l4xx_hal.h>

/*******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */

static void Tim15_Cfg(uint16_t u16ARR, uint16_t u16PSC);
static void Tim1_Cfg(uint16_t u16ARR, uint16_t u16PSC);
static void Tim15_Init(uint16_t u16ARR, uint16_t u16PSC);

static void Tim1_Init(uint16_t u16ARR, uint16_t u16PSC);
static void Tim15_DeInit(void);
static void Tim1_DeInit(void);

static void Tim3_Cfg(uint16_t u16ARR, uint16_t u16PSC);
static void Tim3_Init(uint16_t u16ARR, uint16_t u16PSC);
static void Tim3_DeInit(void);

static void Tim2_Cfg(uint16_t u16ARR, uint16_t u16PSC);
static void Tim2_Init(uint16_t u16ARR, uint16_t u16PSC);
static void Tim2_DeInit(void);

/*!
 * @}
 * @endcond
 */

__attribute__((weak))
void Calibration_Done(uint32_t u32PulseCnt)
{
	(void)u32PulseCnt;
}

void Calibrate_Run(void)
{
	// Enable Calibration Timer
	BSP_TmrCalib_Enable();
	// Enable 1Hz Clock
	BSP_TmrClk_Enable(sTimSetup[O_CLK_1Hz].sT15, sTimSetup[O_CLK_1Hz].sT1, 0);
}

void Calibrate_Cancel(void)
{
	// Disable 1Hz Clock
	BSP_TmrClk_Disable();
	// Disable Calibration Timer
	BSP_TmrCalib_Disable();
}

static uint8_t _bCalibDone_;
int32_t Calibrate_WaitDone(uint32_t ticktmo)
{
	uint32_t tickstart;
	_bCalibDone_ = 0;
	// ------------------------------------------------------
	// Wait
	tickstart = HAL_GetTick();
	do
	{
		if ((HAL_GetTick() - tickstart) > ticktmo)
		{
			return -1;
		}
	}
	while(!_bCalibDone_);
	// ------------------------------------------------------
	return 0;
}

void CalibTimer_IRQHandler(void)
{
	register uint32_t reg = TIM2->SR;
	TIM2->SR = 0;

	if ((reg & TIM_FLAG_CC1) == TIM_FLAG_CC1)
	{
	    if ((TIM2->DIER & TIM_IT_CC1) == TIM_IT_CC1)
	    {
	    	if ( (TIM2->CCMR1 & TIM_CCMR1_CC1S) == TIM_CCMR1_CC1S)
	    	{
	    		// Disable interrupt
	    		TIM2->DIER = 0;
	    		// Disable timer
	    		Calibrate_Cancel();
	    		// Call back
	    		Calibration_Done(TIM2->CCR1);
	    		_bCalibDone_ = 1;
	    	}
	    }
	}
}

/*******************************************************************************/
void BSP_TmrCalib_Enable(void)
{
	__HAL_DBGMCU_UNFREEZE_TIM3();

	// Enable Calibration Timer
	Tim2_Init(0xFFFF, 0x0000);
	Tim3_Init(32, 0);

	// Enable interrupt on Capture Channel 1
	TIM2->DIER = TIM_IT_CC1;
    HAL_NVIC_SetPriority(TIM2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);

	// Enable Timers
	TIM2->CR1 |= (TIM_CR1_CEN);
	TIM3->CR1 |= (TIM_CR1_CEN);
}

void BSP_TmrCalib_Disable(void)
{
	// Disable Calibration Timer
	Tim3_DeInit();
	Tim2_DeInit();
	// Disable interrupt on Capture Channel 1
	HAL_NVIC_DisableIRQ(TIM2_IRQn);
}

/*******************************************************************************/

#define TMR_SETUP(arrVal, pscVal) {.arr = arrVal, .psc = pscVal}

timer_setup_t sTimSetup[O_CLK_NB] =
{
	//[O_CLK_1Hz]     = {.sT15 = TMR_SETUP(0x289, 0),     .sT1 = TMR_SETUP(0x270F, 0), },
	[O_CLK_1Hz]     = {.sT15 = TMR_SETUP(0xC7, 0x3F7A), .sT1 = TMR_SETUP(1, 0), },
	//[O_CLK_1KHz]    = {.sT15 = TMR_SETUP(0xCB1, 0), .    sT1 = TMR_SETUP(1, 0), },
	[O_CLK_1KHz]    = {.sT15 = TMR_SETUP(1, 0x658),     .sT1 = TMR_SETUP(1, 0), },
	[O_CLK_1625KHz] = {.sT15 = TMR_SETUP(1, 0),         .sT1 = TMR_SETUP(1, 0), },
};

void BSP_TmrClk_SetRef(uint8_t eOutClkFreq, timer_period_t sT15, timer_period_t sT1)
{
	sTimSetup[eOutClkFreq].sT15 = sT15;
	sTimSetup[eOutClkFreq].sT1 = sT1;
}

void BSP_TmrClk_GetRef(uint8_t eOutClkFreq, timer_period_t *sT15, timer_period_t *sT1)
{
	*sT15 = sTimSetup[eOutClkFreq].sT15;
	*sT1 = sTimSetup[eOutClkFreq].sT1;
}


/*******************************************************************************/

/*
 * OutFreq = InFreq x
 *             1              1                      1              1
 *     ( --------------- + ----------- ) x (  -------------- + ----------- )
 *       (TIM15_PSC + 1)   (TIM15_ARR)        (TIM1_PSC + 1)   (TIM1_ARR)
 *
 * Input Frequency (InFreq) is expected signal on GPIO PB14 (at 1.625 MHz from ADF7030)
 * If selected, output frequency (OutFreq) is mapped on PA11
 */

/**
  * @brief  This function .
  *
  * @retval HAL status
  */
void BSP_TmrClk_Enable(timer_period_t sT15, timer_period_t sT1, uint8_t bOCEnable)
{
	__HAL_DBGMCU_UNFREEZE_TIM15();
	__HAL_DBGMCU_UNFREEZE_TIM1();

	// Gpio input cfg as for TIM15 External CLK
	GPIO_InitTypeDef GPIO_InitStruct =
	{
		.Pin = GPIO_PIN_14,
		.Mode = GPIO_MODE_AF_PP,
		.Pull = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_LOW,
		.Alternate = GPIO_AF14_TIM15,
	};
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// Init Timer
	Tim15_Init(sT15.arr, sT15.psc);
	Tim1_Init(sT1.arr, sT1.psc);

	// Gpio output cfg from TIM1 OC4
	if (bOCEnable)
	{
		GPIO_InitStruct.Pin = GPIO_PIN_11;
		GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}

	// Enable Timers
	TIM1->CR1 |= (TIM_CR1_CEN);
	TIM15->CR1 |= (TIM_CR1_CEN);
}

void BSP_TmrClk_Disable(void)
{
	// DeInit Timer
	Tim1_DeInit();
	Tim15_DeInit();

	// DeInit Gpio
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11);
	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_14);
}

void BSP_TmrClk_Trim(timer_period_t sT15, timer_period_t sT1)
{
	TIM15->PSC = sT15.psc;
	TIM15->ARR = sT15.arr;
	TIM1->PSC = sT1.psc;
	TIM1->ARR = sT1.arr;
}

/******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */

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

static void Tim15_Cfg(uint16_t u16ARR, uint16_t u16PSC)
{
	// Initialize TIM15
	register TIM_TypeDef *pInstance = TIM15;
	pInstance->CR1 = Tim15_CR1;
	pInstance->CR2 = Tim15_CR2;
	pInstance->ARR = u16ARR;
	pInstance->PSC = u16PSC;
	pInstance->SMCR = Tim15_SMCR;
	pInstance->CCMR1 = Tim15_CCMR1;
	pInstance->CCER = Tim15_CCER;
	pInstance->OR2 = 0;
}

static void Tim15_Init(uint16_t u16ARR, uint16_t u16PSC)
{
	// Force Reset and Enable clock
	__HAL_RCC_TIM15_FORCE_RESET();
	__HAL_RCC_TIM15_CLK_ENABLE();
	__HAL_RCC_TIM15_RELEASE_RESET();
	Tim15_Cfg(u16ARR, u16PSC);
	// register TIM_TypeDef *pInstance = TIM15;
	//TIM15->EGR = TIM_EGR_UG;
}

static void Tim15_DeInit(void)
{
	__HAL_RCC_TIM15_FORCE_RESET();
	__HAL_RCC_TIM15_RELEASE_RESET();
	__HAL_RCC_TIM15_CLK_DISABLE();
}

/******************************************************************************/

// Autoreload enable
#define	Tim1_CR1 TIM_AUTORELOAD_PRELOAD_ENABLE
// Master mode is "Update"
#define	Tim1_CR2 TIM_TRGO_UPDATE
// OC4 value
#define	Tim1_CCR4 0
// Trigger on ITR0, External Clock 1,
#define	Tim1_SMCR (TIM_TS_ITR0 | TIM_SLAVEMODE_EXTERNAL1)
// Channel 4 as Output, OC4 is in toggle mode
#define	Tim1_CCMR2 (TIM_OCMODE_TOGGLE << 8) | TIM_CCMR2_OC4PE
// Output Compare OC4 is enable
#define	Tim1_CCER (TIM_OUTPUTSTATE_ENABLE << 12)

static void Tim1_Cfg(uint16_t u16ARR, uint16_t u16PSC)
{
	// Initialize TIM1
	register TIM_TypeDef *pInstance = TIM1;
	pInstance->CR1 = Tim1_CR1;
	pInstance->CR2 = Tim1_CR2;
	pInstance->ARR = u16ARR;
	pInstance->PSC = u16PSC;
	pInstance->CCR4 = Tim1_CCR4;
	pInstance->SMCR = Tim1_SMCR;
	pInstance->CCMR2 = Tim1_CCMR2;
	pInstance->CCER = Tim1_CCER;
	pInstance->OR2 = 0;
	pInstance->OR3 = 0;
	pInstance->BDTR |= (TIM_BDTR_MOE);
}

static void Tim1_Init(uint16_t u16ARR, uint16_t u16PSC)
{
	// Force Reset and Enable clock
	__HAL_RCC_TIM1_FORCE_RESET();
	__HAL_RCC_TIM1_CLK_ENABLE();
	__HAL_RCC_TIM1_RELEASE_RESET();
	Tim1_Cfg(u16ARR, u16PSC);
	// register TIM_TypeDef *pInstance = TIM1;
	//TIM1->EGR = TIM_EGR_UG;
}

static void Tim1_DeInit(void)
{
	__HAL_RCC_TIM1_FORCE_RESET();
	__HAL_RCC_TIM1_RELEASE_RESET();
	__HAL_RCC_TIM1_CLK_DISABLE();
}

/******************************************************************************/

// Autoreload disable
#define	Tim3_CR1 TIM_AUTORELOAD_PRELOAD_DISABLE
// Master mode is "Update"
#define	Tim3_CR2 TIM_TRGO_UPDATE
// Trigger on ITR0, External Clock 1,
#define	Tim3_SMCR (TIM_SLAVEMODE_EXTERNAL1 | TIM_TS_ITR0)

static void Tim3_Cfg(uint16_t u16ARR, uint16_t u16PSC)
{
	// Initialize TIM3
	register TIM_TypeDef *pInstance = TIM3;
	pInstance->CR1 = Tim3_CR1;
	pInstance->CR2 = Tim3_CR2;
	pInstance->ARR = u16ARR;
	pInstance->PSC = u16PSC;
	pInstance->SMCR = Tim3_SMCR;

	pInstance->OR1 = 0;
	pInstance->OR2 = 0;
}

static void Tim3_Init(uint16_t u16ARR, uint16_t u16PSC)
{
	// Force Reset and Enable clock
	__HAL_RCC_TIM3_FORCE_RESET();
	__HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_TIM3_RELEASE_RESET();
	Tim3_Cfg(u16ARR, u16PSC);
	//TIM3->EGR = TIM_EGR_UG;
}

static void Tim3_DeInit(void)
{
	__HAL_RCC_TIM3_FORCE_RESET();
	__HAL_RCC_TIM3_RELEASE_RESET();
	__HAL_RCC_TIM3_CLK_DISABLE();
}

/******************************************************************************/

// Autoreload disable
#define	Tim2_CR1 TIM_AUTORELOAD_PRELOAD_DISABLE
// Master mode is "Update"
#define	Tim2_CR2 TIM_TRGO_UPDATE
// Trigger on ITR0, External Clock 2,
#define	Tim2_SMCR (TIM_SMCR_ECE | TIM_SLAVEMODE_COMBINED_RESETTRIGGER | TIM_TS_ITR0)
//#define	Tim2_SMCR (TIM_CLOCKSOURCE_ETRMODE2 | TIM_SLAVEMODE_RESET | TIM_TS_ITR0)
// External trigger remap to LSE
#define	Tim2_OR1 TIM_TIM2_ETR_LSE

// Channel 1 as Input, IC1 on TRC
#define	Tim2_CCMR1 TIM_ICSELECTION_TRC
// Input Capture IC1 is enable
#define	Tim2_CCER TIM_CCER_CC1E

// Possible output for CH3 or CH4, on GPIO PB10 or PB11 (GPIO_AF1_TIM2)

static void Tim2_Cfg(uint16_t u16ARR, uint16_t u16PSC)
{
	(void)u16ARR;
	// Initialize TIM2
	register TIM_TypeDef *pInstance = TIM2;
	pInstance->CR1 = Tim2_CR1;
	pInstance->CR2 = Tim2_CR2;
	//pInstance->ARR = u16ARR;
	pInstance->PSC = u16PSC;
	pInstance->SMCR = Tim2_SMCR;
	pInstance->CCMR1 = Tim2_CCMR1;

	pInstance->CCER = Tim2_CCER;
	pInstance->OR1 = Tim2_OR1;
	pInstance->OR2 = 0;
}

static void Tim2_Init(uint16_t u16ARR, uint16_t u16PSC)
{
	// Force Reset and Enable clock
	__HAL_RCC_TIM2_FORCE_RESET();
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_TIM2_RELEASE_RESET();
	Tim2_Cfg(u16ARR, u16PSC);
	//TIM2->EGR = TIM_EGR_UG;
	//TIM2->SR = 0;
}

static void Tim2_DeInit(void)
{
	__HAL_RCC_TIM2_FORCE_RESET();
	__HAL_RCC_TIM2_RELEASE_RESET();
	__HAL_RCC_TIM2_CLK_DISABLE();
}

/*!
 * @}
 * @endcond
 */

/*******************************************************************************/

#ifdef __cplusplus
}
#endif

/*! @} */
