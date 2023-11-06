/**
  * @file: wize_app_itf.c
  * @brief: // TODO This file ...
  * 
  *****************************************************************************
  * @Copyright 2019, GRDF, Inc.  All rights reserved.
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
  *****************************************************************************
  *
  * Revision history
  * ----------------
  * 1.0.0 : 2023/08/27[GBI]
  * Initial version
  *
  *
  */

/*!
 *  @addtogroup itf
 *  @ingroup app
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "wize_app_itf.h"
#include "wize_app.h"
#include "rtos_macro.h"
#include "update.h"

#include "phy_layer_private.h"

/******************************************************************************/
static void _adm_ann_to_fw_info_(admin_ann_fw_info_t *pFwAnnInfo, admin_cmd_anndownload_t *pAnn);

/******************************************************************************/
#ifdef HAS_CUSTOM_ADM_CMD
void AdmInt_Custom(net_msg_t *pReqMsg, net_msg_t * pRspMsg)
{

}
void AdmInt_PostCustom(net_msg_t *pReqMsg, net_msg_t *pRspMsg)
{

}
#endif

/*
void AdmInt_PostAnndownload(net_msg_t *pReqMsg, net_msg_t *pRspMsg)
{
	(void)pReqMsg;
	(void)pRspMsg;
}
*/

#ifdef HAS_HIRES_TIME_MEAS
	extern int32_t BSP_HiResTmr_EnDis(uint8_t bEnable);
	extern void BSP_HiResTmr_Capture(register uint8_t id);
	extern uint32_t BSP_HiResTmr_Get(register uint8_t id);
	#define INST_INT_TMR_ENABLE() BSP_HiResTmr_EnDis(1);
    #define INST_INT_TMR_DISABLE() BSP_HiResTmr_EnDis(0);
	#define INST_INT_TMR_GET_PONG_COMPLETE() BSP_HiResTmr_Get(CAPTURE_ID_RX_COMPLETE)
	#define INST_INT_TMR_GET() BSP_HiResTmr_Get(1)
	#define INST_INT_TMR_CAPTURE() BSP_HiResTmr_Capture(1)
#endif

extern admin_ann_fw_info_t sFwAnnInfo;
uint8_t WizeApp_OnDwnBlkRecv(uint16_t u16Id, const uint8_t *pData)
{
	/*
	 * WARNING :
	 * - Block Id from the Wize rev 1.2 protocol start from 1
	 * - "Inside" management start from 0
	 */
	if ((uint8_t)sFwAnnInfo.u32Type == UPD_TYPE_INTERNAL)
	{
		ImgStore_StoreBlock(u16Id - 1, (uint8_t *)pData);
	}
	else if ((uint8_t)sFwAnnInfo.u32Type == UPD_TYPE_EXTERNAL)
	{
		ITF_StoreBlock(u16Id, (uint8_t *)pData);
	}
	return 0;
}

/*
uint8_t WizeApp_AnnCheckFwInfo(admin_cmd_anndownload_t *pAnn)
{
	admin_ann_fw_info_t sFwAnnInfo;
	if ( (pAnn->L7DayRepeat & 0xF0) == 0x00)
	{
		register uint8_t eErrCode;
		uint8_t eErrParam;
		_adm_ann_to_fw_info_(&sFwAnnInfo, pAnn);
		sFwAnnInfo.u32Type = (uint32_t)UPD_TYPE_INTERNAL;
		eErrCode = AdmInt_AnnCheckIntFW(&sFwAnnInfo, &eErrParam);
		WizeApp_AnnReady(eErrCode, eErrParam);
	}
	return 0;
}
*/

/******************************************************************************/
/*!
 * @brief This function get the last ADM command.
 *
 * @param [out] pData Pointer on output buffer
 * @param [out] rssi  Pointer on output RSSI
 *
 * @retval the size of ADM command message or 0 if an error RSP was already or
 *         if the given pointer is NULL,
 */
uint8_t WizeApp_GetAdmCmd(uint8_t *pData, uint8_t *rssi)
{
	uint8_t size = 0;
	if (pData)
	{
		// If RSP has already been sent
		//if (bCheckErr && (sAdmCtx.sCmdMsg.u16Id == sAdmCtx.sRspMsg.u16Id) )
		if (sAdmCtx.sCmdMsg.u16Id == sAdmCtx.sRspMsg.u16Id)
		{
			if ( ((admin_rsp_t*)sAdmCtx.aSendBuff)->L7ErrorCode != ADM_NONE )
			{
				return size;
			}
		}

		size = sAdmCtx.sCmdMsg.u8Size - 1;
		memcpy(pData, &(sAdmCtx.aRecvBuff[1]), size);

		if (rssi)
		{
			*rssi = sAdmCtx.sCmdMsg.u8Rssi;
		}
	}
	return size;
}

/*!
 * @brief This function get the last FW info
 *
 * @param [out] pData Pointer on output buffer
 * @param [out] rssi  Pointer on output RSSI
 *
 * @retval the size of FW info or 0 if an error RSP was already or
 *         if the given pointer is NULL,
 */
uint8_t WizeApp_GetFwInfo(admin_ann_fw_info_t *pFwAnnInfo, uint8_t *rssi)
{
	admin_cmd_anndownload_t *pAnn = (admin_cmd_anndownload_t*)(sAdmCtx.aRecvBuff);

	if (pFwAnnInfo)
	{
		// If RSP has already been sent
		if (sAdmCtx.sCmdMsg.u16Id == sAdmCtx.sRspMsg.u16Id)
		{
			if ( ((admin_rsp_t*)sAdmCtx.aSendBuff)->L7ErrorCode != ADM_NONE )
			{
				return 0;
			}
		}

		_adm_ann_to_fw_info_(pFwAnnInfo, pAnn);

		if ( (pAnn->L7DayRepeat & 0xF0) == 0x80)
		{
			pFwAnnInfo->u32Type = (uint32_t)UPD_TYPE_EXTERNAL;
		}
		else
		{
			pFwAnnInfo->u32Type = (uint32_t)UPD_TYPE_INTERNAL;
		}

		if (rssi)
		{
			*rssi = sAdmCtx.sCmdMsg.u8Rssi;
		}
	}
	return sizeof(admin_ann_fw_info_t);
}

/*!
 * @brief This function get the FW info type
 *
 * @retval return update_type_e::UPD_TYPE_INTERNAL
 *         return update_type_e::UPD_TYPE_EXTERNAL
 *         return -1 otherwise
 */
int32_t WizeApp_GetFwInfoType(void)
{
	register admin_cmd_anndownload_t *pAnn = (admin_cmd_anndownload_t*)(sAdmCtx.aRecvBuff);
	if (pAnn->L7CommandId == ADM_ANNDOWNLOAD)
	{
		if ( (pAnn->L7DayRepeat & 0xF0) == 0x80)
		{
			return (int32_t)UPD_TYPE_EXTERNAL;
		}
		else
		{
			return (int32_t)UPD_TYPE_INTERNAL;
		}
	}
	return -1;
}

/******************************************************************************/

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
			if ( sys_flag_wait(&ulEvent, WIZE_APP_ITF_TMO_EVT) == 0 )
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


/******************************************************************************/
/******************************************************************************/
/*!
 * @static
 * @brief This function convert an admin_cmd_anndownload_t to admin_ann_fw_info_t structure
 *
 * @param[out] pFwAnnInfo Pointer on output admin_ann_fw_info_t struct
 * @param[in]  pAnn       Pointer on input admin_cmd_anndownload_t struct
 *
 */
static void _adm_ann_to_fw_info_(admin_ann_fw_info_t *pFwAnnInfo, admin_cmd_anndownload_t *pAnn)
{
	pFwAnnInfo->u32DwnId    = (uint32_t)( (pAnn->L7DwnId[0] << 16) | (pAnn->L7DwnId[1] << 8) | (pAnn->L7DwnId[2]) );
	pFwAnnInfo->u16SwVerIni = __ntohs( *(uint16_t*)(pAnn->L7SwVersionIni) );
	pFwAnnInfo->u16SwVerTgt = __ntohs( *(uint16_t*)(pAnn->L7SwVersionTarget) );
	pFwAnnInfo->u16MField   = *(uint16_t*)(pAnn->L7MField);
	pFwAnnInfo->u16DcHwId   = __ntohs( *(uint16_t*)(pAnn->L7DcHwId) );
	pFwAnnInfo->u16BlkCnt   = __ntohs( *(uint16_t*)(pAnn->L7BlocksCount) );
	//pFwAnnInfo->u32HashSW = __ntohl( *(uint32_t*)(pAnn->L7HashSW) );
	pFwAnnInfo->u32HashSW   = ( *(uint32_t*)(pAnn->L7HashSW) );
	// ---
	pFwAnnInfo->u8DayRepeat = pAnn->L7DayRepeat & 0x0F;
	pFwAnnInfo->u8DeltaSec  = pAnn->L7DeltaSec;
	pFwAnnInfo->u32DaysProg = __ntohl( *(uint32_t*)(pAnn->L7DaysProg) );
}

/******************************************************************************/

/*
static admin_ann_fw_info_t _fw_info_;
uint8_t WizeApp_AnnCheckFwInfo(admin_cmd_anndownload_t *pAnn)
{
	uint8_t eErrCode;
	uint8_t eErrParam;
	_adm_ann_to_fw_info(&_fw_info_, pAnn);
	if ( (pAnn->L7DayRepeat & 0xF0) == 0x80)
	{
		_fw_info_.u32Type = (uint32_t)UPD_TYPE_EXTERNAL;
	}
	else
	{
		_fw_info_.u32Type = (uint32_t)UPD_TYPE_INTERNAL;
		eErrCode = AdmInt_AnnCheckIntFW(&sFwAnnInfo, &eErrParam);
		WizeApp_AnnReady(eErrCode, eErrParam);
	}
	return 0;
}

void AdmInt_PostAnndownload(net_msg_t *pReqMsg)
{
	// if this function is called, it means that ANN is valid (no error)
	(void)pReqMsg;

	// start the update session
	if (Update_Open(_fw_info_))
	{
		// Error
		return LO_DWN_ERR_UNK;
	}
	//

}
*/
/*
void _xxx(void)
{
	time_evt_t sTimeEvt;
	uint32_t ulEvent;
	uint8_t bComIsStarted = 0;
	uint8_t bTimeEvtIsStarted = 0;

	uint32_t u32Tmo;
	int16_t i16DeltaMs;

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

	BSP_Uart_Receive(UART_ID_COM, atciCmdData->pComRxBuf->data, (uint16_t)AT_CMD_BUF_LEN);
	bComIsStarted = 1;

	status = Exec_ATADMANN_Notify(atciCmdData);

	TimeEvt_TimerStart(&sTimeEvt, u32Tmo, i16DeltaMs, (uint32_t)TMO_EVT);
	bTimeEvtIsStarted = 1;
	do
	{
		if ( sys_flag_wait(&ulEvent, WIZE_APP_ITF_TMO_EVT) == 0)
		{
			// timeout due to (seems) no activity on session
			return ATCI_ERR_UNK;
		}

		if (ulEvent & COM_MSK)
		{
			Atci_Com(atciCmdData, ulEvent);
			if (atciCmdData->cmdCode == UNS_ATADMANN )
			{
				TimeEvt_TimerStop( &sTimeEvt);
				bTimeEvtIsStarted = 0;
				bComIsStarted = 0;

				Atci_Cmd_Param_Init(atciCmdData);
				status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);

				ITF_OnDwnAnnRet(*(atciCmdData->params[0].data));

				eErrCode = _get_adm_err_code_(u8Err, &eErrParam);
				WizeApp_AnnReady(eErrCode, eErrParam);
				break;
			}
			else
			{
				Atci_AckNack(ATCI_ERR_CMD_FORBIDDEN);
				BSP_Uart_Receive(UART_ID_COM, atciCmdData->pComRxBuf->data, (uint16_t)AT_CMD_BUF_LEN);
				bComIsStarted = 1;
			}
		}

		if (ulEvent & TMO_EVT)
		{
			BSP_Uart_AbortReceive(UART_ID_COM);
			break;
		}

	} while ( !(ulEvent) );

}
*/
/*
uint8_t WizeApp_AnnCheckFwInfo(admin_cmd_anndownload_t *pAnn)
{
	(void)pAnn;
	admin_ann_fw_info_t sFwAnnInfo;
	uint8_t eErrCode;
	uint8_t eErrParam;

	if (WizeApp_GetFwInfo(&sFwAnnInfo, NULL))
	{
		if (sFwAnnInfo.u32Type == (uint32_t)UPD_TYPE_INTERNAL)
		{
			eErrCode = AdmInt_AnnCheckIntFW(&sFwAnnInfo, &eErrParam);
			WizeApp_AnnReady(eErrCode, eErrParam);
		}
	}

	return 0;
}
*/
/*
void WizeApi_OnTimeFlag(uint32_t u32Flg)
{
	_uns_as_time_(u32Flg);
}
*/
/*
uint8_t WizeApp_OnTimeEvt(uint32_t u32Evt)
{
	uint32_t ret;
	// Day passed occurs
	if (u32Evt & TIME_FLG_DAY_PASSED)
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
		if( Update_IsReady() )
		{
			// Param_Access(DATEHOUR_LAST_UPDATE, tmp, 1);
			// Param_Access(VERS_HW_TRX, tmp, 0);
			// Param_Access(VERS_FW_TRX, tmp, 1);
			BSP_Boot_Reboot(0);
		}
	}
	return 0;
}
*/


/******************************************************************************/

#ifdef __cplusplus
}
#endif

/*! @} */
