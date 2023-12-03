/**
  * @file: wize_app_itf.c
  * @brief: // TODO This file ...
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
  *
  * @par Revision history
  * ----------------
  * @par 1.0.0 : 2023/08/28[GBI]
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

//#include "phy_layer_private.h"

/******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
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

/*!
 * @}
 * @endcond
 */

/******************************************************************************/
/******************************************************************************/

/*!
 * @brief This is a "weak" function intend to implement the treatment of one
 *        FW block. It will be called by "WizeApp_Common" when a download block
 *        will be received. Its content is user defined.
 *
 * @param [in] u16Id Identify the block number
 * @param [in] pData Pointer on the data block of fixed 210 bytes size.
 *
 * @return 0
 *
 */
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

/******************************************************************************/
/*!
 * @brief This function get the last ADM command.
 *
 * @param [out] pData Pointer on output buffer
 * @param [out] rssi  Pointer on output RSSI
 *
 * @retval size of ADM command message
 * @retval 0 if an error RSP was already or if the given pointer is NULL
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
 * @param [out] pFwAnnInfo Pointer on output buffer
 * @param [out] rssi       Pointer on output RSSI
 *
 * @retval size of FW info
 * @retval 0 if an error RSP was already sent or if the given pointer is NULL,
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
 * @retval update_type_e::UPD_TYPE_INTERNAL
 * @retval update_type_e::UPD_TYPE_EXTERNAL
 * @retval -1 otherwise
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
/******************************************************************************/

#ifdef __cplusplus
}
#endif

/*! @} */
