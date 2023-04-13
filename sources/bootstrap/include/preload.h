#ifndef _PRELOAD_H_
#define _PRELOAD_H_

#if defined(__cplusplus)
extern "C"
{
#endif

#include "img.h"

boot_request_e preload(register struct __exch_info_s * pp );

#if defined(__cplusplus)
}
#endif

#endif // _PRELOAD_H_
