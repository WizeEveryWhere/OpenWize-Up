/**
  * @file update.c
  * @brief // TODO This file ...
  * 
  * @details
  *
  * @copyright 2022, GRDF, Inc.  All rights reserved.
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
  * @par 1.0.0 : 2022/12/12 [GBI]
  * Initial version
  *
  */

#ifdef __cplusplus
extern "C" {
#endif

#include "update.h"

#include "wize_app.h"
#include "img_storage.h"

#include "rtos_macro.h"
#include "bsp.h"

#ifndef BUILD_STANDALAONE_APP
	#include "img.h"
#else
	#define MAGIC_WORD_0 0xDEADC0DEUL // Dead Code
	#define MAGIC_WORD_8 0x0DAC0DACUL // o dec o dac
#endif
/******************************************************************************/

uint8_t WizeApp_AnnCheckFwInfo(admin_cmd_anndownload_t *pAnn);
uint8_t WizeApp_OnDwnBlkRecv(uint16_t u16Id, const uint8_t *pData);
int32_t AdmInt_AnnIsLocalUpdate(void);

/******************************************************************************/
wize_api_ret_e WizeApp_Download(void);
void WizeApp_Download_Cancel(void);

static update_status_e _update_dwn_start_(void);
static void _update_dwn_process_(uint32_t u32Evt);
static update_status_e _update_write_header_(void);

admin_ann_fw_info_t sFwAnnInfo;
struct update_ctx_s sUpdateCtx;
struct update_area_s sUpdateArea;

extern uint32_t __header_size__;

#ifndef BUILD_STANDALAONE_APP
	const struct __exch_info_s *p = (const struct __exch_info_s *)&(__exchange_area_org__);
#endif
/******************************************************************************/
void Update_Task(void const * argument)
{
	(void)argument;

	uint32_t ulEvent;
	uint32_t ulUpdateReq;

#ifndef BUILD_STANDALAONE_APP
	uint32_t crc;
	crc = p->crc + 1;
#warning "*** Ensure that CRC computation is enable in the bootstrap ***"
#ifdef HAS_CRC_COMPUTE
	// check the CRC
	crc_init();
	crc = crc_compute( (uint32_t*)p, (uint32_t)(sizeof(struct __exch_info_s) - 4));
	crc_deinit();
#endif
	if ( p->crc == crc )
	{
		sUpdateArea.u32HeaderSz     = p->header_sz;
		sUpdateArea.u32MagicHeader  = p->magic;
		sUpdateArea.u32ImgAdd       = p->dest + p->header_sz;
		sUpdateArea.u32ImgMaxSz     = p->dest_sz - p->header_sz;
	}
	else
#endif
	{
		sUpdateArea.u32HeaderSz     = (uint32_t)&(__header_size__);
		sUpdateArea.u32MagicHeader  = MAGIC_WORD_8;
		sUpdateArea.u32ImgAdd       = 0x0802C200 + (uint32_t)&(__header_size__);
		sUpdateArea.u32ImgMaxSz     = 0x2A000 - (uint32_t)&(__header_size__);
	}
	sUpdateArea.u32MagicTrailer = MAGIC_WORD_0;

	sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
	sUpdateCtx.eUpdateStatus = UPD_STATUS_UNK;
	/*
	FIXME : #define IMG_MAX_SZ FLASH_IMG_SIZE
	*/
	if( ImgStore_Setup(
			sUpdateArea.u32ImgAdd,
			sUpdateArea.u32ImgMaxSz,
			BSP_Flash_Write,
			BSP_Flash_EraseArea) )
	{
		// if Image Storage initialization failed then UPSATE is not possible
		sUpdateCtx.ePendUpdate = UPD_PEND_FORBIDDEN;
		sUpdateCtx.eUpdateStatus = UPD_STATUS_STORE_FAILED;
	}

	while(1)
	{
		if ( sys_flag_wait(&ulEvent, UPDATE_TMO_EVT) )
		{
			ulUpdateReq = (ulEvent & UPDATE_REQ_MSK) >> UPDATE_REQ_OFFSET;
			switch (sUpdateCtx.ePendUpdate)
			{
				case UPD_PEND_INTERNAL:
				case UPD_PEND_EXTERNAL:
					if (ulUpdateReq == UPDATE_REQ_START)
					{
						sUpdateCtx.eUpdateStatus = _update_dwn_start_();
			#ifdef HAS_EXTERNAL_FW_UPDATE
						// If failed notify external
						if (sUpdateCtx.ePendUpdate == UPD_PEND_EXTERNAL)
						{
							if(sUpdateCtx.eUpdateStatus == UPD_STATUS_SES_FAILED)
							{
								/* Notify "external" that failed to start download session */
								/* YOUR CODE HERE : ExtApi_ProcNotify(EXT_PEND_DWN_FAILED); */
							}
						}
			#endif
						if (sUpdateCtx.eUpdateStatus != UPD_STATUS_INPROGRESS)
						{
							sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
						}
					}
					else if (ulUpdateReq == UPDATE_REQ_STOP)
					{
						WizeApp_Download_Cancel();
					}
					else
					{
						_update_dwn_process_(ulEvent);
					}
					break;
				case UPD_PEND_LOCAL:
					break;
				case UPD_PEND_NONE:
					break;
				default:
					break;
			}
		}
		else
		{
			// Timeout
		}
	}
}

/******************************************************************************/
// Convenient Local functions
/******************************************************************************/
inline
void WizeApp_Download_Cancel(void)
{
	// Start Download session
	WizeApi_Download_Cancel();
}

inline
wize_api_ret_e WizeApp_Download(void)
{
	// Start Download session
	return WizeApi_Download();
}

//inline
static update_status_e _update_write_header_(void)
{
	// Finalize with Header
	uint32_t temp[2];
	//uint32_t img_sz = sFwAnnInfo.u16BlkCnt * BLOCK_SZ;
	uint32_t img_sz = sFwAnnInfo.u16BlkCnt * 210; // FIXME
	// magic_dead
	temp[0] = sUpdateArea.u32MagicTrailer;
	temp[1] = 0xFFFFFFFF;
	uint32_t u32TgtAdd = sUpdateArea.u32ImgAdd + img_sz;

	img_sz += (u32TgtAdd % 8);
	u32TgtAdd += (u32TgtAdd % 8);

	if ( BSP_Flash_Write(u32TgtAdd, (uint64_t*)temp, 1) != DEV_SUCCESS )
	{
		return UPD_STATUS_STORE_FAILED;
	}
	// magic_header , img_sz
	temp[0] = sUpdateArea.u32MagicHeader;
	temp[1] = img_sz;

	// FIXME : HEADER SIZE
	if ( BSP_Flash_Write(
		(sUpdateArea.u32ImgAdd - sUpdateArea.u32HeaderSz), (uint64_t*)temp, 1)
			!= DEV_SUCCESS)
	{
		return UPD_STATUS_STORE_FAILED;
	}
	return UPD_STATUS_READY;
}

static void _update_dwn_process_(uint32_t u32Evt)
{
	WizeApp_Common(u32Evt);

	u32Evt &= SES_FLG_DWN_MSK;
	if ( sUpdateCtx.ePendUpdate == UPD_PEND_INTERNAL)
	{
		if ( ImgStore_IsComplete() )
		{
			if ( ImgStore_Verify((uint8_t*)&(sFwAnnInfo.u32HashSW), 4) )
			{
				// image is corrupted
				sUpdateCtx.eUpdateStatus = UPD_STATUS_CORRUPTED;
			}
			else
			{
				// image is valid
				sUpdateCtx.eUpdateStatus = UPD_STATUS_VALID;
				// Finalize with Header
				sUpdateCtx.eUpdateStatus = _update_write_header_();
			}
			sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
			WizeApp_Download_Cancel();
		}
		else
		{
			// Download failed
			if ( u32Evt & SES_FLG_DWN_COMPLETE)
			{
				sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
				// image is not complete
				sUpdateCtx.eUpdateStatus = UPD_STATUS_INCOMPLETE;
			}
		}
	}
#ifdef HAS_EXTERNAL_FW_UPDATE
	else if (sUpdateCtx.ePendUpdate == UPD_PEND_EXTERNAL)
	{
		if ( u32Evt & SES_FLG_DWN_COMPLETE)
		{
			// no error
			if ( !(u32Evt & SES_FLG_DWN_ERROR) )
			{
				/* Notify "external" that download session is completed with success */
				/* YOUR CODE HERE : ExtApi_ProcNotify(EXT_PEND_DWN_COMPLETE); */
			}
			// error
			else
			{
				/* Notify "external" that download session is completed with failure */
				/* YOUR CODE HERE : ExtApi_ProcNotify(EXT_PEND_DWN_FAILED); */
			}
		}
	}
#endif
}

static update_status_e _update_dwn_start_(void)
{
	/*
	* ADM_RSP was potentially "negative", so check that sUpdateCtx.eErrCode is 0
	*/
	if ( !(sUpdateCtx.eErrCode) )
	{
		// Init dwn storage
		if ( sUpdateCtx.ePendUpdate == UPD_PEND_INTERNAL)
		{
			if ( ImgStore_Init(sFwAnnInfo.u16BlkCnt) )
			{
				// Failed
				return UPD_STATUS_STORE_FAILED;
			}
		}
#ifdef HAS_EXTERNAL_FW_UPDATE
		else if (sUpdateCtx.ePendUpdate == UPD_PEND_EXTERNAL)
		{
			/* Initialize the area to store the external FW block(s) */
			/* YOUR CODE HERE : ExtApi_fw_buffer_init(); */
		}
#endif
		// Start the download session
		if ( WizeApp_Download() != WIZE_API_SUCCESS )
		{
			// failed
			return UPD_STATUS_SES_FAILED;
		}
		else
		{
			return UPD_STATUS_INPROGRESS;
		}
	}
	return UPD_STATUS_UNK;
}

/******************************************************************************/
// WizeCore App hooks functions
/******************************************************************************/

uint8_t WizeApp_AnnCheckFwInfo(admin_cmd_anndownload_t *pAnn)
{
	uint32_t u32PrevDwnId = sFwAnnInfo.u32DwnId;
	sFwAnnInfo.u32DwnId = (pAnn->L7DwnId[0] >> 16) | (pAnn->L7DwnId[1]) | (pAnn->L7DwnId[2] << 16);
	sFwAnnInfo.u16SwVerIni = __ntohs( *(uint16_t*)(pAnn->L7SwVersionIni) );
	sFwAnnInfo.u16SwVerTgt = __ntohs( *(uint16_t*)(pAnn->L7SwVersionTarget) );
	sFwAnnInfo.u16DcHwId = __ntohs( *(uint16_t*)(pAnn->L7DcHwId) );
	sFwAnnInfo.u16BlkCnt = __ntohs( *(uint16_t*)(pAnn->L7BlocksCount) );
	sFwAnnInfo.u8DayRepeat = pAnn->L7DayRepeat;
	sFwAnnInfo.u8DeltaSec = pAnn->L7DeltaSec;
	sFwAnnInfo.u32DaysProg = __ntohl( *(uint32_t*)(pAnn->L7DaysProg) );
	//sFwAnnInfo.u32HashSW = __ntohl( *(uint32_t*)(pAnn->L7HashSW) );
	sFwAnnInfo.u32HashSW = ( *(uint32_t*)(pAnn->L7HashSW) );

	/*
	 *  TODO : request to reprogram the same download "pAnn->L7DwnId"
	 *  - Internal FW : don't erase the the image storage area
	 *  - External FW : ??
	 */
	if (sFwAnnInfo.u32DwnId == u32PrevDwnId)
	{
		// request to restart the same download session
		if (sUpdateCtx.eUpdateStatus == UPD_STATUS_INCOMPLETE)
		{
			// image is not complete, so session is permitted
		}
		else if (sUpdateCtx.eUpdateStatus == UPD_STATUS_CORRUPTED)
		{
			// image is corrupted, so erase the image storage area
		}
	}

#ifdef HAS_EXTERNAL_FW_UPDATE
	// Check if it's external or internal FW
	if ( !(sFwAnnInfo.u8DayRepeat & 0x80) )
#endif
	{
		// Internal FW
		sUpdateCtx.ePendUpdate = UPD_PEND_INTERNAL;
		sUpdateCtx.eErrCode = AdmInt_AnnCheckIntFW(&sFwAnnInfo, &sUpdateCtx.eErrParam);
		WizeApp_AnnReady(sUpdateCtx.eErrCode, sUpdateCtx.eErrParam);

		if(sUpdateCtx.hTask)
		{
			sys_flag_set(sUpdateCtx.hTask, UPDATE_REQ(UPDATE_REQ_START) );
		}
	}
#ifdef HAS_EXTERNAL_FW_UPDATE
	else
	{
		// external FW
		sUpdateCtx.ePendUpdate = UPD_PEND_EXTERNAL;
		/* Notify "external" that an ANN_DOWNLOAD is received */
		/* YOUR CODE HERE : ExtApi_ProcNotify(EXT_PEND_ADM_ANN_RECV); */
	}
#endif
	return 0;
}

uint8_t WizeApp_OnDwnBlkRecv(uint16_t u16Id, const uint8_t *pData)
{
	/*
	 * WARNING :
	 * - Block Id from the Wize rev 1.2 protocol start from 1
	 * - "Inside" management start from 0
	 */
	if ( sUpdateCtx.ePendUpdate == UPD_PEND_INTERNAL )
	{
		ImgStore_StoreBlock(u16Id - 1, (uint8_t *)pData);
	}
#ifdef HAS_EXTERNAL_FW_UPDATE
	else if (sUpdateCtx.ePendUpdate == UPD_PEND_EXTERNAL)
	{
		/* Store the external FW block */
		/* YOUR CODE HERE : ExtApi_fw_buffer_write(u16Id, pData); */
	}
#endif
	return 0;
}

int32_t AdmInt_AnnIsLocalUpdate(void)
{
	if(sUpdateCtx.ePendUpdate == UPD_PEND_NONE)
	{
		return -1;
	}
	else
	{
		// An update already in progress
		return (sFwAnnInfo.u16SwVerTgt & 0x0000FFFF);
	}
}

/******************************************************************************/

#ifdef __cplusplus
}
#endif
