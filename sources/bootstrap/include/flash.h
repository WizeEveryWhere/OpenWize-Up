#ifndef _FLASH_H_
#define _FLASH_H_

#if defined(__cplusplus)
extern "C"
{
#endif

#ifndef RAMFUNCTION
#if defined(RAM_CODE)
#  if defined(ARCH_ARM)
#    define RAMFUNCTION __attribute__((used,section(".ramcode"),long_call))
#  else
#    define RAMFUNCTION __attribute__((used,section(".ramcode")))
#  endif
#else
# define RAMFUNCTION
#endif
#endif

#include <stdint.h>

void hal_flash_clear_errors(void);
void hal_flash_lock(uint8_t bLock);
uint32_t hal_flash_get_page(register uint32_t u32Address);
uint32_t hal_flash_is_aligned(register uint32_t u32Address);
uint32_t hal_flash_erase(uint32_t Page, uint32_t NbPages, uint32_t *PageError);
uint32_t hal_flash_write(uint32_t dest, uint32_t src, uint32_t nbLine);

#if defined(__cplusplus)
}
#endif

#endif // _FLASH_H_
