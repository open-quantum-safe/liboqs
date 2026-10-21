#include <assert.h>
#include <string.h>
#include <immintrin.h>
#include "qruov.h"
#include "helpers.h"
#include "kernel.h"
#include "qruov_simd.h"
#include "emi_transform.h"
#include "fastop.h"

void fql_soa_to_aos_permute(uint8_t dst_aos[], const uint8_t src_soa[], int items, int plane)
{
    for (int i = 0; i < items; i++) {
        for (int j = 0; j < QRUOV_L; j++) {
            dst_aos[i * QRUOV_L + QRUOV_perm(j)] = src_soa[j * plane + i];
        }
    }
}

void fql_soa_to_aos_permute_double(uint8_t dst_aos[], const uint8_t src_soa[], int items, int plane)
{
    for (int i = 0; i < items; i++) {
        for (int j = 0; j < QRUOV_L; j++) {
            uint8_t t = src_soa[j * plane + i];
            t = (uint8_t)(t + t);
            if (t >= QRUOV_q) t = (uint8_t)(t - QRUOV_q);
            dst_aos[i * QRUOV_L + QRUOV_perm(j)] = t;
        }
    }
}

void fql_aos_to_soa(uint8_t dst_soa[], const uint8_t src_aos[], int items, int plane)
{
    for (int i = 0; i < items; i++) {
        for (int j = 0; j < QRUOV_L; j++) {
            dst_soa[j * plane + i] = src_aos[i * QRUOV_L + j];
        }
    }
}

void fql_matmul_vnni_fast(uint8_t C[], const uint8_t A[], const uint8_t Bvnni[], int M, int N, int K)
{
    assert(K % 4 == 0);
    assert(N == 6 || N == 7 || N >= 8);
    const int incout  = N;
    const int incA    = K;
    const int incB    = N;
    const int incLout = M * N;
    const int incLA   = M * K;
    const int incLB   = K * N;
    if (N == 7) {
        for (int i = 0; i < M; i++) {
            int j = 0;
            matmulfql_vnni_1x7(&C[i * N + j], &A[i * K], &Bvnni[j * 4], K, incout, incA, incB, incLout, incLA, incLB);
        }
        return;
    }
    if (N == 6) {
        for (int i = 0; i < M; i++) {
            int j = 0;
            matmulfql_vnni_1x6(&C[i * N + j], &A[i * K], &Bvnni[j * 4], K, incout, incA, incB, incLout, incLA, incLB);
        }
        return;
    }
    for (int i = 0; i < M; i++) {
        int j = 0;
        for (; j + 8 <= N; j += 8) {
            matmulfql_vnni_1x8(&C[i * N + j], &A[i * K], &Bvnni[j * 4], K, incout, incA, incB, incLout, incLA, incLB);
        }
        int rem = N - j;
        if (rem > 0) {
            if (rem <= 4) {
                j = N - 4;
                matmulfql_vnni_1x4(&C[i * N + j], &A[i * K], &Bvnni[j * 4], K, incout, incA, incB, incLout, incLA, incLB);
            } else {
                j = N - 8;
                matmulfql_vnni_1x8(&C[i * N + j], &A[i * K], &Bvnni[j * 4], K, incout, incA, incB, incLout, incLA, incLB);
            }
        }
    }
}

#if QRUOV_L == 3
static void fql_aos_to_vnni_L3_impl(uint8_t Avnni[], int M, int N, const uint8_t bytes[], int origM, int origN)
{
    assert(M % 4 == 0);
    assert(M >= origM && N >= origN);
    if (origM <= 0 || origN <= 0) {
        return;
    }
    const int plane = M * N;
    int i = 0;
    const int stride = origN * QRUOV_L;
    const __m128i INTL4 = _mm_setr_epi8(0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15);
    for (; i + 3 < origM; i += 4) {
        int j = 0;
        for (; j < origN; j += 4) {
            if (j + 3 >= origN) j = origN - 4;
            const uint8_t *input = &bytes[(i * origN + j) * QRUOV_L];
            __m128i a0 = _mm_loadu_si128((__m128i*)&input[0 * stride]);
            __m128i a1 = _mm_loadu_si128((__m128i*)&input[1 * stride]);
            __m128i a2 = _mm_loadu_si128((__m128i*)&input[2 * stride]);
            __m128i a3 = _mm_loadu_si128((__m128i*)&input[3 * stride]);
            __m128i y0, y1, y2;
            deint3_transpose(a0, a1, a2, a3, &y0, &y1, &y2);
            uint8_t *vnni = &Avnni[i / 4 * N * 4 + j * 4];
            _mm_storeu_si128((__m128i*)&vnni[0 * plane], _mm_shuffle_epi8(y0, INTL4));
            _mm_storeu_si128((__m128i*)&vnni[1 * plane], _mm_shuffle_epi8(y1, INTL4));
            _mm_storeu_si128((__m128i*)&vnni[2 * plane], _mm_shuffle_epi8(y2, INTL4));
        }
    }
    if (i < origM) {
        const int rem = origM - i;
        for (int j = 0; j < origN; j++) {
            const uint8_t *input = &bytes[(i * origN + j) * QRUOV_L];
            const size_t dst_base = (size_t)(i >> 2) * (size_t)N * 4u + (size_t)j * 4u;
            for (int l = 0; l < QRUOV_L; l++) {
                uint32_t v = input[l];
                if (rem > 1) v |= (uint32_t)input[stride + l] << 8;
                if (rem > 2) v |= (uint32_t)input[2 * stride + l] << 16;
                store_u32(&Avnni[(size_t)l * (size_t)plane + dst_base], v);
            }
        }
    }
}
#elif QRUOV_L == 10
static void fql_aos_to_vnni_L10_impl(uint8_t Avnni[], int M, int N, const uint8_t bytes[], int origM, int origN)
{
    assert(M % 4 == 0);
    assert(M >= origM && N >= origN);
    if (origM <= 0 || origN <= 0) {
        return;
    }
    const int plane = M * N;
    int i = 0;
    const int stride = origN * QRUOV_L;
    const __m128i INTL4 = _mm_setr_epi8(0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15);
    uint8_t buffer[160], buffer2[160];
    for (; i + 3 < origM; i += 4) {
        int j = 0;
        for (; j < origN; j += 4) {
            if (j + 3 >= origN) j = origN - 4;
            const uint8_t *input = &bytes[(i * origN + j) * QRUOV_L];
            memcpy(&buffer[0 * 4 * QRUOV_L], &input[0 * stride], QRUOV_L * 4);
            memcpy(&buffer[1 * 4 * QRUOV_L], &input[1 * stride], QRUOV_L * 4);
            memcpy(&buffer[2 * 4 * QRUOV_L], &input[2 * stride], QRUOV_L * 4);
            memcpy(&buffer[3 * 4 * QRUOV_L], &input[3 * stride], QRUOV_L * 4);
            transpose16x10(buffer2, buffer);
            uint8_t *vnni = &Avnni[i / 4 * N * 4 + j * 4];
            for (int l = 0; l < QRUOV_L; l++) {
                __m128i y = _mm_loadu_si128((__m128i*)&buffer2[l * 16]);
                _mm_storeu_si128((__m128i*)&vnni[l * plane], _mm_shuffle_epi8(y, INTL4));
            }
        }
    }
    if (i < origM) {
        const int rem = origM - i;
        for (int j = 0; j < origN; j++) {
            const uint8_t *input = &bytes[(i * origN + j) * QRUOV_L];
            const size_t dst_base = (size_t)(i >> 2) * (size_t)N * 4u + (size_t)j * 4u;
            for (int l = 0; l < QRUOV_L; l++) {
                uint32_t v = input[l];
                if (rem > 1) v |= (uint32_t)input[stride + l] << 8;
                if (rem > 2) v |= (uint32_t)input[2 * stride + l] << 16;
                store_u32(&Avnni[(size_t)l * (size_t)plane + dst_base], v);
            }
        }
    }
}
#else
#error "Unknown QRUOV_L"
#endif

void fql_aos_to_vnni(uint8_t Avnni[], int M, int N, const uint8_t bytes[], int origM, int origN)
{
#if QRUOV_L == 3
    fql_aos_to_vnni_L3_impl(Avnni, M, N, bytes, origM, origN);
#elif QRUOV_L == 10
    fql_aos_to_vnni_L10_impl(Avnni, M, N, bytes, origM, origN);
#else
#error "Unknown QRUOV_L"
#endif
}

#if QRUOV_L == 3
static void fql_compact_sym_aos_to_vnni_L3_impl(uint8_t Avnni[], int M, int N, const uint8_t bytes[], int origM)
{
    assert(M % 4 == 0);
    assert(M >= origM && N >= origM);
    int i = 0;
    for (; i + 3 < origM; i += 4) {
        const __m128i INTL4 = _mm_setr_epi8(0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15);
        const __m128i SYMM = _mm_setr_epi8(0, 1, 2, 3, 1, 4, 5, 6, 2, 5, 8, 9, 3, 6, 9, 12);
        const int stride = origM - i;
        const int plane = M * N;
        {
            const uint8_t *input = &bytes[SYMM_COMPACT_INDEX(i, i, origM) * QRUOV_L];
            __m128i a0 = _mm_loadu_si128((__m128i*)&input[0]);
            __m128i a1 = _mm_loadu_si128((__m128i*)&input[(1 * stride) * 3]);
            __m128i a2 = _mm_loadu_si128((__m128i*)&input[(2 * stride - 1) * 3]);
            __m128i a3 = _mm_loadu_si128((__m128i*)&input[(3 * stride - 3) * 3]);
            __m128i y0, y1, y2;
            deint3_transpose(a0, a1, a2, a3, &y0, &y1, &y2);
            uint8_t *symm = &Avnni[i / 4 * N * 4 + i * 4];
            _mm_storeu_si128((__m128i*)&symm[0 * plane], _mm_shuffle_epi8(y0, SYMM));
            _mm_storeu_si128((__m128i*)&symm[1 * plane], _mm_shuffle_epi8(y1, SYMM));
            _mm_storeu_si128((__m128i*)&symm[2 * plane], _mm_shuffle_epi8(y2, SYMM));
        }
        int j = i + 4;
        for (; j + 3 < origM; j += 4) {
            const uint8_t *input = &bytes[SYMM_COMPACT_INDEX(i, j, origM) * QRUOV_L];
            __m128i a0 = _mm_loadu_si128((__m128i*)&input[0]);
            __m128i a1 = _mm_loadu_si128((__m128i*)&input[(1 * stride - 1) * 3]);
            __m128i a2 = _mm_loadu_si128((__m128i*)&input[(2 * stride - 3) * 3]);
            __m128i a3 = _mm_loadu_si128((__m128i*)&input[(3 * stride - 6) * 3]);
            __m128i y0, y1, y2;
            deint3_transpose(a0, a1, a2, a3, &y0, &y1, &y2);
            uint8_t *trans = &Avnni[j / 4 * N * 4 + i * 4];
            uint8_t *copy = &Avnni[i / 4 * N * 4 + j * 4];
            _mm_storeu_si128((__m128i*)&trans[0 * plane], y0);
            _mm_storeu_si128((__m128i*)&trans[1 * plane], y1);
            _mm_storeu_si128((__m128i*)&trans[2 * plane], y2);
            _mm_storeu_si128((__m128i*)&copy[0 * plane], _mm_shuffle_epi8(y0, INTL4));
            _mm_storeu_si128((__m128i*)&copy[1 * plane], _mm_shuffle_epi8(y1, INTL4));
            _mm_storeu_si128((__m128i*)&copy[2 * plane], _mm_shuffle_epi8(y2, INTL4));
        }
        for (; j < origM; j++) {
            for (int i0 = i; i0 < i + 4; i0++) {
                for (int l = 0; l < QRUOV_L; l++) {
                    Avnni[l * M * N + i0 / 4 * N * 4 +  j * 4 + i0 % 4] =
                    Avnni[l * M * N +  j / 4 * N * 4 + i0 * 4 +  j % 4] =
                        bytes[SYMM_COMPACT_INDEX(i0, j, origM) * QRUOV_L + l];
                }
            }
        }
    }
    for (; i < origM; i++) {
        for (int j = i; j < origM; j++) {
            for (int l = 0; l < QRUOV_L; l++) {
                Avnni[l * M * N + i / 4 * N * 4 + j * 4 + i % 4] =
                Avnni[l * M * N + j / 4 * N * 4 + i * 4 + j % 4] =
                    bytes[SYMM_COMPACT_INDEX(i, j, origM) * QRUOV_L + l];
            }
        }
    }
}
#endif

#if QRUOV_L == 10
static void fql_compact_sym_aos_to_vnni_L10_impl(uint8_t Avnni[], int M, int N, const uint8_t bytes[], int origM)
{
    assert(M % 4 == 0);
    assert(M >= origM && N >= origM);
    uint8_t buffer[160] = {0}, buffer2[160];
    int i = 0;
    for (; i + 3 < origM; i += 4) {
        const __m128i INTL4 = _mm_setr_epi8(0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15);
        const __m128i SYMM = _mm_setr_epi8(0, 1, 2, 3, 1, 4, 5, 6, 2, 5, 8, 9, 3, 6, 9, 12);
        const int stride = origM - i;
        const int plane = M * N;
        {
            const uint8_t *input = &bytes[SYMM_COMPACT_INDEX(i, i, origM) * QRUOV_L];
            memcpy(&buffer[0 * 4 * QRUOV_L], &input[0], QRUOV_L * 4);
            memcpy(&buffer[1 * 4 * QRUOV_L], &input[(1 * stride) * QRUOV_L], QRUOV_L * 4);
            memcpy(&buffer[2 * 4 * QRUOV_L], &input[(2 * stride - 1) * QRUOV_L], QRUOV_L * 4);
            memcpy(&buffer[3 * 4 * QRUOV_L], &input[(3 * stride - 3) * QRUOV_L], QRUOV_L * 4);
            transpose16x10(buffer2, buffer);
            uint8_t *symm = &Avnni[i / 4 * N * 4 + i * 4];
            for (int l = 0; l < QRUOV_L; l++) {
                __m128i y = _mm_loadu_si128((__m128i*)&buffer2[l * 16]);
                _mm_storeu_si128((__m128i*)&symm[l * plane], _mm_shuffle_epi8(y, SYMM));
            }
        }
        int j = i + 4;
        for (; j + 3 < origM; j += 4) {
            const uint8_t *input = &bytes[SYMM_COMPACT_INDEX(i, j, origM) * QRUOV_L];
            memcpy(&buffer[0 * 4 * QRUOV_L], &input[0], QRUOV_L * 4);
            memcpy(&buffer[1 * 4 * QRUOV_L], &input[(1 * stride - 1) * QRUOV_L], QRUOV_L * 4);
            memcpy(&buffer[2 * 4 * QRUOV_L], &input[(2 * stride - 3) * QRUOV_L], QRUOV_L * 4);
            memcpy(&buffer[3 * 4 * QRUOV_L], &input[(3 * stride - 6) * QRUOV_L], QRUOV_L * 4);
            transpose16x10(buffer2, buffer);
            uint8_t *trans = &Avnni[j / 4 * N * 4 + i * 4];
            uint8_t *copy = &Avnni[i / 4 * N * 4 + j * 4];
            for (int l = 0; l < QRUOV_L; l++) {
                __m128i y = _mm_loadu_si128((__m128i*)&buffer2[l * 16]);
                _mm_storeu_si128((__m128i*)&trans[l * plane], y);
                _mm_storeu_si128((__m128i*)&copy[l * plane], _mm_shuffle_epi8(y, INTL4));
            }
        }
        for (; j < origM; j++) {
            for (int i0 = i; i0 < i + 4; i0++) {
                for (int l = 0; l < QRUOV_L; l++) {
                    Avnni[l * M * N + i0 / 4 * N * 4 +  j * 4 + i0 % 4] =
                    Avnni[l * M * N +  j / 4 * N * 4 + i0 * 4 +  j % 4] =
                        bytes[SYMM_COMPACT_INDEX(i0, j, origM) * QRUOV_L + l];
                }
            }
        }
    }
    for (; i < origM; i++) {
        for (int j = i; j < origM; j++) {
            for (int l = 0; l < QRUOV_L; l++) {
                Avnni[l * M * N + i / 4 * N * 4 + j * 4 + i % 4] =
                Avnni[l * M * N + j / 4 * N * 4 + i * 4 + j % 4] =
                    bytes[SYMM_COMPACT_INDEX(i, j, origM) * QRUOV_L + l];
            }
        }
    }
}
#endif

#if QRUOV_L == 3
void fql_compact_sym_aos_to_vnni(uint8_t Avnni[], int M, int N, const uint8_t bytes[], int origM)
{
    fql_compact_sym_aos_to_vnni_L3_impl(Avnni, M, N, bytes, origM);
}
#elif QRUOV_L == 10
void fql_compact_sym_aos_to_vnni(uint8_t Avnni[], int M, int N, const uint8_t bytes[], int origM)
{
    fql_compact_sym_aos_to_vnni_L10_impl(Avnni, M, N, bytes, origM);
}
#else
#error "unknown QRUOV_L"
#endif

static void transpose_mat(uint8_t *B, const uint8_t *A, int ldB, int ldA, int M, int N)
{
    int i = 0;
    for (; i + 7 < M; i += 8) {
        int j = 0;
        for (; j + 7 < N; j += 8) {
            transpose8x8(&B[j * ldB + i], &A[i * ldA + j], ldB, ldA);
        }
        for (; j < N; j++) {
            B[j * ldB + i + 0] = A[(i + 0) * ldA + j];
            B[j * ldB + i + 1] = A[(i + 1) * ldA + j];
            B[j * ldB + i + 2] = A[(i + 2) * ldA + j];
            B[j * ldB + i + 3] = A[(i + 3) * ldA + j];
            B[j * ldB + i + 4] = A[(i + 4) * ldA + j];
            B[j * ldB + i + 5] = A[(i + 5) * ldA + j];
            B[j * ldB + i + 6] = A[(i + 6) * ldA + j];
            B[j * ldB + i + 7] = A[(i + 7) * ldA + j];
        }
    }
    for (; i < M; i++) {
        for (int j = 0; j < N; j++) {
            B[j * ldB + i] = A[i * ldA + j];
        }
    }
}

void fq_eval_transpose_planes(uint8_t B[], const uint8_t A[], int planeB, int planeA, int ldB, int ldA, int M, int N)
{
    for (int l = 0; l < QRUOV_LL; l++) {
        transpose_mat(&B[l * planeB], &A[l * planeA], ldB, ldA, M, N);
    }
}

static void fql_aos_to_transposed_soa(uint8_t A[], int M, int N, const uint8_t bytes[], int origM, int origN)
{
    assert(M >= origN && N >= origM);
    for (int i = 0; i < origM; i++) {
        for (int j = 0; j < origN; j++) {
            for (int k = 0; k < QRUOV_L; k++) {
                A[k * M * N + j * N + i] = bytes[i * origN * QRUOV_L + j * QRUOV_L + k];
            }
        }
    }
}

void fql_matmul_eval_vnni_fast(uint8_t C[], const uint8_t A[], const uint8_t Bvnni[], int M, int N, int K)
{
    assert(K % 4 == 0);
    assert(N >= 4);
    for (int l = 0; l < QRUOV_LL; l++) {
        for (int i = 0; i < M; i += 4) {
            if (i + 3 >= M) i = M - 4;
            int j = 0;
            for (; j + 15 < N; j += 16) {
                matmulfq_vnni_4x16(&C[l * M * N + i * N + j], &A[l * M * K + i * K],
                                   &Bvnni[l * K * N + j * 4], K, N, K, 4 * N);
            }
            if (j + 7 < N) {
                matmulfq_vnni_4x8(&C[l * M * N + i * N + j], &A[l * M * K + i * K],
                                  &Bvnni[l * K * N + j * 4], K, N, K, 4 * N);
                j += 8;
            }
            int rem = N - j;
            if (rem == 7 && N == 7) {
                matmulfq_vnni_4x7(&C[l * M * N + i * N + j], &A[l * M * K + i * K],
                                  &Bvnni[l * K * N + j * 4], K, N, K, 4 * N);
            } else if (rem == 6 && N == 6) {
                matmulfq_vnni_4x6(&C[l * M * N + i * N + j], &A[l * M * K + i * K],
                                  &Bvnni[l * K * N + j * 4], K, N, K, 4 * N);
            } else if (rem >= 5 && N >= 8) {
                j = N - 8;
                matmulfq_vnni_4x8(&C[l * M * N + i * N + j], &A[l * M * K + i * K],
                                  &Bvnni[l * K * N + j * 4], K, N, K, 4 * N);
            } else if (rem >= 5) {
                matmulfq_vnni_4x4(&C[l * M * N + i * N + j], &A[l * M * K + i * K],
                                  &Bvnni[l * K * N + j * 4], K, N, K, 4 * N);
                j = N - 4;
                matmulfq_vnni_4x4(&C[l * M * N + i * N + j], &A[l * M * K + i * K],
                                  &Bvnni[l * K * N + j * 4], K, N, K, 4 * N);
            } else if (rem >= 1) {
                j = N - 4;
                matmulfq_vnni_4x4(&C[l * M * N + i * N + j], &A[l * M * K + i * K],
                                  &Bvnni[l * K * N + j * 4], K, N, K, 4 * N);
            }
        }
    }
}

void fql_compact_sym_aos_to_eval_vnni(uint8_t Avnni[], int M, int N, const uint8_t bytes[], int origM)
{
    assert(M % 4 == 0);
    assert(M >= origM && N >= origM);
    uint8_t tmp[QRUOV_L * M * N];
    memset(tmp, 0, sizeof(tmp));
    fql_compact_sym_aos_to_vnni(tmp, M, N, bytes, origM);
    evaluate(Avnni, M * N, tmp, M * N, M * N);
}

void fql_aos_to_eval_vnni(uint8_t Avnni[], int M, int N, const uint8_t bytes[], int origM, int origN)
{
    assert(M % 4 == 0);
    assert(M >= origM && N >= origN);
    uint8_t tmp[QRUOV_L * M * N];
    if (N != origN) memset(tmp, 0, sizeof(tmp));
    fql_aos_to_vnni(tmp, M, N, bytes, origM, origN);
    evaluate(Avnni, M * N, tmp, M * N, M * N);
}

void fql_aos_to_eval_transposed_soa(uint8_t A[], int M, int N, const uint8_t bytes[], int origM, int origN)
{
    assert(M >= origN && N >= origM);
    uint8_t tmp[QRUOV_L * M * N];
    memset(tmp, 0, sizeof(tmp));
    fql_aos_to_transposed_soa(tmp, M, N, bytes, origM, origN);
    evaluate(A, M * N, tmp, M * N, M * N);
}
