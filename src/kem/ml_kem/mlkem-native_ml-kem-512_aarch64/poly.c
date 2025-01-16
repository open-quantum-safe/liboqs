/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdint.h>
#include <string.h>

#include "arith_backend.h"
#include "cbd.h"
#include "cbmc.h"
#include "debug/debug.h"
#include "fips202x4.h"
#include "ntt.h"
#include "poly.h"
#include "reduce.h"
#include "symmetric.h"
#include "verify.h"

MLKEM_NATIVE_INTERNAL_API
void poly_compress_du(uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_DU], const poly *a)
{
  unsigned j;
#if (MLKEM_POLYCOMPRESSEDBYTES_DU == 352)
  for (j = 0; j < MLKEM_N / 8; j++)
  __loop__(invariant(j >= 0 && j <= MLKEM_N / 8))
  {
    unsigned k;
    uint16_t t[8];
    for (k = 0; k < 8; k++)
    __loop__(
      invariant(k >= 0 && k <= 8)
      invariant(forall(r, 0, k, t[r] < (1u << 11))))
    {
      t[k] = scalar_compress_d11(a->coeffs[8 * j + k]);
    }

    /*
     * Make all implicit truncation explicit. No data is being
     * truncated for the LHS's since each t[i] is 11-bit in size.
     */
    r[11 * j + 0] = (t[0] >> 0) & 0xFF;
    r[11 * j + 1] = (t[0] >> 8) | ((t[1] << 3) & 0xFF);
    r[11 * j + 2] = (t[1] >> 5) | ((t[2] << 6) & 0xFF);
    r[11 * j + 3] = (t[2] >> 2) & 0xFF;
    r[11 * j + 4] = (t[2] >> 10) | ((t[3] << 1) & 0xFF);
    r[11 * j + 5] = (t[3] >> 7) | ((t[4] << 4) & 0xFF);
    r[11 * j + 6] = (t[4] >> 4) | ((t[5] << 7) & 0xFF);
    r[11 * j + 7] = (t[5] >> 1) & 0xFF;
    r[11 * j + 8] = (t[5] >> 9) | ((t[6] << 2) & 0xFF);
    r[11 * j + 9] = (t[6] >> 6) | ((t[7] << 5) & 0xFF);
    r[11 * j + 10] = (t[7] >> 3);
  }

#elif (MLKEM_POLYCOMPRESSEDBYTES_DU == 320)
  for (j = 0; j < MLKEM_N / 4; j++)
  __loop__(invariant(j >= 0 && j <= MLKEM_N / 4))
  {
    unsigned k;
    uint16_t t[4];
    for (k = 0; k < 4; k++)
    __loop__(
      invariant(k >= 0 && k <= 4)
      invariant(forall(r, 0, k, t[r] < (1u << 10))))
    {
      t[k] = scalar_compress_d10(a->coeffs[4 * j + k]);
    }

    /*
     * Make all implicit truncation explicit. No data is being
     * truncated for the LHS's since each t[i] is 10-bit in size.
     */
    r[5 * j + 0] = (t[0] >> 0) & 0xFF;
    r[5 * j + 1] = (t[0] >> 8) | ((t[1] << 2) & 0xFF);
    r[5 * j + 2] = (t[1] >> 6) | ((t[2] << 4) & 0xFF);
    r[5 * j + 3] = (t[2] >> 4) | ((t[3] << 6) & 0xFF);
    r[5 * j + 4] = (t[3] >> 2);
  }
#else
#error "MLKEM_POLYCOMPRESSEDBYTES_DU needs to be in {320,352}"
#endif
}


MLKEM_NATIVE_INTERNAL_API
void poly_decompress_du(poly *r, const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_DU])
{
  unsigned j;
#if (MLKEM_POLYCOMPRESSEDBYTES_DU == 352)
  for (j = 0; j < MLKEM_N / 8; j++)
  __loop__(
    invariant(0 <= j && j <= MLKEM_N / 8)
    invariant(array_bound(r->coeffs, 0, 8 * j, 0, MLKEM_Q)))
  {
    int k;
    uint16_t t[8];
    uint8_t const *base = &a[11 * j];
    t[0] = 0x7FF & ((base[0] >> 0) | ((uint16_t)base[1] << 8));
    t[1] = 0x7FF & ((base[1] >> 3) | ((uint16_t)base[2] << 5));
    t[2] = 0x7FF & ((base[2] >> 6) | ((uint16_t)base[3] << 2) |
                    ((uint16_t)base[4] << 10));
    t[3] = 0x7FF & ((base[4] >> 1) | ((uint16_t)base[5] << 7));
    t[4] = 0x7FF & ((base[5] >> 4) | ((uint16_t)base[6] << 4));
    t[5] = 0x7FF & ((base[6] >> 7) | ((uint16_t)base[7] << 1) |
                    ((uint16_t)base[8] << 9));
    t[6] = 0x7FF & ((base[8] >> 2) | ((uint16_t)base[9] << 6));
    t[7] = 0x7FF & ((base[9] >> 5) | ((uint16_t)base[10] << 3));

    for (k = 0; k < 8; k++)
    __loop__(
      invariant(0 <= k && k <= 8)
      invariant(array_bound(r->coeffs, 0, 8 * j + k, 0, MLKEM_Q)))
    {
      r->coeffs[8 * j + k] = scalar_decompress_d11(t[k]);
    }
  }
#elif (MLKEM_POLYCOMPRESSEDBYTES_DU == 320)
  for (j = 0; j < MLKEM_N / 4; j++)
  __loop__(
    invariant(0 <= j && j <= MLKEM_N / 4)
    invariant(array_bound(r->coeffs, 0, 4 * j, 0, MLKEM_Q)))
  {
    int k;
    uint16_t t[4];
    uint8_t const *base = &a[5 * j];

    t[0] = 0x3FF & ((base[0] >> 0) | ((uint16_t)base[1] << 8));
    t[1] = 0x3FF & ((base[1] >> 2) | ((uint16_t)base[2] << 6));
    t[2] = 0x3FF & ((base[2] >> 4) | ((uint16_t)base[3] << 4));
    t[3] = 0x3FF & ((base[3] >> 6) | ((uint16_t)base[4] << 2));

    for (k = 0; k < 4; k++)
    __loop__(
      invariant(0 <= k && k <= 4)
      invariant(array_bound(r->coeffs, 0, 4 * j + k, 0, MLKEM_Q)))
    {
      r->coeffs[4 * j + k] = scalar_decompress_d10(t[k]);
    }
  }
#else
#error "MLKEM_POLYCOMPRESSEDBYTES_DU needs to be in {320,352}"
#endif
}

MLKEM_NATIVE_INTERNAL_API
void poly_compress_dv(uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_DV], const poly *a)
{
  unsigned i;
  POLY_UBOUND(a, MLKEM_Q);

#if (MLKEM_POLYCOMPRESSEDBYTES_DV == 128)
  for (i = 0; i < MLKEM_N / 8; i++)
  __loop__(invariant(i >= 0 && i <= MLKEM_N / 8))
  {
    unsigned j;
    uint8_t t[8] = {0};
    for (j = 0; j < 8; j++)
    __loop__(
      invariant(i >= 0 && i <= MLKEM_N / 8 && j >= 0 && j <= 8)
      invariant(array_bound(t, 0, j, 0, 16)))
    {
      t[j] = scalar_compress_d4(a->coeffs[8 * i + j]);
    }

    r[i * 4] = t[0] | (t[1] << 4);
    r[i * 4 + 1] = t[2] | (t[3] << 4);
    r[i * 4 + 2] = t[4] | (t[5] << 4);
    r[i * 4 + 3] = t[6] | (t[7] << 4);
  }
#elif (MLKEM_POLYCOMPRESSEDBYTES_DV == 160)
  for (i = 0; i < MLKEM_N / 8; i++)
  __loop__(invariant(i >= 0 && i <= MLKEM_N / 8))
  {
    unsigned j;
    uint8_t t[8] = {0};
    for (j = 0; j < 8; j++)
    __loop__(
      invariant(i >= 0 && i <= MLKEM_N / 8 && j >= 0 && j <= 8)
      invariant(array_bound(t, 0, j, 0, 32)))
    {
      t[j] = scalar_compress_d5(a->coeffs[8 * i + j]);
    }

    /*
     * Explicitly truncate to avoid warning about
     * implicit truncation in CBMC, and use array indexing into
     * r rather than pointer-arithmetic to simplify verification
     */
    r[i * 5] = 0xFF & ((t[0] >> 0) | (t[1] << 5));
    r[i * 5 + 1] = 0xFF & ((t[1] >> 3) | (t[2] << 2) | (t[3] << 7));
    r[i * 5 + 2] = 0xFF & ((t[3] >> 1) | (t[4] << 4));
    r[i * 5 + 3] = 0xFF & ((t[4] >> 4) | (t[5] << 1) | (t[6] << 6));
    r[i * 5 + 4] = 0xFF & ((t[6] >> 2) | (t[7] << 3));
  }
#else
#error "MLKEM_POLYCOMPRESSEDBYTES_DV needs to be in {128, 160}"
#endif
}

MLKEM_NATIVE_INTERNAL_API
void poly_decompress_dv(poly *r, const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_DV])
{
  unsigned i;
#if (MLKEM_POLYCOMPRESSEDBYTES_DV == 128)
  for (i = 0; i < MLKEM_N / 2; i++)
  __loop__(
    invariant(i >= 0 && i <= MLKEM_N / 2)
    invariant(array_bound(r->coeffs, 0, 2 * i, 0, MLKEM_Q)))
  {
    r->coeffs[2 * i + 0] = scalar_decompress_d4((a[i] >> 0) & 0xF);
    r->coeffs[2 * i + 1] = scalar_decompress_d4((a[i] >> 4) & 0xF);
  }
#elif (MLKEM_POLYCOMPRESSEDBYTES_DV == 160)
  for (i = 0; i < MLKEM_N / 8; i++)
  __loop__(
    invariant(i >= 0 && i <= MLKEM_N / 8)
    invariant(array_bound(r->coeffs, 0, 8 * i, 0, MLKEM_Q)))
  {
    unsigned j;
    uint8_t t[8];
    const int offset = i * 5;
    /*
     * Explicitly truncate to avoid warning about
     * implicit truncation in CBMC and unwind loop for ease
     * of proof.
     */

    /*
     * Decompress 5 8-bit bytes (so 40 bits) into
     * 8 5-bit values stored in t[]
     */
    t[0] = 0x1F & (a[offset + 0] >> 0);
    t[1] = 0x1F & ((a[offset + 0] >> 5) | (a[offset + 1] << 3));
    t[2] = 0x1F & (a[offset + 1] >> 2);
    t[3] = 0x1F & ((a[offset + 1] >> 7) | (a[offset + 2] << 1));
    t[4] = 0x1F & ((a[offset + 2] >> 4) | (a[offset + 3] << 4));
    t[5] = 0x1F & (a[offset + 3] >> 1);
    t[6] = 0x1F & ((a[offset + 3] >> 6) | (a[offset + 4] << 2));
    t[7] = 0x1F & (a[offset + 4] >> 3);

    /* and copy to the correct slice in r[] */
    for (j = 0; j < 8; j++)
    __loop__(
      invariant(j >= 0 && j <= 8 && i >= 0 && i <= MLKEM_N / 8)
      invariant(array_bound(r->coeffs, 0, 8 * i + j, 0, MLKEM_Q)))
    {
      r->coeffs[8 * i + j] = scalar_decompress_d5(t[j]);
    }
  }
#else
#error "MLKEM_POLYCOMPRESSEDBYTES_DV needs to be in {128, 160}"
#endif

  POLY_UBOUND(r, MLKEM_Q);
}

#if !defined(MLKEM_USE_NATIVE_POLY_TOBYTES)
MLKEM_NATIVE_INTERNAL_API
void poly_tobytes(uint8_t r[MLKEM_POLYBYTES], const poly *a)
{
  unsigned i;
  POLY_UBOUND(a, MLKEM_Q);


  for (i = 0; i < MLKEM_N / 2; i++)
  __loop__(invariant(i >= 0 && i <= MLKEM_N / 2))
  {
    const uint16_t t0 = a->coeffs[2 * i];
    const uint16_t t1 = a->coeffs[2 * i + 1];
    /*
     * t0 and t1 are both < MLKEM_Q, so contain at most 12 bits each of
     * significant data, so these can be packed into 24 bits or exactly
     * 3 bytes, as follows.
     */

    /* Least significant bits 0 - 7 of t0. */
    r[3 * i + 0] = t0 & 0xFF;

    /*
     * Most significant bits 8 - 11 of t0 become the least significant
     * nibble of the second byte. The least significant 4 bits
     * of t1 become the upper nibble of the second byte.
     */
    r[3 * i + 1] = (t0 >> 8) | ((t1 << 4) & 0xF0);

    /* Bits 4 - 11 of t1 become the third byte. */
    r[3 * i + 2] = t1 >> 4;
  }
}
#else  /* MLKEM_USE_NATIVE_POLY_TOBYTES */
MLKEM_NATIVE_INTERNAL_API
void poly_tobytes(uint8_t r[MLKEM_POLYBYTES], const poly *a)
{
  POLY_UBOUND(a, MLKEM_Q);
  poly_tobytes_native(r, a);
}
#endif /* MLKEM_USE_NATIVE_POLY_TOBYTES */

#if !defined(MLKEM_USE_NATIVE_POLY_FROMBYTES)
MLKEM_NATIVE_INTERNAL_API
void poly_frombytes(poly *r, const uint8_t a[MLKEM_POLYBYTES])
{
  unsigned i;
  for (i = 0; i < MLKEM_N / 2; i++)
  __loop__(
    invariant(i >= 0 && i <= MLKEM_N / 2)
    invariant(array_bound(r->coeffs, 0, 2 * i, 0, UINT12_LIMIT)))
  {
    const uint8_t t0 = a[3 * i + 0];
    const uint8_t t1 = a[3 * i + 1];
    const uint8_t t2 = a[3 * i + 2];
    r->coeffs[2 * i + 0] = t0 | ((t1 << 8) & 0xFFF);
    r->coeffs[2 * i + 1] = (t1 >> 4) | (t2 << 4);
  }

  /* Note that the coefficients are not canonical */
  POLY_UBOUND(r, 4096);
}
#else  /* MLKEM_USE_NATIVE_POLY_FROMBYTES */
MLKEM_NATIVE_INTERNAL_API
void poly_frombytes(poly *r, const uint8_t a[MLKEM_POLYBYTES])
{
  poly_frombytes_native(r, a);
}
#endif /* MLKEM_USE_NATIVE_POLY_FROMBYTES */

MLKEM_NATIVE_INTERNAL_API
void poly_frommsg(poly *r, const uint8_t msg[MLKEM_INDCPA_MSGBYTES])
{
  unsigned i;
#if (MLKEM_INDCPA_MSGBYTES != MLKEM_N / 8)
#error "MLKEM_INDCPA_MSGBYTES must be equal to MLKEM_N/8 bytes!"
#endif

  for (i = 0; i < MLKEM_N / 8; i++)
  __loop__(
    invariant(i >= 0 && i <= MLKEM_N / 8)
    invariant(array_bound(r->coeffs, 0, 8 * i, 0, MLKEM_Q)))
  {
    unsigned j;
    for (j = 0; j < 8; j++)
    __loop__(
      invariant(i >= 0 && i <  MLKEM_N / 8 && j >= 0 && j <= 8)
      invariant(array_bound(r->coeffs, 0, 8 * i + j, 0, MLKEM_Q)))
    {
      /* Prevent the compiler from recognizing this as a bit selection */
      uint8_t mask = value_barrier_u8(1u << j);
      r->coeffs[8 * i + j] = ct_sel_int16(HALF_Q, 0, msg[i] & mask);
    }
  }
  POLY_BOUND_MSG(r, MLKEM_Q, "poly_frommsg output");
}

MLKEM_NATIVE_INTERNAL_API
void poly_tomsg(uint8_t msg[MLKEM_INDCPA_MSGBYTES], const poly *a)
{
  unsigned i;
  POLY_UBOUND(a, MLKEM_Q);

  for (i = 0; i < MLKEM_N / 8; i++)
  __loop__(invariant(i >= 0 && i <= MLKEM_N / 8))
  {
    unsigned j;
    msg[i] = 0;
    for (j = 0; j < 8; j++)
    __loop__(
      invariant(i >= 0 && i <= MLKEM_N / 8 && j >= 0 && j <= 8))
    {
      uint32_t t = scalar_compress_d1(a->coeffs[8 * i + j]);
      msg[i] |= t << j;
    }
  }
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

  POLY_BOUND_MSG(r0, MLKEM_ETA1 + 1, "poly_getnoise_eta1_4x output 0");
  POLY_BOUND_MSG(r1, MLKEM_ETA1 + 1, "poly_getnoise_eta1_4x output 1");
  POLY_BOUND_MSG(r2, MLKEM_ETA1 + 1, "poly_getnoise_eta1_4x output 2");
  POLY_BOUND_MSG(r3, MLKEM_ETA1 + 1, "poly_getnoise_eta1_4x output 3");
}

#if MLKEM_K == 2 || MLKEM_K == 4
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

  POLY_BOUND_MSG(r, MLKEM_ETA1 + 1, "poly_getnoise_eta2 output");
}
#endif /* MLKEM_K == 2 || MLKEM_K == 4 */

#if MLKEM_K == 2
MLKEM_NATIVE_INTERNAL_API
void poly_getnoise_eta1122_4x(poly *r0, poly *r1, poly *r2, poly *r3,
                              const uint8_t seed[MLKEM_SYMBYTES],
                              uint8_t nonce0, uint8_t nonce1, uint8_t nonce2,
                              uint8_t nonce3)
{
  ALIGN uint8_t buf1[KECCAK_WAY / 2][MLKEM_ETA1 * MLKEM_N / 4];
  ALIGN uint8_t buf2[KECCAK_WAY / 2][MLKEM_ETA2 * MLKEM_N / 4];
  ALIGN uint8_t extkey[KECCAK_WAY][MLKEM_SYMBYTES + 1];
  memcpy(extkey[0], seed, MLKEM_SYMBYTES);
  memcpy(extkey[1], seed, MLKEM_SYMBYTES);
  memcpy(extkey[2], seed, MLKEM_SYMBYTES);
  memcpy(extkey[3], seed, MLKEM_SYMBYTES);
  extkey[0][MLKEM_SYMBYTES] = nonce0;
  extkey[1][MLKEM_SYMBYTES] = nonce1;
  extkey[2][MLKEM_SYMBYTES] = nonce2;
  extkey[3][MLKEM_SYMBYTES] = nonce3;

  prf_eta1(buf1[0], extkey[0]);
  prf_eta1(buf1[1], extkey[1]);
  prf_eta2(buf2[0], extkey[2]);
  prf_eta2(buf2[1], extkey[3]);

  poly_cbd_eta1(r0, buf1[0]);
  poly_cbd_eta1(r1, buf1[1]);
  poly_cbd_eta2(r2, buf2[0]);
  poly_cbd_eta2(r3, buf2[1]);

  POLY_BOUND_MSG(r0, MLKEM_ETA1 + 1, "poly_getnoise_eta1122_4x output 0");
  POLY_BOUND_MSG(r1, MLKEM_ETA1 + 1, "poly_getnoise_eta1122_4x output 1");
  POLY_BOUND_MSG(r2, MLKEM_ETA2 + 1, "poly_getnoise_eta1122_4x output 2");
  POLY_BOUND_MSG(r3, MLKEM_ETA2 + 1, "poly_getnoise_eta1122_4x output 3");
}
#endif /* MLKEM_K == 2 */

MLKEM_NATIVE_INTERNAL_API
void poly_basemul_montgomery_cached(poly *r, const poly *a, const poly *b,
                                    const poly_mulcache *b_cache)
{
  unsigned i;
  POLY_BOUND(b_cache, 4096);

  for (i = 0; i < MLKEM_N / 4; i++)
  __loop__(
    assigns(i, object_whole(r))
    invariant(i >= 0 && i <= MLKEM_N / 4)
    invariant(array_abs_bound(r->coeffs, 0, 4 * i, 2 * MLKEM_Q)))
  {
    basemul_cached(&r->coeffs[4 * i], &a->coeffs[4 * i], &b->coeffs[4 * i],
                   b_cache->coeffs[2 * i]);
    basemul_cached(&r->coeffs[4 * i + 2], &a->coeffs[4 * i + 2],
                   &b->coeffs[4 * i + 2], b_cache->coeffs[2 * i + 1]);
  }
}

#if !defined(MLKEM_USE_NATIVE_POLY_TOMONT)
MLKEM_NATIVE_INTERNAL_API
void poly_tomont(poly *r)
{
  unsigned i;
  const int16_t f = (1ULL << 32) % MLKEM_Q; /* 1353 */
  for (i = 0; i < MLKEM_N; i++)
  __loop__(
    invariant(i >= 0 && i <= MLKEM_N)
    invariant(array_abs_bound(r->coeffs ,0, i, MLKEM_Q)))
  {
    r->coeffs[i] = fqmul(r->coeffs[i], f);
  }

  POLY_BOUND(r, MLKEM_Q);
}
#else  /* MLKEM_USE_NATIVE_POLY_TOMONT */
MLKEM_NATIVE_INTERNAL_API
void poly_tomont(poly *r)
{
  poly_tomont_native(r);
  POLY_BOUND(r, MLKEM_Q);
}
#endif /* MLKEM_USE_NATIVE_POLY_TOMONT */

#if !defined(MLKEM_USE_NATIVE_POLY_REDUCE)
MLKEM_NATIVE_INTERNAL_API
void poly_reduce(poly *r)
{
  unsigned i;
  for (i = 0; i < MLKEM_N; i++)
  __loop__(
    invariant(i >= 0 && i <= MLKEM_N)
    invariant(array_bound(r->coeffs, 0, i, 0, MLKEM_Q)))
  {
    /* Barrett reduction, giving signed canonical representative */
    int16_t t = barrett_reduce(r->coeffs[i]);
    /* Conditional addition to get unsigned canonical representative */
    r->coeffs[i] = scalar_signed_to_unsigned_q(t);
  }

  POLY_UBOUND(r, MLKEM_Q);
}
#else  /* MLKEM_USE_NATIVE_POLY_REDUCE */
MLKEM_NATIVE_INTERNAL_API
void poly_reduce(poly *r)
{
  poly_reduce_native(r);
  POLY_UBOUND(r, MLKEM_Q);
}
#endif /* MLKEM_USE_NATIVE_POLY_REDUCE */

MLKEM_NATIVE_INTERNAL_API
void poly_add(poly *r, const poly *b)
{
  unsigned i;
  for (i = 0; i < MLKEM_N; i++)
  __loop__(
    invariant(i >= 0 && i <= MLKEM_N)
    invariant(forall(k0, i, MLKEM_N, r->coeffs[k0] == loop_entry(*r).coeffs[k0]))
    invariant(forall(k1, 0, i, r->coeffs[k1] == loop_entry(*r).coeffs[k1] + b->coeffs[k1])))
  {
    r->coeffs[i] = r->coeffs[i] + b->coeffs[i];
  }
}

MLKEM_NATIVE_INTERNAL_API
void poly_sub(poly *r, const poly *b)
{
  unsigned i;
  for (i = 0; i < MLKEM_N; i++)
  __loop__(
    invariant(i >= 0 && i <= MLKEM_N)
    invariant(forall(k0, i, MLKEM_N, r->coeffs[k0] == loop_entry(*r).coeffs[k0]))
    invariant(forall(k1, 0, i, r->coeffs[k1] == loop_entry(*r).coeffs[k1] - b->coeffs[k1])))
  {
    r->coeffs[i] = r->coeffs[i] - b->coeffs[i];
  }
}

#if !defined(MLKEM_USE_NATIVE_POLY_MULCACHE_COMPUTE)
MLKEM_NATIVE_INTERNAL_API
void poly_mulcache_compute(poly_mulcache *x, const poly *a)
{
  unsigned i;
  for (i = 0; i < MLKEM_N / 4; i++)
  __loop__(invariant(i >= 0 && i <= MLKEM_N / 4))
  {
    x->coeffs[2 * i + 0] = fqmul(a->coeffs[4 * i + 1], zetas[64 + i]);
    x->coeffs[2 * i + 1] = fqmul(a->coeffs[4 * i + 3], -zetas[64 + i]);
  }
  POLY_BOUND(x, MLKEM_Q);
}
#else  /* MLKEM_USE_NATIVE_POLY_MULCACHE_COMPUTE */
MLKEM_NATIVE_INTERNAL_API
void poly_mulcache_compute(poly_mulcache *x, const poly *a)
{
  poly_mulcache_compute_native(x, a);
  /* Omitting POLY_BOUND(x, MLKEM_Q) since native implementations may
   * decide not to use a mulcache. Note that the C backend implementation
   * of poly_basemul_montgomery_cached() does still include the check. */
}
#endif /* MLKEM_USE_NATIVE_POLY_MULCACHE_COMPUTE */
