/**
  * @file: auto_cal.c
  * @brief: // TODO This file ...
  * 
  * @details
  *
  * @copyright 2025, GRDF, Inc.  All rights reserved.
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
  * @par 1.0.0 : 2025/02/06[GBI]
  * Initial version
  *
  *
  */

/*! @addtogroup autocal
 *  @ingroup extra
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "auto_cal.h"
#include "auto_clk.h"
#include "ext_ref_clk.h"

#include <bsp_clk.h>
#include <bsp_gpio.h>

#include <stm32l4xx_hal.h>

/*******************************************************************************/
extern struct sys_info_s g_xSysInfo;

/*******************************************************************************/

/*!
 * @cond INTERNAL
 * @{
 */

enum meas_type_e {
	TIM_IC_FREQ = 0,
	TIM_CK_FREQ = 1,
};

enum cc_state_e {
	CC_STATE_UNK       = ((uint8_t) 0x00),
	CC_STATE_START     = ((uint8_t) 0x01),
	CC_STATE_ONGOING   = ((uint8_t) 0x02),
	CC_STATE_COMPLETED = ((uint8_t) 0x03),
};

enum tim_id_e {
	TIM_ID_TIM1,
	TIM_ID_TIM16,
};

struct tim_cc_s {
	uint32_t meas;
	volatile uint8_t state;
	void (*run)(uint8_t bStart);
	void (*complete)(uint32_t meas);
};

static struct tim_cc_s tim_cc[2];

/*******************************************************************************/

static uint32_t _avg_meas_(
							enum tim_id_e eTimId,
							uint8_t avg_nb,
							uint8_t u8Type,
							uint32_t u32Scale,
							uint32_t timeout);

static void _update_(enum tim_id_e eTimId, uint32_t u32Value);

static uint32_t _pvd_check(void);

/*******************************************************************************/

void TIM1_CC_IRQHandler(void);
void TIM1_UP_TIM16_IRQHandler(void);

static void tim1_run(uint8_t bStart);
static void tim16_run(uint8_t bStart);

static void Tim16_Cfg(uint16_t u16RMP, uint16_t u16PSC, uint16_t u16IcPSC);
static void Tim16_Init(uint16_t u16RMP, uint16_t u16PSC, uint16_t u16IcPSC);
static void Tim16_DeInit(void);

static void Tim1_Cfg(uint16_t u16ARR, uint16_t u16PSC, uint8_t channel);
static void Tim1_Init(uint16_t u16ARR, uint16_t u16PSC, uint8_t channel);
static void Tim1_DeInit(void);
static void Tim3_Cfg(uint16_t u16ARR, uint16_t u16PSC);
static void Tim3_Init(uint16_t u16ARR, uint16_t u16PSC);
static void Tim3_DeInit(void);

/******************************************************************************/

void AutoCal_Init(void)
{
	// Configure the NVIC for TIMx
	//HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 0, 1);
	HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 5, 0);
	// Disable the TIMx global Interrupt
	HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);
	// Configure the NVIC for TIMx
	//HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 1);
	HAL_NVIC_SetPriority(TIM1_CC_IRQn, 5, 0);
	// Disable the TIMx global Interrupt
	HAL_NVIC_DisableIRQ(TIM1_CC_IRQn);

	tim_cc[TIM_ID_TIM1].run = tim1_run;
	tim_cc[TIM_ID_TIM1].complete = NULL;

	tim_cc[TIM_ID_TIM16].run = tim16_run;
	tim_cc[TIM_ID_TIM16].complete = NULL;

	g_xSysInfo.pvd = _pvd_check();
	g_xSysInfo.lsi = -1;
	g_xSysInfo.lse = -1;
	g_xSysInfo.hsi = -1;
	g_xSysInfo.hse = -1;
	g_xSysInfo.msi = -1;
	g_xSysInfo.state = 0;
}

//#pragma GCC push_options
//#pragma GCC optimize("O0")

#define _NB_LOOP ((uint8_t)10)
#define _TIMEOUT ((uint32_t)0xFFFFFF)
#define _TIM_CK_FREQ ((uint32_t)16000000) // HSI16


int32_t AutoCal_LSIMeas(uint32_t u32RefFreq, uint8_t u8NbLoop)
{
	int32_t meas = -1;
	uint8_t ratio = 4; // IcPSC / (PSC + 1)
	uint32_t u32Scale = u32RefFreq * ratio;

	Tim16_Init(TIM_TIM16_TI1_LSI, 0, TIM_ICPSC_DIV4);
	meas = (int32_t)_avg_meas_(TIM_ID_TIM16, u8NbLoop, TIM_IC_FREQ, u32Scale, _TIMEOUT);
	Tim16_DeInit();

	// if meas = 0, an error occurs
	if (meas == 0)
	{
		meas = -1;
	}
	g_xSysInfo.lsi = meas;
	return 0;
}

int32_t AutoCal_HSEMeas(uint32_t u32RefFreq, uint8_t u8NbLoop)
{
	int32_t meas = -1;
	uint8_t ratio = 4; // IcPSC / (PSC + 1)
	uint32_t u32Scale = u32RefFreq / ratio;

	Tim16_Init(TIM_TIM16_TI1_LSI, 0, TIM_ICPSC_DIV4);
	meas = (int32_t)_avg_meas_(TIM_ID_TIM16, u8NbLoop, TIM_CK_FREQ, u32Scale, _TIMEOUT);
	Tim16_DeInit();

	// if meas = 0, an error occurs
	if (meas == 0)
	{
		meas = -1;
	}
	g_xSysInfo.hse = meas;
	return 0;
}

//#pragma GCC pop_options

int32_t AutoCal_SysMeas(uint32_t u32RefFreq, uint8_t u8NbLoop)
{
	int32_t meas = -1;
	uint8_t ratio = 2; // IcPSC / (PSC + 1)
	uint32_t u32Scale = u32RefFreq / ratio;

	Tim1_Init(0xFFFF, 0, 1);
	meas = (int32_t)_avg_meas_(TIM_ID_TIM1, u8NbLoop, TIM_CK_FREQ, u32Scale, _TIMEOUT);
	Tim1_DeInit();

	// if meas = 0, an error occurs
	if (meas == 0)
	{
		meas = -1;
	}
	g_xSysInfo.sys = meas;
	return 0;
}

/******************************************************************************/
uint8_t _done_;
static void _complete_(uint32_t meas)
{
	_done_ = 1;
}

static
uint32_t _avg_meas_(enum tim_id_e eTimId, uint8_t avg_nb, uint8_t u8Type, uint32_t u32Scale, uint32_t timeout)
{
	uint32_t avg = 0;
	uint8_t loop_cnt = 0;

	struct tim_cc_s *pTimCC = &tim_cc[eTimId];

	if ( (eTimId != TIM_ID_TIM1) && (eTimId != TIM_ID_TIM16) )
	{
		// Error
		goto done;
	}

	if (pTimCC->run == NULL)
	{
		// Error
		goto done;
	}

	while (loop_cnt <= avg_nb)
	{
		uint32_t tmo = timeout;
		// --------------------------------------------------------------------
		pTimCC->state = CC_STATE_START;
		pTimCC->run(1);
		// --------------------------------------------------------------------
		// Wait for end of capture: two consecutive captures
		while ((pTimCC->state != CC_STATE_COMPLETED) && (tmo != 0))
		{
			if (--tmo == 0)
			{
				break;
			}
		}

		// --------------------------------------------------------------------
		pTimCC->run(0);

		// --------------------------------------------------------------------
		// Timeout
		if ( (tmo == 0) || (pTimCC->meas == 0))
		{
			// Error
			avg = 0;
			goto done;
		}

		// --------------------------------------------------------------------
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
				avg += (u32Scale * pTimCC->meas);
			}
			else // type == TIM_IC_FREQ
			{
				avg += (u32Scale / pTimCC->meas);
			}

		}
	    /* Increment counter */
		loop_cnt++;
	}

	/* Compute the average of frequency value */
	if (avg_nb)
	{
		avg = (uint32_t) (avg / avg_nb);
	}
done :
	return (uint32_t)(avg);
}

/******************************************************************************/

static const struct gpio_id_s gpio_cfg_ref[3] =
{
	{.port = GPIO_PORT_ID_A, .pin = GPIO_PIN_ID_9},  // TIM1_CH2
	{.port = GPIO_PORT_ID_A, .pin = GPIO_PIN_ID_10}, // TIM1_CH3
	{.port = GPIO_PORT_ID_A, .pin = GPIO_PIN_ID_11}, // TIM1_CH4
};

static const struct iomux_s io_cfg_ref[3] =
{
	// Choice of out clock from TIM1
	{ IOMUX_INIT(GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM, GPIO_MODE_AF_PP, 0, GPIO_AF1_TIM1) }, // PA9  IO6 TIM1_CH2
	{ IOMUX_INIT(GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM, GPIO_MODE_AF_PP, 0, GPIO_AF1_TIM1) }, // PA10 IO5 TIM1_CH3
	{ IOMUX_INIT(GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM, GPIO_MODE_AF_PP, 0, GPIO_AF1_TIM1) }, // PA11 IO4 TIM1_CH4
};

/*
 * OutFreq = InFreq x
 *              1                  1
 *     ( --------------- x ------------------- )
 *       (TIM1_PSC + 1)     (TIM1_ARR + 1) / 2
 *
 */
static tim_period_t aTimSetup[2] =
{
	TMR_SETUP(0xFFFF, 0),
	TMR_SETUP(1,      0),
};

/**
  * @brief  This function .
  *
  *
  * If selected, output frequency (OutFreq) can be mapped on one of PA9, PA10 or PA11
  *
  * @retval HAL status
  */
void AutoCal_Tmr_Enable(uint8_t bOCxEnable)
{
	uint8_t id = bOCxEnable & 0x03;
	// Init Timer 1
	if (id == 0) // for measurement
	{
		Tim1_Init(aTimSetup[0].arr, aTimSetup[0].psc, 1);
	}
	else // for output on channel id + 1
	{
		Tim1_Init(aTimSetup[1].arr, aTimSetup[1].psc, id + 1);
		// Gpio output cfg from TIM1 OCx
		if (id)
		{
			id--;
			BSP_Gpio_Config(gpio_cfg_ref[id], io_cfg_ref[id].io);
		}
		// Enable Timers
		TIM1->CR1 |= (TIM_CR1_CEN);
	}
}

void AutoCal_Tmr_Disable(uint8_t bOCxEnable)
{
	// DeInit Timer
	Tim1_DeInit();
	// DeInit Gpio
	uint8_t id = bOCxEnable & 0x03;
	struct iomux_s iomux = {IOMUX_DEFAULT_ANALOG()};

	if (id)
	{
		id--;
		BSP_Gpio_Config(gpio_cfg_ref[id], iomux.io);
	}
}

/******************************************************************************/
/******************************************************************************/

static void _update_(enum tim_id_e eTimId, uint32_t u32Value)
{
	struct tim_cc_s *pTimCC = &tim_cc[eTimId];
	if (pTimCC->state == CC_STATE_START)
	{
		// Get the 1st Input Capture value
		pTimCC->meas = u32Value;
		pTimCC->state = CC_STATE_ONGOING;
	}
	else if (pTimCC->state == CC_STATE_ONGOING)
	{
		pTimCC->state = CC_STATE_COMPLETED;

		// Capture computation
		if (u32Value > pTimCC->meas)
		{
			pTimCC->meas = (u32Value - pTimCC->meas);
		}
		else if (u32Value < pTimCC->meas)
		{
			pTimCC->meas = ((0xFFFF - u32Value) + pTimCC->meas);
		}
		else
		{
			// If capture values are equal, we have reached the limit of frequency measures
			// Error
			pTimCC->meas = 0;
		}

		if (pTimCC->complete)
		{
			pTimCC->complete(pTimCC->meas);
		}
	}
}

/******************************************************************************/

static
uint32_t _pvd_check(void)
{
	uint32_t state = 0;
	uint8_t below = 0;

	// Detect / Measure VDDA
	// PVM4 : VDDA vs 1.80V
	// PVM3 : VDDA vs 1.62V (min. value)
	// if VDDA < PVM3 and ADC required, set SYSCFG_CFGR1 BOOSTEN = 1
	WRITE_REG(PWR->CR2, (PWR_CR2_PVME3 | PWR_CR2_PVME4 ));
	if ( (READ_BIT(PWR->SR2, PWR_SR2_PVMO3)) == PWR_SR2_PVMO3)
	{
		// VDDA is below the selected PVM3 threshold
		state |= 0x10;
	}
	else if ( (READ_BIT(PWR->SR2, PWR_SR2_PVMO4)) == PWR_SR2_PVMO4 )
	{
		// VDDA is below the selected PVM3 threshold
		state |= 0x20;
	}
	else
	{
		// VDDA is above PWR_SR2_PVMO4 or missing
		state |= 0x40;
	}


	// Detect / Measure VDD
	/*
	 * Note :
	 * PWR_CR2_PLS = 7, compare VDD to VREFINT
	 * VREFINT typ. 1.212V; 1.182V @-40°C; 1.232V @+130°C
	 */
	uint8_t i;
	for (i = 6; i <= 0; i--)
	{
		WRITE_REG(PWR->CR2, (i << PWR_CR2_PLS_Pos) );
		SET_BIT(PWR->CR2, PWR_CR2_PVDE);
		if ( (READ_BIT(PWR->SR2, PWR_SR2_PVDO)) == PWR_SR2_PVDO)
		{
			// VDD is below the selected PVD threshold
			// So real  PLS(i) < VDD < PLS(i + 1)
			below = 1;
			break;
		}
		// else { // VDD is above the selected PVD threshold }
		CLEAR_BIT(PWR->CR2, PWR_CR2_PVDE);
	}

	/*
	 * Here : real VDD is : PLS(i) < VDD < PLS(i + 1)
	 * Except at limits :
	 * - below = 0 and i == 0, so PLS(6) < VDD
	 * - below = 1           , so PLS(i) < VDD < PLS(i + 1)
	 * - below = 1 and i == 0, so          VDD < PLS(0)
	 *
	*/
	state |= (below)?(6):(i);

	return state;
}

/******************************************************************************/

void TIM1_CC_IRQHandler(void)
{
	if ((TIM1->SR & TIM_FLAG_CC1) == TIM_FLAG_CC1)
	{
		if ((TIM1->DIER & TIM_IT_CC1) == TIM_IT_CC1)
		{
			TIM1->SR = 0;
			if ( TIM1->CCMR1 & TIM_CCMR1_CC1S )
			{
				_update_(TIM_ID_TIM1, (uint32_t)(TIM1->CCR1));
			}
		}
	}
}

void TIM1_UP_TIM16_IRQHandler(void)
{
	if ((TIM16->SR & TIM_FLAG_CC1) == TIM_FLAG_CC1)
	{
		if ((TIM16->DIER & TIM_IT_CC1) == TIM_IT_CC1)
		{
			TIM16->SR = 0;
			//if ( (TIM16->CCMR1 & TIM_CCMR1_CC1S) == TIM_CCMR1_CC1S_0)
			if (TIM16->CCMR1 & TIM_CCMR1_CC1S)
			{
				_update_(TIM_ID_TIM16, (uint32_t)(TIM16->CCR1));
			}
		}
	}
}

/******************************************************************************/

static void tim1_run(uint8_t bStart)
{
	// --------------------------------------------------------------------
	if (bStart)
	{
		// Enable IT
		TIM1->DIER = (TIM_DIER_CC1IE);
		// Enable capture on IC1
		TIM1->CCER |= TIM_CCER_CC1E;
		// Enable counting
		TIM1->CR1 |= (TIM_CR1_CEN);
		/* Enable the TIMx IRQ channel */
		HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
	}
	else
	{
		/* Disable the TIMx global Interrupt */
		HAL_NVIC_DisableIRQ(TIM1_CC_IRQn);

		// This will/should be done in TimX_DeInit:
		// - Disable IT, Disable capture on IC1, Disable counting
		// Disable counting
		TIM1->CR1 &= ~(TIM_CR1_CEN);
		// Disable IT
		TIM1->DIER = 0 ; //&= ~(TIM_DIER_CC1IE);
		// Disable capture on IC1
		TIM1->CCER &= ~(TIM_CCER_CC1E);
		//
		TIM16->SR = 0;
	}
}

static void tim16_run(uint8_t bStart)
{
	// --------------------------------------------------------------------
	if (bStart)
	{
		// Enable IT
		TIM16->DIER = (TIM_DIER_CC1IE);
		// Enable capture on IC1
		TIM16->CCER |= TIM_CCER_CC1E;
		// Enable counting
		TIM16->CR1 |= (TIM_CR1_CEN);
		/* Enable the TIMx IRQ channel */
		HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
	}
	else
	{
		/* Disable the TIMx global Interrupt */
		HAL_NVIC_DisableIRQ(TIM1_UP_TIM16_IRQn);

		// This will/should be done in TimX_DeInit:
		// - Disable IT, Disable capture on IC1, Disable counting
		/*
		// Disable IT
		TIM16->DIER &= ~(TIM_DIER_CC1IE);
		// Disable capture on IC1
		TIM16->CCER &= ~(TIM_CCER_CC1E);
		// Disable counting
		TIM16->CR1 &= ~(TIM_CR1_CEN);
		*/
	}
}

/******************************************************************************/
/******************************************************************************/

// Possible output : (AF14)
// CH1N PB6
static void Tim16_Cfg(uint16_t u16RMP, uint16_t u16PSC, uint16_t u16IcPSC)
{
	// Initialize TIM16
	register TIM_TypeDef *TIMx = TIM16;
	// Setup ARR and PSC
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

/******************************************************************************/

// Autoreload enable
#define	Tim1_CR1 TIM_AUTORELOAD_PRELOAD_ENABLE
// Master mode is "Update"
#define	Tim1_CR2 TIM_TRGO_UPDATE
// OCx value
#define	Tim1_CCRx 0
// ITR0 selected (TIM15), External Clock 1
#define	Tim1_SMCR_OCx (TIM_TS_ITR0 | TIM_SLAVEMODE_EXTERNAL1)

// ITR0 selected (TIM15), Slave mode disable, External Clock 1,
#define	Tim1_SMCR_Meas (TIM_TS_ITR0)


// Trigger on ITR1, External Clock 1, (TIM2
#define	x_Tim1_SMCR (TIM_TS_ITR1 | TIM_SLAVEMODE_EXTERNAL1)


// Channel 2 as Output, OC2 is in toggle mode
#define	Tim1_CCMR1_CH2 (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_0) | TIM_CCMR1_OC2PE
// Output Compare OC2 is enable
#define	Tim1_CCER_CH2 TIM_CCER_CC2E

// Channel 3 as Output, OC3 is in toggle mode
#define	Tim1_CCMR2_CH3 (TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_0) | TIM_CCMR2_OC3PE
// Output Compare OC3 is enable
#define	Tim1_CCER_CH3 TIM_CCER_CC3E

// Channel 4 as Output, OC4 is in toggle mode
#define	Tim1_CCMR2_CH4 (TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_0) | TIM_CCMR2_OC4PE
// Output Compare OC4 is enable
#define	Tim1_CCER_CH4 TIM_CCER_CC4E


// Channel 1 as IC on TRC
#define	Tim1_CCMR1_CH1 TIM_CCMR1_CC1S
// Input Capture IC1 is enable
#define	Tim1_CCER_CH1 TIM_CCER_CC1E

// Possible output : (AF1)
// CH2 PA9
// CH3 PA10
// CH4 PA11
static void Tim1_Cfg(uint16_t u16ARR, uint16_t u16PSC, uint8_t channel)
{
	// Initialize TIM1
	register TIM_TypeDef *TIMx = TIM1;
	TIMx->CR1 = Tim1_CR1;
	TIMx->CR2 = Tim1_CR2;
	TIMx->PSC = u16PSC;

	TIMx->ARR = u16ARR;
	TIMx->SMCR = Tim1_SMCR_OCx;

	TIMx->OR2 = 0;
	TIMx->OR3 = 0;
	TIMx->BDTR = (TIM_BDTR_MOE);

	switch (channel)
	{
		case 2:
			TIMx->CCMR1 = Tim1_CCMR1_CH2;
			TIMx->CCER  = Tim1_CCER_CH2;
			break;
		case 3:
			TIMx->CCMR2 = Tim1_CCMR2_CH3;
			TIMx->CCER  = Tim1_CCER_CH3;
			break;
		case 4:
			TIMx->CCMR2 = Tim1_CCMR2_CH4;
			TIMx->CCER  = Tim1_CCER_CH4;
			break;
		case 1: // Measurement
		default:
			//TIMx->PSC = 0xFFFF;
			TIMx->SMCR = Tim1_SMCR_Meas;
			TIMx->CCMR1 = Tim1_CCMR1_CH1 | TIM_ICPSC_DIV4; // FIXME ?
			//TIMx->DIER  = TIM_DIER_CC1IE;
			//TIMx->CCER  = Tim1_CCER_CH1;
			break;
	}
}

static void Tim1_Init(uint16_t u16ARR, uint16_t u16PSC, uint8_t channel)
{
	// Force Reset and Enable clock
	__HAL_RCC_TIM1_FORCE_RESET();
	__HAL_RCC_TIM1_CLK_ENABLE();
	__HAL_RCC_TIM1_RELEASE_RESET();
	Tim1_Cfg(u16ARR, u16PSC, channel);
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

/*******************************************************************************/

/*!
 * @}
 * @endcond
 */

/*******************************************************************************/

#ifdef __cplusplus
}
#endif

/*! @} */
