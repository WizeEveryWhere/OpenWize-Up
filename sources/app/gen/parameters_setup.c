/*!
  * @file parameters_setup.c
  * @brief This file was generated from sources/app/gen/.MergedParam.xml(Modif. : 2023-07-27 14:50:34.283474634 +0200).
  * 
  * @details
  *
  * @copyright 2023, GRDF, Inc.  All rights reserved.
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
  * @par Generation Date
  * 
  * @par x.x.x : 27/07/2023 14:50 [neo]
  *
  */

#include "parameters_def.h"


/******************************************************************************/

#ifndef ATTR_PARAM_TABLE
#define ATTR_PARAM_TABLE()
#endif
/*!
 * @brief This array define the parameter default value
 */
ATTR_PARAM_TABLE()
const uint8_t aParamSetup[] = {
   0x1,0x0, /*!< Hardware version number of the device (or transceiver for a remote module)*/
   0x1,0x0, /*!< Software version number run by the device (or transceiver for a remote module)*/
   0x0,0x0,0x0,0x0, /*!< Date/time of the last successful firmware download*/
   0x0, /*!< Version of the application layer*/
   0x64, /*!< Frequency channel to be used for all uplink message transmissions*/
   0x78, /*!< Frequency channel to be used for all message receptions (except firmware download)*/
   0x0, /*!< Modulation to be used for all uplink message transmissions*/
   0x0, /*!< Modulation to be used for all message receptions (except firmware download)*/
   0x0, /*!< Transceiver nominal transmission power*/
   0x0,0x0, /*!< Maximum time between two COMMAND messages before the device automatically returns to maximum transmission power*/
   0x0,0x0, /*!< Absolute transmission correction frequency offset*/
   0x5, /*!< Fixed wait time after transmission of a DATA message by the device and before opening the COMMAND message listening window*/
   0x8, /*!< Duration of the COMMAND message listening window by the device*/
   0x5, /*!< Time between reception of a COMMAND message by the device and transmission of the corresponding RESPONSE message*/
   0x0, /*!< Minimum value accepted for the EXCH_RESPONSE_DELAY parameter (defined by the device MANUFACTURER)*/
   0x50, /*!< Maximum length of application messages that can be sent by the device*/
   0x64, /*!< Maximum length of application messages that can be received by the device*/
   0x50,0xe2,0x27,0x0, /*!< Current time of device*/
   0x0,0x0, /*!< Relative correction (time delta) to be applied to the device clock once only to correct its absolute drift*/
   0x0,0x1, /*!< Correction of device clock frequency*/
   0x2, /*!< Current key number*/
   0xe, /*!< Number of encryption keys available in the device*/
   0x1, /*!< Kmac key index*/
   0xa, /*!< Fixed waiting time after transmission of an INSTPING message by the device and before opening the INSTPONG message listening window*/
   0x5, /*!< Duration of the INSTPONG message listening window by the device*/
   0x0, /*!< Minimum value of the PING_RX_DELAY parameter*/
   0x14, /*!< Maximum value of the PING_RX_LENGTH parameter*/
   0x0,0x0,0x0,0x0, /*!< Execution time of the last connectivity test (INSTPING/INSTPONG)*/
   0x0, /*!< Number of different INSTPONG messages received in response to the last connectivity test*/
   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0, /*!< Response 1 received for the last connectivity test (Best L7RssiDown)*/
   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0, /*!< Response 2 received for the last connectivity test*/
   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0, /*!< Response 3 received for the last connectivity test*/
   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0, /*!< Response 4 received for the last connectivity test*/
   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0, /*!< Response 5 received for the last connectivity test*/
   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0, /*!< Response 6 received for the last connectivity test*/
   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0, /*!< Response 7 received for the last connectivity test*/
   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0, /*!< Response 8 received for the last connectivity test (Lowest L7RssiDown)*/
   0x6, /*!< Periodic time of execping sending by the device, in months*/
   0x0, /*!< Get or Set the extend flags. b[0] if 1: Enable ATCI +DBG; b[1] : Reserved; b[2] if 1: Activate the immediate update when image is ready; b[3] if 1: Activate the WDT (bootcount for roll-back FW); b[4] : Reserved; b[5] if 1: Activate the phy calibration (rssi, power, internal) writing in NVM; b[6] if 1: Activate the device id writing in NVM; b[7] if 1: Activate the keys writing in NVM;*/
   0x0,0x7d, /*!< Clock and Frequency Offset Auto-Adjustment. MSB : control, LSB : RSSI min. level (see struct adm_config_s).*/
   0x0, /*!< ADM ANN Reception Filter disable (see struct adm_config_s)*/
   0x0, /*!< ADM R/W parameters Reception Filter disable (see struct adm_config_s)*/
   0x0, /*!< ADM Key Change Reception Filter disable (see struct adm_config_s)*/
   0x0,0xd,0x2f,0x0, /*!< Minimum delay between the AnnDownload and the day of the first block (see struct adm_config_s)*/
   0x0,0x4f,0x1a,0x0, /*!< Maximum delay between the AnnDownload and the day of the first block (see struct adm_config_s)*/
   0x0,0x0,0x38,0x40, /*!< Maintenance Window duration in second from 00:00:00 UTC (see struct adm_config_s)*/
   0x0,0x0,0x0,0xa, /*!< Minimum deltaSec accepted in second (see struct adm_config_s)*/
   0x0,0x0,0x3,0xe8, /*!< Duration of one block in ms (see struct adm_config_s)*/
   0x0,0x0,0x3,0x2e, /*!< Maximum number of block accepted (see struct adm_config_s)*/
   0x0,0x0, /*!< Offset to start RX before (in ms) (MSB first, Signed number limited to +/-63ms).*/
   0x0,0x0, /*!< Offset to start RX before (in ms) (MSB first, Signed number limited to +/-1000ms).*/
   0x0, /*!< L2 Reception Filter disable (see struct proto_config_s)*/
   0x0, /*!< L6 Reception Filter disable (see struct proto_config_s)*/
   0x78, /*!< Get or Set the current test mode channel*/
   0x0, /*!< Get or Set the current test mode modulation*/
   0x51, /*!< Get or Set the LowPower mode. b[0] : Enable/Disable, b[1:3] : Reserved, b[4:7] : Delay without activity to go in low power mode (0 : manual only).*/
   0x0, /*!< Get or Set the Logger level (see logger_level_e for details)*/
   0x0, /*!< Get or Set the Logger time (Unix EPOCH) option (see logger_tstamp_e for details). [b0] : Enable (1) / Disable (0); [b1] : If set : Extended with millisecond; [b2] : If set : Truncated on 5 digits; [b3-b6] Reserved; [b7] Enable/Disable Color.*/
   };

