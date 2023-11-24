/**
  * @file: update_area.h
  * @brief: This file define the functions to treat the partition header
  *         and/or the image FW.
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
  * 1.0.0 : 2023/07/11[GBI]
  * Initial version
  *
  *
  */

/*!
 * @addtogroup update
 * @ingroup app
 * @{
 */

#ifndef _UPDATE_AREA_H_
#define _UPDATE_AREA_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "update.h"

#include "img.h"
/******************************************************************************/
/*!
 * @brief This function setup the update area
 *
 * @return see the update_status_e enum
 */
update_status_e UpdateArea_Setup(void);

/*!
 * @brief This function initialize the update area
 *
 * @param [in] eType     The type of update (see update_type_e)
 * @param [in] u16BlkCnt The expected number of update block
 *
 * @return see the update_status_e enum
 */
update_status_e UpdateArea_Initialize(uint8_t eType, uint16_t u16BlkCnt);

/*!
 * @brief This function store an update FW block
 *
 * @param [in] eType The type of update (see update_type_e)
 * @param [in] u16Id The update block id
 * @param [in] pData The update block data to store
 *
 * @return see the update_status_e enum
 */
update_status_e UpdateArea_Proceed(uint8_t eType, uint16_t u16Id, const uint8_t *pData);

/*!
 * @brief This function finalize the update area
 *
 * @param [in] eType     The type of update (see update_type_e)
 * @param [in] u32HashSW The hash number to validate the FW image
 * @param [in] img_sz    The size of FW image
 *
 * @return see the update_status_e enum
 */
update_status_e UpdateArea_Finalize(uint8_t eType, uint32_t u32HashSW, uint32_t img_sz);

/*!
 * @brief This function validate the FW image
 *
 * @param [in] u32HashSW The hash number to validate the FW image
 *
 * @retval update_status_e::UPD_STATUS_VALID If image is valid
 *         update_status_e::UPD_STATUS_INPROGRESS If a FW block is missing
 *         update_status_e::UPD_STATUS_CORRUPTED If image validation failed
 */
update_status_e UpdateArea_CheckImg(uint32_t u32HashSW);

/*!
 * @brief This function write the partition header
 *
 * @param [in] p_img_info Pointer on internal partition image info
 *
 * @retval update_status_e::UPD_STATUS_READY If success
 *         update_status_e::UPD_STATUS_STORE_FAILED If fail to write header
 */
update_status_e UpdateArea_WriteHeader(struct __img_info_s *p_img_info);

/*!
 * @brief This function set the boot request for an Update
 *
 */
void UpdateArea_SetBootUpdate(void);

/*!
 * @brief This function set the boot request for an Roll-Back
 *
 */
void UpdateArea_SetBootBack(void);

/*!
 * @brief This function set the application as "bootable"
 *
 */
void UpdateArea_SetBootable(void);

/******************************************************************************/

#ifdef __cplusplus
}
#endif
#endif /* _UPDATE_AREA_H_ */

/*! @} */
