/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */


#include "lowmc_pars.h"
#include "mpc.h"
#include "mpc_lowmc.h"
#include "mzd_additional.h"

#if !defined(_MSC_VER)
#include <stdalign.h>
#endif
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>



#define bitsliced_step_1(sc)                                                                       \
  mpc_and_const(out, in, mask->mask, sc);                                                          \
                                                                                                   \
  mpc_and_const(vars->x0m, in, mask->x0, sc);                                                      \
  mpc_and_const(vars->x1m, in, mask->x1, sc);                                                      \
  mpc_and_const(vars->x2m, in, mask->x2, sc);                                                      \
  mpc_and_const(vars->r0m, rvec, mask->x0, sc);                                                    \
  mpc_and_const(vars->r1m, rvec, mask->x1, sc);                                                    \
  mpc_and_const(vars->r2m, rvec, mask->x2, sc);                                                    \
                                                                                                   \
  mpc_shift_left(vars->x0s, vars->x0m, 2, sc);                                                     \
  mpc_shift_left(vars->r0s, vars->r0m, 2, sc);                                                     \
                                                                                                   \
  mpc_shift_left(vars->x1s, vars->x1m, 1, sc);                                                     \
  mpc_shift_left(vars->r1s, vars->r1m, 1, sc)

#define bitsliced_step_2(sc)                                                                       \
  /* (b & c) ^ a */                                                                                \
  mpc_xor(vars->r2m, vars->r2m, vars->x0s, sc);                                                    \
  /* a ^ b */                                                                                      \
  mpc_xor(vars->x0s, vars->x0s, vars->x1s, sc);                                                    \
  /* (c & a) ^ a ^ b */                                                                            \
  mpc_xor(vars->r1m, vars->r1m, vars->x0s, sc);                                                    \
  /* (a & b) ^ a ^ b ^ c */                                                                        \
  mpc_xor(vars->r0m, vars->r0m, vars->x0s, sc);                                                    \
  mpc_xor(vars->r0m, vars->r0m, vars->x2m, sc);                                                    \
                                                                                                   \
  mpc_shift_right(vars->x0s, vars->r2m, 2, sc);                                                    \
  mpc_shift_right(vars->x1s, vars->r1m, 1, sc);                                                    \
                                                                                                   \
  mpc_xor(out, out, vars->r0m, sc);                                                                \
  mpc_xor(out, out, vars->x0s, sc);                                                                \
  mpc_xor(out, out, vars->x1s, sc)


#define bitsliced_step_1_uint64(sc)                                                                \
  uint64_t r0m[sc];                                                                                \
  uint64_t r0s[sc];                                                                                \
  uint64_t r1m[sc];                                                                                \
  uint64_t r1s[sc];                                                                                \
  uint64_t r2m[sc];                                                                                \
  uint64_t x0s[sc];                                                                                \
  uint64_t x1s[sc];                                                                                \
  uint64_t x2m[sc];                                                                                \
  do {                                                                                             \
    for (unsigned int m = 0; m < (sc); ++m) {                                                      \
      const uint64_t inm   = in[m];                                                                \
      const uint64_t rvecm = rvec[m];                                                              \
                                                                                                   \
      x0s[m] = (inm & MASK_X0I) << 2;                                                              \
      x1s[m] = (inm & MASK_X1I) << 1;                                                              \
      x2m[m] = inm & MASK_X2I;                                                                     \
                                                                                                   \
      r0m[m] = rvecm & MASK_X0I;                                                                   \
      r1m[m] = rvecm & MASK_X1I;                                                                   \
      r2m[m] = rvecm & MASK_X2I;                                                                   \
                                                                                                   \
      r0s[m] = r0m[m] << 2;                                                                        \
      r1s[m] = r1m[m] << 1;                                                                        \
    }                                                                                              \
  } while (0)

#define bitsliced_step_2_uint64(sc)                                                                \
  do {                                                                                             \
    for (unsigned int m = 0; m < sc; ++m) {                                                        \
      const uint64_t tmp1 = r2m[m] ^ x0s[m];                                                       \
      const uint64_t tmp2 = x0s[m] ^ x1s[m];                                                       \
      const uint64_t tmp3 = tmp2 ^ r1m[m];                                                         \
      const uint64_t tmp4 = tmp2 ^ r0m[m] ^ x2m[m];                                                \
                                                                                                   \
      in[m] = (in[m] & MASK_MASK) ^ (tmp4) ^ (tmp1 >> 2) ^ (tmp3 >> 1);                            \
    }                                                                                              \
  } while (0)

static void _mpc_sbox_layer_bitsliced_uint64(uint64_t* in, view_t* view, uint64_t const* rvec) {
  bitsliced_step_1_uint64(SC_PROOF);

  mpc_and_uint64(r0m, x0s, x1s, r2m, view, 0);
  mpc_and_uint64(r2m, x1s, x2m, r1s, view, 1);
  mpc_and_uint64(r1m, x0s, x2m, r0s, view, 2);

  bitsliced_step_2_uint64(SC_PROOF);
}

static void _mpc_sbox_layer_bitsliced_verify_uint64(uint64_t* in, view_t* view,
                                                    uint64_t const* rvec) {
  bitsliced_step_1_uint64(SC_VERIFY);

  mpc_and_verify_uint64(r0m, x0s, x1s, r2m, view, MASK_X2I, 0);
  mpc_and_verify_uint64(r2m, x1s, x2m, r1s, view, MASK_X2I, 1);
  mpc_and_verify_uint64(r1m, x0s, x2m, r0s, view, MASK_X2I, 2);

  bitsliced_step_2_uint64(SC_VERIFY);
}


#define MPC_LOOP MPC_LOOP_CONST
#define MPC_LOOP_TWO_MATRICES MPC_LOOP_SHARED
#define MPC_IF_ELSE MPC_LOOP_CONST_C

#define noscr(const_mat_mul_func, add_func, const_addmat_mul_func, shares)                         \
  MPC_LOOP(const_addmat_mul_func, x, lowmc_key, round->k_lookup, shares);

#define scr(const_mat_mul_func, add_func, const_addmat_mul_func, shares)                           \
  MPC_LOOP(const_mat_mul_func, y, lowmc_key, round->k_matrix, shares);                             \
  MPC_LOOP_TWO_MATRICES(add_func, x, x, y, shares);

#define SBOX(X, sbox, sbox_selector, y, x, views, r, lowmcmask, vars, n, shares)                   \
  SBOX_##sbox_selector(X, sbox, y, x, views, r, lowmcmask, vars, n, shares)

#define SBOX_mzd(X, sbox, y, x, views, r, lowmcmask, vars, n, shares)                              \
  SBOX_mzd_##X(sbox, y, x, views, r, lowmcmask, vars, n)

#define SBOX_mzd_5(sbox, y1, x, views, r, lowmcmask, vars, n) sbox(y1, x, views, r, lowmcmask)
#define SBOX_mzd_6(sbox, y1, x, views, r, lowmcmask, vars, n) sbox(y1, x, views, r, lowmcmask, vars)

#define SBOX_uint64(X, sbox, y, x, views, r, lowmcmask, vars, n, shares)                           \
  SBOX_uint64_##shares(sbox, y, x, views, r, lowmcmask, vars, n)

#define SBOX_uint64_3(sbox, y1, x, views, r, lowmcmask, vars, n)                                   \
  uint64_t in[SC_PROOF];                                                                           \
  for (unsigned int count = 0; count < SC_PROOF; ++count) {                                        \
    in[count] = CONST_FIRST_ROW(x[count])[n / (sizeof(word) * 8) - 1];                             \
  }                                                                                                \
  _mpc_sbox_layer_bitsliced_uint64(in, views, r);                                                  \
  for (unsigned int count = 0; count < SC_PROOF; ++count) {                                        \
    memcpy(FIRST_ROW(y1[count]), CONST_FIRST_ROW(x[count]),                                        \
           (n / (sizeof(word) * 8) - 1) * sizeof(word));                                           \
    FIRST_ROW(y1[count])[n / (sizeof(word) * 8) - 1] = in[count];                                  \
  }

#define SBOX_uint64_2(sbox, y1, x, views, r, lowmcmask, vars, n)                                   \
  uint64_t in[SC_VERIFY];                                                                          \
  for (unsigned int count = 0; count < SC_VERIFY; ++count) {                                       \
    in[count] = CONST_FIRST_ROW(x[count])[n / (sizeof(word) * 8) - 1];                             \
  }                                                                                                \
  _mpc_sbox_layer_bitsliced_verify_uint64(in, views, r);                                           \
  for (unsigned int count = 0; count < SC_VERIFY; ++count) {                                       \
    memcpy(FIRST_ROW(y1[count]), CONST_FIRST_ROW(x[count]),                                        \
           (n / (sizeof(word) * 8) - 1) * sizeof(word));                                           \
    FIRST_ROW(y1[count])[n / (sizeof(word) * 8) - 1] = in[count];                                  \
  }

#define R(selector, shares) R_##selector##_##shares

#define R_mzd_2 mzd_local_t** r = rvec[i].s
#define R_mzd_3 R_mzd_2

#ifdef _MSC_VER
#define R_uint64_2                                                                                 \
  uint64_t r[SC_VERIFY];                                                                           \
  r[0] = rvec[i].t[0];                                                                             \
  r[1] = rvec[i].t[1]
#define R_uint64_3                                                                                 \
  uint64_t r[SC_PROOF];                                                                            \
  r[0] = rvec[i].t[0];                                                                             \
  r[1] = rvec[i].t[1];                                                                             \
  r[2] = rvec[i].t[2]
#else
#define R_uint64_2 uint64_t r[SC_VERIFY] = {rvec[i].t[0], rvec[i].t[1]}
#define R_uint64_3 uint64_t r[SC_PROOF] = {rvec[i].t[0], rvec[i].t[1], rvec[i].t[2]}
#endif

#define loop_optimize(sbox_args, sbox, sbox_selector, no_scr, no_scr_active, const_mat_mul_func,   \
                      add_func, mul_more_cols, const_addmat_mul_func, ch, shares, lowmc_n,         \
                      lowmc_r)                                                                     \
  mzd_local_t* nl_part[shares];                                                                    \
  mzd_local_init_multiple_ex(nl_part, shares, 1, (lowmc_r)*32, false);                             \
  MPC_LOOP(mul_more_cols, nl_part, lowmc_key, lowmc->precomputed_non_linear_part_##no_scr,         \
           shares);                                                                                \
  for (unsigned i = 0; i < (lowmc_r); ++i, ++views, ++round) {                                     \
    R(sbox_selector, shares);                                                                      \
    SBOX(sbox_args, sbox, sbox_selector, y, x, views, r, &lowmc->mask, &vars, lowmc_n, shares);    \
    const word mask          = (i & 1) ? WORD_C(0xFFFFFFFF00000000) : WORD_C(0x00000000FFFFFFFF);  \
    const unsigned int shift = (i & 1) ? 2 : 34;                                                   \
    for (unsigned int k = 0; k < shares; ++k) {                                                    \
      FIRST_ROW(y[k])                                                                              \
      [(lowmc_n) / (sizeof(word) * 8) - 1] ^=                                                      \
          (CONST_FIRST_ROW(nl_part[k])[i >> 1] & mask) << shift;                                   \
    }                                                                                              \
    MPC_LOOP(const_mat_mul_func, x, y, round->l_##no_scr, shares);                                 \
    MPC_IF_ELSE(add_func, x, x, round->constant, shares, ch);                                      \
  }                                                                                                \
  mzd_local_free_multiple(nl_part);

#define loop(sbox_args, sbox, sbox_selector, no_scr, no_scr_active, const_mat_mul_func, add_func,  \
             mul_more_cols, const_addmat_mul_func, ch, shares, lowmc_n, lowmc_r)                   \
  for (unsigned i = 0; i < (lowmc_r); ++i, ++views, ++round) {                                     \
    R(sbox_selector, shares);                                                                      \
    SBOX(sbox_args, sbox, sbox_selector, y, x, views, r, &lowmc->mask, &vars, lowmc_n, shares);    \
    mpc_clear(x, shares);                                                                          \
    MPC_LOOP(const_mat_mul_func, x, y, round->l_##no_scr, shares);                                 \
    MPC_IF_ELSE(add_func, x, x, round->constant, shares, ch);                                      \
    no_scr_active(const_mat_mul_func, add_func, const_addmat_mul_func, shares);                    \
  }

#define VARS_5(shares, n)
#define VARS_6(shares, n)                                                                          \
  sbox_vars_t vars;                                                                                \
  sbox_vars_init(&vars, n, shares)

#define VARS_FREE_5
#define VARS_FREE_6 sbox_vars_clear(&vars)

#define _mpc_lowmc_call_bitsliced(ch, sbox_args, sbox, sbox_selector, no_scr, no_scr_active,       \
                                  optimize, const_mat_mul_func, add_func, mul_more_cols,           \
                                  const_addmat_mul_func, lowmc_n, lowmc_r)                         \
  mpc_copy(in_out_shares->s, lowmc_key, SC_PROOF);                                                 \
  ++in_out_shares;                                                                                 \
  VARS_##sbox_args(SC_PROOF, lowmc_n);                                                             \
  mzd_local_t** x = in_out_shares->s;                                                              \
  mzd_local_t* y[SC_PROOF];                                                                        \
  mzd_local_init_multiple_ex(y, SC_PROOF, 1, lowmc_n, false);                                      \
                                                                                                   \
  MPC_LOOP(const_mat_mul_func, x, lowmc_key, lowmc->k0_##no_scr, SC_PROOF);                        \
  MPC_IF_ELSE(add_func, x, x, p, SC_PROOF, ch);                                                    \
                                                                                                   \
  lowmc_round_t const* round = lowmc->rounds;                                                      \
                                                                                                   \
  loop##optimize(sbox_args, sbox, sbox_selector, no_scr, no_scr_active, const_mat_mul_func,        \
                 add_func, mul_more_cols, const_addmat_mul_func, ch, SC_PROOF, lowmc_n, lowmc_r)   \
      VARS_FREE_##sbox_args;                                                                       \
  mzd_local_free_multiple(y);

#define _mpc_lowmc_call_bitsliced_verify_m(ch, sbox_args, sbox, sbox_selector, no_scr,             \
                                           no_scr_active, optimize, const_mat_mul_func, add_func,  \
                                           mul_more_cols, const_addmat_mul_func, lowmc_n, lowmc_r) \
  mzd_local_t* const* lowmc_key = &in_out_shares->s[0];                                            \
                                                                                                   \
  ++in_out_shares;                                                                                 \
  VARS_##sbox_args(SC_VERIFY, lowmc_n);                                                            \
  mzd_local_t* x[2 * SC_VERIFY];                                                                   \
  mzd_local_t** y = &x[SC_VERIFY];                                                                 \
  mzd_local_init_multiple_ex(x, 2 * SC_VERIFY, 1, lowmc_n, false);                                 \
                                                                                                   \
  MPC_LOOP(const_mat_mul_func, x, lowmc_key, lowmc->k0_##no_scr, SC_VERIFY);                       \
  MPC_IF_ELSE(add_func, x, x, p, SC_VERIFY, ch);                                                   \
                                                                                                   \
  lowmc_round_t const* round = lowmc->rounds;                                                      \
                                                                                                   \
  loop##optimize(sbox_args, sbox, sbox_selector, no_scr, no_scr_active, const_mat_mul_func,        \
                 add_func, mul_more_cols, const_addmat_mul_func, ch, SC_VERIFY, lowmc_n, lowmc_r); \
  mpc_copy(in_out_shares->s, x, SC_VERIFY);                                                        \
  mzd_local_free_multiple(x);                                                                      \
  VARS_FREE_##sbox_args;

#define mpc_lowmc_call_def_gen(N, R, N_SIGN, N_VERIFY, SBOX_SIGN, SBOX_VERIFY, MUL, MUL_L, XOR,    \
                               MUL_MC, MUL_MC_L, ADDMUL, ADDMUL_L, ARGS)                           \
  static inline void N_SIGN(lowmc_t const* lowmc, mpc_lowmc_key_t* lowmc_key,                      \
                            mzd_local_t const* p, view_t* views, in_out_shares_t* in_out_shares,   \
                            rvec_t* rvec) {                                                        \
    _mpc_lowmc_call_bitsliced(0, ARGS, SBOX_SIGN, mzd, lookup, noscr, _optimize, MUL_L, XOR,       \
                              MUL_MC_L, ADDMUL_L, N, R);                                           \
  }                                                                                                \
  static inline void N_VERIFY(lowmc_t const* lowmc, mzd_local_t const* p, view_t* views,           \
                              in_out_shares_t* in_out_shares, rvec_t* rvec, unsigned int ch) {     \
    _mpc_lowmc_call_bitsliced_verify_m(ch, ARGS, SBOX_VERIFY, mzd, lookup, noscr, _optimize,       \
                                       MUL_L, XOR, MUL_MC_L, ADDMUL_L, N, R);                      \
  }

#define mpc_lowmc_call_def_10(N, R, N_SIGN, N_VERIFY, SBOX_SIGN, SBOX_VERIFY, MUL, MUL_L, XOR,     \
                              MUL_MC, MUL_MC_L, ADDMUL, ADDMUL_L)                                  \
  static inline void N_SIGN##_10(lowmc_t const* lowmc, mpc_lowmc_key_t* lowmc_key,                 \
                                 mzd_local_t const* p, view_t* views,                              \
                                 in_out_shares_t* in_out_shares, rvec_t* rvec) {                   \
    _mpc_lowmc_call_bitsliced(0, 5, , uint64, lookup, noscr, _optimize, MUL_L, XOR, MUL_MC_L,      \
                              ADDMUL_L, N, R);                                                     \
  }                                                                                                \
  static inline void N_VERIFY##_10(lowmc_t const* lowmc, mzd_local_t const* p, view_t* views,      \
                                   in_out_shares_t* in_out_shares, rvec_t* rvec,                   \
                                   unsigned int ch) {                                              \
    _mpc_lowmc_call_bitsliced_verify_m(ch, 5, , uint64, lookup, noscr, _optimize, MUL_L, XOR,      \
                                       MUL_MC_L, ADDMUL_L, N, R);                                  \
  }

#define mpc_lowmc_call_def_args(N, R, N_SIGN, N_VERIFY, SBOX_SIGN, SBOX_VERIFY, MUL, MUL_L, XOR,   \
                                MUL_MC, MUL_MC_L, ADDMUL, ADDMUL_L, ARGS)                          \
  mpc_lowmc_call_def_10(N, R, N_SIGN, N_VERIFY, SBOX_SIGN, SBOX_VERIFY, MUL, MUL_L, XOR, MUL_MC,   \
                        MUL_MC_L, ADDMUL, ADDMUL_L);

mpc_lowmc_call_def_args(lowmc->n, lowmc->r, mpc_lowmc_call, mpc_lowmc_call_verify,
                        _mpc_sbox_layer_bitsliced, _mpc_sbox_layer_bitsliced_verify,
                        mzd_mul_v_uint64, mzd_mul_vl_uint64, mzd_xor_uint64, mzd_mul_v_uint64,
                        mzd_mul_vl_uint64, mzd_addmul_v_uint64, mzd_addmul_vl_uint64, 6);



#define general_or_10(l, f) f##_10

lowmc_implementation_f get_lowmc_implementation(const lowmc_t* lowmc) {

  (void)lowmc;
  return general_or_10(lowmc, mpc_lowmc_call);
}

lowmc_verify_implementation_f get_lowmc_verify_implementation(const lowmc_t* lowmc) {

  (void)lowmc;
  return general_or_10(lowmc, mpc_lowmc_call_verify);
}
