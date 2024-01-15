/**
  * @file always_on.h
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
  * @par 1.0.0 : 2023/12/06 [GBI]
  * Initial version
  *
  */
#ifndef _ALWAYS_ON_H_
#define _ALWAYS_ON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#include "ses_common.h"

/******************************************************************************/

/*!
 * @cond INTERNAL
 * @{
 */

#undef RECV_BUFFER_SZ
#define RECV_BUFFER_SZ 229 // L7: 229; L6: 13 ; L2: 13

/*!
 * @}
 * @endcond
 */

/*!
 * @brief This struct defines the custom manager internal context.
 */
struct cust_mgr_ctx_s
{
	net_msg_t sCmdMsg;                 /**< Command message content */
    uint8_t aRecvBuff[RECV_BUFFER_SZ]; /**< Buffer that hold the received command frame */
};

#ifdef __cplusplus
}
#endif

#endif /* _ALWAYS_ON_H_ */
