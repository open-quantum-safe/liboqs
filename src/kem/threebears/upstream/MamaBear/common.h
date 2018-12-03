#ifndef __THREEBEARS_COMMON_H__
#define __THREEBEARS_COMMON_H__
/** ThreeBears common utilities header */

#define __STDC_WANT_LIB_EXT1__ 1 /* for memset_s */
#include <string.h>
#include <stdint.h>
#include <assert.h>
#define WEAK INTERNAL __attribute__((weak))

#ifndef __GNUC__
#define  __attribute__(x)  /*nope*/
#endif

#define INTERNAL __attribute__((visibility("hidden")))

/* Securely erase size bytes from s */
void WEAK secure_bzero(void *s,size_t size);

#endif /* __THREEBEARS_COMMON_H__ */
