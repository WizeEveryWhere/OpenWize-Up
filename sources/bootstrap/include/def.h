#ifndef _DEF_H_
#define _DEF_H_
#ifdef __cplusplus
extern "C" {
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

#ifndef __IO
#define __IO volatile
#endif

#ifdef __cplusplus
}
#endif
#endif /* _DEF_H_ */
