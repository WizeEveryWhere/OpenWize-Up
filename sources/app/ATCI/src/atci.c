/**********************************************************************************************************
  * @file: atci.c
  * @brief: This file contains functions of the AT command interpreter for WizeUp
  * module
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
  * 0.0.1 : 2021/01/11
  * Dev. version
  *
  *
 *********************************************************************************************************/

/*=========================================================================================================
 * INCLUDES
 *=======================================================================================================*/

#include <stdint.h>

#include "atci.h"
#include "atci_get_cmd.h"
#include "atci_resp.h"
#include "console.h"

#include "bsp.h"

#include "wize_api.h"
#include "proto.h"
#include "parameters.h"
#include "parameters_cfg.h"
#include "crypto.h"
#include "storage.h"
#include "phy_layer_private.h"
#include "phy_test.h"

#include "FreeRTOS.h"
#include "task.h"

/*=========================================================================================================
 * GLOBAL VARIABLES
 *=======================================================================================================*/

atci_status_t (*Atci_Exec_Cmd[NB_AT_CMD])(atci_cmd_t *atciCmdData);

extern phydev_t sPhyDev;

/*=========================================================================================================
 * LOCAL FUNCTIONS PROTOTYPES
 *=======================================================================================================*/

atci_status_t Exec_AT_Cmd(atci_cmd_t *atciCmdData);
atci_status_t Exec_ATI_Cmd(atci_cmd_t *atciCmdData);
atci_status_t Exec_ATF_Cmd(atci_cmd_t *atciCmdData);
atci_status_t Exec_ATW_Cmd(atci_cmd_t *atciCmdData);
atci_status_t Exec_ATPARAM_Cmd(atci_cmd_t *atciCmdData);
atci_status_t Exec_ATKMAC_Cmd(atci_cmd_t *atciCmdData);
atci_status_t Exec_ATKENC_Cmd(atci_cmd_t *atciCmdData);
atci_status_t Exec_ATIDENT_Cmd(atci_cmd_t *atciCmdData);
atci_status_t Exec_ATSEND_Cmd(atci_cmd_t *atciCmdData);
atci_status_t Exec_ATPING_Cmd(atci_cmd_t *atciCmdData);
atci_status_t Exec_ATFC_Cmd(atci_cmd_t *atciCmdData);
atci_status_t Exec_ATTEST_Cmd(atci_cmd_t *atciCmdData);


/*=========================================================================================================
 * FUNCTIONS
 *=======================================================================================================*/

/*!--------------------------------------------------------------------------------------------------------
 * @brief		AT command interpreter task
 * 				Wait AT command reception, decode and execute it
 * 				Manage sleep and reset
 *
 * @param[IN]	argument: unused
 * @param[OUT]	None
 *
 * @return		None (this task never return)
 *-------------------------------------------------------------------------------------------------------*/
void Atci_Task(void const *argument)
{
	atci_state_t atciState = ATCI_WAKEUP;
	atci_cmd_t atciCmdData;
	atci_status_t status;
	uint8_t bPaState;

	//Inits

	Atci_Exec_Cmd[CMD_AT] = Exec_AT_Cmd; //nothing to do
	Atci_Exec_Cmd[CMD_ATI] = Exec_ATI_Cmd;
	Atci_Exec_Cmd[CMD_ATZ] = Exec_AT_Cmd; //something to do in states machine only
	Atci_Exec_Cmd[CMD_ATQ] = Exec_AT_Cmd; //something to do in states machine only
	Atci_Exec_Cmd[CMD_ATF] = Exec_ATF_Cmd;
	Atci_Exec_Cmd[CMD_ATW] = Exec_ATW_Cmd;
	Atci_Exec_Cmd[CMD_ATPARAM] = Exec_ATPARAM_Cmd;
	Atci_Exec_Cmd[CMD_ATKMAC] = Exec_ATKMAC_Cmd;
	Atci_Exec_Cmd[CMD_ATKENC] = Exec_ATKENC_Cmd;
	Atci_Exec_Cmd[CMD_ATIDENT] = Exec_ATIDENT_Cmd;
	Atci_Exec_Cmd[CMD_ATSEND] = Exec_ATSEND_Cmd;
	Atci_Exec_Cmd[CMD_ATPING] = Exec_ATPING_Cmd;
	Atci_Exec_Cmd[CMD_ATFC] = Exec_ATFC_Cmd;
	Atci_Exec_Cmd[CMD_ATTEST] = Exec_ATTEST_Cmd;

	EX_PHY_SetCpy();
	//Loop
	while(1)
	{
		switch(atciState)
		{
			case ATCI_WAKEUP:
				Atci_Send_Wakeup_Msg();
				Atci_Restart_Rx(&atciCmdData);
				atciState = ATCI_WAIT;
				break;

			case ATCI_WAIT:

				switch(Atci_Rx_Cmd(&atciCmdData))
				{
					case ATCI_AVAIL_AT_CMD:
						atciState = ATCI_EXEC_CMD;
						break;
					case ATCI_RX_CMD_ERR:
						Atci_Resp_Ack(ATCI_RX_CMD_ERR);
						Atci_Debug_Str("Command RX error!!!");
						Atci_Restart_Rx(&atciCmdData);
						break;
					case ATCI_RX_CMD_TIMEOUT:
						Atci_Restart_Rx(&atciCmdData);
						Atci_Send_Sleep_Msg();
						atciState = ATCI_SLEEP;
						break;
					default:
						break;
				}

				break;

			case ATCI_EXEC_CMD:

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
					Atci_Restart_Rx(&atciCmdData);
					switch(atciCmdData.cmdCode)
					{
						case CMD_ATZ:
							atciState = ATCI_RESET;
							break;
						case CMD_ATQ:
							Atci_Send_Sleep_Msg();
							atciState = ATCI_SLEEP;
							break;
						default: //other commands
							atciState = ATCI_WAIT;
							break;
					}
				}
				else
				{
					Atci_Resp_Ack(status);
					/////////////////////////
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
						default:
							Atci_Debug_Str("Command execution error!!!");
							break;
					}
					/////////////////////////
					Atci_Restart_Rx(&atciCmdData);
					atciState = ATCI_WAIT;
				}
				break;

			case ATCI_SLEEP:
#ifdef HAS_LPOWER
				Atci_Debug_Str("Sleep");
				CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
		        bPaState = Phy_GetPa();
				Phy_OnOff(&sPhyDev, 0);
				BSP_LowPower_Enter(LP_STOP2_MODE);
				Phy_OnOff(&sPhyDev, 1);
		        Phy_SetPa(bPaState);
		        SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
#else
				Atci_Debug_Str("No sleep!");
#endif
				atciState = ATCI_WAKEUP;
				break;
			default:
			case ATCI_RESET:
				Atci_Debug_Str("Reset");
				atciState = ATCI_WAKEUP;
				BSP_Boot_Reboot(1);
				break;
		}
	}
}


/*=========================================================================================================
 * LOCAL FUNCTIONS - commands executions
 *=======================================================================================================*/

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Execute AT command (nothing to do)
 *
 * @param[I/O]	atciCmdData ("atci_cmd_t" structure)
 *
 * @return		status: ATCI_OK if succeed, else error code (ATCI_INV_NB_PARAM_ERR)
 *-------------------------------------------------------------------------------------------------------*/
atci_status_t Exec_AT_Cmd(atci_cmd_t *atciCmdData)
{
	if(atciCmdData->cmdType != AT_CMD_WITHOUT_PARAM)
		return ATCI_ERR_INV_NB_PARAM;

	return ATCI_OK;
}

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Execute ATI command (Queries the identification of the module)
 * 				Command format: "ATI"
 * 				Response format: "+ATI :"WIZEUP",<manufacturer>,<model>,<hw version>,<major sw version>,<minor sw version>"
 *
 * @param[I/O]	atciCmdData ("atci_cmd_t" structure)
 *
 * @return		status: ATCI_OK if succeed, else error code (ATCI_INV_NB_PARAM_ERR or ATCI_ERR)
 *-------------------------------------------------------------------------------------------------------*/
atci_status_t Exec_ATI_Cmd(atci_cmd_t *atciCmdData)
{
	uint16_t tmp;

	if(atciCmdData->cmdType != AT_CMD_WITHOUT_PARAM)
		return ATCI_ERR_INV_NB_PARAM;

	//Get infos: TODO
	Atci_Cmd_Param_Init(atciCmdData);
	//	Name
	strcpy(atciCmdData->params[atciCmdData->nbParams].str, "WIZEUP");
	atciCmdData->params[atciCmdData->nbParams].size = (strlen(atciCmdData->params[atciCmdData->nbParams].str) + 1) | PARAM_STR;
	Atci_Add_Cmd_Param_Resp(atciCmdData);
	//	Manufacturer
	strcpy(atciCmdData->params[atciCmdData->nbParams].str, "ALCIOM");
	atciCmdData->params[atciCmdData->nbParams].size = (strlen(atciCmdData->params[atciCmdData->nbParams].str) + 1) | PARAM_STR;
	Atci_Add_Cmd_Param_Resp(atciCmdData);
	//	Model
	strcpy(atciCmdData->params[atciCmdData->nbParams].str, "WZ1000");
	atciCmdData->params[atciCmdData->nbParams].size = (strlen(atciCmdData->params[atciCmdData->nbParams].str) + 1) | PARAM_STR;
	Atci_Add_Cmd_Param_Resp(atciCmdData);

	// HW version
	Param_Access(VERS_HW_TRX, (uint8_t *) &tmp, 0);
	sprintf(atciCmdData->params[atciCmdData->nbParams].str, "%04X", tmp);
	atciCmdData->params[atciCmdData->nbParams].size = (strlen(atciCmdData->params[atciCmdData->nbParams].str) + 1) | PARAM_STR;
	Atci_Add_Cmd_Param_Resp(atciCmdData);
	// Major SW version
	Param_Access(VERS_FW_TRX, (uint8_t *) &tmp, 0);
	*(atciCmdData->params[atciCmdData->nbParams].val8) = (uint8_t) (tmp>>8);
	atciCmdData->params[atciCmdData->nbParams].size = PARAM_INT8;
	Atci_Add_Cmd_Param_Resp(atciCmdData);
	// Minor SW version
	*(atciCmdData->params[atciCmdData->nbParams].val8) = (uint8_t) tmp;
	atciCmdData->params[atciCmdData->nbParams].size = PARAM_INT8;
	Atci_Add_Cmd_Param_Resp(atciCmdData);


	Atci_Resp_Data("ATI", atciCmdData); //"\r\n+ATI:\"WIZEUP\",\"ALCIOM\",\"WZ1000\",\"1C\",$00,$00\r\n"

	Atci_Debug_Printf("Compilation date: %s", __DATE__);//////////////////

	return ATCI_OK;
}

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Execute AT&F command (Restore registers to their factory settings)
 * 				Command format: "AT&F"
 * 				No specific response
 *
 * @param[I/O]	atciCmdData ("atci_cmd_t" structure)
 *
 * @return		status: ATCI_OK if succeed, else error code (ATCI_INV_NB_PARAM_ERR or ATCI_ERR)
 *-------------------------------------------------------------------------------------------------------*/
atci_status_t Exec_ATF_Cmd(atci_cmd_t *atciCmdData)
{
	if(atciCmdData->cmdType != AT_CMD_WITHOUT_PARAM)
		return ATCI_ERR_INV_NB_PARAM;

	Atci_Debug_Str("Restore to Factory settings");
	Storage_SetDefault();
	return ATCI_OK;
}

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Execute AT&W command (Store current registers values in flash)
 * 					Command format: "AT&W"
 * 					No specific response
 *
 * @param[I/O]	atciCmdData ("atci_cmd_t" structure)
 *
 * @return		status: ATCI_OK if succeed, else error code (ATCI_INV_NB_PARAM_ERR or ATCI_ERR)
 *-------------------------------------------------------------------------------------------------------*/
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

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Execute ATPARAM command (Modify/read the value of a Wize LAN parameter)
 * 				This command may be a read or a write command:
 * 					"ATPARAM?" -> read all registers
 * 					"ATPARAM=<address>?" -> read register of address <address>
 * 					"ATPARAM=<address>,<value>" -> write <value> to register of address <address> (1 byte)
 * 				Read response format:
 * 					"+ATPARAM:<address>,<value>" (this response is send for each register in read all register mode)
 * 				Fields:
 * 					<address> is the register address (decimal or hexadecimal 8 bits number)
 *					<value> may be a 8, 16 or 32 bits integer (decimal or hexadecimal format may be used)
 *					  or an array (hexadecimal format must be used) -> refer to register list in specifications
 *
 * @param[I/O]	atciCmdData ("atci_cmd_t" structure)
 * 					params[0] is used for the register address witch is always a 8 bits integer
 * 					params[1] is used for the register value witch can be a 8, 16 or 32 bits integer or a bytes array
 *
 * @return		status: ATCI_OK if succeed, else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *-------------------------------------------------------------------------------------------------------*/
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

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Execute ATKMAC command (Modify the value of the Kmac key)
 * 				ATKMAC command is a write only command:
 * 					"ATKMAC=<key>" <key> is a 16 or 32 bytes key (32 bytes KMAC but only the 16 1st bytes uses)
 * 						and must be written in hexadecimal format (with "$" char)
 * 				No specific response
 *
 * @param[I/O]	atciCmdData ("atci_cmd_t" structure)
 * 					params[0] is used for the key (16 bytes in command but extended to 32 bytes by adding zeros
 * 						because a 32 bytes key is needed but only 16 bytes are used)
 *
 * @return		status: ATCI_OK if succeed, else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *-------------------------------------------------------------------------------------------------------*/
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

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Execute ATKENC command (Modify the value of the Kenc key)
 * 				ATKENC command is a write only command:
 * 					"ATKENC=<id>,<key>"	<id> is the key number (decimal or hexadecimal 1 byte number)
 * 										<key> is a 16 or 32 bytes key (32 bytes KENC but only the 16 1st bytes uses)
 * 											and must be written in hexadecimal format (with "$" char)
 * 				No specific response
 *
 * @param[I/O]	atciCmdData ("atci_cmd_t" structure)
 *
 * @return		status: ATCI_OK if succeed, else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *-------------------------------------------------------------------------------------------------------*/
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
		if((*(atciCmdData->params[0].val8) < KEY_ENC_MIN) || (*(atciCmdData->params[0].val8) > KEY_ENC_MAX))
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

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Execute ATIDENT command (Modify/read the value of M-field and A-field)
 * 				This command may be a read or a write command:
 * 					"ATIDENT=<M-field>,<A-field>" -> write M-field and A-field
 * 					"ATIDENT?" -> read M-field and A-field
 * 					<M-field> is 2 bytes array (must be in hex format)
 * 					<A-field> is 6 bytes array (must be in hex format)
 * 				Response to a read command:
 * 					"+ATIDENT:<M-field>,<A-field>"
 *
 * @param[I/O]	atciCmdData ("atci_cmd_t" structure)
 *
 * @return		status: ATCI_OK if succeed, else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *-------------------------------------------------------------------------------------------------------*/
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
					Atci_Debug_Param_Data("Write IDENT Failed", atciCmdData);/////////
					return ATCI_ERR;
				}
				Atci_Debug_Param_Data("Write IDENT succeed", atciCmdData);/////////

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

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Execute ATSEND command (Send a Wize message)
 * 				Command format:
 * 					"ATSEND=<l6app>,<l7msg>"
 * 						<l6app> is the layer 6 application code used to give for witch application are the L7 data (decimal or hexadecimal 8bits integer)
 * 						<l7msg> is the layer 7 message to send (array in hexadecimal format, maximum length is 102 bytes for PRES-EXCHANGE L6 frames)
 * 				Maybe 2 types of responses:
 * 						- an APP-ADMIN write command was received and processed by the on-board Wize stack
 * 							message: "+ATADMWRITE :<paramid>,<paramvalue>,<rssi>"
 * 						- a response was received in response of the Wize message, which can’t be managed
 * 							by the on-board Wize stack (other application layer than APP-ADMIN)
 * 							message: "+ATRCV:<L6App>,<l7resp>,<rssi>"
 *
 *
 * @param[I/O]	atciCmdData ("atci_cmd_t" structure)
 *
 * @return		status: ATCI_OK if succeed, else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *-------------------------------------------------------------------------------------------------------*/
atci_status_t Exec_ATSEND_Cmd(atci_cmd_t *atciCmdData)
{
	atci_status_t status;
	uint8_t sta;
	net_msg_t rxMsg;
	uint8_t i;

	if(atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
	{
		//get L6-app field
		Atci_Cmd_Param_Init(atciCmdData);
		status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);

		if(status != ATCI_OK)
			return status;

		if(atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
		{
			//get L7 message
			status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_VARIABLE_LEN);

			if(status != ATCI_OK)
				return status;

			if(atciCmdData->params[1].size > ATSEND_L7_MAX_MSG_LEN)
				return ATCI_ERR_INV_PARAM_LEN;

			if(atciCmdData->cmdType == AT_CMD_WITH_PARAM)
			{
				Atci_Debug_Param_Data("Send Frame.", atciCmdData);/////////

				//send frame and wait for response:
				sta = WIZE_API_FAILED;
				sta = WizeApi_SendEx(atciCmdData->paramsMem, atciCmdData->params[1].size+1, APP_DATA);
				if(sta == WIZE_API_ADM_SUCCESS)
				{
					//APP-ADMIN write command reception
					// msg format: <cmd ID 1 (1 byte)><cmd val 1 (s1 bytes)>...<cmd ID n (1 byte)><cmd val n (sn bytes)>
					atciCmdData->nbParams = 3;
					atciCmdData->params[0].size = PARAM_INT8;
					atciCmdData->params[2].size = PARAM_INT8;
					atciCmdData->params[2].data = &(atciCmdData->paramsMem[AT_CMD_DATA_MAX_LEN-1]);
					rxMsg.pData = atciCmdData->paramsMem; //write directly RX message in params memory
					sta = WizeApi_GetAdmCmd(&rxMsg);
					if (sta == WIZE_API_SUCCESS)
					{
						//TODO: Rx msg format to be verified
						//get RSSI
						*(atciCmdData->params[2].val8) = rxMsg.u8Rssi;
						i=1;
						while(i<rxMsg.u8Size)
						{
							//get param ID (1st byte of received message)
							atciCmdData->params[0].data = &(atciCmdData->paramsMem[i++]);
							//get param Value (next bytes of received message)
							atciCmdData->params[1].size = (uint16_t) Param_GetSize(*(atciCmdData->params[0].val8));
							atciCmdData->params[1].data = &(atciCmdData->paramsMem[i]);
							i += atciCmdData->params[1].size;

							//send received APP-ADMIN command
							Atci_Resp_Data("ATADMWRITE", atciCmdData);
						}
					}

					//Response of the Wize message reception
					Atci_Cmd_Param_Init(atciCmdData);
					rxMsg.pData = atciCmdData->paramsMem; //write directly RX message in params memory
					sta = WizeApi_GetAdmRsp(&rxMsg);
					if (sta == WIZE_API_SUCCESS)
					{
#if 0
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
#warning "ATRCV is not available"
#warning "ATRCV is not available"
#endif
					}

					return ATCI_OK;
				}
				else if(sta == WIZE_API_SUCCESS)
					return ATCI_OK;
				else
					return ATCI_ERR;

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

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Execute ATPING command (Send an INSTPING request)
 * 				Command format: "ATPING"
 * 				No specific response (read dedicated registers with ATPARAM command)
 *
 * @param[I/O]	atciCmdData ("atci_cmd_t" structure)
 *
 * @return		status: ATCI_OK if succeed, else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *-------------------------------------------------------------------------------------------------------*/
atci_status_t Exec_ATPING_Cmd(atci_cmd_t *atciCmdData)
{
	uint8_t sta;
	uint8_t nbPong, i;

	if(atciCmdData->cmdType != AT_CMD_WITHOUT_PARAM)
		return ATCI_ERR_INV_NB_PARAM;

	Atci_Debug_Str("Send PING");/////////

	nbPong = 0;
	Param_Access(PING_NBFOUND, &nbPong, 1);

	sta = WIZE_API_FAILED;
	sta = WizeApi_ExecPing();
	if(sta == WIZE_API_SUCCESS)
	{
		////////////////
		Param_LocalAccess(PING_NBFOUND, &nbPong, 0);
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
			Param_LocalAccess(PING_REPLY1+i, atciCmdData->params[1].data, 0);
			Atci_Debug_Param_Data("INSTPONG", atciCmdData);

		}
		////////////////

		return ATCI_OK;
	}
	else
		return ATCI_ERR;

	return ATCI_OK;
}

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Execute ATFC command (set or get factory configuration)
 * 				This command may be a read or a write command:
 * 					"ATFC=<id>?" -> read configuration <id> (if parameter available)
 * 					"ATPARAM=<id>,<value_1>,<value_2>,...,<value_n>" -> write one or more values to a configuration
 * 				Read response format:
 * 					"+ATPARAM:<id>,<value_1>,<value_2>,...,<value_n>"
 * 				Fields:
 * 					<id> is the configuration ID (decimal or hexadecimal 8 bits number)
 *					<value_1>,<value_2>... one or more values:  may be a 8, 16 or 32 bits integer (decimal or hexadecimal format may be used)
 *					  or an array (hexadecimal format must be used)
 *				Configurations:
 *					- ADF7030 output power configuration: <id>,<value_1>,<value_2>,<value_3>
 *						<id> = 0x00 -> configuration for max power
 *						<id> = 0x01 -> configuration for 6dB under max power
 *						<id> = 0x02 -> configuration for 12dB under max power
 *						<value_1> -> coarse PA settings, from 1 to 6 (8 bits integer, see ADF7030 Datasheet)
 *						<value_2> -> fine PA settings from 0 to 255 (8 bits integer, see ADF7030 Datasheet)
 *						<value_3> -> micro PA settings from 0 to 255 (8 bits integer, see ADF7030 Datasheet)
 *					- Power amplifier (SKY66100-11) enable: <id>,<value_1>  (<value_1> is 8 bits integer)
 *						<id> = 0x10
 *						<value_1> = 0 -> Power amplifier is bypassed in TX
 *						<value_1> = 1 -> Power amplifier is enabled in TX
 *					- RSSI calibration (Apply Carrier at mid band frequency with -77dbm level): <id>
 *						<id> = 0x20
 *						There is no parameter, read will return an error
 *					- Auto-Calibration: <id>
 *						<id> = 0xFC
 *						There is no parameter, read will return an error
 *
 * @param[I/O]	atciCmdData ("atci_cmd_t" structure)
 *
 * @return		status: ATCI_OK if succeed, else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *-------------------------------------------------------------------------------------------------------*/
atci_status_t Exec_ATFC_Cmd(atci_cmd_t *atciCmdData)
{
	atci_status_t status;
	uint8_t i;

	phy_power_e eEntryId;
	phy_power_t sPwrEntry;

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

					eEntryId = PHY_PMAX_minus_0db + *(atciCmdData->params[0].val8);

					if(Phy_GetPowerEntry(&sPhyDev, eEntryId, &sPwrEntry) != PHY_STATUS_OK)
						return ATCI_ERR;

					*(atciCmdData->params[1].val8) = sPwrEntry.coarse;
					*(atciCmdData->params[2].val8) = sPwrEntry.fine;
					*(atciCmdData->params[3].val8) = sPwrEntry.micro;

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

					eEntryId = PHY_PMAX_minus_0db + *(atciCmdData->params[0].val8);

					sPwrEntry.coarse	= *(atciCmdData->params[1].val8);
					sPwrEntry.fine		= *(atciCmdData->params[2].val8);
					sPwrEntry.micro		= *(atciCmdData->params[3].val8);
					if((sPwrEntry.coarse < FC_TX_PWR_COARSE_MIN) || (sPwrEntry.coarse > FC_TX_PWR_COARSE_MAX))
						return ATCI_ERR_INV_PARAM_VAL;
					if((sPwrEntry.fine < FC_TX_PWR_FINE_MIN) || (sPwrEntry.fine > FC_TX_PWR_FINE_MAX))
						return ATCI_ERR_INV_PARAM_VAL;
					if((sPwrEntry.micro < FC_TX_PWR_MICRO_MIN) || (sPwrEntry.micro > FC_TX_PWR_MICRO_MAX))
						return ATCI_ERR_INV_PARAM_VAL;

					if(Phy_SetPowerEntry(&sPhyDev, eEntryId, sPwrEntry) != PHY_STATUS_OK)
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

					*(atciCmdData->params[1].val8) = (uint8_t) Phy_GetPa();

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
						Phy_SetPa(0);
					else
						Phy_SetPa(1);
				}
				else
					return ATCI_ERR_INV_NB_PARAM;

				return ATCI_OK;



			case FC_RSSI_CAL_ID:

				if(atciCmdData->cmdType == AT_CMD_WITH_PARAM) //write command
				{
					Atci_Debug_Param_Data("Set Fact Cfg. (CAL RSSI)", atciCmdData);/////////

					if(Phy_RssiCalibrate(&sPhyDev) != PHY_STATUS_OK)
						return ATCI_ERR;
				}
				else
					return ATCI_ERR_INV_NB_PARAM;

				return ATCI_OK;


			case FC_ADF7030_CAL_ID:

				if(atciCmdData->cmdType == AT_CMD_WITH_PARAM) //write command
				{
					Atci_Debug_Param_Data("Set Fact Cfg. (CAL ADF7030)", atciCmdData);/////////

					if(Phy_AutoCalibrate(&sPhyDev) != PHY_STATUS_OK)
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

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Execute TEST command (enable or disable a test mode)
 * 				This command is a write only command:
 * 					"ATTEST=<test_mode>" -> enable or disable a test mode
 * 				Fields:
 * 					<test_mode> = 0x00 -> disable test mode (RX or TX test)
 * 					<test_mode> = 0x01 -> enable TX test mode, transmit a carrier
 * 					<test_mode> = 0x02 -> enable TX test mode, transmit frequency deviation tone, −fDEV, in 2FSK or off in OOK
 * 					<test_mode> = 0x03 -> enable TX test mode, transmit −fDEV_MAX in 4FSK only
 * 					<test_mode> = 0x04 -> enable TX test mode, transmit +fDEV in 2FSK or on in OOK
 * 					<test_mode> = 0x05 -> enable TX test mode, transmit +fDEV_MAX in 4FSK only
 * 					<test_mode> = 0x06 -> enable TX test mode, transmit transmit preamble pattern.
 * 					<test_mode> = 0x07 -> enable TX test mode, transmit pseudorandom (PN9) sequence
 * 					<test_mode> = 0x10 -> enable RX test mode, get copy of SPORT_CLK to EXT_I2C_SCL and SPORT_DATA to EXT_I2C_SDA
 * 					<test_mode> = 0x11 -> enable RX test mode, get PREAMBLE detect on EXT_I2C_SCL and SYNCH detect on EXT_I2C_SDA
 *
 * @param[I/O]	atciCmdData ("atci_cmd_t" structure)
 *
 * @return		status: ATCI_OK if succeed, else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *-------------------------------------------------------------------------------------------------------*/
atci_status_t Exec_ATTEST_Cmd(atci_cmd_t *atciCmdData)
{
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


		if(*(atciCmdData->params[0].val8) == TEST_MODE_DIS) //also TMODE_TX_NONE witch correspond to the same thing (see test_modes_tx_e)
		{
			Atci_Debug_Param_Data("Set Fact Cfg. (DIS TEST MODE)", atciCmdData);/////////

			EX_PHY_Test(PHY_TST_MODE_NONE, 0);
		}
		else if(*(atciCmdData->params[0].val8) < TMODE_TX_NB) // (see test_modes_tx_e)
		{
			Atci_Debug_Param_Data("Set Fact Cfg. (TX TEST MODE)", atciCmdData);/////////

			if(EX_PHY_Test(PHY_TST_MODE_TX, *(atciCmdData->params[0].val8)) != PHY_TST_MODE_TX)
				return ATCI_ERR;
		}
		else if(*(atciCmdData->params[0].val8) == TEST_MODE_RX_0)
		{
			Atci_Debug_Param_Data("Set Fact Cfg. (RX TEST MODE)", atciCmdData);/////////

			if(EX_PHY_Test(PHY_TST_MODE_RX, 0) != PHY_TST_MODE_RX)
				return ATCI_ERR;
		}
		else if(*(atciCmdData->params[0].val8) == TEST_MODE_RX_1)
		{
			Atci_Debug_Param_Data("Set Fact Cfg. (RX TEST MODE)", atciCmdData);/////////

			if(EX_PHY_Test(PHY_TST_MODE_RX, 1) != PHY_TST_MODE_RX)
				return ATCI_ERR;
		}
		else
			return ATCI_ERR_INV_PARAM_VAL;
	}
	else
		return ATCI_ERR_INV_NB_PARAM;

	return ATCI_OK;
}



/************************************************** EOF **************************************************/



