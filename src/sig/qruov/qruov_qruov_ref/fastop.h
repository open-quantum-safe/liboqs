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
    fastop_rejection_sample(src, (int)tau, (int)length);
    for (unsigned int i = 0; i < length; i++) {
        sum += (uint32_t)src[i] * vec[i];
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
    size_t in = 0;
    uint32_t acc = 0;
    unsigned bits = 0;
    const uint32_t mask = (1u << QRUOV_q_LOG) - 1u;
    for (size_t i = 0; i < dst_len; i++) {
        while (bits < QRUOV_q_LOG) {
            acc |= ((uint32_t)src[in++]) << bits;
            bits += 8;
        }
        dst[i] = (uint8_t)(acc & mask);
        acc >>= QRUOV_q_LOG;
        bits -= QRUOV_q_LOG;
    }
}

#endif // FASTOP_H
