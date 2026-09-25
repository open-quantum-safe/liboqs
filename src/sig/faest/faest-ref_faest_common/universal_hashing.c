/*
 *  SPDX-License-Identifier: MIT
 */

#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif

#include "instances.h"
#include "universal_hashing.h"
#include "utils.h"

#include <assert.h>
#include <string.h>

void vole_hash_128(uint8_t* h, const uint8_t* sd, const uint8_t* x, unsigned int ell) {
  const unsigned int ell_prime = ell + D_ZK - 1;

  bf128_t bf_y[5];
  {
    const uint8_t* s0 = sd + 4 * BF128_NUM_BYTES;
    const uint8_t* t  = sd + 8 * BF128_NUM_BYTES;

    // line 5
    bf128_t bf_s[5];
    for (unsigned int i = 0; i != 4; ++i) {
      bf128_load(&bf_s[i], s0 + i * BF128_NUM_BYTES);
    }

    // line 6
    {
      uint8_t t_zero_padd[BF128_NUM_BYTES] = {0};
      memcpy(t_zero_padd, t, 8);
      bf128_load(&bf_s[4], t_zero_padd);
    }

    // line 7
    for (unsigned int j = 0; j < 5; j++) {
      bf128_load(&bf_y[j], x);
    }

    const unsigned int deg_max = ell_prime - 1;
    for (unsigned int i = 1; i <= deg_max; ++i) {
      bf128_t bf_x0;
      bf128_load(&bf_x0, x + i * BF128_NUM_BYTES);
      for (unsigned int j = 0; j < 5; j++) {
        bf128_mul_inplace(&bf_y[j], &bf_s[j]);
        bf128_add_inplace(&bf_y[j], &bf_x0);
      }
    }
  }

  // line 4
  for (unsigned int i = 0; i != 4; ++i) {
    bf128_t bf_h;
    bf128_load(&bf_h, sd + i * BF128_NUM_BYTES);

    // line 8
    bf128_mul_inplace(&bf_h, &bf_y[0]);
    bf128_add_inplace(&bf_h, &bf_y[i + 1]);

    bf128_t x1i;
    bf128_load(&x1i, x + (ell_prime + i) * BF128_NUM_BYTES);
    bf128_add_inplace(&bf_h, &x1i);

    bf128_store(h + i * BF128_NUM_BYTES, &bf_h);
  }
}

void vole_hash_192(uint8_t* h, const uint8_t* sd, const uint8_t* x, unsigned int ell) {
  const unsigned int ell_prime = ell + D_ZK - 1;

  bf192_t bf_y[5];
  {
    const uint8_t* s0 = sd + 4 * BF192_NUM_BYTES;
    const uint8_t* t  = sd + 8 * BF192_NUM_BYTES;

    // line 5
    bf192_t bf_s[5];
    for (unsigned int i = 0; i != 4; ++i) {
      bf192_load(&bf_s[i], s0 + i * BF192_NUM_BYTES);
    }

    // line 6
    {
      uint8_t t_zero_padd[BF192_NUM_BYTES] = {0};
      memcpy(t_zero_padd, t, 8);
      bf192_load(&bf_s[4], t_zero_padd);
    }

    // line 7
    for (unsigned int j = 0; j < 5; j++) {
      bf192_load(&bf_y[j], x);
    }

    const unsigned int deg_max = ell_prime - 1;
    for (unsigned int i = 1; i <= deg_max; ++i) {
      bf192_t bf_x0;
      bf192_load(&bf_x0, x + i * BF192_NUM_BYTES);
      for (unsigned int j = 0; j < 5; j++) {
        bf192_mul_inplace(&bf_y[j], &bf_s[j]);
        bf192_add_inplace(&bf_y[j], &bf_x0);
      }
    }
  }

  // line 4
  for (unsigned int i = 0; i != 4; ++i) {
    bf192_t bf_h;
    bf192_load(&bf_h, sd + i * BF192_NUM_BYTES);

    // line 8
    bf192_mul_inplace(&bf_h, &bf_y[0]);
    bf192_add_inplace(&bf_h, &bf_y[i + 1]);

    bf192_t x1i;
    bf192_load(&x1i, x + (ell_prime + i) * BF192_NUM_BYTES);
    bf192_add_inplace(&bf_h, &x1i);

    bf192_store(h + i * BF192_NUM_BYTES, &bf_h);
  }
}

void vole_hash_256(uint8_t* h, const uint8_t* sd, const uint8_t* x, unsigned int ell) {
  const unsigned int ell_prime = ell + D_ZK - 1;

  bf256_t bf_y[5];
  {
    const uint8_t* s0 = sd + 4 * BF256_NUM_BYTES;
    const uint8_t* t  = sd + 8 * BF256_NUM_BYTES;

    // line 5
    bf256_t bf_s[5];
    for (unsigned int i = 0; i != 4; ++i) {
      bf256_load(&bf_s[i], s0 + i * BF256_NUM_BYTES);
    }

    // line 6
    {
      uint8_t t_zero_padd[BF256_NUM_BYTES] = {0};
      memcpy(t_zero_padd, t, 8);
      bf256_load(&bf_s[4], t_zero_padd);
    }

    // line 7
    for (unsigned int j = 0; j < 5; j++) {
      bf256_load(&bf_y[j], x);
    }

    const unsigned int deg_max = ell_prime - 1;
    for (unsigned int i = 1; i <= deg_max; ++i) {
      bf256_t bf_x0;
      bf256_load(&bf_x0, x + i * BF256_NUM_BYTES);
      for (unsigned int j = 0; j < 5; j++) {
        bf256_mul_inplace(&bf_y[j], &bf_s[j]);
        bf256_add_inplace(&bf_y[j], &bf_x0);
      }
    }
  }

  // line 4
  for (unsigned int i = 0; i != 4; ++i) {
    bf256_t bf_h;
    bf256_load(&bf_h, sd + i * BF256_NUM_BYTES);

    // line 8
    bf256_mul_inplace(&bf_h, &bf_y[0]);
    bf256_add_inplace(&bf_h, &bf_y[i + 1]);

    bf256_t x1i;
    bf256_load(&x1i, x + (ell_prime + i) * BF256_NUM_BYTES);
    bf256_add_inplace(&bf_h, &x1i);

    bf256_store(h + i * BF256_NUM_BYTES, &bf_h);
  }
}

void vole_hash(uint8_t* h, const uint8_t* sd, const uint8_t* x, unsigned int ell, uint32_t lambda) {
  switch (lambda) {
  case 256:
    vole_hash_256(h, sd, x, ell);
    break;
  case 192:
    vole_hash_192(h, sd, x, ell);
    break;
  default:
    vole_hash_128(h, sd, x, ell);
    break;
  }
}

void zk_hash_128_init(zk_hash_128_ctx* ctx, const uint8_t* sd) {
  const uint8_t* s = sd + 2 * BF128_NUM_BYTES;
  const uint8_t* t = sd + 3 * BF128_NUM_BYTES;

  ctx->h0 = bf128_zero();
  ctx->h1 = bf128_zero();
  bf128_load(&ctx->s, s);
  ctx->t  = bf64_load(t);
  ctx->sd = sd;
}

void zk_hash_128_update(zk_hash_128_ctx* ctx, const bf128_t* v) {
  bf128_mul_inplace(&ctx->h0, &ctx->s);
  bf128_add_inplace(&ctx->h0, v);
  bf128_mul_64_inplace(&ctx->h1, ctx->t);
  bf128_add_inplace(&ctx->h1, v);
}

void zk_hash_128_finalize(uint8_t* h, zk_hash_128_ctx* ctx, const bf128_t* x1) {
  bf128_t r0;
  bf128_t r1;
  bf128_load(&r0, ctx->sd);
  bf128_load(&r1, ctx->sd + BF128_NUM_BYTES);

  bf128_mul_inplace(&r0, &ctx->h0);
  bf128_mul_inplace(&r1, &ctx->h1);

  bf128_add_inplace(&r0, &r1);
  bf128_add_inplace(&r0, x1);
  bf128_store(h, &r0);
}

void zk_hash_128_7_init(zk_hash_128_7_ctx* ctx, const uint8_t* sd) {
  const uint8_t* s = sd + 2 * BF128_NUM_BYTES;
  const uint8_t* t = sd + 3 * BF128_NUM_BYTES;

  ctx->h0[0] = bf128_zero();
  ctx->h0[1] = bf128_zero();
  ctx->h0[2] = bf128_zero();
  ctx->h0[3] = bf128_zero();
  ctx->h0[4] = bf128_zero();
  ctx->h0[5] = bf128_zero();
  ctx->h0[6] = bf128_zero();

  ctx->h1[0] = bf128_zero();
  ctx->h1[1] = bf128_zero();
  ctx->h1[2] = bf128_zero();
  ctx->h1[3] = bf128_zero();
  ctx->h1[4] = bf128_zero();
  ctx->h1[5] = bf128_zero();
  ctx->h1[6] = bf128_zero();

  bf128_load(&ctx->s, s);
  ctx->t  = bf64_load(t);
  ctx->sd = sd;
}

void zk_hash_128_7_update(zk_hash_128_7_ctx* ctx, const bf128_t* v_i) {
  for (unsigned int deg_idx = 0; deg_idx < 7; deg_idx++) {
    bf128_mul_inplace(&ctx->h0[deg_idx], &ctx->s);
    bf128_add_inplace(&ctx->h0[deg_idx], v_i + deg_idx);

    bf128_mul_64_inplace(&ctx->h1[deg_idx], ctx->t);
    bf128_add_inplace(&ctx->h1[deg_idx], v_i + deg_idx);
  }
}

void zk_hash_128_7_raise_and_update(zk_hash_128_7_ctx* ctx, const bf128_t* v_1,
                                    const bf128_t* v_2) {
  for (unsigned int deg_idx = 0; deg_idx < 7; deg_idx++) {
    bf128_mul_inplace(&ctx->h0[deg_idx], &ctx->s);
    bf128_mul_64_inplace(&ctx->h1[deg_idx], ctx->t);
  }

  bf128_add_inplace(&ctx->h0[5], v_1);
  bf128_add_inplace(&ctx->h1[5], v_1);
  bf128_add_inplace(&ctx->h0[6], v_2);
  bf128_add_inplace(&ctx->h1[6], v_2);
}

void zk_hash_128_7_finalize(uint8_t* a0, uint8_t* a1toi, zk_hash_128_7_ctx* ctx,
                            const bf128_t* x1_i) {
  bf128_t r0;
  bf128_t r1;
  bf128_load(&r0, ctx->sd);
  bf128_load(&r1, ctx->sd + BF128_NUM_BYTES);

  bf128_t t0;
  bf128_t t1;
  bf128_mul(&t0, &r0, &ctx->h0[0]);
  bf128_mul(&t1, &r1, &ctx->h1[0]);
  bf128_add_inplace(&t0, &t1);
  bf128_add_inplace(&t0, &x1_i[0]);
  bf128_store(a0, &t0);

  for (unsigned int i = 1; i < 7; i++) {
    bf128_mul(&t0, &r0, &ctx->h0[i]);
    bf128_mul(&t1, &r1, &ctx->h1[i]);
    bf128_add_inplace(&t0, &t1);
    bf128_add_inplace(&t0, &x1_i[i]);
    bf128_store(a1toi + (i - 1) * BF128_NUM_BYTES, &t0);
  }
}

void zk_hash_192_init(zk_hash_192_ctx* ctx, const uint8_t* sd) {
  const uint8_t* s = sd + 2 * BF192_NUM_BYTES;
  const uint8_t* t = sd + 3 * BF192_NUM_BYTES;

  ctx->h0 = bf192_zero();
  ctx->h1 = bf192_zero();
  bf192_load(&ctx->s, s);
  ctx->t  = bf64_load(t);
  ctx->sd = sd;
}

void zk_hash_192_update(zk_hash_192_ctx* ctx, const bf192_t* v) {
  bf192_mul_inplace(&ctx->h0, &ctx->s);
  bf192_add_inplace(&ctx->h0, v);
  bf192_mul_64_inplace(&ctx->h1, ctx->t);
  bf192_add_inplace(&ctx->h1, v);
}

void zk_hash_192_finalize(uint8_t* h, zk_hash_192_ctx* ctx, const bf192_t* x1) {
  bf192_t r0;
  bf192_t r1;
  bf192_load(&r0, ctx->sd);
  bf192_load(&r1, ctx->sd + BF192_NUM_BYTES);

  bf192_mul_inplace(&r0, &ctx->h0);
  bf192_mul_inplace(&r1, &ctx->h1);

  bf192_add_inplace(&r0, &r1);
  bf192_add_inplace(&r0, x1);
  bf192_store(h, &r0);
}

void zk_hash_192_7_init(zk_hash_192_7_ctx* ctx, const uint8_t* sd) {
  const uint8_t* s = sd + 2 * BF192_NUM_BYTES;
  const uint8_t* t = sd + 3 * BF192_NUM_BYTES;

  ctx->h0[0] = bf192_zero();
  ctx->h0[1] = bf192_zero();
  ctx->h0[2] = bf192_zero();
  ctx->h0[3] = bf192_zero();
  ctx->h0[4] = bf192_zero();
  ctx->h0[5] = bf192_zero();
  ctx->h0[6] = bf192_zero();

  ctx->h1[0] = bf192_zero();
  ctx->h1[1] = bf192_zero();
  ctx->h1[2] = bf192_zero();
  ctx->h1[3] = bf192_zero();
  ctx->h1[4] = bf192_zero();
  ctx->h1[5] = bf192_zero();
  ctx->h1[6] = bf192_zero();

  bf192_load(&ctx->s, s);
  ctx->t  = bf64_load(t);
  ctx->sd = sd;
}

void zk_hash_192_7_update(zk_hash_192_7_ctx* ctx, const bf192_t* v_i) {
  for (unsigned int deg_idx = 0; deg_idx < 7; deg_idx++) {
    bf192_mul_inplace(&ctx->h0[deg_idx], &ctx->s);
    bf192_add_inplace(&ctx->h0[deg_idx], v_i + deg_idx);

    bf192_mul_64_inplace(&ctx->h1[deg_idx], ctx->t);
    bf192_add_inplace(&ctx->h1[deg_idx], v_i + deg_idx);
  }
}

void zk_hash_192_7_raise_and_update(zk_hash_192_7_ctx* ctx, const bf192_t* v_1,
                                    const bf192_t* v_2) {
  for (unsigned int deg_idx = 0; deg_idx < 7; deg_idx++) {
    bf192_mul_inplace(&ctx->h0[deg_idx], &ctx->s);
    bf192_mul_64_inplace(&ctx->h1[deg_idx], ctx->t);
  }

  bf192_add_inplace(&ctx->h0[5], v_1);
  bf192_add_inplace(&ctx->h1[5], v_1);
  bf192_add_inplace(&ctx->h0[6], v_2);
  bf192_add_inplace(&ctx->h1[6], v_2);
}

void zk_hash_192_7_finalize(uint8_t* a0, uint8_t* a1toi, zk_hash_192_7_ctx* ctx,
                            const bf192_t* x1_i) {
  bf192_t r0;
  bf192_t r1;
  bf192_load(&r0, ctx->sd);
  bf192_load(&r1, ctx->sd + BF192_NUM_BYTES);

  bf192_t t0;
  bf192_t t1;
  bf192_mul(&t0, &r0, &ctx->h0[0]);
  bf192_mul(&t1, &r1, &ctx->h1[0]);
  bf192_add_inplace(&t0, &t1);
  bf192_add_inplace(&t0, &x1_i[0]);
  bf192_store(a0, &t0);

  for (unsigned int i = 1; i < 7; i++) {
    bf192_mul(&t0, &r0, &ctx->h0[i]);
    bf192_mul(&t1, &r1, &ctx->h1[i]);
    bf192_add_inplace(&t0, &t1);
    bf192_add_inplace(&t0, &x1_i[i]);
    bf192_store(a1toi + (i - 1) * BF192_NUM_BYTES, &t0);
  };
}

void zk_hash_256_init(zk_hash_256_ctx* ctx, const uint8_t* sd) {
  const uint8_t* s = sd + 2 * BF256_NUM_BYTES;
  const uint8_t* t = sd + 3 * BF256_NUM_BYTES;

  ctx->h0 = bf256_zero();
  ctx->h1 = bf256_zero();
  bf256_load(&ctx->s, s);
  ctx->t  = bf64_load(t);
  ctx->sd = sd;
}

void zk_hash_256_update(zk_hash_256_ctx* ctx, const bf256_t* v) {
  bf256_mul_inplace(&ctx->h0, &ctx->s);
  bf256_add_inplace(&ctx->h0, v);
  bf256_mul_64_inplace(&ctx->h1, ctx->t);
  bf256_add_inplace(&ctx->h1, v);
}

void zk_hash_256_finalize(uint8_t* h, zk_hash_256_ctx* ctx, const bf256_t* x1) {
  bf256_t r0;
  bf256_t r1;
  bf256_load(&r0, ctx->sd);
  bf256_load(&r1, ctx->sd + BF256_NUM_BYTES);

  bf256_mul_inplace(&r0, &ctx->h0);
  bf256_mul_inplace(&r1, &ctx->h1);

  bf256_add_inplace(&r0, &r1);
  bf256_add_inplace(&r0, x1);
  bf256_store(h, &r0);
}

void zk_hash_256_7_init(zk_hash_256_7_ctx* ctx, const uint8_t* sd) {
  const uint8_t* s = sd + 2 * BF256_NUM_BYTES;
  const uint8_t* t = sd + 3 * BF256_NUM_BYTES;

  ctx->h0[0] = bf256_zero();
  ctx->h0[1] = bf256_zero();
  ctx->h0[2] = bf256_zero();
  ctx->h0[3] = bf256_zero();
  ctx->h0[4] = bf256_zero();
  ctx->h0[5] = bf256_zero();
  ctx->h0[6] = bf256_zero();

  ctx->h1[0] = bf256_zero();
  ctx->h1[1] = bf256_zero();
  ctx->h1[2] = bf256_zero();
  ctx->h1[3] = bf256_zero();
  ctx->h1[4] = bf256_zero();
  ctx->h1[5] = bf256_zero();
  ctx->h1[6] = bf256_zero();

  bf256_load(&ctx->s, s);
  ctx->t  = bf64_load(t);
  ctx->sd = sd;
}

void zk_hash_256_7_update(zk_hash_256_7_ctx* ctx, const bf256_t* v_i) {
  for (unsigned int deg_idx = 0; deg_idx < 7; deg_idx++) {
    bf256_mul_inplace(&ctx->h0[deg_idx], &ctx->s);
    bf256_add_inplace(&ctx->h0[deg_idx], v_i + deg_idx);

    bf256_mul_64_inplace(&ctx->h1[deg_idx], ctx->t);
    bf256_add_inplace(&ctx->h1[deg_idx], v_i + deg_idx);
  }
}

void zk_hash_256_7_raise_and_update(zk_hash_256_7_ctx* ctx, const bf256_t* v_1,
                                    const bf256_t* v_2) {
  for (unsigned int deg_idx = 0; deg_idx < 7; deg_idx++) {
    bf256_mul_inplace(&ctx->h0[deg_idx], &ctx->s);
    bf256_mul_64_inplace(&ctx->h1[deg_idx], ctx->t);
  }

  bf256_add_inplace(&ctx->h0[5], v_1);
  bf256_add_inplace(&ctx->h1[5], v_1);
  bf256_add_inplace(&ctx->h0[6], v_2);
  bf256_add_inplace(&ctx->h1[6], v_2);
}

void zk_hash_256_7_finalize(uint8_t* a0, uint8_t* a1toi, zk_hash_256_7_ctx* ctx,
                            const bf256_t* x1_i) {
  bf256_t r0;
  bf256_t r1;
  bf256_load(&r0, ctx->sd);
  bf256_load(&r1, ctx->sd + BF256_NUM_BYTES);

  bf256_t t0;
  bf256_t t1;
  bf256_mul(&t0, &r0, &ctx->h0[0]);
  bf256_mul(&t1, &r1, &ctx->h1[0]);
  bf256_add_inplace(&t0, &t1);
  bf256_add_inplace(&t0, &x1_i[0]);
  bf256_store(a0, &t0);

  for (unsigned int i = 1; i < 7; i++) {
    bf256_mul(&t0, &r0, &ctx->h0[i]);
    bf256_mul(&t1, &r1, &ctx->h1[i]);
    bf256_add_inplace(&t0, &t1);
    bf256_add_inplace(&t0, &x1_i[i]);
    bf256_store(a1toi + (i - 1) * BF256_NUM_BYTES, &t0);
  };
}

void leaf_hash_128(uint8_t* h, const uint8_t* uhash, const uint8_t* x) {
  bf384_t u;
  bf384_load(&u, uhash);
  {
    bf128_t x0;
    bf128_load(&x0, x);
    bf384_mul_128_inplace(&u, &x0);
  }
  {
    bf384_t x1;
    bf384_load(&x1, x + BF128_NUM_BYTES);
    bf384_add_inplace(&u, &x1);
  }
  bf384_store(h, &u);
}

void leaf_hash_192(uint8_t* h, const uint8_t* uhash, const uint8_t* x) {
  bf192_t x0;
  bf576_t x1;
  bf576_t u;
  bf192_load(&x0, x);
  bf576_load(&x1, x + BF192_NUM_BYTES);
  bf576_load(&u, uhash);

  bf576_mul_192_inplace(&u, &x0);
  bf576_add_inplace(&u, &x1);
  bf576_store(h, &u);
}

void leaf_hash_256(uint8_t* h, const uint8_t* uhash, const uint8_t* x) {
  bf256_t x0;
  bf768_t x1;
  bf768_t u;
  bf256_load(&x0, x);
  bf768_load(&x1, x + BF256_NUM_BYTES);
  bf768_load(&u, uhash);

  bf768_mul_256_inplace(&u, &x0);
  bf768_add_inplace(&u, &x1);
  bf768_store(h, &u);
}
