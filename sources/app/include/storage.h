/**
  * @file storage.h
  * @brief This file define the storage functions
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
  * @par 1.0.0 : 2021/02/07 [GBI]
  * Initial version
  *
  *
  */

/*!
 * @addtogroup storage
 * @ingroup app
 * @{
 */
#ifndef _STORAGE_H_
#define _STORAGE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "version.h"
/******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */

#define EXT_FLAGS_DBG_MSG_EN_MSK      0x01
#define EXT_FLAGS_PHYCAL_WRITE_EN_MSK 0x20
#define EXT_FLAGS_IDENT_WRITE_EN_MSK  0x40
#define EXT_FLAGS_KEYS_WRITE_EN_MSK   0x80

/*!
 * @}
 * @endcond
 */
/******************************************************************************/

/*!
  * @brief Select area.
  * @details This is used by Storage_SetDefault and Storage_Get functions to
  * select which area is concern by the required action.
  */
typedef enum
{
	KEY_AREA_ID = 0x01, /*!< Key area only */
	SPE_AREA_ID = 0x02, /*!< Special area (device id, calibration, ...) only */
	PAR_AREA_ID = 0x04, /*!< Parameters area only */
	// ---
	ALL_AREA_ID = 0x07, /*!< All area */
	// ---
} area_id_e;


/*!
 * @brief  This initialize the storage area
 *
 * @param [in] bForce Force to defaults.
 *
 */
void Storage_Init(uint8_t bForce);

/*!
 * @brief  Set to defaults
 *
 * @param [in] eArea Select the concern area (ORed value from @link area_id_e @endlink).
 *
 */
void Storage_SetDefault(uint8_t eArea);

/*!
 * @brief  Store current into the flash memory
 *
 * @retval  0 Success
 * @retval  1 Failed
 *
 */
uint8_t Storage_Store(void);

/*!
 * @brief  Get from flash memory the current
 *
 * @param [in] eArea Select the concern area (ORed value from @link area_id_e @endlink).
 *
 * @retval  0 Success
 * @retval  1 Failed
 *
 */
uint8_t Storage_Get(uint8_t eArea);

/******************************************************************************/

#ifdef __cplusplus
}
#endif
#endif /* _STORAGE_H_ */

/*! @} */
