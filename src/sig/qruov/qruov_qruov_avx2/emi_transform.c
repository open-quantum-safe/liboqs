#include <assert.h>
#include <stdlib.h>
#include "emi_transform.h"
#include "qruov_simd.h"

extern const int MAT_EVAL[];
extern const int MAT_INTERP[];
extern const int MAT_REEVAL[];

#if QRUOV_q == 31 && QRUOV_L == 3
void interpolate_q31_q31L3(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
void reevaluate_q31_q31L3(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
#elif QRUOV_q == 127 && QRUOV_L == 3
void interpolate_avx2_q127L3(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
void reevaluate_avx2_q127L3(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
#elif QRUOV_q == 31 && QRUOV_L == 10
void interpolate_q31_q31L10(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
void reevaluate_q31_q31L10(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
#elif QRUOV_q == 7 && QRUOV_L == 10
void evaluate_q7_q7L10(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
void interpolate_q7_q7L10(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
void reevaluate_q7_q7L10(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
#elif QRUOV_q == 127 && QRUOV_L == 10
void interpolate_avx2_q127L10(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
void reevaluate_avx2_q127L10(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
#else
#error "Unsupported (q, L) for AVX2 EMI transform"
#endif

#if QRUOV_L == 3 && (QRUOV_q == 31 || QRUOV_q == 127)
static void evaluate_specialized_L3(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size)
{
    const __m256i q = _mm256_set1_epi8(QRUOV_q);
    assert(size >= 32);

    for (int i = 0; i < size; i += 32) {
        const int offset = (i + 31) < size ? i : (size - 32);
        const __m256i a0 = _mm256_loadu_si256((const __m256i *)&input[0 * plane_in + offset]);
        const __m256i a1 = _mm256_loadu_si256((const __m256i *)&input[1 * plane_in + offset]);
        const __m256i a2 = _mm256_loadu_si256((const __m256i *)&input[2 * plane_in + offset]);
        const __m256i a0a2 = ymm_subq_if_greater_epi8(_mm256_add_epi8(a0, a2));
        const __m256i a2d = ymm_subq_if_greater_epi8(_mm256_add_epi8(a2, a2));
        const __m256i a1a2d = ymm_subq_if_greater_epi8(_mm256_add_epi8(a1, a2d));
        const __m256i a1da2q = ymm_subq_if_greater_epi8(_mm256_add_epi8(a1a2d, a1a2d));

        _mm256_storeu_si256((__m256i *)&output[0 * plane_out + offset], a0);
        _mm256_storeu_si256((__m256i *)&output[1 * plane_out + offset], ymm_subq_if_greater_epi8(_mm256_add_epi8(a1, a0a2)));
        _mm256_storeu_si256((__m256i *)&output[2 * plane_out + offset],
                            ymm_subq_if_greater_epi8(_mm256_add_epi8(_mm256_xor_si256(a1, q), a0a2)));
        _mm256_storeu_si256((__m256i *)&output[3 * plane_out + offset], ymm_subq_if_greater_epi8(_mm256_add_epi8(a0, a1da2q)));
        _mm256_storeu_si256((__m256i *)&output[4 * plane_out + offset], a2);
    }
}
#endif

#if QRUOV_q == 31 && QRUOV_L == 10
static void evaluate_specialized_q31L10(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size)
{
    assert(size >= 32);

    for (int i = 0; i < size; i += 16) {
        const int offset = (i + 15) < size ? i : (size - 16);
        __m256i b09;
        __m256i b24;
        __m256i b68;
        __m256i b13;
        __m256i b57;
        __m128i a9_saved;

        {
            const __m128i a0 = _mm_loadu_si128((const __m128i *)&input[0 * plane_in + offset]);
            const __m128i a1 = _mm_loadu_si128((const __m128i *)&input[1 * plane_in + offset]);
            const __m128i a2 = _mm_loadu_si128((const __m128i *)&input[2 * plane_in + offset]);
            const __m128i a3 = _mm_loadu_si128((const __m128i *)&input[3 * plane_in + offset]);
            const __m128i a4 = _mm_loadu_si128((const __m128i *)&input[4 * plane_in + offset]);
            const __m128i a5 = _mm_loadu_si128((const __m128i *)&input[5 * plane_in + offset]);
            const __m128i a6 = _mm_loadu_si128((const __m128i *)&input[6 * plane_in + offset]);
            const __m128i a7 = _mm_loadu_si128((const __m128i *)&input[7 * plane_in + offset]);
            const __m128i a8 = _mm_loadu_si128((const __m128i *)&input[8 * plane_in + offset]);
            const __m128i a9 = _mm_loadu_si128((const __m128i *)&input[9 * plane_in + offset]);

            a9_saved = a9;
            _mm_storeu_si128((__m128i *)&output[offset], a0);
            b09 = _mm256_setr_m128i(_mm_unpacklo_epi8(a0, a9), _mm_unpackhi_epi8(a0, a9));
            b24 = _mm256_setr_m128i(_mm_unpacklo_epi8(a2, a4), _mm_unpackhi_epi8(a2, a4));
            b68 = _mm256_setr_m128i(_mm_unpacklo_epi8(a6, a8), _mm_unpackhi_epi8(a6, a8));
            b13 = _mm256_setr_m128i(_mm_unpacklo_epi8(a1, a3), _mm_unpackhi_epi8(a1, a3));
            b57 = _mm256_setr_m128i(_mm_unpacklo_epi8(a5, a7), _mm_unpackhi_epi8(a5, a7));
        }

#define OUTPUT_PAIR(x, idx_pos, idx_neg) do { \
            const __m256i even = _mm256_add_epi16(_mm256_maddubs_epi16(b24, _mm256_set1_epi16(QRUOV_POW2(x) + (QRUOV_POW4(x) << 8))), \
                                                  _mm256_maddubs_epi16(b68, _mm256_set1_epi16(QRUOV_POW6(x) + (QRUOV_POW8(x) << 8)))); \
            const __m256i odd = _mm256_add_epi16(_mm256_maddubs_epi16(b13, _mm256_set1_epi16(QRUOV_POW1(x) + (QRUOV_POW3(x) << 8))), \
                                                 _mm256_maddubs_epi16(b57, _mm256_set1_epi16(QRUOV_POW5(x) + (QRUOV_POW7(x) << 8)))); \
            __m256i fx = _mm256_add_epi16(_mm256_maddubs_epi16(b09, _mm256_set1_epi16(QRUOV_POW0(x) + (QRUOV_POW9(x) << 8))), even); \
            __m256i fmx = _mm256_add_epi16(_mm256_maddubs_epi16(b09, _mm256_set1_epi16(QRUOV_POW0(-(x)) + (QRUOV_POW9(-(x)) << 8))), even); \
            fx = _mm256_add_epi16(fx, odd); \
            fmx = _mm256_add_epi16(fmx, _mm256_sub_epi16(_mm256_set1_epi16(31 * 31 * 16), odd)); \
            _mm_storeu_si128((__m128i *)&output[(idx_pos) * plane_out + offset], ymm_cvtusepi16_epi8(ymm_modq_epi15(fx))); \
            _mm_storeu_si128((__m128i *)&output[(idx_neg) * plane_out + offset], ymm_cvtusepi16_epi8(ymm_modq_epi15(fmx))); \
        } while (0)

        OUTPUT_PAIR(1, 1, 2);
        OUTPUT_PAIR(2, 3, 4);
        OUTPUT_PAIR(3, 5, 6);
        OUTPUT_PAIR(4, 7, 8);
        OUTPUT_PAIR(5, 9, 10);
        OUTPUT_PAIR(6, 11, 12);
        OUTPUT_PAIR(7, 13, 14);
        OUTPUT_PAIR(8, 15, 16);
        {
            __m256i fx = _mm256_setzero_si256();

            fx = _mm256_add_epi16(fx, _mm256_maddubs_epi16(b09, _mm256_set1_epi16(QRUOV_POW0(9) + (QRUOV_POW9(9) << 8))));
            fx = _mm256_add_epi16(fx, _mm256_maddubs_epi16(b24, _mm256_set1_epi16(QRUOV_POW2(9) + (QRUOV_POW4(9) << 8))));
            fx = _mm256_add_epi16(fx, _mm256_maddubs_epi16(b68, _mm256_set1_epi16(QRUOV_POW6(9) + (QRUOV_POW8(9) << 8))));
            fx = _mm256_add_epi16(fx, _mm256_maddubs_epi16(b13, _mm256_set1_epi16(QRUOV_POW1(9) + (QRUOV_POW3(9) << 8))));
            fx = _mm256_add_epi16(fx, _mm256_maddubs_epi16(b57, _mm256_set1_epi16(QRUOV_POW5(9) + (QRUOV_POW7(9) << 8))));
            _mm_storeu_si128((__m128i *)&output[17 * plane_out + offset], ymm_cvtusepi16_epi8(ymm_modq_epi15(fx)));
        }
        _mm_storeu_si128((__m128i *)&output[18 * plane_out + offset], a9_saved);
#undef OUTPUT_PAIR
    }
}
#endif

#if QRUOV_q == 127 && QRUOV_L == 10
static void evaluate_specialized_q127L10(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size)
{
    assert(size >= 32);

    for (int i = 0; i < size; i += 16) {
        const int offset = (i + 15) < size ? i : (size - 16);
        __m256i b09;
        __m256i b24;
        __m256i b68;
        __m256i b13;
        __m256i b57;
        __m256i signed_lift;
        __m128i a9_saved;

        {
            const __m128i a0 = _mm_loadu_si128((const __m128i *)&input[0 * plane_in + offset]);
            const __m128i a1 = _mm_loadu_si128((const __m128i *)&input[1 * plane_in + offset]);
            const __m128i a2 = _mm_loadu_si128((const __m128i *)&input[2 * plane_in + offset]);
            const __m128i a3 = _mm_loadu_si128((const __m128i *)&input[3 * plane_in + offset]);
            const __m128i a4 = _mm_loadu_si128((const __m128i *)&input[4 * plane_in + offset]);
            const __m128i a5 = _mm_loadu_si128((const __m128i *)&input[5 * plane_in + offset]);
            const __m128i a6 = _mm_loadu_si128((const __m128i *)&input[6 * plane_in + offset]);
            const __m128i a7 = _mm_loadu_si128((const __m128i *)&input[7 * plane_in + offset]);
            const __m128i a8 = _mm_loadu_si128((const __m128i *)&input[8 * plane_in + offset]);
            const __m128i a9 = _mm_loadu_si128((const __m128i *)&input[9 * plane_in + offset]);

            a9_saved = a9;
            _mm_storeu_si128((__m128i *)&output[0 * plane_out + offset], a0);
            b09 = _mm256_setr_m128i(_mm_unpacklo_epi8(a0, a9), _mm_unpackhi_epi8(a0, a9));
            b24 = _mm256_setr_m128i(_mm_unpacklo_epi8(a2, a4), _mm_unpackhi_epi8(a2, a4));
            b68 = _mm256_setr_m128i(_mm_unpacklo_epi8(a6, a8), _mm_unpackhi_epi8(a6, a8));
            b13 = _mm256_setr_m128i(_mm_unpacklo_epi8(a1, a3), _mm_unpackhi_epi8(a1, a3));
            b57 = _mm256_setr_m128i(_mm_unpacklo_epi8(a5, a7), _mm_unpackhi_epi8(a5, a7));
            signed_lift = _mm256_set1_epi16(127 * 258);
        }
        const __m256i zero = _mm256_setzero_si256();

#define Q127_SIGNED(c) ((c) > 63 ? (c) - 127 : (c))
#define Q127_PACK(c0, c1) \
        ((int16_t)((uint16_t)(uint8_t)(c0) | ((uint16_t)(uint8_t)(c1) << 8)))
#define Q127_MADD(pair, c0, c1) \
        _mm256_maddubs_epi16((pair), _mm256_set1_epi16(Q127_PACK((c0), (c1))))
#define Q127_SIGNED_MADD(pair, c0, c1) \
        Q127_MADD((pair), Q127_SIGNED(c0), Q127_SIGNED(c1))
#define Q127_LIFT_NEG(x) \
        _mm256_add_epi16((x), _mm256_and_si256(_mm256_cmpgt_epi16(zero, (x)), signed_lift))
#define OUTPUT_SIGNED_PAIR(x, idx_pos, idx_neg) do { \
            const __m256i even = _mm256_add_epi16(Q127_SIGNED_MADD(b24, QRUOV_POW2(x), QRUOV_POW4(x)), \
                                                  Q127_SIGNED_MADD(b68, QRUOV_POW6(x), QRUOV_POW8(x))); \
            const __m256i odd = _mm256_add_epi16(Q127_SIGNED_MADD(b13, QRUOV_POW1(x), QRUOV_POW3(x)), \
                                                 Q127_SIGNED_MADD(b57, QRUOV_POW5(x), QRUOV_POW7(x))); \
            __m256i fx = _mm256_add_epi16(Q127_SIGNED_MADD(b09, QRUOV_POW0(x), QRUOV_POW9(x)), even); \
            __m256i fmx = _mm256_add_epi16(Q127_SIGNED_MADD(b09, QRUOV_POW0(-(x)), QRUOV_POW9(-(x))), even); \
            fx = _mm256_add_epi16(fx, odd); \
            fmx = _mm256_sub_epi16(fmx, odd); \
            _mm_storeu_si128((__m128i *)&output[(idx_pos) * plane_out + offset], \
                             ymm_cvtusepi16_epi8(ymm_mod127_epi15(Q127_LIFT_NEG(fx)))); \
            _mm_storeu_si128((__m128i *)&output[(idx_neg) * plane_out + offset], \
                             ymm_cvtusepi16_epi8(ymm_mod127_epi15(Q127_LIFT_NEG(fmx)))); \
        } while (0)

        OUTPUT_SIGNED_PAIR(1, 1, 2);
        OUTPUT_SIGNED_PAIR(2, 3, 4);
        OUTPUT_SIGNED_PAIR(3, 5, 6);
        OUTPUT_SIGNED_PAIR(4, 7, 8);
        OUTPUT_SIGNED_PAIR(5, 9, 10);
        OUTPUT_SIGNED_PAIR(6, 11, 12);
        OUTPUT_SIGNED_PAIR(7, 13, 14);
        OUTPUT_SIGNED_PAIR(8, 15, 16);
        {
            __m256i fx = Q127_SIGNED_MADD(b09, QRUOV_POW0(9), QRUOV_POW9(9));
            fx = _mm256_add_epi16(fx, Q127_SIGNED_MADD(b24, QRUOV_POW2(9), QRUOV_POW4(9)));
            fx = _mm256_add_epi16(fx, Q127_SIGNED_MADD(b68, QRUOV_POW6(9), QRUOV_POW8(9)));
            fx = _mm256_add_epi16(fx, Q127_SIGNED_MADD(b13, QRUOV_POW1(9), QRUOV_POW3(9)));
            fx = _mm256_add_epi16(fx, Q127_SIGNED_MADD(b57, QRUOV_POW5(9), QRUOV_POW7(9)));
            _mm_storeu_si128((__m128i *)&output[17 * plane_out + offset],
                             ymm_cvtusepi16_epi8(ymm_mod127_epi15(Q127_LIFT_NEG(fx))));
        }
        _mm_storeu_si128((__m128i *)&output[18 * plane_out + offset], a9_saved);

#undef OUTPUT_SIGNED_PAIR
#undef Q127_LIFT_NEG
#undef Q127_SIGNED_MADD
#undef Q127_MADD
#undef Q127_PACK
#undef Q127_SIGNED
    }
}
#endif

void evaluate(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size)
{
    if (size < 32) abort();
#if QRUOV_q == 7 && QRUOV_L == 10
    evaluate_q7_q7L10(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 31 && QRUOV_L == 3
    evaluate_specialized_L3(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 127 && QRUOV_L == 3
    evaluate_specialized_L3(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 31 && QRUOV_L == 10
    evaluate_specialized_q31L10(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 127 && QRUOV_L == 10
    evaluate_specialized_q127L10(output, plane_out, input, plane_in, size);
#else
#error "Unsupported (q, L) for AVX2 EMI evaluate"
#endif
}

void reevaluate(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size)
{
    if (size < 32) abort();
#if QRUOV_q == 7 && QRUOV_L == 10
    reevaluate_q7_q7L10(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 31 && QRUOV_L == 3
    reevaluate_q31_q31L3(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 127 && QRUOV_L == 3
    reevaluate_avx2_q127L3(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 31 && QRUOV_L == 10
    reevaluate_q31_q31L10(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 127 && QRUOV_L == 10
    reevaluate_avx2_q127L10(output, plane_out, input, plane_in, size);
#else
#error "Unsupported (q, L) for AVX2 EMI reevaluate"
#endif
}

void interpolate(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size)
{
    if (size < 32) abort();
#if QRUOV_q == 7 && QRUOV_L == 10
    interpolate_q7_q7L10(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 31 && QRUOV_L == 3
    interpolate_q31_q31L3(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 127 && QRUOV_L == 3
    interpolate_avx2_q127L3(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 31 && QRUOV_L == 10
    interpolate_q31_q31L10(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 127 && QRUOV_L == 10
    interpolate_avx2_q127L10(output, plane_out, input, plane_in, size);
#else
#error "Unsupported (q, L) for AVX2 EMI interpolate"
#endif
}
