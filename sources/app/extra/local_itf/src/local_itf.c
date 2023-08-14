/**
  * @file local_itf.c
  * @brief // TODO This file ...
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
  * @par 1.0.0 : 2023/04/28 [TODO: your name]
  * Initial version
  *
  */

#include "local_itf.h"
#include "local_frm.h"

#include "bsp.h"
#include "crc_sw.h"
#include <string.h>
#include <machine/endian.h>

#include "app_layer.h"
#include "wize_app.h"
#include "stm32l4xx_hal.h"

#include "parameters_cfg.h"
#include "parameters.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

/*!
 * @cond INTERNAL
 * @{
 */

static uint8_t _get_local_err_code_(uint8_t eErrCode, uint8_t u8ErrorParam);
static uint8_t _get_adm_err_code_(uint8_t eErrCode, uint8_t *u8ErrorParam);

static void _fw_buffer_init_(fw_buffer_t *pFwBuffer);
static void* _fw_buffer_get_rptr_(fw_buffer_t *pFwBuffer);
static void* _fw_buffer_get_wptr_(fw_buffer_t *pFwBuffer);

struct local_ctx_s sLocalCtx;

/*!
 * @}
 * @endcond
 */

/******************************************************************************/
uint8_t LocalItf_OnDwnAnnToSend(local_cmd_anndownload_t *pAnn)
{

	LocalItf_FwInfoToAnn(pAnn, pFwAnnInfo);


}

void LocalItf_OnDwnAnnRet(uint8_t u8Err)
{
	uint8_t eErrParam;
	uint8_t eErrCode;

	eErrCode = _get_adm_err_code_(u8Err, &eErrParam);
	// notify WizeApp ready
	WizeApp_AnnReady(eErrCode, eErrParam);

	if (u8Err == LO_DWN_ERR_NONE)
	{
		// notify update
	}
}

uint8_t LocalItf_OnDwnAnnRecv(local_cmd_anndownload_t *pAnn, uint8_t u8KeyId)
{
	// check ann_local
	uint8_t eErr = LO_DWN_ERR_ILLEGAL_VALUE;

	admin_ann_fw_info_t sFwAnnInfo;
	uint8_t eErrCode;
	uint8_t eErrParam;

	// Check key number
	if ((u8KeyId < KEY_ENC_MIN) || (u8KeyId >= KEY_MAX_NB))
	{
		return LO_DWN_ERR_ILLEGAL_VALUE;
	}

	// Convert into FW info
	LocalItf_AnnToFwInfo(&sFwAnnInfo, pAnn);

	// Check Session Id
	if (sFwAnnInfo.u32DwnId == sLocalCtx.u32DwnId)
	{
		// Same session
		return LO_DWN_ERR_SES_ID;
	}

	// Check FW info
	eErrCode = AdmInt_AnnCheckIntFW(&sFwAnnInfo, &eErrParam);
	eErr = _get_local_err_code_(eErrCode, eErrParam);
	if (eErr)
	{
		return eErr;
	}

	// set the key id
	sLocalCtx.u8KeyId = u8KeyId;
	// set the Session Id
	sLocalCtx.u32DwnId = sFwAnnInfo.u32DwnId;

	// Stop current update (if any)


	// Start update session under limited time window
	if (Update_Open(sFwAnnInfo))
	{
		eErr = LO_DWN_ERR_UNK;
	}

	return eErr;
}


/******************************************************************************/
void LocalItf_FwInfoToAnn(local_cmd_anndownload_t *pAnn, admin_ann_fw_info_t *pFwAnnInfo)
{
	pAnn->Type                          = (uint8_t)pFwAnnInfo->u32Type;
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

void LocalItf_AnnToFwInfo(admin_ann_fw_info_t *pFwAnnInfo, local_cmd_anndownload_t *pAnn)
{
	pFwAnnInfo->u32DwnId    = __ntohl(*(uint32_t*)(pAnn->DwnId));
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

	pFwAnnInfo->u32Type = 2;
}

void LocalItf_AdmAnnToFwInfo(admin_ann_fw_info_t *pFwAnnInfo, admin_cmd_anndownload_t *pAnn)
{
	pFwAnnInfo->u32DwnId    = (pAnn->L7DwnId[0] >> 16) | (pAnn->L7DwnId[1]) | (pAnn->L7DwnId[2] << 16);
	pFwAnnInfo->u16SwVerIni = __ntohs( *(uint16_t*)(pAnn->L7SwVersionIni) );
	pFwAnnInfo->u16SwVerTgt = __ntohs( *(uint16_t*)(pAnn->L7SwVersionTarget) );
	pFwAnnInfo->u16MField   = *(uint16_t*)(pAnn->L7MField);
	pFwAnnInfo->u16DcHwId   = __ntohs( *(uint16_t*)(pAnn->L7DcHwId) );
	pFwAnnInfo->u16BlkCnt   = __ntohs( *(uint16_t*)(pAnn->L7BlocksCount) );
	//pFwAnnInfo->u32HashSW = __ntohl( *(uint32_t*)(pAnn->L7HashSW) );
	pFwAnnInfo->u32HashSW   = ( *(uint32_t*)(pAnn->L7HashSW) );
	// ---
	pFwAnnInfo->u8DayRepeat = pAnn->L7DayRepeat;
	pFwAnnInfo->u8DeltaSec  = pAnn->L7DeltaSec;
	pFwAnnInfo->u32DaysProg = __ntohl( *(uint32_t*)(pAnn->L7DaysProg) );
}

void LocalItf_AdmAnnToAnn(local_cmd_anndownload_t *pLocalAnn, admin_cmd_anndownload_t *pAnn)
{
	pLocalAnn->Type                          = (uint8_t)pFwAnnInfo->u32Type;

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

/******************************************************************************/
/*!
 * @brief This function
 *
 * @param[in] u16Id
 * @param[in] pData
 *
 */
void LocalItf_StoreBlock(uint16_t u16Id, uint8_t *pData)
{
	blk_t *pBlk;
	pBlk = (blk_t *)_fw_buffer_get_wptr_(&sLocalCtx.sFwBuffer);
	if (pBlk)
	{
		// Store blk
		memcpy(pBlk->aData, pData, BLK_SZ);
		pBlk->u16Id = u16Id;
		sLocalCtx.u8AvalableBlkCnt++;
		// notify pending frame
	}
	else
	{
		sLocalCtx.u8MissedBlkCnt++;
	}
}

/******************************************************************************/
/*!
 * @brief This function
 *
 * @param[in] u16Id
 * @param[in] pData
 *
 */
uint8_t LocalItf_OnDwnBlkToSend(local_cmd_writeblock_t *pFrame)
{
	blk_t *pBlk;
	pBlk = (blk_t *)_fw_buffer_get_rptr_(&sLocalCtx.sFwBuffer);
	if (pBlk)
	{
		*(uint32_t*)(pFrame->DwnId) = __htonl(sLocalCtx.u32DwnId);
		*(uint16_t*)(pFrame->BlockId) = __htons(pBlk->u16Id);

		sLocalCtx.u8Err = LocalFrm_Build(pFrame, pBlk->aData, sLocalCtx.u8KeyId);
		sLocalCtx.u8AvalableBlkCnt--;
		//return sLocalCtx.u8AvalableBlkCnt;
		return sLocalCtx.u8Err;
	}
	return LO_DWN_ERR_NONE;
}

/******************************************************************************/
/*!
 * @brief This function
 *
 * @param[in] u16Id
 * @param[in] pData
 *
 */
uint8_t LocalItf_OnDwnBlkRecv(local_cmd_writeblock_t *pFrame)
{
	blk_t *pBlk;

	// check dwnid
	if (sLocalCtx.u32DwnId != __ntohl( *(uint32_t*)(pFrame->DwnId)) )
	{
		sLocalCtx.u8Err = LO_DWN_ERR_SES_ID;
	}
	else
	{
		// Extract block
		pBlk = (blk_t *)(sLocalCtx.sFwBuffer.fw_blk[0]);
		sLocalCtx.u8Err = LocalFrm_Extract(pBlk->aData, pFrame, sLocalCtx.u8KeyId);
		if (sLocalCtx.u8Err == LO_DWN_ERR_NONE)
		{
			// Write the vlock
			pBlk->u16Id = __ntohs( *(uint16_t*)(pFrame->BlockId) );
			if (WizeApp_OnDwnBlkRecv(pBlk->u16Id, pBlk->aData))
			{
				sLocalCtx.u8Err = LO_DWN_ERR_BLK_ID;
			}
		}
	}
	return sLocalCtx.u8Err;
}

/******************************************************************************/
/******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */

/*!
  * @brief This function c
  *
  * @param [in]
  * @return The error code
  */
static uint8_t _get_local_err_code_(uint8_t eErrCode, uint8_t u8ErrorParam)
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

static uint8_t _get_adm_err_code_(uint8_t eErrCode, uint8_t *u8ErrorParam)
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

/*!
 * @}
 * @endcond
 */

/******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */

static
void _fw_buffer_init_(fw_buffer_t *pFwBuffer)
{
	pFwBuffer->idx_read = 0;
	pFwBuffer->idx_write = 0;
}

static
void* _fw_buffer_get_rptr_(fw_buffer_t *pFwBuffer)
{
	void *ptr = NULL;
	if(pFwBuffer->idx_read != pFwBuffer->idx_write)
	{
		ptr = pFwBuffer->fw_blk[pFwBuffer->idx_read++];
		if(pFwBuffer->idx_read > NB_BLK_STORE_MAX)
		{
			pFwBuffer->idx_read = 0;
		}
	}
	// else // EMPTY then return NULL;
	return ptr;
}

static
void* _fw_buffer_get_wptr_(fw_buffer_t *pFwBuffer)
{
	void *ptr = NULL;
	if(pFwBuffer->idx_write != pFwBuffer->idx_read)
	{
		ptr = pFwBuffer->fw_blk[pFwBuffer->idx_write++];
		if(pFwBuffer->idx_write > NB_BLK_STORE_MAX)
		{
			pFwBuffer->idx_write = 0;
		}
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
