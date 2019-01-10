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

lowmc_round_t const* round = LOWMC_INSTANCE.rounds;
#if defined(REDUCED_ROUND_KEY_COMPUTATION)
#if defined(M_FIXED_10)
  mzd_local_t nl_part[reduced_shares][(LOWMC_R * 32 + 255) / 256];
#elif defined(M_FIXED_1)
  mzd_local_t nl_part[reduced_shares][(((LOWMC_R + 20) / 21) * 64 + 255) / 256];
#endif
#if defined(OPTIMIZED_LINEAR_LAYER_EVALUATION)
  MPC_LOOP_CONST_C(XOR, x, x, LOWMC_INSTANCE.precomputed_constant_linear, reduced_shares, ch);
  MPC_LOOP_CONST(MUL_MC, nl_part, lowmc_key,
                 CONCAT(LOWMC_INSTANCE.precomputed_non_linear_part, matrix_postfix), reduced_shares);
  MPC_LOOP_CONST_C(XOR_MC, nl_part, nl_part, LOWMC_INSTANCE.precomputed_constant_non_linear, reduced_shares, ch);
  for (unsigned i = 0; i < (LOWMC_R-1); ++i, ++views, ++round) {
    RANDTAPE;
#if defined(RECOVER_FROM_STATE)
    RECOVER_FROM_STATE(x, i);
#endif
    SBOX(sbox, y, x, views, r, LOWMC_N, shares, reduced_shares);
    for (unsigned int k = 0; k < reduced_shares; ++k) {
#if defined(M_FIXED_10)
      const word nl = CONST_BLOCK(nl_part[k], i >> 3)->w64[(i & 0x7) >> 1];
      BLOCK(y[k], 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^=
        (i & 1) ? (nl & WORD_C(0xFFFFFFFF00000000)) : (nl << 32);
#elif defined(M_FIXED_1)
      const word nl = CONST_BLOCK(nl_part[k], i / (4 * 21))->w64[(i % (4 * 21)) / 21];
      BLOCK(y[k], 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^= (nl << ((20-(i%21))*3)) & WORD_C(0xE000000000000000);
#endif
    }
    MPC_LOOP_CONST(MUL_Z, x, y, CONCAT(round->z, matrix_postfix), reduced_shares);

    for(unsigned int k = 0; k < reduced_shares; ++k) {
      MZD_SHUFFLE(y[k], round->r_mask);
    }

    MPC_LOOP_CONST(MUL_R, x, y, CONCAT(round->r, matrix_postfix), reduced_shares);
    for(unsigned int k = 0; k < reduced_shares; ++k) {
#if defined(M_FIXED_10)
      BLOCK(y[k], 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] &= WORD_C(0x00000003FFFFFFFF); //clear nl part
#elif defined(M_FIXED_1)
      BLOCK(y[k], 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] &= WORD_C(0x1FFFFFFFFFFFFFFF); //clear nl part
#endif
    }
    MPC_LOOP_SHARED(XOR, x, x, y, reduced_shares);
  }
  unsigned i = (LOWMC_R-1);
  RANDTAPE;
#if defined(RECOVER_FROM_STATE)
  RECOVER_FROM_STATE(x, i);
#endif
  SBOX(sbox, y, x, views, r, LOWMC_N, shares, reduced_shares);

  for (unsigned int k = 0; k < reduced_shares; ++k) {
#if defined(M_FIXED_10)
    const word nl = CONST_BLOCK(nl_part[k], i >> 3)->w64[(i & 0x7) >> 1];
    BLOCK(y[k], 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^=
      (i & 1) ? (nl & WORD_C(0xFFFFFFFF00000000)) : (nl << 32);
#elif defined(M_FIXED_1)
    const word nl = CONST_BLOCK(nl_part[k], i / (4 * 21))->w64[(i % (4 * 21)) / 21];
    BLOCK(y[k], 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^= (nl << ((20-(i%21))*3)) & WORD_C(0xE000000000000000);
#endif
  }
  MPC_LOOP_CONST(MUL, x, y, CONCAT(LOWMC_INSTANCE.zr, matrix_postfix), reduced_shares);
#else
  MPC_LOOP_CONST_C(XOR, x, x, LOWMC_INSTANCE.precomputed_constant_linear, reduced_shares, ch);
  MPC_LOOP_CONST(MUL_MC, nl_part, lowmc_key,
                 CONCAT(LOWMC_INSTANCE.precomputed_non_linear_part, matrix_postfix), reduced_shares);
  MPC_LOOP_CONST_C(XOR_MC, nl_part, nl_part, LOWMC_INSTANCE.precomputed_constant_non_linear, reduced_shares, ch);
  for (unsigned i = 0; i < (LOWMC_R); ++i, ++views, ++round) {
    RANDTAPE;
#if defined(RECOVER_FROM_STATE)
    RECOVER_FROM_STATE(x, i);
#endif
    SBOX(sbox, y, x, views, r, LOWMC_N, shares, reduced_shares);
    for (unsigned int k = 0; k < reduced_shares; ++k) {
#if defined(M_FIXED_10)
      const word nl = CONST_BLOCK(nl_part[k], i >> 3)->w64[(i & 0x7) >> 1];
      BLOCK(y[k], 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^=
        (i & 1) ? (nl & WORD_C(0xFFFFFFFF00000000)) : (nl << 32);
#elif defined(M_FIXED_1)
      const word nl = CONST_BLOCK(nl_part[k], i / (4 * 21))->w64[(i % (4 * 21)) / 21];
      BLOCK(y[k], 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^= (nl << ((20-(i%21))*3)) & WORD_C(0xE000000000000000);
#endif
    }
    MPC_LOOP_CONST(MUL, x, y, CONCAT(round->l, matrix_postfix), reduced_shares);
  }
#endif
#else
for (unsigned i = 0; i < (LOWMC_R); ++i, ++views, ++round) {
  RANDTAPE;
#if defined(RECOVER_FROM_STATE)
  RECOVER_FROM_STATE(x, i);
#endif
  SBOX(sbox, y, x, views, r, LOWMC_N, shares, reduced_shares);
  MPC_LOOP_CONST(MUL, x, y, CONCAT(round->l, matrix_postfix), reduced_shares);
  MPC_LOOP_CONST_C(XOR, x, x, round->constant, reduced_shares, ch);
  MPC_LOOP_CONST(ADDMUL, x, lowmc_key, CONCAT(round->k, matrix_postfix), reduced_shares);
}
#endif
#if defined(RECOVER_FROM_STATE)
RECOVER_FROM_STATE(x, LOWMC_R);
#endif

// vim: ft=c
