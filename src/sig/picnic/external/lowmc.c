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

// clang-format off
#if defined(WITH_LOWMC_128_128_20) || defined(WITH_LOWMC_192_192_30) || defined(WITH_LOWMC_256_256_38)
// clang-format on
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
static inline void sbox_s128_full(mzd_local_t* in, const mzd_local_t* mask_a,
                                  const mzd_local_t* mask_b, const mzd_local_t* mask_c) {
  word128 inm[2] = {mm128_load(&in->w64[0]), mm128_load(&in->w64[2])};
  word128 x0m[2] = {mm128_load(&mask_a->w64[0]), mm128_load(&mask_a->w64[2])};
  word128 x1m[2] = {mm128_load(&mask_b->w64[0]), mm128_load(&mask_b->w64[2])};
  word128 x2m[2] = {mm128_load(&mask_c->w64[0]), mm128_load(&mask_c->w64[2])};
  mm128_and_256(x0m, inm, x0m);
  mm128_and_256(x1m, inm, x1m);
  mm128_and_256(x2m, inm, x2m);

  mm128_shift_left_256(x0m, x0m, 2);
  mm128_shift_left_256(x1m, x1m, 1);

  word128 t0[2], t1[2], t2[2];
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
  mm128_xor_256(inm, t0, t2);
  mm128_store(&in->w64[0], inm[0]);
  mm128_store(&in->w64[2], inm[1]);
}

#if defined(WITH_LOWMC_129_129_4)
ATTR_TARGET_S128
static inline void sbox_s128_lowmc_129_129_4(mzd_local_t* in) {
  sbox_s128_full(in, mask_129_129_43_a, mask_129_129_43_b, mask_129_129_43_c);
}
#endif

#if defined(WITH_LOWMC_192_192_4)
ATTR_TARGET_S128
static inline void sbox_s128_lowmc_192_192_4(mzd_local_t* in) {
  sbox_s128_full(in, mask_192_192_64_a, mask_192_192_64_b, mask_192_192_64_c);
}
#endif

#if defined(WITH_LOWMC_255_255_4)
ATTR_TARGET_S128
static inline void sbox_s128_lowmc_255_255_4(mzd_local_t* in) {
  sbox_s128_full(in, mask_255_255_85_a, mask_255_255_85_b, mask_255_255_85_c);
}
#endif
#endif

#if defined(WITH_AVX2)
ATTR_TARGET_AVX2
static inline void sbox_s256_lowmc_full(mzd_local_t* in, const word256 mask_a, const word256 mask_b,
                                        const word256 mask_c) {
  const word256 min = mm256_load(in);
  // a
  word256 x0m = mm256_and(min, mask_a);
  // b
  word256 x1m = mm256_and(min, mask_b);
  // c
  word256 x2m = mm256_and(min, mask_c);

  x0m = mm256_rotate_left(x0m, 2);
  x1m = mm256_rotate_left(x1m, 1);

  // b & c
  word256 t0 = mm256_and(x1m, x2m);
  // a & c
  word256 t1 = mm256_and(x0m, x2m);
  // a & b
  word256 t2 = mm256_and(x0m, x1m);

  // (b & c) ^ a
  t0 = mm256_xor(t0, x0m);

  // a ^ b
  x0m = mm256_xor(x0m, x1m);
  // a ^ b ^ (a & c)
  t1 = mm256_xor(t1, x0m);
  // a ^ b ^ c ^ (a & b)
  t2 = mm256_xor(mm256_xor(t2, x0m), x2m);

  t0 = mm256_rotate_right(t0, 2);
  t1 = mm256_rotate_right(t1, 1);

  mm256_store(in, mm256_xor(mm256_xor(t0, t1), t2));
}

#if defined(WITH_LOWMC_129_129_4)
ATTR_TARGET_AVX2
static inline void sbox_s256_lowmc_129_129_4(mzd_local_t* in) {
  sbox_s256_lowmc_full(in,
                       mm256_set_4(MASK_129_129_43_A_0, MASK_129_129_43_A_1, MASK_129_129_43_A_2,
                                   MASK_129_129_43_A_3),
                       mm256_set_4(MASK_129_129_43_B_0, MASK_129_129_43_B_1, MASK_129_129_43_B_2,
                                   MASK_129_129_43_B_3),
                       mm256_set_4(MASK_129_129_43_C_0, MASK_129_129_43_C_1, MASK_129_129_43_C_2,
                                   MASK_129_129_43_C_3));
}
#endif

#if defined(WITH_LOWMC_192_192_4)
ATTR_TARGET_AVX2
static inline void sbox_s256_lowmc_192_192_4(mzd_local_t* in) {
  sbox_s256_lowmc_full(in,
                       mm256_set_4(MASK_192_192_64_A_0, MASK_192_192_64_A_1, MASK_192_192_64_A_2,
                                   MASK_192_192_64_A_3),
                       mm256_set_4(MASK_192_192_64_B_0, MASK_192_192_64_B_1, MASK_192_192_64_B_2,
                                   MASK_192_192_64_B_3),
                       mm256_set_4(MASK_192_192_64_C_0, MASK_192_192_64_C_1, MASK_192_192_64_C_2,
                                   MASK_192_192_64_C_3));
}
#endif

#if defined(WITH_LOWMC_255_255_4)
ATTR_TARGET_AVX2
static inline void sbox_s256_lowmc_255_255_4(mzd_local_t* in) {
  sbox_s256_lowmc_full(in,
                       mm256_set_4(MASK_255_255_85_A_0, MASK_255_255_85_A_1, MASK_255_255_85_A_2,
                                   MASK_255_255_85_A_3),
                       mm256_set_4(MASK_255_255_85_B_0, MASK_255_255_85_B_1, MASK_255_255_85_B_2,
                                   MASK_255_255_85_B_3),
                       mm256_set_4(MASK_255_255_85_C_0, MASK_255_255_85_C_1, MASK_255_255_85_C_2,
                                   MASK_255_255_85_C_3));
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
    word128 bita[2] = {mm128_load(&bitmask_a->w64[0]), mm128_load(&bitmask_a->w64[2])};            \
    word128 bitb[2] = {mm128_load(&bitmask_b->w64[0]), mm128_load(&bitmask_b->w64[2])};            \
    word128 bitc[2] = {mm128_load(&bitmask_c->w64[0]), mm128_load(&bitmask_c->w64[2])};            \
                                                                                                   \
    word128 aux[2] = {mm128_load(&statein->w64[0]), mm128_load(&statein->w64[2])};                 \
                                                                                                   \
    word128 a[2];                                                                                  \
    word128 b[2];                                                                                  \
    word128 c[2];                                                                                  \
    /* a */                                                                                        \
    AND(a, bita, aux);                                                                             \
    /* b */                                                                                        \
    AND(b, bitb, aux);                                                                             \
    /* c */                                                                                        \
    AND(c, bitc, aux);                                                                             \
                                                                                                   \
    SHL(a, a, 2);                                                                                  \
    SHL(b, b, 1);                                                                                  \
                                                                                                   \
    aux[0] = mm128_load(&stateout->w64[0]);                                                        \
    aux[1] = mm128_load(&stateout->w64[2]);                                                        \
    word128 d[2];                                                                                  \
    word128 e[2];                                                                                  \
    word128 f[2];                                                                                  \
    /* a */                                                                                        \
    AND(d, bita, aux);                                                                             \
    /* b */                                                                                        \
    AND(e, bitb, aux);                                                                             \
    /* c */                                                                                        \
    AND(f, bitc, aux);                                                                             \
                                                                                                   \
    SHL(d, d, 2);                                                                                  \
    SHL(e, e, 1);                                                                                  \
                                                                                                   \
    word128 fresh_output_ab[2];                                                                    \
    word128 fresh_output_bc[2];                                                                    \
    word128 fresh_output_ca[2];                                                                    \
    XOR(fresh_output_ab, a, b);                                                                    \
    XOR(fresh_output_ca, e, fresh_output_ab);                                                      \
    XOR(fresh_output_bc, d, a);                                                                    \
    XOR(fresh_output_ab, fresh_output_ab, c);                                                      \
    XOR(fresh_output_ab, fresh_output_ab, f);                                                      \
                                                                                                   \
    word128 t0[2];                                                                                 \
    word128 t1[2];                                                                                 \
    word128 t2[2];                                                                                 \
    SHR(t2, fresh_output_ca, 2);                                                                   \
    SHR(t1, fresh_output_bc, 1);                                                                   \
    XOR(t2, t2, t1);                                                                               \
    XOR(aux, t2, fresh_output_ab);                                                                 \
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
    XOR(aux, aux, t2);                                                                             \
                                                                                                   \
    bitstream_t parity_tape     = {{tapes->parity_tapes}, tapes->pos};                             \
    bitstream_t last_party_tape = {{tapes->tape[15]}, tapes->pos};                                 \
                                                                                                   \
    /* calculate aux_bits to fix and_helper */                                                     \
    w128_from_bitstream(&parity_tape, t0, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);       \
    XOR(aux, aux, t0);                                                                             \
    w128_from_bitstream(&last_party_tape, t1, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);   \
    XOR(aux, aux, t1);                                                                             \
                                                                                                   \
    last_party_tape.position = tapes->pos;                                                         \
    w128_to_bitstream(&last_party_tape, aux, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);    \
    bitstream_t aux_tape = {{tapes->aux_bits}, tapes->aux_pos};                                    \
    w128_to_bitstream(&aux_tape, aux, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);           \
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
#define picnic3_aux_sbox_bitsliced_mm256(LOWMC_N, bitmask_a, bitmask_b, bitmask_c)                 \
  do {                                                                                             \
    const word256 bita = bitmask_a, bitb = bitmask_b, bitc = bitmask_c;                            \
    word256 aux = mm256_load(statein->w64);                                                        \
    /* a */                                                                                        \
    word256 a = mm256_and(bita, aux);                                                              \
    /* b */                                                                                        \
    word256 b = mm256_and(bitb, aux);                                                              \
    /* c */                                                                                        \
    word256 c = mm256_and(bitc, aux);                                                              \
                                                                                                   \
    a = mm256_rotate_left(a, 2);                                                                   \
    b = mm256_rotate_left(b, 1);                                                                   \
                                                                                                   \
    aux = mm256_load(stateout->w64);                                                               \
    /* d */                                                                                        \
    word256 d = mm256_and(bita, aux);                                                              \
    /* e */                                                                                        \
    word256 e = mm256_and(bitb, aux);                                                              \
    /* f */                                                                                        \
    word256 f = mm256_and(bitc, aux);                                                              \
                                                                                                   \
    d = mm256_rotate_left(d, 2);                                                                   \
    e = mm256_rotate_left(e, 1);                                                                   \
                                                                                                   \
    word256 fresh_output_ab = mm256_xor(a, b);                                                     \
    word256 fresh_output_ca = mm256_xor(e, fresh_output_ab);                                       \
    word256 fresh_output_bc = mm256_xor(d, a);                                                     \
    fresh_output_ab         = mm256_xor(fresh_output_ab, mm256_xor(c, f));                         \
                                                                                                   \
    word256 t2 = mm256_rotate_right(fresh_output_ca, 2);                                           \
    word256 t1 = mm256_rotate_right(fresh_output_bc, 1);                                           \
    t2         = mm256_xor(t2, t1);                                                                \
    aux        = mm256_xor(t2, fresh_output_ab);                                                   \
                                                                                                   \
    /* a & b */                                                                                    \
    word256 t0 = mm256_and(a, b);                                                                  \
    /* b & c */                                                                                    \
    t1 = mm256_and(b, c);                                                                          \
    /* c & a */                                                                                    \
    t2  = mm256_and(c, a);                                                                         \
    t2  = mm256_rotate_right(t2, 2);                                                               \
    t1  = mm256_rotate_right(t1, 1);                                                               \
    aux = mm256_xor(mm256_xor(aux, t2), mm256_xor(t0, t1));                                        \
                                                                                                   \
    bitstream_t parity_tape     = {{tapes->parity_tapes}, tapes->pos};                             \
    bitstream_t last_party_tape = {{tapes->tape[15]}, tapes->pos};                                 \
                                                                                                   \
    /* calculate aux_bits to fix and_helper */                                                     \
    t0  = w256_from_bitstream(&parity_tape, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);     \
    t1  = w256_from_bitstream(&last_party_tape, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N); \
    aux = mm256_xor(aux, mm256_xor(t0, t1));                                                       \
                                                                                                   \
    last_party_tape.position = tapes->pos;                                                         \
    w256_to_bitstream(&last_party_tape, aux, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);    \
    bitstream_t aux_tape = {{tapes->aux_bits}, tapes->aux_pos};                                    \
    w256_to_bitstream(&aux_tape, aux, (LOWMC_N + 63) / (sizeof(uint64_t) * 8), LOWMC_N);           \
                                                                                                   \
    tapes->aux_pos += LOWMC_N;                                                                     \
  } while (0)

#if defined(WITH_LOWMC_129_129_4)
ATTR_TARGET_AVX2
static void sbox_aux_s256_lowmc_129_129_4(mzd_local_t* statein, mzd_local_t* stateout,
                                          randomTape_t* tapes) {
  picnic3_aux_sbox_bitsliced_mm256(LOWMC_129_129_4_N,
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
static void sbox_aux_s256_lowmc_192_192_4(mzd_local_t* statein, mzd_local_t* stateout,
                                          randomTape_t* tapes) {
  picnic3_aux_sbox_bitsliced_mm256(LOWMC_192_192_4_N,
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
static void sbox_aux_s256_lowmc_255_255_4(mzd_local_t* statein, mzd_local_t* stateout,
                                          randomTape_t* tapes) {
  picnic3_aux_sbox_bitsliced_mm256(LOWMC_255_255_4_N,
                                   mm256_set_4(MASK_255_255_85_A_0, MASK_255_255_85_A_1,
                                               MASK_255_255_85_A_2, MASK_255_255_85_A_3),
                                   mm256_set_4(MASK_255_255_85_B_0, MASK_255_255_85_B_1,
                                               MASK_255_255_85_B_2, MASK_255_255_85_B_3),
                                   mm256_set_4(MASK_255_255_85_C_0, MASK_255_255_85_C_1,
                                               MASK_255_255_85_C_2, MASK_255_255_85_C_3));
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

void lowmc_compute(const lowmc_parameters_t* lowmc, const lowmc_key_t* key, const mzd_local_t* x,
                   mzd_local_t* y) {
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
      lowmc_s256_lowmc_128_128_20(key, x, y);
      return;
#endif
#if defined(WITH_LOWMC_192_192_30)
    case LOWMC_ID(192, 10):
      lowmc_s256_lowmc_192_192_30(key, x, y);
      return;
#endif
#if defined(WITH_LOWMC_256_256_38)
    case LOWMC_ID(256, 10):
      lowmc_s256_lowmc_256_256_38(key, x, y);
      return;
#endif
#endif
      /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
    case LOWMC_ID(129, 43):
      lowmc_s256_lowmc_129_129_4(key, x, y);
      return;
#endif
#if defined(WITH_LOWMC_192_192_4)
    case LOWMC_ID(192, 64):
      lowmc_s256_lowmc_192_192_4(key, x, y);
      return;
#endif
#if defined(WITH_LOWMC_255_255_4)
    case LOWMC_ID(255, 85):
      lowmc_s256_lowmc_255_255_4(key, x, y);
      return;
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
      lowmc_s128_lowmc_128_128_20(key, x, y);
      return;
#endif
#if defined(WITH_LOWMC_192_192_30)
    case LOWMC_ID(192, 10):
      lowmc_s128_lowmc_192_192_30(key, x, y);
      return;
#endif
#if defined(WITH_LOWMC_256_256_38)
    case LOWMC_ID(256, 10):
      lowmc_s128_lowmc_256_256_38(key, x, y);
      return;
#endif
#endif
      /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
    case LOWMC_ID(129, 43):
      lowmc_s128_lowmc_129_129_4(key, x, y);
      return;
#endif
#if defined(WITH_LOWMC_192_192_4)
    case LOWMC_ID(192, 64):
      lowmc_s128_lowmc_192_192_4(key, x, y);
      return;
#endif
#if defined(WITH_LOWMC_255_255_4)
    case LOWMC_ID(255, 85):
      lowmc_s128_lowmc_255_255_4(key, x, y);
      return;
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
    lowmc_uint64_lowmc_128_128_20(key, x, y);
    return;
#endif
#if defined(WITH_LOWMC_192_192_30)
  case LOWMC_ID(192, 10):
    lowmc_uint64_lowmc_192_192_30(key, x, y);
    return;
#endif
#if defined(WITH_LOWMC_256_256_38)
  case LOWMC_ID(256, 10):
    lowmc_uint64_lowmc_256_256_38(key, x, y);
    return;
#endif
#endif
    /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
  case LOWMC_ID(129, 43):
    lowmc_uint64_lowmc_129_129_4(key, x, y);
    return;
#endif
#if defined(WITH_LOWMC_192_192_4)
  case LOWMC_ID(192, 64):
    lowmc_uint64_lowmc_192_192_4(key, x, y);
    return;
#endif
#if defined(WITH_LOWMC_255_255_4)
  case LOWMC_ID(255, 85):
    lowmc_uint64_lowmc_255_255_4(key, x, y);
    return;
#endif
  }
#endif

  UNREACHABLE;
}

#if defined(PICNIC_STATIC)
// This function is only used by the LowMC benchmark.
lowmc_implementation_f lowmc_get_implementation(const lowmc_parameters_t* lowmc) {
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
      return lowmc_s256_lowmc_128_128_20;
#endif
#if defined(WITH_LOWMC_192_192_30)
    case LOWMC_ID(192, 10):
      return lowmc_s256_lowmc_192_192_30;
#endif
#if defined(WITH_LOWMC_256_256_38)
    case LOWMC_ID(256, 10):
      return lowmc_s256_lowmc_256_256_38;
#endif
#endif
      /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
    case LOWMC_ID(129, 43):
      return lowmc_s256_lowmc_129_129_4;
#endif
#if defined(WITH_LOWMC_192_192_4)
    case LOWMC_ID(192, 64):
      return lowmc_s256_lowmc_192_192_4;
#endif
#if defined(WITH_LOWMC_255_255_4)
    case LOWMC_ID(255, 85):
      return lowmc_s256_lowmc_255_255_4;
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
      return lowmc_s128_lowmc_128_128_20;
#endif
#if defined(WITH_LOWMC_192_192_30)
    case LOWMC_ID(192, 10):
      return lowmc_s128_lowmc_192_192_30;
#endif
#if defined(WITH_LOWMC_256_256_38)
    case LOWMC_ID(256, 10):
      return lowmc_s128_lowmc_256_256_38;
#endif
#endif
      /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
    case LOWMC_ID(129, 43):
      return lowmc_s128_lowmc_129_129_4;
#endif
#if defined(WITH_LOWMC_192_192_4)
    case LOWMC_ID(192, 64):
      return lowmc_s128_lowmc_192_192_4;
#endif
#if defined(WITH_LOWMC_255_255_4)
    case LOWMC_ID(255, 85):
      return lowmc_s128_lowmc_255_255_4;
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
    return lowmc_uint64_lowmc_128_128_20;
#endif
#if defined(WITH_LOWMC_192_192_30)
  case LOWMC_ID(192, 10):
    return lowmc_uint64_lowmc_192_192_30;
#endif
#if defined(WITH_LOWMC_256_256_38)
  case LOWMC_ID(256, 10):
    return lowmc_uint64_lowmc_256_256_38;
#endif
#endif
    /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
  case LOWMC_ID(129, 43):
    return lowmc_uint64_lowmc_129_129_4;
#endif
#if defined(WITH_LOWMC_192_192_4)
  case LOWMC_ID(192, 64):
    return lowmc_uint64_lowmc_192_192_4;
#endif
#if defined(WITH_LOWMC_255_255_4)
  case LOWMC_ID(255, 85):
    return lowmc_uint64_lowmc_255_255_4;
#endif
  }
#endif

  UNREACHABLE;
  return NULL;
}
#endif

#if defined(WITH_ZKBPP)
void lowmc_record_state(const lowmc_parameters_t* lowmc, const lowmc_key_t* key,
                        const mzd_local_t* x, recorded_state_t* state) {
  const uint32_t lowmc_id = lowmc_get_id(lowmc);
#if defined(WITH_OPT)
#if defined(WITH_AVX2)
  /* AVX2 enabled instances */
  if (CPU_SUPPORTS_AVX2) {
    switch (lowmc_id) {
      /* Instances with partial Sbox layer */
#if defined(WITH_LOWMC_128_128_20)
    case LOWMC_ID(128, 10):
      lowmc_store_s256_lowmc_128_128_20(key, x, state);
      return;
#endif
#if defined(WITH_LOWMC_192_192_30)
    case LOWMC_ID(192, 10):
      lowmc_store_s256_lowmc_192_192_30(key, x, state);
      return;
#endif
#if defined(WITH_LOWMC_256_256_38)
    case LOWMC_ID(256, 10):
      lowmc_store_s256_lowmc_256_256_38(key, x, state);
      return;
#endif
      /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
    case LOWMC_ID(129, 43):
      lowmc_store_s256_lowmc_129_129_4(key, x, state);
      return;
#endif
#if defined(WITH_LOWMC_192_192_4)
    case LOWMC_ID(192, 64):
      lowmc_store_s256_lowmc_192_192_4(key, x, state);
      return;
#endif
#if defined(WITH_LOWMC_255_255_4)
    case LOWMC_ID(255, 85):
      lowmc_store_s256_lowmc_255_255_4(key, x, state);
      return;
#endif
    }
  }
#endif

#if defined(WITH_SSE2) || defined(WITH_NEON)
  /* SSE2/NEON enabled instances */
  if (CPU_SUPPORTS_SSE2 || CPU_SUPPORTS_NEON) {
    switch (lowmc_id) {
      /* Instances with partial Sbox layer */
#if defined(WITH_LOWMC_128_128_20)
    case LOWMC_ID(128, 10):
      lowmc_store_s128_lowmc_128_128_20(key, x, state);
      return;
#endif
#if defined(WITH_LOWMC_192_192_30)
    case LOWMC_ID(192, 10):
      lowmc_store_s128_lowmc_192_192_30(key, x, state);
      return;
#endif
#if defined(WITH_LOWMC_256_256_38)
    case LOWMC_ID(256, 10):
      lowmc_store_s128_lowmc_256_256_38(key, x, state);
      return;
#endif
      /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
    case LOWMC_ID(129, 43):
      lowmc_store_s128_lowmc_129_129_4(key, x, state);
      return;
#endif
#if defined(WITH_LOWMC_192_192_4)
    case LOWMC_ID(192, 64):
      lowmc_store_s128_lowmc_192_192_4(key, x, state);
      return;
#endif
#if defined(WITH_LOWMC_255_255_4)
    case LOWMC_ID(255, 85):
      lowmc_store_s128_lowmc_255_255_4(key, x, state);
      return;
#endif
    }
  }
#endif
#endif

#if !defined(NO_UINT64_FALLBACK)
  /* uint64_t implementations */
  switch (lowmc_id) {
    /* Instances with partial Sbox layer */
#if defined(WITH_LOWMC_128_128_20)
  case LOWMC_ID(128, 10):
    lowmc_store_uint64_lowmc_128_128_20(key, x, state);
    return;
#endif
#if defined(WITH_LOWMC_192_192_30)
  case LOWMC_ID(192, 10):
    lowmc_store_uint64_lowmc_192_192_30(key, x, state);
    return;
#endif
#if defined(WITH_LOWMC_256_256_38)
  case LOWMC_ID(256, 10):
    lowmc_store_uint64_lowmc_256_256_38(key, x, state);
    return;
#endif
    /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
  case LOWMC_ID(129, 43):
    lowmc_store_uint64_lowmc_129_129_4(key, x, state);
    return;
#endif
#if defined(WITH_LOWMC_192_192_4)
  case LOWMC_ID(192, 64):
    lowmc_store_uint64_lowmc_192_192_4(key, x, state);
    return;
#endif
#if defined(WITH_LOWMC_255_255_4)
  case LOWMC_ID(255, 85):
    lowmc_store_uint64_lowmc_255_255_4(key, x, state);
    return;
#endif
  }
#endif

  UNREACHABLE;
}
#endif

#if defined(WITH_KKW)
void lowmc_compute_aux(const lowmc_parameters_t* lowmc, lowmc_key_t* key, randomTape_t* tapes) {
  const uint32_t lowmc_id = lowmc_get_id(lowmc);
#if defined(WITH_OPT)
#if defined(WITH_AVX2)
  /* AVX2 enabled instances */
  if (CPU_SUPPORTS_AVX2) {
    switch (lowmc_id) {
      /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
    case LOWMC_ID(129, 43):
      lowmc_aux_s256_lowmc_129_129_4(key, tapes);
      return;
#endif
#if defined(WITH_LOWMC_192_192_4)
    case LOWMC_ID(192, 64):
      lowmc_aux_s256_lowmc_192_192_4(key, tapes);
      return;
#endif
#if defined(WITH_LOWMC_255_255_4)
    case LOWMC_ID(255, 85):
      lowmc_aux_s256_lowmc_255_255_4(key, tapes);
      return;
#endif
    }
  }
#endif

#if defined(WITH_SSE2) || defined(WITH_NEON)
  /* SSE2/NEON enabled instances */
  if (CPU_SUPPORTS_SSE2 || CPU_SUPPORTS_NEON) {
    switch (lowmc_id) {
      /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
    case LOWMC_ID(129, 43):
      lowmc_aux_s128_lowmc_129_129_4(key, tapes);
      return;
#endif
#if defined(WITH_LOWMC_192_192_4)
    case LOWMC_ID(192, 64):
      lowmc_aux_s128_lowmc_192_192_4(key, tapes);
      return;
#endif
#if defined(WITH_LOWMC_255_255_4)
    case LOWMC_ID(255, 85):
      lowmc_aux_s128_lowmc_255_255_4(key, tapes);
      return;
#endif
    }
  }
#endif
#endif

#if !defined(NO_UINT64_FALLBACK)
  /* uint64_t implementations */
  switch (lowmc_id) {
    /* Instances with full Sbox layer */
#if defined(WITH_LOWMC_129_129_4)
  case LOWMC_ID(129, 43):
    lowmc_aux_uint64_lowmc_129_129_4(key, tapes);
    return;
#endif
#if defined(WITH_LOWMC_192_192_4)
  case LOWMC_ID(192, 64):
    lowmc_aux_uint64_lowmc_192_192_4(key, tapes);
    return;
#endif
#if defined(WITH_LOWMC_255_255_4)
  case LOWMC_ID(255, 85):
    lowmc_aux_uint64_lowmc_255_255_4(key, tapes);
    return;
#endif
  }
#endif

  UNREACHABLE;
}
#endif
