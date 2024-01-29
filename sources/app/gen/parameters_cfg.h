/*!
  * @file parameters_cfg.h
  * @brief This file was generated from sources/app/gen/.MergedParam.xml(Modif. : 2023-12-13 09:10:45.947396839 +0100).
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
  * @par x.x.x : 13/12/2023 09:10 [neo]
  *
  */


#ifndef _PARAMETERS_ID_H_
#define _PARAMETERS_ID_H_
#ifdef __cplusplus
extern C {
#endif

#include "parameters_def.h"

/******************************************************************************/
#define PARAM_ACCESS_CFG_SZ (0xFF)
#define PARAM_DEFAULT_SZ (0xA2)

extern const param_s a_ParamAccess[PARAM_ACCESS_CFG_SZ];
extern const uint8_t a_ParamDefault[PARAM_DEFAULT_SZ];

/*!
 * @brief This enum define the parameter id
 */
typedef enum {
    VERS_HW_TRX = 0x01, /*!< Hardware version number of the device (or transceiver for a remote module) */
    VERS_FW_TRX = 0x02, /*!< Software version number run by the device (or transceiver for a remote module) */
    DATEHOUR_LAST_UPDATE = 0x03, /*!< Date/time of the last successful firmware download */
    L6App = 0x04, /*!< Version of the application layer */
    RF_UPLINK_CHANNEL = 0x08, /*!< Frequency channel to be used for all uplink message transmissions */
    RF_DOWNLINK_CHANNEL = 0x09, /*!< Frequency channel to be used for all message receptions (except firmware download) */
    RF_UPLINK_MOD = 0x0A, /*!< Modulation to be used for all uplink message transmissions */
    RF_DOWNLINK_MOD = 0x0B, /*!< Modulation to be used for all message receptions (except firmware download) */
    TX_POWER = 0x10, /*!< Transceiver nominal transmission power */
    TX_DELAY_FULLPOWER = 0x11, /*!< Maximum time between two COMMAND messages before the device automatically returns to maximum transmission power */
    TX_FREQ_OFFSET = 0x12, /*!< Absolute transmission correction frequency offset */
    EXCH_RX_DELAY = 0x18, /*!< Fixed wait time after transmission of a DATA message by the device and before opening the COMMAND message listening window */
    EXCH_RX_LENGTH = 0x19, /*!< Duration of the COMMAND message listening window by the device */
    EXCH_RESPONSE_DELAY = 0x1A, /*!< Time between reception of a COMMAND message by the device and transmission of the corresponding RESPONSE message */
    EXCH_RESPONSE_DELAY_MIN = 0x1B, /*!< Minimum value accepted for the EXCH_RESPONSE_DELAY parameter (defined by the device MANUFACTURER) */
    L7TRANSMIT_LENGTH_MAX = 0x1C, /*!< Maximum length of application messages that can be sent by the device */
    L7RECEIVE_LENGTH_MAX = 0x1D, /*!< Maximum length of application messages that can be received by the device */
    CLOCK_CURRENT_EPOC = 0x20, /*!< Current time of device */
    CLOCK_OFFSET_CORRECTION = 0x21, /*!< Relative correction (time delta) to be applied to the device clock once only to correct its absolute drift */
    CLOCK_DRIFT_CORRECTION = 0x22, /*!< Correction of device clock frequency */
    CIPH_CURRENT_KEY = 0x28, /*!< Current key number */
    CIPH_KEY_COUNT = 0x29, /*!< Number of encryption keys available in the device */
    L6NetwIdSelect = 0x2A, /*!< Kmac key index */
    PING_RX_DELAY = 0x30, /*!< Fixed waiting time after transmission of an INSTPING message by the device and before opening the INSTPONG message listening window */
    PING_RX_LENGTH = 0x31, /*!< Duration of the INSTPONG message listening window by the device */
    PING_RX_DELAY_MIN = 0x32, /*!< Minimum value of the PING_RX_DELAY parameter */
    PING_RX_LENGTH_MAX = 0x33, /*!< Maximum value of the PING_RX_LENGTH parameter */
    PING_LAST_EPOCH = 0x34, /*!< Execution time of the last connectivity test (INSTPING/INSTPONG) */
    PING_NBFOUND = 0x35, /*!< Number of different INSTPONG messages received in response to the last connectivity test */
    PING_REPLY1 = 0x36, /*!< Response 1 received for the last connectivity test (Best L7RssiDown) */
    PING_REPLY2 = 0x37, /*!< Response 2 received for the last connectivity test */
    PING_REPLY3 = 0x38, /*!< Response 3 received for the last connectivity test */
    PING_REPLY4 = 0x39, /*!< Response 4 received for the last connectivity test */
    PING_REPLY5 = 0x3A, /*!< Response 5 received for the last connectivity test */
    PING_REPLY6 = 0x3B, /*!< Response 6 received for the last connectivity test */
    PING_REPLY7 = 0x3C, /*!< Response 7 received for the last connectivity test */
    PING_REPLY8 = 0x3D, /*!< Response 8 received for the last connectivity test (Lowest L7RssiDown) */
    EXECPING_PERIODE = 0x3E, /*!< Periodic time of execping sending by the device, in months */
    EXTEND_FLAGS = 0xD0, /*!< Get or Set the extend flags. b[0] if 1: Enable ATCI +DBG; b[1] : Reserved; b[2] if 1: Activate the immediate update when image is ready; b[3] : Reserved; b[4] : Reserved; b[5] if 1: Activate the phy calibration (rssi, power, internal) writing in NVM; b[6] if 1: Activate the device id writing in NVM; b[7] if 1: Activate the keys writing in NVM; */
    LOCAL_KEY_ID = 0xD1, /*!< Get or Set the local key id. */
    BOOT_COUNT = 0xD2, /*!< Get the current boot counter value. */
    BOOT_COUNT_MAX = 0xD3, /*!< Get or Set the maximum value of boot counter to rollback. */
    AUTO_ADJ_CLK_FREQ = 0xDA, /*!< Clock and Frequency Offset Auto-Adjustment (see struct adm_config_s). MSB : control, b[0] : Coarse clock auto-adjust on PONG. 1: enable, 0: disable b[1] : Reserved b[2] : Frequency Offset auto-adjust on PONG. 1: enable, 0: disable b[3] : Reserved b[4] : Coarse clock auto-adjust on PONG appliance 1: One Shot, 0: Every time b[5] : Adjust error due to the Gateway on clock auto-adjust 1: Enable, 0: Disable b[6] : Frequency Offset auto-adjust on PONG appliance. 1: One Shot, 0: Every time b[7] : Reserved LSB : RSSI min. level. */
    ADM_ANN_DIS_FLT = 0xDD, /*!< ADM ANN Reception Filter disable (see struct adm_config_s) b[0] : Day Prog. filter. 0: enable, 1: disable b[1] : Delta Second filter. 0: enable, 1: disable b[2] : HW ver.filter. 0: enable, 1: disable b[3] : SW Ver. Initial filter. 0: enable, 1: disable b[4] : SW Ver. Target filter. 0: enable, 1: disable b[5] : Max. Block Number filter. 0: enable, 1: disable b[6:7] : Reserved */
    ADM_PARAM_DIS_FLT = 0xDE, /*!< ADM R/W parameters Reception Filter disable (see struct adm_config_s) b[0] : Read parameter WO filter. 0: enable, 1: disable b[1] : Write parameter RO filter. 0: enable, 1: disable b[2:7] : Reserved */
    ADM_KEY_DIS_FLT = 0xDF, /*!< ADM Key Change Reception Filter disable (see struct adm_config_s) b[0] : Write Key id filter. 0: enable, 1: disable b[1] : Key chg filter. 0: enable, 1: disable b[2:7] : Reserved */
    DWN_DAY_PRG_WIN_MIN = 0xE0, /*!< Minimum delay in second between the AnnDownload and the day of the first block (see struct adm_config_s) */
    DWN_DAY_PRG_WIN_MAX = 0xE1, /*!< Maximum delay in second between the AnnDownload and the day of the first block (see struct adm_config_s) */
    MNT_WINDOW_DURATION = 0xE2, /*!< Maintenance Window duration in second from 00:00:00 UTC (see struct adm_config_s). Set to 0, means no maintenance Window. */
    DWN_DELTA_SEC_MIN = 0xE3, /*!< Minimum deltaSec accepted in second (see struct adm_config_s) */
    DWN_BLK_DURATION_MOD = 0xE4, /*!< Duration of one block in ms (see struct adm_config_s) */
    DWN_BLK_NB_MAX = 0xE5, /*!< Maximum number of block accepted (see struct adm_config_s) */
    ADM_RECEPTION_OFFSET = 0xEA, /*!< Offset to start RX before (in ms) (MSB first, Signed number limited to +/-63ms). */
    DWN_RECEPTION_OFFSET = 0xEB, /*!< Offset to start RX before (in ms) (MSB first, Signed number limited to +/-1000ms). */
    L2_EXCH_DIS_FLT = 0xEE, /*!< L2 Reception Filter disable (see struct proto_config_s) b[0] : Crc filter. 0: enable, 1: disable b[1] : Afield filter. 0: enable, 1: disable b[2] : MField filter. 0: enable, 1: disable b[3] : CiFiled filter. 0: enable, 1: disable b[4] : DownId filter. 0: enable, 1: disable b[5:7] : Reserved */
    L6_EXCH_DIS_FLT = 0xEF, /*!< L6 Reception Filter disable (see struct proto_config_s) b[0] : Reserved b[1] : HashKmac filter. 0: enable, 1: disable b[2] : HashKenc filter. 0: enable, 1: disable b[3] : L6NetId filter. 0: enable, 1: disable b[4:7] : Reserved */
    TEST_MODE_RSSI_CAL_REF = 0xF9, /*!< Get or Set the input RF power (dB) applied to calibrate the RSSI. */
    TEST_MODE_CHANNEL = 0xFA, /*!< Get or Set the current test mode channel */
    TEST_MODE_MODULATION = 0xFB, /*!< Get or Set the current test mode modulation */
    LOW_POWER_MODE = 0xFC, /*!< Get or Set the LowPower mode. b[0] : Enable/Disable, b[1:3] : Reserved, b[4:7] : Delay (in second) without activity to go in low power mode (0 : manual only). */
    LOGGER_LEVEL = 0xFD, /*!< Get or Set the Logger level (see logger_level_e for details) */
    LOGGER_TIME_OPT = 0xFE, /*!< Get or Set the Logger time (Unix EPOCH) option (see logger_tstamp_e for details). [b0] : Enable (1) / Disable (0); [b1] : If set : Extended with millisecond; [b2] : If set : Truncated on 5 digits; [b3-b6] Reserved; [b7] : Enable/Disable Color. */
    LAST_ID = 0xFE, /*!< Don't remove, it marks the end of table. */
}param_ids_e;

/******************************************************************************/
#define PARAM_RESTR_CFG_SZ (0x7)

extern const restr_s a_ParamRestr[PARAM_RESTR_CFG_SZ];

extern const uint8_t a_ENUM_01[6];
extern const uint8_t a_RANGE_02[2];
extern const uint8_t a_RANGE_03[2];
extern const uint8_t a_RANGE_04[2];
extern const uint8_t a_RANGE_05[2];
extern const uint8_t a_RANGE_06[2];

#ifdef __cplusplus
}
#endif
#endif /* _PARAMETERS_ID_H_ */
