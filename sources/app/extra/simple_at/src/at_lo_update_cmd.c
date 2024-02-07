/**
  * @file at_lo_update_cmd.c
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

#include "at_lo_update_cmd.h"

#include "atci_get_cmd.h"
#include "atci_resp.h"

#include "itf.h"

/******************************************************************************/
/******************************************************************************/

/*!
 * @brief		Execute ATANN command (Announce for local update)
 *
 * @details
 *
 * ATANN=$\<key_id\>,$\<session_id\>,$\<announce\>
 * @parblock
 * - \<key_id\> : The key id used to encrypt and authenticate the FW blocks (1 byte).
 *              The key id 0 (unencrypted) is forbidden.
 * - \<session_id\> : The local update session identification (4 bytes)
 * - \<announce\> : The local update announcement information
 * @endparblock
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_ATANN_Cmd(atci_cmd_t *atciCmdData)
{
	atci_error_e status;

	Atci_Cmd_Param_Init(atciCmdData);

	if (atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
	{
		// Get key id
		status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
		if (status != ATCI_ERR_NONE)
		{
			return status;
		}

		// Get session id
		status = Atci_Buf_Get_Cmd_Param(atciCmdData, DWN_ID_SZ);
		if (status != ATCI_ERR_NONE)
		{
			return status;
		}

		// Get announce
		status = Atci_Buf_Get_Cmd_Param(atciCmdData, sizeof(local_cmd_anndownload_t) - DWN_ID_SZ);
		if (status != ATCI_ERR_NONE)
		{
			return status;
		}

		status = (atci_error_e) ITF_LocalAnnRecv(
					(local_cmd_anndownload_t*)(atciCmdData->params[1].data),
					*(atciCmdData->params[0].val8)
					);

		// set ATCI mode to UPDATE
	}
	else
	{
		status = ATCI_ERR_PARAM_NB;

	}
	return status;
}

/******************************************************************************/

/*!
 * @brief		Execute ATBLK command (Set FW block)
 *
 * @details
 *
 * ATBLK=$\<session_id\>,$\<block_id\>,$\<block\>,$\<auth\>
 * @parblock
 * - \<session_id\> : A 4 bytes identifying the update session (8 digits)
 * - \<block_id\> : A 2 bytes identifying the block number (4 digits)
 * - \<block\> : A 210 bytes hexadecimal string ciphered (420 digits)
 * - \<auth\> : A 4 bytes hash cmac authenticate the block (8 digits)
 * @endparblock
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_ATBLK_Cmd(atci_cmd_t *atciCmdData)
{
	atci_error_e status = ATCI_ERR_NONE;

	Atci_Cmd_Param_Init(atciCmdData);

	if (atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
	{
		status |= Atci_Buf_Get_Cmd_Param(atciCmdData, (uint16_t)DWN_ID_SZ);
		status |= Atci_Buf_Get_Cmd_Param(atciCmdData, (uint16_t)BLK_ID_SZ);
		status |= Atci_Buf_Get_Cmd_Param(atciCmdData, (uint16_t)BLK_SZ);
		status |= Atci_Buf_Get_Cmd_Param(atciCmdData, (uint16_t)HASH_KMOB_SZ);

		if (status == ATCI_ERR_NONE)
		{
			status = ITF_LocalBlkRecv( (local_cmd_writeblock_t*)atciCmdData->params[0].data );
		}
		else
		{
			status = ATCI_ERR_UNK;
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
 * @brief		Execute ATUPD command (Request to apply local update)
 *
 * @details
 *
 * ATUPD=$\<session_id\>
 * @parblock
 * - \<session_id\> : A 4 bytes identifying the update session (8 digits)
 * @endparblock
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_ATUPD_Cmd(atci_cmd_t *atciCmdData)
{
	atci_error_e status = ATCI_ERR_PARAM_NB;

	Atci_Cmd_Param_Init(atciCmdData);

	if (atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
	{
		// Get session id
		status = Atci_Buf_Get_Cmd_Param(atciCmdData, DWN_ID_SZ);
		if (status != ATCI_ERR_NONE)
		{
			return status;
		}

		status = (atci_error_e)ITF_LocalUpdateReq((local_cmd_update_t*)atciCmdData->params[0].data);
	}
	if (status == ATCI_ERR_NONE)
	{
		atciCmdData->bNeedReboot = 1;
	}
	return status;
}

/******************************************************************************/

/*!
 * @brief		Execute ATBMAP command (Get local update block bitmap representation)
 *
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_ATBMAP_Cmd(atci_cmd_t *atciCmdData)
{
	return ATCI_ERR_NONE;
}

/******************************************************************************/

#ifdef __cplusplus
}
#endif

/*! @} */
