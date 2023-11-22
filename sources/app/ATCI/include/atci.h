/**
  * @file atci.h
  * @brief This file contains functions of the AT command interpreter for WizeUp
  * module
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

#ifndef INC_ATCI_H_
#define INC_ATCI_H_

#include "version.h"
#include "console.h"

/*==============================================================================
 * DEFINES
 *============================================================================*/

/*!
 * @cond INTERNAL
 * @{
 */

/*
 * (GBI) Note :
 * -----------------------------------------------------------------------------
 * In the Wize 1.2 specifications, exchange frame length is limited to :
 * - L2 : 127 + 1 (LField) bytes.
 * - L7 : 102 + 1 (L6App) bytes
 *
 * Then, ATCI ascii buffers size should be at least :
 * For : ATSEND=$xx,$yyyyyyyyy<CR>
 * - 2x( 102 (L7) + 1 (L6App) ) + 6 (ATSEND command) + 1 ('=') + 2 (2x'$') + 1 (',') + 1 ('\r')
 *   ---> 217 bytes
 *
 * -----------------------------------------------------------------------------
 * Technically, the Wize protocol is (should be) able to treat frame size at most :
 * For exchange frames :
 * - L2 : 254 + 1 (LField) bytes
 * - L7 : 229 bytes
 *
 * For download frames :
 * - L2 : 255 + 1 (LField) bytes
 * - L7 : 210 bytes
 *
 * Then, ATCI ascii buffers size should be at least :
 * For : ATSEND=$xx,$yyyyyyyyy<CR>
 * - 2x( 229 (L7) + 1 (L6App) ) + 6 (ATSEND command) + 1 ('=') + 2 (2x'$') + 1 (',') + 1 ('\r')
 *   ---> 471 bytes
 * -----------------------------------------------------------------------------
 */
#ifndef AT_CMD_DATA_MAX_LEN
#define AT_CMD_DATA_MAX_LEN		256	// 230// maximum length for all parameter of a command (after conversion; in bytes; note: maximum data is ATSEND_L7_MAX_MSG_LEN for ATSEND command)
#endif
#ifndef AT_CMD_BUF_LEN
#define AT_CMD_BUF_LEN			512 // 471 // maximum received command length (as text)
#endif

#define AT_CMD_CODE_MIN_LEN		2	// minimum command code length (reformatted text)
#define AT_CMD_CODE_MAX_LEN		16	// maximum command code length (reformatted text)
#define AT_CMD_MAX_NB_PARAM		6	// maximum number of parameters for a command/response

#define PARAM_INT 			0xF000
#define PARAM_INT8			0xF001
#define PARAM_INT16			0xF002
#define PARAM_INT32			0xF004
#define PARAM_STR			0x8000
#define PARAM_VARIABLE_LEN	0x0000

#define IS_PARAM_INT(size)		( (size & PARAM_INT) == PARAM_INT )
#define PARAM_INT_SIZE(size)	( size & ~(PARAM_INT) )

#define IS_PARAM_STR(size)		( (size & PARAM_INT) == PARAM_STR )
#define PARAM_STR_SIZE(size)	( size & ~(PARAM_INT) )

/*!
 * @}
 * @endcond
 */
/******************************************************************************/
/******************************************************************************/

/*!
 * @brief This enum define the ATCI state
 */
typedef enum
{
	ATCI_WAKEUP,   /*!<  */
	ATCI_WAIT,     /*!<  */
	ATCI_EXEC_CMD, /*!<  */
	ATCI_SLEEP,    /*!<  */
    ATCI_RESET,    /*!<  */
	ATCI_EXEC_RSP, /*!<  */

	ATCI_SESSION,  /*!<  */
	ATCI_COMMAND,  /*!<  */
} atci_state_t;

/*!
 * @brief This enum define the ATCI error code
 */
typedef enum
{
	// Success
	ATCI_ERR_NONE          = 0x00,  /*!< Success */
	//AT commands decoding error
	ATCI_ERR_PARAM_NB      = 0x01,	/*!< Invalid number of parameters for the current AT command */
	ATCI_ERR_PARAM_LEN     = 0x02,	/*!< Invalid parameter length */
	ATCI_ERR_PARAM_VAL     = 0x03,	/*!< Invalid parameter value */
	ATCI_ERR_CMD_UNK       = 0x04,	/*!< Unknown command code */
	ATCI_ERR_CMD_LEN       = 0x05,	/*!< Invalid command length (too short or too long to be decoded) */
	ATCI_ERR_CMD_FORBIDDEN = 0x06,	/*!< Command is forbidden in the current state */
	// --- Internal error
	ATCI_ERR_INTERNAL      = 0x10, /*!< Internal error */
	ATCI_ERR_RX_CMD        = 0x11, /*!< Internal error */
	ATCI_ERR_RX_TMO        = 0x12, /*!< Internal error */

	// --- Reserved for "user" error codes
	ATCI_ERR_USER_START    = 0x80, /*!< Reserved for "user" error codes */
	ATCI_ERR_USER_END      = 0xFE, /*!< Reserved for "user" error codes */
	// --- Generic "unknown" error
	ATCI_ERR_UNK           = 0xFF  /*!< Generic "unknown" error */
} atci_error_e;

/*!
 * @brief This enum define the ATCI command type and parameter decoding
 */
typedef enum
{
	AT_CMD_WITHOUT_PARAM,			/*!< command without parameter */
	AT_CMD_WITH_PARAM_TO_GET,		/*!< command with parameters (not all parameters decoded) */
	AT_CMD_WITH_PARAM,				/*!< command with parameters (all parameters decoded) */
	AT_CMD_READ_WITHOUT_PARAM,		/*!< read command without parameter */
	AT_CMD_READ_WITH_PARAM_TO_GET,	/*!< read command with parameters (not all parameters decoded) */
	AT_CMD_READ_WITH_PARAM			/*!< read command with parameters (all parameters decoded) */
} atci_cmd_type_e;

/*!
 * @brief This enum define the ATCI command/response parameters size and data pointer
 */
typedef struct
{
	uint16_t size; /*!< size/type:
	                    size=0~AT_PARAM_DATA_MAX_LEN:
	                    array of length size (in bytes),
	                    size=0xFFF0+n: integer of size n (n is 1, 2 or 4; in bytes)
	                    */
	union
	{  // data, val8, val16 and val32 point on the same area paramsMem;
		uint8_t *data;   /*!< Pointer used when data is an array */
		uint8_t *val8;   /*!< Pointer used when data is a 8 bits size */
		uint16_t *val16; /*!< Pointer used when data is a 16 bits size */
		uint32_t *val32; /*!< Pointer used when data is a 32 bits size */
		char *str;       /*!<  */
	};
} atci_cmd_param_t;


typedef struct at_desc_s at_desc_t;

/*!
 * @brief This enum define the ATCI command/response main structure
 */
typedef struct atci_cmd_s
{
	// --- used as uart input buffer
	console_buf_t *pComTxBuf; /*!< buffer used to send cmd to UART */
	console_buf_t *pComRxBuf; /*!< buffer used to receive cmd from UART */


	// --- used to extract input command
	uint16_t idx;                                 /*!< read index in buffer */
	char cmdCodeStr[AT_CMD_CODE_MAX_LEN];         /*!< reformatted command code string */
	struct at_desc_s * pCmdDesc;
	uint16_t cmd_code_nb;
	uint16_t cmdCode;                      /*!< command code (decoded) */
	atci_cmd_type_e cmdType;                      /*!< if command is read or write and if it has parameters or not */


	// ---
	uint8_t paramsMem[AT_CMD_DATA_MAX_LEN];       /*!< buffer where parameters data are saved */
	uint16_t paramsMemIdx;                        /*!< 1st free byte in paramsMem */
	// ---
	uint8_t nbParams;                             /*!< number of command/response parameters */
	atci_cmd_param_t params[AT_CMD_MAX_NB_PARAM]; /*!< command/response parameters list (give a size and a pointer in paramsMem buffer for each parameters) */


	// ---
	uint8_t bLpAllowed;
	uint8_t bNeedAck;
	uint8_t bNeedReboot;

	uint8_t bSession;

	atci_state_t eState;
	atci_error_e eErr;

	int32_t (*pf_inner_loop)(struct atci_cmd_s *pAtciCtx);

} atci_cmd_t;

/*!
 * @brief This struct define the ATCI command description
 */
struct at_desc_s
{
	atci_error_e (*pf)(atci_cmd_t *pAtciCtx);
	const char * const str;
};

typedef struct
{
	void *hAtciTsk;
	void *hUnsTsk;
	void *hAtciLock;
	void *hUnsQueue;

	atci_cmd_t sAtciCmd;

	console_buf_t *pComTxBuf;
	console_buf_t *pComRxBuf;

	// ---
	uint8_t bLpAllowed;
	uint8_t bSession;
	uint8_t bShouldAck;

	atci_state_t eState;
	atci_error_e eErr;
} atci_ctx_t;



/*==============================================================================
 * FUNCTIONS PROTOTYPES
 *============================================================================*/

/*!-----------------------------------------------------------------------------
 * @brief		AT command interpreter task
 *
 * @details		Wait AT command reception, decode and execute it
 * 				Manage sleep and reset
 *
 * @param[in]	argument: unused
 *
 *-----------------------------------------------------------------------------*/
void Atci_Setup(void);

int32_t Atci_Com(atci_cmd_t *pAtciCtx, uint32_t ulEvent);
int32_t Atci_Run(atci_cmd_t *pAtciCtx, uint32_t ulEvent);


int32_t UNS_Notify(uint32_t evt);
int32_t UNS_NotifyAtci(uint32_t evt);
int32_t UNS_NotifyTime(uint32_t evt);
int32_t UNS_NotifySession(uint32_t evt);

#endif /* INC_ATCI_H_ */

/*! @} */
