/**
  * @file: at_wize_cmd.c
  * @brief: This file group some AT command.
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
  * @par 1.0.0 : 2023/07/10 [GBI]
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

#include "at_wize_cmd.h"

#include "atci_get_cmd.h"
#include "atci_resp.h"

#include "itf.h"
#include "app_entry.h"

#include "wize_app_itf.h"
#include "update.h"
#include "platform.h"

#include "at_command.h"

/******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */

/*!
 * @}
 * @endcond
 */

/******************************************************************************/

/*!
 * @brief		Execute ATSEND command (Send a Wize message)
 *
 * @details		Command format: "ATSEND=\<l6app\>,\<l7msg\>"
 * @parblock
 * @li l6app is the layer 6 application code used to give for witch application are the L7 data (decimal or hexadecimal 8bits integer)
 * @li l7msg is the layer 7 message to send (array in hexadecimal format, maximum length is 102 bytes for PRES-EXCHANGE L6 frames)
 * @endparblock
 *
 * Maybe 2 types of responses:
 * @parblock
 * @li an APP-ADMIN write command was received and processed by the on-board Wize stack message: "+ATADMWRITE :\<paramid\>,\<paramvalue\>,\<rssi\>"
 * @li a response was received in response of the Wize message, which can’t be managed by the on-board Wize stack (other application layer than APP-ADMIN) message: "+ATRCV:\<L6App\>,\<l7resp\>,\<rssi\>"
 * @endparblock
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_CMD_ATSEND(atci_cmd_t *atciCmdData)
{
	atci_error_e status;
	uint8_t i;

	if (atciCmdData->cmdType != AT_CMD_WITH_PARAM_TO_GET)
	{
		return ATCI_ERR_PARAM_NB;
	}

	Atci_Cmd_Param_Init(atciCmdData);

	// -------------------------------------------------------------------------
	// get L6-app field
	status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
	if (status != ATCI_ERR_NONE) { return status; }
	if (atciCmdData->cmdType != AT_CMD_WITH_PARAM_TO_GET)
	{
		return ATCI_ERR_PARAM_NB;
	}

	// if is currently in test mode
	if (atciCmdData->bTestMode)
	{
		return ATCI_ERR_CMD_FORBIDDEN;
	}

	// -------------------------------------------------------------------------
	//get L7 message
	status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_VARIABLE_LEN);
	if (status != ATCI_ERR_NONE) { return status; }
	if (atciCmdData->cmdType != AT_CMD_WITH_PARAM)
	{
		return ATCI_ERR_PARAM_NB;
	}
	Param_Access(L7TRANSMIT_LENGTH_MAX, &i, 0);
	if (atciCmdData->params[1].size > i)
	{
		return ATCI_ERR_PARAM_NB;
	}

	// -------------------------------------------------------------------------
	Atci_Debug_Param_Data("Send Frame.", atciCmdData);/////////

	if ( ITF_Send(atciCmdData->paramsMem, atciCmdData->params[1].size+1) )
	{
		// Failure
		return ATCI_ERR_UNK;
	}

	atciCmdData->bNeedAck = 0;

	return ATCI_ERR_NONE;
}

/*!
 * @brief		Execute ATADMANN command
 *
 * @details     Command format: "ATADMANN=\<ErrCode\>"
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_CMD_ATADMANN(atci_cmd_t *atciCmdData)
{
	atci_error_e status;

	// if is currently in test mode
	if (atciCmdData->bTestMode)
	{
		return ATCI_ERR_CMD_FORBIDDEN;
	}

	if (atciCmdData->cmdType != AT_CMD_WITH_PARAM_TO_GET)
	{
		return ATCI_ERR_PARAM_NB;
	}

	Atci_Cmd_Param_Init(atciCmdData);

	status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
	if (status != ATCI_ERR_NONE) { return status; }

	ITF_AnnReady((*(atciCmdData->params[0].data)));

	return status;
}

/******************************************************************************/
/*!
 * @brief		Execute ATADMWRITE notify
 *
 * @details
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_UNS_ATADMWRITE(atci_cmd_t *atciCmdData)
{
	uint8_t size;

	atciCmdData->cmdCode = UNS_ATADMWRITE;
	Atci_Cmd_Param_Init(atciCmdData);

	// APP-ADMIN write command reception
	// msg format: <cmd ID 1 (1 byte)><cmd val 1 (s1 bytes)>...<cmd ID n (1 byte)><cmd val n (sn bytes)>

	atciCmdData->nbParams = 3;

	// buffer for the RSSI
	atciCmdData->params[2].size = PARAM_INT8;
	atciCmdData->params[2].data = &(atciCmdData->paramsMem[AT_CMD_DATA_MAX_LEN-1]);

	// write RX message in params memory
	// write RSSI in ???
	size = WizeApp_GetAdmReq(atciCmdData->paramsMem, atciCmdData->params[2].val8);

	if (size > 1)
	{
		uint8_t i = 0;
		while (i < size)
		{
			// get param ID (1st byte of received message)
			atciCmdData->params[0].size = PARAM_INT8;
			atciCmdData->params[0].data = &(atciCmdData->paramsMem[i++]);
			// get param Value (next bytes of received message)
			atciCmdData->params[1].size = (uint16_t) Param_GetSize(*(atciCmdData->params[0].val8));
			atciCmdData->params[1].data = &(atciCmdData->paramsMem[i]);

			i += atciCmdData->params[1].size;
			// send received APP-ADMIN WRITE command
			Atci_Resp(atciCmdData);
		}
	}
	return ATCI_ERR_NONE;
}

/*!
 * @brief		Execute ATADMANN notify
 *
 * @details
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_UNS_ATADMANN(atci_cmd_t *atciCmdData)
{
	admin_ann_fw_info_t sFwAnnInfo;
	atci_error_e status = ATCI_ERR_NONE;
	//uint8_t rssi;
	//if (WizeApp_GetFwInfo(&sFwAnnInfo, &rssi) )
	if (WizeApp_GetFwInfo(&sFwAnnInfo, NULL) != 0)
	{
		uint8_t sz = sizeof(local_cmd_anndownload_t) - 4;
		atciCmdData->cmdCode = UNS_ATADMANN;
		Atci_Cmd_Param_Init(atciCmdData);

		// Add param of size 1 for type
		atciCmdData->params[0].size = 1;
		status = Atci_Add_Cmd_Param_Resp(atciCmdData);
		// Add param of size 4 for session_id
		atciCmdData->params[1].size = 4;
		status |= Atci_Add_Cmd_Param_Resp(atciCmdData);
		// Add param of size 20 for announce
		atciCmdData->params[2].size = sz;
		status |= Atci_Add_Cmd_Param_Resp(atciCmdData);

		// Add param of size 1 for RSSI
		//atciCmdData->params[3].size = 1;
		//status |= Atci_Add_Cmd_Param_Resp(atciCmdData);

		if (status == ATCI_ERR_NONE)
		{
			// set type
			*(atciCmdData->params[0].data) = (uint8_t)sFwAnnInfo.u32Type;
			//*(atciCmdData->params[3].data) = rssi;
			ITF_FwInfoToLocalAnn((local_cmd_anndownload_t*)atciCmdData->params[1].data, &sFwAnnInfo);
			Atci_Resp(atciCmdData);
		}
	}
	// else {// error, bypass }
	return status;
}

/*!
 * @brief		Execute ATADMDATA notify
 *
 * @details
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_UNS_ATADMDATA(atci_cmd_t *atciCmdData)
{
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
	return ATCI_ERR_NONE;
}

/*!
 * @brief		Execute ATBLK notify
 *
 * @details
 *
 * +ATBLK=$\<session_id\>,$\<block_id\>,$\<block\>,$\<auth\>
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
atci_error_e Exec_UNS_ATBLK(atci_cmd_t *atciCmdData)
{
	atci_error_e status;

	atciCmdData->cmdCode = UNS_ATBLK;
	Atci_Cmd_Param_Init(atciCmdData);

	// Add param of size 4 : upd_id
	atciCmdData->params[0].size = DWN_ID_SZ;
	status = Atci_Add_Cmd_Param_Resp(atciCmdData);
	// Add param of size 2 : blk_id
	atciCmdData->params[1].size = BLK_ID_SZ;
	status |= Atci_Add_Cmd_Param_Resp(atciCmdData);
	// Add param of size 210 : blk
	atciCmdData->params[2].size = BLK_SZ;
	status |= Atci_Add_Cmd_Param_Resp(atciCmdData);
	// Add param of size 2 : auth
	atciCmdData->params[3].size = HASH_KMOB_SZ;
	status |= Atci_Add_Cmd_Param_Resp(atciCmdData);

	int8_t ret = (int8_t)ITF_LocalBlkSend( (local_cmd_writeblock_t*)atciCmdData->params[0].data );
	if (ret == -1)
	{
		return ATCI_ERR_UNK;
	}

	Atci_Resp(atciCmdData);
	return ATCI_ERR_NONE;
}

/******************************************************************************/
/******************************************************************************/

/*!
 * @brief		Execute ATPING command (Send an INSTPING request)
 *
 * @details 	Command format: "ATPING"
 * 				No specific response (read dedicated registers with ATPARAM command)
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_CMD_ATPING(atci_cmd_t *atciCmdData)
{
	uint8_t nbPong;

	if (atciCmdData->cmdType != AT_CMD_WITHOUT_PARAM)
		return ATCI_ERR_PARAM_NB;

	// if is currently in test mode
	if (atciCmdData->bTestMode)
	{
		return ATCI_ERR_CMD_FORBIDDEN;
	}

	Atci_Debug_Str("Send PING");/////////

	// clear PING_NBFOUND
	nbPong = 0;
	Param_Access(PING_NBFOUND, &nbPong, 1);

	uint8_t size;
	size = WizeApp_GetInstReq(atciCmdData->paramsMem, NULL);
	if ( ITF_Ping(atciCmdData->paramsMem, size) )
	{
		// Failure
		return ATCI_ERR_UNK;
	}

	atciCmdData->bNeedAck = 0;

	return ATCI_ERR_NONE;
}

/******************************************************************************/
/*!
 * @brief		Execute ATPING notify
 *
 * @details
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_UNS_ATPING(atci_cmd_t *atciCmdData)
{
	uint8_t nbPong, i;

	atciCmdData->cmdCode = UNS_ATPING;

	Atci_Cmd_Param_Init(atciCmdData);
	atciCmdData->params[0].size = PARAM_INT8;
	Atci_Add_Cmd_Param_Resp(atciCmdData);


	Param_Access(PING_NBFOUND, &nbPong, 0);
	*(atciCmdData->params[0].val8) = nbPong;
	//Atci_Debug_Param_Data("Nb Pong", atciCmdData);
	Atci_Resp(atciCmdData);

	// -------
	//Atci_Cmd_Param_Init(atciCmdData);
	//atciCmdData->params[0].size = PARAM_INT8;
	//Atci_Add_Cmd_Param_Resp(atciCmdData);
	atciCmdData->params[1].size = 9;
	Atci_Add_Cmd_Param_Resp(atciCmdData);

	if(nbPong > 8) { nbPong = 8; }

	for(i = 0; i < nbPong; i++)
	{
		*(atciCmdData->params[0].val8) = i;
		Param_Access(PING_REPLY1+i, atciCmdData->params[1].data, 0);
		Atci_Debug_Param_Data("INSTPONG", atciCmdData);
	}
	return ATCI_ERR_NONE;
}

/******************************************************************************/
/******************************************************************************/

#ifdef __cplusplus
}
#endif

/*! @} */
