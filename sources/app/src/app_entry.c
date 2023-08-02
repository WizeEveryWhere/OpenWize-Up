/**
  * @file app_entry.c
  * @brief This implement the default entry point after board initialization
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
  * @par 1.0.0 : 2019/11/20 [GBI]
  * Initial version
  *
  *
  */

/*!
 * @addtogroup app
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "app_entry.h"
#include "atci.h"
#include "update.h"
#include "wize_app.h"

extern void Sys_Init(void);
extern void Sys_Start(void);

void App_Init(void);

/**
  * @brief  The application entry point.
  * @retval None
  */
void app_entry(void)
{
  	Sys_Init();
  	App_Init();
  	Sys_Start();
}

/******************************************************************************/

/*!
 * @cond INTERNAL
 * @{
 */

void* hLoItfTask;
extern void Atci_Task(void const * argument);
#define LOITF_TASK_NAME loitf
#define LOITF_TASK_FCT Atci_Task
#define LOITF_STACK_SIZE 800
#define LOITF_PRIORITY (UBaseType_t)(tskIDLE_PRIORITY+1)
SYS_TASK_CREATE_DEF(loitf, LOITF_STACK_SIZE, LOITF_PRIORITY);


//void* hUpdateTask;
extern void Update_Task(void const * argument);
#define UPDATE_TASK_NAME update
#define UPDATE_TASK_FCT Update_Task
#define UPDATE_STACK_SIZE 800
#define UPDATE_PRIORITY (UBaseType_t)(tskIDLE_PRIORITY+1)
SYS_TASK_CREATE_DEF(update, UPDATE_STACK_SIZE, UPDATE_PRIORITY);

void* hMonitorTask;
void Monitor_Task(void const * argument);
#define MONITOR_TASK_NAME monitor
#define MONITOR_TASK_FCT Monitor_Task
#define MONITOR_STACK_SIZE 800
#define MONITOR_PRIORITY (UBaseType_t)(tskIDLE_PRIORITY+1)
SYS_TASK_CREATE_DEF(monitor, MONITOR_STACK_SIZE, MONITOR_PRIORITY);

/*!
 * @}
 * @endcond
 */

extern struct update_ctx_s sUpdateCtx;

/**
  * @brief  Called to initialize application before starting the scheduler.
  */
void App_Init(void)
{
	uint8_t u8ExtFlags = 0b11100101;

#ifdef HAS_EXTEND_PARAMETER
	Param_Access(EXTEND_FLAGS, &u8ExtFlags, 0);
#endif
	Atci_Send_Dbg_Enable( (u8ExtFlags & EXT_FLAGS_DBG_MSG_EN_MSK) );

	sUpdateCtx.hTask = SYS_TASK_CREATE_CALL(update, UPDATE_TASK_FCT, NULL);
	//_setup_wakeup_loitf_();
	hLoItfTask = SYS_TASK_CREATE_CALL(loitf, LOITF_TASK_FCT, NULL);
	hMonitorTask = SYS_TASK_CREATE_CALL(monitor, MONITOR_TASK_FCT, NULL);

	// FIXME
	WizeApp_Init();
}
/******************************************************************************/

extern admin_ann_fw_info_t sFwAnnInfo;;
extern struct update_ctx_s sUpdateCtx;

#ifndef MONITOR_TMO_EVT
#define MONITOR_TMO_EVT 0xFFFFFFFF
#endif

#ifndef MONITOR_PERIOD_EVT
#define MONITOR_PERIOD_EVT 30000
#endif

#include "default_device_config.h"

/*
 * EXTEND_FLAGS :
Get or Set the extend flags.
b[0] if 1: Disable ATCI +DBG;
b[1] : Reserved;
b[2] : Reserved;
b[3] : Reserved;
b[4] if 1: Activate the immediat update when image is ready;
b[5] if 1: Activate the WDT (bootcount for roll-back FW);
b[6] if 1: Activate the device id writing in NVM;
b[7] if 1: Activate the keys writing in NVM;
*/
#define EXT_FLAGS_UPD_IMM 0b00010000

void Monitor_Task(void const * argument)
{
	uint32_t ulEvent;
	uint32_t ret;

	uint32_t ulPeriod = pdMS_TO_TICKS(MONITOR_PERIOD_EVT);

	WizeApi_TimeMgr_Register(sys_get_pid());

	uint8_t extend_flags = 0;

#ifdef HAS_EXTEND_PARAMETER
/*
Get or Set the extend flags.
b[0] if 1: Disable ATCI +DBG;
b[1] : Reserved;
b[2] : Reserved;
b[3] : Reserved;
b[4] if 1: Activate the immediat update when image is ready;
b[5] if 1: Activate the WDT (bootcount for roll-back FW);
b[6] if 1: Activate the device id writing in NVM;
b[7] if 1: Activate the keys writing in NVM;
*/
	Param_Access(EXTEND_FLAGS, &extend_flags, 0);
#endif

	while(1)
	{
		if ( sys_flag_wait(&ulEvent, ulPeriod) )
		{
			// Day passed occurs
			if (ulEvent & TIME_FLG_DAY_PASSED)
			{
				ret = WizeApp_Time();

				// Periodic Install
				if (ret & WIZEAPP_INFO_PERIO_INST)
				{
					if ( WizeApp_Install() == WIZE_API_SUCCESS)
					{
						WizeApp_WaitSesComplete(SES_INST);
					}
				}
				// Back Full Power
				if (ret & WIZEAPP_INFO_FULL_POWER)
				{
					// go back in full power
					uint8_t temp = PHY_PMAX_minus_0db;
					Param_Access(TX_POWER, &temp, 1 );
				}
				// Current update ?
				if( sUpdateCtx.eUpdateStatus == UPD_STATUS_READY)
				{
					// Param_Access(DATEHOUR_LAST_UPDATE, tmp, 1);
					// Param_Access(VERS_HW_TRX, tmp, 0);
					// Param_Access(VERS_FW_TRX, tmp, 1);
					BSP_Boot_Reboot(0);
				}
			}
		}
		else
		{
			if(extend_flags & EXT_FLAGS_UPD_IMM)
			{
				if( sUpdateCtx.eUpdateStatus == UPD_STATUS_READY)
				{
					BSP_Boot_Reboot(0);
				}
			}
			// Timeout
			LOG_DBG("Monitor alive\n");
		}

#ifdef HAS_EXTEND_PARAMETER

#endif

	}
}

/******************************************************************************/

void WizeApp_CtxClear(void)
{
	// TODO :
	BSP_Rtc_Backup_Write(0, (uint32_t)0);
	BSP_Rtc_Backup_Write(1, (uint32_t)0);
}

void WizeApp_CtxRestore(void)
{
	// TODO :
	((uint32_t*)&sTimeUpdCtx)[0] = BSP_Rtc_Backup_Read(0);
	((uint32_t*)&sTimeUpdCtx)[1] = BSP_Rtc_Backup_Read(1);
}

void WizeApp_CtxSave(void)
{
	// TODO :
	BSP_Rtc_Backup_Write(0, ((uint32_t*)&sTimeUpdCtx)[0]);
	BSP_Rtc_Backup_Write(1, ((uint32_t*)&sTimeUpdCtx)[1]);
}
/******************************************************************************/
#define LO_ITF_TMO_EVT 0xFFFFFFFF

static const uint32_t session_mask[SES_NB] =
{
	[SES_INST] = SES_FLG_INST_MSK,
	[SES_ADM]  = SES_FLG_ADM_MSK,
	[SES_DWN]  = SES_FLG_DWN_MSK
};

int32_t WizeApp_WaitSesComplete(ses_type_t eSesId)
{
	uint32_t ret;
	uint32_t ulEvent;
	uint32_t mask;

	if( eSesId < SES_NB)
	{
		mask = session_mask[eSesId];
		do
		{
			if ( sys_flag_wait(&ulEvent, LO_ITF_TMO_EVT) == 0 )
			{
				// Timeout
				return -1;
			}

			ret = WizeApp_Common(ulEvent);
			ulEvent &= mask & SES_FLG_SES_COMPLETE_MSK;
		} while ( !(ulEvent) );

		ulEvent &= mask & SES_FLG_SES_ERROR_MSK;
		if ( !(ulEvent) )
		{
			if(eSesId == SES_ADM)
			{
				if (ret == ADM_WRITE_PARAM)
				{
					return 1;
				}
				else if ( ret == ADM_ANNDOWNLOAD)
				{
					return 2;
				}
			}
			return 0;
		}
	}
	return -1;
}

uint8_t WizeApp_GetAdmCmd(uint8_t *pData, uint8_t *rssi)
{
	uint8_t size = 0;
	if(pData && rssi)
	{
		if ( ((admin_rsp_t*)sAdmCtx.aSendBuff)->L7ErrorCode == ADM_NONE )
		{
			size = sAdmCtx.sCmdMsg.u8Size - 1;
			*rssi = sAdmCtx.sCmdMsg.u8Rssi;
			memcpy(pData, &(sAdmCtx.aRecvBuff[1]), size);
		}
	}
	return size;
}

/******************************************************************************/

#ifdef __cplusplus
}
#endif

/*! @} */
