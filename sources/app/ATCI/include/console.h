/**********************************************************************************************************
  * @file: console.h
  * @brief: This file contains functions for a console interface
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

#ifndef INC_CONSOLE_H_
#define INC_CONSOLE_H_


/*=========================================================================================================
 * INCLUDES
 *=======================================================================================================*/


/*=========================================================================================================
 * DEFINES
 *=======================================================================================================*/


#define CONSOLE_TX_BUF_LEN		1024

//#define CONSOLE_RX_TIMEOUT_ms		5000

#define CONSOLE_RX_ERR			0xEF
#define CONSOLE_BYTE_RX			0x01
#define CONSOLE_RX_EMPTY		0x00
#define CONSOLE_TIMEOUT			0xE0


/*=========================================================================================================
 * TYPEDEF
 *=======================================================================================================*/

typedef struct
{
	uint16_t len;
	uint8_t data[CONSOLE_TX_BUF_LEN];
}console_tx_buf_t;


/*=========================================================================================================
 * FUNCTIONS PROTOTYPES - RX
 *=======================================================================================================*/

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Receive byte from console UART
 * 				This function is non blocking
 *
 * @param[In]	None
 * @param[Out]	data: byte received
 *
 * @return		status: CONSOLE_BYTE_RX if byte received, CONSOLE_RX_EMPTY if no byte received,
 * 					CONSOLE_RX_ERR if reception error
 *-------------------------------------------------------------------------------------------------------*/
uint8_t Console_Rx_Byte(uint8_t *data);

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Wait and receive byte from console UART
 * 				This function is blocking until a character is received or reception error
 *
 * @param[in]	None
 * @param[Out]	data: byte received
 *
 * @return		CONSOLE_BYTE_RX if byte received, CONSOLE_TIMEOUT if no byte received after a timeout time,
 * 					CONSOLE_RX_ERR if reception error
 *-------------------------------------------------------------------------------------------------------*/
uint8_t Console_Wait_Rx_Byte(uint8_t *data);

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Flush UART Receiption
 * 				This function is used to delete last received data
 *
 * @param[in]	None
 * @param[Out]	None
 *
 * @return		None
 *-------------------------------------------------------------------------------------------------------*/
void Console_Rx_Flush(void);


/*=========================================================================================================
 * FUNCTIONS PROTOTYPES - TX
 *=======================================================================================================*/


/*!--------------------------------------------------------------------------------------------------------
 * @brief		Send byte to console
 *
 * @param[in]	data: byte to send
 * @param[Out]	None
 *
 * @return		None
 *-------------------------------------------------------------------------------------------------------*/
void Console_Tx_Byte(uint8_t data);

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Send data to console
 *
 * @param[in]	data: data array to send
 * @param[in]	len: array length (in bytes)
 * @param[Out]	None
 *
 * @return		None
 *-------------------------------------------------------------------------------------------------------*/
void Console_Send(uint8_t *data, uint16_t len);

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Send data as strings representing hexadecimal number to console
 *
 * @param[in]	data: data array to convert and send
 * @param[in]	len: array length (in bytes)
 * @param[Out]	None
 *
 * @return		None
 *-------------------------------------------------------------------------------------------------------*/
void Console_Send_Array_To_Hex_Ascii(uint8_t *data, uint16_t len);

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Send data as strings representing hexadecimal number to console
 *
 * @param[in]	data: data (1, 2 or 4 bytes integer) to convert and send
 * @param[in]	size: size of data (in bytes: 1, 2 or 4)
 * @param[Out]	None
 *
 * @return		None
 *-------------------------------------------------------------------------------------------------------*/
void Console_Send_Nb_To_Hex_Ascii(uint32_t data, uint8_t size);

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Send string to console
 * 				In not enough space in internal buffer, string is truncated
 *
 * @param[in]	str: string to send
 * @param[Out]	None
 *
 * @return		None
 *-------------------------------------------------------------------------------------------------------*/
void Console_Send_Str(char *str);

/*!--------------------------------------------------------------------------------------------------------
 * @brief		Send formated string to console (like printf)
 * 				In not enough space in internal buffer, string is truncated
 *
 * @param[in]	format: formated string, followed by parameters
 * @param[Out]	None
 *
 * @return		None
 *-------------------------------------------------------------------------------------------------------*/
void Console_Printf(char *format, ...);


/*=========================================================================================================
 * FUNCTIONS PROTOTYPES - Tools
 *=======================================================================================================*/

//------------------------------------------------------------------------------
//	hexascii2nibble
//
// inputs: character ('0' to '9', 'a' to 'f', 'A' to 'F')
// outputs: /
// return : nibble value (0x00 to 0x0F or 0xFF in wrong character)
// Overview: used to convert ASCII character representing an hexadecimal number to its value
//
//------------------------------------------------------------------------------
uint8_t hexascii2nibble(uint8_t data);

//------------------------------------------------------------------------------
//	nibble2hexascii
//
// inputs: nibble value (0x00 to 0x0F)
// outputs: /
// return : character ('0' to '9', 'A' to 'F')
// Overview: used to convert a value to an ASCII charcater representing an hexadecimal
//
//------------------------------------------------------------------------------
uint8_t nibble2hexascii(uint8_t data);

//------------------------------------------------------------------------------
//	decascii2nb
//
// inputs: character ('0' to '9)
// outputs: /
// return : value (0 to 9 or 0xFF in wrong character)
// Overview: used to convert ASCII character representing a decimal number to its value
//
//------------------------------------------------------------------------------
uint8_t decascii2nb(uint8_t data);


#endif /* INC_CONSOLE_H_ */
/************************************************** EOF **************************************************/
