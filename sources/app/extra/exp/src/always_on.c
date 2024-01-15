/**
  * @file always_on.c
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
  * @par 1.0.0 : 2023/12/06 [GBI]
  * Initial version
  *
  */

/*!
 * @addtogroup wize_api
 * @{
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "always_on.h"
#include <assert.h>
#include <string.h>

#include "wize_api.h"
#include "rtos_macro.h"

#include "net_api_private.h"
#include "net_mgr.h"

/******************************************************************************/

#ifndef LSN_STACK_SIZE
	#define LSN_STACK_SIZE 100
#endif

#ifndef LSN_PRIORITY
	#define LSN_PRIORITY (UBaseType_t)(tskIDLE_PRIORITY+2)
#endif

#ifndef LSN_EVT_TMO
	#define LSN_EVT_TMO 0xFFFFFFFF
#endif

SYS_TASK_CREATE_DEF(lsn, LSN_STACK_SIZE, LSN_PRIORITY);

static void _lsn_tsk_(void const *argument);
static uint32_t _always_on_fsm_(uint32_t ulEvent);
static uint32_t _always_on_net_cfg_(void);
static uint32_t _always_on_medium_cfg_(void);
static uint32_t _always_on_proto_cfg_(void);

/******************************************************************************/

struct cust_mgr_ctx_s sCustCtx __attribute__(( weak ));
static void* hLsnTask;

/******************************************************************************/
/*!
 * @brief This function TODO
 *
 * @return
 */
atci_error_e Exec_CMD_ATLSN(atci_cmd_t *atciCmdData)
{
	atci_error_e status = ATCI_ERR_NONE;

	if (atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
	{
		Atci_Cmd_Param_Init(atciCmdData);

		status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
		if (status != ATCI_ERR_NONE)
		{
			return status;
		}
		if (*(atciCmdData->params[0].val8) == 1)
		{
			if (AlwaysOn_Start())
			{
				status = ATCI_ERR_UNK;
			}
		}
		else
		{
			if (AlwaysOn_Stop())
			{
				status = ATCI_ERR_UNK;
			}
		}
	}
	else
	{
		status = ATCI_ERR_PARAM_NB;
	}
	return status;
}

/*!
 * @brief This function TODO
 *
 * @return
 */
atci_error_e Exec_UNS_ATLSN(atci_cmd_t *atciCmdData)
{
	uint8_t size;

	atciCmdData->cmdCode = UNS_ATLSN;
	Atci_Cmd_Param_Init(atciCmdData);

	atciCmdData->nbParams = 2;

	atciCmdData->params[0].size = sCustCtx.sCmdMsg.u8Size;
	memcpy(atciCmdData->params[0].data, sCustCtx.sCmdMsg.pData, sCustCtx.sCmdMsg.u8Size);

	atciCmdData->params[1].size = 1;
	*(atciCmdData->params[1].data) = sCustCtx.sCmdMsg.u8Rssi;

	AlwaysOn_Rearm();

	Atci_Resp(atciCmdData);

	return ATCI_ERR_NONE;
}

/******************************************************************************/

/*!
 * @brief This function TODO
 *
 * @return
 */
int32_t AlwaysOn_Setup(void)
{
	hLsnTask = SYS_TASK_CREATE_CALL(lsn, _lsn_tsk_, &sAtciCtx);
}

/*!
 * @brief This function TODO
 *
 * @return
 */
int32_t AlwaysOn_Start(void)
{
	uint8_t i;
    for (i = 0; i < SES_NB; i++)
    {
        if ( WizeApi_SesGetState(i) != SES_STATE_IDLE )
        {
            return -1;
        }
    }
    if ( NetMgr_Open(hLsnTask) )
    {
    	return -1;
    }
    if ( _always_on_net_cfg_() )
    {
    	NetMgr_Close();
    	return -1;
    }
    if ( NetMgr_Listen(&(sCustCtx.sCmdMsg), 0xFFFFFFFF, NET_LISTEN_TYPE_MANY) )
    {
        NetMgr_Close();
        return -1;
    }
    return 0;
}

/*!
 * @brief This function TODO
 *
 * @return
 */
int32_t AlwaysOn_Stop(void)
{
	NetMgr_Close();
	return 0;
}

/*!
 * @brief This function TODO
 *
 * @return
 */
int32_t AlwaysOn_Rearm(void)
{
	if ( NetMgr_ListenReady() )
	{
		return -1;
	}
	return 0;
}

/******************************************************************************/
/*!
 * @brief This function TODO
 *
 * @return
 */
static void _lsn_tsk_(void const *argument)
{
	uint32_t ulEvent;
	do
	{
		if ( sys_flag_wait(&ulEvent, LSN_EVT_TMO))
		{
		    if (ulEvent & SES_EVT_RECV_DONE)
			{
		    	LOG_INF("CMD Received\n");
		#ifdef HAS_ALWAYS_ON_TREATMENT
				// Has Treatment
		#else
				if (sCustCtx.aRecvBuff[0] == ADM_WRITE_PARAM)
				{
					UNS_Notify(UNS_ATLSN);
				}
		#endif
		    }
		    if (ulEvent & SES_EVT_TIMEOUT)
			{
		    	// should never happen
			}
		}
		// else { // timeout }
	} while (1);
}

/******************************************************************************/
/*!
 * @static
 * @brief This function configure the NetMgr.
 *
 * @return The NetDev status
 */
static
uint32_t _always_on_net_cfg_(void)
{
	uint32_t ret;
	ret = _always_on_medium_cfg_();
	ret |= _always_on_proto_cfg_();
    return ret;
}

/*!
 * @static
 * @brief This function configure the NetMgr medium.
 *
 * @return The NetDev status
 */
static
uint32_t _always_on_medium_cfg_(void)
{
    struct medium_cfg_s sMediumCfg;
    WizeApi_FillMediumCfg(&sMediumCfg);
    return NetMgr_Ioctl(NETDEV_CTL_CFG_MEDIUM, (uint32_t)(&sMediumCfg));
}

/*!
 * @static
 * @brief This function configure the NetMgr protocol.
 *
 * @return The NetDev status
 */
static
uint32_t _always_on_proto_cfg_(void)
{
	struct proto_config_s sProto_Cfg;
	WizeApi_FillProtoCfg(&sProto_Cfg);
	sProto_Cfg.filterDisL2_b.AField = 1;
    return NetMgr_Ioctl(NETDEV_CTL_CFG_PROTO, (uint32_t)(&sProto_Cfg));
}

/******************************************************************************/

#ifdef __cplusplus
}
#endif

/*! @} */
