/**
  * @file phy_layer_private.h
  * @brief This file defines structures, type, enum required by the phy device
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
  * @par 1.0.0 : 2020/05/15[GBI]
  * Initial version
  *
  *
  */

/*!
 * @addtogroup phy_layer
 * @ingroup device
 * @{
 *
 */
#ifndef _PHY_LAYER_PRIVATE_H_
#define _PHY_LAYER_PRIVATE_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAS_HIRES_TIME_MEAS
	//extern void BSP_HiResTmr_Capture(register uint8_t id);
	//extern uint32_t BSP_HiResTmr_Get(register uint8_t id);

	/*
	 * HIRES_TIME_MEAS
	 * Capture        Capture                Capture
	 * channel          when                  where
	 *   2     Preamble is detected    phy_layer::_frame_it
	 *   3     Transmitting is done    phy_layer::_frame_it
	 *   4     Receiving is done       phy_layer::_frame_it
	 *
	 */
#ifndef CAPTURE_ID_PREAMBLE_DETECTED
	#define CAPTURE_ID_PREAMBLE_DETECTED 2
#endif

#ifndef CAPTURE_ID_TX_COMPLETE
	#define CAPTURE_ID_TX_COMPLETE 3
#endif

#ifndef CAPTURE_ID_RX_COMPLETE
	#define CAPTURE_ID_RX_COMPLETE 4
#endif

#ifndef PHY_TMR_CAPTURE_PREAMBLE_DETECTED
	#define PHY_TMR_CAPTURE_PREAMBLE_DETECTED() BSP_HiResTmr_Capture((uint8_t)CAPTURE_ID_PREAMBLE_DETECTED)
#endif

#ifndef PHY_TMR_CAPTURE_TX_COMPLETE
	#define PHY_TMR_CAPTURE_TX_COMPLETE() BSP_HiResTmr_Capture((uint8_t)CAPTURE_ID_TX_COMPLETE)
#endif

#ifndef PHY_TMR_CAPTURE_RX_COMPLETE
	#define PHY_TMR_CAPTURE_RX_COMPLETE() BSP_HiResTmr_Capture((uint8_t)CAPTURE_ID_RX_COMPLETE)
#endif
#endif

#include "adf7030-1_phy.h"
#include "phy_itf.h"

/*!
 * @cond INTERNAL
 * @{
 */

#define RADIO_CAL_HEADER_BE 0x00002C38200003C8
#define RADIO_CAL_HEADER_SZ sizeof(RADIO_CAL_HEADER_BE)
#define RADIO_CAL_SZ sizeof(radio_cal_results_t) + RADIO_CAL_HEADER_SZ

#define VCO_CAL_HEADER_BE 0x0000283820000844
#define VCO_CAL_HEADER_SZ sizeof(VCO_CAL_HEADER_BE)
#define VCO_CAL_SZ sizeof(vco_cal_results_t) + VCO_CAL_HEADER_SZ

#define CAL_RES_SZ (36+8+32+8)

/*!
 * @}
 * @endcond
 */

/******************************************************************************/
/*!
 * @brief This define the available command to change the PHY state
 */
typedef enum {
	/*! @cond INTERNAL @{ */
	PHY_CMD_INTERNAL = PHY_CTL_CMD_LAST,
	/*! @} @endcond */
	PHY_CMD_RX       , /*!< RX command */
	PHY_CMD_TX       , /*!< TX command */
	PHY_CMD_CCA      , /*!< Noise measurement */
	// ----
	PHY_CMD_SPORT    , /*!< Set the SPORT ini/out */
	PHY_CMD_TEST     , /*!< Test mode */
	PHY_CMD_AUTO_CAL , /*!< Auto calibration */
	PHY_CMD_RSSI_CAL , /*!< RSSI calibration ( plus auto-calibration) */

	PHY_CMD_CLKOUT   , /*!< Set the CLK to output on GPIO*/
	PHY_CMD_TEMP     , /*!< Get the internal device temperature */
} phy_cmd_e;

/*!
 * @brief This define one TX power set point
 */
typedef struct {
	uint8_t coarse; /*!< TX power coarse value */
	uint8_t fine;   /*!< TX power fine value */
	uint8_t micro;  /*!< TX power micro value */
} phy_power_t;

typedef struct {
	phy_power_e eEntryId;
	phy_power_t sEntryValue;
} phy_power_entry_t;


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
int32_t Phy_GetCal(uint8_t *pBuf);
int32_t Phy_SetCal(uint8_t *pBuf);
int32_t Phy_ClrCal(void);

#ifdef PHY_USE_POWER_RAMP
	extern pa_ramp_rate_e pa_ramp_rate;
#endif

//extern int16_t rssi_offset_cal ;

//extern phy_power_t aPhyPower[PHY_NB_PWR];

extern const char * const aChanStr[PHY_NB_CH];
extern const char * const aModulationStr[PHY_NB_MOD];
extern const char * const aTestModeStr[PHY_NB_TST_MODE];
extern const char * const aTestModeTXStr[TMODE_TX_NB];
extern const char * const aPhyPwrStr[PHY_NB_PWR];
/******************************************************************************/

#ifdef __cplusplus
}
#endif
#endif /* _PHY_LAYER_PRIVATE_H_ */

/*! @} */
