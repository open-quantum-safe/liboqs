#ifndef __RIJDAEL_PLATFORM_H__
#define __RIJDAEL_PLATFORM_H__

/* Defined when we want to overload some low-level APIs */
#if defined(USE_WEAK_LOW_LEVEL_API)
#define WEAK __attribute__((weak))
#else
#define WEAK
#endif

/* Select the best Rijndael implementation depending on the platform if
 * not overloaded by the user */
#if !defined(RIJNDAEL_CONSTANT_TIME_REF) && !defined(RIJNDAEL_TABLE) && !defined(RIJNDAEL_AES_NI) && !defined(RIJNDAEL_BITSLICE) && !defined(RIJNDAEL_EXTERNAL)
/* When AES-NI is present, select the optimized implementation */
#ifdef __AES__
#define RIJNDAEL_AES_NI
#else
/* When no AES-NI is detected, select the constant time bitslice implementation as it is portable */
#define RIJNDAEL_BITSLICE
#endif
#endif

#endif /* __RIJDAEL_PLATFORM_H__ */
