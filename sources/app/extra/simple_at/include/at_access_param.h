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
#ifndef AT_ACCESS_PARAM_H_
#define AT_ACCESS_PARAM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "atci.h"

atci_error_t Exec_ATI_Cmd(atci_cmd_t *atciCmdData);
atci_error_t Exec_ATPARAM_Cmd(atci_cmd_t *atciCmdData);
atci_error_t Exec_ATIDENT_Cmd(atci_cmd_t *atciCmdData);

#ifdef __cplusplus
}
#endif

#endif /* AT_ACCESS_PARAM_H_ */
