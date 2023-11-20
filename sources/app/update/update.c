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

#define UPDATE_REQ_MSK ( SES_FLG_AVAILABLE_MSK ) // 0x000FFF00
#define UPDATE_REQ_OFFSET 8
#define UPDATE_REQ_BITS 12

#define UPDATE_REQ_START 1
#define UPDATE_REQ_STOP  2
#define UPDATE_REQ_FINALIZE 3
#define UPDATE_REQ_BLK 4

#define UPDATE_RSP_MSK 0xF

#define UPDATE_REQ(update_req) ( (update_req <<  UPDATE_REQ_OFFSET) & UPDATE_REQ_MSK )

/******************************************************************************/
/******************************************************************************/
admin_ann_fw_info_t sFwAnnInfo;
struct update_ctx_s sUpdateCtx;

void Update_Task(void const * argument);
#define UPDATE_TASK_NAME update
#define UPDATE_TASK_FCT Update_Task
#define UPDATE_STACK_SIZE 400
#define UPDATE_PRIORITY (UBaseType_t)(tskIDLE_PRIORITY+2)
SYS_TASK_CREATE_DEF(update, UPDATE_STACK_SIZE, UPDATE_PRIORITY);

// Define the timeout on trying to acquire update
#ifndef UPDATE_ACQUIRE_TIMEOUT
	#define UPDATE_ACQUIRE_TIMEOUT() pdMS_TO_TICKS(5000)
#endif
SYS_EVENT_CREATE_DEF(update);

#ifndef UPDATE_TMO_EVT
	#define UPDATE_TMO_EVT 0xFFFFFFFF // in cycle
#endif

#ifndef UPDATE_TMO_LO
	#define UPDATE_TMO_LO 300000 // in ms
#endif

void Update_LogStatus(update_status_e eUpdateStatus);
void Update_FsmInternal(uint32_t ulEvent);
void Update_FsmExternal(uint32_t ulEvent);
void Update_FsmLocal(uint32_t ulEvent);

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
void Update_Setup(void)
{
	sUpdateCtx.hTask = SYS_TASK_CREATE_CALL(update, UPDATE_TASK_FCT, NULL);
	assert(sUpdateCtx.hTask);

	sUpdateCtx.hLock = SYS_EVENT_CREATE_CALL(update);
	assert(sUpdateCtx.hLock);

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

int32_t Update_Open(admin_ann_fw_info_t sFwInfo)
{
	update_status_e eStatus;
	uint8_t eType = (uint8_t)sFwInfo.u32Type;

	if (sUpdateCtx.ePendUpdate == UPD_PEND_NONE)
	{
		if (eType < UPD_TYPE_NB)
		{
			memcpy(&sFwAnnInfo, &sFwInfo, sizeof(admin_ann_fw_info_t));
			sUpdateCtx.ePendUpdate = eType + 1;

			sys_flag_set(sUpdateCtx.hTask, UPDATE_REQ(UPDATE_REQ_START) );
			uint32_t evt = sys_evtg_wait(sUpdateCtx.hLock, UPDATE_RSP_MSK, UPDATE_ACQUIRE_TIMEOUT());
			if ( evt == UPDATE_REQ_START)
			{
				sys_evtg_clear(sUpdateCtx.hLock, UPDATE_REQ_START);
				if (sUpdateCtx.eUpdateStatus != UPD_STATUS_INPROGRESS)
				{
					LOG_ERR("UPD : Open Failed\n");
				}
				else
				{
					return 0;
				}
			}
			else
			{
				// timeout
				LOG_WRN("UPD : Open timeout\n");
			}
		}
		else
		{
			LOG_ERR("UPD : Unknown FW type\n");
		}
	}
	else
	{
		LOG_WRN("UPD : Busy\n");
	}
	return -1;
}

int32_t Update_Close(void)
{
	sys_flag_set(sUpdateCtx.hTask, UPDATE_REQ(UPDATE_REQ_STOP) );

	uint32_t evt = sys_evtg_wait(sUpdateCtx.hLock, UPDATE_RSP_MSK, UPDATE_ACQUIRE_TIMEOUT());
	if (evt == UPDATE_REQ_STOP)
	{
		sys_evtg_clear(sUpdateCtx.hLock, UPDATE_REQ_STOP);
		return 0;
	}
	// timeout
	LOG_WRN("UPD : Close timeout\n");
	return -1;
}

int32_t Update_Store(uint16_t u16Id, const uint8_t *pData)
{
	if (sUpdateCtx.ePendUpdate != UPD_PEND_LOCAL)
	{
		return -1;
	}
	UpdateArea_Proceed((uint8_t)sFwAnnInfo.u32Type, u16Id, pData);
	return 0;
}

int32_t Update_Finalize(void)
{
	/*
	if (sUpdateCtx.ePendUpdate == UPD_PEND_EXTERNAL)
	{
		return -1;
	}
	*/
	sys_flag_set(sUpdateCtx.hTask, UPDATE_REQ(UPDATE_REQ_FINALIZE) );
	uint32_t evt = sys_evtg_wait(sUpdateCtx.hLock, UPDATE_RSP_MSK, UPDATE_ACQUIRE_TIMEOUT());
	sys_evtg_clear(sUpdateCtx.hLock, UPDATE_RSP_MSK);
	if ( evt == UPDATE_REQ_FINALIZE)
	{
		return sUpdateCtx.eUpdateStatus;
	}
	// timeout
	LOG_WRN("UPD : Finalize timeout\n");
	return -1;
}

int32_t Update_IsReady(void)
{
	return ((sUpdateCtx.eUpdateStatus == UPD_STATUS_READY)?(1):(0));
}

void Update_LogStatus(update_status_e eUpdateStatus)
{
	switch (eUpdateStatus)
	{
		case UPD_STATUS_SES_FAILED :
			LOG_ERR("UPD : Session Failed\n");
			break;
		case UPD_STATUS_STORE_FAILED:
			LOG_ERR("UPD : Storage Failure\n");
			break;
		case UPD_STATUS_INCOMPLETE:
			LOG_ERR("UPD : FW Incomplete\n");
			break;
		case UPD_STATUS_CORRUPTED:
			LOG_ERR("UPD : FW Corrupted\n");
			break;
		case UPD_STATUS_INPROGRESS:
			LOG_DBG("UPD : In progress\n");
			break;
		case UPD_STATUS_VALID:
			LOG_DBG("UPD : FW Valid\n");
			break;
		case UPD_STATUS_READY:
			LOG_DBG("UPD : FW Ready\n");
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
				sUpdateCtx.ePendUpdate = UPD_PEND_FORBIDDEN;
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
			//sUpdateCtx.ePendUpdate = UPD_PEND_INTERNAL;
			sys_evtg_set(sUpdateCtx.hLock, UPDATE_REQ_START);
		}
		else if (ulUpdateReq == UPDATE_REQ_STOP)
		{
			WizeApp_Download_Cancel();
			sUpdateCtx.eUpdateStatus = UPD_STATUS_UNK;
			sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
			sys_evtg_set(sUpdateCtx.hLock, UPDATE_REQ_STOP);
		}
		else if (ulUpdateReq == UPDATE_REQ_FINALIZE)
		{
			// Nothing to do here
			sys_evtg_set(sUpdateCtx.hLock, UPDATE_REQ_FINALIZE);
		}
		else
		{
			if (sUpdateCtx.ePendUpdate == UPD_PEND_INTERNAL)
			{
				WizeApp_Common(ulEvent);

				ulEvent &= SES_FLG_DWN_MSK;
				if ( ulEvent & SES_FLG_DWN_COMPLETE)
				{
					sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
					sUpdateCtx.eUpdateStatus = UpdateArea_Finalize(
							(uint8_t)sFwAnnInfo.u32Type,
							sFwAnnInfo.u32HashSW,
							sFwAnnInfo.u16BlkCnt * 210);
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
			//_fw_buffer_init_(fw_buffer_t *pFwBuffer);

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
			sys_evtg_set(sUpdateCtx.hLock, UPDATE_REQ_START);
		}
		else if (ulUpdateReq == UPDATE_REQ_STOP)
		{
			WizeApp_Download_Cancel();
			sUpdateCtx.eUpdateStatus = UPD_STATUS_UNK;
			sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
			sys_evtg_set(sUpdateCtx.hLock, UPDATE_REQ_STOP);
		}
		else if (ulUpdateReq == UPDATE_REQ_FINALIZE)
		{
			// Nothing to do here
			sys_evtg_set(sUpdateCtx.hLock, UPDATE_REQ_FINALIZE);
		}
		else
		{
			if (sUpdateCtx.ePendUpdate == UPD_PEND_EXTERNAL)
			{
				WizeApp_Common(ulEvent);

				ulEvent &= SES_FLG_DWN_MSK;
				if ( ulEvent & SES_FLG_DWN_COMPLETE)
				{
					sUpdateCtx.eUpdateStatus = UPD_STATUS_UNK;
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
				sUpdateCtx.ePendUpdate = UPD_PEND_FORBIDDEN;
				break;
			}
			// Start the session
			sUpdateCtx.eUpdateStatus = UPD_STATUS_INPROGRESS;
			//sUpdateCtx.ePendUpdate = UPD_PEND_LOCAL;
			sys_evtg_set(sUpdateCtx.hLock, UPDATE_REQ_START);
		}
		else if (ulUpdateReq == UPDATE_REQ_STOP)
		{
			sUpdateCtx.eUpdateStatus = UPD_STATUS_UNK;
			sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
			sys_evtg_set(sUpdateCtx.hLock, UPDATE_REQ_STOP);
		}
		else if (ulUpdateReq == UPDATE_REQ_FINALIZE)
		{
			sUpdateCtx.eUpdateStatus = UpdateArea_Finalize(
					(uint8_t)sFwAnnInfo.u32Type,
					sFwAnnInfo.u32HashSW,
					sFwAnnInfo.u16BlkCnt * 210);
			sys_evtg_set(sUpdateCtx.hLock, UPDATE_REQ_FINALIZE);
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
					sUpdateCtx.eUpdateStatus = UPD_STATUS_UNK;
					sUpdateCtx.ePendUpdate = UPD_PEND_NONE;
					sys_evtg_set(sUpdateCtx.hLock, UPDATE_REQ_STOP);
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
			LOG_WRN("UPD : Session timeout\n");
		}
	}
}

/******************************************************************************/

#ifdef __cplusplus
}
#endif
