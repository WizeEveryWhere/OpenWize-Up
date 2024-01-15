/**
  * @file at_key_cmd.h
  * @brief This file group some AT command.
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
  * @par 1.0.0 : 2023/05/03 [GBI]
  * Initial version
  *
  */

/*!
 *  @addtogroup atci
 *  @ingroup app
 *  @{
 */

#ifndef _AT_KEY_CMD_H_
#define _AT_KEY_CMD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "atci.h"

/******************************************************************************/

/*!
 * @cond INTERNAL
 * @{
 */
#define ATKMAC_KEY_LEN			16
#define ATKENC_KEY_LEN			16
/*!
 * @}
 * @endcond
 */

/******************************************************************************/

/*!
 * @brief		Execute ATKEY command (Modify the value of one key)
 *
 * @details		ATKEY command is a write only command:
 *
 * "ATKEY=<id>,<key>"
 * @parblock
 * @li id is the key number (decimal or hexadecimal 1 byte number)
 * @li key is a 16 or 32 bytes key (32 bytes KEY but only the 16 1st bytes uses) and must be written in hexadecimal format (with "$" char)
 * @endparblock
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_CMD_ATKEY(atci_cmd_t *atciCmdData);

#ifdef __cplusplus
}
#endif

#endif /* _AT_KEY_CMD_H_ */

/*! @} */
