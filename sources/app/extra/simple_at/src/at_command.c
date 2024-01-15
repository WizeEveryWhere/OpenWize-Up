/**
  * @file: at_command.c
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

#ifndef DISABLE_GRP0
	#define GRP0(Id, Str, pFct) DECLARE_AT(Id, Str, pFct)
#else
	#define GRP0(Id, Str, pFct) UN_DECLARE_AT(Id, Str, pFct)
#endif

#ifndef DISABLE_GRP1
	#define GRP1(Id, Str, pFct) DECLARE_AT(Id, Str, pFct)
#else
	#define GRP1(Id, Str, pFct) UN_DECLARE_AT(Id, Str, pFct)
#endif

#ifndef DISABLE_GRP2
	#define GRP2(Id, Str, pFct) DECLARE_AT(Id, Str, pFct)
#else
	#define GRP2(Id, Str, pFct) UN_DECLARE_AT(Id, Str, pFct)
#endif

#ifndef DISABLE_GRP3
	#define GRP3(Id, Str, pFct) DECLARE_AT(Id, Str, pFct)
#else
	#define GRP3(Id, Str, pFct) UN_DECLARE_AT(Id, Str, pFct)
#endif

#ifndef DISABLE_GRP4
	#define GRP4(Id, Str, pFct) DECLARE_AT(Id, Str, pFct)
#else
	#define GRP4(Id, Str, pFct) UN_DECLARE_AT(Id, Str, pFct)
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
	GRP0( CMD_AT      , "AT"      , Exec_CMD_AT)
	GRP0( CMD_ATI     , "ATI"     , Exec_CMD_ATI)
	GRP0( CMD_ATZ     , "ATZ"     , Exec_CMD_ATZn)
	GRP0( CMD_ATZ0    , "ATZ0"    , Exec_CMD_ATZn)
	GRP0( CMD_ATZ1    , "ATZ1"    , Exec_CMD_ATZn)
	GRP0( CMD_ATQ     , "ATQ"     , Exec_CMD_ATQ)
	GRP0( CMD_ATF     , "AT&F"    , Exec_CMD_ATF)
	GRP0( CMD_ATW     , "AT&W"    , Exec_CMD_ATW)
	GRP0( CMD_ATPARAM , "ATPARAM" , Exec_CMD_ATPARAM)

	GRP0( CMD_ATSEND  , "ATSEND"  , Exec_CMD_ATSEND)
	GRP0( CMD_ATPING  , "ATPING"  , Exec_CMD_ATPING)

	// --- Group 1 : Actions that could be processed at factory without impact
	GRP1( CMD_ATIDENT , "ATIDENT" , Exec_CMD_ATIDENT)
	GRP1( CMD_ATKEY   , "ATKEY"   , Exec_CMD_ATKEY)
	GRP1( CMD_ATFC    , "ATFC"    , Exec_CMD_ATFC)
	GRP1( CMD_ATTEST  , "ATTEST"  , Exec_CMD_ATTEST)
	GRP1( CMD_ATCAL   , "AT%CAL"  , Exec_CMD_ATCAL)

	// --- Group 2 : Actions required for update external FW
	GRP2( CMD_ATADMANN, "ATADMANN", Exec_CMD_ATADMANN)

	// --- Group 3 : Action required to locally update internal FW
	GRP3( CMD_ATANN   , "ATANN"   , Exec_CMD_ATANN)
	GRP3( CMD_ATBLK   , "ATBLK"   , Exec_CMD_ATBLK)
	GRP3( CMD_ATUPD   , "ATUPD"   , Exec_CMD_ATUPD)
	GRP3( CMD_ATBMAP  , "ATBMAP"  , Exec_CMD_ATBMAP)

	// --- Group 4 :
	GRP4( CMD_ATSTAT  , "AT%STAT" , Exec_CMD_ATSTAT)
	GRP4( CMD_ATCCLK  , "AT%CCLK" , Exec_CMD_ATCCLK)
	GRP4( CMD_ATUID   , "AT%UID"  , Exec_CMD_ATUID)
	// ---
	DECLARE_AT( CMD_NULL, "NULL"    , NULL)

	// ---
	DECLARE_AT( UNS_ACK       , ""          , Exec_UNS_ACK)
	DECLARE_AT( UNS_ATPING    , "ATPING"    , Exec_UNS_ATPING)
	DECLARE_AT( UNS_ATADMWRITE, "ATADMWRITE", Exec_UNS_ATADMWRITE)
	DECLARE_AT( UNS_ATADMANN  , "ATADMANN"  , Exec_UNS_ATADMANN)
	DECLARE_AT( UNS_ATBLK     , "ATBLK"     , Exec_UNS_ATBLK)
	DECLARE_AT( UNS_NOTIFY    , "NOTIFY"    , Exec_UNS_NOTIFY)
	// ---
	DECLARE_AT( UNS_NULL      , "NULL"      , NULL)
};

#ifdef __cplusplus
}
#endif

/*! @} */
