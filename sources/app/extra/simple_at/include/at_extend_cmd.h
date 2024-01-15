/**
  * @file at_extend_cmd.h
  * @brief This file group some AT command
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

#ifndef _AT_EXTEND_CMD_H_
#define _AT_EXTEND_CMD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "atci.h"

/*!
 * @brief		Execute ATQ command (Restore registers to their factory settings)
 *
 * @details		Command format: "ATQ".
 *
 * @param[in,out]	atciCmdData  Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_CMD_ATQ(atci_cmd_t *atciCmdData);

/*!
 * @brief		Execute ATZn command (reboot the system)
 *
 * @details		Command format: "ATZn".
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_CMD_ATZn(atci_cmd_t *atciCmdData);

/*
 * @brief		Execute AT&F command (Restore registers to their factory settings)
 *
 * @details		Command format: "AT&F".
 *
 * @param[in,out]	atciCmdData  Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_CMD_ATF(atci_cmd_t *atciCmdData);

/*!
 * @brief		Execute AT&W command (Store current registers values in flash)
 *
 * @details		Command format: "AT&W".
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_CMD_ATW(atci_cmd_t *atciCmdData);

/*!
 * @brief		Execute AT%CCLK command (get the current clock as unix epoch)
 *
 * @details		Command format: "AT%CCLK".
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_CMD_ATCCLK(atci_cmd_t *atciCmdData);

/*!
 * @brief		Execute AT%UID command (get the unique identifier)
 *
 * @details		Command format: "AT%UID".
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_CMD_ATUID(atci_cmd_t *atciCmdData);

/*!
 * @brief		Execute AT%STAT command (get the wize statistics)
 *
 * @details		Command format: "AT%STAT".
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_e Exec_CMD_ATSTAT(atci_cmd_t *atciCmdData);

/*!
 * @brief Build and send ATCI ACK
 *
 * @param[in,out]	atciCmdData  Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_ERR_NONE
 */
atci_error_e Exec_UNS_ACK(atci_cmd_t *atciCmdData);

/*!
 * @brief Build and send ATCI notification
 *
 * @param[in,out]	atciCmdData  Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_ERR_NONE
 */
atci_error_e Exec_UNS_NOTIFY(atci_cmd_t *atciCmdData);

/*!
 * @brief Set the code for a generic notification (from ulEvent)
 *
 * @param[in,out] atciCmdData  Pointer on "atci_cmd_t" structure
 * @param[in]	  ulEvent
 *
 * @retval atci_error_e::ATCI_ERR_NONE If success
 *         atci_error_e::ATCI_ERR_INTERNAL If code/event is unknown
 *
 */
atci_error_e Generic_Notify_SetCode(atci_cmd_t *atciCmdData, uint32_t ulEvent);



atci_error_e Exec_CMD_ATCAL(atci_cmd_t *atciCmdData);

#ifdef __cplusplus
}
#endif

#endif /* _AT_EXTEND_CMD_H_ */

/*! @} */
