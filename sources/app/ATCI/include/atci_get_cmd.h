/**
  * @file atci_get_cmd.h
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

#ifndef ATCI_GET_CMD_H_
#define ATCI_GET_CMD_H_


/*==============================================================================
 * INCLUDES
 *============================================================================*/

/*==============================================================================
 * DEFINES
 *============================================================================*/

/*! @cond INTERNAL @{ */

#define END_OF_CMD_CHAR	0x0D //CR Carriage Return
#define BACK_SPACE_CHAR	0x08 //BS Backspace
#define CMD_AND_CHAR	'&'
#define CMD_READ_CHAR	'?'
#define CMD_PARAM_CHAR	'='
#define CMD_SEP_CHAR	','
#define CMD_HEX_CHAR	'$'
#define CMD_NEG_CHAR	'-'

#define IS_PRINTABLE_CHAR(c) (((c) >= 0x20) && ((c) <= 0x7E)) //from space to tilde
#define TO_MAG(c)		((c)-0x20)

/*! @} @endcond */

/*==============================================================================
 * TYPEDEF
 *============================================================================*/

typedef enum
{
	PARAM_WAIT_BEGIN,
	PARAM_HEX,
	PARAM_HEX_LSB,
	PARAM_DEC,
	PARAM_DEC_NEG,
	PARAM_ERR
} atci_param_state_t;


/*==============================================================================
 * FUNCTIONS PROTOTYPES - Command reception
 *============================================================================*/

/*!-----------------------------------------------------------------------------
 * @brief		Receive AT command from UART interface
 *
 * @details		This function is blocking until a character has been received by UART or an error occurred
 *
 * @param[out]	atciCmdData Pointer "atci_cmd_t" structure:
 * 					- buf [in,out]: buffer to receive command (as text) from console
 * 					- len [in,out]: actual received command length (other fields are unused)
 *
 * @retval ATCI_NO_AT_CMD if no cmd received
 * @retval ATCI_AVAIL_AT_CMD if full command received
 * @retval ATCI_RX_ERR if buffer overflow or RX error
 * @retval ATCI_RX_CMD_TIMEOUT if no characters received for a specified time
 *----------------------------------------------------------------------------*/
atci_status_t Atci_Rx_Cmd(atci_cmd_t *atciCmdData);

/*!-----------------------------------------------------------------------------
 * @brief		Clean reception in order to receive next command
 *
 * @param[in,out]	atciCmdData (Pointer on "atci_cmd_t" structure
 *
 *----------------------------------------------------------------------------*/
void Atci_Restart_Rx(atci_cmd_t *atciCmdData);

/*==============================================================================
 * FUNCTIONS - command decoding
 *============================================================================*/

/*!-----------------------------------------------------------------------------
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
 * 	- ATCI_OK if succeed
 * 	- else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR)
 *
 *----------------------------------------------------------------------------*/
atci_status_t Atci_Get_Cmd_Code(atci_cmd_t *atciCmdData);

/*!-----------------------------------------------------------------------------
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
 * 	- ATCI_OK if succeed
 * 	- else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR)
 *
 *----------------------------------------------------------------------------*/
atci_status_t Atci_Buf_Get_Cmd_Param(atci_cmd_t *atciCmdData, uint16_t valTypeSize);


/*==============================================================================
 * FUNCTIONS - command parameters memory management
 *============================================================================*/

/*!-----------------------------------------------------------------------------
 * @brief		Init 1st cmd data pointer and reset number of params
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 *----------------------------------------------------------------------------*/
void Atci_Cmd_Param_Init(atci_cmd_t *atciCmdData);

/*!-----------------------------------------------------------------------------
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
 * 	- ATCI_OK if succeed
 * 	- else error code (ATCI_ERR)
 *
 *----------------------------------------------------------------------------*/
atci_status_t Atci_Add_Cmd_Param_Resp(atci_cmd_t *atciCmdData);

/*!-----------------------------------------------------------------------------
 * @brief		Update last param length and update next param pointer
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 * @param[in] 	    newSize     The size of parameter
 *
 * @return
 * 	- ATCI_OK if succeed
 * 	- else error code (ATCI_ERR)
 *
 *----------------------------------------------------------------------------*/
atci_status_t Atci_Update_Cmd_Param_len(atci_cmd_t *atciCmdData, uint16_t newSize);



#endif /* ATCI_GET_CMD_H_ */
/*********************************** EOF **************************************/

/*! @} */
