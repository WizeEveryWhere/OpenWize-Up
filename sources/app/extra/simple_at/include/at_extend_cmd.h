/**
  * @file at_extend_cmd.h
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
  * @par 1.0.0 : 2023/05/03 [GBI]
  * Initial version
  *
  */
#ifndef AT_EXTEND_CMD_H_
#define AT_EXTEND_CMD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "atci.h"

atci_error_t Exec_ATF_Cmd(atci_cmd_t *atciCmdData);
atci_error_t Exec_ATW_Cmd(atci_cmd_t *atciCmdData);

#ifdef HAS_ATCCLK_CMD
atci_error_t Exec_ATCCLK_Cmd(atci_cmd_t *atciCmdData);
#endif

#ifdef HAS_ATUID_CMD
atci_error_t Exec_ATUID_Cmd(atci_cmd_t *atciCmdData);
#endif

#ifdef HAS_ATZn_CMD
atci_error_t Exec_ATZn_Cmd(atci_cmd_t *atciCmdData);
#endif

#ifdef HAS_ATSTAT_CMD
atci_error_t Exec_ATSTAT_Cmd(atci_cmd_t *atciCmdData);
#endif

#ifdef __cplusplus
}
#endif

#endif /* AT_EXTEND_CMD_H_ */
