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
 *  @addtogroup itf
 *  @ingroup app
 *  @{
 */
 
#ifdef __cplusplus
extern "C" {
#endif

#include "phy_test.h"

#include "platform.h"
#include "bsp_pwrlines.h"
#include "default_device_config.h"

#include "parameters_cfg.h"
#include "parameters.h"


/*! @cond INTERNAL @{ */

extern phydev_t sPhyDev;

/*! @} @endcond */

static void _phy_sport_cpy_cb_(void *pCBParam, void *pArg);
static void _phy_sport_cb_(void *pCBParam, void *pArg);
static void _test_set_io(uint8_t eType, uint8_t bEnable);

/*!
  * @static
  * @brief Copy Callback function.
  *
  * @details Called by interrupt handler to copy adf7030 gpio serial port clk and data to defined MCU gpio
  *
  * @param [in] pCBParam Unused
  * @param [in] pArg Unused
  *
  */
static void _phy_sport_cpy_cb_(void *pCBParam, void *pArg)
{
	(void)pCBParam;
	(void)pArg;
	uint8_t b_Level;
	// copy clk
	BSP_Gpio_Get((uint32_t)ADF7030_1_SPORT_CLK_GPIO_PORT, ADF7030_1_SPORT_CLK_GPIO_PIN, &b_Level);
	BSP_Gpio_Set((uint32_t)IOx0_GPIO_Port, IOx0_Pin, b_Level);
	// copy data
	BSP_Gpio_Get((uint32_t)ADF7030_1_SPORT_DATA_GPIO_PORT, ADF7030_1_SPORT_DATA_GPIO_PIN, &b_Level);
	BSP_Gpio_Set((uint32_t)IOx1_GPIO_Port, IOx1_Pin, b_Level);
}

/*!
  * @static
  * @brief Callback function.
  *
  * @details Called by interrupt handler to toggle MCU gpio on ADF7030 peramble and sync detection
  *
  * @param [in] pCBParam Unused
  * @param [in] pArg Unused
  *
  */
static void _phy_sport_cb_(void *pCBParam, void *pArg)
{
	(void)pCBParam;
	(void)pArg;

#define PHY_WM2400_PREAMBLE_DATA 0x5555
#define PHY_WM2400_SYNC_WORD 0xF672

	static uint32_t sport_data;
	uint8_t b_Level;

	BSP_Gpio_Get(ADF7030_1_SPORT_DATA_GPIO_PORT, ADF7030_1_SPORT_DATA_GPIO_PIN, &b_Level);
	sport_data = (sport_data << 1) | b_Level;

	if( (uint16_t)(sport_data & 0xFFFF) == PHY_WM2400_PREAMBLE_DATA)
	{
		BSP_Gpio_Set((uint32_t)IOx0_GPIO_Port, IOx0_Pin, 1);
	}
	else
	{
		BSP_Gpio_Set((uint32_t)IOx0_GPIO_Port, IOx0_Pin, 0);
	}

	if( (uint16_t)(sport_data & 0xFFFF) ==  PHY_WM2400_SYNC_WORD)
	{
		BSP_Gpio_Set((uint32_t)IOx1_GPIO_Port, IOx1_Pin, 1);
	}
	else
	{
		BSP_Gpio_Set((uint32_t)IOx1_GPIO_Port, IOx1_Pin, 0);
	}
}

/*!
  * @static
  * @brief Setup the io for test mode
  *
  * @param [in] eType   IO type is 0 : copy mode or 1 : preamble and sync detect
  * @param [in] bEnable Enable (1) / disable (0) the IO test mode
  *
  */
static void _test_set_io(uint8_t eType, uint8_t bEnable)
{
	if (bEnable)
	{
#ifdef USE_I2C
#ifdef I2C_HAS_POWER_LINE
			// Disable I2C power
			BSP_PwrLine_Clr(EXT_I2C_EN_MSK);
#endif
			// disable external I2C
			BSP_I2C_Enable(I2C_ID_EXT, 0);
#endif
			// Configure Host GPIO as input
			BSP_Gpio_InputEnable((uint32_t)ADF7030_1_SPORT_DATA_GPIO_PORT, ADF7030_1_SPORT_DATA_GPIO_PIN, 1);
			BSP_Gpio_InputEnable((uint32_t)ADF7030_1_SPORT_CLK_GPIO_PORT, ADF7030_1_SPORT_CLK_GPIO_PIN, 1);
			// reconfigure I2C pin as gpio output
			BSP_Gpio_OutputEnable((uint32_t)IOx0_GPIO_Port, IOx0_Pin, 1);
			BSP_Gpio_OutputEnable((uint32_t)IOx1_GPIO_Port, IOx1_Pin, 1);
#ifdef USE_I2C
#ifdef I2C_HAS_POWER_LINE
			// set external I2C power on
			BSP_PwrLine_Set(EXT_I2C_EN_MSK);
#endif
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
		BSP_Gpio_OutputEnable((uint32_t)IOx0_GPIO_Port, IOx0_Pin, 0);
		BSP_Gpio_OutputEnable((uint32_t)IOx1_GPIO_Port, IOx1_Pin, 0);
#ifdef USE_I2C
#ifdef I2C_HAS_POWER_LINE
		// Disable I2C power
		BSP_PwrLine_Clr(EXT_I2C_EN_MSK);
#endif
		// Enable I2C peripheral
		BSP_I2C_Enable(I2C_ID_EXT, 1);
#endif
	}

}

#define TEST_MODE_DEF_CH        PHY_CH120
#define TEST_MODE_DEF_MOD       PHY_WM2400

/*!
 * @brief		Get a default test mode structure
 *
 * @return initialized tes_mode_info_t structure
 */
test_mode_info_t EX_PHY_TestInit(void)
{
	test_mode_info_t eTestModeInfo;
	uint8_t t;
	// Init test variable
	eTestModeInfo.eTestMode = PHY_TST_MODE_NONE;
	eTestModeInfo.eTxMode = TMODE_TX_NONE;

	Param_Access(TEST_MODE_CHANNEL, &t, 0);
	if (!t)	{ t = 120; }
	eTestModeInfo.eChannel = (phy_chan_e)((t -100)/10);
	Param_Access(TEST_MODE_MODULATION, &t, 0);
	eTestModeInfo.eModulation = (phy_mod_e)t;
	return eTestModeInfo;
}

/*!
  * @brief Initialize the PHY test
  *
  * @param [in] eTestModeInfo PHY test mode info(see  test_mode_info_t)
  *
  * @return the current test mode
  *
  */
phy_test_mode_e EX_PHY_Test(test_mode_info_t eTestModeInfo)
{
#define EXPECTED_STATE (ADF7030_1_STATE_OPENED | ADF7030_1_STATE_INITIALIZED | ADF7030_1_STATE_CONFIGURED | ADF7030_1_STATE_READY)
	static test_sport_t eTestSport = {
		.eGpioData = ADF7030_1_SPORT_DATA_GPIO_PHY_PIN,
		.eGpioClk = ADF7030_1_SPORT_CLK_GPIO_PHY_PIN,
		.bGpioClk = 0,
		.bGpioData = 0
	};

	uint32_t state;
	uint8_t eStatus = PHY_STATUS_OK;

	sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CTL_GET_STATE, (uint32_t)&state);
	if ( (adf7030_1_state_e)( state & EXPECTED_STATE) != EXPECTED_STATE)
	{
		if (sPhyDev.pIf->pfUnInit(&sPhyDev) == PHY_STATUS_OK)
		{
			eStatus = sPhyDev.pIf->pfInit(&sPhyDev);
		}
		else
		{
			// FIXME
			return PHY_TST_MODE_NONE;
		}
	}

	if (eTestModeInfo.eTestMode && eStatus == PHY_STATUS_OK)
	{
		// CLKOUT mode
		if (eTestModeInfo.eTestMode == PHY_TST_MODE_CLKOUT )
		{
			eTestSport.bGpioClk = 1;
			eStatus = sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CTL_CMD_READY, 0);
			eStatus |= sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CMD_CLKOUT, eTestSport.testSport);
		}
		else
		{
			// RX mode
			if (eTestModeInfo.eTestMode < PHY_TST_MODE_TX )
			{
				// Enable IT
				_test_set_io(eTestModeInfo.eTxMode, 1);
				eTestSport.bGpioClk = 1;
				eTestSport.bGpioData = 1;
				eStatus = sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CTL_CMD_READY, 0);
				eStatus |= sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CMD_SPORT, eTestSport.testSport);
			}
			eStatus |= sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CMD_TEST, eTestModeInfo.testMode);
		}
	}

	if ( (!eTestModeInfo.eTestMode) || (eStatus != PHY_STATUS_OK) )
	{
		if ( eTestSport.bGpioClk || eTestSport.bGpioData)
		{
			// Disable IT
			_test_set_io(eTestModeInfo.eTxMode, 0);
			eTestSport.bGpioClk = 0;
			eTestSport.bGpioData = 0;
		}
		sPhyDev.pIf->pfUnInit(&sPhyDev);
	}
	return eTestModeInfo.eTestMode;
}

/*!
  * @brief Copy AFD7030 interrupt 0 to MCU IO1
  *
  */
inline void EX_PHY_SetCpy(void)
{
#ifdef HAS_CPY_PIN
	BSP_GpioIt_SetGpioCpy(BSP_GpioIt_GetLineId(ADF7030_1_INT0_GPIO_PIN), IOx0_GPIO_Port, IOx0_Pin);
#else
#warning HAS_CPY_PIN not defined
#endif
}

/*!
 * @brief  This function set the RF part power to ON/OFF
 *
 * @param [in]  bOn   On / Off the RF power
 *
 */
inline void EX_PHY_OnOff(uint8_t bOn)
{
	if(bOn)
	{
		sPhyDev.pIf->pfInit(&sPhyDev);
	}
	else
	{
		sPhyDev.pIf->pfUnInit(&sPhyDev);
	}
}

/*!
 * @brief  This function enable/disable the PA
 *
 * @param [in]  bEnable   Enable / Disable the PA
 *
 */
inline void EX_PHY_SetPa(uint8_t bEnable)
{
	sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CTL_SET_PA, (uint32_t)(bEnable & 0x1));
}


/*!
 * @brief  This function get the current PA state
 *
 * @return      Status
 * - 0          PA is disable
 * - 1          PA is enable
 *
 */
inline int32_t EX_PHY_GetPa(void)
{
	int32_t ret;
	sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CTL_GET_PA, (uint32_t)(&ret));
	return ret;
}

/*!
 * @brief  This function launch the RSSI offset calibration sequence.
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_BUSY (see phy_status_e::PHY_STATUS_BUSY)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 *
 */
inline int32_t EX_PHY_RssiCalibrate(void)
{
	int32_t i32Ret;
	int8_t level;
	Param_Access(TEST_MODE_RSSI_CAL_REF, (uint8_t*)(&level), 0);
	EX_PHY_OnOff(1);
	i32Ret = sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CMD_RSSI_CAL, (uint32_t)level);
	EX_PHY_OnOff(0);
	return i32Ret;
}

/*!
 * @brief  This function launch the calibration sequence
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_BUSY (see phy_status_e::PHY_STATUS_BUSY)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 *
 */
inline int32_t EX_PHY_AutoCalibrate(void)
{
	int32_t i32Ret;
	EX_PHY_OnOff(1);
	i32Ret = sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CMD_AUTO_CAL, (uint32_t)0);
	EX_PHY_OnOff(0);
	return i32Ret;
}


/*!
 * @brief  This function set/change entry in power table
 *
 * @param [in] pPhyPwrEntry Pointer on the transmission power entry
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_BUSY (see phy_status_e::PHY_STATUS_BUSY)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 */
inline int32_t EX_PHY_SetPowerEntry(phy_power_entry_t *pPhyPwrEntry)
{
	return sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CTL_SET_PWR_ENTRY, (uint32_t)(pPhyPwrEntry));
}

/*!
 * @brief  This function get entry in power table
 *
 * @param [in] pPhyPwrEntry Pointer on the transmission power entry
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_BUSY (see phy_status_e::PHY_STATUS_BUSY)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 */
inline int32_t EX_PHY_GetPowerEntry(phy_power_entry_t *pPhyPwrEntry)
{
	return sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CTL_GET_PWR_ENTRY, (uint32_t)pPhyPwrEntry);
}

inline int32_t EX_PHY_GetIhmRssi(int16_t *i16_IntPart, uint8_t *u8_DecPart)
{
	PHY_CONV_Signed11ToIhm(sPhyDev.u16_Rssi, i16_IntPart, u8_DecPart);
	return 0;
}

inline int32_t EX_PHY_GetIhmNoise(int16_t *i16_IntPart, uint8_t *u8_DecPart)
{
	PHY_CONV_Signed11ToIhm(sPhyDev.u16_Noise, i16_IntPart, u8_DecPart);
	return 0;
}

#ifdef __cplusplus
}
#endif

/*! @} */
