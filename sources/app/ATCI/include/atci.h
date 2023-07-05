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

#define ATKMAC_KEY_LEN			16
#define ATKENC_KEY_LEN			16
#define ATIDENT_MFIELD_LEN		2
#define ATIDENT_AFIELD_LEN		6

// GBI : This is deprecate. Replaced with a "get" from parameter table.
#define ATSEND_L7_MAX_MSG_LEN	102 //maximum length is 102 bytes of L7 data for PRES-EXCHANGE L6 frames (ATSEND command)

#define PARAM_VARIABLE_LEN	0
#define PARAM_INT8			0xFFF1
#define PARAM_INT16			0xFFF2
#define PARAM_INT32			0xFFF4
#define PARAM_STR			0x8000

#define IS_PARAM_INT(size)		((size & 0xFFF0)==0xFFF0)
#define PARAM_INT_SIZE(size)	(size & 0x000F)

#define IS_PARAM_STR(size)		((size & 0xF000)==0x8000)
#define PARAM_STR_SIZE(size)	(size & 0x0FFF)


// Factory calibration

#define FC_TX_PWR_0dB_ID		0x00
#define FC_TX_PWR_m6dB_ID		0x01
#define FC_TX_PWR_m12dB_ID		0x02
#define FC_PA_EN_ID				0x10
#define FC_RSSI_CAL_ID			0x20
#define FC_ADF7030_CAL_ID		0xFC

#define FC_TX_PWR_CFG_NB_VAL	3

// PWR_COARSE is coded on 4 bits
#define FC_TX_PWR_COARSE_MIN	1
#define FC_TX_PWR_COARSE_MAX	6
// PWR_FINE is coded on 7 bits
#define FC_TX_PWR_FINE_MIN		3
#define FC_TX_PWR_FINE_MAX		127
// PWR_MICRO is coded on 7 bits
#define FC_TX_PWR_MICRO_MIN		0 //1
#define FC_TX_PWR_MICRO_MAX		31

#define TEST_MODE_DIS			0x00
#define TEST_MODE_RX_0			0x10
#define TEST_MODE_RX_1			0x11

#define TEST_MODE_DEF_CH        PHY_CH120
#define TEST_MODE_DEF_MOD       PHY_WM2400

/*!
 * @}
 * @endcond
 */

/*==============================================================================
 * TYPEDEF
 *============================================================================*/


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

	ATCI_SESSION,  /*!<  */
	ATCI_COMMAND,  /*!<  */
} atci_state_t;


/*!
 * @brief This enum define the RX ATCI status
 */
typedef enum
{
	ATCI_RX_CMD_OK,       /*!<  */
	ATCI_RX_CMD_NONE,     /*!<  */
	ATCI_RX_CMD_ERR,      /*!<  */
	ATCI_RX_CMD_TIMEOUT,  /*!<  */
} atci_status_t;

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
	ATCI_ERR_RX_CMD        = 0x10, /*!< Internal error */
	ATCI_ERR_RX_TMO        = 0x11, /*!< Internal error */
	// --- Reserved for "user" error codes
	ATCI_ERR_USER_START    = 0x80, /*!< Reserved for "user" error codes */
	ATCI_ERR_USER_END      = 0xFE, /*!< Reserved for "user" error codes */
	// --- Generic "unknown" error
	ATCI_ERR_UNK           = 0xFF  /*!< Generic "unknown" error */
} atci_error_t;

/*!
 * @brief This enum define the ATCI command code
 */
typedef enum
{
	CMD_AT,      /*!<  */
	CMD_ATI,     /*!<  */
	CMD_ATZ,     /*!<  */
	CMD_ATQ,     /*!<  */
	CMD_ATF,     /*!<  */
	CMD_ATW,     /*!<  */
	CMD_ATPARAM, /*!<  */
	CMD_ATIDENT, /*!<  */
	CMD_ATSEND,  /*!<  */
	CMD_ATPING,  /*!<  */
	CMD_ATFC,    /*!<  */
	CMD_ATTEST,  /*!<  */
	// ----
#ifdef HAS_ATZn_CMD
	CMD_ATZ0,     /*!<  */
	CMD_ATZ1,     /*!<  */
#else
	CMD_ATZC,     /*!<  */
#endif
	// ----
#ifndef HAS_ATKEY_CMD
	CMD_ATKMAC,  /*!<  */
	CMD_ATKENC,  /*!<  */
#else
	CMD_ATKEY,   /*!<  */
#endif
	// ----
#ifdef HAS_ATSTAT_CMD
	CMD_ATSTAT,
#endif
	// ----
#ifdef HAS_ATCCLK_CMD
	CMD_ATCCLK,
#endif
	// ----
#ifdef HAS_ATUID_CMD
	CMD_ATUID,
#endif
	// ----
#ifdef HAS_LO_UPDATE_CMD
	CMD_ATANN,
	CMD_ATBLK,
	CMD_ATUPD,
#ifdef HAS_LO_ATBMAP_CMD
	CMD_ATBMAP,
#endif
#endif
	// ----
#ifdef HAS_EXTERNAL_FW_UPDATE
	CMD_ATADMANN,
#endif
	// ----
	NB_AT_CMD //used to get number of commands
} atci_cmd_code_t;

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
} atci_cmd_type_t;

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

/*!
 * @brief This enum define the ATCI command/response main structure
 */
typedef struct
{
	// --- used as uart input buffer
	uint8_t buf[AT_CMD_BUF_LEN];                  /*!< cmd data buffer used to receive cmd from UART */
	uint16_t len;                                 /*!< cmd length in buffer */
	// --- used to extract input command
	uint16_t idx;                                 /*!< read index in buffer */
	char cmdCodeStr[AT_CMD_CODE_MAX_LEN];         /*!< reformatted command code string */
	atci_cmd_code_t cmdCode;                      /*!< command code (decoded) */
	atci_cmd_type_t cmdType;                      /*!< if command is read or write and if it has parameters or not */
	// ---
	uint8_t paramsMem[AT_CMD_DATA_MAX_LEN];       /*!< buffer where parameters data are saved */
	uint16_t paramsMemIdx;                        /*!< 1st free byte in paramsMem */
	// ---
	uint8_t nbParams;                             /*!< number of command/response parameters */
	atci_cmd_param_t params[AT_CMD_MAX_NB_PARAM]; /*!< command/response parameters list (give a size and a pointer in paramsMem buffer for each parameters) */
} atci_cmd_t;

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
void Atci_Task(void const *argument);

#endif /* INC_ATCI_H_ */

/*! @} */
