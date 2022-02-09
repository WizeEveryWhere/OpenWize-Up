/**********************************************************************************************************
  * @file: atci.c
  * @brief: This file contains low level functions (command responses) of the AT command interpreter for WizeUp
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
#include <string.h>

#include "atci.h"
#include "console.h"

/*=========================================================================================================
 * GLOBAL VARIABLES
 *=======================================================================================================*/


/*=========================================================================================================
 * LOCAL FUNCTIONS PROTOTYPES
 *=======================================================================================================*/

/*=========================================================================================================
 * FUNCTIONS - AT responses
 *=======================================================================================================*/

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Send wakeup message
 *
 * @param[in]	None
 * @param[Out]	None
 *
 * @return		None
 *-------------------------------------------------------------------------------------------------------*/
void Atci_Send_Wakeup_Msg(void)
{
	Console_Send_Str("\r\n+WAKEUP\r\n");
}

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Send sleep message
 *
 * @param[in]	None
 * @param[Out]	None
 *
 * @return		None
 *-------------------------------------------------------------------------------------------------------*/
void Atci_Send_Sleep_Msg(void)
{
	Console_Send_Str("\r\n+SLEEP\r\n");
}


/*!--------------------------------------------------------------------------------------------------------
 * @brief		Send AT response status (OK or error)
 *
 * @param[in]	errCode: 0 for succeess, else error code
 * @param[Out]	None
 *
 * @return		None
 *-------------------------------------------------------------------------------------------------------*/
void Atci_Resp_Ack(atci_status_t errCode)
{
	if(errCode)
		Console_Printf("\r\nERROR:%02X\r\n", errCode);
	else
		Console_Send_Str("\r\nOK\r\n");
}


/*!--------------------------------------------------------------------------------------------------------
 * @brief		Send AT response data
 *
 * @param[IN]	cmdCodeStr: command code as string
 * @param[IN]	atciCmdData ("atci_cmd_t" structure):
 * 					nbParams: number of parameters in command response
 * 					params: parameters list (with size and data)
 *
 * @return		None
 *-------------------------------------------------------------------------------------------------------*/
void Atci_Resp_Data(char *cmdCodeStr, atci_cmd_t *atciCmdData)
{
	uint8_t i;

	Console_Send_Str("\r\n+"); //new line + prefix (beginning)
	Console_Send_Str(cmdCodeStr); //command code

	//each parameter data
	for(i=0; i<atciCmdData->nbParams; i++)
	{
		//parameter data as hexadecimal number / bytes array or string
		if(atciCmdData->params[i].size == PARAM_INT8)
		{
			if(i==0)
				Console_Send_Str(":$"); //command code / data separator + hex flag
			else
				Console_Send_Str(",$");//data separator + hex flag
			Console_Send_Nb_To_Hex_Ascii(*(atciCmdData->params[i].val8), 1);
		}
		else if(atciCmdData->params[i].size == PARAM_INT16)
		{
			if(i==0)
				Console_Send_Str(":$"); //command code / data separator + hex flag
			else
				Console_Send_Str(",$");//data separator + hex flag
			Console_Send_Nb_To_Hex_Ascii(*(atciCmdData->params[i].val16), 2);
		}
		else if(atciCmdData->params[i].size == PARAM_INT32)
		{
			if(i==0)
				Console_Send_Str(":$"); //command code / data separator + hex flag
			else
				Console_Send_Str(",$");//data separator + hex flag
			Console_Send_Nb_To_Hex_Ascii(*(atciCmdData->params[i].val32), 4);
		}
		else if(IS_PARAM_STR(atciCmdData->params[i].size))
		{
			if(i==0)
				Console_Send_Str(":\""); //command code / data separator + string flag
			else
				Console_Send_Str(",\"");//data separator + string flag
			Console_Send_Str(atciCmdData->params[i].str);
			Console_Tx_Byte('\"');//string flag
		}
		else
		{
			if(i==0)
				Console_Send_Str(":$"); //command code / data separator + hex flag
			else
				Console_Send_Str(",$");//data separator + hex flag
			Console_Send_Array_To_Hex_Ascii(atciCmdData->params[i].data, atciCmdData->params[i].size);
		}
	}

	Console_Send_Str("\r\n"); //new line (end)
}

/*=========================================================================================================
 * FUNCTIONS - AT debug messages
 *=======================================================================================================*/

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Send debug data: command/response parameters (with Atci_Resp_Data format)
 *
 * @param[IN]	cmdCodeStr: command code as string
 * @param[IN]	atciCmdData ("atci_cmd_t" structure):
 * 					nbParams: number of parameters in command response
 * 					params: parameters list (with size and data)
 *
 * @return		None
 *-------------------------------------------------------------------------------------------------------*/
void _Atci_Debug_Param_Data(char *dbgMsd, atci_cmd_t *atciCmdData)
{
	uint8_t i;

	Console_Send_Str("\r\n+DBG: "); //new line + prefix (beginning) + debug code
	Console_Send_Str(dbgMsd); //debug message

	//each parameter data
	for(i=0; i<atciCmdData->nbParams; i++)
	{
		//parameter data as hexadecimal number / bytes array or string
		if(atciCmdData->params[i].size == PARAM_INT8)
		{
			if(i==0)
				Console_Send_Str(":$"); //command code / data separator + hex flag
			else
				Console_Send_Str(",$");//data separator + hex flag
			Console_Send_Nb_To_Hex_Ascii(*(atciCmdData->params[i].val8), 1);
		}
		else if(atciCmdData->params[i].size == PARAM_INT16)
		{
			if(i==0)
				Console_Send_Str(":$"); //command code / data separator + hex flag
			else
				Console_Send_Str(",$");//data separator + hex flag
			Console_Send_Nb_To_Hex_Ascii(*(atciCmdData->params[i].val16), 2);
		}
		else if(atciCmdData->params[i].size == PARAM_INT32)
		{
			if(i==0)
				Console_Send_Str(":$"); //command code / data separator + hex flag
			else
				Console_Send_Str(",$");//data separator + hex flag
			Console_Send_Nb_To_Hex_Ascii(*(atciCmdData->params[i].val32), 4);
		}
		else if(IS_PARAM_STR(atciCmdData->params[i].size))
		{
			if(i==0)
				Console_Send_Str(":\""); //command code / data separator + string flag
			else
				Console_Send_Str(",\"");//data separator + string flag
			Console_Send_Str(atciCmdData->params[i].str);
			Console_Tx_Byte('\"');//string flag
		}
		else
		{
			if(i==0)
				Console_Send_Str(":$"); //command code / data separator + hex flag
			else
				Console_Send_Str(",$");//data separator + hex flag
			Console_Send_Array_To_Hex_Ascii(atciCmdData->params[i].data, atciCmdData->params[i].size);
		}
	}

	Console_Send_Str("\r\n"); //new line (end)
}

/************************************************** EOF **************************************************/
