/**
  * @file update.h
  * @brief This file define the functions to treat the update session
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

/*!
 * @addtogroup update
 * @ingroup app
 * @{
 */

#ifndef _UPDATE_H_
#define _UPDATE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "wize_app.h"

/******************************************************************************/
/*!
 * @brief This enum define the type of update
 */
typedef enum
{
	UPD_TYPE_INTERNAL  = 0, /**< Remote Internal update  */
	UPD_TYPE_EXTERNAL  = 1, /**< Remote external update  */
	UPD_TYPE_LOCAL     = 2, /**< Local Internal update  */
	// ---
	UPD_TYPE_NB,
} update_type_e;

/*!
 * @brief This enum define the current update state
 */
typedef enum
{
	UPD_PEND_NONE      = 0x00, /**< None */
	UPD_PEND_INTERNAL  = 0x01, /**< An internal update is currently processed */
	UPD_PEND_EXTERNAL  = 0x02, /**< An external update is currently processed */
	UPD_PEND_LOCAL     = 0x03, /**< A local update is currently processed */
	// ---
	UPD_PEND_FORBIDDEN = 0x04, /**< Update is forbidden due to an internal error */
} pend_update_e;

/*!
 * @brief This enum define the return status
 */
typedef enum
{
	UPD_STATUS_UNK          = 0x00, /**< Status is unknown */
	UPD_STATUS_SES_FAILED   = 0x01, /**< The last session failed */
	UPD_STATUS_STORE_FAILED = 0x02, /**< Update was not able to write into memory */
	// ---
	UPD_STATUS_INPROGRESS   = 0x04, /**< Update is progressing */
	// ---
	UPD_STATUS_INCOMPLETE   = 0x05, /**< The FW image is incomplete */
	UPD_STATUS_CORRUPTED    = 0x06, /**< The FW image is corrupted. Hash is mismatched */
	UPD_STATUS_VALID        = 0x07, /**< The FW image is valid */
	// ---
	UPD_STATUS_READY        = 0x08, /**< The FW partition is ready */
	// ---
} update_status_e;

/*!
 * @brief This hold the update context
 */
struct update_ctx_s
{
	void            *hTask;        /**< Handler on the task  */
	void            *hLock;        /**< Hnadler on lock */
	pend_update_e   ePendUpdate;   /**< The current state  */
	update_status_e eUpdateStatus; /**< The last status  */
	uint32_t        u32Tmo;        /**< The update loop timeout */
} ;

/******************************************************************************/

/*!
 * @brief This function setup the update module
 *
 */
void Update_Setup(void);

/*!
 * @brief This function open an update session
 *
 * @param [in] sFwInfo The FW info required to process this update (see admin_ann_fw_info_t)
 *
 * @return 0 if success, -1 otherwise
 */
int32_t Update_Open(admin_ann_fw_info_t sFwAnnInfo);

/*!
 * @brief This function close (force) an update session
 *
 * @return 0 if success, -1 otherwise
 */
int32_t Update_Close(void);

/*!
 * @brief This function store a piece of FW
 *
 * @param [in] u16Id Id of the block
 * @param [in] pData Pointer on data to store
 *
 * @return 0 if success, -1 otherwise
 */
int32_t Update_Store(uint16_t u16Id, const uint8_t *pData);

/*!
 * @brief This function request to finalize the current update session
 *
 * @return 0 if success, -1 otherwise
 */
int32_t Update_Finalize(void);

/*!
 * @brief This function check if the FW in the current update is ready
 *
 * @return 1 if ready, 0 otherwise
 */
int32_t Update_IsReady(void);

/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _UPDATE_H_ */

/*! @} */
