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
static void N_SIGN(mzd_local_t const* p, view_t* views, in_out_shares_t* in_out_shares,
                   rvec_t* rvec, recorded_state_t* recorded_state) {
#define reduced_shares (SC_PROOF - 1)
#define MPC_LOOP_CONST_C(function, result, first, second, sc, c)                                   \
  MPC_LOOP_CONST_C_0(function, result, first, second, sc)
#define RECOVER_FROM_STATE(x, i)                                                                   \
  XOR((x)[SC_PROOF - 1], (x)[0], (x)[1]);                                                          \
  XOR((x)[SC_PROOF - 1], (x)[SC_PROOF - 1], recorded_state[i].state)
#define ch 0
#define shares SC_PROOF
#if defined(LOWMC_PARTIAL)
#define sbox mpc_sbox_prove_uint64_10
#else
#define sbox CONCAT(mpc_sbox_prove, CONCAT(IMPL, LOWMC_INSTANCE))
#endif

  mzd_local_t x[SC_PROOF][((LOWMC_N) + 255) / 256];
  mzd_local_t y[SC_PROOF][((LOWMC_N) + 255) / 256];

  MPC_LOOP_CONST(MUL, x, in_out_shares->s, LOWMC_INSTANCE.k0_matrix, reduced_shares);
  MPC_LOOP_CONST_C(XOR, x, x, p, reduced_shares, ch);

#if defined(LOWMC_PARTIAL)
  #include "mpc_lowmc_loop_partial.c.i"
#else
  #include "mpc_lowmc_loop.c.i"
#endif

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
#if defined(LOWMC_PARTIAL)
#define sbox mpc_sbox_verify_uint64_10
#else
#define sbox CONCAT(mpc_sbox_verify, CONCAT(IMPL, LOWMC_INSTANCE))
#endif

  mzd_local_t x[SC_VERIFY][((LOWMC_N) + 255) / 256];
  mzd_local_t y[SC_VERIFY][((LOWMC_N) + 255) / 256];

  MPC_LOOP_CONST(MUL, x, in_out_shares->s, LOWMC_INSTANCE.k0_matrix, SC_VERIFY);
  MPC_LOOP_CONST_C(XOR, x, x, p, SC_VERIFY, ch);

#if defined(LOWMC_PARTIAL)
  #include "mpc_lowmc_loop_partial.c.i"
#else
  #include "mpc_lowmc_loop.c.i"
#endif

  MPC_LOOP_SHARED_1(COPY, in_out_shares->s, x, SC_VERIFY);

#undef sbox
#undef reduced_shares
#undef shares
#undef MPC_LOOP_CONST_C
}

#undef loop_impl
#undef N_SIGN
#undef N_VERIFY
#undef LOWMC_M

// vim: ft=c
