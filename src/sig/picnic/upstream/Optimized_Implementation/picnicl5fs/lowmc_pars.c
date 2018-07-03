
#include "lowmc_pars.h"

#include "macros.h"
#include "mzd_additional.h"

#include "lowmc_256_256_38.h"
#include "lowmc_256_256_38.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static mzd_local_t* readMZD_TStructFromFile(FILE* file);

static mask_t* prepare_masks(mask_t* mask, unsigned int n, unsigned int m) {
  mask->x0   = mzd_local_init(1, n);
  mask->x1   = mzd_local_init_ex(1, n, false);
  mask->x2   = mzd_local_init_ex(1, n, false);
  mask->mask = mzd_local_init(1, n);

  const unsigned int bound = n - 3 * m;
  for (unsigned int i = 0; i < bound; ++i) {
    mzd_local_write_bit(mask->mask, 0, i, 1);
  }
  for (unsigned int i = bound; i < n; i += 3) {
    mzd_local_write_bit(mask->x0, 0, i, 1);
  }
  mzd_shift_left(mask->x1, mask->x0, 1);
  mzd_shift_left(mask->x2, mask->x0, 2);

  mask->x0i   = FIRST_ROW(mask->x0)[n / 64 - 1];
  mask->x1i   = FIRST_ROW(mask->x1)[n / 64 - 1];
  mask->x2i   = FIRST_ROW(mask->x2)[n / 64 - 1];
  mask->maski = FIRST_ROW(mask->mask)[n / 64 - 1];

  return mask;
}

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
  lowmc->precomputed_linear_part_matrix =                                                          \
      lowmc_##N##_##K##_##R##_get_precomputed_round_key_matrix_linear_part()

#define LOAD(N, K, R)

#define LOAD_FROM_FIXED_IMPL(N, K, R, PREC)                                                        \
  lowmc->k0_matrix = lowmc_##N##_##K##_##R##_get_round_key(0);                                     \
  for (unsigned int i = 0; i < (R); ++i) {                                                         \
    lowmc->rounds[i].l_matrix = lowmc_##N##_##K##_##R##_get_linear_layer(i);                       \
    lowmc->rounds[i].k_matrix = lowmc_##N##_##K##_##R##_get_round_key(i + 1);                      \
    lowmc->rounds[i].constant = lowmc_##N##_##K##_##R##_get_round_const(i);                        \
  }                                                                                                \
  LOAD##PREC(N, K, R);

#define LOAD_FROM_FIXED(N, K, R) LOAD_FROM_FIXED_IMPL(N, K, R, _OPT)

  if (n == 256 && k == 256 && r == 38) {
    LOAD_FROM_FIXED(256, 256, 38);
    goto precomp;
  }

  lowmc_clear(lowmc);
  return false;

precomp:

#ifdef MUL_M4RI
  lowmc->k0_lookup = mzd_precompute_matrix_lookup(lowmc->k0_matrix);
  lowmc->precomputed_non_linear_part_lookup =
      mzd_precompute_matrix_lookup(lowmc->precomputed_non_linear_part_matrix);
  lowmc->precomputed_linear_part_lookup =
      mzd_precompute_matrix_lookup(lowmc->precomputed_linear_part_matrix);
#endif
#ifdef MUL_M4RI
  for (unsigned int i = 0; i < r; ++i) {
    lowmc->rounds[i].l_lookup = mzd_precompute_matrix_lookup(lowmc->rounds[i].l_matrix);
  }
#endif

  if (!prepare_masks(&lowmc->mask, n, m)) {
    lowmc_clear(lowmc);
    return false;
  }

  return true;
}

bool lowmc_read_file(lowmc_t* lowmc, unsigned int m, unsigned int n, unsigned int r,
                     unsigned int k) {
  if (!lowmc) {
    return false;
  }

  char file_name[4 * 11] = {0};
  if (snprintf(file_name, sizeof(file_name), "%u-%u-%u-%u", m, n, r, k) < 0) {
    return false;
  }

  FILE* file = fopen(file_name, "r+");
  if (file) {
    int ret = 0;
    ret     = fread(&lowmc->m, sizeof(lowmc->m), 1, file);
    ret += fread(&lowmc->n, sizeof(lowmc->n), 1, file);
    ret += fread(&lowmc->r, sizeof(lowmc->r), 1, file);
    ret += fread(&lowmc->k, sizeof(lowmc->k), 1, file);

    if (lowmc->m != m || lowmc->n != n || lowmc->r != r || lowmc->k != k) {
      fclose(file);
      return false;
    }

    lowmc->needs_free = true;
    lowmc->k0_matrix  = readMZD_TStructFromFile(file);
    lowmc->rounds     = calloc(r, sizeof(lowmc_round_t));
    for (unsigned int i = 0; i < lowmc->r; ++i) {
      lowmc->rounds[i].k_matrix = readMZD_TStructFromFile(file);
      lowmc->rounds[i].l_matrix = readMZD_TStructFromFile(file);
      lowmc->rounds[i].constant = readMZD_TStructFromFile(file);
    }
    lowmc->precomputed_linear_part_matrix     = readMZD_TStructFromFile(file);
    lowmc->precomputed_non_linear_part_matrix = readMZD_TStructFromFile(file);

    fclose(file);
  }

  return true;
}

static mzd_local_t* readMZD_TStructFromFile(FILE* file) {
  int ret   = 0;
  int nrows = 0;
  int ncols = 0;
  ret += fread(&(nrows), sizeof(uint32_t), 1, file);
  ret += fread(&(ncols), sizeof(uint32_t), 1, file);

  mzd_local_t* A = mzd_local_init_ex(nrows, ncols, false);
  for (unsigned int i = 0; i < A->nrows; i++) {
    ret += fread(ROW(A, i), A->rowstride * sizeof(word), 1, file);
  }

  return A;
}

void lowmc_clear(lowmc_t* lowmc) {
  for (unsigned int i = 0; i < lowmc->r; ++i) {
#ifdef MUL_M4RI
    mzd_local_free(lowmc->rounds[i].l_lookup);
#endif
    if (lowmc->needs_free) {
      mzd_local_free((mzd_local_t*)lowmc->rounds[i].constant);
      mzd_local_free((mzd_local_t*)lowmc->rounds[i].k_matrix);
      mzd_local_free((mzd_local_t*)lowmc->rounds[i].l_matrix);
    }
  }
  if (lowmc->needs_free) {
    mzd_local_free((mzd_local_t*)lowmc->precomputed_non_linear_part_matrix);
    mzd_local_free((mzd_local_t*)lowmc->precomputed_linear_part_matrix);
  }
#ifdef MUL_M4RI
  mzd_local_free(lowmc->k0_lookup);
  mzd_local_free(lowmc->precomputed_non_linear_part_lookup);
  mzd_local_free(lowmc->precomputed_linear_part_lookup);
#endif
  if (lowmc->needs_free) {
    mzd_local_free((mzd_local_t*)lowmc->k0_matrix);
  }
  free(lowmc->rounds);

  mzd_local_free(lowmc->mask.x0);
  mzd_local_free(lowmc->mask.x1);
  mzd_local_free(lowmc->mask.x2);
  mzd_local_free(lowmc->mask.mask);
}
