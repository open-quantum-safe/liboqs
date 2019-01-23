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

#define copy(d, s) memcpy(BLOCK(d, 0), CONST_BLOCK(s, 0), LOWMC_N / 8)

#if defined(FN_ATTR)
FN_ATTR
#endif
#if defined(RECORD_STATE)
static void N_LOWMC(lowmc_key_t const* lowmc_key, mzd_local_t const* p, recorded_state_t* state) {
#else
static mzd_local_t* N_LOWMC(lowmc_key_t const* lowmc_key, mzd_local_t const* p) {
#endif
#if defined(REDUCED_ROUND_KEY_COMPUTATION)
  mzd_local_t* x       = oqs_sig_picnic_mzd_local_init_ex(1, LOWMC_N, false);
  mzd_local_t* y       = oqs_sig_picnic_mzd_local_init_ex(1, LOWMC_N, false);
#if defined(M_FIXED_10)
  mzd_local_t nl_part[(LOWMC_R * 32 + 255) / 256];
#elif defined(M_FIXED_1)
  mzd_local_t nl_part[(((LOWMC_R + 20) / 21) * 64 + 255) / 256];
#endif

#if defined(OPTIMIZED_LINEAR_LAYER_EVALUATION)
  XOR(x, p, LOWMC_INSTANCE.precomputed_constant_linear);
  ADDMUL(x, lowmc_key, CONCAT(LOWMC_INSTANCE.k0, matrix_postfix));
  MUL_MC(nl_part, lowmc_key, CONCAT(LOWMC_INSTANCE.precomputed_non_linear_part, matrix_postfix));
  XOR_MC(nl_part, nl_part, LOWMC_INSTANCE.precomputed_constant_non_linear);

  //multiply non-linear part of state with Z0 matrix

  lowmc_round_t const* round = LOWMC_INSTANCE.rounds;
  for (unsigned i = 0; i < LOWMC_R-1; ++i, ++round) {
#if defined(RECORD_STATE)
    copy(state->state[i], x);
#endif
    SBOX(x);

#if defined(M_FIXED_10)
    const word nl = CONST_BLOCK(nl_part, i >> 3)->w64[(i & 0x7) >> 1];
    BLOCK(x, 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^=
        (nl << (1 - (i & 1)) * 32) & WORD_C(0xFFFFFFFF00000000);
#elif defined(M_FIXED_1)
    const word nl = CONST_BLOCK(nl_part, i / (4 * 21))->w64[(i % (4 * 21)) / 21];
    BLOCK(x, 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^= (nl << ((20-(i%21))*3)) & WORD_C(0xE000000000000000);
#endif

    MUL_Z(y, x, CONCAT(round->z, matrix_postfix));
    MZD_SHUFFLE(x, round->r_mask);
    MUL_R(y, x, CONCAT(round->r, matrix_postfix));

#if defined(M_FIXED_10)
    BLOCK(x, 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] &= WORD_C(0x00000003FFFFFFFF); //clear nl part
#elif defined(M_FIXED_1)
    BLOCK(x, 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] &= WORD_C(0x1FFFFFFFFFFFFFFF); //clear nl part
#endif
    XOR(x, y, x);
  }
#if defined(RECORD_STATE)
  copy(state->state[LOWMC_R-1], x);
#endif
  SBOX(x);

  unsigned i = (LOWMC_R-1);
#if defined(M_FIXED_10)
  const word nl = CONST_BLOCK(nl_part, i >> 3)->w64[(i & 0x7) >> 1];
  BLOCK(x, 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^=
      (nl << (1 - (i & 1)) * 32) & WORD_C(0xFFFFFFFF00000000);
#elif defined(M_FIXED_1)
  const word nl = CONST_BLOCK(nl_part, i / (4 * 21))->w64[(i % (4 * 21)) / 21];
  BLOCK(x, 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^=
      (nl << ((20 - (i % 21)) * 3)) & WORD_C(0xE000000000000000);
#endif
  MUL(y, x, CONCAT(LOWMC_INSTANCE.zr, matrix_postfix));
  mzd_local_t* t = x;
  x              = y;
  y              = t;
#else
  XOR(x, p, LOWMC_INSTANCE.precomputed_constant_linear);
  ADDMUL(x, lowmc_key, CONCAT(LOWMC_INSTANCE.k0, matrix_postfix));
  MUL_MC(nl_part, lowmc_key, CONCAT(LOWMC_INSTANCE.precomputed_non_linear_part, matrix_postfix));
  XOR_MC(nl_part, nl_part, LOWMC_INSTANCE.precomputed_constant_non_linear);

  lowmc_round_t const* round = LOWMC_INSTANCE.rounds;
  for (unsigned i = 0; i < LOWMC_R; ++i, ++round) {
#if defined(RECORD_STATE)
    copy(state->state[i], x);
#endif
    SBOX(x);

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
    // swap x and y
    mzd_local_t* t = x;
    x              = y;
    y              = t;
  }
#endif
  oqs_sig_picnic_mzd_local_free(y);
#if defined(RECORD_STATE)
  copy(state->state[LOWMC_R], x);
  oqs_sig_picnic_mzd_local_free(x);
#else
  return x;
#endif
#else
  mzd_local_t* x = oqs_sig_picnic_mzd_local_init_ex(1, LOWMC_N, false);
  mzd_local_t* y = oqs_sig_picnic_mzd_local_init_ex(1, LOWMC_N, false);

  copy(x, p);
  ADDMUL(x, lowmc_key, CONCAT(LOWMC_INSTANCE.k0, matrix_postfix));

  lowmc_round_t const* round = LOWMC_INSTANCE.rounds;
  for (unsigned i = 0; i < LOWMC_R; ++i, ++round) {
#if defined(RECORD_STATE)
    copy(state->state[i], x);
#endif
    SBOX(x);

    MUL(y, x, CONCAT(round->l, matrix_postfix));
    XOR(x, y, round->constant);
    ADDMUL(x, lowmc_key, CONCAT(round->k, matrix_postfix));
  }

  oqs_sig_picnic_mzd_local_free(y);
#if defined(RECORD_STATE)
  copy(state->state[LOWMC_R], x);
  oqs_sig_picnic_mzd_local_free(x);
#else
  return x;
#endif
#endif
}

#undef copy

// vim: ft=c
