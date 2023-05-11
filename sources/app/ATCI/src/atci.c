/**
  * @file atci.c
  * @brief This file contains functions of the AT command interpreter for WizeUp
  * module
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
  * @par 0.0.1 : 2021/01/11 [Alciom]
  * Dev. version
  *
  *
  */

/*!
 *  @addtogroup atci
 *  @ingroup app
 *  @{
 */

/*==============================================================================
 * INCLUDES
 *============================================================================*/

#include <stdint.h>

#include "atci.h"
#include "atci_get_cmd.h"
#include "atci_resp.h"
#include "console.h"

#include "app_entry.h"
#include "platform.h"

/*==============================================================================
 * GLOBAL VARIABLES
 *============================================================================*/

/*==============================================================================
 * LOCAL FUNCTIONS PROTOTYPES
 *============================================================================*/

atci_status_t Exec_AT_Cmd(atci_cmd_t *atciCmdData);
atci_status_t Exec_ATI_Cmd(atci_cmd_t *atciCmdData);
atci_status_t Exec_ATF_Cmd(atci_cmd_t *atciCmdData);
atci_status_t Exec_ATW_Cmd(atci_cmd_t *atciCmdData);
atci_status_t Exec_ATPARAM_Cmd(atci_cmd_t *atciCmdData);
atci_status_t Exec_ATIDENT_Cmd(atci_cmd_t *atciCmdData);
atci_status_t Exec_ATSEND_Cmd(atci_cmd_t *atciCmdData);
atci_status_t Exec_ATPING_Cmd(atci_cmd_t *atciCmdData);
atci_status_t Exec_ATFC_Cmd(atci_cmd_t *atciCmdData);
atci_status_t Exec_ATTEST_Cmd(atci_cmd_t *atciCmdData);

#ifndef HAS_ATKEY_CMD
	atci_status_t Exec_ATKMAC_Cmd(atci_cmd_t *atciCmdData);
	atci_status_t Exec_ATKENC_Cmd(atci_cmd_t *atciCmdData);
#else
	#include "at_key_cmd.h"
#endif

#include "at_extend_cmd.h"

#ifdef HAS_LO_UPDATE_CMD
	#include "at_lo_update_cmd.h"
#endif

#ifdef HAS_EXTERNAL_FW_UPDATE
	#include "at_ext_update_cmd.h"
#endif

/*!
 * @cond INTERNAL
 * @{
 */
typedef atci_status_t (*pf_exec_cmd_t)(atci_cmd_t *atciCmdData);

const pf_exec_cmd_t Atci_Exec_Cmd[NB_AT_CMD] =
{
	[CMD_AT] = Exec_AT_Cmd, //nothing to do
	[CMD_ATI] = Exec_ATI_Cmd,
	[CMD_ATZ] = Exec_AT_Cmd, //something to do in states machine only
	[CMD_ATQ] = Exec_AT_Cmd, //something to do in states machine only
	[CMD_ATF] = Exec_ATF_Cmd,
	[CMD_ATW] = Exec_ATW_Cmd,
	[CMD_ATPARAM] = Exec_ATPARAM_Cmd,
	[CMD_ATIDENT] = Exec_ATIDENT_Cmd,
	[CMD_ATSEND] = Exec_ATSEND_Cmd,
	[CMD_ATPING] = Exec_ATPING_Cmd,
	[CMD_ATFC] = Exec_ATFC_Cmd,
	[CMD_ATTEST] = Exec_ATTEST_Cmd,
	[CMD_ATZC] = Exec_AT_Cmd, //something to do in states machine only
#ifndef HAS_ATKEY_CMD
	[CMD_ATKMAC] = Exec_ATKMAC_Cmd,
	[CMD_ATKENC] = Exec_ATKENC_Cmd,
#else
	[CMD_ATKEY] = Exec_ATKEY_Cmd,
#endif
#ifdef HAS_LO_UPDATE_CMD
	[CMD_ATANN] = Exec_ATANN_Cmd,
	[CMD_ATBLK] = Exec_ATBLK_Cmd,
	[CMD_ATUPD] = Exec_ATUPD_Cmd,
#ifdef HAS_LO_ATBMAP_CMD
	[CMD_ATBMAP] = Exec_ATBMAP_Cmd,
#endif
#endif
#ifdef HAS_ATSTAT_CMD
	[CMD_ATSTAT] = Exec_ATSTAT_Cmd,
#endif
#ifdef HAS_ATCCLK_CMD
	[CMD_ATCCLK] = Exec_ATCCLK_Cmd,
#endif
#ifdef HAS_EXTERNAL_FW_UPDATE
	[CMD_ATADMANN] = Exec_ATADMANN_Cmd,
#endif
#ifdef HAS_ATUID_CMD
	[CMD_ATUID] = Exec_ATUID_Cmd,
#endif
};

static test_mode_info_t _atci_init_test_var_(void);
static uint8_t _atci_init_lp_var_(void);

/*!
 * @}
 * @endcond
 */

/*==============================================================================
 * LOCAL VARIABLES
 *============================================================================*/
/*!
 * @cond INTERNAL
 * @{
 */

static uint8_t _bTestMode_;

/*!
 * @}
 * @endcond
 */

/*==============================================================================
 * FUNCTIONS
 *============================================================================*/

extern void* hLoItfTask;

static void _loitf_evt_(void *p_CbParam,  uint32_t evt)
{
	if (p_CbParam)
	{
		((atci_cmd_t*)p_CbParam)->len = BSP_Uart_GetNbReceive(UART_ID_COM);
	}
	sys_flag_set_isr(hLoItfTask, evt);
}

void _loitf_sleep_(void)
{
	//EX_PHY_OnOff(0);
	EX_PHY_SetPa(0);

	Console_Disable();

	BSP_LowPower_Enter(LP_STOP2_MODE);
	//WizeApp_Sleep();
}

void _loitf_wakeup_(uint8_t bPaState)
{
	Console_Enable();

	//EX_PHY_OnOff(1);
	EX_PHY_SetPa(bPaState);
}

/*!-----------------------------------------------------------------------------
 * @internal
 * @brief		AT command interpreter task
 *
 * @details 	Wait AT command reception, decode and execute it. Manage sleep and reset.
 *
 * @param[in]	argument Unused
 *
 * @endinternal
 *----------------------------------------------------------------------------*/
void Atci_Task(void const *argument)
{
	atci_state_t atciState = ATCI_WAKEUP;
	atci_cmd_t atciCmdData;
	atci_status_t status;
	uint8_t bPaState;
	uint8_t eLPmode;
	uint8_t eRebootMode;

	//Inits
	Console_Init(END_OF_CMD_CHAR, _loitf_evt_, &atciCmdData);

	EX_PHY_SetCpy();
	bPaState = EX_PHY_GetPa();
	eLPmode = _atci_init_lp_var_();

	eRebootMode = 0;
	//Loop
	while(1)
	{
		switch(atciState)
		{
			case ATCI_SLEEP:
				if (!_bTestMode_ && eLPmode)
				{
					Atci_Send_Sleep_Msg();
					bPaState = EX_PHY_GetPa();
					_loitf_sleep_();
				}
				atciState = ATCI_WAKEUP;
				break;
			case ATCI_WAKEUP:
				_loitf_wakeup_(bPaState);
				Atci_Send_Wakeup_Msg();
				atciState = ATCI_WAIT;
				break;
			case ATCI_WAIT:
			{
				switch(Atci_Rx_Cmd(&atciCmdData))
				{
					case ATCI_AVAIL_AT_CMD:
						atciState = ATCI_EXEC_CMD;
						break;
					case ATCI_RX_CMD_ERR:
						Atci_Resp_Ack(ATCI_RX_CMD_ERR);
						Atci_Debug_Str("Command RX error!!!");
						break;
					case ATCI_RX_CMD_TIMEOUT:
						if (!_bTestMode_ && eLPmode)
						{
							atciState = ATCI_SLEEP;
							break;
						}
					default:
						atciState = ATCI_WAIT;
						break;
				}
				break;
			}

			case ATCI_EXEC_CMD:
			{
				eLPmode = _atci_init_lp_var_();
				//decode and execute command
				status = Atci_Get_Cmd_Code(&atciCmdData);
				if(status == ATCI_OK)
				{
					if(Atci_Exec_Cmd[atciCmdData.cmdCode] == NULL)
						status = ATCI_ERR_UNKNOWN_CMD;
					else
						status = Atci_Exec_Cmd[atciCmdData.cmdCode](&atciCmdData);
				}

				//send response
				if(status == ATCI_OK)
				{
					Atci_Resp_Ack(status);
					switch(atciCmdData.cmdCode)
					{
						case CMD_ATZC:
							eRebootMode = 1; // Clear backup domain
						case CMD_ATZ:
							atciState = ATCI_RESET;
							break;
						case CMD_ATQ:
							if (!_bTestMode_ && eLPmode)
							{
								atciState = ATCI_SLEEP;
								break;
							}
						default: //other commands
							atciState = ATCI_WAIT;
							break;
					}
				}
				else
				{
					Atci_Resp_Ack(status);
					switch(status)
					{
						case ATCI_ERR_INV_NB_PARAM:
							Atci_Debug_Str("Invalid number of parameters!!!");
							break;
						case ATCI_ERR_INV_PARAM_LEN:
							Atci_Debug_Str("Invalid parameter length!!!");
							break;
						case ATCI_ERR_INV_PARAM_VAL:
							Atci_Debug_Str("Invalid parameter value!!!");
							break;
						case ATCI_ERR_UNKNOWN_CMD:
							Atci_Debug_Str("Unknown command!!!");
							break;
						case ATCI_ERR_INV_CMD_LEN:
							Atci_Debug_Str("Invalid command length!!!");
							break;
						case ATCI_ERR_FORBIDDEN_CMD:
							Atci_Debug_Str("Forbidden command in the current state!!!");
							break;
						default:
							Atci_Debug_Str("Command execution error!!!");
							break;
					}
					atciState = ATCI_WAIT;
				}
				break;
			}

			default:
			case ATCI_RESET:
				Atci_Debug_Str("Reset");
				atciState = ATCI_WAKEUP;
				BSP_Boot_Reboot(eRebootMode);
				break;
		}
	}
}

/*==============================================================================
 * LOCAL FUNCTIONS - commands executions
 *============================================================================*/

/*!-----------------------------------------------------------------------------
 * @brief		Execute AT command (nothing to do)
 *
 * @param[in,out]	atciCmdData  Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_OK if succeed
 * 	- Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 *----------------------------------------------------------------------------*/
atci_status_t Exec_AT_Cmd(atci_cmd_t *atciCmdData)
{
	if(atciCmdData->cmdType != AT_CMD_WITHOUT_PARAM)
		return ATCI_ERR_INV_NB_PARAM;

	return ATCI_OK;
}

/*!-----------------------------------------------------------------------------
 * @brief		Execute ATI command (Queries the identification of the module)
 *
 * @details		Command format: "ATI".
 *
 * 	Response format: "+ATI :"name",<manufacturer>,<model>,<hw version>,<major sw version>,<minor sw version>"
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_OK if succeed
 * 	- Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 *----------------------------------------------------------------------------*/
atci_status_t Exec_ATI_Cmd(atci_cmd_t *atciCmdData)
{
	int sz;
	int len;
	uint8_t next = 0;

	if(atciCmdData->cmdType != AT_CMD_WITHOUT_PARAM)
		return ATCI_ERR_INV_NB_PARAM;

	Atci_Cmd_Param_Init(atciCmdData);

	sz = (AT_CMD_DATA_MAX_LEN - 1);
	do
	{
		switch(next)
		{
			case 0:
				//	Name
				len = snprintf(atciCmdData->params[atciCmdData->nbParams].str, sz, "%s", sHwInfo.name);
				break;
			case 1:
				//	Manufacturer
				len = snprintf(atciCmdData->params[atciCmdData->nbParams].str, sz, "%s", sHwInfo.vendor);
				break;
			case 2:
				//	Model
				len = snprintf(atciCmdData->params[atciCmdData->nbParams].str, sz, "%s", sHwInfo.model);
				break;
			case 3:
				// Board version
				len = snprintf(atciCmdData->params[atciCmdData->nbParams].str, sz, "%02X%02X", sHwInfo.version[1], sHwInfo.version[2]);
				break;
			default:
				len = 0;
				break;
		}
		next++;
		if (len > 0)
		{
			len++;
			atciCmdData->params[atciCmdData->nbParams].size = len | PARAM_STR;
			Atci_Add_Cmd_Param_Resp(atciCmdData);
			sz -= len;
		}
	}
	while(next < 4);

	// Major FW version
	*(atciCmdData->params[atciCmdData->nbParams].val8) = sFwInfo.version[1];
	atciCmdData->params[atciCmdData->nbParams].size = PARAM_INT8;
	Atci_Add_Cmd_Param_Resp(atciCmdData);
	// Minor FW version
	*(atciCmdData->params[atciCmdData->nbParams].val8) = sFwInfo.version[2];
	atciCmdData->params[atciCmdData->nbParams].size = PARAM_INT8;
	Atci_Add_Cmd_Param_Resp(atciCmdData);

	Atci_Resp_Data("ATI", atciCmdData); //"\r\n+ATI:\"WIZEUP\",\"ALCIOM\",\"WZ1000\",\"1C\",$00,$00\r\n"

	// ---
	Atci_Cmd_Param_Init(atciCmdData);
	len = snprintf(atciCmdData->params[atciCmdData->nbParams].str, (AT_CMD_DATA_MAX_LEN-1), "%s", sFwInfo.build_date);
	if (len > 0)
	{
		atciCmdData->params[atciCmdData->nbParams].size = (len + 1) | PARAM_STR;
		Atci_Add_Cmd_Param_Resp(atciCmdData);
		Atci_Debug_Param_Data("Compilation date", atciCmdData);
	}
	return ATCI_OK;
}

/*!-----------------------------------------------------------------------------
 * @brief		Execute AT&F command (Restore registers to their factory settings)
 *
 * @details		Command format: "AT&F".
 *
 * @param[in,out]	atciCmdData  Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_OK if succeed
 * 	- Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 *----------------------------------------------------------------------------*/
atci_status_t Exec_ATF_Cmd(atci_cmd_t *atciCmdData)
{
	if(atciCmdData->cmdType != AT_CMD_WITHOUT_PARAM)
		return ATCI_ERR_INV_NB_PARAM;

	Atci_Debug_Str("Restore to Factory settings");
	Storage_SetDefault();
	return ATCI_OK;
}

/*!-----------------------------------------------------------------------------
 * @brief		Execute AT&W command (Store current registers values in flash)
 *
 * @details		Command format: "AT&W".
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_OK if succeed
 * 	- Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 *----------------------------------------------------------------------------*/
atci_status_t Exec_ATW_Cmd(atci_cmd_t *atciCmdData)
{
	if(atciCmdData->cmdType != AT_CMD_WITHOUT_PARAM)
		return ATCI_ERR_INV_NB_PARAM;

	Atci_Debug_Str("Store current registers values in non volatile memory");
	if ( Storage_Store() == 1)
	{
		// error
		Atci_Debug_Str("Flash : Failed to store ");
		return ATCI_ERR;
	}
	return ATCI_OK;
}

/*!-----------------------------------------------------------------------------
 * @brief		Execute ATPARAM command (Modify/read the value of a Wize LAN parameter)
 *
 * @details		This command may be a read or a write command:
 *
 * @parblock
 * @li "ATPARAM?" : read all registers
 * @li "ATPARAM=<address>?" : read register of address "address"
 * @li "ATPARAM=<address>,<value>" : write "value" to register of address "address" (1 byte)
 * @endparblock
 *
 * Read response format:
 * @parblock
 * "+ATPARAM:<address>,<value>"
 * (this response is send for each register in read all register mode)
 * @li address : is the register address (decimal or hexadecimal 8 bits number)
 * @li value : may be a 8, 16 or 32 bits integer (decimal or hexadecimal format may be used) or an array (hexadecimal format must be used) -> refer to register list in specifications
 * @endparblock
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 * 					- params[0] is used for the register address witch is always a 8 bits integer
 * 					- params[1] is used for the register value witch can be a 8, 16 or 32 bits integer or a bytes array
 *
 * @return
 * - ATCI_OK if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 *----------------------------------------------------------------------------*/
atci_status_t Exec_ATPARAM_Cmd(atci_cmd_t *atciCmdData)
{
	atci_status_t status;
	param_access_e regAccess;
	uint8_t regId;

	if(atciCmdData->cmdType == AT_CMD_READ_WITHOUT_PARAM) //read all registers command
	{
		//read all registers:
		Atci_Cmd_Param_Init(atciCmdData);
		atciCmdData->params[0].size = PARAM_INT8;
		Atci_Add_Cmd_Param_Resp(atciCmdData);
		atciCmdData->params[1].size = PARAM_VARIABLE_LEN;
		Atci_Add_Cmd_Param_Resp(atciCmdData);
		for(regId = 0; regId < PARAM_ACCESS_CFG_SZ; regId++)
		{
			if(Param_GetLocAccess(regId) & RO)
			{
				*(atciCmdData->params[0].val8) = regId;

				atciCmdData->params[1].size = (uint16_t) Param_GetSize(regId);
				if(atciCmdData->params[1].size == 0)
					return ATCI_ERR;
				else if(atciCmdData->params[1].size == 1)
					atciCmdData->params[1].size = PARAM_INT8;
				else if(atciCmdData->params[1].size == 2)
					atciCmdData->params[1].size = PARAM_INT16;
				else if(atciCmdData->params[1].size == 4)
					atciCmdData->params[1].size = PARAM_INT32;
				else if(atciCmdData->params[1].size > (AT_CMD_DATA_MAX_LEN-atciCmdData->paramsMemIdx))
					return ATCI_ERR;

				Param_Access(regId, atciCmdData->params[1].data, 0);

				Atci_Resp_Data("ATPARAM", atciCmdData);
			}
		}
		return ATCI_OK;
	}
	else if(atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET) //read/write one register command
	{
		//get register address
		Atci_Cmd_Param_Init(atciCmdData);
		status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
		if(status != ATCI_OK)
			return status;

		//check param address:
		regAccess = Param_GetLocAccess(*(atciCmdData->params[0].val8));
		if(regAccess == NA)
			return ATCI_ERR_INV_PARAM_VAL;

		//get param size:
		atciCmdData->params[1].size = (uint16_t) Param_GetSize(*(atciCmdData->params[0].val8));
		if(atciCmdData->params[1].size == 0)
			return ATCI_ERR;
		else if(atciCmdData->params[1].size == 1)
			atciCmdData->params[1].size = PARAM_INT8;
		else if(atciCmdData->params[1].size == 2)
			atciCmdData->params[1].size = PARAM_INT16;
		else if(atciCmdData->params[1].size == 4)
			atciCmdData->params[1].size = PARAM_INT32;
		else if(atciCmdData->params[1].size > (AT_CMD_DATA_MAX_LEN-atciCmdData->paramsMemIdx))
			return ATCI_ERR;

		if(atciCmdData->cmdType == AT_CMD_READ_WITH_PARAM) //read one register command
		{
			//check if param can be read:
			if((regAccess & RO) == 0)
				return ATCI_ERR_INV_PARAM_VAL;

			//register 2nd param in params list
			Atci_Add_Cmd_Param_Resp(atciCmdData);

			//get parameter value
			if(!Param_Access(*(atciCmdData->params[0].val8), atciCmdData->params[1].data, 0))
				return ATCI_ERR;

			//display parameter
			Atci_Resp_Data("ATPARAM", atciCmdData);

			return ATCI_OK;
		}
		else if(atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET) //write one register command
		{
			//check if param can be written:
			if((regAccess & WO) == 0)
				return ATCI_ERR_INV_PARAM_VAL;

			//get register value:
			status = Atci_Buf_Get_Cmd_Param(atciCmdData, atciCmdData->params[1].size);
			if(status != ATCI_OK)
				return status;
			else if(atciCmdData->cmdType != AT_CMD_WITH_PARAM)
				return ATCI_ERR_INV_NB_PARAM;

			//check param value:
			if(!Param_CheckConformity(*(atciCmdData->params[0].val8), atciCmdData->params[1].data))
				return ATCI_ERR_INV_PARAM_VAL;

			//write param value:
			if(!Param_Access(*(atciCmdData->params[0].val8), atciCmdData->params[1].data, 1))
				return ATCI_ERR;

			Atci_Debug_Param_Data("Write register", atciCmdData);/////////

			return ATCI_OK;
		}
		else
			return ATCI_ERR_INV_NB_PARAM;
	}
	else
		return ATCI_ERR_INV_NB_PARAM;

}
#ifndef HAS_ATKEY_CMD
/*!-----------------------------------------------------------------------------
 * @brief		Execute ATKMAC command (Modify the value of the Kmac key)
 *
 * @details		ATKMAC command is a write only command:
 *
 * "ATKMAC=<key>" key is a 16 or 32 bytes key (32 bytes KMAC but only the 16 1st bytes uses) and must be written in hexadecimal format (with "$" char)
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 * 					- params[0] is used for the key (16 bytes in command but extended to 32 bytes by adding zeros
 * 						because a 32 bytes key is needed but only 16 bytes are used)
 *
 * @return
 * - ATCI_OK if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 *----------------------------------------------------------------------------*/
atci_status_t Exec_ATKMAC_Cmd(atci_cmd_t *atciCmdData)
{
	atci_status_t status;
	uint8_t i;

	if(atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
	{
		Atci_Cmd_Param_Init(atciCmdData);
		status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_VARIABLE_LEN);
		if(status != ATCI_OK)
			return status;

		if(atciCmdData->cmdType == AT_CMD_WITH_PARAM)
		{
			if(atciCmdData->params[0].size == ATKMAC_KEY_LEN) //if key given is on 16 bytes
			{
				//add zeros to received key because KMAC is on 32 bytes but only the 16 first bytes are used
				if(Atci_Update_Cmd_Param_len(atciCmdData, KEY_SIZE) != ATCI_OK)
					return ATCI_ERR;
				for(i=ATKMAC_KEY_LEN; i<KEY_SIZE; i++)
					atciCmdData->params[0].data[i] = 0;
			}
			else if(atciCmdData->params[0].size != KEY_SIZE) //bad key length
				return ATCI_ERR_INV_PARAM_LEN;
			//else -> if key given is on 32 bytes

			//write KMAC:
			if(Crypto_WriteKey(atciCmdData->params[0].data, KEY_MAC_ID) != CRYPTO_OK)
				return ATCI_ERR;

			Atci_Update_Cmd_Param_len(atciCmdData, ATKMAC_KEY_LEN);////////////
			Atci_Debug_Param_Data("Write KMAC", atciCmdData);////////////

			return ATCI_OK;
		}
		else
			return ATCI_ERR_INV_NB_PARAM;
	}
	else
		return ATCI_ERR_INV_NB_PARAM;
}

/*!-----------------------------------------------------------------------------
 * @brief		Execute ATKENC command (Modify the value of the Kenc key)
 *
 * @details		ATKENC command is a write only command:
 *
 * "ATKENC=<id>,<key>"
 * @parblock
 * @li id is the key number (decimal or hexadecimal 1 byte number)
 * @li key is a 16 or 32 bytes key (32 bytes KENC but only the 16 1st bytes uses) and must be written in hexadecimal format (with "$" char)
 * @endparblock
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_OK if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 *----------------------------------------------------------------------------*/
atci_status_t Exec_ATKENC_Cmd(atci_cmd_t *atciCmdData)
{
	atci_status_t status;
	uint8_t i;

	if(atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
	{
		//get key number
		Atci_Cmd_Param_Init(atciCmdData);
		status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
		if(status != ATCI_OK)
			return status;

		//check KENC number
		// FIXME : KCHG
		//if((*(atciCmdData->params[0].val8) < KEY_ENC_MIN) || (*(atciCmdData->params[0].val8) > KEY_ENC_MAX))
		if((*(atciCmdData->params[0].val8) < KEY_ENC_MIN) || (*(atciCmdData->params[0].val8) > KEY_MAX_NB))
			return ATCI_ERR_INV_PARAM_VAL;

		if(atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
		{
			//get key value
			status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_VARIABLE_LEN);
			if(status != ATCI_OK)
				return status;

			if(atciCmdData->cmdType == AT_CMD_WITH_PARAM)
			{
				if(atciCmdData->params[1].size == ATKENC_KEY_LEN) //if key given is on 16 bytes
				{
					//add zeros to received key because KENC is on 32 bytes but only the 16 first bytes are used
					if(Atci_Update_Cmd_Param_len(atciCmdData, KEY_SIZE) != ATCI_OK)
						return ATCI_OK;
					for(i=ATKENC_KEY_LEN; i<KEY_SIZE; i++)
						atciCmdData->params[1].data[i] = 0;
				}
				else if(atciCmdData->params[1].size != KEY_SIZE) //bad key length
					return ATCI_ERR_INV_PARAM_LEN;
				//else -> if key given is on 32 bytes

				//write KENC:
				if(Crypto_WriteKey(atciCmdData->params[1].data, *(atciCmdData->params[0].val8)) != CRYPTO_OK)
					return ATCI_ERR;

				Atci_Update_Cmd_Param_len(atciCmdData, ATKENC_KEY_LEN);////////////
				Atci_Debug_Param_Data("Write KENC", atciCmdData);////////////

				return ATCI_OK;
			}
			else
				return ATCI_ERR_INV_NB_PARAM;
		}
		else
			return ATCI_ERR_INV_NB_PARAM;
	}
	else
		return ATCI_ERR_INV_NB_PARAM;
}
#endif

/*!-----------------------------------------------------------------------------
 * @brief		Execute ATIDENT command (Modify/read the value of M-field and A-field)
 *
 * @details		This command may be a read or a write command:
 *
 * @parblock
 * @li "ATIDENT=<M-field>,<A-field>" : write M-field and A-field
 * @li "ATIDENT?" : read M-field and A-field
 * 		- <M-field> is 2 bytes array (must be in hex format)
 * 		- <A-field> is 6 bytes array (must be in hex format)
 *
 * Response to a read command:
 * @parblock
 * @li "+ATIDENT:<M-field>,<A-field>"
 * @endparblock
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_OK if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 *----------------------------------------------------------------------------*/
atci_status_t Exec_ATIDENT_Cmd(atci_cmd_t *atciCmdData)
{
	atci_status_t status;

	if(atciCmdData->cmdType == AT_CMD_READ_WITHOUT_PARAM) //read command
	{
		// Init
		Atci_Cmd_Param_Init(atciCmdData);
		// Add param of size 2
		atciCmdData->params[0].size = 2;
		Atci_Add_Cmd_Param_Resp(atciCmdData);
		// Add param of size 6
		atciCmdData->params[1].size = 6;
		Atci_Add_Cmd_Param_Resp(atciCmdData);

		// Read M-field and A-field
		WizeApi_GetDeviceId( (device_id_t *)(atciCmdData->params[0].data));

		Atci_Resp_Data("ATIDENT", atciCmdData);
		return ATCI_OK;
	}
	else if(atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET) //write command
	{
		//get M-field
		Atci_Cmd_Param_Init(atciCmdData);
		status = Atci_Buf_Get_Cmd_Param(atciCmdData, ATIDENT_MFIELD_LEN);

		if(status != ATCI_OK)
			return status;

		if(atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
		{
			//get A-field
			status = Atci_Buf_Get_Cmd_Param(atciCmdData, ATIDENT_AFIELD_LEN);

			if(status != ATCI_OK)
				return status;

			if(atciCmdData->cmdType == AT_CMD_WITH_PARAM)
			{
				//write M-field & A-field:
				if ( WizeApi_SetDeviceId( (device_id_t *)(atciCmdData->params[0].data) ) != WIZE_API_SUCCESS)
				{
					Atci_Debug_Param_Data("Write IDENT", atciCmdData);/////////
					return ATCI_ERR;
				}
				Atci_Debug_Param_Data("Write IDENT", atciCmdData);/////////

				return ATCI_OK;
			}
			else
				return ATCI_ERR_INV_NB_PARAM;
		}
		else
			return ATCI_ERR_INV_NB_PARAM;
	}
	else
		return ATCI_ERR_INV_NB_PARAM;
}

/*!-----------------------------------------------------------------------------
 * @brief		Execute ATSEND command (Send a Wize message)
 *
 * @details		Command format: "ATSEND=<l6app>,<l7msg>"
 * @parblock
 * @li l6app is the layer 6 application code used to give for witch application are the L7 data (decimal or hexadecimal 8bits integer)
 * @li l7msg is the layer 7 message to send (array in hexadecimal format, maximum length is 102 bytes for PRES-EXCHANGE L6 frames)
 * @endparblock
 *
 * Maybe 2 types of responses:
 * @parblock
 * @li an APP-ADMIN write command was received and processed by the on-board Wize stack message: "+ATADMWRITE :<paramid>,<paramvalue>,<rssi>"
 * @li a response was received in response of the Wize message, which can’t be managed by the on-board Wize stack (other application layer than APP-ADMIN) message: "+ATRCV:<L6App>,<l7resp>,<rssi>"
 * @endparblock
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_OK if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 *----------------------------------------------------------------------------*/
atci_status_t Exec_ATSEND_Cmd(atci_cmd_t *atciCmdData)
{
	atci_status_t status;
	uint8_t i;

	if (atciCmdData->cmdType != AT_CMD_WITH_PARAM_TO_GET)
	{
		return ATCI_ERR_INV_NB_PARAM;
	}

	Atci_Cmd_Param_Init(atciCmdData);

	// -------------------------------------------------------------------------
	// get L6-app field
	status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
	if (status != ATCI_OK) { return status; }
	if (atciCmdData->cmdType != AT_CMD_WITH_PARAM_TO_GET)
	{
		return ATCI_ERR_INV_NB_PARAM;
	}

	// if is currently in test mode
	if(_bTestMode_)
	{
		return ATCI_ERR_FORBIDDEN_CMD;
	}

	// -------------------------------------------------------------------------
	//get L7 message
	status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_VARIABLE_LEN);
	if (status != ATCI_OK) { return status; }
	if (atciCmdData->cmdType != AT_CMD_WITH_PARAM)
	{
		return ATCI_ERR_INV_NB_PARAM;
	}
	Param_Access(L7TRANSMIT_LENGTH_MAX, &i, 0);
	if (atciCmdData->params[1].size > i)
	{
		return ATCI_ERR_INV_NB_PARAM;
	}

	// -------------------------------------------------------------------------
	Atci_Debug_Param_Data("Send Frame.", atciCmdData);/////////

	int32_t ret;
	//send frame and...
	if( WIZE_API_SUCCESS != WizeApp_Send(atciCmdData->paramsMem, atciCmdData->params[1].size+1) )
	{
		// Failure
		return ATCI_ERR;
	}
	// ...wait for response:
	ret = WizeApp_WaitSesComplete(SES_ADM);
	if ( ret < 0 )
	{
		return ATCI_ERR;
	}
	// if session is complete without error and CMD is WRITE_PARAM
	if ( ret > 0 )
	{
		// APP-ADMIN write command reception
		// msg format: <cmd ID 1 (1 byte)><cmd val 1 (s1 bytes)>...<cmd ID n (1 byte)><cmd val n (sn bytes)>
		atciCmdData->nbParams = 3;
		atciCmdData->params[0].size = PARAM_INT8;
		atciCmdData->params[2].size = PARAM_INT8;
		atciCmdData->params[2].data = &(atciCmdData->paramsMem[AT_CMD_DATA_MAX_LEN-1]);

		uint8_t size = WizeApp_GetAdmCmd(
				//write RX message in params memory
				atciCmdData->paramsMem,
				//write RSSI in ???
				atciCmdData->params[2].val8
				);
		if ( ret == 1)
		{
			if (size > 1)
			{
				i = 0;
				while (i < size)
				{
					//get param ID (1st byte of received message)
					atciCmdData->params[0].data = &(atciCmdData->paramsMem[i++]);
					//get param Value (next bytes of received message)
					atciCmdData->params[1].size = (uint16_t) Param_GetSize(*(atciCmdData->params[0].val8));
					atciCmdData->params[1].data = &(atciCmdData->paramsMem[i]);
					i += atciCmdData->params[1].size;

					// send received APP-ADMIN WRITE command
					Atci_Resp_Data("ATADMWRITE", atciCmdData);
				}
			}
		}
		else // if ( ret == 2)
		{
			//send received APP-ADMIN ANN_DOWNLOAD command
			Atci_Resp_Data("ATADMANN", atciCmdData);
		}
#if 0
		// FIXME : The following is not Wize 1.0 compliant
		//Response of the Wize message reception
		Atci_Cmd_Param_Init(atciCmdData);
		rxMsg.pData = atciCmdData->paramsMem; //write directly RX message in params memory
		sta = WizeApi_GetAdmRsp(&rxMsg);
		if (sta == WIZE_API_SUCCESS)
		{
			//TODO: Rx msg format to be verified
			// msg format: <L6 App code (1 byte)>...<cL7 data (n bytes)>
			//get L6 app code (1st byte of received message)
			atciCmdData->params[0].size = PARAM_INT8;
			Atci_Add_Cmd_Param_Resp(atciCmdData);
			//get L7 response message (next bytes of received message)
			atciCmdData->params[1].size = rxMsg.u8Size - 1;
			Atci_Add_Cmd_Param_Resp(atciCmdData);
			//get RSSI
			atciCmdData->params[2].size = PARAM_INT8;
			*(atciCmdData->params[2].val8) = rxMsg.u8Rssi;
			Atci_Add_Cmd_Param_Resp(atciCmdData);

			//send received APP-ADMIN command
			Atci_Resp_Data("ATRCV", atciCmdData);
		}
#warning "ATRCV is not available"
#warning "ATRCV is not is not Wize rev. 1.2 compliant"
#endif
	}
	return ATCI_OK;
}

/*!-----------------------------------------------------------------------------
 * @brief		Execute ATPING command (Send an INSTPING request)
 *
 * @details 	Command format: "ATPING"
 * 				No specific response (read dedicated registers with ATPARAM command)
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_OK if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 *----------------------------------------------------------------------------*/
atci_status_t Exec_ATPING_Cmd(atci_cmd_t *atciCmdData)
{
	uint8_t nbPong, i;

	if(atciCmdData->cmdType != AT_CMD_WITHOUT_PARAM)
		return ATCI_ERR_INV_NB_PARAM;

	// if is currently in test mode
	if(_bTestMode_)
	{
		return ATCI_ERR_FORBIDDEN_CMD;
	}

	Atci_Debug_Str("Send PING");/////////

	// clear PING_NBFOUND
	nbPong = 0;
	Param_Access(PING_NBFOUND, &nbPong, 1);

	int32_t ret;
	// start install session and...
	if( WIZE_API_SUCCESS != WizeApp_Install() )
	{
		// Failure
		return ATCI_ERR;
	}
	// ...wait for complete
	ret = WizeApp_WaitSesComplete(SES_INST);
	if ( ret < 0 )
	{
		return ATCI_ERR;
	}

	Param_Access(PING_NBFOUND, &nbPong, 0);
	if(nbPong > 8)
		nbPong = 8;

	Atci_Cmd_Param_Init(atciCmdData);
	atciCmdData->params[0].size = PARAM_INT8;
	*(atciCmdData->params[0].val8) = nbPong;
	Atci_Add_Cmd_Param_Resp(atciCmdData);
	Atci_Debug_Param_Data("Nb Pong", atciCmdData);

	Atci_Cmd_Param_Init(atciCmdData);
	atciCmdData->params[0].size = PARAM_INT8;
	Atci_Add_Cmd_Param_Resp(atciCmdData);
	atciCmdData->params[1].size = 9;
	Atci_Add_Cmd_Param_Resp(atciCmdData);

	for(i = 0; i < nbPong; i++)
	{
		*(atciCmdData->params[0].val8) = i;
		Param_Access(PING_REPLY1+i, atciCmdData->params[1].data, 0);
		Atci_Debug_Param_Data("INSTPONG", atciCmdData);

	}
	return ATCI_OK;
}

/*!-----------------------------------------------------------------------------
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
 * - ATCI_OK if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 *----------------------------------------------------------------------------*/
atci_status_t Exec_ATFC_Cmd(atci_cmd_t *atciCmdData)
{
	atci_status_t status;
	uint8_t i;

	phy_power_entry_t sPwrEntry;

	if(atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
	{
		//get configuration ID
		Atci_Cmd_Param_Init(atciCmdData);
		status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);

		if(status != ATCI_OK)
			return status;

		switch(*(atciCmdData->params[0].val8))
		{
			case FC_TX_PWR_0dB_ID:
			case FC_TX_PWR_m6dB_ID:
			case FC_TX_PWR_m12dB_ID:

				if(atciCmdData->cmdType == AT_CMD_READ_WITH_PARAM) //read command
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

					if(EX_PHY_GetPowerEntry(&sPwrEntry) != PHY_STATUS_OK)
						return ATCI_ERR;

					*(atciCmdData->params[1].val8) = sPwrEntry.sEntryValue.coarse;
					*(atciCmdData->params[2].val8) = sPwrEntry.sEntryValue.fine;
					*(atciCmdData->params[3].val8) = sPwrEntry.sEntryValue.micro;

					Atci_Resp_Data("ATFC", atciCmdData);
				}
				else //write command
				{
					for(i=0; i < FC_TX_PWR_CFG_NB_VAL; i++)
					{
						if(atciCmdData->cmdType != AT_CMD_WITH_PARAM_TO_GET)
							return ATCI_ERR_INV_NB_PARAM;

						status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
						if(status != ATCI_OK)
							return status;
					}

					if(atciCmdData->cmdType != AT_CMD_WITH_PARAM)
						return ATCI_ERR_INV_NB_PARAM;

					Atci_Debug_Param_Data("Set Fact Cfg. (TX PWR)", atciCmdData);/////////

					sPwrEntry.eEntryId = PHY_PMAX_minus_0db + *(atciCmdData->params[0].val8);

					sPwrEntry.sEntryValue.coarse	= *(atciCmdData->params[1].val8);
					sPwrEntry.sEntryValue.fine		= *(atciCmdData->params[2].val8);
					sPwrEntry.sEntryValue.micro		= *(atciCmdData->params[3].val8);
					if((sPwrEntry.sEntryValue.coarse < FC_TX_PWR_COARSE_MIN) || (sPwrEntry.sEntryValue.coarse > FC_TX_PWR_COARSE_MAX))
						return ATCI_ERR_INV_PARAM_VAL;
					if((sPwrEntry.sEntryValue.fine < FC_TX_PWR_FINE_MIN) || (sPwrEntry.sEntryValue.fine > FC_TX_PWR_FINE_MAX))
						return ATCI_ERR_INV_PARAM_VAL;
					if((sPwrEntry.sEntryValue.micro < FC_TX_PWR_MICRO_MIN) || (sPwrEntry.sEntryValue.micro > FC_TX_PWR_MICRO_MAX))
						return ATCI_ERR_INV_PARAM_VAL;

					if(EX_PHY_SetPowerEntry(&sPwrEntry) != PHY_STATUS_OK)
						return ATCI_ERR;
				}

				return ATCI_OK;


			case FC_PA_EN_ID:

				if(atciCmdData->cmdType == AT_CMD_READ_WITH_PARAM) //read command
				{
					Atci_Cmd_Param_Init(atciCmdData);
					atciCmdData->params[0].size = PARAM_INT8;
					Atci_Add_Cmd_Param_Resp(atciCmdData);
					atciCmdData->params[1].size = PARAM_INT8;
					Atci_Add_Cmd_Param_Resp(atciCmdData);
					*(atciCmdData->params[1].val8) = (uint8_t) EX_PHY_GetPa();
					Atci_Resp_Data("ATFC", atciCmdData);
				}
				else if(atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)//write command
				{
					status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
					if(status != ATCI_OK)
						return status;
					if(atciCmdData->cmdType != AT_CMD_WITH_PARAM)
						return ATCI_ERR_INV_NB_PARAM;

					Atci_Debug_Param_Data("Set Fact Cfg. (PA EN)", atciCmdData);/////////

					if(*(atciCmdData->params[1].val8) == 0)
						EX_PHY_SetPa(0);
					else
						EX_PHY_SetPa(1);
				}
				else
					return ATCI_ERR_INV_NB_PARAM;

				return ATCI_OK;



			case FC_RSSI_CAL_ID:

				if(atciCmdData->cmdType == AT_CMD_WITH_PARAM) //write command
				{
					Atci_Debug_Param_Data("Set Fact Cfg. (CAL RSSI)", atciCmdData);/////////

					if(EX_PHY_RssiCalibrate(-77) != PHY_STATUS_OK)
						return ATCI_ERR;
				}
				else
					return ATCI_ERR_INV_NB_PARAM;

				return ATCI_OK;


			case FC_ADF7030_CAL_ID:

				if(atciCmdData->cmdType == AT_CMD_WITH_PARAM) //write command
				{
					Atci_Debug_Param_Data("Set Fact Cfg. (CAL ADF7030)", atciCmdData);/////////

					if(EX_PHY_AutoCalibrate() != PHY_STATUS_OK)
						return ATCI_ERR;
				}
				else
					return ATCI_ERR_INV_NB_PARAM;

				return ATCI_OK;


			default:
				return ATCI_ERR_INV_PARAM_VAL;
		}
	}
	else
		return ATCI_ERR_INV_NB_PARAM;
}

/*!-----------------------------------------------------------------------------
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
 * - ATCI_OK if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 *----------------------------------------------------------------------------*/
atci_status_t Exec_ATTEST_Cmd(atci_cmd_t *atciCmdData)
{
	uint8_t eRet = ATCI_OK;
	atci_status_t status;

	if(atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
	{
		//get test mode
		Atci_Cmd_Param_Init(atciCmdData);
		status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
		if(status != ATCI_OK)
			return status;

		if(atciCmdData->cmdType != AT_CMD_WITH_PARAM)
			return ATCI_ERR_INV_NB_PARAM;

		test_mode_info_t eTestModeInfo = _atci_init_test_var_();
		_bTestMode_ = 1;
		if(*(atciCmdData->params[0].val8) == TEST_MODE_DIS) //also TMODE_TX_NONE witch correspond to the same thing (see test_modes_tx_e)
		{
			Atci_Debug_Param_Data("Set Fact Cfg. (DIS TEST MODE)", atciCmdData);/////////

			eTestModeInfo.eTestMode = PHY_TST_MODE_NONE;
			eTestModeInfo.eTxMode = 0;
			_bTestMode_ = 0;
		}
		else if(*(atciCmdData->params[0].val8) < TMODE_TX_NB) // (see test_modes_tx_e)
		{
			Atci_Debug_Param_Data("Set Fact Cfg. (TX TEST MODE)", atciCmdData);/////////

			eTestModeInfo.eTestMode = PHY_TST_MODE_TX;
			eTestModeInfo.eTxMode = *(atciCmdData->params[0].val8);
		}
		else if(*(atciCmdData->params[0].val8) == TEST_MODE_RX_0)
		{
			Atci_Debug_Param_Data("Set Fact Cfg. (RX TEST MODE)", atciCmdData);/////////

			eTestModeInfo.eTestMode = PHY_TST_MODE_RX;
			eTestModeInfo.eTxMode = 0;
		}
		else if(*(atciCmdData->params[0].val8) == TEST_MODE_RX_1)
		{
			Atci_Debug_Param_Data("Set Fact Cfg. (RX TEST MODE)", atciCmdData);/////////

			eTestModeInfo.eTestMode = PHY_TST_MODE_RX;
			eTestModeInfo.eTxMode = 1;
		}
		else
		{
			eRet = ATCI_ERR_INV_PARAM_VAL;
		}

		if (eRet == ATCI_OK)
		{
			if (EX_PHY_Test(eTestModeInfo) != eTestModeInfo.eTestMode)
			{
				eRet = ATCI_ERR;
			}
		}

		if (eRet != ATCI_OK)
		{
			_bTestMode_ = 0;
		}
	}
	else
		eRet = ATCI_ERR_INV_NB_PARAM;

	return eRet;
}

/******************************************************************************/

/******************************************************************************/
/******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */

static test_mode_info_t _atci_init_test_var_(void)
{
	test_mode_info_t eTestModeInfo;
	// Init test variable
	eTestModeInfo.eChannel = TEST_MODE_DEF_CH;
	eTestModeInfo.eModulation = TEST_MODE_DEF_MOD;
	eTestModeInfo.eTestMode = PHY_TST_MODE_NONE;
	eTestModeInfo.eTxMode = TMODE_TX_NONE;
#ifdef HAS_TEST_CFG_PARAMETER
	uint8_t t;
	Param_Access(TEST_MODE_CHANNEL, &t, 0);
	if (!t)	{ t = 120; }
	eTestModeInfo.eChannel = (phy_chan_e)((t -100)/10);
	Param_Access(TEST_MODE_MODULATION, &t, 0);
	eTestModeInfo.eModulation = (phy_mod_e)t;
#endif
	return eTestModeInfo;
}

static uint8_t _atci_init_lp_var_(void)
{
	// Init LP mode
	uint32_t u32LPdelay = CONSOLE_RX_TIMEOUT;
	uint8_t eLPmode = 1;
#ifdef HAS_LP_PARAMETER
	/*
	 *  0b xxxx xxxx
	 *  0b xxxx xx00 : disable
	 *  0b xxxx xx01 : enable
	 *  0b 0000 xx01 : manual LP (no TMO)
	 *  0b 0001 xx01 : 1 seconds
	 *  0b 0010 xx01 : 2 seconds
	 *  0b 0011 xx01 : 3 seconds
	 *  ....
	 *  0b 1111 xx01 : 15 seconds
	 */
	Param_Access(LOW_POWER_MODE, &eLPmode, 0);
	u32LPdelay = (eLPmode >> 4) * 1000;
	eLPmode = eLPmode & 0x1;
	if (!u32LPdelay)
	{
		u32LPdelay = 0xFFFFFFFF;
	}
#endif
	BSP_Console_SetTXTmo(CONSOLE_TX_TIMEOUT);
	//BSP_Console_SetRXTmo(u32LPdelay);
	Atci_Rx_Cmd_Tmo(u32LPdelay);
	return eLPmode;
}

/*!
 * @}
 * @endcond
 */
/*********************************** EOF **************************************/

/*! @} */
