/*
 * Copyright (c) 2024-2025 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdint.h>
#include <string.h>

#include "arith_backend.h"
#include "compress.h"
#include "debug.h"
#include "poly_k.h"
#include "sampling.h"
#include "symmetric.h"

/* Level namespacing
 * This is to facilitate building multiple instances
 * of mlkem-native (e.g. with varying security levels)
 * within a single compilation unit. */
#define mlk_poly_cbd_eta1 MLK_ADD_LEVEL(mlk_poly_cbd_eta1)
#define mlk_poly_cbd_eta2 MLK_ADD_LEVEL(mlk_poly_cbd_eta2)
/* End of level namespacing */

/* Reference: `polyvec_compress()` in the reference implementation
 *            - In contrast to the reference implementation, we assume
 *              unsigned canonical coefficients here.
 *              The reference implementation works with coefficients
 *              in the range (-MLKEM_Q+1,...,MLKEM_Q-1). */
MLK_INTERNAL_API
void mlk_polyvec_compress_du(uint8_t r[MLKEM_POLYVECCOMPRESSEDBYTES_DU],
                             const mlk_polyvec *a)
{
  unsigned i;
  mlk_assert_bound_2d(a, MLKEM_K, MLKEM_N, 0, MLKEM_Q);

  for (i = 0; i < MLKEM_K; i++)
  {
    mlk_poly_compress_du(r + i * MLKEM_POLYCOMPRESSEDBYTES_DU, &a->vec[i]);
  }
}

/* Reference: `polyvec_decompress()` in the reference implementation. */
MLK_INTERNAL_API
void mlk_polyvec_decompress_du(mlk_polyvec *r,
                               const uint8_t a[MLKEM_POLYVECCOMPRESSEDBYTES_DU])
{
  unsigned i;
  for (i = 0; i < MLKEM_K; i++)
  {
    mlk_poly_decompress_du(&r->vec[i], a + i * MLKEM_POLYCOMPRESSEDBYTES_DU);
  }

  mlk_assert_bound_2d(r, MLKEM_K, MLKEM_N, 0, MLKEM_Q);
}

/* Reference: `polyvec_tobytes()` in the reference implementation.
 *            - In contrast to the reference implementation, we assume
 *              unsigned canonical coefficients here.
 *              The reference implementation works with coefficients
 *              in the range (-MLKEM_Q+1,...,MLKEM_Q-1). */
MLK_INTERNAL_API
void mlk_polyvec_tobytes(uint8_t r[MLKEM_POLYVECBYTES], const mlk_polyvec *a)
{
  unsigned i;
  mlk_assert_bound_2d(a, MLKEM_K, MLKEM_N, 0, MLKEM_Q);

  for (i = 0; i < MLKEM_K; i++)
  {
    mlk_poly_tobytes(r + i * MLKEM_POLYBYTES, &a->vec[i]);
  }
}

/* Reference: `polyvec_frombytes()` in the reference implementation. */
MLK_INTERNAL_API
void mlk_polyvec_frombytes(mlk_polyvec *r, const uint8_t a[MLKEM_POLYVECBYTES])
{
  unsigned i;
  for (i = 0; i < MLKEM_K; i++)
  {
    mlk_poly_frombytes(&r->vec[i], a + i * MLKEM_POLYBYTES);
  }

  mlk_assert_bound_2d(r, MLKEM_K, MLKEM_N, 0, MLKEM_UINT12_LIMIT);
}

/* Reference: `polyvec_ntt()` in the reference implementation. */
MLK_INTERNAL_API
void mlk_polyvec_ntt(mlk_polyvec *r)
{
  unsigned i;
  for (i = 0; i < MLKEM_K; i++)
  {
    mlk_poly_ntt(&r->vec[i]);
  }

  mlk_assert_abs_bound_2d(r, MLKEM_K, MLKEM_N, MLK_NTT_BOUND);
}

/* Reference: `polyvec_invntt_tomont()` in the reference implementation.
 *            - We normalize at the beginning of the inverse NTT,
 *              while the reference implementation normalizes at
 *              the end. This allows us to drop a call to `poly_reduce()`
 *              from the base multiplication. */
MLK_INTERNAL_API
void mlk_polyvec_invntt_tomont(mlk_polyvec *r)
{
  unsigned i;
  for (i = 0; i < MLKEM_K; i++)
  {
    mlk_poly_invntt_tomont(&r->vec[i]);
  }

  mlk_assert_abs_bound_2d(r, MLKEM_K, MLKEM_N, MLK_INVNTT_BOUND);
}

#if !defined(MLK_USE_NATIVE_POLYVEC_BASEMUL_ACC_MONTGOMERY_CACHED)
/* Reference: `polyvec_basemul_acc_montgomery()` in the
 *            reference implementation.
 *            - We use a multiplication cache ('mulcache') here
 *              which is not present in the reference implementation.
 *              This is an idea originally taken from https://ia.cr/2021/986
 *              and used at the C level here.
 *            - We compute the coefficients of the scalar product in 32-bit
 *              coefficients and perform only a single modular reduction
 *              at the end. The reference implementation uses 2 * MLKEM_K
 *              more modular reductions since it reduces after every modular
 *              multiplication. */
MLK_INTERNAL_API
void mlk_polyvec_basemul_acc_montgomery_cached(
    mlk_poly *r, const mlk_polyvec *a, const mlk_polyvec *b,
    const mlk_polyvec_mulcache *b_cache)
{
  unsigned i;
  mlk_assert_bound_2d(a, MLKEM_K, MLKEM_N, 0, MLKEM_UINT12_LIMIT);
  for (i = 0; i < MLKEM_N / 2; i++)
  __loop__(invariant(i <= MLKEM_N / 2))
  {
    unsigned k;
    int32_t t[2] = {0};
    for (k = 0; k < MLKEM_K; k++)
    __loop__(
      invariant(k <= MLKEM_K &&
         t[0] <=    (int32_t) k * 2 * MLKEM_UINT12_LIMIT * 32768  &&
         t[0] >= - ((int32_t) k * 2 * MLKEM_UINT12_LIMIT * 32768) &&
         t[1] <=   ((int32_t) k * 2 * MLKEM_UINT12_LIMIT * 32768) &&
         t[1] >= - ((int32_t) k * 2 * MLKEM_UINT12_LIMIT * 32768)))
    {
      t[0] += (int32_t)a->vec[k].coeffs[2 * i + 1] * b_cache->vec[k].coeffs[i];
      t[0] += (int32_t)a->vec[k].coeffs[2 * i] * b->vec[k].coeffs[2 * i];
      t[1] += (int32_t)a->vec[k].coeffs[2 * i] * b->vec[k].coeffs[2 * i + 1];
      t[1] += (int32_t)a->vec[k].coeffs[2 * i + 1] * b->vec[k].coeffs[2 * i];
    }
    r->coeffs[2 * i + 0] = mlk_montgomery_reduce(t[0]);
    r->coeffs[2 * i + 1] = mlk_montgomery_reduce(t[1]);
  }
}

#else /* !MLK_USE_NATIVE_POLYVEC_BASEMUL_ACC_MONTGOMERY_CACHED */
MLK_INTERNAL_API
void mlk_polyvec_basemul_acc_montgomery_cached(
    mlk_poly *r, const mlk_polyvec *a, const mlk_polyvec *b,
    const mlk_polyvec_mulcache *b_cache)
{
  mlk_assert_bound_2d(a, MLKEM_K, MLKEM_N, 0, MLKEM_UINT12_LIMIT);
  /* Omitting bounds assertion for cache since native implementations may
   * decide not to use a mulcache. Note that the C backend implementation
   * of poly_basemul_montgomery_cached() does still include the check. */
#if MLKEM_K == 2
  mlk_polyvec_basemul_acc_montgomery_cached_k2_native(
      r->coeffs, (const int16_t *)a, (const int16_t *)b,
      (const int16_t *)b_cache);
#elif MLKEM_K == 3
  mlk_polyvec_basemul_acc_montgomery_cached_k3_native(
      r->coeffs, (const int16_t *)a, (const int16_t *)b,
      (const int16_t *)b_cache);
#elif MLKEM_K == 4
  mlk_polyvec_basemul_acc_montgomery_cached_k4_native(
      r->coeffs, (const int16_t *)a, (const int16_t *)b,
      (const int16_t *)b_cache);
#endif
}
#endif /* MLK_USE_NATIVE_POLYVEC_BASEMUL_ACC_MONTGOMERY_CACHED */

/* Reference: Does not exist in the reference implementation.
 *            - The reference implementation does not use a
 *              multiplication cache ('mulcache'). This is an idea
 *              originally taken from https://ia.cr/2021/986
 *              and used at the C level here. */
MLK_INTERNAL_API
void mlk_polyvec_mulcache_compute(mlk_polyvec_mulcache *x, const mlk_polyvec *a)
{
  unsigned i;
  for (i = 0; i < MLKEM_K; i++)
  {
    mlk_poly_mulcache_compute(&x->vec[i], &a->vec[i]);
  }
}

/* Reference: `polyvec_reduce()` in the reference implementation.
 *            - We use _unsigned_ canonical outputs, while the reference
 *              implementation uses _signed_ canonical outputs.
 *              Accordingly, we need a conditional addition of MLKEM_Q
 *              here to go from signed to unsigned representatives.
 *              This conditional addition is then dropped from all
 *              polynomial compression functions instead (see `compress.c`). */
MLK_INTERNAL_API
void mlk_polyvec_reduce(mlk_polyvec *r)
{
  unsigned i;
  for (i = 0; i < MLKEM_K; i++)
  {
    mlk_poly_reduce(&r->vec[i]);
  }

  mlk_assert_bound_2d(r, MLKEM_K, MLKEM_N, 0, MLKEM_Q);
}

/* Reference: `polyvec_add()` in the reference implementation.
 *            - We use destructive version (output=first input) to avoid
 *              reasoning about aliasing in the CBMC specification */
MLK_INTERNAL_API
void mlk_polyvec_add(mlk_polyvec *r, const mlk_polyvec *b)
{
  unsigned i;
  for (i = 0; i < MLKEM_K; i++)
  {
    mlk_poly_add(&r->vec[i], &b->vec[i]);
  }
}

/* Reference: `polyvec_tomont()` in the reference implementation. */
MLK_INTERNAL_API
void mlk_polyvec_tomont(mlk_polyvec *r)
{
  unsigned i;
  for (i = 0; i < MLKEM_K; i++)
  {
    mlk_poly_tomont(&r->vec[i]);
  }

  mlk_assert_abs_bound_2d(r, MLKEM_K, MLKEM_N, MLKEM_Q);
}


/*************************************************
 * Name:        mlk_poly_cbd_eta1
 *
 * Description: Given an array of uniformly random bytes, compute
 *              polynomial with coefficients distributed according to
 *              a centered binomial distribution with parameter MLKEM_ETA1.
 *
 * Arguments:   - mlk_poly *r: pointer to output polynomial
 *              - const uint8_t *buf: pointer to input byte array
 *
 * Specification: Implements [FIPS 203, Algorithm 8, SamplePolyCBD_eta1], where
 *                eta1 is specified per level in [FIPS 203, Table 2]
 *                and represented as MLKEM_ETA1 here.
 *
 **************************************************/

/* Reference: `poly_cbd_eta1` in the reference implementation. */
static MLK_INLINE void mlk_poly_cbd_eta1(
    mlk_poly *r, const uint8_t buf[MLKEM_ETA1 * MLKEM_N / 4])
__contract__(
  requires(memory_no_alias(r, sizeof(mlk_poly)))
  requires(memory_no_alias(buf, MLKEM_ETA1 * MLKEM_N / 4))
  assigns(memory_slice(r, sizeof(mlk_poly)))
  ensures(array_abs_bound(r->coeffs, 0, MLKEM_N, MLKEM_ETA1 + 1))
)
{
#if MLKEM_ETA1 == 2
  mlk_poly_cbd2(r, buf);
#elif MLKEM_ETA1 == 3
  mlk_poly_cbd3(r, buf);
#else
#error "Invalid value of MLKEM_ETA1"
#endif
}

/* Reference: Does not exist in the reference implementation.
 *            - This implements a x4-batched version of `poly_getnoise_eta1()`
 *              from the reference implementation, to leverage
 *              batched Keccak-f1600.*/
MLK_INTERNAL_API
void mlk_poly_getnoise_eta1_4x(mlk_poly *r0, mlk_poly *r1, mlk_poly *r2,
                               mlk_poly *r3, const uint8_t seed[MLKEM_SYMBYTES],
                               uint8_t nonce0, uint8_t nonce1, uint8_t nonce2,
                               uint8_t nonce3)
{
  MLK_ALIGN uint8_t buf0[MLKEM_ETA1 * MLKEM_N / 4];
  MLK_ALIGN uint8_t buf1[MLKEM_ETA1 * MLKEM_N / 4];
  MLK_ALIGN uint8_t buf2[MLKEM_ETA1 * MLKEM_N / 4];
  MLK_ALIGN uint8_t buf3[MLKEM_ETA1 * MLKEM_N / 4];
  MLK_ALIGN uint8_t extkey0[MLKEM_SYMBYTES + 1];
  MLK_ALIGN uint8_t extkey1[MLKEM_SYMBYTES + 1];
  MLK_ALIGN uint8_t extkey2[MLKEM_SYMBYTES + 1];
  MLK_ALIGN uint8_t extkey3[MLKEM_SYMBYTES + 1];
  memcpy(extkey0, seed, MLKEM_SYMBYTES);
  memcpy(extkey1, seed, MLKEM_SYMBYTES);
  memcpy(extkey2, seed, MLKEM_SYMBYTES);
  memcpy(extkey3, seed, MLKEM_SYMBYTES);
  extkey0[MLKEM_SYMBYTES] = nonce0;
  extkey1[MLKEM_SYMBYTES] = nonce1;
  extkey2[MLKEM_SYMBYTES] = nonce2;
  extkey3[MLKEM_SYMBYTES] = nonce3;
  mlk_prf_eta1_x4(buf0, buf1, buf2, buf3, extkey0, extkey1, extkey2, extkey3);
  mlk_poly_cbd_eta1(r0, buf0);
  mlk_poly_cbd_eta1(r1, buf1);
  mlk_poly_cbd_eta1(r2, buf2);
  mlk_poly_cbd_eta1(r3, buf3);

  mlk_assert_abs_bound(r0, MLKEM_N, MLKEM_ETA1 + 1);
  mlk_assert_abs_bound(r1, MLKEM_N, MLKEM_ETA1 + 1);
  mlk_assert_abs_bound(r2, MLKEM_N, MLKEM_ETA1 + 1);
  mlk_assert_abs_bound(r3, MLKEM_N, MLKEM_ETA1 + 1);

  /* Specification: Partially implements
   * [FIPS 203, Section 3.3, Destruction of intermediate values] */
  mlk_zeroize(buf0, sizeof(buf0));
  mlk_zeroize(buf1, sizeof(buf1));
  mlk_zeroize(buf2, sizeof(buf2));
  mlk_zeroize(buf3, sizeof(buf3));
  mlk_zeroize(extkey0, sizeof(extkey0));
  mlk_zeroize(extkey1, sizeof(extkey1));
  mlk_zeroize(extkey2, sizeof(extkey2));
  mlk_zeroize(extkey3, sizeof(extkey3));
}

#if MLKEM_K == 2 || MLKEM_K == 4
/*************************************************
 * Name:        mlk_poly_cbd_eta2
 *
 * Description: Given an array of uniformly random bytes, compute
 *              polynomial with coefficients distributed according to
 *              a centered binomial distribution with parameter MLKEM_ETA2.
 *
 * Arguments:   - mlk_poly *r: pointer to output polynomial
 *              - const uint8_t *buf: pointer to input byte array
 *
 * Specification: Implements [FIPS 203, Algorithm 8, SamplePolyCBD_eta2], where
 *                eta2 is specified per level in [FIPS 203, Table 2]
 *                and represented as MLKEM_ETA2 here.
 *
 **************************************************/

/* Reference: `poly_cbd_eta2` in the reference implementation. */
static MLK_INLINE void mlk_poly_cbd_eta2(
    mlk_poly *r, const uint8_t buf[MLKEM_ETA2 * MLKEM_N / 4])
__contract__(
  requires(memory_no_alias(r, sizeof(mlk_poly)))
  requires(memory_no_alias(buf, MLKEM_ETA2 * MLKEM_N / 4))
  assigns(memory_slice(r, sizeof(mlk_poly)))
  ensures(array_abs_bound(r->coeffs, 0, MLKEM_N, MLKEM_ETA2 + 1)))
{
#if MLKEM_ETA2 == 2
  mlk_poly_cbd2(r, buf);
#else
#error "Invalid value of MLKEM_ETA2"
#endif
}

/* Reference: `poly_getnoise_eta1()` in the reference implementation.
 *            - We include buffer zeroization. */
MLK_INTERNAL_API
void mlk_poly_getnoise_eta2(mlk_poly *r, const uint8_t seed[MLKEM_SYMBYTES],
                            uint8_t nonce)
{
  MLK_ALIGN uint8_t buf[MLKEM_ETA2 * MLKEM_N / 4];
  MLK_ALIGN uint8_t extkey[MLKEM_SYMBYTES + 1];

  memcpy(extkey, seed, MLKEM_SYMBYTES);
  extkey[MLKEM_SYMBYTES] = nonce;
  mlk_prf_eta2(buf, extkey);

  mlk_poly_cbd_eta2(r, buf);

  mlk_assert_abs_bound(r, MLKEM_N, MLKEM_ETA1 + 1);

  /* Specification: Partially implements
   * [FIPS 203, Section 3.3, Destruction of intermediate values] */
  mlk_zeroize(buf, sizeof(buf));
  mlk_zeroize(extkey, sizeof(extkey));
}
#endif /* MLKEM_K == 2 || MLKEM_K == 4 */

#if MLKEM_K == 2
/* Reference: Does not exist in the reference implementation.
 *            - This implements a x4-batched version of `poly_getnoise_eta1()`
 *              and `poly_getnoise_eta1()` from the reference implementation,
 *              leveraging batched Keccak-f1600.
 *            - If a x4-batched Keccak-f1600 is available, we squeeze
 *              more random data than needed for the eta2 calls, to be
 *              be able to use a x4-batched Keccak-f1600. */
MLK_INTERNAL_API
void mlk_poly_getnoise_eta1122_4x(mlk_poly *r0, mlk_poly *r1, mlk_poly *r2,
                                  mlk_poly *r3,
                                  const uint8_t seed[MLKEM_SYMBYTES],
                                  uint8_t nonce0, uint8_t nonce1,
                                  uint8_t nonce2, uint8_t nonce3)
{
#if MLKEM_ETA2 >= MLKEM_ETA1
#error mlk_poly_getnoise_eta1122_4x assumes MLKEM_ETA1 > MLKEM_ETA2
#endif
  MLK_ALIGN uint8_t buf0[MLKEM_ETA1 * MLKEM_N / 4];
  MLK_ALIGN uint8_t buf1[MLKEM_ETA1 * MLKEM_N / 4];
  /* Pad to larger buffer */
  MLK_ALIGN uint8_t buf2[MLKEM_ETA1 * MLKEM_N / 4];
  MLK_ALIGN uint8_t buf3[MLKEM_ETA1 * MLKEM_N / 4];

  MLK_ALIGN uint8_t extkey0[MLKEM_SYMBYTES + 1];
  MLK_ALIGN uint8_t extkey1[MLKEM_SYMBYTES + 1];
  MLK_ALIGN uint8_t extkey2[MLKEM_SYMBYTES + 1];
  MLK_ALIGN uint8_t extkey3[MLKEM_SYMBYTES + 1];

  memcpy(extkey0, seed, MLKEM_SYMBYTES);
  memcpy(extkey1, seed, MLKEM_SYMBYTES);
  memcpy(extkey2, seed, MLKEM_SYMBYTES);
  memcpy(extkey3, seed, MLKEM_SYMBYTES);
  extkey0[MLKEM_SYMBYTES] = nonce0;
  extkey1[MLKEM_SYMBYTES] = nonce1;
  extkey2[MLKEM_SYMBYTES] = nonce2;
  extkey3[MLKEM_SYMBYTES] = nonce3;

  /* On systems with fast batched Keccak, we use 4-fold batched PRF,
   * even though that means generating more random data in buf2 and buf3
   * than necessary. */
#if !defined(FIPS202_X4_DEFAULT_IMPLEMENTATION)
  mlk_prf_eta1_x4(buf0, buf1, buf2, buf3, extkey0, extkey1, extkey2, extkey3);
#else  /* FIPS202_X4_DEFAULT_IMPLEMENTATION */
  mlk_prf_eta1(buf0, extkey0);
  mlk_prf_eta1(buf1, extkey1);
  mlk_prf_eta2(buf2, extkey2);
  mlk_prf_eta2(buf3, extkey3);
#endif /* FIPS202_X4_DEFAULT_IMPLEMENTATION */

  mlk_poly_cbd_eta1(r0, buf0);
  mlk_poly_cbd_eta1(r1, buf1);
  mlk_poly_cbd_eta2(r2, buf2);
  mlk_poly_cbd_eta2(r3, buf3);

  mlk_assert_abs_bound(r0, MLKEM_N, MLKEM_ETA1 + 1);
  mlk_assert_abs_bound(r1, MLKEM_N, MLKEM_ETA1 + 1);
  mlk_assert_abs_bound(r2, MLKEM_N, MLKEM_ETA2 + 1);
  mlk_assert_abs_bound(r3, MLKEM_N, MLKEM_ETA2 + 1);

  /* Specification: Partially implements
   * [FIPS 203, Section 3.3, Destruction of intermediate values] */
  mlk_zeroize(buf0, sizeof(buf0));
  mlk_zeroize(buf1, sizeof(buf1));
  mlk_zeroize(buf2, sizeof(buf2));
  mlk_zeroize(buf3, sizeof(buf3));
  mlk_zeroize(extkey0, sizeof(extkey0));
  mlk_zeroize(extkey1, sizeof(extkey1));
  mlk_zeroize(extkey2, sizeof(extkey2));
  mlk_zeroize(extkey3, sizeof(extkey3));
}
#endif /* MLKEM_K == 2 */

/* To facilitate single-compilation-unit (SCU) builds, undefine all macros.
 * Don't modify by hand -- this is auto-generated by scripts/autogen. */
#undef mlk_poly_cbd_eta1
#undef mlk_poly_cbd_eta2
