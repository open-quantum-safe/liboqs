/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */


#if defined(FN_ATTR)
FN_ATTR
#endif
#if defined(RECORD_STATE)
static void N_LOWMC(lowmc_key_t const* lowmc_key, mzd_local_t const* p, recorded_state_t* state) {
#else
static void N_LOWMC(lowmc_key_t const* lowmc_key, mzd_local_t const* p, mzd_local_t* c) {
#endif
  mzd_local_t x[((LOWMC_N) + 255) / 256];
  mzd_local_t y[((LOWMC_N) + 255) / 256];

  COPY(x, p);
  ADDMUL(x, lowmc_key, LOWMC_INSTANCE.k0_matrix);

  lowmc_round_t const* round = LOWMC_INSTANCE.rounds;
  for (unsigned i = 0; i < LOWMC_R; ++i, ++round) {
#if defined(RECORD_STATE)
    COPY(state[i].state, x);
#endif
    SBOX(x);

    MUL(y, x, round->l_matrix);
    XOR(x, y, round->constant);
    ADDMUL(x, lowmc_key, round->k_matrix);
  }

#if defined(RECORD_STATE)
  COPY(state[LOWMC_R].state, x);
#else
  COPY(c, x);
#endif
}

// vim: ft=c
