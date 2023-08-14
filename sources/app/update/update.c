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
#include "update_area.h"

#include "rtos_macro.h"

/******************************************************************************/
/******************************************************************************/
admin_ann_fw_info_t sFwAnnInfo;
struct update_ctx_s sUpdateCtx;

extern void Update_Task(void const * argument);
#define UPDATE_TASK_NAME update
#define UPDATE_TASK_FCT Update_Task
#define UPDATE_STACK_SIZE 800
#define UPDATE_PRIORITY (UBaseType_t)(tskIDLE_PRIORITY+1)
SYS_TASK_CREATE_DEF(update, UPDATE_STACK_SIZE, UPDATE_PRIORITY);

// Define the timeout on trying to acquire update
#ifndef UPDATE_ACQUIRE_TIMEOUT
	#define UPDATE_ACQUIRE_TIMEOUT() 5
#endif
SYS_BINSEM_CREATE_DEF(update);

/******************************************************************************/
void Update_Setup(void)
{
	sUpdateCtx.hTask = SYS_TASK_CREATE_CALL(update, UPDATE_TASK_FCT, NULL);
	assert(sUpdateCtx.hTask);

	//sUpdateCtx.hLock = SYS_BINSEM_CREATE_CALL(update);
	//assert(sUpdateCtx.hLock);

	sUpdateCtx.eUpdateStatus = UpdateArea_Setup();
	if (sUpdateCtx.eUpdateStatus != UPD_STATUS_READY)
	{
		sUpdateCtx.ePendUpdate = UPD_PEND_FORBIDDEN;
		LOG_ERR("UPD : Area Setup\n");
	}
	else
	{
		sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
		sUpdateCtx.eUpdateStatus = UPD_STATUS_UNK;
		//sys_binsen_release(sUpdateCtx.hLock);
	}
	sUpdateCtx.u32Tmo = UPDATE_TMO_EVT;
}

int32_t Update_Open(admin_ann_fw_info_t sFwAnnInfo)
{
	update_status_e eStatus;
	uint8_t eType = (uint8_t)sFwAnnInfo.u32Type;
/*
	if (sUpdateCtx.ePendUpdate == UPD_PEND_NONE)
	{
		memcpy(&sUpdateCtx.sFwAnnInfo, &sFwAnnInfo, sizeof(admin_ann_fw_info_t));
		sys_flag_set(sUpdateCtx.hTask, UPDATE_REQ(UPDATE_REQ_START) );
		if ( sys_binsen_acquire(sUpdateCtx.hLock, UPDATE_ACQUIRE_TIMEOUT()) )
		{
			if (sUpdateCtx.eUpdateStatus != UPD_STATUS_INPROGRESS)
			{
				LOG_ERR("UPD : Open Failed\n");
				return -1;
			}
			return 0;
		}
		else
		{
			// timeout
			LOG_WRN("UPD : Open timeout\n");
		}
	}
	else
	{
		LOG_WRN("UPD : Busy\n");
	}
	*/
	return -1;
}

int32_t Update_Close(void)
{
	/*
	sys_binsen_release(sUpdateCtx.hLock);
	sys_flag_set(sUpdateCtx.hTask, UPDATE_REQ(UPDATE_REQ_STOP) );
	*/
	return 0;
}

int32_t Update_Store(uint16_t u16Id, const uint8_t *pData)
{
	if (sUpdateCtx.ePendUpdate != UPD_PEND_LOCAL)
	{
		return -1;
	}
	WizeApp_OnDwnBlkRecv(u16Id, pData);
	return 0;
}

void Update_LogStatus(update_status_e eUpdateStatus)
{
	switch (eUpdateStatus)
	{
		case UPD_STATUS_SES_FAILED :
			LOG_ERR("UPD : Session Failed");
			break;
		case UPD_STATUS_STORE_FAILED:
			LOG_ERR("UPD : Storage Failure");
			break;
		case UPD_STATUS_INCOMPLETE:
			LOG_ERR("UPD : FW Incomplete");
			break;
		case UPD_STATUS_CORRUPTED:
			LOG_ERR("UPD : FW Corrupted");
			break;
		case UPD_STATUS_INPROGRESS:
			LOG_DBG("UPD : In progress");
			break;
		case UPD_STATUS_VALID:
			LOG_DBG("UPD : FW Valid");
			break;
		case UPD_STATUS_READY:
			LOG_DBG("UPD : FW Ready");
			break;
		default:
			// UPD_STATUS_UNK
			break;
	}
}

/******************************************************************************/

void Update_FsmInternal(uint32_t ulEvent)
{
	uint32_t ulUpdateReq;
	do
	{
		ulUpdateReq = (ulEvent & UPDATE_REQ_MSK) >> UPDATE_REQ_OFFSET;
		if (ulUpdateReq == UPDATE_REQ_START)
		{
			// Init dwn storage
			sUpdateCtx.eUpdateStatus = UpdateArea_Initialize((uint8_t)sFwAnnInfo.u32Type, sFwAnnInfo.u16BlkCnt);
			if (sUpdateCtx.eUpdateStatus)
			{
				// Failed
				sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
				break;
			}
			// Start the download session
			if ( WizeApp_Download() != WIZE_API_SUCCESS )
			{
				// failed
				sUpdateCtx.eUpdateStatus = UPD_STATUS_SES_FAILED;
				sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
				break;
			}

			sUpdateCtx.eUpdateStatus = UPD_STATUS_INPROGRESS;
			sUpdateCtx.ePendUpdate = UPD_PEND_INTERNAL;
		}
		else if (ulUpdateReq == UPDATE_REQ_STOP)
		{
			WizeApp_Download_Cancel();
			sUpdateCtx.eUpdateStatus = UPD_STATUS_UNK;
			sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
		}
		else if (ulUpdateReq == UPDATE_REQ_FINALIZE)
		{
			// Nothing to do here
		}
		else
		{
			if (sUpdateCtx.ePendUpdate == UPD_PEND_INTERNAL)
			{
				WizeApp_Common(ulEvent);
				sUpdateCtx.eUpdateStatus = UpdateArea_Finalize(sFwAnnInfo.u32HashSW, sFwAnnInfo.u16BlkCnt * 210);

				ulEvent &= SES_FLG_DWN_MSK;
				if ( ulEvent & SES_FLG_DWN_COMPLETE)
				{
					sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
					// no error
					if ( !(ulEvent & SES_FLG_DWN_ERROR) )
					{
						/* Notify "external" that download session is completed with success */
						/* YOUR CODE HERE : ExtApi_ProcNotify(EXT_PEND_DWN_COMPLETE); */
					}
					// error
					else
					{
						/* Notify "external" that download session is completed with failure */
						/* YOUR CODE HERE : ExtApi_ProcNotify(EXT_PEND_DWN_FAILED); */
						Update_LogStatus(UPD_STATUS_SES_FAILED);
					}
				}
			}
		}
	} while(0);
}

void Update_FsmExternal(uint32_t ulEvent)
{
	uint32_t ulUpdateReq;

	do
	{
		ulUpdateReq = (ulEvent & UPDATE_REQ_MSK) >> UPDATE_REQ_OFFSET;
		if (ulUpdateReq == UPDATE_REQ_START)
		{
			// Init dwn storage

			// Start the download session
			if ( WizeApp_Download() != WIZE_API_SUCCESS )
			{
				// failed
				sUpdateCtx.eUpdateStatus = UPD_STATUS_SES_FAILED;
				sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
				break;
			}

			sUpdateCtx.eUpdateStatus = UPD_STATUS_INPROGRESS;
			sUpdateCtx.ePendUpdate = UPD_PEND_EXTERNAL;
		}
		else if (ulUpdateReq == UPDATE_REQ_STOP)
		{
			WizeApp_Download_Cancel();
			sUpdateCtx.eUpdateStatus = UPD_STATUS_UNK;
			sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
		}
		else if (ulUpdateReq == UPDATE_REQ_FINALIZE)
		{
			// Nothing to do here
		}
		else
		{
			if (sUpdateCtx.ePendUpdate == UPD_PEND_EXTERNAL)
			{
				WizeApp_Common(ulEvent);

				ulEvent &= SES_FLG_DWN_MSK;
				if ( ulEvent & SES_FLG_DWN_COMPLETE)
				{
					sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
					// no error
					if ( !(ulEvent & SES_FLG_DWN_ERROR) )
					{
						/* Notify "external" that download session is completed with success */
						/* YOUR CODE HERE : ExtApi_ProcNotify(EXT_PEND_DWN_COMPLETE); */
					}
					// error
					else
					{
						/* Notify "external" that download session is completed with failure */
						/* YOUR CODE HERE : ExtApi_ProcNotify(EXT_PEND_DWN_FAILED); */
						Update_LogStatus(UPD_STATUS_SES_FAILED);
					}
				}
			}
		}
	} while(0);
}

void Update_FsmLocal(uint32_t ulEvent)
{
	uint32_t ulUpdateReq;

	do
	{
		ulUpdateReq = (ulEvent & UPDATE_REQ_MSK) >> UPDATE_REQ_OFFSET;
		if (ulUpdateReq == UPDATE_REQ_START)
		{
			// Init dwn storage
			sUpdateCtx.eUpdateStatus = UpdateArea_Initialize((uint8_t)sFwAnnInfo.u32Type, sFwAnnInfo.u16BlkCnt);
			if (sUpdateCtx.eUpdateStatus)
			{
				// Failed
				sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
				break;
			}
			// Start the session
			sUpdateCtx.eUpdateStatus = UPD_STATUS_INPROGRESS;
			sUpdateCtx.ePendUpdate = UPD_PEND_LOCAL;

		}
		else if (ulUpdateReq == UPDATE_REQ_STOP)
		{
			sUpdateCtx.eUpdateStatus = UPD_STATUS_UNK;
			sUpdateCtx.ePendUpdate = UPD_PEND_NONE;

		}
		else if (ulUpdateReq == UPDATE_REQ_FINALIZE)
		{
			sUpdateCtx.eUpdateStatus = UpdateArea_Finalize(sFwAnnInfo.u32HashSW, sFwAnnInfo.u16BlkCnt * 210);
		}
		else if (ulUpdateReq == UPDATE_REQ_BLK)
		{
			WizeApp_OnDwnBlkRecv( sDwnCtx.sRecvMsg.u16Id, sDwnCtx.aRecvBuff );
		}
		// else { // nothing }

		if (sUpdateCtx.ePendUpdate == UPD_PEND_LOCAL)
		{
			sUpdateCtx.u32Tmo = pdMS_TO_TICKS(UPDATE_TMO_LO);
		}
		else
		{
			sUpdateCtx.u32Tmo = UPDATE_TMO_EVT;
		}
	} while (0);
}

void Update_Task(void const * argument)
{
	(void)argument;
	uint32_t ulEvent;

	while(1)
	{
		if ( sys_flag_wait(&ulEvent, sUpdateCtx.u32Tmo) )
		{
			switch(sUpdateCtx.ePendUpdate)
			{
				case UPD_PEND_INTERNAL:
					Update_FsmInternal(ulEvent);
					Update_LogStatus(sUpdateCtx.eUpdateStatus);
					break;
				case UPD_PEND_EXTERNAL:
					Update_FsmExternal(ulEvent);
					Update_LogStatus(sUpdateCtx.eUpdateStatus);
					break;
				case UPD_PEND_LOCAL:
					Update_FsmLocal(ulEvent);
					Update_LogStatus(sUpdateCtx.eUpdateStatus);
					break;
				case UPD_PEND_FORBIDDEN:
					LOG_ERR("UPD : Area Failure\n");
					break;
				default:
					break;
			}
		}
		else
		{
			// Timeout
			if (sUpdateCtx.ePendUpdate != UPD_PEND_FORBIDDEN)
			{
				sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
			}
			sUpdateCtx.u32Tmo = UPDATE_TMO_EVT;
		}
	}
}



/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/*
void Update_Task(void const * argument)
{
	(void)argument;

	uint32_t ulEvent;
	uint32_t ulUpdateReq;

	// setup
	Update_Setup();


	while(1)
	{
		if ( sys_flag_wait(&ulEvent, sUpdateCtx.u32Tmo) )
		{
			ulUpdateReq = (ulEvent & UPDATE_REQ_MSK) >> UPDATE_REQ_OFFSET;
			if (sUpdateCtx.ePendUpdate == UPD_PEND_FORBIDDEN)
			{
				LOG_ERR("UPD : Area Failure\n");
			}
			else
			{
				if (ulUpdateReq == UPDATE_REQ_START)
				{

				}
				else if (ulUpdateReq == UPDATE_REQ_STOP)
				{

				}
				else if (ulUpdateReq == UPDATE_REQ_FINALIZE)
				{
					switch (sUpdateCtx.ePendUpdate)
					{
						case UPD_PEND_LOCAL:
							//uint32_t img_sz = sFwAnnInfo.u16BlkCnt * 210; // FIXME
							sUpdateCtx.eUpdateStatus = UpdateArea_Finalize(sFwAnnInfo.u32HashSW, sFwAnnInfo.u16BlkCnt * 210);

						case UPD_PEND_INTERNAL:
						case UPD_PEND_EXTERNAL:
							sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
						default:
							break;
					}
					sUpdateCtx.u32Tmo = UPDATE_TMO_EVT;
				}
				else
				{
					switch (sUpdateCtx.ePendUpdate)
					{
						case UPD_PEND_INTERNAL:
						case UPD_PEND_EXTERNAL:
						{
							WizeApp_Common(ulEvent);
							break;
						}
						default:
							break;
					}
				}

				switch (sUpdateCtx.eUpdateStatus)
				{
					case UPD_STATUS_CORRUPTED:
						LOG_ERR("UPD : FW Corrupted");
						break;
					case UPD_STATUS_STORE_FAILED:
						LOG_ERR("UPD : Storage Failure");
						break;
					case UPD_STATUS_READY:
						LOG_DBG("UPD : Ready");
						break;
					default:
						break;
				}

			}

			if ( !(sUpdateCtx.eErrCode) )
			{
				ulUpdateReq = (ulEvent & UPDATE_REQ_MSK) >> UPDATE_REQ_OFFSET;
				if (ulUpdateReq == UPDATE_REQ_START)
				{
					uint8_t bIntStorage = 0;
					uint8_t bExtStorage = 0;
					uint8_t bStartSes = 0;
					switch (sUpdateCtx.ePendUpdate)
					{
						case UPD_PEND_LOCAL:
							sUpdateCtx.u32Tmo = pdMS_TO_TICKS(UPDATE_TMO_LO);
							bIntStorage = 1;
							break;
						case UPD_PEND_INTERNAL:
							bIntStorage = 1;
							bStartSes = 1;
							break;
						case UPD_PEND_EXTERNAL:
							bExtStorage = 1;
							bStartSes = 1;
							break;
						default:
							break;
					}
					sUpdateCtx.eUpdateStatus = _update_dwn_start_(bIntStorage, bExtStorage, bStartSes);
					if (sUpdateCtx.eUpdateStatus != UPD_STATUS_INPROGRESS)
					{
						// failed to start session or initialize the storage
					}
				}
				else if (ulUpdateReq == UPDATE_REQ_STOP)
				{
					switch (sUpdateCtx.ePendUpdate)
					{
						case UPD_PEND_INTERNAL:
						case UPD_PEND_EXTERNAL:
							WizeApp_Download_Cancel();
						case UPD_PEND_LOCAL:
							sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
							break;
						default:
							break;
					}
					sUpdateCtx.u32Tmo = UPDATE_TMO_EVT;
				}
				else if (ulUpdateReq == UPDATE_REQ_FINALIZE)
				{
					switch (sUpdateCtx.ePendUpdate)
					{
						case UPD_PEND_LOCAL:
							//uint32_t img_sz = sFwAnnInfo.u16BlkCnt * 210; // FIXME
							sUpdateCtx.eUpdateStatus = UpdateArea_Finalize(sFwAnnInfo.u32HashSW, sFwAnnInfo.u16BlkCnt * 210);
						case UPD_PEND_INTERNAL:
						case UPD_PEND_EXTERNAL:
							sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
						default:
							break;
					}
					sUpdateCtx.u32Tmo = UPDATE_TMO_EVT;
				}
				else
				{
					switch (sUpdateCtx.ePendUpdate)
					{
						case UPD_PEND_INTERNAL:
						case UPD_PEND_EXTERNAL:
						{
							WizeApp_Common(ulEvent);
							break;
						}
						default:
							break;
					}

					switch (sUpdateCtx.ePendUpdate)
					{
						case UPD_PEND_INTERNAL:
							//uint32_t img_sz = sFwAnnInfo.u16BlkCnt * 210; // FIXME
							sUpdateCtx.eUpdateStatus = UpdateArea_Finalize(sFwAnnInfo.u32HashSW, sFwAnnInfo.u16BlkCnt * 210);
						case UPD_PEND_EXTERNAL:
						{
							ulEvent &= SES_FLG_DWN_MSK;
							if ( ulEvent & SES_FLG_DWN_COMPLETE)
							{
								sUpdateCtx.ePendUpdate = UPD_PEND_NONE;

								// no error
								if ( !(ulEvent & SES_FLG_DWN_ERROR) )
								{
									// Notify "external" that download session is completed with success
									// YOUR CODE HERE : ExtApi_ProcNotify(EXT_PEND_DWN_COMPLETE);
								}
								// error
								else
								{
									// Notify "external" that download session is completed with failure
									// YOUR CODE HERE : ExtApi_ProcNotify(EXT_PEND_DWN_FAILED);
								}

							}
							break;
						}
						default:
							break;
					}

				}

				sUpdateCtx.eUpdateStatus = UPD_STATUS_UNK;
				sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
			}
		}
		else
		{
			// Timeout
			if (sUpdateCtx.ePendUpdate == UPD_PEND_LOCAL)
			{
				sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
				sUpdateCtx.u32Tmo = UPDATE_TMO_EVT;
			}
		}
	}
}

*/
/******************************************************************************/
// Convenient Local functions
/******************************************************************************/
/*
static update_status_e _update_dwn_process_(uint32_t u32Evt)
{
	update_status_e status;

	WizeApp_Common(u32Evt);
	u32Evt &= SES_FLG_DWN_MSK;

	switch(sUpdateCtx.ePendUpdate)
	{
		case UPD_PEND_INTERNAL:
		case UPD_PEND_LOCAL:
		{
			status = Update_CheckImg(sFwAnnInfo.u32HashSW);
			if (status == UPD_STATUS_VALID)
			{
				// image is valid : Finalize with Header
				//uint32_t img_sz = sFwAnnInfo.u16BlkCnt * BLOCK_SZ; // FIXME
				sUpdateCtx.eUpdateStatus = UpdateArea_WriteHeader(sFwAnnInfo.u16BlkCnt * 210);
				sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
			}
			else if (status == UPD_STATUS_CORRUPTED)
			{
				// image is corrupted
				sUpdateCtx.eUpdateStatus = status;
				sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
			}
			//else if (status == UPD_STATUS_INPROGRESS)
			//    image is not (yet) complete
			// else { // nothing }
			break;
		}
		case UPD_PEND_EXTERNAL:
			break;
		default: // UPD_PEND_NONE || UPD_PEND_FORBIDDEN
			if (u32Evt)
			{
				WizeApp_Download_Cancel();
			}
			break;
	}

	if ( u32Evt & SES_FLG_DWN_COMPLETE)
	{
		sUpdateCtx.eUpdateStatus = status;
		sUpdateCtx.ePendUpdate = UPD_PEND_NONE;

		// no error
		if ( !(u32Evt & SES_FLG_DWN_ERROR) )
		{
			// Notify "external" that download session is completed with success
			// YOUR CODE HERE : ExtApi_ProcNotify(EXT_PEND_DWN_COMPLETE);
		}
		// error
		else
		{
			// Notify "external" that download session is completed with failure
			// YOUR CODE HERE : ExtApi_ProcNotify(EXT_PEND_DWN_FAILED);
		}

	}
	// else // nothing, just wait the session termination
	return status;
}
*/
/*
static update_status_e _update_dwn_start_(uint8_t bIntStorage, uint8_t bExtStorage, uint8_t bStartSes)
{
	if (bIntStorage)
	{
		// Init dwn storage
		if ( ImgStore_Init(sFwAnnInfo.u16BlkCnt) )
		{
			// Failed
			return UPD_STATUS_STORE_FAILED;
		}
	}

	if (bExtStorage)
	{
		// Initialize the area to store the external FW block(s)
		// YOUR CODE HERE : ExtApi_fw_buffer_init();
	}

	if (bStartSes)
	{
		// Start the download session
		if ( WizeApp_Download() != WIZE_API_SUCCESS )
		{
			// failed
			return UPD_STATUS_SES_FAILED;
		}
	}

	return UPD_STATUS_INPROGRESS;
}
*/


/******************************************************************************/
// WizeCore App hooks functions
/******************************************************************************/
static void _adm_ann_to_fw_info_(admin_ann_fw_info_t *pFwAnnInfo, admin_cmd_anndownload_t *pAnn);


static void _adm_ann_to_fw_info_(admin_ann_fw_info_t *pFwAnnInfo, admin_cmd_anndownload_t *pAnn)
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

#if 0

uint8_t WizeApp_AnnCheckFwInfo(admin_cmd_anndownload_t *pAnn)
{
	admin_ann_fw_info_t sFwAnnInfo;
	uint8_t eErrCode;
	uint8_t eErrParam;

	//sFwAnnInfo.u32PreDwnId = sFwAnnInfo.u32DwnId;
	_adm_ann_to_fw_info_(&sFwAnnInfo, pAnn);

	if ( !(sFwAnnInfo.u8DayRepeat & 0x80) )
	{
		sFwAnnInfo.u32Type = 0;
		eErrCode = AdmInt_AnnCheckIntFW(&sFwAnnInfo, &eErrParam);
	}
	else
	{
		sFwAnnInfo.u32Type = 1;
		eErrCode = ExternApp_AnnCheckFWInfo(&sFwAnnInfo, &eErrParam);
	}

	// Build and send a local message


	// Notify RSP is ready
	WizeApp_AnnReady(eErrCode, eErrParam);

	return 0;
}
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
	if ( (sUpdateCtx.ePendUpdate) && (sUpdateCtx.ePendUpdate != UPD_PEND_FORBIDDEN))
	{
		if (sUpdateCtx.pUpdateItf->pfOnBlkRecv)
		{
			sUpdateCtx.pUpdateItf->pfOnBlkRecv(u16Id - 1, (uint8_t *)pData);
			return 0;
		}
	}
	return -1;
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

#endif
/******************************************************************************/

#ifdef __cplusplus
}
#endif
