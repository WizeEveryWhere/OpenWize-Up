/*!
 *****************************************************************************
  @file:    adf7030-1__common.h
  @brief:    ...
  @version:    $Revision:
  @date:    $Date:
 -----------------------------------------------------------------------------
Copyright (c) 2017, Analog Devices, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted (subject to the limitations in the disclaimer below) provided that
the following conditions are met:
  - Redistributions of source code must retain the above copyright notice, 
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation 
    and/or other materials provided with the distribution.
  - Neither the name of Analog Devices, Inc. nor the names of its contributors 
    may be used to endorse or promote products derived from this software without
    specific prior written permission.

NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY 
THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT 
NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
OF THE POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/

/** \addtogroup adf7030-1 adf7030-1 Driver
 *  @{
 */

/** \addtogroup adf7030-1__common Common
 *  @{
 */

#ifndef _ADF7030_1__COMMON_H_
#define _ADF7030_1__COMMON_H_

#include <bsp.h>
#include <stdint.h>
#include <stddef.h>
#include <machine/endian.h>

#include "adf7030-1_phy_fw.h"

#define ATTRIBUTE(x) __attribute__((x))
#define KEEP_VAR(var) var ATTRIBUTE(used)

/************* Radio Driver features ***************/

/*!
   Enable PHY Radio error reporting support in the driver code.\n
   1 -  To have the Error reporting code.\n
   0 -  To eliminate Error reporting code.
*/
#define ADF7030_1_PHY_ERROR_REPORT_ENABLE    1

/*!
   If PHY Radio error reporting is enabled, this select the reporting scheme to be used.\n
   1 -  IRQ Error reporting code.\n
   0 -  SPI Polling Error reporting code.
*/
#define ADF7030_1_PHY_ERROR_REPORT_SCHEME    0


/************* Radio Driver configurations ***************/

/*!
 *  Defines the number of ADF7030-1 Radio instances to support
 *  in the driver.
 */
#define ADF7030_1_NUM_INSTANCES             1

/*!
 *  Defines the number of interrupt pins connected between the
 *  MCU and the PHY Radio
 */
#define ADF7030_1_NUM_INT_PIN               2

/*!
 *  Defines the number of trigger pin connected between the
 *  MCU and the PHY Radio
 */
#define ADF7030_1_NUM_TRIG_PIN              2

/*!
 *  Defines size of SPI rx and tx buffers used to communicate with the PHY Radio.
 */
//#define ADF7030_1_SPI_BUFFER_SIZE 256u
// FIXME: increased form 256 to 300 due to "buffer" overflow of spi_CfgBuffer
#define ADF7030_1_SPI_BUFFER_SIZE 300u

/*!
 *  Defines the maximum size of an SPI transaction.
 *  The Radio drivers requires a minimum of 8bytes used for command framing
 *  during word transfer operations.
 *  For instance, a 256 bytes buffer allows:
 *  - up to 62 words (32bits) per SPI transaction.
 *  - up to 252 bytes (8bits) per SPI transaction.
 */
#define ADF7030_1_SPI_MAX_TRX_SIZE (ADF7030_1_SPI_BUFFER_SIZE - 8u)

/*!
 *  Defines the maximum numbers of byte transfert for which "Fast SPI mode" will
 *  be used. Above this number, standart SPI API is used.
 */
#define ADF7030_1_SPI_FAST_SIZE_THRESHOLD 20

/*!
 *  Defines the numbers of SPI pointers address the driver will keep to schedule
 *  SPI transactions.
 */
#define ADF7030_1_SPI_PNTR_NUM 8


/*! Enumeration of return codes from ADF7030_1 driver.
 *
 */
typedef enum
{
    /*! The API is successful. */
    ADF7030_1_SUCCESS,
    /*! The API failed to complete the operation. */
    ADF7030_1_FAILURE,
    /*! A hardware error was detected. */
    ADF7030_1_HW_ERROR,
    /*! The given device number is outside the number of instances supported by the driver.*/
    ADF7030_1_INVALID_DEVICE_NUM,
    /*! The given device handle is invalid. */
    ADF7030_1_INVALID_HANDLE,
    /*! The given memory is not sufficient to operate the device. */
    ADF7030_1_INSUFFICIENT_MEMORY,
    /*! One of the given pointer parameter points to NULL */
    ADF7030_1_NULL_POINTER,
    /*! The given pointer to the device memory is not aligned to word boundary. */
    ADF7030_1_UNALIGNED_MEM_PTR,
    /*! The given device instance is already opened. */
    ADF7030_1_DEVICE_ALREADY_OPENED,
    /*! The given device instance is not opened. */
    ADF7030_1_DEVICE_NOT_OPENED,
    /*! The given operation is not permitted in the current state of the driver. */
    ADF7030_1_INVALID_OPERATION,
    /*! Failed to read or write the device registers. */
    ADF7030_1_DEVICE_ACCESS_FAILED,
    /*! Failed to register or unregister callback with GPIO service */
    ADF7030_1_GPIO_DEV_FAILED,
    /*! SPI device driver failure. */
    ADF7030_1_SPI_DEV_FAILED,
    /*! SPI device driver polling timeout. */
    ADF7030_1_SPI_DEV_POLL_EXPIRE,
    /*! SPI Communication device related failure. */
    ADF7030_1_SPI_COMM_FAILED,
    /*! The given PHY Radio configuration is invalid */
    ADF7030_1_INVALID_PHY_CONFIGURATION,
    /*! If trying to perform a read when another read/write in progress. */
    ADF7030_1_PENDING_IO,
    /*! If trying to enable the DMA mode when communication device is TWI. */
    ADF7030_1_DMA_NOT_SUPPORTED
} adf7030_1_res_e;

/*! Enumeration of interrupts pins.
 *
 * The ADF7030-1 has two interrupt pin outs to the host processor. The host
 * can configure each of them independently using adf7030_1__IRQ_SetMap
 *
 * */
typedef enum
{
    /*! The interrupt output pin 0. */
    ADF7030_1_INTPIN0 = 0x00,
    /*! The interrupt output pin 1. */
    ADF7030_1_INTPIN1 = 0x01
} adf7030_1_intpin_e;

/*! Enumeration of external trigger pins.
 *
 * The ADF7030-1 has two interrupt pin which can trigger state transition
 * in place of a PHY Radio command through the SPI.
 *
 * */
typedef enum
{
    /*! The trigger intput pin 0. */
    ADF7030_1_TRIGPIN0 = 0x00,
    /*! The trigger intput pin 1. */
    ADF7030_1_TRIGPIN1 = 0x01
}  adf7030_1_trigpin_e;

/*! Enumeration of events generated by ADF7030_1. */
typedef enum
{
    /*! Interrupt is generated via interrupt pin0. */
    ADF7030_1_EVENT_INT0,
    /*! Interrupt is generated via interrupt pin1. */
    ADF7030_1_EVENT_INT1,
    /*! Host Driver Error */
    ADF7030_1_HOST_ERROR,
    /*! PHY operation Error */
    ADF7030_1_PHY_ERROR
} adf7030_1_event_e;

/*! Enumeration of different radio firmware states */
typedef enum {
    /*! Lowest current state */
    PHY_SLEEP  = 0x00,
    /*! Default startup state */
    PHY_OFF  = 0x01,
    /*! Standby state for TRX operations */
    PHY_ON   = 0x02,
    /*! Receive state */
    PHY_RX   = 0x03,
    /*! Transmit state */
    PHY_TX   = 0x04,
    /*! Configuring state */
    CFG_DEV  = 0x05,
    /*! Energy sniffing state (no rx possible)*/
    CCA      = 0x06,
	/* Prepares the program RAM for a firmware module download */
	// CMD_RAM_LOAD_INT (0x87), CMD_RAM_LOAD_DONE (0xC8)
	RAM_LOAD_INT = 0x07,
    /*! Calibrating state */
    DO_CAL   = 0x09,
    /*! Monitoring state */
    MON      = 0x0A,
    /*! Low frequency calibration state */
    LFRC_CAL = 0x0C,
    /*! GPIO Clock output state */
    GPCLK    = 0x10
} adf7030_1_radio_state_e;

/*! Enumeration of different phy radio error codes
 */
typedef enum {
    /*! No error */
    SM_NOERROR                                      = 0x00,
    /*! Requested state does not exist */
    SM_INVALID_STATE                                = 0x01,
    /*! State transition has been denied */
    SM_TRANS_BLOCKED                                = 0x02,
    /*! State machine timeout */
    SM_TIMEOUT                                      = 0x03,
    /*! Illegal script command */
    SM_ILLEGAL_SCRIPT_CMD                           = 0x04,
    /*! Unknown state machine special command */
    SM_UNKNOWN_SPECIAL_CMD                          = 0x05,
    /*! Special command is not configured */
    SM_UNPOPULATED_CUSTOM_CMD                       = 0x06,
    /*! Backup up configuration is not valid */
    SM_PROFILE_CRC_INCORRECT                        = 0x07,
    /*! Not available  */
    SM_LPM_CRC_INCORRECT                            = 0x08,
    /*! Not available */
    SM_INVALID_SCRIPT_DATA                          = 0x09,
    /*! Not available */
    SM_ROM_CRC_INCORRECT                            = 0x30,
    /*! Startup CRC calculation running (not really an error code) */
    SM_PROFILE_CRC_CALCULATING                      = 0x31,
    /*! PLL course calibration failure */
    HW_MAIN_PLL_VCO_CAL_FAILURE_COARSE_CAL_FAILED   = 0x11,
    /*! PLL amplitude calibration failure */
    HW_MAIN_PLL_VCO_CAL_FAILURE_AMP_CAL_FAILED      = 0x12,
    /*! PLL fine calibration failure */
    HW_MAIN_PLL_VCO_CAL_FAILURE_FINE_CAL_FAILED     = 0x13,
    /*! Auxiliary PLL lock failure */
    HW_ANC_PLL_LOCK_FAILED                          = 0x14,
    /*! PLL calibration mode invalid */
    HW_MAIN_PLL_VCO_CAL_MODE_INVALID                = 0x15,
    /*! Temperature Compensated Crystal Oscillator failure */
    HW_TCXO_NOT_READY                               = 0x20,
    /*! Crystal Oscillator failure */
    HW_XTAL_NOT_READY                               = 0x21,
    /*! Offset calibration failure */
    HW_OCL_CAL_FAILED                               = 0x23,
    /*! Hardware divide by zero */
    HW_DIV_BY_ZERO                                  = 0x24,
    /*! VCO KV calibration step failure */
    HW_VCO_KV_CAL_SINGLE_SHOT_FAILED                = 0x25,
    /*! VCO KV fine calibration failure */
    HW_VCO_KV_CAL_FINE_CAL_FAILED                   = 0x26,
    /*! VCO KV amplitude calibration failure */
    HW_VCO_KV_CAL_AMP_CAL_FAILED                    = 0x27,
    /*! VCO KV clock source invalid */
    HW_VCO_KV_HFXTAL_INVALID                        = 0x28,
    /*! VCO KV coarse calibration failure */
    HW_VCO_KV_CAL_COARSE_CAL_FAILED                 = 0x29,
    /*! Temperature sensor failure */
    HW_TEMP_SENSE_FAILED                            = 0x32,
    /*! Low frequency oscillator calibration failure */
    HW_LFRC_CAL_FAILED                              = 0x33,
    /*! Firmware hard fault */
    HW_HARD_FAULT_FAIL                              = 0x3F
} adf7030_1_radio_error_e;

/*! Enumeration of different firmware transition states */
typedef enum
{
    /*! PHY is transitionning between two state */
    TRANSITION  = 0,
    /*! PHY is within one of the "adf7030_1_radio_state_e" state */
    STATE       = 1,
    /*! PHY is waiting in OFF or ON state for a command */
    IDLE        = 2,
    /*! PHY is currently executing a calibration routine */
    CALIBRATION = 3
} adf7030_1_radio_transition_e;

/*! Enumeration of different radio firmware extended command */
typedef enum
{
    /*! Disable both PHY Radio ADF7030_1_TRIGPIN0 and ADF7030_1_TRIGPIN1 */
    TRIGPIN_NONE  = 0x0C,
    /*! Enable PHY Radio ADF7030_1_TRIGPIN0 and disable ADF7030_1_TRIGPIN1 */
    TRIGPIN0_ONLY = 0x0D,
    /*! Disable PHY ADF7030_1_TRIGPIN0 and enable ADF7030_1_TRIGPIN1 */
    TRIGPIN1_ONLY = 0x0E,
    /*! Enable both PHY Radio ADF7030_1_TRIGPIN0 and ADF7030_1_TRIGPIN1
     *  -- Does not work -- currently triggers pins are mutually exclusive
     *  only one trigger pin can be enabled at a time */
    TRIGPIN_ALL   = 0x0F
} adf7030_1_radio_extended_e;

/*! Structure to hold the current trigger pin status.*/
typedef enum
{
    /*! Host GPIO trigger is not configured*/
    NONE      = 0,
    /*! Host GPIO trigger has been configured */
    HOST_READY = 1,
    /*! PHY GPIO trigger has been setup */
    PHY_READY = 2,
    /*! Both Host and Phy are configured for GPIO trigger operation,
        but Radio PHY has not enabled the trigger */
    ENABLED   = 3
} adf7030_1_trig_status_e;

/*! Enumeration of different PHY Radio gpio configuration */
typedef enum
{
    /*! General purpose PHY Radio GPIO pin 0 */
    ADF7030_1_GPIO0 = 0,
    /*! General purpose PHY Radio GPIO pin 1 */
    ADF7030_1_GPIO1 = 1,
    /*! General purpose PHY Radio GPIO pin 2 */
    ADF7030_1_GPIO2 = 2,
    /*! General purpose PHY Radio GPIO pin 3 */
    ADF7030_1_GPIO3 = 3,
    /*! General purpose PHY Radio GPIO pin 4 */
    ADF7030_1_GPIO4 = 4,
    /*! General purpose PHY Radio GPIO pin 5 */
    ADF7030_1_GPIO5 = 5,
    /*! General purpose PHY Radio GPIO pin 6 */
    ADF7030_1_GPIO6 = 6,
    /*! General purpose PHY Radio GPIO pin 7 */
    ADF7030_1_GPIO7 = 7,
    /*! unmapped */
    ADF7030_1_GPIO_NONE = 0xF,
} adf7030_1_gpio_pin_e;

/*! Enumeration of different radio calibrations */
typedef enum {
    /*! Analog filter RC calibration */
    ANAFILT_RC_CAL      = 0x00000001,
    /*! ADC Notch calibration */
    ADC_NOTCH_CAL       = 0x00000010,
    /*! Analog QEC calibration */
    ANA_QEC_CAL         = 0x00000040,
    /*! Digital QEC calibration */
    DIG_QEC_CAL         = 0x00000100,
    /*! Anxillary PLL calibration */
    ANCPLL_CAL          = 0x00000400,
    /*! Low frequency RC calibration */
    LF_RC_CAL           = 0x00001000,
    /*! High frequency RC calibration */
    HF_RC_CAL           = 0x00004000,
    /*! VCO calibration */
    VCO_CAL             = 0x00010000,
    /*! VCO KV calibration */
    VCO_KV_CAL          = 0x00040000,
   /*! Xtal temperature compensation calibration */
    TEMP_XTAL_CAL       = 0x00100000,
    /*! Analog filter RC calibration */
    HF_XTAL             = 0x00400000,
    /*! Analog filter RC calibration */
    INLINE_OCL_CAL      = 0x40000000,
    //DEFAULT_CALS =
    //OFFLINE_CALS =
} adf7030_1_radio_cal_e;

typedef uint32_t adf7030_1_spi_pntr[ADF7030_1_SPI_PNTR_NUM];
//Forward Typedef declarartion
typedef struct data_blck_desc_s data_blck_desc_t;
typedef struct mem_desc_s       mem_desc_t;
typedef struct mem_cfg_desc_s   mem_cfg_desc_t;
typedef struct patch_desc_s     patch_desc_t;

/*! Structure to hold the information regarding the SPI PHY Status */
typedef struct adf7030_1_spi_status_s
{
    union {
        uint8_t  VALUE; /*!< SPI Slave Status */
        struct {
            uint8_t  MEM_ACC_STATUS : 1; /*!< *Instantaneous* value of an AHB
                                          read data underflow condition for
                                          memory reads. The same bit doubles up
                                          as an AHB bus error indicator for
                                          memory writes. */
            uint8_t  TRANS_STATUS   : 2; /*!< *Instantaneous* value of the
                                         firmware status of the radio controller
                                         state machine.
                                         - 0: transition in progress
                                         - 1: executing in a state
                                         - 2: idle in a state */
            uint8_t  ERR_STATUS     : 1; /*!< *Instantaneous* value of the
                                         firmware error (for example, an
                                         unsupported destination state or an
                                         internal error)
                                         - 0: no error
                                         - 1: error */
            uint8_t  RESERVED       : 1;
            uint8_t  CMD_READY      : 1; /*!< *Instantaneous* value of the
                                          indicator to the host that no posted
                                          (to the SPI Slave) radio controller
                                          command is currently awaiting servicing
                                          by the firmware radio state machine
                                          running on the Cortex-M0.
                                          - 0: not ready to receive a radio command
                                          - 1: ready to receive a radio command */
            uint8_t  IRQ_STATUS     : 1; /*!< *Instantaneous* value of the
                                          commoned-up (bitwise OR of the)
                                          external interrupt outputs from the
                                          ADF7030, made available in this format
                                          to the host for use in pin-limited
                                          systems.
                                          - 0: no pending interrupt condition
                                          - 1: pending interrupt condition */
            uint8_t  SPIS_READY     : 1; /*!< *Instantaneous* value of the
                                          confirmation to the external host that
                                          the HCLK clock domain in the ADF7030
                                          has power, is out of reset and has a
                                          running clock.
                                          - 0: SPI is not ready for access
                                          - 1: SPI is ready for access */
        }VALUE_b; /*!< BitSize*/
    };
} adf7030_1_spi_status_t;

/*! Structure to hold the information regarding the SPI device configuration */
typedef struct adf7030_1_spi_info_s
{
    /*! Pointer to the parent device instance information */
    void*                   hDevInfo; //adf7030_1_info_t
    /*! SPI device handle */
    void*                   hSPIDevice;
    /*! SPI TX transaction buffer */
    uint8_t*                pSPI_TX_BUFF;
    /*! SPI RX transaction buffer */
    uint8_t*                pSPI_RX_BUFF;
    /*! SPI clock frequency for default communication */
    uint32_t                nClkFreq;
    /*! SPI clock frequency for fast SPI transfer */
    uint32_t                nClkFreq_Fast;
    /*! Currently used SPI clock frequency */
    uint32_t                nClkFreq_Current;
    /*! SPI PHY pointer */
    adf7030_1_spi_pntr      PHY_PNTR;
    /*! SPI Driver communication result */
    adf7030_1_res_e         eXferResult;
    /*! SPI status from last transaction */
    adf7030_1_spi_status_t  nStatus;
    /*! PHY firmware state from last transaction */
    adf7030_1_radio_state_e nPhyState;
    /*! PHY firmware expected state */
    adf7030_1_radio_state_e nPhyNextState;


    /*! PHY Error Callback function pointer */
    pf_cb_t                 pfPhyErrCb;
    /*! Dynamic enable/disable of ePhyError checking */
    uint8_t                 bPhyErrorCheck;
    /*! PHY radio error code */
    adf7030_1_radio_error_e ePhyError;
}adf7030_1_spi_info_t;

/*! Structure to hold mapping between ADF7030_1 interrupt pin and host processor
 * GPIO pin.
 */
typedef struct adf7030_1_gpio_int_info_s
{
    /*! Host GPIO port to which the interrupt pin is connected */
    uint32_t             u32Port;
    /*! Host GPIO pin within the GPIO port */
    uint16_t             u16Pin;
    /*! PHY Radio GPIO pin */
    adf7030_1_gpio_pin_e ePhyPin;
    /*! Radio PHY interrupt mask configuration */
    uint32_t             nIntMap;
    /*! Last IRQ status */
    uint32_t             nIntStatus;
    /*! PHY Interrupt Callback function pointer */
	pf_cb_t              pfIntCb;
    /*! PHY Interrupt Callback parameter pointer */
	void*                pIntCbParam;

} adf7030_1_gpio_int_info_t;

/*! Structure to hold mapping between ADF7030_1 trigger pin and host processor
 * GPIO pin.
 */
typedef struct adf7030_1_gpio_trig_info_s
{
    /*! Host GPIO port to which the interrupt pin is connected */
    uint32_t                u32Port;
    /*! Host GPIO pin within the GPIO port */
    uint16_t                u16Pin;
    /*! PHY Radio GPIO pin */
    adf7030_1_gpio_pin_e    ePhyPin;
    /*! PHY Radio Command to execute on trigger */
    uint8_t                 nTrigCmd;
    /*! Current trigger status */
    adf7030_1_trig_status_e eTrigStatus;
} adf7030_1_gpio_trig_info_t;


/*! Structure to hold the mapping of the ADF7030_1 Hard Reset on the host
 *  processor GPIO pin.
 */
typedef struct adf7030_1_gpio_reset_info_s
{
    /*! Host GPIO port to which the interrupt pin is connected */
    uint32_t u32Port;
    /*! Host GPIO pin within the GPIO port */
    uint16_t u16Pin;
} adf7030_1_gpio_reset_info_t;

/*! Enumeration of different device or driver states
 */
typedef enum
{
    /*! The device is not yet opened */
    ADF7030_1_STATE_NOT_OPENED   = 0x00,

    /*! The device is opened */
    ADF7030_1_STATE_OPENED       = 0x01,
    /*! The device is opened, underlying drivers have been initialized */
    ADF7030_1_STATE_INITIALIZED  = 0x02,
    /*! The device is awake and its profile has been applied */
    ADF7030_1_STATE_CONFIGURED   = 0x04,
    /*! The device offline calibrations has been performed */
    ADF7030_1_STATE_CALIBRATED   = 0x08,

    /*! The device is awake and ready to perform Receive or Transmit operation */
    ADF7030_1_STATE_READY        = 0x10,
    /*! The device is in transmit state */
    ADF7030_1_STATE_TRANSMITTING = 0x20,
    /*! The device is in receive state */
    ADF7030_1_STATE_RECEIVING    = 0x40,
	/*! The device is in CCA state */
    ADF7030_1_STATE_NOISE_MEAS   = 0x80,
	ADF7030_1_STATE_BUSY         = ADF7030_1_STATE_TRANSMITTING | ADF7030_1_STATE_RECEIVING | ADF7030_1_STATE_NOISE_MEAS,

} adf7030_1_state_e;

/*! ADF7030_1 device instance data */
typedef struct adf7030_1_device_s
{
    /*! State of the driver */
    adf7030_1_state_e           eState;
    /*! SPI information for underlying driver */
    adf7030_1_spi_info_t        SPIInfo;
    /*! GPIO pin connection information for the Hard Reset pin */
    adf7030_1_gpio_reset_info_t ResetGPIOInfo;
    /*! GPIO pin connection information for Interrupt pin 1 and 2 */
    adf7030_1_gpio_int_info_t   IntGPIOInfo[ADF7030_1_NUM_INT_PIN];
    /*! GPIO pin connection information for Trigger pin 1 and 2 */
    adf7030_1_gpio_trig_info_t  TrigGPIOInfo[ADF7030_1_NUM_TRIG_PIN];
    /*! GPIO pin connection information for External PA control */
    adf7030_1_gpio_pin_e        eExtPaPin;
    /*! GPIO pin connection information for External LNA control */
    adf7030_1_gpio_pin_e        eExtLnaPin;

    /*! Internal: PREAMBLE+SYNCH is detected */
	uint8_t                     bDetected;
	/*! Internal : PHY CRC is configured  */
	uint8_t                     bCrcOn;
    /*! Internal : Configuration is completed */
	uint8_t                     bCfgDone;
	/*! Internal : TX power is configured */
	uint8_t                     bTxPwrDone;
	/*! Internal : Pending TX size */
	uint8_t                     u8PendTXBuffSize;
    /*! Internal : Set calibration parts */
	radio_cal_cfg0_t            CalCfg;

} adf7030_1_device_t ;

#endif /* _ADF7030_1__COMMON_H_ */

/** @} */ /* End of group adf7030-1__common */
/** @} */ /* End of group adf7030-1 adf7030-1 Driver */

