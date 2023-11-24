/**
  * @file atci_get_cmd.c
  * @brief This file contains low level functions (command decoding) of the AT
  * command interpreter for WizeUp module.
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
  * @par 0.0.1 : 2021/01/11 [Alciom]
  * Dev. version
  *
  *
  */

/*!
 *  @addtogroup atci
 *  @ingroup app
 *  @{
 */

#include <stdint.h>
#include <string.h>

#include "atci.h"
#include "atci_get_cmd.h"
#include "console.h"

/******************************************************************************/
static atci_error_e Atci_Buf_Get_Cmd_Str(atci_cmd_t *atciCmdData);
static atci_error_e Atci_Buf_Get_Cmd_Param_Val(atci_cmd_t *atciCmdData, uint16_t valType);
static atci_error_e Atci_Buf_Get_Cmd_Param_Array(atci_cmd_t *atciCmdData, uint16_t valLen);

/******************************************************************************/
/*
 * FUNCTIONS - command decoding
 */

/*!
 * @brief		Decode AT command code
 *
 * @details		When a full command has been received (Atci_Rx_Cmd return ATCI_AVAIL_AT_CMD) this function
 * 				extract the command code from buffer and decode it.
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure:
 * 					- buf [in]: received command as text from console
 * 					- len [in]: received command length
 * 					- cmdCode [out]: received command code (CMD_AT ... CMD_ATPING) (other fields are used internally)
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR)
 *
 */
atci_error_e Atci_Get_Cmd_Code(atci_cmd_t *atciCmdData)
{
	atci_error_e status;
	uint8_t i;

	atciCmdData->cmdCode = 0xFF;

	status = Atci_Buf_Get_Cmd_Str(atciCmdData);

	if(status != ATCI_ERR_NONE)
	{
		return status;
	}

	for (i = 0; i < atciCmdData->cmd_code_nb; i++)
	{
		if (strcmp(atciCmdData->cmdCodeStr, atciCmdData->pCmdDesc[i].str) == 0)
		{
			atciCmdData->cmdCode = i;
			return ATCI_ERR_NONE;
		}
	}

	return ATCI_ERR_CMD_UNK;
}

/*!
 * @brief		Extract one command parameter from buffer (parameter is a 8, 16 or 32 bits integer)
 *
 * @param[in,out] atciCmdData Pointer on "atci_cmd_t" structure:
 * @parblock
 * - buf [in]: received command as text from console
 * - len [in]: received command length
 * - idx [out]: index in command buffer for command parameters beginning (if any; reset at the beginning of this function)
 * - cmdType [in,out]: command type: if all parameters are read or not and if it is a read command or not (AT_CMD_WITHOUT_PARAM ... AT_CMD_READ_WITH_PARAM) (other fields are used internally or unused)
 * - params [in,out]: command parameters list, the first free slot is used (nbParams)
 * - size [out]: type of value (PARAM_INT8, PARAM_INT16, PARAM_INT32)
 * - val8, val16 or val32 [out]: parameter value (according to size)
 * - nbParams [in,out]: IN: current parameter list index to write; OUT new number of parameters read (nbParams incremented in function)
 * @endparblock
 *
 * @param[in]	  valTypeSize Parameter type:
 * @parblock
 * - PARAM_INT8, PARAM_INT16 or PARAM_INT32 (8, 16 or 32 bits integer),
 * - 0x00 = array of variable length,
 * - 0x01~0x7F = array of this wanted length
 * @endparblock
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR)
 *
 */
atci_error_e Atci_Buf_Get_Cmd_Param(atci_cmd_t *atciCmdData, uint16_t valTypeSize)
{
	if(IS_PARAM_INT(valTypeSize))
		return Atci_Buf_Get_Cmd_Param_Val(atciCmdData, valTypeSize);
	else
		return Atci_Buf_Get_Cmd_Param_Array(atciCmdData, valTypeSize);
}

/******************************************************************************/
/*
 * FUNCTIONS - command parameters memory management
 */

/*!
 * @brief		Init 1st cmd data pointer and reset number of params
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 */
void Atci_Cmd_Param_Init(atci_cmd_t *atciCmdData)
{
	atciCmdData->nbParams = 0;
	atciCmdData->params[0].data = atciCmdData->paramsMem;
	atciCmdData->paramsMemIdx=0;
}

/*!
 * @brief		Init next param data pointer and increment number of params (previous params length must not
 * 					be modified extept if this new param is the last one)
 *
 * @details		This function does nothing if maximum number of parameter reached)
 *
 * @note        Atci_Buf_Get_Cmd_Param call this function if parameter extraction succeed
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- else error code (ATCI_ERR)
 *
 */
atci_error_e Atci_Add_Cmd_Param_Resp(atci_cmd_t *atciCmdData)
{
	if(atciCmdData->nbParams < AT_CMD_MAX_NB_PARAM)
	{
		if(IS_PARAM_STR(atciCmdData->params[atciCmdData->nbParams].size))
		{
			atciCmdData->paramsMemIdx += PARAM_STR_SIZE(atciCmdData->params[atciCmdData->nbParams].size);
		}
		else if(IS_PARAM_INT(atciCmdData->params[atciCmdData->nbParams].size))
		{
			atciCmdData->paramsMemIdx += PARAM_INT_SIZE(atciCmdData->params[atciCmdData->nbParams].size);
		}
		else
		{
			atciCmdData->paramsMemIdx += atciCmdData->params[atciCmdData->nbParams].size;
		}

		if(atciCmdData->paramsMemIdx < AT_CMD_DATA_MAX_LEN)
		{
			atciCmdData->nbParams++;
			if(atciCmdData->nbParams < AT_CMD_MAX_NB_PARAM)
			{
				atciCmdData->params[atciCmdData->nbParams].data = &(atciCmdData->paramsMem[atciCmdData->paramsMemIdx]);
			}
		}
		return ATCI_ERR_NONE;
	}
	else
	{
		return ATCI_ERR_UNK;
	}
}

/*!
 * @brief		Update last param length and update next param pointer
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 * @param[in] 	    newSize     The size of parameter
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- else error code (ATCI_ERR)
 *
 */
atci_error_e Atci_Update_Cmd_Param_len(atci_cmd_t *atciCmdData, uint16_t newSize)
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

		return ATCI_ERR_NONE;
	}
	else
		return ATCI_ERR_UNK;
}

/******************************************************************************/
/******************************************************************************/

/*!
 * @static
 * @brief		Extract command string from buffer
 *
 * @param[in,out]	atciCmdData Pointer "atci_cmd_t" structure:
 * 					- buf [in]: received command as text from console
 * 					- len [in]: received command length
 * 					- idx [out]: index in command buffer for command parameters beginning (if any; reset at the beginning of this function)
 * 					- cmdCodeStr [out]: command code as a string to be decoded
 * 					- cmdType [out]: command type: if it has parameters or not and if it is a read command or not (AT_CMD_WITHOUT_PARAM ... AT_CMD_READ_WITH_PARAM)
 * 					(other fields are used internally or unused)
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR)
 *
 */
static
atci_error_e Atci_Buf_Get_Cmd_Str(atci_cmd_t *atciCmdData)
{
	uint16_t cmdLen = 0;

	atciCmdData->cmdType = AT_CMD_WITHOUT_PARAM;

	if (atciCmdData->pComRxBuf->len < AT_CMD_CODE_MIN_LEN )
	{
		return ATCI_ERR_RX_CMD;
	}

	atciCmdData->pComRxBuf->len--;

	// If the last char is not the character match one
	if (atciCmdData->pComRxBuf->data[atciCmdData->pComRxBuf->len] != END_OF_CMD_CHAR)
	{
		return ATCI_ERR_RX_CMD;
	}

	for (atciCmdData->idx = 0; atciCmdData->idx < atciCmdData->pComRxBuf->len;  atciCmdData->idx++)
	{
		if (cmdLen >= AT_CMD_CODE_MAX_LEN)
		{
			cmdLen = 0;
			break;
		}
		if (atciCmdData->pComRxBuf->data[atciCmdData->idx] == CMD_READ_CHAR)
		{
			atciCmdData->cmdType = AT_CMD_READ_WITHOUT_PARAM;
			atciCmdData->idx++;
			break;
		}
		else if (atciCmdData->pComRxBuf->data[atciCmdData->idx] == CMD_PARAM_CHAR)
		{
			atciCmdData->cmdType = AT_CMD_WITH_PARAM_TO_GET;
			atciCmdData->idx++;
			break;
		}
		else if (atciCmdData->pComRxBuf->data[atciCmdData->idx] == CMD_LF_CHAR)
		{
			// bypass
			//atciCmdData->idx++;
		}
		else if ((atciCmdData->pComRxBuf->data[atciCmdData->idx] >= 'a') && (atciCmdData->pComRxBuf->data[atciCmdData->idx] <= 'z'))
		{
			atciCmdData->cmdCodeStr[cmdLen++] = UPPERCASE(atciCmdData->pComRxBuf->data[atciCmdData->idx]);
		}
		else
		{
			atciCmdData->cmdCodeStr[cmdLen++] = atciCmdData->pComRxBuf->data[atciCmdData->idx];
		}
	}

	atciCmdData->cmdCodeStr[cmdLen] = 0;

	if (cmdLen < AT_CMD_CODE_MIN_LEN)
	{
		return ATCI_ERR_CMD_LEN;
	}
	else
	{
		return ATCI_ERR_NONE;
	}
}

/*!
 * @static
 * @brief		Extract one command parameter from buffer (parameter is a 8, 16 or 32 bits integer)
 *
 * @param[in,out]	atciCmdData Pointer "atci_cmd_t" structure:
 * 					- buf [in]: received command as text from console
 * 					- len [in]: received command length
 * 					- idx [out]: index in command buffer for command parameters beginning (if any; reset at the beginning of this function)
 * 					- cmdType [in,out]: command type: if all parameters are read or not and if it is a read command or not (AT_CMD_WITHOUT_PARAM ... AT_CMD_READ_WITH_PARAM)
 * 					(other fields are used internally or unused)
 *					- params [in,out]: command parameters list, the first free slot is used (nbParams)
 *						- size [out]: type of value (PARAM_INT8, PARAM_INT16, PARAM_INT32)
 *						- val8, val16 or val32 [out]: parameter value (according to size)
 *					- nbParams [in,out]: IN: current parameter list index to write; OUT new number of parameters read (nbParams incremented in function)
 *
 * @param[in]	valType: parameter type: PARAM_INT8, PARAM_INT16 or PARAM_INT32 (8, 16 or 32 bits integer)
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR)
 *
 */
static
atci_error_e Atci_Buf_Get_Cmd_Param_Val(atci_cmd_t *atciCmdData, uint16_t valType)
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
		return ATCI_ERR_PARAM_NB;

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
	for(; atciCmdData->idx < atciCmdData->pComRxBuf->len; atciCmdData->idx++)
	{
		if(atciCmdData->pComRxBuf->data[atciCmdData->idx] == CMD_SEP_CHAR)
		{
			atciCmdData->cmdType = AT_CMD_WITH_PARAM_TO_GET;
			atciCmdData->idx++;
			break;
		}
		else if(atciCmdData->pComRxBuf->data[atciCmdData->idx] == CMD_READ_CHAR)
		{
			atciCmdData->cmdType = AT_CMD_READ_WITH_PARAM;
			atciCmdData->idx++;
			break;
		}
		//else
		switch(state)
		{
			case PARAM_WAIT_BEGIN:
				if(atciCmdData->pComRxBuf->data[atciCmdData->idx] == CMD_HEX_CHAR)
					state = PARAM_HEX;
				else if(atciCmdData->pComRxBuf->data[atciCmdData->idx] == CMD_NEG_CHAR)
					state = PARAM_DEC_NEG;
				else
				{
					data = decascii2nb(atciCmdData->pComRxBuf->data[atciCmdData->idx]);
					if(data != 0xFF)
					{
						state = PARAM_DEC;
						val = data;
					}
				}
				break;

			case PARAM_HEX:

				data = hexascii2nibble(atciCmdData->pComRxBuf->data[atciCmdData->idx]);
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

				data = decascii2nb(atciCmdData->pComRxBuf->data[atciCmdData->idx]);
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

				data = decascii2nb(atciCmdData->pComRxBuf->data[atciCmdData->idx]);
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
		return ATCI_ERR_PARAM_VAL;
	else if(state == PARAM_WAIT_BEGIN)
		return ATCI_ERR_PARAM_LEN;
	else if((state == PARAM_HEX) && (nbNibbles != 0))
		return ATCI_ERR_PARAM_LEN;
	else if((state == PARAM_DEC_NEG) && (val == 0))
		return ATCI_ERR_PARAM_VAL;
	else
	{
		if(state == PARAM_DEC_NEG)
			val = -val;

		switch(valType)
		{
			default:
			case PARAM_INT8:
				if(atciCmdData->paramsMemIdx >= AT_CMD_DATA_MAX_LEN)
					return ATCI_ERR_CMD_LEN;
				*(atciCmdData->params[atciCmdData->nbParams].val8) = (uint8_t) val;
				break;
			case PARAM_INT16:
				if(atciCmdData->paramsMemIdx > (AT_CMD_DATA_MAX_LEN-2))
					return ATCI_ERR_CMD_LEN;
				*(atciCmdData->params[atciCmdData->nbParams].val16) = __htons((uint16_t)val);
				break;
			case PARAM_INT32:
				if(atciCmdData->paramsMemIdx > (AT_CMD_DATA_MAX_LEN-2))
					return ATCI_ERR_CMD_LEN;
				*(atciCmdData->params[atciCmdData->nbParams].val32) = __htonl(val);
				break;
		}
		atciCmdData->params[atciCmdData->nbParams].size = valType;
		Atci_Add_Cmd_Param_Resp(atciCmdData);

		return ATCI_ERR_NONE;
	}
}

/*!
 * @static
 * @brief		Extract one command parameter from buffer (parameter is a bytes array)
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure:
 * 					- buf [in]: received command as text from console
 * 					- len [in]: received command length
 * 					- idx [out]: index in command buffer for command parameters beginning (if any; reset at the beginning of this function)
 * 					- cmdType [out]: command type: if all parameters are read or not and if it is a read command or not (AT_CMD_WITHOUT_PARAM ... AT_CMD_READ_WITH_PARAM)
 * 					(other fields are used internally or unused)
 *					- params [in,out]: command parameters list, the first free slot is used (nbParams)
 *						- size [out]: parameter length (in bytes)
 *						- array [out]: parameter data
 *					- nbParams [in,out]: IN: current parameter list index to write; OUT new number of parameters read (nbParams incremented in function)
 *
 * @param[in]	valLen 0x00 = array of variable length, 0x01~0x7F = array of this wanted length
 *
 * @return
 * 	- ATCI_ERR_NONE if succeed
 * 	- else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR)
 *
 */
static
atci_error_e Atci_Buf_Get_Cmd_Param_Array(atci_cmd_t *atciCmdData, uint16_t valLen)
{
	atci_param_state_t state = PARAM_WAIT_BEGIN;
	uint8_t data;
	uint16_t nbBytes = 0;

	if((atciCmdData->cmdType != AT_CMD_WITH_PARAM_TO_GET) || (atciCmdData->nbParams >= AT_CMD_MAX_NB_PARAM))
		return ATCI_ERR_PARAM_NB;

	atciCmdData->cmdType = AT_CMD_WITH_PARAM;
	for(; atciCmdData->idx < atciCmdData->pComRxBuf->len; atciCmdData->idx++)
	{
		if(atciCmdData->pComRxBuf->data[atciCmdData->idx] == CMD_SEP_CHAR)
		{
			atciCmdData->cmdType = AT_CMD_WITH_PARAM_TO_GET;
			atciCmdData->idx++;
			break;
		}
		else if(atciCmdData->pComRxBuf->data[atciCmdData->idx] == CMD_READ_CHAR)
		{
			atciCmdData->cmdType = AT_CMD_READ_WITH_PARAM;
			atciCmdData->idx++;
			break;
		}
		//else
		switch(state)
		{
			case PARAM_WAIT_BEGIN:
				if(atciCmdData->pComRxBuf->data[atciCmdData->idx] == CMD_HEX_CHAR)
					state = PARAM_HEX;
				break;

			case PARAM_HEX:

				data = hexascii2nibble(atciCmdData->pComRxBuf->data[atciCmdData->idx]);
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

				data = hexascii2nibble(atciCmdData->pComRxBuf->data[atciCmdData->idx]);
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
		return ATCI_ERR_PARAM_VAL;
	else if(state == PARAM_WAIT_BEGIN)
		return ATCI_ERR_PARAM_LEN;
	else if(state == PARAM_HEX_LSB)
		return ATCI_ERR_PARAM_LEN;
	else if((valLen != PARAM_VARIABLE_LEN) && (valLen != nbBytes))
		return ATCI_ERR_PARAM_LEN;
	else
	{
		atciCmdData->params[atciCmdData->nbParams].size = nbBytes;
		Atci_Add_Cmd_Param_Resp(atciCmdData);
		return ATCI_ERR_NONE;
	}
}

/******************************************************************************/

/*! @} */
