/**
  * @file: at_access_param.h
  * @brief: // TODO This file ...
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
  * 1.0.0 : 2023/07/10[GBI]
  * Initial version
  *
  *
  */

/*!
 *  @addtogroup atci
 *  @ingroup app
 *  @{
 */

#ifndef AT_ACCESS_PARAM_H_
#define AT_ACCESS_PARAM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "atci.h"

/*!
 * @brief		Execute ATI command (Queries the identification of the module)
 *
 * @details		Command format: "ATI".
 *
 * 	Response format: "+ATI :"name",<manufacturer>,<model>,<hw version>,<major sw version>,<minor sw version>"
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_t Exec_ATI_Cmd(atci_cmd_t *atciCmdData);

/*!
 * @brief		Execute ATPARAM command (Modify/read the value of a Wize LAN parameter)
 *
 * @details		This command may be a read or a write command:
 *
 * @parblock
 * @li "ATPARAM?" : read all registers
 * @li "ATPARAM=<address>?" : read register of address "address"
 * @li "ATPARAM=<address>,<value>" : write "value" to register of address "address" (1 byte)
 * @endparblock
 *
 * Read response format:
 * @parblock
 * "+ATPARAM:<address>,<value>"
 * (this response is send for each register in read all register mode)
 * @li address : is the register address (decimal or hexadecimal 8 bits number)
 * @li value : may be a 8, 16 or 32 bits integer (decimal or hexadecimal format may be used) or an array (hexadecimal format must be used) -> refer to register list in specifications
 * @endparblock
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 * 					- params[0] is used for the register address witch is always a 8 bits integer
 * 					- params[1] is used for the register value witch can be a 8, 16 or 32 bits integer or a bytes array
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_t Exec_ATPARAM_Cmd(atci_cmd_t *atciCmdData);

/*!
 * @brief		Execute ATIDENT command (Modify/read the value of M-field and A-field)
 *
 * @details		This command may be a read or a write command:
 *
 * @parblock
 * @li "ATIDENT=<M-field>,<A-field>" : write M-field and A-field
 * @li "ATIDENT?" : read M-field and A-field
 * 		- <M-field> is 2 bytes array (must be in hex format)
 * 		- <A-field> is 6 bytes array (must be in hex format)
 *
 * Response to a read command:
 * @parblock
 * @li "+ATIDENT:<M-field>,<A-field>"
 * @endparblock
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_t Exec_ATIDENT_Cmd(atci_cmd_t *atciCmdData);

#ifdef __cplusplus
}
#endif

#endif /* AT_ACCESS_PARAM_H_ */

/*! @} */
