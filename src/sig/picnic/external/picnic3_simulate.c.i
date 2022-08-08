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
  uint8_t output[MAX_LOWMC_BLOCK_SIZE] = {0};
  mzd_to_char_array(output, state, params->input_output_size);

  /* timingsafe_bcmp is not strictly necessary here. The comparison does not leak
   * any information on the secret key. Also, the result is fine to be leaked as
   * it essentially only conveys the information whether the public key stored in
   * the secret key is consistent. */
  const int ret = picnic_timingsafe_bcmp(output, pubKey, params->input_output_size);
  picnic_declassify(&ret, sizeof(ret));
#if !defined(NDEBUG)
  if (ret) {
    printf("%s: output does not match pubKey\n", __func__);
    printf("pubKey: ");
    print_hex(stdout, pubKey, params->input_output_size);
    printf("\noutput: ");
    print_hex(stdout, output, params->input_output_size);
    printf("\n");
  }
#endif
  return ret;
}
#endif
