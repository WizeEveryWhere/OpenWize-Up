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
static void Tim15_Init(uint16_t u16ARR, uint16_t u16PSC);
static void Tim15_DeInit(void);

static void Tim1_Cfg(uint16_t u16ARR, uint16_t u16PSC);
static void Tim1_Init(uint16_t u16ARR, uint16_t u16PSC);
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
/******************************************************************************/

// Configuration
#define Lptim1_CFGR LPTIM_COUNTERSOURCE_EXTERNAL

// LPTIM_UPDATE_ENDOFPERIOD

// LPTIM_CFGR_TIMOUT


// TRIGEN[1:0]: Trigger enable and polarity

// TRIGSEL[2:0]: Trigger selector
// LPTIM_TRIGSOURCE_0 : GPIO
// LPTIM_TRIGSOURCE_1 : RTC_ALR_A
// LPTIM_TRIGSOURCE_2 : RTC_ALR_B

// PRESC[2:0]: Clock prescaler
// From LPTIM_PRESCALER_DIV1 to LPTIM_PRESCALER_DIV128

// CKPOL[1:0]: Clock Polarity

// CKSEL: Clock selector
// LPTIM_CLOCKSOURCE_ULPTIM
// LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC

// Control
#define Lptim1_CR

#define Lptim1_CMP

#define Lptim1_ARR

#define Lptim1_CNT


#define Lptim1_OR


#define RCC_Lptim1_CLKSEL
// APB, LSE, LSI, HSI


// Possible input/output : (AF1)
// OUT PA14
// IN1 PB5
// ETR PB6
static void Lptim1_Cfg(uint16_t u16ARR, uint16_t u16PSC)
{

}

static void Lptim1_Init(uint16_t u16ARR, uint16_t u16PSC)
{

}

static void Lptim1_DeInit(void)
{
	// Initialize LPTIM1
	register LPTIM_TypeDef *TIMx = LPTIM1;
}

/******************************************************************************/
#if 0

// Control 1
// TIM_OPMODE_SINGLE : Counter stops counting at the next update event (clearing the bit CEN)
//#define Tim16_CR1 TIM_OPMODE_SINGLE | TIM_AUTORELOAD_PRELOAD_ENABLE
#define Tim16_CR1 0x0

// Capture/Compare mode 1
// CC1S: Capture/Compare 1 selection : CC1 channel is configured as input, IC1 is mapped on TI1
//#define Tim16_CCMR1 TIM_CCMR1_CC1S_0

//#define	Tim16_CCER (TIM_INPUTCHANNELPOLARITY_RISING | TIM_OUTPUTSTATE_ENABLE)


#pragma GCC push_options
#pragma GCC optimize("O0")

#define CAPTURE_START              ((uint32_t) 0x00000001)
#define CAPTURE_ONGOING            ((uint32_t) 0x00000002)
#define CAPTURE_COMPLETED          ((uint32_t) 0x00000003)

uint32_t  __IO CaptureState = 0;
uint32_t  __IO Capture = 0;
uint32_t IC1ReadValue1 = 0;
uint32_t IC1ReadValue2 = 0;

void Update_CC(uint32_t u32CCValue)
{
	if (CaptureState == CAPTURE_START)
	{
		/* Get the 1st Input Capture value */
		IC1ReadValue1 = u32CCValue;
		CaptureState = CAPTURE_ONGOING;
	}
	else if (CaptureState == CAPTURE_ONGOING)
	{
		/* Get the 2nd Input Capture value */
		IC1ReadValue2 = u32CCValue;

		/* Capture computation */
		if (IC1ReadValue2 > IC1ReadValue1)
		{
			Capture = (IC1ReadValue2 - IC1ReadValue1);
		}
		else if (IC1ReadValue2 < IC1ReadValue1)
		{
			Capture = ((0xFFFF - IC1ReadValue1) + IC1ReadValue2);
		}
		else
		{
			/* If capture values are equal, we have reached the limit of frequency
			measures */
			Error_Handler();
		}

		CaptureState = CAPTURE_COMPLETED;
	}
}

// void HAL_TIM_IRQHandler(TIM_HandleTypeDef *htim)
void Meas_IRQHandler(void)
{
	register uint32_t reg = TIM16->SR;
	if ((reg & TIM_FLAG_CC1) == TIM_FLAG_CC1)
	{
	    if ((TIM16->DIER & TIM_IT_CC1) == TIM_IT_CC1)
	    {
	    	TIM16->SR = 0;
	    	if ( (TIM16->CCMR1 & TIM_CCMR1_CC1S) == TIM_CCMR1_CC1S_0)
	    	{
	    		// Call back
	    		Update_CC( (uint32_t)(TIM16->CCR1) );
	    	}
	    }
	}
}

static uint32_t _do_one_meas_(uint32_t timeout)
{
	uint32_t tmo = timeout;

	CaptureState = CAPTURE_START;
	// Enable IT
	TIM16->DIER = (TIM_DIER_CC1IE);
	// Enable capture on IC1
	TIM16->CCER |= TIM_CCER_CC1E;
	// Enable counting
	TIM16->CR1 |= (TIM_CR1_CEN);
	/* Enable the TIMx IRQ channel */
	HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
	/* Wait for end of capture: two consecutive captures */
	while ((CaptureState != CAPTURE_COMPLETED) && (tmo != 0))
	{
		if (--tmo == 0)
		{
			break;
		}
	}
	/* Disable the TIMx global Interrupt */
	HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);

	// This will be odne in Tim16_DeInit:
	// - Disable IT, Disable capture on IC1, Disable counting
	/*
	// Disable IT
	TIM16->DIER &= ~(TIM_DIER_CC1IE);
	// Disable capture on IC1
	TIM16->CCER &= ~(TIM_CCER_CC1E);
	// Disable counting
	TIM16->CR1 &= ~(TIM_CR1_CEN);
	*/

	// Timeout
	if (tmo == 0)
	{
		Capture = 0;
	}

	return Capture;
}

enum meas_type_e {
	TIM_IC_FREQ = 0,
	TIM_CK_FREQ = 1,
};

static uint32_t _do_avg_meas_(uint8_t u8Type, uint8_t u8NbLoop, uint32_t u32Scale, uint32_t timeout)
{
	uint32_t meas = 0;
	uint8_t loop_cnt = 0;
	uint32_t capture;

	while (loop_cnt <= u8NbLoop)
	{
		capture = _do_one_meas_(timeout);

		if (capture == 0)
		{
			// Error
			meas = 0;
			goto done;
		}

		if (loop_cnt != 0)
		{
			/* Compute the frequency value :
			 *
			 * ratio = IcPSC / (PSC + 1)
			 * scale = RefFreq x ratio
			 *       = RefFreq x IcPSC / (PSC + 1)
			 *
			 * case 1 : IC clk is ref.  --> measure the TIM clk
			 * meas = ratio x RefFreq x capture
			 *      = scale x capture
			 *
			 * case 0 : TIM clk is ref. --> measure the IC clk
			 * meas = ratio x RefFreq / capture
			 *      = scale / capture
			 *
			*/
			if (u8Type == TIM_CK_FREQ)
			{
				meas += (u32Scale * capture);
			}
			else // u8Type == TIM_IC_FREQ
			{
				meas += (u32Scale / capture);
			}

		}
	    /* Increment counter */
		loop_cnt++;
	}

	/* Compute the average of frequency value */
	meas = (uint32_t) (meas / u8NbLoop);
done :
	return (uint32_t)(meas);
}

void BSP_CalInit(void)
{
	__HAL_DBGMCU_UNFREEZE_TIM16();
	__HAL_DBGMCU_FREEZE_TIM16();


	// If use LSE as ref clk

#if 0
	/* The signal in input capture is divided by 8 */
	#define MSI_TIMx_IC_DIVIDER         TIM_ICPSC_DIV8
	/* The LSE is divided by 8 => LSE/8 = 32768/8 = 4096 */
	#define MSI_REFERENCE_FREQUENCY     ((uint32_t)4096) /* The reference frequency value in Hz */

	#define MSI_NUMBER_OF_LOOPS         ((uint32_t)50)
	// If use GPIO as ref clk (

	/* The signal in input capture is not divided */
	#define MSI_TIMx_IC_DIVIDER         TIM_ICPSC_DIV1
	/* The reference frequency is 1000 Hz (the signal in input capture is not divided) */
	#define MSI_REFERENCE_FREQUENCY     ((uint32_t)1000) /* The reference frequency value in Hz */

	#define MSI_NUMBER_OF_LOOPS         ((uint32_t)10)
#endif
	/* Configure the NVIC for TIMx */
	HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 0, 1);
	/* Disable the TIMx global Interrupt */
	HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);


	// Option register 1
	// TI1_RMP[2:0]: Input capture 1 remap
	// TIM_TIM16_TI1_GPIO
	// TIM_TIM16_TI1_LSI
	// TIM_TIM16_TI1_LSE
	// TIM_TIM16_TI1_RTC
	// TIM_TIM16_TI1_MSI
	// TIM_TIM16_TI1_HSE_32
	// TIM_TIM16_TI1_MCO

	// With TIM16
	// LSI measurement : TIM_ICPSC_DIV4

	// With TIM15
	// MSI measurement :
	// - LSE as ref. clk  : TIM_ICPSC_DIV8
	// - GPIO as ref. clk : TIM_ICPSC_DIV1 (for input clock of 1000 Hz)

	g_xSysInfo.pvd = BSP_Pvd_Check();

}

uint32_t BSP_OscMeas(uint32_t InternOscFrequency, uint8_t u8NbLoop)
{
#define LSI_TIMEOUT                 ((uint32_t)0xFFFFFF)

	uint32_t meas = 0;


	uint8_t ratio = 4; // IcPSC / (PSC + 1)
	uint32_t u32Scale = ratio * InternOscFrequency;

	Tim16_Init(TIM_TIM16_TI1_LSI, 0, TIM_ICPSC_DIV4);
	meas = _do_avg_meas_(TIM_IC_FREQ, u8NbLoop, u32Scale, LSI_TIMEOUT);
	// if meas = 0, an error occurs

	Tim16_DeInit();
	/* Return the LSI frequency or 0 if an error occurs*/
	return (uint32_t)(meas);
}




#pragma GCC pop_options


// Possible output : (AF14)
// CH1N PB6
static void Tim16_Cfg(uint16_t u16RMP, uint16_t u16PSC, uint16_t u16IcPSC)
{
	// Initialize TIM16
	register TIM_TypeDef *TIMx = TIM16;
	TIMx->ARR = 0xFFFF;
	TIMx->PSC = u16PSC;
	// update psc and arr
	TIMx->EGR = TIM_EGR_UG;
	// Setup remap
	TIMx->OR1 = (u16RMP & TIM16_OR1_TI1_RMP);
	TIMx->OR2 = 0x0;
	// Disable counting
	TIMx->CCER = 0x0;
	// Configure IC1
	TIMx->CCMR1 = TIM_CCMR1_CC1S_0 | (u16IcPSC & TIM_CCMR1_IC1PSC);
    TIMx->CCER = TIM_ICPOLARITY_RISING;
}

static void Tim16_Init(uint16_t u16RMP, uint16_t u16PSC, uint16_t u16IcPSC)
{
	// Force Reset and Enable clock
	__HAL_RCC_TIM16_FORCE_RESET();
	__HAL_RCC_TIM16_CLK_ENABLE();
	__HAL_RCC_TIM16_RELEASE_RESET();

	Tim16_Cfg(u16RMP, u16PSC, u16IcPSC);
}


static void Tim16_DeInit(void)
{
	__HAL_RCC_TIM16_FORCE_RESET();
	__HAL_RCC_TIM16_RELEASE_RESET();
	__HAL_RCC_TIM16_CLK_DISABLE();
}
#endif

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

// Autoreload enable
#define	Tim1_CR1 TIM_AUTORELOAD_PRELOAD_ENABLE
// Master mode is "Update"
#define	Tim1_CR2 TIM_TRGO_UPDATE
// OC4 value
#define	Tim1_CCR4 0
// Trigger on ITR0, External Clock 1, (TIM15
#define	Tim1_SMCR (TIM_TS_ITR0 | TIM_SLAVEMODE_EXTERNAL1)

// Trigger on ITR1, External Clock 1, (TIM2
#define	x_Tim1_SMCR (TIM_TS_ITR1 | TIM_SLAVEMODE_EXTERNAL1)


// Channel 4 as Output, OC4 is in toggle mode
#define	Tim1_CCMR2 (TIM_OCMODE_TOGGLE << 8) | TIM_CCMR2_OC4PE
// Output Compare OC4 is enable
#define	Tim1_CCER (TIM_OUTPUTSTATE_ENABLE << 12)

// Possible output : (AF1)
// CH2 PA9
// CH3 PA10
// CH4 PA11
static void Tim1_Cfg(uint16_t u16ARR, uint16_t u16PSC)
{
	// Initialize TIM1
	register TIM_TypeDef *TIMx = TIM1;
	TIMx->CR1 = Tim1_CR1;
	TIMx->CR2 = Tim1_CR2;
	TIMx->ARR = u16ARR;
	TIMx->PSC = u16PSC;
	TIMx->CCR4 = Tim1_CCR4;
	TIMx->SMCR = Tim1_SMCR;
	TIMx->CCMR2 = Tim1_CCMR2;
	TIMx->CCER = Tim1_CCER;
	TIMx->OR2 = 0;
	TIMx->OR3 = 0;
	TIMx->BDTR |= (TIM_BDTR_MOE);
}

static void Tim1_Init(uint16_t u16ARR, uint16_t u16PSC)
{
	// Force Reset and Enable clock
	__HAL_RCC_TIM1_FORCE_RESET();
	__HAL_RCC_TIM1_CLK_ENABLE();
	__HAL_RCC_TIM1_RELEASE_RESET();
	Tim1_Cfg(u16ARR, u16PSC);
	// register TIM_TypeDef *TIMx = TIM1;
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

// Possible output : (AF2)
// CH1 PB4
// CH2 PB5
static void Tim3_Cfg(uint16_t u16ARR, uint16_t u16PSC)
{
	// Initialize TIM3
	register TIM_TypeDef *TIMx = TIM3;
	TIMx->CR1 = Tim3_CR1;
	TIMx->CR2 = Tim3_CR2;
	TIMx->ARR = u16ARR;
	TIMx->PSC = u16PSC;
	TIMx->SMCR = Tim3_SMCR;

	TIMx->OR1 = 0;
	TIMx->OR2 = 0;
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

// Possible output : (AF1)
// CH1 PA0, PA15
// CH2 PA1, PB3
// CH3 PB10
// CH4 PB11
static void Tim2_Cfg(uint16_t u16ARR, uint16_t u16PSC)
{
	(void)u16ARR;
	// Initialize TIM2
	register TIM_TypeDef *TIMx = TIM2;
	TIMx->CR1 = Tim2_CR1;
	TIMx->CR2 = Tim2_CR2;
	//TIMx->ARR = u16ARR;
	TIMx->PSC = u16PSC;
	TIMx->SMCR = Tim2_SMCR;
	TIMx->CCMR1 = Tim2_CCMR1;

	TIMx->CCER = Tim2_CCER;
	TIMx->OR1 = Tim2_OR1;
	TIMx->OR2 = 0;
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
