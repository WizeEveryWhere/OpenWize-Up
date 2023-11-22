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
#include "itf.h"

extern void Sys_Init(void);
extern void Sys_Start(void);

void App_Init(void);

/**
  * @brief  The application entry point.
  * @retval None
  */
void app_entry(void)
{
#ifndef NOT_BOOTABLE
  	Sys_Init();
  	App_Init();
  	Sys_Start();
#endif
}

/******************************************************************************/

/*!
 * @cond INTERNAL
 * @{
 */


extern void Atci_Setup(void);

void* hMonitorTask;
void Monitor_Task(void const * argument);
#define MONITOR_TASK_NAME monitor
#define MONITOR_TASK_FCT Monitor_Task
#define MONITOR_STACK_SIZE 400
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
	Update_Setup();
	Atci_Setup();

	hMonitorTask = SYS_TASK_CREATE_CALL(monitor, MONITOR_TASK_FCT, NULL);

	// FIXME
	WizeApp_Init();
	NetMgr_Init();
	NetMgr_Uninit();
}
/******************************************************************************/
extern boot_info_t gBootInfo;
extern admin_ann_fw_info_t sFwAnnInfo;
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
	uint8_t boot_count_max = 0;
	uint8_t boot_count_current = gBootInfo.unstab_cnt;

#ifdef HAS_EXTEND_PARAMETER
/*
Get or Set the extend flags.
b[0] if 1: Enable ATCI +DBG;
b[1] : Reserved;
b[2] if 1: Activate the immediate update when image is ready;
b[3] : Reserved;
b[4] : Reserved;
b[5] if 1: Activate the phy calibration (rssi, power, internal) writing in NVM;
b[6] if 1: Activate the device id writing in NVM;
b[7] if 1: Activate the keys writing in NVM;
*/
	Param_Access(EXTEND_FLAGS, &extend_flags, 0);
	// Write the current boot count
	Param_Access(BOOT_COUNT, &(boot_count_current), 1);
	// Get the boot count max
	Param_Access(BOOT_COUNT_MAX, &boot_count_max, 0);
#endif

	UNS_NotifyAtci(BOOT_NOTIFY);
	//
	LOG_DBG("Monitor Boot Count %d\n", gBootInfo.unstab_cnt);
	if (boot_count_current > boot_count_max)
	{
		LOG_WRN("Instability detected...Roll-Back\n");
#ifndef BUILD_STANDALONE_APP
		//swap to the previous sw slot (if any)
		UpdateArea_SetBootReq(BOOT_REQ_BACK);
		//(option) reboot
		BSP_Boot_Reboot(0);
#endif
	}
	else
	{
		UpdateArea_SetBootable();
	}
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
					UNS_NotifyTime((uint32_t)WIZEAPP_INFO_PERIO_INST);
				}
				// Back Full Power
				if (ret & WIZEAPP_INFO_FULL_POWER)
				{
					// go back in full power
					uint8_t temp = PHY_PMAX_minus_0db;
					Param_Access(TX_POWER, &temp, 1 );

					UNS_NotifyTime((uint32_t)WIZEAPP_INFO_FULL_POWER);
				}
				// Current update ?
				if( Update_IsReady() )
				{
					// Param_Access(DATEHOUR_LAST_UPDATE, tmp, 1);
					// Param_Access(VERS_HW_TRX, tmp, 0);
					// Param_Access(VERS_FW_TRX, tmp, 1);
					BSP_Boot_Reboot(0);
				}
				// Clear boot count
				BSP_UpdateInfo();
			}
			if (ulEvent & TIME_FLG_TIME_ADJ)
			{
				UNS_NotifyTime((uint32_t)WIZEAPP_INFO_CLOCK_MSK);
			}
		}
		// Low Power management
		/*
		else if (ulEvent & LOWPOWER_EVT_REQ)
		{
			// Set LP allowed
		}
		*/
		else
		{
#ifndef BUILD_STANDALONE_APP
			BSP_Iwdg_Refresh();
#endif
			if(extend_flags & EXT_FLAGS_UPD_IMM)
			{
				if( Update_IsReady() )
				{
					BSP_Boot_Reboot(0);
				}
			}
			// Get state
			int32_t state;
			for (uint8_t i = 0; i < SES_NB; i++)
			{
				state = WizeApi_GetState(i);
			}

			state = NetMgr_IsBusy(); // priority 5
			// timemgr task // priority 4
			// wizeapi task // priority 4
			state = Logger_IsBusy(); // priority 3
			// update task // priority 2
			// atci task  // priority 2
			// uns task // priority 2


			// Timeout
			LOG_DBG("Monitor alive\n");
		}
#ifdef HAS_EXTEND_PARAMETER

#endif
	}
}

/******************************************************************************/
/*
 * task notification
 * queue
 * event_group
 *
 * semaphore
 * bin_semaphore
 * mutex
 *
 *
 */

/*
// Determine whether we are in thread mode or handler mode.
static int inHandlerMode (void)
{
  return __get_IPSR() != 0;
}
*/
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

#ifdef __cplusplus
}
#endif

/*! @} */
