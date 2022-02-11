/**********************************************************************************************************
  * @file: atci_get_cmd.c
  * @brief: This file contains low level functions (command decoding) of the AT command interpreter for WizeUp
  * module
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
  * 0.0.1 : 2021/01/11
  * Dev. version
  *
  *
 *********************************************************************************************************/

/*=========================================================================================================
 * INCLUDES
 *=======================================================================================================*/

#include <stdint.h>
#include <string.h>

#include "atci.h"
#include "atci_get_cmd.h"
#include "console.h"


/*=========================================================================================================
 * GLOBAL VARIABLES
 *=======================================================================================================*/

/*=========================================================================================================
 * LOCAL FUNCTIONS PROTOTYPES
 *=======================================================================================================*/

atci_status_t Atci_Buf_Get_Cmd_Str(atci_cmd_t *atciCmdData);
atci_status_t Atci_Buf_Get_Cmd_Param_Val(atci_cmd_t *atciCmdData, uint16_t valType);
atci_status_t Atci_Buf_Get_Cmd_Param_Array(atci_cmd_t *atciCmdData, uint16_t valLen);


/*=========================================================================================================
 * FUNCTIONS - Command reception
 *=======================================================================================================*/

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Receive AT command from UART interface
 * 				This function is blocking until a character has been received by UART or an error occurred
 *
 * @param[OUT]	atciCmdData ("atci_cmd_t" structure):
 * 					- buf [I/O]: buffer to receive command (as text) from console
 * 					- len [I/O]: actual received command length
 * 					(other fields are unused)
 *
 * @return		ATCI_NO_AT_CMD if no cmd received, ATCI_AVAIL_AT_CMD if full command received,
 * 				ATCI_RX_ERR if buffer overflow or RX error, ATCI_RX_CMD_TIMEOUT if no characters received for a specified time
 *-------------------------------------------------------------------------------------------------------*/
atci_status_t Atci_Rx_Cmd(atci_cmd_t *atciCmdData)
{
	atci_status_t status = ATCI_NO_AT_CMD;

	switch(Console_Wait_Rx_Byte(&(atciCmdData->buf[atciCmdData->len])))
	{
		case CONSOLE_RX_ERR:
			status = ATCI_RX_CMD_ERR;
			atciCmdData->len = 0;
			break;

		case CONSOLE_BYTE_RX:
			if(atciCmdData->buf[atciCmdData->len] == END_OF_CMD_CHAR)
			{
				if(atciCmdData->len != 0)
					status = ATCI_AVAIL_AT_CMD;
			}
			else if(atciCmdData->buf[atciCmdData->len] == BACK_SPACE_CHAR)
			{
				if(atciCmdData->len != 0)
					atciCmdData->len--;
			}
			else if(atciCmdData->len >= AT_CMD_BUF_LEN)
			{
				status = ATCI_RX_CMD_ERR;
				atciCmdData->len = 0;
			}
			else if(IS_PRINTABLE_CHAR(atciCmdData->buf[atciCmdData->len]))
				atciCmdData->len++;
			break;

		case CONSOLE_TIMEOUT:
			status = ATCI_RX_CMD_TIMEOUT;
			break;

		default:
			break;
	}

	return status;
}

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Clean reception in order to receive next command
 *
 * @param[I/O]	atciCmdData ("atci_cmd_t" structure)
 *
 * @return		None
 *-------------------------------------------------------------------------------------------------------*/
void Atci_Restart_Rx(atci_cmd_t *atciCmdData)
{
	atciCmdData->len = 0;
	Console_Rx_Flush(); // new command received during executing a command are discarded
}

/*=========================================================================================================
 * FUNCTIONS - command decoding
 *=======================================================================================================*/

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Decode AT command code
 * 				When a full command has been received (Atci_Rx_Cmd return ATCI_AVAIL_AT_CMD) this function
 * 				extract the command code from buffer and decode it.
 *
 * @param[I/O]	atciCmdData ("atci_cmd_t" structure):
 * 					- buf [IN]: received command as text from console
 * 					- len [IN]: received command length
 * 					- cmdCode [OUT]: received command code (CMD_AT ... CMD_ATPING)
 * 					(other fields are used internally)
 *
 * @return		status: ATCI_OK if succeed, else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR)
 *-------------------------------------------------------------------------------------------------------*/
atci_status_t Atci_Get_Cmd_Code(atci_cmd_t *atciCmdData)
{
	atci_status_t status;

	atciCmdData->idx = 0;
	status = Atci_Buf_Get_Cmd_Str(atciCmdData);

	if(status != ATCI_OK)
		return status;
	else if(strcmp(atciCmdData->cmdCodeStr, "AT") == 0)
		atciCmdData->cmdCode = CMD_AT;
	else if(strcmp(atciCmdData->cmdCodeStr, "ATI") == 0)
		atciCmdData->cmdCode = CMD_ATI;
	else if(strcmp(atciCmdData->cmdCodeStr, "ATZ") == 0)
		atciCmdData->cmdCode = CMD_ATZ;
	else if(strcmp(atciCmdData->cmdCodeStr, "ATQ") == 0)
		atciCmdData->cmdCode = CMD_ATQ;
	else if(strcmp(atciCmdData->cmdCodeStr, "AT&F") == 0)
		atciCmdData->cmdCode = CMD_ATF;
	else if(strcmp(atciCmdData->cmdCodeStr, "AT&W") == 0)
		atciCmdData->cmdCode = CMD_ATW;
	else if(strcmp(atciCmdData->cmdCodeStr, "ATPARAM") == 0)
		atciCmdData->cmdCode = CMD_ATPARAM;
	else if(strcmp(atciCmdData->cmdCodeStr, "ATKMAC") == 0)
		atciCmdData->cmdCode = CMD_ATKMAC;
	else if(strcmp(atciCmdData->cmdCodeStr, "ATKENC") == 0)
		atciCmdData->cmdCode = CMD_ATKENC;
	else if(strcmp(atciCmdData->cmdCodeStr, "ATIDENT") == 0)
		atciCmdData->cmdCode = CMD_ATIDENT;
	else if(strcmp(atciCmdData->cmdCodeStr, "ATSEND") == 0)
		atciCmdData->cmdCode = CMD_ATSEND;
	else if(strcmp(atciCmdData->cmdCodeStr, "ATPING") == 0)
		atciCmdData->cmdCode = CMD_ATPING;
	else if(strcmp(atciCmdData->cmdCodeStr, "ATFC") == 0)
		atciCmdData->cmdCode = CMD_ATFC;
	else if(strcmp(atciCmdData->cmdCodeStr, "ATTEST") == 0)
		atciCmdData->cmdCode = CMD_ATTEST;
	else
	{
		return ATCI_ERR_UNKNOWN_CMD;
	}
	return ATCI_OK;
}

/*!--------------------------------------------------------------------------------------------------------
 * @brief		extract one command parameter from buffer (parameter is a 8, 16 or 32 bits integer)
 *
 * @param[I/O]	atciCmdData ("atci_cmd_t" structure):
 * 					- buf [IN]: received command as text from console
 * 					- len [IN]: received command length
 * 					- idx [OUT]: index in command buffer for command parameters beginning (if any; reset at the beginning of this function)
 * 					- cmdType [I/O]: command type: if all parameters are read or not and if it is a read command or not (AT_CMD_WITHOUT_PARAM ... AT_CMD_READ_WITH_PARAM)
 * 					(other fields are used internally or unused)
 *					- params [I/O]: command parameters list, the first free slot is used (nbParams)
 *						- size [OUT]: type of value (PARAM_INT8, PARAM_INT16, PARAM_INT32)
 *						- val8, val16 or val32 [OUT]: parameter value (according to size)
 *					- nbParams [I/O]: IN: current parameter list index to write; OUT new number of parameters read (nbParams incremented in function)
 * @param[IN]	valTypeSize: parameter type: PARAM_INT8, PARAM_INT16 or PARAM_INT32 (8, 16 or 32 bits integer), 0x00 = array of variable length, 0x01~0x7F = array of this wanted length
 *
 * @return		status: ATCI_OK if succeed, else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR)
 *-------------------------------------------------------------------------------------------------------*/
atci_status_t Atci_Buf_Get_Cmd_Param(atci_cmd_t *atciCmdData, uint16_t valTypeSize)
{
	if(IS_PARAM_INT(valTypeSize))
		return Atci_Buf_Get_Cmd_Param_Val(atciCmdData, valTypeSize);
	else
		return Atci_Buf_Get_Cmd_Param_Array(atciCmdData, valTypeSize);
}

/*=========================================================================================================
 * FUNCTIONS - command parameters memory management
 *=======================================================================================================*/

/*!--------------------------------------------------------------------------------------------------------
 * @brief		init 1st cmd data pointer and reset number of params
 *
 * @param[I/O]	atciCmdData ("atci_cmd_t" structure)
 *
 * @return		None
 *-------------------------------------------------------------------------------------------------------*/
void Atci_Cmd_Param_Init(atci_cmd_t *atciCmdData)
{
	atciCmdData->nbParams = 0;
	atciCmdData->params[0].data = atciCmdData->paramsMem;
	atciCmdData->paramsMemIdx=0;
}

/*!--------------------------------------------------------------------------------------------------------
 * @brief		init next param data pointer and increment number of params (previous params length must not
 * 					be modified extept if this new param is the last one)
 * 					This function does nothing if maximum number of parameter reached)
 * 					Note: Atci_Buf_Get_Cmd_Param call this function if parameter extraction succeed
 *
 * @param[I/O]	atciCmdData ("atci_cmd_t" structure)
 *
 * @return		status: ATCI_OK if succeed, else error code (ATCI_ERR)
 *-------------------------------------------------------------------------------------------------------*/
atci_status_t Atci_Add_Cmd_Param_Resp(atci_cmd_t *atciCmdData)
{
	if(atciCmdData->nbParams < AT_CMD_MAX_NB_PARAM)
	{
		if(IS_PARAM_INT(atciCmdData->params[atciCmdData->nbParams].size))
			atciCmdData->paramsMemIdx += PARAM_INT_SIZE(atciCmdData->params[atciCmdData->nbParams].size);
		else if(IS_PARAM_STR(atciCmdData->params[atciCmdData->nbParams].size))
			atciCmdData->paramsMemIdx += PARAM_STR_SIZE(atciCmdData->params[atciCmdData->nbParams].size);
		else
			atciCmdData->paramsMemIdx += atciCmdData->params[atciCmdData->nbParams].size;
		if(atciCmdData->paramsMemIdx < AT_CMD_DATA_MAX_LEN)
		{
			atciCmdData->nbParams++;
			if(atciCmdData->nbParams < AT_CMD_MAX_NB_PARAM)
				atciCmdData->params[atciCmdData->nbParams].data = &(atciCmdData->paramsMem[atciCmdData->paramsMemIdx]);
		}
		return ATCI_OK;
	}
	else
		return ATCI_ERR;
}

/*!--------------------------------------------------------------------------------------------------------
 * @brief		update last param length and update next param pointer
 *
 * @param[I/O]	atciCmdData ("atci_cmd_t" structure)
 *
 * @return		status: ATCI_OK if succeed, else error code (ATCI_ERR)
 *-------------------------------------------------------------------------------------------------------*/
atci_status_t Atci_Update_Cmd_Param_len(atci_cmd_t *atciCmdData, uint16_t newSize)
{
	uint16_t newLen;

	if(IS_PARAM_INT(newSize))
		newLen = PARAM_INT_SIZE(newSize);
	else if(IS_PARAM_STR(newSize))
		newLen = PARAM_STR_SIZE(newSize);
	else
		newLen = newSize;

	if((atciCmdData->paramsMemIdx + newLen) <= AT_CMD_DATA_MAX_LEN)
	{
		atciCmdData->params[atciCmdData->nbParams-1].size = newSize;
		if(atciCmdData->nbParams < AT_CMD_MAX_NB_PARAM)
			atciCmdData->params[atciCmdData->nbParams].data = &(atciCmdData->paramsMem[atciCmdData->paramsMemIdx]);

		return ATCI_OK;
	}
	else
		return ATCI_ERR;
}


/*=========================================================================================================
 * LOCAL FUNCTIONS
 *=======================================================================================================*/


/*!--------------------------------------------------------------------------------------------------------
 * @brief		extract command string from buffer
 *
 * @param[I/O]	atciCmdData ("atci_cmd_t" structure):
 * 					- buf [IN]: received command as text from console
 * 					- len [IN]: received command length
 * 					- idx [OUT]: index in command buffer for command parameters beginning (if any; reset at the beginning of this function)
 * 					- cmdCodeStr [OUT]: command code as a string to be decoded
 * 					- cmdType [OUT]: command type: if it has parameters or not and if it is a read command or not (AT_CMD_WITHOUT_PARAM ... AT_CMD_READ_WITH_PARAM)
 * 					(other fields are used internally or unused)
 *
 * @return		status: ATCI_OK if succeed, else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR)
 *-------------------------------------------------------------------------------------------------------*/
atci_status_t Atci_Buf_Get_Cmd_Str(atci_cmd_t *atciCmdData)
{
	uint16_t cmdLen = 0;

	atciCmdData->cmdType = AT_CMD_WITHOUT_PARAM;

	for(atciCmdData->idx = 0; atciCmdData->idx < atciCmdData->len;  atciCmdData->idx++)
	{
		if(cmdLen>=AT_CMD_CODE_MAX_LEN)
		{
			cmdLen = 0;
			break;
		}
		else if((atciCmdData->buf[atciCmdData->idx] >= 'A') && (atciCmdData->buf[atciCmdData->idx] <= 'Z'))
			atciCmdData->cmdCodeStr[cmdLen++] = atciCmdData->buf[atciCmdData->idx];
		else if((atciCmdData->buf[atciCmdData->idx] >= 'a') && (atciCmdData->buf[atciCmdData->idx] <= 'z'))
			atciCmdData->cmdCodeStr[cmdLen++] = TO_MAG(atciCmdData->buf[atciCmdData->idx]);
		else if(atciCmdData->buf[atciCmdData->idx] == CMD_AND_CHAR)
			atciCmdData->cmdCodeStr[cmdLen++] = atciCmdData->buf[atciCmdData->idx];
		else if(atciCmdData->buf[atciCmdData->idx] == CMD_READ_CHAR)
		{
			atciCmdData->cmdType = AT_CMD_READ_WITHOUT_PARAM;
			atciCmdData->idx++;
			break;
		}
		else if(atciCmdData->buf[atciCmdData->idx] == CMD_PARAM_CHAR)
		{
			atciCmdData->cmdType = AT_CMD_WITH_PARAM_TO_GET;
			atciCmdData->idx++;
			break;
		}
		//else ignore char
	}

	atciCmdData->cmdCodeStr[cmdLen] = 0;

	if(cmdLen < AT_CMD_CODE_MIN_LEN)
		return ATCI_ERR_INV_CMD_LEN;
	else
		return ATCI_OK;
}

/*!--------------------------------------------------------------------------------------------------------
 * @brief		extract one command parameter from buffer (parameter is a 8, 16 or 32 bits integer)
 *
 * @param[I/O]	atciCmdData ("atci_cmd_t" structure):
 * 					- buf [IN]: received command as text from console
 * 					- len [IN]: received command length
 * 					- idx [OUT]: index in command buffer for command parameters beginning (if any; reset at the beginning of this function)
 * 					- cmdType [I/O]: command type: if all parameters are read or not and if it is a read command or not (AT_CMD_WITHOUT_PARAM ... AT_CMD_READ_WITH_PARAM)
 * 					(other fields are used internally or unused)
 *					- params [I/O]: command parameters list, the first free slot is used (nbParams)
 *						- size [OUT]: type of value (PARAM_INT8, PARAM_INT16, PARAM_INT32)
 *						- val8, val16 or val32 [OUT]: parameter value (according to size)
 *					- nbParams [I/O]: IN: current parameter list index to write; OUT new number of parameters read (nbParams incremented in function)
 * @param[IN]	valType: parameter type: PARAM_INT8, PARAM_INT16 or PARAM_INT32 (8, 16 or 32 bits integer)
 *
 * @return		status: ATCI_OK if succeed, else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR)
 *-------------------------------------------------------------------------------------------------------*/
atci_status_t Atci_Buf_Get_Cmd_Param_Val(atci_cmd_t *atciCmdData, uint16_t valType)
{
	atci_param_state_t state = PARAM_WAIT_BEGIN;
	uint32_t val = 0;
	uint8_t data;
	uint32_t lastValUMax;
	uint32_t newDigitUMax;
	uint32_t lastValSMax;
	uint32_t  newDigitSMax;
	uint8_t nbNibbles;


	if((atciCmdData->cmdType != AT_CMD_WITH_PARAM_TO_GET) || (atciCmdData->nbParams >= AT_CMD_MAX_NB_PARAM))
		return ATCI_ERR_INV_NB_PARAM;

	switch(valType)
	{
		default:
		case PARAM_INT8:
			lastValUMax = 25;
			newDigitUMax = 5;
			lastValSMax = 12;
			newDigitSMax = 8;
			nbNibbles = 2;
			break;
		case PARAM_INT16:
			lastValUMax = 6553;
			newDigitUMax = 5;
			lastValSMax = 3276;
			newDigitSMax = 8;
			nbNibbles = 4;
			break;
		case PARAM_INT32:
			lastValUMax = 429496729;
			newDigitUMax = 5;
			lastValSMax = 214748364;
			newDigitSMax = 8;
			nbNibbles = 8;
			break;
	}

	val = 0;
	atciCmdData->cmdType = AT_CMD_WITH_PARAM;
	for(; atciCmdData->idx < atciCmdData->len; atciCmdData->idx++)
	{
		if(atciCmdData->buf[atciCmdData->idx] == CMD_SEP_CHAR)
		{
			atciCmdData->cmdType = AT_CMD_WITH_PARAM_TO_GET;
			atciCmdData->idx++;
			break;
		}
		else if(atciCmdData->buf[atciCmdData->idx] == CMD_READ_CHAR)
		{
			atciCmdData->cmdType = AT_CMD_READ_WITH_PARAM;
			atciCmdData->idx++;
			break;
		}
		//else
		switch(state)
		{
			case PARAM_WAIT_BEGIN:
				if(atciCmdData->buf[atciCmdData->idx] == CMD_HEX_CHAR)
					state = PARAM_HEX;
				else if(atciCmdData->buf[atciCmdData->idx] == CMD_NEG_CHAR)
					state = PARAM_DEC_NEG;
				else
				{
					data = decascii2nb(atciCmdData->buf[atciCmdData->idx]);
					if(data != 0xFF)
					{
						state = PARAM_DEC;
						val = data;
					}
				}
				break;

			case PARAM_HEX:

				data = hexascii2nibble(atciCmdData->buf[atciCmdData->idx]);
				if(data != 0xFF)
				{
					if(nbNibbles == 0)
						state = PARAM_ERR;
					else
					{
						val = (val << 4) | data;
						nbNibbles--;
					}
				}

				break;

			case PARAM_DEC:

				data = decascii2nb(atciCmdData->buf[atciCmdData->idx]);
				if(data != 0xFF)
				{
					if(val > lastValUMax)
						state = PARAM_ERR;
					else if((val == lastValUMax) && (data > newDigitUMax))
						state = PARAM_ERR;
					else
						val = (val * 10) + data;
				}

				break;

			case PARAM_DEC_NEG:

				data = decascii2nb(atciCmdData->buf[atciCmdData->idx]);
				if(data != 0xFF)
				{
					if(val > lastValSMax)
						state = PARAM_ERR;
					else if((val == lastValSMax) && (data > newDigitSMax))
						state = PARAM_ERR;
					else
						val = (val * 10) + data;
				}

				break;

			default:
			case PARAM_ERR:
				break;
		}
	}

	if(state == PARAM_ERR)
		return ATCI_ERR_INV_PARAM_VAL;
	else if(state == PARAM_WAIT_BEGIN)
		return ATCI_ERR_INV_PARAM_LEN;
	else if((state == PARAM_HEX) && (nbNibbles != 0))
		return ATCI_ERR_INV_PARAM_LEN;
	else if((state == PARAM_DEC_NEG) && (val == 0))
		return ATCI_ERR_INV_PARAM_VAL;
	else
	{
		if(state == PARAM_DEC_NEG)
			val = -val;

		switch(valType)
		{
			default:
			case PARAM_INT8:
				if(atciCmdData->paramsMemIdx >= AT_CMD_DATA_MAX_LEN)
					return ATCI_ERR_INV_CMD_LEN;
				*(atciCmdData->params[atciCmdData->nbParams].val8) = (uint8_t) val;
				break;
			case PARAM_INT16:
				if(atciCmdData->paramsMemIdx > (AT_CMD_DATA_MAX_LEN-2))
					return ATCI_ERR_INV_CMD_LEN;
				*(atciCmdData->params[atciCmdData->nbParams].val16) = (uint16_t) val;
				break;
			case PARAM_INT32:
				if(atciCmdData->paramsMemIdx > (AT_CMD_DATA_MAX_LEN-2))
					return ATCI_ERR_INV_CMD_LEN;
				*(atciCmdData->params[atciCmdData->nbParams].val32) = val;
				break;
		}
		atciCmdData->params[atciCmdData->nbParams].size = valType;
		Atci_Add_Cmd_Param_Resp(atciCmdData);

		return ATCI_OK;
	}
}

/*!--------------------------------------------------------------------------------------------------------
 * @brief		extract one command parameter from buffer (parameter is a bytes array)
 *
 * @param[I/O]	atciCmdData ("atci_cmd_t" structure):
 * 					- buf [IN]: received command as text from console
 * 					- len [IN]: received command length
 * 					- idx [OUT]: index in command buffer for command parameters beginning (if any; reset at the beginning of this function)
 * 					- cmdType [OUT]: command type: if all parameters are read or not and if it is a read command or not (AT_CMD_WITHOUT_PARAM ... AT_CMD_READ_WITH_PARAM)
 * 					(other fields are used internally or unused)
 *					- params [I/O]: command parameters list, the first free slot is used (nbParams)
 *						- size [OUT]: parameter length (in bytes)
 *						- array [OUT]: parameter data
 *					- nbParams [I/O]: IN: current parameter list index to write; OUT new number of parameters read (nbParams incremented in function)
 * @param[IN]	valLen: 0x00 = array of variable length, 0x01~0x7F = array of this wanted length
 * @return		status: ATCI_OK if succeed, else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR)
 *
 *-------------------------------------------------------------------------------------------------------*/
atci_status_t Atci_Buf_Get_Cmd_Param_Array(atci_cmd_t *atciCmdData, uint16_t valLen)
{
	atci_param_state_t state = PARAM_WAIT_BEGIN;
	uint8_t data;
	uint16_t nbBytes = 0;

	if((atciCmdData->cmdType != AT_CMD_WITH_PARAM_TO_GET) || (atciCmdData->nbParams >= AT_CMD_MAX_NB_PARAM))
		return ATCI_ERR_INV_NB_PARAM;

	atciCmdData->cmdType = AT_CMD_WITH_PARAM;
	for(; atciCmdData->idx < atciCmdData->len; atciCmdData->idx++)
	{
		if(atciCmdData->buf[atciCmdData->idx] == CMD_SEP_CHAR)
		{
			atciCmdData->cmdType = AT_CMD_WITH_PARAM_TO_GET;
			atciCmdData->idx++;
			break;
		}
		else if(atciCmdData->buf[atciCmdData->idx] == CMD_READ_CHAR)
		{
			atciCmdData->cmdType = AT_CMD_READ_WITH_PARAM;
			atciCmdData->idx++;
			break;
		}
		//else
		switch(state)
		{
			case PARAM_WAIT_BEGIN:
				if(atciCmdData->buf[atciCmdData->idx] == CMD_HEX_CHAR)
					state = PARAM_HEX;
				break;

			case PARAM_HEX:

				data = hexascii2nibble(atciCmdData->buf[atciCmdData->idx]);
				if(data != 0xFF)
				{
					if(nbBytes >= (AT_CMD_DATA_MAX_LEN-atciCmdData->paramsMemIdx))
						state = PARAM_ERR;
					else
					{
						atciCmdData->params[atciCmdData->nbParams].data[nbBytes] = data << 4;
						state = PARAM_HEX_LSB;
					}
				}
				break;

			case PARAM_HEX_LSB:

				data = hexascii2nibble(atciCmdData->buf[atciCmdData->idx]);
				if(data != 0xFF)
				{
					atciCmdData->params[atciCmdData->nbParams].data[nbBytes] |= data;
					nbBytes++;
					state = PARAM_HEX;
				}
				break;

			default:
			case PARAM_ERR:
				break;
		}
	}

	if(state == PARAM_ERR)
		return ATCI_ERR_INV_PARAM_VAL;
	else if(state == PARAM_WAIT_BEGIN)
		return ATCI_ERR_INV_PARAM_LEN;
	else if(state == PARAM_HEX_LSB)
		return ATCI_ERR_INV_PARAM_LEN;
	else if((valLen != PARAM_VARIABLE_LEN) && (valLen != nbBytes))
		return ATCI_ERR_INV_PARAM_LEN;
	else
	{
		atciCmdData->params[atciCmdData->nbParams].size = nbBytes;
		Atci_Add_Cmd_Param_Resp(atciCmdData);
		return ATCI_OK;
	}
}


/************************************************** EOF **************************************************/


