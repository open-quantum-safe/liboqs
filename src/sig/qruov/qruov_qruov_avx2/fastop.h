#ifndef FASTOP_H
#define FASTOP_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <immintrin.h>
#include "qruov.h"
#include "qruov_simd.h"

extern uint8_t qruov_avx2_bextmask[1u << 16][16];
extern uint8_t qruov_avx2_bdepmask[1u << 16][16];

static inline size_t fastop_rejsamp_prep(unsigned int length, const uint8_t *src, uint8_t *dst,
                                         const uint8_t bextmask[][16])
{
    uint8_t *dst0 = dst;
    int i = 0;
    for (; i + 15 < (int)length; i += 16) {
        __m128i buf = _mm_loadu_si128((const __m128i *)src);
        buf = _mm_and_si128(buf, _mm_set1_epi8(QRUOV_q));
        int mask = 0xFFFF ^ _mm_movemask_epi8(_mm_cmpeq_epi8(buf, _mm_set1_epi8(QRUOV_q)));
        buf = _mm_shuffle_epi8(buf, _mm_load_si128((const __m128i *)bextmask[mask]));
        _mm_storeu_si128((__m128i *)dst, buf);
        src += 16;
        dst += __builtin_popcount((unsigned int)mask);
    }
    for (; i < (int)length; i++) {
        uint8_t v = (uint8_t)(*src & QRUOV_q);
        if (v != QRUOV_q) *dst++ = v;
        src++;
    }
    return (size_t)(dst - dst0);
}

static inline void fastop_rejection_sample(uint8_t *dst, int tau, int length)
{
    const int tail_len = tau - length;
    uint8_t tmp[tail_len + 16];
    size_t written = fastop_rejsamp_prep((unsigned int)tail_len, dst + length, tmp,
                                         (const uint8_t (*)[16])qruov_avx2_bextmask);
    memset(tmp + written, 0, 16);

    uint8_t *ptmp = tmp;
    uint8_t *tmp_end = tmp + written;
    int i = 0;
    for (; i + 15 < length && ptmp <= tmp_end; i += 16) {
        __m128i inp = _mm_loadu_si128((__m128i *)&dst[i]);
        __m128i mux = _mm_loadu_si128((__m128i *)ptmp);
        inp = _mm_and_si128(inp, _mm_set1_epi8(QRUOV_q));
        __m128i q = _mm_cmpeq_epi8(inp, _mm_set1_epi8(QRUOV_q));
        int mask = _mm_movemask_epi8(q);
        __m128i ans = _mm_or_si128(_mm_andnot_si128(q, inp),
                                   _mm_shuffle_epi8(mux, _mm_load_si128((__m128i *)qruov_avx2_bdepmask[mask])));
        _mm_storeu_si128((__m128i *)&dst[i], ans);
        ptmp += __builtin_popcount((unsigned int)mask);
    }
    for (; i < length; i++) {
        int val = dst[i] & QRUOV_q;
        if (val == QRUOV_q) {
            dst[i] = (ptmp < tmp_end) ? *ptmp++ : 0;
        } else {
            dst[i] = (uint8_t)val;
        }
    }
}

static inline uint8_t fastop_rejection_dotprod(unsigned int length, unsigned int tau,
                                               uint8_t *src, const uint8_t *vec)
{
    const unsigned int tail_len = tau - length;
    uint8_t tmp[tail_len + 32];
    size_t written = fastop_rejsamp_prep(tail_len, src + length, tmp,
                                         (const uint8_t (*)[16])qruov_avx2_bextmask);
    memset(tmp + written, 0, 32);

    enum { FASTPATH = QRUOV_q == 127 };
    const uint8_t *ptmp = tmp;
    const uint8_t *tmp_end = tmp + written;
    int i = 0;
    __m256i acc = _mm256_setzero_si256();
    for (; i + 31 < (int)length && ptmp <= tmp_end; i += 32) {
        __m256i inp = _mm256_loadu_si256((const __m256i *)&src[i]);
        inp = _mm256_and_si256(inp, _mm256_set1_epi8(QRUOV_q));
        __m256i q = _mm256_cmpeq_epi8(inp, _mm256_set1_epi8(QRUOV_q));
        __m256i v = _mm256_loadu_si256((const __m256i *)&vec[i]);
        uint32_t mask = (uint32_t)_mm256_movemask_epi8(q);
        if (FASTPATH && mask == 0) {
            acc = ymm_dot_u8s8(acc, inp, v);
            continue;
        }
        int c0 = __builtin_popcount(mask & 0xFFFFu);
        __m128i mux0 = _mm_loadu_si128((const __m128i *)ptmp);
        __m128i mux1 = _mm_loadu_si128((const __m128i *)(ptmp + c0));
        __m128i msk0 = _mm_load_si128((const __m128i *)qruov_avx2_bdepmask[mask & 0xFFFFu]);
        __m128i msk1 = _mm_load_si128((const __m128i *)qruov_avx2_bdepmask[mask >> 16]);
        __m256i mux = _mm256_set_m128i(mux1, mux0);
        __m256i msk = _mm256_set_m128i(msk1, msk0);
        __m256i ans = _mm256_or_si256(_mm256_andnot_si256(q, inp), _mm256_shuffle_epi8(mux, msk));
        acc = ymm_dot_u8s8(acc, ans, v);
        ptmp += __builtin_popcount(mask);
    }
    if (i + 15 < (int)length && ptmp <= tmp_end) {
        __m128i inp = _mm_loadu_si128((const __m128i *)&src[i]);
        inp = _mm_and_si128(inp, _mm_set1_epi8(QRUOV_q));
        __m128i q = _mm_cmpeq_epi8(inp, _mm_set1_epi8(QRUOV_q));
        uint16_t mask = _mm_movemask_epi8(q);
        __m128i mux = _mm_loadu_si128((const __m128i *)ptmp);
        __m128i msk = _mm_load_si128((const __m128i *)qruov_avx2_bdepmask[mask & 0xFFFFu]);
        __m128i ans = _mm_or_si128(_mm_andnot_si128(q, inp), _mm_shuffle_epi8(mux, msk));
        __m128i v = _mm_loadu_si128((const __m128i *)&vec[i]);
        __m128i pair16 = _mm_maddubs_epi16(ans, v);
        __m128i part32 = _mm_madd_epi16(_mm_set1_epi16(1), pair16);
        acc = _mm256_add_epi32(acc, _mm256_set_m128i(_mm_setzero_si128(), part32));
        ptmp += __builtin_popcount(mask);
        i += 16;
    }
    int sum = ymm_reduce_add_epi32(acc);
    for (; i < (int)length; i++) {
        int val = src[i] & QRUOV_q;
        if (val == QRUOV_q) {
            sum += ((ptmp < tmp_end) ? *ptmp++ : 0) * vec[i];
        } else {
            sum += val * vec[i];
        }
    }
    return (uint8_t)(sum % QRUOV_q);
}

static inline uint8_t fastop_dotprod(unsigned int length, const uint8_t *a, const uint8_t *b)
{
    unsigned int i = 0;
    __m256i acc = _mm256_setzero_si256();

    for (; i + 31 < length; i += 32) {
        __m256i av = _mm256_loadu_si256((const __m256i *)&a[i]);
        __m256i bv = _mm256_loadu_si256((const __m256i *)&b[i]);
        acc = ymm_dot_u8s8(acc, av, bv);
    }

    uint32_t sum = (uint32_t)ymm_reduce_add_epi32(acc);
    for (; i < length; i++) {
        sum += (uint32_t)a[i] * b[i];
    }
    return (uint8_t)(sum % QRUOV_q);
}

static inline void fastop_row_elim(uint8_t *dst, const uint8_t *pivot, uint8_t m_mul, int len)
{
    int t = 0;
    __m256i m_mul_v = _mm256_set1_epi16(m_mul);
    for (; t + 15 < len; t += 16) {
        __m256i dst_v = _mm256_cvtepi8_epi16(_mm_loadu_si128((const __m128i *)&dst[t]));
        __m256i piv_v = _mm256_cvtepi8_epi16(_mm_loadu_si128((const __m128i *)&pivot[t]));
        __m256i tmp = _mm256_add_epi16(dst_v, _mm256_mullo_epi16(m_mul_v, piv_v));
        tmp = ymm_modq_epi15(tmp);
        _mm_storeu_si128((__m128i *)&dst[t], ymm_cvtusepi16_epi8(tmp));
    }
    for (; t < len; t++) {
        dst[t] = (uint8_t)(((uint32_t)dst[t] + (uint32_t)m_mul * pivot[t]) % QRUOV_q);
    }
}

static inline void fastop_store_fq(uint8_t *dst, const uint8_t *src, size_t src_len)
{
    size_t i = 0;
    size_t out = 0;
    uint32_t acc = 0;
    unsigned bits = 0;

#if defined(__BMI2__)
    {
        const size_t FQ_IN_64 = (64u + QRUOV_q_LOG - 1u) / QRUOV_q_LOG;
        const uint64_t q = QRUOV_q;
        const uint64_t mask = q | (q << 8) | (q << 16) | (q << 24) |
                              (q << 32) | (q << 40) | (q << 48) | (q << 56);
        while (src_len - i >= FQ_IN_64) {
            uint64_t mem = _pext_u64(load_u64(&src[i]), mask);
            memcpy(&dst[out], &mem, QRUOV_q_LOG);
            i += 8;
            out += QRUOV_q_LOG;
        }
    }
#endif

    while (i < src_len) {
        acc |= (uint32_t)src[i++] << bits;
        bits += QRUOV_q_LOG;
        while (bits >= 8) {
            dst[out++] = (uint8_t)acc;
            acc >>= 8;
            bits -= 8;
        }
    }
    if (bits > 0) dst[out] = (uint8_t)acc;
}

static inline void fastop_load_fq(uint8_t *dst, const uint8_t *src, size_t dst_len)
{
    size_t i = 0;
    size_t in = 0;
    uint32_t acc = 0;
    unsigned bits = 0;
    const uint32_t mask32 = (1u << QRUOV_q_LOG) - 1u;

#if defined(__BMI2__)
    {
        const size_t FQ_IN_64 = (64u + QRUOV_q_LOG - 1u) / QRUOV_q_LOG;
        const uint64_t q = QRUOV_q;
        const uint64_t mask = q | (q << 8) | (q << 16) | (q << 24) |
                              (q << 32) | (q << 40) | (q << 48) | (q << 56);
        while (dst_len - i >= FQ_IN_64) {
            uint64_t packed = load_u64(&src[in]);
            uint64_t mem = _pdep_u64(packed, mask);
            store_u64(&dst[i], mem);
            i += 8;
            in += QRUOV_q_LOG;
        }
    }
#endif

    while (i < dst_len) {
        while (bits < QRUOV_q_LOG) {
            acc |= ((uint32_t)src[in++]) << bits;
            bits += 8;
        }
        dst[i++] = (uint8_t)(acc & mask32);
        acc >>= QRUOV_q_LOG;
        bits -= QRUOV_q_LOG;
    }
}

#endif // FASTOP_H
