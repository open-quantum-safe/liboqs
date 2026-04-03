/**
 * @file crypto_memset.h
 * @brief Header file for crypto_memset.c
 */

#ifndef HQC_CRYPTO_MEMSET_H
#define HQC_CRYPTO_MEMSET_H

#include <stddef.h>

/**
 * safer call to memset https://github.com/veorq/cryptocoding#problem-4
 */
extern void *(*volatile memset_volatile)(void *, int, size_t);

/**
 * @def memset_zero
 * @brief Securely zero a memory region.
 */
#define memset_zero(ptr, len) memset_volatile((ptr), 0, (len))

#endif  // HQC_CRYPTO_MEMSET_H
