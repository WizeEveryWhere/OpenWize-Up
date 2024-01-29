/**
  * @file: wize_app_itf.h
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

#ifndef _WIZE_APP_ITF_H_
#define _WIZE_APP_ITF_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "app_layer.h"
#include "ses_common.h"
/******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */
#ifndef WIZE_APP_ITF_TMO_EVT
	#define WIZE_APP_ITF_TMO_EVT 0xFFFFFFFF
#endif
/*!
 * @}
 * @endcond
 */

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
uint8_t WizeApp_OnDwnBlkRecv(uint16_t u16Id, const uint8_t *pData);

/*!
 * @brief This function get the last ADM command.
 *
 * @param [out] pData Pointer on output buffer
 * @param [out] rssi  Pointer on output RSSI
 *
 * @retval size of ADM command message
 * @retval 0 if an error RSP was already or if the given pointer is NULL
 */
uint8_t WizeApp_GetAdmCmd(uint8_t *pData, uint8_t *rssi);

/*!
 * @brief This function get the last FW info
 *
 * @param [out] pFwAnnInfo Pointer on output buffer
 * @param [out] rssi       Pointer on output RSSI
 *
 * @retval size of FW info
 * @retval 0 if an error RSP was already sent or if the given pointer is NULL,
 */
uint8_t WizeApp_GetFwInfo(admin_ann_fw_info_t *pFwAnnInfo, uint8_t *rssi);

/*!
 * @brief This function get the FW info type
 *
 * @retval update_type_e::UPD_TYPE_INTERNAL
 * @retval update_type_e::UPD_TYPE_EXTERNAL
 * @retval -1 otherwise
 */
int32_t WizeApp_GetFwInfoType(void);

/******************************************************************************/

#ifdef __cplusplus
}
#endif
#endif /* _WIZE_APP_ITF_H_ */

/*! @} */
