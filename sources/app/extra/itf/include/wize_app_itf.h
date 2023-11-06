/**
  * @file: wize_app_itf.h
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
  * 1.0.0 : 2023/08/28[GBI]
  * Initial version
  *
  *
  */

/*!
 *  @addtogroup itf
 *  @ingroup app
 *  @{
 */

#ifndef _ITF_WIZE_APP_H_
#define _ITF_WIZE_APP_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "app_layer.h"
#include "ses_common.h"

#ifndef WIZE_APP_ITF_TMO_EVT
	#define WIZE_APP_ITF_TMO_EVT 0xFFFFFFFF
#endif

uint8_t WizeApp_AnnCheckFwInfo(admin_cmd_anndownload_t *pAnn);
uint8_t WizeApp_OnDwnBlkRecv(uint16_t u16Id, const uint8_t *pData);


int32_t WizeApp_WaitSesComplete(ses_type_t eSesId);
uint8_t WizeApp_GetAdmCmd(uint8_t *pData, uint8_t *rssi);
uint8_t WizeApp_GetFwInfo(admin_ann_fw_info_t *pFwAnnInfo, uint8_t *rssi);
int32_t WizeApp_GetFwInfoType(void);

//uint8_t WizeApp_OnTimeEvt(uint32_t u32Evt);

#ifdef __cplusplus
}
#endif
#endif /* _ITF_WIZE_APP_H_ */

/*! @} */
