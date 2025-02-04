/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#include "poly_k.h"
#include <stdint.h>
#include <string.h>
#include "arith_backend.h"
#include "compress.h"
#include "sampling.h"
#include "symmetric.h"

#include "debug.h"

/* Static namespacing
 * This is to facilitate building multiple instances
 * of mlkem-native (e.g. with varying security levels)
 * within a single compilation unit. */
#define poly_cbd_eta1 MLKEM_NAMESPACE_K(poly_cbd_eta1)
#define poly_cbd_eta2 MLKEM_NAMESPACE_K(poly_cbd_eta2)
/* End of static namespacing */

MLKEM_NATIVE_INTERNAL_API
void polyvec_compress_du(uint8_t r[MLKEM_POLYVECCOMPRESSEDBYTES_DU],
                         const polyvec *a)
{
  unsigned i;
  debug_assert_bound_2d(a, MLKEM_K, MLKEM_N, 0, MLKEM_Q);

  for (i = 0; i < MLKEM_K; i++)
  {
    poly_compress_du(r + i * MLKEM_POLYCOMPRESSEDBYTES_DU, &a->vec[i]);
  }
}

MLKEM_NATIVE_INTERNAL_API
void polyvec_decompress_du(polyvec *r,
                           const uint8_t a[MLKEM_POLYVECCOMPRESSEDBYTES_DU])
{
  unsigned i;
  for (i = 0; i < MLKEM_K; i++)
  {
    poly_decompress_du(&r->vec[i], a + i * MLKEM_POLYCOMPRESSEDBYTES_DU);
  }

  debug_assert_bound_2d(r, MLKEM_K, MLKEM_N, 0, MLKEM_Q);
}

MLKEM_NATIVE_INTERNAL_API
void polyvec_tobytes(uint8_t r[MLKEM_POLYVECBYTES], const polyvec *a)
{
  unsigned i;
  debug_assert_bound_2d(a, MLKEM_K, MLKEM_N, 0, MLKEM_Q);

  for (i = 0; i < MLKEM_K; i++)
  {
    poly_tobytes(r + i * MLKEM_POLYBYTES, &a->vec[i]);
  }
}

MLKEM_NATIVE_INTERNAL_API
void polyvec_frombytes(polyvec *r, const uint8_t a[MLKEM_POLYVECBYTES])
{
  unsigned i;
  for (i = 0; i < MLKEM_K; i++)
  {
    poly_frombytes(&r->vec[i], a + i * MLKEM_POLYBYTES);
  }

  debug_assert_bound_2d(r, MLKEM_K, MLKEM_N, 0, UINT12_LIMIT);
}

MLKEM_NATIVE_INTERNAL_API
void polyvec_ntt(polyvec *r)
{
  unsigned i;
  for (i = 0; i < MLKEM_K; i++)
  {
    poly_ntt(&r->vec[i]);
  }

  debug_assert_abs_bound_2d(r, MLKEM_K, MLKEM_N, NTT_BOUND);
}

MLKEM_NATIVE_INTERNAL_API
void polyvec_invntt_tomont(polyvec *r)
{
  unsigned i;
  for (i = 0; i < MLKEM_K; i++)
  {
    poly_invntt_tomont(&r->vec[i]);
  }

  debug_assert_abs_bound_2d(r, MLKEM_K, MLKEM_N, INVNTT_BOUND);
}

#if !defined(MLKEM_USE_NATIVE_POLYVEC_BASEMUL_ACC_MONTGOMERY_CACHED)
MLKEM_NATIVE_INTERNAL_API
void polyvec_basemul_acc_montgomery_cached(poly *r, const polyvec *a,
                                           const polyvec *b,
                                           const polyvec_mulcache *b_cache)
{
  unsigned i;
  poly t;
  debug_assert_bound_2d(a, MLKEM_K, MLKEM_N, 0, UINT12_LIMIT);

  poly_basemul_montgomery_cached(r, &a->vec[0], &b->vec[0], &b_cache->vec[0]);
  for (i = 1; i < MLKEM_K; i++)
  {
    poly_basemul_montgomery_cached(&t, &a->vec[i], &b->vec[i],
                                   &b_cache->vec[i]);
    poly_add(r, &t);
  }

  /*
   * This bound is true for the C implementation, but not needed
   * in the higher level bounds reasoning. It is thus omitted
   * them from the spec to not unnecessarily constrain native
   * implementations, but checked here nonetheless.
   */
  debug_assert_abs_bound(r, MLKEM_K, MLKEM_N * 2 * MLKEM_Q);
}
#else  /* !MLKEM_USE_NATIVE_POLYVEC_BASEMUL_ACC_MONTGOMERY_CACHED */
MLKEM_NATIVE_INTERNAL_API
void polyvec_basemul_acc_montgomery_cached(poly *r, const polyvec *a,
                                           const polyvec *b,
                                           const polyvec_mulcache *b_cache)
{
  debug_assert_bound_2d(a, MLKEM_K, MLKEM_N, 0, UINT12_LIMIT);
  /* Omitting bounds assertion for cache since native implementations may
   * decide not to use a mulcache. Note that the C backend implementation
   * of poly_basemul_montgomery_cached() does still include the check. */
  polyvec_basemul_acc_montgomery_cached_native(r->coeffs, (const int16_t *)a,
                                               (const int16_t *)b,
                                               (const int16_t *)b_cache);
}
#endif /* MLKEM_USE_NATIVE_POLYVEC_BASEMUL_ACC_MONTGOMERY_CACHED */

MLKEM_NATIVE_INTERNAL_API
void polyvec_basemul_acc_montgomery(poly *r, const polyvec *a, const polyvec *b)
{
  polyvec_mulcache b_cache;
  polyvec_mulcache_compute(&b_cache, b);
  polyvec_basemul_acc_montgomery_cached(r, a, b, &b_cache);
}

MLKEM_NATIVE_INTERNAL_API
void polyvec_mulcache_compute(polyvec_mulcache *x, const polyvec *a)
{
  unsigned i;
  for (i = 0; i < MLKEM_K; i++)
  {
    poly_mulcache_compute(&x->vec[i], &a->vec[i]);
  }
}

MLKEM_NATIVE_INTERNAL_API
void polyvec_reduce(polyvec *r)
{
  unsigned i;
  for (i = 0; i < MLKEM_K; i++)
  {
    poly_reduce(&r->vec[i]);
  }

  debug_assert_bound_2d(r, MLKEM_K, MLKEM_N, 0, MLKEM_Q);
}

MLKEM_NATIVE_INTERNAL_API
void polyvec_add(polyvec *r, const polyvec *b)
{
  unsigned i;
  for (i = 0; i < MLKEM_K; i++)
  {
    poly_add(&r->vec[i], &b->vec[i]);
  }
}

MLKEM_NATIVE_INTERNAL_API
void polyvec_tomont(polyvec *r)
{
  unsigned i;
  for (i = 0; i < MLKEM_K; i++)
  {
    poly_tomont(&r->vec[i]);
  }

  debug_assert_abs_bound_2d(r, MLKEM_K, MLKEM_N, MLKEM_Q);
}


/*************************************************
 * Name:        poly_cbd_eta1
 *
 * Description: Given an array of uniformly random bytes, compute
 *              polynomial with coefficients distributed according to
 *              a centered binomial distribution with parameter MLKEM_ETA1.
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const uint8_t *buf: pointer to input byte array
 **************************************************/
static INLINE void poly_cbd_eta1(poly *r,
                                 const uint8_t buf[MLKEM_ETA1 * MLKEM_N / 4])
__contract__(
  requires(memory_no_alias(r, sizeof(poly)))
  requires(memory_no_alias(buf, MLKEM_ETA1 * MLKEM_N / 4))
  assigns(memory_slice(r, sizeof(poly)))
  ensures(array_abs_bound(r->coeffs, 0, MLKEM_N, MLKEM_ETA1 + 1))
)
{
#if MLKEM_ETA1 == 2
  poly_cbd2(r, buf);
#elif MLKEM_ETA1 == 3
  poly_cbd3(r, buf);
#else
#error "Invalid value of MLKEM_ETA1"
#endif
}

MLKEM_NATIVE_INTERNAL_API
void poly_getnoise_eta1_4x(poly *r0, poly *r1, poly *r2, poly *r3,
                           const uint8_t seed[MLKEM_SYMBYTES], uint8_t nonce0,
                           uint8_t nonce1, uint8_t nonce2, uint8_t nonce3)
{
  ALIGN uint8_t buf0[MLKEM_ETA1 * MLKEM_N / 4];
  ALIGN uint8_t buf1[MLKEM_ETA1 * MLKEM_N / 4];
  ALIGN uint8_t buf2[MLKEM_ETA1 * MLKEM_N / 4];
  ALIGN uint8_t buf3[MLKEM_ETA1 * MLKEM_N / 4];
  ALIGN uint8_t extkey0[MLKEM_SYMBYTES + 1];
  ALIGN uint8_t extkey1[MLKEM_SYMBYTES + 1];
  ALIGN uint8_t extkey2[MLKEM_SYMBYTES + 1];
  ALIGN uint8_t extkey3[MLKEM_SYMBYTES + 1];
  memcpy(extkey0, seed, MLKEM_SYMBYTES);
  memcpy(extkey1, seed, MLKEM_SYMBYTES);
  memcpy(extkey2, seed, MLKEM_SYMBYTES);
  memcpy(extkey3, seed, MLKEM_SYMBYTES);
  extkey0[MLKEM_SYMBYTES] = nonce0;
  extkey1[MLKEM_SYMBYTES] = nonce1;
  extkey2[MLKEM_SYMBYTES] = nonce2;
  extkey3[MLKEM_SYMBYTES] = nonce3;
  prf_eta1_x4(buf0, buf1, buf2, buf3, extkey0, extkey1, extkey2, extkey3);
  poly_cbd_eta1(r0, buf0);
  poly_cbd_eta1(r1, buf1);
  poly_cbd_eta1(r2, buf2);
  poly_cbd_eta1(r3, buf3);

  debug_assert_abs_bound(r0, MLKEM_N, MLKEM_ETA1 + 1);
  debug_assert_abs_bound(r1, MLKEM_N, MLKEM_ETA1 + 1);
  debug_assert_abs_bound(r2, MLKEM_N, MLKEM_ETA1 + 1);
  debug_assert_abs_bound(r3, MLKEM_N, MLKEM_ETA1 + 1);
}

#if MLKEM_K == 2 || MLKEM_K == 4
/*************************************************
 * Name:        poly_cbd_eta2
 *
 * Description: Given an array of uniformly random bytes, compute
 *              polynomial with coefficients distributed according to
 *              a centered binomial distribution with parameter MLKEM_ETA2.
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const uint8_t *buf: pointer to input byte array
 **************************************************/
static INLINE void poly_cbd_eta2(poly *r,
                                 const uint8_t buf[MLKEM_ETA2 * MLKEM_N / 4])
__contract__(
  requires(memory_no_alias(r, sizeof(poly)))
  requires(memory_no_alias(buf, MLKEM_ETA2 * MLKEM_N / 4))
  assigns(memory_slice(r, sizeof(poly)))
  ensures(array_abs_bound(r->coeffs, 0, MLKEM_N, MLKEM_ETA2 + 1)))
{
#if MLKEM_ETA2 == 2
  poly_cbd2(r, buf);
#else
#error "Invalid value of MLKEM_ETA2"
#endif
}

MLKEM_NATIVE_INTERNAL_API
void poly_getnoise_eta2(poly *r, const uint8_t seed[MLKEM_SYMBYTES],
                        uint8_t nonce)
{
  ALIGN uint8_t buf[MLKEM_ETA2 * MLKEM_N / 4];
  ALIGN uint8_t extkey[MLKEM_SYMBYTES + 1];

  memcpy(extkey, seed, MLKEM_SYMBYTES);
  extkey[MLKEM_SYMBYTES] = nonce;
  prf_eta2(buf, extkey);

  poly_cbd_eta2(r, buf);

  debug_assert_abs_bound(r, MLKEM_N, MLKEM_ETA1 + 1);
}
#endif /* MLKEM_K == 2 || MLKEM_K == 4 */


#if MLKEM_K == 2
MLKEM_NATIVE_INTERNAL_API
void poly_getnoise_eta1122_4x(poly *r0, poly *r1, poly *r2, poly *r3,
                              const uint8_t seed[MLKEM_SYMBYTES],
                              uint8_t nonce0, uint8_t nonce1, uint8_t nonce2,
                              uint8_t nonce3)
{
#if MLKEM_ETA2 >= MLKEM_ETA1
#error poly_getnoise_eta1122_4x assumes MLKEM_ETA1 > MLKEM_ETA2
#endif
  ALIGN uint8_t buf0[MLKEM_ETA1 * MLKEM_N / 4];
  ALIGN uint8_t buf1[MLKEM_ETA1 * MLKEM_N / 4];
  /* Pad to larger buffer */
  ALIGN uint8_t buf2[MLKEM_ETA1 * MLKEM_N / 4];
  ALIGN uint8_t buf3[MLKEM_ETA1 * MLKEM_N / 4];

  ALIGN uint8_t extkey0[MLKEM_SYMBYTES + 1];
  ALIGN uint8_t extkey1[MLKEM_SYMBYTES + 1];
  ALIGN uint8_t extkey2[MLKEM_SYMBYTES + 1];
  ALIGN uint8_t extkey3[MLKEM_SYMBYTES + 1];

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
  prf_eta1_x4(buf0, buf1, buf2, buf3, extkey0, extkey1, extkey2, extkey3);
#else  /* FIPS202_X4_DEFAULT_IMPLEMENTATION */
  prf_eta1(buf0, extkey0);
  prf_eta1(buf1, extkey1);
  prf_eta2(buf2, extkey2);
  prf_eta2(buf3, extkey3);
#endif /* FIPS202_X4_DEFAULT_IMPLEMENTATION */

  poly_cbd_eta1(r0, buf0);
  poly_cbd_eta1(r1, buf1);
  poly_cbd_eta2(r2, buf2);
  poly_cbd_eta2(r3, buf3);

  debug_assert_abs_bound(r0, MLKEM_N, MLKEM_ETA1 + 1);
  debug_assert_abs_bound(r1, MLKEM_N, MLKEM_ETA1 + 1);
  debug_assert_abs_bound(r2, MLKEM_N, MLKEM_ETA2 + 1);
  debug_assert_abs_bound(r3, MLKEM_N, MLKEM_ETA2 + 1);
}
#endif /* MLKEM_K == 2 */
