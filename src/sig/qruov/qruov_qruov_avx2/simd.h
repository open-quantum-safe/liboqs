#ifndef SIMD_H_
#define SIMD_H_

#include <x86intrin.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define SIMD_CAT_(a, b) a ## b
#define SIMD_CAT(a, b) SIMD_CAT_(a, b)

static inline uint16_t load_u16 (const void *ptr) { uint16_t x; memcpy(&x, ptr, 2); return x; }
static inline uint32_t load_u32 (const void *ptr) { uint32_t x; memcpy(&x, ptr, 4); return x; }
static inline uint64_t load_u64 (const void *ptr) { uint64_t x; memcpy(&x, ptr, 8); return x; }

static inline void store_u32(void *ptr, uint32_t x) { memcpy(ptr, &x, 4); }
static inline void store_u64(void *ptr, uint64_t x) { memcpy(ptr, &x, 8); }

static inline __m256i ymm_dot_u8s8(__m256i acc32, __m256i u8, __m256i s8)
{
    return _mm256_add_epi32(acc32, _mm256_madd_epi16(_mm256_set1_epi16(1), _mm256_maddubs_epi16(u8, s8)));
}

static inline __m256i ymm2_dot_u8s8(__m256i acc32, __m256i u8_0, __m256i s8_0, __m256i u8_1, __m256i s8_1)
{
    acc32 = ymm_dot_u8s8(acc32, u8_0, s8_0);
    return ymm_dot_u8s8(acc32, u8_1, s8_1);
}

// Unsafe grouped variant of ymm2_dot_u8s8: exact only when the grouped
// signed 16-bit maddubs lane sums cannot overflow.
static inline __m256i ymm2_dot_u8s8_unsafe(__m256i acc32, __m256i u8_0, __m256i s8_0, __m256i u8_1, __m256i s8_1)
{
    __m256i tmp = _mm256_add_epi16(_mm256_maddubs_epi16(u8_0, s8_0), _mm256_maddubs_epi16(u8_1, s8_1));
    return _mm256_add_epi32(acc32, _mm256_madd_epi16(_mm256_set1_epi16(1), tmp));
}

static inline __m256i ymm4_dot_u8s8(__m256i acc32, __m256i u8_0, __m256i s8_0, __m256i u8_1, __m256i s8_1, __m256i u8_2, __m256i s8_2, __m256i u8_3, __m256i s8_3)
{
    acc32 = ymm_dot_u8s8(acc32, u8_0, s8_0);
    acc32 = ymm_dot_u8s8(acc32, u8_1, s8_1);
    acc32 = ymm_dot_u8s8(acc32, u8_2, s8_2);
    return ymm_dot_u8s8(acc32, u8_3, s8_3);
}

// Unsafe grouped variant of ymm4_dot_u8s8: exact only when the grouped
// signed 16-bit maddubs lane sums cannot overflow.
static inline __m256i ymm4_dot_u8s8_unsafe(__m256i acc32, __m256i u8_0, __m256i s8_0, __m256i u8_1, __m256i s8_1, __m256i u8_2, __m256i s8_2, __m256i u8_3, __m256i s8_3)
{
    __m256i tmp0 = _mm256_add_epi16(_mm256_maddubs_epi16(u8_0, s8_0), _mm256_maddubs_epi16(u8_1, s8_1));
    __m256i tmp1 = _mm256_add_epi16(_mm256_maddubs_epi16(u8_2, s8_2), _mm256_maddubs_epi16(u8_3, s8_3));
    return _mm256_add_epi32(acc32, _mm256_madd_epi16(_mm256_set1_epi16(1), _mm256_add_epi16(tmp0, tmp1)));
}

static inline __m128i ymm_cvtusepi16_epi8(__m256i x)
{
    __m128i lo = _mm256_castsi256_si128(x);
    __m128i hi = _mm256_extracti128_si256(x, 1);
    return _mm_packus_epi16(lo, hi);
}
static inline __m128i ymm_cvtusepi32_epi16(__m256i x)
{
    __m128i lo = _mm256_castsi256_si128(x);
    __m128i hi = _mm256_extracti128_si256(x, 1);
    return _mm_packus_epi32(lo, hi);
}

static inline __m256i ymm2_cvtusepi32_epi16(__m256i x0, __m256i x1)
{
    __m128i y0 = ymm_cvtusepi32_epi16(x0);
    __m128i y1 = ymm_cvtusepi32_epi16(x1);
    return _mm256_set_m128i(y1, y0);
}

#define GENERATE_YMM_MODQ_EPI15(Q, W) \
static inline __m256i SIMD_CAT(ymm_mod, SIMD_CAT(Q, _epi15))(__m256i x)\
{\
    __m256i q = _mm256_mulhi_epu16(x, _mm256_set1_epi16((int16_t)(((1 << (16 + W)) + Q - 1) / Q)));\
    q = _mm256_srli_epi16(q, W);\
    x = _mm256_add_epi16(q, x);\
    return _mm256_and_si256(x, _mm256_set1_epi16(Q));\
}

#define GENERATE_YMM_MODQ_EPI32_EPI8(Q) \
static inline __m128i SIMD_CAT(ymm_mod, SIMD_CAT(Q, _epi32_epi8))(__m256i x)\
{\
    x = ymm_dot_u8s8(_mm256_setzero_si256(), x, _mm256_set1_epi32(((1 << 24) % Q) << 24 | ((1 << 16) % Q) << 16 | ((1 << 8) % Q) << 8 | 1));\
    __m128i y = ymm_cvtusepi32_epi16(x);\
    __m256i z = SIMD_CAT(ymm_mod, SIMD_CAT(Q, _epi15))(_mm256_castsi128_si256(y));\
    return _mm_packus_epi16(_mm256_castsi256_si128(z), _mm_setzero_si128());\
}\
static inline __m128i SIMD_CAT(ymm2_mod, SIMD_CAT(Q, _epi32_epi8))(__m256i x0, __m256i x1)\
{\
    x0 = ymm_dot_u8s8(_mm256_setzero_si256(), x0, _mm256_set1_epi32(((1 << 24) % Q) << 24 | ((1 << 16) % Q) << 16 | ((1 << 8) % Q) << 8 | 1));\
    x1 = ymm_dot_u8s8(_mm256_setzero_si256(), x1, _mm256_set1_epi32(((1 << 24) % Q) << 24 | ((1 << 16) % Q) << 16 | ((1 << 8) % Q) << 8 | 1));\
    __m256i y = ymm2_cvtusepi32_epi16(x0, x1);\
    __m256i z = SIMD_CAT(ymm_mod, SIMD_CAT(Q, _epi15))(y);\
    return ymm_cvtusepi16_epi8(z);\
}

GENERATE_YMM_MODQ_EPI15(7, 1)
GENERATE_YMM_MODQ_EPI15(31, 3)
GENERATE_YMM_MODQ_EPI15(127, 6)
GENERATE_YMM_MODQ_EPI32_EPI8(7)
GENERATE_YMM_MODQ_EPI32_EPI8(31)
GENERATE_YMM_MODQ_EPI32_EPI8(127)

static inline void transpose8x8(uint8_t *output, const uint8_t *input, int stride_out, int stride_in)
{
    __m128i r0 = _mm_loadl_epi64((__m128i*)(input + 0*stride_in));
    __m128i r1 = _mm_loadl_epi64((__m128i*)(input + 1*stride_in));
    __m128i r2 = _mm_loadl_epi64((__m128i*)(input + 2*stride_in));
    __m128i r3 = _mm_loadl_epi64((__m128i*)(input + 3*stride_in));
    __m128i r4 = _mm_loadl_epi64((__m128i*)(input + 4*stride_in));
    __m128i r5 = _mm_loadl_epi64((__m128i*)(input + 5*stride_in));
    __m128i r6 = _mm_loadl_epi64((__m128i*)(input + 6*stride_in));
    __m128i r7 = _mm_loadl_epi64((__m128i*)(input + 7*stride_in));
    __m128i t0 = _mm_unpacklo_epi8(r0, r1);
    __m128i t1 = _mm_unpacklo_epi8(r2, r3);
    __m128i t2 = _mm_unpacklo_epi8(r4, r5);
    __m128i t3 = _mm_unpacklo_epi8(r6, r7);
    __m128i s0 = _mm_unpacklo_epi16(t0, t1);
    __m128i s1 = _mm_unpackhi_epi16(t0, t1);
    __m128i s2 = _mm_unpacklo_epi16(t2, t3);
    __m128i s3 = _mm_unpackhi_epi16(t2, t3);
    __m128i o0 = _mm_unpacklo_epi32(s0, s2);
    __m128i o1 = _mm_unpackhi_epi32(s0, s2);
    __m128i o2 = _mm_unpacklo_epi32(s1, s3);
    __m128i o3 = _mm_unpackhi_epi32(s1, s3);
    _mm_storel_epi64((__m128i*)(output + 0*stride_out), o0);
    _mm_storel_epi64((__m128i*)(output + 1*stride_out), _mm_srli_si128(o0, 8));
    _mm_storel_epi64((__m128i*)(output + 2*stride_out), o1);
    _mm_storel_epi64((__m128i*)(output + 3*stride_out), _mm_srli_si128(o1, 8));
    _mm_storel_epi64((__m128i*)(output + 4*stride_out), o2);
    _mm_storel_epi64((__m128i*)(output + 5*stride_out), _mm_srli_si128(o2, 8));
    _mm_storel_epi64((__m128i*)(output + 6*stride_out), o3);
    _mm_storel_epi64((__m128i*)(output + 7*stride_out), _mm_srli_si128(o3, 8));
}

static inline void transpose4x4_epi32(__m128i a0, __m128i a1, __m128i a2, __m128i a3, __m128i *b0, __m128i *b1, __m128i *b2, __m128i *b3)
{
    __m128i t0 = _mm_unpacklo_epi32(a0, a1);
    __m128i t1 = _mm_unpackhi_epi32(a0, a1);
    __m128i t2 = _mm_unpacklo_epi32(a2, a3);
    __m128i t3 = _mm_unpackhi_epi32(a2, a3);
    *b0 = _mm_unpacklo_epi64(t0, t2);
    *b1 = _mm_unpackhi_epi64(t0, t2);
    *b2 = _mm_unpacklo_epi64(t1, t3);
    *b3 = _mm_unpacklo_epi64(t1, t3);
}

static inline void deint3_transpose(__m128i x0, __m128i x1, __m128i x2, __m128i x3, __m128i* y1, __m128i* y2, __m128i *y3)
{
    const __m128i DEINTL3 = _mm_setr_epi8(0, 3, 6, 9, 1, 4, 7, 10, 2, 5, 8, 11, -1, -1, -1, -1);
    __m128i t0 = _mm_shuffle_epi8(x0, DEINTL3);
    __m128i t1 = _mm_shuffle_epi8(x1, DEINTL3);
    __m128i t2 = _mm_shuffle_epi8(x2, DEINTL3);
    __m128i t3 = _mm_shuffle_epi8(x3, DEINTL3);
    __m128i dummy;
    transpose4x4_epi32(t0, t1, t2, t3, y1, y2, y3, &dummy);
}

static inline void transpose16x10(uint8_t output[], const uint8_t input[])
{
    __m128i row[16];
    for (int i = 0; i < 16; i++) {
        row[i] = _mm_loadl_epi64((__m128i const*)(input + i * 10));
    }
    // AVX2: transpose two 8x8 blocks in parallel across 128-bit lanes.
    __m256i x0 = _mm256_set_m128i(row[8], row[0]);
    __m256i x1 = _mm256_set_m128i(row[9], row[1]);
    __m256i x2 = _mm256_set_m128i(row[10], row[2]);
    __m256i x3 = _mm256_set_m128i(row[11], row[3]);
    __m256i x4 = _mm256_set_m128i(row[12], row[4]);
    __m256i x5 = _mm256_set_m128i(row[13], row[5]);
    __m256i x6 = _mm256_set_m128i(row[14], row[6]);
    __m256i x7 = _mm256_set_m128i(row[15], row[7]);

    __m256i t0 = _mm256_unpacklo_epi8(x0, x1);
    __m256i t1 = _mm256_unpacklo_epi8(x2, x3);
    __m256i t2 = _mm256_unpacklo_epi8(x4, x5);
    __m256i t3 = _mm256_unpacklo_epi8(x6, x7);
    __m256i s0 = _mm256_unpacklo_epi16(t0, t1);
    __m256i s1 = _mm256_unpackhi_epi16(t0, t1);
    __m256i s2 = _mm256_unpacklo_epi16(t2, t3);
    __m256i s3 = _mm256_unpackhi_epi16(t2, t3);
    __m256i y0 = _mm256_unpacklo_epi32(s0, s2);
    __m256i y1 = _mm256_unpackhi_epi32(s0, s2);
    __m256i y2 = _mm256_unpacklo_epi32(s1, s3);
    __m256i y3 = _mm256_unpackhi_epi32(s1, s3);

    // Reorder qwords to [col0_top,col0_bot,col1_top,col1_bot] per ymm.
    _mm256_storeu_si256((__m256i *)(output + 0 * 16),
                        _mm256_permute4x64_epi64(y0, _MM_SHUFFLE(3, 1, 2, 0)));
    _mm256_storeu_si256((__m256i *)(output + 2 * 16),
                        _mm256_permute4x64_epi64(y1, _MM_SHUFFLE(3, 1, 2, 0)));
    _mm256_storeu_si256((__m256i *)(output + 4 * 16),
                        _mm256_permute4x64_epi64(y2, _MM_SHUFFLE(3, 1, 2, 0)));
    _mm256_storeu_si256((__m256i *)(output + 6 * 16),
                        _mm256_permute4x64_epi64(y3, _MM_SHUFFLE(3, 1, 2, 0)));

    const __m128i MASK_LO = _mm_set1_epi16(0x00FF);
    __m128i tail0 = _mm_setr_epi16(
        load_u16(input + 0 * 10 + 8), load_u16(input + 1 * 10 + 8),
        load_u16(input + 2 * 10 + 8), load_u16(input + 3 * 10 + 8),
        load_u16(input + 4 * 10 + 8), load_u16(input + 5 * 10 + 8),
        load_u16(input + 6 * 10 + 8), load_u16(input + 7 * 10 + 8));
    __m128i tail1 = _mm_setr_epi16(
        load_u16(input + 8 * 10 + 8), load_u16(input + 9 * 10 + 8),
        load_u16(input + 10 * 10 + 8), load_u16(input + 11 * 10 + 8),
        load_u16(input + 12 * 10 + 8), load_u16(input + 13 * 10 + 8),
        load_u16(input + 14 * 10 + 8), load_u16(input + 15 * 10 + 8));
    __m128i col8 = _mm_packus_epi16(_mm_and_si128(tail0, MASK_LO),
                                    _mm_and_si128(tail1, MASK_LO));
    __m128i col9 = _mm_packus_epi16(_mm_srli_epi16(tail0, 8),
                                    _mm_srli_epi16(tail1, 8));
    _mm256_storeu_si256((__m256i *)(output + 8 * 16), _mm256_set_m128i(col9, col8));
}

static inline __m256i ymm_sub7_if_greater_epi8(__m256i x)
{
    return _mm256_min_epu8(x, _mm256_sub_epi8(x, _mm256_set1_epi8(7)));
}

static inline __m256i ymm_sub31_if_greater_epi8(__m256i x)
{
    return _mm256_min_epu8(x, _mm256_sub_epi8(x, _mm256_set1_epi8(31)));
}

static inline __m256i ymm_sub127_if_greater_epi8(__m256i x)
{
    return _mm256_min_epu8(x, _mm256_sub_epi8(x, _mm256_set1_epi8(127)));
}

static inline __m256i ymm_reduce7_epi8(__m256i x)
{
    __m256i CONST_7 = _mm256_set1_epi8(7);
    __m256i CONST_248 = _mm256_set1_epi8(-8);
    return _mm256_add_epi8(_mm256_and_si256(x, CONST_7), _mm256_srli_epi16(_mm256_and_si256(x, CONST_248), 3));
}

static inline __m256i ymm_reduce31_epi8(__m256i x)
{
    __m256i CONST_31 = _mm256_set1_epi8(31);
    __m256i CONST_224 = _mm256_set1_epi8(-32);
    return _mm256_add_epi8(_mm256_and_si256(x, CONST_31), _mm256_srli_epi16(_mm256_and_si256(x, CONST_224), 5));
}

static inline __m256i ymm_mod7_epi8(__m256i x)
{
    x = ymm_reduce7_epi8(x);
    x = ymm_reduce7_epi8(x);
    return ymm_sub7_if_greater_epi8(x);
}

static inline __m256i ymm_mod31_epi8(__m256i x)
{
    x = ymm_reduce31_epi8(x);
    return ymm_sub31_if_greater_epi8(x);
}

static inline int ymm_reduce_add_epi32(__m256i a)
{
    __m256i a_hi = _mm256_permute2x128_si256(a, a, 1);
    a = _mm256_hadd_epi32(a, a_hi);
    a = _mm256_hadd_epi32(a, a);
    a = _mm256_hadd_epi32(a, a);
    return _mm256_extract_epi32(a, 0);
}

static inline __m256i ymm_reduce7_epi16(__m256i x)
{
    return _mm256_maddubs_epi16(x, _mm256_set1_epi16((4 << 8) + 1));
}

static inline __m256i ymm_reduce31_epi16(__m256i x)
{
    return _mm256_maddubs_epi16(x, _mm256_set1_epi16((8 << 8) + 1));
}

static inline __m256i ymm_reduce127_epi16(__m256i x)
{
    return _mm256_maddubs_epi16(x, _mm256_set1_epi16((2 << 8) + 1));
}

#endif // SIMD_H_
