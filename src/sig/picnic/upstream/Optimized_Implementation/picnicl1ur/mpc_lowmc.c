
#include "lowmc_pars.h"
#include "mpc.h"
#include "mpc_lowmc.h"
#include "mzd_additional.h"

#include <stdalign.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


static inline word mzd_read_bits(mzd_local_t const* M, uint32_t const x, uint32_t const y,
                                 uint32_t const n) {
  uint32_t const spot  = y % 64;
  uint32_t const block = y / 64;
  int const spill      = spot + n - 64;
  word temp            = (spill <= 0) ? ROW(M, x)[block] << -spill
                           : (ROW(M, x)[block + 1] << (64 - spill)) | (ROW(M, x)[block] >> spill);
  return temp >> (64 - n);
}

typedef struct {
  mzd_local_t* x0m[SC_PROOF]; // a
  mzd_local_t* x1m[SC_PROOF]; // b
  mzd_local_t* x2m[SC_PROOF]; // c
  mzd_local_t* r0m[SC_PROOF];
  mzd_local_t* r1m[SC_PROOF];
  mzd_local_t* r2m[SC_PROOF];
  mzd_local_t* x0s[SC_PROOF];
  mzd_local_t* r0s[SC_PROOF];
  mzd_local_t* x1s[SC_PROOF];
  mzd_local_t* r1s[SC_PROOF];
  mzd_local_t* v[SC_PROOF];

  mzd_local_t** storage;
} sbox_vars_t;

static sbox_vars_t* sbox_vars_init(sbox_vars_t* vars, uint32_t n, unsigned sc);
static void sbox_vars_clear(sbox_vars_t* vars);

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

static void _mpc_sbox_layer_bitsliced(mzd_local_t** out, mzd_local_t* const* in, view_t* view,
                                      mzd_local_t* const* rvec, mask_t const* mask,
                                      sbox_vars_t const* vars) {
  bitsliced_step_1(SC_PROOF);

  mpc_clear(view->s, SC_PROOF);
  // a & b
  mpc_and(vars->r0m, vars->x0s, vars->x1s, vars->r2m, view, 0, vars->v);
  // b & c
  mpc_and(vars->r2m, vars->x1s, vars->x2m, vars->r1s, view, 1, vars->v);
  // c & a
  mpc_and(vars->r1m, vars->x0s, vars->x2m, vars->r0s, view, 2, vars->v);

  bitsliced_step_2(SC_PROOF);
}

static void _mpc_sbox_layer_bitsliced_verify(mzd_local_t** out, mzd_local_t* const* in,
                                             view_t* view, mzd_local_t* const* rvec,
                                             mask_t const* mask, sbox_vars_t const* vars) {
  bitsliced_step_1(SC_VERIFY);

  mzd_local_clear(view->s[0]);
  // a & b
  mpc_and_verify(vars->r0m, vars->x0s, vars->x1s, vars->r2m, view, mask->x2, 0, vars->v);
  // b & c
  mpc_and_verify(vars->r2m, vars->x1s, vars->x2m, vars->r1s, view, mask->x2, 1, vars->v);
  // c & a
  mpc_and_verify(vars->r1m, vars->x0s, vars->x2m, vars->r0s, view, mask->x2, 2, vars->v);

  bitsliced_step_2(SC_VERIFY);
}

#define bitsliced_step_1_uint64(sc)                                                                \
  uint64_t r0m[sc];                                                                                \
  uint64_t r0s[sc];                                                                                \
  uint64_t r1m[sc];                                                                                \
  uint64_t r1s[sc];                                                                                \
  uint64_t r2m[sc];                                                                                \
  uint64_t x0s[sc];                                                                                \
  uint64_t x1s[sc];                                                                                \
  uint64_t x2m[sc];                                                                                \
  const uint64_t mx2 = mask->x2i;                                                                  \
  do {                                                                                             \
    const uint64_t mx0 = mask->x0i;                                                                \
    const uint64_t mx1 = mask->x1i;                                                                \
                                                                                                   \
    for (unsigned int m = 0; m < (sc); ++m) {                                                      \
      const uint64_t inm   = in[m];                                                                \
      const uint64_t rvecm = rvec[m];                                                              \
                                                                                                   \
      x0s[m] = (inm & mx0) << 2;                                                                   \
      x1s[m] = (inm & mx1) << 1;                                                                   \
      x2m[m] = inm & mx2;                                                                          \
                                                                                                   \
      r0m[m] = rvecm & mx0;                                                                        \
      r1m[m] = rvecm & mx1;                                                                        \
      r2m[m] = rvecm & mx2;                                                                        \
                                                                                                   \
      r0s[m] = r0m[m] << 2;                                                                        \
      r1s[m] = r1m[m] << 1;                                                                        \
    }                                                                                              \
  } while (0)

#define bitsliced_step_2_uint64(sc)                                                                \
  do {                                                                                             \
    const uint64_t maskm = mask->maski;                                                            \
    for (unsigned int m = 0; m < sc; ++m) {                                                        \
      const uint64_t inm = in[m];                                                                  \
      uint64_t* outm     = &out[m];                                                                \
                                                                                                   \
      const uint64_t tmp1 = r2m[m] ^ x0s[m];                                                       \
      const uint64_t tmp2 = x0s[m] ^ x1s[m];                                                       \
      const uint64_t tmp3 = tmp2 ^ r1m[m];                                                         \
      const uint64_t tmp4 = tmp2 ^ r0m[m] ^ x2m[m];                                                \
                                                                                                   \
      const uint64_t mout = maskm & inm;                                                           \
      *outm               = mout ^ (tmp4) ^ (tmp1 >> 2) ^ (tmp3 >> 1);                             \
    }                                                                                              \
  } while (0)

static void _mpc_sbox_layer_bitsliced_uint64(uint64_t* out, uint64_t const* in, view_t* view,
                                             uint64_t const* rvec, mask_t const* mask) {
  bitsliced_step_1_uint64(SC_PROOF);

  memset(view->t, 0, sizeof(uint64_t) * SC_PROOF);
  mpc_and_uint64(r0m, x0s, x1s, r2m, view, 0);
  mpc_and_uint64(r2m, x1s, x2m, r1s, view, 1);
  mpc_and_uint64(r1m, x0s, x2m, r0s, view, 2);

  bitsliced_step_2_uint64(SC_PROOF);
}

static void _mpc_sbox_layer_bitsliced_verify_uint64(uint64_t* out, uint64_t const* in, view_t* view,
                                                    uint64_t const* rvec, mask_t const* mask) {
  bitsliced_step_1_uint64(SC_VERIFY);

  view->t[0] = 0;
  mpc_and_verify_uint64(r0m, x0s, x1s, r2m, view, mx2, 0);
  mpc_and_verify_uint64(r2m, x1s, x2m, r1s, view, mx2, 1);
  mpc_and_verify_uint64(r1m, x0s, x2m, r0s, view, mx2, 2);

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

#define SBOX_mzd_5(sbox, y1, x, views, r, lowmcmask, vars, n) sbox(y1, x, views, r, lowmcmask);
#define SBOX_mzd_6(sbox, y1, x, views, r, lowmcmask, vars, n)                                      \
  sbox(y1, x, views, r, lowmcmask, vars);

#define SBOX_uint64(X, sbox, y, x, views, r, lowmcmask, vars, n, shares)                           \
  SBOX_uint64_##shares(sbox, y, x, views, r, lowmcmask, vars, n)

#define SBOX_uint64_3(sbox, y1, x, views, r, lowmcmask, vars, n)                                   \
  uint64_t in[SC_PROOF];                                                                           \
  uint64_t out[SC_PROOF];                                                                          \
  for (int count = 0; count < SC_PROOF; count++) {                                                 \
    in[count]  = FIRST_ROW(x[count])[n / 64 - 1];                                                  \
    out[count] = FIRST_ROW(y1[count])[n / 64 - 1];                                                 \
  }                                                                                                \
  _mpc_sbox_layer_bitsliced_uint64(out, in, views, r, lowmcmask);                                  \
  for (int count = 0; count < SC_PROOF; count++) {                                                 \
    FIRST_ROW(y1[count])[n / 64 - 1] = out[count];                                                 \
    for (unsigned int copy_in = 0; copy_in < n / 64 - 1; copy_in++) {                              \
      FIRST_ROW(y1[count])[copy_in] = FIRST_ROW(x[count])[copy_in];                                \
    }                                                                                              \
  }

#define SBOX_uint64_2(sbox, y1, x, views, r, lowmcmask, vars, n)                                   \
  uint64_t in[SC_VERIFY];                                                                          \
  uint64_t out[SC_VERIFY];                                                                         \
  for (int count = 0; count < SC_VERIFY; count++) {                                                \
    in[count]  = FIRST_ROW(x[count])[n / 64 - 1];                                                  \
    out[count] = FIRST_ROW(y1[count])[n / 64 - 1];                                                 \
  }                                                                                                \
  _mpc_sbox_layer_bitsliced_verify_uint64(out, in, views, r, lowmcmask);                           \
  for (int count = 0; count < SC_VERIFY; count++) {                                                \
    FIRST_ROW(y1[count])[n / 64 - 1] = out[count];                                                 \
    for (unsigned int copy_in = 0; copy_in < n / 64 - 1; copy_in++) {                              \
      FIRST_ROW(y1[count])[copy_in] = FIRST_ROW(x[count])[copy_in];                                \
    }                                                                                              \
  }

#define R(selector, shares) R_##selector##_##shares
#define R_mzd_2 mzd_local_t** r                = rvec[i].s;
#define R_mzd_3 mzd_local_t** r                = rvec[i].s;
#define R_uint64_2 uint64_t const r[SC_VERIFY] = {rvec[i].t[0], rvec[i].t[1]};
#define R_uint64_3 uint64_t const r[SC_PROOF]  = {rvec[i].t[0], rvec[i].t[1], rvec[i].t[2]};

#define loop_optimize(sbox_args, sbox, sbox_selector, no_scr, no_scr_active, const_mat_mul_func,   \
                      add_func, mul_more_cols, const_addmat_mul_func, ch, shares)                  \
  MPC_LOOP(mul_more_cols, y, lowmc_key, lowmc->precomputed_linear_part_##no_scr, shares);          \
  mpc_xor(x, x, y, shares);                                                                        \
  mzd_local_t* nl_part[shares];                                                                    \
  mzd_local_init_multiple_ex(nl_part, shares, 1, lowmc->r*(3 * lowmc->m + 2), false);              \
  MPC_LOOP(mul_more_cols, nl_part, lowmc_key, lowmc->precomputed_non_linear_part_##no_scr,         \
           shares);                                                                                \
  word mask          = 0x00000000FFFFFFFF;                                                         \
  unsigned int shift = 0;                                                                          \
  for (unsigned i = 0; i < lowmc->r; ++i, ++views, ++round) {                                      \
    R(sbox_selector, shares)                                                                       \
    SBOX(sbox_args, sbox, sbox_selector, y, x, views, r, &lowmc->mask, &vars, lowmc->n, shares);   \
    shift = ((mask & 0xFFFFFFFF) ? 34 : 2);                                                        \
    for (unsigned int k = 0; k < shares; k++) {                                                    \
      FIRST_ROW(y[k])                                                                              \
      [y[k]->width - 1] ^=                                                                         \
          (CONST_FIRST_ROW(nl_part[k])[i * (3 * lowmc->m + 2) / 64] & mask) << shift;              \
    }                                                                                              \
    mask = ~mask;                                                                                  \
    MPC_LOOP(const_mat_mul_func, x, y, round->l_##no_scr, shares);                                 \
    MPC_IF_ELSE(add_func, x, x, round->constant, shares, ch);                                      \
  }                                                                                                \
  mzd_local_free_multiple(nl_part);

#define loop(sbox_args, sbox, sbox_selector, no_scr, no_scr_active, const_mat_mul_func, add_func,  \
             mul_more_cols, const_addmat_mul_func, ch, shares)                                     \
  for (unsigned i = 0; i < lowmc->r; ++i, ++views, ++round) {                                      \
    R(sbox_selector, shares)                                                                       \
    SBOX(sbox_args, sbox, sbox_selector, y, x, views, r, &lowmc->mask, &vars, lowmc->n, shares);   \
    mpc_clear(x, shares);                                                                          \
    MPC_LOOP(const_mat_mul_func, x, y, round->l_##no_scr, shares);                                 \
    MPC_IF_ELSE(add_func, x, x, round->constant, shares, ch);                                      \
    no_scr_active(const_mat_mul_func, add_func, const_addmat_mul_func, shares);                    \
  }

#define VARS_5(shares)
#define VARS_6(shares)                                                                             \
  sbox_vars_t vars;                                                                                \
  sbox_vars_init(&vars, lowmc->n, shares)

#define VARS_FREE_5
#define VARS_FREE_6 sbox_vars_clear(&vars)

#define _mpc_lowmc_call_bitsliced(ch, sbox_args, sbox, sbox_selector, no_scr, no_scr_active,       \
                                  optimize, const_mat_mul_func, add_func, mul_more_cols,           \
                                  const_addmat_mul_func)                                           \
  mpc_copy(in_out_shares->s, lowmc_key, SC_PROOF);                                                 \
  ++in_out_shares;                                                                                 \
  VARS_##sbox_args(SC_PROOF);                                                                      \
  mzd_local_t** x = in_out_shares->s;                                                              \
  mzd_local_t* y[SC_PROOF];                                                                        \
  mzd_local_init_multiple_ex(y, SC_PROOF, 1, lowmc->n, false);                                     \
                                                                                                   \
  MPC_LOOP(const_mat_mul_func, x, lowmc_key, lowmc->k0_##no_scr, SC_PROOF);                        \
  MPC_IF_ELSE(add_func, x, x, p, SC_PROOF, ch);                                                    \
                                                                                                   \
  lowmc_round_t const* round = lowmc->rounds;                                                      \
                                                                                                   \
  loop##optimize(sbox_args, sbox, sbox_selector, no_scr, no_scr_active, const_mat_mul_func,        \
                 add_func, mul_more_cols, const_addmat_mul_func, ch, SC_PROOF)                     \
      VARS_FREE_##sbox_args;                                                                       \
  mzd_local_free_multiple(y);

#define init_key mzd_local_t* const* lowmc_key = &in_out_shares->s[0];

#define _mpc_lowmc_call_bitsliced_verify_m(ch, sbox_args, sbox, sbox_selector, no_scr,             \
                                           no_scr_active, optimize, const_mat_mul_func, add_func,  \
                                           mul_more_cols, const_addmat_mul_func)                   \
  init_key;                                                                                        \
                                                                                                   \
  ++in_out_shares;                                                                                 \
  VARS_##sbox_args(SC_VERIFY);                                                                     \
  mzd_local_t* x[2 * SC_VERIFY];                                                                   \
  mzd_local_t** y = &x[SC_VERIFY];                                                                 \
  mzd_local_init_multiple_ex(x, 2 * SC_VERIFY, 1, lowmc->n, false);                                \
                                                                                                   \
  MPC_LOOP(const_mat_mul_func, x, lowmc_key, lowmc->k0_##no_scr, SC_VERIFY);                       \
  MPC_IF_ELSE(add_func, x, x, p, SC_VERIFY, ch);                                                   \
                                                                                                   \
  lowmc_round_t const* round = lowmc->rounds;                                                      \
                                                                                                   \
  loop##optimize(sbox_args, sbox, sbox_selector, no_scr, no_scr_active, const_mat_mul_func,        \
                 add_func, mul_more_cols, const_addmat_mul_func, ch, SC_VERIFY);                   \
  mpc_copy(in_out_shares->s, x, SC_VERIFY);                                                        \
  mzd_local_free_multiple(x);                                                                      \
  VARS_FREE_##sbox_args;

static void mpc_lowmc_call(lowmc_t const* lowmc, mpc_lowmc_key_t* lowmc_key, mzd_local_t const* p,
                           view_t* views, in_out_shares_t* in_out_shares, rvec_t* rvec) {
#ifdef MUL_M4RI
  if (lowmc->m != 10) {
    _mpc_lowmc_call_bitsliced(0, 6, _mpc_sbox_layer_bitsliced, mzd, lookup, noscr, _optimize,
                              mzd_mul_vl_general, mzd_xor_general, mzd_mul_vl_general,
                              mzd_addmul_vl_general);
  } else {
    _mpc_lowmc_call_bitsliced(0, 6, , uint64, lookup, noscr, _optimize, mzd_mul_vl_general,
                              mzd_xor_general, mzd_mul_vl_general, mzd_addmul_vl_general)
  }
#else
  if (lowmc->m != 10) {
    _mpc_lowmc_call_bitsliced(0, 6, _mpc_sbox_layer_bitsliced, mzd, matrix, scr, _optimize,
                              mzd_mul_v_general, mzd_xor_general, mzd_mul_v_general,
                              mzd_addmul_v_general);
  } else {_mpc_lowmc_call_bitsliced(0, 6, , uint64, matrix, scr, _optimize, mzd_mul_v_general,
                            mzd_xor_general, mzd_mul_v_general, mzd_addmul_v_general
  }
#endif
}

static void mpc_lowmc_call_verify(lowmc_t const* lowmc, mzd_local_t const* p, view_t* views,
                                  in_out_shares_t* in_out_shares, rvec_t* rvec, unsigned int ch) {
#ifdef MUL_M4RI
  if (lowmc->m != 10) {
    _mpc_lowmc_call_bitsliced_verify_m(ch, 6, _mpc_sbox_layer_bitsliced_verify, mzd, lookup, noscr,
                                       _optimize, mzd_mul_vl_general, mzd_xor_general,
                                       mzd_mul_vl_general, mzd_addmul_vl_general);
  } else {
    _mpc_lowmc_call_bitsliced_verify_m(ch, 6, , uint64, lookup, noscr, _optimize,
                                       mzd_mul_vl_general, mzd_xor_general, mzd_mul_vl_general,
                                       mzd_addmul_vl_general);
  }
#else
  if (lowmc->m != 10) {
    _mpc_lowmc_call_bitsliced_verify_m(ch, 6, _mpc_sbox_layer_bitsliced_verify, mzd, matrix, scr,
                                       _optimize, mzd_mul_v_general, mzd_xor_general,
                                       mzd_mul_v_general, mzd_addmul_v_general);
  } else {
    _mpc_lowmc_call_bitsliced_verify_m(ch, 6, , uint64, matrix, scr, _optimize, mzd_mul_v_general,
                                       mzd_xor_general, mzd_mul_v_general, mzd_addmul_v_general);
  }
#endif
}

#ifdef MUL_M4RI
#define mpc_lowmc_call_def(N_SIGN, N_VERIFY, SBOX_SIGN, SBOX_VERIFY, MUL, MUL_L, XOR, XOR_L,       \
                           MUL_MC, MUL_MC_L, ADDMUL, ADDMUL_L)                                     \
  static void N_SIGN(lowmc_t const* lowmc, mpc_lowmc_key_t* lowmc_key, mzd_local_t const* p,       \
                     view_t* views, in_out_shares_t* in_out_shares, rvec_t* rvec) {                \
    if (lowmc->m != 10) {                                                                          \
      _mpc_lowmc_call_bitsliced(0, 5, SBOX_SIGN, mzd, lookup, noscr, _optimize, MUL_L, XOR_L,      \
                                MUL_MC_L, ADDMUL_L);                                               \
    } else {                                                                                       \
      _mpc_lowmc_call_bitsliced(0, 5, , uint64, lookup, noscr, _optimize, MUL_L, XOR_L, MUL_MC_L,  \
                                ADDMUL_L);                                                         \
    }                                                                                              \
  }                                                                                                \
  static void N_VERIFY(lowmc_t const* lowmc, mzd_local_t const* p, view_t* views,                  \
                       in_out_shares_t* in_out_shares, rvec_t* rvec, unsigned int ch) {            \
    if (lowmc->m != 10) {                                                                          \
      _mpc_lowmc_call_bitsliced_verify_m(ch, 5, SBOX_VERIFY, mzd, lookup, noscr, _optimize, MUL_L, \
                                         XOR_L, MUL_MC_L, ADDMUL_L);                               \
    } else {                                                                                       \
      _mpc_lowmc_call_bitsliced_verify_m(ch, 5, , uint64, lookup, noscr, _optimize, MUL_L, XOR_L,  \
                                         MUL_MC_L, ADDMUL_L);                                      \
    }                                                                                              \
  }
#else
#define mpc_lowmc_call_def(N_SIGN, N_VERIFY, SBOX_SIGN, SBOX_VERIFY, MUL, MUL_L, XOR, XOR_L,       \
                           MUL_MC, MUL_MC_L, ADDMUL, ADDMUL_L)                                     \
  static void N_SIGN(lowmc_t const* lowmc, mpc_lowmc_key_t* lowmc_key, mzd_local_t const* p,       \
                     view_t* views, in_out_shares_t* in_out_shares, rvec_t* rvec) {                \
    if (lowmc->m != 10) {                                                                          \
      _mpc_lowmc_call_bitsliced(0, 5, SBOX_SIGN, mzd, matrix, scr, _optimize, MUL, XOR, MUL_MC,    \
                                ADDMUL);                                                           \
    } else {                                                                                       \
      _mpc_lowmc_call_bitsliced(0, 5, , uint64, matrix, scr, _optimize, MUL, XOR, MUL_MC, ADDMUL); \
    }                                                                                              \
  }                                                                                                \
  static void N_VERIFY(lowmc_t const* lowmc, mzd_local_t const* p, view_t* views,                  \
                       in_out_shares_t* in_out_shares, rvec_t* rvec, unsigned int ch) {            \
    if (lowmc->m != 10) {                                                                          \
      _mpc_lowmc_call_bitsliced_verify_m(ch, 5, SBOX_VERIFY, matrix, scr, _optimize, MUL, XOR,     \
                                         MUL_MC, ADDMUL);                                          \
    } else {                                                                                       \
      _mpc_lowmc_call_bitsliced_verify_m(ch, 5, , uint64, matrix, scr, _optimize, MUL, XOR,        \
                                         MUL_MC, ADDMUL);                                          \
    }                                                                                              \
  }
#endif


                                    static void sbox_vars_clear(sbox_vars_t* vars) {
  if (vars->storage) {
    mzd_local_free_multiple(vars->storage);
    free(vars->storage);
    memset(vars, 0, sizeof(*vars));
  }
}

static sbox_vars_t* sbox_vars_init(sbox_vars_t* vars, uint32_t n, unsigned sc) {
  vars->storage = calloc(11 * sc, sizeof(mzd_local_t*));
  mzd_local_init_multiple_ex(vars->storage, 11 * sc, 1, n, false);

  for (unsigned int i = 0; i < sc; ++i) {
    vars->x0m[i] = vars->storage[11 * i + 0];
    vars->x1m[i] = vars->storage[11 * i + 1];
    vars->x2m[i] = vars->storage[11 * i + 2];
    vars->r0m[i] = vars->storage[11 * i + 3];
    vars->r1m[i] = vars->storage[11 * i + 4];
    vars->r2m[i] = vars->storage[11 * i + 5];
    vars->x0s[i] = vars->storage[11 * i + 6];
    vars->x1s[i] = vars->storage[11 * i + 7];
    vars->r0s[i] = vars->storage[11 * i + 8];
    vars->r1s[i] = vars->storage[11 * i + 9];
    vars->v[i]   = vars->storage[11 * i + 10];
  }

  return vars;
}

lowmc_implementation_f get_lowmc_implementation(const lowmc_t* lowmc) {
  lowmc_implementation_f lowmc_call;

  {
    (void)lowmc;
    lowmc_call = mpc_lowmc_call;
  }

  return lowmc_call;
}

lowmc_verify_implementation_f get_lowmc_verify_implementation(const lowmc_t* lowmc) {
  lowmc_verify_implementation_f lowmc_call;

  {
    (void)lowmc;
    lowmc_call = mpc_lowmc_call_verify;
  }

  return lowmc_call;
}
