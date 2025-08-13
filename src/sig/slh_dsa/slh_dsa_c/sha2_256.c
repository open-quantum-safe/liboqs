/*
 * Copyright (c) The slhdsa-c project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* === FIPS 180-4 SHA2-256 / Portable C Implementation */

#include <string.h>
#include "plat_local.h"
#include "sha2_api.h"

#ifdef SLH_EXPERIMENTAL
uint64_t sha2_256_compress_count = 0; /* instrumentation */
#endif

/* processing step, sets "d" and "h" as a function of all 8 inputs */
/* and message schedule "mi", round constant "ki" */
#define STEP_SHA256_R(a, b, c, d, e, f, g, h, mi, ki) \
  {                                                   \
    h += (g ^ (e & (f ^ g))) + mi + ki +              \
         (ror32(e, 6) ^ ror32(e, 11) ^ ror32(e, 25)); \
    d += h;                                           \
    h += (((a | c) & b) | (c & a)) +                  \
         (ror32(a, 2) ^ ror32(a, 13) ^ ror32(a, 22)); \
  }

/* keying step, sets x0 as a function of 4 inputs */
#define STEP_SHA256_K(x0, x1, x9, xe)                       \
  {                                                         \
    x0 += x9 + (ror32(x1, 7) ^ ror32(x1, 18) ^ (x1 >> 3)) + \
          (ror32(xe, 17) ^ ror32(xe, 19) ^ (xe >> 10));     \
  }

void sha2_256_compress(void *v)
{
  /* 4.2.2 SHA-224 and SHA-256 Constants */

  const uint32_t ck[64] = {
      0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1,
      0x923F82A4, 0xAB1C5ED5, 0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3,
      0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174, 0xE49B69C1, 0xEFBE4786,
      0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
      0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147,
      0x06CA6351, 0x14292967, 0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13,
      0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85, 0xA2BFE8A1, 0xA81A664B,
      0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
      0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A,
      0x5B9CCA4F, 0x682E6FF3, 0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208,
      0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2};

  uint32_t a, b, c, d, e, f, g, h;
  uint32_t m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, ma, mb, mc, md, me, mf;

  uint32_t *sp = (uint32_t *)v;
  const uint32_t *mp = sp + 8;
  const uint32_t *kp = ck;

#ifdef SLH_EXPERIMENTAL
  sha2_256_compress_count++; /* instrumentation */
#endif

  a = sp[0] = rev8_be32(sp[0]);
  b = sp[1] = rev8_be32(sp[1]);
  c = sp[2] = rev8_be32(sp[2]);
  d = sp[3] = rev8_be32(sp[3]);
  e = sp[4] = rev8_be32(sp[4]);
  f = sp[5] = rev8_be32(sp[5]);
  g = sp[6] = rev8_be32(sp[6]);
  h = sp[7] = rev8_be32(sp[7]);

  /* load and reverse bytes (if needed) */

  m0 = rev8_be32(mp[0]);
  m1 = rev8_be32(mp[1]);
  m2 = rev8_be32(mp[2]);
  m3 = rev8_be32(mp[3]);
  m4 = rev8_be32(mp[4]);
  m5 = rev8_be32(mp[5]);
  m6 = rev8_be32(mp[6]);
  m7 = rev8_be32(mp[7]);
  m8 = rev8_be32(mp[8]);
  m9 = rev8_be32(mp[9]);
  ma = rev8_be32(mp[10]);
  mb = rev8_be32(mp[11]);
  mc = rev8_be32(mp[12]);
  md = rev8_be32(mp[13]);
  me = rev8_be32(mp[14]);
  mf = rev8_be32(mp[15]);

  while (1)
  {
    STEP_SHA256_R(a, b, c, d, e, f, g, h, m0, kp[0]); /* rounds */
    STEP_SHA256_R(h, a, b, c, d, e, f, g, m1, kp[1]);
    STEP_SHA256_R(g, h, a, b, c, d, e, f, m2, kp[2]);
    STEP_SHA256_R(f, g, h, a, b, c, d, e, m3, kp[3]);
    STEP_SHA256_R(e, f, g, h, a, b, c, d, m4, kp[4]);
    STEP_SHA256_R(d, e, f, g, h, a, b, c, m5, kp[5]);
    STEP_SHA256_R(c, d, e, f, g, h, a, b, m6, kp[6]);
    STEP_SHA256_R(b, c, d, e, f, g, h, a, m7, kp[7]);
    STEP_SHA256_R(a, b, c, d, e, f, g, h, m8, kp[8]);
    STEP_SHA256_R(h, a, b, c, d, e, f, g, m9, kp[9]);
    STEP_SHA256_R(g, h, a, b, c, d, e, f, ma, kp[10]);
    STEP_SHA256_R(f, g, h, a, b, c, d, e, mb, kp[11]);
    STEP_SHA256_R(e, f, g, h, a, b, c, d, mc, kp[12]);
    STEP_SHA256_R(d, e, f, g, h, a, b, c, md, kp[13]);
    STEP_SHA256_R(c, d, e, f, g, h, a, b, me, kp[14]);
    STEP_SHA256_R(b, c, d, e, f, g, h, a, mf, kp[15]);

    if (kp == &ck[64 - 16])
    {
      break;
    }
    kp += 16;

    STEP_SHA256_K(m0, m1, m9, me); /* message schedule */
    STEP_SHA256_K(m1, m2, ma, mf);
    STEP_SHA256_K(m2, m3, mb, m0);
    STEP_SHA256_K(m3, m4, mc, m1);
    STEP_SHA256_K(m4, m5, md, m2);
    STEP_SHA256_K(m5, m6, me, m3);
    STEP_SHA256_K(m6, m7, mf, m4);
    STEP_SHA256_K(m7, m8, m0, m5);
    STEP_SHA256_K(m8, m9, m1, m6);
    STEP_SHA256_K(m9, ma, m2, m7);
    STEP_SHA256_K(ma, mb, m3, m8);
    STEP_SHA256_K(mb, mc, m4, m9);
    STEP_SHA256_K(mc, md, m5, ma);
    STEP_SHA256_K(md, me, m6, mb);
    STEP_SHA256_K(me, mf, m7, mc);
    STEP_SHA256_K(mf, m0, m8, md);
  }

  sp[0] = rev8_be32(sp[0] + a);
  sp[1] = rev8_be32(sp[1] + b);
  sp[2] = rev8_be32(sp[2] + c);
  sp[3] = rev8_be32(sp[3] + d);
  sp[4] = rev8_be32(sp[4] + e);
  sp[5] = rev8_be32(sp[5] + f);
  sp[6] = rev8_be32(sp[6] + g);
  sp[7] = rev8_be32(sp[7] + h);
}

/* initialize */

static void sha2_256_init_h0(sha2_256_t *sha, const uint8_t *h0)
{
  memcpy(sha->s, h0, 32);
  sha->i = 0;
  sha->len = 0;
}

void sha2_256_init(sha2_256_t *sha)
{
  /* SHA-256 initial values H0, Sect 5.3.3. */
  const uint8_t sha2_256_h0[32] = {
      0x6A, 0x09, 0xE6, 0x67, 0xBB, 0x67, 0xAE, 0x85, 0x3C, 0x6E, 0xF3,
      0x72, 0xA5, 0x4F, 0xF5, 0x3A, 0x51, 0x0E, 0x52, 0x7F, 0x9B, 0x05,
      0x68, 0x8C, 0x1F, 0x83, 0xD9, 0xAB, 0x5B, 0xE0, 0xCD, 0x19};

  sha2_256_init_h0(sha, sha2_256_h0);
}

void sha2_224_init(sha2_256_t *sha)
{
  /* SHA-224 initial values H0, Sect 5.3.2. */
  const uint8_t sha2_224_h0[32] = {
      0xC1, 0x05, 0x9E, 0xD8, 0x36, 0x7C, 0xD5, 0x07, 0x30, 0x70, 0xDD,
      0x17, 0xF7, 0x0E, 0x59, 0x39, 0xFF, 0xC0, 0x0B, 0x31, 0x68, 0x58,
      0x15, 0x11, 0x64, 0xF9, 0x8F, 0xA7, 0xBE, 0xFA, 0x4F, 0xA4};

  sha2_256_init_h0(sha, sha2_224_h0);
}

void sha2_256_copy(sha2_256_t *dst, const sha2_256_t *src)
{
  dst->i = src->i;
  dst->len = src->len;
  memcpy(dst->s, src->s, 32 + src->i);
}

/* SHA2-256 process input */

void sha2_256_update(sha2_256_t *sha, const uint8_t *m, size_t m_sz)
{
  size_t l;
  uint8_t *mp = (uint8_t *)&sha->s[8];

  sha->len += m_sz;
  l = 64 - sha->i;

  if (m_sz < l)
  {
    memcpy(mp + sha->i, m, m_sz);
    sha->i += m_sz;
    return;
  }
  if (sha->i > 0)
  {
    memcpy(mp + sha->i, m, l);
    sha2_256_compress(sha->s);
    m_sz -= l;
    m += l;
    sha->i = 0;
  }
  while (m_sz >= 64)
  {
    memcpy(mp, m, 64);
    sha2_256_compress(sha->s);
    m_sz -= 64;
    m += 64;
  }
  memcpy(mp, m, m_sz);
  sha->i = m_sz;
}

/* perform final padding */

void sha2_256_final_pad(sha2_256_t *sha)
{
  uint8_t *mp = (uint8_t *)&sha->s[8];
  uint64_t x;
  size_t i;

  i = sha->i; /* last data block */
  mp[i++] = 0x80;
  if (i > 56)
  {
    memset(mp + i, 0x00, 64 - i);
    sha2_256_compress(sha->s);
    i = 0;
  }
  memset(mp + i, 0x00, 64 - i); /* clear rest */

  x = ((uint64_t)sha->len) << 3; /* process length */
  i = 64;
  while (x > 0)
  {
    mp[--i] = x & 0xFF;
    x >>= 8;
  }
}

/* produce h_sz byte hash */

void sha2_256_final_len(sha2_256_t *sha, uint8_t *h, size_t h_sz)
{
  sha2_256_final_pad(sha);
  sha2_256_compress(sha->s);

  memcpy(h, sha->s, h_sz);
}

/* SHA-224/256 public single-call interfaces */

void sha2_256(uint8_t *h, const void *m, size_t m_sz)
{
  sha2_256_t sha;

  sha2_256_init(&sha);
  sha2_256_update(&sha, m, m_sz);
  sha2_256_final(&sha, h);
}

void sha2_224(uint8_t *h, const void *m, size_t m_sz)
{
  sha2_256_t sha;

  sha2_224_init(&sha);
  sha2_224_update(&sha, m, m_sz);
  sha2_224_final(&sha, h);
}
