/**
 * @file crypto_memset.h
 * @brief Header file for crypto_memset.c
 */

#ifndef HQC_CRYPTO_MEMSET_H
#define HQC_CRYPTO_MEMSET_H

#include <stddef.h>
#include <oqs/common.h>

/**
 * @def memset_zero
 * @brief Securely zero a memory region.
 */
#define memset_zero OQS_MEM_cleanse

#endif  // HQC_CRYPTO_MEMSET_H
