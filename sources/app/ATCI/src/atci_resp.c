/**
  * @file atci.c
  * @brief This file contains low level functions (command responses) of the AT
  * command interpreter for WizeUp module.
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
#include <string.h>

#include "atci.h"
#include "console.h"
#include "common.h"

/*==============================================================================
 * GLOBAL VARIABLES
 *============================================================================*/
/*! @internal */

/*! @endinternal */

/*==============================================================================
 * LOCAL FUNCTIONS PROTOTYPES
 *============================================================================*/
static uint8_t _bDbgEn_;
static void _atci_send_(atci_cmd_t *atciCmdData);

/*==============================================================================
 * FUNCTIONS - AT responses
 *============================================================================*/

/*!-----------------------------------------------------------------------------
 * @internal
 *
 * @brief		Enable / Disable the debug messages
 *
 * @return		None
 *
 * @endinternal
 *----------------------------------------------------------------------------*/

void Atci_Send_Dbg_Enable(uint8_t bFlag)
{
	_bDbgEn_ = bFlag;
}

/*!-----------------------------------------------------------------------------
 * @internal
 *
 * @brief		Send wakeup message
 *
 * @return		None
 *
 * @endinternal
 *----------------------------------------------------------------------------*/
void Atci_Send_Wakeup_Msg(void)
{
	Console_Send_Str("\r\n+WAKEUP\r\n");
}

/*!-----------------------------------------------------------------------------
 * @internal
 *
 * @brief		Send sleep message
 *
 * @return		None
 *
 * @endinternal
 *----------------------------------------------------------------------------*/
void Atci_Send_Sleep_Msg(void)
{
	Console_Send_Str("\r\n+SLEEP\r\n");
}


/*!-----------------------------------------------------------------------------
 * @internal
 *
 * @brief		Send AT response status (OK or error)
 *
 * @param[in]	errCode Error code : 0 for success, else error code
 *
 * @return		None
 *
 * @endinternal
 *----------------------------------------------------------------------------*/
void Atci_Resp_Ack(atci_status_t errCode)
{
	if(errCode)
		Console_Printf("\r\nERROR:%02X\r\n", errCode);
	else
		Console_Send_Str("\r\nOK\r\n");
}

/*!-----------------------------------------------------------------------------
 * @internal
 *
 * @brief		Send AT response data
 *
 * @param[in]	cmdCodeStr Command code as string
 * @param[in]	atciCmdData Pointer on "atci_cmd_t" structure:
 * 					- nbParams: number of parameters in command response
 * 					- params: parameters list (with size and data)
 *
 * @endinternal
 *----------------------------------------------------------------------------*/
void Atci_Resp_Data(char *cmdCodeStr, atci_cmd_t *atciCmdData)
{
	Console_Send_Str("\r\n+"); //new line + prefix (beginning)
	Console_Send_Str(cmdCodeStr); //command code
	_atci_send_(atciCmdData);
	Console_Send_Str("\r\n"); //new line (end)
}

/*==============================================================================
 * FUNCTIONS - AT debug messages
 *============================================================================*/

/*!-----------------------------------------------------------------------------
 * @internal
 *
 * @brief		Send debug data: command/response parameters (with Atci_Resp_Data format)
 *
 * @param[in]	dbgMsd      Pointer on debug message
 * @param[in]	atciCmdData Pointer on "atci_cmd_t" structure:
 * 					- nbParams: number of parameters in command response
 * 					- params: parameters list (with size and data)
 *
 * @endinternal
 *----------------------------------------------------------------------------*/
void _Atci_Debug_Param_Data(char *dbgMsd, atci_cmd_t *atciCmdData)
{
	if( _bDbgEn_ )
	{
		Console_Send_Str("\r\n+DBG: "); //new line + prefix (beginning) + debug code
		Console_Send_Str(dbgMsd); //command code
		if(atciCmdData)
		{
			_atci_send_(atciCmdData);
		}
		Console_Send_Str("\r\n"); //new line (end)
	}
}

/*!-----------------------------------------------------------------------------
 * @internal
 *
 * @brief		Send AT response data or Debug
 *
 * @param[in]	cmdCodeStr Command code as string
 * @param[in]	atciCmdData Pointer on "atci_cmd_t" structure:
 * 					- nbParams: number of parameters in command response
 * 					- params: parameters list (with size and data)
 *
 * @endinternal
 *----------------------------------------------------------------------------*/
static void _atci_send_(atci_cmd_t *atciCmdData)
{
	uint8_t i;

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
			Console_Send_Nb_To_Hex_Ascii( __ntohs( *(atciCmdData->params[i].val16) ), 2);
		}
		else if(atciCmdData->params[i].size == PARAM_INT32)
		{
			if(i==0)
				Console_Send_Str(":$"); //command code / data separator + hex flag
			else
				Console_Send_Str(",$");//data separator + hex flag
			Console_Send_Nb_To_Hex_Ascii( __ntohl( *(atciCmdData->params[i].val32) ), 4);
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
}
/*********************************** EOF **************************************/

/*! @} */
