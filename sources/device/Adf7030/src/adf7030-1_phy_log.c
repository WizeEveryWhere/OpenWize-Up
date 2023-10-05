/**
  * @file adf7030-1_phy_log.c
  * @brief This file implement PHY error management convenient function.
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
  * @par 1.0.0 : 2020/04/23[GBI]
  * Initial version
  *
  *
  */
/*!
 * @addtogroup adf7030-1_phy
 * @ingroup device
 * @{
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "adf7030-1_phy_log.h"

/*!
   Instance error debug type definition.\n
*/
typedef struct adf7030_1_instance_error_debug_s
{
    uint8_t instance_error_id;
    const char * const * instance_error_str;
} adf7030_1_instance_error_debug_t;

/*!
   PHY Radio error debug type definition.\n
*/
typedef struct adf7030_1_phy_error_debug_s
{
    uint8_t phy_error_id;
    const char * const * phy_error_str;
} adf7030_1_phy_error_debug_t;


/*!
   Instance error debug messages array.\n
*/
const char * const instance_error_msgs[] =
{
    "ADF7030_1_SUCCESS -> The API is successful.",
    "ADF7030_1_FAILURE -> The API failed to complete the operation.",
    "ADF7030_1_HW_ERROR -> A hardware error was detected.",
    "ADF7030_1_INVALID_DEVICE_NUM -> The given device number is outside the number of instances supported by the driver.",
    "ADF7030_1_INVALID_HANDLE -> The given device handle is invalid.",
    "ADF7030_1_INSUFFICIENT_MEMORY -> The given memory is not sufficient to operate the device.",
    "ADF7030_1_NULL_POINTER -> One of the given pointer parameter points to NULL.",
    "ADF7030_1_UNALIGNED_MEM_PTR -> The given pointer to the device memory is not aligned to word boundary.",
    "ADF7030_1_DEVICE_ALREADY_OPENED -> The given device instance is already opened.",
    "ADF7030_1_DEVICE_NOT_OPENED -> The given device instance is not opened.",
    "ADF7030_1_INVALID_OPERATION -> The given operation is not permitted in the current state of the driver.",
    "ADF7030_1_DEVICE_ACCESS_FAILED -> Failed to read or write the device registers.",
    "ADF7030_1_GPIO_DEV_FAILED -> Failed to register or unregister callback with GPIO service.",
    "ADF7030_1_SPI_DEV_FAILED -> SPI device driver failure.",
    "ADF7030_1_SPI_DEV_POLL_EXPIRE -> SPI device driver polling timeout.",
    "ADF7030_1_SPI_COMM_FAILED -> SPI Communication device related failure.",
    "ADF7030_1_INVALID_PHY_CONFIGURATION -> The given PHY Radio configuration is invalid.",
    "ADF7030_1_PENDING_IO -> If trying to perform a read when another read/write in progress.",
    "ADF7030_1_DMA_NOT_SUPPORTED -> If trying to enable the DMA mode when communication device is TWI."
};

/*!
   PHY Radio hardware error debug messages array.\n
*/
const char * const phy_error_msgs[] =
{
    "SM_NOERROR -> No error",                                                                    // 0x00
    "SM_INVALID_STATE -> Requested state does not exist",                                         // 0x01
    "SM_TRANS_BLOCKED -> State transition has been denied",                                     // 0x02
    "SM_TIMEOUT -> State machine timeout",                                                         // 0x03
    "SM_ILLEGAL_SCRIPT_CMD -> Illegal script command",                                             // 0x04
    "SM_UNKNOWN_SPECIAL_CMD -> Unknown state machine special command",                             // 0x05
    "SM_UNPOPULATED_CUSTOM_CMD -> Special command is not configured",                             // 0x06
    "SM_PROFILE_CRC_INCORRECT -> Backup up configuration is not valid",                         // 0x07
    "SM_LPM_CRC_INCORRECT -> Not available ",                                                    // 0x08
    "SM_INVALID_SCRIPT_DATA -> Not available",                                                     // 0x09
    "SM_ROM_CRC_INCORRECT -> Not available",                                                     // 0x30
    "SM_PROFILE_CRC_CALCULATING -> Startup CRC calculation running (not really an error code)", // 0x31
    "HW_MAIN_PLL_VCO_CAL_FAILURE_COARSE_CAL_FAILED -> PLL course calibration failure",          // 0x11
    "HW_MAIN_PLL_VCO_CAL_FAILURE_AMP_CAL_FAILED -> PLL amplitude calibration failure",          // 0x12
    "HW_MAIN_PLL_VCO_CAL_FAILURE_FINE_CAL_FAILED -> PLL fine calibration failure",              // 0x13
    "HW_ANC_PLL_LOCK_FAILED -> Auxiliary PLL lock failure",                                     // 0x14
    "HW_MAIN_PLL_VCO_CAL_MODE_INVALID -> PLL calibration mode invalid",                         // 0x15
    "HW_TCXO_NOT_READY -> Temperature Compensated Crystal Oscillator failure",                  // 0x20
    "HW_XTAL_NOT_READY -> Crystal Oscillator failure",                                          // 0x21
    "HW_OCL_CAL_FAILED -> Offset calibration failure",                                          // 0x23
    "HW_DIV_BY_ZERO -> Hardware divide by zero",                                                // 0x24
    "HW_VCO_KV_CAL_SINGLE_SHOT_FAILED -> VCO KV calibration step failure",                      // 0x25
    "HW_VCO_KV_CAL_FINE_CAL_FAILED -> VCO KV fine calibration failure",                         // 0x26
    "HW_VCO_KV_CAL_AMP_CAL_FAILED -> VCO KV amplitude calibration failure",                     // 0x27
    "HW_VCO_KV_HFXTAL_INVALID -> VCO KV clock source invalid",                                  // 0x28
    "HW_VCO_KV_CAL_COARSE_CAL_FAILED -> VCO KV coarse calibration failure",                     // 0x29
    "HW_TEMP_SENSE_FAILED -> Temperature sensor failure",                                       // 0x32
    "HW_LFRC_CAL_FAILED -> Low frequency oscillator calibration failure",                       // 0x33
    "HW_HARD_FAULT_FAIL -> Firmware hard fault \0",                                             // 0x3F
	//               0x0A to 0x10
	//               0x16 to 0x1F
	//               0x22
	//               0x2A to 0x2F
	//               0x34 to 0x3E
	"HW_ERROR -> undetermined !"                                                                // 0xFF
};

/*!
   Instance error debug array.\n
*/
static const adf7030_1_instance_error_debug_t instance_error_id_dbg[NUM_INSTANCE_ERROR] =
{
    {ADF7030_1_SUCCESS, &(instance_error_msgs[0]) },
    {ADF7030_1_FAILURE, &(instance_error_msgs[1]) },
    {ADF7030_1_HW_ERROR, &(instance_error_msgs[2])},
    {ADF7030_1_INVALID_DEVICE_NUM, &(instance_error_msgs[3])},
    {ADF7030_1_INVALID_HANDLE, &(instance_error_msgs[4])},
    {ADF7030_1_INSUFFICIENT_MEMORY, &(instance_error_msgs[5])},
    {ADF7030_1_NULL_POINTER, &(instance_error_msgs[6])},
    {ADF7030_1_UNALIGNED_MEM_PTR, &(instance_error_msgs[7])},
    {ADF7030_1_DEVICE_ALREADY_OPENED, &(instance_error_msgs[8])},
    {ADF7030_1_DEVICE_NOT_OPENED, &(instance_error_msgs[9])},
    {ADF7030_1_INVALID_OPERATION, &(instance_error_msgs[10])},
    {ADF7030_1_DEVICE_ACCESS_FAILED, &(instance_error_msgs[11])},
    {ADF7030_1_GPIO_DEV_FAILED, &(instance_error_msgs[12])},
    {ADF7030_1_SPI_DEV_FAILED, &(instance_error_msgs[13])},
    {ADF7030_1_SPI_DEV_POLL_EXPIRE, &(instance_error_msgs[14])},
    {ADF7030_1_SPI_COMM_FAILED, &(instance_error_msgs[15])},
    {ADF7030_1_INVALID_PHY_CONFIGURATION, &(instance_error_msgs[16])},
    {ADF7030_1_PENDING_IO, &(instance_error_msgs[17])},
    {ADF7030_1_DMA_NOT_SUPPORTED, &(instance_error_msgs[18])}
};


/*!
   PHY Radio error debug array.\n
*/
static const adf7030_1_phy_error_debug_t phy_error_id_dbg[NUM_PHY_ERROR] =
{
        {0x00, &phy_error_msgs[0]},
        {0x01, &phy_error_msgs[1]},
        {0x02, &phy_error_msgs[2]},
        {0x03, &phy_error_msgs[3]},
        {0x04, &phy_error_msgs[4]},
        {0x05, &phy_error_msgs[5]},
        {0x06, &phy_error_msgs[6]},
        {0x07, &phy_error_msgs[7]},
        {0x08, &phy_error_msgs[8]},
        {0x09, &phy_error_msgs[9]},
        {0x30, &phy_error_msgs[10]},
        {0x31, &phy_error_msgs[11]},
        {0x11, &phy_error_msgs[12]},
        {0x12, &phy_error_msgs[13]},
        {0x13, &phy_error_msgs[14]},
        {0x14, &phy_error_msgs[15]},
        {0x15, &phy_error_msgs[16]},
        {0x20, &phy_error_msgs[17]},
        {0x21, &phy_error_msgs[18]},
        {0x23, &phy_error_msgs[19]},
        {0x24, &phy_error_msgs[20]},
        {0x25, &phy_error_msgs[21]},
        {0x26, &phy_error_msgs[22]},
        {0x27, &phy_error_msgs[23]},
        {0x28, &phy_error_msgs[24]},
        {0x29, &phy_error_msgs[25]},
        {0x32, &phy_error_msgs[26]},
        {0x33, &phy_error_msgs[27]},
        {0x3F, &phy_error_msgs[28]},
        {0xFF, &phy_error_msgs[29]}
};

/**
 * @brief      Check erroneous return code of the Radio driver functions
 *             and display extra informations
 *
 * @param [in] pDevice Pointer to the PHY device info structure
 *
 * @return     None
 */
void CheckReturn(adf7030_1_device_t* const pDevice )
{
    adf7030_1_spi_info_t* pSPIDevInfo = &(pDevice->SPIInfo);
    adf7030_1_res_e eResult = pSPIDevInfo->eXferResult;
    const char *pError = getErrMsg(pDevice);
    if(eResult == ADF7030_1_HW_ERROR)
    {
        DBG("[ERR] PHY 0x%02X : %s", pSPIDevInfo->ePhyError, pError);
    }
    else {
        DBG("[ERR] INST 0x%02X : %s", eResult, pError);
    }
}

/**
 * @brief     Get the current error string
 *
* @param [in] pDevice Pointer to the PHY device info structure
 *
 * @return    Current error string
 */
const char* getErrMsg(adf7030_1_device_t* const pDevice)
{
    adf7030_1_spi_info_t* pSPIDevInfo = &(pDevice->SPIInfo);
    adf7030_1_res_e eResult = pSPIDevInfo->eXferResult;

    //const char **ppError = instance_error_id[0].instance_error_str;
    const char * const *ppError = phy_error_id_dbg[29].phy_error_str;

    if(eResult == ADF7030_1_HW_ERROR)
    {
        for(int j=0; j < NUM_PHY_ERROR; j++)
        {
            if(phy_error_id_dbg[j].phy_error_id == pSPIDevInfo->ePhyError)
            {
                ppError = phy_error_id_dbg[j].phy_error_str;
                break;
            }
        }
    }
    else {
        /* Search throught Instance ERROR messages */
        for(int i=0; i < NUM_INSTANCE_ERROR; i++)
        {
            /* Display Instance ERROR message */
            if(instance_error_id_dbg[i].instance_error_id == (uint8_t)eResult)
            {
                ppError = instance_error_id_dbg[i].instance_error_str;
                break;
            }
        }
    }
       return (const char*)(*ppError);
}

#ifdef __cplusplus
}
#endif

/*! @} */
