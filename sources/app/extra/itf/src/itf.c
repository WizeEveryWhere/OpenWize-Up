/**
  * @file itf.c
  * @brief This file implement interface function's
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
  * @par 1.0.0 : 2023/04/28 [GBI]
  * Initial version
  *
  */

/*!
 *  @addtogroup itf
 *  @ingroup app
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "itf.h"

#include <string.h>
#include <machine/endian.h>

#include "at_command.h"
#include "rtos_macro.h"
#include "wize_app_itf.h"

#include "parameters_cfg.h"
#include "parameters.h"

#include "update.h"
#include "adm_internal.h"
#include "crypto.h"

/******************************************************************************/

/*!
 * @cond INTERNAL
 * @{
 */

static void _fw_buffer_init_(fw_buffer_t *pFwBuffer);
static void* _fw_buffer_get_rptr_(fw_buffer_t *pFwBuffer);
static void* _fw_buffer_get_wptr_(fw_buffer_t *pFwBuffer);

#ifndef ITF_LOCK_TMO
	#define ITF_LOCK_TMO 100 // in rtos cycles
#endif

SYS_MUTEX_CREATE_DEF(itf);

struct itf_ctx_s sItfCtx;

/*!
 * @}
 * @endcond
 */
/******************************************************************************/
/******************************************************************************/
#if 0
/*!
 * @brief This function
 *
 * @param[out] pFwAnnInfo
 * @param[in]  pAnn
 *
 */
void ITF_AdmAnnToLocalAnn(local_cmd_anndownload_t *pLocalAnn, admin_cmd_anndownload_t *pAnn)
{
	*(uint32_t*)(pLocalAnn->DwnId)           = (pAnn->L7DwnId[0] >> 16) | (pAnn->L7DwnId[1]) | (pAnn->L7DwnId[2] << 16);
	*(uint16_t*)(pLocalAnn->SwVersionIni)    = *(uint16_t*)(pAnn->L7SwVersionIni);
	*(uint16_t*)(pLocalAnn->SwVersionTarget) = *(uint16_t*)(pAnn->L7SwVersionTarget) ;
	*(uint16_t*)(pLocalAnn->MField)          = *(uint16_t*)(pAnn->L7MField);
	*(uint16_t*)(pLocalAnn->HwVersion)       = *(uint16_t*)(pAnn->L7DcHwId);
	*(uint16_t*)(pLocalAnn)->BlockCount      = *(uint16_t*)(pAnn->L7BlocksCount);
	pLocalAnn->DayRepeat                     = pAnn->L7DayRepeat;
	pLocalAnn->DeltaSec                      = pAnn->L7DeltaSec;
	*(uint32_t*)(pLocalAnn->DaysProg)        = *(uint32_t*)(pAnn->L7DaysProg);
	*(uint32_t*)(pLocalAnn->HashSW)          = *(uint32_t*)(pAnn->L7HashSW);
}
#endif

/*!
 * @brief This function setup the interface
 *
 */
void ITF_Setup(void)
{
	sItfCtx.hLock = SYS_MUTEX_CREATE_CALL(itf);
}

/*!
 * @brief This function initialize interface for a FW download to local interface .
 *
 * @retval  local_dwn_err_code_e::LO_DWN_ERR_NONE If success or there is no pending FW download.
 *          local_dwn_err_code_e::LO_DWN_ERR_UNK  Otherwise
 *
 */
uint8_t ITF_On(void)
{
	admin_ann_fw_info_t sFwAnnInfo;
	if (WizeApp_GetFwInfo(&sFwAnnInfo, NULL) != 0)
	{
		// set the Itf local key id
		Param_Access(LOCAL_KEY_ID, &(sItfCtx.u8KeyId), 0);

		// set the Itf down id
		sItfCtx.u32DwnId = sFwAnnInfo.u32DwnId;

		// init theItf FW buffer
		_fw_buffer_init_(&sItfCtx.sFwBuffer);

		// Try to start the update process
		if (Update_Open(sFwAnnInfo))
		{
			// Error
			return LO_DWN_ERR_UNK;
		}
	}
	return LO_DWN_ERR_NONE;
}

/*!
 * @brief This function store fw block into local buffer
 *
 * @param[in] u16Id The block id
 * @param[in] pData Pointer on the block's data
 *
 */
void ITF_StoreBlock(uint16_t u16Id, uint8_t *pData)
{
	blk_t *pBlk;

	if (sys_mutex_acquire(sItfCtx.hLock, ITF_LOCK_TMO) == 0)
	{
		sItfCtx.u8MissedBlkCnt++;
		return;
	}

	pBlk = (blk_t *)_fw_buffer_get_wptr_(&sItfCtx.sFwBuffer);

	if (pBlk == NULL)
	{
		sItfCtx.u8MissedBlkCnt++;
	}
	else
	{
		// Store blk
		memcpy(pBlk->aData, pData, BLK_SZ);
		pBlk->u16Id = u16Id;
		// notify pending frame
		UNS_Notify(UNS_ATBLK);
	}
	sys_mutex_release(sItfCtx.hLock);
}

/******************************************************************************/
/******************************************************************************/

/*!
 * @brief This function build an announce to be send on local interface
 *
 * @param[out] pAnn       Pointer on resulting announce
 * @param[in]  pFwAnnInfo Pointer on announce to build
 *
 */
void ITF_FwInfoToLocalAnn(local_cmd_anndownload_t *pAnn, admin_ann_fw_info_t *pFwAnnInfo)
{
	//pAnn->Type                          = (uint8_t)pFwAnnInfo->u32Type;
	*(uint32_t*)(pAnn->DwnId)           = __htonl(pFwAnnInfo->u32DwnId);
	*(uint16_t*)(pAnn->SwVersionIni)    = __htons(pFwAnnInfo->u16SwVerIni);
	*(uint16_t*)(pAnn->SwVersionTarget) = __htons(pFwAnnInfo->u16SwVerTgt);
	*(uint16_t*)(pAnn->MField)          = pFwAnnInfo->u16MField;
	*(uint16_t*)(pAnn->HwVersion)       = __htons(pFwAnnInfo->u16DcHwId);
	*(uint16_t*)(pAnn->BlockCount)      = __htons(pFwAnnInfo->u16BlkCnt);
	pAnn->DayRepeat                     = pFwAnnInfo->u8DayRepeat;
	pAnn->DeltaSec                      = pFwAnnInfo->u8DeltaSec;
	*(uint32_t*)(pAnn->DaysProg)        = __htonl(pFwAnnInfo->u32DaysProg);
	*(uint32_t*)(pAnn->HashSW)          = pFwAnnInfo->u32HashSW;
}

/*!
 * @brief This function build a local interface FW block frame from internal store.
 *
 * @param[in] pFrame Pointer frame containing the request.
 *
 * @retval  local_dwn_err_code_e::LO_DWN_ERR_NONE If the build success.
 *          local_dwn_err_code_e::LO_DWN_ERR_UNK  Otherwise
 *
 */
uint8_t ITF_LocalBlkSend(local_cmd_writeblock_t *pFrame)
{
	blk_t *pBlk;
	sItfCtx.u8Err = LO_DWN_ERR_UNK;
	if (sys_mutex_acquire(sItfCtx.hLock, ITF_LOCK_TMO))
	{
		pBlk = (blk_t *)_fw_buffer_get_rptr_(&sItfCtx.sFwBuffer);
		if (pBlk)
		{
			*(uint32_t*)(pFrame->DwnId) = __htonl(sItfCtx.u32DwnId);
			*(uint16_t*)(pFrame->BlockId) = __htons(pBlk->u16Id);
			sItfCtx.u8Err = LocalFrm_Build(pFrame, pBlk->aData, sItfCtx.u8KeyId);
		}
		sys_mutex_release(sItfCtx.hLock);
	}
	//return LO_DWN_ERR_NONE;
	return sItfCtx.u8Err;
}

/*!
 * @brief This function convert error code from local interface to admin. L7 layer ones
 *
 * @param[in]  eErrCode     Local interface error code.
 * @param[out] u8ErrorParam Parameter number on which error occurs (if any).
 *
 * @return The converted error code as describe in admin_ann_err_code_e.
 */
uint8_t ITF_GetAdmErrCode(uint8_t eErrCode, uint8_t *u8ErrorParam)
{
	uint8_t err = ADM_UNK_CMD;
	switch(eErrCode)
	{
		case LO_DWN_ERR_MFIELD:
			*u8ErrorParam = ANN_FIELD_ID_L7MField;
			err = ANN_ILLEGAL_VALUE;
			break;
		case LO_DWN_ERR_BLK_CNT:
			*u8ErrorParam = ANN_FIELD_ID_L7BlocksCount;
			err = ANN_ILLEGAL_VALUE;
			break;
		case LO_DWN_ERR_FRM_LEN:
		case LO_DWN_ERR_INI_SW_VER:
		case LO_DWN_ERR_HW_VER:
		case LO_DWN_ERR_TGT_SW_VER:
		case LO_DWN_ERR_TGT_VER_DWL:
		case LO_DWN_ERR_OUT_OF_WINDOW:
			err = eErrCode - 0x80;
			break;
		case LO_DWN_ERR_NONE:
			err = ADM_NONE;
		case LO_DWN_ERR_ILLEGAL_VALUE:
		default:
			break;
	}
	return err;
}

/******************************************************************************/
/******************************************************************************/

/*!
 * @brief This function extract an announce from local interface
 *
 * @param[out] pFwAnnInfo Pointer on resulting announce
 * @param[in]  pAnn       Pointer on announce to extract
 *
 */
void ITF_LocalAnnToFwInfo(admin_ann_fw_info_t *pFwAnnInfo, local_cmd_anndownload_t *pAnn)
{
	//pFwAnnInfo->u32Type = 2;
	//pFwAnnInfo->u32Type     = __ntohl((uint32_t)pAnn->Type);
	pFwAnnInfo->u32DwnId    = __ntohl( *(uint32_t*)(pAnn->DwnId));
	pFwAnnInfo->u16SwVerIni = __ntohs( *(uint16_t*)(pAnn->SwVersionIni) );
	pFwAnnInfo->u16SwVerTgt = __ntohs( *(uint16_t*)(pAnn->SwVersionTarget) );
	pFwAnnInfo->u16MField   = *(uint16_t*)(pAnn->MField);
	pFwAnnInfo->u16DcHwId   = __ntohs( *(uint16_t*)(pAnn->HwVersion) );
	pFwAnnInfo->u16BlkCnt   = __ntohs( *(uint16_t*)(pAnn->BlockCount) );
	//pFwAnnInfo->u32HashSW = __ntohl( *(uint32_t*)(pAnn->HashSW) );
	pFwAnnInfo->u32HashSW   = ( *(uint32_t*)(pAnn->HashSW) );
	// ---
	pFwAnnInfo->u8DayRepeat = 0;
	pFwAnnInfo->u8DeltaSec  = 0;
	pFwAnnInfo->u32DaysProg = 0;
}

/*!
 * @brief This function request to start a new FW update session from local interface.
 *
 * @param[in] pAnn    Pointer frame containing the request.
 * @param[in] u8KeyId The id of the key which will be used for FW block encryption and authentication.
 *
 * @return Error code from local_dwn_err_code_e
 *
 */
uint8_t ITF_LocalAnnRecv(local_cmd_anndownload_t *pAnn, uint8_t u8KeyId)
{
	// check ann_local
	uint8_t eErr = LO_DWN_ERR_ILLEGAL_VALUE;

	admin_ann_fw_info_t sFwAnnInfo;
	uint8_t eErrCode;
	uint8_t eErrParam;

	// Convert into FW info
	ITF_LocalAnnToFwInfo(&sFwAnnInfo, pAnn);

	// Check key number
	if ((u8KeyId < KEY_ENC_MIN) || (u8KeyId > KEY_MAX_NB))
	{
		return LO_DWN_ERR_ILLEGAL_VALUE;
	}

	// Check Session Id
	if (sFwAnnInfo.u32DwnId == sItfCtx.u32DwnId)
	{
		// Same session
		return LO_DWN_ERR_SES_ID;
	}

	// set is local
	sFwAnnInfo.u32Type = UPD_TYPE_LOCAL;

	// Check FW info
	eErrCode = AdmInt_AnnCheckIntFW(&sFwAnnInfo, &eErrParam);
	eErr = ITF_GetLocalErrCode(eErrCode, eErrParam);
	if (eErr)
	{
		return eErr;
	}

	// Stop current update (if any)
	if (Update_Close())
	{
		return LO_DWN_ERR_UNK;
	}

	// Start update session under limited time window
	if (Update_Open(sFwAnnInfo))
	{
		return LO_DWN_ERR_UNK;
	}

	// set the Session Id
	sItfCtx.u32DwnId = sFwAnnInfo.u32DwnId;
	// set the key id
	sItfCtx.u8KeyId = u8KeyId;

	return eErr;
}

/*!
 * @brief This function request to extract and store a FW block from local interface.
 *
 * @param[in] pFrame Pointer frame containing the request.
 *
 * @retval local_dwn_err_code_e::LO_DWN_ERR_NONE If success
 *         local_dwn_err_code_e::LO_DWN_ERR_SES_ID If the given session id doesn't match
 *         local_dwn_err_code_e::LO_DWN_ERR_AUTH If authentication failed
 *         local_dwn_err_code_e::LO_DWN_ERR_BLK_ID If the block write failed
 *         local_dwn_err_code_e::LO_DWN_ERR_UNK Otherwise
 *
 */
uint8_t ITF_LocalBlkRecv(local_cmd_writeblock_t *pFrame)
{
	blk_t *pBlk;

	// check dwnid
	if (sItfCtx.u32DwnId != __ntohl( *(uint32_t*)(pFrame->DwnId)) )
	{
		sItfCtx.u8Err = LO_DWN_ERR_SES_ID;
	}
	else
	{
		// Extract block
		pBlk = (blk_t *)(sItfCtx.sFwBuffer.fw_blk[0]);
		sItfCtx.u8Err = LocalFrm_Extract(pBlk->aData, pFrame, sItfCtx.u8KeyId);
		if (sItfCtx.u8Err == LO_DWN_ERR_NONE)
		{
			// Write the block
			pBlk->u16Id = __ntohs( *(uint16_t*)(pFrame->BlockId) );
			if (Update_Store(pBlk->u16Id, pBlk->aData))
			{
				sItfCtx.u8Err = LO_DWN_ERR_BLK_ID;
			}
		}
	}
	return sItfCtx.u8Err;
}

/*!
 * @brief This function request to finalize a FW update from local interface.
 *
 * @param[in] pFrame Pointer frame containing the request.
 *
 * @retval local_dwn_err_code_e::LO_DWN_ERR_NONE If success
 *         local_dwn_err_code_e::LO_DWN_ERR_SES_ID If the given session id doesn't match
 *         local_dwn_err_code_e::LO_DWN_ERR_CORRUPTED If the downloaded FW is corrupted
 *         local_dwn_err_code_e::LO_DWN_ERR_BLK_CNT If the downloaded FW is incomplete
 *         local_dwn_err_code_e::LO_DWN_ERR_WRITE If the final write failed
 *
 */
uint8_t ITF_LocalUpdateReq(local_cmd_update_t *pFrame)
{
	// Check the session id
	if (sItfCtx.u32DwnId == __ntohl( *(uint32_t*)(pFrame->DwnId)) )
	{
		// Check img
		update_status_e update_status;
		update_status = Update_Finalize();
		if (update_status == UPD_STATUS_CORRUPTED)
		{
			sItfCtx.u8Err = LO_DWN_ERR_CORRUPTED;
		}
		else if (update_status == UPD_STATUS_INCOMPLETE)
		{
			sItfCtx.u8Err = LO_DWN_ERR_BLK_CNT;
		}
		else if (update_status == UPD_STATUS_INPROGRESS)
		{
			sItfCtx.u8Err = LO_DWN_ERR_BLK_CNT;
		}
		else if (update_status == UPD_STATUS_STORE_FAILED)
		{
			sItfCtx.u8Err = LO_DWN_ERR_WRITE;
		}
		else if (update_status == UPD_STATUS_READY)
		{
			sItfCtx.u8Err = LO_DWN_ERR_NONE;
		}
		else
		{
			sItfCtx.u8Err = LO_DWN_ERR_UNK;
		}
	}
	else
	{
		sItfCtx.u8Err = LO_DWN_ERR_SES_ID;
	}
	return sItfCtx.u8Err;
}

/*!
 * @brief This function convert error code from admin. L7 layer to local ones
 *
 * @param[in] eErrCode     L7 admin. layer error code.
 * @param[in] u8ErrorParam Parameter number on which error occurs (if any).
 *
 * @return The converted error code as describe in local_dwn_err_code_e.
 */
uint8_t ITF_GetLocalErrCode(uint8_t eErrCode, uint8_t u8ErrorParam)
{
	uint8_t err = LO_DWN_ERR_NONE;
	switch(eErrCode)
	{
		case ANN_ILLEGAL_VALUE:
			if (u8ErrorParam == ANN_FIELD_ID_L7MField)
			{
				err = LO_DWN_ERR_MFIELD;
			}
			else if (u8ErrorParam == ANN_FIELD_ID_L7BlocksCount)
			{
				err = LO_DWN_ERR_BLK_CNT;
			}
			break;
		case ANN_INCORRECT_HW_VER:
		case ANN_INCORRECT_INI_SW_VER:
		case ANN_TGT_VER_DWL:
		case ANN_TGT_SW_VER:
		case ANN_DIFF_TIME_OUT_OF_WINDOWS:
			err = eErrCode + 0x80;
			break;
		case 0:
			break;
		default:
			err = LO_DWN_ERR_UNK;
			break;
	}
	return err;
}

/******************************************************************************/
/******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */

/*!
 * @static
 * @brief  This function initialize temporary buffer for FW block to/from local
 *         interface.
 *
 * @param [in,out] pFwBuffer Pointer on the buffer structure
 *
 */
static
void _fw_buffer_init_(fw_buffer_t *pFwBuffer)
{
	// set buffer to empty
	pFwBuffer->idx_read = 0;
	pFwBuffer->idx_write = 0;
	pFwBuffer->u8ReadyBlkCnt = 0;

}

/*!
 * @static
 * @brief  This function get the current read pointer on temporary FW buffer.
 *
 * @param [in,out] pFwBuffer Pointer on the buffer structure.
 *
 * @return The current read pointer. The NULL pointer is returned if buffer is empty.
 *
 */
static
void* _fw_buffer_get_rptr_(fw_buffer_t *pFwBuffer)
{
	void *ptr = NULL;
	if (pFwBuffer->u8ReadyBlkCnt > 0)
	{
		ptr = pFwBuffer->fw_blk[pFwBuffer->idx_read++];
		if(pFwBuffer->idx_read > NB_BLK_STORE_MAX)
		{
			pFwBuffer->idx_read = 0;
		}
		pFwBuffer->u8ReadyBlkCnt--;
	}
	// else // EMPTY then return NULL;
	return ptr;
}

/*!
 * @static
 * @brief  This function get the current write pointer on temporary FW buffer.
 *
 * @param [in,out] pFwBuffer Pointer on the buffer structure.
 *
 * @return The current write pointer. The NULL pointer is returned if buffer is full.
 *
 */
static
void* _fw_buffer_get_wptr_(fw_buffer_t *pFwBuffer)
{
	void *ptr = NULL;
	if (pFwBuffer->u8ReadyBlkCnt < NB_BLK_STORE_MAX)
	{
		ptr = pFwBuffer->fw_blk[pFwBuffer->idx_write++];
		if(pFwBuffer->idx_write > NB_BLK_STORE_MAX)
		{
			pFwBuffer->idx_write = 0;
		}
		pFwBuffer->u8ReadyBlkCnt++;
	}
	// else // FULL then return NULL;
	return ptr;
}

/*!
 * @}
 * @endcond
 */
/******************************************************************************/

#ifdef __cplusplus
}
#endif

/*! @} */
