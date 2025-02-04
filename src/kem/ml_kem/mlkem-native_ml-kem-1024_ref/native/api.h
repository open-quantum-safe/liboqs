/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Native arithmetic interface
 *
 * This header is primarily for documentation purposes.
 * It should not be included by backend implementations.
 *
 * To ensure consistency with backends, the header will be
 * included automatically after inclusion of the active
 * backend, to ensure consistency of function signatures,
 * and run sanity checks.
 */
#ifdef MLKEM_NATIVE_ARITH_NATIVE_API_H
#error \
    "The arithmetic backend API `mlkem/native/api.h` "		\
    "should not be directly included. Please include the relevant "	\
    "structure headers directly."
#else /* MLKEM_NATIVE_ARITH_NATIVE_API_H */
#define MLKEM_NATIVE_ARITH_NATIVE_API_H

#include <stdint.h>
#include "../cbmc.h"
#include "../common.h"

/* Absolute exclusive upper bound for the output of the inverse NTT
 *
 * NOTE: This is the same bound as in poly.h and has to be kept
 * in sync. */
#define INVNTT_BOUND (8 * MLKEM_Q)

/* Absolute exclusive upper bound for the output of the forward NTT
 *
 * NOTE: This is the same bound as in poly.h and has to be kept
 * in sync. */
#define NTT_BOUND (8 * MLKEM_Q)

/*
 * This is the C<->native interface allowing for the drop-in of
 * native code for performance critical arithmetic components of ML-KEM.
 *
 * A _backend_ is a specific implementation of (part of) this interface.
 *
 * To add a function to a backend, define MLKEM_USE_NATIVE_XXX and
 * implement `static inline xxx(...)` in the profile header.
 *
 * The only exception is MLKEM_USE_NATIVE_NTT_CUSTOM_ORDER. This option can
 * be set if there are native implementations for all of NTT, invNTT, and
 * base multiplication, and allows the native implementation to use a
 * custom order of polynomial coefficients in NTT domain -- the use of such
 * custom order is not an implementation-detail since the public matrix
 * is generated in NTT domain. In this case, a permutation function
 * poly_permute_bitrev_to_custom() needs to be provided that permutes
 * polynomials in NTT domain from bitreversed to the custom order.
 */

/*
 * Those functions are meant to be trivial wrappers around the chosen native
 * implementation. The are static inline to avoid unnecessary calls.
 * The macro before each declaration controls whether a native
 * implementation is present.
 */

#if defined(MLKEM_USE_NATIVE_NTT)
/*************************************************
 * Name:        ntt_native
 *
 * Description: Computes negacyclic number-theoretic transform (NTT) of
 *              a polynomial in place.
 *
 *              The input polynomial is assumed to be in normal order.
 *              The output polynomial is in bitreversed order, or of a
 *              custom order if MLKEM_USE_NATIVE_NTT_CUSTOM_ORDER is set.
 *              See the documentation of MLKEM_USE_NATIVE_NTT_CUSTOM_ORDER
 *              for more information.
 *
 * Arguments:   - int16_t p[MLKEM_N]: pointer to in/output polynomial
 **************************************************/
static INLINE void ntt_native(int16_t p[MLKEM_N])
__contract__(
  requires(memory_no_alias(p, sizeof(int16_t) * MLKEM_N))
  requires(array_abs_bound(p, 0, MLKEM_N, MLKEM_Q))
  assigns(memory_slice(p, sizeof(int16_t) * MLKEM_N))
  ensures(array_abs_bound(p, 0, MLKEM_N, NTT_BOUND))
);
#endif /* MLKEM_USE_NATIVE_NTT */

#if defined(MLKEM_USE_NATIVE_NTT_CUSTOM_ORDER)
/*
 * This must only be set if NTT, invNTT, basemul, mulcache, and
 * to/from byte stream conversions all have native implementations
 * that are adapted to the custom order.
 */
#if !defined(MLKEM_USE_NATIVE_NTT) || !defined(MLKEM_USE_NATIVE_INTT) || \
    !defined(MLKEM_USE_NATIVE_POLY_MULCACHE_COMPUTE) ||                  \
    !defined(MLKEM_USE_NATIVE_POLYVEC_BASEMUL_ACC_MONTGOMERY_CACHED) ||  \
    !defined(MLKEM_USE_NATIVE_POLY_TOBYTES) ||                           \
    !defined(MLKEM_USE_NATIVE_POLY_FROMBYTES)
#error \
    "Invalid native profile: MLKEM_USE_NATIVE_NTT_CUSTOM_ORDER can only be \
set if there are native implementations for NTT, invNTT, mulcache, basemul, \
and to/from bytes conversions."
#endif

/*************************************************
 * Name:        poly_permute_bitrev_to_custom
 *
 * Description: When MLKEM_USE_NATIVE_NTT_CUSTOM_ORDER is defined,
 *              convert a polynomial in NTT domain from bitreversed
 *              order to the custom order output by the native NTT.
 *
 *              This must only be defined if there is native code for
 *              all of (a) NTT, (b) invNTT, (c) basemul, (d) mulcache.
 * Arguments:   - int16_t p[MLKEM_N]: pointer to in/output polynomial
 *
 **************************************************/
static INLINE void poly_permute_bitrev_to_custom(int16_t p[MLKEM_N])
__contract__(
  /* We don't specify that this should be a permutation, but only
   * that it does not change the bound established at the end of gen_matrix. */
  requires(memory_no_alias(p, sizeof(int16_t) * MLKEM_N))
  requires(array_bound(p, 0, MLKEM_N, 0, MLKEM_Q))
  assigns(memory_slice(p, sizeof(int16_t) * MLKEM_N))
  ensures(array_bound(p, 0, MLKEM_N, 0, MLKEM_Q)));
#endif /* MLKEM_USE_NATIVE_NTT_CUSTOM_ORDER */

#if defined(MLKEM_USE_NATIVE_INTT)
/*************************************************
 * Name:        intt_native
 *
 * Description: Computes inverse of negacyclic number-theoretic transform (NTT)
 *              of a polynomial in place.
 *
 *              The input polynomial is in bitreversed order, or of a
 *              custom order if MLKEM_USE_NATIVE_NTT_CUSTOM_ORDER is set.
 *              See the documentation of MLKEM_USE_NATIVE_NTT_CUSTOM_ORDER
 *              for more information.
 *              The output polynomial is assumed to be in normal order.
 *
 * Arguments:   - uint16_t *a: pointer to in/output polynomial
 **************************************************/
static INLINE void intt_native(int16_t p[MLKEM_N])
__contract__(
  requires(memory_no_alias(p, sizeof(int16_t) * MLKEM_N))
  assigns(memory_slice(p, sizeof(int16_t) * MLKEM_N))
  ensures(array_abs_bound(p, 0, MLKEM_N, INVNTT_BOUND))
);
#endif /* MLKEM_USE_NATIVE_INTT */

#if defined(MLKEM_USE_NATIVE_POLY_REDUCE)
/*************************************************
 * Name:        poly_reduce_native
 *
 * Description: Applies modular reduction to all coefficients of a polynomial.
 *
 * Arguments:   - int16_t r[MLKEM_N]: pointer to input/output polynomial
 **************************************************/
static INLINE void poly_reduce_native(int16_t p[MLKEM_N])
__contract__(
  requires(memory_no_alias(p, sizeof(int16_t) * MLKEM_N))
  assigns(memory_slice(p, sizeof(int16_t) * MLKEM_N))
  ensures(array_bound(p, 0, MLKEM_N, 0, MLKEM_Q))
);
#endif /* MLKEM_USE_NATIVE_POLY_REDUCE */

#if defined(MLKEM_USE_NATIVE_POLY_TOMONT)
/*************************************************
 * Name:        poly_tomont_native
 *
 * Description: Inplace conversion of all coefficients of a polynomial
 *              from normal domain to Montgomery domain
 *
 * Arguments:   - int16_t r[MLKEM_N]: pointer to input/output polynomial
 **************************************************/
static INLINE void poly_tomont_native(int16_t p[MLKEM_N])
__contract__(
  requires(memory_no_alias(p, sizeof(int16_t) * MLKEM_N))
  assigns(memory_slice(p, sizeof(int16_t) * MLKEM_N))
  ensures(array_abs_bound(p, 0, MLKEM_N, MLKEM_Q))
);
#endif /* MLKEM_USE_NATIVE_POLY_TOMONT */

#if defined(MLKEM_USE_NATIVE_POLY_MULCACHE_COMPUTE)
/*************************************************
 * Name:        poly_mulcache_compute_native
 *
 * Description: Compute multiplication cache for a polynomial
 *              in NTT domain.
 *
 *              The purpose of the multiplication cache is to
 *              cache repeated computations required during a
 *              base multiplication of polynomials in NTT domain.
 *              The structure of the multiplication-cache is
 *              implementation defined.
 *
 * Arguments:   INPUT:
 *              - poly: const pointer to input polynomial.
 *                  This must be in NTT domain and inin bitreversed order, or of
 *                  a custom order if MLKEM_USE_NATIVE_NTT_CUSTOM_ORDER is set.
 *                  See the documentation of MLKEM_USE_NATIVE_NTT_CUSTOM_ORDER
 *                  for more information.
 *              OUTPUT
 *              - cache: pointer to multiplication cache
 **************************************************/
static INLINE void poly_mulcache_compute_native(int16_t cache[MLKEM_N / 2],
                                                const int16_t poly[MLKEM_N])
__contract__(
  requires(memory_no_alias(cache, sizeof(int16_t) * (MLKEM_N / 2)))
  requires(memory_no_alias(poly, sizeof(int16_t) * MLKEM_N))
  assigns(object_whole(cache))
);
#endif /* MLKEM_USE_NATIVE_POLY_MULCACHE_COMPUTE */

#if defined(MLKEM_USE_NATIVE_POLYVEC_BASEMUL_ACC_MONTGOMERY_CACHED)
/*************************************************
 * Name:        poly_mulcache_compute_native
 *
 * Description: Compute multiplication of polynomials in NTT domain.
 *
 * Arguments:   INPUT:
 *              - a: First polynomial operand.
 *                 This must be in NTT domain and inin bitreversed order, or of
 *                 a custom order if MLKEM_USE_NATIVE_NTT_CUSTOM_ORDER is set.
 *                 See the documentation of MLKEM_USE_NATIVE_NTT_CUSTOM_ORDER
 *                 for more information.
 *              - b: Second polynomial operand.
 *                 As for a.
 *              - b_cache: Multiplication-cache for b.
 *              OUTPUT
 *              - r: Result of the base multiplication. This is again
 *                   in NTT domain, and of the same order as a and b.
 **************************************************/
static INLINE void polyvec_basemul_acc_montgomery_cached_native(
    int16_t r[MLKEM_N], const int16_t a[MLKEM_K * MLKEM_N],
    const int16_t b[MLKEM_K * MLKEM_N],
    const int16_t b_cache[MLKEM_K * (MLKEM_N / 2)])
__contract__(
  requires(memory_no_alias(r, sizeof(int16_t) * MLKEM_N))
  requires(memory_no_alias(a, sizeof(int16_t) * MLKEM_K * MLKEM_N))
  requires(memory_no_alias(b, sizeof(int16_t) * MLKEM_K * MLKEM_N))
  requires(memory_no_alias(b_cache, sizeof(int16_t) * MLKEM_K * (MLKEM_N / 2)))
  /* Because of https://github.com/diffblue/cbmc/issues/8570, we can't
   * just use a single flattened array_bound(...) here.
   *
   * Once fixed, change to:
   * ```
   * requires(array_bound(a, 0, MLKEM_K * MLKEM_N, 0, UINT12_LIMIT))
   * ```
   */
  requires(forall(kN, 0, MLKEM_K,					  \
              array_bound(&((int16_t(*)[MLKEM_N])(a))[kN][0], 0, MLKEM_N, \
			  0, UINT12_LIMIT)))
  assigns(memory_slice(r, sizeof(int16_t) * MLKEM_N))
);
#endif

#if defined(MLKEM_USE_NATIVE_POLY_TOBYTES)
/*************************************************
 * Name:        poly_tobytes_native
 *
 * Description: Serialization of a polynomial.
 *              Signed coefficients are converted to
 *              unsigned form before serialization.
 *
 * Arguments:   INPUT:
 *              - a: const pointer to input polynomial,
 *                with each coefficient in the range -Q+1 .. Q-1
 *              OUTPUT
 *              - r: pointer to output byte array
 *                   (of MLKEM_POLYBYTES bytes)
 **************************************************/
static INLINE void poly_tobytes_native(uint8_t r[MLKEM_POLYBYTES],
                                       const int16_t a[MLKEM_N])
__contract__(
  requires(memory_no_alias(r, MLKEM_POLYBYTES))
  requires(memory_no_alias(a, sizeof(int16_t) * MLKEM_N))
  requires(array_bound(a, 0, MLKEM_N, 0, MLKEM_Q))
  assigns(object_whole(r))
);
#endif /* MLKEM_USE_NATIVE_POLY_TOBYTES */

#if defined(MLKEM_USE_NATIVE_POLY_FROMBYTES)
/*************************************************
 * Name:        poly_frombytes_native
 *
 * Description: Serialization of a polynomial.
 *              Signed coefficients are converted to
 *              unsigned form before serialization.
 *
 * Arguments:   INPUT:
 *              - r: pointer to output polynomial in NTT domain
 *              OUTPUT
 *              - a: const pointer to input byte aray
 *                   (of MLKEM_POLYBYTES bytes)
 **************************************************/
static INLINE void poly_frombytes_native(int16_t a[MLKEM_N],
                                         const uint8_t r[MLKEM_POLYBYTES])
__contract__(
  requires(memory_no_alias(r, MLKEM_POLYBYTES))
  requires(memory_no_alias(a, sizeof(int16_t) * MLKEM_N))
  assigns(memory_slice(a, sizeof(int16_t) * MLKEM_N))
  ensures(array_bound(a, 0, MLKEM_N, 0, UINT12_LIMIT))
);
#endif /* MLKEM_USE_NATIVE_POLY_FROMBYTES */

#if defined(MLKEM_USE_NATIVE_REJ_UNIFORM)
/*************************************************
 * Name:        rej_uniform_native
 *
 * Description: Run rejection sampling on uniform random bytes to generate
 *              uniform random integers mod q
 *
 * Arguments:   - int16_t *r:          pointer to output buffer
 *              - unsigned len:        requested number of 16-bit integers
 *                                     (uniform mod q).
 *              - const uint8_t *buf:  pointer to input buffer
 *                                     (assumed to be uniform random bytes)
 *              - unsigned buflen:     length of input buffer in bytes.
 *
 * Return -1 if the native implementation does not support the input lengths.
 * Otherwise, returns non-negative number of sampled 16-bit integers (at most
 * len).
 **************************************************/
static INLINE int rej_uniform_native(int16_t *r, unsigned len,
                                     const uint8_t *buf, unsigned buflen)
__contract__(
  requires(len <= 4096 && buflen <= 4096 && buflen % 3 == 0)
  requires(memory_no_alias(r, sizeof(int16_t) * len))
  requires(memory_no_alias(buf, buflen))
  assigns(memory_slice(r, sizeof(int16_t) * len))
  ensures(return_value == -1 || (0 <= return_value && return_value <= len))
  ensures(return_value != -1 ==> array_bound(r, 0, (unsigned) return_value, 0, MLKEM_Q))
);
#endif /* MLKEM_USE_NATIVE_REJ_UNIFORM */

#if defined(MLKEM_NATIVE_MULTILEVEL_BUILD_WITH_SHARED) || \
    (MLKEM_K == 2 || MLKEM_K == 3)
#if defined(MLKEM_USE_NATIVE_POLY_COMPRESS_D4)
/*************************************************
 * Name:        poly_compress_d4_native
 *
 * Description: Compression (4 bits) and subsequent serialization of a
 *              polynomial
 *
 * Arguments:   - uint8_t *r: pointer to output byte array
 *                   (of length MLKEM_POLYCOMPRESSEDBYTES_D4 bytes)
 *              - const int16_t a[MLKEM_N]: pointer to input polynomial
 *                  Coefficients must be unsigned canonical,
 *                  i.e. in [0,1,..,MLKEM_Q-1].
 **************************************************/
static INLINE void poly_compress_d4_native(
    uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D4], const int16_t a[MLKEM_N]);
#endif /* MLKEM_USE_NATIVE_POLY_COMPRESS_D4 */

#if defined(MLKEM_USE_NATIVE_POLY_COMPRESS_D10)
/*************************************************
 * Name:        poly_compress_d10_native
 *
 * Description: Compression (10 bits) and subsequent serialization of a
 *              polynomial
 *
 * Arguments:   - uint8_t *r: pointer to output byte array
 *                   (of length MLKEM_POLYCOMPRESSEDBYTES_D10 bytes)
 *              - const int16_t a[MLKEM_N]: pointer to input polynomial
 *                  Coefficients must be unsigned canonical,
 *                  i.e. in [0,1,..,MLKEM_Q-1].
 **************************************************/
static INLINE void poly_compress_d10_native(
    uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D10], const int16_t a[MLKEM_N]);
#endif /* MLKEM_USE_NATIVE_POLY_COMPRESS_D10 */

#if defined(MLKEM_USE_NATIVE_POLY_DECOMPRESS_D4)
/*************************************************
 * Name:        poly_decompress_d4
 *
 * Description: De-serialization and subsequent decompression (dv bits) of a
 *              polynomial; approximate inverse of poly_compress
 *
 * Arguments:   - int16_t r[MLKEM_N]: pointer to output polynomial
 *              - const uint8_t *a: pointer to input byte array
 *                   (of length MLKEM_POLYCOMPRESSEDBYTES_D4 bytes)
 *
 * Upon return, the coefficients of the output polynomial are unsigned-canonical
 * (non-negative and smaller than MLKEM_Q).
 *
 **************************************************/
static INLINE void poly_decompress_d4_native(
    int16_t r[MLKEM_N], const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D4]);
#endif /* MLKEM_USE_NATIVE_POLY_DECOMPRESS_D4 */

#if defined(MLKEM_USE_NATIVE_POLY_DECOMPRESS_D10)
/*************************************************
 * Name:        poly_decompress_d10_native
 *
 * Description: De-serialization and subsequent decompression (10 bits) of a
 *              polynomial; approximate inverse of poly_compress_d10
 *
 * Arguments:   - int16_t r[MLKEM_N]: pointer to output polynomial
 *              - const uint8_t *a: pointer to input byte array
 *                   (of length MLKEM_POLYCOMPRESSEDBYTES_D10 bytes)
 *
 * Upon return, the coefficients of the output polynomial are unsigned-canonical
 * (non-negative and smaller than MLKEM_Q).
 *
 **************************************************/
static INLINE void poly_decompress_d10_native(
    int16_t r[MLKEM_N], const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D10]);
#endif /* MLKEM_USE_NATIVE_POLY_DECOMPRESS_D10 */
#endif /* defined(MLKEM_NATIVE_MULTILEVEL_BUILD_WITH_SHARED) || (MLKEM_K == 2 \
          || MLKEM_K == 3) */

#if defined(MLKEM_NATIVE_MULTILEVEL_BUILD_WITH_SHARED) || MLKEM_K == 4
#if defined(MLKEM_USE_NATIVE_POLY_COMPRESS_D5)
/*************************************************
 * Name:        poly_compress_d5_native
 *
 * Description: Compression (5 bits) and subsequent serialization of a
 *              polynomial
 *
 * Arguments:   - uint8_t *r: pointer to output byte array
 *                   (of length MLKEM_POLYCOMPRESSEDBYTES_D5 bytes)
 *              - const int16_t a[MLKEM_N]: pointer to input polynomial
 *                  Coefficients must be unsigned canonical,
 *                  i.e. in [0,1,..,MLKEM_Q-1].
 **************************************************/
static INLINE void poly_compress_d5_native(
    uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D5], const int16_t a[MLKEM_N]);
#endif /* MLKEM_USE_NATIVE_POLY_COMPRESS_D5 */

#if defined(MLKEM_USE_NATIVE_POLY_COMPRESS_D11)
/*************************************************
 * Name:        poly_compress_d11_native
 *
 * Description: Compression (11 bits) and subsequent serialization of a
 *              polynomial
 *
 * Arguments:   - uint8_t *r: pointer to output byte array
 *                   (of length MLKEM_POLYCOMPRESSEDBYTES_D11 bytes)
 *              - const int16_t a[MLKEM_N]: pointer to input polynomial
 *                  Coefficients must be unsigned canonical,
 *                  i.e. in [0,1,..,MLKEM_Q-1].
 **************************************************/
static INLINE void poly_compress_d11_native(
    uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D11], const int16_t a[MLKEM_N]);
#endif /* MLKEM_USE_NATIVE_POLY_COMPRESS_D11 */

#if defined(MLKEM_USE_NATIVE_POLY_DECOMPRESS_D5)
/*************************************************
 * Name:        poly_decompress_d5_native
 *
 * Description: De-serialization and subsequent decompression (dv bits) of a
 *              polynomial; approximate inverse of poly_compress
 *
 * Arguments:   - int16_t r[MLKEM_N]: pointer to output polynomial
 *              - const uint8_t *a: pointer to input byte array
 *                   (of length MLKEM_POLYCOMPRESSEDBYTES_D5 bytes)
 *
 * Upon return, the coefficients of the output polynomial are unsigned-canonical
 * (non-negative and smaller than MLKEM_Q).
 *
 **************************************************/
static INLINE void poly_decompress_d5_native(
    int16_t r[MLKEM_N], const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D5]);
#endif /* MLKEM_USE_NATIVE_POLY_DECOMPRESS_D5 */

#if defined(MLKEM_USE_NATIVE_POLY_DECOMPRESS_D11)
/*************************************************
 * Name:        poly_decompress_d11_native
 *
 * Description: De-serialization and subsequent decompression (11 bits) of a
 *              polynomial; approximate inverse of poly_compress_d11
 *
 * Arguments:   - int16_t r[MLKEM_N]: pointer to output polynomial
 *              - const uint8_t *a: pointer to input byte array
 *                   (of length MLKEM_POLYCOMPRESSEDBYTES_D11 bytes)
 *
 * Upon return, the coefficients of the output polynomial are unsigned-canonical
 * (non-negative and smaller than MLKEM_Q).
 *
 **************************************************/
static INLINE void poly_decompress_d11_native(
    int16_t r[MLKEM_N], const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D11]);
#endif /* MLKEM_USE_NATIVE_POLY_DECOMPRESS_D11 */
#endif /* defined(MLKEM_NATIVE_MULTILEVEL_BUILD_WITH_SHARED) || MLKEM_K == 4 \
        */

#endif /* MLKEM_NATIVE_ARITH_NATIVE_API_H */
