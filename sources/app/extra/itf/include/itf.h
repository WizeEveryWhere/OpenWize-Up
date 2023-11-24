/**
  * @file itf.h
  * @brief This file define the interface function prototype's.
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

/*!
 * @cond INTERNAL
 * @{
 */
#ifndef NB_BLK_STORE_MAX
	#define NB_BLK_STORE_MAX 4
#endif

/*!
 * @brief This struct hold a FW block
 */
typedef struct __attribute__((packed)) blk_s
{ //
	uint16_t u16Id;         /**< Block id */
	uint8_t  aData[BLK_SZ]; /**< Block data */
} blk_t;

/*!
 * @brief This struct hold the temporary FW buffer
 */
typedef struct
{
	uint8_t u8ReadyBlkCnt; /**< Number of pending block */
	uint8_t idx_write;     /**< Current index to write */
	uint8_t idx_read;      /**< Current index to read */
	uint8_t fw_blk[NB_BLK_STORE_MAX][sizeof(blk_t)]; /**< Buffer to store the FW block */
} fw_buffer_t;

/*!
 * @brief This define the interface context
 */
struct itf_ctx_s
{
	void *hTask;            /**< Handler on the task */
	void *hLock;            /**< Handler on the lock */
	uint32_t u32DwnId;      /**< Current session id  */
	uint8_t u8Err;          /**< Last error  */
	uint8_t u8KeyId;        /**< Key id to used for encryption and authentication */
	uint8_t u8MissedBlkCnt; /**< Number of missed block due to a full buffer */
	fw_buffer_t sFwBuffer;  /**< Temporary FW buffer */
};

/*!
 * @}
 * @endcond
 */
/******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */
typedef enum
{
	UNK_SRC_ID,
	SELF_SRC_ID,
	COM_SRC_ID,
	NOTIFY_SRC_ID,
	TIME_SRC_ID,
	SESSION_SRC_ID
} evt_src_id;

enum
{
	NONE_NOTIFY,
	BOOT_NOTIFY, //= NB_AT_UNS,

	//BOOT_NOTIFY,
	SESSION_NOTIFY,
	TIME_NOTIFY,
};

/*!
 * @}
 * @endcond
 */

/******************************************************************************/
/******************************************************************************/

/*!
 * @brief This function setup the interface
 *
 */
void ITF_Setup(void);

/*!
 * @brief This function initialize interface for a FW download to local interface .
 *
 * @retval  local_dwn_err_code_e::LO_DWN_ERR_NONE If success or there is no pending FW download.
 *          local_dwn_err_code_e::LO_DWN_ERR_UNK  Otherwise
 *
 */
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
/******************************************************************************/

/*!
 * @brief This function build an announce to be send on local interface
 *
 * @param[out] pAnn       Pointer on resulting announce
 * @param[in]  pFwAnnInfo Pointer on announce to build
 *
 */
void ITF_FwInfoToLocalAnn(local_cmd_anndownload_t *pAnn, admin_ann_fw_info_t *pFwAnnInfo);

/*!
 * @brief This function build a local interface FW block frame from internal store.
 *
 * @param[in] pFrame Pointer frame containing the request.
 *
 * @retval  local_dwn_err_code_e::LO_DWN_ERR_NONE If the build success.
 *          local_dwn_err_code_e::LO_DWN_ERR_UNK  Otherwise
 *
 */
uint8_t ITF_LocalBlkSend(local_cmd_writeblock_t *pFrame);

/*!
 * @brief This function convert error code from local interface to admin. L7 layer ones
 *
 * @param[in]  eErrCode     Local interface error code.
 * @param[out] u8ErrorParam Parameter number on which error occurs (if any).
 *
 * @return The converted error code as describe in admin_ann_err_code_e.
 */
uint8_t ITF_GetAdmErrCode(uint8_t eErrCode, uint8_t *u8ErrorParam);

/******************************************************************************/
/******************************************************************************/

/*!
 * @brief This function extract an announce from local interface
 *
 * @param[out] pFwAnnInfo Pointer on resulting announce
 * @param[in]  pAnn       Pointer on announce to extract
 *
 */
void ITF_LocalAnnToFwInfo(admin_ann_fw_info_t *pFwAnnInfo, local_cmd_anndownload_t *pAnn);

/*!
 * @brief This function request to start a new FW update session from local interface.
 *
 * @param[in] pAnn    Pointer frame containing the request.
 * @param[in] u8KeyId The id of the key which will be used for FW block encryption and authentication.
 *
 * @return Error code from local_dwn_err_code_e
 *
 */
uint8_t ITF_LocalAnnRecv(local_cmd_anndownload_t *pAnn, uint8_t u8KeyId);

/*!
 * @brief This function request to extract and store a FW block from local interface.
 *
 * @param[in] pFrame Pointer frame containing the request.
 *
 * @retval local_dwn_err_code_e::LO_DWN_ERR_NONE If success
 *         local_dwn_err_code_e::LO_DWN_ERR_SES_ID If the given session id doesn't match
 *         local_dwn_err_code_e::LO_DWN_ERR_AUTH If authentication failed
 *         local_dwn_err_code_e::LO_DWN_ERR_BLK_ID If the block write failed
 *         local_dwn_err_code_e::LO_DWN_ERR_UNK Otherwise
 *
 */
uint8_t ITF_LocalBlkRecv(local_cmd_writeblock_t *pFrame);

/*!
 * @brief This function request to finalize a FW update from local interface.
 *
 * @param[in] pFrame Pointer frame containing the request.
 *
 * @retval local_dwn_err_code_e::LO_DWN_ERR_NONE If success
 *         local_dwn_err_code_e::LO_DWN_ERR_SES_ID If the given session id doesn't match
 *         local_dwn_err_code_e::LO_DWN_ERR_CORRUPTED If the downloaded FW is corrupted
 *         local_dwn_err_code_e::LO_DWN_ERR_BLK_CNT If the downloaded FW is incomplete
 *         local_dwn_err_code_e::LO_DWN_ERR_WRITE If the final write failed
 *
 */
uint8_t ITF_LocalUpdateReq(local_cmd_update_t *pFrame);

/*!
 * @brief This function convert error code from admin. L7 layer to local ones
 *
 * @param[in] eErrCode     L7 admin. layer error code.
 * @param[in] u8ErrorParam Parameter number on which error occurs (if any).
 *
 * @return The converted error code as describe in local_dwn_err_code_e.
 */
uint8_t ITF_GetLocalErrCode(uint8_t eErrCode, uint8_t u8ErrorParam);

/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _ITF_H_ */

/*! @} */
