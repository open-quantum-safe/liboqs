/*
 * Copyright (c) The slhdsa-c project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* === FIPS 180-4 SHA2-512 / Portable C Implementation */

#include <string.h>
#include "plat_local.h"
#include "sha2_api.h"

#ifdef SLH_EXPERIMENTAL
uint64_t sha2_512_compress_count = 0; /* instrumentation */
#endif

/* processing step, sets "d" and "h" as a function of all 8 inputs */
/* and message schedule "mi", round constant "ki" */
#define STEP_SHA512_R(a, b, c, d, e, f, g, h, mi, ki)  \
  {                                                    \
    h += (g ^ (e & (f ^ g))) + mi + ki +               \
         (ror64(e, 14) ^ ror64(e, 18) ^ ror64(e, 41)); \
    d += h;                                            \
    h += (((a | c) & b) | (c & a)) +                   \
         (ror64(a, 28) ^ ror64(a, 34) ^ ror64(a, 39)); \
  }

/* keying step, sets x0 as a function of 4 inputs */
#define STEP_SHA512_K(x0, x1, x9, xe)                      \
  {                                                        \
    x0 += x9 + (ror64(x1, 1) ^ ror64(x1, 8) ^ (x1 >> 7)) + \
          (ror64(xe, 19) ^ ror64(xe, 61) ^ (xe >> 6));     \
  }

/* compression function (this one does *not* modify m[16]) */

void sha2_512_compress(void *v)
{
  /* 4.2.3 SHA-384, SHA-512, SHA-512/224 and SHA-512/256 Constants */

  const uint64_t ck[80] = {
      UINT64_C(0x428A2F98D728AE22), UINT64_C(0x7137449123EF65CD),
      UINT64_C(0xB5C0FBCFEC4D3B2F), UINT64_C(0xE9B5DBA58189DBBC),
      UINT64_C(0x3956C25BF348B538), UINT64_C(0x59F111F1B605D019),
      UINT64_C(0x923F82A4AF194F9B), UINT64_C(0xAB1C5ED5DA6D8118),
      UINT64_C(0xD807AA98A3030242), UINT64_C(0x12835B0145706FBE),
      UINT64_C(0x243185BE4EE4B28C), UINT64_C(0x550C7DC3D5FFB4E2),
      UINT64_C(0x72BE5D74F27B896F), UINT64_C(0x80DEB1FE3B1696B1),
      UINT64_C(0x9BDC06A725C71235), UINT64_C(0xC19BF174CF692694),
      UINT64_C(0xE49B69C19EF14AD2), UINT64_C(0xEFBE4786384F25E3),
      UINT64_C(0x0FC19DC68B8CD5B5), UINT64_C(0x240CA1CC77AC9C65),
      UINT64_C(0x2DE92C6F592B0275), UINT64_C(0x4A7484AA6EA6E483),
      UINT64_C(0x5CB0A9DCBD41FBD4), UINT64_C(0x76F988DA831153B5),
      UINT64_C(0x983E5152EE66DFAB), UINT64_C(0xA831C66D2DB43210),
      UINT64_C(0xB00327C898FB213F), UINT64_C(0xBF597FC7BEEF0EE4),
      UINT64_C(0xC6E00BF33DA88FC2), UINT64_C(0xD5A79147930AA725),
      UINT64_C(0x06CA6351E003826F), UINT64_C(0x142929670A0E6E70),
      UINT64_C(0x27B70A8546D22FFC), UINT64_C(0x2E1B21385C26C926),
      UINT64_C(0x4D2C6DFC5AC42AED), UINT64_C(0x53380D139D95B3DF),
      UINT64_C(0x650A73548BAF63DE), UINT64_C(0x766A0ABB3C77B2A8),
      UINT64_C(0x81C2C92E47EDAEE6), UINT64_C(0x92722C851482353B),
      UINT64_C(0xA2BFE8A14CF10364), UINT64_C(0xA81A664BBC423001),
      UINT64_C(0xC24B8B70D0F89791), UINT64_C(0xC76C51A30654BE30),
      UINT64_C(0xD192E819D6EF5218), UINT64_C(0xD69906245565A910),
      UINT64_C(0xF40E35855771202A), UINT64_C(0x106AA07032BBD1B8),
      UINT64_C(0x19A4C116B8D2D0C8), UINT64_C(0x1E376C085141AB53),
      UINT64_C(0x2748774CDF8EEB99), UINT64_C(0x34B0BCB5E19B48A8),
      UINT64_C(0x391C0CB3C5C95A63), UINT64_C(0x4ED8AA4AE3418ACB),
      UINT64_C(0x5B9CCA4F7763E373), UINT64_C(0x682E6FF3D6B2B8A3),
      UINT64_C(0x748F82EE5DEFB2FC), UINT64_C(0x78A5636F43172F60),
      UINT64_C(0x84C87814A1F0AB72), UINT64_C(0x8CC702081A6439EC),
      UINT64_C(0x90BEFFFA23631E28), UINT64_C(0xA4506CEBDE82BDE9),
      UINT64_C(0xBEF9A3F7B2C67915), UINT64_C(0xC67178F2E372532B),
      UINT64_C(0xCA273ECEEA26619C), UINT64_C(0xD186B8C721C0C207),
      UINT64_C(0xEADA7DD6CDE0EB1E), UINT64_C(0xF57D4F7FEE6ED178),
      UINT64_C(0x06F067AA72176FBA), UINT64_C(0x0A637DC5A2C898A6),
      UINT64_C(0x113F9804BEF90DAE), UINT64_C(0x1B710B35131C471B),
      UINT64_C(0x28DB77F523047D84), UINT64_C(0x32CAAB7B40C72493),
      UINT64_C(0x3C9EBE0A15C9BEBC), UINT64_C(0x431D67C49C100D4C),
      UINT64_C(0x4CC5D4BECB3E42B6), UINT64_C(0x597F299CFC657E2A),
      UINT64_C(0x5FCB6FAB3AD6FAEC), UINT64_C(0x6C44198C4A475817)};

  uint64_t a, b, c, d, e, f, g, h;
  uint64_t m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, ma, mb, mc, md, me, mf;

  uint64_t *sp = (uint64_t *)v;
  const uint64_t *mp = sp + 8;
  const uint64_t *kp = ck;

#ifdef SLH_EXPERIMENTAL
  sha2_512_compress_count++; /* instrumentation */
#endif

  /* get state */
  a = sp[0] = rev8_be64(sp[0]);
  b = sp[1] = rev8_be64(sp[1]);
  c = sp[2] = rev8_be64(sp[2]);
  d = sp[3] = rev8_be64(sp[3]);
  e = sp[4] = rev8_be64(sp[4]);
  f = sp[5] = rev8_be64(sp[5]);
  g = sp[6] = rev8_be64(sp[6]);
  h = sp[7] = rev8_be64(sp[7]);

  /* load and reverse bytes (if needed) */

  m0 = rev8_be64(mp[0]);
  m1 = rev8_be64(mp[1]);
  m2 = rev8_be64(mp[2]);
  m3 = rev8_be64(mp[3]);
  m4 = rev8_be64(mp[4]);
  m5 = rev8_be64(mp[5]);
  m6 = rev8_be64(mp[6]);
  m7 = rev8_be64(mp[7]);
  m8 = rev8_be64(mp[8]);
  m9 = rev8_be64(mp[9]);
  ma = rev8_be64(mp[10]);
  mb = rev8_be64(mp[11]);
  mc = rev8_be64(mp[12]);
  md = rev8_be64(mp[13]);
  me = rev8_be64(mp[14]);
  mf = rev8_be64(mp[15]);

  while (1)
  {
    /* main rounds */
    STEP_SHA512_R(a, b, c, d, e, f, g, h, m0, kp[0]);
    STEP_SHA512_R(h, a, b, c, d, e, f, g, m1, kp[1]);
    STEP_SHA512_R(g, h, a, b, c, d, e, f, m2, kp[2]);
    STEP_SHA512_R(f, g, h, a, b, c, d, e, m3, kp[3]);
    STEP_SHA512_R(e, f, g, h, a, b, c, d, m4, kp[4]);
    STEP_SHA512_R(d, e, f, g, h, a, b, c, m5, kp[5]);
    STEP_SHA512_R(c, d, e, f, g, h, a, b, m6, kp[6]);
    STEP_SHA512_R(b, c, d, e, f, g, h, a, m7, kp[7]);
    STEP_SHA512_R(a, b, c, d, e, f, g, h, m8, kp[8]);
    STEP_SHA512_R(h, a, b, c, d, e, f, g, m9, kp[9]);
    STEP_SHA512_R(g, h, a, b, c, d, e, f, ma, kp[10]);
    STEP_SHA512_R(f, g, h, a, b, c, d, e, mb, kp[11]);
    STEP_SHA512_R(e, f, g, h, a, b, c, d, mc, kp[12]);
    STEP_SHA512_R(d, e, f, g, h, a, b, c, md, kp[13]);
    STEP_SHA512_R(c, d, e, f, g, h, a, b, me, kp[14]);
    STEP_SHA512_R(b, c, d, e, f, g, h, a, mf, kp[15]);

    if (kp == &ck[80 - 16])
    {
      break;
    }
    kp += 16;

    STEP_SHA512_K(m0, m1, m9, me); /* key schedule */
    STEP_SHA512_K(m1, m2, ma, mf);
    STEP_SHA512_K(m2, m3, mb, m0);
    STEP_SHA512_K(m3, m4, mc, m1);
    STEP_SHA512_K(m4, m5, md, m2);
    STEP_SHA512_K(m5, m6, me, m3);
    STEP_SHA512_K(m6, m7, mf, m4);
    STEP_SHA512_K(m7, m8, m0, m5);
    STEP_SHA512_K(m8, m9, m1, m6);
    STEP_SHA512_K(m9, ma, m2, m7);
    STEP_SHA512_K(ma, mb, m3, m8);
    STEP_SHA512_K(mb, mc, m4, m9);
    STEP_SHA512_K(mc, md, m5, ma);
    STEP_SHA512_K(md, me, m6, mb);
    STEP_SHA512_K(me, mf, m7, mc);
    STEP_SHA512_K(mf, m0, m8, md);
  }

  sp[0] = rev8_be64(sp[0] + a);
  sp[1] = rev8_be64(sp[1] + b);
  sp[2] = rev8_be64(sp[2] + c);
  sp[3] = rev8_be64(sp[3] + d);
  sp[4] = rev8_be64(sp[4] + e);
  sp[5] = rev8_be64(sp[5] + f);
  sp[6] = rev8_be64(sp[6] + g);
  sp[7] = rev8_be64(sp[7] + h);
}

/* initialize */

static void sha2_512_init_h0(sha2_512_t *sha, const uint8_t *h0)
{
  memcpy(sha->s, h0, 64);
  sha->i = 0;
  sha->len = 0;
}

void sha2_512_init(sha2_512_t *sha)
{
  /* SHA-512 initial values H0, Sect 5.3.5. */
  const uint8_t sha2_512_h0[64] = {
      0x6A, 0x09, 0xE6, 0x67, 0xF3, 0xBC, 0xC9, 0x08, 0xBB, 0x67, 0xAE,
      0x85, 0x84, 0xCA, 0xA7, 0x3B, 0x3C, 0x6E, 0xF3, 0x72, 0xFE, 0x94,
      0xF8, 0x2B, 0xA5, 0x4F, 0xF5, 0x3A, 0x5F, 0x1D, 0x36, 0xF1, 0x51,
      0x0E, 0x52, 0x7F, 0xAD, 0xE6, 0x82, 0xD1, 0x9B, 0x05, 0x68, 0x8C,
      0x2B, 0x3E, 0x6C, 0x1F, 0x1F, 0x83, 0xD9, 0xAB, 0xFB, 0x41, 0xBD,
      0x6B, 0x5B, 0xE0, 0xCD, 0x19, 0x13, 0x7E, 0x21, 0x79};

  sha2_512_init_h0(sha, sha2_512_h0);
}

void sha2_384_init(sha2_512_t *sha)
{
  /* SHA-384 initial values H0, Sect 5.3.4. */
  const uint8_t sha2_384_h0[64] = {
      0xCB, 0xBB, 0x9D, 0x5D, 0xC1, 0x05, 0x9E, 0xD8, 0x62, 0x9A, 0x29,
      0x2A, 0x36, 0x7C, 0xD5, 0x07, 0x91, 0x59, 0x01, 0x5A, 0x30, 0x70,
      0xDD, 0x17, 0x15, 0x2F, 0xEC, 0xD8, 0xF7, 0x0E, 0x59, 0x39, 0x67,
      0x33, 0x26, 0x67, 0xFF, 0xC0, 0x0B, 0x31, 0x8E, 0xB4, 0x4A, 0x87,
      0x68, 0x58, 0x15, 0x11, 0xDB, 0x0C, 0x2E, 0x0D, 0x64, 0xF9, 0x8F,
      0xA7, 0x47, 0xB5, 0x48, 0x1D, 0xBE, 0xFA, 0x4F, 0xA4};

  sha2_512_init_h0(sha, sha2_384_h0);
}

void sha2_512_224_init(sha2_512_t *sha)
{
  /* SHA-512/224 initial values H0, Sect 5.3.6.1. */
  const uint8_t sha2_512_224_h0[64] = {
      0x8C, 0x3D, 0x37, 0xC8, 0x19, 0x54, 0x4D, 0xA2, 0x73, 0xE1, 0x99,
      0x66, 0x89, 0xDC, 0xD4, 0xD6, 0x1D, 0xFA, 0xB7, 0xAE, 0x32, 0xFF,
      0x9C, 0x82, 0x67, 0x9D, 0xD5, 0x14, 0x58, 0x2F, 0x9F, 0xCF, 0x0F,
      0x6D, 0x2B, 0x69, 0x7B, 0xD4, 0x4D, 0xA8, 0x77, 0xE3, 0x6F, 0x73,
      0x04, 0xC4, 0x89, 0x42, 0x3F, 0x9D, 0x85, 0xA8, 0x6A, 0x1D, 0x36,
      0xC8, 0x11, 0x12, 0xE6, 0xAD, 0x91, 0xD6, 0x92, 0xA1};

  sha2_512_init_h0(sha, sha2_512_224_h0);
}

void sha2_512_256_init(sha2_512_t *sha)
{
  /* SHA-512/256 initial values H0, Sect 5.3.6.2. */
  const uint8_t sha2_512_256_h0[64] = {
      0x22, 0x31, 0x21, 0x94, 0xFC, 0x2B, 0xF7, 0x2C, 0x9F, 0x55, 0x5F,
      0xA3, 0xC8, 0x4C, 0x64, 0xC2, 0x23, 0x93, 0xB8, 0x6B, 0x6F, 0x53,
      0xB1, 0x51, 0x96, 0x38, 0x77, 0x19, 0x59, 0x40, 0xEA, 0xBD, 0x96,
      0x28, 0x3E, 0xE2, 0xA8, 0x8E, 0xFF, 0xE3, 0xBE, 0x5E, 0x1E, 0x25,
      0x53, 0x86, 0x39, 0x92, 0x2B, 0x01, 0x99, 0xFC, 0x2C, 0x85, 0xB8,
      0xAA, 0x0E, 0xB7, 0x2D, 0xDC, 0x81, 0xC5, 0x2C, 0xA2};

  sha2_512_init_h0(sha, sha2_512_256_h0);
}

void sha2_512_copy(sha2_512_t *dst, const sha2_512_t *src)
{
  dst->i = src->i;
  dst->len = src->len;
  memcpy(dst->s, src->s, 64 + src->i);
}

/* take message input */

void sha2_512_update(sha2_512_t *sha, const uint8_t *m, size_t m_sz)
{
  size_t l;
  uint8_t *mp = (uint8_t *)&sha->s[8];

  sha->len += m_sz;
  l = 128 - sha->i;

  if (m_sz < l)
  {
    memcpy(mp + sha->i, m, m_sz);
    sha->i += m_sz;
    return;
  }
  if (sha->i > 0)
  {
    memcpy(mp + sha->i, m, l);
    sha2_512_compress(sha->s);
    m_sz -= l;
    m += l;
    sha->i = 0;
  }
  while (m_sz >= 128)
  {
    memcpy(mp, m, 128);
    sha2_512_compress(sha->s);
    m_sz -= 128;
    m += 128;
  }
  memcpy(mp, m, m_sz);
  sha->i = m_sz;
}

/* perform final padding */

void sha2_512_final_pad(sha2_512_t *sha)
{
  uint8_t *mp = (uint8_t *)&sha->s[8];
  uint64_t x;
  size_t i;

  i = sha->i; /* last data block */
  mp[i++] = 0x80;
  if (i > 112)
  {
    memset(mp + i, 0x00, 128 - i);
    sha2_512_compress(sha->s);
    i = 0;
  }
  memset(mp + i, 0x00, 128 - i); /* clear rest */

  x = ((uint64_t)sha->len) << 3; /* process length */
  i = 128;
  while (x > 0)
  {
    mp[--i] = x & 0xFF;
    x >>= 8;
  }
}

/* produce h_sz byte hash */

void sha2_512_final_len(sha2_512_t *sha, uint8_t *h, size_t h_sz)
{
  sha2_512_final_pad(sha);
  sha2_512_compress(sha->s);
  memcpy(h, sha->s, h_sz);
}

/* public single-call interfaces */

void sha2_512(uint8_t *h, const void *m, size_t m_sz)
{
  sha2_512_t sha;

  sha2_512_init(&sha);
  sha2_512_update(&sha, m, m_sz);
  sha2_512_final(&sha, h);
}

void sha2_384(uint8_t *h, const void *m, size_t m_sz)
{
  sha2_384_t sha;

  sha2_384_init(&sha);
  sha2_512_update(&sha, m, m_sz);
  sha2_512_final_len(&sha, h, 48);
}

void sha2_512_224(uint8_t *h, const void *m, size_t m_sz)
{
  sha2_512_t sha;

  sha2_512_224_init(&sha);
  sha2_512_update(&sha, m, m_sz);
  sha2_512_final_len(&sha, h, 28);
}

void sha2_512_256(uint8_t *h, const void *m, size_t m_sz)
{
  sha2_512_t sha;

  sha2_512_256_init(&sha);
  sha2_512_update(&sha, m, m_sz);
  sha2_512_final_len(&sha, h, 32);
}
