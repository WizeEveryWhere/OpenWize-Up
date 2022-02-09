/**
  * @file: phy_layer_private.h
  * @brief: This file defines structures, type, enum required by the phy device
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
  * 1.0.0 : 2020/05/15[GBI]
  * Initial version
  *
  *
  */
#ifndef _PHY_LAYER_PRIVATE_H_
#define _PHY_LAYER_PRIVATE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "adf7030-1_phy.h"
#include "phy_itf.h"

#define RADIO_CAL_HEADER_BE 0x00002C38200003C8
#define RADIO_CAL_HEADER_SZ sizeof(RADIO_CAL_HEADER_BE)
#define RADIO_CAL_SZ sizeof(radio_cal_results_t) + RADIO_CAL_HEADER_SZ

#define VCO_CAL_HEADER_BE 0x0000283820000844
#define VCO_CAL_HEADER_SZ sizeof(VCO_CAL_HEADER_BE)
#define VCO_CAL_SZ sizeof(vco_cal_results_t) + VCO_CAL_HEADER_SZ

#define CAL_RES_SZ (36+8+32+8)

/******************************************************************************/
/*!
 * @brief This define the available command to change the PHY state
 */
typedef enum {
	PHY_CMD_INTERNAL = PHY_CTL_CMD_LAST,
	PHY_CMD_RX       , /*!< RX command */
	PHY_CMD_TX       , /*!< TX command */
	PHY_CMD_CCA      , /*!< Noise measurement */
	// ----
	PHY_CMD_SPORT    , /*!< Set the SPORT ini/out */
	PHY_CMD_TEST     , /*!< Test mode */
} phy_cmd_e;

/*!
 * @brief This define one TX power set point
 */
typedef struct {
	uint8_t coarse; /*!< TX power coarse value */
	uint8_t fine;   /*!< TX power fine value */
	uint8_t micro;  /*!< TX power micro value */
} phy_power_t;


/*!
 * @brief PHY device SPORT I/O selection
 */
typedef union {
	uint32_t testSport;
	struct {
		adf7030_1_gpio_pin_e eGpioData; /*!< Select the ADF7030 GPIO pin to redirect SPORT_DATA */
		adf7030_1_gpio_pin_e eGpioClk;  /*!< Select the ADF7030 GPIO pin to redirect SPORT_CLK */
		uint8_t bGpioData;              /*!< Enable/Disable the SPORT_DATA */
		uint8_t bGpioClk;               /*!< Enable/Disable the SPORT_CLK */
	};
} test_sport_t;

/*!
 * @brief PHY device test mode info structure
 */
typedef union
{
	uint32_t testMode;
	struct {
		phy_chan_e      eChannel;    /*!< Channel in test mode */
		phy_mod_e       eModulation; /*!< Modulation in test mode */
		phy_test_mode_e eTestMode;   /*!< Select the test mode */
		test_modes_tx_e eTxMode;     /*!< Select the TX test  */
	};
} test_mode_info_t;

/******************************************************************************/

int32_t Phy_adf7030_setup(
    phydev_t *pPhydev,
    adf7030_1_device_t *pCtx,
    adf7030_1_gpio_int_info_t*   const pINTDevInfo,
    adf7030_1_gpio_trig_info_t*  const pTRIGDevInfo,
    adf7030_1_gpio_reset_info_t* const pRESETDevInfo,
    adf7030_1_gpio_pin_e        eExtPaPin,
    adf7030_1_gpio_pin_e        eExtLnaPin
);

/******************************************************************************/
void Phy_OnOff(phydev_t *pPhydev, uint8_t bOn);

void Phy_SetPa(uint8_t bEnable);
int32_t Phy_GetPa(void);

int32_t Phy_SetPowerEntry(phydev_t *pPhydev, phy_power_e eEntryId, phy_power_t sPwrEntry);
int32_t Phy_GetPowerEntry(phydev_t *pPhydev, phy_power_e eEntryId, phy_power_t *sPwrEntry);

int32_t Phy_GetCal(uint8_t *pBuf);
int32_t Phy_SetCal(uint8_t *pBuf);
int32_t Phy_ClrCal(void);
int32_t Phy_AutoCalibrate(phydev_t *pPhydev);
int32_t Phy_RssiCalibrate(phydev_t *pPhydev);

#ifdef PHY_USE_POWER_RAMP
	extern pa_ramp_rate_e pa_ramp_rate;
#endif
extern int16_t rssi_offset_cal ;

extern phy_power_t aPhyPower[PHY_NB_PWR];

extern const char * const aChanStr[PHY_NB_CH];
extern const char * const aModulationStr[PHY_NB_MOD];
extern const char * const aTestModeStr[PHY_NB_TST_MODE];
extern const char * const aTestModeTXStr[TMODE_TX_NB];

/******************************************************************************/

#ifdef __cplusplus
}
#endif
#endif /* _PHY_LAYER_PRIVATE_H_ */
