/**
  * @file: phy_test.c
  * @brief: This file implement some phy test usefull functions
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
  * @par 1.0.0 : 2021/04/07 [GBI]
  * Initial version
  *
  *
  */
/*!
 * @addtogroup OpenWize'Up
 * @{
 */
 
#ifdef __cplusplus
extern "C" {
#endif

#include "phy_test.h"

#include "platform.h"
#include "bsp_pwrlines.h"
#include "default_device_config.h"

extern phydev_t sPhyDev;

static void _phy_sport_cpy_cb_(void *pCBParam, void *pArg);
static void _phy_sport_cb_(void *pCBParam, void *pArg);
static void _test_set_io(uint8_t eType, uint8_t bEnable);

static void _phy_sport_cpy_cb_(void *pCBParam, void *pArg)
{
	uint8_t b_Level;
	// copy clk
	BSP_Gpio_Get((uint32_t)ADF7030_1_SPORT_CLK_GPIO_PORT, ADF7030_1_SPORT_CLK_GPIO_PIN, &b_Level);
	BSP_Gpio_Set((uint32_t)EXT_SCL_GPIO_Port, EXT_SCL_Pin, b_Level);
	// copy data
	BSP_Gpio_Get((uint32_t)ADF7030_1_SPORT_DATA_GPIO_PORT, ADF7030_1_SPORT_DATA_GPIO_PIN, &b_Level);
	BSP_Gpio_Set((uint32_t)EXT_SDA_GPIO_Port, EXT_SDA_Pin, b_Level);
}

static void _phy_sport_cb_(void *pCBParam, void *pArg)
{
#define PHY_WM2400_PREAMBLE_DATA 0x5555
#define PHY_WM2400_SYNC_WORD 0xF672

	static uint32_t sport_data;
	uint8_t b_Level;

	BSP_Gpio_Get(ADF7030_1_SPORT_DATA_GPIO_PORT, ADF7030_1_SPORT_DATA_GPIO_PIN, &b_Level);
	sport_data = (sport_data << 1) | b_Level;

	if( (uint16_t)(sport_data & 0xFFFF) == PHY_WM2400_PREAMBLE_DATA)
	{
		BSP_Gpio_Set((uint32_t)EXT_SCL_GPIO_Port, EXT_SCL_Pin, 1);
	}
	else
	{
		BSP_Gpio_Set((uint32_t)EXT_SCL_GPIO_Port, EXT_SCL_Pin, 0);
	}

	if( (uint16_t)(sport_data & 0xFFFF) ==  PHY_WM2400_SYNC_WORD)
	{
		BSP_Gpio_Set((uint32_t)EXT_SDA_GPIO_Port, EXT_SDA_Pin, 1);
	}
	else
	{
		BSP_Gpio_Set((uint32_t)EXT_SDA_GPIO_Port, EXT_SDA_Pin, 0);
	}
}

static void _test_set_io(uint8_t eType, uint8_t bEnable)
{
	if (bEnable)
	{
#ifdef I2C_HAS_POWER_LINE
			// Disable I2C power
			BSP_PwrLine_Clr(EXT_I2C_EN_MSK);
#endif
			// disable external I2C
			BSP_I2C_Enable(I2C_ID_EXT, 0);
			// Configure Host GPIO as input
			BSP_Gpio_InputEnable((uint32_t)ADF7030_1_SPORT_DATA_GPIO_PORT, ADF7030_1_SPORT_DATA_GPIO_PIN, 1);
			BSP_Gpio_InputEnable((uint32_t)ADF7030_1_SPORT_CLK_GPIO_PORT, ADF7030_1_SPORT_CLK_GPIO_PIN, 1);
			// reconfigure I2C pin as gpio output
			BSP_Gpio_OutputEnable((uint32_t)EXT_SCL_GPIO_Port, EXT_SCL_Pin, 1);
			BSP_Gpio_OutputEnable((uint32_t)EXT_SDA_GPIO_Port, EXT_SDA_Pin, 1);
#ifdef I2C_HAS_POWER_LINE
			// set external I2C power on
			BSP_PwrLine_Set(EXT_I2C_EN_MSK);
#endif
			// Setup the GPIO pin IT callback
			if ( eType )
			{
				// Setup the GPIO pin IT sensitivity
				BSP_GpioIt_ConfigLine(ADF7030_1_SPORT_CLK_GPIO_PORT, ADF7030_1_SPORT_CLK_GPIO_PIN, GPIO_IRQ_RISING_EDGE );
				// Copy PREMABLE and SYNCHRO
				BSP_GpioIt_SetCallback(ADF7030_1_SPORT_CLK_GPIO_PORT, ADF7030_1_SPORT_CLK_GPIO_PIN, _phy_sport_cb_, NULL);
			}
			else
			{
				// Setup the GPIO pin IT sensitivity
				BSP_GpioIt_ConfigLine(ADF7030_1_SPORT_CLK_GPIO_PORT, ADF7030_1_SPORT_CLK_GPIO_PIN, GPIO_IRQ_EITHER_EDGE );
				// Copy CLK and DATA
				BSP_GpioIt_SetCallback(ADF7030_1_SPORT_CLK_GPIO_PORT, ADF7030_1_SPORT_CLK_GPIO_PIN, _phy_sport_cpy_cb_, NULL);
			}
			// Enable the GPIO pin IT line
			BSP_GpioIt_SetLine(ADF7030_1_SPORT_CLK_GPIO_PORT, ADF7030_1_SPORT_CLK_GPIO_PIN, 1);
	}
	else
	{
		// Disable IT
		BSP_GpioIt_SetLine(ADF7030_1_SPORT_CLK_GPIO_PORT, ADF7030_1_SPORT_CLK_GPIO_PIN, 0);
		// Disable input
		BSP_Gpio_InputEnable((uint32_t)ADF7030_1_SPORT_DATA_GPIO_PORT, ADF7030_1_SPORT_DATA_GPIO_PIN, 0);
		BSP_Gpio_InputEnable((uint32_t)ADF7030_1_SPORT_CLK_GPIO_PORT, ADF7030_1_SPORT_CLK_GPIO_PIN, 0);
		// Disable output
		BSP_Gpio_OutputEnable((uint32_t)EXT_SCL_GPIO_Port, EXT_SCL_Pin, 0);
		BSP_Gpio_OutputEnable((uint32_t)EXT_SDA_GPIO_Port, EXT_SDA_Pin, 0);
#ifdef I2C_HAS_POWER_LINE
		// Disable I2C power
		BSP_PwrLine_Clr(EXT_I2C_EN_MSK);
#endif
		// Enable I2C peripheral
		BSP_I2C_Enable(I2C_ID_EXT, 1);
	}

}

phy_test_mode_e EX_PHY_Test(phy_test_mode_e eMode, uint8_t eType)
{
	//static test_mode_info_t eTestModeInfo = { .eTestMode = PHY_TST_MODE_NONE };
	static test_mode_info_t eTestModeInfo = {
		.eChannel = PHY_CH120,
		.eModulation = PHY_WM2400,
		.eTestMode = PHY_TST_MODE_NONE,
		.eTxMode = TMODE_TX_NONE
	};

	static test_sport_t eTestSport = {
		.eGpioData = ADF7030_1_SPORT_DATA_GPIO_PHY_PIN,
		.eGpioClk = ADF7030_1_SPORT_CLK_GPIO_PHY_PIN,
		.bGpioClk = 0,
		.bGpioData = 0
	};

	uint8_t eStatus = PHY_STATUS_OK;
	eTestModeInfo.eTxMode = eType;

	if (eMode)
	{
		eStatus = sPhyDev.pIf->pfInit(&sPhyDev);
	}
	else
	{
		eStatus = sPhyDev.pIf->pfUnInit(&sPhyDev);
	}


	if (eMode && eStatus == PHY_STATUS_OK)
	{
		// RX mode
		if (eMode < PHY_TST_MODE_TX )
		{
			// Enable IT
			_test_set_io(eType, 1);
			eTestSport.bGpioClk = 1;
			eTestSport.bGpioData = 1;
			eStatus = sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CMD_SPORT, eTestSport.testSport);
		}
		eTestModeInfo.eTestMode = eMode;
		eStatus |= sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CMD_TEST, eTestModeInfo.testMode);
	}

	if ( (!eMode) || (eStatus != PHY_STATUS_OK) )
	{
		// RX mode
		if (eTestModeInfo.eTestMode < PHY_TST_MODE_TX )
		{
			// Disable IT
			_test_set_io(eType, 0);
			eTestSport.bGpioClk = 0;
			eTestSport.bGpioData = 0;
			eStatus = sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CMD_SPORT, eTestSport.testSport);
		}
		eTestModeInfo.eTestMode = PHY_TST_MODE_NONE;
		eStatus |= sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CMD_TEST, eTestModeInfo.testMode);
	}
	return eTestModeInfo.eTestMode;
}
void EX_PHY_SetCpy(void)
{
#ifdef HAS_CPY_PIN
	BSP_GpioIt_SetGpioCpy(BSP_GpioIt_GetLineId(ADF7030_1_INT0_GPIO_PIN), IO1_GPIO_Port, IO1_Pin);
#else
#warning HAS_CPY_PIN not defined
#endif
}

#ifdef __cplusplus
}
#endif

/*! @} */
