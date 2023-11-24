/**
  * @file: at_test_cmd.c
  * @brief: This file group some AT command.
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
  * 1.0.0 : 2023/07/10[GBI]
  * Initial version
  *
  *
  */

/*!
 *  @addtogroup atci
 *  @ingroup app
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "at_test_cmd.h"

#include "atci_get_cmd.h"
#include "atci_resp.h"

#include "app_entry.h"


/******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */

uint8_t bTestMode;

/*!
 * @}
 * @endcond
 */

/******************************************************************************/

/*!
 * @brief		Execute ATFC command (set or get factory configuration)
 *
 * @details		This command may be a read or a write command:
 *
 * @li	"ATFC=<id>?" -> read configuration "id" (if parameter available)
 * @li	"ATPARAM=<id>,<value_1>,<value_2>,...,<value_n>" -> write one or more values to a configuration
 *
 * Read response format:
 * @li	"+ATPARAM:<id>,<value_1>,<value_2>,...,<value_n>"
 *
 * @parblock
 * @li	id is the configuration ID (decimal or hexadecimal 8 bits number)
 * @li	value_1, value_2... one or more values:  may be a 8, 16 or 32 bits integer (decimal or hexadecimal format may be used) or an array (hexadecimal format must be used)
 * @endparblock
 *
 * Configurations:
 * @parblock
 * <ul>
 * <li>	ADF7030 output power configuration: id, value_1, value_2, value_3
 * 		<ul>
 * 		<li> id
 * 			<ul>
 * 			<li> 0x00 : configuration for max power
 *			<li> 0x01 : configuration for 6dB under max power
 *			<li> 0x02 : configuration for 12dB under max power
 *			</ul>
 *		<li> value_1 : coarse PA settings, from 1 to 6 (8 bits integer, see ADF7030 Datasheet)
 *		<li> value_2 : fine PA settings from 0 to 255 (8 bits integer, see ADF7030 Datasheet)
 *		<li> value_3 : micro PA settings from 0 to 255 (8 bits integer, see ADF7030 Datasheet)
 * 		</ul>
 * <li>	Power amplifier (SKY66100-11) enable: id, value_1  (value_1 is 8 bits integer)
 *		<ul>
 *		<li> id : 0x10
  *		<li> value_1
 *			<ul>
 *			<li> 0 : Power amplifier is bypassed in TX
 *			<li> 1 : Power amplifier is enabled in TX
 *			</ul>
 *		</ul>
 * </li>
 * <li>	RSSI calibration (Apply Carrier at mid band frequency with -77dbm level): id
 *		<ul>
 *		<li> id : 0x20 : There is no parameter, read will return an error
 *		</ul>
 * </li>
 * <li>	Auto-Calibration: id
 * 		<ul>
 * 		<li> id : 0xFC : There is no parameter, read will return an error
 * 		</ul>
 * </ul>
 * @endparblock
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_ATFC_Cmd(atci_cmd_t *atciCmdData)
{
	atci_error_e status;
	uint8_t i;

	phy_power_entry_t sPwrEntry;

	if (atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
	{
		//get configuration ID
		Atci_Cmd_Param_Init(atciCmdData);
		status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);

		if (status != ATCI_ERR_NONE)
			return status;

		switch (*(atciCmdData->params[0].val8))
		{
			case FC_TX_PWR_0dB_ID:
			case FC_TX_PWR_m6dB_ID:
			case FC_TX_PWR_m12dB_ID:

				if (atciCmdData->cmdType == AT_CMD_READ_WITH_PARAM) //read command
				{
					Atci_Cmd_Param_Init(atciCmdData);
					atciCmdData->params[0].size = PARAM_INT8;
					Atci_Add_Cmd_Param_Resp(atciCmdData);
					atciCmdData->params[1].size = PARAM_INT8;
					Atci_Add_Cmd_Param_Resp(atciCmdData);
					atciCmdData->params[2].size = PARAM_INT8;
					Atci_Add_Cmd_Param_Resp(atciCmdData);
					atciCmdData->params[3].size = PARAM_INT8;
					Atci_Add_Cmd_Param_Resp(atciCmdData);

					sPwrEntry.eEntryId = PHY_PMAX_minus_0db + *(atciCmdData->params[0].val8);

					if (EX_PHY_GetPowerEntry(&sPwrEntry) != PHY_STATUS_OK)
						return ATCI_ERR_UNK;

					*(atciCmdData->params[1].val8) = sPwrEntry.sEntryValue.coarse;
					*(atciCmdData->params[2].val8) = sPwrEntry.sEntryValue.fine;
					*(atciCmdData->params[3].val8) = sPwrEntry.sEntryValue.micro;

					Atci_Resp(atciCmdData);
					//Atci_Resp_Data(atciCmdData->cmd_code_str[atciCmdData->cmdCode], atciCmdData);
				}
				else //write command
				{
					for (i = 0; i < FC_TX_PWR_CFG_NB_VAL; i++)
					{
						if (atciCmdData->cmdType != AT_CMD_WITH_PARAM_TO_GET)
							return ATCI_ERR_PARAM_NB;

						status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
						if (status != ATCI_ERR_NONE)
							return status;
					}

					if (atciCmdData->cmdType != AT_CMD_WITH_PARAM)
						return ATCI_ERR_PARAM_NB;

					Atci_Debug_Param_Data("Set Fact Cfg. (TX PWR)", atciCmdData);/////////

					sPwrEntry.eEntryId = PHY_PMAX_minus_0db + *(atciCmdData->params[0].val8);

					sPwrEntry.sEntryValue.coarse	= *(atciCmdData->params[1].val8);
					sPwrEntry.sEntryValue.fine		= *(atciCmdData->params[2].val8);
					sPwrEntry.sEntryValue.micro		= *(atciCmdData->params[3].val8);
					if ((sPwrEntry.sEntryValue.coarse < FC_TX_PWR_COARSE_MIN) || (sPwrEntry.sEntryValue.coarse > FC_TX_PWR_COARSE_MAX))
						return ATCI_ERR_PARAM_VAL;
					if ((sPwrEntry.sEntryValue.fine < FC_TX_PWR_FINE_MIN) || (sPwrEntry.sEntryValue.fine > FC_TX_PWR_FINE_MAX))
						return ATCI_ERR_PARAM_VAL;
					if ((sPwrEntry.sEntryValue.micro < FC_TX_PWR_MICRO_MIN) || (sPwrEntry.sEntryValue.micro > FC_TX_PWR_MICRO_MAX))
						return ATCI_ERR_PARAM_VAL;

					if (EX_PHY_SetPowerEntry(&sPwrEntry) != PHY_STATUS_OK)
						return ATCI_ERR_UNK;
				}

				return ATCI_ERR_NONE;


			case FC_PA_EN_ID:

				if (atciCmdData->cmdType == AT_CMD_READ_WITH_PARAM) //read command
				{
					Atci_Cmd_Param_Init(atciCmdData);
					atciCmdData->params[0].size = PARAM_INT8;
					Atci_Add_Cmd_Param_Resp(atciCmdData);
					atciCmdData->params[1].size = PARAM_INT8;
					Atci_Add_Cmd_Param_Resp(atciCmdData);
					*(atciCmdData->params[1].val8) = (uint8_t) EX_PHY_GetPa();
					Atci_Resp(atciCmdData);
					//Atci_Resp_Data(atciCmdData->cmd_code_str[atciCmdData->cmdCode], atciCmdData);
				}
				else if (atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)//write command
				{
					status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
					if (status != ATCI_ERR_NONE)
						return status;
					if (atciCmdData->cmdType != AT_CMD_WITH_PARAM)
						return ATCI_ERR_PARAM_NB;

					Atci_Debug_Param_Data("Set Fact Cfg. (PA EN)", atciCmdData);/////////
					EX_PHY_SetPa(*(atciCmdData->params[1].val8));
				}
				else
					return ATCI_ERR_PARAM_NB;

				return ATCI_ERR_NONE;

			case FC_RSSI_CAL_ID:

				if (atciCmdData->cmdType == AT_CMD_WITH_PARAM) //write command
				{
					Atci_Debug_Param_Data("Set Fact Cfg. (CAL RSSI)", atciCmdData);/////////
					if (EX_PHY_RssiCalibrate() != PHY_STATUS_OK)
						return ATCI_ERR_UNK;
				}
				else
					return ATCI_ERR_PARAM_NB;

				return ATCI_ERR_NONE;

			case FC_ADF7030_CAL_ID:
				if (atciCmdData->cmdType == AT_CMD_WITH_PARAM) //write command
				{
					Atci_Debug_Param_Data("Set Fact Cfg. (CAL ADF7030)", atciCmdData);/////////
					if (EX_PHY_AutoCalibrate() != PHY_STATUS_OK)
						return ATCI_ERR_UNK;
				}
				else
					return ATCI_ERR_PARAM_NB;

				return ATCI_ERR_NONE;


			default:
				return ATCI_ERR_PARAM_VAL;
		}
	}
	else
		return ATCI_ERR_PARAM_NB;
}

/******************************************************************************/

/*!
 * @brief		Execute TEST command (enable or disable a test mode)
 *
 * @details		The received AT command "ATTEST=<test_mode>" where test_mode is one of :
 * @parblock
 * @li	0x00 : disable test mode (RX or TX test)
 * @li	0x01 : enable TX test mode, transmit a carrier
 * @li	0x02 : enable TX test mode, transmit frequency deviation tone, −fDEV, in 2FSK or off in OOK
 * @li	0x03 : enable TX test mode, transmit −fDEV_MAX in 4FSK only
 * @li	0x04 : enable TX test mode, transmit +fDEV in 2FSK or on in OOK
 * @li	0x05 : enable TX test mode, transmit +fDEV_MAX in 4FSK only
 * @li	0x06 : enable TX test mode, transmit transmit preamble pattern.
 * @li	0x07 : enable TX test mode, transmit pseudorandom (PN9) sequence
 * @li	0x10 : enable RX test mode, get copy of SPORT_CLK to EXT_I2C_SCL and SPORT_DATA to EXT_I2C_SDA
 * @li	0x11 : enable RX test mode, get PREAMBLE detect on EXT_I2C_SCL and SYNCH detect on EXT_I2C_SDA
 * @endparblock
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_ATTEST_Cmd(atci_cmd_t *atciCmdData)
{
	uint8_t eRet = ATCI_ERR_NONE;
	atci_error_e status;

	if (atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
	{
		//get test mode
		Atci_Cmd_Param_Init(atciCmdData);
		status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
		if(status != ATCI_ERR_NONE)
			return status;

		if (atciCmdData->cmdType != AT_CMD_WITH_PARAM)
			return ATCI_ERR_PARAM_NB;

		test_mode_info_t eTestModeInfo = EX_PHY_TestInit();
		bTestMode = 1;

		if (*(atciCmdData->params[0].val8) == TEST_MODE_DIS) //also TMODE_TX_NONE witch correspond to the same thing (see test_modes_tx_e)
		{
			Atci_Debug_Param_Data("Set Fact Cfg. (DIS TEST MODE)", atciCmdData);/////////

			eTestModeInfo.eTestMode = PHY_TST_MODE_NONE;
			eTestModeInfo.eTxMode = 0;
			bTestMode = 0;
		}
		else if (*(atciCmdData->params[0].val8) < TMODE_TX_NB) // (see test_modes_tx_e)
		{
			Atci_Debug_Param_Data("Set Fact Cfg. (TX TEST MODE)", atciCmdData);/////////

			eTestModeInfo.eTestMode = PHY_TST_MODE_TX;
			eTestModeInfo.eTxMode = *(atciCmdData->params[0].val8);
		}
		else if (*(atciCmdData->params[0].val8) == TEST_MODE_RX_0)
		{
			Atci_Debug_Param_Data("Set Fact Cfg. (RX TEST MODE)", atciCmdData);/////////

			//eTestModeInfo.eTestMode = PHY_TST_MODE_RX;
			eTestModeInfo.eTestMode = PHY_TST_MODE_PER_RX;
			eTestModeInfo.eTxMode = 0;
		}
		else if (*(atciCmdData->params[0].val8) == TEST_MODE_RX_1)
		{
			Atci_Debug_Param_Data("Set Fact Cfg. (RX TEST MODE)", atciCmdData);/////////

			eTestModeInfo.eTestMode = PHY_TST_MODE_RX;
			eTestModeInfo.eTxMode = 1;
		}
		else
		{
			eRet = ATCI_ERR_PARAM_VAL;
		}

		if (eRet == ATCI_ERR_NONE)
		{
			if (EX_PHY_Test(eTestModeInfo) != eTestModeInfo.eTestMode)
			{
				eRet = ATCI_ERR_UNK;
			}
		}

		if (eRet != ATCI_ERR_NONE)
		{
			bTestMode = 0;
		}
	}
	else
	{
		eRet = ATCI_ERR_PARAM_NB;
	}

	return eRet;
}

/******************************************************************************/

#ifdef __cplusplus
}
#endif

/*! @} */
