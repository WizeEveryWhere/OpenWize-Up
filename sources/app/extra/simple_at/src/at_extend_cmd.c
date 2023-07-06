/**
  * @file at_extend_cmd.c
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

#ifdef __cplusplus
extern "C" {
#endif

#include "at_extend_cmd.h"

#include "atci_get_cmd.h"
#include "atci_resp.h"


/******************************************************************************/
#ifdef HAS_ATSTAT_CMD
#include "app_entry.h"

static
void _format_stats_(uint8_t *pData, net_stats_t *pStats);

static
void _format_stats_(uint8_t *pData, net_stats_t *pStats)
{
	int i;
	int sz = sizeof(net_stats_t);

	unsigned char *pIn = (unsigned char *)pStats;
	unsigned char *pOut = &(pData[sz-1]);

	for (i = 0; i < sz; i++)
	{
		*pOut = *pIn;
		pIn++;
		pOut--;
	}
}

/*!
 * @brief This function
 *
 * @retval
 */
atci_error_t Exec_ATSTAT_Cmd(atci_cmd_t *atciCmdData)
{
	atci_error_t status = ATCI_ERR_NONE;
	net_stats_t sStats;

	Atci_Cmd_Param_Init(atciCmdData);

	if ( atciCmdData->cmdType == AT_CMD_READ_WITHOUT_PARAM )
	{
		// Add param of size sizeof(net_stats_t)
		atciCmdData->params[0].size = sizeof(net_stats_t);
		status = Atci_Add_Cmd_Param_Resp(atciCmdData);
		if (status == ATCI_ERR_NONE)
		{
			NetMgr_Open(NULL);
			if (NetMgr_Ioctl(NETDEV_CTL_GET_STATS, (uint32_t)(&sStats)) == NET_STATUS_OK)
			{
				_format_stats_( atciCmdData->params[0].data, &sStats);

				Atci_Resp_Data(atci_cmd_code_str[atciCmdData->cmdCode], atciCmdData);
			}
			else
			{
				status = ATCI_ERR_UNK;
			}
			NetMgr_Close();
		}
	}
	else if (atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
	{
		status = Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
		if (status == ATCI_ERR_NONE)
		{
			if (atciCmdData->cmdType != AT_CMD_WITH_PARAM)
			{
				status = ATCI_ERR_PARAM_NB;
			}
			else
			{
				NetMgr_Open(NULL);
				//if ( *(atciCmdData->params[0].val8 == 0)
				if (NetMgr_Ioctl(NETDEV_CTL_CLR_STATS, (uint32_t)(&sStats)))
				{
					// error
					status = ATCI_ERR_UNK;
				}
				NetMgr_Close();
			}
		}
	}
	else
	{
		status = ATCI_ERR_PARAM_NB;
	}

	return status;
}
#endif
/******************************************************************************/

#ifdef HAS_ATCCLK_CMD
/*!
 * @brief This function
 *
 * @retval
 */
atci_error_t Exec_ATCCLK_Cmd(atci_cmd_t *atciCmdData)
{
	if (
		(atciCmdData->cmdType == AT_CMD_READ_WITHOUT_PARAM) ||
		(atciCmdData->cmdType == AT_CMD_WITHOUT_PARAM)
		)
	{
		Atci_Cmd_Param_Init(atciCmdData);

		// Add param of size 4
		atciCmdData->params[0].size = 4;
		Atci_Add_Cmd_Param_Resp(atciCmdData);

		// Add param of size 2
		atciCmdData->params[1].size = 2;
		Atci_Add_Cmd_Param_Resp(atciCmdData);

		struct timeval tm;
		gettimeofday(&tm, NULL);

		// Get the EPOCH (second part)
		*(uint32_t*)(atciCmdData->params[0].data) = __htonl(tm.tv_sec);
		// Get the EPOCH (millisecond part)
		*(uint16_t*)(atciCmdData->params[1].data) = __htons(tm.tv_usec/1000);

		Atci_Resp_Data(atci_cmd_code_str[atciCmdData->cmdCode], atciCmdData);
		return ATCI_ERR_NONE;
	}
	else
	{
		return ATCI_ERR_PARAM_NB;
	}
}
#endif

/******************************************************************************/

#ifdef HAS_ATUID_CMD
/*!
 * @brief This function
 *
 * @retval
 */
atci_error_t Exec_ATUID_Cmd(atci_cmd_t *atciCmdData)
{
	if (
		(atciCmdData->cmdType == AT_CMD_READ_WITHOUT_PARAM) ||
		(atciCmdData->cmdType == AT_CMD_WITHOUT_PARAM)
		)
	{
		Atci_Cmd_Param_Init(atciCmdData);

		// Add param of size 8
		atciCmdData->params[0].size = 8;
		Atci_Add_Cmd_Param_Resp(atciCmdData);

		// Get the UID
		uint64_t uuid = BSP_GetUid();
		((uint32_t*)(atciCmdData->params[0].data))[0] = __htonl(((uint32_t*)&uuid)[1]);
		((uint32_t*)(atciCmdData->params[0].data))[1] = __htonl(((uint32_t*)&uuid)[0]);

		Atci_Resp_Data(atci_cmd_code_str[atciCmdData->cmdCode], atciCmdData);
		return ATCI_ERR_NONE;
	}
	else
	{
		return ATCI_ERR_PARAM_NB;
	}
}
#endif

#ifdef HAS_ATZn_CMD
/*!
 * @brief This function
 *
 * @retval
 */
atci_error_t Exec_ATZn_Cmd(atci_cmd_t *atciCmdData)
{
	if(atciCmdData->cmdType != AT_CMD_WITHOUT_PARAM)
		return ATCI_ERR_PARAM_NB;

	uint8_t eRebootMode = 0;
	switch (atciCmdData->cmdCode)
	{
	/*
	 * 	ATZ or ATZ0 : (cold reboot)
	 * 	-	Restore all registers from last stored ones in NVM
	 * 	-	Clear the current clock initialize flag value
	 * 	-	Clear all internal counters.
	 */
		case CMD_ATZ:
		case CMD_ATZ0:
			eRebootMode = 1;
			break;
	/*
	 * 	ATZ1 : (warm reboot)
	 * 	-	Keep all registers from RAM
	 * 	-	Keep the current clock initialize flag value
	 * 	-	Keep all internal counters.
	 */
		case CMD_ATZ1: // (warm reboot)
		default:
			break;
	}

	Atci_Resp_Ack(ATCI_ERR_NONE);
	if(eRebootMode)
	{
		Atci_Debug_Str("Cold Reboot");
		Storage_SetDefault();
	}
	else
	{
		Atci_Debug_Str("Warm Reboot");
	}


	BSP_Boot_Reboot(eRebootMode);

	return ATCI_ERR_NONE;
}
#endif

#ifdef __cplusplus
}
#endif
