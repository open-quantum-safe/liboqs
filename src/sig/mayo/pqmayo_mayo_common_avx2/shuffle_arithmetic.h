// SPDX-License-Identifier: Apache-2.0

#ifndef SHUFFLE_ARITHMETIC_H
#define SHUFFLE_ARITHMETIC_H

#include <stdint.h>
#include <mayo.h>
#include <immintrin.h>
#include <arithmetic_common.h>
#include <arithmetic_fixed.h>

//#define TICTOC
//#include <debug_bench_tools.h>

#define O_AVX_ROUND_UP ((O_MAX + 1)/2*2)

#define AVX_REGISTER_PER_M_VEC ((M_MAX + 63)/64)

#if M_MAX <= 64
// Multiply an m_vec with a vec of "Len" scalars, and accumulate in temp
#define MULTIPLY_ACCUMULATE_M_VEC(vec, multabs, Len)                                     \
    __m256i in_odd = _mm256_loadu_si256((__m256i *) (vec));                              \
    __m256i in_even = _mm256_srli_epi16(in_odd, 4) & low_nibble_mask;                    \
    in_odd &= low_nibble_mask;                                                           \
    for (size_t k = 0; k < Len; k+=2)                                                    \
    {                                                                                    \
        temp[k]     ^= _mm256_shuffle_epi8(*(multabs + k/2), in_odd);                    \
        temp[k + 1] ^= _mm256_shuffle_epi8(*(multabs + k/2), in_even);                   \
    }        

// convert to normal format and add to accumulator 
#define DISENTANGLE_AND_ACCUMULATE_M_VECS(Len , acc, stride)                             \
    for (size_t k = 0; k+1 < Len; k+=2)                                                  \
    {                                                                                    \
        __m256i acc0 = _mm256_loadu_si256((__m256i *)(acc +      k * stride));           \
        __m256i acc1 = _mm256_loadu_si256((__m256i *)(acc + (k + 1)* stride));           \
                                                                                         \
        __m256i t = (temp[k + 1] ^ _mm256_srli_epi16(temp[k],4)) & low_nibble_mask;      \
        acc0 ^= temp[k] ^ _mm256_slli_epi16(t,4);                                        \
        acc1 ^= temp[k+1] ^ t;                                                           \
                                                                                         \
        _mm256_storeu_si256((__m256i *)(acc +       k * stride), acc0);                  \
        _mm256_storeu_si256((__m256i *)(acc + (k + 1) * stride), acc1);                  \
    }                                                                                    \
    if (Len % 2 == 1)                                                                    \
    {                                                                                    \
        __m256i acc0 = _mm256_loadu_si256((__m256i *)(acc + (Len-1) * stride));          \
                                                                                         \
        __m256i t = (temp[Len] ^ _mm256_srli_epi16(temp[Len - 1],4)) & low_nibble_mask;  \
        acc0 ^= temp[Len-1] ^ _mm256_slli_epi16(t,4);                                    \
                                                                                         \
        _mm256_storeu_si256((__m256i *)(acc + (Len-1) * stride),     acc0);              \
    }                                                                           
#elif M_MAX <= 128
// Multiply an m_vec with a vec of "Len" scalars, and accumulate in temp
#define MULTIPLY_ACCUMULATE_M_VEC(vec, multabs, Len)                                     \
    __m256i in_odd0 = _mm256_loadu_si256((__m256i *) (vec));                             \
    __m256i in_even0 = _mm256_srli_epi16(in_odd0, 4) & low_nibble_mask;                  \
    __m256i in_odd1 = _mm256_loadu_si256((__m256i *) (vec + M_VEC_LIMBS_MAX - 4));       \
    __m256i in_even1 = _mm256_srli_epi16(in_odd1, 4) & low_nibble_mask;                  \
    in_odd0 &= low_nibble_mask;                                                          \
    in_odd1 &= low_nibble_mask;                                                          \
    for (size_t k = 0; k < Len; k+=2)                                                    \
    {                                                                                    \
        temp[2*k]     ^= _mm256_shuffle_epi8(*(multabs + k/2), in_odd0);                 \
        temp[2*k + 1] ^= _mm256_shuffle_epi8(*(multabs + k/2), in_even0);                \
        temp[2*k + 2] ^= _mm256_shuffle_epi8(*(multabs + k/2), in_odd1);                 \
        temp[2*k + 3] ^= _mm256_shuffle_epi8(*(multabs + k/2), in_even1);                \
    }        

// convert to normal format and add to accumulator 
#define DISENTANGLE_AND_ACCUMULATE_M_VECS(Len , acc, stride)                                         \
    for (size_t k = 0; k+1 < Len; k+=2)                                                              \
    {                                                                                                \
        __m256i acc0 = _mm256_loadu_si256((__m256i *)(acc +      k * stride));                       \
        __m256i acc1 = _mm256_loadu_si256((__m256i *)(acc +      k * stride + M_VEC_LIMBS_MAX - 4)); \
        __m256i acc2 = _mm256_loadu_si256((__m256i *)(acc + (k + 1)* stride));                       \
        __m256i acc3 = _mm256_loadu_si256((__m256i *)(acc + (k + 1)* stride + M_VEC_LIMBS_MAX - 4)); \
                                                                                                     \
        __m256i t0 = (temp[2*k + 1] ^ _mm256_srli_epi16(temp[2*k    ],4)) & low_nibble_mask;         \
        __m256i t1 = (temp[2*k + 3] ^ _mm256_srli_epi16(temp[2*k + 2],4)) & low_nibble_mask;         \
        acc0 ^= temp[2*k    ] ^ _mm256_slli_epi16(t0,4);                                             \
        acc1 ^= temp[2*k + 2] ^ _mm256_slli_epi16(t1,4);                                             \
        acc2 ^= temp[2*k + 1] ^ t0;                                                                  \
        acc3 ^= temp[2*k + 3] ^ t1;                                                                  \
                                                                                                     \
        _mm256_storeu_si256((__m256i *)(acc +       k * stride), acc0);                              \
        _mm256_storeu_si256((__m256i *)(acc +       k * stride + M_VEC_LIMBS_MAX - 4), acc1);        \
        _mm256_storeu_si256((__m256i *)(acc + (k + 1) * stride), acc2);                              \
        _mm256_storeu_si256((__m256i *)(acc + (k + 1) * stride + M_VEC_LIMBS_MAX - 4), acc3);        \
    }                                                                                                \
    if (Len % 2 == 1)                                                                                \
    {                                                                                                \
        const size_t k = Len - 1;                                                                    \
        __m256i acc0 = _mm256_loadu_si256((__m256i *)(acc + k * stride));                            \
        __m256i acc1 = _mm256_loadu_si256((__m256i *)(acc + k * stride + M_VEC_LIMBS_MAX - 4));      \
                                                                                                     \
        __m256i t0 = (temp[2*k + 1] ^ _mm256_srli_epi16(temp[2*k    ],4)) & low_nibble_mask;         \
        __m256i t1 = (temp[2*k + 3] ^ _mm256_srli_epi16(temp[2*k + 2],4)) & low_nibble_mask;         \
        acc0 ^= temp[2*k    ] ^ _mm256_slli_epi16(t0,4);                                             \
        acc1 ^= temp[2*k + 2] ^ _mm256_slli_epi16(t1,4);                                             \
                                                                                                     \
        _mm256_storeu_si256((__m256i *)(acc + k * stride), acc0);                                    \
        _mm256_storeu_si256((__m256i *)(acc + k * stride + M_VEC_LIMBS_MAX - 4), acc1);              \
    }        
#elif M_MAX <= 192
// Multiply an m_vec with a vec of "Len" scalars, and accumulate in temp
#define MULTIPLY_ACCUMULATE_M_VEC(vec, multabs, Len)                                     \
    __m256i in_odd0 = _mm256_loadu_si256((__m256i *) (vec));                             \
    __m256i in_even0 = _mm256_srli_epi16(in_odd0, 4) & low_nibble_mask;                  \
    __m256i in_odd1 = _mm256_loadu_si256((__m256i *) (vec + 4));                         \
    __m256i in_even1 = _mm256_srli_epi16(in_odd1, 4) & low_nibble_mask;                  \
    __m256i in_odd2 = _mm256_loadu_si256((__m256i *) (vec + M_VEC_LIMBS_MAX - 4));       \
    __m256i in_even2 = _mm256_srli_epi16(in_odd2, 4) & low_nibble_mask;                  \
    in_odd0 &= low_nibble_mask;                                                          \
    in_odd1 &= low_nibble_mask;                                                          \
    in_odd2 &= low_nibble_mask;                                                          \
    for (size_t k = 0; k < Len; k+=2)                                                    \
    {                                                                                    \
        temp[3*k]     ^= _mm256_shuffle_epi8(*(multabs + k/2), in_odd0);                 \
        temp[3*k + 1] ^= _mm256_shuffle_epi8(*(multabs + k/2), in_even0);                \
        temp[3*k + 2] ^= _mm256_shuffle_epi8(*(multabs + k/2), in_odd1);                 \
        temp[3*k + 3] ^= _mm256_shuffle_epi8(*(multabs + k/2), in_even1);                \
        temp[3*k + 4] ^= _mm256_shuffle_epi8(*(multabs + k/2), in_odd2);                 \
        temp[3*k + 5] ^= _mm256_shuffle_epi8(*(multabs + k/2), in_even2);                \
    }        

// convert to normal format and add to accumulator 
#define DISENTANGLE_AND_ACCUMULATE_M_VECS(Len , acc, stride)                                         \
    for (size_t k = 0; k+1 < Len; k+=2)                                                              \
    {                                                                                                \
        __m256i acc0 = _mm256_loadu_si256((__m256i *)(acc +      k * stride));                       \
        __m256i acc1 = _mm256_loadu_si256((__m256i *)(acc +      k * stride + 4));                   \
        __m256i acc2 = _mm256_loadu_si256((__m256i *)(acc +      k * stride + M_VEC_LIMBS_MAX - 4)); \
        __m256i acc3 = _mm256_loadu_si256((__m256i *)(acc + (k + 1)* stride));                       \
        __m256i acc4 = _mm256_loadu_si256((__m256i *)(acc + (k + 1)* stride + 4));                   \
        __m256i acc5 = _mm256_loadu_si256((__m256i *)(acc + (k + 1)* stride + M_VEC_LIMBS_MAX - 4)); \
                                                                                                     \
        __m256i t0 = (temp[3*k + 1] ^ _mm256_srli_epi16(temp[3*k    ],4)) & low_nibble_mask;         \
        __m256i t1 = (temp[3*k + 3] ^ _mm256_srli_epi16(temp[3*k + 2],4)) & low_nibble_mask;         \
        __m256i t2 = (temp[3*k + 5] ^ _mm256_srli_epi16(temp[3*k + 4],4)) & low_nibble_mask;         \
        acc0 ^= temp[3*k    ] ^ _mm256_slli_epi16(t0,4);                                             \
        acc1 ^= temp[3*k + 2] ^ _mm256_slli_epi16(t1,4);                                             \
        acc2 ^= temp[3*k + 4] ^ _mm256_slli_epi16(t2,4);                                             \
        acc3 ^= temp[3*k + 1] ^ t0;                                                                  \
        acc4 ^= temp[3*k + 3] ^ t1;                                                                  \
        acc5 ^= temp[3*k + 5] ^ t2;                                                                  \
                                                                                                     \
        _mm256_storeu_si256((__m256i *)(acc +       k * stride), acc0);                              \
        _mm256_storeu_si256((__m256i *)(acc +       k * stride + 4), acc1);                          \
        _mm256_storeu_si256((__m256i *)(acc +       k * stride + M_VEC_LIMBS_MAX - 4), acc2);        \
        _mm256_storeu_si256((__m256i *)(acc + (k + 1) * stride), acc3);                              \
        _mm256_storeu_si256((__m256i *)(acc + (k + 1) * stride + 4), acc4);                          \
        _mm256_storeu_si256((__m256i *)(acc + (k + 1) * stride + M_VEC_LIMBS_MAX - 4), acc5);        \
    }                                                                                                \
    if (Len % 2 == 1)                                                                                \
    {                                                                                                \
        const size_t k = Len - 1;                                                                    \
        __m256i acc0 = _mm256_loadu_si256((__m256i *)(acc +      k * stride));                       \
        __m256i acc1 = _mm256_loadu_si256((__m256i *)(acc +      k * stride + 4));                   \
        __m256i acc2 = _mm256_loadu_si256((__m256i *)(acc +      k * stride + M_VEC_LIMBS_MAX - 4)); \
                                                                                                     \
        __m256i t0 = (temp[3*k + 1] ^ _mm256_srli_epi16(temp[3*k    ],4)) & low_nibble_mask;         \
        __m256i t1 = (temp[3*k + 3] ^ _mm256_srli_epi16(temp[3*k + 2],4)) & low_nibble_mask;         \
        __m256i t2 = (temp[3*k + 5] ^ _mm256_srli_epi16(temp[3*k + 4],4)) & low_nibble_mask;         \
        acc0 ^= temp[3*k    ] ^ _mm256_slli_epi16(t0,4);                                             \
        acc1 ^= temp[3*k + 2] ^ _mm256_slli_epi16(t1,4);                                             \
        acc2 ^= temp[3*k + 4] ^ _mm256_slli_epi16(t2,4);                                             \
                                                                                                     \
        _mm256_storeu_si256((__m256i *)(acc +       k * stride), acc0);                              \
        _mm256_storeu_si256((__m256i *)(acc +       k * stride + 4), acc1);                          \
        _mm256_storeu_si256((__m256i *)(acc +       k * stride + M_VEC_LIMBS_MAX - 4), acc2);        \
    }        
#else
    NOT IMPLEMENTED
#endif

// P1*0 -> P1: v x v, O: v x o
static 
inline void P1_times_O(const uint64_t *P1, __m256i *O_multabs, uint64_t *acc){
    const __m256i low_nibble_mask  = _mm256_set_epi64x(0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f);
    size_t limbs_used = 0;
    for (size_t r = 0; r < V_MAX; r++)
    {
        // do multiplications for one row and accumulate results in temporary format
        __m256i temp[O_AVX_ROUND_UP*AVX_REGISTER_PER_M_VEC] = {0};
        for (size_t c = r; c < V_MAX; c++)
        {
            MULTIPLY_ACCUMULATE_M_VEC(P1 + limbs_used, O_multabs + O_AVX_ROUND_UP/2*c, O_MAX);            
            limbs_used += M_VEC_LIMBS_MAX;
        }
        DISENTANGLE_AND_ACCUMULATE_M_VECS(O_MAX , acc + r*O_MAX*M_VEC_LIMBS_MAX, M_VEC_LIMBS_MAX); 
    }
}

static 
inline void Ot_times_P1O_P2(const uint64_t *P1O_P2, __m256i *O_multabs, uint64_t *acc){
    const __m256i low_nibble_mask  = _mm256_set_epi64x(0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f);

    for (size_t c = 0; c < O_MAX; c++)
    {
        // do multiplications for one row and accumulate results in temporary format
        __m256i temp[O_AVX_ROUND_UP*AVX_REGISTER_PER_M_VEC] = {0};
        for (size_t r = 0; r < V_MAX; r++)
        {
            MULTIPLY_ACCUMULATE_M_VEC(P1O_P2 + (r*O_MAX + c)*M_VEC_LIMBS_MAX, O_multabs + O_AVX_ROUND_UP/2*r, O_MAX);            
        }
        DISENTANGLE_AND_ACCUMULATE_M_VECS(O_MAX , acc + c*M_VEC_LIMBS_MAX, O_MAX*M_VEC_LIMBS_MAX); 
    }
}

static
inline void P1P1t_times_O(const mayo_params_t* p, const uint64_t *P1, const unsigned char *O, uint64_t *acc){
    (void) p;
    const __m256i low_nibble_mask  = _mm256_set_epi64x(0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f);

    __m256i O_multabs[O_AVX_ROUND_UP/2*V_MAX];
    mayo_O_multabs(O, O_multabs);

    size_t cols_used = 0;
    for (size_t r = 0; r < V_MAX; r++)
    {
        // do multiplications for one row and accumulate results in temporary format
        __m256i temp[O_AVX_ROUND_UP*AVX_REGISTER_PER_M_VEC] = {0};
        cols_used += 1;
        size_t pos = r;
        for (size_t c = 0; c < r; c++)
        {
            MULTIPLY_ACCUMULATE_M_VEC(P1 + pos * M_VEC_LIMBS_MAX, O_multabs + O_AVX_ROUND_UP/2*c, O_MAX); 
            pos += (V_MAX -c - 1);              
        }

        for (size_t c = r+1; c < V_MAX; c++)
        {
            MULTIPLY_ACCUMULATE_M_VEC(P1 + cols_used * M_VEC_LIMBS_MAX, O_multabs + O_AVX_ROUND_UP/2*c, O_MAX); 
            cols_used ++;
        }
        DISENTANGLE_AND_ACCUMULATE_M_VECS(O_MAX , acc + r*O_MAX*M_VEC_LIMBS_MAX, M_VEC_LIMBS_MAX); 
    }
}


static 
inline void Vt_times_L(const uint64_t *L, const __m256i *V_multabs, uint64_t *acc){
    const __m256i low_nibble_mask  = _mm256_set_epi64x(0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f);

    for (size_t c = 0; c < O_MAX; c++)
    {
        // do multiplications for one row and accumulate results in temporary format
        __m256i temp[K_OVER_2*2*AVX_REGISTER_PER_M_VEC] = {0};
        for (size_t r = 0; r < V_MAX; r++)
        {  
            MULTIPLY_ACCUMULATE_M_VEC(L + (r*O_MAX + c) * M_VEC_LIMBS_MAX, V_multabs + K_OVER_2*r, K_MAX); 
        }
        DISENTANGLE_AND_ACCUMULATE_M_VECS(K_MAX , acc + c*M_VEC_LIMBS_MAX, O_MAX*M_VEC_LIMBS_MAX); 
    }
}


static 
inline void Vt_times_Pv(const uint64_t *Pv, const __m256i *V_multabs, uint64_t *acc){
    const __m256i low_nibble_mask  = _mm256_set_epi64x(0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f);

    for (size_t c = 0; c < K_MAX; c++)
    {
        // do multiplications for one row and accumulate results in temporary format
        __m256i temp[K_OVER_2*2*AVX_REGISTER_PER_M_VEC] = {0};
        for (size_t r = 0; r < V_MAX; r++)
        {
            MULTIPLY_ACCUMULATE_M_VEC(Pv + (r*K_MAX + c) * M_VEC_LIMBS_MAX, V_multabs + K_OVER_2*r, K_MAX); 
        }
        DISENTANGLE_AND_ACCUMULATE_M_VECS(K_MAX , acc + c*M_VEC_LIMBS_MAX, K_MAX*M_VEC_LIMBS_MAX); 
    }
}

static 
inline void P1_times_Vt(const uint64_t *P1, __m256i *V_multabs, uint64_t *acc){
    const __m256i low_nibble_mask  = _mm256_set_epi64x(0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f);

    size_t cols_used = 0;
    for (size_t r = 0; r < V_MAX; r++)
    {
        // do multiplications for one row and accumulate results in temporary format
        __m256i temp[K_OVER_2*2*AVX_REGISTER_PER_M_VEC] = {0};

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
inline void P1_times_S1(const uint64_t *_P1, __m256i *S1_multabs, uint64_t *_acc){
    P1_times_Vt(_P1, S1_multabs, _acc);
}

static
inline void S1t_times_PS1(const uint64_t *_PS1, __m256i *S1_multabs, uint64_t *_acc){
    Vt_times_Pv(_PS1, S1_multabs, _acc);
}

static
inline void S2t_times_PS2(const uint64_t *PS2, __m256i *S2_multabs, uint64_t *acc){
    const __m256i low_nibble_mask  = _mm256_set_epi64x(0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f);

    for (size_t c = 0; c < K_MAX; c++)
    {
        // do multiplications for one row and accumulate results in temporary format
        __m256i temp[K_OVER_2*2*AVX_REGISTER_PER_M_VEC] = {0};
        for (size_t r = 0; r < O_MAX; r++)
        {
            MULTIPLY_ACCUMULATE_M_VEC(PS2 + (r*K_MAX + c)*M_VEC_LIMBS_MAX, S2_multabs + K_OVER_2*r, K_MAX); 
        }
        DISENTANGLE_AND_ACCUMULATE_M_VECS(K_MAX , acc + c*M_VEC_LIMBS_MAX, K_MAX*M_VEC_LIMBS_MAX);
    }
}

// P2*S2 -> P2: v x o, S2: o x k
static 
inline void P1_times_S1_plus_P2_times_S2(const uint64_t *P1, const uint64_t *P2, __m256i *S1_multabs, __m256i *S2_multabs, uint64_t *acc){
    const __m256i low_nibble_mask  = _mm256_set_epi64x(0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f);

    size_t P1_cols_used = 0;
    for (size_t r = 0; r < V_MAX; r++)
    {
        // do multiplications for one row and accumulate results in temporary format
        __m256i temp[K_OVER_2*2*AVX_REGISTER_PER_M_VEC] = {0};

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
inline void P3_times_S2(const uint64_t *P3, __m256i *S2_multabs, uint64_t *acc){
    const __m256i low_nibble_mask  = _mm256_set_epi64x(0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f);

    size_t cols_used = 0;
    for (size_t r = 0; r < O_MAX; r++)
    {
        // do multiplications for one row and accumulate results in temporary format
        __m256i temp[K_OVER_2*2*AVX_REGISTER_PER_M_VEC] = {0};

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
    __m256i V_multabs[(K_MAX+1)/2*V_MAX];
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
    __m256i O_multabs[O_AVX_ROUND_UP/2*V_MAX];
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

    __m256i S1_multabs[(K_MAX+1)/2*V_MAX];    
    __m256i S2_multabs[(K_MAX+1)/2*O_MAX];
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
