/**
  * @file: bsp_i2c.h
  * @brief: This file defines functions to deal with I2C.
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
#ifndef _BSP_I2C_H_
#define _BSP_I2C_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

#define I2Cx_TIMEOUT_MAX 3000

typedef struct
{
    uint8_t bus_id;
    uint8_t device_id;
} i2c_dev_t;

typedef i2c_dev_t* p_i2c_dev_t;

//void BSP_I2C_Enable(i2c_id_e eBusId, uint8_t bFlag);
void BSP_I2C_Enable(uint8_t eBusId, uint8_t bFlag);
uint8_t BSP_I2C_IsDeviceReady(const i2c_dev_t *pDev, const uint32_t u32NbTrials);
uint8_t BSP_I2C_Write(const i2c_dev_t *pDev, uint8_t *pData, uint16_t u16Length);
uint8_t BSP_I2C_Read(const i2c_dev_t *pDev, uint8_t *pData, uint16_t u16Length);
uint8_t BSP_I2C_MemWrite(const i2c_dev_t *pDev, uint16_t u16MemAddress, uint8_t *pData, uint16_t u16Length);
uint8_t BSP_I2C_MemRead(const i2c_dev_t *pDev, uint16_t u16MemAddress, uint8_t *pData, uint16_t u16Length);
uint8_t BSP_I2C_RegWrite (const i2c_dev_t *pDev, uint8_t u8RegAddress, uint8_t *pData, uint16_t u16Length);
uint8_t BSP_I2C_RegRead (const i2c_dev_t *pDev, uint8_t u8RegAddress, uint8_t *pData, uint16_t u16Length);

#ifdef __cplusplus
}
#endif
#endif /* _BSP_I2C_H_ */
