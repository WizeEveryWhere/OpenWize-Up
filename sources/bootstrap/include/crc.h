#ifndef CRC_H_
#define CRC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define CRC_POLY DEFAULT_CRC32_POLY
#define CRC_INITVALUE DEFAULT_CRC_INITVALUE
#define CRC_POLYLENGTH CRC_POLYLENGTH_32B
//#define CRC_POLYLENGTH CRC_POLYLENGTH_16B

void crc_init(void);
void crc_deinit(void);
uint32_t crc_compute(uint32_t *pData, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* CRC_H_ */
