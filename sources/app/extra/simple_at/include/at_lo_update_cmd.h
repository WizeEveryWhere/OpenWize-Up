/**
  * @file at_lo_update_cmd.h
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

#ifndef _AT_LO_UPDATE_CMD_H_
#define _AT_LO_UPDATE_CMD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "atci.h"

/******************************************************************************/

/*!
 * @brief		Execute ATANN command (Announce for local update)
 *
 * @details
 *
 * ATANN=$\<key_id\>,$\<session_id\>,$\<announce\>
 * @parblock
 * - \<key_id\> : The key id used to encrypt and authenticate the FW blocks (1 byte).
 *              The key id 0 (unencrypted) is forbidden.
 * - \<session_id\> : The local update session identification (4 bytes)
 * - \<announce\> : The local update announcement information
 * @endparblock
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_CMD_ATANN(atci_cmd_t *atciCmdData);

/*!
 * @brief		Execute ATBLK command (Set FW block)
 *
 * @details
 *
 * ATBLK=$\<session_id\>,$\<block_id\>,$\<block\>,$\<auth\>
 * @parblock
 * - \<session_id\> : A 4 bytes identifying the update session (8 digits)
 * - \<block_id\> : A 2 bytes identifying the block number (4 digits)
 * - \<block\> : A 210 bytes hexadecimal string ciphered (420 digits)
 * - \<auth\> : A 4 bytes hash cmac authenticate the block (8 digits)
 * @endparblock
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_CMD_ATBLK(atci_cmd_t *atciCmdData);

/*!
 * @brief		Execute ATUPD command (Request to apply local update)
 *
 * @details
 *
 * ATUPD=$\<session_id\>
 * @parblock
 * - \<session_id\> : A 4 bytes identifying the update session (8 digits)
 * @endparblock
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_CMD_ATUPD(atci_cmd_t *atciCmdData);

/*!
 * @brief		Execute ATBMAP command (Get local update block bitmap representation)
 *
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_CMD_ATBMAP(atci_cmd_t *atciCmdData);

#ifdef __cplusplus
}
#endif

#endif /* _AT_LO_UPDATE_CMD_H_ */

/*! @} */
