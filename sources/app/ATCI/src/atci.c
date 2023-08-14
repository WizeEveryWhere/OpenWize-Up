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

/*==============================================================================
 * INCLUDES
 *============================================================================*/

#include <stdint.h>

#include "atci.h"
#include "atci_get_cmd.h"
#include "atci_resp.h"
#include "console.h"

#include "app_entry.h"
#include "platform.h"

/*==============================================================================
 * GLOBAL VARIABLES
 *============================================================================*/

/*==============================================================================
 * LOCAL FUNCTIONS PROTOTYPES
 *============================================================================*/

atci_error_t Exec_AT_Cmd(atci_cmd_t *atciCmdData);


#include "at_access_param.h"
#include "at_test_cmd.h"
#include "at_wize_cmd.h"
#include "at_key_cmd.h"
#include "at_extend_cmd.h"

#ifdef HAS_LO_UPDATE_CMD
	#include "at_lo_update_cmd.h"
#endif

#ifdef HAS_EXTERNAL_FW_UPDATE
	#include "at_ext_update_cmd.h"
#endif

/*!
 * @cond INTERNAL
 * @{
 */
typedef atci_error_t (*pf_exec_cmd_t)(atci_cmd_t *atciCmdData);

const pf_exec_cmd_t Atci_Exec_Cmd[NB_AT_CMD] =
{
	[CMD_AT] = Exec_AT_Cmd, //nothing to do
	[CMD_ATI] = Exec_ATI_Cmd,
	[CMD_ATZ] = Exec_ATZn_Cmd,
	[CMD_ATQ] = Exec_AT_Cmd, //something to do in states machine only
	[CMD_ATF] = Exec_ATF_Cmd,
	[CMD_ATW] = Exec_ATW_Cmd,
	[CMD_ATPARAM] = Exec_ATPARAM_Cmd,
	[CMD_ATIDENT] = Exec_ATIDENT_Cmd,
	[CMD_ATSEND] = Exec_ATSEND_Cmd,
	[CMD_ATPING] = Exec_ATPING_Cmd,
	[CMD_ATFC] = Exec_ATFC_Cmd,
	[CMD_ATTEST] = Exec_ATTEST_Cmd,

#ifndef HAS_ATZn_CMD
	[CMD_ATZC] = Exec_ATZn_Cmd,
#else
	[CMD_ATZ0] = Exec_ATZn_Cmd,
	[CMD_ATZ1] = Exec_ATZn_Cmd,
#endif

#ifndef HAS_ATKEY_CMD
	[CMD_ATKMAC] = Exec_ATKMAC_Cmd,
	[CMD_ATKENC] = Exec_ATKENC_Cmd,
#else
	[CMD_ATKEY] = Exec_ATKEY_Cmd,
#endif

#ifdef HAS_ATSTAT_CMD
	[CMD_ATSTAT] = Exec_ATSTAT_Cmd,
#endif

#ifdef HAS_ATCCLK_CMD
	[CMD_ATCCLK] = Exec_ATCCLK_Cmd,
#endif

#ifdef HAS_ATUID_CMD
	[CMD_ATUID] = Exec_ATUID_Cmd,
#endif

#ifdef HAS_EXTERNAL_FW_UPDATE
	[CMD_ATADMANN] = Exec_ATADMANN_Cmd,
#endif

#ifdef HAS_LO_UPDATE_CMD
	[CMD_ATANN] = Exec_ATANN_Cmd,
	[CMD_ATBLK] = Exec_ATBLK_Cmd,
	[CMD_ATUPD] = Exec_ATUPD_Cmd,
#ifdef HAS_LO_ATBMAP_CMD
	[CMD_ATBMAP] = Exec_ATBMAP_Cmd,
#endif
#endif

};

static uint8_t _is_lp_allowed_(void);
static uint8_t _init_lp_var_(void);

/*!
 * @}
 * @endcond
 */

/*==============================================================================
 * LOCAL VARIABLES
 *============================================================================*/
/*!
 * @cond INTERNAL
 * @{
 */

extern uint8_t bTestMode;

static uint32_t _u32_rx_cmd_tmo_;

SYS_MUTEX_CREATE_DEF(atci);
/*!
 * @}
 * @endcond
 */

/*==============================================================================
 * FUNCTIONS
 *============================================================================*/

/*!-----------------------------------------------------------------------------
 * @internal
 *
 * @brief		Receive AT command from UART interface
 *
 * @details		This function is blocking until a character has been received by UART or an error occurred
 *
 * @param[out]	atciCmdData Pointer "atci_cmd_t" structure:
 * 					- buf [in,out]: buffer to receive command (as text) from console
 * 					- len [in,out]: actual received command length (other fields are unused)
 *
 * @retval ATCI_NO_AT_CMD if no cmd received
 * @retval ATCI_AVAIL_AT_CMD if full command received
 * @retval ATCI_RX_ERR if buffer overflow or RX error
 * @retval ATCI_RX_CMD_TIMEOUT if no characters received for a specified time
 *
 * @endinternal
 *----------------------------------------------------------------------------*/
atci_status_t Atci_Rx_Cmd(console_buf_t *pComRxBuf)
{
	atci_status_t status = ATCI_RX_CMD_NONE;
	uint32_t ulEvent;

	pComRxBuf->len = 0;

	uint8_t ret = 0;
	// ---
	ret |= BSP_Uart_Receive(UART_ID_COM, pComRxBuf->data, (uint16_t)AT_CMD_BUF_LEN);
	if ( ret == DEV_SUCCESS )
	{
		if ( sys_flag_wait(&ulEvent, _u32_rx_cmd_tmo_) )
		{
			switch(ulEvent)
			{
				case UART_EVT_RX_ABT: // buffer overflow
					if (pComRxBuf->data[pComRxBuf->len] != END_OF_CMD_CHAR)
					{
						// If the last char is not the character match one
						status = ATCI_RX_CMD_ERR;
						break;
					}
				case UART_EVT_RX_CPLT:
					if ( pComRxBuf->len > AT_CMD_CODE_MIN_LEN )
					{
						status = ATCI_RX_CMD_OK;
					}
					// Don't take the END_OF_CMD_CHAR
					pComRxBuf->len--;
					break;
				default:
					break;
			}
		}
		else
		{
			// Timeout
			BSP_Uart_AbortReceive(UART_ID_COM);
			status = ATCI_RX_CMD_TIMEOUT;
		}
	}
	return status;
}

static uint8_t _bPaState_;

/******************************************************************************/
extern console_buf_t consoleRxBuf;
extern console_buf_t consoleTxBuf;

void Atci_Init(atci_cmd_t *atciCmdData)
{
	atciCmdData->pComTxBuf = &consoleTxBuf;
	atciCmdData->pComRxBuf = &consoleRxBuf;

	atciCmdData.hMutex = SYS_MUTEX_CREATE_CALL(atci);
	assert(atciCmdData.hMutex);

	Console_Init(END_OF_CMD_CHAR, &consoleRxBuf);
	EX_PHY_SetCpy();
	_bPaState_ = EX_PHY_GetPa();
}

void Atci_Sleep(void)
{
	Atci_Send_Sleep_Msg();
	_bPaState_ = EX_PHY_GetPa();
	EX_PHY_SetPa(0);
	Console_Disable();
	BSP_LowPower_Enter(LP_STOP2_MODE);
}

void Atci_Wakeup(void)
{
	Console_Enable();
	EX_PHY_SetPa(_bPaState_);
	Atci_Send_Wakeup_Msg();
}

/*!-----------------------------------------------------------------------------
 * @internal
 * @brief		AT command interpreter task
 *
 * @details 	Wait AT command reception, decode and execute it. Manage sleep and reset.
 *
 * @param[in]	argument Unused
 *
 * @endinternal
 *----------------------------------------------------------------------------*/
void Atci_Task(void const *argument)
{
	atci_state_t atciState = ATCI_WAKEUP;
	atci_cmd_t atciCmdData;
	atci_error_t status;
	uint8_t bLpAllowed;

	//Inits

	Atci_Init(&atciCmdData);
	bLpAllowed = _is_lp_allowed_();
	//Loop
	while(1)
	{
		switch(atciState)
		{
			case ATCI_SLEEP:
				Atci_Sleep();
				atciState = ATCI_WAKEUP;
				break;
			case ATCI_WAKEUP:
				Atci_Wakeup();
				atciState = ATCI_WAIT;
				break;
			case ATCI_WAIT:
			{
				switch(Atci_Rx_Cmd(atciCmdData.pComRxBuf))
				{
					case ATCI_RX_CMD_OK:
						atciState = ATCI_EXEC_CMD;
						break;
					case ATCI_RX_CMD_ERR:
						Atci_Resp_Ack(ATCI_ERR_RX_CMD);
						break;
					case ATCI_RX_CMD_TIMEOUT:
						if ( bLpAllowed )
						{
							atciState = ATCI_SLEEP;
							break;
						}
					default:
						break;
				}
				break;
			}

			case ATCI_EXEC_CMD:
			{
				bLpAllowed = _is_lp_allowed_();

				//decode and execute command
				status = Atci_Get_Cmd_Code(&atciCmdData);
				if(status == ATCI_ERR_NONE)
				{
					if(Atci_Exec_Cmd[atciCmdData.cmdCode] == NULL)
						status = ATCI_ERR_CMD_UNK;
					else
						status = Atci_Exec_Cmd[atciCmdData.cmdCode](&atciCmdData);
				}

				//send response
				if(status == ATCI_ERR_NONE)
				{
					Atci_Resp_Ack(status);
					switch(atciCmdData.cmdCode)
					{
						case CMD_ATQ:
							if (bLpAllowed)
							{
								atciState = ATCI_SLEEP;
								break;
							}
						default: //other commands
							atciState = ATCI_WAIT;
							break;
					}
				}
				else
				{
					Atci_Resp_Ack(status);
					atciState = ATCI_WAIT;
				}
				break;
			}

			default:
				Atci_Exec_Cmd[CMD_ATZ](&atciCmdData);
				break;
		}
	}
}

/******************************************************************************/

/*!-----------------------------------------------------------------------------
 * @brief		Execute AT command (nothing to do)
 *
 * @param[in,out]	atciCmdData  Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 *----------------------------------------------------------------------------*/
atci_error_t Exec_AT_Cmd(atci_cmd_t *atciCmdData)
{
	if(atciCmdData->cmdType != AT_CMD_WITHOUT_PARAM)
		return ATCI_ERR_PARAM_NB;

	return ATCI_ERR_NONE;
}

/******************************************************************************/
/******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */


static uint8_t _is_lp_allowed_(void)
{
	return (_init_lp_var_() && ( !bTestMode ))?(1):(0);
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
/*********************************** EOF **************************************/

/*! @} */
