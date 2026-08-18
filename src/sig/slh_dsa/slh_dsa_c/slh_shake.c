/*
 * Copyright (c) The slhdsa-c project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* === Portable C code: Functions for instantiation of SLH-DSA with SHAKE */

#include <oqs/oqs.h>
#include "sha3_api.h"
#include "slh_adrs.h"
#include "slh_var.h"

/* === 10.1.   SLH-DSA Using SHAKE */

/* Hmsg(R, PK.seed, PK.root, M) = SHAKE256(R || PK.seed || PK.root || M, */
/* 8m) */

static void shake_h_msg(slh_var_t *var, uint8_t *h, const uint8_t *r,
                        const uint8_t *m, size_t m_sz, const uint8_t *ctx,
                        size_t ctx_sz)
{
  sha3_var_t sha3;
  size_t n = var->prm->n;
  uint8_t buf[2];

  shake256_init(&sha3);
  shake_update(&sha3, r, n);
  shake_update(&sha3, var->pk_seed, n);
  shake_update(&sha3, var->pk_root, n);

  /* add "pure" domain separator and context, if supplied */
  if (ctx_sz != SLH_CTX_SZ_NO_CONTEXT)
  {
    buf[0] = 0;
    buf[1] = ctx_sz & 0xFF;
    shake_update(&sha3, buf, 2);
    shake_update(&sha3, ctx, ctx_sz);
  }
  shake_update(&sha3, m, m_sz);

  shake_out(&sha3, h, var->prm->m);
}

/* F(PK.seed, ADRS, M1 ) = SHAKE256(PK.seed || ADRS || M1, 8n) */

static void shake_f(slh_var_t *var, uint8_t *h, const uint8_t *m1)
{
  sha3_var_t sha3;
  size_t n = var->prm->n;

  shake256_init(&sha3);
  shake_update(&sha3, var->pk_seed, n);
  shake_update(&sha3, (const uint8_t *)var->adrs->u8, 32);
  shake_update(&sha3, m1, n);

  shake_out(&sha3, h, n);
}

/* PRF(PK.seed, SK.seed, ADRS) = SHAKE256(PK.seed || ADRS || SK.seed, 8n) */

static void shake_prf(slh_var_t *var, uint8_t *h)
{
  shake_f(var, h, var->sk_seed);
}

/* PRFmsg (SK.prf, opt_rand, M) = SHAKE256(SK.prf || opt_rand || M, 8n) */

static void shake_prf_msg(slh_var_t *var, uint8_t *h, const uint8_t *opt_rand,
                          const uint8_t *m, size_t m_sz, const uint8_t *ctx,
                          size_t ctx_sz)

{
  sha3_var_t sha3;
  size_t n = var->prm->n;
  uint8_t buf[2];

  shake256_init(&sha3);
  shake_update(&sha3, var->sk_prf, n);
  shake_update(&sha3, opt_rand, n);

  /* add "pure" domain separator and context, if supplied */
  if (ctx_sz != SLH_CTX_SZ_NO_CONTEXT)
  {
    buf[0] = 0;
    buf[1] = ctx_sz & 0xFF;
    shake_update(&sha3, buf, 2);
    shake_update(&sha3, ctx, ctx_sz);
  }
  shake_update(&sha3, m, m_sz);

  shake_out(&sha3, h, n);
}

/* T_l(PK.seed, ADRS, M ) = SHAKE256(PK.seed || ADRS || Ml, 8n) */

static void shake_t(slh_var_t *var, uint8_t *h, const uint8_t *m, size_t m_sz)
{
  sha3_var_t sha3;
  size_t n = var->prm->n;

  shake256_init(&sha3);
  shake_update(&sha3, var->pk_seed, n);
  shake_update(&sha3, (const uint8_t *)var->adrs->u8, 32);
  shake_update(&sha3, m, m_sz);

  shake_out(&sha3, h, n);
}

/* H(PK.seed, ADRS, M2 ) = SHAKE256(PK.seed || ADRS || M2, 8n) */

static void shake_h(slh_var_t *var, uint8_t *h, const uint8_t *m1,
                    const uint8_t *m2)
{
  sha3_var_t sha3;
  size_t n = var->prm->n;

  shake256_init(&sha3);
  shake_update(&sha3, var->pk_seed, n);
  shake_update(&sha3, (const uint8_t *)var->adrs->u8, 32);
  shake_update(&sha3, m1, n);
  shake_update(&sha3, m2, n);

  shake_out(&sha3, h, n);
}

/* create a context */

static void shake_mk_var(slh_var_t *var, const uint8_t *pk, const uint8_t *sk,
                         const slh_param_t *prm)
{
  size_t n = prm->n;

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

  /* local ADRS buffer */
  var->adrs = &var->t_adrs;
}

/* === Chaining function used in WOTS+ */
/* Algorithm 5: chain(X, i, s, PK.seed, ADRS) */

/* chaining by processor (some optimizations) */

static void shake_chain(slh_var_t *var, uint8_t *tmp, const uint8_t *x,
                        uint32_t i, uint32_t s)
{
  uint32_t j, k;
  uint64_t ks[25];
  size_t n = var->prm->n;
  const uint32_t r = (1600 - 256 * 2) / 64; /* SHAKE256 rate */
  uint32_t n8 = n / 8;                      /* number of words */
  uint32_t h = n8 + (32 / 8);               /* static part len */
  uint32_t l = h + n8;                      /* input length */

  if (s == 0)
  { /* no-op */
    memcpy(tmp, x, n);
    return;
  }

  memcpy(ks + h, x, n); /* start node */
  for (j = 0; j < s; j++)
  {
    if (j > 0)
    {
      memcpy(ks + h, ks, n); /* chaining */
    }
    memcpy(ks, var->pk_seed, n);       /* PK.seed */
    adrs_set_hash_address(var, i + j); /* address */
    memcpy(ks + n8, (const uint8_t *)var->adrs->u8, 32);

    /* padding */
    ks[l] = 0x1F; /* shake padding */
    for (k = l + 1; k < r - 1; k++)
    {
      ks[k] = 0;
    }
    ks[r - 1] = UINT64_C(1) << 63; /* rate padding */
    for (k = r; k < 25; k++)
    {
      ks[k] = 0;
    }

    keccak_f1600(ks); /* permutation */
  }
  memcpy(tmp, ks, n);
}

/* Combination WOTS PRF + Chain */

static void shake_wots_chain(slh_var_t *var, uint8_t *tmp, uint32_t s)
{
  /* PRF secret key */
  adrs_set_type(var, ADRS_WOTS_PRF);
  adrs_set_tree_index(var, 0);
  shake_prf(var, tmp);

  /* chain */
  adrs_set_type(var, ADRS_WOTS_HASH);
  shake_chain(var, tmp, tmp, 0, s);
}

/* Combination FORS PRF + F (if s == 1) */

static void shake_fors_hash(slh_var_t *var, uint8_t *tmp, uint32_t s)
{
  /* PRF secret key */
  adrs_set_type(var, ADRS_FORS_PRF);
  adrs_set_tree_height(var, 0);
  shake_prf(var, tmp);

  /* hash it again */
  if (s == 1)
  {
    adrs_set_type(var, ADRS_FORS_TREE);
    shake_f(var, tmp, tmp);
  }
}

/* parameter sets */

OQS_API const slh_param_t slh_dsa_shake_128s = {/* .alg_id = */ "SLH-DSA-SHAKE-128s",
                                        /* .n = */ 16,
                                        /* .h = */ 63,
                                        /* .d = */ 7,
                                        /* .hp = */ 9,
                                        /* .a = */ 12,
                                        /* .k = */ 14,
                                        /* .lg_w = */ 4,
                                        /* .m = */ 30,
                                        /* .mk_var = */ shake_mk_var,
                                        /* .chain = */ shake_chain,
                                        /* .wots_chain = */ shake_wots_chain,
                                        /* .fors_hash = */ shake_fors_hash,
                                        /* .h_msg = */ shake_h_msg,
                                        /* .prf = */ shake_prf,
                                        /* .prf_msg = */ shake_prf_msg,
                                        /* .h_f = */ shake_f,
                                        /* .h_h = */ shake_h,
                                        /* .h_t = */ shake_t};

OQS_API const slh_param_t slh_dsa_shake_128f = {/* .alg_id = */ "SLH-DSA-SHAKE-128f",
                                        /* .n = */ 16,
                                        /* .h = */ 66,
                                        /* .d = */ 22,
                                        /* .hp = */ 3,
                                        /* .a = */ 6,
                                        /* .k = */ 33,
                                        /* .lg_w = */ 4,
                                        /* .m = */ 34,
                                        /* .mk_var = */ shake_mk_var,
                                        /* .chain = */ shake_chain,
                                        /* .wots_chain = */ shake_wots_chain,
                                        /* .fors_hash = */ shake_fors_hash,
                                        /* .h_msg = */ shake_h_msg,
                                        /* .prf = */ shake_prf,
                                        /* .prf_msg = */ shake_prf_msg,
                                        /* .h_f = */ shake_f,
                                        /* .h_h = */ shake_h,
                                        /* .h_t = */ shake_t};

OQS_API const slh_param_t slh_dsa_shake_192s = {/* .alg_id = */ "SLH-DSA-SHAKE-192s",
                                        /* .n = */ 24,
                                        /* .h = */ 63,
                                        /* .d = */ 7,
                                        /* .hp = */ 9,
                                        /* .a = */ 14,
                                        /* .k = */ 17,
                                        /* .lg_w = */ 4,
                                        /* .m = */ 39,
                                        /* .mk_var = */ shake_mk_var,
                                        /* .chain = */ shake_chain,
                                        /* .wots_chain = */ shake_wots_chain,
                                        /* .fors_hash = */ shake_fors_hash,
                                        /* .h_msg = */ shake_h_msg,
                                        /* .prf = */ shake_prf,
                                        /* .prf_msg = */ shake_prf_msg,
                                        /* .h_f = */ shake_f,
                                        /* .h_h = */ shake_h,
                                        /* .h_t = */ shake_t};

OQS_API const slh_param_t slh_dsa_shake_192f = {/* .alg_id = */ "SLH-DSA-SHAKE-192f",
                                        /* .n = */ 24,
                                        /* .h = */ 66,
                                        /* .d = */ 22,
                                        /* .hp = */ 3,
                                        /* .a = */ 8,
                                        /* .k = */ 33,
                                        /* .lg_w = */ 4,
                                        /* .m = */ 42,
                                        /* .mk_var = */ shake_mk_var,
                                        /* .chain = */ shake_chain,
                                        /* .wots_chain = */ shake_wots_chain,
                                        /* .fors_hash = */ shake_fors_hash,
                                        /* .h_msg = */ shake_h_msg,
                                        /* .prf = */ shake_prf,
                                        /* .prf_msg = */ shake_prf_msg,
                                        /* .h_f = */ shake_f,
                                        /* .h_h = */ shake_h,
                                        /* .h_t = */ shake_t};

OQS_API const slh_param_t slh_dsa_shake_256s = {/* .alg_id = */ "SLH-DSA-SHAKE-256s",
                                        /* .n = */ 32,
                                        /* .h = */ 64,
                                        /* .d = */ 8,
                                        /* .hp = */ 8,
                                        /* .a = */ 14,
                                        /* .k = */ 22,
                                        /* .lg_w = */ 4,
                                        /* .m = */ 47,
                                        /* .mk_var = */ shake_mk_var,
                                        /* .chain = */ shake_chain,
                                        /* .wots_chain = */ shake_wots_chain,
                                        /* .fors_hash = */ shake_fors_hash,
                                        /* .h_msg = */ shake_h_msg,
                                        /* .prf = */ shake_prf,
                                        /* .prf_msg = */ shake_prf_msg,
                                        /* .h_f = */ shake_f,
                                        /* .h_h = */ shake_h,
                                        /* .h_t = */ shake_t};

OQS_API const slh_param_t slh_dsa_shake_256f = {/* .alg_id = */ "SLH-DSA-SHAKE-256f",
                                        /* .n = */ 32,
                                        /* .h = */ 68,
                                        /* .d = */ 17,
                                        /* .hp = */ 4,
                                        /* .a = */ 9,
                                        /* .k = */ 35,
                                        /* .lg_w = */ 4,
                                        /* .m = */ 49,
                                        /* .mk_var = */ shake_mk_var,
                                        /* .chain = */ shake_chain,
                                        /* .wots_chain = */ shake_wots_chain,
                                        /* .fors_hash = */ shake_fors_hash,
                                        /* .h_msg = */ shake_h_msg,
                                        /* .prf = */ shake_prf,
                                        /* .prf_msg = */ shake_prf_msg,
                                        /* .h_f = */ shake_f,
                                        /* .h_h = */ shake_h,
                                        /* .h_t = */ shake_t};
