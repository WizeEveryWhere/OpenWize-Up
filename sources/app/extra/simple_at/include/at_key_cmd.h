/**
  * @file at_key_cmd.h
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
  * @par 1.0.0 : 2023/05/03 [TODO: your name]
  * Initial version
  *
  */
#ifndef AT_KEY_CMD_H_
#define AT_KEY_CMD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "atci.h"

#ifdef HAS_ATKEY_CMD
	atci_error_t Exec_ATKEY_Cmd(atci_cmd_t *atciCmdData);
#else //ifndef HAS_ATKEY_CMD
	atci_error_t Exec_ATKMAC_Cmd(atci_cmd_t *atciCmdData);
	atci_error_t Exec_ATKENC_Cmd(atci_cmd_t *atciCmdData);
#endif

#ifdef __cplusplus
}
#endif

#endif /* AT_KEY_CMD_H_ */
