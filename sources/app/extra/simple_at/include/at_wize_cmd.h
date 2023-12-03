/**
  * @file: at_wize_cmd.h
  * @brief: This file group some AT command.
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
  * @par 1.0.0 : 2023/07/10 [GBI]
  * Initial version
  *
  *
  */

/*!
 *  @addtogroup atci
 *  @ingroup app
 *  @{
 */

#ifndef _AT_WIZE_CMD_H_
#define _AT_WIZE_CMD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "atci.h"

/******************************************************************************/

/*!
 * @brief		Execute ATSEND command (Send a Wize message)
 *
 * @details		Command format: "ATSEND=\<l6app\>,\<l7msg\>"
 * @parblock
 * @li l6app is the layer 6 application code used to give for witch application are the L7 data (decimal or hexadecimal 8bits integer)
 * @li l7msg is the layer 7 message to send (array in hexadecimal format, maximum length is 102 bytes for PRES-EXCHANGE L6 frames)
 * @endparblock
 *
 * Maybe 2 types of responses:
 * @parblock
 * @li an APP-ADMIN write command was received and processed by the on-board Wize stack message: "+ATADMWRITE :\<paramid\>,\<paramvalue\>,\<rssi\>"
 * @li a response was received in response of the Wize message, which can’t be managed by the on-board Wize stack (other application layer than APP-ADMIN) message: "+ATRCV:\<L6App\>,\<l7resp\>,\<rssi\>"
 * @endparblock
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_ATSEND_Cmd(atci_cmd_t *atciCmdData);

/*!
 * @brief		Execute ATADMWRITE notify
 *
 * @details
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_ATADMWRITE_Notify(atci_cmd_t *atciCmdData);

/*!
 * @brief		Execute ATADMANN notify
 *
 * @details
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_ATADMANN_Notify(atci_cmd_t *atciCmdData);

/*!
 * @brief		Execute ATADMDATA notify
 *
 * @details
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_ATADMDATA_Notify(atci_cmd_t *atciCmdData);

/*!
 * @brief		Execute ATBLK notify
 *
 * @details
 *
 * +ATBLK=$\<session_id\>,$\<block_id\>,$\<block\>,$\<auth\>
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
atci_error_e Exec_ATBLK_Notify(atci_cmd_t *atciCmdData);

/******************************************************************************/

/*!
 * @brief		Execute ATPING command (Send an INSTPING request)
 *
 * @details 	Command format: "ATPING"
 * 				No specific response (read dedicated registers with ATPARAM command)
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_ATPING_Cmd(atci_cmd_t *atciCmdData);

#ifdef __cplusplus
}
#endif

#endif /* _AT_WIZE_CMD_H_ */

/*! @} */
