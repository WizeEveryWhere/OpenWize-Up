/**
  * @file update.h
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
#ifndef UPDATE_H_
#define UPDATE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "wize_app.h"

/******************************************************************************/

#ifndef UPDATE_TMO_EVT
	#define UPDATE_TMO_EVT 0xFFFFFFFF // in cycle
#endif

#ifndef UPDATE_TMO_LO
	#define UPDATE_TMO_LO 300000 // in ms
#endif

#define UPDATE_REQ_MSK ( ~(SES_FLG_SES_MSK | SES_FLG_SENDRECV_MSK) )
#define UPDATE_REQ_OFFSET 8
#define UPDATE_REQ_BITS 12

#define UPDATE_REQ_START 1
#define UPDATE_REQ_STOP  2
#define UPDATE_REQ_FINALIZE 3
#define UPDATE_REQ_BLK 4

#define UPDATE_REQ(update_req) ( (update_req <<  UPDATE_REQ_OFFSET) & UPDATE_REQ_MSK )

/******************************************************************************/
typedef enum
{
	UPD_TYPE_INTERNAL  = 0,
	UPD_TYPE_EXTERNAL  = 1,
	UPD_TYPE_LOCAL     = 2,
	// ---
	UPD_TYPE_NB,
} update_type_e;

typedef enum
{
	UPD_PEND_NONE      = 0x00,
	UPD_PEND_INTERNAL  = 0x01,
	UPD_PEND_EXTERNAL  = 0x02,
	UPD_PEND_LOCAL     = 0x03,
	// ---
	UPD_PEND_FORBIDDEN = 0x04,
} pend_update_e;

typedef enum
{
	UPD_STATUS_UNK          = 0x00,
	UPD_STATUS_SES_FAILED   = 0x01,
	UPD_STATUS_STORE_FAILED = 0x02,
	// ---
	UPD_STATUS_INPROGRESS   = 0x04,
	// ---
	UPD_STATUS_INCOMPLETE   = 0x05,
	UPD_STATUS_CORRUPTED    = 0x06,
	UPD_STATUS_VALID        = 0x07,
	// ---
	UPD_STATUS_READY        = 0x08,
	// ---
} update_status_e;

struct update_itf_s
{
	uint8_t (*pfCheckAnnFW)(admin_ann_fw_info_t *pFwInfo, uint8_t *u8ErrorParam);
	void (*pfNotifyAnnFW)(uint8_t eErrCode, uint8_t u8ErrorParam);
	uint8_t (*pfOnBlkRecv)(uint16_t u16Id, const uint8_t *pData);


	uint8_t (*pfInit)(admin_ann_fw_info_t *pFwInfo, uint8_t *u8ErrorParam);
	uint8_t (*pfProcess)(uint16_t u16Id, const uint8_t *pData);
	uint8_t (*pfFini)(admin_ann_fw_info_t *pFwInfo, uint8_t *u8ErrorParam);
};

struct update_ctx_s
{
	void            *hTask;
	void            *hLock;         /*!< Pointer on lock */
	pend_update_e   ePendUpdate;
	update_status_e eUpdateStatus;
	uint8_t         eErrCode;
	uint8_t         eErrParam;

	time_evt_t      sTimeEvt; /*!< Window timer */
	uint32_t        u32Tmo;

	uint8_t         eType;
	struct update_itf_s *pUpdateItf;

	admin_ann_fw_info_t sFwAnnInfo;
} ;

/******************************************************************************/
void Update_Setup(void);
//void Update_Task(void const * argument);

/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* UPDATE_H_ */
