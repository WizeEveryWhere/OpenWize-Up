
/**
  * @file: bsp_clk.c
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
  * 1.0.0 : 2025/01/29[TODO: your name]
  * Initial version
  *
  *
  */
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_clk.h"
#include "platform.h"

#include <stm32l4xx_hal.h>
#include "errata.h"

/******************************************************************************/
__attribute__((weak))
uint32_t BSP_Clk_GetUartCfg(uint8_t u8DevId)
{
	union peripheral_cfg_u cfg = {
		.baud = SERIAL_BAUD,
		.swap = SERIAL_SWAP,
		.enable = 0,
	};
	return cfg.peripheral_cfg;
}



/******************************************************************************/
// external
extern void Error_Handler(void);

/******************************************************************************/
// Static

static HAL_StatusTypeDef _wait_LSE_state_(uint8_t bState);
static HAL_StatusTypeDef _wait_LSI_state_(uint8_t bState);
static HAL_StatusTypeDef _wait_HSE_state_(uint8_t bState);
static HAL_StatusTypeDef _wait_HSI_state_(uint8_t bState);
static HAL_StatusTypeDef _wait_MSI_state_(uint8_t bState);
static HAL_StatusTypeDef _wait_PLL_state_(uint8_t bState);

static HAL_StatusTypeDef _wait_RCC_BDCR_state_(uint8_t bState);
static HAL_StatusTypeDef _wait_RCC_SWS_state_(uint8_t u8State);

/*----------------------------------------------------------------------------*/

static void _set_peripheral_clk_(uint32_t u32Peripheral, uint8_t u8BaseClk);

/******************************************************************************/
// Global
struct sys_info_s g_xSysInfo;
struct pll_fact_s g_xPllFact = {.pll_fact = 0};
/******************************************************************************/
#ifndef LSI_TIMEOUT_VALUE
#define LSI_TIMEOUT_VALUE 2U
#endif

#ifndef MSI_TIMEOUT_VALUE
#define MSI_TIMEOUT_VALUE 2U
#endif

#ifndef HSE_TIMEOUT_VALUE
#define HSE_TIMEOUT_VALUE HSE_STARTUP_TIMEOUT
#endif

#ifndef HSI_TIMEOUT_VALUE
#define HSI_TIMEOUT_VALUE 2U
#endif

#ifndef PLL_TIMEOUT_VALUE
#define PLL_TIMEOUT_VALUE 2U
#endif

#ifndef CLOCKSWITCH_TIMEOUT_VALUE
#define CLOCKSWITCH_TIMEOUT_VALUE  5000U /* 5 s    */
#endif

#ifdef ERRATA_ES0387_REV11_2_2_8
extern void ES0387_REV11_2_2_8(void);
#endif

/******************************************************************************/
// Main PLL related static function

#define _GET_MIN(_target, _tol) (_target - _tol)
#define _GET_MAX(_target, _tol) (_target + _tol)
#define _IS_TARGET_RANGE(_T, _target, _tol) \
	( \
		(_T >= _GET_MIN(_target, _tol) ) \
		&& \
		(_T <= _GET_MAX(_target, _tol)) \
	)?(1):(0)

#ifndef TARGET_FREQ
#define TARGET_FREQ  48000000
#endif

#ifndef TARGET_TOL
#define TARGET_TOL    1920000 // 4%
#endif

#define TARGET_MIN  _GET_MIN(TARGET_FREQ, TARGET_TOL)
#define TARGET_MAX  _GET_MAX(TARGET_FREQ, TARGET_TOL)
#define IS_TARGET_RANGE(_T) ( (_T >= TARGET_MIN) && (_T <= TARGET_MAX) )?(1):(0)

#define VCO_IN_MIN    4000000
#define VCO_IN_MAX   16000000
#define VCO_OUT_MIN  64000000
#define VCO_OUT_MAX 344000000
#define M_MIN 1
#define M_MAX 8
#define M_INC(_M) _M++
#define N_MIN 8
#define N_MAX 86
#define N_INC(_N) _N++
#define R_MIN 2
#define R_MAX 8
#define R_INC(_R) _R+=2
#define IS_VCO_IN_RANGE(_VCO_IN)  ( (_VCO_IN  >= VCO_IN_MIN)  && (_VCO_IN  <= VCO_IN_MAX)  )?(1):(0)
#define IS_VCO_OUT_RANGE(_VCO_OUT)( (_VCO_OUT >= VCO_OUT_MIN) && (_VCO_OUT <= VCO_OUT_MAX) )?(1):(0)

/*----------------------------------------------------------------------------*/

static uint8_t _check_frequency_(uint32_t frequency, uint32_t tolerance)
{
	if (_IS_TARGET_RANGE(frequency, TARGET_FREQ, tolerance))
	{
		// perfect match or acceptable range
		return 1;
	}
	else
	{
		return 0;
	}
}

/*----------------------------------------------------------------------------*/

static uint32_t _find_N_R_(uint32_t vco_in, uint32_t tolerance)
{
	uint32_t vco_out;
	uint32_t freq_out;

	struct pll_fact_s pll = {0};

	pll.R = R_MIN;
	while (pll.R <= R_MAX)
	{
		pll.N = N_MIN;
		while (pll.N <= N_MAX)
		{
			vco_out = vco_in * pll.N;
			if (IS_VCO_OUT_RANGE(vco_out))
			{
				freq_out = vco_out / pll.R;
				if (_check_frequency_(freq_out, tolerance))
				{
					return pll.pll_fact;
				}
				if (freq_out > 80000000)
				{
					break;
				}
			}
			N_INC(pll.N);
		}
		R_INC(pll.R);
	}
	return 0;
}

/*----------------------------------------------------------------------------*/

static uint32_t _find_N_R_M_(uint32_t hse, uint32_t tolerance)
{
	struct pll_fact_s NRM;

	uint8_t M = M_MIN;
	do {
		uint32_t vco_in = hse / M;

		if ( IS_VCO_IN_RANGE(vco_in) )
		{
			NRM.pll_fact = _find_N_R_(vco_in, tolerance);
			if (NRM.pll_fact != 0)
			{
				//
				NRM.M = M;
				if (tolerance == 0)
				{
					NRM.exact = 1;
				}
				return NRM.pll_fact;
			}
		}
		M_INC(M);

		if (M > M_MAX)
		{
			// error ?
			break;
		}
	} while(1);
	return 0;
}

/*----------------------------------------------------------------------------*/

static uint32_t _get_PLL_factor_(uint32_t u32HseXtalValue)
{
	/*
	 * 1 <= M <= 8
	 * 8 <= N <= 86
	 * 2 <= R <= 8
	 * 4MHz <= OSC_IN x 1/M <= 16MHz
	 * 64MHz <= OSC_IN x 1/M x N <= 344MHz
	 * Target frequency is : OSC_IN x 1/M x N x 1/R = 48MHz +/- tol.
	 *
	 * Some examples : (manually computed)
	 * OSC_IN(MHz) PLL M, N, R
	 * Exact match
	 * 4   1  24  2
	 * 6   1  16  2
	 * 8   1  12  2
	 * 12  1   8  2
	 * 16  2  24  4
	 * 18  2  32  6
	 * 24  2  16  4
	 * 32  2  12  4
	 * 36  3  16  4
	 * 48  2   8  4
	 *
	 * Best for "not exact"
	 * *10  2  58  6  48333.333
	 * *20  2  29  6  48333.333
	 * *22  2  26  6  47666.666
	 * *26  2  22  6  47666.666
	 *
	 * *28  3  21  4  49000
	 * *30  3  19  4  47500
	 * *34  3  17  4  48166.666
	 *
	 * *38  3  15  4  47500
	 * *40  3  14  4  46666.666
	 * *42  3  14  4  49000
	 * *44  3  13  4  47666.666
	 * *46  3  13  4  49833.333
	 *
	*/
	uint32_t tolerance = 0; // or TARGET_TOL
	uint32_t NRM;

	do {
		NRM = _find_N_R_M_(u32HseXtalValue, tolerance);
		if (NRM == 0)
		{
			if (tolerance == 0)
			{
				tolerance = TARGET_TOL;
			}
			else
			{
				// no acceptable PLL factor have been found
				break;
			}
		}
		else
		{
			// exact or acceptable PLL factor have been found
			break;
		}
	} while (1);

	return NRM;
}

/******************************************************************************/
// Wait Oscillator static functions

static HAL_StatusTypeDef _wait_LSE_state_(uint8_t bState)
{
	uint32_t tickstart;
	uint32_t state = (bState)?(RCC_BDCR_LSERDY):(0);
	// Get Start Tick
	tickstart = HAL_GetTick();
	// Wait until LSE is ready
	while (READ_BIT(RCC->BDCR, RCC_BDCR_LSERDY) != state)
	{
		if ((HAL_GetTick() - tickstart) > RCC_LSE_TIMEOUT_VALUE)
		{
			return HAL_TIMEOUT;
		}
	}
	return HAL_OK;
}

/*----------------------------------------------------------------------------*/

static HAL_StatusTypeDef _wait_LSI_state_(uint8_t bState)
{
	uint32_t tickstart;
	uint32_t state = (bState)?(RCC_CSR_LSIRDY):(0);
	// Get Start Tick
	tickstart = HAL_GetTick();
    // Wait until LSI is disabled
    while (READ_BIT(RCC->CSR, RCC_CSR_LSIRDY) != state)
    {
      if ((HAL_GetTick() - tickstart) > LSI_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }
	return HAL_OK;
}

/*----------------------------------------------------------------------------*/

static HAL_StatusTypeDef _wait_HSE_state_(uint8_t bState)
{
	uint32_t tickstart;
	uint32_t state = (bState)?(RCC_CR_HSERDY):(0);
    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    /* Wait until HSE is ready */
    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) != state)
    {
		if ((HAL_GetTick() - tickstart) > HSE_TIMEOUT_VALUE)
		{
			return HAL_TIMEOUT;
		}
    }
    return HAL_OK;
}

/*----------------------------------------------------------------------------*/

static HAL_StatusTypeDef _wait_HSI_state_(uint8_t bState)
{
	uint32_t tickstart;
	uint32_t state = (bState)?(RCC_CR_HSIRDY):(0);
    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    /* Wait until HSI is ready */
    while (READ_BIT(RCC->CR, RCC_CR_HSIRDY) != state)
    {
		if ((HAL_GetTick() - tickstart) > HSI_TIMEOUT_VALUE)
		{
			return HAL_TIMEOUT;
		}
    }
    return HAL_OK;
}

/*----------------------------------------------------------------------------*/

static HAL_StatusTypeDef _wait_MSI_state_(uint8_t bState)
{
	uint32_t tickstart;
	uint32_t state = (bState)?(RCC_CR_MSIRDY):(0);
    /* Get timeout */
    tickstart = HAL_GetTick();

    /* Wait until MSI is ready */
    while (READ_BIT(RCC->CR, RCC_CR_MSIRDY) != state)
    {
		if ((HAL_GetTick() - tickstart) > MSI_TIMEOUT_VALUE)
		{
			return HAL_TIMEOUT;
		}
    }
    return HAL_OK;
}

/*----------------------------------------------------------------------------*/

static HAL_StatusTypeDef _wait_PLL_state_(uint8_t bState)
{
	uint32_t tickstart;
	uint32_t state = (bState)?(RCC_CR_PLLRDY):(0);
    /* Get timeout */
    tickstart = HAL_GetTick();

    /* Wait until PLL is ready */
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) != state)
    {
		if ((HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
		{
			return HAL_TIMEOUT;
		}
    }
    return HAL_OK;
}

/*----------------------------------------------------------------------------*/

static HAL_StatusTypeDef _wait_RCC_BDCR_state_(uint8_t bState)
{
	uint32_t tickstart;
	uint32_t state = (bState)?(PWR_CR1_DBP):(0);
	// Wait for Backup domain Write protection disable/enable
	tickstart = HAL_GetTick();

	while (READ_BIT(PWR->CR1, PWR_CR1_DBP) != state)
	{
		if ((HAL_GetTick() - tickstart) > RCC_DBP_TIMEOUT_VALUE)
		{
			return HAL_TIMEOUT;
		}
	}
	return HAL_OK;
}

/*----------------------------------------------------------------------------*/

static HAL_StatusTypeDef _wait_RCC_SWS_state_(uint8_t u8State)
{
	uint32_t tickstart;
	u8State &= RCC_CFGR_SW_Msk;
    /* Get Start Tick*/
    tickstart = HAL_GetTick();

	while (__HAL_RCC_GET_SYSCLK_SOURCE() != ((uint32_t)u8State << RCC_CFGR_SWS_Pos))
	{
		if ((HAL_GetTick() - tickstart) > CLOCKSWITCH_TIMEOUT_VALUE)
		{
			return HAL_TIMEOUT;
		}
	}
    return HAL_OK;
}

/******************************************************************************/
// Enable / Disable the Backup Domain Register function

dev_res_e BSP_Clk_BDCR(uint8_t bEnable)
{
	uint8_t state;
	dev_res_e ret = DEV_SUCCESS;
	uint8_t pwrclkchanged = 0;

	// Enable Power Clock
	if (__HAL_RCC_PWR_IS_CLK_DISABLED() != 0U)
	{
		__HAL_RCC_PWR_CLK_ENABLE();
		pwrclkchanged = 1;
	}

	if (bEnable)
	{
		// Enable write access to Backup domain
		SET_BIT(PWR->CR1, PWR_CR1_DBP);
		state = 1;
	}
	else
	{
		// disable write access to Backup domain
		CLEAR_BIT(PWR->CR1, PWR_CR1_DBP);
		state = 0;
	}

	// Wait for Backup domain Write protection disable/enable
	ret = _wait_RCC_BDCR_state_(state);

	// Restore clock configuration if changed
	if (pwrclkchanged)
	{
		__HAL_RCC_PWR_CLK_DISABLE();
	}
	return ret;
}

/******************************************************************************/
// Enable / Disable Oscillator function

dev_res_e BSP_Clk_LSE(uint8_t bEnable)
{
	dev_res_e ret = DEV_SUCCESS;
	if ( (RCC->BDCR & RCC_BDCR_LSERDY) == 0)
	{
		if (bEnable)
		{
			RCC->BDCR |= RCC_BDCR_LSEON;
			ret = _wait_LSE_state_(1);
		}
	}
	else
	{
		if (!bEnable)
		{
			RCC->BDCR &= ~(RCC_BDCR_LSEON);
			ret = _wait_LSE_state_(0);
		}
	}

	return ret;
}

/*----------------------------------------------------------------------------*/

dev_res_e BSP_Clk_LSI(uint8_t bEnable)
{
	dev_res_e ret = DEV_SUCCESS;
	if ( (RCC->CSR & RCC_CSR_LSIRDY) == 0)
	{
		if (bEnable)
		{
			RCC->CSR |= RCC_CSR_LSION;
			ret = _wait_LSI_state_(1);
		}
	}
	else
	{
		if (!bEnable)
		{
			RCC->CSR &= ~(RCC_CSR_LSION);
			ret = _wait_LSI_state_(0);
		}
	}

	return ret;
}

/*----------------------------------------------------------------------------*/

dev_res_e BSP_Clk_HSE(uint8_t bEnable)
{
	dev_res_e ret = DEV_SUCCESS;
	if ( (RCC->CR & RCC_CR_HSERDY) == 0)
	{
		if (bEnable)
		{
#ifdef ERRATA_ES0387_REV11_2_2_8
			ES0387_REV11_2_2_8();
#endif
			RCC->CR |= RCC_CR_HSEON;
			ret = _wait_HSE_state_(1);
		}
	}
	else
	{
		if (!bEnable)
		{
			RCC->CR &= ~(RCC_CR_HSEON);
			ret = _wait_HSE_state_(0);
		}
	}

	return ret;
}

/*----------------------------------------------------------------------------*/

dev_res_e BSP_Clk_HSI(uint8_t bEnable)
{
	dev_res_e ret = DEV_SUCCESS;
	if ( (RCC->CR & RCC_CR_HSIRDY) == 0)
	{
		if (bEnable)
		{
			RCC->CR |= RCC_CR_HSION;
			ret = _wait_HSI_state_(1);
		}
	}
	else
	{
		if (!bEnable)
		{
			RCC->CR &= ~(RCC_CR_HSION);
			ret = _wait_HSI_state_(0);
		}
	}

	return ret;
}

/*----------------------------------------------------------------------------*/

dev_res_e BSP_Clk_MSI(uint8_t bEnable)
{
	dev_res_e ret = DEV_SUCCESS;
	if ( (RCC->CR & RCC_CR_MSIRDY) == 0)
	{
		if (bEnable)
		{
			RCC->CR |= RCC_CR_MSION;
			ret = _wait_MSI_state_(1);
		}
	}
	else
	{
		if (!bEnable)
		{
			RCC->CR &= ~(RCC_CR_MSION);
			ret = _wait_MSI_state_(0);
		}
	}

	return ret;
}

/*----------------------------------------------------------------------------*/

void BSP_Clk_OscTrim(uint32_t u32RccOscType, uint16_t u16TrimValue)
{
	if (u32RccOscType == RCC_OSCILLATORTYPE_MSI)
	{
		__HAL_RCC_MSI_CALIBRATIONVALUE_ADJUST(u16TrimValue);
	}
	else if ( u32RccOscType == RCC_OSCILLATORTYPE_HSI)
	{
		__HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(u16TrimValue);
	}
}
/*----------------------------------------------------------------------------*/

dev_res_e BSP_Clk_PLL(uint32_t u32PllSrc, int32_t src_freq, uint8_t bEnable)
{
	dev_res_e ret = DEV_SUCCESS;

	if (bEnable)
	{
		switch (u32PllSrc)
		{
			case RCC_PLLCFGR_PLLSRC_HSE:
			case RCC_PLLCFGR_PLLSRC_HSI:
				break;
			default:
				ret = DEV_FAILURE;
				break;
		}

	    if ( ret == DEV_SUCCESS)
	    {
	    	if (g_xPllFact.init == 0)
	    	{
	    		g_xPllFact.pll_fact = _get_PLL_factor_((uint32_t)src_freq);
				if (g_xPllFact.pll_fact)
				{
					g_xPllFact.init = 1;
				}
	    	}

			if (g_xPllFact.pll_fact)
			{
				// Disable the main PLL.
				RCC->CR &= ~(RCC_CR_PLLON);

				ret = _wait_PLL_state_(0);
				if ( ret  == DEV_SUCCESS)
				{
					uint32_t pllcfgr = 0;
					RCC->PLLCFGR = pllcfgr;
					uint8_t x = (g_xPllFact.R / 2) - 1;
					pllcfgr |= (g_xPllFact.M - 1 ) << RCC_PLLCFGR_PLLM_Pos;
					pllcfgr |= g_xPllFact.N << RCC_PLLCFGR_PLLN_Pos;
					pllcfgr |= x << RCC_PLLCFGR_PLLR_Pos;
					pllcfgr |= x << RCC_PLLCFGR_PLLQ_Pos;

					RCC->PLLCFGR = pllcfgr | u32PllSrc;

					// Enable the main PLL.
					RCC->CR |= (RCC_CR_PLLON);
			        // Enable PLL Clock output.
			        RCC->PLLCFGR |= (RCC_PLLCFGR_PLLREN);

					ret = _wait_PLL_state_(1);
					if (ret != DEV_SUCCESS)
					{
						// Disable PLL Clock output.
						RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLREN);
						// Disable the main PLL.
						RCC->CR &= ~(RCC_CR_PLLON);
					}
				}
			}
	    }
	}
	else
	{
		// Disable PLL Clock output.
		RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLREN);
		// Disable the main PLL.
		RCC->CR &= ~(RCC_CR_PLLON);
		ret = _wait_PLL_state_(0);

		RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLSRC);
	}
	return ret;
}
/******************************************************************************/
#ifndef TRY_AGAIN
	#define TRY_AGAIN 3
#endif

dev_res_e BSP_Clk_SetPllOsc(uint32_t u32RccOscType, uint8_t bEnable)
{
	dev_res_e ret = DEV_FAILURE;
	dev_res_e (*_enable_OSC_)(uint8_t bEnable) = NULL;
	int32_t src_freq = -1;
	uint32_t u32PllSrc = 0;
	switch (u32RccOscType)
	{
		case RCC_OSCILLATORTYPE_HSE:
			src_freq = g_xSysInfo.hse;
			_enable_OSC_ = BSP_Clk_HSE;
			u32PllSrc = RCC_PLLCFGR_PLLSRC_HSE;
			break;
		case RCC_OSCILLATORTYPE_HSI:
			src_freq = g_xSysInfo.hsi;
			_enable_OSC_ = BSP_Clk_HSI;
			 u32PllSrc = RCC_PLLCFGR_PLLSRC_HSI;
			break;
		default:
			break;
	}

	if (bEnable)
	{
		if (src_freq != -1)
		{
			if (_enable_OSC_)
			{
				int8_t try_again = (int8_t)(TRY_AGAIN & 0xF);
				do {
					ret = _enable_OSC_(bEnable);
					if (ret == DEV_SUCCESS)
					{
						ret = BSP_Clk_PLL(u32PllSrc, src_freq, bEnable);
						if (ret == DEV_SUCCESS)
						{
							break;
						}
					}
					try_again--;
				} while (try_again > 0);
			}
		}
	}
	else
	{
		ret = BSP_Clk_PLL(u32PllSrc, src_freq, bEnable);
		if (_enable_OSC_)
		{
			ret = _enable_OSC_(bEnable);
		}
	}
	return ret;
}

/******************************************************************************/

/**
  * @brief Set the RTC peripheral independent clock
  * @note At entry point of this function it is assumed that :
  * 1. the selected independent clock is ready
  * 2. the Backup domain control register (RCC_BDCR) write access is enable
  *
  * @param u32RTCClockSelection : The clock selection as defined by ST HAL
  *
  * @retval (see HAL_StatusTypeDef)
  */
dev_res_e BSP_Clk_SetRtcOsc(uint32_t u32RccRtcclk)
{
	/*-------------------------- RTC clock source configuration ----------------------*/
	uint32_t tmpregister;
	dev_res_e ret;

	// Check for RTC Parameters used to output RTCCLK
	assert_param( IS_RCC_RTCCLKSOURCE(u32RccRtcclk) );

    if ( (u32RccRtcclk != RCC_RTCCLKSOURCE_LSE)
    	&& (u32RccRtcclk != RCC_RTCCLKSOURCE_LSI) )
    {
    	return DEV_FAILURE;
    }

    ret = DEV_SUCCESS;
	// Reset the Backup domain only if the RTC Clock source selection is modified from default
	tmpregister = READ_BIT(RCC->BDCR, RCC_BDCR_RTCSEL);

	if ((tmpregister != RCC_RTCCLKSOURCE_NONE) && (tmpregister != u32RccRtcclk))
	{
		// Store the content of BDCR register before the reset of Backup Domain
		tmpregister = READ_BIT(RCC->BDCR, ~(RCC_BDCR_RTCSEL));
		// RTC Clock selection can be changed only if the Backup Domain is reset
		__HAL_RCC_BACKUPRESET_FORCE();
		__HAL_RCC_BACKUPRESET_RELEASE();
		// Restore the Content of BDCR register
		RCC->BDCR = tmpregister;
	}

	// Wait for LSE reactivation if LSE was enable prior to Backup Domain reset
	if (HAL_IS_BIT_SET(tmpregister, RCC_BDCR_LSEON))
	{
		ret = _wait_LSE_state_(1);
	}

	if (ret == DEV_SUCCESS)
	{
		// Apply new RTC clock source selection
		__HAL_RCC_RTC_CONFIG(u32RccRtcclk);
	}

	return ret;
}

/******************************************************************************/
/**
  * @brief Set the System Clock source
  *
  * @param u32RccSysclk The system clock selection as defined by ST HAL
  * @param FLatency     The Flash Latency as defined by ST HAL
  *
  * @retval (see HAL_StatusTypeDef)
  */
dev_res_e BSP_Clk_SetSysClk(uint32_t u32RccSysclk, uint32_t FLatency)
{
	dev_res_e ret = DEV_FAILURE;
	// Force Flash latency to max
	__HAL_FLASH_SET_LATENCY(FLASH_LATENCY_4);
	if (__HAL_FLASH_GET_LATENCY() == FLASH_LATENCY_4)
	{
		uint32_t reg;
		// Get the register
		reg = RCC->CFGR;
		// Ensure not altering the "Wake Up from stop and CSS backup clock selection" bit,
		// and clear all others (i.e.: AHB, APB1 and APB2 prescaler = divided by 1)
		reg &= RCC_CFGR_STOPWUCK_Msk;
		// Set the System clock
		reg |= (RCC_CFGR_SW_Msk & u32RccSysclk);
		RCC->CFGR = reg;

		ret = _wait_RCC_SWS_state_((uint8_t)u32RccSysclk);

		// Update SystemClcok
		SystemCoreClock = BSP_Clk_GetSysClk();

		// Set the expected Flash latency
		__HAL_FLASH_SET_LATENCY(FLatency);
		if (__HAL_FLASH_GET_LATENCY() != FLatency)
		{
			ret = DEV_FAILURE;
		}
	}
	return ret;
}

uint32_t BSP_Clk_GetSysClk(void)
{
	// FIXME : Update the SystemCoreClock global variable
	uint32_t msirange = 0U, sysclockfreq = 0U;
	uint32_t pllvco;    /* no init needed */

	uint32_t pllm, plln, pllr;
	uint32_t sysclk_source = __HAL_RCC_GET_SYSCLK_SOURCE();
	uint32_t clk_src = sysclk_source;

	if (sysclk_source == RCC_CFGR_SWS_PLL)
	{
		uint32_t pllsource = READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC);
		switch (pllsource)
		{
			case RCC_PLLSOURCE_HSI:  /* HSI used as PLL clock source */
				clk_src = RCC_CFGR_SWS_HSI;
				break;

			case RCC_PLLSOURCE_HSE:  /* HSE used as PLL clock source */
				clk_src = RCC_CFGR_SWS_HSE;
				break;

			case RCC_PLLSOURCE_MSI:  /* MSI used as PLL clock source */
				clk_src = RCC_CFGR_SWS_MSI;
				break;
			default:
				// Error !!!
				clk_src = RCC_CFGR_SWS_MSI;
				break;
		}
		pllm = (READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLM) >> RCC_PLLCFGR_PLLM_Pos) + 1U ;
		plln = (READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos);
		pllr = ((READ_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLR) >> RCC_PLLCFGR_PLLR_Pos) + 1U ) * 2U;
	}

	if ( (clk_src == RCC_CFGR_SWS_MSI) )
	{
		if (READ_BIT(RCC->CR, RCC_CR_MSIRGSEL) == 0U)
		{ /* MSISRANGE from RCC_CSR applies */
			msirange = READ_BIT(RCC->CSR, RCC_CSR_MSISRANGE) >> RCC_CSR_MSISRANGE_Pos;
		}
		else
		{ /* MSIRANGE from RCC_CR applies */
			msirange = READ_BIT(RCC->CR, RCC_CR_MSIRANGE) >> RCC_CR_MSIRANGE_Pos;
		}

		/* MSI frequency range in HZ*/
		sysclockfreq = MSIRangeTable[msirange];
	}
	else if ( (clk_src == RCC_CFGR_SWS_HSI) )
	{
		/* HSI used as system clock source */
		sysclockfreq = HSI_VALUE;
		if (g_xSysInfo.hsi != -1)
		{
			sysclockfreq = (uint32_t)g_xSysInfo.hsi;
		}
	}
	else if ( (clk_src == RCC_CFGR_SWS_HSE) )
	{
		/* HSE used as system clock source */
		sysclockfreq = HSE_VALUE;
		if (g_xSysInfo.hse != -1)
		{
			sysclockfreq = (uint32_t)g_xSysInfo.hse;
		}
	}
	else
	{
		/* unexpected case: sysclockfreq at 0 !!!*/
	}

	if (sysclk_source == RCC_CFGR_SWS_PLL)
	{
		pllvco = (sysclockfreq * plln) / pllm;
		sysclockfreq = pllvco / pllr;
	}
	return sysclockfreq;
}

/*----------------------------------------------------------------------------*/

/**
  * @brief LSE Clock Configuration
  *
  * @retval DEV_SUCCESS on success, LSE is ready and RTC clock is LSE
  * @retval DEV_FAILURE, LSE start or set RTC clock failed
  */
dev_res_e BSP_Clk_LscCfg(void)
{
	dev_res_e ret = DEV_FAILURE;
	// Enable Backup Domain access (must be set before accessing RCC_BDCR)
	ret = BSP_Clk_BDCR(1);

	if (ret == DEV_SUCCESS)
	{
		// Configure LSE Drive Capability to Hiht
		__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH);
		// Initializes LSE Oscillator
		ret = BSP_Clk_LSE(1);
		// Configure LSE Drive Capability to LOW
		__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

		if (ret == DEV_SUCCESS)
		{
			ret = BSP_Clk_SetRtcOsc(RCC_RTCCLKSOURCE_LSE);
		}
		//else  { // failed to start LSE }
	}//else  { // }
	return ret;
}


/**
  * @brief Main Clock Configuration
  *
  * @retval DEV_SUCCESS on success
  * @retval DEV_FAILURE on fail
  */
dev_res_e BSP_Clk_OscCfg(void)
{
	dev_res_e ret = DEV_SUCCESS;
	// ------------------------------------------------------------------------
	// Configure the main internal regulator output voltage
	ret = HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
	if (ret == DEV_SUCCESS)
	{
		// Force Flash latency to max
		__HAL_FLASH_SET_LATENCY(FLASH_LATENCY_4);
		// Enable MSI OSC if not already ON
		ret = BSP_Clk_MSI(1);
		if (ret == DEV_SUCCESS)
		{
			// Set frequency
			__HAL_RCC_MSI_RANGE_CONFIG(RCC_MSIRANGE_11);
			// Set MSI as default sysclk
			ret = BSP_Clk_SetSysClk(RCC_SYSCLKSOURCE_MSI, FLASH_LATENCY_2);
			// Re-Configure the source of time base considering new system clocks settings*/
			ret |= HAL_InitTick(uwTickPrio);
		}
	}
	// ------------------------------------------------------------------------
	return ret;
}

void BSP_Clk_Init(void)
{
	if (BSP_Clk_LscCfg() != DEV_SUCCESS)
	{
		// fatal
		Error_Handler();
	}
	if (BSP_Clk_OscCfg() != DEV_SUCCESS)
	{
		// fatal
		Error_Handler();
	}
	HAL_RCCEx_EnableMSIPLLMode();
}

/******************************************************************************/


/*----------------------------------------------------------------------------*/

/**
  * @static
  * @brief Set the peripheral independent clock
  *
  * @param u32Peripheral : The peripheral id as defined by ST HAL
  *
  * @param u8BaseClk     : The base clock as defined by "bsp_clk"
  *
  * @retval None
  */
static void _set_peripheral_clk_(uint32_t u32Peripheral, uint8_t u8BaseClk)
{
#define RCC_CCIPR_Msk 0x3UL
	volatile uint32_t *reg = &(RCC->CCIPR);
	// Peripheral has independent clock
	uint32_t pos;
	switch (u32Peripheral)
	{
		case RCC_PERIPHCLK_USART1:
			pos = RCC_CCIPR_USART1SEL_Pos;
			break;
		case RCC_PERIPHCLK_UART4:
			pos = RCC_CCIPR_UART4SEL_Pos;
			break;
		case RCC_PERIPHCLK_LPUART1:
			pos = RCC_CCIPR_LPUART1SEL_Pos;
			break;
#ifdef USE_I2C
		case RCC_PERIPHCLK_I2C1:
			pos = RCC_CCIPR_I2C1SEL_Pos;
			break;
#endif
#ifdef USE_LPTIMER
#if defined (LPTIM1)
		case RCC_PERIPHCLK_LPTIM1:
			pos = RCC_CCIPR_LPTIM1SEL_Pos;
			break;
#endif
#if defined (LPTIM2)
		case RCC_PERIPHCLK_LPTIM2:
			pos = RCC_CCIPR_LPTIM2SEL_Pos;
			break;
#endif
#endif
#ifdef USE_I2C
		case RCC_PERIPHCLK_I2C4:
			pos = RCC_CCIPR2_I2C4SEL_Pos;
			reg = &(RCC->CCIPR2);
			break;
#endif
		default:
			// do nothing, just return
			return;
			break;
	}
	MODIFY_REG(*reg, RCC_CCIPR_Msk << pos, (u8BaseClk << pos));
}

/******************************************************************************/

void BSP_Clk_Peripheral(uint32_t u32Peripheral, uint8_t u8BaseClk)
{
	if ( (u32Peripheral == RCC_PERIPHCLK_LPUART1)
			|| (u32Peripheral == RCC_PERIPHCLK_UART4)
			|| (u32Peripheral == RCC_PERIPHCLK_USART1)
		)
	{
		union peripheral_cfg_u cfg;
		cfg.peripheral_cfg = BSP_Clk_GetUartCfg(SERIAL_ID_COM);

		switch (u8BaseClk)
		{
			case UART_CLK_ID_LSE:
				// Force baud, if required
				if (cfg.baud > 9600)
				{
					cfg.baud = 9600;
				}
				break;
			default:
				//
				break;
		}
	}
	_set_peripheral_clk_(u32Peripheral, u8BaseClk);
}

/******************************************************************************/

#ifdef __cplusplus
}
#endif
