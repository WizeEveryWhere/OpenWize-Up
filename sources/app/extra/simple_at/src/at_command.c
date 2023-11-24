/**
  * @file: at_command.c
  * @brief: This file define the AT command global table
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
  * 1.0.0 : 2023/11/06[GBI]
  * Initial version
  *
  *
  */

/*!
 *  @addtogroup atci
 *  @ingroup app
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "at_command.h"

#include "at_access_param.h"
#include "at_test_cmd.h"
#include "at_wize_cmd.h"
#include "at_key_cmd.h"
#include "at_extend_cmd.h"
#include "at_lo_update_cmd.h"

/******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */

#define DECLARE_AT(Id, Str, pFct) [Id] = {pFct, Str},
#define UN_DECLARE_AT(Id, Str, pFct) [Id] = { Exec_AT_Cmd, Str},

#ifndef DISABLE_GROUP0
	#define GROUP0(Id, Str, pFct) DECLARE_AT(Id, Str, pFct)
#else
	#define GROUP0(Id, Str, pFct) UN_DECLARE_AT(Id, Str, pFct)
#endif

#ifndef DISABLE_GROUP1
	#define GROUP1(Id, Str, pFct) DECLARE_AT(Id, Str, pFct)
#else
	#define GROUP1(Id, Str, pFct) UN_DECLARE_AT(Id, Str, pFct)
#endif

#ifndef DISABLE_GROUP2
	#define GROUP2(Id, Str, pFct) DECLARE_AT(Id, Str, pFct)
#else
	#define GROUP2(Id, Str, pFct) UN_DECLARE_AT(Id, Str, pFct)
#endif

#ifndef DISABLE_GROUP3
	#define GROUP3(Id, Str, pFct) DECLARE_AT(Id, Str, pFct)
#else
	#define GROUP3(Id, Str, pFct) UN_DECLARE_AT(Id, Str, pFct)
#endif
/*!
 * @}
 * @endcond
 */

/*!
 * @brief This table describe all AT command
 */
const struct at_desc_s aAtDescCmd[NB_UNS_CMD] =
{
	GROUP0( CMD_AT     , "AT"     , Exec_AT_Cmd)
	GROUP0( CMD_ATI    , "ATI"    , Exec_ATI_Cmd)
	GROUP0( CMD_ATZ    , "ATZ"    , Exec_ATZn_Cmd)
	GROUP0( CMD_ATZ0   , "ATZ0"   , Exec_ATZn_Cmd)
	GROUP0( CMD_ATZ1   , "ATZ1"   , Exec_ATZn_Cmd)
	GROUP0( CMD_ATQ    , "ATQ"    , Exec_ATQ_Cmd)
	GROUP0( CMD_ATF    , "AT&F"   , Exec_ATF_Cmd)
	GROUP0( CMD_ATW    , "AT&W"   , Exec_ATW_Cmd)
	GROUP0( CMD_ATPARAM, "ATPARAM", Exec_ATPARAM_Cmd)
	GROUP0( CMD_ATSEND , "ATSEND" , Exec_ATSEND_Cmd)
	GROUP0( CMD_ATPING , "ATPING" , Exec_ATPING_Cmd)

	// --- Group 1 : Actions that could be processed at factory without impact
	GROUP1( CMD_ATIDENT, "ATIDENT", Exec_ATIDENT_Cmd)
	GROUP1( CMD_ATKEY  , "ATKEY"  , Exec_ATKEY_Cmd)
	GROUP1( CMD_ATFC   , "ATFC"   , Exec_ATFC_Cmd)
	GROUP1( CMD_ATTEST , "ATTEST" , Exec_ATTEST_Cmd)
	GROUP1( CMD_ATCAL  , "AT%CAL" , Exec_ATCAL_Cmd)

	// --- Group 2 : Action required to locally update internal FW
	GROUP2( CMD_ATANN  , "ATANN"  , Exec_ATANN_Cmd)
	GROUP2( CMD_ATBLK  , "ATBLK"  , Exec_ATBLK_Cmd)
	GROUP2( CMD_ATUPD  , "ATUPD"  , Exec_ATUPD_Cmd)
	GROUP2( CMD_ATBMAP , "ATBMAP" , Exec_ATBMAP_Cmd)

	// --- Group 3 :
	GROUP3( CMD_ATSTAT , "AT%STAT", Exec_ATSTAT_Cmd)
	GROUP3( CMD_ATCCLK , "AT%CCLK", Exec_ATCCLK_Cmd)
	GROUP3( CMD_ATUID  , "AT%UID" , Exec_ATUID_Cmd)
	// ---
	GROUP3( CMD_NULL   , "NULL"   , NULL)

	// ---
	DECLARE_AT( UNS_ATBLK     , "ATBLK"     , Exec_ATBLK_Notify)
	DECLARE_AT( UNS_ATADMWRITE, "ATADMWRITE", Exec_ATADMWRITE_Notify)
	DECLARE_AT( UNS_ATADMANN  , "ATADMANN"  , Exec_ATADMANN_Notify)
	DECLARE_AT( UNS_NOTIFY    , "NOTIFY"    , Exec_Generic_Notify)
	// ---
	DECLARE_AT( UNS_NULL      , "NULL"      , NULL)
};

#ifdef __cplusplus
}
#endif

/*! @} */
