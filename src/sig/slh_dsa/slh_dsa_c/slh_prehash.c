/*
 * Copyright (c) The slhdsa-c project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

#include "slh_prehash.h"
#include <string.h>
#include "sha2_api.h"
#include "sha3_api.h"
#include "slh_dsa.h"
#include "slh_var.h"

#include <stdio.h>

#define SLH_PREHASH_MAX_MP 512

/* shared formatting routine for alg 23 and alg 25 */

static size_t hash_slh_dsa_pad(uint8_t *mp, const uint8_t *m, size_t m_sz,
                               const uint8_t *ctx, size_t ctx_sz,
                               const char *ph)
{
  size_t mp_sz = 0;

  const uint8_t sha2_256_oid[11] = {0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                    0x65, 0x03, 0x04, 0x02, 0x01};
  const uint8_t sha2_384_oid[11] = {0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                    0x65, 0x03, 0x04, 0x02, 0x02};
  const uint8_t sha2_512_oid[11] = {0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                    0x65, 0x03, 0x04, 0x02, 0x03};
  const uint8_t sha2_224_oid[11] = {0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                    0x65, 0x03, 0x04, 0x02, 0x04};
  const uint8_t sha2_512_224_oid[11] = {0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                        0x65, 0x03, 0x04, 0x02, 0x05};
  const uint8_t sha2_512_256_oid[11] = {0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                        0x65, 0x03, 0x04, 0x02, 0x06};
  const uint8_t sha3_224_oid[11] = {0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                    0x65, 0x03, 0x04, 0x02, 0x07};
  const uint8_t sha3_256_oid[11] = {0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                    0x65, 0x03, 0x04, 0x02, 0x08};
  const uint8_t sha3_384_oid[11] = {0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                    0x65, 0x03, 0x04, 0x02, 0x09};
  const uint8_t sha3_512_oid[11] = {0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                    0x65, 0x03, 0x04, 0x02, 0x0A};
  const uint8_t shake_128_oid[11] = {0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                     0x65, 0x03, 0x04, 0x02, 0x0B};
  const uint8_t shake_256_oid[11] = {0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                     0x65, 0x03, 0x04, 0x02, 0x0C};

  if (ctx_sz > 255)
  {
    return 0;
  }

  mp[0] = 1;
  mp[1] = ctx_sz & 0xFF;
  memcpy(mp + 2, ctx, ctx_sz);
  mp_sz = 2 + ctx_sz;

  if (strcmp(ph, "SHA2-256") == 0)
  {
    memcpy(mp + mp_sz, sha2_256_oid, 11);
    mp_sz += 11;
    sha2_256(mp + mp_sz, m, m_sz);
    mp_sz += 32;
  }
  else if (strcmp(ph, "SHA2-384") == 0)
  {
    memcpy(mp + mp_sz, sha2_384_oid, 11);
    mp_sz += 11;
    sha2_384(mp + mp_sz, m, m_sz);
    mp_sz += 48;
  }
  else if (strcmp(ph, "SHA2-512") == 0)
  {
    memcpy(mp + mp_sz, sha2_512_oid, 11);
    mp_sz += 11;
    sha2_512(mp + mp_sz, m, m_sz);
    mp_sz += 64;
  }
  else if (strcmp(ph, "SHA2-224") == 0)
  {
    memcpy(mp + mp_sz, sha2_224_oid, 11);
    mp_sz += 11;
    sha2_224(mp + mp_sz, m, m_sz);
    mp_sz += 28;
  }
  else if (strcmp(ph, "SHA2-512/224") == 0)
  {
    memcpy(mp + mp_sz, sha2_512_224_oid, 11);
    mp_sz += 11;
    sha2_512_224(mp + mp_sz, m, m_sz);
    mp_sz += 28;
  }
  else if (strcmp(ph, "SHA2-512/256") == 0)
  {
    memcpy(mp + mp_sz, sha2_512_256_oid, 11);
    mp_sz += 11;
    sha2_512_256(mp + mp_sz, m, m_sz);
    mp_sz += 32;
  }
  else if (strcmp(ph, "SHA3-224") == 0)
  {
    memcpy(mp + mp_sz, sha3_224_oid, 11);
    mp_sz += 11;
    sha3(mp + mp_sz, 28, m, m_sz);
    mp_sz += 28;
  }
  else if (strcmp(ph, "SHA3-256") == 0)
  {
    memcpy(mp + mp_sz, sha3_256_oid, 11);
    mp_sz += 11;
    sha3(mp + mp_sz, 32, m, m_sz);
    mp_sz += 32;
  }
  else if (strcmp(ph, "SHA3-384") == 0)
  {
    memcpy(mp + mp_sz, sha3_384_oid, 11);
    mp_sz += 11;
    sha3(mp + mp_sz, 48, m, m_sz);
    mp_sz += 48;
  }
  else if (strcmp(ph, "SHA3-512") == 0)
  {
    memcpy(mp + mp_sz, sha3_512_oid, 11);
    mp_sz += 11;
    sha3(mp + mp_sz, 64, m, m_sz);
    mp_sz += 64;
  }
  else if (strcmp(ph, "SHAKE-128") == 0)
  {
    memcpy(mp + mp_sz, shake_128_oid, 11);
    mp_sz += 11;
    shake128(mp + mp_sz, 32, m, m_sz);
    mp_sz += 32;
  }
  else if (strcmp(ph, "SHAKE-256") == 0)
  {
    memcpy(mp + mp_sz, shake_256_oid, 11);
    mp_sz += 11;
    shake256(mp + mp_sz, 64, m, m_sz);
    mp_sz += 64;
  }
  else
  {
    return 0;
  }

  return mp_sz;
}

/* === Generates a pre-hash SLH-DSA signature. */
/* Algorithm 23: hash_slh_sign(M, ctx, PH, SK) */

size_t hash_slh_sign(uint8_t *sig, const uint8_t *m, size_t m_sz,
                     const uint8_t *ctx, size_t ctx_sz, const char *ph,
                     const uint8_t *sk, const uint8_t *addrnd,
                     const slh_param_t *prm)
{
  uint8_t mp[SLH_PREHASH_MAX_MP];
  size_t mp_sz;

  mp_sz = hash_slh_dsa_pad(mp, m, m_sz, ctx, ctx_sz, ph);
  if (mp_sz == 0)
  {
    return 0;
  }

  return slh_sign_internal(sig, mp, mp_sz, sk, addrnd, prm);
}

/* === Verifies a pre-hash SLH-DSA signature. */
/* Algorithm 25: hash_slh_verify(M, SIG, ctx, PH, PK) */

int hash_slh_verify(const uint8_t *m, size_t m_sz, const uint8_t *sig,
                    size_t sig_sz, const uint8_t *ctx, size_t ctx_sz,
                    const char *ph, const uint8_t *pk, const slh_param_t *prm)
{
  uint8_t mp[SLH_PREHASH_MAX_MP];
  size_t mp_sz;

  mp_sz = hash_slh_dsa_pad(mp, m, m_sz, ctx, ctx_sz, ph);
  if (mp_sz == 0)
  {
    return 0; /* false */
  }

  return slh_verify_internal(mp, mp_sz, sig, sig_sz, pk, prm);
}
