/**
  * @file at_extend_cmd.c
  * @brief This file group some AT command
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
  * @par 1.0.0 : 2023/05/03 [GBI]
  * Initial version
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

#include "at_extend_cmd.h"
#include "at_command.h"

#include "atci_get_cmd.h"
#include "atci_resp.h"

#include "app_entry.h"
#include <sys/time.h>

#include "itf.h"

/******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */

// FIXME :
extern int32_t Calib_WaitDone(uint32_t *pulse_cnt);

/******************************************************************************/

static void _format_stats_(uint8_t *pData, net_stats_t *pStats);

extern boot_info_t gBootInfo;

/*!
 * @}
 * @endcond
 */

/******************************************************************************/

/*!
 * @brief		Execute ATQ command (Restore registers to their factory settings)
 *
 * @details		Command format: "ATQ".
 *
 * @param[in,out]	atciCmdData  Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_ATQ_Cmd(atci_cmd_t *atciCmdData)
{
	if (atciCmdData->bLpAllowed && !(atciCmdData->bTestMode))
	{
		atciCmdData->bNeedAck = 0;
		atciCmdData->eState = ATCI_SLEEP;
		//_atci_sleep_(atciCmdData);
		Atci_Send_Sleep_Msg();

		Console_Disable();
		BSP_LowPower_Enter(LP_STOP2_MODE);
		Console_Enable();

		Atci_Send_Wakeup_Msg();
		//_atci_wakeup_(atciCmdData);
		atciCmdData->eState = ATCI_WAKEUP;
		return ATCI_ERR_NONE;
	}
	else
	{
		return ATCI_ERR_CMD_FORBIDDEN;
	}
}

/******************************************************************************/

/*!
 * @brief		Execute ATZn command (reboot the system)
 *
 * @details		Command format: "ATZn".
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_ATZn_Cmd(atci_cmd_t *atciCmdData)
{
	if(atciCmdData->cmdType != AT_CMD_WITHOUT_PARAM)
		return ATCI_ERR_PARAM_NB;

	uint8_t eRebootMode = 0;

	switch (atciCmdData->cmdCode)
	{
	/*
	 * 	ATZ or ATZ0 : (cold reboot)
	 * 	-	Restore all registers from last stored ones in NVM
	 * 	-	Clear the current clock initialize flag value
	 * 	-	Clear all internal counters.
	 * 	-	Clear network statistics.
	 */
		case CMD_ATZ:
		case CMD_ATZ0:
			eRebootMode = 1;
			break;
	/*
	 * 	ATZ1 : (warm reboot)
	 * 	-	Keep all registers from RAM
	 * 	-	Keep the current clock initialize flag value
	 * 	-	Keep all internal counters.
	 */
		case CMD_ATZ1: // (warm reboot)
		default:
			break;
	}
	Atci_AckNack(ATCI_ERR_NONE);
	if(eRebootMode)
	{
		Atci_Debug_Str("Cold Reboot");
		Storage_SetDefault(ALL_AREA_ID);
		// FIXME : put it somewhere else
		NetMgr_Open(NULL);
		NetMgr_Ioctl(NETDEV_CTL_CLR_STATS, 0);
		NetMgr_Close();
	}
	else
	{
		Atci_Debug_Str("Warm Reboot");
	}

	BSP_Boot_Reboot(eRebootMode);
	return ATCI_ERR_NONE;
}

/******************************************************************************/

/******************************************************************************/
/*
 * @brief		Execute AT&F command (Restore registers to their factory settings)
 *
 * @details		Command format: "AT&F".
 *
 * @param[in,out]	atciCmdData  Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_ATF_Cmd(atci_cmd_t *atciCmdData)
{
	if(atciCmdData->cmdType != AT_CMD_WITHOUT_PARAM)
		return ATCI_ERR_PARAM_NB;

	Atci_Debug_Str("Restore to Factory settings");
	Storage_SetDefault(ALL_AREA_ID);
	return ATCI_ERR_NONE;
}

/******************************************************************************/
/*!
 * @brief		Execute AT&W command (Store current registers values in flash)
 *
 * @details		Command format: "AT&W".
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_ATW_Cmd(atci_cmd_t *atciCmdData)
{
	if(atciCmdData->cmdType != AT_CMD_WITHOUT_PARAM)
		return ATCI_ERR_PARAM_NB;

	Atci_Debug_Str("Store current settings in NVM");
	if ( Storage_Store() == 1)
	{
		// error
		Atci_Debug_Str("NVM : Failed to store ");
		return ATCI_ERR_UNK;
	}
	return ATCI_ERR_NONE;
}


/******************************************************************************/
/*!
 * @brief		Execute AT%CCLK command (get the current clock as unix epoch)
 *
 * @details		Command format: "AT%CCLK".
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_ATCCLK_Cmd(atci_cmd_t *atciCmdData)
{
	if (
		(atciCmdData->cmdType == AT_CMD_READ_WITHOUT_PARAM) ||
		(atciCmdData->cmdType == AT_CMD_WITHOUT_PARAM)
		)
	{
		Atci_Cmd_Param_Init(atciCmdData);

		// Add param of size 4
		atciCmdData->params[0].size = 4;
		Atci_Add_Cmd_Param_Resp(atciCmdData);

		// Add param of size 2
		atciCmdData->params[1].size = 2;
		Atci_Add_Cmd_Param_Resp(atciCmdData);

		struct timeval tm;
		gettimeofday(&tm, NULL);

		// Get the EPOCH (second part)
		*(uint32_t*)(atciCmdData->params[0].data) = __htonl(tm.tv_sec);
		// Get the EPOCH (millisecond part)
		*(uint16_t*)(atciCmdData->params[1].data) = __htons(tm.tv_usec/1000);

		Atci_Resp(atciCmdData);
		//Atci_Resp_Data(atciCmdData->cmd_code_str[atciCmdData->cmdCode], atciCmdData);
		return ATCI_ERR_NONE;
	}
	else
	{
		return ATCI_ERR_PARAM_NB;
	}
}

/******************************************************************************/
/*!
 * @brief		Execute AT%UID command (get the unique identifier)
 *
 * @details		Command format: "AT%UID".
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_ATUID_Cmd(atci_cmd_t *atciCmdData)
{
	if (
		(atciCmdData->cmdType == AT_CMD_READ_WITHOUT_PARAM) ||
		(atciCmdData->cmdType == AT_CMD_WITHOUT_PARAM)
		)
	{
		Atci_Cmd_Param_Init(atciCmdData);

		// Add param of size 8
		atciCmdData->params[0].size = 8;
		Atci_Add_Cmd_Param_Resp(atciCmdData);

		// Get the UID
		uint64_t uuid = BSP_GetUid();
		((uint32_t*)(atciCmdData->params[0].data))[0] = __htonl(((uint32_t*)&uuid)[1]);
		((uint32_t*)(atciCmdData->params[0].data))[1] = __htonl(((uint32_t*)&uuid)[0]);

		Atci_Resp(atciCmdData);
		//Atci_Resp_Data(atciCmdData->cmd_code_str[atciCmdData->cmdCode], atciCmdData);
		return ATCI_ERR_NONE;
	}
	else
	{
		return ATCI_ERR_PARAM_NB;
	}
}

/******************************************************************************/
/*!
 * @brief		Execute AT%STAT command (get the wize statistics)
 *
 * @details		Command format: "AT%STAT".
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_ATSTAT_Cmd(atci_cmd_t *atciCmdData)
{
	atci_error_e status = ATCI_ERR_NONE;
	net_stats_t sStats;

	Atci_Cmd_Param_Init(atciCmdData);

	if ( atciCmdData->cmdType == AT_CMD_READ_WITHOUT_PARAM )
	{
		// Add param of size sizeof(net_stats_t)
		atciCmdData->params[0].size = sizeof(net_stats_t);
		status = Atci_Add_Cmd_Param_Resp(atciCmdData);
		if (status == ATCI_ERR_NONE)
		{
			NetMgr_Open(NULL);
			if (NetMgr_Ioctl(NETDEV_CTL_GET_STATS, (uint32_t)(&sStats)) == NET_STATUS_OK)
			{
				/*
				int16_t i16_IntPart;
				uint8_t u8_DecPart;
				EX_PHY_GetIhmRssi(&i16_IntPart, &u8_DecPart);
				EX_PHY_GetIhmNoise(&i16_IntPart, &u8_DecPart);
				*/
				_format_stats_( atciCmdData->params[0].data, &sStats);

				Atci_Resp(atciCmdData);

				// FIXME : maybe costly
				uint8_t *pBase = atciCmdData->paramsMem;
				// ---
#define RSSI_TO_FLOAT_DBM(u8Val) ((255 - u8Val) / 2 + 20) , ( (255 - u8Val) % 2)*5
				snprintf( (char*) pBase, AT_CMD_DATA_MAX_LEN,
						"RSSI dBm (Min, Avg, Max): -%d.%d, -%d.%d, -%d.%d",
						RSSI_TO_FLOAT_DBM(sStats.u8RxRssiMin),
						RSSI_TO_FLOAT_DBM(sStats.u8RxRssiAvg),
						RSSI_TO_FLOAT_DBM(sStats.u8RxRssiMax) );
				Atci_Debug_Param_Data(pBase, NULL);

				snprintf( (char*) pBase, AT_CMD_DATA_MAX_LEN,
						"Noise dBm (Min, Avg, Max): -%d.%d, -%d.%d, -%d.%d",
						RSSI_TO_FLOAT_DBM(sStats.u8TxNoiseMin),
						RSSI_TO_FLOAT_DBM(sStats.u8TxNoiseAvg),
						RSSI_TO_FLOAT_DBM(sStats.u8TxNoiseMax) );
				Atci_Debug_Param_Data(pBase, NULL);

				snprintf( (char*) pBase, AT_CMD_DATA_MAX_LEN,
						"RX (Frm, Bytes) : %ld, %ld",
						(sStats.u32RxNbFrmOK),
						(sStats.u32RxNbBytes) );
				Atci_Debug_Param_Data(pBase, NULL);

				snprintf( (char*) pBase, AT_CMD_DATA_MAX_LEN,
						"TX (Frm, Bytes) : %ld, %ld",
						(sStats.u32TxNbFrames),
						(sStats.u32TxNbBytes) );
				Atci_Debug_Param_Data(pBase, NULL);

				/*
				// ---
				atciCmdData->params[0].size = 1;
				atciCmdData->params[1].size = 1;
				atciCmdData->params[2].size = 1;
				atciCmdData->nbParams = 3;

				pBase++;
				atciCmdData->params[0].data = pBase++;
				atciCmdData->params[1].data = pBase++;
				atciCmdData->params[2].data = pBase++;
				Atci_Debug_Param_Data("RSSI (Min, Avg, Max)", atciCmdData);

				pBase++;
				atciCmdData->params[0].data = pBase++;
				atciCmdData->params[1].data = pBase++;
				atciCmdData->params[2].data = pBase++;
				Atci_Debug_Param_Data("Noise (Min, Avg, Max)", atciCmdData);

				// ---
				atciCmdData->params[0].size = PARAM_INT32;
				atciCmdData->params[1].size = PARAM_INT32;
				atciCmdData->nbParams = 2;

				pBase += sizeof(frm_err_stats_t) + sizeof(sStats.u32RxNbFrmErr);
				atciCmdData->params[0].data = pBase;
				pBase += 4;
				atciCmdData->params[1].data = pBase;
				Atci_Debug_Param_Data("RX (Frm, Bytes)", atciCmdData);

				pBase += 4;
				atciCmdData->params[0].data = pBase;
				pBase += 4;
				atciCmdData->params[1].data = pBase;
				Atci_Debug_Param_Data("TX (Frm, Bytes)", atciCmdData);
				*/
			}
			else
			{
				status = ATCI_ERR_UNK;
			}
			NetMgr_Close();
		}
	}
	else if (atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
	{
		status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
		if (status == ATCI_ERR_NONE)
		{
			if (atciCmdData->cmdType != AT_CMD_WITH_PARAM)
			{
				status = ATCI_ERR_PARAM_NB;
			}
			else
			{
				NetMgr_Open(NULL);
				//if ( *(atciCmdData->params[0].val8 == 0)
				if (NetMgr_Ioctl(NETDEV_CTL_CLR_STATS, 0))
				{
					// error
					status = ATCI_ERR_UNK;
				}
				NetMgr_Close();
			}
		}
	}
	else
	{
		status = ATCI_ERR_PARAM_NB;
	}

	return status;
}
/******************************************************************************/

/*!
 * @brief Set the code for a generic notification (from ulEvent)
 *
 * @param[in,out] atciCmdData  Pointer on "atci_cmd_t" structure
 * @param[in]	  ulEvent
 *
 * @retval atci_error_e::ATCI_ERR_NONE If success
 *         atci_error_e::ATCI_ERR_INTERNAL If code/event is unknown
 *
 */
atci_error_e Generic_Notify_SetCode(atci_cmd_t *atciCmdData, uint32_t ulEvent)
{
	uint8_t info = 0xFF;

	atciCmdData->bNeedAck = 0;

	// "NOTIFY" unsolicited command case
	if (ulEvent >= NB_AT_UNS)
	{
		ulEvent -= NB_AT_UNS;
		switch (ulEvent & 0xFF)
		{
			case TIME_NOTIFY: //
				if (ulEvent & WIZEAPP_INFO_CLOCK_MSK)
				{
					info = 0x80;
				}
				else if (ulEvent & WIZEAPP_INFO_FULL_POWER)
				{
					info = 0x81;
				}
				break;
			case SESSION_NOTIFY: //
				//ulEvent &= session_notify_mask;
				ulEvent &= SES_FLG_DWN_COMPLETE | SES_FLG_DWN_ERROR;
				if (ulEvent & SES_FLG_DWN_MSK)
				{
					info = 0x30 | ( (ulEvent >> 28) & 0x0F) ;
					// TODO The download session failed to start
					//info = 0x32;
				}
				/*
				else if (ulEvent & SES_FLG_INST_MSK)
				{
					info = 0x10 | ( (ulEvent >> 20) & 0x0F) ;
				}
				else if (ulEvent & SES_FLG_ADM_MSK)
				{
					info = 0x20 | ( (ulEvent >> 24) & 0x0F) ;
				}
				*/
				break;
			case BOOT_NOTIFY: // Cold, Warm, WDT, Update, Rollback
				info = 0x00;
				// Get the Boot reason
				// case WDT
				if (gBootInfo.reason & INSTAB_DETECT)
				{
					info = 0x03;
				}
				// TODO : case Update
				//info = 0x01;
				// TODO : case Roll-back
				//info = 0x02;
				break;
			default:
				return ATCI_ERR_INTERNAL;
				break;
		}

		Atci_Cmd_Param_Init(atciCmdData);
		// Add param of size 1
		atciCmdData->params[0].size = 1;
		Atci_Add_Cmd_Param_Resp(atciCmdData);
		// Add param value
		*(atciCmdData->params[0].data) = info;
		atciCmdData->cmdCode = UNS_NOTIFY;
	}
	// "standard" unsolicited command
	else if (ulEvent > NB_AT_CMD)
	{
		atciCmdData->cmdCode = (at_cmd_code_e)ulEvent;
	}
	// unsolicited command is not known
	else
	{
		return ATCI_ERR_INTERNAL;
	}

	return ATCI_ERR_NONE;
}

/*!
 * @brief Build and send ATCI notification
 *
 * @param[in,out]	atciCmdData  Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_ERR_NONE
 */
atci_error_e Exec_Generic_Notify(atci_cmd_t *atciCmdData)
{
	Atci_Resp(atciCmdData);
	return ATCI_ERR_NONE;
}

/******************************************************************************/

#define Calib_TMO 60000

static uint8_t _bClkOut_ = 0;
static uint32_t _u32PulseCnt_;
void Calibration_Done(uint32_t u32PulseCnt)
{
	_u32PulseCnt_ = u32PulseCnt;
}

static void __disable_clk__(void)
{
	test_mode_info_t eTestModeInfo;
	eTestModeInfo.testMode = 0;
	if (_bClkOut_)
	{
		EX_PHY_Test(eTestModeInfo);
		BSP_TmrClk_Disable();
		_bClkOut_ = 0;
	}
}

atci_error_e Exec_ATCAL_Cmd(atci_cmd_t *atciCmdData)
{
	enum
	{
		CAL_MODE_DISABLE      = 0x00,
		CAL_MODE_TEST_TIME    = 0x01,
		CAL_MODE_REF_CLK_OUT  = 0x02,
		CAL_MODE_REF_CLK_TRIM = 0x03,
		CAL_MODE_RTC_CALIB    = 0x04,
		CAL_MODE_RTC_TRIM     = 0x05,
		CAL_MODE_PHY_TEMP     = 0x06,
		CAL_MODE_MCU_TEMP     = 0x07,
	};


	test_mode_info_t eTestModeInfo;
	uint8_t eRet = ATCI_ERR_NONE;
	atci_error_e status;
	uint8_t s_mode;

	eTestModeInfo.testMode = 0;

	if (atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
	{
		// ----
		Atci_Cmd_Param_Init(atciCmdData);
		status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
		if(status != ATCI_ERR_NONE)
			return status;

		s_mode = *(atciCmdData->params[0].val8);


		if (s_mode == CAL_MODE_DISABLE)
		{
			if (atciCmdData->cmdType != AT_CMD_WITH_PARAM)
			{
				eRet = ATCI_ERR_PARAM_NB;
			}
			__disable_clk__();
		}
		else if (s_mode == CAL_MODE_TEST_TIME)
		{
			__disable_clk__();
			uint8_t i;
			struct timeval tm;
			char buff[50];

			status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
			if(status != ATCI_ERR_NONE)
			{
				return status;
			}
			if (atciCmdData->cmdType != AT_CMD_WITH_PARAM)
			{
				eRet = ATCI_ERR_PARAM_NB;
			}

			uint8_t mul = *(atciCmdData->params[1].data);
			for (i = 0; i < mul; i++)
			{
				gettimeofday(&tm, NULL);
				snprintf(buff, 49, "%lu.%06lu", (uint32_t)(tm.tv_sec), tm.tv_usec );
				Atci_Debug_Param_Data(buff, NULL);
				msleep(100);
			}
		}
		else if (s_mode == CAL_MODE_REF_CLK_OUT)
		{
			if (_bClkOut_)
			{
				__disable_clk__();
			}
			else
			{
				uint8_t clk_select = O_CLK_1Hz;
				if (atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
				{
					status = Atci_Buf_Get_Cmd_Param(atciCmdData, 1);
					if(status != ATCI_ERR_NONE)
					{
						return status;
					}
					if (atciCmdData->cmdType != AT_CMD_WITH_PARAM)
					{
						eRet = ATCI_ERR_PARAM_NB;
					}
				}

				clk_select = *(atciCmdData->params[1].data);
				if (clk_select < O_CLK_NB)
				{
					eTestModeInfo.eTestMode = PHY_TST_MODE_CLKOUT;
					BSP_TmrClk_Enable(sTimSetup[clk_select].sT15, sTimSetup[clk_select].sT1, 1);
					EX_PHY_Test(eTestModeInfo);
					_bClkOut_ = 1;
				}
				else
				{
					eRet = ATCI_ERR_PARAM_VAL;
				}
			}
		}
		else if (s_mode == CAL_MODE_REF_CLK_TRIM)
		{
			if (_bClkOut_)
			{
				status = Atci_Buf_Get_Cmd_Param(atciCmdData, 2);
				if(status != ATCI_ERR_NONE)
				{
					return status;
				}
				if (atciCmdData->cmdType != AT_CMD_WITH_PARAM_TO_GET)
				{
					eRet = ATCI_ERR_PARAM_NB;
				}

				status = Atci_Buf_Get_Cmd_Param(atciCmdData, 2);
				if(status != ATCI_ERR_NONE)
				{
					return status;
				}
				if (atciCmdData->cmdType != AT_CMD_WITH_PARAM_TO_GET)
				{
					eRet = ATCI_ERR_PARAM_NB;
				}

				status = Atci_Buf_Get_Cmd_Param(atciCmdData, 2);
				if(status != ATCI_ERR_NONE)
				{
					return status;
				}
				if (atciCmdData->cmdType != AT_CMD_WITH_PARAM_TO_GET)
				{
					eRet = ATCI_ERR_PARAM_NB;
				}

				status = Atci_Buf_Get_Cmd_Param(atciCmdData, 2);
				if(status != ATCI_ERR_NONE)
				{
					return status;
				}
				if (atciCmdData->cmdType != AT_CMD_WITH_PARAM)
				{
					eRet = ATCI_ERR_PARAM_NB;
				}

				timer_period_t sT15, sT1;
				sT15.arr = __ntohs( *(uint16_t*)atciCmdData->params[1].data);
				sT15.psc = __ntohs( *(uint16_t*)atciCmdData->params[2].data);
				sT1.arr  = __ntohs( *(uint16_t*)atciCmdData->params[3].data);
				sT1.psc  = __ntohs( *(uint16_t*)atciCmdData->params[4].data);
				if ( sT15.arr == 0)
				{
					sT15.arr = 1;
				}
				if ( sT1.arr == 0)
				{
					sT1.arr = 1;
				}
				BSP_TmrClk_Trim(sT15, sT1);
			}
			else
			{
				return ATCI_ERR_CMD_FORBIDDEN;
			}
		}
		else if (s_mode == CAL_MODE_RTC_CALIB)
		{
			__disable_clk__();
			eTestModeInfo.eTestMode = PHY_TST_MODE_CLKOUT;

			Calibrate_Run();
			EX_PHY_Test(eTestModeInfo);
			if (Calibrate_WaitDone(Calib_TMO))
			{
				eTestModeInfo.testMode = 0;
				// Timeout
				EX_PHY_Test(eTestModeInfo);
				Calibrate_Cancel();
				eRet = ATCI_ERR_UNK;
			}
		}
		else if (s_mode == CAL_MODE_RTC_TRIM)
		{
			uint32_t pulse_cnt = _u32PulseCnt_;
			if (atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
			{
				status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT32);
				if(status != ATCI_ERR_NONE)
				{
					return status;
				}
				if (atciCmdData->cmdType != AT_CMD_WITH_PARAM)
				{
					eRet = ATCI_ERR_PARAM_NB;
				}

				pulse_cnt = __ntohl( *((uint32_t*)atciCmdData->params[1].val32));
			}

			if (eRet == ATCI_ERR_NONE)
			{
				BSP_Rtc_SetCal(pulse_cnt);
			}
		}
		else if (s_mode == CAL_MODE_PHY_TEMP)
		{
			__disable_clk__();

		}
		else if (s_mode == CAL_MODE_MCU_TEMP)
		{

		}
		else
		{
			return ATCI_ERR_CMD_FORBIDDEN;
		}
	}
	else
	{
		eRet = ATCI_ERR_PARAM_NB;
	}
	return eRet;
}

/******************************************************************************/

/*!
 * @static
 * @brief		Format statistics
 *
 * @param[in]	pData  Pointer on formated output buffer
 * @param[in]	pStats Pointer on input structure holding statistics
 *
 */
static
void _format_stats_(uint8_t *pData, net_stats_t *pStats)
{
	int i;
	int sz = sizeof(net_stats_t);

	unsigned char *pIn = (unsigned char *)pStats;
	unsigned char *pOut = &(pData[sz-1]);

	for (i = 0; i < sz; i++)
	{
		*pOut = *pIn;
		pIn++;
		pOut--;
	}
}

/******************************************************************************/

#ifdef __cplusplus
}
#endif

/*! @} */
