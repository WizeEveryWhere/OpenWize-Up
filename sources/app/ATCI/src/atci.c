/**
  * @file atci.c
  * @brief This file contains functions of the AT command interpreter for WizeUp
  * module
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
  * @par 0.0.1 : 2021/01/11 [Alciom]
  * Dev. version
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

#include <stdint.h>

#include "atci.h"
#include "atci_get_cmd.h"
#include "atci_resp.h"
#include "console.h"

#include "app_entry.h"
#include "platform.h"

#include "itf.h"
#include "wize_app_itf.h"

#include "at_command.h"
#include "at_extend_cmd.h"

/******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */

static void _atci_init_(atci_cmd_t *pAtciCtx);
static void _atci_tsk_(void const * argument);
static int32_t _inner_loop_(atci_cmd_t *pAtciCtx);

static void _uns_tsk_(void const *argument);
static int32_t _uns_notify_(uint32_t src, uint32_t evt);

static uint8_t _is_lp_allowed_(void);
static uint8_t _init_lp_var_(void);

void* hAtciTask;
static void* hUnsTask;
static void *hUnsQueue;
static void *hAtciMutex;

static uint32_t _u32_rx_cmd_tmo_;
static atci_cmd_t sAtciCtx;

struct queue_evt_s
{
	uint32_t src;
	uint32_t evt;
};

/******************************************************************************/
extern uint8_t bTestMode;

extern console_buf_t consoleRxBuf;
extern console_buf_t consoleTxBuf;

/******************************************************************************/

#define ATCI_TASK_NAME atci
#define ATCI_TASK_FCT _atci_tsk_

#ifndef ATCI_STACK_SIZE
	#define ATCI_STACK_SIZE 300
#endif

#ifndef ATCI_PRIORITY
	#define ATCI_PRIORITY (UBaseType_t)(tskIDLE_PRIORITY+2)
#endif

SYS_TASK_CREATE_DEF(atci, ATCI_STACK_SIZE, ATCI_PRIORITY);
SYS_MUTEX_CREATE_DEF(atci);

#ifndef ATCI_MUTEX_TMO
	#define ATCI_MUTEX_TMO 100 // in rtos cycles
#endif


#define UNS_TASK_NAME uns
#define UNS_TASK_FCT _uns_tsk_

#ifndef UNS_STACK_SIZE
	#define UNS_STACK_SIZE 120
#endif

#ifndef UNS_PRIORITY
	#define UNS_PRIORITY (UBaseType_t)(tskIDLE_PRIORITY+2)
#endif

SYS_TASK_CREATE_DEF(uns, UNS_STACK_SIZE, UNS_PRIORITY);

#ifndef UNS_QUEUE_LEN
	#define UNS_QUEUE_LEN 10
#endif

#ifndef UNS_QUEUE_SIZE
	#define UNS_QUEUE_SIZE sizeof(struct queue_evt_s)
#endif

SYS_QUEUE_CREATE_DEF(uns, UNS_QUEUE_LEN, UNS_QUEUE_SIZE);

#ifndef UNS_QUEUE_RCV_TMO
	#define UNS_QUEUE_RCV_TMO 0xFFFFFFFF // in rtos cycles
#endif

#ifndef UNS_QUEUE_SEND_TMO
	#define UNS_QUEUE_SEND_TMO 100 // in rtos cycles
#endif

/*!
 * @}
 * @endcond
 */

/******************************************************************************/
/******************************************************************************/

/*!
 * @brief Setup the ATCI task
 *
 */
void Atci_Setup(void)
{
	hAtciTask = SYS_TASK_CREATE_CALL(atci, ATCI_TASK_FCT, &sAtciCtx);
	hUnsTask = SYS_TASK_CREATE_CALL(uns, UNS_TASK_FCT, &sAtciCtx);
	hUnsQueue = SYS_QUEUE_CREATE_CALL(uns);
	hAtciMutex = SYS_MUTEX_CREATE_CALL(atci);

	assert(hAtciTask);
	assert(hUnsTask);
	assert(hUnsQueue);
	assert(hAtciMutex);

	ITF_Setup();

	sAtciCtx.pComTxBuf = &consoleTxBuf;
	sAtciCtx.pComRxBuf = &consoleRxBuf;
	sAtciCtx.pCmdDesc = aAtDescCmd;
	sAtciCtx.cmd_code_nb = NB_AT_CMD; //sizeof(aAtDescCmd);
	sAtciCtx.pf_inner_loop = _inner_loop_;
}

/*!
 * @brief This function proceed the AT command parsing
 *
 * @param [in] pAtciCtx Pointer on the ATCI context
 * @param [in] ulEvent  Current event
 *
 * @return 0 if success, -1 otherwise
 */
int32_t Atci_Com(atci_cmd_t *pAtciCtx, uint32_t ulEvent)
{
	atci_error_e eErr = ATCI_ERR_NONE;
	// ---
	switch (ulEvent)
	{
		case UART_EVT_RX_ABT :// buffer overflow
		case UART_EVT_RX_CPLT:
		{
			eErr = Atci_Get_Cmd_Code(pAtciCtx);
			break;
		}
		default:
		{
			eErr = ATCI_ERR_RX_CMD;
			break;
		}
	}
	pAtciCtx->eErr = eErr;
	return (eErr)?(-1):(0);
}

/*!
 * @brief This function execute the AT command
 *
 * @param [in] pAtciCtx Pointer on the ATCI context
 * @param [in] ulEvent  Current event
 *
 * @return 0 if success, -1 otherwise
 */
int32_t Atci_Run(atci_cmd_t *pAtciCtx, uint32_t ulEvent)
{
	atci_error_e eErr = ATCI_ERR_UNK;

	if (sys_mutex_acquire(hAtciMutex, ATCI_MUTEX_TMO))
	{
		if (ulEvent)
		{
			// Unsolicited command
			eErr = Generic_Notify_SetCode(pAtciCtx, ulEvent);
		}
		else
		{
			// "standard" command received by COM port, so keep error from Atci_Com
			eErr = pAtciCtx->eErr;
		}

		pAtciCtx->eErr = ATCI_ERR_NONE;

		// Try to Execute command
		if (eErr == ATCI_ERR_NONE)
		{
			if (pAtciCtx->pCmdDesc[pAtciCtx->cmdCode].pf != NULL)
			{
				eErr = pAtciCtx->pCmdDesc[pAtciCtx->cmdCode].pf(pAtciCtx);
			}
			else
			{
				eErr = ATCI_ERR_CMD_UNK;
			}
		}
		// Send ACK/NACK
		if (pAtciCtx->bNeedAck)
		{
			Atci_AckNack(eErr);
		}
		else
		{
			pAtciCtx->bNeedAck = 1;
		}
		sys_mutex_release(hAtciMutex);
	}
	// else // timeout
	return (eErr)?(-1):(0);
}
/******************************************************************************/
/*!
 * @brief This function push a notification to the ATCI
 *
 * @param [in] evt Event or id of the notification
 *
 * @return 0 if success, -1 otherwise (queue is full)
 */
int32_t UNS_Notify(uint32_t evt)
{
	return _uns_notify_(UNK_SRC_ID, evt );
}

/*!
 * @brief This function push a notification to the ATCI
 *
 * @param [in] evt Event or id of the notification
 *
 * @return 0 if success, -1 otherwise (queue is full)
 */
int32_t UNS_NotifyAtci(uint32_t evt)
{
	return _uns_notify_(NOTIFY_SRC_ID, evt );
}

/*!
 * @brief This function push a notification to the ATCI
 *
 * @param [in] evt Event or id of the notification
 *
 * @return 0 if success, -1 otherwise (queue is full)
 */
int32_t UNS_NotifyTime(uint32_t evt)
{
	return _uns_notify_(TIME_SRC_ID, evt );
}

/*!
 * @brief This function push a notification to the ATCI
 *
 * @param [in] evt Event or id of the notification
 *
 * @return 0 if success, -1 otherwise (queue is full)
 */
int32_t UNS_NotifySession(uint32_t evt)
{
	return _uns_notify_(SESSION_SRC_ID, evt );
}

void ready_to_sleep()
{

}

/******************************************************************************/
/******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */

#define COM_MSK 0x0F

#define TMO_EVT 0x00010000
#ifndef TMO_RET
	#define TMO_RET 800 // in ms
#endif

#ifndef TMO_RET_MIN
	#define TMO_RET_MIN 5 // in ms
#endif

/*!
 * @}
 * @endcond
 */

/******************************************************************************/
/******************************************************************************/

/*!
 * @brief TODO:
 *
 */
static void _atci_init_(atci_cmd_t *pAtciCtx)
{


	Console_Init(END_OF_CMD_CHAR, &consoleRxBuf);
	EX_PHY_SetCpy();


	pAtciCtx->eErr = ATCI_ERR_NONE;

	pAtciCtx->idx = 0;
	pAtciCtx->nbParams = 0;
	pAtciCtx->eState = ATCI_WAIT;
	pAtciCtx->bNeedAck = 1;
	pAtciCtx->bNeedReboot = 0;
	pAtciCtx->bTestMode = 0;
}

/*!
 * @static
 * @brief This is the ATCI task function
 *
 * @param [in] argument Opaque pointer. Expect an ATCI context pointer's
 *
 */
static void _atci_tsk_(void const *argument)
{
	atci_cmd_t *pAtciCtx;
	assert(argument);
	pAtciCtx = (atci_cmd_t *)argument;

	uint32_t ulEvent;
	uint32_t ulNotify;

	uint8_t bTmo;
	uint8_t bComIsStarted;

	//Inits
	bTmo = 0;
	bComIsStarted = 0;
	ulEvent = 0;
	_atci_init_(pAtciCtx);

	while(1)
	{
		pAtciCtx->bLpAllowed = _is_lp_allowed_();
		do
		{
			if (ulEvent & COM_MSK)
			{
				Atci_Com(pAtciCtx, ulEvent);
				bComIsStarted = 0;
				pAtciCtx->eState = ATCI_EXEC_CMD;
				ulEvent &= ~COM_MSK;
			}
			if (pAtciCtx->eState == ATCI_EXEC_CMD)
			{
				if ( (Atci_Run(pAtciCtx, 0) == 0) && (pAtciCtx->bNeedReboot))
				{
					pAtciCtx->bNeedReboot = 0;
					pAtciCtx->cmdType = AT_CMD_WITHOUT_PARAM;
					pAtciCtx->cmdCode = CMD_ATZ1;
					pAtciCtx->eState = ATCI_EXEC_CMD;
					ulEvent = ~COM_MSK;
				}
				else
				{
					pAtciCtx->eState = ATCI_WAIT;
					ulEvent = 0;
				}
			}
		} while (ulEvent);

		// If COM is not started, then start it
		if (!bComIsStarted)
		{
			if ( DEV_SUCCESS == BSP_Uart_Receive(UART_ID_COM, consoleRxBuf.data, (uint16_t)AT_CMD_BUF_LEN))
			{
				bComIsStarted = 1;
			}
			else
			{
				// failure
				//_send_to_this_(COM_FAILURE);
			}
		}

		// Wait for event
		if ( sys_flag_wait(&ulEvent, _u32_rx_cmd_tmo_) == 0 )
		{
			// timeout
			bTmo = 1;
			if (bTmo && pAtciCtx->bLpAllowed && !(pAtciCtx->bTestMode))
			{
				BSP_Uart_AbortReceive(UART_ID_COM);
				bComIsStarted = 0;
				bTmo = 0;

				pAtciCtx->cmdCode = CMD_ATQ;
				pAtciCtx->eState = ATCI_EXEC_CMD;
			}
		}
	}
}

/*!
 * @static
 * @brief Inner loop called when remote external update is requested
 *
 * @param [in] pAtciCtx Pointer on the ATCI context
 *
 * @return The ATADMANN return error code 0 if success, negative value otherwise
 */
static int32_t _inner_loop_(atci_cmd_t *pAtciCtx)
{
	time_evt_t sTimeEvt;
	int32_t ret;
	uint32_t ulEvent;
	uint32_t u32Tmo;
	int16_t i16DeltaMs;
	atci_error_e status;
	uint8_t bComIsStarted;
	uint8_t bTimeEvtIsStarted;

	ret = -1;
	status = ATCI_ERR_NONE;
	bComIsStarted = 0;
	bTimeEvtIsStarted = 0;

	u32Tmo = 0;
	Param_Access(EXCH_RESPONSE_DELAY, (uint8_t*)&( u32Tmo ), 0);
	if (u32Tmo)
	{
		u32Tmo--;
		i16DeltaMs = TMO_RET;
	}
	else
	{
		i16DeltaMs = TMO_RET_MIN;
	}
	TimeEvt_TimerInit( &sTimeEvt, sys_get_pid(), TIMEEVT_CFG_ONESHOT);

	if ( Exec_ATADMANN_Notify(pAtciCtx) != ATCI_ERR_NONE )
	{
		return ret;
	}

	TimeEvt_TimerStart(&sTimeEvt, u32Tmo, i16DeltaMs, (uint32_t)TMO_EVT);
	bTimeEvtIsStarted = 1;

	do
	{
		if (!bComIsStarted)
		{
			if ( BSP_Uart_Receive(UART_ID_COM, pAtciCtx->pComRxBuf->data, (uint16_t)AT_CMD_BUF_LEN) != DEV_SUCCESS)
			{
				ret = -2;
				break;
			}
			bComIsStarted = 1;
		}

		if ( sys_flag_wait(&ulEvent, WIZE_APP_ITF_TMO_EVT) == 0)
		{
			// timeout
			break;
		}

		if (ulEvent & COM_MSK)
		{
			bComIsStarted = 0;

			status = ATCI_ERR_CMD_FORBIDDEN;
			Atci_Com(pAtciCtx, ulEvent);
			pAtciCtx->eErr = ATCI_ERR_NONE;
			if (strcmp(pAtciCtx->cmdCodeStr, pAtciCtx->pCmdDesc[UNS_ATADMANN].str) == 0)
			{
				pAtciCtx->cmdCode = UNS_ATADMANN;
				Atci_Cmd_Param_Init(pAtciCtx);
				status = Atci_Buf_Get_Cmd_Param(pAtciCtx, PARAM_INT8);
				if (status == ATCI_ERR_NONE)
				{
					ret = *(pAtciCtx->params[0].data);
					break;
				}
			}
			Atci_AckNack(status);
		}

		if (ulEvent & TMO_EVT)
		{
			break;
		}

	} while (1);

	if (bTimeEvtIsStarted)
	{
		TimeEvt_TimerStop( &sTimeEvt);
	}

	if (bComIsStarted)
	{
		BSP_Uart_AbortReceive(UART_ID_COM);
	}

	return ret;
}

/******************************************************************************/
/*!
 * @static
 * @brief This is the "Unsolicited" task function
 *
 * @param [in] argument Opaque pointer. Expect an ATCI context pointer's
 *
 */
static void _uns_tsk_(void const *argument)
{
	atci_cmd_t *pAtciCtx;

	struct queue_evt_s sQEvt = { .src = UNK_SRC_ID, .evt = 0 };
	assert(argument);
	pAtciCtx = (atci_cmd_t *)argument;

	while(1)
	{
		if ( sys_queue_receive(hUnsQueue, &sQEvt,  UNS_QUEUE_RCV_TMO) )
		{
			// pending
			switch(sQEvt.src)
			{
				case TIME_SRC_ID :
				{
					sQEvt.evt += TIME_NOTIFY + NB_AT_UNS;
					break;
				}
				case NOTIFY_SRC_ID :
				{
					sQEvt.evt += NB_AT_UNS;
					break;
				}
				case UNK_SRC_ID :
				{
					break;
				}
				default :
				{
					sQEvt.evt = 0;
					break;
				}
			}

			if (sQEvt.evt)
			{
				Atci_Run(pAtciCtx, sQEvt.evt);
			}
			// Should we sleep for ms before take into account a new element
		}
		// else { // timeout }
	}
}

/*
void WizeApi_OnTimeFlag(uint32_t u32Flg)
{
	_uns_as_time_(u32Flg);
}
*/


/*!
 * @static
 * @brief This function push a notification to the ATCI
 *
 * @param [in] src Source of the event
 * @param [in] evt Event or id of the notification
 *
 * @return 0 if success, -1 otherwise (queue is full)
 */
static int32_t _uns_notify_(uint32_t src, uint32_t evt)
{
	struct queue_evt_s sQEvt;
	sQEvt.src = src;
	sQEvt.evt = evt;

	if ( sys_queue_send(hUnsQueue, &sQEvt, UNS_QUEUE_SEND_TMO) != pdTRUE )
	{
		// queue is full
		LOG_WRN("UNS queue is full");
		return -1;
	}
	return 0;
}

/******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */

static uint8_t _is_lp_allowed_(void)
{
	uint8_t u8Flags_0 = 0;
	uint8_t u8Flags_1 = 0;

	// Check logger is enable
	Param_Access(LOGGER_LEVEL, (uint8_t*)(&u8Flags_0), 0);
	Param_Access(LOGGER_TIME_OPT, (uint8_t*)(&u8Flags_1), 0);
	Logger_SetLevel( u8Flags_0, u8Flags_1 );

	// check if ATCI DBG is enable
#ifdef HAS_EXTEND_PARAMETER
	Param_Access(EXTEND_FLAGS, &u8Flags_0, 0);
#else
	u8Flags_0 = 0b11100101;
#endif

	Atci_Send_Dbg_Enable( (u8Flags_0 & EXT_FLAGS_DBG_MSG_EN_MSK) );

	return (_init_lp_var_()); // && ( !bTestMode ))?(1):(0);
}


static uint8_t _init_lp_var_(void)
{
	// Init LP mode
	uint32_t u32LPdelay = CONSOLE_RX_TIMEOUT;
	uint8_t eLPmode = 1;
#ifdef HAS_LP_PARAMETER
	/*
	 *  0b xxxx xxxxxx00
	 *  0b xxxx xx00 : disable
	 *  0b xxxx xx01 : enable
	 *  0b 0000 xx01 : manual LP (no TMO)
	 *  0b 0001 xx01 : 1 seconds
	 *  0b 0010 xx01 : 2 seconds
	 *  0b 0011 xx01 : 3 seconds
	 *  ....
	 *  0b 1111 xx01 : 15 seconds
	 */
	Param_Access(LOW_POWER_MODE, &eLPmode, 0);
	u32LPdelay = (eLPmode >> 4) * 1000;
	eLPmode = eLPmode & 0x1;
	if (!u32LPdelay)
	{
		u32LPdelay = 0xFFFFFFFF;
	}
#endif
	BSP_Console_SetTXTmo(CONSOLE_TX_TIMEOUT);
	//BSP_Console_SetRXTmo(u32LPdelay);
	_u32_rx_cmd_tmo_ = pdMS_TO_TICKS(u32LPdelay);
	return eLPmode;
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
