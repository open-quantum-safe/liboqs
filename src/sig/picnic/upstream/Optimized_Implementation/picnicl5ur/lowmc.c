/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */


#include "lowmc.h"
#include "lowmc_pars.h"
#include "mzd_additional.h"


#if !defined(_MSC_VER)
#include <stdalign.h>
#endif
#include <string.h>

static uint64_t sbox_layer_bitsliced_uint64(uint64_t in) {
  // a, b, c
  const uint64_t x0m = (in & MASK_X0I) << 2;
  const uint64_t x1m = (in & MASK_X1I) << 1;
  const uint64_t x2m = in & MASK_X2I;

  // (b & c) ^ a
  const uint64_t t0 = (x1m & x2m) ^ x0m;
  // (c & a) ^ a ^ b
  const uint64_t t1 = (x0m & x2m) ^ x0m ^ x1m;
  // (a & b) ^ a ^ b ^c
  const uint64_t t2 = (x0m & x1m) ^ x0m ^ x1m ^ x2m;

  return (in & MASK_MASK) ^ (t0 >> 2) ^ (t1 >> 1) ^ t2;
}

/**
 * S-box for m = 10
 */
static void sbox_layer_uint64(mzd_local_t* x, mask_t const* mask) {
  (void)mask;

  uint64_t* d = &FIRST_ROW(x)[x->width - 1];
  *d = sbox_layer_bitsliced_uint64(*d);
}


typedef void (*sbox_layer_impl)(mzd_local_t*, mask_t const*);

static sbox_layer_impl get_sbox_layer(const lowmc_t* lowmc) {
  if (lowmc->m == 10) {
    return sbox_layer_uint64;
  }
  return NULL;
}

static mzd_local_t* lowmc_reduced_linear_layer(lowmc_t const* lowmc, lowmc_key_t const* lowmc_key,
                                               mzd_local_t const* p) {
  mzd_local_t* x       = mzd_local_init_ex(1, lowmc->n, false);
  mzd_local_t* y       = mzd_local_init_ex(1, lowmc->n, false);
  mzd_local_t* nl_part = mzd_local_init_ex(1, lowmc->r * 32, false);

  mzd_local_copy(x, p);
  mzd_addmul_vl(x, lowmc_key, lowmc->k0_lookup);
  mzd_mul_vl(nl_part, lowmc_key, lowmc->precomputed_non_linear_part_lookup);

  lowmc_round_t const* round = lowmc->rounds;
  for (unsigned i = 0; i < lowmc->r; ++i, ++round) {
    sbox_layer_uint64(x, NULL);

    const word mask          = (i & 1) ? WORD_C(0xFFFFFFFF00000000) : WORD_C(0x00000000FFFFFFFF);
    const unsigned int shift = (i & 1) ? 2 : 34;

    FIRST_ROW(x)[x->width - 1] ^= (CONST_FIRST_ROW(nl_part)[i >> 1] & mask) << shift;

    mzd_mul_vl(y, x, round->l_lookup);
    mzd_xor(x, y, round->constant);
  }

  mzd_local_free(y);
  mzd_local_free(nl_part);
  return x;
}

mzd_local_t* lowmc_call(lowmc_t const* lowmc, lowmc_key_t const* lowmc_key, mzd_local_t const* p) {
  sbox_layer_impl sbox_layer = get_sbox_layer(lowmc);
  if (!sbox_layer) {
    return NULL;
  }

  if (lowmc->m == 10) {
    return lowmc_reduced_linear_layer(lowmc, lowmc_key, p);
  }
  return NULL;
}
