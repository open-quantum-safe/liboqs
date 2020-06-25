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
static void N_LOWMC(lowmc_key_t* lowmc_key, randomTape_t* tapes) {
  mzd_local_t x[((LOWMC_N) + 255) / 256] = {{{0, 0, 0, 0}}};
  mzd_local_t y[((LOWMC_N) + 255) / 256];
  mzd_local_t key0[((LOWMC_N) + 255) / 256];

  COPY(key0, lowmc_key);
  MUL(lowmc_key, key0, LOWMC_INSTANCE.ki0_matrix);

  lowmc_round_t const* round = &LOWMC_INSTANCE.rounds[LOWMC_R - 1];
  for (unsigned r = 0; r < LOWMC_R; ++r, round--) {
    ADDMUL(x, lowmc_key, round->k_matrix);
    MUL(y, x, round->li_matrix);

    // recover input masks from tapes, only in first round we use the key as input
    if (r == LOWMC_R - 1) {
      COPY(x, key0);
    } else {
      bitstream_t bs = {{tapes->parity_tapes}, LOWMC_N * 2 * (LOWMC_R - 1 - r)};
      mzd_from_bitstream(&bs, x, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);
    }
    tapes->pos     = LOWMC_N * 2 * (LOWMC_R - 1 - r) + LOWMC_N;
    tapes->aux_pos = LOWMC_N * (LOWMC_R - 1 - r);
    SBOX(x, y, tapes);
  }
}

// vim: ft=c
