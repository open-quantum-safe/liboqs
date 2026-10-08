#ifndef FASTOP_H
#define FASTOP_H

#include <stdint.h>
#include <stddef.h>
#include "qruov.h"

static inline void fastop_rejection_sample(uint8_t *dst, int tau, int length)
{
    for (int i = 0; i < tau; i++) {
        dst[i] &= QRUOV_q;
    }
    const uint8_t *aux = dst + length;
    const uint8_t *aux_end = dst + tau;
    while (aux < aux_end && *aux == QRUOV_q) aux++;
    for (int i = 0; i < length; i++) {
        if (*dst == QRUOV_q) {
            if (aux < aux_end) {
                *dst = *aux++;
                while (aux < aux_end && *aux == QRUOV_q) aux++;
            } else {
                *dst = 0;
            }
        }
        dst++;
    }
}

static inline uint8_t fastop_rejection_dotprod(unsigned int length, unsigned int tau,
                                               uint8_t *src, const uint8_t *vec)
{
    uint32_t sum = 0;
    // src is scratch input here; only the returned dot product is used.
    for (unsigned int i = 0; i < tau; i++) {
        src[i] &= QRUOV_q;
    }
    const uint8_t *aux = src + length;
    const uint8_t *aux_end = src + tau;
    while (aux < aux_end && *aux == QRUOV_q) aux++;
    for (unsigned int i = 0; i < length; i++) {
        uint8_t v = src[i];
        if (v == QRUOV_q) {
            if (aux < aux_end) {
                v = *aux++;
                while (aux < aux_end && *aux == QRUOV_q) aux++;
            } else {
                v = 0;
            }
        }
        sum += (uint32_t)v * vec[i];
    }
    return (uint8_t)(sum % QRUOV_q);
}

static inline uint8_t fastop_dotprod(unsigned int length, const uint8_t *a, const uint8_t *b)
{
    uint32_t sum = 0;
    for (unsigned int i = 0; i < length; i++) {
        sum += (uint32_t)a[i] * b[i];
    }
    return (uint8_t)(sum % QRUOV_q);
}

static inline void fastop_row_elim(uint8_t *dst, const uint8_t *pivot, uint8_t m_mul, int len)
{
    for (int t = 0; t < len; t++) {
        dst[t] = (uint8_t)(((uint32_t)dst[t] + (uint32_t)m_mul * pivot[t]) % QRUOV_q);
    }
}

static inline void fastop_store_fq(uint8_t *dst, const uint8_t *src, size_t src_len)
{
    size_t out = 0;
    uint32_t acc = 0;
    unsigned bits = 0;
    for (size_t i = 0; i < src_len; i++) {
        acc |= (uint32_t)src[i] << bits;
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

#if QRUOV_q_LOG == 3
    while (dst_len - i >= 8) {
        uint32_t packed = (uint32_t)src[in] |
                          ((uint32_t)src[in + 1] << 8) |
                          ((uint32_t)src[in + 2] << 16);
        dst[i + 0] = (uint8_t)(packed & QRUOV_q);
        dst[i + 1] = (uint8_t)((packed >> 3) & QRUOV_q);
        dst[i + 2] = (uint8_t)((packed >> 6) & QRUOV_q);
        dst[i + 3] = (uint8_t)((packed >> 9) & QRUOV_q);
        dst[i + 4] = (uint8_t)((packed >> 12) & QRUOV_q);
        dst[i + 5] = (uint8_t)((packed >> 15) & QRUOV_q);
        dst[i + 6] = (uint8_t)((packed >> 18) & QRUOV_q);
        dst[i + 7] = (uint8_t)((packed >> 21) & QRUOV_q);
        i += 8;
        in += 3;
    }
#elif QRUOV_q_LOG == 5
    while (dst_len - i >= 8) {
        uint64_t packed = (uint64_t)src[in] |
                          ((uint64_t)src[in + 1] << 8) |
                          ((uint64_t)src[in + 2] << 16) |
                          ((uint64_t)src[in + 3] << 24) |
                          ((uint64_t)src[in + 4] << 32);
        dst[i + 0] = (uint8_t)(packed & QRUOV_q);
        dst[i + 1] = (uint8_t)((packed >> 5) & QRUOV_q);
        dst[i + 2] = (uint8_t)((packed >> 10) & QRUOV_q);
        dst[i + 3] = (uint8_t)((packed >> 15) & QRUOV_q);
        dst[i + 4] = (uint8_t)((packed >> 20) & QRUOV_q);
        dst[i + 5] = (uint8_t)((packed >> 25) & QRUOV_q);
        dst[i + 6] = (uint8_t)((packed >> 30) & QRUOV_q);
        dst[i + 7] = (uint8_t)((packed >> 35) & QRUOV_q);
        i += 8;
        in += 5;
    }
#elif QRUOV_q_LOG == 7
    while (dst_len - i >= 8) {
        uint64_t packed = (uint64_t)src[in] |
                          ((uint64_t)src[in + 1] << 8) |
                          ((uint64_t)src[in + 2] << 16) |
                          ((uint64_t)src[in + 3] << 24) |
                          ((uint64_t)src[in + 4] << 32) |
                          ((uint64_t)src[in + 5] << 40) |
                          ((uint64_t)src[in + 6] << 48);
        dst[i + 0] = (uint8_t)(packed & QRUOV_q);
        dst[i + 1] = (uint8_t)((packed >> 7) & QRUOV_q);
        dst[i + 2] = (uint8_t)((packed >> 14) & QRUOV_q);
        dst[i + 3] = (uint8_t)((packed >> 21) & QRUOV_q);
        dst[i + 4] = (uint8_t)((packed >> 28) & QRUOV_q);
        dst[i + 5] = (uint8_t)((packed >> 35) & QRUOV_q);
        dst[i + 6] = (uint8_t)((packed >> 42) & QRUOV_q);
        dst[i + 7] = (uint8_t)((packed >> 49) & QRUOV_q);
        i += 8;
        in += 7;
    }
#endif

    for (; i < dst_len; i++) {
        while (bits < QRUOV_q_LOG) {
            acc |= ((uint32_t)src[in++]) << bits;
            bits += 8;
        }
        dst[i] = (uint8_t)(acc & QRUOV_q);
        acc >>= QRUOV_q_LOG;
        bits -= QRUOV_q_LOG;
    }
}

#endif // FASTOP_H
