// SPDX-License-Identifier: Apache-2.0

#ifndef SHUFFLE_ARITHMETIC_96_H
#define SHUFFLE_ARITHMETIC_96_H

#include <arm_neon.h>
#include <stdint.h>
#include <mayo.h>
#include <arithmetic_common.h>
#include <arithmetic_fixed.h>

#define O_NEON_ROUND_UP ((O_MAX + 1)/2*2)

#define NEON_REGISTER_PER_M_VEC ((M_MAX + 31)/32)

#if M_MAX <= 32
    NOT IMPLEMENTED
#elif M_MAX <= 64
// Multiply an m_vec with a vec of "Len" scalars, and accumulate in temp
#define MULTIPLY_ACCUMULATE_M_VEC(vec, multabs, Len)               \
  uint8x16_t in_odd0 = *(uint8x16_t *)(vec);                       \
  uint8x16_t in_even0 = in_odd0 >> 4;                              \
  uint8x16_t in_odd1 = *(uint8x16_t *)(vec + M_VEC_LIMBS_MAX - 2); \
  uint8x16_t in_even1 = in_odd1 >> 4;                              \
  in_odd0 &= low_nibble_mask;                                      \
  in_odd1 &= low_nibble_mask;                                      \
  for (size_t k = 0; k < Len; k += 2) {                            \
    temp[2 * k] ^= vqtbl1q_u8((multabs)[k / 2], in_odd0);          \
    temp[2 * k + 1] ^= vqtbl1q_u8((multabs)[k / 2], in_even0);     \
    temp[2 * k + 2] ^= vqtbl1q_u8((multabs)[k / 2], in_odd1);      \
    temp[2 * k + 3] ^= vqtbl1q_u8((multabs)[k / 2], in_even1);     \
  }

// convert to normal format and add to accumulator
#define DISENTANGLE_AND_ACCUMULATE_M_VECS(Len, acc, stride)                                                                 \
  for (size_t k = 0; k + 1 < Len; k += 2) {                                                                                 \
    uint8x16_t acc0 = *(uint8x16_t *)(acc + k * stride + M_VEC_LIMBS_MAX - 2);                                              \
    uint8x16_t acc1 = *(uint8x16_t *)(acc + (k + 1) * stride + M_VEC_LIMBS_MAX - 2);                                        \
    *(uint8x16_t *)(acc + k * stride) ^= vsliq_n_u8(temp[2 * k], temp[2 * k + 1], 4);                                       \
    *(uint8x16_t *)(acc + k * stride + M_VEC_LIMBS_MAX - 2) = acc0 ^ vsliq_n_u8(temp[2 * k + 2], temp[2 * k + 3], 4);       \
    *(uint8x16_t *)(acc + (k + 1) * stride + 0) ^= vsriq_n_u8(temp[2 * k + 1], temp[2 * k], 4);                             \
    *(uint8x16_t *)(acc + (k + 1) * stride + M_VEC_LIMBS_MAX - 2) = acc1 ^ vsriq_n_u8(temp[2 * k + 3], temp[2 * k + 2], 4); \
  }                                                                                                                         \
  if (Len % 2 == 1) {                                                                                                       \
    const size_t k = Len - 1;                                                                                               \
    uint8x16_t acc0 = *(uint8x16_t *)(acc + k * stride + M_VEC_LIMBS_MAX - 2);                                              \
    *(uint8x16_t *)(acc + k * stride) ^= vsliq_n_u8(temp[2 * k], temp[2 * k + 1], 4);                                       \
    *(uint8x16_t *)(acc + k * stride + M_VEC_LIMBS_MAX - 2) = acc0 ^ vsliq_n_u8(temp[2 * k + 2], temp[2 * k + 3], 4);       \
  }
#elif M_MAX <= 96
// Multiply an m_vec with a vec of "Len" scalars, and accumulate in temp
#define MULTIPLY_ACCUMULATE_M_VEC(vec, multabs, Len)               \
  uint8x16_t in_odd0 = *(uint8x16_t *)(vec);                       \
  uint8x16_t in_even0 = in_odd0 >> 4;                              \
  uint8x16_t in_odd1 = *(uint8x16_t *)(vec + 2);                   \
  uint8x16_t in_even1 = in_odd1 >> 4;                              \
  uint8x16_t in_odd2 = *(uint8x16_t *)(vec + M_VEC_LIMBS_MAX - 2); \
  uint8x16_t in_even2 = in_odd2 >> 4;                              \
  in_odd0 &= low_nibble_mask;                                      \
  in_odd1 &= low_nibble_mask;                                      \
  in_odd2 &= low_nibble_mask;                                      \
  for (size_t k = 0; k < Len; k += 2) {                            \
    temp[3 * k] ^= vqtbl1q_u8((multabs)[k / 2], in_odd0);          \
    temp[3 * k + 1] ^= vqtbl1q_u8((multabs)[k / 2], in_even0);     \
    temp[3 * k + 2] ^= vqtbl1q_u8((multabs)[k / 2], in_odd1);      \
    temp[3 * k + 3] ^= vqtbl1q_u8((multabs)[k / 2], in_even1);     \
    temp[3 * k + 4] ^= vqtbl1q_u8((multabs)[k / 2], in_odd2);      \
    temp[3 * k + 5] ^= vqtbl1q_u8((multabs)[k / 2], in_even2);     \
  }

// convert to normal format and add to accumulator
#define DISENTANGLE_AND_ACCUMULATE_M_VECS(Len, acc, stride)                                                                 \
  for (size_t k = 0; k + 1 < Len; k += 2) {                                                                                 \
    uint8x16_t acc0 = *(uint8x16_t *)(acc + k * stride + M_VEC_LIMBS_MAX - 2);                                              \
    uint8x16_t acc1 = *(uint8x16_t *)(acc + (k + 1) * stride + M_VEC_LIMBS_MAX - 2);                                        \
    *(uint8x16_t *)(acc + k * stride) ^= vsliq_n_u8(temp[3 * k], temp[3 * k + 1], 4);                                       \
    *(uint8x16_t *)(acc + k * stride + 2) ^= vsliq_n_u8(temp[3 * k + 2], temp[3 * k + 3], 4);                               \
    *(uint8x16_t *)(acc + k * stride + M_VEC_LIMBS_MAX - 2) = acc0 ^ vsliq_n_u8(temp[3 * k + 4], temp[3 * k + 5], 4);       \
    *(uint8x16_t *)(acc + (k + 1) * stride + 0) ^= vsriq_n_u8(temp[3 * k + 1], temp[3 * k], 4);                             \
    *(uint8x16_t *)(acc + (k + 1) * stride + 2) ^= vsriq_n_u8(temp[3 * k + 3], temp[3 * k + 2], 4);                         \
    *(uint8x16_t *)(acc + (k + 1) * stride + M_VEC_LIMBS_MAX - 2) = acc1 ^ vsriq_n_u8(temp[3 * k + 5], temp[3 * k + 4], 4); \
  }                                                                                                                         \
  if (Len % 2 == 1) {                                                                                                       \
    const size_t k = Len - 1;                                                                                               \
    uint8x16_t acc0 = *(uint8x16_t *)(acc + k * stride + M_VEC_LIMBS_MAX - 2);                                              \
    *(uint8x16_t *)(acc + k * stride) ^= vsliq_n_u8(temp[3 * k], temp[3 * k + 1], 4);                                       \
    *(uint8x16_t *)(acc + k * stride + 2) ^= vsliq_n_u8(temp[3 * k + 2], temp[3 * k + 3], 4);                               \
    *(uint8x16_t *)(acc + k * stride + M_VEC_LIMBS_MAX - 2) = acc0 ^ vsliq_n_u8(temp[3 * k + 4], temp[3 * k + 5], 4);       \
  }
#elif M_MAX <= 128
// Multiply an m_vec with a vec of "Len" scalars, and accumulate in temp
#define MULTIPLY_ACCUMULATE_M_VEC(vec, multabs, Len)               \
  uint8x16_t in_odd0 = *(uint8x16_t *)(vec);                       \
  uint8x16_t in_even0 = in_odd0 >> 4;                              \
  uint8x16_t in_odd1 = *(uint8x16_t *)(vec + 2);                   \
  uint8x16_t in_even1 = in_odd1 >> 4;                              \
  uint8x16_t in_odd2 = *(uint8x16_t *)(vec + 4);                   \
  uint8x16_t in_even2 = in_odd2 >> 4;                              \
  uint8x16_t in_odd3 = *(uint8x16_t *)(vec + M_VEC_LIMBS_MAX - 2); \
  uint8x16_t in_even3 = in_odd3 >> 4;                              \
  in_odd0 &= low_nibble_mask;                                      \
  in_odd1 &= low_nibble_mask;                                      \
  in_odd2 &= low_nibble_mask;                                      \
  in_odd3 &= low_nibble_mask;                                      \
  for (size_t k = 0; k < Len; k += 2) {                            \
    temp[4 * k] ^= vqtbl1q_u8((multabs)[k / 2], in_odd0);          \
    temp[4 * k + 1] ^= vqtbl1q_u8((multabs)[k / 2], in_even0);     \
    temp[4 * k + 2] ^= vqtbl1q_u8((multabs)[k / 2], in_odd1);      \
    temp[4 * k + 3] ^= vqtbl1q_u8((multabs)[k / 2], in_even1);     \
    temp[4 * k + 4] ^= vqtbl1q_u8((multabs)[k / 2], in_odd2);      \
    temp[4 * k + 5] ^= vqtbl1q_u8((multabs)[k / 2], in_even2);     \
    temp[4 * k + 6] ^= vqtbl1q_u8((multabs)[k / 2], in_odd3);      \
    temp[4 * k + 7] ^= vqtbl1q_u8((multabs)[k / 2], in_even3);     \
  }

// convert to normal format and add to accumulator
#define DISENTANGLE_AND_ACCUMULATE_M_VECS(Len, acc, stride)                                                                 \
  for (size_t k = 0; k + 1 < Len; k += 2) {                                                                                 \
    uint8x16_t acc0 = *(uint8x16_t *)(acc + k * stride + M_VEC_LIMBS_MAX - 2);                                              \
    uint8x16_t acc1 = *(uint8x16_t *)(acc + (k + 1) * stride + M_VEC_LIMBS_MAX - 2);                                        \
    *(uint8x16_t *)(acc + k * stride) ^= vsliq_n_u8(temp[4 * k], temp[4 * k + 1], 4);                                       \
    *(uint8x16_t *)(acc + k * stride + 2) ^= vsliq_n_u8(temp[4 * k + 2], temp[4 * k + 3], 4);                               \
    *(uint8x16_t *)(acc + k * stride + 4) ^= vsliq_n_u8(temp[4 * k + 4], temp[4 * k + 5], 4);                               \
    *(uint8x16_t *)(acc + k * stride + M_VEC_LIMBS_MAX - 2) = acc0 ^ vsliq_n_u8(temp[4 * k + 6], temp[4 * k + 7], 4);       \
    *(uint8x16_t *)(acc + (k + 1) * stride + 0) ^= vsriq_n_u8(temp[4 * k + 1], temp[4 * k], 4);                             \
    *(uint8x16_t *)(acc + (k + 1) * stride + 2) ^= vsriq_n_u8(temp[4 * k + 3], temp[4 * k + 2], 4);                         \
    *(uint8x16_t *)(acc + (k + 1) * stride + 4) ^= vsriq_n_u8(temp[4 * k + 5], temp[4 * k + 4], 4);                         \
    *(uint8x16_t *)(acc + (k + 1) * stride + M_VEC_LIMBS_MAX - 2) = acc1 ^ vsriq_n_u8(temp[4 * k + 7], temp[4 * k + 6], 4); \
  }                                                                                                                         \
  if (Len % 2 == 1) {                                                                                                       \
    const size_t k = Len - 1;                                                                                               \
    uint8x16_t acc0 = *(uint8x16_t *)(acc + k * stride + M_VEC_LIMBS_MAX - 2);                                              \
    *(uint8x16_t *)(acc + k * stride) ^= vsliq_n_u8(temp[4 * k], temp[4 * k + 1], 4);                                       \
    *(uint8x16_t *)(acc + k * stride + 2) ^= vsliq_n_u8(temp[4 * k + 2], temp[4 * k + 3], 4);                               \
    *(uint8x16_t *)(acc + k * stride + 4) ^= vsliq_n_u8(temp[4 * k + 4], temp[4 * k + 5], 4);                               \
    *(uint8x16_t *)(acc + k * stride + M_VEC_LIMBS_MAX - 2) = acc0 ^ vsliq_n_u8(temp[4 * k + 6], temp[4 * k + 7], 4);       \
  }
#elif M_MAX <= 160
// Multiply an m_vec with a vec of "Len" scalars, and accumulate in temp
#define MULTIPLY_ACCUMULATE_M_VEC(vec, multabs, Len)               \
  uint8x16_t in_odd0 = *(uint8x16_t *)(vec);                       \
  uint8x16_t in_even0 = in_odd0 >> 4;                              \
  uint8x16_t in_odd1 = *(uint8x16_t *)(vec + 2);                   \
  uint8x16_t in_even1 = in_odd1 >> 4;                              \
  uint8x16_t in_odd2 = *(uint8x16_t *)(vec + 4);                   \
  uint8x16_t in_even2 = in_odd2 >> 4;                              \
  uint8x16_t in_odd3 = *(uint8x16_t *)(vec + 6);                   \
  uint8x16_t in_even3 = in_odd3 >> 4;                              \
  uint8x16_t in_odd4 = *(uint8x16_t *)(vec + M_VEC_LIMBS_MAX - 2); \
  uint8x16_t in_even4 = in_odd4 >> 4;                              \
  in_odd0 &= low_nibble_mask;                                      \
  in_odd1 &= low_nibble_mask;                                      \
  in_odd2 &= low_nibble_mask;                                      \
  in_odd3 &= low_nibble_mask;                                      \
  in_odd4 &= low_nibble_mask;                                      \
  for (size_t k = 0; k < Len; k += 2) {                            \
    temp[5 * k] ^= vqtbl1q_u8((multabs)[k / 2], in_odd0);          \
    temp[5 * k + 1] ^= vqtbl1q_u8((multabs)[k / 2], in_even0);     \
    temp[5 * k + 2] ^= vqtbl1q_u8((multabs)[k / 2], in_odd1);      \
    temp[5 * k + 3] ^= vqtbl1q_u8((multabs)[k / 2], in_even1);     \
    temp[5 * k + 4] ^= vqtbl1q_u8((multabs)[k / 2], in_odd2);      \
    temp[5 * k + 5] ^= vqtbl1q_u8((multabs)[k / 2], in_even2);     \
    temp[5 * k + 6] ^= vqtbl1q_u8((multabs)[k / 2], in_odd3);      \
    temp[5 * k + 7] ^= vqtbl1q_u8((multabs)[k / 2], in_even3);     \
    temp[5 * k + 8] ^= vqtbl1q_u8((multabs)[k / 2], in_odd4);      \
    temp[5 * k + 9] ^= vqtbl1q_u8((multabs)[k / 2], in_even4);     \
  }

// convert to normal format and add to accumulator
#define DISENTANGLE_AND_ACCUMULATE_M_VECS(Len, acc, stride)                                                                 \
  for (size_t k = 0; k + 1 < Len; k += 2) {                                                                                 \
    uint8x16_t acc0 = *(uint8x16_t *)(acc + k * stride + M_VEC_LIMBS_MAX - 2);                                              \
    uint8x16_t acc1 = *(uint8x16_t *)(acc + (k + 1) * stride + M_VEC_LIMBS_MAX - 2);                                        \
    *(uint8x16_t *)(acc + k * stride) ^= vsliq_n_u8(temp[5 * k], temp[5 * k + 1], 4);                                       \
    *(uint8x16_t *)(acc + k * stride + 2) ^= vsliq_n_u8(temp[5 * k + 2], temp[5 * k + 3], 4);                               \
    *(uint8x16_t *)(acc + k * stride + 4) ^= vsliq_n_u8(temp[5 * k + 4], temp[5 * k + 5], 4);                               \
    *(uint8x16_t *)(acc + k * stride + 6) ^= vsliq_n_u8(temp[5 * k + 6], temp[5 * k + 7], 4);                               \
    *(uint8x16_t *)(acc + k * stride + M_VEC_LIMBS_MAX - 2) = acc0 ^ vsliq_n_u8(temp[5 * k + 8], temp[5 * k + 9], 4);       \
    *(uint8x16_t *)(acc + (k + 1) * stride + 0) ^= vsriq_n_u8(temp[5 * k + 1], temp[5 * k], 4);                             \
    *(uint8x16_t *)(acc + (k + 1) * stride + 2) ^= vsriq_n_u8(temp[5 * k + 3], temp[5 * k + 2], 4);                         \
    *(uint8x16_t *)(acc + (k + 1) * stride + 4) ^= vsriq_n_u8(temp[5 * k + 5], temp[5 * k + 4], 4);                         \
    *(uint8x16_t *)(acc + (k + 1) * stride + 6) ^= vsriq_n_u8(temp[5 * k + 7], temp[5 * k + 6], 4);                         \
    *(uint8x16_t *)(acc + (k + 1) * stride + M_VEC_LIMBS_MAX - 2) = acc1 ^ vsriq_n_u8(temp[5 * k + 9], temp[5 * k + 8], 4); \
  }                                                                                                                         \
  if (Len % 2 == 1) {                                                                                                       \
    const size_t k = Len - 1;                                                                                               \
    uint8x16_t acc0 = *(uint8x16_t *)(acc + k * stride + M_VEC_LIMBS_MAX - 2);                                              \
    *(uint8x16_t *)(acc + k * stride) ^= vsliq_n_u8(temp[5 * k], temp[5 * k + 1], 4);                                       \
    *(uint8x16_t *)(acc + k * stride + 2) ^= vsliq_n_u8(temp[5 * k + 2], temp[5 * k + 3], 4);                               \
    *(uint8x16_t *)(acc + k * stride + 4) ^= vsliq_n_u8(temp[5 * k + 4], temp[5 * k + 5], 4);                               \
    *(uint8x16_t *)(acc + k * stride + 6) ^= vsliq_n_u8(temp[5 * k + 6], temp[5 * k + 7], 4);                               \
    *(uint8x16_t *)(acc + k * stride + M_VEC_LIMBS_MAX - 2) = acc0 ^ vsliq_n_u8(temp[5 * k + 8], temp[5 * k + 9], 4);       \
  }
#else
    NOT IMPLEMENTED
#endif

// P1*0 -> P1: v x v, O: v x o
static
inline void P1_times_O(const uint64_t *P1, uint8x16_t *O_multabs, uint64_t *acc){
    const uint8x16_t low_nibble_mask  = vdupq_n_u8( 0xf );
    size_t limbs_used = 0;
    for (size_t r = 0; r < V_MAX; r++)
    {
        // do multiplications for one row and accumulate results in temporary format
        uint8x16_t temp[O_NEON_ROUND_UP*NEON_REGISTER_PER_M_VEC] = {0};
        for (size_t c = r; c < V_MAX; c++)
        {
            MULTIPLY_ACCUMULATE_M_VEC(P1 + limbs_used, O_multabs + O_NEON_ROUND_UP/2*c, O_MAX);
            limbs_used += M_VEC_LIMBS_MAX;
        }
        DISENTANGLE_AND_ACCUMULATE_M_VECS(O_MAX , acc + r*O_MAX*M_VEC_LIMBS_MAX, M_VEC_LIMBS_MAX);
    }
}

static
inline void Ot_times_P1O_P2(const uint64_t *P1O_P2, uint8x16_t *O_multabs, uint64_t *acc){
    const uint8x16_t low_nibble_mask  = vdupq_n_u8( 0xf );
    for (size_t c = 0; c < O_MAX; c++)
    {
        // do multiplications for one row and accumulate results in temporary format
        uint8x16_t temp[O_NEON_ROUND_UP*NEON_REGISTER_PER_M_VEC] = {0};
        for (size_t r = 0; r < V_MAX; r++)
        {
            MULTIPLY_ACCUMULATE_M_VEC(P1O_P2 + (r*O_MAX + c)*M_VEC_LIMBS_MAX, O_multabs + O_NEON_ROUND_UP/2*r, O_MAX);
        }
        DISENTANGLE_AND_ACCUMULATE_M_VECS(O_MAX , acc + c*M_VEC_LIMBS_MAX, O_MAX*M_VEC_LIMBS_MAX);
    }
}

static
inline void P1P1t_times_O(const mayo_params_t* p, const uint64_t *P1, const unsigned char *O, uint64_t *acc){
    (void) p;
    const uint8x16_t low_nibble_mask  = vdupq_n_u8( 0xf );

    uint8x16_t O_multabs[O_NEON_ROUND_UP/2*V_MAX];
    mayo_O_multabs(O, O_multabs);

    size_t cols_used = 0;
    for (size_t r = 0; r < V_MAX; r++)
    {
        // do multiplications for one row and accumulate results in temporary format
        uint8x16_t temp[O_NEON_ROUND_UP*NEON_REGISTER_PER_M_VEC] = {0};
        cols_used += 1;
        size_t pos = r;
        for (size_t c = 0; c < r; c++)
        {
            MULTIPLY_ACCUMULATE_M_VEC(P1 + pos * M_VEC_LIMBS_MAX, O_multabs + O_NEON_ROUND_UP/2*c, O_MAX);
            pos += (V_MAX -c - 1);
        }

        for (size_t c = r+1; c < V_MAX; c++)
        {
            MULTIPLY_ACCUMULATE_M_VEC(P1 + cols_used * M_VEC_LIMBS_MAX, O_multabs + O_NEON_ROUND_UP/2*c, O_MAX);
            cols_used ++;
        }
        DISENTANGLE_AND_ACCUMULATE_M_VECS(O_MAX, acc + r*O_MAX*M_VEC_LIMBS_MAX, M_VEC_LIMBS_MAX);
    }
}

static
inline void Vt_times_L(const uint64_t *L, const uint8x16_t *V_multabs, uint64_t *acc){
    const uint8x16_t low_nibble_mask  = vdupq_n_u8( 0xf );

    for (size_t c = 0; c < O_MAX; c++)
    {
        // do multiplications for one row and accumulate results in temporary format
        uint8x16_t temp[K_OVER_2*2*NEON_REGISTER_PER_M_VEC] = {0};
        for (size_t r = 0; r < V_MAX; r++)
        {
            MULTIPLY_ACCUMULATE_M_VEC(L + (r*O_MAX + c) * M_VEC_LIMBS_MAX, V_multabs + K_OVER_2*r, K_MAX);
        }
        DISENTANGLE_AND_ACCUMULATE_M_VECS(K_MAX , acc + c*M_VEC_LIMBS_MAX, O_MAX*M_VEC_LIMBS_MAX);
    }
}

static
inline void Vt_times_Pv(const uint64_t *Pv, const uint8x16_t *V_multabs, uint64_t *acc){
    const uint8x16_t low_nibble_mask  =  vdupq_n_u8( 0xf );

    for (size_t c = 0; c < K_MAX; c++)
    {
        // do multiplications for one row and accumulate results in temporary format
        uint8x16_t temp[K_OVER_2*2*NEON_REGISTER_PER_M_VEC] = {0};
        for (size_t r = 0; r < V_MAX; r++)
        {
            MULTIPLY_ACCUMULATE_M_VEC(Pv + (r*K_MAX + c) * M_VEC_LIMBS_MAX, V_multabs + K_OVER_2*r, K_MAX);
        }
        DISENTANGLE_AND_ACCUMULATE_M_VECS(K_MAX , acc + c*M_VEC_LIMBS_MAX, K_MAX*M_VEC_LIMBS_MAX);
    }
}

static
inline void P1_times_Vt(const uint64_t *P1, uint8x16_t *V_multabs, uint64_t *acc){
    const uint8x16_t low_nibble_mask  = vdupq_n_u8( 0xf );

    size_t cols_used = 0;
    for (size_t r = 0; r < V_MAX; r++)
    {
        // do multiplications for one row and accumulate results in temporary format
        uint8x16_t temp[K_OVER_2*2*NEON_REGISTER_PER_M_VEC] = {0};

        for (size_t c=r; c < V_MAX; c++)
        {
            MULTIPLY_ACCUMULATE_M_VEC(P1 + cols_used, V_multabs + K_OVER_2*c, K_MAX);
            cols_used += M_VEC_LIMBS_MAX;
        }
        DISENTANGLE_AND_ACCUMULATE_M_VECS(K_MAX , acc + r*K_MAX*M_VEC_LIMBS_MAX, M_VEC_LIMBS_MAX);
    }
}

// P1*S1 -> P1: v x v, S1: v x k // P1 upper triangular
// same as mayo_12_P1_times_Vt
static
inline void P1_times_S1(const uint64_t *_P1, uint8x16_t *S1_multabs, uint64_t *_acc){
    P1_times_Vt(_P1, S1_multabs, _acc);
}

static
inline void S1t_times_PS1(const uint64_t *_PS1, uint8x16_t *S1_multabs, uint64_t *_acc){
    Vt_times_Pv(_PS1, S1_multabs, _acc);
}

static
inline void S2t_times_PS2(const uint64_t *PS2, uint8x16_t *S2_multabs, uint64_t *acc){
    const uint8x16_t low_nibble_mask  = vdupq_n_u8( 0xf );

    for (size_t c = 0; c < K_MAX; c++)
    {
        // do multiplications for one row and accumulate results in temporary format
        uint8x16_t temp[K_OVER_2*2*NEON_REGISTER_PER_M_VEC] = {0};
        for (size_t r = 0; r < O_MAX; r++)
        {
            MULTIPLY_ACCUMULATE_M_VEC(PS2 + (r*K_MAX + c)*M_VEC_LIMBS_MAX, S2_multabs + K_OVER_2*r, K_MAX);
        }
        DISENTANGLE_AND_ACCUMULATE_M_VECS(K_MAX , acc + c*M_VEC_LIMBS_MAX, K_MAX*M_VEC_LIMBS_MAX);
    }
}


// P2*S2 -> P2: v x o, S2: o x k
static
inline void P1_times_S1_plus_P2_times_S2(const uint64_t *P1, const uint64_t *P2, uint8x16_t *S1_multabs, uint8x16_t *S2_multabs, uint64_t *acc){
    const uint8x16_t low_nibble_mask  = vdupq_n_u8( 0xf );

    size_t P1_cols_used = 0;
    for (size_t r = 0; r < V_MAX; r++)
    {
        // do multiplications for one row and accumulate results in temporary format
        uint8x16_t temp[K_OVER_2*2*NEON_REGISTER_PER_M_VEC] = {0};

        // P1 * S1
        for (size_t c = r; c < V_MAX; c++)
        {
            MULTIPLY_ACCUMULATE_M_VEC(P1 + P1_cols_used, S1_multabs + K_OVER_2*c, K_MAX);
            P1_cols_used += M_VEC_LIMBS_MAX;
        }

        // P2 * S2
        for (size_t c = 0; c < O_MAX; c++)
        {
            MULTIPLY_ACCUMULATE_M_VEC(P2 + (r*O_MAX + c)*M_VEC_LIMBS_MAX, S2_multabs + K_OVER_2*c, K_MAX);
        }

        DISENTANGLE_AND_ACCUMULATE_M_VECS(K_MAX , acc + r*K_MAX*M_VEC_LIMBS_MAX, M_VEC_LIMBS_MAX);
    }
}


// P3*S2 -> P3: o x o, S2: o x k // P3 upper triangular
static
inline void P3_times_S2(const uint64_t *P3, uint8x16_t *S2_multabs, uint64_t *acc){
    const uint8x16_t low_nibble_mask  = vdupq_n_u8( 0xf );

    size_t cols_used = 0;
    for (size_t r = 0; r < O_MAX; r++)
    {
        // do multiplications for one row and accumulate results in temporary format
        uint8x16_t temp[K_OVER_2*2*NEON_REGISTER_PER_M_VEC] = {0};
        for (size_t c = r; c < O_MAX; c++)
        {
            MULTIPLY_ACCUMULATE_M_VEC(P3 + cols_used, S2_multabs + K_OVER_2*c, K_MAX);
            cols_used += M_VEC_LIMBS_MAX;
        }

        DISENTANGLE_AND_ACCUMULATE_M_VECS(K_MAX , acc + r*K_MAX*M_VEC_LIMBS_MAX, M_VEC_LIMBS_MAX);
    }
}

static inline
void compute_M_and_VPV(const mayo_params_t* p, const unsigned char* Vdec, const uint64_t *L, const uint64_t *P1, uint64_t *VL, uint64_t *VP1V){
    (void) p;
    uint8x16_t V_multabs[(K_MAX+1)/2*V_MAX];
    mayo_V_multabs(Vdec, V_multabs);

    // M
    Vt_times_L(L, V_multabs, VL);

    // VP1V
    uint64_t Pv[V_MAX * K_MAX * M_VEC_LIMBS_MAX] = {0};
    P1_times_Vt(P1, V_multabs, Pv);
    Vt_times_Pv(Pv, V_multabs, VP1V);
}

static inline
void compute_P3(const mayo_params_t* p, const uint64_t* P1, uint64_t *P2, const unsigned char *O, uint64_t *P3){
    (void) p;
    uint8x16_t O_multabs[O_NEON_ROUND_UP/2*V_MAX];
    mayo_O_multabs(O, O_multabs);
    P1_times_O(P1, O_multabs, P2);
    Ot_times_P1O_P2(P2, O_multabs, P3);
}

// compute P * S^t = [ P1  P2 ] * [S1] = [P1*S1 + P2*S2]
//                   [  0  P3 ]   [S2]   [        P3*S2]
// compute S * PS  = [ S1 S2 ] * [ P1*S1 + P2*S2 = P1 ] = [ S1*P1 + S2*P2 ]
//                               [         P3*S2 = P2 ]
static inline void m_calculate_PS_SPS(const mayo_params_t *p, const uint64_t *P1, const uint64_t *P2, const uint64_t *P3, const unsigned char *S,
                              uint64_t *SPS) {
    (void) p;
    const int o = PARAM_NAME(o);
    const int v = PARAM_NAME(v);
    const int k = PARAM_NAME(k);
    const int n = o + v;
    /* Old approach which is constant time but doesn't have to be */
    unsigned char S1[V_MAX*K_MAX]; // == N-O, K
    unsigned char S2[O_MAX*K_MAX]; // == O, K
    unsigned char *s1_write = S1;
    unsigned char *s2_write = S2;

    for (int r=0; r < k; r++)
    {
        for (int c = 0; c < n; c++)
        {
            if(c < v){
                *(s1_write++) = S[r*n + c];
            } else {
                *(s2_write++) = S[r*n + c];
            }
        }
    }

    uint64_t PS[N_MAX * K_MAX * M_VEC_LIMBS_MAX] = { 0 };
    (void) PS;

    uint8x16_t S1_multabs[(K_MAX+1)/2*V_MAX];
    uint8x16_t S2_multabs[(K_MAX+1)/2*O_MAX];
    mayo_S1_multabs(S1, S1_multabs);
    mayo_S2_multabs(S2, S2_multabs);

    P1_times_S1_plus_P2_times_S2(P1, P2, S1_multabs, S2_multabs, PS);
    P3_times_S2(P3, S2_multabs, PS + V_MAX*K_MAX*M_VEC_LIMBS_MAX); // upper triangular

    // S^T * PS = S1^t*PS1 + S2^t*PS2
    S1t_times_PS1(PS, S1_multabs, SPS);
    S2t_times_PS2(PS + V_MAX*K_MAX*M_VEC_LIMBS_MAX, S2_multabs, SPS);
}


#undef K_OVER_2
#endif

