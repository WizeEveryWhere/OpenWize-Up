
#include <stm32l4xx_hal_rcc.h>
#include "stm32l4xx_hal_crc.h"
#include "crc.h"

#ifdef __cplusplus
extern "C" {
#endif

#if CRC_POLYLENGTH == CRC_POLYLENGTH_32B
static uint32_t crc32_compute(uint32_t *pData, uint32_t len);
#else
static uint32_t crc16_compute(uint16_t pData, uint32_t len);
#endif

void crc_init(void)
{
	// Enable CRC clock
	__HAL_RCC_CRC_CLK_ENABLE();
	// initialize peripheral with default generating polynomial
#if CRC_POLY != DEFAULT_CRC32_POLY
	WRITE_REG(CRC->POL, CRC_POLY);
#endif
	/*
	 * Default values from RESET
	 * MODIFY_REG(CRC->CR, CRC_CR_REV_IN, CRC_INPUTDATA_INVERSION_NONE);
	 * MODIFY_REG(CRC->CR, CRC_CR_REV_OUT, CRC_OUTPUTDATA_INVERSION_DISABLE);
	 */
#if CRC_POLYLENGTH != CRC_POLYLENGTH_32B
	MODIFY_REG(CRC->CR, CRC_CR_POLYSIZE, CRC_POLYLENGTH);
#endif

#if CRC_INITVALUE != DEFAULT_CRC_INITVALUE
	WRITE_REG(CRC->INIT, CRC_INITVALUE);
#endif

}
void crc_deinit(void)
{
	// Reset CRC calculation unit
	CRC->CR |= CRC_CR_RESET;
	// Reset IDR register content
	CLEAR_BIT(CRC->IDR, CRC_IDR_IDR);
	// Disable CRC clock
	__HAL_RCC_CRC_CLK_DISABLE();
}

#if CRC_POLYLENGTH == CRC_POLYLENGTH_32B
static uint32_t crc32_compute(uint32_t *pData, uint32_t len)
{
	register uint32_t index;      /* CRC input data buffer index */
	register CRC_TypeDef *p_reg = CRC;
	// Reset CRC calculation unit
	p_reg->CR |= CRC_CR_RESET;

    // Enter 32-bit input data to the CRC calculator
    for (index = 0U; index < len; index++)
    {
    	p_reg->DR = pData[index];
    }
    return p_reg->DR;
}

#else
static uint32_t crc16_compute(uint16_t pData, uint32_t len)
{
	uint32_t i;  /* input data buffer index */
	__IO uint16_t *pReg;

	/* Processing time optimization: 2 HalfWords are entered in a row with a single word write,
	* in case of odd length, last HalfWord must be carefully fed to the CRC calculator to ensure
	* a correct type handling by the peripheral */
	for (i = 0U; i < (len / 2U); i++)
	{
		CRC->DR = ((uint32_t)pData[2U * i] << 16U) | (uint32_t)pData[(2U * i) + 1U];
	}
	if ((len % 2U) != 0U)
	{
		pReg = (__IO uint16_t *)(__IO void *)(&CRC->DR);  /* Derogation MisraC2012 R.11.5 */
		*pReg = pData[2U * i];
	}
	return CRC->DR;
}
#endif

uint32_t crc_compute(uint32_t *pData, uint32_t len)
{
#if CRC_POLYLENGTH == CRC_POLYLENGTH_32B
	return crc32_compute(pData, len);
#else
	return crc16_compute((uint16_t*)pData, len);
#endif
}

#ifdef __cplusplus
}
#endif


