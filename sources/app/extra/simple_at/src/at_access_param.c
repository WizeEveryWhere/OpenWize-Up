/**
  * @file: at_access_param.c
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

#include "at_access_param.h"

#include "atci_get_cmd.h"
#include "atci_resp.h"

#include "app_entry.h"

/******************************************************************************/

atci_error_e Exec_AT_Cmd(atci_cmd_t *atciCmdData)
{
	if(atciCmdData->cmdType != AT_CMD_WITHOUT_PARAM)
		return ATCI_ERR_PARAM_NB;

	return ATCI_ERR_NONE;
}

atci_error_e Exec_ATI_Cmd(atci_cmd_t *atciCmdData)
{
	int sz;
	int len;
	uint8_t next = 0;

	if(atciCmdData->cmdType != AT_CMD_WITHOUT_PARAM)
		return ATCI_ERR_PARAM_NB;

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

	Atci_Resp(atciCmdData);
	// ---
	Atci_Cmd_Param_Init(atciCmdData);
	len = snprintf(atciCmdData->params[atciCmdData->nbParams].str, (AT_CMD_DATA_MAX_LEN-1), "%s", sFwInfo.build_date);
	if (len > 0)
	{
		atciCmdData->params[atciCmdData->nbParams].size = (len + 1) | PARAM_STR;
		Atci_Add_Cmd_Param_Resp(atciCmdData);
		Atci_Debug_Param_Data("Compilation date", atciCmdData);
	}
	return ATCI_ERR_NONE;
}

/******************************************************************************/

atci_error_e Exec_ATPARAM_Cmd(atci_cmd_t *atciCmdData)
{
	atci_error_e status;
	param_access_e regAccess;
	uint8_t regId;

	Atci_Cmd_Param_Init(atciCmdData);

	if (atciCmdData->cmdType == AT_CMD_READ_WITHOUT_PARAM) //read all registers command
	{
		atciCmdData->params[0].size = PARAM_INT8;
		Atci_Add_Cmd_Param_Resp(atciCmdData);

		atciCmdData->params[1].size = PARAM_VARIABLE_LEN;
		Atci_Add_Cmd_Param_Resp(atciCmdData);

		for (regId = 0; regId < PARAM_ACCESS_CFG_SZ; regId++)
		{
			if (Param_GetLocAccess(regId) & RO)
			{
				*(atciCmdData->params[0].val8) = regId;

				atciCmdData->params[1].size = (uint16_t) Param_GetSize(regId);

				if (atciCmdData->params[1].size > (AT_CMD_DATA_MAX_LEN-atciCmdData->paramsMemIdx))
				{
					return ATCI_ERR_UNK;
				}
				Param_Access(regId, atciCmdData->params[1].data, 0);
				Atci_Resp(atciCmdData);
				//Atci_Resp_Data(atciCmdData->cmd_code_str[atciCmdData->cmdCode], atciCmdData);
			}
		}
		return ATCI_ERR_NONE;
	}
	else if (atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET) //read/write one register command
	{
		//get register address
		status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
		if (status != ATCI_ERR_NONE)
			return status;

		//check param address:
		regAccess = Param_GetLocAccess(*(atciCmdData->params[0].val8));
		if (regAccess == NA)
			return ATCI_ERR_PARAM_VAL;

		//get param size:
		atciCmdData->params[1].size = (uint16_t) Param_GetSize(*(atciCmdData->params[0].val8));

		if(atciCmdData->params[1].size == 0)
			return ATCI_ERR_UNK;
		else if(atciCmdData->params[1].size == 1)
			atciCmdData->params[1].size = PARAM_INT8;
		else if(atciCmdData->params[1].size == 2)
			atciCmdData->params[1].size = PARAM_INT16;
		else if(atciCmdData->params[1].size == 4)
			atciCmdData->params[1].size = PARAM_INT32;
		else if(atciCmdData->params[1].size > (AT_CMD_DATA_MAX_LEN-atciCmdData->paramsMemIdx))
			return ATCI_ERR_UNK;

		if (atciCmdData->cmdType == AT_CMD_READ_WITH_PARAM) //read one register command
		{
			//check if param can be read:
			if ((regAccess & RO) == 0)
				return ATCI_ERR_PARAM_VAL;

			//register 2nd param in params list
			Atci_Add_Cmd_Param_Resp(atciCmdData);

			//get parameter value
			if (!Param_Access(*(atciCmdData->params[0].val8), atciCmdData->params[1].data, 0))
				return ATCI_ERR_UNK;

			//display parameter
			Atci_Resp(atciCmdData);
			//Atci_Resp_Data(atciCmdData->cmd_code_str[atciCmdData->cmdCode], atciCmdData);

			return ATCI_ERR_NONE;
		}
		else if (atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET) //write one register command
		{
			//check if param can be written:
			if ((regAccess & WO) == 0)
				return ATCI_ERR_PARAM_VAL;

			//get register value:
			status = Atci_Buf_Get_Cmd_Param(atciCmdData, atciCmdData->params[1].size);
			if (status != ATCI_ERR_NONE)
				return status;
			else if (atciCmdData->cmdType != AT_CMD_WITH_PARAM)
				return ATCI_ERR_PARAM_NB;

			//check param value:
			if (!Param_CheckConformity(*(atciCmdData->params[0].val8), atciCmdData->params[1].data))
				return ATCI_ERR_PARAM_VAL;

			//write param value:
			if (!Param_Access(*(atciCmdData->params[0].val8), atciCmdData->params[1].data, 1))
				return ATCI_ERR_UNK;

			Atci_Debug_Param_Data("Write register", atciCmdData);/////////

			return ATCI_ERR_NONE;
		}
		else
			return ATCI_ERR_PARAM_NB;
	}
	else
		return ATCI_ERR_PARAM_NB;

}

/******************************************************************************/

atci_error_e Exec_ATIDENT_Cmd(atci_cmd_t *atciCmdData)
{
	atci_error_e status;

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

		Atci_Resp(atciCmdData);
		//Atci_Resp_Data(atciCmdData->cmd_code_str[atciCmdData->cmdCode], atciCmdData);
		return ATCI_ERR_NONE;
	}
	else if(atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET) //write command
	{
		//get M-field
		Atci_Cmd_Param_Init(atciCmdData);
		status = Atci_Buf_Get_Cmd_Param(atciCmdData, ATIDENT_MFIELD_LEN);

		if(status != ATCI_ERR_NONE)
			return status;

		if(atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
		{
			//get A-field
			status = Atci_Buf_Get_Cmd_Param(atciCmdData, ATIDENT_AFIELD_LEN);

			if(status != ATCI_ERR_NONE)
				return status;

			if(atciCmdData->cmdType == AT_CMD_WITH_PARAM)
			{
				//write M-field & A-field:
				if ( WizeApi_SetDeviceId( (device_id_t *)(atciCmdData->params[0].data) ) != WIZE_API_SUCCESS)
				{
					Atci_Debug_Param_Data("Write IDENT", atciCmdData);/////////
					return ATCI_ERR_UNK;
				}
				Atci_Debug_Param_Data("Write IDENT", atciCmdData);/////////

				return ATCI_ERR_NONE;
			}
			else
				return ATCI_ERR_PARAM_NB;
		}
		else
			return ATCI_ERR_PARAM_NB;
	}
	else
		return ATCI_ERR_PARAM_NB;
}

/******************************************************************************/

#ifdef __cplusplus
}
#endif

/*! @} */
