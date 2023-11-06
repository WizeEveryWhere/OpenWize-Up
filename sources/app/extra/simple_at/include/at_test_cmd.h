/**
  * @file: at_test_cmd.h
  * @brief: // TODO This file ...
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
  * 1.0.0 : 2023/07/10[GBI]
  * Initial version
  *
  *
  */

/*!
 *  @addtogroup atci
 *  @ingroup app
 *  @{
 */

#ifndef AT_TEST_CMD_H_
#define AT_TEST_CMD_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "atci.h"

/*!
 * @brief		Execute ATFC command (set or get factory configuration)
 *
 * @details		This command may be a read or a write command:
 *
 * @li	"ATFC=<id>?" -> read configuration "id" (if parameter available)
 * @li	"ATPARAM=<id>,<value_1>,<value_2>,...,<value_n>" -> write one or more values to a configuration
 *
 * Read response format:
 * @li	"+ATPARAM:<id>,<value_1>,<value_2>,...,<value_n>"
 *
 * @parblock
 * @li	id is the configuration ID (decimal or hexadecimal 8 bits number)
 * @li	value_1, value_2... one or more values:  may be a 8, 16 or 32 bits integer (decimal or hexadecimal format may be used) or an array (hexadecimal format must be used)
 * @endparblock
 *
 * Configurations:
 * @parblock
 * <ul>
 * <li>	ADF7030 output power configuration: id, value_1, value_2, value_3
 * 		<ul>
 * 		<li> id
 * 			<ul>
 * 			<li> 0x00 : configuration for max power
 *			<li> 0x01 : configuration for 6dB under max power
 *			<li> 0x02 : configuration for 12dB under max power
 *			</ul>
 *		<li> value_1 : coarse PA settings, from 1 to 6 (8 bits integer, see ADF7030 Datasheet)
 *		<li> value_2 : fine PA settings from 0 to 255 (8 bits integer, see ADF7030 Datasheet)
 *		<li> value_3 : micro PA settings from 0 to 255 (8 bits integer, see ADF7030 Datasheet)
 * 		</ul>
 * <li>	Power amplifier (SKY66100-11) enable: id, value_1  (value_1 is 8 bits integer)
 *		<ul>
 *		<li> id : 0x10
  *		<li> value_1
 *			<ul>
 *			<li> 0 : Power amplifier is bypassed in TX
 *			<li> 1 : Power amplifier is enabled in TX
 *			</ul>
 *		</ul>
 * </li>
 * <li>	RSSI calibration (Apply Carrier at mid band frequency with -77dbm level): id
 *		<ul>
 *		<li> id : 0x20 : There is no parameter, read will return an error
 *		</ul>
 * </li>
 * <li>	Auto-Calibration: id
 * 		<ul>
 * 		<li> id : 0xFC : There is no parameter, read will return an error
 * 		</ul>
 * </ul>
 * @endparblock
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_t Exec_ATFC_Cmd(atci_cmd_t *atciCmdData);

/*!
 * @brief		Execute TEST command (enable or disable a test mode)
 *
 * @details		The received AT command "ATTEST=<test_mode>" where test_mode is one of :
 * @parblock
 * @li	0x00 : disable test mode (RX or TX test)
 * @li	0x01 : enable TX test mode, transmit a carrier
 * @li	0x02 : enable TX test mode, transmit frequency deviation tone, −fDEV, in 2FSK or off in OOK
 * @li	0x03 : enable TX test mode, transmit −fDEV_MAX in 4FSK only
 * @li	0x04 : enable TX test mode, transmit +fDEV in 2FSK or on in OOK
 * @li	0x05 : enable TX test mode, transmit +fDEV_MAX in 4FSK only
 * @li	0x06 : enable TX test mode, transmit transmit preamble pattern.
 * @li	0x07 : enable TX test mode, transmit pseudorandom (PN9) sequence
 * @li	0x10 : enable RX test mode, get copy of SPORT_CLK to EXT_I2C_SCL and SPORT_DATA to EXT_I2C_SDA
 * @li	0x11 : enable RX test mode, get PREAMBLE detect on EXT_I2C_SCL and SYNCH detect on EXT_I2C_SDA
 * @endparblock
 *
 * @param[in,out]	atciCmdData Pointer on "atci_cmd_t" structure
 *
 * @return
 * - ATCI_ERR_NONE if succeed
 * - Else error code (ATCI_INV_NB_PARAM_ERR ... ATCI_INV_CMD_LEN_ERR or ATCI_ERR)
 *
 */
atci_error_t Exec_ATTEST_Cmd(atci_cmd_t *atciCmdData);

#ifdef __cplusplus
}
#endif
#endif /* AT_TEST_CMD_H_ */

/*! @} */
