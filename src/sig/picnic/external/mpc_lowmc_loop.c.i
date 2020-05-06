/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

lowmc_round_t const* round = LOWMC_INSTANCE.rounds;
#if defined(REDUCED_ROUND_KEY_COMPUTATION)
  mzd_local_t nl_part[reduced_shares][(LOWMC_R * 32 + 255) / 256];
#if defined(OPTIMIZED_LINEAR_LAYER_EVALUATION)
  MPC_LOOP_CONST_C(XOR, x, x, LOWMC_INSTANCE.precomputed_constant_linear, reduced_shares, ch);
  MPC_LOOP_CONST(MUL_MC, nl_part, lowmc_key,
                 LOWMC_INSTANCE.precomputed_non_linear_part_matrix, reduced_shares);
  MPC_LOOP_CONST_C(XOR_MC, nl_part, nl_part, LOWMC_INSTANCE.precomputed_constant_non_linear, reduced_shares, ch);
  for (unsigned i = 0; i < (LOWMC_R-1); ++i, ++views, ++round) {
    RANDTAPE;
#if defined(RECOVER_FROM_STATE)
    RECOVER_FROM_STATE(x, i);
#endif
    SBOX(sbox, y, x, views, r, LOWMC_N, shares, reduced_shares);
    for (unsigned int k = 0; k < reduced_shares; ++k) {
      const word nl = CONST_BLOCK(nl_part[k], i >> 3)->w64[(i & 0x7) >> 1];
      BLOCK(y[k], 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^=
        (i & 1) ? (nl & WORD_C(0xFFFFFFFF00000000)) : (nl << 32);
    }
    MPC_LOOP_CONST(MUL_Z, x, y, round->z_matrix, reduced_shares);

    for(unsigned int k = 0; k < reduced_shares; ++k) {
      SHUFFLE(y[k], round->r_mask);
    }

    MPC_LOOP_CONST(ADDMUL_R, x, y, round->r_matrix, reduced_shares);
    for(unsigned int k = 0; k < reduced_shares; ++k) {
      BLOCK(y[k], 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] &= WORD_C(0x00000003FFFFFFFF); //clear nl part
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
    const word nl = CONST_BLOCK(nl_part[k], i >> 3)->w64[(i & 0x7) >> 1];
    BLOCK(y[k], 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^=
      (i & 1) ? (nl & WORD_C(0xFFFFFFFF00000000)) : (nl << 32);
  }
  MPC_LOOP_CONST(MUL, x, y, LOWMC_INSTANCE.zr_matrix, reduced_shares);
#else
  MPC_LOOP_CONST_C(XOR, x, x, LOWMC_INSTANCE.precomputed_constant_linear, reduced_shares, ch);
  MPC_LOOP_CONST(MUL_MC, nl_part, lowmc_key,
                 LOWMC_INSTANCE.precomputed_non_linear_part_matrix, reduced_shares);
  MPC_LOOP_CONST_C(XOR_MC, nl_part, nl_part, LOWMC_INSTANCE.precomputed_constant_non_linear, reduced_shares, ch);
  for (unsigned i = 0; i < (LOWMC_R); ++i, ++views, ++round) {
    RANDTAPE;
#if defined(RECOVER_FROM_STATE)
    RECOVER_FROM_STATE(x, i);
#endif
    SBOX(sbox, y, x, views, r, LOWMC_N, shares, reduced_shares);
    for (unsigned int k = 0; k < reduced_shares; ++k) {
      const word nl = CONST_BLOCK(nl_part[k], i >> 3)->w64[(i & 0x7) >> 1];
      BLOCK(y[k], 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^=
        (i & 1) ? (nl & WORD_C(0xFFFFFFFF00000000)) : (nl << 32);
    }
    MPC_LOOP_CONST(MUL, x, y, round->l_matrix, reduced_shares);
  }
#endif
#else
for (unsigned i = 0; i < (LOWMC_R); ++i, ++views, ++round) {
  RANDTAPE;
#if defined(RECOVER_FROM_STATE)
  RECOVER_FROM_STATE(x, i);
#endif
  SBOX(sbox, y, x, views, r, LOWMC_N, shares, reduced_shares);
  MPC_LOOP_CONST(MUL, x, y, round->l_matrix, reduced_shares);
  MPC_LOOP_CONST_C(XOR, x, x, round->constant, reduced_shares, ch);
  MPC_LOOP_CONST(ADDMUL, x, lowmc_key, round->k_matrix, reduced_shares);
}
#endif
#if defined(RECOVER_FROM_STATE)
RECOVER_FROM_STATE(x, LOWMC_R);
#endif

// vim: ft=c
