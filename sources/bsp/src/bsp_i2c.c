/**
  * @file: bsp_i2c.c
  * @brief: This file contains functions to deal with I2C.
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
  * 1.0.0 : 2020/10/15[GBI]
  * Initial version
  *
  *
  */
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_i2c.h"
#include "platform.h"

extern I2C_HandleTypeDef *paI2C_BusHandle[I2C_ID_MAX];

static void _i2c_error_ (uint8_t u8BusId);

static uint32_t I2cxTimeout = I2Cx_TIMEOUT_MAX;   /*<! Value of Timeout when I2C communication fails */


//void BSP_I2C_Enable(i2c_id_e eBusId, uint8_t bFlag)
void BSP_I2C_Enable(uint8_t eBusId, uint8_t bFlag)
{
	if ( eBusId < I2C_ID_MAX)
	{
		if (bFlag)
		{
			 HAL_I2C_MspInit(paI2C_BusHandle[eBusId]);
		}
		else
		{
			 HAL_I2C_MspDeInit(paI2C_BusHandle[eBusId]);
		}
	}
}

uint8_t BSP_I2C_IsDeviceReady(const i2c_dev_t *pDev, const uint32_t u32NbTrials)
{
	return HAL_I2C_IsDeviceReady(paI2C_BusHandle[pDev->bus_id], pDev->device_id, u32NbTrials, I2cxTimeout);
}

uint8_t BSP_I2C_Write(
	const i2c_dev_t *pDev,
	uint8_t *pData,
	uint16_t u16Length
	)
{
	dev_res_e eRet = DEV_INVALID_PARAM;
	if (pDev && pData)
	{
		eRet = HAL_I2C_Master_Transmit(
				paI2C_BusHandle[pDev->bus_id],
				pDev->device_id,
				pData,
				u16Length,
				I2cxTimeout);
		/* Check the communication status */
		if(eRet == DEV_FAILURE)
		{
			/* Re-Initiaize the BUS */
			_i2c_error_(pDev->bus_id);
		}
	}
	return eRet;
}

uint8_t BSP_I2C_Read(
	const i2c_dev_t *pDev,
	uint8_t *pData,
	uint16_t u16Length
	)
{
	dev_res_e eRet = DEV_INVALID_PARAM;
	if (pDev && pData)
	{
		eRet = HAL_I2C_Master_Receive(
				paI2C_BusHandle[pDev->bus_id],
				pDev->device_id,
				pData,
				u16Length,
				I2cxTimeout);
		/* Check the communication status */
		if(eRet == DEV_FAILURE)
		{
			/* Re-Initiaize the BUS */
			_i2c_error_(pDev->bus_id);
		}
	}
	return eRet;
}

uint8_t BSP_I2C_MemRead(
	const i2c_dev_t *pDev,
	uint16_t u16MemAddress,
	uint8_t *pData,
	uint16_t u16Length
	)
{
	dev_res_e eRet = DEV_INVALID_PARAM;

	if (pDev && pData)
	{
		eRet = HAL_I2C_Mem_Read(
				paI2C_BusHandle[pDev->bus_id],
				pDev->device_id,
				u16MemAddress,
				I2C_MEMADD_SIZE_16BIT,
				pData, u16Length,
				I2cxTimeout);

		/* Check the communication status */
		if(eRet == DEV_FAILURE)
		{
			/* Re-Initiaize the BUS */
			_i2c_error_(pDev->bus_id);
		}
	}
	return eRet;
}

uint8_t BSP_I2C_MemWrite (
	const i2c_dev_t *pDev,
	uint16_t u16MemAddress,
	uint8_t *pData,
	uint16_t u16Length
	)
{
	dev_res_e eRet = DEV_INVALID_PARAM;

	if (pDev && pData)
	{
		eRet = HAL_I2C_Mem_Write(
			paI2C_BusHandle[pDev->bus_id],
			pDev->device_id,
			u16MemAddress,
			I2C_MEMADD_SIZE_16BIT,
			pData, u16Length,
			I2cxTimeout);
		/* Check the communication status */
		if(eRet == DEV_FAILURE)
		{
			/* Re-Initiaize the BUS */
			_i2c_error_(pDev->bus_id);
		}
	}
	return eRet;
}

uint8_t BSP_I2C_RegRead(
	const i2c_dev_t *pDev,
	uint8_t u8RegAddress,
	uint8_t *pData,
	uint16_t u16Length
	)
{
	dev_res_e eRet = DEV_INVALID_PARAM;
	if (pDev && pData)
	{
		eRet = HAL_I2C_Mem_Read(
				paI2C_BusHandle[pDev->bus_id],
				pDev->device_id,
				(uint16_t)u8RegAddress,
				I2C_MEMADD_SIZE_8BIT,
				pData, u16Length,
				I2cxTimeout);
		/* Check the communication status */
		if(eRet == DEV_FAILURE)
		{
			/* Re-Initiaize the BUS */
			_i2c_error_(pDev->bus_id);
		}
	}
	return eRet;
}

uint8_t BSP_I2C_RegWrite (
	const i2c_dev_t *pDev,
	uint8_t u8RegAddress,
	uint8_t *pData,
	uint16_t u16Length
	)
{
	dev_res_e eRet = DEV_INVALID_PARAM;
	if (pDev && pData)
	{
		eRet = HAL_I2C_Mem_Write(
				paI2C_BusHandle[pDev->bus_id],
				pDev->device_id,
				(uint16_t)u8RegAddress,
				I2C_MEMADD_SIZE_8BIT,
				pData, u16Length,
				I2cxTimeout);
		/* Check the communication status */
		if(eRet == DEV_FAILURE)
		{
			/* Re-Initiaize the BUS */
			_i2c_error_(pDev->bus_id);
		}
	}
	return eRet;
}

/**
  * @brief Eval I2Cx error treatment function
  * @retval None
  */
static void _i2c_error_ (uint8_t u8BusId)
{
  /* De-initialize the I2C communication BUS */
  HAL_I2C_DeInit(paI2C_BusHandle[u8BusId]);

  /* Re- Initiaize the I2C communication BUS */
  HAL_I2C_Init(paI2C_BusHandle[u8BusId]);
}

#ifdef __cplusplus
}
#endif
