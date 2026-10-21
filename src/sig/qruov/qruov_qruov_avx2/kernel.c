#include <assert.h>
#include <string.h>

#include "kernel.h"
#include "qruov_simd.h"

static inline __m256i load_row_vnni_8(const uint8_t *ptr)
{
    return _mm256_loadu_si256((const __m256i *)ptr);
}

static inline __m256i load_row_vnni_7(const uint8_t *ptr)
{
    __m128i lo = _mm_loadu_si128((const __m128i *)ptr);
    __m128i hi = _mm_loadu_si128((const __m128i *)(ptr + 12));
    return _mm256_setr_m128i(lo, _mm_srli_si128(hi, 4));
}

static inline __m256i load_row_vnni_6(const uint8_t *ptr)
{
    __m128i lo = _mm_loadu_si128((const __m128i *)ptr);
    __m128i hi = _mm_loadu_si128((const __m128i *)(ptr + 8));
    return _mm256_setr_m128i(lo, _mm_srli_si128(hi, 8));
}

static inline __m256i load_row_vnni_4x2(const uint8_t *ptr0, const uint8_t *ptr1)
{
    const __m256i idx = _mm256_set_epi32(7, 3, 6, 2, 5, 1, 4, 0);
    __m128i lo = _mm_loadu_si128((const __m128i *)ptr0);
    __m128i hi = _mm_loadu_si128((const __m128i *)ptr1);
    return _mm256_permutevar8x32_epi32(
            _mm256_set_m128i(hi, lo), idx);
}

static inline __m256i load_row_vnni_4x1(const uint8_t *ptr)
{
    const __m256i idx = _mm256_set_epi32(7, 3, 6, 2, 5, 1, 4, 0);
    __m128i lo = _mm_loadu_si128((const __m128i *)ptr);
    return _mm256_permutevar8x32_epi32(
            _mm256_set_m128i(_mm_setzero_si128(), lo), idx);
}

static inline void load_row_vnni_16(__m256i *lo, __m256i *hi, const uint8_t *ptr)
{
    *lo = _mm256_loadu_si256((const __m256i *)ptr);
    *hi = _mm256_loadu_si256((const __m256i *)(ptr + 32));
}

static inline void store_row_8_epi32(uint8_t *out, __m256i acc)
{
    _mm_storel_epi64((__m128i *)out, ymm_modq_epi32_epi8(acc));
}

static inline void store_row_7_epi32(uint8_t *out, __m256i acc)
{
    __m128i tmp = ymm_modq_epi32_epi8(acc);
    memcpy(out, &tmp, 7);
}

static inline void store_row_6_epi32(uint8_t *out, __m256i acc)
{
    __m128i tmp = ymm_modq_epi32_epi8(acc);
    memcpy(out, &tmp, 6);
}

static inline void store_row_4_pair_epi32(uint8_t *out, __m256i acc)
{
    __m256i sum = _mm256_hadd_epi32(acc, _mm256_setzero_si256());
    sum = _mm256_permute4x64_epi64(sum, _MM_SHUFFLE(3, 1, 2, 0));
    __m128i tmp = ymm_modq_epi32_epi8(sum);
    store_u32(out, (uint32_t)_mm_cvtsi128_si32(tmp));
}

#if QRUOV_q != 7
static inline void store_row_16_epi32(uint8_t *out, __m256i lo, __m256i hi)
{
    __m128i tmp = ymm2_modq_epi32_epi8(lo, hi);
    memcpy(out, &tmp, 16);
}
#endif

#if QRUOV_q == 7
static inline void store_row_8_hadd_epi16(uint8_t *out, __m256i acc)
{
    __m256i sum = _mm256_hadd_epi16(acc, _mm256_setzero_si256());
    sum = ymm_mod7_epi15(sum);
    __m128i sum16 = _mm_unpacklo_epi64(_mm256_castsi256_si128(sum),
                                       _mm256_extracti128_si256(sum, 1));
    __m128i tmp = _mm_packus_epi16(sum16, _mm_setzero_si128());
    _mm_storel_epi64((__m128i *)out, tmp);
}

static inline void store_row_16_hadd_epi16(uint8_t *out, __m256i lo, __m256i hi)
{
    __m256i sum = _mm256_hadd_epi16(lo, hi);
    sum = ymm_mod7_epi15(sum);
    sum = _mm256_permute4x64_epi64(sum, _MM_SHUFFLE(3, 1, 2, 0));
    __m128i tmp = _mm_packus_epi16(_mm256_castsi256_si128(sum),
                                   _mm256_extracti128_si256(sum, 1));
    memcpy(out, &tmp, 16);
}
#endif

#define ADD1(dst, src)                 \
    do {                               \
        dst = _mm256_add_epi32(dst, src); \
    } while (0)

#define ADD2(dst, src)                 \
    do {                               \
        dst = _mm256_add_epi32(dst, src); \
        dst = _mm256_add_epi32(dst, src); \
    } while (0)

#define ADD5(dst, src)                 \
    do {                               \
        dst = _mm256_add_epi32(dst, src); \
        dst = _mm256_add_epi32(dst, src); \
        dst = _mm256_add_epi32(dst, src); \
        dst = _mm256_add_epi32(dst, src); \
        dst = _mm256_add_epi32(dst, src); \
    } while (0)

#define ADD25(dst, src)                            \
    do {                                           \
        __m256i src_x5 = _mm256_add_epi32(src, _mm256_slli_epi32(src, 2)); \
        dst = _mm256_add_epi32(dst, _mm256_add_epi32(src_x5, _mm256_slli_epi32(src_x5, 2))); \
    } while (0)

#if QRUOV_L == 3
static void matmulfql3_symmetric_upper_impl(uint8_t *output_soa, const uint8_t *vector_soa, int size)
{
    assert(size % 16 == 0);
    for (int i = 0; i < size; i++) {
        __m256i a0 = _mm256_set1_epi16(vector_soa[0 * size + i]);
        __m256i a1 = _mm256_set1_epi16(vector_soa[1 * size + i]);
        __m256i a2 = _mm256_set1_epi16(vector_soa[2 * size + i]);
        for (int j = i & ~15; j < size; j += 16) {
            __m256i b0 = _mm256_cvtepi8_epi16(
                    _mm_loadu_si128((const __m128i *)&vector_soa[0 * size + j]));
            __m256i b1 = _mm256_cvtepi8_epi16(
                    _mm_loadu_si128((const __m128i *)&vector_soa[1 * size + j]));
            __m256i b2 = _mm256_cvtepi8_epi16(
                    _mm_loadu_si128((const __m128i *)&vector_soa[2 * size + j]));
            __m256i c0 = _mm256_setzero_si256();
            __m256i c1 = _mm256_setzero_si256();
            __m256i c2 = _mm256_setzero_si256();
            __m256i c3 = _mm256_setzero_si256();
            __m256i c4 = _mm256_setzero_si256();

            c0 = _mm256_add_epi16(c0, _mm256_mullo_epi16(a0, b0));
            c1 = _mm256_add_epi16(c1, _mm256_mullo_epi16(a0, b1));
            c2 = _mm256_add_epi16(c2, _mm256_mullo_epi16(a0, b2));
            c1 = _mm256_add_epi16(c1, _mm256_mullo_epi16(a1, b0));
            c2 = _mm256_add_epi16(c2, _mm256_mullo_epi16(a1, b1));
            c3 = _mm256_add_epi16(c3, _mm256_mullo_epi16(a1, b2));
            c2 = _mm256_add_epi16(c2, _mm256_mullo_epi16(a2, b0));
            c3 = _mm256_add_epi16(c3, _mm256_mullo_epi16(a2, b1));
            c4 = _mm256_add_epi16(c4, _mm256_mullo_epi16(a2, b2));

#if QRUOV_q == 127
            c2 = ymm_reduceq_epi16(c2);
            c3 = ymm_reduceq_epi16(c3);
            c4 = ymm_reduceq_epi16(c4);
#endif
            c0 = _mm256_add_epi16(c0, c3);
            c1 = _mm256_add_epi16(c1, c3);
            c1 = _mm256_add_epi16(c1, c4);
            c2 = _mm256_add_epi16(c2, c4);
            _mm_storeu_si128((__m128i *)&output_soa[0 * size * size + i * size + j],
                             ymm_cvtusepi16_epi8(ymm_modq_epi15(c0)));
            _mm_storeu_si128((__m128i *)&output_soa[1 * size * size + i * size + j],
                             ymm_cvtusepi16_epi8(ymm_modq_epi15(c1)));
            _mm_storeu_si128((__m128i *)&output_soa[2 * size * size + i * size + j],
                             ymm_cvtusepi16_epi8(ymm_modq_epi15(c2)));
        }
    }
}
#elif QRUOV_L == 10
static void matmulfql10_symmetric_upper_impl(uint8_t *output_soa, const uint8_t *vector_soa, int size)
{
    assert(size % 16 == 0);
    for (int i = 0; i < size; i++) {
        __m256i a[QRUOV_L];
        for (int l = 0; l < QRUOV_L; l++) {
            a[l] = _mm256_set1_epi16(vector_soa[l * size + i]);
        }
        for (int j = i & ~15; j < size; j += 16) {
            __m256i b[QRUOV_L];
            __m256i c[2 * QRUOV_L - 1];
            for (int l = 0; l < QRUOV_L; l++) {
                b[l] = _mm256_cvtepi8_epi16(_mm_loadu_si128((const __m128i *)&vector_soa[l * size + j]));
            }
            for (int l = 0; l < 2 * QRUOV_L - 1; l++) {
                c[l] = _mm256_setzero_si256();
            }
            for (int l = 0; l < QRUOV_L; l++) {
                for (int m = 0; m < QRUOV_L; m++) {
#if QRUOV_q == 127
                    const __m256i prod = _mm256_mullo_epi16(a[l], b[m]);
                    c[l + m] = ymm_modq_epi15(_mm256_add_epi16(c[l + m], prod));
#else
                    c[l + m] = _mm256_add_epi16(c[l + m], _mm256_mullo_epi16(a[l], b[m]));
#endif
                }
            }
#if QRUOV_q == 127 || QRUOV_q == 31
            for (int l = 2 * QRUOV_L - 2; l >= QRUOV_L - 1; l--) {
                c[l] = ymm_reduceq_epi16(c[l]);
            }
#endif
            for (int t = 2 * QRUOV_L - 2; t >= QRUOV_L; t--) {
                for (int n = 0; n < QRUOV_fc0; n++) {
                    c[t - QRUOV_L] = _mm256_add_epi16(c[t - QRUOV_L], c[t]);
                }
                for (int n = 0; n < QRUOV_fc; n++) {
                    c[t - QRUOV_L + QRUOV_fe] = _mm256_add_epi16(c[t - QRUOV_L + QRUOV_fe], c[t]);
                }
            }
            for (int l = 0; l < QRUOV_L; l++) {
                _mm_storeu_si128((__m128i *)&output_soa[l * size * size + i * size + j],
                                 ymm_cvtusepi16_epi8(ymm_modq_epi15(c[l])));
            }
        }
    }
}
#else
#error "Unsupported QRUOV_L"
#endif

void matmulfql_symmetric_upper(uint8_t *output_soa, const uint8_t *vector_soa, int size)
{
#if QRUOV_L == 3
    matmulfql3_symmetric_upper_impl(output_soa, vector_soa, size);
#elif QRUOV_L == 10
    matmulfql10_symmetric_upper_impl(output_soa, vector_soa, size);
#else
#error "Unsupported QRUOV_L"
#endif
}

#if QRUOV_L == 3

static void matmulfql3_vnni_1x8_impl(uint8_t *out, const uint8_t *A, const uint8_t *Bvnni, int K,
        int incout __attribute__((unused)), int incA __attribute__((unused)), int incB,
        int incLout, int incLA, int incLB)
{
    assert(K % 4 == 0);
    __m256i c0 = _mm256_setzero_si256();
    __m256i c1 = _mm256_setzero_si256();
    __m256i c2 = _mm256_setzero_si256();
    __m256i c3 = _mm256_setzero_si256();
    __m256i c4 = _mm256_setzero_si256();

    for (int k = 0; k < K; k += 4) {
        __m256i b0 = load_row_vnni_8(&Bvnni[k * incB + 0 * incLB]);
        __m256i b1 = load_row_vnni_8(&Bvnni[k * incB + 1 * incLB]);
        __m256i b2 = load_row_vnni_8(&Bvnni[k * incB + 2 * incLB]);
        __m256i a0 = _mm256_set1_epi32(load_u32(&A[k + 0 * incLA]));
        __m256i a1 = _mm256_set1_epi32(load_u32(&A[k + 1 * incLA]));
        __m256i a2 = _mm256_set1_epi32(load_u32(&A[k + 2 * incLA]));
        c0 = ymm_dot_u8s8(c0, a0, b0);
        c1 = ymm_dot_u8s8(c1, a0, b1);
        c1 = ymm_dot_u8s8(c1, a1, b0);
        c2 = ymm_dot_u8s8(c2, a0, b2);
        c2 = ymm_dot_u8s8(c2, a1, b1);
        c2 = ymm_dot_u8s8(c2, a2, b0);
        c3 = ymm_dot_u8s8(c3, a1, b2);
        c3 = ymm_dot_u8s8(c3, a2, b1);
        c4 = ymm_dot_u8s8(c4, a2, b2);
    }

    c0 = _mm256_add_epi32(c0, c3);
    c1 = _mm256_add_epi32(c1, c3);
    c1 = _mm256_add_epi32(c1, c4);
    c2 = _mm256_add_epi32(c2, c4);
    store_row_8_epi32(&out[0 * incLout], c0);
    store_row_8_epi32(&out[1 * incLout], c1);
    store_row_8_epi32(&out[2 * incLout], c2);
}

static void matmulfql3_vnni_1x7_impl(uint8_t *out, const uint8_t *A, const uint8_t *Bvnni, int K,
        int incout __attribute__((unused)), int incA __attribute__((unused)), int incB,
        int incLout, int incLA, int incLB)
{
    assert(K % 4 == 0);
    __m256i c0 = _mm256_setzero_si256();
    __m256i c1 = _mm256_setzero_si256();
    __m256i c2 = _mm256_setzero_si256();
    __m256i c3 = _mm256_setzero_si256();
    __m256i c4 = _mm256_setzero_si256();

    for (int k = 0; k < K; k += 4) {
        __m256i b0 = load_row_vnni_7(&Bvnni[k * incB + 0 * incLB]);
        __m256i b1 = load_row_vnni_7(&Bvnni[k * incB + 1 * incLB]);
        __m256i b2 = load_row_vnni_7(&Bvnni[k * incB + 2 * incLB]);
        __m256i a0 = _mm256_set1_epi32(load_u32(&A[k + 0 * incLA]));
        __m256i a1 = _mm256_set1_epi32(load_u32(&A[k + 1 * incLA]));
        __m256i a2 = _mm256_set1_epi32(load_u32(&A[k + 2 * incLA]));
        c0 = ymm_dot_u8s8(c0, a0, b0);
        c1 = ymm_dot_u8s8(c1, a0, b1);
        c1 = ymm_dot_u8s8(c1, a1, b0);
        c2 = ymm_dot_u8s8(c2, a0, b2);
        c2 = ymm_dot_u8s8(c2, a1, b1);
        c2 = ymm_dot_u8s8(c2, a2, b0);
        c3 = ymm_dot_u8s8(c3, a1, b2);
        c3 = ymm_dot_u8s8(c3, a2, b1);
        c4 = ymm_dot_u8s8(c4, a2, b2);
    }

    c0 = _mm256_add_epi32(c0, c3);
    c1 = _mm256_add_epi32(c1, c3);
    c1 = _mm256_add_epi32(c1, c4);
    c2 = _mm256_add_epi32(c2, c4);
    store_row_7_epi32(&out[0 * incLout], c0);
    store_row_7_epi32(&out[1 * incLout], c1);
    store_row_7_epi32(&out[2 * incLout], c2);
}

static void matmulfql3_vnni_1x6_impl(uint8_t *out, const uint8_t *A, const uint8_t *Bvnni, int K,
        int incout __attribute__((unused)), int incA __attribute__((unused)), int incB,
        int incLout, int incLA, int incLB)
{
    assert(K % 4 == 0);
    __m256i c0 = _mm256_setzero_si256();
    __m256i c1 = _mm256_setzero_si256();
    __m256i c2 = _mm256_setzero_si256();
    __m256i c3 = _mm256_setzero_si256();
    __m256i c4 = _mm256_setzero_si256();

    for (int k = 0; k < K; k += 4) {
        __m256i b0 = load_row_vnni_6(&Bvnni[k * incB + 0 * incLB]);
        __m256i b1 = load_row_vnni_6(&Bvnni[k * incB + 1 * incLB]);
        __m256i b2 = load_row_vnni_6(&Bvnni[k * incB + 2 * incLB]);
        __m256i a0 = _mm256_set1_epi32(load_u32(&A[k + 0 * incLA]));
        __m256i a1 = _mm256_set1_epi32(load_u32(&A[k + 1 * incLA]));
        __m256i a2 = _mm256_set1_epi32(load_u32(&A[k + 2 * incLA]));
        c0 = ymm_dot_u8s8(c0, a0, b0);
        c1 = ymm_dot_u8s8(c1, a0, b1);
        c1 = ymm_dot_u8s8(c1, a1, b0);
        c2 = ymm_dot_u8s8(c2, a0, b2);
        c2 = ymm_dot_u8s8(c2, a1, b1);
        c2 = ymm_dot_u8s8(c2, a2, b0);
        c3 = ymm_dot_u8s8(c3, a1, b2);
        c3 = ymm_dot_u8s8(c3, a2, b1);
        c4 = ymm_dot_u8s8(c4, a2, b2);
    }

    c0 = _mm256_add_epi32(c0, c3);
    c1 = _mm256_add_epi32(c1, c3);
    c1 = _mm256_add_epi32(c1, c4);
    c2 = _mm256_add_epi32(c2, c4);
    store_row_6_epi32(&out[0 * incLout], c0);
    store_row_6_epi32(&out[1 * incLout], c1);
    store_row_6_epi32(&out[2 * incLout], c2);
}

static void matmulfql3_vnni_1x4_impl(uint8_t *out, const uint8_t *A, const uint8_t *Bvnni, int K,
        int incout __attribute__((unused)), int incA __attribute__((unused)), int incB,
        int incLout, int incLA, int incLB)
{
    assert(K % 4 == 0);
    __m256i c0 = _mm256_setzero_si256();
    __m256i c1 = _mm256_setzero_si256();
    __m256i c2 = _mm256_setzero_si256();
    __m256i c3 = _mm256_setzero_si256();
    __m256i c4 = _mm256_setzero_si256();

    int k = 0;
    for (; k + 8 <= K; k += 8) {
        __m256i b0 = load_row_vnni_4x2(&Bvnni[(k + 0) * incB + 0 * incLB],
                                       &Bvnni[(k + 4) * incB + 0 * incLB]);
        __m256i b1 = load_row_vnni_4x2(&Bvnni[(k + 0) * incB + 1 * incLB],
                                       &Bvnni[(k + 4) * incB + 1 * incLB]);
        __m256i b2 = load_row_vnni_4x2(&Bvnni[(k + 0) * incB + 2 * incLB],
                                       &Bvnni[(k + 4) * incB + 2 * incLB]);
        __m256i a0 = _mm256_set1_epi64x((long long)load_u64(&A[k + 0 * incLA]));
        __m256i a1 = _mm256_set1_epi64x((long long)load_u64(&A[k + 1 * incLA]));
        __m256i a2 = _mm256_set1_epi64x((long long)load_u64(&A[k + 2 * incLA]));
        c0 = ymm_dot_u8s8(c0, a0, b0);
        c1 = ymm_dot_u8s8(c1, a0, b1);
        c1 = ymm_dot_u8s8(c1, a1, b0);
        c2 = ymm_dot_u8s8(c2, a0, b2);
        c2 = ymm_dot_u8s8(c2, a1, b1);
        c2 = ymm_dot_u8s8(c2, a2, b0);
        c3 = ymm_dot_u8s8(c3, a1, b2);
        c3 = ymm_dot_u8s8(c3, a2, b1);
        c4 = ymm_dot_u8s8(c4, a2, b2);
    }
    for (; k < K; k += 4) {
        __m256i b0 = load_row_vnni_4x1(&Bvnni[k * incB + 0 * incLB]);
        __m256i b1 = load_row_vnni_4x1(&Bvnni[k * incB + 1 * incLB]);
        __m256i b2 = load_row_vnni_4x1(&Bvnni[k * incB + 2 * incLB]);
        __m256i a0 = _mm256_set1_epi64x((long long)load_u32(&A[k + 0 * incLA]));
        __m256i a1 = _mm256_set1_epi64x((long long)load_u32(&A[k + 1 * incLA]));
        __m256i a2 = _mm256_set1_epi64x((long long)load_u32(&A[k + 2 * incLA]));
        c0 = ymm_dot_u8s8(c0, a0, b0);
        c1 = ymm_dot_u8s8(c1, a0, b1);
        c1 = ymm_dot_u8s8(c1, a1, b0);
        c2 = ymm_dot_u8s8(c2, a0, b2);
        c2 = ymm_dot_u8s8(c2, a1, b1);
        c2 = ymm_dot_u8s8(c2, a2, b0);
        c3 = ymm_dot_u8s8(c3, a1, b2);
        c3 = ymm_dot_u8s8(c3, a2, b1);
        c4 = ymm_dot_u8s8(c4, a2, b2);
    }

    c0 = _mm256_add_epi32(c0, c3);
    c1 = _mm256_add_epi32(c1, c3);
    c1 = _mm256_add_epi32(c1, c4);
    c2 = _mm256_add_epi32(c2, c4);
    store_row_4_pair_epi32(&out[0 * incLout], c0);
    store_row_4_pair_epi32(&out[1 * incLout], c1);
    store_row_4_pair_epi32(&out[2 * incLout], c2);
}

#elif QRUOV_L == 10

#define DOT_ACC(dst, a, b) dst = ymm_dot_u8s8(dst, a, b)

#define MATMULFQL10_DOT_BODY                                               \
    DOT_ACC(c0, a0, b0);                                                   \
    DOT_ACC(c1, a0, b1); DOT_ACC(c1, a1, b0);                              \
    DOT_ACC(c2, a0, b2); DOT_ACC(c2, a1, b1); DOT_ACC(c2, a2, b0);        \
    DOT_ACC(c3, a0, b3); DOT_ACC(c3, a1, b2); DOT_ACC(c3, a2, b1); DOT_ACC(c3, a3, b0); \
    DOT_ACC(c4, a0, b4); DOT_ACC(c4, a1, b3); DOT_ACC(c4, a2, b2); DOT_ACC(c4, a3, b1); DOT_ACC(c4, a4, b0); \
    DOT_ACC(c5, a0, b5); DOT_ACC(c5, a1, b4); DOT_ACC(c5, a2, b3); DOT_ACC(c5, a3, b2); DOT_ACC(c5, a4, b1); DOT_ACC(c5, a5, b0); \
    DOT_ACC(c6, a0, b6); DOT_ACC(c6, a1, b5); DOT_ACC(c6, a2, b4); DOT_ACC(c6, a3, b3); DOT_ACC(c6, a4, b2); DOT_ACC(c6, a5, b1); DOT_ACC(c6, a6, b0); \
    DOT_ACC(c7, a0, b7); DOT_ACC(c7, a1, b6); DOT_ACC(c7, a2, b5); DOT_ACC(c7, a3, b4); DOT_ACC(c7, a4, b3); DOT_ACC(c7, a5, b2); DOT_ACC(c7, a6, b1); DOT_ACC(c7, a7, b0); \
    DOT_ACC(c8, a0, b8); DOT_ACC(c8, a1, b7); DOT_ACC(c8, a2, b6); DOT_ACC(c8, a3, b5); DOT_ACC(c8, a4, b4); DOT_ACC(c8, a5, b3); DOT_ACC(c8, a6, b2); DOT_ACC(c8, a7, b1); DOT_ACC(c8, a8, b0); \
    DOT_ACC(c9, a0, b9); DOT_ACC(c9, a1, b8); DOT_ACC(c9, a2, b7); DOT_ACC(c9, a3, b6); DOT_ACC(c9, a4, b5); DOT_ACC(c9, a5, b4); DOT_ACC(c9, a6, b3); DOT_ACC(c9, a7, b2); DOT_ACC(c9, a8, b1); DOT_ACC(c9, a9, b0); \
    DOT_ACC(c10, a1, b9); DOT_ACC(c10, a2, b8); DOT_ACC(c10, a3, b7); DOT_ACC(c10, a4, b6); DOT_ACC(c10, a5, b5); DOT_ACC(c10, a6, b4); DOT_ACC(c10, a7, b3); DOT_ACC(c10, a8, b2); DOT_ACC(c10, a9, b1); \
    DOT_ACC(c11, a2, b9); DOT_ACC(c11, a3, b8); DOT_ACC(c11, a4, b7); DOT_ACC(c11, a5, b6); DOT_ACC(c11, a6, b5); DOT_ACC(c11, a7, b4); DOT_ACC(c11, a8, b3); DOT_ACC(c11, a9, b2); \
    DOT_ACC(c12, a3, b9); DOT_ACC(c12, a4, b8); DOT_ACC(c12, a5, b7); DOT_ACC(c12, a6, b6); DOT_ACC(c12, a7, b5); DOT_ACC(c12, a8, b4); DOT_ACC(c12, a9, b3); \
    DOT_ACC(c13, a4, b9); DOT_ACC(c13, a5, b8); DOT_ACC(c13, a6, b7); DOT_ACC(c13, a7, b6); DOT_ACC(c13, a8, b5); DOT_ACC(c13, a9, b4); \
    DOT_ACC(c14, a5, b9); DOT_ACC(c14, a6, b8); DOT_ACC(c14, a7, b7); DOT_ACC(c14, a8, b6); DOT_ACC(c14, a9, b5); \
    DOT_ACC(c15, a6, b9); DOT_ACC(c15, a7, b8); DOT_ACC(c15, a8, b7); DOT_ACC(c15, a9, b6); \
    DOT_ACC(c16, a7, b9); DOT_ACC(c16, a8, b8); DOT_ACC(c16, a9, b7); \
    DOT_ACC(c17, a8, b9); DOT_ACC(c17, a9, b8); \
    DOT_ACC(c18, a9, b9)

#if QRUOV_q == 7
#define MATMULFQL10_FOLD_BODY                                            \
    ADD1(c0, c10); ADD2(c1, c10);                                        \
    ADD1(c1, c11); ADD2(c2, c11);                                        \
    ADD1(c2, c12); ADD2(c3, c12);                                        \
    ADD1(c3, c13); ADD2(c4, c13);                                        \
    ADD1(c4, c14); ADD2(c5, c14);                                        \
    ADD1(c5, c15); ADD2(c6, c15);                                        \
    ADD1(c6, c16); ADD2(c7, c16);                                        \
    ADD1(c7, c17); ADD2(c8, c17);                                        \
    ADD1(c8, c18); ADD2(c9, c18)
#elif QRUOV_q == 31
#define MATMULFQL10_FOLD_BODY                                            \
    ADD1(c0, c10); ADD5(c3, c10); ADD25(c3, c17);                        \
    ADD1(c1, c11); ADD5(c4, c11); ADD25(c4, c18);                        \
    ADD1(c2, c12); ADD5(c5, c12);                                        \
    ADD1(c3, c13); ADD5(c6, c13);                                        \
    ADD1(c4, c14); ADD5(c7, c14);                                        \
    ADD1(c5, c15); ADD5(c8, c15);                                        \
    ADD1(c6, c16); ADD5(c9, c16);                                        \
    ADD1(c7, c17); ADD5(c0, c17);                                        \
    ADD1(c8, c18); ADD5(c1, c18)
#elif QRUOV_q == 127
#define MATMULFQL10_FOLD_BODY                                            \
    ADD2(c10, c17); ADD2(c11, c18);                                      \
    ADD1(c0, c10); ADD2(c3, c10);                                        \
    ADD1(c1, c11); ADD2(c4, c11);                                        \
    ADD1(c2, c12); ADD2(c5, c12);                                        \
    ADD1(c3, c13); ADD2(c6, c13);                                        \
    ADD1(c4, c14); ADD2(c7, c14);                                        \
    ADD1(c5, c15); ADD2(c8, c15);                                        \
    ADD1(c6, c16); ADD2(c9, c16);                                        \
    ADD1(c7, c17);                                                       \
    ADD1(c8, c18)
#else
#error "Unsupported QRUOV_q for QRUOV_L == 10"
#endif

static void matmulfql10_vnni_1x4_impl(uint8_t *out, const uint8_t *A, const uint8_t *Bvnni, int K,
        int incout __attribute__((unused)), int incA __attribute__((unused)), int incB,
        int incLout, int incLA, int incLB)
{
    assert(K % 4 == 0);
    __m256i c0 = _mm256_setzero_si256();
    __m256i c1 = _mm256_setzero_si256();
    __m256i c2 = _mm256_setzero_si256();
    __m256i c3 = _mm256_setzero_si256();
    __m256i c4 = _mm256_setzero_si256();
    __m256i c5 = _mm256_setzero_si256();
    __m256i c6 = _mm256_setzero_si256();
    __m256i c7 = _mm256_setzero_si256();
    __m256i c8 = _mm256_setzero_si256();
    __m256i c9 = _mm256_setzero_si256();
    __m256i c10 = _mm256_setzero_si256();
    __m256i c11 = _mm256_setzero_si256();
    __m256i c12 = _mm256_setzero_si256();
    __m256i c13 = _mm256_setzero_si256();
    __m256i c14 = _mm256_setzero_si256();
    __m256i c15 = _mm256_setzero_si256();
    __m256i c16 = _mm256_setzero_si256();
    __m256i c17 = _mm256_setzero_si256();
    __m256i c18 = _mm256_setzero_si256();

    int k = 0;
    for (; k + 8 <= K; k += 8) {
        __m256i b0 = load_row_vnni_4x2(&Bvnni[(k + 0) * incB + 0 * incLB],
                                       &Bvnni[(k + 4) * incB + 0 * incLB]);
        __m256i b1 = load_row_vnni_4x2(&Bvnni[(k + 0) * incB + 1 * incLB],
                                       &Bvnni[(k + 4) * incB + 1 * incLB]);
        __m256i b2 = load_row_vnni_4x2(&Bvnni[(k + 0) * incB + 2 * incLB],
                                       &Bvnni[(k + 4) * incB + 2 * incLB]);
        __m256i b3 = load_row_vnni_4x2(&Bvnni[(k + 0) * incB + 3 * incLB],
                                       &Bvnni[(k + 4) * incB + 3 * incLB]);
        __m256i b4 = load_row_vnni_4x2(&Bvnni[(k + 0) * incB + 4 * incLB],
                                       &Bvnni[(k + 4) * incB + 4 * incLB]);
        __m256i b5 = load_row_vnni_4x2(&Bvnni[(k + 0) * incB + 5 * incLB],
                                       &Bvnni[(k + 4) * incB + 5 * incLB]);
        __m256i b6 = load_row_vnni_4x2(&Bvnni[(k + 0) * incB + 6 * incLB],
                                       &Bvnni[(k + 4) * incB + 6 * incLB]);
        __m256i b7 = load_row_vnni_4x2(&Bvnni[(k + 0) * incB + 7 * incLB],
                                       &Bvnni[(k + 4) * incB + 7 * incLB]);
        __m256i b8 = load_row_vnni_4x2(&Bvnni[(k + 0) * incB + 8 * incLB],
                                       &Bvnni[(k + 4) * incB + 8 * incLB]);
        __m256i b9 = load_row_vnni_4x2(&Bvnni[(k + 0) * incB + 9 * incLB],
                                       &Bvnni[(k + 4) * incB + 9 * incLB]);
        __m256i a0 = _mm256_set1_epi64x((long long)load_u64(&A[k + 0 * incLA]));
        __m256i a1 = _mm256_set1_epi64x((long long)load_u64(&A[k + 1 * incLA]));
        __m256i a2 = _mm256_set1_epi64x((long long)load_u64(&A[k + 2 * incLA]));
        __m256i a3 = _mm256_set1_epi64x((long long)load_u64(&A[k + 3 * incLA]));
        __m256i a4 = _mm256_set1_epi64x((long long)load_u64(&A[k + 4 * incLA]));
        __m256i a5 = _mm256_set1_epi64x((long long)load_u64(&A[k + 5 * incLA]));
        __m256i a6 = _mm256_set1_epi64x((long long)load_u64(&A[k + 6 * incLA]));
        __m256i a7 = _mm256_set1_epi64x((long long)load_u64(&A[k + 7 * incLA]));
        __m256i a8 = _mm256_set1_epi64x((long long)load_u64(&A[k + 8 * incLA]));
        __m256i a9 = _mm256_set1_epi64x((long long)load_u64(&A[k + 9 * incLA]));
        MATMULFQL10_DOT_BODY;
    }
    for (; k < K; k += 4) {
        __m256i b0 = load_row_vnni_4x1(&Bvnni[k * incB + 0 * incLB]);
        __m256i b1 = load_row_vnni_4x1(&Bvnni[k * incB + 1 * incLB]);
        __m256i b2 = load_row_vnni_4x1(&Bvnni[k * incB + 2 * incLB]);
        __m256i b3 = load_row_vnni_4x1(&Bvnni[k * incB + 3 * incLB]);
        __m256i b4 = load_row_vnni_4x1(&Bvnni[k * incB + 4 * incLB]);
        __m256i b5 = load_row_vnni_4x1(&Bvnni[k * incB + 5 * incLB]);
        __m256i b6 = load_row_vnni_4x1(&Bvnni[k * incB + 6 * incLB]);
        __m256i b7 = load_row_vnni_4x1(&Bvnni[k * incB + 7 * incLB]);
        __m256i b8 = load_row_vnni_4x1(&Bvnni[k * incB + 8 * incLB]);
        __m256i b9 = load_row_vnni_4x1(&Bvnni[k * incB + 9 * incLB]);
        __m256i a0 = _mm256_set1_epi64x((long long)load_u32(&A[k + 0 * incLA]));
        __m256i a1 = _mm256_set1_epi64x((long long)load_u32(&A[k + 1 * incLA]));
        __m256i a2 = _mm256_set1_epi64x((long long)load_u32(&A[k + 2 * incLA]));
        __m256i a3 = _mm256_set1_epi64x((long long)load_u32(&A[k + 3 * incLA]));
        __m256i a4 = _mm256_set1_epi64x((long long)load_u32(&A[k + 4 * incLA]));
        __m256i a5 = _mm256_set1_epi64x((long long)load_u32(&A[k + 5 * incLA]));
        __m256i a6 = _mm256_set1_epi64x((long long)load_u32(&A[k + 6 * incLA]));
        __m256i a7 = _mm256_set1_epi64x((long long)load_u32(&A[k + 7 * incLA]));
        __m256i a8 = _mm256_set1_epi64x((long long)load_u32(&A[k + 8 * incLA]));
        __m256i a9 = _mm256_set1_epi64x((long long)load_u32(&A[k + 9 * incLA]));
        MATMULFQL10_DOT_BODY;
    }
    MATMULFQL10_FOLD_BODY;
    store_row_4_pair_epi32(&out[0 * incLout], c0);
    store_row_4_pair_epi32(&out[1 * incLout], c1);
    store_row_4_pair_epi32(&out[2 * incLout], c2);
    store_row_4_pair_epi32(&out[3 * incLout], c3);
    store_row_4_pair_epi32(&out[4 * incLout], c4);
    store_row_4_pair_epi32(&out[5 * incLout], c5);
    store_row_4_pair_epi32(&out[6 * incLout], c6);
    store_row_4_pair_epi32(&out[7 * incLout], c7);
    store_row_4_pair_epi32(&out[8 * incLout], c8);
    store_row_4_pair_epi32(&out[9 * incLout], c9);
}

static void matmulfql10_vnni_1x8_impl(uint8_t *out, const uint8_t *A, const uint8_t *Bvnni, int K,
        int incout __attribute__((unused)), int incA __attribute__((unused)), int incB,
        int incLout, int incLA, int incLB)
{
    assert(K % 4 == 0);
    __m256i c0 = _mm256_setzero_si256();
    __m256i c1 = _mm256_setzero_si256();
    __m256i c2 = _mm256_setzero_si256();
    __m256i c3 = _mm256_setzero_si256();
    __m256i c4 = _mm256_setzero_si256();
    __m256i c5 = _mm256_setzero_si256();
    __m256i c6 = _mm256_setzero_si256();
    __m256i c7 = _mm256_setzero_si256();
    __m256i c8 = _mm256_setzero_si256();
    __m256i c9 = _mm256_setzero_si256();
    __m256i c10 = _mm256_setzero_si256();
    __m256i c11 = _mm256_setzero_si256();
    __m256i c12 = _mm256_setzero_si256();
    __m256i c13 = _mm256_setzero_si256();
    __m256i c14 = _mm256_setzero_si256();
    __m256i c15 = _mm256_setzero_si256();
    __m256i c16 = _mm256_setzero_si256();
    __m256i c17 = _mm256_setzero_si256();
    __m256i c18 = _mm256_setzero_si256();

    for (int k = 0; k < K; k += 4) {
        __m256i b0 = load_row_vnni_8(&Bvnni[k * incB + 0 * incLB]);
        __m256i b1 = load_row_vnni_8(&Bvnni[k * incB + 1 * incLB]);
        __m256i b2 = load_row_vnni_8(&Bvnni[k * incB + 2 * incLB]);
        __m256i b3 = load_row_vnni_8(&Bvnni[k * incB + 3 * incLB]);
        __m256i b4 = load_row_vnni_8(&Bvnni[k * incB + 4 * incLB]);
        __m256i b5 = load_row_vnni_8(&Bvnni[k * incB + 5 * incLB]);
        __m256i b6 = load_row_vnni_8(&Bvnni[k * incB + 6 * incLB]);
        __m256i b7 = load_row_vnni_8(&Bvnni[k * incB + 7 * incLB]);
        __m256i b8 = load_row_vnni_8(&Bvnni[k * incB + 8 * incLB]);
        __m256i b9 = load_row_vnni_8(&Bvnni[k * incB + 9 * incLB]);
        __m256i a0 = _mm256_set1_epi32(load_u32(&A[k + 0 * incLA]));
        __m256i a1 = _mm256_set1_epi32(load_u32(&A[k + 1 * incLA]));
        __m256i a2 = _mm256_set1_epi32(load_u32(&A[k + 2 * incLA]));
        __m256i a3 = _mm256_set1_epi32(load_u32(&A[k + 3 * incLA]));
        __m256i a4 = _mm256_set1_epi32(load_u32(&A[k + 4 * incLA]));
        __m256i a5 = _mm256_set1_epi32(load_u32(&A[k + 5 * incLA]));
        __m256i a6 = _mm256_set1_epi32(load_u32(&A[k + 6 * incLA]));
        __m256i a7 = _mm256_set1_epi32(load_u32(&A[k + 7 * incLA]));
        __m256i a8 = _mm256_set1_epi32(load_u32(&A[k + 8 * incLA]));
        __m256i a9 = _mm256_set1_epi32(load_u32(&A[k + 9 * incLA]));
        MATMULFQL10_DOT_BODY;
    }
    MATMULFQL10_FOLD_BODY;
    store_row_8_epi32(&out[0 * incLout], c0);
    store_row_8_epi32(&out[1 * incLout], c1);
    store_row_8_epi32(&out[2 * incLout], c2);
    store_row_8_epi32(&out[3 * incLout], c3);
    store_row_8_epi32(&out[4 * incLout], c4);
    store_row_8_epi32(&out[5 * incLout], c5);
    store_row_8_epi32(&out[6 * incLout], c6);
    store_row_8_epi32(&out[7 * incLout], c7);
    store_row_8_epi32(&out[8 * incLout], c8);
    store_row_8_epi32(&out[9 * incLout], c9);
}

static void matmulfql10_vnni_1x7_impl(uint8_t *out, const uint8_t *A, const uint8_t *Bvnni, int K,
        int incout __attribute__((unused)), int incA __attribute__((unused)), int incB,
        int incLout, int incLA, int incLB)
{
    assert(K % 4 == 0);
    __m256i c0 = _mm256_setzero_si256();
    __m256i c1 = _mm256_setzero_si256();
    __m256i c2 = _mm256_setzero_si256();
    __m256i c3 = _mm256_setzero_si256();
    __m256i c4 = _mm256_setzero_si256();
    __m256i c5 = _mm256_setzero_si256();
    __m256i c6 = _mm256_setzero_si256();
    __m256i c7 = _mm256_setzero_si256();
    __m256i c8 = _mm256_setzero_si256();
    __m256i c9 = _mm256_setzero_si256();
    __m256i c10 = _mm256_setzero_si256();
    __m256i c11 = _mm256_setzero_si256();
    __m256i c12 = _mm256_setzero_si256();
    __m256i c13 = _mm256_setzero_si256();
    __m256i c14 = _mm256_setzero_si256();
    __m256i c15 = _mm256_setzero_si256();
    __m256i c16 = _mm256_setzero_si256();
    __m256i c17 = _mm256_setzero_si256();
    __m256i c18 = _mm256_setzero_si256();

    for (int k = 0; k < K; k += 4) {
        __m256i b0 = load_row_vnni_7(&Bvnni[k * incB + 0 * incLB]);
        __m256i b1 = load_row_vnni_7(&Bvnni[k * incB + 1 * incLB]);
        __m256i b2 = load_row_vnni_7(&Bvnni[k * incB + 2 * incLB]);
        __m256i b3 = load_row_vnni_7(&Bvnni[k * incB + 3 * incLB]);
        __m256i b4 = load_row_vnni_7(&Bvnni[k * incB + 4 * incLB]);
        __m256i b5 = load_row_vnni_7(&Bvnni[k * incB + 5 * incLB]);
        __m256i b6 = load_row_vnni_7(&Bvnni[k * incB + 6 * incLB]);
        __m256i b7 = load_row_vnni_7(&Bvnni[k * incB + 7 * incLB]);
        __m256i b8 = load_row_vnni_7(&Bvnni[k * incB + 8 * incLB]);
        __m256i b9 = load_row_vnni_7(&Bvnni[k * incB + 9 * incLB]);
        __m256i a0 = _mm256_set1_epi32(load_u32(&A[k + 0 * incLA]));
        __m256i a1 = _mm256_set1_epi32(load_u32(&A[k + 1 * incLA]));
        __m256i a2 = _mm256_set1_epi32(load_u32(&A[k + 2 * incLA]));
        __m256i a3 = _mm256_set1_epi32(load_u32(&A[k + 3 * incLA]));
        __m256i a4 = _mm256_set1_epi32(load_u32(&A[k + 4 * incLA]));
        __m256i a5 = _mm256_set1_epi32(load_u32(&A[k + 5 * incLA]));
        __m256i a6 = _mm256_set1_epi32(load_u32(&A[k + 6 * incLA]));
        __m256i a7 = _mm256_set1_epi32(load_u32(&A[k + 7 * incLA]));
        __m256i a8 = _mm256_set1_epi32(load_u32(&A[k + 8 * incLA]));
        __m256i a9 = _mm256_set1_epi32(load_u32(&A[k + 9 * incLA]));
        MATMULFQL10_DOT_BODY;
    }
    MATMULFQL10_FOLD_BODY;
    store_row_7_epi32(&out[0 * incLout], c0);
    store_row_7_epi32(&out[1 * incLout], c1);
    store_row_7_epi32(&out[2 * incLout], c2);
    store_row_7_epi32(&out[3 * incLout], c3);
    store_row_7_epi32(&out[4 * incLout], c4);
    store_row_7_epi32(&out[5 * incLout], c5);
    store_row_7_epi32(&out[6 * incLout], c6);
    store_row_7_epi32(&out[7 * incLout], c7);
    store_row_7_epi32(&out[8 * incLout], c8);
    store_row_7_epi32(&out[9 * incLout], c9);
}

static void matmulfql10_vnni_1x6_impl(uint8_t *out, const uint8_t *A, const uint8_t *Bvnni, int K,
        int incout __attribute__((unused)), int incA __attribute__((unused)), int incB,
        int incLout, int incLA, int incLB)
{
    assert(K % 4 == 0);
    __m256i c0 = _mm256_setzero_si256();
    __m256i c1 = _mm256_setzero_si256();
    __m256i c2 = _mm256_setzero_si256();
    __m256i c3 = _mm256_setzero_si256();
    __m256i c4 = _mm256_setzero_si256();
    __m256i c5 = _mm256_setzero_si256();
    __m256i c6 = _mm256_setzero_si256();
    __m256i c7 = _mm256_setzero_si256();
    __m256i c8 = _mm256_setzero_si256();
    __m256i c9 = _mm256_setzero_si256();
    __m256i c10 = _mm256_setzero_si256();
    __m256i c11 = _mm256_setzero_si256();
    __m256i c12 = _mm256_setzero_si256();
    __m256i c13 = _mm256_setzero_si256();
    __m256i c14 = _mm256_setzero_si256();
    __m256i c15 = _mm256_setzero_si256();
    __m256i c16 = _mm256_setzero_si256();
    __m256i c17 = _mm256_setzero_si256();
    __m256i c18 = _mm256_setzero_si256();

    for (int k = 0; k < K; k += 4) {
        __m256i b0 = load_row_vnni_6(&Bvnni[k * incB + 0 * incLB]);
        __m256i b1 = load_row_vnni_6(&Bvnni[k * incB + 1 * incLB]);
        __m256i b2 = load_row_vnni_6(&Bvnni[k * incB + 2 * incLB]);
        __m256i b3 = load_row_vnni_6(&Bvnni[k * incB + 3 * incLB]);
        __m256i b4 = load_row_vnni_6(&Bvnni[k * incB + 4 * incLB]);
        __m256i b5 = load_row_vnni_6(&Bvnni[k * incB + 5 * incLB]);
        __m256i b6 = load_row_vnni_6(&Bvnni[k * incB + 6 * incLB]);
        __m256i b7 = load_row_vnni_6(&Bvnni[k * incB + 7 * incLB]);
        __m256i b8 = load_row_vnni_6(&Bvnni[k * incB + 8 * incLB]);
        __m256i b9 = load_row_vnni_6(&Bvnni[k * incB + 9 * incLB]);
        __m256i a0 = _mm256_set1_epi32(load_u32(&A[k + 0 * incLA]));
        __m256i a1 = _mm256_set1_epi32(load_u32(&A[k + 1 * incLA]));
        __m256i a2 = _mm256_set1_epi32(load_u32(&A[k + 2 * incLA]));
        __m256i a3 = _mm256_set1_epi32(load_u32(&A[k + 3 * incLA]));
        __m256i a4 = _mm256_set1_epi32(load_u32(&A[k + 4 * incLA]));
        __m256i a5 = _mm256_set1_epi32(load_u32(&A[k + 5 * incLA]));
        __m256i a6 = _mm256_set1_epi32(load_u32(&A[k + 6 * incLA]));
        __m256i a7 = _mm256_set1_epi32(load_u32(&A[k + 7 * incLA]));
        __m256i a8 = _mm256_set1_epi32(load_u32(&A[k + 8 * incLA]));
        __m256i a9 = _mm256_set1_epi32(load_u32(&A[k + 9 * incLA]));
        MATMULFQL10_DOT_BODY;
    }
    MATMULFQL10_FOLD_BODY;
    store_row_6_epi32(&out[0 * incLout], c0);
    store_row_6_epi32(&out[1 * incLout], c1);
    store_row_6_epi32(&out[2 * incLout], c2);
    store_row_6_epi32(&out[3 * incLout], c3);
    store_row_6_epi32(&out[4 * incLout], c4);
    store_row_6_epi32(&out[5 * incLout], c5);
    store_row_6_epi32(&out[6 * incLout], c6);
    store_row_6_epi32(&out[7 * incLout], c7);
    store_row_6_epi32(&out[8 * incLout], c8);
    store_row_6_epi32(&out[9 * incLout], c9);
}

#undef MATMULFQL10_DOT_BODY
#undef MATMULFQL10_FOLD_BODY
#undef DOT_ACC

#else
#error "Unsupported QRUOV_L"
#endif

void matmulfql_vnni_1x8(uint8_t *out, const uint8_t *A, const uint8_t *Bvnni, int K,
        int incout, int incA, int incB, int incLout, int incLA, int incLB)
{
#if QRUOV_L == 3
    matmulfql3_vnni_1x8_impl(out, A, Bvnni, K, incout, incA, incB, incLout, incLA, incLB);
#elif QRUOV_L == 10
    matmulfql10_vnni_1x8_impl(out, A, Bvnni, K, incout, incA, incB, incLout, incLA, incLB);
#else
#error "Unsupported QRUOV_L"
#endif
}

void matmulfql_vnni_1x7(uint8_t *out, const uint8_t *A, const uint8_t *Bvnni, int K,
        int incout, int incA, int incB, int incLout, int incLA, int incLB)
{
#if QRUOV_L == 3
    matmulfql3_vnni_1x7_impl(out, A, Bvnni, K, incout, incA, incB, incLout, incLA, incLB);
#elif QRUOV_L == 10
    matmulfql10_vnni_1x7_impl(out, A, Bvnni, K, incout, incA, incB, incLout, incLA, incLB);
#else
#error "Unsupported QRUOV_L"
#endif
}

void matmulfql_vnni_1x6(uint8_t *out, const uint8_t *A, const uint8_t *Bvnni, int K,
        int incout, int incA, int incB, int incLout, int incLA, int incLB)
{
#if QRUOV_L == 3
    matmulfql3_vnni_1x6_impl(out, A, Bvnni, K, incout, incA, incB, incLout, incLA, incLB);
#elif QRUOV_L == 10
    matmulfql10_vnni_1x6_impl(out, A, Bvnni, K, incout, incA, incB, incLout, incLA, incLB);
#else
#error "Unsupported QRUOV_L"
#endif
}

void matmulfql_vnni_1x4(uint8_t *out, const uint8_t *A, const uint8_t *Bvnni, int K,
        int incout, int incA, int incB, int incLout, int incLA, int incLB)
{
#if QRUOV_L == 3
    matmulfql3_vnni_1x4_impl(out, A, Bvnni, K, incout, incA, incB, incLout, incLA, incLB);
#elif QRUOV_L == 10
    matmulfql10_vnni_1x4_impl(out, A, Bvnni, K, incout, incA, incB, incLout, incLA, incLB);
#else
#error "Unsupported QRUOV_L"
#endif
}

#if QRUOV_q != 7 && QRUOV_q != 31
static void matmulfq_vnni_4x16_impl(uint8_t *out, const uint8_t *A, const uint8_t *B, int K, int incout, int incA, int incB)
{
    assert(K % 4 == 0);
    __m256i c00 = _mm256_setzero_si256();
    __m256i c01 = _mm256_setzero_si256();
    __m256i c10 = _mm256_setzero_si256();
    __m256i c11 = _mm256_setzero_si256();
    __m256i c20 = _mm256_setzero_si256();
    __m256i c21 = _mm256_setzero_si256();
    __m256i c30 = _mm256_setzero_si256();
    __m256i c31 = _mm256_setzero_si256();

    for (int k = 0; k < K; k += 4) {
        __m256i b0, b1;
        load_row_vnni_16(&b0, &b1, &B[(k >> 2) * incB]);
        __m256i a0 = _mm256_set1_epi32(load_u32(&A[0 * incA + k]));
        __m256i a1 = _mm256_set1_epi32(load_u32(&A[1 * incA + k]));
        __m256i a2 = _mm256_set1_epi32(load_u32(&A[2 * incA + k]));
        __m256i a3 = _mm256_set1_epi32(load_u32(&A[3 * incA + k]));
        c00 = ymm_dot_u8s8(c00, a0, b0);
        c01 = ymm_dot_u8s8(c01, a0, b1);
        c10 = ymm_dot_u8s8(c10, a1, b0);
        c11 = ymm_dot_u8s8(c11, a1, b1);
        c20 = ymm_dot_u8s8(c20, a2, b0);
        c21 = ymm_dot_u8s8(c21, a2, b1);
        c30 = ymm_dot_u8s8(c30, a3, b0);
        c31 = ymm_dot_u8s8(c31, a3, b1);
    }

    store_row_16_epi32(&out[0 * incout], c00, c01);
    store_row_16_epi32(&out[1 * incout], c10, c11);
    store_row_16_epi32(&out[2 * incout], c20, c21);
    store_row_16_epi32(&out[3 * incout], c30, c31);
}
#endif

#if QRUOV_q == 7
// q=7 can keep maddubs pair-sums in 16-bit lanes and reduce mod 7 at the end.
static void matmulfq7_vnni_4x16_impl(uint8_t *out, const uint8_t *A, const uint8_t *B, int K, int incout, int incA, int incB)
{
    assert(K % 4 == 0);
    __m256i c00 = _mm256_setzero_si256();
    __m256i c01 = _mm256_setzero_si256();
    __m256i c10 = _mm256_setzero_si256();
    __m256i c11 = _mm256_setzero_si256();
    __m256i c20 = _mm256_setzero_si256();
    __m256i c21 = _mm256_setzero_si256();
    __m256i c30 = _mm256_setzero_si256();
    __m256i c31 = _mm256_setzero_si256();

    for (int k = 0; k < K; k += 4) {
        __m256i b0, b1;
        load_row_vnni_16(&b0, &b1, &B[(k >> 2) * incB]);
        __m256i a0 = _mm256_set1_epi32(load_u32(&A[0 * incA + k]));
        __m256i a1 = _mm256_set1_epi32(load_u32(&A[1 * incA + k]));
        __m256i a2 = _mm256_set1_epi32(load_u32(&A[2 * incA + k]));
        __m256i a3 = _mm256_set1_epi32(load_u32(&A[3 * incA + k]));
        c00 = _mm256_add_epi16(c00, _mm256_maddubs_epi16(a0, b0));
        c01 = _mm256_add_epi16(c01, _mm256_maddubs_epi16(a0, b1));
        c10 = _mm256_add_epi16(c10, _mm256_maddubs_epi16(a1, b0));
        c11 = _mm256_add_epi16(c11, _mm256_maddubs_epi16(a1, b1));
        c20 = _mm256_add_epi16(c20, _mm256_maddubs_epi16(a2, b0));
        c21 = _mm256_add_epi16(c21, _mm256_maddubs_epi16(a2, b1));
        c30 = _mm256_add_epi16(c30, _mm256_maddubs_epi16(a3, b0));
        c31 = _mm256_add_epi16(c31, _mm256_maddubs_epi16(a3, b1));
    }

    store_row_16_hadd_epi16(&out[0 * incout], c00, c01);
    store_row_16_hadd_epi16(&out[1 * incout], c10, c11);
    store_row_16_hadd_epi16(&out[2 * incout], c20, c21);
    store_row_16_hadd_epi16(&out[3 * incout], c30, c31);
}
#elif QRUOV_q == 31

// q=31 uses the unsafe grouped dot paths; even the 4-way case stays below
// the signed 16-bit lane limit: 4 * 2 * (31 - 1)^2 = 7200.
static void matmulfq31_vnni_4x16_impl(uint8_t *out, const uint8_t *A, const uint8_t *B, int K, int incout, int incA, int incB)
{
    assert(K % 4 == 0);
    __m256i c00 = _mm256_setzero_si256();
    __m256i c01 = _mm256_setzero_si256();
    __m256i c10 = _mm256_setzero_si256();
    __m256i c11 = _mm256_setzero_si256();
    __m256i c20 = _mm256_setzero_si256();
    __m256i c21 = _mm256_setzero_si256();
    __m256i c30 = _mm256_setzero_si256();
    __m256i c31 = _mm256_setzero_si256();

    int k = 0;
    for (; k + 8 <= K; k += 8) {
        __m256i b00, b01, b40, b41;
        load_row_vnni_16(&b00, &b01, &B[((k >> 2) + 0) * incB]);
        load_row_vnni_16(&b40, &b41, &B[((k >> 2) + 1) * incB]);
        __m256i a00 = _mm256_set1_epi32(load_u32(&A[0 * incA + k + 0]));
        __m256i a04 = _mm256_set1_epi32(load_u32(&A[0 * incA + k + 4]));
        __m256i a10 = _mm256_set1_epi32(load_u32(&A[1 * incA + k + 0]));
        __m256i a14 = _mm256_set1_epi32(load_u32(&A[1 * incA + k + 4]));
        __m256i a20 = _mm256_set1_epi32(load_u32(&A[2 * incA + k + 0]));
        __m256i a24 = _mm256_set1_epi32(load_u32(&A[2 * incA + k + 4]));
        __m256i a30 = _mm256_set1_epi32(load_u32(&A[3 * incA + k + 0]));
        __m256i a34 = _mm256_set1_epi32(load_u32(&A[3 * incA + k + 4]));
        c00 = ymm2_dot_u8s8_unsafe(c00, a00, b00, a04, b40);
        c01 = ymm2_dot_u8s8_unsafe(c01, a00, b01, a04, b41);
        c10 = ymm2_dot_u8s8_unsafe(c10, a10, b00, a14, b40);
        c11 = ymm2_dot_u8s8_unsafe(c11, a10, b01, a14, b41);
        c20 = ymm2_dot_u8s8_unsafe(c20, a20, b00, a24, b40);
        c21 = ymm2_dot_u8s8_unsafe(c21, a20, b01, a24, b41);
        c30 = ymm2_dot_u8s8_unsafe(c30, a30, b00, a34, b40);
        c31 = ymm2_dot_u8s8_unsafe(c31, a30, b01, a34, b41);
    }

    for (; k < K; k += 4) {
        __m256i b0, b1;
        load_row_vnni_16(&b0, &b1, &B[(k >> 2) * incB]);
        __m256i a0 = _mm256_set1_epi32(load_u32(&A[0 * incA + k]));
        __m256i a1 = _mm256_set1_epi32(load_u32(&A[1 * incA + k]));
        __m256i a2 = _mm256_set1_epi32(load_u32(&A[2 * incA + k]));
        __m256i a3 = _mm256_set1_epi32(load_u32(&A[3 * incA + k]));
        c00 = ymm_dot_u8s8(c00, a0, b0);
        c01 = ymm_dot_u8s8(c01, a0, b1);
        c10 = ymm_dot_u8s8(c10, a1, b0);
        c11 = ymm_dot_u8s8(c11, a1, b1);
        c20 = ymm_dot_u8s8(c20, a2, b0);
        c21 = ymm_dot_u8s8(c21, a2, b1);
        c30 = ymm_dot_u8s8(c30, a3, b0);
        c31 = ymm_dot_u8s8(c31, a3, b1);
    }

    store_row_16_epi32(&out[0 * incout], c00, c01);
    store_row_16_epi32(&out[1 * incout], c10, c11);
    store_row_16_epi32(&out[2 * incout], c20, c21);
    store_row_16_epi32(&out[3 * incout], c30, c31);
}
#endif

void matmulfq_vnni_4x16(uint8_t *out, const uint8_t *A, const uint8_t *B, int K, int incout, int incA, int incB)
{
#if QRUOV_q == 7
    matmulfq7_vnni_4x16_impl(out, A, B, K, incout, incA, incB);
#elif QRUOV_q == 31
    matmulfq31_vnni_4x16_impl(out, A, B, K, incout, incA, incB);
#else
    matmulfq_vnni_4x16_impl(out, A, B, K, incout, incA, incB);
#endif
}

#if QRUOV_q != 7 && QRUOV_q != 31
static void matmulfq_vnni_4x8_impl(uint8_t *out, const uint8_t *A, const uint8_t *B, int K, int incout, int incA, int incB)
{
    assert(K % 4 == 0);
    __m256i c0 = _mm256_setzero_si256();
    __m256i c1 = _mm256_setzero_si256();
    __m256i c2 = _mm256_setzero_si256();
    __m256i c3 = _mm256_setzero_si256();

    for (int k = 0; k < K; k += 4) {
        __m256i b = load_row_vnni_8(&B[(k >> 2) * incB]);
        __m256i a0 = _mm256_set1_epi32(load_u32(&A[0 * incA + k]));
        __m256i a1 = _mm256_set1_epi32(load_u32(&A[1 * incA + k]));
        __m256i a2 = _mm256_set1_epi32(load_u32(&A[2 * incA + k]));
        __m256i a3 = _mm256_set1_epi32(load_u32(&A[3 * incA + k]));
        c0 = ymm_dot_u8s8(c0, a0, b);
        c1 = ymm_dot_u8s8(c1, a1, b);
        c2 = ymm_dot_u8s8(c2, a2, b);
        c3 = ymm_dot_u8s8(c3, a3, b);
    }

    store_row_8_epi32(&out[0 * incout], c0);
    store_row_8_epi32(&out[1 * incout], c1);
    store_row_8_epi32(&out[2 * incout], c2);
    store_row_8_epi32(&out[3 * incout], c3);
}
#endif

#if QRUOV_q == 7
static void matmulfq7_vnni_4x8_impl(uint8_t *out, const uint8_t *A, const uint8_t *B, int K, int incout, int incA, int incB)
{
    assert(K % 4 == 0);
    __m256i c0 = _mm256_setzero_si256();
    __m256i c1 = _mm256_setzero_si256();
    __m256i c2 = _mm256_setzero_si256();
    __m256i c3 = _mm256_setzero_si256();

    for (int k = 0; k < K; k += 4) {
        __m256i b = load_row_vnni_8(&B[(k >> 2) * incB]);
        __m256i a0 = _mm256_set1_epi32(load_u32(&A[0 * incA + k]));
        __m256i a1 = _mm256_set1_epi32(load_u32(&A[1 * incA + k]));
        __m256i a2 = _mm256_set1_epi32(load_u32(&A[2 * incA + k]));
        __m256i a3 = _mm256_set1_epi32(load_u32(&A[3 * incA + k]));
        c0 = _mm256_add_epi16(c0, _mm256_maddubs_epi16(a0, b));
        c1 = _mm256_add_epi16(c1, _mm256_maddubs_epi16(a1, b));
        c2 = _mm256_add_epi16(c2, _mm256_maddubs_epi16(a2, b));
        c3 = _mm256_add_epi16(c3, _mm256_maddubs_epi16(a3, b));
    }

    store_row_8_hadd_epi16(&out[0 * incout], c0);
    store_row_8_hadd_epi16(&out[1 * incout], c1);
    store_row_8_hadd_epi16(&out[2 * incout], c2);
    store_row_8_hadd_epi16(&out[3 * incout], c3);
}
#elif QRUOV_q == 31
static void matmulfq31_vnni_4x8_impl(uint8_t *out, const uint8_t *A, const uint8_t *B, int K, int incout, int incA, int incB)
{
    assert(K % 4 == 0);
    __m256i c0 = _mm256_setzero_si256();
    __m256i c1 = _mm256_setzero_si256();
    __m256i c2 = _mm256_setzero_si256();
    __m256i c3 = _mm256_setzero_si256();

    int k = 0;
    for (; k + 16 <= K; k += 16) {
        __m256i b0 = load_row_vnni_8(&B[((k >> 2) + 0) * incB]);
        __m256i b1 = load_row_vnni_8(&B[((k >> 2) + 1) * incB]);
        __m256i b2 = load_row_vnni_8(&B[((k >> 2) + 2) * incB]);
        __m256i b3 = load_row_vnni_8(&B[((k >> 2) + 3) * incB]);
        __m256i a00 = _mm256_set1_epi32(load_u32(&A[0 * incA + k + 0]));
        __m256i a01 = _mm256_set1_epi32(load_u32(&A[0 * incA + k + 4]));
        __m256i a02 = _mm256_set1_epi32(load_u32(&A[0 * incA + k + 8]));
        __m256i a03 = _mm256_set1_epi32(load_u32(&A[0 * incA + k + 12]));
        __m256i a10 = _mm256_set1_epi32(load_u32(&A[1 * incA + k + 0]));
        __m256i a11 = _mm256_set1_epi32(load_u32(&A[1 * incA + k + 4]));
        __m256i a12 = _mm256_set1_epi32(load_u32(&A[1 * incA + k + 8]));
        __m256i a13 = _mm256_set1_epi32(load_u32(&A[1 * incA + k + 12]));
        __m256i a20 = _mm256_set1_epi32(load_u32(&A[2 * incA + k + 0]));
        __m256i a21 = _mm256_set1_epi32(load_u32(&A[2 * incA + k + 4]));
        __m256i a22 = _mm256_set1_epi32(load_u32(&A[2 * incA + k + 8]));
        __m256i a23 = _mm256_set1_epi32(load_u32(&A[2 * incA + k + 12]));
        __m256i a30 = _mm256_set1_epi32(load_u32(&A[3 * incA + k + 0]));
        __m256i a31 = _mm256_set1_epi32(load_u32(&A[3 * incA + k + 4]));
        __m256i a32 = _mm256_set1_epi32(load_u32(&A[3 * incA + k + 8]));
        __m256i a33 = _mm256_set1_epi32(load_u32(&A[3 * incA + k + 12]));
        c0 = ymm4_dot_u8s8_unsafe(c0, a00, b0, a01, b1, a02, b2, a03, b3);
        c1 = ymm4_dot_u8s8_unsafe(c1, a10, b0, a11, b1, a12, b2, a13, b3);
        c2 = ymm4_dot_u8s8_unsafe(c2, a20, b0, a21, b1, a22, b2, a23, b3);
        c3 = ymm4_dot_u8s8_unsafe(c3, a30, b0, a31, b1, a32, b2, a33, b3);
    }

    for (; k < K; k += 4) {
        __m256i b = load_row_vnni_8(&B[(k >> 2) * incB]);
        __m256i a0 = _mm256_set1_epi32(load_u32(&A[0 * incA + k]));
        __m256i a1 = _mm256_set1_epi32(load_u32(&A[1 * incA + k]));
        __m256i a2 = _mm256_set1_epi32(load_u32(&A[2 * incA + k]));
        __m256i a3 = _mm256_set1_epi32(load_u32(&A[3 * incA + k]));
        c0 = ymm_dot_u8s8(c0, a0, b);
        c1 = ymm_dot_u8s8(c1, a1, b);
        c2 = ymm_dot_u8s8(c2, a2, b);
        c3 = ymm_dot_u8s8(c3, a3, b);
    }

    store_row_8_epi32(&out[0 * incout], c0);
    store_row_8_epi32(&out[1 * incout], c1);
    store_row_8_epi32(&out[2 * incout], c2);
    store_row_8_epi32(&out[3 * incout], c3);
}
#endif

void matmulfq_vnni_4x8(uint8_t *out, const uint8_t *A, const uint8_t *B, int K, int incout, int incA, int incB)
{
#if QRUOV_q == 7
    matmulfq7_vnni_4x8_impl(out, A, B, K, incout, incA, incB);
#elif QRUOV_q == 31
    matmulfq31_vnni_4x8_impl(out, A, B, K, incout, incA, incB);
#else
    matmulfq_vnni_4x8_impl(out, A, B, K, incout, incA, incB);
#endif
}

void matmulfq_vnni_4x7(uint8_t *out, const uint8_t *A, const uint8_t *B, int K, int incout, int incA, int incB)
{
    assert(K % 4 == 0);
    __m256i c0 = _mm256_setzero_si256();
    __m256i c1 = _mm256_setzero_si256();
    __m256i c2 = _mm256_setzero_si256();
    __m256i c3 = _mm256_setzero_si256();

    for (int k = 0; k < K; k += 4) {
        __m256i b = load_row_vnni_7(&B[(k >> 2) * incB]);
        __m256i a0 = _mm256_set1_epi32(load_u32(&A[0 * incA + k]));
        __m256i a1 = _mm256_set1_epi32(load_u32(&A[1 * incA + k]));
        __m256i a2 = _mm256_set1_epi32(load_u32(&A[2 * incA + k]));
        __m256i a3 = _mm256_set1_epi32(load_u32(&A[3 * incA + k]));
        c0 = ymm_dot_u8s8(c0, a0, b);
        c1 = ymm_dot_u8s8(c1, a1, b);
        c2 = ymm_dot_u8s8(c2, a2, b);
        c3 = ymm_dot_u8s8(c3, a3, b);
    }

    store_row_7_epi32(&out[0 * incout], c0);
    store_row_7_epi32(&out[1 * incout], c1);
    store_row_7_epi32(&out[2 * incout], c2);
    store_row_7_epi32(&out[3 * incout], c3);
}

void matmulfq_vnni_4x6(uint8_t *out, const uint8_t *A, const uint8_t *B, int K, int incout, int incA, int incB)
{
    assert(K % 4 == 0);
    __m256i c0 = _mm256_setzero_si256();
    __m256i c1 = _mm256_setzero_si256();
    __m256i c2 = _mm256_setzero_si256();
    __m256i c3 = _mm256_setzero_si256();

    for (int k = 0; k < K; k += 4) {
        __m256i b = load_row_vnni_6(&B[(k >> 2) * incB]);
        __m256i a0 = _mm256_set1_epi32(load_u32(&A[0 * incA + k]));
        __m256i a1 = _mm256_set1_epi32(load_u32(&A[1 * incA + k]));
        __m256i a2 = _mm256_set1_epi32(load_u32(&A[2 * incA + k]));
        __m256i a3 = _mm256_set1_epi32(load_u32(&A[3 * incA + k]));
        c0 = ymm_dot_u8s8(c0, a0, b);
        c1 = ymm_dot_u8s8(c1, a1, b);
        c2 = ymm_dot_u8s8(c2, a2, b);
        c3 = ymm_dot_u8s8(c3, a3, b);
    }

    store_row_6_epi32(&out[0 * incout], c0);
    store_row_6_epi32(&out[1 * incout], c1);
    store_row_6_epi32(&out[2 * incout], c2);
    store_row_6_epi32(&out[3 * incout], c3);
}

void matmulfq_vnni_4x4(uint8_t *out, const uint8_t *A, const uint8_t *B, int K, int incout, int incA, int incB)
{
    assert(K % 4 == 0);
    __m256i c0 = _mm256_setzero_si256();
    __m256i c1 = _mm256_setzero_si256();
    __m256i c2 = _mm256_setzero_si256();
    __m256i c3 = _mm256_setzero_si256();
    const __m256i idx = _mm256_set_epi32(7, 3, 6, 2, 5, 1, 4, 0);

    int k = 0;
    for (; k + 16 <= K; k += 16) {
        __m128i b00 = _mm_loadu_si128((const __m128i *)&B[((k >> 2) + 0) * incB]);
        __m128i b04 = _mm_loadu_si128((const __m128i *)&B[((k >> 2) + 1) * incB]);
        __m128i b08 = _mm_loadu_si128((const __m128i *)&B[((k >> 2) + 2) * incB]);
        __m128i b12 = _mm_loadu_si128((const __m128i *)&B[((k >> 2) + 3) * incB]);
        __m256i bk0 = _mm256_set_m128i(b04, b00);
        __m256i bk1 = _mm256_set_m128i(b12, b08);
        bk0 = _mm256_permutevar8x32_epi32(bk0, idx);
        bk1 = _mm256_permutevar8x32_epi32(bk1, idx);

        __m256i a00 = _mm256_set1_epi64x((long long)load_u64(&A[0 * incA + k + 0]));
        __m256i a01 = _mm256_set1_epi64x((long long)load_u64(&A[0 * incA + k + 8]));
        __m256i a10 = _mm256_set1_epi64x((long long)load_u64(&A[1 * incA + k + 0]));
        __m256i a11 = _mm256_set1_epi64x((long long)load_u64(&A[1 * incA + k + 8]));
        __m256i a20 = _mm256_set1_epi64x((long long)load_u64(&A[2 * incA + k + 0]));
        __m256i a21 = _mm256_set1_epi64x((long long)load_u64(&A[2 * incA + k + 8]));
        __m256i a30 = _mm256_set1_epi64x((long long)load_u64(&A[3 * incA + k + 0]));
        __m256i a31 = _mm256_set1_epi64x((long long)load_u64(&A[3 * incA + k + 8]));
#if QRUOV_q == 127
        c0 = ymm2_dot_u8s8(c0, a00, bk0, a01, bk1);
        c1 = ymm2_dot_u8s8(c1, a10, bk0, a11, bk1);
        c2 = ymm2_dot_u8s8(c2, a20, bk0, a21, bk1);
        c3 = ymm2_dot_u8s8(c3, a30, bk0, a31, bk1);
#else
        c0 = ymm2_dot_u8s8_unsafe(c0, a00, bk0, a01, bk1);
        c1 = ymm2_dot_u8s8_unsafe(c1, a10, bk0, a11, bk1);
        c2 = ymm2_dot_u8s8_unsafe(c2, a20, bk0, a21, bk1);
        c3 = ymm2_dot_u8s8_unsafe(c3, a30, bk0, a31, bk1);
#endif
    }

    for (; k < K; k += 4) {
        __m128i b0 = _mm_loadu_si128((const __m128i *)&B[(k >> 2) * incB]);
        __m256i b = _mm256_set_m128i(_mm_setzero_si128(), b0);
        b = _mm256_permutevar8x32_epi32(b, idx);
        __m256i a0 = _mm256_set1_epi64x((long long)load_u32(&A[0 * incA + k]));
        __m256i a1 = _mm256_set1_epi64x((long long)load_u32(&A[1 * incA + k]));
        __m256i a2 = _mm256_set1_epi64x((long long)load_u32(&A[2 * incA + k]));
        __m256i a3 = _mm256_set1_epi64x((long long)load_u32(&A[3 * incA + k]));
        c0 = ymm_dot_u8s8(c0, a0, b);
        c1 = ymm_dot_u8s8(c1, a1, b);
        c2 = ymm_dot_u8s8(c2, a2, b);
        c3 = ymm_dot_u8s8(c3, a3, b);
    }

    __m256i t01 = _mm256_hadd_epi32(c0, c1);
    __m256i t23 = _mm256_hadd_epi32(c2, c3);
    t01 = _mm256_permute4x64_epi64(t01, _MM_SHUFFLE(3, 1, 2, 0));
    t23 = _mm256_permute4x64_epi64(t23, _MM_SHUFFLE(3, 1, 2, 0));
    __m128i r01 = ymm_modq_epi32_epi8(t01);
    __m128i r23 = ymm_modq_epi32_epi8(t23);
    store_u32(&out[0 * incout], (uint32_t)_mm_extract_epi32(r01, 0));
    store_u32(&out[1 * incout], (uint32_t)_mm_extract_epi32(r01, 1));
    store_u32(&out[2 * incout], (uint32_t)_mm_extract_epi32(r23, 0));
    store_u32(&out[3 * incout], (uint32_t)_mm_extract_epi32(r23, 1));
}

#undef ADD5
#undef ADD25
#undef ADD2
#undef ADD1
