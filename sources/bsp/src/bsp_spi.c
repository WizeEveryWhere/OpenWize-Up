/*!
  * @file bsp_spi.c
  * @brief This file contains functions to deal with SPI.
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
  * @par 1.0.0 : 2019/12/14 [GBI]
  * Initial version
  *
  *
  */

/*!
 * @addtogroup OpenWize'Up_bsp
 * @{
 */

#include "bsp_spi.h"
#include "platform.h"
#include <stm32l4xx_hal.h>

/*!
 * @cond INTERNAL
 * @{
 */

#ifndef SPI_RX_TIMEOUT
	#define SPI_RX_TIMEOUT 1000
#endif
#ifndef SPI_TX_TIMEOUT
	#define SPI_TX_TIMEOUT 1000
#endif

/*!
 * @}
 * @endcond
 */

extern SPI_HandleTypeDef *paSPI_BusHandle[SPI_ID_MAX];

static uint32_t _get_SPI_freq_(void);
static uint8_t _get_APB_div_(void);
static uint32_t _get_SPI_freq_(void);

/*!
  * @static
  * @brief This hold the SPI prescaler table
  */
static const uint32_t prescaler_table[] =
{
	SPI_BAUDRATEPRESCALER_2,
	SPI_BAUDRATEPRESCALER_4,
	SPI_BAUDRATEPRESCALER_8,
	SPI_BAUDRATEPRESCALER_16,
	SPI_BAUDRATEPRESCALER_32,
	SPI_BAUDRATEPRESCALER_64,
	SPI_BAUDRATEPRESCALER_128,
	SPI_BAUDRATEPRESCALER_256
};

/*!
  * @static
  * @brief Get the AHB divisor
  *
  * @return the divisor value
  *
  */
static uint16_t _get_AHB_div_(void){
	uint32_t cfgr, hpre;
	uint16_t hdiv;
	cfgr = (RCC->CFGR);
	hpre = cfgr & RCC_CFGR_HPRE_Msk;
	if (hpre){
		switch (hpre){
		case RCC_CFGR_HPRE_DIV2:
			hdiv = 2;
			break;
		case RCC_CFGR_HPRE_DIV4:
			hdiv = 4;
			break;
		case RCC_CFGR_HPRE_DIV8:
			hdiv = 8;
			break;
		case RCC_CFGR_HPRE_DIV16:
			hdiv = 16;
			break;
		case RCC_CFGR_HPRE_DIV64:
			hdiv = 64;
			break;
		case RCC_CFGR_HPRE_DIV128:
			hdiv = 128;
			break;
		case RCC_CFGR_HPRE_DIV256:
			hdiv = 256;
			break;
		default :
			hdiv = 512;
			break;
		}
	}
	else {
		hdiv = 1;
	}
	return hdiv;
}

/*!
  * @static
  * @brief Get the APB divisor
  *
  * @return the divisor value
  *
  */
static uint8_t _get_APB_div_(void){
	uint32_t cfgr, pres;
	uint8_t pdiv;
	cfgr = (RCC->CFGR);
	pres = cfgr & RCC_CFGR_PPRE2_Msk;
	if (pres){
		switch (pres)
		{
		case RCC_CFGR_PPRE2_DIV1:
			pdiv = 1;
			break;
		case RCC_CFGR_PPRE2_DIV2:
			pdiv = 2;
			break;
		case RCC_CFGR_PPRE2_DIV4:
			pdiv = 4;
			break;
		case RCC_CFGR_PPRE2_DIV8:
			pdiv = 8;
			break;
		default :
			pdiv = 16;
			break;
		}
	}
	else {
		pdiv = 1;
	}
	return pdiv;
}

/*!
  * @static
  * @brief Compute the current SPI clock frequency
  *
  * @return the SPI clock frequency
  *
  */
static uint32_t _get_SPI_freq_(void){
	uint16_t hdiv;
	uint8_t pdiv;
	int spi_freq;
	hdiv = _get_AHB_div_();
	pdiv = _get_APB_div_();
	spi_freq = ( SystemCoreClock / (uint32_t)hdiv ) / (uint32_t)pdiv;
	return spi_freq;
}

/*!
  * @brief Initialize the SPI peripheral
  *
  * @param [in] p_Device Pointer on the spi device structure
  *
  * @retval DEV_SUCCESS if everything is fine (see @link dev_res_e::DEV_SUCCESS @endlink)
  * @retval DEV_FAILURE if failed (see @link dev_res_e::DEV_FAILURE @endlink)
  *
  */
uint8_t BSP_Spi_Init(const p_spi_dev_t p_Device)
{
	uint8_t ret = DEV_SUCCESS;
	uint8_t u8_Status;
    SPI_HandleTypeDef *p_handle = paSPI_BusHandle[p_Device->bus_id];
    u8_Status = HAL_SPI_Init(p_handle);
    if ( u8_Status != HAL_OK) {
    	DBG_BSP("SPI 0x%8X Init: status %d\r\n", paSPI_BusHandle[p_Device->bus_id]->Instance, u8_Status);
        ret = DEV_FAILURE;
    }
    return ret;
}

/*!
  * @brief Set the SPI device structure to its default
  *
  * @param [in] p_Device Pointer on the spi device structure
  *
  * @retval DEV_SUCCESS if everything is fine (see @link dev_res_e::DEV_SUCCESS @endlink)
  *
  */
uint8_t BSP_Spi_SetDefault(const p_spi_dev_t p_Device)
{
	SPI_HandleTypeDef *p_handle = paSPI_BusHandle[p_Device->bus_id];
    /* Fill default value */
    p_handle->Init.Mode              = SPI_MODE_MASTER;
    p_handle->Init.Direction         = SPI_DIRECTION_2LINES;
    p_handle->Init.NSS               = SPI_NSS_SOFT;
    p_handle->Init.FirstBit          = SPI_FIRSTBIT_MSB;
    p_handle->Init.TIMode            = SPI_TIMODE_DISABLE;
    p_handle->Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    p_handle->Init.CRCPolynomial     = 7;
    p_handle->Init.CRCLength         = SPI_CRC_LENGTH_DATASIZE;
    p_handle->Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;

    /* Could be adjusted to related device */
    p_handle->Init.DataSize          = SPI_DATASIZE_8BIT;
    p_handle->Init.CLKPolarity       = SPI_POLARITY_LOW;
    p_handle->Init.CLKPhase          = SPI_PHASE_1EDGE;
    p_handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;

    return DEV_SUCCESS;
}

/*!
  * @brief Open / init the SPI device
  *
  * @param [in] p_Device Pointer on the spi device structure
  *
  * @retval DEV_SUCCESS if everything is fine (see @link dev_res_e::DEV_SUCCESS @endlink)
  *
  */
uint8_t BSP_Spi_Open(const p_spi_dev_t p_Device)
{
	BSP_Gpio_SetHigh(p_Device->ss_port, p_Device->ss_pin);
	HAL_SPI_MspInit(paSPI_BusHandle[p_Device->bus_id]);
	return DEV_SUCCESS;
}

/*!
  * @brief Close / uninit the SPI device
  *
  * @param [in] p_Device Pointer on the spi device structure
  *
  * @retval DEV_SUCCESS if everything is fine (see @link dev_res_e::DEV_SUCCESS @endlink)
  *
  */
uint8_t BSP_Spi_Close (const p_spi_dev_t p_Device)
{
	HAL_SPI_MspDeInit(paSPI_BusHandle[p_Device->bus_id]);
	BSP_Gpio_SetLow(p_Device->ss_port, p_Device->ss_pin);
	return DEV_SUCCESS;
}

/*!
  * @brief Set the SPI clock frequency
  *
  * @param [in] p_Device  Pointer on the spi device structure
  * @param [in] u32_Hertz Frequency to set (in Hertz)
  *
  * @retval DEV_SUCCESS if everything is fine (see @link dev_res_e::DEV_SUCCESS @endlink)
  * @retval DEV_FAILURE if failed (see @link dev_res_e::DEV_FAILURE @endlink)
  * @retval DEV_BUSY if the given device is busy (see @link dev_res_e::DEV_BUSY @endlink)
  *
  */
uint8_t BSP_Spi_SetBitrate (const p_spi_dev_t p_Device, const uint32_t u32_Hertz)
{
	int spi_hz = 0;
	uint8_t prescaler_rank = 0;
	uint8_t last_index = (sizeof(prescaler_table) / sizeof(prescaler_table[0])) - 1;

	/* Calculate the spi clock for prescaler_rank 0: SPI_BAUDRATEPRESCALER_2 */
	spi_hz = _get_SPI_freq_() / 2;

	/* Define pre-scaler in order to get highest available frequency below requested frequency */
	while ((spi_hz > u32_Hertz) && (prescaler_rank < last_index)) {
		spi_hz = spi_hz / 2;
		prescaler_rank++;
	}

	/*  Use the best fit pre-scaler */
	paSPI_BusHandle[p_Device->bus_id]->Init.BaudRatePrescaler = prescaler_table[prescaler_rank];

	/*  In case maximum pre-scaler still gives too high freq, raise an error */
	if (spi_hz > u32_Hertz) {
		DBG_BSP("WRN: lowest SPI freq (%d)  higher than requested (%d)\r\n", spi_hz, (int)u32_Hertz);
	}

	DBG_BSP("spi_frequency, request:%d, select:%d\r\n", (int)u32_Hertz, spi_hz);

	return BSP_Spi_Init(p_Device);
}

/*!
  * @brief Set the SPI clock phase
  *
  * @param [in] p_Device Pointer on the spi device structure
  * @param [in] b_Flag   0 : one edge; 1 : two edges.
  *
  * @retval DEV_SUCCESS if everything is fine (see @link dev_res_e::DEV_SUCCESS @endlink)
  *
  */
uint8_t BSP_Spi_SetClockPhase (const p_spi_dev_t p_Device, const bool b_Flag)
{
    switch (b_Flag) {
        case 1:
        	paSPI_BusHandle[p_Device->bus_id]->Init.CLKPhase = SPI_PHASE_2EDGE;
            break;
        default : //case 0:
        	paSPI_BusHandle[p_Device->bus_id]->Init.CLKPhase = SPI_PHASE_1EDGE;
            break;
    }
	return DEV_SUCCESS;
}

/*!
  * @brief Set the SPI clock polarity
  *
  * @param [in] p_Device Pointer on the spi device structure
  * @param [in] b_Flag   0 : polarity low; 1 : polarity high.
  *
  * @retval DEV_SUCCESS if everything is fine (see @link dev_res_e::DEV_SUCCESS @endlink)
  *
  */
uint8_t BSP_Spi_SetClockPol (const p_spi_dev_t p_Device, const bool b_Flag)
{
	switch (b_Flag) {
		case 1:
			paSPI_BusHandle[p_Device->bus_id]->Init.CLKPolarity = SPI_POLARITY_HIGH;
			break;
		default:// case 0:
			paSPI_BusHandle[p_Device->bus_id]->Init.CLKPolarity = SPI_POLARITY_LOW;
			break;
	}
	return DEV_SUCCESS;
}

/*!
  * @brief Read and Write to the SPI bus
  *
  * @param [in]     p_Device Pointer on the spi device structure
  * @param [in,out] p_Xfr    Pointer on data structure
  *
  * @retval DEV_SUCCESS if everything is fine (see @link dev_res_e::DEV_SUCCESS @endlink)
  * @retval DEV_FAILURE if failed (see @link dev_res_e::DEV_FAILURE @endlink)
  * @retval DEV_BUSY if the given device is busy (see @link dev_res_e::DEV_BUSY @endlink)
  *
  */
uint8_t BSP_Spi_ReadWrite (const p_spi_dev_t p_Device, spi_transceiver_s* const p_Xfr)
{
	uint8_t ret = DEV_SUCCESS;
	uint8_t u8_Status;
	if (HAL_SPI_GetState(paSPI_BusHandle[p_Device->bus_id]) == HAL_SPI_STATE_READY)
	{
		BSP_Gpio_SetLow(p_Device->ss_port, p_Device->ss_pin);
		u8_Status = HAL_SPI_TransmitReceive(
				paSPI_BusHandle[p_Device->bus_id],
				p_Xfr->pTransmitter,
				p_Xfr->pReceiver,
				p_Xfr->ReceiverBytes, SPI_TX_TIMEOUT);
		if ( u8_Status != HAL_OK )
		{
			DBG_BSP("SPI %x Transmit: status %d\r\n", paSPI_BusHandle[p_Device->bus_id]e->Instance, u8_Status);
			ret = DEV_FAILURE;
		}
		BSP_Gpio_SetHigh(p_Device->ss_port, p_Device->ss_pin);
	}
	else {
		ret = DEV_BUSY;
	}
	return ret;
}

/*! @} */
