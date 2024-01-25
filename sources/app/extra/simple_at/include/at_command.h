/**
  * @file: at_command.h
  * @brief: This file define the AT command global table
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

#ifndef _AT_COMMAND_H_
#define _AT_COMMAND_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "atci.h"

/*!
 * @cond INTERNAL
 * @{
 */

/*!
 * @brief This enum define the AT command code
 */
typedef enum
{
	// ----------------
	CMD_AT,
	CMD_ATI,
	CMD_ATZ,
	CMD_ATZ0,
	CMD_ATZ1,
	CMD_ATQ,
	CMD_ATF,
	CMD_ATW,
	CMD_ATPARAM,

	CMD_ATSEND,
	CMD_ATPING,
	CMD_ATLSN,
	// ----
	CMD_ATIDENT,
	CMD_ATKEY,
	CMD_ATFC,
	CMD_ATTEST,
	CMD_ATCAL,
	// ----
	CMD_ATADMANN,
	// ----
	CMD_ATANN,
	CMD_ATBLK,
	CMD_ATUPD,
	CMD_ATBMAP,
	// ----
	CMD_ATSTAT,
	CMD_ATCCLK,
	CMD_ATUID,
	// ----
	NB_AT_CMD,
	CMD_NULL = NB_AT_CMD,
	// ----------------
	UNS_ACK,
	UNS_ATPING,
	UNS_ATADMWRITE,
	UNS_ATADMANN,
	UNS_ATBLK,
	UNS_NOTIFY,
	UNS_ATLSN,
	NB_AT_UNS,
	// ----
	UNS_NULL = NB_AT_UNS,
	// ----------------
	NB_UNS_CMD,
} at_cmd_code_e;

/*!
 * @}
 * @endcond
 */

/*!
 * @brief This table describe all AT command
 */
extern const struct at_desc_s aAtDescCmd[];

#ifdef __cplusplus
}
#endif
#endif /* _AT_COMMAND_H_ */

/*! @} */
