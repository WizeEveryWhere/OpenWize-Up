/**
  * @file adf7030-1_phy_log.h
  * @brief This file declare function to get log error messages
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
  * @par 1.0.0 : 2020/04/23 [GBI]
  * Initial version
  *
  *
  */

/*!
 * @addtogroup adf7030-1_phy
 * @ingroup device
 * @{
 *
 */
#ifndef _ADF7030_1_PHY_LOG_H_
#define _ADF7030_1_PHY_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "adf7030-1_phy.h"

/*!
   Define max number of logged Host Radio driver instance error.\n
*/
#define NUM_INSTANCE_ERROR 19

/*!
   Define max number of logged PHY Radio error.\n
*/
#define NUM_PHY_ERROR 30


void CheckReturn(adf7030_1_device_t* const pDevice);
const char* getErrMsg(adf7030_1_device_t* const pDevice );


#ifdef __cplusplus
}
#endif
#endif /* _ADF7030_1PHY_LOG_H_ */

/*! @} */
