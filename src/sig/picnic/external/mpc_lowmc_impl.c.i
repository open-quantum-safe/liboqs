/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#define RANDTAPE R_uint64
#define SBOX SBOX_uint64

#if defined(M_FIXED_10)
#undef SBOX_SIGN
#undef SBOX_VERIFY

#define LOWMC_M 10
#define SBOX_SIGN mpc_sbox_layer_bitsliced_uint64_10
#define SBOX_VERIFY mpc_sbox_layer_bitsliced_verify_uint64_10
#elif defined(M_FIXED_1)
#define LOWMC_M 1
#undef SBOX_SIGN
#undef SBOX_VERIFY

#define SBOX_SIGN mpc_sbox_layer_bitsliced_uint64_1
#define SBOX_VERIFY mpc_sbox_layer_bitsliced_verify_uint64_1
#endif

#if defined(FN_ATTR)
FN_ATTR
#endif
static void N_SIGN(mpc_lowmc_key_t const* lowmc_key, mzd_local_t const* p, view_t* views,
                   in_out_shares_t* in_out_shares, rvec_t* rvec, recorded_state_t* recorded_state) {
#define reduced_shares (SC_PROOF - 1)
#define MPC_LOOP_CONST_C(function, result, first, second, sc, c)                                   \
  MPC_LOOP_CONST_C_0(function, result, first, second, sc)
#define RECOVER_FROM_STATE(x, i)                                                                   \
  XOR((x)[SC_PROOF - 1], (x)[0], (x)[1]);                                                          \
  XOR((x)[SC_PROOF - 1], (x)[SC_PROOF - 1], recorded_state->state[i])
#define ch 0
#define shares SC_PROOF
#define sbox SBOX_SIGN

  MPC_LOOP_SHARED_1(COPY, in_out_shares->s, lowmc_key, SC_PROOF);
  ++in_out_shares;

  mzd_local_t x[SC_PROOF][((LOWMC_N) + 255) / 256];
  mzd_local_t y[SC_PROOF][((LOWMC_N) + 255) / 256];

  MPC_LOOP_CONST(MUL, x, lowmc_key, CONCAT(LOWMC_INSTANCE.k0, matrix_postfix), reduced_shares);
  MPC_LOOP_CONST_C(XOR, x, x, p, reduced_shares, ch);

  #include "mpc_lowmc_loop.c.i"

  MPC_LOOP_SHARED_1(COPY, in_out_shares->s, x, SC_PROOF);

#undef reduced_shares
#undef RECOVER_FROM_STATE
#undef ch
#undef shares
#undef sbox
#undef MPC_LOOP_CONST_C
}

#if defined(FN_ATTR)
FN_ATTR
#endif
static void N_VERIFY(mzd_local_t const* p, view_t* views, in_out_shares_t* in_out_shares,
                     rvec_t* rvec, unsigned int ch) {
#define MPC_LOOP_CONST_C(function, result, first, second, sc, c)                                   \
  MPC_LOOP_CONST_C_ch(function, result, first, second, sc, c)

#define shares SC_VERIFY
#define reduced_shares shares
#define sbox SBOX_VERIFY

  mzd_local_t* const* lowmc_key = &in_out_shares->s[0];
  ++in_out_shares;

  mzd_local_t x[SC_VERIFY][((LOWMC_N) + 255) / 256];
  mzd_local_t y[SC_VERIFY][((LOWMC_N) + 255) / 256];

  MPC_LOOP_CONST(MUL, x, lowmc_key, CONCAT(LOWMC_INSTANCE.k0, matrix_postfix), SC_VERIFY);
  MPC_LOOP_CONST_C(XOR, x, x, p, SC_VERIFY, ch);

#include "mpc_lowmc_loop.c.i"

  MPC_LOOP_SHARED_1(COPY, in_out_shares->s, x, SC_VERIFY);

#undef sbox
#undef reduced_shares
#undef shares
#undef MPC_LOOP_CONST_C
}

#undef SBOX_SIGN
#undef SBOX_VERIFY
#undef SBOX_ARGS
#undef loop_impl
#undef N_SIGN
#undef N_VERIFY
#undef RANDTAPE
#undef SBOX
#undef LOWMC_M

// vim: ft=c
