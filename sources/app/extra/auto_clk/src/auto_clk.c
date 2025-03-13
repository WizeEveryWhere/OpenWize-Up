
/**
  * @file: auto_clk.c
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

#include "auto_clk.h"
#include "auto_cal.h"
#include <string.h>

#include "platform.h"

#include "ext_ref_clk.h"
#include "phy_test.h"

#include <stm32l4xx_hal.h>


/******************************************************************************/
__attribute__((weak))
uint32_t Setup_GetPeriphCfg(uint8_t u8DevId)
{
	union peripheral_cfg_u cfg = {
		.baud = SERIAL_BAUD,
		.swap = SERIAL_SWAP,
		.enable = 0,
	};

#ifdef HAS_IO_CFG_PARAMETER
	uint8_t u8CfgId;
	switch(u8DevId)
	{
		case DEV_ID_1:
			u8CfgId = PORT1_CFG;
			break;
		case DEV_ID_0:
		default:
			u8CfgId = PORT0_CFG;
			break;
	}

	union peripheral_cfg_u temp;
	if (Param_Access(u8CfgId, (uint8_t*)&temp, 0) == 1)
	{
		cfg.peripheral_cfg = temp.peripheral_cfg;
	}
#endif

	return cfg.peripheral_cfg;
}

__attribute__((weak))
uint32_t Setup_GetClockCfg(void)
{
	union clk_cfg_param_u clk = {
		.osc.source = OSC_SOURCE,
		.osc.trim   = OSC_TRIM,
		.osc.bypass = OSC_BYPASS,
		.osc.css    = OSC_CSS,

		.msi_trim = OSC_MSI_TRIM,

		.lsc.drive  = LSC_DRIVE,
		.lsc.toggle = LSC_TOGGLE,
	};

#ifdef HAS_IO_CFG_PARAMETER
	union clk_cfg_param temp;
	if (Param_Access(CLK_CFG, (uint8_t*)&temp, 0) == 1)
	{
		clk.clk_cfg = temp.clk_cfg;
	}
#endif

	return clk.clk_cfg;
}

/******************************************************************************/
// external
extern void Error_Handler(void);

extern struct sys_info_s g_xSysInfo;

/******************************************************************************/
// Global constant
const char * const osc_str[] =
{
	[OSC_MSI] = "MSI",
	[OSC_HSE] = "HSE",
};

const char * const bypass_str[] =
{
	[BYP_NONE] = "NONE",
	[BYP_HSE]  = "HSE",
	[BYP_LSE]  = "LSE",
	[BYP_BOTH] = "BOTH",
};

const char * const css_str[] =
{
	[CSS_NONE] = "NONE",
	[CSS_HSE]  = "HSE",
	[CSS_LSE]  = "LSE",
	[CSS_BOTH] = "BOTH",
};

// Global variables
union clk_cfg_param_u g_sClk;

/******************************************************************************/
// Static
static uint32_t _get_HSE_xtal_(uint32_t u32MeasValue);

/*----------------------------------------------------------------------------*/

#define XTAL_TOLERANCE (15) // 15/1000 (i.e. 1.5%)
static const uint16_t typ_values[3] = {16800, 19200, 25000};
static uint32_t _get_HSE_xtal_(uint32_t u32MeasValue)
{
	uint32_t found = 0;
	/*
	 * Accepted HSE xtal frequency (KHz)
	 * From 4000 to 48000 by step of 2000
	 * Plus "special" list : (tolerance env. +/-1.5%)
	 * 16800 : tol. +/- 252
	 * 19200 : tol. +/- 288
	 * 25000 : tol. +/- 375
	 *
	*/
	uint32_t m = u32MeasValue;

	for (uint8_t i = 0; i < 3; i++)
	{
		uint32_t tol = (typ_values[i] * XTAL_TOLERANCE) / 1000;
		uint8_t upper = typ_values[i] + tol + 1;
		uint8_t lower = typ_values[i] - tol - 1;

		if ( (lower < m) && (m < upper))
		{
			found = typ_values[i] * 1000;
			break;
		}
	}
	if (found == 0) // not a typical/special value
	{
		// try with multiple of 2Mhz
		found = m;
		found /= 1000; // get the KHz 9811
		if (found% 2000)
		{
			found += 2000;
		}
		found /= 2000;
		found *= 2000000;
	}
	return found;
}

/******************************************************************************/

/**
  * @static
  * @brief Main Clock Configuration
  *
  * @retval 0 on success, OSC is ready and as expected
  * @retval 1 on success, OSC is ready, but not the one expected
  * @retval -1 on failure, none of available OSC is ready
  */
int32_t AutoClk_OscCfg(void)
{
	dev_res_e ret = DEV_SUCCESS;

	uint32_t u32RccOscType;
	uint32_t u32RccSysclkSelection;
	uint32_t _current_osc_;

	// ------------------------------------------------------------------------
	// Configure the main internal regulator output voltage
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
	// ------------------------------------------------------------------------
	// Try to configure the given oscillator
	switch (g_sClk.osc.source)
	{
		case OSC_HSE:
			u32RccOscType = RCC_OSCILLATORTYPE_HSE;
			break;
		case OSC_MSI:
		default:
			u32RccOscType = RCC_OSCILLATORTYPE_MSI;
			break;
	}
	_current_osc_ = u32RccOscType;

	// ------------------------------------------------------------------------
	// Force Flash latency to max
	__HAL_FLASH_SET_LATENCY(FLASH_LATENCY_4);

	// Set MSI as default sysclk
	u32RccSysclkSelection = RCC_SYSCLKSOURCE_MSI;

	// Check if PLL is required due to HSE or HSI use
	if ( (u32RccOscType == RCC_OSCILLATORTYPE_HSE) || (u32RccOscType == RCC_OSCILLATORTYPE_HSI) )
	{
		ret = BSP_Clk_SetPllOsc(u32RccOscType, 1);
		if (ret == DEV_SUCCESS)
		{
			u32RccSysclkSelection = RCC_SYSCLKSOURCE_PLLCLK;
		}
	}

	// MSI as default OSC
	if ( (u32RccOscType == RCC_OSCILLATORTYPE_MSI) || (ret != DEV_SUCCESS))
	{
		// Enable MSI OSC if not already ON
		ret = BSP_Clk_MSI(1);
		// Set frequency
		__HAL_RCC_MSI_RANGE_CONFIG(RCC_MSIRANGE_11);
		//
		_current_osc_ = RCC_OSCILLATORTYPE_MSI;
	}

	// Setup SYSCLK
	ret = BSP_Clk_SetSysClk(u32RccSysclkSelection, FLASH_LATENCY_2);
    // Re-Configure the source of time base considering new system clocks settings*/
	ret |= HAL_InitTick(uwTickPrio);

	if (ret == DEV_SUCCESS)
	{
		// disable unused OSC
		if (u32RccSysclkSelection == RCC_SYSCLKSOURCE_PLLCLK)
		{
			// Set frequency
			__HAL_RCC_MSI_RANGE_CONFIG(RCC_MSIRANGE_11);
			// Disable MSI OSC
			BSP_Clk_MSI(0);
		}
		else
		{
			// Disable PLL and related OSC
			BSP_Clk_PLL(0, 0, 0);
			BSP_Clk_HSI(0);
			BSP_Clk_HSE(0);
		}
	}

	// ------------------------------------------------------------------------
	//BSP_Clk_OscTrim(RCC_OSCILLATORTYPE_MSI, (uint16_t)sClk.msi_trim);

	if (_current_osc_ != u32RccOscType)
	{
		g_sClk.osc.source = OSC_MSI;
		return 1;
	}

	if (ret != DEV_SUCCESS)
	{
		// error !!!
		return -1;
	}

	return 0;
}

/**
  * @brief LSE Clock Configuration
  *
  * @retval 0 on success, LSE is ready
  * @retval 1 on success, LSE fail, but LSI ready
  * @retval -1 on failure, LSE fail and LSI fail
  */
int32_t AutoClk_LscCfg(void)
{
	dev_res_e ret;

	uint32_t u32RccLseDrive;
	switch (g_sClk.lsc.drive)
	{
		case LSE_DRIVE_LOW:
			u32RccLseDrive = RCC_LSEDRIVE_LOW;
			break;
		case LSE_DRIVE_MEDIUMLOW:
			u32RccLseDrive = RCC_LSEDRIVE_MEDIUMLOW;
			break;
		case LSE_DRIVE_MEDIUMHIGH:
			u32RccLseDrive = RCC_LSEDRIVE_MEDIUMHIGH;
			break;
		case LSE_DRIVE_HIGH:
		default:
			u32RccLseDrive = RCC_LSEDRIVE_HIGH;
			break;
	}

	// Enable Backup Domain access (must be set before accessing RCC_BDCR)
	ret = BSP_Clk_BDCR(1);

	if (ret == DEV_SUCCESS)
	{
		if (g_sClk.osc.bypass & BYP_LSE)
		{
			// Check if it is already set
			if ( (RCC->BDCR & RCC_BDCR_LSEBYP) == 0)
			{
				// LSE must be disabled before
				BSP_Clk_LSE(0);
				// Set LSE bypass
				RCC->BDCR |= RCC_BDCR_LSEBYP;
			}
		}

		__HAL_RCC_LSEDRIVE_CONFIG(u32RccLseDrive);
		// Configure LSE Drive Capability
		if (g_sClk.lsc.toggle)
		{
			__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH);
		}

		// Initializes LSE Oscillator
		ret = BSP_Clk_LSE(1);

		// Configure LSE Drive Capability
		if (g_sClk.lsc.toggle)
		{
			__HAL_RCC_LSEDRIVE_CONFIG(u32RccLseDrive);
		}

		if (ret != DEV_SUCCESS) // failed to start LSE
		{
			// Disable the LSE OSC
			BSP_Clk_LSE(0);

			// Note that, from bootstrap the LSI should be ON by using the IWDG
			// Initializes LSI Oscillator
			ret = BSP_Clk_LSI(1);
			if (ret == DEV_SUCCESS)
			{
				ret = BSP_Clk_SetRtcOsc(RCC_RTCCLKSOURCE_LSI);
				// LSC is LSI, so the main clock will require a calibration.
				// RTC div_a and div_s have to be switch to LSI ones.
				return 1;
			}//else  { // }
		}
		//else  { // LSE ready }

		ret = BSP_Clk_SetRtcOsc(RCC_RTCCLKSOURCE_LSE);

		if (ret == DEV_SUCCESS) // RTC clock is set
		{
			if (g_sClk.osc.css & CSS_LSE) // LSE must be enabled and ready and RTCSEL set before
			{
				// Enable LSE CSS
				RCC->BDCR |= RCC_BDCR_LSECSSON;
				// Enable interrupt on LSE failure
				RCC->CIER |= RCC_CIER_LSECSSIE;
			}
			return 0;
		}
	}//else  { // }

	// fatal
	return -1;
}


/******************************************************************************/
/******************************************************************************/

//extern void Sys_PreInit(void);

#if 0
static void __enable_clk__(uint8_t bEnable)
{
	test_mode_info_t eTestModeInfo;

	if (bEnable)
	{
		// Output clock
		AutoCal_Tmr_Enable(1);
		ExtRefClk_Enable(REF_CLK_20000Hz);
		ExtRefClk_Trim(REF_CLK_2000Hz);
		ExtRefClk_Trim(REF_CLK_20Hz);
		ExtRefClk_Disable();
		AutoCal_Tmr_Disable(1);
	}
	else
	{
		ExtRefClk_Disable();
		AutoCal_Tmr_Disable(1);
	}
}
#endif
/**
  * @brief System Clock accurate measurement base on the ADF7030 TCXO
  *
  * @retval 0 on success,
  * @retval -1 on failure
  */
int32_t AutoClk_MeasAccurate(void)
{
//	Sys_PreInit();

	test_mode_info_t eTestModeInfo;
	eTestModeInfo.eTestMode = PHY_TST_MODE_CLKOUT;
	EX_PHY_Test(eTestModeInfo);

	//__enable_clk__(1);
	//__enable_clk__(0);

	// Measurement
	ExtRefClk_Enable(REF_CLK_20000Hz);
	AutoCal_SysMeas(20000, 10);
	ExtRefClk_Disable();

	eTestModeInfo.testMode = 0;
	EX_PHY_Test(eTestModeInfo);

	return 0;
}

/**
  * @brief LSI and HSE raw measurement
  *
  * @retval 0 on success,
  * @retval -1 on failure
  */
int32_t AutoClk_MeasRaw(void)
{
	int32_t ret = -1;
	// ----------------

	// keep LSI state
	uint32_t prev_LSI_state = RCC_LSI_ON;
	if ( (RCC->CSR & RCC_CSR_LSIRDY) == 0)
	{
		prev_LSI_state = RCC_LSI_OFF;
	}
	if (BSP_Clk_LSI(1) == DEV_SUCCESS)
	{
		if (BSP_Clk_HSI(1) == DEV_SUCCESS)
		{
			if (BSP_Clk_SetSysClk(RCC_SYSCLKSOURCE_HSI, FLASH_LATENCY_1) == DEV_SUCCESS)
			{
				// FIXME : Update TickTimer ?
				if (AutoCal_LSIMeas((uint32_t)16000000, 10) == 0)
				{
					ret = 0;
				}
				// else // error occurs
			}
			// else { // error : failure }
		}

		if ( BSP_Clk_HSE(1) == DEV_SUCCESS)
		{
			if (BSP_Clk_SetSysClk(RCC_SYSCLKSOURCE_HSE, FLASH_LATENCY_4) == DEV_SUCCESS)
			{
				// FIXME : Update TickTimer ?
				if (AutoCal_HSEMeas((uint32_t)32000, 10) == 0)
				{
					ret = 0;
				}
				// else // error occurs
			}
			// else { // error : failure }
		}
		// else { // error : failure }
		BSP_Clk_SetSysClk(RCC_SYSCLKSOURCE_MSI, FLASH_LATENCY_0);
		// FIXME : Update TickTimer ?
		BSP_Clk_HSI(0);
		BSP_Clk_HSE(0);
	}
	// else { // error : failure }

	if (prev_LSI_state == RCC_LSI_OFF)
	{
		BSP_Clk_LSI(0);
	}

	return ret;
}

/******************************************************************************/
/******************************************************************************/

void CSS_IRQHandler()
{
	// CCS failure occurs
	if (RCC->CIFR & RCC_CIFR_CSSF)
	{
		// Clear flag
		RCC->CICR = RCC_CICR_CSSC;
	}

	// LSE failure occurs
	if (RCC->CIFR & RCC_CIFR_LSECSSF)
	{
		// IRQ was enable
		if (RCC->CIER & RCC_CIER_LSECSSIE)
		{
			// Clear flag
			RCC->CICR = RCC_CICR_LSECSSC;
		}
	}
}

void NMI_Handler(void)
{
	if (g_sClk.osc.css & CSS_HSE)
	{
		CSS_IRQHandler();
	}
	else
	{
		while(1);
	}
}

/******************************************************************************/

void AutoClk_Init(void)
{
	union clk_cfg_param_u clk;
	int32_t ret;
	uint8_t u8Lsc = (uint8_t)LSC_ID_NONE;

	clk.clk_cfg = Setup_GetClockCfg();

	// Store it as global variable
	g_sClk.clk_cfg = clk.clk_cfg;

	// Init autocal
	AutoCal_Init();

	// -------------------------------------------------------------------------
	// Configure the LSC
	ret = AutoClk_LscCfg();
	switch (ret)
	{
		case 0: // Ok LSE is fine
			u8Lsc = (uint8_t)LSC_ID_LSE;
			g_xSysInfo.lsc_err = 0;
			break;
		case 1: // Only LSI is available
			u8Lsc = (uint8_t)LSC_ID_LSI;
		default:
			// fatal error
			g_xSysInfo.lsc_err = 1;
			break;
	}

	// -------------------------------------------------------------------------
	// Detect the HSE oscillator frequency
	if (g_sClk.osc.source == OSC_HSE)
	{
		if (g_sClk.osc.bypass & BYP_HSE)
		{
			if ( (RCC->CR & RCC_CR_HSEBYP) == 0)
			{
				BSP_Clk_HSE(0);
				RCC->CR |= RCC_CR_HSEBYP;
			}
		}

		AutoClk_MeasRaw();
		if (g_xSysInfo.hse != -1)
		{
			g_xSysInfo.hse = (int32_t)_get_HSE_xtal_((uint32_t)g_xSysInfo.hse);
		}
		else
		{
			// error, the main oscillator can't be HSE
		}
	}

	// -------------------------------------------------------------------------
	// Set the OSC
	ret = AutoClk_OscCfg();
	switch (ret)
	{
		case -1 :
			// fatal error
			g_xSysInfo.osc_err = 1;
			break;
		// case 0: // OSC set was as expected
		// case 1: // OSC set was not as expected
		default:
			g_xSysInfo.osc_err = 0;
			break;
	}

	// Enable / Disable MSI Auto calibration (Must be called after LSEON and LSERDY)
	if (g_sClk.osc.source == OSC_MSI)
	{
		if (g_sClk.osc.trim && (u8Lsc == (uint8_t)LSC_ID_LSE) )
		{
			HAL_RCCEx_EnableMSIPLLMode();
		}
		else
		{
			HAL_RCCEx_DisableMSIPLLMode();
		}
	}
	else if (g_sClk.osc.source == OSC_HSE)
	{
		if (g_sClk.osc.css & CSS_HSE)
		{
			RCC->CR |= RCC_CR_CSSON;
		}
	}

	//AutoClk_MeasAccurate();
}


// Status of : LSE and LSI
// Status of : MSI, HSI, HSE

// LSE :
// - ready   --> RTC is clocked by LSE
// - failure
//   LSI :
//   - ready   --> RTC is clocked by LSI; LSI calibration is required.
//   - failure --> fatal!!!

// MSI :
// - ready  --> MSI is selected as main clock;
//   LSE
//   - ready    --> enable MSI PLL
//   - failure  --> disable MSI PLL; MSI calibration is required.
// - failure --> fatal!!!

// HSE :
// - ready  --> HSE + PLL is selected as main clock;
// - failure
//   - Try to select MSI

// HSI16 :
// - ready   --> HSI16 + PLL is selected as main clock; HSI16 calibration is required.
// - failure
//   - Try to select HSE


// During runtime
// Clock interrupt enable register (RCC_CIER)
// - LSECSSIE: LSE clock security system interrupt enable
//
// - PLLRDYIE: PLL ready interrupt enable
// - HSERDYIE: HSE ready interrupt enable
// - HSIRDYIE: HSI16 ready interrupt enable
// - MSIRDYIE: MSI ready interrupt enable
// - LSERDYIE: LSE ready interrupt enable
// - LSIRDYIE: LSI ready interrupt enable
//
// Clock interrupt flag register (RCC_CIFR)
// - LSECSSF: LSE Clock security system interrupt flag (failure is detected in the LSE oscillator)
// - CSSF: Clock security system interrupt flag (failure is detected in the HSE oscillator)
//
// - PLLRDYF, HSERDYF, HSIRDYF, MSIRDYF, LSERDYF, LSIRDYF
// -
// -
//



/******************************************************************************/


#ifdef __cplusplus
}
#endif
