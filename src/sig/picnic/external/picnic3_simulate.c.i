/*! @file picnic3_impl.c
 *  @brief This is the main file of the signature scheme for the Picnic3
 *  parameter sets.
 *
 *  This file is part of the reference implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#if defined(LOWMC_INSTANCE)
#if defined(FN_ATTR)
FN_ATTR
#endif
static int SIM_ONLINE(mzd_local_t* maskedKey, randomTape_t* tapes, msgs_t* msgs,
                      const mzd_local_t* plaintext, const uint8_t* pubKey,
                      const picnic_instance_t* params) {

#define mpc_sbox CONCAT(picnic3_mpc_sbox, CONCAT(IMPL, LOWMC_INSTANCE))
  int ret = 0;
  mzd_local_t state[(LOWMC_N + 255) / 256];
  mzd_local_t temp[(LOWMC_N + 255) / 256];

  //  MPC_MUL(temp, maskedKey, LOWMC_INSTANCE.k0_matrix,
  //          mask_shares); // roundKey = maskedKey * KMatrix[0]
  MUL(temp, maskedKey, LOWMC_INSTANCE.k0_matrix);
  XOR(state, temp, plaintext);

  for (uint32_t r = 0; r < LOWMC_R; r++) {
    mpc_sbox(state, tapes, msgs);
    // MPC_MUL(state, state, LOWMC_INSTANCE.rounds[r].l_matrix,
    //        mask_shares); // state = state * LMatrix (r-1)
    MUL(temp, state, LOWMC_INSTANCE.rounds[r].l_matrix);
    XOR(state, temp, LOWMC_INSTANCE.rounds[r].constant);
    ADDMUL(state, maskedKey, LOWMC_INSTANCE.rounds[r].k_matrix);
  }

  /* check that the output is correct */
  uint8_t output[MAX_LOWMC_BLOCK_SIZE];
  mzd_to_char_array(output, state, params->output_size);

  if (timingsafe_bcmp(output, pubKey, params->output_size) != 0) {
#if !defined(NDEBUG)
    printf("%s: output does not match pubKey\n", __func__);
    printf("pubKey: ");
    print_hex(stdout, pubKey, params->output_size);
    printf("\noutput: ");
    print_hex(stdout, output, params->output_size);
    printf("\n");
#endif
    ret = -1;
  }
  return ret;
}
#endif
