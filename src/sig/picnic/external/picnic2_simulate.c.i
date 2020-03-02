/*! @file picnic2_impl.c
 *  @brief This is the main file of the signature scheme for the Picnic2
 *  parameter sets.
 *
 *  This file is part of the reference implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#if !defined(_MSC_VER)
#include <stdalign.h>
#endif

#include "kdf_shake.h"
#include "macros.h"
#include "picnic_impl.h"
#include "picnic2_impl.h"
#include "picnic.h"
#include "picnic2_types.h"
#include "picnic2_tree.h"
#include "io.h"

#if defined(FN_ATTR)
FN_ATTR
#endif
static int SIM_ONLINE(mzd_local_t* maskedKey, shares_t* mask_shares, randomTape_t* tapes, msgs_t* msgs,
                      const mzd_local_t* plaintext, const uint32_t* pubKey,
                      const picnic_instance_t* params) {
  int ret                 = 0;
  mzd_local_t* state = mzd_local_init_ex(1, LOWMC_N, false);
  mzd_local_t* state2 = mzd_local_init_ex(1, LOWMC_N, false);
  mzd_local_t* roundKey = mzd_local_init_ex(1, LOWMC_N, false);
  mzd_local_t* nl_part = mzd_local_init_ex(1, (LOWMC_R * 32), false);
  shares_t* nl_part_masks = allocateShares(LOWMC_R * 32);
  shares_t* key_masks = allocateShares(LOWMC_N); // Make a copy to use when computing each round key
  shares_t* mask2_shares = allocateShares(LOWMC_N);
  uint8_t* unopened_msgs = NULL;

  if (msgs->unopened >= 0) { // We are in verify, save the unopenend parties msgs
    unopened_msgs = malloc(params->view_size + params->input_size);
    memcpy(unopened_msgs, msgs->msgs[msgs->unopened], params->view_size + params->input_size);
  }

  copyShares(key_masks, mask_shares);

#if defined(REDUCED_ROUND_KEY_COMPUTATION)
  MPC_MUL(state, maskedKey, LOWMC_INSTANCE.k0_matrix,
          mask_shares);                                    // roundKey = maskedKey * KMatrix[0]

  XOR(state, state, plaintext);
  XOR(state, state, LOWMC_INSTANCE.precomputed_constant_linear);
  MPC_MUL_MC(nl_part, maskedKey, LOWMC_INSTANCE.precomputed_non_linear_part_matrix,
             LOWMC_INSTANCE.precomputed_constant_non_linear, nl_part_masks, key_masks);
#if defined(OPTIMIZED_LINEAR_LAYER_EVALUATION)
  for (uint32_t r = 0; r < LOWMC_R - 1; r++) {
    mpc_sbox(state, mask_shares, tapes, msgs, unopened_msgs, params);
    mpc_xor_masks_nl(mask_shares, mask_shares, nl_part_masks, r*32 + 2, 30);
    const word nl = CONST_BLOCK(nl_part, r >> 3)->w64[(r & 0x7) >> 1];
    BLOCK(state, 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^=
        (nl << (1 - (r & 1)) * 32) & WORD_C(0xFFFFFFFF00000000);

    MPC_MUL_Z(state2, state, mask2_shares, mask_shares, LOWMC_INSTANCE.rounds[r].z_matrix);
    mpc_shuffle(state, mask_shares, LOWMC_INSTANCE.rounds[r].r_mask);
    MPC_ADDMUL_R(state2, state, mask2_shares, mask_shares, LOWMC_INSTANCE.rounds[r].r_matrix);
    for (uint32_t i = 0; i < 30; i++) {
      mask_shares->shares[i] = 0;
    }
    BLOCK(state, 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] &=
        WORD_C(0x00000003FFFFFFFF); // clear nl part
    XOR(state, state, state2);
    mpc_xor_masks(mask_shares, mask_shares, mask2_shares);
  }
  mpc_sbox(state, mask_shares, tapes, msgs, unopened_msgs, params);
  mpc_xor_masks_nl(mask_shares, mask_shares, nl_part_masks, (LOWMC_R-1)*32 + 2, 30);
  const word nl = CONST_BLOCK(nl_part, (LOWMC_R-1) >> 3)->w64[((LOWMC_R-1) & 0x7) >> 1];
  BLOCK(state, 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^=
        (nl << (1 - ((LOWMC_R-1) & 1)) * 32) & WORD_C(0xFFFFFFFF00000000);
  MPC_MUL(state, state, LOWMC_INSTANCE.zr_matrix,
          mask_shares); // state = state * LMatrix (r-1)
#else
  for (uint32_t r = 0; r < LOWMC_R; r++) {
    mpc_sbox(state, mask_shares, tapes, msgs, unopened_msgs, params);
    mpc_xor_masks_nl(mask_shares, mask_shares, nl_part_masks, r*32 + 2, 30);
    const word nl = CONST_BLOCK(nl_part, r >> 3)->w64[(r & 0x7) >> 1];
    BLOCK(state, 0)->w64[(LOWMC_N) / (sizeof(word) * 8) - 1] ^=
        (nl << (1 - (r & 1)) * 32) & WORD_C(0xFFFFFFFF00000000);
    MPC_MUL(state, state, LOWMC_INSTANCE.rounds[r].l_matrix,
            mask_shares); // state = state * LMatrix (r-1)
  }
#endif
#else
  MPC_MUL(roundKey, maskedKey, LOWMC_INSTANCE.k0_matrix,
          mask_shares);                                       // roundKey = maskedKey * KMatrix[0]
  XOR(state, roundKey, plaintext);

  shares_t* round_key_masks = allocateShares(mask_shares->numWords);
  for (uint32_t r = 0; r < LOWMC_R; r++) {
    copyShares(round_key_masks, key_masks);
    MPC_MUL(roundKey, maskedKey, LOWMC_INSTANCE.rounds[r].k_matrix, round_key_masks);

    mpc_sbox(state, mask_shares, tapes, msgs, unopened_msgs, params);
    MPC_MUL(state, state, LOWMC_INSTANCE.rounds[r].l_matrix,
            mask_shares); // state = state * LMatrix (r-1)
    XOR(state, state, LOWMC_INSTANCE.rounds[r].constant);
    XOR(state, state, roundKey);
    mpc_xor_masks(mask_shares, mask_shares, round_key_masks);
  }
  freeShares(round_key_masks);
#endif

  /* Unmask the output, and check that it's correct */
  if (msgs->unopened >= 0) {
    /* During signature verification we have the shares of the output for
     * the unopened party already in msgs, but not in mask_shares. */
    for (size_t i = 0; i < LOWMC_N; i++) {
      uint8_t share = getBit(unopened_msgs, msgs->pos + i);
      setBit((uint8_t*)&mask_shares->shares[i], msgs->unopened, share);
    }
  }
  uint32_t output[LOWMC_N / 32];
  uint32_t outstate[LOWMC_N / 32];
  mzd_to_char_array((uint8_t*)outstate, state, LOWMC_N/8);
  reconstructShares(output, mask_shares);
  xor_word_array(output, output, outstate, (LOWMC_N / 32));

  if (memcmp(output, pubKey, LOWMC_N / 8) != 0) {
#if !defined(NDEBUG)
    printf("%s: output does not match pubKey\n", __func__);
    printf("pubKey: ");
    print_hex(stdout, (uint8_t*)pubKey, LOWMC_N / 8);
    printf("\noutput: ");
    print_hex(stdout, (uint8_t*)output, LOWMC_N / 8);
    printf("\n");
#endif
    ret = -1;
    goto Exit;
  }

  broadcast(mask_shares, msgs);
  msgsTranspose(msgs);

Exit:
  free(unopened_msgs);
  mzd_local_free(state);
  mzd_local_free(state2);
  mzd_local_free(roundKey);
  mzd_local_free(nl_part);
  freeShares(key_masks);
  freeShares(mask2_shares);
  freeShares(nl_part_masks);

  return ret;
}
