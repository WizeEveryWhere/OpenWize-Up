/**
  * @file at_key_cmd.c
  * @brief This file group some AT command.
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

#include "at_key_cmd.h"

#include "atci_get_cmd.h"
#include "atci_resp.h"

#include "crypto.h"

/******************************************************************************/

/*!
 * @brief		Execute ATKEY command (Modify the value of one key)
 *
 * @details		ATKEY command is a write only command:
 *
 * "ATKEY=<id>,<key>"
 * @parblock
 * @li id is the key number (decimal or hexadecimal 1 byte number)
 * @li key is a 16 or 32 bytes key (32 bytes KEY but only the 16 1st bytes uses) and must be written in hexadecimal format (with "$" char)
 * @endparblock
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_ATKEY_Cmd(atci_cmd_t *atciCmdData)
{
	atci_error_e status;
	uint8_t i;

	if (atciCmdData->cmdType != AT_CMD_WITH_PARAM_TO_GET)
	{
		return ATCI_ERR_PARAM_NB;
	}

	Atci_Cmd_Param_Init(atciCmdData);

	//get key number
	status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
	if (status != ATCI_ERR_NONE)
		return status;

	//check key number
	if ((*(atciCmdData->params[0].val8) < KEY_ENC_MIN) || (*(atciCmdData->params[0].val8) >= KEY_MAX_NB))
	{
		return ATCI_ERR_PARAM_VAL;
	}

	if (atciCmdData->cmdType != AT_CMD_WITH_PARAM_TO_GET)
	{
		return ATCI_ERR_PARAM_NB;
	}

	//get key value
	status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_VARIABLE_LEN);
	if (status != ATCI_ERR_NONE)
		return status;

	if (atciCmdData->cmdType != AT_CMD_WITH_PARAM)
	{
		return ATCI_ERR_PARAM_NB;
	}

	uint8_t org_sz = atciCmdData->params[1].size;
	if (atciCmdData->params[1].size == KEY_USED_BYTE_SIZE) // 16 bytes key size
	{
		// add padding to the key
		if (Atci_Update_Cmd_Param_len(atciCmdData, KEY_SIZE) != ATCI_ERR_NONE)
		{
			return ATCI_ERR_NONE;
		}
		for (i = KEY_USED_BYTE_SIZE; i < KEY_TOTAL_BYTE_SIZE; i++)
		{
			atciCmdData->params[1].data[i] = 0;
		}
	}

	if (atciCmdData->params[1].size == KEY_TOTAL_BYTE_SIZE) // 32 bytes key size
	{
		// write key:
		if (Crypto_WriteKey(atciCmdData->params[1].data, *(atciCmdData->params[0].val8)) != CRYPTO_OK)
		{
			return ATCI_ERR_UNK;
		}

		Atci_Update_Cmd_Param_len(atciCmdData, org_sz);

		/*
		 *  TODO :	// Obfuscate part of the key for DBG
		for (i = 2; i < (org_sz -2); i++)
		{
			atciCmdData->params[1].data[i] = 'x';
		}
		Atci_Update_Cmd_Param_len(atciCmdData, 2);
		atciCmdData->nbParams++;
		atciCmdData->params[2].str = &(atciCmdData->params[1].data[2]);
		Atci_Update_Cmd_Param_len(atciCmdData, ((org_sz -4) | PARAM_STR));

		atciCmdData->nbParams++;
		atciCmdData->params[3].str = &(atciCmdData->params[2].data[org_sz - 4]);
		Atci_Update_Cmd_Param_len(atciCmdData, 2);
		*/

		Atci_Debug_Param_Data("Write KEY", atciCmdData);
		return ATCI_ERR_NONE;
	}
	else
	{
		return ATCI_ERR_PARAM_LEN;
	}
}

/******************************************************************************/

#ifdef __cplusplus
}
#endif

/*! @} */
