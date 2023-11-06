/**
  * @file: at_test_cmd.c
  * @brief: // TODO This file ...
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
uint8_t bTestMode;

static test_mode_info_t _atci_init_test_var_(void);

/******************************************************************************/

atci_error_t Exec_ATFC_Cmd(atci_cmd_t *atciCmdData)
{
	atci_error_t status;
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

					//Atci_Resp_Data("ATFC", atciCmdData);
					Atci_Resp_Data(atci_cmd_code_str[atciCmdData->cmdCode], atciCmdData);
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
					Atci_Resp_Data(atci_cmd_code_str[atciCmdData->cmdCode], atciCmdData);
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

atci_error_t Exec_ATTEST_Cmd(atci_cmd_t *atciCmdData)
{
	uint8_t eRet = ATCI_ERR_NONE;
	atci_error_t status;

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
