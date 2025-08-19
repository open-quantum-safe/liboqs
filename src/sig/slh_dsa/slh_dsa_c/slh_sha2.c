/*
 * Copyright (c) The slhdsa-c project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* === Portable C code: Functions for instantiation of SLH-DSA with SHA2 */

#include <string.h>
#include <oqs/oqs.h>
#include "plat_local.h"
#include "sha2_api.h"
#include "slh_adrs.h"
#include "slh_var.h"

/* Cat 1: Hmsg(R, PK.seed, PK.root, M) = */
/* MGF1-SHA-256(R || PK.seed || SHA-256(R ||PK.seed || PK.root || */
/* M), m) */

static void sha2_256_h_msg(slh_var_t *var, uint8_t *h, const uint8_t *r,
                           const uint8_t *m, size_t m_sz, const uint8_t *ctx,
                           size_t ctx_sz)
{
  sha2_256_t sha2;
  size_t i;
  size_t n = var->prm->n;
  uint8_t buf[16 + 16 + 32 + 4];
  size_t mgf_sz;
  uint8_t *ctr;

  /* SHA-256(R || PK.seed || PK.root || M) */
  sha2_256_init(&sha2);
  sha2_256_update(&sha2, r, n);
  sha2_256_update(&sha2, var->pk_seed, n);
  sha2_256_update(&sha2, var->pk_root, n);

  /* add "pure" domain separator and context, if supplied */
  if (ctx_sz != SLH_CTX_SZ_NO_CONTEXT)
  {
    buf[0] = 0;
    buf[1] = ctx_sz & 0xFF;
    sha2_256_update(&sha2, buf, 2);
    sha2_256_update(&sha2, ctx, ctx_sz);
  }
  /* message */
  sha2_256_update(&sha2, m, m_sz);
  sha2_256_final(&sha2, buf + 2 * n);

  /* MGF1-SHA-256(R || PK.seed || .. */
  memcpy(buf, r, n);
  memcpy(buf + n, var->pk_seed, n);

  mgf_sz = 2 * n + 32 + 4;
  ctr = buf + mgf_sz - 4;

  /* MGF1 counter mode */
  for (i = 0; i < var->prm->m; i += 32)
  {
    uint32_t c = i / 32;
    ctr[0] = c >> 24;
    ctr[1] = (c >> 16) & 0xFF;
    ctr[2] = (c >> 8) & 0xFF;
    ctr[3] = c & 0xFF;

    sha2_256_init(&sha2);
    sha2_256_update(&sha2, buf, mgf_sz);
    if ((var->prm->m - i) >= 32)
    {
      sha2_256_final(&sha2, h + i);
    }
    else
    {
      sha2_256_final(&sha2, buf);
      memcpy(h + i, buf, var->prm->m - i);
    }
  }
}

/* Cat 3, 5: Hmsg(R, PK.seed, PK.root, M) = */
/* MGF1-SHA-512(R || PK.seed || SHA-512(R || PK.seed || PK.root || */
/* M), m) */

static void sha2_512_h_msg(slh_var_t *var, uint8_t *h, const uint8_t *r,
                           const uint8_t *m, size_t m_sz, const uint8_t *ctx,
                           size_t ctx_sz)
{
  size_t i;
  sha2_512_t sha2;
  uint8_t buf[32 + 32 + 64 + 4];
  size_t mgf_sz;
  uint8_t *ctr;
  size_t n = var->prm->n;

  /* SHA-512(R || PK.seed || PK.root || M) */
  sha2_512_init(&sha2);
  sha2_512_update(&sha2, r, n);
  sha2_512_update(&sha2, var->pk_seed, n);
  sha2_512_update(&sha2, var->pk_root, n);

  /* add "pure" domain separator and context, if supplied */
  if (ctx_sz != SLH_CTX_SZ_NO_CONTEXT)
  {
    buf[0] = 0;
    buf[1] = ctx_sz & 0xFF;
    sha2_512_update(&sha2, buf, 2);
    sha2_512_update(&sha2, ctx, ctx_sz);
  }
  sha2_512_update(&sha2, m, m_sz);
  sha2_512_final(&sha2, buf + 2 * n);

  /* MGF1-SHA-512(R || PK.seed || .. */
  memcpy(buf, r, n);
  memcpy(buf + n, var->pk_seed, n);

  mgf_sz = 2 * n + 64 + 4;
  ctr = buf + mgf_sz - 4;

  /* MGF1 counter mode */
  for (i = 0; i < var->prm->m; i += 64)
  {
    uint32_t c = i / 64;
    ctr[0] = c >> 24;
    ctr[1] = (c >> 16) & 0xFF;
    ctr[2] = (c >> 8) & 0xFF;
    ctr[3] = c & 0xFF;

    sha2_512_init(&sha2);
    sha2_512_update(&sha2, buf, mgf_sz);
    if ((var->prm->m - i) >= 64)
    {
      sha2_512_final(&sha2, h + i);
    }
    else
    {
      sha2_512_final(&sha2, buf);
      memcpy(h + i, buf, var->prm->m - i);
    }
  }
}

/* insert ARDSc. */

static void sha2_256_adrsc(sha2_256_t *sha2, const slh_var_t *var)
{
  uint8_t buf[22];
  adrsc_22(var, buf);
  sha2_256_update(sha2, buf, 22);
}

static void sha2_512_adrsc(sha2_512_t *sha2, const slh_var_t *var)
{
  uint8_t buf[22];
  adrsc_22(var, buf);
  sha2_512_update(sha2, buf, 22);
}

/* Cat 1, 3, 5: PRF(PK.seed, SK.seed, ADRS) = */
/* Trunc_n(SHA-256(PK.seed || toByte(0, 64 − n) || ADRSc || */
/* SK.seed)) */

static void sha2_256_prf(slh_var_t *var, uint8_t *h)
{
  sha2_256_t sha2;
  size_t n = var->prm->n;

  sha2_256_copy(&sha2, &var->sha2_256_pk_seed);
  sha2_256_adrsc(&sha2, var);
  sha2_256_update(&sha2, var->sk_seed, n);
  sha2_256_final_pad(&sha2);
  sha2_256_compress(sha2.s);
  memcpy(h, sha2.s, n);
}

/* Cat 1: PRFmsg(SK.prf, opt_rand, M) = */
/* Trunc_n(HMAC-SHA-256(SK.prf, opt_rand || M)) */

static void sha2_256_prf_msg(slh_var_t *var, uint8_t *h,
                             const uint8_t *opt_rand, const uint8_t *m,
                             size_t m_sz, const uint8_t *ctx, size_t ctx_sz)
{
  unsigned i;
  sha2_256_t sha2;
  uint8_t pad[64], buf[32];
  size_t n = var->prm->n;

  /* ipad */
  memcpy(pad, var->sk_prf, n);
  for (i = 0; i < n; i++)
  {
    pad[i] ^= 0x36;
  }
  memset(pad + n, 0x36, 64 - n);

  sha2_256_init(&sha2);
  sha2_256_update(&sha2, pad, 64);
  sha2_256_update(&sha2, opt_rand, n);

  /* add "pure" domain separator and context, if supplied */
  if (ctx_sz != SLH_CTX_SZ_NO_CONTEXT)
  {
    buf[0] = 0;
    buf[1] = ctx_sz & 0xFF;
    sha2_256_update(&sha2, buf, 2);
    sha2_256_update(&sha2, ctx, ctx_sz);
  }
  sha2_256_update(&sha2, m, m_sz);
  sha2_256_final(&sha2, buf);

  /* opad */
  for (i = 0; i < 64; i++)
  {
    pad[i] ^= 0x36 ^ 0x5C;
  }

  sha2_256_init(&sha2);
  sha2_256_update(&sha2, pad, 64);
  sha2_256_update(&sha2, buf, 32);
  sha2_256_final_len(&sha2, h, n);
}

/* Cat 3, 5: PRFmsg(SK.prf, opt_rand, M) = */
/* Trunc_n(HMAC-SHA-512(SK.prf, opt_rand || M)) */

static void sha2_512_prf_msg(slh_var_t *var, uint8_t *h,
                             const uint8_t *opt_rand, const uint8_t *m,
                             size_t m_sz, const uint8_t *ctx, size_t ctx_sz)
{
  unsigned i;
  sha2_512_t sha2;
  uint8_t pad[128], buf[64];
  size_t n = var->prm->n;

  /* ipad */
  memcpy(pad, var->sk_prf, n);
  for (i = 0; i < n; i++)
  {
    pad[i] ^= 0x36;
  }
  memset(pad + n, 0x36, 128 - n);

  sha2_512_init(&sha2);
  sha2_512_update(&sha2, pad, 128);
  sha2_512_update(&sha2, opt_rand, n);

  /* add "pure" domain separator and context, if supplied */
  if (ctx_sz != SLH_CTX_SZ_NO_CONTEXT)
  {
    buf[0] = 0;
    buf[1] = ctx_sz & 0xFF;
    sha2_512_update(&sha2, buf, 2);
    sha2_512_update(&sha2, ctx, ctx_sz);
  }
  sha2_512_update(&sha2, m, m_sz);
  sha2_512_final(&sha2, buf);

  /* opad */
  for (i = 0; i < 128; i++)
  {
    pad[i] ^= 0x36 ^ 0x5C;
  }

  sha2_512_init(&sha2);
  sha2_512_update(&sha2, pad, 128);
  sha2_512_update(&sha2, buf, 64);
  sha2_512_final_len(&sha2, h, n);
}

/* Cat 1: T_l(PK.seed, ADRS, M1 ) = */
/* Trunc_n(SHA-256(PK.seed || toByte(0, 64 − n) || ADRSc || Ml )) */

static void sha2_256_tl(slh_var_t *var, uint8_t *h, const uint8_t *m,
                        size_t m_sz)
{
  sha2_256_t sha2;
  size_t n = var->prm->n;

  sha2_256_copy(&sha2, &var->sha2_256_pk_seed);
  sha2_256_adrsc(&sha2, var);
  sha2_256_update(&sha2, m, m_sz);
  sha2_256_final_len(&sha2, h, n);
}

/* Cat 1: F(PK.seed, ADRS, M1 ) = */
/* Trunc_n(SHA-256(PK.seed || toByte(0, 64 − n) || ADRSc || M1 )) */

static void sha2_256_f(slh_var_t *var, uint8_t *h, const uint8_t *m1)
{
  sha2_256_tl(var, h, m1, var->prm->n);
}

/* Cat 1: H(PK.seed, ADRS, M2 ) = */
/* Trunc_n(SHA-256(PK.seed || toByte(0, 64 − n) || ADRSc || M2 )) */

static void sha2_256_h(slh_var_t *var, uint8_t *h, const uint8_t *m1,
                       const uint8_t *m2)
{
  sha2_256_t sha2;
  size_t n = var->prm->n;

  sha2_256_copy(&sha2, &var->sha2_256_pk_seed);
  sha2_256_adrsc(&sha2, var);
  sha2_256_update(&sha2, m1, n);
  sha2_256_update(&sha2, m2, n);
  sha2_256_final_len(&sha2, h, n);
}

/* Cat 3, 5: Tl(PK.seed, ADRS, Ml ) = */
/* Trunc_n(SHA-512(PK.seed || toByte(0, 128 − n) || ADRSc || Ml )) */

static void sha2_512_tl(slh_var_t *var, uint8_t *h, const uint8_t *m,
                        size_t m_sz)
{
  sha2_512_t sha2;
  size_t n = var->prm->n;

  sha2_512_copy(&sha2, &var->sha2_512_pk_seed);
  sha2_512_adrsc(&sha2, var);
  sha2_512_update(&sha2, m, m_sz);
  sha2_512_final_len(&sha2, h, n);
}

/* Cat 3, 5: H(PK.seed, ADRS, M2 ) = */
/* Trunc_n(SHA-512(PK.seed || toByte(0, 128 − n) || ADRSc || M2 )) */

static void sha2_512_h(slh_var_t *var, uint8_t *h, const uint8_t *m1,
                       const uint8_t *m2)
{
  sha2_512_t sha2;
  size_t n = var->prm->n;

  sha2_512_copy(&sha2, &var->sha2_512_pk_seed);
  sha2_512_adrsc(&sha2, var);
  sha2_512_update(&sha2, m1, n);
  sha2_512_update(&sha2, m2, n);
  sha2_512_final_len(&sha2, h, n);
}

/* create a context */

static void sha2_mk_var(slh_var_t *var, const uint8_t *pk, const uint8_t *sk,
                        const slh_param_t *prm)
{
  uint8_t buf[128 - 24];
  size_t n = prm->n;
  var->prm = prm;

  var->prm = prm; /* store fixed parameters */
  if (sk != NULL)
  {
    memcpy(var->sk_seed, sk, n);
    memcpy(var->sk_prf, sk + n, n);
    memcpy(var->pk_seed, sk + 2 * n, n);
    memcpy(var->pk_root, sk + 3 * n, n);
  }
  else if (pk != NULL)
  {
    memcpy(var->pk_seed, pk, n);
    memcpy(var->pk_root, pk + n, n);
  }

  /* eliminate the first compression function */
  memset(buf, 0x00, sizeof(buf));
  sha2_256_init(&var->sha2_256_pk_seed);
  sha2_256_update(&var->sha2_256_pk_seed, var->pk_seed, n);
  sha2_256_update(&var->sha2_256_pk_seed, buf, 64 - n);

  if (n > 16)
  {
    sha2_512_init(&var->sha2_512_pk_seed);
    sha2_512_update(&var->sha2_512_pk_seed, var->pk_seed, n);
    sha2_512_update(&var->sha2_512_pk_seed, buf, 128 - n);
  }

  /* local ADRS buffer */
  var->adrs = &var->t_adrs;
}

/* === Chaining function used in WOTS+ */
/* Algorithm 5: chain(X, i, s, PK.seed, ADRS) */

static void sha2_256_chain(slh_var_t *var, uint8_t *tmp, const uint8_t *x,
                           uint32_t i, uint32_t s)
{
  uint32_t j;
  size_t n = var->prm->n;
  sha2_256_t sha2;
  uint32_t sp[8 + 24];
  uint8_t *mp = (uint8_t *)&sp[8];
  uint8_t *op = (uint8_t *)&sha2.s[8];

  /* these cases exist */
  if (s == 0)
  {
    memcpy(tmp, x, n);
    return;
  }

  /* set initial address */
  adrs_set_hash_address(var, i);

  /* initial set-up */
  sha2_256_copy(&sha2, &var->sha2_256_pk_seed);
  sha2_256_adrsc(&sha2, var);
  sha2_256_update(&sha2, x, n);
  if (s == 1)
  {
    /* just one hash */
    sha2_256_final_len(&sha2, tmp, n);
    return;
  }
  sha2_256_final_pad(&sha2);
  memcpy(sp, sha2.s, sizeof(sp));
  sha2_256_compress(sp);

  /* iteration */
  for (j = 1; j < s; j++)
  {
    memcpy(mp + 22, sp, n);        /* copy result back to input */
    slh_tobyte(mp + 18, i + j, 4); /* hash address, last part of ADSc */
    memcpy(sp, sha2.s, 32 + 18);   /* PK.seed compressed, start of ADRSc */
    memcpy(mp + 22 + n, op + 22 + n, 64 - 22 - n); /* padding */
    sha2_256_compress(sp);
  }

  /* final output */
  memcpy(tmp, sp, n);
}

/* Combination WOTS PRF + Chain */

static void sha2_256_wots_chain(slh_var_t *var, uint8_t *tmp, uint32_t s)
{
  /* PRF secret key */
  adrs_set_type(var, ADRS_WOTS_PRF);
  adrs_set_tree_index(var, 0);
  sha2_256_prf(var, tmp);

  /* chain */
  adrs_set_type(var, ADRS_WOTS_HASH);
  adrs_set_tree_index(var, 0);
  sha2_256_chain(var, tmp, tmp, 0, s);
}

/* Combination FORS PRF + F (if s == 1) */

static void sha2_256_fors_hash(slh_var_t *var, uint8_t *tmp, uint32_t s)
{
  /* PRF secret key */
  adrs_set_type(var, ADRS_FORS_PRF);
  adrs_set_tree_height(var, 0);
  sha2_256_prf(var, tmp);

  /* hash it again */
  if (s == 1)
  {
    adrs_set_type(var, ADRS_FORS_TREE);
    sha2_256_f(var, tmp, tmp);
  }
}

/* 10.2.   SLH-DSA Using SHA2 for Security Category 1 */

OQS_API const slh_param_t slh_dsa_sha2_128s = {/* .alg_id = */ "SLH-DSA-SHA2-128s",
                                       /* .n = */ 16,
                                       /* .h = */ 63,
                                       /* .d = */ 7,
                                       /* .hp = */ 9,
                                       /* .a = */ 12,
                                       /* .k = */ 14,
                                       /* .lg_w = */ 4,
                                       /* .m = */ 30,
                                       /* .mk_var = */ sha2_mk_var,
                                       /* .chain = */ sha2_256_chain,
                                       /* .wots_chain = */ sha2_256_wots_chain,
                                       /* .fors_hash = */ sha2_256_fors_hash,
                                       /* .h_msg = */ sha2_256_h_msg,
                                       /* .prf = */ sha2_256_prf,
                                       /* .prf_msg = */ sha2_256_prf_msg,
                                       /* .h_f = */ sha2_256_f,
                                       /* .h_h = */ sha2_256_h,
                                       /* .h_t = */ sha2_256_tl};

OQS_API const slh_param_t slh_dsa_sha2_128f = {/* .alg_id = */ "SLH-DSA-SHA2-128f",
                                       /* .n = */ 16,
                                       /* .h = */ 66,
                                       /* .d = */ 22,
                                       /* .hp = */ 3,
                                       /* .a = */ 6,
                                       /* .k = */ 33,
                                       /* .lg_w = */ 4,
                                       /* .m = */ 34,
                                       /* .mk_var = */ sha2_mk_var,
                                       /* .chain = */ sha2_256_chain,
                                       /* .wots_chain = */ sha2_256_wots_chain,
                                       /* .fors_hash = */ sha2_256_fors_hash,
                                       /* .h_msg = */ sha2_256_h_msg,
                                       /* .prf = */ sha2_256_prf,
                                       /* .prf_msg = */ sha2_256_prf_msg,
                                       /* .h_f = */ sha2_256_f,
                                       /* .h_h = */ sha2_256_h,
                                       /* .h_t = */ sha2_256_tl};

/* 10.3.   SLH-DSA Using SHA2 for Security Categories 3 and 5 */

OQS_API const slh_param_t slh_dsa_sha2_192s = {/* .alg_id = */ "SLH-DSA-SHA2-192s",
                                       /* .n = */ 24,
                                       /* .h = */ 63,
                                       /* .d = */ 7,
                                       /* .hp = */ 9,
                                       /* .a = */ 14,
                                       /* .k = */ 17,
                                       /* .lg_w = */ 4,
                                       /* .m = */ 39,
                                       /* .mk_var = */ sha2_mk_var,
                                       /* .chain = */ sha2_256_chain,
                                       /* .wots_chain = */ sha2_256_wots_chain,
                                       /* .fors_hash = */ sha2_256_fors_hash,
                                       /* .h_msg = */ sha2_512_h_msg,
                                       /* .prf = */ sha2_256_prf,
                                       /* .prf_msg = */ sha2_512_prf_msg,
                                       /* .h_f = */ sha2_256_f,
                                       /* .h_h = */ sha2_512_h,
                                       /* .h_t = */ sha2_512_tl};

OQS_API const slh_param_t slh_dsa_sha2_192f = {/* .alg_id = */ "SLH-DSA-SHA2-192f",
                                       /* .n = */ 24,
                                       /* .h = */ 66,
                                       /* .d = */ 22,
                                       /* .hp = */ 3,
                                       /* .a = */ 8,
                                       /* .k = */ 33,
                                       /* .lg_w = */ 4,
                                       /* .m = */ 42,
                                       /* .mk_var = */ sha2_mk_var,
                                       /* .chain = */ sha2_256_chain,
                                       /* .wots_chain = */ sha2_256_wots_chain,
                                       /* .fors_hash = */ sha2_256_fors_hash,
                                       /* .h_msg = */ sha2_512_h_msg,
                                       /* .prf = */ sha2_256_prf,
                                       /* .prf_msg = */ sha2_512_prf_msg,
                                       /* .h_f = */ sha2_256_f,
                                       /* .h_h = */ sha2_512_h,
                                       /* .h_t = */ sha2_512_tl};

OQS_API const slh_param_t slh_dsa_sha2_256s = {/* .alg_id = */ "SLH-DSA-SHA2-256s",
                                       /* .n = */ 32,
                                       /* .h = */ 64,
                                       /* .d = */ 8,
                                       /* .hp = */ 8,
                                       /* .a = */ 14,
                                       /* .k = */ 22,
                                       /* .lg_w = */ 4,
                                       /* .m = */ 47,
                                       /* .mk_var = */ sha2_mk_var,
                                       /* .chain = */ sha2_256_chain,
                                       /* .wots_chain = */ sha2_256_wots_chain,
                                       /* .fors_hash = */ sha2_256_fors_hash,
                                       /* .h_msg = */ sha2_512_h_msg,
                                       /* .prf = */ sha2_256_prf,
                                       /* .prf_msg = */ sha2_512_prf_msg,
                                       /* .h_f = */ sha2_256_f,
                                       /* .h_h = */ sha2_512_h,
                                       /* .h_t = */ sha2_512_tl};

OQS_API const slh_param_t slh_dsa_sha2_256f = {/* .alg_id = */ "SLH-DSA-SHA2-256f",
                                       /* .n = */ 32,
                                       /* .h = */ 68,
                                       /* .d = */ 17,
                                       /* .hp = */ 4,
                                       /* .a = */ 9,
                                       /* .k = */ 35,
                                       /* .lg_w = */ 4,
                                       /* .m = */ 49,
                                       /* .mk_var = */ sha2_mk_var,
                                       /* .chain = */ sha2_256_chain,
                                       /* .wots_chain = */ sha2_256_wots_chain,
                                       /* .fors_hash = */ sha2_256_fors_hash,
                                       /* .h_msg = */ sha2_512_h_msg,
                                       /* .prf = */ sha2_256_prf,
                                       /* .prf_msg = */ sha2_512_prf_msg,
                                       /* .h_f = */ sha2_256_f,
                                       /* .h_h = */ sha2_512_h,
                                       /* .h_t = */ sha2_512_tl};
