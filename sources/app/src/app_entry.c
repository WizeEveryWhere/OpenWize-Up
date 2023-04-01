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

void* hTask;
extern void Atci_Task(void const * argument);
#define APP_TASK_NAME atci
#define APP_TASK_FCT Atci_Task
#define APP_STACK_SIZE 800
#define APP_PRIORITY (UBaseType_t)(tskIDLE_PRIORITY+1)
SYS_TASK_CREATE_DEF(atci, APP_STACK_SIZE, APP_PRIORITY);

/*!
 * @}
 * @endcond
 */

/**
  * @brief  Called to initialize application before starting the scheduler.
  */
void App_Init(void)
{
	hTask = SYS_TASK_CREATE_CALL(atci, APP_TASK_FCT, NULL);
}

/******************************************************************************/
#include "wize_app.h"
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
