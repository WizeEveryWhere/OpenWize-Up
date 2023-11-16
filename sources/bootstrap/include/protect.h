#ifndef _PROTECT_H_
#define _PROTECT_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "img.h"

int protect(register struct __exch_info_s * pp);

#ifdef __cplusplus
}
#endif
#endif /* _PROTECT_H_ */
