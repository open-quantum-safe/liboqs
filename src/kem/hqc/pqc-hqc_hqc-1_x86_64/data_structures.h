/**
 * @file    data_structures.h
 * @brief   HQC PKE and KEM ciphertext data structures.
 */

#ifndef HQC_DATA_STRUCTURES_H
#define HQC_DATA_STRUCTURES_H

#include <stdint.h>
#include <x86intrin.h>
#include "parameters.h"

/**
 * @brief  a 128-bit reed–muller codeword, viewable as masks or integer arrays.
 *
 * This union allows interpreting a 128-bit codeword in three ways:
 *   - \c mask : as eight 16-bit predicate masks
 *   - \c u16  : as eight 16-bit unsigned integers
 *   - \c u32  : as four 32-bit unsigned integers
 */
typedef union {
    __mmask16 mask[8]; /**< eight 16-bit avx-512 predicate masks, one per lane */
    uint16_t u16[8];   /**< eight 16-bit unsigned integer views of the codeword */
    uint32_t u32[4];   /**< four 32-bit unsigned integer views of the codeword */
} rm_codeword128_t;

/**
 * @brief  a 256-bit reed–muller vector register, viewable as raw simd or 16-bit lanes.
 *
 * This union allows interpreting a 256-bit avx2 register in two ways:
 *   - \c mm  : as a single __m256i simd value
 *   - \c u16 : as sixteen 16-bit unsigned integers
 */
typedef union {
    __m256i mm;       /**< raw 256-bit avx2 simd register */
    uint16_t u16[16]; /**< sixteen 16-bit unsigned integer lanes */
} rm_vector256_t;

/**
 * @brief  an “expanded” 128-element reed–muller codeword, packed into eight avx2 registers.
 *
 * This union allows treating a 128-element array of int16_t in two ways:
 *   - \c mm  : as an array of eight __m256i simd registers (8×32 B = 256 B total)
 *   - \c i16 : as a flat array of 128 signed 16-bit integers
 */
typedef union {
    __m256i mm[8];    /**< eight 256-bit avx2 simd registers (256 bytes total) */
    int16_t i16[128]; /**< flat array of 128 signed 16-bit integers */
} rm_expanded_cdw128_t;

#endif  // HQC_DATA_STRUCTURES_H
