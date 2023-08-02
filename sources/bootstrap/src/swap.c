
#include <stdint.h>

#include "swap.h"
#include "flash.h"

/******************************************************************************/
static int32_t flash_erase(uint32_t address, uint32_t len);
static int32_t flash_write(uint32_t dest, uint32_t src, uint32_t len);

static int32_t flash_erase(uint32_t address, uint32_t len)
{
    register uint32_t FirstPage = 0;
    register uint32_t LastPage = 0;
    uint32_t PAGEError = 0;

    // Assume that address is page aligned
    if ( ! hal_flash_is_aligned(address) || (len < 1) )
    {
    	return -1;
    }

    FirstPage = hal_flash_get_page(address);
    LastPage = hal_flash_get_page(address + len);
    LastPage += !( hal_flash_is_aligned( address + len) );

    if ( hal_flash_erase(FirstPage, (LastPage - FirstPage), &PAGEError) )
    {
        return -1;
    	//return PAGEError;
    }
    return 0;
}

static int32_t flash_write(uint32_t dest, uint32_t src, uint32_t len)
{
	register uint32_t nbLine = len / 8;
	register uint32_t rest = len % 8;

    hal_flash_clear_errors();
    // Program the first part
    if ( hal_flash_write(dest, src, nbLine) )
    {
    	// failed
    	return -1;
    }

    // fill the rest, if any
    if(rest)
    {
    	uint8_t dword[8];
    	*(uint64_t*)dword = (uint64_t)(-1);
    	register uint8_t *s = (uint8_t*)(src + len - rest);
    	do
    	{
    		rest--;
    		dword[rest] = s[rest];
    	} while(rest);

        // Program the rest part
        if ( hal_flash_write(dest + (nbLine*8), (uint32_t)(dword), 1) )
        {
        	// failed
        	return -1;
        }
    }
    return 0;
}

int swap(register struct __exch_info_s * pp)
{
	hal_flash_lock(0);
	if ( flash_erase(pp->dest, pp->dest_sz) )
	{
		// error occurs
		goto failed;
	}
	// Write app bin
	if( flash_write(pp->dest + HEADER_SZ, pp->src + HEADER_SZ, pp->src_sz - HEADER_SZ) )
	{
		// error occurs
		goto failed;
	}
	// write header (magic and size)
	if( flash_write(pp->dest, pp->src, HEADER_SZ) )
	{
		// error occurs
		goto failed;
	}
	return 0;
failed :
	hal_flash_lock(1);
	return -1;
	/*
	 * check type of part
	 * 1) Is Compressed ?
	 *    a) lz4 ?
	 *    b) lzma ?
	 * 2) Is ciphered / signed ?
	 * 	  a) aes ?
	 * 	  b)
	 *
	 */
}
