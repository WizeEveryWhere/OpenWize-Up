/**
  * @file console.c
  * @brief This file contains functions for a console interface module.
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

/*=========================================================================================================
 * INCLUDES
 *=======================================================================================================*/

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "platform.h"

#include "console.h"

/*=========================================================================================================
 * GLOBAL VARIABLES
 *=======================================================================================================*/

/*! @cond INTERNAL @{ */

console_buf_t consoleTxBuf;
console_buf_t consoleRxBuf;

/*! @} @endcond */

/*=========================================================================================================
 * LOCAL FUNCTIONS PROTOTYPES
 *=======================================================================================================*/


/*=========================================================================================================
 * FUNCTIONS - RX
 *=======================================================================================================*/

/*!-----------------------------------------------------------------------------
 * @internal
 *
 * @brief		Init the UART console
 *
 * @endinternal
 *----------------------------------------------------------------------------*/
void Console_Init(const char cMatch, pfEvtCb_t const pfEvtCb, void *pCbParam)
{
	uint8_t ret;
	// ---
	ret = BSP_Uart_Init(UART_ID_COM, cMatch, UART_MODE_EOB);
	ret |= BSP_Uart_SetCallback(UART_ID_COM, pfEvtCb, pCbParam);
	assert(ret == DEV_SUCCESS);
}

/*!-----------------------------------------------------------------------------
 * @internal
 *
 * @brief		Enable the UART console
 *
 * @endinternal
 *----------------------------------------------------------------------------*/
void Console_Enable(void)
{
	BSP_Uart_Open(UART_ID_COM);
}

/*!-----------------------------------------------------------------------------
 * @internal
 *
 * @brief		Disable the UART console
 *
 * @endinternal
 *----------------------------------------------------------------------------*/
void Console_Disable(void)
{
	BSP_Uart_Close(UART_ID_COM);
}

/*==============================================================================
 * FUNCTIONS - TX
 *============================================================================*/


/*!-----------------------------------------------------------------------------
 * @internal
 *
 * @brief		Send byte to console
 *
 * @param[in]	data Byte to send
 *
 * @return		None
 *
 * @endinternal
 *----------------------------------------------------------------------------*/
void Console_Tx_Byte(uint8_t data)
{
	BSP_Console_Send(&data, 1);
}

/*!-----------------------------------------------------------------------------
 * @internal
 *
 * @brief		Send data as strings representing hexadecimal number to console
 *
 * @param[in]	data Data array to convert and send
 * @param[in]	len  Array length (in bytes)
 *
 * @return		None
 *
 * @endinternal
 *----------------------------------------------------------------------------*/
void Console_Send_Array_To_Hex_Ascii(uint8_t *data, uint16_t len)
{
	uint16_t i;

	if (len > (CONSOLE_BUF_LEN >> 1))
	{
		len = CONSOLE_BUF_LEN >> 1;
	}

	consoleTxBuf.len = 0;
	for (i = 0; i < len; i++)
	{
		consoleTxBuf.data[consoleTxBuf.len++] = nibble2hexascii(data[i] >> 4);
		consoleTxBuf.data[consoleTxBuf.len++] = nibble2hexascii(data[i] & 0x0F);
	}
	BSP_Console_Send(consoleTxBuf.data, consoleTxBuf.len);
}

/*!-----------------------------------------------------------------------------
 * @internal
 *
 * @brief		Send data as strings representing hexadecimal number to console
 *
 * @param[in]	data Data (1, 2 or 4 bytes integer) to convert and send
 * @param[in]	size Size of data (in bytes: 1, 2 or 4)
 *
 * @return		None
 *
 * @endinternal
 *----------------------------------------------------------------------------*/
void Console_Send_Nb_To_Hex_Ascii(uint32_t data, uint8_t size)
{
	consoleTxBuf.len = 0;

	switch(size)
	{
		case 4:
			consoleTxBuf.data[consoleTxBuf.len++] = nibble2hexascii((data>>28)&0xF);
			consoleTxBuf.data[consoleTxBuf.len++] = nibble2hexascii((data>>24)&0xF);
			consoleTxBuf.data[consoleTxBuf.len++] = nibble2hexascii((data>>20)&0xF);
			consoleTxBuf.data[consoleTxBuf.len++] = nibble2hexascii((data>>16)&0xF);
		case 2:
			consoleTxBuf.data[consoleTxBuf.len++] = nibble2hexascii((data>>12)&0xF);
			consoleTxBuf.data[consoleTxBuf.len++] = nibble2hexascii((data>>8)&0xF);
		default:
			consoleTxBuf.data[consoleTxBuf.len++] = nibble2hexascii((data>>4)&0xF);
			consoleTxBuf.data[consoleTxBuf.len++] = nibble2hexascii(data&0xF);
	}

	BSP_Console_Send(consoleTxBuf.data, consoleTxBuf.len);
}

/*!-----------------------------------------------------------------------------
 * @internal
 *
 * @brief		Send string to console
 *
 * @details		In not enough space in internal buffer, string is truncated
 *
 * @param[in]	str String to send
 *
 * @return		None
 *
 * @endinternal
 *----------------------------------------------------------------------------*/
void Console_Send_Str(char *str)
{
	consoleTxBuf.len = strlen(str);
	if(consoleTxBuf.len > CONSOLE_BUF_LEN)
		consoleTxBuf.len = CONSOLE_BUF_LEN;
	memcpy(consoleTxBuf.data, str, consoleTxBuf.len);

	BSP_Console_Send(consoleTxBuf.data, consoleTxBuf.len);
}

/*!-----------------------------------------------------------------------------
 * @internal
 *
 * @brief		Send formated string to console (like printf)
 *
 * @details		In not enough space in internal buffer, string is truncated
 *
 * @param[in]	format Formated string, followed by parameters
 *
 * @return		None
 *
 * @endinternal
 *----------------------------------------------------------------------------*/
void Console_Printf(char *format, ...)
{
	va_list argList;

	va_start(argList, format);
	vsnprintf((char *) consoleTxBuf.data, (CONSOLE_BUF_LEN - 1), format, argList);
	consoleTxBuf.len = strlen((char *) consoleTxBuf.data);
	va_end(argList);

	BSP_Console_Send(consoleTxBuf.data, consoleTxBuf.len);
}

/*==============================================================================
 * FUNCTIONS - Tools
 *============================================================================*/

//------------------------------------------------------------------------------
//	hexascii2nibble
//
// inputs: character ('0' to '9', 'a' to 'f', 'A' to 'F')
// outputs: /
// return : nibble value (0x00 to 0x0F or 0xFF in wrong character)
// Overview: used to convert ASCII character representing an hexadecimal number to its value
//
//------------------------------------------------------------------------------
uint8_t hexascii2nibble(uint8_t data)
{
	if((data >= '0') && (data <= '9'))
		return (data - '0');
	else if ((data >= 'A') && (data <= 'F'))
		return (data - 'A' + 0xA);
	else if ((data >= 'a') && (data <= 'f'))
		return (data - 'a' + 0xA);
	else
		return 0xFF;
}

//------------------------------------------------------------------------------
//	nibble2hexascii
//
// inputs: nibble value (0x00 to 0x0F)
// outputs: /
// return : character ('0' to '9', 'A' to 'F')
// Overview: used to convert a value to an ASCII charcater representing an hexadecimal
//
//------------------------------------------------------------------------------
uint8_t nibble2hexascii(uint8_t data)
{
	if(data <= 0x9)
		return (data + '0');
	else if (data <= 0xF)
		return (data - 0xA + 'A');
	else
		return ' ';
}

//------------------------------------------------------------------------------
//	decascii2nb
//
// inputs: character ('0' to '9)
// outputs: /
// return : value (0 to 9 or 0xFF in wrong character)
// Overview: used to convert ASCII character representing a decimal number to its value
//
//------------------------------------------------------------------------------
uint8_t decascii2nb(uint8_t data)
{
	if((data >= '0') && (data <= '9'))
		return (data - '0');
	else
		return 0xFF;
}

/*==============================================================================
 * LOCAL FUNCTIONS
 *============================================================================*/



/*********************************** EOF **************************************/

/*! @} */
