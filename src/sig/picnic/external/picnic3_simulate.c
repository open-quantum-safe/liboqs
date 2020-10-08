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
    word128 a[2] ATTR_ALIGNED(alignof(word128));                                                   \
    word128 b[2] ATTR_ALIGNED(alignof(word128));                                                   \
    word128 c[2] ATTR_ALIGNED(alignof(word128));                                                   \
    /* a */                                                                                        \
    AND(a, bitmask_a->w128, statein->w128);                                                        \
    /* b */                                                                                        \
    AND(b, bitmask_b->w128, statein->w128);                                                        \
    /* c */                                                                                        \
    AND(c, bitmask_c->w128, statein->w128);                                                        \
                                                                                                   \
    SHL(a, a, 2);                                                                                  \
    SHL(b, b, 1);                                                                                  \
                                                                                                   \
    word128 t0[2] ATTR_ALIGNED(alignof(word128));                                                  \
    word128 t1[2] ATTR_ALIGNED(alignof(word128));                                                  \
    word128 t2[2] ATTR_ALIGNED(alignof(word128));                                                  \
    word128 s_ab[2] ATTR_ALIGNED(alignof(word128));                                                \
    word128 s_bc[2] ATTR_ALIGNED(alignof(word128));                                                \
    word128 s_ca[2] ATTR_ALIGNED(alignof(word128));                                                \
                                                                                                   \
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
        AND(t0, bitmask_a->w128, tmp->w128);                                                       \
        /* b */                                                                                    \
        AND(t1, bitmask_b->w128, tmp->w128);                                                       \
        /* c */                                                                                    \
        AND(t2, bitmask_c->w128, tmp->w128);                                                       \
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
      word128 mask_a[2] ATTR_ALIGNED(alignof(word128));                                            \
      word128 mask_b[2] ATTR_ALIGNED(alignof(word128));                                            \
      word128 mask_c[2] ATTR_ALIGNED(alignof(word128));                                            \
      mzd_from_bitstream(&party_tape, tmp, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);      \
      /* a */                                                                                      \
      AND(mask_a, bitmask_a->w128, tmp->w128);                                                     \
      /* b */                                                                                      \
      AND(mask_b, bitmask_b->w128, tmp->w128);                                                     \
      /* c */                                                                                      \
      AND(mask_c, bitmask_c->w128, tmp->w128);                                                     \
      SHL(mask_a, mask_a, 2);                                                                      \
      SHL(mask_b, mask_b, 1);                                                                      \
                                                                                                   \
      /* make a mzd_local from tape[i] for and_helper */                                           \
      word128 and_helper_ab[2] ATTR_ALIGNED(alignof(word128));                                     \
      word128 and_helper_bc[2] ATTR_ALIGNED(alignof(word128));                                     \
      word128 and_helper_ca[2] ATTR_ALIGNED(alignof(word128));                                     \
      mzd_from_bitstream(&party_tape, tmp, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);      \
      /* a */                                                                                      \
      AND(and_helper_ab, bitmask_c->w128, tmp->w128);                                              \
      /* b */                                                                                      \
      AND(and_helper_bc, bitmask_b->w128, tmp->w128);                                              \
      /* c */                                                                                      \
      AND(and_helper_ca, bitmask_a->w128, tmp->w128);                                              \
      SHL(and_helper_ca, and_helper_ca, 2);                                                        \
      SHL(and_helper_bc, and_helper_bc, 1);                                                        \
                                                                                                   \
      /* s_ab */                                                                                   \
      AND(t0, a, mask_b);                                                                          \
      AND(t1, b, mask_a);                                                                          \
      XOR(t0, t0, t1);                                                                             \
      XOR(tmp->w128, t0, and_helper_ab);                                                           \
      XOR(s_ab, tmp->w128, s_ab);                                                                  \
      /* s_bc */                                                                                   \
      AND(t0, b, mask_c);                                                                          \
      AND(t1, c, mask_b);                                                                          \
      XOR(t0, t0, t1);                                                                             \
      XOR(t0, t0, and_helper_bc);                                                                  \
      XOR(s_bc, t0, s_bc);                                                                         \
                                                                                                   \
      SHR(t0, t0, 1);                                                                              \
      XOR(tmp->w128, tmp->w128, t0);                                                               \
      /* s_ca */                                                                                   \
      AND(t0, c, mask_a);                                                                          \
      AND(t1, a, mask_c);                                                                          \
      XOR(t0, t0, t1);                                                                             \
      XOR(t0, t0, and_helper_ca);                                                                  \
      XOR(s_ca, t0, s_ca);                                                                         \
                                                                                                   \
      SHR(t0, t0, 2);                                                                              \
      XOR(tmp->w128, tmp->w128, t0);                                                               \
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
    XOR(statein->w128, t2, t0);                                                                    \
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
#define picnic3_mpc_sbox_bitsliced_mm256(LOWMC_N, XOR, AND, ROL, ROR, bitmask_a, bitmask_b,        \
                                         bitmask_c)                                                \
  do {                                                                                             \
    word256 a ATTR_ALIGNED(alignof(word256));                                                      \
    word256 b ATTR_ALIGNED(alignof(word256));                                                      \
    word256 c ATTR_ALIGNED(alignof(word256));                                                      \
    /* a */                                                                                        \
    a = AND(bitmask_a->w256, statein->w256);                                                       \
    /* b */                                                                                        \
    b = AND(bitmask_b->w256, statein->w256);                                                       \
    /* c */                                                                                        \
    c = AND(bitmask_c->w256, statein->w256);                                                       \
                                                                                                   \
    a = ROL(a, 2);                                                                                 \
    b = ROL(b, 1);                                                                                 \
                                                                                                   \
    word256 t0 ATTR_ALIGNED(alignof(word256));                                                     \
    word256 t1 ATTR_ALIGNED(alignof(word256));                                                     \
    word256 t2 ATTR_ALIGNED(alignof(word256));                                                     \
    word256 s_ab ATTR_ALIGNED(alignof(word256));                                                   \
    word256 s_bc ATTR_ALIGNED(alignof(word256));                                                   \
    word256 s_ca ATTR_ALIGNED(alignof(word256));                                                   \
                                                                                                   \
    /* b & c */                                                                                    \
    s_bc = AND(b, c);                                                                              \
    /* c & a */                                                                                    \
    s_ca = AND(c, a);                                                                              \
    /* a & b */                                                                                    \
    s_ab = AND(a, b);                                                                              \
    for (int i = 0; i < 16; i++) {                                                                 \
      mzd_local_t tmp[1];                                                                          \
      bitstream_t party_msgs = {{msgs->msgs[i]}, msgs->pos};                                       \
      if (i == msgs->unopened) {                                                                   \
        /* we are in verify, just grab the broadcast s from the msgs array */                      \
        mzd_from_bitstream(&party_msgs, tmp, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);    \
        /* a */                                                                                    \
        t0 = AND(bitmask_a->w256, tmp->w256);                                                      \
        /* b */                                                                                    \
        t1 = AND(bitmask_b->w256, tmp->w256);                                                      \
        /* c */                                                                                    \
        t2   = AND(bitmask_c->w256, tmp->w256);                                                    \
        t0   = ROL(t0, 2);                                                                         \
        t1   = ROL(t1, 1);                                                                         \
        s_ab = XOR(t2, s_ab);                                                                      \
        s_bc = XOR(t1, s_bc);                                                                      \
        s_ca = XOR(t0, s_ca);                                                                      \
                                                                                                   \
        continue;                                                                                  \
      }                                                                                            \
      bitstream_t party_tape = {{tapes->tape[i]}, tapes->pos};                                     \
      /* make a mzd_local from tape[i] for input_masks */                                          \
      word256 mask_a ATTR_ALIGNED(alignof(word256));                                               \
      word256 mask_b ATTR_ALIGNED(alignof(word256));                                               \
      word256 mask_c ATTR_ALIGNED(alignof(word256));                                               \
      mzd_from_bitstream(&party_tape, tmp, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);      \
      /* a */                                                                                      \
      mask_a = AND(bitmask_a->w256, tmp->w256);                                                    \
      /* b */                                                                                      \
      mask_b = AND(bitmask_b->w256, tmp->w256);                                                    \
      /* c */                                                                                      \
      mask_c = AND(bitmask_c->w256, tmp->w256);                                                    \
      mask_a = ROL(mask_a, 2);                                                                     \
      mask_b = ROL(mask_b, 1);                                                                     \
                                                                                                   \
      /* make a mzd_local from tape[i] for and_helper */                                           \
      word256 and_helper_ab ATTR_ALIGNED(alignof(word256));                                        \
      word256 and_helper_bc ATTR_ALIGNED(alignof(word256));                                        \
      word256 and_helper_ca ATTR_ALIGNED(alignof(word256));                                        \
      mzd_from_bitstream(&party_tape, tmp, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);      \
      /* a */                                                                                      \
      and_helper_ab = AND(bitmask_c->w256, tmp->w256);                                             \
      /* b */                                                                                      \
      and_helper_bc = AND(bitmask_b->w256, tmp->w256);                                             \
      /* c */                                                                                      \
      and_helper_ca = AND(bitmask_a->w256, tmp->w256);                                             \
      and_helper_ca = ROL(and_helper_ca, 2);                                                       \
      and_helper_bc = ROL(and_helper_bc, 1);                                                       \
                                                                                                   \
      /* s_ab */                                                                                   \
      t0        = AND(a, mask_b);                                                                  \
      t1        = AND(b, mask_a);                                                                  \
      t0        = XOR(t0, t1);                                                                     \
      tmp->w256 = XOR(t0, and_helper_ab);                                                          \
      s_ab      = XOR(tmp->w256, s_ab);                                                            \
      /* s_bc */                                                                                   \
      t0   = AND(b, mask_c);                                                                       \
      t1   = AND(c, mask_b);                                                                       \
      t0   = XOR(t0, t1);                                                                          \
      t0   = XOR(t0, and_helper_bc);                                                               \
      s_bc = XOR(t0, s_bc);                                                                        \
                                                                                                   \
      t0        = ROR(t0, 1);                                                                      \
      tmp->w256 = XOR(tmp->w256, t0);                                                              \
      /* s_ca */                                                                                   \
      t0   = AND(c, mask_a);                                                                       \
      t1   = AND(a, mask_c);                                                                       \
      t0   = XOR(t0, t1);                                                                          \
      t0   = XOR(t0, and_helper_ca);                                                               \
      s_ca = XOR(t0, s_ca);                                                                        \
                                                                                                   \
      t0        = ROR(t0, 2);                                                                      \
      tmp->w256 = XOR(tmp->w256, t0);                                                              \
      mzd_to_bitstream(&party_msgs, tmp, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);        \
    }                                                                                              \
    tapes->pos += LOWMC_N;                                                                         \
    tapes->pos += LOWMC_N;                                                                         \
    msgs->pos += LOWMC_N;                                                                          \
                                                                                                   \
    /* (b & c) ^ a */                                                                              \
    t0 = XOR(s_bc, a);                                                                             \
                                                                                                   \
    /* (c & a) ^ a ^ b */                                                                          \
    a  = XOR(a, b);                                                                                \
    t1 = XOR(s_ca, a);                                                                             \
                                                                                                   \
    /* (a & b) ^ a ^ b ^c */                                                                       \
    t2 = XOR(s_ab, a);                                                                             \
    t2 = XOR(t2, c);                                                                               \
                                                                                                   \
    t0 = ROR(t0, 2);                                                                               \
    t1 = ROR(t1, 1);                                                                               \
                                                                                                   \
    t2            = XOR(t2, t1);                                                                   \
    statein->w256 = XOR(t2, t0);                                                                   \
  } while (0)

#if defined(WITH_LOWMC_129_129_4)
ATTR_TARGET_AVX2
static void picnic3_mpc_sbox_s256_lowmc_129_129_4(mzd_local_t* statein, randomTape_t* tapes,
                                                  msgs_t* msgs) {
  picnic3_mpc_sbox_bitsliced_mm256(LOWMC_129_129_4_N, mm256_xor, mm256_and, mm256_rotate_left,
                                   mm256_rotate_right, mask_129_129_43_a, mask_129_129_43_b,
                                   mask_129_129_43_c);
}
#endif

#if defined(WITH_LOWMC_192_192_4)
ATTR_TARGET_AVX2
static void picnic3_mpc_sbox_s256_lowmc_192_192_4(mzd_local_t* statein, randomTape_t* tapes,
                                                  msgs_t* msgs) {
  picnic3_mpc_sbox_bitsliced_mm256(LOWMC_192_192_4_N, mm256_xor, mm256_and, mm256_rotate_left,
                                   mm256_rotate_right, mask_192_192_64_a, mask_192_192_64_b,
                                   mask_192_192_64_c);
}
#endif

#if defined(WITH_LOWMC_255_255_4)
ATTR_TARGET_AVX2
static void picnic3_mpc_sbox_s256_lowmc_255_255_4(mzd_local_t* statein, randomTape_t* tapes,
                                                  msgs_t* msgs) {
  picnic3_mpc_sbox_bitsliced_mm256(LOWMC_255_255_4_N, mm256_xor, mm256_and, mm256_rotate_left,
                                   mm256_rotate_right, mask_255_255_85_a, mask_255_255_85_b,
                                   mask_255_255_85_c);
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
  assert((lowmc->m == 43 && lowmc->n == 129) || (lowmc->m == 64 && lowmc->n == 192) ||
         (lowmc->m == 85 && lowmc->n == 255));

#if defined(WITH_OPT)
#if defined(WITH_AVX2)
  if (CPU_SUPPORTS_AVX2) {
#if defined(WITH_LOWMC_129_129_4)
    if (lowmc->n == 129 && lowmc->m == 43)
      return lowmc_simulate_online_s256_129_43;
#endif
#if defined(WITH_LOWMC_192_192_4)
    if (lowmc->n == 192 && lowmc->m == 64)
      return lowmc_simulate_online_s256_192_64;
#endif
#if defined(WITH_LOWMC_255_255_4)
    if (lowmc->n == 255 && lowmc->m == 85)
      return lowmc_simulate_online_s256_255_85;
#endif
  }
#endif

#if defined(WITH_SSE2) || defined(WITH_NEON)
  if (CPU_SUPPORTS_SSE2 || CPU_SUPPORTS_NEON) {
#if defined(WITH_LOWMC_129_129_4)
    if (lowmc->n == 129 && lowmc->m == 43)
      return lowmc_simulate_online_s128_129_43;
#endif
#if defined(WITH_LOWMC_192_192_4)
    if (lowmc->n == 192 && lowmc->m == 64)
      return lowmc_simulate_online_s128_192_64;
#endif
#if defined(WITH_LOWMC_255_255_4)
    if (lowmc->n == 255 && lowmc->m == 85)
      return lowmc_simulate_online_s128_255_85;
#endif
  }
#endif
#endif

#if !defined(NO_UINT64_FALLBACK)
#if defined(WITH_LOWMC_129_129_4)
  if (lowmc->n == 129 && lowmc->m == 43)
    return lowmc_simulate_online_uint64_129_43;
#endif
#if defined(WITH_LOWMC_192_192_4)
  if (lowmc->n == 192 && lowmc->m == 64)
    return lowmc_simulate_online_uint64_192_64;
#endif
#if defined(WITH_LOWMC_255_255_4)
  if (lowmc->n == 255 && lowmc->m == 85)
    return lowmc_simulate_online_uint64_255_85;
#endif
#endif

  return NULL;
}
