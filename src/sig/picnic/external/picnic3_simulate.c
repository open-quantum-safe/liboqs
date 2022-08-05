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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if !defined(_MSC_VER)
#include <stdalign.h>
#endif

#include "compat.h"
#include "bitstream.h"
#include "io.h"
#include "picnic3_simulate.h"
#include "picnic3_types.h"
#if defined(WITH_OPT)
#include "simd.h"
#endif

#define picnic3_mpc_sbox_bitsliced(LOWMC_N, XOR, AND, SHL, SHR, bitmask_a, bitmask_b, bitmask_c)   \
  do {                                                                                             \
    mzd_local_t a[1], b[1], c[1];                                                                  \
    /* a */                                                                                        \
    AND(a, bitmask_a, statein);                                                                    \
    /* b */                                                                                        \
    AND(b, bitmask_b, statein);                                                                    \
    /* c */                                                                                        \
    AND(c, bitmask_c, statein);                                                                    \
                                                                                                   \
    SHL(a, a, 2);                                                                                  \
    SHL(b, b, 1);                                                                                  \
                                                                                                   \
    mzd_local_t t0[1], t1[1], t2[1];                                                               \
                                                                                                   \
    mzd_local_t s_ab[1], s_bc[1], s_ca[1];                                                         \
    /* b & c */                                                                                    \
    AND(s_bc, b, c);                                                                               \
    /* c & a */                                                                                    \
    AND(s_ca, c, a);                                                                               \
    /* a & b */                                                                                    \
    AND(s_ab, a, b);                                                                               \
    for (int i = 0; i < 16; i++) {                                                                 \
      mzd_local_t tmp[1];                                                                          \
      bitstream_t party_msgs = {{msgs->msgs[i]}, msgs->pos};                                       \
      if (i == msgs->unopened) {                                                                   \
        /* we are in verify, just grab the broadcast s from the msgs array */                      \
        mzd_from_bitstream(&party_msgs, tmp, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);    \
        /* a */                                                                                    \
        AND(t0, bitmask_a, tmp);                                                                   \
        /* b */                                                                                    \
        AND(t1, bitmask_b, tmp);                                                                   \
        /* c */                                                                                    \
        AND(t2, bitmask_c, tmp);                                                                   \
        SHL(t0, t0, 2);                                                                            \
        SHL(t1, t1, 1);                                                                            \
        XOR(s_ab, t2, s_ab);                                                                       \
        XOR(s_bc, t1, s_bc);                                                                       \
        XOR(s_ca, t0, s_ca);                                                                       \
                                                                                                   \
        continue;                                                                                  \
      }                                                                                            \
      bitstream_t party_tape = {{tapes->tape[i]}, tapes->pos};                                     \
      /* make a mzd_local from tape[i] for input_masks */                                          \
      mzd_local_t mask_a[1], mask_b[1], mask_c[1];                                                 \
      mzd_from_bitstream(&party_tape, tmp, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);      \
      /* a */                                                                                      \
      AND(mask_a, bitmask_a, tmp);                                                                 \
      /* b */                                                                                      \
      AND(mask_b, bitmask_b, tmp);                                                                 \
      /* c */                                                                                      \
      AND(mask_c, bitmask_c, tmp);                                                                 \
      SHL(mask_a, mask_a, 2);                                                                      \
      SHL(mask_b, mask_b, 1);                                                                      \
                                                                                                   \
      /* make a mzd_local from tape[i] for and_helper */                                           \
      mzd_local_t and_helper_ab[1], and_helper_bc[1], and_helper_ca[1];                            \
      mzd_from_bitstream(&party_tape, tmp, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);      \
      /* a */                                                                                      \
      AND(and_helper_ab, bitmask_c, tmp);                                                          \
      /* b */                                                                                      \
      AND(and_helper_bc, bitmask_b, tmp);                                                          \
      /* c */                                                                                      \
      AND(and_helper_ca, bitmask_a, tmp);                                                          \
      SHL(and_helper_ca, and_helper_ca, 2);                                                        \
      SHL(and_helper_bc, and_helper_bc, 1);                                                        \
                                                                                                   \
      /* s_ab */                                                                                   \
      AND(t0, a, mask_b);                                                                          \
      AND(t1, b, mask_a);                                                                          \
      XOR(t0, t0, t1);                                                                             \
      XOR(tmp, t0, and_helper_ab);                                                                 \
      XOR(s_ab, tmp, s_ab);                                                                        \
      /* s_bc */                                                                                   \
      AND(t0, b, mask_c);                                                                          \
      AND(t1, c, mask_b);                                                                          \
      XOR(t0, t0, t1);                                                                             \
      XOR(t0, t0, and_helper_bc);                                                                  \
      XOR(s_bc, t0, s_bc);                                                                         \
                                                                                                   \
      SHR(t0, t0, 1);                                                                              \
      XOR(tmp, tmp, t0);                                                                           \
      /* s_ca */                                                                                   \
      AND(t0, c, mask_a);                                                                          \
      AND(t1, a, mask_c);                                                                          \
      XOR(t0, t0, t1);                                                                             \
      XOR(t0, t0, and_helper_ca);                                                                  \
      XOR(s_ca, t0, s_ca);                                                                         \
                                                                                                   \
      SHR(t0, t0, 2);                                                                              \
      XOR(tmp, tmp, t0);                                                                           \
      mzd_to_bitstream(&party_msgs, tmp, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);        \
    }                                                                                              \
    tapes->pos += LOWMC_N;                                                                         \
    tapes->pos += LOWMC_N;                                                                         \
    msgs->pos += LOWMC_N;                                                                          \
                                                                                                   \
    /* (b & c) ^ a */                                                                              \
    XOR(t0, s_bc, a);                                                                              \
                                                                                                   \
    /* (c & a) ^ a ^ b */                                                                          \
    XOR(a, a, b);                                                                                  \
    XOR(t1, s_ca, a);                                                                              \
                                                                                                   \
    /* (a & b) ^ a ^ b ^c */                                                                       \
    XOR(t2, s_ab, a);                                                                              \
    XOR(t2, t2, c);                                                                                \
                                                                                                   \
    SHR(t0, t0, 2);                                                                                \
    SHR(t1, t1, 1);                                                                                \
                                                                                                   \
    XOR(t2, t2, t1);                                                                               \
    XOR(statein, t2, t0);                                                                          \
  } while (0)

#if defined(WITH_LOWMC_129_129_4)
#include "lowmc_129_129_4.h"
#endif
#if defined(WITH_LOWMC_192_192_4)
#include "lowmc_192_192_4.h"
#endif
#if defined(WITH_LOWMC_255_255_4)
#include "lowmc_255_255_4.h"
#endif

#if !defined(NO_UINT64_FALLBACK)
#if defined(WITH_LOWMC_129_129_4)
static void picnic3_mpc_sbox_uint64_lowmc_129_129_4(mzd_local_t* statein, randomTape_t* tapes,
                                                    msgs_t* msgs) {

  picnic3_mpc_sbox_bitsliced(LOWMC_129_129_4_N, mzd_xor_uint64_192, mzd_and_uint64_192,
                             mzd_shift_left_uint64_192, mzd_shift_right_uint64_192,
                             mask_129_129_43_a, mask_129_129_43_b, mask_129_129_43_c);
}
#endif

#if defined(WITH_LOWMC_192_192_4)
static void picnic3_mpc_sbox_uint64_lowmc_192_192_4(mzd_local_t* statein, randomTape_t* tapes,
                                                    msgs_t* msgs) {

  picnic3_mpc_sbox_bitsliced(LOWMC_192_192_4_N, mzd_xor_uint64_192, mzd_and_uint64_192,
                             mzd_shift_left_uint64_192, mzd_shift_right_uint64_192,
                             mask_192_192_64_a, mask_192_192_64_b, mask_192_192_64_c);
}
#endif

#if defined(WITH_LOWMC_255_255_4)
static void picnic3_mpc_sbox_uint64_lowmc_255_255_4(mzd_local_t* statein, randomTape_t* tapes,
                                                    msgs_t* msgs) {

  picnic3_mpc_sbox_bitsliced(LOWMC_255_255_4_N, mzd_xor_uint64_256, mzd_and_uint64_256,
                             mzd_shift_left_uint64_256, mzd_shift_right_uint64_256,
                             mask_255_255_85_a, mask_255_255_85_b, mask_255_255_85_c);
}
#endif
#define IMPL uint64
/* PICNIC3_L1_FS */
#include "lowmc_129_129_4_fns_uint64.h"
#undef SIM_ONLINE
#define SIM_ONLINE lowmc_simulate_online_uint64_129_43
#include "picnic3_simulate.c.i"

/* PICNIC3_L3_FS */
#include "lowmc_192_192_4_fns_uint64.h"
#undef SIM_ONLINE
#define SIM_ONLINE lowmc_simulate_online_uint64_192_64
#include "picnic3_simulate.c.i"

/* PICNIC3_L5_FS */
#include "lowmc_255_255_4_fns_uint64.h"
#undef SIM_ONLINE
#define SIM_ONLINE lowmc_simulate_online_uint64_255_85
#include "picnic3_simulate.c.i"
#undef IMPL
#endif

#if defined(WITH_OPT)
#if defined(WITH_SSE2) || defined(WITH_NEON)
#define picnic3_mpc_sbox_bitsliced_mm128(LOWMC_N, XOR, AND, SHL, SHR, bitmask_a, bitmask_b,        \
                                         bitmask_c)                                                \
  do {                                                                                             \
    word128 tmp[2], bitm_a[2], bitm_b[2], bitm_c[2];                                               \
    tmp[0]    = mm128_load(&statein->w64[0]);                                                      \
    tmp[1]    = mm128_load(&statein->w64[2]);                                                      \
    bitm_a[0] = mm128_load(&bitmask_a->w64[0]);                                                    \
    bitm_a[1] = mm128_load(&bitmask_a->w64[2]);                                                    \
    bitm_b[0] = mm128_load(&bitmask_b->w64[0]);                                                    \
    bitm_b[1] = mm128_load(&bitmask_b->w64[2]);                                                    \
    bitm_c[0] = mm128_load(&bitmask_c->w64[0]);                                                    \
    bitm_c[1] = mm128_load(&bitmask_c->w64[2]);                                                    \
                                                                                                   \
    word128 a[2];                                                                                  \
    word128 b[2];                                                                                  \
    word128 c[2];                                                                                  \
    /* a */                                                                                        \
    AND(a, bitm_a, tmp);                                                                           \
    /* b */                                                                                        \
    AND(b, bitm_b, tmp);                                                                           \
    /* c */                                                                                        \
    AND(c, bitm_c, tmp);                                                                           \
                                                                                                   \
    SHL(a, a, 2);                                                                                  \
    SHL(b, b, 1);                                                                                  \
                                                                                                   \
    word128 t0[2];                                                                                 \
    word128 t1[2];                                                                                 \
    word128 t2[2];                                                                                 \
    word128 s_ab[2];                                                                               \
    word128 s_bc[2];                                                                               \
    word128 s_ca[2];                                                                               \
                                                                                                   \
    /* b & c */                                                                                    \
    AND(s_bc, b, c);                                                                               \
    /* c & a */                                                                                    \
    AND(s_ca, c, a);                                                                               \
    /* a & b */                                                                                    \
    AND(s_ab, a, b);                                                                               \
    for (int i = 0; i < 16; i++) {                                                                 \
      bitstream_t party_msgs = {{msgs->msgs[i]}, msgs->pos};                                       \
      if (i == msgs->unopened) {                                                                   \
        /* we are in verify, just grab the broadcast s from the msgs array */                      \
        w128_from_bitstream(&party_msgs, tmp, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);   \
        /* a */                                                                                    \
        AND(t0, bitm_a, tmp);                                                                      \
        /* b */                                                                                    \
        AND(t1, bitm_b, tmp);                                                                      \
        /* c */                                                                                    \
        AND(t2, bitm_c, tmp);                                                                      \
        SHL(t0, t0, 2);                                                                            \
        SHL(t1, t1, 1);                                                                            \
        XOR(s_ab, t2, s_ab);                                                                       \
        XOR(s_bc, t1, s_bc);                                                                       \
        XOR(s_ca, t0, s_ca);                                                                       \
                                                                                                   \
        continue;                                                                                  \
      }                                                                                            \
      bitstream_t party_tape = {{tapes->tape[i]}, tapes->pos};                                     \
      /* make a mzd_local from tape[i] for input_masks */                                          \
      word128 mask_a[2];                                                                           \
      word128 mask_b[2];                                                                           \
      word128 mask_c[2];                                                                           \
      w128_from_bitstream(&party_tape, tmp, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);     \
      /* a */                                                                                      \
      AND(mask_a, bitm_a, tmp);                                                                    \
      /* b */                                                                                      \
      AND(mask_b, bitm_b, tmp);                                                                    \
      /* c */                                                                                      \
      AND(mask_c, bitm_c, tmp);                                                                    \
      SHL(mask_a, mask_a, 2);                                                                      \
      SHL(mask_b, mask_b, 1);                                                                      \
                                                                                                   \
      /* make a mzd_local from tape[i] for and_helper */                                           \
      word128 and_helper_ab[2];                                                                    \
      word128 and_helper_bc[2];                                                                    \
      word128 and_helper_ca[2];                                                                    \
      w128_from_bitstream(&party_tape, tmp, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);     \
      /* a */                                                                                      \
      AND(and_helper_ab, bitm_c, tmp);                                                             \
      /* b */                                                                                      \
      AND(and_helper_bc, bitm_b, tmp);                                                             \
      /* c */                                                                                      \
      AND(and_helper_ca, bitm_a, tmp);                                                             \
      SHL(and_helper_ca, and_helper_ca, 2);                                                        \
      SHL(and_helper_bc, and_helper_bc, 1);                                                        \
                                                                                                   \
      /* s_ab */                                                                                   \
      AND(t0, a, mask_b);                                                                          \
      AND(t1, b, mask_a);                                                                          \
      XOR(t0, t0, t1);                                                                             \
      XOR(tmp, t0, and_helper_ab);                                                                 \
      XOR(s_ab, tmp, s_ab);                                                                        \
      /* s_bc */                                                                                   \
      AND(t0, b, mask_c);                                                                          \
      AND(t1, c, mask_b);                                                                          \
      XOR(t0, t0, t1);                                                                             \
      XOR(t0, t0, and_helper_bc);                                                                  \
      XOR(s_bc, t0, s_bc);                                                                         \
                                                                                                   \
      SHR(t0, t0, 1);                                                                              \
      XOR(tmp, tmp, t0);                                                                           \
      /* s_ca */                                                                                   \
      AND(t0, c, mask_a);                                                                          \
      AND(t1, a, mask_c);                                                                          \
      XOR(t0, t0, t1);                                                                             \
      XOR(t0, t0, and_helper_ca);                                                                  \
      XOR(s_ca, t0, s_ca);                                                                         \
                                                                                                   \
      SHR(t0, t0, 2);                                                                              \
      XOR(tmp, tmp, t0);                                                                           \
      w128_to_bitstream(&party_msgs, tmp, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);       \
    }                                                                                              \
    tapes->pos += LOWMC_N;                                                                         \
    tapes->pos += LOWMC_N;                                                                         \
    msgs->pos += LOWMC_N;                                                                          \
                                                                                                   \
    /* (b & c) ^ a */                                                                              \
    XOR(t0, s_bc, a);                                                                              \
                                                                                                   \
    /* (c & a) ^ a ^ b */                                                                          \
    XOR(a, a, b);                                                                                  \
    XOR(t1, s_ca, a);                                                                              \
                                                                                                   \
    /* (a & b) ^ a ^ b ^c */                                                                       \
    XOR(t2, s_ab, a);                                                                              \
    XOR(t2, t2, c);                                                                                \
                                                                                                   \
    SHR(t0, t0, 2);                                                                                \
    SHR(t1, t1, 1);                                                                                \
                                                                                                   \
    XOR(t2, t2, t1);                                                                               \
    XOR(tmp, t2, t0);                                                                              \
    mm128_store(&statein->w64[0], tmp[0]);                                                         \
    mm128_store(&statein->w64[2], tmp[1]);                                                         \
  } while (0)

#if defined(WITH_LOWMC_129_129_4)
ATTR_TARGET_S128
static void picnic3_mpc_sbox_s128_lowmc_129_129_4(mzd_local_t* statein, randomTape_t* tapes,
                                                  msgs_t* msgs) {
  picnic3_mpc_sbox_bitsliced_mm128(LOWMC_129_129_4_N, mm128_xor_256, mm128_and_256,
                                   mm128_shift_left_256, mm128_shift_right_256, mask_129_129_43_a,
                                   mask_129_129_43_b, mask_129_129_43_c);
}
#endif

#if defined(WITH_LOWMC_192_192_4)
ATTR_TARGET_S128
static void picnic3_mpc_sbox_s128_lowmc_192_192_4(mzd_local_t* statein, randomTape_t* tapes,
                                                  msgs_t* msgs) {
  picnic3_mpc_sbox_bitsliced_mm128(LOWMC_192_192_4_N, mm128_xor_256, mm128_and_256,
                                   mm128_shift_left_256, mm128_shift_right_256, mask_192_192_64_a,
                                   mask_192_192_64_b, mask_192_192_64_c);
}
#endif

#if defined(WITH_LOWMC_255_255_4)
ATTR_TARGET_S128
static void picnic3_mpc_sbox_s128_lowmc_255_255_4(mzd_local_t* statein, randomTape_t* tapes,
                                                  msgs_t* msgs) {
  picnic3_mpc_sbox_bitsliced_mm128(LOWMC_255_255_4_N, mm128_xor_256, mm128_and_256,
                                   mm128_shift_left_256, mm128_shift_right_256, mask_255_255_85_a,
                                   mask_255_255_85_b, mask_255_255_85_c);
}
#endif

#define IMPL s128
#undef FN_ATTR
#define FN_ATTR ATTR_TARGET_S128
/* PICNIC3_L1_FS */
#include "lowmc_129_129_4_fns_s128.h"
#undef SIM_ONLINE
#define SIM_ONLINE lowmc_simulate_online_s128_129_43
#include "picnic3_simulate.c.i"

/* PICNIC3_L3_FS */
#include "lowmc_192_192_4_fns_s128.h"
#undef SIM_ONLINE
#define SIM_ONLINE lowmc_simulate_online_s128_192_64
#include "picnic3_simulate.c.i"

/* PICNIC3_L5_FS */
#include "lowmc_255_255_4_fns_s128.h"
#undef SIM_ONLINE
#define SIM_ONLINE lowmc_simulate_online_s128_255_85
#include "picnic3_simulate.c.i"

#undef IMPL
#endif // SSE/NEON

#if defined(WITH_AVX2)
#define picnic3_mpc_sbox_bitsliced_mm256(LOWMC_N, bitmask_a, bitmask_b, bitmask_c)                 \
  do {                                                                                             \
    const word256 bitm_a = bitmask_a;                                                              \
    const word256 bitm_b = bitmask_b;                                                              \
    const word256 bitm_c = bitmask_c;                                                              \
                                                                                                   \
    word256 tmp = mm256_load(statein->w64);                                                        \
    /* a */                                                                                        \
    word256 a = mm256_and(bitm_a, tmp);                                                            \
    /* b */                                                                                        \
    word256 b = mm256_and(bitm_b, tmp);                                                            \
    /* c */                                                                                        \
    word256 c = mm256_and(bitm_c, tmp);                                                            \
                                                                                                   \
    a = mm256_rotate_left(a, 2);                                                                   \
    b = mm256_rotate_left(b, 1);                                                                   \
                                                                                                   \
    /* b & c */                                                                                    \
    word256 s_bc = mm256_and(b, c);                                                                \
    /* c & a */                                                                                    \
    word256 s_ca = mm256_and(c, a);                                                                \
    /* a & b */                                                                                    \
    word256 s_ab = mm256_and(a, b);                                                                \
    word256 t0, t1, t2;                                                                            \
    for (int i = 0; i < 16; ++i) {                                                                 \
      bitstream_t party_msgs = {{msgs->msgs[i]}, msgs->pos};                                       \
      if (i == msgs->unopened) {                                                                   \
        /* we are in verify, just grab the broadcast s from the msgs array */                      \
        tmp = w256_from_bitstream(&party_msgs, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);  \
        /* a */                                                                                    \
        t0 = mm256_and(bitm_a, tmp);                                                               \
        /* b */                                                                                    \
        t1 = mm256_and(bitm_b, tmp);                                                               \
        /* c */                                                                                    \
        t2   = mm256_and(bitm_c, tmp);                                                             \
        t0   = mm256_rotate_left(t0, 2);                                                           \
        t1   = mm256_rotate_left(t1, 1);                                                           \
        s_ab = mm256_xor(t2, s_ab);                                                                \
        s_bc = mm256_xor(t1, s_bc);                                                                \
        s_ca = mm256_xor(t0, s_ca);                                                                \
                                                                                                   \
        continue;                                                                                  \
      }                                                                                            \
      bitstream_t party_tape = {{tapes->tape[i]}, tapes->pos};                                     \
      /* make a mzd_local from tape[i] for input_masks */                                          \
      tmp = w256_from_bitstream(&party_tape, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);    \
      /* a */                                                                                      \
      word256 mask_a = mm256_and(bitm_a, tmp);                                                     \
      /* b */                                                                                      \
      word256 mask_b = mm256_and(bitm_b, tmp);                                                     \
      /* c */                                                                                      \
      word256 mask_c = mm256_and(bitm_c, tmp);                                                     \
      mask_a         = mm256_rotate_left(mask_a, 2);                                               \
      mask_b         = mm256_rotate_left(mask_b, 1);                                               \
                                                                                                   \
      /* make a mzd_local from tape[i] for and_helper */                                           \
      tmp = w256_from_bitstream(&party_tape, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);    \
      /* a */                                                                                      \
      word256 and_helper_ab = mm256_and(bitm_c, tmp);                                              \
      /* b */                                                                                      \
      word256 and_helper_bc = mm256_and(bitm_b, tmp);                                              \
      /* c */                                                                                      \
      word256 and_helper_ca = mm256_and(bitm_a, tmp);                                              \
      and_helper_ca         = mm256_rotate_left(and_helper_ca, 2);                                 \
      and_helper_bc         = mm256_rotate_left(and_helper_bc, 1);                                 \
                                                                                                   \
      /* s_ab */                                                                                   \
      t0   = mm256_xor(mm256_and(a, mask_b), mm256_and(b, mask_a));                                \
      tmp  = mm256_xor(t0, and_helper_ab);                                                         \
      s_ab = mm256_xor(tmp, s_ab);                                                                 \
      /* s_bc */                                                                                   \
      t0   = mm256_xor(mm256_and(b, mask_c), mm256_and(c, mask_b));                                \
      t0   = mm256_xor(t0, and_helper_bc);                                                         \
      s_bc = mm256_xor(t0, s_bc);                                                                  \
                                                                                                   \
      tmp = mm256_xor(tmp, mm256_rotate_right(t0, 1));                                             \
      /* s_ca */                                                                                   \
      t0   = mm256_xor(mm256_and(c, mask_a), mm256_and(a, mask_c));                                \
      t0   = mm256_xor(t0, and_helper_ca);                                                         \
      s_ca = mm256_xor(t0, s_ca);                                                                  \
                                                                                                   \
      tmp = mm256_xor(tmp, mm256_rotate_right(t0, 2));                                             \
      w256_to_bitstream(&party_msgs, tmp, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);       \
    }                                                                                              \
    tapes->pos += LOWMC_N;                                                                         \
    tapes->pos += LOWMC_N;                                                                         \
    msgs->pos += LOWMC_N;                                                                          \
                                                                                                   \
    /* (b & c) ^ a */                                                                              \
    t0 = mm256_xor(s_bc, a);                                                                       \
                                                                                                   \
    /* (c & a) ^ a ^ b */                                                                          \
    a  = mm256_xor(a, b);                                                                          \
    t1 = mm256_xor(s_ca, a);                                                                       \
                                                                                                   \
    /* (a & b) ^ a ^ b ^c */                                                                       \
    t2 = mm256_xor(mm256_xor(s_ab, a), c);                                                         \
                                                                                                   \
    t0 = mm256_rotate_right(t0, 2);                                                                \
    t1 = mm256_rotate_right(t1, 1);                                                                \
                                                                                                   \
    mm256_store(statein->w64, mm256_xor(mm256_xor(t2, t1), t0));                                   \
  } while (0)

#if defined(WITH_LOWMC_129_129_4)
ATTR_TARGET_AVX2
static void picnic3_mpc_sbox_s256_lowmc_129_129_4(mzd_local_t* statein, randomTape_t* tapes,
                                                  msgs_t* msgs) {
  picnic3_mpc_sbox_bitsliced_mm256(LOWMC_129_129_4_N,
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
static void picnic3_mpc_sbox_s256_lowmc_192_192_4(mzd_local_t* statein, randomTape_t* tapes,
                                                  msgs_t* msgs) {
  picnic3_mpc_sbox_bitsliced_mm256(LOWMC_192_192_4_N,
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
static void picnic3_mpc_sbox_s256_lowmc_255_255_4(mzd_local_t* statein, randomTape_t* tapes,
                                                  msgs_t* msgs) {
  picnic3_mpc_sbox_bitsliced_mm256(LOWMC_255_255_4_N,
                                   mm256_set_4(MASK_255_255_85_A_0, MASK_255_255_85_A_1,
                                               MASK_255_255_85_A_2, MASK_255_255_85_A_3),
                                   mm256_set_4(MASK_255_255_85_B_0, MASK_255_255_85_B_1,
                                               MASK_255_255_85_B_2, MASK_255_255_85_B_3),
                                   mm256_set_4(MASK_255_255_85_C_0, MASK_255_255_85_C_1,
                                               MASK_255_255_85_C_2, MASK_255_255_85_C_3));
}
#endif

#define IMPL s256
#undef FN_ATTR
#define FN_ATTR ATTR_TARGET_AVX2
/* PICNIC3_L1_FS */
#include "lowmc_129_129_4_fns_s256.h"
#undef SIM_ONLINE
#define SIM_ONLINE lowmc_simulate_online_s256_129_43
#include "picnic3_simulate.c.i"

/* PICNIC3_L3_FS */
#include "lowmc_192_192_4_fns_s256.h"
#undef SIM_ONLINE
#define SIM_ONLINE lowmc_simulate_online_s256_192_64
#include "picnic3_simulate.c.i"

/* PICNIC3_L5_FS */
#include "lowmc_255_255_4_fns_s256.h"
#undef SIM_ONLINE
#define SIM_ONLINE lowmc_simulate_online_s256_255_85
#include "picnic3_simulate.c.i"

#undef IMPL
#endif // AVX2
#endif // WITH_OPT

lowmc_simulate_online_f lowmc_simulate_online_get_implementation(const lowmc_parameters_t* lowmc) {
  const uint32_t lowmc_id = lowmc_get_id(lowmc);

#if defined(WITH_OPT)
#if defined(WITH_AVX2)
  if (CPU_SUPPORTS_AVX2) {
    switch (lowmc_id) {
#if defined(WITH_LOWMC_129_129_4)
    case LOWMC_ID(129, 43):
      return lowmc_simulate_online_s256_129_43;
#endif
#if defined(WITH_LOWMC_192_192_4)
    case LOWMC_ID(192, 64):
      return lowmc_simulate_online_s256_192_64;
#endif
#if defined(WITH_LOWMC_255_255_4)
    case LOWMC_ID(255, 85):
      return lowmc_simulate_online_s256_255_85;
#endif
    }
  }
#endif

#if defined(WITH_SSE2) || defined(WITH_NEON)
  if (CPU_SUPPORTS_SSE2 || CPU_SUPPORTS_NEON) {
    switch (lowmc_id) {
#if defined(WITH_LOWMC_129_129_4)
    case LOWMC_ID(129, 43):
      return lowmc_simulate_online_s128_129_43;
#endif
#if defined(WITH_LOWMC_192_192_4)
    case LOWMC_ID(192, 64):
      return lowmc_simulate_online_s128_192_64;
#endif
#if defined(WITH_LOWMC_255_255_4)
    case LOWMC_ID(255, 85):
      return lowmc_simulate_online_s128_255_85;
#endif
    }
  }
#endif
#endif

#if !defined(NO_UINT64_FALLBACK)
  switch (lowmc_id) {
#if defined(WITH_LOWMC_129_129_4)
  case LOWMC_ID(129, 43):
    return lowmc_simulate_online_uint64_129_43;
#endif
#if defined(WITH_LOWMC_192_192_4)
  case LOWMC_ID(192, 64):
    return lowmc_simulate_online_uint64_192_64;
#endif
#if defined(WITH_LOWMC_255_255_4)
  case LOWMC_ID(255, 85):
    return lowmc_simulate_online_uint64_255_85;
#endif
  }
#endif

  UNREACHABLE;
  return NULL;
}
