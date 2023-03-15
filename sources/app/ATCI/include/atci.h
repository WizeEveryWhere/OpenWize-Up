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
#define AT_CMD_BUF_LEN			512 // 471 // maximum received command length (as text)
#define AT_CMD_CODE_MIN_LEN		2	// minimum command code length (reformatted text)
#define AT_CMD_CODE_MAX_LEN		16	// maximum command code length (reformatted text)
#define AT_CMD_MAX_NB_PARAM		6	// maximum number of parameters for a command/response
#define AT_CMD_DATA_MAX_LEN		256	// 230// maximum length for all parameter of a command (after conversion; in bytes; note: maximum data is ATSEND_L7_MAX_MSG_LEN for ATSEND command)

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

#define FC_TX_PWR_COARSE_MIN	1
#define FC_TX_PWR_COARSE_MAX	6
#define FC_TX_PWR_FINE_MIN		3
#define FC_TX_PWR_FINE_MAX		127
#define FC_TX_PWR_MICRO_MIN		1
#define FC_TX_PWR_MICRO_MAX		31

#define TEST_MODE_DIS			0x00
#define TEST_MODE_RX_0			0x10
#define TEST_MODE_RX_1			0x11

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
	ATCI_WAKEUP,    /*!<  */
	ATCI_WAIT,      /*!<  */
	ATCI_EXEC_CMD,  /*!<  */
	ATCI_SLEEP,     /*!<  */
    ATCI_RESET      /*!<  */
} atci_state_t;


/*!
 * @brief This enum define the ATCI status
 */
typedef enum
{
	//success
	ATCI_OK,                /*!< Success */
	//AT commands decoding error
	ATCI_ERR_INV_NB_PARAM,	/*!< Invalid number of parameters for the current AT command */
	ATCI_ERR_INV_PARAM_LEN,	/*!< Invalid parameter length */
	ATCI_ERR_INV_PARAM_VAL,	/*!< Invalid parameter value */
	ATCI_ERR_UNKNOWN_CMD,	/*!< Unknown command code */
	ATCI_ERR_INV_CMD_LEN,	/*!< Invalid command length (too short or too long to be decoded) */
	//AT commands reception status
	ATCI_AVAIL_AT_CMD,      /*!<  */
	ATCI_NO_AT_CMD,         /*!<  */
	ATCI_RX_CMD_ERR,        /*!<  */
	ATCI_RX_CMD_TIMEOUT,    /*!<  */
	//generic error
	ATCI_ERR = 0xFF         /*!<  */
} atci_status_t;

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
	CMD_ATKMAC,  /*!<  */
	CMD_ATKENC,  /*!<  */
	CMD_ATIDENT, /*!<  */
	CMD_ATSEND,  /*!<  */
	CMD_ATPING,  /*!<  */
	CMD_ATFC,    /*!<  */
	CMD_ATTEST,  /*!<  */
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
	uint8_t buf[AT_CMD_BUF_LEN];                  /*!< cmd data buffer used to receive cmd from UART */
	uint16_t len;                                 /*!< cmd length in buffer */
	uint16_t idx;                                 /*!< read index in buffer */
	char cmdCodeStr[AT_CMD_CODE_MAX_LEN];         /*!< reformatted command code string */
	atci_cmd_code_t cmdCode;                      /*!< command code (decoded) */
	atci_cmd_type_t cmdType;                      /*!< if command is read or write and if it has parameters or not */
	uint8_t nbParams;                             /*!< number of command/response parameters */
	atci_cmd_param_t params[AT_CMD_MAX_NB_PARAM]; /*!< command/response parameters list (give a size and a pointer in paramsMem buffer for each parameters) */
	uint8_t paramsMem[AT_CMD_DATA_MAX_LEN];       /*!< buffer where parameters data are saved */
	uint16_t paramsMemIdx;                        /*!< 1st free byte in paramsMem */
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
