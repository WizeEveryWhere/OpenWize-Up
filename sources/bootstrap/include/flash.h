#ifndef _FLASH_H_
#define _FLASH_H_

#if defined(__cplusplus)
extern "C"
{
#endif

#include <stdint.h>

void hal_flash_clear_errors(void);
void hal_flash_lock(uint8_t bLock);
uint32_t hal_flash_get_page(register uint32_t u32Address);
uint32_t hal_flash_is_aligned(register uint32_t u32Address);
uint32_t hal_flash_erase(uint32_t Page, uint32_t NbPages, uint32_t *PageError);
uint32_t hal_flash_write(uint32_t dest, uint32_t src, uint32_t nbLine);

void hal_flash_ob_lock(register uint8_t bLock);
uint32_t hal_flash_ob_read(void);
uint32_t hal_flash_ob_write(uint32_t ob_value);
uint32_t hal_flash_ob_launch(void);

#if defined(__cplusplus)
}
#endif

#endif // _FLASH_H_
