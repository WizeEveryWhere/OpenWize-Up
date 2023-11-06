/**
  * @file itf.h
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
  * @par 1.0.0 : 2023/04/26 [GBI]
  * Initial version
  *
  */

/*!
 *  @addtogroup itf
 *  @ingroup app
 *  @{
 */

#ifndef _ITF_H_
#define _ITF_H_

#include <stdint.h>
#include "local_frm.h"

#include "app_layer.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NB_BLK_STORE_MAX
	#define NB_BLK_STORE_MAX 4
#endif

/*!
 * @brief This
 */
typedef struct __attribute__((packed)) blk_s
{ //
	uint16_t u16Id;        /**<   */
	uint8_t aData[BLK_SZ]; /**<   */
} blk_t;

/*!
 * @brief This
 */
typedef struct
{
	uint8_t u8ReadyBlkCnt; /**<   */
	uint8_t idx_write; /**<   */
	uint8_t idx_read;  /**<   */
	uint8_t fw_blk[NB_BLK_STORE_MAX][sizeof(blk_t)]; /**<   */
} fw_buffer_t;

/*!
 * @brief This
 */
struct itf_ctx_s
{
	void *hTask;                /**<   */
	void *hLock;
	uint32_t u32DwnId;          /**<   */
	uint8_t u8Err;              /**<   */
	uint8_t u8KeyId;            /**<   */

	uint8_t u8MissedBlkCnt;     /**<   */
	fw_buffer_t sFwBuffer;      /**<   */
	//admin_ann_fw_info_t sFwAnnInfo;  /**<   */
};

/******************************************************************************/
typedef enum
{
	UNK_SRC_ID,
	SELF_SRC_ID,
	COM_SRC_ID,
	NOTIFY_SRC_ID,
	TIME_SRC_ID,
	SESSION_SRC_ID
} evt_src_id;

/******************************************************************************/



/*!
 * @brief This function convert an admin_cmd_anndownload_t structure to
 * admin_ann_fw_info_t one
 *
 * @param[out] pFwAnnInfo Pointer to output structure
 * @param[in]  pAnn       Pointer to input structure
 *
 */
//void ITF_AdmAnnToFwInfo(admin_ann_fw_info_t *pFwAnnInfo, admin_cmd_anndownload_t *pAnn);


//uint8_t ITF_OnDwnAnnToSend(local_cmd_anndownload_t *pAnn);


/*!
 * @brief This function treat the incoming local update announcement
 *
 * @param[in] pAnn    Pointer local ANN frame
 * @param[in] u8KeyId The key id to use to decrypt and authenticate
 *
 * @retval  (see local_dwn_err_code_e)
 *
 */
//uint8_t ITF_OnDwnAnnRecv(local_cmd_anndownload_t *pAnn, uint8_t u8KeyId);



/*!
 * @brief This function convert an local_cmd_anndownload_t structure to
 * admin_cmd_anndownload_t one
 *
 * @param[out] pLocalAnn Pointer to output structure
 * @param[in]  pAnn      Pointer to input structure
 *
 */
void ITF_AdmAnnToLocalAnn(local_cmd_anndownload_t *pLocalAnn, admin_cmd_anndownload_t *pAnn);


void ITF_Setup(void);

uint8_t ITF_On(void);


/*!
 * @brief This function store fw block into local buffer
 *
 * @param[in] u16Id The block id
 * @param[in] pData Pointer on the block's data
 *
 */
void ITF_StoreBlock(uint16_t u16Id, uint8_t *pData);


/******************************************************************************/

/*!
 * @brief This function convert an admin_ann_fw_info_t structure to
 * local_cmd_anndownload_t one
 *
 * @param[out] pAnn       Pointer to output structure
 * @param[in]  pFwAnnInfo Pointer to input structure
 *
 */
void ITF_FwInfoToLocalAnn(local_cmd_anndownload_t *pAnn, admin_ann_fw_info_t *pFwAnnInfo);

/*!
 * @brief This function build a local frame with current fw block
 *
 * @param[out] pFrame Pointer on output frame
 *
 * @retval  (see local_dwn_err_code_e)
 *
 */
uint8_t ITF_LocalBlkSend(local_cmd_writeblock_t *pFrame);

/*!
 * @brief  This
 *
 * @param [in]  eErrCode       (see local_dwn_err_code_e)
 * @param [out] u8ErrorParam
 *
 * @retval  (see admin_ann_err_code_e)
 *
 */
uint8_t ITF_GetAdmErrCode(uint8_t eErrCode, uint8_t *u8ErrorParam);

/******************************************************************************/
/*!
 * @brief This function convert an local_cmd_anndownload_t structure to
 * admin_ann_fw_info_t one
 *
 * @param[out] pFwAnnInfo Pointer to output structure
 * @param[in]  pAnn       Pointer to input structure
 *
 */
void ITF_LocalAnnToFwInfo(admin_ann_fw_info_t *pFwAnnInfo, local_cmd_anndownload_t *pAnn);

/*!
 * @brief This function
 *
 * @param[in] pFrame
 *
 * @retval  (see local_dwn_err_code_e)
 *
 */
uint8_t ITF_LocalAnnRecv(local_cmd_anndownload_t *pAnn, uint8_t u8KeyId);

/*!
 * @brief This function
 *
 * @param[in] pFrame
 *
 * @retval  (see local_dwn_err_code_e)
 *
 */
uint8_t ITF_LocalBlkRecv(local_cmd_writeblock_t *pFrame);

/*!
 * @brief This function
 *
 * @param[in] pFrame
 *
 * @retval  (see local_dwn_err_code_e)
 *
 */
uint8_t ITF_LocalUpdateReq(local_cmd_update_t *pFrame);

/*!
 * @brief  This
 *
 * @param [in]  eErrCode      (see admin_ann_err_code_e)
 * @param [out] u8ErrorParam
 *
 * @retval  (see local_dwn_err_code_e)
 *
 */
uint8_t ITF_GetLocalErrCode(uint8_t eErrCode, uint8_t u8ErrorParam);

/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _ITF_H_ */

/*! @} */
