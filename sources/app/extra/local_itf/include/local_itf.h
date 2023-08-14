/**
  * @file local_itf.h
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
  * @par 1.0.0 : 2023/04/26 [TODO: your name]
  * Initial version
  *
  */
#ifndef LOCAL_ITF_H_
#define LOCAL_ITF_H_

#include <stdint.h>
#include "local_frm.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NB_BLK_STORE_MAX
	#define NB_BLK_STORE_MAX 4
#endif

__attribute__((packed))
typedef struct  blk_s
{ //
	uint16_t u16Id;
	uint8_t aData[BLK_SZ];
} blk_t;

typedef struct
{
	uint8_t idx_write;
	uint8_t idx_read;
	uint8_t fw_blk[NB_BLK_STORE_MAX][sizeof(blk_t)];
} fw_buffer_t;

struct local_ctx_s
{
	void *hTask;
	uint32_t u32DwnId;
	uint8_t u8Err;
	uint8_t u8KeyId;

	uint8_t u8AvalableBlkCnt;
	uint8_t u8MissedBlkCnt;
	fw_buffer_t sFwBuffer;

};

/******************************************************************************/

uint8_t LocalItf_OnDwnAnnRecv(local_cmd_anndownload_t *pAnn, uint8_t u8KeyId);
void LocalItf_AnnToFwInfo(admin_ann_fw_info_t *pFwAnnInfo, local_cmd_anndownload_t *pAnn);
void LocalItf_StoreBlock(uint16_t u16Id, uint8_t *pData);
uint8_t LocalItf_OnDwnBlkToSend(local_cmd_writeblock_t *pFrame);
uint8_t LocalItf_OnDwnBlkRecv(local_cmd_writeblock_t *pFrame);

/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* LOCAL_ITF_H_ */
