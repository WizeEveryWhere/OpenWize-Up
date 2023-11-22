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

#include "atci_resp.h"

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

typedef struct
{
	uint8_t code;
	const char *str;
} atci_dbg_str_t;

const atci_dbg_str_t atci_dbg_err_tab[] =
{
	{ATCI_ERR_PARAM_NB,      "Invalid number of parameters!!!"},
	{ATCI_ERR_PARAM_LEN,     "Invalid parameter length!!!"},
	{ATCI_ERR_PARAM_VAL,     "Invalid parameter value!!!"},
	{ATCI_ERR_CMD_UNK,       "Unknown command!!!"},
	{ATCI_ERR_CMD_LEN,       "Invalid command length!!!"},
	{ATCI_ERR_CMD_FORBIDDEN, "Forbidden command in the current state!!!"},
	// ---
	{ATCI_ERR_INTERNAL,      "Internal error!!!"},
	{ATCI_ERR_RX_CMD,        "Com. RX error!!!"},
	{ATCI_ERR_RX_TMO,        "Com. RX timeout!!!"},
	// ---
	{ATCI_ERR_UNK,           "Command execution error!!!"},
};

//Atci_Resp_Data("ATIDENT", atciCmdData);
//Atci_Resp_Data("ATPARAM", atciCmdData);
//Atci_Resp_Data("ATI", atciCmdData);
//Atci_Resp_Data("ATFC", atciCmdData);
/*
Atci_Debug_Str("Restore to Factory settings");
Atci_Debug_Str("Store current registers values in non volatile memory");
Atci_Debug_Str("Flash : Failed to store ");
Atci_Debug_Str("Cold Reboot");
Atci_Debug_Str("Warm Reboot");
Atci_Debug_Str("Send PING");/////////

Atci_Debug_Param_Data("Write IDENT", atciCmdData);
Atci_Debug_Param_Data("Write register", atciCmdData);
Atci_Debug_Param_Data("Compilation date", atciCmdData);
Atci_Debug_Param_Data("Write KEY", atciCmdData);

Atci_Debug_Param_Data("Set Fact Cfg. (TX PWR)", atciCmdData);/////////
Atci_Debug_Param_Data("Set Fact Cfg. (PA EN)", atciCmdData);/////////
Atci_Debug_Param_Data("Set Fact Cfg. (CAL RSSI)", atciCmdData);/////////
Atci_Debug_Param_Data("Set Fact Cfg. (CAL ADF7030)", atciCmdData);/////////
Atci_Debug_Param_Data("Set Fact Cfg. (DIS TEST MODE)", atciCmdData);/////////
Atci_Debug_Param_Data("Set Fact Cfg. (TX TEST MODE)", atciCmdData);/////////
Atci_Debug_Param_Data("Set Fact Cfg. (RX TEST MODE)", atciCmdData);/////////
Atci_Debug_Param_Data("Set Fact Cfg. (RX TEST MODE)", atciCmdData);/////////
Atci_Debug_Param_Data("Send Frame.", atciCmdData);/////////

Atci_Debug_Param_Data("Nb Pong", atciCmdData);
Atci_Debug_Param_Data("INSTPONG", atciCmdData);
*/
/*
const atci_dbg_str_t atci_dbg_sta_tab[] =
{
	{ATCI_STA_, "Cold Reboot"},
	{ATCI_STA_, "Warm Reboot"},
	{ATCI_STA_, "Restore to Factory settings"},
	{ATCI_STA_, "Store current settings in NVM"},
	{ATCI_STA_, "NVM : Failed to store "},
	{ATCI_STA_, "Send PING"},
	{ATCI_STA_, "Send Frame"},

	{ATCI_STA_, "Write IDENT"},
	{ATCI_STA_, "Write register"},
	{ATCI_STA_, "Compilation date"},
	{ATCI_STA_, "Write KEY"},


	{ATCI_STA_, "Nb Pong"},
	{ATCI_STA_, "INSTPONG"},

	{ATCI_STA_, "Set Fact Cfg."},
	{ATCI_STA_, "(RX TEST MODE)"},
	{ATCI_STA_, "(TX TEST MODE)"},
	{ATCI_STA_, "(DIS TEST MODE)"},
	{ATCI_STA_, "(CAL ADF7030)"},
	{ATCI_STA_, "(CAL RSSI)"},
	{ATCI_STA_, "(PA EN)"},
	{ATCI_STA_, "(TX PWR)"},

	{ATCI_STA_UNK,  "No message"},
};
*/

const char* Atci_Get_Dbg_Str(atci_error_e error)
{
	uint32_t i;
	for (i = 0; i < sizeof(atci_dbg_err_tab) / sizeof(atci_dbg_str_t); i++)
	{
		if (atci_dbg_err_tab[i].code == error)
		{
			break;
		}
	}

	if (i == (sizeof(atci_dbg_err_tab)/ sizeof(atci_dbg_str_t) ))
	{
		i--;
	}
	return atci_dbg_err_tab[i].str;
}

/*==============================================================================
 * FUNCTIONS - AT responses
 *============================================================================*/

const char *str_format[] =
{
	"+WAKEUP",
	"+SLEEP",
	"ERROR"":%02X",
	"OK",
	"+DBG",
	"NOTIFY",
	"",
	"",


};




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
void Atci_AckNack(atci_error_e errCode)
{
	if(errCode)
	{
		Console_Printf("\r\nERROR:%02X\r\n", errCode);
		_Atci_Debug_Param_Data(Atci_Get_Dbg_Str(errCode), NULL);
	}
	else
	{
		Console_Send_Str("\r\nOK\r\n");
	}
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

void Atci_Resp(atci_cmd_t *atciCmdData)
{
	Atci_Resp_Data(atciCmdData->pCmdDesc[atciCmdData->cmdCode].str, atciCmdData);
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
	for (i = 0; i < atciCmdData->nbParams; i++)
	{
		if (IS_PARAM_STR(atciCmdData->params[i].size))
		{
			if (i == 0)
			{
				Console_Send_Str(":\""); //command code / data separator + string flag
			}
			else
			{
				Console_Send_Str(",\"");//data separator + string flag
			}
			Console_Send_Str(atciCmdData->params[i].str);
			Console_Tx_Byte('\"');//string flag
		}
		else
		{
			if (i == 0)
			{
				Console_Send_Str(":$"); //command code / data separator + hex flag
			}
			else
			{
				Console_Send_Str(",$");//data separator + hex flag
			}
			Console_Send_Array_To_Hex_Ascii(atciCmdData->params[i].data, PARAM_INT_SIZE(atciCmdData->params[i].size) );
		}
	}
}

/*********************************** EOF **************************************/

/*! @} */
