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

#include "io.h"
#include "lowmc.h"
#include "mzd_additional.h"
#if defined(WITH_KKW)
#include "bitstream.h"
#include "picnic3_impl.h"
#include "picnic3_types.h"
#endif
#if defined(WITH_OPT)
#include "simd.h"
#endif

#if !defined(_MSC_VER)
#include <stdalign.h>
#endif
#include <string.h>
#include <assert.h>

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

#if defined(WITH_LOWMC_128_128_20) || defined(WITH_LOWMC_192_192_30) || defined(WITH_LOWMC_256_256_38)
/* S-box for m = 10 */
static inline uint64_t sbox_layer_10_bitsliced_uint64(uint64_t in) {
  // a, b, c
  const uint64_t x0s = (in & MASK_X0I) << 2;
  const uint64_t x1s = (in & MASK_X1I) << 1;
  const uint64_t x2m = in & MASK_X2I;

  // (b & c) ^ a
  const uint64_t t0 = (x1s & x2m) ^ x0s;
  // (c & a) ^ a ^ b
  const uint64_t t1 = (x0s & x2m) ^ x0s ^ x1s;
  // (a & b) ^ a ^ b ^c
  const uint64_t t2 = (x0s & x1s) ^ x0s ^ x1s ^ x2m;

  return (in & MASK_MASK) ^ (t0 >> 2) ^ (t1 >> 1) ^ t2;
}

/* S-box for m = 10 */
static inline void sbox_layer_10_uint64(uint64_t* d) {
  *d = sbox_layer_10_bitsliced_uint64(*d);
}
#endif

#if !defined(NO_UINT64_FALLBACK)
#if defined(WITH_LOWMC_129_129_4)
/**
 * S-box for m = 43
 */
static void sbox_uint64_lowmc_129_129_4(mzd_local_t* in) {
  mzd_local_t x0m[1], x1m[1], x2m[1];
  // a
  mzd_and_uint64_192(x0m, mask_129_129_43_a, in);
  // b
  mzd_and_uint64_192(x1m, mask_129_129_43_b, in);
  // c
  mzd_and_uint64_192(x2m, mask_129_129_43_c, in);

  mzd_shift_left_uint64_192(x0m, x0m, 2);
  mzd_shift_left_uint64_192(x1m, x1m, 1);

  mzd_local_t t0[1], t1[1], t2[1];
  // b & c
  mzd_and_uint64_192(t0, x1m, x2m);
  // c & a
  mzd_and_uint64_192(t1, x0m, x2m);
  // a & b
  mzd_and_uint64_192(t2, x0m, x1m);

  // (b & c) ^ a
  mzd_xor_uint64_192(t0, t0, x0m);

  // (c & a) ^ a ^ b
  mzd_xor_uint64_192(t1, t1, x0m);
  mzd_xor_uint64_192(t1, t1, x1m);

  // (a & b) ^ a ^ b ^c
  mzd_xor_uint64_192(t2, t2, x0m);
  mzd_xor_uint64_192(t2, t2, x1m);
  mzd_xor_uint64_192(t2, t2, x2m);

  mzd_shift_right_uint64_192(t0, t0, 2);
  mzd_shift_right_uint64_192(t1, t1, 1);

  mzd_xor_uint64_192(t2, t2, t1);
  mzd_xor_uint64_192(in, t2, t0);
}
#endif

#if defined(WITH_LOWMC_192_192_4)
/**
 * S-box for m = 64
 */
static void sbox_uint64_lowmc_192_192_4(mzd_local_t* in) {
  mzd_local_t x0m[1], x1m[1], x2m[1];
  // a
  mzd_and_uint64_192(x0m, mask_192_192_64_a, in);
  // b
  mzd_and_uint64_192(x1m, mask_192_192_64_b, in);
  // c
  mzd_and_uint64_192(x2m, mask_192_192_64_c, in);

  mzd_shift_left_uint64_192(x0m, x0m, 2);
  mzd_shift_left_uint64_192(x1m, x1m, 1);

  mzd_local_t t0[1], t1[1], t2[1];
  // b & c
  mzd_and_uint64_192(t0, x1m, x2m);
  // c & a
  mzd_and_uint64_192(t1, x0m, x2m);
  // a & b
  mzd_and_uint64_192(t2, x0m, x1m);

  // (b & c) ^ a
  mzd_xor_uint64_192(t0, t0, x0m);

  // (c & a) ^ a ^ b
  mzd_xor_uint64_192(t1, t1, x0m);
  mzd_xor_uint64_192(t1, t1, x1m);

  // (a & b) ^ a ^ b ^c
  mzd_xor_uint64_192(t2, t2, x0m);
  mzd_xor_uint64_192(t2, t2, x1m);
  mzd_xor_uint64_192(t2, t2, x2m);

  mzd_shift_right_uint64_192(t0, t0, 2);
  mzd_shift_right_uint64_192(t1, t1, 1);

  mzd_xor_uint64_192(t2, t2, t1);
  mzd_xor_uint64_192(in, t2, t0);
}
#endif

#if defined(WITH_LOWMC_255_255_4)
/**
 * S-box for m = 85
 */
static void sbox_uint64_lowmc_255_255_4(mzd_local_t* in) {
  mzd_local_t x0m[1], x1m[1], x2m[1];
  // a
  mzd_and_uint64_256(x0m, mask_255_255_85_a, in);
  // b
  mzd_and_uint64_256(x1m, mask_255_255_85_b, in);
  // c
  mzd_and_uint64_256(x2m, mask_255_255_85_c, in);

  mzd_shift_left_uint64_256(x0m, x0m, 2);
  mzd_shift_left_uint64_256(x1m, x1m, 1);

  mzd_local_t t0[1], t1[1], t2[1];
  // b & c
  mzd_and_uint64_256(t0, x1m, x2m);
  // c & a
  mzd_and_uint64_256(t1, x0m, x2m);
  // a & b
  mzd_and_uint64_256(t2, x0m, x1m);

  // (b & c) ^ a
  mzd_xor_uint64_256(t0, t0, x0m);

  // (c & a) ^ a ^ b
  mzd_xor_uint64_256(t1, t1, x0m);
  mzd_xor_uint64_256(t1, t1, x1m);

  // (a & b) ^ a ^ b ^c
  mzd_xor_uint64_256(t2, t2, x0m);
  mzd_xor_uint64_256(t2, t2, x1m);
  mzd_xor_uint64_256(t2, t2, x2m);

  mzd_shift_right_uint64_256(t0, t0, 2);
  mzd_shift_right_uint64_256(t1, t1, 1);

  mzd_xor_uint64_256(t2, t2, t1);
  mzd_xor_uint64_256(in, t2, t0);
}
#endif
#endif /* NO_UINT_FALLBACK */

#if defined(WITH_OPT)
#if defined(WITH_SSE2) || defined(WITH_NEON)
ATTR_TARGET_S128
static inline void sbox_s128_full(mzd_local_t* in, const word128* mask_a, const word128* mask_b,
                                  const word128* mask_c) {
  word128 x0m[2] ATTR_ALIGNED(alignof(word128)), x1m[2] ATTR_ALIGNED(alignof(word128)),
      x2m[2] ATTR_ALIGNED(alignof(word128));
  mm128_and_256(x0m, CONST_BLOCK(in, 0)->w128, mask_a);
  mm128_and_256(x1m, CONST_BLOCK(in, 0)->w128, mask_b);
  mm128_and_256(x2m, CONST_BLOCK(in, 0)->w128, mask_c);

  mm128_shift_left_256(x0m, x0m, 2);
  mm128_shift_left_256(x1m, x1m, 1);

  word128 t0[2] ATTR_ALIGNED(alignof(word128)), t1[2] ATTR_ALIGNED(alignof(word128)),
      t2[2] ATTR_ALIGNED(alignof(word128));
  mm128_and_256(t0, x1m, x2m);
  mm128_and_256(t1, x0m, x2m);
  mm128_and_256(t2, x0m, x1m);

  mm128_xor_256(t0, t0, x0m);

  mm128_xor_256(x0m, x0m, x1m);
  mm128_xor_256(t1, t1, x0m);

  mm128_xor_256(t2, t2, x0m);
  mm128_xor_256(t2, t2, x2m);

  mm128_shift_right_256(t0, t0, 2);
  mm128_shift_right_256(t1, t1, 1);

  mm128_xor_256(t0, t0, t1);
  mm128_xor_256(in->w128, t0, t2);
}

#if defined(WITH_LOWMC_129_129_4)
ATTR_TARGET_S128
static inline void sbox_s128_lowmc_129_129_4(mzd_local_t* in) {
  sbox_s128_full(in, mask_129_129_43_a->w128, mask_129_129_43_b->w128, mask_129_129_43_c->w128);
}
#endif

#if defined(WITH_LOWMC_192_192_4)
ATTR_TARGET_S128
static inline void sbox_s128_lowmc_192_192_4(mzd_local_t* in) {
  sbox_s128_full(in, mask_192_192_64_a->w128, mask_192_192_64_b->w128, mask_192_192_64_c->w128);
}
#endif

#if defined(WITH_LOWMC_255_255_4)
ATTR_TARGET_S128
static inline void sbox_s128_lowmc_255_255_4(mzd_local_t* in) {
  sbox_s128_full(in, mask_255_255_85_a->w128, mask_255_255_85_b->w128, mask_255_255_85_c->w128);
}
#endif
#endif

#if defined(WITH_AVX2)
ATTR_TARGET_AVX2
static inline word256 sbox_s256_lowmc_full(const word256 min, const word256 mask_a,
                                           const word256 mask_b, const word256 mask_c) {
  word256 x0m ATTR_ALIGNED(alignof(word256)) = mm256_and(min, mask_a);
  word256 x1m ATTR_ALIGNED(alignof(word256)) = mm256_and(min, mask_b);
  word256 x2m ATTR_ALIGNED(alignof(word256)) = mm256_and(min, mask_c);

  x0m = mm256_rotate_left(x0m, 2);
  x1m = mm256_rotate_left(x1m, 1);

  word256 t0 ATTR_ALIGNED(alignof(word256)) = mm256_and(x1m, x2m);
  word256 t1 ATTR_ALIGNED(alignof(word256)) = mm256_and(x0m, x2m);
  word256 t2 ATTR_ALIGNED(alignof(word256)) = mm256_and(x0m, x1m);

  t0 = mm256_xor(t0, x0m);

  x0m = mm256_xor(x0m, x1m);
  t1  = mm256_xor(t1, x0m);

  t2 = mm256_xor(t2, x0m);
  t2 = mm256_xor(t2, x2m);

  t0 = mm256_rotate_right(t0, 2);
  t1 = mm256_rotate_right(t1, 1);

  return mm256_xor(mm256_xor(t0, t1), t2);
}

#if defined(WITH_LOWMC_129_129_4)
ATTR_TARGET_AVX2
static inline void sbox_s256_lowmc_129_129_4(mzd_local_t* in) {
  BLOCK(in, 0)->w256 = sbox_s256_lowmc_full(
      BLOCK(in, 0)->w256, CONST_BLOCK(mask_129_129_43_a, 0)->w256,
      CONST_BLOCK(mask_129_129_43_b, 0)->w256, CONST_BLOCK(mask_129_129_43_c, 0)->w256);
}
#endif

#if defined(WITH_LOWMC_192_192_4)
ATTR_TARGET_AVX2
static inline void sbox_s256_lowmc_192_192_4(mzd_local_t* in) {
  BLOCK(in, 0)->w256 = sbox_s256_lowmc_full(
      BLOCK(in, 0)->w256, CONST_BLOCK(mask_192_192_64_a, 0)->w256,
      CONST_BLOCK(mask_192_192_64_b, 0)->w256, CONST_BLOCK(mask_192_192_64_c, 0)->w256);
}
#endif

#if defined(WITH_LOWMC_255_255_4)
ATTR_TARGET_AVX2
static inline void sbox_s256_lowmc_255_255_4(mzd_local_t* in) {
  BLOCK(in, 0)->w256 = sbox_s256_lowmc_full(
      BLOCK(in, 0)->w256, CONST_BLOCK(mask_255_255_85_a, 0)->w256,
      CONST_BLOCK(mask_255_255_85_b, 0)->w256, CONST_BLOCK(mask_255_255_85_c, 0)->w256);
}
#endif
#endif /* WITH_AVX2 */
#endif /* WITH_OPT */

#if defined(WITH_KKW)
#if !defined(NO_UINT64_FALLBACK)
#define picnic3_aux_sbox_bitsliced(LOWMC_N, XOR, AND, SHL, SHR, bitmask_a, bitmask_b, bitmask_c)   \
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
    mzd_local_t d[1], e[1], f[1];                                                                  \
    /* a */                                                                                        \
    AND(d, bitmask_a, stateout);                                                                   \
    /* b */                                                                                        \
    AND(e, bitmask_b, stateout);                                                                   \
    /* c */                                                                                        \
    AND(f, bitmask_c, stateout);                                                                   \
                                                                                                   \
    SHL(d, d, 2);                                                                                  \
    SHL(e, e, 1);                                                                                  \
                                                                                                   \
    mzd_local_t fresh_output_ab[1], fresh_output_bc[1], fresh_output_ca[1];                        \
    XOR(fresh_output_ab, a, b);                                                                    \
    XOR(fresh_output_ca, e, fresh_output_ab);                                                      \
    XOR(fresh_output_bc, d, a);                                                                    \
    XOR(fresh_output_ab, fresh_output_ab, c);                                                      \
    XOR(fresh_output_ab, fresh_output_ab, f);                                                      \
                                                                                                   \
    mzd_local_t t0[1], t1[1], t2[1], aux[1];                                                       \
    SHR(t2, fresh_output_ca, 2);                                                                   \
    SHR(t1, fresh_output_bc, 1);                                                                   \
    XOR(t2, t2, t1);                                                                               \
    XOR(aux, t2, fresh_output_ab);                                                                 \
    /* a & b */                                                                                    \
    AND(t0, a, b);                                                                                 \
    /* b & c */                                                                                    \
    AND(t1, b, c);                                                                                 \
    /* c & a */                                                                                    \
    AND(t2, c, a);                                                                                 \
    SHR(t2, t2, 2);                                                                                \
    SHR(t1, t1, 1);                                                                                \
    XOR(t2, t2, t1);                                                                               \
    XOR(t2, t2, t0);                                                                               \
    XOR(aux, aux, t2);                                                                             \
                                                                                                   \
    bitstream_t parity_tape     = {{tapes->parity_tapes}, tapes->pos};                             \
    bitstream_t last_party_tape = {{tapes->tape[15]}, tapes->pos};                                 \
                                                                                                   \
    /* calculate aux_bits to fix and_helper */                                                     \
    mzd_from_bitstream(&parity_tape, t0, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);        \
    XOR(aux, aux, t0);                                                                             \
    mzd_from_bitstream(&last_party_tape, t1, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);    \
    XOR(aux, aux, t1);                                                                             \
                                                                                                   \
    last_party_tape.position = tapes->pos;                                                         \
    mzd_to_bitstream(&last_party_tape, aux, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);     \
    bitstream_t aux_tape = {{tapes->aux_bits}, tapes->aux_pos};                                    \
    mzd_to_bitstream(&aux_tape, aux, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);            \
                                                                                                   \
    tapes->aux_pos += LOWMC_N;                                                                     \
  } while (0)

#if defined(WITH_LOWMC_129_129_4)
static void sbox_aux_uint64_lowmc_129_129_4(mzd_local_t* statein, mzd_local_t* stateout,
                                            randomTape_t* tapes) {
  picnic3_aux_sbox_bitsliced(LOWMC_129_129_4_N, mzd_xor_uint64_192, mzd_and_uint64_192,
                             mzd_shift_left_uint64_192, mzd_shift_right_uint64_192,
                             mask_129_129_43_a, mask_129_129_43_b, mask_129_129_43_c);
}
#endif
#if defined(WITH_LOWMC_192_192_4)
static void sbox_aux_uint64_lowmc_192_192_4(mzd_local_t* statein, mzd_local_t* stateout,
                                            randomTape_t* tapes) {
  picnic3_aux_sbox_bitsliced(LOWMC_192_192_4_N, mzd_xor_uint64_192, mzd_and_uint64_192,
                             mzd_shift_left_uint64_192, mzd_shift_right_uint64_192,
                             mask_192_192_64_a, mask_192_192_64_b, mask_192_192_64_c);
}
#endif
#if defined(WITH_LOWMC_255_255_4)
static void sbox_aux_uint64_lowmc_255_255_4(mzd_local_t* statein, mzd_local_t* stateout,
                                            randomTape_t* tapes) {
  picnic3_aux_sbox_bitsliced(LOWMC_255_255_4_N, mzd_xor_uint64_256, mzd_and_uint64_256,
                             mzd_shift_left_uint64_256, mzd_shift_right_uint64_256,
                             mask_255_255_85_a, mask_255_255_85_b, mask_255_255_85_c);
}
#endif
#endif /* !NO_UINT64_FALLBACK */
#endif /* WITH_KKW */

#if !defined(NO_UINT64_FALLBACK)
// uint64 based implementation
#define IMPL uint64

#include "lowmc_129_129_4_fns_uint64.h"
#include "lowmc.c.i"

#include "lowmc_192_192_4_fns_uint64.h"
#include "lowmc.c.i"

#include "lowmc_255_255_4_fns_uint64.h"
#include "lowmc.c.i"

#include "lowmc_128_128_20_fns_uint64.h"
#include "lowmc.c.i"

#include "lowmc_192_192_30_fns_uint64.h"
#include "lowmc.c.i"

#include "lowmc_256_256_38_fns_uint64.h"
#include "lowmc.c.i"
#endif

#if defined(WITH_OPT)
#if defined(WITH_SSE2) || defined(WITH_NEON)
#define FN_ATTR ATTR_TARGET_S128
#undef IMPL
#define IMPL s128

#if defined(WITH_KKW)
#define picnic3_aux_sbox_bitsliced_mm128(LOWMC_N, XOR, AND, SHL, SHR, bitmask_a, bitmask_b,        \
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
    word128 d[2] ATTR_ALIGNED(alignof(word128));                                                   \
    word128 e[2] ATTR_ALIGNED(alignof(word128));                                                   \
    word128 f[2] ATTR_ALIGNED(alignof(word128));                                                   \
    /* a */                                                                                        \
    AND(d, bitmask_a->w128, stateout->w128);                                                       \
    /* b */                                                                                        \
    AND(e, bitmask_b->w128, stateout->w128);                                                       \
    /* c */                                                                                        \
    AND(f, bitmask_c->w128, stateout->w128);                                                       \
                                                                                                   \
    SHL(d, d, 2);                                                                                  \
    SHL(e, e, 1);                                                                                  \
                                                                                                   \
    word128 fresh_output_ab[2] ATTR_ALIGNED(alignof(word128));                                     \
    word128 fresh_output_bc[2] ATTR_ALIGNED(alignof(word128));                                     \
    word128 fresh_output_ca[2] ATTR_ALIGNED(alignof(word128));                                     \
    XOR(fresh_output_ab, a, b);                                                                    \
    XOR(fresh_output_ca, e, fresh_output_ab);                                                      \
    XOR(fresh_output_bc, d, a);                                                                    \
    XOR(fresh_output_ab, fresh_output_ab, c);                                                      \
    XOR(fresh_output_ab, fresh_output_ab, f);                                                      \
                                                                                                   \
    word128 t0[2] ATTR_ALIGNED(alignof(word128));                                                  \
    word128 t1[2] ATTR_ALIGNED(alignof(word128));                                                  \
    word128 t2[2] ATTR_ALIGNED(alignof(word128));                                                  \
    mzd_local_t tmp[1], aux[1];                                                                    \
    SHR(t2, fresh_output_ca, 2);                                                                   \
    SHR(t1, fresh_output_bc, 1);                                                                   \
    XOR(t2, t2, t1);                                                                               \
    XOR(aux->w128, t2, fresh_output_ab);                                                           \
                                                                                                   \
    /* a & b */                                                                                    \
    AND(t0, a, b);                                                                                 \
    /* b & c */                                                                                    \
    AND(t1, b, c);                                                                                 \
    /* c & a */                                                                                    \
    AND(t2, c, a);                                                                                 \
    SHR(t2, t2, 2);                                                                                \
    SHR(t1, t1, 1);                                                                                \
    XOR(t2, t2, t1);                                                                               \
    XOR(t2, t2, t0);                                                                               \
    XOR(aux->w128, aux->w128, t2);                                                                 \
                                                                                                   \
    bitstream_t parity_tape     = {{tapes->parity_tapes}, tapes->pos};                             \
    bitstream_t last_party_tape = {{tapes->tape[15]}, tapes->pos};                                 \
                                                                                                   \
    /* calculate aux_bits to fix and_helper */                                                     \
    mzd_from_bitstream(&parity_tape, tmp, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);       \
    XOR(aux->w128, aux->w128, tmp->w128);                                                          \
    mzd_from_bitstream(&last_party_tape, tmp, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);   \
    XOR(aux->w128, aux->w128, tmp->w128);                                                          \
                                                                                                   \
    last_party_tape.position = tapes->pos;                                                         \
    mzd_to_bitstream(&last_party_tape, aux, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);     \
    bitstream_t aux_tape = {{tapes->aux_bits}, tapes->aux_pos};                                    \
    mzd_to_bitstream(&aux_tape, aux, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);            \
                                                                                                   \
    tapes->aux_pos += LOWMC_N;                                                                     \
  } while (0)

#if defined(WITH_LOWMC_129_129_4)
ATTR_TARGET_S128
static void sbox_aux_s128_lowmc_129_129_4(mzd_local_t* statein, mzd_local_t* stateout,
                                          randomTape_t* tapes) {
  picnic3_aux_sbox_bitsliced_mm128(LOWMC_129_129_4_N, mm128_xor_256, mm128_and_256,
                                   mm128_shift_left_256, mm128_shift_right_256, mask_129_129_43_a,
                                   mask_129_129_43_b, mask_129_129_43_c);
}
#endif
#if defined(WITH_LOWMC_192_192_4)
ATTR_TARGET_S128
static void sbox_aux_s128_lowmc_192_192_4(mzd_local_t* statein, mzd_local_t* stateout,
                                          randomTape_t* tapes) {
  picnic3_aux_sbox_bitsliced_mm128(LOWMC_192_192_4_N, mm128_xor_256, mm128_and_256,
                                   mm128_shift_left_256, mm128_shift_right_256, mask_192_192_64_a,
                                   mask_192_192_64_b, mask_192_192_64_c);
}
#endif
#if defined(WITH_LOWMC_255_255_4)
ATTR_TARGET_S128
static void sbox_aux_s128_lowmc_255_255_4(mzd_local_t* statein, mzd_local_t* stateout,
                                          randomTape_t* tapes) {
  picnic3_aux_sbox_bitsliced_mm128(LOWMC_255_255_4_N, mm128_xor_256, mm128_and_256,
                                   mm128_shift_left_256, mm128_shift_right_256, mask_255_255_85_a,
                                   mask_255_255_85_b, mask_255_255_85_c);
}
#endif
#endif

#include "lowmc_129_129_4_fns_s128.h"
#include "lowmc.c.i"

#include "lowmc_192_192_4_fns_s128.h"
#include "lowmc.c.i"

#include "lowmc_255_255_4_fns_s128.h"
#include "lowmc.c.i"

#include "lowmc_128_128_20_fns_s128.h"
#include "lowmc.c.i"

#include "lowmc_192_192_30_fns_s128.h"
#include "lowmc.c.i"

#include "lowmc_256_256_38_fns_s128.h"
#include "lowmc.c.i"
#endif

#if defined(WITH_AVX2)
#undef FN_ATTR
#define FN_ATTR ATTR_TARGET_AVX2
#undef IMPL
#define IMPL s256

#if defined(WITH_KKW)
#define picnic3_aux_sbox_bitsliced_mm256(LOWMC_N, XOR, AND, ROL, ROR, bitmask_a, bitmask_b,        \
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
    word256 d ATTR_ALIGNED(alignof(word256));                                                      \
    word256 e ATTR_ALIGNED(alignof(word256));                                                      \
    word256 f ATTR_ALIGNED(alignof(word256));                                                      \
    /* d */                                                                                        \
    d = AND(bitmask_a->w256, stateout->w256);                                                      \
    /* e */                                                                                        \
    e = AND(bitmask_b->w256, stateout->w256);                                                      \
    /* f */                                                                                        \
    f = AND(bitmask_c->w256, stateout->w256);                                                      \
                                                                                                   \
    d = ROL(d, 2);                                                                                 \
    e = ROL(e, 1);                                                                                 \
                                                                                                   \
    word256 fresh_output_ab ATTR_ALIGNED(alignof(word256));                                        \
    word256 fresh_output_bc ATTR_ALIGNED(alignof(word256));                                        \
    word256 fresh_output_ca ATTR_ALIGNED(alignof(word256));                                        \
    fresh_output_ab = XOR(a, b);                                                                   \
    fresh_output_ca = XOR(e, fresh_output_ab);                                                     \
    fresh_output_bc = XOR(d, a);                                                                   \
    fresh_output_ab = XOR(fresh_output_ab, c);                                                     \
    fresh_output_ab = XOR(fresh_output_ab, f);                                                     \
                                                                                                   \
    word256 t0 ATTR_ALIGNED(alignof(word256));                                                     \
    word256 t1 ATTR_ALIGNED(alignof(word256));                                                     \
    word256 t2 ATTR_ALIGNED(alignof(word256));                                                     \
    mzd_local_t tmp[1], aux[1];                                                                    \
    t2        = ROR(fresh_output_ca, 2);                                                           \
    t1        = ROR(fresh_output_bc, 1);                                                           \
    t2        = XOR(t2, t1);                                                                       \
    aux->w256 = XOR(t2, fresh_output_ab);                                                          \
                                                                                                   \
    /* a & b */                                                                                    \
    t0 = AND(a, b);                                                                                \
    /* b & c */                                                                                    \
    t1 = AND(b, c);                                                                                \
    /* c & a */                                                                                    \
    t2        = AND(c, a);                                                                         \
    t2        = ROR(t2, 2);                                                                        \
    t1        = ROR(t1, 1);                                                                        \
    t2        = XOR(t2, t1);                                                                       \
    t2        = XOR(t2, t0);                                                                       \
    aux->w256 = XOR(aux->w256, t2);                                                                \
                                                                                                   \
    bitstream_t parity_tape     = {{tapes->parity_tapes}, tapes->pos};                             \
    bitstream_t last_party_tape = {{tapes->tape[15]}, tapes->pos};                                 \
                                                                                                   \
    /* calculate aux_bits to fix and_helper */                                                     \
    mzd_from_bitstream(&parity_tape, tmp, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);       \
    aux->w256 = XOR(aux->w256, tmp->w256);                                                         \
    mzd_from_bitstream(&last_party_tape, tmp, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);   \
    aux->w256 = XOR(aux->w256, tmp->w256);                                                         \
                                                                                                   \
    last_party_tape.position = tapes->pos;                                                         \
    mzd_to_bitstream(&last_party_tape, aux, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);     \
    bitstream_t aux_tape = {{tapes->aux_bits}, tapes->aux_pos};                                    \
    mzd_to_bitstream(&aux_tape, aux, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);            \
                                                                                                   \
    tapes->aux_pos += LOWMC_N;                                                                     \
  } while (0)

#if defined(WITH_LOWMC_129_129_4)
ATTR_TARGET_AVX2
static void sbox_aux_s256_lowmc_129_129_4(mzd_local_t* statein, mzd_local_t* stateout,
                                          randomTape_t* tapes) {
  picnic3_aux_sbox_bitsliced_mm256(LOWMC_129_129_4_N, mm256_xor, mm256_and, mm256_shift_left,
                                   mm256_shift_right, mask_129_129_43_a, mask_129_129_43_b,
                                   mask_129_129_43_c);
}
#endif
#if defined(WITH_LOWMC_192_192_4)
ATTR_TARGET_AVX2
static void sbox_aux_s256_lowmc_192_192_4(mzd_local_t* statein, mzd_local_t* stateout,
                                          randomTape_t* tapes) {
  picnic3_aux_sbox_bitsliced_mm256(LOWMC_192_192_4_N, mm256_xor, mm256_and, mm256_rotate_left,
                                   mm256_rotate_right, mask_192_192_64_a, mask_192_192_64_b,
                                   mask_192_192_64_c);
}
#endif
#if defined(WITH_LOWMC_255_255_4)
ATTR_TARGET_AVX2
static void sbox_aux_s256_lowmc_255_255_4(mzd_local_t* statein, mzd_local_t* stateout,
                                          randomTape_t* tapes) {
  picnic3_aux_sbox_bitsliced_mm256(LOWMC_255_255_4_N, mm256_xor, mm256_and, mm256_rotate_left,
                                   mm256_rotate_right, mask_255_255_85_a, mask_255_255_85_b,
                                   mask_255_255_85_c);
}
#endif
#endif

#include "lowmc_129_129_4_fns_s256.h"
#include "lowmc.c.i"

#include "lowmc_192_192_4_fns_s256.h"
#include "lowmc.c.i"

#include "lowmc_255_255_4_fns_s256.h"
#include "lowmc.c.i"

#include "lowmc_128_128_20_fns_s256.h"
#include "lowmc.c.i"

#include "lowmc_192_192_30_fns_s256.h"
#include "lowmc.c.i"

#include "lowmc_256_256_38_fns_s256.h"
#include "lowmc.c.i"
#endif
#endif

lowmc_implementation_f lowmc_get_implementation(const lowmc_parameters_t* lowmc) {
  assert((lowmc->m == 43 && lowmc->n == 129) || (lowmc->m == 64 && lowmc->n == 192) ||
         (lowmc->m == 85 && lowmc->n == 255) ||
         (lowmc->m == 10 && (lowmc->n == 128 || lowmc->n == 192 || lowmc->n == 256)));

#if defined(WITH_OPT)
#if defined(WITH_AVX2)
  /* AVX2 enabled instances */
  if (CPU_SUPPORTS_AVX2) {
#if defined(WITH_ZKBPP)
    /* Instances with partial Sbox layer */
    if (lowmc->m == 10) {
      switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_20)
      case 128:
        return lowmc_s256_lowmc_128_128_20;
#endif
#if defined(WITH_LOWMC_192_192_30)
      case 192:
        return lowmc_s256_lowmc_192_192_30;
#endif
#if defined(WITH_LOWMC_256_256_38)
      case 256:
        return lowmc_s256_lowmc_256_256_38;
#endif
      }
    }
#endif

    /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
    if (lowmc->n == 129 && lowmc->m == 43)
      return lowmc_s256_lowmc_129_129_4;
#endif
#if defined(WITH_LOWMC_192_192_4)
    if (lowmc->n == 192 && lowmc->m == 64)
      return lowmc_s256_lowmc_192_192_4;
#endif
#if defined(WITH_LOWMC_255_255_4)
    if (lowmc->n == 255 && lowmc->m == 85)
      return lowmc_s256_lowmc_255_255_4;
#endif
  }
#endif

#if defined(WITH_SSE2) || defined(WITH_NEON)
  /* SSE2/NEON enabled instances */
  if (CPU_SUPPORTS_SSE2 || CPU_SUPPORTS_NEON) {
#if defined(WITH_ZKBPP)
    /* Instances with partial Sbox layer */
    if (lowmc->m == 10) {
      switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_20)
      case 128:
        return lowmc_s128_lowmc_128_128_20;
#endif
#if defined(WITH_LOWMC_192_192_30)
      case 192:
        return lowmc_s128_lowmc_192_192_30;
#endif
#if defined(WITH_LOWMC_256_256_38)
      case 256:
        return lowmc_s128_lowmc_256_256_38;
#endif
      }
    }
#endif

    /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
    if (lowmc->n == 129 && lowmc->m == 43)
      return lowmc_s128_lowmc_129_129_4;
#endif
#if defined(WITH_LOWMC_192_192_4)
    if (lowmc->n == 192 && lowmc->m == 64)
      return lowmc_s128_lowmc_192_192_4;
#endif
#if defined(WITH_LOWMC_255_255_4)
    if (lowmc->n == 255 && lowmc->m == 85)
      return lowmc_s128_lowmc_255_255_4;
#endif
  }
#endif
#endif

#if !defined(NO_UINT64_FALLBACK)
  /* uint64_t implementations */
#if defined(WITH_ZKBPP)
  /* Instances with partial Sbox layer */
  if (lowmc->m == 10) {
    switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_20)
    case 128:
      return lowmc_uint64_lowmc_128_128_20;
#endif
#if defined(WITH_LOWMC_192_192_30)
    case 192:
      return lowmc_uint64_lowmc_192_192_30;
#endif
#if defined(WITH_LOWMC_256_256_38)
    case 256:
      return lowmc_uint64_lowmc_256_256_38;
#endif
    }
  }
#endif

  /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
  if (lowmc->n == 129 && lowmc->m == 43)
    return lowmc_uint64_lowmc_129_129_4;
#endif
#if defined(WITH_LOWMC_192_192_4)
  if (lowmc->n == 192 && lowmc->m == 64)
    return lowmc_uint64_lowmc_192_192_4;
#endif
#if defined(WITH_LOWMC_255_255_4)
  if (lowmc->n == 255 && lowmc->m == 85)
    return lowmc_uint64_lowmc_255_255_4;
#endif
#endif

  return NULL;
}

#if defined(WITH_ZKBPP)
lowmc_store_implementation_f lowmc_store_get_implementation(const lowmc_parameters_t* lowmc) {
  assert((lowmc->m == 43 && lowmc->n == 129) || (lowmc->m == 64 && lowmc->n == 192) ||
         (lowmc->m == 85 && lowmc->n == 255) ||
         (lowmc->m == 10 && (lowmc->n == 128 || lowmc->n == 192 || lowmc->n == 256)));

#if defined(WITH_OPT)
#if defined(WITH_AVX2)
  /* AVX2 enabled instances */
  if (CPU_SUPPORTS_AVX2) {
    /* Instances with partial Sbox layer */
    if (lowmc->m == 10) {
      switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_20)
      case 128:
        return lowmc_store_s256_lowmc_128_128_20;
#endif
#if defined(WITH_LOWMC_192_192_30)
      case 192:
        return lowmc_store_s256_lowmc_192_192_30;
#endif
#if defined(WITH_LOWMC_256_256_38)
      case 256:
        return lowmc_store_s256_lowmc_256_256_38;
#endif
      }
    }

    /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
    if (lowmc->n == 129 && lowmc->m == 43)
      return lowmc_store_s256_lowmc_129_129_4;
#endif
#if defined(WITH_LOWMC_192_192_4)
    if (lowmc->n == 192 && lowmc->m == 64)
      return lowmc_store_s256_lowmc_192_192_4;
#endif
#if defined(WITH_LOWMC_255_255_4)
    if (lowmc->n == 255 && lowmc->m == 85)
      return lowmc_store_s256_lowmc_255_255_4;
#endif
  }
#endif

#if defined(WITH_SSE2) || defined(WITH_NEON)
  /* SSE2/NEON enabled instances */
  if (CPU_SUPPORTS_SSE2 || CPU_SUPPORTS_NEON) {
    /* Instances with partial Sbox layer */
    if (lowmc->m == 10) {
      switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_20)
      case 128:
        return lowmc_store_s128_lowmc_128_128_20;
#endif
#if defined(WITH_LOWMC_192_192_30)
      case 192:
        return lowmc_store_s128_lowmc_192_192_30;
#endif
#if defined(WITH_LOWMC_256_256_38)
      case 256:
        return lowmc_store_s128_lowmc_256_256_38;
#endif
      }
    }

    /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
    if (lowmc->n == 129 && lowmc->m == 43)
      return lowmc_store_s128_lowmc_129_129_4;
#endif
#if defined(WITH_LOWMC_192_192_4)
    if (lowmc->n == 192 && lowmc->m == 64)
      return lowmc_store_s128_lowmc_192_192_4;
#endif
#if defined(WITH_LOWMC_255_255_4)
    if (lowmc->n == 255 && lowmc->m == 85)
      return lowmc_store_s128_lowmc_255_255_4;
#endif
  }
#endif
#endif

#if !defined(NO_UINT64_FALLBACK)
  /* uint64_t implementations */
  /* Instances with partial Sbox layer */
  if (lowmc->m == 10) {
    switch (lowmc->n) {
#if defined(WITH_LOWMC_128_128_20)
    case 128:
      return lowmc_store_uint64_lowmc_128_128_20;
#endif
#if defined(WITH_LOWMC_192_192_30)
    case 192:
      return lowmc_store_uint64_lowmc_192_192_30;
#endif
#if defined(WITH_LOWMC_256_256_38)
    case 256:
      return lowmc_store_uint64_lowmc_256_256_38;
#endif
    }
  }

  /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
  if (lowmc->n == 129 && lowmc->m == 43)
    return lowmc_store_uint64_lowmc_129_129_4;
#endif
#if defined(WITH_LOWMC_192_192_4)
  if (lowmc->n == 192 && lowmc->m == 64)
    return lowmc_store_uint64_lowmc_192_192_4;
#endif
#if defined(WITH_LOWMC_255_255_4)
  if (lowmc->n == 255 && lowmc->m == 85)
    return lowmc_store_uint64_lowmc_255_255_4;
#endif
#endif

  return NULL;
}
#endif

#if defined(WITH_KKW)
lowmc_compute_aux_implementation_f
lowmc_compute_aux_get_implementation(const lowmc_parameters_t* lowmc) {
  assert((lowmc->m == 43 && lowmc->n == 129) || (lowmc->m == 64 && lowmc->n == 192) ||
         (lowmc->m == 85 && lowmc->n == 255));

#if defined(WITH_OPT)
#if defined(WITH_AVX2)
  if (CPU_SUPPORTS_AVX2) {
#if defined(WITH_LOWMC_129_129_4)
    if (lowmc->n == 129 && lowmc->m == 43)
      return lowmc_compute_aux_s256_lowmc_129_129_4;
#endif
#if defined(WITH_LOWMC_192_192_4)
    if (lowmc->n == 192 && lowmc->m == 64)
      return lowmc_compute_aux_s256_lowmc_192_192_4;
#endif
#if defined(WITH_LOWMC_255_255_4)
    if (lowmc->n == 255 && lowmc->m == 85)
      return lowmc_compute_aux_s256_lowmc_255_255_4;
#endif
  }
#endif
#if defined(WITH_SSE2) || defined(WITH_NEON)
  if (CPU_SUPPORTS_SSE2 || CPU_SUPPORTS_NEON) {
#if defined(WITH_LOWMC_129_129_4)
    if (lowmc->n == 129 && lowmc->m == 43)
      return lowmc_compute_aux_s128_lowmc_129_129_4;
#endif
#if defined(WITH_LOWMC_192_192_4)
    if (lowmc->n == 192 && lowmc->m == 64)
      return lowmc_compute_aux_s128_lowmc_192_192_4;
#endif
#if defined(WITH_LOWMC_255_255_4)
    if (lowmc->n == 255 && lowmc->m == 85)
      return lowmc_compute_aux_s128_lowmc_255_255_4;
#endif
  }
#endif
#endif

#if !defined(NO_UINT64_FALLBACK)
#if defined(WITH_LOWMC_129_129_4)
  if (lowmc->n == 129 && lowmc->m == 43)
    return lowmc_compute_aux_uint64_lowmc_129_129_4;
#endif
#if defined(WITH_LOWMC_192_192_4)
  if (lowmc->n == 192 && lowmc->m == 64)
    return lowmc_compute_aux_uint64_lowmc_192_192_4;
#endif
#if defined(WITH_LOWMC_255_255_4)
  if (lowmc->n == 255 && lowmc->m == 85)
    return lowmc_compute_aux_uint64_lowmc_255_255_4;
#endif
#endif

  return NULL;
}
#endif
