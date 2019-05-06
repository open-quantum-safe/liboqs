/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

/* Inspired by m4ri's mzd implementation, but completely re-written for our use-case. */

#ifndef MZD_ADDITIONAL_H
#define MZD_ADDITIONAL_H

#include "macros.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint64_t word;
#define WORD_C(v) UINT64_C(v)

#if defined(WITH_OPT)
#include "simd.h"
#endif

typedef union {
  word w64[4];
#if defined(WITH_OPT)
#if defined(WITH_SSE2) || defined(WITH_NEON)
  word128 w128[2];
#endif
#if defined(WITH_AVX2)
  word256 w256;
#endif
#endif
} block_t ATTR_ALIGNED(32);

/**
 * Representation of matrices and vectors
 *
 * The basic memory unit is a block of 256 bit. Each row is stored in (possible multiple) blocks
 * depending on the number of columns. Matrices with up to 128 columns are the only excpetion. In
 * this case a block actually contains two rows. The row with even index is contained in w64[0] and
 * w61[1], the row with odd index is contained in w64[2] and w64[3].
 */
typedef block_t mzd_local_t;

mzd_local_t* oqs_sig_picnic_mzd_local_init_ex(uint32_t r, uint32_t c, bool clear) ATTR_ASSUME_ALIGNED(32);

#define mzd_local_init(r, c) oqs_sig_picnic_mzd_local_init_ex(r, c, true)

void oqs_sig_picnic_mzd_local_free(mzd_local_t* v);

void oqs_sig_picnic_mzd_local_init_multiple_ex(mzd_local_t** dst, size_t n, uint32_t r, uint32_t c, bool clear)
    ATTR_NONNULL_ARG(1);

#define mzd_local_init_multiple(dst, n, r, c) oqs_sig_picnic_mzd_local_init_multiple_ex(dst, n, r, c, true)

/**
 * mzd_local_free for mzd_local_init_multiple.
 */
void oqs_sig_picnic_mzd_local_free_multiple(mzd_local_t** vs);

void oqs_sig_picnic_mzd_xor_uint64_128(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;
void oqs_sig_picnic_mzd_xor_uint64_192(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;
void oqs_sig_picnic_mzd_xor_uint64_256(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;
void oqs_sig_picnic_mzd_xor_uint64_576(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;
void oqs_sig_picnic_mzd_xor_uint64_640(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;
void oqs_sig_picnic_mzd_xor_uint64_896(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;
void oqs_sig_picnic_mzd_xor_uint64_960(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;
void oqs_sig_picnic_mzd_xor_uint64_1152(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;
void oqs_sig_picnic_mzd_xor_uint64_1216(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;
void oqs_sig_picnic_mzd_xor_s128_128(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;
void oqs_sig_picnic_mzd_xor_s128_256(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;
void oqs_sig_picnic_mzd_xor_s128_640(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;
void oqs_sig_picnic_mzd_xor_s128_896(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;
void oqs_sig_picnic_mzd_xor_s128_1024(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;
void oqs_sig_picnic_mzd_xor_s128_1152(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;
void oqs_sig_picnic_mzd_xor_s128_1280(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;
void oqs_sig_picnic_mzd_xor_s256_128(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;
void oqs_sig_picnic_mzd_xor_s256_256(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;
void oqs_sig_picnic_mzd_xor_s256_768(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;
void oqs_sig_picnic_mzd_xor_s256_1024(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;
void oqs_sig_picnic_mzd_xor_s256_1280(mzd_local_t* res, mzd_local_t const* first, mzd_local_t const* second) ATTR_NONNULL;

/**
 * Compute v * A optimized for v being a vector.
 */
void oqs_sig_picnic_mzd_mul_v_uint64_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_uint64_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_uint64_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_uint64_128_576(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_uint64_128_640(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_uint64_192_896(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_uint64_192_960(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_uint64_256_1152(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_uint64_256_1216(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_s128_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_s128_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_s128_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_s128_128_640(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_s128_192_896(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_s128_192_1024(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_s128_256_1152(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_s128_256_1280(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_s256_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_s256_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_s256_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_s256_128_768(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_s256_192_1024(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_s256_256_1280(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* At) ATTR_NONNULL;

/**
 * Compute v * A optimized for v being a vector, for specific sizes depending on instance
 * Only work for specific sizes and RLL_NEXT algorithm using uint64 operations
 */
void oqs_sig_picnic_mzd_addmul_v_uint64_30_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_uint64_30_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_uint64_30_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_uint64_3_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_uint64_3_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_uint64_3_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;

/**
 * Use SSE2 or NEON
 */
void oqs_sig_picnic_mzd_addmul_v_s128_30_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_s128_30_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_s128_30_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_s128_3_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_s128_3_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_s128_3_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;

/**
 * Use AVX2
 */
void oqs_sig_picnic_mzd_addmul_v_s256_30_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_s256_30_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_s256_30_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_s256_3_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_s256_3_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_s256_3_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;

/**
 * Compute using parity based algorithm
 * */
void oqs_sig_picnic_mzd_mul_v_parity_uint64_128_30(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_parity_uint64_192_30(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_parity_uint64_256_30(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_parity_uint64_128_3(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_parity_uint64_192_3(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_parity_uint64_256_3(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;

/**
 * Using popcnt
 */
void oqs_sig_picnic_mzd_mul_v_parity_popcnt_128_30(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_parity_popcnt_192_30(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_parity_popcnt_256_30(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_parity_popcnt_128_3(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_parity_popcnt_192_3(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_v_parity_popcnt_256_3(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;

/**
 * Compute c + v * A optimized for c and v being vectors.
 */
void oqs_sig_picnic_mzd_addmul_v_uint64_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_uint64_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_uint64_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_s128_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_s128_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_s128_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_s256_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_s256_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_v_s256_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;

#if defined(MUL_M4RI)
/**
 * Compute v * A optimized for v being a vector.
 */
void oqs_sig_picnic_mzd_mul_vl_uint64_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_uint64_128_576(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_uint64_128_640(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_uint64_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_uint64_192_896(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_uint64_192_960(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_uint64_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_uint64_256_1152(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_uint64_256_1216(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_s128_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_s128_128_640(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_s128_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_s128_192_896(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_s128_192_1024(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_s128_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_s128_256_1152(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_s128_256_1280(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_s256_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_s256_128_768(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_s256_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_s256_192_1024(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_s256_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_mul_vl_s256_256_1280(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
/**
 * Compute c + v * A optimized for c and v being vectors.
 */
void oqs_sig_picnic_mzd_addmul_vl_uint64_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_vl_uint64_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_vl_uint64_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_vl_s128_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_vl_s128_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_vl_s128_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_vl_s256_128(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_vl_s256_192(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;
void oqs_sig_picnic_mzd_addmul_vl_s256_256(mzd_local_t* c, mzd_local_t const* v, mzd_local_t const* A) ATTR_NONNULL;

/**
 * Pre-compute matrices for mzd_{add,}mul_vl computions.
 */
mzd_local_t* oqs_sig_picnic_mzd_precompute_matrix_lookup(mzd_local_t const* A, unsigned int r, unsigned int c) ATTR_NONNULL;
#endif

/**
 * Shuffle vector x according to info in mask. Needed for OLLE optimiztaions.
 */
void oqs_sig_picnic_mzd_shuffle_128_30(mzd_local_t* x, const word mask) ATTR_NONNULL;
void oqs_sig_picnic_mzd_shuffle_128_3(mzd_local_t* x, const word mask) ATTR_NONNULL;
void oqs_sig_picnic_mzd_shuffle_192_30(mzd_local_t* x, const word mask) ATTR_NONNULL;
void oqs_sig_picnic_mzd_shuffle_192_3(mzd_local_t* x, const word mask) ATTR_NONNULL;
void oqs_sig_picnic_mzd_shuffle_256_30(mzd_local_t* x, const word mask) ATTR_NONNULL;
void oqs_sig_picnic_mzd_shuffle_256_3(mzd_local_t* x, const word mask) ATTR_NONNULL;
void oqs_sig_picnic_mzd_shuffle_pext_128_30(mzd_local_t* x, const word mask) ATTR_NONNULL;
void oqs_sig_picnic_mzd_shuffle_pext_128_3(mzd_local_t* x, const word mask) ATTR_NONNULL;
void oqs_sig_picnic_mzd_shuffle_pext_192_30(mzd_local_t* x, const word mask) ATTR_NONNULL;
void oqs_sig_picnic_mzd_shuffle_pext_192_3(mzd_local_t* x, const word mask) ATTR_NONNULL;
void oqs_sig_picnic_mzd_shuffle_pext_256_30(mzd_local_t* x, const word mask) ATTR_NONNULL;
void oqs_sig_picnic_mzd_shuffle_pext_256_3(mzd_local_t* x, const word mask) ATTR_NONNULL;

#define BLOCK(v, b) ((block_t*)ASSUME_ALIGNED(&(v)[(b)], 32))
#define CONST_BLOCK(v, b) ((const block_t*)ASSUME_ALIGNED(&(v)[(b)], 32))

#endif
