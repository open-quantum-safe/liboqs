/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#if defined(OPTIMIZED_LINEAR_LAYER_EVALUATION) && !defined(M_FIXED_1) && !defined(M_FIXED_10)
#error "OLLE is only implemented for 1 or 10 Sboxes"
#endif

// TODO: fix PICNIC2_AUX_COMPUTATION for OFF & ORKC

#if defined(FN_ATTR)
FN_ATTR
#endif
#if defined(PICNIC2_AUX_COMPUTATION)
static void N_LOWMC(lowmc_key_t const* lowmc_key, randomTape_t* tapes) {
#else
#if defined(RECORD_STATE)
static void N_LOWMC(lowmc_key_t const* lowmc_key, mzd_local_t const* p, recorded_state_t* state) {
#else
static mzd_local_t* N_LOWMC(lowmc_key_t const* lowmc_key, mzd_local_t const* p) {
#endif
#endif
  mzd_local_t x[((LOWMC_N) + 255) / 256];
  mzd_local_t y[((LOWMC_N) + 255) / 256];
#if defined(REDUCED_ROUND_KEY_COMPUTATION)
#if defined(M_FIXED_10)
  mzd_local_t nl_part[(LOWMC_R * 32 + 255) / 256];
#elif defined(M_FIXED_1)
  mzd_local_t nl_part[(((LOWMC_R + 20) / 21) * 64 + 255) / 256];
#endif

#if defined(OPTIMIZED_LINEAR_LAYER_EVALUATION) // LOWMC_OPT=OLLE
#if defined(PICNIC2_AUX_COMPUTATION)
  MUL(x, lowmc_key, CONCAT(LOWMC_INSTANCE.k0, matrix_postfix));
  MUL_MC(nl_part, lowmc_key, CONCAT(LOWMC_INSTANCE.precomputed_non_linear_part, matrix_postfix));
#else
  XOR(x, p, LOWMC_INSTANCE.precomputed_constant_linear);
  ADDMUL(x, lowmc_key, CONCAT(LOWMC_INSTANCE.k0, matrix_postfix));
  MUL_MC(nl_part, lowmc_key, CONCAT(LOWMC_INSTANCE.precomputed_non_linear_part, matrix_postfix));
  XOR_MC(nl_part, nl_part, LOWMC_INSTANCE.precomputed_constant_non_linear);
#endif

  // multiply non-linear part of state with Z0 matrix

  lowmc_round_t const* round = LOWMC_INSTANCE.rounds;
  for (unsigned i = 0; i < LOWMC_R - 1; ++i, ++round) {
#if defined(RECORD_STATE)
    COPY(state->state[i], x);
#endif
#if defined(PICNIC2_AUX_COMPUTATION)
    SBOX(x, tapes);
#else
    SBOX(x);
#endif

#if defined(M_FIXED_10)
    const word nl = CONST_BLOCK(nl_part, i >> 3)->w64[(i & 0x7) >> 1];
    BLOCK(x, 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^=
        (nl << (1 - (i & 1)) * 32) & WORD_C(0xFFFFFFFF00000000);
#elif defined(M_FIXED_1)
    const word nl = CONST_BLOCK(nl_part, i / (4 * 21))->w64[(i % (4 * 21)) / 21];
    BLOCK(x, 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^=
        (nl << ((20 - (i % 21)) * 3)) & WORD_C(0xE000000000000000);
#endif

    MUL_Z(y, x, round->z_matrix);
    MZD_SHUFFLE(x, round->r_mask);
    ADDMUL_R(y, x, round->r_matrix);

#if defined(M_FIXED_10)
    BLOCK(x, 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] &=
        WORD_C(0x00000003FFFFFFFF); // clear nl part
#elif defined(M_FIXED_1)
    BLOCK(x, 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] &=
        WORD_C(0x1FFFFFFFFFFFFFFF); // clear nl part
#endif
    XOR(x, y, x);
  }
#if defined(RECORD_STATE)
  COPY(state->state[LOWMC_R - 1], x);
#endif
#if defined(PICNIC2_AUX_COMPUTATION)
  SBOX(x, tapes);
#else
  SBOX(x);

  unsigned int i = (LOWMC_R - 1);
#if defined(M_FIXED_10)
  const word nl  = CONST_BLOCK(nl_part, i >> 3)->w64[(i & 0x7) >> 1];
  BLOCK(x, 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^=
      (nl << (1 - (i & 1)) * 32) & WORD_C(0xFFFFFFFF00000000);
#elif defined(M_FIXED_1)
  const word nl = CONST_BLOCK(nl_part, i / (4 * 21))->w64[(i % (4 * 21)) / 21];
  BLOCK(x, 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^=
      (nl << ((20 - (i % 21)) * 3)) & WORD_C(0xE000000000000000);
#endif
  MUL(y, x, CONCAT(LOWMC_INSTANCE.zr, matrix_postfix));
  COPY(x, y);
#endif
#else // LOWMC_OPT=ORKC
#if defined(PICNIC2_AUX_COMPUTATION)
  MUL(x, lowmc_key, CONCAT(LOWMC_INSTANCE.k0, matrix_postfix));
  MUL_MC(nl_part, lowmc_key, CONCAT(LOWMC_INSTANCE.precomputed_non_linear_part, matrix_postfix));
#else
  XOR(x, p, LOWMC_INSTANCE.precomputed_constant_linear);
  ADDMUL(x, lowmc_key, CONCAT(LOWMC_INSTANCE.k0, matrix_postfix));
  MUL_MC(nl_part, lowmc_key, CONCAT(LOWMC_INSTANCE.precomputed_non_linear_part, matrix_postfix));
  XOR_MC(nl_part, nl_part, LOWMC_INSTANCE.precomputed_constant_non_linear);
#endif

  lowmc_round_t const* round = LOWMC_INSTANCE.rounds;
  for (unsigned i = 0; i < LOWMC_R; ++i, ++round) {
#if defined(RECORD_STATE)
    COPY(state->state[i], x);
#endif
#if defined(PICNIC2_AUX_COMPUTATION)
    SBOX(x, tapes);
#else
    SBOX(x);
#endif

#if defined(M_FIXED_10)
    const word nl = CONST_BLOCK(nl_part, i >> 3)->w64[(i & 0x7) >> 1];
    BLOCK(x, 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^=
        (i & 1) ? (nl & WORD_C(0xFFFFFFFF00000000)) : (nl << 32);
#elif defined(M_FIXED_1)
    const word nl = CONST_BLOCK(nl_part, i / (4 * 21))->w64[(i % (4 * 21)) / 21];
    BLOCK(x, 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^=
        (nl << ((20 - (i % 21)) * 3)) & WORD_C(0xE000000000000000);
#endif
    MUL(y, x, CONCAT(round->l, matrix_postfix));
    COPY(x, y);
  }
#endif
#else // LOWMC_OPT=OFF
#if defined(PICNIC2_AUX_COMPUTATION)
  MUL(x, lowmc_key, CONCAT(LOWMC_INSTANCE.k0, matrix_postfix));
#else
  COPY(x, p);
  ADDMUL(x, lowmc_key, CONCAT(LOWMC_INSTANCE.k0, matrix_postfix));
#endif

  lowmc_round_t const* round = LOWMC_INSTANCE.rounds;
  for (unsigned i = 0; i < LOWMC_R; ++i, ++round) {
#if defined(RECORD_STATE)
    COPY(state->state[i], x);
#endif
#if defined(PICNIC2_AUX_COMPUTATION)
    SBOX(x, tapes);
#else
    SBOX(x);
#endif

    MUL(y, x, CONCAT(round->l, matrix_postfix));
#if !defined(PICNIC2_AUX_COMPUTATION)
    XOR(x, y, round->constant);
#else
    COPY(x, y);
#endif
    ADDMUL(x, lowmc_key, CONCAT(round->k, matrix_postfix));
  }
#endif

#if !defined(PICNIC2_AUX_COMPUTATION)
#if defined(RECORD_STATE)
  COPY(state->state[LOWMC_R], x);
#else
  mzd_local_t* res = mzd_local_init_ex(1, LOWMC_N, false);
  COPY(res, x);
  return res;
#endif
#endif
}

// vim: ft=c
