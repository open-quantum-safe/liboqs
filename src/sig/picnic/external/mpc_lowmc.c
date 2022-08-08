/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "mpc_lowmc.h"
#include "mzd_additional.h"

#if !defined(_MSC_VER)
#include <stdalign.h>
#endif
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#if defined(WITH_OPT)
#include "simd.h"
#endif

#if defined(WITH_LOWMC_128_128_20)
#include "lowmc_128_128_20.h"
#endif
#if defined(WITH_LOWMC_129_129_4)
#include "lowmc_129_129_4.h"
#endif
#if defined(WITH_LOWMC_192_192_4)
#include "lowmc_192_192_4.h"
#endif
#if defined(WITH_LOWMC_192_192_30)
#include "lowmc_192_192_30.h"
#endif
#if defined(WITH_LOWMC_256_256_38)
#include "lowmc_256_256_38.h"
#endif
#if defined(WITH_LOWMC_255_255_4)
#include "lowmc_255_255_4.h"
#endif

#define MPC_LOOP_CONST(function, result, first, second, sc)                                        \
  do {                                                                                             \
    for (unsigned int e = 0; e < (sc); ++e) {                                                      \
      function((result)[e], (first)[e], (second));                                                 \
    }                                                                                              \
  } while (0)

#define MPC_LOOP_SHARED(function, result, first, second, sc)                                       \
  do {                                                                                             \
    for (unsigned int o = 0; o < (sc); ++o) {                                                      \
      function((result)[o], (first)[o], (second)[o]);                                              \
    }                                                                                              \
  } while (0)

#define MPC_LOOP_SHARED_1(function, result, first, sc)                                             \
  do {                                                                                             \
    for (unsigned int o = 0; o < (sc); ++o) {                                                      \
      function((result)[o], (first)[o]);                                                           \
    }                                                                                              \
  } while (0)

#define MPC_LOOP_CONST_C_0(function, result, first, second, sc)                                    \
  function((result)[0], (first)[0], (second))

#define MPC_LOOP_CONST_C_ch(function, result, first, second, sc, c)                                \
  do {                                                                                             \
    if (!(c)) {                                                                                    \
      MPC_LOOP_CONST_C_0(function, result, first, second, sc);                                     \
    } else if ((c) == (sc)) {                                                                      \
      function((result)[(sc)-1], first[(sc)-1], (second));                                         \
    }                                                                                              \
  } while (0)

// clang-format off
#if defined(WITH_LOWMC_128_128_20) || defined(WITH_LOWMC_192_192_30) || defined(WITH_LOWMC_256_256_38)
// clang-format on
/* MPC Sbox implementation for partical Sbox */
static void mpc_and_uint64(uint64_t* res, uint64_t const* first, uint64_t const* second,
                           uint64_t const* r, view_t* view, unsigned viewshift) {
  for (unsigned m = 0; m < SC_PROOF; ++m) {
    const unsigned j = (m + 1) % SC_PROOF;
    uint64_t tmp1    = second[m] ^ second[j];
    uint64_t tmp2    = first[j] & second[m];
    tmp1             = tmp1 & first[m];
    tmp1             = tmp1 ^ tmp2;
    tmp2             = r[m] ^ r[j];
    res[m] = tmp1 = tmp1 ^ tmp2;
    if (viewshift) {
      tmp1       = tmp1 >> viewshift;
      view->t[m] = view->t[m] ^ tmp1;
    } else {
      // on first call (viewshift == 0), view->t[0..2] == 0
      view->t[m] = tmp1;
    }
  }
}

static void mpc_and_verify_uint64(uint64_t* res, uint64_t const* first, uint64_t const* second,
                                  uint64_t const* r, view_t* view, uint64_t const mask,
                                  unsigned viewshift) {
  for (unsigned m = 0; m < (SC_VERIFY - 1); ++m) {
    const unsigned j = (m + 1);
    uint64_t tmp1    = second[m] ^ second[j];
    uint64_t tmp2    = first[j] & second[m];
    tmp1             = tmp1 & first[m];
    tmp1             = tmp1 ^ tmp2;
    tmp2             = r[m] ^ r[j];
    res[m] = tmp1 = tmp1 ^ tmp2;
    if (viewshift || m) {
      tmp1       = tmp1 >> viewshift;
      view->t[m] = view->t[m] ^ tmp1;
    } else {
      // on first call (viewshift == 0), view->t[0] == 0
      view->t[m] = tmp1;
    }
  }

  const uint64_t rsc = view->t[SC_VERIFY - 1] << viewshift;
  res[SC_VERIFY - 1] = rsc & mask;
}

#define bitsliced_step_1_uint64_10(sc)                                                             \
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

#define bitsliced_step_2_uint64_10(sc)                                                             \
  do {                                                                                             \
    for (unsigned int m = 0; m < (sc); ++m) {                                                      \
      const uint64_t tmp1 = r2m[m] ^ x0s[m];                                                       \
      const uint64_t tmp2 = x0s[m] ^ x1s[m];                                                       \
      const uint64_t tmp3 = tmp2 ^ r1m[m];                                                         \
      const uint64_t tmp4 = tmp2 ^ r0m[m] ^ x2m[m];                                                \
                                                                                                   \
      in[m] = (in[m] & MASK_MASK) ^ (tmp4) ^ (tmp1 >> 2) ^ (tmp3 >> 1);                            \
    }                                                                                              \
  } while (0)

static void mpc_sbox_prove_uint64_10(uint64_t* in, view_t* view, uint64_t const* rvec) {
  bitsliced_step_1_uint64_10(SC_PROOF);

  mpc_and_uint64(r0m, x0s, x1s, r2m, view, 0);
  mpc_and_uint64(r2m, x1s, x2m, r1s, view, 1);
  mpc_and_uint64(r1m, x0s, x2m, r0s, view, 2);

  bitsliced_step_2_uint64_10(SC_PROOF - 1);
}

static void mpc_sbox_verify_uint64_10(uint64_t* in, view_t* view, uint64_t const* rvec) {
  bitsliced_step_1_uint64_10(SC_VERIFY);

  mpc_and_verify_uint64(r0m, x0s, x1s, r2m, view, MASK_X2I, 0);
  mpc_and_verify_uint64(r2m, x1s, x2m, r1s, view, MASK_X2I, 1);
  mpc_and_verify_uint64(r1m, x0s, x2m, r0s, view, MASK_X2I, 2);

  bitsliced_step_2_uint64_10(SC_VERIFY);
}
#endif

/* MPC Sbox implementation for full instances */
#if !defined(NO_UINT64_FALLBACK)
#if defined(WITH_LOWMC_129_129_4) || defined(WITH_LOWMC_192_192_4)
static void mpc_and_uint64_192(mzd_local_t* res, const mzd_local_t* first,
                               const mzd_local_t* second, const mzd_local_t* r, view_t* view,
                               unsigned viewshift) {
  mzd_local_t tmp = {{0}};

  for (unsigned int m = 0; m < SC_PROOF; ++m) {
    const unsigned int j = (m + 1) % SC_PROOF;

    // f[m] & s[m]
    mzd_and_uint64_192(&res[m], &first[m], &second[m]);

    // f[m + 1] & s[m]
    mzd_and_uint64_192(&tmp, &first[j], &second[m]);
    mzd_xor_uint64_192(&res[m], &res[m], &tmp);

    // f[m] & s[m + 1]
    mzd_and_uint64_192(&tmp, &first[m], &second[j]);
    mzd_xor_uint64_192(&res[m], &res[m], &tmp);

    // ... ^ r[m] ^ r[m + 1]
    mzd_xor_uint64_192(&tmp, &r[m], &r[j]);
    mzd_xor_uint64_192(&res[m], &res[m], &tmp);

    if (viewshift) {
      mzd_shift_right_uint64_192(&tmp, &res[m], viewshift);
      mzd_xor_uint64_192(&view->s[m], &view->s[m], &tmp);
    } else {
      // on first call (viewshift == 0), view->t[0..2] == 0
      mzd_copy_uint64_192(&view->s[m], &res[m]);
    }
  }
}

static void mpc_and_verify_uint64_192(mzd_local_t* res, const mzd_local_t* first,
                                      const mzd_local_t* second, const mzd_local_t* r, view_t* view,
                                      const mzd_local_t* mask, unsigned viewshift) {
  mzd_local_t tmp = {{0}};

  for (unsigned int m = 0; m < (SC_VERIFY - 1); ++m) {
    const unsigned int j = m + 1;

    mzd_and_uint64_192(&res[m], &first[m], &second[m]);

    mzd_and_uint64_192(&tmp, &first[j], &second[m]);
    mzd_xor_uint64_192(&res[m], &res[m], &tmp);

    mzd_and_uint64_192(&tmp, &first[m], &second[j]);
    mzd_xor_uint64_192(&res[m], &res[m], &tmp);

    mzd_xor_uint64_192(&tmp, &r[m], &r[j]);
    mzd_xor_uint64_192(&res[m], &res[m], &tmp);

    if (viewshift) {
      mzd_shift_right_uint64_192(&tmp, &res[m], viewshift);
      mzd_xor_uint64_192(&view->s[m], &view->s[m], &tmp);
    } else {
      // on first call (viewshift == 0), view->s[0] == 0
      mzd_copy_uint64_192(&view->s[m], &res[m]);
    }
  }

  if (viewshift) {
    mzd_shift_left_uint64_192(&tmp, &view->s[SC_VERIFY - 1], viewshift);
    mzd_and_uint64_192(&res[SC_VERIFY - 1], &tmp, mask);
  } else {
    mzd_and_uint64_192(&res[SC_VERIFY - 1], &view->s[SC_VERIFY - 1], mask);
  }
}
#endif

#if defined(WITH_LOWMC_255_255_4)
static void mpc_and_uint64_256(mzd_local_t* res, const mzd_local_t* first,
                               const mzd_local_t* second, const mzd_local_t* r, view_t* view,
                               unsigned viewshift) {
  mzd_local_t tmp = {{0}};

  for (unsigned int m = 0; m < SC_PROOF; ++m) {
    const unsigned int j = (m + 1) % SC_PROOF;

    // f[m] & s[m]
    mzd_and_uint64_256(&res[m], &first[m], &second[m]);

    // f[m + 1] & s[m]
    mzd_and_uint64_256(&tmp, &first[j], &second[m]);
    mzd_xor_uint64_256(&res[m], &res[m], &tmp);

    // f[m] & s[m + 1]
    mzd_and_uint64_256(&tmp, &first[m], &second[j]);
    mzd_xor_uint64_256(&res[m], &res[m], &tmp);

    // ... ^ r[m] ^ r[m + 1]
    mzd_xor_uint64_256(&tmp, &r[m], &r[j]);
    mzd_xor_uint64_256(&res[m], &res[m], &tmp);

    if (viewshift) {
      mzd_shift_right_uint64_256(&tmp, &res[m], viewshift);
      mzd_xor_uint64_256(&view->s[m], &view->s[m], &tmp);
    } else {
      // on first call (viewshift == 0), view->t[0..2] == 0
      mzd_copy_uint64_256(&view->s[m], &res[m]);
    }
  }
}

static void mpc_and_verify_uint64_256(mzd_local_t* res, const mzd_local_t* first,
                                      const mzd_local_t* second, const mzd_local_t* r, view_t* view,
                                      const mzd_local_t* mask, unsigned viewshift) {
  mzd_local_t tmp = {{0}};

  for (unsigned int m = 0; m < (SC_VERIFY - 1); ++m) {
    const unsigned int j = m + 1;

    mzd_and_uint64_256(&res[m], &first[m], &second[m]);

    mzd_and_uint64_256(&tmp, &first[j], &second[m]);
    mzd_xor_uint64_256(&res[m], &res[m], &tmp);

    mzd_and_uint64_256(&tmp, &first[m], &second[j]);
    mzd_xor_uint64_256(&res[m], &res[m], &tmp);

    mzd_xor_uint64_256(&tmp, &r[m], &r[j]);
    mzd_xor_uint64_256(&res[m], &res[m], &tmp);

    if (viewshift) {
      mzd_shift_right_uint64_256(&tmp, &res[m], viewshift);
      mzd_xor_uint64_256(&view->s[m], &view->s[m], &tmp);
    } else {
      // on first call (viewshift == 0), view->s[0] == 0
      mzd_copy_uint64_256(&view->s[m], &res[m]);
    }
  }

  if (viewshift) {
    mzd_shift_left_uint64_256(&tmp, &view->s[SC_VERIFY - 1], viewshift);
    mzd_and_uint64_256(&res[SC_VERIFY - 1], &tmp, mask);
  } else {
    mzd_and_uint64_256(&res[SC_VERIFY - 1], &view->s[SC_VERIFY - 1], mask);
  }
}
#endif

#define bitsliced_step_1(sc, AND, ROL, MASK_A, MASK_B, MASK_C)                                     \
  mzd_local_t x2m[sc] = {{{0}}};                                                                   \
  mzd_local_t r0m[sc] = {{{0}}}, r1m[sc] = {{{0}}}, r2m[sc] = {{{0}}};                             \
  mzd_local_t x0s[sc] = {{{0}}}, x1s[sc] = {{{0}}}, r0s[sc] = {{{0}}}, r1s[sc] = {{{0}}};          \
                                                                                                   \
  for (unsigned int m = 0; m < (sc); ++m) {                                                        \
    AND(&x0s[m], &in[m], MASK_A);                                                                  \
    AND(&x1s[m], &in[m], MASK_B);                                                                  \
    AND(&x2m[m], &in[m], MASK_C);                                                                  \
                                                                                                   \
    ROL(&x0s[m], &x0s[m], 2);                                                                      \
    ROL(&x1s[m], &x1s[m], 1);                                                                      \
                                                                                                   \
    AND(&r0m[m], &rvec->s[m], MASK_A);                                                             \
    AND(&r1m[m], &rvec->s[m], MASK_B);                                                             \
    AND(&r2m[m], &rvec->s[m], MASK_C);                                                             \
                                                                                                   \
    ROL(&r0s[m], &r0m[m], 2);                                                                      \
    ROL(&r1s[m], &r1m[m], 1);                                                                      \
  }

#define bitsliced_step_2(sc, XOR, ROR)                                                             \
  for (unsigned int m = 0; m < sc; ++m) {                                                          \
    XOR(&r2m[m], &r2m[m], &x0s[m]);                                                                \
    XOR(&x0s[m], &x0s[m], &x1s[m]);                                                                \
    XOR(&r1m[m], &r1m[m], &x0s[m]);                                                                \
    XOR(&r0m[m], &r0m[m], &x0s[m]);                                                                \
    XOR(&r0m[m], &r0m[m], &x2m[m]);                                                                \
                                                                                                   \
    ROR(&x0s[m], &r2m[m], 2);                                                                      \
    ROR(&x1s[m], &r1m[m], 1);                                                                      \
                                                                                                   \
    XOR(&x0s[m], &x0s[m], &r0m[m]);                                                                \
    XOR(&out[m], &x0s[m], &x1s[m]);                                                                \
  }

#if defined(WITH_LOWMC_129_129_4)
static void mpc_sbox_prove_uint64_lowmc_129_129_4(mzd_local_t* out, const mzd_local_t* in,
                                                  view_t* view, const rvec_t* rvec) {
  bitsliced_step_1(SC_PROOF, mzd_and_uint64_192, mzd_shift_left_uint64_192, mask_129_129_43_a,
                   mask_129_129_43_b, mask_129_129_43_c);

  // a & b
  mpc_and_uint64_192(r0m, x0s, x1s, r2m, view, 0);
  // b & c
  mpc_and_uint64_192(r2m, x1s, x2m, r1s, view, 1);
  // c & a
  mpc_and_uint64_192(r1m, x0s, x2m, r0s, view, 2);

  bitsliced_step_2(SC_PROOF, mzd_xor_uint64_192, mzd_shift_right_uint64_192);
}

static void mpc_sbox_verify_uint64_lowmc_129_129_4(mzd_local_t* out, const mzd_local_t* in,
                                                   view_t* view, const rvec_t* rvec) {
  bitsliced_step_1(SC_VERIFY, mzd_and_uint64_192, mzd_shift_left_uint64_192, mask_129_129_43_a,
                   mask_129_129_43_b, mask_129_129_43_c);

  // a & b
  mpc_and_verify_uint64_192(r0m, x0s, x1s, r2m, view, mask_129_129_43_c, 0);
  // b & c
  mpc_and_verify_uint64_192(r2m, x1s, x2m, r1s, view, mask_129_129_43_c, 1);
  // c & a
  mpc_and_verify_uint64_192(r1m, x0s, x2m, r0s, view, mask_129_129_43_c, 2);

  bitsliced_step_2(SC_VERIFY, mzd_xor_uint64_192, mzd_shift_right_uint64_192);
}
#endif

#if defined(WITH_LOWMC_192_192_4)
static void mpc_sbox_prove_uint64_lowmc_192_192_4(mzd_local_t* out, const mzd_local_t* in,
                                                  view_t* view, const rvec_t* rvec) {
  bitsliced_step_1(SC_PROOF, mzd_and_uint64_192, mzd_shift_left_uint64_192, mask_192_192_64_a,
                   mask_192_192_64_b, mask_192_192_64_c);

  // a & b
  mpc_and_uint64_192(r0m, x0s, x1s, r2m, view, 0);
  // b & c
  mpc_and_uint64_192(r2m, x1s, x2m, r1s, view, 1);
  // c & a
  mpc_and_uint64_192(r1m, x0s, x2m, r0s, view, 2);

  bitsliced_step_2(SC_PROOF, mzd_xor_uint64_192, mzd_shift_right_uint64_192);
}

static void mpc_sbox_verify_uint64_lowmc_192_192_4(mzd_local_t* out, const mzd_local_t* in,
                                                   view_t* view, const rvec_t* rvec) {
  bitsliced_step_1(SC_VERIFY, mzd_and_uint64_192, mzd_shift_left_uint64_192, mask_192_192_64_a,
                   mask_192_192_64_b, mask_192_192_64_c);

  // a & b
  mpc_and_verify_uint64_192(r0m, x0s, x1s, r2m, view, mask_192_192_64_c, 0);
  // b & c
  mpc_and_verify_uint64_192(r2m, x1s, x2m, r1s, view, mask_192_192_64_c, 1);
  // c & a
  mpc_and_verify_uint64_192(r1m, x0s, x2m, r0s, view, mask_192_192_64_c, 2);

  bitsliced_step_2(SC_VERIFY, mzd_xor_uint64_192, mzd_shift_right_uint64_192);
}
#endif

#if defined(WITH_LOWMC_255_255_4)
static void mpc_sbox_prove_uint64_lowmc_255_255_4(mzd_local_t* out, const mzd_local_t* in,
                                                  view_t* view, const rvec_t* rvec) {
  bitsliced_step_1(SC_PROOF, mzd_and_uint64_256, mzd_shift_left_uint64_256, mask_255_255_85_a,
                   mask_255_255_85_b, mask_255_255_85_c);

  // a & b
  mpc_and_uint64_256(r0m, x0s, x1s, r2m, view, 0);
  // b & c
  mpc_and_uint64_256(r2m, x1s, x2m, r1s, view, 1);
  // c & a
  mpc_and_uint64_256(r1m, x0s, x2m, r0s, view, 2);

  bitsliced_step_2(SC_PROOF, mzd_xor_uint64_256, mzd_shift_right_uint64_256);
}

static void mpc_sbox_verify_uint64_lowmc_255_255_4(mzd_local_t* out, const mzd_local_t* in,
                                                   view_t* view, const rvec_t* rvec) {
  bitsliced_step_1(SC_VERIFY, mzd_and_uint64_256, mzd_shift_left_uint64_256, mask_255_255_85_a,
                   mask_255_255_85_b, mask_255_255_85_c);

  // a & b
  mpc_and_verify_uint64_256(r0m, x0s, x1s, r2m, view, mask_255_255_85_c, 0);
  // b & c
  mpc_and_verify_uint64_256(r2m, x1s, x2m, r1s, view, mask_255_255_85_c, 1);
  // c & a
  mpc_and_verify_uint64_256(r1m, x0s, x2m, r0s, view, mask_255_255_85_c, 2);

  bitsliced_step_2(SC_VERIFY, mzd_xor_uint64_256, mzd_shift_right_uint64_256);
}
#endif
#endif /* NO_UINT_FALLBACK */

#if defined(WITH_OPT)
#define NROLR(a, b, c)                                                                             \
  do {                                                                                             \
    a[0] = b[0];                                                                                   \
    a[1] = b[1];                                                                                   \
    (void)c;                                                                                       \
  } while (0)

/* requires IN and RVEC to be defined */
#define bitsliced_mm256_step_1(sc, MASK_A, MASK_B, MASK_C)                                         \
  word256 r0m[sc];                                                                                 \
  word256 r0s[sc];                                                                                 \
  word256 r1m[sc];                                                                                 \
  word256 r1s[sc];                                                                                 \
  word256 r2m[sc];                                                                                 \
  word256 x0s[sc];                                                                                 \
  word256 x1s[sc];                                                                                 \
  word256 x2m[sc];                                                                                 \
  do {                                                                                             \
    for (unsigned int m = 0; m < (sc); ++m) {                                                      \
      word256 tmp = mm256_load(IN(m));                                                             \
      x0s[m]      = mm256_rotate_left(mm256_and(tmp, MASK_A), 2);                                  \
      x1s[m]      = mm256_rotate_left(mm256_and(tmp, MASK_B), 1);                                  \
      x2m[m]      = mm256_and(tmp, MASK_C);                                                        \
                                                                                                   \
      tmp    = mm256_load(RVEC(m));                                                                \
      r0s[m] = mm256_rotate_left(mm256_and(tmp, MASK_A), 2);                                       \
      r1s[m] = mm256_rotate_left(mm256_and(tmp, MASK_B), 1);                                       \
      r2m[m] = mm256_and(tmp, MASK_C);                                                             \
    }                                                                                              \
  } while (0)

#define bitsliced_mm256_step_2(sc)                                                                 \
  do {                                                                                             \
    for (unsigned int m = 0; m < sc; ++m) {                                                        \
      r2m[m] = mm256_xor(r2m[m], x0s[m]);                                                          \
      x0s[m] = mm256_xor(x0s[m], x1s[m]);                                                          \
      r1m[m] = mm256_xor(x0s[m], r1m[m]);                                                          \
      r0m[m] = mm256_xor(x0s[m], r0m[m]);                                                          \
                                                                                                   \
      x0s[m] = mm256_rotate_right(r2m[m], 2);                                                      \
      x1s[m] = mm256_rotate_right(r1m[m], 1);                                                      \
                                                                                                   \
      mm256_store(OUT(m), mm256_xor(mm256_xor(r0m[m], x2m[m]), mm256_xor(x0s[m], x1s[m])));        \
    }                                                                                              \
  } while (0)

#define mpc_mm256_and_def(res, first, second, r, viewshift)                                        \
  do {                                                                                             \
    for (unsigned int m = 0; m < SC_PROOF; ++m) {                                                  \
      const unsigned int j = (m + 1) % SC_PROOF;                                                   \
                                                                                                   \
      word256 tmp1 = mm256_xor(mm256_and(first[m], second[m]), mm256_and(first[j], second[m]));    \
      word256 tmp2 = mm256_xor(mm256_xor(r[m], r[j]), mm256_and(first[m], second[j]));             \
      res[m]       = mm256_xor(tmp1, tmp2);                                                        \
      if (viewshift) {                                                                             \
        mm256_store(VIEW(m),                                                                       \
                    mm256_xor(mm256_rotate_right(res[m], viewshift), mm256_load(VIEW(m))));        \
      } else {                                                                                     \
        mm256_store(VIEW(m), res[m]);                                                              \
      }                                                                                            \
    }                                                                                              \
  } while (0)

#define mpc_mm256_and_verify_def(res, first, second, r, MASK, viewshift)                           \
  do {                                                                                             \
    for (unsigned int m = 0; m < (SC_VERIFY - 1); ++m) {                                           \
      const unsigned int j = m + 1;                                                                \
                                                                                                   \
      word256 tmp1 = mm256_xor(mm256_and(first[m], second[m]), mm256_and(first[j], second[m]));    \
      word256 tmp2 = mm256_xor(mm256_xor(r[m], r[j]), mm256_and(first[m], second[j]));             \
      res[m]       = mm256_xor(tmp1, tmp2);                                                        \
      if (viewshift) {                                                                             \
        mm256_store(VIEW(m),                                                                       \
                    mm256_xor(mm256_rotate_right(res[m], viewshift), mm256_load(VIEW(m))));        \
      } else {                                                                                     \
        mm256_store(VIEW(m), res[m]);                                                              \
      }                                                                                            \
    }                                                                                              \
    if (viewshift) {                                                                               \
      const word256 tmp  = mm256_load(VIEW(SC_VERIFY - 1));                                        \
      res[SC_VERIFY - 1] = mm256_and(mm256_rotate_left(tmp, viewshift), MASK);                     \
    } else {                                                                                       \
      res[SC_VERIFY - 1] = mm256_and(mm256_load(VIEW(SC_VERIFY - 1)), MASK);                       \
    }                                                                                              \
  } while (0)

#if defined(WITH_SSE2) || defined(WITH_NEON)
#define bitsliced_mm128_256_step_1(sc, MASK_A, MASK_B, MASK_C)                                     \
  word128 r0m[sc][2];                                                                              \
  word128 r0s[sc][2];                                                                              \
  word128 r1m[sc][2];                                                                              \
  word128 r1s[sc][2];                                                                              \
  word128 r2m[sc][2];                                                                              \
  word128 x0s[sc][2];                                                                              \
  word128 x1s[sc][2];                                                                              \
  word128 x2m[sc][2];                                                                              \
  do {                                                                                             \
    word128 tmp[2], bitm_a[2], bitm_b[2], bitm_c[2];                                               \
    bitm_a[0] = mm128_load(&MASK_A->w64[0]);                                                       \
    bitm_a[1] = mm128_load(&MASK_A->w64[2]);                                                       \
    bitm_b[0] = mm128_load(&MASK_B->w64[0]);                                                       \
    bitm_b[1] = mm128_load(&MASK_B->w64[2]);                                                       \
    bitm_c[0] = mm128_load(&MASK_C->w64[0]);                                                       \
    bitm_c[1] = mm128_load(&MASK_C->w64[2]);                                                       \
    for (unsigned int m = 0; m < (sc); ++m) {                                                      \
      tmp[0] = mm128_load(&IN(m)[0]);                                                              \
      tmp[1] = mm128_load(&IN(m)[2]);                                                              \
      mm128_and_256(x0s[m], tmp, bitm_a);                                                          \
      mm128_and_256(x1s[m], tmp, bitm_b);                                                          \
      mm128_and_256(x2m[m], tmp, bitm_c);                                                          \
                                                                                                   \
      mm128_shift_left_256(x0s[m], x0s[m], 2);                                                     \
      mm128_shift_left_256(x1s[m], x1s[m], 1);                                                     \
                                                                                                   \
      tmp[0] = mm128_load(&RVEC(m)[0]);                                                            \
      tmp[1] = mm128_load(&RVEC(m)[2]);                                                            \
      mm128_and_256(r0m[m], tmp, bitm_a);                                                          \
      mm128_and_256(r1m[m], tmp, bitm_b);                                                          \
      mm128_and_256(r2m[m], tmp, bitm_c);                                                          \
                                                                                                   \
      mm128_shift_left_256(r0s[m], r0m[m], 2);                                                     \
      mm128_shift_left_256(r1s[m], r1m[m], 1);                                                     \
    }                                                                                              \
  } while (0)

#define bitsliced_mm128_256_step_2(sc)                                                             \
  do {                                                                                             \
    for (unsigned int m = 0; m < sc; ++m) {                                                        \
      mm128_xor_256(r2m[m], r2m[m], x0s[m]);                                                       \
      mm128_xor_256(x0s[m], x0s[m], x1s[m]);                                                       \
      mm128_xor_256(r1m[m], x0s[m], r1m[m]);                                                       \
      mm128_xor_256(r0m[m], x0s[m], r0m[m]);                                                       \
      mm128_xor_256(r0m[m], r0m[m], x2m[m]);                                                       \
                                                                                                   \
      mm128_shift_right_256(x0s[m], r2m[m], 2);                                                    \
      mm128_shift_right_256(x1s[m], r1m[m], 1);                                                    \
                                                                                                   \
      mm128_xor_256(x0s[m], x0s[m], x1s[m]);                                                       \
      mm128_xor_256(r0m[m], r0m[m], x0s[m]);                                                       \
      mm128_store(&OUT(m)[0], r0m[m][0]);                                                          \
      mm128_store(&OUT(m)[2], r0m[m][1]);                                                          \
    }                                                                                              \
  } while (0)

#define mpc_mm128_256_and_def(ROR, res, first, second, r, viewshift)                               \
  do {                                                                                             \
    for (unsigned int m = 0; m < SC_PROOF; ++m) {                                                  \
      const unsigned int j = (m + 1) % SC_PROOF;                                                   \
      word128 tmp1[2], tmp2[2];                                                                    \
                                                                                                   \
      mm128_and_256(tmp1, first[m], second[m]);                                                    \
      mm128_and_256(tmp2, first[j], second[m]);                                                    \
      mm128_xor_256(res[m], tmp1, tmp2);                                                           \
      mm128_and_256(tmp1, first[m], second[j]);                                                    \
      mm128_xor_256(res[m], res[m], tmp1);                                                         \
      mm128_xor_256(tmp2, r[m], r[j]);                                                             \
      mm128_xor_256(res[m], res[m], tmp2);                                                         \
      if (viewshift) {                                                                             \
        ROR(tmp1, res[m], viewshift);                                                              \
        tmp2[0] = mm128_load(&VIEW(m)[0]);                                                         \
        tmp2[1] = mm128_load(&VIEW(m)[2]);                                                         \
        mm128_xor_256(tmp2, tmp1, tmp2);                                                           \
        mm128_store(&VIEW(m)[0], tmp2[0]);                                                         \
        mm128_store(&VIEW(m)[2], tmp2[1]);                                                         \
      } else {                                                                                     \
        mm128_store(&VIEW(m)[0], res[m][0]);                                                       \
        mm128_store(&VIEW(m)[2], res[m][1]);                                                       \
      }                                                                                            \
    }                                                                                              \
  } while (0)

#define mpc_mm128_256_and_verify_def(ROL, ROR, res, first, second, r, MASK, viewshift)             \
  do {                                                                                             \
    word128 tmp1[2], tmp2[2];                                                                      \
    for (unsigned int m = 0; m < (SC_VERIFY - 1); ++m) {                                           \
      const unsigned int j = (m + 1) % SC_PROOF;                                                   \
                                                                                                   \
      mm128_and_256(tmp1, first[m], second[m]);                                                    \
      mm128_and_256(tmp2, first[j], second[m]);                                                    \
      mm128_xor_256(res[m], tmp1, tmp2);                                                           \
      mm128_and_256(tmp1, first[m], second[j]);                                                    \
      mm128_xor_256(res[m], res[m], tmp1);                                                         \
      mm128_xor_256(tmp2, r[m], r[j]);                                                             \
      mm128_xor_256(res[m], res[m], tmp2);                                                         \
      if (viewshift) {                                                                             \
        ROR(tmp1, res[m], viewshift);                                                              \
        tmp2[0] = mm128_load(&VIEW(m)[0]);                                                         \
        tmp2[1] = mm128_load(&VIEW(m)[2]);                                                         \
        mm128_xor_256(tmp2, tmp1, tmp2);                                                           \
        mm128_store(&VIEW(m)[0], tmp2[0]);                                                         \
        mm128_store(&VIEW(m)[2], tmp2[1]);                                                         \
      } else {                                                                                     \
        mm128_store(&VIEW(m)[0], res[m][0]);                                                       \
        mm128_store(&VIEW(m)[2], res[m][1]);                                                       \
      }                                                                                            \
    }                                                                                              \
                                                                                                   \
    tmp2[0] = mm128_load(&VIEW(SC_VERIFY - 1)[0]);                                                 \
    tmp2[1] = mm128_load(&VIEW(SC_VERIFY - 1)[2]);                                                 \
    tmp1[0] = mm128_load(&MASK->w64[0]);                                                           \
    tmp1[1] = mm128_load(&MASK->w64[2]);                                                           \
    if (viewshift) {                                                                               \
      ROL(tmp2, tmp2, viewshift);                                                                  \
      mm128_and_256(res[SC_VERIFY - 1], tmp2, tmp1);                                               \
    } else {                                                                                       \
      mm128_and_256(res[SC_VERIFY - 1], tmp2, tmp1);                                               \
    }                                                                                              \
  } while (0)

#define IN(m) in[m].w64
#define OUT(m) out[m].w64
#define RVEC(m) rvec->s[m].w64
#define VIEW(m) view->s[m].w64

#if defined(WITH_LOWMC_129_129_4) || defined(WITH_LOWMC_192_192_4) || defined(WITH_LOWMC_255_255_4)
ATTR_TARGET_S128
static inline void mpc_sbox_prove_s128_256(mzd_local_t* out, const mzd_local_t* in, view_t* view,
                                           const rvec_t* rvec, const mzd_local_t* mask_a,
                                           const mzd_local_t* mask_b, const mzd_local_t* mask_c) {
  bitsliced_mm128_256_step_1(SC_PROOF, mask_a, mask_b, mask_c);

  // a & b
  mpc_mm128_256_and_def(NROLR, r0m, x0s, x1s, r2m, 0);
  // b & c
  mpc_mm128_256_and_def(mm128_shift_right_256, r2m, x1s, x2m, r1s, 1);
  // c & a
  mpc_mm128_256_and_def(mm128_shift_right_256, r1m, x0s, x2m, r0s, 2);

  bitsliced_mm128_256_step_2(SC_PROOF);
}

ATTR_TARGET_S128
static inline void mpc_sbox_verify_s128_256(mzd_local_t* out, const mzd_local_t* in, view_t* view,
                                            const rvec_t* rvec, const mzd_local_t* mask_a,
                                            const mzd_local_t* mask_b, const mzd_local_t* mask_c) {
  bitsliced_mm128_256_step_1(SC_VERIFY, mask_a, mask_b, mask_c);

  // a & b
  mpc_mm128_256_and_verify_def(NROLR, NROLR, r0m, x0s, x1s, r2m, mask_c, 0);
  // b & c
  mpc_mm128_256_and_verify_def(mm128_shift_left_256, mm128_shift_right_256, r2m, x1s, x2m, r1s,
                               mask_c, 1);
  // c & a
  mpc_mm128_256_and_verify_def(mm128_shift_left_256, mm128_shift_right_256, r1m, x0s, x2m, r0s,
                               mask_c, 2);

  bitsliced_mm128_256_step_2(SC_VERIFY);
}
#endif

#if defined(WITH_LOWMC_129_129_4)
ATTR_TARGET_S128
static void mpc_sbox_prove_s128_lowmc_129_129_4(mzd_local_t* out, const mzd_local_t* in,
                                                view_t* view, const rvec_t* rvec) {
  mpc_sbox_prove_s128_256(out, in, view, rvec, mask_129_129_43_a, mask_129_129_43_b,
                          mask_129_129_43_c);
}

ATTR_TARGET_S128
static void mpc_sbox_verify_s128_lowmc_129_129_4(mzd_local_t* out, const mzd_local_t* in,
                                                 view_t* view, const rvec_t* rvec) {
  mpc_sbox_verify_s128_256(out, in, view, rvec, mask_129_129_43_a, mask_129_129_43_b,
                           mask_129_129_43_c);
}
#endif

#if defined(WITH_LOWMC_192_192_4)
ATTR_TARGET_S128
static void mpc_sbox_prove_s128_lowmc_192_192_4(mzd_local_t* out, const mzd_local_t* in,
                                                view_t* view, const rvec_t* rvec) {
  mpc_sbox_prove_s128_256(out, in, view, rvec, mask_192_192_64_a, mask_192_192_64_b,
                          mask_192_192_64_c);
}

ATTR_TARGET_S128
static void mpc_sbox_verify_s128_lowmc_192_192_4(mzd_local_t* out, const mzd_local_t* in,
                                                 view_t* view, const rvec_t* rvec) {
  mpc_sbox_verify_s128_256(out, in, view, rvec, mask_192_192_64_a, mask_192_192_64_b,
                           mask_192_192_64_c);
}
#endif

#if defined(WITH_LOWMC_255_255_4)
ATTR_TARGET_S128
static void mpc_sbox_prove_s128_lowmc_255_255_4(mzd_local_t* out, const mzd_local_t* in,
                                                view_t* view, const rvec_t* rvec) {
  mpc_sbox_prove_s128_256(out, in, view, rvec, mask_255_255_85_a, mask_255_255_85_b,
                          mask_255_255_85_c);
}

ATTR_TARGET_S128
static void mpc_sbox_verify_s128_lowmc_255_255_4(mzd_local_t* out, const mzd_local_t* in,
                                                 view_t* view, const rvec_t* rvec) {
  mpc_sbox_verify_s128_256(out, in, view, rvec, mask_255_255_85_a, mask_255_255_85_b,
                           mask_255_255_85_c);
}
#endif

#undef IN
#undef OUT
#undef RVEC
#undef VIEW
#endif /* WITH_SSE2 || WITH_NEON */

#if defined(WITH_AVX2)
#define IN(m) in[m].w64
#define OUT(m) out[m].w64
#define RVEC(m) rvec->s[m].w64
#define VIEW(m) view->s[m].w64

#if defined(WITH_LOWMC_129_129_4) || defined(WITH_LOWMC_192_192_4) || defined(WITH_LOWMC_255_255_4)
ATTR_TARGET_AVX2
static inline void mpc_sbox_prove_s256_256(mzd_local_t* out, const mzd_local_t* in, view_t* view,
                                           const rvec_t* rvec, const word256 mask_a,
                                           const word256 mask_b, const word256 mask_c) {
  bitsliced_mm256_step_1(SC_PROOF, mask_a, mask_b, mask_c);

  // a & b
  mpc_mm256_and_def(r0m, x0s, x1s, r2m, 0);
  // b & c
  mpc_mm256_and_def(r2m, x1s, x2m, r1s, 1);
  // c & a
  mpc_mm256_and_def(r1m, x0s, x2m, r0s, 2);

  bitsliced_mm256_step_2(SC_PROOF);
}

ATTR_TARGET_AVX2
static void mpc_sbox_verify_s256_256(mzd_local_t* out, const mzd_local_t* in, view_t* view,
                                     const rvec_t* rvec, const word256 mask_a, const word256 mask_b,
                                     const word256 mask_c) {
  bitsliced_mm256_step_1(SC_VERIFY, mask_a, mask_b, mask_c);

  // a & b
  mpc_mm256_and_verify_def(r0m, x0s, x1s, r2m, mask_c, 0);
  // b & c
  mpc_mm256_and_verify_def(r2m, x1s, x2m, r1s, mask_c, 1);
  // c & a
  mpc_mm256_and_verify_def(r1m, x0s, x2m, r0s, mask_c, 2);

  bitsliced_mm256_step_2(SC_VERIFY);
}
#endif

#if defined(WITH_LOWMC_129_129_4)
ATTR_TARGET_AVX2
static void mpc_sbox_prove_s256_lowmc_129_129_4(mzd_local_t* out, const mzd_local_t* in,
                                                view_t* view, const rvec_t* rvec) {
  mpc_sbox_prove_s256_256(out, in, view, rvec,
                          mm256_set_4(MASK_129_129_43_A_0, MASK_129_129_43_A_1, MASK_129_129_43_A_2,
                                      MASK_129_129_43_A_3),
                          mm256_set_4(MASK_129_129_43_B_0, MASK_129_129_43_B_1, MASK_129_129_43_B_2,
                                      MASK_129_129_43_B_3),
                          mm256_set_4(MASK_129_129_43_C_0, MASK_129_129_43_C_1, MASK_129_129_43_C_2,
                                      MASK_129_129_43_C_3));
}

ATTR_TARGET_AVX2
static void mpc_sbox_verify_s256_lowmc_129_129_4(mzd_local_t* out, const mzd_local_t* in,
                                                 view_t* view, const rvec_t* rvec) {
  mpc_sbox_verify_s256_256(out, in, view, rvec,
                           mm256_set_4(MASK_129_129_43_A_0, MASK_129_129_43_A_1,
                                       MASK_129_129_43_A_2, MASK_129_129_43_A_3),
                           mm256_set_4(MASK_129_129_43_B_0, MASK_129_129_43_B_1,
                                       MASK_129_129_43_B_2, MASK_129_129_43_B_3),
                           mm256_set_4(MASK_129_129_43_C_0, MASK_129_129_43_C_1,
                                       MASK_129_129_43_C_2, MASK_129_129_43_C_3));
}
#endif

#if defined(WITH_LOWMC_192_192_4)
ATTR_TARGET_AVX2
static void mpc_sbox_prove_s256_lowmc_192_192_4(mzd_local_t* out, const mzd_local_t* in,
                                                view_t* view, const rvec_t* rvec) {
  mpc_sbox_prove_s256_256(out, in, view, rvec,
                          mm256_set_4(MASK_192_192_64_A_0, MASK_192_192_64_A_1, MASK_192_192_64_A_2,
                                      MASK_192_192_64_A_3),
                          mm256_set_4(MASK_192_192_64_B_0, MASK_192_192_64_B_1, MASK_192_192_64_B_2,
                                      MASK_192_192_64_B_3),
                          mm256_set_4(MASK_192_192_64_C_0, MASK_192_192_64_C_1, MASK_192_192_64_C_2,
                                      MASK_192_192_64_C_3));
}

ATTR_TARGET_AVX2
static void mpc_sbox_verify_s256_lowmc_192_192_4(mzd_local_t* out, const mzd_local_t* in,
                                                 view_t* view, const rvec_t* rvec) {
  mpc_sbox_verify_s256_256(out, in, view, rvec,
                           mm256_set_4(MASK_192_192_64_A_0, MASK_192_192_64_A_1,
                                       MASK_192_192_64_A_2, MASK_192_192_64_A_3),
                           mm256_set_4(MASK_192_192_64_B_0, MASK_192_192_64_B_1,
                                       MASK_192_192_64_B_2, MASK_192_192_64_B_3),
                           mm256_set_4(MASK_192_192_64_C_0, MASK_192_192_64_C_1,
                                       MASK_192_192_64_C_2, MASK_192_192_64_C_3));
}
#endif

#if defined(WITH_LOWMC_255_255_4)
ATTR_TARGET_AVX2
static void mpc_sbox_prove_s256_lowmc_255_255_4(mzd_local_t* out, const mzd_local_t* in,
                                                view_t* view, const rvec_t* rvec) {
  mpc_sbox_prove_s256_256(out, in, view, rvec,
                          mm256_set_4(MASK_255_255_85_A_0, MASK_255_255_85_A_1, MASK_255_255_85_A_2,
                                      MASK_255_255_85_A_3),
                          mm256_set_4(MASK_255_255_85_B_0, MASK_255_255_85_B_1, MASK_255_255_85_B_2,
                                      MASK_255_255_85_B_3),
                          mm256_set_4(MASK_255_255_85_C_0, MASK_255_255_85_C_1, MASK_255_255_85_C_2,
                                      MASK_255_255_85_C_3));
}

ATTR_TARGET_AVX2
static void mpc_sbox_verify_s256_lowmc_255_255_4(mzd_local_t* out, const mzd_local_t* in,
                                                 view_t* view, const rvec_t* rvec) {
  mpc_sbox_verify_s256_256(out, in, view, rvec,
                           mm256_set_4(MASK_255_255_85_A_0, MASK_255_255_85_A_1,
                                       MASK_255_255_85_A_2, MASK_255_255_85_A_3),
                           mm256_set_4(MASK_255_255_85_B_0, MASK_255_255_85_B_1,
                                       MASK_255_255_85_B_2, MASK_255_255_85_B_3),
                           mm256_set_4(MASK_255_255_85_C_0, MASK_255_255_85_C_1,
                                       MASK_255_255_85_C_2, MASK_255_255_85_C_3));
}
#endif
#endif /* WITH_AVX2*/
#endif /* WITH_OPT */

/* TODO: get rid of the copies */
#define SBOX(sbox, y, x, views, rvec, n, shares, shares2)                                          \
  {                                                                                                \
    mzd_local_t tmp[shares];                                                                       \
    for (unsigned int count = 0; count < shares; ++count) {                                        \
      memcpy(tmp[count].w64, CONST_BLOCK(x[count], 0)->w64, sizeof(mzd_local_t));                  \
    }                                                                                              \
    sbox(tmp, tmp, views, rvec);                                                                   \
    for (unsigned int count = 0; count < shares; ++count) {                                        \
      memcpy(BLOCK(y[count], 0)->w64, tmp[count].w64, sizeof(mzd_local_t));                        \
    }                                                                                              \
  }                                                                                                \
  while (0)

#define SBOX_uint64(sbox, y, x, views, rvec, n, shares, shares2)                                   \
  do {                                                                                             \
    uint64_t in[shares];                                                                           \
    for (unsigned int count = 0; count < shares; ++count) {                                        \
      in[count] = CONST_BLOCK(x[count], 0)->w64[(n) / (sizeof(word) * 8) - 1];                     \
    }                                                                                              \
    sbox(in, views, rvec->t);                                                                      \
    for (unsigned int count = 0; count < shares2; ++count) {                                       \
      memcpy(BLOCK(y[count], 0)->w64, CONST_BLOCK(x[count], 0)->w64,                               \
             ((n) / (sizeof(word) * 8) - 1) * sizeof(word));                                       \
      BLOCK(y[count], 0)->w64[(n) / (sizeof(word) * 8) - 1] = in[count];                           \
    }                                                                                              \
  } while (0)

#if !defined(NO_UINT64_FALLBACK)
#define IMPL uint64

// uint64 based implementation
#include "lowmc_128_128_20_fns_uint64.h"
#include "mpc_lowmc.c.i"

#include "lowmc_129_129_4_fns_uint64.h"
#include "mpc_lowmc.c.i"

#include "lowmc_192_192_30_fns_uint64.h"
#include "mpc_lowmc.c.i"

#include "lowmc_192_192_4_fns_uint64.h"
#include "mpc_lowmc.c.i"

#include "lowmc_256_256_38_fns_uint64.h"
#include "mpc_lowmc.c.i"

#include "lowmc_255_255_4_fns_uint64.h"
#include "mpc_lowmc.c.i"
#endif

#if defined(WITH_OPT)
#if defined(WITH_SSE2) || defined(WITH_NEON)
#define FN_ATTR ATTR_TARGET_S128
#undef IMPL
#define IMPL s128

// L1 using SSE2/NEON
#include "lowmc_128_128_20_fns_s128.h"
#include "mpc_lowmc.c.i"

#include "lowmc_129_129_4_fns_s128.h"
#include "mpc_lowmc.c.i"

// L3 using SSE2/NEON
#include "lowmc_192_192_30_fns_s128.h"
#include "mpc_lowmc.c.i"

#include "lowmc_192_192_4_fns_s128.h"
#include "mpc_lowmc.c.i"

// L5 using SSE2/NEON
#include "lowmc_256_256_38_fns_s128.h"
#include "mpc_lowmc.c.i"

#include "lowmc_255_255_4_fns_s128.h"
#include "mpc_lowmc.c.i"

#undef FN_ATTR
#endif

#if defined(WITH_AVX2)
#define FN_ATTR ATTR_TARGET_AVX2
#undef IMPL
#define IMPL s256

// L1 using AVX2
#include "lowmc_128_128_20_fns_s256.h"
#include "mpc_lowmc.c.i"

#include "lowmc_129_129_4_fns_s256.h"
#include "mpc_lowmc.c.i"

// L3 using AVX2
#include "lowmc_192_192_30_fns_s256.h"
#include "mpc_lowmc.c.i"

#include "lowmc_192_192_4_fns_s256.h"
#include "mpc_lowmc.c.i"

// L5 using AVX2
#include "lowmc_256_256_38_fns_s256.h"
#include "mpc_lowmc.c.i"

#include "lowmc_255_255_4_fns_s256.h"
#include "mpc_lowmc.c.i"

#undef FN_ATTR
#endif
#endif

zkbpp_lowmc_implementation_f get_zkbpp_lowmc_implementation(const lowmc_parameters_t* lowmc) {
  const uint32_t lowmc_id = lowmc_get_id(lowmc);
#if defined(WITH_OPT)
#if defined(WITH_AVX2)
  /* AVX2 enabled instances */
  if (CPU_SUPPORTS_AVX2) {
    switch (lowmc_id) {
#if defined(WITH_ZKBPP)
      /* Instances with partial Sbox layer */
#if defined(WITH_LOWMC_128_128_20)
    case LOWMC_ID(128, 10):
      return mpc_lowmc_prove_s256_lowmc_128_128_20;
#endif
#if defined(WITH_LOWMC_192_192_30)
    case LOWMC_ID(192, 10):
      return mpc_lowmc_prove_s256_lowmc_192_192_30;
#endif
#if defined(WITH_LOWMC_256_256_38)
    case LOWMC_ID(256, 10):
      return mpc_lowmc_prove_s256_lowmc_256_256_38;
#endif
#endif
      /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
    case LOWMC_ID(129, 43):
      return mpc_lowmc_prove_s256_lowmc_129_129_4;
#endif
#if defined(WITH_LOWMC_192_192_4)
    case LOWMC_ID(192, 64):
      return mpc_lowmc_prove_s256_lowmc_192_192_4;
#endif
#if defined(WITH_LOWMC_255_255_4)
    case LOWMC_ID(255, 85):
      return mpc_lowmc_prove_s256_lowmc_255_255_4;
#endif
    }
  }
#endif

#if defined(WITH_SSE2) || defined(WITH_NEON)
  /* SSE2/NEON enabled instances */
  if (CPU_SUPPORTS_SSE2 || CPU_SUPPORTS_NEON) {
    switch (lowmc_id) {
#if defined(WITH_ZKBPP)
      /* Instances with partial Sbox layer */
#if defined(WITH_LOWMC_128_128_20)
    case LOWMC_ID(128, 10):
      return mpc_lowmc_prove_s128_lowmc_128_128_20;
#endif
#if defined(WITH_LOWMC_192_192_30)
    case LOWMC_ID(192, 10):
      return mpc_lowmc_prove_s128_lowmc_192_192_30;
#endif
#if defined(WITH_LOWMC_256_256_38)
    case LOWMC_ID(256, 10):
      return mpc_lowmc_prove_s128_lowmc_256_256_38;
#endif
#endif
      /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
    case LOWMC_ID(129, 43):
      return mpc_lowmc_prove_s128_lowmc_129_129_4;
#endif
#if defined(WITH_LOWMC_192_192_4)
    case LOWMC_ID(192, 64):
      return mpc_lowmc_prove_s128_lowmc_192_192_4;
#endif
#if defined(WITH_LOWMC_255_255_4)
    case LOWMC_ID(255, 85):
      return mpc_lowmc_prove_s128_lowmc_255_255_4;
#endif
    }
  }
#endif
#endif

#if !defined(NO_UINT64_FALLBACK)
  /* uint64_t implementations */
  switch (lowmc_id) {
#if defined(WITH_ZKBPP)
    /* Instances with partial Sbox layer */
#if defined(WITH_LOWMC_128_128_20)
  case LOWMC_ID(128, 10):
    return mpc_lowmc_prove_uint64_lowmc_128_128_20;
#endif
#if defined(WITH_LOWMC_192_192_30)
  case LOWMC_ID(192, 10):
    return mpc_lowmc_prove_uint64_lowmc_192_192_30;
#endif
#if defined(WITH_LOWMC_256_256_38)
  case LOWMC_ID(256, 10):
    return mpc_lowmc_prove_uint64_lowmc_256_256_38;
#endif
#endif
    /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
  case LOWMC_ID(129, 43):
    return mpc_lowmc_prove_uint64_lowmc_129_129_4;
#endif
#if defined(WITH_LOWMC_192_192_4)
  case LOWMC_ID(192, 64):
    return mpc_lowmc_prove_uint64_lowmc_192_192_4;
#endif
#if defined(WITH_LOWMC_255_255_4)
  case LOWMC_ID(255, 85):
    return mpc_lowmc_prove_uint64_lowmc_255_255_4;
#endif
  }
#endif

  UNREACHABLE;
  return NULL;
}

zkbpp_lowmc_verify_implementation_f
get_zkbpp_lowmc_verify_implementation(const lowmc_parameters_t* lowmc) {
  const uint32_t lowmc_id = lowmc_get_id(lowmc);
#if defined(WITH_OPT)
#if defined(WITH_AVX2)
  /* AVX2 enabled instances */
  if (CPU_SUPPORTS_AVX2) {
    switch (lowmc_id) {
#if defined(WITH_ZKBPP)
      /* Instances with partial Sbox layer */
#if defined(WITH_LOWMC_128_128_20)
    case LOWMC_ID(128, 10):
      return mpc_lowmc_verify_s256_lowmc_128_128_20;
#endif
#if defined(WITH_LOWMC_192_192_30)
    case LOWMC_ID(192, 10):
      return mpc_lowmc_verify_s256_lowmc_192_192_30;
#endif
#if defined(WITH_LOWMC_256_256_38)
    case LOWMC_ID(256, 10):
      return mpc_lowmc_verify_s256_lowmc_256_256_38;
#endif
#endif
      /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
    case LOWMC_ID(129, 43):
      return mpc_lowmc_verify_s256_lowmc_129_129_4;
#endif
#if defined(WITH_LOWMC_192_192_4)
    case LOWMC_ID(192, 64):
      return mpc_lowmc_verify_s256_lowmc_192_192_4;
#endif
#if defined(WITH_LOWMC_255_255_4)
    case LOWMC_ID(255, 85):
      return mpc_lowmc_verify_s256_lowmc_255_255_4;
#endif
    }
  }
#endif

#if defined(WITH_SSE2) || defined(WITH_NEON)
  /* SSE2/NEON enabled instances */
  if (CPU_SUPPORTS_SSE2 || CPU_SUPPORTS_NEON) {
    switch (lowmc_id) {
#if defined(WITH_ZKBPP)
      /* Instances with partial Sbox layer */
#if defined(WITH_LOWMC_128_128_20)
    case LOWMC_ID(128, 10):
      return mpc_lowmc_verify_s128_lowmc_128_128_20;
#endif
#if defined(WITH_LOWMC_192_192_30)
    case LOWMC_ID(192, 10):
      return mpc_lowmc_verify_s128_lowmc_192_192_30;
#endif
#if defined(WITH_LOWMC_256_256_38)
    case LOWMC_ID(256, 10):
      return mpc_lowmc_verify_s128_lowmc_256_256_38;
#endif
#endif
      /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
    case LOWMC_ID(129, 43):
      return mpc_lowmc_verify_s128_lowmc_129_129_4;
#endif
#if defined(WITH_LOWMC_192_192_4)
    case LOWMC_ID(192, 64):
      return mpc_lowmc_verify_s128_lowmc_192_192_4;
#endif
#if defined(WITH_LOWMC_255_255_4)
    case LOWMC_ID(255, 85):
      return mpc_lowmc_verify_s128_lowmc_255_255_4;
#endif
    }
  }
#endif
#endif

#if !defined(NO_UINT64_FALLBACK)
  /* uint64_t implementations */
  switch (lowmc_id) {
#if defined(WITH_ZKBPP)
    /* Instances with partial Sbox layer */
#if defined(WITH_LOWMC_128_128_20)
  case LOWMC_ID(128, 10):
    return mpc_lowmc_verify_uint64_lowmc_128_128_20;
#endif
#if defined(WITH_LOWMC_192_192_30)
  case LOWMC_ID(192, 10):
    return mpc_lowmc_verify_uint64_lowmc_192_192_30;
#endif
#if defined(WITH_LOWMC_256_256_38)
  case LOWMC_ID(256, 10):
    return mpc_lowmc_verify_uint64_lowmc_256_256_38;
#endif
#endif
    /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
  case LOWMC_ID(129, 43):
    return mpc_lowmc_verify_uint64_lowmc_129_129_4;
#endif
#if defined(WITH_LOWMC_192_192_4)
  case LOWMC_ID(192, 64):
    return mpc_lowmc_verify_uint64_lowmc_192_192_4;
#endif
#if defined(WITH_LOWMC_255_255_4)
  case LOWMC_ID(255, 85):
    return mpc_lowmc_verify_uint64_lowmc_255_255_4;
#endif
  }
#endif

  UNREACHABLE;
  return NULL;
}
