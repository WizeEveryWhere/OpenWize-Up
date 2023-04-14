#ifndef _BSP_CRC_H_
#define _BSP_CRC_H_

#ifdef __cplusplus
extern "C" {
#endif

#define CRC_POLY DEFAULT_CRC32_POLY
#define CRC_INITVALUE DEFAULT_CRC_INITVALUE
#define CRC_POLYLENGTH CRC_POLYLENGTH_32B
//#define CRC_POLYLENGTH CRC_POLYLENGTH_16B

void BSP_CRC_Init(void);
void BSP_CRC_Deinit(void);
uint32_t BSP_CRC_Compute(uint32_t *pData, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* _BSP_CRC_H_ */
