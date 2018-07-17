/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */


#include "lowmc_pars.h"

#include "macros.h"
#include "mzd_additional.h"

#include "lowmc_128_128_20.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


bool lowmc_init(lowmc_t* lowmc, unsigned int m, unsigned int n, unsigned int r, unsigned int k) {
  if (!lowmc) {
    return false;
  }

  if (n - 3 * m < 2) {
    return false;
  }

  lowmc->m          = m;
  lowmc->n          = n;
  lowmc->r          = r;
  lowmc->k          = k;
  lowmc->needs_free = false;

  lowmc->rounds = calloc(sizeof(lowmc_round_t), r);

#define LOAD_OPT(N, K, R)                                                                          \
  lowmc->precomputed_non_linear_part_matrix =                                                      \
      lowmc_##N##_##K##_##R##_get_precomputed_round_key_matrix_non_linear_part();                  \
  lowmc->k0_matrix = lowmc_##N##_##K##_##R##_get_precomputed_round_key_matrix_linear_part()

#define LOAD(N, K, R)                                                                              \
  lowmc->k0_matrix = lowmc_##N##_##K##_##R##_get_round_key(0);                                     \
  for (unsigned int i = 0; i < (R); ++i) {                                                         \
    lowmc->rounds[i].k_matrix = lowmc_##N##_##K##_##R##_get_round_key(i + 1);                      \
  }

#define LOAD_FROM_FIXED_IMPL(N, K, R, PREC)                                                        \
  for (unsigned int i = 0; i < (R); ++i) {                                                         \
    lowmc->rounds[i].l_matrix = lowmc_##N##_##K##_##R##_get_linear_layer(i);                       \
    lowmc->rounds[i].constant = lowmc_##N##_##K##_##R##_get_round_const(i);                        \
  }                                                                                                \
  LOAD##PREC(N, K, R);

#define LOAD_FROM_FIXED(N, K, R) LOAD_FROM_FIXED_IMPL(N, K, R, _OPT)

  if (n == 128 && k == 128 && r == 20) {
    LOAD_FROM_FIXED(128, 128, 20);
    goto precomp;
  }

  lowmc_clear(lowmc);
  return false;

precomp:

  lowmc->k0_lookup = mzd_precompute_matrix_lookup(lowmc->k0_matrix);
  lowmc->precomputed_non_linear_part_lookup =
      mzd_precompute_matrix_lookup(lowmc->precomputed_non_linear_part_matrix);
  for (unsigned int i = 0; i < r; ++i) {
    lowmc->rounds[i].l_lookup = mzd_precompute_matrix_lookup(lowmc->rounds[i].l_matrix);
  }


  return true;
}


void lowmc_clear(lowmc_t* lowmc) {
  for (unsigned int i = 0; i < lowmc->r; ++i) {
    mzd_local_free(lowmc->rounds[i].l_lookup);
    if (lowmc->needs_free) {
      mzd_local_free((mzd_local_t*)lowmc->rounds[i].constant);
      mzd_local_free((mzd_local_t*)lowmc->rounds[i].l_matrix);
    }
  }
  if (lowmc->needs_free) {
    mzd_local_free((mzd_local_t*)lowmc->precomputed_non_linear_part_matrix);
  }
  mzd_local_free(lowmc->k0_lookup);
  mzd_local_free(lowmc->precomputed_non_linear_part_lookup);
  if (lowmc->needs_free) {
    mzd_local_free((mzd_local_t*)lowmc->k0_matrix);
  }
  free(lowmc->rounds);

}
